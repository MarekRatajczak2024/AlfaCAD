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

#define __O_EXTEND__
#include <forwin.h>
#ifndef LINUX
#include <dos.h>
#endif
#define ALLEGWIN
#include <allegext.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <limits.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_pline.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_osnap.h"
#include "b_messag.h"

#include "leak_detector_c.h"

extern void Set_Block_Change_Mode (void *, int) ;
extern NODE bnodes[MAX_NODES];
extern double curviness;
extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
extern void set_bnodes(SPLINE* ptrs_spline, float *ad_xy, int* node_count_);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);
extern void get_control_points(NODE n1, NODE n2, float points[]);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern int Get_Ellipse_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double xy[]);
extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle, double df_eangle, double xy[]);
extern void Set_Ell_Verno (int i_ell_verno);
extern int Get_Ell_Verno (void);
extern BOOL End_EllipticalArc (double *x, double *y, double x0, double y0, ELLIPTICALARC *ea);
extern BOOL check_if_point_on_ellipse(double cx, double cy, double a, double b, double angle,double px, double py);
extern void Get_EllipticalArc_EndPoints (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle0, double df_eangle0, double *x1, double *y1, double *x2, double *y2);
extern BOOL ini_extend (int type);
extern void undo_extend (void);
extern BOOL change_date_ (BLOK *ptrs_b, void *ptr_ob, void *ptr_new, void *ptr_new1);
/*--------------------------------------------*/

/*---------Wydluz---------------------------------------------*/


static void *obiekt_wybrany1 (unsigned *typ)
/*---------------------------------------*/
{
  void *ptr_ob ;
  BLOK *ptrs_pl ;

   ptr_ob = select_w (typ, NULL) ;
   if (ptr_ob != NULL)
   {
      if (0 == Layers [((LINIA*)ptr_ob)->warstwa].edit)
      {
	ptr_ob = NULL ;
        komunikat (77);
      }
      else
      {
      if (O2BlockPline == ((NAGLOWEK*)ptr_ob)->obiektt2   &&
	  NULL != (ptrs_pl = LASTB((char  *)ptr_ob)) &&
	  (TRUE == Check_if_Close_Pline (ptrs_pl)         ||
	   TRUE == Check_if_Inside_Pline (ptrs_pl, ptr_ob)))
       {
	ptr_ob = NULL ;
	ErrList (81) ;
       }
      }
   }
   return ptr_ob ;
}

