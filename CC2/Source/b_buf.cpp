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

#define __BUF__
#pragma warning(disable : 4996)
#include "forwin.h"
#define ALLEGWIN
#include "allegext.h"
#include<stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_buf.h"
#include "o_libfun.h"

#include "message.h"

////#include "leak_detector_cpp.hpp"

#ifdef __cplusplus
extern "C" {

#endif
	long_long Get_memory_size(void);
#ifdef __cplusplus
}
#endif

typedef struct
  {
    char **Master;
    long  Size;
  } TBuffer;


typedef struct TBufHeapTab
{
	int control_flag = 0;
	long_long BufHeapSize = 0;
	long_long BufHeapLen = 0;
	char  * BufHeapStart = NULL;
	char  * BufHeapPtr = NULL;
	char  * BufHeapPtrBase = NULL;
	char  * BufHeapEnd = NULL;
} TBufHeapTab;

static TBufHeapTab BufHeapTab[16];

static int i_test_count = 0 ;

/*--------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif


extern int DRAWING_NUMBER;

extern char  *dane0;
extern void Shift_Global_Ptr (long_long n);

unsigned long_long Get_imageBMPsize(void);
void InitBufPCX(unsigned long buffer_size);
void FreeBufPCX(void);

extern void outtext_r(char *textstring);

#ifdef __cplusplus
}
#endif

static long GetBufSize(TBuffer * P)
{ return P->Size+sizeof(TBuffer); }

static TBuffer *GetBufSpec (char  *P)
/*-------------------------------------*/
{
  TBuffer * ptrs_buffer ;

  ptrs_buffer = (TBuffer*)(P - sizeof(TBuffer)) ;
  if (P < BufHeapTab[DRAWING_NUMBER].BufHeapStart ||
  ptrs_buffer->Size < 0 ||
      P + ptrs_buffer->Size > BufHeapTab[DRAWING_NUMBER].BufHeapEnd)
  {
    Internal_Error (__LINE__, (char*)__FILE__) ;
  }
  return ptrs_buffer ;
}

static void SetBufSize(TBuffer *BufSpec,long NewSize)
{ long Size,nbyte, dk;
  char  *k, *k1, *k2;
  char  *P;
  Size = GetBufSize(BufSpec);
  k=(char  *)BufSpec+NewSize;
  k1=(char  *)BufSpec+Size;
  k2= BufHeapTab[DRAWING_NUMBER].BufHeapStart;
  dk = k1 - k2;
  nbyte= BufHeapTab[DRAWING_NUMBER].BufHeapLen - dk;
  movmem_((void*)k1, (void*)k,nbyte);
  BufHeapTab[DRAWING_NUMBER].BufHeapPtr+=NewSize - Size;
  BufHeapTab[DRAWING_NUMBER].BufHeapLen+=NewSize - Size;
  P = k;
  BufSpec=(TBuffer*)P;
  while( (char *)BufSpec< BufHeapTab[DRAWING_NUMBER].BufHeapPtr )
   { *(BufSpec->Master)+=NewSize-Size;
     P+=GetBufSize(BufSpec);
     BufSpec=(TBuffer*)P;
   }
}

/*--------------------------------------------*/


extern unsigned long _go32_dpmi_remaining_physical_memory(void);

void InitBufPCX (unsigned long buffer_size)
{
  bufor_pcx = (char *) malloc (buffer_size);
}

void FreeBufPCX(void)
{
	free (bufor_pcx);
}

unsigned long_long Get_imageBMPsize(void)
{ unsigned long_long phisical_RAM;
  unsigned long_long new_imageBMPsize;

    phisical_RAM=Get_Free_Physical_Memory();
    new_imageBMPsize =  phisical_RAM - 2000000  ; //z marginesem bezpieczenstwa 2MB

  return new_imageBMPsize;
}


