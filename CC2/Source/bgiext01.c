//#define fullcolor   //zmienic tez w rysuj_e.c

/*
 * BCC2GRX  -  Interfacing Borland based graphics programs to LIBGRX
 * Copyright (C) 1993-96 by Hartmut Schirmer
 *
 * This library is copyrighted (see above). It might be used and
 * distributed freely as long as all copyright notices are left
 * intact.
 *
 * You may not distribute any changed versions of BCC2GRX without
 * written permission by Hartmut Schirmer.
 *
 * You are permitted to distribute an application linked with BCC2GRX
 * in binary only, provided that the documentation of the program:
 *
 *    a)   informs the user that BCC2GRX is used in the program, AND
 *
 *    b)   provides the user with the necessary information about
 *         how to obtain BCC2GRX. (i.e. ftp site, etc..)
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact :                Hartmut Schirmer
 *                          Feldstrasse 118
 *                  D-24105 Kiel
 *                          Germany
 *
 * e-mail : hsc@techfak.uni-kiel.d400.de
 */
//#include <graphics.h>
#define ALLEGWIN
#include <allegext.h>

#define PUTPIXEL myputpixel
#include "bgiext.h"

extern void setrgbpalette(int color, int red, int green, int blue);
extern void setfillstyle(int pattern, int color);
extern void setfillpattern( char  *upattern, int color);
extern void setcolor__(int color);
extern void myputpixel(int x, int y, int color);

static int global_color;

