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

#define __PCX__
#ifndef PCX
#define PCX
#endif
#define byte_ unsigned char

#include <allegro.h>


/*
  Here's the header file I use for PCX files.  I've tried to comment it
  somewhat, though it may still be quite ambiguous.

  The palette functions are not included, so these won't work "as is".

  Also, this was written with DJGPP V2.0, so it assumes it is running in
  386 protected mode; and contains some DJGPP-specific code.
    (" __attribute__ ((packed))" is DJGPP-specific)

  SlamPal() simply sets all 256 colors, but is not included.


  Note that "bytes_per_line" MUST be even!  Zsoft must have been having
  a bad day when they made this file format, because it is totally
  pointless to require images to have an even width.

  If the image has an odd width, an extra byte is added to the end of
  each line ... so we have to compensate for the extra junk that is
  stored.

  Fortunately, I've never seen a program that wouldn't load an
  incorrectly-saved image (with an odd width, and stored as an odd width);
  so we can go ahead and save PCX files "incorrectly" (really, it is more
  efficient, and creates smaller files).  Only the loader needs to
  compensate for odd-width images (and also for "incorrect" images).

  If the image width and the "bytes_per_line" are the same, this problem
  doesn't show up at all.  And, if they are different, we just need to
  ignore the last pixel on each line.

*/


#include <forwin.h>
#include <stdio.h>
#include <stdlib.h>
#define ALLEGWIN
#include <allegext.h>
#include<math.h>
#include<string.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_bloklb.h"
#include "bgiext.h"
#include<fcntl.h>
#include "jpgalleg.h"

#include "message.h"

#include "leak_detector_c.h"
#include "internal/aintern.h"

#define PUTPIXEL myputpixel

void Procent_0(void);
int Draw_png_virtual(B_PCX *pcx, int opcja, int ctx_x0, int ctx_y0, int ctx_ww, int ctx_wh, int kod_obiektu);

typedef char T768[768];

#define FIXED_POINT_FRACTIONAL_BITS 5

typedef uint16_t fixed_point_t;

extern BITMAP *screenplay;


extern BITMAP *_fixup_loaded_bitmap(BITMAP *bmp, PALETTE pal, int bpp);

//extern int qsort_by_val(double *e1, double *e2);
extern int qsort_by_val(const void *e1, const void *e2);
extern double pikseleX0d (double jednostki);
extern double pikseleY0d (double jednostki);
extern char * Get_PCX (int option);
extern void ReGet_PCX (void);
extern int get_palette_color(int color);
extern void GrHLine(int x1,int x2,int y,unsigned int c);
extern BOOL Check_if_Equal (double x, double y);
extern BOOL Check_if_Equal2(double x, double y);
extern void GrFilledPolygon(int numpts,int minx,int miny,int maxx,int maxy,const int *points,unsigned int c, int nr);
extern void Normalize_Solid (WIELOKAT *ptr_w);
extern void setrgbpalette(int color, int red, int green, int blue);
extern void myputpixel(int x, int y, int color);
extern void fade_in2(COLOR pallete, int speed);
extern void delay(int time);
extern void fade_out2(COLOR pallete, int speed);
int Draw_pcx_8(B_PCX *pcx, int opcja);
int Draw_pcx_1(B_PCX *pcx);
extern long int Wyslij_par_ekranu(int x);
int Draw_pcx_8_virtual(B_PCX *pcx, int opcja, int ctx_x0, int ctx_y0, int ctx_ww, int ctx_wh);
int Draw_pcx_1_virtual(B_PCX *pcx, int ctx_x0, int ctx_y0, int ctx_ww, int ctx_wh);
extern void getviewsettings(struct viewporttype  *viewport);
extern void destroy_sprite_bmp(int nr);
extern int my_kbhit(void);
extern unsigned char my_getch(void);
extern long pikseleX00 (double jednostki);
extern long pikseleY00 (double jednostki);
extern void getviewsettings(struct viewporttype  *viewport);
extern void getimage(int left, int top, int right, int bottom, void  *bitmap);
extern void setviewport(int left, int top, int right, int bottom, int clip);

extern void InitBufPCX(unsigned long_long buffer_size);
extern void FreeBufPCX(void);
extern int TRANSLUCENCY;
extern void set_mode_solid(void);
extern void Rotate_Point(double, double, double, double, double, double, double*, double*);
extern int Draw_png(B_PCX *pcx, int kod_obiektu);
extern long filelength(int f);

static B_PCX *adr_pcx;
static char *adres_pcx;

static long mxx,mxy,mmx,mmy;
int xxyy[4][2];
static byte_ key_r[180000];  //28000
static int nr_i=0;
static int i_i[16];
static int j_j[16];
static fixed fixkat=0;

static int __minx,__miny,__maxx,__maxy;


byte_ color_key[256];
long procent0;
static unsigned int dlugosc_bloku_pcx;

extern long pXp,pYp,pXk,pYk;


int LoadPCX_(B_PCX *b_pcx, PCXheader *head, char enablepal);
int LoadPCX(B_PCX *b_pcx, PCXheader *head, char enablepal, int x, int y, int opcja);
int LoadPCXinfo(char *filename, PCXheader *head); // , COLOR *palette);
int SavePCX(char *filename, int width, int height);

inline double fixed_to_float(fixed_point_t input)
{
	return ((double)input / (double)(1 << FIXED_POINT_FRACTIONAL_BITS));
}

inline fixed_point_t float_to_fixed(double input)
{
	return (fixed_point_t)(round(input * (1 << FIXED_POINT_FRACTIONAL_BITS)));
}


BOOL get_on_front_PCX(void)
{
    B_PCX *b_pcx;

    b_pcx=(B_PCX *)adres_pcx;

    if (b_pcx != NULL)
    {
        return b_pcx->on_front;
    }
    else return FALSE;
}

int get_dx_dy_PCX(double *dx, double *dy)
 { B_PCX *b_pcx;

   adres_pcx = Get_PCX(0);
   if (adres_pcx!=NULL)
    {

      b_pcx=(B_PCX *)adres_pcx;
      *dx=b_pcx->dx;
      *dy=b_pcx->dy;
      ReGet_PCX();

	  blokzap(adres_pcx, adres_pcx, ANieOkreslony, XOR_PUT, 1);
      return 1;
    }
     else
      {
       ReGet_PCX();
       return 0;
      }
 }

 char *get_address_PCX(int typ)
 { B_PCX *b_pcx;

   adres_pcx = Get_PCX(typ);
   if (adres_pcx!=NULL)
    {
      b_pcx=(B_PCX *)adres_pcx;
      ReGet_PCX();
	  Cur_offd(X, Y);
	  blokzap(adres_pcx, adres_pcx, ANieOkreslony, XOR_PUT, 1);
	  Cur_ond(X, Y);
      return (char*)b_pcx;
    }
     else
      {
       ReGet_PCX();
       return NULL;
      }
 }

 void undo_pcx_blockzap(void)
 {
	 blokzap(adres_pcx, adres_pcx, ANieOkreslony, COPY_PUT, 1);
     CUR_OFF(X,Y);
     CUR_ON(X,Y);
 }

 void set_dx_dy_PCX(double dx, double dy)
 { B_PCX *b_pcx;

   b_pcx=(B_PCX *)adres_pcx;
   b_pcx->dx=dx;
   b_pcx->dy=dy;
   Change = TRUE;
 }

 void set_x_y_PCX(double x, double y)
 {
   adr_pcx->x=x;
   adr_pcx->y=y;
 }

 unsigned char tab_bit[8]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
 void prostokat1(int x, int y, int i, int j, int kolor)
  {unsigned char pixels;
   int kolor_map;

   int l;
   int ii;

   kolor_map=get_palette_color(15);

   for (ii=0; ii<8; ii++)
    {
     pixels = (kolor & tab_bit[ii]);
     if (pixels)
      {
        for(l=0;l<j;l++)
          GrHLine(x, x+ii, y+l, kolor_map);
          GrHLine(x, x+ii, y+l, kolor_map);
      }
    }
  }

 //DO MODYFIKACJI
 void  get_xxyy0(int x0, int y0, int xx1_, int xx2_, int y, int j, double kos, double koc)
  { double xx11, yy11;
    double xx1, xx2;

                   xx1 = (xx1_-x0);
				   xx1 /= sk_x;
				   xx1 += x0;
				   
 
                   xx2 = (xx2_-x0);
				   xx2 /= sk_x;
				   xx2 += x0;
				   

				   xx11 = x0 + ((xx1 - x0) * koc + (y - y0) * kos) * sk_x;
                   xxyy[0][0]=(int)xx11;
                   yy11 = y0 - (xx1 - x0) * kos + (y - y0) * koc;
                   xxyy[0][1]=(int)yy11;

                   xx11 = x0 + ((xx2 - x0) * koc + (y - y0) * kos) * sk_x;
                   xxyy[1][0]=(int)xx11;
                   yy11 = y0 - (xx2 - x0) * kos + (y - y0) * koc;
                   xxyy[1][1]=(int)yy11;

                   xx11 = x0 + ((xx2 - x0) * koc + (y + j - y0) * kos) * sk_x;
                   xxyy[2][0]=(int)xx11;
                   yy11 = y0 - (xx2 - x0) * kos + (y + j - y0) * koc;
                   xxyy[2][1]=(int)yy11;

                   xx11 = x0 + ((xx1 - x0) * koc + (y + j - y0) * kos) * sk_x;
                   xxyy[3][0]=(int)xx11;
                   yy11 = y0 - (xx1 - x0) * kos + (y + j - y0) * koc;
                   xxyy[3][1]=(int)yy11;

  }


 void  get_xxyy(int x0, int y0, int x_0, int y_0, int xx1_, int xx2_, int y, int j, double kos, double koc)
  { double xx11, yy11;
    double xx1, xx2;

                   xx1 = (xx1_-x0);
				   xx1 /= sk_x;
				   xx1 += x0;

                   xx2 = (xx2_-x0);
				   xx2 /= sk_x;
				   xx2 += x0;

				   xx11 = x0 + ((xx1 - x0) * koc + (y - y0) * kos) * sk_x;
				   xxyy[0][0] = round(xx11);
                   yy11 = y0 - (xx1 - x0) * kos + (y - y0) * koc;
				   xxyy[0][1] = round(yy11);

                   xx11 = x0 + ((xx2 - x0) * koc + (y - y0) * kos) * sk_x;
				   xxyy[1][0] = round(xx11);
                   yy11 = y0 - (xx2 - x0) * kos + (y - y0) * koc;
				   xxyy[1][1] = round(yy11);

                   xx11 = x0 + ((xx2 - x0) * koc + (y + j - y0) * kos) * sk_x;
				   xxyy[2][0] = round(xx11);
                   yy11 = y0 - (xx2 - x0) * kos + (y + j - y0) * koc;
				   xxyy[2][1] = round(yy11);

                   xx11 = x0 + ((xx1 - x0) * koc + (y + j - y0) * kos) * sk_x;
				   xxyy[3][0] = round(xx11);
                   yy11 = y0 - (xx1 - x0) * kos + (y + j - y0) * koc;
				   xxyy[3][1] = round(yy11);


				   xxyy[0][0]-=x_0;
                   xxyy[1][0]-=x_0;
                   xxyy[2][0]-=x_0;
                   xxyy[3][0]-=x_0;

				   xxyy[0][1]-=y_0;
                   xxyy[1][1]-=y_0;
                   xxyy[2][1]-=y_0;
                   xxyy[3][1]-=y_0;


				   __minx=xxyy[0][0];
	               if (xxyy[1][0]<__minx) __minx=xxyy[1][0];
	               if (xxyy[2][0]<__minx) __minx=xxyy[2][0];
	               if (xxyy[3][0]<__minx) __minx=xxyy[3][0];
	               __maxx=xxyy[0][0];
	               if (xxyy[1][0]>__maxx) __maxx=xxyy[1][0];
	               if (xxyy[2][0]>__maxx) __maxx=xxyy[2][0];
	               if (xxyy[3][0]>__maxx) __maxx=xxyy[3][0];

				   __miny=xxyy[0][1];
	               if (xxyy[1][1]<__miny) __miny=xxyy[1][1];
	               if (xxyy[2][1]<__miny) __miny=xxyy[2][1];
	               if (xxyy[3][1]<__miny) __miny=xxyy[3][1];
	               __maxy=xxyy[0][1];
	               if (xxyy[1][1]>__maxy) __maxy=xxyy[1][1];
	               if (xxyy[2][1]>__maxy) __maxy=xxyy[2][1];
	               if (xxyy[3][1]>__maxy) __maxy=xxyy[3][1];

  }



