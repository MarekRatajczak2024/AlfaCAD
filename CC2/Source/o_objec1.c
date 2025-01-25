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

#define __O_OBJECT1__
#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#include<math.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_libfun.h"
#include "o_libfun.h"
#include "o_osnap.h"
#include "o_object.h"
#include "b_textw.h"

#include "menu.h"

extern void obiekt_tok_all (char *, char  *, char  **, int) ;
extern void adrem_obiekt1 (void *ad, int atrybut0);
extern void adrem_blok1 (BLOK *adb, int atrybut);
extern BOOL get_pattern_count(void);
extern void set_pattern_count(BOOL p_count);
extern int	global_mode;
extern int Ini_Blocks_Array(void);
extern void Destroy_Blocks_Array(void);
extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle, double df_eangle, double xy[]);
extern void Get_EllipticalArc_EndPoints (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle0, double df_eangle0, double *x1, double *y1, double *x2, double *y2);
extern void set_global_hidden_blocks_visibility(BOOL block_visibility);

static int get_maxy_point 	(void *) ;

int get_block_layer(BLOK *b)
/*------------------------*/
{ char *adp, *adk;
  LINIA *L;
  BLOK *b1;
  int ret;

  ret=-1;
  adp=(char *)b+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
  adk=(char *)b+sizeof(NAGLOWEK)+b->n-1;
  L = (LINIA*)adp;
  while ((L->obiekt != Okoniec) && (adp < adk))
  {
   if (L->obiekt == OdBLOK)
    {
	  b1=(BLOK *)adp;
	  adp+=sizeof(NAGLOWEK)+B3+b1->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
      {
        ret=L->warstwa;
        break;
      }
     adp+=sizeof(NAGLOWEK)+L->n;
    }
   L = (LINIA*)adp;
  }
 return ret;
}

int get_block_obiektt2(BLOK *b)
/*---------------------------*/
{ char *adp, *adk;
  LINIA *L;
  BLOK *b1;
  int ret;

  ret=Current_Layer;
  adp=(char *)b+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
  adk=(char *)b+sizeof(NAGLOWEK)+b->n-1;
  L = (LINIA*)adp;
  while ((L->obiekt != Okoniec) && (adp < adk))
  {
   if (L->obiekt == OdBLOK)
    {
	  b1=(BLOK *)adp;
	  adp+=sizeof(NAGLOWEK)+B3+b1->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->atrybut!=Ausuniety) && (L->atrybut!=Abad))
      {
        ret=L->obiektt2;
        break;
      }
     adp+=sizeof(NAGLOWEK)+L->n;
    }
   L = (LINIA*)adp;
  }
 return ret;
}


BOOL Check_if_Empty_Block (BLOK *ptrs_bl)
/*---------------------------------------*/
{
  BOOL b_ret ;

  b_ret = FALSE ;
  if (ptrs_bl->n == B3 + ptrs_bl->dlugosc_opisu_obiektu)
  {
    b_ret = TRUE ;
  }
  return b_ret ;
}

BOOL End_Spline(double *x, double *y, double x0, double y0, SPLINE *bspline)
{
	double x1, y1, x2, y2;
	BOOL ret;

	x1 = bspline->xy[0];
	y1 = bspline->xy[1];
	x2 = bspline->xy[bspline->lp-2];
	y2 = bspline->xy[bspline->lp-1];
	if ((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0) >= (x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))
	{
		*x = x2;
		*y = y2;
		ret = FALSE;
	}
	else
	{
		*x = x1;
		*y = y1;
		ret = TRUE;
	}
	return ret;
}

BOOL End_Arc (double *x, double *y, double x0, double y0, LUK *l)
/*-----------------------------------------------------------------*/
{
  double x1,y1,x2,y2;
  BOOL ret;

  x1=l->x+l->r*cos(l->kat1);
  y1=l->y+l->r*sin(l->kat1);
  x2=l->x+l->r*cos(l->kat2);
  y2=l->y+l->r*sin(l->kat2);
  if( (x1-x0)*(x1-x0)+(y1-y0)*(y1-y0) >= (x2-x0)*(x2-x0)+(y2-y0)*(y2-y0) )
  {
    *x=x2;
    *y=y2;
    ret = FALSE;
  }
  else
  {
    *x=x1;
    *y=y1;
    ret = TRUE;
  }
  return ret;
}


BOOL End_EllipticalArc (double *x, double *y, double x0, double y0, ELLIPTICALARC *ea)
/*-----------------------------------------------------------------------*/
{
    double x1,y1,x2,y2;
    BOOL ret;

    Get_EllipticalArc_EndPoints (ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, &x1, &y1, &x2, &y2);

    if( (x1-x0)*(x1-x0)+(y1-y0)*(y1-y0) >= (x2-x0)*(x2-x0)+(y2-y0)*(y2-y0) )
    {
        *x=x2;
        *y=y2;
        ret = FALSE;
    }
    else
    {
        *x=x1;
        *y=y1;
        ret = TRUE;
    }
    return ret;
}


BOOL End_Elliptical_Arc(double *x, double *y, double x0, double y0, ELLIPTICALARC *ea)
{

    double x1,y1,x2,y2;
    BOOL ret;
    int numpoints;
    double xy[256];

    numpoints=Get_EllipticalArc_Points(ea->x, ea->y,ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, xy);

    x1=xy[0];
    y1=xy[1];
    x2=xy[numpoints-2];
    y2=xy[numpoints-1];
    if( (x1-x0)*(x1-x0)+(y1-y0)*(y1-y0) >= (x2-x0)*(x2-x0)+(y2-y0)*(y2-y0) )
    {
        *x=x2;
        *y=y2;
        ret = FALSE;
    }
    else
    {
        *x=x1;
        *y=y1;
        ret = TRUE;
    }
    return ret;

}

BOOL End_Elliptical_Arc_XY(double *x, double *y, double x0, double y0, int numpoints, double xy[])
{
    double x1,y1,x2,y2;
    BOOL ret;

    x1=xy[0];
    y1=xy[1];
    x2=xy[numpoints-2];
    y2=xy[numpoints-1];
    if( (x1-x0)*(x1-x0)+(y1-y0)*(y1-y0) >= (x2-x0)*(x2-x0)+(y2-y0)*(y2-y0) )
    {
        *x=x2;
        *y=y2;
        ret = FALSE;
    }
    else
    {
        *x=x1;
        *y=y1;
        ret = TRUE;
    }
    return ret;

}


int Cmp_Arc_Angle (double df_val1,   double df_val2,
		   double df_arc_angle1, double df_arc_angle2)
/*----------------------------------------------------------*/
{
  if (TRUE == Check_if_Equal (df_val1, df_val2))
  {
    return 0 ;
  }
  if (FALSE == Check_Angle (df_val1, df_arc_angle1, df_arc_angle2))
  {
    if (Get_Angle_Difference (df_val1, df_arc_angle1) >
	Get_Angle_Difference (df_val1, df_arc_angle2))
    {
      df_val1 = df_arc_angle2 ;
    }
    else
    {
      df_val1 = df_arc_angle1 ;
    }
  }
  if (FALSE == Check_Angle (df_val2, df_arc_angle1, df_arc_angle2))
  {
    if (Get_Angle_Difference (df_val2, df_arc_angle1) >
	Get_Angle_Difference (df_val2, df_arc_angle2))
    {
      df_val2 = df_arc_angle2 ;
    }
    else
    {
      df_val2 = df_arc_angle1 ;
    }
  }
  if (TRUE == Check_Angle (df_val1, df_val2, df_arc_angle2))
  {
    return 1 ;
  }
  else
  {
   return -1 ;
  }
}


