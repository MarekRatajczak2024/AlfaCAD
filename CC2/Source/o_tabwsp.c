#define __O_TABWSP__

#include <stdlib.h>
#ifdef LINUX
#include <dirent.h>
#else
#include <direct.h>
#endif
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <forwin.h>
#ifdef LINUX
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <allegext.h>

//#pragma pack(4)  //to avoid warning //WARNING !!!!

#include "bib_e.h"
#include "o_bloklb.h"
#include "bib_blok.h"
#include "o_object.h"
#include "rysuj_e.h"
#include "message.h"
#include "o_loadf.h"

#include "menu.h"

#include "leak_detector_c.h"

#define max_distans 100.0     //maksymalna odleglosc dla analizy punktu wysokosciowego
#define max_points 1024
#define max_points_level 1024
//#define EXT_XYZ ".xyz"
#define n_count_level 3       //liczba najblizszych punktow branych pod uwage
                              //przy interpolacji rzednej terenu i ulicy

extern BOOL Check_if_Equal (double x, double y);
extern char *File_Ext(char *fn, char *fext);
extern char *find_obj2(char *adrp, char *adrk, int obiekt1, int obiekt2, int typ_obiektu1, int typ_obiektu2, int info_p);
extern char *find_block(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu);
extern char /*void*/ *find_obj(char *adrp, char *adrk, int obiekt, int typ_obiektu, int info_p);
extern char *find_block_sub(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu, int l_char);
//extern BOOL Edit_File (char * f_name, int type, BOOL cur);
extern int EditFile(char *, int adjust, int nCmdShow);
extern int ask_question (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);
extern void get_spec_string(int nr, char *st);
extern int text2PDF (char *in_file_name, char *out_file_name);
extern char *File_New_Ext (char *fn, char *fext);
extern void set_decimal_format(char *text, double l, double precision);
extern void decimal(char *text);

extern int mynCmdShow;
/*
typedef struct
 { double x1;
   double y1;
   double x2;
   double y2;
   char *adr;
   char flag;
 }  VECTOR_ADR;
*/

typedef struct
{ double x;
  double y;
  double dlugosc;
  char kierunek;   // 0  /
  char opis[15];
  double rzedna;
  double srednica; //lub wysokosc kanalu Cok
  double szerokosc;
} net_point;

typedef struct
{ double l;
  double rt;
  double ru;
} Point_Length;


typedef struct
{ double x;
  double y;
  double ru;
  double rt;
} Point_Level;

typedef struct
{ double ru;
  double rt;
} Two_Double;

//int find_level_for_node(Two_Double *rt_ru, double x_node, double y_node);

Point_Level point_level[max_points_level+1];
Record_Profil record_profil;

net_point point_xy[max_points+1];    //wezly jednej sieci
net_point point_xy_k[max_points+1];  //kolizje jednej sieci
int n_kolizje;
int n_level;

BOOL find_vector(int nn, double x1, double y1, double x2, double y2)
{ int i;
  int wynik;

  wynik=0;
  for (i=0; i<nn; i++)
   {
     if ((Check_if_Equal(point_xy[i].x,x1)==TRUE) && (Check_if_Equal(point_xy[i].y,y1)==TRUE))
         wynik++;
     if ((Check_if_Equal(point_xy[i].x,x2)==TRUE) && (Check_if_Equal(point_xy[i].y,y2)==TRUE))
         wynik++;
   }
 if (wynik<2) return FALSE;
  else return TRUE;
}

/*
/////////////
void sort_vectors(char *adp01, char *adp02)
//-----------------------------------------
{ char *adp1, *adp2, *adp3;
  BOOL find_next;
  BOOL zmieniono;
  int no_vectors;
  LINIA *L, L1;
  NAGLOWEK *nag;
  int k,j,i,jj;
  double min_v, l_v;
#define max_vectors 20
  VECTOR_ADR vector[21], vector1[21];


  adp1=adp01;
  adp2=adp02;
  find_next=TRUE;
  zmieniono=FALSE;
  no_vectors=0;

  //odczytanie wektorow

  while ((adp1<adp2) && (find_next==TRUE))
   {
    adp3=find_obj(adp1, adp2, Olinia, 128, 1);
    if (adp3 != NULL)
     {
       nag=(NAGLOWEK *)adp3;
       if (nag->obiekt==Olinia)  //linia
         {
           L=(LINIA *)adp3;
           if (no_vectors<max_vectors)
            {
             no_vectors++;
             if (L->obiektt3==0) //widac element byl modyfikowany
              {
               if (no_vectors>1)
                {
                 vector[no_vectors-1].x1=vector[0].x1;
                 vector[no_vectors-1].y1=vector[0].y1;
                 vector[no_vectors-1].x2=vector[0].x2;;
                 vector[no_vectors-1].y2=vector[0].y2;;
                 vector[no_vectors-1].adr=vector[0].adr;
                 vector[no_vectors-1].flag=0;
                }
               vector[0].x1=L->x1;
               vector[0].y1=L->y1;
               vector[0].x2=L->x2;
               vector[0].y2=L->y2;
               vector[0].adr=adp3;
               vector[0].flag=0;
              }
               else
                {
                  vector[no_vectors-1].x1=L->x1;
                  vector[no_vectors-1].y1=L->y1;
                  vector[no_vectors-1].x2=L->x2;
                  vector[no_vectors-1].y2=L->y2;
                  vector[no_vectors-1].adr=adp3;
                  vector[no_vectors-1].flag=0;
                }
            }
         }
       adp1=adp3+sizeof(NAGLOWEK)+nag->n;
     }
      else find_next=FALSE;
   }
   if (no_vectors>2)  //sortowanie
    {
      vector1[0].x1=vector[0].x1;
      vector1[0].y1=vector[0].y1;
      vector1[0].x2=vector[0].x2;
      vector1[0].y2=vector[0].y2;
      vector1[0].adr=vector[0].adr;


      for (k=1; k<no_vectors; k++)
      {
      for (j=k; j<no_vectors; j++)
       {
        if (vector[j].flag==0)
        {
         l_v=(vector[j].x1-vector[k-1].x2)*(vector[j].x1-vector[k-1].x2)+
             (vector[j].y1-vector[k-1].y2)*(vector[j].y1-vector[k-1].y2);
         if (j==k)
          {
            min_v=l_v;
            vector1[k].x1=vector[j].x1;
            vector1[k].y1=vector[j].y1;
            vector1[k].x2=vector[j].x2;
            vector1[k].y2=vector[j].y2;
            vector1[k].adr=vector[j].adr;
            jj=j;
          }
          else
           {
             if (l_v<min_v)
              {
                zmieniono=TRUE;
                min_v=l_v;
                vector1[k].x1=vector[j].x1;
                vector1[k].y1=vector[j].y1;
                vector1[k].x2=vector[j].x2;
                vector1[k].y2=vector[j].y2;
                vector1[k].adr=vector[j].adr;
                jj=j;
              }
           }
         vector[jj].flag=1;
        }
       }   //j
      }   //k

      //jest posortowane
      if (zmieniono==TRUE)
       {
        for (i=1; i<no_vectors; i++)
         {
          L=(LINIA *) vector[i].adr;
          memmove(&L1,L, sizeof(LINIA));
          L1.x1=vector1[i].x1;
          L1.y1=vector1[i].y1;
          L1.x2=vector1[i].x2;
          L1.y2=vector1[i].y2;
          memmove(L,&L1, sizeof(LINIA));
         }
       }

    }
}
/////////////
*/

