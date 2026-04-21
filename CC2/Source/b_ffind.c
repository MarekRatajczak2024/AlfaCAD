#define _DEFAULT_SOURCE
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

#ifdef LINUX
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

//#include "leak_detector_c.h"

#define DEBUG 1

static int findfile_recursive_exe(char *folder,const char *filename, char *fullpath)
{
    DIR *dir;
    char *slash = "";

    int ret = 1;

    struct dirent *entry;
//checking if it failed to open and report errors to STDERR
    if((dir = opendir(folder)) == NULL){
        fprintf(stderr,"opendir: %s\n",strerror(errno));
        return 0;
    }

    while ((entry = readdir(dir)))
    {

//if is . or .. we continue to prevent winging back and forth

        if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0)
            continue;

//we check if the path has already a / if not we add one

        int length = strlen(folder);

        if(folder[length-1] != '/')
        {
            slash = "/";
        }

        length += strlen(entry->d_name)+2;
        char *newpath = malloc(length);
        if (!newpath)
        {
            fprintf(stderr,"malloc: %s\n",strerror(errno));
            break;
        }

        snprintf(newpath,length,"%s%s%s",folder,slash,entry->d_name);

        if(strcmp(entry->d_name,filename) ==0)
        {
#if DEBUG
            printf("Was found here %s\n",newpath);
#endif
            sprintf(fullpath,"%s",newpath);
            ret =  strlen(fullpath);
            break;
        }
        //checking if is a directory to do a recursive call
        // using DT_DIR to avoid the use of lstat or stat
        // if not directory we free the memory and move on
        if(entry->d_type == DT_DIR)
            findfile_recursive_exe(newpath,filename,fullpath);
        else
        {
            free(newpath);
            continue;
        }

        free(newpath);
    }
    if(closedir(dir) != 0){
        fprintf(stderr,"closedir: %s\n",strerror(errno));
        return 0;
    }

    return ret;
}

int findfile_recursive(char *folder,const char *filename, char *fullpath)
{
    return findfile_recursive_exe(folder, filename, fullpath);
}
#else
#include <stdio.h>
#include <Windows.h>

int findfile_recursive(const char *folder, const char *filename, char *fullpath)
{
    char wildcard[MAX_PATH];
    sprintf(wildcard, "%s\\*", folder);
    WIN32_FIND_DATA fd;
    HANDLE handle = FindFirstFile(wildcard, &fd);
    if(handle == INVALID_HANDLE_VALUE) return 0;
    do
    {
        if(strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
            continue;
        char path[MAX_PATH];
        sprintf(path, "%s\\%s", folder, fd.cFileName);

        if(_stricmp(fd.cFileName, filename) == 0)
            strcpy(fullpath, path);
        else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            findfile_recursive(path, filename, fullpath);
        if(strlen(fullpath))
            break;
    } while(FindNextFile(handle, &fd));
    FindClose(handle);
    return strlen(fullpath);
}
#endif
