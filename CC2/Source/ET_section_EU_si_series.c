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

PPOLE pmET_section_EU_si_0_0[] = {
{u8"SEHS 60x60x3.5",L' ',0,NULL},
{u8"SEHS 60x60x4",L' ',0,NULL},
{u8"SEHS 60x60x5",L' ',0,NULL},
{u8"SEHS 60x60x6",L' ',0,NULL},
{u8"SEHS 75x75x4",L' ',0,NULL},
{u8"SEHS 75x75x5",L' ',0,NULL},
{u8"SEHS 75x75x6",L' ',0,NULL},
{u8"SEHS 75x75x8",L' ',0,NULL},
{u8"SEHS 90x90x4",L' ',0,NULL},
{u8"SEHS 90x90x5",L' ',0,NULL},
{u8"SEHS 90x90x6",L' ',0,NULL},
{u8"SEHS 90x90x8",L' ',0,NULL},
{u8"SEHS 110x110x4",L' ',0,NULL},
{u8"SEHS 110x110x5",L' ',0,NULL},
{u8"SEHS 110x110x6",L' ',0,NULL},
{u8"SEHS 110x110x8",L' ',0,NULL},
{u8"SEHS 125x125x5",L' ',0,NULL},
{u8"SEHS 125x125x6",L' ',0,NULL},
{u8"SEHS 125x125x8",L' ',0,NULL},
{u8"SEHS 125x125x10",L' ',0,NULL},
{u8"SEHS 160x160x5",L' ',0,NULL},
{u8"SEHS 160x160x6",L' ',0,NULL},
{u8"SEHS 160x160x8",L' ',0,NULL},
{u8"SEHS 160x160x10",L' ',0,NULL},
{u8"SEHS 180x180x6",L' ',0,NULL},
{u8"SEHS 180x180x8",L' ',0,NULL},
{u8"SEHS 180x180x10",L' ',0,NULL},
{u8"SEHS 180x180x12",L' ',0,NULL},
{u8"SEHS 203x223x6",L' ',0,NULL},
{u8"SEHS 203x223x8",L' ',0,NULL},
{u8"SEHS 203x223x10",L' ',0,NULL},
{u8"SEHS 203x223x12",L' ',0,NULL},
{u8"SEHS 225x259x6",L' ',0,NULL},
{u8"SEHS 225x259x8",L' ',0,NULL},
{u8"SEHS 225x259x10",L' ',0,NULL},
{u8"SEHS 225x259x12",L' ',0,NULL},
};

PTMENU mET_section_EU_si_0_0 = { 36,0,0,16,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmET_section_EU_si_0_0,NULL,NULL };

PPOLE pmET_section_EU_si_0_1[] = {
{u8"SEHS 203x223x5.0",L' ',0,NULL},
{u8"SEHS 203x223x6.3",L' ',0,NULL},
{u8"SEHS 203x223x8.0",L' ',0,NULL},
{u8"SEHS 203x223x10.0",L' ',0,NULL},
{u8"SEHS 203x223x12.5",L' ',0,NULL},
{u8"SEHS 225x259x5.0",L' ',0,NULL},
{u8"SEHS 225x259x6.3",L' ',0,NULL},
{u8"SEHS 225x259x8.0",L' ',0,NULL},
{u8"SEHS 225x259x10.0",L' ',0,NULL},
{u8"SEHS 225x259x12.5",L' ',0,NULL},
{u8"SEHS 252x276x5.0",L' ',0,NULL},
{u8"SEHS 252x276x6.3",L' ',0,NULL},
{u8"SEHS 252x276x8.0",L' ',0,NULL},
{u8"SEHS 252x276x10.0",L' ',0,NULL},
{u8"SEHS 252x276x12.5",L' ',0,NULL},
{u8"SEHS 252x276x14.2",L' ',0,NULL},
{u8"SEHS 280x322x5.0",L' ',0,NULL},
{u8"SEHS 280x322x6.3",L' ',0,NULL},
{u8"SEHS 280x322x8.0",L' ',0,NULL},
{u8"SEHS 280x322x10.0",L' ',0,NULL},
{u8"SEHS 280x322x12.5",L' ',0,NULL},
{u8"SEHS 280x322x14.2",L' ',0,NULL},
{u8"SEHS 324x375x6.3",L' ',0,NULL},
{u8"SEHS 324x375x8.0",L' ',0,NULL},
{u8"SEHS 324x375x10.0",L' ',0,NULL},
{u8"SEHS 324x375x12.5",L' ',0,NULL},
{u8"SEHS 324x375x14.2",L' ',0,NULL},
};

