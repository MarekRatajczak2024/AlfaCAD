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

#define __O_OFFSET__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<math.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_offset.h"
#include "b_polyline_offset.h"

#include "menu.h"

#include "leak_detector_c.h"

extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;
extern BLOK *LASTB(char  *ado) ;
extern char * Copy_Object (char  *) ;
extern double Dist_PP (double, double, double, double) ;
extern void Usun_Object (void *, BOOL ) ;
extern void Redraw_Block (void) ;
extern void obru(double si,double co, double xp, double yp, double *xn,double *yn) ;
extern void obrd(double si,double co, double xp, double yp, double *xn, double *yn) ;
extern void get_polyline_offset_data(int size, VERTEX* vertex, int* capacity, int* sizes, int *closes, BOOL closed, double distance, BOOL smooth_vertex);

extern void store_dane0(void);
extern void restore_dane0(void);

extern void delete_spline_shadows(BOOL make_Ablok);
extern void	layer_info (void);
extern BOOL Check_Attribute (int i_attribute, int i_check);
extern BOOL Check_if_Close_Pline (BLOK *) ;
extern int make_spline_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block);
extern void POLYARC(double x_s, double y_s, double x_e, double y_e, double wpsc, LUK *l);


BOOL offset_smooth = FALSE;


//enum OFFSET_MODE { POINT_OFFSET_MODE, DIST_OFFSET_MODE, SET_DIST_OFFSET, SET_OFFSET_STYLE, SET_OFFSET_NORMAL, SET_OFFSET_SMOOTH,
//} ;
static int i__offset_mode = DIST_OFFSET_MODE ; //POINT_OFFSET_MODE ;
static double df__dist_off = 2 ;
static int np = -1 ;		/*numer pola edycjnego dystansu*/

#define r18 18
#define r22 16

#define VERTEX_D_MIN 0.01
#define VERTEX_B_MIN 0.001

static TMENU mOffset = {4, 0,0,26,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr, 0,0,0,&pmOffset, NULL,NULL} ;

static void (*CUR_oN0)(double ,double);
static void (*CUR_oFF0)(double ,double);

int get_i__offset_mode(void)
{
    return i__offset_mode;
}

void set_i__offset_mode(int offs_mode)
{
    i__offset_mode=offs_mode;
}

static BOOL translate_offset_line (void *ptr_ob, double df_x, double df_y)
/*----------------------------------------------------------------------*/
{
  double df_dist ;
  double x01, y01, x1, y1, x2, y2, a, si, co, df_l ;
  LINIA  *ptrs_line ;

  ptrs_line = (LINIA *)ptr_ob ;
  x1 = ptrs_line->x1 ;
  y1 = ptrs_line->y1 ;
  x2 = ptrs_line->x2 ;
  y2 = ptrs_line->y2 ;

  x01 = x1 ;
  y01 = y1 ;
  x2 -= x1 ;
  y2 -= y1 ;
  df_x -= x1 ;
  df_y -= y1 ;
  a = Atan2 (y2, x2) ;
  si = sin (a) ;
  co = cos (a) ;
  obru (si, co, x2, y2, &x2, &y2) ;     /*po obrocie y2 = 0*/
  df_l = fabs (x2) ;			/*dlugosc linii*/
  obru (si, co, df_x, df_y, &df_x, &df_y) ;
  if (i__offset_mode == POINT_OFFSET_MODE)
  {
    df_dist = df_y ;
  }
  else
  {
    df_dist = df__dist_off * (df_y > 0 ? 1 : -1) ;
  }
  obrd (si, co, 0, df_dist, &x1, &y1) ;
  obrd (si, co, df_l, df_dist, &x2, &y2) ;
  x1 = x01 + x1 ;
  y1 = y01 + y1 ;
  x2 = x01 + x2 ;
  y2 = y01 + y2 ;
  ptrs_line->x1 = x1 ;
  ptrs_line->y1 = y1 ;
  ptrs_line->x2 = x2 ;
  ptrs_line->y2 = y2 ;

  return TRUE ;
}

BOOL translate_offset_line_distance (void *ptr_ob, double df_x, double df_y, double distance)
{
    BOOL ret;
    double df__dist_off_=df__dist_off;

    df__dist_off=distance;
    ret=translate_offset_line (ptr_ob, df_x, df_y);
    df__dist_off=df__dist_off_;
    return ret;
}

static BOOL find_offset_line(void* ptr_ob, double df_x, double df_y, double *df_dist)
/*----------------------------------------------------------------------------------*/
{
    double x01, y01, x1, y1, x2, y2, a, si, co, df_l;
    LINIA* ptrs_line;
    
    ptrs_line = (LINIA*)ptr_ob;
    x1 = ptrs_line->x1;
    y1 = ptrs_line->y1;
    x2 = ptrs_line->x2;
    y2 = ptrs_line->y2;
    
    x01 = x1;
    y01 = y1;
    x2 -= x1;
    y2 -= y1;
    df_x -= x1;
    df_y -= y1;
    a = Atan2(y2, x2);
    si = sin(a);
    co = cos(a);
    obru(si, co, x2, y2, &x2, &y2);     /*po obrocie y2 = 0*/
    df_l = fabs(x2);			/*dlugosc linii*/
    obru(si, co, df_x, df_y, &df_x, &df_y);
    if (i__offset_mode == POINT_OFFSET_MODE)
    {
        *df_dist = df_y;
    }
    else
    {
        *df_dist = df__dist_off * (df_y > 0 ? 1 : -1);
    }
    
    return TRUE;
}

static BOOL translate_offset_arc (void *ptr_ob, double df_x, double df_y)
/*----------------------------------------------------------------------*/
{
  double df_dist ;
  LUK * ptrs_arc ;
  BOOL b_ret ;

  b_ret = TRUE ;
  ptrs_arc = (LUK *)ptr_ob ;
  df_dist = Dist_PP (df_x, df_y, ptrs_arc->x, ptrs_arc->y) ;
  if (i__offset_mode == POINT_OFFSET_MODE)
  {
    ptrs_arc->r = df_dist ;
  }
  else
  if (df_dist >= ptrs_arc->r)
  {
    ptrs_arc->r += df__dist_off ;
  }
  else
  if (ptrs_arc->r > df__dist_off)
  {
    ptrs_arc->r -= df__dist_off ;
  }
  else
  {
     ErrList (73) ;
     b_ret = FALSE ;
  }
  return b_ret ;
}

