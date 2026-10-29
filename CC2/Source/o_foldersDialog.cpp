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

#define __O_FOLDERSDIALOG__
// FoldersDialog.cpp: implementation of the CFoldersDialog class.
//
#ifndef LINUX
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include <dwmapi.h>
#include <uxtheme.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

#include <strsafe.h>
#include <WinUser.h>

#include <cstdint>
#include<windows.h>
#include<windowsx.h>
#include<commctrl.h>
#include<shlobj.h>
#include<winsock.h>
#include "o_foldersDialog.h"
//#include "ttf.h"

#include "menu.h"

#include "leak_detector_cpp.hpp"

HWND hdialog;
TCHAR c[MAX_PATH];
HFONT hFont=NULL;
HFONT hFont1 = NULL;

#define ID_BUTTON_ROOT 1001

WNDPROC CBProc;

static HWND hEditBox;
TCHAR szDira[255];

int f_height = 26;
int f_width = 16;
//CString font_name;
static char font_family_name[255];


int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

#define FILENAMEMAX 64 //32
//char **strarray = NULL;
static int strcount = 0;
char name[FILENAMEMAX];

extern char* get_font_family_name(void);

//extern char font_family_name[255];

#ifdef __cplusplus
extern "C" {
#endif

	extern int HEIGHT, WIDTH;
	//void find_font_face(char *Font_File);
	extern void sw_callback_locked_proc(void);
	unsigned int Ntohl(int chunklen);

	int myCharUpperW(int zn);

	void DoneBuffers8(void);

#ifdef __cplusplus
}
#endif

int myCharUpperW(int zn)
{
	// Cast through intptr_t to bridge the 32-bit/64-bit pointer widening and narrowing gap safely
	if (zn > 0) return (int)(intptr_t)CharUpperW((LPWSTR)(intptr_t)zn);
	return zn;
}


unsigned int Ntohl(int chunklen)
{
	return  ntohl(chunklen);
}

void DisplayErrorBox(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and clean up

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		//TEXT("%s failed with error %d: %s"),
		//lpszFunction, dw, lpMsgBuf);
		TEXT("%s"),
		lpszFunction);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


TCHAR szDir[MAX_PATH];
extern "C" {
	int My_GetOpenFolder(char *f_name, char *sz__current_path_file, char *sz__default_path_file, char *sz__current_mask, char *dlg_name); // , int font_height, int font_width, char *font_name);
	int My_GetFiles(char *szDir, int *n_list, char *pattern, char *pattern1, char* pattern2, char* pattern3, BOOL blocks);
	void DoneBuffers8(void);

	char **strarray = NULL;

}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFoldersDialog::CFoldersDialog()
{

}

CFoldersDialog::~CFoldersDialog()
{

}

LPITEMIDLIST ConvertPathToLpItemIdList(const char *pszPath)
{
	LPITEMIDLIST  pidl = NULL;
	LPSHELLFOLDER pDesktopFolder = NULL;
	OLECHAR       olePath[MAX_PATH];
	ULONG         chEaten;
	HRESULT       hr;

	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszPath, -1, olePath, MAX_PATH);
		hr = pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pidl, NULL);
		pDesktopFolder->Release();
		sw_callback_locked_proc();
		return pidl;
	}
	sw_callback_locked_proc();
	return NULL;
}

#define BROWSE_WIDTH      400
#define BROWSE_HEIGHT     540 

HWND g_hMyEditBox;

void ShowCustomBrowseForFolderDlg()
{
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = _T("Pick a Directory");
	bi.lpfn = BrowseCallbackProc;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
}

// Subclass procedure 
LRESULT APIENTRY ButtonSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_COMMAND)
	{
		switch (wParam)
		{
		case ID_BUTTON_ROOT:
		{
			break;
		}
		default:
			return DefWindowProcW(hwnd, uMsg, wParam, lParam);

		}
	}

	return CallWindowProc(CBProc, hwnd, uMsg,
		wParam, lParam);
}


// 1. Maintain the global theme brushes cleanly at the top of the file
static HBRUSH hDarkBgBrush = CreateSolidBrush(RGB(45, 45, 45));    // Main Gray Background Canvas
static HBRUSH hDarkInputBrush = CreateSolidBrush(RGB(30, 30, 30)); // Inner Edit text box fill
static HWND hBackPanel = NULL;                                    // Custom dark backdrop container

