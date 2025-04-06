#include "CT_section_AU_si_standards.c"

POLE pmCT_section_AU_si[] = {
{u8"AS/NZS 4600",L' ',0,&mCT_section_AU_si_0},
{u8"AS 4100-1998",L' ',0,&mCT_section_AU_si_1},
};

TMENU mCT_section_AU_si = { 2,0,0,13,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_AU_si,NULL,NULL };