void  get_xxyy1(int x0, int y0, int x_0, int y_0, int xx1_, int xx2_, int y, int j, double kos, double koc)
  { double xx11, yy11;
    double xx1, xx2;
                   
                   xx1 = (xx1_-x0);
				   xx1 /= sk_x;
				   xx1 += x0;

                   xx2 = (xx2_-x0);
				   xx2 /= sk_x;
				   xx2 += x0;

				   xx11 = x0 + ((xx1 - x0) * koc + (y - y0) * kos) * sk_x;
                   xxyy[0][0]=(int)xx11;
                   yy11 = y0 - (xx1 - x0) * kos + (y - y0) * koc;
                   xxyy[0][1]=(int)yy11;

				   xxyy[0][0]-=x_0;
                   xxyy[0][1]-=y_0;
  }

 void prostokat2(int x_, int y_, int i, int j, int nn,int kolor, int kolor_tla0,int kolor_tla, int kolor_punktu, int ignore_bk, int x0, int y0, double kos, double koc)
  {unsigned char pixels;

   int l;
   int bits;
   int xx1, xx2, xx22;
   int ii;
   int n8=8;
   int y;
   int x;
   BOOL kat0;
   int kolor_map, kolor_punktu_map, kolor_tla0_map, kolor_tla_map;
   int nr;

    kolor_map=get_palette_color(kolor);
	kolor_tla0_map=get_palette_color(kolor_tla0);
	kolor_tla_map=get_palette_color(kolor_tla);
	kolor_punktu_map=get_palette_color(kolor_punktu);

    ii=i/8;

    if (Check_if_Equal(kos,0.0)==TRUE) kat0=TRUE; 
	  else 
	  {
		  kat0=FALSE;
		  nr = 0;
	  }

		if (nn<8)
		 {
		  n8=nn;
		 }

       xx1=x_;
       x=x_;
       y=y_;

       xx2=x+ii;
       xx22=x+(ii*n8);

   if ((xx1<=mxx) && (xx22>=mmx) && ((y+j)>=mmy) && (y<=mxy))
    {
    for (bits=0; bits<n8; bits++)
     {
       pixels = (kolor & tab_bit[bits]);
      if (kolor_tla0!=255) //ustawione tlo
       {
        if (!pixels)  //[0]
        {
         if (kolor_punktu==kolor_tla0)  //to jest tlo
          {
           if (ignore_bk==0)
            {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                 GrHLine(xx1, xx2, y+l, kolor_punktu_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0, 0, 0, xx1, xx2, y, j, kos,koc);
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,nr);
                 }
            }
          }
          else //to nie jest tlo tylko linia
           {
            if (kat0==TRUE)
             {
              for(l=0;l<j;l++)
              GrHLine(xx1, xx2, y+l, kolor_punktu_map);
             }
              else if (j>0)
                 {
                   get_xxyy(x0, y0, 0, 0,xx1, xx2, y, j, kos, koc);
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,nr);
                 }
           }
        }
         else
           {
             if (kolor_tla==kolor_tla0)  //to jest tlo
              {
               if (ignore_bk==0)
                {
                 if (kat0==TRUE)
                  {
                   for(l=0;l<j;l++)
                   GrHLine(xx1, xx2, y+l, kolor_tla0_map);
                  }
                   else if (j>0)
                    {
                      get_xxyy(x0, y0, 0, 0,xx1, xx2, y, j, kos, koc);
                      GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla0_map,nr);
                    }
                }
              }
               else //to nie jest tlo tylko linia
                {
                  if (kat0==TRUE)
                   {
                     for(l=0;l<j;l++)
                     GrHLine(xx1, xx2, y+l, kolor_tla_map);
                   }
                    else if (j>0)
                     {
                      get_xxyy(x0, y0, 0, 0,xx1, xx2, y, j, kos, koc);
                      GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla_map,nr);
                     }
                }
           }
       }
        else  //nie jest ustawione tlo
         {
          if (pixels==0)           //kolor 0
           {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                GrHLine(xx1, xx2, y+l, kolor_punktu_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0, 0, 0,xx1, xx2, y, j, kos, koc);
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,nr);
                 }
           }
           else                     //kolor 1
           {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                GrHLine(xx1, xx2, y+l, kolor_tla_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0, 0, 0,xx1, xx2, y, j, kos, koc);
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla_map,nr);
                 }
           }
         }

       xx1=xx2;
       if (bits==6) xx2=x+i;
         else
          {
            ii=((bits+2)*i);
            ii/=8;
            xx2=x+ii;
          }
     }
    }
  }

 void prostokat2_(int x_0, int y_0, int x_, int y_, int i, int j, int nn,int kolor, int kolor_tla0,int kolor_tla, int kolor_punktu, int ignore_bk, int x0, int y0, double kos, double koc)
  {unsigned char pixels;

   int l;
   int bits;
   int xx1, xx2, xx22;
   int ii;
   int n8=8;
   int y;
   int x;
   BOOL kat0;
   int kolor_map, kolor_punktu_map, kolor_tla0_map, kolor_tla_map;
   int nr;
   
    kolor_map=get_palette_color(kolor);
	kolor_tla0_map=get_palette_color(kolor_tla0);
	kolor_tla_map=get_palette_color(kolor_tla);
	kolor_punktu_map=get_palette_color(kolor_punktu);

    ii=i/8;

    if (Check_if_Equal(kos,0.0)==TRUE) kat0=TRUE; 
	  else
	  {
		  kat0=FALSE;
		  nr = 0;
	  }
    
    if (nn<8)
     {
      n8=nn;
     }

       xx1=x_;
       x=x_;
       y=y_;

       xx2=x+ii;
       xx22=x+(ii*n8);

    if ((xx1<=mxx) && (xx22>=mmx) && ((y+j)>=mmy) && (y<=mxy))
    {
    for (bits=0; bits<n8; bits++)
     {
       pixels = (kolor & tab_bit[bits]);
      if (kolor_tla0!=255) //ustawione tlo
       {
        if (!pixels)  //[0]
        {
         if (kolor_punktu==kolor_tla0)  //to jest tlo
          {
           if (ignore_bk==0)
            {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                 GrHLine(xx1 - x_0, xx2 - x_0, y+l - y_0, kolor_punktu_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0, x_0, y_0, xx1, xx2, y, j, kos, koc);				             			  				   
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,nr);
                 }
            }
          }
          else //to nie jest tlo tylko linia
           {
            if (kat0==TRUE)
             {
              for(l=0;l<j;l++)
              GrHLine(xx1 - x_0, xx2 - x_0, y+l - y_0, kolor_punktu_map);
             }
              else if (j>0)
                 {
                   get_xxyy(x0, y0, x_0, y_0, xx1, xx2, y, j, kos, koc);
				  
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,nr);
                 }
           }
        }
         else //[1]
           {
             if (kolor_tla==kolor_tla0)  //to jest tlo
              {
               if (ignore_bk==0)
                {
                 if (kat0==TRUE)
                  {
                   for(l=0;l<j;l++)
                   GrHLine(xx1 - x_0, xx2 - x_0, y+l - y_0, kolor_tla0_map);
                  }
                   else if (j>0)
                    {
                      get_xxyy(x0, y0, x_0, y_0, xx1, xx2, y, j, kos, koc);
					 
                      GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla0_map,nr);
                    }
                }
              }
               else //to nie jest tlo tylko linia
                {
                  if (kat0==TRUE)
                   {
                     for(l=0;l<j;l++)
                     GrHLine(xx1 - x_0, xx2 - x_0, y+l - y_0, kolor_tla_map);
                   }
                    else if (j>0)
                     {
                      get_xxyy(x0, y0, x_0, y_0, xx1, xx2, y - y_0, j, kos, koc);
					 
                      GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla_map,nr);
                     }
                }
           }
       }
        else  //nie jest ustawione tlo
         {
          if (pixels==0)           //kolor 0
           {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                GrHLine(xx1 - x_0, xx2 - x_0, y+l - y_0, kolor_punktu_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0, x_0, y_0, xx1, xx2, y - y_0, j, kos, koc);
				   				  				   
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,nr);
                 }
           }
           else                     //kolor 1
           {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                GrHLine(xx1, xx2, y+l - y_0, kolor_tla_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0, x_0, y_0, xx1, xx2, y - y_0, j, kos, koc);
				                   			   
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla_map,nr);
                 }
           }
         }

       xx1=xx2;
       if (bits==6) xx2=x+i;
         else
          {
            ii=((bits+2)*i);
            ii/=8;
            xx2=x+ii;
          }
     }
    }
  }

 void load_prostokat2_to_RAM(int x_, int y_, int i, int j, int nn,int kolor, int kolor_tla0,int kolor_tla, int kolor_punktu, int ignore_bk, int x0, int y0, double kos, double koc)
  {unsigned char pixels;

   int l;
   int bits;
   int xx1, xx2, xx22;
   int ii;
   int n8=8;
   int y;
   int x;
   BOOL kat0;
   int kolor_map, kolor_punktu_map, kolor_tla0_map, kolor_tla_map;

    kolor_map=get_palette_color(kolor);
	kolor_tla0_map=get_palette_color(kolor_tla0);
	kolor_tla_map=get_palette_color(kolor_tla);
	kolor_punktu_map=get_palette_color(kolor_punktu);

    ii=i/8;

    if (Check_if_Equal(kos,0.0)==TRUE) kat0=TRUE; else kat0=FALSE;

    if (nn<8)
     {
      n8=nn;
     }

       xx1=x_;
       x=x_;
       y=y_;

       xx2=x+ii;
       xx22=x+(ii*n8);


    if ((xx1<=mxx) && (xx22>=mmx) && ((y+j)>=mmy) && (y<=mxy))
    {
    for (bits=0; bits<n8; bits++)
     {
       pixels = (kolor & tab_bit[bits]);
      if (kolor_tla0!=255) //ustawione tlo
       {
        if (!pixels)  //[0]
        {
         if (kolor_punktu==kolor_tla0)  //to jest tlo
          {
           if (ignore_bk==0)
            {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                 GrHLine(xx1, xx2, y+l, kolor_punktu_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0, 0, 0, xx1, xx2, y, j, kos, koc);
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,0);
                 }
            }
          }
          else //to nie jest tlo tylko linia
           {
            if (kat0==TRUE)
             {
              for(l=0;l<j;l++)
              GrHLine(xx1, xx2, y+l, kolor_punktu_map);
             }
              else if (j>0)
                 {
                   get_xxyy(x0, y0, 0,0, xx1, xx2, y, j, kos, koc);
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,0);
                 }
           }
        }
         else
           {
             if (kolor_tla==kolor_tla0)  //to jest tlo
              {
               if (ignore_bk==0)
                {
                 if (kat0==TRUE)
                  {
                   for(l=0;l<j;l++)
                   GrHLine(xx1, xx2, y+l, kolor_tla0_map);
                  }
                   else if (j>0)
                    {
                      get_xxyy(x0, y0, 0,0,xx1, xx2, y, j, kos, koc);
                      GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla0_map,0);
                    }
                }
              }
               else //to nie jest tlo tylko linia
                {
                  if (kat0==TRUE)
                   {
                     for(l=0;l<j;l++)
                     GrHLine(xx1, xx2, y+l, kolor_tla_map);
                   }
                    else if (j>0)
                     {
                      get_xxyy(x0, y0,0,0, xx1, xx2, y, j, kos, koc);
                      GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla_map,0);
                     }
                }
           }
       }
        else  //nie jest ustawione tlo
         {
          if (pixels==0)           //kolor 0
           {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                GrHLine(xx1, xx2, y+l, kolor_punktu_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0,0,0, xx1, xx2, y, j, kos, koc);
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_punktu_map,0);
                 }
           }
           else                     //kolor 1
           {
             if (kat0==TRUE)
               {
                for(l=0;l<j;l++)
                GrHLine(xx1, xx2, y+l, kolor_tla_map);
               }
                else if (j>0)
                 {
                   get_xxyy(x0, y0, 0,0,xx1, xx2, y, j, kos, koc);
                   GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_tla_map,0);
                 }
           }
         }

       xx1=xx2;
       if (bits==6) xx2=x+i;
         else
          {
            ii=((bits+2)*i);
            ii/=8;
            xx2=x+ii;
          }
     }
    }
  }

 void prostokat(int x_, int y_, int i, int j, int kolor, int x0, int y0, double kos, double koc)
  {
   int l;
   int x,y;
   int kolor_map;
   int nr;
   int iii;

    kolor_map=get_palette_color(kolor);

    x=x_;
    y=y_;

    if (Check_if_Equal(kos,0.0)==TRUE)
     {
       for(l=0;l<j;l++)
        GrHLine(x, x+i, y+l, kolor_map);
     }
      else if (j>0)
       {

		if (nr_i==0)
		{
			i_i[0]=i;
			j_j[0]=j;
			nr_i=1;
			nr=0;
		}
			else
			{
				//poszukiwanie bitmapy o takich samych wymiarach
				for (iii=0; iii<nr_i; iii++)
				{
					if ((i==i_i[iii]) && (j==j_j[iii]))
					{
						nr=iii;
						break;
					}
				}
				if (iii>=nr_i)
				//nowa bitmapa
				{
					if (nr_i<16)
					{
						i_i[iii]=i;
						j_j[iii]=j;
						nr_i=iii+1;
						nr=iii;
					}
					else 
						{
							nr_i=nr_i;
							return;
						}
				}
		
			}
         get_xxyy(x0, y0, 0,0,x, x+i, y, j, kos, koc);
         GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_map,nr);
       }
  }


 void prostokat8(int x_, int y_, int i, int j, byte_ red, byte_ gre, byte_ blu, int x0, int y0, double kos, double koc)
  {
   int l;
   int x,y;
   int kolor_map;
   int nr;

	kolor_map=red*65536 + gre*256 + blu;

    x=x_;
    y=y_;

    if ((Check_if_Equal(kos,0.0)==TRUE)) // && (1==2))  //TEMPORARY
     {
       for(l=0;l<j;l++)
        GrHLine(x, x+i, y+l, kolor_map);
     }
      else if (j>0)
       {
		nr = 0;

         get_xxyy(x0, y0, 0,0,x, x+i, y, j, kos, koc);
         GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_map,nr);
       }
  }

  void prostokat_8(int x_0, int y_0, int x_, int y_, int i, int j, byte_ red, byte_ gre, byte_ blu, int x0, int y0, double kos, double koc)
  {
   int l;
   int x,y;
   int kolor_map;
   int nr;

   kolor_map=red*65536 + gre*256 + blu;

    x=x_;
    y=y_;

    if ((Check_if_Equal(kos,0.0)==TRUE)) // && (Check_if_Equal(koc, 1.0) == TRUE))
     {
       for(l=0;l<j;l++)
        GrHLine(x-x_0, x+i-x_0, y+l - y_0, kolor_map);
     }
      else if (j>0)
       {
		nr = 0;

         get_xxyy(x0, y0, x_0, y_0, x, x+i, y, j, kos, koc);

         GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_map,nr);
       }
  }

 void prostokat_(int x_0, int y_0, int x_, int y_, int i, int j, int kolor, int x0, int y0, double kos, double koc)
  {
   int l;
   int x,y;
   int kolor_map;
   int nr;
   int iii;

    kolor_map=get_palette_color(kolor);

    x=x_;
    y=y_;

    if (Check_if_Equal(kos,0.0)==TRUE)
     {
       for(l=0;l<j;l++)
        GrHLine(x-x_0, x+i-x_0, y+l - y_0, kolor_map);
     }
      else if (j>0)
       {
        
		if (nr_i==0)
		{
			i_i[0]=i;
			j_j[0]=j;
			nr_i=1;
			nr=0;
		}
			else
			{
			//poszukiwanie bitmapy o takich samych wymiarach
				for (iii=0; iii<nr_i; iii++)
				{
					if ((i==i_i[iii]) && (j==j_j[iii]))
					{
						nr=iii;
						break;
					}
				}
				if (iii>=nr_i)
				//nowa bitmapa
				{
					if (nr_i<16)
					{
						i_i[iii]=i;
						j_j[iii]=j;
						nr_i=iii+1;
						nr=iii;
					}
					else 
					{
						nr_i=nr_i;
						return;
					}
				}
		
			}
         get_xxyy(x0, y0, x_0, y_0, x, x+i, y, j, kos, koc);
         GrFilledPolygon(4,__minx,__miny,__maxx,__maxy,xxyy,kolor_map,nr);
       }
  }