static BOOL translate_offset_ellipticalarc (void *ptr_ob, double df_x, double df_y)
/*-------------------------------------------------------------------------------*/
{
    double df_dist ;
    ELLIPTICALARC * ptrs_ellipticalarc ;
    BOOL b_ret ;
    double d_r;

    b_ret = TRUE ;
    ptrs_ellipticalarc = (ELLIPTICALARC *)ptr_ob ;
    df_dist = Dist_PP (df_x, df_y, ptrs_ellipticalarc->x, ptrs_ellipticalarc->y) ;
    d_r=ptrs_ellipticalarc->ry/ptrs_ellipticalarc->rx;

    if (i__offset_mode == POINT_OFFSET_MODE)
    {
        ptrs_ellipticalarc->rx = df_dist ;
        ptrs_ellipticalarc->ry = ptrs_ellipticalarc->rx*d_r ;
    }
    else
    if (df_dist >= ptrs_ellipticalarc->rx)
    {
        ptrs_ellipticalarc->rx += df__dist_off ;
        ptrs_ellipticalarc->ry = ptrs_ellipticalarc->rx*d_r ;
    }
    else if (ptrs_ellipticalarc->rx > df__dist_off)
    {
        ptrs_ellipticalarc->rx -= df__dist_off ;
        ptrs_ellipticalarc->ry = ptrs_ellipticalarc->rx*d_r ;
    }
    else
    {
        ErrList (73) ;
        b_ret = FALSE ;
    }
    return b_ret ;
}

static BOOL find_offset_arc(void* ptr_ob, double df_x, double df_y, double *df_dist, double bulge)
/*----------------------------------------------------------------------------------------------*/
{
    double df_dist1;
    LUK* ptrs_arc;
    BOOL b_ret;
    double new_r;

    b_ret = TRUE;
    ptrs_arc = (LUK*)ptr_ob;
    df_dist1 = Dist_PP(df_x, df_y, ptrs_arc->x, ptrs_arc->y);

    if (i__offset_mode == POINT_OFFSET_MODE)
    {
        new_r = df_dist1;
    }
    else
        if (df_dist1 >= ptrs_arc->r)
        {
            new_r = ptrs_arc->r + df__dist_off;
        }
        else
            if (ptrs_arc->r > df__dist_off)
            {
                new_r = ptrs_arc->r - df__dist_off;
            }
   
    *df_dist = new_r - ptrs_arc->r;
    if (bulge > 0) *df_dist *= -1;

    return TRUE;
}

/*
static BOOL find_offset_ellipticalarc(void* ptr_ob, double df_x, double df_y, double *df_dist, double bulge)
//----------------------------------------------------------------------------------------------------------
{
    double df_dist1;
    ELLIPTICALARC* ptrs_ellipticalarc;
    BOOL b_ret;
    double new_r;

    b_ret = TRUE;
    ptrs_ellipticalarc = (ELLIPTICALARC*)ptr_ob;
    df_dist1 = Dist_PP(df_x, df_y, ptrs_ellipticalarc->x, ptrs_ellipticalarc->y);

    if (i__offset_mode == POINT_OFFSET_MODE)
    {
        new_r = df_dist1;
    }
    else
    if (df_dist1 >= ptrs_ellipticalarc->rx)
    {
        new_r = ptrs_ellipticalarc->rx + df__dist_off;
    }
    else
    if (ptrs_ellipticalarc->rx > df__dist_off)
    {
        new_r = ptrs_ellipticalarc->rx - df__dist_off;
    }

    *df_dist = new_r - ptrs_ellipticalarc->rx;
    if (bulge > 0) *df_dist *= -1;

    return TRUE;
}
*/

static BOOL translate_offset_spline(void *ptr_ob, double df_x, double df_y, int *shadows_no)
/*----------------------------------------------------------------------------------------*/
{
	double df_dist;
	SPLINE * ptrs_spline;
	BOOL b_ret;
    //int shadows_no;
    long_long l_offb, l_offe;

	b_ret = TRUE;
	ptrs_spline = (SPLINE *)ptr_ob;


	b_ret = FALSE;
	
	df_dist = Dist_PP(df_x, df_y, ptrs_spline->xy[0], ptrs_spline->xy[ptrs_spline->lp]-2);

    l_offb = (char *)ptr_ob-dane;
    l_offe = dane_size;

    blokzap(dane, dane+dane_size, Ablok, COPY_PUT, 1);


    *shadows_no = make_spline_shadows(l_offb, l_offe, Ablok, TRUE, Anormalny, 0, 0);  //1
    if (*shadows_no > 0)
    {
        l_offb = 0;
        l_offe = dane_size;
        b_ret = TRUE;
    }

	return b_ret;
}


static BOOL translate_offset_circle (void *ptr_ob, double df_x, double df_y)
/*------------------------------------------------------------------------*/
{
  double df_dist ;
  OKRAG * ptrs_circle ;
  BOOL b_ret ;

  b_ret = TRUE ;
  ptrs_circle = (OKRAG*)ptr_ob ;
  df_dist = Dist_PP (df_x, df_y, ptrs_circle->x, ptrs_circle->y) ;
  if (i__offset_mode == POINT_OFFSET_MODE)
  {
    ptrs_circle->r = df_dist ;
  }
  else
  if (df_dist >= ptrs_circle->r)
  {
    ptrs_circle->r += df__dist_off ;
  }
  else
  if (ptrs_circle->r > df__dist_off)
  {
    ptrs_circle->r -= df__dist_off ;
  }
  else
  {
     ErrList (74) ;
     b_ret = FALSE ;
  }
  return b_ret ;
}

