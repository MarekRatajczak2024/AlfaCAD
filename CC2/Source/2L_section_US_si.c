#include "2L_section_US_si_standards.c"

POLE pm2L_section_US_si[] = {
{u8"AISC 13",L' ',0,&m2L_section_US_si_0},
{u8"AISC 14",L' ',0,&m2L_section_US_si_1},
{u8"AISC 15",L' ',0,&m2L_section_US_si_2},
{u8"AISC 16",L' ',0,&m2L_section_US_si_3},
};

TMENU m2L_section_US_si = { 4,0,0,8,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pm2L_section_US_si,NULL,NULL };

