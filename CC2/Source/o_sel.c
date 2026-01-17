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

#define __O_SEL__

#include<forwin.h>
#include<stddef.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_textw.h"

#include "message.h"

static int wyj1;
static int kom;
void *PTR__Sel_Adr,*PTR__Sel_Adr1 , *PTR__Sel_Adr_w;
static unsigned TYP,typL;
static void (*CUR_oN)(double ,double);
static void (*CUR_oFF)(double ,double);

extern int przeciecieLl_(double *x, double *y, void *adr, void *adr1);
extern int przecieciell_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieLW_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSAW_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieLSA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielW_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielSA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSASA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieWO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSAO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieWW_(double *x, double *y, void *adr, void *adr1);

extern int przeciecieLS_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielS_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSAS_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieWS_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSS_(double *x, double *y, void *adr, void *adr1);

extern int przeciecieLE_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieLEA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSAE_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSAEA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielE_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielEA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieEO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieEAO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieWE_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieWEA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSE_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSEA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieEE_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieEEA_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieEAEA_(double *x, double *y, void *adr, void *adr1);

extern int Point_Selected (T_Point *) ;
extern int Vector_Selected (AVECTOR *ptrs_vector);
extern int pcx_wybrany(B_PCX *ad, BOOL printing);

extern NODE bnodes[MAX_NODES];
extern double curviness;
extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
extern void set_bnodes(SPLINE* ptrs_spline, float *ad_xy, int* node_count_);
extern int solid_in_progress_vertices(void);
extern char *solid_in_progress_ad(void);
extern BOOL get_entire_poly(void);
extern BLOK *FIRSTB(char *ado);
extern BLOK *LASTB(char *ado);

extern void najblizszyE (double *x, double *y, void *adr);
extern void Elliptical_Arc_Ends (void *adr, double *x1, double *y1, double *x2, double *y2);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void get_control_points(NODE n1, NODE n2, float points[]);
extern void get_control_points_end(NODE n1, NODE n2, float points[8]);
extern int solidarc_elements(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner1, LUK *l_inner2, LUK *l_outer1, LUK *l_outer2);
extern int choose_object(int type_address_no, TYPE_ADDRESS *type_address);
extern BOOL global_any_choice;
extern char *load_symbol[];

static void  nooop(void)
{}
#define noopM (void (*)(int x,int y))nooop
static int  nooop1(void)
{return 0;}
#define noopS (int ( *)(void))nooop1


int tekst_w_prostokacie(TEXT  *ad)
{ double x1,x2,x3,x4,y1,y2,y3,y4,xt1,yt1;
  double adsin,adcos,addl;
  double WysokoscT;
  double del_wys=1;
  TEXT ptrs_text_;

  adsin=sin(ad->kat);
  adcos=cos(ad->kat);
  addl = Get_TextLen (ad, NULL) ;
  if (addl==0)
   {
     memmove(&ptrs_text_,ad, sizeof(NAGLOWEK) + ad->n);
     strcat(&ptrs_text_.text," ");
     ptrs_text_.dl+=1;
     ptrs_text_.n+=1;
     addl = Get_TextLen (&ptrs_text_, NULL) ;
   }

  WysokoscT = ad->wysokosc;

  WysokoscT *= del_wys;
  
  x1=ad->x;
  y1=ad->y;

   switch (ad->justowanie)
     {
  case 1: xt1 = x1 - addl * adcos ;  //do prawej
          yt1 = y1 - addl * adsin ; 
          x1 = xt1;
          y1 = yt1;
          break;
  case 2: xt1 = x1 - (addl/2) * adcos ;  //srodkowo
          yt1 = y1 - (addl)/2 * adsin ; 
          x1 = xt1;
          y1 = yt1;
          break;
  case 3: xt1 = x1 - (addl/2) * adcos + (WysokoscT/2) * adsin;  //centralnie
          yt1 = y1 - (addl)/2 * adsin - (WysokoscT/2) * adcos; 
          x1 = xt1;
          y1 = yt1;
          break;    
  default : break;            
     }    

  x2=x1+addl*adcos;         y2=y1-Ky*addl*adsin;

  x4=x1-ad->wysokosc*del_wys*adsin;   y4=y1-Ky*ad->wysokosc*del_wys*adcos;
  x3=-x1+x2+x4;                 y3=-y1+y2+y4;
  return (  punkt_w_prostokacie(x1,y1) &&
	    punkt_w_prostokacie(x2,y2) &&
	    punkt_w_prostokacie(x3,y3) &&
	    punkt_w_prostokacie(x4,y4)    );
}

int wielokat_w_prostokacie(WIELOKAT *w)
{ int i,l;
  for (i=0;i<w->lp;i+=2)
   { l=punkt_w_prostokacie(w->xy[i],w->xy[i+1]);
     if(!l) break;
   }
  return l;
}

int spline_w_prostokacie(SPLINE *ad)
{
	int ret = 0;
	int i, l, j;
	float out_x[MaxNpts], out_y[MaxNpts];
	SPLINE tmp_spline = Splinedef;
	double bnodes_2_tangent_org, bnodes_2_tangent;
	int node_count;

#define Npts 90 //30

	if (ad->npts < 5)
	{
		calc_bspline(ad->lp, ad->npts, ad->xy, Npts, &out_x, &out_y);
		for (i = 0; i < Npts; i++)
		{
			l = punkt_w_prostokacie(out_x[i], out_y[i]);
			if (!l) break;
		}
		return l;
	}
	else //multipoints
	{
		curviness = ad->xy[ad->lp];

		tmp_spline.npts = 4;
		tmp_spline.lp = 8;
		tmp_spline.kolor = ad->kolor;
		tmp_spline.typ = ad->typ;

		node_count = 1;


		set_bnodes(ad, ad->xy, &node_count);

		calc_tangents(ad->closed, &node_count, &bnodes_2_tangent);

		if (ad->closed == 0)
		{
			bnodes_2_tangent_org = bnodes[2].tangent;
			bnodes[2].tangent = bnodes_2_tangent;
			get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
			//rysuj_spline_(&tmp_spline, &tmp_spline.xy, mode, kolor, count_mvcurb, TRUE);
			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
			for (i = 0; i < (Npts - 1); i++)
			{
				l = punkt_w_prostokacie(out_x[i], out_y[i]);
				if (!l) return l;
			}

			bnodes[2].tangent = bnodes_2_tangent_org;
		}

		for (j = 2; j < node_count - 2; j++)  //1 - extra_node
		{
			get_control_points(bnodes[j], bnodes[j + 1], tmp_spline.xy);
			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
			for (i = 0; i < (Npts - 1); i++)
			{
				l = punkt_w_prostokacie(out_x[i], out_y[i]);
				if (!l) return l;
			}
		}

		if ((node_count > 3) && (ad->closed == 0))
		{
			get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
			for (i = 0; i < (Npts - 1); i++)
			{
				l = punkt_w_prostokacie(out_x[i], out_y[i]);
				if (!l) return 1;
			}
		}
		return l;

	}
}

