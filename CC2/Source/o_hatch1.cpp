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
#define __O_HATCH1__

#include "forwin.h"
#define ALLEGWIN
#include "allegext.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_osnap.h"
#include "o_object.h"
#include "b_buf.h"
#include "o_hatch.h"
#include "b_messag.h"
#include "b_libfun.h"

#include "b_messag.h"
#include "message.h"

#include "leak_detector_cpp.hpp"

extern "C" {

extern int make_solidarc_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block);
extern int make_spline_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block);
extern void delete_spline_shadows(BOOL make_Ablok);

extern int make_arc_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block);
extern void delete_arc_shadows(void);

extern int make_elliptical_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block);
extern void delete_elliptical_shadows(void);

extern unsigned long_long get_free_mem_size (int) ;
extern void Rotate_Arc (void*, double, double , double , double ) ;
extern void Rotate_Line (void *, double , double , double , double ) ;
extern void Make_Hatch (T_PTR_Hatch_Param ptrs_hatch_param,
	    T_PTR_Area ptrs_area, int comput_area,
	    double df_apx1, double df_apy1, double df_apx2, double df_apy2, double df_pointx, double df_pointy, /*void *abd */ long_long le_abd, int to_block=0, int draw=TRUE);
extern void calc_bspline(int lp, int n, float points[], int p1, float* out_x, float* out_y);
}

static int i__objects_no ; 		/*liczba obiektow wskazanych*/
static T_Node_Coord_Alloc s__nodes_coord_alloc =
    {NULL, 0, 0, sizeof(T_Node_Coord), DNODE_COORD} ; /*adres tablicy wspolrzednych wezlow*/
static T_PTR_Nodes_Object ptrs__nodes_table ;	/*tablica o rozmiarze i__objects_no,
						o elemencie zawierajacym
						adres tablicy wezlow obiektu*/
static T_Area s__area ;				/*obszar po kondensacji*/
static double df__arc_angle1, df__arc_angle2 ; 	/*pomocnicze do sortowania wezlow lukow*/

static int get_objectsno (long_long, long_long) ; /*liczba obiektow wskazanych*/
static void get_global_node_coord (int , double *, double *) ;

extern void Free_Areas_Mem(void);

#define MAX_NO_OF_OBJECTS 6000


static int fcmp_linear (const void *val1, const void *val2)
/*-------------------------------------------------------*/
{
  double df_val1, df_val2 ;

  df_val1 = *((double*)val1) ;
  df_val2 = *((double*)val2) ;
  if (df_val1 < df_val2)
    return -1 ;
  else
  if (df_val1 > df_val2)
    return 1 ;
  else
    return 0 ;
}

static int fcmp_arc_angle (const void *val1, const void *val2)
/*------------------------------------------------------------*/
{
  double df_val1, df_val2 ;

  df_val1 = *((double*)val1) ;
  df_val2 = *((double*)val2) ;
  if (TRUE == Check_if_Equal (df_val1, df_val2))
  {
    return 0 ;
  }
  if (FALSE == Check_Angle (df_val1, df__arc_angle1, df__arc_angle2))
  {
    if (Get_Angle_Difference (df_val1, df__arc_angle1) >
	Get_Angle_Difference (df_val1, df__arc_angle2))
    {
      df_val1 = df__arc_angle2 ;
    }
    else
    {
      df_val1 = df__arc_angle1 ;
    }
  }
  if (FALSE == Check_Angle (df_val2, df__arc_angle1, df__arc_angle2))
  {
    if (Get_Angle_Difference (df_val2, df__arc_angle1) >
	Get_Angle_Difference (df_val2, df__arc_angle2))
    {
      df_val2 = df__arc_angle2 ;
    }
    else
    {
      df_val2 = df__arc_angle1 ;
    }
  }
  if (TRUE == Check_Angle (df_val1, df_val2, df__arc_angle2))
  {
    return 1 ;
  }
  else
  {
   return -1 ;
  }
}


static void sort_nodes_table (long_long l_offb, long_long l_offe)
/*----------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long l_off ;
  int i_object_no ; 	/*numer obiektu na liscie zaznaczonych*/
  T_PTR_Nodes_Alloc ptrs_nodes_alloc ;
  T_PTR_Node ptrs_nodes ;


  i_object_no = 0 ;
  Object_Tok (&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON) ;
  while (l_off != -1)
  {
    nag = (NAGLOWEK*)(dane + l_off) ;
    if (nag->atrybut == Ablok)
    {
      ptrs_nodes_alloc = &ptrs__nodes_table [i_object_no].s_nodes_alloc ;
      ptrs_nodes = ptrs_nodes_alloc->ptrs_nodes_object ;
      switch (nag->obiekt)
      {
	case Olinia :
	case Otekst :
	case Owwielokat :
	case Ookrag :
	case Okolo :
	  qsort (ptrs_nodes, ptrs_nodes_alloc->i_size,
		 ptrs_nodes_alloc->i_unit_size, fcmp_linear) ;
	  break ;
	case Oluk :
	  df__arc_angle1 = ((LUK*)nag)->kat1 ;
	  df__arc_angle2 = ((LUK*)nag)->kat2 ;
	  qsort (ptrs_nodes, ptrs_nodes_alloc->i_size,
		 ptrs_nodes_alloc->i_unit_size, fcmp_arc_angle) ;
	  break ;
	default :
	  break ;
      }
      i_object_no++ ;
    }
    Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON) ;
  }
}


