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

#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#ifndef LINUX
#include <dos.h>
#endif
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_bloklb.h"
#include "o_libfun.h"
#include "o_object.h"

extern BOOL global_no_pcx;
extern BOOL global_only_pcx;
extern BOOL global_only_solid;
extern int Point_in_Rectangle (T_Point *, int dwc) ;
extern int Pcx_in_Rectangle (B_PCX *, int dwc) ;
extern int Point_Selected (T_Point *) ;
extern int Vector_Selected (AVECTOR *);
extern int linia_wybrana_pin(LINIA *ad);
extern void break_wire (void * ad, double xjunction, double yjunction);
extern BLOK *FIRSTB(char  *ado);
extern void *select_w_info (unsigned *typ, void **adr);
extern int wszystko_special(void);
extern int w_spline_wybrany(SPLINE* s);
extern void	layer_info(void);
extern int Ini_Blocks_Array(void);
extern void Destroy_Blocks_Array(void);
extern int vector_drag_wybrany(AVECTOR *ad);
extern int vector_w_prostokacie(AVECTOR *ad);
extern void getcolor_RGB(int *red, int *green, int *blue, int color);
extern int arc_to_isometric_ellipticalarc_a_ea(enum PlaneType plane, LUK *l, ELLIPTICALARC *ea);

void adrem_obiekt_FIRSTB (void *ad, int (*DZI)(void *), int (*ODZI)(void *), BOOL b_win_sel);
static void redcrc(char typ);
static void redcrok(char typ);

BOOL alert_on_object=FALSE;
void adrem_obiekt(void *ad, int (*DZI)(void *), int (*ODZI)(void *), BOOL);

static int ciag=0;
static int add = ADD_MODE, win = Windoww ;
static BOOL b_auto = TRUE ;
static BOOL del_quick = TRUE;
static  OKNO O,Oc;


static void utwierdzenie_linii_pin(LINIA *adl, int w)
{ if (w)
   { if(adl->obiektt1!=Sztywny)
     {if ((adl->blok != ElemBlok) || (swobodny_blok == TRUE))
      {
       switch(w)
	  { case 1 : adl->obiektt1=Utwierdzony2;break;
	    case 2 : adl->obiektt1=Utwierdzony1;break;
	    case 3 :
	    case 4 : adl->obiektt1=Guma;break;
	    default : break;
	  }
      }
      else 
      { 
       if ((adl->obiektt2==O2BlockDim) || (adl->obiektt2==O2BlockPline))
       {
       switch(w)
	  { case 1 : adl->obiektt1=Utwierdzony2;break;
	    case 2 : adl->obiektt1=Utwierdzony1;break;
	    case 3 :
	    case 4 : adl->obiektt1=Guma;break;
	    default : break;
	  }
       }
       else
         {
          switch(w)
	  { case 1 : 
	    case 2 : 
	    case 3 :
	    case 4 : adl->obiektt1=Guma;break;
	    default : break;
	  }
         }
      }  
     }	  
	  
   }
}



OKNO *Get_Stretch_Window (void)
/*---------------------------*/
{
  return &Oc ;
}

OKNO *Get_Block_Window (void)
/*---------------------------*/
{
  return &O ;
}

void Set_Block_Window (OKNO *o)
/*---------------------------*/
{
    memmove(&O, o, sizeof(OKNO)) ;
}


BOOL Get_Stretch_Mode (void)
/*-------------------------*/
{
  return (ciag == 1) ? TRUE : FALSE ;
}

void Set_Stretch_Mode (BOOL b_stretch)
/*----------------------------------*/
{
  ciag = b_stretch ;
}


int *Block_Proc_Win_Mode_Ptr (void)
/*-------------------------------*/
{
  return &win ;
}

BOOL *Block_Proc_Add_Mode_Ptr (void)
/*---------------------------------*/
{
  return &add ;
}

void Set_Block_Proc_Auto_Mode (BOOL b_a)
/*-------------------------------------*/
{
  b_auto  = b_a ;
}

void Change_Delete_Mode(void)
/*-------------------------------------*/
{
	del_quick = !del_quick;
}

BOOL get_b_auto(void)
{
	return b_auto;
}

BOOL get_del_quick(void)
{
	return del_quick;
}

static int nooop1(void *ad)
{ ad = ad ;
  return 0;
}

/*--------------------------------------------*/
/*------------  okno blok ------------------------*/

static void  cur_offo(double x,double y)
{
    flip_screen();
}

static void  cur_offo__(double x,double y)
{ cursel_off(x,y);
  outokno(&O,0,0);
}

static void  cur_ono(double x,double y)
{ O.x2=x; O.y2=y;
  outokno(&O,0,0);
  cursel_on(x, y);
}

/*--------------------------------------*/
static int linia_dwc(LINIA *adl,int dwc)
{
    int w;
    w = linia_wybrana(adl);
    if (dwc == Windoww && w != 3) return 0;
    if (dwc == Dragg && w == 4) return 0;
    return w;
}

static int vector_dwc(AVECTOR *adv,int dwc)
{ int w;
    LUK adluk=ldef;
    ELLIPTICALARC ea=eldef;
    OKRAG adOkrag=Odef;
    int ret;
    int plane;

    switch (adv->style)
    {
        case 5:
        case 6:
        case 8:
        case 9:
            adluk.x=adv->x1;
            adluk.y=adv->y1;
            adluk.r=adv->r;
            adluk.kat1=adv->angle1;
            adluk.kat2=adv->angle2;
            switch(dwc)
            {
                case Windoww:
                        w=luk_w_prostokacie(&adluk);
                    break;
                case Dragg : //w=prostokat_luk(&adluk);
                        w= punkt_w_prostokacie(adluk.x, adluk.y);
                        if(w==4) w=0;
                    break;
                case Cross :
                        w=luk_wybrany(&adluk);
                    break;
                default:
                    w=0;
                    break;
            }
            break;
        case 21:  //isometry
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
            adluk.x=adv->x1;
            adluk.y=adv->y1;
            adluk.r=adv->r;
            adluk.kat1=adv->angle1;
            adluk.kat2=adv->angle2;
            switch (adv->style) {
                case 21:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    break;
                case 23:
                    plane = YZ_PLANE;
                    break;
                case 25:
                    plane = XY_PLANE;
                    break;
                case 22:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    break;
                case 24:
                    plane = YZ_PLANE;
                    break;
                case 26:
                    plane = XY_PLANE;
                    break;
                case 28:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    break;
                case 30:
                    plane = YZ_PLANE;
                    break;
                case 32:
                    plane = XY_PLANE;
                    break;
                case 29:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    break;
                case 31:
                    plane = YZ_PLANE;
                    break;
                case 33:
                    plane = XY_PLANE;
                    break;
            }

            ret=arc_to_isometric_ellipticalarc_a_ea(plane, &adluk, &ea);

            switch(dwc)
            { case Windoww  :
                    w=lukeliptyczny_w_prostokacie(&ea);  //w=lukeliptyczny_wybrany(ea);
                    break;
                case Dragg : //w=lukeliptyczny_w_prostokacie(ea);  //w=lukeliptyczny_wybrany(ea);
                    w=prostokat_ellipticalarc(&ea);
                    if(w==4) w=0;
                    break;
                case Cross :
                    w=lukeliptyczny_wybrany_prec(&ea);
                    break;
                default:
                    w=0;
                    break;
            }
            break;
        case 16:
            adOkrag.x=adv->x1;
            adOkrag.y=adv->y1;
            adOkrag.r=adv->r;
            switch(dwc)
            {
                case Windoww: w=okrag_w_prostokacie(&adOkrag);
                    break;
                case Dragg : //w=prostokat_luk(&adluk);
                    w= punkt_w_prostokacie(adOkrag.x, adOkrag.y);
                    if(w==4) w=0;
                    break;
                case Cross : w=okrag_wybrany(&adOkrag);
                    break;
                default    : break;
            }

            switch(dwc)
            {
                case Windoww:
                    w = vector_w_prostokacie(adv);
                    break;
                case Dragg :
                    //w = prostokat_vector(adv);
                    w = vector_drag_wybrany(adv);
                    if (w == 4) w = 0;
                    break;
                case Cross :
                    w = vector_wybrany(adv);
                    break;
                default    :
                    break;
            }
            break;
        default:
            switch(dwc)
            {
                case Windoww:
                    w = vector_w_prostokacie(adv);
                    break;
                case Dragg :
                    w = vector_drag_wybrany(adv);
                    if (w == 4) w = 0;
                    break;
                case Cross :
                    w = vector_wybrany(adv);
                    break;
                default    :
                    break;
            }
            break;
    }
    return w;
}

