#include "T_section_CN_si_standards.c"

POLE pmT_section_CN_si[] = {
{u8"GB/T 11263-2010",L' ',0,&mT_section_CN_si_0},
{u8"GB/T 11263-2005",L' ',0,&mT_section_CN_si_1},
{u8"GB/T 11263-1998",L' ',0,&mT_section_CN_si_2},
};

TMENU mT_section_CN_si = { 3,0,0,16,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmT_section_CN_si,NULL,NULL };

