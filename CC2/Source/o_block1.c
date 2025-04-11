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
#include <limits.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_block1.h"
#include "o_ellip1.h"
#include "bib_blok.h"
#include "bib_edit.h"
#include "o_libfun.h"
#include "o_object.h"

#include "leak_detector_c.h"

/*-----skala------------------------------------*/
#define def67 67
#define def68 68

static double Px,Py, ks1, ks2 ;
static LINIA L=Ldef;
static float Z0=0.0;

#define r18 18
#define r22 16 //22
static int strwyj;


static double k01, k02;

extern BLOK *add_shadow_block(int atrybut, int to_block);
extern BOOL Make_Ellipse_Arc_Shadow(double df_xc, double df_yc, double df_xaxis, double df_yaxis, BLOK* ptrptr_adb, void* ptr_ob, int i_type, int obiektt2);
extern BOOL Make_Ellipse_Shadow(double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, int i_mode, BLOK* ptrptr_adb, void* ptr_ob, int i_type, int obiektt2);
extern void set_ellipse_poligon_num(int msg_no);

static void set_block_change_mode_scale (long_long offp, long_long offk)
/*---------------------------------------------------------*/
{
  long_long off ;
  BLOK * ptrs_block ;

  ObiektTok (&offp, offk, &off, OdBLOK) ;
  while (off != -1)
  {
    ptrs_block =(BLOK*)(dane + off) ;
    if (ptrs_block->atrybut == Ablok &&
	ptrs_block->obiektt1 < OB1CHANGE_SCALE )
    {
       ptrs_block->obiektt1 = OB1CHANGE_SCALE ;
    }
    ObiektTok (&offp, offk, &off, OdBLOK) ;
  }
}

static BLOK *object_to_block (void *ptr_ob, char c_type)
/*----------------------------------------------------*/
{
#ifndef LINUX
  BLOKD s_blockd = BDdef ;
#else
  BLOK s_blockd = BDdef ;
#endif
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = B_PLINE ;
  s_blockd.atrybut = Ablok ;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;

  s_blockd.blok = ((NAGLOWEK*)ptr_ob)->blok;

  if (NULL == (ptrs_block = (BLOK*)Change_Object_Set_Global_Ptr (ptr_ob, &s_blockd)))
  {
    return ptrs_block;
  }
  c_pltype = c_type ;
  memmove (&(ptrs_block->opis_obiektu [0]),&c_pltype, sizeof(c_pltype)) ;
  return ptrs_block ;
}

static char* trans_scale_arc (LUK    *ptrs_arc,
		 double df_basex,
		 double df_basey,
		 double df_sx,
		 double df_sy,
         BOOL enforce)
/*---------------------------*/
{
  double df_cx, df_cy, df_xaxis, df_yaxis, df_angle ; ;
  BOOL b_ret ;
  int i_mode ;
  BLOK *ptrs_block ;
  char c_desc ;
  int i_make_type ;
  double df_s0x, df_s0y ;
  ELLIPTICALARC s_ellipticarc = eldef;
  ELLIPTICALARC *ea;
  double kat1, kat2;
  double x1,y1,x2,y2;
  double df_x1, df_y1, df_x2, df_y2;
  double vx1, vy1, vx2, vy2;
  double dot,det;
  LUK s_arc=ldef;


    Scale_Point (df_sx, df_sy ,df_basex, df_basey, ptrs_arc->x, ptrs_arc->y, &df_cx, &df_cy);

    df_xaxis = ptrs_arc->r * fabs (df_sx) ;
    df_yaxis = ptrs_arc->r * fabs (df_sy) ;

    df_angle = 0 ;
    i_mode = 0 ;

    if (!enforce) {

        x1 = ptrs_arc->x + ptrs_arc->r * cos(ptrs_arc->kat1);
        y1 = ptrs_arc->y + ptrs_arc->r * sin(ptrs_arc->kat1);

        x2 = ptrs_arc->x + ptrs_arc->r * cos(ptrs_arc->kat2);
        y2 = ptrs_arc->y + ptrs_arc->r * sin(ptrs_arc->kat2);

        Scale_Point(df_sx, df_sy, df_basex, df_basey, x1, y1, &df_x1, &df_y1);
        Scale_Point(df_sx, df_sy, df_basex, df_basey, x2, y2, &df_x2, &df_y2);

        vx1 = df_x1 - df_cx;
        vy1 = df_y1 - df_cy;
        vx2 = 1.0;
        vy2 = 0.0;
        dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
        det = vx1 * vy2 - vy1 * vx2;  // Determinant
        kat1 = -atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
        kat1 = fmod(kat1, Pi2);
        if (kat1 < 0) kat1 += Pi2;

        vx1 = df_x2 - df_cx;
        vy1 = df_y2 - df_cy;
        dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
        det = vx1 * vy2 - vy1 * vx2;  // Determinant
        kat2 = -atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
        kat2 = fmod(kat2, Pi2);
        if (kat2 < 0) kat2 += Pi2;

        s_ellipticarc.obiekt = Oellipticalarc;
        s_ellipticarc.blok = ptrs_arc->blok;
        s_ellipticarc.atrybut = ptrs_arc->atrybut;
        s_ellipticarc.warstwa = ptrs_arc->warstwa;
        s_ellipticarc.kolor = ptrs_arc->kolor;
        s_ellipticarc.typ = ptrs_arc->typ;
        s_ellipticarc.obiektt1 = ptrs_arc->obiektt1;
        s_ellipticarc.obiektt2 = ptrs_arc->obiektt2;
        s_ellipticarc.obiektt3 = ptrs_arc->obiektt3;
        s_ellipticarc.x = df_cx;
        s_ellipticarc.y = df_cy;
        s_ellipticarc.rx = ptrs_arc->r * df_sx;
        s_ellipticarc.ry = ptrs_arc->r * df_sy;
        s_ellipticarc.angle = 0.0;
        s_ellipticarc.kat1 = kat1;
        s_ellipticarc.kat2 = kat2;
        s_ellipticarc.widoczny = ptrs_arc->widoczny;
        s_ellipticarc.przec = ptrs_arc->przec;

        if ((ea = korekta_obiekt((void *) ptrs_arc, (void *) &s_ellipticarc)) == NULL) return NULL;

        return (char*)ea;
    }


  df_s0x = fabs (df_sx) / df_sx ;
  df_s0y = fabs (df_sy) / df_sy ;
  memcpy (&s_arc, ptrs_arc, sizeof(LUK)) ;
  if (FALSE == Check_if_Equal (fabs (ks1), fabs (ks2)))
  {
    Scale_Arc (&s_arc, df_basex, df_basey, df_s0x, df_s0y) ;
  }
  Scale_Point (fabs (df_sx), fabs (df_sy), df_basex, df_basey,
		s_arc.x, s_arc.y,
		&df_cx, &df_cy) ;
  df_xaxis = s_arc.r * fabs (df_sx) ;
  df_yaxis = s_arc.r * fabs (df_sy) ;
  c_desc = PL_ELLIPSE_ARC ;
  i_make_type = DE_ADD_Ell_Arc_SGP ;
  if (NULL == (ptrs_block = object_to_block ((void *)ptrs_arc, c_desc)))
  {
    return (char*)ptrs_arc ;
  }
  b_ret = Make_Ellipse_Arc (df_cx, df_cy, df_xaxis, df_yaxis,
		   &ptrs_block, (void*)&s_arc, i_make_type, O2BlockPline) ;
  return (char*)ptrs_block ;

}