#define BIGNOD 1e200
#define BIGNOF 1e20
static void extend_line (void *ad, double x0, double y0)
/*------------------------------------------------------*/
{
  LINIA *ptrs_line, line_tmp, l_temp, line_tmp_w;
  NAGLOWEK *nag ;
  BLOK * ptrs_b ;
  WIELOKAT *ptrs_w;
  SPLINE *ptrs_s;
  ELLIPSE *ptrs_e;
  ELLIPTICALARC *ptrs_ea;
  double xe, ye, x, y, te, t1, t2;
  BOOL b_temp ;
  void  *ad_h,  *obbl_h ;
  float df_x1, df_y1;
  int i, j, k;

  SPLINE tmp_spline = Splinedef;
  double bnodes_2_tangent_org, bnodes_2_tangent;
  int node_count;

  ptrs_line = (LINIA *)ad ;
  ad_h = (void *)ad ;
  memcpy (&l_temp, ptrs_line, sizeof(LINIA)) ;
  koniecL_ (&x0, &y0, ad) ;
  b_temp = TRUE ;
  if (TRUE == Check_if_Equal (x0, ptrs_line->x1) &&
      TRUE == Check_if_Equal (y0, ptrs_line->y1))
  {
    l_temp.x1 = ptrs_line->x2 ;
    l_temp.y1 = ptrs_line->y2 ;
    l_temp.x2 = ptrs_line->x1 ;
    l_temp.y2 = ptrs_line->y1 ;
    b_temp = FALSE ;
  }
  te = BIGNOD ;
  obiekt_tok ((char*)ADP, ADK, (char **)&nag, ONieOkreslony);
  while (nag != NULL)
  {
    obbl_h  = (void  *)nag ;	/*pominiecie wydluzenia do samego siebie*/
    if (nag->atrybut == Ablok  && ad_h != obbl_h)
    {
       switch (nag->obiekt)
       {
	 case Olinia :
	    if (0 == przeciecieLL_tt (&x, &y, (void *)&l_temp, (void *)nag, &t1, &t2))
	    {
	      break ;  /*linie rownolegle*/
	    }
	    if (Check_if_GE (t2, 0) && Check_if_LE (t2, 1) )
	    {
	      if (TRUE == Check_if_Equal (t1, 1))
	      {
		return ;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
	      }
	      if (t1 > 1 &&  t1 < te)
	      {
		xe = x ;
		ye = y ;
		te = t1 ;
	      }
	    }
	    break ;
	case Owwielokat:
		memcpy(&line_tmp_w, &l_temp, sizeof(l_temp));
		ptrs_w = (WIELOKAT*)nag;

		df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
		df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

		for (i = 0; i < ptrs_w->lp; i += 2)
		{
			line_tmp_w.x1 = df_x1;
			line_tmp_w.y1 = df_y1;
			line_tmp_w.x2 = ptrs_w->xy[i];
			line_tmp_w.y2 = ptrs_w->xy[i + 1];

		    if (0 == przeciecieLL_tt(&x, &y, (void *)&l_temp, &line_tmp_w, &t1, &t2))
			{
				;  /*linie rownolegle*/
			}
			else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
			{
				if (TRUE == Check_if_Equal(t1, 1))
				{
					;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
				}
				if (t1 > 1 && t1 < te)
				{
					xe = x;
					ye = y;
					te = t1;
				}
			}

			df_x1 = line_tmp_w.x2;
			df_y1 = line_tmp_w.y2;
		}
		break;
	case Ospline:
	{
		int i;
		float out_x[MaxNpts], out_y[MaxNpts];

		memcpy(&line_tmp_w, &l_temp, sizeof(l_temp));
		ptrs_s = (SPLINE*)nag;

#define Npts 90  //for more precission

		if (ptrs_s->npts < 5)
		{

			calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, out_x, out_y);

			for (i = 0; i < Npts - 1; i++)
			{
				line_tmp_w.x1 = out_x[i];
				line_tmp_w.y1 = out_y[i];
				line_tmp_w.x2 = out_x[i + 1];
				line_tmp_w.y2 = out_y[i + 1];

				if (0 == przeciecieLL_tt(&x, &y, (void*)&l_temp, &line_tmp_w, &t1, &t2))
				{
					;  /*linie rownolegle*/
				}
				else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
				{
					if (TRUE == Check_if_Equal(t1, 1))
					{
						;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
					}
					if (t1 > 1 && t1 < te)
					{
						xe = x;
						ye = y;
						te = t1;
					}
				}

			}
		}
		else
		{
			curviness = ptrs_s->xy[ptrs_s->lp];

			tmp_spline.npts = 4;
			tmp_spline.lp = 8;
			tmp_spline.kolor = ptrs_s->kolor;
			tmp_spline.typ = ptrs_s->typ;

			node_count = 1;

			set_bnodes(ptrs_s, ptrs_s->xy, &node_count);

			calc_tangents(ptrs_s->closed, &node_count, &bnodes_2_tangent);

			if (ptrs_s->closed == 0)
			{
				bnodes_2_tangent_org = bnodes[2].tangent;
				bnodes[2].tangent = bnodes_2_tangent;
				get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
			
				for (i = 0; i < Npts - 1; i++)
				{
					line_tmp_w.x1 = out_x[i];
					line_tmp_w.y1 = out_y[i];
					line_tmp_w.x2 = out_x[i + 1];
					line_tmp_w.y2 = out_y[i + 1];

					if (0 == przeciecieLL_tt(&x, &y, (void*)&l_temp, &line_tmp_w, &t1, &t2))
					{
						;  /*linie rownolegle*/
					}
					else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
					{
						if (TRUE == Check_if_Equal(t1, 1))
						{
							;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
						}
						if (t1 > 1 && t1 < te)
						{
							xe = x;
							ye = y;
							te = t1;

						}
					}

				}

				bnodes[2].tangent = bnodes_2_tangent_org;
			}

			for (k = 2; k < node_count - 2; k++)  //1 - extra_node
			{
				get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

				for (i = 0; i < Npts - 1; i++)
				{
					line_tmp_w.x1 = out_x[i];
					line_tmp_w.y1 = out_y[i];
					line_tmp_w.x2 = out_x[i + 1];
					line_tmp_w.y2 = out_y[i + 1];

					if (0 == przeciecieLL_tt(&x, &y, (void*)&l_temp, &line_tmp_w, &t1, &t2))
					{
						;  /*linie rownolegle*/
					}
					else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
					{
						if (TRUE == Check_if_Equal(t1, 1))
						{
							;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
						}
						if (t1 > 1 && t1 < te)
						{
							xe = x;
							ye = y;
							te = t1;

						}
					}

				}
			}

			if ((node_count > 3) && (ptrs_s->closed == 0))
			{
				get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

				for (i = 0; i < Npts - 1; i++)
				{
					line_tmp_w.x1 = out_x[i];
					line_tmp_w.y1 = out_y[i];
					line_tmp_w.x2 = out_x[i + 1];
					line_tmp_w.y2 = out_y[i + 1];

					if (0 == przeciecieLL_tt(&x, &y, (void*)&l_temp, &line_tmp_w, &t1, &t2))
					{
						;  /*linie rownolegle*/
					}
					else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
					{
						if (TRUE == Check_if_Equal(t1, 1))
						{
							;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
						}
						if (t1 > 1 && t1 < te)
						{
							xe = x;
							ye = y;
							te = t1;

						}
					}

				}
			}

		}
		break;
	}
       case Oellipse:
       case Ofilledellipse:
       {  int i;
           int numpoints;
           double xy[512];
           int verno;

           memcpy(&line_tmp_w, &l_temp, sizeof(l_temp));
           ptrs_e = (ELLIPSE*)nag;

           verno = Get_Ell_Verno () ;
           Set_Ell_Verno (250) ;
           numpoints=Get_Ellipse_Points(ptrs_e->x, ptrs_e->y, ptrs_e->rx, ptrs_e->ry, ptrs_e->angle, xy);
           Set_Ell_Verno (verno) ;

           xy[numpoints] = xy[0];
           xy[numpoints+1] = xy[1];
           numpoints+=2;

           for (i = 0; i < (numpoints-2) - 1; i+=2)
           {
               line_tmp_w.x1 = xy[i];
               line_tmp_w.y1 = xy[i+1];
               line_tmp_w.x2 = xy[i + 2];
               line_tmp_w.y2 = xy[i + 3];

               if (0 == przeciecieLL_tt(&x, &y, (void*)&l_temp, &line_tmp_w, &t1, &t2))
               {
                   ;  /*linie rownolegle*/
               }
               else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
               {
                   if (TRUE == Check_if_Equal(t1, 1))
                   {
                       ;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
                   }
                   if (t1 > 1 && t1 < te)
                   {
                       xe = x;
                       ye = y;
                       te = t1;
                   }
               }
           }
           break;
       }
       case Oellipticalarc:
       {  int i;
           int numpoints;
           double xy[512];
           int verno;

           memcpy(&line_tmp_w, &l_temp, sizeof(l_temp));
           ptrs_ea = (ELLIPTICALARC *)nag;

           verno = Get_Ell_Verno () ;
           Set_Ell_Verno (250) ;
           numpoints=Get_EllipticalArc_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, ptrs_ea->kat1, ptrs_ea->kat2,xy);
           Set_Ell_Verno (verno) ;

           for (i = 0; i < (numpoints-2) - 1; i+=2)
           {
               line_tmp_w.x1 = xy[i];
               line_tmp_w.y1 = xy[i+1];
               line_tmp_w.x2 = xy[i + 2];
               line_tmp_w.y2 = xy[i + 3];

               if (0 == przeciecieLL_tt(&x, &y, (void*)&l_temp, &line_tmp_w, &t1, &t2))
               {
                   ;  /*linie rownolegle*/
               }
               else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
               {
                   if (TRUE == Check_if_Equal(t1, 1))
                   {
                       ;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
                   }
                   if (t1 > 1 && t1 < te)
                   {
                       xe = x;
                       ye = y;
                       te = t1;
                   }
               }
           }
           break;
       }
	 case Ookrag :
	 case Okolo :
	 case Oluk :
	 {
	    double x1, x2, y1, y2, t1, t2, t0, x0, y0 ;
	    int l;

	    if (nag->obiekt == Oluk)
	    {
	      l = przeciecieLl_tt (&x1, &y1, &t1, &x2, &y2, &t2,
		    (void *)&l_temp, (void *)nag) ;  /*t2>=t1*/
	    }
	    else
	    {
	      l = przeciecieLO_tt (&x1, &y1, &t1, &x2, &y2, &t2,
		    (void *)&l_temp, (void *)nag) ;  /*t2>=t1*/
	    }
	    if (0 == l)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    if (TRUE == Check_if_Equal (t1, 1) ||
		TRUE == Check_if_Equal (t2, 1) )
	    {
	      return ; /*koniec linii lezy na brzegu okregu*/
	    }
	    if (t1 > 1)
	    {
	      x0 = x1 ;
	      y0 = y1 ;
	      t0 = t1 ;
	    }
	    else
	    if (t1 < 1  && t2 > 1)
	    {
	      x0 = x2 ;
	      y0 = y2 ;
	      t0 = t2 ;
	    }
	    else
	    {
	      break ;
	    }
	    if (t0 < te)
	    {
	      xe = x0 ;
	      ye = y0 ;
	      te = t0 ;
	    }
	    break ;
	 }
	 default :
	    break ;
       }
    }
    obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony);
  }
  if ( te < BIGNOF)
  {
    if (TRUE == Check_if_Vertex_Inside_Pline (ptrs_line,
			(b_temp == TRUE ? ptrs_line->x2 : ptrs_line->x1),
			(b_temp == TRUE ? ptrs_line->y2 : ptrs_line->y1)))
    {
      ErrList (81) ;
      return ;
    }
    Set_Block_Change_Mode ((void *)ad, OB1CHANGE_BREAK) ;
    ptrs_b = NULL ;
    memcpy (&line_tmp, ptrs_line, sizeof (line_tmp)) ;

    /* rysuj_obiekt (ad, COPY_PUT, 0) ; */
    if (b_temp == TRUE)
    {
        line_tmp.x2 = xe ;
        line_tmp.y2 = ye ;
    }
    else
    {
        line_tmp.x1 = xe ;
        line_tmp.y1 = ye ;
    }

    change_date_ (ptrs_b, (void *)ptrs_line, (void *)&line_tmp, NULL);

    Change = TRUE;
    Set_Auto_Backup (TRUE);
  }
}

static BOOL przecieciell_ex (void *adr, void *adr1,
		double *x0, double *y0, BOOL b_first_end)
/*-------------------------------------------------------*/
{
  LUK *luk, *arc_to;
  OKRAG o ;
  BOOL b_p1 = FALSE, b_p2 = FALSE ;
  double x1, x2, y1, y2 ;
  double kat1, kat2, angle1_to, angle2_to ;

  luk=(LUK*)adr1 ;
  arc_to = (LUK*)adr ;
  o.r = luk->r ;
  o.x = luk->x ;
  o.y = luk->y ;
  if (0 == przeciecieOO_ex (&x1, &y1, &x2, &y2, adr, (void*)&o))
  {
    return FALSE ;
  }
  kat1 = Atan2 (y1 - luk->y, x1 - luk->x) ;
  angle1_to = Atan2 (y1 - arc_to->y, x1 - arc_to->x) ;
  if (TRUE == Check_Angle (kat1, luk->kat2, luk->kat1) &&
      TRUE == Check_Angle (angle1_to, arc_to->kat1, arc_to->kat2))
  {
    b_p1 = TRUE ;
  }
  kat2 = Atan2 (y2 - luk->y, x2 - luk->x) ;
  angle2_to = Atan2 (y2 - arc_to->y, x2 - arc_to->x) ;
  if (TRUE == Check_Angle (kat2, luk->kat2, luk->kat1) &&
      TRUE == Check_Angle (angle2_to, arc_to->kat1, arc_to->kat2))
  {
    b_p2 = TRUE ;
  }
  if (b_p1 == FALSE && b_p2 == FALSE)
  {
    return FALSE ;
  }
  if (b_p1 == TRUE && b_p2 == FALSE)
  {
    *x0 = x1 ;
    *y0 = y1 ;
    return TRUE ;
  }
  else
  if (b_p1 == FALSE && b_p2 == TRUE)
  {
    *x0 = x2 ;
    *y0 = y2 ;
    return TRUE ;
  }
  else
  if (b_p1 == TRUE && b_p2 == TRUE)
  {
    if (TRUE == b_first_end)
    {
       *x0 = x1 ;
       *y0 = y1 ;
       if (TRUE == Check_Angle (kat2, kat1, luk->kat1))
       {
	 *x0 = x2 ;
	 *y0 = y2 ;
       }
    }
    else
    {
       *x0 = x1 ;
       *y0 = y1 ;
       if (TRUE == Check_Angle (kat2, luk->kat2, kat1))
       {
	 *x0 = x2 ;
	 *y0 = y2 ;
       }
    }
    return TRUE ;
  }
  return FALSE ;  /*sytuacja teoretycznie niemozliwa*/
}


