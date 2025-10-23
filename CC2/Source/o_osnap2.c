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
#include <string.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_osnap.h"
#include "o_object.h"

extern void Rotate_Line (void *, double, double, double, double) ;
extern int przeciecieLE_tt_prec_(double *x1, double *y1, double *x2, double *y2, void *adr, void *adr1);
extern int przeciecieLEA_tt_prec_(double *x1, double *y1, double *x2, double *y2, void *adr, void *adr1);

#define BIGNO 1e200
int przeciecieLL_tt (double *x, double *y, void *adr, void *adr1, double *t1, double *t2)
/*-----------------------------------------------------------*/
{
  LINIA  *adrl,*adrl1;
  double t,p,kx;

  DF_PRECISION=1.0E+7; //1.0E+5
  *t1 = BIGNO ;
  *t2 = BIGNO ;
  adrl=(LINIA*)adr ;
  adrl1=(LINIA*)adr1 ;
  if (TRUE == Check_if_Equal (adrl->x1, adrl->x2))
  {
    if (TRUE == Check_if_Equal (adrl->y2 - adrl->y1, 0))
    {
      DF_PRECISION=1.0E+5;
      return 0 ;	/*odcinek 'zerowy'*/
    }
    if (TRUE == Check_if_Equal (adrl1->x1, adrl1->x2))
    {
      if (TRUE != Check_if_Equal (adrl->x1, adrl1->x1))
      {
        DF_PRECISION=1.0E+5;
	return 0 ;
      }
      if (TRUE == Check_if_Equal (adrl->y1, adrl1->y1))
      { t = 0 ; p = 0 ; }
      else
      if (TRUE == Check_if_Equal (adrl->y1, adrl1->y2))
      { t = 0 ; p = 1 ; }
      else
      if (TRUE == Check_if_Equal (adrl->y2, adrl1->y1))
      { t = 1 ; p = 0 ; }
      else
      if (TRUE == Check_if_Equal (adrl->y2, adrl1->y2))
      { t = 1 ; p = 1 ; }
      else
       {
        DF_PRECISION=1.0E+5;
	return 0 ;
       }	
      *x = adrl->x1 ;
      *y = adrl->y1 + t * (adrl->y2 - adrl->y1) ;
      *t1 = t ; *t2 = p ; /*t1 dotyczy 1 linii, t2 drugiej*/
      
      DF_PRECISION=1.0E+5;
      return 1 ;
    }
    p=(adrl->x1-adrl1->x1)/(adrl1->x2-adrl1->x1) ;
    t=(adrl1->y1-adrl->y1+p*(adrl1->y2-adrl1->y1))/(adrl->y2-adrl->y1) ;
  }
  else
  if (TRUE == Check_if_Equal (adrl1->x1, adrl1->x2))
  {
    if (TRUE == Check_if_Equal (adrl1->y2, adrl1->y1))
    {
      DF_PRECISION=1.0E+5;
      return 0 ;	/*odcinek 'zerowy'*/
    }
    t=(adrl1->x1-adrl->x1)/(adrl->x2-adrl->x1);
    p=(adrl->y1-adrl1->y1+t*(adrl->y2-adrl->y1))/(adrl1->y2-adrl1->y1);
  }
  else
  {
    kx=(adrl1->x2-adrl1->x1)*(adrl->y2-adrl->y1)-(adrl1->y2-adrl1->y1)*(adrl->x2-adrl->x1);
    if (TRUE == Check_if_Equal (kx, 0))
    {
      if (TRUE == Check_if_Equal (adrl->x1, adrl1->x1) &&
	  TRUE == Check_if_Equal (adrl->y1, adrl1->y1))
      { t = 0 ; p = 0 ; }
      else
      if (TRUE == Check_if_Equal (adrl->x1, adrl1->x2) &&
	  TRUE == Check_if_Equal (adrl->y1, adrl1->y2))
      { t = 0 ; p = 1 ; }
      else
      if (TRUE == Check_if_Equal (adrl->x2, adrl1->x1) &&
	  TRUE == Check_if_Equal (adrl->y2, adrl1->y1))
      { t = 1 ; p = 0 ; }
      else
      if (TRUE == Check_if_Equal (adrl->x2, adrl1->x2) &&
	  TRUE == Check_if_Equal (adrl->y2, adrl1->y2))
      { t = 1 ; p = 1 ; }
      else
       {
        DF_PRECISION=1.0E+5;
	return 0 ;
       }	
      *x = adrl->x1 + t * (adrl->x2 - adrl->x1) ;
      *y = adrl->y1 + t * (adrl->y2 - adrl->y1) ;
      *t1 = t ; *t2 = p ; /*t1 dotyczy 1 linii, t2 drugiej*/
      DF_PRECISION=1.0E+5;
      return 1 ;
    }
    p=((adrl1->y1-adrl->y1)*(adrl->x2-adrl->x1)-(adrl1->x1-adrl->x1)*(adrl->y2-adrl->y1))/kx;
    t=(adrl1->x1-adrl->x1+p*(adrl1->x2-adrl1->x1))/(adrl->x2-adrl->x1);
  }
  *x=adrl->x1+t*(adrl->x2-adrl->x1);
  *y=adrl->y1+t*(adrl->y2-adrl->y1);
  *t1 = t; *t2 = p ; /*t1 dotyczy 1 linii, t2 drugiej*/
  DF_PRECISION=1.0E+5;
  return 1;
}


