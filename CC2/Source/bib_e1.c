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

#pragma warning(disable : 4996)
#include<forwin.h>
#ifndef LINUX
#include<dos.h>
#endif
#define ALLEGWIN
#include <allegext.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "bib_e.h"
#include "rysuj_e.h"

extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;
extern int GetColorAC(int) ;
extern BOOL Check_if_Equal0(double, double);
extern BOOL Check_if_Equal (double, double) ;
extern BOOL Check_if_Equal2(double, double);
extern int Set_Orto_Dir (LINIA *, int ) ;
extern void setwritemode( int mode );
extern int linestyle_xor(int typ);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);

extern void set_pattern_offset(int offset);
extern int get_pattern_offset(void);
extern int get_sketch_offset(int sk_offset);

/*--------------------------------------------------------*/
/*--------      linia     ---------*/

//#include <math.h>
//#define M_PI 3.14159265358979323846  // Define PI if not available

/*
 * Calculate the angle (in degrees) that the vector from (x1, y1) to (x2, y2)
 * makes relative to the x-axis in the isometric coordinate system.
 * Positive angles are counterclockwise, range: [0, 360).
 * Returns: angle in degrees, or 0 if vector is zero-length.
 */
double isometric_vector_angle(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dx == 0.0 && dy == 0.0) {
        return 0.0;  // Undefined for zero vector; return 0 by convention
    }

    // Transform to isometric components
    double inv_sqrt3 = 1.0 / sqrt(3.0);
    double i_dx = (dx * inv_sqrt3) + dy;
    double i_dy = (-dx * inv_sqrt3) + dy;

    // Compute angle using atan2
    double angle_rad = atan2(i_dy, i_dx);
    double angle_deg = angle_rad * (180.0 / PI);

    // Normalize to [0, 360)
    if (angle_deg < 0.0) {
        angle_deg += 360.0;
    }

    return angle_deg;
}

/*
 * Calculate the length of a vector defined by endpoints (x1,y1) to (x2,y2)
 * in the isometric coordinate system.
 *
 * The length is measured along the isometric axes (same as Cartesian distance
 * in the underlying world, since the transformation preserves lengths for
 * displacements along the axes).
 *
 * Inputs:
 *   x1, y1 - Starting point in Cartesian (drawing) coordinates
 *   x2, y2 - Ending point in Cartesian (drawing) coordinates
 *
 * Returns:
 *   The length in the same units as krok_g / object units (world units)
 */
double isometric_vector_length(double x1, double y1, double x2, double y2, double *pl_dx, double *pl_dy) {
    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dx == 0.0 && dy == 0.0) {
        return 0.0;  // Zero vector
    }

    // Transform Cartesian displacement to isometric components
    double inv_sqrt3 = 1.0 / sqrt(3.0);
    double i_dx = inv_sqrt3 * dx + dy;
    double i_dy = -inv_sqrt3 * dx + dy;

    // Euclidean distance in isometric space (preserves true length)
    *pl_dx=i_dx;
    *pl_dy=i_dy;
    return sqrt(i_dx * i_dx + i_dy * i_dy);
}

double isometric_vector_length_f(float x1, float y1, float x2, float y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dx == 0.0 && dy == 0.0) {
        return 0.0;  // Zero vector
    }

    // Transform Cartesian displacement to isometric components
    double inv_sqrt3 = 1.0 / sqrt(3.0);
    double i_dx = inv_sqrt3 * dx + dy;
    double i_dy = -inv_sqrt3 * dx + dy;

    // Euclidean distance in isometric space (preserves true length)
    return sqrt(i_dx * i_dx + i_dy * i_dy);
}

