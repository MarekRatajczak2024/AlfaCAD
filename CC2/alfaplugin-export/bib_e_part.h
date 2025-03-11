//
// Created by marek on 2/5/24.
//

#ifndef ALFAPLUGIN_BIB_E_PART_H
#define ALFAPLUGIN_BIB_E_PART_H

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifdef BIT64
#define long_long long long
#else
#define long_long long
#endif

#define CMNU 15            /* tla menu*/
#define CMBR 14            /* ramki mn*/
#define CMTX 1             /* tekstu w mn*/

#define TADD 0x04          /*additional texts*/
#define ICONS 0x40         /*icons + tips instead of texts*/

#define MaxNumPoints 96
#define NumSplinePoints 256
#define MaxTextLen  254
#define MaxMultitextLen  2048

#define maxlen_w 64

#define MAX_NUMBER_OF_LAYERS 256

#define B3 4
#define T18   37

#define Bdef   {Anormalny,OdBLOK,0,0,0,1,0,0,  B3, '\0', 0, 0,'\0'}
#define Ldef  {Anormalny,Olinia,0,0,0,1,0,0,  20, 0,7,64, 0,0,0,0}
#define ldef  {Anormalny,Oluk,0,0,0,1,0,0, 24, 0,7,64, 0,0, 0,0,0, 0,0}
#define Tdef {Anormalny,Otekst,0,0,0,1,0,0, T18, 0,7,0,0,0,3,1,0,0,0,0,0,0,0,0,0,0,6,0,0,""}
#define PointDef {Anormalny,Opoint,0,0,0,1,0,0, 12, 0,7,0, 0,0}
#define Edef  {Anormalny,Oellipse,0,0,0,1,0,0, 24, 0,7,64,  255, 0,0,0,0,0}
#define FEdef  {Anormalny,Ofilledellipse,0,0,0,1,0,0, 24, 0,7,64,  255, 0,0,0,0,0}
#define eldef  {Anormalny,Oellipticalarc,0,0,0,1,0,0, 32, 0,7,64,  0,0, 0,0,0,0,0,0,0}
#define PCXdef {Anormalny,Opcx,0,0,0,1,0,0, T294, 0,0,0,0,0,0,0,0,0,0,0,0,0,41,1,0,0,0}
#define Kdef  {Anormalny,Okolo, 0,0,0,1,0,0, 16, 0,7,64,  0,0,0}
#define Odef  {Anormalny,Ookrag,0,0,0,1,0,0, 16, 0,7,64,  0,0,0}
#define Stdef {Anormalny,Owwielokat,0,0,0,1,0,0, 32, 0,7,0,0,0,0,0,0, 6,0,0,0,0,0,0}
#define S4def {Anormalny,Owwielokat,0,0,0,1,0,0, 40, 0,7,0,0,0,0,0,0, 8,0,0,0,0,0,0,0,0} /*czworokat*/
#define Splinedef {Anormalny,Ospline,0,0,0,1,0,0, 40, 0,7,0,0,4,0, 8,0,0,0,0,0,0,0,0} /*Bezier spline 4 points*/
#define SOLIDARC_N 60
#define sadef {Anormalny,Osolidarc,0,0,0,1,0,0, SOLIDARC_N /*52*/,  0,7,0, 0, 0, 0,0,0, 255,0,0,0,0,0,0,0,0,0,1,0,0,0,""};


#define Bnic             0
#define Blinia           1
#define Bluk             2
#define Bokrag           4
#define Bkolo            8
#define Bwwielokat      16
#define Btekst          32
#define Bpoint          64
#define Bspline        128
#define Bellipse       256
#define Bfilledellipse 512
#define Bellipticalarc 1024
#define Bsolidarc      2048
#define Bprzeciecie    4096
#define Bwymiarowanie  8192
#define Bblok         16384
#define Bpolygon      32768
#define Bpcx          65536

#define j_do_lewej 0
#define j_do_prawej 1
#define j_srodkowo 2
#define j_centralnie 3

