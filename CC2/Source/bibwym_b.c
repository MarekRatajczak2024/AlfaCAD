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


#define kat0 10  /*17*/
#define w0_5 0.5
#define w1 1.0
#define w1_5 1.5
#define w5 5.0
#define w3 3.0

extern int colors2(int old_color);
extern void return_menu_par0 (char *pole, char * par);
extern void setfillstyle_(int pattern, int color);
extern void setcolor(int kolor);
extern void setwritemode( int mode );
extern void Angle_Measure (LINIA *line_gee, LINIA *line_gee1, int *out_kat, int out_angle_ok);
extern int Set_Orto_Dir (LINIA *L, int orto_dir);
extern double Get_Text_Len (TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index, int *t_width, int *t_height, double *matrix_d, int *i_matrix_d);

static int Continue=0,Wst;
static char *ADLr;  /* adres ostatniej linii */
static int Lsw=0;
static double K0_5,K1,K1_5,K5,K3,Kp2, Kp2s;
static int t,b,r,bl,p;
static int ws;
static double X0,Y0,X01,Y01,X02,Y02;
static int WymInter=1;
static int typ_wymiar;
static int wym_luku=0;
static int wym_okregu=0;
static int wym_kata=0;
static int kat_w_now=0;

static LINIA line_g = Ldef  ;

#define TADD 0x04	   /*teksty dodatkowe*/

void Edit_Wym (unsigned , void *);  /* funkcja obslugi edycji wymiarowania */
void new_dim_info (BOOL) ;

static TEXT  T=Tdef;
static LINIA L=Ldef;     //linie ukosne
static LINIA Lr=Ldef;   //linia rozszerzenia
static LINIA Lw=Ldef;   //linia wymiarowa
static LUK Llw=ldef;    //luk wymiarowy
static LINIA Ls=Ldef;   //prawe rozszerzenie linii wymiarowej
static LINIA Ls1=Ldef;  //lewe rozszerzenie linii wymiarowej
static OKRAG  Ko=Kdef;
static WIELOKAT St=Stdef;

 struct KAT {
       double kat;
       double sin;
       double cos;} ;
static struct KAT kat={ 0,0,1};
static struct KAT katr;


 struct W {
		double x2;
		double y2;
	   	double x0;
	   	double y0;
	   	};
struct W w;

 struct Wl {
		   double x;
		   double y;
		   double kat1;
		   double kat2;
		   double r;
		  };
	    struct Wl wl;

enum KIERUNEK {NoRownolegly,Rownolegly};
enum KATKIER {NoNoZnany,NoZnany,Znany};
static int kierunek=Rownolegly;
static int katkier=NoNoZnany;

extern float ttf_width_w;

extern void orto_l(LINIA *L, int *Orto_Dir);

extern void reset_strwyj(void);
extern BOOL get_strwyj(void);

void break_dim_line(NAGLOWEK *nag, int atrybut, int kolor, int blok);

static void nooop(void)
{}

void WymInterClock(int k)
{
	WymInter=k;
}

void WymInterF(int k)
{
  WymInter=k;
  typ_wymiar = Olinia;
  if (NULL == PTRS__Text_Style [zmwym.czcionka] &&
      NULL != PTRS__Text_Style [TextG.czcionka])
  {
    zmwym.czcionka = TextG.czcionka ;
  }
  L.kolor = Ls.kolor = Ls1.kolor = Ko.kolor =
  St.kolor = Lw.kolor = Lr.kolor = zmwym.Lkolor ;
  T.kolor = zmwym.Tkolor ;
  T.czcionka = zmwym.czcionka ;
  T.wysokosc = zmwym.wysokosc ;
  T.width_factor = zmwym.width_factor ;
  T.italics = zmwym.italics ;
  T.bold = zmwym.bold;
  T.width = 0.0;
  T.height = 0.0;
  K0_5=jednostkiplt(w0_5);
  K1=jednostkiplt(w1);
  K1_5=jednostkiplt(w1_5);
  K3=jednostkiplt(w3);
  K5=jednostkiplt(w5);
  Kp2=sqrt(2.0) * K1_5;
  Kp2s = (2.5 / 2.47) * (2.5/1.5) * K1_5;  /*?????????????????*/
  L.obiektt2=Ls.obiektt2=Ls1.obiektt2=
  Ko.obiektt2=St.obiektt2=T.obiektt2=
  Lw.obiektt2=Lr.obiektt2=O2BlockDim;

  L.blok=Ls.blok=Ls1.blok=
  Ko.blok=St.blok=T.blok=
  Lw.blok=Lr.blok=ElemBlok;

  L.obiektt1=Sztywny;
  Lr.obiektt3=O3WymRoz;
  /*   ??????????????????????????*/
  L.typ = Lw.typ = Lr.typ = 32 /*0*/;
  Ls.typ = 32 /*0*/;
  Ls1.typ = 32 /*0*/;
  /*   ??????????????????????????*/
  Continue=0;
  Wst=0;
  katkier=NoNoZnany;
}