static int linia_dwc_pin(LINIA *adl,int dwc)
{ int w;
  w=linia_wybrana_pin(adl);
  
  if(dwc==Windoww && w!=3) return 0;
  if(dwc==Dragg && w==4) return 0;
  return w;
}

static int tekst_dwc(TEXT *adt,int dwc)
{ int w;
double x1, y1;
  switch(dwc)
   { case Dragg : 
        x1 = adt->x;
        y1 = adt->y;

        w=punkt_w_prostokacie(x1,y1);

		  break;
     case Windoww: w=tekst_w_prostokacie(adt);
		  break;
     case Cross : w=tekst_wybrany(adt);
		  break;
     default    : break;
   }
  return w;
}

int luk_dwc(LUK *adluk,int dwc)
{
  int w=0;

  switch(dwc)
   { case Windoww: w=luk_w_prostokacie(adluk);
		  break;
     case Dragg : w=prostokat_luk(adluk);
		  if(w==4) w=0;
		  break;
     case Cross : w=luk_wybrany(adluk);
		  break;
     default    : break;
   }
  return w;
}

int solidarc_dwc(SOLIDARC *adsolidarc,int dwc)
{
    int w=0;

    switch(dwc)
    { case Windoww: w=solidarc_w_prostokacie(adsolidarc);
            break;
        case Dragg : w=prostokat_solidarc(adsolidarc);
            if(w==4) w=0;
            break;
        case Cross : w=solidarc_wybrany(adsolidarc);
            break;
        default    : break;
    }
    return w;
}

static int okrag_dwc(OKRAG *o,int dwc)
{ int w;
  switch(dwc)
   { case Windoww:
     case Dragg : w=okrag_w_prostokacie(o);
		  break;
     case Cross : w=okrag_wybrany(o);
		  break;
     default    : break;
   }
  return w;
}

static int ellipse_dwc(ELLIPSE *e, int dwc)
{
    int w;
    switch(dwc)
    { case Windoww:
        case Dragg : w=elipsa_w_prostokacie(e); //w=elipsa_wybrana(e);
            break;
        case Cross : w=elipsa_wybrana_prec(e);
            break;
        default    : break;
    }
    return w;
}

static int ellipticalarc_dwc(ELLIPTICALARC *ea, int dwc)
{
    int w;
    switch(dwc)
    { case Windoww  : w=lukeliptyczny_w_prostokacie(ea);  //w=lukeliptyczny_wybrany(ea);
        break;
        case Dragg : //w=lukeliptyczny_w_prostokacie(ea);  //w=lukeliptyczny_wybrany(ea);
            w=prostokat_ellipticalarc(ea);
            if(w==4) w=0;
            break;
        case Cross : w=lukeliptyczny_wybrany_prec(ea);
            break;
        default    : break;
    }
    return w;
}

static int point_dwc (T_Point *ptrs_point, int dwc)
{
  int w;

  switch (dwc)
  {
     case Windoww: w = Point_in_Rectangle (ptrs_point, dwc) ;
                  break;
     case Dragg : w=punkt_w_prostokacie(ptrs_point->x,ptrs_point->y);
		  break;
     case Cross :
                  w = Point_in_Rectangle (ptrs_point, dwc) ;
		  break ;
     default    : break ;
   }
  return w ;
}

static int pcx_dwc (B_PCX *pcx, int dwc)
{
  int w;

  switch (dwc)
  {
     case Windoww: w = Pcx_in_Rectangle (pcx, dwc) ;
                  break;
     case Dragg : w=punkt_w_prostokacie(pcx->x,pcx->y);
		  break;
     case Cross :
                  w = Pcx_in_Rectangle (pcx, dwc) ;
		  break ;
     default    : break ;
   }
  return w ;
}

static BOOL check_solid_dragg (WIELOKAT *w)
{
  int i ;
  BOOL b_check ;

  b_check = FALSE ;
  for (i = 0 ; i < (int)w->lp ; i += 2)
  {
    if (punkt_w_prostokacie (w->xy[i], w->xy[i + 1]))
    {
      b_check = TRUE ;
      break ;
    }
  }
  return b_check ;
}

static int wielokat_dwc(WIELOKAT *W,int dwc)
{ int w;
  switch(dwc)
   {
     case Windoww: w = wielokat_w_prostokacie (W) ;
		  break;
     case Dragg : w = (1 == wielokat_wybrany (W) &&
		       TRUE == check_solid_dragg (W)) ? 1 : 0 ;
		  break;
     case Cross : w = wielokat_wybrany (W) ;
		  break;
     default    : break;
   }
  return w;
}

static BOOL check_spline_dragg(SPLINE *s)
{
	int i;
	BOOL b_check;

	b_check = FALSE;
	for (i = 0; i < (int)s->lp; i += 2)
	{
		if (punkt_w_prostokacie(s->xy[i], s->xy[i + 1]))
		{
			b_check = TRUE;
			break;
		}
	}
	return b_check;
}

static int spline_dwc(SPLINE *S, int dwc)
{
	int w;
	switch (dwc)
	{
	case Windoww: w = spline_w_prostokacie(S);
		break;
	case Dragg: w = (1 == w_spline_wybrany(S) &&
		TRUE == check_spline_dragg(S)) ? 1 : 0;
		break;
	case Cross: w = spline_wybrany(S);
		break;
	default: break;
	}
	return w;
}

/*---------------------------------------------*/

void zamien1(double *x1, double *x2)
{ double pom;
  pom=*x1;
  *x1=*x2;
  *x2=pom;
}

void addrem_obiekty(int dwc, OKNO *O, int (*DZI)(void *), int (*ODZI)(void *) )
{ char *ad;
  NAGLOWEK *nag;
  int w;
  char  *adh;
  int blok_no;

  adh=dane;
  adh+=dane_size;

  blok_no=Ini_Blocks_Array();

  O->x01=O->x1; O->y01=O->y1;
  O->x02=O->x2; O->y02=O->y2;
  if(O->x01>O->x02) zamien1(&(O->x01),&(O->x02));
  if(O->y01>O->y02) zamien1(&(O->y01),&(O->y02));
  oknoS(O->x01,O->y01,O->x02,O->y02);
  ad=NULL;
  obiekt_tok((char *)dane,adh,&ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag=(NAGLOWEK*)ad;
     if (((nag->atrybut == Anormalny || nag->atrybut == Ablok || nag->atrybut == Aoblok || nag->atrybut==Apoblok
          || nag->atrybut==Appoblok || nag->atrybut==Apppoblok ) &&
	   add == ADD_MODE  &&
	   (dwc == Windoww ? /*nag->przec == 0*/ TRUE : TRUE)) ||      //change to allow multiple choice
	 (nag->atrybut == Ablok && add == DEL_MODE) )
     {
	w=0;
	switch(nag->obiekt)
	 { case Olinia :
	      w=linia_dwc((LINIA*)ad,dwc);
	      break;
      case Ovector :
         w=vector_dwc((AVECTOR*)ad,dwc);
          break;
	   case Otekst :
	      w=tekst_dwc((TEXT*)ad,dwc);
	      break;
	   case Oluk :
	      w=luk_dwc((LUK*)ad,dwc);
	      break;
       case Osolidarc :
          w=solidarc_dwc((SOLIDARC*)ad,dwc);
          break;
	   case Ookrag :
	   case Okolo   :
	      w=okrag_dwc((OKRAG*)ad,dwc);
	      break;
       case Oellipse :
       case Ofilledellipse   :
             w=ellipse_dwc((ELLIPSE*)ad,dwc);
             break;
       case Oellipticalarc :
             w=ellipticalarc_dwc((ELLIPTICALARC*)ad,dwc);
             break;
	   case Owwielokat   :
	      w=wielokat_dwc((WIELOKAT*)ad,dwc);
	      break ;
	  case Ospline:
		  w = spline_dwc((SPLINE*)ad, dwc);
		  break;
	   case Opoint :
	      w = point_dwc ((T_Point *)ad, dwc) ;
	      break ;
      case Opcx :
	      w = pcx_dwc ((B_PCX *)ad, dwc) ;
	      break ;
	   default :
	      break;
	 }
	if (w) {
        adrem_obiekt(ad, DZI, ODZI, TRUE);
    }
     }
     obiekt_tok(NULL,adh,&ad,ONieOkreslony);
   }

  Destroy_Blocks_Array();
}