static BOOL add_block_pcx (void)
/*----------------------------*/
{
#ifndef LINUX
  BLOKD s_blockd = BDdef ;
#else
    BLOK s_blockd = BDdef ;
#endif
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = B_PLINE ;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  c_pltype = PL_OTHER ;
  memmove (&(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;

  ptrs_block->n = s_blockd.n + dlugosc_bloku_pcx;
  return TRUE ;
}


int Solid4(double x1,double y1,double x2,double y2, int kolor)
/*---------------------------------------------------------------------------------------------------*/
{

  WIELOKAT Solid=Stdef;

  Solid.warstwa = Current_Layer ;
  Solid.blok = /*0*/ 1;
  Solid.obiektt1 = 0;
  Solid.obiektt2 = O2BlockPline;
  Solid.kolor = kolor ;
  Solid.lp = 8 ;
  Solid.n = 8 + Solid.lp * sizeof (float) ;
  Solid.pcx_solid = 1; /*znacznik bitmapy*/

  Solid.xy [0] = x1 ;
  Solid.xy [1] = y1 ;
  Solid.xy [2] = x2 ;
  Solid.xy [3] = y1 ;
  Solid.xy [4] = x2 ;
  Solid.xy [5] = y2 ;
  Solid.xy [6] = x1 ;
  Solid.xy [7] = y2 ;
  Normalize_Solid (&Solid) ;
  if(dodaj_obiekt(NULL /*(BLOK*)dane*/,&Solid)==NULL) return 0;

  dlugosc_bloku_pcx+=48;
  return 1 ;
}

int DrawSolid4(double x1,double y1,double x2,double y2, int kolor)
/*---------------------------------------------------------------------------------------------------*/
{

  WIELOKAT Solid=Stdef;

  Solid.warstwa = Current_Layer ;
  Solid.blok = /*0*/ 1;
  Solid.obiektt1 = 0;
  Solid.obiektt2 = O2BlockPline;
  Solid.kolor = kolor ;
  Solid.lp = 8 ;
  Solid.n = 8 + Solid.lp * sizeof (float) ;
  Solid.pcx_solid = 1; /*znacznik bitmapy*/

  Solid.xy [0] = x1 ;
  Solid.xy [1] = y1 ;
  Solid.xy [2] = x2 ;
  Solid.xy [3] = y1 ;
  Solid.xy [4] = x2 ;
  Solid.xy [5] = y2 ;
  Solid.xy [6] = x1 ;
  Solid.xy [7] = y2 ;
  Normalize_Solid (&Solid) ;
  rysuj_obiekt((char*)&Solid,COPY_PUT,0);
  return 1 ;
}


//////////////
int LoadPCX(B_PCX *b_pcx, PCXheader *head, char enablepal, int pcx_x0, int pcx_y0, int opcja)
{
    int i;
    //int j;
  int width, width1, height;
  int byte_x;
  int count=0;          // current pixel number in the image
  int total;            // number of total pixels in the image
  COLOR_ key_pal[256];             // current byte read from the image
  byte_ key0;
  int num_bytes;        // number of bytes in a "run"
  int pcx_x;
  int pcx_y;
  BOOL empty_line = TRUE;
  long size_of_file;
  long nr_byte;


  #pragma pack(1)

  #define PAL_SIZE     256

  typedef RGB PALLETE[PAL_SIZE];
  #pragma pack()

  PALLETE Pallete;

  int fp;

  COLOR palette; 
  key0 = 0;

  fp = open (b_pcx->pcx, O_RDWR|O_BINARY) ;
 
  if (fp == -1) return 0;

  if (read(fp,head, sizeof(PCXheader)) != sizeof(PCXheader)) return 0;        // header is 128 bytes

  width  = head->xmax - head->xmin + 1;         // image dimensions...
  height = head->ymax - head->ymin + 1;

  if(head->manufacturer   != 10         // check for errors
   ||head->version         <  5
   ||head->encoding       !=  1
   ||head->bits_per_pixel !=  8
   ||head->xmin  >=  head->xmax
   ||head->ymin  >=  head->ymax)
  {
    close(fp);
    return 0;
  }

  lseek(fp,-768, SEEK_END);             // palette is the last 768 bytes
  
  size_of_file = lseek (fp, 0, 1);


  if (read(fp,&key_pal[0], (256*3)) != (256*3)) return 0;


  for(i=0;i<256;i++)
  {

    palette[i].red = key_pal[i].red>>2;
    palette[i].gre = key_pal[i].gre>>2;
    palette[i].blu = key_pal[i].blu>>2;
  }  

#ifdef fullcolor

#else
  if (opcja==1)  for (i = 0; i < 256; ++i)
       setrgbpalette(i,0,0,0);
  else  for (i = 0; i < 256; ++i)
    setrgbpalette(i,palette[i].red,palette[i].gre,palette[i].blu);
  for (i = 0; i < 256; ++i)
   {
    Pallete[i].r=palette[i].red*4;
    Pallete[i].g=palette[i].gre*4;
    Pallete[i].b=palette[i].blu*4;
    Pallete[i].filler=1;
   }
#endif

  lseek(fp,128,SEEK_SET);

  if(width%2 && width != head->bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }
    
  pcx_x = pcx_x0;
  byte_x = 0;
  pcx_y = pcx_y0;
  
  
  dlugosc_bloku_pcx=0;
   
  nr_byte = 128;
  Procent_0();

  if (read(fp, &key_r, filelength(fp)-896) != (filelength(fp)-896)) return 0;
  close(fp);

  i=-1;
  while(count<total)
  {
    i++;
    nr_byte++;
    
    if(key_r[i]>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_r[i] - 192;    // pixels in this "run" can be up to 63
      i++;
      nr_byte++;
      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
      {
       // write the pixel value unless is is a "filler" byte
        if(width == head->bytes_per_line || count % head->bytes_per_line)
        {
      //    *dest++ = key_r[i];
        //wstawienie punktu w kolorze key
#ifdef fullcolor
        //  kolor_i=GrAllocColor(palette[key_r[i]].red,palette[key_r[i]].gre,palette[key_r[i]].blu);
        //  putpixel__(pcx_x,pcx_y,palette[key_r[i]].red,palette[key_r[i]].gre,palette[key_r[i]].blu);
          kolor_i=GrAllocColor(key_pal[key_r[i]].red,key_pal[key_r[i]].gre,key_pal[key_r[i]].blu);
          PUTPIXEL(pcx_x,pcx_y,kolor_i);
#else
          PUTPIXEL(/*pcx_x0+*/pcx_x,/*pcx_y0+*/pcx_y,key_r[i]);
#endif
          pcx_x+=1;
          byte_x+=1;
          if (byte_x>=width)
           {
            pcx_x=pcx_x0;
            byte_x=0;
            pcx_y+=1;
           }
          num_bytes--;
        }
        count++;
      }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == head->bytes_per_line || count % head->bytes_per_line)
      {
      //  *dest++ = key_r[i];
      //wstawienie punktu w kolorze key
#ifdef fullcolor
        //  kolor_i=GrAllocColor(palette[key_r[i]].red,palette[key_r[i]].gre,palette[key_r[i]].blu);
        //  putpixel__(pcx_x,pcx_y,palette[key_r[i]].red,palette[key_r[i]].gre,palette[key_r[i]].blu);
          kolor_i=GrAllocColor(key_pal[key_r[i]].red,key_pal[key_r[i]].gre,key_pal[key_r[i]].blu);
          PUTPIXEL(pcx_x,pcx_y,kolor_i);
#else
          PUTPIXEL(/*pcx_x0+*/pcx_x,/*pcx_y0+*/pcx_y,key_r[i]);
#endif
      pcx_x+=1;
      byte_x+=1;
      if (byte_x>=width)
       {
        pcx_x=pcx_x0;
        byte_x=0;
        pcx_y+=1;
       }
      }
      count++;
    }
  }

#ifndef nofade
if (opcja==1)
 {
  fade_in2(palette,2);
  delay(1200);
  fade_out2(palette,2);
 }

#endif

  return 1;
}


void procent_(long nr_byte, long size_of_file)
 { 
   int procent;
   double procent_d;
   char str1[30];
   char str2[10];
   int ret;
   
   if (size_of_file == 0) return;
   procent_d=(nr_byte*100);
   procent_d/=size_of_file;
   procent=(int)procent_d;
   if (procent>procent0) 
     { 
       strcpy(str1, _PROCESSED_);

         ret = snprintf(str2, 10, "%d",procent);
       strcat(str1, str2);
       strcat(str1, " %");
       komunikat_str(str1);
       procent0=procent;
     }  
   return;  
 } 

void Procent_0(void)
 {
   procent0=0;
 }


int Draw_pcx(B_PCX *pcx)
{ REAL_PCX *rpcx;
  int kk;
  long dscalex, dscaley;
  long nx, ny;

  if (pcx->kod_obiektu == 1) //PCX
  {
	  rpcx = (REAL_PCX*)pcx->pcx;
	  if (rpcx->header.bits_per_pixel == 8)
	  {
		  dscalex = pikseleX00(pcx->dx);
		  nx = dscalex * (abs(rpcx->header.xmax - rpcx->header.xmin));
		  dscaley = pikseleY00(pcx->dy);
		  ny = dscaley * (abs(rpcx->header.ymax - rpcx->header.ymin));
		  if (((nx>32000) || (ny > 32000)) && (rpcx->header.num_color_planes==1)) //32000
	          kk = Draw_pcx_8(pcx, 1);
	      else kk = Draw_png(pcx, 1);
      }
	  else kk = Draw_pcx_1(pcx);
  }
  else if (pcx->kod_obiektu == 2) //PNG
  {

	  kk = Draw_png(pcx, 2);
  }
  else if (pcx->kod_obiektu == 3) //JPG
  {
      kk = Draw_png(pcx, 3);
  }
  else kk = 0;
 return kk;
}

int Draw_pcx_virtual(B_PCX *pcx, int opcje, int ctx_x0, int ctx_y0, int ctx_ww, int ctx_wh)
{ REAL_PCX *rpcx;
  int kk;

  if (pcx->kod_obiektu == 1) //PCX
  {
	  rpcx = (REAL_PCX*)pcx->pcx;
	  if (rpcx->header.bits_per_pixel == 8)
	      kk = Draw_png_virtual(pcx, 1, ctx_x0, ctx_y0, ctx_ww, ctx_wh, 1);
	  else kk = Draw_pcx_1_virtual(pcx, ctx_x0, ctx_y0, ctx_ww, ctx_wh);
  }
  else if (pcx->kod_obiektu == 2) //PNG
  {
	  kk = Draw_png_virtual(pcx, 1, ctx_x0, ctx_y0, ctx_ww, ctx_wh, 2);
  }
  else if (pcx->kod_obiektu == 3) //JPG
  {
      kk = Draw_png_virtual(pcx, 1, ctx_x0, ctx_y0, ctx_ww, ctx_wh, 3);
  }
  else kk = 0;
 return kk;
}


unsigned char var_bit[8]={1,2,4,8,16,32,64,128};

int transform_color(int kolor, int j, char pixel_per_byte)
/*------------------------------------------------------*/
{
  if (pixel_per_byte==1) return kolor;
   else if (kolor!=0)
    {
      return (kolor & var_bit[j]);
    }
     else return 0;
}

int fix_color(B_PCX *pcx, int kolor)
{
  if ((pcx->set_foreground) && (kolor != pcx->background))
   {
     return (pcx->foreground);
   }
  return kolor;
}


int scan_pcx_strips_8(B_PCX *pcx)
{
  unsigned char key_i;
  int i;
  int aa = 0, bb = 0;
  int cc=0,dd=0,ee=0;
  double SkalaX,SkalaY;
  REAL_PCX *rpcx;
  int width, width1, height;
  int pcx_x,pcx_y,pcx_x0,pcx_y0;
  int byte_x;
  unsigned char key0;
  int count=0;          // current pixel number in the image
  int total;
  char *key_rr, *key_rr0;
  int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
  double SkalaXX, SkalaYY;
  int SSkalaX, SSkalaY;
  double width_d;
  double height_d;
  int kolor_tla0, kolor_tla, kolor_punktu, ignore_bk;
  int width_r;
  int numer_wiersza;
  STRIPS *strips;
  int no_strip;
  int height_100;

  rpcx = (REAL_PCX*) pcx->pcx;
  strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS)+2);

  key0 = 0;

  width_d  = rpcx->header.xmax - rpcx->header.xmin+1;         // image dimensions...
  width_r= rpcx->header.xmax - rpcx->header.xmin+1;
  width=(int)width_d;
  height_d = rpcx->header.ymax - rpcx->header.ymin; //  + 1;
  height=(int)height_d;

  height_100=height/100;
  no_strip=0;

  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  8
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=Wyslij_par_ekranu(0);
  mxx=Wyslij_par_ekranu(1);
  mxy=Wyslij_par_ekranu(2);
  mmy=Wyslij_par_ekranu(3);

  pcx_x0 = pikseleX0(pcx->x);
  byte_x = 0;
  pcx_y0 = pikseleY0(pcx->y)+mmy;
  pcx_x = pcx_x0;
  pcx_y = pcx_y0;

  dlugosc_bloku_pcx=0;
  numer_wiersza=0;

   if (pcx->ignore_background==1) ignore_bk=1; else ignore_bk=0;
   kolor_tla0=pcx->background;
   kolor_tla=pcx->color_key[1];
   if (pcx->set_foreground==1) kolor_punktu=pcx->foreground; else kolor_punktu=pcx->color_key[0];

  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);
  key_rr0 = key_rr;

  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width_d)-pikseleX0d(0.0);
  SkalaX=fabs(SkalaXX/width_d)*1000000.0;
  SkalaYY=pikseleY0d(pcx->dy*height_d)-pikseleY0(0.0);
  SkalaY=fabs(SkalaYY/height_d)*1000000.0;

  SSkalaX=(int)SkalaX;
  SSkalaY=(int)SkalaY;

  ee+=SSkalaY;
  bb=ee/1000000;
  ee-=bb*1000000;

  while(count<total && pcx_y<mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          dd+=SSkalaX;
          aa=dd/1000000;
          dd-=(aa*1000000);

         // pcx_x+=aa;
          byte_x+=1;
          if (byte_x >= width)
           {
            pcx_x=pcx_x0;
            pcx_y+=bb;

            if (numer_wiersza>0)
             {
              if ((numer_wiersza % height_100)==0 ) //reszta z dzielenia przez zero
               {
                if (no_strip<100)
                 {
                   strips->strip[no_strip].wiersz=numer_wiersza;
                   strips->strip[no_strip].adr=key_rr-key_rr0+1;  //+1
                   strips->strip[no_strip].count=count;
                   no_strip++;
                 }
               }
             }

            numer_wiersza++;

            dd=0;

            ee+=SSkalaY;
            bb=ee/1000000;
            ee-=bb*1000000;

            byte_x=0;
           }
          num_bytes--;
        }

        count++;
        if (byte_x<width && byte_x>0)
          {
            pcx_x+=aa;
          }
       }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key
      dd+=SSkalaX;
      aa=dd/1000000;
      dd-=aa*1000000;

      byte_x+=1;
      if (byte_x >= width )
       {
        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (numer_wiersza>0)
         {
          if ((numer_wiersza % height_100)==0 ) //reszta z dzielenia przez zero
           {
             if (no_strip<100)
              {
               strips->strip[no_strip].wiersz=numer_wiersza;
               strips->strip[no_strip].adr=key_rr-key_rr0+1;  //+1
               strips->strip[no_strip].count=count;
               no_strip++;
              }
           }
         }
        numer_wiersza++;

        dd=0;
        byte_x=0;

        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;
       }
      }
      count++;
      if (byte_x<width && byte_x>0)
       {
        pcx_x+=aa;
       }
    }
   key_rr++;
  }
return 1;
}

BITMAP *fixup_loaded_bitmap(BITMAP *bmp, PALETTE pal, int bpp)
{
	BITMAP *b2;
	ASSERT(bmp);

	b2 = create_bitmap_ex(bpp, bmp->w, bmp->h);
	if (!b2) {
		destroy_bitmap(bmp);
		return NULL;
	}

	if (bpp == 8) {
		RGB_MAP *old_map = rgb_map;

		if (pal)
			generate_optimized_palette(bmp, pal, NULL);
		else
			pal = _current_palette;

		rgb_map = malloc(sizeof(RGB_MAP));
		if (rgb_map != NULL)
			create_rgb_table(rgb_map, pal, NULL);

		blit(bmp, b2, 0, 0, 0, 0, bmp->w, bmp->h);

		if (rgb_map != NULL)
			free(rgb_map);
		rgb_map = old_map;
	}
	else if (bitmap_color_depth(bmp) == 8) {
		select_palette(pal);
		blit(bmp, b2, 0, 0, 0, 0, bmp->w, bmp->h);
		unselect_palette();
	}
	else {
		blit(bmp, b2, 0, 0, 0, 0, bmp->w, bmp->h);
	}

	destroy_bitmap(bmp);

	return b2;
}


BITMAP *load_memory_pcx(AL_CONST void *buffer, PALETTE *pal)
{
	BITMAP *b;
	PALETTE tmppal;
	int want_palette = TRUE;
	int c;
	int width, height;
	int bpp, bytes_per_line;
	int xx, po;
	int x, y;
	unsigned char ch;
	int dest_depth;
	char *pcx;
	char bit_per_pixel;
	char marker;
	PCXheader *pcx_header;
	RGB pal1[16];

	/* we really need a palette */
	if (!pal) {
		want_palette = FALSE;
		pal = tmppal;
	}

	pcx = buffer;
	pcx_header = pcx;

	bit_per_pixel = pcx_header->bits_per_pixel;

	if (bit_per_pixel != 8) {         // we like 8 bit color planes 
		return NULL;
	}

	width = pcx_header->xmax - pcx_header->xmin + 1;
	height = pcx_header->ymax - pcx_header->ymin + 1;

	for (c = 0; c < 16; c++) {           /* read the 16 color palette */

		pal1[c].r = pcx_header->ega_palette[c * 3] / 4;

		pal1[c].g = pcx_header->ega_palette[c * 3 + 1] / 4;

		pal1[c].b = pcx_header->ega_palette[c * 3 + 2] / 4;

	}

	bpp = pcx_header->num_color_planes * 8;

	if ((bpp != 8) && (bpp != 24)) {
		return NULL;
	}

	dest_depth = _color_load_depth(bpp, FALSE);

	bytes_per_line = pcx_header->bytes_per_line;

	b = create_bitmap_ex(bpp, width, height);
	if (!b) {
		return NULL;
	}

	*allegro_errno = 0;

	pcx += sizeof(PCXheader);

	for (y = 0; y < height; y++) {       /* read RLE encoded PCX data */
		x = xx = 0;
#ifdef ALLEGRO_LITTLE_ENDIAN
		po = _rgb_r_shift_24 / 8;
#elif defined ALLEGRO_BIG_ENDIAN
		po = 2 - _rgb_r_shift_24 / 8;
#elif !defined SCAN_DEPEND
#error endianess not defined
#endif

		while (x < bytes_per_line*bpp / 8) {
			memmove(&ch, pcx, 1);
			pcx++;
			if ((ch & 0xC0) == 0xC0) {
				c = (ch & 0x3F);
				memmove(&ch, pcx, 1);
				pcx++;
			}
			else
				c = 1;

			if (bpp == 8) {
				while (c--) {
					if (x < b->w)
						b->line[y][x] = ch;
					x++;
				}
			}
			else {
				while (c--) {
					if (xx < b->w)
						b->line[y][xx * 3 + po] = ch;
					x++;
					if (x == bytes_per_line) {
						xx = 0;
#ifdef ALLEGRO_LITTLE_ENDIAN
						po = _rgb_g_shift_24 / 8;
#elif defined ALLEGRO_BIG_ENDIAN
						po = 2 - _rgb_g_shift_24 / 8;
#elif !defined SCAN_DEPEND
#error endianess not defined
#endif
					}
					else if (x == bytes_per_line * 2) {
						xx = 0;
#ifdef ALLEGRO_LITTLE_ENDIAN
						po = _rgb_b_shift_24 / 8;
#elif defined ALLEGRO_BIG_ENDIAN
						po = 2 - _rgb_b_shift_24 / 8;
#elif !defined SCAN_DEPEND
#error endianess not defined
#endif
					}
					else
						xx++;
				}
			}
		}
	}

	if (bpp == 8) {                  /* look for a 256 color palette */
		marker = *pcx;
		pcx++;
		if (marker == 12) {
			
			for (c = 0; c < 256; c++) {
				ch = *pcx;
				tmppal[c].r = (ch >> 2);
				pcx++;
				ch = *pcx;
				tmppal[c].g =  (ch >> 2);
				pcx++;
				ch = *pcx;
				tmppal[c].b = (ch >> 2);
			    pcx++;
			}
			pal = tmppal;
			pcx += (256 * 3);
		}
	}

	if (*allegro_errno) {
		destroy_bitmap(b);
		return NULL;
	}


	if (dest_depth != bpp) {
		/* restore original palette except if it comes from the bitmap */
		if ((bpp != 8) && (!want_palette))
			pal = NULL;

		b = _fixup_loaded_bitmap(b, pal, dest_depth);
	}

	/* construct a fake palette if 8-bit mode is not involved */
	if ((bpp != 8) && (dest_depth != 8) && want_palette)
		generate_332_palette(pal);

	return b;
}

#define BIGNOD 1e200