static BOOL translate_offset_ellipse (void *ptr_ob, double df_x, double df_y)
/*------------------------------------------------------------------------*/
{
    double df_dist, df_dist_el ;
    ELLIPSE * ptrs_ellipse ;
    BOOL b_ret ;
    double rx, ry;
    double d_r;
    double kos, koc, kos1, koc1;
    double df_x1, df_y1;
    double angle;
    double vx1, vy1, vx2, vy2;
    double dot,det;
    double a,b,ratio,eccentricity;
    BOOL inverted=FALSE;
    float ptrs_ellipse_ry;
    float ptrs_ellipse_angle;

    b_ret = TRUE ;
    ptrs_ellipse = (ELLIPSE*)ptr_ob ;

    if (ptrs_ellipse->rx<ptrs_ellipse->ry)
    {
        ptrs_ellipse_ry=ptrs_ellipse->ry;
        ptrs_ellipse->ry=ptrs_ellipse->rx;
        ptrs_ellipse->rx=ptrs_ellipse_ry;
        ptrs_ellipse_angle=ptrs_ellipse->angle;
        ptrs_ellipse->angle+=Pi/2.0;
        inverted=TRUE;
    }

    ratio=ptrs_ellipse->ry/ptrs_ellipse->rx;

    eccentricity = sqrt((ptrs_ellipse->rx * ptrs_ellipse->rx) - (ptrs_ellipse->ry * ptrs_ellipse->ry)) / ptrs_ellipse->rx;

    kos=sin(-ptrs_ellipse->angle);
    koc=cos(-ptrs_ellipse->angle);
    Rotate_Point(kos, koc, ptrs_ellipse->x, ptrs_ellipse->y, df_x, df_y, &df_x1, &df_y1);

    vx1 = df_x1 - ptrs_ellipse->x;
    vy1 = df_y1 - ptrs_ellipse->y;
    vx2 = 1.0;
    vy2 = 0.0;
    dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
    det = vx1 * vy2 - vy1 * vx2;  // Determinant
    angle = -atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
    angle = fmod(angle, Pi2);
    if (angle < 0) angle += Pi2;
    kos1=sin(angle);
    koc1=cos(angle);

    if (Check_if_Equal(angle, 0.0))
    {
        a=df_x1-ptrs_ellipse->x;
        b=a*ratio;
    }
    else if (Check_if_Equal(angle, Pi/2.0))
    {
        b=df_y1-ptrs_ellipse->y;
        a=b/ratio;
    }
    else if (Check_if_Equal(angle, Pi))
    {
        a=df_x1-ptrs_ellipse->x;
        b=a*ratio;
    }
    else if (Check_if_Equal(angle, Pi*3.0/2.0))
    {
        b=df_y1-ptrs_ellipse->y;
        a=b/ratio;
    }
    else {
            a = sqrt((vx1 * vx1) + ((vy1 * vy1) / (1 - (eccentricity * eccentricity))));
            b = sqrt((1 - (eccentricity * eccentricity)) * a * a);
    }


    df_dist = Dist_PP (df_x, df_y, ptrs_ellipse->x, ptrs_ellipse->y) ;  //distance of the point frm the center

    rx=ptrs_ellipse->rx;
    ry=ptrs_ellipse->ry;

    if (i__offset_mode == POINT_OFFSET_MODE)
    {
        if (inverted) {
            ptrs_ellipse->rx = b;
            ptrs_ellipse->ry = a;
            ptrs_ellipse->angle=ptrs_ellipse_angle;
        }
        else
        {
            ptrs_ellipse->rx = a;
            ptrs_ellipse->ry = b;
        }
    }
    else {
        //checking if point is inside or outside ellipse
        //if (inverted) df_dist_el = (vx1*vx1)/(ptrs_ellipse->ry*ptrs_ellipse->ry) + (vy1*vy1)/(ptrs_ellipse->rx*ptrs_ellipse->rx);
        df_dist_el = (vx1*vx1)/(ptrs_ellipse->rx*ptrs_ellipse->rx) + (vy1*vy1)/(ptrs_ellipse->ry*ptrs_ellipse->ry);

        if (df_dist_el > 1.0)  //outside
        {
            ptrs_ellipse->rx += df__dist_off;
            ptrs_ellipse->ry += df__dist_off;
        }
        else //inside
        {
            if ((ptrs_ellipse->rx > df__dist_off) && (ptrs_ellipse->ry > df__dist_off))
            {
                ptrs_ellipse->rx -= df__dist_off;
                ptrs_ellipse->ry -= df__dist_off;
            } else {
                ErrList(215);
                b_ret = FALSE;
            }
        }
    }
    return b_ret ;
}


static BOOL translate_offset_object (void *ptr_ob, double df_x, double df_y)
/*-------------------------------------------------------------------------*/
{
  BOOL b_ret ;

  b_ret = TRUE ;
  switch (((NAGLOWEK *)ptr_ob)->obiekt)
  {
    case Olinia :
       b_ret = translate_offset_line (ptr_ob, df_x, df_y) ;
       break ;
    case Oluk :
       b_ret = translate_offset_arc (ptr_ob, df_x, df_y) ;
       break ;
    case Oellipticalarc :
       b_ret = translate_offset_ellipticalarc (ptr_ob, df_x, df_y) ;
       break ;
	//case Ospline:
	//	b_ret = translate_offset_spline(ptr_ob, df_x, df_y);
	//	break;
    case Ookrag :
    case Okolo :
       b_ret = translate_offset_circle (ptr_ob, df_x, df_y) ;
       break ;
    case Oellipse :
    case Ofilledellipse :
       b_ret = translate_offset_ellipse (ptr_ob, df_x, df_y) ;
      break ;
    default :
       break ;
  }
  return b_ret ;
}


static BOOL add_block(void)
/*------------------------*/
{
#ifndef LINUX
    BLOKD s_blockd = BDdef;
#else
    BLOK s_blockd = BDdef;
#endif
    BLOK* ptrs_block;
    char c_pltype;

    s_blockd.n = B3 + sizeof(c_pltype);
    s_blockd.kod_obiektu = B_PLINE;
    s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype);
    if ((ptrs_block = (BLOK*)dodaj_obiekt(NULL, &s_blockd)) == NULL)
    {
        return FALSE;
    }
    c_pltype = PL_PLINE;
    memmove(&(ptrs_block->opis_obiektu[0]), &c_pltype, sizeof(c_pltype));

    return TRUE;
}

void remove_element(VERTEX* array, int index, int array_length)
{
    int i;
    for (i = index; i < array_length - 1; i++) array[i] = array[i + 1];
}

