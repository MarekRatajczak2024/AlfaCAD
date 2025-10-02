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

#define __O_VECTOR__

#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#ifndef LINUX
#include<process.h>
#else

#endif
#include<ctype.h>

#ifndef LINUX
#include<io.h>
#include<dos.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<math.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_pline.h"
#include "o_libfun.h"
#include "o_object.h"

#include "menu.h"

#include "leak_detector_c.h"

extern void Get_Graph_Values_Cur_Pos (double *, double *) ;
extern void view_line_type(LINIA *L);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void setwritemode( int mode );
extern int Vector_Selected(AVECTOR *ptrs_vector);
extern void Draw_Vector (AVECTOR *ptrs_vector, int mode, int kolor, int redraw_obj);
extern int Set_Orto_Dir (LINIA *, int ) ;
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void out_parametry_lini (LINIA *L);
extern void setcolor(int kolor);
extern void setfillstyle_(int pattern, int color);
extern void set_mode_solid(void);
extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out);
extern void *get_vector_c(void);
extern void *get_vector_s(void);

extern int Pline_Slab (void);

static void redcr(char);
static int last_vector_style=-1;

enum PLINE_MODE {  PL_MODE_CONTINUE = 1, PL_MODE_UNDO, PL_MODE_END} ;

static void (*cursor_on)(double ,double)=out_cur_on;
static void (*cursor_off)(double ,double)=out_cur_off;
static BOOL add_vector (BOOL b_strwyj) ;

TMENU mVector={23,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmVector,NULL,NULL} ;

static TMENU mVector_Con={1,0,0,15,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmVector_Con,NULL,NULL} ;

TMENU mMember_style= {4, 0,0, 12,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmMember_style, NULL,NULL} ;
TMENU mForce_Displacement_style= {2, 0,0, 12,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmForce_Displacement_style, NULL,NULL} ;
TMENU mMoment_Rotation_style= {4, 0,0, 12,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmMoment_Rotation_style, NULL,NULL} ;
TMENU mLoad_style= {5, 0,0, 12,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmLoad_style, NULL,NULL} ;

ESTR eVe, eVf, eVm, eVd, eVr, eVt, eVt1, eVn;

double radius_magnitude=1.0; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
double depth_magnitude=1.0; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
double thermal_magnitude=5.0; //units per mm  default 1 Celsius per 1 mm on drawing paper
double load_magnitude=10.0; //units per mm  default 10kN/m load per 1 mm on drawing paper
double flood_magnitude=10.0; //units per mm  default 10kN/m² load per 1 mm on drawing paper
double force_magnitude=10.0; //units per mm  default 10kN force per 1 mm on drawing paper
double moment_magnitude=10.0; //units per mm  default 10kNm force per 1 mm radius on drawing paper
double displacement_magnitude=1.0; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
double rotation_magnitude=0.001; //units per mm  default 0.001 rad desplacement per 1 mm radius on drawing paper

double radius_magnitude0=1.0; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
double depth_magnitude0=1.0; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
double thermal_magnitude0=5.0; //units per mm  default 1 Celsius per 1 mm on drawing paper
double load_magnitude0=10.0; //units per mm  default 10kN/m load per 1 mm on drawing paper
double flood_magnitude0=1.0; //units per mm  default 10kN/m² load per 1 mm on drawing paper
double force_magnitude0=10.0; //units per mm  default 10kN force per 1 mm on drawing paper
double moment_magnitude0=10.0; //units per mm  default 10kNm force per 1 mm radius on drawing paper
double displacement_magnitude0=1.0; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
double rotation_magnitude0=0.001; //units per mm  default 0.001 rad desplacement per 1 mm radius on drawing paper

static ESTR *eVa;
static int set_arc_stage=0;

static char *format_float="%#9.4lf";
static char *format_floatd="%#11.6lf";
static char *format_float2="%#9.4lf;%#9.4lf";
static char *format2_float2="%#9.4lf\0%#9.4lf;%#9.4lf";


enum MENU_ID {IDM_UNDO = 0, IDM_RIGID_RIGID, IDM_RIGID_PIN, IDM_PIN_RIGID, IDM_PIN_PIN, IDM_FORCE, IDM_MOMENT, IDM_MOMENT_REV,
        IDM_DISPLACEMENT, IDM_ROTATION, IDM_ROTATION_REV, IDM_TRAPEZIUM_Y, IDM_TRAPEZIUM_X, IDM_TRAPEZIUM_N, IDM_TRAPEZIUM_H, IDM_TRAPEZIUM_V, IDM_THERMAL, IDM_NODE,
        IDM_SLAB_PLATE, IDM_SLAB_SPACE, IDM_SLAB_WALL, IDM_SLAB_ZONE, IDM_SLAB_LOAD} ;

/*-----------------------------------------------------------------------*/
double line2len(AVECTOR *V)
{
    return sqrt(((V->x2-V->x1)*(V->x2-V->x1)) + ((V->y2-V->y1)*(V->y2-V->y1)));
}

void len2line(AVECTOR *V, float *x2, float *y2)
{
    //return sqrt(((V->x2-V->x1)*(V->x2-V->x1)) + ((V->y2-V->y1)*(V->y2-V->y1)));
    ;
}

void set_eVa(ESTR *eVx)
{
    eVa=eVx;
}

void reset_eVa(void)
{
    eVa=NULL;
}

void set_set_arc_stage(int stage)
{
    set_arc_stage=stage;
}

double angle2len(AVECTOR *V)
{
    double angle2;
    if (V->angle2<V->angle1) angle2=V->angle2+Pi2;
    else angle2=V->angle2;
    return (angle2-V->angle1);
}

void magnitude2angle(AVECTOR *V, double factor)
{
    double del;
    del=V->magnitude1/factor;
    if (del>=Pi2*0.99) del=Pi2*0.99;
    if ((V->style==5) || (V->style==8)) V->angle2=Angle_Normal(V->angle1+del);
    else V->angle1=Angle_Normal(V->angle2-del);
    return;
}

void magnitude2line(AVECTOR *V, double factor)
{   PLINIA PL;
    double kat, kos, koc;
    double x0, y0, x2, y2;

    parametry_lini(V, &PL);
    kat = PL.kat;
    kos = sin(PL.kat * Pi / 180);
    koc = cos(PL.kat * Pi / 180);
    x0 = V->x1 + (V->magnitude1 / force_magnitude);
    y0 = V->y1;
    Rotate_Point(kos, koc, V->x1, V->y1, x0, y0, &x2, &y2);
    V->x2=(float)x2;
    V->y2=(float)y2;
}

void set_magnitude(AVECTOR *V)
{   double l, a;
    switch (V->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            eVa=&eVe;
            break;
        case 4:
            V->magnitude1=(float)(line2len(V)*force_magnitude);
            eVa=NULL;
            break;
        case 5:
            a=angle2len(V);
            V->magnitude1=(float)(a*moment_magnitude);
            eVa=NULL;
            break;
        case 6:
            a=angle2len(V);
            V->magnitude1=a*moment_magnitude;
            eVa=NULL;
            break;
        case 7:
            l=line2len(V);
            V->r=l;
            V->magnitude1=(float)(l*displacement_magnitude);
            eVa=NULL;
            break;
        case 8:
            a=angle2len(V);
            V->magnitude1=(float)(a*rotation_magnitude);
            eVa=NULL;
            break;
        case 9:
            a=angle2len(V);
            V->magnitude1=(float)(a*rotation_magnitude);
            eVa=NULL;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            eVa=&eVt;
            break;
        case 17:
            eVa=&eVt1;
            break;
        case 16:
            eVa=&eVn;
            break;
        default:
            break;
    }
}

void set_length_angle2(AVECTOR *V)
{   double l, a;
    switch (V->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            eVa=&eVe;
            break;
        case 4:
            eVa=NULL;
            break;
        case 5:
        case 6:
            magnitude2angle(V, moment_magnitude);
            eVa=NULL;
            break;
        case 7:
            eVa=NULL;
            break;
        case 8:
        case 9:
            magnitude2angle(V, rotation_magnitude);
            eVa=NULL;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            eVa=&eVt;
            break;
        case 17:
            eVa=&eVt1;
            break;
        case 16:
            eVa=&eVn;
            break;
        default:
            break;
    }
}

void out_parametry_vector (LINIA *L, AVECTOR *V)
/*-------------------------------------------*/
{
    double l, a ;
    PLINIA PL ;
    double angle_l, angle2, PL_kat ;

    if (L == NULL)
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }
    parametry_linior (L, &PL) ;
    switch (V->style)
    {
        case 4:
            l=PL.dl*force_magnitude;
            if (eVf.st == NULL) break;

            eVf.st [0] = '\0' ;
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
                sprintf (eVf.st, format_float2, l, PL_kat);
            }
            else
            {
                sprintf (eVf.st, format_float, l) ;
            }
            Out_Edited_Draw_Param ((ESTR *)&eVf, TRUE) ;
            break;
        case 5:
        case 6:
            l = milimetryob (PL.dl) ;
            if (V->angle2<V->angle1) angle2=V->angle2+Pi2;
            else angle2=V->angle2;
            a=(V->angle2-V->angle1)*moment_magnitude;
            if (eVm.st == NULL) break;
            sprintf (eVm.st, format_float, a) ;
            Out_Edited_Draw_Param ((ESTR *)&eVm, TRUE) ;
            break;
        case 7:
            l=PL.dl*displacement_magnitude;
            if (eVd.st == NULL) break;
            eVd.st [0] = '\0' ;
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
                sprintf (eVd.st, format_float2, l, PL_kat);
            }
            else
            {
                sprintf (eVd.st, format_float, l) ;
            }
            Out_Edited_Draw_Param ((ESTR *)&eVd, TRUE) ;
            break;
        case 8:
        case 9:
            l = milimetryob (PL.dl) ;
            if (V->angle2<V->angle1) angle2=V->angle2+Pi2;
            else angle2=V->angle2;
            a=(V->angle2-V->angle1)*rotation_magnitude;
            if (eVr.st == NULL) break;
            sprintf (eVr.st, format_floatd, a) ;
            Out_Edited_Draw_Param ((ESTR *)&eVr, TRUE) ;
            break;
        default:
            l = milimetryob (PL.dl) ;
            break;
    }

}