PTMENU mET_section_EU_si_0_1 = { 27,0,0,18,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmET_section_EU_si_0_1,NULL,NULL };

PPOLE pmET_section_EU_si_0_2[] = {
{u8"EHS 117x60x3.2",L' ',0,NULL},
{u8"EHS 117x60x4.0",L' ',0,NULL},
{u8"EHS 117x60x5.0",L' ',0,NULL},
{u8"EHS 117x60x6.0",L' ',0,NULL},
{u8"EHS 117x60x8.0",L' ',0,NULL},
{u8"EHS 149x70x4.0",L' ',0,NULL},
{u8"EHS 149x70x5.0",L' ',0,NULL},
{u8"EHS 149x70x6.0",L' ',0,NULL},
{u8"EHS 149x70x8.0",L' ',0,NULL},
{u8"EHS 149x70x10.0",L' ',0,NULL},
{u8"EHS 178x90x6.0",L' ',0,NULL},
{u8"EHS 178x90x8.0",L' ',0,NULL},
{u8"EHS 178x90x10.0",L' ',0,NULL},
{u8"EHS 220x106x6.0",L' ',0,NULL},
{u8"EHS 220x106x8.0",L' ',0,NULL},
{u8"EHS 220x106x10.0",L' ',0,NULL},
{u8"EHS 248x109x6.0",L' ',0,NULL},
{u8"EHS 248x109x8.0",L' ',0,NULL},
{u8"EHS 248x109x10.0",L' ',0,NULL},
{u8"EHS 248x109x12.0",L' ',0,NULL},
{u8"EHS 320x160x8.0",L' ',0,NULL},
{u8"EHS 320x160x10.0",L' ',0,NULL},
{u8"EHS 320x160x12.0",L' ',0,NULL},
{u8"EHS 320x160x14.0",L' ',0,NULL},
{u8"EHS 400x200x8.0",L' ',0,NULL},
{u8"EHS 400x200x10.0",L' ',0,NULL},
{u8"EHS 400x200x12.0",L' ',0,NULL},
{u8"EHS 400x200x14.0",L' ',0,NULL},
{u8"EHS 480x240x10.0",L' ',0,NULL},
{u8"EHS 480x240x12.0",L' ',0,NULL},
{u8"EHS 480x240x14.0",L' ',0,NULL},
};

PTMENU mET_section_EU_si_0_2 = { 31,0,0,17,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmET_section_EU_si_0_2,NULL,NULL };

PPOLE pmET_section_EU_si_0_3[] = {
{u8"EHS 120x60x3.2",L' ',0,NULL},
{u8"EHS 120x60x4",L' ',0,NULL},
{u8"EHS 120x60x5",L' ',0,NULL},
{u8"EHS 120x60x6.3",L' ',0,NULL},
{u8"EHS 120x60x8",L' ',0,NULL},
{u8"EHS 150x75x4",L' ',0,NULL},
{u8"EHS 150x75x5",L' ',0,NULL},
{u8"EHS 150x75x6.3",L' ',0,NULL},
{u8"EHS 150x75x8",L' ',0,NULL},
{u8"EHS 180x90x5",L' ',0,NULL},
{u8"EHS 180x90x6.3",L' ',0,NULL},
{u8"EHS 180x90x8",L' ',0,NULL},
{u8"EHS 180x90x10",L' ',0,NULL},
{u8"EHS 220x110x6.3",L' ',0,NULL},
{u8"EHS 220x110x8",L' ',0,NULL},
{u8"EHS 220x110x10",L' ',0,NULL},
{u8"EHS 220x110x12.5",L' ',0,NULL},
{u8"EHS 250x125x6.3",L' ',0,NULL},
{u8"EHS 250x125x8",L' ',0,NULL},
{u8"EHS 250x125x10",L' ',0,NULL},
{u8"EHS 250x125x12.5",L' ',0,NULL},
{u8"EHS 320x160x8",L' ',0,NULL},
{u8"EHS 320x160x10",L' ',0,NULL},
{u8"EHS 320x160x12.5",L' ',0,NULL},
{u8"EHS 320x160x14.2",L' ',0,NULL},
{u8"EHS 400x200x8",L' ',0,NULL},
{u8"EHS 400x200x10",L' ',0,NULL},
{u8"EHS 400x200x12.5",L' ',0,NULL},
{u8"EHS 400x200x14.2",L' ',0,NULL},
{u8"EHS 480x240x8",L' ',0,NULL},
{u8"EHS 480x240x10",L' ',0,NULL},
{u8"EHS 480x240x12.5",L' ',0,NULL},
{u8"EHS 480x240x14.2",L' ',0,NULL},
};

