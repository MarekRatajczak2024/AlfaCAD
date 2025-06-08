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
#include <dos.h>
#include <io.h>
#include <direct.h> //needed by CLion
#else
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <values.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_lfile.h"

#include "leak_detector_c.h"

#ifndef LINUX
#define Slash  '\\'
#define BSlash '\\'
#define SSlash "\\"
#else
#define Slash  '/'
#define BSlash '/'
#define SSlash "/"
#endif

extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
extern int	getdisk(void);
extern int	setdisk(int _drive);
extern void fnmerge (char *path, const char *drive, const char *dir, const char *name, const char *ext);
extern char *strlwr2(char *_s);

BOOL Copy_File_Nb(FILE* fdest, FILE* fsrc, long nbyte);

BOOL Delete_File(char *ptrsz_fnd)
{
	BOOL b_ret;

	b_ret = TRUE;

	int ret=unlink(ptrsz_fnd);
	return  b_ret;
}

BOOL get_dest_image_file_name(char *source, char *destination, int i_pageno, char *image_format)
{
	int k;
	char i_str[12];
	char  fnd[MAXPATH], drive[MAXDRIVE], dir[MAXDIR], name[MAXFILE], ext[MAXEXT];
	char img_format[12];
	char *img_ptr_format;

	strcpy(img_format, image_format);
	img_ptr_format = strlwr2(img_format);

	k = fnsplit(source, drive, dir, name, ext);
	if (i_pageno > 0)
	{
	   sprintf(i_str, "%03d", i_pageno);
	   strcat(name, i_str);
    }
	fnmerge(destination, drive, dir, name, img_ptr_format);
	return 1;
}

BOOL Copy_File0 (char *ptrsz_fnd, char *ptrsz_fns)
//------------------------------------------------
{
  BOOL b_ret ;
  FILE* fs;
  FILE* fd;



  b_ret = TRUE ;
  int ret=unlink (ptrsz_fnd) ;
  fs = fopen(ptrsz_fns, "rb");
  fd = fopen(ptrsz_fnd, "wb");
  if (fs == NULL || fd == NULL || FALSE == Copy_File_Nb (fd, fs, -1l))
  {
    b_ret = FALSE ;
  }
  if (fs != NULL) fclose (fs) ; 
  if (fd != NULL) fclose (fd) ;
  return b_ret ;
}


char *Set_Full_File_Name (char *ptrsz_dest, char *ptrsz_src)
//------------------------------------------------------------
{
  int flags;
  char drive [MAXDRIVE]="", dir [MAXDIR]="", name [MAXFILE]="", ext [MAXEXT]="";
  int i_drive ;

  //strupr (ptrsz_src);  //TO CHECK
  flags = fnsplit (ptrsz_src, drive, dir, name, ext);
  if (flags & DRIVE && flags & DIRECTORY && dir [0] == Slash)
  {
    return strcpy (ptrsz_dest, ptrsz_src) ;
  }
  if (flags & DRIVE)
  {
    i_drive = drive [0] - 'A' ;
    ptrsz_src += 2 ;
  }
  else
  {
    i_drive = getdisk ();
  }
  strcpy (ptrsz_dest, "X:") ;
  ptrsz_dest [0] = i_drive + 'A' ;
  if (flags & DIRECTORY && dir [0] == Slash)
  {
    strcpy (&ptrsz_dest [2], ptrsz_src) ;
    return ptrsz_dest ;
  }
  if (NULL == Current_Directory (i_drive + 1, ptrsz_dest))
  {
    return NULL ;
  }
  strcat (ptrsz_dest, SSlash) ;
  strcat (ptrsz_dest, ptrsz_src) ;
  return ptrsz_dest;
}

BOOL Copy_File_Nb (FILE* fdest, FILE* fsrc, long nbyte)
/*---------------------------------------------------*/
{
  char *buf ;
  unsigned long l;
  unsigned long k ;

#define BUF_LEN 1024

  if (nbyte == -1)
  {
    nbyte = MAXLONG ;
  }
  buf = (char *) malloc (BUF_LEN) ;
  if (buf == NULL)
  {
    ErrList (18) ;
    return FALSE ;
  }
  do
  {
	k = fread((char*)buf, 1, BUF_LEN, fsrc);
    if (k > 0)
    {
	  l=fwrite((char *)buf, 1, k, fdest);
	  if (l<k)
      {
		  free(buf);
		  return FALSE;
      }
    }
  }
  while (k > 0);
  free(buf);
  return TRUE ;
}

int Copy_File_ (int fdest, int fsrc, long nbyte)
/*-----------------------------------------------*/
{
    char *buf ;
    unsigned l;
    int k ;
#define BUF_LEN 1024

    if (nbyte == -1)
    {
        nbyte = MAXLONG ;
    }
    buf = (char *) malloc (BUF_LEN) ;
    if (buf == NULL)
    {
        return 0 ;
    }
    l = 0 ;
    do
    {
        k = read (fsrc, (char*)buf, BUF_LEN) ;
        if (k == -1)
        {
            free (buf) ;
            return 0 ;
        }
        if (k > 0)
        {
            l += k ;
            if (k != write (fdest, (char *)buf, k))
            {
                free (buf) ;
                return 0 ;
            }
        }
    }
    while (k == BUF_LEN && l < nbyte) ;
    free (buf) ;
    return 1 ;
}

