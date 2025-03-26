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
#include <math.h>
#include <stdlib.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_osnap.h"
#include "o_object.h"


extern int solidarc_elements(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner1, LUK *l_inner2, LUK *l_outer1, LUK *l_outer2);

#define Npts 90

static double LINE_ARC_ALIGN=0.006; //0.005, 0.001this parameter should be available to set in options

static BOOL check_line_t (double df_t,
                         LINIA  *ptrs_line)
//------------------------------------------
{
  double df_x, df_y ;

  if (df_t >= 0 && df_t <= 1)
  {
    return TRUE ;
  }
  df_x = ptrs_line->x1 + df_t * (ptrs_line->x2 - ptrs_line->x1) ;
  df_y = ptrs_line->y1 + df_t * (ptrs_line->y2 - ptrs_line->y1) ;
  if ((TRUE == Check_if_Equal (df_x, ptrs_line->x1) &&
      TRUE == Check_if_Equal (df_y, ptrs_line->y1))   ||
      (TRUE == Check_if_Equal (df_x, ptrs_line->x2) &&
      TRUE == Check_if_Equal (df_y, ptrs_line->y2)))
  {
    return TRUE ;
  }
  return FALSE ;
}

#ifdef aaaaaa
static BOOL check_line_t (double df_t)
/*-----------------------------------*/
{
  BOOL b_ret = FALSE ;
  double df_prec ;

  df_prec = Get_Prec (MPREC_2, 1, 1) * 5 ;
  if (df_t >= 0 - df_prec && df_t <= 1 + df_prec)
  {
    b_ret = TRUE ;
  }
  return b_ret ;
}
#endif


int IntersectionLL (LINIA  *ptrs_line1,
		    LINIA  *ptrs_line2,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_t1,
		    double *ptrdf_t2)
/*-----------------------------------*/
{
  int i_ret ;

  i_ret = 0 ;
  if (TRUE == przeciecieLL_tt (ptrdf_x, ptrdf_y, ptrs_line1, ptrs_line2, ptrdf_t1, ptrdf_t2) &&
      TRUE == check_line_t (*ptrdf_t1, ptrs_line1) &&
      TRUE == check_line_t (*ptrdf_t2, ptrs_line2))
  {
    i_ret = 1 ;
  }
  return i_ret ;
}

int IntersectionLO (LINIA  *ptrs_line,
		    OKRAG  *ptrs_circle,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_tl,
		    double *ptrdf_ac)
/*-----------------------------------*/
{
  int i_ret, i_interno, i ;
  double df_x [2], df_y [2], df_t [2] ;

  i_ret = 0 ;
  i_interno = przeciecieLO_tt (
		    &df_x [0], &df_y [0], &df_t [0],
		    &df_x [1], &df_y [1], &df_t [1],
		    ptrs_line, ptrs_circle) ;
  for (i = 0 ; i < i_interno ; i++)
  {
    if (TRUE == check_line_t (df_t [i], ptrs_line))
    {
      ptrdf_tl [i_ret] = df_t [i] ;
      ptrdf_x [i_ret] = df_x [i] ;
      ptrdf_y [i_ret] = df_y [i] ;
      ptrdf_ac [i_ret] = Angle_Normal (Atan2 (
	ptrdf_y [i_ret] - ptrs_circle->y,
	ptrdf_x [i_ret] - ptrs_circle->x)) ;
      i_ret++ ;
    }
  }
  return i_ret ;
}

int IntersectionLl (LINIA  *ptrs_line,
		    LUK  *ptrs_arc,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_tl,
		    double *ptrdf_aa)
/*-----------------------------------*/
{
  int i_ret, i, i_interno ;
  double df_x [2], df_y [2], df_t [2] ;

  i_ret = 0 ;
  i_interno = przeciecieLl_tt (
		    &df_x [0], &df_y [0], &df_t [0],
		    &df_x [1], &df_y [1], &df_t [1],
		    ptrs_line, ptrs_arc) ;
  for (i = 0 ; i < i_interno ; i++)
  {

      if (fabs(1-df_t [i])<LINE_ARC_ALIGN) df_t [i]=1.0;   //<0.005 or 0.001 //this parameter should be available to set in options

      if (fabs(df_t [i])<LINE_ARC_ALIGN) df_t [i]=0.0;   //<0.005 or 0.001 //this parameter should be available to set in options

    if (TRUE == check_line_t (df_t [i], ptrs_line))
    {
      ptrdf_tl [i_ret] = df_t [i] ;
      ptrdf_x [i_ret] = df_x [i] ;
      ptrdf_y [i_ret] = df_y [i] ;
      ptrdf_aa [i_ret] = Angle_Normal (Atan2 (
       ptrdf_y [i_ret] - ptrs_arc->y,
       ptrdf_x [i_ret] - ptrs_arc->x)) ;
      i_ret++ ;
    }
  }
  return i_ret ;
}

