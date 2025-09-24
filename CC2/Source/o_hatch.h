#define DNODE_COORD 10
#define DNODE_OB 2
#define DAREAVERTEX 5
#define DAREALINE 5
#define DAREAARC 2
#define DTRAPOPEN 3
#define PATERN_NAME_MAXLEN 36 //32
enum OBJECT_TYPE {OT_NOOBJECT, OT_LINE, OT_ARC, OT_CIRCLE, OT_TEXT} ;
enum VERTEX_TYPE {VT_UNKNOWN1, VT_REGULAR, VT_stalaktyt, VT_stalagmit } ;
enum AREA_SEL_TYPE {AST_NOSEL, AST_SEL, AST_MINSEL, AST_BAD} ;
enum TRAP_CORNER {TRC_LD ,TRC_LU, TRC_RD, TRC_RU} ;

enum HATCH {
    ID_PATTERN = 0,
    ID_PATTERN_FOLDER = 1,
    ID_ANGLE_ROTATE = 2,
    ID_SCALE = 3,
    ID_SNAP_BASE = 4,
    ID_SNAP_BASE_POINT = 5,
    ID_DIST = 6,
    ID_INSULATION = 7,
    ID_VPATTERN_ANGLE = 9,
    ID_VPATTERN_PARALLEL = 10,
    ID_VPATTERN_PERPENDICULAR = 11,
    ID_IMAGE_PATTERN = 12,
    ID_GRAB_IMAGE_PATTERN = 17,
    ID_IMAGE_PATTERN_SCALE = 18,
    ID_IMAGE_PATTERN_ANGLE = 25,
    ID_IMAGE_PATTERN_DX = 29,
    ID_IMAGE_PATTERN_DY = 39,
    ID_TRANSLUCENCY = 561,
    ID_END = 49
};



typedef struct
{
  double x, y ;			/*wspolrzedne wezla*/
}
T_Node_Coord, *T_PTR_Node_Coord ;


typedef struct
{
  T_PTR_Node_Coord ptrs_nodes_coord ;/*adres tablicy wezlow objectow*/
  int i_size ;			/*rozmiar tablicy*/
  int i_size_alloc ;		/*aktualna alokacja*/
  int i_unit_size ;		/*rozmiar jednostki*/
  int i_d_alloc ;               /*wielkosc dealokacji*/
}
T_Node_Coord_Alloc , *T_PTR_Node_Coord_Alloc ;


typedef struct
{
  double df_t ;   		/*dla odcinka [0,1], dla luku [0,2*Pi] */
  unsigned node_no ;  		/*numer wezla*/
  unsigned use_r       : 1 ;  	/*sprawdzony w kierunku zgodnym z kierunkiem
				linii lub luku, przeciwnie do kierunk zegara
				dla okregu*/
  unsigned use_l       : 1 ;  	/*sprawdzony w kierunku przeciwnym do kierunku
				  linii lub luku*/
  unsigned reserve     : 14 ;
}
T_Node, *T_PTR_Node ;
/*T_Node wyznacza wezel na obiekcie*/
/*zmienne use_l, use_r dotycza segmentu ktorego wezel jest pierwszym koncem*/

typedef struct
{
  T_PTR_Node ptrs_nodes_object ;/*adres tablicy wezlow obiektu*/
  int i_size ;			/*rozmiar tablicy*/
  int i_size_alloc ;		/*aktualna alokacja*/
  int i_unit_size ;		/*rozmiar jednostki*/
  int i_d_alloc ;               /*wielkosc dealokacji*/
}
T_Nodes_Alloc , *T_PTR_Nodes_Alloc ;

typedef struct
{
  long_long 	  	off_object ;      /*offset obiektu w bazie danych*/
  T_Nodes_Alloc s_nodes_alloc ;   /*alokacja wezlow obiektu*/

}
T_Nodes_Object, * T_PTR_Nodes_Object ;

/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/

typedef struct
{
  double x, y ;			/*wspolrzedne wezla*/
  unsigned vertex_no ;		/*numer wezla*/
  BOOL b_multi ;		/*wezel wspolnej krawedzi dziur*/
  BOOL b_single ;		/*typ wezla : dla dziur mozliwy wspolny wezel*/
  BOOL b_equel ;		/*typ wezla : rowny poprzedniemu*/
}
T_Vertex, *T_PTR_Vertex ;	/*wierzcholek obszaru*/


