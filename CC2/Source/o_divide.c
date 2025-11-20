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

#define __O_DIVIDE__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<stdlib.h>
#include <math.h>
#include <stdio.h>
#include "bib_e.h"
#include "alffont.h"
#include "rysuj_e.h"
#include "bib_edit.h"
#include "o_pline.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_osnap.h"

#include "menu.h"

#include "leak_detector_c.h"

extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;
extern BOOL Add_Point (double df_x, double df_y) ;

static void divide_seg_no (void) ;
static void measure_seg_len (void) ;
static BOOL add_divide_block (double , double, long*, BOOL, int, void* ) ;
extern BOOL TTF_redraw;

extern NODE bnodes[MAX_NODES];
extern double curviness;
extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
extern void set_bnodes(SPLINE* ptrs_spline, float *ad_xy, int* node_count_);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void get_control_points(NODE n1, NODE n2, float points[]);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);
extern void layer_info (void);
extern void koniecSpline_(double *x, double *y, void *adr);
extern int Get_Ellipse_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double xy[]);
extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle, double df_eangle, double xy[]);

typedef struct
{
    BOOL     b_divide ;	  /*podziel, zmierz*/
    BOOL     b_pline ;	  /*podzial dotyczy calej polilinii,
			    czy jednego wskazanego segmentu*/
    int      i_seg_no ;	  /*liczba segmentow podzialu*/
    int      i_max_seg_no ;/*maksymalna liczba segmentow podzialu*/
    double   df_seg_len ; /*dlugosc segmentu*/
}
t_divide ;

enum DIVIDE
{ IDM_DIVIDE = 0, IDM_SEGMENT_NO, IDM_MEASURE, IDM_SEGMENT_LEN, IDM_MAX, IDM_UNDO = 6,
  IDM_PLINE = 8, IDM_SEG_PLINE,
  IDM_ALIGN_BLOCK = 10, IDM_NO_ALIGN_BLOCK } ;
#define MAX_DIVIDE_SEG_NO 1000
enum DIVIDE_MODE { DIVIDE_PL_1, DIVIDE_PL_2, DIVIDE_SINGLE } ; /*powtorzone w O_IMPORT*/

static t_divide s__divide = { TRUE, TRUE, 5, 1000, 20.0 } ;
static ESTR e__divide, e__measure ;
static int np__divide = -1, np__measure = -1 ;
static BOOL b__divide_point ;
static BOOL b__align_blok = TRUE ;
static double df__ipx, df__ipy ;

TMENU mDivide = {8, 0,0,31, 56,4,TADD | ICONS,CMNU,CMBR,CMTX, 0, COMNDmnr, 0, 0,0,&pmDivide, NULL, NULL} ;

/*-------------------------------------------------------------*/
#define STOS_SIZE 20
int sp = 0 ; 	/*indeks szczytu stosu*/
double stos [STOS_SIZE] ;


BOOL Push_Stos (double f)
/*---------------------*/
{
  BOOL b_ret ;

  if (sp < STOS_SIZE)
  {
    stos [sp++] = f ;
    b_ret = TRUE ;
  }
  else
  {
    memmove (stos + 1, stos, --sp) ;
    b_ret = FALSE ;
  }
  return b_ret ;
}


BOOL Pop_Stos (double *f)
/*----------------------*/
{
  BOOL b_ret ;

  if (sp > 0)
  {
   *f = stos [--sp] ;
   b_ret = TRUE ;
  }
  else
  {
    b_ret = FALSE ;
  }
  return b_ret ;
}

void Clear_Stos (void)
/*------------------*/
{
  sp = 0 ;
}

/*-------------------------------------------------------------*/

void undo_divide_proc (void)
/*------------------------*/
{
  long_long l_add_block_size, l_erase ;
  double df_temp ;
  NAGLOWEK *nag ;

  TTF_redraw=FALSE;
  if (FALSE == Pop_Stos (&df_temp))
  {
    return ;
  }
  l_add_block_size = (long)df_temp ;
  l_erase = 0 ;
  while (l_erase < l_add_block_size)
  {
     nag = (NAGLOWEK*)(dane + l_erase) ;
     rysuj_obiekt ((char *)nag, COPY_PUT, 0) ;
	 if (nag->obiekt == Otekst)
	 {
		 if (PTRS__Text_Style[((TEXT*)nag)->czcionka]->type == 2) TTF_redraw = TRUE;
	 }
     if (nag->obiekt == OdBLOK)
     {
       l_erase += sizeof(NAGLOWEK) + B3 + ((BLOK*)nag)->dlugosc_opisu_obiektu ;
     }
     else
     {
       l_erase += sizeof(NAGLOWEK) + nag->n ;
     }
  }
  Erase_First_Objects (l_add_block_size) ;

  CUR_OFF (X, Y) ;
  CUR_ON (X, Y) ;
  if (TTF_redraw) redraw();
}


static BOOL make_divide (int mode, double df_x, double df_y, long * ptrl_off, void * ptr_ob)
/*-----------------------------------------------------------------------------*/
{
  BOOL b_ret ;
  int typ_bak;

  if (b__divide_point == TRUE)
  {
      typ_bak=PointG.typ;
      PointG.typ=0;
      b_ret = Add_Point (df_x, df_y) ;
      PointG.typ=typ_bak;
      *ptrl_off = ((b_ret == TRUE) ? sizeof (T_Point) : 0 ) ;
  }
  else
  {
    b_ret = add_divide_block (df_x, df_y, ptrl_off, b__align_blok, mode, ptr_ob) ;
  }
  return b_ret ;
}

void set_b_align(BOOL opcja)
/*------------------------*/
{
  if (opcja==TRUE) b__align_blok=TRUE;
    else b__align_blok=FALSE;
}

BOOL get_b_align(void)
/*------------------*/
{
  return b__align_blok;
}

static void set_max_seg_no (void)
/*------------------------------*/
{
  char sk [MaxTextLen], *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;

  if (EOF == sprintf(sk, "%-4d", s__divide.i_max_seg_no))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (!get_string (sk, "", MaxTextLen, 0, 49)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)
	|| retval_no < 1 || buf_ret [0] <= 0)
  {
    return ;
  }
  if (buf_ret [0] > 1000)
  {
    buf_ret [0] = 1000 ;
  }
  s__divide.i_max_seg_no = (int)buf_ret [0] ;
  if (EOF == sprintf(sk, "%-4d", s__divide.i_max_seg_no))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  sk [5] = '\0' ;
  menu_par_new ((*mDivide.pola)[4].txt, sk) ;
}

static void set_divide_mode (int ev_number)
/*----------------------------------------*/
{
  usunstr (np__measure) ;
  usunstr (np__divide) ;
  np__measure = np__divide = -1 ;
  switch (ev_number)
  {
    case IDM_DIVIDE :
       komunikat0 (82) ;
       if (-1 != (np__divide = dodajstr (&e__divide)))
       {
	 sprintf (e__divide.st, "%#6d", s__divide.i_seg_no) ;
	 Out_Edited_Draw_Param ((ESTR *)&e__divide, TRUE) ;
       }
       break ;
    case IDM_MEASURE :
       komunikat0 (83) ;
       if (-1 != (np__measure = dodajstr (&e__measure)))
       {
	 sprintf (e__measure.st, "%#7.2lf", milimetryob (s__divide.df_seg_len)) ;
	 Out_Edited_Draw_Param ((ESTR *)&e__measure, TRUE) ;
       }
       break ;
    default :
       break ;
  }
}

double measure_line (LINIA *ptrs_line,
			   BOOL b_first_end,
			   double df_l0, double df_dl,
			   double *df_x, double *df_y)
/*----------------------------------------------------*/
{
  double df_line_len ;
  double df_t ;
  double df_x1, df_x2, df_y1, df_y2 ;

  df_line_len = Dist_PP (ptrs_line->x1, ptrs_line->y1, ptrs_line->x2, ptrs_line->y2) ;
  if (df_line_len >= df_l0 + df_dl &&
      FALSE == Check_if_Equal (df_line_len, 0))
  {
    df_t = (df_l0 + df_dl) / df_line_len ;
    df_x1 = ptrs_line->x1 ;
    df_y1 = ptrs_line->y1 ;
    df_x2 = ptrs_line->x2 ;
    df_y2 = ptrs_line->y2 ;
    if (FALSE == b_first_end)
    {
      df_x1 = ptrs_line->x2 ;
      df_y1 = ptrs_line->y2 ;
      df_x2 = ptrs_line->x1 ;
      df_y2 = ptrs_line->y1 ;
    }
    *df_x = df_x1 + df_t * (df_x2 - df_x1) ;
    *df_y = df_y1 + df_t * (df_y2 - df_y1) ;
  }
  return df_line_len - df_l0 - df_dl ;
}

static double measure_arc (LUK *ptrs_arc,
			   BOOL b_first_end,
			   double df_l0, double df_dl,
			   double *df_x, double *df_y)