#define __BIBWYM_B_
#include "menu.h"

/*--------------------------------------------*/
#include "bibwym1b.c"
/*    zmienne   */
/*--------------------------------------------*/

static TMENU mWymiarujm={7,0,0,18,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmWymiarujm,NULL,NULL};

#undef __BIBWYM_B_

void new_dim_info (BOOL b_draw)
/*----------------------------------*/
{

  setfillstyle_(SOLID_FILL, BKCOLOR);
  bar(maxX/2+1,0,maxX/2+4*ttf_width_w,HEIGHT /*11*//*9*/);
  if (b_draw == TRUE)
  {
    moveto(maxX/2+1,1);
    setcolor(kolory.ink);

    outtext_r(_NEW_);
  }
}

int WymNowy(void)
{

  Continue=0;
  new_dim_info (TRUE) ;
  Wst=0;
  katkier=NoNoZnany;
  if (Lsw==1)
  {
    Lsw=0;
    Ls.warstwa = Current_Layer;
    if(dodaj_obiekt((BLOK*)dane, (void*)&Ls)==NULL) return 0;
    else
    if(WymInter)
    {
      rysuj_obiekt(&Ls, COPY_PUT, 1);
    }
  }
  return 1;
}

static void usun(void)
{ char *ad, *adh, *adp;
  NAGLOWEK *nag;
  int i;

  Wst=0;
 if (Continue==0)return;

  //searchimg for the last segment

  if (Continue==1) {
      adp=dane;
      adh = dane + ((NAGLOWEK *) dane)->n + sizeof(NAGLOWEK) - 1;
      Continue=0;
  }
  else {
      adp=dane;
      adh = dane + ((NAGLOWEK *) dane)->n + sizeof(NAGLOWEK) - 1;
      Continue=0;
  }

  zmien_atrybut(adp,adh,Anormalny,Ablok);
  Cur_offd(X,Y);
  blokzap(adp,adh,Ablok,COPY_PUT,0);
  usun_obiekt((void*)adp);
  Cur_ond(X, Y);
  if (PTRS__Text_Style[zmwym.czcionka]->type == 2) redraw();
  if (Continue==0) new_dim_info (TRUE) ;
  katkier=NoNoZnany;
  Lsw=0;
}

static void  (*COMNDmw[16])(void);

/*--------------------------------------------*/
#include "bibwym2b.c"
/*  katy  */
/*--------------------------------------------*/
/*--------------------------------------------*/
#include "bibwym3.c"
/*  strzalka  */
/*--------------------------------------------*/
/*--------------------------------------------*/
#include "bibwym4.c"
/*  cursor - l,r,t,b, mysz  */
/*--------------------------------------------*/
/*--------------------------------------------*/
#include "bibwym5.c"
/*  punkt  */
/*--------------------------------------------*/


static void (*COMNDmw[16])(void)={
/*0  mWymiarujm 7*/   nooop,(void (*)(void))WymNowy,usun,edwym,zmienne,wymiarowanier,wymiarowaniel,
		       wymiarowanied,wymiarowaniero,
/*8  mKier      13*/   kierunek0,kierunek90,kierunekr,kierunekXk,kierunekX,kierunekkat, kierunektext };

