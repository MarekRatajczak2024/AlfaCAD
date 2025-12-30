#pragma warning( disable : 4103 )

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


#ifndef LINUX
#define Slash  '\\'
#define BSlash '\\'
#define SSlash "\\"
#define SDirUp "..\\"
#else
#define Slash  '/'
#define BSlash '/'
#define SSlash "/"
#define SDirUp "../"
#endif

#define n_typ_normal 0
#define n_etykieta_wew 1
#define n_etykieta_sklad 2
#define n_typ_symbol 3
#define n_typ_typ 4
#define n_typ_pin_name 5
#define n_typ1 5
#define n_typ_zasilanie 6
#define n_typ2 6
#define n_typ_port 7
#define n_typ_atrybut 8
#define n_typ_schemat 9
#define n_typ_plik 10
#define n_typ_siec 11
#define n_typ_komentarz 12
#define n_typ_zmienna 13
#define n_typ_symbol_drabinki 14
#define n_typ_opis_drabinki 15

//#define n_typ_normal 0
#define n_odleglosc 1           //*
#define n_rzedna_kanalu_i 2     //*
#define n_srednica_kanalu_i 3   //*
#define n_rzedna_kanalu_p 4     //*
#define n_srednica_kanalu_p 5   //*
#define n_rzedna_ulicy 6        //*
#define n_rzedna_terenu 7       //*
//#define n_typ_atrybut 8
#define n_spadek_kanalu_i 9     //*
//#define n_typ_plik 10
#define n_spadek_kanalu_p 11    //*
#define n_dlugosc_kanalu_i 12   //*
#define n_dlugosc_kanalu_p 13   //*
#define n_zaglebienie_kanalu_i 14 //*
#define n_zaglebienie_kanalu_p 15 //*

#define MaxNpts 92 //128 //max number of points for bspline

#include "axx.h"

#define j_do_lewej 0
#define j_do_prawej 1
#define j_srodkowo 2
#define j_centralnie 3

#define HIGH_SIZE 40000

#define MAX_IMAGES 32 //25

#define MAX_NUMBER_OF_WINDOWS 16

#define HATCH_OUTLINE_TYPE 225

static int MaxNoBlock=1000;
#define MaxNoAllBlocks 1000000
 
#define ESTR_Y 2

#define MAX_CLIENT_BITMAP 64
#define FIRST_CLIENT_BITMAP_NO 1000

#define V_EDGE_SIMPLE 21
#define V_EDGE_SIMPLE_INV 22
#define V_EDGE_FIXED 23
#define V_EDGE_FIXED_INV 24
#define V_EDGE_ARC_SIMPLE 25
#define V_EDGE_ARC_SIMPLE_INV 26
#define V_EDGE_ARC_FIXED 27
#define V_EDGE_ARC_FIXED_INV 28
//rolling edges
#define V_EDGE_ROLL 29
#define V_EDGE_ROLL_INV 30
#define V_EDGE_ARC_ROLL 31
#define V_EDGE_ARC_ROLL_INV 32

#define V_SLAB_PLATE 21
#define V_SLAB_SPACE 22
#define V_SLAB_WALL 23
#define V_SLAB_ZONE 24

#define V_FIXED_LINE 96

#pragma pack(4)

#ifdef FORWIN32
typedef struct 
{
       int da_year;
       int da_day;
       int da_mon;
	   int da_hour;
	   int da_min;
	   int da_sec;
} date;

#endif

typedef struct
{
	char FileName[256];
	char Comment[64];
	char ACADVER[32];
	double EXTMIN_X;
	double EXTMIN_Y;
	double EXTMAX_X;
	double EXTMAX_Y;
	double LIMMIN_X;
	double LIMMIN_Y;
	double LIMMAX_X;
	double LIMMAX_Y;
	double PEXTMIN_X;
	double PEXTMIN_Y;
	double PEXTMAX_X;
	double PEXTMAX_Y;
	double PLIMMIN_X;
	double PLIMMIN_Y;
	double PLIMMAX_X;
	double PLIMMAX_Y;
	double Jednostki_dxf;
	double SkalaF_dxf;
	int format_x;
	int format_y;
	double offset_x;
	double offset_y;
	int auto_size;
} DXF_Header;

typedef struct
{
	unsigned check :1;
	unsigned on    :1;
}  WARSTWY_ON_PCX;

#pragma pack(1)

typedef struct
   { unsigned draw :1;
     unsigned prefix :14;  //max 16384
     unsigned ramka :1;
     unsigned first_number :14; //max 16384
     unsigned styl : 2;  //0-gora, 1-dol, 2-dol i gora, 3-dol, gora i boki
     float dx;   //:32
   } SEKTORY;  //64 bits

typedef struct
{
	float dy;   //:32   //4
	float margin_width;  //:32   //4
	float frame_offset; //:32
	unsigned frame_color : 8;
	unsigned sector_color : 8;
	unsigned font : 8;
	unsigned frame_line : 3;
	unsigned sector_line : 3;
	unsigned reversed : 1;
	unsigned reserve : 1;
} SEKTORY_EXT; //2 x 64 bits

typedef struct
{
    unsigned node_element_color : 8;
    unsigned tension_color : 8;
    unsigned compression_color : 8;
    unsigned shear_color : 8;
    unsigned moment_color : 8;
    unsigned deformation_color : 8;
    unsigned reaction_color : 8;
    unsigned dynamic_color : 8;
} STATIC_COLORS;

typedef struct
{
    unsigned axial_stress_plus_color : 8;
    unsigned axial_stress_minus_color : 8;
    unsigned shear_stress_color : 8;
    unsigned shear_reserve_color : 8;
} STATIC_STRESS_COLORS;


#pragma pack(4)

typedef struct
   { double srednica;  //[mm]
     double zakotwienie; //* srednica, po kazdej stronie styku plyt
     double rozstaw;   //[cm]
     double ciezar; //[kg/m]
   } JOINING_NET_PARAMS;

typedef struct
   { char ch1;
     char ch2;
   } CHAR2_BUF;

typedef struct
   { double odl;
     double rz;
     double s;
   } MyDaneL;

typedef struct
   { int n;
     double odl; //odleglosci                     1
     double rki; //rzedne kanalu istn.            2
     double rkp;//rzedne kanalu proj.             3
     double ru; //rzedne ulicy                    4
     double rt;//rzedne terenu                    5
     double si1; //srednica kanalu istniejacego   6
     double si2; //material ki                    7
     double si3; //srednica kp                    8
     double oi1; //material kp                    9
     double oi2; //spadek ki                      10
     double oi3; //dlugosc ki                     11
     double sp1; //spadek kp                      12
     double sp2; //dlugosc kp                     13
     double sp3; //zaglebienie ki                 14
     double op1; //zaglebienie kp                 15
     double op2; //rodzaj nawierzchni             16
     double op3; //                               17
     double spi1; //  zarezerwowane dla 1 hektometru      18
     double spi2; //  zarezerwowane dla ostatniego        19
     double spi3; /* archiwalna wartosc odleglosci dla ki 20  */
     double opi1; /* archiwalna wartosc odleglosci dla kp 21  */
     double opi2; /* archiwalna wartosc odleglosci dla ru 22  */
     double opi3; /* archiwalna wartosc odleglosci dla rt 23  */
     double spp1; /* aktualna wysokosc generowania tabelki opisowej - zalezy
		      ona od wartosci maksymalnej najwyzszej rzednej + 7 cm   24 */
     double param1; //                              25
     
   } MyDane;
   
typedef struct
   { int n;
     double odl; //odleglosci                     1
     double rki; //rzedne kanalu istn.            2
     double rkp;//rzedne kanalu proj.             3
     double ru; //rzedne ulicy                    4
     double rt;//rzedne terenu                    5
     double si1; //srednica kanalu istniejacego   6
     double si2; //material ki                    7
     double si3; //srednica kp                    8
     double oi1; //material kp                    9
     double oi2; //spadek ki                      10
     double oi3; //dlugosc ki                     11
     double sp1; //spadek kp                      12
     double sp2; //dlugosc kp                     13
     double sp3; //                               14
     double op1; //                               15
     double op2; //                               16
     double op3; //                               17
     double spi1; //                              18
     double spi2; //                              19
     double spi3; /* archiwalna wartosc odleglosci dla ki 20  */
     double opi1; /* archiwalna wartosc odleglosci dla kp 21  */
     double opi2; /* archiwalna wartosc odleglosci dla ru 22  */
     double opi3; /* archiwalna wartosc odleglosci dla rt 23  */
     double spp1; /* aktualna wysokosc generowania tabelki opisowej - zalezy
		      ona od wartosci maksymalnej najwyzszej rzednej + 7 cm   24 */
     double param1; //                              25
   } MyDane1_1;   


typedef int                 BOOL;
typedef char			    * T_Fstring;

typedef T_Fstring           * T_LpFstring;
typedef long                T_PixelTVal ;  //long_long  03-07-2022
typedef float               T_Float, * T_PTR_Float ;

typedef struct
{
  int flag;  //flaga kontrolna, domyslnie 0
  int l;  //liczba rekordow sieci;
  int i;  //numer rekordu sieci, numerowany od 0
  char file_name[30];  //nazwa pliku z ktorego pochodzi
  char rys_name[60];   //nazwa rysunku z ktorego pochodzi
  char obiekt;          //albo wezel 0, albo kolizja
                        //1-KS, 2-KO, 3-KD,  4-W, 5-G, 6-Cok, 7-Co
                        //8-e,  9-E, 10-kT, 11-T
  char node[15];
  double odl_0;         //poczatkowa odleglosc pierwszego wezla
  double x;
  double y;
  MyDane dane_prof;  //to bardzo przyszlosciowe, zawiera interfejs ze wszystkimi parametrami
} Record_Profil;

typedef struct
{
  int flag;
  double kat;
  double srednica;
}  Enter_Clock;

typedef struct
{
  char material[16];
  char node_name[16];
  Enter_Clock enter_clock[16];
} Record_Clock;

#ifdef UKRAINIAN
#define POLE_TXT_MAX 64
#else
#define POLE_TXT_MAX 64
#endif

/*struktury danych definiujace okna*/
typedef struct {
#ifdef FORWIN32
         char txt[POLE_TXT_MAX]; //128 or 64 due to void set_list_string (void) if o_listst.c
#else
	     char *txt;             /*adres lancucha tresci pozycji*/ 
#endif
         unsigned short  wcod;                 /*kod znakowy pozycji*/
		 int iconno; 
	     struct tmn *menu;      /*adres opisu pod-menu*/
               } POLE;

typedef struct {
    char *txt;             /*adres lancucha tresci pozycji*/
    unsigned short  wcod;                 /*kod znakowy pozycji*/
    int iconno;
    struct tmn *menu;      /*adres opisu pod-menu*/
} PPOLE;

#ifdef ARM64
typedef struct tmn {
             int max;                   /*liczba pozycji menu*/
             int maxw;                  /*rozmiar okna ( 0 dla nie scrolowanych )*/
			 int maxw0;               /*oryginalny rozmiar okna ( 0 dla nie scrolowanych )*/
		     int xdl;                   /*wymiary menu*/
		     int xpcz, ypcz;            /*pozycja menu*/
			 unsigned int flags;         //unsigned short unsigned char       /*rozne opcje*/

			 char border;
			 char ink;
	         int paper;     /*kolory w menu*/

             int poz,off;               /*pozycja kursora i offset*/
             int foff;                  /*pierwsza pozycja w okienku*/
			 int xtip;
			 int ytip;
	     POLE (*pola)[];        /*tablica opisow pozycji w menu*/
             void  *back;            /*zawartosc tla*/
             struct tmn  *next;      /*kolejne menu aktywne-lista*/
			 void  *tip_back;  /*zawartosc tla tip*/
             //unsigned char flags2;
	   } TMENU;

