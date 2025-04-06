#include "CT_section_CA_si_series.c"

POLE pmCT_section_CA_si_0[] = {
{u8"Round HSS (ASTM A500)\0\0",L' ',0,&mCT_section_CA_si_0_0},
{u8"Round HSS\0\0",L' ',0,&mCT_section_CA_si_0_1},
{u8"Pipe (ASTM A53)\0\0",L' ',0,&mCT_section_CA_si_0_2},
};

TMENU mCT_section_CA_si_0 = { 3,0,0,26,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_CA_si_0,NULL,NULL };

POLE pmCT_section_CA_si_1[] = {
{u8"Round HSS (CSA G40.20)\0\0",L' ',0,&mCT_section_CA_si_1_0},
{u8"Round HSS (ASTM A500)\0\0",L' ',0,&mCT_section_CA_si_1_1},
{u8"Pipe XXS\0\0",L' ',0,&mCT_section_CA_si_1_2},
{u8"Pipe XS\0\0",L' ',0,&mCT_section_CA_si_1_3},
{u8"Pipe STD\0\0",L' ',0,&mCT_section_CA_si_1_4},
};

TMENU mCT_section_CA_si_1 = { 5,0,0,27,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_CA_si_1,NULL,NULL };