void outarcvectoror (LUK *l, AVECTOR *V, int mode,int pl)
{
    if (l == NULL)
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }
    okno_r();
    setwritemode(mode);
    linestyle(l->typ);

    if (l->kolor == 0)
    {
        setfillstyle_(SOLID_FILL, kolory.paper);
        setcolor(kolory.paper);
        set_mode_solid();
    }
    else
    {
        SetColorAC(l->kolor);
        setfillstyle_(SOLID_FILL, GetColorAC(l->kolor));
    }

    V->x1=l->x;
    V->y1=l->y;
    V->r=l->r;
    V->angle1=l->kat1;
    V->angle2=l->kat2;
    //set_magnitude(V);
    V->magnitude1=0.0f;
    Draw_Vector(V, COPY_PUT, 1, 0);
}

void outvectoror (LINIA *L, AVECTOR *V, int mode,int pl)
/*---------------------------------------------------*/
{ LINIA L1;
    double angle_l;
    double P_Orto_Dir;
    double L1000, L2000;
    int grubosc;
    PLINIA PL;
    double angle1, angle2;

    if (L == NULL)
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }
    okno_r();
    setwritemode(mode);
    linestyle(L->typ);

    if (L->kolor == 0)
    {
        setfillstyle_(SOLID_FILL, kolory.paper);
        setcolor(kolory.paper);
        set_mode_solid();
    }
    else
    {
        SetColorAC(L->kolor);
        setfillstyle_(SOLID_FILL, GetColorAC(L->kolor));
    }

    if (orto)
    {
        L1.x1=L->x1;
        L1.y1=L->y1;
        L1.x2=L->x2;
        L1.y2=L->y2;
        Orto_Dir =  Set_Orto_Dir (L, Orto_Dir);
        orto_l(&L1, &Orto_Dir);
        switch (V->style)
        {
            case 5:
            case 6:
            case 8:
            case 9:
               linestyle(64);
               lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L1.x2),pikseleY0(L1.y2));
                parametry_lini(&L1, &PL);
               if (set_arc_stage==0)
               {
                   V->angle1=V->angle2=Pi_*PL.kat/180;
                   V->r=PL.dl;
               }
                   else if (set_arc_stage==1)
                   {
                       if ((V->style==5) || (V->style==8)) V->angle2=Pi_*PL.kat/180;
                       else V->angle1=Pi_*PL.kat/180;
                   }

               break;
            default:
                break;
        }
        V->x1=L->x1;
        V->y1=L->y1;
        V->x2=L1.x2;
        V->y2=L1.y2;
        set_magnitude(V);
        Draw_Vector(V, COPY_PUT, 1, 0);
        setlinestyle1(DOTTED_LINE,0,NORM_WIDTH);
        lineC(pikseleX0(L1.x2),pikseleY0(L1.y2),pikseleX0(L->x2),pikseleY0(L->y2));
    }
    else
    {
        switch (V->style)
        {
            case 5:
            case 6:
            case 8:
            case 9:
                linestyle(64);
                lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L->x2),pikseleY0(L->y2));

                parametry_lini(L, &PL);
                if (set_arc_stage==0)
                {
                    V->angle1=V->angle2=Pi_*PL.kat/180;
                    V->r=PL.dl;
                }
                else if (set_arc_stage==1)
                {
                    if ((V->style==5) || (V->style==8)) V->angle2=Pi_*PL.kat/180;
                    else V->angle1=Pi_*PL.kat/180;
                }
                else if (set_arc_stage==2)
                {
                    V->r=PL.dl;
                }
            default:
                break;
        }
        V->x1=L->x1;
        V->y1=L->y1;
        V->x2=L->x2;
        V->y2=L->y2;
        set_magnitude(V);
        Draw_Vector(V, COPY_PUT, 1, 0);
    }

    okno_all();
    switch (V->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            if (pl) out_parametry_lini(L);
            if (eVa) Out_Edited_Draw_Param (eVa, TRUE) ;
            break;
        case 4:
        case 7:
            out_parametry_vector(L, V);
            break;
        case 5:
        case 6:
        case 8:
        case 9:
            if (set_arc_stage==0)
            {
                if (pl) out_parametry_lini(L);
            }
            else out_parametry_vector(L, V);
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            if (pl) out_parametry_lini(L);
            if (eVa) Out_Edited_Draw_Param (eVa, TRUE) ;
            break;
        default:
            break;
    }
}


void outvectoror1 (LINIA *L, AVECTOR *V, int mode,int pl)
{       LINIA L1, LL;
        double angle;
        double P_Orto_Dir;
        double L1000, L2000;
        int grubosc;
        PLINIA PL, PL1;
        double angle0, angle1, angle2;
        double koc, kos;
        int n;
        double dl;

        if (L == NULL)
        {
            Internal_Error (__LINE__,__FILE__);
            return;
        }
        okno_r();
        setwritemode(mode);
        linestyle(L->typ);

        if (L->kolor == 0)
        {
            setfillstyle_(SOLID_FILL, kolory.paper);
            setcolor(kolory.paper);
            set_mode_solid();
        }
        else
        {
            SetColorAC(L->kolor);
            setfillstyle_(SOLID_FILL, GetColorAC(L->kolor));
        }

        if (orto)
        {
            L1.x1=LL.x1=L->x1;
            L1.y1=LL.y1=L->y1;
            L1.x2=LL.x2=L->x2;
            L1.y2=LL.y2=L->y2;
            Orto_Dir =  Set_Orto_Dir (L, Orto_Dir);
            orto_l(&L1, &Orto_Dir);
            linestyle(64);
            lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L1.x2),pikseleY0(L1.y2));
            parametry_lini(&L1, &PL);
            switch (V->style)
            {
                case 10:
                    if (Check_if_Equal(LL.x1, LL.x2))
                    {
                        LL.x2 += 0.001;
                    }
                case 13:
                    n=(LL.x1<LL.x2) ? 1 : -1;
                    if (V->flags & 1) n*=-1;
                    V->magnitude1=n*PL.dy*load_magnitude;
                    break;
                case 17:
                    if (Check_if_Equal(LL.x1, LL.x2))
                    {
                        LL.x2 += 0.001;
                    }
                    n=(LL.x1<LL.x2) ? 1 : -1;
                    if (V->flags & 1) n*=-1;
                    V->magnitude1=V->magnitude2=n*PL.dy*flood_magnitude;
                    break;
                case 11:
                    if (Check_if_Equal(LL.y1, LL.y2))
                    {
                        LL.y2 += 0.001;
                    }
                case 14:
                    n=(LL.y1>LL.y2) ? 1 : -1;
                    if (V->flags & 1) n*=-1;
                    V->magnitude1=n*PL.dx*load_magnitude;
                    break;
                case 12:
                    parametry_lini(V, &PL1);
                    angle0=Pi_*(PL.kat/180);
                    angle1=Pi_*(PL1.kat/180);
                    angle2=Angle_Normal(angle0+Pi_/2);
                    if (angle2<angle1) angle2+=Pi2;
                    angle=Angle_Normal(angle2-angle1);
                    koc=cos(angle);
                    dl=-PL.dl*koc;

                    n=1;
                    if (V->flags & 1) n*=-1;

                    V->magnitude1=n*dl*load_magnitude;
                    break;
                case 15:
                    parametry_lini(V, &PL1);
                    angle0=Pi_*(PL.kat/180);
                    angle1=Pi_*(PL1.kat/180);
                    angle2=Angle_Normal(angle0);
                    if (angle2<angle1) angle2+=Pi2;
                    angle=Angle_Normal(angle2-angle1);
                    koc=cos(angle);
                    dl=PL.dl*koc;

                    V->magnitude1=dl*thermal_magnitude;
                    break;
                default:
                    break;
            }
            Draw_Vector(V, COPY_PUT, 1, 0);
            setlinestyle1(DOTTED_LINE,0,NORM_WIDTH);
            lineC(pikseleX0(L1.x2),pikseleY0(L1.y2),pikseleX0(L->x2),pikseleY0(L->y2));
        }
        else
        {
            LL.x1=L->x1;
            LL.y1=L->y1;
            LL.x2=L->x2;
            LL.y2=L->y2;

            linestyle(64);
            lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L->x2),pikseleY0(L->y2));
            parametry_lini(L, &PL);
            switch (V->style)
            {
                case 10:
                    if (Check_if_Equal(LL.x1, LL.x2))
                    {
                        LL.x2 += 0.001;
                    }
                case 13:
                    n=(LL.x1<LL.x2) ? 1 : -1;
                    if (V->flags & 1) n*=-1;
                    V->magnitude1=n*PL.dy*load_magnitude;
                    break;
                case 17:
                    if (Check_if_Equal(LL.x1, LL.x2))
                    {
                        LL.x2 += 0.001;
                    }
                    n=(LL.x1<LL.x2) ? 1 : -1;
                    if (V->flags & 1) n*=-1;
                    V->magnitude1=V->magnitude2=n*PL.dy*flood_magnitude;
                    break;
                case 11:
                    if (Check_if_Equal(LL.y1, LL.y2))
                    {
                        LL.y2 += 0.001;
                    }
                case 14:
                    n=(LL.y1>LL.y2) ? 1 : -1;
                    if (V->flags & 1) n*=-1;
                    V->magnitude1=n*PL.dx*load_magnitude;
                    break;
                case 12:
                    parametry_lini(V, &PL1);
                    angle0=Pi_*(PL.kat/180);
                    angle1=Pi_*(PL1.kat/180);
                    angle2=Angle_Normal(angle0+Pi_/2);
                    if (angle2<angle1) angle2+=Pi2;
                    angle=Angle_Normal(angle2-angle1);
                    koc=cos(angle);
                    dl=-PL.dl*koc;

                    n=1;
                    if (V->flags & 1) n*=-1;

                    V->magnitude1=n*dl*load_magnitude;
                    break;
                case 15:
                    parametry_lini(V, &PL1);
                    angle0=Pi_*(PL.kat/180);
                    angle1=Pi_*(PL1.kat/180);
                    angle2=Angle_Normal(angle0);
                    if (angle2<angle1) angle2+=Pi2;
                    angle=Angle_Normal(angle2-angle1);
                    koc=cos(angle);
                    dl=PL.dl*koc;

                    V->magnitude1=dl*thermal_magnitude;
                    break;
                default:
                    break;
            }
            Draw_Vector(V, COPY_PUT, 1, 0);
        }

        okno_all();
        switch (V->style)
        {
            case 10:
            case 17:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
                sprintf(eVa->st,format_float, V->magnitude1);
                if (eVa) Out_Edited_Draw_Param (eVa, TRUE) ;
                break;
            default:
                break;
        }
}