// Child control style scanner
BOOL CALLBACK EnumDarkModeChildProc(HWND hwndChild, LPARAM lParam)
{
    char className[256];
    GetClassNameA(hwndChild, className, sizeof(className));

    if (strcmp(className, "SysTreeView32") == 0)
    {
        ::SetWindowTheme(hwndChild, L"DarkMode_Explorer", NULL);
        TreeView_SetBkColor(hwndChild, RGB(30, 30, 30));
        TreeView_SetTextColor(hwndChild, RGB(240, 240, 240));
    }
    else if (strcmp(className, "Edit") == 0)
    {
        ::SetWindowTheme(hwndChild, L"CFD", NULL);
    }
    else if (strcmp(className, "Button") == 0)
    {
        ::SetWindowTheme(hwndChild, L"Explorer", NULL);
    }

    InvalidateRect(hwndChild, NULL, TRUE);
    return TRUE;
}

// 2. The bulletproof dark theme dialog callback procedure
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    // Intercept painting loops for standard static elements and the edit text field background
    if (uMsg == WM_CTLCOLORSTATIC || uMsg == WM_CTLCOLORDLG)
    {
        HDC hdc = (HDC)lParam;
        SetTextColor(hdc, RGB(240, 240, 240));      // White text prompt labels
        SetBkColor(hdc, RGB(45, 45, 45));          // Gray text box backing
        SetWindowLongPtr(hwnd, DWLP_MSGRESULT, (LONG_PTR)hDarkBgBrush);
        return TRUE;
    }
    else if (uMsg == WM_CTLCOLOREDIT)
    {
        HDC hdc = (HDC)lParam;
        SetTextColor(hdc, RGB(255, 255, 255));      // White typing characters
        SetBkColor(hdc, RGB(30, 30, 30));          // Dark input box fill style
        SetWindowLongPtr(hwnd, DWLP_MSGRESULT, (LONG_PTR)hDarkInputBrush);
        return TRUE;
    }

    // Initialization routine
    else if (BFFM_INITIALIZED == uMsg)
    {
        RECT DesktopRect;
        HWND hDesktop = ::GetDesktopWindow();
        HWND p_hWnd = hwnd;

        ::GetWindowRect(hDesktop, &DesktopRect);
        ::MoveWindow(p_hWnd, (DesktopRect.right / 2) - (BROWSE_WIDTH / 2), (DesktopRect.bottom / 2) - (BROWSE_HEIGHT / 2), BROWSE_WIDTH, BROWSE_HEIGHT, TRUE);

        HWND hListView = FindWindowEx(p_hWnd, NULL, "SysTreeView32", NULL);
        HWND hOKButton = FindWindowEx(p_hWnd, NULL, "Button", "OK");
        HWND hCancelButton = FindWindowEx(p_hWnd, NULL, "Button", "Cancel");
        hEditBox = FindWindowEx(p_hWnd, NULL, "Edit", NULL);
        HWND hStatic = FindWindowEx(p_hWnd, NULL, "Static", NULL);

        SetWindowTextW(hCancelButton, L"Esc");
        SetWindowTextW(hStatic, _SELECT_FOLDER_);

        // ---------------------------------------------------------------------
        // FIX: INJECT A STATIC BACK PANEL TO COVER STUBBORN WHITE CANVAS AREAS
        // ---------------------------------------------------------------------
        // This places a dark gray window background across the entire dialog window canvas size
        hBackPanel = CreateWindowExA(0, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
                                    0, 0, BROWSE_WIDTH, BROWSE_HEIGHT, p_hWnd, NULL, NULL, NULL);

        // Z-Order Adjustment: Push the back panel to the bottom so it sits cleanly behind your controls
        ::SetWindowPos(hBackPanel, HWND_BOTTOM, 0, 0, BROWSE_WIDTH, BROWSE_HEIGHT, SWP_NOMOVE | SWP_NOSIZE);
        // ---------------------------------------------------------------------

        // Apply dark title window framing configuration
        BOOL bDark = TRUE;
        ::DwmSetWindowAttribute(hwnd, 20, &bDark, sizeof(bDark));

        // Scan child objects to attach modern library assets
        ::EnumChildWindows(hwnd, EnumDarkModeChildProc, 0);

        // Core positioning adjustments
        strcpy(font_family_name, get_font_family_name());
        hFont = CreateFont(f_height, f_width, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, font_family_name);
        hFont1 = CreateFont((int)((float)f_height * 0.8f), (int)((float)f_width * 0.8f), 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, font_family_name);

        ::SetWindowPos(hStatic,       0, 10,  10, 360, 32, SWP_NOZORDER);
        ::SetWindowPos(hEditBox,      0, 10,  50, 360, 32, SWP_NOZORDER);
        ::SetWindowPos(hListView,     0, 10,  90, 360, 350, SWP_NOZORDER);
        ::SetWindowPos(hOKButton,     0, 180, 450, 80, 32, SWP_NOZORDER);
        ::SetWindowPos(hCancelButton, 0, 290, 450, 80, 32, SWP_NOZORDER);

        // Map custom fonts
        SendMessageW(hwnd,          WM_SETFONT, WPARAM(hFont),  TRUE);
        SendMessageW(hListView,     WM_SETFONT, WPARAM(hFont),  TRUE);
        SendMessageW(hOKButton,     WM_SETFONT, WPARAM(hFont),  TRUE);
        SendMessageW(hCancelButton, WM_SETFONT, WPARAM(hFont),  TRUE);
        SendMessageW(hEditBox,      WM_SETFONT, WPARAM(hFont),  TRUE);
        SendMessageW(hStatic,       WM_SETFONT, WPARAM(hFont1), TRUE);

        // Recompute bounds safely
        ::SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        ::InvalidateRect(hEditBox, NULL, TRUE);
        ::InvalidateRect(hwnd, NULL, TRUE);
        ::UpdateWindow(hEditBox);
        ::UpdateWindow(hwnd);
    }
    // Handle background coloring for our newly added static panel container
    else if (uMsg == WM_DRAWITEM)
    {
        LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
        if (lpDrawItem->hwndItem == hBackPanel)
        {
            FillRect(lpDrawItem->hDC, &lpDrawItem->rcItem, hDarkBgBrush);
            return TRUE;
        }
    }
    else if (BFFM_SELCHANGED == uMsg)
    {
        TCHAR szDirLocal[MAX_PATH * 2] = { 0 };
        BOOL bRet = SHGetPathFromIDList((LPITEMIDLIST)lParam, szDirLocal);
        if (bRet)
        {
            char f_name[MAX_PATH * 2];
            strncpy(f_name, szDirLocal, sizeof(f_name) - 1);
            f_name[sizeof(f_name) - 1] = '\0';
        }
    }
    else if (BFFM_VALIDATEFAILED == uMsg)
    {
        GetWindowTextA(hEditBox, szDir, 255);
        strncpy(szDira, szDir, sizeof(szDira) - 1);
        szDira[sizeof(szDira) - 1] = '\0';

        if (strcmp(szDir, "*") == 0) return 2;
        return 0;
    }
    else if (WM_COMMAND == uMsg)
    {
        if (lParam == ID_BUTTON_ROOT) strcpy(szDir, "\\");
    }
    else if (WM_CLOSE == uMsg)
    {
        sw_callback_locked_proc();
        return DefWindowProcW(hwnd, uMsg, lParam, lpData);
    }

    return 0;
}


