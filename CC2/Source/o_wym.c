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

#include<forwin.h>
#include<stddef.h>
#define ALLEGWIN
#include <allegext.h>
#include<math.h>
#include<string.h>
#ifndef LINUX
#include<dos.h>
#endif
#include<stdlib.h>
#include<stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "alffont.h"
#include "o_object.h"
#include "o_osnap.h"
#include "b_textw.h"
#include "bib_edit.h"
#include "alf_res.h"
#include "o_luk.h"

extern BOOL Semaphore;


 #include "bibwym_b.c"
void Wymiarowanie(void)
  {
    Semaphore = FALSE;
    wymiarowanie();
    CUR_OFF(X, Y);
    CUR_ON(X,Y);
  }