unsigned char _dac_normal[256][3] = {
  { 0, 0, 0}, 
  { 0, 0,42},   { 0,42, 0},   { 0,42,42},   {42, 0, 0},
  {42, 0,42},   {42,21, 0},   {42,42,42},   {21,21,21},
  {13,13,63},   {21,63,21},   {21,63,63},   {63,21,21},
  {63,21,63},   {63,63,21},   {63,63,63},   { 0, 0, 0}, 

  /*17*/  //58           //54           //45
  { 63, 63, 50}, { 63, 63, 44}, { 63, 63, 38}, { 63, 63, 31},
  { 63, 63, 27}, { 63, 63, 18}, { 63, 63,  9}, { 63, 63,  0},
  { 63, 51,  0}, { 63, 51,  9}, { 63, 51, 18}, { 63, 51, 27}, 
  { 63, 51, 36}, { 63, 51, 45}, { 63, 51, 54}, { 63, 51, 63},
  { 63, 39, 63}, { 63, 39, 54}, { 63, 39, 45}, { 63, 39, 36}, 
  { 63, 39, 27}, { 63, 39, 18}, { 63, 39,  9}, { 63, 39,  0},
  { 63, 26,  0}, { 63, 26,  9}, { 63, 26, 18}, { 63, 26, 27}, 
  { 63, 26, 36}, { 63, 26, 45}, { 63, 26, 54}, { 63, 26, 63},
  { 63, 13, 63}, { 63, 13, 54}, { 63, 13, 45}, { 63, 13, 36}, 
  { 63, 13, 27}, { 63, 13, 18}, { 63, 13,  9}, { 63, 13,  0},
  { 63,  0,  0}, { 63,  0,  9}, { 63,  0, 18}, { 63,  0, 27}, 
  { 63,  0, 36}, { 63,  0, 45}, { 63,  0, 54}, { 63,  0, 63},
  
  /*65*/
  { 48,  0, 63}, { 48,  0, 54}, { 48,  0, 45}, { 48,  0, 36}, 
  { 48,  0, 27}, { 48,  0, 18}, { 48,  0,  9}, { 48,  0,  0},
  { 48, 13,  0}, { 48, 13,  9}, { 48, 13, 18}, { 48, 13, 27}, 
  { 48, 13, 36}, { 48, 13, 45}, { 48, 13, 54}, { 48, 13, 63},
  { 48, 26, 63}, { 48, 26, 54}, { 48, 26, 45}, { 48, 26, 36}, 
  { 48, 26, 27}, { 48, 26, 18}, { 48, 26,  9}, { 48, 26,  0},
  { 48, 39,  0}, { 48, 39,  9}, { 48, 39, 18}, { 48, 39, 27}, 
  { 48, 39, 36}, { 48, 39, 45}, { 48, 39, 54}, { 48, 39, 63},
  { 48, 51, 63}, { 48, 51, 54}, { 48, 51, 45}, { 48, 51, 36}, 
  { 48, 51, 27}, { 48, 51, 18}, { 48, 51,  9}, { 48, 51,  0},
  { 48, 63,  0}, { 48, 63,  9}, { 48, 63, 18}, { 48, 63, 27}, 
  { 48, 63, 36}, { 48, 63, 45}, { 48, 63, 54}, { 48, 63, 63},

  /*113*/
  { 32, 63, 63}, { 32, 63, 54}, { 32, 63, 45}, { 32, 63, 36}, 
  { 32, 63, 27}, { 32, 63, 18}, { 32, 63,  9}, { 32, 63,  0},
  { 32, 51,  0}, { 32, 51,  9}, { 32, 51, 18}, { 32, 51, 27}, 
  { 32, 51, 36}, { 32, 51, 45}, { 32, 51, 54}, { 32, 51, 63},
  { 32, 39, 63}, { 32, 39, 54}, { 32, 39, 45}, { 32, 39, 36}, 
  { 32, 39, 27}, { 32, 39, 18}, { 32, 39,  9}, { 32, 39,  0},
  { 32, 26,  0}, { 32, 26,  9}, { 32, 26, 18}, { 32, 26, 27}, 
  { 32, 26, 36}, { 32, 26, 45}, { 32, 26, 54}, { 32, 26, 63},
  { 32, 13, 63}, { 32, 13, 54}, { 32, 13, 45}, { 32, 13, 36}, 
  { 32, 13, 27}, { 32, 13, 18}, { 32, 13,  9}, { 32, 13,  0},
  { 32,  0,  0}, { 32,  0,  9}, { 32,  0, 18}, { 32,  0, 27}, 
  { 32,  0, 36}, { 32,  0, 45}, { 32,  0, 54}, { 32,  0, 63},
  
  /*161*/
  { 16,  0, 63}, { 16,  0, 54}, { 16,  0, 45}, { 16,  0, 36}, 
  { 16,  0, 27}, { 16,  0, 18}, { 16,  0,  9}, { 16,  0,  0},
  { 16, 13,  0}, { 16, 13,  9}, { 16, 13, 18}, { 16, 13, 27}, 
  { 16, 13, 36}, { 16, 13, 45}, { 16, 13, 54}, { 16, 13, 63},
  { 16, 26, 63}, { 16, 26, 54}, { 16, 26, 45}, { 16, 26, 36}, 
  { 16, 26, 27}, { 16, 26, 18}, { 16, 26,  9}, { 16, 26,  0},
  { 16, 39,  0}, { 16, 39,  9}, { 16, 39, 18}, { 16, 39, 27}, 
  { 16, 39, 36}, { 16, 39, 45}, { 16, 39, 54}, { 16, 39, 63},
  { 16, 51, 63}, { 16, 51, 54}, { 16, 51, 45}, { 16, 51, 36}, 
  { 16, 51, 27}, { 16, 51, 18}, { 16, 51,  9}, { 16, 51,  0},
  { 16, 63,  0}, { 16, 63,  9}, { 16, 63, 18}, { 16, 63, 27}, 
  { 16, 63, 36}, { 16, 63, 45}, { 16, 63, 54}, { 16, 63, 63},

  /*209*/
  {  0, 63, 63}, {  0, 63, 54}, {  0, 63, 45}, {  0, 63, 36},   //209  0, 63, 63
  {  0, 63, 27}, {  0, 63, 18}, {  0, 63,  9}, {  0, 63,  0},
  {  0, 42,  0}, {  0, 42,  9}, {  0, 42, 18}, {  0, 42, 27}, 
  {  0, 42, 36}, {  0, 42, 45}, {  0, 42, 54}, {  0, 42, 63},


  {  0, 21, 63}, {  0, 21, 54}, {  0, 21, 45}, {  0, 21, 36}, 
  {  0, 21, 27}, {  0, 21, 18}, {  0, 21,  9}, {  0, 21,  0},
  {  0,  0,  0}, {  0,  0,  9}, {  0,  0, 18}, {  0,  0, 27}, 
  {  0,  0, 36}, {  0,  0, 45}, {  0,  0, 54}, {  0,  0, 63},
  //16 odcieni szarosci
  {  8,  8,  8}, { 13, 13, 13}, { 18, 18, 18}, { 23, 23, 23},
  { 28, 28, 28}, { 32, 32, 32}, { 36, 36, 36}, { 40, 40, 40},
  { 43, 43, 43}, { 46, 46, 46}, { 49, 49, 49}, { 52, 52, 52},
  { 55, 55, 55}, { 58, 58, 58}, { 61, 61, 61},// { 63, 63, 63},

  
};