void parametry_linior (LINIA *L,PLINIA *PL)
/*--------------------------------------------*/
{
  double dx,dy,dxprim,dyprim;
  double dxprim1,dyprim1;
  double sinfi,cosfi,angle_l,angle_lr;
  double sinfi1,cosfi1,angle_lr1;

  if (L == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  if (orto == 0)
  {
   if (options1.uklad_izometryczny)
    {
       angle_l=get_angle_l();
       dx = L->x2 - L->x1;
       dy = L->y2 - L->y1;
       PL->kat=isometric_vector_angle(L->x1, L->y1, L->x2, L->y2)+angle_l;
       PL->sin=sin(PL->kat/180.*PI);
       PL->cos=cos(PL->kat/180.*PI);
       //PL->dl=sqrt(dx*dx+dy*dy);
       PL->dl=isometric_vector_length(L->x1, L->y1, L->x2, L->y2, &PL->dx, &PL->dy);
       //PL->dx=dx;
       //PL->dy=dy;
    }
    else parametry_lini (L, PL);
    return;
  }

  angle_l=get_angle_l();
		      /* uklad globalny */
  dx = L->x2 - L->x1;
  dy = L->y2 - L->y1;

  if (angle_l!=0)		   /* uklad lokalny */
   {
       angle_lr=angle_l*0.017453293;
       sinfi=sin(angle_lr);
       cosfi=cos(angle_lr);
       dxprim=dx*cosfi+dy*sinfi;
       dyprim=-dx*sinfi+dy*cosfi;
       if (options1.uklad_izometryczny)
       {
           angle_lr1=(angle_l+120)*0.017453293;
           sinfi1=sin(angle_lr1);
           cosfi1=cos(angle_lr1);
           dxprim1=dx*cosfi1+dy*sinfi1;
           dyprim1=-dx*sinfi1+dy*cosfi1;
       }
   }
  if (angle_l==0)
  {
   if ( Orto_Dir == I_Orto_XDir )
   {
    PL->kat=(dx>=0 ? 0 : 180);
    PL->cos=(dx>=0 ? 1 : -1);
    PL->sin=0;
    PL->dl=fabs(dx);
   }
   else
   if ( Orto_Dir == I_Orto_YDir )
   {
    PL->kat=(dy>=0 ? 90 : 270);
    PL->sin=(dy>=0 ? 1 : -1);
    PL->cos=0;
    PL->dl=fabs(dy);
   }
   else
   {
    parametry_lini (L, PL) ;
   }
  }
  else
  {
   if ( Orto_Dir == I_Orto_XDir )
   {
    PL->kat=(dxprim>=0 ? angle_l : angle_l+180);
    PL->cos=(dxprim>=0 ? cosfi : -cosfi);
    PL->sin=(dxprim>=0 ? sinfi : -sinfi);
    PL->dl=fabs(dxprim);
   }
   else
    if ( Orto_Dir == I_Orto_YDir )
    {
     if (options1.uklad_izometryczny)
         angle_lr=(angle_l+120)*0.017453293;
     else angle_lr=(angle_l+90)*0.017453293;
     sinfi=sin(angle_lr);
     cosfi=cos(angle_lr);
     if (options1.uklad_izometryczny)
     {
         PL->kat = (dxprim1 >= 0 ? angle_l + 120 : angle_l + 300);
         PL->sin=(dxprim1>=0 ? sinfi1 : -sinfi1);
         PL->cos=(dxprim1>=0 ? cosfi1 : -cosfi1);
         ////PL->sin=(dyprim1>=0 ? -sinfi1 : sinfi1);
         ////PL->cos=(dyprim1>=0 ? -cosfi1 : cosfi1);
         //PL->dl=fabs(dxprim1);
         PL->dl=sqrt(dx*dx+dy*dy);
         PL->dx=dx;
         PL->dy=dy;
     }
     else
     {
         PL->kat = (dyprim >= 0 ? angle_l + 90 : angle_l + 270);

         PL->sin = (dyprim >= 0 ? sinfi : -sinfi);
         PL->cos = (dyprim >= 0 ? cosfi : -cosfi);
         PL->dl = fabs(dyprim);
     }
    }
    else
    {
        if (options1.uklad_izometryczny)
        {
            dx = L->x2 - L->x1;
            dy = L->y2 - L->y1;
            PL->kat=isometric_vector_angle(L->x1, L->y1, L->x2, L->y2)+angle_l;
            PL->sin=sin(PL->kat/180.*PI);
            PL->cos=cos(PL->kat/180.*PI);
            PL->dl=sqrt(dx*dx+dy*dy);
            PL->dx=dx;
            PL->dy=dy;
        }
        else parametry_lini (L, PL) ;
    }
  }
}


void out_parametry_lini (LINIA *L)
/*-------------------------------*/
{
  double l ;
  PLINIA PL ;
  double angle_l, PL_kat ;

  if (eL.st == NULL) return;

  if (L == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  parametry_linior (L, &PL) ;
  l = milimetryob (PL.dl) ;
  eL.st [0] = '\0' ;
  if (!orto)
  {
    angle_l=get_angle_l();
    if (angle_l!=0)
    {
    PL_kat=PL.kat-angle_l;
    if (PL_kat<0) PL_kat+=360;
    }
    else
    {
     PL_kat=PL.kat;
    }
    sprintf (eL.st, "%#12.9lg;%#8.3lf", l, PL_kat);
  }
  else
  {
    sprintf (eL.st, "%#12.9lg", l) ;
  }
  Out_Edited_Draw_Param ((ESTR *)&eL, TRUE) ;
}

void clear_parametry_lini (void)
/*----------------------------*/
{
  Out_Edited_Draw_Param ((ESTR *)&eL, FALSE) ;
}


void outline (LINIA *L,int mode,int pl)
{

  if (L == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  okno_r();
  //setwritemode(mode);
  if (mode==XOR_PUT) linestyle_xor(L->typ); else linestyle(L->typ);

  setwritemode(COPY_PUT);

/*  linestyle(L->typ); */
  SetColorAC(L->kolor);
  lineC (pikseleX0 (L->x1),pikseleY0 (L->y1), pikseleX0 (L->x2),pikseleY0 (L->y2));
  okno_all();
  if (pl) out_parametry_lini(L);
}

void outlineor (LINIA *L,int mode,int pl)
/*-----------------------------------------*/
{ LINIA L1;
  double angle_l;
  double P_Orto_Dir;
  double L1000, L2000;

  if (L == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  okno_r();
  ////setwritemode(mode);
  if (mode==XOR_PUT) linestyle_xor(L->typ); else linestyle(L->typ);

  setwritemode(COPY_PUT);

  if (L->kolor == 0) SetColorAC(15);
  else SetColorAC(L->kolor);

  if (orto)
  {
    L1.x1=L->x1;
    L1.y1=L->y1;
    L1.x2=L->x2;
    L1.y2=L->y2;
    Orto_Dir =  Set_Orto_Dir (L, Orto_Dir);
    orto_l(&L1, &Orto_Dir);
    lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L1.x2),pikseleY0(L1.y2));
    setlinestyle1(DOTTED_LINE,0,NORM_WIDTH);
    lineC(pikseleX0(L1.x2),pikseleY0(L1.y2),pikseleX0(L->x2),pikseleY0(L->y2));
    if ((AXIS==TRUE) || (AXIS_DIV==TRUE))
      { if (AXIS==TRUE)
         {
          linestyle(34);
          SetColorAC(kolorys.osie);
          L1000=1000;
          L2000=2000;
         }
          else
           {
             linestyle(67);
             SetColorAC(kolorys.linie_podzialu);
           }
        angle_l=get_angle_l();
        if (Check_if_Equal(angle_l,0)==TRUE)
        {
         if (L1.y1==L1.y2)  //linia pionowa
          {
            L_axis.x1=L1.x2;
            L_axis.x2=L1.x2;
            if (AXIS==TRUE)
              {
                L_axis.y1=-L1000;
                L_axis.y2=L2000;
              }
               else
                 {
                  L_axis.y1=L1.y2-L50;
                  L_axis.y2=L1.y2+L50;
                 }
            lineC(pikseleX0(L1.x2),pikseleY0(L_axis.y1),pikseleX0(L1.x2),pikseleY0(L_axis.y2));
          }
         else //linia pozioma
          {
            L_axis.y1=L1.y2;
            L_axis.y2=L1.y2;
            if (AXIS==TRUE)
             {
              L_axis.x1=-L1000;
              L_axis.x2=L2000;
             }
              else
               {
                L_axis.x1=L1.x2-L50;
                L_axis.x2=L1.x2+L50;
               }
            lineC(pikseleX0(L_axis.x1),pikseleY0(L1.y2),pikseleX0(L_axis.x2),pikseleY0(L1.y2));
          }
        }
         else
          {

            if (Check_if_Equal(L1.x2, L->x1)==TRUE)  //linia pionowa
              {
               L_axis.x1=L1.x2;
               L_axis.x2=L1.x2;
               if (AXIS==TRUE)
                {
                 L_axis.y1=-L1000;
                 L_axis.y2=L2000;
                }
                 else
                  {
                   L_axis.y1=L1.y2-L50;
                   L_axis.y2=L1.y2+L50;
                  }
              }
               else
                {
                 P_Orto_Dir = atan((L1.y2-L->y1)/(L1.x2-L->x1));
                 if (AXIS==TRUE)
                  {
                   L_axis.x1=L1.x2-L1000*sin(P_Orto_Dir);
                   L_axis.y1=L1.y2+L1000*cos(P_Orto_Dir);
                   L_axis.x2=L1.x2+L1000*sin(P_Orto_Dir);
                   L_axis.y2=L1.y2-L1000*cos(P_Orto_Dir);
                  }
                   else
                    {
                     L_axis.x1=L1.x2-L50*sin(P_Orto_Dir);
                     L_axis.y1=L1.y2+L50*cos(P_Orto_Dir);
                     L_axis.x2=L1.x2+L50*sin(P_Orto_Dir);
                     L_axis.y2=L1.y2-L50*cos(P_Orto_Dir);
                    }
                }
            lineC(pikseleX0(L_axis.x1),pikseleY0(L_axis.y1),pikseleX0(L_axis.x2),pikseleY0(L_axis.y2));
          }
      }


      L->x2=L1.x2;
      L->y2=L1.y2;
      //okno_all();
      //if (pl) out_parametry_lini(L);
  }
  else
  {
    lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L->x2),pikseleY0(L->y2));

    //okno_all();
    //if (pl) out_parametry_lini(L);
  }
  okno_all();
  if (pl) out_parametry_lini(L);

}



/*--------------------------------------------------*/

