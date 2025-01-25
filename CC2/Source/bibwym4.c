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

#define __BIBWYM4__
#include "menu.h"
static BLOK * lps_edit_block = NULL ;
static BOOL b__edit_current_block ;
static float xr, yr, rad;
static double X00,Y00;
static double sxy_;

static float x01=0, y01=0, k01=0, k02=0;
int f0=0;

extern BOOL TTF_redraw;
extern BOOL text_edit_dialog;
extern int ttf_digits27_len;
extern void outlinetext(TEXT *t, QUAD *outline, double margin);
extern void select_color_type(char *ad);
extern void select_color_paper_type(char *ad);

extern char last_buf[MaxSizeObiekt];

extern void out_blok2 (double x,double y,double k1,double k2,
                double xa, double ya, double ka1, double ka2, int trans, double z, BOOL update_par, int no_object);

static void near Katp(LINIA *llead)
{
    PLINIA pl;

    parametry_lini(llead, &pl);
    katr.kat = pl.kat+90;
    katr.sin = sin(katr.kat*Pi / 180);
    katr.cos = cos(katr.kat*Pi / 180);
}


static void near Katr(void)
{
  float kat1;
  T_Desc_Ex_Block *ptrs_desc_bl;

  if (lps_edit_block == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return ;
  }

  if (lps_edit_block->dlugosc_opisu_obiektu==sizeof(float))
      memmove(&kat1,&(lps_edit_block->opis_obiektu),sizeof(kat1));  //it's ok for linear dim
  else
  {
	  ptrs_desc_bl = (T_Desc_Ex_Block *)(&lps_edit_block->opis_obiektu[0]);
	  memmove(&kat1, (char *)(lps_edit_block->opis_obiektu + lps_edit_block->dlugosc_opisu_obiektu-sizeof(float)), sizeof(kat1));
  }
  katr.kat = kat1;
  katr.sin=sin(katr.kat*Pi/180);
  katr.cos=cos(katr.kat*Pi/180);
}

static void near KatrL(void)
{
	float kat1;
	LINIA *laux;
	PLINIA pl;

	if (lps_edit_block == NULL)
	{
		Internal_Error(__LINE__, __FILE__);
		return;
	}
	laux = ((LINIA *)ADP);
	parametry_lini(laux, &pl);
	katr.kat = pl.kat+90;
	katr.sin = sin(katr.kat*Pi / 180);
	katr.cos = cos(katr.kat*Pi / 180);
}

static void near parlr(void)
{
  PLINIA PL;
  LINIA *adrl;
  adrl=(LINIA *)ADLr;
  parametry_lini(adrl,&PL);
  X01=adrl->x2;
  Y01=adrl->y2;
  X0=X01-K1_5*PL.cos;
  Y0=Y01-K1_5*PL.sin;
  X02=X01-2*K1_5*PL.cos;
  Y02=Y01-2*K1_5*PL.sin;
  ws=1;
}

static void near korektalr(void)
{ double y1,y2;
  LINIA  *adrl;
  adrl=(LINIA *)ADLr;
  y1=-(adrl->x1-X0)*katr.sin+(adrl->y1-Y0)*katr.cos;
  y2=-(adrl->x2-X0)*katr.sin+(adrl->y2-Y0)*katr.cos;
  if (y1*y2>0)
   if(ws==1)
    { adrl->x2=X02;
      adrl->y2=Y02;
      ws=2;
    }
   else
    { adrl->x2=X01;
      adrl->y2=Y01;
      ws=1;
    }
}

/*---------------------------------------------------------------*/

