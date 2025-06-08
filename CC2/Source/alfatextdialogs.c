/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Mouse input routines.
 *
 *      By Shawn Hargreaves.
 *
 *      Mark Wodrich added double-buffered drawing of the mouse pointer and
 *      the set_mouse_sprite_focus() function.
 *
 *      See readme.txt for copyright information.
 */
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

extern BOOL RETINA;

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
    //static char pparams[MAXPARAMSLEN];
    static char lBuff[MAX_TEXT_SIZE]="";   //MAX_TEXT_SIZE  MAX_PATH_OR_CMD
    static char lBuff_ex[MAX_TEXT_SIZE]="";  //MAX_TEXT_SIZE
    char * lDialogString = NULL;
    char * lpDialogString;
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

    int WspX_, WspY_;
    int lang;

    char *lang_str[]={"EN","PL","UA","ES"};

    lTitleLen =  aTitle ? strlen(aTitle) : 0 ;
    lMessageLen =  aMessage ? strlen(aMessage) : 0 ;
    //if ( !aTitle || strcmp(aTitle,"tinyfd_query") )
    //{
    //lDialogString = (char *) malloc( MAX_PATH_OR_CMD + lTitleLen + lMessageLen );
    lDialogString = (char *) malloc( MAXPARAMSLEN + lTitleLen + lMessageLen );
    //}

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
            if (!get_editbox_geometry_line_set())
                set_geometry(1);
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

    ////sprintf(editbox_geometry_r,"%dx%d+%d+%d",w,h,x/(RETINA+1),y/(RETINA+1));
    lang=getlanguage();

    sprintf(lBuff,"'%s' \"%s\" %d '%s' '%s' '%d' '%d' '%s' '%s' %d '%s' '%s' '%s' '%s'",aTitle, aMessage, 0, fontface, fontfile_p, fontsize_r, fontwidth_r, etype, params, *single, editbox_geometry /*editbox_geometry_r*/, default_path_TTF, default_path_OTF, lang_str[lang]);
    //printf("%s\n",lBuff);

    ret=get_window_origin_and_size(&x0m, &y0m, &dxm, &dym);
    position_mouse_xy(x+w-x0m, y+h-y0m);

    set_forget_mouse(x+w-x0m, y+h-y0m);

    //move_pointer(x+w, y+h);

    /*
    do {
        get_mouse_mickeys(&WspX_, &WspY_);
    } while ((WspX_!=0) || (WspY_!=0));
     */
    //sprintf(lDialogString,"szAnswer=$(python ../AlfaText-master/alfamtext.py %s >/tmp/tinyfdt.txt);if [ $? = 0 ];then echo 1$szAnswer;else echo 0$szAnswer;fi",pparams);
    //sprintf(lDialogString,"python ../AlfaText-master/alfamtext.py %s",pparams);
    //sprintf(lDialogString,"%s alfamtext.py %s",PYTHON,lBuff);

    edit_text_flag=1;
#ifndef MACOS
    enable_hardware_cursor();
    free_mouse();
#endif
    show_os_cursor(MOUSE_CURSOR_ARROW);
#ifndef MACOS
    sprintf(lDialogString,"%s alfamtext.py %s",PYTHON,lBuff);
    lIn = popen( lDialogString , "r" );
#else
    edit_text_flag=1;
    //sprintf(lDialogString,"%s alfamtext.py %s",PYTHON,lBuff);
    sprintf(lDialogString,"./alfamtext %s",lBuff); //TO DO WITHOUT PYTHON
    strcat(lDialogString," > text.out");
    DWORD retD=SystemSilentS(lDialogString);
    edit_text_flag=0;
    lIn=fopen("text.out", "r");
#endif
    //show_os_cursor(MOUSE_CURSOR_NONE);  //???????
    //disable_hardware_cursor();  //????????
    if ( ! lIn  )
    {
        if ( fileExists("/tmp/tinyfd.txt") )
        {
            wipefile("/tmp/tinyfd.txt");
            remove("/tmp/tinyfd.txt");
        }
        if ( fileExists("/tmp/tinyfd0.txt") )
        {
            wipefile("/tmp/tinyfd0.txt");
            remove("/tmp/tinyfd0.txt");
        }
        free(lDialogString);
        return NULL ;
    }

