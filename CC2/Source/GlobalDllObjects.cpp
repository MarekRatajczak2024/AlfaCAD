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

// GlobalDllObjects.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <new.h>
#include <WinUser.h>
#include <mmsystem.h>

#define GLOBALDLLOBJECTS_EXPORTS
#include "GlobalDllObjects.h"

// blocking access between processes
HANDLE g_hMutex = ::CreateMutex(NULL, FALSE, "CTest");


#pragma data_seg(".SHARDAT")
static char lpszMemory[sizeof(CTest)];
CTest* g_pTest = NULL;
int g_nClients = 0;
int g_noClient = 0;
int g_noClient_ = 0;
char* ptr1;
#define EMPTY "                                                                                                                                "
#define MAX_NUMBER_OF_CLIENTS 32
static Client_Bitmap client_bitmap[MAX_NUMBER_OF_CLIENTS]= {{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},
															{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},
															{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},
															{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY}

};

static struct shmseg shmp = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };

#pragma data_seg()


static void nooop(void)
{}

void(*AlfaRedrawPtr)(void) = NULL;
int(*AlfaEditTextPtr)(char *mytext, int edit_params, int nCmdShow) = NULL;
int(*AlfatestCallPtr)(int) = NULL;

void(*FF)(void) = nooop;

TEST_CALLBACK _theCallback = NULL;


void EXPORTED_DLL_FUNCTION SetCallback(TEST_CALLBACK callback)
{
	_theCallback = callback;
}


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CGlobalDllObjectsApp

BEGIN_MESSAGE_MAP(CGlobalDllObjectsApp, CWinApp)
	//{{AFX_MSG_MAP(CGlobalDllObjectsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlobalDllObjectsApp construction

CGlobalDllObjectsApp::CGlobalDllObjectsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGlobalDllObjectsApp object

CGlobalDllObjectsApp theApp;


BOOL CGlobalDllObjectsApp::InitInstance() 
{
 ::WaitForSingleObject(g_hMutex,INFINITE);

 if (!g_pTest)
 {

  
  g_pTest = ::new (lpszMemory) CTest;
  ASSERT(g_pTest);


 }
 
 g_nClients++;
 g_noClient++;
 if (g_noClient <= MAX_NUMBER_OF_CLIENTS) g_noClient_ = g_noClient;
 else g_noClient_ = MAX_NUMBER_OF_CLIENTS;


 client_bitmap[g_noClient_ - 1].flag = 0;
 strcpy(client_bitmap[g_noClient_ - 1].dump_file, "");

 ::ReleaseMutex(g_hMutex);
	
 return CWinApp::InitInstance();
}

int CGlobalDllObjectsApp::ExitInstance() 
{
  MMRESULT ret_timer;

 ::WaitForSingleObject(g_hMutex,INFINITE);

 if (0 == --g_nClients)
 {

  MSG msg;

  ::PeekMessage(&msg, NULL, 
   WM_QUIT, WM_QUIT, PM_REMOVE);


  g_noClient=1;
  g_noClient_ = g_noClient;

  delete g_pTest;
  g_pTest = NULL;

 }

 ::ReleaseMutex(g_hMutex);
 ::CloseHandle(g_hMutex);
	
 return CWinApp::ExitInstance();
}

