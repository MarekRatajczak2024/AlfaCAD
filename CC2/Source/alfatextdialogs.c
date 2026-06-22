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
//
// Created by marek on 4/26/25.
//
#define _POSIX_C_SOURCE 200809L // Enables POSIX functions like popen
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#ifdef LINUX
//#include <ncurses.h>
#endif
#ifdef _WIN32
#ifdef __BORLANDC__
  #define _getch getch
 #endif
 #ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0500
 #endif
 #include <windows.h>
 #include <commdlg.h>
 #include <shlobj.h>
 #include <conio.h>
 #include <direct.h>
 #define TINYFD_NOCCSUNICODE
 #define SLASH "\\"
#else
#include <limits.h>
#include <unistd.h>
#include <dirent.h> /* on old systems try <sys/dir.h> instead */
#include <termios.h>
#include <sys/utsname.h>
#include <signal.h> /* on old systems try <sys/signal.h> instead */
#define SLASH "/"
#endif /* _WIN32 */

#include <mouse.h>
#include "alfversion.h"

#define MAX_PATH_OR_CMD 1024 /* _MAX_PATH or MAX_PATH */
#define MAX_TEXT_SIZE 32000

#ifdef LINUX
typedef int BOOL;
#endif

#define MAXPATH   260

extern int Client_number;

extern BOOL RETINA;

extern char _EDIT_TEXT_;  // NOLINT
extern char _EDIT_FILE_;  // NOLINT

extern int getmaxx(void);
extern int getmaxy(void);
extern char editbox_geometry_text[32];   //{"640x400+400+250"};
extern char editbox_geometry_line[32];   //{"800x80+50+50"};
extern char editbox_geometry_file[32];
static char *editbox_geometry;
extern void set_geometry(int single);
extern int get_editbox_geometry_set(void);
extern int get_editbox_geometry_file_set(void);
extern int get_editbox_geometry_line_set(void);
extern char default_path_TTF[MAXPATH];
extern char default_path_OTF[MAXPATH];

extern int get_window_origin_and_size(int* x_win_orig, int* y_win_orig, int* win_width, int* win_height);
extern void get_editbox_size_origin(int *w, int *h, int *x, int *y);
extern void get_editbox_size_origin_file(int *w, int *h, int *x, int *y);
extern void get_editbox_size_origin_line(int *w, int *h, int *x, int *y);
extern int getlanguage(void);
#ifdef ALLEGRO5
extern void position_mouse(int x, int y);
extern void position_mouse_(int x, int y);
extern void position_mouse_xy(int x, int y);
#endif
extern void get_mouse_mickeys(int *WspX, int *WspY_);
extern void move_pointer(int x, int y);

typedef unsigned long DWORD;

extern DWORD SystemSilent(char * strFunct, char *strstrParams);
extern DWORD SystemSilentS(char *strstrParams);
extern DWORD RunSilent(char * strFunct, char *strstrParams);
extern void free_mouse(void);
extern void lock_mouse(void);

extern int edit_text_flag;
extern void set_forget_mouse(int exp_x, int exp_y);

#ifndef _WIN32

static int fileExists( char const * aFilePathAndName )
{
    FILE * lIn ;
    if ( ! aFilePathAndName || ! strlen(aFilePathAndName) )
    {
        return 0 ;
    }
    lIn = fopen( aFilePathAndName , "r" ) ;
    if ( ! lIn )
    {
        return 0 ;
    }
    fclose( lIn ) ;
    return 1 ;
}

#endif


static void wipefile(char const * aFilename)
{
    int i;
    struct stat st;
    FILE * lIn;

    if (stat(aFilename, &st) == 0)
    {
        if ((lIn = fopen(aFilename, "w")))
        {
            for (i = 0; i < st.st_size; i++)
            {
                fputc('A', lIn);
            }
            fclose(lIn);
        }
    }
}


