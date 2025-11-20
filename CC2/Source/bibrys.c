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

#define __BIBRYS__
#include <forwin.h>
#include "o_graphf.h"
#include <stdio.h>
#include <math.h>
#include <allegext.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "alfaplugin_manager.h"
#include "menu.h"

extern void Rysuj_zbrojenie(void);
extern void Zapamietaj(void);
extern void WezBlok(void);
extern void Import(void);
extern void Import_DXF(void);
extern void Open_Catalog(void);
extern void Export(void);
extern void ImportPCX(void);
extern void WstawPCX(void);
extern void WstawPNG(void);
extern void WstawJPG(void);
extern void VectorizeALX(void);
extern void ImportGEO(void);
extern void Kolo (void);
extern void Explode (void);
extern void Explode_ap (void);
extern void Chamfer (void);
extern void Break (void);
extern void Change_Properties (void) ;
extern void Extend (void) ;
extern void Solid (void) ;
extern void Spline(void);
extern void Solid3D (void);
extern void Rectangle1 (void);
extern void Polygon1 (void) ;
extern void List_Blocks (void) ;
extern void Sketch (void) ;
extern void Set_Block (void) ;
extern void Offset (void) ;
extern void Trim (void) ;
extern void Mirror (void) ;
extern void Mirror_X (void) ;
extern void Mirror_Y (void) ;
extern void Array_Rect (void) ;
extern void Array_Polar_Rot (void) ;
extern void Array_Polar (void) ;
extern void Trace (void) ;
extern void Dline (void) ;
extern void Tline (void) ;
extern void Pline (void) ;
extern void Point (void) ;
extern void Divide_Point (void) ;
extern void Convert_Old_Block (void) ;
extern void Ellipse (void) ;
extern void FilledEllipse (void) ;
extern void EllipticalArc (void) ;
extern void Area (void) ;
extern void Srodek_ciezkosci (void) ;
extern void Moment_statyczny (void) ;
extern void Moment_bezwladnosci (void) ;
extern void Centr_mom_bezwl (void) ;
extern void Test_Area(void) ;
extern void Change_Hatch(void);
#ifdef aster
extern void Baza_Danych (void) ;
extern void Specyfikacja_z_j (void) ;
extern void Specyfikacja_z_p (void) ;
extern void Specyfikacja_z_h (void) ;
extern void Specyfikacja_k_j (void) ;
extern void Specyfikacja_k_p (void) ;
extern void Specyfikacja_k_h (void) ;
extern void Listwy_montazowe (void) ;
extern void Polaczenia_wewn (void) ;
extern void Albumy_kabli (void) ;
extern void Zestawienie_odb (void) ;
extern void Bilans_mocy (void) ;
extern void Spis_dokumentacji (void) ;
#endif
extern void menu_par_new(char * pole, const char * par);   
extern void Usun_bloki_DXF (void);
extern int ask_question (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien);
extern void korekta_obiektow_blokow(void);
extern void zlikwiduj_wszystkie_bloki(void);
extern char *get_address_PCX(int option);
extern int  Get_Vector (int i);
extern void Scale_Point (double k1, double k2, double x1,double y1,double x2,double y2,double *x,double *y);
extern int get_dx_dy_PCX(double *dx, double *dy);
extern int  read_esc(char  *tekst,int lmax,int kom);
extern void set_dx_dy_PCX(double dx, double dy);
extern void Export_Block_PCX (char *adr);
extern int kolorL_bitmap(void);
extern int GetColorAC1(int color);
extern void color_bar(void);
extern int Write_Block_PCX (char *adr);
extern int	getdisk(void);
extern char *Current_Directory (int drive, char *path);
extern int	setdisk(int _drive);
extern void set_scrsave_time (void);
extern int LoadPCX_real(B_PCX *b_pcx, char *nazwa_pcx);
extern int Get_Point (double *coord_x, double *coord_y, int opcja);
extern void set_x_y_PCX(double x, double y);
extern void change_hatch_block_pattern_proc(void);

extern void set_spline_tension(int typ, BOOL set_np);
extern void set_spline_tension_only(int typ);
extern void spline_amend_proc(double x0, double y0);
extern void redcr_spline0(char typ);
extern void Set_Control_Points_View(BOOL b_pv);

extern void Set_Curviness(double n_curviness);