/*----------------------------------------------------*/
{
  double df_line_len ;
  double df_d_angle, df_angle ;
  double df_angle1, df_angle2 ;

  df_angle1 = Angle_Normal (ptrs_arc->kat1) ;
  df_angle2 = Angle_Normal (ptrs_arc->kat2) ;
  if (df_angle1 > df_angle2)
  {
    df_angle2 += Pi2 ;
  }
  df_line_len = fabs (df_angle2 - df_angle1) * ptrs_arc->r ;
  if (df_line_len >= df_l0 + df_dl &&
      FALSE == Check_if_Equal (df_line_len, 0))
  {
    df_d_angle = (df_l0 + df_dl) / ptrs_arc->r ;
    df_angle = ptrs_arc->kat1 + df_d_angle ;
    if (b_first_end == FALSE)
    {
      df_angle = ptrs_arc->kat2 - df_d_angle ;
    }
    *df_x = ptrs_arc->x + ptrs_arc->r * cos (df_angle) ;
    *df_y = ptrs_arc->y + ptrs_arc->r * sin (df_angle) ;
  }
  return df_line_len - df_l0 - df_dl ;
}

static double measure_spline(SPLINE *ptrs_spline,
	BOOL b_first_end,
	double df_l0, double df_dl,
	double *df_x, double *df_y)
	/*----------------------------------------------------*/
{
	double df_line_len;
	float out_x[MaxNpts], out_y[MaxNpts];
	int npts;
	int i, j, k;
	double dx, dy, len_sum, len_last, len_goal, x_d, y_d, len_del, del_len, ratio;
	int first, last, inc;

	SPLINE tmp_spline = Splinedef;
	double bnodes_2_tangent_org, bnodes_2_tangent;
	int node_count;
	int points_count = 0;

	int NODES_NO;

	float* all_out_x;
	float* all_out_y;

	int exclude_point;


#define NPA 90

	npts = NPA;

	if (ptrs_spline->npts < 5)
	{
		all_out_x = malloc(90 * sizeof(float));
		if (all_out_x == NULL) return 0.0;
		all_out_y = malloc(90 * sizeof(float));
		if (all_out_y == NULL)
		{
			free(all_out_x);
			return 0.0;
		}

		calc_bspline(ptrs_spline->lp, ptrs_spline->npts, ptrs_spline->xy, npts, out_x, out_y);

		df_line_len = 0.0;

		for (i = 0; i < (npts - 1); i++)
		{
			dx = out_x[i + 1] - out_x[i];
			dy = out_y[i + 1] - out_y[i];
			df_line_len += (sqrt(dx * dx + dy * dy));
		}

		memmove(&all_out_x[0], &out_x, sizeof(float) * npts);
		memmove(&all_out_y[0], &out_y, sizeof(float) * npts);
		points_count = npts;
	}
	else
	{
		NODES_NO = ptrs_spline->lp / 2 + 4;
		
		all_out_x = malloc(90 * NODES_NO * sizeof(float));
		if (all_out_x == NULL) return 0.0;
		all_out_y = malloc(90 * NODES_NO * sizeof(float));
		if (all_out_y == NULL)
		{
			free(all_out_x);
			return 0.0;
		}

		curviness = ptrs_spline->xy[ptrs_spline->lp];

		tmp_spline.npts = 4;
		tmp_spline.lp = 8;
		tmp_spline.kolor = ptrs_spline->kolor;
		tmp_spline.typ = ptrs_spline->typ;

		node_count = 1;


		set_bnodes(ptrs_spline, ptrs_spline->xy, &node_count);

		calc_tangents(ptrs_spline->closed, &node_count, &bnodes_2_tangent);

		df_line_len = 0.0;

		if (ptrs_spline->closed == 0)
		{
			bnodes_2_tangent_org = bnodes[2].tangent;
			bnodes[2].tangent = bnodes_2_tangent;
			get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);
			
			for (i = 0; i < (npts - 1); i++)
			{
				dx = out_x[i + 1] - out_x[i];
				dy = out_y[i + 1] - out_y[i];
				df_line_len += (sqrt(dx * dx + dy * dy));
			}

			memmove(&all_out_x[points_count], &out_x, sizeof(float) * npts);
			memmove(&all_out_y[points_count], &out_y, sizeof(float) * npts);
			points_count += npts;

			bnodes[2].tangent = bnodes_2_tangent_org;
		}

		for (k = 2; k < node_count - 2; k++)  //1 - extra_node
		{
			get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);
			
			for (i = 0; i < (npts - 1); i++)
			{
				dx = out_x[i + 1] - out_x[i];
				dy = out_y[i + 1] - out_y[i];
				df_line_len += (sqrt(dx * dx + dy * dy));
			}

			memmove(&all_out_x[points_count], &out_x, sizeof(float) * npts);
			memmove(&all_out_y[points_count], &out_y, sizeof(float) * npts);
			points_count += npts;

		}

		if ((node_count > 3) && (ptrs_spline->closed == 0))
		{
			get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);
			
			for (i = 0; i < (npts - 1); i++)
			{
				dx = out_x[i + 1] - out_x[i];
				dy = out_y[i + 1] - out_y[i];
				df_line_len += (sqrt(dx * dx + dy * dy));
			}

			memmove(&all_out_x[points_count], &out_x, sizeof(float)* npts);
			memmove(&all_out_y[points_count], &out_y, sizeof(float)* npts);
			points_count += npts;
		}
	}
	
	if (df_line_len >= df_l0 + df_dl &&
		FALSE == Check_if_Equal(df_line_len, 0))
	{
		//finding dividion point for the length df_l0 + df_dl
		if ((ptrs_spline->npts > 4) && (ptrs_spline->closed == 1)) exclude_point = 0;
		else exclude_point = 1;
		if (b_first_end == TRUE)
		{
			first = 0; last = points_count - 1; inc = 1;
		}
		else
		{
			first = points_count - 1; last = -1; inc = -1;
		}
		
		x_d = all_out_x[0];
		y_d = all_out_y[0];
		len_sum = 0.0;
		len_last = 0.0;
		len_goal = df_l0 + df_dl;
		i = 0;
		for (i = first; i != last; i += inc)
		{
			if (b_first_end == TRUE)
			{
				dx = all_out_x[i + 1] - all_out_x[i];
				dy = all_out_y[i + 1] - all_out_y[i];
				x_d = all_out_x[i + 1];
				y_d = all_out_y[i + 1];
			}
			else
			{
				dx = all_out_x[i - 1] - all_out_x[i];
				dy = all_out_y[i - 1] - all_out_y[i];
				x_d = all_out_x[i - 1];
				y_d = all_out_y[i - 1];
			}
			len_del = (sqrt(dx*dx + dy * dy));
			len_sum += len_del;
			if (len_sum >= len_goal)
			{
				del_len = len_sum - len_goal;
				if (len_del>0.0)
				{
					ratio = del_len / len_del;
					x_d -= (dx * ratio);
					y_d -= (dy * ratio);
				}
				break;
			}
			len_last = len_sum;
		}
		
		*df_x = x_d;
		*df_y = y_d;
	}
	//returning remaining length
	free(all_out_x);
	free(all_out_y);

	return df_line_len - df_l0 - df_dl;
}


static double measure_ellipse(ELLIPSE *ptrs_ellipse,
                             BOOL b_first_end,
                             double df_l0, double df_dl,
                             double *df_x, double *df_y, int numpoints0, double xy0[])
/*----------------------------------------------------------------------------------*/
{
    double df_line_len=0.0;
    double xy_[256];
    double *xy;
    int npts;
    int i, j, k;
    double dx, dy, len_sum, len_last, len_goal, x_d, y_d, len_del, del_len, ratio;
    int first, last, inc;
    ELLIPSE *tmp_ellipse = (ELLIPSE *)ptrs_ellipse;
    int numpoints;

    if (xy0==NULL) {
        numpoints = Get_Ellipse_Points(tmp_ellipse->x, tmp_ellipse->y, tmp_ellipse->rx, tmp_ellipse->ry,
                                       tmp_ellipse->angle, xy_);
        xy_[numpoints] = xy_[0];
        xy_[numpoints + 1] = xy_[1];
        numpoints += 2;
        xy=xy_;
    }
    else {
        xy=xy0;
        numpoints=numpoints0;
    }
    for (i = 0; i < (numpoints - 2); i+=2)
    {
        dx = xy[i + 2] - xy[i];
        dy = xy[i + 3] - xy[i+1];
        df_line_len += (sqrt(dx * dx + dy * dy));
    }

    if (df_line_len >= df_l0 + df_dl &&
        FALSE == Check_if_Equal(df_line_len, 0))
    {
        //finding dividion point for the length df_l0 + df_dl

        if (b_first_end == TRUE)
        {
            first = 0; last = numpoints - 2; inc = 2;
        }
        else
        {
            first = numpoints - 2; last = -2; inc = -2;
        }

        x_d = xy[0];
        y_d = xy[1];
        len_sum = 0.0;
        len_last = 0.0;
        len_goal = df_l0 + df_dl;
        i = 0;
        for (i = first; i != last; i += inc)
        {
            if (b_first_end == TRUE)
            {
                dx = xy[i + 2] - xy[i];
                dy = xy[i + 3] - xy[i+1];
                x_d = xy[i + 2];
                y_d = xy[i + 3];
            }
            else
            {
                dx = xy[i - 2] - xy[i];
                dy = xy[i - 3] - xy[-1];
                x_d = xy[i - 2];
                y_d = xy[i - 3];
            }
            len_del = (sqrt(dx*dx + dy * dy));
            len_sum += len_del;
            if (len_sum >= len_goal)
            {
                del_len = len_sum - len_goal;
                if (len_del>0.0)
                {
                    ratio = del_len / len_del;
                    x_d -= (dx * ratio);
                    y_d -= (dy * ratio);
                }
                break;
            }
            len_last = len_sum;
        }

        *df_x = x_d;
        *df_y = y_d;
    }

    return df_line_len - df_l0 - df_dl;
}

