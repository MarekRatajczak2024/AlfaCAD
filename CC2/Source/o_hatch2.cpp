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
#include "bib_blok.h"
#include "b_menu.h"

#include "leak_detector_cpp.hpp"


static int i__objects_no ; 		/*liczba obiektow wskazanych*/
static int i__areasno, i__areasno_max ;	/*liczba obszarow, max. liczba obszarow*/
static int i__area_sel ;		/*numer wskazanego obszaru*/
static T_PTR_Node_Coord_Alloc ptrs__nodes_coord_alloc ;/*adres tablicy wspolrzednych wezlow*/
static T_PTR_Nodes_Object ptrs__nodes_table ;	/*tablica o rozmiarze i__objects_no,
						o elemencie zawierajacym
						adres tablicy wezlow obiektu*/
static T_PTR_Area ptrs__areas ;			/*adres tablicy obszarow*/
static T_PTR_Area ptrs__area ;			/*obszar po kondensacji*/

static int add_vertex (T_PTR_Area, int) ;
static BOOL add_area_seg (int, int, int, BOOL, int, int, double *, T_PTR_Area) ;
static T_PTR_Area ini_area_struc (T_PTR_Area = NULL) ;
static BOOL free_area_mem (int = -1) ;
void Free_Areas_Mem(void);
static void get_global_node_coord (int , double *, double *) ;
static void get_object_node_coord (T_PTR_Nodes_Object, int , double *, double *) ;
static T_PTR_Vertex get_vertex (T_PTR_Area, int) ;
static BOOL add_area_trace (T_PTR_Area, int, int, BOOL) ;
static BOOL check_area (T_PTR_Area) ;
static void erase_area_trace (T_PTR_Area) ;

int ababab = 0;


static double atan2_my  (double y, double x)
{
  if (y == 0 && x == 0)
  {
   Internal_Error (__LINE__,__FILE__) ;
   return 0 ;
  }
  return atan2 (y, x) ;
}


static T_PTR_Area ini_area_struc (T_PTR_Area ptrs_ini_area)
/*--------------------------------------------------------------*/
{
  T_PTR_Area ptrs_area ;

  if (ptrs_ini_area == NULL)
  {
    if (i__areasno >= i__areasno_max)
    {
      ErrList (18) ;
      return NULL ;
    }
    ptrs_area = &ptrs__areas [i__areasno] ;
  }
  else
  {
    ptrs_area = ptrs_ini_area ;
  }
  ptrs_area->sel = AST_NOSEL ;	/*czy wskazany*/
  ptrs_area->hole = 0 ;	/*dziura*/
  ptrs_area->close = 0 ;
  ptrs_area->s_vertexs_alloc.ptrs_vertexs = NULL ;
  ptrs_area->s_vertexs_alloc.i_size = 0 ;
  ptrs_area->s_vertexs_alloc.i_size_alloc = 0 ;
  ptrs_area->s_vertexs_alloc.i_unit_size = sizeof(T_Vertex) ;
  ptrs_area->s_vertexs_alloc.i_d_alloc = DAREAVERTEX ;

  ptrs_area->s_vertexs_cpy_alloc.ptrs_vertexs = NULL ;
  ptrs_area->s_vertexs_cpy_alloc.i_size = 0 ;
  ptrs_area->s_vertexs_cpy_alloc.i_size_alloc = 0 ;
  ptrs_area->s_vertexs_cpy_alloc.i_unit_size = sizeof(T_Vertex) ;
  ptrs_area->s_vertexs_cpy_alloc.i_d_alloc = DAREAVERTEX ;

  ptrs_area->s_traces_alloc.ptrs_traces = NULL ;
  ptrs_area->s_traces_alloc.i_size = 0 ;
  ptrs_area->s_traces_alloc.i_size_alloc = 0 ;
  ptrs_area->s_traces_alloc.i_unit_size = sizeof(T_ATrace) ;
  ptrs_area->s_traces_alloc.i_d_alloc = DAREAVERTEX ;

  ptrs_area->s_lines_alloc.ptrs_lines = NULL ;
  ptrs_area->s_lines_alloc.i_size = 0 ;
  ptrs_area->s_lines_alloc.i_size_alloc = 0 ;
  ptrs_area->s_lines_alloc.i_unit_size = sizeof(T_ALine) ;
  ptrs_area->s_lines_alloc.i_d_alloc = DAREALINE ;

  ptrs_area->s_arcs_alloc.ptrs_arcs = NULL ;
  ptrs_area->s_arcs_alloc.i_size = 0 ;
  ptrs_area->s_arcs_alloc.i_size_alloc = 0 ;
  ptrs_area->s_arcs_alloc.i_unit_size = sizeof(T_AArc) ;
  ptrs_area->s_arcs_alloc.i_d_alloc = DAREAARC ;

  ptrs_area->s_arcs_cpy_alloc.ptrs_arcs = NULL ;
  ptrs_area->s_arcs_cpy_alloc.i_size = 0 ;
  ptrs_area->s_arcs_cpy_alloc.i_size_alloc = 0 ;
  ptrs_area->s_arcs_cpy_alloc.i_unit_size = sizeof(T_AArc) ;
  ptrs_area->s_arcs_cpy_alloc.i_d_alloc = DAREAARC ;

  if (FALSE == h_alloc ((char  **) &
      ptrs_area->s_vertexs_alloc.ptrs_vertexs, 0) ||
      FALSE == h_alloc ((char  **) &
      ptrs_area->s_vertexs_cpy_alloc.ptrs_vertexs, 0) ||
      FALSE == h_alloc ((char  **) &
      ptrs_area->s_traces_alloc.ptrs_traces, 0) ||
      FALSE == h_alloc ((char  **) &
      ptrs_area->s_lines_alloc.ptrs_lines, 0) ||
      FALSE == h_alloc ((char  **) &
      ptrs_area->s_arcs_alloc.ptrs_arcs, 0) ||
      FALSE == h_alloc ((char  **) &
      ptrs_area->s_arcs_cpy_alloc.ptrs_arcs, 0))
  {
    return NULL ;
  }
  if (ptrs_ini_area == NULL)
  {
    i__areasno++ ;
  }
  return ptrs_area ;
}

static BOOL free_area_mem (int i_area_no)
/*-------------------------------------*/
{
  T_PTR_Area ptrs_area ;

  if (i_area_no == -1)
  {
    ptrs_area = ptrs__area ;
  }
  else
  {
    ptrs_area = &ptrs__areas [i_area_no] ;
  }
  if (ptrs_area != NULL)
  {
      h_free(ptrs_area->s_arcs_alloc.ptrs_arcs);
      h_free(ptrs_area->s_lines_alloc.ptrs_lines);
      h_free(ptrs_area->s_traces_alloc.ptrs_traces);
      h_free(ptrs_area->s_vertexs_alloc.ptrs_vertexs);
      h_free(ptrs_area->s_vertexs_cpy_alloc.ptrs_vertexs);
      h_free(ptrs_area->s_arcs_cpy_alloc.ptrs_arcs);
  }
  return TRUE ;
}

static void free_areas_mem (void)
/*------------------------------*/
{
  int i ;

  for (i = i__areasno - 1 ; i >= 0  ; i--)
  {
    free_area_mem (i) ;
  }
}

void Free_Areas_Mem(void)
{
    free_areas_mem();
}

void
Get_Tangent_Ob (void *ptr_ob,
		double df_x1,
		double df_y1,
		double *ptrdf_x2,
		double *ptrdf_y2,
		BOOL b_right_dir)
/*------------------------------*/
{
  double df_dx, df_dy, df_x2, df_y2 ;
  double df_si, df_co, df_dist, df_a ;
  NAGLOWEK *nag ;
  LUK *ptrs_arc, s_arc ;
  OKRAG *ptrs_circle ;
  //SOLIDARC *ptrs_solidarc;

  df_x2 = *ptrdf_x2 ;
  df_y2 = *ptrdf_y2 ;
  nag = (NAGLOWEK*)ptr_ob ;
  if (nag->obiekt == Oluk)
  {
    ptrs_arc = (LUK*)nag ;
    s_arc.x = ptrs_arc->x ;
    s_arc.y = ptrs_arc->y ;
    s_arc.r = ptrs_arc->r ;
  }
  else
  {
    ptrs_circle = (OKRAG*)nag ;
    s_arc.x = ptrs_circle->x ;
    s_arc.y = ptrs_circle->y ;
    s_arc.r = ptrs_circle->r ;
  }
  if (b_right_dir == TRUE)
  {
    Points_To_Arc (&s_arc, df_x1, df_y1, df_x2, df_y2) ;
  }
  else
  {
    Points_To_Arc (&s_arc, df_x2, df_y2, df_x1, df_y1) ;
  }
  df_dx = df_x2 - df_x1 ;
  df_dy = df_y2 - df_y1 ;
  df_a = atan2_my  (df_dy, df_dx) ;
  df_si = sin (- df_a) ;
  df_co = cos (- df_a) ;
  df_dist = sqrt (df_dx * df_dx + df_dy * df_dy) ;
  Rotate_Arc (&s_arc, df_x1, df_y1, df_si, df_co) ;
  df_x2 = df_x1 ;
  df_y2 = df_y1 + 100 ;
  if (FALSE == Check_if_Equal0 (df_y1, s_arc.y))  ////WARNING
  {
    df_x2 = df_x1 + df_dist / 2 ;
    df_y2 = df_y1 + 0.25 * df_dist * df_dist / (df_y1 - s_arc.y) ;
  }
  if (FALSE == Point_Arc (df_x2, df_y2, &s_arc))
  {
    df_x2 = df_x1 ;
    df_y2 = df_y1 - 100 ;
    if (FALSE == Check_if_Equal0 (df_y1, s_arc.y))  ////WARNING
    {
      df_x2 = df_x1 - df_dist / 2 ;
      df_y2 = df_y1 - 0.25 * df_dist * df_dist / (df_y1 - s_arc.y) ;
    }
  }
  df_si = sin (df_a) ;
  df_co = cos (df_a) ;
  Rotate_Point (df_si, df_co, df_x1, df_y1, df_x2, df_y2, &df_x2, &df_y2) ;
  *ptrdf_x2 = df_x2 ;
  *ptrdf_y2 = df_y2 ;
}