void break_dim_line(NAGLOWEK *nag, int atrybut, int kolor, int blok)
{
    LINIA Lbuf, *L, *Ld=NULL;
    QUAD t_outline;
    int ret;
    LINIA L_tmp, L_tmp1;
    LUK l_tmp, l_tmp1;
    int grubosc_l, typ_l;
    char *adp,*adk;
    NAGLOWEK *ad;
    TEXT *t=NULL;
    LUK lbuf, *l, *ld=NULL;


    if (nag->atrybut!=atrybut) return;

    //checking if text and it is dimensioning text
    if ((nag->obiekt==Otekst) && (nag->blok == 1) && (nag->obiektt1 == 0) && (nag->obiektt2 == 1) && (nag->obiektt3 == 0))
    {//searching for dim line
        t = nag;
        adp= FIRSTB(nag);
        if (adp!=NULL)
        {
            adk=(char*)adp+sizeof(NAGLOWEK)+((BLOK*)adp)->n;
            ad=(char*)adp+B3+sizeof(NAGLOWEK)+((BLOK*)adp)->dlugosc_opisu_obiektu;
            while (ad<t)
            {
                if ((ad->obiekt==Olinia) && (ad->blok == 1) && (ad->obiektt1 == 0) && (ad->obiektt2 == 1) && (ad->obiektt3 == 0))
                {
                    Ld = ad;
                }
                else if ((ad->obiekt==Oluk) && (ad->blok == 1) && (ad->obiektt1 == 0) && (ad->obiektt2 == 1) && (ad->obiektt3 == 0))
                {
                    ld = ad;
                }
                if (ad->obiekt==OdBLOK) ad=(char*)ad+B3+sizeof(NAGLOWEK)+((BLOK*)ad)->dlugosc_opisu_obiektu;
                ad=(char*)ad + sizeof(NAGLOWEK) + ad->n;
            }
        }

    }

    else if ((blok) && (nag->obiekt==OdBLOK) && ((((BLOK*)nag)->kod_obiektu==B_DIM) || (((BLOK*)nag)->kod_obiektu==B_DIM1) || (((BLOK*)nag)->kod_obiektu==B_DIM2) || (((BLOK*)nag)->kod_obiektu==B_DIM3)))
    {
        adk=(char*)nag+sizeof(NAGLOWEK)+((BLOK*)nag)->n;
        //adk=ADK;
        ad=(char*)nag+B3+sizeof(NAGLOWEK)+((BLOK*)nag)->dlugosc_opisu_obiektu;
        while (ad<adk)
        {
            if ((ad->obiekt==Olinia) && (ad->blok == 1) && (ad->obiektt1 == 0) && (ad->obiektt2 == 1) && (ad->obiektt3 == 0))
            {
                Ld = ad;
            }
            else if ((ad->obiekt==Oluk) && (ad->blok == 1) && (ad->obiektt1 == 0) && (ad->obiektt2 == 1) && (ad->obiektt3 == 0))
            {
                ld = ad;
            }
            else if ((ad->obiekt==Otekst)  && (ad->atrybut == atrybut) && (ad->blok == 1) && (ad->obiektt1 == 0) && (ad->obiektt2 == 1) && (ad->obiektt3 == 0))
            {
                t = ad;
                break;  //text is always the last one
            }
            if (ad->obiekt==OdBLOK) ad=(char*)ad+B3+sizeof(NAGLOWEK)+((BLOK*)ad)->dlugosc_opisu_obiektu;
            ad=(char*)ad + sizeof(NAGLOWEK) + ad->n;
        }
    }

    if ((Ld!=NULL) && (t!=NULL))
    {
        //checking crossing with text
        outlinetext(t, &t_outline, 0.5);
        Normalize_Quad (&t_outline);

        ret=trim_line_to_quad(Ld, &t_outline, &L_tmp, &L_tmp1);
        ////
        if (ret>=0) {


            if (kolor==1) {

                memcpy(&Lbuf, Ld, sizeof(LINIA));

                grubosc_l = (Lbuf.typ & 224) / 32;
                typ_l = (Lbuf.typ & 31);
                if (grubosc_l < 3) grubosc_l = 3;
                else grubosc_l += 1;
                if (grubosc_l > 4) grubosc_l = 4;
                Lbuf.typ = grubosc_l * 32 + typ_l;

                rysuj_obiekt(&Lbuf, COPY_PUT, 0);

                select_color_type(Ld);
            }
            else select_color_paper_type(Ld);

            Set_Second_Screen();
            okno_r_second();

            switch (ret) {

                case 0:
                    Ld->widoczny = lineC(pikseleX0(Ld->x1), pikseleY0(Ld->y1), pikseleX0(Ld->x2), pikseleY0(Ld->y2));
                    break;
                case 1:
                    Ld->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),
                                         pikseleY0(L_tmp.y2));
                    break;
                case 2:
                    Ld->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),
                                         pikseleY0(L_tmp.y2));
                    Ld->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),
                                         pikseleY0(L_tmp1.y2));
                    break;
                default:
                    break;
            }


            okno_all_second();
            Set_Screen();
        }
    }
    else if ((ld!=NULL) && (t!=NULL))
    {
        //checking crossing with text
        outlinetext(t, &t_outline, 0.5);
        Normalize_Quad (&t_outline);

        ret=trim_arc_to_quad(ld, &t_outline, &l_tmp, &l_tmp1);

        if (ret>=0) {

            if (kolor==1) {
                memcpy(&lbuf, ld, sizeof(LUK));

                grubosc_l = (lbuf.typ & 224) / 32;
                typ_l = (lbuf.typ & 31);
                if (grubosc_l < 3) grubosc_l = 3;
                else grubosc_l += 1;
                if (grubosc_l > 4) grubosc_l = 4;
                lbuf.typ = grubosc_l * 32 + typ_l;

                rysuj_obiekt(&lbuf, COPY_PUT, 0);

                select_color_type(ld);
            }
            else select_color_paper_type(ld);

            Set_Second_Screen();
            okno_r_second();

            switch (ret)
            {
                case 0:
                    //L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2),pikseleY0(L->y2));
                    DrawArc(pikseleX0(ld->x),pikseleY0(ld->y),ld->kat1,ld->kat2,pikseleDX(ld->r), COPY_PUT);
                    break;
                case 1: //L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                    DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), COPY_PUT);
                    break;
                case 2:// L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                    DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), COPY_PUT);
                    // L->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                    DrawArc(pikseleX0(l_tmp1.x),pikseleY0(l_tmp1.y),l_tmp1.kat1,l_tmp1.kat2,pikseleDX(l_tmp1.r), COPY_PUT);
                    break;
                default:
                    break;
            }

            okno_all_second();
            Set_Screen();
        }
    }

}

