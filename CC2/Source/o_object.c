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
#include "b_libfun.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_debug.h"
#include "o_osnap.h"
#include "alffont.h"

extern float pikseleX0f(float jednostki);
extern float pikseleY0f(float jednostki);

extern long czytaj (int f, char  *buf, unsigned long nbyte) ;
extern BOOL Buff_Dan_Realloc (void) ;
extern void transformacja_blok2 (char  *adr,char  *adrk,double x,double y,double k1,double k2,
		double xa, double ya, double ka1, double ka2, int trans, double z) ;
extern void delay(int time);
extern BOOL TTF_redraw;
extern void Set_Bitmap_Outline(BOOL b_pv);

extern void clearBit(unsigned char *n, int k);

void Object_Tok_All1_1 (long_long *, long_long , long_long *, int );

enum TOK_MODE {TM_NO = 0, TM_TOK, TM_TOK1, TM_TOKALL, TM_TOKOFF} ;

static int tok_mode = TM_NO ;

static char  *dane0all;  /*zmienne globalne uzyte do sledzenia*/
static char  *dane01;

char  *dane0;

static char* dane0__;
static BOOL hidden_blocks_visibility=FALSE;
long_long l_tokoff ;

#define MAXBLOCK 655360000L ;

#define LMAX  4294967295 // for 32bit  // 65535L for 16 bit

char *get_dane0(void)
{
    return dane0;
}

void reset_dane0(void)
{
    dane0=dane;
}

char *get_dane01(void)
{
    return dane01;
}

char *get_dane0all(void)
{
    return dane0all;
}

void movmem_(void* src, void* dest, long nbyte)  //??? long  or long_long
{

    ////if (nbyte>100000000)  //TEMPORARY, just for tests
    ////    return;
	memmove(dest, src, nbyte);
}

void movmem____(void *src,void * dest,long nbyte)
{
 long l = LMAX  ;
 long i,k,k1;
 char  *buf1,  *buf2;
 int a;

 if (src == NULL || dest == NULL)
 {
   Internal_Error (__LINE__,__FILE__);
   return;
 }

 
 if (nbyte<0)
 {
     a=0;
	 return;
 }
 if (nbyte>9999999)
 {
    a=0;
 }
 if (nbyte>1000000)
 {
     int possible_error=1;
 }
 
 k=nbyte / l;
 k1=nbyte - k * l;
 buf1=src;
 buf2=dest;

 if ( buf1 < buf2 )
  {
    buf1 += nbyte; buf2 += nbyte;
    for(i=0;i<k;i++)
    {
      buf1 -= l; buf2 -= l;
      memmove((void*)buf2,(void*)buf1,(unsigned)l);
    }
   buf1 -= k1; buf2 -= k1;
   memmove((void*)buf2,(void*)buf1,(unsigned)k1);
  }
 else
  {
    for(i=0;i<k;i++)
     { memmove((void*)buf2,(void*)buf1,(unsigned)l);
       buf1+=l; buf2+=l;
     }
    memmove((void*)buf2,(void*)buf1,(unsigned)k1);
  }
}

static BOOL test_global_ptr (char  *ptrh_test,
			     char  *ptrh_src,
			     long l_move_to,
			     long l_move_len)
/*--------------------------------------------*/
{
  BOOL b_ret ;

  b_ret = FALSE ;
  if (ptrh_test != NULL)
  {
    if (ptrh_test == ptrh_src || 	/*adres wskazuje na poczatek przesuwanego obszaru*/
	(ptrh_test == ptrh_src + l_move_len && l_move_to > 0) ||
	/*adres wskazuje na element zmniejszany*/
	(ptrh_test > ptrh_src && ptrh_test < ptrh_src + l_move_len))
    {
      b_ret = TRUE ;
    }
  }
  return b_ret ;
}

void
Change_Global_Ptr_Move (char  *ptrh_src,
			     long l_move_to,
			     long l_move_len)
/*--------------------------------------------*/
{
  if (TRUE == test_global_ptr (ADP, ptrh_src, l_move_to, l_move_len))
  {
    ADP += l_move_to ;
  }
  if (TRUE == test_global_ptr (ADK, ptrh_src, l_move_to, l_move_len))
  {
    ADK += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMP1, ptrh_src, l_move_to, l_move_len))
  {
    PTR__GTMP1 += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMP2, ptrh_src, l_move_to, l_move_len))
  {
    PTR__GTMP2 += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMP3, ptrh_src, l_move_to, l_move_len))
  {
    PTR__GTMP3 += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMP4, ptrh_src, l_move_to, l_move_len))
  {
    PTR__GTMP4 += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMP5, ptrh_src, l_move_to, l_move_len))
  {
    PTR__GTMP5 += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMP6, ptrh_src, l_move_to, l_move_len))
  {
    PTR__GTMP6 += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMP7, ptrh_src, l_move_to, l_move_len))  //1 extra for plines break
  {
    PTR__GTMP7 += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMPBLOCK, ptrh_src, l_move_to, l_move_len))  //1 extra for plines break
  {
    PTR__GTMPBLOCK += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMPFIRST, ptrh_src, l_move_to, l_move_len))  //1 extra for plines break
  {
    PTR__GTMPFIRST += l_move_to ;
  }
  if (TRUE == test_global_ptr (PTR__GTMPLAST, ptrh_src, l_move_to, l_move_len))  //1 extra for plines break
{
    PTR__GTMPLAST += l_move_to ;
  }
}


int Get_Tok_Mode (void)
/*--------------------*/
{
  return tok_mode ;
}


int korekta_parblok(void  *st,long n)
{
  long lmax = MAXBLOCK ;  /*maksymalna wielkosc bloku 640000*1024 */
  BLOK  *b ;
  long size_blok ;
  long_long off, offk, ad, off_ob, off_endb ;

  off = 0 ;
  offk = dane_size - 1 ;
  off_ob = (char  *)st - dane ;
  Object_Tok_All (&off, offk, &ad, OdBLOK) ;
  while (ad != -1 && ad <= off_ob)
  {
     b = (BLOK*)(dane + ad) ;
     off_endb = ad + sizeof(NAGLOWEK) + b->n - 1 ;
     if (ad <= off_ob && off_endb >= off_ob)
     {
       size_blok = b->n + n ;
       if (size_blok >= lmax)
       {
	 Set_Error_No (ERR_BLOCK_SIZE) ;
	 ErrList (21);
	 return 1;
       }
	   if (size_blok <= 0)
	   {
		   b->n = (B3 + b->dlugosc_opisu_obiektu);
		  // b->atrybut = Ausuniety;
	   }
	   else
	   {
		   b->n = (unsigned)size_blok;
		  // if (b->n == (B3 + b->dlugosc_opisu_obiektu)) 
		  // b->atrybut = Ausuniety;
	   }
     }
     Object_Tok_All (&off, offk, &ad, OdBLOK) ;
   }
  return 0 ;
}

int korekta_parblok_ex(void* st, long l_move, long n)
{
	long lmax = MAXBLOCK;  /*maksymalna wielkosc bloku 640000*1024 */
	BLOK* b;
	long size_blok;
	long_long off, offk, ad, off_ob, off_endb;

	off = 0;
	offk = dane_size - 1;
	off_ob = (char*)st - dane + l_move;
	Object_Tok_All(&off, offk, &ad, OdBLOK);
	while (ad != -1 && ad <= off_ob)
	{
		b = (BLOK*)(dane + ad);
		off_endb = ad + sizeof(NAGLOWEK) + b->n; // -1;  14-06-2022 -1;
		if (ad < off_ob && off_endb >= off_ob)  //<=  .... >=    // 14-06-2022
		{
			size_blok = b->n + n;
			if (size_blok >= lmax)
			{
				Set_Error_No(ERR_BLOCK_SIZE);
				ErrList(21);
				return 1;
			}
			if (size_blok <= 0)
			{
				b->n = (B3 + b->dlugosc_opisu_obiektu);
				// b->atrybut = Ausuniety;
			}
			else
			{
				b->n = (unsigned)size_blok;
				// if (b->n == (B3 + b->dlugosc_opisu_obiektu)) 
			    // b->atrybut = Ausuniety;
			}
		}
		Object_Tok_All(&off, offk, &ad, OdBLOK);
	}
	return 0;
}
/*!!!!!!!!!!! to na razie nie jest potrzebne */
int korekta_parblok1_1(void  *st,long n)
{
  long /*unsigned*/ lmax = 65536000 ;  /*maksymalna wielkosc bloku 64000*1024 */
  BLOK  *b ;
  long size_blok ;
  long_long off, offk, ad, off_ob, off_endb ;

  off = 0 ;
  offk = dane_size - 1 ;
  off_ob = (char  *)st - dane ;
  Object_Tok_All1_1 (&off, offk, &ad, OdBLOK) ;
  while (ad != -1 && ad <= off_ob)
  {
     b = (BLOK1_1*)(dane + ad) ;
     off_endb = ad + sizeof(NAGLOWEK1_1) + b->n - 1 ;
     if (ad <= off_ob && off_endb >= off_ob)
     {
       size_blok = b->n + n ;
       if (size_blok >= lmax)
       {
	 Set_Error_No (ERR_BLOCK_SIZE) ;
	 ErrList (21);
	 return 1;
       }
       b->n = (unsigned)size_blok ;
     }
     Object_Tok_All1_1 (&off, offk, &ad, OdBLOK) ;
   }
  return 0 ;
}


char *dodaj_obiektFD(BLOK *adb, int handle)
{ NAGLOWEK nag;
  char  *st1, *st;
  long ll ;
  long n;
  long poz_akt;

  char *dane00;
  long_long del_dane0;
  dane0 = dane,
  dane00 = dane0;
  
  if (read(handle,&nag,sizeof(NAGLOWEK))!=sizeof(NAGLOWEK))
    {ErrList(45); delay(2000);return NULL;}
    
  n=sizeof(NAGLOWEK)+nag.n;
  ll=dane_size+n;
  if(!SetBufferDaneSize(ll)) { ErrList(18); return NULL;}

  del_dane0=dane0-dane00;
  if (del_dane0!=0)
  {
    ;
  }

  if(dane!=dane0 && adb!=NULL) { st=(char *)adb; st+=dane-dane0; adb=(BLOK*)st;}
  if (adb!=NULL && korekta_parblok((void *)adb,n)) 
  { return NULL;}
  if(adb!=NULL)
   { st1=(char *)adb;
	 st1+=sizeof(NAGLOWEK)+adb->n-n;
	 st=st1-n;
     movmem_((void*)dane,(void*)(dane-n),st1-dane);
     Change_Global_Ptr_Move (dane, -n, st1-dane) ;

   }
  else st=dane-n;
  memmove((void*)st, (void *)&nag, sizeof(NAGLOWEK));
  if (czytaj (handle,st+sizeof(NAGLOWEK),n-sizeof(NAGLOWEK))!=n-sizeof(NAGLOWEK))
     {ErrList(45); return NULL;}
  poz_akt = lseek (handle, 0, SEEK_CUR);
/*  ChangePointers(dane-dane0);*/
  dane-=n;
  dane_size+=n;
  Change = TRUE;
  Set_Auto_Backup (TRUE);
  return (char*)dane;
}