int Draw_png(B_PCX *pcx, int kod_obiektu)
{
	BITMAP *png, *png1;
	PALETTE pal;
	char *png_data;
	PCXheader *head;
	int width, height;
	double aspect1;
	long dx_scr, dy_scr;
	struct viewporttype view_port;
	int pcx_x, pcx_y, pcx_x0, pcx_y0, pcx_x_max, pcx_y_max;
	int byte_x;
	double kos, koc, kos1, koc1;
	int x_[4], y_[4];
    int i;
	double mmxd[4], mmyd[4];
	long mmx1, mmy1, mxx1, mxy1, x_min, x_max, y_min, y_max;
	long png_mem;
	double half_width, half_height;
	double x0, y0;
	double pcx_x1, pcx_y1;
	int pcx_x01, pcx_y01;
	double scalex;
    int zero = 0;
    int del_x, del_y;
    int sizew, sizeh;
    double unitx[4], unity[4];
    double dsizew, dsizeh;
    double ddelx, ddely;

	aspect1 = fabs(1 - sk_x);

	head = pcx->pcx;
	width = (head->xmax - head->xmin);
	height = (head->ymax - head->ymin);
	png_data = pcx->pcx + sizeof(PCXheader);
	png_mem = width * height * 4 + 100;  // 4 BYTES per pixel , 100 is reserve

    png1 = NULL;

	if (kod_obiektu == 2)
	{
		png1 = load_memory_png(png_data, png_mem, &pal);
	}
	else if (kod_obiektu == 1)
	{
		png1 = load_memory_pcx(pcx->pcx, &pal);
	}
    else if (kod_obiektu == 3)
    {
        png1 = load_memory_jpg(png_data, png_mem, &pal);
    }

	if (png1 == NULL) return 1;

    if (pcx->v_flip == 1)
    {
        png = create_bitmap_ex(32, png1->w * (pcx->dx / pcx->dy), png1->h);
        if (png == NULL) return 1;
        draw_sprite_v_flip(png, png1, 0, 0);
        destroy_bitmap(png1);
        png1 = png;
    }

    if (pcx->h_flip == 1)
    {
        png = create_bitmap_ex(32, png1->w * (pcx->dx / pcx->dy), png1->h);
        if (png == NULL) return 1;
        draw_sprite_h_flip(png, png1, 0, 0);
        destroy_bitmap(png1);
        png1 = png;
    }
	
	scalex = pcx->dx / pcx->dy;

	if (Check_if_Equal(pcx->dx, pcx->dy) == FALSE)
	{
		png = create_bitmap_ex(32, png1->w*(pcx->dx / pcx->dy), png1->h);

		if (png == NULL) return 1;

		stretch_blit(png1, png, 0, 0, png1->w, png1->h, 0, 0, png->w, png->h);
		destroy_bitmap(png1);
	}
	else png = png1;

	if (png != NULL)
	{
		mmx = Wyslij_par_ekranu(0);
		mxx = Wyslij_par_ekranu(1);
		mxy = Wyslij_par_ekranu(2);
		mmy = Wyslij_par_ekranu(3);

		dx_scr = (mxx - mmx)*aspect1;
		dy_scr = (mxy - mmy)*aspect1;

		getviewsettings(&view_port);

		mmy -= view_port.top;
		mxy -= view_port.top;

		pcx_x0 = pikseleX0(pcx->x);
		byte_x = 0;
		pcx_y0 = pikseleY0(pcx->y) + mmy;
		pcx_x_max = pikseleX0(pcx->x + (pcx->dx*width));
		pcx_y_max = pikseleY0(pcx->y - (pcx->dy*height)) + mmy;

		pcx_x = pcx_x0;
		pcx_y = pcx_y0;//50;//pYp-pYk;
		kos = sin(Pi2 - pcx->kat);
		koc = cos(Pi2 - pcx->kat);

		if (Check_if_Equal(pcx->kat, 0.0) == FALSE)
		{

			kos1 = sin(-(pcx->kat));
			koc1 = cos(-(pcx->kat));

			x_[0] = mmx; y_[0] = mmy;
			x_[1] = mmx; y_[1] = mxy;
			x_[2] = mxx; y_[2] = mxy;
			x_[3] = mxx; y_[3] = mmy;

			for (i = 0; i < 4; i++)
			{
				mmxd[i] = pcx_x + (x_[i] - pcx_x)* koc - (y_[i] - pcx_y) * kos;
				mmyd[i] = pcx_y + (x_[i] - pcx_x)* kos + (y_[i] - pcx_y) * koc;
			}
#ifdef LINUX
#ifndef MACOS
            qsort(mmxd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
            qsort(mmyd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
#else
			qsort(mmxd, 4, sizeof(double), qsort_by_val);
			qsort(mmyd, 4, sizeof(double), qsort_by_val);
#endif
#else
			//good for Clion
			qsort(mmxd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);
			qsort(mmyd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);

			//good for VS
			//qsort(mmxd, 4, sizeof(double),(int(*)(const double*, const double*)) qsort_by_val);
			//qsort(mmyd, 4, sizeof(double),(int(*)(const double*, const double*)) qsort_by_val);
#endif


			mmx = (int)mmxd[0] - dx_scr;    //dodano dx_scr z uwagi na aspect ratio
			mmy = (int)mmyd[0] - dy_scr;
			mxx = (int)mmxd[3] + dx_scr;
			mxy = (int)mmyd[3] + dy_scr;

		}

		mmx1 = pcx_x0;
		mmy1 = pcx_y0;
		mxx1 = pcx_x_max;
		mxy1 = pcx_y_max;

		x_min = mmx1;
		if (mxx1 < x_min) x_min = mxx1;
		x_max = mmx1;
		if (mxx1 > x_max) x_max = mxx1;

		y_min = mmy1;
		if (mxy1 < y_min) y_min = mxy1;
		y_max = mmy1;
		if (mxy1 > y_max) y_max = mxy1;
		
		double dscale = (double)pikseleX00(pcx->dy * 10000)/10000;

		fixed scale = ftofix(dscale);

		//rotation is around central point, so point origin must be moved
		half_width = pikseleX00(pcx->dx*width) / 2.0;
		half_height = pikseleY00(pcx->dy*height) / 2.0;
		x0 = (double)(pcx_x0 + half_width);
		y0 = (double)pcx_y0 + half_height;

		Rotate_Point(kos, koc, x0, y0, (double)pcx_x0, (double)pcx_y0, &pcx_x1, &pcx_y1);

		pcx_x01 = pcx_x0 + (pcx_x0 - pcx_x1);
		pcx_y01 = pcx_y0 - (pcx_y1 - pcx_y0);

        if (head->bits_per_pixel == 32)
        {
            unitx[0] = 0.0;
            unity[0] = 0.0;

            dsizew = (int)((float)png->w * dscale);
            dsizeh = (int)((float)png->h * dscale);

            Rotate_Point(kos, koc, 0.0, 0.0, 0.0, dsizeh, &unitx[1], &unity[1]);
            Rotate_Point(kos, koc, 0.0, 0.0, dsizew, dsizeh, &unitx[2], &unity[2]);
            Rotate_Point(kos, koc, 0.0, 0.0, dsizew, 0.0, &unitx[3], &unity[3]);

            double minx = BIGNOD;
            double miny = BIGNOD;
            double maxx = -BIGNOD;
            double maxy = -BIGNOD;

            for (i = 0; i < 4; i++)
            {
                if (unitx[i] < minx) minx = unitx[i];
                if (unitx[i] > maxx) maxx = unitx[i];

                if (unity[i] < miny) miny = unity[i];
                if (unity[i] > maxy) maxy = unity[i];
            }


            minx = pcx_x0 + minx; 
            maxx = pcx_x0 + maxx; 
            miny = pcx_y0 + miny; 
            maxy = pcx_y0 + maxy;

            int sizew_0 = maxx - minx;
            int sizeh_0 = maxy - miny;

            long X1 = 0, Y1 = 0, X2 = pXk - pXp, Y2 = pYp - pYk;

            if (maxx < X1) return 0;
            if (minx > X2) return 0;

            if (maxy < Y1) return 0;
            if (miny > Y2) return 0;

            ddely = 0.0;
            ddelx = 0.0;

            if (minx < X1)
            {
                ddelx = X1 - minx;
                minx = X1;
            }
            if (maxx > X2) maxx = X2;

            if (miny < Y1)
            {
                ddely = miny - Y1;
                miny = Y1;
            }
            if (maxy > X2) maxy = Y2;

            int sizew_ = maxx - minx;
            int sizeh_ = maxy - miny;
            
            sizew = sizew_;
            del_x = (int)((float)((sizew_0 - dsizew)/2.0+0.5 - ddelx));
            
            sizeh = sizeh_;
            del_y = (int)((float)((sizeh_0 - dsizeh) / 2.0 + 0.5 + ddely));
           

            BITMAP* buffer = create_bitmap_ex(32, sizew, sizeh);
            if (buffer != NULL)
            {
                clear_to_color(buffer, bitmap_mask_color(buffer));
                rotate_scaled_sprite(buffer, png, del_x, del_y, ftofix(-pcx->kat * 256.0f / (2.0 * PI)), scale); 
                set_alpha_blender();
                draw_trans_sprite(screenplay, buffer, pcx_x01 - del_x, pcx_y01 - del_y);
                destroy_bitmap(buffer);
                set_trans_blender(0, 0, 0, TRANSLUCENCY);
                set_mode_solid();
            }
            else //ignore transparency
            {
               rotate_scaled_sprite(screenplay, png, pcx_x01, pcx_y01, ftofix(-pcx->kat * 256.0f / (2.0 * PI)), scale);
            }
        }
        else
        {
           rotate_scaled_sprite(screenplay, png, pcx_x01, pcx_y01, ftofix(-pcx->kat * 256.0f / (2.0 * PI)), scale); 
        }
        destroy_bitmap(png);

		return 0;
	}
	else return 1;
}


int Draw_pcx_8(B_PCX *pcx, int opcja)
{
 unsigned char key_i;
 int  i;
 int  aa = 0, bb = 0;
 int cc=0,dd=0,ee=0;
 double SkalaX,SkalaY;
 REAL_PCX *rpcx;
 int width, width1, height;
 int  pcx_x,pcx_y,pcx_yyy,pcx_x0,pcx_y0,pcx_x_max,pcx_y_max;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
 BOOL empty_line = TRUE;
 long nr_byte;
 double SkalaXX, SkalaYY;
 int SSkalaX, SSkalaY;
 double kos,koc, kos1, koc1;
 double mmxd[4],mmyd[4];
 long mxx1,mxy1,mmx1,mmy1,x_min,x_max,y_min,y_max;
 int x_[4], y_[4];
 STRIPS *strips;
 int no_strip;
 BOOL find_strip;
 BOOL stripping;
 int numer_wiersza, numer_wiersza1, numer_wiersza2;
 long dx_scr, dy_scr;
 double aspect1;
 int iii;

  struct viewporttype view_port;

  byte_ *red, *gre, *blu;

  aspect1=fabs(1-sk_x);

  rpcx = (REAL_PCX*) pcx->pcx;
  if (pcx->markers==1)
   {
    if (pcx->stripped==0)
      {
        if (scan_pcx_strips_8(pcx)==1)
         {
           pcx->stripped=1;
           strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS) + 2);
         }
      }
   }

  key0 = 0;
  width  = rpcx->header.xmax - rpcx->header.xmin + 1;         // image dimensions...
  height = rpcx->header.ymax - rpcx->header.ymin + 1;

  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  8
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=Wyslij_par_ekranu(0);
  mxx=Wyslij_par_ekranu(1);
  mxy=Wyslij_par_ekranu(2);
  mmy=Wyslij_par_ekranu(3);

  dx_scr=(mxx-mmx)*aspect1;
  dy_scr=(mxy-mmy)*aspect1;

  getviewsettings(&view_port);

  mmy -= view_port.top;
  mxy -= view_port.top;

  pcx_x0 = pikseleX0(pcx->x);
  byte_x = 0;
  pcx_y0 = pikseleY0(pcx->y)+mmy;
  pcx_x_max=pikseleX0(pcx->x+(pcx->dx*width1));
  pcx_y_max=pikseleY0(pcx->y-(pcx->dy*height))+mmy;

  pcx_x = pcx_x0;
  pcx_y = pcx_y0;
  kos=sin(pcx->kat);
  koc=cos(pcx->kat);

  if (Check_if_Equal(pcx->kat,0.0)==FALSE)
   {
    kos1=sin(-(pcx->kat));
    koc1=cos(-(pcx->kat));

    x_[0]=mmx; y_[0]=mmy;
    x_[1]=mmx; y_[1]=mxy;
    x_[2]=mxx; y_[2]=mxy;
    x_[3]=mxx; y_[3]=mmy;

    for (i=0; i<4; i++)
     {
  
       mmxd[i] =  pcx_x + (x_[i]-pcx_x)* koc - (y_[i]-pcx_y) * kos;
       mmyd[i] =  pcx_y + (x_[i]-pcx_x)* kos + (y_[i]-pcx_y) * koc;
     }

#ifdef LINUX
#ifndef MACOS
      qsort(mmxd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
      qsort(mmyd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
#else
  	qsort(mmxd, 4, sizeof(double), qsort_by_val);
  	qsort(mmyd, 4, sizeof(double), qsort_by_val);
#endif
#else
  	//good for Clion
  	qsort(mmxd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);
  	qsort(mmyd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);

  	//good for VS
    //qsort(mmxd, 4, sizeof(double),(int(*)(const double*, const double*)) qsort_by_val);
    //qsort(mmyd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
#endif

	
	mmx=(int)mmxd[0]-dx_scr;    //dodano dx_scr z uwagi na aspect ratio
    mmy=(int)mmyd[0]-dy_scr;
    mxx=(int)mmxd[3]+dx_scr;
    mxy=(int)mmyd[3]+dy_scr;

   }

   mmx1=pcx_x0;
   mmy1=pcx_y0;
   mxx1=pcx_x_max;
   mxy1=pcx_y_max;

   x_min=mmx1;
   if (mxx1<x_min) x_min=mxx1;
   x_max=mmx1;
   if (mxx1>x_max) x_max=mxx1;

   y_min=mmy1;
   if (mxy1<y_min) y_min=mxy1;
   y_max=mmy1;
   if (mxy1>y_max) y_max=mxy1;

   if ((mmx>x_max) || (mxx<x_min) || (mmy>y_max) || (mxy<y_min)) return 0;

  dlugosc_bloku_pcx=0;
   
  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);
  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width)-pikseleX0d(0.0);
  SkalaX=fabs(SkalaXX/width)*1000000.0;
  SkalaYY=pikseleY0d(pcx->dy*height)-pikseleY0d(0.0);
  SkalaY=fabs(SkalaYY/height)*1000000.0;

  SSkalaX=(int)SkalaX;
  SSkalaY=(int)SkalaY;

  ee+=SSkalaY;
  bb=ee/1000000;
  ee-=bb*1000000;

  //odszukanie najblizszego paska obrazu
  stripping=TRUE;  // FALSE;  //!!!!!!!!!!!!!

   if ((pcx->markers==1) && (pcx->stripped==1) && (stripping==TRUE))
    {
     strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS) + 2);
     numer_wiersza=0;
     pcx_yyy=pcx_y0;
     while (pcx_yyy<mmy)
      {
       pcx_yyy+=bb;
       ee+=SSkalaY;
       bb=ee/1000000;
       ee-=bb*1000000;
       numer_wiersza++;
      }
     if (numer_wiersza>0)
      {
        //poszukiwanie najblizszego paska
        no_strip=0;
        find_strip=FALSE;
        while (no_strip<100)
         {
           if (strips->strip[no_strip].wiersz>numer_wiersza)
            {
             if (no_strip>0)
             {
              numer_wiersza1=strips->strip[no_strip-1].wiersz;
              key_rr+=strips->strip[no_strip-1].adr;
              count+=strips->strip[no_strip-1].count+1;
              ///////
              ee=0;
              ee+=SSkalaY;
              bb=ee/1000000;
              ee-=bb*1000000;
              ///////
              numer_wiersza2=0;
              while (numer_wiersza2<numer_wiersza1)
               {
                pcx_y+=bb;
                ee+=SSkalaY;
                bb=ee/1000000;
                ee-=bb*1000000;
                numer_wiersza2++;
               }
              find_strip=TRUE;
             }
             break;
            }
           no_strip++;
         }
        if (find_strip==FALSE)
         {
          ///////
          ee=0;
          ee+=SSkalaY;
          bb=ee/1000000;
          ee-=bb*1000000;
          ///////
         }
      }
      else
       {
        ///////
        ee=0;
        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;
        ///////
       }
    }
     else
      {
       ///////
       ee=0;
       ee+=SSkalaY;
       bb=ee/1000000;
       ee-=bb*1000000;
       ///////
      }

  //////////////////

  while(count<total && pcx_y<mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;


	  red=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3+2);
	  gre=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3+1);
	  blu=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3);
      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          dd+=SSkalaX;
          aa=dd/1000000;
          dd-=(aa*1000000);

          byte_x+=1;
          if (byte_x >= width)
           {
            if ((pcx->ignore_background==0) || (pcx->color_key[key_i]!=pcx->background))
             {
               if (pcx_x<=mxx && (pcx_x+aa)>=mmx && (pcx_y+bb)>=mmy && pcx_y<=mxy) prostokat8(pcx_x,pcx_y/*+mmy*/,aa,bb, *red, *gre, *blu, pcx_x0,pcx_y0,kos,koc);
             }

            pcx_x=pcx_x0;
            pcx_y+=bb;
            if (pcx_y>mxy)
			{
				for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
                nr_i=0;
                return 0;
			}
            dd=0;

            ee+=SSkalaY;
            bb=ee/1000000;
            ee-=bb*1000000;

            byte_x=0;
           }
          num_bytes--;
        }
        count++;
              if (byte_x<=width && byte_x>0)
              {
               if (pcx_x<=mxx && (pcx_x+aa)>=mmx && (pcx_y+bb)>=mmy && pcx_y<=mxy)
               {
                if ((pcx->ignore_background==0) || (pcx->color_key[key_i]!=pcx->background))
                    prostokat8(pcx_x,pcx_y/*+mmy*/,aa,bb,*red, *gre, *blu,pcx_x0,pcx_y0,kos,koc);
               }
                pcx_x+=aa;
              }
       }
    }
    else
    {
      red=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3+2);
	  gre=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3+1);
	  blu=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3);
     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key

      dd+=SSkalaX;
      aa=dd/1000000;
      dd-=aa*1000000;

      if ((pcx->ignore_background==0) || (pcx->color_key[key_i]!=pcx->background))
        {
	     if (pcx_x<=mxx && (pcx_x+aa)>=mmx && (pcx_y+bb)>=mmy && pcx_y<=mxy) prostokat8(pcx_x,pcx_y/*+mmy*/,aa,bb,*red, *gre, *blu,pcx_x0,pcx_y0,kos,koc);
        }
      byte_x+=1;
      if (byte_x >= width )
       {
        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (pcx_y>mxy)
		{
			for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
            nr_i=0;
            return 0;
		}

        dd=0;
        byte_x=0;

        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;

       }
      }
      count++;
      if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
		   for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
           nr_i=0;
           return 1;
  	      }
       }
              if (byte_x<=width && byte_x>0)
              {
              if (pcx_x<=mxx && (pcx_x+aa)>=mmx && (pcx_y+bb)>=mmy && pcx_y<=mxy)
               {
                if ((pcx->ignore_background==0) || (pcx->color_key[key_i]!=pcx->background))
                   prostokat8(pcx_x,pcx_y/*+mmy*/,aa,bb,*red, *gre, *blu, pcx_x0,pcx_y0,kos,koc);
               }
               pcx_x+=aa;
              }
    }
   key_rr++;
  }

for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
nr_i=0;
return 1;
}