extern "C"
{

int EXPORTED_DLL_FUNCTION GetShmpPtr(shmseg **ptr)
{
	ptr1 = (char*) & shmp;
	*ptr = (shmseg*)ptr1;
	if (ptr != NULL) return 1;
	return 0;
}

void EXPORTED_DLL_FUNCTION GetTestPtr(CTest*& rpTest)
{
 rpTest = g_pTest;
}

int EXPORTED_DLL_FUNCTION GetNbrOfClients()
{
 return g_nClients;
}

int EXPORTED_DLL_FUNCTION GetNoOfClient()
{
 return g_noClient;
}

int EXPORTED_DLL_FUNCTION RegRedraw(void(*ptr)(void))  //(char *ptr)
{
	AlfaRedrawPtr = ptr;
	return 1;
}

int EXPORTED_DLL_FUNCTION RegEditText(int(*ptr)(char *mytext, int edit_params, int nCmdShow))
{
	AlfaEditTextPtr = ptr;
	return 1;
}

int EXPORTED_DLL_FUNCTION RegtestCall(int(*ptr)(int))
{
	AlfatestCallPtr = ptr;
	return 1;
}


int EXPORTED_DLL_FUNCTION AlfaRedraw()
{
	if (AlfaRedrawPtr != NULL)
	{
		AlfaRedrawPtr();
		return 1;
	}
	else return 0;
}

int EXPORTED_DLL_FUNCTION AlfaEditText(char *mytext, int edit_params, int nCmdShow)
{
	if (AlfaEditTextPtr != NULL)
	{
		return AlfaEditTextPtr(mytext, edit_params, nCmdShow);
	}
	else return 0;
}


int EXPORTED_DLL_FUNCTION AlfatestCall(int val)
{
	if (AlfatestCallPtr != NULL)
	{
		return (int)AlfatestCallPtr;
	}
	else return 0;
}

int EXPORTED_DLL_FUNCTION Alfacallback(int val)
{
	if (_theCallback != NULL)
	{
		return (int)_theCallback;
	}
	else return 0;
}

int EXPORTED_DLL_FUNCTION AlfacallbackGo(int val)
{
	if (_theCallback != NULL)
	{
		_theCallback(val);
		return 1;
	}
	else return 0;
}

unsigned char EXPORTED_DLL_FUNCTION Check_Client_Flag(int client0)
{
	int client = client0 - 1;
	
	if (client_bitmap[client].flag >= 3)
	{
		client_bitmap[client].flag = 1;
		return 3;
	}
	else
	{
		
		if ((client_bitmap[client].flag == 2) || (client_bitmap[client].flag == 0)) //testing activity  //0
		{
			//reset flag
			client_bitmap[client].flag = 1;
		}
		return client_bitmap[client].flag;
	}

}

void EXPORTED_DLL_FUNCTION Set_Client_Flag(int client0, unsigned char flag)
{

	int client = client0 - 1;

	client_bitmap[client].flag = flag;
	
}

void EXPORTED_DLL_FUNCTION Reset_Client_Flag(int client0)
{
	
	int client = client0 - 1;
	if (client_bitmap[client].flag != 0)
	{
		strcpy(client_bitmap[client].dump_file, "");
	}
	client_bitmap[client].flag = 0;
	
}

int EXPORTED_DLL_FUNCTION Deposit_Bitmap(int client0, char *dump_file, int x1, int y1, int x2, int y2, char *drawing_file)
{
	int ret;
	int client = client0 - 1;
	
	if (client >= MAX_NUMBER_OF_CLIENTS) client = MAX_NUMBER_OF_CLIENTS - 1;
	
	if (client_bitmap[client].flag != 0)
	{
		strcpy(client_bitmap[client].dump_file, "");
	}

	strcpy(client_bitmap[client].dump_file, dump_file);
	client_bitmap[client].x1 = x1;
	client_bitmap[client].y1 = y1;
	client_bitmap[client].x2 = x2;
	client_bitmap[client].y2 = y2;

	strcpy(client_bitmap[client].drawing_file, drawing_file);  //file name seems to be obvious
	client_bitmap[client].flag = 1;
	
	
	return 1;
}

int EXPORTED_DLL_FUNCTION Return_Active_Clients(void)
{
	
	int client;
	int client_no = 0;
	for (client = 0; client < MAX_NUMBER_OF_CLIENTS; client++)
	{
		if (client_bitmap[client].flag != 0) client_no++;
	}
	
	return client_no;
}

int EXPORTED_DLL_FUNCTION Return_Bitmap(int client0, char *dump_file, int *x1, int *y1, int *x2, int *y2, char *drawing_file)
{
	
	int client = client0 - 1;
	if (client >= MAX_NUMBER_OF_CLIENTS) client = MAX_NUMBER_OF_CLIENTS - 1;
    
	if (client_bitmap[client].flag != 0)
	{
		strcpy(dump_file, client_bitmap[client].dump_file);
		*x1 = client_bitmap[client].x1;
		*y1 = client_bitmap[client].y1;
		*x2 = client_bitmap[client].x2;
		*y2 = client_bitmap[client].y2;
		strcpy(drawing_file, client_bitmap[client].drawing_file);

		
		return 1;
	}
	else
	{
	
		return 0;
	}
}


}

