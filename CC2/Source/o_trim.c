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

#define __O_TRIM__
#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#ifndef LINUX
#include <dos.h>
#endif
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_debug.h"
#include "o_osnap.h"
#include "o_object.h"
#include "o_undo.h"
#include "o_pline.h"

#include "menu.h"

#include "leak_detector_c.h"

/*---------Utnij---------------------------------------------*/

static double df__xe1, df__ye1, df__xe2, df__ye2, df__te1, df__te2, df__tsel ; /*parametru przeciecia linii*/
#ifndef LINUX
static
#endif
char *ptrsz__undo_old_ob=NULL, *ptrsz__undo_new_ob=NULL, *ptrsz__undo_new1_ob=NULL ;
static BOOL b__undo, b__undopl ;
static int i__undolp_attrib ;

TMENU mTrim = {1,0,0,15,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmTrim, NULL,NULL};
TMENU mExtend = {1,0,0,15,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmExtend, NULL,NULL};

static int przecieciell_tr (void *, void *, double *, double *, double *, double *) ;
static int przeciecieOl_tr (void *, void *, double *, double *, double *, double *) ;
static BOOL break_pline (void *, void *, void *, double, double, double, double, BOOL) ;
static void undo_pline (void) ;

static int shadows_no = 0;

extern NODE bnodes[MAX_NODES];
extern double curviness;
extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
extern void set_bnodes(SPLINE* ptrs_spline, float *ad_xy, int* node_count_);
extern int make_spline_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block);
extern void spline_shadows_to_plines(void);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);
extern void calc_bspline(int lp, int n, float points[], int p1, float* out_x, float* out_y);
extern void get_control_points(NODE n1, NODE n2, float points[]);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);
extern BOOL Point_in_Quad (QUAD *quad, double x, double y);
extern int Get_Ellipse_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double xy[]);
extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle, double df_eangle, double xy[]);
extern void Set_Ell_Verno (int i_ell_verno);
extern int Get_Ell_Verno (void);
extern int przeciecieLEA_ (double *x, double *y, void *adr, void *adr1);
extern void Get_EllipticalArc_EndPoints (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle,double df_sangle0,double df_eangle0,double *x1, double *y1, double *x2, double *y2);

static void *obiekt_wybrany1 (unsigned *typ)
/*---------------------------------------*/
{
  return select_w(typ, NULL);
}

static void new_intersection (double x, double y, double t)
/*---------------------------------------------------------*/
{
  if (t > df__te1 && t < df__te2  && t < df__tsel)
  {
    df__te1 = t ;
    df__xe1 = x ;
    df__ye1 = y ;
  }
  if (t < df__te2 && t > df__te1 && t > df__tsel)
  {
    df__te2 = t ;
    df__xe2 = x ;
    df__ye2 = y ;
  }
}


static BOOL change_date (BLOK *ptrs_b, void *ptr_ob,
			void *ptr_new, void *ptr_new1)
/*--------------------------------------------------*/
{
  void *ptr_temp ;

  b__undo = FALSE ;
  ptrsz__undo_new1_ob = NULL ;
  if (FALSE == Test_Object (ptr_ob) || FALSE == Test_Object (ptr_new))	/*dla celow testowych*/
  {
    Internal_Error (__LINE__,__FILE__);
    return FALSE ;
  }
  memcpy (ptrsz__undo_old_ob, ptr_ob,
	sizeof (NAGLOWEK) + ((NAGLOWEK*)ptr_ob)->n) ;
  memcpy (ptrsz__undo_new_ob, ptr_new,
	sizeof (NAGLOWEK) + ((NAGLOWEK*)ptr_new)->n) ;
  rysuj_obiekt ((char *)ptr_ob, COPY_PUT, 0) ;
  if (NULL == (ptr_temp = Change_Object_Set_Global_Ptr (ptr_ob, ptr_new)))
  {
    rysuj_obiekt ((char *)ptr_ob, COPY_PUT, 1) ;
    return FALSE ;
  }
  rysuj_obiekt ((char *)ptr_temp, COPY_PUT, 1) ;
  if (((NAGLOWEK*)ptr_temp)->atrybut == Ablok)
  {
    rysuj_obiekt (ptr_temp, XOR_PUT, 1) ;
  }
  if (ptr_new1 != NULL)
  {
    if (NULL == (ptr_temp = Add_Object_Set_Global_Ptr (&ptrs_b, ptr_new1)))
    {
      return FALSE ;
    }
    ptrsz__undo_new1_ob = ptr_temp ;
    rysuj_obiekt ((char *)ptr_temp, COPY_PUT, 1) ;
    if (((NAGLOWEK*)ptr_temp)->atrybut == Ablok)
    {
      rysuj_obiekt (ptr_temp, XOR_PUT, 1) ;
    }
  }
  b__undo = TRUE ;
  Set_Block_Change_Mode ((void *)ptr_temp, OB1CHANGE_BREAK) ;
  return TRUE ;
}

BOOL change_date_ (BLOK *ptrs_b, void *ptr_ob, void *ptr_new, void *ptr_new1)
{
    return change_date (ptrs_b, ptr_ob, ptr_new, ptr_new1);
}

static void undo_trim (void)
/*-------------------------*/
{
  void *ptr_tmp ;

  if (TRUE == b__undopl)
  {
    undo_pline () ;
    b__undopl = FALSE ;
    return ;
  }
  if (FALSE == b__undo)
  {
    return ;
  }
  b__undo = FALSE ;
  if (NULL != ptrsz__undo_new1_ob)
  {
    rysuj_obiekt ((char *)ptrsz__undo_new1_ob, COPY_PUT, 0) ;
    Erase_Object_Set_Global_Ptr (ptrsz__undo_new1_ob) ;
    ptrsz__undo_new1_ob = NULL ;
  }
  if (NULL == (ptr_tmp = Find_Object ((void *)ptrsz__undo_new_ob)))
  {
    Internal_Error (__LINE__,__FILE__);
    return ;
  }
  if (NULL == (ptr_tmp = Change_Object_Set_Global_Ptr (ptr_tmp, (void *)ptrsz__undo_old_ob)))
  {
    return ;
  }
  rysuj_obiekt ((char *)ptrsz__undo_new_ob, COPY_PUT, 0) ;
  rysuj_obiekt ((char *)ptr_tmp, COPY_PUT, 1) ;
  if (((NAGLOWEK*)ptr_tmp)->atrybut == Ablok)
  {
    rysuj_obiekt (ptr_tmp, XOR_PUT, 1) ;
  }
}

void undo_extend (void)
{
    undo_trim ();
}


void Set_Global_Ptr (void *ptr_ob)
/*------------------------------*/
{
  char  *ptrh_temp ;
  NAGLOWEK *ptrs_nag ;

  if (ADP == NULL || ADK == NULL)
  {
    return ;
  }
  ptrs_nag = (NAGLOWEK*)ptr_ob ;
  ptrh_temp = (char *)ptrs_nag ;
  if (ADP < ptrh_temp)
  {
    ADP -= ptrs_nag->n + sizeof (NAGLOWEK) ;
  }
  if (ADK < ptrh_temp)
  {
    ADK -= ptrs_nag->n + sizeof (NAGLOWEK) ;
  }
}


static void trim_line (void *ad, double x0, double y0)
/*----------------------------------------------------*/
{
  LINIA *ptrs_line, line_tmp, line_tmp1, line_tmp_w ;
  NAGLOWEK *nag ;
  BLOK * ptrs_b ;
  WIELOKAT *ptrs_w;
  SPLINE *ptrs_s;
  ELLIPSE *ptrs_e;
  ELLIPTICALARC *ptrs_ea;
  static double xsel, ysel, x, y, t1, t2 ;
  void  *ad_h,  *obbl_h ;
  BOOL b_add1, b_add2 ;
  int i, j, k;
  float df_x1, df_y1;

  SPLINE tmp_spline = Splinedef;
  double bnodes_2_tangent_org, bnodes_2_tangent;
  int node_count;
  float out_x[MaxNpts], out_y[MaxNpts];
  int numpoints;
  double xy[512];
  int verno;


  ptrs_line = (LINIA *)ad ;
  ad_h = (void *)ad ;
  df__te1 = - 1 ;    /*wstepnie df__te1, df__te2 - inicjowane sa poza odcinkiem <0, 1> */
  df__te2 = 2 ;
  xsel = x0 ;
  ysel = y0 ;
  najblizszyL_ (&xsel, &ysel, ad) ;
  if (FALSE == Check_if_Equal (ptrs_line->x1, ptrs_line->x2))
  {
    df__tsel = (xsel - ptrs_line->x1) / (ptrs_line->x2 - ptrs_line->x1) ;
  }
  else
  if (FALSE == Check_if_Equal (ptrs_line->y1, ptrs_line->y2))
  {
    df__tsel = (ysel - ptrs_line->y1) / (ptrs_line->y2 - ptrs_line->y1) ;
  }
  else
  {
    return ;	/*dla odcinka 'zerowego'*/
  }
  obiekt_tok ((char*)ADP, ADK, (char **)&nag, ONieOkreslony) ;
  while (nag != NULL)
  {
    obbl_h  = (void  *)nag ;	/*pominiecie uciecia do samego siebie*/
    if (nag->atrybut == Ablok  && ad_h != obbl_h)
    {
       switch (nag->obiekt)
       {
	 case Olinia :
	    if (0 == IntersectionLL (ptrs_line, (LINIA*)nag, &x, &y, &t1, &t2))
	    {
	      break ;  /*linie rownolegle*/
	    }
	    if (t2 >= 0 - o_male   &&   t2 <= 1 + o_male)
	    {
	      new_intersection (x, y, t1) ;
	    }
	    break ;
	case Owwielokat:
		memcpy(&line_tmp_w, ptrs_line, sizeof(line_tmp));
		ptrs_w = (WIELOKAT*)nag;
		
		df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
		df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

		for (i = 0; i < ptrs_w->lp; i += 2)
		{
			line_tmp_w.x1 = df_x1;
			line_tmp_w.y1 = df_y1;
			line_tmp_w.x2 = ptrs_w->xy[i];
			line_tmp_w.y2 = ptrs_w->xy[i + 1];

			if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2))
			{
				;  /*linie rownolegle*/
			}
			else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
			{
				new_intersection(x, y, t1);
			}

			df_x1 = line_tmp_w.x2;
			df_y1 = line_tmp_w.y2;
		}
		break;
	case Ospline:

		memcpy(&line_tmp_w, ptrs_line, sizeof(line_tmp));
		ptrs_s = (SPLINE*)nag;

#define Npts 90  //for more precission

		if (ptrs_s->npts < 5)
		{
			calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, &out_x, &out_y);

			for (i = 0; i < Npts - 1; i++)
			{
				line_tmp_w.x1 = out_x[i];
				line_tmp_w.y1 = out_y[i];
				line_tmp_w.x2 = out_x[i + 1];
				line_tmp_w.y2 = out_y[i + 1];

				if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2))
				{
					;  /*linie rownolegle*/
				}
				else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
				{
					new_intersection(x, y, t1);
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

					if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2))
					{
						;  /*linie rownolegle*/
					}
					else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
					{
						new_intersection(x, y, t1);
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

					if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2))
					{
						;  /*linie rownolegle*/
					}
					else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
					{
						new_intersection(x, y, t1);
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

					if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2))
					{
						;  /*linie rownolegle*/
					}
					else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
					{
						new_intersection(x, y, t1);
					}

				}
			}

		}
		break;
        case Oellipse:
        case Ofilledellipse:
            memcpy(&line_tmp_w, ptrs_line, sizeof(line_tmp));
            ptrs_e = (ELLIPSE*)nag;

            verno = Get_Ell_Verno () ;
            Set_Ell_Verno (250) ;
            numpoints=Get_Ellipse_Points(ptrs_e->x, ptrs_e->y, ptrs_e->rx, ptrs_e->ry, ptrs_e->angle, xy);
            Set_Ell_Verno (verno) ;

            xy[numpoints] = xy[0];
            xy[numpoints+1] = xy[1];
            numpoints+=2;

            for (i = 0; i < (numpoints-2); i+=2)
            {
                line_tmp_w.x1 = xy[i];
                line_tmp_w.y1 = xy[i+1];
                line_tmp_w.x2 = xy[i+2];
                line_tmp_w.y2 = xy[i+3];

                if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2)) { ;  /*linie rownolegle*/
                } else if (t2 >= 0 - o_male && t2 <= 1 + o_male) {
                    new_intersection(x, y, t1);
                }
            }
           break;
           case Oellipticalarc:
               memcpy(&line_tmp_w, ptrs_line, sizeof(line_tmp));
               ptrs_ea = (ELLIPTICALARC*)nag;

               verno = Get_Ell_Verno () ;
               Set_Ell_Verno (250) ;
               numpoints=Get_EllipticalArc_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, ptrs_ea->kat1, ptrs_ea->kat2, xy);
               Set_Ell_Verno (verno) ;

               for (i = 0; i < (numpoints-2); i+=2)
               {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i+1];
                   line_tmp_w.x2 = xy[i+2];
                   line_tmp_w.y2 = xy[i+3];

                   if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2)) { ;  /*linie rownolegle*/
                   } else if (t2 >= 0 - o_male && t2 <= 1 + o_male) {
                       new_intersection(x, y, t1);
                   }
               }
               break;
	 case Ookrag :
	 case Okolo :
	 case Oluk :
	 {
	    double x1, x2, y1, y2, t1, t2 ;
	    int l;

	    if (nag->obiekt == Oluk)
	    {
	      l = przeciecieLl_tt (&x1, &y1, &t1, &x2, &y2, &t2,
		    (void *)ptrs_line, (void *)nag) ;  /*t2>=t1*/
	    }
	    else
	    {
	      l = przeciecieLO_tt (&x1, &y1, &t1, &x2, &y2, &t2,
		    (void *)ptrs_line, (void *)nag) ;  /*t2>=t1*/
	    }
	    if (0 == l)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    new_intersection (x1, y1, t1) ;
	    if (l == 2)
	    {
	      new_intersection (x2, y2, t2) ;
	    }
	    break ;
	 }
	 default :
	    break ;
       }
    }
    obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony);
  }
  if (df__te1 < 0 - o_male   &&   df__te2 > 1 + o_male)
  {
    return ; 	/*obiekt zle wybrany, nie ma nic do wyciecja*/
  }
  b_add1 = FALSE ;
  b_add2 = FALSE ;
  ptrs_b = NULL ;
  memcpy (&line_tmp, ptrs_line, sizeof (line_tmp)) ;
  if (df__te1 < 0 - o_male)
  {
    line_tmp.x1 = df__xe2 ;
    line_tmp.y1 = df__ye2 ;
  }
  else
  if (df__te2 > 1 + o_male)
  {
    line_tmp.x2 = df__xe1 ;
    line_tmp.y2 = df__ye1 ;
  }
  else
  {
    memcpy (&line_tmp1, ptrs_line, sizeof (line_tmp)) ;
    ptrs_b = Parent_Block ((char  *)ptrs_line) ;
    line_tmp.x2 = df__xe1 ;
    line_tmp.y2 = df__ye1 ;
    line_tmp1.x1 = df__xe2 ;
    line_tmp1.y1 = df__ye2 ;
    if (FALSE == Check_if_Equal (line_tmp1.x1, line_tmp1.x2) ||
	FALSE == Check_if_Equal (line_tmp1.y1, line_tmp1.y2))
    {
       b_add2 = TRUE ;
    }
  }
  if (FALSE == Check_if_Equal (line_tmp.x1, line_tmp.x2) ||
      FALSE == Check_if_Equal (line_tmp.y1, line_tmp.y2))
  {
     b_add1 = TRUE ;
  }
  if (b_add1 == FALSE && b_add2 == FALSE)
  {
    return ;
  }
  if (b_add1 == FALSE && b_add2 == TRUE)
  {
    memcpy ((void*)&line_tmp, (void*)&line_tmp1, sizeof (line_tmp)) ;
    b_add1 = TRUE ;
    b_add2 = FALSE ;
  }
  change_date (ptrs_b, (void *)ptrs_line, (void *)&line_tmp,  /*ptrs_b tylko dla line_tmp1*/
     ((b_add2 == TRUE) ? (void *)&line_tmp1 : NULL)) ;
}

