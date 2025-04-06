#include "T_section_UK_si_series.c"

POLE pmT_section_UK_si_0[] = {
{u8"TB\0™Continental Steel\0",L' ',0,&mT_section_UK_si_0_0},
{u8"1/2 UC\0™Macsteel\0",L' ',0,&mT_section_UK_si_0_1},
{u8"1/2 UB\0™Macsteel\0",L' ',0,&mT_section_UK_si_0_2},
};

TMENU mT_section_UK_si_0 = { 3,0,0,27,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmT_section_UK_si_0,NULL,NULL };

POLE pmT_section_UK_si_1[] = {
{u8"Advance UKT (UKC)\0™Tata Steel\0",L' ',0,&mT_section_UK_si_1_0},
{u8"Advance UKT (UKB)\0™Tata Steel\0",L' ',0,&mT_section_UK_si_1_1},
};

TMENU mT_section_UK_si_1 = { 2,0,0,35,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmT_section_UK_si_1,NULL,NULL };

POLE pmT_section_UK_si_2[] = {
{u8"1/2 UC\0™British Steel\0",L' ',0,&mT_section_UK_si_2_0},
{u8"1/2 UB\0™British Steel\0",L' ',0,&mT_section_UK_si_2_1},
};

TMENU mT_section_UK_si_2 = { 2,0,0,27,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmT_section_UK_si_2,NULL,NULL };

