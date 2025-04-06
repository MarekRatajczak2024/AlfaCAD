#include "T_section_EU_si_standards.c"

POLE pmT_section_EU_si[] = {
{u8"DIN 1025-2:1995-11; Euronorm 53-62",L' ',0,&mT_section_EU_si_0},
{u8"DIN 1025-3:1994-03; Euronorm 53-62",L' ',0,&mT_section_EU_si_1},
{u8"DIN 1025-4:1994-03; Euronorm 53-62",L' ',0,&mT_section_EU_si_2},
{u8"DIN 1025-5:1994-03; Euronorm 19-57",L' ',0,&mT_section_EU_si_3},
{u8"EN 10055:1995; DIN 1024:1982-03",L' ',0,&mT_section_EU_si_4},
{u8"Euronorm 55-80",L' ',0,&mT_section_EU_si_5},
};

TMENU mT_section_EU_si = { 6,0,0,35,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmT_section_EU_si,NULL,NULL };

