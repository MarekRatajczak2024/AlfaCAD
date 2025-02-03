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

#define __ALFMAIN__
#pragma warning(disable : 4996)

#ifndef LINUX
#ifndef ALLEGRO_AND_MFC
#define ALLEGRO_AND_MFC  //????????
#endif
#else

#endif
#define ALLEGRO_NO_MAGIC_MAIN
#include <allegro.h>
#ifndef LINUX
#include<winalleg.h>
#include <process.h>
#include <dwmapi.h>
#include <windowsx.h>
#else
#include<xalleg.h>
#endif
#include <stdio.h>
#include <wchar.h>

#pragma comment (lib, "Dwmapi")

#ifdef LINUX
#include "res/resource.h"

#include "tinyfiledialogs.h"
#include "clip.h"
#include <iostream>
#include <string>
#else
#include "..\..\source\res\resource.h"
#endif
#include "ttf.h"
#include <locale.h>

#ifndef LINUX
#ifdef BIT64
#include "GlobalDllObjects.h"
#else
#include "GlobalDllObjects.h"
#endif
#endif

#ifndef LINUX
#define RGB(r,g,b)      ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

#include "menu.h"

#include "leak_detector_cpp.hpp"


#ifdef BIT64
#define long_long long long
#else
#define long_long long
#endif


int single_=0;
int tab_ = 0;
UINT uHeight_;
int Height;
int Width;

#ifndef LINUX
CToolTipCtrl* m_ToolTip;
HWND editor_hWnd = NULL;
#endif

bool next_time=0;

#define EXT__INI "INI"

typedef std::string String;
typedef std::wstring WString;
extern String WStringToString(const WString& widestr);

char* get_font_family_name(void);

BOOL editor_on = FALSE;

#ifdef __cplusplus
extern "C" {
#endif

extern int Client_number;

extern int Save_Update_flex(int save_upd, int *curr_h, int *curr_v);
extern int get_window_origin_and_size(int* x_win_orig, int* y_win_orig, int* win_width, int* win_height);

extern	char** argv_;
extern 	char* argp__;

void  QInitialize(int argc, char *argv[]);

#ifdef LINUX
extern Window get_toplevel_parent(Display * display, Window window);
#endif

extern bool DEMO_RECORDING;

extern void set_demo_scale(float demo_scale_);

extern int code_page;
extern int dxf_encoding;
extern int Rysuj_main(int child, char file_name[255], int nCmdShow, char *application, char *arguments);
extern void delay(int time);
extern void sw_callback_locked_proc(void);

extern void dialog_cursor(int on);
extern void lock_mouse(void);
extern void _free_mouse(void);
extern void free_mouse(void);
extern void get_posXY(double *pozx, double *pozy);
extern void set_posXY(double pozx, double pozy);
extern void(*CUR_ON)(double, double);
extern long pikseleDX(double jednostki);
extern long pikseleDY(double jednostki);

extern long pikseleX(double jednostki);
extern long pikseleY(double jednostki);


extern char * tinyfd_inputBox(
        char const * aTitle , // NULL or ""
        char const * aMessage , // NULL or "" (\n and \t have no effect)
        char const * aDefaultInput ); // "" , if NULL it's a passwordBox

extern int texteditor(char *text, int maxchars, const int startrow, const int startcol,int maxrows,const int maxcols,const int displayrows,const int returnhandler,const char *permitted, bool ins, const bool allowcr);

extern bool file_bak (char *fn, char *fext);
extern char *File_Ext(char *fn, char *fext);

#ifndef LINUX
__declspec(dllexport) int testCall(int val);
#endif
int testCall(int val);
int GoRegtestCall(int(*ptr)(int));
bool Copy_File(char *ptrsz_fnd, char *ptrsz_fns);

#ifdef ALF_MAIN
int first_window_main=0;
#endif
int EditText(char *mytext, int adjust, int nCmdShow, int *single, int *tab);
int EditFile(char *mytextfilename, int adjust, int nCmdShow);
int get_cursor_info(void);
int set_window_icon(void);
#ifndef LINUX
void Set_Focus(HWND HWnd);
#endif
void windows2mazovia(char *tekst);
extern int HEIGHT, WIDTH;
extern int32_t utf8_to_ucs2(const uint8_t * input, const uint8_t ** end_ptr);
extern int32_t ucs2_to_utf8(int32_t ucs2, uint8_t * utf8);
void find_font_face(char *Font_File);
void find_any_font_face(char *Font_File, char *face_name);
int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext);
void unicode2windxf(char *unicodetext, char *wintext, int count);
void unicode2win(char *unicodetext, char *wintext, int count);
void utf82win(char *utf8text, char *wintext);
extern void get_strings_list(int *string_no, long_long *ptrsz_long);
extern int circle_and_rectangle_proc0(int untrap_mouse);
char *GetStringFromClipboard(void);
//void PutStringToClipboard(char *ptrsz_buf);
void PutStringToClipboard(LPCWSTR ptrsz_buf);
void mouseMove(int dx, int dy);

void get_tens_value(char* st, double tens);

void Put_Str_To_Clip(char *ptrsz_buf);
bool Get_Str_From_Clip(char *ptrsz_buf,
                       int i_poz,
                       int i_buflen,
                       int xpcz,
                       int ypcz);


void report_mem_leak_cpp_(void);

void set_editbox_geometry_win(int x, int y);
void set_editbox_geometry_line_win(int x, int y);

//char *get_font_family_name(void);


#ifndef LINUX
_locale_t locale;
#endif

#ifdef __cplusplus
}
#endif

int win2unicode(char *wintext, char *unicodetext);
int win2unicodefactory(char *wintext, char *unicodetext, int codepage);
void unicode2winfactory(char *unicodetext, char *wintext, int count, int codepage);

static char font_family_name[255];
static char font_file[255];
static bool alt=FALSE;
static bool altlock = FALSE;
static bool altpressed = FALSE;
static bool shiftpressed = FALSE;

#ifdef LINUX
#include <string.h>
typedef char CString[255];
#endif

bool NOCHDIR = FALSE;

CString font_name;


#ifndef LINUX

HWND hwndCombobox;
HWND hwndButtonOK;
HWND hwndButtonESC;
HWND hwndButtonLeft;
HWND hwndButtonRight;
HWND hwndButtonCopyText;
HWND hwndButtonCenter;
HWND hwndButtonMiddleCenter;
HWND hwndButtonUnderlined;
HWND hwndButtonBold;
HWND hwndButtonAlt;
HWND hwndButtonItalic;
HWND hwndButtonHidden;
static HWND hwndEdit=NULL;
int new_adjust;
BOOL new_bold;
BOOL new_italics;
BOOL new_underline;
BOOL new_hidden;
int new_params;

WNDPROC oldEditProc;
WNDPROC oldComboboxProc;

#define ID_EDITCHILD 100
#define ID_BUTTON_OK 108
#define ID_BUTTON_ESC 109
#define ID_BUTTON_LEFT 101
#define ID_BUTTON_CENTER 102
#define ID_BUTTON_MIDDLE_CENTER 110
#define ID_BUTTON_RIGHT 103
#define ID_BUTTON_COPY_TEXT 111
#define ID_BUTTON_UNDERLINED 104
#define ID_BUTTON_BOLD 105
#define ID_BUTTON_ITALIC 106
#define ID_BUTTON_ALT 107
#define ID_BUTTON_HIDDEN 112

int BUT_DY = 89;
int COMBO_DY = 120;
int EDIT_DY = 132;
int dHeight = 60;

#define DEGREESIGN 176
#define SUPERSCRIPT3 179
#define SUPERSCRIPT2 178
#define PLUSMINUS 177
#define DIVISION 247
#define MIDDLEDOT 183
#define ONEHALF 0xBD
#define ONEQUARTER 0xBC
#define THREEQUARTERS 0xBE
#define DIAMETERSIGN 0xD8

#define ALPHASIGN 0x3B1
#define BETASIGN 0x3B2
#define GAMMASIGN 0x3B3
#define DELTASIGN 0x3B4
#define EPSILONSIGN 0x3B5
#define ZETASIGN 0x3B6
#define ETASIGN 0x3B7
#define THETASIGN 0x3B8
#define IOTASIGN 0x3B9
#define KAPPASIGN 0x3BA
#define LAMBDASIGN 0x3BB
#define MUSIGN 0x3BC
#define NUSIGN 0x3BD
#define XISIGN 0x3BE
#define OMICRONSIGN 0x3BF
#define PISIGN 0x3C0
#define RHOSIGN 0x3C1
#define FINALSIGMASIGN 0x3C2
#define SIGMASIGN 0x3C3
#define TAUSIGN 0x3C4
#define UPSYLONSIGN 0x3C5
#define PHISIGN 0x3C6
#define CHISIGN 0x3C7
#define PSISIGN 0x3C8
#define OMEGASIGN 0x3C9
#define YOTSIGN 0x3F4
#define PAMPHYLIANDIGAMMASIGN 0x37A

#define CAPITALALPHASIGN 0x391
#define CAPITALBETASIGN 0x392
#define CAPITALGAMMASIGN 0x393
#define CAPITALDELTASIGN 0x394
#define CAPITALEPSILONSIGN 0x395
#define CAPITALZETASIGN 0x396
#define CAPITALETASIGN 0x397
#define CAPITALTHETASIGN 0x398
#define CAPITALIOTASIGN 0x399
#define CAPITALKAPPASIGN 0x39A
#define CAPITALLAMBDASIGN 0x39B
#define CAPITALMUSIGN 0x39C
#define CAPITALNUSIGN 0x39D
#define CAPITALXISIGN 0x39E
#define CAPITALOMICRONSIGN 0x39F
#define CAPITALPISIGN 0x3A0
#define CAPITALRHOSIGN 0x3A1
#define CAPITALSIGMASIGN 0x3A3
#define CAPITALTAUSIGN 0x3A4
#define CAPITALUPSYLONSIGN 0x3A5
#define CAPITALPHISIGN 0x3A6
#define CAPITALCHISIGN 0x3A7
#define CAPITALPSISIGN 0x3A8
#define CAPITALOMEGASIGN 0x3A9
#define CAPITALYOTSIGN 0x384
#define CAPITALPAMPHYLIANDIGAMMASIGN 0x377