typedef struct ptmn {
            int max;                   /*liczba pozycji menu*/
            int maxw;                  /*rozmiar okna ( 0 dla nie scrolowanych )*/
            int maxw0;               /*oryginalny rozmiar okna ( 0 dla nie scrolowanych )*/
            int xdl;                   /*wymiary menu*/
            int xpcz, ypcz;            /*pozycja menu*/
            unsigned int flags;         //unsigned short unsigned char       /*rozne opcje*/
            char border;
            char ink;
            int paper;     /*kolory w menu*/

            int poz,off;               /*pozycja kursora i offset*/
            int foff;                  /*pierwsza pozycja w okienku*/
            int xtip;
            int ytip;
            PPOLE (*pola)[];        /*tablica opisow pozycji w menu*/
            void  *back;            /*zawartosc tla*/
            struct tmn  *next;      /*kolejne menu aktywne-lista*/
            void  *tip_back;  /*zawartosc tla tip*/
} PTMENU;

#else
typedef struct tmn {
             int max;                   /*liczba pozycji menu*/
             int maxw;                  /*rozmiar okna ( 0 dla nie scrolowanych )*/
			 int maxw0;               /*oryginalny rozmiar okna ( 0 dla nie scrolowanych )*/
	         int xdl;                   /*wymiary menu*/
	         int xpcz, ypcz;            /*pozycja menu*/

             unsigned short flags;      //unsigned char /*rozne opcje*/
	         char border,ink,paper;     /*kolory w menu*/
             int poz,off;               /*pozycja kursora i offset*/
             int foff;                  /*pierwsza pozycja w okienku*/
			 int xtip;
			 int ytip;
	     POLE (*pola)[];        /*tablica opisow pozycji w menu*/
             void  *back;            /*zawartosc tla*/
             struct tmn  *next;      /*kolejne menu aktywne-lista*/
			 void  *tip_back;  /*zawartosc tla tip*/
             //unsigned char flags2;
	   } TMENU;

typedef struct ptmn {
            int max;                   /*liczba pozycji menu*/
            int maxw;                  /*rozmiar okna ( 0 dla nie scrolowanych )*/
            int maxw0;               /*oryginalny rozmiar okna ( 0 dla nie scrolowanych )*/
            int xdl;                   /*wymiary menu*/
            int xpcz, ypcz;            /*pozycja menu*/
            unsigned short flags;         //unsigned char       /*rozne opcje*/
            char border,ink,paper;     /*kolory w menu*/
            int poz,off;               /*pozycja kursora i offset*/
            int foff;                  /*pierwsza pozycja w okienku*/
            int xtip;
            int ytip;
            PPOLE (*pola)[];        /*tablica opisow pozycji w menu*/
            void  *back;            /*zawartosc tla*/
            struct tmn  *next;      /*kolejne menu aktywne-lista*/
            void  *tip_back;  /*zawartosc tla tip*/
} PTMENU;

#endif

typedef struct {
	int x1[MAX_IMAGES];
	int y1[MAX_IMAGES];
	int x2[MAX_IMAGES];
	int y2[MAX_IMAGES];
} FORTIPS_MAP;


#define PAPER 0            /* tla obrazu*/
#define INK 3              /* rysowania*/
#define FLCLR 1            /* wypelniania*/
#define CMNU 15            /* tla menu*/
#define CMBR 14            /* ramki mn*/
#define CMTX 1             /* tekstu w mn*/
/* flagi okien dopuszczalne w polu flags TMENU */
#define NOCLOSE 0x01       /* nigdy nie zamykane */
#define NOWCLOSE 0x02      /* nie zamykane po wyborze */
#define NVERT 0x80         /* okno poziome */
#define ICONS 0x40         /* icons */
#define TADD 0x04          /*teksty dodatkowe*/
#define NOMENU 0x01        /*dlg,listbox, hlp*/
#define FIXED 0x100          /*teksty dodatkowe*/
#define SVMAX 181 /*166*/ /*132*/          /*najwieksza wartosc bajtu pomocniczego klawiszy funkcyjnych*/
//#define MP_SIZE 12         /*wielkosc pola menu poziomego*/
#define MaxNumPoints 96 //12
#define NumSplinePoints 288 //287 //260 //256 //8   //due to animation
#define MaxNumPoints_Z 48
#define MaxSizeObiekt 4160 //2080 //300 /*tekst moze byc dlugi*/
//#define ED_INF_HEIGHT 12
#define MaxTextLen  254  //512
#define MaxMultitextLen  4096 //2048
#define MaxPinCount 2000
#define max_dodatkowe_linie 500 /*wystarczy tyle bo linie dodawane sa sekwencyjnie*/
#define BKCOLOR         0
#define TEXT_WIDTH_FACTOR   (1/1.06042)
#define o_male       1.0E-5             /*0.0001*/
#define o_male_male  1.0E-7    /*0.000001  ????*/

/*------------------------------------------------------------------------*/
enum OBIEKT { ONoInDimBlock = -3, ONoInBlock = -2, ONieOkreslony= - 1, Olinia = 1, Otekst = 2,
	      Oluk = 3,Ookrag = 4,Okolo = 5, Owwielokat = 6, Opoint = 7, OdBLOK = 8, Osolidarc=9,
         Oellipticalarc = 10, Oellipse = 11, Ofilledellipse = 12, Ospline = 13,
         Ovector = 14, Opcx = 0,
         ONoObiect = 16, Okoniec=0xf};
         /*ONoInBlock oznacza obiekty nie zawarte w zadnym bloku*/
	      /*OSimple, ONieOkreslony - nigdy nie wystapi w naglowku
                        obiektu */

/*------------------------------------------------------------------------*/
enum ATRYBUT      { ANieOkreslony=-1,Anormalny = 0,Ausuniety = 1,Ablok = 2, Aoblok = 3, Abad = 4, 
                    Apoblok = 5, Appoblok = 6, Apppoblok = 7 };
                        /*Abad - oznacza oznacza obiekt uszkodzony lub zbedny*/
			/* ANieOkreslony - nigdy nie wystapi w naglowku obiektu*/
			
/*------------------------------------------------------------------------*/
enum OBIEKTT1Ll  { Guma=0,Utwierdzony1,Utwierdzony2,Sztywny};
                                /*parametr uzywany przy przesuwaniu bloku,
                                atrybut Sztywny w zasadzie nie powinien
                                byc zmieniany*/
enum OBIEKTT1W { SIMPW = 0, InterS1_3, InterS2_4 };
                                /*dla czworokata : krawedzie nie przecinaja sie,
						   1 i 3 krawedz przecina sie,
                                                   2 i 4 krawedz przecina sie*/

enum OBIEKTT1BL { OB1NOCHANGE = 0 , OB1CHANGE_SCALE= 1, OB1CHANGE_DRAG = 2,
                  OB1CHANGE_BREAK = 3 } ;  /* dla bloku */
        /*OB1NOCHANGE - elementy bloku zostaly co najwyzej :
	  przesuniete, obrocone, przeskalowane (skx = sky)*/
        /*OB1CHANGE_SCALE- elementy bloku zostaly co najwyzej :
          przeskalowane (skx != sky)*/
        /*OB1CHANGE_DRAG - elementy bloku zostaly co najwyzej :
          ciagniete*/
        /*OB1CHANGE_BREAK - elementy bloku zostaly co najwyzej :
          wyciete, uciete, wydluzone, fazowane, profilowane
          (dyskusyjna jest sprawa ostatniego elementu galezi)*/

/*----------------------------------------------------------------------------------------------------------------------------------*/
enum OBIEKTT2        { O2NieOkreslony=-1, O2NoBlockS,O2BlockDim, O2BlockPline, O2BlockAparat, O2BlockDXF, O2BlockSpecial, O2BlockHatch25 /*, O2BlockHatch50*/} ;
enum OBIEKTT2_PLATE  { O2ROLL_EDGE=4, O2FREE_EDGE=5, O2HINGED_EDGE=6, O2FIXED_EDGE=7};

enum OBIEKTT3_PLATE  {O3REGULAR_EDGE=0, O3INVERTED_EDGE=1};
/*----------------------------------------------------------------------------------------------------------------------------------*/
enum OBIEKTT3Wym  { O3NoWymRoz,O3WymRoz}; /*wykorzystywane tylko w bloku wymiarowania*/
					  /*O3WymRoz   - linia rozszerzenia*/
                                          /*O3NoWymRoz - pozostale obiekty bloku wymiarowania*/

enum OBIEKTT3Pline {O3LeftDir = 0, RightDir = 1} ; /*wykorzystywane tylko w bloku polilinii*/
                                          /*O3LeftDir - kierunek obiektu przeciwny do kierunku plilinii*/
                                          /*RightDir -  kierunek obiektu zgodny z kierunkiem plilinii*/
enum OBIEKTT3HatchLine {O3Bold = 0, O3Thin = 1} ; /*wykorzystywane w liniach wypelnienia TERMAT*/
                                                  /*O2BlockHatch25    O2BlockHatch50   O2BlockAparat*/
                                            /*  0 -      36.5 cm          36.5 cm           18.0 cm*/
                                            /*  1 -      24.0 cm           8.0 cm           18.0 cm   - tymczasowo*/
enum OBIEKTT3SpecialTexts {O3NormalText = 0, O3SpecialText = 1};
                                            /*wykorzystywane w PROFILACH*/
                                            /*dla tekstow specjalnych*/
                /*O3SpecialText, n_rzedna_terenu - rzedna terenu w odwiertach*/
enum OBIEKTT3DnoOs {O3Dno = 0, O3Os = 1};
                /*wykorzystywane w PROFILACH dla oznaczania punktow*/

enum OBIEKTT3IstProj {O3Teren_p = 0, O3Teren_i = 1};
                /*wykorzystywane w PROFILACH dla oznaczania tekstow n_zaglebienie_kanalu_i */

enum OBIEKTT3HatchSolid { O3HatchLine = 0, O3HatchSolid = 1 };
/*-------------------------------------------------------------------------------------------------*/

enum SOLID_TEMP1   { OT3W_Move=0, OT3W_Drag } ;
                                /*parametry uzywany przy ciagnieciu wielokatow i Bezier*/

enum BLOK_ELEM   { NoElemBlok = 0, ElemBlok = 1};

enum BLOKM { NoBlokSel=0,BlokSel,NoBlokWid};
enum TRANSFORMACJA { Tprzesuw = 1, Tobrot = 2, Tskala = 4, Tmirror = 8, Tpolar = 16, TprzesuwZ = 32, TobrotXZ = 64, TobrotYZ = 128, TobrotPCX = 256 } ; /*w bib_blok uzyto sumy logicznej  Tobrot | Tskala*/

