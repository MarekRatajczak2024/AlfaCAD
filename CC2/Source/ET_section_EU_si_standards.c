#include "ET_section_EU_si_series.c"

POLE pmET_section_EU_si_0[] = {
{u8"SEHS\0™Continental Steel\0",L' ',0,&mET_section_EU_si_0_0},
{u8"SEHS\0™ALUKÖNIGSTAHL\0",L' ',0,&mET_section_EU_si_0_1},
{u8"EHS\0™Continental Steel\0",L' ',0,&mET_section_EU_si_0_2},
{u8"EHS\0™Condesa\0",L' ',0,&mET_section_EU_si_0_3},
{u8"EHS\0™ALUKÖNIGSTAHL\0",L' ',0,&mET_section_EU_si_0_4},
{u8"Celsius 355 EHS\0™Tata Steel\0",L' ',0,&mET_section_EU_si_0_5},
};

TMENU mET_section_EU_si_0 = { 6,0,0,33,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmET_section_EU_si_0,NULL,NULL };

POLE pmET_section_EU_si_1[] = {
{u8"EHS\0\0",L' ',0,&mET_section_EU_si_1_0},
};

TMENU mET_section_EU_si_1 = { 1,0,0,8,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmET_section_EU_si_1,NULL,NULL };