PTMENU mET_section_EU_si_0_3 = { 33,0,0,17,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmET_section_EU_si_0_3,NULL,NULL };

PPOLE pmET_section_EU_si_0_4[] = {
{u8"EHS 120x60x3.2",L' ',0,NULL},
{u8"EHS 120x60x4.0",L' ',0,NULL},
{u8"EHS 120x60x5.0",L' ',0,NULL},
{u8"EHS 120x60x6.3",L' ',0,NULL},
{u8"EHS 120x60x8.0",L' ',0,NULL},
{u8"EHS 150x75x4.0",L' ',0,NULL},
{u8"EHS 150x75x5.0",L' ',0,NULL},
{u8"EHS 150x75x6.3",L' ',0,NULL},
{u8"EHS 150x75x8.0",L' ',0,NULL},
{u8"EHS 150x75x10.0",L' ',0,NULL},
{u8"EHS 180x90x6.3",L' ',0,NULL},
{u8"EHS 180x90x8.0",L' ',0,NULL},
{u8"EHS 180x90x10.0",L' ',0,NULL},
{u8"EHS 220x110x6.3",L' ',0,NULL},
{u8"EHS 220x110x8.0",L' ',0,NULL},
{u8"EHS 220x110x10.0",L' ',0,NULL},
{u8"EHS 250x125x6.3",L' ',0,NULL},
{u8"EHS 250x125x8.0",L' ',0,NULL},
{u8"EHS 250x125x10.0",L' ',0,NULL},
{u8"EHS 250x125x12.5",L' ',0,NULL},
{u8"EHS 320x160x8.0",L' ',0,NULL},
{u8"EHS 320x160x10.0",L' ',0,NULL},
{u8"EHS 320x160x12.5",L' ',0,NULL},
{u8"EHS 320x160x14.2",L' ',0,NULL},
{u8"EHS 400x200x8.0",L' ',0,NULL},
{u8"EHS 400x200x10.0",L' ',0,NULL},
{u8"EHS 400x200x12.5",L' ',0,NULL},
{u8"EHS 400x200x14.2",L' ',0,NULL},
{u8"EHS 480x240x8.0",L' ',0,NULL},
{u8"EHS 480x240x10.0",L' ',0,NULL},
{u8"EHS 480x240x12.5",L' ',0,NULL},
{u8"EHS 480x240x14.2",L' ',0,NULL},
};

PTMENU mET_section_EU_si_0_4 = { 32,0,0,17,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmET_section_EU_si_0_4,NULL,NULL };

PPOLE pmET_section_EU_si_0_5[] = {
{u8"Celsius 355 EHS 150x75x4",L' ',0,NULL},
{u8"Celsius 355 EHS 150x75x5",L' ',0,NULL},
{u8"Celsius 355 EHS 150x75x6.3",L' ',0,NULL},
{u8"Celsius 355 EHS 200x100x5",L' ',0,NULL},
{u8"Celsius 355 EHS 200x100x6.3",L' ',0,NULL},
{u8"Celsius 355 EHS 200x100x8",L' ',0,NULL},
{u8"Celsius 355 EHS 200x100x10",L' ',0,NULL},
{u8"Celsius 355 EHS 200x100x12.5",L' ',0,NULL},
{u8"Celsius 355 EHS 250x125x6.3",L' ',0,NULL},
{u8"Celsius 355 EHS 250x125x8",L' ',0,NULL},
{u8"Celsius 355 EHS 250x125x10",L' ',0,NULL},
{u8"Celsius 355 EHS 250x125x12.5",L' ',0,NULL},
{u8"Celsius 355 EHS 300x150x8",L' ',0,NULL},
{u8"Celsius 355 EHS 300x150x10",L' ',0,NULL},
{u8"Celsius 355 EHS 300x150x12.5",L' ',0,NULL},
{u8"Celsius 355 EHS 300x150x14.2",L' ',0,NULL},
{u8"Celsius 355 EHS 300x150x16",L' ',0,NULL},
{u8"Celsius 355 EHS 400x200x8",L' ',0,NULL},
{u8"Celsius 355 EHS 400x200x10",L' ',0,NULL},
{u8"Celsius 355 EHS 400x200x12.5",L' ',0,NULL},
{u8"Celsius 355 EHS 400x200x14.2",L' ',0,NULL},
{u8"Celsius 355 EHS 400x200x16",L' ',0,NULL},
{u8"Celsius 355 EHS 500x250x10",L' ',0,NULL},
{u8"Celsius 355 EHS 500x250x12.5",L' ',0,NULL},
{u8"Celsius 355 EHS 500x250x14.2",L' ',0,NULL},
{u8"Celsius 355 EHS 500x250x16",L' ',0,NULL},
};

