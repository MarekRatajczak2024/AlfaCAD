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

#define __O_PLINE__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#ifndef LINUX
#include<process.h>
#else
#endif
#include<ctype.h>
#ifndef LINUX
#include<io.h>
#include<dos.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<math.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_pline.h"
#include "o_libfun.h"

#include "menu.h"

extern int Pline_Line (double, double, BLOK **, int) ;
extern int Pline_Arc (double, double, BLOK **, int) ;
extern int last_pline_delete (void) ;
extern void Wymiarowanie_polilinii(char *adr0, int opcja);
extern void view_line_type(LINIA *L);
extern void Wymiarowanie_polilinii1(char *adr0, int opcja);
extern BOOL group_block_with_type (long dane_size000, double x, double y, char *blok_type0, char obiektt2, char *prefix, char kod_obiektu);
extern BLOK *koniecP(double *xsel, double *ysel);
extern BOOL Set_Beginning_Pline (BLOK *blk_adr, double *X0, double *Y0);
//extern int PLine_Line_Command_Proc (int ev_nr);
extern int vector_command_proc (int ev_nr);
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);
static TMENU mPLine_Con={1,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr+1,0,0,0,&pmPLine_Con,NULL,NULL} ;
static TMENU mPLine_Con_Slab={2,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr+1,0,0,0,&pmPLine_Con_Slab,NULL,NULL} ;
static BLOK *BLK_ADR;

static int last_plate_property_no=0;

enum PLINE_MODE {PL_MODE_CONTINUE = 1 , PL_MODE_LINE , PL_MODE_ARC,
	PL_MODE_END, PL_MODE_UNDO } ;

int pline_trace=0;
int add_property_number(double X0, double Y0);

/*-----------------------------------------------------------------------*/

static void erase_pline (void)
/*---------------------------*/
{
  BLOK *ptrs_block ;
  NAGLOWEK *nag ;

  nag = (NAGLOWEK*)dane ;
  ptrs_block = (BLOK*)dane ;
  if (nag->obiekt == OdBLOK &&
      ptrs_block->kod_obiektu == B_PLINE &&
     ptrs_block->dlugosc_opisu_obiektu == 1 &&
     ((ptrs_block->opis_obiektu [0] == PL_PLINE) ||
      (ptrs_block->opis_obiektu [0] == PL_PLATE) ||
      (ptrs_block->opis_obiektu [0] == PL_HOLE) ||
      (ptrs_block->opis_obiektu [0] == PL_WALL) ||
      (ptrs_block->opis_obiektu [0] == PL_ZONE) ||
      (ptrs_block->opis_obiektu [0] == PL_SIEC)))

  {
    blokzap(dane, (char  *)(dane + nag->n), Anormalny, COPY_PUT, 0) ;
    usun_obiekt ((void *)dane) ;
  }
  else
  {
    ErrList (77) ;
  }
}

void erase_pline_ (void)
{
    erase_pline ();
}