void outvectoror2 (LINIA *L, AVECTOR *V, int mode,int pl)
{       LINIA L1, LL;
    double angle;
    double P_Orto_Dir;
    double L1000, L2000;
    int grubosc;
    PLINIA PL, PL1;
    double angle0, angle1, angle2;
    int n;
    double dl;
    double koc, kos;

    if (L == NULL)
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }
    okno_r();
    setwritemode(mode);
    linestyle(L->typ);

    if (L->kolor == 0)
    {
        setfillstyle_(SOLID_FILL, kolory.paper);
        setcolor(kolory.paper);
        set_mode_solid();
    }
    else
    {
        SetColorAC(L->kolor);
        setfillstyle_(SOLID_FILL, GetColorAC(L->kolor));
    }

    if (orto)
    {
        L1.x1=LL.x1=L->x1;
        L1.y1=LL.y1=L->y1;
        L1.x2=LL.x2=L->x2;
        L1.y2=LL.y2=L->y2;
        Orto_Dir =  Set_Orto_Dir (L, Orto_Dir);
        orto_l(&L1, &Orto_Dir);

        linestyle(64);
        lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L1.x2),pikseleY0(L1.y2));
        parametry_lini(&L1, &PL);
        switch (V->style)
        {
            case 10:
                if (Check_if_Equal(LL.x1, LL.x2))
                {
                    LL.x2 += 0.001;
                }
            case 13:
                n=(LL.x1<LL.x2) ? 1 : -1;
                if (V->flags & 1) n*=-1;
                V->magnitude2=n*PL.dy*load_magnitude;
                break;
            case 17:
                if (Check_if_Equal(LL.x1, LL.x2))
                {
                    LL.x2 += 0.001;
                }
                n=(LL.x1<LL.x2) ? 1 : -1;
                if (V->flags & 1) n*=-1;
                V->magnitude2=V->magnitude1=n*PL.dy*flood_magnitude;
                break;
            case 11:
                if (Check_if_Equal(LL.y1, LL.y2))
                {
                    LL.y2 += 0.001;
                }
            case 14:
                n=(LL.y1>LL.y2) ? 1 : -1;
                if (V->flags & 1) n*=-1;
                V->magnitude2=n*PL.dx*load_magnitude;
                break;
            case 12:
                parametry_lini(V, &PL1);
                angle0=Pi_*(PL.kat/180);
                angle1=Pi_*(PL1.kat/180);
                angle2=Angle_Normal(angle0+Pi_/2);
                if (angle2<angle1) angle2+=Pi2;
                angle=Angle_Normal(angle2-angle1);
                koc=cos(angle);
                dl=-PL.dl*koc;

                n=1;
                if (V->flags & 1) n*=-1;

                V->magnitude2=n*dl*load_magnitude;
                break;
            case 15:
                parametry_lini(V, &PL1);
                angle0=Pi_*(PL.kat/180);
                angle1=Pi_*(PL1.kat/180);
                angle2=Angle_Normal(angle0);
                if (angle2<angle1) angle2+=Pi2;
                angle=Angle_Normal(angle2-angle1);
                koc=cos(angle);
                dl=PL.dl*koc;

                V->magnitude2=dl*thermal_magnitude;
                break;
            default:
                break;
        }
        Draw_Vector(V, COPY_PUT, 1, 0);
        setlinestyle1(DOTTED_LINE,0,NORM_WIDTH);
        lineC(pikseleX0(L1.x2),pikseleY0(L1.y2),pikseleX0(L->x2),pikseleY0(L->y2));
    }
    else
    {
        LL.x1=L->x1;
        LL.y1=L->y1;
        LL.x2=L->x2;
        LL.y2=L->y2;

        linestyle(64);
        lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L->x2),pikseleY0(L->y2));
        parametry_lini(L, &PL);
        switch (V->style)
        {
            case 10:
                if (Check_if_Equal(LL.x1, LL.x2))
                {
                    LL.x2 += 0.001;
                }
            case 13:
                n=(LL.x1<LL.x2) ? 1 : -1;
                if (V->flags & 1) n*=-1;
                V->magnitude2=n*PL.dy*load_magnitude;
                break;
            case 17:
                if (Check_if_Equal(LL.x1, LL.x2))
                {
                    LL.x2 += 0.001;
                }
                n=(LL.x1<LL.x2) ? 1 : -1;
                if (V->flags & 1) n*=-1;
                V->magnitude2=V->magnitude1=n*PL.dy*flood_magnitude;
                break;
            case 11:
                if (Check_if_Equal(LL.y1, LL.y2))
                {
                    LL.y2 += 0.001;
                }
            case 14:
                n=(LL.y1>LL.y2) ? 1 : -1;
                if (V->flags & 1) n*=-1;
                V->magnitude2=n*PL.dx*load_magnitude;
                break;
            case 12:
                parametry_lini(V, &PL1);
                angle0=Pi_*(PL.kat/180);
                angle1=Pi_*(PL1.kat/180);
                angle2=Angle_Normal(angle0+Pi_/2);
                if (angle2<angle1) angle2+=Pi2;
                angle=Angle_Normal(angle2-angle1);
                koc=cos(angle);
                dl=-PL.dl*koc;

                n=1;
                if (V->flags & 1) n*=-1;

                V->magnitude2=n*dl*load_magnitude;
                break;
            case 15:
                parametry_lini(V, &PL1);
                angle0=Pi_*(PL.kat/180);
                angle1=Pi_*(PL1.kat/180);
                angle2=Angle_Normal(angle0);
                if (angle2<angle1) angle2+=Pi2;
                angle=Angle_Normal(angle2-angle1);
                koc=cos(angle);
                dl=PL.dl*koc;

                V->magnitude2=dl*thermal_magnitude;
                break;
            default:
                break;
        }
        Draw_Vector(V, COPY_PUT, 1, 0);
    }

    okno_all();
    switch (V->style)
    {
        case 10:
        case 17:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            sprintf(eVa->st,format_float, V->magnitude2);
            if (eVa) Out_Edited_Draw_Param (eVa, TRUE) ;
            break;
        default:
            break;
    }
}