int przeciecieLO_tt (double *x1, double *y1, double *t1,
		    double *x2, double *y2, double *t2,
		    void *adr, void *adr1)
/*----------------------------------------------------*/
{
  OKRAG *O ;
  LINIA *L, s_line ;
  double t_temp, df_dx, df_dy, df_si, df_co, df_angle ;
  int i_ret ;

  DF_PRECISION=1.0E+7;
  L=(LINIA*)adr ;
  O=(OKRAG*)adr1 ;
  if (TRUE == Check_if_Equal (L->x2 - L->x1, 0) &&
      TRUE == Check_if_Equal (L->y2 - L->y1, 0))
  {
    DF_PRECISION=1.0E+5;
    return 0 ;		/*odcinek 'zerowy'*/
  }
  df_angle = Atan2 (L->y2 - L->y1,  L->x2 - L->x1) ;
  memcpy (&s_line, L, sizeof (LINIA)) ;
  df_si = sin (-df_angle) ;
  df_co = cos (-df_angle) ;
  Rotate_Line ((void *)&s_line, O->x, O->y, df_si, df_co) ;
  df_dy = fabs (O->y - s_line.y1) ;
  if (TRUE == Check_if_GT (df_dy, O->r))
  {
    if (df_dy - O->r > Get_Prec (MPREC_3 /*MPREC_4*/, df_dy, O->r)) //!!!!!!!!!!! zmieniono dokladnosc
    {
      DF_PRECISION=1.0E+5;
      return 0 ;
    }
  }
  df_dx =  O->r * O->r - df_dy * df_dy ;
  if (df_dx < 0)
  {
    df_dx = 0 ;
  }
  df_dx = sqrt (df_dx) ;
  *x1 = O->x - df_dx ;
  *x2 = O->x + df_dx ;
  *y1 = *y2 = s_line.y1 ;
  i_ret = 2 ;
  if (fabs (*x1 - *x2) < Get_Prec (MPREC_3, *x1, *x2))
  {
    *x1 = O->x ;
    *x2 = O->x ;
    i_ret = 1 ;
  }
  df_si = sin (df_angle) ;
  df_co = cos (df_angle) ;
  Rotate_Point (df_si, df_co, O->x, O->y, *x1, *y1, x1, y1) ;
  Rotate_Point (df_si, df_co, O->x, O->y, *x2, *y2, x2, y2) ;
  if (fabs (L->x2 - L->x1) < fabs (L->y2 - L->y1))
  {
    *t1 = (*y1 - L->y1) / (L->y2 - L->y1) ;
    *t2 = (*y2 - L->y1) / (L->y2 - L->y1) ;
  }
  else
  {
    *t1 = (*x1 - L->x1) / (L->x2 - L->x1) ;
    *t2 = (*x2 - L->x1) / (L->x2 - L->x1) ;
  }
  if (*t1 > *t2)
  {
    t_temp = *t2 ;
    *t2 = *t1 ;
    *t1 = t_temp ;
  }		/* zawsze   t2 >= t1*/
  DF_PRECISION=1.0E+5;
  return i_ret ;
}

int przeciecieLE_tt (double *x1, double *y1, double *t1,
                     double *x2, double *y2, double *t2,
                     void *adr, void *adr1)
