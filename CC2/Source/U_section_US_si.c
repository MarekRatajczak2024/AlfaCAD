#include "U_section_US_si_standards.c"

POLE pmU_section_US_si[] = {
{u8"ADM 2015",L' ',0,&mU_section_US_si_0},
{u8"ADM 2020",L' ',0,&mU_section_US_si_1},
{u8"AISC 13",L' ',0,&mU_section_US_si_2},
{u8"AISC 14",L' ',0,&mU_section_US_si_3},
{u8"AISC 15",L' ',0,&mU_section_US_si_4},
{u8"AISC 16",L' ',0,&mU_section_US_si_5},
{u8"AISI D100-17",L' ',0,&mU_section_US_si_6},
{u8"ASD 9",L' ',0,&mU_section_US_si_7},
{u8"ASTM A 6/A 6M",L' ',0,&mU_section_US_si_8},
};

TMENU mU_section_US_si = { 9,0,0,14,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmU_section_US_si,NULL,NULL };

