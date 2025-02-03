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

#define no_Check_Blocks

#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdio.h>
#ifndef LINUX
#include<io.h>
#endif
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_debug.h"
#include "o_libfun.h"
#include "o_object.h"
#include "b_messag.h"
#include "o_pline.h"


static int set_block_flag (void) ;
static long_long tokoff = -1 ;

static int i__version = VERSION1_1 ;

enum TOK_MODE {TM_NO = 0, TM_TOK, TM_TOK1, TM_TOKALL, TM_TOKOFF} ; /*powtorzono e o_obiect.c*/

extern char  *get_dane0all(void);  /*zmienne globalne zdefiniowane w o_obiect.c*/
extern char  *get_dane01(void);
extern char  *get_dane0(void);
extern long_long l_tokoff ;

void Set_Version (int i_ver)
/*------------------------*/
{
  i__version = i_ver ;
}

void Normalize_Objects (long_long off, long_long offk, int atrybut)
/*----------------------------------------------------------[----*/
{
  NAGLOWEK *nag ;
  long_long ad ;

  ObiektTok (&off, offk, &ad, Owwielokat) ; /*na razie tylko obszar*/
  while (ad!=-1)
  {
    nag=(NAGLOWEK*)(dane+ad);
    if (nag->atrybut == atrybut)
    {
      Normalize_Object (nag) ;
    }
    ObiektTok (&off, offk, &ad, Owwielokat) ;
  }
}

void Normalize_Object (NAGLOWEK *nag)
/*-----------------------------------*/
{
  switch(nag->obiekt)
  {
     case Owwielokat :
	 Normalize_Solid ((WIELOKAT*)nag) ;
	 break ;
     default :
       break ;
  }
}

BOOL Check_Text (NAGLOWEK *nag, BOOL b_correct)
/*--------------------------------------------*/
{
  TEXT *t;
  TEXT_OLD1_0 *t1_0 ;

  b_correct = b_correct ;
  switch (i__version)
  {
    case VERSION1_0 :
	t1_0 = (TEXT_OLD1_0*)nag;
	if (t1_0->n + sizeof(NAGLOWEK) > sizeof (TEXT_OLD1_0))  /*w przyblizeniu*/
	{
	  ErrList (40);
	  return FALSE;
	}
	if (t1_0->dl != strlen (t1_0->text))
	{
	  ErrList (40);
	  return FALSE;
	}
	break ;
    case VERSION1_1 :
	t = (TEXT*)nag;
	if (t->n + sizeof(NAGLOWEK) > sizeof (TEXT))  /*w przyblizeniu*/
	{
	  ErrList (40);
	  return FALSE;
	}
	if (t->dl != strlen (t->text))
	{
	  ErrList (40);
	  return FALSE;
	}
	break ;
   }
   return TRUE ;
}