static BOOL copy_offset_object(double df_x, double df_y)
/*------------------------------------------------------*/
{
    NAGLOWEK* ptrs_off_ob;
    NAGLOWEK* nag, *nags;
    BLOK* ptrs_b;
    NAGLOWEK* ptrs, * ptrs1, * last_ptrs;
    NAGLOWEK *sel_ptrs=NULL;
    int sel_ptrs_no = -1;
    int vertex_n, n;
    VERTEX* vertex;
    BOOL closed;
    double distance;
    double sta, ea, sta1, ea1, ang2, angle, angle1, ea2, sta2;
    double x1, y1, x2, y2, x11, y11, x22, y22, x111, y111, x222, y222, x1111, y1111, x2222, y2222;
    int i;
    unsigned char warstwa, kolor, typ;
    double length2, length_2, dlength2;
    double min_length2 = 1e10;
    BOOL is_set = FALSE;
    BOOL extra_arc=FALSE;
    BOOL arcs_reversed;
    char* adp1;
    int capacity=0;
    int sizes[255];
    int closes[255];
    int shadows_no=0;

    

    CUR_OFF(X, Y);

    if (((NAGLOWEK*)ADP)->obiekt == Ospline)
    {
        if (TRUE == translate_offset_spline(ADP, df_x, df_y, &shadows_no))
        {
            ADP = dane; ADK = dane + (((NAGLOWEK*)dane)->n) - 1;
        }
    }

    else if ((((NAGLOWEK*)ADP)->obiekt == OdBLOK) && (((BLOK*)ADP)->kod_obiektu == B_PLINE))
    {
        ptrs_b = (BLOK*)ADP;
        nags = ADP + sizeof(NAGLOWEK) + B3 + ptrs_b->dlugosc_opisu_obiektu;
        if (nags->obiekt == Ospline)
        {
            //polilyne of splines;
            if (TRUE == translate_offset_spline(ADP, df_x, df_y, &shadows_no))
            {
                ADP = dane; ADK = dane + (((NAGLOWEK*)dane)->n) - 1;
            }
        }
    }

    if ((((NAGLOWEK*)ADP)->obiekt == Olinia) || (((NAGLOWEK*)ADP)->obiekt == Oluk) || (((NAGLOWEK*)ADP)->obiekt == Ookrag) || (((NAGLOWEK*)ADP)->obiekt == Okolo)
          || (((NAGLOWEK*)ADP)->obiekt == Oellipse) || (((NAGLOWEK*)ADP)->obiekt == Ofilledellipse) || (((NAGLOWEK*)ADP)->obiekt == Oellipticalarc))
    {
        if (NULL == (ptrs_off_ob = (NAGLOWEK*)Copy_Object(ADP)))
        {
            CUR_ON(X, Y);
            return TRUE; 	/*!!!!!!*/
        }
        ptrs_off_ob->obiektt2 = O2NoBlockS;
        ptrs_off_ob->blok = NoElemBlok;
        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
        zmien_atrybut(ADP, ADK, Ablok, Anormalny);
        ADP = (char*)ptrs_off_ob;
        ADK = ADP + ptrs_off_ob->n - 1;

            if (TRUE == translate_offset_object((void*)ptrs_off_ob, df_x, df_y))
            {
                rysuj_obiekt((char*)ptrs_off_ob, COPY_PUT, 1);
            }
            else
            {
                Usun_Object((void*)ptrs_off_ob, TRUE);
                ADP = ADK = NULL;
                return FALSE;
            }

        blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
        CUR_ON(X, Y);
        return TRUE;
    }
    else if ((((NAGLOWEK*)ADP)->obiekt == OdBLOK) && (((BLOK*)ADP)->kod_obiektu == B_PLINE))
    {

        if (((BLOK*)ADP)->opis_obiektu[0] != PL_TRACE)
        {

            obiekt_tok((char*)ADP, ADK, (char**)&ptrs, ONieOkreslony);

            vertex_n = 0;
            while (ptrs != NULL)
            {
                if ((ptrs->obiekt != OdBLOK) && (TRUE == Check_Attribute(ptrs->atrybut, Ablok)))
                {
                    vertex_n++;
                    last_ptrs = ptrs;
                }
                obiekt_tok(NULL, ADK, (char**)&ptrs, ONieOkreslony);
            }

            if (vertex_n > 1)
            {

                vertex = malloc((2 * (vertex_n + 1)) * sizeof(VERTEX));

                n = 0;
                obiekt_tok((char*)ADP, ADK, (char**)&ptrs, ONieOkreslony);
                while (ptrs != NULL)
                {
                    extra_arc = FALSE;
                    if (TRUE == Check_Attribute(ptrs->atrybut, Ablok))
                    {
                        if (ptrs->obiekt == Olinia)
                        {
                            x1 = ((LINIA*)ptrs)->x1;
                            y1 = ((LINIA*)ptrs)->y1;
                            x2 = ((LINIA*)ptrs)->x2;
                            y2 = ((LINIA*)ptrs)->y2;

                            length2 = ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
                            length_2 = ((df_x - x1) * (df_x - x1) + (df_y - y1) * (df_y - y1)) + ((x2 - df_x) * (x2 - df_x) + (y2 - df_y) * (y2 - df_y));
                            dlength2 = length_2 - length2;
                            if (dlength2 < min_length2)
                            {
                                min_length2 = dlength2;
                                sel_ptrs = ptrs;
                                sel_ptrs_no = n;
                            }

                            vertex[n].x = x1;
                            vertex[n].y = y1;
                            vertex[n].bulge = 0.0;

                            if (!is_set)
                            {
                                warstwa = ((LINIA*)ptrs)->warstwa;
                                kolor = ((LINIA*)ptrs)->kolor;
                                typ = ((LINIA*)ptrs)->typ;
                                is_set = TRUE;
                            }

                            n++;
                        }
                        else if (ptrs->obiekt == Oluk)
                        {
                            sta = Angle_Normal(((LUK*)ptrs)->kat1);
                            if (sta > (2 * PI)) sta -= (2 * PI);
                            ea = Angle_Normal(((LUK*)ptrs)->kat2);
                            if (ea > (2 * PI)) ea -= (2 * PI);
                            if (ea < sta) ea += (2 * PI);
                            angle = ea - sta;
                            if (angle > PI * 0.85)  //division
                            {
                                extra_arc = TRUE;

                                //checking which arc is first in chain
                                x11 = (((LUK*)ptrs)->x) + (((LUK*)ptrs)->r) * cos(sta);
                                y11 = (((LUK*)ptrs)->y) + (((LUK*)ptrs)->r) * sin(sta);

                                x22 = (((LUK*)ptrs)->x) + (((LUK*)ptrs)->r) * cos(ea);
                                y22 = (((LUK*)ptrs)->y) + (((LUK*)ptrs)->r) * sin(ea);

                                if (n == 0)  //first element - mathing with next one
                                {
                                    store_dane0();
                                    obiekt_tok(NULL, ADK, (char**)&ptrs1, ONieOkreslony);
                                    restore_dane0();
                                    if (ptrs1 != NULL)
                                    {
                                        if (ptrs1->obiekt == Olinia)
                                        {
                                            x111 = ((LINIA*)ptrs1)->x1;
                                            y111 = ((LINIA*)ptrs1)->y1;
                                            x222 = ((LINIA*)ptrs1)->x2;
                                            y222 = ((LINIA*)ptrs1)->y2;
                                        }
                                        else if (ptrs1->obiekt == Oluk)
                                        {
                                            sta1 = Angle_Normal(((LUK*)ptrs1)->kat1);
                                            if (sta1 > (2 * PI)) sta1 -= (2 * PI);
                                            ea1 = Angle_Normal(((LUK*)ptrs1)->kat2);
                                            if (ea1 > (2 * PI)) ea1 -= (2 * PI);

                                            if (ea1 < sta1) ea1 += (2 * PI);
                                            angle1 = ea1 - sta1;

                                            x111 = (((LUK*)ptrs1)->x) + (((LUK*)ptrs1)->r) * cos(sta1);
                                            y111 = (((LUK*)ptrs1)->y) + (((LUK*)ptrs1)->r) * sin(sta1);

                                            x222 = (((LUK*)ptrs1)->x) + (((LUK*)ptrs1)->r) * cos(ea1);
                                            y222 = (((LUK*)ptrs1)->y) + (((LUK*)ptrs1)->r) * sin(ea1);
                                        }
                                        if (((Check_if_Equal(x11, x111)) && (Check_if_Equal(y11, y111))) ||
                                            ((Check_if_Equal(x11, x222)) && (Check_if_Equal(y11, y222))))
                                            arcs_reversed = TRUE;
                                        else arcs_reversed = FALSE;
                                    }
                                    else arcs_reversed = FALSE;
                                }
                                else //matching with previous one
                                {
                                    if ((Check_if_Equal(x11, x2)) && (Check_if_Equal(y11, y2)))
                                        arcs_reversed = FALSE;
                                    else arcs_reversed = TRUE;
                                }

                                if (!arcs_reversed)
                                {
                                    sta2 = sta + angle / 2.0;
                                    ea2 = ea;
                                    ea = sta2;
                                    angle = ea - sta;  //or simply angle/2.0
                                    angle1 = ea2 - sta2;
                                }
                                else
                                {
                                    sta2 = sta;
                                    ea2 = sta + angle / 2.0;
                                    sta = ea2;
                                    angle = ea - sta;
                                    angle1 = ea2 - sta2;
                                }

                                x11 = (((LUK*)ptrs)->x) + (((LUK*)ptrs)->r) * cos(sta);
                                y11 = (((LUK*)ptrs)->y) + (((LUK*)ptrs)->r) * sin(sta);

                                x22 = (((LUK*)ptrs)->x) + (((LUK*)ptrs)->r) * cos(ea);
                                y22 = (((LUK*)ptrs)->y) + (((LUK*)ptrs)->r) * sin(ea);

                                x1111 = (((LUK*)ptrs)->x) + (((LUK*)ptrs)->r) * cos(sta2);
                                y1111 = (((LUK*)ptrs)->y) + (((LUK*)ptrs)->r) * sin(sta2);

                                x2222 = (((LUK*)ptrs)->x) + (((LUK*)ptrs)->r) * cos(ea2);
                                y2222 = (((LUK*)ptrs)->y) + (((LUK*)ptrs)->r) * sin(ea2);

                            }
                            else
                            {
                                x11 = (((LUK*)ptrs)->x) + (((LUK*)ptrs)->r) * cos(sta);
                                y11 = (((LUK*)ptrs)->y) + (((LUK*)ptrs)->r) * sin(sta);

                                x22 = (((LUK*)ptrs)->x) + (((LUK*)ptrs)->r) * cos(ea);
                                y22 = (((LUK*)ptrs)->y) + (((LUK*)ptrs)->r) * sin(ea);
                            }

                            if (!is_set)
                            {
                                warstwa = ((LUK*)ptrs)->warstwa;
                                kolor = ((LUK*)ptrs)->kolor;
                                typ = ((LUK*)ptrs)->typ;
                                is_set = TRUE;
                            }

                            length2 = ((x22 - x11) * (x22 - x11) + (y22 - y11) * (y22 - y11));
                            length_2 = ((df_x - x11) * (df_x - x11) + (df_y - y11) * (df_y - y11)) + ((x22 - df_x) * (x22 - df_x) + (y22 - df_y) * (y22 - df_y));
                            dlength2 = length_2 - length2;
                            if (dlength2 < min_length2)
                            {
                                min_length2 = dlength2;
                                sel_ptrs = ptrs;
                                sel_ptrs_no = n;
                            }

                            if (n > 0)//checking with previous element
                            {
                                if ((Check_if_Equal(x11, x2)) && (Check_if_Equal(y11, y2)))  //
                                {
                                    vertex[n].x = x11;
                                    vertex[n].y = y11;
                                    vertex[n].bulge = tan(angle / 4.0);
                                    x2 = x22;
                                    y2 = y22;
                                    ang2 = ea;

                                    if (extra_arc == TRUE)
                                    {
                                        n++;
                                        vertex[n].x = x1111;
                                        vertex[n].y = y1111;
                                        vertex[n].bulge = tan(angle1 / 4.0);
                                        x2 = x2222;
                                        y2 = y2222;
                                        ang2 = ea;

                                    }

                                }
                                else if ((Check_if_Equal(x22, x2)) && (Check_if_Equal(y22, y2)))
                                {
                                    vertex[n].x = x22;
                                    vertex[n].y = y22;
                                    vertex[n].bulge = tan(-angle / 4.0);
                                    x2 = x11;
                                    y2 = y11;
                                    ang2 = sta;

                                    if (extra_arc == TRUE)
                                    {
                                        n++;
                                        vertex[n].x = x2222;
                                        vertex[n].y = y2222;
                                        vertex[n].bulge = tan(-angle1 / 4.0);
                                        x2 = x1111;
                                        y2 = y1111;
                                        ang2 = ea;

                                    }
                                }
                            }
                            else
                            {
                                store_dane0();
                                obiekt_tok(NULL, ADK, (char**)&ptrs1, ONieOkreslony);
                                restore_dane0();
                                if (ptrs1 != NULL)
                                {
                                    if (ptrs1->obiekt == Olinia)
                                    {
                                        x1 = ((LINIA*)ptrs1)->x1;
                                        y1 = ((LINIA*)ptrs1)->y1;
                                        if ((Check_if_Equal(x22, x1)) && (Check_if_Equal(y22, y1)))
                                        {
                                            vertex[n].x = x11;
                                            vertex[n].y = y11;
                                            vertex[n].bulge = tan(angle / 4.0);
                                            x2 = x22;
                                            y2 = y22;
                                            ang2 = ea;
                                        }
                                        else
                                        {
                                            vertex[n].x = x22;
                                            vertex[n].y = y22;
                                            vertex[n].bulge = tan(-angle / 4.0);
                                            x2 = x11;
                                            y2 = y11;
                                            ang2 = sta;
                                        }
                                    }
                                    else if (ptrs1->obiekt == Oluk)
                                    {
                                        sta1 = Angle_Normal(((LUK*)ptrs1)->kat1);
                                        if (sta1 > (2 * PI)) sta1 -= (2 * PI);
                                        ea1 = Angle_Normal(((LUK*)ptrs1)->kat2);
                                        if (ea1 > (2 * PI)) ea1 -= (2 * PI);

                                        if (ea1 < sta1) ea1 += (2 * PI);
                                        angle1 = ea1 - sta1;

                                        x111 = (((LUK*)ptrs1)->x) + (((LUK*)ptrs1)->r) * cos(sta1);
                                        y111 = (((LUK*)ptrs1)->y) + (((LUK*)ptrs1)->r) * sin(sta1);

                                        x222 = (((LUK*)ptrs1)->x) + (((LUK*)ptrs1)->r) * cos(ea1);
                                        y222 = (((LUK*)ptrs1)->y) + (((LUK*)ptrs1)->r) * sin(ea1);

                                        if (((Check_if_Equal(x22, x111)) && (Check_if_Equal(y22, y111))) ||
                                            ((Check_if_Equal(x22, x222)) && (Check_if_Equal(y22, y222))))
                                        {
                                            vertex[n].x = x11;
                                            vertex[n].y = y11;
                                            vertex[n].bulge = tan(angle / 4);
                                            x2 = x22;
                                            y2 = y22;
                                            ang2 = ea;
                                        }
                                        else
                                        {
                                            vertex[n].x = x22;
                                            vertex[n].y = y22;
                                            vertex[n].bulge = tan(-angle / 4);
                                            x2 = x11;
                                            y2 = y11;
                                            ang2 = sta;
                                        }
                                    }
                                }
                                else //it is the only alament so doesn't matter
                                {
                                    vertex[n].x = x11;
                                    vertex[n].y = y11;
                                    vertex[n].bulge = tan(angle / 4);
                                    x2 = x22;
                                    y2 = y22;
                                    ang2 = ea;
                                }
                            }

                            n++;
                        }
                    }
                    obiekt_tok(NULL, ADK, (char**)&ptrs, ONieOkreslony);
                }
                if (last_ptrs->obiekt == Olinia)
                {
                    vertex[n].x = x2;
                    vertex[n].y = y2;
                    vertex[n].bulge = 0.0;

                }
                else if (last_ptrs->obiekt == Oluk)
                {
                    vertex[n].x = x2;
                    vertex[n].y = y2;
                    vertex[n].bulge = tan(ang2 / 4);

                }

                if (Check_if_Close_Pline((BLOK*)ADP))
                {
                    closed = TRUE;
                    vertex_n = n;
                }
                else
                {
                    closed = FALSE;
                    vertex_n = n + 1;
                }

                //distance = -2.0;  //for test purpose
                if (sel_ptrs != NULL)
                {
                    if (sel_ptrs->obiekt == Olinia)
                    {
                        if (!find_offset_line(sel_ptrs, df_x, df_y, &distance))
                        {
                            free(vertex);
                        }
                    }
                    else if (sel_ptrs->obiekt == Oluk)
                    {
                        if (!find_offset_arc(sel_ptrs, df_x, df_y, &distance, vertex[sel_ptrs_no].bulge))
                        {
                            free(vertex);
                        }
                    }
                    else
                    {
                        free(vertex);
                        CUR_ON(X, Y);
                        return TRUE;
                    }
                }

                get_polyline_offset_data(vertex_n, vertex, &capacity, &sizes, &closes, closed, distance, offset_smooth);

                if (shadows_no == 0) blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
                zmien_atrybut(ADP, ADK, Ablok, Anormalny);

                int m = 0;

                for (int j = 0; j < capacity; j++)
                {
                    if (sizes[j] > 0)
                    {
                        vertex_n = m + sizes[j];
                        closed = closes[j];
                        LINIA L = Ldef;
                        LUK l = ldef;

                        L.warstwa = warstwa;
                        L.kolor = kolor;
                        L.typ = typ;
                        L.obiektt2 = O2BlockPline;
                        L.blok = 1;

                        l.warstwa = warstwa;
                        l.kolor = kolor;
                        l.typ = typ;
                        l.obiektt2 = O2BlockPline;
                        l.blok = 1;

                         //adding pline blok
                        if (!add_block())
                        {
                            free(vertex);
                            if (shadows_no > 0) delete_spline_shadows(FALSE);
                            CUR_ON(X, Y);
                            return TRUE;
                        }

                        //adding pline segments
                        for (i = m; i < vertex_n - 1; i++)
                        {
                            //evaluation of 2 following vertices 
                            if (i < (vertex_n - 2))
                            {
                                if ((fabs(vertex[i + 1].x - vertex[i].x) < VERTEX_D_MIN) && (fabs(vertex[i + 1].y - vertex[i].y) < VERTEX_D_MIN))
                                    continue;
                            }

                            if (fabs(vertex[i].bulge) < VERTEX_B_MIN) vertex[i].bulge = 0;


                            if (vertex[i].bulge == 0)
                            {
                                L.x1 = vertex[i].x;
                                L.y1 = vertex[i].y;
                                L.x2 = vertex[i + 1].x;
                                L.y2 = vertex[i + 1].y;

                                if (NULL == dodaj_obiekt(dane, &L))
                                {
                                    free(vertex);
                                    if (shadows_no > 0) delete_spline_shadows(FALSE);
                                    CUR_ON(X, Y);
                                    return TRUE;
                                }
                                rysuj_obiekt((char*)&L, COPY_PUT, 1);
                            }
                            else
                            {
                                POLYARC(vertex[i].x, vertex[i].y, vertex[i + 1].x, vertex[i + 1].y, vertex[i].bulge, &l);

                                if (l.r > 0.00001)
                                {
                                    if (NULL == dodaj_obiekt(dane, &l))
                                    {
                                        free(vertex);

                                        ptrs_off_ob = dane;
                                        ADP = (char*)ptrs_off_ob;
                                        ADK = ADP + ptrs_off_ob->n - 1;
                                        zmien_atrybut(ADP, ADK, Anormalny, Ablok);
                                        blokzap(ADP, ADK, Ablok, XOR_PUT, 1);

                                        if (shadows_no > 0) delete_spline_shadows(FALSE);
                                        CUR_ON(X, Y);
                                        return TRUE;
                                    }
                                    rysuj_obiekt((char*)&l, COPY_PUT, 1);
                                }
                            }
                        }
                        if (closed == TRUE)
                        {
                            if (vertex[vertex_n - 1].bulge == 0)
                            {
                                L.x1 = vertex[vertex_n - 1].x;
                                L.y1 = vertex[vertex_n - 1].y;
                                L.x2 = vertex[m].x;
                                L.y2 = vertex[m].y;

                                if (NULL == dodaj_obiekt(dane, &L))
                                {
                                    free(vertex);
                                    ptrs_off_ob = dane;
                                    ADP = (char*)ptrs_off_ob;
                                    ADK = ADP + ptrs_off_ob->n - 1;
                                    zmien_atrybut(ADP, ADK, Anormalny, Ablok);
                                    blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
                                    if (shadows_no > 0) delete_spline_shadows(FALSE);
                                    CUR_ON(X, Y);
                                    return TRUE;
                                }
                                rysuj_obiekt((char*)&L, COPY_PUT, 1);
                            }
                            else
                            {
                                POLYARC(vertex[vertex_n - 1].x, vertex[vertex_n - 1].y, vertex[m].x, vertex[m].y, vertex[vertex_n - 1].bulge, &l);

                                if (l.r > 0.00001)
                                {
                                    if (NULL == dodaj_obiekt(dane, &l))
                                    {
                                        free(vertex);
                                        ptrs_off_ob = dane;
                                        ADP = (char*)ptrs_off_ob;
                                        ADK = ADP + ptrs_off_ob->n - 1;
                                        zmien_atrybut(ADP, ADK, Anormalny, Ablok);
                                        blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
                                        if (shadows_no > 0) delete_spline_shadows(FALSE);
                                        CUR_ON(X, Y);
                                        return TRUE;
                                    }
                                    rysuj_obiekt((char*)&l, COPY_PUT, 1);
                                }
                            }

                        }
                        m += sizes[j];
                    }
                }

                free(vertex);
                ptrs_off_ob = dane;
                ADP = (char*)ptrs_off_ob;
                ADK = ADP + ptrs_off_ob->n - 1;
                if (shadows_no > 0) delete_spline_shadows(FALSE);
                zmien_atrybut(ADP, ADK, Anormalny, Ablok);
                blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
                CUR_ON(X, Y);
                return TRUE; 	/*!!!!!!*/

            }
            else if (vertex_n > 0)
            {
                if (NULL == (ptrs_off_ob = (NAGLOWEK*)Copy_Object(last_ptrs)))
                {
                    CUR_ON(X, Y);
                    return TRUE; 	/*!!!!!!*/
                }
                ptrs_off_ob->obiektt2 = O2NoBlockS;
                ptrs_off_ob->blok = NoElemBlok;
                blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
                zmien_atrybut(ADP, ADK, Ablok, Anormalny);
                ADP = (char*)ptrs_off_ob;
                ADK = ADP + ptrs_off_ob->n - 1;

                if (TRUE == translate_offset_object((void*)ptrs_off_ob, df_x, df_y))
                {
                    rysuj_obiekt((char*)ptrs_off_ob, COPY_PUT, 1);
                }
                else
                {
                    Usun_Object((void*)ptrs_off_ob, TRUE);
                    ADP = ADK = NULL;
                    if (shadows_no > 0) delete_spline_shadows(FALSE);
                    return FALSE;
                }
                blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
                CUR_ON(X, Y);
                return TRUE;
            }

            blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
            zmien_atrybut(ADP, ADK, Ablok, Anormalny);
            CUR_ON(X, Y);
            return TRUE; 	/*!!!!!!*/
        }
    }
    else
    {
        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
        zmien_atrybut(ADP, ADK, Ablok, Anormalny);
        CUR_ON(X, Y);
        return TRUE; 	/*!!!!!!*/
    }

    
}


