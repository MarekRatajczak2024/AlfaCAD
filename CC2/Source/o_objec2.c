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
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_debug.h"

#include "leak_detector_c.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

typedef struct
{
  char *address;
  BOOL blok;
} OdBLOK_ADDRESS;

OdBLOK_ADDRESS *OdBLOK_array;
static int block_array_no=0;
static int block_array_part=0;
static int block_array_last_block0=0;

void Set_Block_Change_Mode (void *ptr_ob, int mode)
/*------------------------------------------------*/
{
  BLOK *ptrs_block ;

  if ( ((NAGLOWEK*)ptr_ob)->blok == ElemBlok &&
       NULL != (ptrs_block = Parent_Block ((char  *)ptr_ob)) &&
       ptrs_block->obiektt1 < mode)
  {
    ptrs_block->obiektt1 = mode ;
  }
}

long_long Get_Size_Block (long_long offp, long_long offk)
/*---------------------------------------*/
{
  long_long off , l_ret ;
  NAGLOWEK *nag ;

  ObiektTok (&offp, offk, &off, ONoInBlock) ;
  l_ret = 0 ;
  while (off != -1)
  {
    nag =(NAGLOWEK*)(dane + off) ;
    if (nag->atrybut == Ablok)
    {
      l_ret += sizeof (NAGLOWEK) + nag->n ;
    }
    ObiektTok (&offp, offk, &off, ONoInBlock) ;
  }
  return l_ret ;
}


void Set_Object_Temp_Flag (int flag, long_long offp, long_long offk)
/*----------------------------------------------------------------*/
{
  long_long off ;
  NAGLOWEK *nag ;

  ObiektTok (&offp, offk, &off, ONieOkreslony) ;
  while (off != -1)
  {
    nag =(NAGLOWEK*)(dane + off) ;
    if (nag->przec != flag)
    {
      nag->przec = flag ;
    }
    ObiektTok (&offp, offk, &off, ONieOkreslony) ;
  }
}

void Set_Object_T2_Flag (int flag, long_long offp, long_long offk)
/*------------------------------------------------------*/
{
  long_long off ;
  NAGLOWEK *nag ;

  ObiektTok (&offp, offk, &off, ONieOkreslony) ;
  while (off != -1)
  {
    nag =(NAGLOWEK*)(dane + off) ;

    if ((flag==O2BlockPline) && ((nag->obiektt2==O2BlockAparat) || (nag->obiektt2>=O2ROLL_EDGE)))
     {
       ;
     }
       else  nag->obiektt2 = flag ;
    ObiektTok (&offp, offk, &off, ONieOkreslony) ;
  }
}

void Ini_Global_Object (void)
/*---------------------------*/
{
    LiniaG.kolor = PointG.kolor = OkragG.kolor=LukG.kolor=TextG.kolor=VectorG.kolor=Layers[Current_Layer].color;
    LiniaG.typ=OkragG.typ=LukG.typ=VectorG.typ=Layers[Current_Layer].line_type;
    LiniaG.warstwa = PointG.warstwa= OkragG.warstwa=LukG.warstwa=TextG.warstwa=VectorG.warstwa=Current_Layer;
}

int Get_Current_Color (void)
/*--------------------------*/
{
  return LiniaG.kolor  /*Layers[Current_Layer].color*/ ;
}

int Get_Width_Line (int type)
/*--------------------*/
{
  return ((type & 224)/32);
}

int Get_Type_Line (int type)
/*------------------------*/
{
  return (type & 31);
}

int Set_WidthL_to_Type (int type, int width)
/*--------------------------------------*/
{
  int type_l, type_new;

  if (width == 5) return type;  //no change

  type_l = (type & 31);  /*typ*/
  type_new = (width * 32) + type_l;
  return type_new;
}

int Set_WidthSolid_to_Type (int type, int width)
/*--------------------------------------------*/
{
  int width_new;

  if (width==5)  width_new = 0;
  else width_new = width + 1;
  return width_new;
}

