#include "RT_section_US_si_standards.c"

POLE pmRT_section_US_si[] = {
{u8"ADM 2015",L' ',0,&mRT_section_US_si_0},
{u8"ADM 2020",L' ',0,&mRT_section_US_si_1},
{u8"AISC 13",L' ',0,&mRT_section_US_si_2},
{u8"AISC 14",L' ',0,&mRT_section_US_si_3},
{u8"AISC 15",L' ',0,&mRT_section_US_si_4},
{u8"AISC 16",L' ',0,&mRT_section_US_si_5},
{u8"ASD 9",L' ',0,&mRT_section_US_si_6},
};

TMENU mRT_section_US_si = { 7,0,0,9,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_US_si,NULL,NULL };

