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

#define __B_HLP__

#include "menu.h"
extern BOOL Semaphore;
extern BOOL Cust_Semaphore;
extern int Expand_flex(void);
extern BOOL Show_Alfa_Instruction(void);

static void noooph(void)
{}
//#define nooph (void (*)(int x,int y))noooph
//#define nooph (void (*)(void))noooph
#define nooph (int (*)(void))noooph

void GetEsc(void)
{ int zn;
  while (1)
   { zn=inkeys(NULL, FALSE);
     if(zn==ESC) break;
   }
}

TMENU mHelp={96,32,32,30,20,4,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmHelp,NULL,NULL};


int Lastfun()
{
	if (Semaphore==TRUE) LASTFUN();
    return 0;
}

int Tabfun()
{
	TABFUN();
    return 0;
}


int KeyE()
{
	//simulate_keypress(_ENDPOINT_);
	simulate_ukeypress(_ENDPOINT_, _ENDPOINT_SC_);
    return 0;
}

int KeyCtrlE(void)
{
	//simulate_keypress(_ENDPOINT_);
	simulate_ukeypress(_ENDPOINT_SC_, _ENDPOINT_SC_);
    return 0;
}


int KeyN(void)
{
	//simulate_keypress(_NEAREST_);
	simulate_ukeypress(_NEAREST_, _NEAREST_SC_);
    return 0;
}

int KeyCtrlN(void)
{
	//simulate_keypress(_NEAREST_);
	simulate_ukeypress(_NEAREST_SC_, _NEAREST_SC_);
    return 0;
}

int KeyM(void)
{
	//simulate_keypress(_MIDDLE_);
	simulate_ukeypress(_MIDDLE_, _MIDDLE_SC_);
    return 0;
}

int KeyCtrlM(void)
{
	//simulate_keypress(_MIDDLE_);
	simulate_ukeypress(_MIDDLE_SC_, _MIDDLE_SC_);
    return 0;
}

int KeyI(void)
{
	//simulate_keypress(_INTERSECTION_);
	simulate_ukeypress(_INTERSECTION_, _INTERSECTION_SC_);
    return 0;
}

int KeyCtrlI(void)
{
	//simulate_keypress(_INTERSECTION_);
	simulate_ukeypress(_INTERSECTION_SC_, _INTERSECTION_SC_);
    return 0;
}

int KeyC(void)
{
	//simulate_keypress(_CENTER_);
	simulate_ukeypress(_CENTER_, _CENTER_SC_);
    return 0;
}

int KeyCtrlC(void)
{
	//simulate_keypress(_CENTER_);
	simulate_ukeypress(_CENTER_SC_, _CENTER_SC_);
    return 0;
}

int KeyP(void)
{
	//simulate_keypress(_PERPENDICULAR_);
	simulate_ukeypress(_PERPENDICULAR_, _PERPENDICULAR_SC_);
    return 0;
}

int KeyCtrlP(void)
{
	//simulate_keypress(_PERPENDICULAR_);
	simulate_ukeypress(_PERPENDICULAR_SC_, _PERPENDICULAR_SC_);
    return 0;
}

int KeyT(void)
{
	//simulate_keypress(_TANGENTIAL_);
	simulate_ukeypress(_TANGENTIAL_, _TANGENTIAL_SC_);
    return 0;
}

int KeyCtrlT(void)
{
	//simulate_keypress(_TANGENTIAL_);
	simulate_ukeypress(_TANGENTIAL_SC_, _TANGENTIAL_SC_);
    return 0;
}

int KeyA(void)
{
	//simulate_keypress(_ADJACENT_);
	simulate_ukeypress(_ADJACENT_, _ADJACENT_SC_);
    return 0;
}

int KeyCtrlA(void)
{
	//simulate_keypress(_ADJACENT_);
	simulate_ukeypress(_ADJACENT_SC_, _ADJACENT_SC_);
    return 0;
}

int KeyO(void)
{
	//simulate_keypress(_POINT_);
	simulate_ukeypress(_POINT_, _POINT_SC_);
    return 0;
}