void new_intersection_arc (double *angle1,
			   double *angle2,
			   double a_sel,
			   double angle,
			   BOOL *b_inter1,
			   BOOL *b_inter2,
			   LUK *ptrs_arc)
/*---------------------------------------*/
{
  angle = Angle_Normal (angle) ;
  if (TRUE == Check_if_Equal (angle, a_sel) &&
      (TRUE == Check_if_Equal (angle, ptrs_arc->kat1) ||
      TRUE == Check_if_Equal (angle, ptrs_arc->kat1)))
  {
    return ;
  }
  if (*b_inter1 == TRUE && TRUE == Check_Angle (angle, *angle1, a_sel))
  {
    *angle1 = angle ;
  }
  if (*b_inter1 == FALSE && TRUE == Check_Angle (angle, ptrs_arc->kat1, a_sel))
  {
    *angle1 = angle ;
    *b_inter1 = TRUE ;
  }
  if (*b_inter2 == TRUE && TRUE == Check_Angle (angle, a_sel, *angle2))
  {
    *angle2 = angle ;
  }
  if (*b_inter2 == FALSE && TRUE == Check_Angle (angle, a_sel, ptrs_arc->kat2))
  {
    *angle2 = angle ;
    *b_inter2 = TRUE ;
  }
}


void new_intersection_circle (double *angle1,
			   double *angle2,
			   double a_sel,
			   double angle,
			   BOOL *b_inter1,
			   BOOL *b_inter2)
/*---------------------------------------*/
{
  angle = Angle_Normal (angle) ;
  if (*b_inter1 == TRUE && TRUE == Check_Angle (angle, *angle1, a_sel))
  {
    *angle1 = angle ;
  }
  if (*b_inter1 == FALSE)
  {
    *angle1 = angle ;
    *b_inter1 = TRUE ;
  }
  if (*b_inter2 == TRUE && TRUE == Check_Angle (angle, a_sel, *angle2))
  {
    *angle2 = angle ;
  }
  if (*b_inter2 == FALSE)
  {
    *angle2 = angle ;
    *b_inter2 = TRUE ;
  }
}

void new_intersection_ellipse (double *angle1,
                              double *angle2,
                              double a_sel,
                              double angle,
                              BOOL *b_inter1,
                              BOOL *b_inter2)
/*---------------------------------------*/
{

    angle = Angle_Normal (angle) ;

    if (*b_inter1 == TRUE && TRUE == Check_Angle (angle, *angle1, a_sel))
    {
        *angle1 = angle;
    }
    if (*b_inter1 == FALSE)
    {
        *angle1 = angle ;
        *b_inter1 = TRUE ;
    }
    if (*b_inter2 == TRUE && TRUE == Check_Angle (angle, a_sel, *angle2))
    {
        *angle2 = angle ;
    }
    if (*b_inter2 == FALSE)
    {
        *angle2 = angle ;
        *b_inter2 = TRUE ;
    }

}

BOOL no_extend_ellpticalarc(ELLIPTICALARC *ea, double px, double py)
{
    double df_angle;
    double df_angle1, df_angle2;

    //check if point px, py creates an angle between kat1 and kat2
    df_angle= Angle_Normal(Atan2(py-ea->y, px-ea->x));
    df_angle1=Angle_Normal(ea->angle+ea->kat1);
    df_angle2=Angle_Normal(ea->angle+ea->kat2);
    if (df_angle1 > df_angle2) df_angle2 += Pi2;
    if (df_angle1 > df_angle) df_angle += Pi2;
    if ((df_angle>df_angle1) && (df_angle<df_angle2)) return TRUE;
    return FALSE;

}

void new_intersection_ellipticalarc (double *angle1,
                               double *angle2,
                               double a_sel,
                               double angle,
                               double kat1,
                               double kat2,
                               BOOL *b_inter1,
                               BOOL *b_inter2)
/*---------------------------------------*/
{   BOOL let_angle1, let_angle2;

    angle = Angle_Normal (angle) ;

    //check if angle in range of ka1, kat2; if not - it's ignored not taken;

    if (TRUE == Check_if_Equal (angle, a_sel) &&
        (TRUE == Check_if_Equal (angle, kat1) ||
         TRUE == Check_if_Equal (angle, kat1)))
    {
        return ;
    }

    if (*b_inter1 == TRUE && TRUE == Check_Angle (angle, *angle1, a_sel))
    {
        *angle1 = angle ;
    }
    if (*b_inter1 == FALSE && TRUE == Check_Angle (angle, kat1, a_sel))
    {
        *angle1 = angle ;
        *b_inter1 = TRUE ;
    }
    if (*b_inter2 == TRUE && TRUE == Check_Angle (angle, a_sel, *angle2))
    {
        *angle2 = angle ;
    }
    if (*b_inter2 == FALSE && TRUE == Check_Angle (angle, a_sel, kat2))
    {
        *angle2 = angle ;
        *b_inter2 = TRUE ;
    }
}


static void trim_arc (void *ad, double x0, double y0)
/*--------------------------------------------------*/
{
  LUK *ptrs_arc, arc_tmp, arc_tmp1 ;
  NAGLOWEK *nag ;
  BLOK * ptrs_b ;
  LINIA line_tmp_w = Ldef;
  WIELOKAT *ptrs_w;
  SPLINE *ptrs_s;
    ELLIPSE *ptrs_e;
    ELLIPTICALARC *ptrs_ea;
  double xsel, ysel, a_sel, angle1, angle2 ;
  void  *ad_h,  *obbl_h ;
  BOOL b_inter1, b_inter2 ;
  BOOL b_add1, b_add2 ;
  float df_x1, df_y1;
  int i;

  b_inter1 = b_inter2 = FALSE ;
  ptrs_arc = (LUK *)ad ;
  ad_h = (void *)ad ;
  xsel = x0 ;
  ysel = y0 ;
  najblizszyl_ (&xsel, &ysel, ad) ;
  a_sel = Angle_Normal (Atan2 (ysel - ptrs_arc->y, xsel - ptrs_arc->x)) ;
  obiekt_tok ((char*)ADP, ADK, (char **)&nag, ONieOkreslony) ;
  while (nag != NULL)
  {
    obbl_h  = (void  *)nag ;	/*pominiecie uciecia do samego siebie*/
    if (nag->atrybut == Ablok  && ad_h != obbl_h)
    {
       switch (nag->obiekt)
       {
	 case Olinia :
	 {
	    double x1, x2, y1, y2, t1, t2, angle ;
	    int l;

	    l = przeciecieLl_tt (&x1, &y1, &t1, &x2, &y2, &t2,
		    (void *)nag, (void *)ptrs_arc) ;  /*t2>=t1*/
	    if (l == 0)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
	    {
	      angle = Atan2 (y1 - ptrs_arc->y, x1 - ptrs_arc->x) ;
	      new_intersection_arc (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc) ;
	    }
	    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
	    {
	      angle = Atan2 (y2 - ptrs_arc->y, x2 - ptrs_arc->x) ;
	      new_intersection_arc (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc) ;
	    }
	    break ;
	 }

	case Owwielokat:
	{

		double x1, x2, y1, y2, t1, t2, angle;
		int l;

		ptrs_w = (WIELOKAT*)nag;

		df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
		df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

		for (i = 0; i < ptrs_w->lp; i += 2)
		{
			line_tmp_w.x1 = df_x1;
			line_tmp_w.y1 = df_y1;
			line_tmp_w.x2 = ptrs_w->xy[i];
			line_tmp_w.y2 = ptrs_w->xy[i + 1];

			l = przeciecieLl_tt(&x1, &y1, &t1, &x2, &y2, &t2,
				&line_tmp_w, (void *)ptrs_arc);  /*t2>=t1*/
			if (l == 0)
			{
				;  /*linie rownolegle*/
			}
			else
			{
				if (t1 >= 0 - o_male && t1 <= 1 + o_male)
				{
					angle = Atan2(y1 - ptrs_arc->y, x1 - ptrs_arc->x);
					new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
				}
				if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
				{
					angle = Atan2(y2 - ptrs_arc->y, x2 - ptrs_arc->x);
					new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
				}
			}

			df_x1 = line_tmp_w.x2;
			df_y1 = line_tmp_w.y2;
		}
		break;
	}
	case Ospline:
	{
		int i, j, k;
		float out_x[MaxNpts], out_y[MaxNpts];
		double x1, x2, y1, y2, t1, t2, angle;
		int l;

		SPLINE tmp_spline = Splinedef;
		double bnodes_2_tangent_org, bnodes_2_tangent;
		int node_count;

		ptrs_s = (SPLINE*)nag;

#define Npts 90  //for more precission
		if (ptrs_s->npts < 5)
		{
			calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, &out_x, &out_y);

			for (i = 0; i < Npts - 1; i++)
			{
				line_tmp_w.x1 = out_x[i];
				line_tmp_w.y1 = out_y[i];
				line_tmp_w.x2 = out_x[i + 1];
				line_tmp_w.y2 = out_y[i + 1];

				l = przeciecieLl_tt(&x1, &y1, &t1, &x2, &y2, &t2,
					&line_tmp_w, (void*)ptrs_arc);  /*t2>=t1*/
				if (l == 0)
				{
					;  /*linie rownolegle*/
				}
				else
				{
					if (t1 >= 0 - o_male && t1 <= 1 + o_male)
					{
						angle = Atan2(y1 - ptrs_arc->y, x1 - ptrs_arc->x);
						new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
					}
					if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
					{
						angle = Atan2(y2 - ptrs_arc->y, x2 - ptrs_arc->x);
						new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
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

					l = przeciecieLl_tt(&x1, &y1, &t1, &x2, &y2, &t2,
						&line_tmp_w, (void*)ptrs_arc);  /*t2>=t1*/
					if (l == 0)
					{
						;  /*linie rownolegle*/
					}
					else
					{
						if (t1 >= 0 - o_male && t1 <= 1 + o_male)
						{
							angle = Atan2(y1 - ptrs_arc->y, x1 - ptrs_arc->x);
							new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
						}
						if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							angle = Atan2(y2 - ptrs_arc->y, x2 - ptrs_arc->x);
							new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
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

					l = przeciecieLl_tt(&x1, &y1, &t1, &x2, &y2, &t2,
						&line_tmp_w, (void*)ptrs_arc);  /*t2>=t1*/
					if (l == 0)
					{
						;  /*linie rownolegle*/
					}
					else
					{
						if (t1 >= 0 - o_male && t1 <= 1 + o_male)
						{
							angle = Atan2(y1 - ptrs_arc->y, x1 - ptrs_arc->x);
							new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
						}
						if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							angle = Atan2(y2 - ptrs_arc->y, x2 - ptrs_arc->x);
							new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
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

					l = przeciecieLl_tt(&x1, &y1, &t1, &x2, &y2, &t2,
						&line_tmp_w, (void*)ptrs_arc);  /*t2>=t1*/
					if (l == 0)
					{
						;  /*linie rownolegle*/
					}
					else
					{
						if (t1 >= 0 - o_male && t1 <= 1 + o_male)
						{
							angle = Atan2(y1 - ptrs_arc->y, x1 - ptrs_arc->x);
							new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
						}
						if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							angle = Atan2(y2 - ptrs_arc->y, x2 - ptrs_arc->x);
							new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
						}
					}
				}
			}
		}
		break;
	}
           case Oellipse:
           case Ofilledellipse:
           {
               int numpoints;
               double xy[512];
               int verno;
               int i, j, k;
               double x1, x2, y1, y2, t1, t2, angle;
               int l;

               ptrs_e = (ELLIPSE *) nag;

               verno = Get_Ell_Verno();
               Set_Ell_Verno(250);
               numpoints = Get_Ellipse_Points(ptrs_e->x, ptrs_e->y, ptrs_e->rx, ptrs_e->ry, ptrs_e->angle, xy);
               Set_Ell_Verno(verno);

               xy[numpoints] = xy[0];
               xy[numpoints + 1] = xy[1];
               numpoints += 2;

               for (i = 0; i < (numpoints - 2); i += 2) {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i + 1];
                   line_tmp_w.x2 = xy[i + 2];
                   line_tmp_w.y2 = xy[i + 3];

                   l = przeciecieLl_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                       &line_tmp_w, (void *) ptrs_arc);  /*t2>=t1*/
                   if (l == 0) { ;  /*linie rownolegle*/
                   } else {
                       if (t1 >= 0 - o_male && t1 <= 1 + o_male) {
                           angle = Atan2(y1 - ptrs_arc->y, x1 - ptrs_arc->x);
                           new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
                       }
                       if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male) {
                           angle = Atan2(y2 - ptrs_arc->y, x2 - ptrs_arc->x);
                           new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
                       }
                   }
               }

               break;
           }
           case Oellipticalarc:
           {
               int numpoints;
               double xy[512];
               int verno;
               int i, j, k;
               double x1, x2, y1, y2, t1, t2, angle;
               int l;

               ptrs_ea = (ELLIPTICALARC *) nag;

               verno = Get_Ell_Verno();
               Set_Ell_Verno(250);
               numpoints = Get_EllipticalArc_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle,
                                                    ptrs_ea->kat1, ptrs_ea->kat2, xy);
               Set_Ell_Verno(verno);

               for (i = 0; i < (numpoints - 2); i += 2) {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i + 1];
                   line_tmp_w.x2 = xy[i + 2];
                   line_tmp_w.y2 = xy[i + 3];

                   l = przeciecieLl_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                       &line_tmp_w, (void *) ptrs_arc);  /*t2>=t1*/
                   if (l == 0) { ;  /*linie rownolegle*/
                   } else {
                       if (t1 >= 0 - o_male && t1 <= 1 + o_male) {
                           angle = Atan2(y1 - ptrs_arc->y, x1 - ptrs_arc->x);
                           new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
                       }
                       if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male) {
                           angle = Atan2(y2 - ptrs_arc->y, x2 - ptrs_arc->x);
                           new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
                       }
                   }
               }
               break;
           }
	 case Ookrag :
	 case Okolo :
	 case Oluk :
	 {
	    double x1, x2, y1, y2, angle ;
	    int l;

	    if (nag->obiekt == Oluk)
	    {
	      l = przecieciell_tr ((void *)nag, (void *)ptrs_arc,
		&x1, &y1, &x2, &y2) ;
	    }
	    else
	    {
	      l = przeciecieOl_tr ((void *)nag, (void *)ptrs_arc,
		&x1, &y1, &x2, &y2) ;
	    }
	    if (l == 0)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    angle = Atan2 (y1 - ptrs_arc->y, x1 - ptrs_arc->x) ;
	    new_intersection_arc (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc) ;
	    if (l == 2)
	    {
	      angle = Atan2 (y2 - ptrs_arc->y, x2 - ptrs_arc->x) ;
	      new_intersection_arc (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc) ;
	    }
	    break ;
	 }
	 default :
	    break ;
       }
    }
    obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony);
  }
  if (b_inter1 == FALSE && b_inter2 == FALSE)
  {
    return ; 	/*obiekt zle wybrany, nie ma nic do wyciecja*/
  }
  ptrs_b = NULL ;
  b_add1 = FALSE ;
  b_add2 = FALSE ;
  memcpy (&arc_tmp, ptrs_arc, sizeof (arc_tmp)) ;
  if (b_inter1 == FALSE)
  {
    arc_tmp.kat1 = angle2 ;
  }
  else
  if (b_inter2 == FALSE)
  {
    arc_tmp.kat2 = angle1 ;
  }
  else
  {
    memcpy (&arc_tmp1, ptrs_arc, sizeof (arc_tmp)) ;
    ptrs_b = Parent_Block ((char  *)ptrs_arc) ;
    arc_tmp.kat2 = angle1 ;
    arc_tmp1.kat1 = angle2 ;
    if (FALSE == Check_if_Equal (Angle_Normal (arc_tmp1.kat1),
				Angle_Normal (arc_tmp1.kat2)))
    {
      b_add2 = TRUE ;
    }
  }
  if (FALSE == Check_if_Equal (Angle_Normal (arc_tmp.kat1),
			      Angle_Normal (arc_tmp.kat2)))
  {
    b_add1 = TRUE ;
  }
  if (b_add1 == FALSE && b_add2 == TRUE)
  {
     memcpy (&arc_tmp, &arc_tmp1, sizeof (arc_tmp)) ;
     b_add1 = TRUE ;
     b_add2 = FALSE ;
  }
  if (b_add1 == FALSE && b_add2 == FALSE)
  {
    return ;
  }
  change_date (ptrs_b, (void *)ptrs_arc, (void *)&arc_tmp,
     ((b_add1 == TRUE && b_add2 == TRUE) ? (void *)&arc_tmp1 : NULL)) ;
}