void Generowanie_tabeli_wspolrzednych(void)
/*---------------------------------------*/
{ NAGLOWEK *nag;
  LINIA *L;
  LUK *l;
  T_Point *P;
  BLOK *b,*b1;
  TEXT *T;
  T_Desc_Ex_Block *ptrs_desc_bl, *ptrs_desc_bl1;
  char *adp, *adk, *adp01, *adp02, *adp0, *adp1, *adp2, *adp3, *adp4, *adp5, *adp6;
  BOOL first_line,find_next,find_vertex;
//  net_point point_xy[max_points+1];
  int n_points, i, k;
  double l_x1, l_y1, l_x2, l_y2, l_x0, l_y0;
  double last_x, last_y;
  double x_vertex, y_vertex;
  static   FILE *stream1;
  char string [MaxTextLen];
  double llx, lly, lll;
  char sk[MAXPATH], sk_xyz[MAXPATH];
  BOOL ret_edit;
  double suma_dlugosci;
  BOOL zapisano_xyz;
  double del_kat;
  double jX,jY,jX0,jY0;
  double sina1, cosa1;
  double angle_l, angle_l_r;
  char *bp1;
  double last_canal_level=0;
  char Xs[32], Ys[32], ls[32], rzedna[32];
  char net_connection[254];
  char *coord_system;
  POINTD local_zero;
  POINTD local_zero_mm;
  char local_zero_x[16], local_zero_y[16], local_angle[16], local_scale[16] ;

  //poszukiwanie polilinii sieci wraz z blokiem opisowym
  adp	= dane ;
  adk	= dane + dane_size ;

  // Generowanie pliku wynikowego *.xyz
   strcpy (sk, "");
   if (Load_File (sk, ZAPIS_XYZ, TRUE) == FALSE)
    {
     return;
    }
   File_Ext (sk, EXT_XYZ) ;

   zapisano_xyz=FALSE;

   stream1=fopen(sk,"wt");

   if (stream1==NULL) goto error1;

   /*
   unsigned char BOM[4];
   BOM[0]=0xEF;
   BOM[1]=0xBB;
   BOM[2]=0xBF;
   BOM[3]='\0';
   fputs((char*)BOM,stream1);
   */

  nag =	(NAGLOWEK*)adp;
  while ((nag->obiekt != Okoniec)	&&	(adp <= adk))
  {
   if (nag->obiekt == OdBLOK)
	 {
     if ((nag->atrybut != Ausuniety) && (nag->atrybut != Abad))
     {
     b=(BLOK *)adp;
     ptrs_desc_bl	= (T_Desc_Ex_Block *)(&b->opis_obiektu	[0]) ;
	  if ((strncmp(ptrs_desc_bl->sz_type,"*#",2)==0) || //to jest jakas siec
         (strncmp(ptrs_desc_bl->sz_type,"*^",2)==0))   //to jest przylacze
      {
        char sz_type[64];
        strncpy(sz_type,&ptrs_desc_bl->sz_type[1],ptrs_desc_bl->len-1);

        if (options1.uklad_geodezyjny==0) {
            coord_system = CARTESIAN;
            local_zero_mm.x = -(get_localx());  //coordinates of drawing origin in mm
            local_zero_mm.y = -(get_localy());
            local_zero.x = -milimetryob(get_localx()); //coordinates of drawing origin in drawing units and scale
            local_zero.y = -milimetryob(get_localy());
        }
        else
        {
            coord_system=GEODETIC;
            local_zero_mm.y = -(get_localx());  //coordinates of drawing origin in mm
            local_zero_mm.x = -(get_localy());
            local_zero.y = -milimetryob(get_localx()); //coordinates of drawing origin in drawing units and scale
            local_zero.x = -milimetryob(get_localy());
        }

          angle_l=get_angle_l();  //in deg
          angle_l_r=(angle_l/360.) * Pi2;  //in radians

          set_decimal_format(local_zero_x, local_zero.x, 0.0001);
          decimal(local_zero_x);
          set_decimal_format(local_zero_y, local_zero.y, 0.0001);
          decimal(local_zero_y);
          set_decimal_format(local_angle, angle_l, 0.0001);  //in deg
          decimal(local_angle);
          set_decimal_format(local_scale, SkalaF, 0.1);  //in deg
          decimal(local_scale);

        snprintf(net_connection, sizeof(net_connection),"%s%s \t%s%s \t%s%s \t%s%s;%s \t%s%s\n",_Network_,sz_type,_COORDINATES_,coord_system,_SCALE_, local_scale, _POINT_ORIGIN_,local_zero_x,local_zero_y,_ANGLE_,local_angle);

        n_points=0;
        //1. Utworzenie tablicy wezlow sieci
        //   Najpierw nalezy odszukac adres polilinii sieci
        //  find_obj(char *adrp, char *adrk, int obiekt, char typ_obiektu, int info_p)
        adp01=adp +sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
        adp02=adp + sizeof(NAGLOWEK) + b->n - 1;

//        sort_vectors(adp01, adp02);

        //adp0=find_block(adp01, adp02, B_PLINE, ""); //poliliniii moze byc wiele
        //if (adp0!=NULL)
        //{
        // b1=(BLOK *)adp0;
        // adp1=adp0 + sizeof(NAGLOWEK)+B3+b1->dlugosc_opisu_obiektu;
        // adp2=adp0 + sizeof(NAGLOWEK) + b1->n - 1;

         adp1=adp01;
         adp2=adp02;
         find_next=TRUE;
         first_line=TRUE;
         //O2BlockAparat;
         while ((adp1<adp2) && (find_next==TRUE))
         {
           adp3=find_obj2(adp1, adp2, Olinia, Oluk, 128, 128, 1);
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
                   strcpy(point_xy[n_points-1].opis,"");
                   point_xy[n_points-1].rzedna=0;
                   point_xy[n_points-1].srednica=0.5;
                   point_xy[n_points-1].x=L->x1;
                   point_xy[n_points-1].y=L->y1;
                   if (n_points>1)
                     {
                       llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
                       lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
                       point_xy[n_points-1].dlugosc=sqrt(llx*llx+lly*lly)*SkalaF/Jednostki;
                     }
                      else point_xy[n_points-1].dlugosc=0;
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
                   strcpy(point_xy[n_points-1].opis,"");
                   point_xy[n_points-1].rzedna=0;
                   point_xy[n_points-1].srednica=0.5;
                   point_xy[n_points-1].x=L->x2;
                   point_xy[n_points-1].y=L->y2;
                   if (n_points>1)
                     {
                       llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
                       lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
                       point_xy[n_points-1].dlugosc=sqrt(llx*llx+lly*lly)*SkalaF/Jednostki;
                     }
                      else point_xy[n_points-1].dlugosc=0;
                  }
                 first_line=FALSE;
                 last_x=L->x2;
                 last_y=L->y2;
               }
                else
                 {
                   //sprawdzenie czy taki punkt juz nie istnieje
                  if (find_vector(n_points,L->x1,L->y1,L->x2,L->y2)==FALSE)
                  {
                   //punkt 1
                   if ((Check_if_Equal(L->x1,last_x)==FALSE) && (Check_if_Equal(L->y1,last_y)==FALSE))
                    {   //nowy punkt
                     if (n_points<max_points)
                      {
                       n_points++;
                       strcpy(point_xy[n_points-1].opis,"");
                       point_xy[n_points-1].rzedna=0;
                       point_xy[n_points-1].srednica=0.5;
                       point_xy[n_points-1].x=L->x1;
                       point_xy[n_points-1].y=L->y1;
                       if (n_points>1)
                        {
                         llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
                         lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
                         point_xy[n_points-1].dlugosc=sqrt(llx*llx+lly*lly)*SkalaF/Jednostki;
                        }
                       else point_xy[n_points-1].dlugosc=0;
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
                     strcpy(point_xy[n_points-1].opis,"");
                     point_xy[n_points-1].rzedna=0;
                     point_xy[n_points-1].srednica=0.5;
                     point_xy[n_points-1].x=L->x2;
                     point_xy[n_points-1].y=L->y2;
                     if (n_points>1)
                     {
                       llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
                       lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
                       point_xy[n_points-1].dlugosc=sqrt(llx*llx+lly*lly)*SkalaF/Jednostki;
                     }
                      else point_xy[n_points-1].dlugosc=0;
                    }
                   first_line=FALSE;
                   last_x=L->x2;
                   last_y=L->y2;
                  }
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
                //dlugosc luku
                if (l->kat1 > l->kat2) del_kat=(l->kat2 + Pi2) - l->kat1;
                  else del_kat = l->kat2 - l->kat1;
                lll= Pi2 * l->r * (del_kat/Pi2);

                //mamy wiec dwa punkty
                if (first_line==TRUE)
                 {
                 //punkt 1
                 if (n_points<max_points)
                  {
                   n_points++;
                   strcpy(point_xy[n_points-1].opis,"");
                   point_xy[n_points-1].rzedna=0;
                   point_xy[n_points-1].srednica=0.5;
                   point_xy[n_points-1].x=l_x1;
                   point_xy[n_points-1].y=l_y1;
                   if (n_points>1)
                     {
                       llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
                       lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
                       point_xy[n_points-1].dlugosc=sqrt(llx*llx+lly*lly)*SkalaF/Jednostki;
                     }
                      else point_xy[n_points-1].dlugosc=0;
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
                   strcpy(point_xy[n_points-1].opis,"");
                   point_xy[n_points-1].rzedna=0;
                   point_xy[n_points-1].srednica=0.5;
                   point_xy[n_points-1].x=l_x2;
                   point_xy[n_points-1].y=l_y2;
                   if (n_points>1)
                     {
                       point_xy[n_points-1].dlugosc=lll*SkalaF/Jednostki;
                     }
                      else point_xy[n_points-1].dlugosc=0;
                  }
                 first_line=FALSE;
                 last_x=l_x2;
                 last_y=l_y2;
                 }
                else
                 {
                  if (find_vector(n_points,l_x1,l_y1,l_x2,l_y2)==FALSE)
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
                       strcpy(point_xy[n_points-1].opis,"");
                       point_xy[n_points-1].rzedna=0;
                       point_xy[n_points-1].srednica=0.5;
                       point_xy[n_points-1].x=l_x1;
                       point_xy[n_points-1].y=l_y1;
                       if (n_points>1)
                        {
                         llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
                         lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
                         point_xy[n_points-1].dlugosc=sqrt(llx*llx+lly*lly)*SkalaF/Jednostki;
                        }
                         else point_xy[n_points-1].dlugosc=0;
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
                     strcpy(point_xy[n_points-1].opis,"");
                     point_xy[n_points-1].rzedna=0;
                     point_xy[n_points-1].srednica=0.5;
                     point_xy[n_points-1].x=l_x2;
                     point_xy[n_points-1].y=l_y2;
                     if (n_points>1)
                     {
                       point_xy[n_points-1].dlugosc=lll*SkalaF/Jednostki;
                     }
                      else point_xy[n_points-1].dlugosc=0;
                    }
                   first_line=FALSE;
                   last_x=l_x2;
                   last_y=l_y2;
                  }
                 }
                }
                adp1=adp3+sizeof(NAGLOWEK)+l->n;
               }
            }
             else find_next=FALSE;
         }
        //}

        if (n_points>0)
        {
         fputs(net_connection, stream1);
         sprintf(string, "%-8s\t%-4s\t%-4s\t%-4s\t%s\n", _Point_, "X", "Y", "Z", "l");
         fputs(string, stream1);
        //znaleziono wezly sieci.
        //2. Poszukiwanie opisow wezlow sieci
        //   Znalezione opisy "przypisywane" sa do wlasiwego wezla
        //   Pozwala to na wycinanie elementow polilinii, edycje opisow itp.
        //   Opisy wezlow wystepuja w sekwencji :
        //   - znacznik bloku *
        //   - punkt
        //   - linia ukosna
        //   - linia pozioma
        //   - tekst opisu

        find_next=TRUE;

        //poszukiwanie wewnatrz bloku

        while ((adp01<adp02) && (find_next==TRUE))
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
              adp5=find_obj(adp1, adp2, Opoint,  ONieOkreslony, 1);
              adp4=find_obj(adp1, adp2, Otekst, n_typ_normal, 1);
              adp6=find_obj(adp1, adp2, Otekst, n_rzedna_kanalu_p, 1);
              if (adp4!=NULL)//znaleziono tekst opisu
               {
                 T=(TEXT *)adp4;
                 if (adp5!=NULL)
                  {
                    P=(T_Point *)adp5;
                    x_vertex=P->x;
                    y_vertex=P->y;
                  }
                 //przyporzadkowanie tekstu do wiezcholka polilinii
                 find_vertex=FALSE;
                 i=0;
                 while ((i<n_points) && (find_vertex==FALSE))
                  {
                     if ((Check_if_Equal(x_vertex,point_xy[i].x)==TRUE) && (Check_if_Equal(y_vertex,point_xy[i].y)==TRUE))
                      {
                        strcpy(point_xy[i].opis,T->text);
                        find_vertex=TRUE;
                        if (adp6!=NULL)//znaleziono tekst rzędnej kanału)
                        {
                           T=(TEXT *)adp6;
                           double rzedna_kanalu_p=strtod(T->text, &bp1);
                           if (bp1!=T->text && *bp1 == '\0' && errno != ERANGE)
                           {
                            point_xy[i].rzedna=rzedna_kanalu_p;
                            last_canal_level=rzedna_kanalu_p;
                           }
                        }
                      }
                    i++;
                  }
               }
              adp01 = adp3 + sizeof(NAGLOWEK) + b1->n;
            }
           else find_next=FALSE;
         }

         //poszukiwanie calego zbioru
         find_next=TRUE;

         adp01=dane;
         adp02=dane+dane_size;

         while ((adp01<adp02) && (find_next==TRUE))
         {
           adp3=find_block_sub(adp01, adp02, B_NIEOKRESLONY, "*~",2);
           if (adp3!=NULL)
            {
              b1=(BLOK *)adp3;
              ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu [0]) ;
              x_vertex = ptrs_desc_bl1->x ;
              y_vertex = ptrs_desc_bl1->y ;
              adp1=adp3 + sizeof(NAGLOWEK)+B3+b1->dlugosc_opisu_obiektu;
              adp2=adp3 + sizeof(NAGLOWEK) + b1->n - 1;
              adp5=find_obj(adp1, adp2, Opoint,  ONieOkreslony, 1);
              adp4=find_obj(adp1, adp2, Otekst, n_typ_normal, 1);
              adp6=find_obj(adp1, adp2, Otekst, n_rzedna_kanalu_p, 1);
              if (adp4!=NULL)//znaleziono tekst opisu
               {
                 T=(TEXT *)adp4;
                 if (adp5!=NULL)
                  {
                    P=(T_Point *)adp5;
                    x_vertex=P->x;
                    y_vertex=P->y;
                  }
                 //przyporzadkowanie tekstu do wiezcholka polilinii
                 find_vertex=FALSE;
                 i=0;
                 while ((i<n_points) && (find_vertex==FALSE))
                  {
                     if ((Check_if_Equal(x_vertex,point_xy[i].x)==TRUE) && (Check_if_Equal(y_vertex,point_xy[i].y)==TRUE))
                      {
                        strcpy(point_xy[i].opis,T->text);
                        find_vertex=TRUE;
                        if (adp6!=NULL)//znaleziono tekst rzędnej kanału)
                        {
                         T=(TEXT *)adp6;
                         double rzedna_kanalu_p=strtod(T->text, &bp1);
                         if (bp1!=T->text && *bp1 == '\0' && errno != ERANGE)
                         {
                          point_xy[i].rzedna=rzedna_kanalu_p;
                          last_canal_level=rzedna_kanalu_p;
                         }
                        }
                      }
                   i++;
                  }
               }
              adp01 = adp3 + sizeof(NAGLOWEK) + b1->n;
            }
             else find_next=FALSE;
         }
         //tablica wezlow zapisana

         suma_dlugosci=0;
         for (i=0; i<n_points; i++)
          {
            //korekta wspolrzednych z uwagi na uklad lokalny i uklad geodezyjny
            angle_l=get_angle_l();

            jX = milimetryobx (point_xy[i].x) ;
            jY = milimetryoby (point_xy[i].y) ;
            if (angle_l!=0)
            {
             jX0=jX;
             jY0=jY;
             sina1=get_sina();
             cosa1=get_cosa();
             jX = (jX0 * cosa1) + (jY0 * sina1);
             jY = (-jX0 * sina1) + (jY0 * cosa1);
            }
           if (options1.uklad_geodezyjny==1)
            {
             point_xy[i].x=jY;
             point_xy[i].y=jX;
            }
             else
              {
               point_xy[i].x=jX;
               point_xy[i].y=jY;
              }
           //level correction if necessary
            sprintf(Xs, "%.2f",point_xy[i].x);
            decimal(Xs);
            sprintf(Ys, "%.2f",point_xy[i].y);
            decimal(Ys);

            if (point_xy[i].rzedna>0.)
            {
                sprintf(rzedna, "%.2f", point_xy[i].rzedna);
                decimal(rzedna);
            }
            else strcpy(rzedna,"");

           sprintf(ls, "%.2f",   point_xy[i].dlugosc);
           decimal(ls);
           ///////////////////////
           sprintf(string, "%-8s\t%4s\t%4s\t%4s\t%4s\n", point_xy[i].opis, Xs, Ys, rzedna, ls);
           fputs(string, stream1);
           zapisano_xyz=TRUE;
           suma_dlugosci+=point_xy[i].dlugosc;
          }

         sprintf(ls, "%.2f",   suma_dlugosci);
         decimal(ls);
            //sprintf(string, "\t\t\t\tTotal L=%.2f\n", suma_dlugosci);
         sprintf(string, "%-8s\t%4s\t%4s\t%4s\tL=%4s\n", "", "", "", "", ls);  //Total L

         fputs(string, stream1);

        }
        //////////////
        adp+=sizeof(NAGLOWEK) + b->n;
      }
       else
        {
          adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
        }
     }
      else adp+= nag->n + sizeof(NAGLOWEK);
    }
	 else
     {
		adp += nag->n	+ sizeof(NAGLOWEK) ;
 	  }
	 nag	= (NAGLOWEK*)adp;
  }

 fclose(stream1);

 error1:
 //
 //zapis do pliku
 if (zapisano_xyz==TRUE)
 {

  strcpy(sk_xyz, sk);
  File_New_Ext (sk, ".pdf") ;

  text2PDF(sk_xyz, sk);
  ret_edit = EditFile (sk_xyz,0,mynCmdShow);
 }
   else if (ask_question(1, "","OK","",(char*)_no_descripted_networks_, 12, "", 12, 1, 62)==0)
         {
          ;
         }
}