enum Block_Type { B_DIM = 'W', B_DIM1 = '\01', B_DIM2 = '2', B_DIM3 = '3', B_EXPORT = 'E', B_PLINE = 'P', B_HATCH = 'H' , B_PROFIL = 'F',
                  B_PPOMIAR = 'p', B_KPOMIAR = 'k', B_INSTALACJE = 'i', B_INSTALACJE_OPIS = 'j', B_INSTALACJE_K = 'K',
                  B_ODWIERT_OLD = 'o', B_ODWIERT = 'O', B_HEKTOMETRY = 'm', B_SEKTORY = 's', B_WIRE = L'-', B_SHADOW = 13,
                  B_ARM = 'A', B_NIEOKRESLONY = ' ', B_SIEC= 'N', B_VERTEX= 'V', B_032= '\032', B_GRAPH = 'G'} ;

enum Pline_Type { PL_OTHER = 0, PL_PLINE = 1, PL_POLYGON = 'P', PL_RECTANGLE = 'R', PL_HATCH = 'H',
                PL_ELLIPSE = 'E', PL_SKETCH = 'S', PL_TRACE = 'T', PL_CURVE = 'C',
                PL_ELLIPSE_FILL = 2, PL_ELLIPSE_ARC = 3, PL_PLATE  = 'A', PL_SHIELD  = 'A', PL_HOLE = 'B', PL_WALL = 'W', PL_ZONE = 'Z', PL_SIEC = 'n', PL_SHADOW = 13, PL_SOLIDARC=14} ;
                


#pragma pack(1)

typedef
  struct
     { unsigned atrybut  : 3;
       unsigned obiekt   : 4;
       unsigned obiektt1 : 2;
       unsigned obiektt2 : 3;
       unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie*/
       unsigned widoczny : 1;
       unsigned przec    : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
       unsigned blok     : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
       unsigned n        : 16;   
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned czcionka : 3;
       unsigned rezerwa1 : 1;
       float kat;
       float wysokosc;
       float x;
	   float y;        
       unsigned char dl;   
       char text [MaxTextLen + 1] ;             /*LIMITS.H UCHAR_MAX 255*/
    } TEXT_OLD1_0 ;


typedef
  struct
     { unsigned atrybut  : 3;
       unsigned obiekt   : 4;
       unsigned obiektt1 : 2;
       unsigned obiektt2 : 3;
       unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie*/
       unsigned widoczny : 1;
       unsigned przec    : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
       unsigned blok     : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
       unsigned n        : 16 ;
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned czcionka : 4;
       float kat ;
       float wysokosc ;        /*wysokosc znaku*/
       float width_factor ;    /*szerokosc znaku*/
       float x;
	   float y;
/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
/*       unsigned vertical    : 1 ; tekst pionowy*/
/*       unsigned justify           : 3 ;*/
       unsigned italics     : 1 ;
       unsigned reserve     : 7 ;
       unsigned char dl;
       char text [MaxTextLen + 1] ;             /*LIMITS.H UCHAR_MAX 255*/
    } TEXT_OLD1_1;

  typedef
  struct
     { 
       float kat ;
       float wysokosc ;        /*wysokosc znaku*/
       float width_factor ;    /*szerokosc znaku*/
       float x;
	   float y;
    } TEXT_OLD1_1_F;

  typedef
  struct
     { 
       unsigned italics     : 1 ;
       unsigned reserve     : 7 ;
    } TEXT_OLD1_1_FF;

  typedef
  struct
     { 
       unsigned char dl;
    } TEXT_OLD1_1_FF1;

  typedef
  struct
     { 
       char text [MaxTextLen + 1] ;             /*LIMITS.H UCHAR_MAX 255*/
    } TEXT_OLD1_1_FFF;
    
    
#pragma pack(4)
 
typedef    
 struct
     { unsigned atrybut  : 3;
       unsigned obiekt   : 4;
       unsigned obiektt1 : 2;
       unsigned obiektt2 : 3;
       unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie*/
       unsigned widoczny : 1;
       unsigned przec    : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
       unsigned blok     : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
       unsigned int n ; //   : 32 /*16*/;
       unsigned warstwa  : 8;
       unsigned kolor    : 8 /*4*/;
       unsigned czcionka : 7 /*4*/;
       unsigned bold     : 1;
       float kat ;
       float wysokosc ;        /*wysokosc znaku*/
       float width_factor ;    /*szerokosc znaku*/
       float x;
	   float y;
/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
/*       unsigned vertical    : 1 ; tekst pionowy*/
/*       unsigned justify           : 3 ;*/
       unsigned italics     : 1 ;
      /* unsigned reserve     : 7 ; */
       unsigned typ         : 4 ;
       unsigned justowanie  : 2 ;
       unsigned ukryty      : 1 ;
	   unsigned dl			: 16; //// 8;
	   unsigned multiline   : 1 ;
	   unsigned underline	: 1;
	   unsigned encoding	: 1;  //0 - Alfa  1-utf8
	   unsigned spacing     : 5 ;
       char text [MaxMultitextLen + 1];      ///MaxTextLen       /*LIMITS.H UCHAR_MAX 255*/
	  } TEXT3;

typedef  TEXT3  * PTR_TEXT3 ;


typedef
struct
{
	unsigned atrybut : 3;
	unsigned obiekt : 4;
	unsigned obiektt1 : 2;
	unsigned obiektt2 : 3;
	unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie*/
	unsigned widoczny : 1;
	unsigned przec : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
	unsigned blok : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
	unsigned int n; //   : 32 /*16*/;
	unsigned warstwa : 8;
	unsigned kolor : 8 /*4*/;
	unsigned czcionka : 7 /*4*/;
	unsigned bold : 1;
	float kat;
	float wysokosc;        /*wysokosc znaku*/
	float width_factor;    /*szerokosc znaku*/
	float x;
	float y;
	/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
	/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
	/*       unsigned vertical    : 1 ; tekst pionowy*/
	/*       unsigned justify           : 3 ;*/
	unsigned italics : 1;
	/* unsigned reserve     : 7 ; */
	unsigned typ : 4;
	unsigned justowanie : 2;
	unsigned ukryty : 1;
	unsigned dl : 16; //// 8;
	unsigned multiline : 1;
	unsigned underline : 1;
	unsigned encoding : 1;  //0 - Alfa  1-utf8
	unsigned spacing : 5;
	int width;
	int height;
	char text[MaxMultitextLen + 1];      ///MaxTextLen       /*LIMITS.H UCHAR_MAX 255*/
} TEXT;

typedef  TEXT  * PTR_TEXT;

#pragma pack(4)

typedef    
 struct
     { unsigned atrybut  : 3;
       unsigned obiekt   : 4;
       unsigned obiektt1 : 2;
       unsigned obiektt2 : 3;
       unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie*/
       unsigned widoczny : 1;
       unsigned przec    : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
       unsigned blok     : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
       unsigned int n ; //   : 32 /*16*/;
       unsigned warstwa  : 8;
       unsigned kolor    : 8 /*4*/;
       unsigned czcionka : 7 /*4*/;
       unsigned bold     : 1;
       float kat ;
       float wysokosc ;        /*wysokosc znaku*/
       float width_factor ;    /*szerokosc znaku*/
       float x,y,z;
       float cosxz;            /*cos kata w plaszczyznie xz*/
       float cosyz;            /*cos kata w plaszczyznie yz*/
/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
/*       unsigned vertical    : 1 ; tekst pionowy*/
/*       unsigned justify           : 3 ;*/
       unsigned italics     : 1 ;
      /* unsigned reserve     : 7 ; */
       unsigned typ         : 4 ;
       unsigned justowanie  : 2 ;
       unsigned ukryty      : 1 ;
       unsigned dl          : 16 ;
	   unsigned multiline	: 1;
	   unsigned underline	: 1;
	   unsigned aux			: 1;
	   unsigned spacing		: 5;
       char text [MaxTextLen + 1] ;             /*LIMITS.H UCHAR_MAX 255*/
    } TEXT3D3;


typedef  TEXT3D3  * PTR_TEXT3D3 ;

typedef
struct
{
	unsigned atrybut : 3;
	unsigned obiekt : 4;
	unsigned obiektt1 : 2;
	unsigned obiektt2 : 3;
	unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie*/
	unsigned widoczny : 1;
	unsigned przec : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
	unsigned blok : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
	unsigned int n; //   : 32 /*16*/;
	unsigned warstwa : 8;
	unsigned kolor : 8 /*4*/;
	unsigned czcionka : 7 /*4*/;
	unsigned bold : 1;
	float kat;
	float wysokosc;        /*wysokosc znaku*/
	float width_factor;    /*szerokosc znaku*/
	float x, y, z;
	float cosxz;            /*cos kata w plaszczyznie xz*/
	float cosyz;            /*cos kata w plaszczyznie yz*/
/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
/*       unsigned vertical    : 1 ; tekst pionowy*/
/*       unsigned justify           : 3 ;*/
	unsigned italics : 1;
	/* unsigned reserve     : 7 ; */
	unsigned typ : 4;
	unsigned justowanie : 2;
	unsigned ukryty : 1;
	unsigned dl : 16;
	unsigned multiline : 1;
	unsigned underline : 1;
	unsigned aux : 1;
	unsigned spacing : 5;
	int width;
	int height;
	char text[MaxTextLen + 1];             /*LIMITS.H UCHAR_MAX 255*/
} TEXT3D;


typedef  TEXT3D  * PTR_TEXT3D;

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

#pragma pack( 1 )
typedef
  struct
     { unsigned atrybut  : 3;
       unsigned obiekt   : 4;
       unsigned obiektt1 : 2;
       unsigned obiektt2 : 3;
       unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie */
       unsigned widoczny : 1;
       unsigned przec    : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
       unsigned blok     : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
       unsigned n        : 16;
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned czcionka : 4;   //////6
       float kat ;
       float wysokosc ;        /*wysokosc znaku*/
       float width ;           /*szerokosc znaku*/
       float x,y;
/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
/*       unsigned vertical    : 1 ; tekst pionowy*/
/*       unsigned prop        : 1 ; tekst proporcjonalny*/
       unsigned italic      : 1 ;
       unsigned reserve     : 7 ;
       unsigned dl          : 8 ;
    } TEXT_NAG1_1,  * PTR_TEXT_NAG1_1;

typedef
  struct
     { float kat ;
       float wysokosc ;        /*wysokosc znaku*/
       float width ;           /*szerokosc znaku*/
       float x,y;
/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
/*       unsigned vertical    : 1 ; tekst pionowy*/
/*       unsigned prop        : 1 ; tekst proporcjonalny*/
       unsigned italic      : 1 ;
       unsigned reserve     : 7 ;
       unsigned dl          : 8 ;         /////22
    } TEXT_NAGEND1_1,  * PTR_TEXT_NAGEND1_1;
    
#pragma pack( 1 )

typedef
  struct
     { unsigned atrybut  : 3;
       unsigned obiekt   : 4;
       unsigned obiektt1 : 2;
       unsigned obiektt2 : 3;
       unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie */
       unsigned widoczny : 1;
       unsigned przec    : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
       unsigned blok     : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
       unsigned int n; //    : 32;
       unsigned warstwa  : 8;
       unsigned kolor    : 8;
       unsigned czcionka : 7;
       unsigned bold     : 1;
       float kat ;
       float wysokosc ;        /*wysokosc znaku*/
       float width ;           /*szerokosc znaku*/
       float x,y;
/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
/*       unsigned vertical    : 1 ; tekst pionowy*/
/*       unsigned prop        : 1 ; tekst proporcjonalny*/
       unsigned italic      : 1 ;
/*       unsigned reserve     : 7 ;  */
       unsigned typ         : 4 ;
       unsigned justowanie  : 2 ;
       unsigned ukryty      : 1 ;
       unsigned dl          : 16 ;
	   unsigned multiline	: 1;
	   unsigned underline	: 1;
	   unsigned aux			: 1;
	   unsigned spacing		: 5;
    } TEXT_NAG3,  * PTR_TEXT_NAG3;

