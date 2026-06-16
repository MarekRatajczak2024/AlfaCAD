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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<fcntl.h>
#include "o_inigfx.h"

#define MASTER_DATA    "MASTER"
#define SLAVE_DATA     "SLAVE"

extern BOOL Get_Private_Profile_Strings_GFX(T_Fstring lpApplicationName, BOOL (*f)(T_Fstring, T_Fstring));
extern BOOL Add_Private_Profile_Group_GFX(T_Fstring lpApplicationName, BOOL (*add_value)(FILE *stru) );

#ifdef LINUX
extern int stricmp(const char *a, const char *b);
extern int strnicmp(const char *a, const char *b, int n);
#endif

static DRIVER_STRUCT drv;

typedef enum
{
  GFX_Driver = 0,
  GFX_Width,
  GFX_Height,
  GFX_Bpp,
  GFX_Display_Switch,
  GFX_Mmx_Mode,
  GFX_Refresh,
  GFX_Aspect,
} Driver_Date;



static char *driver_param [] =
{
  "GFX_Driver",
  "GFX_Width",
  "GFX_Heihgt",
  "GFX_bpp",
  "GFX_Display_Switch",
  "GFX_MMX_Mode",
  "GFX_Refresh",
  "GFX_Aspect",
};
static int no_driver_param= sizeof(driver_param) / sizeof(driver_param [0]);

static char *driver_param_comment [] =
{
  ";",
  ";",
  ";",
  ";",
  ";",
  ";",
  ";",
  ";",
};


extern BOOL Get_Private_Profile_Strings (T_Fstring, BOOL (*f)(FILE *stru));


extern BOOL Add_Private_Profile_Group (T_Fstring, BOOL (*f)(T_Fstring, T_Fstring)); 



static BOOL get_driver_data (T_Fstring key_name, T_Fstring ret_string)
/*-------------------------------------------------------------------*/
{
   int i;
   double double_val;
   int int_val;
   BOOL bool_val;

   //strupr (key_name);
   for (i = 0; i < no_driver_param; i++)
   {
      if (stricmp (key_name, driver_param [i]) == 0)
      {
	    break;
      }
   }
   if (i >= no_driver_param)
   {
      return FALSE;
   }
  switch (i)
  {
     case GFX_Driver:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  drv.gfx_driver= int_val;
	}
	break;
     case GFX_Width:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  drv.gfx_width=int_val;
	}
	break;
     case GFX_Height:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  drv.gfx_height=int_val;
	}
	break;
     case GFX_Bpp:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  drv.gfx_bpp=int_val;
	}
	break;
     case GFX_Display_Switch:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  drv.gfx_display_switch=int_val;
	}
	break;
     case GFX_Mmx_Mode:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  drv.gfx_mmx_mode=int_val;
	}
	break;
     case GFX_Refresh:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  drv.gfx_refresh=int_val;
	}
	break;
	 case GFX_Aspect:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )
	{
	  drv.gfx_aspect=double_val;
	}
	break;
  }
  return TRUE;
}



static BOOL add_driver_data (FILE *stru)
/*------------------------------------*/
{
  T_Fstring key_name;
  T_Fstring comment;
  int i;

   for (i = 0; i < no_driver_param; i++)
   {
     key_name = driver_param [i];
     comment = driver_param_comment [i];
     switch (i)
     {
       case GFX_Driver:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, drv.gfx_driver, comment))
	    return FALSE;
	  break;
       case GFX_Width:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, drv.gfx_width, comment))
	    return FALSE;
	  break;
       case GFX_Height:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, drv.gfx_height, comment))
	    return FALSE;
	  break;
       case GFX_Bpp:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, drv.gfx_bpp, comment))
	    return FALSE;
	  break;
       case GFX_Display_Switch:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, drv.gfx_display_switch, comment))
	    return FALSE;
	  break;
       case GFX_Mmx_Mode:
	  if (EOF == fprintf (stru, "%s=%d \t\t%s\n",
			key_name, drv.gfx_mmx_mode, comment))
	    return FALSE;
	  break;
       case GFX_Refresh:
	  if (EOF == fprintf (stru, "%s=%d \t\t%s\n",
			key_name, drv.gfx_refresh, comment))
	    return FALSE;
	  break;
	   case GFX_Aspect:
	  if (EOF == fprintf (stru, "%s=%lf \t\t%s\n",
			key_name, drv.gfx_aspect, comment))
	    return FALSE;
	  break;
     }
   }
   if (EOF == fprintf (stru, "\n"))
    return FALSE;
   return TRUE;
}


int get_master_data(DRIVER_STRUCT *drv_data)
{ BOOL ret;
  ret = Get_Private_Profile_Strings_GFX((T_Fstring)MASTER_DATA, get_driver_data);
#ifndef NDEBUG
	////drv.gfx_display_switch = 1;  //it doesn't help in debugging anyway
#endif
  memcpy(drv_data, &drv, sizeof(DRIVER_STRUCT));
  return ret;
}

int get_slave_data(DRIVER_STRUCT *drv_data)
{ BOOL ret;
  ret = Get_Private_Profile_Strings_GFX((T_Fstring)SLAVE_DATA, get_driver_data);
#ifndef NDEBUG
	////drv.gfx_display_switch = 1;  //it doesn't help in debugging anyway
#endif
  memcpy(drv_data, &drv, sizeof(DRIVER_STRUCT));
  return ret;
}


int set_master_data(DRIVER_STRUCT *drv_data)
{ BOOL ret;
  memcpy(&drv, drv_data, sizeof(DRIVER_STRUCT));
  ret = Add_Private_Profile_Group_GFX((T_Fstring)MASTER_DATA, add_driver_data); 
  return ret;
}


int set_slave_data(DRIVER_STRUCT *drv_data)
{ BOOL ret;
  memcpy(&drv, drv_data, sizeof(DRIVER_STRUCT));
  ret = Add_Private_Profile_Group_GFX((T_Fstring)SLAVE_DATA, add_driver_data); 
  return ret;
}