int CALLBACK BrowseCallbackProc1(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData) 
{
	switch(uMsg) 
	{

		case BFFM_SELCHANGED: 
		{
		   // Set the status window to the currently selected path.
		   if (SHGetPathFromIDList((LPITEMIDLIST) lp, szDir)) 
			  SendMessage(hwnd,BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
		   break;
		}
        case BFFM_INITIALIZED:
        {
            SendMessage(hwnd, BFFM_SETSELECTION, 1,(LPARAM) szDir);

			//Create a push button
			HWND hMyRaioBtn = CreateWindowEx(0, "BUTTON", "RadioButton", SS_NOTIFY | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 400, 200, 20, hwnd, 0, NULL, NULL);
        }
		
        break;
		
		default:
		   break;
	}
	return 0;
}

//Callback of the main window
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_QUIT || message == WM_CLOSE)
		PostQuitMessage(0);


	if (message == WM_INITDIALOG)
	{
		hdialog = hDlg;

		RECT rc;

		GetWindowRect(hDlg, &rc);
		int w = rc.right - rc.left, h = rc.bottom - rc.top;
		int cx = GetSystemMetrics(SM_CXSCREEN) / 2, cy = GetSystemMetrics(SM_CYSCREEN) / 2;
		MoveWindow(hDlg, cx - w / 2, cy - h / 2, w, h, FALSE);

	}

	if (message == WM_COMMAND)
	{
		if ((LOWORD(wParam)) == IDOK)
		{
			TCHAR dname[MAX_PATH];
			IMalloc *imalloc; SHGetMalloc(&imalloc);
			BROWSEINFO bi; ZeroMemory(&bi, sizeof(bi));
			bi.hwndOwner = hDlg;
			bi.pszDisplayName = dname;
			bi.lpszTitle = TEXT("Click the check box to HIDE/SHOW the edit contol.....");

////#define BIF_NONEWFOLDERBUTTON  0x0200

			bi.ulFlags = BIF_NONEWFOLDERBUTTON | BIF_RETURNONLYFSDIRS;
			bi.lpfn = BrowseCallbackProc;
			//ITEMIDLIST *pidl = SHBrowseForFolder(&bi);
			//instead changed on 12-06-2026
			LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

			//Displays the selected folder
			if (pidl != nullptr)
				MessageBox(hDlg, c, "Show the edit control", 0);

			imalloc->Free(pidl);
			imalloc->Release();

		}

		if ((LOWORD(wParam)) == IDCANCEL)
		{
			PostQuitMessage(0);
			EndDialog(hDlg, 0);
		}

	}

	return 1;
}