typedef
struct
{
	unsigned atrybut : 3;
	unsigned obiekt : 4;
	unsigned obiektt1 : 2;
	unsigned obiektt2 : 3;
	unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie */
	unsigned widoczny : 1;
	unsigned przec : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
	unsigned blok : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
	unsigned int n; //    : 32;
	unsigned warstwa : 8;
	unsigned kolor : 8;
	unsigned czcionka : 7;
	unsigned bold : 1;
	float kat;
	float wysokosc;        /*wysokosc znaku*/
	float width;           /*szerokosc znaku*/
	float x, y;
	/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
	/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
	/*       unsigned vertical    : 1 ; tekst pionowy*/
	/*       unsigned prop        : 1 ; tekst proporcjonalny*/
	unsigned italic : 1;
	/*       unsigned reserve     : 7 ;  */
	unsigned typ : 4;
	unsigned justowanie : 2;
	unsigned ukryty : 1;
	unsigned dl : 16;
	unsigned multiline : 1;
	unsigned underline : 1;
	unsigned aux : 1;
	unsigned spacing : 5;
	int t_width;
	int t_height;
} TEXT_NAG, *PTR_TEXT_NAG;

typedef
  struct
     { unsigned atrybut  : 3;
       unsigned obiekt   : 4;
       unsigned obiektt1 : 2;
       unsigned obiektt2 : 3;
       unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie */
       unsigned widoczny : 1;
       unsigned przec    : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
       unsigned blok     : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
       unsigned int n; //    : 32;
       unsigned warstwa  : 8;
       unsigned kolor    : 8;
       unsigned czcionka : 7;
       unsigned bold     : 1;
       float kat ;
       float wysokosc ;        /*wysokosc znaku*/
       float width ;           /*szerokosc znaku*/
       float x,y,z;
       float cosxz;
       float cosyz;
/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
/*       unsigned vertical    : 1 ; tekst pionowy*/
/*       unsigned prop        : 1 ; tekst proporcjonalny*/
       unsigned italic      : 1 ;
/*       unsigned reserve     : 7 ;  */
       unsigned typ         : 4 ;
       unsigned justowanie  : 2 ;
       unsigned ukryty      : 1 ;
       unsigned dl          : 16 ;
	   unsigned multiline	: 1;
	   unsigned underline	: 1;
	   unsigned aux			: 1;
	   unsigned spacing		: 5;
    } TEXT_NAG3D3,  * PTR_TEXT_NAG3D3;

typedef
struct
{
	unsigned atrybut : 3;
	unsigned obiekt : 4;
	unsigned obiektt1 : 2;
	unsigned obiektt2 : 3;
	unsigned obiektt3 : 1;  /* wykorzystane w : wymiarowanie */
	unsigned widoczny : 1;
	unsigned przec : 1;  /* wyznaczanie pzeciec, ciagnij (po uzyciu zerowane)*/
	unsigned blok : 1;  /*obiekt jest elementem bloku typu OdBLOK*/
	unsigned int n; //    : 32;
	unsigned warstwa : 8;
	unsigned kolor : 8;
	unsigned czcionka : 7;
	unsigned bold : 1;
	float kat;
	float wysokosc;        /*wysokosc znaku*/
	float width;           /*szerokosc znaku*/
	float x, y, z;
	float cosxz;
	float cosyz;
	/*       unsigned backwords   : 1 ; lustrzane odbicie wzgledem osi pionowej*/
	/*       unsigned upside_down : 1 ; lustrzane odbicie wzgledem osi poziomej*/
	/*       unsigned vertical    : 1 ; tekst pionowy*/
	/*       unsigned prop        : 1 ; tekst proporcjonalny*/
	unsigned italic : 1;
	/*       unsigned reserve     : 7 ;  */
	unsigned typ : 4;
	unsigned justowanie : 2;
	unsigned ukryty : 1;
	unsigned dl : 16;
	unsigned multiline : 1;
	unsigned underline : 1;
	unsigned aux : 1;
	unsigned spacing : 5;
	int t_width;
	int t_height;
} TEXT_NAG3D, *PTR_TEXT_NAG3D;


#pragma pack( 1 )    //1
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
       unsigned n        :16;
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned typ      : 4;    ///////  6 bajtow
/*       unsigned rezerwa1 : 1;*/
       float x;
	   float y;
	   float r;
       float kat1;
	   float kat2;             
    } LUK1_1;
typedef  LUK1_1  * LUK_1_1;

typedef
  struct
     { float x;
	   float y;
	   float r;
       float kat1;
	   float kat2;         ///////  20 bajtow
    } LUKEND1_1;
typedef  LUKEND1_1  * LUKEND_1_1;

#pragma pack( 4 )

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

       unsigned atrybut_  : 3;
       unsigned obiekt_   : 4;
       unsigned obiektt1_ : 2;
       unsigned obiektt2_ : 3;
       unsigned obiektt3_ : 1;
       unsigned widoczny_ : 1;
       unsigned przec_    : 1;
       unsigned blok_     : 1;
       unsigned int n_ ; //   :32;
       unsigned warstwa_  : 8;
       unsigned kolor_    : 8;
       unsigned typ_      : 8;

       unsigned shadowed_ : 1;
       unsigned rezerwa_  : 7;

       float x_,y_,r_;
       float kat1_,kat2_;
    } DWA_LUKI;

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
    float x,y,rx,ry,angle;
    float kat1,kat2;

    unsigned atrybut_  : 3;
    unsigned obiekt_   : 4;
    unsigned obiektt1_ : 2;
    unsigned obiektt2_ : 3;
    unsigned obiektt3_ : 1;
    unsigned widoczny_ : 1;
    unsigned przec_    : 1;
    unsigned blok_     : 1;
    unsigned int n_ ; //   :32;
    unsigned warstwa_  : 8;
    unsigned kolor_    : 8;
    unsigned typ_      : 8;
    unsigned shadowed_ : 1;
    unsigned rezerwa_  : 7;
    float x_,y_,rx_,ry_,angle_;
    float kat1_,kat2_;
} TWO_ELLIPTICAL_ARCS;

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
/*       unsigned rezerwa1 : 1;*/
       float x,y,r;
       float kat1,kat2;
       float z;
       float cosxz;
       float cosyz;
    } LUK3D;
typedef  LUK3D  * LUK3D_;

#pragma pack( 1 )
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
       unsigned n        :16;
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned typ      : 4;    //6
/*       unsigned rezerwa1 : 1;*/
       float x,y,r;
    } OKRAG1_1;
typedef  OKRAG1_1  * OKRAG_1_1;

typedef
  struct
     {
       float x,y,r;           //12
    } OKRAGEND1_1;
typedef  OKRAGEND1_1  * OKRAGEND_1_1;
#pragma pack( 4 )

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
/*       unsigned rezerwa1 : 1;*/
       float x,y,r;
       float z;
       float cosxz;
       float cosyz;
    } OKRAG3D;
typedef  OKRAG3D  * OKRAG3D_;

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
    unsigned translucency:8;
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


#pragma pack( 1 )
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
       unsigned n        : 16;
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned temp1    : 1;
       unsigned rezerwa1 : 3;
       unsigned lp       :16;  //        
       float xy[MaxNumPoints] ;
    } WIELOKAT1_1;
typedef  WIELOKAT1_1  * WIELOKAT_1_1;

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
       unsigned n        : 16;
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned temp1    : 1;
       unsigned rezerwa1 : 3;
       unsigned lp       : 16;
    } WIELOKATX1_1;
typedef  WIELOKATX1_1  * WIELOKATX_1_1;

typedef
  struct
     { 
       float xy[6] ;
    } VERTEX61_1;
typedef  VERTEX61_1  * VERTEX6_1_1;

typedef
  struct
     { 
       float xy[8] ;
    } VERTEX81_1;
typedef  VERTEX81_1  * VERTEX8_1_1;

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
	   //unsigned drawpoly : 1;
       unsigned gradient : 1;
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
	unsigned temp1 : 1;  //OT3W_Move=0, OT3W_Drag=1    //temp1
    unsigned shadowed : 1;
	unsigned npts : 3;  //qbic=3,quadratic=4, multipoints=5, max=8 //32
    unsigned multiple : 2;  //for animation 0 no animation, 1 26 frames 2 reserve, 3 reserve
	unsigned closed : 1;    //D3  //2D=0, 3D=1;
	unsigned lp : 16;  //can vary during drawing, ending with 8, but can be more for multipoints spline
	float xy[NumSplinePoints*2];  //just 8 points, maybe more for multipoints spline
} SPLINE, SPLINE3D;
typedef  SPLINE  * SPLINE_;
typedef  SPLINE3D  * SPLINE3D_;


#pragma pack( 1 )

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
       unsigned n        : 16;
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned typ      : 4;           //6
/*       unsigned rezerwa1 : 1;*/
       float x1          ;
       float y1          ;
       float x2          ;
       float y2          ;
    } LINIA1_1;

typedef  LINIA1_1  * LINIA_1_1;

typedef
  struct
     { float x1          ;
       float y1          ;
       float x2          ;
       float y2          ;     //16
    } LINIAEND1_1;

typedef  LINIAEND1_1  * LINIAEND_1_1;

typedef
struct
{
    unsigned char flags;  // 'G' 71 graph data
    unsigned char dt;  //data type (Dx=0, Nx=1, Vy=2, Mz=3, sigma=4, tau=5, geometry=7)
    unsigned char st;  //state number 2 for ULS or 3 for SLS
    unsigned char material;  //steel or wood, 1 RC
    float dlf;  //front distance
    float dlb;  //back distance
    float rdf;  //front radius
    float rdb;  //back radius
    unsigned enr : 16;  //element number
    unsigned nx : 16;  //number of data
} GRAPH_DATA;
typedef  GRAPH_DATA  *GRAPH_DATA_;

typedef
struct
{
    float H,B,A,Asy,Asz,Jx,Iy,Iz,Wy,Wz;
} SECTION_DATA;
typedef  SECTION_DATA  *SECTION_DATA_;

typedef
struct
{
    int Dy_no;
    char *Dy_data;
    int Dx_no;
    char *Dx_data;
    int Nx_no;
    char *Nx_data;
    int Vy_no;
    char *Vy_data;
    int Mz_no;
    char *Mz_data;
    int S_no;
    char *S_data;
    int Ss_no;
    char *Ss_data;
} SECTION_GRAPH_DATA;
typedef  SECTION_GRAPH_DATA  *SECTION_GRAPH_DATA_;

typedef
struct
{
    float Dy_min;
    float Dy_max;
    float Dx_min;
    float Dx_max;
    float Nx_min;
    float Nx_max;
    float Vy_min;
    float Vy_max;
    float Mz_min;
    float Mz_max;
    float S_min;
    float S_max;
    float Ss_min;
    float Ss_max;
} SECTION_FORCES;
typedef  SECTION_FORCES  *SECTION_FORCES_;