void addrem_obiekty_items(int items, int dwc, OKNO *O, int (*DZI)(void *), int (*ODZI)(void *) )
{ char *ad;
    NAGLOWEK *nag;
    int w;
    char  *adh;
    int blok_no;

    adh=dane;
    adh+=dane_size;

    blok_no=Ini_Blocks_Array();

    O->x01=O->x1; O->y01=O->y1;
    O->x02=O->x2; O->y02=O->y2;
    if(O->x01>O->x02) zamien1(&(O->x01),&(O->x02));
    if(O->y01>O->y02) zamien1(&(O->y01),&(O->y02));
    oknoS(O->x01,O->y01,O->x02,O->y02);
    ad=NULL;
    obiekt_tok((char *)dane,adh,&ad,ONieOkreslony);
    while (ad!=NULL)
    {
        nag=(NAGLOWEK*)ad;
        if (((nag->atrybut == Anormalny || nag->atrybut == Ablok || nag->atrybut == Aoblok || nag->atrybut==Apoblok
              || nag->atrybut==Appoblok || nag->atrybut==Apppoblok ) &&
             add == ADD_MODE  &&
             (dwc == Windoww ? /*nag->przec == 0*/ TRUE : TRUE)) ||      //change to allow multiple choice
            (nag->atrybut == Ablok && add == DEL_MODE))
        {
            w=0;
            switch(nag->obiekt)
            { case Olinia :
                    if (!(items & Blinia)) break;
                    w=linia_dwc((LINIA*)ad,dwc);
                    break;
                case Ovector :
                    if (!(items & Bvector)) break;
                    w=vector_dwc((AVECTOR*)ad,dwc);
                    break;
                case Otekst :
                    //this item is restricted
                    //if text has typ==15 is ignored  (aux_description used in colorbars)
                    //if ((!(items & Btekst)) || (((TEXT*)nag)->typ)==15) break;
                    if ((!(items & Btekst)) && (!(items & BtekstNoType15) && (((TEXT*)ad)->typ!=15))) break;
                    w=tekst_dwc((TEXT*)ad,dwc);
                    break;
                case Oluk :
                    if (!(items & Bluk)) break;
                    w=luk_dwc((LUK*)ad,dwc);
                    break;
                case Osolidarc :
                    if (!(items & Bsolidarc)) break;
                    w=solidarc_dwc((SOLIDARC*)ad,dwc);
                    break;
                case Ookrag :
                case Okolo   :
                    if (!(items & Bokrag)) break;
                    w=okrag_dwc((OKRAG*)ad,dwc);
                    break;
                case Oellipse :
                case Ofilledellipse   :
                    if (!(items & Bellipse)) break;
                    w=ellipse_dwc((ELLIPSE*)ad,dwc);
                    break;
                case Oellipticalarc :
                    if (!(items & Bellipticalarc)) break;
                    w=ellipticalarc_dwc((ELLIPTICALARC*)ad,dwc);
                    break;
                case Owwielokat   :
                    if (!(items & Bwwielokat)) break;
                    w=wielokat_dwc((WIELOKAT*)ad,dwc);
                    break ;
                case Ospline:
                    if (!(items & Bspline)) break;
                    w = spline_dwc((SPLINE*)ad, dwc);
                    break;
                case Opoint :
                    if (!(items & Bpoint)) break;
                    w = point_dwc ((T_Point *)ad, dwc) ;
                    break ;
                case Opcx :
                    if (!(items & Bpcx)) break;
                    w = pcx_dwc ((B_PCX *)ad, dwc) ;
                    break ;
                default :
                    break;
            }
            if (w) {
                adrem_obiekt(ad, DZI, ODZI, TRUE);
            }
        }
        obiekt_tok(NULL,adh,&ad,ONieOkreslony);
    }

    Destroy_Blocks_Array();
}

void addrem_obiekty_FIRSTB(int dwc, OKNO *O, int (*DZI)(void *), int (*ODZI)(void *) )
{ char *ad;
  NAGLOWEK *nag;
  int w;
  char  *adh;
  adh=dane;
  adh+=dane_size;
  O->x01=O->x1; O->y01=O->y1;
  O->x02=O->x2; O->y02=O->y2;
  if(O->x01>O->x02) zamien1(&(O->x01),&(O->x02));
  if(O->y01>O->y02) zamien1(&(O->y01),&(O->y02));
  oknoS(O->x01,O->y01,O->x02,O->y02);
  ad=NULL;
  obiekt_tok((char *)dane,adh,&ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag=(NAGLOWEK*)ad;
     if (((nag->atrybut == Anormalny || nag->atrybut == Aoblok || nag->atrybut==Apoblok
          || nag->atrybut==Appoblok || nag->atrybut==Apppoblok ) &&
	   add == ADD_MODE  &&
	   (dwc == Windoww ? nag->przec == 0 : TRUE)) ||
	 (nag->atrybut == Ablok && add == DEL_MODE) )
     {
	w=0;
	switch(nag->obiekt)
	 { case Olinia :
	      w=linia_dwc((LINIA*)ad,dwc);
	      break;
	   case Otekst :
	      w=tekst_dwc((TEXT*)ad,dwc);
	      break;
	   case Oluk :
	      w=luk_dwc((LUK*)ad,dwc);
	      break;
	   case Ookrag :
	   case Okolo   :
	      w=okrag_dwc((OKRAG*)ad,dwc);
	      break;
	   case Owwielokat   :
	      w=wielokat_dwc((WIELOKAT*)ad,dwc);
	      break ;
	  case Ospline:
		  w = spline_dwc((SPLINE*)ad, dwc);
		  break;
	   case Opoint :
	      w = point_dwc ((T_Point *)ad, dwc) ;
	      break ;
      case Opcx :
	      w = pcx_dwc ((B_PCX *)ad, dwc) ;
	      break ;
	   default :
	      break;
	 }
	if (w) adrem_obiekt_FIRSTB(ad,DZI,ODZI, TRUE);
     }
     obiekt_tok(NULL,adh,&ad,ONieOkreslony);
   }
}

void addrem_linie_pin(int dwc, OKNO *O, int (*DZI)(void *), int (*ODZI)(void *) )
{ char *ad;
  NAGLOWEK *nag;
  int w;
  char  *adh;
  adh=dane;
  adh+=dane_size;
  ad=dane;
  ad=NULL;
  obiekt_tok((char *)dane,adh,&ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag=(NAGLOWEK*)ad;
     if (((nag->atrybut == Anormalny || nag->atrybut == Aoblok || nag->atrybut==Apoblok
         || nag->atrybut==Appoblok || nag->atrybut==Apppoblok ) &&
	   add == ADD_MODE  &&
	   (dwc == Windoww ? nag->przec == 0 : TRUE)) ||
	 (nag->atrybut == Ablok && add == DEL_MODE) )
     {
	w=0;
	switch(nag->obiekt)
	 { case Olinia :
	      w=linia_dwc_pin((LINIA*)ad,dwc);
	      if (w) utwierdzenie_linii_pin((LINIA*)ad,w);
	      break;
	   default :
	      break;
	 }
	if (w) adrem_obiekt(ad,DZI,ODZI, TRUE);
     }
     obiekt_tok(NULL,adh,&ad,ONieOkreslony);
   }
}


static int shift_grey(int kolory_blok)
{  int ret;
    ret=((kolory_blok+8)%16);
    return ret;
}

static double ColourDistance(RGBA e1, RGBA e2)
{
    long rmean = ( (long)e1.r + (long)e2.r ) / 2;
    long r = (long)e1.r - (long)e2.r;
    long g = (long)e1.g - (long)e2.g;
    long b = (long)e1.b - (long)e2.b;
    return sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
}