void DoneBuffers8(void)
{
	int i;
	//deleteing previous strarray
	for (i = 0; i < strcount; i++)
		free(strarray[i]);
	    free(strarray);
}

int VeryMy_GetOpenFolder_32(HWND hwnd, char *f_name, char *sz__current_path_file, char *sz__default_path_file, char *sz__current_mask, char *dlg_name)
{
    BROWSEINFO bi;
    TCHAR szDir[MAX_PATH];
    LPITEMIDLIST pidl = NULL;
    LPMALLOC pMalloc = NULL;
    int nRet = 0, nGo = 0;
    char *ptr;
    BOOL rr;

    f_height = (int)(((float)HEIGHT * 1.05) + 0.5);
    f_width = (int)(((float)WIDTH * 0.73) + 0.5);

    // Initialize tracking string buffers cleanly
    CString RootPath = sz__current_path_file;
    CString DefaultRootPath = sz__default_path_file;
    szDir[0] = 0;

    // Allocate the dynamic path string tracking buffers EXACTLY ONCE outside the loop
    char *pszRootPath = new char[RootPath.GetLength() + MAX_PATH + 1];
    char *pszDefaultRootPath = new char[DefaultRootPath.GetLength() + MAX_PATH + 1];
    strcpy(pszRootPath, RootPath);
    strcpy(pszDefaultRootPath, DefaultRootPath);

    if (SUCCEEDED(SHGetMalloc(&pMalloc)))
    {
        ZeroMemory(&bi, sizeof(bi));
        bi.hwndOwner = hwnd;
        bi.pszDisplayName = 0;
        bi.lpszTitle = dlg_name;
        bi.pidlRoot = ConvertPathToLpItemIdList(pszRootPath);
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_EDITBOX | BIF_VALIDATE;
        bi.lpfn = BrowseCallbackProc;

        while (!nGo)
        {
            pidl = SHBrowseForFolder(&bi);

            // If dialog closed but custom navigation token was intercepted via BrowseCallbackProc
            if ((!pidl) && (szDira[0] != 0) && ((strcmp(szDira, "..") == 0) || (strcmp(szDira, "/") == 0) || (strcmp(szDira, "//") == 0)))
            {
                if (strcmp(szDira, "..") == 0)
                {
                    size_t len = strlen(pszRootPath);
                    if (len > 0 && pszRootPath[len - 1] == '\\') {
                        pszRootPath[len - 1] = '\0';
                    }
                    ptr = strrchr(pszRootPath, '\\');
                    if (ptr != NULL) {
                        *(ptr + 1) = '\0'; // Drop down one directory tier cleanly
                    }

                    // Safe cleanup of the old PIDL pointer to prevent memory leaks before assigning new root
                    if (bi.pidlRoot) { pMalloc->Free((void*)bi.pidlRoot); }
                    bi.pidlRoot = ConvertPathToLpItemIdList(pszRootPath);
                }
                else if (strcmp(szDira, "//") == 0)
                {
                    if (bi.pidlRoot) { pMalloc->Free((void*)bi.pidlRoot); }
                    bi.pidlRoot = SHSimpleIDListFromPath(L"\\");
                }
                else // Single "/" command input fallback
                {
                    if (bi.pidlRoot) { pMalloc->Free((void*)bi.pidlRoot); }
                    bi.pidlRoot = ConvertPathToLpItemIdList(pszDefaultRootPath);
                }

                // Reset strings cleanly to prevent infinite evaluation loops
                szDira[0] = 0;
                szDir[0] = 0;
                nGo = 0; // Keep the loop alive to restart the shell dialog seamlessly
            }
            else if (pidl)
            {
                SHGetPathFromIDList(pidl, szDir);
                ptr = strchr(szDir, ':');

                // If it is a bare drive selection like C:\, redirect safely
                if ((ptr != NULL) && strlen(szDir) < 4)
                {
                    if (bi.pidlRoot) { pMalloc->Free((void*)bi.pidlRoot); }
                    bi.pidlRoot = SHSimpleIDListFromPath(L"\\");
                    szDir[0] = 0;
                    szDira[0] = 0;
                    nGo = 0;
                }
                else
                {
                    if (strlen(szDir) > 0) {
                        strncpy(f_name, szDir, MAX_PATH - 1);
                        f_name[MAX_PATH - 1] = '\0';
                    }
                    nRet = 1;
                    nGo = 1;
                }

                // FIX: Only call Free if the item ID list pointer is valid!
                pMalloc->Free(pidl);
                pidl = NULL;
            }
            else
            {
                // User pressed Cancel explicitly
                nRet = 0;
                nGo = 1;
            }
        }

        // Final cleanup of the root PIDL context built inside the loop iterations
        if (bi.pidlRoot) {
            pMalloc->Free((void*)bi.pidlRoot);
        }
        pMalloc->Release();
    }

    // Clean up heap buffers safely
    delete[] pszRootPath;
    delete[] pszDefaultRootPath;

    // Safely flush residual font assignments out of memory
    if (hFont)  { DeleteObject(hFont);  hFont = NULL; }
    if (hFont1) { DeleteObject(hFont1); hFont1 = NULL; }

    sw_callback_locked_proc();
    return nRet;
}