typedef
struct
{
    int enr;
    char *element_line;
    float x1, y1, x2, y2;
    char *graph_data;
    char *section_data;
    SECTION_GRAPH_DATA ULS;
    //SECTION_GRAPH_DATA ULSLC;
    SECTION_GRAPH_DATA SLS;
    //SECTION_GRAPH_DATA SLSLC;
    SECTION_GRAPH_DATA QPSLS;
} ALL_SECTION_GRAPH_DATA;
typedef  ALL_SECTION_GRAPH_DATA  *ALL_SECTION_GRAPH_DATA_;


typedef
struct
{
  char *b;
  char *adp;
  char *adk;
  char *plate_name;
  char *component_name;
  double xminp, xmaxp, yminp, ymaxp;
  float factor;
  int limit_state; //0 SLS, 1 ULS, 2 ULSLC, 3 SLSLC, 4 QPSLSLC   - for consistency
} PLATE_GRAPH_DATA;


/*
typedef
struct
{
    unsigned char flags;  // 'E' 45 element data
    unsigned char reserve;
    short element_no;
} ELEMENT_DATA;
typedef  ELEMENT_DATA  *ELEMENT_DATA_;
*/

typedef
struct
{
   float x;
   float vmin;
   float vmax;
} GRAPH_VALUES;

#pragma pack( 4 )

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
       unsigned kolor    : 8;   //255 - przezroczysty - dla potrzeb stropu
       unsigned typ      : 8;
       float x1          ;  //32
       float y1          ;  //32
       float x2          ;  //32
       float y2          ;  //32   -> 20
    } LINIA;
typedef  LINIA  * LINIA_;

#define MAXGRAPHDATANUMBER 256

    typedef
struct
{ unsigned atrybut  : 3;
    unsigned obiekt   : 4;
    unsigned obiektt1 : 2;
    unsigned obiektt2 : 3;
    unsigned obiektt3 : 1;
    unsigned widoczny : 1;
    unsigned przec    : 1;
    unsigned blok     : 1;                     //2
    unsigned int n ; //    : 32;               //6
    unsigned warstwa  : 8;
    unsigned kolor    : 8;   //255 - przezroczysty - dla potrzeb stropu
    unsigned typ      : 8;
    unsigned reserve  : 8;                     //10
    float x1          ;  //first point or center point
    float y1          ;  //first point or center point
    float x2          ;  //second point
    float y2          ;  //second point
    float r;  //for arcs                       //30
    //float angle;
    unsigned load      :8;  //0 undefined, 1 dead, 2 live, 3 live roof load, 4 wind, 5 snow, 6 seismic, 7 rainwater load or ice water load, 8 hydraulic loads from soil, 9  F = hydraulic loads from fluids
    unsigned variant   :8;  //0 undefined, 1..255 number of load character with different factors
    unsigned flags     :8;  //0  regular   1 inverted
    unsigned load22     :8;
    float angle1;
    float angle2;
    float magnitude1; //power magnitude or start magnitude
    float magnitude2; //end magnitude          //50
    unsigned style    : 8;
    unsigned reserve1 : 8;                     //52
    unsigned property_no : 16;                          //54
} AVECTOR;
typedef  AVECTOR  * AVECTOR_;

/*
Load Types
Loads used in design load equations are given letters by type:
0 undefined
1  D = dead load                        DL   cw ciężar własny           CM  Carga Muerta
2  L = live load                        LL   ou obc.użytkowe          CV  Carga Viva
3  Lr = live roof load                  Lr   od           Ct  Carga viva de techo
4  W = wind load                        WL   ow           CP  Carga de viento (Presión)
5  S = snow load                        SL   os           CN  Carga de Nieve
6  E = earthquake load                  EL   oq           CS  Carga sísmica
7  R = rainwater load or ice water load RL   od           CL  Carga de Lluvia y hielo
8  H = hydraulic load from soil        HL              CE  Cargas de Empuje del suelo, agua subterránea o materiales sueltos
9  F = hydraulic load from fluids      FL              CH  Carga Hidrostática
marked by vector style
10  T = effect of material & temperature TL
*/

#pragma pack( 4 )


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
       unsigned kolor    : 8;   //255 - przezroczysty - dla potrzeb stropu
       unsigned typ      : 8;
       float x1          ;  //32
       float y1          ;  //32
       float x2          ;  //32
       float y2          ;  //32   -> 20

       unsigned atrybut_  : 3;
       unsigned obiekt_   : 4;
       unsigned obiektt1_ : 2;
       unsigned obiektt2_ : 3;
       unsigned obiektt3_ : 1;
       unsigned widoczny_ : 1;
       unsigned przec_    : 1;
       unsigned blok_     : 1;
       unsigned int n_ ; //   : 32;
       unsigned warstwa_  : 8;
       unsigned kolor_    : 8;   //255 - przezroczysty - dla potrzeb stropu
       unsigned typ_      : 8;
       float x1_          ;  //32
       float y1_          ;  //32
       float x2_          ;  //32
       float y2_          ;  //32   -> 20
    } DWIE_LINIE;

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
       unsigned typ      : 8;
       float x1          ;  //32
       float y1          ;  //32
       float x2          ;  //32
       float y2          ;  //32
       float z1          ;  //32
       float z2          ;  //32
    } LINIA3D;
typedef  LINIA3D  * LINIA3D_;

#pragma pack( 1 )
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
       unsigned n        : 16;
       unsigned warstwa  : 8;
       unsigned kolor    : 4;
       unsigned rez      : 4;    //6
       float x           ; 
       float y           ;
    } T_Point1_1 ;

typedef
  struct
     { float x           ; 
       float y           ;    //8
    } T_PointEND1_1 ;
#pragma pack( 4 )
    
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
       //unsigned typ      : 6;     /*0 normalny, 1,2,3,4,5,6,7 junction, 8 pin, 9 pin g, 10 pin d, 11 pin s*/
       //unsigned rotation : 2;     /* 0=0deg, 1=90deg, 2=180deg, 3=270deg*/
       float x           ; 
       float y           ;
    } T_Point ;    

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
       unsigned typ      : 8;     /*0 normalny, 1,2,3,4,5,6,7 inne, 8 pin*/
       float x           ; 
       float y           ;
       float z           ;
    } T_Point3D ;


#pragma pack( 1 )
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
       unsigned n        : 16; //4 bajty
       char kod_obiektu  : 8;
       short dlugosc_opisu_obiektu ; //:16;
       char opis_obiektu [1]   ;
    } BLOK1_1;
typedef  BLOK1_1  * BLOK_1_1;

typedef
  struct
     { 
       char kod_obiektu  : 8;  //1 bajt
    } BLOK1_1F;
typedef  BLOK1_1F  * BLOK_1_1F;

typedef
  struct
     { 
       short dlugosc_opisu_obiektu ; //:16;  //2 bajty
    } BLOK1_1FF;
typedef  BLOK1_1FF  * BLOK_1_1FF;

typedef
  struct
     { 
       char opis_obiektu [1]   ;
    } BLOK1_1FFF;
typedef  BLOK1_1FFF  * BLOK_1_1FFF;

#ifndef LINUX
#pragma pack( 1 )
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
       unsigned char flag ; //     : 8;  //rezerwa
       unsigned short dlugosc_opisu_obiektu :16 ;
       char opis_obiektu [1]   ;
    } BLOK;
typedef  BLOK  * BLOK_;

#pragma pack( 1 )
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
       unsigned n        : 16;
       char kod_obiektu ; // : 8;
       unsigned short dlugosc_opisu_obiektu :16;
    } BLOKD1_1;
typedef  BLOKD1_1  * BLOKD_1_1;
#pragma pack( 1 )

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
       char flag; // rezerwa; //    : 8;
       unsigned short dlugosc_opisu_obiektu :16;
      ////char opis_obiektu [1]   ;  //???
    } BLOKD;
typedef  BLOKD  * BLOKD_;

#pragma pack( 1 )

typedef struct
{
  unsigned flags ;      /*1 - bez x, y*/
  int len ;             /*dlugosc tekstu opisu*/
  char sz_type [1] ;    /*tekst opisu zakonczony '\0'*/
}
T_Desc_Ex_Block_Old ;


typedef struct
{
  unsigned flags ;      /*2 - dodano x, y*/
  float  x, y ;         /*punkt wstawienia*/
  int len ;             /*dlugosc tekstu opisu*/
  char sz_type [1] ;    /*tekst opisu zakonczony '\0'*/
}
T_Desc_Ex_Block1_1 ;

typedef struct
{
  unsigned int flags ;      /*2 - dodano x, y*/
  float  x, y ;         /*punkt wstawienia*/
  int len ;             /*dlugosc tekstu opisu*/
}
T_Desc_Ex_Block1_1korekt ;

#pragma pack( 4 )


typedef struct
{
    int flags ;           /*2 - dodano x, y*/
    float  x, y ;         /*punkt wstawienia*/
    int len ;             /*dlugosc tekstu opisu*/
    char sz_type [1] ;    /*tekst opisu zakonczony '\0'*/
}
T_Desc_Ex_Block_4_1 ;

typedef struct
{
  int flags ;           /*2 - dodano x, y*/
  float  x, y ;         /*punkt wstawienia*/
  int len ;             /*dlugosc tekstu opisu*/
  char sz_type [1] ;    /*tekst opisu zakonczony '\0'*/
}
T_Desc_Ex_Block ;

enum ExportBlockFlags {EBF_Old = 0x01, EBF_IP = 0x02 } ;

#pragma pack( 1 )
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
       unsigned n        : 16;   
     } NAGLOWEK1_1;
#pragma pack( 4 )

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
     } NAGLOWEK;    

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
	   
     } NAGLOWEK_EXT;    

typedef struct
     { double sin,cos,kat;
       double dx;
       double dy;
       double dl;
     }PLINIA;

typedef struct
     { double sin,cos,kat,sin1,cos1,kat1;
       double dl;
     }PLINIA3D;

typedef enum
{
 I_Orto_NoDir = 0,
 I_Orto_XDir,
 I_Orto_YDir
}
T_Orto_Dir;


typedef
  struct { double x,y;}
 CURPOZ;

enum WARWID   { NoWarWid=0,WarWid=1};
enum WARZAM   { NoWarZam=0,WarZam=1};



#define MAX_NUMBER_OF_LAYERS 256
#define MAX_NUMBER_OF_LAYERS_DXF 256 //1024
#define MAX_OLD_NUMBER_OF_LAYERS 16

#pragma pack(1)

//#ifdef FORWIN32
//#define maxlen_w 19    //20                          //win32todo   ?????
//#else
#define maxlen_w20 20
#define maxlen_w 64
//#endif

typedef
  struct
   { 
#pragma pack(1)
	 unsigned on        : 1;
     unsigned edit      : 1;
     unsigned point     : 1;
     unsigned color     : 4;
     unsigned line_type : 4;
     unsigned grey      : 1;
	 unsigned bw        : 1;
	 unsigned reserve   : 3;
     unsigned len_name  : 8;  //3 bytes
     char name [maxlen_w20] ;
   } LAYER1_1 ;

  typedef
  struct
   { 
#pragma pack(1)
     char name [maxlen_w20] ;
   } LAYER_name_1_1 ;
   
#pragma pack(4)

#pragma pack(1)

