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

PPOLE pmU_section_AU_si_0_0[] = {
{u8"PFC 75x40x6",L' ',0,NULL},
{u8"PFC 100x50x10",L' ',0,NULL},
{u8"PFC 125x65x12",L' ',0,NULL},
};

PTMENU mU_section_AU_si_0_0 = { 3,0,0,14,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmU_section_AU_si_0_0,NULL,NULL };

PPOLE pmU_section_AU_si_0_1[] = {
{u8"PFC 150x75x18",L' ',0,NULL},
{u8"PFC 180x75x20",L' ',0,NULL},
{u8"PFC 200x75x23",L' ',0,NULL},
{u8"PFC 230x75x25",L' ',0,NULL},
{u8"PFC 250x90x36",L' ',0,NULL},
};

PTMENU mU_section_AU_si_0_1 = { 5,0,0,14,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmU_section_AU_si_0_1,NULL,NULL };

PPOLE pmU_section_AU_si_0_2[] = {
{u8"PFC 300x90x40",L' ',0,NULL},
{u8"PFC 380x100x55",L' ',0,NULL},
};

PTMENU mU_section_AU_si_0_2 = { 2,0,0,15,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmU_section_AU_si_0_2,NULL,NULL };

PPOLE pmU_section_AU_si_0_3[] = {
{u8"PFC 150x75x17.7",L' ',0,NULL},
{u8"PFC 180x75x20.9",L' ',0,NULL},
{u8"PFC 200x75x22.9",L' ',0,NULL},
{u8"PFC 230x75x25.1",L' ',0,NULL},
{u8"PFC 250x90x35.5",L' ',0,NULL},
{u8"PFC 300x90x40.1",L' ',0,NULL},
{u8"PFC 380x100x55.2",L' ',0,NULL},
};

PTMENU mU_section_AU_si_0_3 = { 7,0,0,17,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmU_section_AU_si_0_3,NULL,NULL };

