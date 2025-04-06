#include "U_section_EU_si_standards.c"

POLE pmU_section_EU_si[] = {
{u8"EN 10279",L' ',0,&mU_section_EU_si_0},
{u8"EN 10365:2017",L' ',0,&mU_section_EU_si_1},
};

TMENU mU_section_EU_si = { 2,0,0,14,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmU_section_EU_si,NULL,NULL };

