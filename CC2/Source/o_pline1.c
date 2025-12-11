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

#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<math.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_pline.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_osnap.h"

extern NODE bnodes[MAX_NODES];
extern double curviness;
extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
extern void set_bnodes(SPLINE* ptrs_spline, float *ad_xy, int* node_count_);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void get_control_points(NODE n1, NODE n2, float points[]);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);

static double check_angle (double, LUK *);
static double check_elliptical_angle (double k, ELLIPTICALARC *el);
static BOOL check_head_pline (BLOK *) ;

BOOL Get_End_Pline (void 	*ptr_pl,
		    void	**ptr_ob,	/*adres ostatniego  obiektu*/
		    BOOL	*b_first_end,	/*ktory koniec objektu jest ostatni*/
		    double 	df_x_pl_beg,
		    double 	df_y_pl_beg,
		    double 	*df_x_pl_end,
		    double 	*df_y_pl_end)
/*-------------------------------------------*/
{
  BLOK * ptrs_block ;
  long_long off, offk, ad ;
  NAGLOWEK *nag ;
  LUK *ptrs_arc_l, *ptrs_arc_bl ;
  SOLIDARC *ptrs_solidarc_l, *ptrs_solidarc_bl ;
  LINIA *ptrs_line_bl ;
  SPLINE *ptrs_spline_bl;
  void *ptr_before_last, *ptr_last ;

  double x1, y1, x2, y2, x1l, y1l, x2l, y2l ;

  *df_x_pl_end = df_x_pl_beg ;
  *df_y_pl_end = df_y_pl_beg ;
  *ptr_ob = NULL ;
  ptrs_block = (BLOK*)ptr_pl ;
  if (FALSE == check_head_pline (ptrs_block))
  {
    return FALSE ;
  }
  ptr_before_last = NULL ;
  ptr_last = NULL ;
  off =  (char  *)ptrs_block - dane ;
  offk = off  + ptrs_block->n + sizeof (NAGLOWEK) - 1 ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Olinia :
      case Oluk :
	  case Ospline:
      case Osolidarc:
	if (ptr_last != NULL)
	{
	  ptr_before_last = ptr_last ;
	}
	ptr_last = (void *)nag ;
	break ;
      default :
	break ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  if (ptr_last == NULL)
  {
    return FALSE ;
  }
  *ptr_ob = ptr_last ;
  if (((NAGLOWEK*)ptr_last)->obiekt == Olinia)
  {
     *df_x_pl_end = ((LINIA*)ptr_last)->x2 ;
     *df_y_pl_end = ((LINIA*)ptr_last)->y2 ;
     *b_first_end = FALSE ;
     return TRUE ;
  }
  else if (((NAGLOWEK*)ptr_last)->obiekt == Ospline)
  {
	  *df_x_pl_end = ((SPLINE*)ptr_last)->xy[((SPLINE*)ptr_last)->lp - 2];
	  *df_y_pl_end = ((SPLINE*)ptr_last)->xy[((SPLINE*)ptr_last)->lp - 1];
	  *b_first_end = FALSE;
	  return TRUE;
  }
  if (((NAGLOWEK*)ptr_last)->obiekt == Oluk)
  {
      ptrs_arc_l = (LUK *) ptr_last;
      x1l = ptrs_arc_l->x + ptrs_arc_l->r * cos(ptrs_arc_l->kat1);
      y1l = ptrs_arc_l->y + ptrs_arc_l->r * sin(ptrs_arc_l->kat1);
      x2l = ptrs_arc_l->x + ptrs_arc_l->r * cos(ptrs_arc_l->kat2);
      y2l = ptrs_arc_l->y + ptrs_arc_l->r * sin(ptrs_arc_l->kat2);
      if (ptr_before_last == NULL) {
          x1 = x2 = df_x_pl_beg;
          y1 = y2 = df_y_pl_beg;
      } else {
          ptrs_arc_bl = (LUK *) ptr_before_last;
          ptrs_line_bl = (LINIA *) ptr_before_last;
          ptrs_spline_bl = (SPLINE *) ptr_before_last;
          if (((NAGLOWEK *) ptr_before_last)->obiekt == Olinia) {
              x1 = ptrs_line_bl->x1;
              y1 = ptrs_line_bl->y1;
              x2 = ptrs_line_bl->x2;
              y2 = ptrs_line_bl->y2;
          } else if (((NAGLOWEK *) ptr_before_last)->obiekt == Ospline) {
              x1 = ptrs_spline_bl->xy[0];
              y1 = ptrs_spline_bl->xy[1];
              x2 = ptrs_spline_bl->xy[ptrs_spline_bl->lp - 2];
              y2 = ptrs_spline_bl->xy[ptrs_spline_bl->lp - 1];
          } else {
              x1 = ptrs_arc_bl->x + ptrs_arc_bl->r * cos(ptrs_arc_bl->kat1);
              y1 = ptrs_arc_bl->y + ptrs_arc_bl->r * sin(ptrs_arc_bl->kat1);
              x2 = ptrs_arc_bl->x + ptrs_arc_bl->r * cos(ptrs_arc_bl->kat2);
              y2 = ptrs_arc_bl->y + ptrs_arc_bl->r * sin(ptrs_arc_bl->kat2);
          }
      }
  }
  else
  { //solidarc
      ptrs_solidarc_l = (SOLIDARC *) ptr_last;
      x1l = ptrs_solidarc_l->x + ptrs_solidarc_l->r * cos(ptrs_solidarc_l->kat1);
      y1l = ptrs_solidarc_l->y + ptrs_solidarc_l->r * sin(ptrs_solidarc_l->kat1);
      x2l = ptrs_solidarc_l->x + ptrs_solidarc_l->r * cos(ptrs_solidarc_l->kat2);
      y2l = ptrs_solidarc_l->y + ptrs_solidarc_l->r * sin(ptrs_solidarc_l->kat2);
      if (ptr_before_last == NULL) {
          x1 = x2 = df_x_pl_beg;
          y1 = y2 = df_y_pl_beg;
      } else {
          ptrs_arc_bl = (LUK *) ptr_before_last;
          ptrs_solidarc_bl = (SOLIDARC *) ptr_before_last;
          ptrs_line_bl = (LINIA *) ptr_before_last;
          ptrs_spline_bl = (SPLINE *) ptr_before_last;
          if (((NAGLOWEK *) ptr_before_last)->obiekt == Olinia) {
              x1 = ptrs_line_bl->x1;
              y1 = ptrs_line_bl->y1;
              x2 = ptrs_line_bl->x2;
              y2 = ptrs_line_bl->y2;
          } else if (((NAGLOWEK *) ptr_before_last)->obiekt == Ospline) {
              x1 = ptrs_spline_bl->xy[0];
              y1 = ptrs_spline_bl->xy[1];
              x2 = ptrs_spline_bl->xy[ptrs_spline_bl->lp - 2];
              y2 = ptrs_spline_bl->xy[ptrs_spline_bl->lp - 1];
          } else if (((NAGLOWEK *) ptr_before_last)->obiekt == Oluk) {
              x1 = ptrs_arc_bl->x + ptrs_arc_bl->r * cos(ptrs_arc_bl->kat1);
              y1 = ptrs_arc_bl->y + ptrs_arc_bl->r * sin(ptrs_arc_bl->kat1);
              x2 = ptrs_arc_bl->x + ptrs_arc_bl->r * cos(ptrs_arc_bl->kat2);
              y2 = ptrs_arc_bl->y + ptrs_arc_bl->r * sin(ptrs_arc_bl->kat2);
          } else {  //solidarc
              x1 = ptrs_solidarc_bl->x + ptrs_solidarc_bl->r * cos(ptrs_solidarc_bl->kat1);
              y1 = ptrs_solidarc_bl->y + ptrs_solidarc_bl->r * sin(ptrs_solidarc_bl->kat1);
              x2 = ptrs_solidarc_bl->x + ptrs_solidarc_bl->r * cos(ptrs_solidarc_bl->kat2);
              y2 = ptrs_solidarc_bl->y + ptrs_solidarc_bl->r * sin(ptrs_solidarc_bl->kat2);
          }
      }
  }
  *df_x_pl_end = x1l ;
  *df_y_pl_end = y1l ;
  *b_first_end = TRUE ;
  if ((TRUE == Check_if_Equal (x1l, x1) &&
       TRUE == Check_if_Equal (y1l, y1)) ||
      (TRUE == Check_if_Equal (x1l, x2) &&
       TRUE == Check_if_Equal (y1l, y2)))
  {
    *df_x_pl_end = x2l ;
    *df_y_pl_end = y2l ;
    *b_first_end = FALSE ;
  }
  return TRUE ;
}