static char *trans_scale_ellipticalarc (ELLIPTICALARC   *ptrs_ellipticalarc,
double df_basex,
double df_basey,
double df_sx,
double df_sy,
double enforce)
/*---------------------------*/
{
double df_cx, df_cy, df_xaxis, df_yaxis, df_angle, df_sangle, df_eangle ;
BOOL b_ret ;
BLOK *ptrs_block ;
ELLIPTICALARC s_ellipticalarc ;
char c_desc ;
int i_make_type ;
double df_s0x, df_s0y ;

//STEP 1

memcpy (&s_ellipticalarc, ptrs_ellipticalarc, sizeof(ELLIPTICALARC)) ;

df_cx=ptrs_ellipticalarc->x;
df_cy=ptrs_ellipticalarc->y;

df_xaxis = s_ellipticalarc.rx * fabs (df_sx) ;
df_yaxis = s_ellipticalarc.ry * fabs (df_sy) ;

df_angle=s_ellipticalarc.angle;
df_sangle=s_ellipticalarc.kat1;
df_eangle=s_ellipticalarc.kat2;

c_desc = PL_ELLIPSE_ARC ;
i_make_type = DE_ADD_Ell_Arc_SGP ;
if (NULL == (ptrs_block = object_to_block ((void *)ptrs_ellipticalarc, c_desc)))
{
return (char*)ptrs_ellipticalarc ;
}

i_make_type = i_make_type = DE_ADD_Ell_Arc_SGP ;
b_ret = Make_EllipticalArc (df_cx, df_cy, df_xaxis, df_yaxis, df_angle, df_sangle, df_eangle, 0,
                          &ptrs_block, (void*)&s_ellipticalarc, i_make_type, Anormalny, Guma, O2BlockPline, 0, s_ellipticalarc.kolor, 1) ;

//STEP 2

transformacja_blok((char*)ptrs_block,(char*)ptrs_block+ptrs_block->n-1,df_basex,df_basey, df_sx, df_sy, Tskala,0);

return (char *)ptrs_block ;
}