static BOOL przeciecieOl_ex (void *adr, void *adr1,
		double *x0, double *y0, BOOL b_first_end)
/*-------------------------------------------------------*/
{
  LUK *luk;
  OKRAG o ;
  BOOL b_p1 = FALSE, b_p2 = FALSE ;
  double x1, x2, y1, y2 ;
  double kat1, kat2 ;

  luk=(LUK*)adr1;
  o.r = luk->r ;
  o.x = luk->x ;
  o.y = luk->y ;
  if (0 == przeciecieOO_ex (&x1, &y1, &x2, &y2, adr, (void*)&o))
  {
    return FALSE ;
  }
  kat1 = Atan2 (y1 - luk->y, x1 - luk->x) ;
  if (TRUE == Check_Angle (kat1, luk->kat2, luk->kat1))
  {
    b_p1 = TRUE ;
  }
  kat2 = Atan2 (y2 - luk->y, x2 - luk->x) ;
  if (TRUE == Check_Angle (kat2, luk->kat2, luk->kat1))
  {
    b_p2 = TRUE ;
  }
  if (b_p1 == FALSE && b_p2 == FALSE)
  {
    return FALSE ;
  }
  if (b_p1 == TRUE && b_p2 == FALSE)
  {
    *x0 = x1 ;
    *y0 = y1 ;
    return TRUE ;
  }
  else
  if (b_p1 == FALSE && b_p2 == TRUE)
  {
    *x0 = x2 ;
    *y0 = y2 ;
    return TRUE ;
  }
  else
  if (b_p1 == TRUE && b_p2 == TRUE)
  {
    if (TRUE == b_first_end)
    {
       *x0 = x1 ;
       *y0 = y1 ;
       if (TRUE == Check_Angle (kat2, kat1, luk->kat1))
       {
	 *x0 = x2 ;
	 *y0 = y2 ;
       }
    }
    else
    {
       *x0 = x1 ;
       *y0 = y1 ;
       if (TRUE == Check_Angle (kat2, luk->kat2, kat1))
       {
	 *x0 = x2 ;
	 *y0 = y2 ;
       }
    }
    return TRUE ;
  }
  return FALSE ;  /*sytuacja teoretycznie niemozliwa*/
}



static BOOL przeciecieLl_ex (void *adr, void *adr1, double *x0, double *y0)
/*------------------------------------------------------------------------*/
{
  LUK *luk;
  OKRAG o ;
  BOOL b_p1 = FALSE, b_p2 = FALSE ;
  double x1, x2, y1, y2, t1, t2, xend , yend ;
  double kat;

  xend = *x0 ;  /*w x0, y0 przekazane jest wybrany koniec luku*/
  yend = *y0 ;
  luk=(LUK*)adr1;
  o.r = luk->r ;
  o.x = luk->x ;
  o.y = luk->y ;
  if (0 == przeciecieLO_tt (&x1, &y1, &t1, &x2, &y2, &t2, adr, (void*)&o))
  {
    return FALSE ;
  }

  kat = Atan2 (y1 - luk->y, x1 - luk->x) ;
  if (TRUE == Check_if_GE (t1, 0) &&
      TRUE == Check_if_LE (t1, 1) &&
      TRUE == Check_Angle (kat, luk->kat2, luk->kat1))
  {
    b_p1 = TRUE ;
  }
  kat = Atan2 (y2 - luk->y, x2 - luk->x) ;
  if (TRUE == Check_if_GE (t2, 0) &&
      TRUE == Check_if_LE (t2, 1) &&
      TRUE == Check_Angle (kat, luk->kat2, luk->kat1))
  {
    b_p2 = TRUE ;
  }
  if (b_p1 == FALSE && b_p2 == FALSE)
  {
    return FALSE ;
  }
  *x0 = x1 ;
  *y0 = y1 ;
  if ( (b_p1 == FALSE && b_p2 == TRUE )  ||
       (b_p1 == TRUE && b_p2 == TRUE &&
      Dist_PP (x1, y1, xend, yend) > Dist_PP (x2, y2, xend, yend)))
  {
    *x0 = x2 ;
    *y0 = y2 ;
  }
  return TRUE ;
}

static void extend_arc_temp (LUK *arc, double x0, double y0, double *xe,
			double *ye, BOOL b_first_ex, BOOL b_first_end)
/*-----------------------------------------------------------*/
{
  double k0, ke ;
  BOOL b_check ;

  if (b_first_ex == TRUE)
  {
    *xe = x0 ;
    *ye = y0 ;
    return ;
  }
  k0 = Atan2 (y0 - arc->y, x0 - arc->x);
  ke = Atan2 (*ye - arc->y, *xe - arc->x);
  if (b_first_end == TRUE)
  {
    b_check = Check_Angle (k0, ke, arc->kat1) ;
  }
  else
  {
    b_check = Check_Angle (k0, arc->kat2, ke) ;
  }
  if (b_check == TRUE)
  {
    *xe = x0 ;
    *ye = y0 ;
  }
}

static void extend_ellipticalarc_temp (ELLIPTICALARC *elarc, double x0, double y0, double *xe,
                             double *ye, BOOL b_first_ex, BOOL b_first_end)
/*-------------------------------------------------------------------------------------------*/
{
    double k0, ke ;
    BOOL b_check ;

    if (b_first_ex == TRUE)
    {
        *xe = x0 ;
        *ye = y0 ;
        return ;
    }
    k0 = Atan2 (y0 - elarc->y, x0 - elarc->x);
    ke = Atan2 (*ye - elarc->y, *xe - elarc->x);
    if (b_first_end == TRUE)
    {
        b_check = Check_Angle (k0, ke, Angle_Normal(elarc->kat1+elarc->angle)) ;
    }
    else
    {
        b_check = Check_Angle (k0, Angle_Normal(elarc->kat2+elarc->angle), ke) ;
    }
    if (b_check == TRUE)
    {
        *xe = x0 ;
        *ye = y0 ;
    }
}