int IntersectionlL (LUK  *ptrs_arc,
                    LINIA  *ptrs_line,
                    double *ptrdf_x,
                    double *ptrdf_y,
                    double *ptrdf_tl,
                    double *ptrdf_aa)
{
    IntersectionLl(ptrs_line,ptrs_arc,ptrdf_x,ptrdf_y,ptrdf_tl,ptrdf_aa);
}

int IntersectionOO (OKRAG  *ptrs_circle1,
		    OKRAG  *ptrs_circle2,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_ac1,
		    double *ptrdf_ac2)
/*-----------------------------------*/
{
  int i_ret, i, i_interno ;

  i_ret = 0 ;
  i_interno = przeciecieOO_ex (&ptrdf_x [0], &ptrdf_y [0],
			&ptrdf_x [1], &ptrdf_y [1],
			ptrs_circle1, ptrs_circle2) ;
  for (i = 0 ; i < i_interno ; i++)
  {
    ptrdf_ac1 [i_ret] = Angle_Normal (Atan2 (
      ptrdf_y [i] - ptrs_circle1->y,
      ptrdf_x [i] - ptrs_circle1->x)) ;
    ptrdf_ac2 [i_ret] = Angle_Normal (Atan2 (
      ptrdf_y [i] - ptrs_circle2->y,
      ptrdf_x [i] - ptrs_circle2->x)) ;
    i_ret++ ;
  }
  return i_ret ;
}


int IntersectionlO (LUK    *ptrs_arc,
		    OKRAG  *ptrs_circle,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_aa,
		    double *ptrdf_ac)
/*-----------------------------------*/
{
  OKRAG s_circle ;
  int i_ret, i, i_interno, i_prec ;
  double df_x [2], df_y [2] ;

  s_circle.r = ptrs_arc->r ;
  s_circle.x = ptrs_arc->x ;
  s_circle.y = ptrs_arc->y ;

  i_ret = 0 ;
  i_interno = przeciecieOO_ex (&df_x [0], &df_y [0],
			&df_x [1], &df_y [1],
			&s_circle, ptrs_circle) ;
  i_prec = ((i_interno == 2) ? MPREC_FLOAT : MPREC_4) ;
  for (i = 0 ; i < i_interno ; i++)
  {
    if (FALSE == Point_Arc_Prec (df_x [i], df_y [i], ptrs_arc, i_prec))
    {
      continue ;
    }
    ptrdf_aa [i_ret] = Angle_Normal (Atan2 (
      df_y [i] - ptrs_arc->y,
      df_x [i] - ptrs_arc->x)) ;
    ptrdf_ac [i_ret] = Angle_Normal (Atan2 (
      df_y [i] - ptrs_circle->y,
      df_x [i] - ptrs_circle->x)) ;
    ptrdf_x [i_ret] = df_x [i] ;
    ptrdf_y [i_ret] = df_y [i] ;
    i_ret++ ;
  }
  return i_ret ;
}

int Intersectionll (LUK    *ptrs_arc1,
		    LUK    *ptrs_arc2,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_aa1,
		    double *ptrdf_aa2)
