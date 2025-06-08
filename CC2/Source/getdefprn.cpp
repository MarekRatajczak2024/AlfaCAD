//  Exam41.cpp
//  1/15/2001  (rk)
//  Last modified: 8/27/2003  (rk)
//  Test the GET_DEF_PRINTER routine
//  This is a Win32 Console Application

#pragma warning(disable : 4996)
#ifndef LINUX
#include <wtypes.h>
#include <atlstr.h>
#include "o_prn_cups.h"

#include "leak_detector_cpp.hpp"

#define MAXPRINTERBUFFERSIZE 250


  extern void set_cups_printer_name_(CUPS_PRINTERS* cups_printer, BOOL defaultp, int p_counter);


// Functions in this file
int GET_WIN_VERSION(int *,int *,int *,int*);
int GET_DEF_PRINTER(char *,int *);
int GET_ALL_PRINTERS(void);


int GET_ALL_PRINTERS(void)
{
    DWORD dwNeeded = 0;
    DWORD dwPrinters = 0;
    PRINTER_INFO_2* ppi2 = NULL;
    BOOL bFlag;
    CUPS_PRINTERS cups_printer;

    ::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 5, NULL, 0, &dwNeeded, &dwPrinters);
    if (dwNeeded == 0)
        return 635;

    // Allocate enough space for PRINTER_INFO_2...
    ppi2 = (PRINTER_INFO_2*)::GlobalAlloc(GPTR, dwNeeded);
    if (!ppi2)
        return 634;

    // The second EnumPrinters() will fill in all the current information...
    bFlag = ::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 5, (LPBYTE)ppi2, dwNeeded, &dwNeeded, &dwPrinters);

    if (!bFlag)
    {
        ::GlobalFree(ppi2);
        return 635;
    }

    // Do we have any printers?
    if (dwPrinters == 0)
        return 635;
    // Cast to PRINTER_INFO_2
    PRINTER_INFO_5* pInfo = (PRINTER_INFO_5*)ppi2;
    // Loop adding the printers to the list
    for (DWORD i = 0; i < dwPrinters; i++, pInfo++)
    {
        cups_printer.printer_name = (char*)(LPCTSTR)CString(pInfo->pPrinterName);
        cups_printer.width = 0;
        cups_printer.length = 0;
        cups_printer.bottom = 0;
        cups_printer.right = 0;
        cups_printer.left = 0;
        cups_printer.top = 0;
        set_cups_printer_name_(&cups_printer, FALSE, 0);

        //cups_printer_no++;
    }

    if (bFlag)
    {
        ::GlobalFree(ppi2);
    }

    return 0;

}

