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

#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#ifndef LINUX
#include<io.h>
#include<dos.h>
#endif
#include<fcntl.h>
#include<string.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_dialog.h"
#include "o_libfun.h"
#include "o_image.h"

#include "leak_detector_c.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

typedef unsigned int GrColor;

extern unsigned long_long get_free_mem_size (int way) ;
extern void  redraw_cur (BOOL cur) ;
extern void getimage(int left, int top, int right, int bottom, void  *bitmap);
extern void putimage(int left, int top,  void  *bitmap, int op);
extern void GrClearScreen(GrColor bg);


static BITMAP *ptr_image0=NULL;   //najwiekszy
static BITMAP *ptr_image1=NULL;   //rezerwowe 100x10
static BITMAP *ptr_image2=NULL;
static BITMAP *ptr_image3=NULL;
static BITMAP *ptr_image4=NULL;
static BITMAP *bitmap_ptr=NULL;

void Image_Free (void) ;

typedef struct
{
  int x1, y1 ; 	/*wspolrzedne bitmapy*/
  int x2, y2 ; 	/*wspolrzedne bitmapy*/
  unsigned n ;	/*wielkosc_bitmapy*/
}
T_Image_Coord ;

#define MAXCOORD 20

typedef struct
{
	int 		b_xms;			/*alokacja w xms czy na dysku*/
	unsigned 	handle;            	/*handle do xms lub pliku gdy otwarty*/
	char 		file_buf[13];         /*nazwa pliku gdy alokacja na dysku*/
    float      letter_spacing;
	BOOL 		b_fopen;		/*TRUE gdy plik otwarty*/
}
T_XMS_Alloc_Param, *T_Lp_XMS_Alloc_Param;



typedef struct
{
  T_Image_Coord 	image_coord [MAXCOORD] ;	/*wspolrzedne i wielkosc bitmap*/
  int 			no ;			/*liczba bitmap*/
  long 			buf_len ;		/*wielkosc bufora*/
  BOOL 			b_buf_menu ;  		/*lokalizacja bufora (bufor menu, ...)*/
  T_XMS_Alloc_Param 	alloc_param ;
}
T_Image_Param,   * T_Lp_Image_Param ;


static BOOL 		b__get_image  = FALSE ;
static T_Image_Param 	image_param ;

#define MAXBUFSIZE 64000L
#define MINIMAGEROW 10
#define ALF_IFN  "ALFIM.TMP"

long Image_Size (int x1, int y1, int x2, int y2,
		T_Lp_Image_Param lps_image_param)
/*----------------------------------------------*/
{
  unsigned buf_len ;
  BOOL b_buf_menu ;
  unsigned image_dy10, image_dy, image_dy2 ;
  int dy10, dy, n, i ;
  long dw_retval = -1 ;		/*-1 dla bledu*/

  if (GetFreeMemSize () > get_free_mem_size (1))
  {
    b_buf_menu = FALSE ;
    buf_len = (unsigned)min (MAXBUFSIZE, GetFreeMemSize ()) ;
  }
  else
  {
    b_buf_menu = TRUE ;
    buf_len = (unsigned)min (MAXBUFSIZE, get_free_mem_size (1)) ;
  }
  buf_len -= 4 ;
  if (buf_len < 4)
  {
    return dw_retval ;
  }
  buf_len -= 4 ;
  dy10 = (y2 - y1) / MINIMAGEROW ;
  image_dy10 = imagesize (x1, y1, x2, y1 + dy10) ;
  if (image_dy10 == (unsigned)-1 || image_dy10 > buf_len)
  {
    return dw_retval ;
  }
  dy = dy10 * (buf_len / image_dy10) ;
  n = (y2 - y1 + 1) / dy ;
  if (n + 1 > MAXCOORD)
  {
    return dw_retval ;
  }
  image_dy = imagesize (x1, y1, x2, y1 + dy - 1) ;
  for (i = 0; i < n ; i++)
  {
     lps_image_param->image_coord [i].x1 = x1 ;
     lps_image_param->image_coord [i].x2 = x2 ;
     lps_image_param->image_coord [i].y1 = y1 + i * dy ;
     lps_image_param->image_coord [i].y2 = y1 + (i + 1) * dy - 1 ;
     lps_image_param->image_coord [i].n = image_dy ;
  }
  if ((y2 - y1 + 1) % dy > 0)
  {
    lps_image_param->image_coord [n].x1 = x1 ;
    lps_image_param->image_coord [n].x2 = x2 ;
    lps_image_param->image_coord [n].y1 = y1 + n * dy ;
    lps_image_param->image_coord [n].y2 = y2 ;
    image_dy2 = imagesize (x1, y1 + n * dy, x2, y2) ;
    lps_image_param->image_coord [n].n = image_dy2 ;
  }
  dw_retval = n * (long)image_dy + image_dy2 ;
  lps_image_param->no = n + 1 ;
  lps_image_param->buf_len = buf_len ;
  lps_image_param->b_buf_menu = b_buf_menu ;
  return dw_retval ;
}

