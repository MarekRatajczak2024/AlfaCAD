
#define __A_PROFILE__

#include <stdlib.h>
#include <errno.h>
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
#endif
#include <sys/stat.h>
#include <allegext.h>

//#pragma pack(4)  //to avoid warning //WARNING !!!!

#include "bib_e.h"
#include "o_bloklb.h"
#include "bib_blok.h"
#include "o_object.h"
#include "rysuj_e.h"
#include "alffont.h"
#include "message.h"
#include "o_loadf.h"

#include "menu.h"

#include "leak_detector_c.h"


#define MaxLen 255
#define max_points 1000

#ifdef LINUX
#define S_IREAD S_IRUSR

#endif

extern void Generowanie_tabeli_wspolrzednych(void);
extern void Wstaw_Profil(void);
extern void Zapisz_Profil(void);
extern void Wstaw_Studnie(void);
extern void SkalaE_S(double skala_e, int dark);
extern void  *select_variable(unsigned *typ);
extern void delay(int time);
void ForceDataToVar(int opcja, int wariant);
extern BOOL Check_if_LE (double x, double y);
extern BOOL Check_if_Equal (double x, double y);
extern void GetTextLen (TEXT *ptrs_text, double *tdl);
extern int get_block_layer(BLOK *b);
extern char *find_obj(char *adrp, char *adrk, int obiekt, int typ_obiektu, int info_p);
extern BOOL get_special_lines_and_texts (char  **adp_l,char  **adk_l);
extern int	delete_block (void);
extern BOOL get_hektometr_blocks0 (char  **adp_l,char  **adk_l);
extern BOOL Check_if_GE (double x, double y);
extern char *find_block(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu);
//extern char *find_block_sub(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu, int l_char);
extern char *find_block_sub_on(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu, int l_char);
extern BOOL get_texts_hatch_cur_layer (char  **adp_l,char  **adk_l);
extern int Get_First_Second_Pomiar_Param(int przekroj, int kanal, double *xi, double *yi);
extern void InfoListStr0(char *st);
extern void ClearInfo0(void);
extern void layer_info (void);
extern void get_global_coords (double *x0, double *y0, int cartflags);
extern char *Pline_single (double *x_begin, double *y_begin);
extern char *find_obj2(char *adrp, char *adrk, int obiekt1, int obiekt2, int typ_obiektu1, int typ_obiektu2, int info_p);
extern int Get_Net_Param (BLOK **blk);
extern int Pline_single_xy (double *x_begin, double *y_begin, BLOK *blk);
extern void set_b_align(BOOL opcja);
extern void WezSymbol (char * sym_name);
extern void set_b_divide(int opc);
extern void set_df_seg_len(double d);
extern BOOL divide_pline (BLOK *ptrs_block, void *ptr_object, double df_x, double df_y, double odleglosc_00);
extern int count_texts (char  *adp_l, char  *adk_l);
extern char *find_obj_count_abs(char *adrp, char *adrk, int obiekt, int typ_obiektu, int info_p, int count);
extern BOOL get_b_align(void);
extern char *find_obj_in_layer(char *adrp, char *adrk, int obiekt, int typ_obiektu, char obiektt2, int warstwa, int info_p);
extern void Wez_warstwe (void);
extern int Get_Pomiar_Param (void);
extern void WymInterClock(int k);
extern int Get_wym_kata(void);
extern void Set_wym_kata(int i_kat);
extern void obtekst_(char  *adr,double x,double y,double k1,double k2);
extern void kat_w_clock (double kat1, double kat2, double x, double y, double r);
extern T_Font_Header *PTRS__Text_Style [MaxNumberTextStyle] ;
extern BOOL Copy_File(char *ptrsz_fnd, char *ptrsz_fns);

extern void Ini_Layers_Dlg (void);
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);
extern char *get_komunikat_ptr (int n);

extern double Get_Text_Len (TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index, int *t_width, int *t_height, double *matrix_d, int *i_matrix_d);

extern int New_window_sketchbook(void);
extern void Restore_drawing(int drawing_number, int sketch);

extern BOOL get_entire_poly(void);
extern void set_entire_poly(BOOL ent_poly);

extern void color_bar(void);
extern BLOK *select_polyline(void);
extern BOOL Set_Beginning_Pline (BLOK *blk_adr, double *X0, double *Y0);

extern void set_decimal_format(char *text, double l, double precision);
extern void view_scale(void);
extern double Angle_Normal (double angle);
extern void Change_Reference_Angle(void);

extern void obrot_import (void);
extern void obrot_import_kat (double kat_obrotu);
extern void obrot_import_K(void);
extern void	obrot_kopiuj_paral(double kat);
extern void	obrot_kopiuj_paralK(void);
extern void set_Px_Py(double Px_, double Py_);
extern void set_block_angle(double angle);
extern void utf8Upper(char* text);

extern int DRAWING_NUMBER;
extern BOOL Semaphore;
extern double global_reference_angle_rad;
extern void set_angle_l_rect(double angle);

extern TMENU mKatR;

int kreowanie_studni(Record_Clock *record_clock, double del_x, double del_y);

static LINIA LiniaG0;
static LINIA LiniaG01;
static LUK LukG0;
static LUK LukG01;
static OKRAG OkragG0;
static OKRAG OkragG01;
static TEXT_NAG Text_Nag0;
static TEXT_NAG Text_Nag01;
static int current_layer;
static int current_layer1;
static void set_new_instal_tab(double rzedna_terenu_p, double rzedna_terenu_i);
static int get_param1(MyDane *dane_prof0);


char point_net_opis[30]="";
int point_net_typ=100;
double data_instal_last[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double data_instal_tab[20][6];
double tabela_p0_y[20]={8,10,13,14.5,17,18.5,24,34,35.5,41,42.5,48,58,68,78,81,86,91,92.5,0};
double tabela_p1_y[20]={0, 0, 0,   0, 0,   0, 0, 0,   3,10,12.5,20,30,40,50,53,58,63,64.5,0};
double tabela_p2_y[20]={8,10,13,14.5,17,18.5,24,34,35.5,41,42.5,48,58,68,78,81,86,91,92.5,0};
int licowanie_dnem = 0;
int ukrywanie_rtin = 0;
int typ_lin_profil[6]={96,96,96,96,65,64};
int decimal_separator = 0; //point

//static
double data_odwiert[29]={0.};
static double dane_profs_tab[29]={0.};

BOOL sketchbook_exists=0;
static int sketchbook;
static int sketchparent;

static size_t bsize;

double reference_angle=0.0;

BLOK *global_blk=NULL;

//Fi sign alternatives
#define Fi u8"Ø"    //0xC3 0x98
//#define Fi u8"ø"  //0xC3 0xB8  nonexistent in chr fonts
//#define Fi u8"Φ"  //looking not good enough
//#define Fi u8"∅"  //not common among TTF fonts, nonexistent in aff and chr fonts
//#define Fi u8"⌀"  //not common among TTF fonts, nonexistent in aff and chr fonts

typedef struct
{
  int no;
  double kat;
} Katy;

Katy katy[16];

typedef struct
{
  char nazwa[65];
  long adr;
} Profil_Sieci;

typedef struct
{
  double x;
  double y;
  unsigned first : 3;
  unsigned dno_os :1;
  unsigned teren_p :1;
  unsigned wyrownywanie : 2;
  double odleglosc;
  unsigned rezerwa1 :1;
} point_x_y;

typedef struct
{
  double x;
  double y;
} point_x_y_io;

typedef struct
{
  double x;
  double y;
  double yki;   //rzedna dna kanalu istniejacego lub osi dla dno_os_ki==1
  double ykp;   //rzedna dna kanalu projektowanego lub osi dla dno_os_kp==1
  unsigned first_ki :3;
  unsigned first_kp :3;
  unsigned dno_os_ki:1;
  unsigned dno_os_kp:1;
  unsigned teren_p_ki:1;
  unsigned teren_p_kp:1;
  unsigned wyrownywanie_ki :2;
  unsigned wyrownywanie_kp :2;
} point_plus_x_y;

typedef struct
{ char nazwa[16];
  double x;
  double y;
  double z;   //rzedna kanalu  m.n.p.m
} Siec_XYZ;

typedef struct
{ char type[16];
    int no;
    int color;
    int geodetic;
    double scale_xy;
    double origin_x;
    double origin_y;
    double angle_l;
} Siec_XYZ_TYPE_NO;

char **siec_xyz_name;
Siec_XYZ **siec_xyz;
Siec_XYZ *siec_xyz0;
int SIEC_NO_MAX=10;
int siec_no=0;
int SIEC_XYZ_NO_MAX=100;
Siec_XYZ_TYPE_NO *siec_xyz_type_no;
int *siec_xyz_max_no;

point_x_y tab_r[6][1001];

static BOOL Auto_Pan0;

point_x_y_io tab_r_io[1001];
point_x_y_io tab_r_plus_io[2][1001];

point_plus_x_y tab_r_plus[2][1001];
point_plus_x_y tab_r_plus0 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int tab_n[6];
int tab_n_io;
int tab_n_plus_io[2];
int tab_n_plus[2];
static double tdl;

static double delta_pp;
static double old_var=0.0;
static double new_var0=0.0;
static double new_var=0.0;
static double del_var=0.0;

double tablica_y[20];

static char comments_tmp[17][61];
//static
char comments_geo[17][61]={"","","","","","","","","","","","","","","","",""};
static char comments_ins[17][61]={"","","","","","","","","","","","","","","","",""};
static int max_i_tmp = 17;
MyDaneTabl dane_profil={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

MyDane dane_prof;
MyDane dane_profs;
MyDane dane_profs1;
MyDane dane_profs0={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
MyDane dane_global;
MyDane dane_prof00;
MyDane dane_prof0;

int change_variable = 0;
static int i_tmp;
char buf_kom [100];

#define  DF 1.0E+4 /*1.0E+5*/

/*PARBLO*/
#define cursor_off() out_cur(X,Y)
#define cursor_on() out_cur(X,Y)

#define wyrownywanie_standard (-1)
#define wyrownywanie_gora 0
#define wyrownywanie_dolem 1
#define wyrownywanie_osiowe 2

extern int get_lock_prof_aktual(void);
extern void put_lock_prof_aktual(int lock);
extern BOOL file_bak (char *fn, char *fext);
extern void ClearWindow(void);     /*bib_e*/
extern void WymKierR(void);        /*b_wym*/
extern void WymKierX(double kat);  /*b_wym*/
extern int ReadBlock_(char *fn,double *Px,double *Py,RYSPOZ *adp,RYSPOZ *adk, char *buf, int lenmax, int *object_no, BOOL b_current_ver);
//extern void addrem_obiekty(int dwc, OKNO *O, int (*DZI)(void *), int (*ODZI)(void *) );
extern int dzi_off(void *ad);
extern int odzi_off(void *ad);
extern void Explode1(void);
extern BOOL Read_Spec_Block (char *fn,
		     char *buf_name, int i_name_len,
		     char *buf_type, int i_type_len,
		     BOOL b_err);
extern char *File_Ext(char *, char *) ;
extern BOOL Add_String_To_List (char *) ;
extern void Get_Key (void) ;
extern void put_setmarker_force ();
extern void put_findmarker_force ();
extern void put_setmarker (int poz);
extern void put_findmarker (int poz);
/* extern void set_marker (void) ;
extern void find_marker (void); */
//extern void Normalize_Solid (WIELOKAT *) ;
//extern int Get_Current_Color (void) ;

extern BOOL TTF_redraw;
/*
extern BOOL Make_Ellipse (double df_xc,double df_yc,double df_xaxis,double df_yaxis,double df_angle,int i_mode,BLOK **ptrptr_adb,void *ptr_ob,int i_type,int obiektt2,int type,int translucency,int kolor,int kolor0);
*/

BOOL Create_Ellipse (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, int i_mode,BLOK **ptrptr_adb, void *ptr_ob, int layer, int i_type, int obiektt2, int type, int translucency, int kolor, int kolor0)
{
    ELLIPSE e=Edef;
    e.x=(float)df_xc;
    e.y=(float)df_yc;
    e.rx=(float)df_xaxis;
    e.ry=(float)df_yaxis;
    e.angle=(float)df_angle;
    e.obiektt2=obiektt2;
    e.warstwa=layer;
    e.typ=type;
    e.translucency=255;
    e.kolor=kolor;
    e.blok=1;

    dodaj_obiekt(*ptrptr_adb, &e);
    return 1;
}

static char *BufVar=NULL;

static TEXT *POZ;

BOOL get_sketchbook_exists(void)
{
    return sketchbook_exists;
}

void zeroing_My_Dane(void)
{
    memmove(&dane_prof, &dane_profs0, sizeof(MyDane));
    memmove(&dane_profs, &dane_profs0, sizeof(MyDane));
    memmove(&dane_profs1, &dane_profs0, sizeof(MyDane));
    memmove(&dane_global, &dane_profs0, sizeof(MyDane));
    memmove(&dane_prof00, &dane_profs0, sizeof(MyDane));
    memmove(&dane_prof0, &dane_profs0, sizeof(MyDane));
}

BOOL copy_profil_files(void)
{   char sfilename[MAXPATH];
    char dfilename[MAXPATH];
    int  in=sizeof(FileNameAll)/sizeof(FileNameAll[0]);
    for (int i=0; i<in; i++)
    {
        sprintf(sfilename,"%s%s%s.axx",bloki_dir,SSlash,FileNameAll[i]);
        sprintf(dfilename,"%s%s%s#%d.alx",bloki_dir,SSlash,FileNameAll[i],Client_number);

        Copy_File(dfilename, sfilename);
    }
    return 1;
}

/*-------------------------------------------------------*/
/*     funkcje generowania obiektow i wymiarowania       */
/*-------------------------------------------------------*/

int Lin(double x1,double y1,double x2,double y2,int typ, int kolor)
{ LINIA L=Ldef;
  L.warstwa=Current_Layer;
  L.x1=(float)x1; L.y1=(float)y1;
  L.x2=(float)x2; L.y2=(float)y2;
  L.typ=typ;
  L.kolor=kolor;
  L.blok=1;
  L.obiektt1 = 0;
  L.obiektt2 = O2BlockPline;
   if (dodaj_obiekt((BLOK*)dane, &L)==NULL) return 0;
  return 1;
}

int Lin_special(double x1,double y1,double x2,double y2,int typ, int kolor)
{ LINIA L=Ldef;
  L.warstwa=Current_Layer;
  L.x1=(float)x1; L.y1=(float)y1;
  L.x2=(float)x2; L.y2=(float)y2;
  L.typ=typ;
  L.kolor=kolor;
  L.blok=0;
  L.obiektt1 = 0;
  L.obiektt2 = O2BlockSpecial;
   if (dodaj_obiekt( NULL, &L)==NULL) return 0;
  return 1;
}

int Lin_special_b(double x1,double y1,double x2,double y2,int typ, int kolor)
{ LINIA L=Ldef;
  L.warstwa=Current_Layer;
  L.x1=(float)x1; L.y1=(float)y1;
  L.x2=(float)x2; L.y2=(float)y2;
  L.typ=typ;
  L.kolor=kolor;
  L.blok=1;
  L.obiektt1 = 0;
  L.obiektt2 = O2BlockSpecial;
   if (dodaj_obiekt((BLOK*)dane, &L)==NULL) return 0;
  return 1;
}



int Punkt (double df_x, double df_y, int typ_p, int kolor, int obiektt2, int obiektt3)
/*----------------------------------------------------------------------------------*/
{ T_Point p=PointDef;

  p.warstwa=Current_Layer;
  p.kolor=kolor;
  p.blok=1;
  p.obiektt1 = 0;
  p.obiektt2 = obiektt2; //O2BlockPline;
  p.obiektt3 = obiektt3; //O3Dno, O3Os
  p.x = (float)df_x ;
  p.y = (float)df_y ;
  p.typ = typ_p;

  if (dodaj_obiekt ((BLOK*)dane, &p) == NULL) return 0;
  return 1;
}


int Okr(double x,double y,double r, int kolor)
{ OKRAG O=Odef;
  O.warstwa=Current_Layer;
  O.x=(float)x;
  O.y=(float)y;
  O.r=(float)r;
  O.typ=64/*5*/;
  O.kolor=kolor;
  O.blok=1;
  O.obiekt=Ookrag;
  O.obiektt1 = 0;
  O.obiektt2 = O2BlockPline;
  if (dodaj_obiekt((BLOK*)dane, &O)==NULL) return 0;
  return 1;
}

int Kol(double x,double y,double r, int kolor)
{ OKRAG O=Odef;
  O.warstwa=Current_Layer;
  O.x=(float)x;
  O.y=(float)y;
  O.r=(float)r;
  O.typ=64 /*5*/;
  O.kolor=kolor;
  O.blok=1;
  O.obiekt=Okolo;
  O.obiektt1 = 0;
  O.obiektt2 = O2BlockPline;
  if (dodaj_obiekt((BLOK*)dane, &O)==NULL) return 0;
  return 1;
}

int l_kat1_tab[7]={1,0,1,0,1,0,1};
int l_kat2_tab[7]={0,1,0,1,0,1,0};

int Wezyk(double x,double y,int kolor)
/*----------------------------------*/
{ LUK l=ldef;
  int i;

  l.warstwa=Current_Layer;
  l.y=(float)y;
  l.r=0.5f;
  l.typ=32;
  l.kolor=kolor;
  l.blok=1;
  l.obiekt=Oluk;
  l.obiektt1 = 0;
  l.obiektt2 = O2BlockPline;
  for (i=0; i<7; i++)
   {
    l.x=(float)(x-(0.5+i));
    l.kat1=(float)(l_kat1_tab[i]*Pi);
    l.kat2=(float)(l_kat2_tab[i]*Pi);
    if (dodaj_obiekt((BLOK*)dane, &l)==NULL) return 0;
   }
  return 1;
}

int Solid3(double x1,double y1,double x2,double y2,double x3,double y3,int kolor)
/*-----------------------------------------------------------------------------*/
{
  int k ;
  WIELOKAT Solid=Stdef;
  Solid.warstwa = Current_Layer ;
  Solid.blok=1;
  Solid.obiektt1 = 0;
  Solid.obiektt2 = O2BlockPline;
  Solid.kolor = kolor ;
  Solid.lp = 6 ;
  Solid.n = 8 + Solid.lp * sizeof (float) ;

  Solid.xy [0] = (float)x1 ;
  Solid.xy [1] = (float)y1 ;
  Solid.xy [2] = (float)x2 ;
  Solid.xy [3] = (float)y2 ;
  Solid.xy [4] = (float)x3 ;
  Solid.xy [5] = (float)y3 ;
  Normalize_Solid (&Solid) ;
  if (dodaj_obiekt((BLOK*)dane,&Solid)==NULL) return 0;
  return 1 ;
}


int PointS_b (char *adr_b, double x, double y)
/*------------------------------------------*/
{ T_Point Point=PointDef;

  Point.warstwa = Current_Layer ;
  Point.blok = 1;
  Point.obiektt1 = 0;
  Point.obiektt2 = O2BlockSpecial;
  Point.typ=11;  //base point, it was 12 in old version, but now 11 is used as fixed D support point
  Point.x = (float)x ;
  Point.y = (float)y ;
  if (dodaj_obiekt ((BLOK*)adr_b, &Point)==NULL) return 0;
  return 1 ;
}


int WymLinR(int size,...)
{
  va_list argptr;
  double x, y;
  int i, ret=1;
  WymInterF (0);
  WymKierR ();
  WymNowy ();
  va_start (argptr, size);
  for(i=0; i<size; i++)
   {
     x=va_arg (argptr, double);
     y=va_arg (argptr, double);
     if (!WymPunkt (x, y, 0)) { ret=0; break; }
   }
  WymNowy ();
  va_end (argptr);
  return ret;
}

int WymLinX (double kat, int size,...)
{
  va_list argptr;
  double x, y;
  int i, ret=1;
  WymInterF (0);
  WymKierX (kat);
  WymNowy ();
  va_start (argptr, size);
  for(i=0; i<size; i++)
   {
     x=va_arg (argptr, double);
     y=va_arg (argptr, double);
     if (!WymPunkt (x, y, 0)) { ret=0; break; }
   }
  WymNowy ();
  va_end (argptr);
  return ret;
}


/*-------------------------------------------------------*/
/*               Import                                  */
/*-------------------------------------------------------*/

int ImportMat(char *FileName_)
{ int len;
  char buf [300]; //[100];
  double X0,Y0;
  int object_no ;

  X=0; Y=0;
  if(ReadBlock_(FileName_,&X0,&Y0,&ADP,&ADK , buf, MaxLen - 10, &object_no, TRUE)) return 0;
  transformacja_blok(ADP,ADK,X-X0,Y-Y0,0,0,Tprzesuw,0);
//  blokzap(ADP,ADK,Ablok,COPY_PUT,1);
  zmien_atrybut(ADP,ADK,Ablok,Aoblok);
  POZ=(TEXT*)dane;
  //WriteBlock(fn,X0,Y0, blok_name, blok_type, 1);
  if (auto_zoom_block==TRUE) SkalaE_S (0.9,1); //0.95
  return 1;
}

/*---------------- EditPar -------------------------*/

char *text_buf;
char st_buf[256];

char *decimal_dot(char *text)
{
    char *comma;
    if (decimal_separator == 1) {
        comma = strchr(text, ',');
        if (comma != NULL) {
            strcpy(st_buf, text);
            comma = strchr(st_buf, ',');
            if (comma != NULL) {
                *comma = '.';
                return st_buf;
            }
        }
    }
    return text;
}

void decimal(char *text)
{
    char *dot;
    if (decimal_separator == 1) {
        dot = strchr(text, '.');
        if (dot != NULL) {
            *dot = ',';
        }
    }
}

int EditPar(void)   /* funkcja obslugi edycji wymiarowania */
{
    unsigned typ;
    char *adr, *endptr;
    TEXT *t,T,Tp;
    char st[256];
    int retval_no = 1 ;
    double buf_ret [1] ;
    double d ;
    char *bp1;
    int wariant;
    char *wariant_t;
    char wariant_tt[20];
    char *dot;


    typ=Btekst;
    ////out_sel(X,Y);
    //  adr=(char*)select_edwym(&typ);
    adr=(char*)select_variable(&typ);
    if (adr==NULL)
        return 0;
    if (typ==Btekst)
    {
        t=(TEXT*)adr;
        memmove(&Tp,(void *)t,sizeof(NAGLOWEK)+t->n);
        rysuj_obiekt((char*)&Tp,COPY_PUT,0);
        flip_screen();
        delay(50);
        rysuj_obiekt((char*)&Tp,COPY_PUT,1);
        flip_screen();
        CUR_ON(X,Y);
        if (t->typ==n_etykieta_wew)
        {
            if (strcmp(t->text,"Esc")==0)
            {
                rysuj_obiekt((char*)&Tp,COPY_PUT,0);
                flip_screen();
                delay(50);
                rysuj_obiekt((char*)&Tp,COPY_PUT,1);
                flip_screen();
                ////out_sel(X,Y);
                /* InitEP(1); */
                return 1;
            }

            if (strcmp(t->text,"OK")==0)
            {
                rysuj_obiekt((char*)&Tp,COPY_PUT,0);
                flip_screen();
                delay(50);
                rysuj_obiekt((char*)&Tp,COPY_PUT,1);
                flip_screen();
                ////out_sel(X,Y);
                /*   n=VarToBuf(-1);
                       if (n)
                        {InitEP(1);return 2;}
                      else return 0;  */
                return 2;
            }

            if (strncmp(t->text,"(",1)==0)
            {
                //badanie liczby po nawiasie
                wariant_t = strpbrk(t->text, "1234567890");
                if (wariant_t != NULL)
                {
                    strcpy(wariant_tt, wariant_t);
                    wariant=0;
                    wariant = (int)strtol(wariant_tt, &bp1, 10);
                    if (wariant>=0)
                    {
                        wariant-=1;
                        rysuj_obiekt((char*)&Tp,COPY_PUT,0);
                        flip_screen();
                        delay(50);
                        rysuj_obiekt((char*)&Tp,COPY_PUT,1);
                        flip_screen();
                        ////out_sel(X,Y);
                        ForceDataToVar(0,wariant);
                        redraw();
                        return 0;
                    }
                }
            }
        }
        strcpy(st,t->text);

        if ( read_esc(st,65,1) )
        {
            //zmienna
            if (t->typ==n_typ_zmienna)
            {
                /*  dodanie kalkulatora */
                /* char sk [MaxTextLen] = "", *str ;*/
                if (FALSE == calculator (st, &retval_no, buf_ret)  || retval_no < 1)
                {
                    ////out_sel(X,Y);
                    return 0;
                }
                //text returns with point, even if initialized with comma

                d = buf_ret [0] ;
                /*	if (d <= 0)
               {
                ErrList (30) ;
                out_sel(X,Y);
                return ;
               }
               */
                strcpy(st,"");
                sprintf (st, "%.8lg", d) ;
                /*****/
                double value = strtod(st,&endptr);
                if (endptr!=st && *endptr == '\0' && errno != ERANGE)
                {
                    memmove(&T,(void *)t,sizeof(NAGLOWEK)+t->n);

                    decimal(st);

                    strcpy(T.text,st);
                    T.dl=strlen(st);
                    T.n=T18+T.dl;
                    t=(TEXT*)korekta_obiekt((void *)adr,(void*)&T);
                }
                else  ErrList(2);
            }
            else //komentarz
            {
                memmove(&T,(void *)t,sizeof(NAGLOWEK)+t->n);
                strcpy(T.text,st);
                T.dl=strlen(st);
                T.n=T18+T.dl;
                t=(TEXT*)korekta_obiekt((void *)adr,(void*)&T);
            }
        }
        if(t!=NULL) POZ=t;
        rysuj_obiekt((char*)&Tp,COPY_PUT,0);
        flip_screen();
        delay(50);
        if(t==NULL) t=&Tp;
        rysuj_obiekt((char*)t,COPY_PUT,1);
        flip_screen();

        ////out_sel(X,Y);
        if (PTRS__Text_Style[t->czcionka]->type == 2) redraw();
        flip_screen();
        CUR_ON(X,Y);
    }
    return 0;
}

/*------------- VarToBuf  ---------------*/

int VarToBuf(int variable_set)
{ TEXT *t;
 // static   FILE *comments_tmp;
  char /*huge*/ *adh, *endptr;
  int *BufSize=(int*)BufVar;
  double /*huge*/ *Var=(double /*huge*/*)( BufVar+sizeof(int) );
  char er[81]=_wrong_real_number_format_;
  int i;
  ////for test
  MyDaneTabl *vars=(MyDaneTabl*)BufVar;
  ////

  *BufSize=0;
  
 // comments_tmp=fopen("COMMENTS.TMP","wt");
  adh=dane;
  adh+=dane_size;
  i_tmp=0;
  for (i=0; i<16; i++) strcpy(comments_tmp[i],"");
  obiekt_tok(dane,adh,(char**)&t,Otekst);
  while ((t!=NULL) && (*BufSize<24))
   { if ((t->obiektt2==O2BlockDim) || (t->typ==n_typ_zmienna))
     {
     char *t_text=decimal_dot(t->text);

     *Var = strtod( t_text, &endptr );
     if (endptr!=t_text && *endptr == '\0' && errno != ERANGE)
     {
         (*BufSize)++;
         Var++;
     }
     else
       { strcat(er,t_text);
	     ErrListStr(er);
	     return 0;
       }
     }
     else
     {
       if (t->typ==n_typ_komentarz)
       {
        //zapis lancuchow do zbioru komentarzy
       // fputs(t->text,comments_tmp); fputs("\n",comments_tmp);
        //zapamietanie lancuchow
        i_tmp+=1;
        if (strlen(t->text)>0)
         {
          if (i_tmp < max_i_tmp)
           {
           //  strncpy(comments_tmp[i_tmp],t->text,60);
             strcpy(comments_tmp[i_tmp],t->text);
           }
         }
       }

     }  
     obiekt_tok(NULL,adh,(char**)&t,Otekst);
   }

    switch (variable_set)
    {
  case 1:
        for (i=0; i<vars->n; i++) dane_profs_tab[i]=vars->var[i];
        break;
  case 5:
        for (i=0; i<vars->n; i++) data_odwiert[i]=vars->var[i];
        break;
  default:
        break;
    }
    ////test
    ////printf("VARS: %d\n",vars->n);
    ////for (i=0; i<24; i++)
    ////    printf("%lf\n",vars->var[i]);
    ////printf("%lf\n",vars->param1);
    ////

  return 1;
}


BOOL isBitSet(int number, int k) {
    return (number & (1 << k)) != 0;
}

/*------------- VarToBuf  ---------------*/

void BufToVar(void)
{ TEXT Tp = Tdef;
  TEXT *T, *t;
  BLOK  *b;
  LINIA *L;
  NAGLOWEK *nag;
  char*adh, *endptr;
  int var1;
  double Var1;
  char er[81]=_wrong_real_number_format_;
  long del_adp;
  char  *adp,  *adk;
  char str1[60];
  int param1;

  adp = dane ;
  adk = dane + dane_size ;
  var1 = 0;
  
  param1 = (int) dane_profil.param1;

  param1 = (int)pow(2, 20) - 1; //????

  if (Check_if_LE(dane_profil.var[4],0.0)==FALSE)
   {
    if (Check_if_Equal(dane_profil.var[3], dane_profil.var[4])==TRUE)
        dane_profil.var[3]=0.0;
   }
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt == Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE))
       {
        T = (TEXT*)adp;
        if ((T->obiektt2==O2BlockDim) || (T->typ==n_typ_zmienna))
         {
          if (var1<21 && isBitSet(param1, var1))  //tab_bit[var1]) != 0))
          {
           del_adp=adp-dane;
           memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);
           Var1=dane_profil.var[var1];
           if (Var1< 1e-4) Var1 = 0;
           if (Var1> 1e+10) Var1 = 0;
           sprintf(str1,"%g",Var1);
           strcpy (Tp.text, str1);

           decimal(Tp.text);

           Tp.dl = strlen(Tp.text);
           Tp.n = T18 + Tp.dl;
           if ( (t = korekta_obiekt(adp, &Tp)) == NULL) return;
           adp=dane + del_adp; 
           adk=dane + dane_size;
          }
          var1 ++; 
         } 
       } 
      L = (LINIA*)adp;
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
}

char *tab_instal[]={"","Ks","Ko","Kd","W","G","e","kT","T","?","","","","","","",""};

void DataToVar(int opcja)
{ TEXT Tp, *T, *t;
  BLOK  *b;
  LINIA *L;
  NAGLOWEK *nag;
  char *adh, *endptr;
  int var1, var2, var3;
  double Var1;
  char er[81]=_wrong_real_number_format_;
  long del_adp;
  char  *adp,  *adk;
  char str1[60];
  int param1;
  double data_profil[30];
  int i;


  if (opcja==0)
   {
    data_profil[0]=get_skala_profilu_x();
    data_profil[1]=get_poziom_pp();
    data_profil[2]=get_typ_profilu();
    for (i=3; i<6; i++)
     {
       data_profil[i]=0;
     }
   }
    else if (opcja==1)
     {
      data_profil[0]=get_hektometr_p();
      data_profil[1]=get_hektometr_k();
      for (i=2; i<6; i++)
       {
         data_profil[i]=0;
       }
     }
      else if (opcja==2) //instalacje
       {
         for (i=0; i<17; i++) data_profil[i]=0.; //data_instal_last[i];
         //ostatni pomiar wpisywany jest do zmiennych bloku
         //poprzednie pomiary wpisywane sa do tabeli archiwalnej
       }
        else if (opcja==3) //odwiert
         {
          for (i=0; i<29; i++) data_profil[i]=data_odwiert[i]; //0;
         }

  adp = dane ;
  adk = dane + dane_size ;
  var1 = 0;
  var2 = 0;
  var3 = 0;
  
  param1 = (int) dane_profil.param1;
  if (param1 == 0) 
   {
    param1 = 2147483647;
   }
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt == Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE))
       {
        T = (TEXT*)adp;
        if ((T->obiektt2==O2BlockDim) || (T->typ==n_typ_zmienna))
         {
           del_adp=adp-dane;
           memmove (&Tp, T, sizeof(NAGLOWEK)+T->n);
           Var1=data_profil[var1];
           if (Var1< 1e-4) Var1 = 0;
           if (Var1> 1e+10) Var1 = 0;
           sprintf(str1,"%-lg",Var1);
           strcpy (Tp.text, str1);

           decimal(Tp.text);

           Tp.dl = strlen(Tp.text);
           Tp.n = T18 + Tp.dl;
           if ( (t = korekta_obiekt(adp, &Tp)) == NULL) return;
           adp=dane + del_adp; 
           adk=dane + dane_size;
           var1 ++;
         }
        else if (T->typ==n_etykieta_sklad)
         {
           del_adp=adp-dane;
           memmove (&Tp, T, sizeof(NAGLOWEK)+T->n);
           Var1=data_instal_tab[var2][var3];
           if (var3==1)
            {
             strcpy(str1,tab_instal[(int)Var1]);
            }
             else
              {
               if (Var1< 1e-4) Var1 = 0;
               if (Var1> 1e+10) Var1 = 0;
               if (Var1 != 0) sprintf(str1,"%-lg",Var1);
                 else strcpy(str1,"");
              }
           strcpy (Tp.text, str1);
           Tp.dl = strlen(Tp.text);
           Tp.n = T18 + Tp.dl;
           if ( (t = korekta_obiekt(adp, &Tp)) == NULL) return;
           adp=dane + del_adp; 
           adk=dane + dane_size;
           var3 ++;
           if (var3>5)
            {
              if (var2<19) var2++;
              var3=0;
            }
         }

       } 
      L = (LINIA*)adp;
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
}

void CommentsToVar(int opcja)
{ TEXT Tp, *T, *t;
  BLOK  *b;
  LINIA *L;
  NAGLOWEK *nag;
  int var1;
  long del_adp;
  char  *adp,  *adk;
  char str1[60];
  int param1;
  char comments_pro[17][61];


    switch (opcja)
    {
    case 0:
        return; //nothing to do
        break;
    case 1:
        return; //nothing to do
        break;
    case 2: //instalacje
        return; //nothing to do
        break;
    case 3: //odwiert
        memmove(comments_pro, comments_geo, sizeof(comments_pro));
        break;
    default:
        break;
    }

  adp = dane ;
  adk = dane + dane_size ;
  var1 = 1;  //in borehole comments start from 1

  param1 = (int) dane_profil.param1;
  if (param1 == 0)
   {
    param1 = 2147483647;
   }

  L = (LINIA*)adp;  //L instead of nag due to indication of layer
  while (L->obiekt != Okoniec && adp <= adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt == Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety)
         && (Layers[L->warstwa].on==TRUE))
       {
        T = (TEXT*)adp;
        if ((T->obiektt2==O2BlockDim) || (T->typ==n_typ_komentarz))
         {
           del_adp=adp-dane;
           memmove (&Tp, T, sizeof(NAGLOWEK)+T->n);
            // Here  comments are inserted
           if (strlen(comments_pro[var1])>0)
           {
               sprintf(str1,"%s",comments_pro[var1]);
               strcpy (Tp.text, str1);
               Tp.dl = strlen(Tp.text);
               Tp.n = T18 + Tp.dl;
               if ( (t = korekta_obiekt(adp, &Tp)) == NULL) return;
               adp=dane + del_adp;
               adk=dane + dane_size;
           }
           var1 ++;
         }
       }
      nag = (NAGLOWEK*)adp;
      adp += nag->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
}

void ForceDataToVar(int opcja, int wariant)
{ TEXT Tp, *T, *t;
  BLOK  *b;
  LINIA *L;
  NAGLOWEK *nag;
  char *adh, *endptr;
  int var1, var2, var3;
  double Var1;
  char er[81]=_wrong_real_number_format_;
  long del_adp;
  char  *adp,  *adk;
  char str1[60];
  int param1;
  double data_profil[30];
  int i,j;
  int wariant_i;


   if (opcja==0) //instalacje
       {
        for (i=0; i<17; i++) data_profil[i]=0;
        if (wariant>=0)
        {
         data_profil[0]=data_instal_tab[wariant][0];
         wariant_i=(int)data_instal_tab[wariant][1];
         switch (wariant_i)
          {
           case 1: data_profil[1]=data_instal_tab[wariant][4];  //Ks
                   data_profil[2]=data_instal_tab[wariant][5];
           break;
           case 2: data_profil[3]=data_instal_tab[wariant][4];  //Ko
                   data_profil[4]=data_instal_tab[wariant][5];
           break;
           case 3: data_profil[5]=data_instal_tab[wariant][4];  //Kd
                   data_profil[6]=data_instal_tab[wariant][5];
           break;
           case 4: data_profil[7]=data_instal_tab[wariant][4];  //W
                   data_profil[8]=data_instal_tab[wariant][5];
           break;
           case 5: data_profil[9]=data_instal_tab[wariant][4];  //G
                   data_profil[10]=data_instal_tab[wariant][5];
           break;
           case 6: data_profil[11]=data_instal_tab[wariant][4];  //e
           break;
           case 7: data_profil[12]=data_instal_tab[wariant][4];  //kT
           break;
           case 8: data_profil[13]=data_instal_tab[wariant][4];  //T
           break;
           default:
           break;
          }
         data_profil[14]=data_instal_tab[wariant][2];
        }
       }
        else if (opcja==1) //odwiert
         {
          for (i=0; i<30; i++) data_profil[i]=0;
         }

  adp = dane ;
  adk = dane + dane_size ;
  var1 = 0;
  var2 = 0;
  var3 = 0;

  
  param1 = (int) dane_profil.param1;
  if (param1 == 0) 
   {
    param1 = 2147483647;
   }
  
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt == Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE))
       {
        T = (TEXT*)adp;
        if ((T->obiektt2==O2BlockDim) || (T->typ==n_typ_zmienna))
         {
           del_adp=adp-dane;
           memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);
           Var1=data_profil[var1];
           if (Var1< 1e-4) Var1 = 0;
           if (Var1> 1e+10) Var1 = 0;
           sprintf(str1,"%-lg",Var1);
           strcpy (Tp.text, str1);

           decimal(Tp.text);

           Tp.dl = strlen(Tp.text);
           Tp.n = T18 + Tp.dl;
           rysuj_obiekt((char*)T,COPY_PUT,0);
           if ( (t = korekta_obiekt(adp, &Tp)) == NULL) return;
           rysuj_obiekt((char*)t,COPY_PUT,1);
           adp=dane + del_adp; 
           adk=dane + dane_size;
           var1 ++;
         }
       } 
      L = (LINIA*)adp;
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
}


/****/
void VerPar(void)
{ if( VarToBuf(-1) )  InfoList(1); }

/*----------------   NEXT  PREV   -------------------------*/
/*
static void CenterText(TEXT *ad, double *x,double *y)
{ double x1,x2,x3,x4,y1,y2,y3,y4;
  double adsin,adcos,addl;
  adsin=sin(ad->kat*Pi/180);
  adcos=cos(ad->kat*Pi/180);
  addl = ad->dl * ad->wysokosc * ad->width_factor ;
  x1=ad->x;                     y1=ad->y;
  x2=x1+addl*adcos;         y2=y1-Ky*addl*adsin;
  x4=x1-ad->wysokosc*adsin;   y4=y1-Ky*ad->wysokosc*adcos;
  x3=-x1+x2+x4;                 y3=-y1+y2+y4;
  *x=(x1+x3)/2;
  *y=(y1+y3)/2;
}
*/

static void CenterText(TEXT *ad, double *x,double *y)
{ double x1,x2,x3,x4,y1,y2,y3,y4, x01, y01;
  double adsin,adcos,addl;
  double WysokoscT;


  adsin=sinf(ad->kat);
  adcos=cosf(ad->kat);
  //addl = ad->dl * ad->wysokosc * ad->width_factor ;
  x1=ad->x;
  y1=ad->y;

  GetTextLen (ad, &addl) ;
  WysokoscT = ad->wysokosc;
  x01 = ad->x ;
  y01 = ad->y ;
  
    switch (ad->justowanie)
     {
  case 1: x1 = x01 - addl * adcos ;  //do prawej
          y1 = y01 - addl * adsin ;
          break;
  case 2: x1 = x01 - (addl/2) * adcos ;  //srodkowo
          y1 = y01 - (addl)/2 * adsin ;
          break;
  case 3: x1 = x01 - (addl/2) * adcos + (WysokoscT/2) * adsin;  //centralnie
          y1 = y01 - (addl)/2 * adsin - (WysokoscT/2) * adcos;
          break;        
  default: break;        
     } 


  x2=x1+addl*adcos;         y2=y1-Ky*addl*adsin;
  x4=x1-ad->wysokosc*adsin;   y4=y1-Ky*(double)ad->wysokosc*adcos;
  x3=-x1+x2+x4;                 y3=-y1+y2+y4;
  *x=(x1+x3)/2;
  *y=(y1+y3)/2;
}

int NextPar(void)
{ TEXT *t;
  char /*huge*/ *adh, /*huge*/ * adp ;
  double x,y;
  adh=dane;
  adh+=dane_size-1;
  adp=(char*)POZ;

  if (((NAGLOWEK*)adp)->obiekt==OdBLOK)
      adp+=sizeof(NAGLOWEK)+B3+((BLOK*)adp)->dlugosc_opisu_obiektu;
  else adp+=sizeof(NAGLOWEK)+POZ->n;
  obiekt_tok(adp,adh,(char**)&t,Otekst);
  while (t!=NULL)
   { if ((t->obiektt2==O2BlockDim) || (t->typ==n_typ_zmienna) || (t->typ==n_typ_komentarz))
      {	CenterText(t,&x,&y);
	mvcurp(jednostkiplt(x)-X,jednostkiplt(y)-Y);
	break;
      }
     obiekt_tok(NULL,adh,(char**)&t,Otekst);
   }
  if(t==NULL)
   { obiekt_tok(dane,(char *)POZ,(char**)&t,Otekst);
     while (t!=NULL)
      { if((t->obiektt2==O2BlockDim) || (t->typ==n_typ_zmienna) || (t->typ==n_typ_komentarz))
	 { CenterText(t,&x,&y);
	   mvcurp(jednostkiplt(x)-X,jednostkiplt(y)-Y);
	   break;
	 }
	obiekt_tok(NULL,(char *)POZ,(char**)&t,Otekst);
      }
   }
  if(t!=NULL) POZ=t;
  return 0;
}

int PrevPar(void)
{ TEXT *t,*POZ0=NULL;
  char *adh, * adp ;
  double x,y;
  adh=(char *)POZ;
  adh--;
  obiekt_tok(dane,adh,(char**)&t,Otekst);
  while (t!=NULL)
   { if((t->obiektt2==O2BlockDim) || (t->typ==n_typ_zmienna) || (t->typ==n_typ_komentarz))  POZ0=t;
     obiekt_tok(NULL,adh,(char**)&t,Otekst);
   }
  if(POZ0==NULL)
   { adp=(char /*huge*/*)POZ;
     adp+=sizeof(NAGLOWEK)+POZ->n;
     adh=dane;
     adh+=dane_size;
     obiekt_tok(adp,adh,(char**)&t,Otekst);
     while (t!=NULL)
      { if ((t->obiektt2==O2BlockDim) || (t->typ==n_typ_zmienna) || (t->typ==n_typ_komentarz))  POZ0=t;
	obiekt_tok(NULL,adh,(char**)&t,Otekst);
      }
   }
  if(POZ0!=NULL)
   { POZ=POZ0;
     CenterText(POZ0,&x,&y);
     mvcurp(jednostkiplt(x)-X,jednostkiplt(y)-Y);
   }
  return 0;
}

int PrevPar1(void)
{
  PrevPar(); 
  int n_edit = EditPar();
  return 0;
}

int NextPar1(void)
{
  NextPar(); 
  int n_edit = EditPar();
  return 0;
}

/*
void InitEP1(char type)
{ static int (far *SW[5])(),kom,kom0,sel_akt,sel_cur;
  static void far(*CUR_oN)(double ,double);
  static void far(*CUR_oFF)(double ,double);
  switch(type)
   { case 1 :
       SERV[73]=SW[0];
       SERV[81]=SW[1];
       SERV[67]=SW[2];
       SERV[68]=SW[3];
       SERV[60]=SW[4];
       komunikat(kom);
       komunikat0(kom0);
       sel.akt=sel_akt;
       sel.cur=sel_cur;
       menupini(NULL,"",' ');
       CUR_OFF(X,Y);
       CUR_ON=CUR_oN;
       CUR_OFF=CUR_oFF;
       break;
   }
}

int DrawPar(void)
 { int n;

//   if( VarToBuf(-1) )  InfoList(1);

   n=VarToBuf(-1);
   if(n)
   {InitEP1(1);return 1;}
  return 0;
 }

*/

/*-------  EditParams -----------------------------------*/
#define DrawParam  0
#define EditParam  1
#define VerParam   2
#define PrevParam  3
#define NextParam  4
#define BreakParam 5

void InitEP(char type)
{ static int (far *SW[4 /* 5 */])(),kom,kom0,sel_akt,sel_cur;
  static void far(*CUR_oN)(double ,double);
  static void far(*CUR_oFF)(double ,double);
  switch(type)
   { case 0 :
       CUR_oFF=CUR_OFF; CUR_OFF=out_sel_off;
       CUR_oN=CUR_ON;   CUR_ON=out_sel_on;
       SW[0]=SERV[73];  SERV[73]=sel_t;
       SW[1]=SERV[81];  SERV[81]=sel_d;
       SW[2]=SERV[67];  SERV[67]=PrevPar1;
       SW[3]=SERV[68];  SERV[68]=NextPar1;
    //   SW[4]=SERV[60];  SERV[60]=DrawPar;
       kom=Komunikat_R;
       kom0=Komunikat_R0;
/*        komunikat(0);  */
       komunikat0(38);
       sel_akt=sel.akt; sel.akt=0;
       sel_cur=sel.cur; sel.cur=1;
       //CUR_ON(X,Y);
       menupini(&mPPar,_PROFILE_,_PROFILE_C,914);
       
       NextPar();
        if (change_variable == 1) BufToVar();   //pierwszy pomiar, kolejny pomiar
        else if (change_variable == 2) DataToVar(0); //hektometr
        else if (change_variable == 3) DataToVar(1); //skala
        else if (change_variable == 4) DataToVar(2);  //instalacje
        else if (change_variable == 5)
        {
            DataToVar(3); //odwiert
            CommentsToVar(3); //odwiert
        }
       redraw();
      // komunikat(255);
      // komunikat_str (buf_kom) ;
       strcpy(global_kom, buf_kom);
       komunikat(255);
       CUR_OFF(X,Y);
       CUR_ON(X,Y);
       break;
     case 1 :
       SERV[73]=SW[0];
       SERV[81]=SW[1];
       SERV[67]=SW[2];
       SERV[68]=SW[3];
//       SERV[60]=SW[4];
       komunikat(kom);
       komunikat0(kom0);
       sel.akt=sel_akt;
       sel.cur=sel_cur;
       menupini(NULL,"",' ',0);
       CUR_OFF(X,Y);
       CUR_ON=CUR_oN;
       CUR_OFF=CUR_oFF;
       break;
   }
}

int EditParams(void)
{ EVENT *ev;
  double X0, Y0;
  char st[30];
  
  int n_edit;

  int n;
  InitEP(0);
  /*
  while(1)
  {
      ev=Get_Event_Point(NULL, &X0, &Y0);
      switch(ev->What)
      { case evKeyDown  :
          if(ev->Number==0)
          {
              InitEP(1);
              return 0;
          }
          if(ev->Number==ENTER)
          {
              n_edit = EditPar();
              if (n_edit == 1) //Esc
              {
                  InitEP(1);
                  return 0;
              }
              if (n_edit == 2) //Ok
              {
                  n=VarToBuf(change_variable);
                  if(n)
                  {
                      InitEP(1);
                      return 1;
                  }
              }
          }
          break;
      case evCommandP :
          if( ev->Number==EditParam )
          {
              n_edit = EditPar();
              if (n_edit == 1) //Esc
              {
                  InitEP(1);
                  return 0;
              }
              if (n_edit == 2) //Ok
              {
                  n=VarToBuf(change_variable);
                  if(n)
                  {
                      InitEP(1);
                      return 1;
                  }
              }
              break;
          }
          if( ev->Number==VerParam  )
          {
              VerPar();
              continue;
          }
          if( ev->Number==PrevParam  )
          {
              PrevPar();
              n_edit = EditPar();
              continue;
          }
          if( ev->Number==NextParam  )
          {
              NextPar();
              n_edit = EditPar();
              continue;
          }
          if( ev->Number==BreakParam)
          {
              InitEP(1);
              return 0;
          }
          if( ev->Number==DrawParam )
          {
              n=VarToBuf(change_variable);
              if(n)
              {
                  InitEP(1);
                  return 1;
              }
          }
      default       : break;
      }
  }
  */
    while(1)
    {
        ev = Get_Event_Point(NULL, &X0, &Y0);
        switch(ev->What)
        {
            case evKeyDown:
                if(ev->Number == 0)
                {
                    InitEP(1);
                    return 0;
                }
                if(ev->Number == ENTER)
                {
                    n_edit = EditPar();
                    if (n_edit == 1) // Esc
                    {
                        InitEP(1);
                        return 0;
                    }
                    if (n_edit == 2) // Ok
                    {
                        n = VarToBuf(change_variable);
                        if(n)
                        {
                            InitEP(1);
                            return 1;
                        }
                    }
                }
                break;

            case evCommandP:
                if(ev->Number == EditParam)
                {
                    n_edit = EditPar();
                    if (n_edit == 1) // Esc
                    {
                        InitEP(1);
                        return 0;
                    }
                    if (n_edit == 2) // Ok
                    {
                        n = VarToBuf(change_variable);
                        if(n)
                        {
                            InitEP(1);
                            return 1;
                        }
                    }
                    break;
                }
                if(ev->Number == VerParam)
                {
                    VerPar();
                    break; // Changed to break
                }
                if(ev->Number == PrevParam)
                {
                    PrevPar();
                    n_edit = EditPar();
                    break; // Changed to break
                }
                if(ev->Number == NextParam)
                {
                    NextPar();
                    n_edit = EditPar();
                    break; // Changed to break
                }
                if(ev->Number == BreakParam)
                {
                    InitEP(1);
                    return 0;
                }
                if(ev->Number == DrawParam)
                {
                    n = VarToBuf(change_variable);
                    if(n)
                    {
                        InitEP(1);
                        return 1;
                    }
                }
                break; // Added to prevent falling through to default
            default:
                break;
        }
    }
}

/*-----------------------------------------------------*/
/*            Przesun Blok			       */
/*-----------------------------------------------------*/
static double Px, Py,DX,DY;

static void far cur_offPB(double x,double y)
{ out_blok1(DX,DY,0,0,Tprzesuw,0);
  cursel_off(x,y);
}

static void far cur_onPB(double x,double y)
{ cursel_on(x,y);
  DX=x-Px;  DY=y-Py;
  out_blok1(DX,DY,0,0,Tprzesuw,0);
}

static void redcrPB(char typ)
{ static void far(*CUR_oN)(double ,double);
  static void far(*CUR_oFF)(double ,double);
 if (typ==0)
   { BlokM=1;
     CUR_oFF=CUR_OFF;    CUR_OFF=cur_offPB;
     CUR_oN=CUR_ON;      CUR_ON=cur_onPB;
     komunikat0(37);
     Cur_ond(X,Y);
   }
 else
  { Cur_offd(X,Y);
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;
    BlokM=0;
    komunikat0(0);
  }
}

static int PrzesunB_old(double X0, double Y0)
{
  EVENT *ev;

  Px=X0;Py=Y0;
  DX=X-X0; DY=Y-Y0;
  sel.akt=1;
  redcrPB(0);
  while(1)
   { ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0) { redcrPB(1); return ESC; }
     if (ev->What == evKeyDown &&  ev->Number==ENTER)
      { redcrPB(1);
	    return ENTER;
      }
   }
}

static int PrzesunB(double X0, double Y0)
{
    EVENT *ev;

    Px=X0;Py=Y0;
    DX=X-X0; DY=Y-Y0;
    redcrPB(0);
    while(1) {
        ev = Get_Event_Point(NULL, &X0, &Y0);
        if (ev->What == evKeyDown && ev->Number == 0) {
            redcrPB(1);
            return ESC;
        }
        if (ev->What == evKeyDown && ev->Number == ENTER) {
            redcrPB(1);
            return ENTER;
        }
        if (ev->What == evCommandP) {
            set_Px_Py(X0,Y0);
            if (ev->Number == 0) {
                obrot_import();
            }
            else if (ev->Number == 1)
            {
                obrot_import_kat (45) ;
            }
            else if (ev->Number == 2)
            {
                obrot_import_kat (90) ;
            }
            else if (ev->Number == 3)
            {
                obrot_import_kat (135) ;
            }
            else if (ev->Number == 4)
            {
                obrot_import_kat (180) ;
            }
            else if (ev->Number == 5)
            {
                obrot_import_kat (225) ;
            }
            else if (ev->Number == 6)
            {
                obrot_import_kat (270) ;
            }
            else if (ev->Number == 7)
            {
                obrot_import_kat (315) ;
            }
            else if (ev->Number == 8)
            {
                obrot_import_kat(0);
            }
            else if (ev->Number == 9)
            {
                obrot_import_K();
            }
            else if (ev->Number == 10)
            {
                obrot_kopiuj_paral(0);
            }
            else if (ev->Number == 11)
            {
                obrot_kopiuj_paral(Pi/2.0);
            }
            else if (ev->Number == 12)
            {
                obrot_kopiuj_paralK();
            }
        }
    }
}

/*----------------------------------------------------*/
/*              RysPar                                */
/*----------------------------------------------------*/

static int BufVarSize(void)
{ NAGLOWEK *nag;
  TEXT *t;
  char /*huge*/ *adh;
  int Size=sizeof(int);
  adh=dane;
  adh+=dane_size;
  obiekt_tok(dane,adh,(char**)&nag,Otekst);
  while (nag!=NULL)
   { t=(TEXT *)nag;
     if (((t->obiekt==Otekst) && ((t->typ==n_typ_zmienna) || (t->typ==n_typ_komentarz))) || (nag->obiektt2==O2BlockDim))
      Size+=sizeof(double);
     obiekt_tok(NULL,adh,(char**)&nag,Otekst);
   }
  return Size;
}

static void redcrMANHOLE (int type)
/*----------------------------*/
{
    static void (*CUR_oN)(double, double);
    static void (*CUR_oFF)(double, double);
    static int(*SW[3])(), akt;

    switch (type) {
        case 0:
            menupini(&mKatR, _MANHOLE_R_, _MANHOLE_C_, 943);
            set_block_angle(0.0);
            break;
        case 1:
            break;
            menupini(NULL, "", ' ', 0);
        default:
            break;
    }
}


int ParBlok(char *FileName0,int (*HEJKA)(void*), int n )
{
    int k;
    long ds;
    static OKNO O;
    int i_maxlen = 80 ;
    char blok_name [80] ;
    char blok_type [80] ;
    char fn1 [MAXPATH] ;
    double dynamic_block_x ;
    //MyDane dane_profs;
    //MyDane dane_profs0={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int znacznik_aplikacji;
    int i_f_handle ;
    double skala_profilu, poziom_pp, local_x, local_y, typ_profilu0;
    int typ_profilu1;
    BOOL save_original_layer;
    BOOL current_layer_on;
    BOOL current_layer_edit;
    BOOL current_layer_point;
    char filename[MAXPATH];

    sprintf(filename,"%s%s%s#%d.alx",bloki_dir,SSlash,FileName0,Client_number);

    /*
    tutaj dokonywana jest kontrola, czy wybrano wlasciwa funkcje.
    jezeli n=0,1,2,3 to jezeli skala=0 to blad;
    jezeli n=0 to jezeli brak danych, to blad;
    jezeli n=0,1 to jezeli local_x=0 lub local_y=0, mozna podac tez komunikat o
    przypuszczalnie blednym usytuowaniu punktu 0,0
    */
    skala_profilu=get_skala_profilu_x();
    poziom_pp=get_poziom_pp();
    typ_profilu0=get_typ_profilu();
    typ_profilu1=(int) typ_profilu0;

    /* odwiert */
    /* if (n==4)
      {
      ErrList(150);
      return;
      }      */
    if ((n == 0) || (n == 1)) change_variable = 1;
    else if (n == 5) change_variable = 2;
    else if (n == 4) change_variable = 3;
    else if (n == 2) change_variable = 4;
    else if (n == 3) change_variable = 5;
    else change_variable = 0;

    if ((n==0) || (n==1) || (n==2) || (n==3) || (n==4))
    {
        if (skala_profilu<=0)
        {
            ErrList(122);
            return 0;
        }
    }

    /*****/
    znacznik_aplikacji=get_znacznik_aplikacji();

    if (n==0)
    {
        if (znacznik_aplikacji==101)
        {
        }
        else
        {
            ErrList(123);
            return 0;
        }
    }
    if ((n==0) || (n==1) || (n==5))
    {
        local_x=get_localx();
        local_y=get_localy();
        if ((local_x==0) || (local_y==0))
        {
            ErrList(124);
            Get_Key ();
            ErrList(0);
        }
    }

    CUR_OFF(X,Y);

#define SKETCHBOOK

#ifdef SKETCHBOOK
sketchparent=DRAWING_NUMBER;  //saving number of current drawing

if (sketchbook_exists==0)
{
    int ret = New_window_sketchbook();
    if (!ret) return 0;

    sketchbook=DRAWING_NUMBER;
    sketchbook_exists=1;

    /*tutaj nalezy przywrocic ostatnio pamietana skale prezentacji i okno*/
    Auto_Pan0=Auto_Pan;
    if (auto_pan_block==FALSE) Auto_Pan=FALSE; else Auto_Pan=TRUE;

}
else
{
    Restore_drawing(sketchbook, 1);  //restoring sketch without changing profile params

    Auto_Pan0=Auto_Pan;
    if (auto_pan_block==FALSE) Auto_Pan=FALSE; else Auto_Pan=TRUE;

    //deleting all from sketch
    zmien_atrybut(dane,dane + dane_size,ANieOkreslony,Ausuniety);
    usun_blok(dane, dane + dane_size);
}

if(!ImportMat(filename))
{
    //optionally deleting all from sketch here
    ////
    Restore_drawing(sketchparent, 0);  //0 to restore previous profile params
    Auto_Pan=Auto_Pan0;
    return 0;
}
#else
    k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
    if ( k )  { CUR_ON(X,Y);return; }

    /*tutaj nalezy przywrocic ostatnio pamietana skale prezentacji i okno*/
    if (auto_zoom_block==FALSE)
        put_findmarker_force();
    Auto_Pan0=Auto_Pan;
    if (auto_pan_block==FALSE)
        Auto_Pan=TRUE;
    else
        Auto_Pan=FALSE;

    zwolnienie_pamieci();

    save_original_layer=options1.save_original_layer;
    options1.save_original_layer=0;
    current_layer_on=(BOOL)Layers[Current_Layer].on;
    current_layer_edit=(BOOL)Layers[Current_Layer].edit;
    current_layer_point=(BOOL)Layers[Current_Layer].point;
    Layers[Current_Layer].on=1;
    Layers[Current_Layer].edit=1;
    Layers[Current_Layer].point=1;
    /*tutaj nalezy przywrocic ostatnio pamietana skale prezentacji i okno*/
    // put_findmarker_force();

    ClearWindow();
    /*****/

    /*****/
    if(!ImportMat(filename))
    { zwolnienie_pamieci();
        czytaj_rysunek(RYSUJ$1, FALSE);
        return 0;
    }
#endif

    int bufsize=BufVarSize();

    if (BufVar==NULL) {
        bsize=max(bufsize, 26 * sizeof(double));
        BufVar = malloc(bsize);
        if (BufVar==NULL)
        {
            ErrList(18);
            return 0;
        }
        memset(BufVar, 0, bsize);
    }
    else {
        if (bufsize>bsize) {
            bsize=bufsize;
            char *BufVar_new = realloc(BufVar, bsize);
            if (BufVar==NULL)
            {
                ErrList(18);
                return 0;
            }
            BufVar=BufVar_new;
        }
    }

    /*****/
    //  if (n==4) /*hektometry*/
    //  {
    //   ;
    //  }
    if (n==5) /* skala */
    {
        sprintf (buf_kom, "%s=%-12.8lg  %s=%-12.8lg  %s=%d", (char*)_last_profile_scale_,
               skala_profilu, (char*)_comparative_level_, poziom_pp, (char*)_profile_type_, typ_profilu1) ;
        Add_String_To_List (buf_kom) ;
    }
    else if (n==6) /* nowy profil */
    {
        ;
    }
    else
    {
        if (znacznik_aplikacji==101)
        {
#ifdef SAVETMP
            i_f_handle = open (PROFIL_TEMP_FILE, O_BINARY | O_RDONLY, S_IREAD) ;
            if (i_f_handle == -1)
            {
                //zerowanie danych dane_profs
                //dane_profs=dane_profs0;
                memmove(&dane_profil, &dane_profs0, sizeof(MyDane));
            }
            else
            {
                if (read(i_f_handle, &dane_profs, sizeof(dane_profs)) != sizeof(dane_profs))
                {
                    //zerowanie danych dane_profs
                    // dane_profs=dane_profs0;
                    memmove(&dane_profil, &dane_profs0, sizeof(MyDane));
                }
                close (i_f_handle) ;
                if ((n == 0) || (n == 1))
                {
                    change_variable = 1;
                    memmove(&dane_profil, &dane_profs, sizeof(MyDane));
                }

            }
#else
            ////memmove(&dane_profil, &dane_profs0, sizeof(MyDane));
            memmove(&dane_profil, &dane_profs1, sizeof(MyDane));  //saving current measurement data to the current buffer for EditParams
#endif

            if (typ_profilu0==0)
                sprintf (buf_kom, "%s odl=%-8.7lg rki=%-8.7lg rkp=%-8.7lg ru=%-8.7lg rt=%-8.7lg",(char*)_last_measure_,
                dane_profs1.odl, dane_profs1.rki, dane_profs1.rkp, dane_profs1.ru, dane_profs1.rt) ;
            else if (typ_profilu0==1)
                sprintf (buf_kom, "%s odl=%-8.7lg rkp=%-8.7lg rtp=%-8.7lg rti=%-8.7lg",(char*)_last_measure_,
                dane_profs1.odl, dane_profs1.rkp, dane_profs1.ru, dane_profs1.rt) ;
            else
                sprintf (buf_kom, "%s odl=%-8.7lg rkp=%-8.7lg rtp=%-8.7lg rti=%-8.7lg",(char*)_last_measure_,
                dane_profs1.odl, dane_profs1.rkp, dane_profs1.ru, dane_profs1.rt) ;

            Add_String_To_List (buf_kom) ;
        }
        else
        {
            sprintf(buf_kom,"%s %s",(char*)_last_measure_, (char*)_no_data_);
        }
    }

    // komunikat_str (buf_kom) ;

    /****/
    //  BufToVar();
    //  k = 0;
    k=EditParams();


//#define SAVEBLOCK
#ifdef SAVEBLOCK
    if ((k) && (n != 2))
    {
        O.x1=0;
        O.y1=0;
        O.x2=200;
        O.y2=160;
        O.x01=0;
        O.y01=0;
        O.x02=200;
        O.y02=160;

        //eksplozja bloku
        *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;
        addrem_obiekty (0, &O, dzi_off, odzi_off) ;
        Explode1();

        strcpy(fn1, filename);
        File_Ext (fn1, ".alx");

        if (TRUE != Read_Spec_Block (fn1, blok_name, 80 ,
              blok_type, 80 , FALSE))
        {
            blok_name [0] ='\0' ;
            blok_type [0] = '\0' ;
        }

        //zapis bloku
        addrem_obiekty (0, &O, dzi_off, odzi_off) ;
        WriteBlock(filename,0,0, blok_name, blok_type, 0, 1);
    }
#endif

#ifdef SKETCHBOOK
    Restore_drawing(sketchparent, 1);  //1 to avoid overwriting profile params after editing
    Auto_Pan=Auto_Pan0;
#else
    options1.save_original_layer=save_original_layer;
    options1.save_original_layer=save_original_layer;
    Layers[Current_Layer].on=current_layer_on;
    Layers[Current_Layer].edit=current_layer_edit;
    Layers[Current_Layer].point=current_layer_point;

    if (auto_zoom_block==FALSE)
        put_setmarker_force();
    Auto_Pan=Auto_Pan0;
    //put_setmarker_force();

    zwolnienie_pamieci();
    czytaj_rysunek(RYSUJ$1, FALSE);
    Ini_Place_Marker ();  ////
    Ini_Layers_Dlg () ;   ////
#endif

    ds=dane_size;

    if( k && (*HEJKA)(BufVar) )
    { ADP=dane; ADK=dane+(dane_size-ds)-1;
        CUR_OFF(X,Y);
        zmien_atrybut(ADP,ADK,Anormalny,Ablok);
        /* z przesuwaniem */
        dynamic_block_x=get_dynamic_block();
        if (dynamic_block_x==1)
        {

            if(PrzesunB(0,0)==ESC)
            { zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
                usun_blok(ADP,ADK);
                komunikat0(0);
                Komunikat_R=0;
                Komunikat_R0=0;
                CUR_OFF(X,Y);
                CUR_ON(X,Y);

                if (BufVar!=NULL)
                {
                    //free(BufVar);
                    //BufVar=NULL;
                }
                return 0;
            }
            transformacja_blok(ADP,ADK,X-0,Y-0,0,0,Tprzesuw,0);
        }
        else
        {
            transformacja_blok(ADP,ADK,get_localx(),get_localy(),0,0,Tprzesuw,0);
        }
        blokzap(ADP,ADK,Ablok,COPY_PUT,1);
        zmien_atrybut(dane, dane + dane_size, Aoblok, Anormalny);
        zmien_atrybut(ADP,ADK,Ablok,Aoblok);
        //CUR_OFF(X,Y);
        //CUR_ON(X,Y);
        Change = TRUE;

    }
    if (k) //if function didn't work, still new data is recorded to correct in next approach
    {
        switch (change_variable)
        {
        case 1:
            dane_profs1.n=24;
            memmove((char*)&dane_profs1+sizeof(int), dane_profs_tab, sizeof(double)*24);
            dane_profs1.param1=pow(2, 24)-1;
            dane_profs1.param1=get_param1(&dane_profs1);
            break;
        case 5:
            memmove(comments_geo, comments_tmp, sizeof(comments_tmp));
            break;
        default:
            break;
        }
    }

    komunikat0(0);
    Komunikat_R=0;
    Komunikat_R0=0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);

    //if (BufVar!=NULL)
    //{
        //free(BufVar);
        //BufVar=NULL;
    //}

    return k;
}

void Destroy_BufVar(void) {
    if (BufVar != NULL) {
      free(BufVar);
      BufVar=NULL;
    }
}

/*PARBLO*/

static double get_little_val (double df_lv, double df_x, double df_y)
/*-----------------------------------------------------------------*/
{
  double df_dd ;

  df_dd = fabs (df_x / df_lv) ;
  if (df_dd == 0)
  {
    df_dd = fabs (df_y / df_lv) ;
    if (df_dd == 0)
    {
      df_dd = 1 / df_lv;
    }
  }
  if (df_dd < 1 / df_lv)
  {
    df_dd = 1 / df_lv;
  }
  return df_dd ;
}
/*
static BOOL Check_if_Equal (double x, double y)
//---------------------------------------------
{
  BOOL retval ;
  double dd ;

  retval = FALSE ;
  dd = get_little_val (DF, x, y) ;
  if (fabs (x - y) < dd)
  {
    retval = TRUE ;
  }
  return retval ;
}
*/

/*---------------------------------------------------*/
/*                Profil1                            */
/*---------------------------------------------------*/
static void tablica_0_1 (double dane_global_odl)
/*---------------------------------------------*/
{ int ii;
    if (dane_global_odl==0)
       { for (ii=0; ii<=19; ii++)
        tablica_y[ii]=tabela_p0_y[ii];
       }
    else if (dane_global_odl==1)
       { for (ii=0; ii<=19; ii++)
	    tablica_y[ii]=tabela_p1_y[ii];
       }
    else /*2*/
       { for (ii=0; ii<=19; ii++)
         tablica_y[ii]=tabela_p2_y[ii];
       }
}

static BOOL add_block (char kod_bloku, unsigned char obiektt2)
/*----------------------------------------------------------*/
{
#ifndef LINUX
    BLOKD s_blockd = BDdef ;
#else
    BLOK s_blockd = BDdef ;
#endif
    BLOK *ptrs_block ;
  char c_pltype ;


  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = kod_bloku;  //B_PLINE ;
  s_blockd.obiektt2 = obiektt2;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  c_pltype = PL_OTHER ;
  memmove (&(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  return TRUE ;
}

static BOOL add_block_with_type (double x, double y, char *blok_type0, unsigned char obiektt2, char kod_bloku)
/*----------------------------------------------------------------------------------------------------------*/
{
  unsigned size_block = B3 ;
  BLOK s_blockd = Bdef;
#ifndef LINUX
    BLOKD blokd= BDdef ;
#else
    BLOK blokd= BDdef ;
#endif
  BLOK *ptrs_block, *buf_block ;
  int  len_type, len_desc ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  char blok_type[31];


  strcpy(blok_type,blok_type0);

  len_type = (int)strlen (blok_type) + 1 ;
  len_desc = sizeof(unsigned) + 2 * sizeof (float) + sizeof(len_type) + len_type ;
  size_block += len_desc ;
  
  if (NULL == (buf_block = (BLOK*) getmem  (sizeof (NAGLOWEK) + size_block)))
  {
    return FALSE;
  }
  memcpy (buf_block, &blokd, sizeof (blokd)) ;
  buf_block->n = size_block ;
  buf_block->kod_obiektu = kod_bloku ;
  buf_block->obiektt2=obiektt2;
  buf_block->dlugosc_opisu_obiektu = len_desc ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&buf_block->opis_obiektu [0]) ;
  ptrs_desc_bl->flags = EBF_IP ;
  ptrs_desc_bl->x = (float)x ;
  ptrs_desc_bl->y = (float)y ;
  ptrs_desc_bl->len = len_type ;
  strcpy (&ptrs_desc_bl->sz_type [0], blok_type) ;
  if ((ptrs_block = (BLOK*)dodaj_obiekt (NULL, buf_block)) == NULL)
  {
    free_mem () ;
    return FALSE;
  }
  free_mem () ;
  return TRUE ;
}


static BOOL group_block_with_type (long dane_size000, double x, double y, char *blok_type0, char kod_bloku)
/*-------------------------------------------------------------------------------------------------------*/
{
#ifndef LINUX
    BLOKD s_blockd = BDdef ;
#else
    BLOK s_blockd = BDdef ;
#endif
  BLOK *ptrs_block, *buf_block, *b ;
  char c_pltype ;
  char *adr, *adrk;
  NAGLOWEK *nag ;
  BOOL first_block;

//  s_blockd.n = B3 + sizeof (c_pltype) + dane_size000;
//  s_blockd.kod_obiektu = B_PLINE ;
//  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
//  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
//  {
//    return FALSE ;
//  }
//  c_pltype = PL_OTHER ;
//  memmove (&(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  //zmiana atrybutu blok


   if (FALSE == add_block_with_type (x, y, blok_type0, 0, kod_bloku))
   {
    return FALSE;
   }

  adr=dane;
  b=(BLOK *)adr;
//  adr+= sizeof(NAGLOWEK) + B3 + sizeof (c_pltype);
//  adrk=adr + sizeof(NAGLOWEK) + B3 + sizeof (c_pltype) + dane_size000 - 1 ;//???????? czy ta jedynka jest potrzebna ?
  adrk=adr + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu + dane_size000 - 1 ;//???????? czy ta jedynka jest potrzebna ?


  nag=(NAGLOWEK*)adr;
  first_block=FALSE;

  while (nag->obiekt!=Okoniec && adr<=adrk)
  {
//     if (TRUE == Check_Attribute (nag->atrybut, aold))
//     {
//       nag->atrybut=anew;
//     }
    switch(nag->obiekt)
     { case OdBLOK :
	    b=(BLOK *)adr;
	    adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
       if (first_block==FALSE)
        {
         b->n+= dane_size000;
         first_block=TRUE;
        }
        else b->blok=ElemBlok;
	    break;
       default :
       nag->blok=ElemBlok;
	    adr+=nag->n+sizeof(NAGLOWEK);
	    break;
     }
    nag=(NAGLOWEK*)adr;
  }
  return TRUE ;
}

static BOOL group_block (long dane_size000, char kod_bloku)
/*-------------------------------------------------------*/
{
#ifndef LINUX
    BLOKD s_blockd = BDdef ;
#else
    BLOK s_blockd = BDdef ;
#endif
  BLOK *ptrs_block, *buf_block, *b ;
  char c_pltype ;
  char *adr, *adrk;
  NAGLOWEK *nag ;
  BOOL first_block;

//  s_blockd.n = B3 + sizeof (c_pltype) + dane_size000;
//  s_blockd.kod_obiektu = B_PLINE ;
//  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
//  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
//  {
//    return FALSE ;
//  }
//  c_pltype = PL_OTHER ;
//  memmove (&(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  //zmiana atrybutu blok


   if (FALSE == add_block (kod_bloku /*B_PLINE*/, 0))
   {
    return FALSE;
   }

  adr=dane;
//  adr+= sizeof(NAGLOWEK) + B3 + sizeof (c_pltype);
  adrk=adr + sizeof(NAGLOWEK) + B3 + sizeof (c_pltype) + dane_size000 - 1 ;//???????? czy ta jedynka jest potrzebna ?

  nag=(NAGLOWEK*)adr;
  first_block=FALSE;

  while (nag->obiekt!=Okoniec && adr<=adrk)
  {
//     if (TRUE == Check_Attribute (nag->atrybut, aold))
//     {
//       nag->atrybut=anew;
//     }
    switch (nag->obiekt)
     { case OdBLOK :
	    b=(BLOK *)adr;
	    adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
        if (first_block==FALSE)
            {
             b->n+= dane_size000;
             first_block=TRUE;
            }
        else b->blok=ElemBlok;
	    break;
       default :
        nag->blok=ElemBlok;
	    adr+=nag->n+sizeof(NAGLOWEK);
	    break;
     }
    nag=(NAGLOWEK*)adr;
  }
  return TRUE ;
}

static BOOL add_block_in_block_with_type (BLOK *parent, double x, double y, char *blok_type0, unsigned char obiektt2, char kod_bloku)
/*---------------------------------------------------------------------------------------------------------------------------------*/
{
    unsigned size_block = B3 ;
    BLOK s_blockd = Bdef;
#ifndef LINUX
    BLOKD blokd= BDdef ;
#else
    BLOK blokd= BDdef ;
#endif
    BLOK *ptrs_block, *buf_block ;
    int  len_type, len_desc ;
    T_Desc_Ex_Block 	*ptrs_desc_bl ;
    char blok_type[31];

    strcpy(blok_type,blok_type0);

    len_type = (int)strlen (blok_type) + 1 ;
    len_desc = sizeof(int) + 2 * sizeof (float) + sizeof(len_type) + len_type ;
    size_block += len_desc ;

    if (NULL == (buf_block = (BLOK*) malloc  (sizeof (NAGLOWEK) + size_block)))
    {
        return FALSE;
    }
    memcpy (buf_block, &blokd, sizeof (blokd)) ;
    buf_block->blok = ElemBlok;
    buf_block->n = size_block ;
    buf_block->kod_obiektu = kod_bloku ;
    buf_block->obiektt2=obiektt2;
    buf_block->dlugosc_opisu_obiektu = len_desc ;
    ptrs_desc_bl = (T_Desc_Ex_Block *)(&buf_block->opis_obiektu [0]) ;
    ptrs_desc_bl->flags = EBF_IP ;
    ptrs_desc_bl->x = (float)x ;
    ptrs_desc_bl->y = (float)y ;
    ptrs_desc_bl->len = len_type ;
    strcpy (&ptrs_desc_bl->sz_type [0], blok_type) ;
    if ((ptrs_block = (BLOK*)dodaj_obiekt (parent, buf_block)) == NULL)
    {
        free (buf_block) ;
        return FALSE;
    }
    free (buf_block) ;

    PTR__GTMPBLOCK=(char *) ptrs_block;
    return TRUE ;
}



static char *dodaj_obiekt_(BLOK * adb,void  *ad)
{
    BLOK *blok;
    char *ad1;
    //int n, n1;

    //return dodaj_obiekt(adb, ad);  //instead of adb ////

    blok=(BLOK*)dane;
    //n=blok->n;
    //ad1=dodaj_obiekt(adb, ad);
    if (PTR__GTMPBLOCK!=NULL)
        ad1=dodaj_obiekt((BLOK*)PTR__GTMPBLOCK, ad);  //instead of adb
    else ad1=dodaj_obiekt(adb, ad);
    blok=(BLOK*)dane;
    //n1=blok->n;
    return ad1;
}

/*
 static BOOL add_block (void)
{
#ifndef LINUX
    BLOKD s_blockd = BDdef ;
#else
    BLOK s_blockd = Bdef ;
#endif
  BLOK *ptrs_block ;

  s_blockd.kod_obiektu = B_HATCH ;
  s_blockd.dlugosc_opisu_obiektu = 0 ;
  s_blockd.n = B3 + s_blockd.dlugosc_opisu_obiektu ;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  return TRUE ;
}
*/

///////////////
//sprintf(tekst_p,"%.2f",odl_r);

void sprintf_prec(char *string, double dokladnosc, double l)
{ double l1; /* l - wymiar */
  int lx,z;

  z=0;
//  if(dokladnosc-floor(dokladnosc)>0) z=1;
/*  l=floor(l/zmwym.dokladnosc+0.5)*zmwym.dokladnosc+z*zmwym.dokladnosc/10; */
  l1=floor(l/dokladnosc)*dokladnosc;
  if ((l-l1)>=(dokladnosc/2.))
  {
    l=l1+dokladnosc;
  }
  else l=l1;
//  sprintf(string,"%#g",l);
  sprintf(string,"%f",l);
  lx=(int)strlen(string);
  string [lx-z]='\0';
}
///////////////

//extern "C" { extern int add_000(char *tekst_p, int n_digit); }
int add_000(char *tekst_p, int n_digit)
{
char *i_point;
int l_zer;

  
  i_point=strrchr(tekst_p,'.');
  if (i_point)
    {
    l_zer=n_digit-(tekst_p+strlen(tekst_p)-i_point-1);
    if (l_zer>0)
     {
      for (int ii=0; ii<l_zer; ii++)
      {
       strncat(tekst_p,"0",60 - strlen(tekst_p) - 1);
      }
     }
      else //trzeba obciac
        {
          tekst_p[strlen(tekst_p)+l_zer]='\0';
        }
    }
    else
     {
      if (n_digit>0)
      {
       strncat(tekst_p,".",60 - strlen(tekst_p) - 1);
       for (int iii=0; iii<n_digit; iii++)  strncat(tekst_p,"0",60 - strlen(tekst_p) - 1);
      }
     }

    decimal(tekst_p);

    return 1;
}

static int remove_0(char *tekst_p)
{
char *i_point;
  
  i_point=(strrchr(tekst_p,'.'));
  if (i_point)
    {
      while (tekst_p[strlen(tekst_p)]=='\x60')
       {
        tekst_p[strlen(tekst_p)]='\x0';
       }
      if (tekst_p[strlen(tekst_p)]=='\x56') 
       {
        tekst_p[strlen(tekst_p)]='\x0';
       }
    }
    return 1;
}

static void parametry_ogolne_tekstu(double kierunek, double wysokosc, int kolor)
{ TextG.kat= (float)kierunek;
  TextG.wysokosc=(float)wysokosc;
  TextG.warstwa=Current_Layer;
  TextG.italics=0;
  TextG.width_factor=1;
  TextG.czcionka=zmwym.czcionka; //0/*1*/;
  TextG.obiektt1 = 0;
  TextG.obiektt2 = O2BlockPline;
  TextG.blok=1;
  TextG.typ=n_typ_normal;
  TextG.justowanie=j_do_lewej;
  TextG.kolor=kolor;
  TextG.ukryty=0;
}

static int expand_table_lines(double local_x, double max_x)
{   char  *adp, *adk, *adpb, *adkb, *adp1;
    NAGLOWEK *nag;
    LINIA *L;
    BLOK  *b, *ptrs_blok;
    int b_warstwa;
    T_Desc_Ex_Block *ptrs_desc_bl ;

    adp = dane ;
    adk = dane + dane_size ;

    adp1=find_block_sub_on(adp, adk, B_EXPORT, "#tp", 3);
    if (adp1!=NULL)
    {
        b=(BLOK *)adp1;
        //searching for horizontal lines in range
        adpb=adp1;
        adp=adp1+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
        adk=adkb=adp1+sizeof(NAGLOWEK)+b->n;

        nag = (NAGLOWEK*)adp;
        while (nag->obiekt != Okoniec && adp <= adk)
        {
            if (nag->obiekt == OdBLOK)
            {
                b=(BLOK *)adp;
                adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
            }
            else if (nag->obiekt == Olinia)
            {
                //checking if line is horizontal and in the range
                L=(LINIA*)adp;;
                if (Check_if_Equal(L->y1, L->y2))  //horizontal
                {
                    if (L->x2<L->x1)
                    {
                        float Lx2=L->x2;
                        L->x2=L->x1;
                        L->x1=Lx2;
                    }
                    if (((local_x-L->x1)>1.) && ((L->x2-local_x)>1.))  //horizontal line across entire table
                    {
                        if (L->x2<max_x) L->x2=(float)max_x;
                    }
                }
                adp+=sizeof(NAGLOWEK)+nag->n;
            }
            else  adp+=sizeof(NAGLOWEK)+nag->n;
            nag = (NAGLOWEK*)adp;
        }
        blokzap(adpb,adkb,ANieOkreslony,COPY_PUT,1);
        return 1;
    }
    return 0;
}

static int generowanie_bloku_hektometrow(double dane_global_rkp, double dane_global_ru, double dane_global_oi1, int insert_local, int check_table)
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
{ const double ht3=3.5;
  double odl_0;
  int i, l_h, l_k;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  int l_kr;
  int LengthT;
  int del_odl;
  double odl_1;
  double local_x, local_y;
  double first_hektometr;
  TEXT textg;

  memmove(&textg, &TextG, sizeof(TEXT));


  if (insert_local==1)
   {
    local_x=get_localx();
    local_y=get_localy();
   }
    else
     {
       local_x=0;
       local_y=0;
     }


   if (FALSE == add_block (B_HEKTOMETRY, 0))
   {
    return 0;
   }


   parametry_ogolne_tekstu(0, ht3,koloryp.hektometry);

/*   TextG.kat= 0;
   TextG.wysokosc=ht3;
   TextG.italics=0;
   TextG.width_factor=1;
   TextG.czcionka=zmwym.czcionka; //1;
   TextG.obiektt1 = 0;
   TextG.obiektt2 = O2BlockPline;;
   TextG.blok=1;     */


   first_hektometr=get_hektometr_p();


   odl_0=0;
   odl_1=jednostkiOb((dane_global_rkp-first_hektometr)*100)/dane_global_ru;
   del_odl=0;


   //ewentualny pierwszy tekst
    if (Check_if_Equal(first_hektometr,dane_global_rkp)==TRUE)
     {
      i=(int)dane_global_rkp;
      TextG.x=(float)(odl_0+del_odl+2.+local_x);
      TextG.y=(float)(-5.5+local_y);

      l_h = i % 10 ;
      l_k = i / 10 ;

      if (l_k>0)
       {
       // gcvt(l_k,ndig,tekst_p);
        sprintf(tekst_p,"%d",l_k);
        strncat(tekst_p,"+",60 - strlen(tekst_p) - 1);
       }
      else strcpy(tekst_p,"");

      //gcvt(l_h,ndig,tekst_p1);
      sprintf(tekst_p1,"%d",l_h);
      strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;

      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
     }


   for (i=(int)dane_global_rkp; i<(int)dane_global_oi1; i++)
   {
    odl_0=odl_1;
    odl_1=(jednostkiOb(100)/dane_global_ru)+odl_0;

    //pierwsza linia
    if (Check_if_Equal(odl_0,0.0)==TRUE)
    {
      if(!Lin(odl_0+local_x,0+local_y,odl_1-1+local_x,0+local_y,64,koloryp.hektometry)) return 0;
    }
    else
      {
        if(!Lin(odl_0+1+local_x,0+local_y,odl_1-1+local_x,0+local_y,64,koloryp.hektometry)) return 0;
      }

    /*rysowanie kolka o promieniu 1*/

    if(!Okr(odl_1+local_x,0+local_y,1,koloryp.hektometry)) return 0;

    /*kolejna wartosc*/

    TextG.x=(float)(odl_1+del_odl+local_x);
    TextG.y=(float)(-5.5+local_y);

    l_h = (i+1) % 10 ;
    l_k = (i+1) / 10 ;

    if (l_h==0)
      {
        if(!Okr(odl_1+local_x,0+local_y,0.5,koloryp.hektometry)) return 0;
      }

    if (l_k>0)
     {
       //gcvt(l_k,ndig,tekst_p);
       sprintf(tekst_p,"%d",l_k);
       strncat(tekst_p,"+",sizeof(tekst_p) - strlen(tekst_p) - 1);
     }
    else strcpy(tekst_p,"");

    //gcvt(l_h,ndig,tekst_p1);
    sprintf(tekst_p1,"%d",l_h);
    strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }


//  strcpy(&TextG.text[0],"");
  /*zapamietanie nowych danych o hektometrach*/
  dane_global_rkp=dane_global_oi1;
  l_kr=put_hektometr(dane_global_rkp,1);

  memmove(&TextG, &textg, sizeof(TEXT));

  //table horizontal lines correction
  if (check_table)
  {
      int int_exp = expand_table_lines(get_localx(), odl_1+get_localx());
  }

  return 1;
}

static int get_param1(MyDane *dane_prof0)
/*-------------------------------------*/
{int param1;

  param1=0;
  
  if (dane_prof0->odl!=0) param1 |= 1;
  if (dane_prof0->rki!=0) param1 |= 2;
  if (dane_prof0->rkp!=0) param1 |= 4;
  if (dane_prof0->ru!=0)  param1 |= 8;
  if (dane_prof0->rt!=0)  param1 |= 16;
  if (dane_prof0->si1!=0) param1 |= 32;
  if (dane_prof0->si2!=0) param1 |= 64;
  if (dane_prof0->si3!=0) param1 |= 128;
  if (dane_prof0->oi1!=0) param1 |= 256;
  if (dane_prof0->oi2!=0) param1 |= 512;
  if (dane_prof0->oi3!=0) param1 |= 1024;
  if (dane_prof0->sp1!=0) param1 |= 2048;
  if (dane_prof0->sp2!=0) param1 |= 4096;
  if (dane_prof0->sp3!=0) param1 |= 8192;
  if (dane_prof0->spi1!=0) param1 |= 16384;
  if (dane_prof0->spi2!=0) param1 |= 32768;
  if (dane_prof0->spi3!=0) param1 |= 65536;
  if (dane_prof0->opi1!=0) param1 |= 131072;
  if (dane_prof0->opi2!=0) param1 |= 262144;
  if (dane_prof0->opi3!=0) param1 |= 524288;
//  if (dane_prof0->spp1!=0) param1 |= 1048576;
  dane_prof0->param1 = param1;
  return param1;
}

static int Profil1H(void *BufVar)              /*KOLEJNY POMIAR - WARIANT STATYCZNY*/
{ const float ht1=2.5f; /*2.25*/ /* opisy pionowe */
  const float ht2=2.5f;  /* opisy poziome */
  const float ht3=3.5f;
  const double dt_y=0.8;
  char *material[]={"","beton","kamionka","żeliwo","PCV","tworzywo sztuczne","stal","PE",""};
  MyDane *dane_prof0=(MyDane*)BufVar;
  int i_f_handle ;

  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  int l_kr;
  double del_odl;


     double rpp;
     double max_r;

     double epsilon;
     double epsilon1;
     double suma_odl;

  MyDaneL punkt_i[6];
  MyDaneL punkt_p[6];

  int l_srednic_i, l_srednic_p, l_spadkow_i, l_spadkow_p;
  int l_punktow_i, l_punktow_p;

  double add_odl;
  double delta_odl;
  double war_odl;
  double sri_max;
  double srp_max;
  double rk_max;
  int i, si2i, oi1i;

  int odl_i;
  double odl_1;
  double odl_r;

  double reszta;
  int reszta_i;
  double odl_0;
  int l_h, l_k;
  double spadek_ki;
  double dlugosc_ki;
  double spadek_kp;
  double dlugosc_kp;

  double dane_prof_odl_ki;
  double dane_prof_odl_kp;
  double dane_prof0_odl_ki;
  double dane_prof0_odl_kp;

  BOOL linia_ki=FALSE, linia_kp=FALSE;
  int len_t1, len_t2, ii;
  int dno_os_i, dno_os_p;
  
  double zaglebienie_ki;
  double zaglebienie_kp;
  
  int op3;
  double local_y_;


  TEXT textg=Tdef;
  
//  char str1[80];
//  int dane_oi1;

  /***************************/

 /*beda potrzebne dodatkowe zmienne lokalne*/
 
  for (i=0; i<5; i++)
   {
     punkt_i[i].odl=0;
     punkt_p[i].odl=0;
   }  

  dane_global.oi2=0;  /*dynamika bloku*/
  l_kr=put_dynamic_block(dane_global.oi2);



  dane_global.ru=get_skala_profilu_x();
  dane_global.rt=get_poziom_pp();
  dane_global.odl=get_typ_profilu();
  tablica_0_1(dane_global.odl);
  /*sprawdzenie czy skala nie jest 0*/
  

  if (dane_global.ru==0)
  {
  ErrList(111);
  return 0;
  }

#ifdef SAVETMP
  //odczytanie danych z pliku binarnego
  i_f_handle = open (PROFIL_TEMP_FILE, O_BINARY | O_RDONLY, S_IREAD) ;
  if (i_f_handle == -1)
   {
   //zerowanie danych dane_profs1
  // dane_profs1=dane_profs0;
   memmove(&dane_profs1, &dane_profs0, sizeof(MyDane));
   }
    else
    {
     if (read (i_f_handle, &dane_profs1, sizeof(dane_profs1)) != sizeof(dane_profs1))
      {
      //zerowanie danych dane_profs1
     // dane_profs1=dane_profs0;
      memmove(&dane_profs1, &dane_profs0, sizeof(MyDane));
      }
     close (i_f_handle) ;
    }
#else
    memmove(&dane_profs1, &dane_profs0, sizeof(MyDane));
#endif

  epsilon = 0.01;       /*tolerancja dla odleglosci w [m]*/
  epsilon1 = 1.0;      /*tolerancja dla rzednych w promilach*/

  rpp=jednostkiOb(dane_global.rt);

  dane_global.rki=get_hektometr_p();
  dane_global.rkp=get_hektometr_k();
  

  dane_prof0->param1=get_param1(dane_prof0);

  dane_prof0->odl=72;

  dane_prof.odl=(jednostkiOb(dane_prof0->odl-(dane_global.rki*100))/dane_global.ru);
  dane_prof_odl_ki=0;
  dane_prof_odl_kp=0;
  dane_prof0_odl_ki=0;
  dane_prof0_odl_kp=0;

  dane_prof.rki=jednostkiOb(dane_prof0->rki);
  dane_prof.rkp=jednostkiOb(dane_prof0->rkp);
  dane_prof.ru=jednostkiOb(dane_prof0->ru);
  dane_prof.rt=jednostkiOb(dane_prof0->rt);
  

  /* poprzednie dane */
  dane_profs.odl=(jednostkiOb(dane_profs1.odl-(dane_global.rki*100))/dane_global.ru);
  dane_profs.rki=jednostkiOb(dane_profs1.rki);
  dane_profs.rkp=jednostkiOb(dane_profs1.rkp);
  dane_profs.ru=jednostkiOb(dane_profs1.ru);
  dane_profs.rt=jednostkiOb(dane_profs1.rt);

  dane_profs.spi3=(jednostkiOb(dane_profs1.spi3-(dane_global.rki*100))/dane_global.ru);  /* dla rki */
  dane_profs.opi1=(jednostkiOb(dane_profs1.opi1-(dane_global.rki*100))/dane_global.ru);  /* dla rkp */
  dane_profs.opi2=(jednostkiOb(dane_profs1.opi2-(dane_global.rki*100))/dane_global.ru);  /* dla ru */
  dane_profs.opi3=(jednostkiOb(dane_profs1.opi3-(dane_global.rki*100))/dane_global.ru);  /* dla rt */
//  dane_profs.spp1=dane_profs1.spp1;  /* rzedna tabelki */

  /*   */
  dno_os_i=0;
  dno_os_p=0;
  

  dane_prof.si1=jednostkiOb(dane_prof0->si1);   /*srednice k.i.*/
  if (dane_prof0->si1>10)
   {
   dane_prof.si1/=1000;
   dno_os_i=1;
   }

  dane_prof.si2=(dane_prof0->si2);   /*material dla k.i.*/

  if (dane_global.odl!=0)
   {
   dane_prof.rki=0;
   dane_prof0->rki=0;
   dane_prof.si1=0;
   dane_prof0->si1=0;
   dane_prof.si2=0;
   dane_prof0->si2=0;
   }
   else
    {
    }

  dane_prof.si3=jednostkiOb(dane_prof0->si3);   /*srednice k.p.*/
  if (dane_prof0->si3>10)
   {
   dane_prof.si3/=1000;
   dno_os_p=1;
   }


  dane_prof.oi1=(dane_prof0->oi1);   /*material dla k.p.*/

  dane_prof.oi2=(dane_prof0->oi2/1000);   /*spadki k.i.*/

  dane_prof.oi3=jednostkiOb(dane_prof0->oi3)/dane_global.ru;   /*dlugosci dla spadkow k.i.*/

  dane_prof.sp1=(dane_prof0->sp1/1000);   /*spadki k.p.*/

  dane_prof.sp2=jednostkiOb(dane_prof0->sp2)/dane_global.ru;   /*dlugosci dla spadkow k.p.*/
  
  dane_prof.sp3=jednostkiOb(dane_prof0->sp3);  //zaglebienie ki
  dane_prof.op1=jednostkiOb(dane_prof0->op1);  //zaglebienie kp
//  dane_prof.op2=dane_prof0->op2;  //opis zaglebienia ki
  dane_prof.op2=dane_prof0->op2;  //opis zaglebienia kp
  dane_prof.op3=dane_prof0->op3;  //rodzaj nawierzchni
  
  /* kontrola poprawnosci danych z ewentualnym wyznaczaniem brakujacych danych */
  

  if (dane_prof0->odl>0)
  {
   if (dane_prof0->odl==(dane_global.rki*100))
   /* zadana odleglosc jest rowna wartosci pierwszego hektometru */
   {
    ErrList(130);
    return 0;
   }

   if (dane_prof.odl<dane_profs.odl)
   {
    ErrList(107);
    return 0;
   }
  }
  /* sprawdzenie zgodnosci dlugosci dla ki i kp  */
  /*
  if ((dane_prof.oi3>0) && (dane_prof.sp2>0) && (dane_prof0->odl>0) && (dane_prof.oi3!=dane_prof.sp2))
    {
    ErrList(114);
    return 0;
    }
   */

  dane_global.oi1=(dane_prof0->odl / 100.);
  if (dane_global.oi1<dane_global.rki)
   {
   ErrList(128);
   return 0;
   }

  /*1. sprawdzenie czy wszystkie odleglosci>=0*/

   if (dane_prof.oi3<0)
     {
     ErrList(115);
     return 0;
     }

   if (dane_prof.sp2<0)
     {
     ErrList(116);
     return 0;
     }

   if (dane_prof0->odl==0)
    {
    if (dane_prof.ru>0)
     {
     if (dane_global.odl==0) ErrList(147); else ErrList(162);
     return 0;
     }
    if (dane_prof.rt>0)
     {
     if (dane_global.odl==0) ErrList(148); else ErrList(163);
     return 0;
     }
    }


  /* PRZYPADEK 1,2,4  odl>0 : */

  dlugosc_ki=0;
  dlugosc_kp=0;
  spadek_ki=0;
  spadek_kp=0;

  if (dane_prof0->odl>0) /* zadana wartosc odleglosci */
   {
   /* KANAL ISTNIEJACY */
   /* PRZYPADEK 1, rki>0 */

    if (dane_prof.rki>0)  /* zadana wartosc rzednej kanalu */
     {
      /* wyznaczenie odleglosci */
      dlugosc_ki=milimetryob((dane_prof.odl-dane_profs.spi3)*dane_global.ru);
      if (dlugosc_ki<=0)
       {
       /*uwaga*/
       ErrList(133);
       /* return 0;*/
       }
      /* sprawdzenie wartosci odleglosci zadanej */
      if (dane_prof.oi3>0)
       {
       if (fabs(milimetryob(dane_prof.oi3*dane_global.ru)-dlugosc_ki)>epsilon)
	{
	ErrList(134);
	return 0;
	}
       }
      /* wyznaczenie wartosci spadku w promilach*/
      if (TRUE == Check_if_Equal (dane_prof.odl, dane_profs.spi3))
    /*  if (dane_prof.odl==dane_profs.spi3) */
	spadek_ki=10000; /*wartosc umowna*/
       else
       {
        spadek_ki=((dane_profs.rki-dane_prof.rki)*1000)/((dane_prof.odl-dane_profs.spi3)*dane_global.ru);
       } 
      /* sprwadzenie wartosci zadanej spadku */
      if (dane_prof.oi2!=0)
       {
       if (fabs((dane_prof.oi2*1000)-spadek_ki)>epsilon1)  /* epsilon1 w promilach */
	{
	ErrList(135);
	return 0;
	}
       }
      dane_prof_odl_ki=dane_prof.odl;
      dane_prof0_odl_ki=dane_prof0->odl;
     }
     else  /* brak rzednej kanalu istniejacego */
     {
      if ((dane_prof.oi3>0) || (dane_prof.oi2!=0)) /* musi byc zadana dlugosc lub spadek <>0 */
      {
      /* wyznaczenie odleglosci */
      dlugosc_ki=milimetryob((dane_prof.odl-dane_profs.spi3)*dane_global.ru);
      if (dlugosc_ki<=0)
       {
       /*uwaga*/
       ErrList(133);
       /* return 0;*/
       }
      /* sprawdzenie wartosci odleglosci zadanej */
      if (dane_prof.oi3>0)
       {
       if (fabs(milimetryob(dane_prof.oi3*dane_global.ru)-dlugosc_ki)>epsilon)
	{
	ErrList(134);
	return 0;
	}
       }

      /* sprwadzenie wartosci zadanej spadku */

      if (dane_prof.oi2==0) /*spadek moze byc =0 ale musi byc podana dlugosc*/
       {
	/* uwaga */
	ErrList(136);
       }
      /* wyznaczenie rzednej koncowej na podstawie spadku w promilach */
      dane_prof.rki=dane_profs.rki-((dane_prof.odl-dane_profs.spi3)*dane_global.ru*(dane_prof.oi2));
      dane_prof0->rki=milimetryob(dane_prof.rki);
      spadek_ki=dane_prof.oi2*1000;

      dane_prof_odl_ki=dane_prof.odl;
      dane_prof0_odl_ki=dane_prof0->odl;

      /* dodatnia wartosc=spadek */
      }
     }
     
     /* koniec kanalu istniejacego */
    /* KANAL PROJEKTOWANY */
    /* ------------------ */
    /* PRZYPADEK 1, rkp>0 */
    if (dane_prof.rkp>0)  /* zadana wartosc rzednej kanalu */
     {
      /* wyznaczenie odleglosci */
      dlugosc_kp=milimetryob((dane_prof.odl-dane_profs.opi1)*dane_global.ru);
      if (dlugosc_kp<=0)
       {
       /*uwaga*/
       ErrList(137);
       /*return 0;*/
       }
      /* sprawdzenie wartosci odleglosci zadanej */
      if (dane_prof.sp2>0)
       {
       if (fabs(milimetryob(dane_prof.sp2*dane_global.ru)-dlugosc_kp)>epsilon)
	{
//	komunikat_str("1");delay(1000);
	ErrList(138);
	return 0;
	}
       }
      /* wyznaczenie wartosci spadku w promilach*/
      if (TRUE == Check_if_Equal (dane_prof.odl, dane_profs.opi1))
    /*  if (dane_prof.odl==dane_profs.opi1) */
       spadek_kp=10000; /*wartosc umowna*/
	else
	{
	 spadek_kp=((dane_profs.rkp-dane_prof.rkp)*1000)/((dane_prof.odl-dane_profs.opi1)*dane_global.ru);
	} 
      /* sprwadzenie wartosci zadanej spadku */
      if (dane_prof.sp1!=0)
       {
       if (fabs((dane_prof.sp1*1000)-spadek_kp)>epsilon1)
	{
	ErrList(139);
	return 0;
	}
       }
      dane_prof_odl_kp=dane_prof.odl;
      dane_prof0_odl_kp=dane_prof0->odl;
     }
     else  /* brak rzednej kanalu istniejacego */
     {
      if ((dane_prof.sp2>0) || (dane_prof.sp1!=0)) /* musi byc zadana dlugosc lug spadek <>0 */
      {
      /* wyznaczenie odleglosci */
      dlugosc_kp=milimetryob((dane_prof.odl-dane_profs.opi1)*dane_global.ru);
      if (dlugosc_kp<=0)
       {
       /*uwaga*/
       ErrList(137);
       /*return 0;*/
       }
      /* sprawdzenie wartosci odleglosci zadanej */
      if (dane_prof.sp2>0)
       {
       if (fabs(milimetryob(dane_prof.sp2*dane_global.ru)-dlugosc_kp)>epsilon)
	{
//	komunikat_str("2");delay(1000);
	ErrList(138);
	return 0;
	}
       }

      /* sprwadzenie wartosci zadanej spadku */

      if (dane_prof.sp1==0)
       {
	/* uwaga */
	ErrList(140);
       }
       

      /* wyznaczenie rzednej koncowej na podstawie spadku w promilach */
      dane_prof.rkp=dane_profs.rkp-((dane_prof.odl-dane_profs.opi1)*dane_global.ru*(dane_prof.sp1));
      dane_prof0->rkp=milimetryob(dane_prof.rkp);
      spadek_kp=dane_prof.sp1*1000;
      dane_prof_odl_kp=dane_prof.odl;
      dane_prof0_odl_kp=dane_prof0->odl;
      /* dodatnia wartosc=spadek */
      }
     }
     /* koniec kanalu projektowanego */

     /* tutaj nalezy dolozyc rzedne ulicy i terenu */
     /*    */
   }
   else  /* brak wartosci odleglosci tzn. odl=0 */
   {
    if (dane_prof0->odl<0)
     {
      ErrList(105);
      return 0;
      }

    if ((dane_prof.oi3==0) && (dane_prof.sp2==0))
     {
      ErrList(145);
      return 0;
     }

    /* odl=0 */
    /* +++++++++++++++++++++++ */
   /* KANAL ISTNIEJACY */
   /* PRZYPADEK 1, rki>0 */
   /* musi byc zadana dlugosc */
   
   
   if (dane_global.odl==0)
    {
    if (dane_prof.oi3==0)     /* dlugosc */
     {
     if (dane_prof.rki>0)     /* rzedna */
      {
      ErrList(141);
      return 0;
      }
      else
       if (dane_prof.oi2!=0)  /* spadek */
	{
	ErrList(142);
	return 0;
	}
     }


    if (dane_prof.oi3>0)  /* zadana dlugosc dla kanalu istniejacego */
    {
     /* wyznaczenie odleglosci i dlugosci */
     dane_prof_odl_ki=dane_profs.spi3+dane_prof.oi3;
     dane_prof0_odl_ki=(dane_global.rki*100)+milimetryob(dane_prof_odl_ki*dane_global.ru);
     dlugosc_ki=dane_prof0->oi3;

    if (dane_prof.rki>0)  /* zadana wartosc rzednej kanalu */
     {
      /* wyznaczenie wartosci spadku w promilach*/
      if (TRUE == Check_if_Equal (dane_prof_odl_ki, dane_profs.spi3))
      {
      spadek_ki=10000; /*wartosc umowna*/
      }
       else
       {
	spadek_ki=((dane_profs.rki-dane_prof.rki)*1000)/((dane_prof_odl_ki-dane_profs.spi3)*dane_global.ru);
       }	
      /* sprawadzenie wartosci zadanej spadku */
      if (dane_prof.oi2!=0)
       {
       if (fabs((dane_prof.oi2*1000)-spadek_ki)>epsilon1)  /* epsilon1 w promilach */
	{
	ErrList(135);
	return 0;
	}
       }
     }
     else  /* brak rzednej kanalu istniejacego */
     {
      /* wyznaczenie odleglosci */
      dlugosc_ki=milimetryob((dane_prof_odl_ki-dane_profs.spi3)*dane_global.ru);
      if (dlugosc_ki<=0)
       {
       ErrList(134);
       return 0;
       }
      /* sprawdzenie wartosci odleglosci zadanej */
      if (dane_prof.oi3>0)
       {
       if (fabs(milimetryob(dane_prof.oi3*dane_global.ru)-dlugosc_ki)>epsilon)
	{
	ErrList(134);
	return 0;
	}
       }

      /* sprwadzenie wartosci zadanej spadku */

      if (dane_prof.oi2==0)
       {
	/* uwaga */
	ErrList(136);
       }
      /* wyznaczenie rzednej koncowej na podstawie spadku w promilach */
      dane_prof.rki=dane_profs.rki-((dane_prof_odl_ki-dane_profs.spi3)*dane_global.ru*(dane_prof.oi2));
      dane_prof0->rki=milimetryob(dane_prof.rki);
      spadek_ki=dane_prof.oi2*1000;
      /* dodatnia wartosc=spadek */
     }
    }  /* zadana dlugosc */
   } 
     /* koniec kanalu istniejacego */
     /* kanal projektowany */
     
     
    if (dane_prof.sp2==0)     /* dlugosc */
     {
     if (dane_prof.rkp>0)     /* rzedna */
      {
      ErrList(143);
      return 0;
      }
      else
       if (dane_prof.sp1!=0)  /* spadek */
	{
	ErrList(144);
	return 0;
	}
     }

    if (dane_prof.sp2>0)  /* zadana dlugosc dla kanalu projektowanego */
    {
     /* wyznaczenie odleglosci i dlugosci */
     dane_prof_odl_kp=dane_profs.opi1+dane_prof.sp2;
     dane_prof0_odl_kp=(dane_global.rki*100)+milimetryob(dane_prof_odl_kp*dane_global.ru);
     dlugosc_kp=dane_prof0->sp2;

    if (dane_prof.rkp>0)  /* zadana wartosc rzednej kanalu */
     {
      /* wyznaczenie wartosci spadku w promilach*/
      if (TRUE == Check_if_Equal (dane_prof_odl_kp, dane_profs.opi1))
      {
      spadek_kp=10000; /*wartosc umowna*/
      }
       else
        {
	 spadek_kp=((dane_profs.rkp-dane_prof.rkp)*1000)/((dane_prof_odl_kp-dane_profs.opi1)*dane_global.ru);
	}
      /* sprawadzenie wartosci zadanej spadku */
      if (dane_prof.sp1!=0)
       {
       if (fabs((dane_prof.sp1*1000)-spadek_kp)>epsilon1)  /* epsilon1 w promilach */
	{
	ErrList(139);
	return 0;
	}
       }
     }
     else  /* brak rzednej kanalu projektowanego */
     {
      /* wyznaczenie odleglosci */
      dlugosc_kp=milimetryob((dane_prof_odl_kp-dane_profs.opi1)*dane_global.ru);
      if (dlugosc_kp<=0)
       {
//       komunikat_str("3");delay(1000);
       ErrList(138);
       return 0;
       }
      /* sprawdzenie wartosci odleglosci zadanej */
      if (dane_prof.sp2>0)
       {
       if (fabs(milimetryob(dane_prof.sp2*dane_global.ru)-dlugosc_kp)>epsilon)
	{
//	komunikat_str("4");delay(1000);
	ErrList(138);
	return 0;
	}
       }

      /* sprwadzenie wartosci zadanej spadku */

      if (dane_prof.sp1==0)
       {
	/* uwaga */
	ErrList(140);
       }
      /* wyznaczenie rzednej koncowej na podstawie spadku w promilach */
      dane_prof.rkp=dane_profs.rkp-((dane_prof_odl_kp-dane_profs.opi1)*dane_global.ru*(dane_prof.sp1));
      dane_prof0->rkp=milimetryob(dane_prof.rkp);
      spadek_kp=dane_prof.sp1*1000;
      /* dodatnia wartosc=spadek */
     }
    }  /* zadana dlugosc */


     /* koniec kanalu projektowanego */

     /* tutaj nalezy dolozyc rzedne ulicy i terenu */
     /*    */


    /* +++++++++++++++++++++++ */
   }

  /* ------------------------------ */

    /* ustalenie wartosci odleglosci */
    /* jezeli ustalone sa dlugosci ki i kp, to odl_ki ma byc = odl_kp */
  if ((dlugosc_ki>0) && (dlugosc_kp>0))
   {
   if (dane_prof0_odl_ki!=dane_prof0_odl_kp)
    {
    ErrList(146);
    return 0;
    }
   }

  if ((dane_prof0_odl_ki>0) || (dane_prof0_odl_kp>0))
  {
   if (dane_prof0_odl_ki>dane_prof0_odl_kp) dane_prof0->odl=dane_prof0_odl_ki;
     else dane_prof0->odl=dane_prof0_odl_kp;

   dane_prof.odl=(jednostkiOb(dane_prof0->odl-(dane_global.rki*100))/dane_global.ru);
  }


  /*ustalenie srednic*/

  l_srednic_i=0;
  if (dane_prof.si1>0) l_srednic_i=1;
  sri_max=0;
  if (l_srednic_i==1) sri_max=dane_prof.si1;

  l_srednic_p=0;
  if (dane_prof.si3>0) l_srednic_p=1;
  srp_max=0;
  if (l_srednic_p==1) srp_max=dane_prof.si3;

  l_spadkow_i=0;
  if (dane_prof.oi2!=0) l_spadkow_i=1;

  l_spadkow_p=0;
  if (dane_prof.sp1!=0) l_spadkow_p=1;



  /* WSZYSTKIE DANE DOTYCZACE ODLEGLOSCI O.K. */
  /* KANAL ISTNIEJACY */
  /********************/
  
  l_punktow_i=0;
  if ((dane_profs.rki>0) && (dane_prof.rki>0))
   {
    l_punktow_i=2;
    punkt_i[1].odl=dane_profs.spi3;
    punkt_i[1].rz=dane_profs.rki;
    punkt_i[1].s=dane_prof.si1;
    punkt_i[2].odl=dane_prof_odl_ki;
    punkt_i[2].rz=dane_prof.rki ;
    punkt_i[2].s=0;
   }

  /* KANAL PROJEKTOWANY*/
  /*******************/
  l_punktow_p=0;
  if ((dane_profs.rkp>0) && (dane_prof.rkp>0))
   {
    l_punktow_p=2;
    punkt_p[1].odl=dane_profs.opi1;
    punkt_p[1].rz=dane_profs.rkp;
    punkt_p[1].s=dane_prof.si3;
    punkt_p[2].odl=dane_prof_odl_kp;
    punkt_p[2].rz=dane_prof.rkp ;
    punkt_p[2].s=0;
   }

   /* wyznaczono wiec 2 punkty dla kanalu istniejacego i projektowanego*/


/* generowanie (ewentualne) bloku hektometrow */


   /* obliczenie nowej wartosci hektometrow si3 */

  /*zaokragloenie wartosci odleglosci w gore, do pelnego hektometra */

    dane_global.oi1 = ceil(dane_prof0->odl / 100);
//    dane_oi1 = (int) (dane_prof0->odl / 100) + 0.5;
//    dane_global.oi1 = dane_oi1;
  /*  */



  dane_global.si1=(jednostkiOb(dane_global.rki)/dane_global.ru);
  dane_global.si2=(jednostkiOb(dane_global.rkp)/dane_global.ru);  /* stare */
  dane_global.si3=(jednostkiOb(dane_global.oi1)/dane_global.ru);  /* nowe */

  if ((dane_global.odl==0) || (dane_global.odl==2))
  {
  
//   sprintf(str1,"%.3f  %.3f  %.3f",dane_global.rkp, dane_global.ru, dane_global.oi1);
//   komunikat_str(str1);
//   Get_Key();
  

  if (dane_global.si3>dane_global.si2)
   {
   /* dodanie nowego segmentu hektomatrow  */
   

   l_kr=generowanie_bloku_hektometrow(dane_global.rkp, dane_global.ru, dane_global.oi1, 0, 1);
   if (l_kr==0) return 0;
 /*
   if (FALSE == add_block (B_PLINE, 0))
   {
    return 0;
   }

   TextG.kat= 0;
   TextG.wysokosc=ht3;
   TextG.italics=0;
   TextG.width_factor=1;
   TextG.czcionka=zmwym.czcionka; //1;
   TextG.obiektt1 = 0;
   TextG.obiektt2 = O2BlockPline;;
   TextG.blok=1;



   del_odl=0;
   odl_1=jednostkiOb(dane_global.rkp*100)/dane_global.ru;
   odl_0=0;


   for (i=int(dane_global.rkp); i<int(dane_global.oi1); i++)
   {
    odl_0=odl_1;
    odl_1=(jednostkiOb(100)/dane_global.ru)+odl_0;

    if(!Lin(odl_0+1,0,odl_1-1,0,5,7)) return 0;


    if(!Okr(odl_1,0,1,7)) return 0;


    TextG.x=odl_1+del_odl;
    TextG.y=-5.5;

    l_h = (i+1) % 10 ;
    l_k = (i+1) / 10 ;

    if (l_h==0)
      {
       if(!Okr(odl_1,0,0.5,7)) return 0;
      }

    if (l_k>0)
     {
     // gcvt(l_k,ndig,tekst_p);
      sprintf(tekst_p,"%d",l_k);
      strncat(tekst_p,"+",1);
     }
      else strcpy(tekst_p,"");

    //gcvt(l_h,ndig,tekst_p1);
    sprintf(tekst_p1,"%d",l_h);
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

  strcpy(&TextG.text[0],"");
  dane_global.rkp=dane_global.oi1;
  l_kr=put_hektometr(dane_global.rkp,1);
  */
   }
  }  /* typ profilu 0*/
  

/* **** */



  if (FALSE == add_block_with_type (dane_prof.odl, 0, "*KP", 0, B_KPOMIAR))
  {
    return 0;
  }


  /*jezeli dane_prof.odl=dane_profs  to nie sa rysowane linie ale tylko wartosci rzednych z prawej strony osi odcietej*/
  

  if (dane_prof.odl==dane_profs.odl)
  {
  del_odl=3.5;
  }
  else
  {
  del_odl=-1;
  }
  
  if (dane_prof0->odl==(dane_global.rki*100)) //pierwszy hektometr
  {
   del_odl=3.5;
  }

  memmove(&textg, &TextG, sizeof(TEXT));
  
  parametry_ogolne_tekstu(Pi/2, ht1,koloryp.rzedne);
/*  TextG.kat= Pi/2;
  TextG.wysokosc=ht1;
  TextG.italics=0;
  TextG.width_factor=1;
  TextG.czcionka=zmwym.czcionka; //1;
  TextG.obiektt1 = 0;
  TextG.obiektt2 = O2BlockPline;
  TextG.blok=1;   */

  if ((dane_global.odl==0) || (dane_global.odl==2))
  {
  /*tekst 1  ODLEGLOSC*/
  TextG.x=(float)(dane_prof.odl+del_odl);
  TextG.y=1.f+dt_y;

  odl_i=(int) dane_prof0->odl;
  odl_1=(odl_i % 100);
  odl_r=odl_1+(dane_prof0->odl-odl_i);


  //gcvt(odl_r,ndig,tekst_p);

  //sprintf(tekst_p,"%.2f",odl_r);
  sprintf_prec(tekst_p,odleglosc_prec,odl_r);

  //l_kr=add_000(tekst_p,2);
  l_kr=add_000(tekst_p,odleglosc_poz);

  strcpy(&TextG.text[0],tekst_p);
  LengthT=strlen(TextG.text);
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_odleglosc;


/*  if (dodaj_obiekt((NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

//  if(!Lin(dane_prof.odl,0,dane_prof.odl,10,64,koloryp.os_pomiaru)) return 0;

  //linia pionowa na wysokosc tabeli
  if(!Lin(dane_prof.odl,0,dane_prof.odl,tablica_y[14]/*78*/,64,koloryp.os_pomiaru)) return 0;
 
  }

  if (dane_global.odl==0)
  {

  /*srednice / dlugosci  tekst2*/
  /*spadki / dlugosci    tekst3*/
  

  if (dane_prof.rki>0)
  {
//  if(!Lin(dane_prof.odl,24,dane_prof.odl,34,64,koloryp.os_pomiaru)) return 0;
  /*tekst 4  rzedne kanalu istniejacego*/
  TextG.x=dane_prof.odl+del_odl;
  TextG.y=24+dt_y;
  //gcvt(dane_prof0->rki,ndig,tekst_p);
  sprintf_prec(tekst_p,rzedna_i_prec,dane_prof0->rki);
  l_kr=add_000(tekst_p,rzedna_i_poz);

  strcpy(&TextG.text[0],tekst_p);
  LengthT=strlen(TextG.text);
  if (LengthT > 6) TextG.width_factor=0.85f;
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_rzedna_kanalu_i;

/*  if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  TextG.width_factor=1;



  if (dno_os_i==0)
   {
    {if (!Punkt(punkt_i[2].odl,tablica_y[14]+(punkt_i[2].rz-rpp),1,koloryp.l_kanal_i,O2BlockPline,O3Dno)) return 0;}
    {if (!Punkt(punkt_i[2].odl,tablica_y[14]+(punkt_i[2].rz-rpp)+punkt_i[1].s,2,koloryp.l_kanal_i,O2BlockPline,O3Dno)) return 0;}
   }
    else
	{
	 {if (!Punkt(punkt_i[2].odl,tablica_y[14]+(punkt_i[2].rz-rpp)-(punkt_i[1].s/2),1,koloryp.l_kanal_i,O2BlockPline,O3Os)) return 0;}
	 {if (!Punkt(punkt_i[2].odl,tablica_y[14]+(punkt_i[2].rz-rpp)+(punkt_i[1].s/2),2,koloryp.l_kanal_i,O2BlockPline,O3Os)) return 0;}
	}

  }
  } /* typ profilu 0*/
  /*srednice / dlugosci  tekst5*/
  /*spadki / dlugosci    tekst6*/
  

  if (dane_prof.rkp>0)
  {
//  if(!Lin(dane_prof.odl,tablica_y[11]/*49*/,dane_prof.odl,tablica_y[12],64,koloryp.os_pomiaru)) return 0;
  /*tekst 7  rzedne kanalu projektowanego*/
  TextG.x=dane_prof.odl+del_odl;
  if ((dane_global.odl==0) || (dane_global.odl==2)) TextG.y=tablica_y[11] /*49*/ +dt_y;
    else  TextG.y=tablica_y[11] /*49*/ + dt_y;
  //gcvt(dane_prof0->rkp,ndig,tekst_p);
  sprintf_prec(tekst_p,rzedna_p_prec,dane_prof0->rkp);
  l_kr=add_000(tekst_p,rzedna_p_poz);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=strlen(TextG.text);
  if (LengthT > 6) TextG.width_factor=0.85;
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_rzedna_kanalu_p;

 /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  TextG.width_factor=1;

  if (dno_os_p==0)
   {
    {if (!Punkt(punkt_p[2].odl,tablica_y[14]+(punkt_p[2].rz-rpp),3,koloryp.l_kanal_p,O2BlockPline,O3Dno)) return 0;}
    {if (!Punkt(punkt_p[2].odl,tablica_y[14]+(punkt_p[2].rz-rpp)+punkt_p[1].s,4,koloryp.l_kanal_p,O2BlockPline,O3Dno)) return 0;}
   }
    else
	{
	 {if (!Punkt(punkt_p[2].odl,tablica_y[14]+(punkt_p[2].rz-rpp)-(punkt_p[1].s/2),3,koloryp.l_kanal_p,O2BlockPline,O3Os)) return 0;}
	 {if (!Punkt(punkt_p[2].odl,tablica_y[14]+(punkt_p[2].rz-rpp)+(punkt_p[1].s/2),4,koloryp.l_kanal_p,O2BlockPline,O3Os)) return 0;}
	}

  }
  
  
  if (dane_global.odl==2)  /* typ profilu 2 */
   {
    // zaglebienie kanalu projektowanego
    if (dane_prof.op2==0)  //od terenu projektowanego
     {
      zaglebienie_kp = dane_prof0->ru - dane_prof0->rkp; 
     }
     else if (dane_prof.op2==1) //od terenu istniejacego
       {
         zaglebienie_kp = dane_prof0->rt - dane_prof0->rkp; 
       }
        else zaglebienie_kp = 0;
     
    if (zaglebienie_kp>0)
     {
//      if(!Lin(dane_prof.odl,24,dane_prof.odl,34,64,koloryp.os_pomiaru)) return 0;
       /*tekst 4  zaglebienie kanalu projektowanego*/
      TextG.x=(float)(dane_prof.odl+del_odl);
      TextG.y=(float)(24.+dt_y);
       //gcvt(dane_prof0->rki,ndig,tekst_p);
      sprintf_prec(tekst_p,zaglebienie_prec,zaglebienie_kp);
      l_kr=add_000(tekst_p,zaglebienie_poz);
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      if (LengthT > 6) TextG.width_factor=0.85f;
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.typ=n_zaglebienie_kanalu_p;
      if (dane_prof.op2==1) TextG.obiektt3=O3Teren_i;
        else TextG.obiektt3=O3Teren_p;

      /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.width_factor=1;
     }
   }
  /**/


  if (dane_prof.ru>0)
  {
//  if(!Lin(dane_prof.odl,tablica_y[12]/*60*/,dane_prof.odl,tablica_y[13]/*69*/,64,koloryp.os_pomiaru)) return 0;
  /*tekst 8  rzedne ulicy*/
  TextG.x=(float)(dane_prof.odl+del_odl);
  TextG.y=(float)(tablica_y[12]/*60*/+dt_y);
  //gcvt(dane_prof0->ru,ndig,tekst_p);
  sprintf_prec(tekst_p,rzedna_tp_prec,dane_prof0->ru);
  l_kr=add_000(tekst_p,rzedna_tp_poz);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  if (LengthT > 6) TextG.width_factor=0.85f;
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_rzedna_ulicy;

/*  if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  TextG.width_factor=1;

  if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.ru-rpp),5,koloryp.l_ulicy,O2BlockPline,O3Dno)) return 0;
	    
  }
 
 
  if (dane_prof.rt>0)
  {
//  if(!Lin(dane_prof.odl,tablica_y[13]/*69*/,dane_prof.odl,tablica_y[14]/*78*/,64,koloryp.os_pomiaru)) return 0;
  /*tekst 8  rzedne terenu*/
  TextG.x=(float)(dane_prof.odl+del_odl);
  TextG.y=(float)(tablica_y[13]/*69*/+dt_y);
 // gcvt(dane_prof0->rt,ndig,tekst_p);
  sprintf_prec(tekst_p,rzedna_ti_prec,dane_prof0->rt);
  l_kr=add_000(tekst_p,rzedna_ti_poz);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  if (LengthT > 6) TextG.width_factor=0.85f;
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.obiektt3=O3NormalText;
  TextG.typ=n_rzedna_terenu;

/*  if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  TextG.width_factor=1;

  if(!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rt-rpp),6,koloryp.l_teren_i,O2BlockPline,O3Dno)) return 0;
    
  }

  TextG.typ=n_typ_normal;
  
  if (dane_global.odl==2)  /* typ profilu 2 */
   {
    // punkty charakterystyczne  comments_tmp[1]
    //******** na razie wpisywany jest pusty lancuch
    strcpy(comments_tmp[1], "");
//    if (strlen(comments_tmp[1]) > 0)
//    {
     TextG.y=(float)(18.0+dt_y);
     strcpy(&TextG.text[0],comments_tmp[1]);
     LengthT=(int)strlen(TextG.text);
     len_t1=LengthT;
     len_t2=0;
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
     TextG.x=(float)(dane_prof.odl + 2.);
     TextG.kat= 0.f;
     TextG.wysokosc=ht3;
     TextG.czcionka=zmwym.czcionka; //0;  //to mozna zmienic
     TextG.justowanie=j_do_lewej ;   //j_srodkowo;
     TextG.kolor=koloryp.spadek_l_fi_mat;

     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
     TextG.justowanie=j_do_lewej;
     TextG.wysokosc=ht2;
//    }
    
    // rodzaj nawierzchni
    if (dane_prof.op3>0)
     {
        op3=(int)dane_prof.op3;
        
        switch (op3)
        {
         case 0: strcpy(tekst_p1,"");
	 break;
         case 1: strcpy(tekst_p1,_ziemna_);
 	 break;
         case 2: strcpy(tekst_p1,_asfaltowa_);
  	 break;
         case 3: strcpy(tekst_p1,_betonowa_);
	 break;
         case 4: strcpy(tekst_p1,_kostka_);
	 break;
         case 5: strcpy(tekst_p1,_brukowa_);
	 break;
         default: strcpy(tekst_p1,"");
         break;
        }

      if (strlen(tekst_p1)>0)
      {
       strcpy(&TextG.text[0],tekst_p1);
       TextG.y=(float)(11.5/*12*/+dt_y);
       LengthT=(int)strlen(TextG.text);
       TextG.dl=LengthT;
       TextG.n=T18+TextG.dl;
       TextG.x=(float)(dane_prof.odl - 5.);  //nalezy raczej justowac centralnie
       TextG.kat= 0.f;
       TextG.wysokosc=ht2;
       TextG.justowanie = j_do_prawej;
       TextG.czcionka=zmwym.czcionka; //0;
       if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      } 
     }
   } 
   


/*dopisanie srednicy i materialu kanalu istniejacego*/
   if (l_srednic_i>0)
   {
   /*pierwsza srednica*/
    TextG.y=(float)(11.5/*12*/+dt_y);
    strcpy(tekst_p,u8"Ø");  //instead of ⌀ due to lack in Arial font
    //gcvt(dane_prof0->si1,ndig,tekst_p1);
    sprintf(tekst_p1,"%g",dane_prof0->si1);
 //   if (dane_prof0->si1<=10)
 //    {
 //     l_kr=add_000(tekst_p1,1);
 //    }
    decimal(tekst_p1);

    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    if (dane_prof0->si1>10)
     {
     strcpy(tekst_p1," mm");
     strncat(tekst_p,tekst_p1,strlen(tekst_p1));
     }
    if (dane_prof.si2>0)
     {
      strcpy(tekst_p1," ");
      strncat(tekst_p,tekst_p1,1);
      si2i=(int) dane_prof.si2;
      if (si2i<8)
       {
         strcpy(tekst_p1,material[si2i]);
         strncat(tekst_p,tekst_p1,strlen(tekst_p1));
       }
       else if (si2i==8) //material inny definiowany
        {
          if (strlen(comments_tmp[2]) > 0)
           {
             strcpy(tekst_p1,comments_tmp[2]);
             if (strcmp(tekst_p1,"?")!=0) strncat(tekst_p,tekst_p1,strlen(tekst_p1));
           }
        }
    /*  switch (si2i)
      {
      case 0: strcpy(tekst_p1,"");
	 break;
      case 1: strcpy(tekst_p1,"beton");
	 break;
      case 2: strcpy(tekst_p1,"kamionka");
	 break;
      case 3: strcpy(tekst_p1,"żeliwo");
	 break;
      case 4: strcpy(tekst_p1,"PCV");
	 break;
      case 5: strcpy(tekst_p1,"tworzywo sztuczne");
	 break;
      case 6: strcpy(tekst_p1,"stal");
	 break;
      default: strcpy(tekst_p1,"");
	       break;
      } */

     }

    strcpy(&TextG.text[0],tekst_p);

    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.x=(float)(dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5));
    TextG.kat= 0.f;
    TextG.wysokosc=ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_srodkowo;
    TextG.typ=n_srednica_kanalu_i;
    /*    if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

//    if(!Lin(dane_prof_odl_ki,10,dane_prof_odl_ki,17,64,koloryp.os_pomiaru)) return 0;
   }


/* dopisanie srednicy i materialu kanalu projektowanego*/
   if (l_srednic_p>0)
   {
    TextG.y=(float)(tablica_y[8]/*37*/+dt_y);
    strcpy(tekst_p,Fi);
  //  gcvt(dane_prof0->si3,ndig,tekst_p1);
    sprintf(tekst_p1,"%g",dane_prof0->si3);
//////  if (dane_prof0->si3<=10)
//////   {
//////     l_kr=add_000(tekst_p1,1);
//////   }
    decimal(tekst_p1);

    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    if (dane_prof0->si3>10)
     {
     strcpy(tekst_p1," mm");
     strncat(tekst_p,tekst_p1,strlen(tekst_p1));
     }
    if (dane_prof.oi1>0)
     {
      strcpy(tekst_p1," ");
      strncat(tekst_p,tekst_p1,1);
      oi1i=(int) dane_prof.oi1;
      if (oi1i <8 )
       {
         strcpy(tekst_p1,material[oi1i]);
         strncat(tekst_p,tekst_p1,strlen(tekst_p1));
       }
       else if (oi1i==8) //material inny definiowany
        {
          if (strlen(comments_tmp[2]) > 0)
           {
             strcpy(tekst_p1,comments_tmp[2]);
             if (strcmp(tekst_p1,"?")!=0) strncat(tekst_p,tekst_p1,strlen(tekst_p1));
           }
        }
     }

    strcpy(&TextG.text[0],tekst_p);

    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.x=(float)(dane_prof_odl_kp-((dane_prof_odl_kp-dane_profs.opi1)*0.5));
    TextG.kat= 0.f;
    TextG.wysokosc=ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_srodkowo;
    TextG.typ=n_srednica_kanalu_p;
/*  if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

//    if(!Lin(dane_prof_odl_kp,tablica_y[7]/*35*/,dane_prof_odl_kp,tablica_y[9]/*42*/,64,koloryp.os_pomiaru)) return 0;
   }

  strcpy(&TextG.text[0],"");
  

 /*DOPISANIE SPADKOW*/
 /* dopisanie spadku i dlugosci kanalu istniejacego */
 /*wartosc spadku podawana jest zawsze jezeli rysowana jest linia spadku*/
   if (linia_ki==TRUE /*l_spadkow_i>0*/)
   {
    TextG.y=(float)(18.5+dt_y);
    strcpy(tekst_p,"");
    //gcvt(fabs(spadek_ki),ndig,tekst_p1);
    sprintf(tekst_p1,"%.2f",fabs(spadek_ki));
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    len_t1=LengthT;
    len_t2=0;
    /****/
      if (dlugosc_ki>0)
       {
	strcpy(tekst_p,"l=");
	//gcvt(dlugosc_ki,ndig,tekst_p1);
	sprintf(tekst_p1,"%.2f",dlugosc_ki);
	strncat(tekst_p,tekst_p1,strlen(tekst_p1));
	len_t2=(int)strlen(tekst_p);
       }
    /****/
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    /*polozenie tekstu w polowie pomiedzy pomiarami*/
    TextG.x=(float)(dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5)-5.);
    TextG.kat= 0;
    TextG.wysokosc=ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_do_prawej;
    TextG.kolor=koloryp.spadek_l_fi_mat;
    TextG.typ=n_spadek_kanalu_i;
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;


    if (dlugosc_ki>0)
     {
      strcpy(tekst_p,"l=");
      //gcvt(dlugosc_ki,ndig,tekst_p1);
      sprintf(tekst_p1,"%.2f",dlugosc_ki);
      strncat(tekst_p,tekst_p1,strlen(tekst_p1));
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
     /*polozenie tekstu w polowie pomiedzy pomiarami*/
      TextG.x=(float)(dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5)+5.);
      TextG.kat= 0;
      TextG.wysokosc=ht2;
      TextG.czcionka=zmwym.czcionka; //0;
      TextG.justowanie=j_do_lewej;
      TextG.typ=n_dlugosc_kanalu_i;
      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
     }

      /* dopisanie linii ukosnej */
    if (spadek_ki>0)   /*   \   */
     {
      if(!Lin(TextG.x-9,TextG.y+4,TextG.x-1,TextG.y-1.5,64,koloryp.spadek_l_fi_mat)) return 0;
     }
     else
     if (spadek_ki<0)  /*  /  */
      {
       if(!Lin(TextG.x-9,TextG.y-1.5,TextG.x-1,TextG.y+4,64,koloryp.spadek_l_fi_mat)) return 0;
      }
      else if(!Lin(TextG.x-9,TextG.y+2,TextG.x-1,TextG.y+2,64,koloryp.spadek_l_fi_mat)) return 0;
    if(!Lin(dane_prof_odl_ki,17,dane_prof_odl_ki,24,64,koloryp.os_pomiaru)) return 0;
   }
   


/* dopisanie spadku i dlugosci kanalu projektowanego */
   if (linia_kp==TRUE /*l_spadkow_p>0*/)
   {
    TextG.y=(float)(tablica_y[10]/*43.5*/+dt_y);
    strcpy(tekst_p,"");
//    gcvt(fabs(spadek_kp),ndig,tekst_p1);
    sprintf(tekst_p1,"%.2f",fabs(spadek_kp));
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    len_t1=LengthT;
    len_t2=0;
    if (dlugosc_kp>0)
     {
      strcpy(tekst_p,"l=");
     // gcvt(dlugosc_kp,ndig,tekst_p1);
      sprintf(tekst_p1,"%.2f",dlugosc_kp);
      strncat(tekst_p,tekst_p1,strlen(tekst_p1));
      len_t2=(int)strlen(tekst_p);
     }


    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.x=(float)(dane_prof_odl_kp-((dane_prof_odl_kp-dane_profs.opi1)*0.5)-5.);
    TextG.kat= 0;
    TextG.wysokosc=ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_do_prawej;
    TextG.kolor=koloryp.spadek_l_fi_mat;
    TextG.typ=n_spadek_kanalu_p;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

    if (dlugosc_kp>0)
     {
      strcpy(tekst_p,"l=");
      //gcvt(dlugosc_kp,ndig,tekst_p1);
      sprintf(tekst_p1,"%.2f",dlugosc_kp);
      strncat(tekst_p,tekst_p1,strlen(tekst_p1));
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.x=(float)(dane_prof_odl_kp-((dane_prof_odl_kp-dane_profs.opi1)*0.5)+5.);
      TextG.kat= 0;
      TextG.wysokosc=ht2;
      TextG.czcionka=zmwym.czcionka; //0;
      TextG.justowanie=j_do_prawej;
      TextG.kolor=koloryp.spadek_l_fi_mat;
      TextG.typ=n_dlugosc_kanalu_p;

      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
     }
    /* dopisanie linii ukosnej */
    if (spadek_kp>0)   /*   \   */
     {
      if(!Lin(TextG.x-9,TextG.y+4,TextG.x-1,TextG.y-1.5,64,koloryp.spadek_l_fi_mat)) return 0;
     }
     else
     if (spadek_kp<0)  /*  /  */
      {
       if(!Lin(TextG.x-9,TextG.y-1.5,TextG.x-1,TextG.y+4,64,koloryp.spadek_l_fi_mat)) return 0;
      }
      else
       if(!Lin(TextG.x-9,TextG.y+2,TextG.x-1,TextG.y+2,64,koloryp.spadek_l_fi_mat)) return 0;

    if(!Lin(dane_prof_odl_kp,tablica_y[9]/*42*/,dane_prof_odl_kp,tablica_y[11]/*49*/,64,koloryp.os_pomiaru)) return 0;
   }

  strcpy(&TextG.text[0],"");
  TextG.justowanie=j_do_lewej;

//  if (FALSE == add_block (B_PLINE, 0))
//   {
//    return 0;
//   }

  /*  generowanie linii rzednych*/
    if ((dane_profs.rki==0) && (dane_prof.rki==0)) rk_max=0;
     else if (dane_profs.rki==0) rk_max=(double)dane_prof.rki;
       else if (dane_prof.rki==0) rk_max=dane_profs.rki;
	 else rk_max=dane_prof.rki;
    if ((rk_max>0) && (dane_prof.si1>0)) rk_max+=dane_prof.si1;

  rk_max+=sri_max;
  max_r=rk_max;

    if ((dane_profs.rkp==0) && (dane_prof.rkp==0)) rk_max=0;
     else if (dane_profs.rkp==0) rk_max=(double)dane_prof.rkp;
       else if (dane_prof.rkp==0) rk_max=dane_profs.rkp;
	 else rk_max=dane_prof.rkp;

    if ((rk_max>0) && (dane_prof.si3>0)) rk_max+=dane_prof.si3;

  rk_max+=srp_max;
  if (rk_max>max_r) max_r=rk_max;

   if (dane_prof.ru==0) rk_max=0;
     else rk_max=dane_prof.ru;


  if (rk_max>max_r) max_r=rk_max;

   if (dane_prof.rt==0) rk_max=0;
     else rk_max=dane_prof.rt;

  if (rk_max>max_r) max_r=rk_max;

  //linia pionowa cienka
  if(!Lin(dane_prof.odl,tablica_y[14]/*78*/,dane_prof.odl,tablica_y[14]/*78*/+(max_r-rpp),32,koloryp.os_pomiaru)) return 0;
  

/*  linia kanalu istniejacego (powinno byc linia b.gruba), nie jest rysowana jezeli punkt_i[1].odl=punkt_i[2].odl*/
  linia_ki=FALSE;
  if ((punkt_i[2].odl>0) && (punkt_i[2].odl>punkt_i[1].odl))
   {

    /*nowa wersja :  mamy punkt_i[l_punktow_i]*/
    if ((dane_profs.rki==0) && (dane_prof.rki==0))  /*nic nie jest rysowane*/
    {
    }
    else
    if ((dane_profs.rki==0) || (dane_prof.rki==0)) /*nic nie jest rysowane*/
    {
    /*  l_kr=0;
      if (dane_prof.rki==0) { dane_prof.rki=dane_profs.rki; l_kr=1;}
      if (dane_profs.rki==0) { dane_profs.rki=dane_prof.rki; l_kr+=1;}
      if(!Lin(dane_profs.odl,78+(dane_profs.rki-rpp),dane_prof.odl,78+(dane_prof.rki-rpp),11,7)) return 0;
      */
    }

    else   /*rysowane sa kolejno wszystkie punkty*/
    {
    for (i=1; i<l_punktow_i; i++)
     {
     if (dno_os_i==0)
      {
       {if(!Lin_special(punkt_i[i].odl,tablica_y[14]/*78*/+(punkt_i[i].rz-rpp),punkt_i[i+1].odl,tablica_y[14]/*78*/+(punkt_i[i+1].rz-rpp),96,koloryp.l_kanal_i)) return 0;}
       {if(!Lin_special(punkt_i[i].odl,tablica_y[14]/*78*/+(punkt_i[i].rz-rpp)+punkt_i[i].s,punkt_i[i+1].odl,tablica_y[14]/*78*/+(punkt_i[i+1].rz-rpp)+punkt_i[i].s,96,koloryp.l_kanal_i)) return 0;}
      }
      else
	{
	 {if(!Lin_special(punkt_i[i].odl,tablica_y[14]/*78*/+(punkt_i[i].rz-rpp)-(punkt_i[i].s/2),punkt_i[i+1].odl,tablica_y[14]/*78*/+(punkt_i[i+1].rz-rpp)-(punkt_i[i].s/2),96,koloryp.l_kanal_i)) return 0;}
	 {if(!Lin_special(punkt_i[i].odl,tablica_y[14]/*78*/+(punkt_i[i].rz-rpp)+(punkt_i[i].s/2),punkt_i[i+1].odl,tablica_y[14]/*78*/+(punkt_i[i+1].rz-rpp)+(punkt_i[i].s/2),96,koloryp.l_kanal_i)) return 0;}
	}
     linia_ki=TRUE;
     /*dodatkowo rysowanie linii cienkiej w miejscu zmiany srednicy lub spadku*/
     /*w tym celu przydatna jest informacja czy jest to zmiana srednicy czy spadku*/
     }
    }
   }
   

/*  linia kanalu projektowanego (powinno byc linia b.gruba), nie jest rysowana jezeli punkt_p[1].odl=punkt_p[2].odl*/
/* jezeli dno_os_p=1 to rzedna jest w osi kanau */
  linia_kp=FALSE;
  
  if ((punkt_p[2].odl>0) && (punkt_p[2].odl>punkt_p[1].odl))
   {
    if ((dane_profs.rkp==0) && (dane_prof.rkp==0)) /*nic nie jest rysowane*/
    {
    }
    else
    if ((dane_profs.rkp==0) || (dane_prof.rkp==0)) /*nic nie jest rysowane*/
    {
    /*  l_kr=0;
      if (dane_prof.rkp==0) { dane_prof.rkp=dane_profs.rkp; l_kr=1;}
      if (dane_profs.rkp==0) { dane_profs.rkp=dane_prof.rkp; l_kr+=1;}
      if(!Lin(dane_profs.odl,tablica_y[14]+(dane_profs.rkp-rpp),dane_prof.odl,tablica_y[14]+(dane_prof.rkp-rpp),11,7)) return 0;
      */
    }

    else  /*rysowane sa kolejno wszystkie punkty*/
    {
    for (i=1; i<l_punktow_p; i++)
     {
     if (dno_os_p==0)
      {
	{if(!Lin_special(punkt_p[i].odl,tablica_y[14]/*78*/+(punkt_p[i].rz-rpp),punkt_p[i+1].odl,tablica_y[14]/*78*/+(punkt_p[i+1].rz-rpp),96,koloryp.l_kanal_p)) return 0;}
	{if(!Lin_special(punkt_p[i].odl,tablica_y[14]/*78*/+(punkt_p[i].rz-rpp)+punkt_p[i].s,punkt_p[i+1].odl,tablica_y[14]/*78*/+(punkt_p[i+1].rz-rpp)+punkt_p[i].s,96,koloryp.l_kanal_p)) return 0;}
      }
      else
       {
	{if(!Lin_special(punkt_p[i].odl,tablica_y[14]/*78*/+(punkt_p[i].rz-rpp)-(punkt_p[i].s/2),punkt_p[i+1].odl,tablica_y[14]/*78*/+(punkt_p[i+1].rz-rpp)-(punkt_p[i].s/2),96,koloryp.l_kanal_p)) return 0;}
	{if(!Lin_special(punkt_p[i].odl,tablica_y[14]/*78*/+(punkt_p[i].rz-rpp)+(punkt_p[i].s/2),punkt_p[i+1].odl,tablica_y[14]/*78*/+(punkt_p[i+1].rz-rpp)+(punkt_p[i].s/2),96,koloryp.l_kanal_p)) return 0;}
       }
     linia_kp=TRUE;
     }
    }
   }


/*  linia ulicy, jest tez rysowana jezeli ...opi2=...odl*/
     if (dane_profs.ru==0)   /* brak poprzedniej wartosci */
     {
         dane_profs.ru=dane_prof.ru;
         if (dane_prof.ru>0)
         {
             if (dane_global.odl==0)
             {if(!Lin_special(dane_profs.opi2,tablica_y[14]/*78*/+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.ru-rpp),65,koloryp.l_ulicy)) return 0;}
             else {if(!Lin_special(dane_profs.opi2,tablica_y[14]/*78*/+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.ru-rpp),65,koloryp.l_teren_p)) return 0;}
             /* linia kreskowa */
         }
     }
     else
       {
           if (dane_prof.ru>0)         //teren istniejacy
           {
               if (dane_global.odl==0)   //profil kanalizacyjny
               {if (!Lin_special(dane_profs.opi2,tablica_y[14]/*78*/+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.ru-rpp),64,koloryp.l_ulicy)) return 0;}
               else if (dane_global.odl==1) //profil technologiczny
               {if (!Lin_special(dane_profs.opi2,tablica_y[14]/*78*/+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.ru-rpp),65,koloryp.l_teren_p)) return 0;}
               else //profil rurociagu
               {if (!Lin_special(dane_profs.opi2,tablica_y[14]/*78*/+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.ru-rpp),64,koloryp.l_teren_p)) return 0;}
               /* dla profilu technologicznego linia kreskowa */
           }
       }

/*  linia terenu, jest tez rysowana jezeli ...opi3=...odl*/
     if (dane_profs.rt==0)
      {
          dane_profs.rt=dane_prof.rt;
          if (dane_prof.rt>0)
           {
           if(!Lin_special(dane_profs.opi3,tablica_y[14]/*78*/+(dane_profs.rt-rpp),dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.rt-rpp),65,koloryp.l_teren_i)) return 0;
           /* linia kreskowa */
           }
      }
    else
      {
          if (dane_prof.rt>0)
          {
              if(!Lin_special(dane_profs.opi3,tablica_y[14]/*78*/+(dane_profs.rt-rpp),dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.rt-rpp),64,koloryp.l_teren_i)) return 0;
          }
      }
      

 /* generowanie tabelki opisowej */

  if (FALSE == add_block (B_PLINE, 0))
   {
    return 0;
   }
   
  /*wysokosc tabelki opisowej - na razie zrezygnowano*/

   /*
   if (max_r<dane_profs.spp1)
   {
   max_r=dane_profs.spp1;
   }
   */

//   max_r=dane_profs.spp1;


  if (dane_prof.odl>dane_profs.odl)
  {
      if ((dane_global.odl==0) || (dane_global.odl==2))
      {
          /*     if(!Lin(dane_profs.odl,tablica_y[14]+(max_r-rpp)+70,dane_prof.odl,tablica_y[14]+(max_r-rpp)+70,64,koloryp.os_pomiaru)) return 0; //dolna
               if(!Lin(dane_profs.odl,tablica_y[14]+(max_r-rpp)+80,dane_prof.odl,tablica_y[14]+(max_r-rpp)+80,64,koloryp.os_pomiaru)) return 0; //gorna
               if (dane_profs.odl==(dane_global.rki*100))
                {
                 if(!Lin(dane_profs.odl,tablica_y[14]+(max_r-rpp)+70,dane_profs.odl,tablica_y[14]+(max_r-rpp)+80,64,koloryp.os_pomiaru)) return 0; //z lewej
                }
          */
          local_y_ = get_localy();
          if(!Lin(dane_profs.odl,FormatY-40-local_y_,dane_prof.odl,FormatY-40-local_y_,64,koloryp.os_pomiaru)) return 0; //dolna
          if(!Lin(dane_profs.odl,FormatY-30-local_y_,dane_prof.odl,FormatY-30-local_y_,64,koloryp.os_pomiaru)) return 0; //gorna
          if (dane_profs.odl==(dane_global.rki*100))
          {
              if(!Lin(dane_profs.odl,FormatY-30-local_y_,dane_profs.odl,FormatY-40-local_y_,64,koloryp.os_pomiaru)) return 0; //z lewej
          }
      }
      else
      {
          /*
                if(!Lin(dane_profs.odl,tablica_y[14]+210,dane_prof.odl,tablica_y[14]+210,64,koloryp.os_pomiaru)) return 0; //dolna
                if(!Lin(dane_profs.odl,tablica_y[14]+220,dane_prof.odl,tablica_y[14]+220,64,koloryp.os_pomiaru)) return 0; //gorna
                if (dane_profs.odl==(dane_global.rki*100))
                 {
              if(!Lin(dane_profs.odl,tablica_y[14]+210,dane_profs.odl,tablica_y[14]+220,64,koloryp.os_pomiaru)) return 0; //z lewej
                 }
          */
          local_y_ = get_localy();
          if(!Lin(dane_profs.odl,FormatY-40-local_y_,dane_prof.odl,FormatY-40-local_y_,64,koloryp.os_pomiaru)) return 0; //dolna
          if(!Lin(dane_profs.odl,FormatY-30-local_y_,dane_prof.odl,FormatY-30-local_y_,64,koloryp.os_pomiaru)) return 0; //gorna
          if (dane_profs.odl==(dane_global.rki*100))
          {
              if(!Lin(dane_profs.odl,FormatY-30-local_y_,dane_profs.odl,FormatY-40-local_y_,64,koloryp.os_pomiaru)) return 0; //z lewej
          }
      }
  }

 memmove(&TextG, &textg, sizeof(TEXT));
 /*zapisanie danych do pliku binarnego*/


  /* jezeli wartosci rki, rkp, ru, rt sa wieksze od zera, to odpowiednio: */
  /* w parametrze dane_prof.spi3 nalezy zapamietac wartosc odl. dla rki */
  /* w parametrze dane_prof.opi1 nalezy zapamietac wartosc odl. dla rkp */
  /* w parametrze dane_prof.opi2 nalezy zapamietac wartosc odl. dla ru */
  /* w parametrze dane_prof.opi3 nalezy zapamietac wartosc odl. dla rt */

  if (dane_prof.rki>0) dane_prof0->spi3=dane_prof0_odl_ki;
  else
     {
      dane_prof0->rki=dane_profs1.rki;
      dane_prof0->spi3=dane_profs1.spi3;
     }

  if (dane_prof.rkp>0) dane_prof0->opi1=dane_prof0_odl_kp;
  else
     {
      dane_prof0->rkp=dane_profs1.rkp;
      dane_prof0->opi1=dane_profs1.opi1;
     }

  if (dane_prof.ru>0) dane_prof0->opi2=dane_prof0->odl;
  else
     {
      dane_prof0->ru=dane_profs1.ru;
      dane_prof0->opi2=dane_profs1.opi2;
     }

  if (dane_prof.rt>0) dane_prof0->opi3=dane_prof0->odl;
  else
     {
      dane_prof0->rt=dane_profs1.rt;
      dane_prof0->opi3=dane_profs1.opi3;
     }


//  dane_prof0->spp1=max_r;


  dane_profs1=*dane_prof0;

#ifdef SAVETMP
  /* tutaj najpierw nalezy zachowac zbior poprzedni w zbiorze PROFIL_TEMP_FILE_BAK*/
  /* a nastepnie zapisac nowe dane */
  if (rename(PROFIL_TEMP_FILE,PROFIL_TEMP_FILE_BAK)!=0)
   {
    remove(PROFIL_TEMP_FILE_BAK);
    rename(PROFIL_TEMP_FILE,PROFIL_TEMP_FILE_BAK);
   }

#ifndef LINUX
    i_f_handle=open(PROFIL_TEMP_FILE, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    i_f_handle=open(PROFIL_TEMP_FILE, O_CREAT|O_TRUNC|O_RDWR, 0666);
#endif


  if (i_f_handle == -1)
  {
  ErrList(103);
  return 0;
  }
  

  if (write (i_f_handle, &dane_profs1, sizeof(dane_profs1)) != sizeof(dane_profs1))
  {
  close (i_f_handle) ;
  ErrList(104);
  return 0;
  }
   else
   {
   close (i_f_handle) ;
   }
#endif
    /*zaznaczenie ze nastapil ostatni zapis pomiaru*/
    /* przyjeto ze znacznikiem aplikacji dla profilu jest wartosc 101 */
    l_kr=put_znacznik_aplikacji(101);

  return 1;
}

int qsort_points_by_x(const void *e1, const void *e2)
{
    const point_x_y *p1 = (const point_x_y *)e1;
    const point_x_y *p2 = (const point_x_y *)e2;

    // Compare float x values safely
    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return  1;

    // If x values are identical, break the tie using 'first'
    return p1->first - p2->first;
}

int qsort_by_kat(const void *e1, const void *e2)
{
    const float *p1 = (const float *)e1;
    const float *p2 = (const float *)e2;

    // Compare float p1 and p2 values safely
    if (*p1 < *p2) return -1;
    if (*p1 > *p2) return  1;

    // If p1 and p2 values are identical:
    return 0;
}


BOOL find_in_tab(double x, int nr_tab, int *before_n, int *after_n)
{ int i;
  //poszukiwanie w tabeli
  *before_n=-1;
  *after_n=-1;
  if (tab_n[nr_tab]==0) return TRUE;
  for (i=0; i<tab_n[nr_tab]; i++)
   {
     if (Check_if_Equal(x, tab_r[nr_tab][i].x)==TRUE) return TRUE;
     if (i>0)
      {
       if ((Check_if_LE(x, tab_r[nr_tab][i-1].x)==FALSE) &&
           (Check_if_LE(x, tab_r[nr_tab][i].x)==TRUE))
        {
          *before_n=i-1;
          *after_n=i;
          return FALSE;
        }
      }
   }
  return TRUE;
}

BOOL find_in_tab1(double x, double y, int nr_tab, int first)
//dla kanalu projektowanego
{ int i;
  //poszukiwanie w tabeli
  if (tab_n_plus[nr_tab]==0) return FALSE;
  for (i=0; i<tab_n_plus[nr_tab]; i++)
   {
     if (Check_if_Equal(x, tab_r_plus[nr_tab][i].x)==TRUE)
      {
        if (first ==tab_r_plus[nr_tab][i].first_kp) return TRUE;
      }
   }
  return FALSE;
}

BOOL find_second_point(double x, int nr_tab, int *second_n, int first)
{ int i;
  //poszukiwanie w tabeli
  if (tab_n[nr_tab]==0) return FALSE;
  for (i=0; i<tab_n[nr_tab]; i++)
   {
     if ((Check_if_Equal(x, tab_r[nr_tab][i].x)==TRUE) &&
         (first == tab_r[nr_tab][i].first))
       {
         *second_n=i;
         return TRUE;
       }
   }
  return FALSE;
}


static int sortowanie_odl(void)
{
  //uaktualnienie tablic tab_rki1, tab_rki2, tab_rkp1, tab_rkp2, tab_ru, tab_rt
  //wyszukiwanie punktow  0<p.typ<7
#define max_tab_n 1000
  int i_retval;
  LINIA *L;
  BLOK  *b, *ptrs_blok;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  T_Desc_Ex_Block 	*ptrs_desc_bl1 ;
  BLOK  *ptrs_blok1;
  char st[128];
  char  *adp, *adk, *adp1, *adp2, *adp3;
  T_Point *P;
  TEXT *T;
  int i,j;
  int before_n, after_n;
  double xi, yi, xj, yj;
  double del_x, del_y, del_x1, del_y1;
  int i_tab6, i_tab7, i_tab8, i_tab9;
  int second_n;
  char wyrownywanie;
  int first_next;
  int b_warstwa;
  BOOL point_of_KP_KK;
  double odleglosc_k;
  double skala_profilu;
  double local_x;
  int l_hekto;
  int hektometr_p;
  int odleglosc_k0;
  char *bp;


  skala_profilu=get_skala_profilu_x();
  local_x=get_localx();
  hektometr_p=(int)get_hektometr_p();
//  InfoList(2);
  for (i=0; i<6; i++)
   {
     tab_n[i]=0;
   }
  tab_n_io=0;

  i_retval = 0;
//  *adp_l = dane + dane_size ;
//  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk)
  {
      if (L->obiekt == OdBLOK)
      {
          b=(BLOK *)adp;
          //     if (b->n > 67108864) b->n=6;
          b_warstwa=get_block_layer(b);
          if (b_warstwa==Current_Layer)
          {
              if ((Layers[b_warstwa].edit==1) &&
                  (Layers[b_warstwa].on==1))
              {
                  if (b->kod_obiektu== B_PPOMIAR) first_next=5;
                  else if (b->kod_obiektu== B_KPOMIAR) first_next=2;
                  else
                  {
                      if ((b->kod_obiektu== B_ODWIERT) ||
                          (b->kod_obiektu== B_ODWIERT) ||
                          (b->kod_obiektu== B_INSTALACJE))
                      {
                          if (b->dlugosc_opisu_obiektu>1)
                          {
                              ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
                              tab_n_io++;
                              tab_r_io[tab_n_io-1].x=ptrs_desc_bl->x;
                          }
                      }
                      first_next=0;
                  }
                  adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
              }
              else adp+=sizeof(NAGLOWEK)+b->n;
          }
          else adp+=sizeof(NAGLOWEK)+b->n;
      }
      else
      {
          if ((L->obiekt==Opoint) && (L->atrybut != Abad) && (L->atrybut != Ausuniety)
            && (L->warstwa==Current_Layer)
            && (Layers[L->warstwa].on==TRUE)
            && (Layers[L->warstwa].edit==TRUE))
          {
              P = (T_Point*)adp;
              if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
              else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
              else wyrownywanie=0;
              if ((P->typ > 0) && (P->typ < 7))
                  //TUTAJ NALEZY JESZCZE SPRAWDZIC CZY JEZELI P-typ==1 to PUNKT TEN NALEZY do blokow *KP lub *KK
              {
                  point_of_KP_KK=TRUE;
                  if (P->typ==1) //mozliwa stara wersja punktu
                  {
                      point_of_KP_KK=FALSE;
                      if (P->blok==1)
                      {
                          if (NULL != (ptrs_blok1=FIRSTB(adp)))
                          {
                              if (ptrs_blok1->dlugosc_opisu_obiektu>1)
                              {
                                  ptrs_desc_bl1 = (T_Desc_Ex_Block *)(&ptrs_blok1->opis_obiektu [0]) ;
                                  strcpy (st, &ptrs_desc_bl1->sz_type [0]) ;
                                  if ((strncmp(st,"*KP",2)==0) || (strncmp(st,"*PP",2)==0)) //jest to blok pomiaru
                                  {
                                      point_of_KP_KK=TRUE;
                                  }
                              }
                          }
                      }
                  }

                  if (point_of_KP_KK==TRUE)
                  {
                      if (tab_n[(P->typ)-1]<max_tab_n)
                      {
                          tab_n[(P->typ)-1]++;
                          tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].x=P->x;
                          tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].y=P->y;
                          //tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].first=P->obiektt2;

                          tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].first=first_next;
                          tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].dno_os=P->obiektt3;
                          tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].wyrownywanie=(unsigned)wyrownywanie;
                          /////////////
                          //odszukanie tekstu odleglosci
                          if (NULL != (ptrs_blok=FIRSTB(adp)))
                          {
                              //znaleziono blok
                              adp1=(char *)ptrs_blok;
                              adp2=adp1 + sizeof(NAGLOWEK) + ptrs_blok->n - 1;
                              adp3=find_obj(adp1, adp2, Otekst, n_odleglosc, 0);
                              if (adp3 !=NULL)
                              {
                                  T=(TEXT *)adp3;
                                  tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].odleglosc=strtod(decimal_dot(T->text), &bp);
                                  //teraz trzeba uwzglednic hektometry
                                  //znamy tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].x
                                  //mozemy wyznaczyc:
                                  odleglosc_k=milimetryob(((tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].x)-local_x)*skala_profilu);
                                  //teraz wyznaczamy liczbe hektometrow
                                  l_hekto=(int)(odleglosc_k / 100);
                                  odleglosc_k0=(l_hekto+hektometr_p)*100;
                                  tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].odleglosc+=odleglosc_k0;
                              }
                              else //base of point location
                              {
                                  tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].odleglosc=milimetryob((P->x-local_x)*skala_profilu);
                              }
                          }
                          else
                          {
                              ////tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].odleglosc=-999999;  //nieoznaczone
                              tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].odleglosc=milimetryob((P->x-local_x)*skala_profilu);
                          }

                          /////////////
                          //odszukanie tekstu zaglebienie
                          if (NULL != (ptrs_blok=FIRSTB(adp)))
                          {
                              //znaleziono blok
                              adp1=(char *)ptrs_blok;
                              adp2=adp1 + sizeof(NAGLOWEK) + ptrs_blok->n - 1;
                              adp3=find_obj(adp1, adp2, Otekst, n_zaglebienie_kanalu_p, 0);
                              if (adp3 !=NULL)
                              {
                                  T=(TEXT *)adp3;
                                  tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].teren_p=T->obiektt3;
                              }
                          }
                          else tab_r[(P->typ)-1][tab_n[(P->typ)-1]-1].teren_p=0;
                          //gdy nie ma to od terenu projektowanego
                          i_retval=1;
                      }
                  }
              }
          }
          adp += L->n + sizeof(NAGLOWEK) ;
      }
      L = (LINIA*)adp;
  }

 //sortowanie

 for (i=0; i<6; i++)  //kolejne tablice
 {
   if (tab_n[i]>1) qsort(tab_r[i],tab_n[i],sizeof(point_x_y),qsort_points_by_x);
 }
//  ClearInfo ();

 //tablice sa posortowane
 //tworzone sa tablice tab_r_plus[0] (teren projektowany/ulica) i
 //                    tab_r_plus[1] (teren istniejacy)
 //ktore uzupelniaja tablice tab_r[4] i tab_r[5] w punktach tab_r[0] oraz
 //                                                         tab_r[2]

 tab_n_plus[0]=0;
 tab_n_plus[1]=0;

 tab_n_plus_io[0]=0;
 tab_n_plus_io[1]=0;



 if (tab_n[0]>1)            //kanal istniejacy, teren projektowany
 {
     for (i=0; i<tab_n[0]; i++)
     {
         if (!find_in_tab(tab_r[0][i].x, 4, &before_n, &after_n))
             //before_n i after_n sa numerami pozycji w tablicy tab_r[4]
                 //gdzie  tab_r[4][before_n].x < tab_r[0][i].x
                     //       tab_r[4][after_n].x > tab_r[0][i].x
         {
             //interpolowanie rzednej
             if ((before_n>-1) && (after_n>-1))
             {
                 xi=tab_r[4][before_n].x;  yi=tab_r[4][before_n].y;
                 xj=tab_r[4][after_n].x;   yj=tab_r[4][after_n].y;

                 //del_x1=P->x-xi;
                 //del_y1=(del_x1/del_x) * del_y;
                 //P->y=yi+del_y1;

                 del_y=yj-yi;
                 del_x=xj-xi;

                 del_x1=tab_r[0][i].x-xi;
                 del_y1=(del_x1/del_x) * del_y;

                 if (tab_n_plus[0]<max_tab_n)
                 {
                     tab_n_plus[0]++;

                     memmove(&tab_r_plus[0][tab_n_plus[0]-1],&tab_r_plus0, sizeof(point_plus_x_y));

                     tab_r_plus[0][tab_n_plus[0]-1].x=tab_r[0][i].x;
                     tab_r_plus[0][tab_n_plus[0]-1].y=yi+del_y1;
                     tab_r_plus[0][tab_n_plus[0]-1].yki=tab_r[0][i].y;
                     tab_r_plus[0][tab_n_plus[0]-1].dno_os_ki=tab_r[0][i].dno_os;
                     tab_r_plus[0][tab_n_plus[0]-1].teren_p_ki=tab_r[0][i].teren_p;
                     tab_r_plus[0][tab_n_plus[0]-1].first_ki=tab_r[0][i].first;
                     //jezeli ...dno_os_ki==1 to nalezy odszukac w tablicy tab_r[1]
                     //z warunku tab_r[1].x = tab_r[0].x
                     //jezeli zostanie znaleziony, to ...yki=(tab_r[0].y+tab_r[1].y)/2
                     if (tab_r_plus[0][tab_n_plus[0]-1].dno_os_ki==1)
                     {
                         if (find_second_point(tab_r[0][i].x, 1, &second_n, tab_r[0][i].first)==TRUE)
                         {
                             tab_r_plus[0][tab_n_plus[0]-1].yki=(tab_r[1][second_n].y+tab_r[0][i].y)/2;
                         }
                     }
                 }
             }
         }
     }
 }
  if (tab_n[2]>1)            //kanal projektowany, teren projektowany
  {
      for (i=0; i<tab_n[2]; i++)
      {
          if (!find_in_tab(tab_r[2][i].x, 4, &before_n, &after_n))
          {
              if (!find_in_tab1(tab_r[2][i].x, tab_r[2][i].y , 0, tab_r[2][i].first))
                  //before_n i after_n sa numerami pozycji w tablicy tab_r[4]
                      //gdzie  tab_r[4][before_n].x < tab_r[0][i].x
                          //       tab_r[4][after_n].x > tab_r[0][i].x
              {
                  //interpolowanie rzednej
                  if ((before_n>-1) && (after_n>-1))
                  {
                      xi=tab_r[4][before_n].x;  yi=tab_r[4][before_n].y;
                      xj=tab_r[4][after_n].x;   yj=tab_r[4][after_n].y;

                      //del_x1=P->x-xi;
                      //del_y1=(del_x1/del_x) * del_y;
                      //P->y=yi+del_y1;

                      del_y=yj-yi;
                      del_x=xj-xi;

                      del_x1=tab_r[2][i].x-xi;
                      del_y1=(del_x1/del_x) * del_y;

                      if (tab_n_plus[0]<max_tab_n)
                      {

                          tab_n_plus[0]++;
                          memmove(&tab_r_plus[0][tab_n_plus[0]-1],&tab_r_plus0, sizeof(point_plus_x_y));

                          tab_r_plus[0][tab_n_plus[0]-1].x=tab_r[2][i].x;
                          tab_r_plus[0][tab_n_plus[0]-1].y=yi+del_y1;
                          tab_r_plus[0][tab_n_plus[0]-1].ykp=tab_r[2][i].y;
                          tab_r_plus[0][tab_n_plus[0]-1].dno_os_kp=tab_r[2][i].dno_os;
                          tab_r_plus[0][tab_n_plus[0]-1].teren_p_kp=tab_r[2][i].teren_p;
                          tab_r_plus[0][tab_n_plus[0]-1].first_kp=tab_r[2][i].first;
                          //jezeli ...dno_os_kp==1 to nalezy odszukac w tablicy tab_r[3]
                          //z warunku tab_r[3].x = tab_r[2].x
                          //jezeli zostanie znaleziony, to ...ykp=(tab_r[2].y+tab_r[3].y)/2
                          if (tab_r_plus[0][tab_n_plus[0]-1].dno_os_kp==1)
                          {
                              if (find_second_point(tab_r[2][i].x, 3, &second_n, tab_r[2][i].first)==TRUE)
                              {
                                  tab_r_plus[0][tab_n_plus[0]-1].ykp=(tab_r[3][second_n].y+tab_r[2][i].y)/2;
                              }
                          }
                      }
                  }
              }
              else  //znaleziono punkt w tablicy
              {
                  tab_r_plus[0][tab_n_plus[0]-1].ykp=tab_r[2][i].y;
                  tab_r_plus[0][tab_n_plus[0]-1].dno_os_kp=tab_r[2][i].dno_os;
                  tab_r_plus[0][tab_n_plus[0]-1].teren_p_kp=tab_r[2][i].teren_p;
                  tab_r_plus[0][tab_n_plus[0]-1].first_kp=tab_r[2][i].first;
                  //jezeli ...dno_os_kp==1 to nalezy odszukac w tablicy tab_r[3]
                  //z warunku tab_r[3].x = tab_r[2].x
                  //jezeli zostanie znaleziony, to ...ykp=(tab_r[2].y+tab_r[3].y)/2
                  if (tab_r_plus[0][tab_n_plus[0]-1].dno_os_kp==1)
                  {
                      if (find_second_point(tab_r[2][i].x, 3, &second_n, tab_r[2][i].first)==TRUE)
                      {
                          tab_r_plus[0][tab_n_plus[0]-1].ykp=(tab_r[3][second_n].y+tab_r[2][i].y)/2;
                      }
                  }
              }
          }
      }
  }

  if (tab_n[0]>1)            //kanal istniejacy, teren istniejacy
  {
      for (i=0; i<tab_n[0]; i++)
      {
          if (!find_in_tab(tab_r[0][i].x, 5, &before_n, &after_n))
              //before_n i after_n sa numerami pozycji w tablicy tab_r[4]
                  //gdzie  tab_r[4][before_n].x < tab_r[0][i].x
                      //       tab_r[4][after_n].x > tab_r[0][i].x
          {
              //interpolowanie rzednej
              if ((before_n>-1) && (after_n>-1))
              {
                  xi=tab_r[5][before_n].x;  yi=tab_r[5][before_n].y;
                  xj=tab_r[5][after_n].x;   yj=tab_r[5][after_n].y;

                  //del_x1=P->x-xi;
                  //del_y1=(del_x1/del_x) * del_y;
                  //P->y=yi+del_y1;

                  del_y=yj-yi;
                  del_x=xj-xi;

                  del_x1=tab_r[0][i].x-xi;
                  del_y1=(del_x1/del_x) * del_y;

                  if (tab_n_plus[1]<max_tab_n)
                  {
                      tab_n_plus[1]++;

                      memmove(&tab_r_plus[1][tab_n_plus[1]-1],&tab_r_plus0, sizeof(point_plus_x_y));

                      tab_r_plus[1][tab_n_plus[1]-1].x=tab_r[0][i].x;
                      tab_r_plus[1][tab_n_plus[1]-1].y=yi+del_y1;
                      tab_r_plus[1][tab_n_plus[1]-1].yki=tab_r[0][i].y;
                      tab_r_plus[1][tab_n_plus[1]-1].dno_os_ki=tab_r[0][i].dno_os;
                      tab_r_plus[1][tab_n_plus[1]-1].teren_p_ki=tab_r[0][i].teren_p;
                      tab_r_plus[1][tab_n_plus[1]-1].first_ki=tab_r[0][i].first;
                      //jezeli ...dno_os_ki==1 to nalezy odszukac w tablicy tab_r[1]
                      //z warunku tab_r[1].x = tab_r[0].x
                      //jezeli zostanie znaleziony, to ...yki=(tab_r[0].y+tab_r[1].y)/2
                      if (tab_r_plus[1][tab_n_plus[1]-1].dno_os_ki==1)
                      {
                          if (find_second_point(tab_r[0][i].x, 1, &second_n, tab_r[0][i].first)==TRUE)
                          {
                              tab_r_plus[1][tab_n_plus[1]-1].yki=(tab_r[1][second_n].y+tab_r[0][i].y)/2;
                          }
                      }
                  }
              }
          }
      }
  }
  if (tab_n[2]>1)            //kanal projektowany, teren istniejacy
  {
      for (i=0; i<tab_n[2]; i++)
      {
          if (!find_in_tab(tab_r[2][i].x, 5, &before_n, &after_n))
          {
              if (!find_in_tab1(tab_r[2][i].x, tab_r[2][i].y, 1, tab_r[2][i].first))
                  //before_n i after_n sa numerami pozycji w tablicy tab_r[4]
                      //gdzie  tab_r[4][before_n].x < tab_r[0][i].x
                          //       tab_r[4][after_n].x > tab_r[0][i].x
              {
                  //interpolowanie rzednej
                  if ((before_n>-1) && (after_n>-1))
                  {
                      xi=tab_r[5][before_n].x;  yi=tab_r[5][before_n].y;
                      xj=tab_r[5][after_n].x;   yj=tab_r[5][after_n].y;

                      //del_x1=P->x-xi;
                      //del_y1=(del_x1/del_x) * del_y;
                      //P->y=yi+del_y1;

                      del_y=yj-yi;
                      del_x=xj-xi;

                      del_x1=tab_r[2][i].x-xi;
                      del_y1=(del_x1/del_x) * del_y;

                      if (tab_n_plus[1]<max_tab_n)
                      {
                          tab_n_plus[1]++;
                          memmove(&tab_r_plus[1][tab_n_plus[1]-1],&tab_r_plus0, sizeof(point_plus_x_y));

                          tab_r_plus[1][tab_n_plus[1]-1].x=tab_r[2][i].x;
                          tab_r_plus[1][tab_n_plus[1]-1].y=yi+del_y1;
                          tab_r_plus[1][tab_n_plus[1]-1].ykp=tab_r[2][i].y;
                          tab_r_plus[1][tab_n_plus[1]-1].dno_os_kp=tab_r[2][i].dno_os;
                          tab_r_plus[1][tab_n_plus[1]-1].teren_p_kp=tab_r[2][i].teren_p;
                          tab_r_plus[1][tab_n_plus[1]-1].first_kp=tab_r[2][i].first;
                          //jezeli ...dno_os_kp==1 to nalezy odszukac w tablicy tab_r[3]
                          //z warunku tab_r[3].x = tab_r[2].x
                          //jezeli zostanie znaleziony, to ...ykp=(tab_r[2].y+tab_r[3].y)/2
                          if (tab_r_plus[1][tab_n_plus[1]-1].dno_os_kp==1)
                          {
                              if (find_second_point(tab_r[2][i].x, 3, &second_n, tab_r[2][i].first)==TRUE)
                              {
                                  tab_r_plus[1][tab_n_plus[1]-1].ykp=(tab_r[3][second_n].y+tab_r[2][i].y)/2;
                              }
                          }
                      }
                  }
              }
              else
              {
                  tab_r_plus[1][tab_n_plus[1]-1].ykp=tab_r[2][i].y;
                  tab_r_plus[1][tab_n_plus[1]-1].dno_os_kp=tab_r[2][i].dno_os;
                  tab_r_plus[1][tab_n_plus[1]-1].teren_p_kp=tab_r[2][i].teren_p;
                  tab_r_plus[1][tab_n_plus[1]-1].first_kp=tab_r[2][i].first;
                  //jezeli ...dno_os_kp==1 to nalezy odszukac w tablicy tab_r[3]
                  //z warunku tab_r[3].x = tab_r[2].x
                  //jezeli zostanie znaleziony, to ...ykp=(tab_r[2].y+tab_r[3].y)/2
                  if (tab_r_plus[1][tab_n_plus[1]-1].dno_os_kp==1)
                  {
                      if (find_second_point(tab_r[2][i].x, 3, &second_n, tab_r[2][i].first)==TRUE)
                      {
                          tab_r_plus[1][tab_n_plus[1]-1].ykp=(tab_r[3][second_n].y+tab_r[2][i].y)/2;
                      }
                  }
              }
          }
      }
  }

 /////////////
 if (tab_n_io>1)            //instalacje i odwierty
 {
     for (i=0; i<tab_n_io; i++)
     {
         if (!find_in_tab(tab_r_io[i].x, 4, &before_n, &after_n)) //teren projektowany
             //before_n i after_n sa numerami pozycji w tablicy tab_r[4]
                 //gdzie  tab_r[4][before_n].x < tab_r[0][i].x
                     //       tab_r[4][after_n].x > tab_r[0][i].x
         {
             //interpolowanie rzednej
             if ((before_n>-1) && (after_n>-1))
             {
                 xi=tab_r[4][before_n].x;  yi=tab_r[4][before_n].y;
                 xj=tab_r[4][after_n].x;   yj=tab_r[4][after_n].y;

                 del_y=yj-yi;
                 del_x=xj-xi;

                 del_x1=tab_r_io[i].x-xi;
                 del_y1=(del_x1/del_x) * del_y;

                 if (tab_n_plus_io[0]<max_tab_n)
                 {
                     tab_n_plus_io[0]++;
                     tab_r_plus_io[0][tab_n_plus_io[0]-1].x=tab_r_io[i].x;
                     tab_r_plus_io[0][tab_n_plus_io[0]-1].y=yi+del_y1;
                 }
             }
         }

         if (!find_in_tab(tab_r_io[i].x, 5, &before_n, &after_n)) //teren istniejacy
             //before_n i after_n sa numerami pozycji w tablicy tab_r[4]
                 //gdzie  tab_r[4][before_n].x < tab_r[0][i].x
                     //       tab_r[4][after_n].x > tab_r[0][i].x
         {
             //interpolowanie rzednej
             if ((before_n>-1) && (after_n>-1))
             {
                 xi=tab_r[5][before_n].x;  yi=tab_r[5][before_n].y;
                 xj=tab_r[5][after_n].x;   yj=tab_r[5][after_n].y;

                 del_y=yj-yi;
                 del_x=xj-xi;

                 del_x1=tab_r_io[i].x-xi;
                 del_y1=(del_x1/del_x) * del_y;

                 if (tab_n_plus_io[1]<max_tab_n)
                 {
                     tab_n_plus_io[1]++;
                     tab_r_plus_io[1][tab_n_plus_io[1]-1].x=tab_r_io[i].x;
                     tab_r_plus_io[1][tab_n_plus_io[1]-1].y=yi+del_y1;
                 }
             }
         }
     }
 }
 /////////////

 i_tab6=tab_n_plus[0];
 i_tab7=tab_n_plus[1];
 i_tab8=tab_n_plus_io[0];
 i_tab9=tab_n_plus_io[1];

 return i_retval;
}

static int usuwanie_profilu(void)   //wraz z opisami spadkow i dlugosci
{ BOOL kk;
  int kkk;
  //usuniecie linii L.obiektt2 = O2BlockSpecial;
  InfoList(2);

  zmien_atrybut(dane,dane + dane_size,Ablok,Anormalny);

//  redrow();
  ADP=dane;
  ADK=dane + dane_size;
  usun_blok	(ADP, ADK) ;
  ADP=NULL; ADK=NULL;
  kk=get_special_lines_and_texts (&ADP, &ADK);
//  if ((ADP!=NULL) && (ADK!=NULL))
//   {
  ADP=dane; ADK=dane+dane_size;

     kkk=delete_block();
     InfoList(0);
     // redrow();
     return 1;
//   }
//    else
//     {
//      InfoList(0);
//      return 0;
//     }
}

static int usuwanie_osi_hektometrow(void)   //wraz z opisami spadkow i dlugosci
{ BOOL kk;
  //usuniecie linii L.obiektt2 = O2BlockSpecial;

  InfoList(2);
//  zmien_atrybut(dane,dane + dane_size,Ablok,Ausuniety);
  ADP=dane;
  ADK=dane + dane_size;
  usun_blok	(ADP, ADK) ;

  ADP=NULL; ADK=NULL;

  kk=get_hektometr_blocks0 (&ADP, &ADK);
  if ((ADP!=NULL) && (ADK!=NULL))
   {
     delete_block();
     InfoList(0);
     // redrow();
     return 1;
   }

  InfoList(0);
  return 0;

/*
if (get_hektometr_blocks ()==TRUE)
 {
  ADP=dane;
  ADK=dane+dane_size;
  delete_block();
  InfoList(0);
  return 1;
 }
  else  return 0;
*/
}

void fill_text(double wsp_xt, double del_xx, TEXT *T)
{
    const double ht2=2.5;
    double wsp_xt1;

    if (wsp_xt<1)
    {
        if (wsp_xt<0.5)
        {
            /* if (del_xx>3.0)
              {
               T->width_factor=0.4;
               T->justowanie=j_centralnie;
               T->kat=Pi/2;
              }
               else
                {
                 T->width_factor=0.4;
                 T->ukryty=1;
                }
              */
            wsp_xt1=wsp_xt/0.5;
            if (wsp_xt1<0.5)
            {
                T->width_factor=0.5f;
                T->wysokosc=(float)(ht2*0.5);
                // T->justowanie=j_centralnie;
                // T->kat=Pi/2;
                T->ukryty=1;
            }
            else
            {
                T->width_factor=0.5f;
                T->wysokosc=(float)(ht2*wsp_xt1);
            }
        }
        else
        {
            T->width_factor=(float)wsp_xt;
        }
    }
}
                    //ki ki kp kp tp ti
// const int typ_lin_profil[6]={96,96,96,96,65,64};


static int aktualizacja_profilu(void)
{
  int i,j;
  int kolor_lin[6];
  double dy0, dy1, dy2;
  const float ht1=2.5f; /*2.25*/ /* opisy pionowe */
  const float ht2=2.5f;  /* opisy poziome */
  const float ht3=3.5f;
  const double dt_y=0.8f;
  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int len_t1, len_t2;
  double spadek_k, spadek_kk, dlugosc_k;
  double skala_profilu, typ_profilu, poziom_pp;
  double local_y;
  TEXT T=Tdef;
  double rzedna_terenu, zaglebienie_kp, rzedna_kanalu;
  int l_kr;
  char *adp, *adk, *adp1, *adp2, *adp3, *adp4, *adp5, *adp6;
  BLOK *b;
  LINIA *L, *L1;
  TEXT *T1, *T2;
  double max_x, y_yki_ykp;
  double wsp_xt, del_xx, x_s, y_s;
  unsigned char *zn;
  char wyrownywanie;
  double y11, y22;
  BOOL add_spadek;
  int b_warstwa;
  int rzedna_poz;
  double rzedna_prec;
  char *bp;

  T.warstwa=Current_Layer;
  T.obiektt2 = O2BlockSpecial;
  T.kolor=koloryp.rzedne;

  skala_profilu=get_skala_profilu_x();
  local_y=get_localy();
  typ_profilu=get_typ_profilu();
  poziom_pp = get_poziom_pp();
  tablica_0_1(typ_profilu);

  kolor_lin[0]=koloryp.l_kanal_i;
  kolor_lin[1]=koloryp.l_kanal_i;
  kolor_lin[2]=koloryp.l_kanal_p;
  kolor_lin[3]=koloryp.l_kanal_p;
  kolor_lin[4]=koloryp.l_ulicy;
  kolor_lin[5]=koloryp.l_teren_i;

  //polaczenie posortowanych punktow liniami Lin_special
  //punkty do polaczenia znajduja sie w 6 tablicach [i]
  // tab_r[i]
  for (i=0; i<6; i++)
  {
      if (tab_n[i]>1)
      {
          switch (i)
          {
          case 0: rzedna_prec=rzedna_i_prec;
              rzedna_poz=rzedna_i_poz;
              break;
          case 1: rzedna_prec=rzedna_i_prec;
              rzedna_poz=rzedna_i_poz;
              break;
          case 2: rzedna_prec=rzedna_p_prec;
              rzedna_poz=rzedna_p_poz;
              break;
          case 3: rzedna_prec=rzedna_p_prec;
              rzedna_poz=rzedna_p_poz;
              break;
          case 4: rzedna_prec=rzedna_tp_prec;
              rzedna_poz=rzedna_tp_poz;
              break;
          case 5: rzedna_prec=rzedna_ti_prec;
              rzedna_poz=rzedna_ti_poz;
              break;
          }
          for (j=1; j<tab_n[i]; j++)
          {
              if(TRUE == Check_if_Equal(tab_r[i][j-1].x,tab_r[i][j].x))  //linia pionowa
              {
                  if(FALSE == Check_if_Equal(tab_r[i][j-1].x,tab_r[i][j].x))
                      if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,32,kolor_lin[i])) return 0;
              }
              else  //linie ukosne lub poziome
              {
                  wyrownywanie=tab_r[i][j].wyrownywanie;
                  if (wyrownywanie==wyrownywanie_dolem)
                  {
                      if ((i==1) || (i==3))   //gorne krawedzie
                      {
                          dy0=tab_r[i][j-1].y-tab_r[i-1][j-1].y;
                          dy1=tab_r[i][j].y-tab_r[i-1][j].y;
                          if(!Lin_special(tab_r[i-1][j-1].x,tab_r[i-1][j-1].y+dy1,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                          //  if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                          if (tab_r[i][j].dno_os==O3Os)   //os
                          {
                              if(!Lin_special(tab_r[i-1][j-1].x,tab_r[i-1][j-1].y+(dy1/2),tab_r[i][j].x,tab_r[i][j].y-(dy1/2),34,kolor_lin[i])) return 0;
                              //++++++++
                              //sprawdzenie czy nie nastapila zmiana rzednej
                              //po prawej stronie przekroju
                              if (tab_r[i-1][j-1].first != 5)  //nie jest to pierwszy pomiar
                              {
                                  if (Check_if_Equal(tab_r[i-1][j-1].y+(dy0/2),tab_r[i-1][j-1].y+(dy1/2))==FALSE)
                                  {
                                      //generowanie dodatkowej rzednej
                                      T.kat= (float)(Pi/2.);
                                      T.wysokosc=2.5f;
                                      T.italics=1;
                                      T.width_factor=0.95f;
                                      T.czcionka=zmwym.czcionka; //0;
                                      T.obiektt1 = 0;
                                      T.obiektt2 = O2BlockSpecial;
                                      T.obiektt3 = 0;
                                      T.ukryty = 0;
                                      T.blok=0;
                                      T.typ=n_typ_normal;
                                      T.justowanie=j_do_lewej;
                                      T.kolor=koloryp.rzedne;
                                      T.x=(float)(tab_r[i][j-1].x+3.5);
                                      if (i==0) T.y=(float)(24.+0.8+local_y);
                                      else T.y=(float)(tablica_y[11]+0.8+local_y);
                                      rzedna_kanalu=milimetryob(tab_r[i-1][j-1].y+(dy1/2) - local_y - tablica_y[14]) + poziom_pp;
                                      sprintf_prec(tekst_p,rzedna_prec,rzedna_kanalu);
                                      l_kr=add_000(tekst_p,rzedna_poz);
                                      strcpy(&T.text[0],tekst_p);

                                      decimal(T.text);

                                      LengthT=(int)strlen(T.text);
                                      if (LengthT > 6) T.width_factor=0.8f;
                                      T.dl=LengthT;
                                      T.n=T18+T.dl;
                                      if (dodaj_obiekt(NULL, &T)==NULL) return 0;
                                  }
                              }
                              //++++++++
                          }
                      }
                      else  //dolne krawedzie
                      {
                          if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                      }
                  }
                  else
                      if (wyrownywanie==wyrownywanie_gora)
                      {
                          if ((i==0) || (i==2))   //dolne krawedzie
                          {
                              dy0=tab_r[i+1][j-1].y-tab_r[i][j-1].y;
                              dy1=tab_r[i+1][j].y-tab_r[i][j].y;
                              if(!Lin_special(tab_r[i+1][j-1].x,tab_r[i+1][j-1].y-dy1,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                              //  if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                              if (tab_r[i][j].dno_os==O3Os)  //os
                              {
                                  if(!Lin_special(tab_r[i+1][j-1].x,tab_r[i+1][j-1].y-(dy1/2),tab_r[i][j].x,tab_r[i][j].y+(dy1/2),34,kolor_lin[i])) return 0;
                                  //sprawdzenie czy nie nastapila zmiana rzednej
                                  //po prawej stronie przekroju
                                  if (tab_r[i][j-1].first != 5)  //nie jest to pierwszy pomiar
                                  {
                                      if (Check_if_Equal(tab_r[i+1][j-1].y-(dy0/2),tab_r[i+1][j-1].y-(dy1/2))==FALSE)
                                      {
                                          //generowanie dodatkowej rzednej
                                          T.kat= (float)Pi/2.f;
                                          T.wysokosc=2.5f;
                                          T.italics=1;
                                          T.width_factor=0.95f;
                                          T.czcionka=zmwym.czcionka; //0;
                                          T.obiektt1 = 0;
                                          T.obiektt2 = O2BlockSpecial;
                                          T.obiektt3 = 0;
                                          T.ukryty = 0;
                                          T.blok=0;
                                          T.typ=n_typ_normal;
                                          T.justowanie=j_do_lewej;
                                          T.kolor=koloryp.rzedne;
                                          T.x=(float)(tab_r[i][j-1].x+3.5);
                                          if (i==0) T.y=(float)(24.+0.8+local_y);
                                          else T.y=(float)(tablica_y[11]+0.8+local_y);
                                          rzedna_kanalu=milimetryob(tab_r[i+1][j-1].y-(dy1/2) - local_y - tablica_y[14]) + poziom_pp;
                                          sprintf_prec(tekst_p,rzedna_prec,rzedna_kanalu);
                                          l_kr=add_000(tekst_p,rzedna_poz);
                                          strcpy(&T.text[0],tekst_p);

                                          decimal(T.text);

                                          LengthT=(int)strlen(T.text);
                                          if (LengthT > 6) T.width_factor=0.8f;
                                          T.dl=LengthT;
                                          T.n=T18+T.dl;
                                          if (dodaj_obiekt(NULL, &T)==NULL) return 0;
                                      }
                                  }
                              }
                              if (tab_r[i][j/*-1*/].dno_os==O3Dno) //jezeli w przekroju // poprzednim
                                  //wymiarowane jest dno kanalu
                              {
                                  //sprawdzenie czy nie nastapila zmiana rzednej
                                  //po prawej stronie przekroju
                                  if (tab_r[i][j-1].first != 5)  //nie jest to pierwszy pomiar
                                  {
                                      if (Check_if_Equal(tab_r[i][j-1].y,tab_r[i+1][j-1].y-dy1)==FALSE)
                                      {
                                          //generowanie dodatkowej rzednej
                                          T.kat= (float)Pi/2.f;
                                          T.wysokosc=2.5f;
                                          T.italics=1;
                                          T.width_factor=0.95f;
                                          T.czcionka=zmwym.czcionka; //0;
                                          T.obiektt1 = 0;
                                          T.obiektt2 = O2BlockSpecial;
                                          T.obiektt3 = 0;
                                          T.ukryty = 0;
                                          T.blok=0;
                                          T.typ=n_typ_normal;
                                          T.justowanie=j_do_lewej;
                                          T.kolor=koloryp.rzedne;
                                          T.x=(float)(tab_r[i][j-1].x+3.5);
                                          if (i==0) T.y=(float)(24.+0.8+local_y);
                                          else T.y=(float)(tablica_y[11]+0.8+local_y);
                                          rzedna_kanalu=milimetryob(tab_r[i+1][j-1].y-dy1 - local_y - tablica_y[14]) + poziom_pp;
                                          sprintf_prec(tekst_p,rzedna_prec,rzedna_kanalu);
                                          l_kr=add_000(tekst_p,rzedna_poz);
                                          strcpy(&T.text[0],tekst_p);

                                          decimal(T.text);

                                          LengthT=(int)strlen(T.text);
                                          if (LengthT > 6) T.width_factor=0.8f;
                                          T.dl=LengthT;
                                          T.n=T18+T.dl;
                                          if (dodaj_obiekt(NULL, &T)==NULL) return 0;
                                      }
                                  }
                              }
                          }
                          else  //gorne krawedzie
                          {
                              if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                          }
                      }
                      else
                          if (wyrownywanie==wyrownywanie_osiowe)
                          {
                              if ((i==0) || (i==2))   //dolne krawedzie
                              {
                                  dy1=(tab_r[i+1][j].y-tab_r[i][j].y)/2;
                                  y11=(tab_r[i][j-1].y+tab_r[i+1][j-1].y)/2;
                                  y22=(tab_r[i][j].y+tab_r[i+1][j].y)/2;
                                  if(!Lin_special(tab_r[i][j-1].x,y11-dy1,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                                  //  if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                                  //++++++++++++
                                  if (tab_r[i][j/*-1*/].dno_os==O3Dno) //jezeli w przekroju //poprzednim
                                      //wymiarowane jest dno kanalu
                                  {
                                      //sprawdzenie czy nie nastapila zmiana rzednej
                                      //po prawej stronie przekroju
                                      if (tab_r[i][j-1].first != 5)  //nie jest to pierwszy pomiar
                                      {
                                          if (Check_if_Equal(tab_r[i][j-1].y,y11-dy1)==FALSE)
                                          {
                                              //generowanie dodatkowej rzednej
                                              T.kat= (float)(Pi/2.);
                                              T.wysokosc=2.5f;
                                              T.italics=1;
                                              T.width_factor=0.95f;
                                              T.czcionka=zmwym.czcionka; //0;
                                              T.obiektt1 = 0;
                                              T.obiektt2 = O2BlockSpecial;
                                              T.obiektt3 = 0;
                                              T.ukryty = 0;
                                              T.blok=0;
                                              T.typ=n_typ_normal;
                                              T.justowanie=j_do_lewej;
                                              T.kolor=koloryp.rzedne;
                                              T.x=(float)(tab_r[i][j-1].x+3.5);
                                              if (i==0) T.y=(float)(24.+0.8+local_y);
                                              else T.y=(float)(tablica_y[11]+0.8+local_y);
                                              rzedna_kanalu=milimetryob(y11-dy1 - local_y - tablica_y[14]) + poziom_pp;
                                              sprintf_prec(tekst_p,rzedna_prec,rzedna_kanalu);
                                              l_kr=add_000(tekst_p,rzedna_poz);
                                              strcpy(&T.text[0],tekst_p);

                                              decimal(T.text);

                                              LengthT=(int)strlen(T.text);
                                              if (LengthT > 6) T.width_factor=0.8f;
                                              T.dl=LengthT;
                                              T.n=T18+T.dl;
                                              if (dodaj_obiekt(NULL, &T)==NULL) return 0;
                                          }
                                      }
                                  }
                                  //++++++++++++
                              }
                              else        //gorne krawedzie
                              {
                                  dy1=(tab_r[i][j].y-tab_r[i-1][j].y)/2;
                                  y11=(tab_r[i][j-1].y+tab_r[i-1][j-1].y)/2;
                                  y22=(tab_r[i][j].y+tab_r[i-1][j].y)/2;
                                  if(!Lin_special(tab_r[i-1][j-1].x,y11+dy1,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                                  //  if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                                  if (tab_r[i][j].dno_os==O3Os)
                                  {
                                      if(!Lin_special(tab_r[i-1][j-1].x,y11,tab_r[i][j].x,y22,34,kolor_lin[i])) return 0;
                                  }
                              }
                          }
              }
              //+++++++++++++++++
              //DOPISANIE SPADKOW
              // dopisanie spadku i dlugosci kanalu istniejacego
              //       parametry_ogolne_tekstu(0, ht2, koloryp.spadek_l_fi_mat);
              T.wysokosc=ht2;
              T.kolor=koloryp.spadek_l_fi_mat;
              T.kat=0;
              T.italics=0;
              T.width_factor=1;
              T.czcionka=zmwym.czcionka; //0;

              if (i<4)
              {
                  add_spadek=FALSE;

                  if (TRUE == Check_if_Equal (tab_r[i][j-1].x,tab_r[i][j].x)) spadek_k=10000; /*wartosc umowna*/
                  else
                  {
                      if ((wyrownywanie==wyrownywanie_dolem) && ((i==0) || (i==2)))
                      {
                          //if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;

                          if ((tab_r[i][j].odleglosc>-999998) && (tab_r[i][j-1].odleglosc>-999998))  //podane wartosci odleglosci
                          {
                              dlugosc_k=(tab_r[i][j].odleglosc-tab_r[i][j-1].odleglosc);
                              spadek_k=(milimetryob(tab_r[i][j-1].y-tab_r[i][j].y)*1000)/dlugosc_k;
                          }
                          else
                          {
                              dlugosc_k=milimetryob((tab_r[i][j].x-tab_r[i][j-1].x)*skala_profilu);
                              spadek_k=((tab_r[i][j-1].y-tab_r[i][j].y)*1000)/((tab_r[i][j].x-tab_r[i][j-1].x)*skala_profilu);

                          }
                          add_spadek=TRUE;
                      }
                      else if ((wyrownywanie==wyrownywanie_gora) && ((i==1) || (i==3)))
                      {
                          //if(!Lin_special(tab_r[i][j-1].x,tab_r[i][j-1].y,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                          if ((tab_r[i][j].odleglosc>-999998) && (tab_r[i][j-1].odleglosc>-999998))  //podane wartosci odleglosci
                          {
                              dlugosc_k=(tab_r[i][j].odleglosc-tab_r[i][j-1].odleglosc);
                              spadek_k=(milimetryob(tab_r[i][j-1].y-tab_r[i][j].y)*1000)/dlugosc_k;
                          }
                          else
                          {
                              dlugosc_k=milimetryob((tab_r[i][j].x-tab_r[i][j-1].x)*skala_profilu);
                              spadek_k=((tab_r[i][j-1].y-tab_r[i][j].y)*1000)/((tab_r[i][j].x-tab_r[i][j-1].x)*skala_profilu);

                          }
                          add_spadek=TRUE;
                      }
                      else if ((wyrownywanie==wyrownywanie_osiowe) && ((i==0) || (i==2)))
                      {
                          dy1=(tab_r[i+1][j].y-tab_r[i][j].y)/2;
                          y11=(tab_r[i][j-1].y+tab_r[i+1][j-1].y)/2;
                          y22=(tab_r[i][j].y+tab_r[i+1][j].y)/2;
                          //    if(!Lin_special(tab_r[i][j-1].x,y11-dy1,tab_r[i][j].x,tab_r[i][j].y,typ_lin_profil[i],kolor_lin[i])) return 0;
                          if ((tab_r[i][j].odleglosc>-999998) && (tab_r[i][j-1].odleglosc>-999998))  //podane wartosci odleglosci
                          {
                              dlugosc_k=(tab_r[i][j].odleglosc-tab_r[i][j-1].odleglosc);
                              spadek_k=(milimetryob((y11-dy1)-tab_r[i][j].y)*1000)/dlugosc_k;
                          }
                          else
                          {
                              dlugosc_k=milimetryob((tab_r[i][j].x-tab_r[i][j-1].x)*skala_profilu);
                              spadek_k=(((y11-dy1)-tab_r[i][j].y)*1000)/((tab_r[i][j].x-tab_r[i][j-1].x)*skala_profilu);
                          }
                          add_spadek=TRUE;
                      }

                      //   if (i==0) T.y=18.5+dt_y+local_y;
                      //     else T.y=tablica_y[10]+dt_y+local_y;
                      if (add_spadek==TRUE)
                      {
                          if ((i==0) || (i==1)) y_s=20.5+local_y;
                          else y_s=(tablica_y[9]+tablica_y[11])/2+local_y;

                          add_block(B_PLINE, O2BlockSpecial);

                          strcpy(tekst_p,"");
                          sprintf_prec(tekst_p1,spadek_prec,fabs(spadek_k));
                          l_kr=add_000(tekst_p1,spadek_poz);
                          strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
                          spadek_kk=strtod(tekst_p1, &bp);
                          strcpy(&T.text[0],tekst_p);

                          decimal(T.text);

                          LengthT=(int)strlen(T.text);
                          len_t1=LengthT;
                          len_t2=0;
                          strcpy(tekst_p,"l=");
                          sprintf_prec(tekst_p1,odleglosc_prec,dlugosc_k);
                          l_kr=add_000(tekst_p1,odleglosc_poz);
                          strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
                          len_t2=(int)strlen(tekst_p);
                          T.dl=LengthT;
                          T.n=T18+T.dl;
                          //polozenie tekstu w polowie pomiedzy pomiarami
                          //T.x=dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5)-5;
                          x_s=(tab_r[i][j].x+tab_r[i][j-1].x)/2;

                          if (Check_if_Equal(spadek_kk,0.0)==TRUE)
                          {
                              T.x=(float)x_s;
                              T.justowanie=j_srodkowo;
                              T.y=(float)(y_s+0.5);
                              del_xx=fabs(tab_r[i][j].x-tab_r[i][j-1].x);
                          }
                          else
                          {
                              T.x=(float)(x_s - 1.);
                              T.justowanie=j_do_prawej;
                              if (spadek_k>0) T.y=(float)((y_s- 3.0));
                              else T.y=(float)((y_s + 0.5));
                              del_xx=fabs(tab_r[i][j].x-tab_r[i][j-1].x)/2 - 1;
                          }
                          T.czcionka=zmwym.czcionka; //0;
                          T.kat= 0;
                          T.wysokosc=ht2;
                          T.width_factor=1.f;

                          if (i==0) T.typ=n_spadek_kanalu_i;
                          else T.typ=n_spadek_kanalu_p;
                          T.obiektt2=O2BlockSpecial;
                          T.blok=1;
                          T.ukryty=0;
                          //wyznaczenie dlugosci

                          GetTextLen (&T, &tdl) ;
                          if (Check_if_Equal(tdl,0.0)==TRUE)
                          {
                              wsp_xt=0;
                          }
                          else
                          {
                              wsp_xt=del_xx/(tdl*1.2);
                          }
                          fill_text(wsp_xt, del_xx, &T);
                          if (dodaj_obiekt((BLOK*)dane, &T)==NULL) return 0;
                          //          T.blok=0;
                          //          if (dodaj_obiekt(NULL, &T)==NULL) return 0;

                          strcpy(tekst_p,"l=");
                          sprintf_prec(tekst_p1,odleglosc_prec,dlugosc_k);
                          l_kr=add_000(tekst_p1,odleglosc_poz);
                          strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
                          strcpy(&T.text[0],tekst_p);

                          decimal(T.text);

                          LengthT=(int)strlen(T.text);
                          T.dl=LengthT;
                          T.n=T18+T.dl;
                          //polozenie tekstu w polowie pomiedzy pomiarami
                          //  T.x=dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5)+5;
                          //     T.x=dane_prof.odl + 5;
                          if (Check_if_Equal(spadek_kk,0.0)==TRUE)
                          {
                              T.x=(float)x_s;
                              T.justowanie=j_srodkowo;
                              T.y=(float)(y_s - 3.);
                          }
                          else
                          {
                              T.x=(float)(x_s + 1.);
                              T.justowanie=j_do_lewej;
                              if (spadek_k>0) T.y=(float)(y_s + 0.5); else T.y=(float)(y_s - 3.0);
                          }
                          T.kat= 0;
                          T.wysokosc=ht2;
                          T.width_factor=1.f;
                          T.czcionka=zmwym.czcionka; //0;
                          if (i==0) T.typ=n_dlugosc_kanalu_i;
                          else T.typ=n_dlugosc_kanalu_p;
                          T.obiektt2=O2BlockSpecial;
                          T.blok=1;
                          T.ukryty=0;

                          //wyznaczenie dlugosci
                          //del_xx=fabs(tab_r[i][j].x-tab_r[i][j-1].x)/2;
                          GetTextLen (&T, &tdl) ;
                          if (Check_if_Equal(tdl,0.0)==TRUE)
                          {
                              wsp_xt=0;
                          }
                          else
                          {
                              wsp_xt=del_xx/(tdl*1.2);
                          }
                          fill_text(wsp_xt, del_xx, &T);
                          if (dodaj_obiekt((BLOK*)dane, &T)==NULL) return 0;
                          //          T.blok=0;
                          //            if (dodaj_obiekt(NULL, &T)==NULL) return 0;

                          // dopisanie linii ukosnej
                          if (fabs(tab_r[i][j].x-tab_r[i][j-1].x)>7)
                          {
                              if (Check_if_Equal(spadek_kk,0.0)==TRUE)
                              {
                                  if(!Lin_special_b(x_s-3.5,y_s,x_s+3.5,y_s,64,koloryp.spadek_l_fi_mat)) return 0;
                              }
                              else
                              {
                                  if (spadek_k>0)
                                  {
                                      if(!Lin_special_b(x_s-3.5,y_s+2.5,x_s+3.5,y_s-2.5,64,koloryp.spadek_l_fi_mat)) return 0;
                                  }
                                  else
                                      if (spadek_k<0)  //
                                      {
                                          if(!Lin_special_b(x_s-3.5,y_s-2.5,x_s+3.5,y_s+2.5,64,koloryp.spadek_l_fi_mat)) return 0;
                                      }
                                  //    if(!Lin(dane_prof_odl_ki,17,dane_prof_odl_ki,24,64,koloryp.os_pomiaru)) return 0;
                              }
                          }
                      }
                  }
              }
              //+++++++++++++++++
          }
      }
  }

  //dodatkowe punkty
  T.kat= (float)(Pi/2.);
  T.wysokosc=2.0f;
  T.italics=1;
  T.width_factor=1;
  T.czcionka=zmwym.czcionka; //0/*1*/;
  T.obiektt1 = 0;
  T.obiektt2 = O2BlockSpecial;
  T.ukryty = ukrywanie_rtin;
  T.blok=0;
  T.typ=n_typ_normal;
  T.justowanie=j_do_lewej;
  T.kolor=koloryp.rzedne;

  if (tab_n_plus[0]>0)        //teren projektowany / ulica
   {
    for (i=0; i<tab_n_plus[0]; i++)
     {
       y_yki_ykp=tab_r_plus[0][i].yki;
       if (tab_r_plus[0][i].ykp>y_yki_ykp) y_yki_ykp=tab_r_plus[0][i].ykp;

       if(!Lin_special(tab_r_plus[0][i].x,tab_r_plus[0][i].y,tab_r_plus[0][i].x,y_yki_ykp/*tablica_y[14]+local_y*/,33,koloryp.os_pomiaru)) return 0;
       if (typ_profilu==0)
        {
         if(!Lin_special(tab_r_plus[0][i].x,tablica_y[12]+local_y,tab_r_plus[0][i].x,tablica_y[13]+local_y,33,koloryp.os_pomiaru)) return 0;
        }
       //dopisanie rzednej
       //+++++++++++++++++
       T.x=(float)(tab_r_plus[0][i].x-1.);
       T.y=(float)(tablica_y[12]+1.5+local_y);
       rzedna_terenu=milimetryob(tab_r_plus[0][i].y - local_y - tablica_y[14]) + poziom_pp;
       sprintf_prec(tekst_p,rzedna_tp_prec,rzedna_terenu);
       l_kr=add_000(tekst_p,rzedna_tp_poz);
       strcpy(&T.text[0],tekst_p);

       decimal(T.text);

       LengthT=(int)strlen(T.text);
       if (LengthT > 6) T.width_factor=0.85f;
       T.dl=LengthT;
       T.n=T18+T.dl;

       if (dodaj_obiekt(NULL, &T)==NULL) return 0;

       //zaglebienie - tylko dla kanalu projektowanego
       if (typ_profilu==2)
        {
         if (tab_r_plus[0][i].teren_p_kp==0)
          {
            T.x=(float)(tab_r_plus[0][i].x-1.);
            T.y=(float)(24.+1.5+local_y);
            zaglebienie_kp=milimetryob(tab_r_plus[0][i].y - tab_r_plus[0][i].ykp);
            sprintf_prec(tekst_p,zaglebienie_prec,zaglebienie_kp);
            l_kr=add_000(tekst_p,zaglebienie_poz);
            strcpy(&T.text[0],tekst_p);

            decimal(T.text);

            LengthT=(int)strlen(T.text);
            if (LengthT > 6) T.width_factor=0.85f;
            T.dl=LengthT;
            T.n=T18+T.dl;

            if (dodaj_obiekt(NULL, &T)==NULL) return 0;
          }
        }
       //+++++++++++++++++
     }
   }
  if (tab_n_plus[1]>0)       //teren istniejacy
   {
    for (i=0; i<tab_n_plus[1]; i++)
     {
       y_yki_ykp=tab_r_plus[1][i].yki;
       if (tab_r_plus[1][i].ykp>y_yki_ykp) y_yki_ykp=tab_r_plus[1][i].ykp;
       if(!Lin_special(tab_r_plus[1][i].x,tab_r_plus[1][i].y,tab_r_plus[1][i].x,y_yki_ykp/*tablica_y[14]+local_y*/,33,koloryp.os_pomiaru)) return 0;
       if (typ_profilu==0)
        {
         if(!Lin_special(tab_r_plus[1][i].x,tablica_y[13]+local_y,tab_r_plus[1][i].x,tablica_y[14]+local_y,33,koloryp.os_pomiaru)) return 0;
        }
       //dopisanie rzednej
       //+++++++++++++++++
       T.x=(float)(tab_r_plus[1][i].x-1.);
       T.y=(float)(tablica_y[13]+1.5+local_y);
       rzedna_terenu=milimetryob(tab_r_plus[1][i].y - local_y - tablica_y[14]) + poziom_pp ;
       sprintf_prec(tekst_p,rzedna_ti_prec,rzedna_terenu);
       l_kr=add_000(tekst_p,rzedna_ti_poz);
       strcpy(&T.text[0],tekst_p);

       decimal(T.text);

       LengthT=(int)strlen(T.text);
       if (LengthT > 6) T.width_factor=0.85f;
       T.dl=LengthT;
       T.n=T18+T.dl;

       if (dodaj_obiekt(NULL, &T)==NULL) return 0;

       //zaglebienie - tylko dla kanalu projektowanego
       if (typ_profilu==2)
        {
         if (tab_r_plus[1][i].teren_p_kp==1)
          {
            T.x=(float)(tab_r_plus[1][i].x-1.);
            T.y=(float)(24.+1.5+local_y);
            zaglebienie_kp=milimetryob(tab_r_plus[1][i].y - tab_r_plus[1][i].ykp);
            sprintf_prec(tekst_p,zaglebienie_prec,zaglebienie_kp);
            l_kr=add_000(tekst_p,zaglebienie_poz);
            strcpy(&T.text[0],tekst_p);

            decimal(T.text);

            LengthT=(int)strlen(T.text);
            if (LengthT > 6) T.width_factor=0.85f;
            T.dl=LengthT;
            T.n=T18+T.dl;

            if (dodaj_obiekt(NULL, &T)==NULL) return 0;
          }
        }
       //+++++++++++++++++
     }
   }
  T.ukryty = 0;
   //dopasowanie opisu srednicy i materialu
   //++++++++++++++++


   adp = dane ;
   adk = dane + dane_size ;
   L = (LINIA*)adp;
   while (L->obiekt != Okoniec && adp < adk)
   {
    if (L->obiekt == OdBLOK)
    {
  	  b=(BLOK *)adp;
     b_warstwa=get_block_layer(b);
     if (b_warstwa==Current_Layer)
     {
      if ((Layers[b_warstwa].edit==1) &&
          (Layers[b_warstwa].on==1))
      {
     if (((b->kod_obiektu==B_PPOMIAR) ||
         (b->kod_obiektu==B_KPOMIAR))
         && ((b->atrybut != Abad) &&
             (b->atrybut != Ausuniety)))
       {
         adp1=adp + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
         T1=(TEXT *) adp1;
         adp1 += sizeof(NAGLOWEK) + T1->n;
         L1=(LINIA *) adp1;
         if (L1->obiekt==Olinia)
         {
          adp2=adp1 + sizeof(NAGLOWEK) + b->n - 1;
          adp3=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_i, 0);
          adp4=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_p, 0);
          adp5=find_obj(adp1, adp2, Otekst, n_typ_normal, 0);
          if (adp5 != NULL) //szukanie opisu nawierzchni
           {
             //ewentualny druki tekst normalny
             T2=(TEXT *) adp5;
             adp6=find_obj(adp5 + sizeof(NAGLOWEK) + T2->n, adp2, Otekst, n_typ_normal, 0);
             if (adp6 != NULL) adp5=adp6;
             //rozbior lancucha na srednice, jednostki, material
             T2=(TEXT *)adp5;
             if ((T2->y > (10+local_y)) && (T2->y < (15+local_y)))
              {
               //to na pewno jest opis nawierzchni
               //odszukanie najdalszego punktu tab_r[4][i].x < L1->x
               //jezeli zostanie znaleziony, punkt srodkowy stanowi
               //srodek tekstu
               T2->x = L1->x1 - 5;
               T2->justowanie=j_do_prawej;
               T2->wysokosc=ht2;
               T2->width_factor=1;
               T2->ukryty=0;
               T2->kat=0;

               if ((tab_n[4]>0) || (tab_n[5]>0))   //teren projektowany lub istniejacy
                {

                 max_x=0;

                 if (tab_n[4]>0) //przekroje terenu projektwanego
                  {
                   i=0;
                   while ((tab_r[4][i].x<L1->x1) && (i<=tab_n[4]))
                     {
                       max_x=tab_r[4][i].x;
                       i++;
                     }
                  }
                 if (tab_n[5]>0) //przekroje terenu istniejacego
                  {
                   i=0;
                   while ((tab_r[5][i].x<L1->x1) && (i<=tab_n[5]))
                     {
                       max_x=tab_r[5][i].x;
                       i++;
                     }
                  }
                 if (max_x>0)
                   {
                     T2->x=(float)((max_x+L1->x1)/2.);
                     T2->justowanie=j_srodkowo;
                     //wyznaczenie wspolczynnika szerokosci
                     zn = (unsigned char *)T2->text ;
                   //  tdl = Get_Text_Len (T2, (char*)zn) ;
                     GetTextLen (T2, &tdl) ;
                     del_xx=fabs(max_x-L1->x1);
                     if (Check_if_Equal(tdl,0.0)==TRUE)
                      {
                        wsp_xt=0;
                      }
                       else
                        {
                          wsp_xt=del_xx/(tdl*1.1);
                        }
                     fill_text(wsp_xt, del_xx, T2);
                   }
                }
              }
           }
          if (adp3 != NULL)   //kanal istniejacy
           {
             //odszukanie najdalszego punktu tab_r[0][i].x < L1->x
             //jezeli zostanie znaleziony, punkt srodkowy stanowi
             //srodek tekstu
             T1=(TEXT *) adp3;
			 T1->wysokosc=ht2;
			 T1->width_factor=1;
			 T1->kat=0;

			 if (b->kod_obiektu==B_PPOMIAR)  //pierwszy pomiar
			 {
                T1->x = L1->x1 +1 ;
				T1->justowanie=j_do_lewej;
				T1->ukryty=1;
			 }
			 else
			 {
				T1->x = L1->x1 - 5;
				T1->justowanie=j_do_prawej;
				T1->ukryty=0;
			 
              if (tab_n[0]>0)
              {
                i=0;
                max_x=0;
                while ((tab_r[0][i].x<L1->x1) && (i<=tab_n[0]))
                 {
                   max_x=tab_r[0][i].x;
                   i++;
                 }
                if (max_x>0)
                 {
                   T1->x=(float)((max_x+L1->x1)/2.);
                   T1->justowanie=j_srodkowo;
                   //wyznaczenie wspolczynnika szerokosci
                     zn = (unsigned char *)T1->text ;
                   //  tdl = Get_Text_Len (T1, (char*)zn) ;
                     GetTextLen (T1, &tdl) ;
                     del_xx=fabs(max_x-L1->x1);
                     if (Check_if_Equal(tdl,0.0)==TRUE)
                      {
                        wsp_xt=0;
                      }
                     else
                        {
                          wsp_xt=del_xx/(tdl*1.1);
                        }
                     fill_text(wsp_xt, del_xx, T1);
                 }
              }
			 }
           }
          if (adp4 != NULL)   //kanal projektowany
           {
             //odszukanie najdalszego punktu tab_r[1][i].x < L1->x
             //jezeli zostanie znaleziony, punkt srodkowy stanowi
             //srodek tekstu
             T1=(TEXT *) adp4;
             T1->wysokosc=ht2;
			 T1->width_factor=1;
			 T1->kat=0;           

			 if (b->kod_obiektu==B_PPOMIAR)  //pierwszy pomiar
			 {
                T1->x = L1->x1 +1 ;
				T1->justowanie=j_do_lewej;
				T1->ukryty=1;
			 }
			 else
			 {
				T1->x = L1->x1 - 5;
				T1->justowanie=j_do_prawej;
				T1->ukryty=0;
			 
              if (tab_n[2]>0)
              {
                i=0;
                max_x=0;
                while ((tab_r[2][i].x<L1->x1) && (i<=tab_n[2]))
                 {
                   max_x=tab_r[2][i].x;
                   i++;
                 }
                if (max_x>0)
                 {
                   T1->x=(float)((max_x+L1->x1)/2.);
                   T1->justowanie=j_srodkowo;
                   //wyznaczenie wspolczynnika szerokosci
                     zn = (unsigned char *)T1->text ;
                   //  tdl = Get_Text_Len (T1, (char*)zn) ;
                     GetTextLen (T1, &tdl) ;
                     del_xx=fabs(max_x-L1->x1);
                     if (Check_if_Equal(tdl,0.0)==TRUE)
                      {
                        wsp_xt=0;
                      }
                     else
                      {
                        wsp_xt=del_xx/(tdl*1.1);
                      }
                     fill_text(wsp_xt, del_xx, T1);
                 }
              }
			 }
           }
         }
       }
	   adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
      }
       else adp+=sizeof(NAGLOWEK)+b->n;
     }
      else adp+=sizeof(NAGLOWEK)+b->n;
    }
    else
    {
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
   }
   //++++++++++++++++

  zmien_atrybut(dane,dane+dane_size,Ablok,Anormalny);
  return 1;
}
/*
extern "C" { extern void *find_obj(char *adrp, char *adrk, int obiekt, char typ_obiektu, int info_p); }
void *find_obj(char *adrp, char *adrk, int obiekt, char typ_obiektu, int info_p)
{ BLOK *b;
  LINIA *L;
  TEXT *T;
  T_Point *P;
  char *adpp;

  adpp=adrp;

  while (adpp<adrk)
   {
    L=(LINIA *)adpp;
    if (L->obiekt==OdBLOK)
     {
      if (L->obiekt==obiekt)
       {
        if ((Layers[L->warstwa].on==TRUE) &&
            ((Layers[L->warstwa].edit==TRUE) || (info_p==1)))
        {
         if (typ_obiektu==ONieOkreslony)
          {
            return adpp;
            break;
          }
           else
            {
             b=(BLOK *) adpp;
             if (b->kod_obiektu == typ_obiektu)
              {
               return adpp;
               break;
              }
            }
        }
       }
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
       if (L->obiekt==obiekt)
        {
         if ((Layers[L->warstwa].on==TRUE) &&
            ((Layers[L->warstwa].edit==TRUE) || (info_p==1)))
         {
          if (typ_obiektu==ONieOkreslony)
           {
            return adpp;
            break;
           }
           else
            {
             if (L->obiekt==Otekst)
              {
                T=(TEXT *) adpp;
                if (T->typ==typ_obiektu)
                 {
                  return adpp;
                  break;
                 }
              }
               else if (L->obiekt==Opoint)
                {
                 P=(T_Point *) adpp;
                 if (P->typ==typ_obiektu)
                  {
                   return adpp;
                   break;
                  }
                }
               else if (L->obiekt==Olinia)
                {
                 L=(LINIA *) adpp;
                 if (L->typ==typ_obiektu)
                  {
                   return adpp;
                   break;
                  }
                }
            }
         }
        }
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}

*/


static int przesuniecie_profilu(double p_odl, double d_odl)
{
  int i_retval;
  LINIA *L, *L1;
  BLOK  *b, *b1;
  BLOK *ptrs_blok;
  LUK *l;
  OKRAG *O;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  TEXT *T, *T1;
  TEXT Tp, *t;
  char  *adp, *adp1, *adp2, *adp3, *adp4, *adk, *adk1;
  T_Point *P, *P1;
  int i,j;
  int typ_bloku;
  char odleglosc[60];
  char tekst_p[60];
  int l_kr, blok_n;
  int b_warstwa;
  //przesuniecie wszystkich blokow od p_odl o d_odl, d_odl moze byc < 0

  double del_wspx, a_odl, aa_odl, new_odl, local_x, local_y;
  double first_hektometr, last_hektometr, skala_profilu, poziom_pp, typ_profilu;
  long del_adp;
  int odl_i;
  double odl_1, odl_r;
  double a_rz, aa_rz, new_rz;
  int nr_tekstu;
  char *bp1;
  double srednica;
  char *srednica_t;
  char srednica_tt[60];
  double max_odl;
  double max_hektometr;
  BOOL equal_ground;
  double a_odl_1;
  double aaa_rz;
  double del_rz;

  max_odl=0;

  adp = dane ;
  adk = dane + dane_size ;

  first_hektometr=get_hektometr_p();
  last_hektometr=get_hektometr_k();
  skala_profilu=get_skala_profilu_x();

  typ_profilu=get_typ_profilu();
  tablica_0_1(typ_profilu);

  poziom_pp=get_poziom_pp();
  local_x=get_localx();
  local_y=get_localy();


    if (get_skala_profilu_x()>0)
   {
    del_wspx=jednostkiOb(d_odl)/(get_skala_profilu_x());
   }
    else del_wspx=0;

  L = (LINIA*)adp;
  typ_bloku=0;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK) typ_bloku=0;
    else if (L->blok==NoElemBlok) typ_bloku=0;
    if (L->obiekt == OdBLOK)
    {
  	  b=(BLOK *)adp;
     b_warstwa=get_block_layer(b);
     if (b_warstwa==Current_Layer)
     {
      if ((Layers[b_warstwa].edit==1) &&
          (Layers[b_warstwa].on==1))
      {
     //sprawdzenie i korekta bloku B_ODWIERT_OLD
     /*+++++++++++++++*/
     if (b->kod_obiektu==(B_ODWIERT_OLD))  
      {
		//poszukiwanie do drugiego tekstu
        adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
        adp2=adp+sizeof(NAGLOWEK)+b->n-1;
        nr_tekstu=0;
        while ((nr_tekstu<2) && (adp1<adp2))
          {
            L1=(LINIA *)adp1;
            if (L1->obiekt==Otekst)
             {
               nr_tekstu++;
               if (nr_tekstu==2)
                {
                  T=(TEXT *)adp1;
                  T->typ=n_rzedna_terenu;
                  T->obiektt3=O3SpecialText;
                  b->kod_obiektu=B_ODWIERT;
                }
             }
            adp1+=(sizeof(NAGLOWEK)+L1->n);
          }
      }
     /*+++++++++++++++*/
     if (((b->kod_obiektu==B_PPOMIAR) ||
         (b->kod_obiektu==B_KPOMIAR) ||
         (b->kod_obiektu==B_INSTALACJE) ||
         (b->kod_obiektu==B_ODWIERT_OLD) ||
         (b->kod_obiektu==B_ODWIERT))
         && ((b->atrybut != Abad) &&
             (b->atrybut != Ausuniety)))
           {
             //najpierw nalezy sprawdzic, czy w bloku: B_PPOMIAR i B_KPOMIAR uwzgledniono
             //wymiarowanie do osi kanalu. Azeby nie robic tego za kazdym razem
             //wprowadzono dodatkowa flage b->rezerwa . Jezeli flaga jest zapalona,
             //pomijamy kontrole.
		     
             if ((b->flag & 1) == 0)  //was rezerwa
              {
               if ((b->kod_obiektu==B_PPOMIAR) ||
                   (b->kod_obiektu==B_KPOMIAR))
                {
				  
                  adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                  adp2=adp+sizeof(NAGLOWEK)+b->n-1;

                  //kanal istniejacy

                  //odszukanie srednicy, szukanie w granicach adp1, adp2
                  adp3=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_i, 0);
                  if (adp3 != NULL)
                   {
                    T=(TEXT *)adp3;
                    srednica_t = strpbrk(T->text, "-1234567890.,");
                    if (srednica_t != NULL)
                    {
                     strcpy(srednica_tt, srednica_t);
                     srednica=0;
                     char *srednica_tt_=decimal_dot(srednica_tt);
                     srednica = strtod(srednica_tt_, &bp1);
                     if (bp1 != srednica_tt_ && errno != ERANGE)
                        {
                            //jezeli srednica>10 to odszukanie punktow i ustawienie
                            //flagi obiektt2=O3Os
                            if (srednica>10)
                            {
                                adp3=find_obj(adp1, adp2, Opoint, 1, 0);
                                adp4=find_obj(adp1, adp2, Opoint, 2, 0);
                                if (adp3 != NULL)
                                {
                                    P=(T_Point *) adp3;
                                    if ((Layers[P->warstwa].on==TRUE) && (Layers[P->warstwa].edit==TRUE))
                                        P->obiektt3=O3Os;
                                }
                                if (adp4 != NULL)
                                {
                                    P=(T_Point *) adp4;
                                    if ((Layers[P->warstwa].on==TRUE) && (Layers[P->warstwa].edit==TRUE))
                                        P->obiektt3=O3Os;
                                }
                            }
                        }
                    }
                   }
                  //kanal projektowany

                  //odszukanie srednicy, szukanie w granicach adp1, adp2
                  adp3=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_p, 0);
                  if (adp3 != NULL)
                   {
                    T=(TEXT *)adp3;
                    srednica_t = strpbrk(T->text, "-1234567890.,");
                    if (srednica_t != NULL)
                     {
                      strcpy(srednica_tt, srednica_t);
                      srednica=0;
                      char *srednica_tt_=decimal_dot(srednica_tt);
                      srednica = strtod(srednica_tt_, &bp1);
                      if (bp1 != srednica_tt_ && errno != ERANGE)
                      {
                          //jezeli srednica>10 to odszukanie punktow i ustawienie
                          //flagi obiektt2=O3Os
                          if (srednica>10)
                          {
                              adp3=find_obj(adp1, adp2, Opoint, 3, 0);
                              adp4=find_obj(adp1, adp2, Opoint, 4, 0);
                              if (adp3 != NULL)
                              {
                                  P=(T_Point *) adp3;
                                  if ((Layers[P->warstwa].on==TRUE) && (Layers[P->warstwa].edit==TRUE))
                                      P->obiektt3=O3Os;
                              }
                              if (adp4 != NULL)
                              {
                                  P=(T_Point *) adp4;
                                  if ((Layers[P->warstwa].on==TRUE) && (Layers[P->warstwa].edit==TRUE))
                                      P->obiektt3=O3Os;
                              }
                          }
                      }
                     }
                   }
                  //ustawienie flagi
                  b->flag += 1;  //was rezerwa
                }
              }

             //nalezy nastepnie sprawdzic, czy nastepujacy po znaczniku element
             //element nie nalezy do warstwy -W lub -E
            L=(LINIA *)(adp + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu);
            a_odl_1=100000;
            if (L->obiekt==OdBLOK)
             {
               adp1=adp + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
               b1=(BLOK *)adp1;
               L=(LINIA *)(adp1 + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
              /* switch (L->obiekt)
               {
                 case Olinia:a_odl_1=L->x1;
                 break;
                 case Otekst: T1=(TEXT *)L;
                              a_odl_1=T1->x;
                 break;
                 case Ookrag:
                 case Okolo:  O=(OKRAG*)L;
                              a_odl_1=O->x;
                 break;
                 case Oluk :  l=(LUK*)L;
                              a_odl_1=l->x;
                 break;
               }
               */
             }
             /*
             else
              {
               switch (L->obiekt)
               {
                 case Olinia:a_odl_1=L->x1;
                 break;
                 case Otekst: T1=(TEXT *)L;
                              a_odl_1=T1->x;
                 break;
                 case Ookrag:
                 case Okolo:  O=(OKRAG*)L;
                              a_odl_1=O->x;
                 break;
                 case Oluk :  l=(LUK*)L;
                              a_odl_1=l->x;
                 break;
               }
              } */

            if ((L->blok==ElemBlok) && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
             {
             typ_bloku=1;
             //przesuniecie bloku
             if (b->dlugosc_opisu_obiektu>1)
              {
                ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
                a_odl = (milimetryob(ptrs_desc_bl->x-local_x)*skala_profilu) + (first_hektometr * 100);

                if (a_odl>a_odl_1) a_odl=a_odl_1;

                if (a_odl>max_odl) max_odl=a_odl;

                if (Check_if_GE (a_odl, p_odl)==TRUE)
                 {
                   //odszukanie ewentualnego bloku LASTB()
                   if (b->blok==ElemBlok)
                    {
                     if (NULL != (ptrs_blok = LASTB (adp)))
                      {
                        ADP=(char *)ptrs_blok;
                        ADK=(char *)ptrs_blok+sizeof(NAGLOWEK)+ptrs_blok->n - 1 ;
                      }
                       else
                        {
                         ADP=adp;
                         ADK=adp+sizeof(NAGLOWEK)+b->n - 1 ;
                        }
                    }
                    else
                     {
                      ADP=adp;
                      ADK=adp+sizeof(NAGLOWEK)+b->n - 1 ;
                     }

					
                   //przesuniecie o del_wspx
                   zmien_atrybut(ADP,ADK,Anormalny,Ablok);
                   transformacja_blok(ADP,ADK,del_wspx,0,0,0,Tprzesuw,0);
                //   ptrs_desc_bl->x += del_wspx;
                   zmien_atrybut(ADP,ADK,Ablok,Anormalny);
				   
                 }
              }
             }
           }
     else if (b->kod_obiektu==B_INSTALACJE_OPIS)
      {
       typ_bloku=2;
      }
      else typ_bloku=0;
      adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
      }
      else adp+=sizeof(NAGLOWEK)+b->n;
     }
     else adp+=sizeof(NAGLOWEK)+b->n;
    }
    else
    {
      if ((L->obiekt==Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety)
        && (L->warstwa==Current_Layer)
		&& (Layers[L->warstwa].on==TRUE)
        && (Layers[L->warstwa].edit==TRUE)
        && ((typ_bloku==1) || (typ_bloku==2)))
      { 
         T = (TEXT*)adp;
         //sprawdzenie czy jest to odleglosc
         if (T->typ==n_odleglosc)
          {
           //sprawdzenie, czy wartosc wieksza niz wartosc p_odl
           //dodanie pelnych hektometrow na podstawie nastepujacej linii
           adp1=adp+ L->n + sizeof(NAGLOWEK) ;
           L1 = (LINIA*)adp1;
           a_odl = (milimetryob(L1->x1-local_x)*skala_profilu) + (first_hektometr * 100);

           //if (a_odl>=p_odl)  //zmiana calego bloku tzn. zmiana tekstu
                              //oraz zmiana wspolrzednych x

            aa_odl=strtod(decimal_dot(T->text), &bp1);

            if (Check_if_Equal (a_odl, aa_odl)==FALSE)
            {
              //new_odl=a_odl+d_odl;
              new_odl=a_odl;
              //przesuniecie juz nastapilo
              //odjecie pelnych hektometrow

              odl_i=(int) new_odl;
              odl_1=(odl_i % 100);
              odl_r=odl_1+(new_odl-odl_i);

              memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);

              sprintf_prec(tekst_p,odleglosc_prec,odl_r);
              l_kr=add_000(tekst_p,odleglosc_poz);
              strcpy(&Tp.text[0],tekst_p);

              decimal(Tp.text);

              Tp.dl = strlen(Tp.text);
              Tp.n = T18 + Tp.dl;
              del_adp=adp-dane;
              if ( (t = korekta_obiekt((void *)adp, (void *)&Tp)) == NULL) return 0;
             // adp=dane+del_adp;
              adp=(char *)t;
              adk=dane+dane_size;
              L=(LINIA*)adp;
            }
          }

         else if ((T->typ==n_rzedna_kanalu_i) || (T->typ==n_rzedna_kanalu_p))
            {
             //sprawdzenie polozenia punktu znaczacego  - dla rki i rkp - dwa punktu
             //                                           dla tu i rt - jeden punkt
             //sprawdzenie, czy wartosc inna niz rzeczywiste polozenie punktu
             adp1=adp+ L->n + sizeof(NAGLOWEK) ;
             P = (T_Point*)adp1;
             P1 = (T_Point*)(adp1 + sizeof(NAGLOWEK) + P->n);     //zawsze jest drugi punkt
			 if ((P->obiekt==Opoint) && (P1->obiekt==Opoint))
			 {
             if (P->obiektt3==O3Os)
               {
                //P1 = (T_Point*)(adp1 + sizeof(NAGLOWEK) + P->n);
                a_rz = (milimetryob(((P->y+P1->y)/2)-local_y-tablica_y[14]) + poziom_pp);
               }
                 else
                   a_rz = (milimetryob(P->y-local_y-tablica_y[14]) + poziom_pp);

             //if (a_rz!=aa_rz)  //zmiana polozenia punktu - wsp.y

			 aa_rz=strtod(decimal_dot(T->text), &bp1);

			 if (Check_if_Equal (a_rz, aa_rz)==FALSE)
				{
				new_rz=a_rz;
				//przeciagniecie juz nastapilo

				 memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);

				if (T->typ==n_rzedna_kanalu_i)
                {
                 sprintf_prec(tekst_p,rzedna_i_prec,new_rz);
                 l_kr=add_000(tekst_p,rzedna_i_poz);
                }
                 else
                  {
                   sprintf_prec(tekst_p,rzedna_p_prec,new_rz);
                   l_kr=add_000(tekst_p,rzedna_p_poz);
                  }

				strcpy(&Tp.text[0],tekst_p);

                decimal(Tp.text);

				//ewentualna korekta polozenia punktu
			    aaa_rz=strtod(decimal_dot(Tp.text), &bp1);
				del_rz=jednostkiOb(aaa_rz-new_rz);

				P->y+=(float)del_rz;
				P1->y+=(float)del_rz;


				Tp.dl = strlen(Tp.text);
				Tp.n = T18 + Tp.dl;
				del_adp=adp-dane;
				if ( (t = korekta_obiekt((void *)adp, (void *)&Tp)) == NULL) return 0;
				adp=(char *)t;
				adk=dane+dane_size;
				L=(LINIA*)adp;
				}
             }
            }
           else if ((T->typ==n_rzedna_ulicy) ||
                    ((T->typ==n_rzedna_terenu) && (T->obiektt3==O3NormalText)))
            {
             //sprawdzenie polozenia punktu znaczacego  - dla rki i rkp - dwa punktu
             //                                           dla tu i rt - jeden punkt
             //sprawdzenie, czy wartosc inna niz rzeczywiste polozenie punktu
             equal_ground=FALSE;

             if (T->typ==n_rzedna_terenu) typ_bloku=0;

             adp1=adp+ L->n + sizeof(NAGLOWEK) ;
             P = (T_Point*)adp1;
             if (P->obiekt==Opoint)
			 {
				a_rz = (milimetryob(P->y-local_y-tablica_y[14]) + poziom_pp);

				if (T->typ==n_rzedna_ulicy)
				{
				//poszukiwanie punktu rzednej terenu
				if (NULL != (ptrs_blok=FIRSTB(adp)))
					{
					//znaleziono blok
					adp2=(char *)ptrs_blok;
					adp3=adp1 + sizeof(NAGLOWEK) + ptrs_blok->n - 1;
					adp4=find_obj(adp2, adp3, Opoint, 6, 0);
					if (adp4 != NULL)
					{
						P1 = (T_Point *)adp4;
						if (Check_if_Equal(P->y, P1->y)==TRUE)
						{
							equal_ground=TRUE;
						}
					}
					}
				}

				//if (a_rz!=aa_rz)  //zmiana polozenia punktu - wsp.y

                aa_rz=strtod(decimal_dot(T->text), &bp1);

				if ((Check_if_Equal (a_rz, aa_rz)==FALSE) || (equal_ground==TRUE))
				{
				new_rz=a_rz;
				//przeciagniecie juz nastapilo

				memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);

				//sprawdzenie, czy aby rzedna terenu projektowanego (ulicy)
				//jest rowna rzednej terenu istniejacego
				if (T->typ==n_rzedna_ulicy)
				{
                 sprintf_prec(tekst_p,rzedna_tp_prec,new_rz);
                 l_kr=add_000(tekst_p,rzedna_tp_poz);
				}
                 else
                   {
                    sprintf_prec(tekst_p,rzedna_ti_prec,new_rz);
                    l_kr=add_000(tekst_p,rzedna_ti_poz);
                   }

				//ewentualna korekta polozenia punktu
				aaa_rz=strtod(tekst_p, &bp1);
				del_rz=jednostkiOb(aaa_rz-new_rz);
				P->y+=(float)del_rz;

				if (equal_ground==FALSE)
				{
					strcpy(&Tp.text[0],tekst_p);
                    decimal(Tp.text);
				}
				else
				{
				    strcpy(tekst_p,"");
				    strcpy(&Tp.text[0],tekst_p);
				}

				Tp.dl = strlen(Tp.text);
				Tp.n = T18 + Tp.dl;
				del_adp=adp-dane;
				if ( (t = korekta_obiekt((void *)adp, (void *)&Tp)) == NULL) return 0;
				adp=(char *)t;
				adk=dane+dane_size;
				L=(LINIA*)adp;
				}
             }
            }
           else if ((T->typ==n_rzedna_terenu) && (T->obiektt3==O3SpecialText))
             {
            //   L1=(LINIA *)(adp + sizeof(NAGLOWEK) + T->n) ;
               //sprawdzenie polozenia bloku odwiertu
               //odszukanie poczatku bloku;

               if (NULL != (ptrs_blok=FIRSTB(adp)))
                 {
                  adp1=(char *)ptrs_blok;
                  adp1+=sizeof(NAGLOWEK) + B3 + ptrs_blok->dlugosc_opisu_obiektu ;
                  L1 = (LINIA *)adp1;
                  a_rz = milimetryob(L1->y1-local_y-tablica_y[14]);
                  a_rz += poziom_pp;

                  aa_rz=strtod(decimal_dot(T->text), &bp1);

                  if (Check_if_Equal (a_rz, aa_rz)==FALSE)
                   {
                    new_rz=a_rz;
                    //przeciagniecie juz nastapilo

                    memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);

                    sprintf_prec(tekst_p,rzedna_ti_prec,new_rz);
                    l_kr=add_000(tekst_p,rzedna_ti_poz);
                    strcpy(&Tp.text[0],tekst_p);

                    decimal(Tp.text);

                    //ewentualna korekta polozenia punktu

                    aaa_rz=strtod(tekst_p, &bp1);
                    del_rz=jednostkiOb(aaa_rz-new_rz);
                    P->y+=(float)del_rz;

                    Tp.dl = strlen(Tp.text);
                    Tp.n = T18 + Tp.dl;
                    del_adp=adp-dane;
                    if ( (t = korekta_obiekt((void *)adp, (void *)&Tp)) == NULL) return 0;
                    adp=(char *)t;
                    adk=dane+dane_size;
                    L=(LINIA*)adp;
                   }
                 }
             }
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }


  if (max_odl>0)
   {
    max_hektometr = ceil(max_odl / 100);
    if (max_hektometr>(last_hektometr-first_hektometr))
     {
        //only for profile 0 and 2
        if ((dane_global.odl==0) || (dane_global.odl==2))
            l_kr=generowanie_bloku_hektometrow(last_hektometr, skala_profilu, max_hektometr, 1, 1);
     }
      else if (max_hektometr<(last_hektometr-first_hektometr))
       {
		usuwanie_osi_hektometrow();  
		ADP=dane;
		ADK=dane + dane_size;
		usun_blok	(ADP, ADK) ;
        if ((dane_global.odl==0) || (dane_global.odl==2))
		    l_kr=generowanie_bloku_hektometrow(first_hektometr, skala_profilu, max_hektometr, 1, 1);
       }
   }
  return 0;
}

/*************/

//extern "C" { extern void przesuniecie_profilu_(void); }
void przesuniecie_profilu_(void)
{
 przesuniecie_profilu(0, 0);
}

static int change_cavity_p(int change_p_i)  //change_p_i==0 bez zmian
                                            //change_p_i==1 zmiana na projektowany
                                            //change_p_i==2 zmiana na istniejacy
{
  LINIA *L, *L1, *L2;
  BLOK  *b, *b1;
  T_Point *P;
  TEXT *T;
  TEXT *T_cav;
  TEXT Tp, *t;
  double zaglebienie;
  double rzedna_kanalu;
  double rzedna_terenu;
  char *zaglebienie_;
  char *rzedna_kanalu_;
  char *rzedna_terenu_;
  char  *adp, *adp1, *adp2, *adp3, *adp4, *adp5, *adp6, *adp7, *adp8, *adk, *adk1;
  int i,j;
  char *bp1, *bp2;
  long del_adp;
  char tekst_p[60];
  int l_kr;
  BOOL zaglebienie_ok;
  double max_y_point;
  int b_warstwa;
  BOOL korekta_t;

  adp = dane ;
  adk = dane + dane_size ;

  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
  	  b=(BLOK *)adp;
     b_warstwa=get_block_layer(b);
     if (b_warstwa==Current_Layer)
     {
      if ((Layers[b_warstwa].edit==1) &&
          (Layers[b_warstwa].on==1))
      {
     if (((b->kod_obiektu==B_PPOMIAR) ||
         (b->kod_obiektu==B_KPOMIAR))
         && ((b->atrybut != Abad) &&
             (b->atrybut != Ausuniety)))
           {
             adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
             adp2=adp+sizeof(NAGLOWEK)+b->n-1;
             //kanal projektowany
             korekta_t=FALSE;
             //odszukanie opisu zaglebienia
             adp3=find_obj(adp1, adp2, Otekst, n_zaglebienie_kanalu_p, 0);
             if (adp3 != NULL)
              {
               zaglebienie_ok=FALSE;
               T_cav=(TEXT *) adp3;
               if ((T_cav->atrybut != Abad) && (T_cav->atrybut != Ausuniety)
                   && (Layers[T_cav->warstwa].on==TRUE) && (Layers[T_cav->warstwa].edit==TRUE))
               {
               adp4=find_obj(adp1, adp2, Otekst, n_rzedna_kanalu_p, 0);
               if (adp4 != NULL)
                {
                 //ewentualna zmiana opcji teren projektowany / teren istniejacy
                 if (change_p_i == 1) //teren projektowany
                   {
                    T_cav->obiektt3=O3Teren_p;
                   }
                     else if (change_p_i == 2) //teren istniejacy
                      {
                       T_cav->obiektt3=O3Teren_i;
                      }

                 if (T_cav->obiektt3==O3Teren_i)
                   adp5=find_obj(adp1, adp2, Otekst, n_rzedna_terenu, 0);
                  else
                   {
                    adp5=find_obj(adp1, adp2, Otekst, n_rzedna_ulicy, 0);
                    if (adp5 != NULL)
                     {
                       T=(TEXT *) adp5;
                       if (strlen(T->text)==0) adp5=find_obj(adp1, adp2, Otekst, n_rzedna_terenu, 0);
                     }
                     else adp5=find_obj(adp1, adp2, Otekst, n_rzedna_terenu, 0);
                   }

                   if (adp5 != NULL)
                    {
                      T=(TEXT *) adp4;
                      rzedna_kanalu_ = strpbrk(T->text, "-1234567890.,");
                      if (rzedna_kanalu_ != NULL)
                       {
                         char *rzedna_kanalu_b=decimal_dot(rzedna_kanalu_);
                         rzedna_kanalu = strtod(rzedna_kanalu_b, &bp1);
                         if (bp1!=rzedna_kanalu_b && *bp1 == '\0' && errno != ERANGE)
                          {
                            T=(TEXT *) adp5;
                            rzedna_terenu_ = strpbrk(T->text, "-1234567890.,");
                            if (rzedna_terenu_ != NULL)
                              {
                                char *rzedna_terenu_b=decimal_dot(rzedna_terenu_);
                                rzedna_terenu = strtod(rzedna_terenu_b, &bp2);
                                if (bp2!=rzedna_terenu_b && *bp2 == '\0' && errno != ERANGE)
                                 {
                                  zaglebienie=rzedna_terenu-rzedna_kanalu;
                                  zaglebienie_ok=TRUE;
                                 }
                              }
                          }
                       }
                    }
                }
                //modyfikacja tekstu
                memmove (&Tp, (void *)T_cav, sizeof(NAGLOWEK)+T_cav->n);
                if (zaglebienie_ok==TRUE)
                 {
                  sprintf_prec(tekst_p,zaglebienie_prec,zaglebienie);
                  l_kr=add_000(tekst_p,zaglebienie_poz);
                 }
                else
                 {
                    strcpy(tekst_p,"");
                 }
                strcpy(&Tp.text[0],tekst_p);
                Tp.dl = strlen(Tp.text);
                Tp.n = T18 + Tp.dl;
                del_adp=adp-dane;
                /////////
                korekta_t=TRUE;
                /////////
                // if ( (t = korekta_obiekt((void *)adp3, (void *)&Tp)) == NULL) return 0;
                // adp=(char *)t;
                // adk=dane+dane_size;
                // L=(LINIA*)adp;
                /////////
               }
              }
             ///////////////
             if (change_p_i==0)    //dociaganie osi  //!!!!!!!!!!!!!!!
              {
                //poszukiwanie najwyzszego punktu
                max_y_point=0;
                for (i=1; i<7; i++)
                 {
                   adp7=find_obj(adp1, adp2, Opoint, i, 0);
                   if (adp7 != NULL)
                    {
                     P=(T_Point *) adp7;
                     if (P->y>max_y_point) max_y_point=P->y;
                    }
                 }
                if (max_y_point>0)
                 {
                   //poszukiwanie linii
                   adp8=find_obj(adp1, adp2, Olinia, 32, 0);
                   if (adp8 != NULL)
                    {
                     L2=(LINIA *) adp8;
                     L2->y2 = (float)max_y_point;
                    }
                 }
              }
             ///////////////
             if (korekta_t==TRUE)
              {
                if ( (t = korekta_obiekt((void *)adp3, (void *)&Tp)) == NULL) return 0;
                adp=(char *)t;
                adk=dane+dane_size;
                L=(LINIA*)adp;
              }
           }
      }
     }
    }
    if (L->obiekt==OdBLOK)
     {
      b=(BLOK *)adp;
      b_warstwa=get_block_layer(b);
      if (b_warstwa==Current_Layer)
      {
       if ((Layers[b_warstwa].edit==1) &&
           (Layers[b_warstwa].on==1))
        {
         adp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
        }
       else adp += sizeof(NAGLOWEK) + b->n;
      }
      else adp += sizeof(NAGLOWEK) + b->n;
     }
    else  adp += L->n + sizeof(NAGLOWEK) ;
    L = (LINIA*)adp;
  }
  return 0;
}

/*************/
static int change_skala_profilu(double skala_profilu0)
{
  int i_retval;
  LINIA *L, *L1;
  BLOK  *b, *b1, *b2;
  BLOK *ptrs_block, *ptrs_block1 ;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  TEXT *T;
  TEXT Tp, *t;
  char  *adp, *adk, *adp1, *adp2, *adp3, *adp4, *adk1;
  T_Point *P;
  int i,j;
  int typ_bloku;
  char odleglosc[60];
  char tekst_p[60];
  int l_kr, blok_n;
  //przesuniecie wszystkich blokow od p_odl o d_odl, d_odl moze byc < 0

//  double a_odl, aa_odl, new_odl,
  double local_x, local_y;
  double first_hektometr, skala_profilu, poziom_pp, typ_profilu;
  long del_adp;
  int odl_i;
  double odl_1, odl_r;
  double a_rz, aa_rz, new_rz;
  char *bp1;
  double del_wspx;
  double wsp_x;
  double hektometry_r;
  int hektometry;
  int b_warstwa;


  adp = dane ;
  adk = dane + dane_size ;

  first_hektometr=get_hektometr_p();
  skala_profilu=get_skala_profilu_x();

  typ_profilu=get_typ_profilu();
  tablica_0_1(typ_profilu);

  poziom_pp=get_poziom_pp();
  local_x=get_localx();
  local_y=get_localy();


  L = (LINIA*)adp;
  typ_bloku=0;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->blok==NoElemBlok) typ_bloku=0;
    if (L->obiekt == OdBLOK)
    {
  	  b=(BLOK *)adp;
     b_warstwa=get_block_layer(b);
     if (b_warstwa==Current_Layer)
     {
      if ((Layers[b_warstwa].edit==1) &&
          (Layers[b_warstwa].on==1))
      {
     if ((
         (b->kod_obiektu==B_PPOMIAR) ||
         (b->kod_obiektu==B_KPOMIAR) ||
         (b->kod_obiektu==B_INSTALACJE))
         && ((b->atrybut != Abad) &&
             (b->atrybut != Ausuniety)))
           {
             //element nie nalezy do warstwy -W lub -E
            adp1=adp + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
            L1=(LINIA *)adp1;
            if (L1->obiekt != OdBLOK)
            {
             if ((L1->blok==ElemBlok) && (Layers[L1->warstwa].on==TRUE) && (Layers[L1->warstwa].edit==TRUE))
              {
               typ_bloku=1;  //do przesuniecia
              }
            }
             else
              {
                adp1=adp + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                b1=(BLOK *)adp1;
                L1=(LINIA *)(adp1 + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
                if (L1->obiekt != OdBLOK)
                 {
                  if ((L1->blok==ElemBlok) && (Layers[L1->warstwa].on==TRUE) && (Layers[L1->warstwa].edit==TRUE))
                   {
                     typ_bloku=1;  //do przesuniecia
                   }
                 }
                  else  //dla nowego bloku
                   {
                    adp2=adp1 + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu;
                    b2=(BLOK *)adp2;
                    L1=(LINIA *)(adp2 + sizeof(NAGLOWEK) + B3 + b2->dlugosc_opisu_obiektu);
                    if (L1->obiekt != OdBLOK)
                     {
                      if ((L1->blok==ElemBlok) && (Layers[L1->warstwa].on==TRUE) && (Layers[L1->warstwa].edit==TRUE))
                       {
                        typ_bloku=1;  //do przesuniecia
                       }
                     }
                   }
              }
           // adp +=  sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu ;
           }
            else
            {
             if (((b->kod_obiektu==B_ODWIERT_OLD) || (b->kod_obiektu==B_ODWIERT))
                 && ((b->atrybut != Abad) &&
                     (b->atrybut != Ausuniety)))
               {
                 L1=(LINIA *)(adp + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu);
                 if ((L1->blok==ElemBlok) && (Layers[L1->warstwa].on==TRUE) && (Layers[L1->warstwa].edit==TRUE))
                  {
                    //przesuniecie calego bloku
                    if (b->dlugosc_opisu_obiektu>1)
                     {
                      ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
                    //  a_odl = (milimetryob(ptrs_desc_bl->x-local_x)*skala_profilu) + (first_hektometr * 100);
                      old_var=ptrs_desc_bl->x;
                      old_var-=local_x;
                      wsp_x=skala_profilu0/skala_profilu;
                      new_var=old_var*wsp_x;
                      del_wspx=new_var-old_var;

                      if (del_wspx != 0)
                       {
                        //mozna jeszcze odszukac blok LASTB() ???????????????
                        //przesuniecie o del_wspx
                        ADP=adp;
                        ADK=adp+sizeof(NAGLOWEK)+b->n - 1 ;
                        zmien_atrybut(ADP,ADK,Anormalny,Ablok);
                        transformacja_blok(ADP,ADK,del_wspx,0,0,0,Tprzesuw,0);
                        //   ptrs_desc_bl->x += del_wspx;
                        zmien_atrybut(ADP,ADK,Ablok,Anormalny);
                      }
                     }
                  }
               //  adp +=  sizeof(NAGLOWEK) + b->n ;
               }
            }
        adp +=  sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu ;
      }
      else adp+=sizeof(NAGLOWEK) + b->n;
     }
     else adp+=sizeof(NAGLOWEK) + b->n;
    }
    else if ((L->obiekt==Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety)
        && (L->warstwa==Current_Layer)
        && (Layers[L->warstwa].on==TRUE)
        && (Layers[L->warstwa].edit==TRUE)
        && (typ_bloku==1))
      {
         T=(TEXT *) adp;
         if (T->typ == n_odleglosc)
           {
                strcpy(odleglosc, decimal_dot(T->text));

                new_var = strtod(odleglosc, &bp1);

                //odczytanie polozenia nastepujacej linii
                //i dodanie liczby hektometrow
                L1=(LINIA *)(adp + sizeof(NAGLOWEK) + T->n);
                //obliczenie poczatkowej wartosci hektometrow
                hektometry_r = ((milimetryob(L1->x1 - local_x) * skala_profilu0)/100) + (first_hektometr);
                hektometry = (int) hektometry_r;
                new_var0 = (hektometry * 100);

                new_var += new_var0;

                if (bp1!=odleglosc && *bp1 == '\0' && errno != ERANGE)
                 {
                  if (skala_profilu!=0)
                  {
                   new_var=(jednostkiOb(new_var - (first_hektometr * 100))/skala_profilu) + local_x;
                   //poszukiwanie aktualnej pozycji na podstawie nastepujacej linii
                   adp1=adp + sizeof(NAGLOWEK) + T->n;
                   L1=(LINIA *) adp1;
                   if ((L1->obiekt==Olinia) && (L1->blok==ElemBlok) && (Layers[L1->warstwa].on==TRUE) && (Layers[L1->warstwa].edit==TRUE))
                   {
                    old_var=L1->x1;
                    del_var=new_var-old_var;
                    if (del_var!=0)
                    {
                     //odszukanie poczatku bloku
                     if (NULL != (ptrs_block = LASTB (adp)))
                      {
                       //przesuniecie bloku
                        ADP=(char *)ptrs_block;
                        ADK=(char *)ptrs_block+sizeof(NAGLOWEK)+ptrs_block->n - 1 ;
                        zmien_atrybut(ADP,ADK,Anormalny,Ablok);
                        transformacja_blok(ADP,ADK,del_var,0,0,0,Tprzesuw,0);
                        zmien_atrybut(ADP,ADK,Ablok,Anormalny);
                        //nalezy teraz przeskalowac wszysko co lezy pomiedzy
                        //LASTB() a FIRSTB() - srodkiem isometrii jest
                        //b->x;, skalax=wsp_x
                        //+++++++++++++++++++
                        if (NULL != (ptrs_block1 = (BLOK  *)FIRSTB (adp)))
                         {
                          if ((char *)ptrs_block1 != (char *)ptrs_block)
                           {
                            wsp_x=skala_profilu0/skala_profilu;

                            //poszukiwanie najbliszczego bloku PPOMIAR, KPOMIAR lub INSTALACJE
                            //przeskalowaniu podlegaj elementy ADP=(najblizszy blok) lub (FIRSTB)
                            //az do ADK=(char *)ptrs_block1 - 1;
                            //!!!!!!!!!!!!!!!!!

                            //!!!!!!!!!!!!!!!!!
                            ADP=(char *)ptrs_block;
                            ADK=(char *)ptrs_block1 - 1;
                            zmien_atrybut(ADP,ADK,Anormalny,Ablok);
                            transformacja_blok(ADP,ADK,L1->x1,L1->y1,wsp_x,1,Tskala,0);
                            zmien_atrybut(ADP,ADK,Ablok,Anormalny);
                           }
                         }
                        //+++++++++++++++++++
                      }
                    }
                   }
                  }
                 }
           }
        adp +=  L->n + sizeof(NAGLOWEK) ;
      }
    else
      {
       adp += L->n + sizeof(NAGLOWEK) ;
      }
    L = (LINIA*)adp;
  }
 return 0;
}
/*************/

static int change_pp(double del_pp)
{
  int i_retval;
  LINIA *L, *L1;
  BLOK  *b, *b1, *ptrs_block, *ptrs_block1;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  TEXT *T, *T1;
  TEXT Tp, *t;
  char  *adp, *adk, *adp1, *adp2, *adp3, *adp4, *adk1;
  T_Point *P, *P1;
  int i,j;
  int typ_bloku;
  char odleglosc[60];
  char tekst_p[60];
  int l_kr, blok_n;
  //przesuniecie wszystkich blokow od p_odl o d_odl, d_odl moze byc < 0
  double a_odl, aa_odl, new_odl, local_x, local_y;
  double first_hektometr, skala_profilu, poziom_pp, typ_profilu;
  long del_adp;
  int odl_i;
  double odl_1, odl_r;
  double a_rz, aa_rz, new_rz;
//  double delta_pp;
// double old_var=0;
// double new_var0=0;
// double new_var=0;
//  double del_var=0;
  char *bp1;
  BOOL found_i_opis;
  int b_warstwa;


  adp = dane ;
  adk = dane + dane_size ;

  first_hektometr=get_hektometr_p();
  skala_profilu=get_skala_profilu_x();

  typ_profilu=get_typ_profilu();
  tablica_0_1(typ_profilu);

  poziom_pp=get_poziom_pp();
  local_x=get_localx();
  local_y=get_localy();

  delta_pp=jednostkiOb(del_pp);


  L = (LINIA*)adp;
  typ_bloku=0;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK) typ_bloku=0;
    else if (L->blok==NoElemBlok) typ_bloku=0;
    if (L->obiekt == OdBLOK)
    {
  	  b=(BLOK *)adp;
     b_warstwa=get_block_layer(b);
     if (b_warstwa==Current_Layer)
     {
      if ((Layers[b_warstwa].edit==1) &&
          (Layers[b_warstwa].on==1))
      {
     if ((
         (b->kod_obiektu==B_PPOMIAR) ||
         (b->kod_obiektu==B_KPOMIAR))
         && ((b->atrybut != Abad) &&
             (b->atrybut != Ausuniety)))     //
           {
             //element nie nalezy do warstwy -W lub -E
            L=(LINIA *)(adp + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu);
            if ((L->blok==ElemBlok) && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
             {
             //odszukanie na koncu bloku linii o typie 32 i przesuniecie
             //drugiego konca linii o wartosc del_pp
             adp2=(adp + sizeof(NAGLOWEK) + b->n);
             adp2-=sizeof(LINIA);
             L1=(LINIA *) adp2;
             if ((Layers[L1->warstwa].on==TRUE) && (Layers[L1->warstwa].edit==TRUE))
              {
              if ((L1->obiekt==Olinia) && (L1->typ==32))
               {
                 L1->y2 -= (float)delta_pp;
               }
              }
             typ_bloku=1;
             }
             adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
           }
     else if ((b->kod_obiektu==B_INSTALACJE)
         && ((b->atrybut != Abad) &&
             (b->atrybut != Ausuniety)))
           {
             //nalezy najpierw sprawdzic, czy nastepujacy po znaczniku element
             //element nie nalezy do warstwy -W lub -E

             //najpierw odszukanie bloku b1->kod_obiektu == B_INSTALACJE_OPIS
             adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
             adp2=adp+sizeof(NAGLOWEK)+b->n-1;
             found_i_opis=FALSE;
             while ((found_i_opis==FALSE) && (adp1<adp2))
              {
                L1=(LINIA *)adp1;
                if (L1->obiekt==OdBLOK)
                 {
                  b1=(BLOK *)adp1;
                  if (b1->kod_obiektu==B_INSTALACJE_OPIS)
                   {
                     found_i_opis=TRUE;
                   }
                    else adp1+=(sizeof(NAGLOWEK)+b1->n);
                 }
                  else  adp1+=(sizeof(NAGLOWEK)+L1->n);
              }

             if (found_i_opis==TRUE)
              {
               adp2=(adp1 + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
               L=(LINIA *)adp2;

               if ((L->blok==ElemBlok) && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
                {
                 typ_bloku=0;

                 //przesuniecie bloku w pionie poza pierwszymi obiektami L,T,L

                 L->y1 += (float)delta_pp;
                 L->y2 += (float)delta_pp;
                 adp3=(adp2 + sizeof(NAGLOWEK) + L->n);
                 T=(TEXT *)adp3;
                 if ((T->blok==ElemBlok) && (Layers[T->warstwa].on==TRUE) && (Layers[T->warstwa].edit==TRUE))
                  {
                   T->y += (float)delta_pp;
                   adp4=(adp3 + sizeof(NAGLOWEK) + T->n);
                   L1=(LINIA *)adp4;
                   if ((L1->blok==ElemBlok) && (Layers[L1->warstwa].on==TRUE) && (Layers[L1->warstwa].edit==TRUE))
                    {
                      L1->y1 += (float)delta_pp;
                     // L1->y2 += delta_pp;  //linia zostanie rozciagnieta
                    }
                  }

                 if (b->dlugosc_opisu_obiektu>1)
                  {
                   //przesuniecie o delta_pp
                   if (b->blok==ElemBlok)
                    {
                     if (NULL != (ptrs_block = LASTB (adp)))
                      {
                       //przesuniecie calego bloku
                        ADP=(char *)ptrs_block;
                        ADK=(char *)ptrs_block+sizeof(NAGLOWEK)+ptrs_block->n - 1 ;
                      }
                     else
                      {
                        ADP=adp;
                        ADK=adp+sizeof(NAGLOWEK)+b->n - 1 ;
                      }
                    }
                     else
                      {
                        ADP=adp;
                        ADK=adp+sizeof(NAGLOWEK)+b->n - 1 ;
                      }
                   zmien_atrybut(ADP,ADK,Anormalny,Ablok);
                   transformacja_blok(ADP,ADK,0,-delta_pp,0,0,Tprzesuw,0);
                   zmien_atrybut(ADP,ADK,Ablok,Anormalny);
                  }
                }
              }
             adp+=sizeof(NAGLOWEK)+b->n;
           }
    else if (((b->kod_obiektu==B_ODWIERT_OLD) ||  (b->kod_obiektu==B_ODWIERT))
         && ((b->atrybut != Abad) &&
             (b->atrybut != Ausuniety)))
           {
             //nalezy najpierw sprawdzic, czy nastepujacy po znaczniku element
             //element nie nalezy do warstwy -W lub -E
            L=(LINIA *)(adp + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu);
            if ((L->blok==ElemBlok) && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
             {
              typ_bloku=0;
             //przesuniecie o del_wspx
              if (b->blok==ElemBlok)
                {
                 if (NULL != (ptrs_block = LASTB (adp)))
                   {
                    //przesuniecie calego bloku
                    ADP=(char *)ptrs_block;
                    ADK=(char *)ptrs_block+sizeof(NAGLOWEK)+ptrs_block->n - 1 ;
                   }
                 else
                   {
                     ADP=adp;
                     ADK=adp+sizeof(NAGLOWEK)+b->n - 1 ;
                   }
                }
              else
                  {
                    ADP=adp;
                    ADK=adp+sizeof(NAGLOWEK)+b->n - 1 ;
                   }
              zmien_atrybut(ADP,ADK,Anormalny,Ablok);
              transformacja_blok(ADP,ADK,0, -delta_pp,0,0,Tprzesuw,0);
           //   ptrs_desc_bl->x += del_wspx;
              zmien_atrybut(ADP,ADK,Ablok,Anormalny);
             }
             adp+=sizeof(NAGLOWEK)+b->n;
           }
            else  //inny typ bloku
             {
               typ_bloku=0;
             //  adp+=sizeof(NAGLOWEK)+b->n;   //tak nie moze byc z uwagi na ewentualne
                                               //zgrupowanie elementow w blok
               adp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
             }
      }
      else adp+=sizeof(NAGLOWEK) + b->n;
     }
      else adp+=sizeof(NAGLOWEK) + b->n;
    }
    else
    {
      if ((L->obiekt==Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety)
        && (L->warstwa==Current_Layer)
        && (Layers[L->warstwa].on==TRUE)
        && (Layers[L->warstwa].edit==TRUE)
        && (typ_bloku==1))
      {
         T=(TEXT *) adp;
         if ((T->typ == n_rzedna_kanalu_i) ||
             (T->typ == n_rzedna_kanalu_p) ||
             (T->typ == n_rzedna_ulicy) ||
             (T->typ == n_rzedna_terenu))
         {
             /*++++++++++++++*/
             //wyznaczenie wysokosci na rysunku
             char *rzedna=decimal_dot(T->text);
             new_var0 = strtod(rzedna, &bp1);

             if (bp1!=rzedna && *bp1 == '\0' && errno != ERANGE)
             {
                 if ((Check_if_Equal(new_var0, 0.0)==TRUE) &&
                   (T->typ == n_rzedna_ulicy))  //teren projektowany
                 {
                     //odszukanie rzednej terenu (terenu istniejacego)
                     if (NULL != (ptrs_block = FIRSTB (adp)))
                     {
                         //zakres bloku
                         adp1=(char *)ptrs_block;
                         adp2=(char *)ptrs_block+sizeof(NAGLOWEK)+ptrs_block->n - 1 ;
                         adp3=find_obj(adp, adk, Otekst, n_rzedna_terenu, 0);
                         if (adp3 != NULL)
                         {
                             T1=(TEXT *) adp3;
                             rzedna=decimal_dot(T->text);
                             new_var0 = strtod(rzedna, &bp1);
                         }
                     }
                 }

                 if (bp1!=rzedna && *bp1 == '\0' && errno != ERANGE)
                 {
                     new_var=jednostkiOb(new_var0 - poziom_pp) + local_y + tablica_y[14];
                     //sprawdzenie czy nastepuje punktu
                     adp1=adp+sizeof(NAGLOWEK)+((TEXT*)adp)->n;
                     if (((T_Point*)adp1)->obiekt==Opoint)
                     {
                         P=((T_Point*)adp1);
                         if ((T->typ==n_rzedna_kanalu_i) ||
                             (T->typ==n_rzedna_kanalu_p))
                         {
                             adp2=adp1+sizeof(NAGLOWEK)+((T_Point*)adp1)->n;
                             P1=((T_Point*)adp2);
                             if (P->obiektt3==O3Os)  //wymiarowanie do osi
                             {
                                 old_var=((P->y)+(P1->y))/2;
                                 del_var=new_var-old_var;
                                 P->y += (float)del_var;
                                 P1->y += (float)del_var;
                             }
                             else  //wymiarowanie do dna
                             {
                                 old_var=P->y;
                                 del_var=new_var-old_var;
                                 P->y += (float)del_var;
                                 P1->y += (float)del_var;
                             }
                         }
                         else
                         {
                             old_var=P->y;
                             del_var=new_var-old_var;
                             P->y += (float)del_var;
                         }
                     }
                 }
             }
            /*++++++++++++++*/
         }
          else if (T->typ == n_odleglosc)  //jest okazja zeby przesunac dolaczony blok
            {
             //////////////
             if (NULL != (ptrs_block = FIRSTB (adp)))
              {
               //przesuniecie calego bloku
               adp1=(char *)ptrs_block;
              }
              if (ptrs_block->blok==ElemBlok)
                {
                 if (NULL != (ptrs_block1 = LASTB (adp1)))
                   {
                    //przesuniecie dolaczonego bloku
                    ADP=(char *)ptrs_block1;
                    ADK=(char *)ptrs_block - 1 ;
                    zmien_atrybut(ADP,ADK,Anormalny,Ablok);
                    transformacja_blok(ADP,ADK,0, -delta_pp,0,0,Tprzesuw,0);
                    zmien_atrybut(ADP,ADK,Ablok,Anormalny);
                   }
                }
            }

      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
  return 0;
}

//extern "C" { extern int get_f_s_pomiar_param(char  **adr, int kanal_pomiar, double *x_pomiar, double *y_pomiar); }
int get_f_s_pomiar_param(char **adr, int kanal_pomiar, double *x_pomiar, double *y_pomiar)
{
BLOK *b, *ptrs_blok;
  TEXT *T;
  LINIA *L;
  T_Point *P, *P1;
  char *adp, *adp1, *adp2, *adk;
  int wyrownywanie_pomiar;
 

  if (NULL != (ptrs_blok=FIRSTB((char*)adr)))
   {
     //znaleziono blok

     adp=(char *)ptrs_blok;
     adk=adp + ptrs_blok->n;

     if (kanal_pomiar == 0)  //kanal projektowany
     {
      wyrownywanie_pomiar=0;
      adp1=find_obj(adp, adk, Opoint, 3, 0);   //pierwszy punkt
      if (adp1 == NULL) adp1=find_obj(adp, adk, Opoint, 4, 0);   //drugi punkt
      if (adp1 != NULL)
       {
        P=(T_Point *) adp1;
        if (P->obiektt2==O2BlockHatch25) wyrownywanie_pomiar=1;
         else if (P->obiektt2==O2BlockHatch50) wyrownywanie_pomiar=2;
          else wyrownywanie_pomiar=0;
       }
      if (wyrownywanie_pomiar==1)  //dol
        {
         adp1=find_obj(adp, adk, Opoint, 3, 0);
         if (adp1 != NULL)
          {
           P=(T_Point *)adp1;
           *x_pomiar=P->x;
           *y_pomiar=P->y;
           return 1;
          }
          return 0;
        }

        if (wyrownywanie_pomiar==0)  //gora
         {
          adp1=find_obj(adp, adk, Opoint, 4, 0);
          if (adp1 != NULL)
           {
            P=(T_Point *)adp1;
            *x_pomiar=P->x;
            *y_pomiar=P->y;
            return 1;
           }
            return 0;
         }
           if (wyrownywanie_pomiar==2)  //os
            {
             adp1=find_obj(adp, adk, Opoint, 3, 0);
             adp2=find_obj(adp, adk, Opoint, 4, 0);
             if ((adp1 != NULL) && (adp2 != NULL))
              {
               P=(T_Point *)adp1;
               P1=(T_Point *)adp2;
               *x_pomiar=P->x;
               *y_pomiar=(P->y+P1->y)/2;
               return 1;
              }
             return 0;
            }

     }
      else if (kanal_pomiar == 1)
      {
       wyrownywanie_pomiar=0;
       adp1=find_obj(adp, adk, Opoint, 1, 0);   //pierwszy punkt
       if (adp1 == NULL) adp1=find_obj(adp, adk, Opoint, 2, 0);   //drugi punkt
       if (adp1 != NULL)
        {
         P=(T_Point *) adp1;
         if (P->obiektt2==O2BlockHatch25) wyrownywanie_pomiar=1;
          else if (P->obiektt2==O2BlockHatch50) wyrownywanie_pomiar=2;
           else wyrownywanie_pomiar=0;
        }
       if (wyrownywanie_pomiar==1)  //dol
        {
         adp1=find_obj(adp, adk, Opoint, 1, 0);
         if (adp1 != NULL)
          {
           P=(T_Point *)adp1;
           *x_pomiar=P->x;
           *y_pomiar=P->y;
           return 1;
          }
           return 0;
        }
         if (wyrownywanie_pomiar==0)  //gora
         {
          adp1=find_obj(adp, adk, Opoint, 2, 0);
          if (adp1 != NULL)
           {
            P=(T_Point *)adp1;
            *x_pomiar=P->x;
            *y_pomiar=P->y;
            return 1;
           }
            return 0;
         }
           if (wyrownywanie_pomiar==2)  //os
            {
             adp1=find_obj(adp, adk, Opoint, 1, 0);
             adp2=find_obj(adp, adk, Opoint, 2, 0);
             if ((adp1 != NULL) && (adp2 != NULL))
              {
               P=(T_Point *)adp1;
               P1=(T_Point *)adp2;
               *x_pomiar=P->x;
               *y_pomiar=(P->y+P1->y)/2;
               return 1;
              }
             return 0;
            }
      }
        else if (kanal_pomiar == 5)  //teren istniejacy
         {
           adp1=find_obj(adp, adk, Opoint, 5, 0);
           if (adp1 != NULL)
            {
              P=(T_Point *)adp1;
              *x_pomiar=P->x;
              *y_pomiar=P->y;
              return 1;
            }
             return 0;
         }
         else if (kanal_pomiar == 6)   //teren projektowany / ulica
          {
            adp1=find_obj(adp, adk, Opoint, 6, 0);
            if (adp1 != NULL)
             {
               P=(T_Point *)adp1;
               *x_pomiar=P->x;
               *y_pomiar=P->y;
               return 1;
             }
              return 0;
          }
           else // if (kanal_pomiar == 2)
            {
              adp1=find_obj(adp, adk, Opoint, -1, 0);
              if (adp1 != NULL)
               {
                 P=(T_Point *)adp1;
                 *x_pomiar=P->x;
                 *y_pomiar=P->y;
                 return 1;
               }
                return 0;
            }
   }
  return 0;
}

int find_interpolated(double x, int teren, double *yi)  //teren==0 projektowany
                                                       //teren==1 istniejacy
{ int i;
  BOOL find_teren;

  if (tab_n_plus[teren]==0) return 0;
  find_teren=FALSE;
  i=0;

  while ((find_teren==0) && (i<tab_n_plus[teren]))
   {
     if (Check_if_Equal(tab_r_plus[teren][i].x, x)==TRUE)
      {
       find_teren=TRUE;
       *yi=tab_r_plus[teren][i].y;
      }
     i++;
   }
  if (find_teren==TRUE) return 1;
  return 0;
}

int find_interpolated_io(double x, int teren, double *yi)  //teren==0 projektowany
                                                       //teren==1 istniejacy
{ int i;
  BOOL find_teren;

  if (tab_n_plus_io[teren]==0) return 0;
  find_teren=FALSE;
  i=0;

  while ((find_teren==0) && (i<tab_n_plus_io[teren]))
   {
     if (Check_if_Equal(tab_r_plus_io[teren][i].x, x)==TRUE)
      {
       find_teren=TRUE;
       *yi=tab_r_plus_io[teren][i].y;
      }
     i++;
   }
  if (find_teren==TRUE) return 1;
  return 0;
}


int find_interpolated_tp_ti(double x, double *yi) //poszukiwanie terenu projektowanego
                                                  //lub istniejacego
{ int i;
  BOOL find_teren;
  int l_kr;
  double del_x, del_xi, del_y;

  l_kr=sortowanie_odl(); 

  find_teren=FALSE;

  if ((tab_n[4]<2) && (tab_n[5]<2)) return 0; //brak terenu projektowanego
                                                //i istniejacego

  //teren projektowany
  if (tab_n[4]>1)
  {
  i=1;
  while ((find_teren==0) && (i<tab_n[4]))
   {
     if ((Check_if_GE(x, tab_r[4][i-1].x)==TRUE) &&
         (Check_if_LE(x, tab_r[4][i].x)==TRUE))
      {
       find_teren=TRUE;
       //interpolacja
       del_x=(tab_r[4][i].x - tab_r[4][i-1].x);
       del_y=(tab_r[4][i].y - tab_r[4][i-1].y);
       if (Check_if_Equal(del_x, 0.0))
        {
         *yi=tab_r[4][i].y;
         return 1;
        }
       del_xi=x - tab_r[4][i-1].x;
       *yi=tab_r[4][i-1].y + ((del_xi / del_x) * del_y);
       return 1;
      }
     i++;
   }
  }

  //teren istniejacy
  if (tab_n[5]>1)
  {
  i=1;
  while ((find_teren==0) && (i<tab_n[5]))
   {
     if ((Check_if_GE(x, tab_r[5][i-1].x)==TRUE) &&
         (Check_if_LE(x, tab_r[5][i].x)==TRUE))
      {
       find_teren=TRUE;
       //interpolacja
       del_x=(tab_r[5][i].x - tab_r[5][i-1].x);
       del_y=(tab_r[5][i].y - tab_r[5][i-1].y);
       if (Check_if_Equal(del_x, 0.0))
        {
         *yi=tab_r[5][i].y;
         return 1;
        }
       del_xi=x - tab_r[5][i-1].x;
       *yi=tab_r[5][i-1].y + ((del_xi / del_x) * del_y);
       return 1;
      }
     i++;
   }
  }

  return 0;
}

int find_interpolated_tp(double x, double *yi) //poszukiwanie terenu projektowaneg
{ int i;
  BOOL find_teren;
  int l_kr;
  double del_x, del_xi, del_y;

  l_kr=sortowanie_odl();

  *yi=0;

  find_teren=FALSE;

  if (tab_n[4]<2) return 0; //brak terenu projektowanego

  //teren projektowany
  if (tab_n[4]>1)
  {
  i=1;
  while ((find_teren==0) && (i<tab_n[4]))
   {
     if ((Check_if_GE(x, tab_r[4][i-1].x)==TRUE) &&
         (Check_if_LE(x, tab_r[4][i].x)==TRUE))
      {
       find_teren=TRUE;
       //interpolacja
       del_x=(tab_r[4][i].x - tab_r[4][i-1].x);
       del_y=(tab_r[4][i].y - tab_r[4][i-1].y);
       if (Check_if_Equal(del_x, 0.0))
        {
         *yi=tab_r[4][i].y;
         return 1;
        }
       del_xi=x - tab_r[4][i-1].x;
       *yi=tab_r[4][i-1].y + ((del_xi / del_x) * del_y);
       return 1;
      }
     i++;
   }
  }
  return 0;
}


int find_interpolated_ti(double x, double *yi) //poszukiwanie terenu istniejacego
{ int i;
  BOOL find_teren;
  int l_kr;
  double del_x, del_xi, del_y;

  l_kr=sortowanie_odl();

  *yi=0;

  find_teren=FALSE;

  if (tab_n[5]<2) return 0; //brak terenu istniejacego

  //teren istniejacy
  if (tab_n[5]>1)
  {
  i=1;
  while ((find_teren==0) && (i<tab_n[5]))
   {
     if ((Check_if_GE(x, tab_r[5][i-1].x)==TRUE) &&
         (Check_if_LE(x, tab_r[5][i].x)==TRUE))
      {
       find_teren=TRUE;
       //interpolacja
       del_x=(tab_r[5][i].x - tab_r[5][i-1].x);
       del_y=(tab_r[5][i].y - tab_r[5][i-1].y);
       if (Check_if_Equal(del_x, 0.0))
        {
         *yi=tab_r[5][i].y;
         return 1;
        }
       del_xi=x - tab_r[5][i-1].x;
       *yi=tab_r[5][i-1].y + ((del_xi / del_x) * del_y);
       return 1;
      }
     i++;
   }
  }

  return 0;
}


static double get_srednice(char *text)
{ char *bp1;
  double srednica;
  char *srednica_t;
  char srednica_tt[60];

  srednica=0;
  srednica_t = strpbrk(text, "-1234567890.,");
  if (srednica_t != NULL)
   {
    strcpy(srednica_tt, decimal_dot(srednica_t));
    srednica = strtod(srednica_tt, &bp1);
    if (bp1!=srednica_tt && errno != ERANGE)
        return srednica;
   }
  return 0.;
}

int get_net(char  **adr)
{ BLOK *b, *b1, *ptrs_blok, *ptrs_blok1;
  T_Desc_Ex_Block *ptrs_desc_bl,*ptrs_desc_bl1 ;
  TEXT *T;
  LINIA *L;
  LUK *l;
  T_Point *P;
  NAGLOWEK *nag;
  OKRAG *K;
  char *adp, *adp1, *adp2, *adp3, *adp4, *adp5, *adk, *adk1, *adk2, *adp01, *adp02;
  int i, param1;
  int i_f_handle ;
  char *bp1;
  int l_kr;
  BOOL sorted;
  char st[30];
  BOOL find_next;
  double x_vertex, y_vertex;
  double len1, len2;
  double l_x1, l_x2, l_y1, l_y2;
  BOOL net_exist;
  BOOL exit_find;

  strcpy(point_net_opis,"");

  //ewentualny skok na koniec linii
  nag=(NAGLOWEK *)adr;
  if (nag->obiekt==Olinia)
   {
     L=(LINIA *)adr;
//     CUR_OFF(X,Y);
     len1=(L->x1-X)*(L->x1-X)+(L->y1-Y)*(L->y1-Y);
     len2=(L->x2-X)*(L->x2-X)+(L->y2-Y)*(L->y2-Y);
     if (len2<len1)
      {
        X=L->x2;
        Y=L->y2;
      }
       else
         {
          X=L->x1;
          Y=L->y1;
         }

   }
    else if (nag->obiekt==Oluk)
     {
//       CUR_OFF(X,Y);
       l=(LUK *)adr;
       l_x1=l->x+l->r*cosf(l->kat1);
       l_y1=l->y+l->r*sinf(l->kat1);
       l_x2=l->x+l->r*cosf(l->kat2);
       l_y2=l->y+l->r*sinf(l->kat2);
       len1=(l_x1-X)*(l_x1-X)+(l_y1-Y)*(l_y1-Y);
       len2=(l_x2-X)*(l_x2-X)+(l_y2-Y)*(l_y2-Y);
       if (len2<len1)
        {
          X=l_x2;
          Y=l_y2;
        }
         else
          {
            X=l_x1;
            Y=l_y1;
           }
     }
     else if (nag->obiekt==Okolo)
      {
       K=(OKRAG *)adr;
       X=K->x;
       Y=K->y;
      }
      else  //inny obiekt
       {
         return 0;
       }

//  CUR_OFF(X,Y);
  net_exist=FALSE;
  point_net_typ=100;

  if (NULL != (ptrs_blok=FIRSTB((char*)adr)))
   {
     if (ptrs_blok->dlugosc_opisu_obiektu>1)
      {
       ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_blok->opis_obiektu [0]) ;
       strcpy (st, &ptrs_desc_bl->sz_type [0]) ;
       if ((strncmp(st,"*#",2)==0) || (strncmp(st,"*^",2)==0)) //znaleziono blok sieci
        {
          //to jest blok sieci
          if (strcmp(st,"*#Pz")==0) point_net_typ=0;
          else if (strcmp(st,"*#Cok")==0) point_net_typ=1;
          else if (strcmp(st,"*#Co")==0) point_net_typ=2;
          else if (strcmp(st,"*#T")==0) point_net_typ=3;
          else if (strcmp(st,"*#Tk")==0) point_net_typ=4;
          else if (strcmp(st,"*#e")==0) point_net_typ=5;
          else if (strcmp(st,"*#E")==0) point_net_typ=6;
          else if (strcmp(st,"*#S")==0) point_net_typ=7;
          else if (strcmp(st,"*#D")==0) point_net_typ=8;
          else if (strcmp(st,"*#W")==0) point_net_typ=9;
          else if (strcmp(st,"*^Pz")==0) point_net_typ=0;
          else if (strcmp(st,"*^Cok")==0) point_net_typ=1;
          else if (strcmp(st,"*^Co")==0) point_net_typ=2;
          else if (strcmp(st,"*^T")==0) point_net_typ=3;
          else if (strcmp(st,"*^Tk")==0) point_net_typ=4;
          else if (strcmp(st,"*^e")==0) point_net_typ=5;
          else if (strcmp(st,"*^E")==0) point_net_typ=6;
          else if (strcmp(st,"*^S")==0) point_net_typ=7;
          else if (strcmp(st,"*^D")==0) point_net_typ=8;
          else if (strcmp(st,"*^W")==0) point_net_typ=9;
          else point_net_typ=100;
          //teraz trzeba odszukac nazwe wezla
          //nalezy wiec odszukac punkt sieci najblizszy kursora
          //a nastepnie przypisac do niego opis wezla
         net_exist=TRUE;
        }
      }
   }
   if (net_exist==FALSE)
   {
    if (NULL != (ptrs_blok=LASTB((char*)adr)))  //jak nie ostatni blok to pierwszy
       {
         if (ptrs_blok->dlugosc_opisu_obiektu>1)
          {
           ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_blok->opis_obiektu [0]) ;
           strncpy (st, &ptrs_desc_bl->sz_type [0], 30) ;
           if ((strncmp(st,"*#",2)==0) || (strncmp(st,"*^",2)==0)) //znaleziono blok sieci
            {
             //to jest blok sieci
             //przyporzadkowanie typu sieci
             ///////////////////
             if (strcmp(st,"*#Pz")==0) point_net_typ=0;
             else if (strcmp(st,"*#Cok")==0) point_net_typ=1;
             else if (strcmp(st,"*#Co")==0) point_net_typ=2;
             else if (strcmp(st,"*#T")==0) point_net_typ=3;
             else if (strcmp(st,"*#Tk")==0) point_net_typ=4;
             else if (strcmp(st,"*#e")==0) point_net_typ=5;
             else if (strcmp(st,"*#E")==0) point_net_typ=6;
             else if (strcmp(st,"*#S")==0) point_net_typ=7;
             else if (strcmp(st,"*#D")==0) point_net_typ=8;
             else if (strcmp(st,"*#W")==0) point_net_typ=9;
             else if (strcmp(st,"*^Pz")==0) point_net_typ=0;
             else if (strcmp(st,"*^Cok")==0) point_net_typ=1;
             else if (strcmp(st,"*^Co")==0) point_net_typ=2;
             else if (strcmp(st,"*^T")==0) point_net_typ=3;
             else if (strcmp(st,"*^Tk")==0) point_net_typ=4;
             else if (strcmp(st,"*^e")==0) point_net_typ=5;
             else if (strcmp(st,"*^E")==0) point_net_typ=6;
             else if (strcmp(st,"*^S")==0) point_net_typ=7;
             else if (strcmp(st,"*^D")==0) point_net_typ=8;
             else if (strcmp(st,"*^W")==0) point_net_typ=9;
             else point_net_typ=100;

             net_exist=TRUE;
            }
          }
       }
   }
      if (net_exist==TRUE)
       {
          adp01=dane;
          adp02=dane+dane_size;

          find_next=TRUE;
          exit_find=FALSE;

          while ((adp01<adp02) && (find_next==TRUE) && (exit_find==FALSE))
          {
           adp3=find_block(adp01, adp02, B_VERTEX, "*");  //!!! not "*~"  ???
           if (adp3!=NULL)
            {
              b1=(BLOK *)adp3;
              ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu [0]) ;
              x_vertex = ptrs_desc_bl1->x ;
              y_vertex = ptrs_desc_bl1->y ;
              adp1=adp3 + sizeof(NAGLOWEK)+B3+b1->dlugosc_opisu_obiektu;
              adp2=adp3 + sizeof(NAGLOWEK) + b1->n - 1;
              adp5=find_obj(adp1, adp2, Opoint, ONieOkreslony, 0);
              if (adp5!=NULL)
                {
                  P=(T_Point *)adp5;
                  x_vertex=P->x;
                  y_vertex=P->y;
                }
                else
                 {
                  adp5=find_obj(adp1, adp2, Okolo, ONieOkreslony, 0);
                  if (adp5!=NULL)
                   {
                     K=(OKRAG *)adp5;
                     x_vertex=K->x;
                     y_vertex=K->y;
                   }
                 }
              if ((Check_if_Equal(x_vertex,X)==TRUE) && (Check_if_Equal(y_vertex,Y)==TRUE))
               {
                adp4=find_obj(adp1, adp2, Otekst, n_typ_normal, 0);
                if (adp4!=NULL) //znaleziono tekst opisu
                 {
                   T=(TEXT *)adp4;
                   //przyporzadkowanie tekstu do wiezcholka polilinii
                   strncpy(point_net_opis,T->text,30);
                   //przyporzadkowanie typu sieci
                   ///////////////////
                   find_next=FALSE;
                   exit_find=TRUE;
                   return 1;
                 }
               }
              adp01 = adp3 + sizeof(NAGLOWEK) + b1->n;
            }
             else exit_find=TRUE;
          }

        if (find_next==TRUE)
         {
          adp01=dane;
          adp02=dane+dane_size;
          exit_find=FALSE;

          while ((adp01<adp02) && (find_next==TRUE) && (exit_find==FALSE))
          {
           adp3=find_block_sub_on(adp01, adp02, B_NIEOKRESLONY, "*~",2);
           if (adp3!=NULL)
            {
              b1=(BLOK *)adp3;
              ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu [0]) ;
              x_vertex = ptrs_desc_bl1->x ;
              y_vertex = ptrs_desc_bl1->y ;
              adp1=adp3 + sizeof(NAGLOWEK)+B3+b1->dlugosc_opisu_obiektu;
              adp2=adp3 + sizeof(NAGLOWEK) + b1->n - 1;
              adp5=find_obj(adp1, adp2, Opoint, ONieOkreslony, 0);
              if (adp5!=NULL)
                {
                  P=(T_Point *)adp5;
                  x_vertex=P->x;
                  y_vertex=P->y;
                }
              if ((Check_if_Equal(x_vertex,X)==TRUE) && (Check_if_Equal(y_vertex,Y)==TRUE))
               {
                adp4=find_obj(adp1, adp2, Otekst, n_typ_normal, 0);
                if (adp4!=NULL) //znaleziono tekst opisu
                 {
                   T=(TEXT *)adp4;
                   //przyporzadkowanie tekstu do wiezcholka polilinii
                   strncpy(point_net_opis,T->text,30);
                   //przyporzadkowanie typu sieci
                   ///////////////////
                   find_next=FALSE;
                   exit_find=TRUE;
                   return 1;
                 }
               }
              adp01 = adp3 + sizeof(NAGLOWEK) + b1->n;
            }
             else exit_find=TRUE;
          }
         }
       }
  return 0;   //nie znaleziono
}

//extern "C" { extern int get_pomiar_param(char  **adr); }
int get_pomiar_param(char  **adr)
{ BLOK *b, *ptrs_blok, *ptrs_blok1;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  TEXT *T;
  LINIA *L;
  T_Point *P;
  char *adp, *adp1, *adp2, *adp3, *adp4, *adk, *adk1, *adk2;
  int i, param1;
  int i_f_handle ;
  double odleglosc, odleglosc0, yi;
  double first_hektometr, skala_profilu, local_x, local_y, poziom_pp, typ_profilu ;
  double srednica, material;
  char *srednica_t;
  char srednica_tt[60];
  char *bp1;
  int l_kr;
  BOOL sorted;
  char st[64];  //30
  double odleglosc_odl, rzedna_terenu_p, rzedna_terenu_i;
  char *bp;
 

  first_hektometr=get_hektometr_p();
  skala_profilu=get_skala_profilu_x();

  local_x=get_localx();
  local_y=get_localy();
  typ_profilu=get_typ_profilu();
  poziom_pp=get_poziom_pp();
  tablica_0_1(typ_profilu);

  if (NULL != (ptrs_blok=FIRSTB((char*)adr)))
   {
     if ((ptrs_blok->kod_obiektu==B_INSTALACJE_OPIS) || (ptrs_blok->kod_obiektu==B_INSTALACJE_K))
      {
        if (NULL != (ptrs_blok1=LASTB((char*)adr)))
         {
           ptrs_blok=ptrs_blok1;
         }
      }
     else if ((ptrs_blok->kod_obiektu==B_ODWIERT) || (ptrs_blok->kod_obiektu==B_ODWIERT_OLD))
     {
         if (NULL != (ptrs_blok1=LASTB((char*)adr)))
         {
             ptrs_blok=ptrs_blok1;
         }
     }
     //znaleziono blok
     dane_profil.n=0;
     for (i=0; i<24; i++) dane_profil.var[i]=0;
     dane_profil.param1=0;
     param1 = 0;
//     param1=2147483647;

     odleglosc0=odleglosc=0.;
     adp=(char *)ptrs_blok;
     adk=adp + ptrs_blok->n;
     adp1=find_obj(adp, adk, Otekst, n_odleglosc, 1);
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
       dane_profil.var[0]=strtod(decimal_dot(T->text), &bp);
       //odczytanie nastepujacej linii
       adp2=adp1+ T->n + sizeof(NAGLOWEK) ;
       L = (LINIA*)adp2;
       if (L->obiekt==Olinia)
        {
         odleglosc0 = L->x1;
         odleglosc = (milimetryob(L->x1-local_x)*skala_profilu) + (first_hektometr * 100);
         dane_profil.var[0]=odleglosc;
         param1 |= 1;
        }
       data_instal_last[0]=dane_profil.var[0];
       //poszukiwanie kolejno wszystkich instalacji
       //poszukiwanie bloku *Ks
       strcpy(st,"*Ks");
       adp2=find_block(adp, adk, B_INSTALACJE_K, st);
       if (adp2!=NULL)
        {
          b=(BLOK *)adp2;
          adk2=adp2+sizeof(NAGLOWEK)+b->n;
          //poszukiwanie rzednej Ks
          adp3=find_obj(adp2, adk2, Otekst, n_rzedna_kanalu_i, 1);
          if (adp3!=NULL)
           {
            T=(TEXT *)adp3;
            data_instal_last[1]=strtod(decimal_dot(T->text), &bp);
            //poszukiwanie srednicy Ks
            adp4=find_obj(adp2, adk2, Otekst, n_srednica_kanalu_i, 1);
            if (adp4!=NULL)
             {
              T=(TEXT *)adp4;
              data_instal_last[2]=get_srednice(T->text);
             }
           }
        }

       //poszukiwanie bloku *Ko
       strcpy(st,"*Ko");
       adp2=find_block(adp, adk, B_INSTALACJE_K, st);
       if (adp2!=NULL)
        {
          b=(BLOK *)adp2;
          adk2=adp2+sizeof(NAGLOWEK)+b->n;
          //poszukiwanie rzednej Ko
          adp3=find_obj(adp2, adk2, Otekst, n_rzedna_kanalu_i, 1);
          if (adp3!=NULL)
           {
            T=(TEXT *)adp3;
            data_instal_last[3]=strtod(decimal_dot(T->text), &bp);
            //poszukiwanie srednicy Ko
            adp4=find_obj(adp2, adk2, Otekst, n_srednica_kanalu_i, 1);
            if (adp4!=NULL)
             {
              T=(TEXT *)adp4;
              data_instal_last[4]=get_srednice(decimal_dot(T->text));
             }
           }
        }
       //poszukiwanie bloku *Kd
       strcpy(st,"*Kd");
       adp2=find_block(adp, adk, B_INSTALACJE_K, st);
       if (adp2!=NULL)
        {
          b=(BLOK *)adp2;
          adk2=adp2+sizeof(NAGLOWEK)+b->n;
          //poszukiwanie rzednej Kd
          adp3=find_obj(adp2, adk2, Otekst, n_rzedna_kanalu_i, 1);
          if (adp3!=NULL)
           {
            T=(TEXT *)adp3;
            data_instal_last[5]=strtod(decimal_dot(T->text), &bp);
            //poszukiwanie srednicy Kd
            adp4=find_obj(adp2, adk2, Otekst, n_srednica_kanalu_i, 1);
            if (adp4!=NULL)
             {
              T=(TEXT *)adp4;
              data_instal_last[6]=get_srednice(decimal_dot(T->text));
             }
           }
        }
       //poszukiwanie bloku *W
       strcpy(st,"*W");
       adp2=find_block(adp, adk, B_INSTALACJE_K, st);
       if (adp2!=NULL)
        {
          b=(BLOK *)adp2;
          adk2=adp2+sizeof(NAGLOWEK)+b->n;
          //poszukiwanie rzednej W
          adp3=find_obj(adp2, adk2, Otekst, n_rzedna_kanalu_i, 1);
          if (adp3!=NULL)
           {
            T=(TEXT *)adp3;
            data_instal_last[7]=strtod(decimal_dot(T->text), &bp);
            //poszukiwanie srednicy W
            adp4=find_obj(adp2, adk2, Otekst, n_srednica_kanalu_i, 1);
            if (adp4!=NULL)
             {
              T=(TEXT *)adp4;
              data_instal_last[8]=get_srednice(decimal_dot(T->text));
             }
           }
        }
       //poszukiwanie bloku *G
       strcpy(st,"*G");
       adp2=find_block(adp, adk, B_INSTALACJE_K, st);
       if (adp2!=NULL)
        {
          b=(BLOK *)adp2;
          adk2=adp2+sizeof(NAGLOWEK)+b->n;
          //poszukiwanie rzednej G
          adp3=find_obj(adp2, adk2, Otekst, n_rzedna_kanalu_i, 1);
          if (adp3!=NULL)
           {
            T=(TEXT *)adp3;
            data_instal_last[9]=strtod(decimal_dot(T->text), &bp);
            //poszukiwanie srednicy G
            adp4=find_obj(adp2, adk2, Otekst, n_srednica_kanalu_i, 1);
            if (adp4!=NULL)
             {
              T=(TEXT *)adp4;
              data_instal_last[10]=get_srednice(decimal_dot(T->text));
             }
           }
        }
       //poszukiwanie bloku *E
       strcpy(st,"*E");
       adp2=find_block(adp, adk, B_INSTALACJE_K, st);
       if (adp2!=NULL)
        {
          b=(BLOK *)adp2;
          adk2=adp2+sizeof(NAGLOWEK)+b->n;
          //poszukiwanie rzednej E
          adp3=find_obj(adp2, adk2, Otekst, n_rzedna_kanalu_i, 1);
          if (adp3!=NULL)
           {
            T=(TEXT *)adp3;
            data_instal_last[11]=strtod(decimal_dot(T->text), &bp);
           }
        }
       //poszukiwanie bloku *kT
       strcpy(st,"*kT");
       adp2=find_block(adp, adk, B_INSTALACJE_K, st);
       if (adp2!=NULL)
        {
          b=(BLOK *)adp2;
          adk2=adp2+sizeof(NAGLOWEK)+b->n;
          //poszukiwanie rzednej kT
          adp3=find_obj(adp2, adk2, Otekst, n_rzedna_kanalu_i, 1);
          if (adp3!=NULL)
           {
            T=(TEXT *)adp3;
            data_instal_last[12]=strtod(decimal_dot(T->text), &bp);
           }
        }
       //poszukiwanie bloku *T
       strcpy(st,"*T");
       adp2=find_block(adp, adk, B_INSTALACJE_K, st);
       if (adp2!=NULL)
        {
          b=(BLOK *)adp2;
          adk2=adp2+sizeof(NAGLOWEK)+b->n;
          //poszukiwanie rzednej T
          adp3=find_obj(adp2, adk2, Otekst, n_rzedna_kanalu_i, 1);
          if (adp3!=NULL)
           {
            T=(TEXT *)adp3;
            data_instal_last[13]=strtod(decimal_dot(T->text), &bp);
           }
        }
       //interpolacja rzednej terenu
       odleglosc_odl=(jednostkiOb(odleglosc-(first_hektometr*100))/skala_profilu);
       l_kr=find_interpolated_tp(odleglosc_odl+local_x, &yi);
       if (l_kr==0)
        {
         rzedna_terenu_p=0;
        }
         else rzedna_terenu_p=jednostkiOb(milimetryob(yi - local_y - tablica_y[14]) + poziom_pp);
       l_kr=find_interpolated_ti(odleglosc_odl+local_x, &yi);
       if (l_kr==0)
        {
         rzedna_terenu_i=0;
        }
         else rzedna_terenu_i=jednostkiOb(milimetryob(yi - local_y - tablica_y[14]) + poziom_pp);

       set_new_instal_tab(rzedna_terenu_p, rzedna_terenu_i);
       //////////////////
      }
       else  //nie znaleziono odleglosci
        {
          //sprawdzenie czy jest to blok odwiertu
          if ((ptrs_blok->kod_obiektu==B_ODWIERT) || (ptrs_blok->kod_obiektu==B_ODWIERT_OLD))
           {
             if (ptrs_blok->dlugosc_opisu_obiektu>1)
               {
                 ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_blok->opis_obiektu [0]) ;
                 odleglosc0 = ptrs_desc_bl->x;
                 odleglosc = (milimetryob(ptrs_desc_bl->x-local_x)*skala_profilu) + (first_hektometr * 100);
                 dane_profil.var[0]=odleglosc;
                 data_odwiert[0]=odleglosc;
                 param1 |= 1;
               }
           }
        }

      adp1=find_obj(adp, adk, Otekst, n_rzedna_kanalu_i, 1);
      if (adp1 != NULL)
       {
        T=(TEXT *)adp1;
        dane_profil.var[1]=strtod(decimal_dot(T->text), &bp);
        param1 |= 2;
       }

     adp1=find_obj(adp, adk, Otekst, n_rzedna_kanalu_p, 1);
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
       dane_profil.var[2]=strtod(decimal_dot(T->text), &bp);
       param1 |= 4;
      }

     sorted=FALSE;

     adp1=find_obj(adp, adk, Otekst, n_rzedna_ulicy, 1);  //teren projektowany
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
       dane_profil.var[3]=strtod(decimal_dot(T->text), &bp);
       param1 |= 8;
      }
       else
        {
         if (odleglosc0 > 0)
         {
          l_kr=sortowanie_odl();  //dla potrzeb interpolacji zaglebienia rury
                                  //w przekrojach "tylko rura"
          sorted=TRUE;
          //szukanie wartosci interpolowanej terenu projektowanego
          if ((ptrs_blok->kod_obiektu==B_ODWIERT) ||
              (ptrs_blok->kod_obiektu==B_ODWIERT_OLD) ||
              (ptrs_blok->kod_obiektu==B_INSTALACJE))
               {
                l_kr=find_interpolated_io(odleglosc0, 1, &yi);   //teren istniejacy
              //  if (l_kr==0) l_kr=find_interpolated_io(odleglosc0, 0, &yi); //teren projektowany zignorowano
               }
                else
                 {
                  l_kr=find_interpolated(odleglosc0, 0, &yi);   //teren projektowany
                  if (l_kr==0) l_kr=find_interpolated(odleglosc0, 1, &yi); //teren istniejacy
                 }
             if (l_kr==1)
              {
               dane_profil.var[3]=milimetryob(yi - local_y - tablica_y[14]) + poziom_pp;
               param1 |= 8;
              }
         }
        }

     adp1=find_obj(adp, adk, Otekst, n_rzedna_terenu, 1); //teren istniejacy
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
       dane_profil.var[4]=strtod(decimal_dot(T->text), &bp);
       param1 |= 16;
      }
       else
        {
         if (odleglosc0 > 0)
         {
          if (sorted==FALSE) l_kr=sortowanie_odl();  //dla potrzeb interpolacji zaglebienia rury
                                  //w przekrojach "tylko rura"
          sorted=TRUE;
          //szukanie wartosci interpolowanej terenu istniejacego
          if ((ptrs_blok->kod_obiektu==B_ODWIERT) ||
              (ptrs_blok->kod_obiektu==B_ODWIERT_OLD) ||
              (ptrs_blok->kod_obiektu==B_INSTALACJE))
               {
                 l_kr=find_interpolated_io(odleglosc0, 1, &yi); //teren istniejacy
               }
                else
                 {
                   l_kr=find_interpolated(odleglosc0, 1, &yi); //teren istniejacy
                 }
          if (l_kr==1)
           {
             dane_profil.var[4]=milimetryob(yi - local_y - tablica_y[14]) + poziom_pp;
             param1 |= 16;
           }
         }
        }

     adp1=find_obj(adp, adk, Otekst, n_srednica_kanalu_i, 1);
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
       srednica_t = strpbrk(T->text, "-1234567890.,");
       if (srednica_t != NULL)
         {
          strcpy(srednica_tt, decimal_dot(srednica_t));
          srednica=0;
          srednica = strtod(srednica_tt, &bp1);
          if (bp1!=srednica_tt && errno != ERANGE)
           {
               ;
           }
          else srednica=0.;
         }
       dane_profil.var[5]=srednica;
       param1 |= 32;
       //odszukanie materialu
       //char *material[]={"","beton","kamionka","żeliwo","PCV","tworzywo sztuczne","stal","PE",""};

       material=0;
       if (strstr(T->text,"beton") != NULL) material=1;
        else if (strstr(T->text,"kamionka") !=NULL) material=2;
         else if (strstr(T->text,"żeliwo") != NULL) material=3;
          else if (strstr(T->text,"PCV") !=NULL) material=4;
           else if (strstr(T->text,"worzywo sztuczne") !=NULL) material=5;
            else if (strstr(T->text,"stal") !=NULL) material=6;
             else if (strstr(T->text,"PE") !=NULL) material=7;

       dane_profil.var[6]=material;
       param1 |= 64;
      }

     adp1=find_obj(adp, adk, Otekst, n_srednica_kanalu_p, 1);
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
       T=(TEXT *)adp1;
       srednica_t = strpbrk(T->text, "-1234567890.,");
       if (srednica_t != NULL)
         {
          strcpy(srednica_tt, decimal_dot(srednica_t));
          srednica=0;
          srednica = strtod(srednica_tt, &bp1);
          if (bp1!=srednica_tt && errno != ERANGE)
           {
               ;
           }
          else srednica=0.;
         }

       dane_profil.var[7]=srednica;
       param1 |= 128;

       //char *material[]={"","beton","kamionka","żeliwo","PCV","tworzywo sztuczne","stal","PE",""};

       material=0;
       if (strstr(T->text,"beton") !=NULL) material=1;
        else if (strstr(T->text,"kamionka") !=NULL) material=2;
         else if (strstr(T->text,"żeliwo") !=NULL) material=3;
          else if (strstr(T->text,"PCV") !=NULL) material=4;
           else if (strstr(T->text,"worzywo sztuczne") !=NULL) material=5;
            else if (strstr(T->text,"stal") !=NULL) material=6;
             else if (strstr(T->text,"PE") !=NULL) material=7;

       dane_profil.var[8]=material;
       param1 |= 256;
      }

//     adp1=find_obj(adp, adk, Otekst, n_spadek_kanalu_i, 1);
//     if (adp1 != NULL)
//      {
//       T=(TEXT *)adp1;
//       dane_profil.var[9]=strtod(decimal_dot(T->text), &bp);
//       param1 |= 512;
//      }

//     adp1=find_obj(adp, adk, Otekst, n_dlugosc_kanalu_i, 1);
//     if (adp1 != NULL)
//      {
//       T=(TEXT *)adp1;
//       dane_profil.var[10]=strtod(decimal_dot(T->text), &bp);
//       param1 |= 1024;
//      }

//     adp1=find_obj(adp, adk, Otekst, n_spadek_kanalu_p, 1);
//     if (adp1 != NULL)
//      {
//       T=(TEXT *)adp1;
//       dane_profil.var[11]=strtod(decimal_dot(T->text), &bp);
//       param1 |= 2048;
//      }

//     adp1=find_obj(adp, adk, Otekst, n_dlugosc_kanalu_p, 1);
//     if (adp1 != NULL)
//      {
//       T=(TEXT *)adp1;
//       dane_profil.var[12]=strtod(decimal_dot(T->text), &bp);
//       param1 |= 4096;
//      }

     adp1=find_obj(adp, adk, Otekst, n_zaglebienie_kanalu_i, 1);
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
//       dane_profil.var[13]=strtod(decimal_dot(T->text), &bp);
//       param1 |= 8192;
       if (T->obiektt3==O3Teren_p)
        {
          dane_profil.var[15]=1;
          param1 |= 32768;
        }
      }

     adp1=find_obj(adp, adk, Otekst, n_zaglebienie_kanalu_p, 1);
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
//       dane_profil.var[14]=strtod(decimal_dot(T->text), &bp);
//       param1 |= 16384;
       if (T->obiektt3==O3Teren_i)
        {
          dane_profil.var[16]=1;
          param1 |= 65536;
        }
      }

//   15   32768
//   16   65536

     //dodatkowe poszukiwanie tekstow swobodnych
     adp1=find_obj(adp, adk, Otekst, n_typ_normal, 1);
     if (adp1 != NULL)
      {
       T=(TEXT *)adp1;
       //sprawdzanie, czy zawiera jeden z tekstow
//       "ziemna","asfaltowa","betonowa","kostka gran.","brukowa"
       material=0;
       if (strstr(T->text,_ziemna_) !=NULL) material=1;
        else if (strstr(T->text,_asfaltowa_) !=NULL) material=2;
         else if (strstr(T->text,_betonowa_) !=NULL) material=3;
          else if (strstr(T->text,_kostka_) !=NULL) material=4;
           else if (strstr(T->text,_brukowa_) !=NULL) material=5;
            else //poszkiwanie kolejnego tekstu
             {
               adp2=find_obj(adp1+T->n, adk, Otekst, n_typ_normal, 1);
               if (adp2 != NULL)
                {
                  T=(TEXT *)adp2;
                  //sprawdzanie, czy zawiera jeden z tekstow
                  //"ziemna","asfaltowa","betonowa","kostka gran.","brukowa"
                  material=0;
                  if (strstr(T->text,_ziemna_) !=NULL) material=1;
                  else if (strstr(T->text,_asfaltowa_) !=NULL) material=2;
                   else if (strstr(T->text,_betonowa_) !=NULL) material=3;
                    else if (strstr(T->text,_kostka_) !=NULL) material=4;
                     else if (strstr(T->text,_brukowa_) !=NULL) material=5;
                }
             }
       dane_profil.var[17]=material;
       param1 |= 131072;
      }

     dane_profil.param1 = param1;

     //if this is not borehole or installation measure, we replace the last measure of profile
      if ((ptrs_blok->kod_obiektu==B_PPOMIAR) ||
          (ptrs_blok->kod_obiektu==B_KPOMIAR))
          memmove(&dane_profs1, &dane_profil, sizeof(MyDane));

      //extra stuff for ODWIERT
      //szukanie wartosci interpolowanej terenu istniejacego
      if ((ptrs_blok->kod_obiektu==B_ODWIERT) || (ptrs_blok->kod_obiektu==B_ODWIERT_OLD))
      {   NAGLOWEK *nag;
          LINIA *Lw;
          nag = (NAGLOWEK*)adp;
          strcpy(comments_geo[0],"");
          int vari=2;
          int vard=6;
          while (nag->obiekt != Okoniec && adp <= adk)
          {
              if (nag->obiekt == OdBLOK)
              {
                  b=(BLOK *)adp;
                  adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
              }
              else if (nag->obiekt == Otekst)
              {
                  T=(TEXT*)adp;
                  if (T->obiektt2==2)  //all of them
                  {
                      switch (T->typ)
                      {
                      case 0:  //numer odwiertu geologicznego / borehole number
                          strncpy(comments_geo[1], T->text, 60);
                          break;
                      case 7: //rzędna terenu / terrain elevation
                          data_odwiert[1]=strtod(decimal_dot(T->text), &bp1);  //dane_prof.rki
                          break;
                      case 8: //zwierciadło wody ustabilizowane / stabilized water table,
                          data_odwiert[2]=strtod(decimal_dot(T->text), &bp1);  //dane_prof.rkp
                          break;
                      case 9: //zwierciadło wody napięte / constrained water table
                          data_odwiert[3]=strtod(decimal_dot(T->text), &bp1);  //dane_prof.ru
                          break;
                      case 10: //zwierciadło wody swobodne / free water table,
                          data_odwiert[4]=strtod(decimal_dot(T->text), &bp1);  //dane_prof.rt
                          break;
                      case 11: //sączenie wody / water seepage
                          data_odwiert[5]=strtod(decimal_dot(T->text), &bp1);  //dane_prof.si1
                          break;
                      case 12: //opis kolejnej warstwy / description of the next layer
                          strncpy(comments_geo[vari], T->text, 60);
                          //taking the position of last line
                          double level=milimetryob(Lw->y1-get_localy()-tablica_y[14]);
                          level+=get_poziom_pp();
                          data_odwiert[vard]=round((data_odwiert[1]-level) / rzedna_ti_prec) * rzedna_ti_prec;
                          vard++;
                          vari++;
                          break;
                      default:
                          break;
                      }
                  }
                  adp+=sizeof(NAGLOWEK)+nag->n;
              }
              else if (nag->obiekt == Olinia)  //to find soil layer thickness
              {
                  Lw=(LINIA*)adp;
                  //searching for horizontal lines will be done after each text comment
                  adp+=sizeof(NAGLOWEK)+nag->n;
              }
              else  adp+=sizeof(NAGLOWEK)+nag->n;
              nag = (NAGLOWEK*)adp;
            }
      }

#ifdef SAVETMP
     /* tutaj najpierw nalezy zachowac zbior poprzedni w zbiorze PROFIL_TEMP_FILE_BAK*/
     /* a nastepnie zapisac nowe dane */
     if (rename(PROFIL_TEMP_FILE,PROFIL_TEMP_FILE_BAK)!=0)
      {
       remove(PROFIL_TEMP_FILE_BAK);
       rename(PROFIL_TEMP_FILE,PROFIL_TEMP_FILE_BAK);
      }

#ifndef LINUX
      i_f_handle=open(PROFIL_TEMP_FILE, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
      i_f_handle=open(PROFIL_TEMP_FILE, O_CREAT|O_TRUNC|O_RDWR, 0666);
#endif

     if (i_f_handle == -1)
      {
        ErrList(103);
        return 0;
      }
  
     if (write (i_f_handle, &dane_profil, sizeof(dane_profil)) != sizeof(dane_profil))
      {
       close (i_f_handle) ;
       ErrList(104);
       return 0;
      }

      close (i_f_handle) ;
      return 0;
#endif

   }
  return 0;
}



static int Profil1HD(void *BufVar)              /*KOLEJNY POMIAR - WARIANT DYNAMICZNY*/
{
  const double ht1=2.5; /*2.25*/ /* opisy pionowe */
  const double ht2=2.5;  /* opisy poziome */
  const double ht3=3.5;
  const double dt_y=0.8;
  char *material[]={"",_beton_,_kamionka_,_zeliwo_,_PVC_,_plastic_,_steel_,_PE_,""};
  MyDane *dane_prof0=(MyDane*)BufVar;
  int i_f_handle ;

  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  int l_kr;
  double del_odl;

  double rpp;
  double max_r;

  double epsilon;
  double epsilon1;
  double suma_odl;

  MyDaneL punkt_i[6];
  MyDaneL punkt_p[6];

  int l_srednic_i, l_srednic_p, l_spadkow_i, l_spadkow_p;
  int l_punktow_i, l_punktow_p;

  double add_odl;
  double delta_odl;
  double war_odl;
  double sri_max;
  double srp_max;
  double rk_max;
  int i, si2i, oi1i;

  int odl_i;
  double odl_1;
  double odl_r;

  double reszta;
  int reszta_i;
  double odl_0;
  int l_h, l_k;
  double spadek_ki;
  double dlugosc_ki;
  double spadek_kp;
  double dlugosc_kp;

  double dane_prof_odl_ki;
  double dane_prof_odl_kp;
  double dane_prof0_odl_ki;
  double dane_prof0_odl_kp;

  BOOL linia_ki, linia_kp;
  int len_t1, len_t2, ii;
  int dno_os_i, dno_os_p;
  
  double zaglebienie_ki;
  double zaglebienie_kp;
  
  int op3;
  double local_y_;

  TEXT textg=Tdef;
  double rzedna_tp;
  BOOL zaglebienie_ok;
  BOOL ru_avail;
  int O2Block;
  
//  char str1[80];
//  int dane_oi1;

  /***************************/

 /*beda potrzebne dodatkowe zmienne lokalne*/
 
  for (i=0; i<5; i++)
   {
     punkt_i[i].odl=0;
     punkt_p[i].odl=0;
   }  

  dane_global.oi2=0;  /*dynamika bloku*/
  l_kr=put_dynamic_block(dane_global.oi2);


  dane_global.ru=get_skala_profilu_x();
  dane_global.rt=get_poziom_pp();
  dane_global.odl=get_typ_profilu();
  tablica_0_1(dane_global.odl);
  /*sprawdzenie czy skala nie jest 0*/
  
  if (dane_global.ru==0)
  {
  ErrList(111);
  return 0;
  }

#ifdef SAVETMP
  //odczytanie danych z pliku binarnego
  i_f_handle = open (PROFIL_TEMP_FILE, O_BINARY | O_RDONLY, S_IREAD) ;
  if (i_f_handle == -1)
   {
   //zerowanie danych dane_profs1
  // dane_profs1=dane_profs0;
   memmove(&dane_profs1, &dane_profs0, sizeof(MyDane));
   }
    else
    {
     if (read (i_f_handle, &dane_profs1, sizeof(dane_profs1)) != sizeof(dane_profs1))
      {
      //zerowanie danych dane_profs1
     // dane_profs1=dane_profs0;
      memmove(&dane_profs1, &dane_profs0, sizeof(MyDane));
      }
     close (i_f_handle) ;
    }

#endif

  epsilon = 0.01;       /*tolerancja dla odleglosci w [m]*/
  epsilon1 = 1;      /*tolerancja dla rzednych w promilach*/

  rpp=jednostkiOb(dane_global.rt);

  dane_global.rki=get_hektometr_p();
  dane_global.rkp=get_hektometr_k();
  
  dane_prof0->param1=get_param1(dane_prof0);


  dane_prof.odl=(jednostkiOb(dane_prof0->odl-(dane_global.rki*100))/dane_global.ru);
  dane_prof_odl_ki=0;
  dane_prof_odl_kp=0;
  dane_prof0_odl_ki=0;
  dane_prof0_odl_kp=0;

  //jezeli podano rzedne terenu istniejacego (rt) a ru==0,
  //to przyjmujemy rt=ru;
  ru_avail=TRUE;
  if ((dane_prof0->rt>0) && (dane_prof0->ru==0))
   {
    ru_avail=FALSE;;
    dane_prof0->ru=dane_prof0->rt;
   }


  dane_prof.rki=jednostkiOb(dane_prof0->rki);
  dane_prof.rkp=jednostkiOb(dane_prof0->rkp);
  dane_prof.ru=jednostkiOb(dane_prof0->ru);
  dane_prof.rt=jednostkiOb(dane_prof0->rt);

  /* poprzednie dane */
  dane_profs.odl=(jednostkiOb(dane_profs1.odl-(dane_global.rki*100))/dane_global.ru);
  dane_profs.rki=jednostkiOb(dane_profs1.rki);
  dane_profs.rkp=jednostkiOb(dane_profs1.rkp);
  dane_profs.ru=jednostkiOb(dane_profs1.ru);
  dane_profs.rt=jednostkiOb(dane_profs1.rt);

  dane_profs.spi3=(jednostkiOb(dane_profs1.spi3-(dane_global.rki*100))/dane_global.ru);  /* dla rki */
  dane_profs.opi1=(jednostkiOb(dane_profs1.opi1-(dane_global.rki*100))/dane_global.ru);  /* dla rkp */
  dane_profs.opi2=(jednostkiOb(dane_profs1.opi2-(dane_global.rki*100))/dane_global.ru);  /* dla ru */
  dane_profs.opi3=(jednostkiOb(dane_profs1.opi3-(dane_global.rki*100))/dane_global.ru);  /* dla rt */
//  dane_profs.spp1=dane_profs1.spp1;  /* rzedna tabelki */

  /*   */
  dno_os_i=0;
  dno_os_p=0;
  
  dane_prof.si1=jednostkiOb(dane_prof0->si1);   /*srednice k.i.*/
  if (dane_prof0->si1>10)
   {
   dane_prof.si1/=1000;
   dno_os_i=1;
   }

  dane_prof.si2=(dane_prof0->si2);   /*material dla k.i.*/

  if (dane_global.odl!=0)
   {
   dane_prof.rki=0;
   dane_prof0->rki=0;
   dane_prof.si1=0;
   dane_prof0->si1=0;
   dane_prof.si2=0;
   dane_prof0->si2=0;
   }
   else
    {
     ;
    }

  dane_prof.si3=jednostkiOb(dane_prof0->si3);   /*srednice k.p.*/
  if (dane_prof0->si3>10)
   {
   dane_prof.si3/=1000;
   dno_os_p=1;
   }

  dane_prof.oi1=(dane_prof0->oi1);   /*material dla k.p.*/

//  dane_prof.oi2=0;
//  dane_prof.oi3=0;
//  dane_prof.sp1=0;
//  dane_prof.sp2=0;
    dane_prof.oi2=(dane_prof0->oi2/1000);   /*spadki k.i.*/

//    dane_prof.oi3=jednostkiOb(dane_prof0->oi3)/dane_global.ru;   /*dlugosci dla spadkow k.i.*/
//    dane_prof.oi3+=(dane_prof.odl-dane_profs.odl);
    dane_prof.oi3 = 0;

    dane_prof.sp1=(dane_prof0->sp1/1000);   /*spadki k.p.*/

    dane_prof0->sp2 += (dane_prof0->odl - dane_profs1.odl);  //ewentualne powiekszenie dlugosci

    dane_prof.sp2=jednostkiOb(dane_prof0->sp2)/dane_global.ru;   /*dlugosci dla spadkow k.p.*/

  //przyjeto koncepcje:
  //jezeli zadano spadek, jest on doliczony do aktualnych rzednych
  //jezeli zadano odleglosc, jest ona doliczana do aktualnej odleglosci
   dane_prof.odl = dane_profs.odl + dane_prof.sp2;
   dane_prof0->odl = dane_profs1.odl + dane_prof0->sp2;

   //ewentualna zmiana rzednych
   if (dane_prof0->rki>0)
     {
      dane_prof0->rki-=(dane_prof0->sp2*dane_prof.oi2);
      dane_prof.rki-=jednostkiOb(dane_prof0->sp2*dane_prof.oi2);
     }
   if (dane_prof0->rkp>0)
     {
      dane_prof0->rkp-=(dane_prof0->sp2*dane_prof.sp1);
      dane_prof.rkp-=jednostkiOb(dane_prof0->sp2*dane_prof.sp1);
     }

  dane_prof.op2=dane_prof0->op2;  //opis zaglebienia kp

  if (dane_global.odl<2)
   {
    dane_prof.sp3=0;
    dane_prof.op1=0;
   }
    else
     {
      dane_prof.sp3=jednostkiOb(dane_prof0->sp3);  //zaglebienie ki
      dane_prof.op1=jednostkiOb(dane_prof0->op1);  //zaglebienie kp
      //jezeli podano zaglebienie <> 0, to rzedna obliczana jest na podstawie -
      //rzednej terenu projektowanego (dla op2==0) lub istniejacego (op2==1)
      if (dane_prof.op1 != 0)
       {
         if (dane_prof.op2==0)
           {
             dane_prof0->rkp = dane_prof0->ru - dane_prof0->op1;
             dane_prof.rkp = jednostkiOb(dane_prof0->rkp);
           }
          else
            {
              dane_prof0->rkp = dane_prof0->rt - dane_prof0->op1;
              dane_prof.rkp = jednostkiOb(dane_prof0->rkp);
            }
       }
     }

  dane_prof.op3=dane_prof0->op3;  //rodzaj nawierzchni
  
  /* kontrola poprawnosci danych z ewentualnym wyznaczaniem brakujacych danych */
  
  if (dane_prof0->odl>0)
  {
   if (dane_prof0->odl==(dane_global.rki*100))
   /* zadana odleglosc jest rowna wartosci pierwszego hektometru */
   {
    ErrList(130);
    return 0;
   }

  }

  dane_global.oi1=(dane_prof0->odl / 100);
  if (dane_global.oi1<dane_global.rki)
   {
   ErrList(128);
   return 0;
   }

   if (dane_prof0->odl==0)
    {
    if (dane_prof.ru>0)
     {
     if (dane_global.odl==0) ErrList(147); else ErrList(162);
     return 0;
     }
    if (dane_prof.rt>0)
     {
     if (dane_global.odl==0) ErrList(148); else ErrList(163);
     return 0;
     }
    }

  /* PRZYPADEK 1,2,4  odl>0 : */

  dlugosc_ki=0;
  dlugosc_kp=0;
  spadek_ki=0;
  spadek_kp=0;
  

  if (dane_prof0->odl>0) /* zadana wartosc odleglosci */
   {
   /* KANAL ISTNIEJACY */
   /* PRZYPADEK 1, rki>0 */

    if (dane_prof.rki>0)  /* zadana wartosc rzednej kanalu */
     {
      /* wyznaczenie odleglosci */
      dlugosc_ki=0;
      spadek_ki=0;
     }

    /* KANAL PROJEKTOWANY */
    /* ------------------ */
    /* PRZYPADEK 1, rkp>0 */
    if (dane_prof.rkp>0)  /* zadana wartosc rzednej kanalu */
     {
      /* wyznaczenie odleglosci */
      dlugosc_kp=0;
  	   spadek_kp=0;
	 }
   }
   else  /* brak wartosci odleglosci tzn. odl=0 */
   {
     ErrList(117);
     return 0;
   }

  /*ustalenie srednic*/
  l_srednic_i=0;
  if (dane_prof.si1>0) l_srednic_i=1;
  sri_max=0;
  if (l_srednic_i==1) sri_max=dane_prof.si1;

  l_srednic_p=0;
  if (dane_prof.si3>0) l_srednic_p=1;
  srp_max=0;
  if (l_srednic_p==1) srp_max=dane_prof.si3;

  l_spadkow_i=0;
  if (dane_prof.oi2!=0) l_spadkow_i=1;

  l_spadkow_p=0;
  if (dane_prof.sp1!=0) l_spadkow_p=1;

  /* WSZYSTKIE DANE DOTYCZACE ODLEGLOSCI O.K. */
  /* KANAL ISTNIEJACY */
  /********************/
  if (licowanie_dnem == 0) O2Block=O2BlockPline; else O2Block=O2BlockHatch25;
  
  l_punktow_i=0;
//  if ((dane_profs.rki>0) && (dane_prof.rki>0))
   if (dane_prof.rki>0)
   {
    l_punktow_i=2;
    punkt_i[1].odl=dane_profs.spi3;
    punkt_i[1].rz=dane_profs.rki;
    punkt_i[1].s=dane_prof.si1;
    punkt_i[2].odl=dane_prof.odl;
    punkt_i[2].rz=dane_prof.rki ;
    punkt_i[2].s=0;
   }

  /* KANAL PROJEKTOWANY*/
  /*******************/
  l_punktow_p=0;
//  if ((dane_profs.rkp>0) && (dane_prof.rkp>0))
  if (dane_prof.rkp>0)
   {
    l_punktow_p=2;
    punkt_p[1].odl=dane_profs.opi1;
    punkt_p[1].rz=dane_profs.rkp;
    punkt_p[1].s=dane_prof.si3;
    punkt_p[2].odl=dane_prof.odl;
    punkt_p[2].rz=dane_prof.rkp ;
    punkt_p[2].s=0;
   }

   /* wyznaczono wiec 2 punkty dla kanalu istniejacego i projektowanego*/

/* generowanie (ewentualne) bloku hektometrow */

   /* obliczenie nowej wartosci hektometrow si3 */

  /*zaokragloenie wartosci odleglosci w gore, do pelnego hektometra */

  dane_global.oi1 = ceil(dane_prof0->odl / 100);

  dane_global.si1=(jednostkiOb(dane_global.rki)/dane_global.ru);
  dane_global.si2=(jednostkiOb(dane_global.rkp)/dane_global.ru);  /* stare */
  dane_global.si3=(jednostkiOb(dane_global.oi1)/dane_global.ru);  /* nowe */

  if ((dane_global.odl==0) || (dane_global.odl==2))
  {
      if (dane_global.si3>dane_global.si2)
      {
          /* dodanie nowego segmentu hektomatrow  */
          l_kr=generowanie_bloku_hektometrow(dane_global.rkp, dane_global.ru, dane_global.oi1, 0, 1);
          if (l_kr==0) return 0;
      }
  }  /* typ profilu 0*/

  else  if (dane_global.odl==1) //projektowany lub technologiczny
    {
        if (dane_global.si3>dane_global.si2)
        {
            //table horizontal lines correction
            //static int generowanie_bloku_hektometrow(double dane_global_rkp, double dane_global_ru, double dane_global_oi1, int insert_local, int check_table)
            odl_1=jednostkiOb((dane_global.oi1)*100)/dane_global.ru;
            int int_exp = expand_table_lines(get_localx(), odl_1+get_localx());
        }
    }  /* typ profilu 0*/

  if (FALSE == add_block_with_type (dane_prof.odl, 0, "*KP", 0, B_KPOMIAR))
  {
  return 0;
  }

//    if (FALSE == add_block (B_PLINE, 0))
//     {
//      return FALSE;
//     }

  del_odl=-1;
  
  if (dane_prof0->odl==(dane_global.rki*100)) //pierwszy hektometr
  {
   del_odl=3.5;
  }

  memmove(&textg, &TextG, sizeof(TEXT));
  
  parametry_ogolne_tekstu(Pi/2, ht1,koloryp.rzedne);

  if ((dane_global.odl==0) || (dane_global.odl==2))
  {
      /*tekst 1  ODLEGLOSC*/
      TextG.x=(float)(dane_prof.odl+del_odl);
      TextG.y=(float)(1.+dt_y);

      odl_i=(int) dane_prof0->odl;
      odl_1=(odl_i % 100);
      odl_r=odl_1+(dane_prof0->odl-odl_i);

      //gcvt(odl_r,ndig,tekst_p);
      sprintf_prec(tekst_p,odleglosc_prec,odl_r);
      l_kr=add_000(tekst_p,odleglosc_poz);
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.typ=n_odleglosc;

      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

      if ((dane_prof0->rki>0 || dane_prof0->rkp>0) && dane_global.odl!=0)
      {
          //linia pionowa na wysokosc tabeli
          if(!Lin(dane_prof.odl,0,dane_prof.odl,tablica_y[14]/*78*/,64,koloryp.os_pomiaru)) return 0;
      }
      else
      {
          //linia pionowa na wysokosc wiersza tabeli tabeli
          if(!Lin(dane_prof.odl,0,dane_prof.odl,10,64,koloryp.os_pomiaru)) return 0;
      }
  }

  if (dane_global.odl==0)
  {
      /*srednice / dlugosci  tekst2*/
      /*spadki / dlugosci    tekst3*/

      if (dane_prof.rki>0)
      {
          /*tekst 4  rzedne kanalu istniejacego*/
          if(!Lin(dane_prof.odl,24,dane_prof.odl,34,64,koloryp.os_pomiaru)) return 0;

          if(!Lin(dane_prof.odl,17,dane_prof.odl,24,64,koloryp.os_pomiaru)) return 0;

          if(!Lin(dane_prof.odl,10,dane_prof.odl,17,64,koloryp.os_pomiaru)) return 0;

          TextG.x=(float)(dane_prof.odl+del_odl);
          TextG.y=(float)(24.+dt_y);
          //gcvt(dane_prof0->rki,ndig,tekst_p);
          sprintf_prec(tekst_p,rzedna_i_prec,dane_prof0->rki);
          l_kr=add_000(tekst_p,rzedna_i_poz);
          strcpy(&TextG.text[0],tekst_p);
          LengthT=(int)strlen(TextG.text);
          if (LengthT > 6) TextG.width_factor=0.85f;
          TextG.dl=LengthT;
          TextG.n=T18+TextG.dl;
          TextG.typ=n_rzedna_kanalu_i;

          if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
          TextG.width_factor=1;

          if (dno_os_i==0)
          {
              {if (!Punkt(punkt_i[2].odl,tablica_y[14]+(punkt_i[2].rz-rpp),1,koloryp.l_kanal_i,O2Block,O3Dno)) return 0;}
              {if (!Punkt(punkt_i[2].odl,tablica_y[14]+(punkt_i[2].rz-rpp)+punkt_i[1].s,2,koloryp.l_kanal_i,O2Block,O3Dno)) return 0;}
          }
          else
          {
              {if (!Punkt(punkt_i[2].odl,tablica_y[14]+(punkt_i[2].rz-rpp)-(punkt_i[1].s/2),1,koloryp.l_kanal_i,O2BlockHatch50,O3Os)) return 0;}
              {if (!Punkt(punkt_i[2].odl,tablica_y[14]+(punkt_i[2].rz-rpp)+(punkt_i[1].s/2),2,koloryp.l_kanal_i,O2BlockHatch50,O3Os)) return 0;}
          }

      }
  } /* typ profilu 0*/
  /*srednice / dlugosci  tekst5*/
  /*spadki / dlugosci    tekst6*/
  
  if (dane_prof.rkp>0)
  {
      /*tekst 7  rzedne kanalu projektowanego*/
      if (dane_global.odl==0)
      {
          if(!Lin(dane_prof.odl,tablica_y[11],dane_prof.odl,tablica_y[12],64,koloryp.os_pomiaru)) return 0;
          if(!Lin(dane_prof.odl,tablica_y[9],dane_prof.odl,tablica_y[11],64,koloryp.os_pomiaru)) return 0;
          if(!Lin(dane_prof.odl,tablica_y[7],dane_prof.odl,tablica_y[9],64,koloryp.os_pomiaru)) return 0;
      }
      else if (dane_global.odl==1)
      {
          if(!Lin(dane_prof.odl,tablica_y[11],dane_prof.odl,tablica_y[12],64,koloryp.os_pomiaru)) return 0;
          if(!Lin(dane_prof.odl,tablica_y[9],dane_prof.odl,tablica_y[11],64,koloryp.os_pomiaru)) return 0;
          if(!Lin(dane_prof.odl,tablica_y[7],dane_prof.odl,tablica_y[9],64,koloryp.os_pomiaru)) return 0;
      }

      TextG.x=(float)(dane_prof.odl+del_odl);
      if ((dane_global.odl==0) || (dane_global.odl==2)) TextG.y=(float)(tablica_y[11] /*49*/ +dt_y);
      else TextG.y=(float)(tablica_y[11] /*49*/ + dt_y);
      sprintf_prec(tekst_p,rzedna_p_prec,dane_prof0->rkp);
      l_kr=add_000(tekst_p,rzedna_p_poz);
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      if (LengthT > 6) TextG.width_factor=0.85f;
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.typ=n_rzedna_kanalu_p;

      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.width_factor=1;

      if (dno_os_p==0)
      {
          {if (!Punkt(punkt_p[2].odl,tablica_y[14]+(punkt_p[2].rz-rpp),3,koloryp.l_kanal_p,O2Block,O3Dno)) return 0;}
          {if (!Punkt(punkt_p[2].odl,tablica_y[14]+(punkt_p[2].rz-rpp)+punkt_p[1].s,4,koloryp.l_kanal_p,O2Block,O3Dno)) return 0;}
      }
      else
      {
          {if (!Punkt(punkt_p[2].odl,tablica_y[14]+(punkt_p[2].rz-rpp)-(punkt_p[1].s/2),3,koloryp.l_kanal_p,O2BlockHatch50,O3Os)) return 0;}
          {if (!Punkt(punkt_p[2].odl,tablica_y[14]+(punkt_p[2].rz-rpp)+(punkt_p[1].s/2),4,koloryp.l_kanal_p,O2BlockHatch50,O3Os)) return 0;}
      }
  }

  if (dane_global.odl==2)  /* typ profilu 2 */
   {
    //jezeli nie jest zdefiniowany kanal projektowany ale jest istniejacy
    //to zaglebienie liczy sie od kanalu istniejacego
  //  rzedna_tp=dane_prof0->ru;

  //  if ((rzedna_tp==0) && (dane_prof0->rt>0)) rzedna_tp=dane_prof0->rt; na razie wylaczono
    // zaglebienie kanalu projektowanego
    zaglebienie_ok=FALSE;
    if (dane_prof.op2==0)  //od terenu projektowanego
     {
      if (dane_prof0->ru>0)
       {
        zaglebienie_ok=TRUE;
        zaglebienie_kp = dane_prof0->ru - dane_prof0->rkp;
       }
     }
    else if (dane_prof.op2==1) //od terenu istniejacego
       {
       if (dane_prof0->rt>0)
         {
          zaglebienie_ok=TRUE;
          zaglebienie_kp = dane_prof0->rt - dane_prof0->rkp;
         }
       }
    else zaglebienie_kp = 0;
     
    if (dane_prof0->rkp>0)
     {
       /*tekst 4  zaglebienie kanalu projektowanego*/

//      if (dane_global.odl==0)  //to nigdy nie wystapi
//       {
//        if(!Lin(dane_prof.odl,24,dane_prof.odl,34,64,koloryp.os_pomiaru)) return 0;
//       }

      TextG.x=(float)(dane_prof.odl+del_odl);
      TextG.y=(float)(24.+dt_y);
      if (zaglebienie_ok==TRUE)
      {
        sprintf_prec(tekst_p,zaglebienie_prec,zaglebienie_kp);
        l_kr=add_000(tekst_p,zaglebienie_poz);
      }
      else strcpy(tekst_p,"");
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      if (LengthT > 6) TextG.width_factor=0.85f;
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.typ=n_zaglebienie_kanalu_p;
      if (dane_prof.op2==1) TextG.obiektt3=O3Teren_i;
      else TextG.obiektt3=O3Teren_p;

      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.width_factor=1;
     }
   }

  if (dane_prof.ru>0)
  {
      /*tekst 8  rzedne ulicy*/
      if (((dane_prof0->rki==0) && (dane_prof0->rkp==0)) || (dane_global.odl==0))
      {
          if(!Lin(dane_prof.odl,tablica_y[12]/*60*/,dane_prof.odl,tablica_y[13]/*69*/,64,koloryp.os_pomiaru)) return 0;
      }
      else if ((dane_prof0->ru>0) && (dane_global.odl==1))
      {
          if(!Lin(dane_prof.odl,tablica_y[12]/*60*/,dane_prof.odl,tablica_y[13]/*69*/,64,koloryp.os_pomiaru)) return 0;
      }

      TextG.x=(float)(dane_prof.odl+del_odl);
      TextG.y=(float)(tablica_y[12]/*60*/+dt_y);
      //gcvt(dane_prof0->ru,ndig,tekst_p);

      if (ru_avail==TRUE)
      {
          sprintf_prec(tekst_p,rzedna_tp_prec,dane_prof0->ru);
          l_kr=add_000(tekst_p,rzedna_tp_poz);
      }
      else strcpy(tekst_p,"");
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      if (LengthT > 6) TextG.width_factor=0.85f;
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.typ=n_rzedna_ulicy;

      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.width_factor=1;

      if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.ru-rpp),5,koloryp.l_ulicy,O2BlockPline, O3Dno)) return 0;
  }

  if (dane_prof.rt>0)
  {
      /*tekst 8  rzedne terenu*/
      if (((dane_prof0->rki==0) && (dane_prof0->rkp==0)) || (dane_global.odl==0))
      {
          if(!Lin(dane_prof.odl,tablica_y[13]/*69*/,dane_prof.odl,tablica_y[14]/*78*/,64,koloryp.os_pomiaru)) return 0;
      }
      else if ((dane_prof0->ru>0) && (dane_global.odl==1))
      {
          if(!Lin(dane_prof.odl,tablica_y[13]/*69*/,dane_prof.odl,tablica_y[14]/*78*/,64,koloryp.os_pomiaru)) return 0;
      }

      TextG.x=(float)(dane_prof.odl+del_odl);
      TextG.y=(float)(tablica_y[13]/*69*/+dt_y);
      sprintf_prec(tekst_p,rzedna_ti_prec,dane_prof0->rt);
      l_kr=add_000(tekst_p,rzedna_ti_poz);
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      if (LengthT > 6) TextG.width_factor=0.85f;
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.obiektt3=O3NormalText;
      TextG.typ=n_rzedna_terenu;

      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.width_factor=1;

      if(!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rt-rpp),6,koloryp.l_teren_i,O2BlockPline, O3Dno)) return 0;
  }

  TextG.typ=n_typ_normal;

  if (dane_global.odl==2)  // typ profilu 2
   {
    // punkty charakterystyczne  comments_tmp[3]
    if (strlen(comments_tmp[3]) > 0)
    {
     TextG.x=(float)(dane_prof.odl - 2.);
     TextG.y=(float)(18.0+dt_y);
     strcpy(&TextG.text[0],comments_tmp[3]);
     LengthT=(int)strlen(TextG.text);
     len_t1=LengthT;
     len_t2=0;
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
     TextG.x=(float)dane_prof.odl;
     TextG.kat= 0;
     TextG.wysokosc=(float)ht3;
     TextG.czcionka=zmwym.czcionka; //0;  //to mozna zmienic
     TextG.justowanie=j_do_prawej;
     TextG.kolor=koloryp.spadek_l_fi_mat;

     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
     TextG.justowanie=j_do_lewej;
     TextG.wysokosc=(float)ht2;
    } 
    
    // rodzaj nawierzchni
    if (dane_prof.op3>0)
     {
        op3=(int)dane_prof.op3;
        
        switch (op3)
        {
         case 0: strcpy(tekst_p1,"");
	 break;
         case 1: strcpy(tekst_p1,"ziemna");
 	 break;
         case 2: strcpy(tekst_p1,"asfaltowa");
  	 break;
         case 3: strcpy(tekst_p1,"betonowa");
	 break;
         case 4: strcpy(tekst_p1,"kostka gran.");
	 break;
         case 5: strcpy(tekst_p1,"brukowa");
	 break;
         default: strcpy(tekst_p1,"");
         break;
        }

      if (strlen(tekst_p1)>0)
      {
       strcpy(&TextG.text[0],tekst_p1);
       TextG.y=(float)(11.5+dt_y);
       LengthT=(int)strlen(TextG.text);
       TextG.dl=LengthT;
       TextG.n=T18+TextG.dl;
       TextG.x=(float)(dane_prof.odl - 5.);
       TextG.justowanie = j_do_prawej;
       TextG.kat= 0;
       TextG.wysokosc=(float)ht2;
       TextG.czcionka=zmwym.czcionka; //0;
       if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      } 
     }
   } 
   
   // opis wezla comments_tmp[0]
    if (strlen(comments_tmp[0]) > 0)
    {
     TextG.y=-10;
     strcpy(&TextG.text[0],comments_tmp[0]);
     LengthT=(int)strlen(TextG.text);
     len_t1=LengthT;
     len_t2=0;
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
     TextG.x=(float)dane_prof.odl;
     TextG.kat= 0;
     TextG.wysokosc=3.0f;
     TextG.czcionka=zmwym.czcionka; //0;  //to mozna zmienic
     TextG.kolor=7;
     TextG.justowanie=j_srodkowo;

     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
     TextG.justowanie=j_do_lewej;
     TextG.wysokosc=(float)ht2;
    }

    // opis wezla comments_tmp[1]   //alternatywnie
    else if (strlen(comments_tmp[1]) > 0)
    {
     TextG.y=-10.f;
     strcpy(&TextG.text[0],comments_tmp[1]);
     LengthT=(int)strlen(TextG.text);
     len_t1=LengthT;
     len_t2=0;
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
     TextG.x=(float)dane_prof.odl;
     TextG.kat= 0.f;
     TextG.wysokosc=3.0f;
     TextG.czcionka=zmwym.czcionka; //0;  //to mozna zmienic
     TextG.kolor=7;
     TextG.justowanie=j_srodkowo;

     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
     TextG.justowanie=j_do_lewej;
     TextG.wysokosc=(float)ht2;
    }


/*dopisanie srednicy i materialu kanalu istniejacego*/

   if (dane_prof0->rki>0)
   {
   //pierwsza srednica
    TextG.y=(float)(11.5+dt_y);
    strcpy(tekst_p,u8"Ø"); //instead of ⌀ due to lack in Arial font
    if (l_srednic_i>0)
    {
     sprintf(tekst_p1,"%g",dane_prof0->si1);

     decimal(tekst_p1);

     strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
     if (dane_prof0->si1>10)
      {
      strcpy(tekst_p1," mm");
      strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
      }
    }
    if (dane_prof.si2>0)
     {
      strcpy(tekst_p1," ");
      strncat(tekst_p,tekst_p1,1);
      si2i=(int) dane_prof.si2;
      if (si2i < 8)
       {
        strcpy(tekst_p1,material[si2i]);
        strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
       }
      else if (si2i==8) //material inny definiowany
        {
          if (strlen(comments_tmp[2]) > 0)
           {
             strcpy(tekst_p1,comments_tmp[2]);
             if (strcmp(tekst_p1,"?")!=0) strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
           }
        }
     }

    strcpy(&TextG.text[0],tekst_p);

    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
  //  TextG.x=dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5);
    TextG.x=(float)(dane_prof.odl-5.);
    TextG.kat= 0;
    TextG.wysokosc=(float)ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_do_prawej;
    TextG.typ=n_srednica_kanalu_i;
    TextG.kolor=koloryp.spadek_l_fi_mat;
    //    if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

/* dopisanie srednicy i materialu kanalu projektowanego*/

   if (dane_prof0->rkp)
   {
   TextG.y=(float)(tablica_y[8]+dt_y);
   strcpy(tekst_p,"");
   if (l_srednic_p>0)
    {
     TextG.y=(float)(tablica_y[8]+dt_y);
     strcpy(tekst_p,u8"Ø"); //instead of ⌀ due to lack in Arial font
     sprintf(tekst_p1,"%g",dane_prof0->si3);

     decimal(tekst_p1);

     strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
     if (dane_prof0->si3>10)
      {
      strcpy(tekst_p1," mm");
      strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
      }
    }
     if (dane_prof.oi1>0)
      {
       strcpy(tekst_p1," ");
       strncat(tekst_p,tekst_p1,1);
       oi1i=(int) dane_prof.oi1;
       if (oi1i < 8)
       {
         strcpy(tekst_p1,material[oi1i]);
         strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
       }
       else if (oi1i==8) //material inny definiowany
        {
          if (strlen(comments_tmp[2]) > 0)
           {
             strcpy(tekst_p1,comments_tmp[2]);
             if (strcmp(tekst_p1,"?")!=0) strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
           }
        }
      }

     strcpy(&TextG.text[0],tekst_p);

     LengthT=(int)strlen(TextG.text);
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
 //    TextG.x=dane_prof_odl_kp-((dane_prof_odl_kp-dane_profs.opi1)*0.5);
     TextG.x=(float)(dane_prof.odl-5.);
     TextG.kat= 0.f;
     TextG.wysokosc=(float)ht2;
     TextG.czcionka=zmwym.czcionka; //0;
     TextG.justowanie=j_do_prawej;
     TextG.typ=n_srednica_kanalu_p;
     TextG.kolor=koloryp.spadek_l_fi_mat;
     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

  strcpy(&TextG.text[0],"");

 //DOPISANIE SPADKOW
 // dopisanie spadku i dlugosci kanalu istniejacego
 //wartosc spadku podawana jest zawsze jezeli rysowana jest linia spadku
//   if (linia_ki==TRUE)
//   {
/*
    TextG.y=18.5+dt_y;
    strcpy(tekst_p,"");
    sprintf(tekst_p1,"%.2f",fabs(spadek_ki));
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=strlen(TextG.text);
    len_t1=LengthT;
    len_t2=0;
  	 strcpy(tekst_p,"l=");
	 sprintf(tekst_p1,"%.2f",dlugosc_ki);
	 strncat(tekst_p,tekst_p1,strlen(tekst_p1));
	 len_t2=strlen(tekst_p);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    //polozenie tekstu w polowie pomiedzy pomiarami
//    TextG.x=dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5)-5;
    TextG.x=dane_prof.odl - 5;
    TextG.kat= 0;
    TextG.wysokosc=ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_do_prawej;
    TextG.kolor=koloryp.spadek_l_fi_mat;
    TextG.typ=n_spadek_kanalu_i;
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;


    strcpy(tekst_p,"l=");
    sprintf(tekst_p1,"%.2f",dlugosc_ki);
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
     //polozenie tekstu w polowie pomiedzy pomiarami
//  TextG.x=dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5)+5;
    TextG.x=dane_prof.odl + 5;
    TextG.kat= 0;
    TextG.wysokosc=ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_do_lewej;
    TextG.typ=n_dlugosc_kanalu_i;
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

      // dopisanie linii ukosnej
    if (spadek_ki>0)   //   \
     {
      if(!Lin(TextG.x-9,TextG.y+4,TextG.x-1,TextG.y-1.5,64,koloryp.spadek_l_fi_mat)) return 0;
     }
     else
     if (spadek_ki<0)  //  /
      {
       if(!Lin(TextG.x-9,TextG.y-1.5,TextG.x-1,TextG.y+4,64,koloryp.spadek_l_fi_mat)) return 0;
      }
      else if(!Lin(TextG.x-9,TextG.y+2,TextG.x-1,TextG.y+2,64,koloryp.spadek_l_fi_mat)) return 0;
//    if(!Lin(dane_prof_odl_ki,17,dane_prof_odl_ki,24,64,koloryp.os_pomiaru)) return 0;
//   }
   
// dopisanie spadku i dlugosci kanalu projektowanego
//   if (linia_kp==TRUE )
//   {
    TextG.y=tablica_y[10]+dt_y;
    strcpy(tekst_p,"");
//    gcvt(fabs(spadek_kp),ndig,tekst_p1);
    sprintf(tekst_p1,"%.2f",fabs(spadek_kp));
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=strlen(TextG.text);
    len_t1=LengthT;
    len_t2=0;

    strcpy(tekst_p,"l=");
    sprintf(tekst_p1,"%.2f",dlugosc_kp);
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    len_t2=strlen(tekst_p);

    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.x=dane_prof.odl-5;
    TextG.kat= 0;
    TextG.wysokosc=ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_do_prawej;
    TextG.kolor=koloryp.spadek_l_fi_mat;
    TextG.typ=n_spadek_kanalu_p;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

    strcpy(tekst_p,"l=");
    //gcvt(dlugosc_kp,ndig,tekst_p1);
    sprintf(tekst_p1,"%.2f",dlugosc_kp);
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.x=dane_prof.odl+5;
    TextG.kat= 0;
    TextG.wysokosc=ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_do_lewej;
    TextG.kolor=koloryp.spadek_l_fi_mat;
    TextG.typ=n_dlugosc_kanalu_p;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

    // dopisanie linii ukosnej
    if (spadek_kp>0)   //   \
     {
      if(!Lin(TextG.x-9,TextG.y+4,TextG.x-1,TextG.y-1.5,64,koloryp.spadek_l_fi_mat)) return 0;
     }
     else
     if (spadek_kp<0)  //  /
      {
       if(!Lin(TextG.x-9,TextG.y-1.5,TextG.x-1,TextG.y+4,64,koloryp.spadek_l_fi_mat)) return 0;
      }
      else
       if(!Lin(TextG.x-9,TextG.y+2,TextG.x-1,TextG.y+2,64,koloryp.spadek_l_fi_mat)) return 0;

//    if(!Lin(dane_prof_odl_kp,tablica_y[9],dane_prof_odl_kp,tablica_y[11],64,koloryp.os_pomiaru)) return 0;
//   }

*/

  strcpy(&TextG.text[0],"");
  TextG.justowanie=j_do_lewej;

  //  generowanie linii rzednych
//    if ((dane_profs.rki==0) && (dane_prof.rki==0)) rk_max=0;
//     else if (dane_profs.rki==0) rk_max=dane_prof.rki;
//       else if (dane_prof.rki==0) rk_max=dane_profs.rki;
//	 else rk_max=dane_prof.rki;

	 rk_max=dane_prof.rki;

    if ((rk_max>0) && (dane_prof.si1>0)) rk_max+=dane_prof.si1;

  rk_max+=sri_max;
  max_r=rk_max;

//    if ((dane_profs.rkp==0) && (dane_prof.rkp==0)) rk_max=0;
//     else if (dane_profs.rkp==0) rk_max=dane_prof.rkp;
//       else if (dane_prof.rkp==0) rk_max=dane_profs.rkp;
//	 else rk_max=dane_prof.rkp;

  rk_max=dane_prof.rkp;

    if ((rk_max>0) && (dane_prof.si3>0)) rk_max+=dane_prof.si3;

  rk_max+=srp_max;
  if (rk_max>max_r) max_r=rk_max;

   if (dane_prof.ru==0) rk_max=0;
   else rk_max=dane_prof.ru;


  if (rk_max>max_r) max_r=rk_max;

   if (dane_prof.rt==0) rk_max=0;
   else rk_max=dane_prof.rt;

  if (rk_max>max_r) max_r=rk_max;

  //linia pionowa cienka
  if(!Lin(dane_prof.odl,tablica_y[14],dane_prof.odl,tablica_y[14]+(max_r-rpp),32,koloryp.os_pomiaru)) return 0;

/*
  
//  linia kanalu istniejacego (powinno byc linia b.gruba), nie jest rysowana jezeli punkt_i[1].odl=punkt_i[2].odl
  linia_ki=FALSE;
  if ((punkt_i[2].odl>0) && (punkt_i[2].odl>punkt_i[1].odl))
   {

    //nowa wersja :  mamy punkt_i[l_punktow_i]
    if ((dane_profs.rki==0) && (dane_prof.rki==0))  //nic nie jest rysowane
    {
     ;
    }
    else
    if ((dane_profs.rki==0) || (dane_prof.rki==0)) //nic nie jest rysowane
    {
     ;
    }

    else   //rysowane sa kolejno wszystkie punkty
    {
    for (i=1; i<l_punktow_i; i++)
     {
     if (dno_os_i==0)
      {
       {if(!Lin_special(punkt_i[i].odl,tablica_y[14]+(punkt_i[i].rz-rpp),punkt_i[i+1].odl,tablica_y[14]+(punkt_i[i+1].rz-rpp),96,koloryp.l_kanal_i)) return 0;}
       {if(!Lin_special(punkt_i[i].odl,tablica_y[14]+(punkt_i[i].rz-rpp)+punkt_i[i].s,punkt_i[i+1].odl,tablica_y[14]+(punkt_i[i+1].rz-rpp)+punkt_i[i].s,96,koloryp.l_kanal_i)) return 0;}
      }
      else
	{
	 {if(!Lin_special(punkt_i[i].odl,tablica_y[14]+(punkt_i[i].rz-rpp)-(punkt_i[i].s/2),punkt_i[i+1].odl,tablica_y[14]+(punkt_i[i+1].rz-rpp)-(punkt_i[i].s/2),96,koloryp.l_kanal_i)) return 0;}
	 {if(!Lin_special(punkt_i[i].odl,tablica_y[14]+(punkt_i[i].rz-rpp)+(punkt_i[i].s/2),punkt_i[i+1].odl,tablica_y[14]+(punkt_i[i+1].rz-rpp)+(punkt_i[i].s/2),96,koloryp.l_kanal_i)) return 0;}
	}
     linia_ki=TRUE;
     //dodatkowo rysowanie linii cienkiej w miejscu zmiany srednicy lub spadku
     //w tym celu przydatna jest informacja czy jest to zmiana srednicy czy spadku
     }
    }
   }
   

//  linia kanalu projektowanego (powinno byc linia b.gruba), nie jest rysowana jezeli punkt_p[1].odl=punkt_p[2].odl
// jezeli dno_os_p=1 to rzedna jest w osi kanau
  linia_kp=FALSE;
  
  if ((punkt_p[2].odl>0) && (punkt_p[2].odl>punkt_p[1].odl))
   {
    if ((dane_profs.rkp==0) && (dane_prof.rkp==0)) //nic nie jest rysowane
    {
     ;
    }
    else
    if ((dane_profs.rkp==0) || (dane_prof.rkp==0)) //nic nie jest rysowane
    {
     ;
    }

    else  //rysowane sa kolejno wszystkie punkty
    {
    for (i=1; i<l_punktow_p; i++)
     {
     if (dno_os_p==0)
      {
     	 {if(!Lin_special(punkt_p[i].odl,tablica_y[14]+(punkt_p[i].rz-rpp),punkt_p[i+1].odl,tablica_y[14]+(punkt_p[i+1].rz-rpp),96,koloryp.l_kanal_p)) return 0;}
	    {if(!Lin_special(punkt_p[i].odl,tablica_y[14]+(punkt_p[i].rz-rpp)+punkt_p[i].s,punkt_p[i+1].odl,tablica_y[14]+(punkt_p[i+1].rz-rpp)+punkt_p[i].s,96,koloryp.l_kanal_p)) return 0;}
      }
      else
       {
	     {if(!Lin_special(punkt_p[i].odl,tablica_y[14]+(punkt_p[i].rz-rpp)-(punkt_p[i].s/2),punkt_p[i+1].odl,tablica_y[14]+(punkt_p[i+1].rz-rpp)-(punkt_p[i].s/2),96,koloryp.l_kanal_p)) return 0;}
	     {if(!Lin_special(punkt_p[i].odl,tablica_y[14]+(punkt_p[i].rz-rpp)+(punkt_p[i].s/2),punkt_p[i+1].odl,tablica_y[14]+(punkt_p[i+1].rz-rpp)+(punkt_p[i].s/2),96,koloryp.l_kanal_p)) return 0;}
       }
     linia_kp=TRUE;
     }
    }
   }

//  linia ulicy, jest tez rysowana jezeli ...opi2=...odl
     if (dane_profs.ru==0)   // brak poprzedniej wartosci
       {
	 dane_profs.ru=dane_prof.ru;
	 if (dane_prof.ru>0)
	  {
	  if (dane_global.odl==0) 
	    {if(!Lin_special(dane_profs.opi2,tablica_y[14]+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]+(dane_prof.ru-rpp),65,koloryp.l_ulicy)) return 0;}
	     else {if(!Lin_special(dane_profs.opi2,tablica_y[14]+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]+(dane_prof.ru-rpp),65,koloryp.l_teren_p)) return 0;}
	   // linia kreskowa
	  }
       }
       else
       {
	if (dane_prof.ru>0)
	 {
	  if (dane_global.odl==0)
	    {if(!Lin_special(dane_profs.opi2,tablica_y[14]+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]+(dane_prof.ru-rpp),64,koloryp.l_ulicy)) return 0;}
	     else {if(!Lin_special(dane_profs.opi2,tablica_y[14]+(dane_profs.ru-rpp),dane_prof.odl,tablica_y[14]+(dane_prof.ru-rpp),65,koloryp.l_teren_p)) return 0;}
	       // dla profilu technologicznego linia kreskowa
	 }
       }

//  linia terenu, jest tez rysowana jezeli ...opi3=...odl
     if (dane_profs.rt==0)
      {
      dane_profs.rt=dane_prof.rt;
      if (dane_prof.rt>0)
       {
       if(!Lin_special(dane_profs.opi3,tablica_y[14]+(dane_profs.rt-rpp),dane_prof.odl,tablica_y[14]+(dane_prof.rt-rpp),65,koloryp.l_teren_i)) return 0;
       // linia kreskowa
       }
      }
      else
      {
       if (dane_prof.rt>0)
	{
	 if(!Lin_special(dane_profs.opi3,tablica_y[14]+(dane_profs.rt-rpp),dane_prof.odl,tablica_y[14]+(dane_prof.rt-rpp),64,koloryp.l_teren_i)) return 0;
	}
      }
      
*/

 /* generowanie tabelki opisowej */
/*
  if (FALSE == add_block (B_PLINE, 0))
   {
    return 0;
   }
   
   max_r=dane_profs.spp1;


  if (dane_prof.odl>dane_profs.odl)
   {
    if ((dane_global.odl==0) || (dane_global.odl==2))
    {
     local_y_ = get_localy();
     if(!Lin(dane_profs.odl,FormatY-40-local_y_,dane_prof.odl,FormatY-40-local_y_,64,koloryp.os_pomiaru)) return 0; //dolna
     if(!Lin(dane_profs.odl,FormatY-30-local_y_,dane_prof.odl,FormatY-30-local_y_,64,koloryp.os_pomiaru)) return 0; //gorna
     if (dane_profs.odl==(dane_global.rki*100))
      {
       if(!Lin(dane_profs.odl,FormatY-30-local_y_,dane_profs.odl,FormatY-40-local_y_,64,koloryp.os_pomiaru)) return 0; //z lewej
      }
    }
     else
      {
       if(!Lin(dane_profs.odl,FormatY-40-local_y_,dane_prof.odl,FormatY-40-local_y_,64,koloryp.os_pomiaru)) return 0; //dolna
       if(!Lin(dane_profs.odl,FormatY-30-local_y_,dane_prof.odl,FormatY-30-local_y_,64,koloryp.os_pomiaru)) return 0; //gorna
       if (dane_profs.odl==(dane_global.rki*100))
        {
         if(!Lin(dane_profs.odl,FormatY-30-local_y_,dane_profs.odl,FormatY-40-local_y_,64,koloryp.os_pomiaru)) return 0; //z lewej
        }
      }
   }

*/

 memmove(&TextG, &textg, sizeof(TEXT));

 /*zapisanie danych do pliku binarnego*/

  /* jezeli wartosci rki, rkp, ru, rt sa wieksze od zera, to odpowiednio: */
  /* w parametrze dane_prof.spi3 nalezy zapamietac wartosc odl. dla rki */
  /* w parametrze dane_prof.opi1 nalezy zapamietac wartosc odl. dla rkp */
  /* w parametrze dane_prof.opi2 nalezy zapamietac wartosc odl. dla ru */
  /* w parametrze dane_prof.opi3 nalezy zapamietac wartosc odl. dla rt */

//  return 1;  //!!!!!!!!!!!!!!!!!!!!
  memcpy(&dane_prof00, dane_prof0,sizeof(MyDane));

  if (dane_prof.rki>0) dane_prof00.spi3=dane_prof0_odl_ki;
  else
     {
      dane_prof00.rki=dane_profs1.rki;
      dane_prof00.spi3=dane_profs1.spi3;
     }

  if (dane_prof.rkp>0) dane_prof00.opi1=dane_prof0_odl_kp;
  else
     {
      dane_prof00.rkp=dane_profs1.rkp;
      dane_prof00.opi1=dane_profs1.opi1;
     }

  if (dane_prof.ru>0) dane_prof00.opi2=dane_prof00.odl;
  else
     {
      dane_prof00.ru=dane_profs1.ru;
      dane_prof00.opi2=dane_profs1.opi2;
     }

  if (dane_prof.rt>0) dane_prof00.opi3=dane_prof00.odl;
  else
     {
       dane_prof00.rt=dane_profs1.rt;
       dane_prof00.opi3=dane_profs1.opi3;
     }


//  dane_prof00.spp1=max_r;

  //kasowanie spadkow i odleglosci
  dane_prof00.oi2=0;
  dane_prof00.oi3=0;
  dane_prof00.sp1=0;
  dane_prof00.sp2=0;

///  memcpy(dane_prof0, &dane_prof00,sizeof(MyDane));  //!!!!!!!!

//  dane_profs1=*dane_prof0;
#ifdef SAVETMP
  // tutaj najpierw nalezy zachowac zbior poprzedni w zbiorze PROFIL_TEMP_FILE_BAK
  // a nastepnie zapisac nowe dane
  if (rename(PROFIL_TEMP_FILE,PROFIL_TEMP_FILE_BAK)!=0)
   {
    remove(PROFIL_TEMP_FILE_BAK);
    rename(PROFIL_TEMP_FILE,PROFIL_TEMP_FILE_BAK);
   }

#ifndef LINUX
    i_f_handle=open(PROFIL_TEMP_FILE, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    i_f_handle=open(PROFIL_TEMP_FILE, O_CREAT|O_TRUNC|O_RDWR, 0666);
#endif

  if (i_f_handle == -1)
  {
      ErrList(103);
      return 0;
  }
  
  if (write (i_f_handle, &dane_prof00, sizeof(MyDane)) != sizeof(MyDane))
  {
      close (i_f_handle) ;
      ErrList(104);
      return 0;
  }

   close (i_f_handle) ;
#endif
   /*zaznaczenie ze nastapil ostatni zapis pomiaru*/
   /* przyjeto ze znacznikiem aplikacji dla profilu jest wartosc 101 */
   l_kr=put_znacznik_aplikacji(101);
   
  return 1;
}

static int Profil2H(void *BufVar)          /*PIERWSZY POMIAR*/
{
  const double ht1=2.5 /*2.25*/;
  const double ht2=2.5;
  const double ht3=3.5;
  const double dt_y=0.8;
  MyDane *dane_prof0=(MyDane*)BufVar;
  ////MyDane dane_prof;
  ////MyDane dane_profs1;
  ////MyDane dane_profs0={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  ////MyDane dane_global;
  ////MyDane dane_prof00;
  int i_f_handle ;

  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  double del_odl;
  int l_kr;

     double rpp;
     double max_r;

  int odl_i;
  double odl_1;
  double odl_r;

  double reszta;
  int reszta_i;
  double odl_0;
  int i;
  int l_h, l_k, ii;
//  int dane_oi1;
//  char str1[80];
  double zaglebienie_ki;
  double zaglebienie_kp;
  int len_t1, len_t2;
  int dno_os_i, dno_os_p;

  TEXT textg=Tdef;
  double rzedna_tp;
  BOOL zaglebienie_ok;
  BOOL ru_avail;
  int O2Block;
  int l_srednic_i, l_srednic_p;
  /***************************/

  dane_global.ru=get_skala_profilu_x();
  dane_global.rt=get_poziom_pp();
  dane_global.odl=get_typ_profilu();
  tablica_0_1(dane_global.odl);

  dane_global.oi2=0;  /*dynamika bloku*/
  l_kr=put_dynamic_block(dane_global.oi2);

  if (dane_global.ru==0)
  {
  ErrList(111);
  return 0;
  }

  memmove(&dane_profs1, &dane_profs0, sizeof(MyDane));
  
  rpp=jednostkiOb(dane_global.rt);

  dane_global.rki=get_hektometr_p();
  dane_global.rkp=get_hektometr_k();
  
  dane_prof0->param1=get_param1(dane_prof0);

  ru_avail=TRUE;
  //jezeli podano rzedne terenu istniejacego (rt) a ru==0,
  //to przyjmujemy rt=ru;
  if ((dane_prof0->rt>0) && (dane_prof0->ru==0))
   {
    dane_prof0->ru=dane_prof0->rt;
    ru_avail=FALSE;
   }
  dane_prof.odl=(jednostkiOb(dane_prof0->odl-(dane_global.rki*100))/dane_global.ru);
  dane_prof.rki=jednostkiOb(dane_prof0->rki);
  dane_prof.rkp=jednostkiOb(dane_prof0->rkp);
  dane_prof.ru=jednostkiOb(dane_prof0->ru);
  dane_prof.rt=jednostkiOb(dane_prof0->rt);

  dane_prof.si1=jednostkiOb(dane_prof0->si1);  //fi i
  dane_prof.si2=jednostkiOb(dane_prof0->si2);  //
  dane_prof.si2=jednostkiOb(dane_prof0->si3);  //fi p

  dno_os_i=0;
  dno_os_p=0;
  
  dane_prof.si1=jednostkiOb(dane_prof0->si1);   /*srednice k.i.*/
  if (dane_prof0->si1>10)
   {
   dane_prof.si1/=1000;
   dno_os_i=1;
   }

  dane_prof.si3=jednostkiOb(dane_prof0->si3);   /*srednice k.i.*/
  if (dane_prof0->si3>10)
   {
   dane_prof.si3/=1000;
   dno_os_p=1;
   }

  /*ustalenie srednic*/

  l_srednic_i=0;
  if (dane_prof.si1>0) l_srednic_i=1;
  //sri_max=0;
  //if (l_srednic_i==1) sri_max=dane_prof.si1;

  l_srednic_p=0;
  if (dane_prof.si3>0) l_srednic_p=1;
  //srp_max=0;
  //if (l_srednic_p==1) srp_max=dane_prof.si3;

  dane_prof.oi1=jednostkiOb(dane_prof0->oi1);

//  dane_prof.sp1=jednostkiOb(dane_prof0->sp1);

//  dane_prof.op1=jednostkiOb(dane_prof0->op1);
  
//  dane_prof.sp3=jednostkiOb(dane_prof0->sp3);  //zaglebienie ki
//  dane_prof.op1=jednostkiOb(dane_prof0->op1);  //zaglebienie kp

  dane_prof.oi2=0;
  dane_prof.oi3=0;
  dane_prof.sp1=0;
  dane_prof.sp2=0;

  dane_prof.op2=dane_prof0->op2;  //opis zaglebienia kp

  if (dane_global.odl<2)
   {
    dane_prof.sp3=0;
    dane_prof.op1=0;
   }
    else
     {
      dane_prof.sp3=jednostkiOb(dane_prof0->sp3);  //zaglebienie ki
      dane_prof.op1=jednostkiOb(dane_prof0->op1);  //zaglebienie kp
      //jezeli podano zaglebienie <> 0, to rzedna obliczana jest na podstawie -
      //rzednej terenu projektowanego (dla op2==0) lub istniejacego (op2==1)
      if (dane_prof.op1 != 0)
       {
         if (dane_prof.op2==0)
           {
             dane_prof0->rkp = dane_prof0->ru - dane_prof0->op1;
             dane_prof.rkp = jednostkiOb(dane_prof0->rkp);
           }
          else
            {
              dane_prof0->rkp = dane_prof0->rt - dane_prof0->op1;
              dane_prof.rkp = jednostkiOb(dane_prof0->rkp);
            }
       }
     }

  dane_prof.op3=dane_prof0->op3;  //rodzaj nawierzchni

  dane_prof.spi3=dane_prof.odl;  /* dla rki */
  dane_prof.opi1=dane_prof.odl;  /* dla rkp */
  dane_prof.opi2=dane_prof.odl;  /* dla ru */
  dane_prof.opi3=dane_prof.odl;  /* dla rt */

  if (dane_global.odl!=0)
  {
   dane_prof.rki=0;
   dane_prof0->rki=0;
  }

  if (dane_prof0->odl<(dane_global.rki*100))
   {
    ErrList(132);
    return 0;
   }

  if (dane_prof.odl<0)
  {
   ErrList(108);
   return 0;
   }

  dane_global.oi1=(dane_prof0->odl / 100);
  if (dane_global.oi1<dane_global.rki)
   {
   ErrList(128);
   return 0;
   }

  if ((dane_global.odl==0) || (dane_global.odl==2))
  {
/* generowanie (ewentualne) bloku hektometrow */

   /* obliczenie nowej wartosci hektometrow si3 */
   /*zaokragloenie wartosci odleglosci w gore, do pelnego hektometra */

    dane_global.oi1 = ceil(dane_prof0->odl / 100);
//    dane_oi1 = (int) (dane_prof0->odl / 100) + 0.5;
//    dane_global.oi1 = dane_oi1;
  /*  */
  
//   sprintf(str1,"%.3f  %.3f  %.3f",dane_global.rkp, dane_global.ru, dane_global.oi1);
//   komunikat_str(str1);
//   Get_Key();

  dane_global.si1=(jednostkiOb(dane_global.rki)/dane_global.ru);
  dane_global.si2=(jednostkiOb(dane_global.rkp)/dane_global.ru);  /* stare */
  dane_global.si3=(jednostkiOb(dane_global.oi1)/dane_global.ru);  /* nowe */

//  usun_blok	(dane, dane + dane_size) ;

  if (dane_global.si3>dane_global.si2)
   {
   /* dodanie nowego segmentu hektomatrow  */

   l_kr=generowanie_bloku_hektometrow(dane_global.rkp, dane_global.ru, dane_global.oi1, 0, 1);
   if (l_kr==0) return 0;
   /*
   if (FALSE == add_block (B_PLINE, 0))
   {
    return 0;
   }

   TextG.kat= 0;
   TextG.wysokosc=ht3;
   TextG.italics=0;
   TextG.width_factor=1;
   TextG.czcionka=zmwym.czcionka; //1;
   TextG.obiektt1 = 0;
   TextG.obiektt2 = O2BlockPline;;
   TextG.blok=1;

   odl_0=0;
   odl_1=jednostkiOb(dane_global.rkp*100)/dane_global.ru;
   del_odl=0;

   for (i=int(dane_global.rkp); i<int(dane_global.oi1); i++)
   {
    odl_0=odl_1;
    odl_1=(jednostkiOb(100)/dane_global.ru)+odl_0;

    if(!Lin(odl_0+1,0,odl_1-1,0,5,7)) return 0;


    if(!Okr(odl_1,0,1,7)) return 0;


    TextG.x=odl_1+del_odl;
    TextG.y=-5.5;

    l_h = (i+1) % 10 ;
    l_k = (i+1) / 10 ;

    if (l_h==0)
      {
       if(!Okr(odl_1,0,0.5,7)) return 0;
      }

    if (l_k>0)
     {
    //  gcvt(l_k,ndig,tekst_p);
      sprintf(tekst_p,"%d",l_k);
      strncat(tekst_p,"+",1);
     }
      else strcpy(tekst_p,"");

    //gcvt(l_h,ndig,tekst_p1);
    sprintf(tekst_p1,"%d",l_h);
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

  strcpy(&TextG.text[0],"");
  dane_global.rkp=dane_global.oi1;
  l_kr=put_hektometr(dane_global.rkp,1);
  */
   }

  }

/* **** */
  /* generowanie bloku pierwszego pomiaru */

  if (FALSE == add_block_with_type (dane_prof.odl, 0, "*PP", 0, B_PPOMIAR))
   {
    return 0;
   }
/*
  if (dane_prof0->odl==(dane_global.rki*100)) //pierwszy hektometr
  {
   del_odl=3.5;
  }
  else
  {
   del_odl=-1;
  }
*/
  del_odl=3.5;

  memmove(&textg, &TextG, sizeof(TEXT));

  parametry_ogolne_tekstu(Pi/2, ht1,koloryp.rzedne);
/*  TextG.kat= Pi/2;
  TextG.wysokosc=ht1;
  TextG.italics=0;
  TextG.width_factor=1;
  TextG.czcionka=zmwym.czcionka; //1;
  TextG.obiektt1 = 0;
  TextG.obiektt2 = O2BlockPline;;
  TextG.blok=1;
  */

 if (licowanie_dnem == 0) O2Block=O2BlockPline; else O2Block=O2BlockHatch25;

 if ((dane_global.odl==0) || (dane_global.odl==2))  /* typ profilu 0 lub 2*/
 {
//  if(!Lin(dane_prof.odl,0,dane_prof.odl,10,64,koloryp.os_pomiaru)) return 0;
  /*tekst 1  ODLEGLOSC*/
  TextG.x=(float)(dane_prof.odl+del_odl);
  TextG.y=(float)(1.+dt_y);

  odl_i=(int) dane_prof0->odl;
  odl_1=(odl_i % 100);
  odl_r=odl_1+(dane_prof0->odl-odl_i);

  //gcvt(odl_r,ndig,tekst_p);
  sprintf_prec(tekst_p,odleglosc_prec,odl_r);
  l_kr=add_000(tekst_p,odleglosc_poz);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_odleglosc;
 /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  /* odleglosc */

  if (((dane_prof0->rki>0) || (dane_prof0->rkp>0)) && (dane_global.odl!=0))
   {
    if(!Lin(dane_prof.odl,0,dane_prof.odl,tablica_y[14],64,koloryp.os_pomiaru)) return 0;
   }
  else
     {
      if(!Lin(dane_prof.odl,0,dane_prof.odl,10,64,koloryp.os_pomiaru)) return 0;
     }
 } 

  /*srednice / dlugosci  tekst2*/
  /*spadki / dlugosci    tekst3*/
 if (dane_global.odl==0)  //typ profilu 0
 {  if (dane_prof.rki>0)
  {
  if(!Lin(dane_prof.odl,24,dane_prof.odl,34,64,koloryp.os_pomiaru)) return 0;

  if(!Lin(dane_prof.odl,17,dane_prof.odl,24,64,koloryp.os_pomiaru)) return 0;

  if(!Lin(dane_prof.odl,10,dane_prof.odl,17,64,koloryp.os_pomiaru)) return 0;

  /*tekst 4  rzedne kanalu istniejacego*/
  TextG.x=(float)(dane_prof.odl+del_odl);
  TextG.y=(float)(24.+dt_y);
  //gcvt(dane_prof0->rki,ndig,tekst_p);
  sprintf_prec(tekst_p,rzedna_i_prec,dane_prof0->rki);
  l_kr=add_000(tekst_p,rzedna_i_poz);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  if (LengthT > 6) TextG.width_factor=0.85f;
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_rzedna_kanalu_i;
 /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  TextG.width_factor=1;

  if (dno_os_i==0)
    {
      if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rki-rpp),1,koloryp.l_kanal_i,O2Block, O3Dno)) return 0;
      if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rki-rpp)+(dane_prof.si1),2,koloryp.l_kanal_i,O2Block,O3Dno)) return 0;
    }
  else
   {
     if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rki-rpp)-(dane_prof.si1/2),1,koloryp.l_kanal_i,O2BlockHatch50,O3Os)) return 0;
     if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rki-rpp)+(dane_prof.si1/2),2,koloryp.l_kanal_i,O2BlockHatch50,O3Os)) return 0;
   }
  }

  /*srednice / dlugosci  tekst5*/
  /*spadki / dlugosci    tekst6*/

 }  /* typ profilu 0  */

  if (dane_prof.rkp>0)
  {
  if (dane_global.odl==0)
   {
    if(!Lin(dane_prof.odl,tablica_y[11],dane_prof.odl,tablica_y[12],64,koloryp.os_pomiaru)) return 0;

    if(!Lin(dane_prof.odl,tablica_y[9],dane_prof.odl,tablica_y[11],64,koloryp.os_pomiaru)) return 0;

    if(!Lin(dane_prof.odl,tablica_y[7],dane_prof.odl,tablica_y[9],64,koloryp.os_pomiaru)) return 0;
   }
     
  /*tekst 7  rzedne kanalu projektowanego*/
  TextG.x=(float)(dane_prof.odl+del_odl);
  if (dane_global.odl==0 || dane_global.odl==2) TextG.y=(float)(tablica_y[11] /*49*/ +dt_y);
  else TextG.y=(float)(tablica_y[11] /*49*/ + dt_y);
  //gcvt(dane_prof0->rkp,ndig,tekst_p);
  sprintf_prec(tekst_p,rzedna_p_prec,dane_prof0->rkp);
  l_kr=add_000(tekst_p,rzedna_p_poz);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  if (LengthT > 6) TextG.width_factor=0.85f;
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_rzedna_kanalu_p;
 /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  TextG.width_factor=1;

  if (dno_os_p==0)
    {
      if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rkp-rpp),3,koloryp.l_kanal_p,O2Block,O3Dno)) return 0;
      if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rkp-rpp)+(dane_prof.si3),4,koloryp.l_kanal_p,O2Block,O3Dno)) return 0;
    }
  else
   {
     if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rkp-rpp)-(dane_prof.si3/2),3,koloryp.l_kanal_p,O2BlockHatch50,O3Os)) return 0;
     if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rkp-rpp)+(dane_prof.si3/2),4,koloryp.l_kanal_p,O2BlockHatch50,O3Os)) return 0;
   }
  }
  
  if (dane_global.odl==2)  /* typ profilu 2 */
   {
    // zaglebienie kanalu projektowanego
    //jezeli nie jest zdefiniowany kanal projektowany ale jest istniejacy
    //to zaglebienie liczy sie od kanalu istniejacego
    //rzedna_tp=dane_prof0->ru;

    //if ((rzedna_tp==0) && (dane_prof0->rt>0)) rzedna_tp=dane_prof0->rt;
    zaglebienie_ok=FALSE;
    if (dane_prof.op2==0)  //od terenu projektowanego
     {
      if (dane_prof0->ru>0)
       {
         zaglebienie_ok = TRUE;
         zaglebienie_kp = dane_prof0->ru - dane_prof0->rkp;
       }
     }
    else if (dane_prof.op2==1) //od terenu istniejacego
       {
         if (dane_prof0->rt>0)
          {
            zaglebienie_ok = TRUE;
            zaglebienie_kp = dane_prof0->rt - dane_prof0->rkp;
          }
       }
    else zaglebienie_kp = 0;
     
    if (dane_prof0->rkp>0)
     {
//      if(!Lin(dane_prof.odl,24,dane_prof.odl,34,64,koloryp.os_pomiaru)) return 0;
       /*tekst 4  zaglebienie kanalu projektowanego*/
      TextG.x=(float)(dane_prof.odl+del_odl);
      TextG.y=(float)(24.+dt_y);
      if (zaglebienie_ok==TRUE)
      {
        sprintf_prec(tekst_p,zaglebienie_prec,zaglebienie_kp);
        l_kr=add_000(tekst_p,zaglebienie_poz);
      }
      else strcpy(tekst_p,"");
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      if (LengthT > 6) TextG.width_factor=0.85;
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.typ=n_zaglebienie_kanalu_p;
      if (dane_prof.op2==1) TextG.obiektt3=O3Teren_i;
      else TextG.obiektt3=O3Teren_p;
      /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.width_factor=1;
     }
   }
  /**/

  if (dane_prof.ru>0)
  {
   if (((dane_prof0->rki==0) && (dane_prof0->rkp==0)) || (dane_global.odl==0))
   {
    if(!Lin(dane_prof.odl,tablica_y[12]/*60*/,dane_prof.odl,tablica_y[13]/*69*/,64,koloryp.os_pomiaru)) return 0;
   }
  /*tekst 8  rzedne ulicy lub terenu projektowanego*/
  TextG.x=(float)(dane_prof.odl+del_odl);
  TextG.y=(float)(tablica_y[12]/*60*/+dt_y);
//  gcvt(dane_prof0->ru,ndig,tekst_p);
  if (ru_avail==TRUE)
   {
    sprintf_prec(tekst_p,rzedna_tp_prec,dane_prof0->ru);
    l_kr=add_000(tekst_p,rzedna_tp_poz);
   }
  else strcpy(tekst_p,"");
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  if (LengthT > 6) TextG.width_factor=0.85f;
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_rzedna_ulicy;
 /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  TextG.width_factor=1;

  {if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.ru-rpp),5,koloryp.l_ulicy,O2BlockPline,O3Dno)) return 0;}

  }

  if (dane_prof.rt>0)
  {
      if (((dane_prof0->rki==0) && (dane_prof0->rkp==0)) || (dane_global.odl==0))
      {
          if(!Lin(dane_prof.odl,tablica_y[13]/*69*/,dane_prof.odl,tablica_y[14]/*78*/,64,koloryp.os_pomiaru)) return 0;
      }
      /*tekst 8  rzedne terenu lub terenu istniejacego*/
      TextG.x=(float)(dane_prof.odl+del_odl);
      TextG.y=(float)(tablica_y[13]/*69*/+dt_y);
      //  gcvt(dane_prof0->rt,ndig,tekst_p);
      sprintf_prec(tekst_p,rzedna_ti_prec,dane_prof0->rt);
      l_kr=add_000(tekst_p,rzedna_ti_poz);
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      if (LengthT > 6) TextG.width_factor=0.85;
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.obiektt3=O3NormalText;
      TextG.typ=n_rzedna_terenu;
      /*  if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.width_factor=1;

      {if (!Punkt(dane_prof.odl,tablica_y[14]+(dane_prof.rt-rpp),6,koloryp.l_teren_i,O2BlockPline,O3Dno)) return 0;}
  }

   TextG.typ=n_typ_normal;
  
  if (dane_global.odl==2)  /* typ profilu 2 */
  {
      // punkty charakterystyczne  comments_tmp[3]
      if (strlen(comments_tmp[3]) > 0)
      {
          TextG.y=(float)(18.0+dt_y);
          strcpy(&TextG.text[0],comments_tmp[3]);
          LengthT=(int)strlen(TextG.text);
          len_t1=LengthT;
          len_t2=0;
          TextG.dl=LengthT;
          TextG.n=T18+TextG.dl;
          TextG.x=(float)dane_prof.odl;
          TextG.kat= 0;
          TextG.wysokosc=(float)ht2;
          TextG.czcionka=zmwym.czcionka; //0;  //to mozna zmienic
          TextG.kolor=koloryp.spadek_l_fi_mat;
          if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      }
  }


    // opis wezla comments_tmp[0]
    if (strlen(comments_tmp[0]) > 0)
    {
     TextG.y=-10.f;
     strcpy(&TextG.text[0],comments_tmp[0]);
     LengthT=(int)strlen(TextG.text);
     len_t1=LengthT;
     len_t2=0;
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
     TextG.x=(float)dane_prof.odl;
     TextG.kat= 0;
     TextG.wysokosc=3.0f;
     TextG.czcionka=zmwym.czcionka; //0;  //to mozna zmienic
     TextG.kolor=7;
     TextG.justowanie=j_srodkowo;

     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
     TextG.justowanie=j_do_lewej;
     TextG.wysokosc=(float)ht2;
    }
     // opis wezla comments_tmp[1]
    else if (strlen(comments_tmp[1]) > 0)
    {
        TextG.y=-10.f;
        strcpy(&TextG.text[0],comments_tmp[1]);
        LengthT=(int)strlen(TextG.text);
        len_t1=LengthT;
        len_t2=0;
        TextG.dl=LengthT;
        TextG.n=T18+TextG.dl;
        TextG.x=(float)(dane_prof.odl);
        TextG.kat= 0;
        TextG.wysokosc=3.0f;
        TextG.czcionka=zmwym.czcionka; //0;  //to mozna zmienic
        TextG.kolor=7;
        TextG.justowanie=j_srodkowo;

        if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
        TextG.justowanie=j_do_lewej;
        TextG.wysokosc=(float)ht2;
    }
  ///////////////////////////////
	/*dopisanie srednicy i materialu kanalu istniejacego*/

   if (dane_prof0->rki>0)
   {
   //pierwsza srednica
    TextG.y=(float)(11.5+dt_y);
    strcpy(tekst_p,u8"Ø"); //instead of ⌀ due to lack in Arial font
    if (l_srednic_i>0)
    {
     sprintf(tekst_p1,"%g",dane_prof0->si1);

     decimal(tekst_p1);

     strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
     if (dane_prof0->si1>10)
      {
      strcpy(tekst_p1," mm");
      strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
      }
    }
	/*
    if (dane_prof.si2>0)
     {
      strcpy(tekst_p1," ");
      strncat(tekst_p,tekst_p1,1);
      si2i=(int) dane_prof.si2;
      if (si2i < 8)
       {
        strcpy(tekst_p1,material[si2i]);
        strncat(tekst_p,tekst_p1,strlen(tekst_p1));
       }
        else if (si2i==8) //material inny definiowany
        {
          if (strlen(comments_tmp[2]) > 0)
           {
             strcpy(tekst_p1,comments_tmp[2]);
             if (strcmp(tekst_p1,"?")!=0) strncat(tekst_p,tekst_p1,strlen(tekst_p1));
           }
        }
     }
	 */

    strcpy(&TextG.text[0],tekst_p);

    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
  //  TextG.x=dane_prof_odl_ki-((dane_prof_odl_ki-dane_profs.spi3)*0.5);
    TextG.x=(float)(dane_prof.odl+11.);
    TextG.kat= 0;
    TextG.wysokosc=(float)ht2;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.justowanie=j_do_lewej;
    TextG.typ=n_srednica_kanalu_i;
    TextG.kolor=koloryp.spadek_l_fi_mat;
	TextG.ukryty=1;
    //    if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

/* dopisanie srednicy i materialu kanalu projektowanego*/

   if (dane_prof0->rkp)
   {
   TextG.y=(float)(tablica_y[8]+dt_y);
   strcpy(tekst_p,"");
   if (l_srednic_p>0)
    {
     TextG.y=(float)(tablica_y[8]+dt_y);
     strcpy(tekst_p,u8"Ø"); //instead of ⌀ due to lack in Arial font
     sprintf(tekst_p1,"%g",dane_prof0->si3);

     decimal(tekst_p1);

     strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
     if (dane_prof0->si3>10)
      {
      strcpy(tekst_p1," mm");
      strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
      }
    }
   /*
     if (dane_prof.oi1>0)
      {
       strcpy(tekst_p1," ");
       strncat(tekst_p,tekst_p1,1);
       oi1i=(int) dane_prof.oi1;
       if (oi1i < 8)
       {
         strcpy(tekst_p1,material[oi1i]);
         strncat(tekst_p,tekst_p1,strlen(tekst_p1));
       }
       else if (oi1i==8) //material inny definiowany
        {
          if (strlen(comments_tmp[2]) > 0)
           {
             strcpy(tekst_p1,comments_tmp[2]);
             if (strcmp(tekst_p1,"?")!=0) strncat(tekst_p,tekst_p1,strlen(tekst_p1));
           }
        }
      }
  */

     strcpy(&TextG.text[0],tekst_p);

     LengthT=(int)strlen(TextG.text);
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
 //    TextG.x=dane_prof_odl_kp-((dane_prof_odl_kp-dane_profs.opi1)*0.5);
     TextG.x=(float)(dane_prof.odl+1.);
     TextG.kat= 0;
     TextG.wysokosc=(float)ht2;
     TextG.czcionka=zmwym.czcionka; //0;
     TextG.justowanie=j_do_lewej;
     TextG.typ=n_srednica_kanalu_p;
     TextG.kolor=koloryp.spadek_l_fi_mat;
	 TextG.ukryty=1;
     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

  strcpy(&TextG.text[0],"");
  TextG.ukryty=0;

  ///////////////////////////////

  if (dane_global.odl==0) max_r=dane_prof.rki; else max_r=0; 
  if (dane_prof.rkp>max_r) max_r=dane_prof.rkp;
  if (dane_prof.ru>max_r) max_r=dane_prof.ru;
  if (dane_prof.rt>max_r) max_r=dane_prof.rt;

  if(!Lin(dane_prof.odl,tablica_y[14]/*78*/,dane_prof.odl,tablica_y[14]/*78*/+(max_r-rpp),32,koloryp.os_pomiaru)) return 0;

  memmove(&TextG, &textg, sizeof(TEXT));
  strcpy(&TextG.text[0],"");

#ifdef SAVETMP
  //zapisanie danych do pliku binarnego
#ifndef LINUX
    i_f_handle=open(PROFIL_TEMP_FILE, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    i_f_handle=open(PROFIL_TEMP_FILE, O_CREAT|O_TRUNC|O_RDWR, 0666);
#endif

  if (i_f_handle == -1)
  {
  ErrList(103);
  return 0;
  }
  /**/
  memcpy(&dane_prof00, dane_prof0,sizeof(MyDane));

  if (dane_prof.rki>0) dane_prof00.spi3=dane_prof00.odl;
  else
     {
      dane_prof00.rki=0;
      dane_prof00.spi3=0;
     }

  if (dane_prof.rkp>0) dane_prof00.opi1=dane_prof00.odl;
  else
     {
      dane_prof00.rkp=0;
      dane_prof00.opi1=0;
     }

  if (dane_prof.ru>0) dane_prof00.opi2=dane_prof00.odl;
  else
     {
      dane_prof00.ru=0;
      dane_prof00.opi2=0;
     }

  if (dane_prof.rt>0) dane_prof00.opi3=dane_prof00.odl;
  else
     {
      dane_prof00.rt=0;
      dane_prof00.opi3=0;
     }

  
//  dane_prof0->spi3=dane_prof0->odl;

//  dane_prof0->opi1=dane_prof0->odl;

//  dane_prof0->opi2=dane_prof0->odl;

//  dane_prof0->opi3=dane_prof0->odl;

//  dane_prof0->spp1=max_r;

//  dane_profs1=*dane_prof0;
//  memmove(&dane_profs1, dane_prof0, sizeof(MyDane));

  if (write (i_f_handle, &dane_prof00, sizeof(MyDane)) != sizeof(MyDane))
  {
   close (i_f_handle) ;
   ErrList(104);
   return 0;
  }

    close (i_f_handle) ;
#endif

    //zaznaczenie ze nastapil ostatni zapis pomiaru
    // przyjeto ze znacznikiem aplikacji dla profilu jest wartosc 101
    l_kr=put_znacznik_aplikacji(101);

  return 1;
}


static int Profil4H(void *BufVar)           /*HEKTOMETRY*/
{ const double ht3=3.5;
  const double dt_y=0.8;
  MyDane *dane_prof0=(MyDane*)BufVar;
  ////MyDane dane_prof;
  ////MyDane dane_global;

  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  int l_kr;
  double del_odl;
  int l_k, l_h;
  int i;

     double rpp;
     double odl_0;
     double odl_1;

  /***************************/

  dane_global.oi2=0;  /*dynamika bloku*/
  l_kr=put_dynamic_block(dane_global.oi2);

  dane_global.ru=get_skala_profilu_x();
  dane_global.rt=get_poziom_pp();
  dane_global.odl=get_typ_profilu();

  rpp=jednostkiOb(dane_global.rt);
  /*zdefiniowane sa dwie wartosci:*/
  /* dane_prof.odl  - wartosc poczatkowa hektometrow*/
  /* dane_prof.rki  - wartosc koncowa hektometrow*/
  /* pozostale warosci nie sa uzywane*/
  /* wartosc poczatkowa >0 uzywana jest w przypadku, gdy rysowany jest */
  /* kolejny arkusz */
  /* wartosc koncowa inicjowana jest poczatkowo w taki sposob aby albo */
  /* zapelnic szerokosc aktualnego formatu arkusza pomniejszonego o szerokosc */
  /* pola opisowego tabeli. Wartosc ta przyjmowana jest jako stala */
  /* a w przypadku, gdy punkt lokalny <0,0> przesuniety jest wzgledem 0,0 */
  /* wartosc ta moze byc przyjeta jako przesuniecie dx0 + 2 cm na prawy margines */
  /* kolejne (ew. dodatkowe) segmenty hektometrow beda dokladane przy dodawaniu */
  /* kolejnego pomiaru, jezeli wartosc ->odl przekroczy wartosc koncowa */
  /* dopuszcza sie podanie wartosci koncowej = 0. Wtedy jeszeli wartosc poczatkowa */
  /* >0 to wartosc koncowa przyjmuje wartosc poczatkowa. Wymusza to dokladanie */
  /* dodatkowych segmentow przy kolejnych pomiarach. W tym przypadku wstawiana */
  /* jest tylko jedna wartosc poczatkowa hektometrow */
  /* obydwie wartosci sa pamietane w rysunku jako zmienne globalne */
  /* hektometr_p, hektometr_k, sa one pamietane jako wartosci oryginalne (prof0) */

  dane_prof.odl=(jednostkiOb(dane_prof0->odl)/dane_global.ru);
  dane_prof.rki=(jednostkiOb(dane_prof0->rki)/dane_global.ru);

  if (dane_global.odl==1)
   {
     //zapamietanie nowych danych o hektometrach
     return 0; /* w profilu technologicznym nie sa rysowane hektometry */
   }

  if (dane_prof.odl<0)
  {
   ErrList(109);
   return 0;
   }

  if ((int)dane_prof.rki<(int)dane_prof.odl)
  {
   ErrList(110);
   return 0;
   }

   //zapamietanie nowych danych o hektometrach
   l_kr=put_hektometr(dane_prof0->odl,0);
   l_kr=put_hektometr(dane_prof0->rki,1);

  return 1;
}


static int ProfilXH(void *BufVar)        /*SKALA*/
{ MyDane *dane_prof0=(MyDane*)BufVar;
  ////MyDane dane_prof;
  ////MyDane dane_global;

  int l_kr;
  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  int ii;
  double skala_profilu0, poziom_pp0;
  TEXT textg;
  BOOL kk;

  /***************************/
  memmove(&textg, &TextG, sizeof(TEXT));

  dane_global.oi2=0;  /*dynamika bloku*/
  l_kr=put_dynamic_block(dane_global.oi2);

  dane_prof.ru =get_skala_profilu_x();
  dane_prof.rt =get_poziom_pp();

  /*w bloku skala_1  definiuje sie warosc skalax/skalay  , zwykle 5*/
  dane_prof.odl=dane_prof0->odl;     /*stosunek skali x do y*/
  dane_prof.rki=dane_prof0->rki;     /*wartosc poczatkowa rzednej rpp*/
//  dane_prof.rkp=dane_prof0->rkp;     /* typ profilu 0 - kanalizacyjny, 1 - technologiczny */

  dane_prof.ru=dane_prof0->ru;
  dane_prof.rt=dane_prof0->rt;

  if (dane_prof.odl<=0)
   {
    ErrList(106);
    return 0;
   }

//   l_kr=put_typ_profilu(dane_prof.rkp);
   dane_global.odl =get_typ_profilu();

   tablica_0_1(dane_global.odl);

   parametry_ogolne_tekstu(0, 4,koloryp.rzedne);

   if ((dane_global.odl==0) || (dane_global.odl==2)) TextG.x=-46; else TextG.x=-66;
   TextG.y=(float)tablica_y[15]/*81*/;
   strcpy(tekst_p,_DATUM_LEVEL_);
   //gcvt(dane_prof.rki,ndig,tekst_p1);
   sprintf(tekst_p1,"%.1f",dane_prof.rki);
   l_kr=add_000(tekst_p1,1);
   strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
   strcpy(tekst_p1,_m_n_p_m_);
   strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
   strcpy(&TextG.text[0],tekst_p);
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.obiektt2=O2BlockHatch25;
   TextG.blok=NoElemBlok;

   ADP=NULL; ADK=NULL;
   kk=get_texts_hatch_cur_layer (&ADP, &ADK);
   if ((ADP!=NULL) && (ADK!=NULL)) delete_block();

   if (dodaj_obiekt( NULL, &TextG)==NULL) return 0;

  /*zapamietanie starych danych*/
  skala_profilu0=get_skala_profilu_x();
  poziom_pp0=get_poziom_pp();

  /*zpisanie nowych danych*/
  l_kr=put_skala_profilu_x(dane_prof.odl);

  l_kr=put_poziom_pp(dane_prof.rki);

  memmove(&TextG, &textg, sizeof(TEXT));
  return 1;       /*opuszczenie funkcji ustawiania bloku*/
}


//char *tab_name[]={"tabela1.alx","tabela2.alx","tabela3.alx",""};

static int Profil_New(void *BufVar)        /*Nowy profil: SKALA i HEKTOMETRY*/
{ MyDane *dane_prof0=(MyDane*)BufVar;
  ////MyDane dane_prof;
  ////MyDane dane_global;

  int l_kr;
  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  int ii;
  double skala_profilu0, poziom_pp0;
  TEXT textg;
  double X0,Y0;
  char buf [300]; //[100];
  int object_no ;
  double xx, yy;
  char typ_bloku [30] = "";
  char blockname[MAXPATH];

  /***************************/
  memmove(&textg, &TextG, sizeof(TEXT));

  dane_global.oi2=1;  /*dynamika bloku*/
  l_kr=put_dynamic_block(dane_global.oi2);

//  dane_prof.ru =get_skala_profilu_x();
//  dane_prof.rt =get_poziom_pp();

  /*w bloku skala_1  definiuje sie warosc skalax/skalay  , zwykle 5*/
  dane_prof.odl=dane_prof0->odl;     /*stosunek skali x do y*/
  dane_prof.rki=dane_prof0->rki;     /*wartosc poczatkowa rzednej rpp*/
  dane_prof.rkp=dane_prof0->rkp;     /* typ profilu 0 - kanalizacyjny, 1 - technologiczny */

  if (dane_prof.rkp<0) dane_prof.rkp=0;
  if (dane_prof.rkp>2) dane_prof.rkp=2;

  if (dane_prof.odl<=0)
   {
   ErrList(106);
   return 0;
   }

   dane_prof.ru=dane_prof0->ru;  //pierwszy hektometr
   dane_prof.rt=dane_prof0->rt;  //ostatni hektometr

   if (dane_prof.ru<0)
    {
     ErrList(109);
     return 0;
    }

   if ((int)dane_prof.rt<(int)dane_prof.ru)
    {
     ErrList(110);
     return 0;
    }

   l_kr=put_typ_profilu(dane_prof.rkp);
   dane_global.odl =get_typ_profilu();

   //import bloku tabeli pomiarowej
   sprintf(blockname,"%s%s%s.alx",bloki_dir,SSlash,tab_name[(int)dane_prof.rkp]);

   if(ReadBlock_(blockname,&X0,&Y0,&ADP,&ADK , buf, MaxLen - 10, &object_no, TRUE)) return 0;
   transformacja_blok(ADP,ADK,-X0,-Y0,0,0,Tprzesuw,0);
   zmien_atrybut(ADP,ADK,Ablok,Aoblok);

   if (!PointS_b(dane, 0, 0)) return 0;
   ///////

   tablica_0_1(dane_global.odl);

   parametry_ogolne_tekstu(0, 4,koloryp.rzedne);

   if (dane_global.odl==0 || dane_global.odl==2) TextG.x=-46; else TextG.x=-66;
   TextG.y=(float)tablica_y[15]/*81*/;
   strcpy(tekst_p,_DATUM_LEVEL_);
   //gcvt(dane_prof.rki,ndig,tekst_p1);
   sprintf(tekst_p1,"%.1f",dane_prof.rki);
   l_kr=add_000(tekst_p1,1);
   strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
   strcpy(tekst_p1,_m_n_p_m_);
   strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
   strcpy(&TextG.text[0],tekst_p);
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.obiektt2=O2BlockHatch25;
   TextG.blok=NoElemBlok;

   ADP=NULL; ADK=NULL;
   get_texts_hatch_cur_layer (&ADP, &ADK);
   if ((ADP!=NULL) && (ADK!=NULL)) delete_block();

   if (dodaj_obiekt( NULL, &TextG)==NULL) return 0;

   //dodanie bloku atrybutu
   strcpy(typ_bloku, "$ATRYBUT");
   xx=0.0;
   yy=0.0;

   if (FALSE == add_block_with_type (xx, yy, typ_bloku, 0, B_EXPORT))
    {
     return 0;
    }

    yy=TextG.y+20.0;

    //symbol atrybutu
    yy-=2.5;
    TextG.y=(float)yy;
    TextG.kat=0;
    TextG.wysokosc=2;
    TextG.italics=0;
    TextG.width_factor=1;
    TextG.warstwa=Current_Layer;
    TextG.kolor=40; //7;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.typ=n_typ_atrybut;
    TextG.justowanie=j_do_lewej;
    TextG.ukryty=0;
    TextG.obiektt1 = 0;
    TextG.obiektt2 = O2BlockSpecial;
    TextG.blok=1;
  
    sprintf(tekst_p,"%s #%d",_Profil_,Current_Layer);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

     TextG.kolor=7; //40;
     //kolejne wiersze
     TextG.typ=n_typ_komentarz;
     TextG.ukryty=1;

     yy-=2.5;
     TextG.y=(float)yy;
  
     strcpy(tekst_p,"%S=");

     sprintf(tekst_p1,"%.1f",dane_prof.odl);
     strcat(tekst_p,tekst_p1);
     strcpy(&TextG.text[0],tekst_p);
     LengthT=(int)strlen(TextG.text);
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
  
     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

     yy-=2.5;
     TextG.y=(float)yy;

     strcpy(tekst_p,"%PP=");
     sprintf(tekst_p1,"%.1f",dane_prof.rki);
     strcat(tekst_p,tekst_p1);
     strcpy(&TextG.text[0],tekst_p);
     LengthT=(int)strlen(TextG.text);
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
  
     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

     yy-=2.5;
     TextG.y=(float)yy;

     strcpy(tekst_p,"%T=");
     sprintf(tekst_p1,"%d",(int)dane_prof.rkp);  //integer ?
     strcat(tekst_p,tekst_p1);
     strcpy(&TextG.text[0],tekst_p);
     LengthT=(int)strlen(TextG.text);
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
  
     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

     yy-=2.5;
     TextG.y=(float)yy;

     strcpy(tekst_p,"%h=");
     sprintf(tekst_p1,"%d",(int)dane_prof0->ru);  //integer ?
     strcat(tekst_p,tekst_p1);
     strcpy(&TextG.text[0],tekst_p);
     LengthT=(int)strlen(TextG.text);
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
  
     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

     yy-=2.5;
     TextG.y=(float)yy;

     strcpy(tekst_p,"%H=");
     sprintf(tekst_p1,"%d",(int)dane_prof0->rt);  //integer ?
     strcat(tekst_p,tekst_p1);
     strcpy(&TextG.text[0],tekst_p);
     LengthT=(int)strlen(TextG.text);
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;
  
     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

   TextG.ukryty=0;
   ///////////

   /*zapamietanie starych danych*/
   skala_profilu0=get_skala_profilu_x();
   poziom_pp0=get_poziom_pp();

   /*zpisanie nowych danych*/
   l_kr=put_skala_profilu_x(dane_prof.odl);

   l_kr=put_poziom_pp(dane_prof.rki);

   //zapamietanie nowych danych o hektometrach
   l_kr=put_hektometr(dane_prof0->ru,0);
   l_kr=put_hektometr(dane_prof0->rt,1);

   l_kr=generowanie_bloku_hektometrow(dane_prof.ru, dane_prof.odl, dane_prof.rt, 0, 0);

   memmove(&TextG, &textg, sizeof(TEXT));

   return 1;       /*opuszczenie funkcji ustawiania bloku*/
}

void delete_instal_tab(void)
{ int i,j;

  for (i=0; i<20; i++)
   {
    data_instal_last[i]=0;
   }

//  for (i=0; i<6; i++)
//   {
//    data_instal_tab[0][i]=0;
//   }

  for (i=1; i<20; i++)
   {
    //memmove(&data_instal_tab[i],&data_instal_tab[0], sizeof(double) * 6);
    for (j=0; j<6; j++) data_instal_tab[i][j]=0;
   }
}

static void set_new_instal_tab(double rzedna_terenu_p, double rzedna_terenu_i)
{ int l_i;
  double data_instal_buf[10][6];
  int i,j;
  //ustalanie nowych rekordow tabeli
  l_i=0;
  if (data_instal_last[1]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=1;                      //Ks
     data_instal_buf[l_i-1][2]=milimetryob(rzedna_terenu_i); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(rzedna_terenu_p); //rzedna terenu proj.
     data_instal_buf[l_i-1][4]=data_instal_last[1];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[2];    //srednica
   }

  if (data_instal_last[3]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=2;                      //Ko
     data_instal_buf[l_i-1][2]=milimetryob(rzedna_terenu_i); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(rzedna_terenu_p); //rzedna terenu proj.
     data_instal_buf[l_i-1][4]=data_instal_last[3];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[4];    //srednica
   }

  if (data_instal_last[5]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=3;                      //Kd
     data_instal_buf[l_i-1][2]=milimetryob(rzedna_terenu_i); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(rzedna_terenu_p); //rzedna terenu proj.
     data_instal_buf[l_i-1][4]=data_instal_last[5];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[6];    //srednica
   }

  if (data_instal_last[7]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=4;                      //W
     data_instal_buf[l_i-1][2]=milimetryob(rzedna_terenu_i); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(rzedna_terenu_p); //rzedna terenu proj.
     data_instal_buf[l_i-1][4]=data_instal_last[7];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[8];    //srednica
   }

  if (data_instal_last[9]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=5;                      //G
     data_instal_buf[l_i-1][2]=milimetryob(rzedna_terenu_i); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(rzedna_terenu_p); //rzedna terenu proj.
     data_instal_buf[l_i-1][4]=data_instal_last[9];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[10];    //srednica
   }

  if (data_instal_last[11]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=6;                      //E
     data_instal_buf[l_i-1][2]=milimetryob(rzedna_terenu_i); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(rzedna_terenu_p); //rzedna terenu proj.
     data_instal_buf[l_i-1][4]=data_instal_last[11];    //rzedna
     data_instal_buf[l_i-1][5]=0;    //srednica
   }

  if (data_instal_last[12]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=7;                      //kT
     data_instal_buf[l_i-1][2]=milimetryob(rzedna_terenu_i); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(rzedna_terenu_p); //rzedna terenu proj.
     data_instal_buf[l_i-1][4]=data_instal_last[12];    //rzedna
     data_instal_buf[l_i-1][5]=0;    //srednica
   }

  if (data_instal_last[13]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=8;                      //T
     data_instal_buf[l_i-1][2]=milimetryob(rzedna_terenu_i); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(rzedna_terenu_p); //rzedna terenu proj.
     data_instal_buf[l_i-1][4]=data_instal_last[13];    //rzedna
     data_instal_buf[l_i-1][5]=0;    //srednica
   }

  //kopiowanie do tablicy pomiarow
  if (l_i>0)
   {
    for (i=0; i<20-(l_i); i++)
     {
      //memmove(&data_instal_tab[i+l_i],&data_instal_tab[i], sizeof(double) * 6);
       for (j=0; j<6; j++) data_instal_tab[19-i][j]=data_instal_tab[19-i-l_i][j];
     }

    for (i=0; i<l_i; i++)
     {
      //memmove(&data_instal_tab[i],&data_instal_buf[i], sizeof(double) * 6);
      for (j=0; j<6; j++) data_instal_tab[i][j]=data_instal_buf[i][j];
     }
   }
}
/////////////////////


static int Profil3H(void *BufVar)    /*INSTALACJE*/
{ const float ht1=2.5f /*2.25*/;
  const float ht2=2.5f;
  const float ht3=3.5f;
  const double dt_y=0.8;
  MyDane *dane_prof0=(MyDane*)BufVar;
  ////MyDane dane_prof;
  ////MyDane dane_global;

  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  int l_kr;
  unsigned u_ell_size ;

     double rpp;
     double max_r;

     double rura_x, rura_y, rura_dx, rura_dy, rura_r;
     double sp1a, sp2a, sp3a, oi2a, si3a, op2a;

  
  int odl_i;
  double odl_1;
  double odl_0;
  double odl_r;
  double del_odl;

  double reszta;
  int reszta_i;
  int i, j, l_h, l_k, ii;
//  int dane_oi1;
//  char str1[80];
  /***************************/
 long dane_size0_0, dane_size0_01, dane_size0_00;
 TEXT textg;
 double yi;
 double local_x, local_y;
 double rzedna_terenu_i;
 char Ks_Co_Cok[15], Ks_Co_Cok1[15];
 double rkp;
// int l_i;
// double data_instal_buf[10][6];

  memmove(&textg, &TextG, sizeof(TEXT));
  
  dane_global.ru=get_skala_profilu_x();
  dane_global.rt=get_poziom_pp();
  dane_global.odl=get_typ_profilu();
  local_x=get_localx();
  local_y=get_localy();

  tablica_0_1(dane_global.odl);

  rpp=jednostkiOb(dane_global.rt);

  dane_global.rki=get_hektometr_p();
  dane_global.rkp=get_hektometr_k();

  dane_global.oi2=0;  /*dynamika bloku*/
  l_kr=put_dynamic_block(dane_global.oi2);

  /*interpretacja danych:
  odl=odleglosc                       [m]
  rki=rzedna dna kanalu sanitarnego lub Co lub Cok
  rkp=srednica kanalu sanitarnego     [m]
  ru=rzedna dna kanalu ogolnosplawnego
  rt=srednica kanalu ogolnosplawnego  [m]
  si1=rzedna dna kanalu deszczowego
  si2=srednica kanalu deszczowego  [m]
  si3=rzedna wodociagu
  oi1=srednica wodociagu           [mm]
  oi2=rzedna rurociagu gazowego
  oi3=srednica rurociagu gazowego  [mm]
  sp1=rzedna kabla elektrycznego (lub 1 + rzedna terenu)
  sp2=rzedna kanalizacji telefonicznego (lub 1 + rzedna terenu)
  sp3=rzedna kabla telefonicznego (lub 1 + rzedna terenu)
  op1=opcjonalnie rzedna terenu (jezeli podano wartosc 1 dla "e","kT" lub "T")
  op2=instalacje inne
  */

  dane_prof.odl=(jednostkiOb(dane_prof0->odl-(dane_global.rki*100.))/dane_global.ru);
  dane_prof.rki=jednostkiOb(dane_prof0->rki);   /*Ks*/

  if (dane_prof0->spi2==2)
    {
     strcpy(Ks_Co_Cok,"Cok");
    }
  else if (dane_prof0->spi2==1)
     {
       strcpy(Ks_Co_Cok,"Co");
     }
  else
      {
        strcpy(Ks_Co_Cok,"Ks");
      }

  dane_prof.rkp=jednostkiOb(dane_prof0->rkp)/2.; /* [m] */

  if (dane_prof0->rkp>10.)
   {
   dane_prof.rkp/=1000.;
   }

  dane_prof.ru=jednostkiOb(dane_prof0->ru);     /*Ko*/
  dane_prof.rt=jednostkiOb(dane_prof0->rt)/2;   /* [m] */
  if (dane_prof0->rt>10.)
   {
    dane_prof.rt/=1000.;
   }

  dane_prof.si1=jednostkiOb(dane_prof0->si1);   /*Kd*/
  dane_prof.si2=jednostkiOb(dane_prof0->si2)/2.;  /* [m] */
  if (dane_prof0->si2>10.)
   {
   dane_prof.si2/=1000.;
   }
  dane_prof.si3=jednostkiOb(dane_prof0->si3);   /*W*/
  dane_prof.oi1=jednostkiOb(dane_prof0->oi1)/2000.;   /* [mm] */
  dane_prof.oi2=jednostkiOb(dane_prof0->oi2);   /*G*/
  dane_prof.oi3=jednostkiOb(dane_prof0->oi3)/2000.;   /* [mm] */

  dane_prof.sp1=jednostkiOb(dane_prof0->sp1);   /*e*/
  dane_prof.sp2=jednostkiOb(dane_prof0->sp2);   /*kT*/
  dane_prof.sp3=jednostkiOb(dane_prof0->sp3);   /*T*/
  dane_prof.op2=jednostkiOb(dane_prof0->op2);   /*inne*/

  dane_prof.op1=jednostkiOb(dane_prof0->op1);   /* rzedne terenu dla potrzeb e,kT,T */

  dane_prof.op3=jednostkiOb(dane_prof0->op3)/dane_global.ru;   /*odleglosc od ostatniego hektometru*/
  dane_prof.odl+=dane_prof.op3;

/*  if (dane_prof.odl==0)
  {
   return 0;
   }
*/
   if ((dane_prof0->odl + dane_prof0->op3)<=0)
   {
    ErrList(129);
    return 0;
   }

//   if (dane_prof0->odl<=(dane_global.rki*100))
//   {
//    ErrList(131);
//    return 0;
//   }

  dane_global.oi1=((dane_prof0->odl + dane_prof0->op3) / 100.);
  if (dane_global.oi1<dane_global.rki)
   {
   ErrList(128);
   return 0;
   }

   sp1a=0;
   sp2a=0;
   sp3a=0;
   oi2a=0;
   si3a=0;
   op2a=0;

  if (dane_prof.op1==0)
   {
    //interpolacja rzednej terenu
    l_kr=find_interpolated_tp_ti(dane_prof.odl+local_x, &yi);
    if (l_kr==0)
     {
      //ErrList(125);
      //return 0;
      dane_prof.op1=0;
     }
    else dane_prof.op1=jednostkiOb(milimetryob(yi - local_y - tablica_y[14]) + dane_global.rt);
   }

   l_kr=find_interpolated_ti(dane_prof.odl+local_x, &yi);
    if (l_kr==0)
     {
      rzedna_terenu_i=0;
     }
    else rzedna_terenu_i=jednostkiOb(milimetryob(yi - local_y - tablica_y[14]) + dane_global.rt);
 


  data_instal_last[0]=dane_prof0->odl+dane_prof0->op3;
  data_instal_last[1]=dane_prof0->rki;
  data_instal_last[2]=dane_prof0->rkp;
  data_instal_last[3]=dane_prof0->ru;
  data_instal_last[4]=dane_prof0->rt;
  data_instal_last[5]=dane_prof0->si1;
  data_instal_last[6]=dane_prof0->si2;
  data_instal_last[7]=dane_prof0->si3;
  data_instal_last[8]=dane_prof0->oi1;
  data_instal_last[9]=dane_prof0->oi2;
  data_instal_last[10]=dane_prof0->oi3;
  data_instal_last[11]=dane_prof0->sp1;
  data_instal_last[12]=dane_prof0->sp2;
  data_instal_last[13]=dane_prof0->sp3;
  data_instal_last[14]=dane_prof0->op1;  //rzedna terenu istniejacego
  data_instal_last[15]=dane_prof0->op2;
  data_instal_last[16]=0;  //odleglosc od ostatniego hektometra
  data_instal_last[17]=rzedna_terenu_i;  //rzedna terenu istniejacego

  set_new_instal_tab(dane_prof.op1, rzedna_terenu_i);
/*
  //ustalanie nowych rekordow tabeli
  l_i=0;
  if (data_instal_last[1]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=1;                      //Ks
     data_instal_buf[l_i-1][2]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][4]=data_instal_last[1];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[2];    //srednica
   }

  if (data_instal_last[3]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=2;                      //Ko
     data_instal_buf[l_i-1][2]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][4]=data_instal_last[3];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[4];    //srednica
   }

  if (data_instal_last[5]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=3;                      //Kd
     data_instal_buf[l_i-1][2]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][4]=data_instal_last[5];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[6];    //srednica
   }

  if (data_instal_last[7]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=4;                      //W
     data_instal_buf[l_i-1][2]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][4]=data_instal_last[7];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[8];    //srednica
   }

  if (data_instal_last[9]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=5;                      //G
     data_instal_buf[l_i-1][2]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][4]=data_instal_last[9];    //rzedna
     data_instal_buf[l_i-1][5]=data_instal_last[10];    //srednica
   }

  if (data_instal_last[11]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=6;                      //E
     data_instal_buf[l_i-1][2]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][4]=data_instal_last[11];    //rzedna
     data_instal_buf[l_i-1][5]=0;    //srednica
   }

  if (data_instal_last[12]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=7;                      //kT
     data_instal_buf[l_i-1][2]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][4]=data_instal_last[12];    //rzedna
     data_instal_buf[l_i-1][5]=0;    //srednica
   }

  if (data_instal_last[13]>0)
   { l_i++;
     data_instal_buf[l_i-1][0]=data_instal_last[0];    //hektometr
     data_instal_buf[l_i-1][1]=8;                      //T
     data_instal_buf[l_i-1][2]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][3]=milimetryob(dane_prof.op1); //rzedna terenu istn.
     data_instal_buf[l_i-1][4]=data_instal_last[13];    //rzedna
     data_instal_buf[l_i-1][5]=0;    //srednica
   }

  //kopiowanie do tablicy pomiarow
  if (l_i>0)
   {
    for (i=0; i<20-(l_i); i++)
     {
      //memmove(&data_instal_tab[i+l_i],&data_instal_tab[i], sizeof(double) * 6);
       for (j=0; j<6; j++) data_instal_tab[19-i][j]=data_instal_tab[19-i-l_i][j];
     }

    for (i=0; i<l_i; i++)
     {
      //memmove(&data_instal_tab[i],&data_instal_buf[i], sizeof(double) * 6);
      for (j=0; j<6; j++) data_instal_tab[i][j]=data_instal_buf[i][j];
     }
   }
*/

/*  data_instal_last[0]=dane_prof0->odl+dane_prof0->op3;
  data_instal_last[1]=dane_prof0->rki;
  data_instal_last[2]=dane_prof0->rkp;
  data_instal_last[3]=dane_prof0->ru;
  data_instal_last[4]=dane_prof0->rt;
  data_instal_last[5]=dane_prof0->si1;
  data_instal_last[6]=dane_prof0->si2;
  data_instal_last[7]=dane_prof0->si3;
  data_instal_last[8]=dane_prof0->oi1;
  data_instal_last[9]=dane_prof0->oi2;
  data_instal_last[10]=dane_prof0->oi3;
  data_instal_last[11]=dane_prof0->sp1;
  data_instal_last[12]=dane_prof0->sp2;
  data_instal_last[13]=dane_prof0->sp3;
  data_instal_last[14]=dane_prof0->op1;
  data_instal_last[15]=dane_prof0->op2;
  data_instal_last[16]=0;  //odleglosc od ostatniego hektometra
  data_instal_last[17]=dane_prof0->op1;  //rzedna terenu projektowanego
*/

  /* kabel */
  if (dane_prof.sp1>0)
   {
   if (dane_prof0->sp1==1)
    {
     sp1a=dane_prof.op1-jednostkiOb(0.8);
    }
   else sp1a=dane_prof.sp1;
   }

  /* kanalizacja telefoniczna */
  if (dane_prof.sp2>0)
   {
   if (dane_prof0->sp2==1)
    {
     sp2a=dane_prof.op1-jednostkiOb(0.8);
    }
   else sp2a=dane_prof.sp2;
   }
  /* kabel telefoniczny */
  if (dane_prof.sp3>0)
   {
   if (dane_prof0->sp3==1)
    {
     sp3a=dane_prof.op1-jednostkiOb(0.8);
    }
   else sp3a=dane_prof.sp3;
   }

  /* instalacje inne */
  if (dane_prof.op2>0)
   {
   if (dane_prof0->op2==1)
    {
     op2a=dane_prof.op1;
    }
   else op2a=dane_prof.op2;
   }

   /* gaz */
  if (dane_prof.oi2>0)
   {
   if (dane_prof0->oi2==1)
    {
     oi2a=dane_prof.op1-jednostkiOb(1.2);
    }
   else oi2a=dane_prof.oi2;
   }

   /* woda */
  if (dane_prof.si3>0)
   {
   if (dane_prof0->si3==1)
    {
     si3a=dane_prof.op1-jednostkiOb(1.6);
    }
   else si3a=dane_prof.si3;
   }

 if ((dane_global.odl==0) || (dane_global.odl==2))
 {
/* generowanie (ewentualne) bloku hektometrow */

   /* obliczenie nowej wartosci hektometrow si3 */
   /*zaokragloenie wartosci odleglosci w gore, do pelnego hektometra */

    dane_global.oi1 = ceil((dane_prof0->odl + dane_prof0->op3) / 100);
//    dane_oi1 = (int) (dane_prof0->odl / 100) + 0.5;
//    dane_global.oi1 = dane_oi1;
  /*  */
  
//  sprintf(str1,"%.3f  %.3f  %.3f",dane_global.rkp, dane_global.ru, dane_global.oi1);
//  komunikat_str(str1);
//  Get_Key();

  dane_global.si1=(jednostkiOb(dane_global.rki)/dane_global.ru);
  dane_global.si2=(jednostkiOb(dane_global.rkp)/dane_global.ru);  /* stare */
  dane_global.si3=(jednostkiOb(dane_global.oi1)/dane_global.ru);  /* nowe */

  if (dane_global.si3>dane_global.si2)
   {
   l_kr=generowanie_bloku_hektometrow(dane_global.rkp, dane_global.ru, dane_global.oi1, 0, 1);
   if (l_kr==0) return 0;
   /*
   if (FALSE == add_block (B_PLINE, 0))
   {
    return 0;
   }

   TextG.kat= 0;
   TextG.wysokosc=ht3;
   TextG.italics=0;
   TextG.width_factor=1;
   TextG.czcionka=zmwym.czcionka; //1;
   TextG.obiektt1 = 0;
   TextG.obiektt2 = O2BlockPline;;
   TextG.blok=1;

   odl_0=0;
   odl_1=jednostkiOb(dane_global.rkp*100)/dane_global.ru;
   del_odl=0;

   for (i=int(dane_global.rkp); i<int(dane_global.oi1); i++)
   {
    odl_0=odl_1;
    odl_1=(jednostkiOb(100)/dane_global.ru)+odl_0;

    if(!Lin(odl_0+1,0,odl_1-1,0,5,7)) return 0;

    if(!Okr(odl_1,0,1,7)) return 0;

    TextG.x=odl_1+del_odl;
    TextG.y=-5.5;

    l_h = (i+1) % 10 ;
    l_k = (i+1) / 10 ;

    if (l_h==0)
      {
       if(!Okr(odl_1,0,0.5,7)) return 0;
      }

    if (l_k>0)
     {
      gcvt(l_k,ndig,tekst_p);
      strncat(tekst_p,"+",1);
     }
      else strcpy(tekst_p,"");

    gcvt(l_h,ndig,tekst_p1);
    strncat(tekst_p,tekst_p1,strlen(tekst_p1));
    strcpy(&TextG.text[0],tekst_p);
    LengthT=strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

  strcpy(&TextG.text[0],"");
  dane_global.rkp=dane_global.oi1;
  l_kr=put_hektometr(dane_global.rkp,1);
  */

   }
 }  /* typ profilu 0 */
/* **** */

  dane_size0_0=dane_size;

  if (FALSE == add_block (B_INSTALACJE_OPIS, 0))
   {
    return 0;
   }

  parametry_ogolne_tekstu(Pi/2, ht1,koloryp.rzedne);
/*  TextG.kat= Pi / 2;
  TextG.wysokosc=ht1;
  TextG.italics=0;
  TextG.width_factor=1;
  TextG.czcionka=zmwym.czcionka; //1;
  TextG.obiektt1 = 0;
  TextG.obiektt2 = O2BlockPline;
  TextG.blok=1;  */

 if ((dane_global.odl==0) || (dane_global.odl==2))
 {

  if(!Lin(dane_prof.odl,0,dane_prof.odl,10,64,koloryp.os_pomiaru)) return 0;

  /*tekst 1  ODLEGLOSC*/
  TextG.x=(float)(dane_prof.odl-1.);
  TextG.y=(float)(1.+dt_y);

  odl_i=(int) (dane_prof0->odl + dane_prof0->op3);
  odl_1=(odl_i % 100);
  odl_r=odl_1+((dane_prof0->odl + dane_prof0->op3) - odl_i);

  //gcvt(odl_r,ndig,tekst_p);
  sprintf_prec(tekst_p,odleglosc_prec,odl_r);
  l_kr=add_000(tekst_p,odleglosc_poz);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.typ=n_odleglosc;
 /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

//  if(!Lin(dane_prof.odl,0,dane_prof.odl,10,64,koloryp.os_pomiaru)) return 0;

 } /* typ profilu 0*/

  TextG.typ=n_typ_normal;
  /*odszukanie maksymalnej rzednej*/
  max_r=dane_prof.rki;
  if (dane_prof.ru>max_r) max_r=dane_prof.ru;
  if (dane_prof.si1>max_r) max_r=dane_prof.si1;
  if (si3a>max_r) max_r=si3a;
  if (oi2a>max_r) max_r=oi2a;
  if (sp1a>max_r) max_r=sp1a;
  if (sp2a>max_r) max_r=sp2a;
  if (sp3a>max_r) max_r=sp3a;
  if (op2a>max_r) max_r=op2a;

  if(!Lin(dane_prof.odl,tablica_y[14]/*78*/,dane_prof.odl,tablica_y[14]/*78*/+(max_r-rpp),32,koloryp.os_pomiaru)) return 0;

  TextG.czcionka=zmwym.czcionka; //0;

  /* wstawianie poszczegolnych instalacji*/

  TextG.kat= 0;
  TextG.wysokosc=ht2;
 // TextG.kolor=koloryp.opis_instalacji;

  /*kolektor sanitarny, Co lub Cok*/
  rura_dx=0;
  if (dane_prof.rki>0)
  {
      dane_size0_01=dane_size;
      /*kolektor sanitarny  - dno*/
      if (dane_prof.rkp>0)
      {
          rura_x=dane_prof.odl;
          rura_y=tablica_y[14]/*78*/+(dane_prof.rki-rpp)+(dane_prof.rkp);
          rura_dy=dane_prof.rkp;
          rura_dx=dane_prof.rkp/dane_global.ru;
          //if (FALSE == add_block_in_block ())
          ////if (FALSE == add_block (B_PLINE, 0))  //B_PLINE
          ////{
          ////    return FALSE ;
          ////}
          if (strcmp(Ks_Co_Cok,"Cok")==0)
          {
              if (FALSE == Create_Ellipse (rura_x-rura_dx, rura_y, rura_dx , rura_dy, 0, XOR_PUT, (BLOK **)&dane, NULL, 1, O2BlockPline, LiniaG.warstwa, 64, 0, LiniaG.kolor, 1))
              {
                  usun_obiekt ((void *)dane) ;
                  return 0 ;
              }
              if (FALSE == Create_Ellipse (rura_x+rura_dx, rura_y, rura_dx , rura_dy, 0, XOR_PUT, (BLOK **)&dane, NULL, 1, O2BlockPline, LiniaG.warstwa, 64, 0, LiniaG.kolor, 1))
              {
                  usun_obiekt ((void *)dane) ;
                  return 0 ;
              }
          }
          else
          {
              if (FALSE == Create_Ellipse (rura_x, rura_y, rura_dx , rura_dy, 0, XOR_PUT, (BLOK **)&dane, NULL, 1, O2BlockPline, LiniaG.warstwa, 64, 0, LiniaG.kolor, 1))
              {
                  usun_obiekt ((void *)dane) ;
                  return 0 ;
              }
          }
          u_ell_size = sizeof (NAGLOWEK) + ((BLOK*)dane)->n ;
      }

      if (FALSE == add_block (B_INSTALACJE_K, 0))
      {
          return 0;
      }
      /*dno kanalu*/
      if (strcmp(Ks_Co_Cok,"Cok")==0)
      {
          if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.rki-rpp),dane_prof.odl+10,tablica_y[14]/*78*/+(dane_prof.rki-rpp),32,koloryp.os_pomiaru))
              return 0;
      }
      else
      {
          if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.rki-rpp),dane_prof.odl+10+rura_dx,tablica_y[14]/*78*/+(dane_prof.rki-rpp),32,koloryp.os_pomiaru))
              return 0;
      }
      if (strcmp(Ks_Co_Cok,"Cok")==0)
      {
          TextG.x=(float)(dane_prof.odl-2*rura_dx-0.5);  /*uzaleznic od srednicy*/
      }
      else TextG.x=(float)(dane_prof.odl-rura_dx-0.5);  /*uzaleznic od srednicy*/

      TextG.justowanie=j_do_prawej;
      TextG.y=(float)(dane_prof.rki-rpp+tablica_y[14]/*78*/+1+(dane_prof.rkp*1.5));
      strcpy(&TextG.text[0],Ks_Co_Cok);
      LengthT=(int)strlen(TextG.text);
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      if (strcmp(Ks_Co_Cok,"Ks")==0) TextG.kolor=koloryp.K;
      else TextG.kolor=12;
      TextG.typ=n_typ_normal;
      /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.justowanie=j_do_lewej;

      if (strcmp(Ks_Co_Cok,"Cok")==0)
      {
          TextG.x=(float)(dane_prof.odl+2*rura_dx+0.5);  /*uzaleznic od srednicy*/
      }
      else TextG.x=(float)(dane_prof.odl+rura_dx+0.5);  /*uzaleznic od srednicy*/
      TextG.y=(float)(dane_prof.rki-rpp+tablica_y[14]/*78*/+1.);
      // gcvt(dane_prof0->rki,ndig,tekst_p);
      sprintf_prec(tekst_p,rzedna_k_prec,dane_prof0->rki);
      l_kr=add_000(tekst_p,rzedna_k_poz);
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      //   TextG.kolor=koloryp.K /*rzedne*/;
      TextG.typ=n_rzedna_kanalu_i;

      /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      /*srednica kanalu sanitarnego*/

      if (dane_prof.rkp>0)
      {
          if (strcmp(Ks_Co_Cok,"Cok")==0)
          {
              TextG.x=(float)(dane_prof.odl+2*rura_dx+0.5);  /*uzaleznic od srednicy*/
          }
          else TextG.x=(float)(dane_prof.odl+rura_dx+0.5);   /*uzaleznic od srednicy*/
          TextG.y=(float)(dane_prof.rki-rpp+tablica_y[14]/*78*/-3.);
          strcpy(tekst_p,Fi);
          // gcvt(dane_prof0->rkp,ndig,tekst_p1);
          sprintf(tekst_p1,"%g",dane_prof0->rkp);
          /////    if (dane_prof0->rkp<=10)
          /////     {
          /////      l_kr=add_000(tekst_p1,1);
          /////     }
          decimal(tekst_p1);

          strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
          if (dane_prof0->rkp>10)
          {
              strcpy(tekst_p1," mm");
          }
          else
          {
              strcpy(tekst_p1," m");
          }

          strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

          strcpy(&TextG.text[0],tekst_p);
          LengthT=(int)strlen(TextG.text);
          TextG.dl=LengthT;
          TextG.n=T18+TextG.dl;
          TextG.typ=n_srednica_kanalu_i;
          /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
          if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      }
      dane_size0_00=dane_size-dane_size0_01;
      //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
      strcpy(Ks_Co_Cok1,"*");
      strcat(Ks_Co_Cok1,Ks_Co_Cok);
      group_block_with_type(dane_size0_00, dane_prof.odl, 0, Ks_Co_Cok1, B_INSTALACJE_K);
  }

   /*kolektor ogolnosplawne*/
  rura_dx=0;
  if (dane_prof.ru>0)
  {
    dane_size0_01=dane_size;
    if (dane_prof.rt>0)
    {
     rura_x=dane_prof.odl;
     rura_y=tablica_y[14]/*78*/+(dane_prof.ru-rpp)+dane_prof.rt;
     rura_dy=dane_prof.rt;
     rura_dx=dane_prof.rt/dane_global.ru;

//   if (FALSE == add_block_in_block ())
     ////if (FALSE == add_block (B_PLINE, 0))  //B_PLINE
     ////{
     ////  return FALSE ;
     ////}

     if (FALSE == Create_Ellipse (rura_x, rura_y, rura_dx , rura_dy, 0, XOR_PUT, (BLOK **)&dane, NULL, 1, O2BlockPline, LiniaG.warstwa, 64, 255, LiniaG.kolor, 1))
     {
      usun_obiekt ((void *)dane) ;
      return 0 ;
     }
     u_ell_size = sizeof (NAGLOWEK) + ((BLOK*)dane)->n ;
    }

    if (FALSE == add_block (B_INSTALACJE_K, 0))
     {
      return 0;
     }
   if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.ru-rpp),dane_prof.odl+10,tablica_y[14]/*78*/+(dane_prof.ru-rpp),32,koloryp.os_pomiaru))
     return 0;

   TextG.justowanie=j_do_prawej;
   TextG.x=(float)(dane_prof.odl-rura_dx-0.5);
   TextG.y=(float)(dane_prof.ru-rpp+tablica_y[14]/*78*/+1+(dane_prof.rt*1.5));
   strcpy(&TextG.text[0],"Ko");
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.kolor=koloryp.K;
   TextG.typ=n_typ_normal;
  /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   TextG.justowanie=j_do_lewej;

   TextG.x=(float)(dane_prof.odl+rura_dx+0.5);
   TextG.y=(float)(dane_prof.ru-rpp+tablica_y[14]/*78*/+1.);
  // gcvt(dane_prof0->ru,ndig,tekst_p);
   sprintf_prec(tekst_p,rzedna_k_prec,dane_prof0->ru);
   l_kr=add_000(tekst_p,rzedna_k_poz);
   strcpy(&TextG.text[0],tekst_p);
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.kolor=koloryp.K;
   TextG.typ=n_rzedna_kanalu_i;
  /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

   /*srednica kanalu ogolnosplawnego*/

   if (dane_prof.rt>0)
   {
    TextG.x=(float)(dane_prof.odl+rura_dx+0.5);
    TextG.y=(float)(dane_prof.ru-rpp+tablica_y[14]/*78*/-3.);
    strcpy(tekst_p,Fi);
   // gcvt(dane_prof0->rt,ndig,tekst_p1);
    sprintf(tekst_p1,"%g",dane_prof0->rt);
////    if (dane_prof0->rt<=10)
////     {
////      l_kr=add_000(tekst_p1,1);
////     }
    decimal(tekst_p1);

    strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
    if (dane_prof0->rt>10)
     {
     strcpy(tekst_p1," mm");
     }
    else
     {
      strcpy(tekst_p1," m");
     }
    strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.typ=n_srednica_kanalu_i;
   /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }
   dane_size0_00=dane_size-dane_size0_01;
   //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
   group_block_with_type(dane_size0_00, dane_prof.odl, 0, "*Ko", B_INSTALACJE_K);
  }

    /*kolektor deszczowy*/
  rura_dx=0;
  if (dane_prof.si1>0)
  {
      dane_size0_01=dane_size;
      if (dane_prof.si2>0)
      {
          rura_x=dane_prof.odl;
          rura_y=tablica_y[14]/*78*/+(dane_prof.si1-rpp)+dane_prof.si2;
          rura_dy=dane_prof.si2;
          rura_dx=dane_prof.si2/dane_global.ru;

          //   if (FALSE == add_block_in_block ())
          ////if (FALSE == add_block (B_PLINE, 0))  //B_PLINE
          ////{
          ////    return FALSE ;
          ////}

          if (FALSE == Create_Ellipse (rura_x, rura_y, rura_dx , rura_dy, 0, XOR_PUT, (BLOK **)&dane, NULL, 1, O2BlockPline, LiniaG.warstwa, 64, 0, LiniaG.kolor, 1))
          {
              usun_obiekt ((void *)dane) ;
              return 0 ;
          }
          u_ell_size = sizeof (NAGLOWEK) + ((BLOK*)dane)->n ;
      }

      if (FALSE == add_block (B_INSTALACJE_K, 0))
      {
          return 0;
      }

      if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.si1-rpp),dane_prof.odl+10,tablica_y[14]/*78*/+(dane_prof.si1-rpp),32,koloryp.os_pomiaru)) return 0;

      TextG.justowanie=j_do_prawej;
      TextG.x=(float)(dane_prof.odl-rura_dx-0.5);
      TextG.y=(float)(dane_prof.si1-rpp+tablica_y[14]/*78*/+1+(dane_prof.si2*1.5));
      strcpy(&TextG.text[0],"Kd");
      LengthT=(int)strlen(TextG.text);
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.kolor=koloryp.K;
      TextG.typ=n_typ_normal;
      /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      TextG.justowanie=j_do_lewej;


      TextG.x=(float)(dane_prof.odl+rura_dx+0.5);
      TextG.y=(float)(dane_prof.si1-rpp+tablica_y[14]/*78*/+1.);
      //gcvt(dane_prof0->si1,ndig,tekst_p);
      sprintf_prec(tekst_p,rzedna_k_prec,dane_prof0->si1);
      l_kr=add_000(tekst_p,rzedna_k_poz);
      strcpy(&TextG.text[0],tekst_p);
      LengthT=(int)strlen(TextG.text);
      TextG.dl=LengthT;
      TextG.n=T18+TextG.dl;
      TextG.kolor=koloryp.K;
      TextG.typ=n_rzedna_kanalu_i;
      /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
      if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      /*srednica kanalu deszczowego*/

      if (dane_prof.si2>0)
      {
          TextG.x=(float)(dane_prof.odl+rura_dx+0.5);
          TextG.y=(float)(dane_prof.si1-rpp+tablica_y[14]/*78*/-3.);
          strcpy(tekst_p,Fi);
          //gcvt(dane_prof0->si2,ndig,tekst_p1);
          sprintf(tekst_p1,"%g",dane_prof0->si2);
          ////    if (dane_prof0->si2<=10)
          ////     {
          ////      l_kr=add_000(tekst_p1,1);
          ////     }
          decimal(tekst_p1);

          strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
          if (dane_prof0->si2>10)
          {
              strcpy(tekst_p1," mm");
          }
          else
          {
              strcpy(tekst_p1," m");
          }

          strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

          strcpy(&TextG.text[0],tekst_p);
          LengthT=(int)strlen(TextG.text);
          TextG.dl=LengthT;
          TextG.n=T18+TextG.dl;
          TextG.typ=n_srednica_kanalu_i;
          /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
          if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
      }
      dane_size0_00=dane_size-dane_size0_01;
      //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
      group_block_with_type(dane_size0_00, dane_prof.odl, 0, "*Kd", B_INSTALACJE_K);
  }

   /*wodociag*/
   rura_dx=0;
   if (dane_prof.si3>0)
   {
   dane_size0_01=dane_size;
   if (dane_prof.oi1>0)
    {
     rura_x=dane_prof.odl;
     rura_y=tablica_y[14]/*78*/+(si3a-rpp);
     rura_dy=dane_prof.oi1;
     rura_dx=dane_prof.oi1/dane_global.ru;


//   if (FALSE == add_block_in_block ())
     ////if (FALSE == add_block (B_PLINE, 0))  //B_PLINE
     ////{
     ////  return FALSE ;
     ////}

     if (FALSE == Create_Ellipse (rura_x, rura_y, rura_dx , rura_dy, 0, XOR_PUT, (BLOK **)&dane, NULL, 1, O2BlockPline, LiniaG.warstwa, 64, 0, LiniaG.kolor, 1))
     {
      usun_obiekt ((void *)dane) ;
      return 0 ;
     }
      u_ell_size = sizeof (NAGLOWEK) + ((BLOK*)dane)->n ;
    }

   if (FALSE == add_block (B_INSTALACJE_K, 0))
    {
      return 0;
    }

   if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(si3a-rpp),dane_prof.odl+10,tablica_y[14]/*78*/+(si3a-rpp),32,koloryp.os_pomiaru)) return 0;

   TextG.justowanie=j_do_prawej;
   TextG.x=(float)(dane_prof.odl-rura_dx-0.5);
   TextG.y=(float)(si3a-rpp+tablica_y[14]/*78*/+1+dane_prof.oi1*0.5);
   strcpy(&TextG.text[0],"W");
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.kolor=koloryp.W;
   TextG.typ=n_typ_normal;
  /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   TextG.justowanie=j_do_lewej;


   if (Check_if_Equal(dane_prof0->si3, 1.0)==FALSE)
   {
    TextG.x=(float)(dane_prof.odl+rura_dx+0.5);
    TextG.y=(float)(si3a-rpp+tablica_y[14]/*78*/+1.);
    //gcvt(dane_prof0->si3,ndig,tekst_p);
    sprintf_prec(tekst_p,rzedna_k_prec,dane_prof0->si3);
    l_kr=add_000(tekst_p,rzedna_k_poz);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.kolor=koloryp.W;
    TextG.typ=n_rzedna_kanalu_i;
    /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

   /*srednica wodociagu*/

   if (dane_prof.oi1>0)
   {
    TextG.x=(float)(dane_prof.odl+rura_dx+0.5);
    TextG.y=(float)(si3a-rpp+tablica_y[14]/*78*/-3.);
    strcpy(tekst_p,Fi);
    //gcvt(dane_prof0->oi1,ndig,tekst_p1);
    sprintf(tekst_p1,"%g",dane_prof0->oi1);

    decimal(tekst_p1);

    strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

    strcpy(tekst_p1," mm");
    strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.typ=n_srednica_kanalu_i;
   /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
    }
    dane_size0_00=dane_size-dane_size0_01;
   //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
    group_block_with_type(dane_size0_00, dane_prof.odl, 0, "*W", B_INSTALACJE_K);
   }

   /*gaz*/
   rura_dx=0;
   if (dane_prof.oi2>0)
   {
   dane_size0_01=dane_size;
   if (dane_prof.oi3>0)
    {
     rura_x=dane_prof.odl;
     rura_y=tablica_y[14]/*78*/+(oi2a-rpp);
     rura_dy=dane_prof.oi3;
     rura_dx=dane_prof.oi3/dane_global.ru;
//   if (FALSE == add_block_in_block ())
     ////if (FALSE == add_block (B_PLINE, 0))  //B_PLINE
     ////{
     ////  return FALSE ;
     ////}

     if (FALSE == Create_Ellipse (rura_x, rura_y, rura_dx , rura_dy, 0, XOR_PUT, (BLOK **)&dane, NULL, 1, O2BlockPline, LiniaG.warstwa, 64, 0, LiniaG.kolor, 1))
      {
       usun_obiekt ((void *)dane) ;
       return 0 ;
      }
     u_ell_size = sizeof (NAGLOWEK) + ((BLOK*)dane)->n ;
    }

   if (FALSE == add_block (B_INSTALACJE_K, 0))
    {
      return 0;
    }
   if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(oi2a-rpp),dane_prof.odl+10,tablica_y[14]/*78*/+(oi2a-rpp),32,koloryp.os_pomiaru)) return 0;

   TextG.justowanie=j_do_prawej;
   TextG.x=(float)(dane_prof.odl-rura_dx-0.5);
   TextG.y=(float)(oi2a-rpp+tablica_y[14]/*78*/+1+(dane_prof.oi3*0.5));
   strcpy(&TextG.text[0],"G");
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.kolor=koloryp.G;
   TextG.typ=n_typ_normal;
  /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   TextG.justowanie=j_do_lewej;

   if (Check_if_Equal(dane_prof0->oi2, 1.0)==FALSE)
   {
    TextG.x=(float)(dane_prof.odl+rura_dx+0.5);
    TextG.y=(float)(oi2a-rpp+tablica_y[14]/*78*/+1.);
    //gcvt(dane_prof0->oi2,ndig,tekst_p);
    sprintf_prec(tekst_p,rzedna_k_prec,dane_prof0->oi2);
    l_kr=add_000(tekst_p,rzedna_k_poz);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.kolor=koloryp.G;
    TextG.typ=n_rzedna_kanalu_i;
   /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

   /*srednica gazu*/

   if (dane_prof.oi3>0)
   {
    TextG.x=(float)(dane_prof.odl+rura_dx+0.5);
    TextG.y=(float)(oi2a-rpp+tablica_y[14]/*78*/-3.);
    strcpy(tekst_p,Fi);
    //gcvt(dane_prof0->oi3,ndig,tekst_p1);
    sprintf(tekst_p1,"%g",dane_prof0->oi3);

    decimal(tekst_p1);

    strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

    strcpy(tekst_p1," mm");
    strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.typ=n_srednica_kanalu_i;
   /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
    }

    dane_size0_00=dane_size-dane_size0_01;
   //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
    group_block_with_type(dane_size0_00, dane_prof.odl, 0, "*G", B_INSTALACJE_K);
   }

   /*kabel*/
   if (dane_prof.sp1>0)
   {
    dane_size0_01=dane_size;
    /*kabel elektryczny  - os*/
//   if (FALSE == add_block_in_block ())
    if (FALSE == add_block (B_PLINE, 0))
    {
     return 0;
    }

    rura_x=dane_prof.odl;
    rura_y=tablica_y[14]/*78*/+(sp1a-rpp);
    rura_r=0.7;

   /*dopisannie kola*/

    if(!Kol(rura_x,rura_y,rura_r,koloryp.os_pomiaru)) return 0;

    /*dopisanie grota*/
    if(!Lin(rura_x,rura_y,rura_x+1.6,rura_y+3.6,64,koloryp.os_pomiaru)) return 0;  /*1,2*/
    if(!Lin(rura_x+1.6,rura_y+3.6,rura_x+2,rura_y+1.5,64,koloryp.os_pomiaru)) return 0;  /*2,3*/
    if(!Lin(rura_x+2,rura_y+1.5,rura_x+2.8,rura_y+4.6,64,koloryp.os_pomiaru)) return 0;  /*3,4*/
    if(!Lin(rura_x+2.5,rura_y+4.65,rura_x+3.1,rura_y+4.5,64,koloryp.os_pomiaru)) return 0; /*5,6*/
    if(!Lin(rura_x+2.5,rura_y+4.65,rura_x+3.25,rura_y+6.3,64,koloryp.os_pomiaru)) return 0; /*5,7*/
    if(!Lin(rura_x+3.1,rura_y+4.5,rura_x+3.25,rura_y+6.3,64,koloryp.os_pomiaru)) return 0; /*6,7*/

   if (FALSE == add_block (B_INSTALACJE_K, 0))
    {
      return 0;
    }

   TextG.x=(float)(dane_prof.odl-3.5);
   TextG.y=(float)(sp1a-rpp+tablica_y[14]/*78*/+1.);
   strcpy(&TextG.text[0],"e");
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.kolor=koloryp.e;
   TextG.typ=n_typ_normal;
   /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

   if (Check_if_Equal(dane_prof0->sp1, 1.0)==FALSE)
    {
    if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(sp1a-rpp),dane_prof.odl+10,tablica_y[14]/*78*/+(sp1a-rpp),32,koloryp.os_pomiaru)) return 0;

    TextG.x=(float)(dane_prof.odl+3.);
    TextG.y=(float)(sp1a-rpp+tablica_y[14]/*78*/+1.);
    //gcvt(dane_prof0->sp1,ndig,tekst_p);
    sprintf_prec(tekst_p,rzedna_k_prec,dane_prof0->sp1);
    l_kr=add_000(tekst_p,rzedna_k_poz);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.kolor=koloryp.e;
    TextG.typ=n_rzedna_kanalu_i;
   /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
    }
    dane_size0_00=dane_size-dane_size0_01;
   //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
    group_block_with_type(dane_size0_00, dane_prof.odl, 0, "*E", B_INSTALACJE_K);
   }

   /*kanalizacja telefoniczna*/
   if (dane_prof.sp2>0)
   {
    dane_size0_01=dane_size;

    OkragG.blok=1;
    LiniaG.blok=1;

    rura_x=dane_prof.odl;
    rura_y=tablica_y[14]/*78*/+(sp2a-rpp);
    rura_r=0.6;
   /*dopisannie kola i kwadratu*/

//   if (FALSE == add_block_in_block ())
    if (FALSE == add_block (B_PLINE, 0))
    {
     return 0;
    }

    if(!Kol(rura_x,rura_y,rura_r,koloryp.os_pomiaru)) return 0;  /*okrag*/
    /*kwadrat*/
    if(!Lin(rura_x-1,rura_y+1,rura_x+1,rura_y+1,64,koloryp.os_pomiaru)) return 0;
    if(!Lin(rura_x+1,rura_y+1,rura_x+1,rura_y-1,64,koloryp.os_pomiaru)) return 0;
    if(!Lin(rura_x+1,rura_y-1,rura_x-1,rura_y-1,64,koloryp.os_pomiaru)) return 0;
    if(!Lin(rura_x-1,rura_y-1,rura_x-1,rura_y+1,64,koloryp.os_pomiaru)) return 0;

   if (FALSE == add_block (B_INSTALACJE_K, 0))
    {
      return 0;
    }

   TextG.x=(float)(dane_prof.odl-5.5);
   TextG.y=(float)(sp2a-rpp+tablica_y[14]/*78*/+1.);
   strcpy(&TextG.text[0],"kT");
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.kolor=koloryp.T;
   TextG.typ=n_typ_normal;
  /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

   if (Check_if_Equal(dane_prof0->sp2, 1.0)==FALSE)
    {
    if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(sp2a-rpp),dane_prof.odl+10,tablica_y[14]/*78*/+(sp2a-rpp),32,koloryp.os_pomiaru)) return 0;

    TextG.x=(float)(dane_prof.odl+3.);
    TextG.y=(float)(sp2a-rpp+tablica_y[14]/*78*/+1.);
    //gcvt(dane_prof0->sp2,ndig,tekst_p);
    sprintf_prec(tekst_p,rzedna_k_prec,dane_prof0->sp2);
    l_kr=add_000(tekst_p,rzedna_k_poz);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.kolor=koloryp.T;
    TextG.typ=n_rzedna_kanalu_i;
   /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
    }
    dane_size0_00=dane_size-dane_size0_01;
   //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
    group_block_with_type(dane_size0_00, dane_prof.odl, 0, "*kT", B_INSTALACJE_K);
   }

   /*telefon*/
   if (dane_prof.sp3>0)
   {
    dane_size0_01=dane_size;

    OkragG.blok=1;
    LiniaG.blok=1;

    rura_x=dane_prof.odl;
    rura_y=tablica_y[14]/*78*/+(sp3a-rpp);
    rura_r=0.6;

   /*dopisannie kola*/

//   if (FALSE == add_block_in_block ())
   if (FALSE == add_block (B_PLINE, 0))
    {
     return 0;
    }
   if(!Kol(rura_x,rura_y,rura_r,koloryp.os_pomiaru)) return 0;  /*kolo*/

   if (FALSE == add_block (B_INSTALACJE_K, 0))
    {
      return 0;
    }

   TextG.x=(float)(dane_prof.odl-3.5);
   TextG.y=(float)(sp3a-rpp+tablica_y[14]/*78*/+1.);
   strcpy(&TextG.text[0],"T");
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.kolor=koloryp.T;
   TextG.typ=n_typ_normal;
  /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

   if (Check_if_Equal(dane_prof0->sp3, 1.0)==FALSE)
    {
    if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(sp3a-rpp),dane_prof.odl+10,tablica_y[14]/*78*/+(sp3a-rpp),32,koloryp.os_pomiaru)) return 0;

    TextG.x=(float)(dane_prof.odl+3.);
    TextG.y=(float)(sp3a-rpp+tablica_y[14]/*78*/+1.);
    //gcvt(dane_prof0->sp3,ndig,tekst_p);
    sprintf_prec(tekst_p,rzedna_k_prec,dane_prof0->sp3);
    l_kr=add_000(tekst_p,rzedna_k_poz);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    TextG.kolor=koloryp.T;
    TextG.typ=n_rzedna_kanalu_i;
   /* if (dodaj_obiekt(NULL,&TextG)==NULL) return 0;*/
    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
    }
    dane_size0_00=dane_size-dane_size0_01;
   //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
    group_block_with_type(dane_size0_00, dane_prof.odl, 0, "*T", B_INSTALACJE_K);
   }

  /*opis lancucha na linii  -  DOKONCZYC*/

  TextG.kat= (float)(Pi / 2.);
  TextG.wysokosc=ht1;
  TextG.czcionka=zmwym.czcionka; //0;
  TextG.typ=n_typ_normal;

  /*koniec opisu na linii*/

   LiniaG.blok=1;

  /*koniec generowania obiektow*/

  LiniaG.blok=0;
  OkragG.blok=0;
  strcpy(&TextG.text[0],"");

  dane_size0_00=dane_size-dane_size0_0;
  //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
  group_block_with_type(dane_size0_00, dane_prof.odl, 0, "*I", B_INSTALACJE);

  memmove(&TextG, &textg, sizeof(TEXT));

  return 1;
}


static int Profil5H(void *BufVar)    /*ODWIERT*/
{ const float ht1=2.5f /*25*/;
  const float ht2=2.5f;
  const float ht3=3.5f;
  const double dt_y=0.8;
  MyDane *dane_prof0 = (MyDane*) BufVar;
  ////MyDane dane_prof;
  ////MyDane dane_global;
  double b_sym = 1.5;
  double h_sym = 2.6;
//  static   FILE *comments_tmp;

  int LengthT;
  char tekst_p[60];
  char tekst_p1[60];
  int ndig = 5;
  int i, l_kr;
  unsigned u_ell_size ;

     double rpp;
     double max_r;

     double rura_x, rura_y, rura_dx, rura_dy, rura_r;

     int odl_i;
     double odl_1;
     double odl_r;
     double odl_0;
     int l_h, l_k;
     double del_odl; 
     int ii;
     double punkt1;
     double punktx1, punkty1;
     double warstwa, warstwa0;
     TEXT textg;
//     int dane_oi1;
//     char str1[80];
  /***************************/
  
  memmove(&textg, &TextG, sizeof(TEXT));

  i_tmp = 1;
  
  dane_global.ru=get_skala_profilu_x();
  dane_global.rt=get_poziom_pp();
  dane_global.odl=get_typ_profilu();

  tablica_0_1(dane_global.odl);

  rpp=jednostkiOb(dane_global.rt);
  
  dane_global.rki=get_hektometr_p();
  dane_global.rkp=get_hektometr_k();

  dane_prof.odl=(jednostkiOb(dane_prof0->odl-(dane_global.rki*100))/dane_global.ru); /*odleglosc*/
//  dane_prof.rki=dane_prof0->rki;                /*numer odwiertu*/
  dane_prof.rki=jednostkiOb(dane_prof0->rki);   /*rzedna terenu*/
  dane_prof.rkp=jednostkiOb(dane_prof0->rkp);     /*zwierciadlo ustabilizowane*/
  dane_prof.ru=jednostkiOb(dane_prof0->ru);     /*zwierciadlo napiete*/
  dane_prof.rt=jednostkiOb(dane_prof0->rt);   /*zwierciadlo swobodne*/
  dane_prof.si1=jednostkiOb(dane_prof0->si1);   /*saczenie*/
  dane_prof.si2=jednostkiOb(dane_prof0->si2);   /*warstwa 1*/
  dane_prof.si3=jednostkiOb(dane_prof0->si3);   /*warstwa 2*/
  dane_prof.oi1=jednostkiOb(dane_prof0->oi1);   /*warstwa 3*/
  dane_prof.oi2=jednostkiOb(dane_prof0->oi2);   /*warstwa 4*/
  dane_prof.oi3=jednostkiOb(dane_prof0->oi3);   /*warstwa 5*/
  dane_prof.sp1=jednostkiOb(dane_prof0->sp1);   /*warstwa 6*/
  dane_prof.sp2=jednostkiOb(dane_prof0->sp2);   /*warstwa 7*/
  dane_prof.sp3=jednostkiOb(dane_prof0->sp3);   /*warstwa 8*/
  dane_prof.op1=jednostkiOb(dane_prof0->op1);   /*warstwa 9*/
  dane_prof.op2=jednostkiOb(dane_prof0->op2);   /*warstwa 10*/

  dane_prof.op3=jednostkiOb(dane_prof0->op3)/dane_global.ru;   /*odleglosc od ostatniego hektometru*/
  dane_prof.odl+=dane_prof.op3;
  /*sprawdzanie odleglosci podobne jak dla instalacji*/

  if ((dane_prof.odl>0) && (dane_prof.rki>0))
     dane_global.oi2=0;  /*dynamika bloku*/
      else dane_global.oi2=1;  /*bez dynamiki bloku*/

  l_kr=put_dynamic_block(dane_global.oi2);

  /*******/
  if ((dane_prof0->odl + dane_prof0->op3)<0)
   {
    ErrList(151);
    return 0;
   }
   
   if ((dane_prof0->odl + dane_prof0->op3)>0)
   {
//    if (dane_prof0->odl<=(dane_global.rki*100))
//    {
//     ErrList(152);
//     return 0;
//    }

    dane_global.oi1=((dane_prof0->odl + dane_prof0->op3) / 100);
    if (dane_global.oi1<dane_global.rki)
     {
      ErrList(128);
      return 0;
     }
   }

  /* sprawdzenie rzednej terenu */
  if ((dane_prof.odl>0) && (dane_prof.rki<=0))
   {
    ErrList(153);
    return 0;
   }

  /* sprawdzenie czy wszystkie wartosci sa >0*/
  if ((dane_prof.rkp<0) ||
      (dane_prof.ru<0) ||
      (dane_prof.rt<0) ||
       (dane_prof.si1<0) ||
	(dane_prof.si2<0) ||
	 (dane_prof.si3<0) ||
	  (dane_prof.oi1<0) ||
	   (dane_prof.oi2<0) ||
	    (dane_prof.oi3<0) ||
	     (dane_prof.sp1<0) ||
	      (dane_prof.sp2<0) ||
	       (dane_prof.sp3<0) ||
		(dane_prof.op1<0) ||
		 (dane_prof.op2<0))
    {
    ErrList(154);
    return 0;
    }

  /*sprawdzenie czy rzedne sa narastajace i wyznaczenie najnizszej rzednej */


  if (dane_prof.si2==0)
   {
   ErrList(156);
   return 0;
   }
  max_r=dane_prof.si2;
  if (dane_prof.si3>0)
   {
    if (dane_prof.si2>dane_prof.si3)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.si3;
   }
 if (dane_prof.oi1>0)
   {
    if (dane_prof.si3>dane_prof.oi1)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.oi1;
   }
 if (dane_prof.oi2>0)
   {
    if (dane_prof.oi1>dane_prof.oi2)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.oi2;
   }
 if (dane_prof.oi3>0)
   {
    if (dane_prof.oi2>dane_prof.oi3)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.oi3;
   }
 if (dane_prof.sp1>0)
   {
    if (dane_prof.oi3>dane_prof.sp1)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.sp1;
   }
 if (dane_prof.sp2>0)
   {
    if (dane_prof.sp1>dane_prof.sp2)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.sp2;
   }
 if (dane_prof.sp3>0)
   {
    if (dane_prof.sp2>dane_prof.sp3)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.sp3;
   }
 if (dane_prof.op1>0)
   {
    if (dane_prof.sp3>dane_prof.op1)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.op1;
   }
 if (dane_prof.op2>0)
   {
    if (dane_prof.op1>dane_prof.op2)
    {
    ErrList(155);
    return 0;
    }
    max_r=dane_prof.op2;
  }

   if (dane_prof.rkp>0)
   {
    if (dane_prof.ru<=0)
     {
     ErrList(157);
     return 0;
     }
   }

   if (dane_prof.ru>0)
   {
    if (dane_prof.rkp<=0)
     {
     ErrList(158);
     return 0;/*sprawdzenie czy rzedne sa narastajace i wyznaczenie najnizszej rzednej */
     }
   }

 /* sprawdzenie czy zwierciadla wody nie sa nizej niz dno otworu */
  if (dane_prof.rkp>max_r)
   {
    ErrList(159);
    return 0;
   }

  if (dane_prof.ru>max_r)
   {
    ErrList(160);
    return 0;
   }

  if (dane_prof.rt>max_r)
   {
    ErrList(161);
    return 0;
   }

  if (dane_prof.si1>max_r)
   {
    ErrList(165);
    return 0;
   }

/* generowanie (ewentualne) bloku hektometrow - tylko w przypadku odl>0 */
 if ((dane_global.odl==0) || (dane_global.odl==2))
  {

  if (dane_prof.odl>0)
  {
   /* obliczenie nowej wartosci hektometrow si3 */
   /*zaokragloenie wartosci odleglosci w gore, do pelnego hektometra */

   dane_global.oi1 = ceil((dane_prof0->odl + dane_prof0->op3) / 100);

   dane_global.si1=(jednostkiOb(dane_global.rki)/dane_global.ru);
   dane_global.si2=(jednostkiOb(dane_global.rkp)/dane_global.ru);  /* stare */
   dane_global.si3=(jednostkiOb(dane_global.oi1)/dane_global.ru);  /* nowe */

   if (dane_global.si3>dane_global.si2)
   {
    l_kr=generowanie_bloku_hektometrow(dane_global.rkp, dane_global.ru, dane_global.oi1, 0, 1);
    if (l_kr==0) return 0;
   /*
    if (FALSE == add_block (B_PLINE, 0))
    {
     return 0;
    }

    TextG.kat= 0;
    TextG.wysokosc=ht3;
    TextG.italics=0;
    TextG.width_factor=1;
    TextG.czcionka=zmwym.czcionka; //1;
    TextG.obiektt1 = 0;
    TextG.obiektt2 = O2BlockPline;;
    TextG.blok=1;


    odl_0=0;
    odl_1=jednostkiOb(dane_global.rkp*100)/dane_global.ru;
    del_odl=0;

    for (i=int(dane_global.rkp); i<int(dane_global.oi1); i++)
    {
     odl_0=odl_1;
     odl_1=(jednostkiOb(100)/dane_global.ru)+odl_0;

     if(!Lin(odl_0+1,0,odl_1-1,0,5,7)) return 0;


     if(!Okr(odl_1,0,1,7)) return 0;


     TextG.x=odl_1+del_odl;
     TextG.y=-5.5;

     l_h = (i+1) % 10 ;
     l_k = (i+1) / 10 ;

     if (l_h==0)
      {
       if(!Okr(odl_1,0,0.5,7)) return 0;
      }

     if (l_k>0)
      {
       gcvt(l_k,ndig,tekst_p);
       strncat(tekst_p,"+",1);
      }
      else strcpy(tekst_p,"");

     gcvt(l_h,ndig,tekst_p1);
     strncat(tekst_p,tekst_p1,strlen(tekst_p1));
     strcpy(&TextG.text[0],tekst_p);
     LengthT=strlen(TextG.text);
     TextG.dl=LengthT;
     TextG.n=T18+TextG.dl;

     if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
    }

   strcpy(&TextG.text[0],"");
   dane_global.rkp=dane_global.oi1;
   l_kr=put_hektometr(dane_global.rkp,1);
   */
   }
  }  /*koniec generowania bloku hektometrow*/
  } /* typ profilu 0 */
/* **** */
  /*generowanie bloku dla odwiertu*/
  /*jezeli dynamika bloku=0 to punktem wstawienia bloku jest punkt .odl,0*/
  /* i zwiazany jest on z ukladem lokalnym profilu */
  /*w przeciwnym razie punktem wstawienia bloku jest punkt 0,0 */
  /* i jest on zwiazany z rzedna terenu */


  if (FALSE == add_block_with_type (dane_prof.odl, 0, "*O", 0, B_ODWIERT))
   {
    return 0;
   }

  parametry_ogolne_tekstu(Pi/2, ht1,koloryp.numer_otworu);
/*  TextG.kat= Pi / 2;
  TextG.wysokosc=ht1;
  TextG.italics=0;
  TextG.width_factor=1;
  TextG.czcionka=zmwym.czcionka; //1;
  TextG.obiektt1 = 0;
  TextG.obiektt2 = O2BlockPline;
  TextG.blok=1;  */

  /*generowanie linii i wartosci odleglosci - ZREZYGNOWANO*/
  /*
  if(!Lin(dane_prof.odl,0,dane_prof.odl,10,5,7)) return 0;

  TextG.x=dane_prof.odl-1;
  TextG.y=1+dt_y;

  odl_i=dane_prof0->odl;
  odl_1=(odl_i % 100);
  odl_r=odl_1+(dane_prof0->odl-odl_i);

  gcvt(odl_r,ndig,tekst_p);
  l_kr=add_000(tekst_p,2);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=strlen(TextG.text);
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  */
  /*******/
  /*symbol otworu*/

  if (dane_global.oi2==0) /*dynamika bloku*/
  {
   punkt1=tablica_y[14]/*78*/+(dane_prof.rki-rpp);
                 
   {if(!Lin(dane_prof.odl,    punkt1,     dane_prof.odl-b_sym,punkt1+h_sym,64,koloryp.os_otworu)) return 0;}
   {if(!Lin(dane_prof.odl-b_sym,punkt1+h_sym,dane_prof.odl+b_sym,punkt1+h_sym,64,koloryp.os_otworu)) return 0;}
   {if(!Lin(dane_prof.odl+b_sym,punkt1+h_sym,dane_prof.odl,    punkt1,     64,koloryp.os_otworu)) return 0;}
  }
  else
   {
    {if(!Lin(0,0,-b_sym,h_sym,64,koloryp.os_otworu)) return 0;}
    {if(!Lin(-b_sym,h_sym,b_sym,h_sym,64,koloryp.os_otworu)) return 0;}
    {if(!Lin(b_sym,h_sym,0,0,64,koloryp.os_otworu)) return 0;}
   }


  /* linia otworu, od rzednej terenu do najnizszej warstwy*/
  /* punkt wstawienia zalezy od dynamiki bloku*/
  if (dane_global.oi2==0)  /*dynamika bloku*/
   {
    if(!Lin(dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.rki-rpp),dane_prof.odl,tablica_y[14]/*78*/+(dane_prof.rki-rpp)-max_r,64,koloryp.os_otworu)) return 0;
    if(!Lin(dane_prof.odl-2.5,tablica_y[14]/*78*/+(dane_prof.rki-rpp)-max_r,dane_prof.odl+2.5,tablica_y[14]/*78*/+(dane_prof.rki-rpp)-max_r,64,koloryp.os_otworu)) return 0;
   }
  else
    {
     if(!Lin(0,0,0,-max_r,64,koloryp.os_otworu)) return 0;
     if(!Lin(-2.5,-max_r,2.5,-max_r,64,koloryp.os_otworu)) return 0;
    }

  /* opis otworu*/
  /* numer otworu */
  TextG.kat= 0;
  TextG.wysokosc=ht3;
  TextG.italics=0;
  TextG.width_factor=1;
  TextG.czcionka=zmwym.czcionka; //1;
  TextG.obiektt1 = 0;
  TextG.obiektt2 = O2BlockPline;
  TextG.blok=1;
  TextG.kolor=koloryp.numer_otworu;
  TextG.justowanie=j_srodkowo;
  TextG.typ=n_typ_normal;

  strcpy(tekst_p,comments_tmp[1]);
//  sprintf(tekst_p1,"%.0f",dane_prof.rki);
//  strncat(tekst_p,tekst_p1,strlen(tekst_p1));
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  if (dane_global.oi2==0)  /*dynamika bloku*/
   {
    TextG.x=(float)dane_prof.odl/*-(1.1*LengthT)*/;
    TextG.y=(float)(dane_prof.rki-rpp+tablica_y[18]/*92.5*/ - 2.);
   }
  else
     {
      TextG.x=0.f/*-1.1*LengthT*/;
      TextG.y=12.5f /*14.5*/;
     }
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
  TextG.typ=n_typ_normal;
  /* linia rozdzielajaca */
  if (dane_global.oi2==0)  /*dynamika bloku*/
   {if(!Lin(dane_prof.odl-8,dane_prof.rki-rpp+tablica_y[17]/*91*/-2,dane_prof.odl+8,dane_prof.rki-rpp+tablica_y[17]/*91*/-2,32,koloryp.os_otworu)) return 0;}
  else {if(!Lin(-8,11/*13*/,8,11/*13*/,32,koloryp.os_otworu)) return 0;}

  /*rzedna*/
  strcpy(tekst_p,"");
  sprintf_prec(tekst_p1,rzedna_ti_prec,dane_prof0->rki);
  l_kr=add_000(tekst_p1,rzedna_ti_poz);
  strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
  strcpy(&TextG.text[0],tekst_p);
  LengthT=(int)strlen(TextG.text);
  TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.kolor=koloryp.rzedna_otworu;

  TextG.typ=n_rzedna_terenu;  //7
  TextG.obiektt3=O3SpecialText;

  if (dane_global.oi2==0)  /*dynamika bloku*/
   {
    TextG.x=(float)dane_prof.odl/*-(1.1*LengthT)*/;
    TextG.y=(float)(dane_prof.rki-rpp+tablica_y[16]/*86*/-2.);
   }
  else
    {
     TextG.x=0.f/*-1.1*LengthT*/;
     TextG.y=6.f /*8*/;
    }
  if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

  TextG.typ=n_typ_normal;  //0
  TextG.obiektt3=O3NormalText;
  TextG.justowanie=j_do_lewej;

  /* generowanie opisu zwierciadla ustabilizowanego rkp i napietego ru */
  if (dane_prof.rkp>0)
   {
   /* linia */
    if (dane_global.oi2==0)  /*dynamika bloku*/
     {if(!Lin(dane_prof.odl-14,dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rkp,dane_prof.odl-7,dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rkp,32,koloryp.symbol_wody)) return 5;}
    else {if(!Lin(-14,-dane_prof.rkp,-7,-dane_prof.rkp,32,koloryp.symbol_wody)) return 0;}
   /* solid */
     if (dane_global.oi2==0)  /*dynamika bloku*/
     {
      punktx1=dane_prof.odl-10.5;
      punkty1=dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rkp;
     }
     else
       {
	punktx1=-10.5;
	punkty1=-dane_prof.rkp;
       }
    {if(!Solid3(punktx1,punkty1,punktx1-1.5,punkty1+2.6,punktx1+1.5,punkty1+2.6,koloryp.symbol_wody)) return 5;}
   /* rzedna */

   TextG.kat= 0;
   TextG.wysokosc=ht2;
   TextG.italics=0;
   TextG.width_factor=1;
   TextG.czcionka=zmwym.czcionka; //1;
   TextG.obiektt1 = 0;
   TextG.obiektt2 = O2BlockPline;
   TextG.blok=1;
   TextG.kolor=koloryp.rzedne;

   TextG.typ=n_typ_atrybut;  //8

   strcpy(tekst_p,"");
   sprintf(tekst_p1,"%.1f",dane_prof0->rkp);
   l_kr=add_000(tekst_p1,1);
   strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
   strcpy(&TextG.text[0],tekst_p);
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   if (dane_global.oi2==0)  /*dynamika bloku*/
    {
     TextG.x=(float)(dane_prof.odl-10.-(1.1*LengthT));
     TextG.y=(float)(dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rkp-4.);
    }
    else
     {
      TextG.x=(float)(-10-(1.1*LengthT));
      TextG.y=(float)(-dane_prof.rkp-4.);
     }
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   /* zwierciadlo napiete  */

    if (dane_global.oi2==0)  /*dynamika bloku*/
     {
     if(!Lin(dane_prof.odl-7,dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rkp,dane_prof.odl-7,dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.ru,32,koloryp.symbol_wody)) return 5;
     if(!Lin(dane_prof.odl-7,dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.ru,dane_prof.odl,dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.ru,32,koloryp.symbol_wody)) return 5;
     }
      else
       {
	if(!Lin(-7,-dane_prof.rkp,-7,-dane_prof.ru,32,koloryp.symbol_wody)) return 0;
	if(!Lin(-7,-dane_prof.ru,0,-dane_prof.ru,32,koloryp.symbol_wody)) return 0;
       }
   /* trojkat */
     if (dane_global.oi2==0)  /*dynamika bloku*/
     {
      punktx1=dane_prof.odl-3.5;
      punkty1=dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.ru;
     }
     else
       {
	    punktx1=-3.5;
	    punkty1=-dane_prof.ru;
       }

    if(!Lin(punktx1,punkty1,punktx1-1.5,punkty1+2.6,64,koloryp.symbol_wody)) return 0;
    if(!Lin(punktx1-1.5,punkty1+2.6,punktx1+1.5,punkty1+2.6,64,koloryp.symbol_wody)) return 0;
    if(!Lin(punktx1+1.5,punkty1+2.6,punktx1,punkty1,64,koloryp.symbol_wody)) return 0;

   /* rzedna */
   strcpy(tekst_p,"");
   sprintf(tekst_p1,"%.1f",dane_prof0->ru);
   l_kr=add_000(tekst_p1,1);
   strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
   strcpy(&TextG.text[0],tekst_p);
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   TextG.typ=n_typ_schemat;  //9

   if (dane_global.oi2==0)  /*dynamika bloku*/
    {
     TextG.x=(float)(dane_prof.odl-3.-(1.1*LengthT));
     TextG.y=(float)(dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.ru-4.);
    }
   else
     {
      TextG.x=(float)(-3-(1.1*LengthT));
      TextG.y=(float)(-dane_prof.ru-4.);
     }
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }

  /* generowanie opisu zwierciadla swobodnego rt */

  if (dane_prof.rt>0)
   {
   /* linia */
    if (dane_global.oi2==0)  /*dynamika bloku*/
     {if(!Lin(dane_prof.odl-14,dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rt,dane_prof.odl,dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rt,32,koloryp.symbol_wody)) return 5;}
    else {if(!Lin(-14,-dane_prof.rt,0,-dane_prof.rt,32,koloryp.symbol_wody)) return 0;}
   /* solid */
     if (dane_global.oi2==0)  /*dynamika bloku*/
     {
      punktx1=dane_prof.odl-10.5;
      punkty1=dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rt;
     }
     else
       {
	punktx1=-10.5;
	punkty1=-dane_prof.rt;
       }
    {if(!Solid3(punktx1,punkty1,punktx1-1.5,punkty1+2.6,punktx1+1.5,punkty1+2.6,koloryp.symbol_wody)) return 5;}
   /* trojkat */
     if (dane_global.oi2==0)  /*dynamika bloku*/
     {
      punktx1=dane_prof.odl-3.5;
      punkty1=dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rt;
     }
     else
       {
	punktx1=-3.5;
	punkty1=-dane_prof.rt;
       }

    if(!Lin(punktx1,    punkty1,    punktx1-1.5,punkty1+2.6,64,koloryp.symbol_wody)) return 0;
    if(!Lin(punktx1-1.5,punkty1+2.6,punktx1+1.5,punkty1+2.6,64,koloryp.symbol_wody)) return 0;
    if(!Lin(punktx1+1.5,punkty1+2.6,punktx1,    punkty1,    64,koloryp.symbol_wody)) return 0;

   /* rzedna */

   TextG.kat= 0;
   TextG.wysokosc=ht2;
   TextG.italics=0;
   TextG.width_factor=1;
   TextG.czcionka=zmwym.czcionka; //1;
   TextG.obiektt1 = 0;
   TextG.obiektt2 = O2BlockPline;
   TextG.blok=1;
   TextG.typ=n_typ_plik;  //10

   strcpy(tekst_p,"");
   sprintf(tekst_p1,"%.1f",dane_prof0->rt);
   l_kr=add_000(tekst_p1,1);
   strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
   strcpy(&TextG.text[0],tekst_p);
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   if (dane_global.oi2==0)  /*dynamika bloku*/
    {
     TextG.x=(float)(dane_prof.odl-5.5-(1.1*LengthT));
     TextG.y=(float)(dane_prof.rki-rpp+tablica_y[14]/*78*/-dane_prof.rt-4.);
    }
   else
     {
      TextG.x=(float)(-5.5-(1.1*LengthT));
      TextG.y=(float)(-dane_prof.rt-4.);
     }
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   }  /* koniec zwierciadla swobodnego */

  /* generowanie opisu saczenia si1 */

  if (dane_prof.si1>0)
   {
   //wezyk
   if (dane_global.oi2==0)  //dynamika bloku
     {if(!Wezyk(dane_prof.odl,dane_prof.rki-rpp+tablica_y[14]-dane_prof.si1,koloryp.symbol_wody)) return 5;}
   else {if(!Wezyk(0,-dane_prof.si1,koloryp.symbol_wody)) return 0;}

   // linia
//    if (dane_global.oi2==0)  //dynamika bloku
//     {if(!Lin(dane_prof.odl-14,dane_prof.rki-rpp+tablica_y[14]-dane_prof.si1,dane_prof.odl,dane_prof.rki-rpp+tablica_y[14]-dane_prof.si1,32,koloryp.symbol_wody)) return 5;}
//      else {if(!Lin(-14,-dane_prof.si1,0,-dane_prof.si1,32,koloryp.symbol_wody)) return 0;}
/*
   // solid
     if (dane_global.oi2==0)  //dynamika bloku
     {
      punktx1=dane_prof.odl-10.5;
      punkty1=dane_prof.rki-rpp+tablica_y[14]-dane_prof.si1;
     }
      else
       {
	     punktx1=-10.5;
	     punkty1=-dane_prof.si1;
       }
    {if(!Solid3(punktx1,punkty1,punktx1-1.5,punkty1+2.6,punktx1+1.5,punkty1+2.6,koloryp.symbol_wody)) return 5;}
   // trojkat
     if (dane_global.oi2==0)  //dynamika bloku
     {
      punktx1=dane_prof.odl-3.5;
      punkty1=dane_prof.rki-rpp+tablica_y[14]-dane_prof.si1;
     }
      else
       {
	     punktx1=-3.5;
	     punkty1=-dane_prof.si1;
       }

    if(!Lin(punktx1,    punkty1,    punktx1-1.5,punkty1+2.6,64,koloryp.symbol_wody)) return 0;
    if(!Lin(punktx1-1.5,punkty1+2.6,punktx1+1.5,punkty1+2.6,64,koloryp.symbol_wody)) return 0;
    if(!Lin(punktx1+1.5,punkty1+2.6,punktx1,    punkty1,    64,koloryp.symbol_wody)) return 0;
*/

   /* rzedna */

   TextG.kat= 0;
   TextG.wysokosc=ht2;
   TextG.italics=0;
   TextG.width_factor=1;
   TextG.czcionka=zmwym.czcionka; //1;
   TextG.obiektt1 = 0;
   TextG.obiektt2 = O2BlockPline;
   TextG.blok=1;
   TextG.justowanie=j_do_prawej;
   TextG.typ=n_typ_siec;  //11

   strcpy(tekst_p,"");
   sprintf(tekst_p1,"%.1f",dane_prof0->si1);
   l_kr=add_000(tekst_p1,1);
   strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
   strcpy(&TextG.text[0],tekst_p);
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   if (dane_global.oi2==0)  /*dynamika bloku*/
    {
     TextG.x=(float)(dane_prof.odl-8.);
     TextG.y=(float)(dane_prof.rki-rpp+tablica_y[14]-dane_prof.si1-1.25);
    }
   else
     {
      TextG.x=-8.f;
      TextG.y=(float)(-dane_prof.si1-1.25);
     }
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;
   TextG.justowanie=j_do_lewej;
   }  /* koniec zwierciadla swobodnego */

  /*******/
  /* generowanie opisow warstw */
  /* znany jest poziom minimalny max_r */
  TextG.kat= 0;
  TextG.wysokosc=ht2;
  TextG.italics=0;
  TextG.width_factor=1;
  TextG.czcionka=zmwym.czcionka; //1;
  TextG.obiektt1 = 0;
  TextG.obiektt2 = O2BlockPline;
  TextG.blok=1;
  TextG.kolor=koloryp.opis_warstw;
  TextG.typ=n_typ_komentarz;  //12
  
  for (i=1; i<=10; i++)
  {
  switch (i)
   {
   case 1: warstwa=dane_prof.si2;warstwa0=0;break;
   case 2: warstwa=dane_prof.si3;warstwa0=dane_prof.si2;break;
   case 3: warstwa=dane_prof.oi1;warstwa0=dane_prof.si3;break;
   case 4: warstwa=dane_prof.oi2;warstwa0=dane_prof.oi1;break;
   case 5: warstwa=dane_prof.oi3;warstwa0=dane_prof.oi2;break;
   case 6: warstwa=dane_prof.sp1;warstwa0=dane_prof.oi3;break;
   case 7: warstwa=dane_prof.sp2;warstwa0=dane_prof.sp1;break;
   case 8: warstwa=dane_prof.sp3;warstwa0=dane_prof.sp2;break;
   case 9: warstwa=dane_prof.op1;warstwa0=dane_prof.sp3;break;
   case 10: warstwa=dane_prof.op2;warstwa0=dane_prof.op1;break;
   default : warstwa=0;warstwa0=0;break;
   }
   if (warstwa>0)
    {
    /* linia */
    if (dane_global.oi2==0)  /*dynamika bloku*/
     {if(!Lin(dane_prof.odl,dane_prof.rki-rpp+tablica_y[14]/*78*/-warstwa,dane_prof.odl+3,dane_prof.rki-rpp+tablica_y[14]/*78*/-warstwa,32,koloryp.os_otworu)) return 5;}
    else {if(!Lin(0,-warstwa,3,-warstwa,32,koloryp.os_otworu)) return 0;}

//   strcpy(tekst_p,"w. ");
//   itoa(i,tekst_p1,10);
//   strncat(tekst_p,tekst_p1,strlen(tekst_p1));
//   strcpy(&TextG.text[0],tekst_p);
   i_tmp += 1;
   strcpy(&TextG.text[0],comments_tmp[i_tmp]);
   LengthT=(int)strlen(TextG.text);
   TextG.dl=LengthT;
   TextG.n=T18+TextG.dl;
   if (dane_global.oi2==0)  /*dynamika bloku*/
    {
     TextG.x=(float)(dane_prof.odl+2.);
     TextG.y=(float)(dane_prof.rki-rpp+tablica_y[14]/*78*/-warstwa+((warstwa-warstwa0)/2.)-1.5);
    }
    else
     {
      TextG.x=2.f;
      TextG.y=(float)(-warstwa+((warstwa-warstwa0)/2.)-1.5);
     }
   if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

    /*************************/
    }
  }
//  strcpy(&TextG.text[0],"");

  memmove(&TextG, &textg, sizeof(TEXT));

  return 1;
}

/*-------------------------------------------------*/
/*                                                 */
/*-------------------------------------------------*/

static int (*FUN[])(void *)={ Profil1HD, Profil2H,   Profil3H,  Profil5H,  Profil4H,   ProfilXH, Profil_New };

static BOOL  zakres_odleglosci(int opcja, double *p_odl, double *k_odl)
{ char sk[80];
  int retval_no = 1 ;
  double buf_ret [1] ;
  int l_kr;
  double xi, xir, xj, xjr, yi, yj;
  double local_x;
  double skala_profilu;
  double first_hektometr;

  local_x=get_localx();
  first_hektometr=get_hektometr_p();
  skala_profilu=get_skala_profilu_x();

/*
  strcpy(sk,"");
  if (!read_esc(sk,60,kom1))  return FALSE;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
   {
    return FALSE;
   }
  *p_odl = buf_ret [0] ;
  if (p_odl < 0)
   {
    ErrList (30) ;
    return FALSE;
   }

  strcpy(sk,"");
  if (!read_esc(sk,60,kom2))  return FALSE;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
   {
     return FALSE;
   }
  *d_odl = buf_ret [0] ;

  return TRUE;
*/

  l_kr=Get_First_Second_Pomiar_Param(0, 2/*kanal*/, &xi, &yi);
  if (l_kr>0)
   {
    xir=milimetryob(xi - local_x) * skala_profilu + (first_hektometr * 100);
  
    sprintf(sk,"Odległość początkowa: %12.3f m",xir);
    InfoListStr0(sk);
    *p_odl=xir;
    if (opcja==0)
     {
      strcpy(sk,"");
      if (!read_esc(sk,60,122))
       {
        return FALSE;
       }
      if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
       {
         return FALSE;
       }
       *k_odl = buf_ret [0] ;
       return TRUE;
     }

     l_kr=Get_First_Second_Pomiar_Param(1, 2, &xj, &yj);
      if (l_kr>0)
       {
        xjr =  milimetryob(xj - local_x) * skala_profilu + (first_hektometr * 100);
        sprintf(sk,"Odległość początkowa: %12.3f m, końcowa: %g m",xir, xjr);
          InfoListStr0(sk);
          *k_odl=xjr;
          return TRUE;
       }
   }
 return FALSE;
}

BOOL alone_kpomiar(int kanal, double x)
{
    char *adp_, *adp1_, *adp2_, *adp3_, *adp4_, *adk_;
    BLOK *b;
    T_Point *P, *P1;
    LINIA *L;
    int b_warstwa;
    //poszukiwanie pierwszego pomiaru w odleglosci x
    //jezeli istnieje pomiar, return FALSE
    //jezeli nie return TRUE
    adp_ = dane ;
    adk_ = dane + dane_size ;

    L = (LINIA*)adp_;
    while (L->obiekt != Okoniec && adp_ < adk_)
    {
        if (L->obiekt == OdBLOK)
        {
            b=(BLOK *)adp_;
            b_warstwa=get_block_layer(b);
            if (b_warstwa==Current_Layer)
            {
                if ((Layers[b_warstwa].edit==1) &&
                    (Layers[b_warstwa].on==1))
                {
                    if ((b->kod_obiektu==B_PPOMIAR) && (b->atrybut != Abad) && (b->atrybut != Ausuniety))
                    {
                        //odszukanie punktu kanalu
                        adp1_=adp_+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                        adp2_=adp_+sizeof(NAGLOWEK)+b->n-1;
                        if (kanal==0) //projektowany
                        {
                            adp3_=find_obj(adp1_, adp2_, Opoint, 3, 0);
                        }
                        else
                        {
                            adp3_=find_obj(adp1_, adp2_, Opoint, 1, 0);
                        }
                        if (adp3_ != NULL)  //znaleziono punkt podstawowy (rzedna dna)
                        {
                            P=(T_Point *)adp3_;
                            if (Check_if_Equal(P->x, x)==TRUE) return FALSE;  //znaleziono punkt
                        }
                    }
                    adp_+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                }
                else adp_ +=sizeof(NAGLOWEK) + b->n;
            }
            else adp_ +=sizeof(NAGLOWEK) + b->n;
        }
        else adp_+=sizeof(NAGLOWEK) + L->n;
        L=(LINIA *) adp_;
    }
    return TRUE;
}

int licowanie_kanalu(int kanal, int wyrownywanie)
{
    int l_kr;
    double xi, xir, xj, xjr, yi, yj, xk, yk;
    char *adp, *adp1, *adp2, *adp3, *adp4, *adk;
    BLOK *b;
    T_Point *P, *P1;
    LINIA *L;
    double fi_kanal, del_x, del_x1, del_y, del_y1;
    double local_x;
    double skala_profilu;
    double first_hektometr;
    char sk[80];
    char O2Block;
    int b_warstwa;

    local_x=get_localx();
    first_hektometr=get_hektometr_p();
    skala_profilu=get_skala_profilu_x();

    if (wyrownywanie==0) O2Block=O2BlockPline;
    else if (wyrownywanie==1) O2Block=O2BlockHatch25;
    else if (wyrownywanie==2) O2Block=O2BlockHatch50;

    l_kr=Get_First_Second_Pomiar_Param(0, kanal, &xi, &yi);
    if (l_kr>0)
    {
        xir=milimetryob(xi - local_x) * skala_profilu + (first_hektometr * 100);

        sprintf(sk,"Odleglość początkowa: %12.3f m",xir);
        InfoListStr0(sk);
        l_kr=Get_First_Second_Pomiar_Param(1, kanal, &xj, &yj);
        if (l_kr>0)
        {
            xjr =  milimetryob(xj - local_x) * skala_profilu + (first_hektometr * 100);
            sprintf(sk,"Odleglość początkowa: %12.3f m, końcowa: %12.3f m",xir, xjr);
            InfoListStr0(sk);
            //korekta punktow
            if (Check_if_Equal(xi, xj)==TRUE)
            {
                ClearInfo0();
                return 0;
            }
            if (xj<xi)
            {
                xk=xj;  yk=yj; xj=xi; yj=yi; xi=xk; yi=yk;
            }
            //+++++++++++++++
            adp = dane ;
            adk = dane + dane_size ;

            del_y=yj-yi;
            del_x=xj-xi;

            L = (LINIA*)adp;
            while (L->obiekt != Okoniec && adp < adk)
            {
                if (L->obiekt == OdBLOK)
                {
                    b=(BLOK *)adp;
                    b_warstwa=get_block_layer(b);
                    if (b_warstwa==Current_Layer)
                    {
                        if ((Layers[b_warstwa].edit==1) &&
                            (Layers[b_warstwa].on==1))
                        {
                            if (((b->kod_obiektu==B_PPOMIAR) ||
                                 (b->kod_obiektu==B_KPOMIAR))
                              && ((b->atrybut != Abad) &&
                                 (b->atrybut != Ausuniety)))
                            {
                                adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                                adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                                if (kanal==0) //projektowany
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 3, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 4, 0);
                                }
                                else
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 1, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 2, 0);
                                }

                                if (adp3 != NULL)  //znaleziono punkt podstawowy (rzedna dna)
                                {
                                    P=(T_Point *)adp3;
                                    if (adp4 != NULL)  //istniej rowniez punkt gorny (rzedna stropu)
                                    {
                                        P1=(T_Point *)adp4;
                                    }
                                    //interpolowanie polozenia y na podstawie polozenia x
                                    if (((b->kod_obiektu==B_KPOMIAR) &&
                                        (((Check_if_LE(P->x, xi)==FALSE) ||
                                           ((Check_if_GE(P->x, xi)==TRUE) && (alone_kpomiar(kanal,xi)==TRUE)))
                                           && (Check_if_LE(P->x, xj)==TRUE)))  ||
                                        ((b->kod_obiektu==B_PPOMIAR) &&
                                        ((Check_if_GE(P->x, xi)==TRUE) && (Check_if_GE(P->x, xj)==FALSE))))
                                    {
                                        P->obiektt2=O2Block;
                                        if (adp4 != NULL)
                                        {
                                            P1->obiektt2=O2Block;
                                        }
                                    }
                                }
                            }
                            adp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                        }
                        else adp+=sizeof(NAGLOWEK) + b->n;
                    }
                    else adp+=sizeof(NAGLOWEK) + b->n;
                }
                else adp+=sizeof(NAGLOWEK) + L->n;
                L=(LINIA *) adp;
            }
            //+++++++++++++++
        }
        else
        {
            ClearInfo0();
            return 0;
        }
        ClearInfo0();
        return 1;
    }
    ClearInfo0();
    return 0;
}

int staly_spadek(int kanal)
{
    int l_kr;
    double xi, xir, xj, xjr, yi, yj, xk, yk;
    char *adp, *adp1, *adp2, *adp3, *adp4, *adk;
    BLOK *b;
    T_Point *P, *P1;
    LINIA *L;
    double fi_kanal, del_x, del_x1, del_y, del_y1;
    double local_x;
    double skala_profilu;
    double first_hektometr;
    char sk[80];
    char O2Block;
    int wyrownywanie;
    int b_warstwa;

    local_x=get_localx();
    first_hektometr=get_hektometr_p();
    skala_profilu=get_skala_profilu_x();

    //  if (wyrownywanie==0) O2Block=O2BlockSpecial;
    //   else if (wyrownywanie==1) O2Block=O2BlockHatch25;
    //    else if (wyrownywanie==2) O2Block=O2BlockHatch50;


    l_kr=Get_First_Second_Pomiar_Param(0, kanal, &xi, &yi);
    if (l_kr>0)
    {
        xir=milimetryob(xi - local_x) * skala_profilu + (first_hektometr * 100);

        sprintf(sk,"Odległość początkowa: %12.3f m",xir);
        InfoListStr0(sk);
        l_kr=Get_First_Second_Pomiar_Param(1, kanal, &xj, &yj);
        if (l_kr>0)
        {
            xjr =  milimetryob(xj - local_x) * skala_profilu + (first_hektometr * 100);
            sprintf(sk,"Odleglość początkowa: %12.3f m, końcowa: %12.3f m",xir, xjr);
            InfoListStr0(sk);
            //korekta punktow
            if (Check_if_Equal(xi, xj)==TRUE)
            {
                ClearInfo0();
                return 0;
            }
            if (xj<xi)
            {
                xk=xj;  yk=yj; xj=xi; yj=yi; xi=xk; yi=yk;
            }
            //+++++++++++++++
            adp = dane ;
            adk = dane + dane_size ;

            del_y=yj-yi;
            del_x=xj-xi;

            L = (LINIA*)adp;
            while (L->obiekt != Okoniec && adp < adk)
            {
                if (L->obiekt == OdBLOK)
                {
                    b=(BLOK *)adp;
                    b_warstwa=get_block_layer(b);
                    if (b_warstwa==Current_Layer)
                    {
                        if ((Layers[b_warstwa].edit==1) &&
                            (Layers[b_warstwa].on==1))
                        {
                            if (((b->kod_obiektu==B_PPOMIAR) ||
                                 (b->kod_obiektu==B_KPOMIAR))
                              && ((b->atrybut != Abad) &&
                                 (b->atrybut != Ausuniety)))
                            {
                                adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                                adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                                if (kanal==0) //projektowany
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 3, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 4, 0);
                                }
                                else if (kanal==1) //istniejacy
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 1, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 2, 0);
                                }
                                else if (kanal==5) //teren projektowany
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 5, 0);
                                    adp4=NULL;
                                }
                                else if (kanal==6) //teren istniejacy
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 6, 0);
                                    adp4=NULL;
                                }

                                if (adp3 != NULL)  //znaleziono punkt podstawowy (rzedna dna)
                                {
                                    P=(T_Point *)adp3;
                                    fi_kanal=0;
                                    if (adp4 != NULL)  //istniej rowniez punkt gorny (rzedna stropu)
                                    {
                                        P1=(T_Point *)adp4;
                                        fi_kanal=(P1->y-P->y);
                                    }
                                    //interpolowanie polozenia y na podstawie polozenia x
                                    if (((b->kod_obiektu==B_KPOMIAR) &&
                                        ((Check_if_LE(P->x, xi)==FALSE) && (Check_if_LE(P->x, xj)==TRUE)))  ||
                                        ((b->kod_obiektu==B_PPOMIAR) &&
                                        ((Check_if_GE(P->x, xi)==TRUE) && (Check_if_GE(P->x, xj)==FALSE))))
                                    {
                                        //zmiana polozenia punktu (y)


                                        //  P->obiektt2=O2Block;
                                        //  if (adp4 != NULL) P1->obiektt2=O2Block;
                                        if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                        else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                        else wyrownywanie=0;

                                        del_x1=P->x-xi;
                                        del_y1=(del_x1/del_x) * del_y;
                                        if ((kanal==0) || (kanal==1))
                                        {
                                            if (wyrownywanie==wyrownywanie_dolem)
                                            {
                                                P->y=yi+del_y1;
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y=P->y+fi_kanal;
                                                }
                                            }
                                            else
                                                if (wyrownywanie==wyrownywanie_gora)
                                                {
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=yi+del_y1;
                                                        P->y=P1->y-fi_kanal;
                                                    }
                                                }
                                                else
                                                    if (wyrownywanie==wyrownywanie_osiowe)
                                                    {
                                                        //przesuniecie punktu gornego
                                                        if (adp4 != NULL)
                                                        {
                                                            P->y=yi+del_y1-(fi_kanal/2);
                                                            P1->y=P->y+fi_kanal;
                                                        }
                                                        // else P->y=yi+del_y1;
                                                    }
                                        }
                                        else //teren projektowany
                                        {
                                            P->y=yi+del_y1;
                                        }
                                    }
                                }
                            }
                            adp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                        }
                        else adp+=sizeof(NAGLOWEK) + b->n;
                    }
                    else adp+=sizeof(NAGLOWEK) + b->n;
                }
                else adp+=sizeof(NAGLOWEK) + L->n;
                L=(LINIA *) adp;
            }
            //+++++++++++++++
        }
        else
        {
            ClearInfo0();
            return 0;
        }
        ClearInfo0();
        return 1;
    }
    ClearInfo0();
    return 0;
}

int stale_zaglebienie (int kanal)
{
    int l_kr;
    double xi, yi, xj, yj, xk, yk, xir, xjr;
    char *adp, *adp1, *adp2, *adp3, *adp4, *adp5, *adp6, *adk;
    BLOK *b;
    T_Point *P, *P1, *P2, *P3;
    LINIA *L;
    double fi_kanal;
    char sk[80];
    int retval_no = 1 ;
    double buf_ret [1] ;
    double zaglebienie;
    double skala_profilu;
    double local_x;
    double first_hektometr;
    char O2Block;
    int wyrownywanie;
    BOOL znaleziono_teren;
    int b_warstwa;

    local_x=get_localx();
    first_hektometr=get_hektometr_p();
    skala_profilu=get_skala_profilu_x();

    l_kr=sortowanie_odl();  //dla potrzeb interpolacji zaglebienia rury
    //w przekrojach "tylko rura"

    //  if (wyrownywanie==0) O2Block=O2BlockSpecial;
    //   else if (wyrownywanie==1) O2Block=O2BlockHatch25;
    //    else if (wyrownywanie==2) O2Block=O2BlockHatch50;

    l_kr=Get_First_Second_Pomiar_Param(0, kanal, &xi, &yi);
    if (l_kr>0)
    {
        xir=milimetryob(xi - local_x) * skala_profilu + (first_hektometr * 100);

        sprintf(sk,"Odległość początkowa: %12.3f m",xir);
        InfoListStr0(sk);
        l_kr=Get_First_Second_Pomiar_Param(1, kanal, &xj, &yj);
        if (l_kr>0)
        {
            xjr =  milimetryob(xj - local_x) * skala_profilu + (first_hektometr * 100);
            sprintf(sk,"Odleglość początkowa: %12.3f m, końcowa: %12.3f m",xir, xjr);
            InfoListStr0(sk);
            //odczyt wartosci zaglebienia
            strcpy(sk,"");
            if (!read_esc(sk,60,135))
            {
                ClearInfo0();
                return 0;
            }
            if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
            {
                ClearInfo0();
                return 0;
            }

            zaglebienie = jednostkiOb(buf_ret [0]) ;


            //korekta punktow
            if (Check_if_Equal(xi, xj)==TRUE) return 0;
            if (xj<xi)
            {
                xk=xj;  yk=yj; xj=xi; yj=yi; xi=xk; yi=yk;
            }
            //+++++++++++++++
            adp = dane ;
            adk = dane + dane_size ;

            L = (LINIA*)adp;
            while (L->obiekt != Okoniec && adp < adk)
            {
                if (L->obiekt == OdBLOK)
                {
                    b=(BLOK *)adp;
                    b_warstwa=get_block_layer(b);
                    if (b_warstwa==Current_Layer)
                    {
                        if ((Layers[b_warstwa].edit==1) &&
                            (Layers[b_warstwa].on==1))
                        {
                            if (((b->kod_obiektu==B_PPOMIAR) ||
                                 (b->kod_obiektu==B_KPOMIAR))
                              && ((b->atrybut != Abad) &&
                                 (b->atrybut != Ausuniety)))
                            {
                                adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                                adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                                if (kanal==0) //projektowany
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 3, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 4, 0);
                                    adp5=find_obj(adp1, adp2, Opoint, 5, 0);     //teren projektowany
                                    adp6=find_obj(adp1, adp2, Opoint, 6, 0);     //teren istniejacy
                                }
                                else if (kanal==1) //istniejacy
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 1, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 2, 0);
                                    adp5=find_obj(adp1, adp2, Opoint, 5, 0);  //teren projektowany
                                    adp6=find_obj(adp1, adp2, Opoint, 6, 0);  //teren istniejacy
                                }

                                if (adp3 != NULL)
                                {
                                    znaleziono_teren=FALSE;
                                    if ((adp5 != NULL) || (adp6 != NULL))
                                    {
                                        if (adp5 != NULL)
                                        {P3=(T_Point *)adp5;
                                            yi=P3->y;
                                        }
                                        else
                                        {P2=(T_Point *)adp6;
                                            yi=P2->y;
                                        }
                                        znaleziono_teren=TRUE;
                                    }
                                    if (znaleziono_teren==FALSE)
                                    {
                                        P=(T_Point *)adp3;
                                        //szukanie wartosci interpolowanej terenu projektowanego
                                        l_kr=find_interpolated(P->x, 0, &yi);   //teren projektowany
                                        if (l_kr==0) l_kr=find_interpolated(P->x, 1, &yi); //teren istniejacy
                                        if (l_kr==1) znaleziono_teren=TRUE;
                                    }
                                }

                                if   ((adp3 != NULL)    //znaleziono punkt podstawowy (rzedna dna lub osi)
                                   && (znaleziono_teren==TRUE)) //znaleziono teren projektowany lub istniejacy,
                                       //rzeczywiety lub interpolowany

                                {
                                    P=(T_Point *)adp3;
                                    fi_kanal=0;
                                    if (adp4 != NULL)  //istniej rowniez punkt gorny
                                    {
                                        P1=(T_Point *)adp4;
                                        fi_kanal=(P1->y-P->y);
                                    }

                                    if (b->kod_obiektu==B_KPOMIAR)
                                    {
                                        // if ((P->x > xi) && (P->x <= xj))
                                        if ((Check_if_GE(P->x, xi)==TRUE) && (Check_if_LE(P->x, xj)==TRUE))
                                        {


                                            //     P->obiektt2=O2Block;
                                            //     if (adp4 != NULL) P1->obiektt2=O2Block;

                                            if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                            else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                            else wyrownywanie=0;

                                            //zmiana polozenia punktu (y)
                                            //   if (adp5 != NULL)
                                            //    {P3=(T_Point *)adp5;
                                            //     yi=P3->y;
                                            //    }
                                            //     else
                                            //      {P2=(T_Point *)adp6;
                                            //        yi=P2->y;
                                            //      }

                                            if (wyrownywanie==wyrownywanie_dolem)
                                            {
                                                P->y=(float)(yi-zaglebienie);
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y=(float)(P->y+fi_kanal);
                                                }
                                            }
                                            else if (wyrownywanie==wyrownywanie_gora)
                                            {
                                                P->y=(float)(yi-zaglebienie-fi_kanal);
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y=(float)(P->y+fi_kanal);
                                                }
                                            }
                                            else if (wyrownywanie==wyrownywanie_osiowe)
                                            {
                                                P->y=(float)(yi-zaglebienie-(fi_kanal/2.));
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y=(float)(P->y+fi_kanal);
                                                }
                                            }
                                        }
                                    }
                                    else if (b->kod_obiektu==B_PPOMIAR)
                                    {
                                        //   if ((P->x > xi) && (P->x < xj))
                                        if ((Check_if_GE(P->x, xi)==TRUE) && (Check_if_GE(P->x, xj)==FALSE))
                                        {

                                            //   P->obiektt2=O2Block;
                                            //   if (adp4 != NULL) P1->obiektt2=O2Block;
                                            if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                            else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                            else wyrownywanie=0;

                                            //zmiana polozenia punktu (y)
                                            //     if (adp5 != NULL)
                                            //     {P3=(T_Point *)adp5;
                                            //      yi=P3->y;
                                            //     }
                                            //      else
                                            //       {P2=(T_Point *)adp6;
                                            //         yi=P2->y;
                                            //       }

                                            //   P->y=yi-zaglebienie;
                                            //przesuniecie punktu gornego
                                            //   if (adp4 != NULL)
                                            //    {
                                            //      P1->y=P->y+fi_kanal;
                                            //    }
                                            if (wyrownywanie==wyrownywanie_dolem)
                                            {
                                                P->y=(float)(yi-zaglebienie);
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y=(float)(P->y+fi_kanal);
                                                }
                                            }
                                            else if (wyrownywanie==wyrownywanie_gora)
                                            {
                                                P->y=(float)(yi-zaglebienie-fi_kanal);
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y=(float)(P->y+fi_kanal);
                                                }
                                            }
                                            else if (wyrownywanie==wyrownywanie_osiowe)
                                            {
                                                P->y=(float)(yi-zaglebienie-(fi_kanal/2.));
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y=(float)(P->y+fi_kanal);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            adp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                        }
                        else adp+=sizeof(NAGLOWEK) + b->n;
                    }
                    else adp+=sizeof(NAGLOWEK) + b->n;
                }
                else adp+=sizeof(NAGLOWEK) + L->n;
                L=(LINIA *) adp;
            }
            //+++++++++++++++
        }
        else
        {
            ClearInfo0();
            return 0;
        }
        ClearInfo0();
        return 1;
    }
    ClearInfo0();
    return 0;
}

int zmien_spadek(int kanal)
{
    int l_kr;
    double xi, yi, xj, yj, xk, yk, xir, xjr;
    char *adp, *adp1, *adp2, *adp3, *adp4, *adk;
    BLOK *b;
    T_Point *P, *P1;
    LINIA *L;
    double fi_kanal, del_x, del_x1, del_y, del_y1;
    char sk[80];
    int retval_no = 1 ;
    double buf_ret [1] ;
    double spadek;
    double skala_profilu;
    double local_x;
    double first_hektometr;
    char O2Block;
    int wyrownywanie;
    int b_warstwa;

    local_x=get_localx();
    first_hektometr=get_hektometr_p();
    skala_profilu=get_skala_profilu_x();

    //  if (wyrownywanie==0) O2Block=O2BlockSpecial;
    //   else if (wyrownywanie==1) O2Block=O2BlockHatch25;
    //    else if (wyrownywanie==2) O2Block=O2BlockHatch50;


    l_kr=Get_First_Second_Pomiar_Param(0, kanal, &xi, &yi);
    if (l_kr>0)
    {
        xir=milimetryob(xi - local_x) * skala_profilu + (first_hektometr * 100);

        sprintf(sk,"Odległość początkowa: %12.3f m",xir);
        InfoListStr0(sk);
        l_kr=Get_First_Second_Pomiar_Param(1, kanal, &xj, &yj);
        if (l_kr>0)
        {
            xjr =  milimetryob(xj - local_x) * skala_profilu + (first_hektometr * 100);
            sprintf(sk,"Odległość początkowa: %12.3f m, końcowa: %12.3f m",xir, xjr);
            InfoListStr0(sk);
            //odczyt wartosci spadku
            strcpy(sk,"");
            if (!read_esc(sk,60,132))
            {
                ClearInfo0();
                return 0;
            }
            if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
            {
                ClearInfo0();
                return 0;
            }

            skala_profilu=get_skala_profilu_x();
            spadek = buf_ret [0] ;
            spadek *= skala_profilu;
            spadek /= (-1000);


            //korekta punktow
            if (Check_if_Equal(xi, xj)==TRUE) return 0;
            //  if (xj<xi)
            //   {
            //     xk=xj;  yk=yj; xj=xi; yj=yi; xi=xk; yi=yk;
            //   }
            //+++++++++++++++
            adp = dane ;
            adk = dane + dane_size ;

            del_x=fabs(xj-xi);
            //del_y=yj-yi;
            //zadany spadek
            del_y=(spadek*del_x);


            L = (LINIA*)adp;
            while (L->obiekt != Okoniec && adp < adk)
            {
                if (L->obiekt == OdBLOK)
                {
                    b=(BLOK *)adp;
                    b_warstwa=get_block_layer(b);
                    if (b_warstwa==Current_Layer)
                    {
                        if ((Layers[b_warstwa].edit==1) &&
                             (Layers[b_warstwa].on==1))
                        {
                            if (((b->kod_obiektu==B_PPOMIAR) ||
                                 (b->kod_obiektu==B_KPOMIAR))
                              && ((b->atrybut != Abad) &&
                                 (b->atrybut != Ausuniety)))
                            {
                                adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                                adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                                if (kanal==0) //projektowany
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 3, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 4, 0);
                                }
                                else if (kanal==1) //istniejacy
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 1, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 2, 0);
                                }
                                else if (kanal==5) //teren projektowany
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 5, 0);
                                    adp4=NULL;
                                }
                                else if (kanal==6) //teren istniejacy
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 6, 0);
                                    adp4=NULL;
                                }

                                if (adp3 != NULL)  //znaleziono punkt podstawowy (rzedna dna lub osi)
                                {
                                    P=(T_Point *)adp3;
                                    fi_kanal=0;
                                    if (adp4 != NULL)  //istniej rowniez punkt gorny
                                    {
                                        P1=(T_Point *)adp4;
                                        fi_kanal=(P1->y-P->y);
                                    }
                                    //interpolowanie polozenia y na podstawie polozenia x
                                    if (xj > xi) //od lewej do prawej
                                    {
                                        if (b->kod_obiektu==B_KPOMIAR)
                                        {
                                            // if ((P->x > xi) && (P->x <= xj))

                                            if ((Check_if_LE(P->x, xi)==FALSE) && (Check_if_LE(P->x, xj)==TRUE))
                                            {
                                                //   P->obiektt2=O2Block;
                                                //   if (adp4 != NULL) P1->obiektt2=O2Block;
                                                if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                                else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                                else wyrownywanie=0;

                                                //zmiana polozenia punktu (y)

                                                del_x1=fabs(P->x-xi);
                                                del_y1=(del_x1/del_x) * del_y;
                                                if (wyrownywanie==wyrownywanie_dolem)
                                                {
                                                    P->y=yi+del_y1;
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=P->y+fi_kanal;
                                                    }
                                                }
                                                else
                                                    if (wyrownywanie==wyrownywanie_gora)
                                                    {
                                                        P->y=(float)(yi+del_y1-fi_kanal);
                                                        //przesuniecie punktu gornego
                                                        if (adp4 != NULL)
                                                        {
                                                            P1->y=(float)(P->y+fi_kanal);
                                                        }
                                                    }
                                                    else
                                                        if (wyrownywanie==wyrownywanie_osiowe)
                                                        {
                                                            //przesuniecie punktu gornego
                                                            if (adp4 != NULL)
                                                            {
                                                                P->y=(float)(yi+del_y1-(fi_kanal/2.));
                                                                P1->y=(float)(P->y+fi_kanal);
                                                            }
                                                            else P->y=(float)(yi+del_y1);
                                                        }


                                            }
                                        }
                                        else if (b->kod_obiektu==B_PPOMIAR)
                                        {
                                            //   if ((P->x > xi) && (P->x < xj))
                                            if ((Check_if_LE(P->x, xi)==FALSE) && (Check_if_GE(P->x, xj)==FALSE))
                                            {

                                                //   P->obiektt2=O2Block;
                                                //   if (adp4 != NULL) P1->obiektt2=O2Block;
                                                if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                                else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                                else wyrownywanie=0;

                                                //zmiana polozenia punktu (y)

                                                del_x1=fabs(P->x-xi);
                                                del_y1=(del_x1/del_x) * del_y;
                                                if (wyrownywanie==wyrownywanie_dolem)
                                                {
                                                    P->y=(float)(yi+del_y1);
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=(float)(P->y+fi_kanal);
                                                    }
                                                }
                                                else
                                                    if (wyrownywanie==wyrownywanie_gora)
                                                    {
                                                        P->y=(float)(yi+del_y1-fi_kanal);
                                                        //przesuniecie punktu gornego
                                                        if (adp4 != NULL)
                                                        {
                                                            P1->y=(float)(P->y+fi_kanal);
                                                        }
                                                    }
                                                    else
                                                        if (wyrownywanie==wyrownywanie_osiowe)
                                                        {
                                                            //przesuniecie punktu gornego
                                                            if (adp4 != NULL)
                                                            {
                                                                P->y=(float)(yi+del_y1-(fi_kanal/2.));
                                                                P1->y=(float)(P->y+fi_kanal);
                                                            }
                                                            else P->y=(float)(yi+del_y1);
                                                        }

                                            }
                                        }
                                    }
                                    else  //od prawej do lewej
                                    {
                                        if (b->kod_obiektu==B_KPOMIAR)
                                        {
                                            //  if ((P->x < xi) && (P->x >= xj))

                                            if ((Check_if_GE(P->x, xi)==FALSE) &&
                                                ((Check_if_LE(P->x, xj)==FALSE) ||
                                                ((Check_if_GE(P->x, xj)==TRUE)) && (alone_kpomiar(kanal, xj)==TRUE)))
                                            {
                                                //    P->obiektt2=O2Block;
                                                //     if (adp4 != NULL) P1->obiektt2=O2Block;
                                                if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                                else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                                else wyrownywanie=0;

                                                //zmiana polozenia punktu (y)

                                                del_x1=fabs(P->x-xi);
                                                del_y1=(del_x1/del_x) * del_y;
                                                if (wyrownywanie==wyrownywanie_dolem)
                                                {
                                                    P->y=(float)(yi+del_y1);
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=(float)(P->y+fi_kanal);
                                                    }
                                                }
                                                else
                                                    if (wyrownywanie==wyrownywanie_gora)
                                                    {
                                                        P->y=(float)(yi+del_y1-fi_kanal);
                                                        //przesuniecie punktu gornego
                                                        if (adp4 != NULL)
                                                        {
                                                            P1->y=(float)(P->y+fi_kanal);
                                                        }
                                                    }
                                                    else
                                                        if (wyrownywanie==wyrownywanie_osiowe)
                                                        {
                                                            //przesuniecie punktu gornego
                                                            if (adp4 != NULL)
                                                            {
                                                                P->y=(float)(yi+del_y1-(fi_kanal/2.));
                                                                P1->y=(float)(P->y+fi_kanal);
                                                            }
                                                            else P->y=(float)(yi+del_y1);
                                                        }

                                            }
                                        }
                                        else if (b->kod_obiektu==B_PPOMIAR)
                                        {
                                            //     if ((P->x < xi) && (P->x > xj))
                                            if ((Check_if_GE(P->x, xi)==FALSE) && (Check_if_GE(P->x, xj)==TRUE))
                                            {

                                                //   P->obiektt2=O2Block;
                                                //   if (adp4 != NULL) P1->obiektt2=O2Block;
                                                if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                                else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                                else wyrownywanie=0;

                                                //zmiana polozenia punktu (y)

                                                del_x1=fabs(P->x-xi);
                                                del_y1=(del_x1/del_x) * del_y;
                                                if (wyrownywanie==wyrownywanie_dolem)
                                                {
                                                    P->y=(float)(yi+del_y1);
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=(float)(P->y+fi_kanal);
                                                    }
                                                }
                                                else if (wyrownywanie==wyrownywanie_gora)
                                                {
                                                    P->y=(float)(yi+del_y1-fi_kanal);
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=(float)(P->y+fi_kanal);
                                                    }
                                                }
                                                else if (wyrownywanie==wyrownywanie_osiowe)
                                                {
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P->y=(float)(yi+del_y1-(fi_kanal/2.));
                                                        P1->y=(float)(P->y+fi_kanal);
                                                    }
                                                    else P->y=(float)(yi+del_y1);
                                                }

                                            }
                                        }
                                    }
                                }
                            }
                            adp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                        }
                        else adp+=sizeof(NAGLOWEK) + b->n;
                    }
                    else adp+=sizeof(NAGLOWEK) + b->n;
                }
                else adp+=sizeof(NAGLOWEK) + L->n;
                L=(LINIA *) adp;
            }
            //+++++++++++++++
        }
        else
        {
            ClearInfo0();
            return 0;
        }
        ClearInfo0();
        return 1;
    }
    ClearInfo0();
    return 0;
}

int nadaj_spadek(int kanal)
{
    int l_kr;
    double xi, yi, xj, yj, xk, yk;
    char *adp, *adp1, *adp2, *adp3, *adp4, *adk;
    BLOK *b;
    T_Point *P, *P1;
    LINIA *L;
    double fi_kanal, del_x, del_x1, del_y, del_y1;
    char sk[80];
    int retval_no = 1 ;
    double buf_ret [1] ;
    double spadek;
    double skala_profilu;
    double del_end_of_section;
    double orig_py;
    BOOL end_of_section;
    double local_x;
    double first_hektometr;
    char O2Block;
    int wyrownywanie;
    int b_warstwa;

    local_x=get_localx();
    first_hektometr=get_hektometr_p();
    skala_profilu=get_skala_profilu_x();

    //  if (wyrownywanie==0) O2Block=O2BlockSpecial;
    //   else if (wyrownywanie==1) O2Block=O2BlockHatch25;
    //    else if (wyrownywanie==2) O2Block=O2BlockHatch50;

    l_kr=Get_First_Second_Pomiar_Param(0, kanal, &xi, &yi);
    if (l_kr>0)
    {
        l_kr=Get_First_Second_Pomiar_Param(1, kanal, &xj, &yj);
        if (l_kr>0)
        {
            //odczyt wartosci spadku
            strcpy(sk,"");
            if (!read_esc(sk,60,132))
            {
                ClearInfo0();
                return 0;
            }
            if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
            {
                ClearInfo0();
                return 0;
            }

            spadek = buf_ret [0] ;
            spadek *= skala_profilu;
            spadek /= (-1000);


            //korekta punktow
            if (Check_if_Equal(xi, xj)==TRUE) return 0;
            //  if (xj<xi)
            //   {
            //     xk=xj;  yk=yj; xj=xi; yj=yi; xi=xk; yi=yk;
            //   }
            //+++++++++++++++
            adp = dane ;
            adk = dane + dane_size ;

            del_x=fabs(xj-xi);
            //del_y=yj-yi;
            //zadany spadek
            del_y=(spadek*del_x);

            del_end_of_section=0;


            L = (LINIA*)adp;
            while (L->obiekt != Okoniec && adp < adk)
            {
                if (L->obiekt == OdBLOK)
                {
                    b=(BLOK *)adp;
                    b_warstwa=get_block_layer(b);
                    if (b_warstwa==Current_Layer)
                    {
                        if ((Layers[b_warstwa].edit==1) &&
                            (Layers[b_warstwa].on==1))
                        {
                            if (((b->kod_obiektu==B_PPOMIAR) ||
                                 (b->kod_obiektu==B_KPOMIAR))
                              && ((b->atrybut != Abad) &&
                                 (b->atrybut != Ausuniety)))
                            {
                                adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                                adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                                if (kanal==0) //projektowany
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 3, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 4, 0);
                                }
                                else
                                {
                                    adp3=find_obj(adp1, adp2, Opoint, 1, 0);
                                    adp4=find_obj(adp1, adp2, Opoint, 2, 0);
                                }

                                if (adp3 != NULL)  //znaleziono punkt podstawowy (rzedna dna lub osi)
                                {
                                    P=(T_Point *)adp3;
                                    fi_kanal=0;
                                    if (adp4 != NULL)  //istniej rowniez punkt gorny
                                    {
                                        P1=(T_Point *)adp4;
                                        fi_kanal=(P1->y-P->y);
                                    }
                                    //interpolowanie polozenia y na podstawie polozenia x
                                    if (xj > xi) //od lewej do prawej
                                    {
                                        if (b->kod_obiektu==B_KPOMIAR)
                                        {
                                            // if ((P->x > xi) && (P->x <= xj))
                                            if ((Check_if_LE(P->x, xi)==FALSE) && (Check_if_LE(P->x, xj)==TRUE))
                                            {

                                                //    P->obiektt2=O2Block;
                                                //    if (adp4 != NULL) P1->obiektt2=O2Block;
                                                if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                                else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                                else wyrownywanie=0;

                                                //odszukiwanie wartosci przesuniecia stalego
                                                end_of_section=FALSE;
                                                if (Check_if_Equal(P->x, xj)==TRUE)
                                                {
                                                    end_of_section=TRUE;
                                                    orig_py=P->y;
                                                }
                                                //zmiana polozenia punktu (y)


                                                del_x1=fabs(P->x-xi);
                                                del_y1=(del_x1/del_x) * del_y;
                                                if (wyrownywanie==wyrownywanie_dolem)
                                                {
                                                    P->y=yi+del_y1;
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=P->y+fi_kanal;
                                                    }
                                                }
                                                else if (wyrownywanie==wyrownywanie_gora)
                                                {
                                                    P->y=yi+del_y1-fi_kanal;
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=P->y+fi_kanal;
                                                    }
                                                }
                                                else if (wyrownywanie==wyrownywanie_osiowe)
                                                {
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P->y=yi+del_y1-(fi_kanal/2);
                                                        P1->y=P->y+fi_kanal;
                                                    }
                                                    else P->y=yi+del_y1;
                                                }

                                                if (end_of_section==TRUE) del_end_of_section=P->y-orig_py;

                                            }
                                        }
                                        else if (b->kod_obiektu==B_PPOMIAR)
                                        {
                                            //   if ((P->x > xi) && (P->x < xj))
                                            if ((Check_if_LE(P->x, xi)==FALSE) && (Check_if_GE(P->x, xj)==FALSE))
                                            {

                                                //    P->obiektt2=O2Block;
                                                //   if (adp4 != NULL) P1->obiektt2=O2Block;
                                                if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                                else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                                else wyrownywanie=0;

                                                //zmiana polozenia punktu (y)

                                                del_x1=fabs(P->x-xi);
                                                del_y1=(del_x1/del_x) * del_y;

                                                if (wyrownywanie==wyrownywanie_dolem)
                                                {
                                                    P->y=(float)(yi+del_y1);
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=(float)(P->y+fi_kanal);
                                                    }
                                                }
                                                else
                                                    if (wyrownywanie==wyrownywanie_gora)
                                                    {
                                                        P->y=(float)(yi+del_y1-fi_kanal);
                                                        //przesuniecie punktu gornego
                                                        if (adp4 != NULL)
                                                        {
                                                            P1->y=(float)(P->y+fi_kanal);
                                                        }
                                                    }
                                                    else
                                                        if (wyrownywanie==wyrownywanie_osiowe)
                                                        {
                                                            //przesuniecie punktu gornego
                                                            if (adp4 != NULL)
                                                            {
                                                                P->y=(float)(yi+del_y1-(fi_kanal/2.));
                                                                P1->y=(float)(P->y+fi_kanal);
                                                            }
                                                            else P->y=(float)(yi+del_y1);
                                                        }

                                            }
                                        }
                                    }
                                    else  //od prawej do lewej
                                    {
                                        if (b->kod_obiektu==B_KPOMIAR)
                                        {
                                            //  if ((P->x < xi) && (P->x >= xj))
                                            if ((Check_if_GE(P->x, xi)==FALSE) &&
                                                ((Check_if_LE(P->x, xj)==FALSE) ||
                                                ((Check_if_GE(P->x, xj)==TRUE)) && (alone_kpomiar(kanal, xj)==TRUE)))

                                            {

                                                //   P->obiektt2=O2Block;
                                                //   if (adp4 != NULL) P1->obiektt2=O2Block;
                                                if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                                else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                                else wyrownywanie=0;

                                                end_of_section=FALSE;
                                                if (Check_if_Equal(P->x, xj)==TRUE)
                                                {
                                                    end_of_section=TRUE;
                                                    orig_py=P->y;
                                                }
                                                //zmiana polozenia punktu (y)
                                                del_x1=fabs(P->x-xi);
                                                del_y1=(del_x1/del_x) * del_y;


                                                if (wyrownywanie==wyrownywanie_dolem)
                                                {
                                                    P->y=(float)(yi+del_y1);
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=(float)(P->y+fi_kanal);
                                                    }
                                                }
                                                else
                                                    if (wyrownywanie==wyrownywanie_gora)
                                                    {
                                                        P->y=(float)(yi+del_y1-fi_kanal);
                                                        //przesuniecie punktu gornego
                                                        if (adp4 != NULL)
                                                        {
                                                            P1->y=(float)(P->y+fi_kanal);
                                                        }
                                                    }
                                                    else
                                                        if (wyrownywanie==wyrownywanie_osiowe)
                                                        {
                                                            //przesuniecie punktu gornego
                                                            if (adp4 != NULL)
                                                            {
                                                                P->y=(float)(yi+del_y1-(fi_kanal/2.));
                                                                P1->y=(float)(P->y+fi_kanal);
                                                            }
                                                            else P->y=(float)(yi+del_y1);
                                                        }
                                                if (end_of_section==TRUE) del_end_of_section=P->y-orig_py;
                                            }
                                        }
                                        else if (b->kod_obiektu==B_PPOMIAR)
                                        {
                                            //     if ((P->x < xi) && (P->x > xj))
                                            if ((Check_if_GE(P->x, xi)==FALSE) && (Check_if_GE(P->x, xj)==TRUE))
                                            {

                                                //    P->obiektt2=O2Block;
                                                //    if (adp4 != NULL) P1->obiektt2=O2Block;
                                                if (P->obiektt2==O2BlockHatch25) wyrownywanie=1;
                                                else if (P->obiektt2==O2BlockHatch50) wyrownywanie=2;
                                                else wyrownywanie=0;

                                                end_of_section=FALSE;
                                                if (Check_if_Equal(P->x, xj)==TRUE)
                                                {
                                                    end_of_section=TRUE;
                                                    orig_py=P->y;
                                                }

                                                //zmiana polozenia punktu (y)

                                                del_x1=fabs(P->x-xi);
                                                del_y1=(del_x1/del_x) * del_y;

                                                if (wyrownywanie==wyrownywanie_dolem)
                                                {
                                                    P->y=(float)(yi+del_y1);
                                                    //przesuniecie punktu gornego
                                                    if (adp4 != NULL)
                                                    {
                                                        P1->y=(float)(P->y+fi_kanal);
                                                    }
                                                }
                                                else
                                                    if (wyrownywanie==wyrownywanie_gora)
                                                    {
                                                        //przesuniecie punktu gornego
                                                        if (adp4 != NULL)
                                                        {
                                                            P1->y=(float)(yi+del_y1);
                                                            P->y=(float)(P1->y-fi_kanal);
                                                        }
                                                    }
                                                    else
                                                        if (wyrownywanie==wyrownywanie_osiowe)
                                                        {
                                                            //przesuniecie punktu gornego
                                                            if (adp4 != NULL)
                                                            {
                                                                P->y=(float)(yi+del_y1-(fi_kanal/2.));
                                                                P1->y=(float)(P->y+fi_kanal);
                                                            }
                                                            // else P->y=yi+del_y1;
                                                        }

                                                if (end_of_section==TRUE) del_end_of_section=P->y-orig_py;

                                            }
                                        }
                                    }
                                }
                            }
                            adp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                        }
                        else adp+=sizeof(NAGLOWEK) + b->n;
                    }
                    else adp+=sizeof(NAGLOWEK) + b->n;
                }
                else adp+=sizeof(NAGLOWEK) + L->n;
                L=(LINIA *) adp;
            }
            //+++++++++++++++
        }
        else
        {
            ClearInfo0();
            return 0;
        }
    }
    else
    {
        ClearInfo0();
        return 0;
    }

    if (Check_if_Equal(del_end_of_section,0.0)==FALSE)
    {
        //zmiana wspolrzednych poza odcinkiem nadanego spadku
        //+++++++++++++
        adp = dane ;
        adk = dane + dane_size ;

        L = (LINIA*)adp;
        while (L->obiekt != Okoniec && adp < adk)
        {
            if (L->obiekt == OdBLOK)
            {
                b=(BLOK *)adp;
                b_warstwa=get_block_layer(b);
                if (b_warstwa==Current_Layer)
                {
                    if ((Layers[b_warstwa].edit==1) &&
                        (Layers[b_warstwa].on==1))
                    {
                        if (((b->kod_obiektu==B_PPOMIAR) ||
                             (b->kod_obiektu==B_KPOMIAR))
                          && ((b->atrybut != Abad) &&
                             (b->atrybut != Ausuniety)))
                        {
                            adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                            adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                            if (kanal==0) //projektowany
                            {
                                adp3=find_obj(adp1, adp2, Opoint, 3, 0);
                                adp4=find_obj(adp1, adp2, Opoint, 4, 0);
                            }
                            else
                            {
                                adp3=find_obj(adp1, adp2, Opoint, 1, 0);
                                adp4=find_obj(adp1, adp2, Opoint, 2, 0);
                            }

                            if (adp3 != NULL)  //znaleziono punkt podstawowy (rzedna dna lub osi)
                            {
                                P=(T_Point *)adp3;
                                fi_kanal=0;
                                if (adp4 != NULL)  //istniej rowniez punkt gorny
                                {
                                    P1=(T_Point *)adp4;
                                    fi_kanal=(P1->y-P->y);
                                }
                                //interpolowanie polozenia y na podstawie polozenia x
                                if (xj > xi) //od lewej do prawej
                                {
                                    if (b->kod_obiektu==B_KPOMIAR)
                                    {
                                        if (Check_if_LE(P->x, xj)==FALSE)
                                        {
                                            P->y+=(float)del_end_of_section;
                                            //przesuniecie punktu gornego
                                            if (adp4 != NULL)
                                            {
                                                P1->y+=(float)del_end_of_section;
                                            }
                                        }
                                    }
                                    else
                                        if (b->kod_obiektu==B_PPOMIAR)
                                        {
                                            if (Check_if_GE(P->x, xj)==TRUE)
                                            {
                                                P->y+=(float)del_end_of_section;
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y+=(float)del_end_of_section;
                                                }
                                            }
                                        }
                                }
                                else  //od prawej do lewej
                                {
                                    if (b->kod_obiektu==B_KPOMIAR)
                                    {
                                        if ((Check_if_GE(P->x, xj)==FALSE) ||
                                           ((Check_if_LE(P->x, xj)==TRUE) && (alone_kpomiar(kanal,xj)==FALSE)))
                                        {
                                            P->y+=(float)del_end_of_section;
                                            //przesuniecie punktu gornego
                                            if (adp4 != NULL)
                                            {
                                                P1->y+=(float)del_end_of_section;
                                            }
                                        }
                                    }
                                    else
                                        if (b->kod_obiektu==B_PPOMIAR)
                                        {
                                            if (Check_if_GE(P->x, xj)==FALSE)
                                            {
                                                P->y+=(float)del_end_of_section;
                                                //przesuniecie punktu gornego
                                                if (adp4 != NULL)
                                                {
                                                    P1->y+=(float)del_end_of_section;
                                                }
                                            }
                                        }
                                }
                            }
                        }
                        adp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                    }
                    else adp+=sizeof(NAGLOWEK) + b->n;
                }
                else adp+=sizeof(NAGLOWEK) + b->n;
            }
            else adp+=sizeof(NAGLOWEK) + L->n;
            L=(LINIA *) adp;
        }
        //+++++++++++++
    }
    return 1;
}

void zmien_srednice(double odl_p, double odl_k, double srednica1, int kanal)
{
    LINIA *L, *L1;
    BLOK *b;
    TEXT *T, Tp, *t;
    T_Point *P, *P1;
    char *adp, *adp1, *adp2, *adp3, *adp4, *adp5, *adp6, *adk;
    double first_hektometr, last_hektometr, skala_profilu, typ_profilu, poziom_pp;
    double local_x, local_y;
    double a_odl;
    char *bp1;
    char tekst_p[60];
    char tekst_p1[60];
    int l_kr;
    double srednica, srednica_s, Y_os;
    char *srednica_t;
    char srednica_tt[60];
    char material[60];
    int typ_tekstu, typ_punktu1, typ_punktu2;
    BOOL was_changes;
    int b_warstwa;

    //szukanie blokow o odleglosci   odl_p < odl <= odl_p

    was_changes=FALSE;

    adp = dane ;
    adk = dane + dane_size ;

    first_hektometr=get_hektometr_p();
    last_hektometr=get_hektometr_k();
    skala_profilu=get_skala_profilu_x();

    typ_profilu=get_typ_profilu();
    tablica_0_1(typ_profilu);

    poziom_pp=get_poziom_pp();
    local_x=get_localx();
    local_y=get_localy();

    L = (LINIA*)adp;
    while (L->obiekt != Okoniec && adp < adk)
    {
        if (L->obiekt == OdBLOK)
        {
            b=(BLOK *)adp;
            b_warstwa=get_block_layer(b);
            if (b_warstwa==Current_Layer)
            {
                if ((Layers[b_warstwa].edit==1) &&
                    (Layers[b_warstwa].on==1))
                {
                    if (((b->kod_obiektu==B_PPOMIAR) ||
                        (b->kod_obiektu==B_KPOMIAR))
                        && ((b->atrybut != Abad) &&
                            (b->atrybut != Ausuniety)))
                    {
                        adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                        adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                        adp3=find_obj(adp1, adp2, Otekst, n_odleglosc, 0);
                        if (adp3 != NULL)
                        {
                            T=(TEXT *) adp3;
                            adp4=adp3 + T->n + sizeof(NAGLOWEK) ;
                            L1 = (LINIA*) adp4;
                            a_odl = (milimetryob(L1->x1-local_x)*skala_profilu) + (first_hektometr * 100);
                            if (((a_odl > odl_p) && (a_odl <= odl_k) && (b->kod_obiektu==B_KPOMIAR)) ||
                               ((a_odl >= odl_p) && (a_odl < odl_k) && (b->kod_obiektu==B_PPOMIAR)))//lezy w zakresie
                            {
                                //poszukiwanie srednicy / materialu

                                if (kanal==0) adp4=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_p, 0);
                                else adp4=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_i /*n_srednica_kanalu_p*/, 0);
                                if (adp4 != NULL)
                                {
                                    //rozbior lancucha na srednice, jednostki, material
                                    T=(TEXT *)adp4;
                                    srednica_t = strpbrk(T->text, "-1234567890.,");
                                    if (srednica_t != NULL)
                                    {
                                        strcpy(srednica_tt, decimal_dot(srednica_t));
                                        srednica=0;
                                        srednica = strtod(srednica_tt, &bp1);
                                        if (bp1!=srednica_tt && errno != ERANGE)
                                        {
                                            ;
                                        }
                                        else srednica=0.;

                                        srednica = srednica1;  //???????????????????
                                        while (*bp1==' ') bp1++;

                                        strcpy(material,bp1);
                                        if (strlen(material)>1) bp1 += 2;
                                        while (*bp1==' ') bp1++;
                                        strcpy(material,bp1);

                                        memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);

                                        strcpy(tekst_p,Fi);
                                        sprintf(tekst_p1,"%g",srednica);
                                        //// if (srednica<=10)
                                        //// {
                                        ////   l_kr=add_000(tekst_p1,1);
                                        //// }
                                        decimal(tekst_p1);

                                        strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
                                        if (srednica>10) strcpy(tekst_p1," mm");
                                        else strcpy(tekst_p1," m");
                                        strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

                                        strcpy(tekst_p1," ");
                                        strncat(tekst_p,tekst_p1,1);
                                        strncat(tekst_p,material,sizeof(tekst_p1)-strlen(tekst_p1)-1);

                                        strcpy(&Tp.text[0],tekst_p);

                                        decimal(Tp.text);

                                        Tp.dl = strlen(Tp.text);
                                        Tp.n = T18 + Tp.dl;

                                        //Odszukanie punktow, oraz rozstawienie
                                        // punktow wzgledem dna lub osi
                                        //jezeli srednica>10 to ustawienie w punktach
                                        //flagi obiektt2=O3Os
                                        if (kanal==0) //kanal projektowany
                                        {
                                            typ_tekstu=n_rzedna_kanalu_p;
                                            typ_punktu1=3;
                                            typ_punktu2=4;
                                        }
                                        else
                                        {
                                            typ_tekstu=n_rzedna_kanalu_i;
                                            typ_punktu1=1;
                                            typ_punktu2=2;
                                        }

                                        adp5=find_obj(adp1, adp2, Opoint, typ_punktu1, 0);
                                        adp6=find_obj(adp1, adp2, Opoint, typ_punktu2, 0);
                                        if ((adp5 != NULL) && (adp6 != NULL))
                                        {
                                            P=(T_Point *) adp5;
                                            P1=(T_Point *) adp6;

                                            if (srednica>10)
                                            {
                                                P->obiektt3=O3Os;
                                                P1->obiektt3=O3Os;
                                                srednica_s=jednostkiOb(srednica/1000);
                                            }
                                            else  //wymiarowanie do dna
                                            {
                                                P->obiektt3=O3Dno;
                                                P1->obiektt3=O3Dno;
                                                srednica_s=jednostkiOb(srednica);
                                            }
                                            //jezeli licowanie jest do dna
                                            if (P->obiektt2==O2BlockHatch25)
                                            {
                                                P1->y=(float)(P->y+srednica_s);
                                                //P->y bez zmian
                                            }
                                            //jezeli licowanie jest do osi
                                            else if (P->obiektt2==O2BlockHatch50)
                                            {
                                                Y_os=(P->y + P1->y)/2;
                                                P->y=(float)(Y_os-(srednica_s/2.));
                                                P1->y=(float)(Y_os+(srednica_s/2.));
                                            }
                                            //jezeli do stropu
                                            else
                                            {
                                                //P1->y bez zmian
                                                P->y=(float)(P1->y-srednica_s);
                                            }
                                            was_changes=TRUE;
                                        }

                                        if ( (t = korekta_obiekt((void *)adp4, (void *)&Tp)) == NULL) return;
                                        adp=(char *)t;
                                        adk=dane+dane_size;
                                        L=(LINIA*)adp;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (L->obiekt==OdBLOK)
        {
            b= (BLOK *) adp;
            b_warstwa=get_block_layer(b);
            if (b_warstwa==Current_Layer)
            {
                if ((Layers[b_warstwa].edit==1) &&
                    (Layers[b_warstwa].on==1))
                {
                    adp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                }
                else adp+=sizeof(NAGLOWEK) + b->n;
            }
            else adp+=sizeof(NAGLOWEK) + b->n;
        }
        else adp += sizeof(NAGLOWEK) + L->n;
        L=(LINIA *)adp;
    }
    if (was_changes==TRUE)
    {
        l_kr=przesuniecie_profilu(0,0);
        l_kr=change_cavity_p(0);
        l_kr=usuwanie_profilu();
        l_kr=sortowanie_odl();
        if (l_kr>0) l_kr=aktualizacja_profilu();
    }
}

void zmien_material(double odl_p, double odl_k, char *material, int kanal)
{
    LINIA *L, *L1;
    BLOK *b;
    TEXT *T, Tp, *t;
    char *adp, *adp1, *adp2, *adp3, *adp4, *adk;
    double first_hektometr, last_hektometr, skala_profilu, typ_profilu, poziom_pp;
    double local_x, local_y;
    double a_odl;
    char *bp1;
    char tekst_p[60];
    char tekst_p1[60];
    int l_kr;
    double srednica;
    char *srednica_t;
    char srednica_tt[60];
    int b_warstwa;

    //szukanie blokow o odleglosci   odl_p < odl <= odl_p
    adp = dane ;
    adk = dane + dane_size ;

    first_hektometr=get_hektometr_p();
    last_hektometr=get_hektometr_k();
    skala_profilu=get_skala_profilu_x();

    typ_profilu=get_typ_profilu();
    tablica_0_1(typ_profilu);

    poziom_pp=get_poziom_pp();
    local_x=get_localx();
    local_y=get_localy();

    L = (LINIA*)adp;
    while (L->obiekt != Okoniec && adp < adk)
    {
        if (L->obiekt == OdBLOK)
        {
            b=(BLOK *)adp;
            b_warstwa=get_block_layer(b);
            if (b_warstwa==Current_Layer)
            {
                if ((Layers[b_warstwa].edit==1) &&
                    (Layers[b_warstwa].on==1))
                {
                    if (((b->kod_obiektu==B_PPOMIAR) ||
                        (b->kod_obiektu==B_KPOMIAR))
                        && ((b->atrybut != Abad) &&
                            (b->atrybut != Ausuniety)))
                    {
                        adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                        adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                        adp3=find_obj(adp1, adp2, Otekst, n_odleglosc, 0);
                        if (adp3 != NULL)
                        {
                            T=(TEXT *) adp3;
                            adp4=adp3 + T->n + sizeof(NAGLOWEK) ;
                            L1 = (LINIA*) adp4;
                            a_odl = (milimetryob(L1->x1-local_x)*skala_profilu) + (first_hektometr * 100);
                            if ((a_odl > odl_p) && (a_odl <= odl_k))  //lezy w zakresie
                            {
                                //poszukiwanie srednicy / materialu
                                if (kanal==0) adp4=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_p, 0);
                                else adp4=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_i /*n_srednica_kanalu_p*/, 0);
                                if (adp4 != NULL)
                                {
                                    //rozbior lancucha na srednice, jednostki, material
                                    T=(TEXT *)adp4;
                                    srednica_t = strpbrk(T->text, "-1234567890.,");
                                    if (srednica_t != NULL)
                                    {
                                        strcpy(srednica_tt, decimal_dot(srednica_t));
                                        srednica=0;
                                        double temp_srednica = strtod(srednica_tt, &bp1);
                                        if (bp1!=srednica_tt && errno != ERANGE)
                                        {
                                            srednica = temp_srednica;
                                        }

                                        memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);

                                        strcpy(tekst_p,Fi);
                                        sprintf(tekst_p1,"%g",srednica);
                                        //// if (srednica<=10)
                                        //// {
                                        ////  l_kr=add_000(tekst_p1,1);
                                        //// }
                                        decimal(tekst_p1);

                                        strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);
                                        if (srednica>10) strcpy(tekst_p1," mm");
                                        else strcpy(tekst_p1," m");
                                        strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

                                        strcpy(tekst_p1," ");
                                        strncat(tekst_p,tekst_p1,1);
                                        strncat(tekst_p,material,sizeof(tekst_p1)-strlen(tekst_p1)-1);

                                        strcpy(&Tp.text[0],tekst_p);

                                        decimal(Tp.text);

                                        Tp.dl = strlen(Tp.text);
                                        Tp.n = T18 + Tp.dl;
                                        if ( (t = korekta_obiekt((void *)adp4, (void *)&Tp)) == NULL) return;
                                        adp=(char *)t;
                                        adk=dane+dane_size;
                                        L=(LINIA*)adp;
                                        /*
                                        //jezeli srednica>10 to odszukanie punktow i ustawienie
                                        //flagi obiektt2=O3Os
                                        if (srednica>10)
                                         {
                                          adp3=find_obj(adp1, adp2, Opoint, 1, 0);
                                          adp4=find_obj(adp1, adp2, Opoint, 2, 0);
                                          if (adp3 != NULL)
                                           {
                                            P=(T_Point *) adp3;
                                            P->obiektt3=O3Os;
                                           }
                                          if (adp4 != NULL)
                                           {
                                            P=(T_Point *) adp4;
                                            P->obiektt3=O3Os;
                                           }
                                         }
                                         */
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (L->obiekt==OdBLOK)
        {
            b= (BLOK *) adp;
            b_warstwa=get_block_layer(b);
            if (b_warstwa==Current_Layer)
            {
                if ((Layers[b_warstwa].edit==1) &&
                    (Layers[b_warstwa].on==1))
                {
                    adp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                }
                else adp += sizeof(NAGLOWEK) + b->n;
            }
            else adp += sizeof(NAGLOWEK) + b->n;
        }
        else adp += sizeof(NAGLOWEK) + L->n;
        L=(LINIA *)adp;
    }
}

void zmien_nawierzchnie(double odl_p, double odl_k, char *nawierzchnia)
{
    LINIA *L, *L1;
    BLOK *b;
    TEXT *T, Tp, *t;
    char *adp, *adp1, *adp2, *adp3, *adp4, *adp5, *adk;
    double first_hektometr, last_hektometr, skala_profilu, typ_profilu, poziom_pp;
    double local_x, local_y;
    double a_odl;
    char *bp1;
    char tekst_p[60];
    char tekst_p1[60];
    int l_kr;
    double srednica;
    char *srednica_t;
    char srednica_tt[60];
    int b_warstwa;

    //szukanie blokow o odleglosci   odl_p < odl <= odl_p
    adp = dane ;
    adk = dane + dane_size ;

    first_hektometr=get_hektometr_p();
    last_hektometr=get_hektometr_k();
    skala_profilu=get_skala_profilu_x();

    typ_profilu=get_typ_profilu();
    tablica_0_1(typ_profilu);

    poziom_pp=get_poziom_pp();
    local_x=get_localx();
    local_y=get_localy();

    L = (LINIA*)adp;
    while (L->obiekt != Okoniec && adp < adk)
    {
        if (L->obiekt == OdBLOK)
        {
            b=(BLOK *)adp;
            b_warstwa=get_block_layer(b);
            if (b_warstwa==Current_Layer)
            {
                if ((Layers[b_warstwa].edit==1) &&
                    (Layers[b_warstwa].on==1))
                {
                    if (((b->kod_obiektu==B_PPOMIAR) ||
                        (b->kod_obiektu==B_KPOMIAR))
                        && ((b->atrybut != Abad) &&
                            (b->atrybut != Ausuniety)))
                    {
                        adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                        adp2=adp+sizeof(NAGLOWEK)+b->n-1;
                        adp3=find_obj(adp1, adp2, Otekst, n_odleglosc, 0);
                        if (adp3 != NULL)
                        {
                            T=(TEXT *) adp3;
                            adp4=adp3 + T->n + sizeof(NAGLOWEK) ;
                            L1 = (LINIA*) adp4;
                            a_odl = (milimetryob(L1->x1-local_x)*skala_profilu) + (first_hektometr * 100);
                            if ((a_odl > odl_p) && (a_odl <= odl_k))  //lezy w zakresie
                            {
                                //poszukiwanie nawierzchni
                                adp4=find_obj(adp1, adp2, Otekst, n_typ_normal, 0);
                                if (adp4 != NULL)
                                {
                                    //ewentualny druki tekst normalny
                                    T=(TEXT *) adp4;
                                    adp5=find_obj(adp4 + sizeof(NAGLOWEK) + T->n, adp2, Otekst, n_typ_normal, 0);
                                    if (adp5 != NULL) adp4=adp5;
                                    //rozbior lancucha na srednice, jednostki, material
                                    T=(TEXT *)adp4;
                                    if ((T->y > (10+local_y)) && (T->y < (15+local_y)))
                                    {
                                        memmove (&Tp, (void *)T, sizeof(NAGLOWEK)+T->n);

                                        strcpy(tekst_p,nawierzchnia);
                                        strcpy(&Tp.text[0],tekst_p);

                                        Tp.dl = strlen(Tp.text);
                                        Tp.n = T18 + Tp.dl;
                                        if ( (t = korekta_obiekt((void *)adp4, (void *)&Tp)) == NULL) return;
                                        adp=(char *)t;
                                        adk=dane+dane_size;
                                        L=(LINIA*)adp;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (L->obiekt==OdBLOK)
        {
            b= (BLOK *) adp;
            b_warstwa=get_block_layer(b);
            if (b_warstwa==Current_Layer)
            {
                if ((Layers[b_warstwa].edit==1) &&
                    (Layers[b_warstwa].on==1))
                {
                    adp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                }
                else adp += sizeof(NAGLOWEK) + b->n;
            }
            else adp += sizeof(NAGLOWEK) + b->n;
        }
        else adp += sizeof(NAGLOWEK) + L->n;
        L=(LINIA *)adp;
    }
}

void zablokuj_profil_on()
{ char st[3];
  static char stY [3]=_YES__, stN [3]=_NO__;

  if (get_lock_prof_aktual() == 1) return;
  put_lock_prof_aktual(1);
  strcpy(st,stY) ;
  menu_par_new ((*mProfil.pola)[16].txt, st) ;
  Change	= TRUE;
}

void zablokuj_profil_off()
{ char st[3];
  static char stY [3]=_YES__, stN [3]=_NO__;

  if (get_lock_prof_aktual() == 0) return;
  put_lock_prof_aktual(0);
  strcpy(st,stN) ;
  menu_par_new ((*mProfil.pola)[16].txt, st) ;
  Change	= TRUE;
}

void uaktualnij_pola_m_profil()
{ char st[3];
  static char stY [3]=_YES__, stN [3]=_NO__;

  if (get_lock_prof_aktual() == 1) strcpy(st,stY) ;
    else strcpy(st,stN) ;
  menu_par_new ((*mProfil.pola)[16].txt, st) ;
}

void save_params (int rodzaj)
{ if (rodzaj==0)
    {
     memmove(&LiniaG0, &LiniaG, sizeof(LINIA));
     memmove(&LukG0, &LukG, sizeof(LUK));
     memmove(&OkragG0, &OkragG, sizeof(OKRAG));
     memmove(&Text_Nag0, &TextG, sizeof(TEXT_NAG));
     current_layer1 = Current_Layer;
    }
     else
      {
       memmove(&LiniaG01, &LiniaG, sizeof(LINIA));
       memmove(&LukG01, &LukG, sizeof(LUK));
       memmove(&OkragG01, &OkragG, sizeof(OKRAG));
       memmove(&Text_Nag01, &TextG, sizeof(TEXT_NAG));
       current_layer1 = Current_Layer;
      }
}

void restore_params (int rodzaj)
/*---------------------------*/
{ if (rodzaj==0)
   {
    memmove(&LiniaG, &LiniaG0, sizeof(LINIA));
    memmove(&LukG, &LukG0, sizeof(LUK));
    memmove(&OkragG, &OkragG0, sizeof(OKRAG));
    memmove(&TextG, &Text_Nag0, sizeof(TEXT_NAG));
    OkragG.warstwa=PointG.warstwa=TextG.warstwa=LukG.warstwa = LiniaG.warstwa= current_layer ;
    Current_Layer = current_layer1;
   }
    else
     {
      memmove(&LiniaG, &LiniaG01, sizeof(LINIA));
      memmove(&LukG, &LukG01, sizeof(LUK));
      memmove(&OkragG, &OkragG01, sizeof(OKRAG));
      memmove(&TextG, &Text_Nag01, sizeof(TEXT_NAG));
      OkragG.warstwa=PointG.warstwa=TextG.warstwa=LukG.warstwa = LiniaG.warstwa= current_layer ;
      Current_Layer = current_layer1;
     }
  layer_info () ;
//  view_line_type( &LiniaG ) ;
}

typedef struct
{ double x;
  double y;
  char kierunek;   // 0  /
} net_point;

int Pline_single_txt(double *x_begin, double *y_begin, Siec_XYZ *siec_xyz_, int n_vertex)
{ LINIA L=Ldef;
  int i;
  double x0_1, y0_1, x0_2, y0_2;
  //jest tablica siec_xyz z n_vertex elementami
  //na ich bazie budujemy polilinie
  if (n_vertex<2) return 0;

  if (FALSE == add_block (B_PLINE, 0))
   {
    return 0;
   }

  memmove(&L,&LiniaG, sizeof(LINIA));
  L.blok=1;
  L.obiektt1 = 0;

  for (i=0; i<(n_vertex-1); i++)
   {

    x0_1=siec_xyz_[i].x;
    y0_1=siec_xyz_[i].y;
    x0_2=siec_xyz_[i+1].x;
    y0_2=siec_xyz_[i+1].y;

    get_global_coords(&x0_1, &y0_1, 0);
    get_global_coords(&x0_2, &y0_2, 0);

    L.x1=(float)x0_1;
    L.y1=(float)y0_1;

    L.x2=(float)x0_2;
    L.y2=(float)y0_2;

    if (dodaj_obiekt((BLOK*)dane, &L)==NULL) return 0;
   }

  return 1;
}

#define epsilon1 0.000000001

int Rysuj_siec(int siec0)    //jezeli siec0==-1, to siec rysowana jest  na podstawie
                             //wartosci odczytanych z pliku TXT
/*---------------------*/
{
    int sel_nr;
    int sel_gor;
    int kolor_sieci;
    long dane_size000,dane_size0000;
    char blok_type[30],blok_type0[30];
    char st_dest[MaxTextLen];
    double x_begin,y_begin;
    BLOK *b;
    char *adp1,*adp2,*adp3,*adp1b,*adp4;
    BOOL first_line,find_next;
    double last_x, last_y;
    OKRAG K=Kdef;
    T_Point P=PointDef;
    NAGLOWEK *nag;
    LINIA *L,L1=Ldef;
    LUK *l;
    net_point point_xy[max_points+1];
    int n_points, i;
    TEXT T=Tdef;
    double l_x1, l_y1, l_x2, l_y2, l_x0, l_y0;
    char st_dest_symbol[MaxTextLen];
    char st_dest_numer[MaxTextLen];
    int st_dest_numer_i, ii, end_char;
    int LengthT;
    int siec;
    int n;
    BOOL read_txt;
    char *blk_address=NULL;
    int go_next=0;
    int n_vertex;
    char sk[MAXPATH];
    char str1[255];
    static   FILE *stru;
    BOOL koniec_pliku;
    char buf[81];
    int k;
    char xyz_x[24], xyz_y[24], xyz_z[24], len[24];
    char xyz_nazwa[128];
    char *bp;
    int t_width, t_height;
    int ret_par;
    BLOK *parent_block;
    int net_color;
    double local_x, local_y, local_z;
    int ret_bak=0;

    read_txt=FALSE;
    siec=siec0;

    if (siec==-1)
    {
        //wybor pliku TXT z danymi

        strcpy (sk, "");
        if (Load_File (sk, ODCZYT_PXYZ, TRUE) == FALSE)
        {
            return 0;
        }
        File_Ext (sk, EXT_PXYZ) ;

        stru=fopen(sk,"rt");
        if (stru==NULL)
        {
            sprintf(str1,"%s %s",_error_opening_file_, sk);
            komunikat_str(str1);
            return 0;
        }

        //wczytywanie tabeli nazw i wspolrzednych, max. 1000
        koniec_pliku=FALSE;
        siec_no=-1;

        siec_xyz_type_no=(Siec_XYZ_TYPE_NO*)malloc(sizeof(Siec_XYZ_TYPE_NO)*SIEC_NO_MAX);
        siec_xyz_max_no=(int*)malloc(sizeof(int)*SIEC_NO_MAX);

        // 1. Allocate the top-level array of pointers (Rows)
        siec_xyz = (Siec_XYZ **)malloc(SIEC_NO_MAX * sizeof(Siec_XYZ *));
        if (siec_xyz == NULL) {
            return 0; // Out of memory
        }

        // 2. Loop through and allocate actual structure arrays for each pointer (Columns)
        for (i = 0; i < SIEC_NO_MAX; i++)
        {
            siec_xyz[i] = (Siec_XYZ *)malloc(SIEC_XYZ_NO_MAX * sizeof(Siec_XYZ));
            if (siec_xyz[i] == NULL) {
                // Memory allocation failed! Clean up previously allocated rows to prevent leaks.
                for (int j = 0; j < i; j++) {
                    free(siec_xyz[j]);
                }
                free(siec_xyz);
                return 0;  // Out of memory
            }

            // Optional but recommended: Zero out the memory so strings/doubles start fresh
            memset(siec_xyz[i], 0, SIEC_XYZ_NO_MAX * sizeof(Siec_XYZ));
        }

        kolor_sieci = koloryps.inne;  //just to initialize

        while ((koniec_pliku==FALSE) && (!feof(stru)))
        {
            if (fgets( buf, 80 , stru ) == NULL )
            {
                koniec_pliku=TRUE;
            }
            else
            {
                buf [ strlen (buf) - 1 ] = '\0';

                if ((strstr(buf,u8"Network")==buf) ||  //starts with name of network
                        (strstr(buf,u8"Sieć")==buf) ||
                        (strstr(buf,u8"Мережа")==buf) ||
                        (strstr(buf,u8"Red")==buf))
                {  //new network
                    siec_no++;
                    if (siec_no==SIEC_NO_MAX)
                    {
                        int old_max = SIEC_NO_MAX;
                        SIEC_NO_MAX+=50;
                        siec_xyz_type_no=realloc(siec_xyz_type_no, sizeof(Siec_XYZ_TYPE_NO)*SIEC_NO_MAX);
                        siec_xyz_max_no=realloc(siec_xyz_max_no, sizeof(int)*SIEC_NO_MAX);
                        // 2. Safely reallocate the top-level pointer array using a temporary pointer
                        Siec_XYZ **temp = (Siec_XYZ **)realloc(siec_xyz, SIEC_NO_MAX * sizeof(Siec_XYZ *));

                        if (temp == NULL) {
                            // Handle error: Out of memory!
                            // 'siec_xyz' and its existing 10 networks are still completely safe here.
                            // You can choose to exit, return an error code, or free everything and quit:
                            for (i = 0; i < old_max; i++)
                            {
                                if (siec_xyz[i] != NULL) {
                                    free(siec_xyz[i]);
                                    siec_xyz[i] = NULL;
                                }
                            }
                            free(siec_xyz);
                            return 1;
                        }

                        // 3. Assign the freshly reallocated space back to your main tracking pointer
                        siec_xyz = temp;
                    }
                    siec_xyz_type_no[siec_no].no=0;
                    siec_xyz_type_no[siec_no].color = kolor_sieci;
                    siec_xyz_max_no[siec_no]=SIEC_XYZ_NO_MAX;
                    
                    //scanning data
                    char net_type[64], coordinates[64], scale[64], point_origin[64], angle[64];
                    
                    ret_par = sscanf(buf, "%s %s %s %s %s", net_type, coordinates, scale, point_origin, angle);
                    if ((ret_par>0) &&  ((strstr(net_type,u8"Network")==net_type) ||  //starts with name of network
                                           (strstr(net_type,u8"Sieć")==net_type) ||
                                           (strstr(net_type,u8"Мережа")==net_type) ||
                                           (strstr(net_type,u8"Red")==net_type)))
                    {
                        char *net_symbol=strchr(net_type,':');
                        if (net_symbol!=NULL) sprintf(siec_xyz_type_no[siec_no].type,"*%s",net_symbol+1);
                        else
                        {
                            if (strchr(net_type,'^')!=NULL)
                                strcpy(siec_xyz_type_no[siec_no].type, "*^");
                            else strcpy(siec_xyz_type_no[siec_no].type, "*#");
                        }
                        
                            if (strstr(net_type, "Pz") != NULL)
                                kolor_sieci = koloryps.Pz;
                            else if (strstr(net_type, "Cok") != NULL)
                                kolor_sieci = koloryps.Cok;
                            else if (strstr(net_type, "Co") != NULL)
                                kolor_sieci = koloryps.Co;
                            else if (strstr(net_type, "T") != NULL)
                                kolor_sieci = koloryps.T;
                            else if (strstr(net_type, "Tk") != NULL)
                                kolor_sieci = koloryps.Tk;
                            else if (strstr(net_type, "e") != NULL)
                                kolor_sieci = koloryps.e;
                            else if (strstr(net_type, "E") != NULL)
                                kolor_sieci = koloryps.E;
                            else if (strstr(net_type, "S") != NULL)
                                kolor_sieci = koloryps.S;
                            else if (strstr(net_type, "D") != NULL)
                                kolor_sieci = koloryps.D;
                            else if (strstr(net_type, "W") != NULL)
                                kolor_sieci = koloryps.W;
                            else kolor_sieci = koloryps.inne;

                          siec_xyz_type_no[siec_no].color = kolor_sieci;
                    }

                    siec_xyz_type_no[siec_no].geodetic=0;
                    //coordinates
                    if ((ret_par>1) &&  ((strstr(coordinates,u8"coordinates")==coordinates) ||  //starts with name of coordinates
                                         (strstr(coordinates,u8"współrzędne")==coordinates) ||
                                         (strstr(coordinates,u8"координати")==coordinates) ||
                                         (strstr(coordinates,u8"coordenadas")==coordinates)))
                    {
                        char *coordinates_symbol = strchr(coordinates, ':');
                        if (coordinates_symbol != NULL)
                        {
                            coordinates_symbol++;
                            if ((strstr(coordinates_symbol,u8"geodetic")==coordinates_symbol) ||
                                    (strstr(coordinates_symbol,u8"geodezyjne")==coordinates_symbol) ||
                                    (strstr(coordinates_symbol,u8"геодезичний")==coordinates_symbol) ||
                                    (strstr(coordinates_symbol,u8"geodésico")==coordinates_symbol))
                                siec_xyz_type_no[siec_no].geodetic=1;
                        }
                    }

                    siec_xyz_type_no[siec_no].scale_xy=SkalaF;

                    //scale
                    if ((ret_par>2) &&  ((strstr(scale,u8"scale_1")==scale) ||  //starts with name of scale
                                         (strstr(scale,u8"skala_1")==scale) ||
                                         (strstr(scale,u8"масштаб_1")==scale) ||
                                         (strstr(scale,u8"escala_1")==scale)))
                    {
                        char *scale_data = strchr(scale, ':');
                        double scale_xy;
                        if (scale_data != NULL)
                        {
                            scale_data++;
                            decimal(scale_data);
                            int ret_origin_data = sscanf(scale_data, "%lf", &scale_xy);
                            siec_xyz_type_no[siec_no].scale_xy=scale_xy;
                        }
                    }

                    siec_xyz_type_no[siec_no].origin_x=0;
                    siec_xyz_type_no[siec_no].origin_y=0;

                    //point origin
                    if ((ret_par>3) &&  ((strstr(point_origin,u8"point_origin")==point_origin) ||  //starts with name of point_origin
                                         (strstr(point_origin,u8"punkt_początkowy")==point_origin) ||
                                         (strstr(point_origin,u8"точка_початку_координат")==point_origin) ||
                                         (strstr(point_origin,u8"punto_de_origen")==point_origin)))
                    {
                        char *origin_data = strchr(point_origin, ':');
                        double origin_x, origin_y;
                        if (origin_data != NULL)
                        {
                            origin_data++;
                            int ret_origin_data = sscanf(decimal_dot(origin_data), "%lf;%lf", &origin_x, &origin_y);
                            siec_xyz_type_no[siec_no].origin_x=jednostkiOb(origin_x);
                            siec_xyz_type_no[siec_no].origin_y=jednostkiOb(origin_y);
                        }
                    }

                    siec_xyz_type_no[siec_no].angle_l=0.;
                    //angle
                    if ((ret_par>4) &&  ((strstr(angle,u8"angle")==angle) ||  //starts with name of angle
                                         (strstr(angle,u8"kąt")==angle) ||
                                         (strstr(angle,u8"кут")==angle) ||
                                         (strstr(angle,u8"ángulo")==angle)))
                    {
                        char *angle_data = strchr(angle, ':');
                        if (angle_data != NULL)
                        {
                            angle_data++;
                            int ret_origin_data = sscanf(decimal_dot(angle_data), "%lf", &siec_xyz_type_no[siec_no].angle_l);
                        }
                    }
                }
                else if (((strstr(buf,"Number")==buf) ||  //starts with name
                             (strstr(buf,"Numer")==buf) ||
                             (strstr(buf,u8"Номер")==buf) ||
                             (strstr(buf,u8"Número")==buf) ||
                             (strstr(buf,"Point")==buf) ||
                             (strstr(buf,"Punkt")==buf) ||
                             (strstr(buf,u8"Точка")==buf) ||
                             (strstr(buf,"Punto")==buf))
                            &&
                            ((strstr(buf,"X")!=NULL) &&   //somewhere in the row
                            (strstr(buf,"Y")!=NULL) ))
                {
                    ;  //just a header
                }
                else if (strncmp(buf,"L=",2)==0) //sum of lengths, ignored
                {
                    ;
                }
                else if (siec_no>=0)
                {
                    ret_par = sscanf(buf, "%s %s %s %s %s", xyz_nazwa, xyz_x, xyz_y, xyz_z, len);
                    
                    if (siec_xyz_type_no[siec_no].no<max_points)  //this is very high limitation for safety reason
                    {
                        if (siec_xyz_type_no[siec_no].no==SIEC_XYZ_NO_MAX)  //reached the limit
                        {
                            siec_xyz_max_no[siec_no]+=SIEC_XYZ_NO_MAX; //adding more

                            Siec_XYZ *xyz_temp=realloc(siec_xyz[siec_no], sizeof(Siec_XYZ)*(siec_xyz_max_no[siec_no]));  //in advance extra 100 points

                            if (xyz_temp!=NULL)
                                siec_xyz[siec_no]=xyz_temp;

                            char *name_temp=realloc(siec_xyz_name[siec_no], sizeof(char*)*(siec_xyz_max_no[siec_no])); //in advance extra 100 names

                            if (name_temp!=NULL)
                            siec_xyz_name[siec_no]=name_temp;
                        }
                        strncpy(siec_xyz[siec_no][siec_xyz_type_no[siec_no].no].nazwa, xyz_nazwa, 16);

                        local_z=0.;
                        if ((ret_par==3) || (ret_par==4))  //could be missing l and Z or Z
                        {
                            /*
                            if (!geodetic) {
                                local_x = strtod(decimal_dot(xyz_x), &bp); //-origin.x;
                                local_y = strtod(decimal_dot(xyz_y), &bp); //-origin.y;
                            }
                            else
                            {   //reversed
                                local_y = strtod(decimal_dot(xyz_x), &bp); //-origin.x;
                                local_x = strtod(decimal_dot(xyz_y), &bp); //-origin.y;
                            }
                             */

                            local_x = strtod(decimal_dot(xyz_x), &bp); //-origin.x;
                            local_y = strtod(decimal_dot(xyz_y), &bp); //-origin.y;

                            siec_xyz[siec_no][siec_xyz_type_no[siec_no].no].x = local_x;
                            siec_xyz[siec_no][siec_xyz_type_no[siec_no].no].y = local_y;
                            siec_xyz[siec_no][siec_xyz_type_no[siec_no].no].z = local_z;
                            siec_xyz_type_no[siec_no].no++;
                        }
                        else if (ret_par>=5) //including Z or even more
                        {
                            /*
                            if (!grodetic) {
                                local_x = strtod(decimal_dot(xyz_x), &bp)-origin.x;
                                local_y = strtod(decimal_dot(xyz_y), &bp)-origin.y;
                            }
                            else
                            {
                                local_y = strtod(decimal_dot(xyz_x), &bp)-origin.y;
                                local_x = strtod(decimal_dot(xyz_y), &bp)-origin.x;
                            }
                             */

                            local_x = strtod(decimal_dot(xyz_x), &bp); //-origin.x;
                            local_y = strtod(decimal_dot(xyz_y), &bp); //-origin.y;
                            local_z=strtod(decimal_dot(xyz_z), &bp);

                            siec_xyz[siec_no][siec_xyz_type_no[siec_no].no].x = local_x;
                            siec_xyz[siec_no][siec_xyz_type_no[siec_no].no].y = local_y;
                            siec_xyz[siec_no][siec_xyz_type_no[siec_no].no].z = local_z;
                            siec_xyz_type_no[siec_no].no++;
                        }
                        else //could be incomplete or empty row
                        {
                            ;
                        }
                    }
                }
            }
        }
        k=fclose(stru);

        read_txt=TRUE;
    }

    else {
        strcpy(blok_type0, "*#");
        switch (siec) {
            case -1:
                kolor_sieci = koloryps.inne;
                strcpy(blok_type, "");
                break;
            case 0:
                kolor_sieci = koloryps.Pz;
                strcpy(blok_type, "Pz");
                break;
            case 1:
                kolor_sieci = koloryps.Cok;
                strcpy(blok_type, "Cok");
                break;
            case 2:
                kolor_sieci = koloryps.Co;
                strcpy(blok_type, "Co");
                break;
            case 3:
                kolor_sieci = koloryps.T;
                strcpy(blok_type, "T");
                break;
            case 4:
                kolor_sieci = koloryps.Tk;
                strcpy(blok_type, "Tk");
                break;
            case 5:
                kolor_sieci = koloryps.e;
                strcpy(blok_type, "e");
                break;
            case 6:
                kolor_sieci = koloryps.E;
                strcpy(blok_type, "E");
                break;
            case 7:
                kolor_sieci = koloryps.S;
                strcpy(blok_type, "S");
                break;
            case 8:
                kolor_sieci = koloryps.D;
                strcpy(blok_type, "D");
                break;
            case 9:
                kolor_sieci = koloryps.W;
                strcpy(blok_type, "W");
                break;
            default:
                kolor_sieci = koloryps.inne;
                strcpy(blok_type, "");
                break;
        }

        strcat(blok_type0, blok_type);
    }

    CUR_OFF(X,Y);
    sel_nr=sel.nr;
    sel_gor=sel.gor;
    sel.nr=8;  //punkt
    // sel.gor=1;
    CUR_ON(X,Y);
    // save_params(0);
    memmove(&LiniaG0, &LiniaG, sizeof(LINIA));
    memmove(&LukG0, &LukG, sizeof(LUK));

    LiniaG.warstwa=Current_Layer;
    LiniaG.kolor=kolor_sieci;
    LiniaG.typ=128;
    LiniaG.obiektt2=O2BlockAparat;  //na wydruku i przy kresleniu dla profili
    //wszystkie linie najgrubsze O2BlockAparat
    //sa rysowane jako linie sienki czarne oraz
    //linie wlasciwe
    LukG.warstwa=Current_Layer;
    LukG.kolor=kolor_sieci;
    LukG.typ=128;
    LukG.obiektt2=O2BlockAparat;
    SIEC_P=TRUE;
    //dodatkowe menu
    dane_size0000=dane_size;
    if (read_txt==FALSE)
    {
        blk_address=Pline_single(&x_begin, &y_begin);
        if (blk_address!=NULL) go_next=1;
    }
    else
    {
        for (int ni=0; ni<=siec_no; ni++)
        {

            LiniaG.kolor=LukG.kolor=siec_xyz_type_no[ni].color;

            //setting local coordinates system
            options1.uklad_geodezyjny=siec_xyz_type_no[ni].geodetic;
            options1.uklad_izometryczny=0;

            SkalaF=siec_xyz_type_no[ni].scale_xy;

            if (siec_xyz_type_no[ni].geodetic) {
                put_localx(-siec_xyz_type_no[ni].origin_y);
                put_localy(-siec_xyz_type_no[ni].origin_x);
            }
            else
            {
                put_localx(-siec_xyz_type_no[ni].origin_x);
                put_localy(-siec_xyz_type_no[ni].origin_y);
            }
            put_angle_l(siec_xyz_type_no[ni].angle_l);

            if (siec_xyz_type_no[ni].no > 1) go_next = Pline_single_txt(&x_begin, &y_begin, siec_xyz[ni], siec_xyz_type_no[ni].no);

            parent_block=(BLOK*)dane;

            adp1 = adp1b = (char *) parent_block + sizeof(NAGLOWEK) + B3 + parent_block->dlugosc_opisu_obiektu;
            adp2 = (char *) parent_block + sizeof(NAGLOWEK) + parent_block->n - 1;

            BLOK *net_polyline = (BLOK *) dane;
            if (net_polyline->kod_obiektu != B_PLINE) {
                goto error_go;
            }
            dane_size000 = (long) (sizeof(NAGLOWEK) + net_polyline->n);

            group_block_with_type(dane_size000, 0, 0, siec_xyz_type_no[ni].type, B_SIEC);

            parent_block = (BLOK *) dane;

        }
    }

    SIEC_P=FALSE;

    //this will be done later, after all
    //memmove(&LiniaG, &LiniaG0, sizeof(LINIA));
    //memmove(&LukG, &LukG0, sizeof(LUK));

    if (go_next==TRUE)
    {
        if (read_txt==FALSE)
        {
            //checking if existing network
            BOOL existing_block = FALSE;
            parent_block = FIRSTB(blk_address);
            if (parent_block != NULL)
            {
                char parent_kod_obiektu = parent_block->kod_obiektu;
                int dlugosc_opisu_obiektu = parent_block->dlugosc_opisu_obiektu;
                T_Desc_Ex_Block *ptrs_desc_bl = (T_Desc_Ex_Block *) (&parent_block->opis_obiektu[0]);
                char *parent_blok_type = (&ptrs_desc_bl->sz_type[0]);
                if ((dlugosc_opisu_obiektu > 1) && (parent_kod_obiektu == B_SIEC) && (parent_blok_type[0] = '*') &&
                    (parent_blok_type[1] = '#')) {
                    existing_block = TRUE;
                    strcpy(st_dest, &parent_blok_type[2]);
                    ////taking layer, line type and color
                    nag = (NAGLOWEK *) (blk_address + sizeof(NAGLOWEK) + ((BLOK *) blk_address)->n);
                    if (nag->obiekt != OdBLOK) {
                        LiniaG.warstwa = LukG.warstwa = ((LINIA *) nag)->warstwa;
                        LiniaG.kolor = LukG.kolor = kolor_sieci = ((LINIA *) nag)->kolor;
                        LiniaG.typ = LukG.typ = ((LINIA *) nag)->typ;
                    }
                }
            } else
            {
                //if no parent block, means thet it is newly created network and its block was created in dane address

                BLOK *net_polyline = (BLOK *) dane;
                if (net_polyline->kod_obiektu != B_PLINE) {
                    goto error_go;
                }
                dane_size000 = (long) (sizeof(NAGLOWEK) + net_polyline->n);

                group_block_with_type(dane_size000, 0, 0, blok_type0, B_SIEC);

                parent_block = (BLOK *) dane;

            }

            if (existing_block == FALSE)
                strcpy(st_dest, blok_type);

            //dopisanie opisu sieci w wezlach
            strcat(st_dest, "1");
            if (read_esc(st_dest, 60, 153) == 0)
            {
                ;
            }
            //////////////////
            //strcpy(change_text_dest,st_dest);
            /*rozbior st_dest ma symbol o numer*/
            /*poszukiwanie ostatniej cyfry*/
            end_char = (int) strlen(st_dest) - 1;

            while ((end_char >= 0) && (st_dest[end_char] > '\057') && (st_dest[end_char] < '\072'))
            {
                end_char--;
            }

            if (end_char == -1)
            {
                strcpy(st_dest_symbol, "");
                st_dest_numer_i = (int)strtol(st_dest, &bp, 10); //atoi(st_dest);
            } else
            {
                strcpy(st_dest_symbol, st_dest);
                st_dest_symbol[end_char + 1] = '\0';

                strcpy(st_dest_numer, st_dest + end_char + 1);
                st_dest_numer_i = (int)strtol(st_dest_numer, &bp, 10); // atoi(st_dest_numer);
            }

            //////////////////
            //generowanie petli opisow w kazdym wezle
            //b=(BLOK *)dane;
            //adp1=dane+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
            //adp2=dane + sizeof(NAGLOWEK) + b->n - 1;


            //paren_block can be virgin pline, or already created net
            //in case of newly created net all follows the plan
            //in case of existing net, first nodes description have to be found,
            //then removed and replaces with complete ones.
            //editing existing pline / net lets remove previously created branches,
            //so the numbering should be done from scratch after removing previous descriptions

            adp1 = adp1b = (char *) parent_block + sizeof(NAGLOWEK) + B3 + parent_block->dlugosc_opisu_obiektu;
            adp2 = (char *) parent_block + sizeof(NAGLOWEK) + parent_block->n - 1;
            if (existing_block == TRUE) {
                PTR__GTMPFIRST = (char *) parent_block;
                //removing description blocks
                //searching for entire block with  blok_type "*~",  kod_bloku B_VERTEX
                while (adp1b < adp2) {
                    adp3 = find_block(adp1b, adp2, B_VERTEX, "*~");
                    if (adp3 != NULL) {
                        b = (BLOK *) adp3;
                        adp4 = (char *) b + sizeof(NAGLOWEK) + b->n - 1;
                        zmien_atrybut(adp3, adp4, ANieOkreslony, Ausuniety);

                        adp1b = adp3 + (sizeof(NAGLOWEK) + b->n);
                    } else break;
                }
                usun_blok(adp1, adp2);

                redraw();

                //new address of the block and range
                parent_block = (BLOK *) PTR__GTMPFIRST;
                adp1 = (char *) parent_block + sizeof(NAGLOWEK) + B3 + parent_block->dlugosc_opisu_obiektu;
                adp2 = (char *) parent_block + sizeof(NAGLOWEK) + parent_block->n - 1;
            }
        }
        else {
            strcpy(st_dest, "");
            strcpy(st_dest_symbol, "");
            strcpy(st_dest_numer, "1");
            st_dest_numer_i = 1;
        }

        find_next = TRUE;
        first_line = TRUE;
        //definicja kola
        K.warstwa = LiniaG.warstwa; //Current_Layer;
        K.blok = ElemBlok;
        K.r = (float) srednica_punktu_zalamania; //0.7f;
        K.kolor = kolor_sieci;
        K.obiektt2 = O2BlockAparat;

        P.warstwa = LiniaG.warstwa; //Current_Layer;
        P.blok = ElemBlok;
        P.typ = 0;
        P.kolor = kolor_sieci;
        P.obiektt2 = O2BlockAparat;

        L1.warstwa = LiniaG.warstwa; //Current_Layer;
        L1.blok = ElemBlok;
        L1.kolor = kolor_sieci;
        L1.typ = 32;
        L1.obiektt2 = O2BlockAparat;

        T.warstwa = LiniaG.warstwa; //Current_Layer;
        T.blok = ElemBlok;
        T.kolor = kolor_sieci;
        T.wysokosc = 3.5f;
        T.czcionka = zmwym.czcionka;
        T.italics = 1;
        T.width_factor = 0.85f;
        T.bold = 1;
        T.obiektt2 = O2BlockAparat;

        //n_points = 0;

        if (read_txt == FALSE) siec_no=0;

        for (int nwi = 0; nwi <= siec_no; nwi++)  //up to first sieci_no networks, connection ot net
        {
            n_points = 0;

            if (read_txt == TRUE)
            {
                //searching for nwi addresses
                int nbi;
                adp1=dane;
                adp2=dane+dane_size;
                for (nbi = 0; nbi<=nwi; nbi++)
                {
                    BLOK *parent_block_net=(BLOK*)find_block_sub_on(adp1, adp2, B_SIEC, "*#", 2);
                    BLOK *parent_block_con=(BLOK*)find_block_sub_on(adp1, adp2, B_SIEC, "*^", 2);

                    if (parent_block_con==NULL)
                        parent_block = parent_block_net;
                    else if (parent_block_net==NULL)
                        parent_block = parent_block_con;
                    else if ((parent_block_net==NULL) && (parent_block_con==NULL))
                        parent_block=NULL;
                    else {
                        if (parent_block_net < parent_block_con)
                            parent_block = parent_block_net;
                        else parent_block = parent_block_con;
                    }

                    if (parent_block!=NULL)
                    {
                        adp1+=sizeof(NAGLOWEK)+parent_block->n;
                    };
                }
                if (parent_block!=NULL)
                {
                    adp1 = adp1b = (char *) parent_block + sizeof(NAGLOWEK) + B3 + parent_block->dlugosc_opisu_obiektu;
                    adp2 = (char *) parent_block + sizeof(NAGLOWEK) + parent_block->n - 1;

                    siec_xyz0=siec_xyz[siec_no-nwi];  //reversed order
                }
                else
                {
                    adp1=adp2=NULL;
                }
            }
            while ((adp1 < adp2) && (find_next == TRUE)) {
                adp3 = find_obj2(adp1, adp2, Olinia, Oluk, 128, 128, 0);
                if (adp3 != NULL) {
                    nag = (NAGLOWEK *) adp3;
                    if (nag->obiekt == Olinia)  //linia
                    {
                        L = (LINIA *) adp3;
                        //mamy wiec dwa punkty
                        if (first_line == TRUE) {
                            //punkt 1
                            if (n_points < max_points) {
                                n_points++;
                                point_xy[n_points - 1].x = L->x1;
                                point_xy[n_points - 1].y = L->y1;
                                if (L->x1 < L->x2) {
                                    if (L->y1 < L->y2) point_xy[n_points - 1].kierunek = 1;
                                    else point_xy[n_points - 1].kierunek = 0;
                                } else {
                                    if (L->y1 < L->y2) point_xy[n_points - 1].kierunek = 0;
                                    else point_xy[n_points - 1].kierunek = 1;
                                }
                            }
                            //punkt 2
                            if (n_points < max_points) {
                                n_points++;
                                point_xy[n_points - 1].x = L->x2;
                                point_xy[n_points - 1].y = L->y2;
                            }
                            first_line = FALSE;
                            last_x = L->x2;
                            last_y = L->y2;
                        } else {
                            //punkt 1
                            if ((Check_if_Equal(L->x1, last_x) == FALSE) &&
                                (Check_if_Equal(L->y1, last_y) == FALSE)) {   //nowy punkt
                                if (n_points < max_points) {
                                    n_points++;
                                    point_xy[n_points - 1].x = L->x1;
                                    point_xy[n_points - 1].y = L->y1;
                                    if (L->x1 < L->x2) {
                                        if (L->y1 < L->y2) point_xy[n_points - 1].kierunek = 1;
                                        else point_xy[n_points - 1].kierunek = 0;
                                    } else {
                                        if (L->y1 < L->y2) point_xy[n_points - 1].kierunek = 0;
                                        else point_xy[n_points - 1].kierunek = 1;
                                    }
                                }
                            } else //stary punkt
                            if (L->x1 < L->x2) {
                                if (L->y1 < L->y2) point_xy[n_points - 1].kierunek = 1;
                                else point_xy[n_points - 1].kierunek = 0;
                            } else {
                                if (L->y1 < L->y2) point_xy[n_points - 1].kierunek = 0;
                                else point_xy[n_points - 1].kierunek = 1;
                            }
                            //punkt2
                            if (n_points < max_points) {
                                n_points++;
                                point_xy[n_points - 1].x = L->x2;
                                point_xy[n_points - 1].y = L->y2;
                            }
                            first_line = FALSE;
                            last_x = L->x2;
                            last_y = L->y2;
                        }
                        adp1 = adp3 + sizeof(NAGLOWEK) + L->n;
                    } else //luk
                    {
                        l = (LUK *) adp3;
                        l_x1 = l->x + l->r * cosf(l->kat1);
                        l_y1 = l->y + l->r * sinf(l->kat1);
                        l_x2 = l->x + l->r * cosf(l->kat2);
                        l_y2 = l->y + l->r * sinf(l->kat2);

                        if ((Check_if_Equal(l_x1, l_x2) == TRUE) && (Check_if_Equal(l_y1, l_y2) == TRUE)) {
                            //zignorowano luk
                            ;
                        } else {
                            //mamy wiec dwa punkty
                            if (first_line == TRUE) {
                                //punkt 1
                                if (n_points < max_points) {
                                    n_points++;
                                    point_xy[n_points - 1].x = l_x1;
                                    point_xy[n_points - 1].y = l_y1;
                                    if (l_x1 < l_x2) {
                                        if (l_y1 < l_y2) point_xy[n_points - 1].kierunek = 1;
                                        else point_xy[n_points - 1].kierunek = 0;
                                    } else {
                                        if (l_y1 < l_y2) point_xy[n_points - 1].kierunek = 0;
                                        else point_xy[n_points - 1].kierunek = 1;
                                    }
                                }
                                //punkt 2
                                if (n_points < max_points) {
                                    n_points++;
                                    point_xy[n_points - 1].x = l_x2;
                                    point_xy[n_points - 1].y = l_y2;
                                }
                                first_line = FALSE;
                                last_x = l_x2;
                                last_y = l_y2;
                            } else {
                                //ewentualne obrocenie luku
                                if ((Check_if_Equal(l_x2, last_x) == TRUE) && (Check_if_Equal(l_y2, last_y) == TRUE)) {
                                    l_x0 = l_x1;
                                    l_y0 = l_y1;
                                    l_x1 = l_x2;
                                    l_y1 = l_y2;
                                    l_x2 = l_x0;
                                    l_y2 = l_y0;
                                }
                                //punkt 1
                                if ((Check_if_Equal(l_x1, last_x) == FALSE) &&
                                    (Check_if_Equal(l_y1, last_y) == FALSE)) {   //nowy punkt
                                    if (n_points < max_points) {
                                        n_points++;
                                        point_xy[n_points - 1].x = l_x1;
                                        point_xy[n_points - 1].y = l_y1;
                                        if (l_x1 < l_x2) {
                                            if (l_y1 < l_y2) point_xy[n_points - 1].kierunek = 1;
                                            else point_xy[n_points - 1].kierunek = 0;
                                        } else {
                                            if (l_y1 < l_y2) point_xy[n_points - 1].kierunek = 0;
                                            else point_xy[n_points - 1].kierunek = 1;
                                        }
                                    }
                                } else //stary punkt
                                if (l_x1 < l_x2) {
                                    if (l_y1 < l_y2) point_xy[n_points - 1].kierunek = 1;
                                    else point_xy[n_points - 1].kierunek = 0;
                                } else {
                                    if (l_y1 < l_y2) point_xy[n_points - 1].kierunek = 0;
                                    else point_xy[n_points - 1].kierunek = 1;
                                }
                                //punkt2
                                if (n_points < max_points) {
                                    n_points++;
                                    point_xy[n_points - 1].x = l_x2;
                                    point_xy[n_points - 1].y = l_y2;
                                }
                                first_line = FALSE;
                                last_x = l_x2;
                                last_y = l_y2;
                            }
                        }
                        adp1 = adp3 + sizeof(NAGLOWEK) + l->n;
                    }
                } else find_next = FALSE;
            }

            PTR__GTMPFIRST = (char *) parent_block;  //setting global movable pointer

            if (n_points > 0)
            {
                if (nag->obiekt == Olinia)  //linia
                {
                    //ostatni punkt
                    if (L->x1 < L->x2) {
                        if (L->y1 < L->y2) point_xy[n_points - 1].kierunek = 1;
                        else point_xy[n_points - 1].kierunek = 0;
                    } else {
                        if (L->y1 < L->y2) point_xy[n_points - 1].kierunek = 0;
                        else point_xy[n_points - 1].kierunek = 1;
                    }
                } else //luk
                {
                    if (l_x1 < l_x2) {
                        if (l_y1 < l_y2) point_xy[n_points - 1].kierunek = 1;
                        else point_xy[n_points - 1].kierunek = 0;
                    } else {
                        if (l_y1 < l_y2) point_xy[n_points - 1].kierunek = 0;
                        else point_xy[n_points - 1].kierunek = 1;
                    }
                }

                //opis odleglosci
                T.x = (float) (point_xy[0].x + 2.);
                T.y = (float) (point_xy[0].y + 2.);
                sprintf(st_dest, "0.0");
                strcpy(&T.text[0], st_dest);
                LengthT = (int) strlen(T.text);
                T.dl = LengthT;
                T.n = T18 + T.dl;
                T.ukryty = 1;
                T.typ = n_typ_atrybut;
                if (dodaj_obiekt((BLOK *) PTR__GTMPFIRST, &T) == NULL) goto error_go;
                rysuj_obiekt((char *) &T, COPY_PUT, 1);
                T.ukryty = 0;
                T.typ = n_typ_normal;

                for (i = 0; i < n_points; i++)
                {
                    //punkt
                    PTR__GTMPBLOCK = NULL;
                    //if (FALSE == add_block_with_type (point_xy[i].x, point_xy[i].y, "*~", 0, B_VERTEX))
                    if (FALSE == add_block_in_block_with_type((BLOK *) PTR__GTMPFIRST, point_xy[i].x, point_xy[i].y, "*~", 0,B_VERTEX))
                    {
                        goto error_go;
                    }
                    P.x = (float) point_xy[i].x;
                    P.y = (float) point_xy[i].y;
                    if (dodaj_obiekt_((BLOK *) dane, &P) == NULL) goto error_go;
                    rysuj_obiekt((char *) &P, COPY_PUT, 1);

                    K.x = (float) point_xy[i].x;
                    K.y = (float) point_xy[i].y;
                    if (dodaj_obiekt_((BLOK *) dane, &K) == NULL) goto error_go;
                    rysuj_obiekt((char *) &K, COPY_PUT, 1);

                    //linie
                    if (point_xy[i].kierunek == 0)  //  /
                    {
                        L1.x1 = (float) point_xy[i].x;
                        L1.y1 = (float) point_xy[i].y;
                        L1.x2 = (float) (point_xy[i].x + 5.);
                        L1.y2 = (float) (point_xy[i].y + 10.);
                        if (dodaj_obiekt_((BLOK *) dane, &L1) == NULL) goto error_go;
                        rysuj_obiekt((char *) &L1, COPY_PUT, 1);

                        //opis
                        T.x = 0;  //temporary
                        T.y = 0;
                        if (read_txt == FALSE) sprintf(st_dest, "%s%d", st_dest_symbol, st_dest_numer_i + i);
                        else
                        {
                            strcpy(st_dest, siec_xyz0[i].nazwa);
                            if (strncpy(siec_xyz_type_no[siec_no-nwi].type, "*^", 2)==0) //reversed order
                                T.ukryty = 1;
                        }
                        strcpy(&T.text[0], st_dest);
                        LengthT = (int) strlen(T.text);
                        T.dl = LengthT;
                        T.n = T18 + T.dl;

                        double t_len_mm = Get_Text_Len(&T, T.text, 1.0, 0, &t_width, &t_height, NULL, NULL);
                        L1.x1 = (float) (point_xy[i].x + 5.);
                        L1.y1 = (float) (point_xy[i].y + 10.);
                        L1.x2 = (float) (point_xy[i].x + 5. + t_len_mm);  //15.
                        L1.y2 = (float) (point_xy[i].y + 10.);
                        if (dodaj_obiekt_((BLOK *) dane, &L1) == NULL) goto error_go;
                        rysuj_obiekt((char *) &L1, COPY_PUT, 1);

                        T.ukryty = 0;
                        //opis add
                        T.x = (float) (L1.x1 + 1.);
                        T.y = (float) (L1.y1 + 0.5);
                        if (dodaj_obiekt_((BLOK *) dane, &T) == NULL) goto error_go;
                        rysuj_obiekt((char *) &T, COPY_PUT, 1);
                        //rzedna
                        T.typ = n_rzedna_kanalu_p;
                        T.wysokosc = 2.0f;
                        T.x = (float) (L1.x1 + 1.);
                        T.y = (float) (L1.y1 - 2.25);
                        T.ukryty = 1;

                        if (read_txt == TRUE)
                        {
                            if (siec_xyz0[i].z>0.)
                                set_decimal_format(st_dest, siec_xyz0[i].z, tab_dokladnosc[4]);
                            else strcpy(st_dest, "");
                        }
                        else strcpy(st_dest, "");
                        strcpy(&T.text[0], st_dest);
                        LengthT = (int) strlen(T.text);
                        T.dl = LengthT;
                        T.n = T18 + T.dl;
                        if (dodaj_obiekt_((BLOK *) dane, &T) == NULL) goto error_go;
                        rysuj_obiekt((char *) &T, COPY_PUT, 1);
                        //srednica
                        T.typ = n_srednica_kanalu_p;
                        T.x = (float) (L1.x1 + 1.);
                        T.y = (float) (L1.y1 - 4.5);
                        strcpy(st_dest, "");
                        strcpy(&T.text[0], st_dest);
                        LengthT = (int) strlen(T.text);
                        T.dl = LengthT;
                        T.n = T18 + T.dl;
                        if (dodaj_obiekt_((BLOK *) dane, &T) == NULL) goto error_go;
                        rysuj_obiekt((char *) &T, COPY_PUT, 1);

                        T.wysokosc = 3.5f;
                        T.ukryty = 0;
                        T.typ = n_typ_normal;
                    } else {
                        L1.x1 = (float) point_xy[i].x;
                        L1.y1 = (float) point_xy[i].y;
                        L1.x2 = (float) (point_xy[i].x + 5.);
                        L1.y2 = (float) (point_xy[i].y - 10.);
                        if (dodaj_obiekt_((BLOK *) dane, &L1) == NULL) goto error_go;
                        rysuj_obiekt((char *) &L1, COPY_PUT, 1);

                        //opis
                        T.x = 0;  //temporary
                        T.y = 0;
                        if (read_txt == FALSE) sprintf(st_dest, "%s%d", st_dest_symbol, st_dest_numer_i + i);
                        else strcpy(st_dest, siec_xyz0[i].nazwa);
                        strcpy(&T.text[0], st_dest);
                        LengthT = (int) strlen(T.text);
                        T.dl = LengthT;
                        T.n = T18 + T.dl;

                        double t_len_mm = Get_Text_Len(&T, T.text, 1.0, 0, &t_width, &t_height, NULL, NULL);
                        L1.x1 = (float) (point_xy[i].x + 5.);
                        L1.y1 = (float) (point_xy[i].y - 10.);
                        L1.x2 = (float) (point_xy[i].x + 5. + t_len_mm);  //15.
                        L1.y2 = (float) (point_xy[i].y - 10.);
                        if (dodaj_obiekt_((BLOK *) dane, &L1) == NULL) goto error_go;
                        rysuj_obiekt((char *) &L1, COPY_PUT, 1);
                        //opis add
                        T.x = (float) (L1.x1 + 1.);
                        T.y = (float) (L1.y1 + 0.5);
                        if (dodaj_obiekt_((BLOK *) dane, &T) == NULL) goto error_go;
                        rysuj_obiekt((char *) &T, COPY_PUT, 1);
                        //rzedna
                        T.typ = n_rzedna_kanalu_p;
                        T.wysokosc = 2.0f;
                        T.x = (float) (L1.x1 + 1.);
                        T.y = (float) (L1.y1 - 2.25);
                        T.ukryty = 1;
                        strcpy(st_dest, "");
                        strcpy(&T.text[0], st_dest);
                        LengthT = (int) strlen(T.text);
                        T.dl = LengthT;
                        T.n = T18 + T.dl;
                        if (dodaj_obiekt_((BLOK *) dane, &T) == NULL) goto error_go;
                        rysuj_obiekt((char *) &T, COPY_PUT, 1);
                        T.typ = n_typ_normal;
                        //srednica
                        T.typ = n_srednica_kanalu_p;
                        T.x = (float) (L1.x1 + 1.);
                        T.y = (float) (L1.y1 - 4.5);
                        strcpy(st_dest, "");
                        strcpy(&T.text[0], st_dest);
                        LengthT = (int) strlen(T.text);
                        T.dl = LengthT;
                        T.n = T18 + T.dl;
                        if (dodaj_obiekt_((BLOK *) dane, &T) == NULL) goto error_go;
                        rysuj_obiekt((char *) &T, COPY_PUT, 1);

                        T.typ = n_typ_normal;
                        T.wysokosc = 3.5f;
                        T.ukryty = 0;
                    }

                    //rotating PTR__GTMPFIRST block if needed
                    if ((PTR__GTMPBLOCK!=NULL) && (Check_if_Equal(reference_angle, 0.0)==FALSE))
                    {
                        char *ADP1 = PTR__GTMPBLOCK;
                        char *ADK1 = ADP1 + sizeof(NAGLOWEK) + ((BLOK *) PTR__GTMPBLOCK)->n - 1;
                        double rotation_angle = reference_angle * Pi / 180;
                        double kos = sin(rotation_angle);
                        double koc = cos(rotation_angle);
                        zmien_atrybut(ADP1,ADK1,ANieOkreslony, Ablok);
                        transformacja_blok (ADP1, ADK1, point_xy[i].x, point_xy[i].y, kos, koc, Tobrot, 0);
                        zmien_atrybut(ADP1,ADK1,Ablok, Anormalny);
                    }
                }
            }
        }
    }

    ret_bak=1;
goto ok_go;
error_go:
    ret_bak = 0;
ok_go:
    //freeing memory
    if (read_txt==TRUE)
    {
        free(siec_xyz_type_no);
        free(siec_xyz_max_no);
        // --- CLEAN UP: Always free in the exact reverse order of allocation ---
        for (i = 0; i < SIEC_NO_MAX; i++) {
            if (siec_xyz[i] != NULL) {
                free(siec_xyz[i]); // Free columns
                siec_xyz[i] = NULL;
            }
        }
        free(siec_xyz); // Free the master pointer array
    }

    PTR__GTMPBLOCK=NULL;

    //after all
    memmove(&LiniaG, &LiniaG0, sizeof(LINIA));
    memmove(&LukG, &LukG0, sizeof(LUK));

    color_bar();

    CUR_OFF(X,Y);
    sel.nr=sel_nr;
    sel.gor=sel_gor;
    CUR_ON(X,Y);
    //if (read_txt==TRUE)
        redraw();
    return ret_bak;
}


int select_polyline_net(void)
{   T_Desc_Ex_Block *ptrs_desc_bl;
    char st[30];
    double X0, Y0;

    global_blk=select_polyline();
    if (global_blk!=NULL)
    {//searching for existing nodes to find point_net_typ
        BOOL ret=Set_Beginning_Pline(global_blk, &X0, &Y0);

        BOOL existing_block=FALSE;
        BLOK *parent_block= FIRSTB((char*)global_blk);
        if (parent_block!=NULL)
        {
            if (parent_block->dlugosc_opisu_obiektu>1)
            {
                ptrs_desc_bl = (T_Desc_Ex_Block *)(&parent_block->opis_obiektu[0]) ;
                strncpy (st, &ptrs_desc_bl->sz_type [0], 30) ;
                if (strncmp(st,"*^",2)==0) //znaleziono blok connection
                {
                    //to jest blok sieci
                    //przyporzadkowanie typu sieci
                    ///////////////////
                    if (strcmp(st,"*^Pz")==0) point_net_typ=0;
                    else if (strcmp(st,"*^Cok")==0) point_net_typ=1;
                    else if (strcmp(st,"*^Co")==0) point_net_typ=2;
                    else if (strcmp(st,"*^T")==0) point_net_typ=3;
                    else if (strcmp(st,"*^Tk")==0) point_net_typ=4;
                    else if (strcmp(st,"*^e")==0) point_net_typ=5;
                    else if (strcmp(st,"*^E")==0) point_net_typ=6;
                    else if (strcmp(st,"*^S")==0) point_net_typ=7;
                    else if (strcmp(st,"*^D")==0) point_net_typ=8;
                    else if (strcmp(st,"*^W")==0) point_net_typ=9;
                    else point_net_typ=100;
                }
            }
        }
        return 1;
    }
    return 0;
}

void Rysuj_przylacze(void)
/*----------------------*/
{
    int sel_nr;
    int sel_gor;
    int kolor_sieci;
    long dane_size000,dane_size0000;
    char blok_type[30],blok_type0[30];
    char st_dest[MaxTextLen];
    double x_begin,y_begin;
    BLOK *b;
    char *adp1,*adp2,*adp3, *adp1b, *adp4;
    BOOL first_line,find_next;
    double last_x, last_y;
    OKRAG K=Kdef;
    T_Point P=PointDef;
    NAGLOWEK *nag;
    LINIA *L,L1=Ldef;
    LUK *l;
    net_point point_xy[max_points+1];
    int n_points, i;
    TEXT T=Tdef;
    double l_x1, l_y1, l_x2, l_y2, l_x0, l_y0;
    char st_dest_symbol[MaxTextLen];
    char st_dest_numer[MaxTextLen];
    int st_dest_numer_i, ii, end_char;
    int LengthT;
    int siec;  //siec musi byc rozpoznana
    int i_net;
    int t_width, t_height;
    BOOL entire_poly_bak=get_entire_poly();
    set_entire_poly(0);
    BLOK *connection_blk=NULL;
    BLOK *parent_block;

    //rozpoznanie typu sieci oraz numeru wezla:

    //oczekiwanie na wskazanie obiektu sieci - poszukiwanie bloku rodzica *# lub *^
    //i odszukanie typu sieci  *#

    SIEC_PC=TRUE;

    i_net = Get_Net_Param(&connection_blk) ;  //if i_net==1 net selected, if i_net==2 connection selected

    SIEC_PC = FALSE;

    set_entire_poly(entire_poly_bak);

    if (i_net==0) {
        return;
    }
    //w zmiennej point_net_opis zapisana jest nazwa sieci
    //w zmiennej point_net_typ zapisany jest typ sieci 0..9

    strcpy(blok_type0,"*^");

    switch (point_net_typ)
    {
    case 0: kolor_sieci=koloryps.Pz;
        strcpy(blok_type,"Pz");
        break;
    case 1: kolor_sieci=koloryps.Cok;
        strcpy(blok_type,"Cok");
        break;
    case 2: kolor_sieci=koloryps.Co;
        strcpy(blok_type,"Co");
        break;
    case 3: kolor_sieci=koloryps.T;
        strcpy(blok_type,"T");
        break;
    case 4: kolor_sieci=koloryps.Tk;
        strcpy(blok_type,"Tk");
        break;
    case 5: kolor_sieci=koloryps.e;
        strcpy(blok_type,"e");
        break;
    case 6: kolor_sieci=koloryps.E;
        strcpy(blok_type,"E");
        break;
    case 7: kolor_sieci=koloryps.S;
        strcpy(blok_type,"S");
        break;
    case 8: kolor_sieci=koloryps.D;
        strcpy(blok_type,"D");
        break;
    case 9: kolor_sieci=koloryps.W;
        strcpy(blok_type,"W");
        break;
    default: kolor_sieci=koloryps.inne;
        strcpy(blok_type,"");
        break;
    }

    strcat(blok_type0,blok_type);

    CUR_OFF(X,Y);
    sel_nr=sel.nr;
    sel_gor=sel.gor;
    sel.nr=8;  //punkt
    // sel.gor=1;
    CUR_ON(X,Y);
    // save_params(0);
    memmove(&LiniaG0, &LiniaG, sizeof(LINIA));
    memmove(&LukG0, &LukG, sizeof(LUK));

    LiniaG.warstwa=Current_Layer;
    LiniaG.kolor=kolor_sieci;
    LiniaG.typ=128;
    LiniaG.obiektt2=O2BlockAparat;  //na wydruku i przy kresleniu dla profili
    //wszystkie linie najgrubsze O2BlockAparat
    //sa rysowane jako linie sienki czarne oraz
    //linie wlasciwe
    LukG.warstwa=Current_Layer;
    LukG.kolor=kolor_sieci;
    LukG.typ=128;
    LukG.obiektt2=O2BlockAparat;
    //dodatkowe menu
    dane_size0000=dane_size;

    PTR__GTMPBLOCK=(char*)connection_blk;

    if (Pline_single_xy(&x_begin, &y_begin, connection_blk))
    {
        memmove(&LiniaG, &LiniaG0, sizeof(LINIA));
        memmove(&LukG, &LukG0, sizeof(LUK));

        if (connection_blk==NULL)  //new connection
        {
            BLOK *net_polyline = (BLOK *) dane;
            if (net_polyline->kod_obiektu != B_PLINE) {
                return;
            }
            dane_size000 = (long) (sizeof(NAGLOWEK) + net_polyline->n);

            if (dane_size000 > 0) {
                group_block_with_type(dane_size000, 0, 0, blok_type0, B_SIEC);
            }
            parent_block=(BLOK*)dane;

            //setting node 1
            strcpy(st_dest,point_net_opis);
            strcat(st_dest,".1");
            if (read_esc(st_dest,60,153)==0)
            {
                return;
            }
        }
        else
        {
            connection_blk=(BLOK*)PTR__GTMPBLOCK; //shifted by adding polyline

            //deleting previous nodes
            //removing description blocks
            //searching for entire block with  blok_type "*~",  kod_bloku B_VERTEX
            parent_block= FIRSTB((char*)connection_blk);
            if (parent_block!=NULL)
            {

                adp1 = adp1b = (char *) parent_block + sizeof(NAGLOWEK) + B3 + parent_block->dlugosc_opisu_obiektu;
                adp2 = (char *) parent_block + sizeof(NAGLOWEK) + parent_block->n - 1;

                PTR__GTMPFIRST=(char*)parent_block;
                int vertex_no=0;
                while (adp1b < adp2)
                {
                    adp3 = find_block(adp1b, adp2, B_VERTEX, "*~");
                    if (adp3 != NULL)
                    {
                        b = (BLOK *) adp3;
                        adp4 = (char *) b + sizeof(NAGLOWEK) + b->n - 1;
                        //checking node 1
                        if (vertex_no==0)
                        {
                            char *node_name= find_obj(adp3, adp4, Otekst, n_typ_normal, 0);
                            if (node_name!=NULL)
                            {
                                strncpy (point_net_opis, ((TEXT*)node_name)->text, 30) ;
                            }
                        }
                        zmien_atrybut(adp3, adp4, ANieOkreslony, Ausuniety);

                        adp1b = adp3 + (sizeof(NAGLOWEK) + b->n);
                        vertex_no++;
                    } else break;
                }
                usun_blok(adp1, adp2);

                redraw();

                //new address of the block and range
                parent_block = (BLOK *) PTR__GTMPFIRST;
                adp1 = (char *) parent_block + sizeof(NAGLOWEK) + B3 + parent_block->dlugosc_opisu_obiektu;
                adp2 = (char *) parent_block + sizeof(NAGLOWEK) + parent_block->n - 1;
            }

            //setting node 1
            strcpy(st_dest,point_net_opis);
            strcat(st_dest,".1");
            if (read_esc(st_dest,60,153)==0)
            {
                return;
            }

        }

        //dopisanie opisu sieci w wezlach

        //////////////////
        //strcpy(change_text_dest,st_dest);
        /*rozbior st_dest ma symbol o numer*/
        /*poszukiwanie ostatniej cyfry*/
        end_char = (int)strlen(st_dest) - 1;

        while ((end_char>=0)  && (st_dest[end_char] > '\057') && (st_dest[end_char] < '\072'))
        {
            end_char--;
        }

        if (end_char==-1)
        {
            strcpy(st_dest_symbol,"");
            st_dest_numer_i=atoi(st_dest);
        }
        else
        {
            strcpy(st_dest_symbol,st_dest);
            st_dest_symbol[end_char+1]='\0';

            strcpy(st_dest_numer, st_dest + end_char + 1);
            st_dest_numer_i=atoi(st_dest_numer);
        }

        //////////////////
        //generowanie petli opisow w kazdym wezle
        b=parent_block;
        adp1=(char*)b+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
        adp2=(char*)b + sizeof(NAGLOWEK) + b->n - 1;
        find_next=TRUE;
        first_line=TRUE;
        //definicja kola
        K.warstwa=Current_Layer;
        K.blok=ElemBlok;
        K.r=(float)srednica_punktu_zalamania; //0.7f;
        K.kolor=kolor_sieci;
        K.obiektt2=O2BlockAparat;

        P.warstwa=Current_Layer;
        P.blok=ElemBlok;
        P.typ=0;
        P.kolor=kolor_sieci;
        P.obiektt2=O2BlockAparat;

        L1.warstwa=Current_Layer;
        L1.blok=ElemBlok;
        L1.kolor=kolor_sieci;
        L1.typ=32;
        L1.obiektt2=O2BlockAparat;

        T.warstwa=Current_Layer;
        T.blok=ElemBlok;
        T.kolor=kolor_sieci;
        T.wysokosc=3.5f;
        T.czcionka=zmwym.czcionka; //0;
        T.italics=1;
        T.width_factor=0.85f;
        T.bold=1;
        T.obiektt2=O2BlockAparat;

        n_points=0;

        while ((adp1<adp2) && (find_next==TRUE))
        {
            adp3=find_obj2(adp1, adp2, Olinia, Oluk, 128, 128, 0);
            if (adp3 != NULL)
            {
                nag=(NAGLOWEK *)adp3;
                if (nag->obiekt==Olinia)  //linia
                {
                    L=(LINIA *)adp3;
                    //mamy wiec dwa punkty
                    if (first_line==TRUE)
                    {
                        //punkt 1
                        if (n_points<max_points)
                        {
                            n_points++;
                            point_xy[n_points-1].x=L->x1;
                            point_xy[n_points-1].y=L->y1;
                            if (L->x1<L->x2)
                            { if (L->y1<L->y2) point_xy[n_points-1].kierunek=1;
                            else point_xy[n_points-1].kierunek=0;
                            }
                            else
                            {
                                if (L->y1<L->y2) point_xy[n_points-1].kierunek=0;
                                else point_xy[n_points-1].kierunek=1;
                            }
                        }
                        //punkt 2
                        if (n_points<max_points)
                        {
                            n_points++;
                            point_xy[n_points-1].x=L->x2;
                            point_xy[n_points-1].y=L->y2;
                        }
                        first_line=FALSE;
                        last_x=L->x2;
                        last_y=L->y2;
                    }
                    else
                    {
                        //punkt 1
                        if ((Check_if_Equal(L->x1,last_x)==FALSE) && (Check_if_Equal(L->y1,last_y)==FALSE))
                        {   //nowy punkt
                            if (n_points<max_points)
                            {
                                n_points++;
                                point_xy[n_points-1].x=L->x1;
                                point_xy[n_points-1].y=L->y1;
                                if (L->x1<L->x2)
                                { if (L->y1<L->y2) point_xy[n_points-1].kierunek=1;
                                else point_xy[n_points-1].kierunek=0;
                                }
                                else
                                {
                                    if (L->y1<L->y2) point_xy[n_points-1].kierunek=0;
                                    else point_xy[n_points-1].kierunek=1;
                                }
                            }
                        }
                        else //stary punkt
                            if (L->x1<L->x2)
                            { if (L->y1<L->y2) point_xy[n_points-1].kierunek=1;
                            else point_xy[n_points-1].kierunek=0;
                            }
                            else
                            {
                                if (L->y1<L->y2) point_xy[n_points-1].kierunek=0;
                                else point_xy[n_points-1].kierunek=1;
                            }
                        //punkt2
                        if (n_points<max_points)
                        {
                            n_points++;
                            point_xy[n_points-1].x=L->x2;
                            point_xy[n_points-1].y=L->y2;
                        }
                        first_line=FALSE;
                        last_x=L->x2;
                        last_y=L->y2;
                    }
                    adp1=adp3+sizeof(NAGLOWEK)+L->n;
                }
                else //luk
                {
                    l=(LUK *)adp3;
                    l_x1=l->x+l->r*cosf(l->kat1);
                    l_y1=l->y+l->r*sinf(l->kat1);
                    l_x2=l->x+l->r*cosf(l->kat2);
                    l_y2=l->y+l->r*sinf(l->kat2);

                    if ((Check_if_Equal(l_x1,l_x2)==TRUE) && (Check_if_Equal(l_y1,l_y2)==TRUE))
                    {
                        //zignorowano luk
                        ;
                    }
                    else
                    {
                        //mamy wiec dwa punkty
                        if (first_line==TRUE)
                        {
                            //punkt 1
                            if (n_points<max_points)
                            {
                                n_points++;
                                point_xy[n_points-1].x=l_x1;
                                point_xy[n_points-1].y=l_y1;
                                if (l_x1<l_x2)
                                { if (l_y1<l_y2) point_xy[n_points-1].kierunek=1;
                                else point_xy[n_points-1].kierunek=0;
                                }
                                else
                                {
                                    if (l_y1<l_y2) point_xy[n_points-1].kierunek=0;
                                    else point_xy[n_points-1].kierunek=1;
                                }
                            }
                            //punkt 2
                            if (n_points<max_points)
                            {
                                n_points++;
                                point_xy[n_points-1].x=l_x2;
                                point_xy[n_points-1].y=l_y2;
                            }
                            first_line=FALSE;
                            last_x=l_x2;
                            last_y=l_y2;
                        }
                        else
                        {
                            //ewentualne obrocenie luku
                            if ((Check_if_Equal(l_x2,last_x)==TRUE) && (Check_if_Equal(l_y2,last_y)==TRUE))
                            {
                                l_x0=l_x1;
                                l_y0=l_y1;
                                l_x1=l_x2;
                                l_y1=l_y2;
                                l_x2=l_x0;
                                l_y2=l_y0;
                            }
                            //punkt 1
                            if ((Check_if_Equal(l_x1,last_x)==FALSE) && (Check_if_Equal(l_y1,last_y)==FALSE))
                            {   //nowy punkt
                                if (n_points<max_points)
                                {
                                    n_points++;
                                    point_xy[n_points-1].x=l_x1;
                                    point_xy[n_points-1].y=l_y1;
                                    if (l_x1<l_x2)
                                    { if (l_y1<l_y2) point_xy[n_points-1].kierunek=1;
                                    else point_xy[n_points-1].kierunek=0;
                                    }
                                    else
                                    {
                                        if (l_y1<l_y2) point_xy[n_points-1].kierunek=0;
                                        else point_xy[n_points-1].kierunek=1;
                                    }
                                }
                            }
                            else //stary punkt
                                if (l_x1<l_x2)
                                { if (l_y1<l_y2) point_xy[n_points-1].kierunek=1;
                                else point_xy[n_points-1].kierunek=0;
                                }
                                else
                                {
                                    if (l_y1<l_y2) point_xy[n_points-1].kierunek=0;
                                    else point_xy[n_points-1].kierunek=1;
                                }
                            //punkt2
                            if (n_points<max_points)
                            {
                                n_points++;
                                point_xy[n_points-1].x=l_x2;
                                point_xy[n_points-1].y=l_y2;
                            }
                            first_line=FALSE;
                            last_x=l_x2;
                            last_y=l_y2;
                        }
                    }
                    adp1=adp3+sizeof(NAGLOWEK)+l->n;
                }
            }
            else find_next=FALSE;
        }

        PTR__GTMPFIRST=(char*)parent_block;  //setting global movable pointer

        if (n_points>0)  //musza byc co najmniej 2 punkty
        {
            if (nag->obiekt==Olinia)  //linia
            {
                //ostatni punkt
                if (L->x1<L->x2)
                { if (L->y1<L->y2) point_xy[n_points-1].kierunek=1;
                else point_xy[n_points-1].kierunek=0;
                }
                else
                {
                    if (L->y1<L->y2) point_xy[n_points-1].kierunek=0;
                    else point_xy[n_points-1].kierunek=1;
                }
            }
            else //luk
            {
                if (l_x1<l_x2)
                { if (l_y1<l_y2) point_xy[n_points-1].kierunek=1;
                else point_xy[n_points-1].kierunek=0;
                }
                else
                {
                    if (l_y1<l_y2) point_xy[n_points-1].kierunek=0;
                    else point_xy[n_points-1].kierunek=1;
                }
            }

            for (i=0; i<n_points; i++)
            {
                //punkt
                PTR__GTMPBLOCK=NULL;
                //if (FALSE == add_block_with_type (point_xy[i].x, point_xy[i].y, "*~", 0, B_VERTEX))
                if (FALSE ==  add_block_in_block_with_type ((BLOK*)PTR__GTMPFIRST, point_xy[i].x, point_xy[i].y, "*~", 0, B_VERTEX))
                {
                    return;
                }
                P.x=(float)point_xy[i].x;
                P.y=(float)point_xy[i].y;
                if (dodaj_obiekt_((BLOK*)dane, &P)==NULL) return;
                rysuj_obiekt((char*)&P,COPY_PUT,1);

                K.x=(float)point_xy[i].x;
                K.y=(float)point_xy[i].y;
                if (dodaj_obiekt_((BLOK*)dane, &K)==NULL) return;
                rysuj_obiekt((char*)&K,COPY_PUT,1);
                //linie
                if (i==0)
                {
                    //opis
                    T.x=(float)(point_xy[i].x+2.);
                    T.y=(float)(point_xy[i].y+2.);
                    sprintf(st_dest,"%s",point_net_opis);
                    strcpy(&T.text[0],st_dest);
                    LengthT=(int)strlen(T.text);
                    T.dl=LengthT;
                    T.n=T18+T.dl;
                    T.ukryty=1;
                    if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return;
                    rysuj_obiekt((char*)&T,COPY_PUT,1);
                    T.ukryty=0;
                    ////////////////////////
                    /*
                    //rzedna
                    T.typ=n_rzedna_kanalu_p;
                    T.wysokosc=2.0;
                    T.x=L1.x1+1;
                    T.y=L1.y1-2.25;
                    T.ukryty=1;
                    strcpy(st_dest,"");
                    strcpy(&T.text[0],st_dest);
                    LengthT=strlen(T.text);
                    T.dl=LengthT;
                    T.n=T18+T.dl;
                    if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return 0;
                    rysuj_obiekt((char*)&T,COPY_PUT,1);
                    //srednica
                    T.typ=n_srednica_kanalu_p;
                    T.x=L1.x1+1;
                    T.y=L1.y1-4.5;
                    strcpy(st_dest,"");
                    strcpy(&T.text[0],st_dest);
                    LengthT=strlen(T.text);
                    T.dl=LengthT;
                    T.n=T18+T.dl;
                    if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return 0;
                    rysuj_obiekt((char*)&T,COPY_PUT,1);

                    T.wysokosc=3.5;
                    T.ukryty=0;
                    T.typ=n_typ_normal;
                    */

                    ////////////////////////
                }
                else
                {
                    if (point_xy[i].kierunek==0)  //  /
                    {
                        L1.x1=(float)point_xy[i].x;
                        L1.y1=(float)point_xy[i].y;
                        L1.x2=(float)(point_xy[i].x+5.);
                        L1.y2=(float)(point_xy[i].y+10.);
                        if (dodaj_obiekt_((BLOK*)dane, &L1)==NULL) return;
                        rysuj_obiekt((char*)&L1,COPY_PUT,1);
                        //opis
                        T.x=0;  //temporary
                        T.y=0;
                        sprintf(st_dest,"%s%d",st_dest_symbol,st_dest_numer_i+i-1);
                        strcpy(&T.text[0],st_dest);
                        LengthT=(int)strlen(T.text);
                        T.dl=LengthT;
                        T.n=T18+T.dl;

                        double t_len_mm = Get_Text_Len(&T, T.text, 1.0, 0, &t_width, &t_height, NULL, NULL);
                        L1.x1=(float)(point_xy[i].x+5.);
                        L1.y1=(float)(point_xy[i].y+10.);
                        L1.x2=(float)(point_xy[i].x+5.+(float)t_len_mm);  //15.
                        L1.y2=(float)(point_xy[i].y+10.);
                        if (dodaj_obiekt_((BLOK*)dane, &L1)==NULL) return;
                        rysuj_obiekt((char*)&L1,COPY_PUT,1);
                        //opis add
                        T.x=(float)(L1.x1+1.);
                        T.y=(float)(L1.y1+0.5);
                        if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return;
                        rysuj_obiekt((char*)&T,COPY_PUT,1);
                        /////????????
                        //rzedna
                        T.typ=n_rzedna_kanalu_p;
                        T.wysokosc=2.0f;
                        T.x=(float)(L1.x1+1.);
                        T.y=(float)(L1.y1-2.25);
                        T.ukryty=1;
                        strcpy(st_dest,"");
                        strcpy(&T.text[0],st_dest);
                        LengthT=(int)strlen(T.text);
                        T.dl=LengthT;
                        T.n=T18+T.dl;
                        if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return;
                        rysuj_obiekt((char*)&T,COPY_PUT,1);
                        //srednica
                        T.typ=n_srednica_kanalu_p;
                        T.x=(float)(L1.x1+1.);
                        T.y=(float)(L1.y1-4.5);
                        strcpy(st_dest,"");
                        strcpy(&T.text[0],st_dest);
                        LengthT=(int)strlen(T.text);
                        T.dl=LengthT;
                        T.n=T18+T.dl;
                        if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return;
                        rysuj_obiekt((char*)&T,COPY_PUT,1);

                        T.wysokosc=3.5f;
                        T.ukryty=0;
                        T.typ=n_typ_normal;
                        ///////////////////
                    }
                    else
                    {
                        L1.x1=(float)point_xy[i].x;
                        L1.y1=(float)point_xy[i].y;
                        L1.x2=(float)(point_xy[i].x+5.);
                        L1.y2=(float)(point_xy[i].y-10.);
                        if (dodaj_obiekt_((BLOK*)dane, &L1)==NULL) return;
                        rysuj_obiekt((char*)&L1,COPY_PUT,1);

                        //opis
                        T.x=0; //temporary
                        T.y=0;
                        sprintf(st_dest,"%s%d",st_dest_symbol,st_dest_numer_i+i-1);
                        strcpy(&T.text[0],st_dest);
                        LengthT=(int)strlen(T.text);
                        T.dl=LengthT;
                        T.n=T18+T.dl;

                        double t_len_mm = Get_Text_Len(&T, T.text, 1.0, 0, &t_width, &t_height, NULL, NULL);
                        L1.x1=(float)(point_xy[i].x+5.);
                        L1.y1=(float)(point_xy[i].y-10.);
                        L1.x2=(float)(point_xy[i].x+5.+t_len_mm);  //15.
                        L1.y2=(float)(point_xy[i].y-10.);
                        if (dodaj_obiekt_((BLOK*)dane, &L1)==NULL) return;
                        rysuj_obiekt((char*)&L1,COPY_PUT,1);
                        //opis add
                        T.x=(float)(L1.x1+1.);
                        T.y=(float)(L1.y1+0.5);
                        if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return;
                        rysuj_obiekt((char*)&T,COPY_PUT,1);
                        ////////////?????????????
                        //rzedna
                        T.typ=n_rzedna_kanalu_p;
                        T.wysokosc=2.0f;
                        T.x=(float)(L1.x1+1.);
                        T.y=(float)(L1.y1-2.25);
                        T.ukryty=1;
                        strcpy(st_dest,"");
                        strcpy(&T.text[0],st_dest);
                        LengthT=(int)strlen(T.text);
                        T.dl=LengthT;
                        T.n=T18+T.dl;
                        if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return;
                        rysuj_obiekt((char*)&T,COPY_PUT,1);
                        T.typ=n_typ_normal;
                        //srednica
                        T.typ=n_srednica_kanalu_p;
                        T.x=(float)(L1.x1+1.);
                        T.y=(float)(L1.y1-4.5);
                        strcpy(st_dest,"");
                        strcpy(&T.text[0],st_dest);
                        LengthT=(int)strlen(T.text);
                        T.dl=LengthT;
                        T.n=T18+T.dl;
                        if (dodaj_obiekt_((BLOK*)dane, &T)==NULL) return;
                        rysuj_obiekt((char*)&T,COPY_PUT,1);

                        T.typ=n_typ_normal;
                        T.wysokosc=3.5f;
                        T.ukryty=0;
                        ///////////////////////
                    }
                }
            }
        }
    }

error_go_c:

    PTR__GTMPBLOCK=NULL;

    CUR_OFF(X,Y);
    sel.nr=sel_nr;
    sel.gor=sel_gor;
    CUR_ON(X,Y);

    //after all
    memmove(&LiniaG, &LiniaG0, sizeof(LINIA));
    memmove(&LukG, &LukG0, sizeof(LUK));

    color_bar();

    //if (read_txt==TRUE)
        redraw();
}

void  Wstaw_siec_z_tabeli(void)
{
   int k=Rysuj_siec(-1);
}

void Rysuj_Pz(void)
{
	int k=Rysuj_siec(0);
}

void Rysuj_Cok(void)
{
	int k=Rysuj_siec(1);
}

void Rysuj_Co(void)
{
	int k=Rysuj_siec(2);
}

void Rysuj_T(void)
{
	int k=Rysuj_siec(3);
}

void Rysuj_Tk(void)
{
	int k=Rysuj_siec(4);
}

void Rysuj_e(void)
{
	int k=Rysuj_siec(5);
}

void Rysuj_E(void)
{
	int k=Rysuj_siec(6);
}

void Rysuj_S(void)
{
	int k=Rysuj_siec(7);
}

void Rysuj_D(void)
{
	int k=Rysuj_siec(8);
}

void Rysuj_W(void)
{
	int k=Rysuj_siec(9);
}

void Rysuj_inne(void)
{
	int k=Rysuj_siec(10);
}


void Set_b_align_T(void)
/*--------------------*/
{ char st[6];
 set_b_align(TRUE);
 strcpy(st,_YES__) ;
 menu_par_new ((*mInstalacje.pola)[6].txt, st) ;
}

void Set_b_align_N(void)
/*--------------------*/
{char st[6];
 set_b_align(FALSE);
 strcpy(st,_NO__) ;
 menu_par_new ((*mInstalacje.pola)[6].txt, st) ;
}

void Opcje_Instalacje(void)
/*-----------------------*/
{
 ;
}

void Wstaw_kolizje(void)
/*--------------------*/
{   char fn[MaxLen]="";
  sprintf(fn,"%s%s%s.axx",bloki_dir,SSlash,BlockName[0]);
  WezSymbol(fn);

    if (get_znacznik_aplikacji()<100)
        put_znacznik_aplikacji(100);
}

void Wstaw_reper(void)
/*------------------*/
{ char fn[MaxLen]="";
  sprintf(fn,"%s%s%s.axx",bloki_dir,SSlash,BlockName[1]);
  WezSymbol(fn);
}

void Polacz_sieci(void)
/*------------------*/
{
 ;
}

void Hektometry_Kilometry(void)
/*---------------------------*/
{
    BLOK *b,*bb, *bbb;
    LINIA *L;
    LUK *l;
    TEXT *t, T, *T_odl;
    BOOL find_last;
    char *adp0, *adp00, *adp_odl, *adp, *adk, *adp1, *adp2, *adk1, *adp3, *adk3, *adp4;
    char *adp5, *adk5;
    int l_blocks, l_u_blocks;
    char st[30];
    int len;
    char buf [300];
    double X0,Y0;
    int object_no ;
    BLOK *ptrs_block;
    NAGLOWEK *ptr_object;
    BOOL b_ret;
    double df_x, df_y;
    double hektometr, kilometr;
    double Xx,Yy;
    unsigned long del_adp, del_adp1;
    char tekst_p[30];
    char tekst_p1[30];
    int i, j, l_h, l_k, l_h0, l_k0;
    int l_texts, LengthT;
    double odleglosc_0, odleglosc_00;
    char blockname[MAXPATH];
    char *bp;

    TTF_redraw=FALSE;

    //usuniecie wszystkich blokow $hm $km
    adp=dane;
    adk=dane+dane_size;
    strcpy(st,"$km");
    find_last=FALSE;
    l_blocks=0;
    while (find_last==FALSE)
    {
        adp1=find_block(adp, adk, B_NIEOKRESLONY, st);
        if (adp1!=NULL)
        {
            b=(BLOK *)adp1;
            adk1=adp1+sizeof(NAGLOWEK)+b->n -1;
            //wszystkie elementy bloku
            zmien_atrybut(adp1,adk1,ANieOkreslony,Ablok);
            l_blocks++;
            adp=adp1+sizeof(NAGLOWEK)+b->n;
        }
        else find_last=TRUE;
    }

    //hektometry

    adp=dane;
    adk=dane+dane_size;
    strcpy(st,"$hm");
    find_last=FALSE;
    while (find_last==FALSE)
    {
        adp1=find_block(adp, adk, B_NIEOKRESLONY, st);
        if (adp1!=NULL)
        {
            b=(BLOK *)adp1;
            adk1=adp1+sizeof(NAGLOWEK)+b->n -1;
            //wszystkie elementy bloku
            zmien_atrybut(adp1,adk1,ANieOkreslony,Ablok);
            l_blocks++;
            adp=adp1+sizeof(NAGLOWEK)+b->n;
        }
        else find_last=TRUE;
    }
    if (l_blocks>0)
    {
        ADP=dane;
        ADK=dane+dane_size;
        blokzap(ADP,ADK,Ablok,COPY_PUT,0);
        zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
        usun_blok(ADP,ADK);
    }

    //wczytanie bloku $hm
    Xx=X;
    Yy=Y;

    X=0; Y=0;

    sprintf(blockname,"%s%s%s.axx",bloki_dir,SSlash,BlockName[2]);

    if(ReadBlock_(blockname,&X0,&Y0,&ADP,&ADK , buf, MaxLen - 10, &object_no, TRUE)) return; // 0;

    transformacja_blok(ADP,ADK,X0, Y0,skala_bloku_hm_km,skala_bloku_hm_km,Tskala,0);

    transformacja_blok(ADP,ADK,X-X0,Y-Y0,0,0,Tprzesuw,0);
    //  blokzap(ADP,ADK,Ablok,COPY_PUT,1);
    //  zmien_atrybut(ADP,ADK,Ablok,Aoblok);

    //zaznaczenie wczytanego (ostatniego) bloku
    //to jest wykonywane automatycznie po wczytaniu

    //ustawienie parametrow podzialu
    set_b_divide(0);
    hektometr=100.0*(1000.0/Jednostki);
    set_df_seg_len(hektometr);

    //odszukiwanie polilinii
    b=(BLOK *)dane;
    adp=dane+sizeof(NAGLOWEK) + b->n;
    adk=dane+dane_size;
    find_last=FALSE;
    l_h0=0;
    l_k0=0;
    //petla poszukiwania polilinii

    while (find_last==FALSE)
    {
        adp0=find_block_sub_on(adp, adk, B_SIEC, "*#",2);
        if (adp0!=NULL)
        {
            bb=(BLOK *)adp0;
            del_adp=dane+dane_size-adp0-sizeof(NAGLOWEK)-bb->n;
            adp=adp0+sizeof(NAGLOWEK)+B3+bb->dlugosc_opisu_obiektu;
            adk=adp0+sizeof(NAGLOWEK)+bb->n;
            //odszukanie wartosci poczatkowej odleglosci
            odleglosc_0=0;
            adp_odl=find_obj(adp, adk, Otekst, n_typ_atrybut, 0);
            if (adp_odl!=NULL)  //znaleziono tekst atrybut
            {
                T_odl=(TEXT *)adp_odl;
                if (strlen(T_odl->text)>0)
                {
                    odleglosc_00=strtod(decimal_dot(T_odl->text), &bp);
                    odleglosc_0=strtod(decimal_dot(T_odl->text), &bp)*(1000.0/Jednostki);
                    l_h0 = (int)((fmod (odleglosc_00,1000)) / 100. );
                    l_k0 = (int)(odleglosc_00 / 1000. );
                }
            }

            adp00=find_block(adp, adk, B_PLINE, "");
            if (adp00!=NULL)
            {
                ptrs_block=(BLOK *)adp00;
                ptr_object=(NAGLOWEK *)(adp00+sizeof(NAGLOWEK)+B3+ptrs_block->dlugosc_opisu_obiektu);
                if (ptr_object->obiekt==Olinia)
                {
                    L=(LINIA *)ptr_object;
                    df_x=L->x1;
                    df_y=L->y1;
                }
                else //luk
                {
                    l=(LUK *)ptr_object;
                    df_x=l->x+l->r*cosf(l->kat1);
                    df_y=l->y+l->r*sinf(l->kat1);
                }
                //podzial polilinii
                if ((ptrs_block->kod_obiektu == B_PLINE) &&
                       (ptrs_block->obiektt1 == OB1NOCHANGE) ||
                     (ptrs_block->obiektt1 == OB1CHANGE_SCALE) ||
                     (ptrs_block->obiektt1 == OB1CHANGE_DRAG) ||
                    (ptrs_block->obiektt1 == OB1CHANGE_BREAK))
                {
                    dane_size0=dane_size;
                    global_reference_angle_rad=reference_angle*Pi/180.;
                    b_ret = divide_pline (ptrs_block, ptr_object, df_x, df_y, odleglosc_0) ;
                    global_reference_angle_rad=0.0;
                    if (b_ret)
                    {
                        //edycja blokow
                        adp3=dane;
                        adk3=dane+dane_size-dane_size0-1;
                        //policzenie liczby tekstow
                        l_texts=count_texts(adp3, adk3);
                        if (l_texts>0)
                        {
                            l_u_blocks=0;
                            for (i=1; i<=l_texts; i++)
                            {
                                //odszukanie adresu kolejnego tekstu
                                adp4=find_obj_count_abs(dane, dane+dane_size, Otekst, n_typ_normal, 0, i);
                                if (adp4!=NULL)
                                {
                                    //zdefiniowanie tekstu
                                    //zmiana istniejacego tekstu
                                    t=(TEXT*)adp4;
                                    memmove(&T,(void *)t,sizeof(NAGLOWEK)+t->n);

                                    j=l_texts-i+1;

                                    l_h = (l_h0 + j) % 10 ;
                                    l_k = l_k0 + (l_h0 + j) / 10 ;

                                    if (l_h==0) //usuniecie bloku pelnego kilometra
                                    {
                                        //rysuj_obiekt((char*)t,COPY_PUT,0);
                                        //odszukanie poczatku bloku
                                        adp5=(char*)LASTB (adp4);
                                        //zaznaczenie bloku
                                        if (adp5!=NULL)
                                        {
                                            bbb=(BLOK *)adp5;
                                            adk5=adp5+sizeof(NAGLOWEK)+bbb->n -1;
                                            //wszystkie elementy bloku
                                            blokzap(adp5,adk5,ANieOkreslony,COPY_PUT,0);
                                            zmien_atrybut(adp5,adk5,ANieOkreslony,Ausuniety);
                                            l_u_blocks++;
                                        }
                                        //przerysowanie bloku
                                    }
                                    else
                                    {
                                        if (l_k>0)
                                        {
                                            sprintf(tekst_p,"%d",l_k);
                                            strncat(tekst_p,"+",sizeof(tekst_p) - strlen(tekst_p) - 1);
                                        }
                                        else strcpy(tekst_p,"");
                                        sprintf(tekst_p1,"%d",l_h);
                                        strncat(tekst_p,tekst_p1,sizeof(tekst_p1)-strlen(tekst_p1)-1);

                                        strcpy(&T.text[0],tekst_p);
                                        LengthT=(int)strlen(T.text);
                                        T.dl=LengthT;
                                        T.n=T18+T.dl;
                                        rysuj_obiekt((char*)t,COPY_PUT,0);
                                        t=(TEXT*)korekta_obiekt((void *)adp4,(void*)&T);
                                        rysuj_obiekt((char*)t,COPY_PUT,1);
                                        if (PTRS__Text_Style[t->czcionka]->type == 2) TTF_redraw=TRUE;
                                    }
                                }
                            }
                            if (l_u_blocks>0)
                            {
                                adp5=dane;
                                adk5=dane+dane_size;
                                // blokzap(adp5,adk5,Ausuniety,COPY_PUT,0);
                                usun_blok(adp5,adk5);
                            }
                        }
                    }
                }
            }
            adp=dane+dane_size-del_adp;
            adk=dane+dane_size;
        }
        else find_last=TRUE;
    }

    //usuniecie bloku zaznaczonego
    //  blokzap(ADP,ADK,Ablok,COPY_PUT,1);
    //  blokzap(ADP,ADK,Ablok,COPY_PUT,0);

    zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
    usun_blok(ADP,ADK);

    //wczytanie bloku $km

    X=0; Y=0;

    sprintf(blockname,"%s%s%s.axx",bloki_dir,SSlash,BlockName[3]);

    if(ReadBlock_(blockname,&X0,&Y0,&ADP,&ADK , buf, MaxLen - 10, &object_no, TRUE)) return; // 0;

    transformacja_blok(ADP,ADK,X0, Y0,skala_bloku_hm_km,skala_bloku_hm_km,Tskala,0);

    transformacja_blok(ADP,ADK,X-X0,Y-Y0,0,0,Tprzesuw,0);
    //  blokzap(ADP,ADK,Ablok,COPY_PUT,1);
    //  zmien_atrybut(ADP,ADK,Ablok,Aoblok);

    //zaznaczenie wczytanego (ostatniego) bloku
    //to jest wykonywane automatycznie po wczytaniu

    //ustawienie parametrow podzialu
    set_b_divide(0);
    kilometr=1000.0*(1000.0/Jednostki);
    set_df_seg_len(kilometr);

    //odszukiwanie polilinii
    b=(BLOK *)dane;
    adp=dane+sizeof(NAGLOWEK) + b->n;
    adk=dane+dane_size;
    find_last=FALSE;
    l_h0=0;
    l_k0=0;
    //petla poszukiwania polilinii

    while (find_last==FALSE)
    {
        adp0=find_block_sub_on(adp, adk, B_SIEC, "*#",2);
        if (adp0!=NULL)
        {
            bb=(BLOK *)adp0;
            del_adp=dane+dane_size-adp0-(int)sizeof(NAGLOWEK)-bb->n;
            adp=adp0+sizeof(NAGLOWEK)+B3+bb->dlugosc_opisu_obiektu;
            adk=adp0+sizeof(NAGLOWEK)+bb->n;

            //odszukanie wartosci poczatkowej odleglosci
            odleglosc_0=0;
            adp_odl=find_obj(adp, adk, Otekst, n_typ_atrybut, 0);
            if (adp_odl!=NULL)  //znaleziono tekst atrybut
            {
                T_odl=(TEXT *)adp_odl;
                if (strlen(T_odl->text)>0)
                {
                    odleglosc_00=strtod(decimal_dot(T_odl->text), &bp);
                    odleglosc_0=strtod(decimal_dot(T_odl->text), &bp)*(1000.0/Jednostki);
                    l_k0 = (int)(odleglosc_00 / 1000. );
                }
            }

            adp00=find_block(adp, adk, B_PLINE, "");
            if (adp00!=NULL)
            {
                ptrs_block=(BLOK *)adp00;
                ptr_object=(NAGLOWEK *)(adp00+sizeof(NAGLOWEK)+B3+ptrs_block->dlugosc_opisu_obiektu);
                if (ptr_object->obiekt==Olinia)
                {
                    L=(LINIA *)ptr_object;
                    df_x=L->x1;
                    df_y=L->y1;
                }
                else //luk
                {
                    l=(LUK *)ptr_object;
                    df_x=l->x+l->r*cosf(l->kat1);
                    df_y=l->y+l->r*sinf(l->kat1);
                }
                //podzial polilinii
                if ((ptrs_block->kod_obiektu == B_PLINE) &&
                       (ptrs_block->obiektt1 == OB1NOCHANGE) ||
                     (ptrs_block->obiektt1 == OB1CHANGE_SCALE) ||
                     (ptrs_block->obiektt1 == OB1CHANGE_DRAG) ||
                     (ptrs_block->obiektt1 == OB1CHANGE_BREAK))
                {
                    dane_size0=dane_size;
                    b_ret = divide_pline (ptrs_block, ptr_object, df_x, df_y, odleglosc_0) ;
                    if (b_ret)
                    {
                        //edycja blokow
                        adp3=dane;
                        adk3=dane+dane_size-dane_size0-1;
                        //policzenie liczby tekstow
                        l_texts=count_texts(adp3, adk3);
                        if (l_texts>0)
                        {
                            for (i=1; i<=l_texts; i++)
                            {
                                //odszukanie adresu kolejnego tekstu
                                adp4=find_obj_count_abs(dane, dane+dane_size, Otekst, n_typ_normal, 0, i);
                                if (adp4!=NULL)
                                {
                                    //zdefiniowanie tekstu
                                    //zmiana istniejacego tekstu
                                    t=(TEXT*)adp4;
                                    memmove(&T,(void *)t,sizeof(NAGLOWEK)+t->n);

                                    l_k=l_k0 + l_texts-i+1;

                                    if (l_k>0)
                                    {
                                        sprintf(tekst_p,"%d",l_k);
                                    }
                                    else strcpy(tekst_p,"");

                                    strcpy(&T.text[0],tekst_p);
                                    LengthT=(int)strlen(T.text);
                                    T.dl=LengthT;
                                    T.n=T18+T.dl;
                                    rysuj_obiekt((char*)t,COPY_PUT,0);
                                    t=(TEXT*)korekta_obiekt((void *)adp4,(void*)&T);
                                    rysuj_obiekt((char*)t,COPY_PUT,1);
                                    if (PTRS__Text_Style[t->czcionka]->type == 2) TTF_redraw=TRUE;
                                }
                            }
                        }
                    }
                }
            }
            adp=dane+dane_size-del_adp;
            adk=dane+dane_size;
        }
        else find_last=TRUE;
    }


    //usuniecie bloku zaznaczonego
    //  blokzap(ADP,ADK,Ablok,COPY_PUT,1);
    //  blokzap(ADP,ADK,Ablok,COPY_PUT,0);
    zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
    usun_blok(ADP,ADK);

    X=Xx;
    Y=Yy;

    if (TTF_redraw) redraw();

    CUR_OFF(X,Y);
    CUR_ON(X,Y);

}

static void restore_reference_angle(void)
{
    reference_angle=0.0;
    simulate_keypress(ESC);
}

static void Reference_Angle(void)
{
    int ret;
    char *obj_ptr;
    unsigned int typ;
    char kom[MaxTextLen];
    PLINIA PL ;
    double kat;
    int sel_akt;
    char *adr;
    static void ( *LASTFUN_BAK)(void)=NULL;

    typ=Blinia;

    ///setting backspace function
    LASTFUN_BAK=LASTFUN;
    LASTFUN=(void (*)(void))restore_reference_angle;
    ////selecting single object
    typ=Blinia;
    strcpy(global_kom, (char*)_SELECT_LINE_);
    sel_akt=sel.akt;
    sel.akt=1;
    adr=select_n(&typ, NULL, 255);
    sel.akt=sel_akt;
    if (adr!=NULL)
    {
        parametry_lini ((LINIA *)adr, &PL);
        reference_angle=PL.kat;
    }
    ///restoring backspace function
    LASTFUN=LASTFUN_BAK;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
}

void Adjust_Reference_Angle(void)
{   char *adp1, *adp2;
    char *adp3, *adp5;
    char *adp01, *adp02;
    BLOK *b1;
    TEXT *t;
    double kat, dkat;
    double rotation_angle=reference_angle*Pi/180.;
    double kos, koc;
    T_Desc_Ex_Block *ptrs_desc_bl ;
    BOOL find_next;

    Semaphore = FALSE;

    blokzap(ADP,ADK,Ablok,COPY_PUT,0); //to avoid translucency interference

    adp01=ADP;
    adp02=ADK;
    //searching for node blocks
    find_next=TRUE;
    while ((adp01<adp02) && (find_next==TRUE))
    {
        adp3 = find_block_sub_on(adp01, adp02, B_VERTEX, "*~", 2);
        if (adp3 != NULL)
        {
            b1 = (BLOK *) adp3;
            if (b1->atrybut==Ablok) {
                adp1 = adp3 + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu;
                adp2 = adp3 + sizeof(NAGLOWEK) + b1->n - 1;
                adp5 = find_obj(adp1, adp2, Otekst, n_typ_normal, 0);
                if (adp5 != NULL) {
                    kat = ((TEXT *) adp5)->kat;
                    dkat = rotation_angle - kat;
                    kos = sin(dkat);
                    koc = cos(dkat);
                    //rotating of entire block
                    ptrs_desc_bl = (T_Desc_Ex_Block *) (&b1->opis_obiektu[0]);
                    transformacja_blok(adp1, adp2, (double) ptrs_desc_bl->x, (double) ptrs_desc_bl->y, kos, koc, Tobrot,0);
                }
            }
            adp01 = adp3 + sizeof(NAGLOWEK) + b1->n;
        }
        else find_next=FALSE;

        Semaphore = TRUE;
    }

    blokzap(ADP,ADK,Ablok,COPY_PUT,1);
    memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
    zmien_atrybut_undo(dane, dane +	dane_size);
    zmien_atrybut(ADP,ADK,Ablok,Aoblok);

    if (TTF_redraw) redraw();
    TTF_redraw=FALSE;

    ADP = ADK = NULL ;
}

static void MarkDrawing(void)
{  char *marking;
    int l_kr=get_znacznik_aplikacji();
   if (l_kr<100)
   {
       put_znacznik_aplikacji(100);
       //potwierdzenie zmiany
       int ret=ask_question(1, "","OK","", "", 12, get_komunikat_ptr(230), 11, 1, 61);
       Change=TRUE;
   }
    else
    {
        switch (l_kr)
        {
        case 100:
            marking=get_komunikat_ptr(231);
            break;
        case 101:
            marking=get_komunikat_ptr(232);
            break;
        default:
            marking="ERROR";
            break;
        }
        //potwierdzenie zmiany
        int ret=ask_question(1, "","OK","", "", 12, marking, 11, 1, 62);
    }
}

static void nooop(void)
{}

static void (* COMNDInstal[])(void)={ nooop, Rysuj_przylacze, Wstaw_kolizje, Wstaw_reper, Polacz_sieci, Hektometry_Kilometry, nooop, Generowanie_tabeli_wspolrzednych,
                                      Wstaw_siec_z_tabeli, nooop, Zapisz_Profil,  Wstaw_Profil, Wstaw_Studnie, nooop, MarkDrawing,
                              /*15*/   Rysuj_Pz,Rysuj_Cok,Rysuj_Co,Rysuj_T,Rysuj_Tk,Rysuj_e,Rysuj_E,Rysuj_S,Rysuj_D,Rysuj_W,Rysuj_inne,
                              /*26*/  Set_b_align_T, Set_b_align_N, Reference_Angle, Change_Reference_Angle};

void Instalacje(void)
{int n;
 BOOL align;
 char st[12];

 add_zbior_2=0;
 align=get_b_align();
 if (align==TRUE) strcpy(st,_YES__) ; else strcpy(st,_NO__) ;
 menu_par_new ((*mInstalacje.pola)[6].txt, st) ;
 sprintf(st,"%.2f",reference_angle);
 menu_par_new ((*mInstalacje.pola)[13].txt, st) ;
 if ((n=getwsp1(&mInstalacje)-1)>=0)
           (*COMNDInstal[n])();    /* n=-1 -> esc */
 LASTFUN=COMNDInstal[n];
 add_zbior_2=1;
}


static void add_profil_layer (int new_layer)
/*----------------------------------------*/
{
  LAYER layer=Defwar;
  int no_dlg_line;
  int i;

  if (new_layer >= MAX_NUMBER_OF_LAYERS)
   {
     ErrList (36);
     return;
   }

   for (i=No_Layers; i<new_layer; i++)
    {
     memmove( &(Layers[No_Layers]), &layer, sizeof(LAYER));
     sprintf(Layers [No_Layers].name,"");
     No_Layers++;
    }
   memmove( &(Layers[No_Layers]), &layer, sizeof(LAYER));
   sprintf(Layers [No_Layers].name,"%s #%d",_Profil_,No_Layers);
   No_Layers++;
}

int find_max_layer (void)
/*---------------------*/
{
  int max_layer;
  NAGLOWEK *nag;
  BLOK  *b;
  LINIA *L;
  char  *adp,  *adk;

  max_layer = 0;
  adp = dane ;
  adk = dane + dane_size ;
  nag = (NAGLOWEK*)adp;
  while (nag->obiekt != Okoniec && adp < adk)
  {
    if (nag->obiekt == OdBLOK)
    {
      b=(BLOK *)adp;
	   adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    { 
     if ((nag->atrybut != Ausuniety) && (nag->atrybut!=Abad))
      {
        L=(LINIA *)adp;
        if (max_layer<((int)L->warstwa)) 
			   max_layer=L->warstwa;
      }
     adp += nag->n + sizeof(NAGLOWEK) ;
    }
   nag = (NAGLOWEK*)adp;
  }
 return max_layer;
}

int find_base_point_cur_layer (double *x_origin, double *y_origin)
/*--------------------------------------------------------------*/
{
  BOOL b_retval;
  BLOK  *b;
  LINIA *L;
  T_Point *P;
  NAGLOWEK *nag;
  void  *ad;
  char  *adp,	*adk;
  char key;
  BOOL first_base_point;
  double del_xx, del_yy, del_xy, min_xy;
  double xx_origin, yy_origin;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  BLOK  *ptrs_blok;
  char st[128];

  adp	= dane ;
  adk	= dane +	dane_size ;
  first_base_point=FALSE;
  
  L =	(LINIA*)adp;
  while ((L->obiekt != Okoniec)	&&	(adp < adk) && (first_base_point==FALSE))
  {
	 if (L->obiekt	==	OdBLOK)
	 {
	  b=(BLOK *)adp;
	  adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 }
	 else
	 {
		if	((L->obiekt	==	Opoint) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) &&
			 (Layers[L->warstwa].on==TRUE))
		 {
        if (L->warstwa==Current_Layer)
        {
		  P =	(T_Point*)adp;
        if ((P->obiektt2 = O2BlockSpecial) && ((P->typ==11) || (P->typ==12) /*starsza wersja*/ || (P->typ==1) /*najstarsza wersja*/))  //musi być elementem bloku
         {
		   //punkt bazowy nie moze byc elementem bloku *KP ani *PK
			if ((P->typ==1) && (P->blok==1)) //stara wersja
			{
				if (NULL != (ptrs_blok=FIRSTB(adp)))
				{
					if (ptrs_blok->dlugosc_opisu_obiektu>1)
					{
						ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_blok->opis_obiektu [0]) ;
						strcpy (st, &ptrs_desc_bl->sz_type [0]) ;
						if ((strncmp(st,"*KP",2)!=0) && (strncmp(st,"*PP",2)!=0)) //nie jest to blok pomiaru
						{
							first_base_point=TRUE;
						}
					}
				}
				else 
				{
					first_base_point=TRUE;
				}
			}
		    else //nowa wersja
			{
				first_base_point=TRUE;
			}

			if (first_base_point==TRUE)
			{
				xx_origin=P->x;
				yy_origin=P->y;
				//min_xy=del_xy;
			}
			
         }
        }
		 }
      adp += L->n	+ sizeof(NAGLOWEK) ;
	 }
	 L	= (LINIA*)adp;
  }
  if (first_base_point==TRUE)
   {
    *x_origin=xx_origin;
	 *y_origin=yy_origin;
    return 1;
   }
    
  *x_origin=0;
   *y_origin=0;
  return 0;
}

int get_var_from_txt(char *text, double *zmienna)
{ char *bp1;
  double zmienna1;
  char *zmienna_t;
  char zmienna_tt[30];
  char *bp;

  zmienna_t = strpbrk(text, ".1234567890");
  if (zmienna_t != NULL)
    {
     strcpy(zmienna_tt, decimal_dot(zmienna_t));
     zmienna1 = strtod(zmienna_tt, &bp);
     *zmienna=zmienna1;
     return 1;
    }

  *zmienna=0;
  return 0;
}

////////////////////
int insert_profil_params(int x_origin, int y_origin)
{
    char typ_bloku [30] = "";
    double xx, yy;
    TEXT textg;
    int LengthT;
    char tekst_p[60];
    char tekst_p1[60];
    double skala_profilu0, poziom_pp0;
    int l_kr;
    double skala_profilu_x__=5.0;
    double poziom_pp__=55.0;
    double hektometr_0__=0;
    double typ_profilu__=0;
    char sk[61];
    int retval_no = 1 ;
    double buf_ret [1] ;


    //edycja wartosci bloku parametrycznego
    skala_label:
       ClearInfo0();
    strcpy(sk,"");
    if (!read_esc(sk,60,186))
    {
        ClearInfo0();
        return 0;
    }
    if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
    {
        ClearInfo0();
        return 0;
    }

    skala_profilu_x__ = buf_ret [0] ;
    if (skala_profilu_x__ < 0)
    {
        ErrList (204) ;
        my_sleep(2000);
        goto skala_label;
    }

    ClearInfo0();

    strcpy(sk,"");
    if (!read_esc(sk,60,187))
    {
        ClearInfo0();
        return 0;
    }
    if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
    {
        ClearInfo0();
        return 0;
    }

    poziom_pp__ = buf_ret [0] ;

    typ_label:
       ClearInfo0();
    strcpy(sk,"");
    if (!read_esc(sk,60,188))
    {
        ClearInfo0();
        return 0;
    }
    if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
    {
        ClearInfo0();
        return 0;
    }

    typ_profilu__ = buf_ret [0] ;
    if (((int)typ_profilu__ < 0) || ((int)typ_profilu__>2))
    {
        ErrList (205) ;
        my_sleep(2000);
        goto typ_label;
    }

    hektometr_label:
       ClearInfo0();
    strcpy(sk,"");
    if (!read_esc(sk,60,189))
    {
        ClearInfo0();
        return 0;
    }
    if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
    {
        ClearInfo0();
        return 0;
    }

    hektometr_0__ = buf_ret [0] ;
    if (hektometr_0__ < 0)
    {
        ErrList (206) ;
        my_sleep(2000);
        goto hektometr_label;
    }

    ClearInfo0();


    //dodanie bloku atrybutu

    memmove(&textg, &TextG, sizeof(TEXT));
    parametry_ogolne_tekstu(0, 4,koloryp.rzedne);

    tablica_0_1(typ_profilu__);

    if ((typ_profilu__==0) || (typ_profilu__==2)) TextG.x=x_origin-46; else TextG.x=x_origin-66;

    TextG.y=(float)y_origin+tablica_y[15]/*81*/;


    strcpy(typ_bloku, "$ATRYBUT");
    xx=x_origin;
    yy=y_origin;

    if (FALSE == add_block_with_type (xx, yy, typ_bloku, 0, B_EXPORT))
    {
        return 0;
    }

    yy=TextG.y+20.0;

    //symbol atrybutu
    yy-=2.5;
    TextG.y=(float)yy;
    TextG.kat=0;
    TextG.wysokosc=2.f;
    TextG.italics=0;
    TextG.width_factor=1.f;
    TextG.warstwa=Current_Layer;
    TextG.kolor=40; //7;
    TextG.czcionka=zmwym.czcionka; //0;
    TextG.typ=n_typ_atrybut;
    TextG.justowanie=j_do_lewej;
    TextG.ukryty=0;
    TextG.obiektt1 = 0;
    TextG.obiektt2 = O2BlockSpecial;
    TextG.blok=1;

    sprintf(tekst_p,"%s #%d",_Profil_,Current_Layer);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;


    TextG.kolor=7; //40;
    //kolejne wiersze
    TextG.typ=n_typ_komentarz;
    TextG.ukryty=1;

    yy-=2.5;
    TextG.y=yy;

    strcpy(tekst_p,"%S=");

    sprintf(tekst_p1,"%.1f",skala_profilu_x__);
    strcat(tekst_p,tekst_p1);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

    yy-=2.5;
    TextG.y=(float)yy;

    strcpy(tekst_p,"%PP=");
    sprintf(tekst_p1,"%.1f",poziom_pp__);
    strcat(tekst_p,tekst_p1);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

    yy-=2.5;
    TextG.y=(float)yy;

    strcpy(tekst_p,"%T=");
    sprintf(tekst_p1,"%d",(int)typ_profilu__);
    strcat(tekst_p,tekst_p1);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

    yy-=2.5;
    TextG.y=(float)yy;

    strcpy(tekst_p,"%h=");
    sprintf(tekst_p1,"%d",(int)hektometr_0__);
    strcat(tekst_p,tekst_p1);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;

    yy-=2.5;
    TextG.y=(float)yy;

    strcpy(tekst_p,"%H=");
    sprintf(tekst_p1,"%d",(int)hektometr_0__);
    strcat(tekst_p,tekst_p1);
    strcpy(&TextG.text[0],tekst_p);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt((BLOK*)dane, &TextG)==NULL) return 0;


    TextG.ukryty=0;
    ///////////

    /*zapamietanie starych danych*/
    skala_profilu0=get_skala_profilu_x();
    poziom_pp0=get_poziom_pp();

    /*zpisanie nowych danych*/
    l_kr=put_skala_profilu_x(skala_profilu_x__);

    l_kr=put_poziom_pp(poziom_pp__);

    //zapamietanie nowych danych o hektometrach
    l_kr=put_hektometr(hektometr_0__,0);
    l_kr=put_hektometr(hektometr_0__,1);

    //l_kr=generowanie_bloku_hektometrow(dane_prof.ru, dane_prof.odl, dane_prof.rt, 0, 1);  //to chyba nie jest konieczne

    memmove(&TextG, &textg, sizeof(TEXT));

    redraw();

    return 1;       /*opuszczenie funkcji ustawiania bloku*/
}

////////////////////
void change_profil_params(int opcja, int var_i, double var_d)
/*---------------------------------------------------------*/
//0-S, 1-PP, 2 -T, 3-h, 4-H
{ TEXT *Ta, *Tk, *t;
  TEXT Tn=Tdef;
  char *adp, *adk, *adp1, *adp2, *adp10, *adk10, *adp11;
  BLOK *ptrs_block, *ptrs_block1;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  int l_kr;
  double zmienna;
  BOOL no_more_text;
  LINIA *L;
  char tekst_p[60];
  char tekst_p1[60];
  int LengthT;

 adp=dane;
 adk=dane+dane_size;

 adp1=find_obj_in_layer(adp, adk, Otekst, n_typ_atrybut, O2BlockSpecial, Current_Layer, 0);
 if (adp1!=NULL)
  {
   Ta=(TEXT *)adp1;
   if (NULL != (ptrs_block = FIRSTB (adp1)))
   {
   if (ptrs_block->dlugosc_opisu_obiektu > 1)
    {
     ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
     if (ptrs_desc_bl->sz_type [0] != '\0')
      {
       if (strcmp(ptrs_desc_bl->sz_type,"$ATRYBUT")==0)  //prawidlowa nazwa bloku
        {
          adp10  = (char *)ptrs_block;
          adk10	= (char *)ptrs_block + sizeof(NAGLOWEK) + ptrs_block->n - 1;
          no_more_text=FALSE;
          while ((adp10 < adk10) && (no_more_text==FALSE))
           {
             adp11=find_obj_in_layer(adp10, adk10, Otekst, n_typ_komentarz, O2BlockSpecial, Current_Layer, 0);
             if (adp11!=NULL)
              {
                Tk=(TEXT *) adp11;
                //poszukiwanie zmiennych
                if (strncmp(Tk->text,"%S",2)==0)
                 { //definicja skali
                  if (opcja==0)
                   {
                    memmove(&Tn,Tk,sizeof(NAGLOWEK)+Tk->n);
                    rysuj_obiekt((char*)Tk,COPY_PUT,0);
                    strcpy(tekst_p,"%S=");
                    sprintf(tekst_p1,"%.1f",var_d);
                    strcat(tekst_p,tekst_p1);
                    strcpy(&Tn.text[0],tekst_p);
                    LengthT=(int)strlen(Tn.text);
                    Tn.dl=LengthT;
                    Tn.n=T18+Tn.dl;
                    t=(TEXT*)korekta_obiekt(Tk,&Tn);
                    if (NULL != (ptrs_block1 = FIRSTB ((char*)t)))
                      { ptrs_block1->obiektt2=O2BlockSpecial;
                      }
                    rysuj_obiekt((char*)&Tn,COPY_PUT,1);
                    return;
                   }
                 }
                 else
                  if (strncmp(Tk->text,"%PP",3)==0)
                   { //definicja poziomu porownawczego
                    if (opcja==1)
                    {
                     memmove(&Tn,(void *)Tk,sizeof(NAGLOWEK)+Tk->n);
                     rysuj_obiekt((char*)Tk,COPY_PUT,0);
                     strcpy(tekst_p,"%PP=");
                     sprintf(tekst_p1,"%.1f",var_d);
                     strcat(tekst_p,tekst_p1);
                     strcpy(&Tn.text[0],tekst_p);
                     LengthT=(int)strlen(Tn.text);
                     Tn.dl=LengthT;
                     Tn.n=T18+Tn.dl;
                     t=(TEXT*)korekta_obiekt(Tk,&Tn);
                     if (NULL != (ptrs_block1 = FIRSTB ((char*)t)))
                      { ptrs_block1->obiektt2=O2BlockSpecial;
                      }
                     rysuj_obiekt((char*)&Tn,COPY_PUT,1);
                     return;
                    }
                   }
                 else
                  if (strncmp(Tk->text,"%T",2)==0)
                   { //definicja typu
                    if (opcja==2)
                     {
                      memmove(&Tn,(void *)Tk,sizeof(NAGLOWEK)+Tk->n);
                      rysuj_obiekt((char*)Tk,COPY_PUT,0);
                      strcpy(tekst_p,"%T=");
                      sprintf(tekst_p1,"%d",var_i);
                      strcat(tekst_p,tekst_p1);
                      strcpy(&Tn.text[0],tekst_p);
                      LengthT=(int)strlen(Tn.text);
                      Tn.dl=LengthT;
                      Tn.n=T18+Tn.dl;
                      t=(TEXT*)korekta_obiekt(Tk,&Tn);
                      if (NULL != (ptrs_block1 = FIRSTB ((char*)t)))
                      { ptrs_block1->obiektt2=O2BlockSpecial;
                      }
                      rysuj_obiekt((char*)&Tn,COPY_PUT,1);
                      return;
                     }
                   }
                   else
                    if (strncmp(Tk->text,"%h",2)==0)
                     { //definicja pierwszego hetkometra
                      if (opcja==3)
                       {
                        memmove(&Tn,(void *)Tk,sizeof(NAGLOWEK)+Tk->n);
                        rysuj_obiekt((char*)Tk,COPY_PUT,0);
                        strcpy(tekst_p,"%h=");
                        sprintf(tekst_p1,"%d",var_i);
                        strcat(tekst_p,tekst_p1);
                        strcpy(&Tn.text[0],tekst_p);
                        LengthT=(int)strlen(Tn.text);
                        Tn.dl=LengthT;
                        Tn.n=T18+Tn.dl;
                        t=(TEXT*)korekta_obiekt(Tk,&Tn);
                        if (NULL != (ptrs_block1 = FIRSTB ((char*)t)))
                        { ptrs_block1->obiektt2=O2BlockSpecial;
                        }
                        rysuj_obiekt((char*)&Tn,COPY_PUT,1);
                        return;
                       }
                     }
                     else
                      if (strncmp(Tk->text,"%H",2)==0)
                       { //definicja ostatniego hektometra
                        if (opcja==4)
                         {
                          memmove(&Tn,(void *)Tk,sizeof(NAGLOWEK)+Tk->n);
                          rysuj_obiekt((char*)Tk,COPY_PUT,0);
                          strcpy(tekst_p,"%H=");
                          sprintf(tekst_p1,"%d",var_i);
                          strcat(tekst_p,tekst_p1);
                          strcpy(&Tn.text[0],tekst_p);
                          LengthT=(int)strlen(Tn.text);
                          Tn.dl=LengthT;
                          Tn.n=T18+Tn.dl;
                          t=(TEXT*)korekta_obiekt(Tk,&Tn);
                          if (NULL != (ptrs_block1 = FIRSTB ((char*)t)))
                          { ptrs_block1->obiektt2=O2BlockSpecial;
                          }
                          rysuj_obiekt((char*)&Tn,COPY_PUT,1);
                          return;
                         }
                       }
                adp10=adp11 + sizeof(NAGLOWEK) + Tk->n;
              }
               else no_more_text=TRUE;
           }
        }
      }
    }
   }
  }
}
///////////////////

////////////////////
int find_profil_params(void)
/*-------------------------*/
{
    TEXT *Ta, *Tk;
    char *adp, *adk, *adp1, *adp2, *adp10, *adk10, *adp11;
    BLOK *ptrs_block;
    T_Desc_Ex_Block *ptrs_desc_bl ;
    int l_kr;
    double zmienna;
    BOOL no_more_text;
    LINIA *L;
    int par_exist;
    unsigned char atrybut_flag;


    adp=dane;
    adk=dane+dane_size;
    par_exist=0;
    atrybut_flag=0;


    while ((par_exist==0) && (adp < adk))
    {
        adp1=find_obj_in_layer(adp, adk, Otekst, n_typ_atrybut, O2BlockSpecial, Current_Layer, 0);
        if (adp1!=NULL)
        {
            Ta=(TEXT *)adp1;
            if (NULL != (ptrs_block = FIRSTB (adp1)))
            {
                if (ptrs_block->dlugosc_opisu_obiektu > 1)
                {
                    ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
                    if (ptrs_desc_bl->sz_type [0] != '\0')
                    {
                        if (strcmp(ptrs_desc_bl->sz_type,"$ATRYBUT")==0)  //prawidlowa nazwa bloku
                        {
                            adp10  = (char *)ptrs_block;
                            adk10	= (char *)ptrs_block + sizeof(NAGLOWEK) + ptrs_block->n - 1;
                            no_more_text=FALSE;
                            while ((adp10 < adk10) && (no_more_text==FALSE))
                            {
                                adp11=find_obj_in_layer(adp10, adk10, Otekst, n_typ_komentarz, O2BlockSpecial, Current_Layer, 0);
                                if (adp11!=NULL)
                                {
                                    Tk=(TEXT *) adp11;
                                    //poszukiwanie zmiennych
                                    if (strncmp(Tk->text,"%S",2)==0)
                                    { //definicja skali
                                        if (get_var_from_txt(Tk->text, &zmienna)==1)
                                        {
                                            if (zmienna>0)
                                            {
                                                l_kr=put_skala_profilu_x(zmienna);
                                                atrybut_flag|=1;
                                            }
                                        }
                                    }
                                    else if (strncmp(Tk->text,"%PP",3)==0)
                                    { //definicja poziomu porownawczego
                                        if (get_var_from_txt(Tk->text, &zmienna)==1)
                                        {
                                            l_kr=put_poziom_pp(zmienna);
                                            atrybut_flag|=2;
                                        }
                                    }
                                    else if (strncmp(Tk->text,"%T",2)==0)
                                    { //definicja typu
                                        if (get_var_from_txt(Tk->text, &zmienna)==1)
                                        {
                                            l_kr=put_typ_profilu(zmienna);
                                            atrybut_flag|=4;
                                        }
                                    }
                                    else if (strncmp(Tk->text,"%h",2)==0)
                                    { //definicja pierwszego hetkometra
                                        if (get_var_from_txt(Tk->text, &zmienna)==1)
                                        {
                                            l_kr=put_hektometr(zmienna,0);
                                            atrybut_flag|=8;
                                        }
                                    }
                                    else if (strncmp(Tk->text,"%H",2)==0)
                                    { //definicja ostatniego hektometra
                                        if (get_var_from_txt(Tk->text, &zmienna)==1)
                                        {
                                            l_kr=put_hektometr(zmienna,1);
                                            atrybut_flag|=16;
                                        }
                                    }
                                    adp10=adp11 + sizeof(NAGLOWEK) + Tk->n;
                                }
                                else no_more_text=TRUE;
                            }
                        }
                    }
                }
            }
            // }
            if (atrybut_flag & 31)
            {
                par_exist=1;
                return par_exist;
            }
            adp=adk10;
        }
        else adp=adk;
    }
    return par_exist;
}
///////////////////

void Wybierz_Profil_Biezacy_(int warstwa)
{ double x_origin, y_origin;
  int k, i;
  int par_exist;
  int ins_p;
    
  //- ustalenie aktualnej warstwy
  //- odszukiwanie punktu T_Point w warstwie
  if (find_base_point_cur_layer (&x_origin, &y_origin)==0)
   {
     //- zerowanie ukladu lokalnego
     k=put_localx(0.0);
     k=put_localy(0.0);
   }
    else
     {
      //- definiowanie ukladu lokalnego
      k=put_localx(x_origin);
      k=put_localy(y_origin);
     }
  //- odszukiwanie bloku atrybutu w warstwie:
  //- odszukiwanie definicji skali: %S=.......    np. 5
  //- odszukanie definicja typu profilu: %T=......   np. 2
  //- odszukanie pierwszego hektometru: %h=.......  np. 0
  //- odszukanie ostatniego hektometru: %H=.......  np. 10
  //- przyjecie zmiennych globalnych put_skala_..., put_hektometr_.....
  //- wszystkie warstwy poza Current_Layer zostaja wylaczone z edycji

  for (i=0; i<Current_Layer; i++)
   {
     Layers [i].edit=0;
   }
  for (i=(Current_Layer+1); i<No_Layers; i++)
   {
     Layers [i].edit=0;
   }

  Layers [Current_Layer].on=1;
  Layers [Current_Layer].edit=1;
  Layers [Current_Layer].point=1;

  par_exist=find_profil_params();
  if (par_exist==0)  
  {
	  ErrList(203);
	  my_sleep(1500);
	  //generowanie bloku parametrycznego
	  ins_p=insert_profil_params(x_origin,y_origin);
	  if (ins_p==0) ErrList(207);

	  //return;  //TYMCZASOWO
  }

  //- redrow()
  //- funkcje aktualizacji profilu i globalnej zmiany profilu dzialaja
  //  tylko na obiektach Current Layer.
}


void Wybierz_Profil_Biezacy(void)
{ int cur_layer;
  //- wskazanie obiektu na rysunku
  //- odszukanie warstwy obiektu
  //- wykonanie procedury Wybierz_Profil_Biezacy_(warstwa)
  cur_layer=Current_Layer;
  select_no_edit_layers=1;
  Wez_warstwe();
  select_no_edit_layers=0;
//  Layers[Current_Layer].edit=1;
//  Layers[Current_Layer].on=1;
//  Layers[Current_Layer].point=1;
//  if (Current_Layer!=cur_layer)
  Wybierz_Profil_Biezacy_(Current_Layer);

  view_scale();
}


void Nowy_Profil(void)
{ int max_layer;
  BOOL save_original_layer;
  int current_layer__;
  double x_origin, y_origin;
  int l_kr;

  current_layer__=Current_Layer;
  //sprawdzany jest najwyzszy numer warstwy obiektow max_layer_number
  max_layer=find_max_layer();
  //ustalana jest i ew. definiowan nowa warstwa o nazwie "Profil #Nr" lub "Profile #Nr"
  //gdzie Nr jest numerem warstwy.
  if (max_layer<253)
   {
    add_profil_layer(max_layer+1);
   }
  //przyjmowana jest warstwa aktualna Nr
  Current_Layer=max_layer+1;

  //zapamietywane jest ustawienie "stalych warstw bloku" i ustawiane na "N"
  save_original_layer=options1.save_original_layer;
  options1.save_original_layer=0;

  //otwierany jest blok definicyjny new_prof.axx
  //w nim definiowana jest skala, poziom porownawczy, typ profilu, pierwszy
  //i  ostatni hm.
  ////////

  int k=ParBlok( FileName[6],FUN[6],6);

    if (!k) return;

  //importowana jest tabela pomiarowa, i uzupelniana danymi profilu
  //dodatkowo definiowany jest blok atrybutu uzupelniajacy tabele
  //blok wstawiany jest interaktywnie na warstwe Current_Layer

  if (find_base_point_cur_layer (&x_origin, &y_origin)==1)
    //co swiadczy o tym ze blok zostal wstawiony pomyslnie
   {
    //po wstawieniu, wykonywana jest procedura Wybierz_Profil_Biezacy_(warstwa)
    l_kr=put_znacznik_aplikacji(101);
    Wybierz_Profil_Biezacy_(Current_Layer);
   }
    else
     {
       Current_Layer=current_layer__;
       Wybierz_Profil_Biezacy_(Current_Layer);
       redraw();
     }

  options1.save_original_layer=save_original_layer;
}

void update_base_point(void)
{  int k;
    double x_origin, y_origin;
    //- ustalenie aktualnej warstwy
    //- odszukiwanie punktu T_Point w warstwie
    if (find_base_point_cur_layer (&x_origin, &y_origin)==0)
    {
        //- zerowanie ukladu lokalnego
        k=put_localx(0.0);
        k=put_localy(0.0);
    }
    else
    {
        //- definiowanie ukladu lokalnego
        k=put_localx(x_origin);
        k=put_localy(y_origin);
    }
}

void Profil(void)
{
  int n;
  int l_kr;
  double typ_profilu0;
  double skala_profilu0, skala_profilu;
  double poziom_pp0, poziom_pp;
  char sk [MaxTextLen] = "", *str ;
  double p_odl, d_odl;
  double first_hektometr, last_hektometr;
  double srednica, spadek;
  char material[60], nawierzchnia[60];
  int retval_no = 1 ;
  double buf_ret [1] ;
  int k;

  uaktualnij_pola_m_profil();

  if((n=getwsp1(&mProfil)-1)>=0)
  {
    //for each operation the actual base point position, so local 0,0 should be known.
    //the best way is to update that point always, to prevent the situation entire profile was moved
    update_base_point();

    add_zbior_2=0;
/*
    if (n==6)
    {
    //przywrocenie ostatniej danej i zainicjowanie usuniecia ostatniego bloku
    if (rename(PROFIL_TEMP_FILE_BAK,PROFIL_TEMP_FILE)!=0)
     {
      remove(PROFIL_TEMP_FILE);
      rename(PROFIL_TEMP_FILE_BAK,PROFIL_TEMP_FILE);
     }

    //tutaj mozna by rowniez przywrocic poprzednia wersja pomiar_1.bak
  //  if (rename (POMIAR_1_BAK,POMIAR_1_FILE)!=0)
  //   {
  //   remove (POMIAR_1_FILE);
  //    rename (POMIAR_1_BAK,POMIAR_1_FILE);
  //  }

    ErrList(149);
    }
*/
//    if (n==1)  usun_blok(dane, dane+dane_size);
    //sprawdzenie blokady
     if ((get_lock_prof_aktual()==1) && (n!=40) && (n!=41))
     {
      komunikat(233);
     }
     else if (n==6)
     {
      ;
     }
     else if (n==7)
      {
       l_kr=Get_Pomiar_Param();
      }
     else if (n==8)
      {
       ;
      }
     else if (n==9)
      {
       ;
      }
     else if (n==10)  //zmiana w przekroju
       {
         //ustalenie wartosci
         if (zakres_odleglosci(0,&p_odl,&d_odl)==TRUE)
          {
        //   l_kr=usuwanie_profilu();
           l_kr=przesuniecie_profilu(p_odl,d_odl);  
           l_kr=change_cavity_p(0);
           l_kr=usuwanie_profilu();  
           l_kr=sortowanie_odl();
           if (l_kr>0) l_kr=aktualizacja_profilu();
           redraw();
          }
          ClearInfo0();
       }
     else if ((n==11) || (n==13))
       {
        ;
       }
     else if (n==12)
        {
         //ustalenie wartosci
         //if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
         //spadek=0;
         l_kr=stale_zaglebienie(0);
         if (l_kr==1)
           {
            l_kr=przesuniecie_profilu(0,0);
            l_kr=change_cavity_p(0);
            l_kr=usuwanie_profilu();
            l_kr=sortowanie_odl();
            if (l_kr>0) l_kr=aktualizacja_profilu();
            redraw();
            ClearInfo0();
           }
        }
     else if (n==14)   //aktualizacja
        {
          l_kr=przesuniecie_profilu(0,0);
          l_kr=change_cavity_p(0);
          l_kr=usuwanie_profilu(); 
          l_kr=sortowanie_odl();
          if (l_kr>0) l_kr=aktualizacja_profilu();
          redraw();
        }
     else if (n == 32)  //zmiana opcji
         {
          l_kr=przesuniecie_profilu(0,0);
          l_kr=change_cavity_p(1);
          l_kr=usuwanie_profilu();
          l_kr=sortowanie_odl();
          if (l_kr>0) l_kr=aktualizacja_profilu();
          redraw();
         }
     else if (n == 33)   //zmiana opcji
          {
           l_kr=przesuniecie_profilu(0,0);
           l_kr=change_cavity_p(2);
           l_kr=usuwanie_profilu();
           l_kr=sortowanie_odl();
           if (l_kr>0) l_kr=aktualizacja_profilu();
           redraw();
          }
          //kanal projektowany
     else if ((n==34) || (n==35) || (n==36) || (n==37) || (n==38) || (n==39))//zmiana wyrownywania - dno,gora,os
          {
           l_kr=0;
           switch (n)
           { case 34: l_kr=licowanie_kanalu(0,0);
                      break;
             case 35: l_kr=licowanie_kanalu(0,1);
                      break;
             case 36: l_kr=licowanie_kanalu(0,2);
                      break;
             case 37: l_kr=licowanie_kanalu(1,0);
                      break;
             case 38: l_kr=licowanie_kanalu(1,1);
                      break;
             case 39: l_kr=licowanie_kanalu(1,2);
                      break;
             default: break;
           }
           if (l_kr==1)
            {
             l_kr=przesuniecie_profilu(0,0);
             l_kr=change_cavity_p(0);
             l_kr=usuwanie_profilu();
             l_kr=sortowanie_odl();
             if (l_kr>0) l_kr=aktualizacja_profilu();
             redraw();
            }
          }
     else if ( n==40 )
       {
        zablokuj_profil_on();
       }
     else if ( n==41 )
       {
        zablokuj_profil_off();
       }
     else if ( n==42 )
       {
        Nowy_Profil();
       }
     else if ( n==43 )
       {
        Wybierz_Profil_Biezacy();
       }
     else if (( n==15 ) || ( n==16 ) || ( n==17 ) || ( n==18 ))
       {
          ;
       }
     else if (n ==19)    //zmien nawierzchnie
        {
          //ustalenie wartosci
          if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
           {
            strcpy(nawierzchnia, "");
            if (read_esc(nawierzchnia,60,127))
              {
               zmien_nawierzchnie(p_odl, d_odl, nawierzchnia);
               redraw();
              }
           }
           ClearInfo0();
        }
     else if (n==20) // staly spadek_kp
        {
         //ustalenie wartosci
        // if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
         l_kr=staly_spadek(0);
         if (l_kr==1)
          {
           l_kr=przesuniecie_profilu(0,0);
           l_kr=change_cavity_p(0);
           l_kr=usuwanie_profilu();
           l_kr=sortowanie_odl();
           if (l_kr>0) l_kr=aktualizacja_profilu();
           redraw();
           ClearInfo0();
          }
        }
     else if (n==21) // staly spadek_ki
        {
         //ustalenie wartosci
       //  if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
         l_kr=staly_spadek(1);
         if (l_kr==1)
          {
           l_kr=przesuniecie_profilu(0,0);
           l_kr=change_cavity_p(0);
           l_kr=usuwanie_profilu();
           l_kr=sortowanie_odl();
           if (l_kr>0) l_kr=aktualizacja_profilu();
           redraw();
           ClearInfo0();
          }
        }
     else if (n==22) // staly spadek_tp
        {
         //ustalenie wartosci
       //  if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
         l_kr=staly_spadek(5);
         if (l_kr==1)
          {
           l_kr=przesuniecie_profilu(0,0);
           l_kr=change_cavity_p(0);
           l_kr=usuwanie_profilu();
           l_kr=sortowanie_odl();
           if (l_kr>0) l_kr=aktualizacja_profilu();
           redraw();
           ClearInfo0();
          }
        }
     else if (n==23) //zmien spadek_kp
          {
           //ustalenie wartosci
           //if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
           //spadek=0;
           l_kr=zmien_spadek(0);
           if (l_kr==1)
            {
             l_kr=przesuniecie_profilu(0,0);
             l_kr=change_cavity_p(0);
             l_kr=usuwanie_profilu();
             l_kr=sortowanie_odl();
             if (l_kr>0) l_kr=aktualizacja_profilu();
             redraw();
             ClearInfo0();
           }
          }
     else if (n==24) //zmien spadek_ki
          {
           //ustalenie wartosci
           //if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
           //spadek=0;
           l_kr=zmien_spadek(1);
           if (l_kr==1)
            {
             l_kr=przesuniecie_profilu(0,0);
             l_kr=change_cavity_p(0);
             l_kr=usuwanie_profilu();
             l_kr=sortowanie_odl();
             if (l_kr>0) l_kr=aktualizacja_profilu();
             redraw();
             ClearInfo0();
           }
          }
     else if (n==25) //zmien spadek_terenu_p
          {
           //ustalenie wartosci
           //if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
           //spadek=0;
           l_kr=zmien_spadek(5);
           if (l_kr==1)
            {
             l_kr=przesuniecie_profilu(0,0);
             l_kr=change_cavity_p(0);
             l_kr=usuwanie_profilu();
             l_kr=sortowanie_odl();
             if (l_kr>0) l_kr=aktualizacja_profilu();
             redraw();
             ClearInfo0();
           }
          }
     else if (n==26) //nadaj spadek_kp
          {
           //ustalenie wartosci
           //if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
           //spadek=0;
           l_kr=nadaj_spadek(0);
           if (l_kr==1)
            {
             l_kr=przesuniecie_profilu(0,0);
             l_kr=change_cavity_p(0);
             l_kr=usuwanie_profilu();
             l_kr=sortowanie_odl();
             if (l_kr>0) l_kr=aktualizacja_profilu();
             redraw();
             ClearInfo0();
           }
          }
     else if (n==27) //nadaj spadek_ki
          {
           //ustalenie wartosci
           //if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
           //spadek=0;
           l_kr=nadaj_spadek(1);
           if (l_kr==1)
            {
             l_kr=przesuniecie_profilu(0,0);
             l_kr=change_cavity_p(0);
             l_kr=usuwanie_profilu();
             l_kr=sortowanie_odl();
             if (l_kr>0) l_kr=aktualizacja_profilu();
             redraw();
             ClearInfo0();
           }
          }
     else if ( n==28 ) //zmien srednice kp
            {
             //ustalenie wartosci
             if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
              {
               srednica=0;
               strcpy(sk,"");
               if (!read_esc(sk,60,128))
                {
                  ClearInfo0();
                  return;
                }
              if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
               {
                ;
               }
               else
                {
                 srednica = buf_ret [0] ;
                 if (srednica < 0)
                  {
                   ErrList (30) ;
                  }
                   else
                    {
                     zmien_srednice(p_odl, d_odl, srednica, 0);
                     redraw();
                    }
                }
              }
              ClearInfo0();
            }
     else if ( n==29 ) //zmien srednice ki
            {
             //ustalenie wartosci
             if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
              {
               srednica=0;
               strcpy(sk,"");
               if (!read_esc(sk,60,129))
                {
                  ClearInfo0();
                  return;
                }
               if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
                {
                  ;
                }
                 else
                  {
                   srednica = buf_ret [0] ;
                   if (srednica < 0)
                    {
                     ErrList (30) ;
                    }
                     else
                      {
                       zmien_srednice(p_odl, d_odl, srednica, 1);
                       redraw();
                      }
                  }                            }
             ClearInfo0();
            }
     else if ( n==30 ) //zmien material kp
          {
           //ustalenie wartosci
           if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
             {
              strcpy(material,"");

              if (read_esc(material,60,126))
               {
                zmien_material(p_odl, d_odl, material, 0);
                redraw();
               }
             }
            ClearInfo0();
           }
     else if ( n==31 ) //zmien material ki
          {
           //ustalenie wartosci
           if (zakres_odleglosci(1,&p_odl,&d_odl)==TRUE)
            {
             strcpy(material,"");
             if (read_esc(material,60,126))
               {
                 zmien_material(p_odl, d_odl, material, 1);
                 redraw();
               }
            }
           ClearInfo0();
          }
     else
     {
      if (n == 44)
      {
      //usuniecie istniejacego tekstu lub bloku hektometrow
       ADP=dane;
       ADK=dane + dane_size;
       usun_blok (ADP, ADK) ;
       n = 5;
      }
      else if (n == 45)
      {
      //usuniecie istniejacego tekstu lub bloku hektometrow
       ADP=dane;
       ADK=dane + dane_size;
       usun_blok (ADP, ADK) ;
       n = 4;
      }

      typ_profilu0=get_typ_profilu();
      skala_profilu0=get_skala_profilu_x();
      poziom_pp0=get_poziom_pp();
      if (typ_profilu0==0) k=ParBlok( FileName[n],FUN[n],n);
       else if (typ_profilu0==1) k=ParBlok( FileName1[n],FUN[n],n);
        else /*2*/ k=ParBlok( FileName2[n],FUN[n],n);

       if (n == 4) //hektometry
       {
        skala_profilu=get_skala_profilu_x();

        first_hektometr=get_hektometr_p();
        last_hektometr=get_hektometr_k();

        change_profil_params(3, (int)first_hektometr, 0);
        change_profil_params(4, (int)last_hektometr, 0);

        usuwanie_osi_hektometrow();
        ADP=dane;
        ADK=dane + dane_size;
        usun_blok	(ADP, ADK) ;

        l_kr=generowanie_bloku_hektometrow(first_hektometr, skala_profilu, last_hektometr, 1, 1);
        redraw();
       }
       else if (n == 5) //jezeli skala profilu
       /*+++++++++++++*/
       {
         /*odczytanie nowych danych*/
         skala_profilu=get_skala_profilu_x();
         poziom_pp=get_poziom_pp();

         change_profil_params(0, 0, skala_profilu);
         change_profil_params(1, 0, poziom_pp);
         //lezeli nastapila zmiana poziomu porownawczego nalezy wykonac procedure:
         //change_pp(del_pp);
         //
         if (Check_if_Equal(poziom_pp, poziom_pp0)==FALSE)
          {
           l_kr=change_pp(poziom_pp - poziom_pp0);
           l_kr=przesuniecie_profilu(0,0);
           l_kr=change_cavity_p(0);
           l_kr=usuwanie_profilu();
           l_kr=sortowanie_odl();
           if (l_kr>0) l_kr=aktualizacja_profilu();
          }
         //jezeli nastapila zmiana skali, nalezy zaktualizowac profil:
         //
         if (Check_if_Equal(skala_profilu0, skala_profilu)==FALSE)
          {
           if ((Check_if_Equal(skala_profilu0, 0.0)==FALSE) &&
              (Check_if_Equal(skala_profilu, 0.0)==FALSE))
                l_kr=change_skala_profilu(skala_profilu0);


           usuwanie_osi_hektometrow();
           ADP=dane;
           ADK=dane + dane_size;
           usun_blok	(ADP, ADK) ;
           first_hektometr=get_hektometr_p();
           last_hektometr=get_hektometr_k();
           l_kr=generowanie_bloku_hektometrow(first_hektometr, skala_profilu, last_hektometr, 1, 1);
           l_kr=przesuniecie_profilu(0,0);
           l_kr=change_cavity_p(0);
           l_kr=usuwanie_profilu();
           l_kr=sortowanie_odl();
           if (l_kr>0) l_kr=aktualizacja_profilu();
          }
         redraw();
       }
     }
       /*+++++++++++++*/
    add_zbior_2=1;
  }
}

void regeneracja_profilu(void)
/*--------------------------*/
{ int l_kr;

 l_kr=change_cavity_p(0);
 l_kr=usuwanie_profilu();
 l_kr=sortowanie_odl();
 if (l_kr>0) l_kr=aktualizacja_profilu();
 redraw();
}

typedef enum
{
  Licowanie_Dnem = 0,
  Ukrywanie_RTin,
  Linia_k_i,
  Linia_k_p,
  Linia_terenu,
  Linia_ulicy,
  Dokladnosc_odleglosci,
  Dokladnosc_rzednych,
  Dokladnosc_rzednych_kp,
  Dokladnosc_rzednych_ki,
  Dokladnosc_rzednych_kolizji,
  Dokladnosc_rzednych_terenu,
  Dokladnosc_rzednych_terenu_p,
  Dokladnosc_rzednych_terenu_i,
  Dokladnosc_zaglebienia,
  Dokladnosc_spadkow,
  Srednica_punktu,
  Skala_bloku_hm_km,
  Separator_dziesietny,
}ID_Profile_Config;

static int no_config_profil= sizeof(config_profil) / sizeof(config_profil[0]);

static BOOL get_config_profil_ (T_Fstring key_name, T_Fstring ret_string)
/*---------------------------------------------------------------------*/
{
    int i, val_int;
    double val_double;
    T_Fstring keyname;

    //strupr (key_name);
    for (i = 0; i < no_config_profil; i++)
    {
        //strupr (&config_profil [i]);
        if (stricmp (key_name, config_profil [i]) == 0)
        {
            break;
        }
    }
    if (i >= no_config_profil)
    {
        return FALSE;
    }
    switch (i)
    {
    case Dokladnosc_odleglosci:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                odleglosc_prec = val_double;
                if (odleglosc_prec<0.001) odleglosc_poz=4;
                else if (odleglosc_prec<0.01) odleglosc_poz=3;
                else if (odleglosc_prec<0.1) odleglosc_poz=2;
                else if (odleglosc_prec<1) odleglosc_poz=1;
                else odleglosc_poz=0;

                tab_dokladnosc[1]=odleglosc_poz;
                tab_dokladnosc[12]=odleglosc_poz;
                tab_dokladnosc[13]=odleglosc_poz;
            }
        break;
    case Dokladnosc_rzednych:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                rzedna_p_prec = val_double;
                if (rzedna_p_prec<0.001) rzedna_p_poz=4;
                else if (rzedna_p_prec<0.01) rzedna_p_poz=3;
                else if (rzedna_p_prec<0.1) rzedna_p_poz=2;
                else if (rzedna_p_prec<1) rzedna_p_poz=1;
                else rzedna_p_poz=0;

                rzedna_i_prec=rzedna_p_prec;
                rzedna_i_poz=rzedna_p_poz;

                rzedna_k_prec=rzedna_p_prec;
                rzedna_k_poz=rzedna_p_poz;

                tab_dokladnosc[2]=rzedna_i_poz;
                tab_dokladnosc[4]=rzedna_p_poz;
            }
        break;
    case Dokladnosc_rzednych_kp:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                rzedna_p_prec = val_double;
                if (rzedna_p_prec<0.001) rzedna_p_poz=4;
                else if (rzedna_p_prec<0.01) rzedna_p_poz=3;
                else if (rzedna_p_prec<0.1) rzedna_p_poz=2;
                else if (rzedna_p_prec<1) rzedna_p_poz=1;
                else rzedna_p_poz=0;

                tab_dokladnosc[4]=rzedna_p_poz;
            }
        break;
    case Dokladnosc_rzednych_ki:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                rzedna_i_prec = val_double;
                if (rzedna_i_prec<0.001) rzedna_i_poz=4;
                else if (rzedna_i_prec<0.01) rzedna_i_poz=3;
                else if (rzedna_i_prec<0.1) rzedna_i_poz=2;
                else if (rzedna_i_prec<1) rzedna_i_poz=1;
                else rzedna_i_poz=0;

                tab_dokladnosc[2]=rzedna_i_poz;
            }
        break;
    case Dokladnosc_rzednych_kolizji:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                rzedna_k_prec = val_double;
                if (rzedna_k_prec<0.001) rzedna_k_poz=4;
                else if (rzedna_k_prec<0.01) rzedna_k_poz=3;
                else if (rzedna_k_prec<0.1) rzedna_k_poz=2;
                else if (rzedna_k_prec<1) rzedna_k_poz=1;
                else rzedna_k_poz=0;
            }
        break;
    case Dokladnosc_rzednych_terenu:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                rzedna_tp_prec = val_double;
                if (rzedna_tp_prec<0.001) rzedna_tp_poz=4;
                else if (rzedna_tp_prec<0.01) rzedna_tp_poz=3;
                else if (rzedna_tp_prec<0.1) rzedna_tp_poz=2;
                else if (rzedna_tp_prec<1) rzedna_tp_poz=1;
                else rzedna_tp_poz=0;

                rzedna_ti_prec=rzedna_tp_prec;
                rzedna_ti_poz=rzedna_tp_poz;
                zaglebienie_prec=rzedna_tp_prec;
                zaglebienie_poz=rzedna_tp_poz;

                tab_dokladnosc[6]=rzedna_ti_poz;
                tab_dokladnosc[7]=rzedna_tp_poz;
                tab_dokladnosc[14]=zaglebienie_poz;
                tab_dokladnosc[15]=zaglebienie_poz;
            }
        break;
    case Dokladnosc_rzednych_terenu_p:
            if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
                if (val_double > 0)
                {
                    rzedna_tp_prec = val_double;
                    if (rzedna_tp_prec<0.001) rzedna_tp_poz=4;
                    else if (rzedna_tp_prec<0.01) rzedna_tp_poz=3;
                    else if (rzedna_tp_prec<0.1) rzedna_tp_poz=2;
                    else if (rzedna_tp_prec<1) rzedna_tp_poz=1;
                    else rzedna_tp_poz=0;

                    tab_dokladnosc[7]=rzedna_tp_poz;
                }
        break;
    case Dokladnosc_rzednych_terenu_i:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                rzedna_ti_prec = val_double;
                if (rzedna_ti_prec<0.001) rzedna_ti_poz=4;
                else if (rzedna_ti_prec<0.01) rzedna_ti_poz=3;
                else if (rzedna_ti_prec<0.1) rzedna_ti_poz=2;
                else if (rzedna_ti_prec<1) rzedna_ti_poz=1;
                else rzedna_ti_poz=0;

                tab_dokladnosc[6]=rzedna_ti_poz;
            }
        break;
    case Dokladnosc_zaglebienia:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                zaglebienie_prec = val_double;
                if (zaglebienie_prec<0.001) zaglebienie_poz=4;
                else if (zaglebienie_prec<0.01) zaglebienie_poz=3;
                else if (zaglebienie_prec<0.1) zaglebienie_poz=2;
                else if (zaglebienie_prec<1) zaglebienie_poz=1;
                else zaglebienie_poz=0;

                tab_dokladnosc[14]=zaglebienie_poz;
                tab_dokladnosc[15]=zaglebienie_poz;
            }
        break;
    case Dokladnosc_spadkow:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                spadek_prec = val_double;
                if (spadek_prec<0.001) spadek_poz=4;
                else if (spadek_prec<0.01) spadek_poz=3;
                else if (spadek_prec<0.1) spadek_poz=2;
                else if (spadek_prec<1) spadek_poz=1;
                else spadek_poz=0;
                tab_dokladnosc[9]=spadek_poz;
                tab_dokladnosc[11]=spadek_poz;
            }
        break;
    case Srednica_punktu:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                srednica_punktu_zalamania = val_double;
            }
        break;
    case Skala_bloku_hm_km:
        if ( sscanf ( ret_string, "%lf", &val_double) == 1 )
            if (val_double > 0)
            {
                skala_bloku_hm_km = val_double;
            }
        break;
    case Separator_dziesietny:
            if ( sscanf ( ret_string, "%d", &val_int) == 1 )
                if ((val_int == 0) || (val_int == 1)) decimal_separator = val_int;
            break;
    case Licowanie_Dnem:
        if ( sscanf ( ret_string, "%d", &val_int) == 1 )
        {
            if ((val_int == 0) || (val_int == 1)) licowanie_dnem = val_int;
        }
        break;
    case Ukrywanie_RTin:
        if ( sscanf ( ret_string, "%d", &val_int) == 1 )
        {
            if ((val_int == 0) || (val_int == 1)) ukrywanie_rtin = val_int;
        }
        break;
    case Linia_k_i:
        if ( sscanf ( ret_string, "%d", &val_int) == 1 )
        {
            if ((val_int >= 0) && (val_int < 160))
            {
                typ_lin_profil[0] = val_int;
                typ_lin_profil[1] = val_int;
            }
        }
        break;
    case Linia_k_p:
        if ( sscanf ( ret_string, "%d", &val_int) == 1 )
        {
            if ((val_int >= 0) && (val_int < 160))
            {
                typ_lin_profil[2] = val_int;
                typ_lin_profil[3] = val_int;
            }
        }
        break;
    case Linia_terenu:
        if ( sscanf ( ret_string, "%d", &val_int) == 1 )
        {
            if ((val_int >= 0) && (val_int < 160)) typ_lin_profil[4] = val_int;
        }
        break;
    case Linia_ulicy:
        if ( sscanf ( ret_string, "%d", &val_int) == 1 )
        {
            if ((val_int >= 0) && (val_int < 160)) typ_lin_profil[5] = val_int;
        }
        break;
        /*
    case Tabela_p0_y:
        if ( sscanf ( ret_string, "%lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg\n",
         &tabela_p0_y [0], &tabela_p0_y [1], &tabela_p0_y [2],
         &tabela_p0_y [3], &tabela_p0_y [4], &tabela_p0_y [5],
         &tabela_p0_y [6], &tabela_p0_y [7], &tabela_p0_y [8],
         &tabela_p0_y [9], &tabela_p0_y [10], &tabela_p0_y [11],
         &tabela_p0_y [12],&tabela_p0_y [13], &tabela_p0_y [14],
         &tabela_p0_y [15],&tabela_p0_y [16], &tabela_p0_y [17],
        &tabela_p0_y [18], &tabela_p0_y [19]) >=1 )
        {
            ;
        }
        break;
    case Tabela_p1_y:
        if ( sscanf ( ret_string, "%lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg\n",
         &tabela_p1_y [0], &tabela_p1_y [1], &tabela_p1_y [2],
         &tabela_p1_y [3], &tabela_p1_y [4], &tabela_p1_y [5],
         &tabela_p1_y [6], &tabela_p1_y [7], &tabela_p1_y [8],
         &tabela_p1_y [9], &tabela_p1_y [10], &tabela_p1_y [11],
         &tabela_p1_y [12],&tabela_p1_y [13], &tabela_p1_y [14],
         &tabela_p1_y [15],&tabela_p1_y [16], &tabela_p1_y [17],
        &tabela_p1_y [18], &tabela_p1_y [19]) >=1 )
        {
            ;
        }
        break;
    case Tabela_p2_y:
        if ( sscanf ( ret_string, "%lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg\n",
         &tabela_p2_y [0], &tabela_p2_y [1], &tabela_p2_y [2],
         &tabela_p2_y [3], &tabela_p2_y [4], &tabela_p2_y [5],
         &tabela_p2_y [6], &tabela_p2_y [7], &tabela_p2_y [8],
         &tabela_p2_y [9], &tabela_p2_y [10], &tabela_p2_y [11],
         &tabela_p2_y [12],&tabela_p2_y [13], &tabela_p2_y [14],
         &tabela_p2_y [15],&tabela_p2_y [16], &tabela_p2_y [17],
        &tabela_p2_y [18], &tabela_p2_y [19]) >=1 )
        {
            ;
        }
        break;
        */
    default:
        break;
    }
    return TRUE;
}

static void get_config0_profil (void)
/*---------------------------------*/
{
  licowanie_dnem=0;
  ukrywanie_rtin=0;
  typ_lin_profil[0]=96;
  typ_lin_profil[1]=96;
  typ_lin_profil[2]=96;
  typ_lin_profil[3]=96;
  typ_lin_profil[4]=65;
  typ_lin_profil[5]=64;
}

//extern "C" { extern BOOL get_config_profil(void); }

BOOL get_config_profil (void)
/*-------------------------*/
{
  get_config0_profil ();
  return Get_Private_Profile_Strings ((T_Fstring)_PROFIL_, get_config_profil_);
}

int kreowanie_profilu(char *plik_profilu)
/*-------------------------------------*/
{
    //static void (*CUR)(int ,int);
    int gmaxy;
    Record_Profil record_profil;
    int log ;
    long akt_pos0;
    BOOL EOF_log;
    int n_siec, l_siec;
    Profil_Sieci profil_sieci[200];
    MyDane dane_profs0={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    ////MyDane dane_global;
    ////MyDane dane_prof0;
    int k;
    int max_length, n;
    int i, key, i_poz, hektometr_p, hektometr_k;
    long dane_size0;


    //  CUR = MVCUR ;
    //  MVCUR = nooph ;
    gmaxy=getmaxy();

    //otwarcie pliku
    log = open (plik_profilu, O_BINARY | O_RDONLY, S_IREAD) ;
    if (log == -1) //plik nie istnieje badz blad otwarcia
    {
        komunikat(156);
        int ret1=ask_question(1, "","OK","", "Error", 12, get_komunikat_ptr(156), 11, 1, 62);
        return 0;
    }

    //odszukanie wszystkich rekordow dla i==0, zapamietanie wartosci node w tablicy node_tab[];
    EOF_log=FALSE;
    n_siec=0;
    max_length=3;

    while (EOF_log==FALSE)
    {
        akt_pos0=lseek(log, 0, SEEK_CUR);

        if (read(log, &record_profil, sizeof(Record_Profil)) != sizeof(Record_Profil))
        {
            EOF_log=TRUE;
        }
        else
        {
            if ((record_profil.i==0) && (strlen(record_profil.node)>0) && (record_profil.node[0]!='\17'))
            {
                n_siec++;
                sprintf(profil_sieci[n_siec-1].nazwa,"%s [%s]",record_profil.node,record_profil.file_name);
                snprintf(profil_sieci[n_siec - 1].nazwa, sizeof(profil_sieci[n_siec - 1].nazwa), "%s [%s]", record_profil.node, record_profil.file_name);

                profil_sieci[n_siec-1].adr=akt_pos0;
                if (strlen(profil_sieci[n_siec-1].nazwa)>max_length)
                    max_length=(int)strlen(profil_sieci[n_siec-1].nazwa);

                //  pmListaSieci [n_siec - 1].txt = profil_sieci[n_siec-1].nazwa ;   win32todo
                strcpy(pmListaSieci [n_siec - 1].txt , profil_sieci[n_siec-1].nazwa) ;
                /////////
            }
        }
    }

    close(log);

    if (n_siec==0)
    {
        int ret1=ask_question(1, "","OK","", "Error", 12, get_komunikat_ptr(157), 11, 1, 62);
        return 0;
    }

    //wyswietlenie listy sieci
    mListaSieci.max=mListaSieci.maxw0=n_siec;
    if (gmaxy<400) mListaSieci.maxw=28;
    else if (gmaxy<500) mListaSieci.maxw=39;  //39
    else mListaSieci.maxw=49; //                  0;
    if (mListaSieci.max<mListaSieci.maxw) mListaSieci.maxw=0;

    mListaSieci.xdl=max_length;

    ////error_log:

    komunikat(159);
    //dopasowanie pozycji kursora manu

    if ((mListaSieci.poz + mListaSieci.foff) > (n_siec-1))
    {
        if (mListaSieci.foff>0)
        {
            mListaSieci.foff -= 1;
        }
        else if (mListaSieci.poz>0)
        {
            mListaSieci.poz -= 1;
        }
    }

    if ((n=getwsp1(&mListaSieci)-1)>=0)  /* n=-1 -> esc */
    {
        komunikat(0);
        //potwierdzenie
        int ret=ask_question(2, _No_,_Yes_,"", "", 12, get_komunikat_ptr(179), 11, 1, 61);
        if (ret!=1) return 0;
        key=_YES_;

        //akceptacja wybranej sieci
        if (key == _YES_ || key == _yes_)
        {
            i_poz = n ;
            if(i_poz >= n_siec)
            {
                //       MVCUR = CUR ;
                return 0;  //na wszelki wypadek
            }

            //ponowne otwarcie pliku
            log = open (plik_profilu, O_BINARY | O_RDONLY, S_IREAD) ;
            if (log == -1) //plik nie istnieje badz blad otwarcia
            {
                int ret1=ask_question(1, "","OK","", "Error", 12, get_komunikat_ptr(156), 11, 1, 62);
                return 0;
            }

            //odczyt pierwszego rekordu sieci
            lseek(log, profil_sieci[i_poz].adr , SEEK_SET);
            if (read(log, &record_profil, sizeof(Record_Profil)) != sizeof(Record_Profil))
            {
                int ret1=ask_question(1, "","OK","", "Error", 12, get_komunikat_ptr(158), 11, 1, 62);
                return 0;
            }

            //ustalenie danych globalnych
            //tymczasem wymagane jest wstepne ustalenie parametrow profilu
            dane_global.ru=get_skala_profilu_x();
            dane_global.rt=get_poziom_pp();
            dane_global.odl=get_typ_profilu();
            tablica_0_1(dane_global.odl);

            //ustalenie pierwszego i ostatniego hektometru
            hektometr_p=(int)(record_profil.odl_0 / 100.);
            hektometr_k=(int)(record_profil.odl_0 / 100.);
            put_hektometr(hektometr_p,0);  //poczatkowy
            put_hektometr(hektometr_k,1);  //koncowy

            //ewentualne ustalenie rzednej jezeli jest==0
            memmove(&dane_prof0, &record_profil.dane_prof, sizeof(MyDane));
            if (dane_prof0.rkp==0) dane_prof0.rkp=dane_global.rt;
            if ((record_profil.obiekt==0) && (dane_prof0.si3==0)) dane_prof0.si3=0.5;

            //generowanie pierwszego pomiaru/kolizji
            dane_size0=dane_size;
            switch (record_profil.obiekt)
            {
            case 0:  strcpy(comments_tmp[0],record_profil.node);
                k=Profil2H(&dane_prof0);  //
                break;
            case 1:  k=Profil3H(&dane_prof0);  //instalacje
                break;
            default:
                break;
            }
            if (dane_size>dane_size0)
            {
                ADP=dane; ADK=dane+(dane_size-dane_size0)-1;
                CUR_OFF(X,Y);
                zmien_atrybut(ADP,ADK,Anormalny,Ablok);
                transformacja_blok(ADP,ADK,get_localx(),get_localy(),0,0,Tprzesuw,0);
                blokzap(ADP,ADK,Ablok,COPY_PUT,1);
                zmien_atrybut(dane, dane + dane_size, Aoblok, Anormalny);
                zmien_atrybut(ADP,ADK,Ablok,Aoblok);
                CUR_ON(X,Y);
                Change = TRUE;
            }

            //odczyt kolejnych rekordow sieci
            l_siec=record_profil.l;
            if (l_siec>1)
            {
                for (i=1; i<l_siec; i++)
                {
                    if (read(log, &record_profil, sizeof(Record_Profil)) != sizeof(Record_Profil))
                    {

                        int ret1=ask_question(1, "","OK","", "Error", 12, get_komunikat_ptr(158), 11, 1, 62);
                        return 0;
                    }
                    //ewentualne ustalenie rzednej jezeli jest==0
                    memmove(&dane_prof0, &record_profil.dane_prof, sizeof(MyDane));
                    if (dane_prof0.rkp==0) dane_prof0.rkp=dane_global.rt;
                    if ((record_profil.obiekt==0) && (dane_prof0.si3==0)) dane_prof0.si3=0.5;

                    //generowanie kolejnych pomiarow/kolizji
                    dane_size0=dane_size;
                    switch (record_profil.obiekt)
                    {
                    case 0:  strcpy(comments_tmp[0],record_profil.node);
                        k=Profil1HD(&dane_prof0);  //kolejny pomiar
                        break;
                    case 1:  k=Profil3H(&dane_prof0);  //instalacje
                        break;
                    default:
                        break;
                    }
                    /////////////////
                    if (dane_size>dane_size0)
                    {
                        ADP=dane; ADK=dane+(dane_size-dane_size0)-1;
                        CUR_OFF(X,Y);
                        zmien_atrybut(ADP,ADK,Anormalny,Ablok);
                        transformacja_blok(ADP,ADK,get_localx(),get_localy(),0,0,Tprzesuw,0);
                        blokzap(ADP,ADK,Ablok,COPY_PUT,1);
                        zmien_atrybut(dane, dane + dane_size, Aoblok, Anormalny);
                        zmien_atrybut(ADP,ADK,Ablok,Aoblok);
                        CUR_ON(X,Y);
                        Change = TRUE;
                    }
                    /////////////////
                }
            }

            //zakonczenie profilu
            close (log) ;
        }
    }
    else
      komunikat(0);
    return 1;
}

int Kreowanie_Studni(char *plik_profilu)
{
    int res;
    int i, key, i_poz;
    int j;
    int log ;
    long akt_pos0;
    long dane_size00, dane_size000;
    int dynamic_block_clock;
    Record_Clock record_clock;
    int no_clocks;
    int gmaxy;
    BOOL EOF_log;
    int n_siec;
    int n;
    int max_length;
    //Record_Profil record_profil, record_profil0, record_profil1;
    Record_Profil record_profil_tab[1000];  //w pliku moze byc opisanych 1000 studni
    Profil_Sieci profil_sieci[200]; //w pliku moze byc 200 sieci
    int node_number;
    double dx, dy;
    int node_first;
    int node_last;
    int k;
    double zmwym_dokladnosc;
    int clock_no;
    BOOL found_exit;


    /////////////////////////
    //  CUR = MVCUR ;
    //  MVCUR = nooph ;
    gmaxy=getmaxy();

    //otwarcie pliku
    log = open (plik_profilu, O_BINARY | O_RDONLY, S_IREAD) ;
    if (log == -1) //plik nie istnieje badz blad otwarcia
    {
        komunikat(156);
        //    MVCUR = CUR ;
        return 0;
    }

    //odszukanie wszystkich rekordow dla i==0, zapamietanie wartosci node w tablicy node_tab[];
    EOF_log=FALSE;
    n_siec=0;
    max_length=3;
    node_number=0;

    while (EOF_log==FALSE)
    {
        akt_pos0=lseek(log, 0, SEEK_CUR);

        node_number++;

        if (read(log, &record_profil_tab[node_number-1], sizeof(Record_Profil)) != sizeof(Record_Profil))
        {
            EOF_log=TRUE;
        }
        else
        {
            if ((record_profil_tab[node_number-1].i==0) && (strlen(record_profil_tab[node_number-1].node)>0) && (record_profil_tab[node_number-1].node[0]!='\17'))
            {
                n_siec++;
                //sprintf(profil_sieci[n_siec-1].nazwa,"%s [%s]",record_profil_tab[node_number-1].node,record_profil_tab[node_number-1].file_name);
                snprintf(profil_sieci[n_siec - 1].nazwa, sizeof(profil_sieci[n_siec - 1].nazwa), "%s [%s]", record_profil_tab[node_number - 1].node, record_profil_tab[node_number - 1].file_name);

                profil_sieci[n_siec-1].adr=node_number-1;
                if (strlen(profil_sieci[n_siec-1].nazwa)>max_length)
                    max_length=(int)strlen(profil_sieci[n_siec-1].nazwa);

                //  pmListaSieci [n_siec - 1].txt = profil_sieci[n_siec-1].nazwa ;   win32todo
                strcpy(pmListaSieci [n_siec - 1].txt , profil_sieci[n_siec-1].nazwa) ;
                /////////
            }
        }
    }

    close(log);

    if (n_siec==0)
    {
        komunikat(157);
        //   MVCUR = CUR ;
        return 0;
    }

    //wyswietlenie listy sieci
    mListaSieci.max=mListaSieci.maxw0=n_siec;
    if (gmaxy<400) mListaSieci.maxw=28;
    else if (gmaxy<500) mListaSieci.maxw=39;  //39
    else mListaSieci.maxw=49; //                  0;
    if (mListaSieci.max<mListaSieci.maxw) mListaSieci.maxw=0;

    mListaSieci.xdl=max_length;

    ////error_log:

    komunikat(190);
    //dopasowanie pozycji kursora menu

    if ((mListaSieci.poz + mListaSieci.foff) > (n_siec-1))
    {
        if (mListaSieci.foff>0)
        {
            mListaSieci.foff -= 1;
        }
        else if (mListaSieci.poz>0)
        {
            mListaSieci.poz -= 1;
        }
    }

    if ((n=getwsp1(&mListaSieci)-1)>=0)  /* n=-1 -> esc */
    {

        komunikat(0);
        i_poz = n ;
        if(i_poz >= n_siec)
        {

            return 0;
        }

    }
    else
    {
        komunikat(0);
        return 0;
    }

    dane_size00=dane_size;

    ////GENEROWANIE DANYCH ZEGAROW

    zmwym_dokladnosc=zmwym.dokladnosc;
    zmwym.dokladnosc=1.0;
    WymInterClock(0);


    //odczytanie liczby wezlow w wybranej sieci czyli liczby zegarow
    no_clocks=record_profil_tab[profil_sieci[i_poz].adr].l;
    node_first=profil_sieci[i_poz].adr;
    node_last=node_first+no_clocks;
    /////////////////////////

    if (no_clocks==0)
    {
        ErrList(208);
        return 0;
    }

    for (j=node_first; j<node_last; j++)
    {


        for (i=0; i<16; i++)
        {
            record_clock.enter_clock[i].flag=-1;
            record_clock.enter_clock[i].kat=0;
            record_clock.enter_clock[i].srednica=0;
        }

        //generowanie rekordu danych zegara
        strcpy(record_clock.node_name,record_profil_tab[j].node);

        //pierwszy wezel
        if (j==node_first)
        {
            if (j<(node_last-1))  //a wiec nie jest wezlem ostatnim, a wiec jedynym
            {

                found_exit=FALSE;
                //teraz trzeba sprawdzic, czy gdzies jeszcze wystepuje ta studnia, tzn. w innej sieci.
                //jezeli wystepuje i nie jest wezlem pierwszym, to wtedy wogole pomijamy rysowanie tej studni tutaj
                //jezeli nie istnieje, rysujemy studnie ale nie rysujemy strzalki (flaga=3)
                for (k=0; k<node_number; k++)
                {
                    if ((k<node_first) || (k>=node_last))  //a wiec pomijamy wezly danej sieci
                    {
                        if ((TRUE == Check_if_Equal (record_profil_tab[k].x, record_profil_tab[j].x)) &&
                            (TRUE == Check_if_Equal (record_profil_tab[k].y, record_profil_tab[j].y)))
                        {  //te same wspolrzedne
                            //jezeli nie jest wezlem pierwszym, to wtedy wogole pomijamy rysowanie tej studni tutaj
                            if (record_profil_tab[k].i>0) found_exit=TRUE;
                        }
                    }
                }

                if (found_exit==FALSE)  //nie znaleziono odplywu
                {
                    //ustalenie danych doplywu
                    dx=record_profil_tab[j+1].x-record_profil_tab[j].x;
                    dy=record_profil_tab[j+1].y-record_profil_tab[j].y;
                    record_clock.enter_clock[0].kat=atan2(dy, dx);

                    record_clock.enter_clock[0].srednica=record_profil_tab[j].dane_prof.si3;
                    record_clock.enter_clock[0].flag=3;  //moze odplyw moze doplyw, trudno ustalic;

                    clock_no=1;

                    //poszukiwanie pozostalych doplywow
                    for (k=0; k<node_number; k++)
                    {
                        if ((k<node_first) || (k>=node_last))  //a wiec pomijamy wezly danej sieci
                        {
                            if ((TRUE == Check_if_Equal (record_profil_tab[k].x, record_profil_tab[j].x)) &&
                                (TRUE == Check_if_Equal (record_profil_tab[k].y, record_profil_tab[j].y)))
                            {  //te same wspolrzedne
                                //ustalenie danych doplywu
                                //sprawdzenie, czy wezel k jest nie jest wezlem ostatnim w sieci
                                if (record_profil_tab[k].i<(record_profil_tab[k].l-1))
                                {
                                    dx=record_profil_tab[k+1].x-record_profil_tab[j].x;
                                    dy=record_profil_tab[k+1].y-record_profil_tab[j].y;
                                    record_clock.enter_clock[clock_no].kat=atan2(dy, dx);

                                    //srednica sieci dochodzacej jest definiowana na podstawie kolejnego znacznika
                                    record_clock.enter_clock[clock_no].srednica=record_profil_tab[k+1].dane_prof.si3;
                                    record_clock.enter_clock[clock_no].flag=0;  //doplyw;

                                    clock_no++;
                                }
                            }
                        }
                    }
                }
                else if (no_clocks==1)
                {
                    ErrList(209);
                    return 0;
                }

            }
        }
        else if (j==(node_last-1)) //ostatni wezel ale nie pierwszy, a wiec ta galaz musi byc odplywem
        {
            //ustalenie danych odplywu
            dx=record_profil_tab[j-1].x-record_profil_tab[j].x;
            dy=record_profil_tab[j-1].y-record_profil_tab[j].y;
            record_clock.enter_clock[0].kat=atan2(dy, dx);

            record_clock.enter_clock[0].srednica=record_profil_tab[j-1].dane_prof.si3;
            record_clock.enter_clock[0].flag=1;  //odplyw;

            clock_no=1;
            //poszukiwanie pozostalych doplywow
            for (k=0; k<node_number; k++)
            {
                if ((k<node_first) || (k>=node_last))  //a wiec pomijamy wezly danej sieci
                {
                    if ((TRUE == Check_if_Equal (record_profil_tab[k].x, record_profil_tab[j].x)) &&
                        (TRUE == Check_if_Equal (record_profil_tab[k].y, record_profil_tab[j].y)))
                    {  //te same wspolrzedne
                        //ustalenie danych doplywu
                        //sprawdzenie, czy wezel k jest nie jest wezlem ostatnim w sieci
                        if (record_profil_tab[k].i<(record_profil_tab[k].l-1))
                        {
                            dx=record_profil_tab[k+1].x-record_profil_tab[j].x;
                            dy=record_profil_tab[k+1].y-record_profil_tab[j].y;
                            record_clock.enter_clock[clock_no].kat=atan2(dy, dx);

                            //srednica sieci dochodzacej jest definiowana na podstawie kolejnego znacznika
                            record_clock.enter_clock[clock_no].srednica=record_profil_tab[k+1].dane_prof.si3;
                            record_clock.enter_clock[clock_no].flag=0;  //doplyw;

                            clock_no++;
                        }
                    }
                }
            }


        }
        else  //posredni wezel
        {

            //ustalenie danych odplywu
            dx=record_profil_tab[j-1].x-record_profil_tab[j].x;
            dy=record_profil_tab[j-1].y-record_profil_tab[j].y;
            record_clock.enter_clock[0].kat=atan2(dy, dx);

            record_clock.enter_clock[0].srednica=record_profil_tab[j-1].dane_prof.si3;
            record_clock.enter_clock[0].flag=1;  //odplyw;

            //ustalenie danych doplywu
            dx=record_profil_tab[j+1].x-record_profil_tab[j].x;
            dy=record_profil_tab[j+1].y-record_profil_tab[j].y;
            record_clock.enter_clock[1].kat=atan2(dy, dx);

            record_clock.enter_clock[1].srednica=record_profil_tab[j].dane_prof.si3;
            record_clock.enter_clock[1].flag=0;  //doplyw;

            clock_no=2;
            //poszukiwanie pozostalych doplywow
            for (k=0; k<node_number; k++)
            {
                if ((k<node_first) || (k>=node_last))  //a wiec pomijamy wezly danej sieci
                {
                    if ((TRUE == Check_if_Equal (record_profil_tab[k].x, record_profil_tab[j].x)) &&
                        (TRUE == Check_if_Equal (record_profil_tab[k].y, record_profil_tab[j].y)))
                    {  //te same wspolrzedne
                        //ustalenie danych doplywu
                        //sprawdzenie, czy wezel k jest nie jest wezlem ostatnim w sieci
                        if (record_profil_tab[k].i<(record_profil_tab[k].l-1))
                        {
                            dx=record_profil_tab[k+1].x-record_profil_tab[j].x;
                            dy=record_profil_tab[k+1].y-record_profil_tab[j].y;
                            record_clock.enter_clock[clock_no].kat=atan2(dy, dx);

                            //srednica sieci dochodzacej jest definiowana na podstawie kolejnego znacznika
                            record_clock.enter_clock[clock_no].srednica=record_profil_tab[k+1].dane_prof.si3;
                            record_clock.enter_clock[clock_no].flag=0;  //doplyw;

                            clock_no++;
                        }
                    }
                }
            }

        }

        /*
        strcpy(record_clock.node_name,"S36");
        record_clock.enter_clock[0].flag=1;
        record_clock.enter_clock[0].kat=1.00;
        record_clock.enter_clock[0].srednica=200;

        record_clock.enter_clock[1].flag=0;
        record_clock.enter_clock[1].kat=2.00;
        record_clock.enter_clock[1].srednica=180;

        record_clock.enter_clock[2].flag=0;
        record_clock.enter_clock[2].kat=3.00;
        record_clock.enter_clock[2].srednica=160;

        record_clock.enter_clock[3].flag=0;
        record_clock.enter_clock[3].kat=0.00;
        record_clock.enter_clock[3].srednica=140;
        */

        //generowanie studni
        res=kreowanie_studni(&record_clock, (j-node_first)*30.0, 0.0);
    }

    zmwym.dokladnosc=zmwym_dokladnosc;
    WymInterClock(1);


    dane_size000=dane_size-dane_size00;
    //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
    //group_block(dane_size000);

    group_block_with_type( dane_size000,0,0,"Clocks", B_EXPORT);



    ADP=dane; ADK=dane+(dane_size-dane_size00)-1;
    CUR_OFF(X,Y);
    zmien_atrybut(ADP,ADK,Anormalny,Ablok);
    /* z przesuwaniem */

    dynamic_block_clock=1;

    if (dynamic_block_clock==1)
    {
        redcrMANHOLE (0) ;
        if(PrzesunB(0,0)==ESC)
        {
            zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
            usun_blok(ADP,ADK);
            CUR_ON(X,Y);
            redcrMANHOLE(1);
            return 0;
        }

        transformacja_blok(ADP,ADK,X-0,Y-0,0,0,Tprzesuw,0);

    }
    else
    {
        transformacja_blok(ADP,ADK,get_localx(),get_localy(),0,0,Tprzesuw,0);
    }
    blokzap(ADP,ADK,Ablok,COPY_PUT,1);
    zmien_atrybut(dane, dane + dane_size, Aoblok, Anormalny);
    zmien_atrybut(ADP,ADK,Ablok,Aoblok);
    CUR_ON(X,Y);
    redcrMANHOLE (1) ;
    Change = TRUE;
    return 1;
}

int kreowanie_studni(Record_Clock *record_clock, double del_x, double del_y)
{
    long dane_size00, dane_size000;
    int dynamic_block_clock;
    int LengthT;
    WIELOKAT St=Stdef;
    LINIA Linia=Ldef;
    LINIA Linia1=Ldef;
    LINIA Linia2=Ldef;
    double wym_clock_r, del_wym_clock_r;
    int wym_kata0;
    double kat1, kat2, x, y;
    char srednica_c[16];
    int srednica;
    double srednica_d;
    double zmwym_wysokosc;
    int normalize_text0;
    int no_channel;
    int no_kat;
    int main_channel_no;
    int i;
    double lines_lenght;
    BOOL draw_arrow;
    char beton[32];

    zmwym_wysokosc=zmwym.wysokosc;
    zmwym.wysokosc=2.0;
    WymInterF(1);
    WymInterClock(0);

    dane_size00=dane_size;
    //generowanie pojednyczego zegara na podstawie record_clock
    /*
    if (FALSE == add_block_with_type (0, 0, record_clock->node_name, 0, B_EXPORT))
    {
     return 0;
    }
    */
    draw_arrow=FALSE;

    //zliczamy ile jest kanalow wpadajacych do studni, tzn. wejsc dla ktorych flag>-1;
    //odszukanie kata poczatkowego - dla kanalu z flag=1
    no_channel=0;
    for (i=0; i<16; i++)
    {
        if (record_clock->enter_clock[i].flag>-1) no_channel++;
        if (record_clock->enter_clock[i].flag==1)
        {
            main_channel_no=i;
            draw_arrow=TRUE;
        }
        else if (record_clock->enter_clock[i].flag==3)
        {
            main_channel_no=i;
            draw_arrow=FALSE;

        }
    }

    //sortowanie katow wewnetrznych
    no_kat=0;
    for (i=0; i<no_channel; i++)
    {
        if (record_clock->enter_clock[i].flag==0)
        {
            katy[no_kat].kat=record_clock->enter_clock[i].kat-record_clock->enter_clock[main_channel_no].kat;
            katy[no_kat].no=i;
            no_kat++;
        }
    }
    qsort(katy,no_kat,sizeof(Katy),qsort_by_kat);

    //wyznaczenie poczatkowego promienia wymiarowania, oraz faktor zmiany promienia
    //wym_clock_r=3.0;
    wym_clock_r=5.0;

    //  if (no_kat>0) del_wym_clock_r=2.5;
    //  if (no_kat>1) del_wym_clock_r=2.0;
    //  if (no_kat>2) del_wym_clock_r=4/(no_kat);
    del_wym_clock_r=3.0;

    //wektor l=7,5 mm dla kanalu

    lines_lenght=(no_kat*3.0)+1;
    if (lines_lenght<7.5) lines_lenght=7.5;

    Linia.warstwa=Current_Layer;
    Linia.x1=(float)-lines_lenght;
    Linia.y1=0.0f;
    Linia.x2=(float)lines_lenght;
    Linia.y2=0.0f;
    Linia.kolor=7;
    Linia.typ=0;
    Linia.blok=0;
    if(dodaj_obiekt(NULL,&Linia)==NULL) return 0;

    Linia.x1=0.0f;
    Linia.y1=(float)-lines_lenght;
    Linia.x2=0.0f;
    Linia.y2=(float)lines_lenght;
    Linia.blok=0;
    if(dodaj_obiekt(NULL,&Linia)==NULL) return 0;

    Linia.x1=(float)-lines_lenght;
    Linia.y1=0.0f;
    Linia.x2=0.0f;
    Linia.y2=0.0f;
    Linia.kolor=7;
    Linia.typ=96;
    Linia.blok=0;
    if(dodaj_obiekt(NULL,&Linia)==NULL) return 0;

    //strzalka
    if (draw_arrow==TRUE)
    {
        St.warstwa=Current_Layer;
        St.blok=0;
        St.xy[0]=(float)-lines_lenght-2.5;
        St.xy[1]=0.0f;
        St.xy[2]=(float)-lines_lenght;
        St.xy[3]=0.5f;
        St.xy[4]=(float)-lines_lenght;
        St.xy[5]=-0.5f;
        St.lp = 6;
        St.blok=0;
        St.n = 8 + St.lp * sizeof (float) ;
        if(dodaj_obiekt(NULL,&St)==NULL) return 0;
    }

    //opis wezla
    TextG.kat= 0;
    TextG.wysokosc=3;
    TextG.warstwa=Current_Layer;
    TextG.italics=0;
    TextG.width_factor=1;
    TextG.czcionka=zmwym.czcionka; //0/*1*/;
    TextG.obiektt1 = 0;
    TextG.obiektt2 = O2BlockPline;
    TextG.blok=0;
    TextG.bold=1;
    TextG.typ=n_typ_normal;
    TextG.justowanie=j_srodkowo;
    TextG.kolor=7;
    TextG.ukryty=0;

    TextG.x=0.0f;
    TextG.y=(float)(lines_lenght+4.5);
    strcpy(&TextG.text[0],record_clock->node_name);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;

    if (dodaj_obiekt(NULL, &TextG)==NULL) return 0;

    //material

    TextG.x=0.0f;
    TextG.y=(float)(lines_lenght+8.5);
    strcpy(beton,_beton_);
    utf8Upper(beton);
    strcpy(&TextG.text[0],beton);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    if (dodaj_obiekt(NULL, &TextG)==NULL) return 0;

    //opis srednicy odplywu

    TextG.bold=0;
    TextG.justowanie=j_do_prawej;
    TextG.wysokosc=2.f;
    TextG.kat=0.f;
    TextG.x=(float)-lines_lenght;
    TextG.y=1.0f;
    TextG.typ=n_srednica_kanalu_p;
    srednica_d=record_clock->enter_clock[main_channel_no].srednica;
    if (FALSE == Check_if_Equal (srednica_d, 0.0)) sprintf(srednica_c,u8"%s%g",Fi,srednica_d);  //⌀ Φ   \316\246
    else strcpy(srednica_c,"");
    strcpy(&TextG.text[0],srednica_c);
    LengthT=(int)strlen(TextG.text);
    TextG.dl=LengthT;
    TextG.n=T18+TextG.dl;
    if (dodaj_obiekt(NULL, &TextG)==NULL) return 0;
    /*
        normalize_text0=normalize_text;
        normalize_text=1;
        obtekst_(dane,TextG.x,TextG.y,0.0,1.0);
        normalize_text=normalize_text0;
    */
    TextG.typ=n_typ_normal;

    wym_kata0=Get_wym_kata();
    Set_wym_kata(1);

    kat1=Pi;

    //tutaj rozpoczynamy petle opisujaca kolejne entrance
    for (i=0; i<no_kat; i++)
    {
        kat2=record_clock->enter_clock[katy[i].no].kat-record_clock->enter_clock[main_channel_no].kat+Pi;
        Linia.x1=0.0f;
        Linia.y1=0.0f;
        Linia.x2=(float)(lines_lenght*cos(kat2));
        Linia.y2=(float)(lines_lenght*sin(kat2));
        Linia.kolor=7;
        Linia.typ=96;
        Linia.blok=0;
        if(dodaj_obiekt(NULL,&Linia)==NULL) return 0;

        //opis srednicy

        TextG.bold=0;
        TextG.justowanie=j_do_lewej;
        TextG.wysokosc=2.f;
        TextG.kat=(float)kat2;
        TextG.x=Linia.x2;
        TextG.y=Linia.y2;
        TextG.typ=n_srednica_kanalu_p;
        srednica_d=record_clock->enter_clock[katy[i].no].srednica;
        if (FALSE == Check_if_Equal (srednica_d, 0.0)) sprintf(srednica_c,u8"%s%g",Fi,srednica_d);   //Φ   \316\246
        else strcpy(srednica_c,"");
        strcpy(&TextG.text[0],srednica_c);
        LengthT=(int)strlen(TextG.text);
        TextG.dl=LengthT;
        TextG.n=T18+TextG.dl;
        if (dodaj_obiekt(NULL, &TextG)==NULL) return 0;

        normalize_text0=normalize_text;
        normalize_text=1;
        obtekst_(dane,TextG.x,TextG.y,0.0,1.0);
        normalize_text=normalize_text0;

        TextG.typ=n_typ_normal;

        //wymiarowanie kata wewnetrznego, zgodnie z ruchem wskazowek zegara, pomiedzy kanalem a katem poczatkowym

        x=0.0;
        y=0.0;
        kat_w_clock (kat2, kat1, x, y, wym_clock_r);

        wym_clock_r+=del_wym_clock_r;
    }
    //koniec petli studni

    Set_wym_kata(wym_kata0);

    dane_size000=dane_size-dane_size00;
    //grupowanie w blok w zakresie dane, (dane_size-dane_size00)
    //group_block(dane_size000);

    group_block_with_type( dane_size000,0,0,record_clock->node_name, B_EXPORT);

    ADP=dane; ADK=dane+(dane_size-dane_size00)-1;
    //CUR_OFF(X,Y);
    zmien_atrybut(ADP,ADK,Anormalny,Ablok);
    /* z przesuwaniem */

    dynamic_block_clock=0;

    if (dynamic_block_clock==1)
    {
        if(PrzesunB(0,0)==ESC)
        {
            zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
            usun_blok(ADP,ADK);
            CUR_ON(X,Y);
            return 0;
        }
        transformacja_blok(ADP,ADK,X-0,Y-0,0,0,Tprzesuw,0);
    }
    else
    {
        transformacja_blok(ADP,ADK,del_x,del_y,0,0,Tprzesuw,0);
    }
    //blokzap(ADP,ADK,Ablok,COPY_PUT,1);
    zmien_atrybut(dane, dane + dane_size, Aoblok, Anormalny);
    zmien_atrybut(ADP,ADK,Ablok,Aoblok);
    //CUR_ON(X,Y);
    Change = TRUE;

    zmwym.wysokosc=zmwym_wysokosc;

    return 1;
}

#undef __A_PROFILE__