int Draw_png_virtual(B_PCX *pcx, int opcja, int ctx_x0, int ctx_y0, int ctx_ww, int ctx_wh, int kod_obiektu)
{
	BITMAP *png, *png1;
	PALETTE pal;
	char *png_data;
	PCXheader *head;
	int width, height;
	int kk;
	double aspect1;
	long dx_scr, dy_scr;
	int pcx_x, pcx_y, pcx_x0, pcx_y0, pcx_x_max, pcx_y_max;
	int byte_x;
	double kos, koc, kos1, koc1;
	int x_[4], y_[4];
	int i;
	double mmxd[4], mmyd[4];
	long mmx1, mmy1, mxx1, mxy1, x_min, x_max, y_min, y_max;
	long png_mem;
	double half_width, half_height;
	double x0, y0;
	double pcx_x1, pcx_y1;
	int pcx_x01, pcx_y01;
	double scalex;
    int del_x, del_y;
    int sizew, sizeh;
    double unitx[4], unity[4];
    double dsizew, dsizeh;
    double ddelx, ddely;

	aspect1 = fabs(1 - sk_x);

	head = pcx->pcx;
	width = (head->xmax - head->xmin);
	height = (head->ymax - head->ymin);
	png_data = pcx->pcx + sizeof(PCXheader);
	png_mem = width * height * 4 + 100;  //4 BYTES per pixel , 100 is reserve
	if (kod_obiektu==2) png1 = load_memory_png(png_data, png_mem, &pal);
	else if (kod_obiektu == 1) png1 = load_memory_pcx(pcx->pcx, &pal);
    else if (kod_obiektu == 3) png1 = load_memory_jpg(png_data, png_mem, &pal);

    if (pcx->v_flip == 1)
    {
        png = create_bitmap_ex(32, png1->w * (pcx->dx / pcx->dy), png1->h);
        if (png == NULL) return 1;
        draw_sprite_v_flip(png, png1, 0, 0);
        destroy_bitmap(png1);
        png1 = png;
    }

    if (pcx->h_flip == 1)
    {
        png = create_bitmap_ex(32, png1->w * (pcx->dx / pcx->dy), png1->h);
        if (png == NULL) return 1;
        draw_sprite_h_flip(png, png1, 0, 0);
        destroy_bitmap(png1);
        png1 = png;
    }

	scalex = pcx->dx / pcx->dy;

	if (Check_if_Equal(pcx->dx, pcx->dy) == FALSE)
	{
		png = create_bitmap(png1->w*(pcx->dx / pcx->dy), png1->h);
		stretch_blit(png1, png, 0, 0, png1->w, png1->h, 0, 0, png->w, png->h);
		destroy_bitmap(png1);
	}
	else png = png1;


	if (png != NULL)
	{

		mmx = ctx_x0;

		mmy = ctx_y0;

		mxx = mmx + ctx_ww;
		mxy = mmy + ctx_wh;


		dx_scr = (mxx - mmx)*aspect1;
		dy_scr = (mxy - mmy)*aspect1;

		pcx_x0 = pikseleX00(pcx->x);
		pcx_y0 = pikseleY00((double)FormatY) - pikseleY00(pcx->y);

		pcx_x_max = pikseleX00(pcx->x + (pcx->dx*width));
		pcx_y_max = pikseleY00(pcx->y - (pcx->dy*height));

		byte_x = 0;

		pcx_x = pcx_x0;
		pcx_y = pcx_y0;
		kos = sin(Pi2 - pcx->kat);
		koc = cos(Pi2 - pcx->kat);


		if (Check_if_Equal(pcx->kat, 0.0) == FALSE)
		{

			kos1 = sin(-(pcx->kat));
			koc1 = cos(-(pcx->kat));

			x_[0] = mmx; y_[0] = mmy;
			x_[1] = mmx; y_[1] = mxy;
			x_[2] = mxx; y_[2] = mxy;
			x_[3] = mxx; y_[3] = mmy;

			for (i = 0; i < 4; i++)
			{

				mmxd[i] = pcx_x + (x_[i] - pcx_x)* koc - (y_[i] - pcx_y) * kos;
				mmyd[i] = pcx_y + (x_[i] - pcx_x)* kos + (y_[i] - pcx_y) * koc;
			}

#ifdef LINUX
#ifndef MACOS
            qsort(mmxd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
            qsort(mmyd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
#else
			qsort(mmxd, 4, sizeof(double), qsort_by_val);
			qsort(mmyd, 4, sizeof(double), qsort_by_val);
#endif
#else
			//good for Clion
			qsort(mmxd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);
			qsort(mmyd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);

			//good for VS
			//qsort(mmxd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
			//qsort(mmyd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
#endif


			mmx = (int)mmxd[0] - dx_scr;    //dodano dx_scr z uwagi na aspect ratio
			mmy = (int)mmyd[0] - dy_scr;
			mxx = (int)mmxd[3] + dx_scr;
			mxy = (int)mmyd[3] + dy_scr;

		}

		mmx1 = pcx_x0;
		mmy1 = pcx_y0;
		mxx1 = pcx_x_max;
		mxy1 = pcx_y_max;

		x_min = mmx1;
		if (mxx1 < x_min) x_min = mxx1;
		x_max = mmx1;
		if (mxx1 > x_max) x_max = mxx1;

		y_min = mmy1;
		if (mxy1 < y_min) y_min = mxy1;
		y_max = mmy1;
		if (mxy1 > y_max) y_max = mxy1;

		double dscale = (double)pikseleX00(pcx->dy * 10000) / 10000;

		fixed scale = ftofix(dscale);

		//rotation is around central point, so point origin must be moved
		half_width = pikseleX00(pcx->dx*width) / 2.0;
		half_height = pikseleY00(pcx->dy*height) / 2.0;
		x0 = (double)(pcx_x0 + half_width);
		y0 = (double)pcx_y0 + half_height;

		Rotate_Point(kos, koc, x0, y0, (double)pcx_x0, (double)pcx_y0, &pcx_x1, &pcx_y1);

		pcx_x01 = pcx_x0 + (pcx_x0 - pcx_x1);
		pcx_y01 = pcx_y0 - (pcx_y1 - pcx_y0);

        if (head->bits_per_pixel == 32)
        {
            unitx[0] = 0.0;
            unity[0] = 0.0;

            dsizew = (int)((float)png->w * dscale);
            dsizeh = (int)((float)png->h * dscale);

            Rotate_Point(kos, koc, 0.0, 0.0, 0.0, dsizeh, &unitx[1], &unity[1]);
            Rotate_Point(kos, koc, 0.0, 0.0, dsizew, dsizeh, &unitx[2], &unity[2]);
            Rotate_Point(kos, koc, 0.0, 0.0, dsizew, 0.0, &unitx[3], &unity[3]);

            double minx = BIGNOD;
            double miny = BIGNOD;
            double maxx = -BIGNOD;
            double maxy = -BIGNOD;

            for (i = 0; i < 4; i++)
            {
                if (unitx[i] < minx) minx = unitx[i];
                if (unitx[i] > maxx) maxx = unitx[i];

                if (unity[i] < miny) miny = unity[i];
                if (unity[i] > maxy) maxy = unity[i];
            }


            minx = pcx_x0 + minx;
            maxx = pcx_x0 + maxx;
            miny = pcx_y0 + miny;
            maxy = pcx_y0 + maxy;

            int sizew_0 = maxx - minx;
            int sizeh_0 = maxy - miny;

            long X1 = ctx_x0, Y1 = ctx_y0, X2 = ctx_x0+ctx_ww, Y2 = ctx_y0+ctx_wh;

            if (maxx < X1) return 0;
            if (minx > X2) return 0;

            if (maxy < Y1) return 0;
            if (miny > Y2) return 0;

            ddely = 0.0;
            ddelx = 0.0;

            if (minx < X1)
            {
                ddelx = X1 - minx;
                minx = X1;
            }
            if (maxx > X2) maxx = X2;

            if (miny < Y1)
            {
                ddely = miny - Y1;
                miny = Y1;
            }
            if (maxy > X2) maxy = Y2;

            int sizew_ = maxx - minx;
            int sizeh_ = maxy - miny;

            sizew = sizew_;
            del_x = (int)((float)((sizew_0 - dsizew) / 2.0 + 0.5 - ddelx));

            sizeh = sizeh_;
            del_y = (int)((float)((sizeh_0 - dsizeh) / 2.0 + 0.5 + ddely));


            BITMAP* buffer = create_bitmap_ex(32, sizew, sizeh);
            if (buffer != NULL)
            {
                clear_to_color(buffer, bitmap_mask_color(buffer));
                rotate_scaled_sprite(buffer, png, del_x, del_y, ftofix(-pcx->kat * 256.0f / (2.0 * PI)), scale);
                set_alpha_blender();
                draw_trans_sprite(screenplay, buffer, pcx_x01 - del_x - ctx_x0, pcx_y01 - del_y - ctx_y0);
                destroy_bitmap(buffer);
                set_trans_blender(0, 0, 0, TRANSLUCENCY);
                set_mode_solid();
            }
            else //ignore transparency
            {
                rotate_scaled_sprite(screenplay, png, pcx_x01 - ctx_x0, pcx_y01 - ctx_y0, ftofix(-pcx->kat * 256.0f / (2.0 * PI)), scale);
            }
        }
        else
        {
            rotate_scaled_sprite(screenplay, png, pcx_x01 - ctx_x0, pcx_y01 - ctx_y0, ftofix(-pcx->kat * 256.0f / (2.0 * PI)), scale); // ftofix(pikseleX00(pcx->dx)));
        }
		destroy_bitmap(png);
		kk = 0;

        return 0;
	}
	else return 1;
}


int Draw_pcx_8_virtual(B_PCX *pcx, int opcja, int ctx_x0, int ctx_y0, int ctx_ww, int ctx_wh)
{
 unsigned char key_i;
 int  i;
 int  aa = 0, bb = 0;
 int cc=0,dd=0,ee=0;
 double SkalaX,SkalaY;
 REAL_PCX *rpcx;
 int width, width1, height;
 int pcx_x,pcx_y,pcx_yyy,pcx_x0,pcx_y0;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
 double SkalaXX, SkalaYY;
  int SSkalaX, SSkalaY;
  double kos,koc, kos1, koc1;
  double mmxd[4],mmyd[4];
  long mxx1,mxy1,mmx1,mmy1,x_min,x_max,y_min,y_max;
  int x_[4], y_[4];
  STRIPS *strips;
  int no_strip;
  BOOL find_strip;
  BOOL stripping;
  int numer_wiersza, numer_wiersza1, numer_wiersza2;
  long dx_scr, dy_scr;
  double aspect1;
  int iii;
  COLOR *pcxpalette;
  byte_ *red, *gre, *blu;

  aspect1=fabs(1-sk_x);

  rpcx = (REAL_PCX*) pcx->pcx;
  if (pcx->markers==1)
   {
    if (pcx->stripped==0)
      {
        if (scan_pcx_strips_8(pcx)==1)
         {
           pcx->stripped=1;
           strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS) + 2);
         }
      }
   }

  pcxpalette=(COLOR *)(pcx->pcx+pcx->len_pcx-768);

  key0 = 0;
  width  = rpcx->header.xmax - rpcx->header.xmin + 1;         // image dimensions...
  height = rpcx->header.ymax - rpcx->header.ymin + 1;

  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  8
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=ctx_x0;

  mmy=ctx_y0;

  mxx=mmx + ctx_ww;
  mxy=mmy + ctx_wh;
  
  dx_scr=(mxx-mmx)*aspect1;
  dy_scr=(mxy-mmy)*aspect1;

  pcx_x0 = pikseleX00(pcx->x);
  pcx_y0 = pikseleY00((double)FormatY) - pikseleY00(pcx->y);

  byte_x = 0;
  pcx_x = pcx_x0;
  pcx_y = pcx_y0;
  kos=sin(pcx->kat);
  koc=cos(pcx->kat);

  if (Check_if_Equal(pcx->kat,0.0)==FALSE)
   {

    kos1=sin(-(pcx->kat));
    koc1=cos(-(pcx->kat));

    x_[0]=mmx; y_[0]=mmy;
    x_[1]=mmx; y_[1]=mxy;
    x_[2]=mxx; y_[2]=mxy;
    x_[3]=mxx; y_[3]=mmy;

    for (i=0; i<4; i++)
     {
       mmxd[i] =  pcx_x + (x_[i]-pcx_x)* koc - (y_[i]-pcx_y) * kos;
       mmyd[i] =  pcx_y + (x_[i]-pcx_x)* kos + (y_[i]-pcx_y) * koc;
     }

#ifdef LINUX
#ifndef MACOS
      qsort(mmxd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
      qsort(mmyd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
#else
  	qsort(mmxd, 4, sizeof(double), qsort_by_val);
  	qsort(mmyd, 4, sizeof(double), qsort_by_val);
#endif
#else
  	//good for Clion
  	qsort(mmxd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);
  	qsort(mmyd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);

  	//good for VS
    //qsort(mmxd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
    //qsort(mmyd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
#endif


 	mmx=(int)mmxd[0]-dx_scr;
    mmy=(int)mmyd[0]-dy_scr;
    mxx=(int)mmxd[3]+dx_scr;
    mxy=(int)mmyd[3]+dy_scr;

   }

   mmx1=pcx_x0;
   mmy1=pcx_y0;
   mxx1=pikseleX00(pcx->x+(pcx->dx*width1));
   mxy1=mmy1 + pikseleY00(pcx->y+(pcx->dy*height));

   x_min=mmx1;
   if (mxx1<x_min) x_min=mxx1;
   x_max=mmx1;
   if (mxx1>x_max) x_max=mxx1;

   y_min=mmy1;
   if (mxy1<y_min) y_min=mxy1;
   y_max=mmy1;
   if (mxy1>y_max) y_max=mxy1;

   if ((mmx>x_max) || (mxx<x_min) || (mmy>y_max) || (mxy<y_min)) return 0;

  dlugosc_bloku_pcx=0;
   
  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);
  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width)-pikseleX0d(0.0);
  SkalaX=fabs(SkalaXX/width)*1000000.0;
  SkalaYY=pikseleY0d(pcx->dy*height)-pikseleY0d(0.0);
  SkalaY=fabs(SkalaYY/height)*1000000.0;

  SSkalaX=(int)SkalaX;
  SSkalaY=(int)SkalaY;


  ee+=SSkalaY;
  bb=ee/1000000;
  ee-=bb*1000000;

  //odszukanie najblizszego paska obrazu
  stripping=TRUE;  // FALSE;  //!!!!!!!!!!!!!

   if ((pcx->markers==1) && (pcx->stripped==1) && (stripping==TRUE))
    {
     strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS) + 2);
     numer_wiersza=0;
     pcx_yyy=pcx_y0;
     while (pcx_yyy<mmy)
      {
       pcx_yyy+=bb;
       ee+=SSkalaY;
       bb=ee/1000000;
       ee-=bb*1000000;
       numer_wiersza++;
      }
     if (numer_wiersza>0)
      {
        //poszukiwanie najblizszego paska
        no_strip=0;
        find_strip=FALSE;
        while (no_strip<100)
         {
           if (strips->strip[no_strip].wiersz>numer_wiersza)
            {
             if (no_strip>0)
             {
              numer_wiersza1=strips->strip[no_strip-1].wiersz;
              key_rr+=strips->strip[no_strip-1].adr;

              count+=strips->strip[no_strip-1].count+1;
              ///////
              ee=0;
              ee+=SSkalaY;
              bb=ee/1000000;
              ee-=bb*1000000;
              ///////
              numer_wiersza2=0;
              while (numer_wiersza2<numer_wiersza1)
               {
                pcx_y+=bb;
                ee+=SSkalaY;
                bb=ee/1000000;
                ee-=bb*1000000;
                numer_wiersza2++;
               }
              find_strip=TRUE;
             }
             break;
            }
           no_strip++;
         }
        if (find_strip==FALSE)
         {
          ///////
          ee=0;
          ee+=SSkalaY;
          bb=ee/1000000;
          ee-=bb*1000000;
          ///////
         }
      }
      else
       {
        ///////
        ee=0;
        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;
        ///////
       }
    }
     else
      {
       ///////
       ee=0;
       ee+=SSkalaY;
       bb=ee/1000000;
       ee-=bb*1000000;
       ///////
      }

  while(count<total && pcx_y<=mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;


	  red=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3+2);
	  gre=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3+1);
	  blu=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3);

	  
      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          dd+=SSkalaX;
          aa=dd/1000000;
          dd-=(aa*1000000);

          byte_x+=1;
          if (byte_x >= width)
           {
            if ((pcx->ignore_background==0) || (pcx->color_key[key_i]!=pcx->background))
             {
              if (pcx_x<=mxx && (pcx_x+aa)>=mmx && (pcx_y+bb)>=mmy && pcx_y<=mxy) prostokat_8(ctx_x0,ctx_y0,pcx_x,pcx_y/*+mmy*/,aa,bb, *red, *gre, *blu, pcx_x0,pcx_y0,kos,koc);
             }

            pcx_x=pcx_x0;
            pcx_y+=bb;
            if (pcx_y>mxy)
			{
				for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
                nr_i=0;
                return 0;
			}
            dd=0;

            ee+=SSkalaY;
            bb=ee/1000000;
            ee-=bb*1000000;

            byte_x=0;
           }
          num_bytes--;
        }
        count++;
              if (byte_x<width && byte_x>0)
              {
               if (pcx_x<=mxx && (pcx_x+aa)>=mmx && (pcx_y+bb)>=mmy && pcx_y<=mxy)
               {
                if ((pcx->ignore_background==0) || (pcx->color_key[key_i]!=pcx->background))
                    prostokat_8(ctx_x0,ctx_y0,pcx_x,pcx_y/*+mmy*/,aa,bb, *red, *gre, *blu,pcx_x0,pcx_y0,kos,koc);
               }
                pcx_x+=aa;
              }
       }
    }
    else
    {
      red=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3+2);
	  gre=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3+1);
	  blu=(byte_ *)(pcx->pcx+pcx->len_pcx-768+key_i*3);

     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key

      dd+=SSkalaX;
      aa=dd/1000000;
      dd-=aa*1000000;

      if ((pcx->ignore_background==0) || (pcx->color_key[key_i]!=pcx->background))
             {
               if (pcx_x<=mxx && (pcx_x+aa)>=mmx && (pcx_y+bb)>=mmy && pcx_y<=mxy) prostokat_8(ctx_x0,ctx_y0,pcx_x,pcx_y/*+mmy*/,aa,bb,*red, *gre, *blu,pcx_x0,pcx_y0,kos,koc);
             }
      byte_x+=1;
      if (byte_x >= width )
       {
        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (pcx_y>mxy)
		{
			for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
            nr_i=0;
            return 0;
		}

        dd=0;
        byte_x=0;

        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;

       }
      }
      count++;
      if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
		   for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
           nr_i=0;
           return -1;   //przerwano rysowanie
  	      }
       }
              if (byte_x<width && byte_x>0)
              {
              if (pcx_x<=mxx && (pcx_x+aa)>=mmx && (pcx_y+bb)>=mmy && pcx_y<=mxy)
               {
                if ((pcx->ignore_background==0) || (pcx->color_key[key_i]!=pcx->background))
                   prostokat_8(ctx_x0,ctx_y0,pcx_x,pcx_y/*+mmy*/,aa,bb, *red, *gre, *blu,pcx_x0,pcx_y0,kos,koc);
               }
               pcx_x+=aa;
              }
    }
   key_rr++;
  }
for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
nr_i=0;
return 1;
}