extern void Export_Block_PNG(char *adr);
extern void Export_Block_JPG(char* adr);


extern double Get_Ds_x(void);

extern SPLINE bspline;

extern void undo_pcx_blockzap(void);
extern BOOL get_on_front_PCX(void);
extern void pcx_outlines_view_off(void);
extern int GetPCXParams(PCXheader head, B_PCX *b_pcx, double scale_xf, double scale_yf, double angle_f, BOOL background, BOOL preserve_aspect, BOOL on_top, BOOL h_flip, BOOL v_flip);
extern void layer_info (void);
extern void set_spline_tension_force(int typ);
extern void remove_short_notice(void);
extern void set_plugin_func_in_COMNDm(int poz, void *plugin_f);
extern int get_selection(void);
extern int set_selection(int new_selection);

extern BOOL Check_if_Equal (double x, double y);
extern double Atan2 (double, double) ;
extern void Rotate_Point(double si, double co,
    double x1, double y1,  /*center point*/
    double x2, double y2, double *x, double *y); /*rotate point*/

extern TMENU mInsetAux;

extern BOOL Semaphore; //////!!!
extern char sk1[32];

static void nooop(void)
{
}

static int  nooop1(void)
{
    return 0;
}

void not_implemented (void)
/*------------------------*/
{
  komunikat(12);
}

void not_active (void)
/*------------------*/
{
  komunikat (18) ;
}


/*---------------------------------------------------*/

static TMENU mNie_Tak_={2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmNie_Tak_,NULL,NULL};

TMENU mRysuj={MRYSUJMAX ,0, 0,15 ,1,3,ICONS | TADD ,CMNU,CMBR,CMTX,0,0,0,0,0,&pmRysuj,NULL,NULL};

static void (* COMNDr[])(void)={
Linia, Okrag, Luk, Tekst, Multitekst,
Polygon1, Kolo, nooop, Pline,
Solid, Spline, Rectangle1, Trace, Dline, Tline, Sketch, Point, Vector,
Graph_Function,nooop, Ellipse,FilledEllipse,EllipticalArc, //Rysuj_zbrojenie,
nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,
nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,
nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,
nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,nooop,
};


TMENU mEdycja={MEDITMAX,0,0,18,1,3,ICONS|TADD,CMNU,CMBR,CMTX,0,0,0,0,0,&pmEdycja,NULL,NULL};


static void (*  COMNDe[])(void)=
{
Przesun,Kopiuj,Usun,undo_blok, Obrot, Obrot1, Skala,
ciagnij, Edit_Obiekt,
Mirror, not_implemented, not_implemented,
Break, Trim, Extend,
profilowanie, Chamfer,
Offset, Divide_Point, Global_Undo,
Array_Rect, Array_Polar, Array_Polar_Rot, Mirror_X, Mirror_Y
};

/*--------------------------*/


  TMENU mBlok={MBLOCKMAX, 0, 0, 32, 1, 3, ICONS | TADD, CMNU, CMBR, CMTX, 0, 0, 0, 0,0,&pmBlok, NULL, NULL};


static void Save_Layer_off(void)
{
    strcpy(sk1, _NO_);
  if (options1.save_original_layer==0) return;
  options1.save_original_layer=0;
  menu_par_new ((*mBlok.pola)[17].txt, sk1);
  return;
 }

static void Save_Layer_on(void)
{
 
  strcpy(sk1, _YES_);
  if (options1.save_original_layer==1) return;
  options1.save_original_layer=1;
  menu_par_new ((*mBlok.pola)[17].txt,sk1);
  return;
 }

static void Korekta_blokow(void)
{
  korekta_obiektow_blokow();
  return;
 }

static void Zlikwiduj_wszystkie_bloki(void)
{
  zlikwiduj_wszystkie_bloki();
  return;
 }

static void Usun_blok_PCX(void)
{
 if (bitmap_exist==TRUE)
  {
   bitmap_exist=FALSE;
   redraw();
  }
}

static void Przywroc_blok_PCX(void)
{
 if (bitmap_exist==FALSE)
  {
   bitmap_exist=TRUE;
   redraw();
  }
}