void outvectoror3 (LINIA *L, AVECTOR *V, int mode,int pl)
{
    LINIA L1, Lth;
    double angle;
    double P_Orto_Dir;
    double L1000, L2000;
    int grubosc;
    PLINIA PL, PL1, PLth;
    double angle0, angle1, angle2;
    int n;
    double dl;
    double koc, kos, koc1, kos1;

    if (L == NULL)
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }
    okno_r();
    setwritemode(mode);
    linestyle(L->typ);

    if (L->kolor == 0)
    {
        setfillstyle_(SOLID_FILL, kolory.paper);
        setcolor(kolory.paper);
        set_mode_solid();
    }
    else
    {
        SetColorAC(L->kolor);
        setfillstyle_(SOLID_FILL, GetColorAC(L->kolor));
    }

    if (orto)
    {
        L1.x1=L->x1;
        L1.y1=L->y1;
        L1.x2=L->x2;
        L1.y2=L->y2;
        Orto_Dir =  Set_Orto_Dir (L, Orto_Dir);
        orto_l(&L1, &Orto_Dir);

        linestyle(64);
        lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L1.x2),pikseleY0(L1.y2));
        parametry_lini(&L1, &PL);

        switch (V->style)
        {
            case 15:
                parametry_lini(V, &PL1);

                kos1=sin(Pi*(PL1.kat+90)/180);
                koc1=cos(Pi*(PL1.kat+90)/180);

                Lth.x1 = (V->x1+V->x2)/2 + (V->r/depth_magnitude)*koc1; //thermal_depth_size
                Lth.y1 = (V->y1+V->y2)/2 + (V->r/depth_magnitude)*kos1;
                Lth.x2 = (V->x1+V->x2)/2 - (V->r/depth_magnitude)*koc1;
                Lth.y2 = (V->y1+V->y2)/2 - (V->r/depth_magnitude)*kos1;

                parametry_lini(&Lth, &PLth);

                angle0=Pi_*(PL.kat/180);
                angle1=Pi_*(PLth.kat/180);
                angle2=Angle_Normal(angle0);
                if (angle2<angle1) angle2+=Pi2;
                angle=Angle_Normal(angle2-angle1);
                koc=cos(angle);
                dl=PL.dl*koc;

                V->r=fabs(dl);
                break;
            default:
                break;
        }
        Draw_Vector(V, COPY_PUT, 1, 0);
        setlinestyle1(DOTTED_LINE,0,NORM_WIDTH);
        lineC(pikseleX0(L1.x2),pikseleY0(L1.y2),pikseleX0(L->x2),pikseleY0(L->y2));
    }
    else
    {
        linestyle(64);
        lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L->x2),pikseleY0(L->y2));
        parametry_lini(L, &PL);
        switch (V->style)
        {
            case 15:
                parametry_lini(V, &PL1);

                kos1=sin(Pi*(PL1.kat+90)/180);
                koc1=cos(Pi*(PL1.kat+90)/180);

                Lth.x1 = (V->x1+V->x2)/2 + (V->r/depth_magnitude)*koc1; //thermal_depth_size
                Lth.y1 = (V->y1+V->y2)/2 + (V->r/depth_magnitude)*kos1;
                Lth.x2 = (V->x1+V->x2)/2 - (V->r/depth_magnitude)*koc1;
                Lth.y2 = (V->y1+V->y2)/2 - (V->r/depth_magnitude)*kos1;

                parametry_lini(&Lth, &PLth);

                angle0=Pi_*(PL.kat/180);
                angle1=Pi_*(PLth.kat/180);
                angle2=Angle_Normal(angle0);
                if (angle2<angle1) angle2+=Pi2;
                angle=Angle_Normal(angle2-angle1);
                koc=cos(angle);
                dl=PL.dl*koc;

                V->r=fabs(dl);
                break;
            default:
                break;
        }
        Draw_Vector(V, COPY_PUT, 1, 0);
    }

    okno_all();
    switch (V->style)
    {
        case 15:
            sprintf(eVa->st,format_float, V->r);
            if (eVa) Out_Edited_Draw_Param (eVa, TRUE) ;
            break;
        default:
            break;
    }
}


void outvectoror4 (LINIA *L, AVECTOR *V, int mode,int pl)
{
    LINIA L1, Lth;
    double angle;
    double P_Orto_Dir;
    double L1000, L2000;
    int grubosc;
    PLINIA PL, PL1, PLth;
    double angle0, angle1, angle2;
    int n;
    double dl;
    double koc, kos, koc1, kos1;

    if (L == NULL)
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }
    okno_r();
    setwritemode(mode);
    linestyle(L->typ);

    if (L->kolor == 0)
    {
        setfillstyle_(SOLID_FILL, kolory.paper);
        setcolor(kolory.paper);
        set_mode_solid();
    }
    else
    {
        SetColorAC(L->kolor);
        setfillstyle_(SOLID_FILL, GetColorAC(L->kolor));
    }

    if (orto)
    {
        L1.x1=L->x1;
        L1.y1=L->y1;
        L1.x2=L->x2;
        L1.y2=L->y2;
        Orto_Dir =  Set_Orto_Dir (L, Orto_Dir);
        orto_l(&L1, &Orto_Dir);

        linestyle(64);
        lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L1.x2),pikseleY0(L1.y2));
        parametry_lini(&L1, &PL);

        V->r=PL.dl*radius_magnitude;
        V->magnitude1 = milimetryob(PL.dl*radius_magnitude);

        Draw_Vector(V, COPY_PUT, 1, 0);
        setlinestyle1(DOTTED_LINE,0,NORM_WIDTH);
        lineC(pikseleX0(L1.x2),pikseleY0(L1.y2),pikseleX0(L->x2),pikseleY0(L->y2));
    }
    else
    {
        linestyle(64);
        lineC(pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L->x2),pikseleY0(L->y2));
        parametry_lini(L, &PL);

        V->r=PL.dl*radius_magnitude;
        V->magnitude1 = milimetryob(PL.dl*radius_magnitude);

        Draw_Vector(V, COPY_PUT, 1, 0);
    }

    okno_all();
    eVa=&eVn;
    switch (V->style)
    {
        case 16:
            sprintf(eVa->st,format_float, milimetryob(V->r));
            if (eVa) Out_Edited_Draw_Param (eVa, TRUE) ;
            break;
        default:
            break;
    }
}


static void  cur_off (double x,double y)
/*----------------------------------------*/
{
    flip_screen();
}

static void  cur_on(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    outvectoror (&LiniaG, &VectorG, COPY_PUT, 1) ;
    cursor_on(x, y);
}


#define r18 18
#define r22 16 //22
static int strwyj;
static double Lx2,Ly2,Lz2 ;
static double df__xbeg, df__ybeg ;
static double df__len = 0 ;


void get_Lx2_Ly2(double *lx, double *ly)
{

}

static void set_vector_len (void)
/*---------------------------*/
{
  char sk [MaxTextLen], *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;

  if (EOF == sprintf(sk, "%-8.4f", df__len))
   {
    strcpy (sk, "");
   }
  if (NULL != (str = strchr (sk, ' ')))
   {
    str [0] = '\0';
   }

  if (!get_string (sk, "", MaxTextLen, 0, 48)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)
	|| retval_no < 1 || buf_ret [0] <= 0)
  {
    return ;
  }
  df__len = buf_ret [0] ;
  if (df__len > 100000l)
  {
    df__len = 100000l ;
  }
  if (EOF == sprintf(sk, "%-6.2f", df__len))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  sk [6] = '\0' ;
  menu_par_new ((*mVector.pola)[2].txt, sk) ;
}