#define ENTER 0x0d
#define SPC 0x20
#define TAB 0x09
#define ESC 0x1b  //27

#define Pi2 (2*3.1415926535897931)
#define Pi_ 3.1415926535897931

#define o_male       1.0E-5             /*0.0001*/
#define o_male_male  1.0E-7    /*0.000001  ????*/

enum OBIEKT { ONoInDimBlock = -3, ONoInBlock = -2, ONieOkreslony= - 1, Olinia = 1, Otekst = 2,
    Oluk = 3,Ookrag = 4,Okolo = 5, Owwielokat = 6, Opoint = 7, OdBLOK = 8, Osolidarc=9,
    Oellipticalarc = 10, Oellipse = 11, Ofilledellipse = 12, Ospline = 13,
    Opcx = 0, ONoObiect = 16, Okoniec=0xf};
/*ONoInBlock none in any block*/
/*OSimple, ONieOkreslony - never appears in object header */

enum Block_Type { B_DIM = 'W', B_DIM1 = '\01', B_DIM2 = '2', B_DIM3 = '3', B_EXPORT = 'E', B_PLINE = 'P', B_HATCH = 'H' , B_PROFIL = 'F',
    B_PPOMIAR = 'p', B_KPOMIAR = 'k', B_INSTALACJE = 'i', B_INSTALACJE_OPIS = 'j', B_INSTALACJE_K = 'K',
    B_ODWIERT_OLD = 'o', B_ODWIERT = 'O', B_HEKTOMETRY = 'm', B_SEKTORY = 's', B_WIRE = L'-', B_SHADOW = 13,
    B_ARM = 'A', B_NIEOKRESLONY = ' ', B_SIEC= 'N', B_VERTEX= 'V', B_032= '\032'} ;

enum Pline_Type { PL_OTHER = 0, PL_PLINE = 1, PL_POLYGON = 'P', PL_RECTANGLE = 'R', PL_HATCH = 'H',
    PL_ELLIPSE = 'E', PL_SKETCH = 'S', PL_TRACE = 'T', PL_CURVE = 'C',
    PL_ELLIPSE_FILL = 2, PL_ELLIPSE_ARC = 3, PL_PLYTA  = 'A', PL_OTWOR = 'B', PL_SIEC = 'n', PL_SHADOW = 13, PL_SOLIDARC=14} ;

enum ATRYBUT      { ANieOkreslony=-1,Anormalny = 0,Ausuniety = 1,Ablok = 2, Aoblok = 3, Abad = 4,
    Apoblok = 5, Appoblok = 6, Apppoblok = 7 };
/*Abad - it's damaged or unwanted object*/
/* ANieOkreslony - never appears in object header*/

typedef int BOOL;

#pragma pack(4)

#define POLE_TXT_MAX 64

typedef struct {
    char txt[POLE_TXT_MAX];
    unsigned short  wcod;  /*key code*/
    int iconno;
    struct tmn *menu;      /*address sub-menu*/
} POLE;

typedef struct tmn {
    int max;                   /*number of positions in menu*/
    int maxw;                  /*size of the menu ( 0 for not scrollable)*/
    int maxw0;                 /*original size of menu ( 0  for not scrollable )*/
    int xdl;                   /*horizontal size of menu in characters*/
    int xpcz, ypcz;            /*menu position (for non-dynamic), or dynamically modified*/
    unsigned short flags;       /*varied options*/
    char border,ink,paper;     /*colours in menu*/
    int poz,off;               /*cursor position and offset*/
    int foff;                  /*first position visible in menu*/
    int xtip;                  /* it's about tips*/
    int ytip;
    POLE (*pola)[];           /*array of menu positions descriptions*/
    void  *back;              /*background content*/
    struct tmn  *next;        /*next menu, active list*/
    void  *tip_back;          /*tip background content*/
} TMENU;

#pragma pack( 4 )

typedef struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ;
} NAGLOWEK;

typedef struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ; //   : 32;
    unsigned warstwa  : 8;
} NAGLOWEK_EXT;