/*----------------------------------------------------*/
{
    ELLIPSE *E ;
    LINIA *L, s_line ;
    double t_temp, df_dx, df_dy, df_si, df_co, df_angle ;
    int i_ret ;

    DF_PRECISION=1.0E+7;
    L=(LINIA*)adr ;
    E=(ELLIPSE*)adr1 ;
    if (TRUE == Check_if_Equal (L->x2 - L->x1, 0) &&
        TRUE == Check_if_Equal (L->y2 - L->y1, 0))
    {
        DF_PRECISION=1.0E+5;
        return 0 ;		/*odcinek 'zerowy'*/
    }

    i_ret=przeciecieLE_tt_prec_(x1, y1, x2, y2, adr, adr1);
    if (i_ret>0) {

        if(i_ret==1)
        {
            if (fabs(L->x2 - L->x1) < fabs(L->y2 - L->y1)) {
                *t1 = (*y1 - L->y1) / (L->y2 - L->y1);
            } else {
                *t1 = (*x1 - L->x1) / (L->x2 - L->x1);
            }
            *t2=0;
        }
        else
        {

            if (fabs(L->x2 - L->x1) < fabs(L->y2 - L->y1)) {
                *t1 = (*y1 - L->y1) / (L->y2 - L->y1);
                *t2 = (*y2 - L->y1) / (L->y2 - L->y1);
            } else {
                *t1 = (*x1 - L->x1) / (L->x2 - L->x1);
                *t2 = (*x2 - L->x1) / (L->x2 - L->x1);
            }
            if (*t1 > *t2) {
                t_temp = *t2;
                *t2 = *t1;
                *t1 = t_temp;
            }        /* zawsze   t2 >= t1*/
        }
    }
    DF_PRECISION=1.0E+5;
    return i_ret ;
}

int przeciecieLEA_tt (double *x1, double *y1, double *t1,
                     double *x2, double *y2, double *t2,
                     void *adr, void *adr1)
/*----------------------------------------------------*/
{
    ELLIPSE *E ;
    LINIA *L, s_line ;
    double t_temp, df_dx, df_dy, df_si, df_co, df_angle ;
    int i_ret ;

    DF_PRECISION=1.0E+7;
    L=(LINIA*)adr ;
    E=(ELLIPSE*)adr1 ;
    if (TRUE == Check_if_Equal (L->x2 - L->x1, 0) &&
        TRUE == Check_if_Equal (L->y2 - L->y1, 0))
    {
        DF_PRECISION=1.0E+5;
        return 0 ;		/*odcinek 'zerowy'*/
    }

    i_ret=przeciecieLEA_tt_prec_(x1, y1, x2, y2, adr, adr1);
    if (i_ret>0) {
        if(i_ret==1)
        {
            if (fabs(L->x2 - L->x1) < fabs(L->y2 - L->y1)) {
                *t1 = (*y1 - L->y1) / (L->y2 - L->y1);
            } else {
                *t1 = (*x1 - L->x1) / (L->x2 - L->x1);
            }
            *t2=0;
        }
        else {

            if (fabs(L->x2 - L->x1) < fabs(L->y2 - L->y1)) {
                *t1 = (*y1 - L->y1) / (L->y2 - L->y1);
                *t2 = (*y2 - L->y1) / (L->y2 - L->y1);
            } else {
                *t1 = (*x1 - L->x1) / (L->x2 - L->x1);
                *t2 = (*x2 - L->x1) / (L->x2 - L->x1);
            }
            if (*t1 > *t2) {
                t_temp = *t2;
                *t2 = *t1;
                *t1 = t_temp;
            }        /* zawsze   t2 >= t1*/
        }
    }
    DF_PRECISION=1.0E+5;
    return i_ret ;
}


int przeciecieLl_tt (double *x1, double *y1, double *t1,
		    double *x2, double *y2, double *t2,
		    void *adr, void *adr1)