BOOL Get_Begin_Pline (void 	*ptr_pl,
		      void	**ptr_ob,	/*adres pierwszego obiektu*/
		      BOOL	*b_first_end,	/*ktory koniec 1-go objektu jest pierwszy*/
		      double 	*df_x_pl_beg,
		      double 	*df_y_pl_beg)
/*------------------------------------------*/
{
  BLOK * ptrs_block ;
  long_long off, offk, ad ;
  NAGLOWEK *nag ;
  LUK *ptrs_arc_l, *ptrs_arc_bl ;
  LINIA *ptrs_line_bl ;
  SPLINE *ptrs_spline_bl;
  void *ptr_first, *ptr_second ;
  double x1, y1, x2, y2, x11, y11, x21, y21 ;

  *ptr_ob = NULL ;
  ptrs_block = (BLOK*)ptr_pl ;
  if (FALSE == check_head_pline (ptrs_block))
  {
    return FALSE ;
  }
  ptr_first = NULL ;
  ptr_second = NULL ;
  off =  (char  *)ptrs_block - dane ;
  offk = off  + ptrs_block->n + sizeof (NAGLOWEK) - 1 ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Olinia :
      case Oluk :
	  case Ospline:
	if (ptr_first != NULL)
	{
	  ptr_second = (void *)nag ;
	}
	if (ptr_first == NULL)
	{
	  ptr_first = (void *)nag ;
	}
	break ;
      default :
	break ;
    }
    if (ptr_second != NULL)
    {
      break ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  if (ptr_first == NULL)
  {
    return FALSE ;
  }
  *ptr_ob = ptr_first ;
  if (((NAGLOWEK*)ptr_first)->obiekt == Olinia)
  {
     *df_x_pl_beg = ((LINIA*)ptr_first)->x1 ;
     *df_y_pl_beg = ((LINIA*)ptr_first)->y1 ;
     *b_first_end = TRUE ;
     return TRUE ;
  }
  else if (((NAGLOWEK*)ptr_first)->obiekt == Ospline)
  {
	  *df_x_pl_beg = ((SPLINE*)ptr_first)->xy[0];
	  *df_y_pl_beg = ((SPLINE*)ptr_first)->xy[1];
	  *b_first_end = TRUE;
	  return TRUE;
  }
  ptrs_arc_l = (LUK*)ptr_first ;
  x11 = ptrs_arc_l->x + ptrs_arc_l->r * cos (ptrs_arc_l->kat1) ;
  y11 = ptrs_arc_l->y + ptrs_arc_l->r * sin (ptrs_arc_l->kat1) ;
  x21 = ptrs_arc_l->x + ptrs_arc_l->r * cos (ptrs_arc_l->kat2) ;
  y21 = ptrs_arc_l->y + ptrs_arc_l->r * sin (ptrs_arc_l->kat2) ;
  if (ptr_second == NULL)
  {
    x1 = x2 = x11 ;
    y1 = y2 = y11 ;
  }
  else
  {
    ptrs_arc_bl = (LUK*)ptr_second ;
    ptrs_line_bl = (LINIA*)ptr_second ;
	ptrs_spline_bl = (SPLINE*)ptr_second;
    if (((NAGLOWEK*)ptr_second)->obiekt == Olinia)
    {
      x1 = ptrs_line_bl->x1 ;
      y1 = ptrs_line_bl->y1 ;
      x2 = ptrs_line_bl->x2 ;
      y2 = ptrs_line_bl->y2 ;
    }
	else if (((NAGLOWEK*)ptr_second)->obiekt == Ospline)
	{
		x1 = ptrs_spline_bl->xy[0];
		y1 = ptrs_spline_bl->xy[1];
		x2 = ptrs_spline_bl->xy[ptrs_spline_bl->lp - 2];
		y2 = ptrs_spline_bl->xy[ptrs_spline_bl->lp - 1];
	}
    else
    {
      x1 = ptrs_arc_bl->x + ptrs_arc_bl->r * cos (ptrs_arc_bl->kat1) ;
      y1 = ptrs_arc_bl->y + ptrs_arc_bl->r * sin (ptrs_arc_bl->kat1) ;
      x2 = ptrs_arc_bl->x + ptrs_arc_bl->r * cos (ptrs_arc_bl->kat2) ;
      y2 = ptrs_arc_bl->y + ptrs_arc_bl->r * sin (ptrs_arc_bl->kat2) ;
    }
  }
  *df_x_pl_beg = x11 ;
  *df_y_pl_beg = y11 ;
  *b_first_end = TRUE ;
  if ((TRUE == Check_if_Equal (x11, x1) &&
       TRUE == Check_if_Equal (y11, y1)) ||
      (TRUE == Check_if_Equal (x11, x2) &&
       TRUE == Check_if_Equal (y11, y2)))
  {
    *df_x_pl_beg = x21 ;
    *df_y_pl_beg = y21 ;
    *b_first_end = FALSE ;
  }
  return TRUE ;
}

