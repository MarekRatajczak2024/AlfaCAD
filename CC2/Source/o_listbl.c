
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

#define __O_LISTBL__
#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bib_e.h"
#include "b_menu.h"
#include "rysuj_e.h"

#include "menu.h"

#include "leak_detector_c.h"

extern unsigned Get_Menu_Image_Size (TMENU *) ;
extern char * Copy_Object (char  *) ;
extern char * Copy_Object_Block (char *) ;
extern void Place_Import_Block (int) ;
extern void obiekt_tok_all (char  *, char  *, char  **, int) ;
extern BOOL set_insert_point (double , double , void *);
extern BOOL set_block_flag ( void *) ;
extern BOOL change_block_name (void *, BOOL, double);
extern BOOL check_if_obiekt (char  *adp, char  *adk, int atrybut, int obiekt) ;
extern void change_colors (char  *adr,char  *adrk, int kolor_elementu);
extern void change_colors_dxf (char  *adr,char  *adrk, int kolor_elementu);
extern BOOL global_no_pcx;
extern int getBlockFromDialog(TMENU *mBlockList, long *my_ptr__off_block);

#define MaxLen 120
#define MaxTextLength 60

static long *ptr__off_block;
static BLOK * ptrs__sel_blok ;

static POLE *pmList_Blok ;
static POLE pm_tmp [] = { {u8"",' ',0,NULL}, } ;

int blocks_dialog=1;

#if defined (MENU_B)
#define MaxMenuWin 10
static TMENU mList_Blok = {0, 0,0, MaxTextLength , 1, 3,0, CMNU,CMBR,CMTX,0,0,0,0,0,&pmList_Blok,NULL,NULL};
#else
#define MaxMenuWin 32
static TMENU mList_Blok = {0, 0, 0,MaxTextLength , 1, 3,0, CMNU,CMBR,CMTX,0,0,0, 0,0,&pm_tmp, NULL,NULL};
#endif


void change_obiektt3_dxf__ (char  *adr,char  *adrk, int ep_space)
/*-----------------------------------------------------------*/
{
 NAGLOWEK *nag;
 char *ad;

  obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK*)ad;
     ((LINIA*)nag)->obiektt3 = ep_space;
     obiekt_tok_all (NULL, adrk, &ad, ONieOkreslony);
  }
}

void reset_przec_dxf (char  *adr,char  *adrk)
/*-----------------------------------------------------------*/
{
    NAGLOWEK *nag;
    char *ad;

    obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
    while (ad!=NULL)
    {
        nag = (NAGLOWEK*)ad;
        if (nag->przec==1) {
            ((LINIA *) nag)->przec = 0;
            ((LINIA *) nag)->atrybut = Anormalny;
        }
        obiekt_tok_all(NULL, adrk, &ad, ONieOkreslony);
    }
}

void delete_przec_dxf (char  *adr,char  *adrk)
/*-----------------------------------------------------------*/
{
    NAGLOWEK *nag;
    char *ad;

    obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
    while (ad!=NULL)
    {
        nag = (NAGLOWEK*)ad;
        if (nag->przec==1) {
            ((LINIA *) nag)->przec = 0;
            ((LINIA *) nag)->atrybut = Abad;
        }
        obiekt_tok_all(NULL, adrk, &ad, ONieOkreslony);
    }
}

void change_objectt3_dxf (char  *adr,char  *adrk, int ep_space)
/*-----------------------------------------------------------*/
{
    NAGLOWEK *nag;
    char *ad;

    obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
    while (ad!=NULL)
    {
        nag = (NAGLOWEK*)ad;
        ((LINIA *) nag)->obiektt3 = ep_space;
        obiekt_tok_all(NULL, adrk, &ad, ONieOkreslony);
    }
}