void Get_Text_Border (TEXT *ptrs_text,
		      double *ptrdf_x,
		      double *ptrdf_y,
		      BOOL b_margin)
/*------------------------------------*/
{
  double df_sin, df_cos, df_len, df_height ;
  double df_x0, df_y0 ;

#define LM .1 		/*lewy margines*/
#define RM .1 		/*prawy margines*/
#define DM (3./7.)	/*dolny margines*/
#define TM (1./3.)	/*gorny margines*/

  df_sin = sin (ptrs_text->kat) ;
  df_cos = cos (ptrs_text->kat) ;
  df_len = Get_TextLen (ptrs_text, NULL) ;
  df_height = ptrs_text->wysokosc ;
  ptrdf_x [0] = ptrs_text->x ;
  ptrdf_y [0] = ptrs_text->y ;
  if (b_margin == TRUE)
  {
    df_x0 = ptrdf_x [0] - df_height * LM ;
    df_y0 = ptrdf_y [0] + Ky * ptrs_text->wysokosc * DM ;
    Rotate_Point (df_sin, df_cos,
		  ptrs_text->x, ptrs_text->y,
		  df_x0, df_y0, &df_x0, &df_y0) ;
    ptrdf_x [0] = df_x0 ;
    ptrdf_y [0] = df_y0 ;
    df_len += df_height * (LM + RM) ;
    df_height += df_height * (DM + TM) ;
  }
  ptrdf_x [1] = ptrdf_x [0] + df_len * df_cos ;
  ptrdf_y [1] = ptrdf_y [0] - Ky * df_len * df_sin ;
  ptrdf_x [3] = ptrdf_x [0] - df_height * df_sin ;
  ptrdf_y [3] = ptrdf_y [0] - Ky * df_height * df_cos ;
  ptrdf_x [2] = ptrdf_x [1] + ptrdf_x [3] - ptrdf_x [0] ;
  ptrdf_y [2] = ptrdf_y [1] + ptrdf_y [3] - ptrdf_y [0] ;
}

WIELOKAT *Text_to_Solid (TEXT *ptrs_text, WIELOKAT *ptrs_solid)
/*-----------------------------------------------------------*/
{
  double df_x [4], df_y [4] ;
  int i ;

  Get_Text_Border (ptrs_text, df_x, df_y, TRUE) ;
  ptrs_solid->lp = 8 ;
  for (i = 0 ; i < 4 ; i++)
  {
    ptrs_solid->xy [2 * i + 0] = df_x [i] ;
    ptrs_solid->xy [2 * i + 1] = df_y [i] ;
  }
  return ptrs_solid ;
}




int Get_Rectangle_Type (WIELOKAT *ptr_w)
/*-------------------------------------*/
{
  LINIA L1, L2 ;
  double x, y ;

  L1.x1 = ptr_w->xy [0] ;
  L1.y1 = ptr_w->xy [1] ;
  L1.x2 = ptr_w->xy [2] ;
  L1.y2 = ptr_w->xy [3] ;
  L2.x1 = ptr_w->xy [4] ;
  L2.y1 = ptr_w->xy [5] ;
  L2.x2 = ptr_w->xy [6] ;
  L2.y2 = ptr_w->xy [7] ;
  if (1 == przeciecieLL_ (&x, &y, (void *)&L1, (void *)&L2))
  {
    return InterS1_3 ;
  }
  L1.x1 = ptr_w->xy [4] ;
  L1.y1 = ptr_w->xy [5] ;
  L1.x2 = ptr_w->xy [2] ;
  L1.y2 = ptr_w->xy [3] ;
  L2.x1 = ptr_w->xy [0] ;
  L2.y1 = ptr_w->xy [1] ;
  L2.x2 = ptr_w->xy [6] ;
  L2.y2 = ptr_w->xy [7] ;
  if (1 == przeciecieLL_ (&x, &y, (void *)&L1, (void *)&L2))
  {
    return InterS2_4 ;
  }
  return SIMPW ;
}


BOOL Get_Solid_Intersec (WIELOKAT *ptr_w,
			double *ptrdf_x,
			double *ptrdf_y,
			double *ptrdf_t1,
			double *ptrdf_t2)
/*--------------------------------------*/
{
  BOOL b_ret = FALSE ;
  LINIA L1, L2 ;

  switch (ptr_w->obiektt1)
  {
    case InterS1_3 :
       L1.x1 = ptr_w->xy [0] ;
       L1.y1 = ptr_w->xy [1] ;
       L1.x2 = ptr_w->xy [2] ;
       L1.y2 = ptr_w->xy [3] ;
       L2.x1 = ptr_w->xy [4] ;
       L2.y1 = ptr_w->xy [5] ;
       L2.x2 = ptr_w->xy [6] ;
       L2.y2 = ptr_w->xy [7] ;
       if (1 == przeciecieLL_tt (ptrdf_x, ptrdf_y,
				(void *)&L1, (void *)&L2,
				ptrdf_t1, ptrdf_t2))
       {
	 b_ret = TRUE ;
	 (*ptrdf_t1) += 0 ;
	 (*ptrdf_t2) += 2 ;
       }
       break ;
    case InterS2_4 :
       L1.x1 = ptr_w->xy [2] ;
       L1.y1 = ptr_w->xy [3] ;
       L1.x2 = ptr_w->xy [4] ;
       L1.y2 = ptr_w->xy [5] ;
       L2.x1 = ptr_w->xy [6] ;
       L2.y1 = ptr_w->xy [7] ;
       L2.x2 = ptr_w->xy [0] ;
       L2.y2 = ptr_w->xy [1] ;
       if (1 == przeciecieLL_tt (ptrdf_x, ptrdf_y,
				(void *)&L1, (void *)&L2,
				ptrdf_t1, ptrdf_t2))
       {
	 (*ptrdf_t1) += 1 ;
	 (*ptrdf_t2) += 3 ;
	 b_ret = TRUE ;
       }
       break ;
    case SIMPW :
       break ;
    default :
       break ;
  }
  return b_ret ;
}

BOOL Get_Rec_Division(WIELOKAT* ptr_w, WIELOKAT* ptr_w1, WIELOKAT* ptr_w2)
{
    ptr_w1->lp = 6;
    ptr_w1->xy[0] = ptr_w->xy[0];
    ptr_w1->xy[1] = ptr_w->xy[1];
    ptr_w1->xy[2] = ptr_w->xy[2];
    ptr_w1->xy[3] = ptr_w->xy[3];
    ptr_w1->xy[4] = ptr_w->xy[4];
    ptr_w1->xy[5] = ptr_w->xy[5];

    ptr_w2->lp = 6;
    ptr_w2->xy[0] = ptr_w->xy[4];
    ptr_w2->xy[1] = ptr_w->xy[5];
    ptr_w2->xy[2] = ptr_w->xy[6];
    ptr_w2->xy[3] = ptr_w->xy[7];
    ptr_w2->xy[4] = ptr_w->xy[0];
    ptr_w2->xy[5] = ptr_w->xy[1];

    return TRUE;
}