static void trim_circle (void *ad, double x0, double y0)
/*-----------------------------------------------------*/
{
  OKRAG *ptrs_circle ;
  LUK arc_tmp =ldef ;
  NAGLOWEK *nag ;
  BLOK * ptrs_b ;
  double xsel, ysel, a_sel, angle1, angle2 ;
  void  *ad_h,  *obbl_h ;
  BOOL b_inter1, b_inter2 ;
  unsigned len ;
  WIELOKAT *ptrs_w;
  SPLINE *ptrs_s;
    ELLIPSE *ptrs_e;
    ELLIPTICALARC *ptrs_ea;
  LINIA line_tmp_w = Ldef;
  float df_x1, df_y1;
  int i;

  b_inter1 = b_inter2 = FALSE ;
  ptrs_circle = (OKRAG *)ad ;
  ad_h = (void *)ad ;
  xsel = x0 ;
  ysel = y0 ;
  najblizszyO_ (&xsel, &ysel, ad) ;
  a_sel = Angle_Normal (Atan2 (ysel - ptrs_circle->y, xsel - ptrs_circle->x)) ;
  obiekt_tok ((char*)ADP, ADK, (char **)&nag, ONieOkreslony) ;
  while (nag != NULL)
  {
    obbl_h  = (void  *)nag ;	/*pominiecie uciecia do samego siebie*/
    if (nag->atrybut == Ablok  && ad_h != obbl_h)
    {
       switch (nag->obiekt)
       {
	 case Olinia :
	 {
	    double x1, x2, y1, y2, t1, t2, angle ;
	    int l;

	    l = przeciecieLO_tt (&x1, &y1, &t1, &x2, &y2, &t2,
		    (void *)nag, (void *)ptrs_circle) ;  /*t2>=t1*/
	    if (l == 0)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
	    {
	      angle = Atan2 (y1 - ptrs_circle->y, x1 - ptrs_circle->x) ;
	      new_intersection_circle (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2) ;
	    }
	    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
	    {
	      angle = Atan2 (y2 - ptrs_circle->y, x2 - ptrs_circle->x) ;
	      new_intersection_circle (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2) ;
	    }
	    break ;
	 }
	case Owwielokat:
	{

		double x1, x2, y1, y2, t1, t2, angle;
		int l;

		ptrs_w = (WIELOKAT*)nag;

		df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
		df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

		for (i = 0; i < ptrs_w->lp; i += 2)
		{
			line_tmp_w.x1 = df_x1;
			line_tmp_w.y1 = df_y1;
			line_tmp_w.x2 = ptrs_w->xy[i];
			line_tmp_w.y2 = ptrs_w->xy[i + 1];

			l = przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2,
				&line_tmp_w, (void *)ptrs_circle);  /*t2>=t1*/
			if (l == 0)
			{
				;  /*linie rownolegle*/
			}
			else
			{
				if (t1 >= 0 - o_male && t1 <= 1 + o_male)
				{
					angle = Atan2(y1 - ptrs_circle->y, x1 - ptrs_circle->x);
					new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
				}
				if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
				{
					angle = Atan2(y2 - ptrs_circle->y, x2 - ptrs_circle->x);
					new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
				}
			}

			df_x1 = line_tmp_w.x2;
			df_y1 = line_tmp_w.y2;
		}
		break;
	}
	case Ospline:
	{
		int i,j,k;
		float out_x[MaxNpts], out_y[MaxNpts];
		double x1, x2, y1, y2, t1, t2, angle;
		int l;

		SPLINE tmp_spline = Splinedef;
		double bnodes_2_tangent_org, bnodes_2_tangent;
		int node_count;

		ptrs_s = (SPLINE*)nag;

#define Npts 90  //for more precission

		if (ptrs_s->npts < 5)
		{
			calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, &out_x, &out_y);

			for (i = 0; i < Npts - 1; i++)
			{
				line_tmp_w.x1 = out_x[i];
				line_tmp_w.y1 = out_y[i];
				line_tmp_w.x2 = out_x[i + 1];
				line_tmp_w.y2 = out_y[i + 1];

				l = przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2,
					&line_tmp_w, (void*)ptrs_circle);  /*t2>=t1*/
				if (l == 0)
				{
					;  /*linie rownolegle*/
				}
				else
				{
					if (t1 >= 0 - o_male && t1 <= 1 + o_male)
					{
						angle = Atan2(y1 - ptrs_circle->y, x1 - ptrs_circle->x);
						new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
					}
					if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
					{
						angle = Atan2(y2 - ptrs_circle->y, x2 - ptrs_circle->x);
						new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
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

					l = przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2,
						&line_tmp_w, (void*)ptrs_circle);  /*t2>=t1*/
					if (l == 0)
					{
						;  /*linie rownolegle*/
					}
					else
					{
						if (t1 >= 0 - o_male && t1 <= 1 + o_male)
						{
							angle = Atan2(y1 - ptrs_circle->y, x1 - ptrs_circle->x);
							new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
						}
						if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							angle = Atan2(y2 - ptrs_circle->y, x2 - ptrs_circle->x);
							new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
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

					l = przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2,
						&line_tmp_w, (void*)ptrs_circle);  /*t2>=t1*/
					if (l == 0)
					{
						;  /*linie rownolegle*/
					}
					else
					{
						if (t1 >= 0 - o_male && t1 <= 1 + o_male)
						{
							angle = Atan2(y1 - ptrs_circle->y, x1 - ptrs_circle->x);
							new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
						}
						if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							angle = Atan2(y2 - ptrs_circle->y, x2 - ptrs_circle->x);
							new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
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

					l = przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2,
						&line_tmp_w, (void*)ptrs_circle);  /*t2>=t1*/
					if (l == 0)
					{
						;  /*linie rownolegle*/
					}
					else
					{
						if (t1 >= 0 - o_male && t1 <= 1 + o_male)
						{
							angle = Atan2(y1 - ptrs_circle->y, x1 - ptrs_circle->x);
							new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
						}
						if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							angle = Atan2(y2 - ptrs_circle->y, x2 - ptrs_circle->x);
							new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
						}
					}

				}
			}

		}
		break;
	}

           case Oellipse:
           case Ofilledellipse:
           {
               int numpoints;
               double xy[512];
               int verno;
               int i, j, k;
               double x1, x2, y1, y2, t1, t2, angle;
               int l;

               ptrs_e = (ELLIPSE *) nag;

               verno = Get_Ell_Verno();
               Set_Ell_Verno(250);
               numpoints = Get_Ellipse_Points(ptrs_e->x, ptrs_e->y, ptrs_e->rx, ptrs_e->ry, ptrs_e->angle, xy);
               Set_Ell_Verno(verno);

               xy[numpoints] = xy[0];
               xy[numpoints + 1] = xy[1];
               numpoints += 2;

               for (i = 0; i < (numpoints - 2); i += 2) {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i + 1];
                   line_tmp_w.x2 = xy[i + 2];
                   line_tmp_w.y2 = xy[i + 3];

                   l = przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                       &line_tmp_w, (void*)ptrs_circle);  /*t2>=t1*/
                   if (l == 0)
                   {
                       ;  /*linie rownolegle*/
                   }
                   else
                   {
                       if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                       {
                           angle = Atan2(y1 - ptrs_circle->y, x1 - ptrs_circle->x);
                           new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                       }
                       if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                       {
                           angle = Atan2(y2 - ptrs_circle->y, x2 - ptrs_circle->x);
                           new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                       }
                   }
               }

               break;
           }
           case Oellipticalarc:
           {
               int numpoints;
               double xy[512];
               int verno;
               int i, j, k;
               double x1, x2, y1, y2, t1, t2, angle;
               int l;


               ptrs_ea = (ELLIPTICALARC *) nag;

               verno = Get_Ell_Verno();
               Set_Ell_Verno(250);
               numpoints = Get_EllipticalArc_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle,
                                                    ptrs_ea->kat1, ptrs_ea->kat2, xy);
               Set_Ell_Verno(verno);

               for (i = 0; i < (numpoints - 2); i += 2) {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i + 1];
                   line_tmp_w.x2 = xy[i + 2];
                   line_tmp_w.y2 = xy[i + 3];

                   l = przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                       &line_tmp_w, (void*)ptrs_circle);  /*t2>=t1*/
                   if (l == 0)
                   {
                       ;  /*linie rownolegle*/
                   }
                   else
                   {
                       if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                       {
                           angle = Atan2(y1 - ptrs_circle->y, x1 - ptrs_circle->x);
                           new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                       }
                       if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                       {
                           angle = Atan2(y2 - ptrs_circle->y, x2 - ptrs_circle->x);
                           new_intersection_circle(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                       }
                   }
               }
               break;
           }

	 case Ookrag :
	 case Okolo :
	 case Oluk :
	 {
	    double x1, x2, y1, y2, angle ;
	    int l;

	    if (nag->obiekt == Oluk)
	    {
	      l = przeciecieOl_tr ((void *)ptrs_circle, (void *)nag,
		&x1, &y1, &x2, &y2) ;
	    }
	    else
	    {
	      l = przeciecieOO_ex (&x1, &y1, &x2, &y2,
		(void *)nag, (void *)ptrs_circle) ;
	    }
	    if (l == 0)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    angle = Atan2 (y1 - ptrs_circle->y, x1 - ptrs_circle->x) ;
	    new_intersection_circle (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2) ;
	    if (l == 2)
	    {
	      angle = Atan2 (y2 - ptrs_circle->y, x2 - ptrs_circle->x) ;
	      new_intersection_circle (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2) ;
	    }
	    break ;
	 }
	 default :
	    break ;
       }
    }
    obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony);
  }
  if (b_inter1 != TRUE || b_inter2 != TRUE ||
	TRUE == Check_if_Equal (angle1, angle2))
  {
    return ; 	/*obiekt zle wybrany, nie ma nic do wyciecja*/
  }
  ptrs_b = NULL ;
  len = arc_tmp.n ;
  memcpy (&arc_tmp, ptrs_circle, sizeof (NAGLOWEK)) ;
  arc_tmp.n = len ;
  arc_tmp.r = ptrs_circle->r ;
  arc_tmp.x = ptrs_circle->x ;
  arc_tmp.y = ptrs_circle->y ;
  arc_tmp.warstwa = ptrs_circle->warstwa ;
  arc_tmp.kolor = ptrs_circle->kolor ;
  arc_tmp.typ = ptrs_circle->typ ;
  arc_tmp.kat1 = angle2 ;
  arc_tmp.obiekt = Oluk ;
  arc_tmp.kat2 = angle1 ;
  change_date (ptrs_b, (void *)ptrs_circle, (void *)&arc_tmp, NULL) ;
}