BOOL Check_Object (NAGLOWEK *adr_nag, BOOL b_correct)
/*-------------------------------------------------*/
{
  NAGLOWEK *nag_next ;
  LINIA *L;
  LINIA3D *L3D;
  OKRAG  *k;
  LUK *l;
  WIELOKAT *w ;
  BLOK *ptr_b ;
  B_PCX *pcx;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  BOOL b_err ;
  double df_1, df_2 ;
  NAGLOWEK *nag;
  

  nag = (NAGLOWEK *) adr_nag;

  if (nag->obiekt >= ONoObiect && nag->obiekt != Okoniec)
  {
      ErrList (40);
      return FALSE;
  }
  if (nag->atrybut != Anormalny &&
     (nag->atrybut != Ausuniety  && nag->atrybut != Abad) &&
     b_correct == TRUE)
  {
      nag->atrybut = Anormalny;
  }
  switch(nag->obiekt)
    { case Olinia :
	L = (LINIA*)nag;
	if ((L->n + sizeof(NAGLOWEK)) !=  sizeof (LINIA) )
	{
        //check if not extended data for Graph
        unsigned char *flags;
        flags=(char*)L + sizeof (LINIA);
        if ((*flags==71) || (*flags==45)) break;  //it's extended data
	  ErrList (40);
	  return FALSE;
	}  
	break;
      case Otekst :
	b_err = Check_Text (nag, b_correct) ;
	return b_err ;
      case Okolo :
     case Ookrag :
	 k=(OKRAG*)nag;
	 if (k->n + sizeof(NAGLOWEK) != sizeof (OKRAG))
	 {
	   ErrList (40);
	   return FALSE;
	 }
	 if (TRUE == Check_if_LE (k->r, 0) && k->atrybut != Abad)
	 {
	   ErrList (39);
	   if (b_correct == TRUE)
	   {
	     k->atrybut = Abad;
	   }
	   else
	   {
	     return FALSE ;
	   }
	   break;
	 }
       break;
     case Oluk :
	 l=(LUK*)nag;
	 if (l->n + sizeof(NAGLOWEK) != sizeof (LUK))
	 {
	   ErrList (40);
	   return FALSE;
	 }
	 if (TRUE == Check_if_LE (l->r, 0) && l->atrybut != Abad)
	 {
	   if (b_correct == TRUE)
	   {
	     l->atrybut = Abad;
	   }
	   else
	   {
	     return FALSE ;
	   }
	   break;
	 }
	 df_1 = (double)(l->kat1) ;
	 df_2 = (double)(l->kat2) ;
	 l->kat1 = Angle_Normal (l->kat1) ;
	 l->kat2 = Angle_Normal (l->kat2) ;
	 if (TRUE == Check_if_Equal (df_1, df_2))
	 {
	   ErrList (66);  //this message is just disturbing, so better just mark it Abad  TO DO
	   if (b_correct == TRUE)
	   {
	     l->atrybut = Abad;
	   }
	   else
	   {
	     return FALSE ;
	   }
	   break;
	 }
       break;
     case Owwielokat :
	 w = (WIELOKAT*)nag ;
	 if (w->n + sizeof(NAGLOWEK) > sizeof (WIELOKAT))
	 {
	   ErrList (40) ;
	   return FALSE ;
	 }
    if (w->lp > 64)
	 {
	   ErrList (40) ;
	   if (b_correct == TRUE)
	   {
	     w->atrybut = Abad;
	   }
	   else
	   {
	     return FALSE ;
	   }
	 }
	 break ;
     case OdBLOK:
	 b_err = FALSE ;
	 ptr_b = (BLOK*)adr_nag ;
     if (ptr_b->n > INT_MAX) 
     {
      ptr_b->n=(B3 + ptr_b->dlugosc_opisu_obiektu);
     }
	 if (ptr_b->n <= (unsigned)ptr_b->dlugosc_opisu_obiektu)
	 {
	   b_err = TRUE ;
	 }
	 if (B_DIM    != ptr_b->kod_obiektu &&
	     B_DIM2 != ptr_b->kod_obiektu &&
	     B_DIM1 != ptr_b->kod_obiektu &&
         B_DIM3 != ptr_b->kod_obiektu &&
	     B_EXPORT != ptr_b->kod_obiektu &&
         B_GRAPH != ptr_b->kod_obiektu &&
	     B_PLINE  != ptr_b->kod_obiektu &&
		 B_SHADOW != ptr_b->kod_obiektu &&
	     B_HATCH  != ptr_b->kod_obiektu &&
        B_PPOMIAR  != ptr_b->kod_obiektu &&
        B_KPOMIAR  != ptr_b->kod_obiektu &&
        B_INSTALACJE  != ptr_b->kod_obiektu &&
        B_HEKTOMETRY  != ptr_b->kod_obiektu &&
        B_SEKTORY  != ptr_b->kod_obiektu &&
        B_INSTALACJE_OPIS  != ptr_b->kod_obiektu &&
        B_INSTALACJE_K != ptr_b->kod_obiektu &&
        B_ODWIERT_OLD  != ptr_b->kod_obiektu &&
        B_ODWIERT  != ptr_b->kod_obiektu &&
        B_WIRE  != ptr_b->kod_obiektu &&
        B_ARM != ptr_b->kod_obiektu &&
        B_SIEC != ptr_b->kod_obiektu &&
        B_VERTEX != ptr_b->kod_obiektu &&
        B_032 != ptr_b->kod_obiektu &&
	     ptr_b->atrybut != Abad)
	 {
	   b_err = TRUE ;
	 }

     if (ptr_b->kod_obiektu == B_032) ptr_b->kod_obiektu = B_EXPORT;  //overwrite the mistake

	 if (ptr_b->kod_obiektu == B_EXPORT && ptr_b->dlugosc_opisu_obiektu>0)
	 {
	   ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptr_b->opis_obiektu [0]) ;
	   if (ptrs_desc_bl->flags != EBF_Old &&
	       ptrs_desc_bl->flags != 30 &&
	       ptrs_desc_bl->flags != EBF_IP &&
		   ptrs_desc_bl->flags != PL_RECTANGLE
		   )
	   {
	     b_err = TRUE ;
	   }
	 }
	 if (b_err == TRUE)
	 {
	   ErrList (40) ;
	   if (b_correct == TRUE)
	   {
	     char  *ptrh_tmp ;

	     ptr_b->atrybut = Abad;
	     ptrh_tmp = (char *)ptr_b ;
	     nag_next = (NAGLOWEK*)(ptrh_tmp + sizeof(NAGLOWEK) + B3 + ptr_b->dlugosc_opisu_obiektu) ;
	     if (TRUE == Check_Object (nag_next, FALSE))
	     {
	       ptr_b->n = B3 + ptr_b->dlugosc_opisu_obiektu ; /*eksplozja bloku jest mozliwa*/
	       return TRUE ;
	     }
	     return FALSE ;  			/*jest bardzo zle*/
	   }
	   else
	   {
	     return FALSE ;
	   }
	 }
	 break;
	 case Opcx:
	    pcx=(B_PCX*)nag;
     break;
     default :
       break ;
    }
  return TRUE;
}