static BOOL set_offset_object (void)
/*--------------------------------*/
{
  unsigned typ;
  void* ptr1, * ptr;
  BLOK *ptrs_b;
  long_long ptr1l=0;

  ptr1 = &ptr1l;

  CUR_OFF (X, Y) ;
  typ = Blinia | Bluk | Bokrag | Bkolo | Bspline | Bellipse | Bellipticalarc; // | Bwwielokat;
  ptr = select_w (&typ, &ptr1) ;
  if (ptr == NULL)
  {
    CUR_ON (X, Y) ;
    return FALSE ;
  }
  ADP = ptr ;

  if (((NAGLOWEK *)ptr)->blok == ElemBlok)
  {
    ptrs_b = LASTB ((char  *)ptr) ;
    if (ptrs_b == NULL)
    {
      Internal_Error (__LINE__,__FILE__);
    }
    if (ptrs_b != NULL || ptrs_b->obiektt1 == OB1NOCHANGE || ptrs_b->obiektt1 == OB1CHANGE_DRAG)
    {
      if ((ptrs_b->kod_obiektu==B_PLINE) && (ptrs_b->opis_obiektu[0]!=PL_TRACE))
          ADP = (char *)ptrs_b ;
    }
  }

  ADK = ADP + ((NAGLOWEK *)ADP)->n - 1 ;
  zmien_atrybut (ADP, ADK, Anormalny, Ablok) ;
  blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
  CUR_ON (X, Y) ;
  return TRUE ;
}