static void trim_ellipse (void *ad, double x0, double y0)
{
    ELLIPSE *ptrs_ellipse ;
    ELLIPTICALARC elarc_tmp =eldef ;
    NAGLOWEK *nag ;
    BLOK * ptrs_b ;
    double xsel, ysel, a_sel, angle1, angle2 ;
    void  *ad_h,  *obbl_h ;
    BOOL b_inter1, b_inter2 ;
    unsigned len ;
    WIELOKAT *ptrs_w;
    SPLINE *ptrs_s;
    ELLIPSE *ptrs_e;
    ELLIPTICALARC *ptrs_ea;
    LINIA line_tmp_w = Ldef;
    float df_x1, df_y1;
    int i;

    b_inter1 = b_inter2 = FALSE ;
    ptrs_ellipse = (ELLIPSE *)ad ;
    ad_h = (void *)ad ;
    xsel = x0 ;
    ysel = y0 ;
    najblizszyO_ (&xsel, &ysel, ad) ;
    a_sel = Angle_Normal (Atan2 (ysel - ptrs_ellipse->y, xsel - ptrs_ellipse->x)) ;
    obiekt_tok ((char*)ADP, ADK, (char **)&nag, ONieOkreslony) ;
    while (nag != NULL)
    {
        obbl_h  = (void  *)nag ;	//pominiecie uciecia do samego siebie
        if (nag->atrybut == Ablok  && ad_h != obbl_h)
        {
            switch (nag->obiekt)
            {
                case Olinia :
                {
                    double x1, x2, y1, y2, t1, t2, angle ;
                    int l;

                    l = przeciecieLE_tt (&x1, &y1, &t1, &x2, &y2, &t2,
                                         (void *)nag, (void *)ptrs_ellipse) ;  //t2>=t1
                    if (l == 0)
                    {
                        break ;  //brak przeciecia
                    }
                    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                    {
                        angle = Atan2 (y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x) ;
                        new_intersection_ellipse (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2) ;
                    }
                    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                    {
                        angle = Atan2 (y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x) ;
                        new_intersection_ellipse (&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2) ;
                    }
                    break ;
                }
                case Owwielokat:
                {

                    double x1, x2, y1, y2, t1, t2, angle;
                    int l;

                    ptrs_w = (WIELOKAT*)nag;

                    df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
                    df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

                    for (i = 0; i < ptrs_w->lp; i += 2)
                    {
                        line_tmp_w.x1 = df_x1;
                        line_tmp_w.y1 = df_y1;
                        line_tmp_w.x2 = ptrs_w->xy[i];
                        line_tmp_w.y2 = ptrs_w->xy[i + 1];

                        l = przeciecieLE_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                            &line_tmp_w, (void *)ptrs_ellipse);  //t2>=t1
                        if (l == 0)
                        {
                            ;  //linie rownolegle
                        }
                        else
                        {
                            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                            {
                                angle = Atan2(y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x);
                                new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                            }
                            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                            {
                                angle = Atan2(y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x);
                                new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                            }
                        }

                        df_x1 = line_tmp_w.x2;
                        df_y1 = line_tmp_w.y2;
                    }
                    break;
                }
                case Ospline:
                {
                    int i,j,k;
                    float out_x[MaxNpts], out_y[MaxNpts];
                    double x1, x2, y1, y2, t1, t2, angle;
                    int l;

                    SPLINE tmp_spline = Splinedef;
                    double bnodes_2_tangent_org, bnodes_2_tangent;
                    int node_count;

                    ptrs_s = (SPLINE*)nag;

#define Npts 90  //for more precission

                    if (ptrs_s->npts < 5)
                    {
                        calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, &out_x, &out_y);

                        for (i = 0; i < Npts - 1; i++)
                        {
                            line_tmp_w.x1 = out_x[i];
                            line_tmp_w.y1 = out_y[i];
                            line_tmp_w.x2 = out_x[i + 1];
                            line_tmp_w.y2 = out_y[i + 1];

                            l = przeciecieLE_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                                &line_tmp_w, (void*)ptrs_ellipse);  //t2>=t1
                            if (l == 0)
                            {
                                ;  //linie rownolegle
                            }
                            else
                            {
                                if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                                {
                                    angle = Atan2(y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x);
                                    new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                                }
                                if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                                {
                                    angle = Atan2(y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x);
                                    new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
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

                                l = przeciecieLE_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                                    &line_tmp_w, (void*)ptrs_ellipse);  //t2>=t1
                                if (l == 0)
                                {
                                    ;  //linie rownolegle
                                }
                                else
                                {
                                    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                                    {
                                        angle = Atan2(y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x);
                                        new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                                    }
                                    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                                    {
                                        angle = Atan2(y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x);
                                        new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
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

                                l = przeciecieLE_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                                    &line_tmp_w, (void*)ptrs_ellipse);  //t2>=t1
                                if (l == 0)
                                {
                                    ;  //linie rownolegle
                                }
                                else
                                {
                                    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                                    {
                                        angle = Atan2(y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x);
                                        new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                                    }
                                    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                                    {
                                        angle = Atan2(y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x);
                                        new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
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

                                l = przeciecieLE_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                                    &line_tmp_w, (void*)ptrs_ellipse);  //t2>=t1
                                if (l == 0)
                                {
                                    ;  //linie rownolegle
                                }
                                else
                                {
                                    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                                    {
                                        angle = Atan2(y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x);
                                        new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                                    }
                                    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                                    {
                                        angle = Atan2(y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x);
                                        new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                                    }
                                }

                            }
                        }

                    }
                    break;
                }
                case Oellipse:
                case Ofilledellipse:
                {
                    int numpoints;
                    double xy[512];
                    int verno;
                    int i, j, k;
                    double x1, x2, y1, y2, t1, t2, angle;
                    int l;

                    ptrs_e = (ELLIPSE *) nag;

                    verno = Get_Ell_Verno();
                    Set_Ell_Verno(250);
                    numpoints = Get_Ellipse_Points(ptrs_e->x, ptrs_e->y, ptrs_e->rx, ptrs_e->ry, ptrs_e->angle, xy);
                    Set_Ell_Verno(verno);

                    xy[numpoints] = xy[0];
                    xy[numpoints + 1] = xy[1];
                    numpoints += 2;

                    for (i = 0; i < (numpoints - 2); i += 2) {
                        line_tmp_w.x1 = xy[i];
                        line_tmp_w.y1 = xy[i + 1];
                        line_tmp_w.x2 = xy[i + 2];
                        line_tmp_w.y2 = xy[i + 3];

                        l = przeciecieLE_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                            &line_tmp_w, (void*)ptrs_ellipse);  //t2>=t1
                        if (l == 0)
                        {
                            ;  //linie rownolegle
                        }
                        else
                        {
                            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                            {
                                angle = Atan2(y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x);
                                new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                            }
                            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                            {
                                angle = Atan2(y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x);
                                new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                            }
                        }
                    }

                    break;
                }
                case Oellipticalarc:
                {
                    int numpoints;
                    double xy[512];
                    int verno;
                    int i, j, k;
                    double x1, x2, y1, y2, t1, t2, angle;
                    int l;

                    ptrs_ea = (ELLIPTICALARC *) nag;

                    verno = Get_Ell_Verno();
                    Set_Ell_Verno(250);
                    numpoints = Get_EllipticalArc_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle,
                                                         ptrs_ea->kat1, ptrs_ea->kat2, xy);
                    Set_Ell_Verno(verno);

                    for (i = 0; i < (numpoints - 2); i += 2) {
                        line_tmp_w.x1 = xy[i];
                        line_tmp_w.y1 = xy[i + 1];
                        line_tmp_w.x2 = xy[i + 2];
                        line_tmp_w.y2 = xy[i + 3];

                        l = przeciecieLE_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                            &line_tmp_w, (void*)ptrs_ellipse);  //t2>=t1
                        if (l == 0)
                        {
                            ;  //linie rownolegle
                        }
                        else
                        {
                            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                            {
                                angle = Atan2(y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x);
                                new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                            }
                            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                            {
                                angle = Atan2(y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x);
                                new_intersection_ellipse(&angle1, &angle2, a_sel, angle,  &b_inter1, &b_inter2);
                            }
                        }
                    }
                    break;
                }

                case Ookrag :
                case Okolo :
                case Oluk :
                {
                    double x1, x2, y1, y2, t1, t2, angle ;
                    double df_xc, df_yc, df_r, df_kat1, df_kat2;
                    double df_dangle, df_elangle;
                    int i__Ell_Verno;
                    int l;

                    i__Ell_Verno=Get_Ell_Verno();

                    if (nag->obiekt == Oluk)
                    {
                        df_xc=((LUK*)nag)->x;
                        df_yc=((LUK*)nag)->y;
                        df_r=((LUK*)nag)->r;
                        df_kat1=((LUK*)nag)->kat1;
                        df_kat2=((LUK*)nag)->kat2;

                        if (df_kat2<df_kat1) df_kat2+=Pi2;
                        df_dangle = (df_kat2-df_kat1) / i__Ell_Verno;
                        df_elangle = df_kat1;
                    } else{
                        df_xc=((OKRAG*)nag)->x;
                        df_yc=((OKRAG*)nag)->y;
                        df_r=((OKRAG*)nag)->r;
                        df_dangle = Pi2 / i__Ell_Verno;
                        df_elangle = 0.0;
                    }


                    line_tmp_w.x1 = df_xc + df_r * cos(df_elangle);
                    line_tmp_w.y1 = df_yc + df_r * sin(df_elangle);

                    df_elangle += df_dangle;

                    for (i = 0; i < i__Ell_Verno; i++, df_elangle += df_dangle)
                    {
                        line_tmp_w.x2 = df_xc + df_r * cos(df_elangle);
                        line_tmp_w.y2 = df_yc + df_r * sin(df_elangle);

                        l = przeciecieLE_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                            &line_tmp_w, (void*)ptrs_ellipse);  //t2>=t1

                        line_tmp_w.x1 = line_tmp_w.x2;
                        line_tmp_w.y1 = line_tmp_w.y2;

                        if (l == 0)
                        {
                            ;  //linie rownolegle
                        }
                        else
                        {
                            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                            {
                                angle = Atan2(y1 - ptrs_ellipse->y, x1 - ptrs_ellipse->x);
                                new_intersection_ellipse(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2);
                            }
                            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                            {
                                angle = Atan2(y2 - ptrs_ellipse->y, x2 - ptrs_ellipse->x);
                                new_intersection_ellipse(&angle1, &angle2, a_sel, angle,  &b_inter1, &b_inter2);
                            }
                        }
                    }
                    break ;
                }
                default :
                    break ;
            }
        }
        obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony);
    }
    if (b_inter1 != TRUE || b_inter2 != TRUE ||
        TRUE == Check_if_Equal (angle1, angle2))
    {
        return ; 	//obiekt zle wybrany, nie ma nic do wyciecja
    }
    ptrs_b = NULL ;
    len = elarc_tmp.n ;
    memcpy (&elarc_tmp, ptrs_ellipse, sizeof (NAGLOWEK)) ;
    elarc_tmp.n = len ;
    elarc_tmp.rx = ptrs_ellipse->rx ;
    elarc_tmp.ry = ptrs_ellipse->ry ;
    elarc_tmp.x = ptrs_ellipse->x ;
    elarc_tmp.y = ptrs_ellipse->y ;
    elarc_tmp.warstwa = ptrs_ellipse->warstwa ;
    elarc_tmp.kolor = ptrs_ellipse->kolor ;
    elarc_tmp.typ = ptrs_ellipse->typ ;
    elarc_tmp.obiekt = Oellipticalarc ;
    elarc_tmp.angle = ptrs_ellipse->angle;
    elarc_tmp.kat1 = Angle_Normal(angle2 - ptrs_ellipse->angle);
    elarc_tmp.kat2 = Angle_Normal(angle1 - ptrs_ellipse->angle);
    change_date (ptrs_b, (void *)ptrs_ellipse, (void *)&elarc_tmp, NULL) ;
}


static void trim_ellipticalarc (void *ad, double x0, double y0)
{
    ELLIPTICALARC *ptrs_ellipticalarc ;
    ELLIPTICALARC elarc_tmp =eldef,  elarc_tmp1 =eldef;
    NAGLOWEK *nag ;
    BLOK * ptrs_b ;
    double xsel, ysel, a_sel, angle1, angle2, kat1, kat2 ;
    void  *ad_h,  *obbl_h ;
    BOOL b_inter1, b_inter2 ;
    BOOL b_add1, b_add2 ;
    unsigned len ;
    WIELOKAT *ptrs_w;
    SPLINE *ptrs_s;
    ELLIPSE *ptrs_e;
    ELLIPTICALARC *ptrs_ea;
    LINIA line_tmp_w = Ldef;
    float df_x1, df_y1;
    int i;

    b_inter1 = b_inter2 = FALSE ;
    ptrs_ellipticalarc = (ELLIPTICALARC *)ad ;
    kat1= Angle_Normal(ptrs_ellipticalarc->kat1+ptrs_ellipticalarc->angle);
    kat2= Angle_Normal(ptrs_ellipticalarc->kat2+ptrs_ellipticalarc->angle);
    ad_h = (void *)ad ;
    xsel = x0 ;
    ysel = y0 ;
    najblizszyO_ (&xsel, &ysel, ad) ;
    a_sel = Angle_Normal (Atan2 (ysel - ptrs_ellipticalarc->y, xsel - ptrs_ellipticalarc->x)) ;
    obiekt_tok ((char*)ADP, ADK, (char **)&nag, ONieOkreslony) ;
    while (nag != NULL)
    {
        obbl_h  = (void  *)nag ;	//pominiecie uciecia do samego siebie
        if (nag->atrybut == Ablok  && ad_h != obbl_h)
        {
            switch (nag->obiekt)
            {
                case Olinia :
                {
                    double x1, x2, y1, y2, t1, t2, angle ;
                    int l;

                    l = przeciecieLEA_tt (&x1, &y1, &t1, &x2, &y2, &t2,
                                         (void *)nag, (void *)ptrs_ellipticalarc) ;  //t2>=t1
                    if (l == 0)
                    {
                        break ;  //brak przeciecia
                    }
                    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                    {
                        angle = Atan2 (y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x) ;
                        new_intersection_ellipticalarc (&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2) ;
                    }
                    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                    {
                        angle = Atan2 (y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x) ;
                        new_intersection_ellipticalarc (&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2) ;
                    }
                    break ;
                }
                case Owwielokat:
                {

                    double x1, x2, y1, y2, t1, t2, angle;
                    int l;

                    ptrs_w = (WIELOKAT*)nag;

                    df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
                    df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

                    for (i = 0; i < ptrs_w->lp; i += 2)
                    {
                        line_tmp_w.x1 = df_x1;
                        line_tmp_w.y1 = df_y1;
                        line_tmp_w.x2 = ptrs_w->xy[i];
                        line_tmp_w.y2 = ptrs_w->xy[i + 1];

                        l = przeciecieLEA_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                            &line_tmp_w, (void *)ptrs_ellipticalarc);  //t2>=t1
                        if (l == 0)
                        {
                            ;  //linie rownolegle
                        }
                        else
                        {
                            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                            {
                                angle = Atan2(y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x);
                                new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                            }
                            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                            {
                                angle = Atan2(y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x);
                                new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                            }
                        }

                        df_x1 = line_tmp_w.x2;
                        df_y1 = line_tmp_w.y2;
                    }
                    break;
                }
                case Ospline:
                {
                    int i,j,k;
                    float out_x[MaxNpts], out_y[MaxNpts];
                    double x1, x2, y1, y2, t1, t2, angle;
                    int l;

                    SPLINE tmp_spline = Splinedef;
                    double bnodes_2_tangent_org, bnodes_2_tangent;
                    int node_count;

                    ptrs_s = (SPLINE*)nag;

#define Npts 90  //for more precission

                    if (ptrs_s->npts < 5)
                    {
                        calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, &out_x, &out_y);

                        for (i = 0; i < Npts - 1; i++)
                        {
                            line_tmp_w.x1 = out_x[i];
                            line_tmp_w.y1 = out_y[i];
                            line_tmp_w.x2 = out_x[i + 1];
                            line_tmp_w.y2 = out_y[i + 1];

                            l = przeciecieLEA_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                                &line_tmp_w, (void*)ptrs_ellipticalarc);  //t2>=t1
                            if (l == 0)
                            {
                                ;  //linie rownolegle
                            }
                            else
                            {
                                if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                                {
                                    angle = Atan2(y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x);
                                    new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                                }
                                if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                                {
                                    angle = Atan2(y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x);
                                    new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
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

                                l = przeciecieLEA_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                                    &line_tmp_w, (void*)ptrs_ellipticalarc);  //t2>=t1
                                if (l == 0)
                                {
                                    ;  //linie rownolegle
                                }
                                else
                                {
                                    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                                    {
                                        angle = Atan2(y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x);
                                        new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                                    }
                                    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                                    {
                                        angle = Atan2(y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x);
                                        new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
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

                                l = przeciecieLEA_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                                    &line_tmp_w, (void*)ptrs_ellipticalarc);  //t2>=t1
                                if (l == 0)
                                {
                                    ;  //linie rownolegle
                                }
                                else
                                {
                                    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                                    {
                                        angle = Atan2(y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x);
                                        new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                                    }
                                    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                                    {
                                        angle = Atan2(y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x);
                                        new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
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

                                l = przeciecieLEA_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                                    &line_tmp_w, (void*)ptrs_ellipticalarc);  //t2>=t1
                                if (l == 0)
                                {
                                    ;  //linie rownolegle
                                }
                                else
                                {
                                    if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                                    {
                                        angle = Atan2(y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x);
                                        new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                                    }
                                    if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                                    {
                                        angle = Atan2(y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x);
                                        new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                                    }
                                }

                            }
                        }

                    }
                    break;
                }
                case Oellipse:
                case Ofilledellipse:
                {
                    int numpoints;
                    double xy[512];
                    int verno;
                    int i, j, k;
                    double x1, x2, y1, y2, t1, t2, angle;
                    int l;

                    ptrs_e = (ELLIPSE *) nag;

                    verno = Get_Ell_Verno();
                    Set_Ell_Verno(250);
                    numpoints = Get_Ellipse_Points(ptrs_e->x, ptrs_e->y, ptrs_e->rx, ptrs_e->ry, ptrs_e->angle, xy);
                    Set_Ell_Verno(verno);

                    xy[numpoints] = xy[0];
                    xy[numpoints + 1] = xy[1];
                    numpoints += 2;

                    for (i = 0; i < (numpoints - 2); i += 2) {
                        line_tmp_w.x1 = xy[i];
                        line_tmp_w.y1 = xy[i + 1];
                        line_tmp_w.x2 = xy[i + 2];
                        line_tmp_w.y2 = xy[i + 3];

                        l = przeciecieLEA_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                            &line_tmp_w, (void*)ptrs_ellipticalarc);  //t2>=t1
                        if (l == 0)
                        {
                            ;  //linie rownolegle
                        }
                        else
                        {
                            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                            {
                                angle = Atan2(y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x);
                                new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                            }
                            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                            {
                                angle = Atan2(y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x);
                                new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                            }
                        }
                    }

                    break;
                }
                case Oellipticalarc:
                {
                    int numpoints;
                    double xy[512];
                    int verno;
                    int i, j, k;
                    double x1, x2, y1, y2, t1, t2, angle;
                    int l;


                    ptrs_ea = (ELLIPTICALARC *) nag;

                    verno = Get_Ell_Verno();
                    Set_Ell_Verno(250);
                    numpoints = Get_EllipticalArc_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle,
                                                         ptrs_ea->kat1, ptrs_ea->kat2, xy);
                    Set_Ell_Verno(verno);

                    for (i = 0; i < (numpoints - 2); i += 2) {
                        line_tmp_w.x1 = xy[i];
                        line_tmp_w.y1 = xy[i + 1];
                        line_tmp_w.x2 = xy[i + 2];
                        line_tmp_w.y2 = xy[i + 3];

                        l = przeciecieLEA_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                            &line_tmp_w, (void*)ptrs_ellipticalarc);  //t2>=t1
                        if (l == 0)
                        {
                            ;  //linie rownolegle
                        }
                        else
                        {
                            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                            {
                                angle = Atan2(y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x);
                                new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                            }
                            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                            {
                                angle = Atan2(y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x);
                                new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                            }
                        }
                    }
                    break;
                }

                case Ookrag :
                case Okolo :
                case Oluk :
                {
                    double x1, x2, y1, y2, t1, t2, angle ;
                    double df_xc, df_yc, df_r, df_kat1, df_kat2;
                    double df_dangle, df_elangle;
                    int i__Ell_Verno;
                    int l;

                    i__Ell_Verno=Get_Ell_Verno();

                    if (nag->obiekt == Oluk)
                    {
                        df_xc=((LUK*)nag)->x;
                        df_yc=((LUK*)nag)->y;
                        df_r=((LUK*)nag)->r;
                        df_kat1=((LUK*)nag)->kat1;
                        df_kat2=((LUK*)nag)->kat2;

                        if (df_kat2<df_kat1) df_kat2+=Pi2;
                        df_dangle = (df_kat2-df_kat1) / i__Ell_Verno;
                        df_elangle = df_kat1;
                    } else{
                        df_xc=((OKRAG*)nag)->x;
                        df_yc=((OKRAG*)nag)->y;
                        df_r=((OKRAG*)nag)->r;
                        df_dangle = Pi2 / i__Ell_Verno;
                        df_elangle = 0.0;
                    }


                    line_tmp_w.x1 = df_xc + df_r * cos(df_elangle);
                    line_tmp_w.y1 = df_yc + df_r * sin(df_elangle);

                    df_elangle += df_dangle;

                    for (i = 0; i < i__Ell_Verno; i++, df_elangle += df_dangle)
                    {
                        line_tmp_w.x2 = df_xc + df_r * cos(df_elangle);
                        line_tmp_w.y2 = df_yc + df_r * sin(df_elangle);

                        l = przeciecieLEA_tt(&x1, &y1, &t1, &x2, &y2, &t2,
                                            &line_tmp_w, (void*)ptrs_ellipticalarc);  //t2>=t1

                        line_tmp_w.x1 = line_tmp_w.x2;
                        line_tmp_w.y1 = line_tmp_w.y2;

                        if (l == 0)
                        {
                            ;  //linie rownolegle
                        }
                        else
                        {
                            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
                            {
                                angle = Atan2(y1 - ptrs_ellipticalarc->y, x1 - ptrs_ellipticalarc->x);
                                new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                            }
                            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
                            {
                                angle = Atan2(y2 - ptrs_ellipticalarc->y, x2 - ptrs_ellipticalarc->x);
                                new_intersection_ellipticalarc(&angle1, &angle2, a_sel, angle, kat1, kat2, &b_inter1, &b_inter2);
                            }
                        }
                    }
                    break ;
                }
                default :
                    break ;
            }
        }
        obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony);
    }
    if ((b_inter1 == FALSE && b_inter2 == FALSE) ||
        TRUE == Check_if_Equal (angle1, angle2))
    {
        return ; 	//obiekt zle wybrany, nie ma nic do wyciecja
    }
    ptrs_b = NULL ;
    b_add1 = FALSE ;
    b_add2 = FALSE ;
    len = elarc_tmp.n ;

    memcpy (&elarc_tmp, ptrs_ellipticalarc, sizeof (elarc_tmp)) ;

    if (b_inter1 == FALSE)
    {
        elarc_tmp.kat1 = Angle_Normal(angle2 - ptrs_ellipticalarc->angle);
    }
    else
    if (b_inter2 == FALSE)
    {
        elarc_tmp.kat2 = Angle_Normal(angle1 - ptrs_ellipticalarc->angle);
    }
    else
    {
        memcpy (&elarc_tmp1, ptrs_ellipticalarc, sizeof (elarc_tmp)) ;
        ptrs_b = Parent_Block ((char  *)ptrs_ellipticalarc) ;
        elarc_tmp.kat2 = Angle_Normal(angle1 - ptrs_ellipticalarc->angle);
        elarc_tmp1.kat1 = Angle_Normal(angle2 - ptrs_ellipticalarc->angle);
        if (FALSE == Check_if_Equal (Angle_Normal (elarc_tmp1.kat1),
                                     Angle_Normal (elarc_tmp1.kat2)))
        {
            b_add2 = TRUE ;
        }
    }
    if (FALSE == Check_if_Equal (Angle_Normal (elarc_tmp.kat1),
                                 Angle_Normal (elarc_tmp.kat2)))
    {
        b_add1 = TRUE ;
    }
    if (b_add1 == FALSE && b_add2 == TRUE)
    {
        memcpy (&elarc_tmp, &elarc_tmp1, sizeof (elarc_tmp)) ;
        b_add1 = TRUE ;
        b_add2 = FALSE ;
    }
    if (b_add1 == FALSE && b_add2 == FALSE)
    {
        return ;
    }
    change_date (ptrs_b, (void *)ptrs_ellipticalarc, (void *)&elarc_tmp,
                 ((b_add1 == TRUE && b_add2 == TRUE) ? (void *)&elarc_tmp1 : NULL)) ;
}

