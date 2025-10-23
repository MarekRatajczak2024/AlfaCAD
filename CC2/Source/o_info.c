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

#define __INFO__
#include<forwin.h>
#ifndef LINUX
#include<stddef.h>
#endif
#include<stdio.h>
#include<math.h>
#include<string.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_buf.h"
#include "alffont.h"
#include "alf_res.h"

#include "menu.h"

extern unsigned long_long Get_imageBMPsize(void);
extern int getmaxx(void);
extern int getmaxy(void);
extern int demo(int Alf_window_number);
extern int demo_out(int Alf_window_number);
extern BLOK *FIRSTB(char *ado);
extern BLOK *LASTB(char *ado);
extern void parametry_lini3D (LINIA3D  *L,PLINIA3D *PL);
extern void return_menu_par (char *pole, char * par);
extern void return_menu_par0 (char *pole, char * par);
extern BOOL Add_String_To_List (char *ptr_string);
extern void TTF_logo(int x, int y);
extern char *Colors_Name_txt [MAXCOLOR + 1];
extern void decodingwin(char *text);
extern long_long Get_memory_size(void);
extern void Set_Memory_Size(unsigned long size);
extern char digits[16];
extern char numbers[16];
extern BOOL Semaphore;
extern BOOL Cust_Semaphore;

extern char * view_type_tab[];
extern char * view_edge_tab[];
extern char *view_edgeinverted_tab[];

extern int Get_Key (void) ;
extern int openwh(TMENU *menu) ;
extern void  closewh(TMENU *menu) ;
extern long Get_UMB_Size (long *dw_sizemax) ;
extern int __dpmi_free_dos_memory(int selector);
extern BOOL TTF_redraw;

extern void set_decimal_format(char *text, double l, double precision);
extern double thermal_precision;
extern double force_precision;
extern double moment_precision;
extern double displacement_precision;
extern double rotation_precision;
extern double load_precision;

extern BOOL edit_param_no0[46];
extern BOOL edit_param_no[46];

extern char* typ_punktu_inf[];
extern int point_icon[];
extern int edge_icon[];
extern char *load_symbol[];

extern void alfacad_logo_info(void);

static void nooph(void)
{}
#define noooph (void ( *)(int x,int y))nooph


enum info {iObject=0, iLayer, iColor, iLType, iLWidth, iEdge, iEdgeInverted, iPType, iVstyle, iX1, iY1, iX2, iY2, iX3, iY3, iX4, iY4,
        iRadius, iRadiusY, iAngle, iAngle1, iAngle2, iWidth1, iWidth2, iMagnitude, iMagnitude1, iMagnitude2, iAxisOffset1, iAxisOffset2, iLoadCharacter, iOpacity, iLenght,  iDX, iDY, iArea, iFont,
        iFType, iHidden, iAllign, iHeight, iWidthFactor, iItalic, iBold, iUnderlined, iLineSpacing,
        iPxlDx, iPxlDy, iInnerBlockName, iOuterBlockName};

static POLE pmInfoAboutA[iOuterBlockName+1];
TMENU mInfoAboutA = { iOuterBlockName+1,0,0,40,2, 4, ICONS | TADD | NOWCLOSE, CMNU,CMBR,CMTX,0,0,0,0,0,&pmInfoAboutA, NULL, NULL };

BOOL active[iOuterBlockName+1] =  {0};  //{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int param_no[iOuterBlockName+1];

static void uaktualnij_pola (void)
/*------------------------------*/
{
  char sk [20] ;
  char sk1[MAXPATH];
  unsigned long_long mem_virtual, mem_dos ;
  char * st ;
  int len ;        /*maxlen = 36*/
  char *ptrsz_lab ;
  unsigned long_long imageBMPsize1 ;
  unsigned long_long imageBMPsize_used1;
  unsigned long_long imageBMPsize_total_used1;

  mem_virtual = Get_Free_Virtual_Memory();

  sprintf (sk, "%#ld / %#ld", Get_Buf_Mem_Size () / 1024, Get_memory_size() / 1024) ;
  menu_par_new((*mInfo.pola)[0].txt, sk) ;

  imageBMPsize1=Get_imageBMPsize() / (1024 * 1000);
  imageBMPsize_used1 = imageBMPsize_used / (1024 * 1000);
  imageBMPsize_total_used1 = imageBMPsize_total_used / (1024 * 1000);
 
//  sprintf (sk, "%#ld", imageBMPsize / 1024/*mem_virtual*/) ;
  sprintf (sk, "%#ld/%#ld (%#ld)", imageBMPsize1, imageBMPsize_total_used1, imageBMPsize_used1) ;
  menu_par_new((*mInfo.pola)[1].txt, sk) ;

  sprintf (sk, "%lu", dane_size - 8 /*4*/) ;
  menu_par_new((*mInfo.pola)[2].txt, sk) ;

  sprintf (sk, "%#lu", Bufor_Wydruku / 1024 ) ;
  menu_par_new((*mInfo.pola)[3].txt, sk) ;

  sprintf (sk, "%u", Get_Buf_Mak_Size ()) ;
  menu_par_new((*mInfo.pola)[4].txt, sk) ;

  sprintf (sk, "%#lu/%#lu", Bufor_PCX / 1024, Wielkosc_Plata / 1024) ;
  menu_par_new((*mInfo.pola)[5].txt, sk) ;

  strcpy(sk1, Current_File_Directory);
  ptrsz_lab = sk1;
  len = strlen (ptrsz_lab) ;
  if (len > 36)
  {
    st = &ptrsz_lab [len - (36 - 7)] ;
    st = strchr (st, Slash) ;
    memmove ( &ptrsz_lab [6], st, strlen (st) + 1) ; /*z '\0' na koncu*/
    ptrsz_lab [3] = '.' ;
    ptrsz_lab [4] = '.' ;
    ptrsz_lab [5] = '.' ;
    ptrsz_lab [36] = '\0' ;
  }
  menu_par_new((*mInfo.pola)[6].txt, ptrsz_lab) ;

  sprintf (sk, "%dx%d", getmaxx()+1, getmaxy()+1) ;
  menu_par_new((*mInfo.pola)[7].txt, sk) ;

  sprintf (sk, u8"%s", Czcionka_Pulpitu) ;
  menu_par_new((*mInfo.pola)[8].txt, sk) ;
}


void Change_Buffer_Size(void)
{
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];
	double d;
	double angle_l;
	long_long Buf_Mem_Size;
	char buf[100];
	int l_kr;

	Buf_Mem_Size = Get_memory_size() / 1024;

	sprintf(sk, "%ld", Buf_Mem_Size);
	if (!get_string(sk, numbers, MaxTextLen, 0, 193)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		return;
	}
	Buf_Mem_Size = (long_long)buf_ret[0];

	if (((Buf_Mem_Size * 1024) > 1048576) && ((Buf_Mem_Size * 1024) < 2048000000L))
	{
		Set_Memory_Size((unsigned long)Buf_Mem_Size * 1024);
	}
	else
	{
		ErrList(213);
		return;
	}
	
	komunikat(0);
}


void Change_Printing_Buffer_Size(void)
{
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];
	double d;
	double angle_l;
	long_long Print_Buf_Size;
	char buf[100];
	int l_kr;

	Print_Buf_Size = Bufor_Wydruku / 1024;

	sprintf(sk, "%ld", Print_Buf_Size);
	if (!get_string(sk, numbers, MaxTextLen, 0, 194)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		return;
	}
	Print_Buf_Size = (long_long)buf_ret[0];

	if (((Print_Buf_Size * 1024) >= 1048576) && ((Print_Buf_Size * 1024) <= 2048000000L))
	{
		Bufor_Wydruku=((unsigned long)Print_Buf_Size * 1024);
	}
	else
	{
		ErrList(213);
		return;
	}

	komunikat(0);
}


static void(*COMNDi[])(void) = {
        Change_Buffer_Size, nooph, nooph, Change_Printing_Buffer_Size, nooph, nooph, nooph, nooph, nooph, alfacad_logo_info,
};

void Info_Sys (void)
/*-------------*/
{
	int n;
	int x_cent, y_cent;

	demo(0);
	x_cent = (getmaxx() / 2);
	y_cent = (getmaxy() / 2) + 6 * HEIGHT;
	TTF_logo(x_cent, y_cent);
  uaktualnij_pola () ;
  Semaphore = FALSE;
  if ((n = getwsp1(&mInfo) - 1) >= 0)
  {
      demo_out(0);
      (*COMNDi[n])();    /* n=-1 -> esc */
      LASTFUN = COMNDi[n];
  }
  else demo_out(0);

}

void Info_Sys_(void)
/*-------------*/
{
	static void(*CUR)(int, int);

	CUR = MVCUR;
	MVCUR = (void (*)(int, int)) nooph;
	uaktualnij_pola();
	if (openwh(&mInfo))
	{
		demo(0);
		Get_Key();
		demo_out(0);
		closewh(&mInfo);
	}
	MVCUR = CUR;
	return;
}