/*-----------------------------------*/
{
  OKRAG s_circle1, s_circle2 ;
  int i_ret, i, i_interno, i_prec ;
  double df_x [2], df_y [2] ;

  s_circle1.r = ptrs_arc1->r ;
  s_circle1.x = ptrs_arc1->x ;
  s_circle1.y = ptrs_arc1->y ;

  s_circle2.r = ptrs_arc2->r ;
  s_circle2.x = ptrs_arc2->x ;
  s_circle2.y = ptrs_arc2->y ;

  i_ret = 0 ;

  //check if 2 arcs have the same ends
  double x11, y11, x12, y12, x21, y21, x22, y22;

    Arc_To_Points(ptrs_arc1, &x11, &y11, &x12, &y12);
    Arc_To_Points(ptrs_arc2, &x21, &y21, &x22, &y22);

    i_interno=0;

    if (Check_if_Equal2(x11,x21) && Check_if_Equal2(y11, y21))  {df_x [0]=x11; df_y[0]=y11;i_interno=1;}
    else if (Check_if_Equal2(x12,x22) && Check_if_Equal2(y12, y22))  {df_x [0]=x12; df_y[0]=y12;i_interno=1;}
    else if (Check_if_Equal2(x11,x22) && Check_if_Equal2(y11, y22))  {df_x [0]=x11; df_y[0]=y11;i_interno=1;}
    else if (Check_if_Equal2(x12,x21) && Check_if_Equal2(y12, y21))  {df_x [0]=x12; df_y[0]=y12;i_interno=1;}
    if (i_interno==0) i_interno = przecieciell_ (&df_x [0], &df_y [0],&s_circle1, &s_circle2) ;

    if (i_interno==0) i_interno = przeciecieOO_ex (&df_x [0], &df_y [0],
			&df_x [1], &df_y [1],
			&s_circle1, &s_circle2) ;

  i_prec = ((i_interno == 2) ? MPREC_3: MPREC_2) ;
  for (i = 0 ; i < i_interno ; i++)
  {
    if (FALSE == Point_Arc_Prec (df_x [i], df_y [i], ptrs_arc1, i_prec) ||
	FALSE == Point_Arc_Prec (df_x [i], df_y [i], ptrs_arc2, i_prec))
    {
      continue ;
    }
    ptrdf_aa1 [i_ret] = Angle_Normal (Atan2 (
      df_y [i] - ptrs_arc1->y,
      df_x [i] - ptrs_arc1->x)) ;
    ptrdf_aa2 [i_ret] = Angle_Normal (Atan2 (
      df_y [i] - ptrs_arc2->y,
      df_x [i] - ptrs_arc2->x)) ;
    ptrdf_x [i_ret] = df_x [i] ;
    ptrdf_y [i_ret] = df_y [i] ;
    i_ret++ ;
  }
  return i_ret ;
}


static int intersection_solid (
		    WIELOKAT *ptrs_solid,
		    void *ptrs_ob,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_ts,
		    double *ptrdf_t,
		    int (*inter)(void *, void *, double *,  double *, double *, double *))
/*-----------------------------------*/
{
  int i_ret, i, j, i_interno ;
  double df_x1, df_y1 ;
  double df_x [2], df_y [2], df_t [2], df_ts [2] ;
  LINIA s_line ;

  i_ret = 0 ;
  df_x1 = ptrs_solid->xy [ptrs_solid->lp-2] ;
  df_y1 = ptrs_solid->xy [ptrs_solid->lp-1] ;
  for (i = 0 ; i < ptrs_solid->lp ; i += 2)
  {
    s_line.x1 = df_x1 ;
    s_line.y1 = df_y1 ;
    s_line.x2 = ptrs_solid->xy [i + 0] ;
    s_line.y2 = ptrs_solid->xy [i + 1] ;
    i_interno = inter (&s_line,
		    ptrs_ob,
		    df_x, df_y,
		    df_ts, df_t) ;
    for (j = 0 ; j < i_interno ; j++)
    {
      ptrdf_x [i_ret] = df_x [j] ;
      ptrdf_y [i_ret] = df_y [j] ;
      ptrdf_t [i_ret] = df_t [j] ;
      ptrdf_ts [i_ret] = df_ts [j] + (double)(i / 2) ;
      i_ret++ ;
    }
    df_x1 = s_line.x2 ;
    df_y1 = s_line.y2 ;
  }
  return i_ret ;
}

static int intersection_solidarc (
        SOLIDARC *ptrs_solidarc,
        void *ptrs_ob,
        double *ptrdf_x,
        double *ptrdf_y,
        double *ptrdf_ts,
        double *ptrdf_t,
        int (*interLine)(void *, void *, double *,  double *, double *, double *),
        int (*interArc)(void *, void *, double *,  double *, double *, double *))