static int przecieciell_tr (void *adr1, void *adr2,
		double *x1, double *y1, double *x2, double *y2)
/*-----------------------------------------------------------*/
{
  LUK *ptrs_arc1, *ptrs_arc2 ;
  OKRAG s_o1, s_o2 ;
  int no_inter ;
  double df_x1, df_x2, df_y1, df_y2, df_angle1, df_angle2 ;
  BOOL b_p1 , b_p2 ;

  b_p1 = b_p2 = FALSE ;
  ptrs_arc1 =(LUK*)adr1 ;
  ptrs_arc2 =(LUK*)adr2 ;
  s_o1.r = ptrs_arc1->r ;
  s_o1.x = ptrs_arc1->x ;
  s_o1.y = ptrs_arc1->y ;
  s_o2.r = ptrs_arc2->r ;
  s_o2.x = ptrs_arc2->x ;
  s_o2.y = ptrs_arc2->y ;
  if (0 == (no_inter = przeciecieOO_ex (&df_x1, &df_y1, &df_x2, &df_y2, (void*)&s_o1, (void*)&s_o2)))
  {
    return 0 ;
  }
  df_angle1 = Atan2 (df_y1 - ptrs_arc1->y, df_x1 - ptrs_arc1->x) ;
  df_angle2 = Atan2 (df_y1 - ptrs_arc2->y, df_x1 - ptrs_arc2->x) ;
  if (TRUE == Check_Angle (df_angle1, ptrs_arc1->kat1, ptrs_arc1->kat2) &&
      TRUE == Check_Angle (df_angle2, ptrs_arc2->kat1, ptrs_arc2->kat2))
  {
    b_p1 = TRUE ;
  }
  if (no_inter == 2)
  {
    df_angle1 = Atan2 (df_y2 - ptrs_arc1->y, df_x2 - ptrs_arc1->x) ;
    df_angle2 = Atan2 (df_y2 - ptrs_arc2->y, df_x2 - ptrs_arc2->x) ;
    if (TRUE == Check_Angle (df_angle1, ptrs_arc1->kat1, ptrs_arc1->kat2) &&
	TRUE == Check_Angle (df_angle2, ptrs_arc2->kat1, ptrs_arc2->kat2))
    {
      b_p2 = TRUE ;
    }
  }
  if (b_p1 == FALSE && b_p2 == FALSE)
  {
    return 0 ;
  }
  if (b_p1 == TRUE && b_p2 == FALSE)
  {
    *x1 = df_x1 ;
    *y1 = df_y1 ;
    return 1 ;
  }
  if (b_p1 == FALSE && b_p2 == TRUE)
  {
    *x1 = df_x2 ;
    *y1 = df_y2 ;
    return 1 ;
  }
  *x1 = df_x1 ;
  *y1 = df_y1 ;
  *x2 = df_x2 ;
  *y2 = df_y2 ;
  return 2 ;
}


static int przeciecieOl_tr (void *adr1, void *adr2,
		double *x1, double *y1, double *x2, double *y2)
/*-----------------------------------------------------------*/
{
  LUK *ptrs_arc ;
  OKRAG s_c, *ptrs_c ;
  int no_inter ;
  double df_x1, df_x2, df_y1, df_y2, df_angle ;
  BOOL b_p1 , b_p2 ;

  b_p1 = b_p2 = FALSE ;
  ptrs_c =(OKRAG*)adr1 ;
  ptrs_arc =(LUK*)adr2 ;
  s_c.r = ptrs_arc->r ;
  s_c.x = ptrs_arc->x ;
  s_c.y = ptrs_arc->y ;
  if (0 == (no_inter = przeciecieOO_ex (&df_x1, &df_y1, &df_x2, &df_y2, (void*)&s_c, (void*)ptrs_c)))
  {
    return 0 ;
  }
  df_angle = Atan2 (df_y1 - ptrs_arc->y, df_x1 - ptrs_arc->x) ;
  if (TRUE == Check_Angle (df_angle, ptrs_arc->kat1, ptrs_arc->kat2))
  {
    b_p1 = TRUE ;
  }
  if (no_inter == 2)
  {
    df_angle = Atan2 (df_y2 - ptrs_arc->y, df_x2 - ptrs_arc->x) ;
    if (TRUE == Check_Angle (df_angle, ptrs_arc->kat1, ptrs_arc->kat2))
    {
      b_p2 = TRUE ;
    }
  }
  if (b_p1 == FALSE && b_p2 == FALSE)
  {
    return 0 ;
  }
  if (b_p1 == TRUE && b_p2 == FALSE)
  {
    *x1 = df_x1 ;
    *y1 = df_y1 ;
    return 1 ;
  }
  if (b_p1 == FALSE && b_p2 == TRUE)
  {
    *x1 = df_x2 ;
    *y1 = df_y2 ;
    return 1 ;
  }
  *x1 = df_x1 ;
  *y1 = df_y1 ;
  *x2 = df_x2 ;
  *y2 = df_y2 ;
  return 2 ;
}




static void trim_line_ob (LINIA *ptrs_line)
/*---------------------------------------*/
{
  long_long l_offb, l_offe, l_offob ;
  double x, y, t1, t2 ;
  double x1, x2, y1, y2 ;
  int l ;
  NAGLOWEK *nag ;
  float df_x1, df_y1;
  LINIA line_tmp_w, line_tmp;
  WIELOKAT* ptrs_w;
  SPLINE* ptrs_s;
    ELLIPSE *ptrs_e;
    ELLIPTICALARC *ptrs_ea;
  int i,j,k;
    int numpoints;
    double xy[512];
    int verno;

  SPLINE tmp_spline = Splinedef;
  double bnodes_2_tangent_org, bnodes_2_tangent;
  int node_count;

  l_offb = ADP - dane ;
  l_offe = ADK - dane ;
  
  while (NULL != (nag = Object_Tok (&l_offb, l_offe, &l_offob, ONieOkreslony, TOK_LAYER_ON)))
  {
    if (nag->atrybut == Ablok && (char *)ptrs_line != (char *)nag)
    {				/*pominiecie uciecia do samego siebie*/
       switch (nag->obiekt)
       {
	 case Olinia :
	    if (0 == przeciecieLL_tt (&x, &y, (void *)ptrs_line,
		(void *)nag, &t1, &t2))
	    {
	      break ;  /*linie rownolegle*/
	    }
	    if (t2 >= 0 - o_male   &&   t2 <= 1 + o_male)
	    {
	      new_intersection (x, y, t1) ;
	    }
	    break ;
	case Owwielokat:
		memcpy(&line_tmp_w, ptrs_line, sizeof(line_tmp_w));
		ptrs_w = (WIELOKAT*)nag;

		df_x1 = ptrs_w->xy[ptrs_w->lp - 2];
		df_y1 = ptrs_w->xy[ptrs_w->lp - 1];

		for (i = 0; i < ptrs_w->lp; i += 2)
		{
			line_tmp_w.x1 = df_x1;
			line_tmp_w.y1 = df_y1;
			line_tmp_w.x2 = ptrs_w->xy[i];
			line_tmp_w.y2 = ptrs_w->xy[i + 1];

			if (0 == przeciecieLL_tt(&x, &y, (void*)ptrs_line, &line_tmp_w, &t1, &t2))
			{
				;  /*linie rownolegle*/
			}
			else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
			{
				new_intersection(x, y, t1);
			}

			df_x1 = line_tmp_w.x2;
			df_y1 = line_tmp_w.y2;
		}
		break;
	case Ospline:
		{
			int i;
			float out_x[MaxNpts], out_y[MaxNpts];

			memcpy(&line_tmp_w, ptrs_line, sizeof(line_tmp));
			ptrs_s = (SPLINE*)nag;

#define Npts 90  //for more precission

			if (ptrs_s->npts < 5)
			{
				calc_bspline(ptrs_s->lp, ptrs_s->npts, ptrs_s->xy, Npts, &out_x, &out_y);

				for (i = 0; i < Npts - 1; i++)
				{
					line_tmp_w.x1 = out_x[i];
					line_tmp_w.y1 = out_y[i];
					line_tmp_w.x2 = out_x[i + 1];
					line_tmp_w.y2 = out_y[i + 1];

					if (0 == przeciecieLL_tt(&x, &y, (void*)ptrs_line, &line_tmp_w, &t1, &t2))
					{
						;  /*linie rownolegle*/
					}
					else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
					{
						new_intersection(x, y, t1);
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

						if (0 == przeciecieLL_tt(&x, &y, (void*)ptrs_line, &line_tmp_w, &t1, &t2))
						{
							;  /*linie rownolegle*/
						}
						else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							new_intersection(x, y, t1);
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

						if (0 == przeciecieLL_tt(&x, &y, (void*)ptrs_line, &line_tmp_w, &t1, &t2))
						{
							;  /*linie rownolegle*/
						}
						else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							new_intersection(x, y, t1);
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

						if (0 == przeciecieLL_tt(&x, &y, (void*)ptrs_line, &line_tmp_w, &t1, &t2))
						{
							;  /*linie rownolegle*/
						}
						else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
						{
							new_intersection(x, y, t1);
						}

					}
				}

			}
			break;
		}
           case Oellipse:
           case Ofilledellipse:
               memcpy(&line_tmp_w, ptrs_line, sizeof(line_tmp));
               ptrs_e = (ELLIPSE*)nag;

               verno = Get_Ell_Verno () ;
               Set_Ell_Verno (250) ;
               numpoints=Get_Ellipse_Points(ptrs_e->x, ptrs_e->y, ptrs_e->rx, ptrs_e->ry, ptrs_e->angle, xy);
               Set_Ell_Verno (verno) ;

               xy[numpoints] = xy[0];
               xy[numpoints+1] = xy[1];
               numpoints+=2;

               for (i = 0; i < (numpoints-2); i+=2)
               {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i+1];
                   line_tmp_w.x2 = xy[i+2];
                   line_tmp_w.y2 = xy[i+3];

                   if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2)) { ;  /*linie rownolegle*/
                   } else if (t2 >= 0 - o_male && t2 <= 1 + o_male) {
                       new_intersection(x, y, t1);
                   }
               }
               break;
           case Oellipticalarc:
               memcpy(&line_tmp_w, ptrs_line, sizeof(line_tmp));
               ptrs_ea = (ELLIPTICALARC*)nag;

               verno = Get_Ell_Verno () ;
               Set_Ell_Verno (250) ;
               numpoints=Get_EllipticalArc_Points(ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, ptrs_ea->kat1, ptrs_ea->kat2, xy);
               Set_Ell_Verno (verno) ;

               for (i = 0; i < (numpoints-2); i+=2)
               {
                   line_tmp_w.x1 = xy[i];
                   line_tmp_w.y1 = xy[i+1];
                   line_tmp_w.x2 = xy[i+2];
                   line_tmp_w.y2 = xy[i+3];

                   if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2)) { ;  /*linie rownolegle*/
                   } else if (t2 >= 0 - o_male && t2 <= 1 + o_male) {
                       new_intersection(x, y, t1);
                   }
               }
               break;
	 case Ookrag :
	 case Okolo :
	 case Oluk :
	 {
	    if (nag->obiekt == Oluk)
	    {
	      l = przeciecieLl_tt (&x1, &y1, &t1, &x2, &y2, &t2,
		    (void *)ptrs_line, (void *)nag) ;  /*t2>=t1*/
	    }
	    else
	    {
	      l = przeciecieLO_tt (&x1, &y1, &t1, &x2, &y2, &t2,
		    (void *)ptrs_line, (void *)nag) ;  /*t2>=t1*/
	    }
	    if (0 == l)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    new_intersection (x1, y1, t1) ;
	    if (l == 2)
	    {
	      new_intersection (x2, y2, t2) ;
	    }
	    break ;
	 }
	 default :
	    break ;
       }
    }
  }
}