int ret_value=0;
char ret_text[64001]; // [2048];
BOOL first_time = TRUE;
int underlined = 0;
HFONT hFont[8];
int font_choice;
long alignement;
RECT lpRect;

RECT lpRect_edit = { 0, 32, 640, 432 };
RECT lpRect_single = { 0, 0, 800, 112 };
RECT lpRect_file = { 0, 32, 640, 432 };

HINSTANCE my_hInstance;

static int on_header = 0;

HWND get_editor_hWnd(void)
{
	return editor_hWnd;
}

BOOL get_editor_on(void)
{
	return editor_on;
}

int get_cursor_info(void)
{
	CURSORINFO ci;
	ci.cbSize = sizeof(ci);

	if (!GetCursorInfo(&ci)) {
		return 0;
	}

	//BOOL ret = DestroyCursor(ci.hCursor);
	ci.hCursor = NULL;
	ShowCursor(FALSE);
		//ShowCursor(FALSE);
		//ShowCursor(FALSE);
		//ShowCursor(FALSE);
		//ShowCursor(FALSE);
		//ShowCursor(FALSE);

		//SetWindowsHookEx(WH_MOUSE_LL, );
	//remove_mouse();
	//install_mouse();

	return 1;
}

void set_editbox_geometry_win(int x, int y)
{
	lpRect_edit = { x, y, x+640, y+400 };
	lpRect_file = { x, y, x+640, y+400 };
}

void set_editbox_geometry_line_win(int x, int y)
{
	lpRect_single = { x, y, x+800, y+80 };
}

RECT get_editbox_geometry_win(int opt)
{
	return lpRect;
}

void get_tens_value(char* st, double tens)
{
	sprintf(st, "%#12.9lg", tens);
}

void find_font_face(char *Font_File)
{
	font_name = CString(Font_File);

	TTF ttf;
	std::wstring wstr;
	USES_CONVERSION;
	ttf.Parse(A2W((LPCSTR)font_name.GetString()));
	wstr = ttf.GetFontName().c_str();
	sprintf(font_family_name, "%ls", wstr.c_str());
}

void find_any_font_face(char *Font_File, char *face_name)
{
	font_name = CString(Font_File);

	TTF ttf;
	std::wstring wstr;
	USES_CONVERSION;
	ttf.Parse(A2W((LPCSTR)font_name.GetString()));
	wstr = ttf.GetFontName().c_str();
	sprintf(face_name, "%ls", wstr.c_str());
}

LRESULT CALLBACK subComboboxProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i;
	BOOL shifted;
	BOOL ret;

	return CallWindowProc(oldComboboxProc, hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK subEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i;
	BOOL shifted;
	BOOL ret;

	switch (msg)
	{
	case WM_SYSCHAR:
		return 0;
		break;
	case WM_SYSKEYUP:
		if (wParam == VK_MENU)
		{
			if (!altlock) alt = !alt;
			else
			{
				alt = FALSE;
				altlock = FALSE;
			}
			altpressed = FALSE;
			if (alt) ShowWindow(hwndButtonAlt, SW_SHOW);
			else ShowWindow(hwndButtonAlt, SW_HIDE);
		}
		else if (wParam == VK_SHIFT)
		{
			shiftpressed = FALSE;
		}
		break;
	case WM_SYSKEYDOWN:
		
		if (wParam == VK_MENU)
		{
			altpressed = TRUE;
			ShowWindow(hwndButtonAlt, SW_SHOW);
		}
		else if (wParam == VK_SHIFT)
		{
			shiftpressed = TRUE;
		}
		else
		{
			if (shiftpressed) shifted = TRUE;
			else shifted = FALSE;
			if (altpressed)
			{
				switch (wParam)
				{
				case '8': wParam = DEGREESIGN;
					break;
				case '3': wParam = SUPERSCRIPT3;
					break;
				case '2': wParam = SUPERSCRIPT2;
					break;
				case '-': wParam = PLUSMINUS;
					break;
				case '/': wParam = DIVISION;
					break;
				case '.': wParam = MIDDLEDOT;
					break;
				case '1': wParam = ONEHALF;
					break;
				case '4': wParam = ONEQUARTER;
					break;
				case '5': wParam = THREEQUARTERS;
					break;
				case '0': wParam = DIAMETERSIGN;
					break;
				case 'A': if (shifted) wParam = CAPITALALPHASIGN;
					    else wParam = ALPHASIGN;
					break;
				case 'B': if (shifted) wParam = CAPITALBETASIGN;
						else wParam = BETASIGN;
					break;
				case 'G': if(shifted) wParam = CAPITALGAMMASIGN;
						else wParam = GAMMASIGN;
					break;
				case 'D': if (shifted) wParam = CAPITALDELTASIGN;
						else wParam = DELTASIGN;
					break;
				case 'E': if (shifted) wParam = CAPITALEPSILONSIGN;
						else wParam = EPSILONSIGN;
					break;
				case 'Z': if (shifted) wParam = CAPITALZETASIGN;
						else wParam = ZETASIGN;
					break;
				case 'Q': if (shifted) wParam = CAPITALETASIGN;
						else wParam = ETASIGN;
					break;
				case 'H': if (shifted) wParam = CAPITALTHETASIGN;
						else wParam = THETASIGN;
					break;
				case 'I': if (shifted) wParam = CAPITALIOTASIGN;
						else wParam = IOTASIGN;
					break;
				case 'K': if (shifted) wParam = CAPITALKAPPASIGN;
						else wParam = KAPPASIGN;
					break;
				case 'L': if (shifted) wParam = CAPITALLAMBDASIGN;
						else wParam = LAMBDASIGN;
					break;
				case 'M': if (shifted) wParam = CAPITALMUSIGN;
						else wParam = MUSIGN;
					break;
				case 'N': if (shifted) wParam = CAPITALNUSIGN;
						else wParam = NUSIGN;
					break;
				case 'X': if (shifted) wParam = CAPITALXISIGN;
						else wParam = XISIGN;
					break;
				case 'O': if (shifted) wParam = CAPITALOMICRONSIGN;
						else wParam = OMICRONSIGN;
					break;
				case 'P': if (shifted) wParam = CAPITALPISIGN;
						else wParam = PISIGN;
					break;
				case 'R': if (shifted) wParam = CAPITALRHOSIGN;
						else wParam = RHOSIGN;
					break;
				case 'S': if (shifted) wParam = CAPITALSIGMASIGN;
					    else wParam = SIGMASIGN;
					break;
				case 'T': if (shifted) wParam = CAPITALTAUSIGN;
						else wParam = TAUSIGN;
					break;
				case 'Y': if (shifted) wParam = CAPITALUPSYLONSIGN;
						else wParam = UPSYLONSIGN;
					break;
				case 'F': if (shifted) wParam = CAPITALPHISIGN;
						else wParam = PHISIGN;
					break;
				case 'C': if (shifted) wParam = CAPITALCHISIGN;
						else wParam = CHISIGN;
					break;
				case 'U': if (shifted) wParam = CAPITALPSISIGN;
						else wParam = PSISIGN;
					break;
				case 'W': if (shifted) wParam = CAPITALOMEGASIGN;
						else wParam = OMEGASIGN;
					break;
				case 'J': if (shifted) wParam = CAPITALYOTSIGN;
						else wParam = YOTSIGN;
					break;
				case 'V': if (shifted) wParam = CAPITALPAMPHYLIANDIGAMMASIGN;
						else wParam = PAMPHYLIANDIGAMMASIGN;
					break;
				default:
					return 0;
					break;
				}

				alt = FALSE;
				altlock = TRUE;
				
				return CallWindowProc(subEditProc, hWnd, WM_CHAR, wParam, lParam);
			}
			
		}
		
		break;
	case WM_CHAR:
		if ((alt==TRUE) || (altpressed==TRUE))
		{
			switch (wParam)
			{
			case '8' : wParam = DEGREESIGN;
				break;
			case '3': wParam = SUPERSCRIPT3;
				break;
			case '2': wParam = SUPERSCRIPT2;
				break;
			case '-': wParam = PLUSMINUS;
				break;
			case '/': wParam = DIVISION;
				break;
			case '.': wParam = MIDDLEDOT;
				break;
			case '1': wParam = ONEHALF;
				break;
			case '4': wParam = ONEQUARTER;
				break;
			case '5': wParam = THREEQUARTERS;
				break;
			case '0': wParam = DIAMETERSIGN;
				break;
			case 'A': wParam = CAPITALALPHASIGN;
				break;
			case 'B': wParam = CAPITALBETASIGN;
				break;
			case 'G': wParam = CAPITALGAMMASIGN;
				break;
			case 'D': wParam = CAPITALDELTASIGN;
				break;
			case 'E': wParam = CAPITALEPSILONSIGN;
				break;
			case 'Z': wParam = CAPITALZETASIGN;
				break;
			case 'Q': wParam = CAPITALETASIGN;
				break;
			case 'H': wParam = CAPITALTHETASIGN;
				break;
			case 'I': wParam = CAPITALIOTASIGN;
				break;
			case 'K': wParam = CAPITALKAPPASIGN;
				break;
			case 'L': wParam = CAPITALLAMBDASIGN;
				break;
			case 'M': wParam = CAPITALMUSIGN;
				break;
			case 'N': wParam = CAPITALNUSIGN;
				break;
			case 'X': wParam = CAPITALXISIGN;
				break;
			case 'O': wParam = CAPITALOMICRONSIGN;
				break;
			case 'P': wParam = CAPITALPISIGN;
				break;
			case 'R': wParam = CAPITALRHOSIGN;
				break;
			case 'S': wParam = CAPITALSIGMASIGN;
				break;
			case 'T': wParam = CAPITALTAUSIGN;
				break;
			case 'Y': wParam = CAPITALUPSYLONSIGN;
				break;
			case 'F': wParam = CAPITALPHISIGN;
				break;
			case 'C': wParam = CAPITALCHISIGN;
				break;
			case 'U': wParam = CAPITALPSISIGN;
				break;
			case 'W': wParam = CAPITALOMEGASIGN;
				break;
			case 'J': wParam = CAPITALYOTSIGN;
				break;
			case 'V': wParam = CAPITALPAMPHYLIANDIGAMMASIGN;
				break;
			case 'a': wParam = ALPHASIGN;
				break;
			case 'b': wParam = BETASIGN;
				break;
			case 'g': wParam = GAMMASIGN;
				break;
			case 'd': wParam = DELTASIGN;
				break;
			case 'e': wParam = EPSILONSIGN;
				break;
			case 'z': wParam = ZETASIGN;
				break;
			case 'q': wParam = ETASIGN;
				break;
			case 'h': wParam = THETASIGN;
				break;
			case 'i': wParam = IOTASIGN;
				break;
			case 'k': wParam = KAPPASIGN;
				break;
			case 'l': wParam = LAMBDASIGN;
				break;
			case 'm': wParam = MUSIGN;
				break;
			case 'n': wParam = NUSIGN;
				break;
			case 'x': wParam = XISIGN;
				break;
			case 'o': wParam = OMICRONSIGN;
				break;
			case 'p': wParam = PISIGN;
				break;
			case 'r': wParam = RHOSIGN;
				break;
			case 's': wParam = SIGMASIGN;
				break;
			case 't': wParam = TAUSIGN;
				break;
			case 'y': wParam = UPSYLONSIGN;
				break;
			case 'f': wParam = PHISIGN;
				break;
			case 'c': wParam = CHISIGN;
				break;
			case 'u': wParam = PSISIGN;
				break;
			case 'w': wParam = OMEGASIGN;
				break;
			case 'j': wParam = YOTSIGN;
				break;
			case 'v': wParam = PAMPHYLIANDIGAMMASIGN;
				break;
			default:
				break;
			}
		}
        alt = FALSE;
		if (!altpressed) ShowWindow(hwndButtonAlt, SW_HIDE);
		if ((single_ == 1) && (wParam == VK_RETURN))
			return 0;
		else if (wParam == VK_ESCAPE)
			return 0;
		else if (wParam == VK_TAB)
			return 0;
		return CallWindowProc(oldEditProc, hWnd, msg, wParam, lParam);
		break;
	case WM_KEYUP:
		if ((single_ == 1) && (wParam == VK_RETURN))
		{
			return 0;
		}
		else if (wParam == VK_TAB)
			return 0;
	    return CallWindowProc(oldEditProc, hWnd, msg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_TAB)
		{
			if (tab_ == 1)
			{
				

				HWND parenthWnd = (HWND)GetParent(hWnd);
				ret_value = -1;
				for (i = 0; i < 8; i++) DeleteObject(hFont[i]);
				
				sw_callback_locked_proc();
				ret = PostMessage(parenthWnd, WM_CLOSE, 0, 0);
				
			}
			return 0;
			break;
		}
		if (wParam == VK_ESCAPE)
		{
			

			HWND parenthWnd = (HWND)GetParent(hWnd);
			ret_value = 0;
			for (i = 0; i < 8; i++) DeleteObject(hFont[i]);
			
			sw_callback_locked_proc();
			ret=PostMessage(parenthWnd, WM_CLOSE , 0, 0);
			
			return 0;
			break;
		}
		
		else if ((single_==1) && (wParam == VK_RETURN))
			{
			


				HWND parenthWnd = (HWND)GetParent(hWnd);

				int len = GetWindowTextLength(hwndEdit) + 1;

				SendMessageW(hwndEdit, WM_GETTEXT, len, (LPARAM)ret_text);  //(LPCWSTR)

				ret_value = 1;

				for (i = 0; i < 8; i++) DeleteObject(hFont[i]);
				sw_callback_locked_proc();

				ret=PostMessage(parenthWnd, WM_CLOSE, 0, 0);

			
				return 0;
				break;
			}
			
		else  return CallWindowProc(oldEditProc, hWnd, msg, wParam, lParam);
		break;
	default:
		return CallWindowProc(oldEditProc, hWnd, msg, wParam, lParam);
		break;
	}
	return 0;
}



