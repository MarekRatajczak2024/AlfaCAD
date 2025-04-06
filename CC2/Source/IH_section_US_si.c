#include "IH_section_US_si_standards.c"

POLE pmIH_section_US_si[] = {
{u8"ADM 2015",L' ',0,&mIH_section_US_si_0},
{u8"ADM 2020",L' ',0,&mIH_section_US_si_1},
{u8"AISC 13",L' ',0,&mIH_section_US_si_2},
{u8"AISC 14",L' ',0,&mIH_section_US_si_3},
{u8"AISC 15",L' ',0,&mIH_section_US_si_4},
{u8"AISC 16",L' ',0,&mIH_section_US_si_5},
{u8"ASD 9",L' ',0,&mIH_section_US_si_6},
{u8"ASTM A 6/A 6M",L' ',0,&mIH_section_US_si_7},
{u8"ASTM A 6/A 6M-07",L' ',0,&mIH_section_US_si_8},
{u8"BS 4-1:2005; ASTM A 6/A 6M",L' ',0,&mIH_section_US_si_9},
{u8"BS 4-1:2005; ASTM A 6/A 6M-07",L' ',0,&mIH_section_US_si_10},
{u8"Euronorm 19-57; DIN 1025-5; ASTM A 6/A 6M",L' ',0,&mIH_section_US_si_11},
{u8"Euronorm 53-62; ASTM A 6/A 6M",L' ',0,&mIH_section_US_si_12},
{u8"Euronorm 53-62; ASTM A 6/A 6M-07",L' ',0,&mIH_section_US_si_13},
};

TMENU mIH_section_US_si = { 14,0,0,42,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmIH_section_US_si,NULL,NULL };