static void extend_arc (void *ad, double x0, double y0)
/*----------------------------------------------------*/
{
  LUK *ptrs_arc, arc_tmp;
  NAGLOWEK *nag ;
  BLOK * ptrs_b ;
  WIELOKAT *ptrs_w;
  SPLINE *ptrs_s;
  ELLIPSE *ptrs_e;
  ELLIPTICALARC *ptrs_ea;
  LINIA line_tmp_w = Ldef;
  double xe, ye, xend, yend, angle, angle_s, angle_e ;
  BOOL b_first_end, b_first_ex, b_przec ;
  void  *ad_h,  *obbl_h ;
  float df_x1, df_y1;
  int i, j, k;

  SPLINE tmp_spline = Splinedef;
  double bnodes_2_tangent_org, bnodes_2_tangent;
  int node_count;

  b_przec = FALSE;

  b_first_ex = TRUE ;
  ptrs_arc = (LUK *)ad ;
  angle_s = Angle_Normal (ptrs_arc->kat1) ;
  angle_e = Angle_Normal (ptrs_arc->kat2) ;
  ad_h = (void *)ad ;
  b_first_end = End_Arc (&xend, &yend, x0, y0, (LUK*)ptrs_arc) ;
  obiekt_tok ((char*)ADP, ADK, (char **)&nag, ONieOkreslony);
  while (nag != NULL)
  {
    obbl_h  = (void  *)nag ;	/*pominiecie wydluzenia do samego siebie*/
    if (nag->atrybut == Ablok  && ad_h != obbl_h)
    {
       switch (nag->obiekt)
       {
	 case Olinia :
	 {
	    x0 = xend ;
	    y0 = yend ;
	    b_przec = przeciecieLl_ex ((void *)nag, (void *)ptrs_arc,
				(double *)&x0, (double *)&y0) ;
	    break ;
	 }
	case Owwielokat:
	{
		double x0_temp[4];
		double y0_temp[4];
		double l2min = BIGNOD;
		double l2;
		BOOL b_przec_temp[4] = { FALSE, FALSE, FALSE, FALSE };

		x0 = xend;
		y0 = yend;

		b_przec = FALSE;

		ptrs_w = (WIELOKAT*)nag;

		df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
		df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

		for (i = 0; i < ptrs_w->lp; i += 2)
		{
			line_tmp_w.x1 = df_x1;
			line_tmp_w.y1 = df_y1;
			line_tmp_w.x2 = ptrs_w->xy[i];
			line_tmp_w.y2 = ptrs_w->xy[i + 1];

			b_przec_temp[i/2] = przeciecieLl_ex(&line_tmp_w, (void *)ptrs_arc,
				(double *)&x0, (double *)&y0);

			x0_temp[i / 2] = x0;
			y0_temp[i / 2] = y0;
			
			df_x1 = line_tmp_w.x2;
			df_y1 = line_tmp_w.y2;
		}
		//possibly we have 4 intersection point; we choose the closest to endpoint
		for (i = 0; i < 4; i++)
		{
			if (b_przec_temp[i])
			{
				l2 = (x0_temp[i] - xend)*(x0_temp[i] - xend) + (y0_temp[i] - yend)*(y0_temp[i] - yend);
				if (l2 < l2min)
				{
					l2min = l2;
					x0 = x0_temp[i];
					y0 = y0_temp[i];
					b_przec = TRUE;
				}
			}
		}

		break;
	}
	case Ospline:
	{
#define Npts 90  //for more precission
		int i;
		float out_x[MaxNpts], out_y[MaxNpts];
		double l2min = BIGNOD;
		double l2;
		double x0_temp[Npts];
		double y0_temp[Npts];

		int points_count;
		int NODES_NO;

		x0 = xend;
		y0 = yend;

		b_przec = FALSE;
		
		BOOL b_przec_temp[Npts-1];

		for (i = 0; i < Npts - 1; i++)  b_przec_temp[i] = FALSE;

		ptrs_s = (SPLINE*)nag;

		if (ptrs_s->npts < 5)
		{

			calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, out_x, out_y);

			for (i = 0; i < Npts - 1; i++)
			{
				line_tmp_w.x1 = out_x[i];
				line_tmp_w.y1 = out_y[i];
				line_tmp_w.x2 = out_x[i + 1];
				line_tmp_w.y2 = out_y[i + 1];

				b_przec_temp[i] = przeciecieLl_ex(&line_tmp_w, (void*)ptrs_arc,
					(double*)&x0, (double*)&y0);

				x0_temp[i] = x0;
				y0_temp[i] = y0;

			}

			//possibly we have Npts-1 intersection point; we choose the closest to endpoint
			for (i = 0; i < Npts - 1; i++)
			{
				if (b_przec_temp[i])
				{
					l2 = (x0_temp[i] - xend) * (x0_temp[i] - xend) + (y0_temp[i] - yend) * (y0_temp[i] - yend);
					if (l2 < l2min)
					{
						l2min = l2;
						x0 = x0_temp[i];
						y0 = y0_temp[i];
						b_przec = TRUE;
					}
				}
			}
		}
		else
		{
			NODES_NO = ptrs_s->lp / 2 + 5;

			float* all_x0_temp = malloc(90 * NODES_NO * sizeof(float));
			if (all_x0_temp == NULL) break;
			float* all_y0_temp = malloc(90 * NODES_NO * sizeof(float));
			if (all_y0_temp == NULL)
			{
				free(all_x0_temp);
				break;
			}
			BOOL* all_b_przec_temp = malloc(90 * NODES_NO * sizeof(BOOL));
			if (all_b_przec_temp == NULL)
				if (all_y0_temp == NULL)
				{
					free(all_x0_temp);
					free(all_y0_temp);
					break;
				}

			curviness = ptrs_s->xy[ptrs_s->lp];

			tmp_spline.npts = 4;
			tmp_spline.lp = 8;
			tmp_spline.kolor = ptrs_s->kolor;
			tmp_spline.typ = ptrs_s->typ;

			node_count = 1;
			points_count = 0;

			set_bnodes(ptrs_s, ptrs_s->xy, &node_count);

			calc_tangents(ptrs_s->closed, &node_count, &bnodes_2_tangent);

			if (ptrs_s->closed == 0)
			{
				bnodes_2_tangent_org = bnodes[2].tangent;
				bnodes[2].tangent = bnodes_2_tangent;
				get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

				for (i = 0; i < Npts - 1; i++)
				{
					line_tmp_w.x1 = out_x[i];
					line_tmp_w.y1 = out_y[i];
					line_tmp_w.x2 = out_x[i + 1];
					line_tmp_w.y2 = out_y[i + 1];

					all_b_przec_temp[i] = przeciecieLl_ex(&line_tmp_w, (void*)ptrs_arc,
						(double*)&x0, (double*)&y0);

					all_x0_temp[i] = x0;
					all_y0_temp[i] = y0;

				}
                points_count += Npts;

				bnodes[2].tangent = bnodes_2_tangent_org;
			}

			for (k = 2; k < node_count - 2; k++)  //1 - extra_node
			{
				get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

				for (i = 0; i < Npts - 1; i++)
				{
					line_tmp_w.x1 = out_x[i];
					line_tmp_w.y1 = out_y[i];
					line_tmp_w.x2 = out_x[i + 1];
					line_tmp_w.y2 = out_y[i + 1];

					all_b_przec_temp[points_count+i] = przeciecieLl_ex(&line_tmp_w, (void*)ptrs_arc,
						(double*)&x0, (double*)&y0);

					all_x0_temp[points_count+i] = x0;
					all_y0_temp[points_count+i] = y0;
				}
				points_count += Npts;

			}

			if ((node_count > 3) && (ptrs_s->closed == 0))
			{
				get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

				for (i = 0; i < Npts - 1; i++)
				{
					line_tmp_w.x1 = out_x[i];
					line_tmp_w.y1 = out_y[i];
					line_tmp_w.x2 = out_x[i + 1];
					line_tmp_w.y2 = out_y[i + 1];

					all_b_przec_temp[points_count+i] = przeciecieLl_ex(&line_tmp_w, (void*)ptrs_arc,
						(double*)&x0, (double*)&y0);

					all_x0_temp[points_count + i] = x0;
					all_y0_temp[points_count + i] = y0;
				}
				points_count += Npts;
			}

			//possibly we have Npts-1 intersection point; we choose the closest to endpoint
			for (i = 0; i < points_count - 1; i++)
			{
				if (all_b_przec_temp[i])
				{
					l2 = (all_x0_temp[i] - xend) * (all_x0_temp[i] - xend) + (all_y0_temp[i] - yend) * (all_y0_temp[i] - yend);
					if (l2 < l2min)
					{
						l2min = l2;
						x0 = all_x0_temp[i];
						y0 = all_y0_temp[i];
						b_przec = TRUE;
					}
				}
			}

			free(all_x0_temp);
			free(all_y0_temp);
			free(all_b_przec_temp);
		}
		break;
	}
           case Oellipse:
           case Ofilledellipse:
           {  int i;
               double x0_temp[8];
               double y0_temp[8];
               int numpoints;
               double xy[512];
               int verno;
               //int NODES_NO;
               double l2min = BIGNOD;
               double l2;
               int points_count;

               x0 = xend;
               y0 = yend;

               b_przec = FALSE;

               ptrs_e = (ELLIPSE*)nag;

               verno = Get_Ell_Verno () ;
               Set_Ell_Verno (250) ;
               numpoints=Get_Ellipse_Points(ptrs_e->x, ptrs_e->y, ptrs_e->rx, ptrs_e->ry, ptrs_e->angle, xy);

               Set_Ell_Verno (verno) ;

               xy[numpoints] = xy[0];
               xy[numpoints+1] = xy[1];
               numpoints+=2;

               points_count = 0;

               for (i = 0; i < (numpoints-2) - 1; i+=2) {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i + 1];
                   line_tmp_w.x2 = xy[i + 2];
                   line_tmp_w.y2 = xy[i + 3];

                   if (przeciecieLl_ex(&line_tmp_w, (void *) ptrs_arc, (double *) &x0, (double *) &y0))
                   {
                       x0_temp[points_count] = x0;
                       y0_temp[points_count] = y0;
                       points_count++;
                    }
               }
               //possibly we have points_count intersection point; we choose the closest to endpoint
               for (i = 0; i < points_count; i++)
               {
                   l2 = (x0_temp[i] - xend) * (x0_temp[i] - xend) + (y0_temp[i] - yend) * (y0_temp[i] - yend);
                   if (l2 < l2min)
                   {
                       l2min = l2;
                       x0 = x0_temp[i];
                       y0 = y0_temp[i];
                       b_przec = TRUE;
                   }
               }
               break;
           }
           case Oellipticalarc:
           {
               int i;
               double x0_temp[8];
               double y0_temp[8];
               int numpoints;
               double xy[512];
               int verno;
               double l2min = BIGNOD;
               double l2;
               int points_count;

               x0 = xend;
               y0 = yend;

               b_przec = FALSE;

               ptrs_ea = (ELLIPTICALARC*)nag;

               verno = Get_Ell_Verno () ;
               Set_Ell_Verno (250) ;
               numpoints=Get_EllipticalArc_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, ptrs_ea->kat1, ptrs_ea->kat2,xy);

               Set_Ell_Verno (verno) ;

               points_count = 0;

               for (i = 0; i < (numpoints-2) - 1; i+=2)
               {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i+1];
                   line_tmp_w.x2 = xy[i+2];
                   line_tmp_w.y2 = xy[i+3];

                   if (przeciecieLl_ex(&line_tmp_w, (void*)ptrs_arc, (double*)&x0, (double*)&y0)) {
                       x0_temp[points_count] = x0;
                       y0_temp[points_count] = y0;
                       points_count++;
                   }
               }
               //possibly we have NODES_NO intersection point; we choose the closest to to endpoint
               for (i = 0; i < points_count; i++)
               {
                   l2 = (x0_temp[i] - xend) * (x0_temp[i] - xend) + (y0_temp[i] - yend) * (y0_temp[i] - yend);
                   if (l2 < l2min)
                   {
                       l2min = l2;
                       x0 = x0_temp[i];
                       y0 = y0_temp[i];
                       b_przec = TRUE;
                   }
               }
               break;
           }
	 case Ookrag :
	 case Okolo :
	 {
	    b_przec = przeciecieOl_ex ((void *)nag, (void *)ptrs_arc,
				(double *)&x0, (double *)&y0, b_first_end) ;
	    break ;
	 }
	 case Oluk :
	 {
	    b_przec = przecieciell_ex ((void *)nag, (void *)ptrs_arc,
				(double *)&x0, (double *)&y0, b_first_end) ;
	    break ;
	 }

	 default :
	    break ;
       }

       if (b_przec == TRUE)
       {
	 angle = Angle_Normal (Atan2 (y0 - ptrs_arc->y, x0 - ptrs_arc->x)) ;
	 if ((TRUE == b_first_end && TRUE == Check_if_Equal (angle, angle_s)) ||
	     (FALSE == b_first_end && TRUE == Check_if_Equal (angle, angle_e)))
	 {
	   return ; /*koniec luku lezy na brzegu linii ograniczajacych*/
	 }
	 extend_arc_temp (ptrs_arc, x0, y0, &xe, &ye, b_first_ex, b_first_end) ;
	 b_first_ex = FALSE ;
      }
    }
    obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony);
  }
  if (b_first_ex == FALSE)
  {
    angle = Angle_Normal (Atan2 (ye - ptrs_arc->y, xe - ptrs_arc->x)) ;
    if ((TRUE == b_first_end && TRUE == Check_if_Equal (angle, angle_e)) ||
	(FALSE == b_first_end && TRUE == Check_if_Equal (angle, angle_s)))
    {
      return ;
    }
    if (TRUE == Check_if_Vertex_Inside_Pline (ad, xend, yend))
    {
      ErrList (81) ;
      return ;
    }
    Set_Block_Change_Mode ((void *)ad, OB1CHANGE_BREAK) ;
    ptrs_b = NULL ;
    memcpy (&arc_tmp, ptrs_arc, sizeof (arc_tmp)) ;
    if (b_first_end == TRUE)
    {
        arc_tmp.kat1 = angle ;
    }
    else
    {
        arc_tmp.kat2 = angle ;
    }
    change_date_ (ptrs_b, (void *)ptrs_arc, (void *)&arc_tmp, NULL) ;
    Change = TRUE;
    Set_Auto_Backup (TRUE);
  }
}