int Set_TypeL_to_Type (int type, int type_line)
/*--------------------------------------------*/
{
  int width, type_new;

  width = (type & 224); /*grubosc*/
  type_new = width + type_line;
  return type_new;
}

BLOK * Get_Common_Blok (void * ado1, void *ado2)
/*--------------------------------------------*/
{
  BLOK  *adb1,  *adb2;

  if (ado1 == NULL || ado2 == NULL)
  {
    return NULL;
  }

  //check if maybe both belong to the same polyline polyline
  if (NULL == (adb1 = (BLOK*)FIRSTB((char*)ado1)))
  {
      return NULL;
  }
  if (NULL == (adb2 = (BLOK*)FIRSTB((char*)ado2)))
  {
      return NULL;
  }
  if (adb1 == adb2)
  {
      return (BLOK*)adb1;
  }

  if (NULL == (adb1 = (BLOK  *)LASTB ((char  *)ado1)))
  {
    return NULL;
  }
  if (NULL == (adb2 = (BLOK  *)LASTB ((char  *)ado2)))
  {
    return NULL;
  }
  if ( adb1 != adb2)
  {
    return NULL;
  }
  return (BLOK *)adb1;
}

BLOK *FIRSTB(char  *ado)
/*------------------------*/
{
  BLOK *lb=NULL;
  char *ad;
  char  *adp;
  int nr_bloku;

  if (ado == NULL)
  {
    return NULL;
  }
   		/*ado - 1  blok nie moze byc swoim wlasnym rodzicem*/
  obiekt_tok2((char*)dane, ado - 1, &ad,OdBLOK);
  while (ad!=NULL)
   { if( (adp=(char *)ad+sizeof(NAGLOWEK)+((NAGLOWEK*)ad)->n) > ado )
     {
       lb=(BLOK*)ad;
       //break;  //13-11-2020  
     }
     obiekt_tok2(NULL,ado-1,&ad,OdBLOK); //-1 to not take block itself
   }
  return lb;
}

//variants of LASTB and FIRSTB with complete array of blocks
int Ini_Blocks_Array(void)
{
    BLOK *lb=NULL;
    char *ad;
    char  *adp;
    char *adk=dane+dane_size-1;
    OdBLOK_ADDRESS *new_OdBLOK_array;

    block_array_no=0;
    block_array_part=0;
    block_array_last_block0=0;

    if (adk == dane) return 0;  //no blocks

    OdBLOK_array = malloc(MaxNoBlock*sizeof(OdBLOK_ADDRESS));  //(OdBLOK_ADDRESS*)
    if (OdBLOK_array==NULL) return -1;  //error

    obiekt_tok1((char*)dane, adk, &ad,OdBLOK);
    while (ad!=NULL)
    {
        OdBLOK_array[block_array_no].address=ad;
        OdBLOK_array[block_array_no].blok=((NAGLOWEK*)ad)->blok;
        block_array_no++;
        block_array_part++;
        if (block_array_part==MaxNoBlock)
        {
            block_array_part=0;
            new_OdBLOK_array = realloc(OdBLOK_array, (block_array_no+MaxNoBlock)*sizeof(OdBLOK_ADDRESS));
            if (new_OdBLOK_array==NULL) return 0;
            OdBLOK_array=new_OdBLOK_array;
        }
        obiekt_tok1(NULL,adk,&ad,OdBLOK);
    }
    return block_array_no;
}

void Destroy_Blocks_Array(void)
{
    free(OdBLOK_array);
    block_array_no=0;
    block_array_part=0;
    block_array_last_block0=0;
}

BLOK *LASTB_A(char *ado)
{   int i;
    char *adb=NULL;

    if (OdBLOK_array==NULL) return NULL;
    if (block_array_no==0) return NULL;

    for (i=block_array_last_block0; i<block_array_no; i++)
    {
        if ((OdBLOK_array[i].address<ado) && (OdBLOK_array[i].blok==0))
        {
            adb=OdBLOK_array[i].address;  //we assume that ((NAGLOWEK*)ado)->blok==1
            block_array_last_block0=i;
        }
        if (OdBLOK_array[i].address>ado) break;
    }
    return adb;
}