static double check_seg (double df_angle,
		       int i_node_coor,
		       int i_object_no,
		       int i_node_no,
		       BOOL b_right_dir,
		       double *ptrdf_r)
/*----------------------------------------*/
{
  double df_x1, df_y1, df_x2, df_y2 ;
  double df_dx, df_dy, df_ret_angle ;
  T_PTR_Nodes_Object ptrs_nodes_ob ;
  NAGLOWEK *nag ;

  *ptrdf_r = 1.0E+10 ;
  if (b_right_dir == FALSE)
  {
    *ptrdf_r = 1.0E-10;
  }
  ptrs_nodes_ob = &ptrs__nodes_table [i_object_no] ;
  get_global_node_coord (i_node_coor, &df_x1, &df_y1) ;
  get_object_node_coord (ptrs_nodes_ob, i_node_no, &df_x2, &df_y2) ;
  if (df_x1 == df_x2 && df_y1 == df_y2)
  {
    Internal_Error (__LINE__,__FILE__) ;
    return Pi2 ;
  }
  nag = (NAGLOWEK*)(dane + ptrs_nodes_ob->off_object) ;
  switch (nag->obiekt)
  {
    case Olinia :
    case Otekst :
    case Owwielokat :
      break ;
    case Oluk :
    case Ookrag :
    case Okolo :
      if (nag->obiekt == Oluk)
      {
	    *ptrdf_r = ((LUK*)nag)->r ;
      }
      else
      {
	    *ptrdf_r = ((OKRAG*)nag)->r ;
      }
      Get_Tangent_Ob (nag, df_x1, df_y1, &df_x2, &df_y2, b_right_dir) ;
      break ;
    default :
      Internal_Error (__LINE__,__FILE__) ;
      return Pi2 ;
      break ;
  }
  df_dx = df_x2 - df_x1 ;
  df_dy = df_y2 - df_y1 ;
  obru (sin (df_angle), cos (df_angle), df_dx, df_dy, &df_x2, &df_y2) ;
  df_ret_angle = Angle_Normal (atan2_my  (df_y2, df_x2)) ;
  if (Oluk == nag->obiekt ||
      Ookrag == nag->obiekt ||
      Okolo == nag->obiekt)
  {
    if (fabs (df_ret_angle) <= Get_Prec (MPREC_3, 1, 1) ||
	fabs (df_ret_angle - Pi2) <= Get_Prec (MPREC_3, 1, 1))
    {
      df_ret_angle = 0 ;
    }
    if (fabs (df_ret_angle - Pi2 / 2) <= Get_Prec (MPREC_3, 1, 1))
    {
      df_ret_angle = Pi2 / 2 ;
    }
  }
  if (TRUE == Check_if_LE (df_ret_angle, Pi2 / 2))
  {
    df_ret_angle = Pi2 / 2 - df_ret_angle ;
  }
  else
  {
    df_ret_angle = 3 * Pi2 / 2 - df_ret_angle ;
  }
  if (Oluk == nag->obiekt ||
      Ookrag == nag->obiekt ||
      Okolo == nag->obiekt)
  {
    if (TRUE == Check_if_Equal0 (df_ret_angle, 0) && b_right_dir == TRUE)  ////WARNING
    {
      df_ret_angle = Pi2 ;
    }
  }
  return df_ret_angle ;
}

static BOOL comp_angle (double df_angle,
			double df_radius,
			BOOL b_dir,
			double df_angle_comp,
			double df_radius_comp,
			BOOL b_dir_comp)  /*dla luku dodatnia czy ujemna krzywizna*/
/*-------------------------------------------*/
{
  BOOL b_ret ;

  b_ret = FALSE ;

  if (TRUE == Check_if_GT (df_angle, df_angle_comp))
  {
    return TRUE ;
  }
  if (TRUE == Check_if_LT (df_angle, df_angle_comp))
  {
    return FALSE ;
  }
  if ((b_dir == FALSE && b_dir_comp == TRUE) ||
      (b_dir == TRUE && b_dir_comp == TRUE && df_radius > df_radius_comp) ||
      (b_dir == FALSE && b_dir_comp == FALSE && df_radius < df_radius_comp))
  {
    b_ret = TRUE ;
  }
  return b_ret ;
}

/*funkcja sprawdza czy punkt nie jest wewnetrznym wezlem obszaru*/
static BOOL check_if_internal_node (T_PTR_Area ptrs_area,
				    double     df_x,
				    double     df_y)
/*------------------------------------------------------*/
{
  double df_xtest, df_ytest ;
  int i, i_vertexno ;
  BOOL b_close ;

  i_vertexno = ptrs_area->s_vertexs_alloc.i_size ;
  b_close = FALSE ;
  for (i = 1 ; i < i_vertexno ; i++)
  {
    df_xtest = get_vertex (ptrs_area, i)->x ;
    df_ytest = get_vertex (ptrs_area, i)->y ;
    if (TRUE == Check_if_Equal0 (df_xtest, df_x) &&  ////WARNING
	TRUE == Check_if_Equal0 (df_ytest, df_y))  ////WARNING
    {
      b_close = TRUE ;
      break ;
    }
  }
  return b_close ;
}


/*funkcja generuje kolejne segmenty obszaru*/
static BOOL
add_area_next_seg (int 		    i_object_no,
		   int 		    i_node0_no,
		   int 		    i_node1_no,
		   int 		    i_vertex1_no,
		   double 	    df_angle,
		   T_PTR_Area 	    ptrs_area)