static void Kalibracja_PCX(void)
{ char sk[30]="";
  double dx, dy;
  int k;
  char *adp;
  B_PCX *pcx0, b_pcx;
  double x1,y1,x2,y2,x1p,y1p,x2p,y2p;
  double df_si, df_co;
  double l, l_, DD_xy;
  double kat, kat_, kat__;
  double xp,yp;

  adp=get_address_PCX(0);
  if (adp==NULL)
   {
    return;
   }

  pcx0=(B_PCX *)adp;
  memmove(&b_pcx, pcx0, sizeof(B_PCX));
  //pierwszy wektor - baza
  k=Get_Vector(0);
  if (k)
    {
      x1=LiniaG.x1;
      y1=LiniaG.y1;
      x2=LiniaG.x2;
      y2=LiniaG.y2;
      //drugi wektor - punkty kartometryczne
      k=Get_Vector(1);
      if (k)
       {
        x1p=LiniaG.x1;
        y1p=LiniaG.y1;
        x2p=LiniaG.x2;
        y2p=LiniaG.y2;


        dx = x2 - x1;
        dy = y2 - y1;
        l=sqrt((dx*dx)+(dy*dy));
        if (TRUE == Check_if_Equal (dy, 0))
         {
          kat=(dx>=0 ? 0 : Pi);
         }
          else
           if (TRUE == Check_if_Equal (dx, 0))
            {
              kat=(dy>=0 ? (Pi/2) : (Pi*1.5));
            }
             else
              {
                kat=Atan2(dy,dx);
                if (kat<0) kat+=Pi2;
              }

        dx = x2p - x1p;
        dy = y2p - y1p;
        l_=sqrt((dx*dx)+(dy*dy));
        if (TRUE == Check_if_Equal (dy, 0))
         {
          kat_=(dx>=0 ? 0 : Pi);
         }
          else
           if (TRUE == Check_if_Equal (dx, 0))
            {
              kat_=(dy>=0 ? (Pi/2) : (Pi*1.5));
            }
             else
              {
                kat_=Atan2(dy,dx);
                if (kat<0) kat_+=Pi*2;
              }

        if ((Check_if_Equal(l,0.0)==TRUE) || (Check_if_Equal(l_,0.0)==TRUE))
         {
          ErrList(201);
          return;
         }
        DD_xy=l_/l;
        //skalowanie wzgledem punktu x1, y1
        Scale_Point (DD_xy, DD_xy ,x1, y1, pcx0->x, pcx0->y, &xp, &yp) ;
        pcx0->x = xp ;
        pcx0->y = yp ;
        pcx0->dx *= DD_xy ;
        pcx0->dy *= DD_xy ;
        //obrot wzgledem punktu x1, y1
        kat__=(kat_-kat);
        if (kat__<0) kat__+=Pi2;
        df_si=sin (kat__);
        df_co=cos (kat__);
        Rotate_Point (df_si, df_co ,x1, y1, pcx0->x, pcx0->y, &xp, &yp) ;
        pcx0->x=xp ;
        pcx0->y=yp ;
        pcx0->kat+=kat__;
        if (pcx0->kat >= Pi2) pcx0->kat-=Pi2;
        if (pcx0->kat <= -Pi2) pcx0->kat+=Pi2;
        //przesuniecie
        pcx0->x+=(x1p-x1);
        pcx0->y+=(y1p-y1);
       }
    }
  else
  {
	  undo_pcx_blockzap();
  }


  if ((k) && (memcmp(pcx0, &b_pcx, sizeof(B_PCX))!=0))
   {
     if ((!pcx0->on_front) && (!b_pcx.on_front)) regen_ctx=1;
     redraw();
   }
}

static void Zmien_dx_dy(void)
{ char sk[30]="";
  char *endptr;
  double d;
  double dx, dy;
  int k;

 k=get_dx_dy_PCX(&dx, &dy);
 if (k==0)
  {
    undo_pcx_blockzap();
    return;
  }

 if (bitmap_view==TRUE)
  {
    sprintf(sk, "%g", dx);
	if (!read_esc(&sk, 10, 113))
	{
		undo_pcx_blockzap();
        pcx_outlines_view_off();
		return;
	}
       d=strtod(sk,&endptr);
       if (d<=0)
         { ErrList(16);
	   {
		   undo_pcx_blockzap();
           pcx_outlines_view_off();
		   return;
	   }
         }
       dx=d;
    sprintf(sk, "%g", dx);
	if (!read_esc(&sk, 10, 114))
	{
		undo_pcx_blockzap();
        pcx_outlines_view_off();
		return;
	}
       d=strtod(sk,&endptr);
       if (d<=0)
         { ErrList(16);
	   {
		   undo_pcx_blockzap();
           pcx_outlines_view_off();
		   return;
	   }
         }
       dy=d;

    set_dx_dy_PCX(dx, dy);
    if (!get_on_front_PCX()) regen_ctx = 1;

    redraw();
  }
}