static void redcrok(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int sel_akt;

  static int kolory_blok;
  RGBA c1, c2;
  double cdistance;

  if(!typ)
   {
	 CUR_OFF(X, Y);
	 O.x1=X;
     O.y1=Y;
     O.x2=X;
     O.y2=Y;
     CUR_oFF=CUR_OFF;  CUR_OFF=cur_offo;
     CUR_oN=CUR_ON; CUR_ON=cur_ono;
     sel_akt=sel.akt; sel.akt=1;

     //determine colours similarity
     kolory_blok=kolory.blok;
     getcolor_RGB(&c1.r, &c1.g, &c1.b, kolory.paper);
     getcolor_RGB(&c2.r, &c2.g, &c2.b, kolory.blok);
     cdistance=ColourDistance(c1, c2);
     if (cdistance < 16)  kolory.blok=shift_grey(kolory.blok);

     CUR_ON(X,Y);
   }
  else
  {
    kolory.blok=kolory_blok;
	CUR_OFF(X, Y);
	CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;
    sel.akt=sel_akt;
	CUR_ON(X, Y);
  }
}


static void Break_Wire (void)
/*-------------------------*/
{ BLOK  *b;
  LINIA *L;
  char  *adp,  *adk;
  int i;

  adp = dane ;
  adk = dane + dane_size ;
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk && dodatkowe_linie<max_dodatkowe_linie)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    { if ((L->obiekt == Olinia) && (L->blok == NoElemBlok) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE))
       {
         /*linie poziome*/
         if (TRUE==Check_if_Equal(L->y1,L->y2))
         { for (i=1; i<=PIN_COUNT; i++)
           {if (TRUE==Check_if_Equal(L->y1,PIN_TAB_Y[i]))
            { if (((PIN_TAB_X[i]>L->x1) && (PIN_TAB_X[i]<L->x2)) 
                || ((PIN_TAB_X[i]<L->x1) && (PIN_TAB_X[i]>L->x2))) 
               {
                break_wire(adp, PIN_TAB_X[i], PIN_TAB_Y[i]);
               }
            }
           }
         }
          else
           {  /*linia pionowa*/
            if (TRUE==Check_if_Equal(L->x1,L->x2)) 
            {  
             for (i=1; i<=PIN_COUNT; i++)
              {
               if (TRUE==Check_if_Equal(L->x1,PIN_TAB_X[i]))
                {
                 if (((PIN_TAB_Y[i]>L->y1) && (PIN_TAB_Y[i]<L->y2)) 
                    || ((PIN_TAB_Y[i]<L->y1) && (PIN_TAB_Y[i]>L->y2))) 
                   {
                    break_wire(adp, PIN_TAB_X[i], PIN_TAB_Y[i]);
                   }
                }
              }
            }
           }
       }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
}


static void Find_PIN (void)
/*-----------------------*/
{ BLOK  *b;
  LINIA *L;
  char  *adp,  *adk;
  T_Point *ptrs_point;
   
  adp = dane ;
  adk = dane + dane_size ;
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    { if ((L->obiekt == Opoint) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE))
       {
         ptrs_point=(T_Point *)adp;
         if (ptrs_point->typ/*>=8*/>0)
         {
          if (PIN_COUNT<MaxPinCount)
           {
            PIN_COUNT+=1;
            PIN_TAB_X[PIN_COUNT]=ptrs_point->x;
            PIN_TAB_Y[PIN_COUNT]=ptrs_point->y;
            if (PIN_COUNT==MaxPinCount)
             {
              komunikat(89);
             }
           } 
         }
       }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
}


static void Dodaj_Linie (void)
/*--------------------------*/
{ int i;
  if (dodatkowe_linie>0)
   {
    for (i=1; i<=dodatkowe_linie; i++)
    {
     if (NULL == dodaj_obiekt (NULL, (void *)&LINE_TAB[i]))
     {
     }
    }
   }
}

static int oknok(int dwc, int (*DZI)(void *), int (*ODZI)(void *))
{ int ret=1;
  double X0,Y0;
  //char key;
  EVENT *ev;

  redcrok(0);
  while(1)
   { wyj=1;
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0)
      {
		ret=0;
		break;
      }
     if (ev->What == evKeyDown &&  ev->Number==ENTER)
     {
	dwc = win ;	/*tymczasowe, poprawic, np.Get_Win () */
	/*
	najpierw rozcinane sa linie (przewody) poziome i pionowe na ktorych
	leza punkty PIN.
	Najpierw robiona jest lista PIN (max. 2000). Nastepnie przeszukiwane sa
	wszystkie linie nie bedace elementami polilinii, ale mogace byc elementami
	blokow.
	jezeli (y1==y2==y_PIN && x_PIN!=x1 && x_PIN!=x2) (pozioma) 
	 lub (x1==x2==x_PIN && y_PIN!=y1 && y_PIN!=y2) (pionowa)
	to wywolywana jest funkcja break_wire(ad, x_Pin, Y_pin);
	*/
	
	if ((ciag!=0) && (rozcinanie_linii == TRUE))
	 {
	  komunikat0(0);
	  PIN_COUNT=0;
	  Find_PIN();
	  if (PIN_COUNT>0)
	   {
	    dodatkowe_linie_all=0;
add_l:	    dodatkowe_linie=0;
	    Break_Wire();
	    if (dodatkowe_linie>0)
	     {
	      Dodaj_Linie();
	      goto add_l;
	     }
	   }
	  komunikat0(3); 
	 }
	/*kasowanie punktow PIN*/
	PIN_COUNT=0;

	CUR_OFF(X, Y);
	addrem_obiekty (dwc, &O, DZI, ODZI) ;

    flip_screen();

	CUR_ON(X, Y);

	/*tutaj nalezy dolozyc ciagniecie linii zaczepionych w punktach PIN*/
	if ((PIN_COUNT>0) && (ciagniecie_linii == TRUE))
	 {
 	  addrem_linie_pin (dwc, &O, DZI, ODZI) ;
	 }
	if (dwc == Windoww)
	{
	  Set_Object_Temp_Flag (0, 0L, dane_size - 1) ;
	}
	break;
      }
   }
  redcrok(1);
  return ret;
}


static int oknok_items(int items, int dwc, int (*DZI)(void *), int (*ODZI)(void *))
{ int ret=1;
    double X0,Y0;
    //char key;
    EVENT *ev;

    redcrok(0);
    while(1)
    { wyj=1;
        ev=Get_Event_Point(NULL, &X0, &Y0);
        if(ev->What == evKeyDown &&  ev->Number==0)
        {
            ret=0;
            break;
        }
        if (ev->What == evKeyDown &&  ev->Number==ENTER)
        {
            dwc = win ;	/*tymczasowe, poprawic, np.Get_Win () */

            CUR_OFF(X, Y);
            addrem_obiekty_items (items, dwc, &O, DZI, ODZI) ;

            flip_screen();

            CUR_ON(X, Y);

            if (dwc == Windoww)
            {
                Set_Object_Temp_Flag (0, 0L, dane_size - 1) ;
            }
            break;
        }
    }
    redcrok(1);
    return ret;
}



static int oknok_FIRSTB(int dwc, int (*DZI)(void *), int (*ODZI)(void *))
{ int ret=1;
  double X0,Y0;
  EVENT *ev;

  redcrok(0);
  while(1)
   { wyj=1;
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0)
      {
	ret=0;
	break;
      }
     if (ev->What == evKeyDown &&  ev->Number==ENTER)
     {
	dwc = win ;	/*tymczasowe, poprawic, np.Get_Win () */

	/*
	najpierw rozcinane sa linie (przewody) poziome i pionowe na ktorych
	leza punkty PIN.
	Najpierw robiona jest lista PIN (max. 2000). Nastepnie przeszukiwane sa
	wszystkie linie nie bedace elementami polilinii, ale mogace byc elementami
	blokow.
	jezeli (y1==y2==y_PIN && x_PIN!=x1 && x_PIN!=x2) (pozioma) 
	 lub (x1==x2==x_PIN && y_PIN!=y1 && y_PIN!=y2) (pionowa)
	to wywolywana jest funkcja break_wire(ad, x_Pin, Y_pin);
	*/
	
	if ((ciag!=0) && (rozcinanie_linii == TRUE))
	 {
	  komunikat0(0);
	  PIN_COUNT=0;
	  Find_PIN();
	  if (PIN_COUNT>0)
	   {
	    dodatkowe_linie_all=0;
add_l:	    dodatkowe_linie=0;
	    Break_Wire();
	    if (dodatkowe_linie>0)
	     {
	      Dodaj_Linie();
	      goto add_l;
	     }
	   }
	  komunikat0(3); 
	 }
	/*kasowanie punktow PIN*/
	PIN_COUNT=0;
	addrem_obiekty_FIRSTB (dwc, &O, DZI, ODZI) ;

	/*tutaj nalezy dolozyc ciagniecie linii zaczepionych w punktach PIN*/
	if ((PIN_COUNT>0) && (ciagniecie_linii == TRUE))
	 {
 	  addrem_linie_pin (dwc, &O, DZI, ODZI) ;
	 }
	if (dwc == Windoww)
	{
	  Set_Object_Temp_Flag (0, 0L, dane_size - 1) ;
	}
	break;
      }
   }
  redcrok(1);
  return ret;
}

