#include "o_printerwarmer.h"
#include <winspool.h>
#include <process.h>

#pragma comment(lib, "winspool.lib")

static volatile LONG g_bPrinterReady = 0;

// ======================================================================
// UNIVERSAL COMPILER ARCHITECTURE MATCHING
// We use _M_X64 to strictly verify the machine target, forcing 
// Visual Studio to apply the correct silent track regardless of IDE state.
// ======================================================================
#if defined(_M_X64) || defined(_WIN64) || defined(__amd64__)

unsigned __stdcall PrinterWarmerWorker(void* pArguments)
{
    ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_LOWEST);

    char szPrinterName[256] = { 0 };
    DWORD dwBufferSize = sizeof(szPrinterName);

    if (GetDefaultPrinterA(szPrinterName, &dwBufferSize))
    {
        // On 64-bit targets, passing NULL is completely silent, 
        // as long as the compiler isn't being tricked into running the 32-bit block!
        HDC hWarmUpDC = ::CreateDCA("WINSPOOL", szPrinterName, NULL, NULL);
        if (hWarmUpDC != NULL)
        {
            ::DeleteDC(hWarmUpDC);
        }
    }
    ::InterlockedExchange(&g_bPrinterReady, 1);
    _endthreadex(0);
    return 0;
}

#else

unsigned __stdcall PrinterWarmerWorker(void* pArguments)
{
    ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_LOWEST);

    char szPrinterName[256] = { 0 };
    DWORD dwBufferSize = sizeof(szPrinterName);

    HDC hDC = NULL;

    if (GetDefaultPrinterA(szPrinterName, &dwBufferSize))
    {
        // On legacy 32-bit targets, we pass the dummy structure to force
        // the spooler to process the network connection in total silence.
        DEVMODEA dmDummy = { 0 };
        dmDummy.dmSize = sizeof(DEVMODEA);
        dmDummy.dmFields = DM_ORIENTATION;
        dmDummy.dmOrientation = DMORIENT_PORTRAIT;
       

        HDC hWarmUpDC = ::CreateDCA("WINSPOOL", szPrinterName, NULL, &dmDummy);
        if (hWarmUpDC != NULL)
        {
            ::DeleteDC(hWarmUpDC);
        }
    }

    ::InterlockedExchange(&g_bPrinterReady, 1);
    _endthreadex(0);
    return 0;
}

#endif


BOOL IsPrinterWarmedUp()
{
    return (::InterlockedOr(&g_bPrinterReady, 0) == 1);
}

////#define WARMING32TOO

#ifdef WARMING32TOO
void StartPrinterWarmerThread()
{
    ::InterlockedExchange(&g_bPrinterReady, 0);

    unsigned threadID;
    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &PrinterWarmerWorker, NULL, 0, &threadID);
    if (hThread != NULL)
    {
        ::CloseHandle(hThread);
    }
}
#else
////if we want to resign from worming printer in 32 bit
void StartPrinterWarmerThread()
{
#ifdef _WIN64
    // ------------------------------------------------======================
    // MODERN 64-BIT PATH: Priming the driver works silently in the background
    // ----------------------------------------------------------------======
    ::InterlockedExchange(&g_bPrinterReady, 0);

    unsigned threadID;
    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &PrinterWarmerWorker, NULL, 0, &threadID);
    if (hThread != NULL)
    {
        ::CloseHandle(hThread);
    }
#else
    // ----------------------------------------------------------------======
    // LEGACY 32-BIT PATH: Skip the thread to stop the annoying popup window!
    // ----------------------------------------------------------------======
    // Set the flag to 1 instantly. This tells AlfaCAD's print synchronization
    // gate that it can skip the wait loop entirely and continue running at full speed.
    ::InterlockedExchange(&g_bPrinterReady, 1);
#endif
}
#endif