int KeyCtrlO(void)
{
	//simulate_keypress(_POINT_);
	simulate_ukeypress(_POINT_SC_, _POINT_SC_);
    return 0;
}

int KeyX(void)
{
	simulate_keypress('X');
    return 0;
}

int KeyCtrlX(void)
{
	simulate_ukeypress(_NOPE_SC_, _NOPE_SC_);
    return 0;
}

int KeyY(void)
{
	simulate_keypress('Y');
    return 0;
}

int KeyL(void)
{
	simulate_keypress('L');
    return 0;
}

int KeyK(void)
{
	simulate_keypress('K');
    return 0;
}

int KeyCtrlK(void)
{
	simulate_keypress('K');
    return 0;
}

int KeyS(void)
{
	simulate_keypress('S');
    return 0;
}

int KeyCtrlS(void)
{
	simulate_keypress('S');
    return 0;
}


int KeyB(void)
{
	simulate_keypress('B');
    return 0;
}

int KeyCtrlB(void)
{
	simulate_keypress('B');
    return 0;
}

int KeyR(void)
{
	simulate_keypress('R');
    return 0;
}

int KeyCtrlR(void)
{
	simulate_keypress('R');
    return 0;
}

int KeyU(void)
{
	simulate_keypress('U');
    return 0;
}

int KeyPgUp()
{	
	simulate_keypress(11776);
    return 0;
}

int KeyPgDn()
{
	simulate_keypress(10240);
    return 0;
}

int KeyF9()
{
	simulate_keypress(14080);
    return 0;
}

int KeyF10(void)
{
	simulate_keypress(14336);
    return 0;
}

int Desktop_instruction()
{
    int ret=Show_Alfa_Instruction();
    return ret;
}

static int(*COMNDh[])(void) = {
	nooph,menupoz,menupoz,Lastfun,Tabfun,KeyPgUp,KeyPgDn,Expand_flex /*Free_Mouse*/,Expand_flex /*Free_Mouse*/,Expand_flex /*Free_Mouse*/,Expand_flex /*Free_Mouse*/, circle_and_rectangle_proc,nooph,
	Change_Orto, Kopiuj_Ctrl_R, Set_Layers_, nooph,Edit_Text_Tab_F2,Edit_Text_F3,Usun_F5,undo_blok_F6,Change_Orto,KeyF9,KeyF10,nooph,
	nooph,Pan_Shift,Pan_Shift,SkalaW_,last_view_,SkalaE_,SkalaZC_,SkalaZ1_,SkalaZ05_,SkalaZ02_,SkalaZ01_,SkalaR_,
	skala_format_,Skala_In_, Skala_Out_, nooph,kolorL_,typ_linii_,grubosc_linii_,Set_Layers_,Wez_kolor_,Wez_typ_,Wez_warstwe_,Hide_Layers_,
	nooph,Point_Measure_,Dist_Measure_,Angle_Measure1_,nooph,Choose_Layer_1,Choose_Layer_2,Choose_Layer_10,Inc_Layer,Dec_Layer,
	All_Layers_On_,Reverse_Layers_,nooph,
	small_large_step,nooph,
	KeyE,KeyN,KeyM,KeyI,KeyC,KeyP,KeyT,KeyA,KeyO,
	KeyE,KeyN,KeyM,KeyI,KeyC,KeyP,KeyT,KeyA,KeyO,//KeyX,KeyY,KeyL,
	KeyCtrlE,KeyCtrlN,KeyCtrlM,KeyCtrlI,KeyCtrlC,KeyCtrlP,KeyCtrlT,KeyCtrlA,KeyCtrlO,KeyCtrlX, Kopiuj_Ctrl_R, Set_Layers_, Set_Layers_,Desktop_instruction,
};

int help(TMENU *menu)
{
	int n;
	static void(*CUR)(int, int);
    int ret;

	if ((n = getwsp1(&mHelp) - 1) >= 0)    /* n=-1 -> esc */
	{
		ret=(*COMNDh[n])();
		//LASTFUN = COMNDh[n];
	}

	set_mouse_sprite(mouse_sprite);
	return 0;
}

#undef __B_HLP__