typedef
  struct
   { unsigned on        : 1;
     unsigned edit      : 1;
     unsigned point     : 1;
     unsigned color     : 4;
     unsigned line_type : 4;
     unsigned grey      : 1;
     unsigned bw        : 1;
	 unsigned reserve   : 3;
     unsigned len_name  : 8;
     char name [maxlen_w20] ;
   } LAYER2_0 ;
   
typedef
  struct
   { unsigned on        : 1;
     unsigned edit      : 1;
     unsigned point     : 1;
     unsigned grey      : 1;
	 unsigned bw        : 1;
	 unsigned reserve   : 3;
     unsigned color     : 8;
     unsigned line_type : 8;
     unsigned len_name  : 8;   
     char name [maxlen_w20] ;
   } LAYER3 ;  

typedef
struct
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

typedef
struct
{
    unsigned on : 1;
    unsigned edit : 1;
    unsigned point : 1;
    unsigned grey : 1;
    unsigned bw : 1;
    unsigned reserve : 3;
}LAYER_SWITCHES;

#pragma pack(4)

#define MaxLiczbaTypowLinii 32
#define MaxLenNameLine 40  //32 //20
#define MaxLenNameLayer 64 //33
#define MaxLenNameLayerDXF 64 //26
#define MaxLiczbaKresekLinii 12
typedef
  struct
   { char       nazwa[MaxLenNameLine];
     unsigned   opis;           /*  16 bitow  */
     double     dlwzorca;
     int        IloscElementow;
     double     kreski[MaxLiczbaKresekLinii];     /* dl 1-ej kreski, dl 2-ej kreski ...*/
   } LTYPE;


#pragma pack(1)

typedef
  struct
    { unsigned char Lkolor;
      unsigned char Tkolor;
      double wysokosc;
      double dokladnosc;
      char strzalka;
      double linia_ob;
      char kierunek_t;
    }ZMIENNE_OLD1_0 ;

#pragma pack(1)
typedef
  struct
    { unsigned char Lkolor;
      unsigned char Tkolor;
      double wysokosc;         /*wysokosc znaku*/
      double width_factor ;    /*wsp. szerokosci znaku*/
      double dokladnosc;
      double linia_ob;
      unsigned strzalka         : 2 ;
      unsigned kierunek_t       : 1 ;
      unsigned b_add_line_const : 1 ;
      unsigned italics          : 1 ;
      unsigned czcionka         : 4 ;
      unsigned reserve          : 7 ;
    }ZMIENNE_OLD1_1 ;
    
#pragma pack(4)