static void near ruchblokw(void)
{
    PTR__GTMP6=ADP;
    PTR__GTMP7=NULL;
    break_dim_line(ADP, Ablok, 0, 0);
    blokzap(ADP,ADK,Ablok,COPY_PUT,0);
    CUR_OFF_ON();
    BlokM=1;
}

static void near noruchblokw(void)
{ NAGLOWEK *nag;

    ////Cur_offd(X,Y);
    blokzap(ADP,ADK,Ablok,COPY_PUT,0);
    ////if (PTR__GTMP6 !=NULL) nag=(NAGLOWEK*)PTR__GTMP6;
    ////else nag=(NAGLOWEK*)ADP;

    ////break_dim_line(nag, Ablok, 1, 0);

    blokzap(ADP,ADK,Ablok,COPY_PUT,1);

    //CUR_OFF_ON();

  BlokM=0;
}

#define r40 20
static ESTR eT;
static int np;

extern void outtextxy_wP_(TEXT  *t,int mode);
extern void outtextxy_wP0_(TEXT  *t);

static int W_t (BOOL b_graph_value)
{
  TEXT Tp, *t;
  char *ptr_n;

  ADP=ADK=PTR__GTMP6;  //just for case if there was refresh

  b_graph_value = b_graph_value ;
  memmove((void*)&T,(void*)ADP,sizeof(NAGLOWEK)+T18-1);
  memmove(&Tp,(void*)ADP,sizeof(NAGLOWEK)+((TEXT *)ADP)->n);
  strcpy(&T.text [0] ,eT.st);

  T.multiline=0;
  ptr_n=strstr(T.text,"\\n");
  while (ptr_n!=NULL)
  {
    *ptr_n='\r';
    *(ptr_n+1)='\n';
    T.multiline=1;
    ptr_n=strstr(T.text,"\\n");
  }

  T.dl=strlen(T.text);
  T.n=T18+T.dl;
  T.width = 0.0;
  T.height = 0.0;
  T.atrybut=Anormalny;

  //updating parameters
  if (text_edit_dialog == TRUE)
  {
      T.italics = TextG.italics;
      T.bold = TextG.bold;
      T.justowanie = TextG.justowanie;
      T.underline = TextG.underline;
  }

  if ((t=korekta_obiekt((void*)ADP,(void*)&T))==NULL) return 0;


  ADP=ADK=(char *)t;
  Set_Second_Screen();
  okno_r_second();
  outtextxy_wP0_(&Tp);

    okno_all_second();
    Set_Screen();

    break_dim_line(t, Anormalny, 1, 1);

    Set_Second_Screen();
    okno_r_second();

  outtextxy_w(&T,COPY_PUT);
  okno_all_second();
  Set_Screen();

  return 1;
}

