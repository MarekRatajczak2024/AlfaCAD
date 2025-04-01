#include "ET_section_CA_si_standards.c"

POLE pmET_section_CA_si[] = {
{u8"CAN/CSA-G40.20",L' ',0,&mET_section_CA_si_0},
};

TMENU mET_section_CA_si = { 1,0,0,15,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmET_section_CA_si,NULL,NULL };

