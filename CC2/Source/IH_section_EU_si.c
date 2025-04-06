#include "IH_section_EU_si_standards.c"

POLE pmIH_section_EU_si[] = {
{u8"Euronorm 53-62; DIN 1025-4",L' ',0,&mIH_section_EU_si_0},
{u8"Euronorm 53-62; DIN 1025-3",L' ',0,&mIH_section_EU_si_1},
{u8"Euronorm 53-62; DIN 1025-2",L' ',0,&mIH_section_EU_si_2},
{u8"Euronorm 19-57; DIN 1025-5; ASTM A 6/A 6M",L' ',0,&mIH_section_EU_si_3},
{u8"EN 10365:2017",L' ',0,&mIH_section_EU_si_4},
{u8"EN 10024",L' ',0,&mIH_section_EU_si_5},
{u8"DIN 1025-5:1994-03; Euronorm 19-57",L' ',0,&mIH_section_EU_si_6},
{u8"DIN 1025-4:1994-03; Euronorm 53-62",L' ',0,&mIH_section_EU_si_7},
{u8"DIN 1025-3:1994-03; Euronorm 53-62",L' ',0,&mIH_section_EU_si_8},
{u8"DIN 1025-2:1995-11; Euronorm 53-62",L' ',0,&mIH_section_EU_si_9},
};

TMENU mIH_section_EU_si = { 10,0,0,42,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmIH_section_EU_si,NULL,NULL };

