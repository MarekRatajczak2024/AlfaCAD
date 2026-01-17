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

#define __O_SOLID__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"

#include "menu.h"

extern double Grid_to_Rad (double angle) ;
extern int Set_WidthL_to_Type (int type, int width) ;
extern int Get_Current_Color (void) ;
extern void Normalize_Solid (WIELOKAT *) ;
extern void setwritemode( int mode );
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void view_line_type(LINIA *L);
extern void view_pattern_type(char* pattern_name);

extern int Open_SolidPatterns(void);
extern void Wez_Solid_Pattern(void);
extern void Set_Solid_Pattern(void);
extern double get_elliptical_tangent_angle(double main_axis_angle, double xr, double yr, double end_angle);
extern double cartesian_angle_to_isometric_angle(double cart_angle_rad);
extern int isometric_polar_to_cartesian(double x1, double y1, double length, double angle_deg, double *x2, double *y2);

static void (*cursor_on)(double ,double)=out_cur_on;
static void (*cursor_off)(double ,double)=out_cur_off;
static void set_corner_nomax (int) ;
static void set_solidtranslucency(int poz);
static void set_filltype(int);
static void set_solidpattern(void);
static void set_grab_solidpattern(void);
static void set_change_solidpattern(void);
static void set_solidpatternscale(int poz);
static void set_solidpatternangle(int poz);
static void set_solidpatterndx(int poz);
static void set_solidpatterndy(int poz);
static double solid_z[5];

static BOOL S3D=FALSE;
static BOOL next_solid=FALSE;
static double last_z3d=0;

static LINIA line_temp = Ldef, line_g = Ldef ;
static WIELOKAT solid = Stdef ; 		/*Stdef strzalka*/
static int i__corner_no = 0, i__solid_corner_nomax = 4 ;

static void redraw_solid (void);

int solid_in_progress_vertices(void);
char *solid_in_progress_ad(void);

TMENU mSolid={3,0,0,28, 56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmSolid,NULL,NULL} ;

float solidpatternscales[7] = {0.125,0.25,0.5,1.0,2.0,4.0,8.0};
int solidpatternangles[4] = { 0, 90, 180, 270 };
char solidpatterndxy[10][5] = {"0","1/32","1/16","1/8","1/4","3/8","1/2","5/8","3/4","7/8"};
/*--------------------------------------------------------------------------------------*/


/* Helper: isometric projection of a single point (float version) */
static void project_point_isometric(float cx, float cy, float *ix, float *iy, enum PlaneType plane) {
    double dx = (double)cx;
    double dy = (double)cy;
    double px, py;

    switch (plane) {
        case XY_PLANE:
            px = (sqrt(3.0)/2.0) * dx - (sqrt(3.0)/2.0) * dy;
            py = 0.5 * dx + 0.5 * dy;
            break;
        case XZ_PLANE:
            px = (sqrt(3.0)/2.0) * dx;
            py = 0.5 * dx + dy;  // Z contributes full height
            break;
        case YZ_PLANE:
            px = -(sqrt(3.0)/2.0) * dy;
            py = 0.5 * dy + dy;  // Z contributes full height
            break;
        default:
            px = dx;
            py = dy;
            break;
    }

    *ix = (float)px;
    *iy = (float)py;
}

/* Main function: isometrize a polygon (triangle or quadrilateral) in place */
void izometrize_solid(enum PlaneType plane, WIELOKAT *St) {
    if (!St || St->lp < 6) return;  // at least 3 vertices (6 floats)

    /* Save the fixed insertion point (vertex 1: indices 2 and 3) */
    float fixed_x = St->xy[2];
    float fixed_y = St->xy[3];

    /* Compute projected coordinates for ALL vertices */
    float proj_x[4], proj_y[4];  // max 4 vertices
    int num_vertices = St->lp / 2;

    for (int i = 0; i < num_vertices; i++) {
        project_point_isometric(St->xy[2*i], St->xy[2*i + 1], &proj_x[i], &proj_y[i], plane);
    }

    /* Compute offset so that the fixed point stays unchanged */
    float offset_x = fixed_x - proj_x[1];  // vertex 1 projected
    float offset_y = fixed_y - proj_y[1];

    /* Apply projection + offset to all vertices */
    for (int i = 0; i < num_vertices; i++) {
        St->xy[2*i]     = proj_x[i] + offset_x;
        St->xy[2*i + 1] = proj_y[i] + offset_y;
    }
}