static double measure_ellipticalarc(ELLIPTICALARC *ptrs_ellipticalarc,
                              BOOL b_first_end,
                              double df_l0, double df_dl,
                              double *df_x, double *df_y, int numpoints0, double xy0[])
/*-----------------------------------------------------------------------------------*/
{
    double df_line_len=0.0;
    double xy_[256];
    double *xy;
    int npts;
    int i, j, k;
    double dx, dy, len_sum, len_last, len_goal, x_d, y_d, len_del, del_len, ratio;
    int first, last, inc;
    ELLIPTICALARC *tmp_ellipse = (ELLIPTICALARC *)ptrs_ellipticalarc;
    int numpoints;

    if (xy0==NULL) {
        numpoints = Get_EllipticalArc_Points(tmp_ellipse->x, tmp_ellipse->y, tmp_ellipse->rx, tmp_ellipse->ry,
                                             tmp_ellipse->angle, tmp_ellipse->kat1, tmp_ellipse->kat2, xy_);
        xy = xy_;
    }
    else {
        xy=xy0;
        numpoints=numpoints0;
    }
    for (i = 0; i < (numpoints - 2); i+=2)
    {
        dx = xy[i + 2] - xy[i];
        dy = xy[i + 3] - xy[i+1];
        df_line_len += (sqrt(dx * dx + dy * dy));
    }


    if (df_line_len >= df_l0 + df_dl &&
        FALSE == Check_if_Equal(df_line_len, 0))
    {
        //finding dividion point for the length df_l0 + df_dl

        if (b_first_end == TRUE)
        {
            first = 0; last = numpoints - 2; inc = 2;
            x_d = xy[0];
            y_d = xy[1];
        }
        else
        {
            first = numpoints - 2; last = 2; inc = -2;
            x_d = xy[numpoints-2];
            y_d = xy[numpoints-1];
        }

        len_sum = 0.0;
        len_last = 0.0;
        len_goal = df_l0 + df_dl;
        i = 0;
        for (i = first; i != last; i += inc)
        {
            if (b_first_end == TRUE)
            {
                dx = xy[i + 2] - xy[i];
                dy = xy[i + 3] - xy[ i + 1];
                x_d = xy[i + 2];
                y_d = xy[i + 3];
            }
            else
            {
                dx = xy[i - 2] - xy[i];
                dy = xy[i - 1] - xy[i + 1];
                x_d = xy[i - 2];
                y_d = xy[i - 1];
            }
            len_del = (sqrt(dx*dx + dy * dy));
            len_sum += len_del;
            if (len_sum >= len_goal)
            {
                del_len = len_sum - len_goal;
                if (len_del>0.0)
                {
                    ratio = del_len / len_del;
                    x_d -= (dx * ratio);
                    y_d -= (dy * ratio);
                }
                break;
            }
            len_last = len_sum;
        }

        *df_x = x_d;
        *df_y = y_d;
    }

    return df_line_len - df_l0 - df_dl;
}


static BOOL divide_line (void *ptr_object, double df_x, double df_y)
/*---------------------------------------------------------------*/
{
  int i ;
  double df_line_len, df_l0, df_seg_len, df_line_rem ;
  LINIA *ptrs_line, s_line ;
  BOOL b_first_end, b_divide ;
  long l_off, l_size_add_ob ;

  l_size_add_ob = 0 ;
  memcpy ((void*)&s_line, ptr_object, sizeof(LINIA)) ;
  ptrs_line = &s_line ;
  df_seg_len = s__divide.df_seg_len ;
  if (s__divide.b_divide == TRUE)
  {
     df_line_len = Dist_PP (ptrs_line->x1, ptrs_line->y1, ptrs_line->x2, ptrs_line->y2) ;
     df_seg_len = df_line_len / s__divide.i_seg_no ;
  }
  koniecL_ (&df_x, &df_y, ptr_object) ;
  b_first_end = TRUE ;
  if (TRUE == Check_if_Equal (ptrs_line->x2, df_x) &&
      TRUE == Check_if_Equal (ptrs_line->y2, df_y))
  {
    b_first_end = FALSE ;
  }
  i = 0 ;
  df_l0 = 0 ;
  do
  {
    df_line_rem = measure_line (ptrs_line,b_first_end, df_l0,  df_seg_len,
      &df_x, &df_y) ;
    if (TRUE == Check_if_GT (df_line_rem, 0))
    {
      b_divide = make_divide (DIVIDE_SINGLE, df_x, df_y, &l_off, (void *)ptrs_line) ;
      l_size_add_ob += l_off ;
      if (b_divide == FALSE)
      {
	break ;
      }
      df_l0 += df_seg_len ;
      i++ ;
    }
  }
  while (i < s__divide.i_max_seg_no && TRUE == Check_if_GT (df_line_rem, 0)) ;
  if (l_size_add_ob > 0)
  {
    Push_Stos ((double)l_size_add_ob) ;
  }
  return b_divide ;
}

static BOOL divide_circle (void *ptr_object, double df_x, double df_y)
/*-----------------------------------------------------------------*/
{
  OKRAG * ptrs_circle, s_circle ;
  int i ;
  double df_d_angle, df_angle ;
  BOOL b_divide ;
  long l_off, l_size_add_ob ;

  l_size_add_ob = 0 ;
  memcpy ((void*)&s_circle, ptr_object, sizeof (OKRAG)) ;
  ptrs_circle = &s_circle ;
  df_x = df_x ;
  df_y = df_y ;
  if (TRUE == Check_if_Equal (ptrs_circle->r, 0))
  {
    return TRUE ;
  }
  df_d_angle = Pi2 / s__divide.i_seg_no ;
  if (s__divide.b_divide == FALSE)
  {
    df_d_angle = s__divide.df_seg_len / ptrs_circle->r ;
  }
  i = 0 ;
  df_angle = 0 ;
  do
  {
    df_x = ptrs_circle->x + ptrs_circle->r * cos (df_angle) ;
    df_y = ptrs_circle->y + ptrs_circle->r * sin (df_angle) ;
    b_divide = make_divide (DIVIDE_SINGLE, df_x, df_y, &l_off, (void *)ptrs_circle) ;
    l_size_add_ob += l_off ;
    if (b_divide == FALSE)
    {
      break ;
    }
    df_angle += df_d_angle ;
    i++ ;
  }
  while (i < s__divide.i_max_seg_no && Pi2 > df_angle) ;
  if (l_size_add_ob > 0)
  {
    Push_Stos ((double)l_size_add_ob) ;
  }
  return b_divide ;
}

static BOOL divide_spline(void *ptr_object, double df_x, double df_y)
/*---------------------------------------------------------------*/
{
	int i, j, k;
	double df_line_len, df_l0, df_seg_len, df_line_rem;
	double x2, y2;
	double df_angle1, df_angle2;
	SPLINE *ptrs_spline, s_spline;
	BOOL b_first_end;
	BOOL b_divide;
	long l_off, l_size_add_ob, l_off__;
	float out_x[MaxNpts], out_y[MaxNpts];
	int npts;
	double dx, dy;
	double min_distance;

	SPLINE tmp_spline = Splinedef;
	double bnodes_2_tangent_org, bnodes_2_tangent;
	int node_count;

#define NPA 90
	b_divide = FALSE;
	l_size_add_ob = 0;
	memcpy((void*)&s_spline, ptr_object, sizeof(SPLINE));
	ptrs_spline = &s_spline;
	df_seg_len = s__divide.df_seg_len;
	if (s__divide.b_divide == TRUE)
	{
		npts = NPA;

		if (ptrs_spline->npts < 5)
		{
			calc_bspline(s_spline.lp, s_spline.npts, s_spline.xy, npts, out_x, out_y);

			df_line_len = 0.0;

			for (i = 0; i < (npts - 1); i++)
			{
				dx = out_x[i + 1] - out_x[i];
				dy = out_y[i + 1] - out_y[i];
				df_line_len += (sqrt(dx * dx + dy * dy));
			}
		}
		else
		{
			df_line_len = 0.0;

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

				for (i = 0; i < (npts - 1); i++)
				{
					dx = out_x[i + 1] - out_x[i];
					dy = out_y[i + 1] - out_y[i];
					df_line_len += (sqrt(dx * dx + dy * dy));
				}

				bnodes[2].tangent = bnodes_2_tangent_org;
			}
			for (k = 2; k < node_count - 2; k++)  //1 - extra_node
			{
				get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

				for (i = 0; i < (npts - 1); i++)
				{
					dx = out_x[i + 1] - out_x[i];
					dy = out_y[i + 1] - out_y[i];
					df_line_len += (sqrt(dx * dx + dy * dy));
				}
			}
			if ((node_count > 3) && (ptrs_spline->closed == 0))
			{
				get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

				for (i = 0; i < (npts - 1); i++)
				{
					dx = out_x[i + 1] - out_x[i];
					dy = out_y[i + 1] - out_y[i];
					df_line_len += (sqrt(dx * dx + dy * dy));
				}
			}
		}
		df_seg_len = df_line_len / s__divide.i_seg_no;
	}
	koniecSpline_(&df_x, &df_y, ptr_object);

	x2 = s_spline.xy[s_spline.lp - 2];
	y2 = s_spline.xy[s_spline.lp - 1];

	b_first_end = TRUE;
	if (TRUE == Check_if_Equal(x2, df_x) &&
		TRUE == Check_if_Equal(y2, df_y))
	{
		b_first_end = FALSE;
	}
	i = 0;

	df_l0 = 0.0;

	do
	{
		df_line_rem = measure_spline(ptrs_spline, b_first_end, df_l0, df_seg_len,
			&df_x, &df_y);
		if ((ptrs_spline->npts > 4) && (ptrs_spline->closed == 1)) min_distance = -0.00001;
		else min_distance = 0;
		if (TRUE == Check_if_GT(df_line_rem, min_distance))
		{
			b_divide = make_divide(DIVIDE_SINGLE, df_x, df_y, &l_off, (void *)ptrs_spline);
			l_size_add_ob += l_off;
			if (b_divide == FALSE)
			{
				break;
			}
			df_l0 += df_seg_len;
			i++;
		}
	} while (i < s__divide.i_max_seg_no && TRUE == Check_if_GT(df_line_rem, 0));
	if (l_size_add_ob > 0)
	{
		Push_Stos((double)l_size_add_ob);
	}
	return b_divide;
}