static int oknok_f1(int dwc, int (*DZI)(void *), int (*ODZI)(void *))
{ int ret=1;

   O.x1=X-2.5;
   O.y1=Y-2.5;
   O.x2=X+2.5;
   O.y2=Y+2.5;

	if ((ciag!=0) && (rozcinanie_linii == TRUE))
	 {
	  komunikat0(0);
	  PIN_COUNT=0;
	  Find_PIN();
	  if (PIN_COUNT>0)
	   {
	    dodatkowe_linie_all=0;
add_l:	    dodatkowe_linie=0;
	    Break_Wire();
	    if (dodatkowe_linie>0)
	     {
	      Dodaj_Linie();
	      goto add_l;
	     }
	   }
	  komunikat0(3); 
	 }
	/*kasowanie punktow PIN*/
	PIN_COUNT=0;
   ADD_NEW_PIN=TRUE;

	addrem_obiekty (dwc, &O, DZI, ODZI) ;
   ADD_NEW_PIN=FALSE;

	/*tutaj nalezy dolozyc ciagniecie linii zaczepionych w punktach PIN*/
	if ((PIN_COUNT>0) && (ciagniecie_linii == TRUE))
	 {
 	  addrem_linie_pin (dwc, &O, DZI, ODZI) ;
	 }

	if (dwc == Windoww)
	{
	  Set_Object_Temp_Flag (0, 0L, dane_size - 1) ;
	}
   redcrok(1);

  return ret;
}

/*--------------------------------------------*/
/*-------   blok  -------------------------*/

BOOL check_if_block_in_win (long_long offp, long_long offk, int dwc)
/*---------------------------------------------------------------*/
{
  NAGLOWEK *ad;
  int w;
  long_long off;
  ObiektTok (&offp, offk, &off, ONieOkreslony);
  while (off != -1)
  {
    ad =(NAGLOWEK*)(dane + off) ;
    w = 0 ;
    switch(ad->obiekt)
    {
       case Olinia :
	  w = linia_dwc ((LINIA*)ad, dwc) ;
	  break;
       case Otekst :
   	  w=tekst_dwc((TEXT*)ad,dwc);
        //tutaj mozna dopuscic teksty ukryte, jezeli nie sa wyswietlane
        if ((w==0) && (((TEXT*)ad)->ukryty==1) && (type_view==0))
         { w=1; }
	  break;
       case Oluk :
	  w=luk_dwc((LUK*)ad,dwc);
	  break;
       case Ookrag :
       case Okolo   :
	  w=okrag_dwc((OKRAG*)ad,dwc);
	  break;
       case Owwielokat   :
	  w=wielokat_dwc((WIELOKAT*)ad,dwc);
	  break;
	   case Ospline:
	   w = spline_dwc((SPLINE*)ad, dwc);
	   break;
        case Oellipse   :
        case Ofilledellipse:
       w = ellipse_dwc((ELLIPSE*)ad,dwc);
       break;
        case Oellipticalarc:
        w = ellipticalarc_dwc((ELLIPTICALARC*)ad,dwc);
        break;
        case Ovector:
        w = vector_dwc((AVECTOR*)ad,dwc);
        break;
       default :
	   w = 1 ;
	  break;
    }
    if (w == 0)
    {
      return FALSE ;
    }
    ObiektTok (&offp, offk, &off, ONieOkreslony) ;
  }
   return TRUE ;
}

static void adrem_obiekt_info(void *ad, int (*DZI)(void *), int (*ODZI)(void *) )
/*-------------------------------------------------------------------------*/
{ NAGLOWEK *nag;

  nag=(NAGLOWEK*)ad;
  if( (nag->atrybut==Anormalny || nag->atrybut==Aoblok || nag->atrybut==Apoblok
      || nag->atrybut==Appoblok || nag->atrybut==Apppoblok  ) 
     && add ==ADD_MODE && DZI(ad))
   { if( ADP==NULL || ( ADP!=NULL && (char *)ad < ADP ) )
				ADP=(char *)ad;
     if( (char *)ad > ADK ) ADK=(char *)ad;
   }
  if(nag->atrybut==Ablok && add == DEL_MODE)  ODZI(ad);
}

static void adrem_obiekt_(void *ad, int (*DZI)(void *), int (*ODZI)(void *) )
/*-------------------------------------------------------------------------*/
{ NAGLOWEK *nag;

  nag=(NAGLOWEK*)ad;
  if (select_no_edit_layers==0)
   {
    if (nag->obiekt != OdBLOK && Layers [((LINIA*)nag)->warstwa].edit == 0)
    {
      return;
    }
   }
  if( (nag->atrybut==Anormalny || nag->atrybut==Ablok || nag->atrybut==Aoblok || nag->atrybut==Apoblok
      || nag->atrybut==Appoblok || nag->atrybut==Apppoblok  ) 
     && add ==ADD_MODE && DZI(ad))
   { if( ADP==NULL || ( ADP!=NULL && (char *)ad < ADP ) )
				ADP=(char *)ad;
     if( (char *)ad > ADK ) ADK=(char *)ad;
   }
  if(nag->atrybut==Ablok && add == DEL_MODE)  ODZI(ad);
}

static void *adxxx;

static void adrem_blok (BLOK *adb, int (*DZI)(void *), int (*ODZI)(void *), BOOL b_win_sel)
/*---------------------------------------------------------------------------------------*/
{
  char *ad;
  char  *adh;
  long_long offp, offk, off ;
  T_Point *ptrs_point;

  if (adb == NULL)
  {
    ((NAGLOWEK*)adxxx)->blok = NoElemBlok;
    adrem_obiekt_ (adxxx,DZI,ODZI);
    return;
  }

  if (((NAGLOWEK*)adb)->obiekt == OdBLOK)
  {
     if ((add == ADD_MODE) && (((NAGLOWEK*)adb)->atrybut == Ablok)) return;  //// WARNING
     else if ((add == DEL_MODE) && (((NAGLOWEK*)adb)->atrybut == Anormalny)) return;  //// WARNING
  }

    adh = (char *)adb + sizeof(NAGLOWEK) + adb->n - 1 ;
  if (TRUE == check_if_edit ((char *)adb, adh, ANieOkreslony))
  {
    komunikat (79);
    return ;
  }
  if (TRUE == check_if_hidden ((char *)adb, adh, ANieOkreslony))
  {
   komunikat (78); 
  }
  offp = ((char  *)adb) - dane ;
  offk = adh - dane ;
  if (win == Windoww && TRUE == b_win_sel)
  {
    if (FALSE == check_if_block_in_win (offp, offk , win))
    {
      Set_Object_Temp_Flag (1, offp, offk) ;
      return ;
    }
  }

  Object_Tok (&offp, offk, &off, ONieOkreslony, TOK_LAYER_ALL) ;
  while (off != -1)
  {
    ad = (char*)(dane + off) ;
    if (((NAGLOWEK*)ad)->obiekt == OdBLOK) /*jest to absolutnie niepotrzebne*/
    {					    /*poniewaz import bloku zapala ta flage*/
      ;
    }
    else ((NAGLOWEK*)ad)->blok = 1 ;   //correction of the blok, in case something went wrong and within blok entite has no blok flag  //WARNING

    adrem_obiekt_ (ad, DZI, ODZI) ;

    /*dla ciagniecia*/
    if ((ciag!=0) && (ciagniecie_linii == TRUE))
      {
       if (((NAGLOWEK*)ad)->obiekt == Opoint)
        {
         ptrs_point=(T_Point *)ad;
         if (ptrs_point->typ/*>=8*/>0)
         {
          if (PIN_COUNT<MaxPinCount)
           {
            PIN_COUNT+=1;
            PIN_TAB_X[PIN_COUNT]=ptrs_point->x;
            PIN_TAB_Y[PIN_COUNT]=ptrs_point->y;
            if (PIN_COUNT==MaxPinCount)
             {
              komunikat(89);
             }
           } 
         }
        }

      } 
    
    Object_Tok (&offp, offk, &off, ONieOkreslony, TOK_LAYER_ALL) ;
  }
  ////adb->blok = 0;   /*zaszlosc, import bloku zapalal niepotrzebnie ta flage dla bloku najbardziej zewnetrznego*/
		   /*niepotrzebne, set_block_flag ustawia prawidlowo flage bloku*/

}