static void redcrLD(char typ)
{
    static void (*CUR_oN)(double, double);
    static void (*CUR_oFF)(double, double);
    static int (*SW[4]) (), KR, KR0, akt, sel_cur;
    static int image_no;
    static void (*RED_Add_M)(void);

    if (typ == 0)
    {
        Cur_offd(X, Y);
        REDRAW_ADD_MENU();
        RED_Add_M = REDRAW_ADD_MENU; REDRAW_ADD_MENU = nooop;
        KR = Komunikat_R;
        KR0 = Komunikat_R0;
        komunikat(0);
        komunikat0(145);
        akt = sel.akt; sel.akt = ASel;
        sel_cur = sel.cur; sel.cur = NCSel;
        CUR_oFF = CUR_OFF;     CUR_OFF = cursel_off;
        CUR_oN = CUR_ON;       CUR_ON = cursel_on;
        SW[0] = SERV[73];    SERV[73] = sel_t;
        SW[1] = SERV[81];    SERV[81] = sel_d;
        Cur_ond(X, Y);
    }
    else
    {
        if (typ == 1) Cur_offd(X, Y);
        CUR_OFF = CUR_oFF;
        CUR_ON = CUR_oN;
        SERV[73] = SW[0];
        SERV[81] = SW[1];
        sel.akt = akt;
        sel.cur = sel_cur;
        komunikat(KR);
        Komunikat_R0 = KR0;
        REDRAW_ADD_MENU = RED_Add_M;
        REDRAW_ADD_MENU();
        Cur_ond(X, Y);
    }
}