static BOOL divide_ellipse(void *ptr_object, double df_x, double df_y)
{  double xy[256];
    int numpoints;
    int i;
    double dx, dy;
    ELLIPSE *ptr_ellipse, s_ellipse;
    double df_line_len=0.0, df_l0, df_seg_len, df_line_rem;
    BOOL b_first_end;
    BOOL b_divide;
    double x2, y2;
    long l_off, l_size_add_ob, l_off__;
    double min_distance;

    memcpy((void*)&s_ellipse, ptr_object, sizeof(ELLIPSE));
    ptr_ellipse = &s_ellipse;

    b_divide = FALSE;
    l_size_add_ob = 0 ;

    numpoints=Get_Ellipse_Points(ptr_ellipse->x, ptr_ellipse->y,ptr_ellipse->rx, ptr_ellipse->ry, ptr_ellipse->angle, xy);
    xy[numpoints] = xy[0];
    xy[numpoints+1] = xy[1];
    numpoints+=2;

    df_seg_len = s__divide.df_seg_len ;
    if (s__divide.b_divide == TRUE)
    {
        for (i = 0; i < (numpoints - 2); i += 2)
        {
            dx = xy[i + 2] - xy[i];
            dy = xy[i + 3] - xy[i + 1];
            df_line_len += (sqrt(dx * dx + dy * dy));
        }
        df_seg_len = df_line_len / s__divide.i_seg_no;
    }

    b_first_end = TRUE;

    i = 0;
    df_l0 = 0;

    df_line_rem = measure_ellipse(ptr_ellipse, b_first_end, df_l0, 0, &df_x, &df_y, numpoints, xy);
    b_divide = make_divide(DIVIDE_SINGLE, df_x, df_y, &l_off__, (void*)ptr_ellipse);
    l_size_add_ob += l_off__;
    if (b_divide == TRUE) {
        do {
            df_line_rem = measure_ellipse(ptr_ellipse, b_first_end, df_l0, df_seg_len, &df_x, &df_y, numpoints, xy);
            min_distance = 0.00;

            if (TRUE == Check_if_GT(df_line_rem, min_distance)) {
                b_divide = make_divide(DIVIDE_SINGLE, df_x, df_y, &l_off, (void *) ptr_ellipse);
                l_size_add_ob += l_off;
                if (b_divide == FALSE) {
                    break;
                }
                df_l0 += df_seg_len;
                i++;
            }
        } while (i < s__divide.i_max_seg_no && TRUE == Check_if_GT(df_line_rem, 0));
    }
    if (l_size_add_ob > 0)
    {
        Push_Stos((double)l_size_add_ob);
    }
    return b_divide;


}

static BOOL divide_ellipticalarc(void *ptr_object, double df_x, double df_y)
{

    {  double xy[256];
        int numpoints;
        int i;
        double dx, dy;
        ELLIPTICALARC *ptr_ellipticalarc, s_ellipticalarc;
        double df_line_len=0.0, df_l0, df_seg_len, df_line_rem;
        BOOL b_first_end;
        BOOL b_divide;
        double x2, y2;
        long l_off, l_size_add_ob, l_off__;
        double min_distance;

        memcpy((void*)&s_ellipticalarc, ptr_object, sizeof(ELLIPTICALARC));
        ptr_ellipticalarc = &s_ellipticalarc;

        b_divide = FALSE;
        l_size_add_ob = 0 ;

        numpoints=Get_EllipticalArc_Points (ptr_ellipticalarc->x, ptr_ellipticalarc->y,ptr_ellipticalarc->rx, ptr_ellipticalarc->ry, ptr_ellipticalarc->angle, ptr_ellipticalarc->kat1, ptr_ellipticalarc->kat2,xy);

        df_seg_len = s__divide.df_seg_len ;
        if (s__divide.b_divide == TRUE)
        {
            for (i = 0; i < (numpoints - 2); i+=2)
            {
                dx = xy[i+2] - xy[i];
                dy = xy[i+3] - xy[i+1];
                df_line_len += (sqrt(dx * dx + dy * dy));
            }
            df_seg_len = df_line_len / s__divide.i_seg_no;
        }

        koniecEA_XY_ (&df_x, &df_y, numpoints, xy) ;
        x2 = xy[numpoints-2];
        y2 = xy[numpoints-1];

        b_first_end = TRUE;
        if (TRUE == Check_if_Equal (x2, df_x) &&
            TRUE == Check_if_Equal (y2, df_y))
        {
            b_first_end = FALSE ;
        }

        i = 0;
        df_l0 = 0;

        do
        {
            df_line_rem = measure_ellipticalarc(ptr_ellipticalarc, b_first_end, df_l0, df_seg_len,&df_x, &df_y, numpoints, xy);
            min_distance = 0.00;

            if (TRUE == Check_if_GT(df_line_rem, min_distance))
            {
                b_divide = make_divide(DIVIDE_SINGLE, df_x, df_y, &l_off, (void *)ptr_ellipticalarc);
                l_size_add_ob += l_off;
                if (b_divide == FALSE)
                {
                    break;
                }
                df_l0 += df_seg_len;
                i++;
            }
        } while (i < s__divide.i_max_seg_no && TRUE == Check_if_GT(df_line_rem, 0));
        if (l_size_add_ob > 0)
        {
            Push_Stos((double)l_size_add_ob);
        }
        return b_divide;

    }


}

static BOOL divide_arc (void *ptr_object, double df_x, double df_y)
/*---------------------------------------------------------------*/
{
  int i ;
  double df_line_len, df_l0, df_seg_len, df_line_rem ;
  double x2, y2 ;
  double df_angle1, df_angle2 ;
  LUK *ptrs_arc, s_arc ;
  BOOL b_first_end ;
  BOOL b_divide ;
  long l_off, l_size_add_ob ;
  BOOL b_e;

  b_divide = FALSE;
  l_size_add_ob = 0 ;
  memcpy ((void*)&s_arc, ptr_object, sizeof (LUK)) ;
  ptrs_arc = &s_arc ;
  df_seg_len = s__divide.df_seg_len ;
  if (s__divide.b_divide == TRUE)
  {
     df_angle1 = Angle_Normal (ptrs_arc->kat1) ;
     df_angle2 = Angle_Normal (ptrs_arc->kat2) ;
     if (df_angle1 > df_angle2)
     {
       df_angle2 += Pi2 ;
     }
     df_line_len = fabs (df_angle2 - df_angle1) * ptrs_arc->r ;
     df_seg_len = df_line_len / s__divide.i_seg_no ;
  }
  koniecl_ (&df_x, &df_y, ptr_object) ;
  x2 = ptrs_arc->x + ptrs_arc->r * cos (ptrs_arc->kat2) ;
  y2 = ptrs_arc->y + ptrs_arc->r * sin (ptrs_arc->kat2) ;
  b_first_end = TRUE ;
  if (TRUE == Check_if_Equal (x2, df_x) &&
      TRUE == Check_if_Equal (y2, df_y))
  {
    b_first_end = FALSE ;
  }
  i = 0 ;
  df_l0 = 0 ;
  do
  {
    df_line_rem = measure_arc (ptrs_arc, b_first_end, df_l0,  df_seg_len,
      &df_x, &df_y) ;
    if (TRUE == Check_if_GT (df_line_rem, 0))
    {
      b_divide = make_divide (DIVIDE_SINGLE, df_x, df_y, &l_off, (void *)ptrs_arc) ;
      l_size_add_ob += l_off ;
      if (b_divide == FALSE)
      {
	break ;
      }
      df_l0 += df_seg_len ;
      i++ ;
    }
  }
  while (i < s__divide.i_max_seg_no && TRUE == Check_if_GT (df_line_rem, 0)) ;
  if (l_size_add_ob > 0)
  {
    Push_Stos ((double)l_size_add_ob) ;
  }
  return b_divide ;
}


