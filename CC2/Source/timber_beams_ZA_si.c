#include "timber_beams_ZA_si_standards.c"

POLE pmtimber_beams_ZA_si[] = {
{u8"SANS 10163",L' ',0,&mtimber_beams_ZA_si_0},
};

TMENU mtimber_beams_ZA_si = { 1,0,0,11,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmtimber_beams_ZA_si,NULL,NULL };