static void export_block_PCX(void)
{ char sk[30]="";

  char *adp;

 adp=get_address_PCX(1);
 if (adp==NULL)
  {
      pcx_outlines_view_off();
      CUR_OFF(X, Y);
      CUR_ON(X, Y);
    return;
  }

 
 if (((B_PCX*)adp)->kod_obiektu == 1) //PCX
    Export_Block_PCX(adp);
 else if (((B_PCX*)adp)->kod_obiektu == 2) //PNG
	Export_Block_PNG(adp);
 else if (((B_PCX*)adp)->kod_obiektu == 3) //JPG
     Export_Block_JPG(adp);

    undo_pcx_blockzap();
    pcx_outlines_view_off();
    CUR_OFF(X, Y);
    CUR_ON(X, Y);

}

static void Change_Params_PCX(void)
{ char sk[30]="";
  char *endptr;
  double d;
  char *adp;
  B_PCX *pcx0, b_pcx;
  int kolor_g, n;
  int dlg_ret;
  PCXheader head;
  double scale_xf, scale_yf, angle_f;
  BOOL preserve_aspect = TRUE;

 //wskazanie bloku PCX
 adp=get_address_PCX(0);
 if (adp==NULL)
  {
    return;
  }
 //zapamietanie parametrow
 pcx0=(B_PCX *)adp;
 memmove(&b_pcx, pcx0, sizeof(B_PCX));
 memmove(&head, &b_pcx.pcx, sizeof(PCXheader));

 //determine scale_x and scale_y
 scale_xf = b_pcx.dx / 25.4 * (double)head.horz_res;
 scale_yf = b_pcx.dy / 25.4 * (double)head.vert_res;
 angle_f = b_pcx.kat / Pi * 180.0;

 if (b_pcx.dx != b_pcx.dy) preserve_aspect = FALSE;

 dlg_ret = GetPCXParams(head, &b_pcx, scale_xf, scale_yf, angle_f, b_pcx.background, preserve_aspect, b_pcx.on_front, b_pcx.h_flip, b_pcx.v_flip);

 if (dlg_ret == 0)
 {
     Cur_offd(X, Y);
	 blokzap(adp, adp, ANieOkreslony, XOR_PUT, 1);
     Cur_ond(X, Y);
	 return;
 }

  b_pcx.inwersja=0;
  b_pcx.ignore_print_background = 1; // 0;
  b_pcx.set_foreground=0;
  b_pcx.background = 15; // 255;
  b_pcx.foreground = 255;

  kolor_g=LiniaG.kolor;

    OkragG.kolor=PointG.kolor=TextG.kolor=LukG.kolor = LiniaG.kolor=kolor_g ;
    Layers[Current_Layer].color=kolor_g;
    color_bar();

    komunikat(0);
 /////////////////
    if (memcmp(pcx0, &b_pcx, sizeof(B_PCX)) != 0)
    {
        //if (pcx0->on_front != b_pcx.on_front)
        regen_ctx = 1;
        Change = TRUE;
    }
 memmove(pcx0, &b_pcx, sizeof(B_PCX));
 redraw();

}


static void Zmien_x_y(void)
{ double x, y;


 if (Get_Point(&x, &y, 0))
  {
    set_x_y_PCX(x, y);
    redraw();
  }
 komunikat0(0);
 komunikat(0);
 return;
}

static void (*COMNDb[])(void)={ Zapamietaj, WezBlok, Export, Import, Open_Catalog, Import_DXF, nooop, nooop, Change_Hatch, export_block_PCX,  Set_Block, Explode,  ciagnij_quad, List_Blocks,
                                Explode_ap , Usun_bloki_DXF, Korekta_blokow, nooop, Save_Layer_on, Save_Layer_off, Zmien_dx_dy, Change_Params_PCX, Kalibracja_PCX, WstawPCX, WstawPNG, WstawJPG, VectorizeALX} ;


TMENU mGeometria={6,0,0,24,1,3,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmGeometria,NULL,NULL};