char *Current_Directory (int drive, char *path)     //path konczy sie Slash
//-------------------------------------------
//drive is 0 for the default drive, 1 for A, 2 for B, etc.
{  int cur_dysk;
#ifndef LINUX
#ifdef FORWIN32
   strcpy (path, "X:\\") ;
#else
   strcpy (path, "X:/") ;
#endif
   if (drive == 0)
   {
	 drive = getdisk () ;
   }
     
   path [0] = 'A' + drive -1 ;
	 
   cur_dysk = getdisk();
#ifdef FORWIN32
   if (-1 == setdisk(drive-1))
   {
     path [0] = '\0';
     return NULL;
   }
#else
   setdisk(drive-1);
#endif
#ifdef LINUX
   if (-1 == /*getcurdir*/ getcwd (path,255))
#else
  if (NULL == _getcwd (path,255))
#endif
   {
     path [0] = '\0';
     return NULL;
   }
/*   if (path [strlen (path) - 1] != Slash)
   {
     strcat (path, SSlash);
   }   */
   setdisk(cur_dysk);
#else
    char *cwd;
    cwd = getcwd(path, MAXPATH);
#endif
   return path;
}

//-------------------------------------------------

BOOL Set_File_Attrib (char * path, int attrib)
/*------------------------------------------------*/
{
  BOOL ret;

  ret = TRUE;
#ifndef LINUX
  if (-1 == _chmod (path, attrib))
#else
      if (-1 == chmod (path, attrib))
#endif
  {
    ret = FALSE;
  }
  return ret;
}

char * AddBackslash (char * ptr_path)
/*----------------------------------*/
{
  if (ptr_path [strlen (ptr_path) -1] != BSlash /*Slash*/)
  {
    strcat (ptr_path, SSlash /*SSlash*/);
  }
  return ptr_path ;
}

char * AddSlash (char * ptr_path)
/*----------------------------------*/
{
#ifndef LINUX
  if (ptr_path [strlen (ptr_path) -1] != '\\')
  {
    strcat (ptr_path, "\\");
  }
#else
    if (ptr_path [strlen (ptr_path) -1] != '/')
    {
        strcat (ptr_path, "/");
    }
#endif
  return ptr_path ;
}



void Del_Backslash (char * ptr_path)
/*----------------------------------*/
{
  int i_len ;

  if (ptr_path == NULL)
  {
    return ;
  }
  i_len = strlen (ptr_path) ;
  if (i_len == 0 ||
      ptr_path [i_len - 1] != Slash ||
      (i_len == 3 && ptr_path [i_len - 1] == Slash))
  {
    return ;
  }
  ptr_path [i_len - 1] = '\0' ;
  return ;
}


BOOL file_bak (char *fn, char *fext)
/*----------------------------------*/
{
  int flags;
  char  path [MAXPATH], drive [MAXDRIVE], dir [MAXDIR], name [MAXFILE], ext [MAXEXT];
  char  fn1 [MAXPATH] ;

  strcpy (fn1, fn) ;
  flags = fnsplit (fn1, drive, dir, name, ext);
  flags = flags ;
  fnmerge(path, drive, dir, name, ".bak");
  int ret=unlink (path);
  rename (File_Ext (fn1, fext), path);
  return TRUE;
}

char *File_Tmp(char *fn, char *fext)
/*---------------------------------*/
{
  int flags;
  char drive [MAXDRIVE]="", dir [MAXDIR]="", name [MAXFILE]="", ext [MAXEXT]="";

  flags = fnsplit (fn, drive, dir, name, ext);
  fnmerge (fn, drive, dir, name, fext);
  return fn;
}


char *File_Ext(char *fn, char *fext)
/*---------------------------------*/
{
  int flags;
  char drive [MAXDRIVE]="", dir [MAXDIR]="", name [MAXFILE]="", ext [MAXEXT]="";

  flags = fnsplit (fn, drive, dir, name, ext);
  if (flags & EXTENSION)
  {
    return fn;
  }
  fnmerge (fn, drive, dir, name, fext);
  return fn;
}

char *Get_File_Ext(char *fn)
/*--------------------------*/
{
	int flags;
	char drive[MAXDRIVE]="", dir[MAXDIR]="", name[MAXFILE]="", ext[MAXEXT]="";

	flags = fnsplit(fn, drive, dir, name, ext);
	if (flags & EXTENSION)
	{
		return strupr(ext);
	}
	else return "";
}


char *File_New_Ext (char *fn, char *fext)
/*--------------------------------------*/
{
  char drive [MAXDRIVE]="", dir [MAXDIR]="", name [MAXFILE]="", ext [MAXEXT]="";

  fnsplit (fn, drive, dir, name, ext);
  fnmerge (fn, drive, dir, name, fext);
  return fn;
}



#ifndef FORWIN32

long Get_Disk_Free_Space  (char *fn)
//-----------------------------------
{
  int flags;
  char  drive [MAXDRIVE], dir [MAXDIR], name [MAXFILE], ext [MAXEXT];
  int disk ;
  struct dfree free ;
  long avail ;

  avail = -1 ;
  flags = fnsplit (fn, drive, dir, name, ext);
  if ((flags & DRIVE) == 0)
  {
    disk = getdisk () ;
  }
  else
  {
    disk = toupper (drive [0]) - 'A' ;
  }
  getdfree (disk + 1, &free);
  if (free.df_sclus == 0xFFFF)
  {
    return avail; //Error in getdfree() call
  }
  avail =  (long) free.df_avail *
	   (long) free.df_bsec *
	   (long) free.df_sclus;
  return avail;
}

#else

long Get_Disk_Free_Space  (char *fn)
//-----------------------------------
{
    return 1000000000;  //TEMPORARY
}

#endif