typedef struct
{
  int vertex1_no ; 		/*numer 1 wierzcholka*/
  int vertex2_no ; 		/*numer 2 wierzcholka*/
  BOOL b_hole ;			/*czy krawedz jest elementem dziury*/
}
T_ALine, *T_PTR_ALine ;		/*krawedz obszaru typu linia*/


typedef struct
{
  int vertex1_no ; 		/*numer 1 wierzcholka*/
  int vertex2_no ; 		/*numer 2 wierzcholka*/
  BOOL b_dir ;			/*TRUE - numeracja wierzcholkow zgodna parametrami luku*/
  BOOL b_hole ;			/*czy krawedz jest elementem dziury*/
  double r, a1, a2, x, y ;	/*parametry luku*/
}
T_AArc, *T_PTR_AArc ;		/*krawedz obszaru typu luk*/

typedef struct
{
  int i_object_no ;		/*numer sprawdzonego obiektu*/
  int i_node_no ;		/*numer wezla*/
  BOOL b_right ;                 /*kierunek sprawdzania*/
}
T_ATrace, *T_PTR_ATrace ;


typedef struct
{
  T_PTR_Vertex ptrs_vertexs ;    /*adres tablicy wierzchlkow obszaru*/
  int i_size ;			/*rozmiar tablicy*/
  int i_size_alloc ;		/*aktualna alokacja*/
  int i_unit_size ;		/*rozmiar jednostki*/
  int i_d_alloc ;               /*wielkosc dealokacji*/
}
T_Vertex_Alloc , *T_PTR_Vertex_Alloc ;

typedef struct
{
  T_PTR_ALine ptrs_lines ;	/*adres tablicy krawedzi typu linia*/
  int i_size ;			/*rozmiar tablicy*/
  int i_size_alloc ;		/*aktualna alokacja*/
  int i_unit_size ;		/*rozmiar jednostki*/
  int i_d_alloc ;               /*wielkosc dealokacji*/
}
T_ALine_Alloc, *T_PTR_ALine_Alloc;

typedef struct
{
  T_PTR_AArc ptrs_arcs ;	/*adres tablicy krawedzi typu luk*/
  int i_size ;			/*rozmiar tablicy*/
  int i_size_alloc ;		/*aktualna alokacja*/
  int i_unit_size ;		/*rozmiar jednostki*/
  int i_d_alloc ;               /*wielkosc dealokacji*/
}
T_AArc_Alloc, *T_PTR_AArc_Alloc;


typedef struct
{
  T_PTR_ATrace ptrs_traces ;	/*adres tablicy zaznaczonych wezlow*/
  int i_size ;			/*rozmiar tablicy*/
  int i_size_alloc ;		/*aktualna alokacja*/
  int i_unit_size ;		/*rozmiar jednostki*/
  int i_d_alloc ;               /*wielkosc dealokacji*/
}
T_ATrace_Alloc, *T_PTR_ATrace_Alloc ;


typedef struct
{
   unsigned sel      : 2 ;	/*czy wskazany*/
   unsigned hole     : 1 ;	/*dziura*/
   unsigned close    : 1 ;	/*czy zamkniety*/
   T_ATrace_Alloc s_traces_alloc ;  /*adres tablicy zaznaczonych wezlow*/
   T_Vertex_Alloc s_vertexs_alloc ;/*adres tablicy wierzcholkow*/
   T_ALine_Alloc s_lines_alloc ;/*adres tablicy krawedzi typu linia*/
   T_AArc_Alloc s_arcs_alloc ;	/*adres tablicy krawedzi typu luk*/
   T_Vertex_Alloc s_vertexs_cpy_alloc ;/*adres tablicy wierzcholkow*/
   T_AArc_Alloc s_arcs_cpy_alloc ;/*adres tablicy krawedzi typu luk*/
}
T_Area, * T_PTR_Area ;

/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/


#define BANK_S	sizeof(T_AArc)
#define MaxNumberSegPattern 12 //24 //6

