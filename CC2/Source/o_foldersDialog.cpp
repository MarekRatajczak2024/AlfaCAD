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
#include "stdafx.h"
#include <strsafe.h>
#include <WinUser.h>


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
	if (zn > 0) return (int)CharUpperW((LPWSTR)zn);
	else return zn;
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



int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	//Initialization callback message
	if (BFFM_INITIALIZED == uMsg)
	{
		RECT rtDialog, rtButton;
		RECT DesktopRect;

		// Get the desktop window
		HWND hDesktop = ::GetDesktopWindow();

		HWND p_hWnd = hwnd;
		
		// Get the Desktop rect
		::GetWindowRect(hDesktop, &DesktopRect);

		// Move the window to center
		::MoveWindow(p_hWnd, (DesktopRect.right / 2) - (BROWSE_WIDTH / 2), (DesktopRect.bottom / 2) - (BROWSE_HEIGHT / 2), BROWSE_WIDTH, BROWSE_HEIGHT, TRUE);

		//Find the listview
		HWND hListView = FindWindowEx(p_hWnd, NULL, "SysTreeView32", NULL);

		//Find the handles of OK and Cancel buttons
		HWND hOKButton = FindWindowEx(p_hWnd, NULL, "Button", "OK");

		HWND hCancelButton = FindWindowEx(p_hWnd, NULL, "Button", "Cancel");

		SetWindowTextW(hCancelButton, L"Esc");

		//Find the handles of OK and Cancel buttons
		//HWND 
		hEditBox = FindWindowEx(p_hWnd, NULL, "Edit", NULL);

		HWND hStatic = FindWindowEx(p_hWnd, NULL, "Static", NULL);


		SetWindowTextW(hStatic, _SELECT_FOLDER_);


		//normal

        strcpy(font_family_name, get_font_family_name());
		

		hFont= CreateFont(f_height, f_width, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, font_family_name);
		hFont1 = CreateFont((int)((float)f_height)*0.8, (int)((float)f_width)*0.8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, font_family_name);
		SendMessageW(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hListView, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hOKButton, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hCancelButton, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hEditBox, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(NULL, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hStatic, WM_SETFONT, WPARAM(hFont1), TRUE);

		//Gets the OK button position
		::GetClientRect(hOKButton, &rtButton);

		//Gets the dimensions BrowseForFolder dialog
		::GetClientRect(p_hWnd, &rtDialog);

		//Relocate the SysTreeView32 control
		::SetWindowPos(hListView, 0, 10, 90, 360, 350, 0);

		//Relocate the Static control
		::SetWindowPos(hStatic, 0, 10, 10, 360, 32, 0);

		//Relocate the OK button
		::SetWindowPos(hOKButton, 0, 180, 450, 80, 32, 0);

		//Relocate the Cancel buttton
		::SetWindowPos(hCancelButton, 0, 290, 450, 80, 32, 0);

		//Relocate the Edit box
		::SetWindowPos(hEditBox, 0, 10, 50, 360, 32, 0);

		//Create a static control
		//HWND hMyStaticCtrl = CreateWindowEx(0, "STATIC", "My Custom Controls", SS_NOTIFY | WS_CHILD | WS_VISIBLE, 20, 370, 200, 25, p_hWnd, 0, NULL, NULL);

		//Create a radio button

		SendMessageW(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hListView, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hOKButton, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hCancelButton, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hEditBox, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessageW(hStatic, WM_SETFONT, WPARAM(hFont1), TRUE);


		//Create an Edit box
		//g_hMyEditBox = CreateWindowEx(0, "EDIT", "", SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 430, 340, 20, p_hWnd, 0, NULL, NULL);
	}
	//Selection change message
	else if (BFFM_SELCHANGED == uMsg)
	{
		TCHAR szDir[MAX_PATH * 2] = { 0 };

		// fail if non-filesystem
		BOOL bRet = SHGetPathFromIDList((LPITEMIDLIST)lParam, szDir);

		if (bRet)
		{
			// set the folder path to edit box
			//FromHandle(g_hMyEditBox)->SetWindowText(szDir);
			char f_name[255];
			strcpy(f_name, szDir);
		}
	}
	else if (BFFM_VALIDATEFAILED == uMsg)
	{

		GetWindowText(hEditBox, szDir, 255);
		strcpy(szDira, szDir);
		if (strcmp(szDir, "*") == 0)
		{
			return 2;  //command to change folder
	    }
		return 0;
	}
	else if (WM_COMMAND == uMsg) //Command from Child windows and menus are under this message	
	{
		switch (lParam) //the ID is wParam
		{
		case ID_BUTTON_ROOT:
			{
	
			  strcpy(szDir, "\\");
	
			}
		}

	}
	else if (WM_CLOSE == uMsg)
	{
		sw_callback_locked_proc();
		return DefWindowProcW(hwnd, uMsg, lParam, lpData);
	}
	//DeleteObject(hFont);

	return 1;
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

#define BIF_NONEWFOLDERBUTTON  0x0200

			bi.ulFlags = BIF_NONEWFOLDERBUTTON | BIF_RETURNONLYFSDIRS;
			bi.lpfn = BrowseCallbackProc;
			ITEMIDLIST *pidl = SHBrowseForFolder(&bi);

			//Displays the selected folder
			if (pidl != NULL)
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

int VeryMy_GetOpenFolder(HWND hwnd, char *f_name, char *sz__current_path_file, char *sz__default_path_file, char *sz__current_mask, char *dlg_name) // , int font_height, int font_width, char *desktop_font_name)
{
	BROWSEINFO bi;
	TCHAR szDir[MAX_PATH];
	LPITEMIDLIST pidl;
	LPMALLOC pMalloc;
	int nRet = 0, nGo=0;

	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	char *ptr;
	BOOL rr;



	f_height = (int)(((float)HEIGHT*1.05)+0.5); 
	f_width = (int)(((float)WIDTH*0.73) + 0.5);


	CString RootPath = sz__current_path_file;
	CString DefaultRootPath = sz__default_path_file;

	szDir[0] = 0;

	if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		ZeroMemory(&bi, sizeof(bi));
		bi.hwndOwner = hwnd;
		bi.pszDisplayName = 0;
		bi.lpszTitle = dlg_name;
		char *pszRootPath = new char[RootPath.GetLength() + 1];
		char *pszDefaultRootPath = new char[DefaultRootPath.GetLength() + 1];
		strcpy(pszRootPath, RootPath);
		strcpy(pszDefaultRootPath, DefaultRootPath);
		bi.pidlRoot = ConvertPathToLpItemIdList(pszRootPath);
		
		
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_EDITBOX | BIF_VALIDATE;

		bi.lpfn = BrowseCallbackProc; 
		
		nRet = 0;
		nGo = 0;
		while (!nGo)
		{
			
			pidl = SHBrowseForFolder(&bi);
			
			if ((!pidl) && ((strcmp(szDira, "..") == 0) || (strcmp(szDira, "/") == 0) || (strcmp(szDira, "//") == 0)))
			{
				if (strcmp(szDira, "..") == 0)
				{
					
					if (pszRootPath[strlen(pszRootPath) - 1] == '\\') pszRootPath[strlen(pszRootPath) - 1] = '\0';
					ptr = strrchr(pszRootPath, '\\');
					if (ptr != NULL) *(ptr+1) = '\0';
					bi.pidlRoot = ConvertPathToLpItemIdList(pszRootPath);
					szDira[0] = 0;
					szDir[0] = 0;
					nGo = 0;
				}
				else if (strcmp(szDira, "//") == 0)
				{
					bi.pidlRoot = SHSimpleIDListFromPath(L"\\");
					szDira[0] = 0;
					szDir[0] = 0;
					nGo = 0;
				}
				else
				{
					bi.pidlRoot = ConvertPathToLpItemIdList(pszDefaultRootPath);
					szDira[0] = 0;
					szDir[0] = 0;
					nGo = 0;
				}

			}
			else if (pidl)	
			{
				SHGetPathFromIDList(pidl, szDir);
				ptr = strchr(szDir, ':');
				if ((ptr!=NULL) && strlen(szDir)<4)
				{
					bi.pidlRoot = SHSimpleIDListFromPath(L"\\"); 
					szDir[0] = 0;
					szDira[0] = 0;
					nGo = 0;
				}
				else
				{	

					if (strlen(szDir)>0) strcpy(f_name, szDir);
					nRet = 1;
					nGo = 1;
				}

			}
			else
			{
				nRet = 0;
				nGo = 1;
			}
          pMalloc->Free(pidl); 
		}

		pMalloc->Release();

    delete pszRootPath;
	delete pszDefaultRootPath;

	}


	if (hFont) rr = DeleteObject(hFont);
	if (hFont1) rr = DeleteObject(hFont1);

	sw_callback_locked_proc();
	return nRet;
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

