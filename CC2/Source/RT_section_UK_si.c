#include "RT_section_UK_si_standards.c"

POLE pmRT_section_UK_si[] = {
{u8"BS EN 10210-2:2006",L' ',0,&mRT_section_UK_si_0},
{u8"BS EN 10219-2:2006",L' ',0,&mRT_section_UK_si_1},
};

TMENU mRT_section_UK_si = { 2,0,0,19,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_UK_si,NULL,NULL };

