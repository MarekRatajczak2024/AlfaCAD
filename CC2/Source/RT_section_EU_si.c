#include "RT_section_EU_si_standards.c"

POLE pmRT_section_EU_si[] = {
{u8"EN 10210-2",L' ',0,&mRT_section_EU_si_0},
{u8"EN 10210-2:1997",L' ',0,&mRT_section_EU_si_1},
{u8"EN 10210-2:2006-04",L' ',0,&mRT_section_EU_si_2},
{u8"EN 10219-2",L' ',0,&mRT_section_EU_si_3},
{u8"EN 10219-2:1997",L' ',0,&mRT_section_EU_si_4},
{u8"EN 10219-2:2006-04",L' ',0,&mRT_section_EU_si_5},
{u8"EN 10305-5",L' ',0,&mRT_section_EU_si_6},
};

TMENU mRT_section_EU_si = { 7,0,0,19,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_EU_si,NULL,NULL };

