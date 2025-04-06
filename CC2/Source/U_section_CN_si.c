#include "U_section_CN_si_standards.c"

POLE pmU_section_CN_si[] = {
{u8"JG/T 380-2012",L' ',0,&mU_section_CN_si_0},
{u8"GB/T 707-1988",L' ',0,&mU_section_CN_si_1},
{u8"GB/T 706-2016",L' ',0,&mU_section_CN_si_2},
{u8"GB/T 706-2008",L' ',0,&mU_section_CN_si_3},
{u8"GB 50018-2002",L' ',0,&mU_section_CN_si_4},
{u8"CECS 102:2002",L' ',0,&mU_section_CN_si_5},
};

TMENU mU_section_CN_si = { 6,0,0,14,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmU_section_CN_si,NULL,NULL };