static void trim_arc_ob (LUK *ptrs_arc,
			 double df_a_sel,
			 double *ptrdf_angle1,
			 double *ptrdf_angle2,
			 BOOL *ptrb_inter1,
			 BOOL *ptrb_inter2)
/*--------------------------------------*/
{
  long_long l_offb, l_offe, l_offob ;
  double df_x1, df_x2, df_y1, df_y2, df_t1, df_t2, df_angle ;
  int l;
  NAGLOWEK *nag ;

  l_offb = ADP - dane ;
  l_offe = ADK - dane ;
  while (NULL != (nag = Object_Tok (&l_offb, l_offe, &l_offob, ONieOkreslony, TOK_LAYER_ON)))
  {
    if (nag->atrybut == Ablok && (char *)ptrs_arc != (char *)nag)
    { 	/*pominiecie uciecia do samego siebie*/
       switch (nag->obiekt)
       {
	 case Olinia :
	 {
	    l = przeciecieLl_tt (&df_x1, &df_y1, &df_t1, &df_x2, &df_y2, &df_t2,
		    (void *)nag, (void *)ptrs_arc) ;  /*t2>=t1*/
	    if (l == 0)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    if (df_t1 >= 0 - o_male && df_t1 <= 1 + o_male)
	    {
	      df_angle = Atan2 (df_y1 - ptrs_arc->y, df_x1 - ptrs_arc->x) ;
	      new_intersection_arc (ptrdf_angle1, ptrdf_angle2, df_a_sel, df_angle, ptrb_inter1, ptrb_inter2, ptrs_arc) ;
	    }
	    if (l == 2 && df_t2 >= 0 - o_male && df_t2 <= 1 + o_male)
	    {
	      df_angle = Atan2 (df_y2 - ptrs_arc->y, df_x2 - ptrs_arc->x) ;
	      new_intersection_arc (ptrdf_angle1, ptrdf_angle2, df_a_sel, df_angle, ptrb_inter1, ptrb_inter2, ptrs_arc) ;
	    }
	    break ;
	 }
	 case Ookrag :
	 case Okolo :
	 case Oluk :
	 {
	    if (nag->obiekt == Oluk)
	    {
	      l = przecieciell_tr ((void *)nag, (void *)ptrs_arc,
		&df_x1, &df_y1, &df_x2, &df_y2) ;
	    }
	    else
	    {
	      l = przeciecieOl_tr ((void *)nag, (void *)ptrs_arc,
		&df_x1, &df_y1, &df_x2, &df_y2) ;
	    }
	    if (l == 0)
	    {
	      break ;  /*brak przeciecia*/
	    }
	    df_angle = Atan2 (df_y1 - ptrs_arc->y, df_x1 - ptrs_arc->x) ;
	    new_intersection_arc (ptrdf_angle1, ptrdf_angle2, df_a_sel, df_angle, ptrb_inter1, ptrb_inter2, ptrs_arc) ;
	    if (l == 2)
	    {
	      df_angle = Atan2 (df_y2 - ptrs_arc->y, df_x2 - ptrs_arc->x) ;
	      new_intersection_arc (ptrdf_angle1, ptrdf_angle2, df_a_sel, df_angle, ptrb_inter1, ptrb_inter2, ptrs_arc) ;
	    }
	    break ;
	 }
	 default :
	    break ;
       }
    }
  }
}

static BOOL
trim_pl_line_tmp1 (BOOL   b_first,
		   BOOL   b_prev,
		   LINIA  *ptrs_line,
		   double df_xsel,
		   double df_ysel,
		   double *ptrdf_xinter,
		   double *ptrdf_yinter)
/*----------------------------------*/
{
  BOOL b_found ;
  double df_t_ob, df_t_end ;

  b_found = FALSE ;
  df__te1 = - 1 ;    /*wstepnie df__te1, df__te2 - inicjowane sa poza odcinkiem <0, 1> */
  df__te2 = 2 ;
  if (TRUE == b_first)
  {
    najblizszyL_ (&df_xsel, &df_ysel, ptrs_line) ;
    if (fabs (ptrs_line->x2 - ptrs_line->x1) >
	fabs (ptrs_line->y2 - ptrs_line->y1))
    {
      df__tsel = (df_xsel - ptrs_line->x1) / (ptrs_line->x2 - ptrs_line->x1) ;
    }
    else
    if (FALSE == Check_if_Equal (ptrs_line->y1, ptrs_line->y2))
    {
      df__tsel = (df_ysel - ptrs_line->y1) / (ptrs_line->y2 - ptrs_line->y1) ;
    }
    else
    {
      df__tsel = .5 ; /*dla odcinka 'zerowego'*/
    }
  }
  else
  {
    if (b_prev == TRUE)
    {
      df__tsel = ptrs_line->obiektt3 == RightDir ? 1 : 0 ; /*df__te1 : df__te2*/
    }
    else
    {
      df__tsel = ptrs_line->obiektt3 == RightDir ? 0 : 1 ; /*df__te2 : df__te1*/
    }
  }
  trim_line_ob (ptrs_line) ;
  if (b_prev == TRUE)
  {
    df_t_ob = ptrs_line->obiektt3 == RightDir ? df__te1 : df__te2 ;
    df_t_end = ptrs_line->obiektt3 == RightDir ? 0 : 1 ;
  }
  else
  {
    df_t_ob = ptrs_line->obiektt3 == RightDir ? df__te2 : df__te1 ;
    df_t_end = ptrs_line->obiektt3 == RightDir ? 1 : 0 ;
  }
  if (df_t_ob >= 0 - o_male && df_t_ob <= 1 + o_male)
  {
    *ptrdf_xinter = df_t_ob * ptrs_line->x2 + (1 - df_t_ob) * ptrs_line->x1 ;
    *ptrdf_yinter = df_t_ob * ptrs_line->y2 + (1 - df_t_ob) * ptrs_line->y1 ;
    b_found = TRUE ;
    out_krz (*ptrdf_xinter, *ptrdf_yinter) ;
  }
  else
  { 	/*koniec odcinka*/
    *ptrdf_xinter = df_t_end * ptrs_line->x2 + (1 - df_t_end) * ptrs_line->x1 ;
    *ptrdf_yinter = df_t_end * ptrs_line->y2 + (1 - df_t_end) * ptrs_line->y1 ;
  }
  return b_found ;
}

static BOOL
trim_pl_arc_tmp1 (BOOL 	 b_first,
		  BOOL   b_prev,
		  LUK    *ptrs_arc,
		  double df_xsel,
		  double df_ysel,
		  double *ptrdf_xinter,
		  double *ptrdf_yinter)
/*-----------------------------------*/
{
  double df_a_sel ;
  BOOL b_found ;
  double df_angle1, df_angle2, df_angle, df_angle_end ;
  BOOL b_inter1, b_inter2 ;

  b_found = FALSE ;
  b_inter1 = b_inter2 = FALSE ;
  df_angle1 = df_angle2 = 0 ;
  if (b_first == TRUE)
  {
    najblizszyl_ (&df_xsel, &df_ysel, ptrs_arc) ;
    df_a_sel = Angle_Normal (Atan2 (df_ysel - ptrs_arc->y, df_xsel - ptrs_arc->x)) ;
  }
  else
  {
    if (b_prev == TRUE)
    {
      df_a_sel = ptrs_arc->obiektt3 == RightDir ? ptrs_arc->kat2 : ptrs_arc->kat1 ;  /*b_inter1 : b_inter2*/
    }
    else
    {
      df_a_sel = ptrs_arc->obiektt3 == RightDir ? ptrs_arc->kat1 : ptrs_arc->kat2 ;  /*b_inter2 : b_inter1*/
    }
  }
  trim_arc_ob (ptrs_arc, df_a_sel, &df_angle1, &df_angle2, &b_inter1, &b_inter2) ;
  if (b_prev == TRUE)
  {
    b_found = ptrs_arc->obiektt3 == RightDir ? b_inter1 : b_inter2 ;
    df_angle = ptrs_arc->obiektt3 == RightDir ? df_angle1 : df_angle2 ;
    df_angle_end = ptrs_arc->obiektt3 == RightDir ? ptrs_arc->kat1 : ptrs_arc->kat2 ;
  }
  else
  {
    b_found = ptrs_arc->obiektt3 == RightDir ? b_inter2 : b_inter1 ;
    df_angle = ptrs_arc->obiektt3 == RightDir ? df_angle2 : df_angle1 ;
    df_angle_end = ptrs_arc->obiektt3 == RightDir ? ptrs_arc->kat2 : ptrs_arc->kat1 ;
  }
  if (b_found == TRUE)
  {
    *ptrdf_xinter = ptrs_arc->x + ptrs_arc->r * cos (df_angle) ;
    *ptrdf_yinter = ptrs_arc->y + ptrs_arc->r * sin (df_angle) ;
    out_krz (*ptrdf_xinter, *ptrdf_yinter) ;
  }
  else
  {   /*koniec luku*/
    *ptrdf_xinter = ptrs_arc->x + ptrs_arc->r * cos (df_angle_end) ;
    *ptrdf_yinter = ptrs_arc->y + ptrs_arc->r * sin (df_angle_end) ;
  }
  return b_found ;
}


static void trim_pline (void *ptr_pl, void *ptr_obsel, double df_xsel, double df_ysel)
/*---------------------------------------------------------------------------*/
{
  void *ptr_ob1, *ptr_ob2, *ptr_ob01, *ptr_ob02 ;
  BOOL b_close, b_close_ins ;
  BOOL b_first, b_found1, b_found2 ;
  double df_x1sel, df_y1sel, df_x2sel, df_y2sel ;
  WIELOKAT* ptrs_w;

  b__undopl = FALSE ;
  b_first = TRUE ;
  b_found1 = FALSE ;
  ptr_ob01 = ptr_obsel ;
  b_close = Check_if_Close_Pline ((BLOK*)ptr_pl) ;
  do
  {
    ptr_ob1 = ptr_ob01 ;
    switch (((NAGLOWEK*)ptr_ob1)->obiekt)
    {
      case Olinia :
	b_found1 = trim_pl_line_tmp1 (b_first, TRUE,
				(LINIA*)ptr_ob1, df_xsel, df_ysel,
				&df_x1sel, &df_y1sel) ;
	break ;
      case Oluk :
	b_found1 = trim_pl_arc_tmp1 (b_first, TRUE,
				     (LUK*)ptr_ob1, df_xsel, df_ysel,
				     &df_x1sel, &df_y1sel) ;
	break ;
    }
    if (b_found1 == TRUE)
    {
      break ;
    }
    b_first = FALSE ;
  }
  while (b_found1 == FALSE &&
	NULL != (ptr_ob01 = Get_Pline_Previous_Ob (ptr_pl, ptr_ob1, b_close)) &&
	 (char *)ptr_ob01 != (char *)ptr_obsel) ;

  b_first = TRUE ;
  b_found2 = FALSE ;
  ptr_ob02 = ptr_obsel ;
  do
  {
    ptr_ob2 = ptr_ob02 ;
    switch (((NAGLOWEK*)ptr_ob2)->obiekt)
    {
      case Olinia :
	b_found2 = trim_pl_line_tmp1 (b_first, FALSE,
				      (LINIA*)ptr_ob2, df_xsel, df_ysel,
				      &df_x2sel, &df_y2sel) ;
	break ;
      case Oluk :
	b_found2 = trim_pl_arc_tmp1 (b_first, FALSE,
				     (LUK*)ptr_ob2, df_xsel, df_ysel,
				     &df_x2sel, &df_y2sel) ;
	break ;
    }
    if (b_found2 == TRUE)
    {
      break ;
    }
    b_first = FALSE ;
  }
  while (b_found2 == FALSE &&
	NULL != (ptr_ob02 = Get_Pline_Next_Ob (ptr_pl, ptr_ob2, b_close)) &&
	 (char *)ptr_ob02 != (char *)ptr_obsel) ;
  if (b_found1 == FALSE && b_found2 == FALSE)
  {
    return ;
  }
  b_close_ins = FALSE ;
  if ((char *)ptr_obsel >= (char *)ptr_ob1 &&
      (char *)ptr_obsel <= (char *)ptr_ob2)
  {
    b_close_ins = TRUE ;
  }

  break_pline (ptr_pl, ptr_ob1, ptr_ob2, df_x1sel, df_y1sel, df_x2sel, df_y2sel, b_close_ins) ;
}


void *check_if_pline (void *ptr_object)
/*-----------------------------------------*/
{
  BLOK *ptrs_temp ;
  void *ptr_ret ;

  if ((char *)ptr_object == NULL)
	  return NULL;

  ptr_ret = NULL ;
  if (((NAGLOWEK*)ptr_object)->obiektt2 == O2BlockPline &&
	  NULL != (ptrs_temp = FIRSTB((char*)ptr_object)) &&
      TRUE == Check_Pline (ptrs_temp))
  {
    ptr_ret = ptrs_temp ;
  }
  return ptr_ret ;
}

static BOOL ini_trim_extend (int type, int tr_ex)
/*---------------------------------------------*/
{
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  unsigned short cod = L' ';
  static int iconno=0;

  if (type == 0)
  {
    if (NULL == (ptrsz__undo_old_ob = (char *)malloc /*getmem*/ (2 * MaxSizeObiekt)))
    {
      return FALSE ;
    }
    ptrsz__undo_new_ob = ptrsz__undo_old_ob + MaxSizeObiekt ;
    b__undo = FALSE ;
    ptrsz__undo_new1_ob = NULL ;
    getmenupini (&menu,st,&cod, &iconno) ;

    if (tr_ex==0) menupini (&mTrim, _TRIM_, _TRIM_C_, 45) ;
    else menupini (&mExtend, _EXTEND_, _EXTEND_C_, 46) ;

    return TRUE ;
  }
  else
  {
    menupini (menu, st, cod, iconno) ;
    free(ptrsz__undo_old_ob);
    return TRUE ;
  }
}

BOOL ini_trim (int type)
{
    return ini_trim_extend (type, 0);
}

BOOL ini_extend (int type)
{
    return ini_trim_extend (type, 1);
}