char* Add_Block_Object_older(BLOK** ptrptr_adb, void* ad, long l_move)
/*--------------------------------------------------------------------*/
{
    BLOK* adb = *ptrptr_adb;
    NAGLOWEK* nag;
    char* st1, * dane0 = dane, * st;
    long ll;
    long n;
    BOOL regen_ctx__ = FALSE;
    BLOK* blok_nag;

    nag = (NAGLOWEK*)ad;
    if (nag->obiekt == Opcx) regen_ctx__ = TRUE;

    if (nag->obiekt == OdBLOK)
    {
        blok_nag = (BLOK*)ad;
        if (TRUE != Check_Object(blok_nag, FALSE))
        {
            return NULL;
        }
    }
    else
    {
        if (TRUE != Check_Object(nag, FALSE))
        {
            return NULL;
        }
    }
    n = sizeof(NAGLOWEK) + nag->n;
    ll = dane_size + n;
    if (!SetBufferDaneSize(ll)) { ErrList(18); return NULL; }
    if (dane != dane0 && adb != NULL)
    {
        st = (char*)adb;
        st += dane - dane0;
        adb = (BLOK*)st;
        *ptrptr_adb = adb;
    }
    if (adb != NULL && korekta_parblok((void*)adb, n)) return NULL;
    if (adb != NULL)
    {
        st1 = (char*)adb;
        st1 += l_move;
        movmem_((void*)dane, (void*)(dane - n), (char*)st1 - dane);
        Change_Global_Ptr_Move(dane, -n, st1 - dane);
        st = (char*)adb;
        st -= n;
        adb = (BLOK*)st;
        *ptrptr_adb = adb;
        st = st1 - n;
    }
    else st = dane - n;
    dane -= n;
    memmove((void*)st, (void*)ad, n);
    dane_size += n;
    Change = TRUE;
    if (regen_ctx__ == TRUE) regen_ctx = TRUE;
    Set_Auto_Backup(TRUE);
    return (char*)st;
}


char* Add_Block_Object(BLOK** ptrptr_adb, void* ad, long l_move)
/*--------------------------------------------------------------------*/
{
	BLOK* adb = *ptrptr_adb;
	NAGLOWEK* nag;
	char* st1, * st;
	long ll;
	long n;
	BOOL regen_ctx__ = FALSE;
	BLOK* blok_nag;
    long_long del_dane0;
    char *dane00;

    ////if (l_move>100000000)  //TEMPORARY, just for tests
    ////   return NULL;

    dane0 = dane,
    dane00 = dane0;

	nag = (NAGLOWEK*)ad;
	if (nag->obiekt == Opcx) regen_ctx__ = TRUE;

	if (nag->obiekt == OdBLOK)
	{
        blok_nag = (BLOK*)ad;
		if (TRUE != Check_Object(blok_nag, FALSE))
		{
			return NULL;
		}
	}
	else
	{
		if (TRUE != Check_Object(nag, FALSE))
		{
			return NULL;
		}
	}
	n = sizeof(NAGLOWEK) + nag->n;
	ll = dane_size + n;

    BOOL shift_ad;
    if ((ad>=dane) && (ad<=(dane+dane_size))) shift_ad=TRUE;
    else shift_ad=FALSE;

	if (!SetBufferDaneSize(ll)) { ErrList(18); return NULL; }

    del_dane0=dane0-dane00;
    if (del_dane0!=0)
    {
        if (adb != NULL) adb = (char*)adb + del_dane0;
        if (shift_ad)
            ad = (char*)ad + del_dane0;
    }

	if (dane != dane0 && adb != NULL)
	{
		st = (char*)adb;
		st += dane - dane0;
		adb = (BLOK*)st;
		*ptrptr_adb = adb;
	}
	if (adb != NULL && korekta_parblok((void*)adb, n)) return NULL;
	if (adb != NULL)
	{
		st1 = (char*)adb;
		st1 += l_move;
		movmem_((void*)dane, (void*)(dane - n), (char*)st1 - dane);
		Change_Global_Ptr_Move(dane, -n, st1 - dane);
		st = (char*)adb;
		st -= n;
		adb = (BLOK*)st;
		*ptrptr_adb = adb;
		st = st1 - n;
	}
	else st = dane - n;
	dane -= n;
	memmove((void*)st, (void*)ad, n);
	dane_size += n;
	Change = TRUE;
	if (regen_ctx__ == TRUE) regen_ctx = TRUE;
	Set_Auto_Backup(TRUE);
	return (char*)st;
}


char* Add_Block_Object_Ex(BLOK** ptrptr_adb, void* ad, long l_move)
/*---------------------------------------------------------------*/
{
	BLOK* adb = *ptrptr_adb;
	NAGLOWEK* nag;
	char* st1, * st;
	long ll;
	long n;
	BOOL regen_ctx__ = FALSE;
	//BLOK *blok_nag;
	long del;
    long_long del_dane0;
    char *dane00;

    dane0 = dane,
    dane00 = dane0;

	nag = (NAGLOWEK*)ad;
	if (nag->obiekt == Opcx) regen_ctx__ = TRUE;

	if (nag->obiekt == OdBLOK)
	{
		if (TRUE != Check_Object(nag, FALSE))
		{
			return NULL;
		}
	}
	else
	{
		if (TRUE != Check_Object(nag, FALSE))
		{
			return NULL;
		}
	}
	n = sizeof(NAGLOWEK) + nag->n;
	ll = dane_size + n;
	if (!SetBufferDaneSize(ll)) { ErrList(18); return NULL; }

	BOOL shift_ad;
	if ((ad >= dane) && (ad <= (dane + dane_size))) shift_ad = TRUE;
	else shift_ad = FALSE;

    del_dane0=dane0-dane00;
	if (del_dane0 != 0)
	{
		if (adb != NULL) adb = (char*)adb + del_dane0;
		if (shift_ad)
			ad = (char*)ad + del_dane0;
	}

	if (dane != dane0 && adb != NULL)
	{
		st = (char*)adb;
		st += dane - dane0;
		adb = (BLOK*)st;
		*ptrptr_adb = adb;
	}
	if (adb != NULL && korekta_parblok_ex((void*)adb, l_move, n)) return NULL;
	if (adb != NULL)
	{
		st1 = (char*)adb;
		st1 += l_move;
		del = st1 - dane;
		movmem_((void*)dane, (void*)(dane - n), del);
		Change_Global_Ptr_Move(dane, -n, st1 - dane);
		st = (char*)adb;
		st -= n;
		adb = (BLOK*)st;
		*ptrptr_adb = adb;
		st = st1 - n;

	}
	else st = dane - n;
	dane -= n;
	movmem_((void*)ad, (void*)st, n);
	dane_size += n;
	Change = TRUE;
	if (regen_ctx__ == TRUE) regen_ctx = TRUE;
	Set_Auto_Backup(TRUE);
	return (char*)st;
}

char *Add_Block_Object_Insert (BLOK ** ptrptr_adb, void  *ad, long l_move)
/*----------------------------------------------------------------------*/
{
  BLOK * adb = *ptrptr_adb ;
  NAGLOWEK  *nag;
  char  *st1, *st2, *st;
  long ll;
  long n;
  long_long del_dane0;
  char *dane00;

  dane0 = dane,
  dane00 = dane0;

  nag=(NAGLOWEK*)ad;
  
  
  if (TRUE != Check_Object (nag, FALSE))
  {
    return NULL ;
  }
  n=sizeof(NAGLOWEK)+nag->n;
  ll=dane_size+n;
  if(!SetBufferDaneSize(ll)) { ErrList(18); return NULL;}

  BOOL shift_ad;
  if ((ad >= dane) && (ad <= (dane + dane_size))) shift_ad = TRUE;
  else shift_ad = FALSE;

    del_dane0=dane0-dane00;
	if (del_dane0 != 0)
	{
		if (adb != NULL) adb = (char*)adb + del_dane0;
		if (shift_ad)
			ad = (char*)ad + del_dane0;
	}

  if(dane!=dane0 && adb!=NULL)
  {
    st = (char *)adb ;
    st += dane - dane0 ;
    adb = (BLOK*)st ;
    *ptrptr_adb = adb ;
    
    st2 = (char *)ad ;
    st2 += dane - dane0 ;
    ad = (BLOK*)st2 ;
  }
  if (adb!=NULL && korekta_parblok((void *)adb,n)) return NULL;
  if(adb!=NULL)
   { st1=(char *)adb;
     st1 += l_move ;
     movmem_((void*)dane,(void*)(dane-n),st1-dane);
     Change_Global_Ptr_Move (dane, -n, st1-dane) ;
     st = (char *)adb ;
     st -= n ;
     adb = (BLOK*)st ;
     *ptrptr_adb = adb ;
     st=st1-n;
   }
  else st=dane-n;
  dane-=n;
  movmem_ ((void*)ad, (void*)st, n) ;
    ((BLOK*)st)->blok=1;
  dane_size+=n;
  Change = TRUE;
  Set_Auto_Backup (TRUE);
  return (char *)st;
}


char  *Add_Object (BLOK ** ptrptr_adb, void  *ad)
/*-----------------------------------------------*/
{
  long l_move ;

  char sstt0[32];
  char sstt01[32];

  int a;


  l_move = 0 ;
  
  if (*ptrptr_adb != NULL)
  {
    l_move = sizeof(NAGLOWEK) + (*ptrptr_adb)->n ;
  }
  
  return Add_Block_Object (ptrptr_adb, ad, l_move) ;    //ptrptr_adb  TO CHECK
}

char  *Add_Object_Reversed (BLOK ** ptrptr_adb, void  *ad)
/*-----------------------------------------------*/
{
    long l_move ;

    char sstt0[32];
    char sstt01[32];

    l_move = 0 ;

    if (*ptrptr_adb != NULL)
    {
        l_move = sizeof(NAGLOWEK) + B3 + (*ptrptr_adb)->dlugosc_opisu_obiektu ;
    }

    return Add_Block_Object (ptrptr_adb, ad, l_move) ;    //ptrptr_adb  TO CHECK
}

char  *Add_Object_Insert (BLOK ** ptrptr_adb, void  *ad)
/*-----------------------------------------------*/
{
  long l_move ;
  
  char sstt0[32];
  char sstt01[32];

  l_move = 0 ;
  
  if (*ptrptr_adb != NULL)
  {
    l_move = sizeof(NAGLOWEK) + (*ptrptr_adb)->n ;
  }
  
  return Add_Block_Object_Insert (ptrptr_adb, ad, l_move) ;
}