void adrem_blok1 (BLOK *adb, int atrybut)
{
  char *ad;
  char  *adh;
  long_long offp, offk, off ;

  if (adb == NULL)
  {
    return;
  }
  adh = (char *)adb + sizeof(NAGLOWEK) + adb->n - 1 ;
  offp = ((char  *)adb) - dane ;
  offk = adh - dane ;

  Object_Tok (&offp, offk, &off, ONieOkreslony, TOK_LAYER_ALL) ;
  while (off != -1)
  {
    ad = (char*)(dane + off) ;
    
    ((NAGLOWEK*)ad)->atrybut = atrybut ;
    
    Object_Tok (&offp, offk, &off, ONieOkreslony, TOK_LAYER_ALL) ;
  }
  adb->blok = 0;   
}

static int usun_element_bloku_;

void adrem_obiekt (void *ad, int (*DZI)(void *), int (*ODZI)(void *), BOOL b_win_sel)
/*-----------------------------------------------------------------------------------------*/
{
  NAGLOWEK *nag;

  nag = (NAGLOWEK*)ad ;
  adxxx = ad ;


  if (nag->blok == ElemBlok   &&   usun_element_bloku_ == 0  &&  select_element_only == 0)
  {
    if (ciag == 0) {
        if (b_win_sel)  adrem_blok(LASTB_A((char *) ad), DZI, ODZI, b_win_sel);
        else adrem_blok(LASTB((char *) ad), DZI, ODZI, b_win_sel);
    }
      else  /*jezeli ciagniecie to w bloku mozna ciagnac jedynie polilinie
             i wielokat, lub blok wymiarowy*/
        {
          if ((nag->obiektt2 == O2BlockDim) || (nag->obiektt2 == O2BlockPline)
            || (swobodny_blok == TRUE))
              adrem_obiekt_ (ad, DZI, ODZI) ;
            else
              {
                  if (b_win_sel) adrem_blok (LASTB_A((char *)ad), DZI, ODZI, b_win_sel) ;
                  else adrem_blok (LASTB((char *)ad), DZI, ODZI, b_win_sel) ;
              }
        }
  }
  else
  {
      adrem_obiekt_ (ad, DZI, ODZI) ;
  }
}

void adrem_obiekt_FIRSTB (void *ad, int (*DZI)(void *), int (*ODZI)(void *), BOOL b_win_sel)
/*-----------------------------------------------------------------------------------------*/
{
  NAGLOWEK *nag;

  nag = (NAGLOWEK*)ad ;
  adxxx = ad ;


  if (nag->blok == ElemBlok   &&   usun_element_bloku_ == 0  &&  select_element_only == 0)
  {
	if (ciag == 0) adrem_blok(LASTB((char *)ad), DZI, ODZI, b_win_sel);
      else  /*jezeli ciagniecie to w bloku mozna ciagnac jedynie polilinie
             i wielokat, lub blok wymiarowy*/
        {
          if ((nag->obiektt2 == O2BlockDim) || (nag->obiektt2 == O2BlockPline)
            || (swobodny_blok == TRUE))
              adrem_obiekt_ (ad, DZI, ODZI) ;
            else
              {
               adrem_blok (LASTB((char *)ad), DZI, ODZI, b_win_sel) ;
              }
        }
  }
  else
  {
    adrem_obiekt_ (ad, DZI, ODZI) ;
  }
}

void adrem_obiekt1 (void *ad, int atrybut0)
/*---------------------------------------*/
{
  NAGLOWEK *nag;
  int atrybut;

  nag = (NAGLOWEK*)ad ;
  adxxx = ad ;
  atrybut = atrybut0;

  if (nag->blok == ElemBlok) 
  {
    adrem_blok1 (LASTB_A((char *)ad), atrybut) ; //LASTB_A
  }
}

static void *obiekt_wybrany (void)
/*------------------------------*/
{ unsigned typ;
  void *adr;
  if (global_no_pcx) typ = Blinia | Bluk | Bokrag | Bkolo | Btekst | Bwwielokat | Bspline | Bpoint | Bellipse | Bfilledellipse | Bellipticalarc | Bsolidarc | Bvector;
  if (global_only_pcx) typ = Bpcx;
  else if (global_only_solid) typ = Bwwielokat | Bsolidarc;
  else typ=Blinia | Bluk | Bokrag | Bkolo | Btekst | Bwwielokat | Bspline | Bpoint | Bpcx | Bellipse | Bfilledellipse |Bellipticalarc | Bsolidarc | Bvector;
  return select_w(&typ,&adr);
}

//static
void *obiekt_wybrany_select (unsigned int Bobjects)
/*----------------------------------------------*/
{ unsigned typ;
    void *adr;
    //if (global_no_pcx) typ = Blinia | Bluk | Bokrag | Bkolo | Btekst | Bwwielokat | Bspline | Bpoint | Bellipse | Bfilledellipse | Bellipticalarc | Bsolidarc | Bvector;
    //if (global_only_pcx) typ = Bpcx;
    //else if (global_only_solid) typ = Bwwielokat | Bsolidarc;
    //else typ=Blinia | Bluk | Bokrag | Bkolo | Btekst | Bwwielokat | Bspline | Bpoint | Bpcx | Bellipse | Bfilledellipse |Bellipticalarc | Bsolidarc | Bvector;
    typ = Bobjects;
    return select_w(&typ,&adr);
}

static void *obiekt_wybrany_info (void)
/*------------------------------*/
{ unsigned typ;
  void *adr;
  typ=Blinia | Bluk | Bokrag | Bkolo | Btekst | Bwwielokat | Bspline | Bpoint | Bpcx | Bellipse | Bfilledellipse |Bellipticalarc | Bsolidarc | Bvector;
  return select_w_info(&typ,&adr);
}

static void* obiekt_wybrany_wielokat(void)
/*--------------------------------------*/
{
	unsigned typ;
	void* adr;
	typ = Bwwielokat;
	return select_w_info(&typ, &adr);
}

static void* obiekt_wybrany_typ(unsigned int typ)
/*---------------------------------------------*/
{
    void* adr;
    return select_w_info(&typ, &adr);
}

/*--------------------------------------------------*/

/*-------------------------------------------------*/
static void  nooop(void)
{}