static void (*  COMNDa[])(void)=
{
Area, Srodek_ciezkosci, Moment_statyczny, Moment_bezwladnosci, Centr_mom_bezwl, Test_Area
};

void add_plugin_function(void *plugin_func, int part)
{
    int poz;

    switch (part)
    {
        case PLUGIN_PART_UNKNOWN:
            break;
        case PLUGIN_PART_DRAW:
            poz=mInsetDraw.off+mInsetDraw.max;
            if (poz<MAXINSETS) COMNDr[poz]=plugin_func;
            break;
        case PLUGIN_PART_EDIT:
            poz=mInsetEdit.off+mInsetEdit.max;
            if (poz<MAXINSETS) COMNDe[poz]=plugin_func;
            break;
        case PLUGIN_PART_BLOCK:
            poz=mInsetBlock.off+mInsetBlock.max;
            if (poz<MAXINSETS) COMNDb[poz]=plugin_func;
            break;
        case PLUGIN_PART_AUX:
            poz=mInsetAux.off+mInsetAux.max;
            if ((poz-COMNDmnr_base)<MAXPINSETS) set_plugin_func_in_COMNDm(poz, plugin_func);
            break;
        default:
            break;
    }
}

/*--------------------------*/

void Rysuj(void)
{ int n;
 Semaphore = FALSE;
 if((n=getwsp1(&mRysuj)-1)>=0) (*COMNDr[n])();    /* n=-1 -> esc */
 LASTFUN=COMNDr[n];
 remove_short_notice();  //in case of running plugins, if forgotten from plugin side
}

/*---------------------------------------------------*/

static void redcr_amd(char typ)
/*---------------------------*/
{
    static void (*CUR_oN)(double, double);
    static void (*CUR_oFF)(double, double);
    static int (*SW[9])(), sel_akt, sel_cur, sel_gor, sel_nr;

    if (typ == 0)
    {
        CUR_OFF(X, Y);
        sel_akt = sel.akt; ////sel.akt = 0;
        sel_cur = sel.cur;
        sel_gor = sel.gor; sel.gor = 0;
        sel_nr = sel.nr;

        layer_info();

        CUR_oFF = CUR_OFF; CUR_OFF = out_sel_off;
        CUR_oN = CUR_ON;   CUR_ON = out_sel_on;
        SW[0] = SERV[73];  SERV[73] = sel_t;
        SW[1] = SERV[81];  SERV[81] = sel_d;

        SW[2] = SERV[58];
        SW[3] = SERV[59];
        SW[4] = SERV[60];
        SW[5] = SERV[61];
        SW[6] = SERV[62];
        SW[7] = SERV[63];
        SW[8] = SERV[64];

        SERV[58] = nooop1;
        SERV[59] = nooop1;
        SERV[60] = nooop1;
        SERV[61] = nooop1;
        SERV[62] = nooop1;
        SERV[63] = nooop1;
        SERV[64] = nooop1;

        komunikat(0);
        komunikat0(141);
        CUR_ON(X, Y);
    }
    else
    {
        CUR_OFF(X, Y);
        CUR_OFF = CUR_oFF;
        CUR_ON = CUR_oN;
        SERV[73] = SW[0];
        SERV[81] = SW[1];
        //     SERV[63]=SW[2];

        SERV[58] = SW[2];
        SERV[59] = SW[3];
        SERV[60] = SW[4];
        SERV[61] = SW[5];
        SERV[62] = SW[6];
        SERV[63] = SW[7];
        SERV[64] = SW[8];

        sel.akt = sel_akt;
        sel.cur = sel_cur;
        sel.gor = sel_gor;
        sel.nr = sel_nr;

        layer_info();

        komunikat(0);
        komunikat0(0);
        CUR_ON(X, Y);
    }
}

static void* obiekt_wybrany(unsigned* typ)
/*---------------------------------------*/
{
    return select_w(typ, NULL);
}

#define SELECT_ENTITY 1
void Change_Vectors(void)
{
    Semaphore = FALSE;
    int cur_selection=get_selection();
    set_selection(SELECT_ENTITY);
    ciagnij();
    set_selection(cur_selection);
    return;
}