typedef struct
{
  unsigned b_line_left  : 1 ;	/*lewa krawedz - linia czy luk*/
  unsigned b_line_right : 1 ;	/*prawa krawedz - linia czy luk*/
  int vertex_up_no ; 		/*numer wierzcholka ograniczajacego trapez*/
  int vertex_dn_no ; 		/*z gory i z dolu , (po posortowaniu)*/
  union
  {
    char bank_l [BANK_S] ;	/*pole logiczne*/
    T_ALine s_line_l ;  	/*lewa krawedz trapezu*/
    T_AArc s_arc_l ;   		/*lewa krawedz trapezu*/
				/*vertex1 ma mniejsze y od vertex2*/
  } ;
  union
  {
    char bank_r [BANK_S] ;	/*pole logiczne*/
    T_ALine s_line_r ;  	/*prawa krawedz, trapezu*/
    T_AArc s_arc_r ;     	/*prawa krawedz, trapezu*/
				/*vertex1 ma mniejsze y od vertex2*/
  } ;
}
T_Trapizium, * T_PTR_Trapizium ;


typedef struct
{
  T_PTR_Trapizium ptrs_traps ;	/*adres tablicy */
  int i_size ;			/*rozmiar tablicy*/
  int i_size_alloc ;		/*aktualna alokacja*/
  int i_unit_size ;		/*rozmiar jednostki*/
  int i_d_alloc ;               /*wielkosc dealokacji*/
}
T_Trapizium_Alloc, *T_PTR_Trapizium_Alloc ;

/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/

typedef struct
{
  double df_angle ;	/*kat pochylenia linii*/
  double df_startx, df_starty ;	/*przesuniecie punktu startowego linii (w jednostkach rysunku)*/
  double df_move ;	/*przesuniecie (w jednostkach rysunku), dla linii ciaglej nieistotne*/
  double df_dist ;      /*odleglosc miedzy liniamii(w jednostkach rysunku)*/
  double df_len_pattern ;/*dlugosc wzorca(w jednostkach rysunku)*/
  int i_def_size ;      /*ilosc segmentow definicji, 0 dla linii ciaglej*/
  int reserve;
  double df_def_line [MaxNumberSegPattern] ; /*dlugosci poszczegolnych segmentow (w jednostkach rysunku)*/
}
T_Hatch_Line_Def, *T_PTR_Hatch_Line_Def ;

typedef struct
{
  char sz_name [PATERN_NAME_MAXLEN] ;/*nazwa wzoru wypelnienia*/
  int  i_number_line ;	/*numer w tablicy definicji linii wypelnien*/
  int  i_size ;		/*ilosc linii definiujacych wzor*/
}
T_Hatch_Def_Param, *T_PTR_Hatch_Def_Param ;

typedef struct
{
  int i_number_hatch ;		/*numer wzoru wypelnienia w tablicy definicji wypelnien*/
  double df_angle ;		/*kat pochylenia linii (w radianach !!!)*/
  double df_scale ;		/*wspolczynnik skali*/
  double df_basex, df_basey ;  	/*wspolrzedne punktu bazowego wypelnien*/
				/*(w jednostkach rysunku)*/
  BOOL insulation_hatch;
}
T_Hatch_Param, *T_PTR_Hatch_Param ;

extern void Get_Tangent_Ob (void *, double, double, double *, double *, BOOL) ;
extern int add_vertex (double, double) ;

#ifdef aaaaaaa
/*------------------------------------------------------*/


T_Node_Coord_Alloc s__nodes_coord_alloc                   T_Node_Coord
----------------------------------------                  -------------
 |--------------------------------------------------|    |---------------------------|
 | ptrs_nodes_coord - adres tablicy wezlow objectow |--> |  x,y  |  x,y  | ...   x,y |
 |--------------------------------------------------|    |---------------------------|
 | i_size           - rozmiar tablicy               |        0       1       i_size-1
 |--------------------------------------------------|
 | i_size_alloc     - aktualna alokacja             |
 |--------------------------------------------------|
 | i_unit_size 	    - rozmiar jednostki             |
 |--------------------------------------------------|
 | i_d_alloc        - wielkosc dealokacji           |
 |--------------------------------------------------|