BOOL Get_Rec_2TrAngle (WIELOKAT *ptr_w, WIELOKAT *ptr_w1, WIELOKAT *ptr_w2)
/*-----------------------------------------------------------------------*/
{
  double x, y ;
  BOOL b_ret = FALSE ;
  LINIA L1, L2 ;

  ptr_w1->lp = 6 ;
  ptr_w2->lp = 6 ;
  switch (ptr_w->obiektt1)
  {
    case InterS1_3 :
       L1.x1 = ptr_w->xy [0] ;
       L1.y1 = ptr_w->xy [1] ;
       L1.x2 = ptr_w->xy [2] ;
       L1.y2 = ptr_w->xy [3] ;
       L2.x1 = ptr_w->xy [4] ;
       L2.y1 = ptr_w->xy [5] ;
       L2.x2 = ptr_w->xy [6] ;
       L2.y2 = ptr_w->xy [7] ;
       if (1 == przeciecieLL_ (&x, &y, (void *)&L1, (void *)&L2))
       {
	 ptr_w1->xy [0] = ptr_w->xy [0] ;
	 ptr_w1->xy [1] = ptr_w->xy [1] ;
	 ptr_w1->xy [2] = ptr_w->xy [6] ;
	 ptr_w1->xy [3] = ptr_w->xy [7] ;
	 ptr_w1->xy [4] = x ;
	 ptr_w1->xy [5] = y ;
	 ptr_w2->xy [0] = ptr_w->xy [2] ;
	 ptr_w2->xy [1] = ptr_w->xy [3] ;
	 ptr_w2->xy [2] = ptr_w->xy [4] ;
	 ptr_w2->xy [3] = ptr_w->xy [5] ;
	 ptr_w2->xy [4] = x ;
	 ptr_w2->xy [5] = y ;
	 b_ret = TRUE ;
       }
       break ;
    case InterS2_4 :
       L1.x1 = ptr_w->xy [4] ;
       L1.y1 = ptr_w->xy [5] ;
       L1.x2 = ptr_w->xy [2] ;
       L1.y2 = ptr_w->xy [3] ;
       L2.x1 = ptr_w->xy [0] ;
       L2.y1 = ptr_w->xy [1] ;
       L2.x2 = ptr_w->xy [6] ;
       L2.y2 = ptr_w->xy [7] ;
       if (1 == przeciecieLL_ (&x, &y, (void *)&L1, (void *)&L2))
       {
	 ptr_w1->xy [0] = ptr_w->xy [0] ;
	 ptr_w1->xy [1] = ptr_w->xy [1] ;
	 ptr_w1->xy [2] = ptr_w->xy [2] ;
	 ptr_w1->xy [3] = ptr_w->xy [3] ;
	 ptr_w1->xy [4] = x ;
	 ptr_w1->xy [5] = y ;
	 ptr_w2->xy [0] = ptr_w->xy [4] ;
	 ptr_w2->xy [1] = ptr_w->xy [5] ;
	 ptr_w2->xy [2] = ptr_w->xy [6] ;
	 ptr_w2->xy [3] = ptr_w->xy [7] ;
	 ptr_w2->xy [4] = x ;
	 ptr_w2->xy [5] = y ;
	 b_ret = TRUE ;
       }
       break ;
    case SIMPW :
       break ;
    default :
       break ;
  }
  return b_ret ;
}


void Normalize_Solid (WIELOKAT *ptr_w)
/*----------------------------------*/
/*
wierzcholki zgodne z ruchem wskazowek zegara,
ustawienie flagi obiektt1 dla czworokata,
*/
{
  int maxyno, x1no, x2no ;

  if (ptr_w->lp == 8 && SIMPW != (ptr_w->obiektt1 = Get_Rectangle_Type (ptr_w)))
  {
    return ;
  }
  maxyno = get_maxy_point (ptr_w) ;
  if (maxyno == 1)
  {
    x1no = ptr_w->lp - 2 ;
    x2no = 2 ;
  }
  else
  if (maxyno == ptr_w->lp - 1)
  {
    x1no = ptr_w->lp - 4 ;
    x2no = 0 ;
  }
  else
  {
    x1no = maxyno - 3 ;
    x2no = maxyno + 1 ;
  }
  if (ptr_w->xy [x1no] > ptr_w->xy [x2no])
  {
    WIELOKAT w ;
    int i ;

    for (i = 0 ; i < ptr_w->lp ; i += 2)
    {
      w.xy [i] = ptr_w->xy [ptr_w->lp - i - 2] ;
      w.xy [i + 1] = ptr_w->xy [ptr_w->lp - i - 1] ;
    }
    memcpy (&ptr_w->xy [0], &w.xy [0], ptr_w->lp * sizeof (ptr_w->xy [0])) ;
  }
}


void Normalize_Spline(SPLINE *ptr_w)
/*----------------------------------*/
/*
wierzcholki zgodne z ruchem wskazowek zegara,
ustawienie flagi obiektt1 dla czworokata,
*/
{
	int maxyno, x1no, x2no;

	if (ptr_w->lp == 8 && SIMPW != (ptr_w->obiektt1 = Get_Rectangle_Type(ptr_w)))
	{
		return;
	}
	maxyno = get_maxy_point(ptr_w);
	if (maxyno == 1)
	{
		x1no = ptr_w->lp - 2;
		x2no = 2;
	}
	else
		if (maxyno == ptr_w->lp - 1)
		{
			x1no = ptr_w->lp - 4;
			x2no = 0;
		}
		else
		{
			x1no = maxyno - 3;
			x2no = maxyno + 1;
		}
	if (ptr_w->xy[x1no] > ptr_w->xy[x2no])
	{
		SPLINE s;
		int i;

		for (i = 0; i < ptr_w->lp; i += 2)
		{
			s.xy[i] = ptr_w->xy[ptr_w->lp - i - 2];
			s.xy[i + 1] = ptr_w->xy[ptr_w->lp - i - 1];
		}
		memcpy(&ptr_w->xy[0], &s.xy[0], ptr_w->lp * sizeof(ptr_w->xy[0]));
	}
}


static int get_maxy_point (void *ptr_w_)
/*-------------------------------------*/
{
  int i, maxyno ;
  WIELOKAT *ptr_w;
  SPLINE *ptr_s;

  maxyno = 1 ;

  if ((NAGLOWEK*)ptr_w_==Owwielokat)
  {
      ptr_w=(WIELOKAT*)ptr_w_;
      for (i = 1; i < ptr_w->lp; i += 2)
      {
          if (ptr_w->xy [maxyno] < ptr_w->xy [i])
          {
              maxyno = i ;
          }
      }
      return maxyno ;
  }
  else if ((NAGLOWEK*)ptr_w_==Ospline)
  {
      ptr_s=(SPLINE*)ptr_w_;
      for (i = 1; i < ptr_s->lp; i += 2)
      {
          if (ptr_s->xy [maxyno] < ptr_s->xy [i])
          {
              maxyno = i ;
          }
      }
      return maxyno ;
  }
  else return 0;
}

static int get_maxy_quad_point (QUAD *quad)
/*-------------------------------------*/
{
    int i, maxyno ;

    maxyno = 1 ;
    for (i = 1; i < 8; i += 2)
    {
        if (quad->xy [maxyno] < quad->xy [i])
        {
            maxyno = i ;
        }
    }
    return maxyno ;
}