/*-----------------------------------------------------------------------------*/
{
    int i_ret, i, j, i_interno ;
    double df_x1, df_y1 ;
    double df_x [2], df_y [2], df_t [2], df_ts [2] ;
    LINIA s_line ;
    LINIA L_right=Ldef, L_left=Ldef ;
    LUK l_inner1=ldef, l_inner2=ldef, l_outer1=ldef, l_outer2=ldef;
    int ln;

    ln=solidarc_elements(ptrs_solidarc, &L_left, &L_right, &l_inner1, &l_inner2, &l_outer1, &l_outer2);

    i_ret = 0 ;
    i_interno = interLine (&L_left, ptrs_ob, df_x, df_y, df_ts, df_t) ;
    for (j = 0 ; j < i_interno ; j++)
    {
        ptrdf_x [i_ret] = df_x [j] ;
        ptrdf_y [i_ret] = df_y [j] ;
        ptrdf_t [i_ret] = df_t [j] ;
        ptrdf_ts [i_ret] = df_ts [j] + (double)(j / 2) ;
        i_ret++ ;
    }

    i_interno = interLine (&L_right, ptrs_ob, df_x, df_y, df_ts, df_t) ;
    for (j = 0 ; j < i_interno ; j++)
    {
        ptrdf_x [i_ret] = df_x [j] ;
        ptrdf_y [i_ret] = df_y [j] ;
        ptrdf_t [i_ret] = df_t [j] ;
        ptrdf_ts [i_ret] = df_ts [j] + (double)(j / 2) ;
        i_ret++ ;
    }

    i_interno = interArc (&l_inner1, ptrs_ob, df_x, df_y, df_ts, df_t) ;
    for (j = 0 ; j < i_interno ; j++)
    {
        ptrdf_x [i_ret] = df_x [j] ;
        ptrdf_y [i_ret] = df_y [j] ;
        ptrdf_t [i_ret] = df_t [j] ;
        ptrdf_ts [i_ret] = df_ts [j] + (double)(j / 2) ;
        i_ret++ ;
    }

    i_interno = interArc (&l_outer1, ptrs_ob, df_x, df_y, df_ts, df_t) ;
    for (j = 0 ; j < i_interno ; j++)
    {
        ptrdf_x [i_ret] = df_x [j] ;
        ptrdf_y [i_ret] = df_y [j] ;
        ptrdf_t [i_ret] = df_t [j] ;
        ptrdf_ts [i_ret] = df_ts [j] + (double)(j / 2) ;
        i_ret++ ;
    }
    if (ln>1)
    {
        i_interno = interArc (&l_inner2, ptrs_ob, df_x, df_y, df_ts, df_t) ;
        for (j = 0 ; j < i_interno ; j++)
        {
            ptrdf_x [i_ret] = df_x [j] ;
            ptrdf_y [i_ret] = df_y [j] ;
            ptrdf_t [i_ret] = df_t [j] ;
            ptrdf_ts [i_ret] = df_ts [j] + (double)(j / 2) ;
            i_ret++ ;
        }

        i_interno = interArc (&l_outer2, ptrs_ob, df_x, df_y, df_ts, df_t) ;
        for (j = 0 ; j < i_interno ; j++)
        {
            ptrdf_x [i_ret] = df_x [j] ;
            ptrdf_y [i_ret] = df_y [j] ;
            ptrdf_t [i_ret] = df_t [j] ;
            ptrdf_ts [i_ret] = df_ts [j] + (double)(j / 2) ;
            i_ret++ ;
        }
    }

    return i_ret ;
}

static int intersection_spline(
		SPLINE *ptrs_spline,
		void *ptrs_ob,
		double *ptrdf_x,
		double *ptrdf_y,
		double *ptrdf_ts,
		double *ptrdf_t,
		int(*inter)(void *, void *, double *, double *, double *, double *))
	/*--------------------------------------------------------*/
{
	LINIA s_line;
	int i_ret, i, j, i_interno;
	double df_x1, df_y1;
	double df_x[2], df_y[2], df_t[2], df_ts[2];
	float out_x[MaxNpts], out_y[MaxNpts];

	i_ret = 0;

//#define Npts 90  //for more precission


	calc_bspline(ptrs_spline->lp, ptrs_spline->npts, ptrs_spline->xy, Npts, &out_x, &out_y);
	for (i = 0; i < Npts - 1; i++)
	{
			s_line.x1 = out_x[i];
			s_line.y1 = out_y[i];
			s_line.x2 = out_x[i + 1];
			s_line.y2 = out_y[i + 1];
			i_interno = inter(&s_line,
				ptrs_ob,
				df_x, df_y,
				df_ts, df_t);
			for (j = 0; j < i_interno; j++)
			{
				ptrdf_x[i_ret] = df_x[j];
				ptrdf_y[i_ret] = df_y[j];
				ptrdf_t[i_ret] = df_t[j];
				ptrdf_ts[i_ret] = df_ts[j] + (double)(i);
				i_ret++;
			}
			df_x1 = s_line.x2;
			df_y1 = s_line.y2;
	}
	return i_ret;
}