int Trim_Proc (void)
/*------------------*/
{
  double X0, Y0 ;
  EVENT *ev;
  unsigned type ;
  void * ad ;
  BLOK *ptrs_pl ;
  long_long l_offb, l_offe;
  BLOK* ptrs_temp;
  BLOK *ptrs_b=NULL;
  char* buff_ptrs_b;
  long_long spline_offset;
  long_long dane_size0;
  long ptrs_pl_n;
  long_long ptrc_b_offs;

  shadows_no = 0;

  if (FALSE == ini_trim (0))
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
	 ini_trim (1) ;
	 return TRUE ;
       }
       if (ev->Number == ENTER)
       {
	 type = Blinia | Bluk | Bokrag | Bspline | Bellipse | Bellipticalarc;
	 if ( NULL != (ad = obiekt_wybrany1 (&type)) &&
	     0 != Layers [((LINIA*)ad)->warstwa].edit)
	 {
	   CUR_OFF(X,Y);
	   if (((NAGLOWEK*)ad)->obiekt == Ospline)
	   {
		   //generatine spline shadow(s)
		   l_offb = (long_long)ad - (long_long)dane;
		   spline_offset = (long_long)ad - (long_long)dane;
		   dane_size0 = dane_size;
		   l_offe = l_offb + sizeof(SPLINE);

		   ptrs_pl = (BLOK*)check_if_pline(ad);
		   if (ptrs_pl != NULL)
		   {
			   ad = ptrs_pl;
		   }

		   l_offb = (long_long)ad;
		   ptrs_pl_n = ((NAGLOWEK*)ad)->n + sizeof(NAGLOWEK);
		   l_offe = l_offb + ptrs_pl_n - 1;
		   zmien_atrybut((char*)l_offb, (char*)l_offe, Anormalny, Ablok);

		   blokzap((char*)l_offb, (char*)l_offe, Ablok, COPY_PUT, 0);

		   l_offb -= (long_long)dane;
		   l_offe -= (long_long)dane;

		   if (((NAGLOWEK*)ad)->blok == 1)
		   {
			   ptrs_b = Parent_Block((char*)ad);
			   ptrc_b_offs = (long_long)ptrs_b - (long_long)dane;
		   }

		   shadows_no = make_spline_shadows(l_offb, l_offe, Anormalny, TRUE, Abad, 0, 0);

		   if (shadows_no == 0)
		   {
			   CUR_ON(X, Y);
			   continue;
		   }
		   
		   if (ptrs_b != NULL)
		   {
			   //moving block
			   ptrs_pl_n = ((NAGLOWEK*)dane)->n + sizeof(NAGLOWEK);
			   buff_ptrs_b = malloc(ptrs_pl_n);
			   if (buff_ptrs_b != NULL)
			   {
				   memmove(buff_ptrs_b, dane, ptrs_pl_n);
				   usun_obiekt(dane);
				   ad = dodaj_obiekt(ptrs_b, buff_ptrs_b);
				   ((NAGLOWEK*)ad)->blok = 1;
				   free(buff_ptrs_b);
			   }
		   }

		   usun_blok(dane, dane + dane_size);

		   type = Blinia;
		   ad = obiekt_wybrany1(&type);
	   }

	   if (NULL != (ptrs_pl = (BLOK*)check_if_pline (ad)))
	   {
	     trim_pline ((void*)ptrs_pl, ad, X0, Y0) ;

		 spline_shadows_to_plines();

	   }
	   else
	   {
	     switch (type)
	     {
		case Blinia :
		  trim_line (ad, X0, Y0) ;
		  break;
		case Bluk :
		  trim_arc (ad, X0, Y0) ;
		  break;
		case Bokrag :
		  trim_circle (ad, X0, Y0) ;
		  break;
        case Bellipse :
          trim_ellipse (ad, X0, Y0) ;
          break;
        case Bellipticalarc :
          trim_ellipticalarc (ad, X0, Y0) ;
          break;
		default :
		  break;
	     }
	   }
	   CUR_OFF(X, Y);
	   CUR_ON(X,Y);
	 }
       }
     }
     if (ev->What == evCommandP)
     {
       if (0 == ev->Number)
       {
		 undo_trim () ;
		 CUR_OFF(X, Y);
		 CUR_ON (X, Y) ;
       }
     }

  }
}

static void undo_pline (void)
/*-------------------------*/
{
  char  *ptrh_beg,  *ptrh_end ;

  if (PTR__GTMP4 != NULL)
  {
    ptrh_beg = PTR__GTMP4 ;
    ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
    blokzap (ptrh_beg, ptrh_end, ANieOkreslony, COPY_PUT, 0) ;
    usun_obiekt ((char*)ptrh_beg);
  }
  if (PTR__GTMP5 != NULL)
  {
    ptrh_beg = PTR__GTMP5 ;
    ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
    blokzap (ptrh_beg, ptrh_end, ANieOkreslony, COPY_PUT, 0) ;
    usun_obiekt ((char*)ptrh_beg);
  }
  ptrh_beg = PTR__GTMP6 ;
  ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
  zmien_atrybut (ptrh_beg, ptrh_end, Abad, i__undolp_attrib) ;
  blokzap (ptrh_beg, ptrh_end, i__undolp_attrib, COPY_PUT, 1) ;
  blokzap (ptrh_beg, ptrh_end, Ablok, XOR_PUT, 1) ;
}


static BOOL break_pline (void *ptr_pl,
	     void *ptr_ob1,
	     void *ptr_ob2,
	     double df_x1sel,
	     double df_y1sel,
	     double df_x2sel,
	     double df_y2sel,
	     BOOL b_close_ins)
/*---------------------------*/
{
  long l_temp, l_off1, l_off2, l_off3;	/*wzgledne polozenie wskazanych obiektow w bloku*/
  double df_x1, df_y1, df_x2, df_y2 ;
  double df_xtemp, df_ytemp ;
  char  *ptrh_beg,  *ptrh_end ;
  BOOL b_close ;
  BLOK *ptrs_temp ;
  char* PTR__GTMP1_ = NULL, * PTR__GTMP2_ = NULL;
  char* ptr_parent;

  PTR__GTMP1 = (char *)ptr_ob1 ;
  PTR__GTMP2 = (char *)ptr_ob2 ;
  PTR__GTMP3 = (char *)ptr_pl ;
  PTR__GTMP4 = PTR__GTMP5 = NULL ;
  l_off1 = (char *)PTR__GTMP1 - PTR__GTMP3 ;
  l_off2 = (char *)PTR__GTMP2 - PTR__GTMP3 ;
  b_close = Check_if_Close_Pline ((BLOK *)PTR__GTMP3) ;
  if (NULL == (PTR__GTMP1 = (char *)dodaj_obiekt (NULL, (void *)PTR__GTMP3)) ||
      NULL == (PTR__GTMP2 = (char *)dodaj_obiekt (NULL, (void *)PTR__GTMP3)) ||
      (b_close == TRUE &&
      NULL == (PTR__GTMP4 = (char *)dodaj_obiekt (NULL, (void *)PTR__GTMP3))))
  {
    if (PTR__GTMP1 != NULL)
    {
      usun_obiekt ((void *)PTR__GTMP1) ;
    }
    if (PTR__GTMP2 != NULL)
    {
      usun_obiekt ((void *)PTR__GTMP2) ;
    }
    ErrList (18) ;
    return FALSE ;
  }
  l_temp = sizeof (NAGLOWEK) + ((BLOK*)PTR__GTMP3)->n ;
  movmem_ ((void*)PTR__GTMP3, (void*)PTR__GTMP1, l_temp) ; /*dodaj_obiekt nie musi dobrze dzialac*/
  movmem_ ((void*)PTR__GTMP3, (void*)PTR__GTMP2, l_temp) ;
  i__undolp_attrib = ((NAGLOWEK*)ptr_ob1)->atrybut ;
  if (i__undolp_attrib == Ablok)
  {
    ADP = dane ;
  }
  if (b_close == TRUE)
  {
    movmem_ ((void*)PTR__GTMP3, (void*)PTR__GTMP4, l_temp) ;
  }
  if (l_off1 > l_off2)
  {
    l_temp = l_off1 ;
    l_off1 = l_off2 ;
    l_off2 = l_temp ;
    df_xtemp = df_x1sel ;
    df_ytemp = df_y1sel ;
    df_x1sel = df_x2sel ;
    df_y1sel = df_y2sel ;
    df_x2sel = df_xtemp ;
    df_y2sel = df_ytemp ;
  }
  df_x1 = df_x1sel ;
  df_y1 = df_y1sel ;
  df_x2 = df_x2sel ;
  df_y2 = df_y2sel ;

  if (TRUE != Truncate_End_Pline ((BLOK **)&PTR__GTMP1, l_off1,
		      df_x1sel, df_y1sel, df_x1, df_y1) ||
      TRUE != Truncate_Begin_Pline ((BLOK **)&PTR__GTMP2, l_off2,
		      df_x2sel, df_y2sel, df_x2, df_y2) ||
      (b_close == TRUE &&
      TRUE != Truncate_End_Pline ((BLOK **)&PTR__GTMP4, l_off2,
		      df_x2sel, df_y2sel, df_x2, df_y2)) ||
      (b_close == TRUE &&
      TRUE != Truncate_Begin_Pline ((BLOK **)&PTR__GTMP4, l_off1,
		      df_x1sel, df_y1sel, df_x1, df_y1)))
  {
    usun_obiekt ((void *)PTR__GTMP1) ;
    usun_obiekt ((void *)PTR__GTMP2) ;
    if (b_close == TRUE)
    {
      usun_obiekt ((void *)PTR__GTMP4) ;
    }
    return FALSE ;
  }
  if (TRUE == b_close)
  {
    if (NULL == (ptrs_temp = Pline_Cat ((BLOK *)PTR__GTMP2,
						(BLOK **)&PTR__GTMP1)))
    {
      usun_obiekt ((void *)PTR__GTMP1) ;
      usun_obiekt ((void *)PTR__GTMP2) ;
      usun_obiekt ((void *)PTR__GTMP4) ;
      return FALSE ;
    }
    PTR__GTMP2 = (char *)ptrs_temp ;
    usun_obiekt ((void *)PTR__GTMP1) ;
    PTR__GTMP1 = PTR__GTMP2 ;
    PTR__GTMP2 = NULL ;
    if (b_close_ins == TRUE)
    {
      usun_obiekt ((void *)PTR__GTMP4) ;
      PTR__GTMP4 = NULL ;
    }
    else
    {
      usun_obiekt ((void *)PTR__GTMP1) ;
      PTR__GTMP1 = PTR__GTMP4 ;
      PTR__GTMP4 = NULL ;
    }
  }
  ptrh_beg = PTR__GTMP3 ;
  ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
  blokzap (ptrh_beg, ptrh_end, Anormalny, COPY_PUT, 0) ;

  l_off1 = (char*)PTR__GTMP1 - dane;
  l_off2 = (char*)PTR__GTMP2 - dane;

  ptr_parent = FIRSTB(PTR__GTMP3);
  if (ptr_parent != NULL) l_off3 = dane + dane_size - (char*)ptr_parent;

  zmien_atrybut (ptrh_beg, ptrh_end, Anormalny, Abad) ;

  if (ptr_parent == NULL)
  {
	  if (PTR__GTMP1 != NULL)
	  {
		  ptrh_beg = PTR__GTMP1;
		  ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
		  blokzap(ptrh_beg, ptrh_end, ANieOkreslony, COPY_PUT, 1);
	  }
	  if (PTR__GTMP2 != NULL)
	  {
		  ptrh_beg = PTR__GTMP2;
		  ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
		  blokzap(ptrh_beg, ptrh_end, ANieOkreslony, COPY_PUT, 1);
	  }
	  PTR__GTMP4 = PTR__GTMP1;
	  PTR__GTMP5 = PTR__GTMP2;
	  PTR__GTMP6 = PTR__GTMP3;
	  b__undopl = TRUE;
  }
  else
  {
	  if (PTR__GTMP1 != NULL)
	  {
		  ptrh_beg = PTR__GTMP1;
		  ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
		  blokzap(ptrh_beg, ptrh_end, ANieOkreslony, COPY_PUT, 1);

		  l_temp = ((NAGLOWEK*)PTR__GTMP1)->n + sizeof(NAGLOWEK);
		  PTR__GTMP1_ = malloc(l_temp);
		  if (PTR__GTMP1_ != NULL) movmem_((void*)PTR__GTMP1, (void*)PTR__GTMP1_, l_temp);
	  }
	  if (PTR__GTMP2 != NULL)
	  {
		  ptrh_beg = PTR__GTMP2;
		  ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
		  blokzap(ptrh_beg, ptrh_end, ANieOkreslony, COPY_PUT, 1);

		  l_temp = ((NAGLOWEK*)PTR__GTMP2)->n + sizeof(NAGLOWEK);
		  PTR__GTMP2_ = malloc(l_temp);
		  if (PTR__GTMP2_ != NULL) movmem_((void*)PTR__GTMP2, (void*)PTR__GTMP2_, l_temp);
	  }

	  if (PTR__GTMP2 != NULL) usun_obiekt((void*)PTR__GTMP2);
	  if (PTR__GTMP1 != NULL) usun_obiekt((void*)PTR__GTMP1);

	  ptr_parent = dane + dane_size - l_off3;

	  if (PTR__GTMP1_ != NULL)
	  {

		  l_off3 = ptr_parent - dane;
		  PTR__GTMP1 = (char*)dodaj_obiekt((void*)ptr_parent, (void*)PTR__GTMP1_);
		  ptr_parent = dane + l_off3;
		  free(PTR__GTMP1_);
	  }

	  if (PTR__GTMP2_ != NULL)
	  {
		  PTR__GTMP2 = (char*)dodaj_obiekt((void*)ptr_parent, (void*)PTR__GTMP2_);
		  free(PTR__GTMP2_);
	  }

	  PTR__GTMP4 = PTR__GTMP1;
	  PTR__GTMP5 = PTR__GTMP2;
	  PTR__GTMP6 = PTR__GTMP3;
	  b__undopl = TRUE;

  }

  return TRUE ;
}