BLOK *FIRSTB_A(char *ado)  //NOT OPTIMIZED YET
{   int i;
    char *adb=NULL;

    if (OdBLOK_array==NULL) return NULL;
    if (block_array_no==0) return NULL;

    for (i=0; i<block_array_no; i++)
    {
        if (OdBLOK_array[i].address<ado) adb=OdBLOK_array[i].address;  //we assume that ((NAGLOWEK*)ado)->blok==1
        if (OdBLOK_array[i].address>ado) break;
    }
    return adb;
}

BLOK *LASTB(char  *ado)
/*------------------------*/
{
  BLOK *lb=NULL;
  char *ad;
  char  *adp;

  if (ado == NULL)
  {
    return NULL;
  }
   		/*ado - 1  blok nie moze byc swoim wlasnym rodzicem*/
  obiekt_tok1((char*)dane, ado - 1, &ad,OdBLOK);  //-1 to not take block itself
  while (ad!=NULL)
   { if( (adp=(char *)ad+sizeof(NAGLOWEK)+((NAGLOWEK*)ad)->n) > ado )
     {
       lb=(BLOK*)ad;
       break;
     }
     obiekt_tok1(NULL,ado-1,&ad,OdBLOK); //-1 to not take block itself
   }
  return lb;
}

BLOK *Parent_Block (char  *ado)	/*Block_Owner*/
/*-------------------------------------------*/
{
  BLOK *lb = NULL ;
  BLOK *adb ;
  char  *adb_end ;

  if (ado == NULL)
  {
    return NULL;
  }
  obiekt_tok1 ((char*)dane, ado - 1, (char **)&adb, OdBLOK) ; //-1 to not take block itself
  while (adb != NULL)
  {
    adb_end = (char *)adb ;
    adb_end += sizeof(NAGLOWEK) + adb->n ;
    if( adb_end > ado )
    {
      lb = (BLOK*)adb ;
	  //break;
    }
    obiekt_tok1 (NULL, ado - 1, (char**)&adb, OdBLOK) ;  //-1 to not take block itself
  }
  return lb ;
}

double Get_Scale (double xmin, double xmax, double ymin, double ymax)
/*------------------------------------------------------------------*/
{
  double dx, dy, skale ;

  if (xmin >= xmax || ymin >= ymax)
  {
    return -1 ;
  }
  dx = maxXr / (DokladnoscF * sk_x) ;
  dy = maxYr / DokladnoscF ;
  skale = max( (xmax - xmin) / dx, (ymax - ymin) / dy ) ;
  skale = 1 / skale ;
  if (skale <= 0)
  {
    return -1 ;
  }
  return skale ;
}