BOOL divide_pline (BLOK *ptrs_block, void *ptr_object, double df_x, double df_y, double odleglosc_00)
/*-------------------------------------------------------------------------------------------------*/
{
  void * ptr_first_ob, * ptr_last_ob ;
  BOOL b_first_end, b_last_end ;
  double df_x_beg, df_y_beg, df_x_end, df_y_end ;
  double df_seg1_len, df_seg0_len, df_seg_len ;
  double df_line_len, df_l0, df_line_rem ;
  double x1, x2, y1, y2 ;
  long_long off, offk, ad ;
  int i ;
  NAGLOWEK *nag ;
  LINIA *ptrs_line, s_line ;
  LINIA3D *ptrs_line3D, s_line3D ;
  LUK *ptrs_arc, s_arc ;
  SPLINE *ptrs_spline, s_spline;
  BOOL b_divide, b_close ;
  long l_off, l_size_add_ob, l_off__;
  double odleglosc_0, odleglosc_01;
  double min_distance;

  odleglosc_0=jednostkiOb (odleglosc_00) ;
  b_divide=FALSE;
  l_size_add_ob = 0 ;
  ptr_object = ptr_object ;
  Get_Begin_Pline ((void *)ptrs_block, &ptr_first_ob,  &b_first_end,
		      &df_x_beg, &df_y_beg) ;
  Get_End_Pline ((void *)ptrs_block, &ptr_last_ob, &b_last_end,
		  df_x_beg, df_y_beg, &df_x_end, &df_y_end) ;
  b_close = FALSE ;
  if (TRUE == Check_if_Equal (df_x_beg, df_x_end) &&
      TRUE == Check_if_Equal (df_y_beg, df_y_end))
  {
    b_close = TRUE ;
  }
  df_line_len = Get_Len_Pline (ptrs_block) ;
  df_seg1_len = s__divide.df_seg_len ;
  if (s__divide.b_divide == TRUE)
  {
     df_seg1_len = df_line_len / s__divide.i_seg_no ;
  }
  df_seg0_len = df_seg1_len ;
  if (Dist_PP (df_x_beg, df_y_beg, df_x, df_y) >
      Dist_PP (df_x_end, df_y_end, df_x, df_y))
  {
    if (s__divide.b_divide == FALSE)
    {
      df_seg0_len = fmod (df_line_len, df_seg1_len) ;
    }
    if (s__divide.i_max_seg_no * df_seg1_len + df_seg0_len < df_line_len)
    {
      df_seg0_len = df_line_len - s__divide.i_max_seg_no * df_seg1_len ;
    }
  }
  if (TRUE == Check_if_Equal (df_seg0_len, 0))
  {
    df_seg0_len = df_seg1_len ;
  }
  off =  (char  *)ptrs_block - dane ;
  offk = off  + ptrs_block->n + sizeof (NAGLOWEK) - 1 ;
  odleglosc_01=fmod(odleglosc_0,df_seg0_len);
  df_seg_len = df_seg0_len - odleglosc_01;
  i = 0 ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Olinia :
	memcpy ((void*)&s_line, (void*)nag, sizeof (LINIA)) ;
	ptrs_line = &s_line ;
	df_l0 = 0 ;
	do
	{
	  df_line_rem = measure_line (ptrs_line, TRUE,
	       df_l0, df_seg_len, &df_x, &df_y);
	  if (TRUE == Check_if_GT (df_line_rem, 0))
	  {
	    df_l0 += df_seg_len ;
	    i++ ;
	    df_seg_len = df_seg1_len ;
	    b_divide = make_divide (DIVIDE_PL_1, df_x, df_y, &l_off, (void *)ptrs_line) ;
	    l_size_add_ob += l_off ;
	    if (b_divide == FALSE)
	    {
	      goto aaa ;
	    }
	    off+= l_off;
	    offk += l_off;
	  }
	}
	while (i < s__divide.i_max_seg_no && TRUE == Check_if_GT (df_line_rem, 0)) ;
	if (TRUE == Check_if_LT (df_line_rem, 0))
	{
	  df_seg_len = - df_line_rem ;
	}
	df_x_beg = ptrs_line->x2 ;
	df_y_beg = ptrs_line->y2 ;
	break ;
	  case Ospline:
		  memcpy((void*)&s_spline, (void*)nag, sizeof(SPLINE));
		  ptrs_spline = &s_spline;

		  df_l0 = 0;

		  do
		  {

			  df_line_rem = measure_spline(ptrs_spline, TRUE,
				  df_l0, df_seg_len, &df_x, &df_y);
			  if ((ptrs_spline->npts > 4) && (ptrs_spline->closed == 1)) min_distance = -0.00001;
			  else min_distance = 0;
			  if (TRUE == Check_if_GT(df_line_rem, min_distance))
			  {
				  df_l0 += df_seg_len;
				  i++;
				  df_seg_len = df_seg1_len;
				  b_divide = make_divide(DIVIDE_PL_1, df_x, df_y, &l_off, (void *)ptrs_spline);
				  l_size_add_ob += l_off;
				  if (b_divide == FALSE)
				  {
					  goto aaa;
				  }
				  off += l_off;
				  offk += l_off;
			  }
		  } while (i < s__divide.i_max_seg_no && TRUE == Check_if_GT(df_line_rem, 0));

		  if (TRUE == Check_if_LT(df_line_rem, 0))
		  {
			  df_seg_len = -df_line_rem;
		  }
		  df_x_beg = ptrs_spline->xy[ptrs_spline->lp - 2];
		  df_y_beg = ptrs_spline->xy[ptrs_spline->lp - 1];
		  break;
      case Oluk :
	memcpy ((void*)&s_arc, (void*)nag, sizeof (LUK)) ;
	ptrs_arc = &s_arc ;
	x1 = ptrs_arc->x + ptrs_arc->r * cos (ptrs_arc->kat1) ;
	y1 = ptrs_arc->y + ptrs_arc->r * sin (ptrs_arc->kat1) ;
	x2 = ptrs_arc->x + ptrs_arc->r * cos (ptrs_arc->kat2) ;
	y2 = ptrs_arc->y + ptrs_arc->r * sin (ptrs_arc->kat2) ;
	if (TRUE == Check_if_Equal (df_x_beg, x2) &&
	    TRUE == Check_if_Equal (df_y_beg, y2))
	{
	  b_first_end = FALSE ;
	  df_x_beg = x1 ;
	  df_y_beg = y1 ;
	}
	else
	{
	  b_first_end = TRUE ;
	  df_x_beg = x2 ;
	  df_y_beg = y2 ;
	}
	df_l0 = 0 ;
	do
	{
	  df_line_rem = measure_arc (ptrs_arc, b_first_end, df_l0,  df_seg_len,
	    &df_x, &df_y);
	  if (TRUE == Check_if_GT (df_line_rem, 0))
	  {
	    df_l0 += df_seg_len ;
	    i++ ;
	    df_seg_len = df_seg1_len ;
	    b_divide = make_divide (
		     ((b_first_end == TRUE) ? DIVIDE_PL_1 : DIVIDE_PL_2),
		     df_x, df_y, &l_off, (void *)ptrs_arc) ;
	    l_size_add_ob += l_off ;
	    if (b_divide == FALSE)
	    {
	      goto aaa ;
	    }
	    off+= l_off;
	    offk += l_off;
	  }
	}
	while (i < s__divide.i_max_seg_no && TRUE == Check_if_GT (df_line_rem, 0)) ;
	if (TRUE == Check_if_LT (df_line_rem, 0))
	{
	  df_seg_len = - df_line_rem ;
	}
	break ;
      default :
	break ;
    }
    if (i >= s__divide.i_max_seg_no)
    {
      break ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  if (b_close == TRUE)
  {
    if (TRUE == Check_if_Equal (df_line_rem, 0))
    {
      if (nag->obiekt == Olinia)
      {
	b_divide = make_divide (DIVIDE_PL_1, df_x_end, df_y_end, &l_off, (void *)ptrs_line) ;
      }
	  else  if (nag->obiekt == Ospline)
	  {
		  b_divide = make_divide(DIVIDE_PL_1, df_x_end, df_y_end, &l_off, (void *)ptrs_spline);
	  }
      else
      {
	make_divide (((b_first_end == TRUE) ? DIVIDE_PL_1 : DIVIDE_PL_2),
	       df_x_end, df_y_end, &l_off, (void *)ptrs_arc) ;
      }
      l_size_add_ob += l_off ;
    }
  }
aaa:
  if (l_size_add_ob > 0)
  {
    Push_Stos ((double)l_size_add_ob) ;
  }
  return b_divide ;
}


