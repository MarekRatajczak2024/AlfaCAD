#include "libfun.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void menu_par_new (char*, const char *) ;
extern void obru(double si,double co, double xp, double yp, double *xn,double *yn) ;
extern void obrd(double si,double co, double xp, double yp, double *xn, double *yn);
extern void DF_to_String (char *ptrsz_buf, char *ptrsz_format, double df_val, int max_size);
extern void Set_Block_Change_Mode (void *ptr_ob, int mode);
extern void Rotate_Point (double si, double co, double x1, double y1, double x2, double y2, double *x, double *y); /*rotate point*/
extern void Change_Global_Ptr (long n);
extern void Ini_Global_Object (void);
extern double Angle_Normal (double angle);
extern double Angle_Simple (double angle);
extern double Angle_Normal_Grid (double angle);
extern double Grid_to_Rad (double angle);
extern double Rad_to_Grid (double angle);
extern double Get_Prec (int i_prec, double df_x, double df_y) ;
extern BOOL Check_Angle_Prec (double k, double k1, double k2, int i_prec);
extern BOOL Check_Angle (double k, double k1, double k2);
extern BOOL Sharp_Check_Angle (double k, double k1, double k2);
extern BOOL Check_if_Equal0 (double x, double y);
extern BOOL Check_if_Equal (double x, double y);
extern BOOL Check_if_Equal1 (double x, double y);
extern BOOL Check_if_Equal2(double x, double y);
extern BOOL Check_if_Equal3(double x, double y);
extern BOOL Check_if_GE (double x, double y);
extern BOOL Check_if_GE02(double x, double y);
extern BOOL Check_if_LE (double x, double y);
extern BOOL Check_if_GT (double x, double y);
extern BOOL Check_if_LT (double x, double y);
extern double Double_to_Float (double x);
extern long Double_to_Long (double x);
extern int Double_to_Int (double x);
extern double Dist_PP (double x1,double y1, double x2, double y2);
extern void  Quick_Exit (int errorno);
extern void out_krz(double X, double Y);
extern void out_krz_sc(double X, double Y);
extern BOOL Auto_Backup_Proc (void);
extern void Set_Auto_Backup (BOOL change);
extern double Get_Angle_Difference (double, double) ;
extern void Get_Menu_Param (int, int, int, int *, int *) ;
extern int Color_No_to_Menu_No (int) ;
extern int Menu_No_to_Color_No (int) ;
extern void Truncate_Spacing (T_Fstring );
extern T_Fstring Ignore_Spacing (T_Fstring );
extern double Atan2 (double, double) ;
extern void Ini_Sys_Param (void) ;
extern int Get_Svga_Mode (void) ;

#ifdef __cplusplus
}
#endif