/*------------------------------------------------*/
{
  static T_PTR_Nodes_Object  ptrs_nodes_object ;
  static T_PTR_Nodes_Object ptrs_nodes_ob, ptrs_nodes_ob_found ;
  static T_PTR_Node ptrs_nodes ;
  static int i_object, i_node, i_node_coor_look, i_node_found1, i_node_found2 ;
  static int i_next, i_nodesno, i_object_found ;
  static int i_vertex2_no ;
  static BOOL b_found, b_cicle, b_right_dir, b_next, b_close, b_internal ;
  static double df_angle_found, df_radius_found,
		df_angle_comp, df_radius_comp ;
  static BOOL b_dir_found, b_dir_comp ;
  static double df_x, df_y, df_x0, df_y0 ;

  b_found = FALSE ;
  df_angle_found = Pi2 ;
  df_radius_found = 0 ;
  b_dir_found = FALSE ;
  ptrs_nodes_object = &ptrs__nodes_table [i_object_no] ;
  ptrs_nodes = ptrs_nodes_object->s_nodes_alloc.ptrs_nodes_object ;
  i_node_coor_look = ptrs_nodes [i_node1_no].node_no ;
  for (i_object = 0 ; i_object < i__objects_no ; i_object++)
  {
    ptrs_nodes_ob = &ptrs__nodes_table [i_object] ;
    b_cicle = FALSE ;
    if (Ookrag == ((NAGLOWEK*)(dane + ptrs_nodes_ob->off_object))->obiekt ||
	Okolo == ((NAGLOWEK*)(dane + ptrs_nodes_ob->off_object))->obiekt ||
	Otekst == ((NAGLOWEK*)(dane + ptrs_nodes_ob->off_object))->obiekt ||
	Owwielokat == ((NAGLOWEK*)(dane + ptrs_nodes_ob->off_object))->obiekt)
    {
      b_cicle = TRUE ;
    }
    ptrs_nodes = ptrs_nodes_ob->s_nodes_alloc.ptrs_nodes_object ;
    i_nodesno = ptrs_nodes_ob->s_nodes_alloc.i_size ;
    for (i_node = 0 ; i_node < i_nodesno ; i_node++)
    {
      if (ptrs_nodes [i_node].node_no == i_node_coor_look)
      {
	b_next = FALSE ;
	if (i_node > 0 ) /*nie pierwszy*/
	{
	  i_next = i_node - 1 ;
	  b_next = TRUE ;
	}
	if (i_node == 0  && TRUE == b_cicle) /*pierwszy*/
	{
	  i_next = i_nodesno - 1 ;
	  b_next = TRUE ;
	}
	if (b_next == TRUE)
	{
	  if ( !(i_object_no == i_object &&
		((i_node0_no == i_node && i_node1_no == i_next) ||
		(i_node0_no == i_next && i_node1_no == i_node))))
	  {
	    df_angle_comp = check_seg (df_angle,
				       i_node_coor_look,
				       i_object,
				       i_next,
				       FALSE,
				       &df_radius_comp) ;
	    if (TRUE == comp_angle (df_angle_found,
				    df_radius_found,
				    b_dir_found,
				    df_angle_comp,
				    df_radius_comp,
				    FALSE))	    /*poprzedni*/
	    {
	      df_angle_found = df_angle_comp ;
	      df_radius_found = df_radius_comp ;
	      b_dir_found = b_dir_comp ;
	      b_found = TRUE ;
	      b_right_dir = FALSE ;
	      i_object_found = i_object ;
	      i_node_found1 = i_node ;
	      i_node_found2 = i_next ;
	    }
	  }
	}
	b_next = FALSE ;
	if (i_node < i_nodesno - 1) /*nie ostatni*/
	{
	  i_next = i_node + 1 ;
	  b_next = TRUE ;
	}
	if (i_node == i_nodesno - 1 && TRUE == b_cicle) /*ostatni*/
	{
	  i_next = 0 ;
	  b_next = TRUE ;
	}
	if (TRUE == b_next)
	{
	  if ( !(i_object_no == i_object &&
		((i_node0_no == i_node && i_node1_no == i_next) ||
		(i_node0_no == i_next && i_node1_no == i_node))))
	  {
	    df_angle_comp = check_seg (df_angle,
				       i_node_coor_look,
				       i_object,
				       i_next,
				       TRUE,
				       &df_radius_comp) ;
	    if (TRUE == comp_angle (df_angle_found,
				    df_radius_found,
				    b_dir_found,
				    df_angle_comp,
				    df_radius_comp,
				    TRUE))	    /*poprzedni*/
	    {
	      df_angle_found = df_angle_comp ;
	      df_radius_found = df_radius_comp ;
	      b_dir_found = b_dir_comp ;
	      b_found = TRUE ;
	      b_right_dir = TRUE ;
	      i_object_found = i_object ;
	      i_node_found1 = i_node ;
	      i_node_found2 = i_next ;
	    }
	  }
	}
      }
    }
  }
  if (TRUE == b_found)
  {
    ptrs_nodes_ob_found = &ptrs__nodes_table [i_object_found] ;
    get_object_node_coord (ptrs_nodes_ob_found, i_node_found2, &df_x, &df_y) ;
    ptrs_nodes = ptrs_nodes_ob_found->s_nodes_alloc.ptrs_nodes_object ;
	ababab++;
	if (ababab == 60)
	{
		ababab = ababab;
	}
    if (((TRUE == b_right_dir) && (ptrs_nodes [i_node_found1].use_r == 1)) ||
	((FALSE == b_right_dir) && (ptrs_nodes [i_node_found2].use_l == 1)) ||
	TRUE == (b_internal = check_if_internal_node (ptrs_area, df_x, df_y)))
    {
      if (TRUE == b_internal)
      {
	erase_area_trace (ptrs_area) ;
      }
      return TRUE ;
    }
    ptrs_nodes = &ptrs_nodes_ob_found->s_nodes_alloc.ptrs_nodes_object [i_node_found2] ;
    df_x0 = get_vertex (ptrs_area, 0)->x ;
    df_y0 = get_vertex (ptrs_area, 0)->y ;
    b_close = FALSE ;
	
	if (ababab == 59)
	{
		ababab = ababab;
	}
	
    if (TRUE == Check_if_Equal0 (df_x0, df_x) &&  ////WARNING
	TRUE == Check_if_Equal0 (df_y0, df_y))   ////WARNING
    {
      b_close = TRUE ;
      i_vertex2_no = 0 ;
    }
    else
    {
      if (-1 ==  (i_vertex2_no = add_vertex (ptrs_area, ptrs_nodes->node_no)))
      {
	return FALSE ;
      }
    }
    if (FALSE == add_area_seg (i_object_found,
			     i_node_found1,
			     i_node_found2,
			     b_right_dir,
			     i_vertex1_no,
			     i_vertex2_no,
			     &df_angle,
			     ptrs_area))
    {
      return FALSE ;
    }
    if (TRUE == b_close)
    {
      ptrs_area->close = 1 ;
      return TRUE ;
    }
    if (FALSE == add_area_next_seg (i_object_found,
				  i_node_found1,
				  i_node_found2,
				  i_vertex2_no,
				  df_angle,
				  ptrs_area))
    {
      return FALSE ;
    }
  }
  return TRUE ;
}


static void get_object_node_coord (T_PTR_Nodes_Object ptrs_nodes_ob,
				   int i_node_no,
				   double *df_x,
				   double *df_y)
/*-------------------------------------------------------------------*/
{
  T_PTR_Node ptrs_nodes ;
  int i_node_coor ;

  ptrs_nodes = ptrs_nodes_ob->s_nodes_alloc.ptrs_nodes_object ;
  i_node_coor = ptrs_nodes [i_node_no].node_no ;
  get_global_node_coord (i_node_coor, df_x, df_y) ;
}

static void get_global_node_coord (int i_node_coord_no, double *df_x, double *df_y)
/*--------------------------------------------------------------------------------*/
{
  *df_x = ptrs__nodes_coord_alloc->ptrs_nodes_coord [i_node_coord_no].x ;
  *df_y = ptrs__nodes_coord_alloc->ptrs_nodes_coord [i_node_coord_no].y ;
}

static T_PTR_Vertex
get_vertex (T_PTR_Area ptrs_area, int i_vertex_no)
/*-----------------------------------------------*/
{
  return &ptrs_area->s_vertexs_alloc.ptrs_vertexs [i_vertex_no] ;
}

static int get_vertex_no (T_PTR_Area ptrs_area)
{
    return ptrs_area->s_vertexs_alloc.i_size;
}

/*funkcja dodaje parametry segmentu do odpowiednich lokalnych tablic obszaru*/
static BOOL
add_area_seg (int i_object_no,
	      int i_node1_no,
	      int i_node2_no,
	      BOOL b_right_dir,
	      int i_vertex1_no,
	      int i_vertex2_no,
	      double *df_angle ,
	      T_PTR_Area ptrs_area)