int sel_t(void)
{
	if(sel.akw || sel.gor || sel.cur)
   { out_sel_off(X,Y);
     sel.size+=(1*(RETINA+1));
     out_sel_on(X,Y);
   }
  return 0;
}
int sel_d(void)
{ 
	if ((sel.akw || sel.gor || sel.cur) && sel.size>(1*(RETINA+1)))
   { out_sel_off(X,Y);
     sel.size-=(1*(RETINA+1));
     out_sel_on(X,Y);
   }
  return 0;
}

static void  cur_off(double x,double y)
{
  CUR_oFF(x, y);
}

static void  cur_on(double x,double y)
{ CUR_oN(x,y);
  if(!sel.cur && !sel.gor) out_sel_on(x,y);
}

static void redcr1(char typ)
{ static int ( *SW[6])();
  static void (*CUR)(int ,int);
  if (typ==0)
   { SW[0]=SERV[72];  SERV[72]=noopS;
     SW[1]=SERV[75];  SERV[75]=noopS;
     SW[2]=SERV[77];  SERV[77]=noopS;
     SW[3]=SERV[80];  SERV[80]=noopS;
     SW[4]=SERV[73];  SERV[73]=noopS;
     SW[5]=SERV[81];  SERV[81]=noopS;
     CUR=MVCUR;       MVCUR=noopM;
   }
  else
   { SERV[72]=SW[0];
     SERV[75]=SW[1];
     SERV[77]=SW[2];
     SERV[80]=SW[3];
     SERV[73]=SW[4];
     SERV[81]=SW[5];
     MVCUR=CUR;
   }
}

static void redcr(char typ)
{ static int ( *SW[2])()/*,kom*/;
  if (typ==0)
   { CUR_oFF=CUR_OFF;  CUR_OFF=cur_off;
     CUR_oN=CUR_ON;   CUR_ON=cur_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     sel.akw=1;
       CUR_ON(X,Y);
   }
  else
   {
     sel.akw=0;
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
     komunikat(kom);
   }
}

/*-----selectS-------------------------------------------*/

int qkoniecS(double *x, double *y)
{ LINIA *adl;
  AVECTOR *adv;
  LUK *ad;
  WIELOKAT *w ;
  SPLINE *s;
  ELLIPTICALARC *ea;
  SOLIDARC *sa;
  double x1, y1, x2, y2;
  char *adr;
  char  *adh;
  double dx,dy;
  int ret;
  int i;

  ret=1;
  if (sel.akt==0) { *x=X; *y=Y; return 0; }
  adh=dane;
  adh+=dane_size;
  dx=dy=jednostkiY(sel.size);
  oknoS(X-dx,Y-dy,X+dx,Y+dx);
  redcr1(0);

  if (solid_in_progress_vertices() > 0)
    {
        adr=solid_in_progress_ad();
        w = (WIELOKAT*)adr ;
        for (i = 0 ; i < w->lp ; i += 2)
        {
            *x = w->xy [i + 0] ;
            *y = w->xy [i + 1] ;
            if (punkt_w_prostokacie (*x, *y)) goto koniec;
        }
    }

  obiekt_tok((char*)dane,adh,&adr,ONieOkreslony);
  while (adr!=NULL)
   { if (((NAGLOWEK*)adr)->widoczny)
     switch(((NAGLOWEK*)adr)->obiekt)
      { case Olinia :
       if (!(Layers [((LINIA*)adr)->warstwa].point )) break;
	   adl=(LINIA*)adr;
	   *x=adl->x1;
	   *y=adl->y1;
	   if (punkt_w_prostokacie(*x,*y)) goto koniec;
	   *x=adl->x2;
	   *y=adl->y2;
	   if (punkt_w_prostokacie(*x,*y)) goto koniec;
	   break;
	case Oluk :
        if (!(Layers [((LUK*)adr)->warstwa].point )) break;
	   ad=(LUK*)adr;
	   *x=ad->x+ad->r*cos(ad->kat1);
	   *y=ad->y+ad->r*sin(ad->kat1);
	   if (punkt_w_prostokacie(*x,*y)) goto koniec;
	   *x=ad->x+ad->r*cos(ad->kat2);
	   *y=ad->y+ad->r*sin(ad->kat2);
	   if (punkt_w_prostokacie(*x,*y)) goto koniec;
	   break;
	case Owwielokat :
        if (!(Layers [((WIELOKAT*)adr)->warstwa].point )) break;
	   w = (WIELOKAT*)adr ;
	   for (i = 0 ; i < w->lp ; i += 2)
	   {
	     *x = w->xy [i + 0] ;
	     *y = w->xy [i + 1] ;
	     if (punkt_w_prostokacie (*x, *y)) goto koniec;
	   }
	   break;
   case Ospline:
       if (!(Layers [((SPLINE*)adr)->warstwa].point )) break;
	   s = (SPLINE*)adr;
		*x = s->xy[0];
		*y = s->xy[1];
		if (punkt_w_prostokacie(*x, *y)) goto koniec;

		*x = s->xy[s->lp-2];
		*y = s->xy[s->lp-1];
		if (punkt_w_prostokacie(*x, *y)) goto koniec;
	  
	   break;
    case Oellipticalarc:
              if (!(Layers [((ELLIPTICALARC*)adr)->warstwa].point )) break;
              ea = (ELLIPTICALARC *)adr;
              Elliptical_Arc_Ends (ea, &x1, &y1, &x2, &y2);

              *x = x1;
              *y = y1;
              if (punkt_w_prostokacie(*x, *y)) goto koniec;

              *x = x2;
              *y = y2;
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
      break;
      case Osolidarc: 
      {
          if (!(Layers[((SOLIDARC *) adr)->warstwa].point)) break;
          LINIA L_left, L_right;
          LUK l_inner1, l_inner2, l_outer1, l_outer2;
          int ln;

          sa = (SOLIDARC *) adr;

          *x = sa->x + sa->r * cos(sa->kat1);
          *y = sa->y + sa->r * sin(sa->kat1);
          if (punkt_w_prostokacie(*x, *y)) goto koniec;
          *x = sa->x + sa->r * cos(sa->kat2);
          *y = sa->y + sa->r * sin(sa->kat2);
          if (punkt_w_prostokacie(*x, *y)) goto koniec;

          ln=solidarc_elements(sa, &L_left, &L_right, &l_inner1, &l_inner2,&l_outer1, &l_outer2);
          
          //lines

          *x = L_left.x1;
          *y = L_left.y1;
          if (punkt_w_prostokacie(*x, *y)) goto koniec;
          *x = L_left.x2;
          *y = L_left.y2;
          if (punkt_w_prostokacie(*x, *y)) goto koniec;
          *x = L_right.x1;
          *y = L_right.y1;
          if (punkt_w_prostokacie(*x, *y)) goto koniec;
          *x = L_right.x2;
          *y = L_right.y2;
          if (punkt_w_prostokacie(*x, *y)) goto koniec;
          //arcs
          if (ln==1) {
              *x = l_inner1.x + l_inner1.r * cos(l_inner1.kat1);
              *y = l_inner1.y + l_inner1.r * sin(l_inner1.kat1);
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
              *x = l_inner1.x + l_inner1.r * cos(l_inner1.kat2);
              *y = l_inner1.y + l_inner1.r * sin(l_inner1.kat2);
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
              *x = l_outer1.x + l_outer1.r * cos(l_outer1.kat1);
              *y = l_outer1.y + l_outer1.r * sin(l_outer1.kat1);
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
              *x = l_outer1.x + l_outer1.r * cos(l_outer1.kat2);
              *y = l_outer1.y + l_outer1.r * sin(l_outer1.kat2);
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
          }
          else
          {
              *x = l_inner1.x + l_inner1.r * cos(l_inner1.kat1);
              *y = l_inner1.y + l_inner1.r * sin(l_inner1.kat1);
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
              *x = l_outer1.x + l_outer1.r * cos(l_outer1.kat1);
              *y = l_outer1.y + l_outer1.r * sin(l_outer1.kat1);
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
              *x = l_inner2.x + l_inner2.r * cos(l_inner2.kat2);
              *y = l_inner2.y + l_inner2.r * sin(l_inner2.kat2);
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
              *x = l_outer2.x + l_outer2.r * cos(l_outer2.kat2);
              *y = l_outer2.y + l_outer2.r * sin(l_outer2.kat2);
              if (punkt_w_prostokacie(*x, *y)) goto koniec;
          }
      }
      break;
          case Ovector :
              switch (((AVECTOR*)adr)->style)
              {
                  case 0:
                  case 1:
                  case 2:
                  case 3:
                  case 4:  //force
                  case 18: //slab force
                  case 19: //force z
                  case 7:  //displacement
                  case 27:  //displacement z
                  if (!(Layers[((AVECTOR *) adr)->warstwa].point)) break;
                  adv = (AVECTOR *) adr;
                  *x = adv->x1;
                  *y = adv->y1;
                  if (punkt_w_prostokacie(*x, *y)) goto koniec;
                  *x = adv->x2;
                  *y = adv->y2;
                  if (punkt_w_prostokacie(*x, *y)) goto koniec;
                  break;
                  case 10:
                  case 17:  //slab
                  case 20:  //trapezium Z
                  case 11:
                  case 12:
                  case 13:
                  case 14:
                  case 15:
                  case 16:
                      if (!(Layers[((AVECTOR *) adr)->warstwa].point)) break;
                      adv = (AVECTOR *) adr;
                      *x = adv->x1;
                      *y = adv->y1;
                      if (punkt_w_prostokacie(*x, *y)) goto koniec;
                      *x = adv->x2;
                      *y = adv->y2;
                      if (punkt_w_prostokacie(*x, *y)) goto koniec;
                      break;
                  default:
                      break;
              }
	default :
	   break;
      }
     obiekt_tok(NULL,adh,&adr,ONieOkreslony);
   }
  *x=X;
  *y=Y;
  ret=0;
koniec :
  redcr1(1);
  return ret;
}