void Normalize_Quad (QUAD *quad)
/*----------------------------------*/
/*
wierzcholki zgodne z ruchem wskazowek zegara,
ustawienie flagi obiektt1 dla czworokata,
*/
{
    int maxyno, x1no, x2no ;

    maxyno = get_maxy_quad_point (quad) ;
    if (maxyno == 1)
    {
        x1no = 6 ;
        x2no = 2 ;
    }
    else
    if (maxyno == 7)
    {
        x1no = 4 ;
        x2no = 0 ;
    }
    else
    {
        x1no = maxyno - 3 ;
        x2no = maxyno + 1 ;
    }
    if (quad->xy [x1no] > quad->xy [x2no])
    {
        QUAD q ;
        int i ;

        for (i = 0 ; i < 8 ; i += 2)
        {
            q.xy [i] = quad->xy [8 - i - 2] ;
            q.xy [i + 1] = quad->xy [8 - i - 1] ;
        }
        memcpy (&quad->xy [0], &q.xy [0], 8 * sizeof (quad->xy [0])) ;
    }
}



int Get_Polygon_Num (BLOK *ptrs_b)
/*-------------------------------*/
{
  long n ;

  n = ptrs_b->n - B3 - ptrs_b->dlugosc_opisu_obiektu ;
  n /= sizeof (LINIA) ;
  return (int)n ;
}

BOOL Point_Arc ( double x, double y, LUK *l)
/*------------------------------------------*/
{
  double kat;

  kat = Atan2 (y - l->y, x - l->x);
  return Check_Angle (kat, l->kat1, l->kat2) ;
}

BOOL
Point_Arc_Prec ( double x, double y, LUK *l, int i_precion)
/*--------------------------------------------------------*/
{
  double kat;

  kat = Atan2 (y - l->y, x - l->x);
  return Check_Angle_Prec (kat, l->kat1, l->kat2, i_precion) ;
}


void Arc_To_Points (LUK *l, double *x1, double *y1, double *x2, double *y2)
/*------------------------------------------------------------------------*/
{
  *x1 = l->x + l->r * cos (l->kat1) ;
  *y1 = l->y + l->r * sin (l->kat1) ;
  *x2 = l->x + l->r * cos (l->kat2) ;
  *y2 = l->y + l->r * sin (l->kat2) ;
}

void Vector_Arc_To_Points (AVECTOR *v, double *x1, double *y1, double *x2, double *y2)
/*---------------------------------------------------------------------------------*/
{
    *x1 = v->x1 + v->r * cos (v->angle1) ;
    *y1 = v->y1 + v->r * sin (v->angle1) ;
    *x2 = v->x1 + v->r * cos (v->angle2) ;
    *y2 = v->y1 + v->r * sin (v->angle2) ;
}

void SolidArc_To_Points (SOLIDARC *sa, double *x1, double *y1, double *x2, double *y2)
/*------------------------------------------------------------------------*/
{
    *x1 = sa->x + sa->r * cos (sa->kat1) ;
    *y1 = sa->y + sa->r * sin (sa->kat1) ;
    *x2 = sa->x + sa->r * cos (sa->kat2) ;
    *y2 = sa->y + sa->r * sin (sa->kat2) ;
}

void Points_To_Arc (LUK *l, double x1, double y1, double x2, double y2)
/*---------------------------------------------------------------------*/
{  double x11, y11, x22, y22;
   x11 = x1 - l->x ;
   y11 = y1 - l->y ;
   y22 = y2 - l->y ;
   x22 = x2 - l->x ;
   l->kat1 = Angle_Normal(Atan2 (y11, x11)) ;
   l->kat2 = Angle_Normal(Atan2 (y22 ,x22)) ;
   l->r = sqrt (x11 * x11 + y11 * y11);
}

void Points_To_Arc_Vector (AVECTOR *v, double x1, double y1, double x2, double y2)
/*--------------------------------------------------------------------------*/
{
    x1 = x1 - v->x1 ;
    y1 = y1 - v->y1 ;
    y2 = y2 - v->y1 ;
    x2 = x2 - v->x1 ;
    v->angle1 = Angle_Normal(Atan2 (y1, x1)) ;
    v->angle2 = Angle_Normal(Atan2 (y2 ,x2)) ;
    v->r = sqrt (x1 * x1 + y1 * y1);
}

void Points_To_SolidArc (SOLIDARC *sa, double x1, double y1, double x2, double y2)
/*---------------------------------------------------------------------*/
{
    x1 = x1 - sa->x ;
    y1 = y1 - sa->y ;
    y2 = y2 - sa->y ;
    x2 = x2 - sa->x ;
    sa->kat1 = Angle_Normal(Atan2 (y1, x1)) ;
    sa->kat2 = Angle_Normal(Atan2 (y2 ,x2)) ;
    sa->r = sqrt (x1 * x1 + y1 * y1);
}

void Redraw_Block (void)
/*---------------------*/
{
    ////blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
}


BOOL check_if_hidden (char  *adp,char  *adk,int atrybut)
/*--------------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;

  if (adp == NULL || adk == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return FALSE;
  }
  b_retval = FALSE;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if (TRUE == Check_Attribute (L->atrybut, atrybut) &&
          Layers [L->warstwa].on == FALSE)
      {
	b_retval = TRUE;
	break;
      }
      adp+=L->n+sizeof(NAGLOWEK);
    }
    L = (LINIA*)adp;
  }
  return b_retval;
}



BOOL check_if_edit (char huge *adp,char  *adk,int atrybut)
/*--------------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;

  if (adp == NULL || adk == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return FALSE;
  }
  b_retval = FALSE;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if (TRUE == Check_Attribute (L->atrybut, atrybut) &&
          Layers [L->warstwa].edit == FALSE)
      {
	b_retval = TRUE;
	break;
      }
      adp+=L->n+sizeof(NAGLOWEK);
    }
    L = (LINIA*)adp;
  }
  return b_retval;
}

BOOL get_extend (char  **adp_l,char  **adk_l,int atrybut)
/*--------------------------------------------------------------*/
{
  BOOL b_retval;
  NAGLOWEK *nag;
  BLOK  *b;
  char  *adp,  *adk;

  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  nag = (NAGLOWEK*)adp;
  while (nag->obiekt != Okoniec && adp < adk)
  {
    if (nag->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    { 
      if (nag->atrybut == atrybut)
      {
	 if (*adp_l > adp)
	 {
	   *adp_l = adp ;
	 }
	 if (*adk_l < adp)
	 {
	   *adk_l = adk;
	 }
      }
      adp += nag->n + sizeof(NAGLOWEK) ;
    }
    nag = (NAGLOWEK*)adp;
  }

  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}

BOOL get_extend1 (char  **adp_l,char  **adk_l,int atrybut)
/*--------------------------------------------------------------*/
{
  BOOL b_retval;
  NAGLOWEK *nag;
  BLOK  *b;
  char  *adp,  *adk;

  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  nag = (NAGLOWEK*)adp;
  while (nag->obiekt != Okoniec && adp < adk)
  {
  /*  if (nag->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {   */
      if (nag->atrybut == atrybut)
      {
	 if (*adp_l > adp)
	 {
	   *adp_l = adp ;
	 }
	 if (*adk_l < adp)
	 {
	   *adk_l = adk;
	 }
      }
     if (nag->obiekt == OdBLOK)
      {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
      }
     else
      { 
        adp += nag->n + sizeof(NAGLOWEK) ;
      }  
 /*   } */
    nag = (NAGLOWEK*)adp;
  }

  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}

BOOL get_all (char  **adp_l,char  **adk_l)
/*--------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;
  int blok_no;

  InfoList(2);
  
  b_retval = TRUE;
  *adp_l = dane ;
  *adk_l = dane + dane_size ;
  adp = dane ;
  adk = dane + dane_size ;

  blok_no=Ini_Blocks_Array();

  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
      if (L->atrybut!=Ablok)
      {
       b=(BLOK *)adp;
       adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
      }
      else
       {
         adp += L->n + sizeof(NAGLOWEK) ;
       } 
    }
    else
    {
      
      if ((L->atrybut != Abad) && (L->atrybut != Ablok) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
        L->atrybut = Ablok; /*zamiana na ostatni blok */
        if (L->blok==ElemBlok) 
         {
           adrem_obiekt1(adp, Ablok);
         }  
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }


  ClearInfo ();
    
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }

  Destroy_Blocks_Array();

  return b_retval;
}


