#include "ET_section_EU_si_standards.c"

POLE pmET_section_EU_si[] = {
{u8"EN 10210-2",L' ',0,&mET_section_EU_si_0},
{u8"EN 10210-2:2006-04",L' ',0,&mET_section_EU_si_1},
};

TMENU mET_section_EU_si = { 2,0,0,19,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmET_section_EU_si,NULL,NULL };