/*--------------------------------*/
{
  T_PTR_Nodes_Object ptrs_nodes_object ;
  T_PTR_Node ptrs_nodes ;
  T_PTR_ALine_Alloc ptrs_lines_alloc ;
  T_PTR_AArc_Alloc ptrs_arcs_alloc ;
  T_PTR_Vertex ptrs_vertex1, ptrs_vertex2 ;
  int i_size ;
  double df_x, df_y, df_r ;
  double df_angle1, df_angle2 ;
  double df_x1, df_x2, df_y1, df_y2 ;
  NAGLOWEK *nag ;

  ptrs_nodes_object = &ptrs__nodes_table [i_object_no] ;
  ptrs_nodes = ptrs_nodes_object->s_nodes_alloc.ptrs_nodes_object ;
  if (TRUE == b_right_dir)
  {
    ptrs_nodes [i_node1_no].use_r = 1 ;
    if (FALSE == add_area_trace (ptrs_area,i_object_no, i_node1_no, b_right_dir))
    {
      return FALSE ;
    }
  }
  else
  {
    ptrs_nodes [i_node2_no].use_l = 1 ;
    if (FALSE == add_area_trace (ptrs_area,i_object_no, i_node2_no, b_right_dir))
    {
      return FALSE ;
    }
  }
  nag = (NAGLOWEK *)(dane + ptrs_nodes_object->off_object) ;
  switch (nag->obiekt)
  {
    case Olinia :
    case Otekst :
    case Owwielokat :
      ptrs_lines_alloc = &ptrs_area->s_lines_alloc ;
      if (FALSE == h_realloc ((T_PTR_Units)ptrs_lines_alloc))
      {
	return FALSE ;
      }
      i_size = ptrs_lines_alloc->i_size ;
      ptrs_lines_alloc->i_size++ ;
      ptrs_lines_alloc->ptrs_lines [i_size].vertex1_no = i_vertex1_no ;
      ptrs_lines_alloc->ptrs_lines [i_size].vertex2_no = i_vertex2_no ;
      ptrs_lines_alloc->ptrs_lines [i_size].b_hole = FALSE ;
      ptrs_vertex1 = get_vertex (ptrs_area, i_vertex1_no) ;
      ptrs_vertex2 = get_vertex (ptrs_area, i_vertex2_no) ;
      df_x1 = ptrs_vertex1->x ;
      df_y1 = ptrs_vertex1->y ;
      df_x2 = ptrs_vertex2->x ;
      df_y2 = ptrs_vertex2->y ;
      *df_angle = Angle_Normal (atan2_my  (df_y2 - df_y1, df_x2 - df_x1)) ;
      break ;
    case Oluk :
    case Ookrag :
    case Okolo :
      if (nag->obiekt == Oluk)
      {
	 LUK *ptrs_arc ;

	 ptrs_arc = (LUK*)nag ;
	 df_r = ptrs_arc->r ;
	 df_x = ptrs_arc->x ;
	 df_y = ptrs_arc->y ;
      }
      else
      {
	OKRAG *ptrs_circle ;

	ptrs_circle = (OKRAG*)nag ;
	df_r = ptrs_circle->r ;
	df_x = ptrs_circle->x ;
	df_y = ptrs_circle->y ;
      }
      ptrs_arcs_alloc = &ptrs_area->s_arcs_alloc ;
      if (FALSE == h_realloc ((T_PTR_Units)ptrs_arcs_alloc))
      {
	return FALSE ;
      }
      i_size = ptrs_arcs_alloc->i_size ;
      ptrs_arcs_alloc->i_size++ ;
      ptrs_arcs_alloc->ptrs_arcs [i_size].vertex1_no = i_vertex1_no ;
      ptrs_arcs_alloc->ptrs_arcs [i_size].vertex2_no = i_vertex2_no ;
      ptrs_arcs_alloc->ptrs_arcs [i_size].b_dir = b_right_dir ;
      ptrs_arcs_alloc->ptrs_arcs [i_size].b_hole = FALSE ;
      ptrs_arcs_alloc->ptrs_arcs [i_size].r = df_r ;
      ptrs_arcs_alloc->ptrs_arcs [i_size].x = df_x ;
      ptrs_arcs_alloc->ptrs_arcs [i_size].y = df_y ;
      df_angle1 = ptrs_nodes [i_node1_no].df_t ;
      df_angle2 = ptrs_nodes [i_node2_no].df_t ;
      if (FALSE == b_right_dir)
      {
	df_angle1 = ptrs_nodes [i_node2_no].df_t ;
	df_angle2 = ptrs_nodes [i_node1_no].df_t ;
      }
      ptrs_arcs_alloc->ptrs_arcs [i_size].a1 = df_angle1 ;
      ptrs_arcs_alloc->ptrs_arcs [i_size].a2 = df_angle2 ;
      ptrs_vertex1 = get_vertex (ptrs_area, i_vertex1_no) ;
      ptrs_vertex2 = get_vertex (ptrs_area, i_vertex2_no) ;
      df_x1 = ptrs_vertex1->x ;
      df_y1 = ptrs_vertex1->y ;
      df_x2 = ptrs_vertex2->x ;
      df_y2 = ptrs_vertex2->y ;
      Get_Tangent_Ob (nag, df_x2, df_y2, &df_x1, &df_y1, !b_right_dir) ;
      *df_angle = Angle_Normal (atan2_my  (df_y2 - df_y1, df_x2 - df_x1)) ;
      break ;
    default :
      break ;
  }
  return TRUE ;
}

/*funkcja dodaje kolejne wspolrzedne do lokalnej tablicy wspol. wierzcholkow*/
/*obszaru*/
static int add_vertex (T_PTR_Area ptrs_area, int i_node_no)
/*-------------------------------------------------------*/
{
  T_PTR_Vertex_Alloc ptrs_vertex_alloc ;
  double df_x, df_y ;
  int i_size ;

  ptrs_vertex_alloc = &ptrs_area->s_vertexs_alloc ;
  if (FALSE == h_realloc ((T_PTR_Units)ptrs_vertex_alloc))
  {
    return -1 ;
  }
  i_size = ptrs_vertex_alloc->i_size ;
  ptrs_vertex_alloc->i_size++ ;
  get_global_node_coord (i_node_no, &df_x, &df_y) ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].x = df_x ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].y = df_y ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].vertex_no = i_size ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].b_single = TRUE ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].b_equel = FALSE ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].b_multi = FALSE ;
  return i_size ;
}

int add_vertex (double df_x, double df_y)
/*--------------------------------------*/
{
  T_PTR_Vertex_Alloc ptrs_vertex_alloc ;
  int i_size ;

  ptrs_vertex_alloc = &ptrs__area->s_vertexs_alloc ;
  if (FALSE == h_realloc ((T_PTR_Units)ptrs_vertex_alloc))
  {
    return -1 ;
  }
  i_size = ptrs_vertex_alloc->i_size ;
  ptrs_vertex_alloc->i_size++ ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].x = df_x ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].y = df_y ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].vertex_no = i_size ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].b_single = TRUE ;
  ptrs_vertex_alloc->ptrs_vertexs [i_size].b_equel = FALSE ;
  return i_size ;
}



static BOOL add_area_trace (T_PTR_Area 	ptrs_area,
			   int 	      	i_ob_no,
			   int 		i_node_no,
			   BOOL 	b_right)
/*-------------------------------------------------------*/
{
  T_PTR_ATrace_Alloc ptrs_traces_alloc ;
  int i_size ;

  ptrs_traces_alloc = &ptrs_area->s_traces_alloc ;
  if (FALSE == h_realloc ((T_PTR_Units)ptrs_traces_alloc))
  {
    return FALSE ;
  }
  i_size = ptrs_traces_alloc->i_size ;
  ptrs_traces_alloc->i_size++ ;
  ptrs_traces_alloc->ptrs_traces [i_size].i_object_no = i_ob_no ;
  ptrs_traces_alloc->ptrs_traces [i_size].i_node_no = i_node_no ;
  ptrs_traces_alloc->ptrs_traces [i_size].b_right = b_right ;
  return TRUE ;
}


static void erase_area_trace (T_PTR_Area ptrs_area)
/*-----------------------------------------------*/
{
  int i, i_size ;
  int i_ob, i_node ;
  T_PTR_ATrace ptrs_traces ;
  T_PTR_Nodes_Object ptrs_nodes_object ;
  T_PTR_Node ptrs_nodes ;

  i_size = ptrs_area->s_traces_alloc.i_size ;
  ptrs_traces = ptrs_area->s_traces_alloc.ptrs_traces ;
  for (i = 0 ; i < i_size ; i++)
  {
    i_ob = ptrs_traces [i].i_object_no ;
    i_node = ptrs_traces [i].i_node_no ;
    ptrs_nodes_object = &ptrs__nodes_table [i_ob] ;
    ptrs_nodes = ptrs_nodes_object->s_nodes_alloc.ptrs_nodes_object ;
    if (TRUE == ptrs_traces [i].b_right)
    {
      ptrs_nodes [i_node].use_r = 0 ;
    }
    else
    {
      ptrs_nodes [i_node].use_l = 0 ;
    }
  }
}


static BOOL
make_area_seg (int  i_object_no,
	       int  i_node1_no,
	       int  i_node2_no,
	       BOOL b_right_dir)
/*-------------------------------*/
{
  T_PTR_Nodes_Object ptrs_nodes_object ;
  T_PTR_Area ptrs_area ;
  T_PTR_Node ptrs_node ;
  int i_vertex1_no, i_vertex2_no ;
  double df_angle ;

  if (NULL == (ptrs_area = ini_area_struc ()))
  {
    return FALSE ;
  }

  ptrs_nodes_object = &ptrs__nodes_table [i_object_no] ;
  ptrs_node = &ptrs_nodes_object->s_nodes_alloc.ptrs_nodes_object [i_node1_no] ;
  if (-1 ==  (i_vertex1_no = add_vertex (ptrs_area, ptrs_node->node_no)))
  {
    return FALSE ;
  }

  ptrs_node = &ptrs_nodes_object->s_nodes_alloc.ptrs_nodes_object [i_node2_no] ;
  if (-1 ==  (i_vertex2_no = add_vertex (ptrs_area, ptrs_node->node_no)))
  {
    return FALSE ;
  }

  if (FALSE == (add_area_seg(i_object_no,
      i_node1_no, i_node2_no, b_right_dir,
      i_vertex1_no, i_vertex2_no,
      &df_angle,
      ptrs_area)))
  {
      return FALSE;
  }

  if (FALSE == add_area_next_seg (i_object_no,
				  i_node1_no, i_node2_no,
				  i_vertex2_no,
				  df_angle,
				  ptrs_area))
  {
    free_area_mem (--i__areasno) ;
    return FALSE ;
  }

  if (ptrs_area->close == 0)
  {
    free_area_mem (--i__areasno) ;
  }
  else
  {
    if (FALSE == check_area (ptrs_area))
    {
      erase_area_trace (ptrs_area) ;
      free_area_mem (--i__areasno) ;
    }
  }
  return TRUE ;
}


