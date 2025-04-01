#include "timber_beams_CA_si_standards.c"

POLE pmtimber_beams_CA_si[] = {
{u8"CSA O86-19",L' ',0,&mtimber_beams_CA_si_0},
};

TMENU mtimber_beams_CA_si = { 1,0,0,11,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmtimber_beams_CA_si,NULL,NULL };

