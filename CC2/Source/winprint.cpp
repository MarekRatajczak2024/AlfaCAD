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

#define __WINPRINT__

#define __WX__
#pragma warning(disable : 4996)

#ifndef LINUX
#include <afxdlgs.h>
#include <winspool.h>
#include <shlobj_core.h>
////#include "WinGdi.h"
#include "..\..\source\res\resource.h"
#else
#include <stdlib.h>
#include <sys/utsname.h>
#include "res/resource.h"
#endif

#include <time.h>
#ifndef LINUX
////#include <Windows.h>
#endif
#ifdef MACOS
#include <mouse.h>
#endif
#include "bgiext.h"
#include "menu.h"
#include "tinyfiledialogs.h"

#include "leak_detector_cpp.hpp"

#include "o_prn_cups.h"

#ifndef LINUX
#include "AlfaObjects\GlobalDllObjects.h"
#include "o_printerwarmer.h"
#else
#include <stdio.h>
#include <cups/cups.h>

#define BOOL unsigned char
#define TRUE true
#include <stdio.h>
#include <string.h>
#define MAXPATH   260

#define geometryfileName "./kdialog.geo"

typedef struct Client_Bitmap
{
    unsigned char flag = 0;
    int x1, y1, x2, y2;
    char drawing_file[MAXPATH];
    char dump_file[MAXPATH];
} Client_Bitmap;
#endif

typedef struct
{
	char quote[160];
	char author[64];
	char about[16];
} QUOTE;
#define EMPTY "                                                                                                                                "

#define MAX_NUMBER_OF_WINDOWS 16  //also copied in bib_e.h

static Client_Bitmap client_bitmap[MAX_NUMBER_OF_WINDOWS] = { {0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},
									{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},{0,0,0,0,0,EMPTY,EMPTY},

};

extern "C" {
#ifdef MACOS
extern void set_sleep_state(BOOL state);
extern char *al_FileNameDialog(
		char const * aTitle , /* NULL or "" */
		char const * aDefaultPathAndFile , /* NULL or "" */
		int aNumOfFilterPatterns0 , /* 0 */
		char *aFilterPatterns0 , /* NULL or {"*.jpg","*.png"} */
		char const * aSingleFilterDescription , /* NULL or "image files" */
		int aAllowMultipleSelects,
		int in_out
		);
extern int show_native_message_box(char *title, char *heading, char *text);
#endif
void PutPixelWin(int i, int i1, int j, unsigned char key);
void PutPixelWinRGB(int i, int i1, int j, unsigned char keyR, unsigned char keyG, unsigned char keyB);
int My_GetOpenFolder(char *f_name, char *sz__current_path_file, char *sz__default_path_file, char *sz__current_mask, char *dlg_name); // , int font_height, int font_width, char *font_name)
int My_GetOpenFileName(char *f_name, char *sz__current_path_file, char *sz__current_mask, char *dlg_name, BOOL in_out, int cur);
int Print2Page(int WINPRINT_DEF);
void Initial_Message(char file_name[255]);
void Child_Message(int mode_);
void Test_App(int doit, char *file_name);
void StartWinPage(void);
void EndWinPage(void);
void EndWinDoc(void);
int StartWinPrintDoc(char *doc_name);

extern int my_file_exists(char *name);
extern long pikseleX(double jednostki);
extern long pikseleY(double jednostki);

#ifndef LINUX
int GetShmpPtr_1(struct shmseg **shmp_);
BOOL clip_cursor(RECT *lpRect);
#endif

extern BOOL wayland;

int GetPrintPageWidth(void);
int GetPrintPageHeight(void);
int GetPrintHorzres(void);
int GetPrintVertres(void);
int GetPrintHorizDensity(void);
int GetPrintVertDensity(void);

extern int Number_of_clients;
extern int Client_number;
extern BOOL just_file;
extern int GFX_WIN;
extern int alert(const char *s1, const char *s2, const char *s3, const char *b1, const char *b2, int c1, int c2);
extern int Load_Last_Window_Settings(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);

extern void redraw(void);

extern void reset_flag(void);

unsigned char check_client_flag(void);

extern int DRAWING_NUMBER;

extern int utf82unicode(unsigned char* utf8text, unsigned char* unicodetext);
extern int code_page;
extern void komunikat0(int n);
extern void restore_focus(void);

extern void my_sleep(int sleepMs);

extern void dialog_cursor(int on);
extern void lock_mouse(void);
extern void _free_mouse(void);
extern void free_mouse(void);
extern void get_posXY(double *pozx, double *pozy);
extern void set_posXY(double pozx, double pozy);
extern int getmaxx(void);
extern int getmaxy(void);
extern void(*CUR_ON)(double, double);
extern void position_mouse(int x, int y);
extern void position_mouse_xy(int x, int y);
extern void set_forget_mouse(int mouse_x, int mouse_y);

extern void set_cups_printer_name(int no, CUPS_PRINTERS *cups_printer, BOOL defaultp, int p_counter);

extern int Save_Update_flex(int save_upd, int *curr_h, int *curr_v);

extern int Test_App_Shm(int doit, char *file_name);

extern void set_cups_default_printer_name(CUPS_PRINTERS* cups_printer);

void Done_CUPS_Printers(void);
BOOL SetDefaultPrinter_(char* printer_name);

extern void Odczyt_licznikow(void);

extern BOOL MacOS15;

extern void komunikat_str_short(char* st, BOOL stay, BOOL center);
extern void remove_short_notice(void);
}

extern int win2unicodefactory(char* wintext, char* unicodetext, int codepage);
extern void unicode2utf8(char* unicodetext, unsigned char* utf8text);


extern int GET_ALL_PRINTERS(void);
extern int GET_DEF_PRINTER(char *szPName, int *iBufferSize);
extern int win2unicode(char *wintext, char *unicodetext);

extern
int Win_Initial_Message(char* logoandquote, char* Ainfo);


#ifndef LINUX
extern int VeryMy_GetOpenFolder(HWND hwnd, char* f_name, char* sz__current_path_file, char* sz__default_path_file, char* sz__current_mask, char* dlg_name);
static HDC hDC=NULL;
static LPDEVMODE devmode;
extern HWND win_get_window_(void);

// Custom class to mimic your working File Dialog forcing behavior
class CAlphaPrintDialog__ : public CPrintDialog
{
public:
	CAlphaPrintDialog__(BOOL bPrintSetupOnly, DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES, CWnd* pParentWnd = NULL)
		: CPrintDialog(bPrintSetupOnly, dwFlags, pParentWnd) {}

protected:
	virtual BOOL OnInitDialog() {
		CPrintDialog::OnInitDialog();

		// LIFT IT UP: Force the dialog to the absolute front of the Z-order stack
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		// Optional: Center it perfectly inside your AlphaCAD main window frame
		CenterWindow(AfxGetMainWnd());

		return TRUE;
	}
};

