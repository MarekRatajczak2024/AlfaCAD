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

#define __O_MAK__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#include<string.h>
#ifndef LINUX
#include<dos.h>
#include<io.h>
#endif
#include<stdio.h>

#include<fcntl.h>
#include<sys/stat.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_loadf.h"

#include "menu.h"

extern void delay(int time);
extern BOOL Semaphore;
extern BOOL Cust_Semaphore;
extern long filelength(int f);
extern void setfillstyle_(int pattern, int color);
extern void setcolor(int kolor);

static char *mkbuf_end=NULL;
static int Demo=0;
static unsigned long buf_mak_size = 100 ;
static int nooop1(void)
{ return 0;}


static void WykonanieMakra(void);

static TMENU mMakro={4,0,0,25,1,3,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmMakro,NULL,NULL};


static BOOL read_makro (char *file_name)
/*-------------------------------------*/
{
  long roz;
  int f;
  BOOL ret ;

  ret = FALSE ;
  f=open(file_name,O_RDWR|O_BINARY);
  if (f==-1)
   {
	ErrList(4);
	return FALSE ;
   }
  roz=filelength(f);
  if (roz >= buf_mak_size )
   {
	ErrList(6);
	close(f);
	return FALSE ;
   }
  read(f, bufor_makra, (unsigned)roz);
  close(f);
  if (roz > 0)
   {
	  mkbuf_end=bufor_makra + (unsigned)roz;
	  ret = TRUE ;
   }
   return ret ;
}


void brkmakro(void)
/*-----------------*/
{ aktmakro=NULL;
  Demo=0;
}

static int zakonczenie_inicjacji_makra(void)
/*------------------------------------------*/
{ mkbuf_end=mkbuf;
  mkbuf=NULL;
  SERV[117]=nooop1;

  komunikat0(0);

  return 0;
}

void test_aktmakro(void)
/*-------------------------*/
{ if (aktmakro==mkbuf_end)
   if (Demo==1) aktmakro=bufor_makra;
   else
   {
       aktmakro=NULL;LASTFUN=WykonanieMakra;
   }
}

void makro_esc(void)
/*--------------------*/
{

  return;

  bufor_makra[0]= ENTER;
  bufor_makra[1]=0;
  mkbuf_end=bufor_makra + 1;
  aktmakro=bufor_makra;

  return;

	bufor_makra_esc[0] = (char*)'\15';
	bufor_makra_esc[1] = '\0';
	mkbuf = bufor_makra_esc;
	mkbuf_end = bufor_makra_esc + 1;
	aktmakro = bufor_makra_esc;
}

void makro_wine(void)
{

	bufor_makra[0] = _DRAW__;
	bufor_makra[1] = _TEXT__;
	bufor_makra[2] = 'A';
	bufor_makra[3] = 'l';
	bufor_makra[4] = 'f';
	bufor_makra[5] = 'a';
	bufor_makra[6] = 'C';
	bufor_makra[7] = 'A';
	bufor_makra[8] = 'D';
	bufor_makra[9] = 0x0D;
	bufor_makra[10] = 0x1B;
	bufor_makra[11] = 0x1B;
	bufor_makra[12] = 0x00;
	bufor_makra[13] = 0x75;
	mkbuf_end = bufor_makra + 14;
	aktmakro = bufor_makra;
}



static void WykonanieMakra(void)
/*---------------------------------*/
{ Demo=0;if (mkbuf_end!=NULL) aktmakro=bufor_makra;
}

void show_macro(void)
/*----------------------------*/
{
	int x, y;
	char buf[MaxTextLen];
	int nn;

	x = maxX/2;
	y = 2;
	moveto(x, y+1);
	setfillstyle_(SOLID_FILL, kolory.paperk);
	bar(x, y, x + 7 * WIDTH + 2, y + ED_INF_HEIGHT - 3);
	setcolor(BKCOLOR);
	strcpy(buf, _MACRO_);

	outtext_r(buf);
	return;
}

static void InicjacjaMakra(void)
/*---------------------------*/
{
  mkbuf=bufor_makra;
  mkbuf_end=NULL;
  SERV[117]=zakonczenie_inicjacji_makra;
  show_macro();
}

static void ReadMakro(void)
/*-------------------------*/
{ char sk[MAXPATH]="";
  strcpy (sk, "");
  if (Load_File (sk, READ_MAK, TRUE) == FALSE)
  {
     return;
  }
  read_makro (sk);
}


static void WriteMakro(void)
/*-------------------------*/
{ int f;
  char sk[MAXPATH]="";
  
  if (mkbuf_end == NULL)
  {
	return;
  }
  
  strcpy (sk, "");
  if (Load_File (sk, WRITE_MAK, TRUE) == FALSE)
  {
     return;
  }

#ifndef LINUX
    f=open(sk, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    f=open(sk, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
  if (f!=-1)
   {
	 write(f, bufor_makra, (mkbuf_end - bufor_makra));
	 close(f);
   }
}


int delay_1(void)
{
  if (aktmakro != NULL)
   delay(1000);
  return 0;
}

int delay_2(void)
{
  if (aktmakro != NULL)
   delay(2000);
   return 0;
}

int delay_3(void)
{
  if (aktmakro != NULL)
   delay(3000);
   return 0;
}

unsigned Get_Buf_Mak_Size (void)
/*------------------------------*/
{
  return buf_mak_size;
}

void Set_Buf_Mak_Size (unsigned long size)
/*---------------------------------------*/
{
  if (size > 1024000L) size = 1024000L ;
  if (size < 100) size = 100 ;
  {
     buf_mak_size = size ;
  }
}

void Set_Memory_Size (unsigned long size)
/*-------------------------------------*/
{
  if (size > 2048000000L) size = 2048000000L;
  if (size < 1024000) size = 1024000 ;
  {
     memory_size = size ;
  }
}

void Set_Buf_PCX (unsigned long size)
/*--------------------------------------*/
{
  if (size < 1024000L) size = 1024000L;
  if (size > 65530000L) size = 65530000L;
  {
     Bufor_PCX = size ;
  }
}


static void (*COMND[])(void)= {  InicjacjaMakra, WykonanieMakra, ReadMakro, WriteMakro };

void Makro(void)
{ int n;
 Semaphore = FALSE;
 if((n=getwsp1(&mMakro)-1)>=0)
 {
     //LASTFUN=NULL;
     (*COMND[n])();    /* n=-1 -> esc */
     LASTFUN = COMND[n];
 }
}

#undef __O_MAK__