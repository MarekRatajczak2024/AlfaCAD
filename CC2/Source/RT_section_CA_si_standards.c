#include "RT_section_CA_si_series.c"

POLE pmRT_section_CA_si_0[] = {
{u8"Sqr HSS (ASTM A500)\0\0",L' ',0,&mRT_section_CA_si_0_0},
{u8"Sqr HSS\0\0",L' ',0,&mRT_section_CA_si_0_1},
{u8"Rect HSS (ASTM A500)\0\0",L' ',0,&mRT_section_CA_si_0_2},
{u8"Rect HSS\0\0",L' ',0,&mRT_section_CA_si_0_3},
};

TMENU mRT_section_CA_si_0 = { 4,0,0,25,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_CA_si_0,NULL,NULL };

POLE pmRT_section_CA_si_1[] = {
{u8"Sqr HSS (CSA G40.20)\0\0",L' ',0,&mRT_section_CA_si_1_0},
{u8"Sqr HSS (ASTM A500)\0\0",L' ',0,&mRT_section_CA_si_1_1},
{u8"Rect HSS (CSA G40.20)\0\0",L' ',0,&mRT_section_CA_si_1_2},
{u8"Rect HSS (ASTM A500)\0\0",L' ',0,&mRT_section_CA_si_1_3},
};

TMENU mRT_section_CA_si_1 = { 4,0,0,26,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_CA_si_1,NULL,NULL };

