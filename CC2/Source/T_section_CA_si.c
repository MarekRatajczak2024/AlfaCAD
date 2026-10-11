#include "T_section_CA_si_standards.c"

POLE pmT_section_CA_si[] = {
{u8"CISC 9\0FeC,Al",L' ',0,&mT_section_CA_si_0},
{u8"CISC 12\0FeC,Al",L' ',0,&mT_section_CA_si_1},
};

TMENU mT_section_CA_si = { 2,0,0,16,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmT_section_CA_si,NULL,NULL };