static void add_leader(TEXT *leader_text, int to_block, int from_dxf, int vertex_no, POINTD *vertices, int line_no, BOOL mleader_text_location, double et_x, double et_y, double mtext_h)
{
    PLINIA PL;
    LINIA L1, L2;
    int Wst0;
    double kat1,kat11,n,l;
    BLOK B=Bdef;
    BLOK *nb;
    double t_len_mm;
    int t_width, t_height;
    double direction;
    char *ptr_n;
    double r2;
    int i;

    if (from_dxf==0) CUR_OFF(X, Y);

    L1.x1 = line_g.x1;
    L1.y1 = line_g.y1;
    L1.x2 = line_g.x2;
    L1.y2 = line_g.y2;

    if (orto) {
    Orto_Dir = Set_Orto_Dir(&line_g, Orto_Dir);
    orto_l(&L1, &Orto_Dir);
    parametry_lini(&L1, &PL);
    direction=line_g.x2-L1.x2;
    if (direction==0) direction=L1.x2-L1.x1;
    }
    else
    {
        parametry_lini(&L1, &PL);
        direction=line_g.x2-line_g.x1;
    }

    kat1=PL.kat;

    kat11=0.0;

    //block header
    B.n=B3+sizeof(kat11);
    B.kod_obiektu=B_DIM3;   //B_DIM1 diameter. B_DIM2 radius, B_DIM3 leader
    B.dlugosc_opisu_obiektu=sizeof(kat11);

    if (to_block==0) {
        if ((nb = (BLOK *) dodaj_obiekt(NULL, &B)) == NULL) return;
    }
    else
    {
        if ((nb = (BLOK *) dodaj_obiekt((BLOK*)dane, &B)) == NULL) return;
    }
    memmove(&(nb->opis_obiektu[0]),&kat11,sizeof(kat11));

    double mh;
    if (mtext_h==0) mh=1.0;
    else mh=mtext_h/2.5;

    r2 = mh * 2.5; //1.5;

    K1_5=mh * jednostkiplt(w1_5);
    Kp2=sqrt(2.0) * K1_5;
    Kp2s = (2.5 / 2.47) * (2.5/1.5) * K1_5;  /*?????????????????*/

    katkat(PL.kat);

    if (L1.x2<L1.x1) {
        r2 *= -1;
    }
    else if (L1.x2==L1.x1) {
        if (L1.y2 < L1.y1) r2 *= -1;
    }


    //leading line
    if (from_dxf==0) Lw.warstwa = Current_Layer;

    Lw.x1 = L1.x1 + r2 * kat.cos;
    Lw.y1 = L1.y1 + r2 * kat.sin;
    Lw.x2 = L1.x2;
    Lw.y2 = L1.y2;

    Lw.obiektt2 = O2BlockDim;
    Lw.obiektt3 = O3WymRoz;
    if (Lw.x1 != Lw.x2 || Lw.y1 != Lw.y2) {
        PTR__GTMP7 = nb;
        if (dodaj_obiekt(nb, &Lw) == NULL) return;
        else rysuj_obiekt(&Lw, COPY_PUT, 1);
        nb = PTR__GTMP7;
    }

    Lw.x1 = L1.x1;
    Lw.y1 = L1.y1;

    n=-1;
    if (from_dxf==0) St.warstwa=Current_Layer;
    else St.warstwa=Lw.warstwa;
    St.kolor=Lw.kolor;
    St.xy[2]=line_g.x1;
    St.xy[3]=line_g.y1;
    St.xy[0]=line_g.x1-n*Kp2s*cos(Pi*(kat1-kat0)/180);
    St.xy[1]=line_g.y1-n*Kp2s*sin(Pi*(kat1-kat0)/180);
    St.xy[4]=line_g.x1-n*Kp2s*cos(Pi*(kat1+kat0)/180);
    St.xy[5]=line_g.y1-n*Kp2s*sin(Pi*(kat1+kat0)/180);
    St.lp = 6;
    St.n = 8 /*4*/ + St.lp * sizeof (float) ;
    St.obiektt2 = O2BlockDim;
    St.obiektt3 = O3NoWymRoz;
    PTR__GTMP7=nb;
    if(dodaj_obiekt(nb,&St)==NULL) return;
    else rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;
    nb=PTR__GTMP7;


    if ((from_dxf==1) && (vertex_no>2)) //possibly another vectors
    {
        Lw.x1 = Lw.x2;
        Lw.y1 = Lw.y2;
        for (i=2; i<vertex_no; i++)
        {
            Lw.x2 = vertices[i].x;
            Lw.y2 = vertices[i].y;

            Lw.obiektt2 = O2BlockDim;
            Lw.obiektt3 = O3NoWymRoz;
            if (Lw.x1 != Lw.x2 || Lw.y1 != Lw.y2) {
                PTR__GTMP7 = nb;
                if (dodaj_obiekt(nb, &Lw) == NULL) return;
                else rysuj_obiekt(&Lw, COPY_PUT, 1);
                nb = PTR__GTMP7;
            }
            Lw.x1 = Lw.x2;
            Lw.y1 = Lw.y2;
        }
        L1.x2 = Lw.x2;
        L1.y2 = Lw.y2;
    }
    //leading text
    //checking for \n sequences
    T.multiline=0;
    strcpy(T.text, leader_text->text);
    ptr_n=strstr(T.text,"\\n");
    while (ptr_n!=NULL)
    {
        *ptr_n='\r';
        *(ptr_n+1)='\n';
        T.multiline=1;
        ptr_n=strstr(T.text,"\\n");
    }
    if (from_dxf==0) T.warstwa=Current_Layer;
    else T.warstwa=leader_text->warstwa;
    if (from_dxf==0) T.wysokosc=zmwym.wysokosc;
    else T.wysokosc=leader_text->wysokosc;
    if (from_dxf==0) T.width_factor=zmwym.width_factor;
    else T.width_factor=leader_text->width_factor;
    T.kat=0.0;
    T.dl=strlen(T.text);
    T.n=T18+T.dl;
    T.width = 0.0;
    T.height = 0.0;
    T.obiektt2 = O2BlockDim;
    T.obiektt3 = O3NoWymRoz;
    //setting text length
    t_len_mm = Get_Text_Len(&T, T.text, 1.0, 0, &t_width, &t_height, NULL, NULL);

    if ((line_no!=0) || (mleader_text_location==FALSE)) {
        if (direction > 0) //to the right
        {
            T.x = L1.x2 + T.wysokosc / 3.0;
            T.y = L1.y2 + T.wysokosc / 3.0;
            T.justowanie = j_do_lewej;

            if (t_len_mm == 0.0) Lw.x2 = L1.x2 + 0.01;
            else Lw.x2 = L1.x2 + t_len_mm + T.wysokosc / 2.5;
        } else  //to the left
        {
            T.x = L1.x2 - T.wysokosc / 3.0;
            T.y = L1.y2 + T.wysokosc / 3.0;
            T.justowanie = j_do_prawej;
            if (t_len_mm == 0.0) Lw.x2 = L1.x2 - 0.01;
            else Lw.x2 = L1.x2 - t_len_mm - T.wysokosc / 2.0;
        }
    }
    else
    {
        T.x=et_x;
        T.y=et_y;
    }

    //landing line
    Lw.x1 = L1.x2;
    Lw.y1 = L1.y2;
    Lw.y2 = Lw.y1;
    Lw.obiektt2 = O2BlockDim;
    Lw.obiektt3 = O3NoWymRoz;
    if (Lw.x1 != Lw.x2 || Lw.y1 != Lw.y2) {
        PTR__GTMP7=nb;
        if (dodaj_obiekt(nb, &Lw) == NULL) return;
        else rysuj_obiekt(&Lw, COPY_PUT, 1);
        nb=PTR__GTMP7;
    }

        PTR__GTMP7 = nb;
        if (dodaj_obiekt(nb, (void *) &T) == NULL) return;
        else rysuj_obiekt(&T, COPY_PUT, 1);
        nb = PTR__GTMP7;

    if (from_dxf==0) {
        CUR_OFF(X, Y);
        CUR_ON(X, Y);
    }
}