BOOL check_if_point_on_line_segment__(double p1x, double p1y, double p2x, double p2y, double px, double py)
{
    if (Check_if_LE(px,max(p1x, p2x))
        && Check_if_LE(px, min(p1x, p2x))
        && Check_if_LE(py, max(p1y, p2y))
            && Check_if_LE(py,min(p1y, p2y))) {
        return TRUE;
    }
        return FALSE;
}


BOOL check_if_point_on_line_segment_(double p1x, double p1y, double p2x, double p2y, double px, double py)
{
    if (px <= max(p1x, p2x)
        && px <= min(p1x, p2x)
        && py <= max(p1y, p2y)
        && py <=min(p1y, p2y)) {
        return TRUE;
    }
    return FALSE;
}

BOOL check_if_point_on_line_segment(double p1x, double p1y, double p2x, double p2y, double px, double py)
{
    double EPSILON=1.0/DF_PRECISION;
    double ab = sqrt((p1x-p2x)*(p1x-p2x) + (p1y-p2y)*(p1y-p2y));
    double ac = sqrt((p1x-px)*(p1x-px) + (p1y-py)*(p1y-py));
    double bc = sqrt((p2x-px)*(p2x-px) + (p2y-py)*(p2y-py));

    return (fabs(ac + bc - ab) < EPSILON);
}


BOOL no_trim_ellpticalarc(ELLIPTICALARC *ea, double px, double py)
{   double df_angle;
    double df_angle1, df_angle2;

    //check if point px, py creates an angle between kat2 and kat1
    df_angle= Angle_Normal(Atan2(py-ea->y, px-ea->x));
    df_angle1=Angle_Normal(ea->angle+ea->kat1);
    df_angle2=Angle_Normal(ea->angle+ea->kat2);
    if (df_angle1 > df_angle2) df_angle2 += Pi2;
    if (df_angle1 > df_angle) df_angle += Pi2;
    if ((df_angle<df_angle1) || (df_angle>df_angle2)) return TRUE;
    return FALSE;
}