static BOOL make_areas_ob (int i_object_no)
/*----------------------------------------------------*/
{
  T_PTR_Nodes_Object ptrs_nodes_object ;
  T_PTR_Node ptrs_nodes ;
  int i, i_nodesno, i_node_next ;
  BOOL b_cicle ;

  b_cicle = FALSE ;
  ptrs_nodes_object = &ptrs__nodes_table [i_object_no] ;
  ptrs_nodes = ptrs_nodes_object->s_nodes_alloc.ptrs_nodes_object ;
  i_nodesno = ptrs_nodes_object->s_nodes_alloc.i_size - 1 ;
  if (Ookrag == ((NAGLOWEK*)(dane + ptrs_nodes_object->off_object))->obiekt ||
      Okolo == ((NAGLOWEK*)(dane + ptrs_nodes_object->off_object))->obiekt ||
      Otekst == ((NAGLOWEK*)(dane + ptrs_nodes_object->off_object))->obiekt ||
      Owwielokat == ((NAGLOWEK*)(dane + ptrs_nodes_object->off_object))->obiekt)
  {
    b_cicle = TRUE ;
    i_nodesno++ ;
  }
  for (i = 0 ; i < i_nodesno ; i++)
  {
    i_node_next = i + 1 ;
    if (TRUE == b_cicle && i_node_next ==i_nodesno)
    {
      i_node_next = 0 ;
    }
    if (ptrs_nodes [i].use_r == 0)
    {
      if (FALSE == make_area_seg (i_object_no, i, i_node_next, TRUE))
      {
	return FALSE ;
      }
    }
    if (ptrs_nodes [i].use_l == 0)
    {
      if (FALSE == make_area_seg (i_object_no, i_node_next, i, FALSE))
      {
	return FALSE ;
      }
    }
  }
  return TRUE ;
}


static BOOL make_areas_table (long_long l_offb, long_long l_offe)
/*----------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long l_off ;
  int i_object_no ;
  char sz_tmp [20] ;
  int i_tmp1 = -1, i_tmp ;
  int ret;

  ababab = 0;

  strcpy (sz_tmp, "//") ;
  i_object_no = 0 ;
  i__areasno = 0 ;
  Object_Tok (&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON) ;
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
          if (i_tmp % 10 == 0) {
              ret = snprintf(&sz_tmp[2], 10, "%d", i_tmp);
              InfoListStr(1, sz_tmp);
          }
		i_tmp1 = i_tmp ;
      }
    }
    nag = (NAGLOWEK*)(dane + l_off) ;
    if (nag->atrybut == Ablok)
    {
      if (FALSE == make_areas_ob (i_object_no))
      {
		return FALSE ;
      }
      i_object_no++ ;
    }
    Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON) ;
  }
  return TRUE ;
}

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))


static BOOL check_if_area_sel (T_PTR_Area ptrs_area, double df_x, double df_y)
/*--------------------------------------------------------------------------*/
{
   double df_x1, df_y1, df_x2, df_y2, df_t1, df_t2 ;
   double df_si, df_co, df_ytemp, df_angle ;
   double df_xs, df_ys, df_xe, df_ye ;
   T_PTR_ALine ptrs_lines ;
   T_PTR_AArc ptrs_arcs ;
   LUK s_arc ;
   OKRAG s_circle ;
   LINIA s_line ;
   int k,  k_check ;
   int i_linesno, i_arcsno, i ;
   int i_vertex1, i_vertex2 ;

   ptrs_lines = ptrs_area->s_lines_alloc.ptrs_lines ;
   ptrs_arcs = ptrs_area->s_arcs_alloc.ptrs_arcs ;
   i_linesno = ptrs_area->s_lines_alloc.i_size ;
   i_arcsno = ptrs_area->s_arcs_alloc.i_size ;
   k = 0 ;
   df_ytemp = 0 ;
   k_check = 0 ;
#define TEMP_X 100
#define CHECK 50
   s_line.x1 = df_x ;
   s_line.y1 = df_y ;
   s_line.x2 = df_x + TEMP_X ;
aa:
   if (k_check > CHECK)
   {
     return FALSE ; 	/*punkt jest elementem brzegu obszaru*/
   }
   df_angle = - atan2_my  (df_ytemp , TEMP_X) ;
   df_si = sin (df_angle) ;
   df_co = cos (df_angle) ;
   k = 0 ;
   for (i = 0 ; i < i_linesno ; i++) 	/*dla kazdej linii*/
   {
     i_vertex1 = ptrs_lines [i].vertex1_no ;
     i_vertex2 = ptrs_lines [i].vertex2_no ;
     df_x1 = (get_vertex (ptrs_area, i_vertex1))->x ;
     df_y1 = (get_vertex (ptrs_area, i_vertex1))->y ;
     df_x2 = (get_vertex (ptrs_area, i_vertex2))->x ;
     df_y2 = (get_vertex (ptrs_area, i_vertex2))->y ;
     Rotate_Point (df_si, df_co,
		   df_x, df_y,    			/*center point*/
		   df_x1, df_y1, &df_x1, &df_y1) ;
     Rotate_Point (df_si, df_co,
		   df_x, df_y,    			/*center point*/
		   df_x2, df_y2, &df_x2, &df_y2) ;
     if (TRUE == Check_if_Equal0 (df_y, df_y1) ||  ////WARNING
	 TRUE == Check_if_Equal0 (df_y, df_y2))   ////WARNING
     {
       if ((TRUE == Check_if_Equal0 (df_y, df_y1) &&  ////WARNING
	   TRUE == Check_if_Equal0 (df_x, df_x1))   ||  ////WARNING
	   (TRUE == Check_if_Equal0 (df_y, df_y2) &&  ////WARNING
	   TRUE == Check_if_Equal0 (df_x, df_x2)))  ////WARNING
       {
	 return FALSE ; 	/*punkt jest elementem brzegu obszaru*/
       }
       df_ytemp += 1 ;
       k_check++ ;
       goto aa ;
     }
     if (max (df_x1, df_x2) < df_x)
     {
       continue ;
     }
     if (TRUE == Check_if_LT ((df_y1 - df_y) * (df_y2 - df_y), 0) &&
	 TRUE == Check_if_LT (df_x, df_x1 + (df_y - df_y1) *
			     (df_x2 - df_x1) / (df_y2 - df_y1)))
     {
       k++ ;
     }
   }
   s_line.y2 = df_y + df_ytemp ;
   for (i = 0 ; i < i_arcsno ; i++) 	/*dla kazdego luku*/
   {
      s_circle.r = s_arc.r = ptrs_arcs [i].r ;
     s_circle.x = s_arc.x = ptrs_arcs [i].x ;
     s_circle.y = s_arc.y = ptrs_arcs [i].y ;
     s_arc.kat1 = ptrs_arcs [i].a1 ;
     s_arc.kat2 = ptrs_arcs [i].a2 ;
     if (0 == przeciecieLO_tt (&df_x1, &df_y1, &df_t1,
		    &df_x2, &df_y2, &df_t2,
		    &s_line, &s_circle))
     {
       continue ;   		/*brak przeciecia*/
     }
     if (FALSE == Point_Arc (df_x1 , df_y1 , &s_arc))
     {
       df_t1 = -1 ;
     }
     if (FALSE == Point_Arc (df_x2 , df_y2 , &s_arc))
     {
       df_t2 = -1 ;
     }
     if (TRUE == Check_if_Equal0 (df_t1, 0)  ||  ////WARNING
	 TRUE == Check_if_Equal0 (df_t2, 0)) ////WARNING
     {
	 return FALSE ; 	/*punkt jest elementem brzegu obszaru*/
     }
     Arc_To_Points (&s_arc, &df_xs, &df_ys, &df_xe, &df_ye) ;
     if (TRUE == Check_if_GT (df_t1, 0))
     {
       if ((TRUE == Check_if_Equal0 (df_ys, df_y1) &&  ////WARNING
	   TRUE == Check_if_Equal0 (df_xs, df_x1))   ||  ////WARNING
	   (TRUE == Check_if_Equal0 (df_ye, df_y1) && ////WARNING
	   TRUE == Check_if_Equal0 (df_xe, df_x1))) ////WARNING
       {
	 df_ytemp += 1 ;	/*polprosta przechodzi przez koniec luku*/
	 k_check++ ;
	 goto aa ;
       }
       k++ ;
     }
     if (TRUE == Check_if_GT (df_t2, 0.0))
     {
       if ((TRUE == Check_if_Equal0 (df_ys, df_y2) &&  ////WARNING
	   TRUE == Check_if_Equal0 (df_xs, df_x2))   || ////WARNING
	   (TRUE == Check_if_Equal0 (df_ye, df_y2) && ////WARNING
	   TRUE == Check_if_Equal0 (df_xe, df_x2))) ////WARNING
       {
	 df_ytemp += 1 ;	/*polprosta przechodzi przez koniec luku*/
	 k_check++ ;
	 goto aa ;
       }
       k++ ;
     }
   }
   return (k % 2 == 1) ? TRUE : FALSE ;
}

static BOOL
check_vertex0 (int  i_vertex_no,
	       int  i_vertex1_no,
	       int  i_vertex2_no,
	       int  *ptri_vertex_find_no)
/*------------------------------------------*/
{
  BOOL b_ret ;

  b_ret = FALSE ;
  if (i_vertex1_no == i_vertex_no)
  {
    *ptri_vertex_find_no = i_vertex2_no ;
    b_ret = TRUE ;
  }
  else
  if (i_vertex2_no == i_vertex_no)
  {
    *ptri_vertex_find_no = i_vertex1_no ;
    b_ret = TRUE ;
  }
  return b_ret ;
}


