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

#ifdef ALF_MAIN2

#define ALLEGRO_NO_MAGIC_MAIN

#define APPLICATION_NAME "AlfaCAD4Win"
#define ALFPRO_WND_CLASS "AWNDCLASS1"

#include <allegro.h>
#ifndef LINUX
#include <winalleg.h>
#else
#include <xalleg.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "leak_detector_cpp.hpp"


#ifdef __cplusplus
extern "C" {
#endif
extern int Rysuj_main(int child, char *file_name, char file_multiname);
int first_window_main=0;

#ifdef __cplusplus
}
#endif

HINSTANCE	app_hInst;
HWND		app_hWnd;

BITMAP *buffer, *cursor;



LRESULT CALLBACK A_WindowProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		
		case WM_CREATE: return 0;
		case WM_DESTROY: PostQuitMessage(0); return 0;
		case WM_KEYDOWN: case WM_KEYUP: case WM_SYSKEYDOWN: case WM_SYSKEYUP: return 0;
		case WM_PAINT: return 0;
		case WM_CLOSE: exit(0);
	
	case WM_SIZE: 
		{	
			return 0;
		}
		
		
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void A_HandleWindowsMessages (void)
{
	MSG msg;

	while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX	a_winClass;
	HWND		a_hWnd;

	int child=0;
	int ret;
	char file_name[255];

	app_hInst = hInstance;

	a_winClass.cbSize			= sizeof(WNDCLASSEX);
	a_winClass.style			= CS_HREDRAW | CS_VREDRAW;
	a_winClass.lpfnWndProc		= A_WindowProc;
	a_winClass.cbClsExtra		= 0;
	a_winClass.cbWndExtra		= 0;
	a_winClass.hInstance		= hInstance;
	a_winClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	a_winClass.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	a_winClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	a_winClass.hbrBackground	= NULL;
	a_winClass.lpszMenuName		= NULL;
	a_winClass.lpszClassName	= ALFPRO_WND_CLASS;

	if (!RegisterClassEx(&a_winClass)) return 0;

	if (!(a_hWnd = CreateWindowEx(NULL,ALFPRO_WND_CLASS,APPLICATION_NAME,
			WS_SIZEBOX   | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
			10,10,2500, 1560,NULL,NULL,hInstance,NULL))) return 0;

	app_hWnd = a_hWnd;

	ShowWindow(a_hWnd,SW_SHOWNORMAL);
	SetForegroundWindow(a_hWnd);
	UpdateWindow(a_hWnd);

	win_set_window(a_hWnd);
	strcpy(file_name,"");

	ret=Rysuj_main(child, file_name, file_multiname);
	
	END_OF_MAIN()

	return 0;
}

#endif