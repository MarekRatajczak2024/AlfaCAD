
#ifdef __cplusplus
extern "C" {
#endif

extern void NearestO_ (double *, double *, void *) ;
extern void koniecL_(double *x, double *y, void *adr);
extern void wielokat_ (double *, double *, void *,void (*)(double*, double*, void*)) ;
extern void wielokat_linia_ (double *x, double *y, void *adr, void *adr1, void (*fun)(double*, double*, void*));
extern void wspline_(double *, double *, void *, void(*)(double*, double*, void*));
extern void wellipticalarc_(double *x, double *y, void *adr, void(*fun)(double*, double*, void*));
extern void koniecw_ (double *x, double *y, void *adr);
extern int przeciecieLL_(double *x, double *y, void *adr, void *adr1);
extern double dP1P2 (double x1, double y1, double x2, double y2);
extern int przeciecieLO_ (double *x, double *y, void *adr, void *adr1);
extern int przeciecieLE_ (double *x, double *y, void *adr, void *adr1);
extern int przeciecieLEA_ (double *x, double *y, void *adr, void *adr1);
extern int przeciecieSAEA_ (double *x, double *y, void *adr, void *adr1);
extern int przeciecieLl_(double *x, double *y, void *adr, void *adr1);
extern void najblizszyL_(double *x, double *y, void *adr);
extern int prostopadlew_ (double *x,double *y,void *adr);
extern int prostopadleL_(double *x,double *y,void *adr);
extern int prostopadleLx_(double* x, double* y, void* adr);
extern void prostopadO(double LX,double LY,double *x, double *y, void *adr);
extern void najblizszyO (double *x, double *y, void *adr);
extern void najblizszyE (double *x, double *y, void *adr);
extern BOOL najblizszyEA_ (double *x, double *y, void *adr);
extern void najblizszyO_(double *x,double *y,void *adr);
extern int prostopadleO_(double *x,double *y,void *adr);
extern int prostopadlel(double LX,double LY,double *x, double *y, void *adr);
extern int prostopadlel_(double *x,double *y,void *adr);
extern int przeciecieOO_(double *x, double *y, void *adr, void *adr1);
extern int przecieciell_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielO_(double *x, double *y, void *adr, void *adr1);
extern BOOL wielokat_przec (double *, double *, void *, void *,int (*)(double*, double*, void*, void*)) ;
extern int mwspline_(double* x, double* y, void* adr, int(*fun)(double*, double*, void*));
extern int przeciecieLW_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielW_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieWO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieSAO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieWW_(double *x, double *y, void *adr, void *adr1);
extern void najblizszyl_ (double *x, double *y, void *adr);
extern int najblizszyLx_(double* x, double* y, void* adr);
extern void koniecl_(double *x,double *y,void *adr);
extern void Polygon_Center (double, double, double, double, int, double *, double *);
extern void srodek(void);
extern void najblizszy(void);
extern void centrum(void);
extern void koniec(void);
extern void koniecS(void);
extern void przeciecie(void);
extern void prostopadle(void);
extern void stycznie(void);
extern void Point_Sel (void);
extern void JumpB (void);
extern void JumpW (void);
extern void JumpL (void);
extern void qkoniec(double *x,double *y);
extern void qsrodek(double *x,double *y);
extern void qprzeciecie(double *x,double *y);
extern void qcentrum(double *x,double *y);
extern void qnajblizszy(double *x,double *y);
extern void qprostopadle(double *x,double *y);
extern void qstycznie(double *x,double *y);
extern void qpoint (double *x, double *y);
extern void Get_Sel_XY (double *x, double *y);

/*2*/
extern int przeciecieLO_tt (double *, double *, double *, double *, double *, double *, void *, void *) ;
extern int przeciecieLE_tt (double *, double *, double *, double *, double *, double *, void *, void *) ;
extern int przeciecieLEA_tt (double *, double *, double *, double *, double *, double *, void *, void *) ;
extern int przeciecieLl_tt (double *, double *, double *, double *, double *, double *, void *, void *) ;
extern int przeciecieOO_ex (double *, double *, double *, double *, void *, void *) ;
extern int przeciecieLL_tt (double *, double *, void *, void *, double *, double *) ;

/*1*/
extern int IntersectionLL (LINIA*, LINIA*, double *,double *,double *, double *) ;
extern int IntersectionLl (LINIA*, LUK*, double *,double *,double *, double *) ;
extern int IntersectionLl_h (LINIA*, LUK*, double *,double *,double *, double *) ;
extern int IntersectionLO (LINIA*, OKRAG*, double *,double *,double *, double *) ;
extern int IntersectionWL (WIELOKAT*, LINIA*, double *,double *,double *, double *) ;
extern int IntersectionSAL (SOLIDARC*, LINIA*, double *,double *,double *, double *) ;
extern int IntersectionWl (WIELOKAT*, LUK*, double *,double *,double *, double *) ;
extern int IntersectionWO (WIELOKAT*, OKRAG*, double *,double *,double *, double *) ;
extern int IntersectionWW (WIELOKAT*, WIELOKAT*, double *,double *,double *, double *) ;

extern int IntersectionSL(SPLINE*, LINIA*, double *, double *, double *, double *);
extern int IntersectionSl(SPLINE*, LUK*, double *, double *, double *, double *);
extern int IntersectionSO(SPLINE*, OKRAG*, double *, double *, double *, double *);
extern int IntersectionSW(SPLINE*, WIELOKAT*, double *, double *, double *, double *);
extern int IntersectionSS(SPLINE*, SPLINE*, double *, double *, double *, double *);

extern int Intersectionll (LUK*, LUK*, double *,double *,double *, double *) ;
extern int IntersectionlO (LUK*, OKRAG*, double *,double *,double *, double *) ;
extern int IntersectionOO (OKRAG*, OKRAG*, double *,double *,double *, double *) ;

#ifdef __cplusplus
}
#endif
