#include "CT_section_US_si_standards.c"

POLE pmCT_section_US_si[] = {
{u8"ADM 2015",L' ',0,&mCT_section_US_si_0},
{u8"ADM 2020",L' ',0,&mCT_section_US_si_1},
{u8"AISC 13",L' ',0,&mCT_section_US_si_2},
{u8"AISC 14",L' ',0,&mCT_section_US_si_3},
{u8"AISC 15",L' ',0,&mCT_section_US_si_4},
{u8"AISC 16",L' ',0,&mCT_section_US_si_5},
{u8"ANSI B 36.10M",L' ',0,&mCT_section_US_si_6},
{u8"ANSI B 36.19M",L' ',0,&mCT_section_US_si_7},
{u8"ASD 9",L' ',0,&mCT_section_US_si_8},
{u8"ASME B 16.9:2003",L' ',0,&mCT_section_US_si_9},
{u8"ASME B 36.10M:2004",L' ',0,&mCT_section_US_si_10},
{u8"ASME B 36.19M:2004",L' ',0,&mCT_section_US_si_11},
};

TMENU mCT_section_US_si = { 12,0,0,19,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_US_si,NULL,NULL };