static void redcr(char typ,void (*REDROWd)(void))
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[2])(),sel_akt,sel_cur, sel_gor, sel_nr;
  static void (*REDdod)(void)=nooop;
  if (typ == 0)
  {
	  Cur_offd(X, Y);

	  sel_akt = sel.akt; sel.akt = 1;
	  sel_cur = sel.cur; sel.cur=1;
	  sel_gor = sel.gor; sel.gor = 0;
	  sel_nr = sel.nr;

	  CUR_oFF = CUR_OFF;
	  CUR_oN = CUR_ON;

	  if (global_only_solid)
	  {
		  sel.gor = 1;
		  CUR_OFF = cursel_off; //out_sel_off; //cursel_off; // out_sel;
		  CUR_ON = cursel_on;//out_sel_on; //cursel_on; // out_sel;
	  }
	  else
	  {
		  CUR_OFF = out_sel_off; //cursel_off; // out_sel;
		  CUR_ON = out_sel_on; //cursel_on; // out_sel;
	  }
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     usun_element_bloku_=0;
     BlokM=0;
     if(!ciag)
     {
	ADP=NULL;
	ADK=NULL;
     }
     REDdod=REDDOD; REDDOD=REDROWd;
	 if (global_only_solid) CUR_ON(X,Y);
	 else Cur_ond(X, Y);
	
   }
  else if (typ==1)
   {
	 Cur_offd(X, Y);
	 
     REDDOD=REDdod;
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=(void*)SW[0];
     SERV[81]= (void*)SW[1];

     usun_element_bloku_=0;
     sel.akt=sel_akt;
     sel.cur=sel_cur;
	 sel.gor = sel_gor;
	 sel.nr = sel_nr;

	 layer_info();
	
	 Cur_ond(X, Y);
   }
   else if (typ==2)
    {
     SERV[73]= (void*)SW[0];
     SERV[81]= (void*)SW[1];
    }
    else if (typ==3)
    {
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
    } 
}

//blok i Block_Proc poprawiac rownolegle*/

void blok(int (*DZI)(void *), int (*ODZI)(void *),
		 void (*REDROWd)(void),const void (*COMND[])(void))
{
  void *ad;
  EVENT *ev;
  double X0, Y0;
  
  redcr(0,REDROWd);
  while (1)
  {
	  komunikaty_bloku = FALSE;
	  ev = Get_Event_Point(NULL, &X0, &Y0);
	  if (ev->What == evKeyDown)
	  {
		  if (ev->Number == 0)
		  {
			  redcr(1, REDROWd);
			  return;
		  }
		  if (ev->Number == ENTER)
		  {
			  if ((ad = obiekt_wybrany()) != NULL)
			  {
				  adrem_obiekt(ad, DZI, ODZI, FALSE);
                  CUR_OFF(X,Y);
			  }
			  else
				  if (b_auto == TRUE)
				  {
					  redcr(2, REDROWd);
					  oknok(win, DZI, ODZI);
					  redcr(3, REDROWd);
				  }
		  }
	  }
	  else if (ev->What == evCommandP)
	    (*COMND[ev->Number])();

   }
}

void blok_items(int items, int (*DZI)(void *), int (*ODZI)(void *),
          void (*REDROWd)(void),const void (*COMND[])(void))
{
    void *ad;
    EVENT *ev;
    double X0, Y0;

    redcr(0,REDROWd);
    while (1)
    {
        komunikaty_bloku = FALSE;
        ev = Get_Event_Point(NULL, &X0, &Y0);
        if (ev->What == evKeyDown)
        {
            if (ev->Number == 0)
            {
                redcr(1, REDROWd);
                return;
            }
            if (ev->Number == ENTER)
            {
                //if ((ad = obiekt_wybrany()) != NULL)
                if ((ad = obiekt_wybrany_select(items)) != NULL)
                {
                    adrem_obiekt(ad, DZI, ODZI, FALSE);
                    CUR_OFF(X,Y);
                }
                else
                if (b_auto == TRUE)
                {
                    redcr(2, REDROWd);
                    oknok_items(items, win, DZI, ODZI);
                    redcr(3, REDROWd);
                }
            }
        }
        else if (ev->What == evCommandP)
            (*COMND[ev->Number])();

    }
}

int blok_single(int (*DZI)(void *), int (*ODZI)(void *),
          void (*REDROWd)(void), int (*COMND[])(void))
{
    void *ad;
    EVENT *ev;
    double X0, Y0;

    redcr(0,REDROWd);
    while (1)
    {
        komunikaty_bloku = FALSE;
        ev = Get_Event_Point(NULL, &X0, &Y0);
        if (ev->What == evKeyDown)
        {
            if (ev->Number == 0)
            {
                redcr(1, REDROWd);
                return 0;
            }
            if (ev->Number == ENTER)
            {
                alert_on_object=FALSE;
                if ((ad = obiekt_wybrany_select(Bvector)) != NULL)
                {
                    adrem_obiekt(ad, DZI, ODZI, FALSE);
                    if (!alert_on_object)
                    {
                        redcr(1, REDROWd);
                        return 1;
                    }
                }

            }
        }
        else if( ev->What == evCommandP &&
                 TRUE == (*COMND[ev->Number]) ())
        {
            redcr(1, REDROWd);
            return -1;
        }
    }
}


int blok_FIRSTB(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),void (*COMND[])(void))
{
  void *ad;
  EVENT *ev;
  double X0, Y0;
  
  redcr(0,REDROWd);
  while(1)
   {
     komunikaty_bloku=FALSE;
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What==evKeyDown)
      { if(ev->Number==0)
	 {  redcr(1,REDROWd);
	   return 0;
	 }
	if(ev->Number==ENTER)
	{
	  if ((ad = obiekt_wybrany ()) != NULL)
	  {
	    adrem_obiekt_FIRSTB (ad, DZI, ODZI, FALSE) ;
		redcr(1, REDROWd);
       break;
	  }
	 }
      }
     else if( ev->What==evCommandP)
      (*COMND[ev->Number])();
   }
  komunikaty_bloku=FALSE;
  return 1;
}




int blok_Ctrl_R(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),void (*COMND[])(void))
{
  void *ad;
  
  redcrc(0);
  CUR_OFF(X,Y);
  if ((ad = obiekt_wybrany ()) != NULL)
   {
    adrem_obiekt (ad, DZI, ODZI, FALSE) ;
    redcrc(1);
	 return 1;
   }
    else
     { redcrc(1);
	    return 0;
     }
}

void blok_special(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),void (*COMND[])(void))
{
  void *ad;
  EVENT *ev;
  double X0, Y0;
  int k;
  
  redcr(0,REDROWd);
  k=wszystko_special();
  while(1)
   {
     komunikaty_bloku=FALSE;
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What==evKeyDown)
      { if(ev->Number==0)
	 {  redcr(1,REDROWd);
	   return;
	 }
	if(ev->Number==ENTER)
	{
	  CUR_OFF(X,Y);
	  if ((ad = obiekt_wybrany ()) != NULL)
	  {
	    adrem_obiekt (ad, DZI, ODZI, FALSE) ;
	  }
	  else
	  if (b_auto == TRUE)
	  {
	    redcr(2,REDROWd); 
	    oknok (win, DZI, ODZI) ;
	    redcr(3,REDROWd);
	  }
	  CUR_ON(X,Y);
	 }
      }
     else if( ev->What==evCommandP)
      (*COMND[ev->Number])();
   }
  komunikaty_bloku=FALSE; 
}

//blok i Block_Proc poprawiac rownolegle*/

int Block_Proc_Wez_w (int (*DZI)(void *), int (*ODZI)(void *),
		 void (*REDROWd)(void), int (*COMND[])(void), int info)
//---------------------------------------------------------------------
{
//kopia funkcji blok, COMND i Block_Proc zwracaja wartosc,

  void *ad;
  EVENT *ev;
  double X0, Y0;

  redcr(0,REDROWd);
  while(1)
   {
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What==evKeyDown)
      { if(ev->Number==0)
	 { redcr(1,REDROWd);
	   return 0 ;
	 }
	if(ev->Number==ENTER)
	{
	  CUR_OFF(X,Y);
	  if (info==1)
	  {
 	   if ((ad = obiekt_wybrany_info ()) != NULL)
	   {
	     adrem_obiekt_info (ad, DZI, ODZI /*, FALSE*/) ;
	     CUR_ON(X,Y);
	     redcr(1,REDROWd);
	     return 1 ;
	   }
	  }
	  else if (info==0)
	  {
	   if ((ad = obiekt_wybrany ()) != NULL)
	   {
	    adrem_obiekt_ (ad, DZI, ODZI /*, FALSE*/) ;
	    CUR_ON(X,Y);
	    redcr(1,REDROWd);
	    return 1 ;
	   }
	  }
      else if ((ad = obiekt_wybrany_typ (info >> 8)) != NULL)
      {
          adrem_obiekt_ (ad, DZI, ODZI /*, FALSE*/) ;
          CUR_ON(X,Y);
          redcr(1,REDROWd);
          return 1 ;
      }
	  CUR_ON(X,Y);
	 }
      }
     else
     {
       if( ev->What == evCommandP &&
           TRUE == (*COMND[ev->Number]) ())
       {
	 redcr (1, REDROWd) ;
	 return 1 ;
       }
     }
   }
  redcr (1, REDROWd) ; 
  return 1; 
}

