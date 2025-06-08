
enum  FILLET_PL
{
  FILLET_PL_NO = 0,    		/*profilowanie zabronione*/
  FILLET_PL_YES = 1,            /*profilowanie sasiednich segmentow wielolinii*/
  FILLET_PL_YES_OUT = 2,        /*profilowanie brzegowych segmentow wielolinii*/
  FILLET_PL_YES_OTHER = 3       /*profilowanie innych obiektow*/
} ;

typedef
struct
{ double xs,ys;
    double xc,yc;
    double xe,ye;
    double x2,y2;
    double a;
    double L;
    double r;
    double d;
    void *ptr_con ; /*kontynuacja, xs, ys : wsp. konca, do kontynuacji*/
    BOOL b_first_end ;	/*ktory koniec objektu jest ostatni*/
    double con_angle ; /*kat kontynuacji*/
} PLUK;

#ifdef __cplusplus
extern "C" {
#endif

extern BOOL Get_End_Pline (void *,void **,BOOL *,double ,double ,double *,double *);
extern BOOL Get_Begin_Pline (void *,void **,BOOL *,double *,double *);
extern void * Get_Pline_First_Ob (void*) ;
extern void * Get_Pline_Last_Ob (void*) ;
extern double Get_Len_Pline (BLOK *) ;
extern BOOL Check_if_Inside_Pline (BLOK *, void *) ;
extern BOOL Check_if_Close_Pline (BLOK *) ;
extern BOOL Check_Pline (BLOK *) ;
extern BOOL Truncate_End_Pline (BLOK **, long_long, double, double, double, double) ;
extern BOOL Truncate_Begin_Pline (BLOK **, long_long, double, double, double, double) ;
extern void Break_Line (LINIA *, LINIA *, LINIA *, LINIA *, double, double, double, double, double *, double *) ;
extern void Break_Arc (LUK *, LUK *, LUK *, LUK *, double, double, double, double) ;
extern void Break_Elliptical_Arc (ELLIPTICALARC *, ELLIPTICALARC *, ELLIPTICALARC *, ELLIPTICALARC *, double, double, double, double);
extern BLOK *Pline_Cat (BLOK *, BLOK **) ;
extern BOOL Check_if_Vertex_Inside_Pline (void *, double, double) ;
extern int Check_Pline_to_Fillet (void *, void *) ;
extern void * Get_Pline_Previous_Ob (void*, void *, BOOL) ;
extern void * Get_Pline_Next_Ob (void*, void *, BOOL) ;

#ifdef __cplusplus
}
#endif