int Draw_pcx_1_0(B_PCX *pcx)
{
 unsigned char key_i;
 int  i;
 int  aa = 0, bb = 0;
 int cc=0,dd=0,ee=0;
 double SkalaX,SkalaY;
 REAL_PCX *rpcx;
 int width, width1, height;
 int pcx_x,pcx_y,pcx_x0,pcx_y0;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
 double SkalaXX, SkalaYY;
  int SSkalaX, SSkalaY;
  double width_d;
  double height_d;
  int kolor_tla0, kolor_tla, kolor_punktu, ignore_bk;
  int width_r;
  double kos, koc;

  rpcx = (REAL_PCX*) pcx->pcx;
  key0 = 0;

  width_d  = ((rpcx->header.xmax - rpcx->header.xmin)/8)+1.5;         // image dimensions...
  width_r= (rpcx->header.xmax - rpcx->header.xmin)+1;
  width=(int)width_d;
  height_d = rpcx->header.ymax - rpcx->header.ymin; //  + 1;
  height=(int)height_d;


  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  1
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=Wyslij_par_ekranu(0);
  mxx=Wyslij_par_ekranu(1);
  mxy=Wyslij_par_ekranu(2);
  mmy=Wyslij_par_ekranu(3);

  pcx_x0 = pikseleX0(pcx->x);
  byte_x = 0;
  pcx_y0 = pikseleY0(pcx->y)+mmy;
  pcx_x = pcx_x0;
  pcx_y = pcx_y0;//50;//pYp-pYk;
  kos=sin(pcx->kat);
  koc=cos(pcx->kat);

  dlugosc_bloku_pcx=0;

   if (pcx->ignore_background==1) ignore_bk=1; else ignore_bk=0;
   kolor_tla0=pcx->background;
   kolor_tla=pcx->color_key[1];
   if (pcx->set_foreground==1) kolor_punktu=pcx->foreground; else kolor_punktu=pcx->color_key[0];

  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);// + 28;

  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width_d)-pikseleX0d(0.0);
  SkalaX=fabs(SkalaXX/width_d)*1000000.0;
  SkalaYY=pikseleY0d(pcx->dy*height_d)-pikseleY0d(0.0);
  SkalaY=fabs(SkalaYY/height_d)*1000000.0;

  SSkalaX=(int)SkalaX;
  SSkalaY=(int)SkalaY;

  ee+=SSkalaY;
  bb=ee/1000000;
  ee-=bb*1000000;

  while(count<total && pcx_y<mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          dd+=(SSkalaX*8);
          aa=dd/1000000;
          dd-=(aa*1000000);

          byte_x+=1;
          if (byte_x >= width)
           {

            pcx_x=pcx_x0;
            pcx_y+=bb;
            if (pcx_y>mxy) return 0;
            dd=0;

            ee+=SSkalaY;
            bb=ee/1000000;
            ee-=bb*1000000;

            byte_x=0;
           }
          num_bytes--;
        }

        count++;

              if (byte_x<width && byte_x>0)
               {

                 prostokat2(pcx_x,pcx_y/*+mmy*/,aa,bb,8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
                 pcx_x+=aa;
               }

       }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key

      dd+=(SSkalaX*8);
      aa=dd/1000000;
      dd-=aa*1000000;


      prostokat2(pcx_x,pcx_y/*+mmy*/,aa,bb,8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);

      byte_x+=1;
      if (byte_x >= width )
       {
        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (pcx_y>mxy) return 0;

        dd=0;

        byte_x=0;

        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;

       }
      }
      count++;
      if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
           return 1;
  	      }
       }

              if (byte_x<width && byte_x>0)
               {
                 prostokat2(pcx_x,pcx_y/*+mmy*/,aa,bb,8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
                 pcx_x+=aa;
               }

    }
   key_rr++;
  }

return 1;
}


int scan_pcx_strips_1(B_PCX *pcx)
{
  unsigned char key_i;
  int i;
  int aa = 0, bb = 0;
  int cc=0,dd=0,ee=0;
  double SkalaX,SkalaY;
  REAL_PCX *rpcx;
  int width, width1, height;
  int pcx_x,pcx_y,pcx_x0,pcx_y0;
  int byte_x;
  unsigned char key0;
  int count=0;          // current pixel number in the image
  int total;
  char *key_rr, *key_rr0;
  int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
  double SkalaXX, SkalaYY;
  int SSkalaX, SSkalaY;
  double width_d;
  double height_d;
  int kolor_tla0, kolor_tla, kolor_punktu, ignore_bk;
  int width_r;
  int numer_wiersza;
  STRIPS *strips;
  int no_strip;
  int height_100;

  rpcx = (REAL_PCX*) pcx->pcx;
  strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS)+2);

  key0 = 0;

  width_d  = ((rpcx->header.xmax - rpcx->header.xmin)/8)+1.5;         // image dimensions...
  width_r= (rpcx->header.xmax - rpcx->header.xmin)+1;
  width=(int)width_d;
  height_d = rpcx->header.ymax - rpcx->header.ymin; //  + 1;
  height=(int)height_d;

  height_100=height/100;
  no_strip=0;


  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  1
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=Wyslij_par_ekranu(0);
  mxx=Wyslij_par_ekranu(1);
  mxy=Wyslij_par_ekranu(2);
  mmy=Wyslij_par_ekranu(3);

  pcx_x0 = pikseleX0(pcx->x);
  byte_x = 0;
  pcx_y0 = pikseleY0(pcx->y)+mmy;
  pcx_x = pcx_x0;
  pcx_y = pcx_y0;

  dlugosc_bloku_pcx=0;
  numer_wiersza=0;


   if (pcx->ignore_background==1) ignore_bk=1; else ignore_bk=0;
   kolor_tla0=pcx->background;
   kolor_tla=pcx->color_key[1];
   if (pcx->set_foreground==1) kolor_punktu=pcx->foreground; else kolor_punktu=pcx->color_key[0];

   
  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);
  key_rr0 = key_rr;

  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width_d)-pikseleX0d(0.0);
  SkalaX=fabs(SkalaXX/width_d)*1000000.0;
  SkalaYY=pikseleY0d(pcx->dy*height_d)-pikseleY0(0.0);
  SkalaY=fabs(SkalaYY/height_d)*1000000.0;

  SSkalaX=(int)SkalaX;
  SSkalaY=(int)SkalaY;

  ee+=SSkalaY;
  bb=ee/1000000;
  ee-=bb*1000000;

  while(count<total && pcx_y<mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          dd+=(SSkalaX*8);
          aa=dd/1000000;
          dd-=(aa*1000000);

          byte_x+=1;
          if (byte_x >= width)
           {
            pcx_x=pcx_x0;
            pcx_y+=bb;

            if (numer_wiersza>0)
             {
              if ((numer_wiersza % height_100)==0 ) //reszta z dzielenia przez zero
               {
                if (no_strip<100)
                 {
                   strips->strip[no_strip].wiersz=numer_wiersza;
                   strips->strip[no_strip].adr=key_rr-key_rr0+1;  //+1
                   strips->strip[no_strip].count=count;
                   no_strip++;
                 }
               }
             }


            numer_wiersza++;

            dd=0;

            ee+=SSkalaY;
            bb=ee/1000000;
            ee-=bb*1000000;

            byte_x=0;
           }
          num_bytes--;
        }

        count++;
        if (byte_x<width && byte_x>0)
          {
            pcx_x+=aa;
          }
       }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key
      dd+=(SSkalaX*8);
      aa=dd/1000000;
      dd-=aa*1000000;

      byte_x+=1;
      if (byte_x >= width )
       {
        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (numer_wiersza>0)
         {
          if ((numer_wiersza % height_100)==0 ) //reszta z dzielenia przez zero
           {
             if (no_strip<100)
              {
               strips->strip[no_strip].wiersz=numer_wiersza;
               strips->strip[no_strip].adr=key_rr-key_rr0+1;  //+1
               strips->strip[no_strip].count=count;
               no_strip++;
              }
           }
         }
        numer_wiersza++;

        dd=0;
        byte_x=0;

        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;
       }
      }
      count++;
      if (byte_x<width && byte_x>0)
       {
        pcx_x+=aa;
       }
    }
   key_rr++;
  }
return 1;
}


int Draw_pcx_1(B_PCX *pcx)
{
 unsigned char key_i;
 int i;
 int  aa = 0, bb = 0;
 int cc=0,dd=0,ee=0;
 double SkalaX,SkalaY;
 REAL_PCX *rpcx;
 int width, width1, height;
 int pcx_x,pcx_y,pcx_yyy,pcx_x0,pcx_y0,pcx_x_max,pcx_y_max;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
 double SkalaXX, SkalaYY;
  int SSkalaX, SSkalaY;
  double width_d;
  double height_d;
  int kolor_tla0, kolor_tla, kolor_punktu, ignore_bk;
  int width_r;
  int numer_wiersza, numer_wiersza1, numer_wiersza2;
  STRIPS *strips;
  int no_strip;
  BOOL find_strip;
  double kos, koc, kos1, koc1;
  double mmxd[4],mmyd[4];
  long mxx1,mxy1,mmx1,mmy1,x_min,x_max,y_min,y_max;
  int x_[4], y_[4];
  BOOL stripping;
  long dx_scr, dy_scr;
  double aspect1;

  struct viewporttype view_port;

  int iii;

  aspect1=fabs(1-sk_x);
  rpcx = (REAL_PCX*) pcx->pcx;

  if (pcx->markers==1)
   {
    if (pcx->stripped==0)
      {
        if (scan_pcx_strips_1(pcx)==1)
         {
           pcx->stripped=1;
           strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS) + 2);
         }
      }
   }


  key0 = 0;

  width_d = ((rpcx->header.xmax - rpcx->header.xmin) / 8) + 1.5;         // image dimensions...  +1.5
  width_r = (rpcx->header.xmax - rpcx->header.xmin) + 1;
  width=(int)width_d;
  height_d = rpcx->header.ymax - rpcx->header.ymin; //  + 1;
  height=(int)height_d;


  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  1
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=Wyslij_par_ekranu(0);
  mxx=Wyslij_par_ekranu(1);
  mxy=Wyslij_par_ekranu(2);
  mmy=Wyslij_par_ekranu(3);
  dx_scr=(mxx-mmx)*aspect1;
  dy_scr=(mxy-mmy)*aspect1;

  getviewsettings(&view_port);

  mmy -= view_port.top;
  mxy -= view_port.top;

  pcx_x0 = pikseleX0(pcx->x);
  byte_x = 0;
  pcx_y0 = pikseleY0(pcx->y)+mmy;

  pcx_x_max=pikseleX0(pcx->x+(pcx->dx*width_d*8));
  pcx_y_max=pikseleY0(pcx->y-(pcx->dy*height_d))+mmy;

  pcx_x = pcx_x0;
  pcx_y = pcx_y0;
  kos=sin(pcx->kat);
  koc=cos(pcx->kat);

  if (Check_if_Equal(pcx->kat,0.0)==FALSE)
   {

    kos1=sin(-(pcx->kat));
    koc1=cos(-(pcx->kat));

    x_[0]=mmx; y_[0]=mmy;
    x_[1]=mmx; y_[1]=mxy;
    x_[2]=mxx; y_[2]=mxy;
    x_[3]=mxx; y_[3]=mmy;

	
    for (i=0; i<4; i++)
     {
	   mmxd[i] =  pcx_x + (x_[i]-pcx_x)* koc - (y_[i]-pcx_y) * kos;
       mmyd[i] =  pcx_y + (x_[i]-pcx_x)* kos + (y_[i]-pcx_y) * koc;
     }

#ifdef LINUX
#ifndef MACOS
      qsort(mmxd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
      qsort(mmyd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
#else
  	qsort(mmxd, 4, sizeof(double), qsort_by_val);
  	qsort(mmyd, 4, sizeof(double), qsort_by_val);
#endif
#else
  	//good for Clion
  	qsort(mmxd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);
  	qsort(mmyd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);

  	//good for VS
    //qsort(mmxd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
    //qsort(mmyd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
#endif

    mmx=(int)mmxd[0]-dx_scr;
    mmy=(int)mmyd[0]-dy_scr;
    mxx=(int)mmxd[3]+dx_scr;
    mxy=(int)mmyd[3]+dy_scr;
   }

   mmx1=pcx_x0;
   mmy1=pcx_y0;
   mxx1=pcx_x_max;  
   mxy1=pcx_y_max;  

   x_min=mmx1;
   if (mxx1<x_min) x_min=mxx1;
   x_max=mmx1;
   if (mxx1>x_max) x_max=mxx1;

   y_min=mmy1;
   if (mxy1<y_min) y_min=mxy1;
   y_max=mmy1;
   if (mxy1>y_max) y_max=mxy1;

   if ((mmx>x_max) || (mxx<x_min) || (mmy>y_max) || (mxy<y_min)) return 0;

   dlugosc_bloku_pcx=0;

   if (pcx->ignore_background==1) ignore_bk=1; else ignore_bk=0;
   kolor_tla0=pcx->background;
   kolor_tla=pcx->color_key[1];
   if (pcx->set_foreground==1) kolor_punktu=pcx->foreground; else kolor_punktu=pcx->color_key[0];

  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);// + 28;

  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width_d)-pikseleX0d(0.0);
  SkalaX=fabs(SkalaXX/width_d)*1000000.0;
  SkalaYY=pikseleY0d(pcx->dy*height_d)-pikseleY0d(0.0);
  SkalaY=fabs(SkalaYY/height_d)*1000000.0;

  SSkalaX=(int)SkalaX;
  SSkalaY=(int)SkalaY;

  ///////
  ee+=SSkalaY;
  bb=ee/1000000;
  ee-=bb*1000000;
  ///////

  //odszukanie najblizszego paska obrazu
  stripping=TRUE;  //!!!!!!!!!!!!!

   if ((pcx->markers==1) && (pcx->stripped==1) && (stripping==TRUE))
    {
     strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS) + 2);
     //TEST
     if (strips->strip[0].wiersz<0)
     {
         pcx->stripped=0;
         return 0;
     }

     numer_wiersza=0;
     pcx_yyy=pcx_y0;
     while (pcx_yyy<mmy)
      {
       pcx_yyy+=bb;
       ee+=SSkalaY;
       bb=ee/1000000;
       ee-=bb*1000000;
       numer_wiersza++;
      }
     if (numer_wiersza>0)
      {
        //poszukiwanie najblizszego paska
        no_strip=0;
        find_strip=FALSE;
        while (no_strip<100)
         {
           if (strips->strip[no_strip].wiersz>numer_wiersza)
            {
             if (no_strip>0)
             {
              numer_wiersza1=strips->strip[no_strip-1].wiersz;
              key_rr+=strips->strip[no_strip-1].adr;
              count+=strips->strip[no_strip-1].count+1;
              ///////
              ee=0;
              ee+=SSkalaY;
              bb=ee/1000000;
              ee-=bb*1000000;
              ///////
              numer_wiersza2=0;
              while (numer_wiersza2<numer_wiersza1)
               {
                pcx_y+=bb;
                ee+=SSkalaY;
                bb=ee/1000000;
                ee-=bb*1000000;
                numer_wiersza2++;
               }
              find_strip=TRUE;
             }
             break;
            }
           no_strip++;
         }
        if (find_strip==FALSE)
         {
          ///////
          ee=0;
          ee+=SSkalaY;
          bb=ee/1000000;
          ee-=bb*1000000;
          ///////
         }
      }
      else
       {
        ///////
        ee=0;
        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;
        ///////
       }
    }
     else
      {
       ///////
       ee=0;
       ee+=SSkalaY;
       bb=ee/1000000;
       ee-=bb*1000000;
       ///////
      }

  while(count<total && pcx_y<mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          dd+=(SSkalaX*8);
          aa=dd/1000000;
          dd-=(aa*1000000);

          byte_x+=1;
          if (byte_x >= width)
           {
            prostokat2(pcx_x,pcx_y/*+mmy*/,aa,bb, 8-((byte_x/*-1*/)*8-width_r) ,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
               
            pcx_x=pcx_x0;
            pcx_y+=bb;
            if (pcx_y>mxy) 
			{ for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
              nr_i=0;
			  return 0;
			}
            dd=0;

            ee+=SSkalaY;
            bb=ee/1000000;
            ee-=bb*1000000;

            byte_x=0;
           }
          num_bytes--;
        }

        count++;
              if (byte_x<width && byte_x>0)
               {
                 prostokat2(pcx_x,pcx_y/*+mmy*/,aa,bb,8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
                 pcx_x+=aa;
               }
       }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key

      dd+=(SSkalaX*8);
      aa=dd/1000000;
      dd-=aa*1000000;

      byte_x+=1;
      if (byte_x >= width )
       {
        prostokat2(pcx_x,pcx_y/*+mmy*/,aa,bb,8-((byte_x/*-1*/) * 8 - width_r), key_i, kolor_tla0, kolor_tla, kolor_punktu, ignore_bk, pcx_x0, pcx_y0, kos, koc);

        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (pcx_y>mxy) 
		{
			for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
            nr_i=0;
			return 0;
		}

        dd=0;
        byte_x=0;

        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;

       }
      }
      count++;
      if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
		   for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
           nr_i=0;
           return 1;
  	      }
       }
              if (byte_x<width && byte_x>0)
               {
                 prostokat2(pcx_x,pcx_y/*+mmy*/,aa,bb,8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
                 pcx_x+=aa;
               }
    }
   key_rr++;
  }
for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
nr_i=0;
return 1;
}