/*----------------------------------------------------*/
{
  LUK *luk;
  LINIA *linia;
  OKRAG o ;
  int l;
  int i_interno ;
  double df_x1, df_y1, df_x2, df_y2 ;
    double Lx1, Ly1, Lx2, Ly2;
    double lx1, ly1, lx2, ly2;

    linia=(LINIA*)adr;
    luk=(LUK*)adr1;

  //check if arc end lays exactly on line end
    lx1=luk->x+luk->r*cos(luk->kat1);
    ly1=luk->y+luk->r*sin(luk->kat1);
    lx2=luk->x+luk->r*cos(luk->kat2);
    ly2=luk->y+luk->r*sin(luk->kat2);

    Lx1=linia->x1;
    Ly1=linia->y1;
    Lx2=linia->x2;
    Ly2=linia->y2;

    if (((Check_if_Equal(Lx2, lx1)) && (Check_if_Equal(Ly2, ly1))) ||
        ((Check_if_Equal(Lx2, lx2)) && (Check_if_Equal(Ly2, ly2))))
    {
        *x1 = Lx2 ;
        *y1 = Ly2 ;
        *t1=1;
        *t2=0;
        return 1;
    }
    else if (((Check_if_Equal(Lx1, lx1)) && (Check_if_Equal(Ly1, ly1))) ||
            ((Check_if_Equal(Lx1, lx2)) && (Check_if_Equal(Ly1, ly2))))
    {
        *x1 = Lx1 ;
        *y1 = Ly1 ;
        *t1=1;
        *t2=0;
        return 1;
    }
    
  o.r = luk->r ;
  o.x = luk->x ;
  o.y = luk->y ;
  if (0 == (i_interno = przeciecieLO_tt (x1, y1, t1, x2, y2, t2, adr, (void*)&o)))
  {
    return 0 ;
  }
  l = 2 ;
  if (i_interno == 1)
  {
    Arc_To_Points (luk, &df_x1, &df_y1, &df_x2, &df_y2) ;
    if (fabs (df_x1 - *x1) < Get_Prec (MPREC_3, df_x1, *x1) &&
	fabs (df_y1 - *y1) < Get_Prec (MPREC_3, df_y1, *y1))
    {
      *x1 = df_x1 ;
      *y1 = df_y1 ;
    }
    if (fabs (df_x2 - *x1) < Get_Prec (MPREC_3, df_x2, *x1) &&
	fabs (df_y2 - *y1) < Get_Prec (MPREC_3, df_y2, *y1))
    {
      *x1 = df_x2 ;
      *y1 = df_y2 ;
    }
  }
  if (FALSE == Point_Arc (*x1 , *y1 , luk))
  {
    *x1 = *x2 ;
    *y1 = *y2 ;
    *t1 = *t2 ;
    l-- ;
  }
  if (FALSE == Point_Arc (*x2 , *y2 , luk))
  {
    *x2 = *x1 ;
    *y2 = *y1 ;
    *t2 = *t1 ;
    l-- ;
  }
  if (1 == i_interno && l == 2)
  {
    --l ;
  }
   return l;
}

int przeciecieOO_ex (double *x1, double *y1, double *x2, double *y2, void *adr, void *adr1)
/*-----------------------------------------------------------------------------------------*/
{
  OKRAG *O1, *O2 ;
  double l, Lx, Ly, si, co, x0, y0, x, y ;
  int ret ;

  O1 = (OKRAG*)adr ;
  O2 = (OKRAG*)adr1 ;
  Lx = O2->x - O1->x ;
  Ly = O2->y - O1->y ;
  if (TRUE == Check_if_Equal (Lx, 0) &&
     TRUE == Check_if_Equal (Ly, 0)) return 0 ;
  l = sqrt (Lx * Lx + Ly * Ly) ;
  si = Ly / l ;
  co = Lx / l ;
  obru (si, co, Lx, Ly, &x0, &y0) ;
  if (O1->r - O2->r - x0 > Get_Prec (MPREC_4, O1->r, O2->r) ||
      O2->r - O1->r - x0 > Get_Prec (MPREC_4, O1->r, O2->r) ||
      x0 - (O1->r + O2->r) > Get_Prec (MPREC_4, O1->r, O2->r))
  {
    return 0 ;
  }
  if (TRUE == Check_if_Equal (x0, 0))
  {
    return 0 ;	/* sytuacja w zasadzie nie mozliwa*/
  }
  x = (x0 * x0 + O1->r * O1->r - O2->r * O2->r) / (2 * x0) ;
  y = O1->r * O1->r -x * x ;
  if (y < 0)
  {
    y = 0 ;   /* sytuacja w zasadzie nie mozliwa, poza stycznoscia*/
  }
  y = sqrt (y) ;
  ret = 2 ;
  if (y < Get_Prec (MPREC_2, O1->r, O2->r))
  {
    ret = 1 ;
  }
  obrd (si, co, x, y, x1, y1) ;
  *x1 += O1->x ;
  *y1 += O1->y ;
  obrd (si, co, x, -y, x2, y2) ;
  *x2 += O1->x ;
  *y2 += O1->y ;
  return ret ;
}