BOOL trans_scale_arc_shadow(LUK    *ptrs_arc,
	double df_basex,
	double df_basey,
	double df_sx,
	double df_sy,
    int to_block)
	/*---------------------------*/
{
	double df_cx, df_cy, df_xaxis, df_yaxis;
	BOOL b_ret;
	LUK s_arc;
	char c_desc;
	int i_make_type;
	double df_s0x, df_s0y;
    BLOK *ptr_block;

	df_s0x = fabs(df_sx) / df_sx;
	df_s0y = fabs(df_sy) / df_sy;
	memcpy(&s_arc, ptrs_arc, sizeof(LUK));
	if (FALSE == Check_if_Equal(fabs(ks1), fabs(ks2)))
	{
		Scale_Arc(&s_arc, df_basex, df_basey, df_s0x, df_s0y);
	}
	Scale_Point(fabs(df_sx), fabs(df_sy), df_basex, df_basey,
                (double)s_arc.x, (double)s_arc.y,
		&df_cx, &df_cy);
	df_xaxis = (double)s_arc.r * fabs(df_sx);
	df_yaxis = (double)s_arc.r * fabs(df_sy);
	c_desc = PL_ELLIPSE_ARC;
	i_make_type = DE_ADD_Ell_Arc_SGP;

    ptr_block=add_shadow_block(Ablok, to_block);

	s_arc.atrybut = Ablok;

	b_ret = Make_Ellipse_Arc_Shadow(df_cx, df_cy, df_xaxis, df_yaxis,
                                    ptr_block, (void*)&s_arc, i_make_type, O2BlockPline);
	return b_ret;
}




static char* trans_scale_circle (OKRAG *ptrs_circle,
		    double df_basex,
		    double df_basey,
		    double df_sx,
		    double df_sy,
            int type,
            BOOL enforce)
/*------------------------------*/
{
  double df_cx, df_cy, df_xaxis, df_yaxis, df_angle ;
  int i_mode ;
  BOOL b_ret ;
  BLOK *ptrs_block ;
  ELLIPSE s_ellipse = Edef;
  OKRAG s_circle = Odef;
  ELLIPSE *e;
  char c_desc ;
  int i_make_type ;

  Scale_Point (df_sx, df_sy ,df_basex, df_basey,
		ptrs_circle->x, ptrs_circle->y,
		&df_cx, &df_cy);

  df_xaxis = ptrs_circle->r * fabs (df_sx) ;
  df_yaxis = ptrs_circle->r * fabs (df_sy) ;

  df_angle = 0 ;
  i_mode = 0 ;

  if (!enforce) {

      s_ellipse.obiekt = (ptrs_circle->obiekt == Ookrag) ? Oellipse : Ofilledellipse;
      s_ellipse.blok = ptrs_circle->blok;
      s_ellipse.atrybut = ptrs_circle->atrybut;
      s_ellipse.warstwa = ptrs_circle->warstwa;
      s_ellipse.kolor = ptrs_circle->kolor;
      s_ellipse.typ = ptrs_circle->typ;
      s_ellipse.obiektt1 = ptrs_circle->obiektt1;
      s_ellipse.obiektt2 = ptrs_circle->obiektt2;
      s_ellipse.obiektt3 = ptrs_circle->obiektt3;
      s_ellipse.x = df_cx;
      s_ellipse.y = df_cy;
      s_ellipse.rx = ptrs_circle->r * df_sx;
      s_ellipse.ry = ptrs_circle->r * df_sy;
      s_ellipse.angle = 0.0;
      s_ellipse.translucency = 255;
      s_ellipse.widoczny = ptrs_circle->widoczny;
      s_ellipse.przec = ptrs_circle->przec;

      if ((e = korekta_obiekt((void *) ptrs_circle, (void *) &s_ellipse)) == NULL) return NULL;

      return (char*)e;
  }


  memcpy (&s_circle, ptrs_circle, sizeof(OKRAG)) ;
  c_desc = (ptrs_circle->obiekt == Ookrag) ? PL_ELLIPSE : PL_ELLIPSE_FILL ;
  i_make_type = (ptrs_circle->obiekt == Ookrag) ? DE_Add_SGP : DE_Add_Ell_Fill_SGP ;
  if (NULL == (ptrs_block = object_to_block ((void *)ptrs_circle, c_desc)))
  {
    return ptrs_circle ;
  }
  b_ret = Make_Ellipse (df_cx, df_cy, df_xaxis, df_yaxis, df_angle,
		   i_mode, &ptrs_block, (void*)&s_circle, i_make_type, O2BlockPline, type, 255, ptrs_circle->kolor, 1) ;

  return ptrs_block ;
}

static char *trans_scale_ellipse(ELLIPSE *ptrs_ellipse,
double df_basex,
double df_basey,
double df_sx,
double df_sy,
int type,
BOOL enforce)
/*------------------------------*/
{
double df_cx, df_cy, df_xaxis, df_yaxis, df_angle ;
int i_mode ;
BOOL b_ret ;
BLOK *ptrs_block ;
ELLIPSE s_ellipse ;
char c_desc ;
int i_make_type ;


//STEP 1

df_cx=ptrs_ellipse->x;
df_cy=ptrs_ellipse->y;

df_xaxis = ptrs_ellipse->rx; // * fabs (df_sx) ;
df_yaxis = ptrs_ellipse->ry ; //* fabs (df_sy) ;  //in first step scale 1;1
df_angle = ptrs_ellipse->angle;

i_mode = 0 ;
memcpy (&s_ellipse, ptrs_ellipse, sizeof(ELLIPSE)) ;
c_desc = (ptrs_ellipse->obiekt == Oellipse) ? PL_ELLIPSE : PL_ELLIPSE_FILL ;
i_make_type = (ptrs_ellipse->obiekt == Oellipse) ? DE_Add_SGP : DE_Add_Ell_Fill_SGP ;
if (NULL == (ptrs_block = object_to_block ((void *)ptrs_ellipse, c_desc)))
{
return ptrs_ellipse ;
}

b_ret = Make_Ellipse (df_cx, df_cy, df_xaxis, df_yaxis, df_angle, i_mode, &ptrs_block, (void*)&s_ellipse, i_make_type, O2BlockPline, type, s_ellipse.translucency, ptrs_ellipse->kolor, 1) ;

//STEP 2

transformacja_blok(ptrs_block,(char*) (long_long)ptrs_block+sizeof(NAGLOWEK)+ptrs_block->n-1,df_basex,df_basey, df_sx, df_sy, Tskala,0);

return ptrs_block ;
}