int trim_line_to_quad(LINIA *ptrs_line, QUAD *quad, LINIA *line_tmp, LINIA *line_tmp1)
{
    LINIA line_tmp_w;
    double df_x1, df_y1;
    int i;
    static double xsel, ysel, x, y, t1, t2 ;
    BOOL b_add1, b_add2 ;
    double xm, ym;  //middlepoint

    df__te1 = - 1 ;    /*wstepnie df__te1, df__te2 - inicjowane sa poza odcinkiem <0, 1> */
    df__te2 = 2 ;

    xsel = (quad->xy[0]+quad->xy[2]+quad->xy[4]+quad->xy[6])/4.0;
    ysel = (quad->xy[1]+quad->xy[3]+quad->xy[5]+quad->xy[7])/4.0;

    najblizszyL_ (&xsel, &ysel, (void *)ptrs_line) ;
    if (FALSE == Check_if_Equal (ptrs_line->x1, ptrs_line->x2))
    {
        df__tsel = (xsel - ptrs_line->x1) / (ptrs_line->x2 - ptrs_line->x1) ;
    }
    else
    if (FALSE == Check_if_Equal (ptrs_line->y1, ptrs_line->y2))
    {
        df__tsel = (ysel - ptrs_line->y1) / (ptrs_line->y2 - ptrs_line->y1) ;
    }
    else
    {
        return 0;	/*dla odcinka 'zerowego'*/
    }

    memcpy(&line_tmp_w, ptrs_line, sizeof(LINIA));

    df_x1 = quad->xy[6];
    df_y1 = quad->xy[7];

    for (i = 0; i < 8; i += 2)
    {
        line_tmp_w.x1 = df_x1;
        line_tmp_w.y1 = df_y1;
        line_tmp_w.x2 = quad->xy[i];
        line_tmp_w.y2 = quad->xy[i+1];

        if (0 == IntersectionLL(ptrs_line, &line_tmp_w, &x, &y, &t1, &t2))
        {
            ;  /*linie rownolegle*/
        }
        else if (t2 >= 0 - o_male && t2 <= 1 + o_male)
        {
            new_intersection(x, y, t1);
        }

        df_x1 = line_tmp_w.x2;
        df_y1 = line_tmp_w.y2;
    }


    if (df__te1 < 0 - o_male   &&   df__te2 > 1 + o_male)
    {
        //check if maybe entire line inside quad
        xm = (ptrs_line->x1+ptrs_line->x2)/2.0;
        ym = (ptrs_line->y1+ptrs_line->y2)/2.0;
        if (Point_in_Quad(quad, xm, ym)) return -1;
        return 0; 	/*obiekt zle wybrany, nie ma nic do wyciecja*/
    }
    b_add1 = FALSE ;
    b_add2 = FALSE ;
    memcpy (line_tmp, ptrs_line, sizeof (LINIA)) ;
    if (df__te1 < 0 - o_male)
    {
        line_tmp->x1 = df__xe2 ;
        line_tmp->y1 = df__ye2 ;
    }
    else
    if (df__te2 > 1 + o_male)
    {
        line_tmp->x2 = df__xe1 ;
        line_tmp->y2 = df__ye1 ;
    }
    else
    {
        memcpy (line_tmp1, ptrs_line, sizeof (LINIA)) ;
        line_tmp->x2 = df__xe1 ;
        line_tmp->y2 = df__ye1 ;
        line_tmp1->x1 = df__xe2 ;
        line_tmp1->y1 = df__ye2 ;
        if (FALSE == Check_if_Equal (line_tmp1->x1, line_tmp1->x2) ||
            FALSE == Check_if_Equal (line_tmp1->y1, line_tmp1->y2))
        {
            b_add2 = TRUE ;
        }
    }
    if (FALSE == Check_if_Equal (line_tmp->x1, line_tmp->x2) ||
        FALSE == Check_if_Equal (line_tmp->y1, line_tmp->y2))
    {
        b_add1 = TRUE ;
    }
    if (b_add1 == FALSE && b_add2 == FALSE)
    {
    //not showing entire line
       return -1;
    }
    if (b_add1 == FALSE && b_add2 == TRUE)
    {
        memcpy ((void*)&line_tmp, (void*)&line_tmp1, sizeof (LINIA)) ;
        b_add1 = TRUE ;
        b_add2 = FALSE ;
    }

    if ((b_add1 == TRUE) && (b_add2 == TRUE)) return 2;  //drawing 2 lines
    else return 1;  //drawing first line
}


int trim_arc_to_quad (LUK *ptrs_arc, QUAD *quad, LUK *arc_tmp, LUK *arc_tmp1)
/*-----------------------------------------------------------------------------*/
{
    NAGLOWEK *nag ;
    BLOK * ptrs_b ;
    LINIA line_tmp_w = Ldef;
    WIELOKAT *ptrs_w;
    SPLINE *ptrs_s;
    double xsel, ysel, a_sel, angle1, angle2 ;
    void  *ad_h,  *obbl_h ;
    BOOL b_inter1, b_inter2 ;
    BOOL b_add1, b_add2 ;
    float df_x1, df_y1;
    int i;
    double x1, x2, y1, y2, t1, t2, angle;
    int l;
    double dx, dy;
    int dl;

    double xi[2];
    double yi[2];
    double tl[2];
    double aa[2];


    b_inter1 = b_inter2 = FALSE ;
    ad_h = (void *)ptrs_arc ;

    df_x1 = quad->xy[6];
    df_y1 = quad->xy[7];

    dx=0.0; dy=0.0;
    dl=0;

    for (i = 0; i < 8; i += 2) {
        line_tmp_w.x1 = df_x1;
        line_tmp_w.y1 = df_y1;
        line_tmp_w.x2 = quad->xy[i];
        line_tmp_w.y2 = quad->xy[i + 1];

        if (i==6){
            line_tmp_w.x2 = df_x1;
            line_tmp_w.y2 = df_y1;
            line_tmp_w.x1 = quad->xy[i];
            line_tmp_w.y1 = quad->xy[i + 1];
        }

        l=IntersectionLl (&line_tmp_w, ptrs_arc, &xi, &yi, &tl, &aa);

        if (l>0) {dx+=xi[0]; dy+=yi[0]; dl++;}
        if (l>1) {dx+=xi[1]; dy+=yi[1]; dl++;}

        df_x1 = line_tmp_w.x2;
        df_y1 = line_tmp_w.y2;
    }

    if (dl>0) {xsel=dx/dl; ysel=dy/dl;}
    else {
        xsel = (quad->xy[0] + quad->xy[2] + quad->xy[4] + quad->xy[6]) / 4.0;
        ysel = (quad->xy[1] + quad->xy[3] + quad->xy[5] + quad->xy[7]) / 4.0;
    }

    prostopadlel(xsel, ysel, &xsel, &ysel, (void *)ptrs_arc) ;

    a_sel = Angle_Normal (Atan2 (ysel - ptrs_arc->y, xsel - ptrs_arc->x)) ;

    df_x1 = quad->xy[6];
    df_y1 = quad->xy[7];

    for (i = 0; i < 8; i += 2)
    {
        line_tmp_w.x1 = df_x1;
        line_tmp_w.y1 = df_y1;
        line_tmp_w.x2 = quad->xy[i];
        line_tmp_w.y2 = quad->xy[i + 1];

        l = przeciecieLl_tt(&x1, &y1, &t1, &x2, &y2, &t2, &line_tmp_w, (void *)ptrs_arc);

        if (l == 0)
        {
            ;  /*linie rownolegle*/
        }
        else
        {
            if (t1 >= 0 - o_male && t1 <= 1 + o_male)
            {
                angle = Atan2(y1 - ptrs_arc->y, x1 - ptrs_arc->x);
                new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
            }
            if (l == 2 && t2 >= 0 - o_male && t2 <= 1 + o_male)
            {
                angle = Atan2(y2 - ptrs_arc->y, x2 - ptrs_arc->x);
                new_intersection_arc(&angle1, &angle2, a_sel, angle, &b_inter1, &b_inter2, ptrs_arc);
            }
        }

        df_x1 = line_tmp_w.x2;
        df_y1 = line_tmp_w.y2;
    }

    if (b_inter1 == FALSE && b_inter2 == FALSE)
    {
        return 0; 	/*obiekt zle wybrany, nie ma nic do wyciecja*/
    }
    ptrs_b = NULL ;
    b_add1 = FALSE ;
    b_add2 = FALSE ;
    memcpy (arc_tmp, ptrs_arc, sizeof (LUK)) ;
    if (b_inter1 == FALSE)
    {
        arc_tmp->kat1 = angle2 ;
    }
    else
    if (b_inter2 == FALSE)
    {
        arc_tmp->kat2 = angle1 ;
    }
    else
    {
        memcpy (arc_tmp1, ptrs_arc, sizeof (LUK)) ;
        ptrs_b = Parent_Block ((char  *)ptrs_arc) ;
        arc_tmp->kat2 = angle1 ;
        arc_tmp1->kat1 = angle2 ;
        if (FALSE == Check_if_Equal (Angle_Normal (arc_tmp1->kat1),
                                     Angle_Normal (arc_tmp1->kat2)))
        {
            b_add2 = TRUE ;
        }
    }
    if (FALSE == Check_if_Equal (Angle_Normal (arc_tmp->kat1),
                                 Angle_Normal (arc_tmp->kat2)))
    {
        b_add1 = TRUE ;
    }
    if (b_add1 == FALSE && b_add2 == TRUE)
    {
        memcpy (arc_tmp, arc_tmp1, sizeof (LUK)) ;
        b_add1 = TRUE ;
        b_add2 = FALSE ;
    }
    if (b_add1 == FALSE && b_add2 == FALSE)
    {
        return -1;
    }
    if ((b_add1 == TRUE) && (b_add2 == TRUE)) return 2;  //drawing 2 lines
    else return 1;  //drawing first line
}

//#include <math.h>
//#include <stdlib.h>  // for qsort

//#define M_PI 3.14159265358979323846

/* Helper: sign function */
static int sign(double v) {
    return (v > 0.0) ? 1 : ((v < 0.0) ? -1 : 0);
}

/* Helper: point inside quad (ray casting) */
static int point_in_quad(double px, double py, const QUAD *quad) {
    int i, j, c = 0;
    for (i = 0, j = 3; i < 4; j = i++) {
        double xi = quad->xy[2*i], yi = quad->xy[2*i + 1];
        double xj = quad->xy[2*j], yj = quad->xy[2*j + 1];
        if (((yi > py) != (yj > py)) &&
            (px < xi + (xj - xi) * (py - yi) / (yj - yi + (yj == yi ? 1e-10 : 0)))) {
            c = !c;
        }
    }
    return c;
}

/* Helper: ellipse parametric point */
static void ellipse_point(const ELLIPTICALARC *ea, double phi, double *px, double *py) {
    double cos_phi = cos(phi);
    double sin_phi = sin(phi);
    double cos_a = cos(ea->angle);
    double sin_a = sin(ea->angle);
    *px = ea->x + ea->rx * cos_phi * cos_a - ea->ry * sin_phi * sin_a;
    *py = ea->y + ea->rx * cos_phi * sin_a + ea->ry * sin_phi * cos_a;
}

/* Helper: solve quadratic for intersection parameter t in [0,1] */
static int solve_intersections(double a, double b, double c, double t[2]) {
    double disc = b*b - 4*a*c;
    if (disc < 0.0) return 0;
    double sqrt_disc = sqrt(disc);
    t[0] = (-b - sqrt_disc) / (2*a);
    t[1] = (-b + sqrt_disc) / (2*a);
    int n = 0;
    if (t[0] >= 0.0 && t[0] <= 1.0) t[n++] = t[0];
    if (t[1] >= 0.0 && t[1] <= 1.0 && fabs(t[1] - t[0]) > 1e-8) t[n++] = t[1];
    return n;
}

/* Helper: intersection of parametric line with quad edge */
static int line_quad_intersection(double x1, double y1, double x2, double y2,
                                  double ex1, double ey1, double ex2, double ey2,
                                  double *t) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double ex = ex2 - ex1;
    double ey = ey2 - ey1;
    double den = dx * ey - dy * ex;
    if (fabs(den) < 1e-10) return 0;  // parallel
    double num_t = (ex1 - x1) * ey - (ey1 - y1) * ex;
    *t = num_t / den;
    return (*t >= 0.0 && *t <= 1.0);
}

static int double_cmp(const void *a, const void *b) {
    return (*(const double*)a > *(const double*)b) - (*(const double*)a < *(const double*)b);
}

/* Main function */
int trim_ellipticalarc_to_quad(ELLIPTICALARC *ptrs_ea, QUAD *quad, ELLIPTICALARC *ea_tmp, ELLIPTICALARC *ea_tmp1) {
    LINIA L=Ldef;
    int ret;
    double int_x, int_y;

    if (!ptrs_ea || !quad || !ea_tmp || !ea_tmp1) return 0;

    /* Copy original to temps */
    memmove(ea_tmp, ptrs_ea, sizeof(ELLIPTICALARC));
    memmove(ea_tmp1, ptrs_ea, sizeof(ELLIPTICALARC));

    if (ea_tmp->kat2 < ea_tmp->kat1) ea_tmp->kat2 += (float)(2.0 * M_PI);
    if (ea_tmp1->kat2 < ea_tmp1->kat1) ea_tmp1->kat2 += (float)(2.0 * M_PI);

    double phi1 = ptrs_ea->angle + ptrs_ea->kat1;
    double phi2 = ptrs_ea->angle + ptrs_ea->kat2;

    /* Ensure phi2 > phi1 and sweep <= 2π */
    if (phi2 <= phi1) phi2 += 2.0 * M_PI;

    /* Compute end points */
    double end1_x, end1_y, end2_x, end2_y;
    ellipse_point(ptrs_ea, phi1, &end1_x, &end1_y);
    ellipse_point(ptrs_ea, phi2, &end2_x, &end2_y);

    int in1 = point_in_quad(end1_x, end1_y, quad);
    int in2 = point_in_quad(end2_x, end2_y, quad);

    if (!in1 && !in2) {
        /* Both outside - check if arc crosses quad */
        /* Sample points along arc */
        const int samples = 32;
        double phi_step = (phi2 - phi1) / samples;
        int crossings = 0;
        for (int i = 1; i < samples; i++) {
            double phi = phi1 + i * phi_step;
            double px, py;
            ellipse_point(ptrs_ea, phi, &px, &py);
            if (point_in_quad(px, py, quad)) crossings++;
        }
        if (crossings == 0) return 0;  // no intersection
    }

    /* Find intersection parameters */
    double t_list[8];  // max 4 edges × 2
    int n_t = 0;

    for (int e = 0; e < 4; e++)
    {
        int e2 = (e + 1) % 4;
        L.x1 = quad->xy[2*e];
        L.y1 = quad->xy[2*e + 1];
        L.x2 = quad->xy[2*e2];
        L.y2 = quad->xy[2*e2 + 1];
        ret = przeciecieLEA_ (&int_x, &int_y, &L, ptrs_ea);  //it's well working legacy procedure finding real intersection point between Line and Elliptical Arc
        if (ret)
        {
            double phi_int = atan2(int_y - ptrs_ea->y, int_x - ptrs_ea->x) - ptrs_ea->angle;
            if (phi_int < 0.0) phi_int += 2.0 * M_PI;
            if (phi_int < phi1) phi_int += 2.0 * M_PI;
            t_list[n_t++] = phi_int;
        }
    }

    if (n_t == 0) {
        /* No intersection - whole arc visible or hidden */
        if (in1 || in2) return 0;  // hidden
        return 1;  // visible
    }

    /* Sort intersection phis */
    qsort(t_list, n_t, sizeof(double), double_cmp);

    if (n_t == 0) return 0;

    if (n_t == 1)
    {
        /* One visible segment */
        //check which points lays inside quad
        double ea_start_x, ea_start_y, ea_end_x, ea_end_y;
        Get_EllipticalArc_EndPoints (ptrs_ea->x, ptrs_ea->y, ptrs_ea->rx, ptrs_ea->ry, ptrs_ea->angle, ptrs_ea->kat1, ptrs_ea->kat2, &ea_start_x, &ea_start_y, &ea_end_x, &ea_end_y);
        if (point_in_quad(ea_end_x, ea_end_y, quad))
        {
            ea_tmp->kat1 = (float) ea_tmp->kat1;
            ea_tmp->kat2 = (float) t_list[0] - ea_tmp->angle;
            if (ea_tmp->kat2 < ea_tmp->kat1) ea_tmp->kat2 += (float) (2.0 * M_PI);
        }
        else
        {
            ea_tmp->kat1 = (float) t_list[0] - ea_tmp->angle;
            ea_tmp->kat2 = ea_tmp->kat2;
            if (ea_tmp->kat2 < ea_tmp->kat1) ea_tmp->kat2 += (float) (2.0 * M_PI);
        }
        return 1;
    }

    /* Two visible segments */
    //ea_tmp->kat1 = ea_tmp->kat1;  //it stays as is
    ea_tmp->kat2 = (float)t_list[0]-ea_tmp->angle;
    if (ea_tmp->kat2 < ea_tmp->kat1) ea_tmp->kat2 += (float)(2.0 * M_PI);

    ea_tmp1->kat1 = (float)t_list[1]-ea_tmp1->angle;
    //ea_tmp1->kat2 = ea_tmp1->kat2;  //it stays as is
    if (ea_tmp1->kat2 < ea_tmp1->kat1) ea_tmp1->kat2 += (float)(2.0 * M_PI);

    return 2;
}

#undef __O_TRIM__