typedef struct
{ double sin,cos,kat;
    double dx;
    double dy;
    double dl;
}PLINIA;

typedef struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ;  //32
    unsigned warstwa  : 8;
    unsigned kolor    : 8;   //255 - transparent - dla potrzeb stropu
    unsigned typ      : 8;
    float x1          ;  //32
    float y1          ;  //32
    float x2          ;  //32
    float y2          ;  //32
} LINIA;
typedef  LINIA  * LINIA_;

typedef
struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n; //    :32;
    unsigned warstwa  : 8;
    unsigned kolor    : 8;
    unsigned typ      : 8;
    unsigned shadowed : 1;
    unsigned rezerwa  : 7;
    float x,y,r;
    float kat1,kat2;
} LUK;
typedef  LUK  * LUK_;

typedef struct
{   unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;     //   _____ 2
    unsigned int n; //    :32;
    unsigned warstwa  : 8;
    unsigned kolor    : 8;
    unsigned pattern  : 1;  //0-color  1-patter
    //unsigned temp1    : 1;
    unsigned reversed    : 1;  //when point for kat2 is the first point with width1 and axis1, and point for kat1 is end point with width2 and axis2
    unsigned translucent : 1;
    unsigned drawpoly : 1;
    //unsigned pcx_solid: 1;
    unsigned shadowed: 1;
    unsigned empty_typ: 3;
    unsigned translucency : 8; //  _____  10
    float x,y,r;
    float kat1,kat2;
    float width1, width2;      //  _____  38
    float axis1, axis2;      //  _____  40
    short int scale;
    short int dx;
    short int angle;
    short int dy;              //  _____  56
    char patternname[256];     //  _____  58
} SOLIDARC;
typedef  SOLIDARC  * SOLIDARC_;

typedef
struct
{
    unsigned atrybut : 3;
    unsigned obiekt : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;  /* used in : dimension block*/
    unsigned widoczny : 1;
    unsigned przec : 1;  /* determining intersections, pull (zeroed after use)*/
    unsigned blok : 1;  /*the object is an element of a block of the OdBLOK type*/
    unsigned int n;   //32
    unsigned warstwa : 8;
    unsigned kolor : 8;
    unsigned czcionka : 7;
    unsigned bold : 1;
    float kat;
    float wysokosc;        /*height of font*/
    float width_factor;    /*width factor*/
    float x;
    float y;
    unsigned italics : 1;
    unsigned typ : 4;
    unsigned justowanie : 2;
    unsigned ukryty : 1;
    unsigned dl : 16;
    unsigned multiline : 1;
    unsigned underline : 1;
    unsigned encoding : 1;  //0 - old Alfa,  1-utf8
    unsigned spacing : 5;
    int width;
    int height;
    char text[MaxMultitextLen + 1];      ///MaxTextLen       /*LIMITS.H UCHAR_MAX 255*/
} TEXT;

typedef  TEXT  * PTR_TEXT;

typedef
struct
{ unsigned char Lkolor;
    unsigned char Tkolor;
    double wysokosc;         /*height*/
    double width_factor ;    /*width factor*/
    double dokladnosc;
    double linia_ob;
    unsigned strzalka         : 2 ;
    unsigned kierunek_t       : 1 ;
    unsigned b_add_line_const : 1 ;
    unsigned italics          : 1 ;
    unsigned czcionka         : 7 ;
    unsigned bold             : 1 ;
    unsigned collinear        : 1 ;
    unsigned format           : 2 ;  //0-Decimal, 1-Engineering, 2-Architectuaral, 3-Fractional
}ZMIENNE ;

typedef struct
{
    char *menu;
    char st[POLE_TXT_MAX];  //128
    unsigned short cod;
    int iconno;
} MENUPSTRUCT;

