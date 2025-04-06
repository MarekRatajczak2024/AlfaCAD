#include "CT_section_EU_si_standards.c"

POLE pmCT_section_EU_si[] = {
{u8"EN 10210-2",L' ',0,&mCT_section_EU_si_0},
{u8"EN 10210-2:1997",L' ',0,&mCT_section_EU_si_1},
{u8"EN 10210-2:2006-04",L' ',0,&mCT_section_EU_si_2},
{u8"EN 10219-2",L' ',0,&mCT_section_EU_si_3},
{u8"EN 10219-2:1997",L' ',0,&mCT_section_EU_si_4},
{u8"EN 10219-2:2006-04",L' ',0,&mCT_section_EU_si_5},
{u8"EN 10220",L' ',0,&mCT_section_EU_si_6},
{u8"EN 10220:2002-12",L' ',0,&mCT_section_EU_si_7},
{u8"EN 10253-2:2007 (Type A)",L' ',0,&mCT_section_EU_si_8},
{u8"EN 10253-2:2007 (Type B)",L' ',0,&mCT_section_EU_si_9},
{u8"EN 10255",L' ',0,&mCT_section_EU_si_10},
};

TMENU mCT_section_EU_si = { 11,0,0,25,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmCT_section_EU_si,NULL,NULL };