BOOL get_all_from_layer (char  **adp_l,char  **adk_l, int nr_warstwy)
/*-----------------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;
  BOOL znaleziono;
  int blok_no;

  InfoList(2);

  znaleziono=FALSE;
  
  b_retval = TRUE;

  *adp_l = NULL ;
  *adk_l = dane + dane_size ;

  adp = dane ;
  adk = dane + dane_size ;

  blok_no=Ini_Blocks_Array();

  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
      if (L->atrybut!=Ablok)
      {
       b=(BLOK *)adp;
       adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
       if (b->n == (B3 + b->dlugosc_opisu_obiektu))
        {
         *adp_l=dane;
         b->atrybut = Ablok;
        }
      }
      else
       {
         adp += L->n + sizeof(NAGLOWEK) ;
       } 
    }
    else
    {
      if ((L->atrybut != Abad) && (L->atrybut != Ablok) && (L->atrybut != Ausuniety)
           && (L->warstwa == nr_warstwy))
      {
        *adp_l=dane;
        znaleziono=TRUE;
        L->atrybut = Ablok; /*zamiana na ostatni blok */
        if (L->blok==ElemBlok) 
         {
           adrem_obiekt1(adp, Ablok);
         }  
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
  ClearInfo ();

  if (znaleziono==FALSE)
   {
    *adp_l = NULL;
    *adk_l = NULL;
   }

  Destroy_Blocks_Array();

  return b_retval;
}


BOOL get_special_hatch25_from_layer (char  **adp_l,char  **adk_l, int nr_warstwy, int opcja)
/*----------------------------------------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;
  int typ5;
  int blok_no;

  InfoList(2);
  
  b_retval = TRUE;

  *adp_l = NULL ;
  *adk_l = dane + dane_size ;

  adp = dane ;
  adk = dane + dane_size ;

  blok_no=Ini_Blocks_Array();

  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
      if (L->atrybut!=Ablok)
      {
       b=(BLOK *)adp;
       adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
       if (b->n == (B3 + b->dlugosc_opisu_obiektu))
        {
         *adp_l=dane;
         b->atrybut = Ablok;
        }
      }
      else
       {
         adp += L->n + sizeof(NAGLOWEK) ;
       } 
    }
    else
    {
      typ5=0;
      if (opcja & 4)
       {
         if ((L->obiekt==Olinia) || (L->obiekt==Oluk) || (L->obiekt==Ookrag))
          {
            if ((L->typ==5) || (L->typ==37) || (L->typ==69) || (L->typ==101) || (L->typ==133))
              typ5=1;
          }
       }
      if ((L->atrybut != Abad) && (L->atrybut != Ablok) && (L->atrybut != Ausuniety)
           && (((L->obiektt2==O2BlockSpecial) && (opcja & 1)) || ((L->obiektt2==O2BlockHatch25) && (opcja & 2))
              || ((L->obiektt2==O2BlockAparat) && (opcja & 8)) || (typ5==1))
           && (L->warstwa == nr_warstwy))
      {
        *adp_l=dane;
        L->atrybut = Ablok; /*zamiana na ostatni blok */
        if (L->blok==ElemBlok) 
         {
           adrem_obiekt1(adp, Ablok);
         }  
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
  ClearInfo ();

  if (*adp_l == NULL)
   {
    *adp_l = NULL;
    *adk_l = NULL;
   }

    Destroy_Blocks_Array();

  return b_retval;
}


BOOL get_all_from_next_layers (char  **adp_l,char  **adk_l, int nr_pierwszej_warstwy)
/*---------------------------------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;
  int blok_no;

  InfoList(2);
  
  b_retval = TRUE;

  *adp_l = NULL ;
  *adk_l = dane + dane_size ;

  adp = dane ;
  adk = dane + dane_size ;

  blok_no=Ini_Blocks_Array();

  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
      if (L->atrybut!=Ablok)
      {
       b=(BLOK *)adp;
       adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
       if (b->n == (B3 + b->dlugosc_opisu_obiektu))
        {
         *adp_l=dane;
         b->atrybut = Ablok;
        }
      }
      else
       {
         adp += L->n + sizeof(NAGLOWEK) ;
       } 
    }
    else
    {
      if ((L->atrybut != Abad) && (L->atrybut != Ablok) && (L->atrybut != Ausuniety)
           && (L->warstwa >= nr_pierwszej_warstwy))
      {
        *adp_l=dane;
        L->atrybut = Ablok; /*zamiana na ostatni blok */
        if (L->blok==ElemBlok) 
         {
           adrem_obiekt1(adp, Ablok);
         }  
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
  ClearInfo ();
    

  if (*adp_l == NULL)
   {
    *adp_l = NULL;
    *adk_l = NULL;
   }
    Destroy_Blocks_Array();

  return b_retval;
}

BOOL get_all_ep_space (char *adp0, char *adk0, char  **adp_l,char  **adk_l)
/*-----------------------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;
  int blok_no;

  InfoList(2);
  
  b_retval = TRUE;
  *adp_l = NULL ;
  *adk_l = NULL ;
  if (adp0!=NULL) adp=adp0;
  else adp = dane ;
  if (adk0!=NULL) adk=adk0;
  else adk = dane + dane_size ;

  blok_no=Ini_Blocks_Array();

  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if ((L->atrybut != Abad) && (L->atrybut != Ausuniety) &&
        (L->przec==1))
      {
        if (*adp_l == NULL) *adp_l=adp;
        *adk_l=adp+L->n-1;
        L->atrybut = Ablok; /*zamiana na ostatni blok */
        if (L->blok==ElemBlok)
         {
           adrem_obiekt1(adp, Ablok);
         }  
      }
       if (L->obiekt == OdBLOK)
        {
          b=(BLOK *)adp;
          adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
        }
        else
         {
           adp += L->n + sizeof(NAGLOWEK) ;
         }
    L = (LINIA*)adp;
  }


  ClearInfo ();

  Destroy_Blocks_Array();

  return b_retval;
}