typedef struct
{
    char  manufacturer;   // always 10
    char  version;        // should be 5
    char  encoding;       // 1 for RLE
    char  bits_per_pixel; // usually 8, for 256-color
    short xmin, ymin;     // the width  is *usually*  (xmax-xmin+1)
    short xmax, ymax;     // the height is *usually*  (ymax-ymin+1)
    short horz_res, vert_res;     // DPI for printing
    unsigned char ega_palette[48];        // junk  :)
    char  reserved;
    char  num_color_planes;       // usually 1 (3 for 24-bit color)
    short bytes_per_line;         // MUST BE an EVEN number
    short palette_type;           // should be 1
    char  padding[58];            // junk

} PCXheader;

typedef
struct
{
    PCXheader header;
    char      dane;
}  REAL_PCX;

typedef
struct
{
    int wiersz;  //numer wiersz liczac od 0
    long adr;
    int count;
} STRIP;

typedef
struct
{
    STRIP strip[102];
}  STRIPS;

typedef
struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ; //   : 32;
    unsigned warstwa      : 8;
    unsigned kod_obiektu  : 6;
    unsigned v_flip : 1;
    unsigned h_flip : 1;
    unsigned inwersja     : 1;
    unsigned ignore_background : 1;
    unsigned set_foreground : 1;
    unsigned ignore_print_background : 1;
    unsigned markers      : 1;   //wprowadzono paski
    unsigned stripped     : 1;   //zaznaczono paski
    unsigned on_front   : 1; //
    unsigned buffered  : 1;  //buforowany w RAM
    unsigned background : 8;      //win32todo
    unsigned foreground : 8;      //win32todo
    float kat;
    float x, y ;         //punkt wstawienia w mm
    float dx, dy ;       //wielkosc punktu w mm
    int i_x, i_y ;       //wielkosc bitmapy w punktach
    unsigned char color_key[256]; //paleta kolorow    //win32todo
    unsigned int len_pcx;         //dlugosc bloku PCX
    char pcx[MaxTextLen];
    //na koncu bloku pcx mozna dopisac znaczniki pozycji
    //caly obraz pociety jest na 100 paskow strip[j] o wysokosci high/100
    //przy pierwszym wyswietlaniu w momencie osiagniecia paska strip[i].y
    //adres poczatku paska wpisywany jest do struktury strip[i].adr
    //struktura strip[100] sa w odleglosci: adr+(B_PCX *)adr->n-100*sizeof(STRIP)
} B_PCX;

typedef  B_PCX  B_PCX_;

typedef struct
{
    unsigned warstwa : 8;
    unsigned kod_obiektu : 8;
    float kat;
    float x, y;         //punkt wstawienia w mm
    float dx, dy;       //wielkosc punktu w mm
    int i_x, i_y;       //wielkosc bitmapy w punktach
    int image_ref;
    int display;        //1=show,
    int resolution_units;  //0=no units, 2=cm, 5=inch
    char file_name[MAXDIR];
} B_PCX_DEF;

typedef
struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ; //    :32;
    unsigned warstwa  : 8;
    unsigned kolor    : 8;
    unsigned typ      : 8;
    float x,y,r;
} OKRAG;
typedef  OKRAG  * OKRAG_;

typedef struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ; //    :32;
    unsigned warstwa  : 8;
    unsigned kolor    : 8;
    unsigned typ      : 8;
    unsigned transulcency:8;
    float x,y,rx,ry,angle;
} ELLIPSE;
typedef  ELLIPSE  * ELLIPSE_;

typedef struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ; //    :32;
    unsigned warstwa  : 8;
    unsigned kolor    : 8;
    unsigned typ      : 8;
    unsigned shadowed : 1;
    unsigned rezerwa  : 7 ;
    float x,y,rx,ry,angle,kat1,kat2;
} ELLIPTICALARC;
typedef  ELLIPTICALARC  * ELLIPTICALARC_;

typedef
struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ; //   : 32;
    unsigned warstwa  : 8;
    unsigned kolor    : 8;
    unsigned typ      : 8;     /*0 normalny, 1,2,3,4,5,6,7 junction, 8 pin, 9 pin g, 10 pin d, 11 pin s*/
    float x           ;
    float y           ;
} T_Point ;