BOOL Set_Beginning_Pline_Factory (BLOK *blk_adr, double *df_x_pl_beg, double *df_y_pl_beg)
{
    void *ptr_ob ;
    BOOL b_first_end ;
    BOOL ret;
    double df_x_pl_beg1, df_y_pl_beg1;

    ret=Get_Begin_Pline (blk_adr, &ptr_ob, &b_first_end, &df_x_pl_beg1, &df_y_pl_beg1);
    *df_x_pl_beg=df_x_pl_beg1;
    *df_y_pl_beg=df_y_pl_beg1;

    return ret;

}


int Check_Pline_to_Fillet (void *ptr_ob1, void *ptr_ob2)
/*---------------------------------------------------*/
{
  NAGLOWEK *nag1, *nag2 ;
  void *ptr_pl1, *ptr_pl2 ;
  void *ptr_first, *ptr_last ;

  nag1 = (NAGLOWEK*)ptr_ob1 ;
  nag2 = (NAGLOWEK*)ptr_ob2 ;
  if ((nag1->obiektt2 == O2BlockPline ||
       nag2->obiektt2 == O2BlockPline))
  {
    if (nag1->obiektt2 != O2BlockPline ||
	nag2->obiektt2 != O2BlockPline)
    {
      return FILLET_PL_NO ;
    }
    if (labs ((char *)ptr_ob1 - (char *)ptr_ob2) == sizeof (LINIA))
    {
      return FILLET_PL_YES ;
    }
    if (NULL == (ptr_pl1 = LASTB((char  *)ptr_ob1)) ||
	NULL == (ptr_pl2 = LASTB((char  *)ptr_ob2)) ||
	(char *)ptr_pl1 != (char *)ptr_pl2)
    {
      return FILLET_PL_NO ;
    }
    if (NULL == (ptr_first = Get_Pline_First_Ob (ptr_pl1)) ||
	NULL == (ptr_last = Get_Pline_Last_Ob (ptr_pl1)))
    {
      return FILLET_PL_YES ;  /* albo odwrotnie ????*/
    }
    if (((char *)ptr_ob1 == (char *)ptr_first ||
	(char *)ptr_ob1 == (char *)ptr_last)  &&
       ((char *)ptr_ob2 == (char *)ptr_first ||
	(char *)ptr_ob2 == (char *)ptr_last))
    {
      return FILLET_PL_YES_OUT ;
    }
    return FILLET_PL_NO ;
 }
 return FILLET_PL_YES_OTHER ;
}


void * Get_Pline_Last_Ob (void* ptr_pl)
/*-----------------------------------*/
{
  BLOK * ptrs_block ;
  long_long off, offk, ad ;
  NAGLOWEK *nag ;
  void  *ptr_last ;

  ptrs_block = (BLOK*)ptr_pl ;
  if (FALSE == check_head_pline (ptrs_block))
  {
    return NULL ;
  }
  ptr_last = NULL ;
  off =  (char  *)ptrs_block - dane ;
  offk = off  + ptrs_block->n + sizeof (NAGLOWEK) - 1 ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Olinia :
      case Oluk :
	ptr_last = (void *)nag ;
	break ;
      default :
	break ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  return ptr_last ;
}

void * Get_Pline_First_Ob (void* ptr_pl)
/*-----------------------------------*/
{
  BLOK * ptrs_block ;
  char   *ptr_first ;

  ptrs_block = (BLOK*)ptr_pl ;
  if (FALSE == check_head_pline (ptrs_block))
  {
    return NULL ;
  }
  ptr_first = (char *)ptrs_block ;
  ptr_first += sizeof(NAGLOWEK) + B3 + ptrs_block->dlugosc_opisu_obiektu ;

  return (void*)ptr_first ;
}


double Get_Len_Pline (BLOK *ptrs_block)
/*-------------------------------------*/
{
  long_long off, offk, ad ;
  NAGLOWEK *nag ;
  LINIA *ptrs_line ;
  LUK *ptrs_arc ;
  SPLINE *ptrs_spline;
  double df_len, df_angle1, df_angle2 ;
  float out_x[MaxNpts], out_y[MaxNpts];
  int npts, i, j, k;
  double  dx, dy, df_line_len;

  SPLINE tmp_spline = Splinedef;
  double bnodes_2_tangent_org, bnodes_2_tangent;
  int node_count;

  df_len = 0 ;
  if (FALSE == check_head_pline (ptrs_block))
  {
    return 0 ;
  }
  off =  (char  *)ptrs_block - dane ;
  offk = off  + ptrs_block->n + sizeof (NAGLOWEK) - 1 ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Olinia :
	ptrs_line = (LINIA*)nag ;
	df_len += Dist_PP (ptrs_line->x1, ptrs_line->y1, ptrs_line->x2, ptrs_line->y2) ;
	break ;
	  case Ospline:
		  ptrs_spline = (SPLINE*)nag;
#define NPA 90

		  npts = NPA;

          if (ptrs_spline->npts < 5)
          {
              calc_bspline(ptrs_spline->lp, ptrs_spline->npts, ptrs_spline->xy, npts, out_x, out_y);

              df_line_len = 0.0;

              for (i = 0; i < (npts - 1); i++)
              {
                  dx = out_x[i + 1] - out_x[i];
                  dy = out_y[i + 1] - out_y[i];
                  df_line_len += (sqrt(dx * dx + dy * dy));
              }

              df_len += df_line_len;
          }
          else
          {
              curviness = ptrs_spline->xy[ptrs_spline->lp];

              tmp_spline.npts = 4;
              tmp_spline.lp = 8;
              tmp_spline.kolor = ptrs_spline->kolor;
              tmp_spline.typ = ptrs_spline->typ;

              node_count = 1;

              set_bnodes(ptrs_spline, ptrs_spline->xy, &node_count);

              calc_tangents(ptrs_spline->closed, &node_count, &bnodes_2_tangent);

              if (ptrs_spline->closed == 0)
              {
                  bnodes_2_tangent_org = bnodes[2].tangent;
                  bnodes[2].tangent = bnodes_2_tangent;
                  get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
                  calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

                  df_line_len = 0.0;

                  for (i = 0; i < (npts - 1); i++)
                  {
                      dx = out_x[i + 1] - out_x[i];
                      dy = out_y[i + 1] - out_y[i];
                      df_line_len += (sqrt(dx * dx + dy * dy));
                  }
                  df_len += df_line_len;
              
                  bnodes[2].tangent = bnodes_2_tangent_org;
              }

              for (k = 2; k < node_count - 2; k++)  //1 - extra_node
              {
                  get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
                  calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

                  df_line_len = 0.0;

                  for (i = 0; i < (npts - 1); i++)
                  {
                      dx = out_x[i + 1] - out_x[i];
                      dy = out_y[i + 1] - out_y[i];
                      df_line_len += (sqrt(dx * dx + dy * dy));
                  }
                  df_len += df_line_len;
              }

              if ((node_count > 3) && (ptrs_spline->closed == 0))
              {
                  get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
                  calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

                  df_line_len = 0.0;

                  for (i = 0; i < (npts - 1); i++)
                  {
                      dx = out_x[i + 1] - out_x[i];
                      dy = out_y[i + 1] - out_y[i];
                      df_line_len += (sqrt(dx * dx + dy * dy));
                  }
                  df_len += df_line_len;
              }
          }
		  break;
      case Oluk :
	ptrs_arc = (LUK*)nag ;
	df_angle1 = Angle_Normal (ptrs_arc->kat1) ;
	df_angle2 = Angle_Normal (ptrs_arc->kat2) ;
	if (df_angle1 > df_angle2)
	{
	  df_angle2 += Pi2 ;
	}
	df_len += fabs (df_angle2 - df_angle1) * ptrs_arc->r ;
	break ;
      default :
	break ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  return df_len ;
}

