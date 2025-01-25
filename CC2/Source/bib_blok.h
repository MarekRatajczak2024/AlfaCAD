#ifdef __cplusplus
extern "C" {
#endif

extern void Scale_Arc (LUK *,double , double , double , double ) ;
extern void Scale_Arc_Vector (AVECTOR *,double , double , double , double ) ;
extern void Scale_Point (double, double, double, double, double, double, double *,double *);
extern void Rotate_Line (void *, double , double , double , double );
extern void Rotate_Arc (void *, double , double , double , double );
extern void transformacja_blok2 (char *,char  *, double, double, double, double, double, double, double, double, int, double);
extern void transformacja_blok (char  *,char  *,double,double,double,double,int,double);
extern void rysuj_obiekt_(char *, int,int);
extern void rysuj_obiekt (char *ad, int mode, int kolor);
extern void mvcurbp(unsigned i);
extern void blokzap(char  *,char  *,int ,int , int);
extern void out_blok(void);
extern void out_blok2 (double ,double,double,double,double,double,double,double,int,double,BOOL, int no_object);
extern void out_blok1(double,double,double,double,int,double);
#ifdef __cplusplus
}
#endif