BOOL Check_Objects (long_long off, long_long offk, BOOL b_file)
/*--------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long ad ;
  int flag_sets;

  flag_sets=0;

#ifndef no_Check_Blocks

  if (FALSE == Check_Blocks (off, offk))
  {
   ClearInfo () ;
    return FALSE ;
  }

  if (b_file == TRUE)
  {
    flag_sets=set_block_flag ();
  }

#endif

  Object_Tok_All (&off, offk, &ad, ONieOkreslony);
  while (ad!=-1)
  {
    nag=(NAGLOWEK*)(dane+ad);
            //korekta tekstu dla plikow uszkodzonych przy edycji Profili

	if (FALSE == Check_Object ((NAGLOWEK *)(dane+ad), TRUE))
    {
      Test_Obiekts_Report () ;
      ClearInfo () ;
      return FALSE;
    }
    Normalize_Object (nag) ;
    if (nag->obiekt == OdBLOK)
    {
      Check_Pline ((BLOK *)nag) ;
    }
    Object_Tok_All (&off, offk, &ad, ONieOkreslony);
  }
  if (0 == Error)  
  {
	if (Info) Info = 0;
  }

  if (flag_sets)
   {
    ADP=dane;
    ADK=dane + dane_size;
    usun_blok(ADP,ADK);
   }
 return TRUE;
}

BOOL Check_Next_Object (void *ptr_ob)
/*----------------------------------*/
{
  char  *ptrh_tmp ;
  NAGLOWEK *nag, *nag_next ;
  BLOK *ptrs_block ;

  nag = (NAGLOWEK*)ptr_ob ;
  if (nag->obiekt != OdBLOK)
  {
    return TRUE ;
  }
  ptrh_tmp = (char  *)ptr_ob ;
  ptrs_block = (BLOK*)ptr_ob ;
  nag_next = (NAGLOWEK*)(ptrh_tmp + sizeof(NAGLOWEK) + ptrs_block->n) ;
  if (FALSE == Check_Object (nag_next, FALSE))
  {
    nag_next = (NAGLOWEK*)(ptrh_tmp + sizeof(NAGLOWEK) + B3 + ptrs_block->dlugosc_opisu_obiektu) ;
    if (FALSE == Check_Object (nag_next, FALSE))
    {
      ptrs_block->atrybut = Abad ;
      return FALSE ;  /*nie mozna dokonac korekty rozmiaru bloku*/
    }
    ptrs_block->n = B3 + ptrs_block->dlugosc_opisu_obiektu ;
    ptrs_block->atrybut = Abad ;
    ErrList (72) ;
    return TRUE ;
  }
  return TRUE ;
}



