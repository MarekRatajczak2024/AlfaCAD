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

#include <forwin.h>
#ifndef LINUX
#include <conio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#else
#include <stdio.h>
////// added for atoi_ atol_
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
//////
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_libfun.h"

static int i__errorno = NO_ERR ;

extern int my_kbhit(void);
extern int my_getch(void);


#ifdef __cplusplus
extern "C" {
#endif

int atoi_(const char *st)
{
    char *endptr;

    errno = 0; // CRITICAL: Reset errno before the call
    long val = strtol(st, &endptr, 10);

    // 1. Check for strtol range errors or no conversion
    if (errno == ERANGE || endptr == st) return 0;

    // 2. Check if the parsed long fits into a standard int
    if (val > INT_MAX || val < INT_MIN) return 0;

    // 3. Skip trailing whitespace (optional but highly recommended)
    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    // 4. Reject if trailing non-whitespace characters remain
    if (*endptr != '\0') return 0;

    return (int)val;
}

long atol_(const char *st)
{
    char *endptr;

    errno = 0; // CRITICAL: Reset errno before the call
    long val = strtol(st, &endptr, 10);

    // 1. Check for strtol range errors or no conversion
    if (errno == ERANGE || endptr == st) return 0;

    // 2. Skip trailing whitespace
    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    // 3. Reject if trailing non-whitespace characters remain
    if (*endptr != '\0') return 0;

    return val;
}


#ifdef LINUX
/* reads from keypress, doesn't echo */
int _getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}
/* reads from keypress, echoes */
int _getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

BOOL  kbhit()
{
    struct termios term;
    tcgetattr(0, &term);

    struct termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

int _kbhit() {
    static const int STDIN = 0;
    static BOOL initialized = FALSE;

    if (! initialized) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = TRUE;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

#endif

BOOL Check_Attribute (int i_attribute, int i_check)
/*-----------------------------------------------*/
{
  BOOL b_ret ;

  b_ret = FALSE ;
  if (i_attribute == i_check)
  {
    b_ret = TRUE ;
  }
  else
  if (i_check == Anormalny && i_attribute == Aoblok)
  {
    b_ret = TRUE ;
  }
  else
  if (i_check == Anormalny && i_attribute == Apoblok)
  {
    b_ret = TRUE ;
  }
  else
  if (i_check == Anormalny && i_attribute == Appoblok)
  {
    b_ret = TRUE ;
  }
  else
  if (i_check == Anormalny && i_attribute == Apppoblok)
  {
    b_ret = TRUE ;
  }
  else
  if (i_check == ANieOkreslony &&
     (i_attribute != Ausuniety && i_attribute != Abad))
  {
    b_ret = TRUE ;
  }
  return b_ret ;
}


int Get_Error_No (void)
/*--------------------*/
{
  int i_ret ;
  i_ret = i__errorno ;
  i__errorno = NO_ERR ;
  return i_ret ;
}

void Set_Error_No (int i_errno)
/*--------------------------*/
{
  i__errorno = i_errno ;
}


BOOL Check_Break_Key (void) {
    if (my_kbhit() && my_getch() == ESC) {
        while (my_kbhit()) {
            my_getch();
        }
        return TRUE;
    }
    return FALSE;
}


BOOL Check_Break_Key_ (void)
/*----------------------*/
{
  if (_kbhit () && _getch () == ESC)
  {
    while (_kbhit ())
    {
       _getch () ;
    }
     return TRUE ;
  }
  return FALSE ;
}

void  SetBit( int A[],  int k )
{
    A[k/32] |= 1 << (k%32);  // Set the bit at the k-th position in A[i]
}

void  ClearBit( int A[],  int k )
{
    A[k/32] &= ~(1 << (k%32));
}

int TestBit( int A[],  int k )
{
    int res=( (A[k/32] & (1 << (k%32) )) != 0 ) ;
    return res;
}
#ifdef __cplusplus
}
#endif

