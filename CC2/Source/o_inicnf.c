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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef LINUX
#include <unistd.h>
#else
#include<fcntl.h>
#endif
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_inicnf.h"
typedef enum
{ I_Comment = 0, I_App, I_Key } T_Init_Row_Type;

#define MAXLINE 255

static char buuf[MAXLINE+1];


#define COM_TER ';'
#define KEY_TER '='
#define APP_END ']'
#define APP_BEG '['
#define KEY_TER_STR "="

static void 		condensation_returned_string (T_Fstring);
static T_Fstring 	ignore_spacing (T_Fstring );
static void 		truncate_spacing (T_Fstring );
static T_Init_Row_Type 	get_buf_contents (T_Fstring, T_Fstring*, T_Fstring*, T_Fstring*);

BOOL Get_Private_Profile_Strings(T_Fstring lpApplicationName, BOOL (*f)(T_Fstring, T_Fstring));
BOOL Get_Private_Profile_Strings_Hatch(T_Fstring lpApplicationName, BOOL(*f)(T_Fstring, T_Fstring));

#ifndef LINUX
extern _locale_t locale;
#else

#endif

extern void utf8Upper(char* text);

BOOL Get_Private_Profile_Strings_Cloud(T_Fstring lpApplicationName, BOOL (*f)(T_Fstring, T_Fstring))
/*----------------------------------------------------------------------------------------------*/
{
    int buf_len;
    FILE *stru_ini;
    T_Fstring key_name;
    T_Fstring ret_val, applicatin_name;
    BOOL b_app_found;
    T_Init_Row_Type cont;
    char ApplicName[128];

    stru_ini = fopen ( CLOUD_INI, "rt" ) ;
    if ( stru_ini == NULL )
    {
        return FALSE;
    }
    b_app_found = FALSE;
    strcpy(ApplicName, lpApplicationName);
    strupr(ApplicName);
    while (!feof(stru_ini))
    {
        if ( fgets(buuf , MAXLINE , stru_ini ) == NULL )
        {
            fclose(stru_ini);
            return TRUE;
        }

        buuf [MAXLINE-1] = '\0';
        buf_len = strlen (buuf) - 1;
        if (buuf [buf_len] == '\n')
        {
            buuf [buf_len] = '\0';
        }

        cont = get_buf_contents (buuf, &applicatin_name, &key_name,
                                 &ret_val);

        switch (cont)
        {
            case I_Comment:
                continue;
            case I_App:
                if (b_app_found == FALSE &&
                    strcmp (applicatin_name, ApplicName) == 0)
                {
                    b_app_found = TRUE;
                    break;
                }
                if (b_app_found == TRUE)
                {
                    fclose(stru_ini);
                    return TRUE;
                }
            case I_Key:
                if (b_app_found == TRUE)
                {
                    f (key_name, ret_val) ;
                }
                break;
            default:
                break;
        }
    }
    fclose(stru_ini);
    return TRUE;
}

BOOL Get_Private_Profile_Strings(T_Fstring lpApplicationName, BOOL (*f)(T_Fstring, T_Fstring))
				
/*---------------------------------------------------------*/
{
  int buf_len;
  FILE *stru_ini; 
 /* char buf [60]; */
  T_Fstring key_name;
  T_Fstring ret_val, applicatin_name;
  BOOL b_app_found;
  T_Init_Row_Type cont;
  char ApplicName[128];
  char* ptr;
  
  stru_ini = fopen ( FILENAME_INI, "rt" ) ;
  
  if ( stru_ini == NULL )
  {
    return FALSE;
  } 
  b_app_found = FALSE;


  strcpy(ApplicName, lpApplicationName);

  utf8Upper(&ApplicName);

  while (!feof(stru_ini))
  {

    if ( fgets(buuf , MAXLINE , stru_ini ) == NULL )
    {
      fclose(stru_ini); 
      return TRUE;
    }
    
    
    buuf [MAXLINE-1] = '\0';
    buf_len = strlen (buuf) - 1;
    if (buuf [buf_len] == '\n')
    {
      buuf [buf_len] = '\0';

        buf_len = strlen (buuf) - 1;
        if (buuf [buf_len] == '\r')
        {
            buuf [buf_len] = '\0';
        }
    }

    cont = get_buf_contents (buuf, &applicatin_name, &key_name,
				&ret_val);
				
				
    switch (cont)
    {
      case I_Comment:
	continue;
      case I_App:

	if (b_app_found == FALSE &&
	      strcmp (applicatin_name, ApplicName ) == 0)
	{
	  b_app_found = TRUE;
	  break;
	}
	if (b_app_found == TRUE)
	{
	  fclose(stru_ini);
	  return TRUE;
	}
      case I_Key:
	if (b_app_found == TRUE)
	{
	  f (key_name, ret_val) ;

	}
	break;
      default:
	break;
    }
  }
    fclose(stru_ini);
    return TRUE;
}


