//
// Created by marek on 3/18/25.
//

#ifndef ALFACAD_O_SECTION_BLOCK_H
#define ALFACAD_O_SECTION_BLOCK_H

extern int create_profile_block(char *units_system, char *series0, char *manufacturer0, char *type0, double h, double b, double tw, double tf, double r1, double r2, double sf, double bt, double bb, double bf, double t, double ha, double ba, double ab, double c, double c1, double r3, double ri, double sw);
extern int calculator(char *buf, int *retval_no, double *buf_ret);
extern BOOL add_block_(void);
extern char  *dodaj_obiekt(BLOK * adb,void  *ad);
extern char  *dane;
extern LINIA LiniaG;
extern void Place_Import_Block (int opcja, char *blockfile);
extern void adrem_blok1 (BLOK *adb, int atrybut);
extern char *Get_PTR__GTMPFIRST(void);
extern void Set_PTR__GTMPFIRST(char *first);

#endif //ALFACAD_O_SECTION_BLOCK_H