BOOL Check_if_Inside_Pline (BLOK *ptrs_block, void *ptr_ob)
/*-------------------------------------------------------*/
{	/*nie uwzgledniono ze pline moze byc zamknieta*/
  char  *ptrh_beg ;
  BOOL b_ret ;

  b_ret = FALSE ;
  if (FALSE == check_head_pline (ptrs_block))
  {
    return b_ret ;
  }
  b_ret = TRUE ;
  ptrh_beg = (char *)ptrs_block ;
  ptrh_beg +=  sizeof(NAGLOWEK) + B3 + ptrs_block->dlugosc_opisu_obiektu ;
  if (ptrh_beg == (char *)ptr_ob)
  {
    b_ret = FALSE ;
  }
  else
  {
    ptrh_beg = (char *)ptrs_block ;
    ptrh_beg += sizeof (NAGLOWEK) + ptrs_block->n ;
    switch (((NAGLOWEK*)ptr_ob)->obiekt)
    {
      case Olinia :
	ptrh_beg -= sizeof(LINIA) ;
	break ;
      case Oluk :
	ptrh_beg -= sizeof(LUK) ;
	break ;
      default :
	break ;
    }
    if (ptrh_beg == (char *)ptr_ob)
    {
      b_ret = FALSE ;
    }
  }
  return b_ret ;
}

BOOL Check_if_Close_Pline (BLOK *ptrs_pl)
/*-----------------------------------------*/
{
  BOOL b_ret ;
  void *ptr_ob ;
  BOOL b_first_end ;
  double df_x_pl_beg, df_y_pl_beg, df_x_pl_end, df_y_pl_end ;

  b_ret = FALSE ;
  if (FALSE == check_head_pline (ptrs_pl))
  {
    return b_ret ;
  }
  if (FALSE == Get_Begin_Pline ((void*)ptrs_pl, &ptr_ob, &b_first_end,
				 &df_x_pl_beg, &df_y_pl_beg)             ||
      FALSE == Get_End_Pline   ((void*)ptrs_pl, &ptr_ob, &b_first_end,
		 df_x_pl_beg, df_y_pl_beg, &df_x_pl_end, &df_y_pl_end))
  {
    return b_ret ;
  }
  if (TRUE == Check_if_Equal (df_x_pl_beg, df_x_pl_end) &&
      TRUE == Check_if_Equal (df_y_pl_beg, df_y_pl_end))
   {
     b_ret = TRUE ;
   }
   return b_ret ;
}

BOOL Check_Pline (BLOK *ptrs_pl)
/*-----------------------------*/
{
  BOOL b_ret ;
  void *ptr_ob ;
  BOOL b_first_end ;
  long_long off, offk, ad ;
  double df_x_pl_beg, df_y_pl_beg ;
  double df_x1, df_y1, df_x2, df_y2 ;
  NAGLOWEK *nag ;
  LINIA * ptrs_line ;
  LUK * ptrs_arc ;
  SPLINE * ptrs_spline;
  int i_flags ;
  BLOK *bint;
  BOOL is_plate=FALSE;

    if (ptrs_pl->kod_obiektu== B_PLINE)
    {
        if ((ptrs_pl->opis_obiektu[0] == PL_PLATE) || (ptrs_pl->opis_obiektu[0] == PL_SHIELD))
            is_plate=TRUE;
    }

  b_ret = FALSE ;
  if (FALSE == check_head_pline (ptrs_pl))
  {
    return b_ret ;
  }

  if (FALSE == Get_Begin_Pline ((void*)ptrs_pl, &ptr_ob, &b_first_end,
				 &df_x_pl_beg, &df_y_pl_beg))
  {
    goto ret ;
  }
  off =  (char  *)ptrs_pl - dane ;
  offk = off  + ptrs_pl->n + sizeof (NAGLOWEK) - 1 ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case OdBLOK :
          bint=(BLOK*)nag;
          if (bint->kod_obiektu== B_PLINE)
          {
              if ((bint->opis_obiektu[0] == PL_PLATE) || (bint->opis_obiektu[0] == PL_SHIELD))
                  is_plate=TRUE;
          }
            break ;
      case Olinia :
	    ptrs_line = (LINIA*)nag ;
        if (!is_plate)
        {
            ptrs_line->obiektt2 = 0;  //don't do that anymore if  plate or shield
            ptrs_line->obiektt3 = 0;
        }

	if (TRUE == Check_if_Equal2 (ptrs_line->x1, df_x_pl_beg) &&
	    TRUE == Check_if_Equal2 (ptrs_line->y1, df_y_pl_beg))
	{
	  df_x_pl_beg = ptrs_line->x2 ;
	  df_y_pl_beg = ptrs_line->y2 ;
      //only if not plate or shield
	  if (!is_plate) ptrs_line->obiektt3 = RightDir ;
	}
	else
	if (TRUE == Check_if_Equal2 (ptrs_line->x2, df_x_pl_beg) &&
	    TRUE == Check_if_Equal2 (ptrs_line->y2, df_y_pl_beg))
	{
	  df_x_pl_beg = ptrs_line->x1 ;
	  df_y_pl_beg = ptrs_line->y1 ;
        if (!is_plate) ptrs_line->obiektt3 = O3LeftDir ;
	}
	else
	{
	  goto ret ;
	}
	break ;
	  case Ospline:
		  ptrs_spline = (SPLINE*)nag;
		  ptrs_spline->obiektt2 = 0;
		  ptrs_spline->obiektt3 = 0;

		  if (TRUE == Check_if_Equal2(ptrs_spline->xy[0], df_x_pl_beg) &&
			  TRUE == Check_if_Equal2(ptrs_spline->xy[1], df_y_pl_beg))
		  {
			  df_x_pl_beg = ptrs_spline->xy[ptrs_spline->lp - 2];  //6
			  df_y_pl_beg = ptrs_spline->xy[ptrs_spline->lp - 1];  //7
			  ptrs_spline->obiektt3 = RightDir;
		  }
		  else
			  if (TRUE == Check_if_Equal2(ptrs_spline->xy[ptrs_spline->lp - 2], df_x_pl_beg) &&
				  TRUE == Check_if_Equal2(ptrs_spline->xy[ptrs_spline->lp - 1], df_y_pl_beg))
			  {
				  df_x_pl_beg = ptrs_spline->xy[0];
				  df_y_pl_beg = ptrs_spline->xy[1];
				  ptrs_spline->obiektt3 = O3LeftDir;
			  }
			  else
			  {
				  goto ret;
			  }
		  break;
      case Otekst:
   break;
      case Oluk :
 	ptrs_arc = (LUK*)nag ;
	Arc_To_Points (ptrs_arc, &df_x1, &df_y1, &df_x2, &df_y2) ;
	if (TRUE == Check_if_Equal2 (df_x1, df_x_pl_beg) &&
	    TRUE == Check_if_Equal2 (df_y1, df_y_pl_beg))
	{
	  df_x_pl_beg = df_x2 ;
	  df_y_pl_beg = df_y2 ;
        if (!is_plate) ptrs_arc->obiektt3 = RightDir ;
	}
	else
	if (TRUE == Check_if_Equal2 (df_x2, df_x_pl_beg) &&
	    TRUE == Check_if_Equal2 (df_y2, df_y_pl_beg))
	{
	  df_x_pl_beg = df_x1 ;
	  df_y_pl_beg = df_y1 ;
        if (!is_plate) ptrs_arc->obiektt3 = O3LeftDir ;
	}
	else
	{
	  goto ret ;
	}
	break ;

	break ;
      default :
	goto ret ;
	break ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  b_ret = TRUE ;
ret :
  i_flags = (b_ret == TRUE ? O2BlockPline : O2NoBlockS) ;
  off =  (char  *)ptrs_pl - dane ;
  offk = off  + ptrs_pl->n + sizeof (NAGLOWEK) - 1 ;
  Set_Object_T2_Flag (i_flags, off, offk) ;
  return b_ret ;
}