void make_dim_elliptical_arc_isometric_solid(ELLIPTICALARC *ea, double ea_start_x, double ea_start_y, double ea_end_x, double ea_end_y, double Kps, double katS, int n, WIELOKAT *w)
{
    double ea_end_xx = (n>0) ? ea_end_x : ea_start_x;
    double ea_end_yy = (n>0) ? ea_end_y : ea_start_y;
    double angle_rad = get_elliptical_tangent_angle(ea->angle, ea->rx, ea->ry, (n>0) ?ea->kat2 : ea->kat1);

    // Reverse direction for arrowhead base behind tip (your working flip)
    if (n>0) angle_rad = fmod(angle_rad - M_PI, 2.0 * M_PI);
    if (angle_rad < 0.0) angle_rad += 2.0 * M_PI;

    double iso_angle = cartesian_angle_to_isometric_angle(angle_rad);

    double katS_rad = katS * M_PI / 180.0;
    double Kp2s = Kps / cos(M_PI*katS/180) ;

    double iso_angle1 = fmod(iso_angle + katS_rad, 2.0 * M_PI);
    double iso_angle2 = fmod(iso_angle - katS_rad, 2.0 * M_PI);

    double iso_angle1_deg = iso_angle1 * 180.0 / M_PI;
    double iso_angle2_deg = iso_angle2 * 180.0 / M_PI;

    double x_tip1, y_tip1, x_tip2, y_tip2;
    isometric_polar_to_cartesian(ea_end_xx, ea_end_yy, Kp2s, iso_angle1_deg, &x_tip1, &y_tip1);
    isometric_polar_to_cartesian(ea_end_xx, ea_end_yy, Kp2s, iso_angle2_deg, &x_tip2, &y_tip2);

    double dxi = ea_end_xx - (x_tip1 + x_tip2) / 2.0;
    double dyi = ea_end_yy - (y_tip1 + y_tip2) / 2.0;

    /*
    w->xy[0] = (float)(x_tip1 + dxi);
    w->xy[1] = (float)(y_tip1 + dyi);
    w->xy[2] = (float)(ea_end_xx + dxi);
    w->xy[3] = (float)(ea_end_yy + dyi);
    w->xy[4] = (float)(x_tip2 + dxi);
    w->xy[5] = (float)(y_tip2 + dyi);
     */

    w->xy[0] = (float)x_tip1;
    w->xy[1] = (float)y_tip1;
    w->xy[2] = (float)ea_end_xx;
    w->xy[3] = (float)ea_end_yy;
    w->xy[4] = (float)x_tip2;
    w->xy[5] = (float)y_tip2;

}

static void out_temp_line (void)
/*-----------------------------*/
{
  LINIA L;
  setwritemode (COPY_PUT) ;
  SetColorAC (Get_Current_Color ());
  setlinestyle1 (DOTTED_LINE, 0, NORM_WIDTH) ;
  okno_r () ;
  if(orto)
  {
    L.x1=line_g.x1;
    L.y1=line_g.y1;
    L.x2=line_g.x2;
    L.y2=line_g.y2;
    orto_l(&L, &Orto_Dir);
    line_temp.x2 = L.x2 ;
    line_temp.y2 = L.y2 ;
  }
  lineC (pikseleX0(line_temp.x1), pikseleY0(line_temp.y1),
	pikseleX0(line_temp.x2), pikseleY0(line_temp.y2)) ;
  okno_all () ;
}

static void  cur_off (double x, double y)
/*--------------------------------------*/
{
    flip_screen();
}

static void  cur_on (double x, double y)
/*--------------------------------------*/
{
    line_g.x2 = x ;
    line_g.y2 = y ;
    line_g.kolor = Get_Current_Color () ;
    outlineor (&line_g, COPY_PUT, 1) ;
    line_temp.x2 = line_g.x2 ;
    line_temp.y2 = line_g.y2 ;
    redraw_solid();
    out_temp_line () ;
    cursor_on(x, y);
}


#define r18 18
#define r22 16 //22
static int strwyj;
static double Lx2,Ly2 ;