PTMENU mET_section_EU_si_0_5 = { 26,0,0,29,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmET_section_EU_si_0_5,NULL,NULL };

PPOLE pmET_section_EU_si_1_0[] = {
{u8"EHS 120x60x3.2",L' ',0,NULL},
{u8"EHS 120x60x4",L' ',0,NULL},
{u8"EHS 120x60x5",L' ',0,NULL},
{u8"EHS 120x60x6",L' ',0,NULL},
{u8"EHS 120x60x8",L' ',0,NULL},
{u8"EHS 150x75x4",L' ',0,NULL},
{u8"EHS 150x75x5",L' ',0,NULL},
{u8"EHS 150x75x6",L' ',0,NULL},
{u8"EHS 150x75x6.3",L' ',0,NULL},
{u8"EHS 150x75x8",L' ',0,NULL},
{u8"EHS 150x75x10",L' ',0,NULL},
{u8"EHS 180x90x6",L' ',0,NULL},
{u8"EHS 180x90x8",L' ',0,NULL},
{u8"EHS 180x90x10",L' ',0,NULL},
{u8"EHS 200x100x6.3",L' ',0,NULL},
{u8"EHS 200x100x8",L' ',0,NULL},
{u8"EHS 200x100x10",L' ',0,NULL},
{u8"EHS 200x100x12.5",L' ',0,NULL},
{u8"EHS 220x110x6",L' ',0,NULL},
{u8"EHS 220x110x8",L' ',0,NULL},
{u8"EHS 220x110x10",L' ',0,NULL},
{u8"EHS 250x125x6",L' ',0,NULL},
{u8"EHS 250x125x6.3",L' ',0,NULL},
{u8"EHS 250x125x8",L' ',0,NULL},
{u8"EHS 250x125x10",L' ',0,NULL},
{u8"EHS 250x125x12",L' ',0,NULL},
{u8"EHS 250x125x12.5",L' ',0,NULL},
{u8"EHS 300x150x8",L' ',0,NULL},
{u8"EHS 300x150x10",L' ',0,NULL},
{u8"EHS 300x150x12.5",L' ',0,NULL},
{u8"EHS 300x150x16",L' ',0,NULL},
{u8"EHS 320x160x8",L' ',0,NULL},
{u8"EHS 320x160x10",L' ',0,NULL},
{u8"EHS 320x160x12",L' ',0,NULL},
{u8"EHS 320x160x14",L' ',0,NULL},
{u8"EHS 400x200x8",L' ',0,NULL},
{u8"EHS 400x200x10",L' ',0,NULL},
{u8"EHS 400x200x12",L' ',0,NULL},
{u8"EHS 400x200x12.5",L' ',0,NULL},
{u8"EHS 400x200x14",L' ',0,NULL},
{u8"EHS 480x240x10",L' ',0,NULL},
{u8"EHS 480x240x12",L' ',0,NULL},
{u8"EHS 480x240x14",L' ',0,NULL},
{u8"EHS 500x250x10",L' ',0,NULL},
{u8"EHS 500x250x12.5",L' ',0,NULL},
{u8"EHS 500x250x16",L' ',0,NULL},
};

PTMENU mET_section_EU_si_1_0 = { 46,0,0,17,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmET_section_EU_si_1_0,NULL,NULL };

