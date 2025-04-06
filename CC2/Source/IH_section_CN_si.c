#include "IH_section_CN_si_standards.c"

POLE pmIH_section_CN_si[] = {
{u8"YB 3301-2005",L' ',0,&mIH_section_CN_si_0},
{u8"YB 3301-1992",L' ',0,&mIH_section_CN_si_1},
{u8"GB/T 706-2016",L' ',0,&mIH_section_CN_si_2},
{u8"GB/T 706-2008",L' ',0,&mIH_section_CN_si_3},
{u8"GB/T 706-1988",L' ',0,&mIH_section_CN_si_4},
{u8"GB/T 11263-2017",L' ',0,&mIH_section_CN_si_5},
{u8"GB/T 11263-2010",L' ',0,&mIH_section_CN_si_6},
{u8"GB/T 11263-2005",L' ',0,&mIH_section_CN_si_7},
{u8"GB/T 11263-1998",L' ',0,&mIH_section_CN_si_8},
};

TMENU mIH_section_CN_si = { 9,0,0,16,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmIH_section_CN_si,NULL,NULL };

