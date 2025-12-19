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

#define __O_KONSTR__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#ifndef LINUX
#include<io.h>
#include<dos.h>
#endif
#include<fcntl.h>
#include<string.h>
#include<math.h>

#include<ctype.h>
#include<limits.h>
#include<stdlib.h>
#ifndef LINUX
#include<process.h>
#else

#endif
#include<stdio.h>
#include<stdarg.h>
#include<sys/stat.h>
#include<values.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_bloklb.h"

#include "menu.h"

#include "leak_detector_c.h"


extern void komunikat_str_short(char *st, BOOL stay, BOOL center);
extern void PlineW1 (char *blok_type);
extern BOOL Check_if_Equal (double x, double y);


static char zb_glowne[30];

static double srednica_preta_tab[20]={3.0,3.5,4.0,4.5,5.0,5.5,6.0,8.0,10.0,12.0,
                               14.0,16.0,18.0,20.0,22.0,25.0,28.0,32.0,36.0,40.0};


static POLE pmNets0[]={
   {u8"Ø3",' ',0,NULL},   //"č3"
   {u8"Ø3.5",' ',0,NULL},
   {u8"Ø4",' ',0,NULL},
   {u8"Ø4.5",' ',0,NULL},
   {u8"Ø5",' ',0,NULL},
   {u8"Ø5.5",' ',0,NULL},
   {u8"Ø6",' ',0,NULL},
   {u8"Ø8",' ',0,NULL},
   {u8"Ø10",' ',0,NULL},
   {u8"Ø12",' ',0,NULL},
   {u8"Ø14",' ',0,NULL},
   {u8"Ø16",' ',0,NULL},
   {u8"Ø18",' ',0,NULL},
   {u8"Ø20",' ',0,NULL},
   {u8"Ø22",' ',0,NULL},
   {u8"Ø25",' ',0,NULL},
   {u8"Ø28",' ',0,NULL},
   {u8"Ø32",' ',0,NULL},
   {u8"Ø36",' ',0,NULL},
   {u8"Ø40",' ',0,NULL},
   };

static TMENU mNets0={20,0,0,6,1,3,0,CMNU,CMBR,CMTX,0,0,0,0,0,&pmNets0,NULL,NULL};

static char *nets_tab0[]={u8"Ø3",u8"Ø3.5",u8"Ø4",u8"Ø4.5",u8"Ø5",u8"Ø5.5",
                         u8"Ø6",u8"Ø8",u8"Ø10",u8"Ø12",u8"Ø14",u8"Ø16",u8"Ø18",
                         u8"Ø20",u8"Ø22",u8"Ø25",u8"Ø28",u8"Ø32",u8"Ø36",u8"Ø40"};


static BOOL add_block_with_type (double x, double y, char *blok_type0, char obiektt2, char *prefix, char kod_obiektu)
/*------------------------------------------------------------------------------------------------------------------*/
{
  unsigned int size_block ;
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


  strcpy(blok_type, prefix);
  strcat(blok_type,blok_type0);
  len_type = strlen (blok_type) + 1 ;
  len_desc = sizeof(unsigned) + 2 * sizeof (float) + sizeof(len_type) + len_type ;
  size_block = B3 + len_desc ;
  
  if (NULL == (buf_block = (BLOK*) malloc /*getmem*/ (sizeof(NAGLOWEK) + size_block)))
  {
    return FALSE;
  }
  memcpy (buf_block, &blokd, sizeof (blokd)) ;
  buf_block->n = size_block ;
  buf_block->kod_obiektu = kod_obiektu;
  buf_block->obiektt2=obiektt2;
  buf_block->dlugosc_opisu_obiektu = len_desc ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&buf_block->opis_obiektu [0]) ;
  ptrs_desc_bl->flags = EBF_IP ;
  ptrs_desc_bl->x = x ;
  ptrs_desc_bl->y = y ;
  ptrs_desc_bl->len = len_type ;
  strcpy (&ptrs_desc_bl->sz_type [0], blok_type) ;
  if ((ptrs_block = (BLOK*)dodaj_obiekt (NULL, buf_block)) == NULL)
  {
      free(buf_block);
    return FALSE;
  }
  free(buf_block);
  return TRUE ;
}