static int L_p(BOOL b_graph_value)
/*----------------*/
{
  PLINIA PL ;
  double l, k ;
  double angle_l;

  b_graph_value = b_graph_value ;
  if (eL.val_no < 1) return 0;
  l = eL.values [0] ;
  if ( orto != 0 || eL.val_no == 1)
  {
    parametry_linior (&line_g, &PL) ;
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
  Lx2 = line_g.x1 + l * cos (k) ;
  Ly2 = line_g.y1 + l * sin (k) ;
  strwyj = 1;
  return 1;
}


static void redraw_solid1(BOOL b_redraw)
/*------------------------------------*/
{
	LINIA lg;
	int i;

	i = 0;
	if (i__corner_no > 1)
	{

		memmove(&lg, &line_g, sizeof(LINIA));
		lg.x1 = solid.xy[i++];
		lg.y1 = solid.xy[i++];
		lg.x2 = solid.xy[i++];
		lg.y2 = solid.xy[i++];
		if (next_solid == FALSE)
		{
		    rysuj_obiekt((char*)&lg, COPY_PUT, b_redraw);
	    }

		if (i__corner_no > 2)
		{
			lg.x1 = lg.x2;
			lg.y1 = lg.y2;
			lg.x2 = solid.xy[i++];
			lg.y2 = solid.xy[i++];
			rysuj_obiekt((char*)&lg, COPY_PUT, b_redraw);
		}
		
	}
}


static void redraw_solid (void)
/*-----------------------------*/
{
  redraw_solid1 (TRUE) ;
}


int solidfill_typ[8]={64,0,32,64,96,128,65,65};

static void next_point (double x0, double y0)
/*-----------------------------------------*/
{ char sk [MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  if(strwyj)
  {
    line_g.x2 = Double_to_Float (Lx2) ;
    line_g.y2 = Double_to_Float (Ly2) ;
  }
  else
  {
    line_g.x2 = x0 ;
    line_g.y2 = y0 ;
  }
  if(orto)
  {
    orto_l(&line_g, &Orto_Dir);
    Orto_Dir = I_Orto_NoDir ;
  }
  line_g.typ=solidfill_typ[SolidFillTyp];
  outline (&line_g, COPY_PUT, 0) ;
  if(line_g.x1!=line_g.x2 || line_g.y1!=line_g.y2)
  {
    i__corner_no++ ;
    solid.xy [solid.lp++] = line_g.x2 ;
    solid.xy [solid.lp++] = line_g.y2 ;
  }
  line_g.x1 = line_g.x2 ;
  line_g.y1 = line_g.y2 ;
  if (S3D==TRUE)
   {
     //pytanie o wspolrzedne LiniaG3D.z1; LiniaG3D.z2
     sprintf (sk, "%f", last_z3d) ;
     if (!get_string (sk, "", MaxTextLen, 0, 144)) return ;
     if (FALSE == calculator (sk, &retval_no, buf_ret) || retval_no < 1)
      {
       return ;
      }
     last_z3d = buf_ret [0] ;
     solid_z[i__corner_no]=milimetryob(last_z3d);
   }

  line_g.x2 = X ;
  line_g.y2 = Y ;
}

static BOOL add_solid (void)
//tymczasowo wprowadzono dla S3D wartosci zerowe z1, z2, z3, z4
/*------------------------------------------------------------*/
{
  int k ;
  WIELOKAT solid_temp ;
  char* scale_ptr;
  char* dx_ptr;
  char* angle_ptr;
  char* dy_ptr;
  char* name_ptr;
  char* translucency_ptr;

  next_solid=TRUE;
  solid.empty_typ=SolidFillTyp;
  solid.kolor = Get_Current_Color () ;
  solid.warstwa = Current_Layer ;
  if (S3D==FALSE)
   {
     solid.obiekt=Owwielokat;
     solid.pattern = SolidPattern;
     if (solid.pattern == 1)
     {
         solid.translucent = 0;
         scale_ptr = solid.xy;
         scale_ptr += (solid.lp * sizeof(float));
         dx_ptr = scale_ptr;
         dx_ptr += sizeof(short int);
         angle_ptr = dx_ptr; 
         angle_ptr += sizeof(short int);
         dy_ptr = angle_ptr;
         dy_ptr += sizeof(short int);
         name_ptr = dy_ptr;
         name_ptr += sizeof(short int);
         memmove(scale_ptr, &SolidPatternScale, sizeof(short int));
         memmove(dx_ptr, &SolidPatternDx, sizeof(short int));
         memmove(angle_ptr, &SolidPatternAngle, sizeof(short int));
         memmove(dy_ptr, &SolidPatternDy, sizeof(short int));
         memmove(name_ptr, &SolidPatternName, strlen(SolidPatternName)+1);

         solid.n = 8 + solid.lp * sizeof(float) + 4 * sizeof(short int) + (int)strlen(SolidPatternName) + 1;
     }
     else
     {
         solid.translucent = SolidTranslucent;
         if (solid.translucent==0)
              solid.n = 8 + solid.lp * sizeof(float);
         else
         {
             translucency_ptr = solid.xy;
             translucency_ptr += (solid.lp * sizeof(float));
             memmove(translucency_ptr, &SolidTranslucency, sizeof(unsigned char));

             solid.n = 8 + solid.lp * sizeof(float) + sizeof(unsigned char);
         }
     }
   }
  memcpy (&solid_temp, &solid, sizeof (solid)) ;

  if (S3D==FALSE) Normalize_Solid (&solid_temp) ;

  if (dodaj_obiekt (NULL, &solid_temp) == NULL) return FALSE;
  rysuj_obiekt ((char *)&solid, COPY_PUT, 1) ;
  flip_screen();
  i__corner_no = 2 ;
  k = solid.lp - 4 ;
  solid.xy [0] = solid.xy [k++] ;
  solid.xy [1] = solid.xy [k++] ;
  solid_z[0]=solid_z[2];
  solid.xy [2] = solid.xy [k++] ;
  solid.xy [3] = solid.xy [k++] ;
  solid_z[1]=solid_z[3];
  solid.lp = 4 ;
  line_temp.x1 = solid.xy [0] ;
  line_temp.y1 = solid.xy [1] ;
  return TRUE ;
}

static void  nooop(void)
{}

static void redcr(char typ, double x0, double y0)
/*----------------------------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int KR0,np;
  static void (*REDdod)(void)=nooop;

  if (typ==0)
  {
	 CUR_OFF(X, Y);
     i__corner_no = 1 ;
     solid.xy [0] = x0 ;
     solid.xy [1] = y0 ;
     solid.lp = 2 ;
     KR0=Komunikat_R0;
     Komunikat_R0 = 53 ;
     komunikat0(Komunikat_R0);
     REDdod = REDDOD ; REDDOD = redraw_solid ;
     poczL = 1 ;
     np = dodajstr (&eL) ;
     CUR_oFF = CUR_OFF ;  CUR_OFF = cur_off ;
     CUR_oN = CUR_ON ;   CUR_ON = cur_on ;
     CUR_ON (X, Y) ;
  }
  else
  {
    redraw_solid1 (FALSE) ;
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;
    Komunikat_R0=KR0;
    poczL=0;
    REDDOD=REDdod;
    komunikat0(Komunikat_R0);
    usunstr(np);
  }
}

static void solid_proc (double x0, double y0)
/*-----------------------------------*/
{ char sk [MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  EVENT *ev;
  int i;
  LINIA line_g_;

  line_g.x1 = line_temp.x1= x0 ;
  line_g.y1 = line_temp.y1= y0 ;
  line_g.x2 = line_temp.x2= X ;
  line_g.y2 = line_temp.y2 = Y ;

  Orto_Dir = I_Orto_NoDir;
  redcr (0, x0, y0);
  if (S3D==TRUE)
   {
     //pytanie o wspolrzedne LiniaG3D.z1; LiniaG3D.z2
     sprintf (sk, "%f", last_z3d) ;
     if (!get_string (sk, "", MaxTextLen, 0, 144)) return ;
     if (FALSE == calculator (sk, &retval_no, buf_ret) || retval_no < 1)
      {
       return ;
      }
     last_z3d = buf_ret [0] ;
     solid_z[i__corner_no]=milimetryob(last_z3d);
   }
  while(1)
  {
     strwyj = 0 ;
     view_line_type(&LiniaG);
     ev = Get_Event_Point (NULL, &x0, &y0) ;
     if (ev->What == evKeyDown &&  ev->Number == 0)
     {
	CUR_OFF (X, Y) ;
	redcr (1, 0, 0) ;
	CUR_ON (X, Y) ;
	return ;
     }
     if ((ev->What == evKeyDown &&  ev->Number== ENTER)  || strwyj)
     {
	CUR_OFF (X, Y) ;
	next_point (x0, y0) ;
	if (i__corner_no >= i__solid_corner_nomax  )
	{
        redraw_solid1(FALSE);

	  add_solid () ;
	}
	CUR_ON (X, Y) ;
     }
     if (ev->What == evCommandP)
     {
       if (ev->Number == 3  || ev->Number == 4) set_corner_nomax (ev->Number) ;
       else if (ev->Number == 12) set_solidpattern();
       else if (ev->Number == 17) set_grab_solidpattern();
       else if (ev->Number == 18) set_change_solidpattern();
       else if (ev->Number > 49) set_solidtranslucency(ev->Number);
       else if (ev->Number > 39) set_solidpatterndy(ev->Number);
        else if (ev->Number > 29) set_solidpatterndx(ev->Number);
         else if (ev->Number > 25) set_solidpatternangle(ev->Number);
          else if (ev->Number > 18) set_solidpatternscale(ev->Number);
           else if (ev->Number > 4) set_filltype(ev->Number);
       if (ev->Number == 1  && solid.lp == 6)
       {
	  CUR_OFF (X, Y) ;
	  add_solid () ;
	  CUR_ON (X, Y) ;
       }
     }
  }
}

void set_solid_pattern_name()
{
    menu_par_new((*mSolidPattern.pola)[0].txt, SolidPatternName);
}

static void redcr0(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[2])(), akt;
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  unsigned short cod = L' ';
  static int iconno=0;
  static char sk[16];
  float d_trans = 12.75;
  int transluc;

  if (typ==0)
   {
	 CUR_OFF(X, Y);

     i__corner_no=0;

     komunikat (0);
     komunikat0 (53);
     akt=sel.akt; sel.akt=ASel;
     cursor_off= cursel_off;
	 cursor_on=cursel_on;
     eL.x=maxX-PL266;
     eL.y= ESTR_Y;
     eL.val_no_max = 2;
     eL.mode = GV_VECTOR ;
     eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf" ;
     eL.lmax=r22;
     eL.ESTRF=L_p;
	 eL.extend = 0;
     getmenupini(&menu,st,&cod, &iconno);
     
     menu_par_new((*mSolidPattern.pola)[0].txt, SolidPatternName);
     sprintf(sk, "%5.3f", solidpatternscales[SolidPatternScale]);
     menu_par_new((*mSolidPattern.pola)[1].txt, sk);
     sprintf(sk, "%d", solidpatternangles[SolidPatternAngle]);
     menu_par_new((*mSolidPattern.pola)[2].txt, sk);

     transluc = (int)((float)SolidTranslucency / d_trans +0.5);
     sprintf(sk, "%d%%", transluc * 5);
     menu_par_new((*mFillTyp.pola)[0].txt, sk);

     if (S3D==FALSE) menupini (&mSolid, _SOLID_, _SOLID_C_, 21) ;
       else menupini (&mSolid, _SOLID_3D_, _SOLID_C_, 21) ;

     CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;   CUR_ON=cursel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     CUR_ON(X,Y);
   }
  else
   {
     CUR_OFF(X,Y);

     i__corner_no=0;

     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
    
     sel.akt = akt;

	 CUR_ON(X, Y);
     menupini (menu, st, cod, iconno) ;
     komunikat (0) ;
     komunikat0 (0) ;
   }
}


static void set_corner_nomax (int poz)
/*----------------------------------*/
{
  char sk [10] ;
  int ret;

  if (poz == 3)
  {
    if (i__solid_corner_nomax   == 3) return ;
      i__solid_corner_nomax   = 3 ;
  }
  else
  {
    if (i__solid_corner_nomax   == 4) return ;
      i__solid_corner_nomax   = 4 ;
  }
  ret = snprintf(sk, 10, "%d",i__solid_corner_nomax);
  menu_par_new ((*mSolid.pola)[0].txt, sk) ;
}

int solid_in_progress_vertices(void)
{
    return i__corner_no;
}

char *solid_in_progress_ad(void)
{
    return &solid;
}

void Solid_ (void)
/*---------------*/
{
  EVENT *ev;
  double X0, Y0;


  redcr0(0);
  line_g.typ=solidfill_typ[SolidFillTyp];
  while(1)
  {
    next_solid=FALSE;
    if ((SolidFillTyp==0) && (SolidPattern==1)) view_pattern_type(SolidPatternName);
    else view_line_type(&line_g);
    ev=Get_Event_Point (NULL, &X0, &Y0);
    switch(ev->What)
    {
      case evKeyDown :
	    if(ev->Number==0)
	    {
	      redcr0(1);
	      return;
	    }
	    if(ev->Number== ENTER)
	    {
	      solid_proc (X0, Y0) ;
	    }
	    break;
      case evCommandP :
      if (ev->Number == 3  || ev->Number == 4) set_corner_nomax (ev->Number) ;
        else if (ev->Number == 12) set_solidpattern();
        else if (ev->Number == 17) set_grab_solidpattern();
        else if (ev->Number == 18) set_change_solidpattern();
        else if (ev->Number > 49) set_solidtranslucency(ev->Number);
         else if (ev->Number > 39) set_solidpatterndy(ev->Number);
          else if (ev->Number > 29) set_solidpatterndx(ev->Number);
           else if (ev->Number > 25) set_solidpatternangle(ev->Number);
            else if (ev->Number > 18) set_solidpatternscale(ev->Number);
             else if (ev->Number > 4) set_filltype(ev->Number);
	    break ;
      default :
	 break;
    }
  }
}

void* obiekt_w_wybrany(void)
/*----------------------------*/
{
    unsigned typ;
    typ = Bwwielokat;
    return select_w(&typ, NULL);
}


void set_solid_pattern_trans(char* sk)
{
    menu_par_new((*mFillTyp.pola)[0].txt, sk);
}


static void set_grab_solidpattern(void)
{
    BOOL ret;

    Komunikat_R00 = Komunikat_R0;
    komunikat0(0);
    Wez_Solid_Pattern();
    Komunikat_R0 = Komunikat_R00;
    komunikat0(Komunikat_R0);
    
    return;
}

static void set_change_solidpattern(void)
{
    BOOL ret;
    Komunikat_R00 = Komunikat_R0;
    komunikat0(0);
    Set_Solid_Pattern();
    Komunikat_R0 = Komunikat_R00;
    komunikat0(Komunikat_R0);
    return;
}

static void set_solidpattern(void)
{
    int ret;
    Komunikat_R00 = Komunikat_R0;
    ret = Open_SolidPatterns();
    if (SolidPattern == 1)
    {
        menu_par_new((*mSolidPattern.pola)[0].txt, SolidPatternName);
        SolidG.empty_typ = 0;
        SolidG.pattern = 1;
        SolidFillTyp = 0;
    }
    else
    {
        ;
    }
    Komunikat_R0 = Komunikat_R00;
    komunikat0(Komunikat_R0);
}

static void set_solidpatternscale(int poz)
{
    char sk[16];
    
    SolidPatternScale = poz - 19;

    sprintf(sk, "%5.3f", solidpatternscales[SolidPatternScale]);

    menu_par_new((*mSolidPattern.pola)[1].txt, sk);
}

static void set_solidpatternangle(int poz)
{
    char sk[16];

    SolidPatternAngle = poz - 26;

    sprintf(sk, "%d", solidpatternangles[SolidPatternAngle]);

    menu_par_new((*mSolidPattern.pola)[2].txt, sk);
}

static void set_solidpatterndx(int poz)
{
    char sk[16];

    SolidPatternDx = poz - 30;

    sprintf(sk, "%s", solidpatterndxy[SolidPatternDx]);

    menu_par_new((*mSolidPattern.pola)[3].txt, sk);
}

static void set_solidpatterndy(int poz)
{
    char sk[16];

    SolidPatternDy = poz - 40;

    sprintf(sk, "%s", solidpatterndxy[SolidPatternDy]);

    menu_par_new((*mSolidPattern.pola)[4].txt, sk);
}

static void set_solidtranslucency(int poz0)
{
    char sk[10];
    int poz = poz0;

    float d_trans = 12.75;
    int transluc;

    SolidPattern = 0;
    SolidFillTyp = 0;

    if (poz0 == 50) SolidTranslucent = 0;
    else SolidTranslucent = 1;

    SolidTranslucency = 255 - (int)((poz0 - 50) * d_trans+0.5);

    transluc = (int)(SolidTranslucency / d_trans +0.5);
    sprintf(sk, "%d%%", transluc * 5);
    
    menu_par_new((*mFillTyp.pola)[0].txt, sk);
}

static void set_filltype (int poz0)
/*------------------------------*/
{
  char sk [10] ;
  int poz = poz0;

  if (poz > 10)
  {
      poz = 5;
      SolidPattern = 1;
  }
  else
  {
      SolidPattern = 0;
  }
  strcpy(sk,filltyp_tab[poz-5]);
  SolidFillTyp=poz-5;
  menu_par_new ((*mSolid.pola)[2].txt, sk) ;
}

void Solid (void)
/*---------------*/
{
  S3D=FALSE;
  Solid_();
}

#undef __O_SOLID__