class CAlphaPrintDialog : public CPrintDialog
{
public:
	CAlphaPrintDialog(BOOL bPrintSetupOnly, DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES, CWnd* pParentWnd = NULL)
		: CPrintDialog(bPrintSetupOnly, dwFlags, pParentWnd) {}

protected:
	virtual BOOL OnInitDialog() {
		CPrintDialog::OnInitDialog();

		// ======================================================================
		// PARAMETER MANIPULATION OVERRIDE FOR THE SYSTEM PRINTER DIALOG
		// ======================================================================
		// 1. Force the OS Window Manager to inject the structural TOPMOST extended style.
		// This makes the window act like a high-priority notification layer.
		ModifyStyleEx(0, WS_EX_TOPMOST | WS_EX_APPWINDOW);

		// 2. Clear any clipping restrictions that allow the dialog to fall behind its parent
		ModifyStyle(WS_CHILD, WS_POPUP);

		// 3. Brute-force the window to the front of the monitor stack
		::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0,
					   SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_FRAMECHANGED);

		// 4. Forcefully grab focus away from the underlying AlfaCAD thread
		::SetForegroundWindow(GetSafeHwnd());
		::SetActiveWindow(GetSafeHwnd());

		// 5. Center it cleanly inside AlfaCAD's workspace frame so it's instantly visible
		CenterWindow(AfxGetMainWnd());

		return TRUE;
	}
};




#endif
static BOOL hdc_deleted=TRUE;

typedef struct
{
  int technology;
  int horzsize;
  int vertsize;
  int horzres;
  int vertres;
  int bitspixel;
  int aspectx;
  int aspecty;
  int colorres;
  int physicalwidth;
  int physicalheight;
  int physicaloffsetx;
  int physicaloffsety;
  int scalingfactorx;
  int scalingfactory;
  int getphyspagesize;
  int getprintingoffset;
  int getscalingfactor;
  int logpixelx;
  int logpixely;
} DEVICECAPS;