static int set_block_flag (void)
/*-----------------------------*/
{
  long_long off, offk, adl ;
  long_long off_b, offk_b, adl_b ;
  BLOK *ptrs_b ;

  off = 0 ;
  offk = dane_size ;
  //poszukiwanie pierwszego bloku
  Object_Tok_All (&off, offk, &adl, OdBLOK) ;
  while ((adl!=-1) && (adl<dane_size))
  {
    off_b = adl ;
    ptrs_b =(BLOK*)(dane + off_b) ;
    if (ptrs_b->atrybut==Abad)
      {
        ptrs_b->atrybut=Ablok;
        return 1;
      }
  else
   {
    offk_b = off_b + sizeof(NAGLOWEK) + ptrs_b->n - 1 ;
    Object_Tok_All (&off_b, offk_b, &adl_b, ONieOkreslony) ;
    while ((adl_b != -1) && (adl_b<dane_size))
    {
      if (killpline == 1)
       { 
         if (ptrs_b->kod_obiektu == B_PLINE)
          {
           ((NAGLOWEK*)(dane + adl_b))->blok = 0;
           ((NAGLOWEK*)(dane + adl_b))->obiektt2 = O2NoBlockS;
          }
          else ((NAGLOWEK*)(dane + adl_b))->blok = 1;
       }
       else
        ((NAGLOWEK*)(dane + adl_b))->blok = 1;
      Object_Tok_All (&off_b, offk_b, &adl_b, ONieOkreslony) ;
    }
    ptrs_b->blok = 0;
    if (killpline == 1)
       { 
         if (ptrs_b->kod_obiektu == B_PLINE)
          {
           ptrs_b->n = B3 + sizeof(char);
           if (ptrs_b->blok == 0)  // usuwany tylko wtedy 
                                   // gdy nie jest elementem bloku
            {
             ptrs_b->atrybut = Ausuniety;
            }
          }
       }
    off = adl + sizeof(NAGLOWEK) + B3 + ptrs_b->dlugosc_opisu_obiektu;
   }
   Object_Tok_All (&off, offk, &adl, OdBLOK) ;
  }
 return 0;
}

/*--------------------------------------------------*/
static void report_obiect_param (FILE *stru, NAGLOWEK *nag) ;


BOOL Check_Blocks (long_long off, long_long offk)
/*-----------------------------------*/
{
 NAGLOWEK *nag, *nag_next ;
 BLOK *ptrs_b ;
 long_long ad ;

  Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    
    if (nag->obiekt == OdBLOK)
    {
      ptrs_b = (BLOK*)(dane + ad);
      if ((long)ptrs_b->n > dane_size)
      {
       ptrs_b->n = B3 + (unsigned)ptrs_b->dlugosc_opisu_obiektu ;
	    ptrs_b->atrybut = Abad ;
	    ErrList (72) ;
      }
       else
      {
       nag_next = (NAGLOWEK*)(dane + ad + sizeof(NAGLOWEK) + ptrs_b->n) ;
       if (FALSE == Check_Object (nag_next, FALSE))
       {
        nag_next = (NAGLOWEK*)(dane + ad + sizeof(NAGLOWEK) + B3 + (unsigned)ptrs_b->dlugosc_opisu_obiektu) ;
	     if (FALSE == Check_Object (nag_next, FALSE))
	      {
	       ptrs_b->atrybut = Abad ;
     	    ErrList (40) ;
	       return FALSE ;  /*nie mozna dokonac korekty rozmiaru bloku*/
	      }
        ptrs_b->n = B3 + (unsigned)ptrs_b->dlugosc_opisu_obiektu ;
	     ptrs_b->atrybut = Abad ;
	     ErrList (72) ;
       }
      }
   }
    Object_Tok_All (&off, offk, &ad, ONieOkreslony);
  }
  return TRUE ;
}