void setrgbdefaults(void)
{
  int i;
#ifdef fullcolor
  return;

  for (i = 0; i < 256; ++i)
    GrSetColor(i,_dac_normal[i][0]*4,_dac_normal[i][1]*4,_dac_normal[i][2]*4);
#endif
  for (i = 0; i < 256; ++i)
    setrgbpalette(i,_dac_normal[i][0],_dac_normal[i][1],_dac_normal[i][2]);
}

#define min(a,b)    (((a) < (b)) ? (a) : (b))

void getcolor_RGB(int *red, int *green, int *blue, int color)
{
  *red  =_dac_normal[color][0];
  *green=_dac_normal[color][1];
  *blue =_dac_normal[color][2];
}

void getcolor_RGB_char(unsigned char *red, unsigned char *green, unsigned char *blue, int color)
{
	*red = _dac_normal[color][0] * 4;
	*green = _dac_normal[color][1] * 4;
	*blue = _dac_normal[color][2] *4;
}

void setrgb2cmyk(int *color_c, int *color_m, int *color_y, int i)
{
  double ccolor_c, ccolor_m, ccolor_y;
/*   *color_c = 255 - (_dac_normal[i][0] * 4); if (*color_c==3) *color_c=0;
   *color_m = 255 - (_dac_normal[i][1] * 4); if (*color_m==3) *color_m=0;
   *color_y = 255 - (_dac_normal[i][2] * 4); if (*color_y==3) *color_y=0;
*/
 if ((i==15) /*bialy*/ || (i==0) /*czarny*/)
  {
   ccolor_c=0;
   ccolor_m=0;
   ccolor_y=0;
  }
   else
    {
     ccolor_c=_dac_normal[i][0]+1;
     ccolor_c = 1.0 - ccolor_c/64.0;
     ccolor_c=255.0 - ccolor_c*255.0;
     ccolor_m=_dac_normal[i][1]+1;
     ccolor_m = 1.0 - ccolor_m/64.0;
     ccolor_m=255.0 - ccolor_m*255.0;
     ccolor_y=_dac_normal[i][2]+1;
     ccolor_y = 1.0 - ccolor_y/64.0;
     ccolor_y=255.0 - ccolor_y*255.0;
    }

 *color_c=(int)ccolor_c;
 *color_m=(int)ccolor_m;
 *color_y=(int)ccolor_y;
}

void setfillstyle_(int pattern, int color)
{
#ifdef fullcolor
 setfillstyle(pattern, GrAllocColor(_dac_normal[color][0]*4,_dac_normal[color][1]*4,_dac_normal[color][2]*4));
#else
 setfillstyle(pattern, color);
#endif
}

void setfillpattern_(char *pattern, int color)
{
#ifdef fullcolor
 setfillpattern(pattern, GrAllocColor(_dac_normal[color][0]*4,_dac_normal[color][1]*4,_dac_normal[color][2]*4));
#else
 setfillpattern(pattern, color);
#endif
}

#ifndef __WIN32__

void setcolor(int kolor)
/*--------------------*/
{
#ifdef fullcolor
  if (kolor==65535) kolor=15;
  setrgbcolor(_dac_normal[kolor][0]*4,_dac_normal[kolor][1]*4,_dac_normal[kolor][2]*4);
  global_color=kolor;
#else
  setcolor__(kolor);
#endif
}

int getcolor(void)
{
#ifdef fullcolor
  return  global_color;
#else
    if (__gr_color>255)
        return 0;
  return __gr_color;
#endif
}

#endif

void putpixel_(int x, int y, int color)
{
#ifdef fullcolor
   PUTPIXEL(x, y, GrAllocColor(_dac_normal[color][0]*4,_dac_normal[color][1]*4,_dac_normal[color][2]*4));
#else
   PUTPIXEL(x, y, color);
#endif
}



#ifdef fullcolor
void putpixel__(int x, int y,  unsigned char color_red, unsigned char color_gre, unsigned char color_blu)
{
  PUTPIXEL(x, y, GrAllocColor(color_red,color_gre,color_blu));
}

int get_fullcolor(int color)
{
  return GrAllocColor(_dac_normal[color][0]*4,_dac_normal[color][1]*4,_dac_normal[color][2]*4);
}
#endif
