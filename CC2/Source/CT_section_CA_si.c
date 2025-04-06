#include "CT_section_CA_si_standards.c"

POLE pmCT_section_CA_si[] = {
{u8"CISC 9",L' ',0,&mCT_section_CA_si_0},
{u8"CISC 12",L' ',0,&mCT_section_CA_si_1},
};

TMENU mCT_section_CA_si = { 2,0,0,8,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_CA_si,NULL,NULL };