void parametry_lini (LINIA  *L,PLINIA *PL)
/*--------------------------------------*/
{
  double dx,dy;

  if (L == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  dx = L->x2 - L->x1;
  dy = L->y2 - L->y1;
  if (TRUE == Check_if_Equal (dy, 0))
  {
    PL->kat=(dx>=0 ? 0 : 180);
    PL->cos=(dx>=0 ? 1 : -1);
    PL->sin=0;
    PL->dl=fabs(dx);
  }
  else
  if (TRUE == Check_if_Equal (dx, 0))
  {
    PL->kat=(dy>=0 ? 90 : 270);
    PL->sin=(dy>=0 ? 1 : -1);
    PL->cos=0;
    PL->dl=fabs(dy);
  }
  else
  {
    PL->dl=sqrt(dx*dx+dy*dy);
    PL->sin=dy/PL->dl;
    PL->cos=dx/PL->dl;
    PL->kat=(180/Pi)*Atan2(dy,dx);
    if ((PL->kat)<0) PL->kat+=360.0;
  }
  PL->dx=dx;
  PL->dy=dy;
}

void parametry_lini3D (LINIA3D  *L,PLINIA3D *PL)
/*--------------------------------------------*/
{
  double dx,dy,dz;

  if (L == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  dx = L->x2 - L->x1;
  dy = L->y2 - L->y1;
  dz = L->z2 - L->z1;
  if (TRUE == Check_if_Equal (dy, 0))
  {
    PL->kat=(dx>=0 ? 0 : 180);
    PL->cos=(dx>=0 ? 1 : -1);
    PL->sin=0;
    PL->dl=sqrt(dx*dx+dz*dz);
  }
  else
  if (TRUE == Check_if_Equal (dx, 0))
  {
    PL->kat=(dy>=0 ? 90 : 270);
    PL->sin=(dy>=0 ? 1 : -1);
    PL->cos=0;
    PL->dl=sqrt(dy*dy+dz*dz);
  }
  else
  {
    PL->dl=sqrt(dx*dx+dy*dy+dz*dz);
    PL->sin=dy/PL->dl;
    PL->cos=dx/PL->dl;
    PL->kat=(180/Pi)*Atan2(dy,dx);
    if ((PL->kat)<0) PL->kat+=360.0;
  }
}


void *Add_Object_Set_Global_Ptr (BLOK ** ptrptr_adb, void  *ptr_ob)
/*------------------------------------------------------------------*/
{
  char  *ptrh_temp ;
  NAGLOWEK *ptrs_nag ;
  void *ptr_ob_ret ;

  l__Off_Bl_Beg = -1l ;
  l__Off_Bl_End = -1l ;
  if (NULL == (ptr_ob_ret = Add_Object (ptrptr_adb, ptr_ob)))
  {
    return NULL ;
  }
  if (ADP == NULL || ADK == NULL)
  {
    return ptr_ob_ret ;
  }
  ptrs_nag = (NAGLOWEK*)ptr_ob_ret ;
  ptrh_temp = (char *)ptr_ob_ret ;
  if (ptrs_nag->atrybut == Ablok)
  {
    if (ptrh_temp < ADP)
    {
      ADP = ptrh_temp ;
    }
    if (ptrh_temp > ADK)
    {
      ADK = ptrh_temp ;
      ADK +=  sizeof(NAGLOWEK) + ((NAGLOWEK*)ADK)->n + 1 ;
    }
  }
  if (ADP != NULL && FALSE == Test_Object ((void*)ADP))
  {
    Internal_Error (__LINE__,__FILE__);
    return NULL ;
  }
  l__Off_Bl_Beg = (long)(ADP - dane ) ;
  l__Off_Bl_End = (long)(ADK - dane ) ;
  return ptr_ob_ret ;
}


void Erase_Object_Set_Global_Ptr (void  *ptr_ob)
/*-----------------------------------------------*/
{
  usun_obiekt (ptr_ob) ;
  if (ADP != NULL && FALSE == Test_Object ((void*)ADP))
  {
    Internal_Error (__LINE__,__FILE__);
    l__Off_Bl_Beg = -1l ;
    l__Off_Bl_End = -1l ;
    return ;
  }
  l__Off_Bl_Beg = (long_long)(ADP - dane ) ;
  l__Off_Bl_End = (long_long)(ADK - dane ) ;
  return ;
}

void Erase_First_Objects (long_long size)
/*--------------------------------*/
{
  dane_size -= size ;
  dane += size ;
  SetBufferDaneSize (dane_size) ;
  Change = TRUE ;
  Set_Auto_Backup (TRUE) ;
}

void *Change_Object_Set_Global_Ptr (void  *ptr_ob, void  *ptr_new)
/*---------------------------------------------------------------------*/
{
  void *ptr_ob_ret ;

  ptr_ob_ret = korekta_obiekt (ptr_ob, ptr_new) ;
  if (ADP != NULL && FALSE == Test_Object ((void*)ADP))
  {
    Internal_Error (__LINE__,__FILE__);
    l__Off_Bl_Beg = -1l ;
    l__Off_Bl_End = -1l ;
    return NULL ;
  }
  l__Off_Bl_Beg = (long)(ADP - dane ) ;
  l__Off_Bl_End = (long)(ADK - dane ) ;
  return ptr_ob_ret ;
}