char * alfa_editBox(
        char const * aTitle ,
        char const * aMessage ,
        int const * aDefaultInput,
        char const *fontface,
        char const *fontfile,
        char const *fontsize,
        char const *fontwidth,
        char const *etype,
        char const *params,
        int *single
)
{
#define MAXPARAMSLEN 255
    static char lBuff[MAX_TEXT_SIZE]="";
    static char lBuff_ex[MAX_TEXT_SIZE]="";
    char * lDialogString = NULL;
    FILE * lIn=NULL , *lInt;
    int lResult ;
    size_t lTitleLen ;
    size_t lMessageLen;
    int inTotal;
    unsigned long chunk;
    char *fontfile_p;

    int w,h,x,y;
    int x0m, y0m, dxm, dym;
    int ret;
    int fontsize_r, fontwidth_r;
    int lang;
    char *lang_str[]={"EN","PL","UA","ES"};

    lTitleLen =  aTitle ? strlen(aTitle) : 0 ;
    lMessageLen =  aMessage ? strlen(aMessage) : 0 ;
    lDialogString = (char *) malloc( MAXPARAMSLEN + lTitleLen + lMessageLen );

#ifdef MACOS
    fontfile_p=strrchr(fontfile,'/');
    if (fontfile_p==NULL) fontfile_p=fontfile;
    else fontfile_p++;
#else
    fontfile_p=(char*)fontfile;
#endif

    if ((strcmp(etype,"TEXT")==0) ||  (strcmp(etype,"INFO")==0))
    {
        if (*single == 1)
        {
            if (!get_editbox_geometry_line_set()) set_geometry(1);
            editbox_geometry = (char *) &editbox_geometry_line;
            get_editbox_size_origin_line(&w, &h, &x, &y);
        }
        else
        {
            if (!get_editbox_geometry_set()) set_geometry(0);
            editbox_geometry = (char *) &editbox_geometry_text;
            get_editbox_size_origin(&w, &h, &x, &y);
        }
    }
    else
    {
        if (!get_editbox_geometry_file_set()) set_geometry(2);
        editbox_geometry = (char *) &editbox_geometry_file;
        get_editbox_size_origin_file(&w, &h, &x, &y);
    }

    fontsize_r=(atoi(fontsize))/(RETINA+1);
    fontwidth_r=(atoi(fontwidth))/(RETINA+1);
    lang=getlanguage();

    sprintf(lBuff,"'%s' \"%s\" %d '%s' '%s' '%d' '%d' '%s' '%s' %d '%s' '%s' '%s' '%s' %d",aTitle, aMessage, 0, fontface, fontfile_p, fontsize_r, fontwidth_r, etype, params, *single, editbox_geometry, default_path_TTF, default_path_OTF, lang_str[lang], Client_number);

    printf("%s\n",lBuff);

    ret=get_window_origin_and_size(&x0m, &y0m, &dxm, &dym);
#ifdef ALLEGRO5
    position_mouse_xy(x+w-x0m, y+h-y0m);
#else
    position_mouse(x+w-x0m, y+h-y0m);
#endif
    set_forget_mouse(x+w-x0m, y+h-y0m);

#ifndef MACOS
    enable_hardware_cursor();
    free_mouse();
#endif
    show_os_cursor(MOUSE_CURSOR_ARROW);

    lBuff_ex[0]='\0';

    // ==================================================================
    // CROSS-PLATFORM ADAPTIVE RUNTIME ROUTING (The Master Logic Unification)
    // ==================================================================
    if (strstr(aTitle, &_EDIT_FILE_) != NULL)
    {
        // 1. HEAVY FILE PATH: Redirects status flag safely to /tmp/text.out or uses standard pipe tracking
#ifndef MACOS
        edit_text_flag=1;
#ifndef NOPYTHON
        sprintf(lDialogString, "%s alfamtext.py %s", PYTHON, lBuff);
#else
        sprintf(lDialogString, "GDK_BACKEND=x11 ./alfamtext %s", lBuff);   ////compiled
#endif
        lIn = popen(lDialogString, "r");
#else
        edit_text_flag=1;
#ifndef NOPYTHON
        sprintf(lDialogString, "%s alfamtext.py %s", PYTHON, lBuff);    ///testing option with Python
#else
        sprintf(lDialogString, "./alfamtext %s", lBuff);   ////compiled
#endif
        lIn = popen(lDialogString, "r");
#endif

    }
    else
    {
        // 2. FAST TEXT PATH: Streams drawing annotations entirely in RAM via popen pipes
#ifndef MACOS
        edit_text_flag=1;
#ifndef NOPYTHON
        sprintf(lDialogString, "%s alfamtext.py %s", PYTHON, lBuff);
#else
        sprintf(lDialogString, "GDK_BACKEND=x11 ./alfamtext %s", lBuff);   ////compiled
#endif
        lIn = popen(lDialogString, "r");
#else
        edit_text_flag=1;
#ifndef NOPYTHON
        sprintf(lDialogString, "%s alfamtext.py %s", PYTHON, lBuff);   ///testing option with Python
#else
        sprintf(lDialogString, "./alfamtext %s", lBuff);    ////compiled
#endif
        lIn = popen(lDialogString, "r");
#endif
    }

    if (!lIn)
    {
        free(lDialogString);
        return NULL;
    }

    // Single-Pass status flag / data stream fetch
    if (fgets(lBuff_ex, sizeof(lBuff_ex), lIn) != NULL) {
        size_t len = strlen(lBuff_ex);
        if (len > 0 && lBuff_ex[len - 1] == '\n') lBuff_ex[len - 1] = '\0';
    }

    // Hardware mouse state restoration blocks
#ifndef MACOS
    lock_mouse();
    show_os_cursor(MOUSE_CURSOR_NONE);
    disable_hardware_cursor();
#else
    show_os_cursor(MOUSE_CURSOR_NONE);
    edit_text_flag=0;
#endif
    // Close the runtime stream handles safely based on what was opened

    pclose(lIn);

    // ==================================================================
    // DATA STRIP AND DECODING LAYER
    // ==================================================================
    if (strstr(aTitle, &_EDIT_FILE_) == NULL)
    {
        // ------------------ STANDARD DRAWING TEXT TRACK ------------------
        // Linux pipe path delivers text data right inside lBuff_ex
        strcpy(lBuff, lBuff_ex + 1);
        if (lBuff_ex[0] == '1') lResult = 1;
        else lResult = 0;

        // Unpack character sequences back into native raw line breaks (ASCII 10)
        char *text_ptr = lBuff;
        while ((text_ptr = strstr(text_ptr, "\\n")) != NULL) {
            text_ptr[0] = 10;
            memmove(text_ptr + 1, text_ptr + 2, strlen(text_ptr + 2) + 1);
            text_ptr++;
        }
    }
    else
    {
        // ------------------ BULK CONFIGURATION FILE TRACK ------------------
        if (lBuff_ex[0] == '1')
        {
            lResult = 1;

            // RESTORED: Both Linux and macOS read bulk contents from /tmp/tinyfdt.txt!
            lInt = fopen("/tmp/tinyfdt.txt", "r");
            inTotal = 0;
            if (lInt != NULL)
            {
                while ((chunk = fread(lBuff + inTotal, 1, sizeof(lBuff) - inTotal - 1, lInt)) != 0)
                {
                    inTotal += chunk;
                }
                lBuff[inTotal] = '\0';

                if (inTotal > 0 && lBuff[inTotal - 1] == '\n') lBuff[inTotal - 1] = '\0';
                fclose(lInt);
            }
            else
            {
                strcpy(lBuff, "");
            }
        }
        else
        {
            strcpy(lBuff, "");
            lResult = 0;
        }
    }

    // Common Parameter & Geometry processing block
    char *pos1D = strchr(lBuff, '\x1D');
    if (pos1D != NULL)
    {
        char *pos_p = pos1D + 1;
        char *pos2t = strstr(pos_p, ">>");
        if (pos2t != NULL)
        {
            *single = -1;
            free(lDialogString);
            return NULL;
        }
        if (pos_p[0] == '0')
        {
            free(lDialogString);
            return NULL;
        }

        char *pos_g = strchr(pos_p, ',');
        if (pos_g != NULL)
        {
            if ((strcmp(etype, "TEXT") == 0) || (strcmp(etype, "INFO") == 0))
            {
                if (*single == 1)
                {
                    strcpy(editbox_geometry_line, pos_g + 1);
                    char *pos_c = strchr(editbox_geometry_line, '>');
                    if (pos_c != NULL) *pos_c = '\0';
                }
                else
                {
                    strcpy(editbox_geometry_text, pos_g + 1);
                    char *pos_c = strchr(editbox_geometry_text, '>');
                    if (pos_c != NULL) *pos_c = '\0';
                }
            }
            else
            {
                strcpy(editbox_geometry_file, pos_g + 1);
                char *pos_c = strchr(editbox_geometry_file, '>');
                if (pos_c != NULL) *pos_c = '\0';
            }
            *pos_g = '\0';
        }
        strcpy((char*)params, pos1D + 1);
        *pos1D = '\0';
    }

    if (strlen(lBuff) > 0 && lBuff[strlen(lBuff) - 1] == '\n')
    {
        lBuff[strlen(lBuff) - 1] = '\0';
    }

    // Wipe scratchpad resources cleanly
    if (fileExists("/tmp/tinyfdt.txt")) remove("/tmp/tinyfdt.txt");

    if (!lResult)
    {
        free(lDialogString);
        return NULL;
    }

    free(lDialogString);
    return lBuff;
}