static void extend_ellipticalarc (void *ad, double x0, double y0)
/*-------------------------------------------------------------*/
{

    ELLIPTICALARC * ptrs_ea, ellipticalarc_tmp ;
    NAGLOWEK *nag ;
    BLOK * ptrs_b ;
    LINIA *ptrs_l;
    WIELOKAT *ptrs_w;
    SPLINE *ptrs_s;
    ELLIPSE *ptrs_e;
    LUK *ptrs_luk;
    LINIA line_tmp_w0= Ldef, line_tmp_w = Ldef;
    double xend, yend, angle, angle_s, angle_e ;
    BOOL b_first_end, b_first_ex, b_przec ;
    void  *ad_h,  *obbl_h ;
    float df_x1, df_y1;
    int i, j, k;
    double x01, y01;
    double xe, ye, x, y, te, t1, t2;

    SPLINE tmp_spline = Splinedef;
    double bnodes_2_tangent_org, bnodes_2_tangent;
    int node_count;
    int numpoints0;
    double xy0[512];
    int verno0;

    b_przec = FALSE;

    b_first_ex = TRUE ;
    ptrs_ea = (ELLIPTICALARC *)ad ;
    angle_s = Angle_Normal (ptrs_ea->kat1+ptrs_ea->angle) ;
    angle_e = Angle_Normal (ptrs_ea->kat2+ptrs_ea->angle) ;
    ad_h = (void *)ad ;
    b_first_end = End_EllipticalArc (&xend, &yend, x0, y0, ptrs_ea) ;
    verno0 = Get_Ell_Verno () ;
    Set_Ell_Verno (250) ;  //250

    numpoints0=Get_Ellipse_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, xy0);
    xy0[numpoints0] = xy0[0];
    xy0[numpoints0+1] = xy0[1];
    numpoints0+=2;

    Set_Ell_Verno (verno0) ;
    obiekt_tok ((char*)ADP, ADK, (char **)&nag, ONieOkreslony);
    while (nag != NULL)
    {
        obbl_h  = (void  *)nag ;	/*pominiecie wydluzenia do samego siebie*/
        if (nag->atrybut == Ablok  && ad_h != obbl_h)
        {
            switch (nag->obiekt)
            {
                case Olinia :
                {
                    int i;
                    double l2min = BIGNOD;
                    double l2;
                    int points_count;
                    double xe1, ye1;

                    ptrs_l = (LINIA*)nag;

                    x0 = xend;
                    y0 = yend;

                    b_przec = FALSE;

                    points_count = 0;

                    te = BIGNOD ;

                    for (i = 0; i < (numpoints0-2); i+=2)
                    {
                        line_tmp_w.x1 = xy0[i];
                        line_tmp_w.y1 = xy0[i+1];
                        line_tmp_w.x2 = xy0[i+2];
                        line_tmp_w.y2 = xy0[i+3];

                        if (0 == przeciecieLL_tt(&x, &y, (void *)&line_tmp_w, (void *)nag, &t1, &t2))
                        {
                            ;  /*linie rownolegle*/
                        }
                        else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
                        {
                            if (TRUE == Check_if_Equal(t1, 1))
                            {
                                ;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
                            }
                            if (t1 > 1 && t1 < te)
                            {
                                if (check_if_point_on_ellipse(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, x, y))
                                {
                                    xe1 = x;
                                    ye1 = y;
                                    te = t1;
                                }
                            }
                        }
                    }

                    if ( te < BIGNOF) ////if (b_przec == TRUE)
                    {
                        if (no_trim_ellpticalarc(ptrs_ea, xe1, ye1))
                        {
                            b_przec = TRUE;
                            x0 = xe1;
                            y0 = ye1;
                        }
                    }
                    break ;
                }
                case Owwielokat:
                {
                    double x0_temp[8];
                    double y0_temp[8];
                    double l2min = BIGNOD;
                    double l2;
                    int points_count;
                    double xe1, ye1;
                    BOOL b_przec1 = FALSE;

                    x0 = xend;
                    y0 = yend;

                    b_przec = FALSE;

                    ptrs_w = (WIELOKAT*)nag;

                    df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
                    df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

                    points_count=0;
                    l2min = BIGNOD;

                    for (i = 0; i < ptrs_w->lp; i += 2)
                    {
                        line_tmp_w0.x1 = df_x1;
                        line_tmp_w0.y1 = df_y1;
                        line_tmp_w0.x2 = ptrs_w->xy[i];
                        line_tmp_w0.y2 = ptrs_w->xy[i + 1];

                        te = BIGNOD ;
                        b_przec1 = FALSE;

                        for (j = 0; j < (numpoints0-2); j+=2)
                        {
                            line_tmp_w.x1 = xy0[j];
                            line_tmp_w.y1 = xy0[j+1];
                            line_tmp_w.x2 = xy0[j+2];
                            line_tmp_w.y2 = xy0[j+3];

                            if (0 == przeciecieLL_tt (&x, &y, (void *)&line_tmp_w, (void *)&line_tmp_w0, &t1, &t2))
                            {
                                ;
                            }
                            else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
                            {
                                if (TRUE == Check_if_Equal(t1, 1))
                                {
                                    ;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
                                }
                                if (t1 > 1 && t1 < te)
                                {
                                    if (check_if_point_on_ellipse(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, x, y))
                                    {
                                        b_przec1 = TRUE;
                                        xe1 = x;
                                        ye1 = y;
                                        te = t1;
                                    }
                                }
                            }
                        }

                        if (b_przec1 == TRUE)
                        {
                            x0_temp[points_count] = xe1;
                            y0_temp[points_count] = ye1;
                            points_count++;
                        }

                        df_x1 = line_tmp_w0.x2;
                        df_y1 = line_tmp_w0.y2;
                    }
                    //possibly we have points_count intersection point; we choose the closest to endpoint
                    for (i = 0; i < points_count; i++)
                    {
                        if (no_trim_ellpticalarc(ptrs_ea, x0_temp[i], y0_temp[i]))
                        {
                            l2 = (x0_temp[i] - xend) * (x0_temp[i] - xend) + (y0_temp[i] - yend) * (y0_temp[i] - yend);
                            if (l2 < l2min) {
                                l2min = l2;
                                x0 = x0_temp[i];
                                y0 = y0_temp[i];
                                b_przec = TRUE;
                            }
                        }
                    }
                    break;
                }
                case Ospline:
                {
#define Npts 90  //for more precission
                    int i;
                    float out_x[MaxNpts], out_y[MaxNpts];
                    double l2min = BIGNOD;
                    double l2;
                    double x0_temp[Npts];
                    double y0_temp[Npts];

                    int points_count;
                    int NODES_NO;
                    double xe1, ye1;
                    double b_przec1=FALSE;

                    x0 = xend;
                    y0 = yend;

                    b_przec = FALSE;

                    BOOL b_przec_temp[Npts-1];

                    for (i = 0; i < Npts - 1; i++)  b_przec_temp[i] = FALSE;

                    ptrs_s = (SPLINE*)nag;

                    points_count=0;
                    l2min = BIGNOD;

                    if (ptrs_s->npts < 5)
                    {

                        calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, out_x, out_y);

                        for (i = 0; i < Npts - 1; i++)
                        {
                            line_tmp_w0.x1 = out_x[i];
                            line_tmp_w0.y1 = out_y[i];
                            line_tmp_w0.x2 = out_x[i + 1];
                            line_tmp_w0.y2 = out_y[i + 1];

                            te = BIGNOD ;
                            b_przec1 = FALSE;

                            for (j = 0; j < (numpoints0-2); j+=2)
                            {
                                line_tmp_w.x1 = xy0[j];
                                line_tmp_w.y1 = xy0[j + 1];
                                line_tmp_w.x2 = xy0[j + 2];
                                line_tmp_w.y2 = xy0[j + 3];

                                if (0 == przeciecieLL_tt(&x, &y, (void *) &line_tmp_w, (void *) &line_tmp_w0, &t1, &t2))
                                {
                                    ;
                                }

                                else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
                                {
                                    if (TRUE == Check_if_Equal(t1,1))
                                    {
                                        ;    /*koniec linii lezy na brzegu krawedzi wydluzania*/
                                    }
                                    if (t1 > 1 && t1 < te)
                                    {
                                        if (check_if_point_on_ellipse(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, x, y))
                                        {
                                            b_przec1 = TRUE;
                                            xe1 = x;
                                            ye1 = y;
                                            te = t1;
                                        }
                                    }
                                }
                            }
                            if (b_przec1 == TRUE)
                            {
                                x0_temp[points_count] = xe1;
                                y0_temp[points_count] = ye1;
                                points_count++;
                            }
                        }
                        //possibly we have points_count intersection point; we choose the closest to endpoint
                        for (i = 0; i < points_count; i++)
                        {
                            if (no_trim_ellpticalarc(ptrs_ea, x0_temp[i], y0_temp[i]))
                            {
                                l2 = (x0_temp[i] - xend) * (x0_temp[i] - xend) + (y0_temp[i] - yend) * (y0_temp[i] - yend);
                                if (l2 < l2min)
                                {
                                    l2min = l2;
                                    x0 = x0_temp[i];
                                    y0 = y0_temp[i];
                                    b_przec = TRUE;
                                }
                            }
                        }
                    }
                    else
                    {
                        NODES_NO = ptrs_s->lp / 2 + 5;

                        float* all_x0_temp = malloc(90 * NODES_NO * sizeof(float));
                        if (all_x0_temp == NULL) break;
                        float* all_y0_temp = malloc(90 * NODES_NO * sizeof(float));
                        if (all_y0_temp == NULL)
                        {
                            free(all_x0_temp);
                            break;
                        }

                        curviness = ptrs_s->xy[ptrs_s->lp];

                        tmp_spline.npts = 4;
                        tmp_spline.lp = 8;
                        tmp_spline.kolor = ptrs_s->kolor;
                        tmp_spline.typ = ptrs_s->typ;

                        node_count = 1;
                        points_count = 0;

                        set_bnodes(ptrs_s, ptrs_s->xy, &node_count);

                        calc_tangents(ptrs_s->closed, &node_count, &bnodes_2_tangent);

                        points_count=0;
                        l2min = BIGNOD;

                        if (ptrs_s->closed == 0)
                        {
                            bnodes_2_tangent_org = bnodes[2].tangent;
                            bnodes[2].tangent = bnodes_2_tangent;
                            get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
                            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                            for (i = 0; i < Npts - 1; i++)
                            {
                                line_tmp_w0.x1 = out_x[i];
                                line_tmp_w0.y1 = out_y[i];
                                line_tmp_w0.x2 = out_x[i + 1];
                                line_tmp_w0.y2 = out_y[i + 1];

                                te = BIGNOD ;
                                b_przec1 = FALSE;

                                for (j = 0; j < (numpoints0-2); j+=2)
                                {
                                    line_tmp_w.x1 = xy0[j];
                                    line_tmp_w.y1 = xy0[j + 1];
                                    line_tmp_w.x2 = xy0[j + 2];
                                    line_tmp_w.y2 = xy0[j + 3];

                                    if (0 == przeciecieLL_tt(&x, &y, (void *) &line_tmp_w, (void *) &line_tmp_w0, &t1, &t2))
                                    {
                                        ;
                                    }

                                    else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
                                    {
                                        if (TRUE == Check_if_Equal(t1,1))
                                        {
                                            ;    /*koniec linii lezy na brzegu krawedzi wydluzania*/
                                        }
                                        if (t1 > 1 && t1 < te)
                                        {
                                            if (check_if_point_on_ellipse(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, x, y))
                                            {
                                                b_przec1 = TRUE;
                                                xe1 = x;
                                                ye1 = y;
                                                te = t1;
                                            }
                                        }
                                    }
                                }

                                if (b_przec1 == TRUE)
                                {
                                    all_x0_temp[points_count] = xe1;
                                    all_y0_temp[points_count] = ye1;
                                    points_count++;
                                }

                            }

                            bnodes[2].tangent = bnodes_2_tangent_org;
                        }

                        for (k = 2; k < node_count - 2; k++)  //1 - extra_node
                        {
                            get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
                            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                            for (i = 0; i < Npts - 1; i++)
                            {
                                line_tmp_w0.x1 = out_x[i];
                                line_tmp_w0.y1 = out_y[i];
                                line_tmp_w0.x2 = out_x[i + 1];
                                line_tmp_w0.y2 = out_y[i + 1];

                                te = BIGNOD ;
                                b_przec1 = FALSE;

                                for (j = 0; j < (numpoints0-2); j+=2)
                                {
                                    line_tmp_w.x1 = xy0[j];
                                    line_tmp_w.y1 = xy0[j + 1];
                                    line_tmp_w.x2 = xy0[j + 2];
                                    line_tmp_w.y2 = xy0[j + 3];

                                    if (0 == przeciecieLL_tt(&x, &y, (void *) &line_tmp_w, (void *) &line_tmp_w0, &t1, &t2))
                                    {
                                        ;
                                    }

                                    else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
                                    {
                                        if (TRUE == Check_if_Equal(t1,1))
                                        {
                                            ;    /*koniec linii lezy na brzegu krawedzi wydluzania*/
                                        }
                                        if (t1 > 1 && t1 < te)
                                        {
                                            if (check_if_point_on_ellipse(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, x, y))
                                            {
                                                b_przec1 = TRUE;
                                                xe1 = x;
                                                ye1 = y;
                                                te = t1;
                                            }
                                        }
                                    }
                                }

                                if (b_przec1 == TRUE)
                                {
                                    all_x0_temp[points_count] = xe1;
                                    all_y0_temp[points_count] = ye1;
                                    points_count++;
                                }
                            }
                        }

                        if ((node_count > 3) && (ptrs_s->closed == 0))
                        {
                            get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
                            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                            for (i = 0; i < Npts - 1; i++)
                            {
                                line_tmp_w0.x1 = out_x[i];
                                line_tmp_w0.y1 = out_y[i];
                                line_tmp_w0.x2 = out_x[i + 1];
                                line_tmp_w0.y2 = out_y[i + 1];

                                te = BIGNOD ;
                                b_przec1 = FALSE;

                                for (j = 0; j < (numpoints0-2); j+=2)
                                {
                                    line_tmp_w.x1 = xy0[j];
                                    line_tmp_w.y1 = xy0[j + 1];
                                    line_tmp_w.x2 = xy0[j + 2];
                                    line_tmp_w.y2 = xy0[j + 3];

                                    if (0 == przeciecieLL_tt(&x, &y, (void *) &line_tmp_w, (void *) &line_tmp_w0, &t1, &t2))
                                    {
                                        ;
                                    }

                                    else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
                                    {
                                        if (TRUE == Check_if_Equal(t1,1))
                                        {
                                            ;    /*koniec linii lezy na brzegu krawedzi wydluzania*/
                                        }
                                        if (t1 > 1 && t1 < te)
                                        {
                                            if (check_if_point_on_ellipse(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, x, y))
                                            {
                                                b_przec1 = TRUE;
                                                xe1 = x;
                                                ye1 = y;
                                                te = t1;
                                            }
                                        }
                                    }
                                }

                                if (b_przec1 == TRUE)
                                {
                                    all_x0_temp[points_count] = xe1;
                                    all_y0_temp[points_count] = ye1;
                                    points_count++;
                                }
                            }
                        }

                        //possibly we have Npts-1 intersection point; we choose the closest to endpoint
                        for (i = 0; i < points_count; i++)
                        {
                            if (no_trim_ellpticalarc(ptrs_ea, all_x0_temp[i], all_y0_temp[i]))
                            {
                                l2 = (all_x0_temp[i] - xend) * (all_x0_temp[i] - xend) + (all_y0_temp[i] - yend) * (all_y0_temp[i] - yend);
                                if (l2 < l2min) {
                                    l2min = l2;
                                    x0 = all_x0_temp[i];
                                    y0 = all_y0_temp[i];
                                    b_przec = TRUE;
                                }
                            }
                        }
                        free(all_x0_temp);
                        free(all_y0_temp);
                    }
                    break;
                }
                case Oellipse:
                case Ofilledellipse:
                case Oellipticalarc:
                {
                    int i,j;
                    double x0_temp[8];
                    double y0_temp[8];
                    int numpoints;
                    double xy[512];
                    int verno;
                    double l2min = BIGNOD;
                    double l2;
                    int points_count;
                    double xe1, ye1;
                    ELLIPTICALARC *ptrs_ea1;
                    ELLIPSE *ptrs_e1;
                    BOOL b_przec1=FALSE;

                    x0 = xend;
                    y0 = yend;

                    b_przec = FALSE;

                    verno = Get_Ell_Verno () ;
                    Set_Ell_Verno (250) ;

                    if ((nag->obiekt==Oellipse) || (nag->obiekt==Ofilledellipse))
                    {
                        ptrs_e1 = (ELLIPSE*)nag;
                        numpoints=Get_Ellipse_Points(ptrs_e1->x, ptrs_e1->y, ptrs_e1->rx, ptrs_e1->ry, ptrs_e1->angle,xy);
                        xy[numpoints] = xy[0];
                        xy[numpoints+1] = xy[1];
                        numpoints+=2;
                    }
                    else
                    {
                        ptrs_ea1 = (ELLIPTICALARC*)nag;
                        numpoints=Get_EllipticalArc_Points(ptrs_ea1->x, ptrs_ea1->y, ptrs_ea1->rx, ptrs_ea1->ry, ptrs_ea1->angle, ptrs_ea->kat1, ptrs_ea->kat2,xy);
                    }

                    Set_Ell_Verno (verno) ;

                    points_count=0;
                    l2min = BIGNOD;

                    for (i = 0; i < (numpoints-2); i+=2)
                    {
                        line_tmp_w0.x1 = xy[i];
                        line_tmp_w0.y1 = xy[i+1];
                        line_tmp_w0.x2 = xy[i+2];
                        line_tmp_w0.y2 = xy[i+3];

                        te = BIGNOD ;
                        b_przec1 = FALSE;

                        for (j = 0; j < (numpoints0-2) - 1; j+=2)
                        {
                            line_tmp_w.x1 = xy0[j];
                            line_tmp_w.y1 = xy0[j+1];
                            line_tmp_w.x2 = xy0[j+2];
                            line_tmp_w.y2 = xy0[j+3];

                            if (0 == przeciecieLL_tt (&x, &y, (void *)&line_tmp_w, (void *)&line_tmp_w0, &t1, &t2))
                            {
                                ;
                            }
                            else if (Check_if_GE(t2, 0) && Check_if_LE(t2, 1))
                            {
                                if (TRUE == Check_if_Equal(t1, 1))
                                {
                                    ;	/*koniec linii lezy na brzegu krawedzi wydluzania*/
                                }
                                if (t1 > 1 && t1 < te)
                                {
                                    if (check_if_point_on_ellipse(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, x, y))
                                    {
                                        b_przec1 = TRUE;
                                        xe1 = x;
                                        ye1 = y;
                                        te = t1;
                                    }
                                }
                            }
                        }

                        if (b_przec1 == TRUE)
                        {
                            x0_temp[points_count] = xe1;
                            y0_temp[points_count] = ye1;
                            points_count++;
                        }
                    }
                    //possibly we have points_count intersection point; we choose the closest to endpoint
                    for (i = 0; i < points_count; i++)
                    {
                        if (no_trim_ellpticalarc(ptrs_ea, x0_temp[i], y0_temp[i]))
                        {
                            l2 = (x0_temp[i] - xend) * (x0_temp[i] - xend) + (y0_temp[i] - yend) * (y0_temp[i] - yend);
                            if (l2 < l2min) {
                                l2min = l2;
                                x0 = x0_temp[i];
                                y0 = y0_temp[i];
                                b_przec = TRUE;
                            }
                        }
                    }
                    break;
                }
                case Ookrag :
                case Okolo :
                case Oluk :
                {
                    int i;
                    double x0_temp[8];
                    double y0_temp[8];
                    int numpoints;
                    double xy[512];
                    int verno;
                    double l2min = BIGNOD;
                    double l2;
                    int points_count;
                    double xe1, ye1;

                    double x1, x2, y1, y2, t1, t2, t0;
                    int l;

                    x0 = xend;
                    y0 = yend;

                    b_przec = FALSE;

                    ptrs_luk = (LUK*)nag;

                    points_count = 0;

                    l2min = BIGNOD;

                    te = BIGNOD ;

                    for (i = 0; i < (numpoints0-2) - 1; i+=2) {
                        line_tmp_w.x1 = xy0[i];
                        line_tmp_w.y1 = xy0[i + 1];
                        line_tmp_w.x2 = xy0[i + 2];
                        line_tmp_w.y2 = xy0[i + 3];

                        if (nag->obiekt == Oluk)
                        {
                            l = przeciecieLl_tt (&x1, &y1, &t1, &x2, &y2, &t2,
                                                 (void *)&line_tmp_w, (void *)nag) ;  /*t2>=t1*/
                        }
                        else
                        {
                            l = przeciecieLO_tt (&x1, &y1, &t1, &x2, &y2, &t2,
                                                 (void *)&line_tmp_w, (void *)nag) ;  /*t2>=t1*/
                        }
                        if (0 == l)
                        {
                            ;
                        }
                        if (l) {
                            if (TRUE == Check_if_Equal(t1, 1) ||
                                TRUE == Check_if_Equal(t2, 1)) {
                                //return ; /*koniec linii lezy na brzegu okregu*/
                                ;
                            }
                            if (t1 > 1) {
                                x0 = x1;
                                y0 = y1;
                                t0 = t1;
                            } else if (t1 < 1 && t2 > 1) {
                                x0 = x2;
                                y0 = y2;
                                t0 = t2;
                            } else {
                                break;
                            }
                            if (t0 < te) {
                                xe1 = x0;
                                ye1 = y0;
                                te = t0;
                            }

                            if ((l) && (check_if_point_on_ellipse(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, x0, y0))) {
                                x0_temp[points_count] = x0;
                                y0_temp[points_count] = y0;
                                points_count++;
                            }
                        }
                    }

                    //possibly we have points_count intersection point; we choose the closest to endpoint
                    for (i = 0; i < points_count; i++)
                    {
                        if (no_trim_ellpticalarc(ptrs_ea, x0_temp[i], y0_temp[i]))
                        {
                            l2 = (x0_temp[i] - xend) * (x0_temp[i] - xend) + (y0_temp[i] - yend) * (y0_temp[i] - yend);
                            if (l2 < l2min) {
                                l2min = l2;
                                x0 = x0_temp[i];
                                y0 = y0_temp[i];
                                b_przec = TRUE;
                            }
                        }
                    }

                    break ;
                }
                default :
                    break ;
            }

            if (b_przec == TRUE)
            {
                angle = Angle_Normal (Atan2 (y0 - ptrs_ea->y, x0 - ptrs_ea->x)) ;
                if ((TRUE == b_first_end && TRUE == Check_if_Equal (angle, angle_s)) ||
                    (FALSE == b_first_end && TRUE == Check_if_Equal (angle, angle_e)))
                {
                    return ; /*koniec luku lezy na brzegu linii ograniczajacych*/
                }
                extend_ellipticalarc_temp (ptrs_ea, x0, y0, &xe, &ye, b_first_ex, b_first_end) ;
                b_first_ex = FALSE ;
            }
        }
        obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony);
    }
    if (b_first_ex == FALSE)
    {
        angle = Angle_Normal (Atan2 (ye - ptrs_ea->y, xe - ptrs_ea->x)) ;
        if ((TRUE == b_first_end && TRUE == Check_if_Equal (angle, angle_e)) ||
            (FALSE == b_first_end && TRUE == Check_if_Equal (angle, angle_s)))
        {
            return ;
        }
        Set_Block_Change_Mode ((void *)ad, OB1CHANGE_BREAK) ;
        ptrs_b = NULL ;
        memcpy (&ellipticalarc_tmp, ptrs_ea, sizeof (ellipticalarc_tmp)) ;
        if (b_first_end == TRUE)
        {
            ellipticalarc_tmp.kat1 = angle-ptrs_ea->angle;
        }
        else
        {
            ellipticalarc_tmp.kat2 = angle-ptrs_ea->angle;
        }

        change_date_ (ptrs_b, (void *)ptrs_ea, (void *)&ellipticalarc_tmp, NULL) ;
        Change = TRUE;
        Set_Auto_Backup (TRUE);
    }
}


