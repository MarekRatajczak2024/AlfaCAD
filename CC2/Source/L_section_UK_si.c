#include "L_section_UK_si_standards.c"

POLE pmL_section_UK_si[] = {
{u8"BS EN 10056-1:1999",L' ',0,&mL_section_UK_si_0},
{u8"BS EN 10056-1:2017",L' ',0,&mL_section_UK_si_1},
};

TMENU mL_section_UK_si = { 2,0,0,19,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmL_section_UK_si,NULL,NULL };

