#include "RT_section_AU_si_series.c"

POLE pmRT_section_AU_si_0[] = {
{u8"Galtube SHS",L' ',0,&mRT_section_AU_si_0_0},
{u8"Galtube RHS",L' ',0,&mRT_section_AU_si_0_1},
};

TMENU mRT_section_AU_si_0 = { 2,0,0,12,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_AU_si_0,NULL,NULL };

POLE pmRT_section_AU_si_1[] = {
{u8"Tubeline SHS",L' ',0,&mRT_section_AU_si_1_0},
{u8"Tubeline RHS",L' ',0,&mRT_section_AU_si_1_1},
{u8"DuraGal SHS",L' ',0,&mRT_section_AU_si_1_2},
{u8"DuraGal RHS",L' ',0,&mRT_section_AU_si_1_3},
};

TMENU mRT_section_AU_si_1 = { 4,0,0,13,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmRT_section_AU_si_1,NULL,NULL };

