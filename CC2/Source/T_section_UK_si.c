#include "T_section_UK_si_standards.c"

POLE pmT_section_UK_si[] = {
{u8"BS 4-1:1993\0FeC,Al",L' ',0,&mT_section_UK_si_0},
{u8"BS 4-1:2005\0FeC,Al",L' ',0,&mT_section_UK_si_1},
{u8"BS EN 10365:2017",L' ',0,&mT_section_UK_si_2},
};

TMENU mT_section_UK_si = { 3,0,0,19,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmT_section_UK_si,NULL,NULL };

