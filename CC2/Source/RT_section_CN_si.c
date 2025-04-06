#include "RT_section_CN_si_standards.c"

POLE pmRT_section_CN_si[] = {
{u8"GB 50018-2002",L' ',0,&mRT_section_CN_si_0},
{u8"GB/T 6728-1986",L' ',0,&mRT_section_CN_si_1},
{u8"GB/T 6728-2002",L' ',0,&mRT_section_CN_si_2},
{u8"GB/T 6728-2017",L' ',0,&mRT_section_CN_si_3},
{u8"JG/T 178-2005",L' ',0,&mRT_section_CN_si_4},
};

TMENU mRT_section_CN_si = { 5,0,0,15,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_CN_si,NULL,NULL };