static int edit_dist_value (BOOL b_graph_value)
/*-----------------------------*/
{
  double df_edit_val ;

  b_graph_value = b_graph_value ;
  if (eO.val_no < 1)
  {
    return 0 ;
  }
  df_edit_val = eO.values [0] ;
  if (df_edit_val > 0)
  {
    df__dist_off = jednostkiOb (df_edit_val) ;
    return 1 ;
  }
  else
  {
    ErrList (30) ;
    return 0 ;
  }
}


static void set_offset_mode (int ev_number)
/*----------------------------------------*/
{
  switch (ev_number)
  {
    case 0 :
       i__offset_mode = POINT_OFFSET_MODE ;
       komunikat0 (65) ;
       usunstr (np) ;
       break ;
    case 1 :
       i__offset_mode = DIST_OFFSET_MODE ;
       komunikat0 (66) ;
       if (-1 != (np = dodajstr(&eO)) &&
	  (EOF != sprintf (eO.st, "%#12.9lg", milimetryob (df__dist_off))) )
       {
	 Out_Edited_Draw_Param ((ESTR *)&eO, TRUE) ;
       }
       break ;
    default :
       break ;
  }
}

static void set_offset_normal(void)
{
    offset_smooth = FALSE;
    menu_par_new((*mOffset.pola)[3].txt, OFFSET_NORMAL);
}