#include <shobjidl.h> // Required header for modern IFileDialog interfaces

int VeryMy_GetOpenFolder_64(HWND hwnd, char *f_name, char *sz__current_path_file, char *sz__default_path_file, char *sz__current_mask, char *dlg_name)
{
    int nRet = 0;
    IFileOpenDialog *pFolderDlg = NULL;

    // 1. Initialize the COM Library interface component cleanly
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    // 2. Instantiate the modern Open Dialog element framework
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFolderDlg));

    if (SUCCEEDED(hr))
    {
        // -------------------------------------------------------------
        // CORE CHANGE: FORCE THE DIALOG TO ONLY CHOOSE DIRECTORIES (PICK FOLDERS)
        // -------------------------------------------------------------
        DWORD dwOptions;
        if (SUCCEEDED(pFolderDlg->GetOptions(&dwOptions)))
        {
            // FOS_PICKFOLDERS changes it from a standard file selector to a folder browser
            pFolderDlg->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
        }

        // Set the dialog title custom string cleanly
        if (dlg_name && strlen(dlg_name) > 0)
        {
            // Convert title string to Wide characters for modern Win32 API compatibility
            wchar_t wTitle[260];
            MultiByteToWideChar(CP_ACP, 0, dlg_name, -1, wTitle, 260);
            pFolderDlg->SetTitle(wTitle);
        }

        // Set the starting initial default directory path context if provided
        if (sz__current_path_file && strlen(sz__current_path_file) > 0)
        {
            wchar_t wPath[MAX_PATH];
            MultiByteToWideChar(CP_ACP, 0, sz__current_path_file, -1, wPath, MAX_PATH);

            IShellItem *pDefaultFolderItem = NULL;
            if (SUCCEEDED(SHCreateItemFromParsingName(wPath, NULL, IID_PPV_ARGS(&pDefaultFolderItem))))
            {
                pFolderDlg->SetFolder(pDefaultFolderItem);
                pDefaultFolderItem->Release();
            }
        }

        // 3. Display the dialog modally attached to your window handle context
        hr = pFolderDlg->Show(hwnd);

        // 4. Capture the user's selected choice output path string
        if (SUCCEEDED(hr))
        {
            IShellItem *pResultItem = NULL;
            if (SUCCEEDED(pFolderDlg->GetResult(&pResultItem)))
            {
                wchar_t *wSelectedPath = NULL;
                // Request the standard system parsing folder path string
                if (SUCCEEDED(pResultItem->GetDisplayName(SIGDN_FILESYSPATH, &wSelectedPath)))
                {
                    // Convert back to your standard MultiByte 8-bit string layout (f_name)
                    WideCharToMultiByte(CP_ACP, 0, wSelectedPath, -1, f_name, MAX_PATH, NULL, NULL);
                    CoTaskMemFree(wSelectedPath); // Clean up system string pointer allocations
                    nRet = 1; // Success!
                }
                pResultItem->Release();
            }
        }
        pFolderDlg->Release();
    }

    CoUninitialize(); // Safely uninitialize the COM environment before exiting
    return nRet; // Returns 1 if selected, 0 if canceled
}