static BOOL check_if_break_vector (void *ptr_data, AVECTOR **ptrs_begin_vector, AVECTOR **ptrs_end_vector, int *ptri_vectorno)
/*------------------------------------------------------------------------------------------------------------------------*/
{
  long_long off, offk, ad ;
  NAGLOWEK *nag ;
  AVECTOR *ptrs_vector ;

  nag = (NAGLOWEK*)ptr_data ;
  if (nag->obiekt != Ovector)
  {
    return FALSE ;
  }
  *ptri_vectorno = 0 ;
  off =  (char  *)nag - dane ;
  offk = dane_size - 1 ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    if ((nag->obiekt == Ovector) &&
	(nag->atrybut == Anormalny || nag->atrybut == Aoblok || nag->atrybut==Apoblok
      || nag->atrybut==Appoblok || nag->atrybut==Apppoblok ))
    {
      ptrs_vector = (AVECTOR*)nag ;
      if (*ptri_vectorno == 0 ||
	 (TRUE == Check_if_Equal (ptrs_vector->x2, (*ptrs_begin_vector)->x1) &&
	   TRUE == Check_if_Equal (ptrs_vector->y2, (*ptrs_begin_vector)->y1)))
      {
          if (*ptri_vectorno == 1)
          {
              *ptrs_begin_vector = ptrs_vector;
              *ptrs_end_vector = ptrs_vector ;
          }
          else  *ptrs_begin_vector = ptrs_vector ;
	(*ptri_vectorno)++ ;
      }
      else
      {
	break ;
      }
    }
    else
    {
      break ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  return (*ptri_vectorno > 0) ? TRUE : FALSE ;
}


//static
int vector_command_proc (int ev_nr)
/*--------------------------------------*/
{
  int ret_val ;
  int orto_temp ;
  AVECTOR *ptrs_vector, *ptrs_begin_vector, *ptrs_end_vector ;
  int i_vectorno ;

  if (ev_nr>=mVector.max)
  {
      if ((mVector.foff+mVector.poz)==16)
      {
          if ((mLoad_Char_Thermal.foff+mLoad_Char_Thermal.poz)>0)
              VectorG.load=mLoad_Char_Thermal.foff+mLoad_Char_Thermal.poz+9;
          else VectorG.load=0;
      }
      else VectorG.load=mLoad_Char.foff+mLoad_Char.poz;
      if (VectorG.load>0)
          VectorG.variant=mLoadVariant.foff+mLoadVariant.poz;
      else VectorG.variant=0;
  }
  else
  {
      VectorG.load=0;
      VectorG.variant=0;
  }

  ev_nr=mVector.foff+mVector.poz;

  switch (ev_nr)
  {
    case IDM_UNDO :
      if (FALSE == check_if_break_vector ((void *)dane,
	    &ptrs_begin_vector, &ptrs_end_vector, &i_vectorno))
      {
	    ret_val = PL_MODE_CONTINUE ;
      }
      else
      {
	ptrs_vector = (AVECTOR*)dane ;
	if (TRUE == Check_if_Equal (ptrs_vector->x2, LiniaG.x1) &&
	    TRUE == Check_if_Equal (ptrs_vector->y2, LiniaG.y1))
	{
        if (i_vectorno > 1)
        {
            LiniaG.x1 = ptrs_end_vector->x2 ;
            LiniaG.y1 = ptrs_end_vector->y2 ;
            ret_val = PL_MODE_CONTINUE ;
        }
        else
        {
            LiniaG.x1 = LiniaG.x2 ;
            LiniaG.y1 = LiniaG.y2 ;
            ret_val = PL_MODE_END ;
        }

	  rysuj_obiekt ((char*)dane, COPY_PUT, 0) ;
	  Usun_Object ((char*)dane, TRUE) ;

      CUR_OFF (X, Y) ;
	  CUR_ON (X, Y) ;
	}
      }
      break ;
      case IDM_RIGID_RIGID:
          VectorG.style=0;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_RIGID_PIN:
          VectorG.style=1;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_PIN_RIGID:
          VectorG.style=2;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_PIN_PIN:
          VectorG.style=3;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_FORCE:
          VectorG.style=4;
          ret_val = PL_MODE_END ;
          break;
      case IDM_MOMENT:
          VectorG.style=5;
          ret_val = PL_MODE_END ;
          break;
      case IDM_MOMENT_REV:
          VectorG.style=6;
          ret_val = PL_MODE_END ;
          break;
      case IDM_DISPLACEMENT:
          VectorG.style=7;
          ret_val = PL_MODE_END ;
          break;
      case IDM_ROTATION:
          VectorG.style=8;
          ret_val = PL_MODE_END ;
          break;
      case IDM_ROTATION_REV:
          VectorG.style=9;
          ret_val = PL_MODE_END ;
          break;
      case IDM_TRAPEZIUM_Y:
          VectorG.style=10;
          ret_val = PL_MODE_END ;
          break;
      case IDM_TRAPEZIUM_X:
          VectorG.style=11;
          ret_val = PL_MODE_END ;
          break;
      case IDM_TRAPEZIUM_N:
          VectorG.style=12;
          ret_val = PL_MODE_END ;
          break;
      case IDM_TRAPEZIUM_H:
          VectorG.style=13;
          ret_val = PL_MODE_END ;
          break;
      case IDM_TRAPEZIUM_V:
          VectorG.style=14;
          ret_val = PL_MODE_END ;
          break;
      case IDM_THERMAL:
          VectorG.style=15;
          VectorG.r=thermal_axis_size;
          ret_val = PL_MODE_END ;
          break;
      case IDM_NODE:
          VectorG.style=16;

          VectorG.magnitude1 = (float)node_size;
          VectorG.r = jednostkiOb(node_size);

          ret_val = PL_MODE_END ;
          break;
      case IDM_SLAB_PLATE:
          VectorG.style=V_SLAB_PLATE;  //fictional
          ret_val = PL_MODE_END ;
          break;
      case IDM_SLAB_SPACE:
          VectorG.style=V_SLAB_SPACE;  //fictional
          ret_val = PL_MODE_END ;
          break;
      case IDM_SLAB_WALL:
          VectorG.style=V_SLAB_WALL;  //fictional
          ret_val = PL_MODE_END ;
          break;
      case IDM_SLAB_ZONE:
          VectorG.style=V_SLAB_ZONE;  //fictional
          ret_val = PL_MODE_END ;
          break;
      case IDM_SLAB_LOAD:
          VectorG.style=17;  //real
          ret_val = PL_MODE_END ;
          break;
    default :
      ret_val = PL_MODE_CONTINUE ;
      break ;
  }
  return ret_val ;
}


static BOOL add_vector (BOOL b_strwyj)
/*----------------------------------*/
{
    float Lx3, Ly3, Lx4, Ly4;
    BOOL b_ret = FALSE;
    char sstt0[32];
    char sstt01[32];
    double angle_l;
    PLINIA PL;
    LINIA L1;
    double P_Orto_Dir;
    double kat1, kos, koc;
    double magnitude;
    double x2,y2;

    if (b_strwyj) {
        LiniaG.x2 = Double_to_Float(Lx2);
        LiniaG.y2 = Double_to_Float(Ly2);
    } else {
        LiniaG.x2 = X;
        LiniaG.y2 = Y;
    }

    if (orto) {
        orto_l(&LiniaG, &Orto_Dir);
        Orto_Dir = I_Orto_NoDir;
    }

    if (LiniaG.x1 == LiniaG.x2 && LiniaG.y1 == LiniaG.y2) {
        return TRUE;
    }

    VectorG.warstwa = LiniaG.warstwa;
    VectorG.kolor = LiniaG.kolor;
    VectorG.typ = LiniaG.typ;

    if (b_strwyj)
    {
        switch (VectorG.style) {
            case 0:
            case 1:
            case 2:
            case 3:
                VectorG.r=0;
                break;
            case 4:  //force
                VectorG.r=0;
                break;
            case 5:  //moment
            case 6:  //moment
                break;
            case 7:  //displacement
                VectorG.r=0;
                break;
            case 8:  //rotation
            case 9:  //rotation
                break;
            case 10:
            case 17:
            case 11:
            case 12:
            case 13:
            case 14:
                VectorG.r=0;
                break;
            default:
                break;
        }

    }

    switch (VectorG.style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            VectorG.x1=LiniaG.x1;
            VectorG.y1=LiniaG.y1;
            VectorG.x2=LiniaG.x2;
            VectorG.y2=LiniaG.y2;
            break;
        case 5:
        case 6:
        case 8:
        case 9:
            break;
        case 10:
        case 17:
        case 11:
        case 12:
        case 13:
        case 14:
            VectorG.x1=LiniaG.x1;
            VectorG.y1=LiniaG.y1;
            VectorG.x2=LiniaG.x2;
            VectorG.y2=LiniaG.y2;
            break;
        default:
            break;
    }

    if (!b_strwyj) set_magnitude(&VectorG);
    else set_length_angle2(&VectorG);

  if (NULL != dodaj_obiekt (NULL, &VectorG))
  {
    
    rysuj_obiekt ((char*)&VectorG, COPY_PUT, 1) ;
    
    LiniaG.x1 = LiniaG.x2 ;
    LiniaG.y1 = LiniaG.y2 ;
    LiniaG.x2 = X ;
    LiniaG.y2 = Y ;
    b_ret = TRUE ;
  }

    if ((b_strwyj) && (VectorG.reserve1>0)) VectorG.style=VectorG.reserve1;
    VectorG.reserve1=0;

    return b_ret ;
}

void Redraw_Vector (BOOL b_draw)
/*----------------------------*/
{
    AVECTOR *ptrs_vector ;
    NAGLOWEK *nag ;
    long_long ad, off, offk ;

    Set_Second_Screen();
    okno_r_second () ;
    setwritemode (COPY_PUT) ;
    if (b_draw == FALSE)
    {
        off = 0 ;
        offk = dane_size - 1 ;
        ObiektTok (&off, offk, &ad, Ovector) ;
        while (ad != -1)
        {
            ptrs_vector = (AVECTOR*)(dane + ad) ;
            if (ptrs_vector->widoczny == 1)
            {
                rysuj_obiekt_ ((char *)ptrs_vector, COPY_PUT, 0) ;
            }
            ObiektTok (&off, offk, &ad, Ovector);
        }
    }
    else
    {
        off = 0 ;
        offk = dane_size - 1 ;
        ObiektTok (&off, offk, &ad, Ovector) ;
        while (ad != -1)
        {
            ptrs_vector = (AVECTOR*)(dane + ad) ;
            if ((1 == (ptrs_vector->widoczny = Vector_Selected(ptrs_vector))))
            {
                rysuj_obiekt_ ((char *)ptrs_vector, COPY_PUT, 1) ;
            }
            ObiektTok (&off, offk, &ad, Ovector);
        }
    }
    okno_all_second () ;
    Set_Screen();
}


void Resize_Vector (void)
/*----------------------*/
{
    AVECTOR *ptrs_vector ;
    NAGLOWEK *nag ;
    long_long ad, off, offk ;
    PLINIA PL;
    double kat, kos, koc;
    double x0, y0, x2, y2;
    double dangle;

    off = 0 ;
    offk = dane_size - 1 ;
    ObiektTok (&off, offk, &ad, Ovector) ;
    while (ad != -1)
    {
        ptrs_vector = (AVECTOR*)(dane + ad) ;
        switch (ptrs_vector->style)
        {
            case 4:
                parametry_lini((LINIA*)ptrs_vector, &PL);
                kat = PL.kat;
                kos = sin(PL.kat * Pi / 180);
                koc = cos(PL.kat * Pi / 180);
                x0 = ptrs_vector->x1 + (ptrs_vector->magnitude1 / force_magnitude);
                y0 = ptrs_vector->y1;
                Rotate_Point(kos, koc, ptrs_vector->x1, ptrs_vector->y1, x0, y0, &x2, &y2);
                ptrs_vector->x2=(float)x2;
                ptrs_vector->y2=(float)y2;
                break;
            case 5:
            case 6:
                magnitude2angle(ptrs_vector, moment_magnitude);
                break;
            case 7:
                parametry_lini((LINIA*)ptrs_vector, &PL);
                kat = PL.kat;
                kos = sin(PL.kat * Pi / 180);
                koc = cos(PL.kat * Pi / 180);
                x0 = ptrs_vector->x1 + (ptrs_vector->magnitude1 / displacement_magnitude);
                y0 = ptrs_vector->y1;
                Rotate_Point(kos, koc, ptrs_vector->x1, ptrs_vector->y1, x0, y0, &x2, &y2);
                ptrs_vector->x2=(float)x2;
                ptrs_vector->y2=(float)y2;
                break;
            case 8:
            case 9:
                magnitude2angle(ptrs_vector, rotation_magnitude);
                break;
            default:
                break;

        }
        ObiektTok (&off, offk, &ad, Ovector);
    }
}


static int L_p(BOOL b_graph_value)
/*-------------------------------*/
{
  PLINIA PL ;
  double l, k ;
  double df_x, df_y ;
  double angle_l ;

  if (eL.val_no < 1) return 0;
  CUR_OFF (X, Y) ;
  l = eL.values [0] ;
  if ( orto == 1 || eL.val_no == 1)
  {
    if (TRUE == b_graph_value)
    {
       Get_Graph_Values_Cur_Pos (&df_x, &df_y) ;
       LiniaG.x2 = df_x ;
       LiniaG.y2 = df_y ;
    }
    parametry_linior (&LiniaG, &PL) ;
    k = PL.kat ;
  }
  else
  {
    angle_l=get_angle_l();
    if (angle_l!=0)
    {
     k = eL.values [1] + angle_l;
     if (k<0) k+=360;
    }
     else
       k = eL.values [1] ;
  }
  l = jednostkiOb(l);
  k = Grid_to_Rad (k) ;
  Lx2 = LiniaG.x1 + l * cos (k) ;
  Ly2 = LiniaG.y1 + l * sin (k) ;
  strwyj = 1;
  CUR_ON (X, Y) ;
  return 1;
}

static int last_vector_delete (void)
/*------------------------*/
{
    if (PL_MODE_CONTINUE != vector_command_proc (IDM_UNDO))
    {
        redcr (1) ;
        return -84;
    }
    return -83;
}

int Ve_n (BOOL b_graph_value)
/*--------------------------------*/
{
    int n;

    b_graph_value = b_graph_value ;
    if (eVe.val_no < 1) return 0 ;

    n = eVe.values [0] ;
    if (n > -1 && n < 32767)
    {
        VectorG.property_no = n ;
        CUR_OFF(X,Y);
        CUR_ON(X,Y);
        return 1 ;
    }
    else
    {
        ErrList (68) ;
        return 0 ;
    }
}

static int Vf_n__(BOOL b_graph_value)
/*-------------------------------*/
{
    PLINIA PL ;
    double l, k ;
    double angle_l;

    b_graph_value = b_graph_value ;
    if (eL.val_no < 1) return 0;
    l = eL.values [0] ;
    if ( orto != 0 || eL.val_no == 1)
    {
        parametry_linior (&LiniaG, &PL) ;
        k = PL.kat ;
    }
    else
    {
        angle_l=get_angle_l();
        if (angle_l!=0)
        {
            k = eL.values [1] + angle_l;
            if (k<0) k+=360;
        }
        else
            k = eL.values [1] ;
    }
    l = jednostkiOb(l);
    k = Grid_to_Rad (k) ;
    Lx2 = LiniaG.x1 + l * cos (k) ;
    Ly2 = LiniaG.y1 + l * sin (k) ;
    strwyj = 1;
    return 1;
}

int Vf_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m, l, k;
    PLINIA PL ;
    double df_x, df_y ;
    double angle_l ;

    b_graph_value = b_graph_value ;
    if (eVf.val_no < 1) return 0 ;

    m = eVf.values [0] ;

    if (m > -3.4E38 && m < 3.4E38)
    {
        if ( orto == 1 || eVf.val_no == 1)
        {
            if (TRUE == b_graph_value)
            {
                Get_Graph_Values_Cur_Pos (&df_x, &df_y) ;
                LiniaG.x2 = df_x ;
                LiniaG.y2 = df_y ;
            }
            parametry_linior (&LiniaG, &PL) ;
            k = PL.kat ;
        }
        else
        {
            angle_l=get_angle_l();
            if (angle_l!=0)
            {
                k = eVf.values [1] + angle_l;
                if (k<0) k+=360;
            }
            else
                k = eVf.values [1] ;
        }
        l = m/force_magnitude;
        k = Grid_to_Rad (k) ;
        Lx2 = LiniaG.x1 + l * cos (k) ;
        Ly2 = LiniaG.y1 + l * sin (k) ;

        VectorG.x2=Lx2;
        VectorG.y2=Ly2;

        VectorG.magnitude1 = (float)fabs(m) ;
        strwyj = 1;
        return 1 ;
    }
    else
    {
        ErrList (9) ;
        return 0 ;
    }
}