static void outarrowhead(void)
{
    PLINIA PL;
    LINIA L1;
    int Wst0;
    double kat1,n,l;

    L1.x1 = line_g.x1;
    L1.y1 = line_g.y1;
    L1.x2 = line_g.x2;
    L1.y2 = line_g.y2;

    if (orto) {
        Orto_Dir = Set_Orto_Dir(&line_g, Orto_Dir);
        orto_l(&L1, &Orto_Dir);
        parametry_lini(&L1, &PL);
    }
    else parametry_lini(&L1, &PL);

    kat1=PL.kat;

    n=-1;

    St.warstwa=Current_Layer;
    St.xy[2]=L1.x1;
    St.xy[3]=L1.y1;
    St.xy[0]=L1.x1-n*Kp2s*cos(Pi*(kat1-kat0)/180);
    St.xy[1]=L1.y1-n*Kp2s*sin(Pi*(kat1-kat0)/180);
    St.xy[4]=L1.x1-n*Kp2s*cos(Pi*(kat1+kat0)/180);
    St.xy[5]=L1.y1-n*Kp2s*sin(Pi*(kat1+kat0)/180);

    St.lp = 6;
    St.n = 8 /*4*/ + St.lp * sizeof (float) ;

    okno_r();
    rysuj_obiekt_ ((char *)&St, COPY_PUT, 1) ;
    okno_all();

}

static void  leader_cur_on(double x, double y)
/*--------------------------------------*/
{
    float s_dx, s_dy;

    line_g.x2 = x;
    line_g.y2 = y;

    line_g.kolor = Get_Current_Color();
    outlineor(&line_g, XOR_PUT, 0);
    outarrowhead();
    cursel_on(x, y);
     if(sel.akt && sel.akw) out_sel_on(x,y);

    X = x;
    Y = y;
}


static void redcrLD1(char typ)
{
    static void (*CUR_oN)(double, double);
    static void (*CUR_oFF)(double, double);
    static int KR0;
    if (typ == 0)
    {
        KR0 = Komunikat_R0;
        komunikat0(146);
        Cur_offd(X, Y);
        CUR_oFF = CUR_OFF;  CUR_OFF = cursel_off;
        CUR_oN = CUR_ON;   CUR_ON = leader_cur_on;
        Orto_Dir = I_Orto_NoDir;
        Cur_ond(X, Y);
    }
    else
    {
        CUR_OFF = CUR_oFF;
        CUR_ON = CUR_oN;
        Komunikat_R0 = KR0;
        komunikat0(Komunikat_R0);
    }
}

void  dim_leader_second_point(double X0, double Y0)
/*------------------------------------------------*/
{
    EVENT* ev;

    line_g.x1 = X0;
    line_g.y1 = Y0;
    line_g.x2 = X;
    line_g.y2 = Y;
    redcrLD1(0);
    while (1)
    {
        ev = Get_Event_Point(NULL, &X0, &Y0);
        if (ev->What == evKeyDown && ev->Number == 0)
        {
            redcrLD1(1);
            CUR_OFF(X, Y);
            CUR_ON(X, Y);
            return;
        }
        if (ev->What == evKeyDown && ev->Number == ENTER)
        {

            redcrLD1(1);

            //get text
            edit_functions = TRUE;

            strcpy(TextG.text, "");
            if (!get_string(&TextG.text[0], "", MaxTextLen, 0, 13))
            {
                redcrLD1(1);
                edit_functions = FALSE;
                return;
            }
            edit_functions = FALSE;

            //generating leader block
            add_leader(&TextG, 0, 0, 0, NULL, -1, FALSE, 0,0,0);

            CUR_OFF(X, Y);
            CUR_ON(X, Y);

            return;
        }
    }
}


