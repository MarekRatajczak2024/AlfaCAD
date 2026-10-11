#include "Z_section_CA_si_standards.c"

POLE pmZ_section_CA_si[] = {
{u8"CISC 9\0FeC,Al",L' ',0,&mZ_section_CA_si_0},
{u8"CISC 12\0FeC,Al",L' ',0,&mZ_section_CA_si_1},
{u8"AISI D100-17",L' ',0,&mZ_section_CA_si_2},
};

TMENU mZ_section_CA_si = { 3,0,0,16,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmZ_section_CA_si,NULL,NULL };