void nazwa_koloru(char *ad1, char *sk)
/*-----------------------------------*/
{
 if (((LINIA *)ad1)->kolor==0) 
   {
    strcpy(sk, Colors_Name_txt[15]);
   }
    else if (((LINIA *)ad1)->kolor>15) 
     {
      sprintf (sk, u8"%#ld", ((LINIA *)ad1)->kolor) ;
     } 
       else
        {
         strcpy(sk, Colors_Name_txt[((LINIA *)ad1)->kolor - 1]);
        }
}


static void nazwa_bloku_first(char *ad1,char *sk, int *plate_hole_wall_zone)
/*-----------------------------------------------------------------------*/
{ BLOK *ptrs_block ;
  T_Desc_Ex_Block *ptrs_desc_bl ;

  *plate_hole_wall_zone=-1;
   
   strcpy(sk,u8"");
   if (((LINIA *)ad1)->blok==NoElemBlok) strcpy(sk,u8"");
     else
      {
       if (NULL != (ptrs_block = (BLOK  *)FIRSTB (ad1)))
        {
         if (ptrs_block->dlugosc_opisu_obiektu > 17)
          { 
            ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
            if (ptrs_desc_bl->sz_type [0] != '\0')
             {
              strncpy(sk,ptrs_desc_bl->sz_type,40);
             }
             else strcpy(sk,u8"");
          }
           else {
               if (ptrs_block->kod_obiektu==B_PLINE)
               {
                   strcpy(sk, u8"*PLINE");
                   if (ptrs_block->opis_obiektu [0] == PL_PLATE)
                   {
                       strcat(sk,__PLATE__);
                       *plate_hole_wall_zone=0;
                   }
                   else if (ptrs_block->opis_obiektu [0] == PL_HOLE)
                   {
                       strcat(sk,__HOLE__);
                       *plate_hole_wall_zone=1;
                   }
                   else if (ptrs_block->opis_obiektu [0] == PL_WALL) 
                   {
                       strcat(sk, __WALL__);
                       *plate_hole_wall_zone=2;
                   }
                   else if (ptrs_block->opis_obiektu [0] == PL_ZONE)
                   {
                       strcat(sk,__ZONE__);
                       *plate_hole_wall_zone=3;
                   }
               }
               else if (ptrs_block->kod_obiektu==B_HATCH) strcpy(sk, u8"*HATCH");
               else if ((ptrs_block->kod_obiektu==B_DIM) || (ptrs_block->kod_obiektu==B_DIM1) ||(ptrs_block->kod_obiektu==B_DIM2) ||(ptrs_block->kod_obiektu==B_DIM3)) strcpy(sk, u8"*DIM");
               else strcpy(sk, u8"");
         }
        }     
      } 
}      


static void nazwa_bloku(char *ad1,char *sk)
/*---------------------------------------*/
{ BLOK *ptrs_block ;
  T_Desc_Ex_Block *ptrs_desc_bl ;
   
   strcpy(sk,u8"");
   if (((LINIA *)ad1)->blok==NoElemBlok) strcpy(sk,"");
     else
      {
       if (NULL != (ptrs_block = (BLOK  *)LASTB (ad1)))
        {
         if (ptrs_block->dlugosc_opisu_obiektu > 17)
          { 
            ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
            if (ptrs_desc_bl->sz_type [0] != '\0')
             {
              strncpy(sk,ptrs_desc_bl->sz_type,40);
             }
             else strcpy(sk,u8"");
          }
          else {
             if (ptrs_block->kod_obiektu==B_PLINE)
             {
                 strcpy(sk, u8"*PLINE");
                 if (ptrs_block->opis_obiektu [0] == PL_PLATE) strcat(sk,__PLATE__);
                 else if (ptrs_block->opis_obiektu [0] == PL_HOLE) strcat(sk,__HOLE__);
                 else if (ptrs_block->opis_obiektu [0] == PL_WALL) strcat(sk,__WALL__);
                 else if (ptrs_block->opis_obiektu [0] == PL_ZONE) strcat(sk,__ZONE__);
             }
             else if (ptrs_block->kod_obiektu==B_HATCH) strcpy(sk, u8"*HATCH");
             else if ((ptrs_block->kod_obiektu==B_DIM) || (ptrs_block->kod_obiektu==B_DIM1) ||(ptrs_block->kod_obiektu==B_DIM2) ||(ptrs_block->kod_obiektu==B_DIM3)) strcpy(sk, u8"*DIM");
             else if (ptrs_block->kod_obiektu==B_WIRE) strcpy(sk, u8"*WIRE");
             else strcpy(sk, u8"");
         }
        }     
      } 
}      

void deact(void)
{
	int i;
	for (i = 0; i < mInfoAbout.max; i++) active[i] = FALSE;

}

void act(int pos)
{
	active[pos] = TRUE;
}

void inact(int pos)
{
    active[pos] = FALSE;
}