int Draw_pcx_1_virtual(B_PCX *pcx, int ctx_x0, int ctx_y0, int ctx_ww, int ctx_wh)
{
 unsigned char key_i;
 int  i;
 int  aa = 0, bb = 0;
 int cc=0,dd=0,ee=0;
 double SkalaX,SkalaY;
 REAL_PCX *rpcx;
 int width, width1, height;
 int pcx_x,pcx_y,pcx_yyy,pcx_x0,pcx_y0;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
 double SkalaXX, SkalaYY;
  int SSkalaX, SSkalaY;
  double width_d;
  double height_d;
  int kolor_tla0, kolor_tla, kolor_punktu, ignore_bk;
  int width_r;
  int numer_wiersza, numer_wiersza1, numer_wiersza2;
  STRIPS *strips;
  int no_strip;
  BOOL find_strip;
  double kos, koc, kos1, koc1;
  double mmxd[4],mmyd[4];
  long mxx1,mxy1,mmx1,mmy1,x_min,x_max,y_min,y_max;
  int x_[4], y_[4];
  BOOL stripping;
  long dx_scr, dy_scr;
  double aspect1;
  int iii;

  aspect1=fabs(1-sk_x);
  rpcx = (REAL_PCX*) pcx->pcx;

  if (pcx->markers==1)
   {
    if (pcx->stripped==0)
      {
        if (scan_pcx_strips_1(pcx)==1)
         {
           pcx->stripped=1;
           strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS) + 2);
         }
      }
   }

  key0 = 0;

  width_d  = ((rpcx->header.xmax - rpcx->header.xmin)/8)+1.5;         // image dimensions...
  width_r= (rpcx->header.xmax - rpcx->header.xmin)+1;
  width=(int)width_d;
  height_d = rpcx->header.ymax - rpcx->header.ymin; //  + 1;
  height=(int)height_d;


  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  1
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
	return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=ctx_x0;
  mmy=ctx_y0;
  mxx=mmx + ctx_ww;
  mxy=mmy + ctx_wh;

  dx_scr=(mxx-mmx)*aspect1;
  dy_scr=(mxy-mmy)*aspect1;

  pcx_x0 = pikseleX00(pcx->x);
  byte_x = 0;
  pcx_y0 = pikseleY00((double)FormatY) - pikseleY00(pcx->y);
  pcx_x = pcx_x0;
  pcx_y = pcx_y0;
  kos=sin(pcx->kat);
  koc=cos(pcx->kat);

  fixkat=pcx->kat*256/360;

  if (Check_if_Equal(pcx->kat,0.0)==FALSE)
   {

    kos1=sin(-(pcx->kat));
    koc1=cos(-(pcx->kat));

    x_[0]=mmx; y_[0]=mmy;
    x_[1]=mmx; y_[1]=mxy;
    x_[2]=mxx; y_[2]=mxy;
    x_[3]=mxx; y_[3]=mmy;

    for (i=0; i<4; i++)
     {
       mmxd[i] =  pcx_x + (x_[i]-pcx_x)* koc - (y_[i]-pcx_y) * kos;
       mmyd[i] =  pcx_y + (x_[i]-pcx_x)* kos + (y_[i]-pcx_y) * koc;
     }

#ifdef LINUX
#ifndef MACOS
      qsort(mmxd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
      qsort(mmyd, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
#else
  	qsort(mmxd, 4, sizeof(double), qsort_by_val);
  	qsort(mmyd, 4, sizeof(double), qsort_by_val);
#endif
#else
  	//good for Clion
  	qsort(mmxd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);
  	qsort(mmyd, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);

  	//good for VS
    //qsort(mmxd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
    //qsort(mmyd, 4, sizeof(double), (int(*)(const double*, const double*)) qsort_by_val);
#endif

	mmx=(int)mmxd[0]-dx_scr;
    mmy=(int)mmyd[0]-dy_scr;
    mxx=(int)mmxd[3]+dx_scr;
    mxy=(int)mmyd[3]+dy_scr;
   }

   mmx1=pcx_x0;
   mmy1=pcx_y0;
   mxx1=pikseleX00(pcx->x+(pcx->dx*width*8));
   mxy1=mmy1 + pikseleY00(pcx->dy*height);

   x_min=mmx1;
   if (mxx1<x_min) x_min=mxx1;
   x_max=mmx1;
   if (mxx1>x_max) x_max=mxx1;

   y_min=mmy1;
   if (mxy1<y_min) y_min=mxy1;
   y_max=mmy1;
   if (mxy1>y_max) y_max=mxy1;

   if ((mmx>x_max) || (mxx<x_min) || (mmy>y_max) || (mxy<y_min)) return 0;

   dlugosc_bloku_pcx=0;

   if (pcx->ignore_background==1) ignore_bk=1; else ignore_bk=0;
   kolor_tla0=pcx->background;
   kolor_tla=pcx->color_key[1];
   if (pcx->set_foreground==1) kolor_punktu=pcx->foreground; else kolor_punktu=pcx->color_key[0];

  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);// + 28;

  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width_d)-pikseleX0d(0.0);
  SkalaX=fabs(SkalaXX/width_d)*1000000.0;
  SkalaYY=pikseleY0d(pcx->dy*height_d)-pikseleY0d(0.0);
  SkalaY=fabs(SkalaYY/height_d)*1000000.0;

  SSkalaX=(int)SkalaX;
  SSkalaY=(int)SkalaY;

  ///////
  ee+=SSkalaY;
  bb=ee/1000000;
  ee-=bb*1000000;
  ///////

  //odszukanie najblizszego paska obrazu
  stripping=TRUE;  // FALSE;  //!!!!!!!!!!!!!

   if ((pcx->markers==1) && (pcx->stripped==1) && (stripping==TRUE))
    {
     strips=(STRIPS *) ((char *)pcx + sizeof(NAGLOWEK) + pcx->n - sizeof(STRIPS) + 2);
     numer_wiersza=0;
     pcx_yyy=pcx_y0;
     while (pcx_yyy<mmy)
      {
       pcx_yyy+=bb;
       ee+=SSkalaY;
       bb=ee/1000000;
       ee-=bb*1000000;
       numer_wiersza++;
      }
     if (numer_wiersza>0)
      {
        //poszukiwanie najblizszego paska
        no_strip=0;
        find_strip=FALSE;
        while (no_strip<100)
         {
           if (strips->strip[no_strip].wiersz>numer_wiersza)
            {
             if (no_strip>0)
             {
              numer_wiersza1=strips->strip[no_strip-1].wiersz;
              key_rr+=strips->strip[no_strip-1].adr;
              count+=strips->strip[no_strip-1].count+1;
              ///////
              ee=0;
              ee+=SSkalaY;
              bb=ee/1000000;
              ee-=bb*1000000;
              ///////
              numer_wiersza2=0;
              while (numer_wiersza2<numer_wiersza1)
               {
                pcx_y+=bb;
                ee+=SSkalaY;
                bb=ee/1000000;
                ee-=bb*1000000;
                numer_wiersza2++;
               }
              find_strip=TRUE;
             }
             break;
            }
           no_strip++;
         }
        if (find_strip==FALSE)
         {
          ///////
          ee=0;
          ee+=SSkalaY;
          bb=ee/1000000;
          ee-=bb*1000000;
          ///////
         }
      }
      else
       {
        ///////
        ee=0;
        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;
        ///////
       }
    }
     else
      {
       ///////
       ee=0;
       ee+=SSkalaY;
       bb=ee/1000000;
       ee-=bb*1000000;
       ///////
      }

  while(count<total && pcx_y<mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          dd+=(SSkalaX*8);
          aa=dd/1000000;
          dd-=(aa*1000000);

          byte_x+=1;
          if (byte_x >= width)
           {

            prostokat2_(ctx_x0,ctx_y0,pcx_x,pcx_y,aa,bb,8-((byte_x)*8-width_r),key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
            pcx_x=pcx_x0;
            pcx_y+=bb;
            if (pcx_y>mxy) 
			{
				for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
                nr_i=0;
				return 0;
			}
		
            dd=0;

            ee+=SSkalaY;
            bb=ee/1000000;
            ee-=bb*1000000;

            byte_x=0;
           }
          num_bytes--;
        }

        count++;
              if (byte_x<width && byte_x>0)
               {
                 prostokat2_(ctx_x0,ctx_y0,pcx_x,pcx_y,aa,bb,byte_x*8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
                 pcx_x+=aa;
               }
       }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key

      dd+=(SSkalaX*8);
      aa=dd/1000000;
      dd-=aa*1000000;

      byte_x+=1;
      if (byte_x >= width )
       {

        prostokat2_(ctx_x0,ctx_y0,pcx_x,pcx_y/*+mmy*/,aa,bb,8-((byte_x/*-1*/)*8-width_r),key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
           
        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (pcx_y>mxy) 
		{
			for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
            nr_i=0;
			return 0;
		}
		
        dd=0;
        byte_x=0;

        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;

       }
      }
      count++;
      if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
		   for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
           nr_i=0;
           return -1;  //przerwano rysowanie
  	      }
       }
              if (byte_x<width && byte_x>0)
               {
                 prostokat2_(ctx_x0,ctx_y0,pcx_x,pcx_y/*+mmy*/,aa,bb,byte_x*8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
                 pcx_x+=aa;
               }
    }
   key_rr++;
  }


for (iii=0; iii<16; iii++) destroy_sprite_bmp(iii);
nr_i=0;
return 1;
}

int Draw_pcx_1_file(B_PCX *pcx)
{
 unsigned char key_i;
 int i;
 int  aa = 0, bb = 0;
 int cc=0,dd=0,ee=0;
 double SkalaX,SkalaY;
 REAL_PCX *rpcx;
 int width, width1, height;
 int pcx_x,pcx_y,pcx_x0,pcx_y0;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
 double SkalaXX, SkalaYY;
  int SSkalaX, SSkalaY;
  double width_d;
  double height_d;
  int kolor_tla0, kolor_tla, kolor_punktu, ignore_bk;
  int width_r;
  double kos, koc;

  rpcx = (REAL_PCX*) pcx->pcx;
  key0 = 0;

  width_d  = ((rpcx->header.xmax - rpcx->header.xmin)/8)+1.5;         // image dimensions...
  width_r= (rpcx->header.xmax - rpcx->header.xmin)+1;
  width=(int)width_d;
  height_d = rpcx->header.ymax - rpcx->header.ymin; //  + 1;
  height=(int)height_d;


  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  1
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=Wyslij_par_ekranu(0);
  mxx=Wyslij_par_ekranu(1);
  mxy=Wyslij_par_ekranu(2);
  mmy=Wyslij_par_ekranu(3);

  pcx_x0 = pikseleX0(pcx->x);
  byte_x = 0;
  pcx_y0 = pikseleY0(pcx->y)+mmy;
  pcx_x = pcx_x0;
  pcx_y = pcx_y0;

  kos=sin(pcx->kat);
  koc=cos(pcx->kat);

  dlugosc_bloku_pcx=0;

   if (pcx->ignore_background==1) ignore_bk=1; else ignore_bk=0;
   kolor_tla0=pcx->background;
   kolor_tla=pcx->color_key[1];
   if (pcx->set_foreground==1) kolor_punktu=pcx->foreground; else kolor_punktu=pcx->color_key[0];

  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);// + 28;

  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width_d)-pikseleX0(0.0);
  SkalaX=fabs(SkalaXX/width_d)*1000000.0;
  SkalaYY=pikseleY0d(pcx->dy*height_d)-pikseleY0(0.0);
  SkalaY=fabs(SkalaYY/height_d)*1000000.0;

  SSkalaX=(int)SkalaX;
  SSkalaY=(int)SkalaY;

  ee+=SSkalaY;
  bb=ee/1000000;
  ee-=bb*1000000;

  while(count<total && pcx_y<mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          dd+=(SSkalaX*8);
          aa=dd/1000000;
          dd-=(aa*1000000);

          byte_x+=1;
          if (byte_x >= width)
           {
             prostokat2(pcx_x-mmx,pcx_y/*+mmy*/,aa,bb,byte_x*8-width_r,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
            pcx_x=pcx_x0;
            pcx_y+=bb;
            if (pcx_y>mxy) return 0;
            dd=0;

            ee+=SSkalaY;
            bb=ee/1000000;
            ee-=bb*1000000;

            byte_x=0;
           }
          num_bytes--;
        }

        count++;

              if (byte_x<width && byte_x>0)
               {
                 prostokat2(pcx_x-mmx,pcx_y/*+mmy*/,aa,bb,8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
                 pcx_x+=aa;
               }

       }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key

      dd+=(SSkalaX*8);
      aa=dd/1000000;
      dd-=aa*1000000;


      prostokat2(pcx_x-mmx,pcx_y/*+mmy*/,aa,bb,8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);

      byte_x+=1;
      if (byte_x >= width )
       {
        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (pcx_y>mxy) return 0;

        dd=0;
        byte_x=0;

        ee+=SSkalaY;
        bb=ee/1000000;
        ee-=bb*1000000;

       }
      }
      count++;
      if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
           return 1;
  	      }
       }

          if (byte_x<width && byte_x>0)
           {
             prostokat2(pcx_x-mmx,pcx_y/*+mmy*/,aa,bb,8,key_i,kolor_tla0,kolor_tla,kolor_punktu,ignore_bk,pcx_x0,pcx_y0,kos,koc);
             pcx_x+=aa;
           }

    }
   key_rr++;
  }

return 1;
}



int Draw_pcx_0(B_PCX *pcx)
{
 unsigned char key_i;
 int i;
 int  aa = 0, bb = 0;
 float   cc=0,dd=0,ee=0;
 double SkalaX,SkalaY;
 REAL_PCX *rpcx;
 int width, width1, height;
 double pcx_x,pcx_y,pcx_x0,pcx_y0;
 long mxx,mxy,mmx,mmy;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
 double SkalaXX, SkalaYY;

  rpcx = (REAL_PCX*) pcx->pcx;
  key0 = 0;
  width  = rpcx->header.xmax - rpcx->header.xmin + 1;         // image dimensions...
  height = rpcx->header.ymax - rpcx->header.ymin + 1;

  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  1
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  mmx=Wyslij_par_ekranu(0);
  mxx=Wyslij_par_ekranu(1);
  mxy=Wyslij_par_ekranu(2);
  mmy=Wyslij_par_ekranu(3);

  pcx_x0 = pikseleX0(pcx->x);
  byte_x = 0;
  pcx_y0 = pikseleY0(pcx->y)+mmy;
  pcx_x = pcx_x0;
  pcx_y = pcx_y0;

  dlugosc_bloku_pcx=0;
   
  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);
  i=-1;

  SkalaXX=pikseleX0d(pcx->dx*width)-pikseleX0d(0.0);
  SkalaX=fabs(SkalaXX/width);
  SkalaYY=pikseleY0d(pcx->dy*height)-pikseleY0(0.0);
  SkalaY=fabs(SkalaYY/height);

  bb=0;

  while(count<total && pcx_y<mxy)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
      {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key
          dd+=SkalaX;
          aa=(int)dd;
          dd=dd-aa;

          pcx_x+=aa*8;
          byte_x+=1;
          if (byte_x>=width)
           {
            if (pcx_x<=mxx && pcx_x>=mmx && pcx_y>=mmy && pcx_y<=mxy) prostokat1(pcx_x,pcx_y/*+mmy*/,aa,bb,key_i);
            pcx_x=pcx_x0;
            pcx_y+=bb;
            if (pcx_y>mxy) return 0;
            dd=0;

            ee+=SkalaY;
            bb=(int)ee;
            ee=ee-bb;
            byte_x=0;
           }
          num_bytes--;
        }
        count++;
        if (byte_x<width && byte_x>0 && pcx_x<=mxx && pcx_x>=mmx && pcx_y>=mmy && pcx_y<=mxy) prostokat1(pcx_x,pcx_y/*+mmy*/,aa,bb,key_i);
      }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      //wstawienie punktu w kolorze key
      dd+=SkalaX;
      aa=(int)dd;
      dd=dd-aa;

      if (pcx_x<=mxx  && pcx_x>=mmx && pcx_y>=mmy && pcx_y<=mxy) prostokat1(pcx_x,pcx_y/*+mmy*/,aa,bb,key_i);
      pcx_x+=aa*8;
      byte_x+=1;
      if (byte_x>=width )
       {
        pcx_x=pcx_x0;
        pcx_y+=bb;
        if (pcx_y>mxy) return 0;
        dd=0;
        byte_x=0;
        ee+=SkalaY;
        bb=(int)ee;
        ee=ee-bb;
       }
      }
      count++;
      if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
           return 1;
  	      }
       }
      if (byte_x<width && byte_x>0 && pcx_x<=mxx && pcx_x>=mmx && pcx_y>=mmy && pcx_y<=mxy) prostokat1(pcx_x,pcx_y/*+mmy*/,aa,bb,key_i);
    }
   key_rr++;
  }

return 1;
}


int LoadPCX_real(B_PCX *b_pcx, char *nazwa_pcx)
//PCXheader *head, char enablepal, int pcx_x0, int pcx_y0)

