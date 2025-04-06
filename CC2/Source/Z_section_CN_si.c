#include "Z_section_CN_si_standards.c"

POLE pmZ_section_CN_si[] = {
{u8"JG/T 380-2012",L' ',0,&mZ_section_CN_si_0},
{u8"GB 50018-2002",L' ',0,&mZ_section_CN_si_1},
};

TMENU mZ_section_CN_si = { 2,0,0,14,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmZ_section_CN_si,NULL,NULL };