static BOOL
find_corner (int 	     i_vertex_no,
	     T_PTR_Trapizium ptrs_trap,
	     int 	     *i_ver,
	     double 	     *df_a,
	     T_PTR_Area      ptrs_area)
/*-----------------------------------------*/
{
  T_PTR_ALine ptrs_lines ;
  T_PTR_AArc ptrs_arcs ;
  LUK s_arc ;
  int i_linesno, i_arcsno ;
  int i, i_find ;
  double df_x1, df_y1, df_x0, df_y0 ;
  BOOL b_dir ;
  int i_vertex_find_no ;

  ptrs_lines = ptrs_area->s_lines_alloc.ptrs_lines ;
  i_linesno = ptrs_area->s_lines_alloc.i_size ;
  ptrs_arcs = ptrs_area->s_arcs_alloc.ptrs_arcs ;
  i_arcsno = ptrs_area->s_arcs_alloc.i_size ;
  df_x0 = get_vertex (ptrs_area, i_vertex_no)->x ;
  df_y0 = get_vertex (ptrs_area, i_vertex_no)->y ;
  i = 0 ;
  i_find = 0 ;
  while (i < i_linesno && i_find < 2)
  {
    if (TRUE == check_vertex0 (i_vertex_no,
		   ptrs_lines [i].vertex1_no,
		   ptrs_lines [i].vertex2_no,
		   &i_vertex_find_no))
    {
      i_ver [i_find] = i_vertex_find_no ;
      df_x1 = get_vertex (ptrs_area, i_vertex_find_no)->x ;
      df_y1 = get_vertex (ptrs_area, i_vertex_find_no)->y ;
      df_a [i_find] = Angle_Normal (atan2_my (df_y1 - df_y0, df_x1 - df_x0)) ;
      if (i_find == 0)
      {
	ptrs_trap->b_line_left = 1 ;
	memcpy (&ptrs_trap->s_line_l, &ptrs_lines [i], sizeof (T_ALine)) ;
      }
      else
      {
	ptrs_trap->b_line_right = 1 ;
	memcpy (&ptrs_trap->s_line_r, &ptrs_lines [i], sizeof (T_ALine)) ;
      }
      i_find++ ;
    }
    i++ ;
  }
  i = 0 ;
  if (i_find < 2)
  {
    while (i < i_arcsno && i_find < 2)
    {
      if (TRUE == check_vertex0 (i_vertex_no,
		     ptrs_arcs [i].vertex1_no,
		     ptrs_arcs [i].vertex2_no,
		     &i_vertex_find_no))
      {
	i_ver [i_find] = i_vertex_find_no ;
	df_x1 = get_vertex (ptrs_area, i_vertex_find_no)->x ;
	df_y1 = get_vertex (ptrs_area, i_vertex_find_no)->y ;
	s_arc.r = ptrs_arcs [i].r ;
	s_arc.x = ptrs_arcs [i].x ;
	s_arc.y = ptrs_arcs [i].y ;
	s_arc.kat1 = ptrs_arcs [i].a1 ;
	s_arc.kat2 = ptrs_arcs [i].a2 ;
	b_dir = ptrs_arcs [i].b_dir ;
	if (ptrs_arcs [i].vertex1_no == i_vertex_find_no)
	{
	  b_dir = !b_dir ;
	}
	Get_Tangent_Ob (&s_arc, df_x0, df_y0, &df_x1, &df_y1, b_dir) ;
	df_a [i_find] = Angle_Normal (atan2_my (df_y1 - df_y0, df_x1 - df_x0)) ;
	if (i_find== 0)
	{
	  ptrs_trap->b_line_left = 0 ;
	  memcpy (&ptrs_trap->s_arc_l, &ptrs_arcs [i], sizeof (T_AArc)) ;
	}
	else
	{
	  ptrs_trap->b_line_right = 0 ;
	  memcpy (&ptrs_trap->s_arc_r, &ptrs_arcs [i], sizeof (T_AArc)) ;
	}
	i_find++ ;
      }
    i++ ;
    }
  }
  if (i_find < 2)
  {
    Internal_Error (__LINE__,__FILE__);
    return FALSE ;
  }
  return TRUE ;
}


static BOOL check_area (T_PTR_Area ptrs_area)
/*------------------------------------------*/
{
  T_Trapizium s_trap ;
  int i_ver [2] ;
  double df_a [2], df_y, df_ymax ;
  int i_vertexno, i, i_max_ver, i_ver_l1, i_arcsno ;
  BOOL b_ret, b_angle, b_dir ;
  T_PTR_AArc ptrs_arcs ;

  b_ret = FALSE;
  i_max_ver = 0 ;
  i_vertexno = ptrs_area->s_vertexs_alloc.i_size ;
  i_arcsno = ptrs_area->s_arcs_alloc.i_size ;
  ptrs_arcs = ptrs_area->s_arcs_alloc.ptrs_arcs ;
  for (i = 1 ; i < i_vertexno ; i++)
  {
    if (get_vertex (ptrs_area, i_max_ver)->y < get_vertex (ptrs_area, i)->y)
    {
      i_max_ver = i ;
    }
  }
  b_angle = FALSE ;
  df_ymax = get_vertex (ptrs_area, i_max_ver)->y ;
  for (i = 0 ; i < i_arcsno ; i++)
  {
    if (TRUE == Sharp_Check_Angle (Pi2 / 4, ptrs_arcs [i].a1, ptrs_arcs [i].a2))
    {
      df_y = ptrs_arcs [i].y + ptrs_arcs [i].r ;
      if (df_ymax < df_y)
      {
	df_ymax = df_y ;
	b_angle = TRUE ;
	b_dir = ptrs_arcs [i].b_dir ;
      }
    }
  }
  if (b_angle == TRUE)
  {
    return b_dir ;
  }
  if (FALSE == find_corner (i_max_ver, &s_trap, i_ver, df_a, ptrs_area))
  {
    return b_ret ;
  }
  if (TRUE == Check_if_Equal (df_a [0], 0))  ////WARNING
  {
    df_a [0] += Pi2 ;
  }
  if (TRUE == Check_if_Equal (df_a [1], 0))  ////WARNING
  {
    df_a [1] += Pi2 ;
  }
  if (df_a [0] > df_a [1])
  {
    if (s_trap.b_line_right == 1)
    {
       i_ver_l1 = s_trap.s_line_r.vertex1_no ;
    }
    else
    {
       i_ver_l1 = s_trap.s_arc_r.vertex1_no ;
    }
  }
  else
  {
    if (s_trap.b_line_left == 1)
    {
       i_ver_l1 = s_trap.s_line_l.vertex1_no ;
    }
    else
    {
       i_ver_l1 = s_trap.s_arc_l.vertex1_no ;
    }
  }
  if (i_ver_l1 == i_max_ver)
  {
    b_ret = TRUE ;
  }
  return b_ret ;
}


static BOOL select_area (double df_pointx, double df_pointy)
/*----------------------------------------------------------*/
{
  BOOL b_sel ;
  int i_sel, i ;
  double df_x, df_y ;

  b_sel = FALSE ;
  for (i = 0 ; i < i__areasno ; i++)	/*wyznaczenie obszarow zaznaczonych*/
  {
    ptrs__areas [i].sel = AST_NOSEL ;
    if (TRUE == check_if_area_sel (&ptrs__areas [i], df_pointx, df_pointy))
    {
      b_sel = TRUE ;
      ptrs__areas [i].sel = AST_SEL ;
    }
  }
  if (b_sel == FALSE)
  {
    return FALSE ;
  }
  b_sel = FALSE ;
  for (i = 0 ; i < i__areasno ; i++) 	/*wyznaczenie najmniejszego obszaru zaznaczonego*/
  {
    if (ptrs__areas [i].sel == AST_SEL)
    {
      if (b_sel == FALSE)
      {
	i_sel = i ;
	b_sel = TRUE ;
      }
      else
      {
	df_x = (get_vertex (&ptrs__areas [i], 0))->x ;   ////(A)
	df_y = (get_vertex (&ptrs__areas [i], 0))->y ;
	if (TRUE == check_if_area_sel (&ptrs__areas [i_sel], df_x, df_y))
	{
	  i_sel = i ;
	}
      }
    }
  }
  ptrs__areas [i_sel].sel = AST_MINSEL ;
  i__area_sel = i_sel ;
  for (i = 0 ; i < i__areasno ; i++)	/*wyznaczenie dziur*/
  {
    ptrs__areas [i].hole = 0 ;
    if (ptrs__areas [i].sel == AST_NOSEL && i != i__area_sel)
    {
      ////instead of this
      //df_x = (get_vertex (&ptrs__areas [i], 0))->x ;  ////(B)
      //df_y = (get_vertex (&ptrs__areas [i], 0))->y ;

      ////is this (21-03-2025) to point into the middle of vertices; possible the same should be done above (A)
      int no_of_vertices=get_vertex_no (&ptrs__areas [i]);

      if (no_of_vertices>0)
      {
          df_x = 0;
          df_y = 0;
          for (int ii = 0; ii < no_of_vertices; ii++)
          {
              df_x += (get_vertex(&ptrs__areas[i], ii))->x;
              df_y += (get_vertex(&ptrs__areas[i], ii))->y;
          }
          df_x /= no_of_vertices;
          df_y /= no_of_vertices;

          if (TRUE == check_if_area_sel(&ptrs__areas[i__area_sel], df_x, df_y))
          {
              ptrs__areas[i].hole = 1;
          }
      }
      ////
    }
  }
  for (i = i__areasno - 1 ; i >= 0  ; i--) 	/*zwolnienie pamieci zbednych obszarow*/
  {
    if (ptrs__areas [i].hole == 0 && i != i__area_sel)
    {
      free_area_mem (i) ;
    }
  }
  return TRUE ;
}