int find_level_for_node(Two_Double *rt_ru, double x_node, double y_node)
/*-------------------------------------------------------------------------*/
{ Point_Length P0={0,0,0}, P1={0,0,0}, P2={0,0,0};
  int i;
  int n_p_level;
  

  //mamy n_level punktow
  //nalezy znalezc 2 najblizsze punkty P1 i P2 lezace w odleglosciach l1 i l2
  //od punktu <x_node, y_node>
  //jezeli l1==0 to przyjmujemy rzedna P1
  //jezeli l1==0 to przyjmujemy rzedna P2
  //jezeli l1==0 && l2==0 to przyjmujemy (P1+P2)/2
  //w innym przypadku wartosc jest interpolowana
  rt_ru->ru=0.0;
  rt_ru->rt=0.0;

  if (n_level==0) return 0;

  n_p_level=0;

  for (i=0; i<n_level; i++)
   {
     P0.l=sqrt((x_node-point_level[i].x)*(x_node-point_level[i].x)+
         (y_node-point_level[i].y)*(y_node-point_level[i].y));
     P0.ru=point_level[i].ru;
     P0.rt=point_level[i].rt;
     //wstepna selekcja punktu odleglego o wiecej niz max_distans [m]
     if ((P0.l*SkalaF/Jednostki)<=max_distans)
     {
     n_p_level++;
     if (n_p_level==1)
      {
        P1.l=P0.l;
        P1.ru=P0.ru;
        P1.rt=P0.rt;
        P2.l=P0.l;
        P2.ru=P0.ru;
        P2.rt=P0.rt;
      }
     else
       {
        if (P0.l<P1.l)
         {
          P2.l=P1.l;
          P2.ru=P1.ru;
          P2.rt=P1.rt;
          P1.l=P0.l;
          P1.ru=P0.ru;
          P1.rt=P0.rt;
         }
          else if (P0.l<P2.l)
           {
            P2.l=P0.l;
            P2.ru=P0.ru;
            P2.rt=P0.rt;
           }
       }
     }
   }
  //otrzymalismy dwa punkty P1 i P2
  if (n_p_level==0) return 0;
  if (n_p_level==1)
   {
     rt_ru->ru=P1.ru;
     rt_ru->rt=P1.rt;
	 
     return 1;
   }
   if ((Check_if_Equal(P1.l,0.0)==TRUE) && (Check_if_Equal(P2.l,0.0)==TRUE))
   {
    rt_ru->ru=(P1.ru+P2.ru)/2.0;
    rt_ru->rt=(P1.rt+P1.rt)/2.0;
	
    return 1;
   }
    if (Check_if_Equal(P1.l,0.0)==TRUE)
     {
      rt_ru->ru=P1.ru;
      rt_ru->rt=P1.rt;
	  
      return 1;
     }
      if (Check_if_Equal(P2.l,0.0)==TRUE)
       {
        rt_ru->ru=P2.ru;
        rt_ru->rt=P2.rt;
		
        return 1;
       }

  rt_ru->ru=P1.ru+(P1.l/(P1.l+P2.l))*(P2.ru-P1.ru);
  rt_ru->rt=P1.rt+(P1.l/(P1.l+P2.l))*(P2.rt-P1.rt);
 
  return 1;
}