void reloadEdit(HWND hWnd)
{
	DWORD dwStyle;
	int len = GetWindowTextLength(hwndEdit) + 1;

	SendMessageW(hwndEdit, WM_GETTEXT, len, (LPARAM)ret_text); 
	DestroyWindow(hwndEdit);

	GetWindowRect(hWnd, &lpRect);

	switch (new_adjust)
	{
	case 0:
		alignement = ES_LEFT;
		break;
	case 1:
		alignement = ES_RIGHT;
		break;
	case 2:
		alignement = ES_CENTER;
		break;
	case 3:
		alignement = ES_CENTER;
		break;
	}

#define GWL_HINSTANCE       (-6)

	if (single_ == 0) dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | alignement | EM_FMTLINES ^ ES_UPPERCASE | ES_WANTRETURN | ES_MULTILINE /* | ES_OEMCONVERT */ | ES_AUTOHSCROLL | ES_AUTOVSCROLL; // | WS_BORDER | WS_EX_TOPMOST ^ ES_UPPERCASE;
	else dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | alignement | EM_FMTLINES ^ ES_UPPERCASE /* | ES_WANTRETURN*/ /* | ES_MULTILINE */ /* | ES_OEMCONVERT */ | ES_AUTOHSCROLL; // /* | WS_BORDER*/ | WS_EX_TOPMOST ^ ES_UPPERCASE;

																																																									      //5, 0
	hwndEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"Edit", L"", dwStyle, 10, 10, lpRect.right - lpRect.left - 30, lpRect.bottom - lpRect.top - 100, hWnd, (HMENU)ID_EDITCHILD, (HINSTANCE)(long long)GetWindowLongW(hWnd, GWL_HINSTANCE), NULL);


	oldEditProc = (WNDPROC)SetWindowLongPtrW(hwndEdit, GWLP_WNDPROC, (LONG_PTR)subEditProc);

	font_choice = new_underline + new_bold * 2 + new_italics * 4;

	SendMessageW(hwndEdit, WM_SETFONT, WPARAM(hFont[font_choice]), TRUE);
	
	SendMessageW(hwndEdit, WM_SETTEXT, 0, (WPARAM)(LPCWSTR)ret_text);
	SendMessageW(hwndEdit, EM_SETLIMITTEXT, (WPARAM)0xFA00, 0);

	GetWindowRect(hWnd, &lpRect);
	
	SetWindowPos(hwndEdit, 0, 5, 0, lpRect.right - lpRect.left - 30, lpRect.bottom - lpRect.top - EDIT_DY, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);



}