int IntersectionWL (WIELOKAT *ptrs_solid,
		    LINIA *ptrs_line,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_ts,
		    double *ptrdf_tl)
/*-----------------------------------*/
{
  return intersection_solid (
	  ptrs_solid, ptrs_line, ptrdf_x, ptrdf_y,
	  ptrdf_ts, ptrdf_tl,
	  (int (*)(void *, void *, double *,
	  double *, double *, double *)) IntersectionLL) ;
}

int IntersectionSAL (SOLIDARC *ptrs_solidarc,
                    LINIA *ptrs_line,
                    double *ptrdf_x,
                    double *ptrdf_y,
                    double *ptrdf_ts,
                    double *ptrdf_tl)
/*-----------------------------------*/
{
    return intersection_solidarc (
            ptrs_solidarc, ptrs_line, ptrdf_x, ptrdf_y,
            ptrdf_ts, ptrdf_tl,
            (int (*)(void *, void *, double *, double *, double *, double *)) IntersectionLL,
            (int (*)(void *, void *, double *, double *, double *, double *)) IntersectionlL) ;
}

int IntersectionSL(SPLINE *ptrs_spline,
	LINIA *ptrs_line,
	double *ptrdf_x,
	double *ptrdf_y,
	double *ptrdf_ts,
	double *ptrdf_tl)
	/*-----------------------------------*/
{
	return intersection_spline(
		ptrs_spline, ptrs_line, ptrdf_x, ptrdf_y,
		ptrdf_ts, ptrdf_tl,
		(int(*)(void *, void *, double *,
			double *, double *, double *)) IntersectionLL);
}

int IntersectionWl (WIELOKAT *ptrs_solid,
		    LUK *ptrs_arc,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_ts,
		    double *ptrdf_ta)
/*-----------------------------------*/
{
  return intersection_solid (
	  ptrs_solid, ptrs_arc, ptrdf_x, ptrdf_y,
	  ptrdf_ts, ptrdf_ta,
	  (int (*)(void *, void *, double *,
	  double *, double *, double *)) IntersectionLl) ;
}

int IntersectionSl(SPLINE *ptrs_spline,
	LUK *ptrs_arc,
	double *ptrdf_x,
	double *ptrdf_y,
	double *ptrdf_ts,
	double *ptrdf_ta)
	/*-----------------------------------*/
{
	return intersection_spline(
		ptrs_spline, ptrs_arc, ptrdf_x, ptrdf_y,
		ptrdf_ts, ptrdf_ta,
		(int(*)(void *, void *, double *,
			double *, double *, double *)) IntersectionLl);
}


int IntersectionWO (WIELOKAT *ptrs_solid,
		    OKRAG  *ptrs_circle,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_ts,
		    double *ptrdf_tc)
/*-----------------------------------*/
{
  return intersection_solid (
	  ptrs_solid, ptrs_circle, ptrdf_x, ptrdf_y,
	  ptrdf_ts, ptrdf_tc,
	  (int (*)(void *, void *, double *,
	  double *, double *, double *)) IntersectionLO) ;
}

int IntersectionSO(SPLINE *ptrs_spline,
	OKRAG  *ptrs_circle,
	double *ptrdf_x,
	double *ptrdf_y,
	double *ptrdf_ts,
	double *ptrdf_tc)
	/*-----------------------------------*/
{
	return intersection_spline(
		ptrs_spline, ptrs_circle, ptrdf_x, ptrdf_y,
		ptrdf_ts, ptrdf_tc,
		(int(*)(void *, void *, double *,
			double *, double *, double *)) IntersectionLO);
}


int IntersectionWW (WIELOKAT *ptrs_solid1,
		    WIELOKAT *ptrs_solid2,
		    double *ptrdf_x,
		    double *ptrdf_y,
		    double *ptrdf_ts1,
		    double *ptrdf_ts2)