BOOL get_all_special (char  **adp_l,char  **adk_l)
/*----------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;

  InfoList(2);
  
  b_retval = TRUE;

  *adp_l = dane ;
  *adk_l = dane + dane_size ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
      if (L->atrybut!=Ablok)
      {
       b=(BLOK *)adp;
       adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
      }
      else
       {
         adp += L->n + sizeof(NAGLOWEK) ;
       } 
    }
    else
    {
      if ((L->obiekt != Otekst) && (L->atrybut != Abad) && (L->atrybut != Ablok) && (L->atrybut != Ausuniety) &&
          (L->obiektt2==O2BlockSpecial) &&
          (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
        L->atrybut = Ablok; /*zamiana na ostatni blok */

      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }


  ClearInfo ();
    
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}

BOOL get_lines_aparat (char  **adp_l,char  **adk_l)
/*-----------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;

  
  b_retval = TRUE;
  adp = *adp_l ;
  adk = *adk_l ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
      if (L->atrybut!=Ablok)
      {
       b=(BLOK *)adp;
       adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
      }
      else
       {
         adp += L->n + sizeof(NAGLOWEK) ;
       } 
    }
    else
    {
      if ((L->obiekt == Olinia) && (L->atrybut != Abad) && (L->atrybut != Ablok) && (L->atrybut != Ausuniety) &&
          (L->obiektt2==O2BlockAparat) &&
          (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
        L->atrybut = Ablok; /*zamiana na ostatni blok */

      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  return b_retval;
}

BOOL get_all_texts (char  **adp_l,char  **adk_l)
/*--------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;

  InfoList(2);
  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt==Otekst) && (L->atrybut != Abad) /*&& (L->atrybut != Ablok)*/ && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
         L->atrybut = Ablok;
        
	 if (*adp_l > adp)
	 {
	   *adp_l = adp ;
	 }
	 if (*adk_l < adp)
	 {
	   *adk_l = adk;
	 }
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  ClearInfo ();
  
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}


int count_texts (char  *adp_l, char  *adk_l)
/*----------------------------------------*/
{
  int  l_texts;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;

  l_texts = 0;
  adp = adp_l ;
  adk = adk_l ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt==Otekst) && (L->atrybut != Abad) /*&& (L->atrybut != Ablok)*/ && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
         l_texts++;
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  return l_texts;
}



BOOL get_texts_hatch (char  **adp_l,char  **adk_l)
/*-------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;

  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt==Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety)
        && ((L->obiektt2==O2BlockHatch25) || (L->obiektt2==O2BlockHatch50) || (L->obiektt2==O2BlockAparat))
        && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
         L->atrybut = Ablok;
        
	 if (*adp_l > adp)
	 {
	   *adp_l = adp ;
	 }
	 if (*adk_l < adp)
	 {
	   *adk_l = adk;
	 }
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }
  
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}



BOOL get_texts_hatch_cur_layer (char  **adp_l,char  **adk_l)
/*--------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;

  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt==Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety)
        && ((L->obiektt2==O2BlockHatch25) || (L->obiektt2==O2BlockHatch50) || (L->obiektt2==O2BlockAparat))
        && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      {
       if (L->warstwa==Current_Layer)
        {
         L->atrybut = Ablok;
        
   	   if (*adp_l > adp)
	       {
	         *adp_l = adp ;
	       }
	      if (*adk_l < adp)
	       {
	         *adk_l = adk;
	       }
         }
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}


BOOL get_hatch_lines (char  **adp_l,char  **adk_l)
/*----------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;

  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
  	  b=(BLOK *)adp;
	  adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt==Olinia) && (L->atrybut != Abad) && (L->atrybut != Ausuniety)
        && ((L->obiektt2==O2BlockHatch25) || (L->obiektt2==O2BlockHatch50) || (L->obiektt2==O2BlockAparat))
        && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
         L->atrybut = Ablok;
        
	 if (*adp_l > adp)
	 {
	   *adp_l = adp ;
	 }
	 if (*adk_l < adp)
	 {
	   *adk_l = adk;
	 }
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}

BOOL get_special_lines_and_texts (char  **adp_l,char  **adk_l)
/*----------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  char  *adp,  *adk, *adp1, *adk1;
  T_Desc_Ex_Block *ptrs_desc_bl ;
  int b_warstwa;


  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
     b=(BLOK *)adp;
	 //tutaj jest usuwany blok ktory zawiera obiekty typu O2BlockSpecial
	 //ryzykom polega na tym, ze nie odroznia sie bloku wewnetrznego, np. stanowiacego opis spadku i dlugosci
	 //od bloku zewnetrznego, jezeli np. kopiuje sie caly profil (jako nowy blok) i nie rozbija bloku nadrzednego
	 //w takim przypadku usuwany jest caly blok profilu.
	 //aby temu zapobiec mozemy przyjac, ze blok musi tez byc oznaczony jako 02BlockSpecial
	 if ((b->obiektt2==O2BlockSpecial) && (b->atrybut != Abad) && (b->atrybut != Ausuniety))
      {
        //za wyjatkiem atrybutu
        if (b->dlugosc_opisu_obiektu > 1)
         {
          ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
          if (ptrs_desc_bl->sz_type [0] != '\0')
           {
            if (strcmp(ptrs_desc_bl->sz_type,"$ATRYBUT")==0)  //prawidlowa nazwa bloku
             {
               adp+=sizeof(NAGLOWEK)+b->n;
             }
              else
               {

                 b_warstwa=get_block_layer(b);
                 if (b_warstwa==Current_Layer)
                  {
                   if ((Layers[b_warstwa].edit==1) &&
                       (Layers[b_warstwa].on==1))
                        {
                         b->atrybut = Ablok;
                         adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                         adk1=adp+sizeof(NAGLOWEK)+b->n - 1;
                         zmien_atrybut(adp1,adk1,ANieOkreslony,Ablok);
                         adp+=sizeof(NAGLOWEK)+b->n;
                        }
                       else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                  }
                   else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;

               }
           }
           else
            {

               b_warstwa=get_block_layer(b);
                 if (b_warstwa==Current_Layer)
                  {
                   if ((Layers[b_warstwa].edit==1) &&
                       (Layers[b_warstwa].on==1))
                        {
                         b->atrybut = Ablok;
                         adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                         adk1=adp+sizeof(NAGLOWEK)+b->n - 1;
                         zmien_atrybut(adp1,adk1,ANieOkreslony,Ablok);
                         adp+=sizeof(NAGLOWEK)+b->n;
                        }
                         else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                  }
                   else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;

            }
         }
          else
           {

              b_warstwa=get_block_layer(b);
              if (b_warstwa==Current_Layer)
               {
                 if ((Layers[b_warstwa].edit==1) &&
                     (Layers[b_warstwa].on==1))
                      {
                       b->atrybut = Ablok;
                       adp1=adp+sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
                       adk1=adp+sizeof(NAGLOWEK)+b->n - 1;
                       zmien_atrybut(adp1,adk1,ANieOkreslony,Ablok);
                       adp+=sizeof(NAGLOWEK)+b->n;
                      }
                      else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
               }
                else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;

           }
      }
       else
        {
         //za wyjatkiem atrybutu
         if (b->dlugosc_opisu_obiektu > 1)
         {
          ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
          if (ptrs_desc_bl->sz_type [0] != '\0')
          {
            if (strcmp(ptrs_desc_bl->sz_type,"$ATRYBUT")==0)  //prawidlowa nazwa bloku
            {
               adp+=sizeof(NAGLOWEK)+b->n;
			}
			 else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
		  }
		    else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
		 }
		    else adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
		}
    }
    else
    {
      if (((L->obiekt==Olinia) || (L->obiekt==Otekst))
        && (L->atrybut != Abad)
        && (L->atrybut != Ausuniety)
        && (L->obiektt2==O2BlockSpecial)
        && (L->warstwa==Current_Layer)
        && (Layers[L->warstwa].on==TRUE)
        && (Layers[L->warstwa].edit==TRUE))
      { 
         L->atrybut = Ablok;
        
	     if (*adp_l > adp)
      	{
	       *adp_l = adp ;
	      }
	     if (*adk_l < adp)
	      {
	       *adk_l = adk;
	      }
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}


BOOL get_sectors_blocks(char  **adp_l, char  **adk_l)
/*----------------------------------------------------------*/
{
	BOOL b_retval;
	LINIA *L, *L1;
	BLOK  *b;
	char  *adp, *adk;

	b_retval = TRUE;
	*adp_l = dane + dane_size;
	*adk_l = dane;
	adp = dane;
	adk = dane + dane_size;
	L = (LINIA*)adp;
	while (L->obiekt != Okoniec && adp < adk)
	{
		if (L->obiekt == OdBLOK)
		{
			b = (BLOK *)adp;
			if (b->kod_obiektu == B_SEKTORY)
			{
				b->atrybut = Ablok;
				if (*adp_l > adp)
				{
					*adp_l = adp;
				}
				if (*adk_l < adp)
				{
					*adk_l = adp;
				}
				adp += sizeof(NAGLOWEK) + b->n;
			}
			else
			{
				b = (BLOK *)adp;
				adp += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
			}
		}
		else
		{
			adp += L->n + sizeof(NAGLOWEK);
		}
		L = (LINIA*)adp;
	}

	if (*adp_l == dane + dane_size)
	{
		*adp_l = NULL;
		*adk_l = NULL;
	}
	return b_retval;
}