void Spline_Amendment(void)
{
    void* ad;
    unsigned typ;
    EVENT* ev;
    double X0, Y0;
    SPLINE* S;
    double delX;

    Semaphore = FALSE;
    redcr_amd(0);
    while (1)
    {
        ev = Get_Event_Point(NULL, &X0, &Y0);

        if (ev->What == evKeyDown)
        {
            if (ev->Number == 0)
            {
                redcr_amd(1);
                return;
            }
            if (ev->Number == ENTER)
            {
                redcr_amd(1);
               
                typ = Bspline /*| Blinia | Bluk | Bokrag | Bkolo | Bwwielokat | Bspline*/;
                if ((ad = obiekt_wybrany(&typ)) != NULL)
                {
                    switch (typ)
                    {
                    case Bspline:
                        if (Layers[((SPLINE*)ad)->warstwa].edit != 0)
                        {
                            S = (SPLINE*)ad;
                            if (S->npts>4)  //multipoint
                            {

                                memcpy(&bspline, S, sizeof(NAGLOWEK) + S->n);
                                
                                set_spline_tension_only(1);

                                set_spline_tension(1, FALSE);
                                set_spline_tension_force(1);

                                redcr_spline0(0);

                                delX= S->xy[S->lp];
                                delX *= 25000.0;
                                delX = delX / Get_Ds_x();

                                delX = sqrt(delX);
                                
                                Set_Curviness((double)S->xy[S->lp]);
                                
                                redcr_spline0(10);


                                Set_Control_Points_View(FALSE);


                                spline_amend_proc(X+delX, Y);

                                CUR_OFF(X, Y);

                                rysuj_obiekt(S, COPY_PUT, 0);

                                //amending original spline
                                S->xy[S->lp] = bspline.xy[bspline.lp];

                                rysuj_obiekt(S, COPY_PUT, 1);

                                CUR_ON(X, Y);

                                set_spline_tension_only(0);

                                redcr_spline0(1);

                                
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            redcr_amd(0);
        }
    }
}

void Edycja(void)
{
    int n;
    add_zbior_2 = 0;
    switch (UNDO_TAB[0].trans)
    {
    case Tprzesuw: strcpy(sk1, _EDIT_[0]);
        break;
    case TprzesuwZ: strcpy(sk1, _EDIT_[1]);
        break;
    case Tobrot:   strcpy(sk1, _EDIT_[2]);
        break;
    case TobrotXZ:   strcpy(sk1, _EDIT_[3]);
        break;
    case TobrotYZ:   strcpy(sk1, _EDIT_[4]);
        break;
    case Tskala:   strcpy(sk1, _EDIT_[5]);
        break;
    case Tmirror:  strcpy(sk1, _EDIT_[6]);
        break;
    case 255:      strcpy(sk1, _EDIT_[7]);
        break;
    default: strcpy(sk1, _EDIT_[8]);
        break;
    }
 
 menu_par_new((*mEdycja.pola)[19].txt,sk1);

 
 if((n=getwsp1(&mEdycja)-1)>=0) 
  {
   memmove(&UNDO_REC, &UNDO_REC_NULL, sizeof(UNDO_TAB_REC));
   Semaphore = FALSE;
   (*COMNDe[n])();    /* n=-1 -> esc */
   LASTFUN=COMNDe[n];
  } 
 add_zbior_2=1;
}
/*---------------------------------------------------*/

void Blok(void)
{ int n;
 add_zbior_2=0;

 if (options1.save_original_layer==0) strcpy(sk1,_NO_); else strcpy(sk1,_YES_); 

 menu_par_new((*mBlok.pola)[17].txt,sk1);
 Semaphore = FALSE;
 if((n=getwsp1(&mBlok)-1)>=0) (*COMNDb[n])();    /* n=-1 -> esc */
 LASTFUN=COMNDb[n];
 add_zbior_2=1;
}
/*---------------------------------------------------*/

void Geometria(void)
{ int n;

 add_zbior_2=0;
 Semaphore = FALSE;
 if((n=getwsp1(&mGeometria)-1)>=0) (*COMNDa[n])();    /* n=-1 -> esc */
 LASTFUN=COMNDa[n];
 add_zbior_2=1;
}
/*---------------------------------------------------*/

#ifdef aster
void Tabele(void)
{ int n;
 add_zbior_2=0;
 if((n=getwsp1(&mTabele)-1)>=0) (*COMNDtab[n])();    /* n=-1 -> esc */
 add_zbior_2=1;
}
#endif
/*---------------------------------------------------*/
#undef __BIBRYS__