BOOL trans_scale_circle_shadow(OKRAG *ptrs_circle,
	double df_basex,
	double df_basey,
	double df_sx,
	double df_sy,
    int to_block)
	/*------------------------------*/
{
	double df_cx, df_cy, df_xaxis, df_yaxis, df_angle;
	int i_mode;
	BOOL b_ret;
	OKRAG s_circle;
	char c_desc;
	int i_make_type;
    BLOK *ptr_block;

	Scale_Point(df_sx, df_sy, df_basex, df_basey,
		ptrs_circle->x, ptrs_circle->y,
		&df_cx, &df_cy);
	df_xaxis = ptrs_circle->r * fabs(df_sx);
	df_yaxis = ptrs_circle->r * fabs(df_sy);
	df_angle = 0;
	i_mode = 0;
	memcpy(&s_circle, ptrs_circle, sizeof(OKRAG));
	c_desc = (ptrs_circle->obiekt == Ookrag) ? PL_ELLIPSE : PL_ELLIPSE_FILL;
	i_make_type = (ptrs_circle->obiekt == Ookrag) ? DE_Add_SGP : DE_Add_Ell_Fill_SGP;

    ptr_block=add_shadow_block(Ablok, to_block);

	s_circle.atrybut = Ablok;

	b_ret = Make_Ellipse_Shadow(df_cx, df_cy, df_xaxis, df_yaxis, df_angle,
		i_mode, ptr_block, (void*)&s_circle, i_make_type, O2BlockPline);
	return b_ret;
}

//static
BOOL trans_scale_arcs (double df_basex,
		  double df_basey,
		  double df_sx,
		  double df_sy,
          BOOL enforce)
/*-----------------------------*/
{
  long_long l_offp, l_off ;
  NAGLOWEK *nag ;
  BLOK *ptrs_bl ;
  ELLIPSE *ptrs_el ;
  ELLIPTICALARC *ptrs_ea ;
  BOOL b_ret ;

  b_ret = TRUE ;
  l__Off_Bl_Beg = (long_long)(ADP - dane) ;
  l__Off_Bl_End = (long_long)(ADK - dane) ;
  l_offp = l__Off_Bl_Beg ;

  ObiektTok (&l_offp, l__Off_Bl_End, &l_off, ONieOkreslony) ;
  while (l_off != -1)
  {
    nag = (NAGLOWEK*)(dane + l_off) ;
    if (nag->atrybut == Ablok)
    {
      if (nag->obiekt == Oluk)
      {
        nag = trans_scale_arc ((LUK*)nag, df_basex, df_basey, df_sx, df_sy, enforce) ;
        if (nag==NULL) return FALSE;
        ptrs_ea = (ELLIPTICALARC *)(nag) ;
        l_offp = (long_long)nag + sizeof(NAGLOWEK) + ptrs_ea->n - (long_long)dane;
        l__Off_Bl_End = (long_long)(ADK - dane) ;
      }
      else if (nag->obiekt == Oellipticalarc)
        {
            if (Check_if_Equal(fmod(((ELLIPTICALARC *)nag)->angle, Pi/2.0), 0.0)==FALSE) {
                ptrs_bl = trans_scale_ellipticalarc((ELLIPTICALARC *) nag, df_basex, df_basey, df_sx, df_sy, enforce);
                if (ptrs_bl == NULL) return FALSE;
                l_offp = (long_long) ptrs_bl + sizeof(NAGLOWEK) + ptrs_bl->n - (long_long) dane;
                l__Off_Bl_End = (long_long) (ADK - dane);
            }
        }
      else if (nag->obiekt == Okolo || nag->obiekt == Ookrag)
      {
        nag = trans_scale_circle ((OKRAG *)nag, df_basex, df_basey, df_sx, df_sy, 0, enforce) ;
        if (nag==NULL) return FALSE;
        ptrs_el = (ELLIPSE*)(nag) ;
        l_offp = (long_long)nag + sizeof(NAGLOWEK) + ptrs_el->n - (long_long)dane;
        l__Off_Bl_End = (long_long)(ADK - dane) ;
      }
      else if (nag->obiekt == Ofilledellipse || nag->obiekt == Oellipse)
      {
          if (Check_if_Equal(fmod(((ELLIPSE *)nag)->angle, Pi/2.0), 0.0)==FALSE) {
              ptrs_bl = trans_scale_ellipse((ELLIPSE *) nag, df_basex, df_basey, df_sx, df_sy, 0, enforce);
              if (ptrs_bl==NULL) return FALSE;
              l_offp = (long_long)ptrs_bl + sizeof(NAGLOWEK) + ptrs_bl->n - (long_long)dane;
              l__Off_Bl_End = (long_long)(ADK - dane) ;
          }
      }
      if (b_ret == FALSE)
      {
	break ;
      }
    }
    ObiektTok (&l_offp, l__Off_Bl_End, &l_off, ONieOkreslony) ;
  }
  return b_ret ;
}