static BOOL divide_object (double df_x, double df_y)
/*------------------------------------------------*/
{
  unsigned type_object ;
  void *ptr_object ;
  BLOK *ptrs_block ;
  BOOL b_ret ;

  b_ret = FALSE ;
  df_x = df_x ;
  df_y = df_y ;
  type_object = Blinia | Bluk | Bspline | Bokrag | Bellipse | Bellipticalarc;
  if (NULL != (ptr_object = select_w (&type_object, NULL)))
  {
    if (type_object != Bokrag &&
	s__divide.b_pline == TRUE &&
	((NAGLOWEK*)ptr_object)->blok == ElemBlok &&
	NULL != (ptrs_block = Parent_Block ((char *)ptr_object)) &&
	((ptrs_block->kod_obiektu == B_PLINE) || (ptrs_block->kod_obiektu == 'E')) &&
	(ptrs_block->obiektt1 == OB1NOCHANGE ||
	 ptrs_block->obiektt1 == OB1CHANGE_SCALE ||
	 ptrs_block->obiektt1 == OB1CHANGE_DRAG ||
	 ptrs_block->obiektt1 == OB1CHANGE_BREAK))
    {
       b_ret = divide_pline (ptrs_block, ptr_object, df_x, df_y, 0) ;
    }
    else
    {
      switch (type_object)
      {
	case Blinia :
	    b_ret = divide_line (ptr_object, df_x, df_y) ;
	    break ;
	case Bluk :
	    b_ret = divide_arc (ptr_object, df_x, df_y) ;
	    break ;
	case Bspline:
		b_ret = divide_spline(ptr_object, df_x, df_y);
		break;
    case Bellipse:
          b_ret = divide_ellipse(ptr_object, df_x, df_y);
        break;
    case Bellipticalarc:
          b_ret = divide_ellipticalarc(ptr_object, df_x, df_y);
        break;
	case Bokrag :
	    b_ret = divide_circle (ptr_object, df_x, df_y) ;
	    break ;
	default :
	    break ;
      }
    }
    CUR_OFF (X, Y) ;
    CUR_ON (X, Y) ;
  }
  return b_ret ;
}

void set_b_divide(int opc)
{
  if (opc==1)
   {
     s__divide.b_divide=TRUE;
   }
    else
     {
      s__divide.b_divide=FALSE;
     }
}

void set_df_seg_len(double d)
{
  s__divide.df_seg_len = jednostkiOb (d) ;
  s__divide.b_pline = TRUE ;
  df__ipx=0;
  df__ipy=0;
}


static void divide_command_proc (int ev_number)
/*---------------------------------------------*/
{
  switch (ev_number)
  {
    case IDM_DIVIDE :
	s__divide.b_divide = TRUE ;
	set_divide_mode (ev_number) ;
	break ;
    case IDM_SEGMENT_NO :
	set_divide_mode(IDM_DIVIDE);
	divide_seg_no () ;
	set_divide_mode(IDM_DIVIDE);
	break ;
    case IDM_MEASURE :
	s__divide.b_divide = FALSE ;
	set_divide_mode (ev_number) ;
	break ;
    case IDM_SEGMENT_LEN :
	set_divide_mode(IDM_MEASURE);
	measure_seg_len () ;
	set_divide_mode(IDM_MEASURE);
	break ;
    case IDM_MAX :
	set_max_seg_no () ;
	break ;
    case IDM_PLINE :
	s__divide.b_pline = TRUE ;
	menu_par_new ((*mDivide.pola)[5].txt, _POLYLINE_) ;
	break ;
    case IDM_SEG_PLINE :
	s__divide.b_pline = FALSE ;
	menu_par_new ((*mDivide.pola)[5].txt, _SEGMENT_) ;
	break ;
    case IDM_ALIGN_BLOCK :
	b__align_blok = TRUE ;
	menu_par_new ((*mDivide.pola)[7].txt, _YES__) ;
	break ;
    case IDM_NO_ALIGN_BLOCK :
	menu_par_new ((*mDivide.pola)[7].txt, _NO__) ;
	b__align_blok = FALSE ;
	break ;
    case IDM_UNDO :
	undo_divide_proc () ;
	break ;
    default :
	break ;
  }
}


static int edit_divide_seg_no (BOOL b_graph_value)
/*--------------------------------*/
{
  b_graph_value = b_graph_value ;
  if (e__divide.val_no < 1 ||
       1 > e__divide.values [0] ||
       MAX_DIVIDE_SEG_NO < e__divide.values [0] )
  {
    ErrList (9) ;
    return 0 ;
  }
  s__divide.i_seg_no = (int)e__divide.values [0] ;
  e__divide.st [6] = '\0' ;
  menu_par_new ((*mDivide.pola)[1].txt, e__divide.st) ;
  return 1 ;
}