void test_blok (FILE *stru)
/*----------------------*/
{
 NAGLOWEK *nag, *nag_next ;
 BLOK *ptrs_b ;
 long_long ad, off, offk ;

  fputs ("\nCheck bloks\n" , stru) ;
  off = 0 ;
  offk = dane_size ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    if (nag->obiekt == OdBLOK)
    {
      ptrs_b = (BLOK*)nag ;
      nag_next = (NAGLOWEK*)(dane + ad + sizeof(NAGLOWEK) + ptrs_b->n) ;
      if (FALSE == Check_Object (nag_next, FALSE))
      {
	report_obiect_param (stru, nag) ;
	fputs ("next obiect - ", stru) ;

	report_obiect_param (stru, nag_next) ;
      }
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony);
  }
}


static void report_tok_param (FILE *stru)
/*--------------------------------------*/
{
  char buf [300] ;

  fputs ("Report tok_param\n" , stru) ;

  switch (Get_Tok_Mode ())
  {
    case TM_NO :
       tokoff = -1 ;
       break ;
    case TM_TOK :
       tokoff = get_dane0() - dane ;
       break ;
    case TM_TOK1 :
       tokoff = get_dane01() - dane ;
       break ;
    case TM_TOKALL :
       tokoff = get_dane0all() - dane ;
       break ;
    case TM_TOKOFF :
       tokoff = l_tokoff ;
       break ;
  }
  sprintf (buf, "last token\n offset = %ld\n", tokoff) ;
  fputs (buf , stru) ;
  if (tokoff != -1)
    report_obiect_param (stru, (NAGLOWEK *)(dane + tokoff)) ;
}

static void report_obiect_param (FILE *stru, NAGLOWEK *nag)
/*-------------------------------------------------------*/
{
  char buf [300] ;
  char  *ptrh_temp ;
  LINIA *ptrs_line ;
  LINIA3D *ptrs_line3D ;
  TEXT_OLD1_0 *ptrs_t_old0_1 ;
  TEXT *ptrs_t ;
  LUK *ptrs_arc ;
  OKRAG *ptrs_circle ;
  BLOK *ptrs_b ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  T_Desc_Ex_Block_Old   *ptrs_desc_bl_old ;
  WIELOKAT *ptrs_w ;
  int i,j ;

  ptrh_temp = (char *)nag ;

  sprintf (buf, "\n offset = %ld\n", ptrh_temp - dane) ;
  fputs (buf , stru) ;

  sprintf (buf, " attribute = %u,  obiect= %u,  obiectt1 = %u,  obiectt2 = %u,\n  obiectt3 = %u,  view = %u,  temp = %u,  block  = %u, size = %u\n",
	nag->atrybut, nag->obiekt, nag->obiektt1, nag->obiektt2,
	nag->obiektt3, nag->widoczny, nag->przec, nag->blok, nag->n) ;
  fputs (buf , stru) ;
  buf [0] = '\0' ;
  switch (nag->obiekt)
  {
    case Olinia :
       ptrs_line = (LINIA*)nag ;
       sprintf (buf, " x1=%lg  y1=%lg  x2=%gl  y2=%lg\n",
	      (double)ptrs_line->x1, (double)ptrs_line->y1,
	      (double)ptrs_line->x2, (double)ptrs_line->y2) ;
       break ;
    case Otekst :
       if (i__version == VERSION1_0)
       {
	 ptrs_t_old0_1= (TEXT_OLD1_0*)nag ;
	 sprintf (buf, "x=%lg  y=%lg  '%.50s'\n", (double)ptrs_t_old0_1->x,
		(double)ptrs_t_old0_1->y, ptrs_t_old0_1->text) ;
       }
       else
       {
	 ptrs_t = (TEXT*)nag ;
	 sprintf (buf, "x=%lg  y=%lg  '%.50s'\n", (double)ptrs_t->x,
		(double)ptrs_t->y, ptrs_t->text) ;
       }
       break ;
    case Ookrag :
    case Okolo :
       ptrs_circle = (OKRAG*)nag ;
       sprintf (buf, "x=%lg  y=%lg  r=%lg\n", (double)ptrs_circle->x,
		(double)ptrs_circle->y, (double)ptrs_circle->r) ;
       break ;
    case Oluk :
       ptrs_arc = (LUK*)nag ;
       sprintf (buf, "x=%lg  y=%lg  r=%lg  a1=%lg  a2=%lg\n",
		(double)ptrs_arc->x, (double)ptrs_arc->y,(double)ptrs_arc->r,
		(double)ptrs_arc->kat1, (double)ptrs_arc->kat2) ;
       break ;
    case Owwielokat :
       ptrs_w = (WIELOKAT*)nag ;
       sprintf (buf, "lp=%d",ptrs_w->lp) ;
       for (i = 0 ; i < (int)ptrs_w->lp; i++)
	    sprintf (&buf [strlen (buf)], "  %lg", (double)ptrs_w->xy [i]) ;
       sprintf (&buf [strlen (buf)], "\n") ;
       break ;
    case OdBLOK :
       ptrs_b = (BLOK*)nag ;
       sprintf (buf, "typ bloku=%c  dlugosc opisu bloku=%d\n", ptrs_b->kod_obiektu,
				ptrs_b->dlugosc_opisu_obiektu) ;
       if (ptrs_b->kod_obiektu == B_EXPORT && 0 < ptrs_b->dlugosc_opisu_obiektu)
       {
	 ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_b->opis_obiektu [0]) ;
	 if (ptrs_desc_bl->flags == EBF_IP ) /*EBF_Old  i 30 w wyniku bledu w save_spec0*/
	 {
	   sprintf (&buf [strlen (buf)] ,
		" flags=%u  x=%lg  y=%lg  len=%d  '%.20s'\n",
		ptrs_desc_bl->flags,
		(double)ptrs_desc_bl->x, (double)ptrs_desc_bl->y,
		ptrs_desc_bl->len, ptrs_desc_bl->sz_type);
	 }
	 else
	 {
	   ptrs_desc_bl_old = (T_Desc_Ex_Block_Old *)(&ptrs_b->opis_obiektu [0]) ;
	   sprintf (&buf [strlen (buf)] ,
		" flags=%u  len=%d  '%.20s'\n",
		ptrs_desc_bl_old->flags,
		ptrs_desc_bl_old->len, ptrs_desc_bl_old->sz_type);
	 }
       }
  }
  if (buf [0] != '\0')
  {
    fputs (buf , stru) ;
  }
}

