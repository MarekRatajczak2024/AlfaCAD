//
// Created by Marek on 13/06/2026.
//

#ifndef O_PRINTERWARMER_H
#define O_PRINTERWARMER_H

#pragma once
#include <windows.h>

// Launches the low-priority background thread to pre-heat the network printer driver.
// This function returns immediately (in less than a millisecond) so AlphaCAD doesn't freeze.
void StartPrinterWarmerThread();

// THE SAFETY VALVE: Check if the thread is done.
// Returns TRUE if the printer driver is fully loaded and safe to use.
BOOL IsPrinterWarmedUp();

#endif //O_PRINTERWARMER_H