static void set_offset_smooth(void)
{
    offset_smooth = TRUE;
    menu_par_new((*mOffset.pola)[3].txt, OFFSET_SMOOTH);
}
static void  nooop(void)
{}

static void redcr1 (char typ)
/*--------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int KR0, sel_akt, sel_cur ;
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  static unsigned short cod = L' ';
  static void (*REDdod)(void)=nooop;
  static int iconno=0;

  if (typ == 0)
   {
     KR0 = Komunikat_R0 ;
     komunikat (0) ;
     CUR_OFF (X, Y) ;
     sel_akt = sel.akt ; sel.akt = 1 ;
     sel_cur = sel.cur ; sel.cur = 0 ;
     CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;   CUR_ON=cursel_on;
     CUR_ON (X, Y) ;
     REDdod = REDDOD ; REDDOD = Redraw_Block ;
     getmenupini (&menu, st, &cod, &iconno) ;

     menupini (&mOffset, _OFFSET_, _OFFSET_C_, 49) ;

   }
  else
  {
    CUR_OFF = CUR_oFF ;
    CUR_ON = CUR_oN ;
    REDDOD = REDdod ;
    sel.akt = sel_akt ;
    sel.cur = sel_cur ;

    memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
    zmien_atrybut_undo (dane, dane + dane_size) ;
    blokzap (ADP, ADK, Ablok, COPY_PUT, 1) ;
    zmien_atrybut (ADP, ADK,  Ablok, Aoblok) ;
    CUR_OFF (X, Y) ;
    ADP = ADK = NULL ;
    menupini (menu, st, cod, iconno) ;
    usunstr (np) ;         /*komunikat0 after usunstr*/
    komunikat0 (KR0) ;
    CUR_ON (X, Y) ;
  }
}