void Edit_Wym (unsigned type_sel, void *ptr_sel)  /* funkcja obslugi edycji wymiarowania */
/*----------------------------------------------*/
{
  unsigned typ;
  void  *adr, *adrw;
  WIELOKAT *w;
  char *ptr_n;
  char buf [MaxTextLen*2];

  if (!(b+bl+r+p+t))
  {
     if (ptr_sel != NULL)
     {
       adr = (void  *)ptr_sel ;
       lps_edit_block = Parent_Block ((char  *)adr) ;
       typ = type_sel ;
       if (typ == (Blinia  && (((LINIA*)adr) -> obiektt3 == O3NoWymRoz)))
       {
           //checking if second point is in the scope
           if (punkt_w_prostokacie(((LINIA*)adr)->x2, ((LINIA*)adr)->y2)) typ=Bpoint;
           else typ=Bblok;

           X00=X;
           Y00=Y;
       }
       else if (typ == Bkolo || typ == Bwwielokat )
       {
	     typ = Bblok;
       }
     }
     else
     {
       typ = Blinia | Btekst | Bluk /*| Bokrag | Bkolo*/ ;
       adr = select_edwym (&typ) ;
       lps_edit_block = LASTb ;
     }
     if (adr == NULL)
     {
       return ;
     }
     switch(typ)
      { case Blinia : ADP = ADK = (char  *)adr ;
		      ADLr = (char*)ADP ;
		      parlr();

              X00=X;
              Y00=Y;

		      zmien_atrybut(ADP,ADK,Anormalny,Ablok);
		      r=1; //ruchblokw();
		      ((LINIA *)adr)->obiektt1=Utwierdzony2;

              if (lps_edit_block->kod_obiektu == B_DIM3)  //leader
              {
                  adrw=(char *)adr+((LINIA *)adr)->n+sizeof(NAGLOWEK);
                  if ((((NAGLOWEK*)adrw)->obiekt==Owwielokat) && (((NAGLOWEK*)adrw)->blok==1) && (((NAGLOWEK*)adrw)->obiektt2==1))
                  {
                      ADK=ADP + ((LINIA *)adr)->n+sizeof(NAGLOWEK);
                      ((NAGLOWEK*)adrw)->atrybut=Ablok;
                  }
              }

              ruchblokw();

			  if (lps_edit_block->kod_obiektu==B_DIM) Katr();
              else if (lps_edit_block->kod_obiektu == B_DIM2) Katr();
              else if (lps_edit_block->kod_obiektu == B_DIM3)  Katp((LINIA *)adr);
		      else if (lps_edit_block->kod_obiektu == B_DIM1) KatrL();
		      break;
	  case Bluk:

			  if (lps_edit_block->kod_obiektu != B_DIM1)
			  {
				  ////out_sel(X, Y);
				  return;
			  }
			  ADP = (char *)lps_edit_block;
			  ADK = ADP + sizeof(NAGLOWEK) + ((BLOK *)ADP)->n - 1;

			  xr=((LUK *)adr)->x;
			  yr = ((LUK *)adr)->y;
			  rad = ((LUK *)adr)->r;

              //X0=X;
              //Y0=Y;

              X00=0;
              Y00=0;

			  zmien_atrybut(ADP, ADK, Anormalny, Ablok);
			  b__edit_current_block = FALSE;
			  if (ADP == dane)
			  {
				  b__edit_current_block = TRUE;
			  }
			  bl = 1; ruchblokw();
			  Katr();
			  sxy_ = 1.0;
			  break;
	case Btekst : ADP=ADK=adr;

              X00=X;
              Y00=Y;

		      zmien_atrybut(ADP,ADK,Anormalny,Ablok);
		      t=1; ruchblokw();
		      if (-1 != (np=dodajstr(&eT)))
		      {
                  strcpy(buf,((TEXT *)adr)->text);
                  strcpy(eT.st,"");
                  ptr_n=buf;
                  while (*ptr_n!='\0')
                  {
                      if (*ptr_n=='\n') strcat(eT.st,"\\n");
                      else if (*ptr_n!='\r') strncat(eT.st,ptr_n,1);
                      ptr_n++;
                  }
                  eT.address = adr;
                 Out_Edited_Draw_Param ((ESTR *)&eT, TRUE) ;
		      }
    		      break;
	case Bblok  :
		      if ((lps_edit_block->kod_obiektu != B_DIM) && (lps_edit_block->kod_obiektu != B_DIM2) && (lps_edit_block->kod_obiektu != B_DIM3))
		      {
			return;
		      }
		      ADP = (char *)lps_edit_block ;
		      ADK = ADP + sizeof(NAGLOWEK) + ((BLOK *)ADP)->n-1;
		      zmien_atrybut(ADP,ADK,Anormalny,Ablok);
              b__edit_current_block = FALSE ;
              if (ADP == dane)
              {
                b__edit_current_block = TRUE ;
              }
		      b=1;ruchblokw();
		      Katr();
		      break;
      case Bpoint  :
              if ((lps_edit_block->kod_obiektu != B_DIM) && (lps_edit_block->kod_obiektu != B_DIM2) && (lps_edit_block->kod_obiektu != B_DIM3))
              {
                  return;
              }
              ADP=ADK=(char  *)adr;
              ADLr = (char*)ADP ;
              parlr();
              zmien_atrybut(ADP,ADK,Anormalny,Ablok);
              //b__edit_current_block = FALSE ;
              p=1;
              ((LINIA *)adr)->obiektt1=Utwierdzony1;
              ruchblokw();
              Katp((LINIA *)adr);
              break;
	default     :
		      return;
      }
   }
  else
   if (b || bl || t || r || p)
    { noruchblokw();
      zmien_atrybut(ADP,ADK,Ablok,Anormalny);
      if ((r)||(p)) ((LINIA*)ADLr)->obiektt1=Guma;
      if(t) usunstr(np);
      r=p=b=bl=t=0;
	  if (TTF_redraw) redraw();
     }
}