{
	int i, j;
	int width, width1, height;
	int count = 0;          // current pixel number in the image
	int total;            // number of total pixels in the image
	COLOR_ key_pal[256];             // current byte read from the image
	PCXheader head;
	byte_ key0;
	BOOL empty_line = TRUE;
	long size_of_file;
	long nr_byte;
	int del_rgb, min_rgb;
    int ii, k;
	double xx, yy;
	int fp;
	COLOR palette;
	int liczba_kolorow;
	STRIPS *strips;
	COLOR *pcxpalette;
	long file_len;

	xx = b_pcx->x; yy = b_pcx->y;
	key0 = 0;

	fp = open(nazwa_pcx, O_RDWR | O_BINARY);

	if (fp == -1)
	{
		ErrList(202);
		return 0;
	}

   file_len = filelength(fp);

   InitBufPCX(sizeof(B_PCX)+ file_len + sizeof(STRIPS) + 64);

   if (bufor_pcx == NULL)
   {
	   ErrList(211);
	   return 0;
   }

  adr_pcx = (B_PCX *)bufor_pcx;

  if (read(fp,&head, sizeof(PCXheader)) != sizeof(PCXheader))
   {
    ErrList(202);
	FreeBufPCX();
    return 0;        // header is 128 bytes
   }

  width  = head.xmax - head.xmin + 1;         // image dimensions...
  height = head.ymax - head.ymin + 1;


  adr_pcx->atrybut = Anormalny;
  adr_pcx->obiekt  = Opcx;
  adr_pcx->obiektt1 = 0;
  adr_pcx->obiektt2 = 0;
  adr_pcx->obiektt3 = 0;
  adr_pcx->widoczny = 1;
  adr_pcx->przec = 0;
  adr_pcx->blok = 0;
  adr_pcx->n = T294+filelength(fp);
  adr_pcx->warstwa = Current_Layer;
  adr_pcx->kod_obiektu = 1;
  adr_pcx->inwersja = b_pcx->inwersja;
  adr_pcx->ignore_background = b_pcx->ignore_background;
  adr_pcx->ignore_print_background = b_pcx->ignore_print_background;
  adr_pcx->set_foreground = b_pcx->set_foreground;
  adr_pcx->background = b_pcx->background;
  adr_pcx->foreground = b_pcx->foreground;
  adr_pcx->markers = b_pcx->markers;
  adr_pcx->stripped = b_pcx->stripped;
  adr_pcx->buffered = 0;
  adr_pcx->on_front = b_pcx->on_front;
  adr_pcx->h_flip = b_pcx->h_flip;
  adr_pcx->v_flip = b_pcx->v_flip;
  adr_pcx->kat = b_pcx->kat;
  adr_pcx->x = b_pcx->x;
  adr_pcx->y = b_pcx->y;
  adr_pcx->dx = b_pcx->dx;
  adr_pcx->dy = b_pcx->dy;
  adr_pcx->i_x = head.xmax - head.xmin + 1;
  adr_pcx->i_y = head.ymax - head.ymin + 1;

  adr_pcx->len_pcx = file_len; // filelength(fp);


  if(head.manufacturer   != 10         // check for errors
   ||head.version         <  5
   ||head.encoding       !=  1
   ||((head.bits_per_pixel !=  8) && (head.bits_per_pixel !=  1))
   ||head.xmin  >=  head.xmax
   ||head.ymin  >=  head.ymax)
  {
    close(fp);
	ErrList(202);
	FreeBufPCX();
    return 0;
  }

  if (head.bits_per_pixel == 8)
  {
	  lseek(fp, -768, SEEK_END);             // palette is the last 768 bytes
	  size_of_file = lseek(fp, 0, 1);
	  if (read(fp, &key_pal[0], (256 * 3)) != (256 * 3))
	  {
		  ErrList(202);
		  FreeBufPCX();
	      return 0;
	  }
    for(i=0;i<256;i++)
    {
     palette[i].red = key_pal[i].red>>2;
     palette[i].gre = key_pal[i].gre>>2;
     palette[i].blu = key_pal[i].blu>>2;

     min_rgb=11907/*765*/;
     adr_pcx->color_key[i]=i;  /*poczatkowe wartosci*/
     for (j=0; j<256; j++)
     {
      del_rgb=abs(_dac_normal[j][0]-palette[i].red)*abs(_dac_normal[j][0]-palette[i].red);
      del_rgb+=abs(_dac_normal[j][1]-palette[i].gre)*abs(_dac_normal[j][1]-palette[i].gre);
      del_rgb+=abs(_dac_normal[j][2]-palette[i].blu)*abs(_dac_normal[j][2]-palette[i].blu);
      if (del_rgb < min_rgb)
        {
         min_rgb=del_rgb;
         adr_pcx->color_key[i]=j;  //poczatkowe wartosci
        }
     }
    }
   }
    else
     {
        switch (head.bits_per_pixel)
         {
          case 1: liczba_kolorow=2;
          break;
          case 2: liczba_kolorow=4;
          break;
          case 3: liczba_kolorow=8;
          break;
          case 4: liczba_kolorow=16;
          break;
          default: liczba_kolorow=16;
          break;
         }

       for (i=0; i<liczba_kolorow; i++)
       {
        ii=i*3;
        palette[i].red = head.ega_palette[ii]>>2;
        palette[i].gre = head.ega_palette[ii+1]>>2;
        palette[i].blu = head.ega_palette[ii+2]>>2;

        min_rgb=11907/*765*/;
        adr_pcx->color_key[i]=i;  /*poczatkowe wartosci*/
        for (j=0; j<256; j++)
        {
         del_rgb=abs(_dac_normal[j][0]-palette[i].red)*abs(_dac_normal[j][0]-palette[i].red);
         del_rgb+=abs(_dac_normal[j][1]-palette[i].gre)*abs(_dac_normal[j][1]-palette[i].gre);
         del_rgb+=abs(_dac_normal[j][2]-palette[i].blu)*abs(_dac_normal[j][2]-palette[i].blu);
         if (del_rgb < min_rgb)
           {
            min_rgb=del_rgb;
            adr_pcx->color_key[i]=j;  //poczatkowe wartosci

           }
        }
       }
     }

  lseek(fp,0,SEEK_SET);

  if(width%2 && width != head.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  
  dlugosc_bloku_pcx=0;
   
  nr_byte = 128;

  if (read(fp, &adr_pcx->pcx, filelength(fp)/*/-896*/) != (filelength(fp)/*-896*/))
  {
	  ErrList(202);
	  FreeBufPCX();
	  return 0;
  }

  //na koncu bloku pcx dostawiony jest blok strips
  adr_pcx->n += sizeof(STRIPS) ;
  adr_pcx->markers=1;   //wystepuja adresy paskow
  adr_pcx->stripped=0;  //jeszcze nie pociety na paski

  //oznacza to adres palety dla 8bpp znajduje sie 
  
  pcxpalette=(COLOR *)(adr_pcx->pcx+adr_pcx->len_pcx-768);

  strips=(STRIPS *)(adr_pcx->pcx+filelength(fp));

  for (k=0; k<100; k++)
   {
     strips->strip[k].wiersz=0;
     strips->strip[k].adr=0;
     strips->strip[k].count=0;
   }

  close(fp);

  if(dodaj_obiekt(NULL,adr_pcx)==NULL)
   {
    bitmap_exist=FALSE;
    bitmap_view=TRUE;
   }

  
  FreeBufPCX();
  return 1;
}

int LoadPCX_(B_PCX *b_pcx, PCXheader *head/*, COLOR *palette*/, char enablepal)
{
  int i, j;
  int width, height;
  int byte_x;
  int count=0;          // current pixel number in the image
  int total;            // number of total pixels in the image
  byte_ key;             // current byte read from the image
  byte_ key0;
  int num_bytes;        // number of bytes in a "run"
  double pcx_x, pcx_x1, pcx_x2;
  double pcx_y, pcx_y1, pcx_y2;
  BOOL empty_line = TRUE;
  long size_of_file;
  long nr_byte;
  int del_rgb, min_rgb;
  int liczba_kolorow;

  int fp;
  COLOR palette; 
  key0 = 0;

  fp = open (b_pcx->pcx, O_RDWR|O_BINARY) ;
 
  if (fp == -1) return 0;

  if (read(fp,head, sizeof(PCXheader)) != sizeof(PCXheader)) return 0;        // header is 128 bytes

  width  = head->xmax - head->xmin + 1;         // image dimensions...
  height = head->ymax - head->ymin + 1;

  if(head->manufacturer   != 10         // check for errors
   ||head->version         <  5
   ||head->encoding       !=  1
   ||head->bits_per_pixel !=  8
   ||head->xmin  >=  head->xmax
   ||head->ymin  >=  head->ymax)
  {
    close(fp);
    return 0;
  }
  if (head->bits_per_pixel>4)
   {
    lseek(fp,-768, SEEK_END);             // palette is the last 768 bytes
    size_of_file = lseek (fp, 0, 1);

    for(i=0;i<256;i++)
    {

     if (read(fp,&key, 1) != 1) return 0;
     palette[i].red = key>>2;

     if (read(fp,&key, 1) != 1) return 0;
     palette[i].gre = key>>2;

     if (read(fp,&key, 1) != 1) return 0;
     palette[i].blu = key>>2;
     /*odszukanie podobnego koloru*/
     min_rgb=11907;
     color_key[i]=i;  /*poczatkowe wartosci*/
     for (j=0; j<256; j++)
     {
      del_rgb=abs(_dac_normal[j][0]-palette[i].red)*abs(_dac_normal[j][0]-palette[i].red);
      del_rgb+=abs(_dac_normal[j][1]-palette[i].gre)*abs(_dac_normal[j][1]-palette[i].gre);
      del_rgb+=abs(_dac_normal[j][2]-palette[i].blu)*abs(_dac_normal[j][2]-palette[i].blu);
      if (del_rgb < min_rgb)
        {
          min_rgb=del_rgb;
          color_key[i]=j;  /*poczatkowe wartosci*/
        }
     }
    }
   }
    else //mapy 1,2 i 4 bitowe  - na razie tylko 1
     {
      switch (head->bits_per_pixel)
       { case 1: liczba_kolorow=1;
         break;
         case 2: liczba_kolorow=2;
         break;
         case 3: liczba_kolorow=4;
         break;
         case 4: liczba_kolorow=8;
         break;
       }

      for(i=0;i<liczba_kolorow;i++)
       {
        if (read(fp,&key, 1) != 1) return 0;
        palette[i].red = key>>2;
        if (read(fp,&key, 1) != 1) return 0;
        palette[i].gre = key>>2;
        if (read(fp,&key, 1) != 1) return 0;
        palette[i].blu = key>>2;
        /*odszukanie podobnego koloru*/
        min_rgb=11907;
        color_key[i]=i;  /*poczatkowe wartosci*/
        for (j=0; j<256; j++)
         {
          del_rgb=abs(_dac_normal[j][0]-palette[i].red)*abs(_dac_normal[j][0]-palette[i].red);
          del_rgb+=abs(_dac_normal[j][1]-palette[i].gre)*abs(_dac_normal[j][1]-palette[i].gre);
          del_rgb+=abs(_dac_normal[j][2]-palette[i].blu)*abs(_dac_normal[j][2]-palette[i].blu);
          if (del_rgb < min_rgb)
           {
            min_rgb=del_rgb;
            color_key[i]=j;  /*poczatkowe wartosci*/
           }
         }
       }
       for (i=liczba_kolorow;i<256;i++)
        {
          color_key[i]=0;
        }
     }

  lseek(fp,128,SEEK_SET);

  if(width%2 && width != head->bytes_per_line)
    total = (width+1) * height;
  else
    total = width * height;
    
  pcx_x = 0;
  byte_x = 0;
  pcx_y = 0;  

  dlugosc_bloku_pcx=0;
   
  nr_byte = 128;
  Procent_0();

  while(count<total)
  {
    if (read(fp,&key, 1) != 1) return 0; 
    nr_byte++;
    procent_(nr_byte,size_of_file);
    
    if(key>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key - 192;    // pixels in this "run" can be up to 63
      if (read(fp,&key, 1) != 1) return 0; 
      nr_byte++;
      procent_(nr_byte,size_of_file);

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
      {
       // write the pixel value unless is is a "filler" byte
        if(width == head->bytes_per_line || count % head->bytes_per_line)
        {
          //wstawienie punktu w kolorze key

          if (empty_line == TRUE)
           {
            empty_line = FALSE;
            pcx_x1=b_pcx->x+pcx_x;
            pcx_y1=b_pcx->y+pcx_y;
            pcx_x2=pcx_x1+b_pcx->dx;
            pcx_y2=pcx_y1+b_pcx->dy;
            key0 = key;
           }
            else if ((key != key0) || (color_key[key] == kolory.paper))
               {
                if (color_key[key] != kolory.paper)
                {
                 if (Solid4(pcx_x1,pcx_y1,pcx_x2,pcx_y2,color_key[key])!=1)
                  {
                    close(fp);
                    if (FALSE == add_block_pcx ())
                     {
                        ;
                     }
                    ErrList(18);
                    delay(1000);
                    return 1;
                  }
                }  
                 pcx_x1=b_pcx->x+pcx_x;
                 pcx_y1=b_pcx->y+pcx_y;
                 pcx_x2=pcx_x1+b_pcx->dx;
                 pcx_y2=pcx_y1+b_pcx->dy;
                 key0 = key;
               }
             else
              {
                pcx_x2+=b_pcx->dx;
              }

          
          pcx_x+=b_pcx->dx;
          byte_x+=1;
          
          if (byte_x>=width)
           {
            if (color_key[key] != kolory.paper)
             {
              if (Solid4(pcx_x1,pcx_y1,pcx_x2,pcx_y2,color_key[key])!=1)
               {
                close(fp);
                if (FALSE == add_block_pcx ())
                 {
                 ;
                 }
                ErrList(18);
                delay(1000);
                return 1;
               }
             } 
            empty_line=TRUE;
            pcx_x=0;
            byte_x=0;
            pcx_y-=b_pcx->dy;
           }
          num_bytes--;
        }
        count++;
      }
    }
    else
    {
     // write the pixel value unless is is a "filler" byte
      if(width == head->bytes_per_line || count % head->bytes_per_line)
      {
      //wstawienie punktu w kolorze key
      //putpixel_(/*pcx_x0+*/pcx_x,/*pcx_y0+*/pcx_y,key);
      
      if (empty_line == TRUE)
           {
            empty_line = FALSE;
            pcx_x1=b_pcx->x+pcx_x;
            pcx_y1=b_pcx->y+pcx_y;
            pcx_x2=pcx_x1+b_pcx->dx;
            pcx_y2=pcx_y1+b_pcx->dy;
            key0 = key;
           }
            else if ((key != key0) || (color_key[key] == kolory.paper))
               {
                if (color_key[key] != kolory.paper)
                {
                 if (Solid4(pcx_x1,pcx_y1,pcx_x2,pcx_y2,color_key[key])!=1) 
                  {
                   close(fp);
                   if (FALSE == add_block_pcx ())
                    {
                     ;
                    }
                   ErrList(18);
                   delay(1000);
                   return 1;
                  } 
                }  
                 pcx_x1=b_pcx->x+pcx_x;
                 pcx_y1=b_pcx->y+pcx_y;
                 pcx_x2=pcx_x1+b_pcx->dx;
                 pcx_y2=pcx_y1+b_pcx->dy;
                 key0 = key;
               }
             else
              {
                pcx_x2+=b_pcx->dx;
              }
      
          
      pcx_x+=b_pcx->dx;
      byte_x+=1;
      if (byte_x>=width)
       {
        if (color_key[key] != kolory.paper)
        {
         if (Solid4(pcx_x1,pcx_y1,pcx_x2,pcx_y2,color_key[key])!=1) 
          {
            close(fp);
            if (FALSE == add_block_pcx ())
             {
              ;
             }
            ErrList(18);
            delay(1000);
            return 1;
          }  
        }  
        empty_line=TRUE;
        pcx_x=0;
        byte_x=0;
        pcx_y-=b_pcx->dy;
       }
      }
      count++;
    }
  }
  close(fp);
  
  if (FALSE == add_block_pcx ())
   {
     ;
     ErrList(18);
     delay(1000);
   }
  
  return 1;
}

/*
  Usually used to validate a PCX file before loading it, and to figure
  out how much memory to allocate for the image data
*/

int LoadPCXinfo(char *filename, PCXheader *head) //, COLOR *palette)
{

  int fp;

  fp = open(filename, O_RDONLY | O_BINARY);

  if (fp == -1) return 0;

  if (read(fp, head, sizeof(PCXheader)) != sizeof(PCXheader))
  {
	  ErrList(202);
	  close(fp);
	  return 0;        // header is 128 bytes
  }


  if(head->manufacturer   != 10         // check for errors
   ||head->version         <  5
   ||head->encoding       !=  1
   || ((head->bits_per_pixel != 8) && (head->bits_per_pixel != 1))
   ||head->xmin  >=  head->xmax
   ||head->ymin  >=  head->ymax)
  {
	ErrList(202);
    close(fp);
    return 0;
  }

  close(fp);
  return 1;
}


/*
  This, is NOT always an accurate PCX saving routine.  Luckily, it doesn't
  matter, because the "error" only shows up with images of odd width.
  Even in this case, though, the image will still load properly in every
  program I have ever seen.  (and it's smaller, too!)

  The "error" also is not an error on my part, but an oversight (or maybe
  a really stupid "feature") of ZSoft, who made the PCX format.  Images
  are required to have an even number of bytes per line, but the image
  itself can have an odd width.  This causes an extra "filler" byte to be
  added to the end of each line, which is really stupid.

  This differs from the original PCX specification, because it allows the
  bytes per line to be odd, which is more efficient than Zsoft's method.

  If you would rather save it "properly" you can figure it out yourself.
  There's no practical reason I can think of to do that, though.
*/

int SavePCX(char *filename, int width, int height)
{
  int i;

  PCXheader head;

  FILE *fp;

  char row_screen_buf[1800];

  int count=0;
  int count_r=0;
  int i_row;
  struct viewporttype view_old1;

  byte_ key;
  byte_ key2;

  int total;

  int runcount=0;

  getviewsettings(&view_old1);
  setviewport(0,0,getmaxx(),getmaxy(), 1);
  

  fp=fopen(filename,"w+b");
  if(!fp) return -1;

  head.manufacturer=10;
  head.version=5;
  head.encoding=1;
  head.bits_per_pixel=8;
  head.xmin=0;
  head.ymin=0;
  head.xmax=width-1;
  head.ymax=height-1;
  head.horz_res=width;
  head.vert_res=height;
  head.num_color_planes=1;
  head.bytes_per_line=width;
  head.palette_type=1;

  fwrite(&head, sizeof(head), 1, fp);

  total=width*height;


  count= 0;
  count_r=0;
  i_row=0;
  getimage(0,i_row,getmaxx(),i_row,row_screen_buf);

  while(count < total)
  {
    runcount = 0;       // reset some variables...
    key = row_screen_buf[count_r+83];
    key2= key;

    while(key == key2 && runcount < 63) // check to see how long this "run"
    {                                   //  in the image is...
      runcount++;
      count++;
      count_r++;
      if (count_r==width)
       {
         i_row++;
         getimage(0,i_row,getmaxx(),i_row,row_screen_buf);
         count_r=0;
       }

      key2=row_screen_buf[count_r+83];
      if(count % width == 0) break;             // "runs" cannot go from one
    }                                           //  row to another.
//   stop_running:
    if(runcount == 1)   // if it isn't a "run"
    {
      if(key >= 0xC0)   // save either a "run" of length 1...
      {
        putc(193, fp);
        putc(key, fp);
      }
      else
        putc(key, fp);  // ... or put a raw pixel
    }
    else                // if it is a "run", save the "runlength" and then
    {                   //  save the color index number
      putc(runcount + 192, fp);
      putc(key, fp);
    }
  }
// image_done:

// save_pal:

  fputc(0x0C,fp);			// put the seperator character (12)

  for(i=0;i<256;i++)			// save the palette
  {
    fputc(_dac_normal[i][0]<<2, fp);
    fputc(_dac_normal[i][1]<<2, fp);
    fputc(_dac_normal[i][2]<<2, fp);
  }

  fclose(fp);

  setviewport(view_old1.left,view_old1.top,view_old1.right,view_old1.bottom,1);

  return 0;
}


int load_pcx__(B_PCX *b_pcx)
/*-------------------------*/
{
 PCXheader header;
 char enablepal = 0;

 return LoadPCX_(b_pcx, &header, enablepal);
}

int load_pcx_(B_PCX *b_pcx,int opcja)
/*---------------------------------*/
{
 PCXheader header;
 int x, y;
 char enablepal = 0;

 x=b_pcx->x;
 y=b_pcx->y;
 if (x<0) x=0;
 if (y<0) y=0;

 return LoadPCX(b_pcx, &header, enablepal, x, y, opcja);
} 

int Write_screen_pcx(void)
{ int k;
 k=SavePCX("screen.pcx", getmaxx()+1, getmaxy()+1);
 return k;
}

#define __PCX__
