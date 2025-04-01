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

PPOLE pmCT_section_AU_si_0_0[] = {
{u8"Galtube CHS 76.1 x 2.6",L' ',0,NULL},
{u8"Galtube CHS 60.3 x 2.3",L' ',0,NULL},
{u8"Galtube CHS 48.3 x 2.3",L' ',0,NULL},
{u8"Galtube CHS 42.4 x 2.0",L' ',0,NULL},
{u8"Galtube CHS 33.7 x 2.0",L' ',0,NULL},
{u8"Galtube CHS 26.9 x 2.0",L' ',0,NULL},
};

PTMENU mCT_section_AU_si_0_0 = { 6,0,0,23,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmCT_section_AU_si_0_0,NULL,NULL };

PPOLE pmCT_section_AU_si_1_0[] = {
{u8"DuraGal CHS 76.1 x 2.3",L' ',0,NULL},
{u8"DuraGal CHS 60.3 x 2.3",L' ',0,NULL},
{u8"DuraGal CHS 48.3 x 2.3",L' ',0,NULL},
{u8"DuraGal CHS 42.4 x 2.0",L' ',0,NULL},
{u8"DuraGal CHS 33.7 x 2.0",L' ',0,NULL},
{u8"DuraGal CHS 26.9 x 2.0",L' ',0,NULL},
};

PTMENU mCT_section_AU_si_1_0 = { 6,0,0,23,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmCT_section_AU_si_1_0,NULL,NULL };

PPOLE pmCT_section_AU_si_1_1[] = {
{u8"Tubeline CHS 457.0 x 12.7",L' ',0,NULL},
{u8"Tubeline CHS 457.0 x 9.5",L' ',0,NULL},
{u8"Tubeline CHS 457.0 x 6.4",L' ',0,NULL},
{u8"Tubeline CHS 406.4 x 12.7",L' ',0,NULL},
{u8"Tubeline CHS 406.4 x 9.5",L' ',0,NULL},
{u8"Tubeline CHS 406.4 x 6.4",L' ',0,NULL},
{u8"Tubeline CHS 355.6 x 12.7",L' ',0,NULL},
{u8"Tubeline CHS 355.6 x 9.5",L' ',0,NULL},
{u8"Tubeline CHS 355.6 x 6.4",L' ',0,NULL},
{u8"Tubeline CHS 323.9 x 12.7",L' ',0,NULL},
{u8"Tubeline CHS 323.9 x 9.5",L' ',0,NULL},
{u8"Tubeline CHS 323.9 x 6.4",L' ',0,NULL},
{u8"Tubeline CHS 273.1 x 12.7",L' ',0,NULL},
{u8"Tubeline CHS 273.1 x 9.3",L' ',0,NULL},
{u8"Tubeline CHS 273.1 x 6.4",L' ',0,NULL},
{u8"Tubeline CHS 273.1 x 4.8",L' ',0,NULL},
{u8"Tubeline CHS 219.1 x 8.2",L' ',0,NULL},
{u8"Tubeline CHS 219.1 x 6.4",L' ',0,NULL},
{u8"Tubeline CHS 219.1 x 4.8",L' ',0,NULL},
{u8"Tubeline CHS 168.3 x 7.1",L' ',0,NULL},
{u8"Tubeline CHS 168.3 x 6.4",L' ',0,NULL},
{u8"Tubeline CHS 168.3 x 4.8",L' ',0,NULL},
{u8"Tubeline CHS 165.1 x 5.4",L' ',0,NULL},
{u8"Tubeline CHS 165.1 x 5.0",L' ',0,NULL},
{u8"Tubeline CHS 165.1 x 3.5",L' ',0,NULL},
{u8"Tubeline CHS 139.7 x 5.4",L' ',0,NULL},
{u8"Tubeline CHS 139.7 x 5.0",L' ',0,NULL},
{u8"Tubeline CHS 139.7 x 3.5",L' ',0,NULL},
{u8"Tubeline CHS 139.7 x 3.0",L' ',0,NULL},
{u8"Tubeline CHS 114.3 x 5.4",L' ',0,NULL},
{u8"Tubeline CHS 114.3 x 4.5",L' ',0,NULL},
{u8"Tubeline CHS 114.3 x 3.6",L' ',0,NULL},
{u8"Tubeline CHS 114.3 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 101.6 x 5.0",L' ',0,NULL},
{u8"Tubeline CHS 101.6 x 4.0",L' ',0,NULL},
{u8"Tubeline CHS 101.6 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 101.6 x 2.6",L' ',0,NULL},
{u8"Tubeline CHS 88.9 x 5.9",L' ',0,NULL},
{u8"Tubeline CHS 88.9 x 5.0",L' ',0,NULL},
{u8"Tubeline CHS 88.9 x 4.0",L' ',0,NULL},
{u8"Tubeline CHS 88.9 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 88.9 x 2.6",L' ',0,NULL},
{u8"Tubeline CHS 76.1 x 5.9",L' ',0,NULL},
{u8"Tubeline CHS 76.1 x 4.5",L' ',0,NULL},
{u8"Tubeline CHS 76.1 x 3.6",L' ',0,NULL},
{u8"Tubeline CHS 76.1 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 76.1 x 2.3",L' ',0,NULL},
{u8"Tubeline CHS 60.3 x 5.4",L' ',0,NULL},
{u8"Tubeline CHS 60.3 x 4.5",L' ',0,NULL},
{u8"Tubeline CHS 60.3 x 3.6",L' ',0,NULL},
{u8"Tubeline CHS 60.3 x 2.9",L' ',0,NULL},
{u8"Tubeline CHS 60.3 x 2.3",L' ',0,NULL},
{u8"Tubeline CHS 48.3 x 5.4",L' ',0,NULL},
{u8"Tubeline CHS 48.3 x 4.0",L' ',0,NULL},
{u8"Tubeline CHS 48.3 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 48.3 x 2.9",L' ',0,NULL},
{u8"Tubeline CHS 48.3 x 2.3",L' ',0,NULL},
{u8"Tubeline CHS 42.4 x 4.0",L' ',0,NULL},
{u8"Tubeline CHS 42.4 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 42.4 x 2.6",L' ',0,NULL},
{u8"Tubeline CHS 42.4 x 2.0",L' ',0,NULL},
{u8"Tubeline CHS 33.7 x 4.0",L' ',0,NULL},
{u8"Tubeline CHS 33.7 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 33.7 x 2.6",L' ',0,NULL},
{u8"Tubeline CHS 33.7 x 2.0",L' ',0,NULL},
{u8"Tubeline CHS 26.9 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 26.9 x 2.6",L' ',0,NULL},
{u8"Tubeline CHS 26.9 x 2.3",L' ',0,NULL},
{u8"Tubeline CHS 26.9 x 2.0",L' ',0,NULL},
{u8"Tubeline CHS 21.3 x 3.2",L' ',0,NULL},
{u8"Tubeline CHS 21.3 x 2.6",L' ',0,NULL},
{u8"Tubeline CHS 21.3 x 2.0",L' ',0,NULL},
};

PTMENU mCT_section_AU_si_1_1 = { 72,0,0,26,16,8,FIXED,CMNU,CMBR,CMTX,0,0,0,0,0,(PPOLE(*)[]) &pmCT_section_AU_si_1_1,NULL,NULL };