static void edwym (void)  /* funkcja obslugi edycji wymiarowania */
/*-------------------*/
{
  //okno_r_second();
  Edit_Wym (0, NULL);  /* funkcja obslugi edycji wymiarowania */
  //okno_all_second();
}


static void near zwl(LINIA *L, double dx, double dy)
{ L->x1+=dx; L->x2+=dx; L->y1+=dy; L->y2+=dy;}


static void cur_off(double x, double y)
{
    flip_screen();
}
/*
static void cur_off__(double x, double y)
{ if(t||b || bl ||r)  out_blok();
}
*/
static void cur_on(double x, double y)
{ double Dx,Dy,xs,ys;
  //static double X0,Y0;
  double sxy, rad1;
  WIELOKAT *wSt_p;
  WIELOKAT wSt;
  LINIA *wL;
  NAGLOWEK *nag;
  double kat1,n;
  PLINIA PL;
  double wL_x1, wL_y1, r2;
  BOOL arrowhead;

  if(t|| b || bl || r || p)
   {
     Dx=x-X00;
     Dy=y-Y00;

     //Dx=x;
     //Dy=y;

     if(b || r || p)
      { if ((lps_edit_block->kod_obiektu != B_DIM3) || (p) || (orto))//it's not a leader block
          {
              Dx = Dx * katr.sin * katr.sin - Dy * katr.sin * katr.cos;
              Dy = -(x - X00) * katr.sin * katr.cos + Dy * katr.cos * katr.cos;
          }
        if(b && b__edit_current_block == TRUE)
         { zwl(&Ls,Dx,Dy);
           zwl(&Ls1,Dx,Dy);
           w.x2+=Dx; w.y2+=Dy;
           w.x0+=Dx; w.y0+=Dy;
         }
      }
	 else if (bl)
	 {
		
		 Dx = Dx * katr.sin*katr.sin - Dy * katr.sin*katr.cos;
		 Dy = -(x - X00)*katr.sin*katr.cos + Dy * katr.cos*katr.cos;

		 rad1 = sqrt((X00 - xr)*(X00 - xr) + (Y00 - yr)*(Y00 - yr));
		 
		 sxy =  rad1 / rad;
		 ////rad = rad1;
	
	 }
	 if (bl)
     {
         ////transformacja_blok(ADP, ADK, xr, yr, sxy, sxy, Tskala, 0);
         x01 = xr; y01 = yr; k01 = sxy; k02 =sxy; f0=Tskala;
         out_blok1(xr, yr, sxy, sxy, Tskala, 0);

         X00=x; Y00=y;
     }
	 else
     {
         ////transformacja_blok(ADP,ADK,Dx,Dy,0,0,Tprzesuw,0);
         x01 = Dx; y01 = Dy; k01= 0; k02=0; f0=Tprzesuw;

         //out_blok1(Dx,Dy,0,0,Tprzesuw,0);

         out_blok2(Dx,Dy,0,0,0,0,0,0,Tprzesuw, 0, FALSE, 6);

         if ((lps_edit_block->kod_obiektu == B_DIM3) && ((((LINIA *) PTR__GTMP6)->obiektt1)==2) && ((((LINIA *) PTR__GTMP6)->obiekt)==Olinia) && ((((LINIA *) PTR__GTMP6)->obiektt2)==1) && ((((LINIA *) PTR__GTMP6)->obiektt3)==1))  // PTR__GTMP6 instead of ADP
         ////if ((lps_edit_block->kod_obiektu == B_DIM3) && ((((LINIA *) last_buf)->obiekt)==Olinia) && ((((LINIA *) last_buf)->obiektt1)==2) && ((((LINIA *) last_buf)->obiektt2)==1) && ((((LINIA *) last_buf)->obiektt3)==1))  // last_buf instead of PTR__GTMP6
         {
             out_blok2(Dx,Dy,0,0,0,0,0,0,Tprzesuw, 0, FALSE, 6);
             arrowhead=TRUE;
         }
         else
         {
             out_blok2(Dx,Dy,0,0,0,0,0,0,Tprzesuw, 0, FALSE, -1);
             arrowhead=FALSE;
         }
         if (arrowhead)
         {
             ////wL=(LINIA *) PTR__GTMP6;
             wL=(LINIA *) last_buf;

             //wSt_p=(WIELOKAT *)(PTR__GTMP6 + wL->n + sizeof(NAGLOWEK));

             //nag=(NAGLOWEK *)(ADP + wL->n + sizeof(NAGLOWEK));
             nag=(NAGLOWEK *)(PTR__GTMP6 + wL->n + sizeof(NAGLOWEK));
             if (nag->obiekt==Owwielokat)
             //if (!bl)
             {

                 wSt_p = (WIELOKAT *) (PTR__GTMP6 + wL->n + sizeof(NAGLOWEK));
                 memmove(&wSt, wSt_p, sizeof(NAGLOWEK) + wSt_p->n);

                 PTR__GTMP7=wSt_p;

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

                 wSt.xy[2] = wL_x1;
                 wSt.xy[3] = wL_y1;
                 wSt.xy[0] = wL_x1 - n * Kp2s * cos(Pi * (kat1 - kat0) / 180);
                 wSt.xy[1] = wL_y1 - n * Kp2s * sin(Pi * (kat1 - kat0) / 180);
                 wSt.xy[4] = wL_x1 - n * Kp2s * cos(Pi * (kat1 + kat0) / 180);
                 wSt.xy[5] = wL_y1 - n * Kp2s * sin(Pi * (kat1 + kat0) / 180);

                 okno_r();
                 setwritemode(COPY_PUT);
                 rysuj_obiekt_(&wSt, COPY_PUT, 1);
                 okno_all();
             }

         }

         //out_blok1(Dx,Dy,0,0,Tprzesuw,0);

     }
     if ((lps_edit_block->kod_obiektu != B_DIM3) && (r)) korektalr();
     out_blok();
   }
  out_sel_on(x,y);
  if (wym_kata==2) out_cur_on(x, y);
  //X00=x; Y00=y;
}