T_PTR_Node_Alloc ptrs__nodes_table_alloc
------------------------------------------
 |------------------------------------------------------------|
 | off_object- offset obiektu w bazie danych       	      |     T_Node
 |------------------------------------------------------------|    |------------------------------|
 | T_Node_Alloc            | T_PTR_Node                       |    | df_t - dla odcinka [0,1],    |
 | s_nodes_alloc- alokacja | ptrs_nodes_object- adres         |--->|      dla luku, okregu [0,2Pi]|
 |         wezlow obiektu  | tablicy wezlow obiektu           |    |      dla tekstu [0,4]        |
 |                         |----------------------------------|    |------------------------------|
 |                         | i_size - rozmiar tablicy         |    | node_no - numer wezla        |
 |                         |----------------------------------|    |------------------------------|
 |                         | i_size_alloc - aktualna alokacja |    | use_no - roboczy dla obszarow|
 |                         |----------------------------------|    |------------------------------|
 |                         | i_unit_size - rozmiar jednostki  |    | reserve                      |
 |		           |----------------------------------|    |------------------------------|
 |		           | i_d_alloc - wielkosc dealokacji  |           .
 |-------------------------|----------------------------------|           .
		    .                                                     .  razy liczba wezlow
		    .                                                     .  obiektu (i_size)
		    .  razy liczba obiektow                               .
		    .   (i__objects_no)                                   .
		    .							  .
		    .                                                     .


T_Area
--------
 |------------------------------------------------------------|
 |  sel      - czy wskazany                                   |
 |------------------------------------------------------------|
 | hole     - dziura                                          |
 |------------------------------------------------------------|
 | area_no  - numer obszaru, dla                              |
 |            dziury numer rodzica                            |     T_Vertex
 |------------------------------------------------------------|    |------------------------------|
 | T_Vertex_Alloc          | T_PTR_Vertex                     |    | x, y - wspolrzedne wezla     |
 | vertexs_alloc - alokacja| ptrs_vertexs - adres             |--->|------------------------------|
 |           wierzcholkow  | tablicy wierzchlkow obszaru      |    | vertex_no - numer wezla      |
 |                         |----------------------------------|    |------------------------------|
 |                         | i_size - rozmiar tablicy         |    | type - typ wezla : regularny,|
 |                         |----------------------------------|    |         stalaktyt, stalagmit |
 |                         | i_size_alloc - aktualna alokacja |    |------------------------------|
 |                         |----------------------------------|             .............
 |                         | i_unit_size - rozmiar jednostki  |       razy liczba wezlow obszaru
 |		           |----------------------------------|               (i_size)
 |		           | i_d_alloc - wielkosc dealokacji  |     T_ALine
 |-------------------------|----------------------------------|    |---------------------------------|
 | T_ALine_Alloc           | T_PTR_ALine		      |    | vertex1_no - numer 1 wierzcholka|
 | lines_alloc  - alokacja | ptrs_lines - adres tablicy	      |--->|---------------------------------|
 |          krawedzi typu  |  krawedzi typu linia             |    | vertex2_no - numer 2 wierzcholka|
 |          linia          |----------------------------------|    |---------------------------------|
 |                         | i_size - rozmiar tablicy         |            ................
 |                         |----------------------------------| razy liczba odcinkow liniowych obszaru
 |                         | i_size_alloc - aktualna alokacja |                (i_size)
 |                         |----------------------------------|
 |                         | i_unit_size - rozmiar jednostki  |
 |		           |----------------------------------|
 |		           | i_d_alloc - wielkosc dealokacji  |     T_AArc
 |-------------------------|----------------------------------|    |---------------------------------|
 |  T_AArc_Alloc           | T_PTR_AArc			      |    | vertex1_no - numer 1 wierzcholka|
 |  arcs_alloc - adres     | ptrs_arcs -adres tablicy krawedzi|--->|---------------------------------|
 |  tablicy krawedzi typu  | typu luk                         |    | vertex2_no - numer 2 wierzcholka|
 |  luk                    |----------------------------------|    |---------------------------------|
 |                         | i_size - rozmiar tablicy         |    | r, a1, a2, x, y - parametry luku|
 |                         |----------------------------------|    |---------------------------------|
 |                         | i_size_alloc - aktualna alokacja |            ...................
 |                         |----------------------------------|  razy liczba odcinkow lukowych obszaru
 |                         | i_unit_size - rozmiar jednostki  |               (i_size)
 |		           |----------------------------------|
 |		           | i_d_alloc - wielkosc dealokacji  |
 |-------------------------|----------------------------------|



/*------------------------------------------------------*/
#endif
