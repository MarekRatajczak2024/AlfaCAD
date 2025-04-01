#include "timber_beams_CA_si_series.c"

POLE pmtimber_beams_CA_si_0[] = {
{u8"Glulam",L' ',0,&mtimber_beams_CA_si_0_0},
{u8"Machine Stress-Rated Lumber",L' ',0,&mtimber_beams_CA_si_0_1},
{u8"Sawn Lumber",L' ',0,&mtimber_beams_CA_si_0_2},
{u8"Sawn Timbers",L' ',0,&mtimber_beams_CA_si_0_3},
};

TMENU mtimber_beams_CA_si_0 = { 4,0,0,28,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmtimber_beams_CA_si_0,NULL,NULL };