BOOL group_block_with_type (long dane_size000, double x, double y, char *blok_type0, char obiektt2, char *prefix, char kod_obiektu)
/*---------------------------------------------------------------------------------------------------------------------------------------*/
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

   if (FALSE == add_block_with_type (x, y, blok_type0, obiektt2, prefix, kod_obiektu))
   {
    return FALSE;
   }

  adr=dane;

  b=(BLOK *)dane;
  adrk=adr + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu + dane_size000 - 1 ;//????????  ?

  nag=(NAGLOWEK*)adr;
  first_block=FALSE;

  while ((nag->obiekt!=Okoniec) && (adr<=adrk))
  {

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

#define max_new_texts 31

void Wymiarowanie_polilinii1(char *adr0, int opcja)
/*----------------------------------------------*/
{ BLOK *ptrs_block, *ptrs_block1;
  char *adp, *adk;
  NAGLOWEK *nag;
  LINIA *L;
  LUK *l;
  int new_texts;
  TEXT Tn[32];
  TEXT Tn0=Tdef, Tn1=Tdef;
  double max_length, c_x, c_y, c_angle;
  double poliline_length;
  double len_pline;
  double line_angle;
  int i;
  double del_kat;
  double l_x1,l_y1,l_x2,l_y2;
  double dh_text;     //0.5 [mm]
  double dh1_text;    //3.0 [mm]

  dh_text=0.75;
  dh1_text=zmwym.wysokosc+1.25;

  Tn0.kolor=zmwym.Tkolor;
  Tn0.wysokosc=zmwym.wysokosc;
  Tn0.width_factor=zmwym.width_factor;
  Tn0.czcionka=zmwym.czcionka;
  Tn0.bold=zmwym.bold;
  Tn0.italics=zmwym.italics;

  new_texts=0;
  len_pline=0;
  poliline_length=0;

  ptrs_block = (BLOK*)adr0;
  adp=adr0+sizeof(NAGLOWEK)+B3+ptrs_block->dlugosc_opisu_obiektu;
  adk=adr0+sizeof(NAGLOWEK)+ptrs_block->n-1;
  nag = (NAGLOWEK*)adp;

  while ((nag->obiekt != Okoniec) && (adp < adk))
   {
    if (nag->obiekt == OdBLOK)
    {
      ptrs_block1 = (BLOK*)adp;
      adp+=(sizeof(NAGLOWEK)+B3+ptrs_block1->dlugosc_opisu_obiektu);
    }
     else if (nag->obiekt == Olinia)
      {
        L = (LINIA*)adp;
        //dimensioning of polyline section;
        if (new_texts<max_new_texts)
         {
          new_texts++;
          memmove(&Tn[new_texts-1], &Tn0, sizeof(TEXT));
          Tn[new_texts-1].x=(L->x1+L->x2)/2;
          Tn[new_texts-1].y=(L->y1+L->y2)/2;
          //length calculation
          len_pline=sqrt(((L->x2-L->x1)*(L->x2-L->x1))+
                     ((L->y2-L->y1)*(L->y2-L->y1)));

          len_pline*=(SkalaF/Jednostki);  //w cm
          poliline_length+=(floor(len_pline+0.5));
          if (Check_if_Equal(L->x2,L->x1)==TRUE) line_angle=Pi/2;
            else line_angle=atan((L->y2-L->y1)/(L->x2-L->x1));
          Tn[new_texts-1].kat=line_angle;
          Tn[new_texts-1].x-=(dh_text*sin(line_angle));
          Tn[new_texts-1].y+=(dh_text*cos(line_angle));
          Tn[new_texts-1].justowanie=j_srodkowo;
          sprintf(Tn[new_texts-1].text,"%.0f",len_pline);
          Tn[new_texts-1].dl=strlen(Tn[new_texts-1].text);
          Tn[new_texts-1].n=T18+Tn[new_texts-1].dl;
		  Tn[new_texts - 1].width = 0;
		  Tn[new_texts - 1].height = 0;
          Tn[new_texts-1].blok=1;
          Tn[new_texts-1].warstwa=Current_Layer;
          Tn[new_texts-1].ukryty=0;
          if (new_texts==1)
           {
             max_length=len_pline;
             c_x=(L->x1+L->x2)/2;
             c_y=(L->y1+L->y2)/2;
             c_angle=line_angle;
           }
            else
             {
               if (len_pline>max_length)
                {
                  max_length=len_pline;
                  c_x=(L->x1+L->x2)/2;
                  c_y=(L->y1+L->y2)/2;
                  c_angle=line_angle;
                }
             }
         }
        adp+=(sizeof(NAGLOWEK)+L->n);
      }
       else if (nag->obiekt == Oluk)
        {
          l = (LUK*)adp;
          //wymiarowanie luku polilinii;
         if (new_texts<max_new_texts)
         {
          new_texts++;
          l_x1=l->x+l->r*cos(l->kat1);
          l_y1=l->y+l->r*sin(l->kat1);
          l_x2=l->x+l->r*cos(l->kat2);
          l_y2=l->y+l->r*sin(l->kat2);
          memmove(&Tn[new_texts-1], &Tn0, sizeof(TEXT));
          Tn[new_texts-1].x=(l_x1+l_x2)/2;
          Tn[new_texts-1].y=(l_y1+l_y2)/2;
          //wyznaczenie dlugosci
          if (l->kat1 > l->kat2) del_kat=(l->kat2 + Pi2) - l->kat1;
                  else del_kat = l->kat2 - l->kat1;
          len_pline = Pi2 * l->r * (del_kat/Pi2);


          len_pline*=(SkalaF/Jednostki);  //w cm
          poliline_length+=(floor(len_pline+0.5));
          if (Check_if_Equal(l_x2,l_x1)==TRUE) line_angle=Pi/2;
            else line_angle=atan((l_y2-l_y1)/(l_x2-l_x1));
          Tn[new_texts-1].kat=line_angle;
          Tn[new_texts-1].x-=(dh_text*sin(line_angle));
          Tn[new_texts-1].y+=(dh_text*cos(line_angle));
          Tn[new_texts-1].justowanie=j_srodkowo;
          sprintf(Tn[new_texts-1].text,"%.0f",len_pline);
          Tn[new_texts-1].dl=strlen(Tn[new_texts-1].text);
          Tn[new_texts-1].n=T18+Tn[new_texts-1].dl;
		  Tn[new_texts - 1].width = 0;
		  Tn[new_texts - 1].height = 0;
          Tn[new_texts-1].blok=1;
          Tn[new_texts-1].warstwa=Current_Layer;
          Tn[new_texts-1].ukryty=0;
          if (new_texts==1)
           {
             max_length=len_pline;
             c_x=(l_x1+l_x2)/2;
             c_y=(l_y1+l_y2)/2;
             c_angle=line_angle;
           }
            else
             {
               if (len_pline>max_length)
                {
                  max_length=len_pline;
                  c_x=(l_x1+l_x2)/2;
                  c_y=(l_y1+l_y2)/2;
                  c_angle=line_angle;
                }
             }
         }
          adp+=(sizeof(NAGLOWEK)+l->n);
        }
         else
          {
            L = (LINIA*)adp;
            adp+=(sizeof(NAGLOWEK)+L->n);
          }
     nag = (NAGLOWEK*)adp;
   }
  if (new_texts>0)
   {
     for (i=0; i<new_texts; i++)
      {
       if (dodaj_obiekt(dane, (void*)&Tn[i])==NULL) return;
       rysuj_obiekt((char*)&Tn[i],COPY_PUT,1);
      }
     //summ length
	 memmove(&Tn1, &Tn0, sizeof(TEXT));
     Tn1.x=c_x;
     Tn1.y=c_y;
     Tn1.kat=c_angle;
     Tn1.x+=(dh1_text*sin(c_angle));
     Tn1.y-=(dh1_text*cos(c_angle));
     Tn1.justowanie=j_srodkowo;
     sprintf(Tn1.text,"%s l=%.0f",zb_glowne,poliline_length);
     Tn1.dl=strlen(Tn1.text);
     Tn1.n=T18+Tn1.dl;
	 Tn1.width = 0;
	 Tn1.height = 0;
     Tn1.blok=1;
     Tn1.warstwa=Current_Layer;
     Tn1.ukryty=0;
     if (dodaj_obiekt(dane, (void*)&Tn1)==NULL) return;
     rysuj_obiekt((char*)&Tn1,COPY_PUT,1);
   }
  return;
}



static int rysuj_zbrojenie(void)
{ 
  int n;
  char st1[60];
  char blok_type[31];
  unsigned int Ltype;


   n=getwsp1(&mNets0)-1;
   komunikat(0);
   komunikat0(0);
   if (n==-1) return 0;

   Ltype = LiniaG.typ;

   strcpy(zb_glowne, nets_tab0[n]);
  
   sprintf(st1,"%s %s :",opis_zbrojenia[0], zb_glowne);
   komunikat_str_short(st1, TRUE, FALSE);
   
   //line global parameters
   //polyline
   LiniaG.typ=128;
   LukG.typ=128;
   sprintf(blok_type,"%s", zb_glowne);
   PlineW1(&blok_type);

   LiniaG.typ = Ltype;
   LukG.typ = Ltype;

   komunikat(0);

 return 0;
}

void Rysuj_zbrojenie(void)
/*----------------------*/
{int k;
 k=rysuj_zbrojenie();
}

#undef __O_KONSTR__