int GET_DEF_PRINTER(char *szPName, int *iBufferSize)
//  Created: 1/12/2001  (rk)
//  Last modified: 8/27/2003  (rk)
// Please report any problems or bugs to Ravi Kochhar
// by email at kochhar@physiology.wisc.edu
// The latest version of this routine can be found at:
//     http://www.neurophys.wisc.edu/ravi/software/defprinter/
//  Get the default printer (queue) name for the current system
//  This routine works for Windows 95/98/ME/NT/2000/XP
//  The return code has the following possible values:
//     0   = Normal return (success)
//     121 = Supplied buffer is too small
//     606 = Unable to identify system type
//     633 = There is no Default Printer defined
//     634 = Unable to allocate memory
//     635 = Unable to enumerate printers
//  szPName is a zero terminated string that will hold the
//  default Printer (queue) name if successful
//  iBufferSize is the size (in bytes) allocated for "szPName"
//  Note: Value of iBufferSize will be modified by this routine
//  to equal the actual length of the string returned in szPName
//  (or to equal the length actually required (if return code=121))
//  Reference: http://support.microsoft.com/support/kb/articles/Q246/7/72.asp
//  Note: This function calls GET_WIN_VERSION, which is included below
{
  BOOL bFlag;
  TCHAR cBuffer[MAXPRINTERBUFFERSIZE];
  PRINTER_INFO_2 *ppi2 = NULL;
  DWORD dwNeeded = 0;
  DWORD dwReturned = 0, dwTemp=0;
  int iRes,iSysType,iMajVer,iMinVer,iSPack;
  
  // What version of Windows are we running?
  iRes=GET_WIN_VERSION(&iSysType,&iMajVer,&iMinVer,&iSPack);
  if(iRes != 0) return iRes;
  
  // If Windows 95 or 98 or ME, use EnumPrinters...
  if (iSysType==2 || iSysType==3 || iSysType==4)
  {
    // The first EnumPrinters() tells you how big our buffer should
    // be in order to hold ALL of PRINTER_INFO_2. Note that this will
    // usually return FALSE. This only means that the buffer (the 4th
    // parameter) was not filled in. You don't want it filled in here...
	  ::EnumPrinters(PRINTER_ENUM_DEFAULT, NULL, 2, NULL, 0, &dwNeeded,
		&dwReturned);
    if (dwNeeded == 0) 
      return 635;
    
    // Allocate enough space for PRINTER_INFO_2...
    ppi2 = (PRINTER_INFO_2 *)::GlobalAlloc(GPTR, dwNeeded);
    if (!ppi2)
      return 634;
    
    // The second EnumPrinters() will fill in all the current information...
    bFlag = ::EnumPrinters(PRINTER_ENUM_DEFAULT, NULL, 2, (LPBYTE)ppi2,
		dwNeeded, &dwNeeded, &dwReturned);
    if (!bFlag)
    {
		::GlobalFree(ppi2);
        return 635;
    }
    
    // If given buffer too small, set required size and fail...
    if ((int)lstrlen(ppi2->pPrinterName) >= *iBufferSize)
    {
      *iBufferSize = (int)lstrlen(ppi2->pPrinterName) + 1;
      ::GlobalFree(ppi2);
      return 121;
    }
    
    // Copy printer name into passed-in buffer...
    lstrcpy(szPName, ppi2->pPrinterName);
    
    // Set buffer size parameter to min required buffer size...
    *iBufferSize = (int)lstrlen(ppi2->pPrinterName) + 1;
  }
  
  // If Windows NT, use the GetDefaultPrinter API for Windows 2000,
  // or GetProfileString for version 4.0 and earlier...
  if (iSysType==6)  // Win/2000 or later
  {
	  dwTemp=(DWORD)*iBufferSize;
      bFlag = ::GetDefaultPrinter(szPName, &dwTemp);
      if (!bFlag)
	  {
		  iRes=(int)::GetLastError();
		  if(iRes==ERROR_INSUFFICIENT_BUFFER) return 121;
		  if(iRes==ERROR_FILE_NOT_FOUND) return 633;
          return iRes;
	  }
	  *iBufferSize=(int)dwTemp;
  }
    
  if(iSysType==5)  // NT4.0 or earlier
  {
      // Retrieve the default string from Win.ini (the registry).
      // String will be in form "printername,drivername,portname".
      if (::GetProfileString("windows", "device", ",,,", cBuffer,
		  MAXPRINTERBUFFERSIZE) <= 0)
        return FALSE;
      
      // Printer name precedes first "," character...
      strtok(cBuffer, ",");
      
      // If given buffer too small, set required size and fail...
      if ((int)lstrlen(cBuffer) >= *iBufferSize)
      {
        *iBufferSize = (int)lstrlen(cBuffer) + 1;
        return 121;
      }
      
      // Copy printer name into passed-in buffer...
      lstrcpy(szPName, cBuffer);
      
      // Set buffer size parameter to min required buffer size...
      *iBufferSize = (int)lstrlen(cBuffer) + 1;
  }
  
  // Cleanup...
  if (ppi2)
	  ::GlobalFree(ppi2);
  
  return 0;
}

int GET_WIN_VERSION(int *iSysType,int *iVerMajor,
					int *iVerMinor,int *iPack)
// Created: 1/4/2001  (RK)
// Last modified: 8/27/2003  (RK)
// Get the system version number and service pack number
// This works for Win/95/98/ME and also Win/NT/2000/XP
// Return codes are as follows:
//   0   = Normal termination
//   606 = Unable to identify system type
// iSysType has the following values:
//   0=Unknown, 1=Win3.1, 2=Win/95, 3=Win/98 4=Win/ME, 5=Win/NT 6=Win/2000 or later
// iVerMajor is (e.g.) 4 for Win/NT, 5 for Win/2000 or later
// iVerMinor is (e.g.) 51 for NT v3.51, 0 for NT v 4.00
// iPack (service pack #) is meaningful for NT or 2000 only
{
	BOOL bResult;
	OSVERSIONINFO osvi;
	char szPack[4]="-1";

	// First check what version of Windows we're in
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    bResult=::GetVersionEx(&osvi);
	if(!bResult)     // Unable to identify system version
	    return 606;

	// The system Type (1=Win3.1, 2=Win95, 3=Win98, 4=Win/ME,
	//  5=Win/NT, 6=Win/2000 or later)
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32s)
        *iSysType=1;
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		*iSysType=2;
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
        *iSysType=5;

	// Major version number
	*iVerMajor=osvi.dwMajorVersion;
	if(*iVerMajor >= 5) *iSysType=6;

	// Minor version number
	*iVerMinor=osvi.dwMinorVersion;

	// For Win/98, minor version is greater than zero
	if((*iSysType==2) && (*iVerMinor>0)) *iSysType=3;
	// For Win/ME, minor version is 90 or greater
	if((*iSysType==3) && (*iVerMinor>=90)) *iSysType=4;

	// Service Pack info
	// For NT/2000 only, extract the service pack as a number
	*iPack=-1;
	if((*iSysType>=5) && (strcmp(osvi.szCSDVersion,"Service Pack")))
	{
       szPack[0]=osvi.szCSDVersion[12];
       szPack[1]=osvi.szCSDVersion[13];
       szPack[2]=osvi.szCSDVersion[14];
	   *iPack=atoi(szPack);
	}

	return 0;
}

#else
#include  "linwtypes.h"
#endif