static void divide_seg_no (void)
/*-----------------------------*/
{
  char sk [MaxTextLen] = "", *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;

  if (EOF == sprintf (sk, "%-5d", s__divide.i_seg_no ))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (!get_string (sk, "", MaxTextLen, 0, 27)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if (d <= 0)
  {
    ErrList (24) ;
    return ;
  }
  if (d > 10000)
  {
    d = 10000 ;
  }
  s__divide.i_seg_no = (int)d ;
  sprintf (e__divide.st, "%#6d", (int)d) ;
  if (s__divide.b_divide == TRUE)
  {
    Out_Edited_Draw_Param ((ESTR *)&e__divide, TRUE) ;
  }
  menu_par_new ((*mDivide.pola)[1].txt, e__divide.st) ;
}



static int edit_measure_seg_len (BOOL b_graph_value)
/*-----------------------------------*/
{
  b_graph_value = b_graph_value ;
  if (e__measure.val_no < 1 ||
      fabs (e__measure.values [0]) > 100000l)
  {
    ErrList (3) ;
    return 0 ;
  }
  s__divide.df_seg_len = jednostkiOb (e__measure.values [0]) ;
  e__measure.st [11] = '\0' ;
  menu_par_new ((*mDivide.pola)[3].txt, e__measure.st) ;
  return 1 ;
}

static void measure_seg_len (void)
/*-------------------------------*/
{
  char sk [MaxTextLen] = "", *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;

  if (EOF == sprintf(sk, "%-6.2f", milimetryob (s__divide.df_seg_len)))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (!get_string (sk, "", MaxTextLen, 0, 29)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if (d <= 0)
  {
    ErrList (30) ;
    return ;
  }
  if (d > 100000.0)
  {
    d = 100000.0 ;
  }
  s__divide.df_seg_len = jednostkiOb (d) ;
  sprintf (e__measure.st, "%#7.2lf", d) ; 
  if (s__divide.b_divide == FALSE)
  {
    Out_Edited_Draw_Param ((ESTR *)&e__measure, TRUE) ;
  }
  menu_par_new ((*mDivide.pola)[3].txt, e__measure.st) ;
}


static void redcr_divide (char typ)
/*---------------------------------*/
{
  static void  (*CUR_oN)(double ,double) ;
  static void  (*CUR_oFF)(double ,double) ;
  static int ( *SW[3])(), akt, sel_cur, sel_gor, sel_nr, kom0 ;
  static TMENU *menu ;
  static char st [POLE_TXT_MAX], cod=' ';
  static int iconno=0;

  if (typ == 0)
  {
    mDivide.max = 8 ;
    if (b__divide_point == TRUE)
    {
      mDivide.max = 7 ;
      if (mDivide.poz + mDivide.foff == 7)
      {
        mDivide.poz = 6 ;
      }
    }
    Clear_Stos () ;
    kom0 = Komunikat_R0 ;
    komunikat (0) ;
    getmenupini (&menu, st, &cod,&iconno) ;
    menupini (&mDivide, _DIVIDE_, _DIVIDE_C_, 50) ;
    np__divide = np__measure = -1 ;
    e__divide.x = maxX - PL266 ;
    e__divide.y = ESTR_Y;
    e__divide.lmax = 8 ;
    e__divide.ESTRF = edit_divide_seg_no ;
	e__divide.extend = 0;
    e__divide.val_no_max = 1 ;
    e__divide.mode = GV_NUMBER ;
    e__divide.format = "\001%#6d" ;
    e__measure.x = maxX - PL266 ;
    e__measure.y= ESTR_Y;
    e__measure.lmax = 16 ;
    e__measure.ESTRF = edit_measure_seg_len ;
	e__measure.extend = 0;
    e__measure.val_no_max = 1 ;
    e__measure.mode = GV_DIST ;
    e__measure.format = "%#7.2lf" ;
    set_divide_mode ((s__divide.b_divide == TRUE) ? IDM_DIVIDE : IDM_MEASURE) ;
    akt = sel.akt ; sel.akt = ASel ;
    sel_cur = sel.cur ; sel.cur = 1 ;
	sel_gor = sel.gor; sel.gor = 0;
	sel_nr = sel.nr;

	layer_info();

    CUR_OFF (X, Y) ;
    CUR_oFF = CUR_OFF ;  CUR_OFF = out_sel_off ;
    CUR_oN = CUR_ON ;   CUR_ON = out_sel_on ;
    SW[0] = SERV [73] ;  SERV [73] = sel_t ;
    SW [1] = SERV [81] ;  SERV [81] = sel_d ;
    CUR_ON (X, Y) ;
	
 }
 else
 {
    CUR_OFF (X, Y) ;
    CUR_OFF = CUR_oFF ;
    CUR_ON = CUR_oN ;
    SERV [73] = SW [0] ;
    SERV [81] = SW [1] ;
    sel.akt = akt ;
    sel.cur = sel_cur ;
	sel.gor = sel_gor;
	sel.nr = sel_nr;

	layer_info();

	CUR_ON(X, Y);
    komunikat0 (kom0) ;
    menupini (menu, st, cod, iconno) ;
    usunstr (np__divide) ;
    usunstr (np__measure) ;
  }
}

void divide_proc (void)
/*-------------------*/
{
  EVENT *ev;
  double df_x, df_y ;
  BOOL TTF_redraw_back;

  TTF_redraw = FALSE;
  redcr_divide (0) ;
  while (1)
  {
    ev = Get_Event_Point (NULL, &df_x, &df_y) ;
    if ( ev->What == evKeyDown  && ev->Number== 0)
    {
      redcr_divide (1) ;
      return ;
    }
    else
    if (ev->What == evKeyDown && ev->Number== ENTER)
    {
      divide_object (df_x, df_y) ;
	  if (TTF_redraw)
	  {
		  redraw();
		  TTF_redraw = TRUE;
	  }
    }
    else
    if( ev->What == evCommandP)
    {
	  TTF_redraw_back = TTF_redraw;
      divide_command_proc (ev->Number) ;
	  TTF_redraw = TTF_redraw_back;
    }
  }
}

/*------------------------------------------------------------------------*/

static double get_angle_tangent_to_object (void *ptr_ob,
				double df_x, double df_y)
/*-------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  double df_ret_angle;
  double df_atemp, df_xtemp, df_ytemp ;
  LINIA *ptrs_line ;
  LUK *ptrs_arc ;
  SPLINE *ptrs_spline;
  OKRAG *ptrs_circle ;
  ELLIPSE *ptrs_ellipse;
  ELLIPTICALARC *ptrs_ellipticalarc;
  int npts, i, j, k;
  float out_x[MaxNpts], out_y[MaxNpts];
  double dx, dy, dx_1, dy_1, dx_p, dy_p, dx_l, dy_l;
  double df_line_len, df_line_len_1;

  SPLINE tmp_spline = Splinedef;
  double bnodes_2_tangent_org, bnodes_2_tangent;
  int node_count;
  int points_count = 0;

  int NODES_NO;
  double xy[256];
  int numpoints;

  float* all_out_x;
  float* all_out_y;

  nag = (NAGLOWEK*)ptr_ob ;
  switch (nag->obiekt)
  {
    case Olinia :
	ptrs_line = (LINIA*)nag ;
	df_ret_angle = Atan2 (ptrs_line->y2 - ptrs_line->y1,
			       ptrs_line->x2 - ptrs_line->x1) ;
	if (df_ret_angle > Pi2 / 4 && df_ret_angle < Pi2 / 2)
	{
	  df_ret_angle += Pi2 / 2 ;
	}
	else
	if (df_ret_angle > -Pi2 / 2 && df_ret_angle < -Pi2 / 4)
	{
	  df_ret_angle += Pi2 / 2 ;
	}
	break ;
	case Ospline:
		df_ret_angle = 0.0;
		ptrs_spline = (SPLINE*)nag;
		//calculation of angle in point df_x, df_y
#define NPA 90
		npts = NPA;

		if (ptrs_spline->npts < 5)
		{
			all_out_x = malloc(90 * sizeof(float));
			if (all_out_x == NULL) break;
			all_out_y = malloc(90 * sizeof(float));
			if (all_out_y == NULL)
			{
				free(all_out_x);
				break;
			}
			calc_bspline(ptrs_spline->lp, ptrs_spline->npts, ptrs_spline->xy, npts, out_x, out_y);

			memmove(&all_out_x[0], &out_x, sizeof(float) * npts);
			memmove(&all_out_y[0], &out_y, sizeof(float) * npts);
			points_count = npts;
		}
		else
		{
			NODES_NO = ptrs_spline->lp / 2 + 4;

			all_out_x = malloc(90 * NODES_NO * sizeof(float));
			if (all_out_x == NULL) break;
			all_out_y = malloc(90 * NODES_NO * sizeof(float));
			if (all_out_y == NULL)
			{
				free(all_out_x);
				break;
			}

			curviness = ptrs_spline->xy[ptrs_spline->lp];

			tmp_spline.npts = 4;
			tmp_spline.lp = 8;
			tmp_spline.kolor = ptrs_spline->kolor;
			tmp_spline.typ = ptrs_spline->typ;

			node_count = 1;

			set_bnodes(ptrs_spline, ptrs_spline->xy, &node_count);

			calc_tangents(ptrs_spline->closed, &node_count, &bnodes_2_tangent);

			df_line_len = 0.0;

			if (ptrs_spline->closed == 0)
			{
				bnodes_2_tangent_org = bnodes[2].tangent;
				bnodes[2].tangent = bnodes_2_tangent;
				get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

				memmove(&all_out_x[points_count], &out_x, sizeof(float) * npts);
				memmove(&all_out_y[points_count], &out_y, sizeof(float) * npts);
				points_count += npts;

				bnodes[2].tangent = bnodes_2_tangent_org;
			}

			for (k = 2; k < node_count - 2; k++)  //1 - extra_node
			{
				get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

				memmove(&all_out_x[points_count], &out_x, sizeof(float) * npts);
				memmove(&all_out_y[points_count], &out_y, sizeof(float) * npts);
				points_count += npts;

			}

			if ((node_count > 3) && (ptrs_spline->closed == 0))
			{
				get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

				memmove(&all_out_x[points_count], &out_x, sizeof(float) * npts);
				memmove(&all_out_y[points_count], &out_y, sizeof(float) * npts);
				points_count += npts;
			}
		}

		for (i = 0; i < (points_count - 1); i++)
		{

			dx_l = df_x - all_out_x[i];
			dy_l = df_y - all_out_y[i];

			dx_p = all_out_x[i + 1] - df_x;
			dy_p = all_out_y[i + 1] - df_y;

			dx = all_out_x[i + 1] - all_out_x[i];
			dy = all_out_y[i + 1] - all_out_y[i];
			df_line_len = (sqrt(dx*dx + dy * dy));

			df_line_len_1 = (sqrt(dx_l*dx_l + dy_l * dy_l)) + (sqrt(dx_p*dx_p + dy_p * dy_p));
			if (fabs(df_line_len_1 - df_line_len) < 0.00001)
			{
				df_ret_angle = Atan2(all_out_y[i + 1] - all_out_y[i],
					all_out_x[i + 1] - all_out_x[i]);

				break;
			}
		}
		free(all_out_x);
		free(all_out_y);
		break;
      case Oellipse:
          df_ret_angle = 0.0;
          ptrs_ellipse = (ELLIPSE*)nag;
          //calculation of angle in point df_x, df_y
          df_line_len = 0.0;

          numpoints= Get_Ellipse_Points(ptrs_ellipse->x, ptrs_ellipse->y, ptrs_ellipse->rx, ptrs_ellipse->ry, ptrs_ellipse->angle, xy) ;
          xy[numpoints] = xy[0];
          xy[numpoints+1] = xy[1];
          numpoints+=2;
          for (i = 0; i < (numpoints - 2); i+=2)
          {
              dx = xy[i + 2] - xy[i];
              dy = xy[i + 3] - xy[i+1];
              df_line_len += (sqrt(dx * dx + dy * dy));
          }

          for (i = 0; i < (numpoints - 2); i+=2)
          {
              dx_l = df_x - xy[i];
              dy_l = df_y - xy[i + 1];

              dx_p = xy[i + 2] - df_x;
              dy_p = xy[i + 3] - df_y;

              dx = xy[i + 2] - xy[i];
              dy = xy[i + 3] - xy[i + 1];
              df_line_len = (sqrt(dx*dx + dy * dy));

              df_line_len_1 = (sqrt(dx_l*dx_l + dy_l * dy_l)) + (sqrt(dx_p*dx_p + dy_p * dy_p));
              if (fabs(df_line_len_1 - df_line_len) < 0.00001)
              {
                  df_ret_angle = Angle_Normal(Atan2(xy[i + 3] - xy[ i + 1], xy[i + 2] - xy[i]));
                  break;
              }
          }
      break;
      case Oellipticalarc:
          df_ret_angle = 0.0;
          ptrs_ellipticalarc = (ELLIPTICALARC*)nag;
          //calculation of angle in point df_x, df_y
          df_line_len = 0.0;

          numpoints= Get_EllipticalArc_Points(ptrs_ellipticalarc->x, ptrs_ellipticalarc->y, ptrs_ellipticalarc->rx, ptrs_ellipticalarc->ry, ptrs_ellipticalarc->angle, ptrs_ellipticalarc->kat1, ptrs_ellipticalarc->kat2, xy) ;
          for (i = 0; i < (numpoints - 2); i+=2)
          {
              dx = xy[i + 2] - xy[i];
              dy = xy[i + 3] - xy[i+1];
              df_line_len += (sqrt(dx * dx + dy * dy));
          }

          for (i = 0; i < (numpoints - 2); i+=2)
          {
              dx_l = df_x - xy[i];
              dy_l = df_y - xy[i + 1];

              dx_p = xy[i + 2] - df_x;
              dy_p = xy[i + 3] - df_y;

              dx = xy[i + 2] - xy[i];
              dy = xy[i + 3] - xy[i + 1];
              df_line_len = (sqrt(dx*dx + dy * dy));

              df_line_len_1 = (sqrt(dx_l*dx_l + dy_l * dy_l)) + (sqrt(dx_p*dx_p + dy_p * dy_p));
              if (fabs(df_line_len_1 - df_line_len) < 0.00001)
              {
                  df_ret_angle = Atan2(xy[i + 3] - xy[ i + 1], xy[i + 2] - xy[i]);
                  break;
              }
          }
      break;
    case Oluk :
	ptrs_arc = (LUK*)nag ;
	df_atemp = Atan2 (df_y - ptrs_arc->y, df_x - ptrs_arc->x) ;
	obrd (sin (df_atemp), cos (df_atemp), ptrs_arc->r, 1, &df_xtemp, &df_ytemp) ;
	df_ret_angle = Atan2 (df_ytemp + ptrs_arc->y - df_y ,  df_xtemp + ptrs_arc->x - df_x) ;
	break ;
    case Ookrag :
	ptrs_circle = (OKRAG*)nag ;
	df_atemp = Atan2 (df_y - ptrs_circle->y, df_x - ptrs_circle->x) ;
	obrd (sin (df_atemp), cos (df_atemp), ptrs_circle->r, 1, &df_xtemp, &df_ytemp) ;
	df_ret_angle = Atan2 (df_ytemp + ptrs_circle->y - df_y ,  df_xtemp + ptrs_circle->x - df_x) ;
	break ;
  }
  return df_ret_angle ;
}

double get_angle_tangent_to_object_ (void *ptr_ob, double df_x, double df_y)
{
    return get_angle_tangent_to_object(ptr_ob, df_x, df_y);
}


static double get_angle_tangent_to_pline (void *ptr_ob,
				double df_x, double df_y, int mode)
/*----------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  double df_ret_angle ;
  double df_atemp, df_xtemp, df_ytemp ;
  LINIA *ptrs_line ;
  SPLINE *ptrs_spline;
  LUK *ptrs_arc ;
  int npts, i, j, k;
  float out_x[MaxNpts], out_y[MaxNpts];
  double dx, dy, dx_1, dy_1, dx_p, dy_p, dx_l, dy_l;
  double df_line_len, df_line_len_1;

  SPLINE tmp_spline = Splinedef;
  double bnodes_2_tangent_org, bnodes_2_tangent;
  int node_count;
  int points_count = 0;

  int NODES_NO;

  float* all_out_x;
  float* all_out_y;

  nag = (NAGLOWEK*)ptr_ob ;
  switch (nag->obiekt)
  {
    case Olinia :
	ptrs_line = (LINIA*)nag ;
	df_ret_angle = Atan2 (ptrs_line->y2 - ptrs_line->y1,
			       ptrs_line->x2 - ptrs_line->x1) ;
	break ;
	case Ospline:
		df_ret_angle = 0.0;
		ptrs_spline = (SPLINE*)nag;
		//calculation of angle in point df_x, df_y
#define NPA 90
		npts = NPA;

		if (ptrs_spline->npts < 5)
		{
			all_out_x = malloc(90 * sizeof(float));
			if (all_out_x == NULL) break;
			all_out_y = malloc(90 * sizeof(float));
			if (all_out_y == NULL)
			{
				free(all_out_x);
				break;
			}
			calc_bspline(ptrs_spline->lp, ptrs_spline->npts, ptrs_spline->xy, npts, out_x, out_y);
			memmove(&all_out_x[0], &out_x, sizeof(float) * npts);
			memmove(&all_out_y[0], &out_y, sizeof(float) * npts);
			points_count = npts;
		}
		else
		{
			NODES_NO = ptrs_spline->lp / 2 + 4;

			all_out_x = malloc(90 * NODES_NO * sizeof(float));
			if (all_out_x == NULL) break;
			all_out_y = malloc(90 * NODES_NO * sizeof(float));
			if (all_out_y == NULL)
			{
				free(all_out_x);
				break;
			}

			curviness = ptrs_spline->xy[ptrs_spline->lp];

			tmp_spline.npts = 4;
			tmp_spline.lp = 8;
			tmp_spline.kolor = ptrs_spline->kolor;
			tmp_spline.typ = ptrs_spline->typ;

			node_count = 1;

			set_bnodes(ptrs_spline, ptrs_spline->xy, &node_count);

			calc_tangents(ptrs_spline->closed, &node_count, &bnodes_2_tangent);

			df_line_len = 0.0;

			if (ptrs_spline->closed == 0)
			{
				bnodes_2_tangent_org = bnodes[2].tangent;
				bnodes[2].tangent = bnodes_2_tangent;
				get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

				memmove(&all_out_x[points_count], &out_x, sizeof(float) * npts);
				memmove(&all_out_y[points_count], &out_y, sizeof(float) * npts);
				points_count += npts;

				bnodes[2].tangent = bnodes_2_tangent_org;
			}

			for (k = 2; k < node_count - 2; k++)  //1 - extra_node
			{
				get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

				memmove(&all_out_x[points_count], &out_x, sizeof(float) * npts);
				memmove(&all_out_y[points_count], &out_y, sizeof(float) * npts);
				points_count += npts;

			}

			if ((node_count > 3) && (ptrs_spline->closed == 0))
			{
				get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

				memmove(&all_out_x[points_count], &out_x, sizeof(float) * npts);
				memmove(&all_out_y[points_count], &out_y, sizeof(float) * npts);
				points_count += npts;
			}
		}

		for (i = 0; i < (points_count - 1); i++)
		{
			
			dx_l = df_x - all_out_x[i];
			dy_l = df_y - all_out_y[i];

			dx_p = all_out_x[i + 1] - df_x;
			dy_p = all_out_y[i + 1] - df_y;

			dx = all_out_x[i + 1] - all_out_x[i];
			dy = all_out_y[i + 1] - all_out_y[i];
			df_line_len = (sqrt(dx*dx + dy * dy));

			df_line_len_1 = (sqrt(dx_l*dx_l + dy_l * dy_l)) + (sqrt(dx_p*dx_p + dy_p * dy_p));
			if (fabs(df_line_len_1 - df_line_len) < 0.00001)
			{
				df_ret_angle = Atan2(all_out_y[i + 1] - all_out_y[i],
					all_out_x[i + 1] - all_out_x[i]);

				break;
			}
		}
		free(all_out_x);
		free(all_out_y);
		break;
    case Oluk :
	ptrs_arc = (LUK*)nag ;
	df_atemp = Atan2 (df_y - ptrs_arc->y, df_x - ptrs_arc->x) ;
	obrd (sin (df_atemp), cos (df_atemp), ptrs_arc->r,
	((mode == DIVIDE_PL_1) ? 1 : -1), &df_xtemp, &df_ytemp) ;
	df_ret_angle = Atan2 (df_ytemp + ptrs_arc->y - df_y ,  df_xtemp + ptrs_arc->x - df_x) ;
	break ;
  }
  return df_ret_angle ;
}



static BOOL add_divide_block (double df_x, double df_y, long *ptrl_block_size,
			BOOL b_align_blok, int mode, void *ptr_ob)
/*-------------------------------------------------------------------*/
{
  BOOL b_ret ;
  double df_angle ;

  if (b_align_blok == TRUE)
  {
    if (mode == DIVIDE_SINGLE)
    {
      df_angle = get_angle_tangent_to_object (ptr_ob, df_x, df_y) ;
    }
    else
    {
      df_angle = get_angle_tangent_to_pline (ptr_ob, df_x, df_y, mode) ;
    }

    b_ret = Add_Block (ADP, ADK, ptrl_block_size,
    df__ipx, df__ipy, sin (df_angle), cos (df_angle),
    df_x - df__ipx, df_y - df__ipy, 0, 0, Tobrot | Tprzesuw) ;
  }
  else
  {
    b_ret = Add_Block (ADP, ADK, ptrl_block_size, df_x - df__ipx, df_y - df__ipy, 0, 0, 0, 0, 0, 0, Tprzesuw) ;
  }
  return b_ret ;
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/



void Divide_Point (void)
/*---------------------*/
{
  b__divide_point = TRUE ;
  divide_proc () ;
}

void Divide_Block (double df_ipx, double df_ipy)
/*---------------------------------------------*/
{
  b__divide_point = FALSE ;
  df__ipx = df_ipx ;
  df__ipy = df_ipy ;
  divide_proc () ;
}
/*-----------------------------------------------------------------------*/

#undef __O_DIVIDE__