int Extend_Proc (void)
/*---------------------*/
{
  double X0, Y0 ;
  EVENT *ev;
  unsigned type ;
  void * ad ;

    if (FALSE == ini_extend (0))
    {
        return FALSE ;
    }

  while(1)
  {
     ev = Get_Event_Point (NULL, &X0, &Y0);
     if (ev->What == evKeyDown)
     {
       if (ev->Number == 0)
       {
       ini_extend (1) ;
	   return ESC;
       }
       if (ev->Number == ENTER)
       {
         CUR_OFF(X,Y);
	 type = Blinia | Bluk | Bellipticalarc;
	 if ( NULL != (ad = obiekt_wybrany1 (&type)) &&
	     0 != Layers [((LINIA*)ad)->warstwa].edit)
	 {
	   switch (type)
	   {
	      case Blinia :
		extend_line (ad, X0, Y0) ;
		break;
	      case Bluk :
		extend_arc (ad, X0, Y0) ;
		break;
        case Bellipticalarc :
        extend_ellipticalarc (ad, X0, Y0) ;
        break;
	      default :
		break;
	   }
	 }
	 CUR_OFF(X, Y);
	 CUR_ON(X,Y);
       }
     }
      if (ev->What == evCommandP)
      {
          if (0 == ev->Number)
          {
              undo_extend () ;
              CUR_OFF(X, Y);
              CUR_ON (X, Y) ;
          }
      }
  }
}

#undef __O_EXTEND__
