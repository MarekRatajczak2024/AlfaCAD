#include "CT_section_UK_si_standards.c"

POLE pmCT_section_UK_si[] = {
{u8"BS EN 10210-2:2006\0FeC,Al",L' ',0,&mCT_section_UK_si_0},
{u8"BS EN 10219-2:2006\0FeC,Al",L' ',0,&mCT_section_UK_si_1},
};

TMENU mCT_section_UK_si = { 2,0,0,25,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_UK_si,NULL,NULL };