static void offset (void)
/*----------------------*/
{
  EVENT *ev ;
  double X0, Y0 ;

  if (!offset_smooth) menu_par_new((*mOffset.pola)[3].txt, OFFSET_NORMAL);
  else  menu_par_new((*mOffset.pola)[3].txt, OFFSET_SMOOTH);
  redcr1 (0) ;
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
	case evKeyDown :
	   if(ev->Number == 0)
	   {
	     redcr1 (1) ;
	     return ;
	   }
	   if(ev->Number == ENTER)
	   {
	     if (FALSE == copy_offset_object (X0, Y0))
	     {
	       redcr1 (1) ;
	       return ;
	     }
         else
         {
             CUR_OFF(X,Y);
             CUR_ON(X,Y);
         }
	   }
	   break ;
	case evCommandP :
	   switch (ev->Number)
	   {
	      case POINT_OFFSET_MODE :
	      case DIST_OFFSET_MODE  :
		 set_offset_mode (ev->Number) ;
		 break ;
		  case SET_DIST_OFFSET:
			  simulate_keypress(TAB);
		 break;
         break;
          case SET_OFFSET_NORMAL:
              set_offset_normal();
         break;
          case SET_OFFSET_SMOOTH:
              set_offset_smooth();
              break;
	      default :
		 break ;
	   }
	   break ;
	default :
	   break ;
    }
  }
}


static void redcr0(char typ)
/*------------------------*/
{
  static int ( *SW[2])(), sel_akt, sel_cur, sel_gor, sel_nr ;

  if (typ == 0)
   {
	  CUR_OFF(X, Y);
     ADP = ADK = NULL ;
     komunikat (0) ;
     komunikat0 (67) ;
     eO.x=maxX-PL266 + 5 ;
     eO.y= ESTR_Y;
     eO.val_no_max = 1;
     eO.lmax=r22 - 1 ;
     eO.mode = GV_DIST ;
     eO.format = "%#12.9lg" ;
     eO.ESTRF = edit_dist_value ;
	 eO.extend = 0;
     sel_akt = sel.akt ; sel.akt = 1 ;
     sel_cur = sel.cur ; sel.cur = 1 ;
     sel_gor = sel.gor; sel.gor = 0;
     sel_nr = sel.nr;

     layer_info();
     
     CUR_oFF0=CUR_OFF;  CUR_OFF = out_sel_off ;
     CUR_oN0=CUR_ON;   CUR_ON = out_sel_on ;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     CUR_ON (X,Y);
	 
   }
  else
  {
    CUR_OFF(X,Y);
    CUR_OFF=CUR_oFF0;
    CUR_ON=CUR_oN0;
    SERV[73]=SW[0];
    SERV[81]=SW[1];
    CUR_ON(X,Y);
    sel.akt = sel_akt ;
    sel.cur = sel_cur ;
    sel.gor = sel_gor;
    sel.nr = sel_nr;

    layer_info();

    komunikat0 (0) ;
  }
}

void Offset (void)
/*---------------*/
{
  EVENT *ev ;
  double X0, Y0 ;

  redcr0 (0) ;
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    X0 = X0 ;
    Y0 = Y0 ;
    switch (ev->What)
    {
	case evKeyDown :
	   if(ev->Number == 0)
	   {
	     redcr0 (1) ;
	     return ;
	   }
	   if(ev->Number == ENTER)
	   {
	     if (TRUE == set_offset_object ())
	     {
             if (i__offset_mode == DIST_OFFSET_MODE)
             {
                 komunikat0(66);
                 if (-1 != (np = dodajstr(&eO)) &&
                     (EOF != sprintf(eO.st, "%#12.9lg", milimetryob(df__dist_off))))
                 {
                     Out_Edited_Draw_Param((ESTR*)&eO, TRUE);
                 }
             }
             else komunikat0(65);
	       offset () ;
           komunikat0(67);
	     }
	   }
	   break ;
	default :
	   break ;
    }
  }
}

#undef __O_OFFSET__