BOOL trans_scale_arcs_ (char *ADP_b, char *ADK_b, double df_basex,
                  double df_basey,
                  double df_sx,
                  double df_sy,
                  BOOL enforce)
/*-----------------------------*/
{
    long_long l_offp, l_off ;
    NAGLOWEK *nag ;
    BLOK *ptrs_bl ;
    char *b_ret;

    b_ret=NULL;
    l__Off_Bl_Beg = (long_long)(ADP_b - dane) ;
    l__Off_Bl_End = (long_long)(ADK_b - dane) ;
    l_offp = l__Off_Bl_Beg ;

    ObiektTok (&l_offp, l__Off_Bl_End, &l_off, ONieOkreslony) ;
    while (l_off != -1)
    {
        nag = (NAGLOWEK*)(dane + l_off) ;
        if (nag->atrybut == Ablok)
        {
            if (nag->obiekt == Oluk)
            {
                b_ret = trans_scale_arc ((LUK*)nag, df_basex, df_basey, df_sx, df_sy, enforce) ;
                if (b_ret == NULL) return FALSE;
                nag = (NAGLOWEK*)(dane + l_off) ;
                l_offp = l_off + sizeof(NAGLOWEK) + nag->n ;
            }
            else if (nag->obiekt == Oellipticalarc)
            {
                b_ret = trans_scale_ellipticalarc((ELLIPTICALARC *)nag, df_basex, df_basey, df_sx, df_sy, enforce) ;
                if (b_ret == NULL) return FALSE;
                ptrs_bl = (BLOK*)(dane + l_off) ;
                l_offp = l_off + sizeof(NAGLOWEK) + ptrs_bl->n ;
            }
            else if (nag->obiekt == Okolo || nag->obiekt == Ookrag)
            {
                b_ret = trans_scale_circle ((OKRAG *)nag, df_basex, df_basey, df_sx, df_sy, 0, enforce) ;
                if (b_ret == NULL) return FALSE;
                nag = (NAGLOWEK*)(dane + l_off) ;
                l_offp = l_off + sizeof(NAGLOWEK) + nag->n ;
            }
            else if (nag->obiekt == Oellipse || nag->obiekt == Ofilledellipse)
            {
                b_ret = trans_scale_ellipse ((ELLIPSE *)nag, df_basex, df_basey, df_sx, df_sy, 0, enforce) ;
                if (b_ret == NULL) return FALSE;
                ptrs_bl = (BLOK*)(dane + l_off) ;
                l_offp = l_off + sizeof(NAGLOWEK) + ptrs_bl->n ;
            }
        }
        ObiektTok (&l_offp, l__Off_Bl_End, &l_off, ONieOkreslony) ;
    }
    return TRUE;
}



BOOL trans_scale_arcs__ (double df_basex, double df_basey, double df_sx, double df_sy)
{
    return trans_scale_arcs (df_basex, df_basey, df_sx, df_sy, FALSE);
}

static void out_parametry_skali (double k)
/*----------------------------------------*/
{
  char buf [100];

  buf [0] = '\0' ;
  sprintf (buf, "%#12.9lg", k) ;
  strcpy(eL.st, buf);
  Out_Edited_Draw_Param ((ESTR *)&eL, TRUE) ;
}


static void  cur_offS(double x, double y)
{
    flip_screen();
}


static void  cur_offS_(double x,double y)
{ out_blok1(Px,Py,ks1,ks2,Tskala,0);
  outlineor(&L,XOR_PUT,0);
  cursel_off(x,y);
}

static void  cur_onS(double x,double y)
{
  cursel_on(x, y); ////
  L.x2=x; L.y2=y;
  outlineor(&L,XOR_PUT,0);
  ks1 = ks2 = 10*sqrt((x-Px)*(x-Px)+(y-Py)*(y-Py))/(Xk-Xp);
  out_parametry_skali(ks1);
  out_blok1(Px,Py, ks1, ks2, Tskala,0);
  cursel_on(x, y);
}

static void  cur_onS_(double x,double y)
{   cursel_on(x,y);
    L.x2=x; L.y2=y;
    outlineor(&L,XOR_PUT,0);
    ks1 = ks2 = 10*sqrt((x-Px)*(x-Px)+(y-Py)*(y-Py))/(Xk-Xp);
    out_blok1(Px,Py, ks1, ks2, Tskala,0);
    cursel_on(x,y);
}

static int el_k (BOOL b_graph_value)
{
  b_graph_value = b_graph_value ;
  if (eL.val_no < 1) return 0;
  k02 = k01 = eL.values [0] ;
  if ( eL.val_no > 1)
  {
    k02 = eL.values [1] ;
  }
  if (k01 == 0 || k02 == 0)
  {
    return 0;
  }
  strwyj=1;
  return 1;
}