BOOL Get_Private_Profile_Strings_Hatch(T_Fstring lpApplicationName, BOOL(*f)(T_Fstring, T_Fstring))

	/*---------------------------------------------------------*/
{
	int buf_len;
	FILE *stru_ini;
	/* char buf [60]; */
	T_Fstring key_name;
	T_Fstring ret_val, applicatin_name;
	BOOL b_app_found;
	T_Init_Row_Type cont;
	char ApplicName[128];
	char* ptr;


	stru_ini = fopen ( PATTERNS_INI, "rt" ) ;
	if (stru_ini == NULL)
	{
		return FALSE;
	}
	b_app_found = FALSE;


	strcpy(ApplicName, lpApplicationName);
	utf8Upper(&ApplicName);
	
	while (!feof(stru_ini))
	{

		if (fgets(buuf, MAXLINE, stru_ini) == NULL)
		{
			fclose(stru_ini);
			return TRUE;
		}

		buuf[MAXLINE - 1] = '\0';
		buf_len = strlen(buuf) - 1;
		if (buuf[buf_len] == '\n')
		{
			buuf[buf_len] = '\0';

            buf_len = strlen (buuf) - 1;
            if (buuf [buf_len] == '\r')
            {
                buuf [buf_len] = '\0';
            }
		}

		cont = get_buf_contents(buuf, &applicatin_name, &key_name,
			&ret_val);

		switch (cont)
		{
		case I_Comment:
			continue;
		case I_App:
			if (b_app_found == FALSE &&
				strcmp(applicatin_name, ApplicName) == 0)
			{
				b_app_found = TRUE;
				break;
			}
			if (b_app_found == TRUE)
			{
				fclose(stru_ini);
				return TRUE;
			}
		case I_Key:
			if (b_app_found == TRUE)
			{
				f(key_name, ret_val);

			}
			break;
		default:
			break;
		}
	}
	fclose(stru_ini);
	return TRUE;
}


BOOL
Add_Private_Profile_Group	(T_Fstring lpApplicationName,
				BOOL (*add_value)(FILE *stru) )