int Vm_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m;

    b_graph_value = b_graph_value ;
    if (eVm.val_no < 1)
    {
        return 0 ;
    }
    m = eVm.values [0] ;
    if (m > -3.4E38 && m < 3.4E38)
    {
        if (m<0)
        {
            if (VectorG.style==5)
            {
                VectorG.reserve1=5;
                VectorG.style=6;
            }
            else if (VectorG.style==6)
            {
                VectorG.reserve1=6;
                VectorG.style=5;
            }
        }
        VectorG.magnitude1 = (float)fabs(m) ;
        strwyj = 1;
        return 1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
}

int Vf1_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m;

    b_graph_value = b_graph_value ;
    if (eVf.val_no < 1)
    {
        return 0 ;
    }
    m = eVf.values [0] ;
    if (m > -3.4E38 && m < 3.4E38)
    {
        VectorG.magnitude1 = (float)m ;
        strwyj = 1;
        return 1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
}

int Vf2_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m;

    b_graph_value = b_graph_value ;
    if (eVf.val_no < 1)
    {
        return 0 ;
    }
    m = eVf.values [0] ;
    if (m > -3.4E38 && m < 3.4E38)
    {
        VectorG.magnitude2 = (float)m ;
        strwyj = 1;
        return 1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
}

int Vth1_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m;

    b_graph_value = b_graph_value ;
    if (eVf.val_no < 1)
    {
        return 0 ;
    }
    m = eVf.values [0] ;
    if (m > -3.4E38 && m < 3.4E38)
    {
        VectorG.r = (float)m ;
        strwyj = 1;
        return 1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
}

int Vr_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m;

    b_graph_value = b_graph_value ;
    if (eVr.val_no < 1)
    {
        return 0 ;
    }
    m = eVr.values [0] ;
    if (m > -3.4E38 && m < 3.4E38)
    {
        if (m<0)
        {
            if (VectorG.style==8)
            {
                VectorG.reserve1=8;
                VectorG.style=9;
            }
            else if (VectorG.style==9)
            {
                VectorG.reserve1=9;
                VectorG.style=8;
            }
        }
        VectorG.magnitude1 = (float)fabs(m) ;
        strwyj = 1;
        return 1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
}

int Vn_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m;

    b_graph_value = b_graph_value ;
    if (eVn.val_no < 1)
    {
        return 0 ;
    }
    m = eVn.values [0] ;
    if (m > 0 && m < 3.4E38)
    {
        VectorG.magnitude1 = (float)m;
        VectorG.r = jednostkiOb(m);
        node_size=m;

        CUR_OFF(X,Y);
        CUR_ON(X,Y);
        return 1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
}

int Vd_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m, l, k;
    PLINIA PL ;
    double df_x, df_y ;
    double angle_l ;

    b_graph_value = b_graph_value ;
    if (eVd.val_no < 1) return 0 ;

    m = eVd.values [0] ;

    if (m > -3.4E38 && m < 3.4E38)
    {
        if ( orto == 1 || eVd.val_no == 1)
        {
            if (TRUE == b_graph_value)
            {
                Get_Graph_Values_Cur_Pos (&df_x, &df_y) ;
                LiniaG.x2 = df_x ;
                LiniaG.y2 = df_y ;
            }
            parametry_linior (&LiniaG, &PL) ;
            k = PL.kat ;
        }
        else
        {
            angle_l=get_angle_l();
            if (angle_l!=0)
            {
                k = eVd.values [1] + angle_l;
                if (k<0) k+=360;
            }
            else
                k = eVd.values [1] ;
        }
        l = m/displacement_magnitude;
        k = Grid_to_Rad (k) ;
        Lx2 = LiniaG.x1 + l * cos (k) ;
        Ly2 = LiniaG.y1 + l * sin (k) ;

        VectorG.x2=Lx2;
        VectorG.y2=Ly2;

        VectorG.magnitude1 = (float)fabs(m) ;
        strwyj = 1;
        return 1 ;
    }
    else
    {
        ErrList (9) ;
        return 0 ;
    }
}