void find_level_points(void)
/*------------------------*/
{ int k;
  LINIA *L1;
  char typ_kolizji[30];
  double rzedna, srednica, szerokosc;
  int i;
  BOOL find_last;
  BLOK *b;
  TEXT *T;
  T_Point *P;
  char *adp, *adk, *adp1, *adk1, *adp2, *adp3, *adp4, *adp5, *adp6;
  double x,y,llx,lly;
  double rzedna_terenu, rzedna_ulicy;
 char *bp;
  

  // mamy linie L
  // poszukujemy wszystkich wszystkich blokow ktorych nazwa (typ)
  // rozpoczynajacych sie od znaku "@"
  // w bloku tym znajduje sie linia gruba L1
  // jezeli linie te przecinaja sie, nalezy wyznaczyc punkt przeciecia linii
  rzedna_terenu=0;
  rzedna_ulicy=0;
  n_level=0;
  adp=dane;
  adk=dane+dane_size;
  find_last=FALSE;
  while (find_last==FALSE)
  {
   adp1=find_block_sub(adp, adk, B_NIEOKRESLONY, "* level", 7);   //it was "*0xFB", 2  //"*\373", 2
   if (adp1!=NULL)
    {
     b=(BLOK *)adp1;
     adk1=adp1+sizeof(NAGLOWEK)+b->n -1;
     adp2=find_obj(adp1, adk1, Opoint,  ONieOkreslony, 1);
     adp3=find_obj(adp1, adk1, Otekst, n_rzedna_ulicy, 1);
     adp4=find_obj(adp1, adk1, Otekst, n_rzedna_terenu, 1);
     rzedna_ulicy=0;
     rzedna_terenu=0;
     if (adp2!=NULL)
     {
      P=(T_Point *)adp2;
      if (adp3!=NULL)
       {
         T=(TEXT *)adp3;
         rzedna_ulicy=strtod(T->text, &bp);  //atof(T->text);
       }
      if (adp4!=NULL)
       {
         T=(TEXT *)adp4;
         rzedna_terenu=strtod(T->text, &bp);  //atof(T->text);
       }
      if ((Check_if_Equal(rzedna_terenu,0.0)==FALSE) || (Check_if_Equal(rzedna_terenu,0.0)==FALSE))
       {
         if (n_level<max_points_level)
          {
           n_level++;
           point_level[n_level-1].x=P->x;
           point_level[n_level-1].y=P->y;
           point_level[n_level-1].ru=rzedna_ulicy;
           point_level[n_level-1].rt=rzedna_terenu;
          }
       }
     }
     adp=adp1+sizeof(NAGLOWEK)+b->n;
    }
     else find_last=TRUE;
  }

 
}