int Block_Proc_Wez_Element (int (*DZI)(void *), int (*ODZI)(void *),
                      void (*REDROWd)(void), int (*COMND[])(void), int style)
//---------------------------------------------------------------------------
{
//kopia funkcji blok, COMND i Block_Proc zwracaja wartosc,

    void *ad;
    EVENT *ev;
    double X0, Y0;

    redcr(0,REDROWd);
    while(1)
    {
        ev=Get_Event_Point(NULL, &X0, &Y0);
        if(ev->What==evKeyDown)
        { if(ev->Number==0)
            { redcr(1,REDROWd);
                return 0 ;
            }
            if(ev->Number==ENTER)
            {
                CUR_OFF(X,Y);
                if ((ad = obiekt_wybrany_typ (Bvector)) != NULL)
                {
                    adrem_obiekt_ (ad, DZI, ODZI /*, FALSE*/) ;
                    CUR_ON(X,Y);
                    redcr(1,REDROWd);
                    return 1 ;
                }
                CUR_ON(X,Y);
            }
        }
        else
        {
            if( ev->What == evCommandP &&
                TRUE == (*COMND[ev->Number]) ())
            {
                redcr (1, REDROWd) ;
                return 1 ;
            }
        }
    }
    redcr (1, REDROWd) ;
    return 1;
}

BOOL Block_Proc_Set_w(int (*DZI)(void*), int (*ODZI)(void*),
	void (*REDROWd)(void), int (*COMND[])(void), int info)
	//---------------------------------------------------------------------
{
	//kopia funkcji blok, COMND i Block_Proc zwracaja wartosc,

	void* ad;
	EVENT* ev;
	double X0, Y0;

	redcr(0, REDROWd);
	while (1)
	{
		ev = Get_Event_Point(NULL, &X0, &Y0);
		if (ev->What == evKeyDown)
		{
			if (ev->Number == 0)
			{
				redcr(1, REDROWd);
				return FALSE;
			}
			if (ev->Number == ENTER)
			{
				CUR_OFF(X, Y);
				if (info == 1)
				{
					if ((ad = obiekt_wybrany_info()) != NULL)
					{
						adrem_obiekt_info(ad, DZI, ODZI /*, FALSE*/);
						CUR_ON(X, Y);
						redcr(1, REDROWd);
						return TRUE;
					}
				}
				else
				{
					if ((ad = obiekt_wybrany()) != NULL)
					{
						adrem_obiekt_(ad, DZI, ODZI /*, FALSE*/);
						CUR_ON(X, Y);
						redcr(1, REDROWd);
						//return TRUE;
						redcr(0, REDROWd);
						continue;
					}
				}
				CUR_ON(X, Y);
			}
		}
		else
		{
			if (ev->What == evCommandP &&
				TRUE == (*COMND[ev->Number]) ())
			{
				redcr(1, REDROWd);
				return TRUE;
			}
		}
	}
	redcr(1, REDROWd);
	return TRUE;
}



BOOL Block_Proc_Wez_hw (int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void), int (*COMND[])(void), int info)
//---------------------------------------------------------------------------------------------------------------------
{
//kopia funkcji blok, COMND i Block_Proc zwracaja wartosc,

  void *ad;
  EVENT *ev;
  double X0, Y0;

  redcr(0,REDROWd);
  while(1)
   {
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What==evKeyDown)
      { if(ev->Number==0)
	 { redcr(1,REDROWd);
	   return FALSE ;
	 }
	if(ev->Number==ENTER)
	{
	  CUR_OFF(X,Y);
	  if (info==1)
	  {
 	   if ((ad = obiekt_wybrany_info ()) != NULL)
	   {
	     adrem_obiekt_info (ad, DZI, ODZI /*, FALSE*/) ;
	     CUR_ON(X,Y);
	     redcr(1,REDROWd);
	     return TRUE ;
	   }
	  }
	  else 
	  {
	   if ((ad = obiekt_wybrany ()) != NULL)
	   {
	    force_redraw=FALSE;
		error_layer=0;
	    adrem_obiekt_ (ad, DZI, ODZI /*, FALSE*/) ;
		ADP=ADK=NULL;
	    if (force_redraw) 
		  {
			  redraw();
		      CUR_ON(X,Y);
		  }
	   }
	  else 
	  if (b_auto == TRUE)
	  {
	    force_redraw=FALSE;
		error_layer=0;
	    redcr(2,REDROWd);
	    oknok (win, DZI, ODZI) ;
		ADP=ADK=NULL;
		if (force_redraw) 
		{
			redraw();
            CUR_ON(X,Y);
		}
	    redcr(3,REDROWd);
	  }  
	 }
	  CUR_ON(X,Y);
	 }
      }
     else
     {
       if( ev->What == evCommandP &&
           TRUE == (*COMND[ev->Number]) ())
       {
	 redcr (1, REDROWd) ;
	 return TRUE ;
       }
     }
   }
  redcr (1, REDROWd) ; 
  return TRUE; 
}


BOOL Block_Proc (int (*DZI)(void *), int (*ODZI)(void *),
		 void (*REDROWd)(void), int (*COMND[])(void))
//------------------------------------------------------------
{
//kopia funkcji blok, COMND i Block_Proc zwracaja wartosc,

  void *ad;
  EVENT *ev;
  double X0, Y0;

  redcr(0,REDROWd);
  while(1)
   {
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What==evKeyDown)
      { if(ev->Number==0)
	 { redcr(1,REDROWd);
	   return FALSE ;
	 }
	if(ev->Number==ENTER)
	{
     deleted_pcx=FALSE;
	 deleted_on_front = FALSE;
	 deleted_TTF = FALSE;
	 deleted_pattern = FALSE;
	  if ((ad = obiekt_wybrany ()) != NULL)
	  {
		CUR_OFF(X, Y);
	    adrem_obiekt (ad, DZI, ODZI, FALSE) ;
		CUR_ON(X, Y);
	  }
	  else
	  if (b_auto == TRUE)
	  {
	    redcr(2,REDROWd);
	    oknok (win, DZI, ODZI) ;
	    redcr(3,REDROWd);
	  }

	 ////flip_screen();
      ////CUR_ON(X,Y);
     //sprawdzenie czy wsrod usunietych obiektow sa bitmapy
    if ((deleted_pcx==TRUE) || (deleted_TTF == TRUE) || deleted_pattern==TRUE || deleted_on_front == TRUE)
     {
      if (deleted_pcx == TRUE) regen_ctx=TRUE;
      redraw();
     }
     CUR_OFF(X,Y);
     CUR_ON(X,Y);
	 }
      }
     else
     {
       if( ev->What == evCommandP &&
           TRUE == (*COMND[ev->Number]) ())
       {
	 redcr (1, REDROWd) ;
	 return TRUE ;
       }
     }
   }
}

static void redcrc(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  if (typ==0)
   { 
	 CUR_OFF(X, Y);
	 CUR_oFF=CUR_OFF; CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;   CUR_ON=cursel_on;
     ADP=NULL;
     ADK=NULL;
     CUR_ON(X,Y);
   }
  else
   {
	 CUR_OFF(X, Y);
     memmove(&Oc,&O,sizeof(OKNO));
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
	 CUR_ON(X, Y);
   }
}

int blokc(int (*DZI)(void *), int (*COMND[])(void))
{
  EVENT *ev;
  double X0, Y0;
  int ret_oknok;

  redcrc(0);
  while(1)
   {
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0)
      {
		redcrc(1);
		return 0;
      }

     if( ev->What == evCommandP &&
           TRUE == (*COMND[ev->Number]) ())
     {
         return -1;
     }
     if (ev->What == evKeyDown &&  ev->Number==ENTER)
      {
		ret_oknok=oknok(Dragg,DZI,nooop1);
		if (ret_oknok==1)
		 { 
		   redcrc(1);
		   return 1;
		 }
		else
		{
			redcrc(1);
		}
      }
   }
}