static int kls10(void)
{ edwym();
  return 0;
}

static int fkey_delete (void)
/*------------------------*/
{
  usun ();
  return 0;
}

static int fkey_new_dim (void)
/*--------------------------*/
{
  WymNowy () ;
  return 0;
}

static void mpini(int n)
{ if (n!=(COMNDmnr+3) && n>=COMNDmnr && (b||t||r) ) edwym(); }

static void redcr(char typ)
{ static int ( *SW[5])(),kom,kom0,sel_akt,sel_cur;
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static void (*MPini)(int);
  if (typ==0)
   { CUR_oFF=CUR_OFF; CUR_OFF=cur_off;
      CUR_oN=CUR_ON;   CUR_ON=cur_on;
     SW[0]=SERV[68];  SERV[68]=kls10;
     SW[1]=SERV[73];  SERV[73]=sel_t;
     SW[2]=SERV[81];  SERV[81]=sel_d;
     SW[3] = SERV [79];  SERV [79] = fkey_new_dim ;
     SW[4] = SERV [83];  SERV [83] = fkey_delete ;
     MPini=MPINI;     MPINI=mpini;
     kom=Komunikat_R; Komunikat_R=0;
     kom0=Komunikat_R0; Komunikat_R0=6;
     komunikat(Komunikat_R);
     komunikat0(Komunikat_R0);
     sel_akt=sel.akt; sel.akt=1;
     sel_cur=sel.cur; sel.cur=1;
     t=b=bl=0;
     eT.x=20*WIDTH;  //120
     eT.y=ESTR_Y;
     eT.val_no_max = 0 ;
     eT.lmax = (getmaxx() - ttf_digits27_len - 5 - 20 * ttf_width_w) / ttf_width_w; // width_w;
     eT.mode = GV_STRING_D ;
     eT.format = "%s" ;
     eT.ESTRF = W_t ;
     Cur_ond (X, Y) ;

     menupini (&mWymiarujm, _DIMENSIONING_, _DIMENSIONING_C_,5) ;

     new_dim_info (TRUE) ;
   }
  else
   { CUR_ON=CUR_oN;
     SERV[68]=SW[0];
     SERV[73]=SW[1];
     SERV[81]=SW[2];
     SERV [79] = SW [3] ;
     SERV [83] = SW [4] ;
     MPINI=MPini;
     komunikat(kom);
     komunikat0(kom0);
     sel.akt=sel_akt;
     sel.cur=sel_cur;
     katkier=NoNoZnany;
     menupini(NULL,"",' ',0);
     if(b|| bl || t||r) edwym();
     Cur_offd (X, Y) ;
     CUR_OFF=CUR_oFF;
     new_dim_info (FALSE) ;
     if (Lsw==1)
      { Lsw=0;
	Ls.warstwa = Current_Layer;
	if(dodaj_obiekt((BLOK*)dane,(void*)&Ls)==NULL) return;
	else if(WymInter)
    {
        rysuj_obiekt(&Ls, COPY_PUT, 1);
    }
      }
   }
}

#undef __BIBWYM4__