/*----------------------------------------------------------------*/
{
  int buf_len;
  FILE *stru_old, *stru_new;
  char buf_new [MAXLINE+1];
  T_Fstring key_name, applicatin_name;
  T_Fstring old_val ;
  BOOL b_app_found, ret_val, append;
  T_Init_Row_Type cont;
  char ApplicName[128];

  ret_val = TRUE;
  append = FALSE;
  stru_new = fopen ( FILENAME_NEW, "wt" ) ;
  if ( stru_new == NULL )
  {
    return FALSE;
  }
  stru_old = fopen ( FILENAME_INI, "rt" ) ;
  if ( stru_old == NULL )
  {
    return FALSE;
  }
  b_app_found = FALSE;

  strcpy(ApplicName, lpApplicationName);

  utf8Upper(ApplicName);

  while (!feof(stru_old))
  {
    if ( fgets(buuf , MAXLINE , stru_old ) == NULL )
    {
      break;
    }
    strcpy (buf_new, buuf);
    buuf [MAXLINE-1] = '\0';
    buf_len = strlen (buuf) - 1;
    if (buuf [buf_len] == '\n')
    {
      buuf [buf_len] = '\0';
    }
    cont = get_buf_contents (buuf, &applicatin_name, &key_name,
				&old_val);
    switch (cont)
    {
      case I_Comment:
	  if (b_app_found == TRUE) continue; 	/*pominiecie usuwanej grupy*/
	  break;
      case I_Key:
	  if (b_app_found == TRUE) continue;  	/*pominiecie usuwanej grupy*/
	  break;
      case I_App:
	  if (b_app_found == FALSE &&
	  strcmp (applicatin_name, ApplicName) == 0)
	  {
	    if (fputs (buf_new , stru_new ) == EOF)
	    {
	      goto err;
	    }
	    if (add_value (stru_new) == FALSE)
	    {
		goto err;
	    }
	    b_app_found = TRUE;
	    continue;
	  }
	  else if (b_app_found == TRUE)
	  {
	    append = TRUE;
	    break;
	  }
	  else
	    break;
      default:
	break;
    }
    if (fputs (buf_new , stru_new ) == EOF)
    {
      goto err;
    }
    if (append == TRUE)
      break;
  }
  while (!feof(stru_old))
  {
    if ( fgets(buf_new , MAXLINE , stru_old ) == NULL )
    {
      break;
    }
    if (fputs (buf_new , stru_new ) == EOF)
    {
      goto err;
    }
  }
  if (fclose(stru_new) == EOF) ret_val = FALSE;
  if (fclose(stru_old) == EOF) ret_val = FALSE;
  if (unlink(FILENAME_INI) == -1)  ret_val = FALSE;
  if (rename(FILENAME_NEW, FILENAME_INI) == -1)  ret_val = FALSE;
  if (ret_val == FALSE) ErrList(8);
  return ret_val;
err:
  ErrList(8);
  fclose(stru_new);
  fclose(stru_old);
  int ret=unlink(FILENAME_NEW);
  return FALSE;
}



static T_Init_Row_Type
get_buf_contents (      T_Fstring buf,
/*--------------*/	T_Fstring *applicatin_name,
			T_Fstring *key_name,
			T_Fstring *ret_string)
{
  T_Fstring app_end, ter;
  T_Init_Row_Type ret_val;
  char* ret;

  ret_val = I_Comment;
  *applicatin_name = NULL;
  *key_name = NULL;
  *ret_string = NULL;
  if (*buf == '\0')
  {
      return ret_val;
  }
  if (*buf == APP_BEG)
  {
    buf++;
    app_end = strchr (buf, APP_END);
    if (app_end != NULL)
    {
      *app_end = '\0';

      *applicatin_name = buf;
	  utf8Upper(*applicatin_name);

      ret_val = I_App;
    }
    return ret_val;
  }
  *key_name = buf;
  *ret_string = strchr (*key_name, KEY_TER);
  if (*ret_string == NULL)
  {
    ter = strchr (*key_name, COM_TER);
    if (ter != NULL)
    {
      *ter = '\0';
    }
  }
  else
  {
    (*ret_string) [0] = '\0';
    (*ret_string)++;
    ter = strchr (*ret_string, COM_TER);
    if (ter != NULL)
    {
      *ter = '\0';
    }
    truncate_spacing (*ret_string);
    condensation_returned_string (*ret_string);
  }
  truncate_spacing (*key_name);
  ret_val = I_Key;
  return ret_val;
}

static void truncate_spacing (T_Fstring buf)
/*-----------------------------------------*/
{
  int i, len;
  T_Fstring s;

  len = strlen (buf) ;
  if (len == 0)
  {
    return;
  }
  s = buf + len - 1;
  for ( i = 0; i < len; i++, s--)
    if (*s != ' ' && *s != '\t') 	/*'\t' tabulacja*/
      break;
  *++s = '\0';
}

static T_Fstring ignore_spacing (T_Fstring buf)
/*-------------------------------------------------*/
{
  T_Fstring s;

  s = buf;
  while (1)
  {
    if (*s == ' ' || *s == '\t')
    {
      s++;
      continue;
    }
    if (*s == COM_TER)
    {
      *s = '\0';
    }
    return s;
  }
}

#define CHR_B "CHR$("
#define CHR_E ')'