char  *dodaj_obiekt(BLOK * adb,void  *ad)
/*-----------------------------------------------*/
{
  BLOK * ptr_adb ;

  ptr_adb = adb ;
  
  return Add_Object (&ptr_adb, ad) ;
}

char  *dodaj_obiekt_reversed(BLOK * adb,void  *ad)
/*-----------------------------------------------*/
{
    BLOK * ptr_adb ;

    ptr_adb = adb ;

    return Add_Object_Reversed (&ptr_adb, ad) ;
}


char  *dodaj_obiekt_insert(BLOK * adb,void  *ad)
/*-----------------------------------------------*/
{
  BLOK * ptr_adb ;

  ptr_adb = adb ;
  
  return Add_Object_Insert (&ptr_adb, ad) ;
}


char * Copy_Object (char  *adr)
/*-------------------------------*/
{
  NAGLOWEK *nag;
  void *ad;
  unsigned k;
  long_long offp = adr - dane ;

  if (adr == NULL)
  {
    return NULL ;
  }
  nag = (NAGLOWEK*)(adr) ;
  k = sizeof(NAGLOWEK) + nag->n ;
  if (NULL == (ad = dodaj_obiekt (NULL, (void*)nag)))
  {
    return NULL ;
  }
  if (TRUE == Buff_Dan_Realloc ())
  {
    nag=(NAGLOWEK*)(dane+offp + k) ;
    //memmove ((void*)ad, nag, k) ;
    movmem_(nag, (void*)ad, k);
  }
  if (((NAGLOWEK*)ad)->obiekt==OdBLOK)
  {
      if (((BLOK *) ad)->flag & 2)
          clearBit(&((BLOK *) ad)->flag, 2);
  }

  return ad ;
}


char * Copy_Object_Block (char  *adr)
/*---------------------------------*/
{
  NAGLOWEK *nag;
  void *ad;
  unsigned k;
  long_long offp = adr - dane ;

  if (adr == NULL)
  {
    return NULL ;
  }
  nag = (NAGLOWEK*)(adr) ;
  k = sizeof(NAGLOWEK) + nag->n ;
  if (NULL == (ad = dodaj_obiekt_insert ((BLOK *)dane, (void*)nag)))
  {
    return NULL ;
  }
  if (TRUE == Buff_Dan_Realloc ())
  {
    nag=(NAGLOWEK*)(dane+offp + k) ;
    //memmove ((void*)ad, nag, k) ;
    movmem_(nag, (void*)ad, k);
    //changing block flag
    ((BLOK*)ad)->blok=1;
  }
  return ad ;
}



char * Copy_ObjectB (BLOK ** ptrptr_adb, char  *adr)
/*-----------------------------------------------------*/
{
  NAGLOWEK *nag;
  void *ad;
  unsigned k, ik;
  long_long offp = adr - dane ;
  char  *ptrh_temp ;

  if (adr == NULL)
  {
    return NULL ;
  }
  nag = (NAGLOWEK*)adr ;
  k = sizeof(NAGLOWEK) + nag->n ;
  ik = 1 ;
  if (*ptrptr_adb != NULL)
  {
    ptrh_temp = (char  *)(*ptrptr_adb) ;
    ptrh_temp +=  sizeof(NAGLOWEK) + (*ptrptr_adb)->n ;
    if (ptrh_temp > adr)
    {
      ik = 0 ;
    }
  }
  if (NULL == (ad = Add_Object (ptrptr_adb, (void*)nag)))
  {
    return NULL ;
  }
  if (TRUE == Buff_Dan_Realloc ())
  {
    nag=(NAGLOWEK*)(dane + offp + k * ik) ;
    //memmove ((void*)ad, nag, k) ;
    movmem_(nag, (void*)ad, k);
  }
  return ad ;
}