#ifndef MACOS
    while ( fgets( lBuff_ex , sizeof( lBuff_ex ) , lIn ) != NULL ) {sleep(0); continue;}

    ////position_mouse(getmaxx()/2, getmaxy()/2);
    ////set_forget_mouse(getmaxx()/2, getmaxy()/2);

    lock_mouse();
    show_os_cursor(MOUSE_CURSOR_NONE);
    disable_hardware_cursor();
#else
    show_os_cursor(MOUSE_CURSOR_NONE);  //???????
    fgets( lBuff_ex, sizeof( lBuff_ex), lIn);
#endif
    //printf(lBuff_ex);
#ifndef MACOS
    pclose( lIn );
#else
    fclose( lIn );
#endif


    if (*single==0)
    {
        if (lBuff_ex[0] == '1')
        {
            lResult = 1;

            lInt = fopen("/tmp/tinyfdt.txt", "r");

            inTotal = 0;
            if (lInt != NULL)
            {
                while ((chunk = fread(lBuff, 1, sizeof(lBuff), lInt)) != 0)
                {
                    inTotal += chunk;
                }
                lBuff[inTotal] = '\0';
                if (lBuff[inTotal - 1] == '\n') lBuff[inTotal - 1] = '\0';
                fclose(lInt);
            } else strcpy(lBuff, "");
        } else
        {
            strcpy(lBuff, "");
            lResult = 0;
        }
    }
    else
    {
    strcpy(lBuff, lBuff_ex+1);

    if (lBuff_ex[0] == '1') lResult = 1;
    else lResult = 0;
    }
    //finding params and geometry
    char *pos1D=strchr(lBuff, '\x1D');
    if (pos1D!=NULL)
    {
        char *pos_p=pos1D+1;

        char *pos2t=strstr(pos_p, ">>");
        if (pos2t!=NULL)
        {
        *single=-1;
        free(lDialogString);
        return NULL ;
        }

        if (pos_p[0]=='0')
        {
            free(lDialogString);
            return NULL ;
        }

        char *pos_g=strchr(pos_p, ',');
        if (pos_g!=NULL)
        {
            if ((strcmp(etype,"TEXT")==0) ||  (strcmp(etype,"INFO")==0))  //cut cursor position
            {
            if (*single==1)
                {
                strcpy(editbox_geometry_line, pos_g + 1);
                char *pos_c = strchr(editbox_geometry_line, '>');
                if (pos_c!=NULL) *pos_c='\0';
                }
            else
                {
                    strcpy(editbox_geometry_text, pos_g + 1);
                    char *pos_c = strchr(editbox_geometry_text, '>');
                    if (pos_c!=NULL) *pos_c='\0';
                }
            }
            else
            {
                strcpy(editbox_geometry_file, pos_g + 1);
                char *pos_c = strchr(editbox_geometry_file, '>');
                if (pos_c!=NULL) *pos_c='\0';
            }
            *pos_g='\0';
        }

        strcpy((char*)params, pos1D+1);
        *pos1D='\0';
    }

    if ( lBuff[strlen( lBuff ) -1] == '\n' )
    {
        lBuff[strlen( lBuff ) -1] = '\0' ;
    }

    if ( fileExists("/tmp/tinyfdt.txt") )
    {
        wipefile("/tmp/tinyfdt.txt");
        remove("/tmp/tinyfdt.txt");
    }

    if ( fileExists("/tmp/tinyfd.txt") )
    {
        wipefile("/tmp/tinyfd.txt");
        remove("/tmp/tinyfd.txt");
    }
    if ( fileExists("/tmp/tinyfd0.txt") )
    {
        wipefile("/tmp/tinyfd0.txt");
        remove("/tmp/tinyfd0.txt");
    }

    if ( ! lResult )
    {
        free(lDialogString);
        return NULL ;
    }

    free(lDialogString);
    //printf( "lBuff: %s\n" , lBuff ) ;
    return lBuff;
}