void Info_about_object(char *ad)
/*----------------------------*/
{ char sk [POLE_TXT_MAX], *sk1;
  char sk0 [POLE_TXT_MAX];
  static void (*CUR)(int ,int);
  double kat;
  double obwod, pole;
  int grubosc_l, typ_l;
  double x1, y1, x2, y2, x3, y3, x4, y4;
  double dX, dY, dZ, dl, promien, promien1, angle, angle1, angle2, width1, width2, axis1, axis2 ;
  char buf [100] ;
  PLINIA pl;
  PLINIA3D pl3D;
  int i;
  BLOK *ptrs_block ;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  char * adr_block;
  int n;
  int i_active;
  B_PCX *pcx0, b_pcx;
  PCXheader head;
  double scale_xf, scale_yf, angle_f;
  int pxl_xi, pxl_yi;
  double size_xf, size_yf;
  int no_param;
  double lx1, ly1, lx2, ly2, lx3, ly3, lx4, ly4;
  int ob_no;
  char *adbp, *adbk, *adbp0;
  BOOL found;
  int i_typ;
  BOOL Wielokat=FALSE;
  char *translucency_ptr;
  unsigned char translucency;
  int transluc;
  float d_trans=12.75;
  int p_h_w_z;

  deact();

  strcpy(sk,u8"");

  memmove(&edit_param_no, &edit_param_no0, sizeof(edit_param_no));

  for (i=iObject; i<iOuterBlockName; i++) {  menu_par_new((*mInfoAbout.pola)[i].txt, sk) ;}
  
  komunikat(111);


  if (((NAGLOWEK*)ad)->obiekt==Olinia)
   {
     strcpy(sk, objects[0]);
	 menu_par_new((*mInfoAbout.pola)[iObject].txt, sk) ;
	 act(iObject);

     sprintf (sk, u8"%#ld", ((LINIA *)ad)->warstwa + 1) ;
     strcat(sk,u8" '");
     strcat(sk,Layers[((LINIA *)ad)->warstwa].name);
     strcat(sk,u8"'");
	 menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
	 act(iLayer);

     nazwa_koloru(ad,sk);
     LiniaG.kolor=((LINIA *)ad)->kolor;
	 menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
     act(iColor);

     grubosc_l=(((LINIA *)ad)->typ & 224 ) / 32 ;
     typ_l=(((LINIA *)ad)->typ & 31 );
     strcpy(sk,view_type_tab[typ_l]);
	 menu_par_new((*mInfoAbout.pola)[iLType].txt, sk) ;
	 act(iLType);

     strcpy(sk,view_width_tab1[grubosc_l]);
	 menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
	 act(iLWidth);

     if (((LINIA *)ad)->obiektt2>4)
     {
        strcpy(sk,view_edge_tab[((LINIA *)ad)->obiektt2]);
        menu_par_new((*mInfoAbout.pola)[iEdge].txt, sk) ;
        (*mInfoAbout.pola)[iEdge].iconno=edge_icon[((T_Point *)ad)->obiektt2];
        act(iEdge);

        strcpy(sk,view_edgeinverted_tab[((LINIA *)ad)->obiektt3]);
        menu_par_new((*mInfoAbout.pola)[iEdgeInverted].txt, sk) ;
        act(iEdgeInverted);
     }

     lx1 =  milimetryobxl(((LINIA *)ad)->x1, (((LINIA *)ad)->y1)) ;
     sprintf (sk, "%13.9lf", lx1) ;
	 menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
	 act(iX1);

     ly1 =   milimetryobyl(((LINIA *)ad)->x1, (((LINIA *)ad)->y1)) ;
     sprintf (sk, "%13.9lf", ly1) ;
	 menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
	 act(iY1);

     lx2 =  milimetryobxl(((LINIA *)ad)->x2, (((LINIA *)ad)->y2));
     sprintf (sk, "%13.9lf", lx2) ;
	 menu_par_new((*mInfoAbout.pola)[iX2].txt, sk) ;
	 act(iX2);

     ly2 = milimetryobyl(((LINIA *)ad)->x2, (((LINIA *)ad)->y2));
     sprintf (sk, "%13.9lf", ly2) ;
	 menu_par_new((*mInfoAbout.pola)[iY2].txt, sk) ;
	 act(iY2);
     
     parametry_lini(ad, &pl) ;
     dX = lx2 - lx1 ;
     dY = ly2 - ly1 ;
     dl = milimetryob (pl.dl) ;
     angle = pl.kat - get_angle_l();
     sprintf (sk, "%8.7lf", angle);
     menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
	 act(iAngle1);

     sprintf (sk, "%13.9lf", dl);
     menu_par_new((*mInfoAbout.pola)[iLenght].txt, sk) ;
	 act(iLenght);
     
     sprintf (sk, "%13.9lf", fabs(dX));
     menu_par_new((*mInfoAbout.pola)[iDX].txt, sk) ;
	 act(iDX);
          
     sprintf (sk, "%13.9lf", fabs(dY));
     menu_par_new((*mInfoAbout.pola)[iDY].txt, sk) ;
	 act(iDY);

     nazwa_bloku_first(ad,&sk, &p_h_w_z);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
	 act(iInnerBlockName);
     if (p_h_w_z==3)   //not for zone
     {
         inact(iEdge);
         inact(iEdgeInverted);
     }

     nazwa_bloku(ad,&sk);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
	 act(iOuterBlockName);
   }
   else if (((NAGLOWEK*)ad)->obiekt==OdBLOK)
    {
        if (((BLOK *)ad)->kod_obiektu==B_HATCH) ob_no=13;
        else if (((BLOK *)ad)->kod_obiektu==B_PLINE)
        {
            if (((BLOK *)ad)->opis_obiektu[0]==PL_TRACE) ob_no=12;
            else ob_no=11;
        }
        strcpy(sk, objects[ob_no]);

        if (((BLOK *)ad)->kod_obiektu==B_PLINE) {
            if (((BLOK *)ad)->opis_obiektu[0] == PL_PLATE) strcat(sk, __PLATE__);
            else if (((BLOK *)ad)->opis_obiektu[0] == PL_HOLE) strcat(sk, __HOLE__);
            else if (((BLOK *)ad)->opis_obiektu[0] == PL_WALL) strcat(sk, __WALL__);
            else if (((BLOK *)ad)->opis_obiektu[0] == PL_ZONE) strcat(sk, __ZONE__);
        }

        menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
        act(iObject);

        adbp = (char *)ad + sizeof(NAGLOWEK)+B3+((BLOK*)ad)->dlugosc_opisu_obiektu;
        adbk = (char *)ad + sizeof(NAGLOWEK) + ((BLOK*)ad)->n - 1;
        found=FALSE;

        while (adbp<adbk)
        {
            switch (((NAGLOWEK*)adbp)->obiekt)
            {
                case Olinia:
                    if (((LINIA *)adbp)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                    {
                        i_typ = ((LINIA *) adbp)->typ;
                        typ_l = (i_typ & 31);
                        grubosc_l = (i_typ & 224) / 32;
                        found=TRUE;
                    }
                    break;
                case Oluk:
                    if (((LUK *)adbp)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                    {
                        i_typ = ((LUK *) adbp)->typ;
                        typ_l = (i_typ & 31);
                        grubosc_l = (i_typ & 224) / 32;
                        found=TRUE;
                    }
                case Owwielokat:
                    grubosc_l = ((WIELOKAT *)adbp)->empty_typ;
                    if (grubosc_l==0) grubosc_l=5;
                    else grubosc_l--;
                    typ_l=0;
                    Wielokat=TRUE;
                    found=TRUE;
                    break;
                case Osolidarc:
                    grubosc_l = ((SOLIDARC *)adbp)->empty_typ;
                    if (grubosc_l==0) grubosc_l=5;
                    else grubosc_l--;
                    typ_l=0;
                    Wielokat=TRUE;
                    found=TRUE;
                    break;
                case Ospline:
                    grubosc_l = ((SPLINE *)adbp)->typ;
                    typ_l = (i_typ & 31);
                    grubosc_l = (i_typ & 224) / 32;
                    found=TRUE;
                    break;
                default:
                    break;
            }
            if (found==TRUE) break;
            adbp0 = adbp; //last object
            adbp+=sizeof(NAGLOWEK) + ((NAGLOWEK*)adbp)->n;
        }

        if (found==FALSE) {
            adbp=adbp0;
            if ((((NAGLOWEK*)adbp)->obiekt==Olinia) && (((LINIA *)adbp)->typ == HATCH_OUTLINE_TYPE))
            {
                i_typ = ((LINIA *) adbp)->typ;
                typ_l = (i_typ & 31);
                grubosc_l = (i_typ & 224) / 32;
            }
            else if ((((NAGLOWEK*)adbp)->obiekt==Oluk) && (((LUK *)adbp)->typ == HATCH_OUTLINE_TYPE))
            {
                i_typ = ((LUK *) adbp)->typ;
                typ_l = (i_typ & 31);
                grubosc_l = (i_typ & 224) / 32;
            }
            else {
                komunikat(0);
                return;
            }
        }

        sprintf (sk, u8"%#ld", ((LINIA *)adbp)->warstwa + 1) ;
        strcat(sk,u8" '");
        strcat(sk,Layers[((LINIA *)adbp)->warstwa].name);
        strcat(sk,u8"'");
        menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
        act(1);

        nazwa_koloru(adbp,sk);
        LiniaG.kolor=((LINIA *)adbp)->kolor;
        menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
        act(iColor);

        if (Wielokat==FALSE) {
            strcpy(sk, view_type_tab[typ_l]);
            menu_par_new((*mInfoAbout.pola)[iLType].txt, sk);
           act(iLType);
        }

        strcpy(sk,view_width_tab1[grubosc_l]);
        menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
        act(iLWidth);

        nazwa_bloku_first(ad,&sk, &p_h_w_z);
        sk[40]='\0';
        menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
        act(iInnerBlockName);

        nazwa_bloku(ad,&sk);
        sk[40]='\0';
        menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
        act(iOuterBlockName);
    }
  else if (((NAGLOWEK*)ad)->obiekt==Otekst)
   {
     strcpy(sk, objects[2]);
	 
     menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
	 act(iObject);

     sprintf (sk, "%#ld", ((TEXT *)ad)->warstwa + 1) ;
     strcat(sk," '");
     strcat(sk,Layers[((TEXT *)ad)->warstwa].name);
     strcat(sk,"'");
     menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
	 act(1);

     nazwa_koloru(ad,&sk);
     LiniaG.kolor=((TEXT *)ad)->kolor;
     menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
	 act(iColor);

     lx1 =  milimetryobxl(((TEXT *)ad)->x, (((TEXT *)ad)->y)) ;
     sprintf (sk, "%13.9lf", lx1) ;
     menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
	 act(iX1);

     ly1 =  milimetryobyl(((TEXT *)ad)->x, (((TEXT *)ad)->y)) ;
     sprintf (sk, "%13.9lf", ly1) ;
     menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
	 act(iY1);

     sprintf (sk, "%8.7lf", ((((TEXT *)ad)->kat)/Pi2)*360.0 - get_angle_l());
     menu_par_new((*mInfoAbout.pola)[iAngle].txt, sk) ;
	 act(iAngle);

     strcpy(sk,PTRS__Text_Style[((TEXT *)ad)->czcionka]->font_name);
     menu_par_new((*mInfoAbout.pola)[iFont].txt, sk) ;
	 act(iFont);

     strcpy(sk, tab_typ_tekstu[((TEXT *)ad)->typ]);
     menu_par_new((*mInfoAbout.pola)[iFType].txt, sk) ;     
	 act(iFType);

     if (((TEXT *)ad)->ukryty==0) strcpy(sk,"N"); else strcpy(sk,"T");
     menu_par_new((*mInfoAbout.pola)[iHidden].txt, sk) ;
	 act(iHidden);

     strcpy(sk, tab_justowanie[((TEXT *)ad)->justowanie]);
     menu_par_new((*mInfoAbout.pola)[iAllign].txt, sk) ; 
	 act(iAllign);

     sprintf (sk, "%6.3lf", ((TEXT *)ad)->wysokosc );
     menu_par_new((*mInfoAbout.pola)[iHeight].txt, sk) ;
	 act(iHeight);

     sprintf (sk, "%6.3lf", ((TEXT *)ad)->width_factor );
     menu_par_new((*mInfoAbout.pola)[iWidthFactor].txt, sk) ;
	 act(iWidthFactor);

     if (((TEXT *)ad)->italics==0) strcpy(sk,"N"); else strcpy(sk,"T");
     menu_par_new((*mInfoAbout.pola)[iItalic].txt, sk) ;
	 act(iItalic);

     if (((TEXT *)ad)->bold==0) strcpy(sk,"N"); else strcpy(sk,"T");
     menu_par_new((*mInfoAbout.pola)[iBold].txt, sk) ;
     act(iBold);

     if (((TEXT *)ad)->underline==0) strcpy(sk,"N"); else strcpy(sk,"T");
     menu_par_new((*mInfoAbout.pola)[iUnderlined].txt, sk) ;
     act(iUnderlined);

     if (((TEXT *)ad)->multiline==1) {
         sprintf(sk, "%.1f", (float) (((TEXT *) ad)->spacing / 10.0 + 1.0));
         menu_par_new((*mInfoAbout.pola)[iLineSpacing].txt, sk);
         act(iLineSpacing);
     }

     nazwa_bloku_first(ad,&sk, &p_h_w_z);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
	 act(iInnerBlockName);

     nazwa_bloku(ad,&sk);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
	 act(iOuterBlockName);

   }
  else if (((NAGLOWEK*)ad)->obiekt==Oluk)
   {
     strcpy(sk, objects[3]);
     menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
	 act(iObject);

     sprintf (sk, "%#ld", ((LUK *)ad)->warstwa + 1) ;
     strcat(sk," '");
     strcat(sk,Layers[((LUK *)ad)->warstwa].name);
     strcat(sk,"'");
     menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;     
	 act(1);

     nazwa_koloru(ad,&sk);
     LiniaG.kolor=((LUK *)ad)->kolor;
     menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
	 act(iColor);

     grubosc_l=(((LUK *)ad)->typ & 224 ) / 32 ;
     typ_l=(((LUK *)ad)->typ & 31 );
     strcpy(sk,view_type_tab[typ_l]);
     menu_par_new((*mInfoAbout.pola)[iLType].txt, sk) ;
	 act(iLType);

     strcpy(sk,view_width_tab1[grubosc_l]);
     menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
	 act(iLWidth);

     if (((LINIA *)ad)->obiektt2>4)
     {
         strcpy(sk,view_edge_tab[((LINIA *)ad)->obiektt2]);
         menu_par_new((*mInfoAbout.pola)[iEdge].txt, sk) ;
         (*mInfoAbout.pola)[iEdge].iconno=edge_icon[((T_Point *)ad)->obiektt2];
         act(iEdge);

         strcpy(sk,view_edgeinverted_tab[((LINIA *)ad)->obiektt3]);
         menu_par_new((*mInfoAbout.pola)[iEdgeInverted].txt, sk) ;
         act(iEdgeInverted);
     }

     lx1 =  milimetryobxl(((LUK *)ad)->x, (((LUK *)ad)->y)) ;
     sprintf (sk, "%13.9lf", lx1) ;
     menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
	 act(iX1);

     ly1 =  milimetryobyl(((LUK *)ad)->x, (((LUK *)ad)->y)) ;
     sprintf (sk, "%13.9lf", ly1) ;
     menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
	 act(iY1);

     promien=milimetryob(((LUK *)ad)->r);     
     sprintf (sk, "%13.9lf", promien) ;
     menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
	 act(iRadius);

     angle=((((LUK *)ad)->kat1)/Pi2)*360.0 - get_angle_l();
     sprintf (sk, "%13.9lf", angle) ;
     menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
	 act(iAngle1);

     angle1=((((LUK *)ad)->kat2)/Pi2)*360.0 - get_angle_l();
     if (angle1 < angle) angle1+=360.0;
     sprintf (sk, "%13.9lf", angle1) ;
     menu_par_new((*mInfoAbout.pola)[iAngle2].txt, sk) ;
	 act(iAngle2);

     dl=Pi2*promien*(angle1-angle)/360.0;
     sprintf (sk, "%13.9lf", dl);
     menu_par_new((*mInfoAbout.pola)[iLenght].txt, sk) ;
	 act(iLenght);

     nazwa_bloku_first(ad,&sk, &p_h_w_z);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
	 act(iInnerBlockName);
     if (p_h_w_z==3)  //not for zone
       {
           inact(iEdge);
           inact(iEdgeInverted);
       }

     nazwa_bloku(ad,&sk);    
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
	 act(iOuterBlockName);

   }
  else if (((NAGLOWEK*)ad)->obiekt==Osolidarc)
  {
      strcpy(sk, objects[17]);
      menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
      act(iObject);

      sprintf (sk, "%#ld", ((SOLIDARC *)ad)->warstwa + 1) ;
      strcat(sk," '");
      strcat(sk,Layers[((SOLIDARC *)ad)->warstwa].name);
      strcat(sk,"'");
      menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
      act(1);

      nazwa_koloru(ad,&sk);
      LiniaG.kolor=((SOLIDARC *)ad)->kolor;
      menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
      act(iColor);

      grubosc_l=((SOLIDARC *)ad)->empty_typ - 1;
      if (grubosc_l==-1)
          strcpy(sk, _FILLING_);
      else strcpy(sk,view_width_tab1[grubosc_l]);
      menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
      act(iLWidth);

      lx1 =  milimetryobxl(((SOLIDARC *)ad)->x, (((SOLIDARC *)ad)->y)) ;
      sprintf (sk, "%13.9lf", lx1) ;
      menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
      act(iX1);

      ly1 =  milimetryobyl(((SOLIDARC *)ad)->x, (((SOLIDARC *)ad)->y)) ;
      sprintf (sk, "%13.9lf", ly1) ;
      menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
      act(iY1);

      promien=milimetryob(((SOLIDARC *)ad)->r);
      sprintf (sk, "%13.9lf", promien) ;
      menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
      act(iRadius);

      angle=((((SOLIDARC *)ad)->kat1)/Pi2)*360.0 - get_angle_l();
      sprintf (sk, "%13.9lf", angle) ;
      menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
      act(iAngle1);

      angle1=((((SOLIDARC *)ad)->kat2)/Pi2)*360.0 - get_angle_l();
      if (angle1 < angle) angle1+=360.0;
      sprintf (sk, "%13.9lf", angle1) ;
      menu_par_new((*mInfoAbout.pola)[iAngle2].txt, sk) ;
      act(iAngle2);

      if (((SOLIDARC *) ad)->reversed) {
          width1 = milimetryob(((SOLIDARC *) ad)->width1);
          width2 = milimetryob(((SOLIDARC *) ad)->width2);
          axis1 = milimetryob(((SOLIDARC *) ad)->axis1);
          axis2 = milimetryob(((SOLIDARC *) ad)->axis2);
      }
      else {
          width1 = milimetryob(((SOLIDARC *) ad)->width2);
          width2 = milimetryob(((SOLIDARC *) ad)->width1);
          axis1 = milimetryob(((SOLIDARC *) ad)->axis2);
          axis2 = milimetryob(((SOLIDARC *) ad)->axis1);
      }

      sprintf (sk, "%13.9lf", width1) ;
      menu_par_new((*mInfoAbout.pola)[iWidth1].txt, sk) ;
      act(iWidth1);

      sprintf (sk, "%13.9lf", width2) ;
      menu_par_new((*mInfoAbout.pola)[iWidth2].txt, sk) ;
      act(iWidth2);

      sprintf (sk, "%13.9lf", axis1) ;
      menu_par_new((*mInfoAbout.pola)[iAxisOffset1].txt, sk) ;
      act(iAxisOffset1);

      sprintf (sk, "%13.9lf", axis2) ;
      menu_par_new((*mInfoAbout.pola)[iAxisOffset2].txt, sk) ;
      act(iAxisOffset2);

      sprintf (sk, "%d%%", (int)((float)(((SOLIDARC *)ad)->translucency)*100.0/255.0+0.5)) ;
      //transluc = (int)((float)(((ELLIPSE *)ad)->transulcency)/ d_trans +0.5);
      //sprintf(sk, "%d%%", transluc * 5);

      menu_par_new((*mInfoAbout.pola)[iOpacity].txt, sk) ;
      act(iOpacity);

      dl=Pi2*promien*(angle1-angle)/360.0;
      sprintf (sk, "%13.9lf", dl);
      menu_par_new((*mInfoAbout.pola)[iLenght].txt, sk) ;
      act(iLenght);

      nazwa_bloku_first(ad,&sk, &p_h_w_z);
      sk[40]='\0';
      menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
      act(iInnerBlockName);

      nazwa_bloku(ad,&sk);
      sk[40]='\0';
      menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
      act(iOuterBlockName);

  }
  else if (((NAGLOWEK*)ad)->obiekt==Oellipticalarc)
  {
      strcpy(sk, objects[14]);
      menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
      act(iObject);

      sprintf (sk, "%#ld", ((ELLIPTICALARC *)ad)->warstwa + 1) ;
      strcat(sk," '");
      strcat(sk,Layers[((ELLIPTICALARC *)ad)->warstwa].name);
      strcat(sk,"'");
      menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
      act(1);

      nazwa_koloru(ad,&sk);
      LiniaG.kolor=((ELLIPTICALARC *)ad)->kolor;
      menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
      act(iColor);

      grubosc_l=(((ELLIPTICALARC *)ad)->typ & 224 ) / 32 ;
      typ_l=(((ELLIPTICALARC *)ad)->typ & 31 );
      strcpy(sk,view_type_tab[typ_l]);
      menu_par_new((*mInfoAbout.pola)[iLType].txt, sk) ;
      act(iLType);

      strcpy(sk,view_width_tab1[grubosc_l]);
      menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
      act(iLWidth);

      lx1 =  milimetryobxl(((ELLIPTICALARC *)ad)->x, (((ELLIPTICALARC *)ad)->y)) ;
      sprintf (sk, "%13.9lf", lx1) ;
      menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
      act(iX1);

      ly1 =  milimetryobyl(((ELLIPTICALARC *)ad)->x, (((ELLIPTICALARC *)ad)->y)) ;
      sprintf (sk, "%13.9lf", ly1) ;
      menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
      act(iY1);

      promien=milimetryob(((ELLIPTICALARC *)ad)->rx);
      sprintf (sk, "%13.9lf", promien) ;
      menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
      act(iRadius);

      promien1=milimetryob(((ELLIPTICALARC *)ad)->ry);
      sprintf (sk, "%13.9lf", promien1) ;
      menu_par_new((*mInfoAbout.pola)[iRadiusY].txt, sk) ;
      act(iRadiusY);

      angle=((((ELLIPTICALARC *)ad)->angle)/Pi2)*360.0 - get_angle_l();
      sprintf (sk, "%13.9lf", angle) ;
      menu_par_new((*mInfoAbout.pola)[iAngle].txt, sk) ;
      act(iAngle);

      angle1=((((ELLIPTICALARC *)ad)->kat1)/Pi2)*360.0 - get_angle_l();
      sprintf (sk, "%13.9lf", angle1) ;
      menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
      act(iAngle1);

      angle2=((((ELLIPTICALARC *)ad)->kat2)/Pi2)*360.0 - get_angle_l();
      sprintf (sk, "%13.9lf", angle2) ;
      menu_par_new((*mInfoAbout.pola)[iAngle2].txt, sk) ;
      act(iAngle2);

      nazwa_bloku_first(ad,&sk, &p_h_w_z);
      sk[40]='\0';
      menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
      act(iInnerBlockName);

      nazwa_bloku(ad,&sk);
      sk[40]='\0';
      menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
      act(iOuterBlockName);

  }
  else if (((NAGLOWEK*)ad)->obiekt==Ookrag)
   {
     strcpy(sk, objects[4]);
     menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
	 act(iObject);

     sprintf (sk, "%#ld", ((OKRAG *)ad)->warstwa + 1) ;
     strcat(sk," '");
     strcat(sk,Layers[((OKRAG *)ad)->warstwa].name);
     strcat(sk,"'");
     menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
	 act(1);

     nazwa_koloru(ad,&sk);
     LiniaG.kolor=((OKRAG *)ad)->kolor;
     menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
	 act(iColor);

     grubosc_l=(((OKRAG *)ad)->typ & 224 ) / 32 ;
     typ_l=(((OKRAG *)ad)->typ & 31 );
     strcpy(sk,view_type_tab[typ_l]);
     menu_par_new((*mInfoAbout.pola)[iLType].txt, sk) ;
	 act(iLType);

     strcpy(sk,view_width_tab1[grubosc_l]);
     menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
	 act(iLWidth);

     lx1 =  milimetryobxl(((OKRAG *)ad)->x, (((OKRAG *)ad)->y)) ;
     sprintf (sk, "%13.9lf", lx1) ;
     menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
	 act(iX1);

     ly1 =  milimetryobyl(((OKRAG *)ad)->x, (((OKRAG *)ad)->y)) ;
     sprintf (sk, "%13.9lf", ly1) ;
     menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
	 act(iY1);

     promien=milimetryob(((OKRAG *)ad)->r);     
     sprintf (sk, "%13.9lf", promien) ;
     menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
	 act(iRadius);

     dl=Pi2*promien;
     sprintf (sk, "%13.9lf", dl);
     menu_par_new((*mInfoAbout.pola)[iLenght].txt, sk) ;
	 act(iLenght);

     dl=(Pi2/2)*promien*promien;
     sprintf (sk, "%13.9lf", dl);
     menu_par_new((*mInfoAbout.pola)[iArea].txt, sk) ;
	 act(iArea);

     nazwa_bloku_first(ad,&sk, &p_h_w_z);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
	 act(iInnerBlockName);

     nazwa_bloku(ad,&sk); 
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
	 act(iOuterBlockName);

   }
  else if (((NAGLOWEK*)ad)->obiekt==Oellipse)
  {
      strcpy(sk, objects[15]);
      menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
      act(iObject);

      sprintf (sk, "%#ld", ((ELLIPSE *)ad)->warstwa + 1) ;
      strcat(sk," '");
      strcat(sk,Layers[((ELLIPSE *)ad)->warstwa].name);
      strcat(sk,"'");
      menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
      act(1);

      nazwa_koloru(ad,&sk);
      LiniaG.kolor=((ELLIPSE *)ad)->kolor;
      menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
      act(iColor);

      grubosc_l=(((ELLIPSE *)ad)->typ & 224 ) / 32 ;
      typ_l=(((ELLIPSE *)ad)->typ & 31 );
      strcpy(sk,view_type_tab[typ_l]);
      menu_par_new((*mInfoAbout.pola)[iLType].txt, sk) ;
      act(iLType);

      strcpy(sk,view_width_tab1[grubosc_l]);
      menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
      act(iLWidth);

      lx1 =  milimetryobxl(((ELLIPSE *)ad)->x, (((OKRAG *)ad)->y)) ;
      sprintf (sk, "%13.9lf", lx1) ;
      menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
      act(iX1);

      ly1 =  milimetryobyl(((ELLIPSE *)ad)->x, (((OKRAG *)ad)->y)) ;
      sprintf (sk, "%13.9lf", ly1) ;
      menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
      act(iY1);

      promien=milimetryob(((ELLIPSE *)ad)->rx);
      sprintf (sk, "%13.9lf", promien) ;
      menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
      act(iRadius);

      promien1=milimetryob(((ELLIPSE *)ad)->ry);
      sprintf (sk, "%13.9lf", promien1) ;
      menu_par_new((*mInfoAbout.pola)[iRadiusY].txt, sk) ;
      act(iRadiusY);

      angle=((((ELLIPSE *)ad)->angle)/Pi2)*360.0 - get_angle_l();
      sprintf (sk, "%13.9lf", angle) ;
      menu_par_new((*mInfoAbout.pola)[iAngle].txt, sk) ;
      act(iAngle);

      /*
      dl=Pi2*promien;
      sprintf (sk, "%13.9lf", dl);
      menu_par_new((*mInfoAbout.pola)[iLenght].txt, sk) ;
      act(iLenght);

      dl=(Pi2/2)*promien*promien;
      sprintf (sk, "%13.9lf", dl);
      menu_par_new((*mInfoAbout.pola)[iArea].txt, sk) ;
      act(iArea);
       */

      nazwa_bloku_first(ad,&sk, &p_h_w_z);
      sk[40]='\0';
      menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
      act(iInnerBlockName);

      nazwa_bloku(ad,&sk);
      sk[40]='\0';
      menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
      act(iOuterBlockName);

  }
  else if (((NAGLOWEK*)ad)->obiekt==Okolo)
   {
     strcpy(sk, objects[5]);
     menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
	 act(iObject);

     sprintf (sk, "%#ld", ((OKRAG *)ad)->warstwa + 1) ;
     strcat(sk," '");
     strcat(sk,Layers[((OKRAG *)ad)->warstwa].name);
     strcat(sk,"'");
     menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
	 act(1);

     nazwa_koloru(ad,&sk);
     LiniaG.kolor=((OKRAG *)ad)->kolor;
     menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
	 act(iColor);

     lx1 =  milimetryobxl(((OKRAG *)ad)->x, (((OKRAG *)ad)->y)) ;
     sprintf (sk, "%13.9lf", lx1) ;
     menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
	 act(iX1);

     ly1 =  milimetryobxl(((OKRAG *)ad)->x, (((OKRAG *)ad)->y)) ;
     sprintf (sk, "%13.9lf", ly1) ;
     menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
	 act(iY1);

     promien=milimetryob(((OKRAG *)ad)->r);     
     sprintf (sk, "%13.9lf", promien) ;
     menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
	 act(iRadius);

     dl=Pi2*promien;
     sprintf (sk, "%13.9lf", dl);
     menu_par_new((*mInfoAbout.pola)[iLenght].txt, sk) ;
	 act(iLenght);

     dl=(Pi2/2)*promien*promien;
     sprintf (sk, "%13.9lf", dl);
     menu_par_new((*mInfoAbout.pola)[iArea].txt, sk) ;
	 act(iArea);

     nazwa_bloku_first(ad,&sk, &p_h_w_z);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
	 act(iInnerBlockName);

     nazwa_bloku(ad,&sk);   
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
	 act(iOuterBlockName);

   }
  else if (((NAGLOWEK*)ad)->obiekt==Ofilledellipse)
  {
      strcpy(sk, objects[16]);
      menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
      act(iObject);

      sprintf (sk, "%#ld", ((ELLIPSE *)ad)->warstwa + 1) ;
      strcat(sk," '");
      strcat(sk,Layers[((ELLIPSE *)ad)->warstwa].name);
      strcat(sk,"'");
      menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
      act(1);

      nazwa_koloru(ad,&sk);
      LiniaG.kolor=((ELLIPSE *)ad)->kolor;
      menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
      act(iColor);

      lx1 =  milimetryobxl(((ELLIPSE *)ad)->x, (((ELLIPSE *)ad)->y)) ;
      sprintf (sk, "%13.9lf", lx1) ;
      menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
      act(iX1);

      ly1 =  milimetryobxl(((ELLIPSE *)ad)->x, (((ELLIPSE *)ad)->y)) ;
      sprintf (sk, "%13.9lf", ly1) ;
      menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
      act(iY1);

      promien=milimetryob(((ELLIPSE *)ad)->rx);
      sprintf (sk, "%13.9lf", promien) ;
      menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
      act(iRadius);

      promien1=milimetryob(((ELLIPSE *)ad)->ry);
      sprintf (sk, "%13.9lf", promien1) ;
      menu_par_new((*mInfoAbout.pola)[iRadiusY].txt, sk) ;
      act(iRadiusY);

      angle=((((ELLIPSE *)ad)->angle)/Pi2)*360.0 - get_angle_l();
      sprintf (sk, "%13.9lf", angle) ;
      menu_par_new((*mInfoAbout.pola)[iAngle].txt, sk) ;
      act(iAngle);

      sprintf (sk, "%d%%", (int)((float)(((ELLIPSE *)ad)->translucency)*100.0/255.0+0.5)) ;

      menu_par_new((*mInfoAbout.pola)[iOpacity].txt, sk) ;
      act(iOpacity);

      nazwa_bloku_first(ad,&sk, &p_h_w_z);
      sk[40]='\0';
      menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
      act(iInnerBlockName);

      nazwa_bloku(ad,&sk);
      sk[40]='\0';
      menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
      act(iOuterBlockName);

  }
  else if (((NAGLOWEK*)ad)->obiekt==Owwielokat)
   {
     strcpy(sk, objects[6]);
     menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
	 act(iObject);

     sprintf (sk, "%#ld", ((WIELOKAT *)ad)->warstwa + 1) ;
     strcat(sk," '");
     strcat(sk,Layers[((WIELOKAT *)ad)->warstwa].name);
     strcat(sk,"'");
     menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
	 act(1);

     nazwa_koloru(ad,&sk);
     LiniaG.kolor=((WIELOKAT *)ad)->kolor;
     menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
	 act(iColor);

     grubosc_l=((WIELOKAT *)ad)->empty_typ - 1;
     if (grubosc_l==-1) 
		 strcpy(sk, _FILLING_);
     else strcpy(sk,view_width_tab1[grubosc_l]);
     menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
	 act(iLWidth);

     lx1 =  milimetryobxl(((WIELOKAT *)ad)->xy[0], ((WIELOKAT *)ad)->xy[1]) ;
     sprintf (sk, "%13.9lf", lx1) ;
     menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
	 act(iX1);

     ly1 =  milimetryobyl(((WIELOKAT *)ad)->xy[0], ((WIELOKAT *)ad)->xy[1]) ;
     sprintf (sk, "%13.9lf", ly1) ;
     menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
	 act(iY1);

     lx2 =  milimetryobxl(((WIELOKAT *)ad)->xy[2], ((WIELOKAT *)ad)->xy[3]) ;
     sprintf (sk, "%13.9lf", lx2) ;
     menu_par_new((*mInfoAbout.pola)[iX2].txt, sk) ;
	 act(iX2);

     ly2 =  milimetryobyl(((WIELOKAT *)ad)->xy[2], ((WIELOKAT *)ad)->xy[3]) ;
     sprintf (sk, "%13.9lf", ly2) ;
     menu_par_new((*mInfoAbout.pola)[iY2].txt, sk) ;
	 act(iY2);

     lx3 =  milimetryobxl(((WIELOKAT *)ad)->xy[4], ((WIELOKAT *)ad)->xy[5]) ;
     sprintf (sk, "%13.9lf", lx3) ;
     menu_par_new((*mInfoAbout.pola)[iX3].txt, sk) ;
	 act(iX3);

     ly3 =  milimetryobyl(((WIELOKAT *)ad)->xy[4], ((WIELOKAT *)ad)->xy[5]) ;
     sprintf (sk, "%13.9lf", ly3) ;
     menu_par_new((*mInfoAbout.pola)[iY3].txt, sk) ;
	 act(iY3);

     if (((WIELOKAT *)ad)->lp==8)
      {
       lx4 =  milimetryobxl(((WIELOKAT *)ad)->xy[6], ((WIELOKAT *)ad)->xy[7]) ;
       sprintf (sk, "%13.9lf", lx4) ;
       menu_par_new((*mInfoAbout.pola)[iX4].txt, sk) ;
	   act(iX4);

       ly4 =  milimetryobyl(((WIELOKAT *)ad)->xy[6], ((WIELOKAT *)ad)->xy[7]) ;
       sprintf (sk, "%13.9lf", ly4) ;
       menu_par_new((*mInfoAbout.pola)[iY4].txt, sk) ;
	   act(iY4);

      }

     if ((((WIELOKAT *)ad)->empty_typ==0) && (((WIELOKAT *)ad)->pattern==0))
     {
         if (((WIELOKAT *)ad)->translucent==1)
         {
             translucency_ptr = ((WIELOKAT *)ad)->xy;
             translucency_ptr += (((WIELOKAT *)ad)->lp * sizeof(float));
             memmove(&translucency, translucency_ptr, sizeof(unsigned char));
         }
         else translucency=255;

         transluc = (int)((float)translucency / d_trans +0.5);
         sprintf(sk, "%d%%", transluc * 5);

         menu_par_new((*mInfoAbout.pola)[iOpacity].txt, sk) ;
         act(iOpacity);
     }

     nazwa_bloku_first(ad,&sk, &p_h_w_z);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
	 act(iInnerBlockName);

     nazwa_bloku(ad,&sk);    
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
	 act(iOuterBlockName);
   }
  else if (((NAGLOWEK*)ad)->obiekt == Ospline)
   {
   strcpy(sk, objects[9]);
   menu_par_new((*mInfoAbout.pola)[0].txt, sk);
   act(iObject);

   sprintf(sk, "%#ld", ((SPLINE *)ad)->warstwa + 1);
   strcat(sk, " '");
   strcat(sk, Layers[((SPLINE *)ad)->warstwa].name);
   strcat(sk, "'");
   menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk);
   act(1);

   nazwa_koloru(ad, &sk);
   LiniaG.kolor=((SPLINE *)ad)->kolor;
   menu_par_new((*mInfoAbout.pola)[iColor].txt, sk);
   act(iColor);

   grubosc_l = (((SPLINE *)ad)->typ & 224) / 32;
   typ_l = (((SPLINE *)ad)->typ & 31);
   strcpy(sk, view_type_tab[typ_l]);
   menu_par_new((*mInfoAbout.pola)[iLType].txt, sk);
   act(iLType);

   strcpy(sk, view_width_tab1[grubosc_l]);
   menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk);
   act(iLWidth);

   lx1 =  milimetryobxl(((SPLINE *)ad)->xy[0], ((SPLINE *)ad)->xy[1]);
   sprintf(sk, "%13.9lf", lx1);
   menu_par_new((*mInfoAbout.pola)[iX1].txt, sk);
   act(iX1);

   ly1 =  milimetryobyl(((SPLINE *)ad)->xy[0], ((SPLINE *)ad)->xy[1]);
   sprintf(sk, "%13.9lf", ly1);
   menu_par_new((*mInfoAbout.pola)[iY1].txt, sk);
   act(iY1);

   lx2 =  milimetryobxl(((SPLINE *)ad)->xy[2], ((SPLINE *)ad)->xy[3]) ;
   sprintf(sk, "%13.9lf", lx2);
   menu_par_new((*mInfoAbout.pola)[iX2].txt, sk);
   act(iX2);

   ly2 =  milimetryobyl(((SPLINE *)ad)->xy[2], ((SPLINE *)ad)->xy[3]) ;
   sprintf(sk, "%13.9lf", ly2);
   menu_par_new((*mInfoAbout.pola)[iY2].txt, sk);
   act(iY2);

   lx3 =  milimetryobxl(((SPLINE *)ad)->xy[4], ((SPLINE *)ad)->xy[5]) ;
   sprintf(sk, "%13.9lf", lx3);
   menu_par_new((*mInfoAbout.pola)[iX3].txt, sk);
   act(iX3);

   ly3 =  milimetryobyl(((SPLINE *)ad)->xy[4], ((SPLINE *)ad)->xy[5]) ;
   sprintf(sk, "%13.9lf", ly3);
   menu_par_new((*mInfoAbout.pola)[iY3].txt, sk);
   act(iY3);

   if (((SPLINE *)ad)->lp > 6)
   {
       lx4 =  milimetryobxl(((SPLINE *)ad)->xy[6], ((SPLINE *)ad)->xy[7]) ;
	   sprintf(sk, "%13.9lf", lx4);
	   menu_par_new((*mInfoAbout.pola)[iX4].txt, sk);
	   act(iX4);

       ly4 =  milimetryobyl(((SPLINE *)ad)->xy[6], ((SPLINE *)ad)->xy[7]) ;
	   sprintf(sk, "%13.9lf", ly4);
	   menu_par_new((*mInfoAbout.pola)[iY4].txt, sk);
	   act(iY4);
   }

   nazwa_bloku_first(ad, &sk, &p_h_w_z);
   sk[40] = '\0';
   menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk);
   act(iInnerBlockName);

   nazwa_bloku(ad, &sk);
   sk[40] = '\0';
   menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk);
   act(iOuterBlockName);

   }

  else if (((NAGLOWEK*)ad)->obiekt==Opoint)
   {   
     strcpy(sk, objects[8]);
     menu_par_new((*mInfoAbout.pola)[0].txt, sk) ;
	 act(iObject);

     sprintf (sk, "%#ld", ((T_Point *)ad)->warstwa + 1) ;
     strcat(sk," '");
     strcat(sk,Layers[((T_Point *)ad)->warstwa].name);
     strcat(sk,"'");
     menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
	 act(1);

     nazwa_koloru(ad,&sk);
     LiniaG.kolor=((T_Point *)ad)->kolor;
     menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
	 act(iColor);

     strcpy(sk,typ_punktu_inf[((T_Point *)ad)->typ]);
     menu_par_new((*mInfoAbout.pola)[iPType].txt, sk) ;
     (*mInfoAbout.pola)[iPType].iconno=point_icon[((T_Point *)ad)->typ];
     act(iPType);

     lx1 =  milimetryobxl(((T_Point *)ad)->x, ((T_Point *)ad)->y) ;
     sprintf (sk, "%13.9lf", lx1) ;
     menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
	 act(iX1);

     ly1 =  milimetryobyl(((T_Point *)ad)->x, ((T_Point *)ad)->y) ;
     sprintf (sk, "%13.9lf", ly1) ;
     menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
	 act(iY1);

     nazwa_bloku_first(ad,&sk, &p_h_w_z);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
	 act(iInnerBlockName);

     nazwa_bloku(ad,&sk);
	 sk[40]='\0';
     menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
	 act(iOuterBlockName);

   }  
  else if (((NAGLOWEK*)ad)->obiekt == Opcx)
  {
	//zapamietanie parametrow
	pcx0 = (B_PCX *)ad;
	memmove(&b_pcx, pcx0, sizeof(B_PCX));
	memmove(&head, &b_pcx.pcx, sizeof(PCXheader));

	//determine scale_x and scale_y
	scale_xf = b_pcx.dx / 25.4 * (double)head.horz_res;
	scale_yf = b_pcx.dy / 25.4 * (double)head.vert_res;
	angle_f = b_pcx.kat / Pi * 180.0;

    strcpy(sk, objects[10]);
    menu_par_new((*mInfoAbout.pola)[0].txt, sk);
    act(iObject);

	sprintf(sk, "%#ld", ((B_PCX *)ad)->warstwa + 1);
	strcat(sk, " '");
	strcat(sk, Layers[((B_PCX *)ad)->warstwa].name);
	strcat(sk, "'");
	menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk);
	act(1);

    lx1 =  milimetryobxl(((B_PCX *)ad)->x, ((B_PCX *)ad)->y) ;
	sprintf(sk, "%13.9lf", lx1);
	menu_par_new((*mInfoAbout.pola)[iX1].txt, sk);
	act(iX1);

    ly1 =  milimetryobyl(((B_PCX *)ad)->x, ((B_PCX *)ad)->y) ;
	sprintf(sk, "%13.9lf", ly1);
	menu_par_new((*mInfoAbout.pola)[iY1].txt, sk);
	act(iY1);

	sprintf(sk, "%8.7lf", angle_f - get_angle_l());
	menu_par_new((*mInfoAbout.pola)[iAngle].txt, sk);
	act(iAngle);

	pxl_xi = head.xmax - head.xmin;
	pxl_yi = head.ymax - head.ymin;
	size_xf = 25.4 / head.horz_res * pxl_xi * scale_xf;
	size_yf = 25.4 / head.vert_res * pxl_yi * scale_yf;

	sprintf(sk, "%13.9lf", fabs(milimetryob(size_xf)));
	menu_par_new((*mInfoAbout.pola)[iDX].txt, sk);
	act(iDX);

	sprintf(sk, "%13.9lf", fabs(milimetryob(size_yf)));
	menu_par_new((*mInfoAbout.pola)[iDY].txt, sk);
	act(iDY);

      sprintf(sk, "%13.9lf", ((B_PCX *)ad)->dx);
      menu_par_new((*mInfoAbout.pola)[iPxlDx].txt, sk);
      act(iPxlDx);

      sprintf(sk, "%13.9lf", ((B_PCX *)ad)->dy);
      menu_par_new((*mInfoAbout.pola)[iPxlDy].txt, sk);
      act(iPxlDy);

	nazwa_bloku_first(ad, &sk, &p_h_w_z);
	sk[40] = '\0';
	menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk);
	act(iInnerBlockName);

	nazwa_bloku(ad, &sk);
	sk[40] = '\0';
	menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk);
	act(iOuterBlockName);

  }
  else if (((NAGLOWEK*)ad)->obiekt==Ovector)
    {
        strcpy(sk, objects[18]);
        menu_par_new((*mInfoAbout.pola)[iObject].txt, sk) ;
        act(iObject);

        sprintf (sk, u8"%#ld", ((AVECTOR *)ad)->warstwa + 1) ;
        strcat(sk,u8" '");
        strcat(sk,Layers[((AVECTOR *)ad)->warstwa].name);
        strcat(sk,u8"'");
        menu_par_new((*mInfoAbout.pola)[iLayer].txt, sk) ;
        act(iLayer);

        nazwa_koloru(ad,sk);
        LiniaG.kolor=((AVECTOR *)ad)->kolor;
        menu_par_new((*mInfoAbout.pola)[iColor].txt, sk) ;
        act(iColor);

        grubosc_l=(((AVECTOR *)ad)->typ & 224 ) / 32 ;
        typ_l=(((AVECTOR *)ad)->typ & 31 );
        strcpy(sk,view_type_tab[typ_l]);
        menu_par_new((*mInfoAbout.pola)[iLType].txt, sk) ;
        act(iLType);

        strcpy(sk,view_width_tab1[grubosc_l]);
        menu_par_new((*mInfoAbout.pola)[iLWidth].txt, sk) ;
        act(iLWidth);

        lx1 =  milimetryobxl(((AVECTOR *)ad)->x1, (((AVECTOR *)ad)->y1)) ;
        sprintf (sk, "%13.9lf", lx1) ;
        menu_par_new((*mInfoAbout.pola)[iX1].txt, sk) ;
        act(iX1);
        edit_param_no[iX1]=TRUE;

        ly1 =   milimetryobyl(((AVECTOR *)ad)->x1, (((AVECTOR *)ad)->y1)) ;
        sprintf (sk, "%13.9lf", ly1) ;
        menu_par_new((*mInfoAbout.pola)[iY1].txt, sk) ;
        act(iY1);
        edit_param_no[iY1]=TRUE;


        lx2 =  milimetryobxl(((AVECTOR *)ad)->x2, (((AVECTOR *)ad)->y2));

        ly2 = milimetryobyl(((AVECTOR *)ad)->x2, (((AVECTOR *)ad)->y2));

        parametry_lini(ad, &pl) ;
        dX = lx2 - lx1 ;
        dY = ly2 - ly1 ;
        dl = milimetryob (pl.dl) ;

        sprintf (sk, "%13.9lf", dl);
        menu_par_new((*mInfoAbout.pola)[iLenght].txt, sk) ;
        act(iLenght);
        edit_param_no[iLenght]=FALSE;

        strcpy(sk,vector_style_tab[((AVECTOR *)ad)->style]);
        menu_par_new((*mInfoAbout.pola)[iVstyle].txt, sk) ;
        act(iVstyle);

        sprintf (sk, "%13.9lf", fabs(dX));
        menu_par_new((*mInfoAbout.pola)[iDX].txt, sk) ;
        act(iDX);

        sprintf (sk, "%13.9lf", fabs(dY));
        menu_par_new((*mInfoAbout.pola)[iDY].txt, sk) ;
        act(iDY);

        switch ((((AVECTOR *)ad)->style))
        {
            case 0:
            case 1:
            case 2:
            case 3:
                sprintf (sk, "%13.9lf", lx2) ;
                menu_par_new((*mInfoAbout.pola)[iX2].txt, sk) ;
                act(iX2);
                edit_param_no[iX2]=TRUE;
                sprintf (sk, "%13.9lf", ly2) ;
                menu_par_new((*mInfoAbout.pola)[iY2].txt, sk) ;
                act(iY2);
                edit_param_no[iY2]=TRUE;

                angle = pl.kat - get_angle_l();
                sprintf (sk, "%8.7lf", angle);
                menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
                act(iAngle1);
                edit_param_no[iAngle1]=FALSE;
                break;
            case 4:
                //magnitude1
                set_decimal_format(&sk, ((AVECTOR *)ad)->magnitude1, force_precision);
                menu_par_new((*mInfoAbout.pola)[iMagnitude].txt, sk) ;
                act(iMagnitude);
                (*mInfoAbout.pola)[iMagnitude].iconno=727;

                if (((AVECTOR *)ad)->variant>0)
                    sprintf(sk, "%s%d",load_symbol[(int)((AVECTOR *)ad)->load], ((AVECTOR *)ad)->variant);
                else sprintf(sk, "%s",load_symbol[(int)((AVECTOR *)ad)->load]);
                menu_par_new((*mInfoAbout.pola)[iLoadCharacter].txt, sk) ;
                act(iLoadCharacter);
                if (((AVECTOR *)ad)->load>0) (*mInfoAbout.pola)[iLoadCharacter].iconno=((AVECTOR *)ad)->load+786;
                else (*mInfoAbout.pola)[iLoadCharacter].iconno=797;

                angle = pl.kat - get_angle_l();
                sprintf (sk, "%8.7lf", angle);
                menu_par_new((*mInfoAbout.pola)[iAngle].txt, sk) ;
                act(iAngle);
                break;
            case 7:
                //magnitude1
                set_decimal_format(&sk, ((AVECTOR *)ad)->magnitude1, displacement_precision);
                menu_par_new((*mInfoAbout.pola)[iMagnitude].txt, sk) ;
                act(iMagnitude);
                (*mInfoAbout.pola)[iMagnitude].iconno=730;
                parametry_lini(ad, &pl) ;
                dX = lx2 - lx1 ;
                dY = ly2 - ly1 ;
                dl = milimetryob (pl.dl) ;
                angle = pl.kat - get_angle_l();
                sprintf (sk, "%8.7lf", angle);
                menu_par_new((*mInfoAbout.pola)[iAngle].txt, sk) ;
                act(iAngle);
                break;
            case 5:
            case 6:
                //magnitude1
                set_decimal_format(&sk, ((AVECTOR *)ad)->magnitude1, moment_precision);
                menu_par_new((*mInfoAbout.pola)[iMagnitude].txt, sk) ;
                act(iMagnitude);
                (*mInfoAbout.pola)[iMagnitude].iconno=728;

                if (((AVECTOR *)ad)->variant>0)
                    sprintf(sk, "%s%d",load_symbol[(int)((AVECTOR *)ad)->load], ((AVECTOR *)ad)->variant);
                else sprintf(sk, "%s",load_symbol[(int)((AVECTOR *)ad)->load]);
                menu_par_new((*mInfoAbout.pola)[iLoadCharacter].txt, sk) ;
                act(iLoadCharacter);
                if (((AVECTOR *)ad)->load>0) (*mInfoAbout.pola)[iLoadCharacter].iconno=((AVECTOR *)ad)->load+786;
                else (*mInfoAbout.pola)[iLoadCharacter].iconno=797;
                //radius
                sprintf (sk, "%9.4lf", ((AVECTOR *)ad)->r) ;
                menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
                act(iRadius);
                //radius
                sprintf (sk, "%9.4lf", 180*((AVECTOR *)ad)->angle1/Pi_) ;
                menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
                act(iAngle1);
                break;
                break;
            case 8:
            case 9:
                //magnitude1
                set_decimal_format(&sk, ((AVECTOR *)ad)->magnitude1, rotation_precision);
                menu_par_new((*mInfoAbout.pola)[iMagnitude].txt, sk) ;
                act(iMagnitude);
                (*mInfoAbout.pola)[iMagnitude].iconno=731;
                //radius
                sprintf (sk, "%9.4lf", ((AVECTOR *)ad)->r) ;
                menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
                act(iRadius);
                //radius
                sprintf (sk, "%9.4lf", 180*((AVECTOR *)ad)->angle1/Pi_) ;
                menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
                act(iAngle1);
                break;
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
                sprintf (sk, "%13.9lf", lx2) ;
                menu_par_new((*mInfoAbout.pola)[iX2].txt, sk) ;
                act(iX2);
                edit_param_no[iX2]=TRUE;
                sprintf (sk, "%13.9lf", ly2) ;
                menu_par_new((*mInfoAbout.pola)[iY2].txt, sk) ;
                act(iY2);
                edit_param_no[iY2]=TRUE;
                //angle1
                angle = pl.kat - get_angle_l();
                sprintf (sk, "%8.7lf", angle);
                menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
                act(iAngle1);
                edit_param_no[iAngle1]=FALSE;
                //magnitude1
                //magnitude2
                set_decimal_format(&sk, ((AVECTOR *)ad)->magnitude1, load_precision);
                menu_par_new((*mInfoAbout.pola)[iMagnitude1].txt, sk) ;
                act(iMagnitude1);
                (*mInfoAbout.pola)[iMagnitude1].iconno=766;

                set_decimal_format(&sk, ((AVECTOR *)ad)->magnitude2, load_precision);
                menu_par_new((*mInfoAbout.pola)[iMagnitude2].txt, sk) ;
                act(iMagnitude2);
                (*mInfoAbout.pola)[iMagnitude2].iconno=767;

                if (((AVECTOR *)ad)->variant>0)
                    sprintf(sk, "%s%d",load_symbol[(int)((AVECTOR *)ad)->load], ((AVECTOR *)ad)->variant);
                else sprintf(sk, "%s",load_symbol[(int)((AVECTOR *)ad)->load]);
                menu_par_new((*mInfoAbout.pola)[iLoadCharacter].txt, sk) ;
                act(iLoadCharacter);
                if (((AVECTOR *)ad)->load>0) (*mInfoAbout.pola)[iLoadCharacter].iconno=((AVECTOR *)ad)->load+786;
                else (*mInfoAbout.pola)[iLoadCharacter].iconno=797;

                break;
            case 15:
                sprintf (sk, "%13.9lf", lx2) ;
                menu_par_new((*mInfoAbout.pola)[iX2].txt, sk) ;
                act(iX2);
                edit_param_no[iX2]=TRUE;
                sprintf (sk, "%13.9lf", ly2) ;
                menu_par_new((*mInfoAbout.pola)[iY2].txt, sk) ;
                act(iY2);
                edit_param_no[iY2]=TRUE;
                //angle1
                angle = pl.kat - get_angle_l();
                sprintf (sk, "%8.7lf", angle);
                menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
                act(iAngle1);
                edit_param_no[iAngle1]=FALSE;
                //magnitude1
                //magnitude2
                //radius
                set_decimal_format(&sk, ((AVECTOR *)ad)->magnitude1, thermal_precision);
                menu_par_new((*mInfoAbout.pola)[iMagnitude1].txt, sk) ;
                act(iMagnitude1);
                (*mInfoAbout.pola)[iMagnitude1].iconno=768;

                set_decimal_format(&sk, ((AVECTOR *)ad)->magnitude2, thermal_precision);
                menu_par_new((*mInfoAbout.pola)[iMagnitude2].txt, sk) ;
                act(iMagnitude2);
                (*mInfoAbout.pola)[iMagnitude2].iconno=769;

                if (((AVECTOR *)ad)->variant>0)
                    sprintf(sk, "%s%d",load_symbol[(int)((AVECTOR *)ad)->load], ((AVECTOR *)ad)->variant);
                else sprintf(sk, "%s",load_symbol[(int)((AVECTOR *)ad)->load]);
                menu_par_new((*mInfoAbout.pola)[iLoadCharacter].txt, sk) ;
                act(iLoadCharacter);
                if (((AVECTOR *)ad)->load>0) (*mInfoAbout.pola)[iLoadCharacter].iconno=((AVECTOR *)ad)->load+787;
                else (*mInfoAbout.pola)[iLoadCharacter].iconno=797;

                sprintf (sk, "%9.4lf", ((AVECTOR *)ad)->r) ;
                menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
                act(iRadius);
                break;
            case 16:
                sprintf (sk, "%13.9lf", lx2) ;
                menu_par_new((*mInfoAbout.pola)[iX2].txt, sk) ;
                act(iX2);
                edit_param_no[iX2]=TRUE;
                sprintf (sk, "%13.9lf", ly2) ;
                menu_par_new((*mInfoAbout.pola)[iY2].txt, sk) ;
                act(iY2);
                edit_param_no[iY2]=TRUE;
                //angle1
                angle = pl.kat - get_angle_l();
                sprintf (sk, "%8.7lf", angle);
                menu_par_new((*mInfoAbout.pola)[iAngle1].txt, sk) ;
                act(iAngle1);
                edit_param_no[iAngle1]=FALSE;
                sprintf (sk, "%9.4lf", ((AVECTOR *)ad)->r) ;
                menu_par_new((*mInfoAbout.pola)[iRadius].txt, sk) ;
                act(iRadius);
                break;
            default:
                break;
        }

        nazwa_bloku_first(ad,&sk, &p_h_w_z);
        sk[40]='\0';
        menu_par_new((*mInfoAbout.pola)[iInnerBlockName].txt, sk) ;
        act(iInnerBlockName);

        nazwa_bloku(ad,&sk);
        sk[40]='\0';
        menu_par_new((*mInfoAbout.pola)[iOuterBlockName].txt, sk) ;
        act(iOuterBlockName);
    }
  else  /*obiekt nieznany*/
   {
    komunikat(0);
    return;
   }

  i_active = 0;

  for (i = 0; i < mInfoAbout.max; i++)
  {
	  if (active[i] == TRUE)
	  {
		  memcpy(&pmInfoAboutA[i_active], &pmInfoAbout[i], sizeof(POLE));
          param_no[i_active]=i;
		  i_active++;
	  }
  }
  mInfoAboutA.max = i_active;

  int LiniaG_kolor=LiniaG.kolor;

  CUR = MVCUR ;
  MVCUR = noooph;
  if((n=getwsp2(&mInfoAboutA, ad, param_no)-1)>=0)    /* n=-1 -> esc */
   {

   ;

   }
  MVCUR = CUR ;

  LiniaG.kolor=LiniaG_kolor;
  komunikat(0);
   if (TTF_redraw) redraw();
   return ;
}


#undef __INFO__