static BOOL add_block (void)
/*------------------------*/
{
    BLOK s_blockd = Bdef ;  //TO CHECK
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
   s_blockd.kod_obiektu = B_PLINE ;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  if (HOLE==TRUE) s_blockd.obiektt2=O2BlockAparat;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  if (PLATE==TRUE) c_pltype = PL_PLATE;
    else if (HOLE==TRUE) c_pltype = PL_HOLE;
     else if (WALL==TRUE) c_pltype = PL_WALL;
      else if (ZONE==TRUE) c_pltype = PL_ZONE;
      else if (SIEC_P==TRUE) c_pltype = PL_SIEC;
        else c_pltype = PL_PLINE ;
  memmove ( &(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;

  return TRUE ;
}

BOOL add_block_(void)
{
    return add_block();
}

void set_obiektt2(BLOK *blk_adr, unsigned char obiektt2)
{
    char *adp;
    char *adk;
    NAGLOWEK *nag;
    adp = (char*)blk_adr+sizeof(NAGLOWEK)+B3+blk_adr->dlugosc_opisu_obiektu;
    adk = (char *)blk_adr+sizeof(NAGLOWEK)+blk_adr->n-1;
    while (adp<adk)
    {
        nag=(NAGLOWEK*)adp;
        if ((nag->obiekt==Olinia) || (nag->obiekt==Oluk))
        {
            nag->obiektt2=obiektt2;
        }
       adp+=sizeof(NAGLOWEK)+nag->n;
    }
}

static void redcr (char typ, int m_con)
/*-----------------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[5])(),akt;
  if (typ == 0)
  {
     komunikat0 (80) ;
     akt = sel.akt ; sel.akt = ASel ;
     CUR_OFF (X, Y) ;
     CUR_oFF = CUR_OFF ;  CUR_OFF = cursel_off ;
     CUR_oN = CUR_ON ;   CUR_ON = cursel_on ;
     SW[0] = SERV [73] ;  SERV [73] = sel_t ;
     SW[1] = SERV [81] ;  SERV [81] = sel_d ;
     SW[4] = SERV [83];  SERV [83] = last_pline_delete ;

     if (m_con==1)
         menupini (&mPLine_Con, _POLYLINE_, _POLYLINE_C_,20) ;
     else if (m_con==2)
         menupini (&mPLine_Con_Slab, _POLYLINE_, _POLYLINE_C_,20) ;
     CUR_ON (X, Y) ;
  }
  else
  {
     CUR_OFF (X, Y) ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     SERV [73] = SW [0] ;
     SERV [81] = SW [1] ;
     SERV [83] = SW [4] ;
     CUR_ON (X, Y) ;
     sel.akt = akt ;
      menupini (NULL, "", ' ', 0) ;
     komunikat0 (0) ;
   }
}

BLOK *select_polyline(void)
{
    double xsel, ysel;
    int ret, ret1;
    BOOL ret2;
    BLOK *blk_adr=NULL;
    char pline_str[32];
    double X0, Y0;
    void *ptr_ob;
    BOOL b_first_end;
    double df__xbeg, df__ybeg;

    blk_adr = koniecP(&xsel, &ysel);
    if (blk_adr!=NULL)
    {
        //PL_PLATE  = 'A', PL_HOLE = 'B', PL_WALL = 'W', PL_ZONE = 'Z',
        //PLATE, HOLE, WALL, ZONE
        if (((PLATE==1) && (blk_adr->opis_obiektu[0]!=PL_PLATE)) ||
            ((HOLE==1) && (blk_adr->opis_obiektu[0]!=PL_HOLE)) ||
            ((WALL==1) && (blk_adr->opis_obiektu[0]!=PL_WALL)) ||
            ((ZONE==1) && (blk_adr->opis_obiektu[0]!=PL_ZONE)))
        {
            sprintf(pline_str,"%s%s%s%s", PLATE==1 ? __PLATE__:"", HOLE==1 ? __HOLE__:"", WALL==1 ? __WALL__:"", ZONE==1 ? __ZONE__:"");
            ret = ask_question(2, _No_, _Yes_, "", _CHANGE_POLYLINE_TYPE_, 12, pline_str, 11, 1, 63);
            if (ret == 1)
            {
                if ((PLATE==1) || (ZONE==1))
                {
                    if ((blk_adr->opis_obiektu[0]!=PL_PLATE) && (blk_adr->opis_obiektu[0]!=PL_ZONE))  //no text
                    {
                        //adding property number
                        ret2 = Get_Begin_Pline(blk_adr, &ptr_ob, &b_first_end, &df__xbeg, &df__ybeg);
                        BLK_ADR = blk_adr;
                        ret1 = add_property_number(df__xbeg, df__ybeg);
                        if (ret1 == 0) return NULL;
                        blk_adr = BLK_ADR;
                    }
                    if (PLATE==1) blk_adr->opis_obiektu[0]=PL_PLATE;
                    else blk_adr->opis_obiektu[0]=PL_ZONE;
                }
                else if (HOLE==1) blk_adr->opis_obiektu[0]=PL_HOLE;
                else if (WALL==1) blk_adr->opis_obiektu[0]=PL_WALL;

                set_obiektt2(blk_adr, O2FREE_EDGE);
            }
            else if (ret == 2) return NULL;
            else return NULL;
        }
        X=xsel;
        Y=ysel;
        return blk_adr;
    }
    return NULL;
}

static int get_start_pline (double *X0, double *Y0, int m_con)
/*-----------------------------------------------------------*/
{
  EVENT *ev;
  BLOK *blk_adr;
  int ret_command, n;

  redcr (0, m_con) ;
  while(1)
  {
    view_line_type(&LiniaG);
    ev = Get_Event_Point (NULL, X0, Y0) ;
    if (ev->What == evKeyDown)
    {
      if(ev->Number == 0)
      {
	    redcr (1, m_con) ;
	    return 0 ;
      }
      if(ev->Number == ENTER)
      {
	    redcr (1, m_con) ;
	    return 1 ;
      }
    }
    else if (ev->What == evCommandP)
    {
        printf("%d",ev->Number);
        if (ev->Number==1)
        {
            blk_adr=select_polyline();
            if (blk_adr!=NULL)
            {
                PTR__GTMPBLOCK=(char*)blk_adr;
                erase_pline () ;
                blk_adr=(BLOK*)PTR__GTMPBLOCK;
                BOOL ret=Set_Beginning_Pline(blk_adr, X0, Y0);
                BLK_ADR=blk_adr;
                return 3;
            }
        }
        //else if (ev->Number==2)  //other vectors
        //{
        //    VectorG.style=0;  //reset to open vector menu
        //    return FALSE;
        //}
        else if (ev->Number>COMNDmnr)
        {
            n=ev->Number-COMNDmnr;
            ret_command=vector_command_proc(n);
            return 2;
        }
    }

  }
}

BOOL get_start_pline_ (double *X0, double *Y0)
{
    return get_start_pline (X0, Y0, 0);
}

void set_pline_param (BOOL b_pline)
/*---------------------------------------*/
{
   if (b_pline == TRUE)
   {
     if (PLATE==TRUE)
       {
         LiniaG.typ = 96; //64 ;
         LukG.typ = 96; //64 ;

         LiniaG.obiektt2 = O2FREE_EDGE ;
         LukG.obiektt2 = O2FREE_EDGE ;

         LiniaG.obiektt3=O3REGULAR_EDGE;
         LukG.obiektt3=O3REGULAR_EDGE;
       }
       else
       if (HOLE==TRUE)
        {
          LiniaG.typ = 96; //64 ;
          LukG.typ = 96; //64 ;

          LiniaG.obiektt2 = O2FREE_EDGE ;
          LukG.obiektt2 = O2FREE_EDGE ;

          LiniaG.obiektt3=O3REGULAR_EDGE;
          LukG.obiektt3=O3REGULAR_EDGE;
        }
       else
       if (WALL==TRUE)
       {
           LiniaG.typ = 97; //65 ;
           LukG.typ = 97; //65 ;

           LiniaG.obiektt2 = O2FREE_EDGE ;
           LukG.obiektt2 = O2FREE_EDGE ;

           LiniaG.obiektt3=O3REGULAR_EDGE;
           LukG.obiektt3=O3REGULAR_EDGE;
       }
       else
       if (ZONE==TRUE)
       {
           LiniaG.typ = 128 ;
           LukG.typ = 128 ;

           LiniaG.obiektt2 = O2FREE_EDGE ;
           LukG.obiektt2 = O2FREE_EDGE ;

           LiniaG.obiektt3=O3REGULAR_EDGE;
           LukG.obiektt3=O3REGULAR_EDGE;
       }
        else
        if (SIEC_P==TRUE)
         {
           LiniaG.obiektt2 = O2BlockAparat ;
           LukG.obiektt2 = O2BlockAparat ;
         }
        else
         {
           LiniaG.obiektt2 = O2BlockPline ;
           LukG.obiektt2 = O2BlockPline ;
         }
   }
   else
   {
     LiniaG.obiektt2 = O2NoBlockS ;
     LukG.obiektt2 = O2NoBlockS ;

     LiniaG.obiektt3=O3REGULAR_EDGE;
     LukG.obiektt3=O3REGULAR_EDGE;
   }
}


int add_property_number(double X0, double Y0)
{   char opis_plyty[31];
    char plate_property_no_str[64];
    TEXT TextGa=Tdef;
    double angle_l, sina, cosa;
    int LengthT;
    double dy_komentarz = 3.5;
    double dx_komentarz = 0.5;

    strcpy(opis_plyty,"");
    //if (read_esc(opis_plyty,30,181)==0)
    //{
    //  ;
    //}
    //get property no
    sprintf(plate_property_no_str,"%d",last_plate_property_no+1);
    if (!get_string (plate_property_no_str, "1234567890", 16, 0, 211))
    {
        set_pline_param (FALSE) ;
        erase_pline () ;
        return 0;
    }
    if (strlen(plate_property_no_str)==0)
    {
        set_pline_param (FALSE) ;
        erase_pline () ;
        return 0;
    }
    sprintf(opis_plyty,"#%s",plate_property_no_str);
    angle_l=get_angle_l();
    sina=get_sina();
    cosa=get_cosa();
    //DESCRIPTION
    TextGa.kat= (angle_l/360) * Pi2;
    TextGa.wysokosc=zmwym.wysokosc;
    TextGa.bold=zmwym.bold;
    TextGa.italics=zmwym.italics;
    TextGa.width_factor=(float)zmwym.width_factor;
    TextGa.warstwa=Current_Layer;
    TextGa.kolor=LiniaG.kolor;
    TextGa.czcionka=zmwym.czcionka;
    TextGa.typ=n_typ_komentarz;
    TextGa.justowanie=j_do_lewej;
    TextGa.ukryty=0;
    TextGa.obiektt1 = 0;
    TextGa.obiektt2 = O2NoBlockS;
    TextGa.blok=1;

    TextGa.x=X0 + (dx_komentarz * cosa) + (dy_komentarz * sina);
    TextGa.y=Y0 - (dy_komentarz * cosa) + (dx_komentarz * sina);

    strcpy(&TextGa.text[0],opis_plyty);
    LengthT=strlen(TextGa.text);
    TextGa.dl=LengthT;
    TextGa.n=T18+TextGa.dl;

    PTR__GTMPBLOCK=(char*)BLK_ADR;
    if (dodaj_obiekt_reversed(BLK_ADR, (void*)&TextGa)!=NULL)
    {
        rysuj_obiekt((char*)&TextGa,COPY_PUT,1);
    }
    BLK_ADR=(BLOK*)PTR__GTMPBLOCK;

    return 1;
}

int Pline_Factory (int mode)
/*-------------------------*/
{
  double X0, Y0, df_xbeg, df_ybeg ;
  int pline_mode ;
 // TEXT TextGa=Tdef;
  //int LengthT;
  //double angle_l, sina, cosa;
  //double dy_komentarz = 3.5;
  //double dx_komentarz = 0.5;
  //char opis_plyty[31];
  int ret;
  //char plate_property_no_str[64];
  int plate_property_no;

  pline_trace=0; //pline

   // menupini (&mPLine_Con, _POLYLINE_, _POLYLINE_C_,20) ;

  set_pline_param (TRUE) ;
beg:
    //new block
  if (FALSE == add_block ())
  {
    set_pline_param (FALSE) ;
    return 0;
  }
  BLK_ADR=(BLOK*)dane;
start:
  ret = get_start_pline (&X0, &Y0, 1+mode);
 if (ret==0)
  {
    set_pline_param (FALSE) ;
    erase_pline () ;
    return 0;
  }
 else if (ret==2)
 {
     return 2;
 }

  pline_mode = PL_MODE_LINE ;
  df_xbeg = X0 ;
  df_ybeg = Y0 ;

  if ((ret!=3) && ((PLATE==TRUE)  || (ZONE==TRUE))) //adding property number, (ret==3) is continuation
   {

      ret = add_property_number(X0, Y0);
      if (ret==0) return 0;
    /*
    strcpy(opis_plyty,"");
    //if (read_esc(opis_plyty,30,181)==0)
    //{
    //  ;
    //}
       //get property no
   sprintf(plate_property_no_str,"%d",last_plate_property_no+1);
   if (!get_string (plate_property_no_str, "1234567890", 16, 0, 211))
   {
       set_pline_param (FALSE) ;
       erase_pline () ;
       return 0;
   }
   if (strlen(plate_property_no_str)==0)
   {
       set_pline_param (FALSE) ;
       erase_pline () ;
       return 0;
   }
    sprintf(opis_plyty,"#%s",plate_property_no_str);
    angle_l=get_angle_l();
    sina=get_sina();
    cosa=get_cosa();
    //DESCRIPTION
    TextGa.kat= (angle_l/360) * Pi2;
    TextGa.wysokosc=zmwym.wysokosc;
    TextGa.bold=zmwym.bold;
    TextGa.italics=zmwym.italics;
    TextGa.width_factor=(float)zmwym.width_factor;
    TextGa.warstwa=Current_Layer;
    TextGa.kolor=LiniaG.kolor;
    TextGa.czcionka=zmwym.czcionka;
    TextGa.typ=n_typ_komentarz;
    TextGa.justowanie=j_do_lewej;
    TextGa.ukryty=0;
    TextGa.obiektt1 = 0;
    TextGa.obiektt2 = O2NoBlockS;
    TextGa.blok=1;

    TextGa.x=X0 + (dx_komentarz * cosa) + (dy_komentarz * sina);
    TextGa.y=Y0 - (dy_komentarz * cosa) + (dx_komentarz * sina);

    strcpy(&TextGa.text[0],opis_plyty);
    LengthT=strlen(TextGa.text);
    TextGa.dl=LengthT;
    TextGa.n=T18+TextGa.dl;

    PTR__GTMPBLOCK=(char*)BLK_ADR;
    if (dodaj_obiekt(BLK_ADR, (void*)&TextGa)!=NULL)
     {
       rysuj_obiekt((char*)&TextGa,COPY_PUT,1);
     }
    BLK_ADR=(BLOK*)PTR__GTMPBLOCK;
     */
   }

  while (1)
  {
    if (pline_mode == PL_MODE_LINE)
    {
      PTR__GTMPBLOCK=(char*)BLK_ADR;
      pline_mode = Pline_Line (df_xbeg, df_ybeg, &BLK_ADR, mode) ;
      BLK_ADR=(BLOK*)PTR__GTMPBLOCK;
    }
    else
    {
      PTR__GTMPBLOCK=(char*)BLK_ADR;
      pline_mode = Pline_Arc (df_xbeg, df_ybeg, &BLK_ADR, mode) ;
      BLK_ADR=(BLOK*)PTR__GTMPBLOCK;
    }
    if (PL_MODE_END == pline_mode)
    {
      goto beg ;
    }
    else
    if (PL_MODE_UNDO == pline_mode)
    {
      goto start ;
    }
  }
  return 1;
}

void Pline (void)
{
    int PLATE0=PLATE, HOLE0=HOLE, WALL0=WALL, ZONE0=ZONE;

    PLATE=HOLE=WALL=ZONE=FALSE;
    int ret = Pline_Factory(0);

    PLATE=PLATE0, HOLE=HOLE0, WALL=WALL0, ZONE=ZONE0;
}

int Pline_Slab (void)
{   LINIA LiniaGBak;
    int ret;
    //saving LiniaG and LukG
    memcpy(&LiniaGBak, &LiniaG, sizeof(LINIA));
    ret=Pline_Factory(1);
    //restoring Lineg and LukG
    LiniaGBak.warstwa=LiniaG.warstwa;  //if layer was changed
    memcpy(&LiniaG, &LiniaGBak, sizeof(LINIA));
    return ret;
}

int Pline_single (double *x_begin, double *y_begin)
/*-----------------------------------------------*/
{
  double X0, Y0, df_xbeg, df_ybeg ;
  int pline_mode ;

  set_pline_param (TRUE) ;
beg1:
  if (FALSE == add_block ())
  {
    set_pline_param (FALSE) ;
    return 0;
  }
start1:
  if (FALSE == get_start_pline (&X0, &Y0, 1))
  {
    set_pline_param (FALSE) ;
    erase_pline () ;
    return 0;
  }
  pline_mode = PL_MODE_LINE ;
  df_xbeg = X0 ;
  df_ybeg = Y0 ;
  while (1)
  {
    if (pline_mode == PL_MODE_LINE)
    {
      pline_mode = Pline_Line (df_xbeg, df_ybeg, (BLOK**)dane, 0) ;
    }
    else
    {
      pline_mode = Pline_Arc (df_xbeg, df_ybeg, (BLOK**)dane, 0) ;
    }
    if (PL_MODE_END == pline_mode)
    {
      *x_begin=X0;
      *y_begin=Y0;
      return 1;
    }
    else
    if (PL_MODE_UNDO == pline_mode)
    {
      goto start1 ;
    }
  }
}

int Pline_single_xy (double *x_begin, double *y_begin)
/*--------------------------------------------------*/
{
  double X0, Y0, df_xbeg, df_ybeg ;
  int pline_mode ;

  set_pline_param (TRUE) ;
beg2:
  if (FALSE == add_block ())
  {
    set_pline_param (FALSE) ;
    return 0;
  }
  BLK_ADR=(BLOK*)dane;

  view_line_type(&LiniaG);
  X0=X;
  Y0=Y;

  pline_mode = PL_MODE_LINE ;
  df_xbeg = X0 ;
  df_ybeg = Y0 ;
  while (1)
  {
    if (pline_mode == PL_MODE_LINE)
    {
      pline_mode = Pline_Line (df_xbeg, df_ybeg, &BLK_ADR, 0) ;
    }
    else
    {
      pline_mode = Pline_Arc (df_xbeg, df_ybeg, &BLK_ADR, 0) ;
    }
    if (PL_MODE_END == pline_mode)
    {
      *x_begin=X0;
      *y_begin=Y0;
      return 1;
    }
    else
    if (PL_MODE_UNDO == pline_mode)
    {
      return 0;
    }
  }
}


void PlineW1 (char *blok_type)
/*--------------------------*/
{
  double X0, Y0, df_xbeg, df_ybeg ;
  int pline_mode ;
  long dane_size000, dane_size0000;

  set_pline_param (TRUE) ;
beg:

  dane_size0000=dane_size;

  if (FALSE == add_block ())
  {
    set_pline_param (FALSE) ;
    return ;
  }
  BLK_ADR=(BLOK*)dane;
start:
  if (FALSE == get_start_pline (&X0, &Y0, 1))
  {
    set_pline_param (FALSE) ;
    erase_pline () ;
    return ;
  }
  pline_mode = PL_MODE_LINE ;
  df_xbeg = X0 ;
  df_ybeg = Y0 ;
  while (1)
  {
    if (pline_mode == PL_MODE_LINE)
    {
      pline_mode = Pline_Line (df_xbeg, df_ybeg, &BLK_ADR, 0) ;
    }
    else
    {
      pline_mode = Pline_Arc (df_xbeg, df_ybeg, &BLK_ADR, 0) ;
    }
    if (PL_MODE_END == pline_mode)
    {
      Wymiarowanie_polilinii1(dane, 0);
      dane_size000=dane_size-dane_size0000;
      if (dane_size000>0)
      {
        group_block_with_type(dane_size000,0,0,blok_type, O2NoBlockS, "", B_WIRE);   //L'č'
      }

      goto beg ;
    }
    else
    if (PL_MODE_UNDO == pline_mode)
    {
      goto start ;
    }
  }
}

#undef __O_PLINE__