int selectS(double *x,double *y, int nr_kom_)
{
  int n;
  if (!sel.akt || sel.akw) return 0;
  redcr(0);
  kom=Komunikat_R;
  komunikat(nr_kom_);
  wyj1=0;
  while(1)
   { if((n=getwsp(NULL))==0) { redcr(1); return 0; }
     if (n== ENTER && qkoniecS(x,y)) { redcr(1); return 1; }
   }
}
/*-----   selectS  ->end   -------------------------------------------*/

/*-----  punkt przeciecia  --------------------------------------------*/

static int przeciecieLL__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przeciecieLL_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Blinia | Blinia;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecieLO__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przeciecieLO_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Blinia | Bokrag;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecieOO__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przeciecieOO_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Bokrag | Bokrag;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecieLl__(char *ad0, char *ad)
/*-------------------------------------------------*/
{ double x,y;
  int l;
  l=0;
  if (przeciecieLl_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Blinia | Bluk;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecielO__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przeciecielO_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Bluk | Bokrag;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przecieciell__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przecieciell_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Bluk | Bluk;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecieLW__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przeciecieLW_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Blinia | Bwwielokat;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecieSAW__(char *ad0, char *ad)
{ double x,y;
    int l;
    l=0;
    if (przeciecieSAW_(&x,&y,(void *)ad0,(void *)ad))
    { PTR__Sel_Adr=ad0;
        PTR__Sel_Adr1 =ad;
        TYP=Bsolidarc | Bwwielokat;
        l=punkt_w_prostokacie(x,y);
    }
    return l;
}

static int przeciecieLSA__(char *ad0, char *ad)
{ double x,y;
    int l;
    l=0;
    if (przeciecieLSA_(&x,&y,(void *)ad0,(void *)ad))
    { PTR__Sel_Adr=ad0;
        PTR__Sel_Adr1 =ad;
        TYP=Blinia | Bsolidarc;
        l=punkt_w_prostokacie(x,y);
    }
    return l;
}

static int przeciecieLS__(char *ad0, char *ad)
{
	double x, y;
	int l;
	l = 0;
	if (przeciecieLS_(&x, &y, (void *)ad0, (void *)ad))
	{
		PTR__Sel_Adr = ad0;
		PTR__Sel_Adr1 = ad;
		TYP = Blinia | Bspline;
		l = punkt_w_prostokacie(x, y);
	}
	return l;
}

static int przeciecieLE__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieLE_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Blinia | Bellipse;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecielE__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecielE_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bluk | Bellipse;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieSAE__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieSAE_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bsolidarc | Bellipse;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieLEA__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieLEA_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Blinia | Bellipticalarc;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieSAEA__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieSAEA_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bsolidarc | Bellipticalarc;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecielEA__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecielEA_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bluk | Bellipticalarc;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieEO__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieEO_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bellipse | Bokrag;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieEAO__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieEAO_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bellipticalarc | Bokrag;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecielW__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przeciecielW_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Bluk | Bwwielokat;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecielSA__(char *ad0, char *ad)
{ double x,y;
    int l;
    l=0;
    if (przeciecielSA_(&x,&y,(void *)ad0,(void *)ad))
    { PTR__Sel_Adr=ad0;
        PTR__Sel_Adr1 =ad;
        TYP=Bluk | Bsolidarc;
        l=punkt_w_prostokacie(x,y);
    }
    return l;
}

static int przeciecieSASA__(char *ad0, char *ad)
{ double x,y;
    int l;
    l=0;
    if (przeciecieSASA_(&x,&y,(void *)ad0,(void *)ad))
    { PTR__Sel_Adr=ad0;
        PTR__Sel_Adr1 =ad;
        TYP=Bsolidarc | Bsolidarc;
        l=punkt_w_prostokacie(x,y);
    }
    return l;
}

static int przeciecielS__(char *ad0, char *ad)
{
	double x, y;
	int l;
	l = 0;
	if (przeciecielS_(&x, &y, (void *)ad0, (void *)ad))
	{
		PTR__Sel_Adr = ad0;
		PTR__Sel_Adr1 = ad;
		TYP = Bluk | Bspline;
		l = punkt_w_prostokacie(x, y);
	}
	return l;
}

static int przeciecieSAS__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieSAS_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bsolidarc | Bspline;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieWO__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przeciecieWO_(&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP= Bwwielokat | Bokrag ;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecieSAO__(char *ad0, char *ad)
{ double x,y;
    int l;
    l=0;
    if (przeciecieSAO_(&x,&y,(void *)ad0,(void *)ad))
    { PTR__Sel_Adr=ad0;
        PTR__Sel_Adr1 =ad;
        TYP= Bsolidarc | Bokrag ;
        l=punkt_w_prostokacie(x,y);
    }
    return l;
}

static int przeciecieSO__(char *ad0, char *ad)
{
	double x, y;
	int l;
	l = 0;
	if (przeciecieSO_(&x, &y, (void *)ad0, (void *)ad))
	{
		PTR__Sel_Adr = ad0;
		PTR__Sel_Adr1 = ad;
		TYP = Bspline | Bokrag;
		l = punkt_w_prostokacie(x, y);
	}
	return l;
}

static int przeciecieSE__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieSE_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bspline | Bellipse;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieEE__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieEE_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bellipse | Bellipse;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieSEA__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieSEA_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bspline | Bellipticalarc;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieEEA__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieEEA_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bellipse | Bellipticalarc;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieEAEA__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieEAEA_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bellipticalarc | Bellipticalarc;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieWW__(char *ad0, char *ad)
{ double x,y;
  int l;
  l=0;
  if (przeciecieWW_ (&x,&y,(void *)ad0,(void *)ad))
   { PTR__Sel_Adr=ad0;
     PTR__Sel_Adr1 =ad;
     TYP=Bwwielokat | Bwwielokat;
     l=punkt_w_prostokacie(x,y);
   }
  return l;
}

static int przeciecieWS__(char *ad0, char *ad)
{
	double x, y;
	int l;
	l = 0;
	if (przeciecieWS_(&x, &y, (void *)ad0, (void *)ad))
	{
		PTR__Sel_Adr = ad0;
		PTR__Sel_Adr1 = ad;
		TYP = Bwwielokat | Bspline;
		l = punkt_w_prostokacie(x, y);
	}
	return l;
}

static int przeciecieWE__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieWE_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bwwielokat | Bellipse;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieWEA__(char *ad0, char *ad)
{
    double x, y;
    int l;
    l = 0;
    if (przeciecieWEA_(&x, &y, (void *)ad0, (void *)ad))
    {
        PTR__Sel_Adr = ad0;
        PTR__Sel_Adr1 = ad;
        TYP = Bwwielokat | Bellipticalarc;
        l = punkt_w_prostokacie(x, y);
    }
    return l;
}

static int przeciecieSS__(char *ad0, char *ad)
{
	double x, y;
	int l;
	l = 0;
	if (przeciecieSS_(&x, &y, (void *)ad0, (void *)ad))
	{
		PTR__Sel_Adr = ad0;
		PTR__Sel_Adr1 = ad;
		TYP = Bspline | Bspline;
		l = punkt_w_prostokacie(x, y);
	}
	return l;
}


static int obtyp(int ob)
{
  int l ;
  switch (ob)
  {
     case Olinia :
	 l = Blinia ;
	 break ;
     case Ovector :
	 l = Bvector ;
	 break ;
     case Oluk   :
	 l = Bluk ;
	 break ;
     case Oellipticalarc   :
     l = Bellipticalarc ;
     break ;
     case Ookrag :
     case Okolo  :
	 l = Bokrag ;
	 break ;
     case Oellipse :
     l = Bellipse ;
     break ;
     case Ofilledellipse :
     l = Bfilledellipse ;
     break ;
     case Owwielokat :
     //case Owwielokat3D :
	 l = Bwwielokat ;
	 break ;
	 case Ospline:
	 l = Bspline;
	 break;
     case Osolidarc:
     l = Bsolidarc;
     break;
     default:
	 l = Bnic ;
	 break ;
   }
  return l;
}

static int przectyp(char *ad0, char *ad)
{
  int typ,l;

  l=0;
  if (FALSE == INTERSECT_ADD_TEST (ad0, ad))
  {
    return l ;
  }
  if ( ((NAGLOWEK*)ad)->obiekt==Owwielokat /*|| ((NAGLOWEK*)ad)->obiekt==Owwielokat3D*/)   /*przestawienie kolejnosci elememtow*/
   { char *adr;			/*Linia zawsze pierwsza, jesli nie ma linii to luk pierwszy*/
     adr=ad0;			/*jesli nie ma linii i luku to obszar pierwszy*/
     ad0=ad;
     ad=adr;
   }
  if (((NAGLOWEK*)ad)->obiekt == Ospline)   /*przestawienie kolejnosci elememtow*/
  {
	  char *adr;			/*Linia zawsze pierwsza, jesli nie ma linii to luk pierwszy*/
	  adr = ad0;			/*jesli nie ma linii i luku to obszar pierwszy*/
	  ad0 = ad;
	  ad = adr;
  }
  if (((NAGLOWEK*)ad)->obiekt == Osolidarc)   /*przestawienie kolejnosci elememtow*/
  {
     char *adr;			/*Linia zawsze pierwsza, jesli nie ma linii to luk pierwszy*/
     adr = ad0;			/*jesli nie ma linii i luku to obszar pierwszy*/
     ad0 = ad;
     ad = adr;
  }
  if ( ((NAGLOWEK*)ad)->obiekt==Oluk)   /* lO */
   { char *adr;
     adr=ad0;
     ad0=ad;
     ad=adr;
   }
  if ( ((NAGLOWEK*)ad)->obiekt==Olinia)   /* LO Ll */
   { char *adr;
     adr=ad0;
     ad0=ad;
     ad=adr;
   }
   if ( ((NAGLOWEK*)ad)->obiekt==Ovector)   // LO Ll
   { char *adr;
     adr=ad0;
     ad0=ad;
     ad=adr;
   }

  typ = obtyp (((NAGLOWEK*)ad0)->obiekt) | obtyp (((NAGLOWEK*)ad)->obiekt) ;
  switch(typ)
   { case Blinia | Blinia : l=przeciecieLL__(ad0,ad);
			     break;
     case Blinia | Bokrag : l=przeciecieLO__(ad0,ad);
			     break;
     case Bokrag | Bokrag : l=przeciecieOO__(ad0,ad);
			     break;

     case Blinia | Bluk :   l=przeciecieLl__(ad0,ad);
			     break;

     case Bvector | Blinia : l=przeciecieLL__(ad0,ad);
                 break;
     case Bvector | Bokrag : l=przeciecieLO__(ad0,ad);
                 break;
     case Bvector | Bluk :   l=przeciecieLl__(ad0,ad);
                break;
     case Bvector | Bwwielokat : l = przeciecieLW__ (ad0, ad) ;
                break ;
     case Bvector | Bspline: l = przeciecieLS__(ad0, ad);
                break;
     case Bvector | Bellipse:
     case Bvector | Bfilledellipse:  l = przeciecieLE__(ad0, ad);
                break;
     case Bvector | Bellipticalarc: l = przeciecieLEA__(ad0, ad);
                break;
     case Bvector | Bvector : l=przeciecieLL__(ad0,ad);
                 break;

     case Bluk | Bokrag :   l=przeciecielO__(ad0,ad);
			     break;
     case Bluk | Bluk :     l=przecieciell__(ad0,ad);
			     break;
     case Blinia | Bwwielokat : l = przeciecieLW__ (ad0, ad) ;
			     break ;
     case Bsolidarc | Bwwielokat : l = przeciecieSAW__ (ad0, ad) ;
                 break ;
     case Bluk | Bwwielokat : l = przeciecielW__ (ad0, ad) ;
			     break ;
     case Bluk | Bsolidarc : l = przeciecielSA__ (ad0, ad) ;
                break ;
     case Bsolidarc | Bsolidarc : l = przeciecieSASA__ (ad0, ad) ;
                break ;
     case Bwwielokat | Bokrag : l = przeciecieWO__ (ad0, ad) ;
			     break ;
     case Bsolidarc | Bokrag : l = przeciecieSAO__ (ad0, ad) ;
                 break ;
     case Bwwielokat | Bwwielokat : l = przeciecieWW__ (ad0, ad) ;
			     break ;
	 case Blinia | Bspline: l = przeciecieLS__(ad0, ad);
		 break;
     case Bsolidarc | Bspline: l = przeciecieSAS__(ad0, ad);
           break;
     case Blinia | Bellipse:
     case Blinia | Bfilledellipse:
             l = przeciecieLE__(ad0, ad);
           break;
     case Bsolidarc | Bellipse:
     case Bsolidarc | Bfilledellipse:
            l = przeciecieSAE__(ad0, ad);
           break;
     case Blinia | Bellipticalarc:
           l = przeciecieLEA__(ad0, ad);
           break;
     case Bsolidarc | Bellipticalarc:
           l = przeciecieSAEA__(ad0, ad);
           break;
     case Blinia | Bsolidarc: l = przeciecieLSA__(ad0, ad);
           break;
     case Bluk | Bellipse:
     case Bluk | Bfilledellipse:
           l = przeciecielE__(ad0, ad);
           break;
     case Bluk | Bellipticalarc:
           l = przeciecielEA__(ad0, ad);
           break;
       case Bokrag | Bellipse:
       case Bokrag | Bfilledellipse:
           l = przeciecieEO__(ad0, ad);
           break;
       case Bokrag | Bellipticalarc:
           l = przeciecieEAO__(ad0, ad);
           break;
       case Bwwielokat | Bellipse:
       case Bwwielokat | Bfilledellipse:
           l = przeciecieWE__(ad0, ad);
           break;
       case Bwwielokat | Bellipticalarc:
           l = przeciecieWEA__(ad0, ad);
           break;
       case Bspline | Bellipse:
       case Bspline | Bfilledellipse:
           l = przeciecieSE__(ad0, ad);
           break;
       case Bspline | Bellipticalarc:
           l = przeciecieSEA__(ad0, ad);
           break;
       case Bellipse | Bellipse:
       case Bellipse | Bfilledellipse:
       case Bfilledellipse | Bfilledellipse:
           l = przeciecieEE__(ad0, ad);
           break;
       case Bellipse | Bellipticalarc:
       case Bfilledellipse | Bellipticalarc:
           l = przeciecieEEA__(ad0, ad);
           break;
       case Bellipticalarc | Bellipticalarc:
           l = przeciecieEAEA__(ad0, ad);
           break;

	   case Bluk | Bspline: l = przeciecielS__(ad0, ad);
		   break;
	   case Bspline | Bokrag: l = przeciecieSO__(ad0, ad);
		   break;
	   case Bwwielokat | Bspline: l = przeciecieWS__(ad0, ad);
		   break;
	   case Bspline | Bspline: l = przeciecieSS__(ad0, ad);
		   break;
        default:
            break;
   }
  return l;
}

static int przecdod(char * ad,int *n)
/*------------------------------------------*/
{
  static char *adp;
  char *ad0;
  static char  *adh;
  int l;

  if (*n == -2)
  {
    adp = NULL ;
    adh = NULL ;
    return 1;
  }
  if (*n==-1)
  {
     if (adp != NULL)
     {
       obiekt_tok1(adp,adh,&ad0,ONieOkreslony);
       while (ad0!=NULL)
       {
	 if( ((NAGLOWEK*)ad0)->przec==1 ) ((NAGLOWEK*)ad0)->przec=0;
	 obiekt_tok1(NULL,adh,&ad0,ONieOkreslony);
       }
     }
     return 1;
  }
  (*n)++;
  ((NAGLOWEK*)ad)->przec=1;
  if(*n==1)
   { adp=ad;
     adh=ad;
     PTR__Sel_Adr=ad;
     TYP=obtyp(((NAGLOWEK*)ad)->obiekt);
     return 0;
   }
  if(*n==2)
   { l=przectyp(ad,adp);
     if (l)  ((NAGLOWEK*)adp)->przec=((NAGLOWEK*)ad)->przec=0;
     else adh=ad;
     return l;
   }
  l=0;
  obiekt_tok1(adp,adh,&ad0,ONieOkreslony);
  while (ad0!=NULL)
   { if( ((NAGLOWEK*)ad0)->przec==1 )
      if( (l=przectyp(ad,ad0)) !=0 ) { l=-1; return przecdod(NULL,&l); }
     obiekt_tok1(NULL,adh,&ad0,ONieOkreslony);
   }
  adh=ad;
  return 0;
}

static int sprawdz_obiekt (NAGLOWEK *ad,int typL, int info)
{ int ws,typ=Bnic;
    LUK l;
  switch(ad->obiekt)
   { case   Opcx : if ((typL & Bpcx) && ((Layers [((B_PCX*)ad)->warstwa].point ) || (info==1)))
		      { ws=pcx_wybrany((B_PCX*)ad, FALSE);
        		  if(ws) typ=Bpcx;
		      }
		     break;
     case   Olinia :
		     if ((typL & Blinia) && ((Layers [((LINIA*)ad)->warstwa].point) || (info==1)) )
		     {
		       ws = linia_wybrana ((LINIA*)ad) ;
		       if(ws) typ=Blinia;
		     }
		     else
		     if ((typL & Bpolygon) &&
			  ((Layers [((LINIA*)ad)->warstwa].point) || (info==1)) &&
			  ad->blok == 1 &&
			  NULL != LASTb &&
			  LASTb->kod_obiektu == B_PLINE &&
			  LASTb->opis_obiektu [0] == PL_POLYGON &&
			  0 != (ws = linia_wybrana ((LINIA*)ad)))
		     {
		       typ = Bpolygon ;
		     }
		     break;
     case   Oluk   : if ((typL & Bluk) && ((Layers [((LUK*)ad)->warstwa].point ) || (info==1)))
		      { 
				ws=luk_wybrany((LUK*)ad);
				if(ws) typ=Bluk;
		      }
		     break;
     case   Ookrag : if ((typL & Bokrag)  &&
			 ((Layers [((OKRAG*)ad)->warstwa].point) || (info==1)))
		      { ws=okrag_wybrany((OKRAG*)ad);
			if(ws) typ = Bokrag ;
		      }
		     break;
       case Oellipse :
           if ((typL & Bellipse)  &&
               ((Layers [((ELLIPSE*)ad)->warstwa].point) || (info==1)))
           { ws=elipsa_wybrana_prec((ELLIPSE*)ad);
               if(ws) typ = Bellipse ;
           }
           break;
       case Ofilledellipse:
           if ((typL & Bfilledellipse)  &&
               ((Layers [((ELLIPSE*)ad)->warstwa].point) || (info==1)))
           { ws=elipsa_wybrana_prec((ELLIPSE*)ad);
               if(ws) typ = Bfilledellipse ;
           }
           break;
       case Oellipticalarc:
               if ((typL & Bellipticalarc)  &&
                   ((Layers [((ELLIPTICALARC*)ad)->warstwa].point) || (info==1)))
               { ws=lukeliptyczny_wybrany_prec((ELLIPTICALARC*)ad);
                   if(ws) typ = Bellipticalarc ;
               }
           break;
     case   Okolo  : if ((typL & Bokrag)  &&
			 ((Layers [((OKRAG*)ad)->warstwa].point ) || (info==1)))
		      { ws=okrag_wybrany((OKRAG*)ad);
			if(ws) typ = Bkolo ;
		      }
		     break;
     case Owwielokat :
         if(((typL & Bwwielokat) || ((typL & BwwielokatGradient) && (((WIELOKAT*)ad)->gradient==1))) &&
			((Layers [((WIELOKAT*)ad)->warstwa].point ) || (info==1)))
		       { ws=wielokat_wybrany((WIELOKAT*)ad);
			 if(ws) 
				 typ=Bwwielokat;
		       }
		     break;
     case   Osolidarc: if ((typL & Bsolidarc) && ((Layers[((SOLIDARC*)ad)->warstwa].point) || (info == 1)))
           {
               ws = solidarc_wybrany((SOLIDARC*)ad);
               if (ws) typ = Bsolidarc;
           }
           break;
	 case   Ospline: if ((typL & Bspline) && ((Layers[((SPLINE*)ad)->warstwa].point) || (info == 1)))
			 {
				 ws = spline_wybrany((SPLINE*)ad);
				 if (ws) typ = Bspline;
			 }
  		    break;
     case   Otekst : if(((typL & Btekst) || ((typL & BtekstNoType15) && (((TEXT*)ad)->typ!=15))) &&
			((Layers [((TEXT*)ad)->warstwa].point ) || (info==1)))
		      { ws=tekst_wybrany((TEXT*)ad);
			if(ws) typ=Btekst;
		      }
		     break;
     case   Opoint : if ((typL & Bpoint)  &&
			 ((Layers [((T_Point*)ad)->warstwa].point ) || (info==1)))
		      { ws = Point_Selected ((T_Point*)ad);
			    if(ws) typ = Bpoint ;
		      }
		     break;
       case   Ovector :
           if ((typL & Bvector) && ((Layers [((LINIA*)ad)->warstwa].point) || (info==1)) )
           {
               ws=vector_wybrany((AVECTOR*)ad);
               if(ws) typ=Bvector;
           }
           break;

     default       : break;
   }
  return typ;
}

static int sprawdz_tekst (NAGLOWEK *ad,int typL, int info)
{ int ws,typ=Bnic;
  switch(ad->obiekt)
   { case   Olinia :
		     if ((typL & Blinia) && ((Layers [((LINIA*)ad)->warstwa].point) || (info==1)) )
		     {
		       ws = linia_wybrana ((LINIA*)ad) ;
		       if(ws) typ=Blinia;
		     }
		     else
		     if ((typL & Bpolygon) &&
			  ((Layers [((LINIA*)ad)->warstwa].point) || (info==1)) &&
			  ad->blok == 1 &&
			  NULL != LASTb &&
			  LASTb->kod_obiektu == B_PLINE &&
			  LASTb->opis_obiektu [0] == PL_POLYGON &&
			  0 != (ws = linia_wybrana ((LINIA*)ad)))
		     {
		       typ = Bpolygon ;
		     }
		     break;
     case   Oluk   : if ((typL & Bluk) && ((Layers [((LUK*)ad)->warstwa].point ) || (info==1)))
		      { ws=luk_wybrany((LUK*)ad);
			if(ws) typ=Bluk;
		      }
		     break;
     case   Ookrag : if ((typL & Bokrag)  &&
			 ((Layers [((OKRAG*)ad)->warstwa].point) || (info==1)))
		      { ws=okrag_wybrany((OKRAG*)ad);
			if(ws) typ = Bokrag ;
		      }
		     break;
     case   Okolo  : if ((typL & Bokrag)  &&
			 ((Layers [((OKRAG*)ad)->warstwa].point ) || (info==1)))
		      { ws=okrag_wybrany((OKRAG*)ad);
			if(ws) typ = Bkolo ;
		      }
		     break;
     case Owwielokat :
         if((typL & Bwwielokat) &&
			((Layers [((WIELOKAT*)ad)->warstwa].point ) || (info==1)))
		       { ws=wielokat_wybrany((WIELOKAT*)ad);
			 if(ws) typ=Bwwielokat;
		       }
		     break;
	 case   Ospline: if ((typL & Bspline) && ((Layers[((SPLINE*)ad)->warstwa].point) || (info == 1)))
			 {
				 ws = spline_wybrany((SPLINE*)ad);
				 if (ws) typ = Bspline;
			 }
			break;
     case   Otekst : if((typL & Btekst) &&
			((Layers [((TEXT*)ad)->warstwa].point ) || (info==1)))
		      { ws=tekst_wybrany((TEXT*)ad);
			if(ws) typ=Btekst;
		      }
		     break;
     case   Opoint : if ((typL & Bpoint)  &&
			 ((Layers [((T_Point*)ad)->warstwa].point ) || (info==1)))
		      { ws = Point_Selected ((T_Point*)ad);
			if(ws) typ = Bpoint ;
		      }
		     break;

     default       : break;
   }
  return typ;
}


/*--------select_edwym------------------------------------------*/

void wskaz_edwym(void)
{ double dx,dy;
  NAGLOWEK *ad;
  char  *adh;
  unsigned typ;
  adh=dane;
  adh+=dane_size;
  redcr1(0);
  dx=dy=jednostkiY(sel.size);
  oknoS(X-dx,Y-dy,X+dx,Y+dx);
  ad=NULL;
  obiekt_tok ((char*)dane,adh,(char**)&ad,ONieOkreslony);
  while (ad!=NULL)
   { if(ad->widoczny && ad->obiektt2==O2BlockDim && (typ=sprawdz_obiekt(ad,typL,0)) !=0 )
      { TYP=typ; PTR__Sel_Adr=((void *) ad);
	goto koniec;
      }
     obiekt_tok(NULL,adh,(char**)&ad,ONieOkreslony);
   }
  dzwonek();
  redcr1(1);
  return;
 koniec:
  redcr1(1);
  wyj1=1;
  return;
}

void  *select_edwym(unsigned *typ)
{ PTR__Sel_Adr=NULL;
  TYP=0;
  typL=*typ;
  wskaz_edwym();
  if (TYP==Blinia && ( ((LINIA*)PTR__Sel_Adr)->obiektt3==O3NoWymRoz) ||
      TYP == Bkolo || TYP == Bwwielokat )
  {
     TYP=Bblok ;
  }
  *typ=TYP;
  return (void  *)PTR__Sel_Adr;
}
/*--------select_edwym  ->end------------------------------------------*/

/*--------select_variable------------------------------------------*/

void wskaz_variable(void)
{ double dx,dy;
  NAGLOWEK *ad;
  TEXT *adt;
  char  *adh;
  unsigned typ;
  adh=dane;
  adh+=dane_size;
  redcr1(0);
  dx=dy=jednostkiY(sel.size);
  oknoS(X-dx,Y-dy,X+dx,Y+dx);
  ad=NULL;
  obiekt_tok ((char*)dane,adh,(char**)&ad,ONieOkreslony);
  while (ad!=NULL)
   { if(ad->widoczny
           && ad->obiekt==Otekst
           && (typ=sprawdz_obiekt(ad,typL,1)) !=0 )
      { 
       adt=ad;
       if ((adt->typ==n_typ_zmienna) || (adt->typ==n_typ_komentarz) || (adt->typ==n_typ_port) || (adt->typ==n_typ_typ)
          || (adt->typ==n_typ_plik) || (adt->typ==n_etykieta_wew)  || (adt->obiektt2==O2BlockDim))
       {
        TYP=typ; PTR__Sel_Adr=((void *) ad);
	goto koniec;
       }	
      }
     obiekt_tok(NULL,adh,(char**)&ad,ONieOkreslony);
   }
  dzwonek();
  redcr1(1);
  return;
 koniec:
  redcr1(1);
  wyj1=1;
  return;
}

void  *select_variable(unsigned *typ)
{ PTR__Sel_Adr=NULL;
  TYP=0;
  typL=*typ;
  wskaz_variable();
  if (TYP==Blinia && ( ((LINIA*)PTR__Sel_Adr)->obiektt3==O3NoWymRoz) ||
      TYP == Bkolo || TYP == Bwwielokat )
  {
     TYP=Bblok ;
  }
  *typ=TYP;
  return (void  *)PTR__Sel_Adr;
}
/*--------select_variable  ->end------------------------------------------*/

int GetObjectColor(char *PTR__Sel_Adr, int *layer)
{   int type;
    int kolor;
    type=((NAGLOWEK *)PTR__Sel_Adr)->obiekt;
    switch (type)
    {

        //Olinia = 1, Otekst = 2, Oluk = 3,Ookrag = 4,Okolo = 5, Owwielokat = 6, Opoint = 7, OdBLOK = 8, Osolidarc=9,
        //Oellipticalarc = 10, Oellipse = 11, Ofilledellipse = 12, Ospline = 13, Ovector = 14, Opcx = 0,
        case Olinia:
            kolor=((LINIA *)PTR__Sel_Adr)->kolor;
            *layer=((LINIA *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Otekst:
            kolor=((TEXT *)PTR__Sel_Adr)->kolor;
            *layer=((TEXT *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Oluk:
            kolor=((LUK *)PTR__Sel_Adr)->kolor;
            *layer=((LUK *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Ookrag:
        case Okolo:
            kolor=((OKRAG *)PTR__Sel_Adr)->kolor;
            *layer=((OKRAG *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Owwielokat:
            kolor=((WIELOKAT *)PTR__Sel_Adr)->kolor;
            *layer=((WIELOKAT *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Opoint:
            kolor=((T_Point *)PTR__Sel_Adr)->kolor;
            *layer=((T_Point *)PTR__Sel_Adr)->warstwa+1;
            break;
        case OdBLOK:
            kolor=15;
            *layer=0;
            break;
        case Osolidarc:
            kolor=((SOLIDARC *)PTR__Sel_Adr)->kolor;
            *layer=((SOLIDARC *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Oellipticalarc:
            kolor=((ELLIPTICALARC *)PTR__Sel_Adr)->kolor;
            *layer=((ELLIPTICALARC *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Oellipse:
        case Ofilledellipse:
            kolor=((ELLIPSE *)PTR__Sel_Adr)->kolor;
            *layer=((ELLIPSE *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Ospline:
            kolor=((SPLINE *)PTR__Sel_Adr)->kolor;
            *layer=((SPLINE *)PTR__Sel_Adr)->warstwa+1;
            break;
        case Ovector:
            kolor=((AVECTOR *)PTR__Sel_Adr)->kolor;
            *layer=((AVECTOR *)PTR__Sel_Adr)->warstwa+1;
            break;
        default:
            kolor=15;
            break;
    }
    return kolor;
}

#define MAX_TYPE_ADDRESS_NO 64 //32

void delete_element(TYPE_ADDRESS *arr, int *n, int index_to_delete) {
    if (index_to_delete < 0 || index_to_delete >= *n) {
        return;
    }

    for (int i = index_to_delete; i < *n - 1; i++) {
        memmove(&arr[i],&arr[i + 1],sizeof(TYPE_ADDRESS)); // Shift elements left
    }
    (*n)--; // Decrement logical size
}

void wskaz(int info)
{ double dx,dy;
  NAGLOWEK *ad;
  char  *adh;
  int n;
  unsigned typ;
  int info1;
  TYPE_ADDRESS type_address[MAX_TYPE_ADDRESS_NO];
  int type_address_no=0;
  int o_layer;

  adh=dane;
  adh+=dane_size;
  redcr1(0);
  dx=dy=jednostkiY(sel.size);
  oknoS(X-dx,Y-dy,X+dx,Y+dx);
  
  info1 = info;
  n = - 2 ;		/*na potrzeby przeciec*/
  przecdod (NULL, &n) ; /*inicjacja pointer'ow*/
  n = 0 ;
  ad = NULL ;

  if (solid_in_progress_vertices() > 0)
    {
        ad=solid_in_progress_ad();
        if ((typ = sprawdz_obiekt (ad, typL, info1)) != 0
        && TRUE == INITIAL_SEL_TEST (ad))
        {
            TYP=typ;
            PTR__Sel_Adr=((void *) ad);
            goto koniec;
        }
    }


  obiekt_tok((char*)dane,adh,(char*)&ad,ONieOkreslony);
  while (ad!=NULL)
  {

    if (((ad->widoczny == 1) || (ad->obiekt==Opcx))
	&&  ad->obiekt != OdBLOK
	&&  ( !(typL & Bwymiarowanie)  /*nie jest to wymiarowanie*/
	   ||  ((typL & Bwymiarowanie)
	       &&  ( ad->obiektt2 == O2BlockDim   /*podczas wymiarowania wskazano element bloku wymiarowego*/
		  || ad->obiektt2 == O2BlockPline  /*podczas wymiarowania wskazano element wielolinii*/
		  || ad->obiektt2 == O2NoBlockS    /*podczas wymiarowania wskazano element nie nalezacy do bloku specjalnego*/
		  || ad->obiektt2 == O2BlockAparat /*podczas wymiarowania wskazano element nalezacy do aparatu*/
		  || ad->obiektt2 == O2BlockSpecial /*podczas wymiarowania wskazano element nalezacy do bloku specjalnego - TERMAT*/
		  || ad->obiektt2 == O2BlockHatch25
		  /*|| ad->obiektt2 == O2BlockHatch50*/)))/*podczas wymiarowania wskazano element nalezacy do bloku wypelnienia - TERMAT*/
	&& (typ = sprawdz_obiekt (ad, typL, info1)) != 0
	&& TRUE == INITIAL_SEL_TEST (ad))
    {
      TYP=typ;
      if ((typL & Bwymiarowanie) &&
	   ad->obiektt2 == O2BlockDim) 	/*brak natychmiastowego przerwania*/
      {		/*poniewaz wymiarowanie ma priorytet przed edycja wymiarowania*/
	     PTR__Sel_Adr_w = ((void *) ad);
      }
      else
      {
	PTR__Sel_Adr=((void *) ad);
	if(/*!(typL & Bprzeciecie) &&*/ !(typL & Bwymiarowanie))
    {
        if ((info1) && (PTR__Sel_Adr!=NULL))
        {
            //registration of object
            if (type_address_no<MAX_TYPE_ADDRESS_NO)
            {
                type_address[type_address_no].type=((NAGLOWEK *)PTR__Sel_Adr)->obiekt;
                type_address[type_address_no].color=GetObjectColor(PTR__Sel_Adr, &o_layer);
                type_address[type_address_no].layer=o_layer;
                if (type_address[type_address_no].type==Ovector)
                    type_address[type_address_no].style=((AVECTOR *)PTR__Sel_Adr)->style;
                else if (type_address[type_address_no].type==Opoint)
                    type_address[type_address_no].style=((T_Point *)PTR__Sel_Adr)->typ;
                else type_address[type_address_no].style=-1;
                type_address[type_address_no].address=PTR__Sel_Adr;
                type_address[type_address_no].TYP=TYP;
                type_address_no++;
                if (global_any_choice) break;
            }
        }
         else goto koniec;
    }
	if ((przecdod((char *)ad,&n)) && (typL & Bwymiarowanie)) goto koniec; //// !!!!
      }
    }
    obiekt_tok(NULL,adh,(char**)&ad,ONieOkreslony);
  }
  if ((typL & Bprzeciecie) && (type_address_no==2))
      goto koniec;
  if ((info1) && (type_address_no>0))
  {
      if ((type_address_no==1)/* || ((typL & Bprzeciecie) && (type_address_no==2))*/)
      {
          PTR__Sel_Adr=type_address[0].address;
          TYP=type_address[0].TYP;
          goto koniec;
      }
      else
      {
          if (typL & Bprzeciecie) {
              komunikat_str_short_reset();
              komunikat_str_short(_FIRST_, FALSE, TRUE);
          }
          int ret_n=choose_object(type_address_no, type_address);
          if (ret_n==-1) PTR__Sel_Adr=NULL;
          else {
              PTR__Sel_Adr = type_address[ret_n].address;
              TYP = type_address[ret_n].TYP;
          }
          if (typL & Bprzeciecie) {
              komunikat_str_short_mask(TRUE);
              if (ret_n > -1)
              {
                  //removing already chosen object
                  delete_element(type_address, &type_address_no, ret_n);
                  //komunikat_str_mask(TRUE);
                  komunikat_str_short(_SECOND_, FALSE, TRUE);
                  ret_n = choose_object(type_address_no, type_address);
                  if (ret_n == -1) PTR__Sel_Adr1 = NULL;
                  else {
                      PTR__Sel_Adr1 = type_address[ret_n].address;
                      TYP |= type_address[ret_n].TYP;
                  }
                  komunikat_str_short_mask(TRUE);
              }
          }

          goto koniec;
      }
  }

  if((typL & Bprzeciecie) || (typL & Bwymiarowanie))
   { n=-1; przecdod(NULL,&n); }
  if((typL & Bwymiarowanie) && TYP) goto koniec;
  if((typL & Bprzeciecie) && PTR__Sel_Adr1 !=NULL) goto koniec;
  dzwonek();
  redcr1(1);
  return;
koniec:
  redcr1(1);
  wyj1=1;
  return;
}

void *select_w_info (unsigned *typ, void **adr)
/*--------------------------------------*/
{
  void * ptr_ret;
  BLOK *ptr_blk;

  typL=*typ;
  PTR__Sel_Adr = PTR__Sel_Adr1  = PTR__Sel_Adr_w = NULL;
  TYP=0;
  wskaz(1);
  *typ=TYP;
  if (adr != NULL)  *adr=PTR__Sel_Adr1 ;
  ptr_ret = PTR__Sel_Adr ;
  if (PTR__Sel_Adr == NULL && (typL & Bwymiarowanie))
  {
    ptr_ret = PTR__Sel_Adr_w;
  }
  if ((ptr_ret!=NULL) && (get_entire_poly()))
  {
      if (((NAGLOWEK *)ptr_ret)->blok==1)
      {
          ptr_blk=FIRSTB(ptr_ret);
          if (ptr_blk!=NULL)
          {
              if (((ptr_blk->kod_obiektu==B_PLINE) || (ptr_blk->kod_obiektu==B_HATCH)))
              {
                  ptr_ret=ptr_blk;
              }
          }
      }
  }
  return ptr_ret ;
}


void *select_w (unsigned *typ, void **adr)
/*--------------------------------------*/
{
  void * ptr_ret;

  typL=*typ;
  PTR__Sel_Adr = PTR__Sel_Adr1  = PTR__Sel_Adr_w = NULL;
  TYP=0;
  wskaz(1);   ////0   //it should take all object no matter layer.point is set or not
  *typ=TYP;
  if (adr != NULL)  *adr=PTR__Sel_Adr1 ;
  ptr_ret = PTR__Sel_Adr ;
  if (PTR__Sel_Adr == NULL && (typL & Bwymiarowanie))
  {
    ptr_ret = PTR__Sel_Adr_w;
  }
  return ptr_ret ;
}

void  *select_n(unsigned *typ, void **adr, int nr_kom_)
{ int n;
  if (!sel.akt || sel.akw) return NULL;
  redcr(0);
  //kom=Komunikat_R;
  komunikat(nr_kom_);
  komunikat_len(nr_kom_);  //to give space for short message of choice
  typL=*typ;
  TYP=0;
  PTR__Sel_Adr=NULL;PTR__Sel_Adr1 =NULL;
  wyj1=0;
  while(1)
   {  if((n=getwsp(NULL))==0)  /* n=-1 -> esc */
       { redcr(1);
	 return NULL;
       }
     if (n== ENTER)
      { wskaz(1);   //0
	if (wyj1)
	 { redcr(1);
	   *typ=TYP;
	   if (adr != NULL)  *adr=PTR__Sel_Adr1 ;
	   return PTR__Sel_Adr;
	 }
      }
   }
}

#undef __O_SEL__