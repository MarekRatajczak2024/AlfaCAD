/*   ______   ___       ___         ____     ______  ____
*   /\  _  \ /\_ \    /'___\       /\  _`\  /\  _  \/\  _`\
*   \ \ \L\ \\//\ \  /\ \__/   __  \ \ \/\_\\ \ \L\ \ \ \/\ \
*    \ \  __ \ \ \ \ \ \ ,__\/'__`\ \ \ \/_/_\ \  __ \ \ \ \ \
*     \ \ \/\ \ \_\ \_\ \ \_/\ \L\.\_\ \ \L\ \\ \ \/\ \ \ \_\ \
*      \ \_\ \_\/\____\\ \_\\ \__/.\_\\ \____/ \ \_\ \_\ \____/
*       \/_/\/_/\/____/ \/_/ \/__/\/_/ \/___/   \/_/\/_/\/___/
*
*   A CAD
*
*   By Marek Ratajczak 2024
*
*   See readme_alfacad.txt for copyright information.
*
*/

#include <stdio.h>
#include <forwin.h>
#include "bib_e.h"

PPOLE pmtimber_beams_ZA_si_0_0[] = {
{u8"SA Pine 36x36",L' ',0,NULL},
{u8"SA Pine 36x48",L' ',0,NULL},
{u8"SA Pine 36x73",L' ',0,NULL},
{u8"SA Pine 36x111",L' ',0,NULL},
{u8"SA Pine 36x149",L' ',0,NULL},
{u8"SA Pine 36x225",L' ',0,NULL},
{u8"SA Pine 48x48",L' ',0,NULL},
{u8"SA Pine 48x73",L' ',0,NULL},
{u8"SA Pine 48x111",L' ',0,NULL},
{u8"SA Pine 48x149",L' ',0,NULL},
{u8"SA Pine 48x225",L' ',0,NULL},
{u8"SA Pine 73x73",L' ',0,NULL},
{u8"SA Pine 73x111",L' ',0,NULL},
{u8"SA Pine 73x149",L' ',0,NULL},
{u8"SA Pine 73x225",L' ',0,NULL},
};

PTMENU mtimber_beams_ZA_si_0_0 = { 15,0,0,15,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmtimber_beams_ZA_si_0_0,NULL,NULL };

PPOLE pmtimber_beams_ZA_si_0_1[] = {
{u8"SA Pine Stocklam 32x100",L' ',0,NULL},
{u8"SA Pine Stocklam 32x133",L' ',0,NULL},
{u8"SA Pine Stocklam 32x167",L' ',0,NULL},
{u8"SA Pine Stocklam 32x200",L' ',0,NULL},
{u8"SA Pine Stocklam 45x100",L' ',0,NULL},
{u8"SA Pine Stocklam 45x133",L' ',0,NULL},
{u8"SA Pine Stocklam 45x167",L' ',0,NULL},
{u8"SA Pine Stocklam 45x200",L' ',0,NULL},
{u8"SA Pine Stocklam 70x100",L' ',0,NULL},
{u8"SA Pine Stocklam 70x133",L' ',0,NULL},
{u8"SA Pine Stocklam 70x167",L' ',0,NULL},
{u8"SA Pine Stocklam 70x200",L' ',0,NULL},
{u8"SA Pine Stocklam 70x233",L' ',0,NULL},
{u8"SA Pine Stocklam 70x267",L' ',0,NULL},
{u8"SA Pine Stocklam 70x300",L' ',0,NULL},
{u8"SA Pine Stocklam 100x100",L' ',0,NULL},
{u8"SA Pine Stocklam 100x133",L' ',0,NULL},
{u8"SA Pine Stocklam 100x167",L' ',0,NULL},
{u8"SA Pine Stocklam 100x200",L' ',0,NULL},
{u8"SA Pine Stocklam 100x233",L' ',0,NULL},
{u8"SA Pine Stocklam 100x267",L' ',0,NULL},
{u8"SA Pine Stocklam 100x300",L' ',0,NULL},
{u8"SA Pine Stocklam 100x333",L' ',0,NULL},
{u8"SA Pine Stocklam 100x367",L' ',0,NULL},
{u8"SA Pine Stocklam 100x400",L' ',0,NULL},
{u8"SA Pine Stocklam 140x100",L' ',0,NULL},
{u8"SA Pine Stocklam 140x133",L' ',0,NULL},
{u8"SA Pine Stocklam 140x167",L' ',0,NULL},
{u8"SA Pine Stocklam 140x200",L' ',0,NULL},
{u8"SA Pine Stocklam 140x233",L' ',0,NULL},
{u8"SA Pine Stocklam 140x267",L' ',0,NULL},
{u8"SA Pine Stocklam 140x300",L' ',0,NULL},
{u8"SA Pine Stocklam 140x333",L' ',0,NULL},
{u8"SA Pine Stocklam 140x367",L' ',0,NULL},
{u8"SA Pine Stocklam 140x400",L' ',0,NULL},
{u8"SA Pine Stocklam 140x433",L' ',0,NULL},
{u8"SA Pine Stocklam 140x467",L' ',0,NULL},
{u8"SA Pine Stocklam 140x500",L' ',0,NULL},
{u8"SA Pine Stocklam 140x533",L' ',0,NULL},
{u8"SA Pine Stocklam 140x567",L' ',0,NULL},
{u8"SA Pine Stocklam 140x600",L' ',0,NULL},
};

