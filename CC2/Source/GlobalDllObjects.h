#include "StdAfx.h"
// GlobalDllObjects.h : main header file for the GLOBALDLLOBJECTS DLL
//
#if !defined(AFX_GLOBALDLLOBJECTS_H__25729FC2_08C5_11D2_8DB2_204C4F4F5020__INCLUDED_)
#define AFX_GLOBALDLLOBJECTS_H__25729FC2_08C5_11D2_8DB2_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef LINUX
#include "res/resource.h"
#endif

#define MAXPATH   260
#define FILENO 32

typedef struct Client_Bitmap
{
	unsigned char flag = 0;
	int x1, y1, x2, y2;
	char drawing_file[MAXPATH];
	char dump_file[MAXPATH];
} Client_Bitmap;

struct shmbuf {
	int client_number;
	int mflag;
	int flag[FILENO];
	char file_name[FILENO][MAXPATH];
};

struct shmseg {
	int cnt;
	int complete;
	//char buf[BUF_SIZE];
	struct shmbuf buf;
};

/////////////////////////////////////////////////////////////////////////////
// CGlobalDllObjectsApp
// See GlobalDllObjects.cpp for the implementation of this class
//

#ifdef GLOBALDLLOBJECTS_EXPORTS
#define GLOBALDLLOBJECTS_API __declspec(dllexport)
#else
#define GLOBALDLLOBJECTS_API __declspec(dllimport)
#endif

typedef void(*TEST_CALLBACK)(int val);

class CGlobalDllObjectsApp : public CWinApp
{
public:
	CGlobalDllObjectsApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlobalDllObjectsApp)
public:
	BOOL InitInstance() override;  // Removed 'virtual', kept 'override'
	int ExitInstance() override;   // Removed 'virtual', kept 'override'
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGlobalDllObjectsApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class GLOBALDLLOBJECTS_API CTest
{
public:
 // Jedyne dozwolone konstruowanie obiektu
 // odbywa się z użyciem operatora placement
 // new.
 void* operator new(size_t) = delete;
 void* operator new(size_t, void* p) { return p; }
 void operator delete(void*) {};

 // W konstruktorze i destruktorze są wyświetlane
 // komunikaty, dzięki czemu możesz się przekonać
 // że obiekt jest konstruowany tylko wtedy, gdy
 // podłącza się pierwszy klient i niszcony tylko
 // wtedy, gdy odłącza się ostatni klient.
 CTest() {
	 ;
 }

 ~CTest() { 
	 ;
 }
 
 CTest(const CTest&);
};

#define EXPORTED_DLL_FUNCTION \
 __declspec(dllexport) __stdcall
extern "C"
{
int EXPORTED_DLL_FUNCTION GetShmpPtr(shmseg **ptr);
void EXPORTED_DLL_FUNCTION GetTestPtr(CTest*& rpTest);
int EXPORTED_DLL_FUNCTION GetNbrOfClients();
int EXPORTED_DLL_FUNCTION GetNoOfClient();
int EXPORTED_DLL_FUNCTION RegRedraw(void(*ptr)(void));
int EXPORTED_DLL_FUNCTION RegtestCall(int(*ptr)(int));
int EXPORTED_DLL_FUNCTION AlfaRedraw();
int EXPORTED_DLL_FUNCTION RegEditText(int(*ptr)(char *mytext, int edit_params, int nCmdShow));
int EXPORTED_DLL_FUNCTION AlfaEditText(char *mytext, int edit_params, int nCmdShow);
int EXPORTED_DLL_FUNCTION AlfatestCall(int);
int EXPORTED_DLL_FUNCTION Alfacallback(int);
int EXPORTED_DLL_FUNCTION AlfacallbackGo(int);
void EXPORTED_DLL_FUNCTION SetCallback(TEST_CALLBACK callback);
int EXPORTED_DLL_FUNCTION Deposit_Bitmap(int client, char *dump_file, int x1, int y1, int x2, int y2, char *drawing_file);
void EXPORTED_DLL_FUNCTION Reset_Client_Flag(int client);
void EXPORTED_DLL_FUNCTION Set_Client_Flag(int client0, unsigned char flag);
unsigned char EXPORTED_DLL_FUNCTION Check_Client_Flag(int client0);
int EXPORTED_DLL_FUNCTION Return_Active_Clients(void);
int EXPORTED_DLL_FUNCTION Return_Bitmap(int client, char *dump_file, int *x1, int *y1, int *x2, int *y2, char *drawing_file);
}
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBALDLLOBJECTS_H__25729FC2_08C5_11D2_8DB2_204C4F4F5020__INCLUDED_)
