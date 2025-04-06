#include "L_section_CN_si_standards.c"

POLE pmL_section_CN_si[] = {
{u8"GB 50018-2002",L' ',0,&mL_section_CN_si_0},
{u8"GB/T 4227-1984",L' ',0,&mL_section_CN_si_1},
{u8"GB/T 706-2008",L' ',0,&mL_section_CN_si_2},
{u8"GB/T 706-2016",L' ',0,&mL_section_CN_si_3},
{u8"GB/T 9787-1988",L' ',0,&mL_section_CN_si_4},
{u8"GB/T 9788-1988",L' ',0,&mL_section_CN_si_5},
{u8"JG/T 380-2012",L' ',0,&mL_section_CN_si_6},
};

TMENU mL_section_CN_si = { 7,0,0,15,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmL_section_CN_si,NULL,NULL };

