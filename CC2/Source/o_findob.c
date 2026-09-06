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

#include<forwin.h>
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
#include<stdio.h>
#include<stdarg.h>
#include<sys/stat.h>
#include<values.h>
#define ALLEGWIN
#include <allegext.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_bloklb.h"


char *find_obj(char *adrp, char *adrk, int obiekt, int typ_obiektu, int info_p)
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
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
       if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
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
       }
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}

char *find_obj_in_layer(char *adrp, char *adrk, int obiekt, int typ_obiektu, char obiektt2, int warstwa, int info_p)
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
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
       if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
       {
       if ((L->obiekt==obiekt) && (L->warstwa==warstwa) &&
           ((L->obiektt2==obiektt2) || (obiektt2==O2NieOkreslony)))
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
       }
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}

char *find_obj_count(char *adrp, char *adrk, int obiekt, int typ_obiektu, int info_p, int count)
{ BLOK *b;
  LINIA *L;
  TEXT *T;
  T_Point *P;
  char *adpp;
  int i_count;

  adpp=adrp;

  i_count=0;
  while (adpp<adrk)
   {
    L=(LINIA *)adpp;
    if (L->obiekt==OdBLOK)
     {
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
      if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
       {
       if (L->obiekt==obiekt)
        {
         if ((Layers[L->warstwa].on==TRUE) &&
            ((Layers[L->warstwa].edit==TRUE) || (info_p==1)))
         {
          if (typ_obiektu==ONieOkreslony)
           {
            i_count++;
            if (i_count==count)
             {
              return adpp;
              break;
             }
           }
           else
            {
             if (L->obiekt==Otekst)
              {
                T=(TEXT *) adpp;
                if (T->typ==typ_obiektu)
                 {
                  i_count++;
                  if (i_count==count)
                   {
                    return adpp;
                    break;
                   }
                 }
              }
               else if (L->obiekt==Opoint)
                {
                 P=(T_Point *) adpp;
                 if (P->typ==typ_obiektu)
                  {
                   i_count++;
                   if (i_count==count)
                    {
                     return adpp;
                     break;
                    }
                  }
                }
               else if (L->obiekt==Olinia)
                {
                 L=(LINIA *) adpp;
                 if (L->typ==typ_obiektu)
                  {
                   i_count++;
                   if (i_count==count)
                    {
                     return adpp;
                     break;
                    }
                  }
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



char *find_obj_count_abs(char *adrp, char *adrk, int obiekt, int typ_obiektu, int info_p, int count)
{ BLOK *b;
  LINIA *L;
  TEXT *T;
  T_Point *P;
  char *adpp;
  int i_count;

  adpp=adrp;

  i_count=0;
  while (adpp<adrk)
   {
    L=(LINIA *)adpp;
    if (L->obiekt==OdBLOK)
     {
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
      if (L->atrybut!=Abad)
       {
       if (L->obiekt==obiekt)
        {
         if ((Layers[L->warstwa].on==TRUE) &&
            ((Layers[L->warstwa].edit==TRUE) || (info_p==1)))
         {
          if (typ_obiektu==ONieOkreslony)
           {
            i_count++;
            if (i_count==count)
             {
              return adpp;
              break;
             }
           }
           else
            {
             if (L->obiekt==Otekst)
              {
                T=(TEXT *) adpp;
                if (T->typ==typ_obiektu)
                 {
                  i_count++;
                  if (i_count==count)
                   {
                    return adpp;
                    break;
                   }
                 }
              }
               else if (L->obiekt==Opoint)
                {
                 P=(T_Point *) adpp;
                 if (P->typ==typ_obiektu)
                  {
                   i_count++;
                   if (i_count==count)
                    {
                     return adpp;
                     break;
                    }
                  }
                }
               else if (L->obiekt==Olinia)
                {
                 L=(LINIA *) adpp;
                 if (L->typ==typ_obiektu)
                  {
                   i_count++;
                   if (i_count==count)
                    {
                     return adpp;
                     break;
                    }
                  }
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



char *find_obj2(char *adrp, char *adrk, int obiekt1, int obiekt2, int typ_obiektu1, int typ_obiektu2, int info_p)
{ BLOK *b;
  LINIA *L;
  LUK *l;
  TEXT *T;
  T_Point *P;
  char *adpp;

  adpp=adrp;

  while (adpp<adrk)
   {
    L=(LINIA *)adpp;
    if (L->obiekt==OdBLOK)
     {
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
      if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
       {
       if (L->obiekt==obiekt1)
        {
         if ((Layers[L->warstwa].on==TRUE) &&
            ((Layers[L->warstwa].edit==TRUE) || (info_p==1)))
         {
          if (typ_obiektu1==ONieOkreslony)
           {
            return adpp;
            break;
           }
           else
            {
             if (L->obiekt==Otekst)
              {
                T=(TEXT *) adpp;
                if (T->typ==typ_obiektu1)
                 {
                  return adpp;
                  break;
                 }
              }
               else if (L->obiekt==Opoint)
                {
                 P=(T_Point *) adpp;
                 if (P->typ==typ_obiektu1)
                  {
                   return adpp;
                   break;
                  }
                }
               else if (L->obiekt==Olinia)
                {
                 L=(LINIA *) adpp;
                 if (L->typ==typ_obiektu1)
                  {
                   return adpp;
                   break;
                  }
                }
               else if (L->obiekt==Oluk)
                {
                 l=(LUK *) adpp;
                 if (l->typ==typ_obiektu1)
                  {
                   return adpp;
                   break;
                  }
                }
            }
         }
        }
        else if (L->obiekt==obiekt2)
        {
         if ((Layers[L->warstwa].on==TRUE) &&
            ((Layers[L->warstwa].edit==TRUE) || (info_p==1)))
         {
          if (typ_obiektu2==ONieOkreslony)
           {
            return adpp;
            break;
           }
           else
            {
             if (L->obiekt==Otekst)
              {
                T=(TEXT *) adpp;
                if (T->typ==typ_obiektu2)
                 {
                  return adpp;
                  break;
                 }
              }
               else if (L->obiekt==Opoint)
                {
                 P=(T_Point *) adpp;
                 if (P->typ==typ_obiektu2)
                  {
                   return adpp;
                   break;
                  }
                }
               else if (L->obiekt==Olinia)
                {
                 L=(LINIA *) adpp;
                 if (L->typ==typ_obiektu2)
                  {
                   return adpp;
                   break;
                  }
                }
               else if (L->obiekt==Oluk)
                {
                 l=(LUK *) adpp;
                 if (l->typ==typ_obiektu2)
                  {
                   return adpp;
                   break;
                  }
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

char *find_obj1(char *adrp, char *adrk, int obiekt, int typ_obiektu, char obiektt2)
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
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
      if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
       {
       if (L->obiekt==obiekt)
        {
         if ((L->atrybut!=Abad) &&
            (L->atrybut!=Ausuniety))
         {
          if (typ_obiektu==ONieOkreslony)
           {
            return adpp;
            break;
           }
           else
            {
             if ((L->obiektt2==obiektt2) || (obiektt2==O2NieOkreslony))
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
        }
       }
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}

/*
void *find_obj_block(char *adrp, char *adrk, char kod_obiektu, char opis_obiektu)
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
       b=(BLOK *) adpp;
       if (kod_obiektu==B_NIEOKRESLONY)
        {
         if (b->opis_obiektu[0]==opis_obiektu)
          {
            return adpp;
            break;
          }
        }
         else
          {
           if (b->kod_obiektu == kod_obiektu)
            {
             if (b->opis_obiektu[0]==opis_obiektu)
              {
                return adpp;
                break;
              }
            }
          }
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}

*/

char *find_obj3(char *adrp, char *adrk, int obiekt, int typ_obiektu1, int typ_obiektu2, int typ_obiektu3, char obiektt2)
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
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
      if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
       {
       if (L->obiekt==obiekt)
        {
         if ((L->atrybut!=Abad) &&
            (L->atrybut!=Ausuniety))
         {
          if (typ_obiektu1==ONieOkreslony)
           {
            return adpp;
            break;
           }
           else
            {
             if (L->obiektt2==obiektt2)
             {
             if (L->obiekt==Otekst)
              {
                T=(TEXT *) adpp;
                if ((T->typ==typ_obiektu1) ||
                    (T->typ==typ_obiektu2) ||
                    (T->typ==typ_obiektu3))
                 {
                  return adpp;
                  break;
                 }
              }
               else if (L->obiekt==Opoint)
                {
                 P=(T_Point *) adpp;
                 if ((P->typ==typ_obiektu1) ||
                     (P->typ==typ_obiektu2) ||
                     (P->typ==typ_obiektu3))
                  {
                   return adpp;
                   break;
                  }
                }
               else if (L->obiekt==Olinia)
                {
                 L=(LINIA *) adpp;
                 if ((L->typ==typ_obiektu1) ||
                     (L->typ==typ_obiektu2) ||
                     (L->typ==typ_obiektu3))
                  {
                   return adpp;
                   break;
                  }
                }
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


char *find_obj3suma(char *adrp, char *adrk, int obiekt, int typ_obiektu1, int typ_obiektu2, int typ_obiektu3, char obiektt2, BOOL *suma_l)
{ BLOK *b;
  LINIA *L;
  TEXT *T;
  T_Point *P;
  char *adpp;
  BOOL suma_ll;

  adpp=adrp;
  suma_ll=TRUE;

  while (adpp<adrk)
   {
    L=(LINIA *)adpp;
    if (L->obiekt==OdBLOK)
     {
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
      suma_ll=FALSE;
     }
      else
      {
       if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
       {
       if (L->obiekt==obiekt)
        {
         if ((L->atrybut!=Abad) &&
            (L->atrybut!=Ausuniety))
         {
          if (typ_obiektu1==ONieOkreslony)
           {
            *suma_l=suma_ll;
            return adpp;
            break;
           }
           else
            {
             if ((L->obiektt2==obiektt2) || (L->obiektt2==O2NoBlockS))
             {
             if (L->obiekt==Otekst)
              {
                T=(TEXT *) adpp;
                if ((T->typ==typ_obiektu1) ||
                    (T->typ==typ_obiektu2) ||
                    (T->typ==typ_obiektu3))
                 {
                  *suma_l=suma_ll;
                  return adpp;
                  break;
                 }
              }
               else if (L->obiekt==Opoint)
                {
                 P=(T_Point *) adpp;
                 if ((P->typ==typ_obiektu1) ||
                     (P->typ==typ_obiektu2) ||
                     (P->typ==typ_obiektu3))
                  {
                   *suma_l=suma_ll;
                   return adpp;
                   break;
                  }
                }
               else if (L->obiekt==Olinia)
                {
                 L=(LINIA *) adpp;
                 if ((L->typ==typ_obiektu1) ||
                     (L->typ==typ_obiektu2) ||
                     (L->typ==typ_obiektu3))
                  {
                   *suma_l=suma_ll;
                   return adpp;
                   break;
                  }
                }
             }
            }
         }
        }
       }
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  *suma_l=suma_ll;
  return NULL ;
}


char *find_obj_atrybut(char *adrp, char *adrk, int obiekt, char atrybut)
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
      b=(BLOK *)adpp;
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
      if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
       {
       if (L->obiekt==obiekt)
        {
         if (L->atrybut==atrybut)
         {
           return adpp;
           break;
         }
        }
       }
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}


char *find_block_atrybut(char *adrp, char *adrk, char atrybut)
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
      b=(BLOK *)adpp;
      if (b->atrybut==atrybut)
       {
        return adpp;
        break;
       }
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}


char *find_block(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu)
{ BLOK *b;
  LINIA *L;
  TEXT *T;
  T_Point *P;
  char *adpp;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;

  adpp=adrp;

  while (adpp<adrk)
   {
    L=(LINIA *)adpp;
    if (L->obiekt==OdBLOK)
     {
      b=(BLOK *)adpp;
      if ((kod_obiektu==B_NIEOKRESLONY) || (b->kod_obiektu==kod_obiektu))
       {
        if (strlen(opis_obiektu)>0)
        {
        if (b->dlugosc_opisu_obiektu>1)
          {
            ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
            if (ptrs_desc_bl->sz_type [0] != '\0')
             {
              if (strcmp(opis_obiektu,ptrs_desc_bl->sz_type)==0)
               {
                return adpp;
                break;
               }
             }
          }
        }
         else
          {
            return adpp;
            break;
          }
       }
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}

char *find_block_sub(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu, int l_char)
{ BLOK *b;
  LINIA *L;
  TEXT *T;
  T_Point *P;
  char *adpp;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;

  adpp=adrp;

  while (adpp<adrk)
   {
    L=(LINIA *)adpp;
    if (L->obiekt==OdBLOK)
     {
      b=(BLOK *)adpp;
      if ((kod_obiektu==B_NIEOKRESLONY) || (b->kod_obiektu==kod_obiektu))
       {
        if (strlen(opis_obiektu)>0)
        {
        if (b->dlugosc_opisu_obiektu>1)
          {
            ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
            if (ptrs_desc_bl->sz_type [0] != '\0')
             {
              if (strncmp(ptrs_desc_bl->sz_type,opis_obiektu,l_char)==0)
               {
                return adpp;
               }
             }
          }
        }
        else
        {
           return adpp;
        }
       }
      adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     }
      else
      {
       adpp+=sizeof(NAGLOWEK) + L->n;
      }
   }
  return NULL ;
}


char *find_block_sub_on(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu, int l_char)  //find visible block
{

 BLOK *b, *b1;
 LINIA *L;
 TEXT *T;
 T_Point *P;
 char *adpp, *adpp1;
 T_Desc_Ex_Block 	*ptrs_desc_bl ;

 adpp=adrp;

 while (adpp<adrk)
 {
  L=(LINIA *)adpp;
  if (L->obiekt==OdBLOK)
  {
   b=(BLOK *)adpp;
   if ((kod_obiektu==B_NIEOKRESLONY) || (b->kod_obiektu==kod_obiektu))
   {
    if (strlen(opis_obiektu)>0)
    {
     if (b->dlugosc_opisu_obiektu>1)
     {
      ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
      if (ptrs_desc_bl->sz_type [0] != '\0')
      {
       if (strncmp(ptrs_desc_bl->sz_type,opis_obiektu,l_char)==0)
       {
        //let's check visibility of next primitive
        adpp1=adpp+sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
        while (adpp1<adrk)
        {
         L=(LINIA *)adpp1;
         if (L->obiekt==OdBLOK)
         {
          b1=(BLOK *)adpp1;
          adpp1+=sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu;
         }
         else
         {
          if  ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad) &&
              (Layers[L->warstwa].on==TRUE) &&
              (Layers[L->warstwa].edit==TRUE))
           return adpp;
          break;
         }
        }
        adpp+=sizeof(NAGLOWEK)+b->n;
        continue;
       }
      }
     }
    }
    else
    {
     //let's check visibility of next primitive
     adpp1=adpp+sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
     while (adpp1<adrk)
     {
      L=(LINIA *)adpp1;
      if (L->obiekt==OdBLOK)
      {
       b1=(BLOK *)adpp1;
       adpp1+=sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu;
      }
      else
      {
       if  ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad) &&
           (Layers[L->warstwa].on==TRUE) &&
           (Layers[L->warstwa].edit==TRUE))
        return adpp;
       break;
      }
     }
     adpp+=sizeof(NAGLOWEK)+b->n;
     continue;
    }
   }
   adpp+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
  }
  else
  {
   adpp+=sizeof(NAGLOWEK) + L->n;
  }
 }
 return NULL ;
}