BOOL Truncate_Begin_Pline (BLOK **ptrptrs_pl,
			   long_long l_off,
			   double df_x1, double df_y1,
			   double df_x2, double df_y2)
/*---------------------------------------------------*/
{
  long_long l_temp ;	/*wzgledne polozenie wskazanych obiektow w bloku*/
  double df_t1, df_t2 ;
  LINIA s_line1, s_line2, s_line3, *ptrs_ltemp, *ptrs_line ;
  LUK s_arc1, s_arc2, s_arc3, *ptrs_atemp,  *ptrs_arc ;
  char  *ptrh_beg,  *ptrh_end ;

  if ((*ptrptrs_pl)->n + sizeof(NAGLOWEK) < l_off)
  {
    Internal_Error (__LINE__,__FILE__);
    return FALSE ;
  }
  l_temp = sizeof(NAGLOWEK) + B3 + (*ptrptrs_pl)->dlugosc_opisu_obiektu ;
  ptrh_beg = (char *)(*ptrptrs_pl) + l_temp ;
  ptrh_end = (char *)(*ptrptrs_pl) + l_off - 1 ;
  zmien_atrybut (ptrh_beg, ptrh_end, ANieOkreslony, Ausuniety) ;
  usun_blok (ptrh_beg, ptrh_end) ;
  if (((NAGLOWEK*)((char *)(*ptrptrs_pl) + l_temp))->obiekt == Olinia)
  {
    ptrs_line = (LINIA*)((char *)(*ptrptrs_pl) + l_temp) ;
    Break_Line (ptrs_line, &s_line1, &s_line2, &s_line3, df_x1, df_y1,
	      df_x2, df_y2, &df_t1, &df_t2) ;
    ptrs_ltemp = (ptrs_line->obiektt3 == 0) ?  &s_line1 : &s_line2 ;
    if (TRUE == Check_if_GT (Dist_PP (ptrs_ltemp->x1, ptrs_ltemp->y1, ptrs_ltemp->x2, ptrs_ltemp->y2), 0))
    {
      if (NULL == Change_Object ((void *)ptrs_line, (void *)ptrs_ltemp, FALSE))
      {
	return FALSE ;
      }
    }
    else
    {
      Usun_Object ((void *)ptrs_line, FALSE) ;
    }
  }
  else
  {
    ptrs_arc = (LUK*)((char *)(*ptrptrs_pl) + l_temp) ;
    Break_Arc (ptrs_arc, &s_arc1, &s_arc2, &s_arc3,
	      df_x1, df_y1, df_x2, df_y2) ;
    ptrs_atemp = (ptrs_arc->obiektt3 == 0) ?  &s_arc1 : &s_arc2 ;
    if (TRUE != Check_if_Equal (ptrs_atemp->kat1, ptrs_atemp->kat2) &&
	TRUE != Check_if_Equal (ptrs_atemp->r, 0))
    {
      if (NULL == Change_Object ((void *)ptrs_arc, (void *)ptrs_atemp, FALSE))
      {
	return FALSE ;
      }
    }
    else
    {
      Usun_Object ((void *)ptrs_arc, FALSE) ;
    }
  }
  return TRUE ;
}

BOOL Truncate_End_Pline (BLOK **ptrptrs_pl,
			   long_long l_off,
			   double df_x1, double df_y1,
			   double df_x2, double df_y2)
/*---------------------------------------------------*/
{
  double df_t1, df_t2 ;
  LINIA s_line1, s_line2, s_line3, *ptrs_ltemp, *ptrs_line ;
  LUK s_arc1, s_arc2, s_arc3, *ptrs_atemp,  *ptrs_arc ;
  char  *ptrh_beg,  *ptrh_end ;

  if ((*ptrptrs_pl)->n + sizeof(NAGLOWEK) < l_off)
  {
    Internal_Error (__LINE__,__FILE__);
    return FALSE ;
  }
  ptrh_beg =  (char *)(*ptrptrs_pl) + l_off + sizeof(NAGLOWEK) + ((NAGLOWEK*)((char *)(*ptrptrs_pl) + l_off))->n ;
  ptrh_end =  (char *)(*ptrptrs_pl) + sizeof(NAGLOWEK) + (*ptrptrs_pl)->n - 1 ;
  zmien_atrybut (ptrh_beg, ptrh_end, ANieOkreslony, Ausuniety) ;
  usun_blok (ptrh_beg, ptrh_end) ;
  if (((NAGLOWEK*)((char *)(*ptrptrs_pl) + l_off))->obiekt == Olinia)
  {
    ptrs_line = (LINIA*)((char *)(*ptrptrs_pl) + l_off) ;
    Break_Line (ptrs_line, &s_line1, &s_line2, &s_line2, df_x1, df_y1, df_x2, df_y2, &df_t1, &df_t2) ;
    ptrs_ltemp = (ptrs_line->obiektt3 == 1) ?  &s_line1 : &s_line2 ;
    if (TRUE == Check_if_GT (Dist_PP (ptrs_ltemp->x1, ptrs_ltemp->y1, ptrs_ltemp->x2, ptrs_ltemp->y2), 0))
    {
      if (NULL == Change_Object ((void *)ptrs_line, (void *)ptrs_ltemp, FALSE))
      {
	return FALSE ;
      }
    }
    else
    {
      Usun_Object ((void *)ptrs_line, FALSE) ;
    }
  }
  else
  {
    ptrs_arc = (LUK*)((char *)(*ptrptrs_pl) + l_off) ;
    Break_Arc (ptrs_arc, &s_arc1, &s_arc2, &s_arc3, df_x1, df_y1, df_x2, df_y2) ;
    ptrs_atemp = (ptrs_arc->obiektt3 == 1) ?  &s_arc1 : &s_arc2 ;
    if (TRUE != Check_if_Equal (ptrs_atemp->kat1, ptrs_atemp->kat2) &&
	TRUE != Check_if_Equal (ptrs_atemp->r, 0))
    {
      if (NULL == Change_Object ((void *)ptrs_arc, (void *)ptrs_atemp, FALSE))
      {
	return FALSE ;
      }
    }
    else
    {
      Usun_Object ((void *)ptrs_arc, FALSE) ;
    }
  }
  return TRUE ;
}