static BOOL get_mem_areas_table (void)
/*----------------------------------*/
{
  long_long l_free ;
  BOOL b_ret ;

  b_ret = FALSE ;
  l_free = get_free_mem_size (1) ;
  i__areasno_max = (int)((l_free - 10000) / sizeof (T_Area)) ;    /*1000 - rezerwa */
  if (i__areasno_max > 10)
  {
    if (i__areasno_max > 10000) i__areasno_max = 10000;
	ptrs__areas = (T_Area*)malloc(i__areasno_max * sizeof(T_Area));  //alternative
    b_ret = TRUE ;
  }
  else
  {
    ErrList (18) ;
  }
  return b_ret ;
}

static void free_nodes_mem (void)
/*-------------------------------*/
{
  int i ;

  h_free (ptrs__nodes_coord_alloc->ptrs_nodes_coord) ;
  if (NULL != ptrs__nodes_table)
  {
    for (i = 0 ; i < i__objects_no ; i++)
    {
      h_free (ptrs__nodes_table [i].s_nodes_alloc.ptrs_nodes_object) ;
    }
  }
  h_free (ptrs__nodes_table) ;
}

static BOOL compare_area_object (void)
/*----------------------------------*/
{
  T_PTR_ALine ptrs_lines ;
  T_PTR_AArc ptrs_arcs ;
  T_PTR_Vertex ptrs_vertexs ;
  int i, j, i_linesno, i_arcsno, i_ver1i, i_ver2i, i_ver1j, i_ver2j ;
  double df_x1i, df_y1i, df_x2i, df_y2i, df_x1j, df_y1j, df_x2j, df_y2j ;
  double df_xi, df_yi, df_ri, df_a1i, df_a2i ;
  double df_xj, df_yj, df_rj, df_a1j, df_a2j ;
  char sz_tmp [20] ;
  int i_tmp1 = -1, i_tmp ;
  int ret;

  i_linesno = ptrs__area->s_lines_alloc.i_size ;
  i_arcsno = ptrs__area->s_arcs_alloc.i_size ;
  ptrs_lines = ptrs__area->s_lines_alloc.ptrs_lines ;
  ptrs_arcs = ptrs__area->s_arcs_alloc.ptrs_arcs ;
  ptrs_vertexs = ptrs__area->s_vertexs_alloc.ptrs_vertexs ;
  strcpy (sz_tmp, "///") ;

  for (i = 0 ; i < i_linesno - 1 ; i++)
  {
    if (TRUE == Check_Break_Key ())
    {
      return FALSE ;
    }
    if (i % 10 == 0) //5
    {
      i_tmp = (int)(i * 100L / (i_linesno + i_arcsno + 1)) ;
      if (i_tmp != i_tmp1)
      {
          if (i_tmp % 10 == 0) {
              ret = snprintf(&sz_tmp[3], 10, "%d", i_tmp);
              InfoListStr(1, sz_tmp);
          }
	   i_tmp1 = i_tmp ;
      }
    }
    if (ptrs_lines [i].b_hole == FALSE)
    {
      continue ;
    }
    i_ver1i = ptrs_lines [i].vertex1_no ;
    i_ver2i = ptrs_lines [i].vertex2_no ;
    df_x1i = ptrs_vertexs [i_ver1i].x ;
    df_y1i = ptrs_vertexs [i_ver1i].y ;
    df_x2i = ptrs_vertexs [i_ver2i].x ;
    df_y2i = ptrs_vertexs [i_ver2i].y ;
    for (j = i + 1 ; j < i_linesno ; j++)
    {
      if (ptrs_lines [j].b_hole == FALSE)
      {
	continue ;
      }
      i_ver1j = ptrs_lines [j].vertex1_no ;
      i_ver2j = ptrs_lines [j].vertex2_no ;
      df_x1j = ptrs_vertexs [i_ver1j].x ;
      df_y1j = ptrs_vertexs [i_ver1j].y ;
      df_x2j = ptrs_vertexs [i_ver2j].x ;
      df_y2j = ptrs_vertexs [i_ver2j].y ;
      if ((TRUE == Check_if_Equal0 (df_x1i, df_x1j) &&  ////WARNING
	  TRUE == Check_if_Equal0 (df_y1i, df_y1j) &&
	  TRUE == Check_if_Equal0 (df_x2i, df_x2j) &&
	  TRUE == Check_if_Equal0 (df_y2i, df_y2j))  ||
	 (TRUE == Check_if_Equal0 (df_x1i, df_x2j) &&
	  TRUE == Check_if_Equal0 (df_y1i, df_y2j) &&
	  TRUE == Check_if_Equal0 (df_x2i, df_x1j) &&
	  TRUE == Check_if_Equal0 (df_y2i, df_y1j)))
      {
	if (j < i_linesno - 1)
	{
	  memcpy (&ptrs_lines [j], &ptrs_lines [i_linesno - 1], sizeof (T_ALine)) ;
	}
	i_linesno-- ;
	if (i < i_linesno - 1)
	{
	  memcpy (&ptrs_lines [i], &ptrs_lines [i_linesno - 1], sizeof (T_ALine)) ;
	}
	i_linesno-- ;
	i-- ;
	ptrs_vertexs [i_ver1j].b_multi = TRUE ;
	ptrs_vertexs [i_ver2j].b_multi = TRUE ;
	ptrs_vertexs [i_ver1i].b_multi = TRUE ;
	ptrs_vertexs [i_ver2i].b_multi = TRUE ;
	break ;
      }
    }
  }
  ptrs__area->s_lines_alloc.i_size = i_linesno ;
  for (i = 0 ; i < i_arcsno - 1 ; i++)
  {
    if (TRUE == Check_Break_Key ())
    {
      return FALSE ;
    }
    if (i % 10 == 0) //5
    {
      i_tmp = (int)((i + i_linesno) * 100L / (i_linesno + i_arcsno + 1)) ;
      if (i_tmp != i_tmp1)
      {
          if (i_tmp % 10 == 0) {
              ret = snprintf(&sz_tmp[3], 10, "%d", i_tmp);
              InfoListStr(1, sz_tmp);
          }
	  i_tmp1 = i_tmp ;
      }
    }
    if (ptrs_arcs [i].b_hole == FALSE)
    {
      continue ;
    }
    df_xi = ptrs_arcs [i].x ;
    df_yi = ptrs_arcs [i].y ;
    df_ri = ptrs_arcs [i].r ;
    df_a1i = ptrs_arcs [i].a1 ;
    df_a2i = ptrs_arcs [i].a2 ;
    for (j = i + 1 ; j < i_arcsno ; j++)
    {
      if (ptrs_arcs [j].b_hole == FALSE)
      {
	continue ;
      }
      df_xj = ptrs_arcs [j].x ;
      df_yj = ptrs_arcs [j].y ;
      df_rj = ptrs_arcs [j].r ;
      df_a1j = ptrs_arcs [j].a1 ;
      df_a2j = ptrs_arcs [j].a2 ;
      if (TRUE == Check_if_Equal0 (df_xi, df_xj) &&  ////WARNING
	  TRUE == Check_if_Equal0 (df_yi, df_yj) &&
	  TRUE == Check_if_Equal0 (df_a1i, df_a1j) &&
	  TRUE == Check_if_Equal0 (df_a2i, df_a2j) &&
	  TRUE == Check_if_Equal0 (df_ri, df_rj))
      {
	if (j < i_arcsno - 1)
	{
	  memcpy (&ptrs_arcs [j], &ptrs_arcs [i_arcsno - 1], sizeof (T_AArc)) ;
	}
	i_arcsno-- ;
	if (i < i_arcsno - 1)
	{
	  memcpy (&ptrs_arcs [i], &ptrs_arcs [i_arcsno - 1], sizeof (T_AArc)) ;
	}
	i_arcsno-- ;
	i-- ;
	ptrs_vertexs [ptrs_arcs [i].vertex1_no].b_multi = TRUE ;
	ptrs_vertexs [ptrs_arcs [i].vertex2_no].b_multi = TRUE ;
	ptrs_vertexs [ptrs_arcs [j].vertex1_no].b_multi = TRUE ;
	ptrs_vertexs [ptrs_arcs [j].vertex2_no].b_multi = TRUE ;
	break ;
      }
    }
  }
  ptrs__area->s_arcs_alloc.i_size = i_arcsno ;
  return TRUE ;
}