void obiekt_tok_all (char  *adr, char  *adrk, char  **ad, int obiekt)
{ 	/*wszystkie obiekty z wyjatkiem usunietych lub zlych*/
 int war ;
 NAGLOWEK *nag;
 BLOK  *b;

 tok_mode = TM_TOKALL ;
 if (adr!=NULL) dane0all=adr;
 do
  { nag=(NAGLOWEK*)dane0all;
    *ad=(char*)dane0all;
    if (nag->obiekt==Okoniec || dane0all>adrk )
    {
      tok_mode = TM_NO ;
      *ad = NULL ;
      return ;
    }
    switch(nag->obiekt)
     {
	   case OdBLOK :
	 b=(BLOK *)dane0all;
     if ((obiekt == ONoInBlock) || ((b->flag & 2) && (!hidden_blocks_visibility)))
	 {
	   dane0all += nag->n + sizeof(NAGLOWEK) ;
	 }
	 else
	 {
	   dane0all+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 }
	 break;
       default :
	 dane0all+=nag->n+sizeof(NAGLOWEK);
	 break;
     }
	if (obiekt == ONieOkreslony || obiekt == ONoInBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (nag->atrybut==Ausuniety || nag->atrybut == Abad || war);
 return;
}

void obiekt_tok_all2 (char  *adr, char  *adrk, char  **ad, int obiekt1, int obiekt2)
{ 	/*wszystkie obiekty z wyjatkiem usunietych lub zlych*/
    int war ;
    NAGLOWEK *nag;
    BLOK  *b;

    tok_mode = TM_TOKALL ;
    if (adr!=NULL) dane0all=adr;
    do
    { nag=(NAGLOWEK*)dane0all;
        *ad=(char*)dane0all;
        if (nag->obiekt==Okoniec || dane0all>adrk )
        {
            tok_mode = TM_NO ;
            *ad = NULL ;
            return ;
        }
        switch(nag->obiekt)
        {
            case OdBLOK :
                b=(BLOK *)dane0all;
                if ((obiekt1 == ONoInBlock) || (obiekt2 == ONoInBlock) || ((b->flag & 2) && (!hidden_blocks_visibility)))
                {
                    dane0all += nag->n + sizeof(NAGLOWEK) ;
                }
                else
                {
                    dane0all+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                }
                break;
            default :
                dane0all+=nag->n+sizeof(NAGLOWEK);
                break;
        }
        if (obiekt1 == ONieOkreslony || obiekt1 == ONoInBlock || obiekt2 == ONieOkreslony || obiekt2 == ONieOkreslony)
            war=0;
        else war=((nag->obiekt!=obiekt1) && (nag->obiekt!=obiekt2));
    }
    while (nag->atrybut==Ausuniety || nag->atrybut == Abad || war);
    return;
}


void obiekt_tok_all1_1 (char  *adr, char  *adrk, char  **ad, int obiekt)
{ 	/*wszystkie obiekty z wyjatkiem usunietych lub zlych*/
 int war ;
 NAGLOWEK1_1 *nag;
 BLOK1_1  *b;

 tok_mode = TM_TOKALL ;
 if (adr!=NULL) dane0all=adr;
 do
  { nag=(NAGLOWEK1_1*)dane0all;
    *ad=(char*)dane0all;
    if (nag->obiekt==Okoniec || dane0all>adrk )
    {
      tok_mode = TM_NO ;
      *ad = NULL ;
      return ;
    }
    switch(nag->obiekt)
     {
	   case OdBLOK :
	 b=(BLOK1_1 *)dane0all;
	 if (obiekt == ONoInBlock)
	 {
	   dane0all += nag->n + sizeof(NAGLOWEK1_1) ;
	 }
	 else
	 {
	   dane0all+=sizeof(NAGLOWEK1_1)+B3_1_1+b->dlugosc_opisu_obiektu;
	 }
	 break;
       default :
	 dane0all+=nag->n+sizeof(NAGLOWEK1_1);
	 break;
     }
	if (obiekt == ONieOkreslony || obiekt == ONoInBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (nag->atrybut==Ausuniety || nag->atrybut == Abad || war);
 return;
}



void obiekt_tok12(char  *adr,char  *adrk,char  **ad,int obiekt, BOOL nobad)
{
 int war, layer_off;
 LINIA * line;
 NAGLOWEK *nag;
 BLOK  *b;

 tok_mode = TM_TOK1 ;
if (adr!=NULL) dane01=adr;
 do
  {
	 nag=(NAGLOWEK*)dane01;
    *ad=(char*)dane01;
    if (nag->obiekt==Okoniec || dane01>adrk ) {tok_mode = TM_NO ; *ad=NULL; return; }
    layer_off = 0;
    switch(nag->obiekt)
     {
	   case OdBLOK :
	 b=(BLOK *)dane01;
     if ((obiekt == ONoInBlock) || ((b->flag & 2) && (!hidden_blocks_visibility)))
	 {
	   dane01 += nag->n + sizeof(NAGLOWEK) ;
	 }
	 else
	 {
	   dane01+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 }
	 break;
       default :
	 dane01+=nag->n+sizeof(NAGLOWEK);
	 line = (LINIA*)nag;
	 layer_off = (Layers [line->warstwa].on == 0);
	 break;
     }
	if (obiekt == ONieOkreslony || obiekt == ONoInBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (layer_off || nag->atrybut==Ausuniety || ((nag->atrybut == Abad) & nobad )|| war) ;
 return;
}

void obiekt_tok1(char  *adr,char  *adrk,char  **ad,int obiekt)
{
    obiekt_tok12(adr, adrk, ad, obiekt, TRUE);
}

void obiekt_tok2(char  *adr,char  *adrk,char  **ad,int obiekt)
{
    obiekt_tok12(adr, adrk, ad, obiekt, FALSE);
}

void store_dane0(void)
{
	dane0__ = dane0;
}

void restore_dane0(void)
{
	dane0 = dane0__;
}

void set_global_hidden_blocks_visibility(BOOL block_visibility)
{
    hidden_blocks_visibility=block_visibility;
}

void obiekt_tok(char  *adr,char  *adrk,char  **ad,int obiekt)
{
 int war, layer_off;
 LINIA * line;
 NAGLOWEK *nag;
 BLOK  *b;
 int a;

 if ((adr>((char*)dane+dane_size)) && (adr==adrk))
 {
     nag=(NAGLOWEK*)adr;
     if ((obiekt == ONieOkreslony) || (obiekt == ONoInBlock) || (nag->obiekt=obiekt))
         *ad=(char*)adr;
     else *ad=NULL;
         return;
 }

 tok_mode = TM_TOK ;
 if (adr!=NULL) dane0=adr;
 else
 {
     if (dane0>=dane+dane_size)
     //if (dane0>dane+dane_size)
         dane0=dane;
 }

 if (dane0<dane){
     *ad=NULL;
     return;
 }

  if (dane0>=(dane+dane_size)) {dane0=dane; return;}  ////!!!!!!!!

 do
 {
    nag=(NAGLOWEK*)dane0;

    *ad=(char*)dane0;
    if (nag->obiekt==Okoniec || dane0>adrk ) {tok_mode = TM_NO ; *ad=NULL; return; }
    layer_off = 0;
    switch(nag->obiekt)
	 {
       case OdBLOK :
         b=LASTb=(BLOK *)dane0;  /* nie jest to ostatni blok */
         if ((obiekt == ONoInBlock) || ((b->flag & 2) && (!hidden_blocks_visibility)))
         {
           dane0 += nag->n + sizeof(NAGLOWEK) ;
         }
         else
         {
           dane0+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
         }
         break;
           default :
         dane0+=nag->n+sizeof(NAGLOWEK);
         line = (LINIA*)nag;
         layer_off = (Layers [line->warstwa].on == 0);
	 break;
     }
	if (obiekt == ONieOkreslony || obiekt == ONoInBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (layer_off || nag->atrybut==Ausuniety || nag->atrybut == Abad || war);
 return;
}

void ObiektTok (long_long *off, long_long offk, long_long *ad, int obiekt)
{
 int war, layer_off;
 LINIA * line;
 NAGLOWEK *nag;
 BLOK *b;

 tok_mode = TM_TOKOFF ;
 do
  {
	if (*off > offk) { tok_mode = TM_NO; *ad = -1; return; }
	nag = (NAGLOWEK*)(dane+*off);
    l_tokoff = *ad = *off ;
    if (nag->obiekt==Okoniec || *off>offk ) {tok_mode = TM_NO ; *ad=-1; return; }
    layer_off = 0;
    switch(nag->obiekt)
	 {
       case OdBLOK :
         b=(BLOK*)(dane+*off);  /* nie jest to ostatni blok */
        if ((obiekt == ONoInDimBlock) && (b->kod_obiektu=='W'))
         {
           *off += nag->n + sizeof(NAGLOWEK) ;
         }
        else if ((obiekt == ONoInBlock) || ((b->flag & 2) && (!hidden_blocks_visibility)))
         {
           *off += nag->n + sizeof(NAGLOWEK) ;
         }
         else
         {
           *off+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
         }
	 break;
       default :
	 *off+=nag->n+sizeof(NAGLOWEK);
	 line = (LINIA*)nag;
	 layer_off = (Layers [line->warstwa].on == 0);
	 break;
     }
    if (obiekt == ONieOkreslony || obiekt == ONoInBlock || obiekt == ONoInDimBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (layer_off || nag->atrybut==Ausuniety || nag->atrybut == Abad || war);
 return;
}

void ObiektTok_No_Superblock (long_long *off, long_long offk, long_long *ad, int obiekt)
{
 int war, layer_off;
 LINIA * line;
 NAGLOWEK *nag;
 BLOK *b;
 T_Desc_Ex_Block *ptrs_desc_bl;

 tok_mode = TM_TOKOFF ;
 do
  {
	 if (*off > offk) { tok_mode = TM_NO; *ad = -1; return; }
	 nag = (NAGLOWEK*)(dane+*off);
    l_tokoff = *ad = *off ;
    if (nag->obiekt==Okoniec || *off>offk ) {tok_mode = TM_NO ; *ad=-1; return; }
    layer_off = 0;
    switch(nag->obiekt)
	 {
       case OdBLOK :
       	b=(BLOK*)(dane+*off);  /* nie jest to ostatni blok */
         if ((obiekt == ONoInDimBlock) && (b->kod_obiektu=='W'))
	       {
	        *off += nag->n + sizeof(NAGLOWEK) ;
	       }
         else if ((obiekt == ONoInBlock) || ((b->flag & 2) && (!hidden_blocks_visibility)))
         //else if (obiekt == ONoInBlock)
	       {
	        *off += nag->n + sizeof(NAGLOWEK) ;
	       }
	       else
	        {
            //eliminacja superblokow arkuszy
			 if (b->dlugosc_opisu_obiektu > 17)
			 {
				 ptrs_desc_bl = (T_Desc_Ex_Block*)(&b->opis_obiektu[0]);
				 if (ptrs_desc_bl->sz_type[0] != '\0')
				 {
					 if (strncmp(ptrs_desc_bl->sz_type , "@SHEET", 6) == 0)
					 {
						 *off += sizeof(NAGLOWEK) + b->n;
					 }
					 else *off += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
				 }
				 else *off += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
			 }
			 else *off += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
	        }
	    break;
       default :
	 *off+=nag->n+sizeof(NAGLOWEK);
	 line = (LINIA*)nag;
	 layer_off = (Layers [line->warstwa].on == 0);
	 break;
     }
    if (obiekt == ONieOkreslony || obiekt == ONoInBlock || obiekt == ONoInDimBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (layer_off || nag->atrybut==Ausuniety || nag->atrybut == Abad || war);
 return;
}

void ObiektTok_Layer (long_long *off, long_long offk, long_long *ad, int obiekt, int warstwa)
{
 int war, layer_off;
 LINIA * line;
 NAGLOWEK *nag;
 BLOK *b;

 tok_mode = TM_TOKOFF ;
 do
  {
	 if (*off > offk) { tok_mode = TM_NO; *ad = -1; return; }
	 nag = (NAGLOWEK*)(dane+*off);
    l_tokoff = *ad = *off ;
    if (nag->obiekt==Okoniec || *off>offk ) {tok_mode = TM_NO ; *ad=-1; return; }
    layer_off = 0;
    switch(nag->obiekt)
	 {
       case OdBLOK :
	 b=(BLOK*)(dane+*off);  /* nie jest to ostatni blok */
    if ((obiekt == ONoInDimBlock) && (b->kod_obiektu=='W'))
	 {
	   *off += nag->n + sizeof(NAGLOWEK) ;
	 }
     else if ((obiekt == ONoInBlock) || ((b->flag & 2) && (!hidden_blocks_visibility)))
	 {
	   *off += nag->n + sizeof(NAGLOWEK) ;
	 }
	 else
	 {
	   *off+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 }
	 break;
       default :
	 *off+=nag->n+sizeof(NAGLOWEK);
	 line = (LINIA*)nag;
	 layer_off = (line->warstwa != warstwa);
	 break;
     }
    if (obiekt == ONieOkreslony || obiekt == ONoInBlock || obiekt == ONoInDimBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (layer_off || nag->atrybut==Ausuniety || nag->atrybut == Abad || war);
 return;
}


void Object_Tok_All (long_long *off, long_long offk, long_long *ad, int obiekt)
/*---------------------------------------------------------------------------*/
{	/*wszystkie obiekty lacznie z usunietymi i zlymi*/
 int war ;
 NAGLOWEK *nag;
 BLOK *b;
 
 tok_mode = TM_TOKOFF ;
 do
  {
	 if (*off > offk) { tok_mode = TM_NO; *ad = -1; return; }
	nag = (NAGLOWEK*)(dane+*off);
    l_tokoff = *ad = *off ;
    if (nag->obiekt==Okoniec || *off>offk ) {tok_mode = TM_NO ; *ad=-1; return; }
    switch(nag->obiekt)
	 {
       case OdBLOK :
	 b=(BLOK*)(dane+*off);  /* nie jest to ostatni blok */
	 if (obiekt == ONoInBlock) 
	 {
	   *off += nag->n + sizeof(NAGLOWEK) ;
	 }
	 else
	 {
	   *off+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 }
	 break;
       default :
	 *off+=nag->n+sizeof(NAGLOWEK);
	 break;
     }
    if (obiekt == ONieOkreslony || obiekt == ONoInBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (war) ;
 return;
}

void Object_Tok_All1_1 (long_long *off, long_long offk, long_long *ad, int obiekt)
/*-------------------------------------------------------------*/
{	/*wszystkie obiekty lacznie z usunietymi i zlymi*/
 int war ;
 NAGLOWEK1_1 *nag;
 BLOK1_1 *b;
 
 tok_mode = TM_TOKOFF ;
 do
  {
	 if (*off > offk) { tok_mode = TM_NO; *ad = -1; return; }
	 nag = (NAGLOWEK1_1*)(dane+*off);
    l_tokoff = *ad = *off ;
    if (nag->obiekt==Okoniec || *off>offk ) {tok_mode = TM_NO ; *ad=-1; return; }
    switch(nag->obiekt)
	 {
       case OdBLOK :
	 b=(BLOK1_1*)(dane+*off);  /* nie jest to ostatni blok */
	 if (obiekt == ONoInBlock)
	 {
	   *off += nag->n + sizeof(NAGLOWEK1_1) ;
	 }
	 else
	 {  
	   *off+=sizeof(NAGLOWEK1_1)+B3_1_1+b->dlugosc_opisu_obiektu;
	 }  
	 break;
       default :
	 *off+=nag->n+sizeof(NAGLOWEK1_1);
	 break;
     }
    if (obiekt == ONieOkreslony || obiekt == ONoInBlock) war=0;
    else war=(nag->obiekt!=obiekt);
  }
 while (war) ;
 return;
}

void *Object_Tok (long_long *off, long_long offk, long_long *ad, int obiekt, int i_mode)
/*-----------------------------------------------------------------=========----*/
{
  BOOL b_object, b_layer_off ;
  LINIA * line;
  NAGLOWEK *nag;
  BLOK *b;

  tok_mode = TM_TOKOFF ;
  do
  {
	  if (*off > offk) { tok_mode = TM_NO; *ad = -1; return NULL; }
     nag = (NAGLOWEK*)(dane+*off);
     l_tokoff = *ad = *off ;
     if (nag->obiekt==Okoniec || *off>offk)   //WARNING:  *off>offk
     {
       tok_mode = TM_NO ;
       *ad = OFF_NULL ;
       return NULL ;
     }
     b_layer_off = FALSE ; /*dla bloku*/
     switch(nag->obiekt)
     {
	case OdBLOK :
	  b=(BLOK*)(dane+*off);  /* nie jest to ostatni blok */
	  if (obiekt == ONoInBlock)
	  {
	    *off += nag->n + sizeof(NAGLOWEK) ;
	  }
	  else
	  {
	    *off+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	  }
	  break;
	default :
	  *off += nag->n + sizeof(NAGLOWEK) ;
	  if (TOK_LAYER_ON == i_mode)
	  {
	    line = (LINIA*)nag ;
	    b_layer_off = Layers [line->warstwa].on == 0 ? TRUE : FALSE ;
	  }
	  break ;
      }
     if (obiekt == ONieOkreslony || obiekt == ONoInBlock) b_object = TRUE ;
     else b_object = nag->obiekt == obiekt ? TRUE : FALSE ;
   }
  while ((TOK_LAYER_ON == i_mode && TRUE == b_layer_off) ||
	 (TOK_ALL != i_mode &&
	      (nag->atrybut == Ausuniety || nag->atrybut == Abad)) ||
	 (b_object == FALSE)) ;
  return (void*)nag ;
}


void Usun_Object (void *ad, BOOL b_simple)
{
  NAGLOWEK  *nag;
  char  *st;
  long_long nbyte;
  long n;

 if (ad == NULL)
 {
   return;
 }
 if (FALSE == Test_Object (ad))		/*dla celow testowych*/
 {
   Internal_Error (__LINE__,__FILE__);
   return;
 }
  
  nag=(NAGLOWEK*)ad;
  n=sizeof(NAGLOWEK)+nag->n;
  if (b_simple == FALSE)
  {
    korekta_parblok(ad,-n);
  }
  st=(char *)ad;
  nbyte=st-dane;
  st=dane+n;
  movmem_((void*)dane,(void*)st,(long)nbyte);
  Change_Global_Ptr_Move (dane, n, (long)nbyte) ;
  dane_size-=n;
  dane=st;
  SetBufferDaneSize(dane_size);
  Change = TRUE;
  Set_Auto_Backup (TRUE);

}

void usun_obiekt(void *ad)
/*-----------------------*/
{
   Usun_Object (ad, FALSE);
}

void *Change_Object1_1 (void *ad, void *obiekt, BOOL b_simple)
/*--------------------------------------------------------*/
{
  NAGLOWEK1_1  *nag1;
  NAGLOWEK *nag2;
  char  *st ;
  long_long nbyte,ll;
  long n;
  char  *st_temp;

  dane0=dane;

  if (ad == NULL || obiekt == NULL ||
      FALSE == Test_Object (obiekt))	/*dla celow testowych*/
  {
    Internal_Error (__LINE__,__FILE__);
    return NULL;
  }
  nag1=(NAGLOWEK1_1*)ad;
  nag2=(NAGLOWEK*)obiekt;
  st=(char *)ad;
  n = sizeof(NAGLOWEK) + nag2->n;
  n -= sizeof(NAGLOWEK1_1) + nag1->n;
  ll=dane_size+n;
  nbyte=st-dane;

  if(dane!=dane0)
  {
    st_temp = (char *)ad;
    st_temp += dane - dane0;
    ad = (BLOK*)st_temp;
  }

  st=dane-n;
  movmem_((void *)dane,(void *)st,nbyte);
  Change_Global_Ptr_Move (dane, -n, nbyte) ;
  dane=st;
  dane_size=ll;
  st=dane+nbyte;
  memmove((void*)st,obiekt,sizeof(NAGLOWEK)+nag2->n);

  st=dane+nbyte;
  Change = TRUE;
  Set_Auto_Backup (TRUE);
  return (void*)st;
}


void *Change_Object (void *ad, void *obiekt, BOOL b_simple)
/*--------------------------------------------------------*/
{
  NAGLOWEK  *nag1,*nag2;
  char  *st ;
  long_long nbyte,ll;
  long n;
  char  *st_temp;

  long_long del_dane0;
  char *dane00;

  dane0 = dane,
  dane00 = dane0;

  if (ad == NULL || obiekt == NULL || FALSE == Test_Object (ad) ||
      FALSE == Test_Object (obiekt))	/*dla celow testowych*/
  {
Internal_Error(__LINE__, __FILE__);
return NULL;
  }
  nag1 = (NAGLOWEK*)ad;
  nag2 = (NAGLOWEK*)obiekt;
  st = (char *)ad;
  n = nag2->n;
  n -= nag1->n;
  ll = dane_size + n;
  nbyte = st - dane;
  if (n > 0 && !SetBufferDaneSize(ll)) { ErrList(18); return NULL; }

  BOOL shift_ad;
  if ((ad >= dane) && (ad <= (dane + dane_size))) shift_ad = TRUE;
  else shift_ad = FALSE;

  BOOL shift_obiekt;
  if ((obiekt >= dane) && (obiekt <= (dane + dane_size))) shift_obiekt = TRUE;
  else shift_obiekt = FALSE;

  del_dane0=dane0-dane00;
  if (del_dane0!=0)
  {

	  if (shift_ad)
		  ad = (char*)ad + del_dane0;
	  if (shift_obiekt)
		  obiekt = (char*)obiekt + del_dane0;

      nag1 = (NAGLOWEK*)ad;
      nag2 = (NAGLOWEK*)obiekt;
  }

  if (dane != dane0)
  {
	  st_temp = (char *)ad;
	  st_temp += dane - dane0;
	  ad = (BLOK*)st_temp;
  }
  if (n != 0)
  {
	  if (b_simple == FALSE && korekta_parblok(ad, n)) return NULL;
  }
  st = dane - n;
  movmem_((void *)dane, (void *)st, nbyte);
  Change_Global_Ptr_Move(dane, -n, nbyte);
  dane = st;
  dane_size = ll;
  st = dane + nbyte;
  memmove((void*)st, obiekt, sizeof(NAGLOWEK) + nag2->n);
  if (n < 0 && !SetBufferDaneSize(ll)) { ErrList(18); return NULL; }
  st = dane + nbyte;
  Change = TRUE;
  Set_Auto_Backup(TRUE);
  return (void*)st;
}

void *korekta_obiekt(void *ad, void *obiekt)
/*---------------------------------------------*/
{
	return Change_Object(ad, obiekt, FALSE);
}


unsigned char  *Change_Block_Descript(BLOK *ptr_block, void *ptr_description, int len_descr)
	/*------------------------------------------------------------------------------------*/
{
#ifndef LINUX
	BLOKD *ptr_blockd;
#else
    BLOK *ptr_blockd;
#endif
	char  *st;
	long_long nbyte, ll;
	long_long n;
	char  *st_temp;

    long_long del_dane0;
    char *dane00;

    dane0 = dane,
    dane00 = dane0;


	if (ptr_block == NULL || ptr_description == NULL)
	{
		Internal_Error(__LINE__, __FILE__);
		return NULL;
	}
	n = len_descr - ptr_block->dlugosc_opisu_obiektu;
	ll = dane_size + n;
	st = (char *)ptr_block;
	st += sizeof(NAGLOWEK) + B3; // 3;
	nbyte = st - dane;
	if (n == 0)
	{
		memmove((void*)st, ptr_description, len_descr);
		return NULL;
	}
	if (n > 0 && !SetBufferDaneSize(ll)) { ErrList(18); return ptr_block; }

    del_dane0=dane0-dane00;
    if (del_dane0!=0)
    {
        if (ptr_block != NULL) ptr_block += del_dane0;
    }

	if (dane != dane0)
	{
		st_temp = (char *)ptr_block;
		st_temp += dane - dane0;
		ptr_block = (BLOK*)st_temp;
	}
	if (korekta_parblok(ptr_block, n)) return NULL;
	st = dane - n;
	movmem_((void *)dane, (void *)st, nbyte);
	Change_Global_Ptr_Move(dane, -n, nbyte);
	dane = st;
	dane_size = ll;
	st = dane + nbyte;
	memmove((void*)st, ptr_description, len_descr);
	if (n < 0 && !SetBufferDaneSize(ll)) 
	{ 
		ErrList(18); 
	    return NULL;
	}  /*blad nie powinien miec miejsca*/
	st = dane + nbyte - sizeof(NAGLOWEK) - B3; //3;
#ifndef LINUX
	ptr_blockd = (BLOKD*)st;
#else
    ptr_blockd = (BLOK*)st;
#endif

	ptr_blockd->dlugosc_opisu_obiektu = len_descr;
	Change = TRUE;
	Set_Auto_Backup(TRUE);

	return (unsigned char *)st;
}


void zmien_atrybut(char  *adr, char  *adrk, int aold, int anew)
{
	NAGLOWEK *nag;
	BLOK  *b;

	if (adr == NULL || adrk == NULL)
	{
		return;
	}

	if (adrk < adr)
	{
		return;
	}
 
 nag=(NAGLOWEK*)adr;
 while ((adr<=adrk) && (nag->obiekt!=Okoniec))
 {
     if (TRUE == Check_Attribute (nag->atrybut, aold))
     {
       nag->atrybut=anew;
       if ((nag->obiekt==Opcx) && (anew==Ausuniety)) deleted_pcx=TRUE;
	   if ((nag->obiekt == Owwielokat) && (((WIELOKAT *)nag)->empty_typ==0) && (((WIELOKAT*)nag)->pattern == 1) && (anew == Ausuniety)) deleted_pattern = TRUE;
	   if ((nag->obiekt == Otekst) &&
		   (PTRS__Text_Style[((TEXT*)nag)->czcionka]->type == 2))
		   deleted_TTF = TRUE;
     } 
     
    switch(nag->obiekt)
     { case OdBLOK :
	 b=(BLOK *)adr;
	 adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 break;
       default :
	 adr+=nag->n+sizeof(NAGLOWEK);
	 break;
     }
    nag=(NAGLOWEK*)adr;
  }
}

void *find_hatch_solid_pattern(char* adr, char* adrk)
{
	NAGLOWEK* nag;
	BLOK* b;
	int li;
	char* adrp;

	if (adr == NULL || adrk == NULL)
	{
		return NULL;
	}

	if (adrk < adr)
	{
		return NULL;
	}

	nag = (NAGLOWEK*)adr;
	adrp = adr;

	while ((nag->obiekt != Okoniec) && (adrp < adrk))
	{
		if ((nag->obiekt == Owwielokat) && (((WIELOKAT*)(nag))->empty_typ == 0) && (((WIELOKAT*)(nag))->pattern == 1))
		{
			return adrp;
		}
		switch (nag->obiekt)
		{
		case OdBLOK:
			b = (BLOK*)adrp;
			adrp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
			break;
		default:
			adrp += nag->n + sizeof(NAGLOWEK);
			break;
		}
		nag = (NAGLOWEK*)adrp;
	}

	return NULL;
}

int zmien_atrybut_hatch_outline(char  *adr, char  *adrk, int aold, int anew, int blok)
{
	NAGLOWEK *nag;
	BLOK  *b;
	int li;
	char *adrp;

	li = 0;

	if (adr == NULL || adrk == NULL)
	{
		return 0;
	}

	if (adrk < adr)
	{
		return 0;
	}

	nag = (NAGLOWEK*)adr;
	adrp = adr;

	while ((nag->obiekt != Okoniec) && (adrp < adrk))
	{
		if (TRUE == Check_Attribute(nag->atrybut, aold))
		{
			if ((nag->obiekt == Olinia) && (((LINIA*)(nag))->typ == HATCH_OUTLINE_TYPE))
			{
				if (nag->blok==blok)
				    nag->atrybut = anew; li++;
			}
			else if ((nag->obiekt == Oluk) && (((LUK*)(nag))->typ == HATCH_OUTLINE_TYPE))
			{
				if (nag->blok == blok)
				    nag->atrybut = anew; li++;
			}
			else if ((nag->obiekt == Ookrag) && (((OKRAG*)(nag))->typ == HATCH_OUTLINE_TYPE))
			{
				if (nag->blok == blok)
					nag->atrybut = anew; li++;
			}
		}
		switch (nag->obiekt)
		{
		case OdBLOK:
			b = (BLOK *)adrp;
			adrp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
			break;
		default:
			adrp += nag->n + sizeof(NAGLOWEK);
			break;
		}
		nag = (NAGLOWEK*)adrp;
	}

	return li;
}

void zmien_atrybut_preserve_hatch_outline(char  *adr, char  *adrk, int aold, int anew)
{
	NAGLOWEK *nag;
	BLOK  *b;
	char *adrp;

	if (adr == NULL || adrk == NULL)
	{
		return;
	}

	if (adrk < adr)
	{
		return;
	}

	nag = (NAGLOWEK*)adr;
	adrp = adr;

	while ((nag->obiekt != Okoniec) && (adrp < adrk))
	{
		if (TRUE == Check_Attribute(nag->atrybut, aold))
		{
			if ((nag->obiekt == Olinia) && (((LINIA*)(nag))->typ != HATCH_OUTLINE_TYPE))
			{
				rysuj_obiekt(nag, COPY_PUT, 0);
				nag->atrybut = anew;
			}
			else if ((nag->obiekt == Oluk) && (((LUK*)(nag))->typ != HATCH_OUTLINE_TYPE))
			{
				rysuj_obiekt(nag, COPY_PUT, 0);
				nag->atrybut = anew;
			}
			else if ((nag->obiekt == Ookrag) && (((OKRAG*)(nag))->typ != HATCH_OUTLINE_TYPE))
			{
				rysuj_obiekt(nag, COPY_PUT, 0);
				nag->atrybut = anew;
			}
			else if (nag->obiekt == Owwielokat)
			{
				rysuj_obiekt(nag, COPY_PUT, 0);
				nag->atrybut = anew;
			}
		}
		switch (nag->obiekt)
		{
		case OdBLOK:
			b = (BLOK *)adrp;
			if (b->kod_obiektu != 'H') return;
			adrp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
			break;
		default:
			adrp += nag->n + sizeof(NAGLOWEK);
			break;
		}
		nag = (NAGLOWEK*)adrp;
	}
}

int get_points_hatch_outline(char* adr, char* adrk, float* hatch_points, int *vertex_counts, int* h_layer, int* h_kolor)
{
	NAGLOWEK* nag;
	BLOK* b;
	int li, polygon_no, vcount;
	char* adrp;
	float x1, y1;
	int i;
	float temp_points[4096];

	int a;

	li = 0;
	polygon_no = 0;
	vcount = 0;

	if (adr == NULL || adrk == NULL)
	{
		return 0;
	}

	if (adrk < adr)
	{
		return 0;
	}

	nag = (NAGLOWEK*)adr;
	adrp = adr;

	while ((nag->obiekt != Okoniec) && (adrp < adrk))
	{
			if ((nag->obiekt == Olinia) && (((LINIA*)(nag))->typ == HATCH_OUTLINE_TYPE))
			{
				if (li == 0)
				{
					*h_kolor = (int)((LINIA*)(nag))->kolor;
					*h_layer = (int)((LINIA*)(nag))->warstwa;
				}

				x1 = pikseleX0f(((LINIA*)(nag))->x1);
				y1 = pikseleY0f(((LINIA*)(nag))->y1);

				if (vcount < 2) vcount++;
				else
				{
					if ((x1 == hatch_points[li - 2]) &&
						(y1 == hatch_points[li - 1]))
					{
						//next segment of the current polygon
						vcount++;
						li -= 2;
					}
					else
					{   //finish polygon
						vertex_counts[polygon_no] = vcount;
						//new polygon
						polygon_no++;
						vcount = 1;
					}
				}

				hatch_points[li++] = x1;
				hatch_points[li++] = y1;

				hatch_points[li++] = pikseleX0f(((LINIA*)(nag))->x2);
				hatch_points[li++] = pikseleY0f(((LINIA*)(nag))->y2);
				
			}
			else if ((nag->obiekt == Oluk) && (((LUK*)(nag))->typ == HATCH_OUTLINE_TYPE))
			{
				;
			}
			else if ((nag->obiekt == Ookrag) && (((OKRAG*)(nag))->typ == HATCH_OUTLINE_TYPE))
			{
				;
			}
		
		switch (nag->obiekt)
		{
		case OdBLOK:
			b = (BLOK*)adrp;
			//if (b->kod_obiektu != 'H') return;
			adrp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
			break;
		default:
			adrp += nag->n + sizeof(NAGLOWEK);
			break;
		}
		nag = (NAGLOWEK*)adrp;
	}
	

	//finish polygon
	vertex_counts[polygon_no] = vcount;
	vertex_counts[polygon_no+1] = 0;

	return li;
}

void zmien_atrybut_obiektt2(char  *adr,char  *adrk,int obiektt2,int anew)
{NAGLOWEK *nag;
 BLOK  *b;
 
 if (adr == NULL || adrk == NULL)
 {
   return;
 }
 nag=(NAGLOWEK*)adr;
 while ((nag->obiekt!=Okoniec) && (adr<=adrk))
 {
    switch(nag->obiekt)
     { case OdBLOK :
	     b=(BLOK *)adr;
	     adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	    break;
       default :
        if (nag->obiektt2==obiektt2) nag->atrybut=anew;
	     adr+=nag->n+sizeof(NAGLOWEK);
	    break;
     }
    nag=(NAGLOWEK*)adr;
  }
}

void zmien_atrybut_undo(char  *adr,char  *adrk)
{NAGLOWEK *nag;
 BLOK  *b;
 
 if (adr == NULL || adrk == NULL)
 {
   return;
 }
 nag=(NAGLOWEK*)adr;
 
 if (UNDO_TAB[0].trans!=255)
 {
 while ((nag->obiekt!=Okoniec) && (adr<=adrk))
  {
  if (TRUE == Check_Attribute (nag->atrybut, Aoblok))
   {
     nag->atrybut=Apoblok;
   } 
   else
    if (TRUE == Check_Attribute (nag->atrybut, Apoblok))
     {
       nag->atrybut=Appoblok;
     }
     else
      if (TRUE == Check_Attribute (nag->atrybut, Appoblok)) 
      {
       nag->atrybut=Apppoblok;
      }  
      else
       if (TRUE == Check_Attribute (nag->atrybut, Apppoblok)) 
       {
		nag->atrybut = Anormalny;
       }  
       
    switch(nag->obiekt)
     { case OdBLOK :
	 b=(BLOK *)adr;
	 adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 break;
       default :
	 adr+=nag->n+sizeof(NAGLOWEK);
	 break;
     }
    nag=(NAGLOWEK*)adr;
  }
  
    memmove(&UNDO_TAB[3], &UNDO_TAB[2], sizeof(UNDO_TAB_REC));
    memmove(&UNDO_TAB[2], &UNDO_TAB[1], sizeof(UNDO_TAB_REC));
    memmove(&UNDO_TAB[1], &UNDO_TAB[0], sizeof(UNDO_TAB_REC));
    memmove(&UNDO_TAB[0], &UNDO_REC, sizeof(UNDO_TAB_REC));
 }  
 else
 {
  while (nag->obiekt!=Okoniec && adr<=adrk)
  {
  if (TRUE == Check_Attribute (nag->atrybut, Aoblok))
   {
     nag->atrybut=Anormalny;
   } 
    switch(nag->obiekt)
     { case OdBLOK :
	 b=(BLOK *)adr;
	 adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 break;
       default :
	 adr+=nag->n+sizeof(NAGLOWEK);
	 break;
     }
    nag=(NAGLOWEK*)adr;
  }
    memmove(&UNDO_TAB[0], &UNDO_REC, sizeof(UNDO_TAB_REC));
 } 

}

void update_atrybut_undo(char  *adr, char  *adrk)
{
	NAGLOWEK *nag;
	BLOK  *b;

	if (adr == NULL || adrk == NULL)
	{
		return;
	}
	nag = (NAGLOWEK*)adr;

	if (UNDO_TAB[0].trans != 255)
	{
		while ((nag->obiekt != Okoniec) && (adr <= adrk))
		{
			if (TRUE == Check_Attribute(nag->atrybut, Aoblok))
			{
				nag->atrybut = Apoblok;
				nag->atrybut = Anormalny;
			}
			else
				if (TRUE == Check_Attribute(nag->atrybut, Apoblok))
				{
					nag->atrybut = Appoblok;
					nag->atrybut = Anormalny;
				}
				else
					if (TRUE == Check_Attribute(nag->atrybut, Appoblok))
					{
						nag->atrybut = Apppoblok;
						nag->atrybut = Anormalny;
					}
					else
						if (TRUE == Check_Attribute(nag->atrybut, Apppoblok))
						{
							nag->atrybut = Anormalny;
						}

			switch (nag->obiekt)
			{
			case OdBLOK:
				b = (BLOK *)adr;
				adr += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
				break;
			default:
				adr += nag->n + sizeof(NAGLOWEK);
				break;
			}
			nag = (NAGLOWEK*)adr;
		}

		memmove(&UNDO_TAB[3], &UNDO_TAB[2], sizeof(UNDO_TAB_REC));
		memmove(&UNDO_TAB[2], &UNDO_TAB[1], sizeof(UNDO_TAB_REC));
		memmove(&UNDO_TAB[1], &UNDO_TAB[0], sizeof(UNDO_TAB_REC));
		memmove(&UNDO_TAB[0], &UNDO_REC, sizeof(UNDO_TAB_REC));
	}
	else
	{
		while ((nag->obiekt != Okoniec) && (adr <= adrk))
		{
			if (TRUE == Check_Attribute(nag->atrybut, Aoblok))
			{
				nag->atrybut = Anormalny;
			}
			switch (nag->obiekt)
			{
			case OdBLOK:
				b = (BLOK *)adr;
				adr += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
				break;
			default:
				adr += nag->n + sizeof(NAGLOWEK);
				break;
			}
			nag = (NAGLOWEK*)adr;
		}
		memmove(&UNDO_TAB[0], &UNDO_REC, sizeof(UNDO_TAB_REC));
	}

}



void zmien_obiektt2_warstwe_old(char  *adr,char  *adrk,int o_old,int o_new, int new_layer)
{NAGLOWEK *nag;
 BLOK  *b;
 LINIA *l;
 
 if (adr == NULL || adrk == NULL)
 {
   return;
 }
 nag=(NAGLOWEK*)adr;
 while ((nag->obiekt!=Okoniec) && (adr<=adrk))
 {
   if (nag->obiektt2==o_old)
   {
     nag->obiektt2=o_new;
     switch(nag->obiekt)
     { case OdBLOK :
            break;
            default : l=(LINIA *)adr;
                      l->warstwa=new_layer;
	    break;
     }	 
   }
    switch(nag->obiekt)
     { case OdBLOK : b=(BLOK *)adr;
	                  adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	    break;
       default : adr+=nag->n+sizeof(NAGLOWEK);
	    break;
     }
    nag=(NAGLOWEK*)adr;
  }
}

void zmien_obiektt2_warstwe(char  *adr, char  *adrk, int o_old, int o_new, int new_layer)
{
	NAGLOWEK *nag, *nag1;
	BLOK  *b;
	LINIA *l;
	char *adrb, *adrbk;
	char *adr0;

	adr0 = adr;

	if (adr == NULL || adrk == NULL)
	{
		return;
	}
	nag = (NAGLOWEK*)adr;
	while ((nag->obiekt != Okoniec) && (adr <= adrk))
	{
		switch (nag->obiekt)
		{
		case OdBLOK:
			if (nag->obiektt2 == o_old)
			{
				nag->obiektt2 = o_new;
				adrb = adr + sizeof(NAGLOWEK) + B3 + ((BLOK *)nag)->dlugosc_opisu_obiektu;
				adrbk = adr + nag->n + sizeof(NAGLOWEK);

				nag1 = (NAGLOWEK*)adrb;
				while (adrb < adrbk)
				{
					switch (nag1->obiekt)
					{
					case OdBLOK:
						nag1->obiektt2 = o_new;
						adrb += sizeof(NAGLOWEK) + B3 + ((BLOK *)nag1)->dlugosc_opisu_obiektu;
						break;
					default:
						((LINIA *)nag1)->warstwa = new_layer;
						adrb += nag1->n + sizeof(NAGLOWEK);
						break;
					}
					nag1 = (NAGLOWEK*)adrb;
				}
			}
			break;
		default:
			break;
		}
		adr += nag->n + sizeof(NAGLOWEK);
		nag = (NAGLOWEK*)adr;
	}
}


void usun_bloki_obiektt2(char  *adr, char  *adrk, int o_old)
{
	NAGLOWEK *nag, *nag1;
	BLOK  *b;
	LINIA *l;
	char *adrb, *adrbk;
	char *adr0;

	adr0 = adr;

	if (adr == NULL || adrk == NULL)
	{
		return;
	}
	nag = (NAGLOWEK*)adr;
	while ((nag->obiekt != Okoniec) && (adr <= adrk))
	{
		switch (nag->obiekt)
		{
		case OdBLOK:
			if (nag->obiektt2 == o_old)
			{
				nag->atrybut = Ausuniety;

				adrb = adr + sizeof(NAGLOWEK) + B3 + ((BLOK *)nag)->dlugosc_opisu_obiektu;
				adrbk = adr + nag->n + sizeof(NAGLOWEK);

				nag1 = (NAGLOWEK*)adrb;
				while (adrb < adrbk)
				{
					switch (nag1->obiekt)
					{
					case OdBLOK:
						nag1->atrybut = Ausuniety;
						adrb += sizeof(NAGLOWEK) + B3 + ((BLOK *)nag1)->dlugosc_opisu_obiektu;
						break;
					default:
						nag1->atrybut = Ausuniety;
						adrb += nag1->n + sizeof(NAGLOWEK);
						break;
					}
					nag1 = (NAGLOWEK*)adrb;
				}
			}
			break;
		default:
			break;
		}
		adr += nag->n + sizeof(NAGLOWEK);
		nag = (NAGLOWEK*)adr;
	}
	////usun_blok(adr0, adrk);
}


void zmien_kolor_typ_warstwa(char  *adr,char  *adrk, unsigned char new_color, unsigned char new_type, unsigned char new_layer)
{NAGLOWEK *nag;
 BLOK  *b;
 LINIA *L;
 
 if (adr == NULL || adrk == NULL)
 {
   return;
 }
 nag=(NAGLOWEK*)adr;
 while ((nag->obiekt!=Okoniec) && (adr<=adrk))
 {
    switch(nag->obiekt)
     { case OdBLOK :
	 b=(BLOK *)adr;
	 adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 break;
       default :
         L=(LINIA *)adr;
         L->kolor = new_color;
         L->typ = new_type;
         L->warstwa = new_layer;    
	 adr+=nag->n+sizeof(NAGLOWEK);
	 break;
     }
    nag=(NAGLOWEK*)adr;
  }
  return;
}

void zmien_kolor_dla_obiektt2(char  *adr,char  *adrk, unsigned char obiektt2, unsigned char new_color)
{NAGLOWEK *nag;
 BLOK  *b;
 LINIA *L;
 
 if (adr == NULL || adrk == NULL)
 {
   return;
 }
 nag=(NAGLOWEK*)adr;
 while ((nag->obiekt!=Okoniec) && (adr<=adrk))
 {
    switch(nag->obiekt)
     { case OdBLOK :
	 b=(BLOK *)adr;
	 adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 break;
       default :
         L=(LINIA *)adr;
         if (L->obiektt2==obiektt2)
          {
           L->kolor = new_color;
          }
	 adr+=nag->n+sizeof(NAGLOWEK);
	 break;
     }
    nag=(NAGLOWEK*)adr;
  }
  return;
}

void zmien_kolor_typ_warstwe_obiektt2(char  *adr,char  *adrk, unsigned char new_color,
                                    unsigned char new_typ, unsigned char new_warstwa, unsigned char obiektt2)
{NAGLOWEK *nag;
 BLOK  *b;
 LINIA *L;
 
 if (adr == NULL || adrk == NULL)
 {
   return;
 }
 nag=(NAGLOWEK*)adr;
 while ((nag->obiekt!=Okoniec) && (adr<=adrk))
 {
    switch(nag->obiekt)
     { case OdBLOK :
   	   b=(BLOK *)adr;
         b->obiektt2=obiektt2;
	      adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 break;
       default :
         L=(LINIA *)adr;
         L->kolor = new_color;
         L->typ = new_typ;
         L->warstwa = new_warstwa;
         L->obiektt2 = obiektt2;
   	   adr+=L->n+sizeof(NAGLOWEK);
	    break;
     }
    nag=(NAGLOWEK*)adr;
  }
  return;
}



void usun_blok (char  *adr, char  *adrk)
/*---------------------------------------------*/
{
  NAGLOWEK *nag,*nag1;
  BLOK  *b;
  long_long dist,dist1,dist_temp,add_n;
  BOOL b_info, deleted ;

  if (adr == NULL || adrk == NULL)
  {
   // Internal_Error (__LINE__,__FILE__);
    return;
  }
  nag=(NAGLOWEK*)adr;

  b_info = FALSE ;
  
#define BLSIZE 5000000
  if ((adrk - adr) > BLSIZE)
  {
    b_info = TRUE ;
  }
  if (b_info == TRUE)  //21-06-2021
  {
    InfoList(2);
  }
  while ((adr<=adrk) && (nag->obiekt!=Okoniec))
  {
     if ((nag->atrybut == Ausuniety || nag->atrybut == Abad) &&
	TRUE == Check_Next_Object ((void*)nag))
    {
       /*tutaj nalezy sprawdzic czy aby kolejny obiekt tez nie jest
        Ausunietuy lub Abad oraz ze nie jest blokiem (na razie).
        Jezeli tak, to nalezy zrobic tak:
        1. sumowac w zmiennej add_n kolejno nag1->n + size(NAGLOWEK)
        2. na koniec zmienic nag->n += add_n */
       /********/ 
       deleted=TRUE;
       add_n=0;
       dist_temp= (long_long) (adr + nag->n + sizeof(NAGLOWEK));
       while ((dist_temp<=adrk) && (deleted==TRUE))
       {
        nag1=(NAGLOWEK*)dist_temp; 
        if (nag1->obiekt==Okoniec) deleted=FALSE;
         else
           {
            if ((nag1->atrybut == Ausuniety || nag1->atrybut == Abad)
             && TRUE == Check_Next_Object ((void*)nag1)) 
	    {
	     add_n+=(nag1->n+sizeof(NAGLOWEK));
	     dist_temp+=nag1->n+sizeof(NAGLOWEK);
            }
             else deleted=FALSE;
           }  
       }
       if (add_n>0)
        {
         nag->n+=add_n;
        }  
       /********/
       dist=adr-dane;
       dist1=(adrk-dane) - ( nag->n + sizeof(NAGLOWEK) ) ;
       Usun_Object ((char *)adr,   /*badanie nag->blok dla rysunkow blednych wersji*/
	  ((nag->atrybut == Ausuniety && nag->blok == NoElemBlok) ? TRUE : FALSE));
       adr=dane+dist;
       adrk=dane+dist1;
    }
    else
    {
      switch(nag->obiekt)
      {
	case OdBLOK :
	  b=(BLOK *)adr;
	
	  adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	  break;
	default :
	  adr+=nag->n+sizeof(NAGLOWEK);
	  break;
      }
    }
    nag=(NAGLOWEK*)adr;
  }
  if (b_info == TRUE)   //21-06-2021
  {
    ClearInfo () ;
  }
}



void usun_blok1 (char  *adr, char  *adrk)
/*---------------------------------------------*/
{
  NAGLOWEK *nag,*nag1;
  BLOK  *b;
  long_long dist,dist1,dist_temp,add_n;
  BOOL b_info, deleted ;

  if (adr == NULL || adrk == NULL)
  {
   // Internal_Error (__LINE__,__FILE__);
    return;
  }
  nag=(NAGLOWEK*)adr;
  b_info = TRUE ;
  
#define BLSIZE 5000
  if (adrk - adr < BLSIZE)
  {
    b_info = FALSE ;
  }
  if (b_info == TRUE)
  {
    InfoList(2);
  }
  while ((nag->obiekt!=Okoniec) && (adr<=adrk))
  {
   if ((nag->atrybut == Ausuniety || nag->atrybut == Abad) )//&&
	//TRUE == Check_Next_Object ((void*)nag))
    {
       /*tutaj nalezy sprawdzic czy aby kolejny obiekt tez nie jest
        Ausunietuy lub Abad oraz ze nie jest blokiem (na razie).
        Jezeli tak, to nalezy zrobic tak:
        1. sumowac w zmiennej add_n kolejno nag1->n + size(NAGLOWEK)
        2. na koniec zmienic nag->n += add_n */
       /********/
    if ((nag->atrybut==Abad) && (nag->n>=68157440))
     {
      // obiekt_tok(NULL,adrk,(char **) &nag,ONieOkreslony);
      // adr=nag;

       switch(nag->obiekt)
        {
	      case OdBLOK :
    	    b=(BLOK *)adr;
	       adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
   	   break;
	      default :
          adr+=sizeof(NAGLOWEK);
          nag=(NAGLOWEK*)adr;
	       //adr+=29/*+sizeof(NAGLOWEK)*/;
	      break;
        }
     }
      else
      {
       deleted=TRUE;
       add_n=0;
       dist_temp= (long_long) (adr + nag->n + sizeof(NAGLOWEK));
       while ((dist_temp<=adrk) && (deleted==TRUE))
       {
        nag1=(NAGLOWEK*)dist_temp; 
        if (nag1->obiekt==Okoniec) deleted=FALSE;
         else
           {
            if ((nag1->atrybut == Ausuniety || nag1->atrybut == Abad) )
	    {
        if ((nag1->atrybut==Abad) && (nag1->n>=68157440))
        {
        switch(nag->obiekt)
        {
	      case OdBLOK :
    	    b=(BLOK *)adr;
	       add_n+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
          dist_temp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
   	   break;
	      default :
          add_n+=sizeof(NAGLOWEK);
          dist_temp+=sizeof(NAGLOWEK);
	      break;
        }
       }
        else
         {
	       add_n+=(nag1->n+sizeof(NAGLOWEK));
	       dist_temp+=nag1->n+sizeof(NAGLOWEK);
         }
            }
             else deleted=FALSE;
           }  
       }
       if (add_n>0)
        {
         nag->n+=add_n;
        }  
       /********/
       dist=adr-dane;
       dist1=(adrk-dane) - ( nag->n + sizeof(NAGLOWEK) ) ;
       Usun_Object ((char *)adr,   /*badanie nag->blok dla rysunkow blednych wersji*/
	  ((nag->atrybut == Ausuniety && nag->blok == NoElemBlok) ? TRUE : FALSE));
       adr=dane+dist;
       adrk=dane+dist1;
      }
    }
    else
    {
      switch(nag->obiekt)
      {
	case OdBLOK :
	  b=(BLOK *)adr;
	  adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	  break;
	default :
	  adr+=nag->n+sizeof(NAGLOWEK);
	  break;
      }
    }
    nag=(NAGLOWEK*)adr;
  }
  if (b_info == TRUE)
  {
    ClearInfo () ;
  }
}

int dodaj_blok (char  *adr,char  *adrk,double x,double y,double k1,double k2,int trans)
{
  adr = adr ;    /*usunac, uzyte tylko w bib_icf*/
  adrk = adrk ;
  x = x ;
  y = y ;
  k1 = k1 ;
  k2 = k2 ;
  trans = trans ;
  return 0 ;
}

BOOL Add_Block (char  *adr,char  *adrk, long *l_block_size,
		double x,  double y,  double k1,  double k2,
		double xa, double ya, double ka1, double ka2, int trans)
/*---------------------------------------------------------------------*/
{
  NAGLOWEK *nag;
  BLOK  *b;
  void *ad;
  char  *adp,  *adk;
  BOOL  b_ret = TRUE ;
  unsigned int k;
  long l = 0;
  long_long offp0=adr-dane, offk0=adrk-dane, offp=offp0, offk=offk0;

  if (adr == NULL || adrk == NULL)
  {
    *l_block_size = 0 ;
    return 0;
  }
  nag=(NAGLOWEK*)(dane+offp);
  while (nag->obiekt != Okoniec && offp <= offk)
   { if (nag->atrybut == Ablok)
     {
	k = sizeof(NAGLOWEK) + nag->n ;
    if (nag->obiekt == Opcx)  TTF_redraw = TRUE;
	if (NULL == (ad = dodaj_obiekt (NULL, (void*)nag)))
	{
	  b_ret = FALSE ;
	  goto aa ;
	}
	if (TRUE == Buff_Dan_Realloc ())
	{
	  nag=(NAGLOWEK*)(dane+offp + k) ;
	  //memmove ((void*)ad, nag, k) ;
      movmem_(nag, (void*)ad, k);
	}
	offp += 2 * k ;
	offk += k ;
	l += k ;
     }
     else
      {	if (nag->obiekt==OdBLOK)
	 { b=(BLOK *)nag;
	   offp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 }
	else offp+=nag->n+sizeof(NAGLOWEK);
      }
     nag=(NAGLOWEK*)(dane+offp);
   }
aa:
  adp=dane;
  adk=adp+l-1;
  ADP=dane+offp0+l;
  ADK=dane+offk0+l;
  l__Off_Bl_Beg = (long_long)(ADP - dane ) ;
  l__Off_Bl_End = (long_long)(ADK - dane ) ;
  transformacja_blok2 (adp, adk, x, y, k1, k2, xa, ya, ka1, ka2, trans, 0) ;
  blokzap(adp,adk,Ablok,COPY_PUT,1);
  zmien_atrybut(adp,adk,Ablok,Anormalny);
  *l_block_size = l ;
  return b_ret ;
}

BOOL Add_Block_DXF (char  *adr,char  *adrk, long *l_block_size,
		double x,  double y,  double k1,  double k2,
		double xa, double ya, double ka1, double ka2, int trans,
		int to_block)
/*--------------------------------------------------------------------*/
{
  NAGLOWEK *nag;
  BLOK  *b;
  void *ad;
  char  *adp,  *adk;
  BOOL  b_ret = TRUE ;
  unsigned k;
  long l = 0;
  long_long offp0=adr-dane, offk0=adrk-dane, offp=offp0, offk=offk0;

  if (adr == NULL || adrk == NULL)
  {
    *l_block_size = 0 ;
    return 0;
  }
  nag=(NAGLOWEK*)(dane+offp);
  while (nag->obiekt != Okoniec && offp <= offk)
   { if (nag->atrybut == Ablok)
     {
	k = sizeof(NAGLOWEK) + nag->n ;
	if (to_block == 0)
	{
 	 if (NULL == (ad = dodaj_obiekt (NULL, (void*)nag)))
	 {
	   b_ret = FALSE ;
	   goto aa ;
	 }
	} 
	 else
	{ 
	 if (NULL == (ad = dodaj_obiekt_insert ((BLOK *)dane, (void*)nag)))
         { 
          b_ret = FALSE ;
          goto aa ;
         }
        } 
	
	if (TRUE == Buff_Dan_Realloc ())
	{
	  nag=(NAGLOWEK*)(dane+offp + k) ;
	  //memmove ((void*)ad, nag, k) ;
      movmem_(nag, (void*)ad, k);
	}
	offp += 2 * k ;
	offk += k ;
	l += k ;
     }
     else
      {	if (nag->obiekt==OdBLOK)
	 { b=(BLOK *)nag;
	   offp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 }
	else offp+=nag->n+sizeof(NAGLOWEK);
      }   
     nag=(NAGLOWEK*)(dane+offp);
   }
aa:
  adp=dane;
  adk=adp+l-1;
  ADP=dane+offp0+l;
  ADK=dane+offk0+l;
  l__Off_Bl_Beg = (long_long)(ADP - dane ) ;
  l__Off_Bl_End = (long_long)(ADK - dane ) ;
  transformacja_blok2 (adp, adk, x, y, k1, k2, xa, ya, ka1, ka2, trans, 0) ;
  zmien_atrybut(adp,adk,Ablok,Anormalny);
  *l_block_size = l ;
  return b_ret ;
}

void undo_blok(void)
{ NAGLOWEK *nag;
  BLOK  *b;
  char  *adr,  *adrk;
  BOOL undeleted_pcx=FALSE;
  BOOL undeleted_TTF = FALSE;
  BOOL undeleted_pattern = FALSE;

  adr=dane;
  adrk=adr+dane_size;
  nag=(NAGLOWEK*)adr;
  CUR_OFF(X,Y);
  
  while (nag->obiekt!=Okoniec && adr<=adrk)
   { if (nag->atrybut==Ausuniety)
      { nag->atrybut=Anormalny;
	rysuj_obiekt((char*)adr,COPY_PUT,1);
	if (nag->obiekt==Opcx)  undeleted_pcx=TRUE;
	if ((nag->obiekt == Owwielokat) && (((WIELOKAT*)nag)->empty_typ == 0) && (((WIELOKAT*)nag)->pattern == 1)) undeleted_pattern = TRUE;
	if ((nag->obiekt == Otekst) &&
		(PTRS__Text_Style[((TEXT*)nag)->czcionka]->type == 2))
		undeleted_TTF = TRUE;
	
	Change = TRUE;

	Set_Auto_Backup (TRUE);
      }
     switch(nag->obiekt)
      { case OdBLOK :
	  b=(BLOK *)adr;
	  adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	  break;
	default :
	  adr+=nag->n+sizeof(NAGLOWEK);
	  break;
      }
     nag=(NAGLOWEK*)adr;
   }

  CUR_OFF(X,Y);
  CUR_ON(X,Y);
  if ((undeleted_pcx==TRUE) || (undeleted_TTF == TRUE) || (undeleted_pattern == TRUE))
    {
	  if (undeleted_pcx == TRUE)
	  {
		  regen_ctx = TRUE;
		  Set_Bitmap_Outline(FALSE);
	  }	 
      redraw();
    }
}

int undo_blok_F6 (void)
//---------------------
{
 undo_blok();
 return 0;
}

void Global_Undo(void)
{ NAGLOWEK *nag;
  BLOK  *b;
  char  *adr,  *adrk;
  
  TTF_redraw = FALSE;
  adr=dane;
  adrk=adr+dane_size;

  zmien_atrybut(adr, adrk, Aoblok, Ablok);  
  blokzap(adr,adrk,Ablok,COPY_PUT,0);

  if ((UNDO_TAB[0].trans!=0) && (UNDO_TAB[0].trans!=255))
  {
    transformacja_blok2(adr, adrk, UNDO_TAB[0].x, UNDO_TAB[0].y, UNDO_TAB[0].k1, UNDO_TAB[0].k2, 
                                 UNDO_TAB[0].xa, UNDO_TAB[0].ya, UNDO_TAB[0].ka1, UNDO_TAB[0].ka2, UNDO_TAB[0].trans, 0);
  }        
                           
  blokzap(adr,adrk,Ablok,COPY_PUT,1);
  
  Change = TRUE;
  Set_Auto_Backup (TRUE);                               
  
  nag=(NAGLOWEK*)adr;
  
  memmove(&UNDO_TAB[0], &UNDO_TAB[1], sizeof(UNDO_TAB_REC));
  memmove(&UNDO_TAB[1], &UNDO_TAB[2], sizeof(UNDO_TAB_REC));
  memmove(&UNDO_TAB[2], &UNDO_TAB[3], sizeof(UNDO_TAB_REC));
  memmove(&UNDO_TAB[3], &UNDO_REC_NULL, sizeof(UNDO_TAB_REC));
                        
  while (nag->obiekt!=Okoniec && adr<=adrk)
   { if (nag->atrybut==Apppoblok)
      { nag->atrybut= Appoblok;}
      else
      if (nag->atrybut==Appoblok)
      { nag->atrybut=Apoblok;}
      else
      if (nag->atrybut==Apoblok)
      { nag->atrybut=Aoblok;}
      else
      if (nag->atrybut==Ablok)
      { nag->atrybut=Anormalny;} 
      
     switch(nag->obiekt)
      { case OdBLOK :
	  b=(BLOK *)adr;
	  adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	  break;
	default :
	  adr+=nag->n+sizeof(NAGLOWEK);
	  break;
      }
     nag=(NAGLOWEK*)adr;
   }
  CUR_OFF(X,Y);
  CUR_ON(X,Y); 
  if (TTF_redraw) redraw();
}