DEVICECAPS devicecaps={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define CUPS_PRINTER_NO_MAX 32
static int cups_printer_no=0;
static int cups_printer_no_def=0;


CUPS_PRINTERS cups_printers[CUPS_PRINTER_NO_MAX]={NULL};

#ifndef LINUX
BOOL clip_cursor(RECT *lpRect)
{ 
	return ClipCursor(lpRect);
}
#endif

void StartWinPage(void)
{
#ifndef LINUX
  StartPage(hDC);
#else

#endif
}

void EndWinPage(void)
{
#ifndef LINUX
  EndPage(hDC);
#else

#endif
}

void EndWinDoc(void)
{
#ifndef LINUX
  EndDoc(hDC);
  hdc_deleted=ReleaseDC(NULL, hDC);  //??????????????????????
  hdc_deleted=DeleteDC(hDC);
#else

#endif
}

void Child_Message(int mode_)
{ int confirm;

#ifdef LINUX
	struct utsname *buf = (struct utsname*)malloc(sizeof(utsname));
	int ret = uname(buf);

	if (strstr(buf->sysname,"Darwin")!=NULL) {
		int osxver=atoi(buf->release);
		if (osxver>23) MacOS15=TRUE;
	}

	free(buf);
#endif

    if (mode_==0)
    {
#ifndef LINUX
          confirm=MessageBox(NULL, _NEW_WINDOW_, _NEW_WINDOW_T_, MB_YESNO | MB_ICONQUESTION);

          if (confirm==IDNO)
          {
              exit(0);
          }
#else

#endif
    }
}

void Initial_Message(char file_name[255])
{ int ret=2;
 char *l_and_q;
 FILE *fptr;
 QUOTE Quote;
 char quote_about[32];
#ifndef LINUX
#ifdef BIT64
 char Ainfo[16] = u8"AlfaCAD Info";
 char logoandquote[512] = u8"AlfaCAD for Windows (x64) © Marek Ratajczak, 2020-2026";
#else
 char Ainfo[16] = u8"AlfaCAD Info";
 char logoandquote[512] = u8"AlfaCAD for Windows © Marek Ratajczak, 2020-2026";
#endif
#else
    char Ainfo[16] = u8"AlfaCAD Info";
#ifndef MACOS
    char logoandquote[512] = u8"AlfaCAD for Linux (x64) © Marek Ratajczak, 2020-2026";
#else
#ifdef ARM64
	char logoandquote[512] = u8"AlfaCAD for MacOS (arm64)\n © Marek Ratajczak, 2020-2026";
#else
    char logoandquote[512] = u8"AlfaCAD for MacOS (x64)\n © Marek Ratajczak, 2020-2026";
#endif
#endif
#endif

 char NT_version[64];

#ifndef LINUX
 char* (CDECL* pwine_get_version)(void);
 
 HMODULE hntdll = GetModuleHandle("ntdll.dll");
 if (!hntdll)
 {
	 strcat(logoandquote,"\nNot running on NT.");
 }

 pwine_get_version = (char* (CDECL*)())GetProcAddress(hntdll, "wine_get_version");

 if (pwine_get_version)
 {
	 sprintf(NT_version, "\nRunning on Wine %s", pwine_get_version());
	 strcat(logoandquote, NT_version);
 }
 else
 {
	 strcat(logoandquote, "\nRunning on Windows");
 }

 srand((unsigned int)time(0));
 int rand_result = (rand() % 2000);
 Sleep(rand_result);
 rand_result = (rand() % max_quote);

 fptr = fopen(_QUOTE_, "rb+, ccs=UTF-8");
 if (fptr != NULL)
 {
	 fseek(fptr, sizeof(QUOTE) * rand_result, SEEK_SET);
	 fread(&Quote, sizeof(QUOTE), 1, fptr);
	 fclose(fptr);

	 l_and_q = strcat(logoandquote, about_pre);

	 //ret = utf82unicode((unsigned char*)logoandquote, (unsigned char*)&logoandquote1);
     if (strlen(Quote.about) < 2) strcpy(quote_about, "");
     else sprintf(quote_about, "%s%s ", _about_, Quote.about);

	 l_and_q = strcat(logoandquote, quote_about);
	 l_and_q = strcat(logoandquote, ":\n\n'");
	 l_and_q = strcat(logoandquote, Quote.quote);
	 l_and_q = strcat(logoandquote, "'\n\n");
	 l_and_q = strcat(logoandquote, Quote.author);	
 }
 
 MSGBOXPARAMS msgbox = { 0 };
 msgbox.cbSize = sizeof(MSGBOXPARAMS);
 msgbox.hwndOwner = NULL;
 msgbox.hInstance = GetModuleHandle(NULL);
 msgbox.lpszText = (LPCSTR)logoandquote;
 msgbox.lpszCaption = (LPCSTR)Ainfo;
 msgbox.dwStyle = MB_OKCANCEL | MB_SETFOREGROUND | MB_USERICON;
 msgbox.lpszIcon = MAKEINTRESOURCE(IDI_ALFA);
 

 ////ret = MessageBoxIndirect(&msgbox); // 65); // 65);  MB_OK);

 ret = Win_Initial_Message(logoandquote, Ainfo);
 
if (ret == IDCANCEL) exit(0);
#else
	char str_linux[100]="";
#ifdef MACOS
    struct utsname *buf = (struct utsname*)malloc(sizeof(utsname));
    ret = uname(buf);

    sprintf(str_linux,"\nRunning on %s %s %s (%s)", buf->sysname,buf->nodename,buf->release,buf->machine);

	if (strstr(buf->sysname,"Darwin")!=NULL) {
		int osxver=atoi(buf->release);
		if (osxver>23) MacOS15=TRUE;
	}

    free(buf);

    ///////////////////
#else LINUX
    char *session_type = getenv("XDG_SESSION_TYPE");

    if (session_type != NULL) {
        if (strcmp(session_type, "wayland") == 0) {
            printf("Running under Wayland\n");
            strcat(str_linux,"\nunder Wayland\n");
            wayland=TRUE;
        } else if (strcmp(session_type, "x11") == 0) {
            printf("Running under Xorg (X11)\n");
            strcat(str_linux,"\nunder Xorg (X11)\n");
        } else {
            printf("Unknown session type: %s\n", session_type);
        }
    } else {
        printf("XDG_SESSION_TYPE environment variable not set. Cannot determine session type.\n");
    }
#endif
    //////////////////

    strcat(logoandquote,str_linux);

    //srand((unsigned int)time(0));
    int rand_result = (rand() % 2000);
    my_sleep(rand_result);
    rand_result = (rand() % max_quote);

    fptr = fopen(_QUOTE_, "rb+, ccs=UTF-8");
    if (fptr != NULL)
    {
        fseek(fptr, sizeof(QUOTE) * rand_result, SEEK_SET);
        fread(&Quote, sizeof(QUOTE), 1, fptr);
        fclose(fptr);

        l_and_q = strcat(logoandquote, about_pre);

        if (strlen(Quote.about) < 2) strcpy(quote_about, "");
        else sprintf(quote_about, "%s%s ", _about_, Quote.about);
        l_and_q = strcat(logoandquote, quote_about);
        l_and_q = strcat(logoandquote, ":\n\n'");
        l_and_q = strcat(logoandquote, Quote.quote);
        l_and_q = strcat(logoandquote, "'\n\n");
        l_and_q = strcat(logoandquote, Quote.author);
    }

#ifdef ALLEGRO5
#ifndef MACOS
    int X_WIN_ORIG_, Y_WIN_ORIG_, WIN_WIDTH_, WIN_HEIGHT_;

    GFX_WIN = 1;
    int ret1=Load_Last_Window_Settings(&X_WIN_ORIG_, &Y_WIN_ORIG_, &WIN_WIDTH_, &WIN_HEIGHT_);
    GFX_WIN = 0;

    ret = tinyfd_messageBox(
            Ainfo , /* NULL or "" */
            logoandquote, /* NULL or ""  may contain \n and \t */
            "okcancel" , /* "ok" "okcancel" "yesno" "yesnocancel" */
            "info" , /* "info" "warning" "error" "question" */
            0, (X_WIN_ORIG_ + WIN_WIDTH_/2), (Y_WIN_ORIG_ + WIN_HEIGHT_/2)); /* 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel */
#else
	//char heading[64]="info";
	ret=show_native_message_box(Ainfo, "info", logoandquote);
	if (ret==2) ret=0;
#endif
#else

    ret = tinyfd_messageBox(
            Ainfo , /* NULL or "" */
            logoandquote, /* NULL or ""  may contain \n and \t */
            "okcancel" , /* "ok" "okcancel" "yesno" "yesnocancel" */
            "info" , /* "info" "warning" "error" "question" */
            0, -1, -1);
#endif
    if (ret == 0) exit(0);
#endif
}

int Deposit_Bitmap_Multi(int client0, char *dump_file, int x1, int y1, int x2, int y2, char *drawing_file)
{
	int client = client0;

	strcpy(client_bitmap[client].dump_file, dump_file);
	client_bitmap[client].x1 = x1;
	client_bitmap[client].y1 = y1;
	client_bitmap[client].x2 = x2;
	client_bitmap[client].y2 = y2;

	strcpy(client_bitmap[client].drawing_file, drawing_file);  //file name seems to be obvious
	client_bitmap[client].flag = 1;

	return 1;
}

int deposit_hbitmap(char *dump_file, int real_x1, int real_y1, int real_x2, int real_y2, char *drawing_file)
{
	int ret=0;

    ret = Deposit_Bitmap_Multi(DRAWING_NUMBER, dump_file, real_x1, real_y1, real_x2, real_y2, drawing_file);
	
	return ret;
}

unsigned char get_client_flag(void)
{
#ifndef LINUX
	return Check_Client_Flag(Client_number);
#else
    return 1;
#endif
}

int return_active_clients_no(void)
{
#ifndef LINUX
	return Return_Active_Clients();
#else
    return 1;
#endif
}

int Return_Bitmap_Multi(int client0, char *dump_file, int *x1, int *y1, int *x2, int *y2, char *drawing_file)
{
	int client = client0;

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

int return_hbitmap(int client, char *dump_file, int *real_x1, int *real_y1, int *real_x2, int *real_y2, char *drawing_file)
{
	int ret;

	ret = Return_Bitmap_Multi(client, dump_file, real_x1, real_y1, real_x2, real_y2, drawing_file);
	return ret;
}

void reset_flag(void)
{
#ifndef LINUX
	Reset_Client_Flag(Client_number);
#endif
}

void set_client_flag(int client_no, unsigned char flag)
{
#ifndef LINUX
	Set_Client_Flag(client_no, flag);
#endif
}

#ifndef LINUX
int GetShmpPtr_1(struct shmseg **shmp_)
{
    return GetShmpPtr(shmp_);
}
#endif

void Test_App(int doit, char *file_name)
{
#ifndef LINUX
 CTest* pTest;
 GetTestPtr(pTest);
 ASSERT(pTest);
 if (pTest)
 {
  CString str;

  Number_of_clients=GetNbrOfClients();
  Client_number=GetNoOfClient();
  //int ret = RegRedraw(redraw);
 }
#endif
    
  int ret = Test_App_Shm(doit, file_name);

}

int GetPrintPageWidth(void)
{
   return devicecaps.horzsize;
}

int GetPrintPageHeight(void)
{
   return devicecaps.vertsize;
}

int GetPrintHorzres(void)
{
   return devicecaps.horzres;
}

int GetPrintVertres(void)
{
   return devicecaps.vertres;
}

int GetPrintHorizDensity(void)
{
   return devicecaps.logpixelx;
}

int GetPrintVertDensity(void)
{
   return devicecaps.logpixely;
}

void PutPixelWinRGB(int i, int i1, int j, unsigned char r, unsigned char g, unsigned char b)
{
#ifndef LINUX
  HPEN hpen;
   
  hpen = CreatePen (PS_SOLID, 0, RGB(r,g,b));

  SelectObject(hDC,hpen);  //tu zmieniac kolor na key

  MoveToEx(hDC,i,j,NULL);
  LineTo(hDC,i1+1,j);

  DeleteObject(hpen);
#else

#endif
}

int Get_HDC_Height(void)
{
    #ifndef LINUX
    return GetDeviceCaps(hDC, VERTRES);
    #else
    return 0;
    #endif
}

void PutPixelWin(int i, int i1, int j, unsigned char key)
{
	unsigned char byte1, byte2, byte3;
#ifndef LINUX
	HPEN hpen;

	byte1 = _dac_normal[key][0] << 2;
	byte2 = _dac_normal[key][1] << 2;
	byte3 = _dac_normal[key][2] << 2;

	hpen = CreatePen(PS_SOLID, 0, RGB(byte1, byte2, byte3));

	SelectObject(hDC, hpen);  //tu zmieniac kolor na key

	MoveToEx(hDC, i, j, NULL);
	LineTo(hDC, i1 + 1, j);

	DeleteObject(hpen);
#else

#endif
}

int StartWinPrintDoc(char *doc_name)
{
#ifndef LINUX
    DOCINFO di;
  memset(&di, '\0', sizeof(DOCINFO));
  di.cbSize = sizeof(DOCINFO);
  di.lpszDocName = doc_name;

  if (StartDoc(hDC, &di) != SP_ERROR)
  {
	  return 0;
  }
#else

#endif
  return 1;
}

int My_GetOpenFileName(char* f_name, char* sz__current_path_file, char* sz__current_mask, char* dlg_name, BOOL in_out, int cur)
{
	char const* lTheOpenFileName;
    int numoffilters=0;
    double PozX0, PozY0;
    int ret;
    static int curr_h, curr_v;

    ret=Save_Update_flex(0, &curr_h, &curr_v);

    get_posXY(&PozX0, &PozY0);
#ifndef ALLEGRO5
    position_mouse(pikseleX(PozX0), pikseleY(PozY0));
	_free_mouse();
	dialog_cursor(1);
#endif

    if (!in_out)
#ifdef LINUX
#ifndef MACOS
        lTheOpenFileName = tinyfd_FileNameDialog(       //tinyfd_FileNameDialog
            dlg_name,
            sz__current_path_file,
            numoffilters,
            sz__current_mask,
            NULL,
            0);
#else

	    //char const * aTitle , /* NULL or "" */
		//char const * aDefaultPathAndFile , /* NULL or "" */
		//int aNumOfFilterPatterns0 , /* 0 */
		//char *aFilterPatterns0 , /* NULL or {"*.jpg","*.png"} */
		//char const * aSingleFilterDescription , /* NULL or "image files" */
		//int aAllowMultipleSelect
	{

		//enable_hardware_cursor();
		set_sleep_state(TRUE);
		free_mouse();

		enable_hardware_cursor();

        char *ptr=strchr(sz__current_mask,'*');
		if (ptr==NULL) ptr=sz__current_mask;
		//strcpy(sz__current_mask,"*.alf");
		strcat(sz__current_path_file,"/");
		show_os_cursor(MOUSE_CURSOR_ARROW);
		lTheOpenFileName = al_FileNameDialog(dlg_name, sz__current_path_file, numoffilters, ptr, NULL, 0, 0);
		show_os_cursor(MOUSE_CURSOR_NONE);

		disable_hardware_cursor();

		lock_mouse();
		set_sleep_state(FALSE);
	}
#endif
#else
    {
        char const* lFilterPatterns[2];

        numoffilters = 1;
        lFilterPatterns[0] = sz__current_mask;
        lFilterPatterns[1] = "";

        printf("tinyfd_FileNameDialog\n");

		// FIX: Get the active main window handle of AlfaCAD
		HWND hMainWnd = AfxGetMainWnd() ? AfxGetMainWnd()->GetSafeHwnd() : NULL;

        lTheOpenFileName = tinyfd_FileNameDialog(       //tinyfd_FileNameDialog
            dlg_name,
            sz__current_path_file,
            numoffilters,
            lFilterPatterns, //sz__current_mask,  (const char* const*)
            NULL,
            0);
    }
#endif
    else
#ifdef LINUX
    #ifndef MACOS
        lTheOpenFileName = tinyfd_saveFileDialog(
                dlg_name,
                sz__current_path_file,
                numoffilters,
                sz__current_mask,
                NULL);
#else
	//char const * aTitle , /* NULL or "" */
	//char const * aDefaultPathAndFile , /* NULL or "" */
	//int aNumOfFilterPatterns0 , /* 0 */
	//char *aFilterPatterns0 , /* NULL or {"*.jpg","*.png"} */
	//char const * aSingleFilterDescription , /* NULL or "image files" */
	//int aAllowMultipleSelect
	{

		//enable_hardware_cursor();
		set_sleep_state(TRUE);
		free_mouse();

		enable_hardware_cursor();

		char *ptr=strchr(sz__current_mask,'*');
		if (ptr==NULL) ptr=sz__current_mask;
		//strcpy(sz__current_mask,"*.alf");
		strcat(sz__current_path_file,"/");
		show_os_cursor(MOUSE_CURSOR_ARROW);
		lTheOpenFileName = al_FileNameDialog(dlg_name, sz__current_path_file, numoffilters, sz__current_mask, NULL, 0, 1);
		show_os_cursor(MOUSE_CURSOR_NONE);

		disable_hardware_cursor();

		lock_mouse();
		set_sleep_state(FALSE);
	}
#endif
#else
    {
        char const* lFilterPatterns[2];

        numoffilters = 1;
        lFilterPatterns[0] = sz__current_mask;
        lFilterPatterns[1] = "";

        lTheOpenFileName = tinyfd_saveFileDialog(
            dlg_name,
            sz__current_path_file,
            numoffilters,
            lFilterPatterns, //sz__current_mask,  (const char* const*)
            NULL);
    }
#endif
#ifdef ALLEGRO5
    position_mouse(getmaxx()/2, getmaxy()/2);

    //do {
    //    get_mouse_mickeys(&WspX_, &WspY_);
    //} while ((WspX_!=0) || (WspY_!=0));
    set_forget_mouse(getmaxx()/2, getmaxy()/2);
#else
    dialog_cursor(0);
	lock_mouse();
#endif

    set_posXY(PozX0, PozY0);
    CUR_ON(PozX0,PozY0);

	if (!lTheOpenFileName)
	{
		return 0;
	}

	strcpy(f_name, lTheOpenFileName);

    ret=Save_Update_flex(1, &curr_h, &curr_v);
#ifndef LINUX
	// ======================================================================
	// THE SMOKING GUN FIX: HEAL ALFACAD AFTER TINYFD FOCUS STEAL
	// ======================================================================
	// tinyfiledialogs leaves AlfaCAD trapped in a background state when it closes.
	// This forces Windows to restore AlfaCAD as the active top-level workspace.
	HWND hMainAppWindow = ::GetActiveWindow();
	if (hMainAppWindow == NULL)
	{
		// Fallback: If GetActiveWindow fails, grab it from MFC directly
		if (AfxGetMainWnd() != NULL) hMainAppWindow = AfxGetMainWnd()->GetSafeHwnd();
	}

	if (hMainAppWindow != NULL)
	{
		::SetActiveWindow(hMainAppWindow);
		::SetForegroundWindow(hMainAppWindow);
	}
	// ======================================================================
#endif
	return 1;

}

int My_GetOpenFolder(char *f_name, char *sz__current_path_file, char *sz__default_path_file, char *sz__current_mask, char *dlg_name) // , int font_height, int font_width, char *font_name)
{
    char const* folder_name;
    int ret;
    static int curr_h, curr_v;
    double PozX0, PozY0;
	char *f_name_ret;

    ret=Save_Update_flex(0, &curr_h, &curr_v);

    get_posXY(&PozX0, &PozY0);
#ifndef ALLEGRO5
    position_mouse(pikseleX(PozX0), pikseleY(PozY0));
	_free_mouse();
	dialog_cursor(1);
#endif

#ifndef LINUX
    HWND hwnd = win_get_window_();
    ret = VeryMy_GetOpenFolder(hwnd, f_name, sz__current_path_file, sz__default_path_file, sz__current_mask, dlg_name); // , int font_height, int font_width, char *font_name)
#else
#ifndef MACOS
    folder_name = tinyfd_selectFolderDialog(
            dlg_name , /* "" */
            sz__current_path_file ) ; /* "" */
    if (folder_name!=NULL)
        strcpy(f_name, folder_name);
#else
	enable_hardware_cursor();

	strcat(sz__current_path_file,"/");
	show_os_cursor(MOUSE_CURSOR_ARROW);
	folder_name=al_FileNameDialog(dlg_name, sz__current_path_file, 1, "*.*", NULL, 0, 2);
	if (folder_name!=NULL)
		strcpy(f_name, folder_name);

	disable_hardware_cursor();

	show_os_cursor(MOUSE_CURSOR_NONE);
#endif
#endif

#ifdef ALLEGRO5
    position_mouse(getmaxx()/2, getmaxy()/2);

    //do {
    //    get_mouse_mickeys(&WspX_, &WspY_);
    //} while ((WspX_!=0) || (WspY_!=0));
    set_forget_mouse(getmaxx()/2, getmaxy()/2);
#else
    dialog_cursor(0);
	lock_mouse();
#endif

    set_posXY(PozX0, PozY0);
    CUR_ON(PozX0,PozY0);

    ret = Save_Update_flex(1, &curr_h, &curr_v);
#ifndef LINUX
	// ======================================================================
	// THE SMOKING GUN FIX: HEAL ALFACAD AFTER TINYFD FOCUS STEAL
	// ======================================================================
	// tinyfiledialogs leaves AlfaCAD trapped in a background state when it closes.
	// This forces Windows to restore AlfaCAD as the active top-level workspace.
	HWND hMainAppWindow = ::GetActiveWindow();
	if (hMainAppWindow == NULL)
	{
		// Fallback: If GetActiveWindow fails, grab it from MFC directly
		if (AfxGetMainWnd() != NULL) hMainAppWindow = AfxGetMainWnd()->GetSafeHwnd();
	}

	if (hMainAppWindow != NULL)
	{
		::SetActiveWindow(hMainAppWindow);
		::SetForegroundWindow(hMainAppWindow);
	}
	// ======================================================================
#endif
    //if (!folder_name) return 0;
    if (strlen(f_name)==0) return 0;
    return 1;
}

void Done_CUPS_Printers(void) {
    int i;

    //zeroing
    for (i=0; i<cups_printer_no; i++)
        if (cups_printers[i].printer_name!=NULL) {
            free(cups_printers[i].printer_name);
            cups_printers[i].printer_name=NULL;
        }
    cups_printer_no=0;
}

#ifdef LINUX
int print_dest(void *user_data, unsigned flags, cups_dest_t *dest)
{
    http_t *http=CUPS_HTTP_DEFAULT;
    cups_dinfo_t *dinfo;
    const char *media;
    int i;

    if (dest->instance)
        printf("%s/%s\n", dest->name, dest->instance);
    else
        puts(dest->name);

        dinfo = cupsCopyDestInfo(http, dest);
        //devicecaps.horzsize

        cups_size_t size;

        media = dest->name;

        if (cups_printer_no<CUPS_PRINTER_NO_MAX) {
            cups_printers[cups_printer_no].printer_name = (char *) malloc(strlen(dest->name) + 1);
            strcpy(cups_printers[cups_printer_no].printer_name, dest->name);
            if (dest->is_default == 1) {
                cups_printer_no_def = cups_printer_no;
                puts("DEFAULT");
            }

            int ret = cupsGetDestMediaDefault(
                    CUPS_HTTP_DEFAULT, dest, dinfo,
                    CUPS_MEDIA_FLAGS_DEFAULT, &size);

            cups_printers[cups_printer_no].width=size.width;
            cups_printers[cups_printer_no].length=size.length;
            cups_printers[cups_printer_no].bottom=size.bottom;
            cups_printers[cups_printer_no].left=size.left;
            cups_printers[cups_printer_no].right=size.right;
            cups_printers[cups_printer_no].top=size.top;

            set_cups_printer_name(cups_printer_no, &cups_printers[cups_printer_no], dest->is_default, cups_printer_no);

            cups_printer_no++;
        }


        int count = cupsGetDestMediaCount(CUPS_HTTP_DEFAULT,
                                          dest, dinfo,
                                          CUPS_MEDIA_FLAGS_DEFAULT	); //CUPS_MEDIA_FLAGS_READY);

        for (i=0; i<dest->num_options; i++)
            printf("%s=%s\n",dest->options[i].name,dest->options[i].value);
        for (i = 0; i < count; i ++) {
            if (cupsGetDestMediaByIndex(CUPS_HTTP_DEFAULT, dest, dinfo,
                                        i, CUPS_MEDIA_FLAGS_DEFAULT,
                                        &size)) {

                printf("%s:\n", size.media);
                printf("   Width: %.2fin\n", size.width / 2540.0);
                printf("  Length: %.2fin\n", size.length / 2540.0);
                printf("  Bottom: %.2fin\n", size.bottom / 2540.0);
                printf("    Left: %.2fin\n", size.left / 2540.0);
                printf("   Right: %.2fin\n", size.right / 2540.0);
                printf("     Top: %.2fin\n", size.top / 2540.0);
            } else
                puts("Printer not available");
        }

    return 1;
}
#else


void set_cups_printer_name_(CUPS_PRINTERS* cups_printer, BOOL defaultp, int p_counter)  //p_counter is just for compatibility with Unix
{

    //memmove(&cups_printers[cups_printer_no], cups_printer, sizeof(CUPS_PRINTERS));

    cups_printers[cups_printer_no].printer_name = (char*)malloc(strlen(cups_printer->printer_name) + 1);
    strcpy(cups_printers[cups_printer_no].printer_name, cups_printer->printer_name);
    if (defaultp == 1) {
        cups_printer_no_def = cups_printer_no;
        //puts("DEFAULT");
    }

    cups_printers[cups_printer_no].width = cups_printer->width;
    cups_printers[cups_printer_no].length = cups_printer->length;
    cups_printers[cups_printer_no].bottom = cups_printer->bottom;
    cups_printers[cups_printer_no].left = cups_printer->left;
    cups_printers[cups_printer_no].right = cups_printer->right;
    cups_printers[cups_printer_no].top = cups_printer->top;


    set_cups_printer_name(cups_printer_no, &cups_printers[cups_printer_no], defaultp, 0);

    cups_printer_no++;
}

BOOL SetDefaultPrinter_(char *printer_name)
{
    return SetDefaultPrinter((LPCTSTR)printer_name);
}

#endif

int Print2Page(int WINPRINT_DEF)
{
    int i;

    //zeroing
    for (i = 0; i < cups_printer_no; i++)
        if (cups_printers[i].printer_name != NULL) {
            free(cups_printers[i].printer_name);
            cups_printers[i].printer_name = NULL;
        }
    cups_printer_no = 0;

#ifndef LINUX
  static char temp1[64+1];
  CUPS_PRINTERS cups_printer;
  LPCSTR temp2;
  int Error1;

  LPCSTR winspooltext; 

  LPDWORD lPrinterNameLength=0;

  char szPName[256];
  int iBufferMax=255, iRes=-1;

 if (WINPRINT_DEF)
 {
	
    iRes = GET_ALL_PRINTERS();
	iRes=GET_DEF_PRINTER(szPName, &iBufferMax);

	strncpy(temp1, szPName, 32)	;
	temp2=(LPCTSTR)temp1;
	winspooltext="WINSPOOL";

 	// ======================================================================
 	// THE SYNCHRONIZATION GATE
 	// ======================================================================
 	// If the user opens the print setup panel incredibly fast, and the
 	// background warming thread is still mid-handshake, we wait safely for it to
 	// finish. This permanently stops the threads from colliding and crashing!
 	if (!IsPrinterWarmedUp())
 	{
 		komunikat_str_short((char*)_NETWORK_PRINTER_, FALSE, TRUE);
 		int loopSafetyWatchdog = 0;
 		while (!IsPrinterWarmedUp() && loopSafetyWatchdog < 200)
 		{
 			// Pause for 100 milliseconds to let the background thread work
 			::Sleep(100);

 			// Safety watchdog: after 20 seconds, break out no matter what
 			// to prevent freezing AlphaCAD if the printer disconnected
 			loopSafetyWatchdog++;
 		}
 		komunikat_str_short((char*)"", FALSE, TRUE);
 		remove_short_notice();
 	}
 	// ======================================================================

#ifndef LINUX
#ifndef BIT64
    komunikat_str_short((char*)_NETWORK_PRINTER_, FALSE, TRUE);
#endif
#endif

	////hDC = CreateDC(winspooltext, temp2, NULL, NULL);
	///devmode contains the pre-verified printer properties from the dialog, Windows skips the network query and generates the hDC almost instantly.
 	hDC = CreateDC(winspooltext, temp2, NULL, devmode);
    ASSERT(hDC);

#ifndef LINUX
#ifndef BIT64
    komunikat_str_short((char*)"", FALSE, TRUE);
    remove_short_notice();
#endif
#endif



    cups_printer.printer_name = temp1;

    set_cups_default_printer_name(&cups_printer);
 }

#ifdef OLDSTUFF
	else
{
    // ======================================================================
    // PASS 2: AUTOMATED FAST PRINTING (NO DANGEROUS UI, NO DRIVER CRASHES)
    // ======================================================================
    Error1 = 0;

    char szDefaultPrinter[256] = {0};
    DWORD dwSize = sizeof(szDefaultPrinter);

    // 1. Fetch the Windows OS default printer name instantly from cache
    if (::GetDefaultPrinterA(szDefaultPrinter, &dwSize))
    {
        strncpy(temp1, szDefaultPrinter, sizeof(temp1) - 1);
        temp1[sizeof(temp1) - 1] = '\0';
        temp2 = (LPCTSTR)temp1;

    	/*
        // 2. Fetch the default settings structure directly from the system registry.
        // This is safe, pure data processing—no windows open, so nothing can crash!
        HANDLE hPrinter = NULL;
        if (::OpenPrinterA(const_cast<LPSTR>(temp2), &hPrinter, NULL))
        {
            LONG dwNeeded = ::DocumentPropertiesA(NULL, hPrinter, const_cast<LPSTR>(temp2), NULL, NULL, 0);
            if (dwNeeded > 0)
            {
                devmode = (LPDEVMODE)malloc(dwNeeded);
                if (devmode != NULL)
                {
                    if (::DocumentPropertiesA(NULL, hPrinter, const_cast<LPSTR>(temp2), devmode, NULL, DM_OUT_BUFFER) != IDOK)
                    {
                        free(devmode);
                        devmode = NULL; // Safe fallback if structure reading fails
                    }
                }
            }
            ::ClosePrinter(hPrinter);
        }
        */
    	/*
    	// Fetch the default settings structure directly from the system registry.
    	HANDLE hPrinter = NULL;
    	if (::OpenPrinterA(const_cast<LPSTR>(temp2), &hPrinter, NULL))
    	{
    		// PASS 1: Find out how much memory the printer properties need
    		LONG dwNeeded = ::DocumentPropertiesA(NULL, hPrinter, const_cast<LPSTR>(temp2), NULL, NULL, 0);
    		if (dwNeeded > 0)
    		{
    			devmode = (LPDEVMODE)malloc(dwNeeded);
    			if (devmode != NULL)
    			{
    				// PASS 2: THE SECURE HANDSHAKE
    				// Using DM_OUT_BUFFER | DM_IN_BUFFER forces Windows to pull the current active system-driver settings
    				// (like an active A5 paper selection) straight out of the user profile registry into your pointer!
    				LONG lResult = ::DocumentPropertiesA(
						NULL,
						hPrinter,
						const_cast<LPSTR>(temp2),
						devmode,        // Output buffer
						NULL,           // No prompt window
						DM_OUT_BUFFER   // Extract current user configuration state
					);

    				if (lResult != IDOK)
    				{
    					free(devmode);
    					devmode = NULL;
    				}
    			}
    		}
    		::ClosePrinter(hPrinter);
    	}
    	*/
    	/*
    	// Fetch the active user settings structure directly from the system registry.
    	HANDLE hPrinter = NULL;
    	if (::OpenPrinterA(const_cast<LPSTR>(temp2), &hPrinter, NULL))
    	{
    		// STEP 1: Determine the memory block layout allocation boundaries
    		LONG dwNeeded = ::DocumentPropertiesA(NULL, hPrinter, const_cast<LPSTR>(temp2), NULL, NULL, 0);
    		if (dwNeeded > 0)
    		{
    			devmode = (LPDEVMODE)malloc(dwNeeded);
    			if (devmode != NULL)
    			{
    				// STEP 2: Pre-populate the buffer with the machine's global base layout
    				::DocumentPropertiesA(NULL, hPrinter, const_cast<LPSTR>(temp2), devmode, NULL, DM_OUT_BUFFER);

    				// STEP 3: THE LIVE SYSTEM HANDSHAKE (THE PREFERENCES MERGE)
    				// Passing BOTH flags tells Windows to fetch the user's customized preference profile
    				// (like an active A5 selection) from the system registry and merge it right over the base.
    				LONG lResult = ::DocumentPropertiesA(
						NULL,
						hPrinter,
						const_cast<LPSTR>(temp2),
						devmode,          // Output target block
						devmode,          // Input base block to merge into
						DM_OUT_BUFFER | DM_IN_BUFFER // Force a registry profile sync
					);

    				if (lResult != IDOK)
    				{
    					free(devmode);
    					devmode = NULL;
    				}
    			}
    		}
    		::ClosePrinter(hPrinter);
    	}
        */
    	// STEP 1: Configure an explicit, authenticated Win32 access request
    	PRINTER_DEFAULTSA pd = { 0 };
    	pd.pDatatype = NULL;
    	pd.pDevMode  = NULL;
    	// PRINTER_ACCESS_USE grants permission to read the live User Preferences registry bucket
    	pd.DesiredAccess = PRINTER_ACCESS_USE;

    	HANDLE hPrinter = NULL;
    	// Pass our 'pd' structure pointer as the last parameter instead of NULL
    	if (::OpenPrinterA(const_cast<LPSTR>(temp2), &hPrinter, &pd))
    	{
    		// STEP 2: Determine the memory block layout allocation boundaries
    		LONG dwNeeded = ::DocumentPropertiesA(NULL, hPrinter, const_cast<LPSTR>(temp2), NULL, NULL, 0);
    		if (dwNeeded > 0)
    		{
    			devmode = (LPDEVMODE)malloc(dwNeeded);
    			if (devmode != NULL)
    			{
    				// STEP 3: Pre-populate the buffer with the machine's global base layout
    				::DocumentPropertiesA(NULL, hPrinter, const_cast<LPSTR>(temp2), devmode, NULL, DM_OUT_BUFFER);

    				// STEP 4: THE LIVE SYSTEM HANDSHAKE (THE PREFERENCES MERGE)
    				// Now that our connection handle has PRINTER_ACCESS_USE permissions,
    				// Windows will successfully pull the active A5 layout from the registry!
    				LONG lResult = ::DocumentPropertiesA(
						NULL,
						hPrinter,
						const_cast<LPSTR>(temp2),
						devmode,          // Output target block
						devmode,          // Input base block to merge into
						DM_OUT_BUFFER | DM_IN_BUFFER // Force a registry profile sync
					);

    				if (lResult != IDOK)
    				{
    					free(devmode);
    					devmode = NULL;
    				}
    			}
    		}
    		::ClosePrinter(hPrinter);
    	}

    }
    else
    {
        // No default printer is set up in Windows
        ::MessageBoxA(NULL, "Printing Error: No default printer discovered in Windows. Please assign a printer in Windows Settings.", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    hDC = NULL;
    winspooltext = "WINSPOOL";

    // 3. Open the Device Context using our safely extracted properties
    hDC = ::CreateDCA(winspooltext, temp2, NULL, devmode);
    ASSERT(hDC);

    // Clean up our local data copy immediately
    if (devmode != NULL)
    {

    		// --- STEP 1: VERIFY MODERN FIELD ASSIGNMENTS ---
    		// Modern IPP class drivers specify the active paper boundary via standard sizing flags.
    		// If the DM_PAPERSIZE bit is checked, the driver has confirmed a layout change!
    		if (devmode->dmFields & DM_PAPERSIZE)
    		{
    			// 1 = Letter, 9 = A4, 11 = A5, 258 = Custom
    			short currentPaperID = devmode->dmPaperSize;

    			// If it specifically reports 11 (DMPAPER_A5), you can force-override your layout
    			// fallback bounds instantly without relying on broken driver handshakes!
    			if (currentPaperID == 11 || currentPaperID == DMPAPER_A5)
    			{
    				// A5 metrics at 600 DPI calculated natively:
    				// Width:  148mm -> 5.82 inches * 600 = 3496 pixels
    				// Height: 210mm -> 8.26 inches * 600 = 4960 pixels
    				printf("AlfaCAD Target Verified: Active Layout is A5\n");
    			}
    		}

    		// --- STEP 2: CHECK EXPLICIT USER HEIGHT/WIDTH ---
    		// If the user set a highly specific custom tray configuration, the size values
    		// will be written directly into dmPaperWidth and dmPaperLength in tenths of a millimeter.
    		if ((devmode->dmFields & DM_PAPERWIDTH) && (devmode->dmFields & DM_PAPERLENGTH))
    		{
    			// For an A5 sheet, width will report 1480 and length will report 2100
    			double paperWidthInMM  = devmode->dmPaperWidth / 10.0;
    			double paperHeightInMM = devmode->dmPaperLength / 10.0;

    			// Use these raw millimeter dimensions to scale your AlfaCAD drawing tiles instantly!
    			if (paperWidthInMM > 0 && paperHeightInMM > 0)
    			{
    				// Calculate your active pixels using your 600 DPI logpixelx variable:
    				int modernWidthInPixels  = (int)((paperWidthInMM / 25.4) * 600);
    				int modernHeightInPixels = (int)((paperHeightInMM / 25.4) * 600);
    			}
    		}




    	/////////////////////
        free(devmode);
        devmode = NULL;
    }
}
#endif
	else
	{
		// ======================================================================
		// PASS 2: SIMPLIFIED RAW SPYING (ZERO DRIVER HARMFUL HANDSHAKES)
		// ======================================================================
		Error1 = 0;

		char szDefaultPrinter[256] = {0};
		DWORD dwSize = sizeof(szDefaultPrinter);

		// 1. Fetch the default printer name instantly from the OS cache
		if (::GetDefaultPrinterA(szDefaultPrinter, &dwSize))
		{
			strncpy(temp1, szDefaultPrinter, sizeof(temp1) - 1);
			temp1[sizeof(temp1) - 1] = '\0';
			temp2 = (LPCTSTR)temp1;
		}
		else
		{
			::MessageBoxA(NULL, "Printing Error: No default printer discovered in Windows. Please assign a printer in Windows Settings.", "Error", MB_OK | MB_ICONERROR);
			return 0;
		}

		winspooltext = "WINSPOOL";

		// ======================================================================
		// THE SYNCHRONIZATION GATE
		// ======================================================================
		// If the engineer opens the print setup panel incredibly fast, and the
		// background warming thread is still mid-handshake, we wait safely for it to
		// finish. This permanently stops the threads from colliding and crashing!
		int loopSafetyWatchdog = 0;
		while (!IsPrinterWarmedUp() && loopSafetyWatchdog < 200)
		{
			// Pause for 100 milliseconds to let the background thread work
			::Sleep(100);

			// Safety watchdog: after 20 seconds, break out no matter what
			// to prevent freezing AlphaCAD if the printer disconnected
			loopSafetyWatchdog++;
		}
		// ======================================================================



		// 2. Open a basic, un-handshaked Device Context directly.
		// Passing NULL as the last argument completely bypasses the driver's profile engine.
		// It runs in less than a millisecond because your startup thread warmer already primed it!
		hDC = ::CreateDCA(winspooltext, temp2, NULL, NULL);
		ASSERT(hDC);
	}


  devicecaps.aspectx= GetDeviceCaps(hDC, ASPECTX);
  devicecaps.aspecty= GetDeviceCaps(hDC, ASPECTY);
  devicecaps.bitspixel= GetDeviceCaps(hDC, BITSPIXEL);
  devicecaps.colorres= GetDeviceCaps(hDC, COLORRES);
  devicecaps.getphyspagesize= GetDeviceCaps(hDC, GETPHYSPAGESIZE);
  devicecaps.getprintingoffset= GetDeviceCaps(hDC, GETPRINTINGOFFSET);
  devicecaps.getscalingfactor= GetDeviceCaps(hDC, GETSCALINGFACTOR);
  devicecaps.horzres= GetDeviceCaps(hDC, HORZRES);
  devicecaps.horzsize= GetDeviceCaps(hDC, HORZSIZE);
  devicecaps.physicalheight= GetDeviceCaps(hDC, PHYSICALHEIGHT);
  devicecaps.physicaloffsetx= GetDeviceCaps(hDC, PHYSICALOFFSETX);
  devicecaps.physicaloffsety= GetDeviceCaps(hDC, PHYSICALOFFSETY);
  devicecaps.physicalwidth= GetDeviceCaps(hDC, PHYSICALWIDTH);
  devicecaps.scalingfactorx= GetDeviceCaps(hDC, SCALINGFACTORX);
  devicecaps.scalingfactory= GetDeviceCaps(hDC, SCALINGFACTORY);
  devicecaps.technology= GetDeviceCaps(hDC, TECHNOLOGY);
  devicecaps.vertres= GetDeviceCaps(hDC, VERTRES);
  devicecaps.vertsize= GetDeviceCaps(hDC, VERTSIZE);
  devicecaps.logpixelx= GetDeviceCaps(hDC, LOGPIXELSX);
  devicecaps.logpixely= GetDeviceCaps(hDC, LOGPIXELSY);

  if (WINPRINT_DEF) DeleteDC(hDC);

	// ========================================================
	// THE GLOBAL FOCUS HEALER FOR ALFACAD
	// ========================================================
	// If we just finished a pass, force AlfaCAD's main frame back to the top of
	// the OS focus chain. This guarantees that subsequent 'OpenFile' dialogs
	// will open in front of the screen normally!
	CWnd* pMainWnd = AfxGetMainWnd();
	if (pMainWnd != NULL && !WINPRINT_DEF)
	{
		// Force Windows to restore AlfaCAD as the top-level user interaction layer
		::SetForegroundWindow(pMainWnd->GetSafeHwnd());
		pMainWnd->SetActiveWindow();
	}

#else
    http_t *http=CUPS_HTTP_DEFAULT;
    cups_dest_t *dest;
    ipp_attribute_t *ipp_attrib, *ipp_attrib_ready;
    cups_dinfo_t *dinfo;
    const char *option=CUPS_FINISHINGS;
    cups_size_t size;

    cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, 0, 0, print_dest, NULL);

#endif
 return 1;
}

#ifndef LINUX
void startup(LPCSTR lpApplicationName, LPSTR lpParams)
{
	// additional information
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcessA
	(
		lpApplicationName,   // the path
		lpParams,                // Command line
		NULL,                   // Process handle not inheritable
		NULL,                   // Thread handle not inheritable
		FALSE,                  // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,     // Opens file in a separate console
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi           // Pointer to PROCESS_INFORMATION structure
	);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
#endif

#undef __WINPRINT__