BOOL get_all_texts_symbols (char  **adp_l,char  **adk_l) 
/*---------------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  TEXT  *T;
  char  *adp,  *adk;

  InfoList(2);
  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt==Otekst) && (L->atrybut != Abad) /*&& (L->atrybut != Ablok)*/ && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
       T = (TEXT*)adp;
       if (T->typ == n_typ_symbol)
        {
         L->atrybut = Ablok;
        
	 if (*adp_l > adp)
	 {
	   *adp_l = adp ;
	 }
	 if (*adk_l < adp)
	 {
	   *adk_l = adk;
	 }
	} 
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  ClearInfo ();
  
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}

BOOL get_all_texts_n_typ (char  **adp_l,char  **adk_l, char n_typ)
/*---------------------------------------------------------------*/
{
  BOOL b_retval;
  LINIA *L;
  BLOK  *b;
  TEXT  *T;
  char  *adp,  *adk;

  InfoList(2);
  b_retval = TRUE;
  *adp_l = dane + dane_size ;
  *adk_l = dane ;
  adp = dane ;
  adk = dane + dane_size ;
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->obiekt==Otekst) && (L->atrybut != Abad) /*&& (L->atrybut != Ablok)*/ && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE) && (Layers[L->warstwa].edit==TRUE))
      { 
       T = (TEXT*)adp;
       if (T->typ == n_typ)
        {
         L->atrybut = Ablok;
        
	 if (*adp_l > adp)
	 {
	   *adp_l = adp ;
	 }
	 if (*adk_l < adp)
	 {
	   *adk_l = adk;
	 }
	} 
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  ClearInfo ();
  
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }
  return b_retval;
}