void find_colisions_line(LINIA *L, double dlugosc0)
/*-----------------------------------------------*/
{ int k;
  LINIA *L1;
  char typ_kolizji[30];
  double rzedna, srednica, szerokosc;
  int i;
  BOOL find_last;
  BLOK *b;
  TEXT *T;
  char *adp, *adk, *adp1, *adk1, *adp2, *adp3, *adp4, *adp5, *adp6;
  double x,y,llx,lly;
  double srednica_domyslna=0.0;
  double poziom_pp;
  char *bp;

  // mamy linie L
  // poszukujemy wszystkich wszystkich blokow ktorych nazwa (typ)
  // rozpoczynajacych sie od znaku "@"
  // w bloku tym znajduje sie linia gruba L1
  // jezeli linie te przecinaja sie, nalezy wyznaczyc punkt przeciecia linii
  poziom_pp=get_poziom_pp();
  adp=dane;
  adk=dane+dane_size;
  find_last=FALSE;
  while (find_last==FALSE)
  {
   adp1=find_block_sub(adp, adk, B_NIEOKRESLONY, "@", 1);
   if (adp1!=NULL)
   {
    b=(BLOK *)adp1;
    if ((b->atrybut!=Ausuniety) && (b->atrybut!=Abad))
    {
     adk1=adp1+sizeof(NAGLOWEK)+b->n -1;
     adp2=find_obj(adp1, adk1, Olinia, 64, 1);
     adp3=find_obj(adp1, adk1, Otekst, n_typ_normal, 1);
     adp4=find_obj(adp1, adk1, Otekst, n_rzedna_kanalu_i, 1);
     adp5=find_obj(adp1, adk1, Otekst, n_srednica_kanalu_i, 1);
     //adp6=find_obj(adp1, adk1, Otekst, n_srednica_kanalu_p, 1);
     if (adp2!=NULL)
     {
     L1=(LINIA *)adp2;
     k=przeciecieLL_(&x, &y, L1, L);
     if (k)
      {
       n_kolizje++;
       strcpy(point_xy_k[n_kolizje-1].opis,typ_kolizji);
       point_xy_k[n_kolizje-1].x=x;
       point_xy_k[n_kolizje-1].y=y;
       llx=x-L->x1;
       lly=y-L->y1;
       point_xy_k[n_kolizje-1].dlugosc=dlugosc0+(sqrt(llx*llx+lly*lly)*SkalaF/Jednostki);
       //ustalenie typu kolizji
       if (adp3!=NULL)
        {
          T=(TEXT *)adp3;
          strncpy(point_xy_k[n_kolizje-1].opis,T->text,15);
        }
       //ustalenie rzednej
       rzedna=0.0;
       if (adp4!=NULL)
        {
          T=(TEXT *)adp4;
          rzedna=strtod(T->text, &bp); //atof(T->text);
        }
       point_xy_k[n_kolizje-1].rzedna=rzedna;
       //ustalenie srednicy lub wysokosci kanalu Cok
       srednica=srednica_domyslna;
       if (adp5!=NULL)
        {
          T=(TEXT *)adp5;
          srednica=strtod(T->text, &bp); //atof(T->text);
        }
       point_xy_k[n_kolizje-1].srednica=srednica;
       //ustalenie szerokosci
       szerokosc=srednica_domyslna;
       /*
       if (adp6!=NULL)
        {
          T=(TEXT *)adp6;
          szerokosc=strtod(T->text, &bp); //atof(T->text);
        }
        */
       point_xy_k[n_kolizje-1].szerokosc=szerokosc;
      }
     }
     adp=adp1+sizeof(NAGLOWEK)+b->n;
    }
     else adp=adp1+sizeof(NAGLOWEK)+b->n;
   }
     else find_last=TRUE;
  }
}


void find_colisions_arc(LUK *l, double dlugosc0)
/*--------------------------------------------*/
{ int k;
  LINIA *L1;
  char typ_kolizji[30];
  double rzedna, srednica, szerokosc;
  int i;
  BOOL find_last;
  BLOK *b;
  TEXT *T;
  char *adp, *adk, *adp1, *adk1, *adp2, *adp3, *adp4, *adp5, *adp6;
  double x,y,llx,lly;
  double srednica_domyslna=0.0;
  double poziom_pp;
  char *bp;
  // mamy luk l
  // poszukujemy wszystkich wszystkich blokow ktorych nazwa (typ)
  // rozpoczynajacych sie od znaku "@"
  // w bloku tym znajduje sie linia gruba L1
  // jezeli luk i linia przecinaja sie, nalezy wyznaczyc punkt przeciecia

  poziom_pp=get_poziom_pp();
  adp=dane;
  adk=dane+dane_size;
  find_last=FALSE;
  while (find_last==FALSE)
  {
   adp1=find_block_sub(adp, adk, B_NIEOKRESLONY, "@", 1);
   if (adp1!=NULL)
    {
     b=(BLOK *)adp1;
     adk1=adp1+sizeof(NAGLOWEK)+b->n -1;
     adp2=find_obj(adp1, adk1, Olinia, 64, 1);
     adp3=find_obj(adp1, adk1, Otekst, n_typ_normal, 1);
     adp4=find_obj(adp1, adk1, Otekst, n_rzedna_kanalu_i, 1);
     adp5=find_obj(adp1, adk1, Otekst, n_srednica_kanalu_i, 1);
     //adp6=find_obj(adp1, adk1, Otekst, n_srednica_kanalu_p, 1);
     if (adp2!=NULL)
     {
     L1=(LINIA *)adp2;
     k=przeciecieLl_(&x, &y, L1, l);
     if (k)
      {
       n_kolizje++;
       strcpy(point_xy_k[n_kolizje-1].opis,typ_kolizji);
       point_xy_k[n_kolizje-1].x=x;
       point_xy_k[n_kolizje-1].y=y;
       llx=x-l->x;  //!!!!!!!!!!!!!  CHWILOWO
       lly=y-l->y;  //!!!!!!!!!!!!!
       point_xy_k[n_kolizje-1].dlugosc=dlugosc0+(sqrt(llx*llx+lly*lly)*SkalaF/Jednostki);
       //ustalenie typu kolizji
       if (adp3!=NULL)
        {
          T=(TEXT *)adp3;
          strncpy(point_xy_k[n_kolizje-1].opis,T->text,15);
        }
       //ustalenie rzednej
       rzedna=0.0;
       if (adp4!=NULL)
        {
          T=(TEXT *)adp4;
          rzedna=strtod(T->text, &bp); //atof(T->text);
        }
       point_xy_k[n_kolizje-1].rzedna=rzedna;
       //ustalenie srednicy lub wysokosci kanalu Cok
       srednica=srednica_domyslna;
       if (adp5!=NULL)
        {
          T=(TEXT *)adp5;
          srednica=strtod(T->text, &bp); //atof(T->text);
        }
       point_xy_k[n_kolizje-1].srednica=srednica;
       //ustalenie szerokosci
       szerokosc=srednica_domyslna;
       /*
       if (adp6!=NULL)
        {
          T=(TEXT *)adp6;
          szerokosc=strtod(T->text, &bp); //atof(T->text);
        }
       */
       point_xy_k[n_kolizje-1].szerokosc=szerokosc;
      }
     }
     adp=adp1+sizeof(NAGLOWEK)+b->n;
    }
     else find_last=TRUE;
  }
}