int Test_Obiekts_Report (void)
/*--------------------*/
{
  NAGLOWEK *nag ;
  long_long off, ad, offk ;
  FILE *stru ;
  char buf [300] ;

  pisz_zbior ("ALFCHECK.ALF", FALSE, 1) ;
  stru = fopen ( "ALFCHECK.CHK", "wt" ) ;
  if ( stru == NULL )
  {
    return 0;
  }

  sprintf (buf, "\n %s  size = %ld\n", zbior_danych, dane_size) ;
  fputs (buf , stru) ;
  report_tok_param (stru) ;
  test_blok (stru) ;
  off = 0 ;
  offk = dane_size - 1 ;
  fputs ("\nCheck objects\n" , stru) ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag=(NAGLOWEK*)(dane+ad);
    if (FALSE == Check_Object (nag, FALSE))
    {
      report_obiect_param (stru, nag) ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  fclose (stru) ;
  return 0 ;
}

/*--------------------------------------------------*/

#ifdef ALF_TEST

BOOL Test_Object (void * ptr)
/*-------------------------*/
{
  return Check_Object ((NAGLOWEK *)ptr, FALSE) ;
}


FILE *TRACE_STRU ;

void Trace_Object (void *ptr_obiect)
/*---------------------------------*/
{
  //report_obiect_param (TRACE_STRU, (NAGLOWEK *)ptr_obiect) ;  //NA RAZIE WYLACZONO, WYDAJE SIE ZBEDNE
  return;
}


void Open_File_Trace (void)
/*---------------------*/
{

  TRACE_STRU = fopen ( "ALFTRACE.TRC", "wt" ) ;
}

void Close_File_Trace (void)
/*------------------------*/
{
  if (TRACE_STRU != NULL)
  {
    fclose (TRACE_STRU) ;
  }
}

#else

BOOL Test_Object (void *ptr)
/*-------------------------*/
{
  ptr = ptr ;
  return TRUE ;
}

void Open_File_Trace (void)
/*---------------------*/
{
}

void Close_File_Trace (void)
/*------------------------*/
{
}


#endif

/*--------------------------------------------------*/