static BOOL ini_nodes_table_alloc (void)
/*-------------------------------------*/
{
  int i ;

  for (i = 0 ; i < i__objects_no ; i++)
  {
    if (FALSE == h_alloc ((char **)
	    &ptrs__nodes_table [i].s_nodes_alloc.ptrs_nodes_object, 0))
    {
      i__objects_no = i ; 	/*dla zwolnienia pamieci*/
      return FALSE ;
    }
    ptrs__nodes_table [i].s_nodes_alloc.i_size = 0 ;
    ptrs__nodes_table [i].s_nodes_alloc.i_size_alloc = 0 ;
    ptrs__nodes_table [i].s_nodes_alloc.i_unit_size = sizeof(T_Node) ;
    ptrs__nodes_table [i].s_nodes_alloc.i_d_alloc = DNODE_OB ;
  }
  return TRUE ;
}


static void get_global_node_coord (int i_node_coord_no, double *df_x, double *df_y)
/*--------------------------------------------------------------------------------*/
{
  *df_x = s__nodes_coord_alloc.ptrs_nodes_coord [i_node_coord_no].x ;
  *df_y = s__nodes_coord_alloc.ptrs_nodes_coord [i_node_coord_no].y ;
}


/*funkcja dodaje kolejne wspolrzedne do globalnej tablicy wspol. wezlow*/
static BOOL add_nodes_coord (double df_x, double df_y)
/*--------------------------------------------------*/
{
  int i_size ;

  if (FALSE == h_realloc ((T_PTR_Units)&s__nodes_coord_alloc))
  {
    return FALSE ;
  }
  i_size = s__nodes_coord_alloc.i_size ;
  s__nodes_coord_alloc.i_size++ ;
  s__nodes_coord_alloc.ptrs_nodes_coord [i_size].x = df_x ;
  s__nodes_coord_alloc.ptrs_nodes_coord [i_size].y = df_y ;
  return TRUE ;
}



/*funkcja do lokalnej tablicy wezlow obiektu dodaje kolejny wezel*/
static BOOL add_nodes_ob (int i_ob_no, double df_t, int i_node_coord_no)
/*---------------------------------------------------------------------*/
{
  T_PTR_Nodes_Alloc ptrs_nodes_alloc ;
  T_PTR_Node ptrs_nodes ;
  int i_size ;

  ptrs_nodes_alloc = &ptrs__nodes_table [i_ob_no].s_nodes_alloc ;
  if (FALSE == h_realloc ((T_PTR_Units)ptrs_nodes_alloc))
  {
    return FALSE ;
  }
  i_size = ptrs_nodes_alloc->i_size ;
  ptrs_nodes_alloc->i_size++ ;
  ptrs_nodes = ptrs_nodes_alloc->ptrs_nodes_object ;
  ptrs_nodes [i_size].node_no = i_node_coord_no ;
  ptrs_nodes [i_size].df_t = df_t ;
  ptrs_nodes [i_size].use_r = 0 ;
  ptrs_nodes [i_size].use_l = 0 ;
  return TRUE ;
}

/*funkcja dla dwu przecinajacych sie obiektow dodaje ich punkt przeciecia*/
/*i uzupelnia globalna tablice wspolrzednych wezlow*/
static BOOL add_node_ob_ob (int i_ob1_no, int i_ob2_no,
			    double df_x, double df_y,
			    double df_t1, double df_t2)
/*----------------------------------------------------*/
{
  int i ;
  T_PTR_Nodes_Alloc ptrs_nodes1_alloc, ptrs_nodes2_alloc ;
  T_PTR_Node ptrs_nodes1, ptrs_nodes2 ;
  BOOL b_found1, b_found2 ;
  int i_node_coord_no ;
  double df_x1, df_y1 ;

  ptrs_nodes1_alloc = &ptrs__nodes_table [i_ob1_no].s_nodes_alloc ;
  ptrs_nodes2_alloc = &ptrs__nodes_table [i_ob2_no].s_nodes_alloc ;
  ptrs_nodes1 = ptrs_nodes1_alloc->ptrs_nodes_object ;
  ptrs_nodes2 = ptrs_nodes2_alloc->ptrs_nodes_object ;
  b_found1 = FALSE ;
  b_found2 = FALSE ;
  for (i = 0 ; i < ptrs_nodes1_alloc->i_size ; i++)
  {
    get_global_node_coord (ptrs_nodes1 [i].node_no, &df_x1, &df_y1) ;
    if (TRUE == Check_if_Equal (df_x, df_x1) &&
	TRUE == Check_if_Equal (df_y, df_y1))
    {
      b_found1 = TRUE ;
      i_node_coord_no = ptrs_nodes1 [i].node_no ;
      break ;
    }
  }
  for (i = 0 ; i < ptrs_nodes2_alloc->i_size ; i++)
  {
    get_global_node_coord (ptrs_nodes2 [i].node_no, &df_x1, &df_y1) ;
    if (TRUE == Check_if_Equal (df_x, df_x1) &&
	TRUE == Check_if_Equal (df_y, df_y1))
    {
      b_found2 = TRUE ;
      i_node_coord_no = ptrs_nodes2 [i].node_no ;
      break ;
    }
  }
  if (b_found1 == FALSE && b_found2 == FALSE)
  {
    if (FALSE == add_nodes_coord (df_x, df_y))
    {
      return FALSE ;
    }
    i_node_coord_no = s__nodes_coord_alloc.i_size - 1 ;
  }
  if ((FALSE == b_found1 &&
       FALSE == add_nodes_ob (i_ob1_no, df_t1, i_node_coord_no)) ||
       (FALSE == b_found2 &&
	FALSE == add_nodes_ob (i_ob2_no, df_t2, i_node_coord_no)))
  {
    return FALSE ;
  }
  return TRUE ;
}