void InitBuffers_factory(long memory_size_)
{ BOOL zaalokowano;


  i_test_count = 0 ;
  unsigned long_long phisical_RAM;
 
  phisical_RAM=Get_Free_Physical_Memory();
  BufHeapTab[DRAWING_NUMBER].BufHeapSize =  phisical_RAM * 0.45  ; //zalecane przez WINDOWS (at least '95), max. 0.5


  if (BufHeapTab[DRAWING_NUMBER].BufHeapSize <= 100000)
  {
	  
	  moveto(5, 5);

	  outtext_r((char*)_NOT_ENOUGH_MEMORY_);

#ifdef LINUX
      sleep(5);
#else
      _sleep(5000);
#endif

	  exit(255);
  }

  /*na razie zablokowano*/
  if (BufHeapTab[DRAWING_NUMBER].BufHeapSize > memory_size_)
  {
	  BufHeapTab[DRAWING_NUMBER].BufHeapSize = memory_size_;
  }
  zaalokowano = FALSE;
  while (zaalokowano == FALSE)
  {
   if (NULL == (BufHeapTab[DRAWING_NUMBER].BufHeapStart = (char *)  malloc (BufHeapTab[DRAWING_NUMBER].BufHeapSize)))
   {
	   BufHeapTab[DRAWING_NUMBER].BufHeapSize *= 0.99 ;
     if (BufHeapTab[DRAWING_NUMBER].BufHeapSize <= 100000)
     {
		 moveto(5, 5);

		 outtext_r((char *)_NOT_ENOUGH_MEMORY_);

#ifdef LINUX
sleep(5);
#else
		_sleep(5000);
#endif
		 exit(255);
     }
   } 
   else zaalokowano=TRUE;
  }



  BufHeapTab[DRAWING_NUMBER].BufHeapLen=0;
  BufHeapTab[DRAWING_NUMBER].BufHeapPtr= BufHeapTab[DRAWING_NUMBER].BufHeapStart;
  BufHeapTab[DRAWING_NUMBER].BufHeapPtrBase = BufHeapTab[DRAWING_NUMBER].BufHeapPtr;
  BufHeapTab[DRAWING_NUMBER].BufHeapEnd= BufHeapTab[DRAWING_NUMBER].BufHeapStart+ BufHeapTab[DRAWING_NUMBER].BufHeapSize;

  //zaalokowanie pamieci dla bufora BMP
  imageBMPsize=(phisical_RAM - BufHeapTab[DRAWING_NUMBER].BufHeapSize) * 0.95;  //z marginesem bezpieczenstwa

 
  return ;
}

void InitBuffers(void)
{
    InitBuffers_factory(memory_size);
}

char *ReInitBuffers(char  **P, long size)
{
    long_long del_dane;
    long_long del_dane0;

    char *BufHeapStart=BufHeapTab[DRAWING_NUMBER].BufHeapStart;

    del_dane0 = BufHeapTab[DRAWING_NUMBER].BufHeapEnd - dane;

    BufHeapTab[DRAWING_NUMBER].BufHeapSize=size*2;
    if (NULL == (BufHeapTab[DRAWING_NUMBER].BufHeapStart = (char *)  realloc (BufHeapTab[DRAWING_NUMBER].BufHeapStart, BufHeapTab[DRAWING_NUMBER].BufHeapSize)))
    {
        return NULL;
    }

    del_dane=BufHeapTab[DRAWING_NUMBER].BufHeapStart-BufHeapStart;


    BufHeapTab[DRAWING_NUMBER].BufHeapPtr +=del_dane;
    BufHeapTab[DRAWING_NUMBER].BufHeapPtrBase += del_dane;
    BufHeapTab[DRAWING_NUMBER].BufHeapEnd = BufHeapTab[DRAWING_NUMBER].BufHeapStart+ BufHeapTab[DRAWING_NUMBER].BufHeapSize;

    dane+=del_dane;
    dane0+=del_dane;

    ((TBuffer*)BufHeapTab[DRAWING_NUMBER].BufHeapPtr)->Master+=del_dane;

    *P += del_dane;

    Shift_Global_Ptr (del_dane);

    return BufHeapTab[DRAWING_NUMBER].BufHeapStart;
}



void DoneBuffers(int dwg_no)
{

	i_test_count = 0;
	if ((BufHeapTab[dwg_no].BufHeapStart!=NULL) && (BufHeapTab[dwg_no].control_flag==7654321))
		free ((void*)BufHeapTab[dwg_no].BufHeapStart);

	BufHeapTab[dwg_no].BufHeapStart = NULL;
	BufHeapTab[dwg_no].control_flag = 0;


}

void DoneBuffMacro___(void)
{
	if (bufor_makra != NULL) free(bufor_makra);
}

