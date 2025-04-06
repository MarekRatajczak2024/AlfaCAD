#include "L_section_EU_si_standards.c"

POLE pmL_section_EU_si[] = {
{u8"EN 10056",L' ',0,&mL_section_EU_si_0},
{u8"EN 10056-1:1998",L' ',0,&mL_section_EU_si_1},
{u8"EN 10056-1:2017",L' ',0,&mL_section_EU_si_2},
};

TMENU mL_section_EU_si = { 3,0,0,16,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmL_section_EU_si,NULL,NULL };

