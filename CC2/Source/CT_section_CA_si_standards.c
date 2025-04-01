#include "CT_section_CA_si_series.c"

POLE pmCT_section_CA_si_0[] = {
{u8"Round HSS (ASTM A500)",L' ',0,&mCT_section_CA_si_0_0},
{u8"Round HSS",L' ',0,&mCT_section_CA_si_0_1},
{u8"Pipe (ASTM A53)",L' ',0,&mCT_section_CA_si_0_2},
};

TMENU mCT_section_CA_si_0 = { 3,0,0,22,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_CA_si_0,NULL,NULL };

POLE pmCT_section_CA_si_1[] = {
{u8"Round HSS (CSA G40.20)",L' ',0,&mCT_section_CA_si_1_0},
{u8"Round HSS (ASTM A500)",L' ',0,&mCT_section_CA_si_1_1},
{u8"Pipe XXS",L' ',0,&mCT_section_CA_si_1_2},
{u8"Pipe XS",L' ',0,&mCT_section_CA_si_1_3},
{u8"Pipe STD",L' ',0,&mCT_section_CA_si_1_4},
};

TMENU mCT_section_CA_si_1 = { 5,0,0,23,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_CA_si_1,NULL,NULL };