BLOK *Pline_Cat (BLOK *ptrs_pl_dest,
		BLOK **ptrptrs_pl_src)  /*ptrptrs_pl_src - 'MOVEABLE' adres globalny*/
/*------------------------------------*/
{
  BLOK *ptrs_add ;
  long l_len ;
  long_long l_off ;

  if (ptrs_pl_dest == NULL || *ptrptrs_pl_src == NULL)
  {
    return NULL ;
  }
  l_len = sizeof(NAGLOWEK) + (*ptrptrs_pl_src)->n ;
  if (NULL == (ptrs_add = (BLOK*)Add_Object (&ptrs_pl_dest, (void *)(*ptrptrs_pl_src))))
  {
    return NULL ;
  }
  movmem_ ((void*)(*ptrptrs_pl_src), (void*)ptrs_add, l_len) ;
  ptrs_add->n = B3 + ptrs_add->dlugosc_opisu_obiektu ;
  ptrs_add->atrybut = Abad ;
  l_off = (char *)ptrs_pl_dest - dane ;
  Usun_Object ((void *)ptrs_add, FALSE) ;
  ptrs_pl_dest = (BLOK*)(dane + l_off) ;
  return ptrs_pl_dest ;
}

void * Get_Pline_Next_Ob (void* ptr_pl, void *ptr_ob, BOOL b_close)
/*--------------------------------------------------------------*/
{
  BLOK * ptrs_block ;
  long_long l_off, l_offk ;
  void  *ptr_next ;

  ptr_next = NULL ;
  ptrs_block = (BLOK*)ptr_pl ;
  if (FALSE == check_head_pline (ptrs_block))
  {
    return NULL ;
  }
  l_off =  (char  *)ptr_ob - dane ;
  l_off += sizeof (NAGLOWEK) + ((NAGLOWEK*)ptr_ob)->n;
  l_offk =  (char  *)ptrs_block - dane ;
  l_offk += sizeof (NAGLOWEK) + ptrs_block->n;
  if (l_off < l_offk)
  {
    ptr_next = (void*)(dane + l_off) ;
  }
  else
  if (b_close == TRUE)
  {
    ptr_next = Get_Pline_First_Ob (ptr_pl) ;
  }
  return ptr_next ;
}

void * Get_Pline_Previous_Ob (void* ptr_pl, void *ptr_ob, BOOL b_close)
/*--------------------------------------------------------------------*/
{
  BLOK * ptrs_block ;
  long_long l_off, l_offk, l_ob ;
  NAGLOWEK *nag ;
  void  *ptr_previous ;

  ptr_previous = NULL ;
  ptrs_block = (BLOK*)ptr_pl ;
  if (FALSE == check_head_pline (ptrs_block))
  {
    return NULL ;
  }
  l_off =  (char  *)ptrs_block - dane ;
  l_offk =  (char  *)ptr_ob - dane - 1 ;
  while (NULL != (nag = (NAGLOWEK*)Object_Tok (&l_off, l_offk, &l_ob, ONieOkreslony, TOK_LAYER_ON)))
  {
    switch (nag->obiekt)
    {
      case Olinia :
      case Oluk :
	ptr_previous = (void *)nag ;
	break ;
      default :
	break ;
    }
  }
  if (ptr_previous == NULL && b_close == TRUE)
  {
    ptr_previous = Get_Pline_Last_Ob (ptr_pl) ;
  }
  return ptr_previous ;
}


/*------------------------------------------------------------------------*/


void Break_Line (LINIA *ptrs_line_s,
		LINIA *ptrs_line_d1,
		LINIA *ptrs_line_d2,
        LINIA *ptrs_line_d3,
		double df_x1,
		double df_y1,
		double df_x2,
		double df_y2,
		double *ptrdf_t1,
		double *ptrdf_t2)