#ifndef LINUX
#pragma pack( 4 )
#else
#pragma pack( )  //TO DO
#endif

typedef
struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ; //    : 32;
    unsigned warstwa  : 8;
    unsigned kolor    : 8;
    unsigned pattern  : 1;  //0-color  1-patter
    unsigned temp1    : 1;
    unsigned translucent : 1;
    unsigned drawpoly : 1;
    unsigned pcx_solid: 1; /*!!!!!*/
    unsigned empty_typ: 3;
    unsigned lp       : 16;
    float xy[MaxNumPoints] ;
} WIELOKAT;
typedef  WIELOKAT  * WIELOKAT_;

typedef
struct
{
    unsigned atrybut : 3;
    unsigned obiekt : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec : 1;
    unsigned blok : 1;
    unsigned int n; //    : 32;
    unsigned warstwa : 8;
    unsigned kolor : 8;
    unsigned typ : 8;
    unsigned temp1 : 1;  //OT3W_Move=0, OT3W_Drag=1
    unsigned shadowed : 1;
    unsigned npts : 5;  //qbic=3,quadratic=4, multipoints=5, max=32
    unsigned closed : 1;    //D3  //2D=0, 3D=1;
    unsigned lp : 16;  //can vary during drawing, ending with 8, but can be more for multipoints spline
    float xy[NumSplinePoints*2];  //just 8 points, maybe more for multipoints spline
} SPLINE, SPLINE3D;
typedef  SPLINE  * SPLINE_;



#ifndef LINUX
#pragma pack( 1 )
////#pragma pack()
#else
#pragma pack()
#endif

typedef
struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;
    unsigned int n ; //   : 32;
    char kod_obiektu ; // : 8;
    char rezerwa ; //     : 8;
    unsigned short dlugosc_opisu_obiektu :16 ;
    char opis_obiektu [1]   ;
} BLOK;
typedef  BLOK  * BLOK_;

#pragma pack( 4 )

typedef struct
{
    int flags ;           /*2 - dodano x, y*/
    float  x, y ;         /*punkt wstawienia*/
    int len ;             /*dlugosc tekstu opisu*/
    char sz_type [1] ;    /*tekst opisu zakonczony '\0'*/
} T_Desc_Ex_Block ;

typedef
struct
{ unsigned akt : 1;   /* mozna uzyc 'snap' */
    unsigned akw : 1;   /* aktywny widoczny */
    unsigned gor : 1;   /* selectq  wlaczony (widoczny gdy akt) */
    unsigned cur : 1;   /* sel jest elementem kursora (nie musi byc akt)*/
    unsigned wyj : 1;   /* snap wybrany */
    unsigned nr  : 4;   /* numer gor snapa */
    unsigned jump: 1;   /* po funkcji srodek, koniec,... skok o dx, dy */
    unsigned rez : 7;   /* wolne */
    unsigned short size ; // : 16;      /* rozmiar sel */
} SEL;

typedef struct
{ int       x, y ;
    //char      j;
    int       extend;
    int       lmax ;
    char      *st;
    double    *values ;
    int       val_no ;
    int       val_no_max ;
    int       mode ;
    char      *format ;
    int       (*ESTRF)(BOOL) ;
    char      * address;
} ESTR;

enum GRAPH_VALUE { GV_POINT = 0, GV_DIST, GV_VECTOR, GV_VECTOR_XY,
                GV_ANGLE, GV_NUMBER, GV_STRING, GV_STRING_D } ;


#pragma pack(1)
typedef struct
{
    unsigned on : 1;
    unsigned edit : 1;
    unsigned point : 1;
    unsigned grey : 1;
    unsigned bw : 1;
    unsigned reserve : 3;
    unsigned color : 8;
    unsigned line_type : 8;
    unsigned len_name : 8;
    char name[maxlen_w+2];
} LAYER;


#pragma pack()

#endif //ALFAPLUGIN_BIB_E_PART_H