int WINAPIV MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int *ret)
{  
	switch (msg)
	{
	case WM_SIZE:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	case WM_NCMOUSEMOVE:
		on_header = 1;
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	case WM_NCMOUSELEAVE:
		on_header = 0;
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	case WM_NCLBUTTONDBLCLK:
	
		return 0;
		break;
	}

	return 1;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	int nMaxCount = 2048;
	int i;

	DWORD firstChar, lastChar;

	switch (msg)
	{
		
	
	case WM_CTLCOLOREDIT:
		// control's window handle is lParam, not hWnd
		if ((HWND)lParam == hwndEdit)
		{
			// HDC of control is wParam
			if (new_hidden) SetTextColor((HDC)wParam, 0x999999);
			else SetTextColor((HDC)wParam, 0x00000000);
			// return new background brush
			return (LRESULT)GetStockObject(WHITE_BRUSH);
		}
		return CallWindowProc(oldEditProc, hWnd, msg, wParam, lParam);
		break;
	case WM_NCHITTEST:
	{
		if (single_==1)
		{
			LRESULT result = DefWindowProc(hWnd, msg, wParam, lParam);
			if (result == HTCLIENT) result = HTCAPTION;
			return result;
		}
		else
		{
			LRESULT result = DefWindowProc(hWnd, msg, wParam, lParam);
			if (result == HTCLIENT) result = HTCAPTION;
			return result;
		}
	
	}
	break;

	
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* mmi = (MINMAXINFO*)lParam;
		if (single_ == 1)
		{
			
			mmi->ptMinTrackSize.y = Height * 2 + 107;
			mmi->ptMaxTrackSize.y = Height * 2 + 107;
		}
		else
		{
			mmi->ptMinTrackSize.y = Height * 2 + 137;
		}
		return 0;
	}
	case WM_CLOSE:
		sw_callback_locked_proc();
		return DefWindowProcW(hWnd, msg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			ret_value = 0;
			for (i = 0; i < 8; i++) DeleteObject(hFont[i]);
			sw_callback_locked_proc();
			PostMessageW(hWnd, WM_CLOSE, 0, 0);
			
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
	case WM_SIZE:
		GetWindowRect(hWnd, &lpRect);
		SetWindowPos(hwndEdit, 0, 5, 0, lpRect.right - lpRect.left - 30, lpRect.bottom - lpRect.top - EDIT_DY /*132*/, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

		SetWindowPos(hwndCombobox, 0, 10, lpRect.bottom - lpRect.top - COMBO_DY/*120*/, lpRect.right - lpRect.left - 40, 180, SWP_NOOWNERZORDER | SWP_NOZORDER);
		

		SetWindowPos(hwndButtonAlt, 0, 10, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonBold, 0, 55, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonItalic, 0, 100, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonUnderlined, 0, 145, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonLeft, 0, 190, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		if (single_ == 0)
		{
			SetWindowPos(hwndButtonCenter, 0, 235, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(hwndButtonRight, 0, 280, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(hwndButtonHidden, 0, 325, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(hwndButtonCopyText, 0, 380, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		}
		else
		{
			SetWindowPos(hwndButtonCenter, 0, 235, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(hwndButtonMiddleCenter, 0, 280, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(hwndButtonRight, 0, 325, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(hwndButtonHidden, 0, 370, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(hwndButtonCopyText, 0, 425, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		}
		SetWindowPos(hwndButtonOK, 0, lpRect.right - lpRect.left - 180, lpRect.bottom - lpRect.top - BUT_DY, 60, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonESC, 0, lpRect.right - lpRect.left - 110, lpRect.bottom - lpRect.top - BUT_DY, 60, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);


		break;
	case WM_MOVE:
		GetWindowRect(hWnd, &lpRect);
		break;
	case WM_COMMAND: //Command from Child windows and menus are under this message
		switch (wParam) //the ID is wParam
		{
			case ID_BUTTON_ESC: 
			{
				ret_value = 0;

				for (i=0; i<8; i++) DeleteObject(hFont[i]);
				sw_callback_locked_proc();
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				
				break;
			}
			case ID_BUTTON_OK:
			{
				int len = GetWindowTextLength(hwndEdit) + 1;
				
				SendMessageW(hwndEdit, WM_GETTEXT, len, (LPARAM)ret_text);  //(LPCWSTR)
				ret_value = 1;

				for (i = 0; i < 8; i++) DeleteObject(hFont[i]);

				PostMessage(hWnd, WM_CLOSE, 0, 0);
				
				break;
			}
			case ID_BUTTON_BOLD:
			{
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
				if (new_bold == 0) new_bold = 1;
				else new_bold = 0;
				reloadEdit(hWnd);
				SetFocus(hwndEdit);
				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				break;
			}

			case ID_BUTTON_ITALIC:
			{
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
				if (new_italics == 0) new_italics = 1;
				else new_italics = 0;
				reloadEdit(hWnd);
				SetFocus(hwndEdit);
				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				break;
			}

			case ID_BUTTON_UNDERLINED:
			{
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
				if (new_underline==0) new_underline=1;
				else new_underline = 0;
				reloadEdit(hWnd);
				SetFocus(hwndEdit);
				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				break;
			}
			case ID_BUTTON_CENTER:
			{
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
				
				new_adjust = 2;
				reloadEdit(hWnd);
				SetFocus(hwndEdit);
				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				
				break;
			}
			case ID_BUTTON_MIDDLE_CENTER:
			{
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
				
				new_adjust = 3;
				reloadEdit(hWnd);
				SetFocus(hwndEdit);
				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				
				break;
			}
			case ID_BUTTON_LEFT:
			{
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
				new_adjust = 0;
				reloadEdit(hWnd);
				SetFocus(hwndEdit);
				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				break;
			}
			case ID_BUTTON_RIGHT:
			{
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
				new_adjust = 1;
				reloadEdit(hWnd);
				SetFocus(hwndEdit);
				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				break;
			}
			case ID_BUTTON_HIDDEN:
			{
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
				new_hidden = (!new_hidden);
				reloadEdit(hWnd);
				SetFocus(hwndEdit);
				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				break;
			}
			case ID_BUTTON_COPY_TEXT:
			{
				//TO DO
				LPTSTR lpch = new char[512];

				int lenC = ComboBox_GetTextLength(hwndCombobox);
				ComboBox_GetText(hwndCombobox, lpch, 512);
				

				int len = GetWindowTextLength(hwndEdit);
				
				SetFocus(hwndEdit);
				/* Get the selection indexes */
				SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);

				if (single_ == 1) //changing end of line characters
				{
					char* ptr = strstr(lpch,"\r\n");
					while (ptr != NULL)
					{
						*ptr = '\\';
						ptr++;
						*ptr = 'n';
						ptr++;
						ptr = strstr(ptr, "\r\n");
					}
				}
				else
				{
					char* ptr = strstr(lpch, "\\n");
					while (ptr != NULL)
					{
						*ptr = '\r';
						ptr++;
						*ptr = '\n';
						ptr++;
						ptr = strstr(ptr, "\\n");
					}
				}

				
				SendMessage(hwndEdit, EM_REPLACESEL, 0, (LPARAM)lpch);

				firstChar = lastChar = firstChar + lenC;

				// restore their cursor position or selection
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)firstChar, (LPARAM)lastChar);
				delete(lpch);

				break;
			}
		}

	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	return ret_value;
}


LRESULT CtlColorEdit(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// control's window handle is lParam, not hWnd
	if ((HWND)lParam == hwndEdit)
	{
		// HDC of control is wParam
		SetTextColor((HDC)wParam, 0x00C800);
		// return new background brush
		return (LRESULT)GetStockObject(BLACK_BRUSH);
	}

	// just use the default settings otherwise
	return DefWindowProc(hWnd, message, wParam, lParam);
}


HWND CreateToolTip(int toolID, HWND hDlg, HINSTANCE hInstance, PTSTR pszText)
{
	if (!toolID || !hDlg || !pszText)
	{
		return FALSE;
	}
	// Get the window of the tool.
	HWND hwndTool = GetDlgItem(hDlg, toolID);

	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hDlg, NULL,
	    hInstance, NULL);  //g_hInst

	if (!hwndTool || !hwndTip)
	{
		return (HWND)NULL;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hDlg;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pszText;
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

	return hwndTip;
}

HWND CreateTextBoxW(CStringW mytext /*char *mytext*/, int edit_params, CONST INT iX, CONST INT iY, CONST UINT uWidth, CONST UINT uHeight, HWND hWnd, CONST UINT uId, HINSTANCE hInstance, int nCmdShow, LPWSTR windowTitle, int *single, int *tab)
{
	LPWSTR windowClass = (LPWSTR)(L"Editor"); 
	WNDCLASSEXW wcex;
	BOOL edit_file;
	DWORD dwStyle0, dwStyle;

	single_ = *single;
	tab_ = *tab;

	Height = (int)(((float)HEIGHT * 1.05) + 0.5);
	Width = (int)(((float)WIDTH * 0.73) + 0.5);

	edit_file = (BOOL)(edit_params & 0x100) / 0x100;

	new_bold = (BOOL)(edit_params & 2)/2;
	new_italics = (BOOL)(edit_params & 4)/4;
	new_underline = (BOOL)(edit_params & 8)/8;
	new_adjust = (int)(edit_params & 48)/16;
	new_hidden = (int)(edit_params & 64) / 64;

	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	COLORREF myColor = 0xffcfc4; // (196, 207, 255);
	wcex.hbrBackground = CreateSolidBrush(myColor);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ALFA));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ALFA));
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = windowClass;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	if (first_time)
	{
		first_time = FALSE;
		if (!RegisterClassExW(&wcex))
		{
			MessageBox(NULL, TEXT("RegisterClassEx Failed!"), TEXT("Error"), MB_ICONERROR);
			return NULL;
		}
	}


	if (*single == 0) { dwStyle0 = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_EX_TOPMOST; 
						uHeight_= uHeight;
						BUT_DY = 89;
						COMBO_DY = 99 + Height; 
						EDIT_DY = 111 + Height;
						dHeight = 60;
						}
	else              { dwStyle0 = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_EX_TOPMOST; 
						uHeight_ = Height*2+107;
						BUT_DY =  58;
						COMBO_DY = 68 + Height;
						EDIT_DY = 80 + Height; 
						dHeight = 10;
						}

	if (!(hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, windowClass, windowTitle, dwStyle0, iX, iY, uWidth, uHeight_, NULL, NULL, hInstance, NULL)))
	{
		MessageBox(NULL, TEXT("CreateWindow Failed!"), TEXT("Error"), MB_ICONERROR);
		return NULL;
	}

	editor_hWnd = hWnd;

	DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_DONOTROUND;
	DwmSetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));


	if (*single > 0)
	{
		LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
		lStyle |= WS_THICKFRAME;
		lStyle = lStyle & ~WS_CAPTION;
		SetWindowLong(hWnd, GWL_STYLE, lStyle);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}


	switch (new_adjust)
	{
	case 0: alignement = ES_LEFT;
		break;
	case 1: alignement = ES_RIGHT;
		break;
	case 2: alignement = ES_CENTER;
		break;
	case 3: alignement = ES_CENTER;
		break;
	default:alignement = ES_LEFT;
		break;
	}

	LoadLibrary(TEXT("Msftedit.dll"));
	
	if (*single==0) dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | alignement | EM_FMTLINES ^ ES_UPPERCASE | ES_WANTRETURN | ES_MULTILINE /* | ES_OEMCONVERT */ | ES_AUTOHSCROLL | ES_AUTOVSCROLL /* | WS_BORDER | WS_EX_TOPMOST*/;
	           else dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | alignement | EM_FMTLINES ^ ES_UPPERCASE /* | ES_WANTRETURN*/  /* | ES_MULTILINE*/ /* | ES_OEMCONVERT */ | ES_AUTOHSCROLL /* | WS_BORDER  | WS_EX_TOPMOST*/;
																																																									     //  5, 0
	hwndEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"Edit", L"", dwStyle, 10, 10, uWidth - 20, uHeight_ - dHeight, hWnd, (HMENU)ID_EDITCHILD, (HINSTANCE)(long long)GetWindowLongW(hWnd, GWL_HINSTANCE), NULL);
	    

	HDC hdc = GetDC(hwndEdit);

	
	//italic underline

	oldEditProc = (WNDPROC)SetWindowLongPtrW(hwndEdit, GWLP_WNDPROC, (LONG_PTR)subEditProc);

	//normal
	hFont[0] = CreateFontW(Height, Width, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET /*EASTEUROPE_CHARSET ANSI_CHARSET*/, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS , (LPCWSTR)font_family_name);
	//underlined
	hFont[1] = CreateFontW(Height, Width, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE, DEFAULT_CHARSET /*EASTEUROPE_CHARSET ANSI_CHARSET*/, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, (LPCWSTR)font_family_name);
    //bold normal
	hFont[2] = CreateFontW(Height, Width, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET /*EASTEUROPE_CHARSET ANSI_CHARSET*/, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, (LPCWSTR)font_family_name);
	//bold underlined
	hFont[3] = CreateFontW(Height, Width, 0, 0, FW_BOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET /*EASTEUROPE_CHARSET ANSI_CHARSET*/, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, (LPCWSTR)font_family_name);
    //italics normal
	hFont[4] = CreateFontW(Height, Width, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, DEFAULT_CHARSET /*EASTEUROPE_CHARSET ANSI_CHARSET*/, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, (LPCWSTR)font_family_name);
    //italics underlined
	hFont[5] = CreateFontW(Height, Width, 0, 0, FW_NORMAL, TRUE, TRUE, FALSE, DEFAULT_CHARSET /*EASTEUROPE_CHARSET ANSI_CHARSET*/, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, (LPCWSTR)font_family_name);
	//italics bold normal
	hFont[6] = CreateFontW(Height, Width, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, DEFAULT_CHARSET /*EASTEUROPE_CHARSET ANSI_CHARSET*/, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, (LPCWSTR)font_family_name);
	//italics bold underlined
	hFont[7] = CreateFontW(Height, Width, 0, 0, FW_BOLD, TRUE, TRUE, FALSE, DEFAULT_CHARSET /*EASTEUROPE_CHARSET ANSI_CHARSET*/, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, (LPCWSTR)font_family_name);


	font_choice = new_underline + new_bold * 2 + new_italics * 4;

	SendMessageW(hwndEdit, WM_SETFONT, WPARAM(hFont[font_choice]), TRUE);


	SendMessageW(hwndEdit, WM_SETTEXT, 0, (LPARAM)(LPCWSTR)mytext);
	SendMessageW(hwndEdit, EM_SETLIMITTEXT, (WPARAM)0xFA00, 0);
	

	/* Get current length of text in the box */
	int index = GetWindowTextLength(hwndEdit);
	SetFocus(hwndEdit);
	/* Set the caret to the end of the text in the box */
	SendMessage(hwndEdit, EM_SETSEL, (WPARAM)index, (LPARAM)index);
	/*  in case of setting it in CallWindowProc()-> case WM_APPEND_EDIT:
	    "Replace" the selection (the selection is actually targeting
		nothing and just sits at the end of the text in the box)
		with the passed in TCHAR* from the button control that
		sent the WM_APPEND_EDIT message */

	hwndCombobox = CreateWindowExW(0,
		L"COMBOBOX",  // Predefined class; Unicode assumed 
		NULL,//"|<",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	oldComboboxProc = (WNDPROC)SetWindowLongPtrW(hwndCombobox, GWLP_WNDPROC, (LONG_PTR)subComboboxProc);

	hwndButtonUnderlined = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL, //"|<",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_BITMAP | BS_BOTTOM,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_UNDERLINED,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndButtonAlt = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL, //"B",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_BITMAP | BS_BOTTOM,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_ALT,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndButtonBold = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL, //"B",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP | BS_BOTTOM,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_BOLD,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndButtonItalic = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL, //"I",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP | BS_BOTTOM,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_ITALIC,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndButtonLeft = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL, //"|<",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP | BS_BOTTOM,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_LEFT,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndButtonCenter = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		L">|<",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_CENTER,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	if (single_ == 1)
	{
		hwndButtonMiddleCenter = CreateWindowExW(0,
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"≥|≤",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,  // Styles 
			0,         // x position 
			0,         // y position 
			40,        // Button width
			38,        // Button height
			hWnd,     // Parent window
			(HMENU)ID_BUTTON_MIDDLE_CENTER,       // No menu.
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL);      // Pointer not needed.
	}

	hwndButtonRight = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		L">|",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_RIGHT,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndButtonHidden = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"..",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_HIDDEN,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndButtonCopyText = CreateWindowExW(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Copy",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,  // Styles 
		0,         // x position 
		0,         // y position 
		40,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_COPY_TEXT,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.


	HWND hwnd = CreateToolTip(ID_BUTTON_BOLD, hWnd, hInstance, __BOLD__);
	HWND hwnd1 = CreateToolTip(ID_BUTTON_UNDERLINED, hWnd, hInstance, __UNDERLINED__);
	HWND hwnd2 = CreateToolTip(ID_BUTTON_ITALIC, hWnd, hInstance, __ITALIC__);
	HWND hwnd3 = CreateToolTip(ID_BUTTON_LEFT, hWnd, hInstance, __A_TO_LEFT__);
	HWND hwnd4 = CreateToolTip(ID_BUTTON_RIGHT, hWnd, hInstance, __A_TO_RIGHT__);
	HWND hwnd5 = CreateToolTip(ID_BUTTON_CENTER, hWnd, hInstance, __A_TO_MIDDLE__);
	HWND hwnd6 = CreateToolTip(ID_BUTTON_MIDDLE_CENTER, hWnd, hInstance, __A_TO_CENTRE__);
	HWND hwnd7 = CreateToolTip(ID_BUTTON_HIDDEN, hWnd, hInstance, __HIDDEN__);
	HWND hwnd8 = CreateToolTip(ID_BUTTON_COPY_TEXT, hWnd, hInstance, __COPY_TEXT__);  //PTSTR pszText

	hwndButtonOK = CreateWindowW(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"OK",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		0,         // x position 
		0,         // y position 
		60,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_OK,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndButtonESC = CreateWindowW(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"ESC",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
		0,         // x position 
		0,         // y position 
		60,        // Button width
		38,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_ESC,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	HANDLE hBitmapLeft;
	HANDLE hBitmapCenter;
    HANDLE hBitmapMiddleCenter;
	HANDLE hBitmapRight;

	if (single_ == 0)
	{

		hBitmapLeft = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_LEFT), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		hBitmapCenter = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_CENTER), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		hBitmapRight = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_RIGHT), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	}
	else
	{
		hBitmapLeft = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_LEFT), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		hBitmapCenter = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_CENTER), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		hBitmapMiddleCenter = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_MIDDLE_CENTER), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		hBitmapRight = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_RIGHT), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	}
	HANDLE hBitmapCopyText = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_COPY_TEXT), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	HANDLE hBitmapUnderlined = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_UNDERLINED), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	HANDLE hBitmapBold = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_BOLD), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	HANDLE hBitmapItalic = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_ITALIC), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	HANDLE hBitmapHidden = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_HIDDEN), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	HANDLE hBitmapAlt = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_ALT), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	

	SendMessageW(hwndButtonLeft, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapLeft);
	SendMessageW(hwndButtonCenter, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapCenter);
	if (single_==1) SendMessageW(hwndButtonMiddleCenter, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapMiddleCenter);
	SendMessageW(hwndButtonRight, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapRight);
	SendMessageW(hwndButtonCopyText, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapCopyText);
	SendMessageW(hwndButtonUnderlined, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapUnderlined);
	SendMessageW(hwndButtonBold, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapBold);
	SendMessageW(hwndButtonItalic, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapItalic);
	SendMessageW(hwndButtonHidden, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapHidden);
	SendMessageW(hwndButtonAlt, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapAlt);

	if (edit_file)
	{
		ShowWindow(hwndCombobox, SW_HIDE);
		ShowWindow(hwndButtonLeft, SW_HIDE);
		ShowWindow(hwndButtonCenter, SW_HIDE);
		if (single_==1) ShowWindow(hwndButtonMiddleCenter, SW_HIDE);
		ShowWindow(hwndButtonRight, SW_HIDE);
		ShowWindow(hwndButtonCopyText, SW_HIDE);
		ShowWindow(hwndButtonUnderlined, SW_HIDE);
		ShowWindow(hwndButtonBold, SW_HIDE);
		ShowWindow(hwndButtonItalic, SW_HIDE);
		ShowWindow(hwndButtonHidden, SW_HIDE);
		ShowWindow(hwndButtonAlt, SW_HIDE);
	}
	else
	{
		ShowWindow(hwndButtonAlt, SW_HIDE);
	}

	SendMessageW(hwndButtonOK, WM_SETFONT, WPARAM(hFont[0]), TRUE);
	SendMessageW(hwndButtonESC, WM_SETFONT, WPARAM(hFont[0]), TRUE);

	SendMessageW(hwndCombobox, WM_SETFONT, WPARAM(hFont[0]), TRUE);

	int string_no = 0;
	char *ptrsz_list;
	long_long ptrsz_long;
	int max_len = 0;
	int len_str = 0;
	int length;

	char Assets[65];
	char uAssets[256]="\0\0";
	int k=0;

	get_strings_list(&string_no, &ptrsz_long);
	ptrsz_list = (char *)ptrsz_long;


	for (k = 0; k < string_no; k+=1)
	{
		
		strncpy((char *)&Assets, (char *)(ptrsz_list + len_str), 64);
		Assets[64] = '\0';

		len_str += (int)strlen(Assets) + 1;

		length = utf82unicode((unsigned char *)Assets, (unsigned char *)uAssets);

		SendMessageW(hwndCombobox, (UINT) CB_INSERTSTRING, (WPARAM) 0, (LPARAM) uAssets);
	}
	
	
	UpdateWindow(hWnd);

	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	GetWindowRect(hWnd, &lpRect);
	SetWindowPos(hwndEdit, 0, 5, 0, lpRect.right - lpRect.left - 30, lpRect.bottom - lpRect.top - EDIT_DY, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	
	SetWindowPos(hwndCombobox, 0, 10, lpRect.bottom - lpRect.top - COMBO_DY, lpRect.right - lpRect.left - 40, 180, SWP_NOOWNERZORDER | SWP_NOZORDER);

	SetWindowPos(hwndButtonAlt, 0, 10, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(hwndButtonBold, 0, 55, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(hwndButtonItalic, 0, 100, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(hwndButtonUnderlined, 0, 145, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(hwndButtonLeft, 0, 190, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	if (single_ == 0)
	{
		SetWindowPos(hwndButtonCenter, 0, 235, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonRight, 0, 280, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonHidden, 0, 325, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonCopyText, 0, 380, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	
	}
	else
	{
		SetWindowPos(hwndButtonCenter, 0, 235, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonMiddleCenter, 0, 280, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonRight, 0, 325, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonHidden, 0, 370, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndButtonCopyText, 0, 425, lpRect.bottom - lpRect.top - BUT_DY, 40, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	}
	SetWindowPos(hwndButtonOK, 0, lpRect.right - lpRect.left - 180, lpRect.bottom - lpRect.top - BUT_DY, 60, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(hwndButtonESC, 0, lpRect.right - lpRect.left - 110, lpRect.bottom - lpRect.top - BUT_DY, 60, 38, SWP_NOOWNERZORDER | SWP_NOZORDER);
	
	SetFocus(hwndEdit);


	MSG msg;

	while (GetMessageW(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_SETCURSOR) {
			int i = 0;
		} 
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return hWnd;
}

int set_window_icon(void)
{
	HWND hwnd = win_get_window();

	HANDLE i = LoadImageW(NULL, L"alfa.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE | LR_SHARED);
	if (i)
	{
		SendMessageA(hwnd, WM_SETICON, ICON_BIG, (LPARAM)i);
	}
	HANDLE i16 = LoadImageW(NULL, L"alfa.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE | LR_SHARED);
	if (i16)
	{
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)i16);
	}
	return 0;
}

void mazovia2windows(char *tekst)
/*-----------------------------*/
{
	int i, dl_tekst;
	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;
	for (i = 0; i < dl_tekst; i++)
	{
		switch (tekst[i])
		{
		case '\217':tekst[i] = '\245';  //A
			break;
		case '\225':tekst[i] = '\306';  //C
			break;
		case '\220':tekst[i] = '\312';  //E
			break;
		case '\234':tekst[i] = '\243';  //L
			break;
		case '\245':tekst[i] = '\321';  //N
			break;
		case '\243':tekst[i] = '\323';  //O
			break;
		case '\230':tekst[i] = '\214';  //S
			break;
		case '\240':tekst[i] = '\217';  //Z'
			break;
		case '\241':tekst[i] = '\257';  //Z.
			break;
		case '\206':tekst[i] = '\271';  //a
			break;
		case '\215':tekst[i] = '\346';  //c
			break;
		case '\221':tekst[i] = '\352';  //e
			break;
		case '\222':tekst[i] = '\263';  //l
			break;
		case '\244':tekst[i] = '\361';  //n
			break;
		case '\242':tekst[i] = '\363';  //o
			break;
		case '\236':tekst[i] = '\234';  //s
			break;
		case '\247':tekst[i] = '\277';  //z'
			break;
		case '\246':tekst[i] = '\237';  //z.
			break;
		default: break;
		}
	}
}

void windows2mazovia(char *tekst)
/*-----------------------------*/
{
	int i, dl_tekst;
	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;
	for (i = 0; i < dl_tekst; i++)
	{
		switch (tekst[i])
		{
		case '\245':tekst[i] = '\217';  //A
			break;
		case '\306':tekst[i] = '\225';  //C
			break;
		case '\312':tekst[i] = '\220';  //E
			break;
		case '\243':tekst[i] = '\234';  //L
			break;
		case '\321':tekst[i] = '\245';  //N
			break;
		case '\323':tekst[i] = '\243';  //O
			break;
		case '\214':tekst[i] = '\230';  //S
			break;
		case '\217':tekst[i] = '\240';  //Z'
			break;
		case '\257':tekst[i] = '\241';  //Z.
			break;
		case '\271':tekst[i] = '\206';  //a
			break;
		case '\346':tekst[i] = '\215';  //c
			break;
		case '\352':tekst[i] = '\221';  //e
			break;
		case '\263':tekst[i] = '\222';  //l
			break;
		case '\361':tekst[i] = '\244';  //n
			break;
		case '\363':tekst[i] = '\242';  //o
			break;
		case '\234':tekst[i] = '\236';  //s
			break;
		case '\277':tekst[i] = '\247';  //z'
			break;
		case '\237':tekst[i] = '\246';  //z.
			break;
		default: break;
		}
	}
}

void unicode2utf8(char *unicodetext, unsigned char *utf8text)
{
	unsigned short *zn;
	unsigned char *utf8ptr;
	int bytes_n;
	uint8_t utf8c[4];
	
	zn = (unsigned short *)unicodetext;
	utf8ptr = utf8text;

	while (*zn != '\0')
	{
		if (*zn >= 127)
		{
			bytes_n = ucs2_to_utf8(*zn, (uint8_t*)utf8c);
			if ((*zn < 1920) && (bytes_n<3))
			{
				*utf8ptr = utf8c[0];  //0xC4  196
				utf8ptr++;
				*utf8ptr = utf8c[1]; // 0x84  132
				utf8ptr++;
			}
			else
			{
				*utf8ptr = 32;
				utf8ptr++;
			}
		}
		else
		{
			*utf8ptr = zn[0];
			utf8ptr++;
		}
      zn++;
	}

	*utf8ptr = '\0';
}

//void utf82win(char *utf8text, char *wintext)
void utf82win(char* utf8text, LPCWSTR wintext)
{
	int result;
	result = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)utf8text, -1, (LPWSTR)wintext, 2048);  //255
}

//void encodewin(char* utf8text, char* wintext)
void encodewin(char* utf8text, LPCWSTR wintext)
{
	int ret;
	int count;
	char unicodetext[2048]; //MaxTextLen

	//count = utf82unicode(utf8text, &unicodetext);
	//unicode2win(unicodetext, wintext, count);

	utf82win(utf8text, wintext);
}

void Put_Str_To_Clip(char* ptrsz_buf)
{
	//char ptrsz_sz_tmp[2048];  //MaxTextLen
	LPCWSTR ptrsz_sz_tmp = new WCHAR[2048] ;

	//utf82win(ptrsz_buf, &ptrsz_sz_tmp);
	//PutStringToClipboard(&ptrsz_sz_tmp);

	encodewin(ptrsz_buf, ptrsz_sz_tmp);  //UTF-8 original
	PutStringToClipboard(ptrsz_sz_tmp);

	////PutStringToClipboard(ptrsz_buf);
}




void unicode2windxf(char *unicodetext, char *wintext, int count)
{
	unicode2winfactory(unicodetext, wintext, count, dxf_encoding);
}

void unicode2win(char *unicodetext, char *wintext, int count)
{
	unicode2winfactory(unicodetext, wintext, count, code_page);
}

void unicode2winfactory(char *unicodetext, char *wintext, int count, int codepage)
{
	int result;

	if (count == 0)
	{
		strcpy(wintext, ""); 
		return;
	}
	int Size = WideCharToMultiByte(codepage, 0, (LPWSTR)unicodetext, count, NULL, 0, NULL, NULL);
	if (Size == 0)
	{
		strcpy(wintext, "");
		return;
	}
	result = WideCharToMultiByte(codepage, 0, (LPWSTR)unicodetext, -1, (LPSTR)wintext, Size, NULL, NULL);
}

void unicode2win___(char *unicodetext, unsigned char *wintext)
{
	unsigned short *zn;
	unsigned char *winptr;


	zn = (unsigned short *)unicodetext;
	winptr = wintext;

	while (*zn != '\0')
	{
		if (*zn < 1920)
		{
			*winptr = zn[1];
		}
		else *winptr = ' ';

		winptr++;
		
		zn++;
	}
	*winptr = '\0';
}

int win2unicodedxf(char *wintext, char *unicodetext)
{
	return win2unicodefactory(wintext, unicodetext, dxf_encoding);
}

int win2unicode(char *wintext, char *unicodetext)
{
	return win2unicodefactory(wintext, unicodetext, code_page);
}

int win2unicodefactory(char *wintext, char *unicodetext, int codepage)
{
	int result;
	result = MultiByteToWideChar(codepage, 0, (LPCCH)wintext, -1, (LPWSTR)unicodetext, (int)strlen(wintext) + 1);
	return result;
}

int win2unicode__(unsigned char *wintext, unsigned char *unicodetext)
{
	unsigned char *zn;
	
	unsigned short *unicodeptr;
	int count = 0;

	zn = (unsigned char*)wintext;
	unicodeptr = (unsigned short*)unicodetext;

	while (*zn != '\0')
	{
		*unicodeptr = *zn;
		unicodeptr++;
		
		count += 2;
		
		zn++;
	}

	*unicodeptr = 0;

	count += 2;
	return count;
}


int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext)
{
	unsigned char *zn;
	unsigned char c1, c2;
	unsigned int u8zn;
	const uint8_t *end_ptr;
	unsigned char *unicodeptr;
	int count=0;
	
	zn = (unsigned char*)utf8text;
	unicodeptr = (unsigned char*)unicodetext;

	while (*zn != '\0')
	{
		if (*zn >= 127)
		{
			//convert to UNICODE and shift index
			u8zn = utf8_to_ucs2((const uint8_t  *)zn, &end_ptr);
			if (u8zn > 1920)
			{
				*unicodeptr = 32;
				unicodeptr++;
				*unicodeptr = '\0';
				unicodeptr++;
				count +=2 ;
			}
			else
			{   
				c1 = (unsigned char)(u8zn & 0xFF);
				c2 = (unsigned char)(u8zn >> 8);	
				*unicodeptr = c1;
				unicodeptr++;
				*unicodeptr = c2;
				unicodeptr++;
				count += 2;
			}
			
			zn++;
		}
		else
		{
			*unicodeptr = *zn;
			unicodeptr++;
			*unicodeptr = '\0';
			unicodeptr++;
			count += 2;
		}
	  zn++;
	}

	*unicodeptr = '\0';
	unicodeptr++;
	*unicodeptr = '\0';
	
	count += 2;
	return count;
}

#define MaxMultitextLen 4096


int EditText(char *mytext, int edit_params, int nCmdShow, int *single, int *tab)
{
	HWND retHWND;
	char my_mb_text[MaxMultitextLen];
	CStringW unicode;
	int length;
	static int curr_h, curr_v;
	int ret;
	double PozX0, PozY0;

	sw_callback_locked_proc();

	strcpy(my_mb_text,"");

	length=utf82unicode((unsigned char *)mytext, (unsigned char *)my_mb_text);


	LPWSTR strBuf = unicode.GetBufferSetLength(length+2);
	memset( strBuf, 0, length+2 );
	memcpy_s(strBuf, length, my_mb_text, length-2);
	unicode.ReleaseBuffer();
	

	LPWSTR windowTitle = __EDIT_MULTILINE__;

	if (*single == 0) memmove(&lpRect, &lpRect_edit, sizeof(RECT));
	else memmove(&lpRect, &lpRect_single, sizeof(RECT));


    ret=Save_Update_flex(0, &curr_h, &curr_v);

    get_posXY(&PozX0, &PozY0);

    //position_mouse((int)pikseleX(PozX0), (int)pikseleY(PozY0));
	//position_mouse((lpRect.left + lpRect.right) / 2, (lpRect.top + lpRect.bottom) / 2);
	int x_win, y_win, h_win, v_win;
	int ret_ref = get_window_origin_and_size(&x_win, &y_win, &h_win, &v_win);
	position_mouse(lpRect.right+5-x_win, lpRect.bottom+5-y_win);

    _free_mouse();
    dialog_cursor(1);

	editor_on = TRUE;

	retHWND = CreateTextBoxW(unicode, edit_params, lpRect.left, lpRect.top, lpRect.right- lpRect.left, lpRect.bottom- lpRect.top, 0, NULL, my_hInstance, nCmdShow, windowTitle, single, tab);

	editor_on = FALSE;

    dialog_cursor(0);
    lock_mouse();
    set_posXY(PozX0, PozY0);
    CUR_ON(PozX0,PozY0);
    ret=Save_Update_flex(1, &curr_h, &curr_v);


	if (*single == 0) memmove(&lpRect_edit, &lpRect, sizeof(RECT));
	else if (*single == 1) memmove(&lpRect_single, &lpRect, sizeof(RECT));

	unicode.ReleaseBuffer(length);
	sw_callback_locked_proc();

	editor_on = FALSE;

	if (ret_value == 1)
	{
		

		unicode2utf8(ret_text, (unsigned char*)mytext);

		new_params = 1 + new_bold * 2 + new_italics * 4 + new_underline * 8 + new_adjust * 16 + new_hidden * 64;


		return new_params;
	}
	else return ret_value;

}


int EditFile(char *filename, int edit_params, int nCmdShow)
{
	HWND ret;
	DWORD read = -1;
	DWORD write = -1;
	
	unsigned char *my_data;
	unsigned char *my_mb_data;
	CStringW unicode;
	int length;
	int single = 0;
	int tab = 0;

	//loading file
	HANDLE hFile = CreateFile((LPCSTR)filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD filesize = GetFileSize(hFile, NULL);
	
	my_data = (unsigned char *)GlobalAlloc(GPTR, filesize +1);
	my_mb_data = (unsigned char *)GlobalAlloc(GPTR, (filesize * 2) + 2);

	
	if (!ReadFile(hFile, my_data, filesize, &read, NULL))
	{
		CloseHandle(hFile);
		sw_callback_locked_proc();
		return -1; //error reading
	}

	CloseHandle(hFile);
	
	*my_mb_data = '\0';
	length = utf82unicode(my_data, my_mb_data);
	GlobalFree(my_data);

	LPWSTR strBuf = unicode.GetBufferSetLength(length + 2);
	memset(strBuf, 0, length + 2);
	memcpy_s(strBuf, length, my_mb_data, length - 2);
	unicode.ReleaseBuffer();

	GlobalFree(my_mb_data);
	
	LPWSTR windowTitle = (LPWSTR)filename_ini;

	memmove(&lpRect, &lpRect_file, sizeof(RECT));

	editor_on = TRUE;
	ret = CreateTextBoxW(unicode, edit_params | 0x100, lpRect_file.left, lpRect_file.top, lpRect_file.right - lpRect_file.left, lpRect_file.bottom - lpRect_file.top, 0, NULL, my_hInstance, nCmdShow, windowTitle, &single, &tab);
	//editor_hWnd = ret;
	// 
	// 
	editor_on = FALSE;

	if (ret_value == 1)
	{
		my_data = (unsigned char *)GlobalAlloc(GPTR, sizeof(ret_text));
		
		unicode2utf8(ret_text, my_data);

		//saving file
		HANDLE hFile1 = CreateFileA(filename, GENERIC_WRITE, 0/*FILE_SHARE_READ*/, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!WriteFile(hFile1, static_cast<LPVOID>(my_data), (int)strlen((const char*)my_data), &write, NULL))
		{
			CloseHandle(hFile1);
			GlobalFree(my_data);
			sw_callback_locked_proc();
			return -2; //error writing#
		}
		CloseHandle(hFile1);
		GlobalFree(my_data);
		sw_callback_locked_proc();
		return 1;
	}
	else
	{
		sw_callback_locked_proc();
		return 0;
	}

}

int testCall(int val)
{
	return val + 1;
}

int GoRegtestCall(int(*ptr)(int))
{
	return RegtestCall(ptr);
}

void thecallback(int val)
{
	return;
}

void SetForegroundWindowInternal(HWND hWnd)
{
	if (!::IsWindow(hWnd)) return;

	BYTE keyState[256] = { 0 };
	//to unlock SetForegroundWindow we need to imitate Alt pressing
	if (::GetKeyboardState((LPBYTE)&keyState))
	{
		if (!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		}
	}

	::SetForegroundWindow(hWnd);

	if (::GetKeyboardState((LPBYTE)&keyState))
	{
		if (!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}
}

void SetForegroundWindowInternal1(HWND hWnd)
{
	if (!::IsWindow(hWnd)) return;

	//relation time of SetForegroundWindow lock
	DWORD lockTimeOut = 0;
	HWND  hCurrWnd = ::GetForegroundWindow();
	DWORD dwThisTID = ::GetCurrentThreadId(),
		dwCurrTID = ::GetWindowThreadProcessId(hCurrWnd, 0);

	//we need to bypass some limitations from Microsoft :)
	if (dwThisTID != dwCurrTID)
	{
		::AttachThreadInput(dwThisTID, dwCurrTID, TRUE);

		::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &lockTimeOut, 0);
		::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);

		::AllowSetForegroundWindow(ASFW_ANY);
	}

	::SetForegroundWindow(hWnd);

	if (dwThisTID != dwCurrTID)
	{
		::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)(long_long)lockTimeOut, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
		::AttachThreadInput(dwThisTID, dwCurrTID, FALSE);
	}
}

void Set_Focus(HWND HWnd)
{
	SetForegroundWindowInternal(HWnd);
}

char *GetStringFromClipboard(void)
{
	HANDLE clip;
	BOOL ret;
	
	if (OpenClipboard(NULL))
	{
		clip = GetClipboardData(CF_TEXT);
		ret=CloseClipboard();
	}
	return (char*)clip;
}

void PutStringToClipboard(LPCWSTR ptrsz_buf)
{
	

	if (OpenClipboard(NULL))
	{
		
		EmptyClipboard();
		HGLOBAL hClipboardData;
		
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, wcslen(ptrsz_buf)*2+2);

        char * pchData;
		pchData = (char*)GlobalLock(hClipboardData);

		memmove(pchData, ptrsz_buf, wcslen(ptrsz_buf)*2+2);
		
		GlobalUnlock(hClipboardData);

		SetClipboardData(CF_UNICODETEXT, hClipboardData);  //CF_TEXT
		
		CloseClipboard();
	}
	return;
}


void mouseMove(int dx, int dy)
	{
		INPUT input;
	
		input.type = INPUT_MOUSE;
	
		input.mi.dx = dx;
		input.mi.dy = dy;
		input.mi.mouseData = 0;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
	
		UINT uSent = SendInput(1, &input, sizeof(INPUT));
	}


char* get_font_family_name(void)
{
	return font_family_name;
}


#else //LINUX
int new_adjust;
BOOL new_bold;
BOOL new_italics;
BOOL new_underline;
BOOL new_hidden;
int new_params;
#define USES_CONVERSION int _convert = 0; (_convert); UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)

char *get_font_family_name(void)
{
    return font_family_name;
}

int set_window_icon(void)  //TO DO
{
    return 1;
}

void get_tens_value(char* st, double tens)
{
    sprintf(st, "%#12.9lg", tens);
}

int EditText(char *mytext, int edit_params, int nCmdShow, int *single, int *tab) {
    char *my_text;
    int new_edit_params;

    int HeightI = (int)((float)HEIGHT*0.9);
    int WidthI = (int)((float)WIDTH*0.9);

    char Height[16];
    char Width[16];
    char New_Edit_Params[16];
    double PozX0, PozY0, PozX01, PozY01;
    double DX,DY;
    int ret;
    static int curr_h, curr_v;

    char eTitle[64];

    sprintf(eTitle, "%s(%d)",_EDIT_TEXT_, Client_number);

    sprintf(Height,"%d", HeightI);
    sprintf(Width, "%d", WidthI);
    sprintf(New_Edit_Params,"%d",edit_params);

    ret=Save_Update_flex(0, &curr_h, &curr_v);

    get_posXY(&PozX0, &PozY0);

    position_mouse(pikseleX(PozX0), pikseleY(PozY0));
    _free_mouse();
    dialog_cursor(1);

    my_text = tinyfd_editBox(eTitle/*"Edit text"*/, mytext, 0, (char *) &font_family_name, (char *) &font_file, (char*)&Height, (char *)&Width, "TEXT", (char*)&New_Edit_Params, single);

    dialog_cursor(0);
    lock_mouse();

    set_posXY(PozX0, PozY0);
    CUR_ON(PozX0,PozY0);

    ret=Save_Update_flex(1, &curr_h, &curr_v);

    if (*single==-1) return -1;


    if (my_text != NULL)
    {

        new_edit_params=atoi(New_Edit_Params);

        new_bold = (BOOL)((new_edit_params & 2)/2);
        new_italics = (BOOL)((new_edit_params & 4)/4);
        new_underline = (BOOL)((new_edit_params & 8)/8);
        new_adjust = (int)((new_edit_params & 48)/16);
        new_hidden = (BOOL)((new_edit_params & 64)/64);

        new_edit_params = 1 + new_bold * 2 + new_italics * 4 + new_underline * 8 + new_adjust * 16 + new_hidden * 64;

        strcpy(mytext, my_text);

        return new_edit_params;
    }
    else return 0;

}

#define MAX_INI_LENGTH 32000

int EditFile(char *filename, int edit_params, int nCmdShow)
{
    char *my_text;
    char *mytext;
    int new_edit_params;
    BOOL ret;
    char *fname;

    int HeightI = (int)((float)HEIGHT*0.9);
    int WidthI = (int)((float)WIDTH*0.9);

    char Height[16];
    char Width[16];
    char New_Edit_Params[16];
    double PozX0, PozY0;
    FILE *f;
    int flength;
    BOOL reload_ini;
    int single = 0;
    long sz;
    int ret1;
    static int curr_h, curr_v;

    //reading file
    f = fopen(filename, "rt");
    if (f == NULL)
    {
        return 0;
    }
    else
    {
        fseek(f, 0L, SEEK_END);
        sz = ftell(f);
        fseek(f, 0L, SEEK_SET);

        mytext=(char*)malloc(sz*1.25);

        flength=fread(mytext, 1, sz, f);
        fclose(f);
    }

    mytext[flength]='\0';

    sprintf(Height,"%d", HeightI);
    sprintf(Width, "%d", WidthI);
    sprintf(New_Edit_Params,"%d",edit_params);

    ret1=Save_Update_flex(0, &curr_h, &curr_v);

    get_posXY(&PozX0, &PozY0);
    _free_mouse();
    dialog_cursor(1);

    my_text = tinyfd_editBox("Edit file", mytext, 0, (char *) &font_family_name, (char *) &font_file, (char*)&Height, (char *)&Width, "FILE", (char*)&New_Edit_Params, &single);

    dialog_cursor(0);
    lock_mouse();

    set_posXY(PozX0, PozY0);
    CUR_ON(PozX0,PozY0);

    ret=Save_Update_flex(1, &curr_h, &curr_v);

    if (my_text != NULL)
    {
        new_edit_params=atoi(New_Edit_Params);

        new_bold = (BOOL)((new_edit_params & 2)/2);
        new_italics = (BOOL)((new_edit_params & 4)/4);
        new_underline = (BOOL)((new_edit_params & 8)/8);
        new_adjust = (int)((new_edit_params & 48)/16);
        reload_ini=(BOOL)((new_edit_params & 128)/128);

        new_edit_params = 1 + new_bold * 2 + new_italics * 4 + new_underline * 8 + new_adjust * 16 + new_hidden * 64;

        ret = file_bak(filename, (char*)EXT__INI);
        fname = File_Ext(filename, (char*)EXT__INI);

       //saving file
        f = fopen(filename, "w");
        if (f == NULL)
        {
            free(mytext);
            return 0;
        }
        else
        {
            flength = fputs(my_text,f);
            fclose(f);
        }

        free(mytext);
        return new_edit_params;
    }
    else
    {
        free(mytext);
        return 0;
    }

}

int StringToWString(std::wstring &ws, const std::string &s)
{
    std::wstring wsTmp(s.begin(), s.end());

    ws = wsTmp;

    return 0;
}

void find_font_face(char *Font_File)
{
    TTF ttf;
    std::wstring wstr;
#ifndef LINUX
    USES_CONVERSION;
    ttf.Parse(A2W((LPCSTR)Font_File));
#else
    std::wstring Font_FileW;
    std::string Font_FileS(Font_File, strlen(Font_File));
    int ret = StringToWString(Font_FileW, Font_FileS);

    ttf.Parse(Font_FileW);
#endif
    wstr = ttf.GetFontName().c_str();
    std::string sstr;

    sstr = WStringToString(wstr);

    sprintf(font_family_name, "%ls", wstr.c_str());
    strcpy(font_file, Font_File);
}

void find_any_font_face(char *Font_File, char *face_name)
{
    TTF ttf;
    std::wstring wstr;
#ifndef LINUX
    USES_CONVERSION;
    ttf.Parse(A2W((LPCSTR)Font_File));
#else
    std::wstring Font_FileW;
    std::string Font_FileS(Font_File, strlen(Font_File));
    int ret = StringToWString(Font_FileW, Font_FileS);

    ttf.Parse(Font_FileW);
#endif
    wstr = ttf.GetFontName().c_str();
    std::string sstr;

    sstr = WStringToString(wstr);

    sprintf(face_name, "%s", sstr.c_str());
}

void unicode2win(char *unicodetext, char *wintext, int count)   //TO DO
{
    wintext=unicodetext;
}

int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext)
{
    unsigned char *zn;
    unsigned char c1, c2;
    unsigned int u8zn;
    const uint8_t *end_ptr;
    unsigned char *unicodeptr;
    int count=0;

    zn = (unsigned char*)utf8text;
    unicodeptr = (unsigned char*)unicodetext;

    while (*zn != '\0')
    {
        if (*zn >= 127)
        {
            //convert to UNICODE and shift index
            u8zn = utf8_to_ucs2((const uint8_t  *)zn, &end_ptr);
            if (u8zn > 1920)
            {
                *unicodeptr = 32;
                unicodeptr++;
                *unicodeptr = '\0';
                unicodeptr++;
                count +=2 ;
            }
            else
            {
                c1 = (unsigned char)(u8zn & 0xFF);
                c2 = (unsigned char)(u8zn >> 8);
                *unicodeptr = c1;
                unicodeptr++;
                *unicodeptr = c2;
                unicodeptr++;
                count += 2;
            }
            zn++;
        }
        else
        {
            *unicodeptr = *zn;
            unicodeptr++;
            *unicodeptr = '\0';
            unicodeptr++;
            count += 2;
        }
        zn++;
    }

    *unicodeptr = '\0';
    unicodeptr++;
    *unicodeptr = '\0';

    count += 2;
    return count;
}

#endif


#ifndef LINUX
bool Copy_File(char *ptrsz_fnd, char *ptrsz_fns)
{
    return CopyFile((LPCSTR)ptrsz_fns, (LPCSTR)ptrsz_fnd, FALSE);
}
#endif


void report_mem_leak_cpp_(void)
{

#ifndef DONTDOTHAT
#ifndef NDEBUG
#ifdef BIT64
	report_mem_leak_cpp();
#endif
#endif
#endif
}

#ifdef ALF_MAIN

#ifndef LINUX


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{ int ret;
#ifdef BIT64
  static char szAppName[] = "AlfaCAD4Win64";
#else
  static char szAppName[] = "AlfaCAD4Win";
#endif
  
   int child=0;
   char file_name[255];
   char    szAppPath[MAX_PATH] = "";
   CString strAppName;

   my_hInstance = hInstance;
 

   BOOL bret=SetProcessDPIAware();

   win_set_msg_pre_proc(MainWndProc);

   CoInitializeEx(NULL, COINIT_MULTITHREADED);


#ifndef LINUX
   locale = _create_locale(LC_ALL, _LOCALE_);
#else

#endif


   if (strstr(lpszCmdParam, "--NOCHDIR") != NULL)
   {
	   NOCHDIR = TRUE;
	   lpszCmdParam += 10;
   }

   child = 0;
   if (lpszCmdParam[strlen(lpszCmdParam) - 1] == '"') lpszCmdParam[strlen(lpszCmdParam) - 1] = '\0';
   if (lpszCmdParam[0] == '"') strcpy(file_name, lpszCmdParam + 1);
   else strcpy(file_name, lpszCmdParam);


  ::GetModuleFileName(0, (LPSTR)szAppPath, MAX_PATH);

  // Extract path
  strAppName = szAppPath;
  strAppName = strAppName.Left(strAppName.ReverseFind('\\'));

#ifndef _DEBUG
  //zmina katalogu
  if (NOCHDIR == FALSE)
  {
	  int ret1 = _chdir(strAppName);
  }
#endif

  SetCallback(thecallback);

  LoadLibrary("comctl32.dll");
    
  ret=Rysuj_main(child, file_name, nCmdShow, szAppPath, lpszCmdParam);

  CoUninitialize();

  return 0;
}


END_OF_MAIN()

#endif

#endif

#ifdef LINUX

void Put_Str_To_Clip(char *ptrsz_buf)
{
    clip::set_text(ptrsz_buf);
}

bool Get_Str_From_Clip(char *ptrsz_buf,
                       int i_poz,
                       int i_buflen,
                       int xpcz,
                       int ypcz)
/*---------------------------------*/
{
    std::string value;

    clip::get_text(value);

    const char *value_p=value.c_str();
    int pos=i_poz;
    size_t l1 = strlen(ptrsz_buf);
    size_t l2 = strlen(value_p);

    if (pos <  0) pos = 0;
    if (pos > l1) pos = l1;

    if ((l1+l2)>i_buflen) l2=i_buflen-l1;
    if (l2>0) {
        if (value_p[l2-1]>127) l2--;
        char *p = ptrsz_buf + pos;
        memmove(p + l2, p, l1 - pos);
        memmove(p, value_p, l2);
        p[l1+l2]='\0';
    }
    return TRUE;
}

#endif

#undef __ALFMAIN__