/*-----------------------------*/
{
  double df_t1, df_t2, df_tempx, df_tempy ;

  *ptrdf_t1 = -1 ;
  *ptrdf_t2 = -1 ;
  memcpy ((void *)ptrs_line_d1, (void *)ptrs_line_s, sizeof (LINIA));
  memcpy ((void *)ptrs_line_d2, (void *)ptrs_line_s, sizeof (LINIA));
  memcpy ((void *)ptrs_line_d3, (void *)ptrs_line_s, sizeof (LINIA));
  if (TRUE == Check_if_Equal (ptrs_line_s->x2, ptrs_line_s->x1) &&
      TRUE == Check_if_Equal (ptrs_line_s->y2, ptrs_line_s->y1))
  {
    return ;
  }
  poczL = 1 ;	/*zmienna globalna*/
  LiniaG.x1 = df_x1 ;
  LiniaG.y1 = df_y1 ;
  prostopadleL_(&df_x1, &df_y1, (void *)ptrs_line_s) ;
  if (fabs (ptrs_line_s->x2 - ptrs_line_s->x1) >
      fabs (ptrs_line_s->y2 - ptrs_line_s->y1))
  {
    df_t1 = (df_x1 - ptrs_line_s->x1) / (ptrs_line_s->x2 - ptrs_line_s->x1) ;
  }
  else
  {
    df_t1 = (df_y1 - ptrs_line_s->y1) / (ptrs_line_s->y2 - ptrs_line_s->y1) ;
  }
  if (df_t1 < 0)
  {
    df_x1 = ptrs_line_s->x1 ;
    df_y1 = ptrs_line_s->y1 ;
  }
  if (df_t1 > 1)
  {
    df_x1 = ptrs_line_s->x2 ;
    df_y1 = ptrs_line_s->y2 ;
  }
  LiniaG.x1 = df_x2 ;
  LiniaG.y1 = df_y2 ;
  prostopadleL_ (&df_x2, &df_y2, (void *)ptrs_line_s) ;
  if (fabs (ptrs_line_s->x2 - ptrs_line_s->x1) >
      fabs (ptrs_line_s->y2 - ptrs_line_s->y1))
  {
    df_t2 = (df_x2 - ptrs_line_s->x1) / (ptrs_line_s->x2 - ptrs_line_s->x1) ;
  }
  else
  {
    df_t2 = (df_y2 - ptrs_line_s->y1) / (ptrs_line_s->y2 - ptrs_line_s->y1) ;
  }
  if (df_t2 < 0)
  {
    df_x2 = ptrs_line_s->x1 ;
    df_y2 = ptrs_line_s->y1 ;
  }
  if (df_t2 > 1)
  {
    df_x2 = ptrs_line_s->x2 ;
    df_y2 = ptrs_line_s->y2 ;
  }
  if (df_t1 > df_t2)
  {
    df_tempx = df_x1 ;
    df_tempy = df_y1 ;
    df_x1 = df_x2 ;
    df_y1 = df_y2 ;
    df_x2 = df_tempx ;
    df_y2 = df_tempy ;
  }
  ptrs_line_d1->x2 = df_x1 ;
  ptrs_line_d1->y2 = df_y1 ;
  ptrs_line_d2->x1 = df_x2 ;
  ptrs_line_d2->y1 = df_y2 ;

  //cut part of the line
  ptrs_line_d3->x1 =  df_x1;
  ptrs_line_d3->y1 =  df_y1;
  ptrs_line_d3->x2 =  df_x2;
  ptrs_line_d3->y2 =  df_y2;

  *ptrdf_t1 = df_t1 ;
  *ptrdf_t2 = df_t2 ;
}

void Break_Arc (LUK *ptrs_arc_s,
		LUK *ptrs_arc_d1,
		LUK *ptrs_arc_d2,
        LUK *ptrs_arc_d3,
		double df_x1,
		double df_y1,
		double df_x2,
		double df_y2)
/*--------------------------*/
{
  OKRAG s_o = Odef ;
  double df_k1, df_k2 ;

  memcpy ((void *)ptrs_arc_d1, (void *)ptrs_arc_s, sizeof (LUK));
  memcpy ((void *)ptrs_arc_d2, (void *)ptrs_arc_s, sizeof (LUK));
  memcpy ((void *)ptrs_arc_d3, (void *)ptrs_arc_s, sizeof (LUK));
  if (TRUE == Check_if_Equal (ptrs_arc_s->kat1, ptrs_arc_s->kat2) ||
      TRUE == Check_if_Equal (ptrs_arc_s->r, 0))
  {
    return ;
  }
  s_o.r = ptrs_arc_s->r ;
  s_o.x = ptrs_arc_s->x ;
  s_o.y = ptrs_arc_s->y ;
  najblizszyO (&df_x1, &df_y1, (void *)&s_o) ;
  df_k1 = Atan2 (df_y1 - ptrs_arc_s->y, df_x1 - ptrs_arc_s->x) ;
  najblizszyO (&df_x2, &df_y2, (void *)&s_o);
  df_k2 = Atan2 (df_y2 - ptrs_arc_s->y, df_x2 - ptrs_arc_s->x) ;
  df_k1 = check_angle (df_k1, ptrs_arc_s) ;
  df_k2 = check_angle (df_k2, ptrs_arc_s) ;
  df_k1 = Angle_Normal (df_k1) ;
  df_k2 = Angle_Normal (df_k2) ;
  ptrs_arc_d1->kat2 = df_k1 ;
  ptrs_arc_d2->kat1 = df_k2 ;
  if (1 == Cmp_Arc_Angle (df_k1, df_k2,
		   Angle_Normal (ptrs_arc_s->kat1),
		   Angle_Normal (ptrs_arc_s->kat2)))
  {
    ptrs_arc_d1->kat2 = df_k2 ;
    ptrs_arc_d2->kat1 = df_k1 ;
  }

  //cut part
  ptrs_arc_d3->kat1 = ptrs_arc_d1->kat2 ;
  ptrs_arc_d3->kat2 = ptrs_arc_d2->kat1 ;

  ptrs_arc_d1->kat1 = Angle_Normal (ptrs_arc_d1->kat1) ;
  ptrs_arc_d1->kat2 = Angle_Normal (ptrs_arc_d1->kat2) ;
  ptrs_arc_d2->kat1 = Angle_Normal (ptrs_arc_d2->kat1) ;
  ptrs_arc_d2->kat2 = Angle_Normal (ptrs_arc_d2->kat2) ;

  ptrs_arc_d3->kat1 = Angle_Normal (ptrs_arc_d3->kat1) ;
  ptrs_arc_d3->kat2 = Angle_Normal (ptrs_arc_d3->kat2) ;
}

void Break_Elliptical_Arc (ELLIPTICALARC *ptrs_ellipticalarc_s,
                ELLIPTICALARC *ptrs_ellipticalarc_d1,
                ELLIPTICALARC *ptrs_ellipticalarc_d2,
                ELLIPTICALARC *ptrs_ellipticalarc_d3,
                double df_x1,
                double df_y1,
                double df_x2,
                double df_y2)
