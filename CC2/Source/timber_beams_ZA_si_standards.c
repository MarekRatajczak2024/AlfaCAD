#include "timber_beams_ZA_si_series.c"

POLE pmtimber_beams_ZA_si_0[] = {
{u8"SA Pine\0\0",L' ',0,&mtimber_beams_ZA_si_0_0},
{u8"SA Pine Stocklam\0\0",L' ',0,&mtimber_beams_ZA_si_0_1},
{u8"Saligna Glulam\0\0",L' ',0,&mtimber_beams_ZA_si_0_2},
};

TMENU mtimber_beams_ZA_si_0 = { 3,0,0,21,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmtimber_beams_ZA_si_0,NULL,NULL };