static  void condensation_returned_string (T_Fstring buf)
/*-------------------------------------------------------*/
{
  T_Fstring s, s1, endptr;
  long  cod;
  unsigned char chr;
  int len;

  s = strstr (buf, CHR_B);
  if (s == NULL)
  {
    return;
  }
  s1 = s + strlen (CHR_B);
  len = strspn(s1, "0123456789");

  if (len > 0 && len < 4 && s1 [len] == CHR_E)
  {
    cod = strtol (s1, &endptr, 10);
    if (cod < 256)
    {
      chr = (unsigned char) cod;
      s [0] = chr;
#ifndef MACOS  //MACOS TEMPORARY
      strcpy (s + 1, s1 + len + 1);
#else

#endif
    }
  }
  condensation_returned_string (s + 1);
}

#ifdef aaaaaa
BOOL Add_Private_Profile_Strings	(T_Fstring lpApplicationName,
				BOOL (*change_value)(T_Fstring, T_Fstring, T_LpFstring, T_LpFstring) ,
				BOOL (*add_value)(T_LpFstring, T_LpFstring, T_LpFstring) )
/*-------------------------------------------------------------------------*/
{
  int buf_len;
  FILE *stru_old, *stru_new;
  char buf [MAXLINE+1];
  char buf_new [MAXLINE+1];
  T_Fstring key_name, applicatin_name;
  T_Fstring old_val, value, comment;
  BOOL b_app_found, ret_val, append;
  T_Init_Row_Type cont;

  ret_val = TRUE;
  append = FALSE;
  stru_new = fopen ( FILENAME_NEW, "wt" ) ;
  if ( stru_new == NULL )
  {
    return FALSE;
  }
  stru_old = fopen ( FILENAME_INI, "rt" ) ;
  if ( stru_old == NULL )
  {
    return FALSE;
  }
  b_app_found = FALSE;
  strupr (lpApplicationName);
  while (!feof(stru_old))
  {
    if ( fgets(buf , MAXLINE , stru_old ) == NULL )
    {
      break;
    }
    strcpy (buf_new, buf);
    buf [MAXLINE-1] = '\0';
    buf_len = strlen (buf) - 1;
    if (buf [buf_len] == '\n')
    {
      buf [buf_len] = '\0';
    }
    cont = get_buf_contents (buf, &applicatin_name, &key_name,
				&old_val);
    switch (cont)
    {
      case I_Comment:
	  break;
      case I_Key:
	  if (b_app_found == TRUE)
	  {
	    if (TRUE == change_value (key_name, old_val, &value, &comment))
	    {
	      sprintf(buf_new, "%s KEY_TER_STR %s%s\n",key_name, value, comment);
	    }
	  }
	  break;
      case I_App:
	  if (b_app_found == FALSE &&
		strcmp (applicatin_name, lpApplicationName) == 0)
	  {
	    b_app_found = TRUE;
	    break;
	  }
	  if (b_app_found == TRUE)
	  {
	    while (add_value (&key_name, &value, &comment) == TRUE)
	    {
	      if (EOF == fprintf (stru_new, "%s KEY_TER_STR %s %s\n",
				key_name, value, comment))
	      {
		goto err;
	      }
	    }
	    append = TRUE;
	    break;
	  }
      default:
	break;
    }
    if (fputs (buf_new , stru_new ) == EOF)
    {
      goto err;
    }
    if (append == TRUE)
      break;
  }
  while (!feof(stru_old))
  {
   /* if (EOF == fputc(fgetc(stru_old), stru_new))
    {
      goto err;
    } */
   if ( fgets(buf_new , MAXLINE , stru_old ) == NULL )
    {
      break;
    }
   if (fputs (buf_new , stru_new ) == EOF)
    {
      goto err;
    }
  }
  if (fclose(stru_new) == EOF) ret_val = FALSE;
  if (fclose(stru_old) == EOF) ret_val = FALSE;
  if (unlink(FILENAME_INI) == -1)  ret_val = FALSE;
  if (rename(FILENAME_NEW, FILENAME_INI) == -1)  ret_val = FALSE;
  if (ret_val == FALSE) ErrList(8);
  return ret_val;
err:
  ErrList(8);
  fclose(stru_new);
  fclose(stru_old);
  int ret=unlink(FILENAME_NEW);
  return FALSE;
}

#endif
