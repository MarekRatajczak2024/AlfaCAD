#include "timber_beams_US_si_standards.c"

POLE pmtimber_beams_US_si[] = {
{u8"ANSI/AWC NDS-2018",L' ',0,&mtimber_beams_US_si_0},
{u8"ANSI/AWC NDS-2024",L' ',0,&mtimber_beams_US_si_1},
};

TMENU mtimber_beams_US_si = { 2,0,0,18,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmtimber_beams_US_si,NULL,NULL };