static void redcrs(char typ)
{
  static int np;
  static int ( *SW[2])();
  switch(typ)
   { case 0 :
       komunikat0(28);
       Px=X;Py=Y;
       ks1 = ks2 = 0;
       L.x1=L.x2=X;
       L.y1=L.y2=Y;
       eL.x = maxX - PL266 ;
       eL.y = ESTR_Y;
       eL.lmax = r22 ;
       eL.val_no_max = 2 ;
       eL.mode = GV_NUMBER ;
       eL.format = "%#12.9lg\0%#8.3lf;%#8.3lf" ;
       eL.ESTRF=el_k;
	   eL.extend = 0;
       np=dodajstr(&eL);
       CUR_OFF(X,Y);
       blokzap(ADP,ADK,Ablok,XOR_PUT,1);
       CUR_OFF=cur_offS;
       CUR_ON=cur_onS;
       mvcurbp(1);
       Cur_ond(X,Y);
       SW [0] = SERV [73] ;  SERV [73] = sel_t ;
       SW [1] = SERV [81] ;  SERV [81] = sel_d ;
       break;
     case 1 :
       Cur_offd(X,Y);
     case 2 :
       CUR_ON=cur_onS_;
       usunstr(np);
       SERV [73] = (void *)SW [0] ;
       SERV [81] = (void *)SW [1] ;
       break;
     default : break;
   }
}

