#include "RT_section_AU_si_standards.c"

POLE pmRT_section_AU_si[] = {
{u8"AS/NZS 4600",L' ',0,&mRT_section_AU_si_0},
{u8"AS 4100-1998",L' ',0,&mRT_section_AU_si_1},
};

TMENU mRT_section_AU_si = { 2,0,0,13,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_AU_si,NULL,NULL };

