#include "U_section_UK_si_standards.c"

POLE pmU_section_UK_si[] = {
{u8"BS 4-1:1993\0FeC,Al",L' ',0,&mU_section_UK_si_0},
{u8"BS 4-1:2005\0FeC,Al",L' ',0,&mU_section_UK_si_1},
{u8"BS 4-1:2005; JIS G 3192\0FeC,Al",L' ',0,&mU_section_UK_si_2},
{u8"BS EN 10365:2017",L' ',0,&mU_section_UK_si_3},
};

TMENU mU_section_UK_si = { 4,0,0,30,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmU_section_UK_si,NULL,NULL };

