#include "T_section_CA_si_standards.c"

POLE pmT_section_CA_si[] = {
{u8"CISC 9",L' ',0,&mT_section_CA_si_0},
{u8"CISC 12",L' ',0,&mT_section_CA_si_1},
};

TMENU mT_section_CA_si = { 2,0,0,8,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmT_section_CA_si,NULL,NULL };