int VeryMy_GetOpenFolder(HWND hwnd, char *f_name, char *sz__current_path_file, char *sz__default_path_file, char *sz__current_mask, char *dlg_name)
{
	// 1. Read the Windows version bitmask metrics at runtime
	DWORD dwVersion = GetVersion();
	DWORD dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));

	// 2. Conditional check: Is the system older than Windows Vista? (Major version 5 = XP/2000)
	if (dwMajorVersion < 6)
	{
		// --- LEGACY FALLBACK TRACK ---
		// Route straight to your classic SHBrowseForFolder implementation loop.
		// It provides 100% stability on legacy 32-bit computing platforms.
		return VeryMy_GetOpenFolder_32(hwnd, f_name, sz__current_path_file, sz__default_path_file, sz__current_mask, dlg_name);
	}
	else
	{
		// --- MODERN DESKTOP TRACK ---
		// Route directly to your modern COM-based IFileDialog interface.
		// It handles expandable paths cleanly and supports native dark mode.
		return VeryMy_GetOpenFolder_64(hwnd, f_name, sz__current_path_file, sz__default_path_file, sz__current_mask, dlg_name);
	}
}



int My_GetFiles(char *szDir0, int *n_list, char *pattern, char *pattern1, char* pattern2, char* pattern3, BOOL blocks)
{
	int i;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	HANDLE hFind1 = INVALID_HANDLE_VALUE;
	HANDLE hFind2 = INVALID_HANDLE_VALUE;
	HANDLE hFind3 = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd, ffd1, ffd2, ffd3;
	LARGE_INTEGER filesize;
	DWORD dwError = 0;
	DWORD dwError1 = 0;
	DWORD dwError2 = 0;
	DWORD dwError3 = 0;
	char szDir[255], szDir1[255], szDir2[255], szDir3[255];
	int FILENAMEMAXC;

	BOOL NO_DIRS = FALSE;
	char _dir_[7] = "<DIR>";


	strcpy(szDir, szDir0);
	strcpy(szDir1, szDir0);
	strcpy(szDir2, szDir0);
	strcpy(szDir3, szDir0);
StringCchCat(szDir, MAX_PATH, TEXT(pattern));  //   "\\*.alx"  "\\*."
if (pattern1 != NULL) StringCchCat(szDir1, MAX_PATH, TEXT(pattern1));  //   "\\*.dxf"  "\\*.pcx"
if (pattern2 != NULL) StringCchCat(szDir2, MAX_PATH, TEXT(pattern2));  //   "\\*.png"
if (pattern3 != NULL) StringCchCat(szDir3, MAX_PATH, TEXT(pattern3));  //   "\\*.jpg"

// Find the first file in the directory.

//deleteing previous strarray
for (i = 0; i < strcount; i++)
	free(strarray[i]);

strcount = 0;


hFind = FindFirstFile(szDir, &ffd);

if (INVALID_HANDLE_VALUE == hFind)
{
	if ((pattern1 == NULL) && (pattern2 == NULL) && (pattern3 == NULL) && (blocks!=-1))
	{
		//DisplayErrorBox(TEXT("FindFirstFile"));
		if (blocks)
		{
			DisplayErrorBox(TEXT(error_message_block));
		}
		else
		{
			DisplayErrorBox(TEXT(error_message_background));
		}

		sw_callback_locked_proc();
		return 0; // dwError;
	}
}
else
{
	// List all the files in the directory with some info about them.

	do
	{
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (NO_DIRS))
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;

			FILENAMEMAXC = sizeof(ffd.cFileName)+6;

			strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
			strarray[strcount] = (char *)malloc(FILENAMEMAXC * sizeof(char));
			strcpy(strarray[strcount], ffd.cFileName);
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				strcat((char *)strarray[strcount], _dir_);
			strcount++;
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();

	FindClose(hFind);
}
  
if (pattern1 != NULL)
{
	hFind1 = FindFirstFile(szDir1, &ffd1);

	if ((INVALID_HANDLE_VALUE == hFind) && (INVALID_HANDLE_VALUE == hFind1) && (blocks != -1))
	{
		//DisplayErrorBox(TEXT("FindFirstFile"));
		if ((pattern2 == NULL) && (pattern3 == NULL))
		{
			if (blocks)
			{
				DisplayErrorBox(TEXT(error_message_block));
			}
			else
			{
				DisplayErrorBox(TEXT(error_message_background));
			}

			sw_callback_locked_proc();
			return 0; // dwError;
		}
	}

	if (INVALID_HANDLE_VALUE != hFind1)
	{
		do
		{
			if ((ffd1.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (NO_DIRS))
			{
				;
			}
			else
			{
				filesize.LowPart = ffd1.nFileSizeLow;
				filesize.HighPart = ffd1.nFileSizeHigh;

				FILENAMEMAXC = sizeof(ffd1.cFileName) + 6;

				strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
				strarray[strcount] = (char *)malloc(FILENAMEMAXC * sizeof(char));
				strcpy(strarray[strcount], ffd1.cFileName);
				if (ffd1.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
					strcat((char*)strarray[strcount], _dir_);
				strcount++;
			}
		} while (FindNextFile(hFind1, &ffd1) != 0);

		dwError1 = GetLastError();

		FindClose(hFind1);
	}
}


if (pattern2 != NULL)
{
	hFind2 = FindFirstFile(szDir2, &ffd2);

	if ((INVALID_HANDLE_VALUE == hFind) && (INVALID_HANDLE_VALUE == hFind1) && (INVALID_HANDLE_VALUE == hFind2) && (blocks != -1))
	{
		if (pattern3 == NULL)
		{
			if (blocks)
			{
				DisplayErrorBox(TEXT(error_message_block));
			}
			else
			{
				DisplayErrorBox(TEXT(error_message_background));
			}

			sw_callback_locked_proc();
			return 0; // dwError;
		}
	}

	if (INVALID_HANDLE_VALUE != hFind2)
	{
		do
		{
			if ((ffd2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (NO_DIRS))
			{
				;
			}
			else
			{
				filesize.LowPart = ffd2.nFileSizeLow;
				filesize.HighPart = ffd2.nFileSizeHigh;
				

				FILENAMEMAXC = sizeof(ffd2.cFileName) + 6;

				strarray = (char**)realloc(strarray, (strcount + 1) * sizeof(char*));
				strarray[strcount] = (char*)malloc(FILENAMEMAXC * sizeof(char));
				strcpy(strarray[strcount], ffd2.cFileName);
				if (ffd2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					strcat((char*)strarray[strcount], _dir_);
				strcount++;
			}
		} while (FindNextFile(hFind2, &ffd2) != 0);

		dwError2 = GetLastError();

		FindClose(hFind2);
	}
}

if (pattern3 != NULL)
{
	hFind3 = FindFirstFile(szDir3, &ffd3);

	if ((INVALID_HANDLE_VALUE == hFind) && (INVALID_HANDLE_VALUE == hFind1) && (INVALID_HANDLE_VALUE == hFind2) && (INVALID_HANDLE_VALUE == hFind3) && (blocks != -1))
	{
		
		if (blocks)
		{
			DisplayErrorBox(TEXT(error_message_block));
		}
		else
		{
			DisplayErrorBox(TEXT(error_message_background));
		}

		sw_callback_locked_proc();
		return 0; // dwError;
	}

	if (INVALID_HANDLE_VALUE != hFind3)
	{
		do
		{
			if ((ffd3.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (NO_DIRS))
			{
				;
			}
			else
			{
				filesize.LowPart = ffd3.nFileSizeLow;
				filesize.HighPart = ffd3.nFileSizeHigh;
				

				FILENAMEMAXC = sizeof(ffd3.cFileName) + 6;

				strarray = (char**)realloc(strarray, (strcount + 1) * sizeof(char*));
				strarray[strcount] = (char*)malloc(FILENAMEMAXC * sizeof(char));
				strcpy(strarray[strcount], ffd3.cFileName);
				if (ffd3.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					strcat((char*)strarray[strcount], _dir_);
				strcount++;
			}
		} while (FindNextFile(hFind3, &ffd3) != 0);

		dwError3 = GetLastError();

		FindClose(hFind3);
	}
}

if ((dwError != ERROR_NO_MORE_FILES) && (dwError1 != ERROR_NO_MORE_FILES) && (dwError2 != ERROR_NO_MORE_FILES) && (dwError3 != ERROR_NO_MORE_FILES) && (blocks != -1))
{
	//DisplayErrorBox(TEXT("FindFirstFile"));
	if (blocks)
	{
		DisplayErrorBox(TEXT(error_message_block));
	}
	else
	{
		DisplayErrorBox(TEXT(error_message_background));
	}
	
	sw_callback_locked_proc();
	return 0;
}


*n_list = strcount;

sw_callback_locked_proc();
return 1;
}

#else

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <wctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

////#include "leak_detector_cpp.hpp"

typedef bool BOOL;

#ifndef NULL
#define NULL		((void *)(0))
#endif

//extern void utf8Lower(unsigned char* text);

extern "C" {
int My_GetFiles(char *szDir, int *n_list, char *pattern, char *pattern1, char* pattern2, char* pattern3, BOOL blocks);
void DoneBuffers8(void);
extern void utf8Lower(unsigned char* text);
extern void sw_callback_locked_proc(void);
}

char **strarray = (char **)malloc(1 * sizeof(char *));  //just tpo initialize

static int strcount = 0;

static int myCompare(const void* a, const void* b)
{

    // setting up rules for comparison
    return strcmp(*(const char**)a, *(const char**)b);
}

// Function to sort the array
void sort(/*const*/ char* arr[], int n)
{
    // calling qsort function to sort the array
    // with the help of Comparator
    qsort(arr, n, sizeof(const char*), myCompare);
}


int My_GetFiles(char *szDir, int *n_list, char *pattern, char *pattern1, char* pattern2, char* pattern3, BOOL blocks)
{
    DIR *d;
    struct dirent *dir;
    int i;
    int FILENAMEMAXC;
    char *pattern_[4];
    char d_name[256];

    char _dir_[7] = "<DIR>";

    pattern_[0]=(pattern==NULL) ? 0 :  pattern+2;
    pattern_[1]=(pattern1==NULL) ? 0 : pattern1+2;
    pattern_[2]=(pattern2==NULL) ? 0 : pattern2+2;
    pattern_[3]=(pattern3==NULL) ? 0 : pattern3+2;

    //deleteing previous strarray
    for (i = 0; i < strcount; i++)
        free((char**)strarray[i]);

    strcount = 0;

    d = opendir(szDir);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            //check conditions / patterns
            //if(dir->d_type==DT_DIR)  //we take all subfolders
            if(dir->d_type==DT_REG)
            {
             //checking pattern
             strcpy(d_name, dir->d_name);
             utf8Lower((unsigned char*)d_name);
             for (i=0; i<4; i++)
             {
                 if ((pattern_[i]!=NULL) && (strcmp(pattern_[i],".")!=0) && (strstr(d_name,pattern_[i])!=NULL))
                 {
                     FILENAMEMAXC = sizeof(dir->d_name) + 6;
                     //printf("%s\n", dir->d_name);
                     strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
                     strarray[strcount] = (char *)malloc(FILENAMEMAXC * sizeof(char));
                     strcpy(strarray[strcount], dir->d_name);
                     strcount++;
                     break;
                 }
             }
            }
            else if (dir->d_type==DT_DIR)
                {
                    FILENAMEMAXC = sizeof(dir->d_name) + 6;
                    //printf("%s\n", dir->d_name);
                    strarray = (char **) realloc(strarray, (strcount + 1) * sizeof(char *));
                    strarray[strcount] = (char *) malloc(FILENAMEMAXC * sizeof(char));
                    strcpy(strarray[strcount], dir->d_name);
                    strcat((char *) strarray[strcount], _dir_);
                    strcount++;
                }
        }
        closedir(d);
    }

    //sorting
    sort(strarray, strcount);


    //if error return 0;

    *n_list = strcount;

    sw_callback_locked_proc();
    return 1;
}

void DoneBuffers8(void)
{
    int i;
    //deleteing previous strarray
    for (i = 0; i < strcount; i++)
        free(strarray[i]);
    free(strarray);
}

#ifdef __cplusplus
extern "C" {
#endif

int myCharUpperW(int zn);

#ifdef __cplusplus
}
#endif


int myCharUpperW(int zn)
{
#ifndef LINUX
    return (int)CharUpperW((LPWSTR)zn);
#else
    return towupper((wint_t)zn);
#endif
}

unsigned int Ntohl(int chunklen)
{
    return  ntohl(chunklen);
}
#endif

#undef __O_FOLDERSDIALOG__