typedef
  struct
    { unsigned char Lkolor;
      unsigned char Tkolor;
      double wysokosc;         /*wysokosc znaku*/
      double width_factor ;    /*wsp. szerokosci znaku*/
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

typedef
struct
{
    double wysokosc;         /*wysokosc znaku*/
    double width_factor ;    /*wsp. szerokosci znaku*/
    unsigned italics          : 1 ;
    unsigned czcionka         : 7 ;
    unsigned bold             : 1 ;
    unsigned format           : 2 ;  //0-metric, 1-imperial
    unsigned reserve          : 5 ;
    unsigned reserve1         : 8 ;
    unsigned reserve2         : 8 ;
    double force_precision;
    double moment_precision;
    double displacement_precision;
    double rotation_precision;
    double load_precision;
    double thermal_precision;
}ZMIENNE_VECTOR ;

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

typedef
   struct
     { double x1,y1,x2,y2;
       double x01,y01,x02,y02;
     } OKNO;

typedef struct
     { double x1,y1,x2,y2;
     } A_WINDOW;

typedef struct
 { int       x, y ;
#ifdef ARM64
long      flag;
#else
   char      flag;
#endif
   int       np;
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

#define COLOR_DEF -1
typedef struct
 {
   int borderm ;
   int paperm ;
   int inkm ;

   int cur ;
   int blok ;
   int border ;
   int paper ;
   int ink ;
   int paperk ;
   int inkk ;
   int paperk_ini ;
   int inkk_ini ;

   int dlg_border ;
   int dlg_paper ;
   int dlg_label ;
   int dlg_in_line_border ;
   int dlg_in_line_ink ;
   int dlg_in_line_paper ;
   int dlg_button_ink ;
/*   int dlg_button_paper;
   int dlg_button_border;
*/
} KOLORY;

typedef struct
 {
  /*dodatkowe kolory Astera  -  24 wartosci, aktywne 19*/
   int aparat ;
   int wezel ;
   int zacisk ;
   int zasilanie ;
   int port ;
   int schemat ;
   int pin ;
   int t_symbol ;
   int t_typ ;
   int t_pin_name ;
   int t_zasilanie ;
   int t_port ;
   int t_schemat ;
   int t_plik ;
   int t_siec ;
   int t_komentarz ;
   int t_zmienna ;
   int t_symbol_drabinki ;
   int t_opis_drabinki ;
   int ramka_sektorow ;
   int opis_sektorow ;
   int rezerwa3 ;
   int rezerwa4 ;
   int rezerwa5 ;
 } KOLORY_ASTER;
 
typedef struct
 {
  /*dodatkowe kolory Profili  -  24 wartosci, aktywne 21*/
   int os_pomiaru ;
   int l_teren_i ;
   int l_teren_p ;
   int l_ulicy ;
   int l_kanal_i ;
   int l_kanal_p ;
   int rzedne ;
   int spadek_l_fi_mat ;
   int odleglosc ;
   int hektometry ;
   int opis_instalacji ;
   int T ;
   int e ;
   int G ;
   int W ;
   int K ;
   int os_otworu ;
   int numer_otworu ;
   int rzedna_otworu ;
   int opis_warstw ;
   int symbol_wody ;
   int rezerwa1 ;
   int rezerwa2 ;
   int rezerwa3 ;
   int rezerwa4 ;
 } KOLORY_PROFILI;

 typedef struct
 {
  /*dodatkowe kolory Sieci  -  24 wartosci, aktywne 21*/
   int Pz ;       //0
   int Cok ;      //1
   int Co ;       //2
   int T ;        //3
   int Tk ;       //4
   int e ;        //5
   int E ;        //6
   int S ;        //7
   int D ;        //8
   int W ;        //9
   int inne ;     //10
   int rezerwa1 ; //11
   int rezerwa2 ; //12
   int rezerwa3 ; //13
   int rezerwa4 ; //14
   int rezerwa5 ; //15
   int rezerwa6 ; //16
   int rezerwa7 ; //17
   int rezerwa8 ; //18
   int rezerwa9 ; //19
   int rezerwa10 ;//20
   int rezerwa11 ;//21
   int rezerwa12 ;//22
   int rezerwa13 ; //23
 } KOLORY_SIECI;

 typedef struct
 {
  /*dodatkowe kolory Stropu  -  24 wartosci*/
   int osie ;
   int obrys ;
   int plyta ;
   int linie_podzialu ;
   int otwor ;
   int cien_otworu ;
   int o_symbol ;
   int o_typ ;
   int o_komentarz ;
   int siatka_dolna ;
   int sd_symbol ;
   int sd_typ ;
   int sd_komentarz ;
   int zbrojenie ;
   int opis_zbrojenia ;
   int siatki_stykowe ;
   int n ;
   int o ;
   int p ;
   int q ;
   int r ;
   int s ;
   int t ;
   int u ;
   int v ;
 } KOLORY_STROPU;

typedef struct
 { int mvc;
   int akton,aktoff;
   unsigned L,T,K,O,l,W,P,EA,E,FE ;
 } MVCURB;

typedef char * RYSPOZ;

typedef struct
 { int What;
   int Number;
  } EVENT;

typedef struct
{
    long x1, y1, x2, y2;
} p_vector;

typedef struct
{
    long x, y;
} p_point;

typedef struct
{
    float x, y;
} pf_point;

/* Event codes */

#define evMouseDown  0x0001
#define evMouseUp    0x0002
#define evMouseMove  0x0004
#define evMouseAuto  0x0008
#define evKeyDown    0x0010
#define evCommand    0x0100
#define evCommandP   0x0101
#define evCommandKF  0x0102
#define evTabFun     0x0103
#define evBroadcast  0x0200

/* Event masks */

#define evNothing    0x0000
#define evMouse      0x000F
#define evKeyboard   0x0010
#define evMessage    0xFF00

#ifdef LINUX
#define O_BINARY 0
#include <string.h>
#include <ctype.h>
/*
=============
itoa

Convert integer to string

PARAMS:
- value     A 64-bit number to convert
- str       Destination buffer; should be 66 characters long for radix2, 24 - radix8, 22 - radix10, 18 - radix16.
- radix     Radix must be in range -36 .. 36. Negative values used for signed numbers.
=============
*/
extern char* itoa (unsigned long long  value,  char str[],  int radix);
extern char* strlwr(char* s);
extern char* strupr(char* s);
/*
char* itoa (unsigned long long  value,  char str[],  int radix)
{
    char        buf [66];
    char*       dest = buf + sizeof(buf);
    BOOL     sign = FALSE;

    if (value == 0) {
        memcpy (str, "0", 2);
        return str;
    }

    if (radix < 0) {
        radix = -radix;
        if ( (long long) value < 0) {
            value = -value;
            sign = TRUE;
        }
    }

    *--dest = '\0';

    switch (radix)
    {
        case 16:
            while (value) {
                * --dest = '0' + (value & 0xF);
                if (*dest > '9') *dest += 'A' - '9' - 1;
                value >>= 4;
            }
            break;
        case 10:
            while (value) {
                *--dest = '0' + (value % 10);
                value /= 10;
            }
            break;

        case 8:
            while (value) {
                *--dest = '0' + (value & 7);
                value >>= 3;
            }
            break;

        case 2:
            while (value) {
                *--dest = '0' + (value & 1);
                value >>= 1;
            }
            break;

        default:            // The slow version, but universal
            while (value) {
                *--dest = '0' + (value % radix);
                if (*dest > '9') *dest += 'A' - '9' - 1;
                value /= radix;
            }
            break;
    }

    if (sign) *--dest = '-';

    memcpy (str, dest, buf +sizeof(buf) - dest);
    return str;
}

char* strlwr(char* s)
{
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }

    return s;
}

char* strupr(char* s)
{
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }

    return s;
}
*/

#define LONG long

typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;


#endif



#include "bib_ic.h"


#define B3 4
#define Bdef   {Anormalny,OdBLOK,0,0,0,1,0,0,  B3, '\0', 0, 0,'\0'}
#define BDdef  {Anormalny,OdBLOK,0,0,0,1,0,0,  B3, '\0', 0, 0} ///TO CHECK
#define Ldef  {Anormalny,Olinia,0,0,0,1,0,0,  20, 0,7,64, 0,0,0,0}
#define L2def {Anormalny,Olinia,0,0,0,1,0,0,  48, 0,7,64, 0,0,0,0, Anormalny,Olinia,0,0,0,1,0,0,  20, 0,7,64, 0,0,0,0}

#define Vdef  {Anormalny,Ovector,0,0,0,1,0,0,  48, 0,7,64, 0, 0,0,0,0, 0, 0,0,0,0, 0,0, 0,0, 0, 0, 0}

#define T18_3   29//37//29 //27 //25
#define T18   37
#define T39_3 39
#define T39 47
#define T25 25
#define T294 294
#define T3def  {Anormalny,Otekst,0,0,0,1,0,0, T18_3, 0,7,0,0,0,3,1,0,0,0,0,0,0,0,0,0,0,6,""}  //win32todo
#define Tdef {Anormalny,Otekst,0,0,0,1,0,0, T18, 0,7,0,0,0,3,1,0,0,0,0,0,0,0,0,0,0,6,0,0,""}  //win32todo
#define T3D3def  {Anormalny,Otekst3D,0,0,0,1,0,0, T39_3, 0,7,0,0,0,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,6,""}

#define PCXdef {Anormalny,Opcx,0,0,0,1,0,0, T294, 0,0,0,0,0,0,0,0,0,0,0,0,0,41,1,0,0,0}
#define Kdef  {Anormalny,Okolo, 0,0,0,1,0,0, 16, 0,7,64,  0,0,0}
#define Odef  {Anormalny,Ookrag,0,0,0,1,0,0, 16, 0,7,64,  0,0,0}

#define Edef  {Anormalny,Oellipse,0,0,0,1,0,0, 24, 0,7,64,  255, 0,0,0,0,0}
#define FEdef  {Anormalny,Ofilledellipse,0,0,0,1,0,0, 24, 0,7,64,  255, 0,0,0,0,0}
#define eldef  {Anormalny,Oellipticalarc,0,0,0,1,0,0, 32, 0,7,64,  0,0, 0,0,0,0,0,0,0}

#define PointDef {Anormalny,Opoint,0,0,0,1,0,0, 12, 0,7,0, 0,0}
#define ldef  {Anormalny,Oluk,0,0,0,1,0,0, 24, 0,7,64, 0,0, 0,0,0, 0,0}
#define l2def  {Anormalny,Oluk,0,0,0,1,0,0, 56, 0,7,64, 0,0,0, 0,0,  Anormalny,Oluk,0,0,0,1,0,0, 24, 0,7,64, 0,0,0, 0,0}

#define SOLIDARC_N 60
#define sadef {Anormalny,Osolidarc,0,0,0,1,0,0, SOLIDARC_N /*52*/,  0,7,0, 0, 0, 0,0,0, 255,0,0,0,0,0,0,0,0,0,1,0,0,0,""};

//SOLIDARC solidarc=sadef;

#define Stdef {Anormalny,Owwielokat,0,0,0,1,0,0, 32, 0,7,0,0,0,0,0,0, 6,0,0,0,0,0,0}
#define St3Ddef {Anormalny,Owwielokat,0,0,0,1,0,0, 44, 0,7,0,0,0,0,0,0, 6,0,0,0,0,0,0,0,0,0}
#define S4def {Anormalny,Owwielokat,0,0,0,1,0,0, 40, 0,7,0,0,0,0,0,0, 8,0,0,0,0,0,0,0,0} /*czworokat*/
#define S43Ddef {Anormalny,Owwielokat,0,0,0,1,0,0, 56, 0,7,0,0,0,0,0,0, 8,0,0,0,0,0,0,0,0,0,0,0,0} /*czworokat 3D*/
//#define Splinedef {Anormalny,Ospline,0,0,0,1,0,0, 40, 0,7,0,0,4,0, 8,0,0,0,0,0,0,0,0} /*Bezier spline 4 points*/
#define Splinedef {Anormalny,Ospline,0,0,0,1,0,0, 40, 0,7,0,0,0,4,0,0, 8,0,0,0,0,0,0,0,0} /*0 no multiple, Bezier spline 4 points*/
#define Defzmwym {7,7,3,1,.01,10,0,0,1,0,0,0,0 }
#define Defzvector {3,1,0,0,0,0,0,0,0,0.01,0.01,0.01,0.001}
#define Defsel {0,0,0,0,0,0,0,0,7}  //5
#define Defwar {1,1,1,0,0,0,7,64,1,"0"}
#define DefLineType {u8"Continuous",0,0.,0,0.}
#define Defkolory  { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0}
#define Defkolorya { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define Defkoloryp { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define Defkoloryps { 2,1,1,128,128,40,40,10,11,5,15,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define Defkolorys { 91,7,1,127,7,15,7,7,7,7,7,7,7,7,7,89,7,7,7,7,7,7,7,7}
#define Defmvcurb { 0,0,0,0,0,0,0,0,0,0   }
#define DefDimVars {TRUE}
#define FormatyAr { {210,297},{210,297},{297,210},{420,297},{297,420},{594,420},{420,594},{841,594},{594,841},{1189,841},{841,1189},\
  	                {185,280},{280,185},{395,280},{565,400},{810,565},{1160,810},\
	                {630,297},{841,297},{1050,297},{1260,297},\
	                {600,280},{810,280},{1020,280},{1230,280},\
                    {250,176},{176,250},{500,353},{353,500},{707,500},{500,707},{1000,707},{707,1000},{1414,1000},{1000,1414}, \
                    {279,215},{215,279},{431,279},{279,431},{559,432},{432,559},{864,559},{559,864},{1118,864},{864,1118},\
                    {215,355}}

//double funits[5] = { 1.0, 10.0, 1000.0, 25.0, 304.8 };
//double fscales[14] = { 1,2,5,10,20,25,50,100,500,1000,5000,10000,0.5,0.2 };
#define InsertDef {0,64,7,0,"",0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define Aparat_def {0,"",0,"",0,""}

/*wersja 1_1*/

#define B3_1_1 3
#define Bdef_1_1  {Anormalny,OdBLOK,0,0,0,1,0,0,  B3_1_1, '\0',0,"\0"}
#define BDdef_1_1  {Anormalny,OdBLOK,0,0,0,1,0,0,  B3_1_1, '\0',0}
#define Ldef_1_1  {Anormalny,Olinia,0,0,0,1,0,0,  18, 0,7,5, 0,0,0,0}

#define T18_1_1 25
#define Tdef_1_1  {Anormalny,Otekst,0,0,0,1,0,0,  T18_1_1, 0,7,0, 0,3,1,0,0,0,0,0,""}
#define Kdef_1_1 {Anormalny,Okolo,0,0,0,1,0,0,   14, 0,7,5,  0,0,0}
#define Odef_1_1     {Anormalny,Ookrag,0,0,0,1,0,0,   14, 0,7,5,  0,0,0}
#define PointDef_1_1 {Anormalny,Opoint,0,0,0,1,0,0,   10, 0,7,0,  0,0}
#define ldef_1_1  {Anormalny,Oluk,0,0,0,1,0,0,   22, 0,7,5,  0,0,0, 0,0}
#define Stdef_1_1 {Anormalny,Owwielokat,0,0,0,1,0,0,  28,  0,7,0,0,  6,0,0,0,0,0,0}
#define S4def_1_1 {Anormalny,Owwielokat,0,0,0,1,0,0,  36,  0,7,0,0,  8,0,0,0,0,0,0,0,0} /*czworokat*/




extern int PL366;
extern int PL266;

enum SELA  {NASel=0,ASel};
enum SELAW {NAWSel=0,AWSel};
enum SELG  {NGSel=0,GSel};
enum SELC  {NCSel=0,CSel};


enum OKNOTYP {Cross=0,Dragg,Windoww};

#define MAXINSETS 64
#define MAXPINSETS 32

#define COMNDmnr_base 173
#define COMNDmnr_Insets 32
#define COMNDmnr (COMNDmnr_base + COMNDmnr_Insets) //173+32 //maksymalny numer funkcji w menu poziomym - (max number of the function in auxiliary menu)
#define ENTER 0x0d
#define MOUSEENTER 2132
#define SPC 0x20
#define CTRL_SPC 0x14  //Device control 4
#define CTRL_CR 0x0a
#define TAB 9
#define ESC 27
#define MOUSEESC ESC
#define DEL 83
#define ShTAB 15
#define SHTAB 271
#define Shift_F5 344
#define Pi2 (2*3.1415926535897931)
#define Pi_ 3.1415926535897931

/*  #define line_ge_x1 0.0
#define line_ge_y1 0.0
#define line_ge_x2 0.0
#define line_ge_y2 0.0
#define line_ge1_x1 0.0
#define line_ge1_y1 0.0
#define line_ge1_x2 0.0
#define line_ge1_y2 0.0  */

#define Bnic                0
#define Blinia              1
#define Bluk                2
#define Bokrag              4
#define Bkolo               8
#define Bwwielokat          16
#define Btekst              32
#define Bpoint              64
#define Bspline             128
#define Bellipse            256
#define Bfilledellipse      512
#define Bellipticalarc      1024
#define Bsolidarc           2048
#define Bvector             4096
#define Bprzeciecie         8192
#define Bwymiarowanie       16384
#define Bblok               32768
#define Bpolygon            65536
#define Bpcx                131072
#define BtekstNoType15      262144
#define BwwielokatGradient  524288

typedef enum { PRN_DRIVE = 0, PLT_DRIVE } DRAW_DRIVE;

#define SZ_YES          "T"
#define SZ_NO           "N"
#define VERSION1_0      1
#define VERSION1_1      2
#define VERSION2_0      3
#define VERSION3_0      4
#define VERSION3_1      5
#define VERSION4_0      6
#define VER_LEN         10
#define VERB_LEN        12

#define VER1_0          "ALFAX  1.0"
#define VERB1_0         "ALFAX  1.0B"
#define VER2_0		    "ALFAX  2.0"
#define VER1_1          "ALFAX  1.1"
#define VERB1_1         "ALFAX  1.1B"
#define VERB2_0         "ALFAX  2.0B"

#define ver1_0 10
#define verb1_0 100
#define ver2_0 20
#define ver1_1 11
#define verb1_1 110
#define verb2_0 200


#define VER2_1          "ALFAX  2.1"
#define VER3_0          "ALFAX  3.0"
#define VER3_1	        "ALFAX  3.1"
#define VER4_0	        "ALFACAD4.0"
#define VER4_1	        "ALFACAD4.1"
#define VER4_2	        "ALFACAD4.2"

#define ver2_1 21
#define ver3_0 30
#define ver3_1 31
#define ver4_0 40
#define ver4_1 41
#define ver4_2 42

#define VER_ASTER       "ASTER-CAD "

#define VERB2_1_        "ALFAX  2.1B"
#define VERB2_1         "ALFAX 2.1B "
#define VERB3_0_        "ALFAX  3.0B"
#define VERB3_0         "ALFAX 3.0B "
#define VERB4_0         "ALFACAD4.0B"
#define VERB4_1         "ALFACAD4.1B"
#define VERB4_2         "ALFACAD4.2B"

#define verb2_1 210
#define verb3_0 300
#define verb4_0 400
#define verb4_1 410
#define verb4_2 420

#define VERB_ASTER      "ASTER-CAD FI"

enum IO_RETVAL { IO_RET_OK = 0,
                 IO_RET_PROT_ERR = 1,
		 IO_RET_RW_ERR = 2,
		 IO_RET_FILE_TYPE_ERR = 3,
                 IO_RET_OPEN_FILE_ERR = 4,
                 IO_RET_OK_ASTER = 5,
                 IO_RET_MEM_ERR = 18, 
                 IO_RET_READ_AGAIN = 20} ;  /* zapisz i odczytaj jeszcze raz */

enum MATH_PREC { MPREC_1      = 1,
                 MPREC_2      = 2,      /*t - dla odcinka, delta dla stycznych okregow*/
                 MPREC_3      = 3,
                 MPREC_4      = 4,      /*kat - dla odcinka stycznego do luku*/
                 MPREC_FLOAT =  5,
                 MPREC_DOUBLE = 6,
                 MPREC_ANGLE = 7} ;

enum POL_CHAR
{ a_PL = 134, c_PL = 141, e_PL = 145,
  l_PL = 146, n_PL = 164, o_PL = 162,
  s_PL = 158, z_PL = 166, x_PL = 167,
  A_PL = 143, C_PL = 149, E_PL= 144,
  L_PL = 156, N_PL = 165, O_PL = 163,
  S_PL = 152, Z_PL = 160, X_PL = 161
} ;

typedef struct {
	     double x1;             
             double y1; 
             double x2;             
             double y2;     
             double x3;             
             double y3;   
             double x4;             
             double y4;   
             double z;
             char symbol[31];
             char typ[32];          
             char komentarz[31];
               } APARAT;

typedef struct {
   	       double x1;
             double y1; 
             double x2;             
             double y2;     
             double x3;             
             double y3;   
             double x4;             
             double y4;   
             double z;
             char symbol[31];
             char typ[32];          
             char komentarz[31];
               } OTWOR;


typedef struct {
   	       double x1;
             double y1; 
             double x2;             
             double y2;     
             double x3;             
             double y3;   
             double x4;             
             double y4;   
             double z;
             char glowne[31];
             char rozdzielcze[31];
             char typ[31];
             int dir;   //0 = X, 1=Y, 2=kat
             double kat;
               } ARMNET;
                    
               
typedef struct {
               int type_flag;
               char typ[32];
               int symbol_flag;
               char symbol[32];
               int komentarz_flag;
               char komentarz[31];
               } APARAT_SPEC;

typedef struct {
               char atrybut[MaxTextLen];
               long adres;  //adres w pliku ATTRIBS.LIS
               int pozycja;
               } ATRYBUT_APARATU;
               
typedef struct {
	     double x1;             
             double y1; 
             double x2;             
             double y2;     
             double x3;             
             double y3;   
             double x4;             
             double y4;   
             double z;
             char nazwa[120];
             char plik[120];          
               } SCHEMAT;               
               
typedef struct {
               unsigned char layer;   
               unsigned char type;
               unsigned char color;
               char atrybut;
               char block_name[60];
               double x;
               double y;
               double z;
               double x_scale;
               double y_scale;
               double z_scale;
               double rotation_angle;
               int column_count;
               int row_count;
               double column_spacing;
               double row_spacing;
               double x_extrusion;
               double y_extrusion;
               double z_extrusion;
               double point_x_x;
               double point_y_y;
               double m_x;
               double m_y;
               double m_z;
               int typ;
               int useri1;
               int ep_space;
			   BOOL dim;
               } INSERT;               

typedef struct {
                char symbol[31];
                int no_typ;
                unsigned char flags;
               }  SYMBOL_APARATU;
typedef struct {
                char typ[31];
                int no_dbf;
                long adres_dbf;
                unsigned char flags;
               }  TYP_APARATU;

typedef struct {               
   double x;
   double y;
   double k1;
   double k2;
   double xa;
   double ya;
   double ka1;
   double ka2;
   int trans;                              
   } UNDO_TAB_REC;
   
typedef struct {
	     double x1;             
             double y1; 
             double x2;             
             double y2;     
             double x3;             
             double y3;   
             double x4;             
             double y4;   
               } PRECTANGLE;   

#pragma pack(1)
typedef struct {
   unsigned scale_DIM : 1;
   unsigned save_original_layer : 1;
   unsigned fill_pcx_solid      : 1;   
   //unsigned lock_prof_aktual : 1;
   unsigned uklad_izometryczny : 1;
   unsigned uklad_geodezyjny : 1;
   unsigned view_only_current_layer : 1;
   unsigned ignore_buffering_pcx : 1;
   unsigned normalize_text : 1;   
   }  OPTIONS1 ; 
 
#pragma pack(4)

typedef struct
{
	BOOL 	   b_layer, b_color, b_width, b_type,
		b_text_font, b_text_type, b_text_ukryty, b_text_just, b_text_height, b_text_width, b_text_italics, b_text_bold, b_text_underline, b_hflip, b_vflip, b_ontop, b_drawpoly, b_translucency;
  int  	   layer,  color,  width,  type, 	/*color - pozycja menu*/
	   i_text_font, i_text_type, i_text_ukryty, i_text_just, i_text_italics, i_text_bold, i_text_underline, i_hflip, i_vflip, i_ontop, i_translucency;
  double   df_text_height, df_text_width ;
}
T_change_param ;


typedef struct
{
  int obiekty;        //59
  int osie;           //60
  int obrys;          //61
  int plyty;          //62
  int otwory;         //63
  int linie_podzialu; //64
  int zbrojenie_dolne;   //65
  int zbrojenie_kr;   //66
  int zbrojenie_dod_d;   //67
  int zbrojenie_dolne2;   //68
  int zbrojenie_dod_d2;   //69
  int zbrojenie_gorne; //70
  int zbrojenie_dod_g; //71
  int zbrojenie_opis_d;  //72
  int zbrojenie_opis_g;//73
  int strop_gestozebrowy_opis;//74
  int wymiarowanie;   //75
  int wymiarowanie_obrys;//76
  int strop_gestozebrowy_uklad;      //77
  int opis_ukladu_plyt;        //78
  int opis;              //79
  int first_pref_layer;  //80
} WARSTWY_STROPU;

typedef struct
{
  int obrys;       //0
  int zbrojenie;   //1
  int wymiarowanie;//2
  int opis;        //3
  int __zbrojenie__; //4
} WARSTWY_PREFABRYKATU;

typedef struct
{ double x;
  double y;
} point_arm;




#define byte_ unsigned char


#pragma pack(1)
typedef struct
{
	byte_ red;
	byte_ gre;
	byte_ blu;
} COLOR_;
#pragma pack(4)

typedef COLOR_ COLOR[256];

typedef struct
{
    short int scale;
    short int dx;
    short int angle;
    short int dy;
    short int flag;  //1 = hide
    char pattern[32];
} SOLID_PATTERN;


typedef struct
{
    char type; //pcx or png
    char pattern_name[32];
    int size;
    char *pattern_buffer;
    #ifndef BIT64
    char* reserve;
    #else
    #endif
} SOLID_PATTERN_LIBRARY_ITEM;

#define MaxMenuWinFont 32

#define MAX_NODES    1024

typedef struct
{
    float x, y;
    double tangent;
} NODE;

typedef struct
{
	float xy[8] ;
} QUAD;

typedef struct
{
	float x;
	float y;
} POINTF;

typedef struct
{
    double x;
    double y;
} POINTD;

typedef struct
{
    double xp;
    double yp;
    double xk;
    double yk;
} RECTD;

/*
typedef struct {
    POINTD start;
    double bulge;
    POINTD end;
} PolylineArc;
*/
typedef struct
{
    char *menu;
    char st[POLE_TXT_MAX];  //128
    unsigned short cod;
    int iconno;
} MENUPSTRUCT;

typedef struct{
    double x,y;
}point;


typedef struct RGBA
{
    int r, g, b, a;
} RGBA;

typedef struct{
    unsigned int TYP;
    int type;
    int style;
    int color;
    int layer;
    char *address;
}TYPE_ADDRESS;

typedef struct{
    //char *str;
    int ns_arc;
    int ch;
    int d;
    int i;
} KOM_BAK;

typedef struct {
    unsigned int c1;
    unsigned int c2;
    unsigned int c3;
} GRADIENT3;

typedef struct {
    unsigned int c1;
    unsigned int c2;
    unsigned int c3;
    unsigned int c4;
} GRADIENT4;

typedef struct {
    unsigned int c1;
    unsigned int c2;
    unsigned int c3;
    unsigned int c4;
    unsigned int c5;
    unsigned int c6;
    unsigned int c7;
    unsigned int c8;
} GRADIENT8;

typedef struct
{
    //unsigned flag:4;
    unsigned el_number:30;
    unsigned flag:2;  //0 normal  1 extended, 2 extended twice, 3 extended tripple
    float f1;  //for stress is stress
    float f2;
    float f3;
    float f4;
} FE_DATA4;

typedef struct
{
    //unsigned flag:4;
    unsigned el_number:30;
    unsigned flag:2;  //0 normal  1 extended, 2 extended twice, 3 extended tripple
    float f1;  //for stress is stress
    float f2;
    float f3;
} FE_DATA3;

typedef struct
{
    float f1;  //for stress is reinforcement ratio
    float f2;
    float f3;
    float f4;
} FE_DATA4_EX;

typedef struct
{
    float f1;  //for stress is reinforcement ratio
    float f2;
    float f3;
} FE_DATA3_EX;

/*
typedef struct
{
    BOOL b_line, b_solid ; 	//narysowano linie, narysowano obszar
    double width1 ;		//szerokosc poczatkowa pierwszego segmentu
    double width ;		//szerokosc bierzaca
    double width0;		//kopia szerokosci poczatkowej pierwszego segmentu
    double axis1 ;        //previous offset
    double axis ;         //current offset
    LINIA line ;
    int color_line ;
    WIELOKAT solid ;
} t_trace ;
*/

/*
typedef struct
{
    BOOL b_line, b_solid ; 	//narysowano linie, narysowano obszar
    double width1 ;		//szerokosc poczatkowa pierwszego segmentu
    double width ;		//szerokosc bierzaca
    double width0;		   //kopia szerokosci poczatkowej pierwszego segmentu
    double axis ;
    LINIA line ;
    int color_line ;
    WIELOKAT solid ;
} t_trace ;
*/

//int solid_pattern_library_no = 0;

//#define SOLID_PATTERN_LIBRARY_MAX_ITEMS 512 //1024

//char *Solid_Pattern_Library[SOLID_PATTERN_LIBRARY_MAX_ITEMS];

/*
#include "o_spec.h"

typedef struct
{
	double Xp;
	double Yp;
	double Xmin;
	double Ymin;
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
	int	orto;
	double df__Point_Size;
	int	b__Point_View;
	double df__Trace_Width;
	double df__Sketch_Dist_Max;
		//l_kr=put_skala_profilu_x(skala_p_x_f);
		//l_kr=put_poziom_pp(rpp_f);
		//local_x=0;
		//local_y=0;
		//angle_l=0;
		//l_kr=put_angle_l(angle_l);
	double local_x;
	double local_y;
	double angle_l;
	int TRANSLUCENCY;
		//if ((TRANSLUCENCY < 77) || (TRANSLUCENCY > 255))  TRANSLUCENCY = 179;
		//set_trans_blender(0, 0, 0, TRANSLUCENCY);
	SEKTORY_EXT sektory_arkusza_ext;
		//l_kr=put_angle_l(angle_l);
	int KursorS;
	BOOL ciagniecie_linii;
	BOOL rozcinanie_linii;
	BOOL ukrywanie_typu;
	BOOL type_view;
	BOOL swobodny_blok;
	BOOL swobodny_tekst;
	OPTIONS1 options1;
	SEL	sel;
	
	//l_kr=put_localx(local_x);
	//l_kr=put_localy(local_y);
	char *dane;
	long dane_size;
	T_spec_name	s__table_spec;
	int	kolory_paper;
	TEXT TextG;
	BOOL Change;
	
	BOOL ctx_created;
    int i_ctx_created;
    BITMAP *ctx_bitmap[32];
} DRAWING_PARAMS;

DRAWING_PARAMS Drawing_Params[MAX_NUMBER_OF_WINDOWS];
*/

#ifdef ARM64
#pragma pack(8)
#endif