char  *NewBuffer(char  **P)
{
  i_test_count++ ;
  if (BufHeapTab[DRAWING_NUMBER].BufHeapPtr >= BufHeapTab[DRAWING_NUMBER].BufHeapEnd - sizeof(TBuffer)) *P=NULL;
  else
   { ((TBuffer*)BufHeapTab[DRAWING_NUMBER].BufHeapPtr)->Master=P;
     ((TBuffer*)BufHeapTab[DRAWING_NUMBER].BufHeapPtr)->Size=0;
	 BufHeapTab[DRAWING_NUMBER].BufHeapPtr+=sizeof(TBuffer);
	 BufHeapTab[DRAWING_NUMBER].BufHeapLen+=sizeof(TBuffer);
     *P= BufHeapTab[DRAWING_NUMBER].BufHeapPtr;

	 BufHeapTab[DRAWING_NUMBER].control_flag = 7654321;

   }
  return *P;
}

void DisposeBuffer (char  *P)
{
  i_test_count-- ;
  SetBufSize (GetBufSpec (P), 0) ;
}

long GetBufferSize(char  * P)
{
  return (GetBufSpec(P))->Size;
}

long_long Get_Buf_Mem_Size (void)
/*-------------------------*/
{
  return BufHeapTab[DRAWING_NUMBER].BufHeapSize ;
}

long_long Get_memory_size(void)
/*-------------------------*/
{
	return memory_size;
}

long_long GetFreeMemSize(void)
{
	long tbuflen;
	tbuflen = sizeof(TBuffer);
	return BufHeapTab[DRAWING_NUMBER].BufHeapSize - BufHeapTab[DRAWING_NUMBER].BufHeapLen - sizeof(TBuffer) - 10; }     /*10 na wszelki wypadek*/

int SetBufferSize(char  *P, long Size)
{ int ret=0;
  TBuffer *BufSpec;
  BufSpec=GetBufSpec(P);
  if(BufHeapTab[DRAWING_NUMBER].BufHeapPtr + Size - BufSpec->Size <= BufHeapTab[DRAWING_NUMBER].BufHeapEnd )
   { SetBufSize(BufSpec,Size+sizeof(TBuffer));
     BufSpec->Size=Size;
     ret=1;
   }
  return ret;
}

/*--------------------------------------------*/

BOOL h_alloc (char  **block, long size)
/*----------------------------------------------*/
{
  if (NULL == NewBuffer (block))
  {
    ErrList (18) ;
    return FALSE ;
  }
  return  SetBufferSize(*block, size) ;
}

BOOL h_alloc_ (char  **block_, long size)
/*----------------------------------------------*/
{
    *block_ = (char*)malloc(size);
    if (*block_==NULL)
    {
        ErrList (18) ;
        return FALSE ;
    }
    return TRUE;
}


BOOL h_realloc (T_PTR_Units ptrs_units, int i_d_alloc)
/*------------------------------------------------------*/
{
  BOOL b_ret ;

  if (ptrs_units->i_size_alloc > ptrs_units->i_size + i_d_alloc)
  {
    return TRUE ;
  }
  if (i_d_alloc == 0)
  {
    i_d_alloc = ptrs_units->i_d_alloc ;
  }
  ptrs_units->i_size_alloc = ptrs_units->i_size + i_d_alloc ;
  b_ret = SetBufferSize((char  *)ptrs_units->ptr_table,
	  ptrs_units->i_size_alloc * ptrs_units->i_unit_size) ;
  if (b_ret == FALSE)
  {
    ErrList (18) ;
  }
  return b_ret ;
}

BOOL h_realloc_ (T_PTR_Units ptrs_units, int i_d_alloc)
/*------------------------------------------------------*/
{
    BOOL b_ret = TRUE ;

    if (ptrs_units->i_size_alloc > ptrs_units->i_size + i_d_alloc)
    {
        return TRUE ;
    }
    if (i_d_alloc == 0)
    {
        i_d_alloc = ptrs_units->i_d_alloc ;
    }
    ptrs_units->i_size_alloc = ptrs_units->i_size + i_d_alloc ;

    ptrs_units->ptr_table=(char*)realloc(ptrs_units->ptr_table, ptrs_units->i_size_alloc * ptrs_units->i_unit_size);
    if (ptrs_units->ptr_table == NULL)
    {
        ErrList (18) ;
        b_ret = FALSE;
    }

    return b_ret ;
}


void h_free (void *block)
/*----------------------*/
{
  if (block == NULL)
  {
    return ;
  }
  DisposeBuffer ((char  *)block) ;
  block = NULL ;
}

void h_free_ (void *block_)
/*----------------------*/
{
    if (block_ == NULL)
    {
        return ;
    }
    free(block_) ;
    block_ = NULL ;
}


#undef __BUF__