static BOOL condense_areas (void)
/*------------------------------*/
{
  int i, j, i_ver_sum, i_lin_sum, i_arc_sum ;
  int i_ver_sum0, i_lin_sum0, i_arc_sum0 ;
  T_PTR_Vertex ptrs_vertexs ;
  T_PTR_ALine ptrs_lines ;
  T_PTR_AArc ptrs_arcs ;
  T_PTR_ALine_Alloc ptrs_lines_alloc ;
  T_PTR_AArc_Alloc ptrs_arcs_alloc ;
  T_PTR_Vertex_Alloc ptrs_vertexs_alloc ;
  BOOL b_hole ;

  if (NULL == ini_area_struc (ptrs__area))
  {
    return FALSE ;
  }

  i_ver_sum0 = 0 ;
  i_lin_sum0 = 0 ;
  i_arc_sum0 = 0 ;
  for (i = 0 ; i < i__areasno ; i++)
  {
    if (ptrs__areas [i].hole == 1 || i == i__area_sel)
    {
      i_ver_sum0 += ptrs__areas [i].s_vertexs_alloc.i_size ;
      i_lin_sum0 += ptrs__areas [i].s_lines_alloc.i_size ;
      i_arc_sum0 += ptrs__areas [i].s_arcs_alloc.i_size ;
    }
  }
  if (FALSE == h_realloc ((T_PTR_Units)&ptrs__area->s_vertexs_alloc, i_ver_sum0) ||
      FALSE == h_realloc ((T_PTR_Units)&ptrs__area->s_lines_alloc, i_lin_sum0) ||
      FALSE == h_realloc ((T_PTR_Units)&ptrs__area->s_arcs_alloc, i_arc_sum0))
  {
    free_area_mem () ;
    return FALSE ;
  }

  ptrs_vertexs = ptrs__area->s_vertexs_alloc.ptrs_vertexs ;
  ptrs_lines = ptrs__area->s_lines_alloc.ptrs_lines ;
  ptrs_arcs = ptrs__area->s_arcs_alloc.ptrs_arcs ;
  i_ver_sum = 0 ;
  i_lin_sum = 0 ;
  i_arc_sum = 0 ;
  ptrs__area->s_vertexs_alloc.i_size = i_ver_sum0 ;
  ptrs__area->s_lines_alloc.i_size = i_lin_sum0 ;
  ptrs__area->s_arcs_alloc.i_size = i_arc_sum0 ;
  for (i = 0 ; i < i__areasno ; i++)
  {
    if (ptrs__areas [i].hole == 1 || i == i__area_sel)
    {
      b_hole = (ptrs__areas [i].hole == 1) ? TRUE : FALSE ;
      ptrs_vertexs_alloc = &ptrs__areas [i].s_vertexs_alloc ;
      ptrs_lines_alloc = &ptrs__areas [i].s_lines_alloc ;
      ptrs_arcs_alloc = &ptrs__areas [i].s_arcs_alloc ;
      for (j = 0 ; j < ptrs_vertexs_alloc->i_size ; j++)
      {
	ptrs_vertexs_alloc->ptrs_vertexs [j].vertex_no += i_ver_sum ;
      }
      for (j = 0 ; j < ptrs_lines_alloc->i_size ; j++)
      {
	ptrs_lines_alloc->ptrs_lines [j].vertex1_no += i_ver_sum ;
	ptrs_lines_alloc->ptrs_lines [j].vertex2_no += i_ver_sum ;
	ptrs_lines_alloc->ptrs_lines [j].b_hole = b_hole ;
      }
      for (j = 0 ; j < ptrs_arcs_alloc->i_size ; j++)
      {
	ptrs_arcs_alloc->ptrs_arcs [j].vertex1_no += i_ver_sum ;
	ptrs_arcs_alloc->ptrs_arcs [j].vertex2_no += i_ver_sum ;
	ptrs_arcs_alloc->ptrs_arcs [j].b_hole = b_hole ;
      }
      memcpy (&ptrs_vertexs [i_ver_sum],
	      ptrs_vertexs_alloc->ptrs_vertexs,
	      ptrs_vertexs_alloc->i_size * sizeof(T_Vertex)) ;
      memcpy (&ptrs_lines [i_lin_sum],
	      ptrs_lines_alloc->ptrs_lines,
	      ptrs_lines_alloc->i_size * sizeof(T_ALine)) ;
      memcpy (&ptrs_arcs [i_arc_sum],
	      ptrs_arcs_alloc->ptrs_arcs,
	      ptrs_arcs_alloc->i_size * sizeof(T_AArc)) ;
      i_ver_sum += ptrs_vertexs_alloc->i_size ;
      i_lin_sum += ptrs_lines_alloc->i_size ;
      i_arc_sum += ptrs_arcs_alloc->i_size ;
    }
  }


  for (i = i__areasno - 1 ; i >= 0  ; i--) 	/*zwolnienie pamieci zbednych obszarow*/
  {
    if (ptrs__areas [i].hole == 1 || i == i__area_sel)
    {
      free_area_mem (i) ;
    }
  }
  i__areasno = 1 ;

  return TRUE ;
}


int Make_areas (long_long l_offb, long_long l_offe,
		 int seeds_no, double df_pointx[], double df_pointy[],
         double *df_pointx_, double *df_pointy_,
		 int i_objects_no,
		 T_PTR_Node_Coord_Alloc ptrs_nodes_coord_alloc,
		 T_PTR_Nodes_Object ptrs_nodes_table,
		 T_PTR_Area ptrs_area)
/*---------------------------------------------*/
{
  BOOL b_ret ;
  int i_seed;

  i__objects_no = i_objects_no ;
  ptrs__nodes_coord_alloc = ptrs_nodes_coord_alloc ;
  ptrs__nodes_table = ptrs_nodes_table ;
  ptrs__area = ptrs_area ;

  ptrs__areas = NULL ;	/*do pozniejszego zwolnienia*/
  if (FALSE == get_mem_areas_table ())
  {
    free_nodes_mem () ;
    return 0 ;
  }

  if (FALSE == make_areas_table (l_offb, l_offe))
  {
    free_areas_mem () ;
    //free_mem () ; 
	free(ptrs__areas);  //alternative
    free_nodes_mem () ;
    return 0 ;
  }

  
  free_nodes_mem () ;
  if (0 == i__areasno)
  {
     ErrList (87) ;
	free(ptrs__areas);
    return 0 ;
  }

  b_ret=FALSE;
  for (i_seed=0; i_seed<seeds_no; i_seed++) {
      b_ret = select_area(df_pointx[i_seed], df_pointy[i_seed]);
      if (b_ret==TRUE)   break;
  }
  if (b_ret==FALSE)
  {
      ErrList(88);
      free_areas_mem();
      free(ptrs__areas);
      return -1;
  }
  else
  {
      *df_pointx_=df_pointx[i_seed];
      *df_pointy_=df_pointy[i_seed];
  }


  if (FALSE == condense_areas ())
  {
    free_areas_mem () ;
	free(ptrs__areas);
    return 0 ;
  }

  b_ret = compare_area_object () ;
  free(ptrs__areas);


  if (b_ret == FALSE)
  {
    free_area_mem () ;
  }
  return b_ret ;
}

BOOL Test_areas (long_long l_offb, long_long l_offe,
		 double df_pointx, double df_pointy,
		 int i_objects_no,
		 T_PTR_Node_Coord_Alloc ptrs_nodes_coord_alloc,
		 T_PTR_Nodes_Object ptrs_nodes_table,
		 T_PTR_Area ptrs_area)
/*---------------------------------------------*/
{
  BOOL b_ret ;
  i__objects_no = i_objects_no ;
  ptrs__nodes_coord_alloc = ptrs_nodes_coord_alloc ;
  ptrs__nodes_table = ptrs_nodes_table ;
  ptrs__area = ptrs_area ;

  ptrs__areas = NULL ;	/*do pozniejszego zwolnienia*/
  if (FALSE == get_mem_areas_table ())
  {
    free_nodes_mem () ;
    return FALSE ;
  }
  if (FALSE == make_areas_table (l_offb, l_offe))
  {
    free_areas_mem () ;
    free(ptrs__areas);  //alternative
    free_nodes_mem () ;
    return FALSE ;
  }
  free_nodes_mem () ;
  if (0 == i__areasno)
  {
    free(ptrs__areas);
    return FALSE ;
  }
  if (FALSE == select_area (df_pointx, df_pointy))
  {
    free_areas_mem () ;
    free(ptrs__areas);
    return FALSE ;
  }
  if (FALSE == condense_areas())
  {
	  free_areas_mem();
      free(ptrs__areas);
	  return FALSE;
  }
  b_ret = compare_area_object();
  free(ptrs__areas);

  if (b_ret == FALSE)
  {
	  free_area_mem();
  }
 return TRUE;  //obszar jest zamkniety i punkt lezy w tym obszarze
}