void generate_leader(POINTD *vertices, int vertex_no, LINIA *sample_line, TEXT *sample_text, int to_block, int line_no, BOOL mleader_text_location, double et_x, double et_y, double mtext_h)
{
    int orto_bak;

    if (vertex_no<2) return;

    WymInterF(1);

    orto_bak=orto;
    orto=0;
    Lw.warstwa=sample_line->warstwa;
    Lw.kolor=sample_line->kolor;
    line_g.x1 = vertices[0].x;  //first vector
    line_g.y1 = vertices[0].y;
    line_g.x2 = vertices[1].x;
    line_g.y2 = vertices[1].y;
    add_leader(sample_text, to_block, 1, vertex_no, vertices, line_no, mleader_text_location, et_x, et_y, mtext_h);
    orto=orto_bak;
}


void  wymiarowanie(void)
{
  EVENT *ev;
  double X0, Y0;
    WIELOKAT *wSt;
    LINIA *wL;
    PLINIA PL;
    double kat1,n;
    double wL_x1, wL_y1, r2;
    int ret;

  CUR_OFF(X, Y);

  WymInterF(1);
  redcr(0);

  while(1)
  {
      reset_strwyj();
      if (wym_kata==1) Komunikat_R0=168;
      else Komunikat_R0=6;
      komunikat0(Komunikat_R0);

      ev = Get_Event_Point(NULL, &X0, &Y0);
      CUR_OFF_ON();
      switch (ev->What) {
          case evKeyDown :
              if (ev->Number == 0) {
                  if (b || bl || t || r || p) {
                      //transformacja_blok(ADP, ADK, x01, y01, k01, k02, f0, 0);
                      edwym();
                      CUR_OFF_ON();
                      break;
                  } else {
                      redcr(1);
                      return;
                  }
              }
              if ((ev->Number == ENTER) || (get_strwyj() == TRUE))
              {
                  if ((b || bl || t || r || p)) // && (do_not_dim_DIM==1)) //&& (sel.akt==1) && (sel.gor==1))
                  {
                      transformacja_blok(ADP, ADK, x01, y01, k01, k02, f0, 0);

                      wL = (LINIA *) PTR__GTMP6;
                      //wSt = (WIELOKAT *) (PTR__GTMP6 + wL->n + sizeof(NAGLOWEK));
                      if (PTR__GTMP7 != NULL) {
                          wSt = (WIELOKAT *) (PTR__GTMP7);

                          parametry_lini(wL, &PL);
                          kat1 = PL.kat;
                          n = -1;

                          katkat(PL.kat);

                          r2 = -2.5; //-1.5;

                          if (wL->x2 < wL->x1) {
                              r2 *= -1;
                          } else if (wL->x2 == wL->x1) {
                              if (wL->y2 < wL->y1) r2 *= -1;
                          }

                          wL_x1 = wL->x1 + r2 * kat.cos;
                          wL_y1 = wL->y1 + r2 * kat.sin;;

                          wSt->xy[2] = wL_x1;
                          wSt->xy[3] = wL_y1;
                          wSt->xy[0] = wL_x1 - n * Kp2s * cos(Pi * (kat1 - kat0) / 180);
                          wSt->xy[1] = wL_y1 - n * Kp2s * sin(Pi * (kat1 - kat0) / 180);
                          wSt->xy[4] = wL_x1 - n * Kp2s * cos(Pi * (kat1 + kat0) / 180);
                          wSt->xy[5] = wL_y1 - n * Kp2s * sin(Pi * (kat1 + kat0) / 180);
                      }

                      edwym();
                      //zmien_atrybut(ADP,ADK,Ablok,Anormalny);
                      CUR_OFF_ON();
                      break;
                  } else {
                      if (typ_wymiar != Olinia) {
                          typ_wymiar = Olinia;
                          WymNowy();
                      }
                      if (sel.wyj || sel.gor) {
                          new_dim_info(FALSE);
                          if (wym_kata == 2) {
                              //Dim_Leader();
                              dim_leader_second_point(X0, Y0);
                          } else punktsel(X0, Y0);
                      } else {
                          punkt();

                          if ((!b && !bl && !t && !r && !p))
                              if (wym_kata == 2) {
                                  dim_leader_second_point(X0, Y0);
                              }
                      }
                  }
              }
              CUR_OFF_ON();
              break;
          case evCommandP :
              return_back == 0;
              (*COMNDmw[ev->Number])();
              if (return_back != 0) redraw();
              break;
          default :
              if (b || bl || t || r) {
                  edwym();
                  CUR_OFF_ON();
                  break;
              }
              break;
      }
 }
}


int Get_wym_kata(void)
{
	return wym_kata;
}

void Set_wym_kata(int i_kat)
{
	wym_kata=i_kat;
}