/*funkcja sprawdza czy dwa objekty przecinaja sie*/
static BOOL make_nodes_ob_ob (long_long l_off_ob1,
			      long_long l_off_ob2,
			      int i_ob1_no,
			      int i_ob2_no)
/*-----------------------------------------*/
{
  NAGLOWEK *nag1, *nag2 ;
#define INTERMAX 16
  double df_x [INTERMAX], df_y [INTERMAX], df_t1 [INTERMAX], df_t2 [INTERMAX] ;
  BOOL b_ret ;
  int i, i_inter ;
  WIELOKAT s_solid1, s_solid2 ;

  b_ret = TRUE ;
  if (l_off_ob1 == l_off_ob2)
  {
    return b_ret ;
  }

  i_inter = 0 ;
  nag1 = (NAGLOWEK*)(dane + l_off_ob1) ;
  nag2 = (NAGLOWEK*)(dane + l_off_ob2) ;

/*---------------------------LINE--------------------------------*/
/*LL*/
  if ((nag1->obiekt == Olinia && nag2->obiekt == Olinia))
  //|| (nag1->obiekt == Olinia3D && nag2->obiekt == Olinia3D))
  {
    i_inter = IntersectionLL ((LINIA *)nag1, (LINIA *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*Ll*/
  if ((nag1->obiekt == Olinia && nag2->obiekt == Oluk))
  {
    i_inter = IntersectionLl ((LINIA *)nag1, (LUK *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*LO*/
  if ((nag1->obiekt == Olinia) && (nag2->obiekt == Ookrag || nag2->obiekt == Okolo))
  {
    i_inter = IntersectionLO ((LINIA *)nag1, (OKRAG *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*LW*/
  if ((nag1->obiekt == Olinia) && (nag2->obiekt == Owwielokat /*|| nag2->obiekt == Owwielokat3D*/))
  {
    i_inter = IntersectionWL ((WIELOKAT*)nag2, (LINIA *)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*LT*/
  if ((nag1->obiekt == Olinia) && nag2->obiekt == Otekst)
  {
    i_inter = IntersectionWL (Text_to_Solid ((TEXT*)nag2, &s_solid1), (LINIA *)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*LSA*/
/*
  if ((nag1->obiekt == Olinia) && nag2->obiekt == Osolidarc)
    {
        i_inter = IntersectionSAL ((SOLIDARC*)nag2, (LINIA *)nag1, df_x, df_y, df_t2, df_t1) ;
    }
  else
  */
/*--------------ARC----------------------------------------*/
/*lL*/
  if (nag1->obiekt == Oluk && (nag2->obiekt == Olinia))
  {
    i_inter = IntersectionLl ((LINIA *)nag2, (LUK *)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*ll*/
  if (nag1->obiekt == Oluk && nag2->obiekt == Oluk)
  {
    i_inter = Intersectionll ((LUK*)nag1, (LUK *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*lO*/
  if (nag1->obiekt == Oluk && (nag2->obiekt == Ookrag || nag2->obiekt == Okolo))
  {
    i_inter = IntersectionlO ((LUK*)nag1, (OKRAG *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*lW*/
  if (nag1->obiekt == Oluk && (nag2->obiekt == Owwielokat))
  {
    i_inter = IntersectionWl ((WIELOKAT*)nag2, (LUK *)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*lT*/
  if (nag1->obiekt == Oluk && nag2->obiekt == Otekst)
  {
    i_inter = IntersectionWl (Text_to_Solid ((TEXT*)nag2, &s_solid1), (LUK *)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*--------------CIRCLE----------------------------------------*/
/*OL*/
  if ((nag1->obiekt == Ookrag || nag1->obiekt == Okolo)  && (nag2->obiekt == Olinia))
  {
    i_inter = IntersectionLO ((LINIA *)nag2, (OKRAG*)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*Ol*/
  if ((nag1->obiekt == Ookrag || nag1->obiekt == Okolo) && nag2->obiekt == Oluk)
  {
    i_inter = IntersectionlO ((LUK*)nag2, (OKRAG*)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*OO*/
  if ((nag1->obiekt == Ookrag || nag1->obiekt == Okolo) && (nag2->obiekt == Ookrag || nag2->obiekt == Okolo))
  {
    i_inter = IntersectionOO ((OKRAG*)nag1, (OKRAG *)nag2, df_x, df_y, df_t1, df_t2) ;
    if (i_inter == 1)
    {
      i_inter = 0 ;
    }
  }
  else
/*OW*/
  if ((nag1->obiekt == Ookrag || nag1->obiekt == Okolo) && (nag2->obiekt == Owwielokat))
  {
    i_inter = IntersectionWO ((WIELOKAT*)nag2, (OKRAG*)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*OT*/
  if ((nag1->obiekt == Ookrag || nag1->obiekt == Okolo)  && nag2->obiekt == Otekst)
  {
    i_inter = IntersectionWO (Text_to_Solid ((TEXT*)nag2, &s_solid1), (OKRAG*)nag1, df_x, df_y, df_t2, df_t1) ;
  }
  else
/*---------------------------SOLID--------------------------------*/
/*WL*/
  if ((nag1->obiekt == Owwielokat) && (nag2->obiekt == Olinia ))
  {
    i_inter = IntersectionWL ((WIELOKAT*)nag1, (LINIA *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*Wl*/
  if ((nag1->obiekt == Owwielokat ) && nag2->obiekt == Oluk)
  {
    i_inter = IntersectionWl ((WIELOKAT*)nag1, (LUK *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*WO*/
  if ((nag1->obiekt == Owwielokat ) && (nag2->obiekt == Ookrag || nag2->obiekt == Okolo))
  {
    i_inter = IntersectionWO ((WIELOKAT*)nag1, (OKRAG *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*WW*/
  if ((nag1->obiekt == Owwielokat ) && (nag2->obiekt == Owwielokat ))
  {
    i_inter = IntersectionWW ((WIELOKAT*)nag1, (WIELOKAT*)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*WT*/
  if ((nag1->obiekt == Owwielokat ) && nag2->obiekt == Otekst)
  {
    i_inter = IntersectionWW ((WIELOKAT*)nag1, Text_to_Solid ((TEXT*)nag2, &s_solid1), df_x, df_y, df_t1, df_t2) ;
  }
  else
/*---------------------------TEXT--------------------------------*/
/*TL*/
  if (nag1->obiekt == Otekst && (nag2->obiekt == Olinia ))
  {
    i_inter = IntersectionWL (Text_to_Solid ((TEXT*)nag1, &s_solid1), (LINIA *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*Tl*/
  if (nag1->obiekt == Otekst && nag2->obiekt == Oluk)
  {
    i_inter = IntersectionWl (Text_to_Solid ((TEXT*)nag1, &s_solid1), (LUK *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*TO*/
  if (nag1->obiekt == Otekst && (nag2->obiekt == Ookrag || nag2->obiekt == Okolo))
  {
    i_inter = IntersectionWO (Text_to_Solid ((TEXT*)nag1, &s_solid1), (OKRAG *)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*TW*/
  if (nag1->obiekt == Otekst && (nag2->obiekt == Owwielokat ))
  {
    i_inter = IntersectionWW (Text_to_Solid ((TEXT*)nag1, &s_solid1), (WIELOKAT*)nag2, df_x, df_y, df_t1, df_t2) ;
  }
  else
/*TT*/
  if (nag1->obiekt == Otekst && nag2->obiekt == Otekst)
  {
    i_inter = IntersectionWW (Text_to_Solid ((TEXT*)nag1, &s_solid1), Text_to_Solid ((TEXT*)nag2, &s_solid2), df_x, df_y, df_t1, df_t2) ;
  }

  for (i = 0 ; i < i_inter ; i++)
  {
    b_ret = add_node_ob_ob (i_ob1_no, i_ob2_no, df_x [i], df_y [i], df_t1 [i], df_t2 [i]) ;
    if (b_ret == FALSE)
    {
      break ;
    }
  }
  return b_ret ;
}

static BOOL make_nodes_ob (long_long l_offb, long_long l_offe, int i_object_no1)
/*------------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long l_off, l_off_ob1 ;
  int i_object_no2 ; 	/*numer obiektu na liscie zaznaczonych*/
  BOOL b_ret ;

  b_ret = TRUE ;

  ptrs__nodes_table [i_object_no1].off_object = l_offb ;

  i_object_no2 = i_object_no1 ;
  l_off_ob1 = l_offb ;
  Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON) ;
  while (l_off != -1)
  {
    nag = (NAGLOWEK*)(dane + l_off) ;
    if (nag->atrybut == Ablok)
    {
      if (FALSE == (b_ret = make_nodes_ob_ob (l_off_ob1, l_off,
					i_object_no1, i_object_no2)))
      {
	break ;
      }
      i_object_no2++ ;
    }
    Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON) ;
  }
  return b_ret ;
}


/*funkcja w petli po obiektach buduje lokalne tablice wezlow i globalna*/
/*tablice wspolrzednych wezlow*/
static BOOL make_nodes_table (long_long l_offb, long_long l_offe)
/*----------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long l_off ;
  int i_object_no ; 	/*numer obiektu na liscie zaznaczonych*/
  BOOL b_ret ;
  char sz_tmp [100] ;
  int i_tmp , i_tmp1 = -1 ;
  int ret;

  InfoList(0);
  strcpy (sz_tmp, "/") ;
  i_object_no = 0 ;
  Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON) ;
  while (l_off != -1)
  {
    if (TRUE == Check_Break_Key ())
    {
      return FALSE ;
    }
    if (i_object_no % 10 == 0)  //5
    {
      i_tmp = (int)(i_object_no * 100L / (i__objects_no + 1)) ;
      if (i_tmp != i_tmp1)
      {
    ret = snprintf(sz_tmp+1, 10, "%d",i_tmp);
          if (i_tmp % 10 == 0) {
              InfoListStr(1, sz_tmp);
          }
	i_tmp1 = i_tmp ;
     }
    }
    nag = (NAGLOWEK*)(dane + l_off) ;
    if (nag->atrybut == Ablok)
    {
      if (FALSE == (b_ret = make_nodes_ob (l_off, l_offe, i_object_no)))
      {
	break ;
      }
      i_object_no++ ;
    }
    Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON) ;
  }
  return b_ret ;
}

/*funkcja dodaje punkt obiektu do lokalnej tablice wezlow obiektu*/
/*i uzupelnia globalna tablice wspolrzednych wezlow*/
static BOOL add_node_ob (int i_ob_no,
			    double df_x,
			    double df_y,
			    double df_t)
/*---------------------------------------*/
{
  int i ;
  T_PTR_Nodes_Alloc ptrs_nodes_alloc ;
  T_PTR_Node ptrs_nodes ;
  BOOL b_found ;
  int i_node_coord_no ;

  ptrs_nodes_alloc = &ptrs__nodes_table [i_ob_no].s_nodes_alloc ;
  ptrs_nodes = ptrs_nodes_alloc->ptrs_nodes_object ;
  b_found = FALSE ;
  for (i = 0 ; i < ptrs_nodes_alloc->i_size ; i++)
  {
     if (TRUE == Check_if_Equal (df_t, ptrs_nodes [i].df_t))
     {
       b_found = TRUE ;
       break ;
     }
  }
  if (b_found == FALSE)
  {
    if (FALSE == add_nodes_coord (df_x, df_y))
    {
      return FALSE ;
    }
    i_node_coord_no = s__nodes_coord_alloc.i_size - 1 ;
    if (FALSE == add_nodes_ob (i_ob_no, df_t, i_node_coord_no))
    {
      return FALSE ;
    }
  }
  return TRUE ;
}


/*funkcja w petli po obiektach inicjuje wezlami brzegowymi lokalne tablice*/
/* wezlow i globalna tablice wspolrzednych wezlow*/
static BOOL ini_nodes_table_edge_vals (long_long l_offb, long_long l_offe)
/*------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  TEXT * ptrs_tekst ;
  WIELOKAT * ptrs_sol ;
  OKRAG *ptrs_circle ;
  long_long l_off ;
  int i_object_no ; 	/*numer obiektu na liscie zaznaczonych*/
  BOOL b_ret ;
  double df_x [4], df_y [4], df_t1, df_t2 ;
  double df_temp ;
  double df_x0, df_x1, df_x2, df_y0, df_y1 ;
  int i ;

  b_ret = TRUE ;
i_object_no = 0;
Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON);
while (l_off != -1)
{
	nag = (NAGLOWEK*)(dane + l_off);
	if (nag->atrybut == Ablok)
	{
		switch (nag->obiekt)
		{
		case Ookrag:
		case Okolo:
			ptrs_circle = (OKRAG*)nag;
			df_x0 = ptrs_circle->x;
			df_x1 = ptrs_circle->x + ptrs_circle->r;
			df_x2 = ptrs_circle->x - ptrs_circle->r;
			df_y0 = ptrs_circle->y;
			df_y1 = ptrs_circle->y + ptrs_circle->r;
			if (FALSE == add_node_ob(i_object_no, df_x1, df_y0, 0.0) ||
				FALSE == add_node_ob(i_object_no, df_x0, df_y1, Pi2 / 4) ||
				FALSE == add_node_ob(i_object_no, df_x2, df_y0, Pi2 / 2))
			{
				b_ret = FALSE;
			}
			break;
		case Otekst:
			ptrs_tekst = (TEXT *)nag;
			Get_Text_Border(ptrs_tekst, df_x, df_y, TRUE);
			if (FALSE == add_node_ob(i_object_no, df_x[3], df_y[3], 0.0) ||
				FALSE == add_node_ob(i_object_no, df_x[0], df_y[0], 1.0) ||
				FALSE == add_node_ob(i_object_no, df_x[1], df_y[1], 2.0) ||
				FALSE == add_node_ob(i_object_no, df_x[2], df_y[2], 3.0))
			{
				b_ret = FALSE;
			}
			break;
		case Owwielokat:
			ptrs_sol = (WIELOKAT*)nag;
			for (i = 0; i < ptrs_sol->lp / 2; i++)
			{
				df_temp = (double)(i + 1);
				if (i == ptrs_sol->lp / 2 - 1)
				{
					df_temp = 0;
				}
				if (FALSE == add_node_ob(i_object_no,
					ptrs_sol->xy[2 * i], ptrs_sol->xy[2 * i + 1], df_temp))
				{
					b_ret = FALSE;
				}
			}
			if (ptrs_sol->lp == 8 && SIMPW != ptrs_sol->obiektt1 &&
				TRUE == Get_Solid_Intersec(ptrs_sol, df_x, df_y, &df_t1, &df_t2))
			{
				if (FALSE == add_node_ob(i_object_no, df_x[0], df_y[0], df_t1) ||
					FALSE == add_node_ob(i_object_no, df_x[0], df_y[0], df_t2))
				{
					b_ret = FALSE;
				}
			}
			break;
		default:
			break;
		}
		i_object_no++;
	}
	if (FALSE == b_ret)
	{
		break;
	}
	Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON);
}
return b_ret;
}


BOOL CHECK_DUPLICATED_LINES=0;  //1  TEMPORARY

/*funkcja w petli po obiektach wyznacza liczba obiektow wskazanych*/
static int get_objectsno(long_long l_offb, long_long l_offe)
/*-------------------------------------------------------*/
{
	NAGLOWEK *nag, *nag0;
	long_long l_off, l_off0;
	int i_object_no;
    long_long l_offb00=l_offb, l_offe00=l_offe;
    long_long l_offb0, l_offe0;
    BOOL accepted;

	i_object_no = 0;
	Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON);
	while (l_off != -1)
	{
		nag = (NAGLOWEK*)(dane + l_off);
		if (nag->atrybut == Ablok)
		{
            //checking if not uplicated  (OPTIONAL)
            accepted=TRUE;

            //this is OPTIONAL because it requires many comparisons; it must be done also with the combination of line - solid, solid - line, solid - solid
            if (CHECK_DUPLICATED_LINES) {
                if (nag->obiekt == Olinia) {
                    l_offb0 = l_offb00;
                    l_offe0 = l_offe00;

                    Object_Tok(&l_offb0, l_offe0, &l_off0, ONieOkreslony, TOK_LAYER_ON);
                    while (l_off0 != -1) {
                        nag0 = (NAGLOWEK *) (dane + l_off0);
                        if (nag0 != nag) {
                            if ((nag0->atrybut == Ablok) && (nag0->obiekt == Olinia)) {
                                if (((Check_if_Equal(((LINIA *) nag0)->x1, ((LINIA *) nag)->x1)) &&
                                    (Check_if_Equal(((LINIA *) nag0)->x2, ((LINIA *) nag)->x2)) &&
                                    (Check_if_Equal(((LINIA *) nag0)->y1, ((LINIA *) nag)->y1)) &&
                                    (Check_if_Equal(((LINIA *) nag0)->y2, ((LINIA *) nag)->y2)))
                                ||
                                ((Check_if_Equal(((LINIA *) nag0)->x1, ((LINIA *) nag)->x2)) &&
                                    (Check_if_Equal(((LINIA *) nag0)->x2, ((LINIA *) nag)->x1)) &&
                                    (Check_if_Equal(((LINIA *) nag0)->y1, ((LINIA *) nag)->y2)) &&
                                    (Check_if_Equal(((LINIA *) nag0)->y2, ((LINIA *) nag)->y1))))
                                {
                                    accepted = FALSE;
                                    break;
                                }
                            }
                        }
                        Object_Tok(&l_offb0, l_offe0, &l_off0, ONieOkreslony, TOK_LAYER_ON);
                    }
                }
            }

            if (accepted) i_object_no++;
            else nag->atrybut=Apppoblok;
		}
		Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON);
	}
	return i_object_no;
}

static void free_nodes_mem (void)
/*------------------------------*/
{
  int i ;

  h_free (s__nodes_coord_alloc.ptrs_nodes_coord) ;
  if (NULL != ptrs__nodes_table)
  {
    for (i = 0 ; i < i__objects_no ; i++)
    {
      h_free (ptrs__nodes_table [i].s_nodes_alloc.ptrs_nodes_object) ;
    }
  }
  h_free (ptrs__nodes_table) ;
}

extern int Make_areas (long_long l_offb, long_long l_offe, int seeds_no,
		 double df_pointx[], double df_pointy[], double *df_pointx_, double *df_pointy_,
		 int i_objects_no,
		 T_PTR_Node_Coord_Alloc ptrs_nodes_coord_alloc,
		 T_PTR_Nodes_Object ptrs_nodes_table,
		 T_PTR_Area ptrs_area) ;

extern BOOL Test_areas (long_long l_offb, long_long l_offe,
		 double df_pointx, double df_pointy,
		 int i_objects_no,
		 T_PTR_Node_Coord_Alloc ptrs_nodes_coord_alloc,
		 T_PTR_Nodes_Object ptrs_nodes_table,
		 T_PTR_Area ptrs_area) ;

extern "C" {
int hatch_proc (long_long l_offb, long_long l_offe, int seeds_no,
	    double df_pointx[], double df_pointy[],
	    T_PTR_Hatch_Param ptrs_hatch_param, int comput_area,
	    double df_apx1, double df_apy1, double df_apx2, double df_apy2, void *adb, int to_block=0, int draw=TRUE);
}

extern "C" {
BOOL  hatch_proc_test (long_long l_offb, long_long l_offe,
	    double df_pointx, double df_pointy,
	    T_PTR_Hatch_Param ptrs_hatch_param, int comput_area,
	    double df_apx1, double df_apy1, double df_apx2, double df_apy2);
}

int hatch_proc (long_long l_offb0, long_long l_offe0, int seeds_no,
	    double df_pointx[], double df_pointy[],
	    T_PTR_Hatch_Param ptrs_hatch_param, int comput_area,
	    double df_apx1, double df_apy1, double df_apx2, double df_apy2, void *abd, int to_block, int draw)
/*------------------------------------------------------------------------------------------------------*/
{
  s__nodes_coord_alloc.ptrs_nodes_coord = NULL ;
  ptrs__nodes_table = NULL ;
  s__nodes_coord_alloc.i_size = 0 ;
  s__nodes_coord_alloc.i_size_alloc = 0 ;
  s__nodes_coord_alloc.ptrs_nodes_coord = NULL ;
  long_long l_offb, l_offe;
  int shadows_no, solidarc_shadows_no, arc_shadows_no, el_shadows_no;
  long_long le_abd;
  int ret;
  double df_pointx_, df_pointy_;
  double DF_PRECISION_;

  if (abd!=NULL)  le_abd = (long_long)dane + dane_size - (long_long)abd;  //position of existing blok from end of data
  else le_abd = -1;

  l_offb = l_offb0;
  l_offe = l_offe0;
  shadows_no = 0;
  solidarc_shadows_no=0;
  arc_shadows_no = 0;
  el_shadows_no = 0;

    solidarc_shadows_no = make_solidarc_shadows(l_offb, l_offe, Ablok, FALSE, Anormalny, 1, to_block);
    if (solidarc_shadows_no > 0)
    {
        l_offb = 0;
        l_offe = dane_size;
    }
    else
    {
        l_offb = l_offb0;
        l_offe = l_offe0;
    }


    shadows_no = make_spline_shadows(l_offb, l_offe, Ablok, FALSE, Anormalny, 1, to_block);  //1
  if (shadows_no > 0)
  {
	  l_offb = 0;
	  l_offe = dane_size;
  }
  else
  {
	  l_offb = l_offb0;
	  l_offe = l_offe0;
  }

  if (ptrs_hatch_param->i_number_hatch == 0)   ////WARNING  ==   >=
  {

      int to_block1;
      if (comput_area==10) to_block1=0;
      else to_block1=to_block;
	  arc_shadows_no = make_arc_shadows(l_offb, l_offe, Ablok, FALSE, Anormalny /*Apppoblok*/, 1, to_block1);
	  if (arc_shadows_no > 0)
	  {
		l_offb = 0;
		l_offe = dane_size;
	  }
	  else
	  {
		  if (shadows_no > 0)
		  {
			  l_offb = 0;
			  l_offe = dane_size;
		  }
		  else
		  {
			  l_offb = l_offb0;
			  l_offe = l_offe0;
		  }
	  }
  }


    el_shadows_no = make_elliptical_shadows(l_offb, l_offe, Ablok, FALSE, Anormalny, 1, to_block);  //1
    if (el_shadows_no > 0)
    {
        l_offb = 0;
        l_offe = dane_size;
    }
    else {
        if ((shadows_no + arc_shadows_no) > 0) {
            l_offb = 0;
            l_offe = dane_size;
        } else {
            l_offb = l_offb0;
            l_offe = l_offe0;
        }
    }


  if (0 == (i__objects_no = get_objectsno (l_offb, l_offe)))
  {
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);
    return 0;
  }

  if (i__objects_no>MAX_NO_OF_OBJECTS)
  {
      if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);
      ErrListStr(_TO_MANY_OBJECTS_);
      return 0;
  }


  if (FALSE == h_alloc ((char  **)&ptrs__nodes_table,
		i__objects_no * sizeof(ptrs__nodes_table [0])))
  {
	  if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);
    return 0;
  }
  if (FALSE == h_alloc (
		(char  **)&s__nodes_coord_alloc.ptrs_nodes_coord,
		s__nodes_coord_alloc.i_unit_size *
		s__nodes_coord_alloc.i_d_alloc))
  {
    free_nodes_mem () ;
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);
    return 0;
  }
  if (FALSE == ini_nodes_table_alloc ())
  {
    free_nodes_mem () ;
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);
    return 0;
  }


  if (FALSE == make_nodes_table (l_offb, l_offe))
  {
    free_nodes_mem () ;
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);
    return 0;
  }
  if (FALSE == ini_nodes_table_edge_vals (l_offb, l_offe))
  {
    free_nodes_mem () ;
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);
    return 0;
  }
  sort_nodes_table (l_offb, l_offe) ;

    //DF_PRECISION_=DF_PRECISION;
    //DF_PRECISION=1000;

  ret = Make_areas (l_offb, l_offe, seeds_no,
			   df_pointx, df_pointy,
                &df_pointx_, &df_pointy_,
			   i__objects_no,
			   &s__nodes_coord_alloc,
			   ptrs__nodes_table,
			   &s__area);

    //DF_PRECISION=DF_PRECISION_;

    if (ret != 1) {
        if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);
        return ret;
    }

    if  (abd !=NULL) abd=PTR__GTMP7-1;

    Make_Hatch (ptrs_hatch_param, &s__area, comput_area, df_apx1, df_apy1, df_apx2, df_apy2, df_pointx_, df_pointy_, /*abd*/ le_abd, to_block, draw) ;

    h_free ((void *)s__area.s_arcs_cpy_alloc.ptrs_arcs) ;
    h_free ((void *)s__area.s_vertexs_cpy_alloc.ptrs_vertexs) ;
    h_free ((void *)s__area.s_vertexs_alloc.ptrs_vertexs) ;
    h_free ((void *)s__area.s_lines_alloc.ptrs_lines) ;
    h_free ((void *)s__area.s_arcs_alloc.ptrs_arcs) ;
	h_free ((void*)s__area.s_traces_alloc.ptrs_traces);


    if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(TRUE);

	return 1;
}


BOOL hatch_proc_test (long_long l_offb0, long_long l_offe0,
	    double df_pointx, double df_pointy,
	    T_PTR_Hatch_Param ptrs_hatch_param, int comput_area,
	    double df_apx1, double df_apy1, double df_apx2, double df_apy2)
/*-------------------------------------------------------------------*/
{
    int to_block=0;

  s__nodes_coord_alloc.ptrs_nodes_coord = NULL ;
  ptrs__nodes_table = NULL ;
  s__nodes_coord_alloc.i_size = 0 ;
  s__nodes_coord_alloc.i_size_alloc = 0 ;
  s__nodes_coord_alloc.ptrs_nodes_coord = NULL ;
  long_long l_offb, l_offe;
  int shadows_no, solidarc_shadows_no, arc_shadows_no, el_shadows_no;

  l_offb = l_offb0;
  l_offe = l_offe0;
  shadows_no = 0;
  solidarc_shadows_no=0;
  arc_shadows_no = 0;
  el_shadows_no = 0;

    solidarc_shadows_no = make_solidarc_shadows(l_offb, l_offe, Ablok, FALSE, Anormalny, 1, to_block);
    if (solidarc_shadows_no > 0)
    {
        l_offb = 0;
        l_offe = dane_size;
    }
    else
    {
        l_offb = l_offb0;
        l_offe = l_offe0;
    }

    shadows_no = make_spline_shadows(l_offb, l_offe, Ablok, FALSE, Anormalny, 1, to_block);  //1
    if (shadows_no > 0)
    {
        l_offb = 0;
        l_offe = dane_size;
    }
    else
    {
        l_offb = l_offb0;
        l_offe = l_offe0;
    }

    el_shadows_no = make_elliptical_shadows(l_offb, l_offe, Ablok, FALSE, Anormalny, 1, to_block);  //1
    if (el_shadows_no > 0)
    {
        l_offb = 0;
        l_offe = dane_size;
    }
    else {
        if ((shadows_no) > 0) {
            l_offb = 0;
            l_offe = dane_size;
        } else {
            l_offb = l_offb0;
            l_offe = l_offe0;
        }
    }

    if (0 == (i__objects_no = get_objectsno (l_offb, l_offe)))
  {
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(FALSE);
    return FALSE;
  }
  if (FALSE == h_alloc ((char  **)&ptrs__nodes_table,
		i__objects_no * sizeof(ptrs__nodes_table [0])))
  {
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(FALSE);
    return FALSE;
  }
  if (FALSE == h_alloc (
		(char  **)&s__nodes_coord_alloc.ptrs_nodes_coord,
		s__nodes_coord_alloc.i_unit_size *
		s__nodes_coord_alloc.i_d_alloc))
  {
    free_nodes_mem () ;
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(FALSE);
    return FALSE;
  }
  if (FALSE == ini_nodes_table_alloc ())
  {
    free_nodes_mem () ;
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(FALSE);
    return FALSE;
  }

  if (FALSE == make_nodes_table (l_offb, l_offe))
  {
    free_nodes_mem () ;
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(FALSE);
    return FALSE;
  }
  if (FALSE == ini_nodes_table_edge_vals (l_offb, l_offe))
  {
    free_nodes_mem () ;
	if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(FALSE);
    return FALSE;
  }
  sort_nodes_table (l_offb, l_offe) ;

  if (FALSE == Test_areas (l_offb, l_offe,
			   df_pointx, df_pointy,
			   i__objects_no,
			   &s__nodes_coord_alloc,
			   ptrs__nodes_table,
			   &s__area))
  {
      if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(FALSE);
    return FALSE;
  }
  if ((solidarc_shadows_no + shadows_no + arc_shadows_no + el_shadows_no) > 0) delete_spline_shadows(FALSE);
  return TRUE;
}

#undef __O_HATCH1__