void change_obiektt2_3_dxf_old(char  *adr, char  *adrk, int code2, int code3)
/*---------------------------------------------------------------------*/
{
	NAGLOWEK *nag;
	BLOK *block;
	char *ad;
	int i;
	int k;

	obiekt_tok_all((char *)adr, adrk, &ad, OdBLOK);
	i = 0;
	k = 6;
	while (ad != NULL)
	{
		nag = (NAGLOWEK*)ad;
		if (nag->obiekt == OdBLOK)
		{
			block = (BLOK*)ad;
			if (block->kod_obiektu != B_DIM)
			{
				obiekt_tok_all(NULL, adrk, &ad, OdBLOK);
				continue;
			}
		}

		((LINIA*)nag)->obiektt2 = code2;
		if (i == 5)
		{
			if ((nag->obiekt == Owwielokat) || (nag->obiekt == Okolo)) k = 6;
			else k = 7;
		}
		if ((nag->obiekt == Olinia) && ((i == 1) || (i == k))) ((LINIA*)nag)->obiektt3 = code3;
		else ((LINIA*)nag)->obiektt3 = 0;

		obiekt_tok_all(NULL, adrk, &ad, ONieOkreslony);
		i++;
	}
}

void change_obiektt2_3_dxf(char  *adr, char  *adrk, int code2, int code3)
/*---------------------------------------------------------------------*/
{

	NAGLOWEK *nag, *nag1;
	BLOK  *b;
	LINIA *l[9];
	char *adrb, *adrbk;
	char *adr0;
	BOOL b_dim = FALSE;

	adr0 = adr;

	if (adr == NULL || adrk == NULL)
	{
		return;
	}
	nag = (NAGLOWEK*)adr;
	while ((nag->obiekt != Okoniec) && (adr < adrk))
	{
		switch (nag->obiekt)
		{
		case OdBLOK:
			if (nag->obiektt2 == O2BlockDXF)
			{
				nag->obiektt2 = 0;
				if ((((BLOK *)nag)->kod_obiektu == B_DIM) || (((BLOK *)nag)->kod_obiektu == B_DIM1) || (((BLOK *)nag)->kod_obiektu == B_DIM2))
					b_dim = TRUE; else b_dim = FALSE;

				if (b_dim == TRUE) nag->obiektt2 = O2BlockDim;
				else nag->obiektt2 = 0;

				adrb = adr + sizeof(NAGLOWEK) + B3 + ((BLOK *)nag)->dlugosc_opisu_obiektu;
				adrbk = adr + nag->n + sizeof(NAGLOWEK);

				nag1 = (NAGLOWEK*)adrb;
				while (adrb < adrbk)
				{
					switch (nag1->obiekt)
					{
					case OdBLOK:
						if ((((BLOK *)nag1)->kod_obiektu == B_DIM) || (((BLOK *)nag1)->kod_obiektu == B_DIM1) || (((BLOK *)nag1)->kod_obiektu == B_DIM2))
							b_dim = TRUE; else b_dim = FALSE;

						if (b_dim == TRUE) nag1->obiektt2 = O2BlockDim;
						else nag1->obiektt2 = 0;

						adrb += sizeof(NAGLOWEK) + B3 + ((BLOK *)nag1)->dlugosc_opisu_obiektu;
						break;
					case Olinia:
						adrb += nag1->n + sizeof(NAGLOWEK);
						break;
					case Oluk:
						adrb += nag1->n + sizeof(NAGLOWEK);
						break;
					case Owwielokat:
						adrb += nag1->n + sizeof(NAGLOWEK);
						break;
					case Okolo:
						adrb += nag1->n + sizeof(NAGLOWEK);
						break;
					case Otekst:
						adrb += nag1->n + sizeof(NAGLOWEK);
						break;
					default:
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

void change_layer_dxf (char  *adr,char  *adrk, int new_layer)
/*---------------------------------------------------------*/
{
 NAGLOWEK *nag;
 char *ad;

  obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK*)ad;
     if (nag->obiekt != OdBLOK)
     {
       if (((LINIA*)nag)->warstwa == 0 ) ((LINIA*)nag)->warstwa = new_layer;
     }
     obiekt_tok_all (NULL, adrk, &ad, ONieOkreslony);
  }
}

static BOOL check_mem_list__ (void)
/*------------------------------*/
{
  long l_free, l_size ;
  BOOL b_ret ;

  b_ret = FALSE ;
  l_free = get_free_mem_size (0) ;
  l_size = MaxNoBlock * (sizeof (long) + sizeof(POLE)) ;
  mList_Blok.maxw = MaxMenuWin ;
  mList_Blok.max = MaxMenuWin ;
  l_size += Get_Menu_Image_Size (&mList_Blok) ;
  if (l_size < l_free - 500) /*500 - rezerwa na dalsze alokacje w set_insert_point*/
  {
    ptr__off_block = (long*)malloc /*getmem*/ (MaxNoBlock * sizeof(long));
    pmList_Blok = (POLE*)malloc /*getmem*/ (MaxNoBlock * sizeof(POLE));
    b_ret = TRUE ;
  }
  return b_ret ;
}

static void free_mem_list (void)
/*----------------------------*/
{
	free(ptr__off_block);
	free(pmList_Blok);
}

int compare (POLE * p1, POLE *p2)
/*--------------------------------*/
{
  char *st1, *st2 ;

  st1 = p1->txt ;
  st2 = p2->txt ;
  return (strcmp (st1, st2) ) ;
}
static BOOL mfind (POLE *key, POLE *pmList_Blok, int k)
{
  int i ;

  for (i = 0 ; i < k; i++)
  {
    if (!compare (key, &pmList_Blok [i]))
     return TRUE ;
  }
  return FALSE ;
}

static char *find_block_name(char *block_name, int only_dxf_block)
/*--------------------------------------------------------------*/
{
  BLOK *ptrs_block ;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  char  *adk ;
  int k, max_len, len ;
  POLE key ;
  char st[10];
  char st1[70];

  k = 0 ;
  max_len = 0 ;
  adk = dane + dane_size ;
  obiekt_tok_all ((char*)dane, adk, (char **)&ptrs_block, OdBLOK) ;
  while (ptrs_block != NULL)
  {
    if ( ((ptrs_block->kod_obiektu == B_EXPORT) ||
            (ptrs_block->kod_obiektu == B_GRAPH) ||
          (ptrs_block->kod_obiektu == B_PPOMIAR) ||
          (ptrs_block->kod_obiektu == B_KPOMIAR) ||
          (ptrs_block->kod_obiektu == B_ODWIERT_OLD) ||
          (ptrs_block->kod_obiektu == B_ODWIERT) ||
          (ptrs_block->kod_obiektu == B_INSTALACJE) ||
          (ptrs_block->kod_obiektu == B_HEKTOMETRY) ||
          (ptrs_block->kod_obiektu == B_SEKTORY) ||
		 (
		  ((ptrs_block->kod_obiektu == B_DIM) ||
		   (ptrs_block->kod_obiektu == B_DIM1) ||
		   (ptrs_block->kod_obiektu == B_DIM2) ||
           (ptrs_block->kod_obiektu == B_DIM3) ||
		   (ptrs_block->kod_obiektu == B_HATCH) ||
		   (ptrs_block->kod_obiektu == B_PLINE)) &&
			 only_dxf_block==1)
         || (ptrs_block->kod_obiektu > 0)
          )
       && ptrs_block->dlugosc_opisu_obiektu > 1)
    { 
      ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
      if (ptrs_desc_bl->flags == EBF_IP && ptrs_desc_bl->sz_type [0] != '\0')
      {
       if ((only_dxf_block==0) || (ptrs_block->obiektt2==O2BlockDXF))
       {
	       strcpy(key.txt, &ptrs_desc_bl->sz_type [0]) ; 
		   //////////
         if (strcmp((char*)key.txt, block_name) == 0)
          {
            return (char  *)ptrs_block ;
          }
       }
      }
    }
   obiekt_tok_all (NULL, adk, (char **)&ptrs_block, OdBLOK) ;
  }
  return NULL;
}


static int set_list_block (int only_dxf_block, char prefix, BOOL set_adr)
/*---------------------------------------------------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  char  *adk ;
  int k, max_len, len ;
  POLE key ;
  char st[10];
  char st1[70];
  int ret;
  long *ptr__off_block1;
  POLE *pmList_Blok1 ;
  int MaxNoBlock1;
  BOOL to_many=FALSE;

  MaxNoBlock=1000;

  ptr__off_block = (long*)malloc (MaxNoBlock * sizeof(long));
  if (ptr__off_block==NULL) return 0;
  pmList_Blok = (POLE*)malloc (MaxNoBlock * sizeof(POLE));
  if (pmList_Blok==NULL)
  {
      free(ptr__off_block);
      return 0;
  }

  k = 0 ;
  max_len = 0 ;
  adk = dane + dane_size ;
  obiekt_tok_all ((char*)dane, adk, (char **)&ptrs_block, OdBLOK) ;
  while (ptrs_block != NULL)
  {
    if ( ((ptrs_block->kod_obiektu == B_EXPORT) ||
            (ptrs_block->kod_obiektu == B_GRAPH) ||
          (ptrs_block->kod_obiektu == B_PPOMIAR) ||
          (ptrs_block->kod_obiektu == B_KPOMIAR) ||
          (ptrs_block->kod_obiektu == B_ODWIERT_OLD) ||
          (ptrs_block->kod_obiektu == B_ODWIERT) ||
          (ptrs_block->kod_obiektu == B_INSTALACJE) ||
          (ptrs_block->kod_obiektu == B_HEKTOMETRY) ||
          (ptrs_block->kod_obiektu == B_SEKTORY)
          )
       && ptrs_block->dlugosc_opisu_obiektu > 1)
    { 
      ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
      if (ptrs_desc_bl->flags == EBF_IP && ptrs_desc_bl->sz_type [0] != '\0')
      {
       if ((only_dxf_block==0) || (ptrs_block->obiektt2==O2BlockDXF))
       {
    strcpy(key.txt, &ptrs_desc_bl->sz_type [0]) ;  
	//////////
	if (FALSE == mfind (&key, pmList_Blok, k))
	{

       sprintf(pmList_Blok [k].txt, "%c%s", prefix, &ptrs_desc_bl->sz_type [0]) ;

        ///////////
	   pmList_Blok [k].wcod = 1 ;
	   len = strlen (&ptrs_desc_bl->sz_type [0]) ;

       //check if empty block

       int len_bl_obj = ptrs_block->n;
       if ((len_bl_obj-len)>21)
       {
           if (len > max_len) {
               max_len = len;
           }

           if (set_adr) {
               pmList_Blok[k].menu = (void *) ((long) ((char *) ptrs_block - dane));
               pmList_Blok[k].iconno = k;
           } else {
               pmList_Blok[k].menu = NULL;
               pmList_Blok[k].iconno = 0;
           }

           ptr__off_block[k] = (char *) ptrs_block - dane;

           k++;

           if (k >= MaxNoBlock - 1)
           {
               //realocation
               MaxNoBlock1=MaxNoBlock+1000;
               ptr__off_block1 = (long*)realloc(ptr__off_block, MaxNoBlock1 * sizeof(long));
               if (ptr__off_block1!=NULL)
               {
                   pmList_Blok1 = (POLE *) realloc(pmList_Blok, MaxNoBlock1 * sizeof(POLE));
                   if (pmList_Blok1 != NULL) {
                       ptr__off_block=ptr__off_block1;
                       pmList_Blok=pmList_Blok1;
                       MaxNoBlock=MaxNoBlock1;
                   }
                   else to_many=TRUE;
               }
               else to_many=TRUE;

               if (to_many) {
                   ret = snprintf(st, 6, "%d", k);
                   strcpy(st1, _NUMBER_OF_BLOCKS_);
                   strcat(st1, st);
                   strcat(st1, _EXCEEDS_NO_);
                   komunikat_str(st1);
                   break;
               }
           }
       }
	}
       }	
      }
    }
    obiekt_tok_all (NULL, adk, (char **)&ptrs_block, OdBLOK) ;
  }
  mList_Blok.poz = 0 ;
  mList_Blok.foff = 0 ;
  mList_Blok.max = k ;
  mList_Blok.xdl = MaxTextLength  ;
  if (max_len + 3 < MaxTextLength )
  {
    mList_Blok.xdl = max_len + 3 ;
  }
  if (k < mList_Blok.maxw)
  {
    mList_Blok.maxw = 0 ;
  }
  mList_Blok.pola = pmList_Blok ;
  return k ;
}

static BLOK * copy_block (int n)
/*---------------------------*/
{
  BLOK *ptrs_sel_blok ;

  ptrs_sel_blok = (BLOK*)(dane + ptr__off_block [n]) ;
  ptrs_sel_blok = (BLOK*)Copy_Object ((char  *)ptrs_sel_blok) ;
  return ptrs_sel_blok ;
}

static BLOK * copy_block_List (int n)
/*---------------------------*/
{
    BLOK *ptrs_sel_blok ;

    ptrs_sel_blok = (BLOK*)(dane + ptr__off_block [n]) ;
    ptrs_sel_blok = (BLOK*)(dane + (long)pmList_Blok[n].menu) ;
    ptrs_sel_blok = (BLOK*)Copy_Object ((char  *)ptrs_sel_blok) ;
    return ptrs_sel_blok ;
}

static BLOK * copy_block_b (int n)
/*------------------------------*/
{
  BLOK *ptrs_sel_blok ;

  ptrs_sel_blok = (BLOK*)(dane + ptr__off_block [n]) ;
  ptrs_sel_blok = (BLOK*)Copy_Object_Block ((char  *)ptrs_sel_blok) ;
  return ptrs_sel_blok ;
}

static BLOK * copy_this_block (char *adr)
/*-------------------------------------*/
{
  BLOK *ptrs_sel_blok ;

  ptrs_sel_blok = (BLOK*)(adr) ;
  ptrs_sel_blok = (BLOK*)Copy_Object ((char  *)ptrs_sel_blok) ;
  return ptrs_sel_blok ;
}

static BLOK * copy_this_block_b (char *adr)
/*---------------------------------------*/
{
  BLOK *ptrs_sel_blok ;

  ptrs_sel_blok = (BLOK*)(adr) ;
  ptrs_sel_blok = (BLOK*)Copy_Object_Block ((char  *)ptrs_sel_blok) ;
  return ptrs_sel_blok ;
}


static BLOK * copy_m_block (BLOK *ptrs_sel_blok)
/*--------------------------------------------*/
{
  ptrs_sel_blok = (BLOK*)Copy_Object ((char  *)ptrs_sel_blok) ;
  return ptrs_sel_blok ;
}

static BLOK * copy_m_block_b (BLOK *ptrs_sel_blok)
/*----------------------------------------------*/
{
  ptrs_sel_blok = (BLOK*)Copy_Object_Block ((char  *)ptrs_sel_blok) ;
  return ptrs_sel_blok ;
}


BOOL Get_List_Block (double *df_x, double *df_y, RYSPOZ *adp, RYSPOZ *adk, char *buf, int lenmax)
/*--------------------------------------------------------------------------------------------*/
{
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  char  *		ptrh_temp ;

  if (ptrs__sel_blok == NULL)
  {
    return FALSE ;
  }
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs__sel_blok->opis_obiektu [0]) ;
  *df_x = ptrs_desc_bl->x  ;
  *df_y = ptrs_desc_bl->y ;
  *adp =  (RYSPOZ)ptrs__sel_blok ;
  ptrh_temp = (char *)ptrs__sel_blok ;
  ptrh_temp += sizeof(NAGLOWEK) + ptrs__sel_blok->n - 1;
  *adk = ptrh_temp ;
  strncpy (buf, &ptrs_desc_bl->sz_type[0], lenmax) ;
  buf [lenmax - 1] = '\0' ;
  zmien_atrybut (*adp, *adk, Anormalny, Ablok) ;
  return  TRUE ;
}


void List_Blocks (void)
/*-------------------*/
{
  int n ;
  char prefix;

  if (blocks_dialog==0) prefix=' ';
  else prefix='\023';

  if (blocks_dialog==0)
  {
      if (0 == set_list_block (0, prefix, FALSE))
      {
          ErrList (69) ;
          return ;
      }

      if ((n = getwsp1(&mList_Blok) - 1) >= 0)  /* n=-1 -> esc */
      {
          ptrs__sel_blok = copy_block(n);
          free_mem_list();
          global_no_pcx = TRUE;
          Place_Import_Block(1);
          global_no_pcx = FALSE;
      } else {
          free_mem_list();
      }
  }
  else
  {
      if (0 == set_list_block (0, prefix, TRUE))
      {
          ErrList (69) ;
          free_mem_list () ;
          return ;
      }
      if ((n = getBlockFromDialog(&mList_Blok, ptr__off_block)) >= 0)
      {
          ptrs__sel_blok = copy_block_List(n);
          free_mem_list();
          global_no_pcx = TRUE;
          Place_Import_Block(1);
          global_no_pcx = FALSE;
      } else {
          free_mem_list();
      }
  }
}



void Insert_Block (INSERT *insert, int to_block, BOOL block)
/*--------------------------------------------------------*/
{
  int n ;
  double X0,Y0;
  char blok_name [MaxLen] = "Blok : ";
  double kos, koc;
  BOOL b_check_arc;
  int k, l_blocks;
  
  double df_dx, df_dy ;
  int i, j ;
  long l_block_size ;
  
  
  for (i = 0, df_dy = 0 ;
       i < insert->row_count;
       i++, df_dy += insert->row_spacing )
{
  for (j = 0, df_dx = 0 ;
	 j < insert->column_count;
	 j++, df_dx += insert->column_spacing )  
 {
  
  ADP=ADK=NULL;

  ptrs__sel_blok = find_block_name(insert->block_name, 1);

 if (ptrs__sel_blok != NULL)
  {

   if (to_block==0) ptrs__sel_blok = copy_this_block (ptrs__sel_blok) ;
     else ptrs__sel_blok = copy_this_block_b (ptrs__sel_blok) ;

    if (FALSE == Get_List_Block (&X0, &Y0, &ADP, &ADK, &blok_name [7], MaxLen - 10))
     {
       return ;
     }

    b_check_arc = check_if_obiekt (ADP, ADK, Ablok, Oluk); 
    
    if (insert->typ==0) 
    {
     if ((insert->x_scale!=1) || (insert->y_scale!=1))
     transformacja_blok (ADP, ADK, X0, Y0, insert->x_scale, insert->y_scale, Tskala,0) ;
    
	
     if (insert->rotation_angle!=0) 
      {
       kos = sin (insert->rotation_angle) ;
       koc = cos (insert->rotation_angle) ;
       transformacja_blok (ADP, ADK, X0, Y0, kos, koc, Tobrot,0) ;
      } 
      
     
     transformacja_blok(ADP,ADK,insert->x -X0 + df_dx,insert->y -Y0 + df_dy,0,0,Tprzesuw,0);
     if (insert->z_scale<0)
      {  
       transformacja_blok (ADP, ADK, insert->point_x_x, 0, insert->point_x_x, 100, Tmirror,0);
      }

    }
    
    else  /*DIMENSION*/
    { 
     transformacja_blok(ADP,ADK,insert->m_x + df_dy, insert->m_y + df_dy,0,0,Tprzesuw,0);

    }  
    
    if (to_block==0)
    {
	  if (!block) change_layer_dxf (ADP, ADK, insert->layer); 
	  else change_layer_dxf(ADP, ADK, Current_Layer);
      change_colors_dxf (ADP, ADK, insert->color);
    }

    change_obiektt2_3_dxf (ADP, ADK, O2BlockDim, O3WymRoz);

    if (insert->typ==0)
    {
      set_insert_point (insert->x + df_dx, insert->y + df_dy, (void*)ADP) ; 
    }
    else
     {
      set_insert_point (insert->m_x + df_dx, insert->m_y + df_dy, (void*)ADP) ; 
     }  

	if  (insert->typ == 1) change_block_name((void*)ADP, TRUE, insert->rotation_angle);
    else if (insert->useri1!=211) change_block_name ((void*)ADP, FALSE, 0.0) ;
 
    zmien_atrybut(ADP, ADK, Ablok, Anormalny);  
  }
  else  /*nie znaleziono bloku*/
  {
   ;
  }
 } 
}  
}

#undef __O_LISTBL__