/*--------------------------*/
{
    ELLIPSE s_e = Edef ;
    double df_k1, df_k2 ;
    int numpoints;
    double xy[256];
    double *df_k1_, *df_k2_;

    memcpy ((void *)ptrs_ellipticalarc_d1, (void *)ptrs_ellipticalarc_s, sizeof (ELLIPTICALARC));
    memcpy ((void *)ptrs_ellipticalarc_d2, (void *)ptrs_ellipticalarc_s, sizeof (ELLIPTICALARC));
    memcpy ((void *)ptrs_ellipticalarc_d3, (void *)ptrs_ellipticalarc_s, sizeof (ELLIPTICALARC));
    if (TRUE == Check_if_Equal (ptrs_ellipticalarc_s->kat1, ptrs_ellipticalarc_s->kat2) ||
        TRUE == Check_if_Equal (ptrs_ellipticalarc_s->rx, 0) || TRUE == Check_if_Equal (ptrs_ellipticalarc_s->ry, 0))
    {
        return ;
    }
    s_e.rx = ptrs_ellipticalarc_s->rx ;
    s_e.ry = ptrs_ellipticalarc_s->ry ;
    s_e.x = ptrs_ellipticalarc_s->x ;
    s_e.y = ptrs_ellipticalarc_s->y ;
    s_e.angle = ptrs_ellipticalarc_s->angle ;
    najblizszyE (&df_x1, &df_y1, (void *)&s_e) ;
    df_k1 = Atan2 (df_y1 - ptrs_ellipticalarc_s->y, df_x1 - ptrs_ellipticalarc_s->x) ;
    df_k1-=s_e.angle;
    df_k1=Angle_Normal (df_k1);
    najblizszyE (&df_x2, &df_y2, (void *)&s_e);
    df_k2 = Atan2 (df_y2 - ptrs_ellipticalarc_s->y, df_x2 - ptrs_ellipticalarc_s->x) ;
    df_k2-=s_e.angle;
    df_k2=Angle_Normal (df_k2);
    df_k1 = check_elliptical_angle (df_k1, ptrs_ellipticalarc_s) ;
    df_k2 = check_elliptical_angle (df_k2, ptrs_ellipticalarc_s) ;

    df_k1 = Angle_Normal (df_k1) ;
    df_k2 = Angle_Normal (df_k2) ;
    ptrs_ellipticalarc_d1->kat2 = df_k1 ;
    ptrs_ellipticalarc_d2->kat1 = df_k2 ;
    if (1 == Cmp_Arc_Angle (df_k1, df_k2,
                            Angle_Normal (ptrs_ellipticalarc_s->kat1),
                            Angle_Normal (ptrs_ellipticalarc_s->kat2)))
    {
        ptrs_ellipticalarc_d1->kat2 = df_k2 ;
        ptrs_ellipticalarc_d2->kat1 = df_k1 ;
    }

    //cut part
    ptrs_ellipticalarc_d3->kat1 = ptrs_ellipticalarc_d1->kat2 ;
    ptrs_ellipticalarc_d3->kat2 = ptrs_ellipticalarc_d2->kat1 ;

    ptrs_ellipticalarc_d1->kat1 = Angle_Normal (ptrs_ellipticalarc_d1->kat1) ;
    ptrs_ellipticalarc_d1->kat2 = Angle_Normal (ptrs_ellipticalarc_d1->kat2) ;
    ptrs_ellipticalarc_d2->kat1 = Angle_Normal (ptrs_ellipticalarc_d2->kat1) ;
    ptrs_ellipticalarc_d2->kat2 = Angle_Normal (ptrs_ellipticalarc_d2->kat2) ;

    ptrs_ellipticalarc_d3->kat1 = Angle_Normal (ptrs_ellipticalarc_d3->kat1) ;
    ptrs_ellipticalarc_d3->kat2 = Angle_Normal (ptrs_ellipticalarc_d3->kat2) ;
}



BOOL Check_if_Vertex_Inside_Pline (void *ptr_ob, double df_x, double df_y)
/*---------------------------------------------------------------------*/
{
  BLOK * ptrs_pl ;
  BOOL b_ret, b_first_end ;
  void *ptr_ob1 ;
  double df_x_beg, df_y_beg, df_x_end, df_y_end ;

  b_ret = FALSE ;

  if (O2BlockPline == ((NAGLOWEK*)ptr_ob)->obiektt2   &&
      NULL != (ptrs_pl = LASTB((char  *)ptr_ob)))
  {
    if (TRUE == Check_if_Close_Pline (ptrs_pl) ||
	TRUE == Check_if_Inside_Pline (ptrs_pl, ptr_ob))
    {
      return TRUE ;
    }
    if (FALSE == Get_Begin_Pline (ptrs_pl,
		      &ptr_ob1,
		      &b_first_end,
		      &df_x_beg,
		      &df_y_beg) ||
	(TRUE == Check_if_Equal (df_x_beg, df_x) &&
	TRUE == Check_if_Equal (df_y_beg, df_y)))
    {
      return b_ret ;
    }
    if (FALSE == Get_End_Pline (ptrs_pl,
		      &ptr_ob1,
		      &b_first_end,
		      df_x_beg,
		      df_y_beg,
		    &df_x_end,
		    &df_y_end) ||
	(TRUE == Check_if_Equal (df_x_end, df_x) &&
	TRUE == Check_if_Equal (df_y_end, df_y)))
    {
      return b_ret ;
    }
    return TRUE ;
  }
  return b_ret ;
}

static double check_angle (double k, LUK *l)
/*-----------------------------------------*/
{
  double x, y, x0, y0;


  if (TRUE == Check_Angle (k, l->kat1, l->kat2))
  {
    return k;
  }
  x0 = l->x + l->r * cos (k) ;
  y0 = l->y + l->r * sin (k) ;
  k = l->kat2;
  if (TRUE == End_Arc ((double *)&x, (double *)&y, x0, y0, l))
  {
    k = l->kat1;
  }
  return k;
}


static double check_elliptical_angle (double k, ELLIPTICALARC *el)
/*--------------------------------------------------------------*/
{
    double x, y, x0, y0;
    ELLIPSE e=Edef;
    double rxy;


    if (el->ry>el->rx) rxy=el->ry;
    else rxy=el->rx;
    if (TRUE == Check_Angle (k, el->kat1, el->kat2))
    {
        return k;
    }

    x0 = el->x + rxy * cos (k) ;
    y0 = el->y + rxy * sin (k) ;

    e.x=el->x;
    e.y=el->y;
    e.rx=el->rx;
    e.ry=el->ry;
    e.angle=el->angle;

    najblizszyE(&x0, &y0, &e);

    k = el->kat2;
    if (TRUE == End_Elliptical_Arc ((double *)&x, (double *)&y, x0, y0, el))
    {
        k = el->kat1;
    }

    return k;
}

static BOOL check_head_pline (BLOK *ptrs_block)
/*------------------------------------------*/
{
  BOOL b_ret ;

  b_ret = FALSE ;
  if (NULL != ptrs_block &&
      ptrs_block->obiekt == OdBLOK &&
      ((ptrs_block->kod_obiektu == B_PLINE) || (ptrs_block->kod_obiektu == 'E')) &&
      (ptrs_block->opis_obiektu [0] == PL_TRACE ||
       ptrs_block->opis_obiektu [0] == PL_PLINE ||
	   ptrs_block->opis_obiektu[0] ==  PL_SHADOW ||
	   ptrs_block->opis_obiektu [0] == PL_CURVE ||
       ptrs_block->opis_obiektu [0] == PL_POLYGON||
       ptrs_block->opis_obiektu [0] == PL_ELLIPSE||
	   ptrs_block->opis_obiektu [0] == PL_RECTANGLE||
       ptrs_block->opis_obiektu [0] == PL_SKETCH||
       ptrs_block->opis_obiektu [0] == PL_ELLIPSE_ARC||
       ptrs_block->opis_obiektu [0] == PL_PLATE ||
       ptrs_block->opis_obiektu [0] == PL_SIEC ||
       ptrs_block->opis_obiektu [0] == PL_HOLE ||
       ptrs_block->opis_obiektu [0] == PL_WALL ||
       ptrs_block->opis_obiektu [0] == PL_ZONE))

  {
    b_ret = TRUE ;
  }
  return b_ret ;
}