static int Vd_n__ (BOOL b_graph_value)
/*--------------------------------*/
{
    double d;

    b_graph_value = b_graph_value ;
    if (eVd.val_no < 1)
    {
        return 0 ;
    }
    d = eVd.values [0] ;
    if (d > -3.4E38 && d < 3.4E38)
    {
        VectorG.magnitude1 = (float)d ;
        strwyj = 1;
        return 1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
}

static int Vt_n (BOOL b_graph_value)
/*--------------------------------*/
{
    double m1, m2;

    b_graph_value = b_graph_value ;
    if (eVt.val_no < 1)
    {
        return 0 ;
    }
    m1 = eVt.values [0] ;
    if (m1 > -3.4E38 && m1 < 3.4E38)
    {
        VectorG.magnitude1 = (float)m1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
    if (eVt.val_no > 1)
    {
        m2 = eVt.values [1] ;
        if (m2 > -3.4E38 && m2 < 3.4E38)
        {
            VectorG.magnitude2 = (float)m2;
            CUR_OFF(X,Y);
            CUR_ON(X,Y);
            return 1;
        } else
        {
            ErrList(2);
            return 0;
        }
    }
    else VectorG.magnitude2 = (float)m1;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
    return 1;
}

static int Vt_n1 (BOOL b_graph_value)
/*--------------------------------*/
{
    double m1, m2;

    b_graph_value = b_graph_value ;
    if (eVt1.val_no < 1)
    {
        return 0 ;
    }
    m1 = eVt1.values [0] ;
    if (m1 > -3.4E38 && m1 < 3.4E38)
    {
        VectorG.magnitude1 = VectorG.magnitude2 = (float)m1 ;
    }
    else
    {
        ErrList (2) ;
        return 0 ;
    }
    /*
    if (eVt1.val_no > 1)
    {
        m2 = eVt1.values [1] ;
        if (m2 > -3.4E38 && m2 < 3.4E38)
        {
            VectorG.magnitude2 = (float)m2;
            CUR_OFF(X,Y);
            CUR_ON(X,Y);
            return 1;
        } else
        {
            ErrList(2);
            return 0;
        }
    }
    else
     */
    VectorG.magnitude2 = VectorG.magnitude1 = (float)m1;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
    return 1;
}


void ini_vector_estr(void)
{
    eVe.x=maxX/2 - (6*WIDTH) ;
    eVe.y= ESTR_Y;
    eVe.val_no_max = 1;
    eVe.mode = GV_NUMBER ;
    eVe.format = "\001%#6d" ;
    eVe.lmax=6;
    eVe.ESTRF=Ve_n;
    eVe.extend = 0;

    eVf.x=maxX/2 + 5 ;
    eVf.y= ESTR_Y;
    eVf.lmax=16;
    eVf.val_no_max	= 2 ;
    eVf.mode	= GV_VECTOR	;
    eVf.format = format2_float2;
    eVf.ESTRF=Vf_n;
    eVf.extend = 0;

    eVm.x=maxX/2 + 5 ;
    eVm.y= ESTR_Y;
    eVm.lmax=12;
    eVm.val_no_max	= 1 ;
    eVm.mode	= GV_DIST	;
    eVm.format = format_float;
    eVm.ESTRF=Vm_n;
    eVm.extend = 0;

    eVd.x=maxX/2  + 5;
    eVd.y= ESTR_Y;
    eVd.lmax=16;
    eVd.val_no_max	= 2 ;
    eVd.mode	= GV_VECTOR	;
    eVd.format = format2_float2;
    eVd.ESTRF=Vd_n;
    eVd.extend = 0;

    eVr.x=maxX/2 + 5 ;
    eVr.y= ESTR_Y;
    eVr.lmax=12;
    eVr.val_no_max	= 1 ;
    eVr.mode	= GV_DIST	;
    eVr.format = format_floatd;
    eVr.ESTRF=Vr_n;
    eVr.extend = 0;

    eVt.x=maxX/2 - (16*WIDTH) ;
    eVt.y= ESTR_Y;
    eVt.lmax=16;
    eVt.val_no_max	= 2 ;
    eVt.mode	= GV_VECTOR	;
    eVt.format = format2_float2 ;  //g g f
    eVt.ESTRF=Vt_n;
    eVt.extend = 0;

    eVt1.x=maxX/2 - (16*WIDTH) ;
    eVt1.y= ESTR_Y;
    eVt1.lmax=16;
    eVt1.val_no_max	= 1 ;
    eVt1.mode	= GV_DIST	;
    eVt1.format = format_floatd ;  //g g f
    eVt1.ESTRF=Vt_n1;
    eVt1.extend = 0;

    eVn.x=maxX/2 + 5 ;
    eVn.y= ESTR_Y;
    eVn.val_no_max = 1;
    eVn.mode = GV_DIST ;
    eVn.format = format_floatd;
    eVn.lmax=12;
    eVn.ESTRF=Vn_n;
    eVn.extend = 0;
}

int Inverse_Vector_factory(AVECTOR *V)
{   BOOL inverse;
    inverse=V->flags & 1;
    V->flags ^= 1;
    V->flags |= !inverse;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
    return 1;
}

int Inverse_Vector(void)
{
    return Inverse_Vector_factory(&VectorG);
}

int Inverse_VectorC(void)
{
    return Inverse_Vector_factory(get_vector_c());
}

int Inverse_VectorP(void)
{   AVECTOR *V;
    V=(AVECTOR*)get_vector_s();
    return Inverse_Vector_factory(V);
}

int Reverse_Vector_factory(AVECTOR *V)
{   float mag1;
    mag1=V->magnitude1;
    V->magnitude1=V->magnitude2;
    V->magnitude2=mag1;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
    return 1;
}

int Reverse_Vector(void)
{
    return Reverse_Vector_factory(&VectorG);
}

int Reverse_VectorC(void)
{
    return Reverse_Vector_factory(get_vector_c());
}

int Reverse_VectorP(void)
{   AVECTOR *V;
    V=(AVECTOR*)get_vector_s();
    return Reverse_Vector_factory(V);
}

static void redcr(char typ)
/*-------------------------*/
{ static int ( *SW[5])();
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int KR0,np,npv;
  PLINIA PL;
  if (typ==0)
   {
     KR0=Komunikat_R0;

      SW[4] = SERV [83];  SERV [83] = last_vector_delete ;

      if ((VectorG.style>9) && (VectorG.style<15)) Komunikat_R0=158;
      else Komunikat_R0=156;

     komunikat0(Komunikat_R0);
     poczL=1;

     menupini (&mVector, _VECTOR_, _VECTOR_C_, 722) ;

     np=-1;

     SW[3]=SERV[71];
     SW[2] = SERV[79];

     switch (VectorG.style)
     {
         case 0:  //members
         case 1:
         case 2:
         case 3:
             np=dodajstr(&eL);
             npv=dodajstr(&eVe);
             sprintf (eVe.st, "%#6d", VectorG.property_no);
             Out_Edited_Draw_Param ((ESTR *)&eVe, TRUE) ;
             break;
         case 4:  //force
             npv=dodajstr(&eVf);
             parametry_lini(&LiniaG, &PL);
             sprintf (eVf.st, format_float2, VectorG.magnitude1, PL.kat);
             Out_Edited_Draw_Param ((ESTR *)&eVf, TRUE) ;
             break;
         case 5:  //moment
         case 6:
             npv=-1;
             set_arc_stage=0;
             break;
         case 7:  //displacement
             npv=dodajstr(&eVd);
             parametry_lini(&LiniaG, &PL);
             sprintf (eVd.st, format_float2, VectorG.magnitude1, PL.kat);
             Out_Edited_Draw_Param ((ESTR *)&eVd, TRUE) ;
             break;
         case 8:  //rotation
         case 9:
             npv=-1;
             set_arc_stage=0;
             break;
         case 10:  //trapezoid load
         case 11:
         case 12:
         case 13:
         case 14:  //traphezoid
             SERV[71]=Inverse_Vector;
             SERV[79]=Reverse_Vector;
             np=dodajstr(&eL);
             npv=dodajstr(&eVt);
             sprintf (eVt.st, format_float2, VectorG.magnitude1,VectorG.magnitude2);
             Out_Edited_Draw_Param ((ESTR *)&eVt, TRUE) ;
             break;
         case 17:  //trapezoid load plate
             SERV[71]=Inverse_Vector;
             SERV[79]=Reverse_Vector;
             np=dodajstr(&eL);
             npv=dodajstr(&eVt1);
             sprintf (eVt1.st, format_floatd, VectorG.magnitude1);
             Out_Edited_Draw_Param ((ESTR *)&eVt1, TRUE) ;
             break;
         case 15:  //thermal
             VectorG.r=thermal_axis_size;
             np=dodajstr(&eL);
             npv=dodajstr(&eVt);
             sprintf (eVt.st, format_float2, VectorG.magnitude1,VectorG.magnitude2);
             Out_Edited_Draw_Param ((ESTR *)&eVt, TRUE) ;
             break;
         case 16:  //node size
             VectorG.magnitude1 = (float)node_size;
             VectorG.r=jednostkiOb(node_size);
             npv=dodajstr(&eVn);
             sprintf (eVn.st, format_float, VectorG.magnitude1);
             Out_Edited_Draw_Param ((ESTR *)&eVn, TRUE) ;
             break;
         default:
             break;
     }

     CUR_OFF (X, Y) ;
     CUR_oFF=CUR_OFF;  CUR_OFF=cur_off;
     CUR_oN=CUR_ON;   CUR_ON=cur_on;
     CUR_ON (X, Y) ;
   }
  else if (typ==2)
   {
    SERV [83] = SW [4] ;
    SERV[71] = SW[3];
    SERV[79] = SW[2];
   }
  else
   {
     CUR_OFF (X, Y) ;
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     CUR_ON (X, Y) ;
     SERV [83] = SW [4] ;
     SERV[71] = SW[3];
     SERV[79] = SW[2];
     Komunikat_R0=KR0;
     poczL=0;

     if (npv>0) usunstr(npv);
     if (np>0) usunstr(np);
     menupini (&mVector, _VECTOR_, _VECTOR_C_, 722) ;

     komunikat0(Komunikat_R0);
   }
}

static void poczatekV (double X0, double Y0)
/*-----------------------------------------*/
{ double LiniaG_x2, LiniaG_y2;
  EVENT *ev ;
  int ret;
  PLINIA PL;
  int npv=-1;
  int orto_;

  df__xbeg = X0 ;
  df__ybeg = Y0 ;
  LiniaG.x1 = X0 ;
  LiniaG.y1 = Y0 ;
  LiniaG.x2 = X ;
  LiniaG.y2 = Y ;
  Orto_Dir = I_Orto_NoDir ;

  orto_=orto;

    if ((last_vector_style!=VectorG.style) || (VectorG.style<10))
    {
        VectorG.magnitude1 = 0.0;
        VectorG.magnitude2 = 0.0;
    }

  redcr (0) ;

  if ((last_vector_style!=VectorG.style) || (VectorG.style<10))
  {
      VectorG.magnitude1=0.0;
      VectorG.magnitude2=0.0;
  }

  while (1)
  {
    strwyj = 0 ;
    if (( (VectorG.style==8) || (VectorG.style==9)) && (set_arc_stage==0))
    {
        VectorG.angle1=VectorG.angle2=0;
    }
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    if (ev->Number == -83)  continue;
    else if (ev->Number == -84)
    {
        if (npv>0) usunstr(npv);
        return;
    }
    if (strwyj == 1)
    {
        if ((VectorG.style==5) || (VectorG.style==6) || (VectorG.style==8) || (VectorG.style==9))
        {
            if (set_arc_stage==0)
            {
                set_arc_stage = 1;
                orto_=orto;
                orto=0; //TEMPORARY

                if ((VectorG.style==5) || (VectorG.style==6))
                {
                    npv = dodajstr(&eVr);
                    sprintf(eVm.st, format_float, VectorG.magnitude1);
                    Out_Edited_Draw_Param((ESTR *) &eVm, TRUE);
                }
                else
                {
                    npv = dodajstr(&eVm);
                    sprintf(eVr.st, format_floatd, VectorG.magnitude1);
                    Out_Edited_Draw_Param((ESTR *) &eVr, TRUE);
                }

                continue;
            }
            else
            {
                add_vector(TRUE);

                redcr(1);

                CUR_OFF(X, Y);
                CUR_ON(X, Y);

                if (npv>0) usunstr(npv);
                orto=orto_;
                return;
            }
            strwyj = 0 ;
        }
        else
        {

            add_vector(TRUE);

            CUR_OFF(X, Y);
            CUR_ON(X, Y);
            if ((VectorG.style > 3) && (VectorG.style < 10)) {
                redcr(1);
                return;
            }
            continue;
        }
    }
    switch (ev->What)
    {
      case evKeyDown :
	 if(ev->Number == 0)
	 {
	   redcr (1) ;
       last_vector_style=VectorG.style;
	   return ;
	 }
	 if (ev->Number == ENTER)
	 {
         last_vector_style=VectorG.style;
         if ((VectorG.style==5) || (VectorG.style==6) || (VectorG.style==8) || (VectorG.style==9))
         {
            if (set_arc_stage==0)
            {
                set_arc_stage = 1;
                orto_=orto;
                orto=0; //TEMPORARY

                if ((VectorG.style==5) || (VectorG.style==6))
                {
                    npv = dodajstr(&eVm);
                    sprintf(eVm.st, format_float, VectorG.magnitude1);
                    Out_Edited_Draw_Param((ESTR *) &eVm, TRUE);
                }
                else
                {
                    npv = dodajstr(&eVr);
                    sprintf(eVr.st, format_floatd, VectorG.magnitude1);
                    Out_Edited_Draw_Param((ESTR *) &eVr, TRUE);
                }

                continue;
            }
            else
            {
                add_vector(FALSE);

                redcr(1);

                CUR_OFF(X, Y);
                CUR_ON(X, Y);
                if (npv>0) usunstr(npv);
                orto=orto_;
                return;

            }
         }
         else
         {
             add_vector(FALSE);

             CUR_OFF(X, Y);
             CUR_ON(X, Y);
             if (((VectorG.style > 3) && (VectorG.style < 10)) || (VectorG.style==16)){
                 redcr(1);
                 return;
             }
             if ((VectorG.style >= 10) && (VectorG.magnitude2!=VectorG.magnitude1))
             {
                 float mag1=VectorG.magnitude1;
                 VectorG.magnitude1=VectorG.magnitude2;
                 VectorG.magnitude2=mag1;
             }
         }
         break;
	 }
      case evCommandP :
        ret = vector_command_proc (ev->Number);
  	    if (ret != PL_MODE_CONTINUE)
	     {
	       redcr (1) ;
           if (npv>0) usunstr(npv);
           orto=orto_;
	       return ;
	     }
	 break ;
      default :
	 break ;
    }
  }
}

static void redcr0(char typ)
/*-------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(), akt ;
  if (typ==0)
   {
       komunikat0 (155) ;
       menupini (&mVector, _VECTOR_, _VECTOR_C_, 722);
       LiniaG.blok = NoElemBlok ;

	 CUR_OFF(X, Y);
     akt=sel.akt; sel.akt=ASel;
	 cursor_off = cursel_off;
	 cursor_on=cursel_on;
     eL.x=maxX-PL266;
     eL.y = ESTR_Y; // 0;
     eL.val_no_max = 2;
     eL.lmax=r22;
     eL.ESTRF=L_p;
	 eL.extend = 0;
     eL.mode = GV_VECTOR ;
     eL.format = "%#13.9lg\0%#13.9lg;%#7.2lf" ;
     CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;   CUR_ON=cursel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     CUR_ON(X,Y);
   }
  else
   {
     CUR_OFF(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
     
     menupini (NULL, "", ' ',0) ;

     sel.akt=akt;
	 CUR_ON(X, Y);

     LiniaG.blok = NoElemBlok ;
     komunikat0 (0) ;
   }
}

void Vector (void)
/*------------*/
{
    EVENT *ev;
    double X0, Y0;
    long dane_size0 ;
    char sk [MaxTextLen], *str ;
    int ret;

beginning:

    ini_vector_estr();

    dane_size0 = dane_size ;
    redcr0 (0) ;

    while (1)
    {
        if (VectorG.style==V_SLAB_PLATE)  //plate
        {
            redcr0 (1) ;
            LiniaG.obiektt2=O2FREE_EDGE;
            LiniaG.obiektt3=O3REGULAR_EDGE;
            PLATE=1;
            HOLE=0;
            WALL=0;
            ZONE=0;
            ret=Pline_Slab();
            if (ret==2) goto beginning;
            return;
        }
        else if (VectorG.style==V_SLAB_SPACE)  //space
        {
            redcr0 (1) ;
            LiniaG.obiektt2=O2FREE_EDGE;
            LiniaG.obiektt3=O3REGULAR_EDGE;
            LukG.obiektt2=O2FREE_EDGE;
            LukG.obiektt3=O3REGULAR_EDGE;
            PLATE=0;
            HOLE=1;
            WALL=0;
            ZONE=0;
            ret=Pline_Slab();
            if (ret==2) goto beginning;
            return;
        }
        else if (VectorG.style==V_SLAB_WALL)  //Wall
        {
            redcr0 (1) ;
            LiniaG.obiektt2=O2FREE_EDGE;
            LiniaG.obiektt3=O3REGULAR_EDGE;
            LukG.obiektt2=O2FREE_EDGE;
            LukG.obiektt3=O3REGULAR_EDGE;
            PLATE=0;
            HOLE=0;
            WALL=1;
            ZONE=0;
            ret=Pline_Slab();
            if (ret==2) goto beginning;
            return;
        }
        else if (VectorG.style==V_SLAB_ZONE)  //zone
        {
            redcr0 (1) ;
            LiniaG.obiektt2=O2FREE_EDGE;
            LiniaG.obiektt3=O3REGULAR_EDGE;
            LukG.obiektt2=O2FREE_EDGE;
            LukG.obiektt3=O3REGULAR_EDGE;
            PLATE=0;
            HOLE=0;
            WALL=0;
            ZONE=1;
            ret=Pline_Slab();
            if (ret==2) goto beginning;
            return;
        }
        else
        {
            view_line_type(&LiniaG);
            ev = Get_Event_Point(NULL, &X0, &Y0);
            Last_GX = X0;
            Last_GY = Y0;
            switch (ev->What) {
                case evKeyDown  :
                    if (ev->Number == 0) {
                        redcr0(1);
                        return;
                    }
                    if (ev->Number == ENTER) {
                        poczatekV(X0, Y0);
                    }
                    break;
                case evCommandP :
                    ret = vector_command_proc(ev->Number);
                    if ((ret != PL_MODE_CONTINUE) && (ret != PL_MODE_END)) {
                        redcr(1);
                        return;
                    }
                    break;
                default :
                    break;
            }
        }
    }
}

#undef __O_VECTOR__