PTMENU mtimber_beams_ZA_si_0_1 = { 41,0,0,25,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmtimber_beams_ZA_si_0_1,NULL,NULL };

PPOLE pmtimber_beams_ZA_si_0_2[] = {
{u8"Saligna Glulam 38x110",L' ',0,NULL},
{u8"Saligna Glulam 38x132",L' ',0,NULL},
{u8"Saligna Glulam 38x154",L' ',0,NULL},
{u8"Saligna Glulam 38x176",L' ',0,NULL},
{u8"Saligna Glulam 38x198",L' ',0,NULL},
{u8"Saligna Glulam 50x154",L' ',0,NULL},
{u8"Saligna Glulam 50x176",L' ',0,NULL},
{u8"Saligna Glulam 50x198",L' ',0,NULL},
{u8"Saligna Glulam 50x220",L' ',0,NULL},
{u8"Saligna Glulam 50x242",L' ',0,NULL},
{u8"Saligna Glulam 65x198",L' ',0,NULL},
{u8"Saligna Glulam 65x220",L' ',0,NULL},
{u8"Saligna Glulam 65x242",L' ',0,NULL},
{u8"Saligna Glulam 65x264",L' ',0,NULL},
{u8"Saligna Glulam 65x286",L' ',0,NULL},
{u8"Saligna Glulam 65x308",L' ',0,NULL},
{u8"Saligna Glulam 65x330",L' ',0,NULL},
{u8"Saligna Glulam 75x220",L' ',0,NULL},
{u8"Saligna Glulam 75x264",L' ',0,NULL},
{u8"Saligna Glulam 75x308",L' ',0,NULL},
{u8"Saligna Glulam 75x352",L' ',0,NULL},
{u8"Saligna Glulam 75x396",L' ',0,NULL},
{u8"Saligna Glulam 89x286",L' ',0,NULL},
{u8"Saligna Glulam 89x330",L' ',0,NULL},
{u8"Saligna Glulam 89x374",L' ',0,NULL},
{u8"Saligna Glulam 89x418",L' ',0,NULL},
{u8"Saligna Glulam 89x462",L' ',0,NULL},
{u8"Saligna Glulam 114x352",L' ',0,NULL},
{u8"Saligna Glulam 114x396",L' ',0,NULL},
{u8"Saligna Glulam 114x440",L' ',0,NULL},
{u8"Saligna Glulam 114x484",L' ',0,NULL},
{u8"Saligna Glulam 114x528",L' ',0,NULL},
{u8"Saligna Glulam 114x572",L' ',0,NULL},
{u8"Saligna Glulam 140x418",L' ',0,NULL},
{u8"Saligna Glulam 140x451",L' ',0,NULL},
{u8"Saligna Glulam 140x485",L' ',0,NULL},
{u8"Saligna Glulam 140x518",L' ',0,NULL},
{u8"Saligna Glulam 140x551",L' ',0,NULL},
{u8"Saligna Glulam 140x585",L' ',0,NULL},
{u8"Saligna Glulam 140x618",L' ',0,NULL},
{u8"Saligna Glulam 140x651",L' ',0,NULL},
{u8"Saligna Glulam 140x685",L' ',0,NULL},
{u8"Saligna Glulam 140x718",L' ',0,NULL},
{u8"Saligna Glulam 160x485",L' ',0,NULL},
{u8"Saligna Glulam 160x518",L' ',0,NULL},
{u8"Saligna Glulam 160x552",L' ',0,NULL},
{u8"Saligna Glulam 160x585",L' ',0,NULL},
{u8"Saligna Glulam 160x618",L' ',0,NULL},
{u8"Saligna Glulam 160x652",L' ',0,NULL},
{u8"Saligna Glulam 160x685",L' ',0,NULL},
{u8"Saligna Glulam 160x718",L' ',0,NULL},
{u8"Saligna Glulam 160x752",L' ',0,NULL},
{u8"Saligna Glulam 160x785",L' ',0,NULL},
{u8"Saligna Glulam 160x818",L' ',0,NULL},
{u8"Saligna Glulam 160x852",L' ',0,NULL},
{u8"Saligna Glulam 160x885",L' ',0,NULL},
};

PTMENU mtimber_beams_ZA_si_0_2 = { 56,0,0,23,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmtimber_beams_ZA_si_0_2,NULL,NULL };