void Image_Free (void)
/*------------------*/
{
   TDIALOG *ptrs_dlg ;

  if (b__get_image  == TRUE)
  {
    b__get_image  = FALSE ;
  }
  if (NULL != (ptrs_dlg = Get_Dlg ()))
  {
    ptrs_dlg->flags = FLDLG_NOIMAGE ;
  }
}

/*---------------------------------------------------------------*/

#define MAX_IMAGE_TMP 5
#define IMAGE_FILE_NAME "ALFTMP"
static T_Image_Param  image_param_tab [MAX_IMAGE_TMP] ;
static BOOL image_tab [MAX_IMAGE_TMP] = {FALSE,FALSE,FALSE,FALSE,FALSE} ;
const int image_max_no = MAX_IMAGE_TMP ;

int Get_Image_Tmp (int x1, int y1, int x2, int y2)
/*------------------------------------------------*/
{
  int image_no, i ;
  char sz_file_name [20], sz_tmp [10] ;

  for (i = 0 ; i < image_max_no ; i++)
  {
    if (image_tab [i] == FALSE)
    {
      break ;
    }
  }
  image_no = -1 ;
  if (i < image_max_no)
  {

    if (i==0)
     {
       bitmap_ptr=create_bitmap(x2-x1+1, y2-y1+1); //???????????
       ptr_image0=bitmap_ptr; //??????????????
       getimage (x1, y1, x2, y2, ptr_image0);
     }
     else
      if (i==1)
       {
         bitmap_ptr=create_bitmap(x2-x1+1, y2-y1+1); //?????????????
         ptr_image1=bitmap_ptr;
         getimage (x1, y1, x2, y2, ptr_image1);
       }
       else
       if (i==2)
         {
           bitmap_ptr=create_bitmap(x2-x1+1, y2-y1+1);  //??????????
           ptr_image2=bitmap_ptr; //????????????????
           getimage (x1, y1, x2, y2, ptr_image2);
         }
        else
         if (i==3)
           {
             bitmap_ptr=create_bitmap(x2-x1+1, y2-y1+1);
             ptr_image3=bitmap_ptr;
             getimage (x1, y1, x2, y2, ptr_image3);  //?????????????????
           }
          else
           if (i==4)
             {
               bitmap_ptr=create_bitmap(x2-x1+1, y2-y1+1);
               ptr_image4=bitmap_ptr;
               getimage (x1, y1, x2, y2, ptr_image4);  //?????????????????
             }

     image_no = i ;
     image_tab [image_no] = TRUE ;
  }
  return image_no ;
}

void Put_Image_Tmp (int image_no, int x, int y)
/*--------------------------------------------*/
{
  if (image_no != -1 && image_tab [image_no] != FALSE)
  {
    if (image_no==0) 
	{
	  putimage (x, y, ptr_image0, COPY_PUT) ;
    }
     else
      if (image_no==1) 
	  {
		  putimage (x, y, ptr_image1, COPY_PUT) ;
	  }
       else
        if (image_no==2) 
		{
			putimage (x, y, ptr_image2, COPY_PUT) ;
		}
         else
		    if (image_no==3) 
			{
				putimage (x, y, ptr_image3, COPY_PUT) ;
			}
           else
            if (image_no==4) 
			{
				putimage (x, y, ptr_image4, COPY_PUT) ;
			}

    image_tab [image_no] = FALSE ;
  }
}
