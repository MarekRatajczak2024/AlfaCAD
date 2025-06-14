/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#pragma warning(disable : 4996)
#include <forwin.h>
//#include <dir.h>
#include <ctype.h>
#include <string.h>

static char *max_ptr(char *p1, char *p2)
{
  if (p1 > p2)
    return p1;
  else
    return p2;
}

int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext)
{
  int flags = 0, len;
  const char *pp, *pe;

  if (drive)
    *drive = '\0';
  if (dir)
    *dir = '\0';
  if (name)
    *name = '\0';
  if (ext)
    *ext = '\0';

  pp = path;

  if ((isalpha((unsigned char )*pp)
       || strchr("[\\]^_`", *pp)) && (pp[1] == ':'))
  {
    flags |= DRIVE;
    if (drive)
    {
      drive[0]='\0';
      strncpy(drive, pp, 2);
      drive[2] = '\0';
    }
    pp += 2;
  }

  pe = max_ptr(strrchr(pp, '\\'), strrchr(pp, '/'));
  if (pe) 
  { 
    flags |= DIRECTORY;
    pe++;
    len = (int)(pe - pp);
    if (dir)
    {
      dir[0]='\0';
      strncpy(dir, pp, len);
      dir[len] = '\0';
    }
    pp = pe;
  }
  else
    pe = pp;

  /* Special case: "c:/path/." or "c:/path/.."
     These mean FILENAME, not EXTENSION.  */
  while (*pp == '.')
    ++pp;
  if (pp > pe)
  {
    flags |= FILENAME;
    if (name)
    {
      len = (int)(pp - pe);
      name[0]='\0';
      strncpy(name, pe, len);
      name[len] = '\0';
      /* advance name over '.'s so they don't get scragged later on when the
       * rest of the name (if any) is copied (for files like .emacs). - WJC
       */
      name+=len;
    }
  }

  pe = strrchr(pp, '.');
  if (pe)
  {
    flags |= EXTENSION;
    if (ext)
      {
      len = strlen(pe);
      ext[0]='\0';
      strcpy(ext, pe);
      ext[len] = '\0';
    }
  }
  else 
    pe = strchr( pp, '\0');

  if (pp != pe)
  {
    flags |= FILENAME;
    len = (int)(pe - pp);
    if (name)
    {
      name[0]='\0';
      strncpy(name, pp, len);
      name[len] = '\0';
    }
  }

  if (strcspn(path, "*?[") < strlen(path))
    flags |= WILDCARDS;

  return flags;
}

#ifdef TEST

#include <stdio.h>

int
main(void)
{
  char arg[81], drive[81], dir[81], fname[81], ext[81];

  fputs("> ", stdout); fflush(stdout);
  gets(arg);

  printf("`%s' (%x): `%s' `%s' `%s' `%s'\n", arg,
         fnsplit(arg, drive, dir, fname, ext), drive, dir, fname, ext);

  return 0;
}

#endif