void Zapisz_Profil(void)
/*--------------------*/
{
 NAGLOWEK *nag;
 LINIA *L;
 LUK *l;
 OKRAG *K;
 T_Point *P;
 BLOK *b,*b1;
 TEXT *T, *rzedna_kp, *srednica_kp;
 T_Desc_Ex_Block *ptrs_desc_bl, *ptrs_desc_bl1;
 char *adp, *adk, *adp01, *adp02, *adp0, *adp1, *adp2, *adp3, *adp4, *adp5;
 char *adp6, *adp7;
 BOOL first_line,find_next,find_vertex;
 // net_point point_xy[max_points+1];
 int n_points, i;
 double l_x1, l_y1, l_x2, l_y2, l_x0, l_y0;
 double last_x, last_y;
 double x_vertex, y_vertex;
 char string [MaxTextLen];
 double llx, lly, lll;
 char sk[MAXPATH];
 BOOL ret_edit;
 double suma_dlugosci;
 BOOL zapisano_xyz;
 double del_kat;
 double jX,jY,jX0,jY0;
 double sina1, cosa1;
 double angle_l;
 MyDane dane_profs0={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 char nazwa_rysunku[MaxTextLen];
 double odleglosc_00, dlugosc;
 TEXT *T_odl;
 char *adp_odl;
 //Record_Profil record_profil;
 int log;
 char *str;
 char sz_file [MAXPATH] ;
 double rt, ru;
 double srednica_domyslna=0.0;
 Two_Double rt_ru;
 char *bp;

 strcpy(sz_file,zbior_danych);
 str = strrchr(sz_file, Slash);
 if (str == NULL)
 {
  str = sz_file;
 }
 else
 {
  str++;
 }

 //poszukiwanie polilinii sieci wraz z blokiem opisowym
 adp	= dane ;
 adk	= dane + dane_size ;

 // Generowanie pliku wynikowego *.xyz
 strcpy (sk, "");
 if (Load_File (sk, ZAPIS_PRO, TRUE) == FALSE)
 {
  return;
 }
 File_Ext (sk, EXT_PRO) ;

#ifndef LINUX
 log =open(sk, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
 log =open(sk, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif


 if (log == -1) //plik nie istnieje badz blad otwarcia
 {
  komunikat(156);
  //   MVCUR = CUR ;
  return;
 }

 zapisano_xyz=FALSE;

 //poszukiwanie punktow wysokosciowych
 find_level_points();


 nag =	(NAGLOWEK*)adp;
 while ((nag->obiekt != Okoniec)	&&	(adp <= adk))
 {
  if (nag->obiekt == OdBLOK)
  {
   if ((nag->atrybut != Ausuniety) && (nag->atrybut != Abad))
   {
    b=(BLOK *)adp;
    ptrs_desc_bl	= (T_Desc_Ex_Block *)(&b->opis_obiektu	[0]) ;
    if ((strncmp(ptrs_desc_bl->sz_type,"*#",2)==0) || //to jest jakas siec
          (strncmp(ptrs_desc_bl->sz_type,"*^",2)==0))   //to jest przylacze
    {
     //////////////
     n_points=0;
     n_kolizje=0;
     //1. Utworzenie tablicy wezlow sieci
     //   Najpierw nalezy odszukac adres polilinii sieci
     //  find_obj(char *adrp, char *adrk, int obiekt, char typ_obiektu, int info_p)
     adp01=adp +sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
     adp02=adp + sizeof(NAGLOWEK) + b->n - 1;
     ///////////////////
     odleglosc_00=0.0;
     dlugosc=0.0;
     adp_odl=find_obj(adp01, adp02, Otekst, n_typ_atrybut, 1);
     if (adp_odl!=NULL)  //znaleziono tekst atrybut
     {
      T_odl=(TEXT *)adp_odl;
      if (strlen(T_odl->text)>0)
      {
       odleglosc_00=strtod(T_odl->text, &bp);  //atof(T_odl->text);

       //   odleglosc_0=atof(T_odl->text)*(1000.0/Jednostki);
       //   l_h0 = (fmod (odleglosc_00,1000)) / 100 ;
       //   l_k0 = odleglosc_00 / 1000 ;
      }
     }

     ///////////////////
     adp0=find_block(adp01, adp02, B_PLINE, "");
     if (adp0!=NULL)
     {
      b1=(BLOK *)adp0;
      adp1=adp0 + sizeof(NAGLOWEK)+B3+b1->dlugosc_opisu_obiektu;
      adp2=adp0 + sizeof(NAGLOWEK) + b1->n - 1;
      find_next=TRUE;
      first_line=TRUE;
      //O2BlockAparat;
      while ((adp1<adp2) && (find_next==TRUE))
      {
       adp3=find_obj2(adp1, adp2, Olinia, Oluk, 128, 128, 1);
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
           strcpy(point_xy[n_points-1].opis,"");
           point_xy[n_points-1].rzedna=0;
           point_xy[n_points-1].srednica=srednica_domyslna;
           point_xy[n_points-1].x=L->x1;
           point_xy[n_points-1].y=L->y1;
           if (n_points>1)
           {
            llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
            lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
            dlugosc+=(sqrt(llx*llx+lly*lly)*SkalaF/Jednostki);
            point_xy[n_points-1].dlugosc=dlugosc;
           }
           else point_xy[n_points-1].dlugosc=0.0;

           //odszukanie kolizji
           //find_colisions_line(L,point_xy[n_points-1].dlugosc);

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
           strcpy(point_xy[n_points-1].opis,"");
           point_xy[n_points-1].rzedna=0.0;
           point_xy[n_points-1].srednica=srednica_domyslna;
           point_xy[n_points-1].x=L->x2;
           point_xy[n_points-1].y=L->y2;
           if (n_points>1)
           {
            llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
            lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
            dlugosc+=(sqrt(llx*llx+lly*lly)*SkalaF/Jednostki);
            point_xy[n_points-1].dlugosc=dlugosc;
           }
           else point_xy[n_points-1].dlugosc=0.0;

           //odszukanie kolizji
           find_colisions_line(L,point_xy[n_points-2].dlugosc);
          }
          first_line=FALSE;
          last_x=L->x2;
          last_y=L->y2;
         }
         else
         {
          if (find_vector(n_points,L->x1,L->y1,L->x2,L->y2)==FALSE)
          {
           //punkt 1
           if ((Check_if_Equal(L->x1,last_x)==FALSE) && (Check_if_Equal(L->y1,last_y)==FALSE))
           {   //nowy punkt
            if (n_points<max_points)
            {
             n_points++;
             strcpy(point_xy[n_points-1].opis,"");
             point_xy[n_points-1].rzedna=0.0;
             point_xy[n_points-1].srednica=srednica_domyslna;
             point_xy[n_points-1].x=L->x1;
             point_xy[n_points-1].y=L->y1;
             if (n_points>1)
             {
              llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
              lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
              dlugosc+=(sqrt(llx*llx+lly*lly)*SkalaF/Jednostki);
              point_xy[n_points-1].dlugosc=dlugosc;
             }
             else point_xy[n_points-1].dlugosc=0.0;

             //odszukanie kolizji
             //find_colisions_line(L,point_xy[n_points-1].dlugosc);

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
            strcpy(point_xy[n_points-1].opis,"");
            point_xy[n_points-1].rzedna=0.0;
            point_xy[n_points-1].srednica=srednica_domyslna;
            point_xy[n_points-1].x=L->x2;
            point_xy[n_points-1].y=L->y2;
            if (n_points>1)
            {
             llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
             lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
             dlugosc+=(sqrt(llx*llx+lly*lly)*SkalaF/Jednostki);
             point_xy[n_points-1].dlugosc=dlugosc;
            }
            else point_xy[n_points-1].dlugosc=0.0;

            //odszukanie kolizji
            find_colisions_line(L,point_xy[n_points-2].dlugosc);

           }
           first_line=FALSE;
           last_x=L->x2;
           last_y=L->y2;
          }
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
          //dlugosc luku
          if (l->kat1 > l->kat2) del_kat=(l->kat2 + Pi2) - l->kat1;
          else del_kat = l->kat2 - l->kat1;
          lll= Pi2 * l->r * (del_kat/Pi2);

          //mamy wiec dwa punkty
          if (first_line==TRUE)
          {
           //punkt 1
           if (n_points<max_points)
           {
            n_points++;
            strcpy(point_xy[n_points-1].opis,"");
            point_xy[n_points-1].rzedna=0;
            point_xy[n_points-1].srednica=srednica_domyslna;
            point_xy[n_points-1].x=l_x1;
            point_xy[n_points-1].y=l_y1;
            if (n_points>1)
            {
             llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
             lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
             dlugosc+=(sqrt(llx*llx+lly*lly)*SkalaF/Jednostki);
             point_xy[n_points-1].dlugosc=dlugosc;
            }
            else point_xy[n_points-1].dlugosc=0;

            //odszukanie kolizji
            //find_colisions_arc(l,point_xy[n_points-1].dlugosc);

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
            strcpy(point_xy[n_points-1].opis,"");
            point_xy[n_points-1].rzedna=0.0;
            point_xy[n_points-1].srednica=srednica_domyslna;
            point_xy[n_points-1].x=l_x2;
            point_xy[n_points-1].y=l_y2;
            if (n_points>1)
            {
             dlugosc+=(lll*SkalaF/Jednostki);
             point_xy[n_points-1].dlugosc=dlugosc;
            }
            else point_xy[n_points-1].dlugosc=0.0;

            //odszukanie kolizji
            find_colisions_arc(l,point_xy[n_points-2].dlugosc);

           }
           first_line=FALSE;
           last_x=l_x2;
           last_y=l_y2;
          }
          else
          {
           if (find_vector(n_points,l_x1,l_y1,l_x2,l_y2)==FALSE)
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
              strcpy(point_xy[n_points-1].opis,"");
              point_xy[n_points-1].rzedna=0.0;
              point_xy[n_points-1].srednica=srednica_domyslna;
              point_xy[n_points-1].x=l_x1;
              point_xy[n_points-1].y=l_y1;
              if (n_points>1)
              {
               llx=point_xy[n_points-1].x-point_xy[n_points-2].x;
               lly=point_xy[n_points-1].y-point_xy[n_points-2].y;
               dlugosc+=(sqrt(llx*llx+lly*lly)*SkalaF/Jednostki);
               point_xy[n_points-1].dlugosc=dlugosc;
              }
              else point_xy[n_points-1].dlugosc=0.0;

              //odszukanie kolizji
              //find_colisions_arc(l,point_xy[n_points-1].dlugosc);

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
             strcpy(point_xy[n_points-1].opis,"");
             point_xy[n_points-1].rzedna=0.0;
             point_xy[n_points-1].srednica=srednica_domyslna;
             point_xy[n_points-1].x=l_x2;
             point_xy[n_points-1].y=l_y2;
             if (n_points>1)
             {
              dlugosc+=(lll*SkalaF/Jednostki);
              point_xy[n_points-1].dlugosc=dlugosc;
             }
             else point_xy[n_points-1].dlugosc=0.0;

             //odszukanie kolizji
             find_colisions_arc(l,point_xy[n_points-2].dlugosc);

            }
            first_line=FALSE;
            last_x=l_x2;
            last_y=l_y2;
           }
          }
         }
         adp1=adp3+sizeof(NAGLOWEK)+l->n;
        }
       }
       else find_next=FALSE;
      }
     }
     if (n_points>0)
     {
      find_next=TRUE;

      while ((adp01<adp02) && (find_next==TRUE))
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
        adp5=find_obj(adp1, adp2, Okolo,  ONieOkreslony, 1);
        adp4=find_obj(adp1, adp2, Otekst, n_typ_normal, 1);
        adp6=find_obj(adp1, adp2, Otekst, n_rzedna_kanalu_p, 1);
        adp7=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_p, 1);
        if (adp4!=NULL)//znaleziono tekst opisu
        {
         T=(TEXT *)adp4;
         if (adp5!=NULL)
         {
          P=(T_Point *)adp5;
          x_vertex=P->x;
          y_vertex=P->y;
         }
         //przyporzadkowanie tekstu do wiezcholka polilinii
         find_vertex=FALSE;
         i=0;
         while ((i<n_points) && (find_vertex==FALSE))
         {
          if ((Check_if_Equal(x_vertex,point_xy[i].x)==TRUE) && (Check_if_Equal(y_vertex,point_xy[i].y)==TRUE))
          {
           strcpy(point_xy[i].opis,T->text);
           if (adp6!=NULL)
           {
            rzedna_kp=(TEXT *)adp6;
            point_xy[i].rzedna=strtod(rzedna_kp->text, &bp);  //atof(rzedna_kp->text);
           }
           if (adp7!=NULL)
           {
            srednica_kp=(TEXT *)adp7;
            point_xy[i].srednica=strtod(srednica_kp->text, &bp); //atof(srednica_kp->text);
           }
           find_vertex=TRUE;
          }
          i++;
         }
        }
        adp01 = adp3 + sizeof(NAGLOWEK) + b1->n;
       }
       else find_next=FALSE;
      }


      //poszukiwanie calego zbioru
      find_next=TRUE;

      adp01=dane;
      adp02=dane+dane_size;

      while ((adp01<adp02) && (find_next==TRUE))
      {
       adp3=find_block_sub(adp01, adp02, B_NIEOKRESLONY, "*~",2);
       if (adp3!=NULL)
       {
        b1=(BLOK *)adp3;
        ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu [0]) ;
        x_vertex = ptrs_desc_bl1->x ;
        y_vertex = ptrs_desc_bl1->y ;
        adp1=adp3 + sizeof(NAGLOWEK)+B3+b1->dlugosc_opisu_obiektu;
        adp2=adp3 + sizeof(NAGLOWEK) + b1->n - 1;
        adp5=find_obj(adp1, adp2, Opoint,  ONieOkreslony, 1);
        adp4=find_obj(adp1, adp2, Otekst, n_typ_normal, 1);
        adp6=find_obj(adp1, adp2, Otekst, n_rzedna_kanalu_p, 1);
        adp7=find_obj(adp1, adp2, Otekst, n_srednica_kanalu_p, 1);
        if (adp4!=NULL)//znaleziono tekst opisu
        {
         T=(TEXT *)adp4;
         if (adp5!=NULL)
         {
          P=(T_Point *)adp5;
          x_vertex=P->x;
          y_vertex=P->y;
         }
         //przyporzadkowanie tekstu do wiezcholka polilinii
         find_vertex=FALSE;
         i=0;
         while ((i<n_points) && (find_vertex==FALSE))
         {
          if ((Check_if_Equal(x_vertex,point_xy[i].x)==TRUE) && (Check_if_Equal(y_vertex,point_xy[i].y)==TRUE))
          {
           strcpy(point_xy[i].opis,T->text);
           if (adp6!=NULL)
           {
            rzedna_kp=(TEXT *)adp6;
            point_xy[i].rzedna=strtod(rzedna_kp->text, &bp); //atof(rzedna_kp->text);
           }
           if (adp7!=NULL)
           {
            srednica_kp=(TEXT *)adp7;
            point_xy[i].srednica=strtod(srednica_kp->text, &bp); //atof(srednica_kp->text);
           }
           find_vertex=TRUE;
          }
          i++;
         }
        }
        adp01 = adp3 + sizeof(NAGLOWEK) + b1->n;
       }
       else find_next=FALSE;
      }
      //tablica wezlow zapisana

      //korekta srednic:
      srednica_domyslna=0.5;
      for (i=0; i<n_points; i++)
      {
       if (Check_if_Equal(point_xy[i].srednica,0.0)==TRUE)
       {
        point_xy[i].srednica=srednica_domyslna;
       }
       else srednica_domyslna=point_xy[i].srednica;
      }
      if ((Check_if_Equal(point_xy[0].srednica,0.0)==TRUE) &&
          (Check_if_Equal(point_xy[1].srednica,0.0)==FALSE))
      {
       point_xy[0].srednica=point_xy[1].srednica;
      }

      suma_dlugosci=0;
      for (i=0; i<n_points; i++)
      {
       record_profil.flag=0;  //flaga kontrolna, domyslnie 0
       record_profil.l=n_points+n_kolizje;  //liczba rekordow sieci wraz z kolizjami;
       record_profil.i=i;  //numer rekordu sieci, numerowany od 0
       strncpy(record_profil.file_name,str,30);
       get_spec_string(3,nazwa_rysunku);
       strncpy(record_profil.rys_name,nazwa_rysunku,60);   //nazwa rysunku z ktorego pochodzi
       record_profil.obiekt=0;          //albo wezel 0, albo kolizja
       //1-KS, 2-KO, 3-KD,  4-W, 5-G, 6-Cok, 7-Co
       //8-e,  9-E, 10-kT, 11-T
       strncpy(record_profil.node,point_xy[i].opis,15);
       record_profil.odl_0=odleglosc_00;         //poczatkowa odleglosc pierwszego wezla
       record_profil.x=point_xy[i].x;
       record_profil.y=point_xy[i].y;

       memmove(&record_profil.dane_prof,&dane_profs0,sizeof(MyDane));
       record_profil.dane_prof.odl=(odleglosc_00+point_xy[i].dlugosc);
       record_profil.dane_prof.rkp=point_xy[i].rzedna;  //tymczasowo, w nastepnej wersji mozna dodac rzedne
       record_profil.dane_prof.si3=point_xy[i].srednica;
       //poszukiwanie interpolowanych rzednych terenu
       //rt_ru.rt=rt;
       //rt_ru.ru=ru;
       if (find_level_for_node(&rt_ru, point_xy[i].x, point_xy[i].y)==1)
       {
        rt=rt_ru.rt;
        ru=rt_ru.ru;
        if (rt>0.0)
        {
         if (Check_if_Equal(ru,0.0)==TRUE) ru=rt;
        }
        record_profil.dane_prof.ru=ru;
        record_profil.dane_prof.rt=rt;
       }
       else
       {
        record_profil.dane_prof.ru=0.0;
        record_profil.dane_prof.rt=0.0;
       }
       ///////////////
       if (write(log, &record_profil, sizeof(Record_Profil)) != sizeof(Record_Profil))
       {
        komunikat(180);
        return;
       }
       zapisano_xyz=TRUE;
      }
      /////////////////
      if (n_kolizje>0)
      {
       for (i=0; i<n_kolizje; i++)
       {
        //korekta wspolrzednych z uwagi na uklad lokalny i uklad geodezyjny
        record_profil.flag=0;  //flaga kontrolna, domyslnie 0
        record_profil.l=n_points+n_kolizje;  //liczba rekordow sieci;
        record_profil.i=n_points+i;  //numer rekordu sieci, numerowany od 0
        strncpy(record_profil.file_name,zbior_danych,30);
        get_spec_string(3,nazwa_rysunku);
        strncpy(record_profil.rys_name,nazwa_rysunku,60);   //nazwa rysunku z ktorego pochodzi
        record_profil.obiekt=1;          //albo wezel 0, albo kolizja
        //1-KS, 2-KO, 3-KD,  4-W, 5-G, 6-Cok, 7-Co
        //8-e,  9-E, 10-kT, 11-T
        strncpy(record_profil.node,point_xy_k[i].opis,15);
        record_profil.odl_0=odleglosc_00;         //poczatkowa odleglosc pierwszego wezla


        memmove(&record_profil.dane_prof,&dane_profs0,sizeof(MyDane));
        record_profil.dane_prof.odl=(odleglosc_00+point_xy_k[i].dlugosc);
        //ustalenie typu instalacji poprzez zadanie rzednej i srednicy
        //Ks, Ko, Kd, W, G, e lub E, T lub Tk, Co lub Cok, inne
        ///////////////
        if ((strncmp(record_profil.node,"Ks",2)==0) ||
            (strncmp(record_profil.node,"ks",2)==0) ||
            (strncmp(record_profil.node,"KS",2)==0))
        {
         record_profil.dane_prof.rki=point_xy_k[i].rzedna;
         record_profil.dane_prof.rkp=point_xy_k[i].srednica;
         record_profil.dane_prof.spi2=0;
        }
        else
         if ((strncmp(record_profil.node,"Ko",2)==0) ||
             (strncmp(record_profil.node,"ko",2)==0) ||
             (strncmp(record_profil.node,"KO",2)==0))
         {
          record_profil.dane_prof.ru=point_xy_k[i].rzedna;
          record_profil.dane_prof.rt=point_xy_k[i].srednica;
         }
         else
          if ((strncmp(record_profil.node,"Kd",2)==0) ||
              (strncmp(record_profil.node,"kd",2)==0) ||
              (strncmp(record_profil.node,"KD",2)==0))
          {
           record_profil.dane_prof.si1=point_xy_k[i].rzedna;
           record_profil.dane_prof.si2=point_xy_k[i].srednica;
          }
          else
           if ((strncmp(record_profil.node,"W",1)==0) ||
               (strncmp(record_profil.node,"w",1)==0))
           {
            record_profil.dane_prof.si3=point_xy_k[i].rzedna;
            record_profil.dane_prof.oi1=point_xy_k[i].srednica;
           }
           else
            if ((strncmp(record_profil.node,"G",1)==0) ||
                (strncmp(record_profil.node,"g",1)==0))
            {
             record_profil.dane_prof.oi2=point_xy_k[i].rzedna;
             record_profil.dane_prof.oi3=point_xy_k[i].srednica;
            }
            else
             if ((strncmp(record_profil.node,"e",1)==0) ||
                 (strncmp(record_profil.node,"E",1)==0))
             {
              record_profil.dane_prof.sp1=point_xy_k[i].rzedna;
             }
             else
              if ((strncmp(record_profil.node,"Tk",2)==0) ||
                  (strncmp(record_profil.node,"tk",2)==0) ||
                  (strncmp(record_profil.node,"TK",2)==0))
              {
               record_profil.dane_prof.sp2=point_xy_k[i].rzedna;
              }
              else
               if ((strncmp(record_profil.node,"T",1)==0) ||
                   (strncmp(record_profil.node,"t",1)==0))
               {
                record_profil.dane_prof.sp3=point_xy_k[i].rzedna;
               }
               else
                if ((strncmp(record_profil.node,"Cok",3)==0) ||
                    (strncmp(record_profil.node,"cok",3)==0) ||
                    (strncmp(record_profil.node,"COK",3)==0) ||
                    (strncmp(record_profil.node,"CoK",3)==0) ||
                    (strncmp(record_profil.node,"COk",3)==0))
                {
                 record_profil.dane_prof.rki=point_xy_k[i].rzedna;
                 record_profil.dane_prof.rkp=point_xy_k[i].srednica;
                 record_profil.dane_prof.spi2=2;
                }
                else
                 if ((strncmp(record_profil.node,"Co",2)==0) ||
                   (strncmp(record_profil.node,"co",2)==0) ||
                   (strncmp(record_profil.node,"CO",2)==0))
                 {
                  record_profil.dane_prof.rki=point_xy_k[i].rzedna;
                  record_profil.dane_prof.rkp=point_xy_k[i].srednica;
                  record_profil.dane_prof.spi2=1;
                 }
                 else
                 {
                  record_profil.dane_prof.op2=point_xy_k[i].rzedna;
                 }

        ///////////////
        if (write(log, &record_profil, sizeof(Record_Profil)) != sizeof(Record_Profil))
        {
         komunikat(180);
         return;
        }
        zapisano_xyz=TRUE;
       }
      }
      /////////////////
     }
     //////////////
     adp+=sizeof(NAGLOWEK) + b->n;
    }
    else
    {
     adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
   }
   else adp+= nag->n + sizeof(NAGLOWEK);
  }
  else
  {
   adp += nag->n	+ sizeof(NAGLOWEK) ;
  }
  nag	= (NAGLOWEK*)adp;
 }

 //zakonczenie profilu
 close (log) ;
 //
 if (zapisano_xyz==TRUE)
 {

  if (ask_question(1, "","OK","",(char*)_networks_saved_, 11, "", 11, 1, 87)==0)
  {
   ;
  }
 }
 else if (ask_question(1, "","OK","",(char*)_no_descripted_networks_, 12, "", 12, 1, 62)==0)
 {
  ;
 }
}

#undef __O_TABWSP__