/*-----------------------------------*/
{
  int i_ret, i, j, i_interno ;
  double df_x1, df_y1 ;
  double df_x [4], df_y [4], df_tl [4], df_ts1 [4] ;
  LINIA s_line ;

  i_ret = 0 ;
  df_x1 = ptrs_solid2->xy [ptrs_solid2->lp-2] ;
  df_y1 = ptrs_solid2->xy [ptrs_solid2->lp-1] ;
  for (i = 0 ; i < ptrs_solid2->lp ; i += 2)
  {
    s_line.x1 = df_x1 ;
    s_line.y1 = df_y1 ;
    s_line.x2 = ptrs_solid2->xy [i + 0] ;
    s_line.y2 = ptrs_solid2->xy [i + 1] ;
    i_interno = IntersectionWL (ptrs_solid1,
		    &s_line,
		    df_x,
		    df_y,
		    df_ts1,
		    df_tl) ;
    for (j = 0 ; j < i_interno ; j++)
    {
      ptrdf_x [i_ret] = df_x [j] ;
      ptrdf_y [i_ret] = df_y [j] ;
      ptrdf_ts1 [i_ret] = df_ts1 [j] ;
      ptrdf_ts2 [i_ret] = df_tl [j] + (double)(i / 2) ;
      i_ret++ ;
    }
    df_x1 = s_line.x2 ;
    df_y1 = s_line.y2 ;
  }
  return i_ret ;
}

int IntersectionSW(SPLINE *ptrs_spline,
	WIELOKAT *ptrs_solid,
	double *ptrdf_x,
	double *ptrdf_y,
	double *ptrdf_ts1,
	double *ptrdf_ts2)
	/*-----------------------------------*/
{
	int i_ret, i, j, i_interno;
	double df_x1, df_y1;
	double df_x[4], df_y[4], df_tl[4], df_ts1[4];
	LINIA s_line;
	float out_x[MaxNpts], out_y[MaxNpts];

//#define Npts 90  //for more precission
	calc_bspline(ptrs_spline->lp, ptrs_spline->npts, ptrs_spline->xy, Npts, &out_x, &out_y);

	i_ret = 0;
	for (i = 0; i < Npts - 1; i++)
	{
		s_line.x1 = out_x[i];
		s_line.y1 = out_y[i];
		s_line.x2 = out_x[i + 1];
		s_line.y2 = out_y[i + 1];
		i_interno = IntersectionWL(ptrs_solid,
			&s_line,
			df_x,
			df_y,
			df_ts1,
			df_tl);
		for (j = 0; j < i_interno; j++)
		{
			ptrdf_x[i_ret] = df_x[j];
			ptrdf_y[i_ret] = df_y[j];
			ptrdf_ts1[i_ret] = df_ts1[j];
			ptrdf_ts2[i_ret] = df_tl[j] + (double)(i / 2);
			i_ret++;
		}
		df_x1 = s_line.x2;
		df_y1 = s_line.y2;
	}
	return i_ret;
}


int IntersectionSS(SPLINE *ptrs_spline1,
	SPLINE *ptrs_spline2,
	double *ptrdf_x,
	double *ptrdf_y,
	double *ptrdf_ts1,
	double *ptrdf_ts2)
	/*-----------------------------------*/
{
	int i_ret, i, j, i_interno, n;
	double df_x1, df_y1;
	double df_x[92], df_y[92], df_tl[92], df_ts1[92];
	LINIA s_line;
	float out_x[MaxNpts], out_y[MaxNpts];

//#define Npts 90

	calc_bspline(ptrs_spline2->lp, ptrs_spline2->npts, ptrs_spline2->xy, Npts, &out_x, &out_y);
	
	i_ret = 0;
	for (i = 0; i < Npts - 1; i++)
	{
		if (ptrs_spline1 == ptrs_spline2) n = i;
		else n = -1;
		s_line.x1 = out_x[i];
		s_line.y1 = out_y[i];
		s_line.x2 = out_x[i + 1];
		s_line.y2 = out_y[i + 1];
		i_interno = IntersectionSL(ptrs_spline1,
			&s_line,
			df_x,
			df_y,
			df_ts1,
			df_tl);
		for (j = 0; j < i_interno; j++)
		{
			ptrdf_x[i_ret] = df_x[j];
			ptrdf_y[i_ret] = df_y[j];
			ptrdf_ts1[i_ret] = df_ts1[j];
			ptrdf_ts2[i_ret] = df_tl[j] + (double)(i / 2);
			i_ret++;
		}
		df_x1 = s_line.x2;
		df_y1 = s_line.y2;
	}
	return i_ret;
}


