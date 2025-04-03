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

PPOLE pmZ_section_CN_si_0_0[] = {
{u8"JL-ZJ 120x1.5",L' ',0,NULL},
{u8"JL-ZJ 120x1.8",L' ',0,NULL},
{u8"JL-ZJ 120x2",L' ',0,NULL},
{u8"JL-ZJ 120x2.2",L' ',0,NULL},
{u8"JL-ZJ 120x2.5",L' ',0,NULL},
{u8"JL-ZJ 120x2.75",L' ',0,NULL},
{u8"JL-ZJ 120x3",L' ',0,NULL},
{u8"JL-ZJ 140x1.5",L' ',0,NULL},
{u8"JL-ZJ 140x1.8",L' ',0,NULL},
{u8"JL-ZJ 140x2",L' ',0,NULL},
{u8"JL-ZJ 140x2.2",L' ',0,NULL},
{u8"JL-ZJ 140x2.5",L' ',0,NULL},
{u8"JL-ZJ 140x2.75",L' ',0,NULL},
{u8"JL-ZJ 140x3",L' ',0,NULL},
{u8"JL-ZJ 160x1.5",L' ',0,NULL},
{u8"JL-ZJ 160x1.8",L' ',0,NULL},
{u8"JL-ZJ 160x2",L' ',0,NULL},
{u8"JL-ZJ 160x2.2",L' ',0,NULL},
{u8"JL-ZJ 160x2.5",L' ',0,NULL},
{u8"JL-ZJ 160x2.75",L' ',0,NULL},
{u8"JL-ZJ 160x3",L' ',0,NULL},
{u8"JL-ZJ 180x1.5",L' ',0,NULL},
{u8"JL-ZJ 180x1.8",L' ',0,NULL},
{u8"JL-ZJ 180x2",L' ',0,NULL},
{u8"JL-ZJ 180x2.2",L' ',0,NULL},
{u8"JL-ZJ 180x2.5",L' ',0,NULL},
{u8"JL-ZJ 180x2.75",L' ',0,NULL},
{u8"JL-ZJ 180x3",L' ',0,NULL},
{u8"JL-ZJ 200x2",L' ',0,NULL},
{u8"JL-ZJ 200x2.2",L' ',0,NULL},
{u8"JL-ZJ 200x2.5",L' ',0,NULL},
{u8"JL-ZJ 200x2.75",L' ',0,NULL},
{u8"JL-ZJ 200x3",L' ',0,NULL},
{u8"JL-ZJ 220x2",L' ',0,NULL},
{u8"JL-ZJ 220x2.2",L' ',0,NULL},
{u8"JL-ZJ 220x2.5",L' ',0,NULL},
{u8"JL-ZJ 220x2.75",L' ',0,NULL},
{u8"JL-ZJ 220x3",L' ',0,NULL},
{u8"JL-ZJ 250x2.2",L' ',0,NULL},
{u8"JL-ZJ 250x2.5",L' ',0,NULL},
{u8"JL-ZJ 250x2.75",L' ',0,NULL},
{u8"JL-ZJ 250x3",L' ',0,NULL},
{u8"JL-ZJ 280x2.2",L' ',0,NULL},
{u8"JL-ZJ 280x2.5",L' ',0,NULL},
{u8"JL-ZJ 280x2.75",L' ',0,NULL},
{u8"JL-ZJ 280x3",L' ',0,NULL},
{u8"JL-ZJ 300x2.2",L' ',0,NULL},
{u8"JL-ZJ 300x2.5",L' ',0,NULL},
{u8"JL-ZJ 300x2.75",L' ',0,NULL},
{u8"JL-ZJ 300x3",L' ',0,NULL},
};

PTMENU mZ_section_CN_si_0_0 = { 50,0,0,15,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmZ_section_CN_si_0_0,NULL,NULL };

PPOLE pmZ_section_CN_si_1_0[] = {
{u8"Z45 140x2",L' ',0,NULL},
{u8"Z45 140x2.2",L' ',0,NULL},
{u8"Z45 140x2.5",L' ',0,NULL},
{u8"Z45 160x2",L' ',0,NULL},
{u8"Z45 160x2.2",L' ',0,NULL},
{u8"Z45 160x2.5",L' ',0,NULL},
{u8"Z45 180x2",L' ',0,NULL},
{u8"Z45 180x2.2",L' ',0,NULL},
{u8"Z45 180x2.5",L' ',0,NULL},
{u8"Z45 200x2",L' ',0,NULL},
{u8"Z45 200x2.2",L' ',0,NULL},
{u8"Z45 200x2.5",L' ',0,NULL},
{u8"Z45 220x2",L' ',0,NULL},
{u8"Z45 220x2.2",L' ',0,NULL},
{u8"Z45 220x2.5",L' ',0,NULL},
{u8"Z45 250x2",L' ',0,NULL},
{u8"Z45 250x2.2",L' ',0,NULL},
{u8"Z45 250x2.5",L' ',0,NULL},
};

PTMENU mZ_section_CN_si_1_0 = { 18,0,0,12,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmZ_section_CN_si_1_0,NULL,NULL };

PPOLE pmZ_section_CN_si_1_1[] = {
{u8"Z90 100x40x2",L' ',0,NULL},
{u8"Z90 100x40x2.5",L' ',0,NULL},
{u8"Z90 120x50x2",L' ',0,NULL},
{u8"Z90 120x50x2.5",L' ',0,NULL},
{u8"Z90 120x50x3",L' ',0,NULL},
{u8"Z90 140x50x2.5",L' ',0,NULL},
{u8"Z90 140x50x3",L' ',0,NULL},
{u8"Z90 160x60x2.5",L' ',0,NULL},
{u8"Z90 160x60x3",L' ',0,NULL},
{u8"Z90 160x70x2.5",L' ',0,NULL},
{u8"Z90 160x70x3",L' ',0,NULL},
{u8"Z90 180x70x2.5",L' ',0,NULL},
{u8"Z90 180x70x3",L' ',0,NULL},
};

PTMENU mZ_section_CN_si_1_1 = { 13,0,0,15,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmZ_section_CN_si_1_1,NULL,NULL };