BOOL get_all_layers (char  **adp_l,char  **adk_l)
/*--------------------------------------------------------------*/
{ /*pobranie wszystkich warstw poza 255 zachowywana jako warstwa blokow*/
  BOOL b_retval;
  BLOK  *b;
  LINIA *L;
  char  *adp,  *adk;
  int blok_no;

  InfoList(2);
  b_retval = TRUE;
  *adp_l = dane ;
  *adk_l = dane + dane_size ;
  adp = dane ;
  adk = dane + dane_size ;

  blok_no=Ini_Blocks_Array();

  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
      if (L->atrybut!=Ablok)
       {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
   if (b->n == (B3 + b->dlugosc_opisu_obiektu))
        {
         *adp_l=dane;
         b->atrybut = Ablok;
        }
       }
       else
        {
         adp += L->n + sizeof(NAGLOWEK) ;
        }	
    }
    else
    {
      
      if ((L->atrybut != Abad) && (L->atrybut != Ausuniety) && (L->atrybut != Ablok)
         && (L->warstwa<255))
      { 
       if (L->atrybut!=Ablok)
        {
          L->atrybut = Ablok; /*zamiana na ostatni blok */
          if (L->blok==ElemBlok) adrem_obiekt1(adp, Ablok);
        } 

      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  ClearInfo ();
  
  if (*adp_l == dane + dane_size)
  {
    *adp_l = NULL;
    *adk_l = NULL;
  }

  Destroy_Blocks_Array();

  return b_retval;
}


BOOL check_if_obiekt (char  *adp, char  *adk, int atrybut, int obiekt)
/*---------------------------------------------------------------------------*/
{
  BOOL b_retval;
  NAGLOWEK *nag;

  b_retval = FALSE;
  if (adp == NULL || adk == NULL)
  {
    return b_retval;
  }
  obiekt_tok_all ((char*)adp, adk, (char **)&nag, obiekt);
  while (nag != NULL)
  {
    if (nag->atrybut == atrybut)
    {
      b_retval = TRUE ;
      break ;
    }
    obiekt_tok_all (NULL, adk, (char **)&nag, obiekt);
  }
  return b_retval ;
}


void Usun_bloki_DXF_old (void)
/*----------------------*/
{ /*usuniecie (fizyczne) wszystkich elementow z warstwy 255*/
  BOOL b_retval;
  BLOK  *b;
  LINIA *L;
  char  *adp,  *adk;
  int key;
  int blok_no;

  b_retval = TRUE;
  adp = dane ;
  adk = dane + dane_size ;

  /*upewnienie*/  
  komunikat (109);

  key = Get_Legal_Key (_YES_NO_ESC_);

  komunikat (0);
  if (key == ESC) return;
  if (key == _NO_ || key == _no_) return;
  
  InfoList(2);

  blok_no=Ini_Blocks_Array();
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp < adk)
  {
    if (L->obiekt == OdBLOK)
    {
      if (L->atrybut!=Ausuniety)
       {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
       }
        else	
         {
          adp += L->n + sizeof(NAGLOWEK) ;
         }
    }
    else
    {
      if ((L->atrybut != Abad) && (L->atrybut != Ausuniety) && (L->obiektt2=O2BlockDXF) && (L->warstwa==255))
      { 
        L->atrybut = Ausuniety;
        if (L->blok==ElemBlok)
         {
           adrem_obiekt1(adp, Ausuniety);
         }  
      }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (LINIA*)adp;
  }

  Destroy_Blocks_Array();

  /*fizyczne usuniecie*/
  Cur_offd (X, Y) ;
  usun_blok (dane, dane + dane_size) ;
  Cur_ond (X, Y) ;

  ClearInfo();


  return;
}


void Usun_bloki_DXF_factory(char *adr, char *adrk)
{
	NAGLOWEK *nag, *nag1;
	char *adrb, *adrbk;
	char *adr0;


	BOOL b_retval;
	BLOK  *b;
	LINIA *L;
	int key;

	b_retval = TRUE;
	adr = dane;
	adrk = dane + dane_size;

	/*upewnienie*/
	komunikat(109);

	key = Get_Legal_Key(_YES_NO_ESC_);

	komunikat(0);
	if (key == ESC) return;
	if (key == _NO_ || key == _no_) return;

	InfoList(2);

	adr0 = adr;

	nag = (NAGLOWEK*)adr;
	while ((nag->obiekt != Okoniec) && (adr < adrk))
	{
		switch (nag->obiekt)
		{
		case OdBLOK:
			if (nag->obiektt2 == O2BlockDXF)
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
						adrb += sizeof(NAGLOWEK) + B3 + ((BLOK *)nag)->dlugosc_opisu_obiektu;
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
	/*fizyczne usuniecie*/
	Cur_offd(X, Y);
	usun_blok(adr0, adrk);
	Cur_ond(X, Y);

	ClearInfo();
}

void Usun_bloki_DXF(void)
{   char  *adr, *adrk;
    adr = dane;
    adrk = dane + dane_size;
    Usun_bloki_DXF_factory(adr, adrk);
}

void shift_layer_numbers(int layer_no)
{
    BOOL b_retval;
    BLOK  *b;
    LINIA *L;
    char  *adp,  *adk;
    int key;
    int blok_no;

    b_retval = TRUE;
    adp = dane ;
    adk = dane + dane_size ;

    L = (LINIA*)adp;
    while (L->obiekt != Okoniec && adp < adk)
    {
        if (L->obiekt == OdBLOK)
        {
            b=(BLOK *)adp;
            adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
        }
        else
        {
            if (L->warstwa == layer_no)
            {
                L->atrybut = Abad;  //something went wrong with cleaning layer
            }
            else if (L->warstwa > layer_no)
            {
                L->warstwa -= 1;
            }
            adp += L->n + sizeof(NAGLOWEK) ;
        }
        L = (LINIA*)adp;
    }

    return;
}

void delete_all_from_layer_atrybut (int layer_no, int atrybut)
/*----------------------------------------------------------*/
{ /*deleting (permanently) all elements from selected layer*/
    BOOL b_retval;
    BLOK  *b;
    LINIA *L;
    char  *adp,  *adk;
    int key;
    int blok_no;

    b_retval = TRUE;
    adp = dane ;
    adk = dane + dane_size ;

    blok_no=Ini_Blocks_Array();

    set_global_hidden_blocks_visibility(TRUE);

    L = (LINIA*)adp;
    while (L->obiekt != Okoniec && adp < adk)
    {
        if (L->obiekt == OdBLOK)
        {
            if (L->atrybut!=Ausuniety)
            {
                b=(BLOK *)adp;
                adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
            }
            else
            {
                adp += L->n + sizeof(NAGLOWEK) ;
            }
        }
        else
        {
            if ((L->atrybut != Abad) && (L->atrybut != Ausuniety) && (L->warstwa==layer_no) && ((L->atrybut == atrybut) || (atrybut==ANieOkreslony)))
            {
                L->atrybut = Ausuniety;
                if (L->blok==ElemBlok)
                {
                    adrem_obiekt1(adp, Ausuniety);
                }
            }
            adp += L->n + sizeof(NAGLOWEK) ;
        }
        L = (LINIA*)adp;
    }

    Destroy_Blocks_Array();

    /*fizyczne usuniecie*/
    usun_blok (dane, dane + dane_size) ;

    set_global_hidden_blocks_visibility(FALSE);

    return;
}

/*
void delete_quiet_all_from_layer_atrybut (int layer_no, int atrybut)
//------------------------------------------------------------------
{ //deleting (permanently) all elements from selected layer
    BOOL b_retval;
    BLOK  *b;
    LINIA *L;
    char  *adp,  *adk;
    int key;
    int blok_no;

    b_retval = TRUE;
    adp = dane ;
    adk = dane + dane_size ;

    blok_no=Ini_Blocks_Array();

    set_global_hidden_blocks_visibility(TRUE);

    L = (LINIA*)adp;
    while (L->obiekt != Okoniec && adp < adk)
    {
        if (L->obiekt == OdBLOK)
        {
            if (L->atrybut!=Ausuniety)
            {
                b=(BLOK *)adp;
                adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
            }
            else
            {
                adp += L->n + sizeof(NAGLOWEK) ;
            }
        }
        else
        {
            if ((L->atrybut != Abad) && (L->atrybut != Ausuniety) && (L->warstwa==layer_no) && ((L->atrybut == atrybut) || (atrybut==ANieOkreslony)))
            {
                L->atrybut = Ausuniety;
                if (L->blok==ElemBlok)
                {
                    adrem_obiekt1(adp, Ausuniety);
                }
            }
            adp += L->n + sizeof(NAGLOWEK) ;
        }
        L = (LINIA*)adp;
    }

    Destroy_Blocks_Array();

    //fizyczne usuniecie
    usun_blok (dane, dane + dane_size) ;

    set_global_hidden_blocks_visibility(FALSE);

    return;
}
*/

void delete_all_pattern_blocks_from_layer (int layer_no, char *pattern)
/*-------------------------------------------------------------------*/
{ /*deleting (permanently) all blocks following the name pattern from selected layer*/
    BOOL b_retval;
    BLOK  *b;
    char  *adp,  *adk;
    int key;
    int blok_no;
    NAGLOWEK *nag;
    T_Desc_Ex_Block *ptrs_desc_bl ;

    b_retval = TRUE;
    adp = dane ;
    adk = dane + dane_size ;

    //blok_no=Ini_Blocks_Array();

    nag = (NAGLOWEK*)adp;
    while (nag->obiekt != Okoniec && adp < adk)
    {
        if (nag->obiekt == OdBLOK)
        {
            if (nag->atrybut!=Ausuniety)
            {
                b=(BLOK *)adp;

                if ((b->kod_obiektu == B_EXPORT) && (b->dlugosc_opisu_obiektu > 17))
                {
                    ptrs_desc_bl = (T_Desc_Ex_Block *) (&b->opis_obiektu[0]);
                    if (strstr(ptrs_desc_bl->sz_type, pattern) != NULL)
                    {
                        adrem_blok1((BLOK*)adp, Ausuniety);
                        ((BLOK*)adp)->atrybut=Ausuniety;
                        adp += nag->n + sizeof(NAGLOWEK) ;
                        continue;
                    }

                }
                adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
            }
            else
            {
                adp += nag->n + sizeof(NAGLOWEK) ;
            }
        }
        else
        {
            adp += nag->n + sizeof(NAGLOWEK) ;
        }
    }

    //Destroy_Blocks_Array();

    /*fizyczne usuniecie*/
    usun_blok (dane, dane + dane_size) ;

    return;
}

#undef __O_OBJECT1__