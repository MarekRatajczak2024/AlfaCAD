#include "Z_section_US_si_standards.c"

POLE pmZ_section_US_si[] = {
{u8"ADM 2015",L' ',0,&mZ_section_US_si_0},
{u8"ADM 2020",L' ',0,&mZ_section_US_si_1},
{u8"AISI D100-17",L' ',0,&mZ_section_US_si_2},
};

TMENU mZ_section_US_si = { 3,0,0,13,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmZ_section_US_si,NULL,NULL };