int Scale_Proc (void)
/*------------------*/
{
  double X0,Y0;
  EVENT *ev;

  redcrs(0);
  strwyj=0;
  while(1)
   { ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0) { redcrs(1); return ESC; }
	 if (ev->What == evCommandP && ev->Number == 0)
	 {
		 set_ellipse_poligon_num(192);
	 }
     if((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
      { Cur_offd(X,Y);
	blokzap(ADP,ADK,Ablok,COPY_PUT,0);
	if(strwyj)
	{
	  ks1 = k01;
	  ks2 = k02;
	}
	transformacja_blok(ADP,ADK,Px,Py, ks1, ks2, Tskala,0);
	if (FALSE == Check_if_Equal (fabs (ks1), fabs (ks2)))
	{
	  trans_scale_arcs (Px,Py, ks1, ks2, FALSE) ;
	  set_block_change_mode_scale ((long)(ADP - dane), (long)(ADK - dane)) ;
	}
	blokzap(ADP,ADK,Ablok,COPY_PUT,1);
	redcrs(2);
	return ENTER;
      }
   }
}
/*-----obrot------------------------------------*/

static double kos0,koc0;
static double kos,koc;

static void  cur_offO(double x,double y)
{ flip_screen();
}

static void  cur_offO__(double x,double y)
{ out_blok1(Px,Py,kos,koc,Tobrot,0);
  outlineor(&L,XOR_PUT,0);
  cursel_off(x,y);
}


static void  cur_offOXZ(double x,double y)
{ flip_screen();
}

static void  cur_offOXZ__(double x,double y)
{ out_blok1(Px,Py,kos,koc,TobrotXZ,Z0);
  outlineor(&L,XOR_PUT,0);
  cursel_off(x,y);
}

static void  cur_offOYZ(double x,double y)
{ flip_screen();
}

static void  cur_offOYZ__(double x,double y)
{ out_blok1(Px,Py,kos,koc,TobrotYZ,Z0);
  outlineor(&L,XOR_PUT,0);
  cursel_off(x,y);
}

static void  cur_onO(double x,double y)
{ double l,kat;
  cursel_on(x,y);
  L.x2=x; L.y2=y;
  outlineor(&L,COPY_PUT,0);
  l=sqrt((x-Px)*(x-Px)+(y-Py)*(y-Py));
  if(l) { koc=(x-Px)/l; kos=(y-Py)/l; kat=Atan2(y-Py,x-Px); }
  else  { koc=1; kos=0; kat=0; }
  if(kat<0) kat=2*Pi+kat;
  out_parametry_skali(kat*180/Pi);
  out_blok1(Px,Py,kos,koc,Tobrot,0);
  cursel_on(x,y);
}

static void  cur_onOXZ(double x,double y)
{ double l,kat;
  cursel_on(x,y);
  L.x2=x; L.y2=y;
  outlineor(&L,COPY_PUT,0);
  l=sqrt((x-Px)*(x-Px)+(y-Py)*(y-Py));
  if(l) { koc=(x-Px)/l; kos=(y-Py)/l; kat=Atan2(y-Py,x-Px); }
  else  { koc=1; kos=0; kat=0; }
  if(kat<0) kat=2*Pi+kat;
  out_parametry_skali(kat*180/Pi);
  out_blok1(Px,Py,kos,koc,TobrotXZ,Z0);
  cursel_on(x,y);
}

static void  cur_onOYZ(double x,double y)
{ double l,kat;
  cursel_on(x,y);
  L.x2=x; L.y2=y;
  outlineor(&L,COPY_PUT,0);
  l=sqrt((x-Px)*(x-Px)+(y-Py)*(y-Py));
  if(l) { koc=(x-Px)/l; kos=(y-Py)/l; kat=Atan2(y-Py,x-Px); }
  else  { koc=1; kos=0; kat=0; }
  if(kat<0) kat=2*Pi+kat;
  out_parametry_skali(kat*180/Pi);
  out_blok1(Px,Py,kos,koc,TobrotYZ,Z0);
  cursel_on(x,y);
}


static int el_o (BOOL b_graph_value)
/*------------------*/
{
  double a;

  b_graph_value = b_graph_value ;
  if (eL.val_no < 1) return 0;
  a = eL.values [0] ;
  kos0 = sin (a * Pi / 180) ;
  koc0 = cos (a * Pi / 180) ;
  strwyj = 1 ;
  return 1 ;
}

static void redcro(char typ)
{
  static int np;
  static int ( *SW[2])();
  switch(typ)
   { case 0 :
       komunikat0 (33) ;
       Px=X;Py=Y;
       kos=0;
       koc=1;
       L.x1=L.x2=X;
       L.y1=L.y2=Y;
       eL.x=maxX-PL266;
       eL.y= ESTR_Y;
       eL.lmax=r22;
       eL.val_no_max = 1 ;
       eL.mode = GV_ANGLE ;
       eL.format = "%#10.5lg" ;
       eL.ESTRF=el_o;
	   eL.extend = 0;
       np=dodajstr(&eL);
       CUR_OFF(X,Y);
       blokzap(ADP,ADK,Ablok,XOR_PUT,1);
       CUR_OFF=cur_offO;
       CUR_ON=cur_onO;
       mvcurbp(1);
       Cur_ond(X,Y);
       SW [0] = SERV [73] ;  SERV [73] = sel_t ;
       SW [1] = SERV [81] ;  SERV [81] = sel_d ;
       break;
     case 1 :
       Cur_offd(X,Y);
     case 2 :
       usunstr(np);
       SERV [73] = (void *)SW [0] ;
       SERV [81] = (void *)SW [1] ;
       break;
     default : break;
   }
}

static void redcroXZ(char typ)
{
  static int np;
  static int ( *SW[2])();
  switch(typ)
   { case 0 :
       komunikat0 (33) ;
       Px=X;Py=Y;
       kos=0;
       koc=1;
       L.x1=L.x2=X;
       L.y1=L.y2=Y;
       eL.x=maxX-PL266;
       eL.y= ESTR_Y;
       eL.lmax=r22;
       eL.val_no_max = 1 ;
       eL.mode = GV_ANGLE ;
       eL.format = "%#10.5lg" ;
       eL.ESTRF=el_o;
	   eL.extend = 0;
       np=dodajstr(&eL);
       CUR_OFF(X,Y);
       blokzap(ADP,ADK,Ablok,XOR_PUT,1);
       CUR_OFF=cur_offOXZ;
       CUR_ON=cur_onOXZ;
       mvcurbp(1);
       Cur_ond(X,Y);
       SW [0] = SERV [73] ;  SERV [73] = sel_t ;
       SW [1] = SERV [81] ;  SERV [81] = sel_d ;
       break;
     case 1 :
       Cur_offd(X,Y);
     case 2 :
       usunstr(np);
       SERV [73] = (void*)SW [0] ;
       SERV [81] = (void*)SW [1] ;
       break;
     default : break;
   }
}

static void redcroYZ(char typ)
{
  static int np;
  static int ( *SW[2])();
  switch(typ)
   { case 0 :
       komunikat0 (33) ;
       Px=X;Py=Y;
       kos=0;
       koc=1;
       L.x1=L.x2=X;
       L.y1=L.y2=Y;
       eL.x=maxX-PL266;
       eL.y= ESTR_Y;
       eL.lmax=r22;
       eL.val_no_max = 1 ;
       eL.mode = GV_ANGLE ;
       eL.format = "%#10.5lg" ;
       eL.ESTRF=el_o;
	   eL.extend = 0;
       np=dodajstr(&eL);
       CUR_OFF(X,Y);
       blokzap(ADP,ADK,Ablok,XOR_PUT,1);
       CUR_OFF=cur_offOYZ;
       CUR_ON=cur_onOYZ;
       mvcurbp(1);
       Cur_ond(X,Y);
       SW [0] = SERV [73] ;  SERV [73] = sel_t ;
       SW [1] = SERV [81] ;  SERV [81] = sel_d ;
       break;
     case 1 :
       Cur_offd(X,Y);
     case 2 :
       usunstr(np);
       SERV [73] = (void*)SW [0] ;
       SERV [81] = (void*)SW [1] ;
       break;
     default : break;
   }
}


int Rotate_Proc (void)
/*-------------------*/
{
  double X0,Y0;
  EVENT *ev;

  redcro(0);
  strwyj=0;
  while(1)
   {
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0) { redcro(1); return ESC; }
     if((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
      { Cur_offd(X,Y);
	blokzap(ADP,ADK,Ablok,COPY_PUT,0);
	if(strwyj) { kos=kos0;koc=koc0; }
	transformacja_blok(ADP,ADK,Px,Py,kos,koc,Tobrot,0);
	blokzap(ADP,ADK,Ablok,COPY_PUT,1);
	redcro(2);
	return ENTER;
      }
   }
}
int Rotate_ProcXZ (void)
/*-------------------*/
{
  double X0,Y0;
  EVENT *ev;
  char sk [MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;

  redcroXZ(0);
  strwyj=0;
  sprintf (sk, "%f", Z0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 144)) return ESC;
  if (FALSE == calculator (sk, &retval_no, buf_ret) || retval_no < 1)
   {
    return ESC;
   }
  Z0 = milimetryob(buf_ret [0]) ;
  while(1)
   {
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0) { redcroXZ(1); return ESC; }
     if((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
      { Cur_offd(X,Y);
	blokzap(ADP,ADK,Ablok,COPY_PUT,0);
	if(strwyj) { kos=kos0;koc=koc0; }
	transformacja_blok(ADP,ADK,Px,Py,kos,koc,TobrotXZ,Z0);
	blokzap(ADP,ADK,Ablok,COPY_PUT,1);
	redcroXZ(2);
	return ENTER;
      }
   }
   return ESC;
}

int Rotate_ProcYZ (void)
/*-------------------*/
{
  double X0,Y0;
  EVENT *ev;
  char sk [MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;

  redcroYZ(0);
  strwyj=0;

  sprintf (sk, "%f", Z0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 144)) return ESC;
  if (FALSE == calculator (sk, &retval_no, buf_ret) || retval_no < 1)
   {
    return ESC;
   }
  Z0 = milimetryob(buf_ret [0]) ;
  while(1)
   {
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0) { redcroYZ(1); return ESC; }
     if((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
      { Cur_offd(X,Y);
	blokzap(ADP,ADK,Ablok,COPY_PUT,0);
	if(strwyj) { kos=kos0;koc=koc0; }
	transformacja_blok(ADP,ADK,Px,Py,kos,koc,TobrotYZ,Z0);
	blokzap(ADP,ADK,Ablok,COPY_PUT,1);
	redcroYZ(2);
	return ENTER;
      }
   }
   return ESC;
}




/*------Set_Block-----------------------------------------------------*/
BOOL Set_Block_Proc (double x, double y, char *blok_type)
/*-----------------------------------------------------------*/
{
  unsigned		size_block = B3 ;
  int 			len_type, len_desc ;
  long_long 	l_size_block ;
#ifndef LINUX
  BLOKD 	blokd = BDdef ;
#else
    BLOK 	blokd = BDdef ;
#endif
  BLOK 			*ptrs_block, *buf_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  long_long off, offp, offk, l_size_object ;
  NAGLOWEK *nag, *ptrs_nag ;
  BOOL b_ret = FALSE ;

  if (ADP == NULL || ADK == NULL)
  {
    return b_ret;
  }
  l_size_block = Get_Size_Block (ADP - dane, ADK - dane) ;
  if (l_size_block >= INT_MAX)
  {
    ErrList (21) ;
    return b_ret;
  }
  len_type = (int)strlen (blok_type) + 1 ;
  len_desc = sizeof(unsigned) + 2 * sizeof (float) + sizeof(len_type) + len_type ;
  size_block += len_desc ;
  if (NULL == (buf_block = (BLOK*)malloc /*getmem*/ (sizeof(NAGLOWEK) + size_block)))
  {
    return b_ret;
  }
  memcpy (buf_block, &blokd, sizeof (blokd)) ;
  buf_block->n = size_block ;
  buf_block->kod_obiektu = 'E' ;
  buf_block->dlugosc_opisu_obiektu = len_desc ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&buf_block->opis_obiektu [0]) ;
  ptrs_desc_bl->flags = EBF_IP ;
  ptrs_desc_bl->x = x ;
  ptrs_desc_bl->y = y ;
  ptrs_desc_bl->len = len_type ;
  strcpy (&ptrs_desc_bl->sz_type [0], blok_type) ;
  if ((ptrs_block = (BLOK*)dodaj_obiekt (NULL, buf_block)) == NULL)
  {
      free(buf_block);
    return b_ret;
  }
  free(buf_block) ;
  offp = ADP - dane ;  	/*dodanie obiektu 'przesuwa' dane, a nie zmienia ADP i ADK*/
  offk = ADK - dane ;
  ObiektTok (&offp, offk, &off, ONoInBlock) ;
  b_ret = TRUE ;
  while (off != -1)
  {
    nag =(NAGLOWEK*)(dane + off) ;
    l_size_object = sizeof(NAGLOWEK) + (long)nag->n ;
    if (nag->atrybut == Ablok)
    {
      if (NULL == (ptrs_nag = (NAGLOWEK*)Copy_ObjectB (&ptrs_block, (char  *)nag)))
      {
	b_ret = FALSE ;
	break ;
      }
      ptrs_nag->blok = ElemBlok ;
      Usun_Object ((void *)(dane + off + l_size_object), TRUE) ;
      ptrs_block = (BLOK*)dane ;
    }
    ObiektTok (&offp, offk, &off, ONoInBlock) ;
    b_ret = TRUE ;
  }
  ADP = dane ;
  if (b_ret == TRUE)
  {
	  ADK = dane + sizeof(NAGLOWEK) + ptrs_block->n  -1;
  }
  return b_ret ;
}

/*-----------------------------------------------*/
