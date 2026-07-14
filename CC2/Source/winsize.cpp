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

#define __WINSIZE__

#ifndef LINUX
////#include <windows.h>
#include <wtypes.h>
#include <tchar.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef LINUX
#include <WinUser.h>
#include <windowsx.h>
#include "res/resource.h"
#else
typedef wchar_t TCHAR;
#define _T(x) L ##x
#include "res/resource.h"
#endif

#include "message.h"

#include "leak_detector_cpp.hpp"

#ifndef LINUX
static TCHAR szWindowClass[] = _T("     ");
static TCHAR szTitle[] = _T("");
HINSTANCE hInst;
RECT lpWinRect = { 100, 100, 600, 400 };
LRESULT CALLBACK WndProc1(HWND, UINT, WPARAM, LPARAM);
HWND hwndWinButtonOK;
HWND hwndWinButtonESC;
#endif

static char WClass[6];

static int x0, y0, dx, dy;

int win_ret = 0;

#define ID_BUTTON_OK 107
#define ID_BUTTON_ESC 108

#define BUT_DX 80
#define BUT_DY 60

extern "C" {
int x0_new, y0_new, dx_new, dy_new;
int dx_back=0, dy_back=0, x0_back=0, y0_back=0;

int WinSize(int x00, int y00, int dx0, int dy0, int NO) 
{
#ifndef LINUX
    WNDCLASSEX wcex;

	sprintf(szWindowClass,"S%d",NO);
	
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc1;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = GetModuleHandle(NULL); //hInstance;
	wcex.hIcon          = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ALFADIM));
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ALFADIM));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = NULL; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 0;
    }

	x0 = x00;  y0 = y00;  dx = dx0;  dy = dy0;

	if ((dx==0) || (dy==0)) return 0;

  
	    HWND hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST,
        szWindowClass,
		NULL,
        WS_SIZEBOX /*WS_THICKFRAME*/ | WS_OVERLAPPEDWINDOW ^ WS_MINIMIZEBOX,
		x0,y0,
		dx,dy,
        NULL,
        NULL,
        wcex.hInstance,
        NULL
    );



		
    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Error"),
            NULL);

        return 1;
    }

	SetLayeredWindowAttributes(hWnd, 0, (255 * 80) / 100, LWA_ALPHA);

	hwndWinButtonOK = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"OK",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		0,         // x position 
		0,         // y position 
		60,        // Button width
		40,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_OK,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndWinButtonESC = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"ESC",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		0,         // x position 
		0,         // y position 
		60,        // Button width
		40,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_BUTTON_ESC,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.




	ShowWindow(hWnd, SW_SHOWNORMAL);
	GetWindowRect(hWnd, &lpWinRect);

     UpdateWindow(hWnd);

	 SetFocus(hWnd);

	 SetWindowPos(hwndWinButtonOK, 0, (lpWinRect.right - lpWinRect.left)/2 +5, (lpWinRect.bottom - lpWinRect.top - BUT_DY)/2, BUT_DX, BUT_DY, SWP_NOOWNERZORDER | SWP_NOZORDER);
	 SetWindowPos(hwndWinButtonESC, 0, (lpWinRect.right - lpWinRect.left)/2 - BUT_DX -5, (lpWinRect.bottom - lpWinRect.top - BUT_DY) / 2, BUT_DX, BUT_DY, SWP_NOOWNERZORDER | SWP_NOZORDER);

	 SetCursorPos(x0+dx-5, y0+dy-5);



    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
#else   //TO DO

#endif
	return win_ret; 
}

} //extern C

#ifndef LINUX
LRESULT CALLBACK WndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

	RECT wndRect, clnRect, rc;
    int w,w_,h,h_;
	int xPos;
	int yPos;
	int yht, yhb;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);
		
		SetBkColor(hdc, 0x00929292);
		
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, 0, 0, 0);

        TextOut(hdc, 5, 5, greeting1, _tcslen(greeting1));
		TextOut(hdc, 5, 25, greeting2, _tcslen(greeting2));
		TextOut(hdc, 5, 45, greeting3, _tcslen(greeting3));
		TextOut(hdc, 5, 65, greeting4, _tcslen(greeting4));
			
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
		
		
		PostQuitMessage(EXIT_SUCCESS);
        break;
	case WM_SIZE:
		GetWindowRect(hWnd, &lpWinRect);

		SetWindowPos(hwndWinButtonOK, 0, (lpWinRect.right - lpWinRect.left) / 2 + 5, (lpWinRect.bottom - lpWinRect.top - BUT_DY) / 2, BUT_DX, BUT_DY, SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(hwndWinButtonESC, 0, (lpWinRect.right - lpWinRect.left) / 2 - BUT_DX - 5, (lpWinRect.bottom - lpWinRect.top - BUT_DY) / 2, BUT_DX, BUT_DY, SWP_NOOWNERZORDER | SWP_NOZORDER);

		break;
	case WM_NCMOUSEMOVE:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		GetWindowRect(hWnd, &wndRect);
		GetClientRect(hWnd, &clnRect);

		yht = wndRect.top;
		yhb = clnRect.top;
		h = wndRect.bottom - wndRect.top;
		h_ = clnRect.bottom - clnRect.top;
		if ((yPos > yht) && (yPos <= (yht+(h-h_))))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			win_ret = 0;
		}
		else return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_NCMOUSELEAVE:
		
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		
		break;
	case WM_MOVE:
		GetWindowRect(hWnd, &lpWinRect);
		break;
	case WM_ERASEBKGND:
        break;
	case WM_LBUTTONDOWN:
		//przekazanie parametrow
		/*
		GetWindowRect(hWnd, &wndRect);
        GetClientRect(hWnd, &clnRect);
        w  = wndRect.right      - wndRect.left; 
        w_ = clnRect.right      - clnRect.left;
        h  = wndRect.bottom     - wndRect.top; 
        h_ = clnRect.bottom     - clnRect.top;

		x0_new=wndRect.left;
		y0_new=wndRect.top;
        dx_new=w_; //w;
        dy_new=h_; //h;

		//PostQuitMessage(0);
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		win_ret=1;
		return 0;
		*/
		break;
	case WM_RBUTTONDOWN:
		

		win_ret = 0;

		PostMessage(hWnd, WM_CLOSE, 0, 0);
		
		break;
	case WM_COMMAND: //Command from Child windows and menus are under this message
		switch (wParam) //the ID is wParam
		{
			case ID_BUTTON_ESC:
			{

				win_ret = 0;

				PostMessage(hWnd, WM_CLOSE, 0, 0);

				break;
			}
			case ID_BUTTON_OK:
			{
				GetWindowRect(hWnd, &wndRect);
				GetClientRect(hWnd, &clnRect);
				w = wndRect.right - wndRect.left;
				w_ = clnRect.right - clnRect.left;
				h = wndRect.bottom - wndRect.top;
				h_ = clnRect.bottom - clnRect.top;

				x0_new = wndRect.left;
				y0_new = wndRect.top;
				dx_new = w_; //w;
				dy_new = h_; //h;


				win_ret = 1;

				PostMessage(hWnd, WM_CLOSE, 0, 0);

				break;
			}
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

   return win_ret;
}
#endif

#undef __WINSIZE__