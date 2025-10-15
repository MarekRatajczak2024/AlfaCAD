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

#define __O_PAR_B__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<values.h>
#include<stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "alf_res.h"
#include "o_dialog.h"

#include "menu.h"


#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

extern void setfillstyle_(int pattern, int color);
extern void ClearScreen(void);
extern void Local_Angle_Num(void);
extern LAYER Layers[MAX_NUMBER_OF_LAYERS];
extern int Get_PTRS__Text_Style_type(int czcionka);
extern int Add_New_Font_TTF_OTF(char *name, int type, int cur);
extern void FreeMouse(void);
extern void LockMouse(void);
extern void Reset_Pointer(void);
extern void set_st_jedn(void);

extern void deactiv_menu(TMENU *menu);
extern void activate_menu(TMENU *menu);

extern void frame_off(TMENU * menu);
extern void frame_on(TMENU * menu);

extern void inc_menu_level(TMENU * menu);
extern void dec_menu_level(void);
extern int get_cursor_posX(void);
extern int get_cursor_posY(void);
extern void move_pointer(int x, int y);
extern int ask_question (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);

//parametry ramki i sektorow
float del_sektor = 0.25;       //przesuniecie ramki do srodka rysunku
float h_sektor = 2.5;          //wysokosc ramki sektorow     
float t_sektor_h = 2.0;       //wysokosc tekstu opisu sektorow
unsigned char t_sektor_czcionka = 0;  //numer czcionki opisu sektorow
unsigned char linia_ramka = 32;         //typ-grubosc linii ramki
unsigned char linia_sektor = 64;        //typ-grubosc linii sektorow

unsigned char linia_ramka_ini = 32;         //typ-grubosc linii ramki
unsigned char linia_sektor_ini = 64;        //typ-grubosc linii sektorow

void Magnitudes(void);
static void near uaktualnij_polap(void);

static int asp = 0;

static void  nooop(void)
{}

static double DokF_=0.0;
static double FormatX_= 0.0 ;
static double FormatY_= 0.0 ;
static int formatx0, formaty0;
#define TADD 0x04	   /*teksty dodatkowe*/
extern BOOL Edit_File (char * f_name, int type, BOOL cur);
extern void menu_par_new(char * pole, const char * par);  
extern void Point_Origin (void) ;
extern void Local_Angle (void) ;
extern int mynCmdShow;
extern int EditFile(char *, int adjust, int nCmdShow);
extern TMENU mKolorAC;
extern int WhNumberTextStyle;

extern int new_color;
extern BOOL preview_blocked;
extern unsigned int colors_zone(unsigned int *color_to_set);
extern void Test_Menu (TMENU *menu);
extern void return_menu_par0 (char *pole, char * par);
extern BOOL get_sectors_blocks(char  **adp_l, char  **adk_l);
extern int	delete_block (void);
extern void GetTextLen (TEXT *ptrs_text, double *tdl);
extern void GetCharLeft(TEXT *ptrs_text, double *tdl);

static char *Aformats[]={u8"X","A4","A4_","A3", "A3_","A2", "A2_","A1", "A1_","A0", "A0_","a4","a4_","a3","a2","a1","a0",
                  "3*A4","4*A4","5*A4","6*A4","3*a4","4*a4","5*a4","6*a4",
                  "B4","B4_","B3","B3_","B2","B2_","B1","B1_","B0","B0_","A","A_","B","B_","C","C_","D","D_","E","E_","Legal","X"};

static char *tab_char_ASCII[] = {u8"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
							   "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z" };

extern int funits_no;
extern double funits[];
extern char* punits[];

extern double radius_magnitude;
extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m force per 1 mm on drawing paper
extern double flood_magnitude; //units per mm  default 10kN/m² load per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude; //units per mm  default 0.001 rad desplacement per 1 mm radius on drawing paper

extern double radius_magnitude0;
extern double depth_magnitude0; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude0; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude0; //units per mm  default 10kN/m load per 1 mm on drawing paper
extern double flood_magnitude0; //units per mm  default 10kN/m² load per 1 mm on drawing paper
extern double force_magnitude0; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude0; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude0; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude0;

extern double radius_magnitude_imp0;
extern double depth_magnitude_imp0; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude_imp0; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude_imp0; //units per mm  default 10kN/m load per 1 mm on drawing paper
extern double flood_magnitude_imp0; //units per mm  default 10kN/m² load per 1 mm on drawing paper
extern double force_magnitude_imp0; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude_imp0; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude_imp0; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude_imp0;

extern double thermal_precision;
extern double force_precision;
extern double moment_precision;
extern double displacement_precision;
extern double rotation_precision;
extern double load_precision;
extern double stress_precision;

extern double n_magnitude;
extern double v_magnitude;
extern double m_magnitude;
extern double d_magnitude;
extern double r_magnitude;
extern double rm_magnitude;
extern double s_magnitude;
extern double src_magnitude;
extern double p_magnitude;
extern double q_magnitude;
extern double sp_magnitude;
extern double sm_magnitude;

extern double n_magnitude0;
extern double v_magnitude0;
extern double m_magnitude0;
extern double d_magnitude0;
extern double r_magnitude0;
extern double rm_magnitude0;
extern double s_magnitude0;
extern double src_magnitude0;
extern double p_magnitude0;
extern double q_magnitude0;
extern double sp_magnitude0;
extern double sm_magnitude0;

extern double n_magnitude_imp0;
extern double v_magnitude_imp0;
extern double m_magnitude_imp0;
extern double d_magnitude_imp0;
extern double r_magnitude_imp0;
extern double rm_magnitude_imp0;
extern double s_magnitude_imp0;
extern double src_magnitude_imp0;
extern double p_magnitude_imp0;
extern double q_magnitude_imp0;
extern double sp_magnitude_imp0;
extern double sm_magnitude_imp0;

extern TMENU mTTF_OTF;
extern void Resize_Vector (void);

extern void Restore_Pointer(void);
extern int get_menu_level(void);
extern void set_menu_level(int menu_l);

extern BOOL Semaphore;

static TMENU mSektory_Arkusza = { 15,0,0,31,1,3,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmSektory_Arkusza,NULL,NULL };

static TMENU mParametry = { 10,0,0,30,1,3,ICONS | TADD | 0x2,CMNU,CMBR,CMTX,0,0,0,0,0,&pmParametry,NULL,NULL };


static BOOL go_refresh=FALSE;
static BOOL vector_refresh=FALSE;

 void  formatA(void)
{
  int format;
  static int fformats[46][2]=FormatyAr ;


  format=mFormat_r.poz+mFormat_r.foff;
  if(FormatX==fformats[format][0] && FormatY==fformats[format][1]) return;
  FormatX= jednostkiplt (fformats [format][0]) ;
  FormatY= jednostkiplt (fformats [format][1]) ;
  X_max=jednostkiplt(FormatX);
  Y_max=jednostkiplt(FormatY);
  Xmax=X_max+Xmin;
  Ymax=Y_max+Ymin;
  if (X>Xmax) X=Xmax;
  if (Y>Ymax) Y=Ymax;
  menu_par_new((*mParametry.pola)[0].txt,Aformats[format]);
  drawp(&mParametry);
}

  static long l__x = 0, l__y = 0 ;

void formatX (void)
/*------------------*/
{
  double xd, yd ;
  char sk [MaxTextLen] = "" ;
  int retval_no = 2 ;
  double buf_ret [2] ;

  sprintf (sk, "%5ld; %5ld", l__x, l__y) ;
  if (!get_string (sk, "", MaxTextLen, 0, 4)) goto restore ;
  if (FALSE == calculator (sk, &retval_no, buf_ret) || retval_no < 2)
  {
      goto restore ;
  }
  xd = buf_ret [0] ;
  yd = buf_ret [1] ;
  if (xd <= 0 || yd <= 0)
   { ErrList (3) ;
       goto restore ;
   }
  if (xd >= MAXINT || yd >= MAXINT)
   { ErrList (5) ;
       goto restore ;
   }
  if (FormatX == xd && FormatY == yd) goto restore ;
  l__x = FormatX = xd ;
  l__y = FormatY = yd ;
  X_max = jednostkiplt (FormatX) ;
  Y_max = jednostkiplt (FormatY) ;
  Xmax = X_max + Xmin ;
  Ymax = Y_max + Ymin ;
  if (X > Xmax) X = Xmax ;
  if (Y > Ymax) Y = Ymax ;
  sprintf (sk, "%5ld * %5ld", (long)xd, (long)yd) ;
  menu_par_new ((*mFormat_r.pola)[0/*5*/].txt, sk) ;
  menu_par_new((*mParametry.pola)[0].txt, sk);
  drawp (&mParametry) ;

restore:
  Restore_Pointer();

}

void  kursorS(void)
{ char sk[10];
  int S, n;
  static int fkursor[10]={10,20,30,40,50,60,70,80,90,0};
  static char *pkursor[]={u8"10","20","30","40","50","60","70","80","90",_MAX_};
  n=mKursorSize.poz+mKursorSize.foff;
  S=fkursor[n];
  if (KursorS==S) return;
  KursorS=S;
  menu_par_new((*mParametry.pola)[7].txt,pkursor[n]);
  drawp(&mParametry);
  Reset_Pointer();
}

void  kursorSB(void)
{
	char sk[10];
	int S, n;
	static int fkursor[10] = {110,120,130,140,150,160,170,180,190,100};
	static char *pkursor[] = {u8"+10","+20","+30","+40","+50","+60","+70","+80","+90",_PLUSMAX_};
	n = mKursorSizeB.poz + mKursorSizeB.foff;
	S = fkursor[n];
	if (KursorS == S) return;
	KursorS = S;
	menu_par_new((*mParametry.pola)[7].txt, pkursor[n]);
	drawp(&mParametry);
	Reset_Pointer();
}

void Uklad_kartezjanski(void)
{
  if (options1.uklad_geodezyjny==0) return;
  options1.uklad_geodezyjny=0;
  menu_par_new((*mParametry.pola)[4].txt, ParTxt[0]);
  drawp(&mParametry);
}

void Uklad_geodezyjny(void)
{
  if (options1.uklad_geodezyjny==1) return;
  options1.uklad_geodezyjny=1;
  menu_par_new((*mParametry.pola)[4].txt, ParTxt[1]);
  drawp(&mParametry);
}

void ramka_on(void)
{
	if (sektory_arkusza.ramka) return;
	sektory_arkusza.ramka = 1;
	menu_par_new((*mSektory_Arkusza.pola)[0].txt, ParTxt[2]);
}

void ramka_off(void)
{
	if (!sektory_arkusza.ramka) return;
	sektory_arkusza.ramka = 0;
	menu_par_new((*mSektory_Arkusza.pola)[0].txt, ParTxt[3]);
}

void sektory_on(void)
{
	if (sektory_arkusza.draw) return;
	sektory_arkusza.draw = 1;
	menu_par_new((*mSektory_Arkusza.pola)[1].txt, ParTxt[2]);
}

void sektory_off(void)
{
	if (!sektory_arkusza.draw) return;
	sektory_arkusza.draw = 0;
	menu_par_new((*mSektory_Arkusza.pola)[1].txt, ParTxt[3]);
}

void reversed_on(void)
{
	if (sektory_arkusza_ext.reversed) return;
	sektory_arkusza_ext.reversed = 1;
	menu_par_new((*mSektory_Arkusza.pola)[4].txt, ParTxt[2]);
}

void reversed_off(void)
{
	if (!sektory_arkusza_ext.reversed) return;
	sektory_arkusza_ext.reversed = 0;
	menu_par_new((*mSektory_Arkusza.pola)[4].txt, ParTxt[3]);
}

void sektory_prefix(void)
{
	int prefix;
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

	sprintf(sk, "%ld", sektory_arkusza.prefix);
	if (!get_string(sk, "", MaxTextLen, 0, 136)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		prefix = 0;
	}
	prefix = (int)buf_ret[0];
	if (prefix < 0) prefix = 0;
	sektory_arkusza.prefix = prefix;
	if (prefix == 0) strcpy(sk, "");
	else sprintf(sk, "%ld", prefix);
	menu_par_new((*mSektory_Arkusza.pola)[2].txt, sk);
}

void sektory_first_number(void)
{
	int first_number;
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

	sprintf(sk, "%ld", sektory_arkusza.first_number);
	if (!get_string(sk, "", MaxTextLen, 0, 137)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		first_number = 1;
	}
	first_number = (int)buf_ret[0];
	if (first_number < 0) first_number = 1;
	sektory_arkusza.first_number = first_number;
	sprintf(sk, "%ld", first_number);
	menu_par_new((*mSektory_Arkusza.pola)[3].txt, sk);
}

void sektory_dx(void)
{
	double dx;
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

	sprintf(sk, "%g", sektory_arkusza.dx);
	if (!get_string(sk, "", MaxTextLen, 0, 139)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		dx = 75;
	}
	dx = buf_ret[0];
	if (dx <= 10) dx = 75;
	sektory_arkusza.dx = dx;
	sprintf(sk, "%g", dx);
	menu_par_new((*mSektory_Arkusza.pola)[6].txt, sk);
}

void sektory_dy(void)
{
	double dy;
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

	sprintf(sk, "%g", sektory_arkusza_ext.dy);
	if (!get_string(sk, "", MaxTextLen, 0, 138)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		dy = 75;
	}
	dy = buf_ret[0];
	if (dy <= 10) dy = 75;
	sektory_arkusza_ext.dy = dy;
	sprintf(sk, "%g", dy);
	menu_par_new((*mSektory_Arkusza.pola)[5].txt, sk);
}

void margin_dh(void)
{
	double margin_width;
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

	sprintf(sk, "%g", sektory_arkusza_ext.margin_width);
	if (!get_string(sk, "", MaxTextLen, 0, 140)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		margin_width = 2.5;
	}
	margin_width = buf_ret[0];
	if (margin_width <= 1) margin_width = 2.5;
	sektory_arkusza_ext.margin_width = margin_width;
	sprintf(sk, "%g", margin_width);
	menu_par_new((*mSektory_Arkusza.pola)[7].txt, sk);
}

void offset_d(void)
{
	double frame_offset;
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

	sprintf(sk, "%g", sektory_arkusza_ext.frame_offset);
	if (!get_string(sk, "", MaxTextLen, 0, 141)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		frame_offset = 0.25;
	}
	frame_offset = buf_ret[0];
	if (frame_offset < 0) frame_offset = 0.25;
	sektory_arkusza_ext.frame_offset = frame_offset;
	sprintf(sk, "%g", frame_offset);
	menu_par_new((*mSektory_Arkusza.pola)[8].txt, sk);
}

void style_up(void)
{
	if (sektory_arkusza.styl == 0) return;
	sektory_arkusza.styl = 0;
	menu_par_new((*mSektory_Arkusza.pola)[13].txt, tablica_styli[sektory_arkusza.styl]);
}

void style_down(void)
{
	if (sektory_arkusza.styl == 1) return;
	sektory_arkusza.styl = 1;
	menu_par_new((*mSektory_Arkusza.pola)[13].txt, tablica_styli[sektory_arkusza.styl]);
}

void style_up_down(void)
{
	if (sektory_arkusza.styl == 2) return;
	sektory_arkusza.styl = 2;
	menu_par_new((*mSektory_Arkusza.pola)[13].txt, tablica_styli[sektory_arkusza.styl]);
}

void style_around(void)
{
	if (sektory_arkusza.styl == 3) return;
	sektory_arkusza.styl = 3;
	menu_par_new((*mSektory_Arkusza.pola)[13].txt, tablica_styli[sektory_arkusza.styl]);
}

static void grubosc_linii_r(void)
{
	char  sk[24];
	int typ;
	sektory_arkusza_ext.frame_line = mGruboscLiniiF.poz;
	
	strcpy(sk, (*mGruboscLiniiF.pola)[mGruboscLiniiF.poz].txt);
	menu_par_new((*mSektory_Arkusza.pola)[9].txt, sk);
}

static void grubosc_linii_z(void)
{
	char  sk[24];
	int typ;

	sektory_arkusza_ext.sector_line = mGruboscLiniiZ.poz;
	
	strcpy(sk, (*mGruboscLiniiZ.pola)[mGruboscLiniiZ.poz].txt);
	menu_par_new((*mSektory_Arkusza.pola)[10].txt, sk);
}

static void kolorF(void)
/*-----------------------------*/
{
  int i_poz, i_color ;
  char  sk[24], *sk1;

  i_poz = mKolorACF.foff + mKolorACF.poz ;
  i_color = Menu_No_to_Color_No (i_poz) ;
  sektory_arkusza_ext.frame_color = i_color;

  strcpy(sk,(*mKolorACF.pola)[i_poz].txt);
  sk1=strpbrk(sk," ");
  menu_par_new((*mSektory_Arkusza.pola)[11].txt,++sk1);
}

static void kolorFX(void)
/*-----------------------------*/
{
  int i_poz, i_color ;
  char  sk[24];

  char *sk1;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;
  char buf [100];
  int l_kr;

  if (colors_zone((unsigned int*)sektory_arkusza_ext.frame_color) == Dlg_Ret_Val_OK)
	  sektory_arkusza_ext.frame_color = new_color;
  i_poz=16;
  strcpy(buf,(*mKolorACF.pola)[i_poz].txt);
  sk1=strpbrk(buf," ");
  menu_par_new((*mSektory_Arkusza.pola)[11].txt, ++sk1);
  return;
}

static void kolorS(void)
{
    int i_poz, i_poz0, i_color ;
    char  sk[24], *sk1;

    i_poz0 = mStaticColors.foff + mStaticColors.poz ;

    i_poz = mKolorSTATIC.foff + mKolorSTATIC.poz ;
    i_color = Menu_No_to_Color_No (i_poz) ;

    switch (i_poz0)
    {
        case 0:
        static_colors.node_element_color = i_color;
            break;
        case 1:
            static_colors.tension_color = i_color;
            break;
        case 2:
            static_colors.compression_color = i_color;
            break;
        case 3:
            static_colors.shear_color = i_color;
            break;
        case 4:
            static_colors.moment_color = i_color;
            break;
        case 5:
            static_colors.deformation_color = i_color;
            break;
        case 6:
            static_colors.reaction_color = i_color;
            break;
        case 7:
            static_stress_colors.axial_stress_plus_color = i_color;
            break;
        case 8:
            static_stress_colors.axial_stress_minus_color = i_color;
            break;
        case 9:
            static_stress_colors.shear_stress_color = i_color;
            break;
        case 10:
            static_colors.dynamic_color = i_color;
            break;
    }

    strcpy(sk,(*mKolorSTATIC.pola)[i_poz].txt);
    sk1=strpbrk(sk," ");

    if (i_color >= 16) {
        strcpy(&sk, "");

        sprintf(sk, u8"%s%#ld", _OTHER_COLOR_, i_color);

        sk1 = strpbrk(sk, " ");
    } else {
        return_menu_par0((*mKolorSTATIC.pola)[i_color-1].txt, &sk);
        sk1 = strpbrk(sk, " ");
    }

    menu_par_new((*mStaticColors.pola)[i_poz0].txt,++sk1);
}

static void kolorSX(void)
/*-----------------------------*/
{
    int i_poz0, i_poz, i_color ;
    char  sk[24];

    char *sk1;
    int retval_no = 1 ;
    double buf_ret [1] ;
    double d;
    char buf [100];
    int l_kr;

    i_poz0 = mStaticColors.foff + mStaticColors.poz ;

    switch (i_poz0)
    {
        case 0:
            i_color=static_colors.node_element_color;
            break;
        case 1:
            i_color=static_colors.tension_color;
            break;
        case 2:
            i_color=static_colors.compression_color;
            break;
        case 3:
            i_color=static_colors.shear_color;
            break;
        case 4:
            i_color=static_colors.moment_color;
            break;
        case 5:
            i_color=static_colors.deformation_color;
            break;
        case 6:
            i_color=static_colors.reaction_color;
            break;
        case 7:
            i_color=static_stress_colors.axial_stress_plus_color;
            break;
        case 8:
            i_color=static_stress_colors.axial_stress_minus_color;
            break;
        case 9:
            i_color=static_stress_colors.shear_stress_color;
            break;
        case 10:
            i_color = static_colors.dynamic_color;
            break;
    }


    if (colors_zone((unsigned int*)i_color) == Dlg_Ret_Val_OK)
        i_color = new_color;

    switch (i_poz0)
    {
        case 0:
            static_colors.node_element_color = i_color;
            break;
        case 1:
            static_colors.tension_color = i_color;
            break;
        case 2:
            static_colors.compression_color = i_color;
            break;
        case 3:
            static_colors.shear_color = i_color;
            break;
        case 4:
            static_colors.moment_color = i_color;
            break;
        case 5:
            static_colors.deformation_color = i_color;
            break;
        case 6:
            static_colors.reaction_color = i_color;
            break;
        case 7:
            static_stress_colors.axial_stress_plus_color = i_color;
            break;
        case 8:
            static_stress_colors.axial_stress_minus_color = i_color;
            break;
        case 9:
            static_stress_colors.shear_stress_color = i_color;
            break;
        case 10:
            static_colors.dynamic_color = i_color;
            break;
    }

    i_poz=16;
    strcpy(buf,(*mKolorSTATIC.pola)[i_poz].txt);
    sk1=strpbrk(buf," ");

    if (i_color >= 16) {
        strcpy(&sk, "");

        sprintf(sk, u8"%s%#ld", _OTHER_COLOR_, i_color);

        sk1 = strpbrk(sk, " ");
    } else {
        return_menu_par0((*mKolorSTATIC.pola)[i_color-1].txt, &sk);
        sk1 = strpbrk(sk, " ");
    }

    menu_par_new((*mStaticColors.pola)[i_poz0].txt, ++sk1);
    return;
}

static void kolorZ(void)
/*-----------------------------*/
{
	int i_poz, i_color;
	char  sk[24], *sk1;

	i_poz = mKolorACZ.foff + mKolorACZ.poz;
	i_color = Menu_No_to_Color_No(i_poz);
	sektory_arkusza_ext.sector_color = i_color;

	strcpy(sk, (*mKolorACZ.pola)[i_poz].txt);
	sk1 = strpbrk(sk, " ");
	menu_par_new((*mSektory_Arkusza.pola)[12].txt, ++sk1);
}

static void kolorZX(void)
/*-----------------------------*/
{
	int i_poz, i_color;
	char  sk[24];

	char *sk1;
	int retval_no = 1;
	double buf_ret[1];
	double d;
	char buf[100];
	int l_kr;

	if (colors_zone((unsigned int*)sektory_arkusza_ext.sector_color) == Dlg_Ret_Val_OK)
		sektory_arkusza_ext.sector_color = new_color;

	i_poz = 16;
	strcpy(buf, (*mKolorACZ.pola)[i_poz].txt);
	sk1 = strpbrk(buf, " ");
	menu_par_new((*mSektory_Arkusza.pola)[12].txt, ++sk1);
	return;
}

static void (* COMNDTO[])(void)={
         nooop,nooop};

static void czcionkaZ(void)
//-------------------------
{
	char *sk;
	int i_poz, i_poz1;
    int n;

	i_poz = mCzcionkaZ.poz + mCzcionkaZ.foff;
    i_poz1 = mTTF_OTF.poz + mTTF_OTF.foff;
	if (i_poz >= WhNumberTextStyle)
	{
		i_poz = Add_New_Font_TTF_OTF(NULL, i_poz1, 1);
		if (i_poz == -1) return;
	}
	if (sektory_arkusza_ext.font == i_poz) return;
	if (i_poz >= WhNumberTextStyle) return;
	if (pmCzcionka[i_poz].txt[0] == '\0')
	{
		return;
	}
	sektory_arkusza_ext.font = i_poz;
	sk = (*mCzcionkaZ.pola)[i_poz].txt;
	menu_par_new((*mSektory_Arkusza.pola)[14].txt, sk);
}

 void  skalaF(void)
{ char sk[20];
  double krok_s0;
  double krok_g0;
  double S;
  static double fscales[14]={1,2,5,10,20,25,50,100,500,1000,5000,10000,0.5,0.2};
  krok_s0=milimetryob(krok_s);
  krok_g0=milimetryob(krok_g);
  S=fscales[mSkalaF.poz+mSkalaF.foff];
  if (SkalaF==S) return;
  SkalaF=S;
  sprintf (sk, "1:%lg", SkalaF) ;
  menu_par_new((*mParametry.pola)[1].txt,sk);
  if (!snap_)krok_s = jednostkiY(1);
  else krok_s = jednostkiOb(krok_s0);
  krok_g=jednostkiOb (krok_g0) ;
  drawp(&mParametry);
}

void skalaFX (void)
//----------------
{
  char sk [MaxTextLen], sk1 [20] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double krok_s0;
  double krok_g0;

  krok_s0=milimetryob(krok_s);
  krok_g0=milimetryob(krok_g);
  sk [0] = '\0' ;
  sprintf(sk,"%lg",SkalaF);
  if (!get_string (sk, "", MaxTextLen, 0, 6)) goto restore ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
      goto restore ;
  }
  if ( buf_ret [0] <= 0 )
  {
    ErrList (30) ;
    goto restore ;
  }
  SkalaF = (double)buf_ret [0] ;
  sprintf(sk,"%lg",SkalaF); 
  menu_par_new ((*mSkalaF.pola)[14].txt, sk) ;
  strcpy (sk1, "1:") ;
  strcat (sk1, sk) ;
  menu_par_new ((*mParametry.pola)[1].txt,sk1) ;
  if (!snap_)krok_s = jednostkiY(1);
  else krok_s = jednostkiOb(krok_s0);
  krok_g=jednostkiOb (krok_g0) ;
  drawp(&mParametry);

restore:
  Restore_Pointer();
}


 void  Doklad(void)
{ double D;
  int n;
  static double fprecision[5]={20,10,5,4,2};
  static char *pprecision[]={u8"0.05mm","0.1mm","0.2mm","0.25mm","0.5mm"};
  n=mDokladnosc.poz+mDokladnosc.foff;
  D=fprecision[n];
  if (DokladnoscF==D) return;
  DokladnoscF=D;
  X_max=jednostkiplt(FormatX);
  Y_max=jednostkiplt(FormatY);
  Xmax=X_max+Xmin;
  Ymax=Y_max+Ymin;
  if (X>Xmax) X=Xmax;
  if (Y>Ymax) Y=Ymax;
  menu_par_new((*mParametry.pola)[3].txt,pprecision[n]);
  drawp(&mParametry);
}

void DokladX(void)
//---------------
{
  char sk [MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;

  sk [0] = '\0' ;
  DF_to_String (sk, "%-6.4f", 1 / DokladnoscF, 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 39)) goto restore ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
      goto restore ;
  }
  d = buf_ret [0] ;
  if ( d <= 0 )
  {
    ErrList (16) ;
    goto restore ;
  }
  DokladnoscF = 1 / d ;
  X_max = jednostkiplt (FormatX) ;
  Y_max = jednostkiplt (FormatY) ;
  Xmax = X_max + Xmin ;
  Ymax = Y_max + Ymin ;
  if (X>Xmax) X=Xmax;
  if (Y>Ymax) Y=Ymax;
  DF_to_String (sk, "%-6.4f", d, 6) ;
  strcat (sk, " mm") ;
  menu_par_new ((*mParametry.pola)[3].txt, sk) ;
  menu_par_new ((*mDokladnosc.pola)[5].txt, sk) ;
  drawp (&mParametry) ;

restore:
  Restore_Pointer();
}


void Change_Precision(int kom_no, double *parameter, int menu_number)
{
    char sk [MaxTextLen] ;
    int retval_no = 1 ;
    double buf_ret [1] ;
    double d ;

    sk [0] = '\0' ;
    DF_to_String (sk, "%-6.4f", *parameter, 0) ;
    if (!get_string (sk, "", MaxTextLen, 0, kom_no)) goto restore ;
    if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
    {
        goto restore ;
    }
    d = buf_ret [0] ;
    if ( d <= 0 )
    {
        ErrList (16) ;
        goto restore ;
    }

    *parameter=d;
    DF_to_String (sk, "%-6.4f", d, 6) ;
    menu_par_new ((*mPrecision.pola)[menu_number].txt, sk) ;
    drawp (&mParametry) ;
    go_refresh=TRUE;
    vector_refresh=TRUE;

restore:
    Restore_Pointer();

    return;
}

void Change_Magnitude(int kom_no, double *parameter, int menu_number)
{
    char sk [MaxTextLen] ;
    int retval_no = 1 ;
    double buf_ret [1] ;
    double d ;

    sk [0] = '\0' ;
    DF_to_String (sk, "%-6.4f", *parameter, 0) ;
    if (!get_string (sk, "", MaxTextLen, 0, kom_no)) goto restore ;
    if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
    {
        goto restore ;
    }
    d = buf_ret [0] ;
    if ( d <= 0 )
    {
        ErrList (16) ;
        goto restore ;
    }

    *parameter=d;
    DF_to_String (sk, "%-6.4f", d, 6) ;
    if (menu_number==4)
    {
        sprintf(sk, "%-6.4f/%-6.4f", load_magnitude, flood_magnitude) ;
        menu_par_new ((*mMagnitude.pola)[menu_number].txt, sk) ;
        DF_to_String (sk, "%-6.4f", load_magnitude, 6) ;
        menu_par_new ((*mLoadMagnitude.pola)[0].txt, sk) ;
        DF_to_String (sk, "%-6.4f", flood_magnitude, 6) ;
        menu_par_new ((*mLoadMagnitude.pola)[1].txt, sk) ;
    }
    if (menu_number==12)
    {
        sprintf(sk, "%-6.4f/%-6.4f", s_magnitude, src_magnitude) ;
        menu_par_new ((*mMagnitude.pola)[menu_number].txt, sk) ;
        DF_to_String (sk, "%-6.4f", s_magnitude, 6) ;
        menu_par_new ((*mStressMagnitude.pola)[0].txt, sk) ;
        DF_to_String (sk, "%-6.4f", src_magnitude, 6) ;
        menu_par_new ((*mStressMagnitude.pola)[1].txt, sk) ;
    }
    else
    {
        DF_to_String (sk, "%-6.4f", d, 6) ;
        menu_par_new ((*mMagnitude.pola)[menu_number].txt, sk) ;
    }
    drawp (&mParametry) ;
    go_refresh=TRUE;
    vector_refresh=TRUE;

restore:
    Restore_Pointer();
    return;
}

void Thermal_Magnitude(void)
{
    Change_Magnitude(204, &thermal_magnitude, 5);
}

void Thermal_Precision(void)
{
    Change_Precision(210, &thermal_precision, 5);
}

void Stress_Precision(void)
{
    Change_Precision(220, &stress_precision, 6);
}

void Load_Magnitude(void)
{
    Change_Magnitude(203, &load_magnitude, 4);
}

void Flood_Magnitude(void)
{
    Change_Magnitude(223, &flood_magnitude, 4);
}

void Load_Precision(void)
{
    Change_Precision(209, &load_precision, 4);
}

void Force_Magnitude(void)
{
    Change_Magnitude(199, &force_magnitude, 0);
}

void Force_Precision(void)
{
    Change_Precision(205, &force_precision, 0);
}

void Moment_Magnitude(void)
{
    Change_Magnitude(200, &moment_magnitude, 1);
}

void Moment_Precision(void)
{
    Change_Precision(206, &moment_precision, 1);
}

void Displacement_Magnitude(void)
{
    Change_Magnitude(201, &displacement_magnitude, 2);
}

void Displacement_Precision(void)
{
    Change_Precision(207, &displacement_precision, 2);
}

void Rotation_Magnitude(void)
{
    Change_Magnitude(202, &rotation_magnitude, 3);
}

void Rotation_Precision(void)
{
    Change_Precision(208, &rotation_precision, 3);
}

void N_Magnitude(void)
{
    Change_Magnitude(212, &n_magnitude, 6);
}
void V_Magnitude(void)
{
    Change_Magnitude(213, &v_magnitude, 7);
}
void M_Magnitude(void)
{
    Change_Magnitude(214, &m_magnitude, 8);
}
void D_Magnitude(void)
{
    Change_Magnitude(215, &d_magnitude, 9);
}
void R_Magnitude(void)
{
    Change_Magnitude(216, &r_magnitude, 10);
}
void RM_Magnitude(void)
{
    Change_Magnitude(217, &rm_magnitude, 11);
}

void S_Magnitude(void)
{
    Change_Magnitude(219, &s_magnitude, 12);
}

void SRC_Magnitude(void)
{
    Change_Magnitude(224, &src_magnitude, 12);
}

void P_Magnitude(void)
{
    Change_Magnitude(222, &p_magnitude, 13);
}

void Q_Magnitude(void)
{
    Change_Magnitude(221, &q_magnitude, 14);
}

void reset_magnitude_SI(void)
{
    int key1;
    int x_cur = mouse_x;
    int y_cur = mouse_y;
    int ret=ask_question(2, _No_,_Yes_,"", _reset_mgnitude_to_, 12, _SI_, 11, 1, 61);
    Restore_Pointer();
    position_mouse(x_cur, y_cur);
    if (ret==1) key1=_YES_;
    else if (ret==2) key1=_NO_;
    else return;
    if (key1 == _YES_ || key1 == _yes_)
    {
        radius_magnitude=radius_magnitude0;
        depth_magnitude=depth_magnitude0;
        thermal_magnitude=thermal_magnitude0;
        load_magnitude=load_magnitude0;
        flood_magnitude=flood_magnitude0;
        force_magnitude=force_magnitude0;
        moment_magnitude=moment_magnitude0;
        displacement_magnitude=displacement_magnitude0;
        rotation_magnitude=rotation_magnitude0;

        n_magnitude=n_magnitude0;
        v_magnitude=v_magnitude0;
        m_magnitude=m_magnitude0;
        d_magnitude=d_magnitude0;
        r_magnitude=r_magnitude0;
        rm_magnitude=rm_magnitude0;
        s_magnitude=s_magnitude0;
        src_magnitude=src_magnitude0;
        q_magnitude=q_magnitude0;
        sp_magnitude=sp_magnitude0;
        sm_magnitude=sm_magnitude0;
        p_magnitude=p_magnitude0;

        uaktualnij_polap();
        Change = TRUE;
    }
    return;
}

void reset_magnitude_IMP(void)
{
    int key1;
    int x_cur = mouse_x;
    int y_cur = mouse_y;
    int ret=ask_question(2, _No_,_Yes_,"", _reset_mgnitude_to_, 12, _IMP_, 11, 1, 61);
    Restore_Pointer();
    position_mouse(x_cur, y_cur);
    if (ret==1) key1=_YES_;
    else if (ret==2) key1=_NO_;
    else return;
    if (key1 == _YES_ || key1 == _yes_)
    {
        radius_magnitude=radius_magnitude_imp0;
        depth_magnitude=depth_magnitude_imp0;
        thermal_magnitude=thermal_magnitude_imp0;
        load_magnitude=load_magnitude_imp0;
        flood_magnitude=flood_magnitude_imp0;
        force_magnitude=force_magnitude_imp0;
        moment_magnitude=moment_magnitude_imp0;
        displacement_magnitude=displacement_magnitude_imp0;
        rotation_magnitude=rotation_magnitude_imp0;

        n_magnitude=n_magnitude_imp0;
        v_magnitude=v_magnitude_imp0;
        m_magnitude=m_magnitude_imp0;
        d_magnitude=d_magnitude_imp0;
        r_magnitude=r_magnitude_imp0;
        rm_magnitude=rm_magnitude_imp0;
        s_magnitude=s_magnitude_imp0;
        src_magnitude=src_magnitude_imp0;
        q_magnitude=q_magnitude_imp0;
        sp_magnitude=sp_magnitude_imp0;
        sm_magnitude=sm_magnitude_imp0;
        p_magnitude=p_magnitude_imp0;

        uaktualnij_polap();
        Change = TRUE;
    }
}

void Jednost(void)
{
  double J;
  double krok_s0, krok_g0;
  int n;

  krok_s0=milimetryob(krok_s);
  krok_g0=milimetryob(krok_g); 
  n = mJednostki.poz + mJednostki.foff;
  J=funits[n];
  if (Jednostki==J) return;
  Jednostki=J;
  set_st_jedn();
  pozycja_kursora();
  menu_par_new((*mParametry.pola)[2].txt,punits[n]);
  if (!snap_)krok_s = jednostkiY(1);
  else krok_s=jednostkiOb (krok_s0) ;
  krok_g=jednostkiOb (krok_g0) ;
  drawp(&mParametry);
}

static void near uaktualnij_polap(void)
{ char sk[50],st[21], *sk1;
int format, i, i_menu_color;
int ret;

  static int fformats[46][2]=FormatyAr;
  for (format=1; format<46;format++)
	if(FormatX==fformats[format][0] && FormatY==fformats[format][1]) break;

  if (strcmp(Aformats[format], "X") != 0) strcpy(sk, Aformats[format]);
  else sprintf(sk, "%5ld * %5ld", FormatX, FormatY);
  
  menu_par_new((*mParametry.pola)[0].txt, sk);


  menu_par_new((*mFormat_r.pola)[0/*5*/].txt, " xxxxx * yyyyy");
  if (format==46)
  {
    sprintf (st, "%5ld * %5ld", (long)FormatX, (long)FormatY) ;
    l__x = (long)FormatX ;
    l__y = (long)FormatY ;
    menu_par_new((*mFormat_r.pola)[0/*5*/].txt, st);
  }

  sprintf (sk, "1:%lg", SkalaF) ;
  menu_par_new((*mParametry.pola)[1].txt,sk);

  sprintf(sk,"%6.3g",1/DokladnoscF);
  strcat(sk,"mm");
  menu_par_new((*mParametry.pola)[3].txt,sk);

  for (i = 0; i<5; i++)
   if (Jednostki == funits [i]) break;
  menu_par_new((*mParametry.pola)[2].txt, punits[i]);

  if (options1.uklad_geodezyjny==0) 

	  strcpy(sk, _CARTESIAN_);

    else strcpy(sk, _GEODETIC_);
  menu_par_new((*mParametry.pola)[4].txt,sk);

  if (KursorS == 0)
  {
  strcpy(sk, _MAX_);
  }
  else if (KursorS == 100)
  {
	  strcpy(sk, _PLUSMAX_);
  }
  else if (KursorS<100)
   {
       ret = snprintf(sk, 10, "%d",KursorS);
   }
  else
  {
	  sprintf(sk,"+%d",KursorS-100);
  }
  menu_par_new((*mParametry.pola)[7].txt,sk);

  //sektory arkusza
  if (sektory_arkusza.ramka) strcpy(sk, ParTxt[2]); else strcpy(sk, ParTxt[3]);
  menu_par_new((*mSektory_Arkusza.pola)[0].txt, sk);

  if (sektory_arkusza.draw) strcpy(sk, ParTxt[2]); else strcpy(sk, ParTxt[3]);
  menu_par_new((*mSektory_Arkusza.pola)[1].txt, sk);

  if (sektory_arkusza.prefix > 0)
  {
	  sprintf(sk, "%ld", sektory_arkusza.prefix);
  }
  else strcpy(sk, "");
  menu_par_new((*mSektory_Arkusza.pola)[2].txt, sk);

  sprintf(sk, "%ld", sektory_arkusza.first_number);
  menu_par_new((*mSektory_Arkusza.pola)[3].txt, sk);

  sprintf(sk, "%lg", sektory_arkusza.dx);
  menu_par_new((*mSektory_Arkusza.pola)[5].txt, sk);

  menu_par_new((*mSektory_Arkusza.pola)[13].txt, tablica_styli[sektory_arkusza.styl]);

  //sektory arkusza ext

  if (sektory_arkusza_ext.reversed) strcpy(sk, ParTxt[2]); else strcpy(sk, ParTxt[3]);
  menu_par_new((*mSektory_Arkusza.pola)[4].txt, sk);

  sprintf(sk, "%lg", sektory_arkusza_ext.dy);
  menu_par_new((*mSektory_Arkusza.pola)[6].txt, sk);

  sprintf(sk, "%lg", sektory_arkusza_ext.margin_width);
  menu_par_new((*mSektory_Arkusza.pola)[7].txt, sk);

  sprintf(sk, "%lg", sektory_arkusza_ext.frame_offset);
  menu_par_new((*mSektory_Arkusza.pola)[8].txt, sk);

  mGruboscLinii.poz = sektory_arkusza_ext.frame_line;

  strcpy(sk, (*mGruboscLinii.pola)[mGruboscLinii.poz].txt);
  menu_par_new((*mSektory_Arkusza.pola)[9].txt, sk);

  mGruboscLinii.poz = sektory_arkusza_ext.sector_line;

  strcpy(sk, (*mGruboscLinii.pola)[mGruboscLinii.poz].txt);
  menu_par_new((*mSektory_Arkusza.pola)[10].txt, sk);

  i_menu_color = Color_No_to_Menu_No(sektory_arkusza_ext.frame_color);
  Test_Menu(&mKolorAC);
  if (i_menu_color >= 16)
  {
	  i_menu_color = 16;
  }
  Get_Menu_Param(i_menu_color, mKolorAC.maxw, mKolorAC.max,
	  &mKolorAC.foff, &mKolorAC.poz);
  if (i_menu_color >= 16)
  {
	  strcpy(&sk, "");

	  sprintf(sk, u8"%s%#ld", _OTHER_COLOR_,sektory_arkusza_ext.frame_color);

	  sk1 = strpbrk(sk, " ");
  }
  else
  {
	  return_menu_par0((*mKolorAC.pola)[i_menu_color].txt, &sk);
	  sk1 = strpbrk(sk, " ");
  }
  menu_par_new((*mSektory_Arkusza.pola)[11].txt, ++sk1);

  i_menu_color = Color_No_to_Menu_No(sektory_arkusza_ext.sector_color);
  Test_Menu(&mKolorAC);
  if (i_menu_color >= 16)
  {
	  i_menu_color = 16;
  }
  Get_Menu_Param(i_menu_color, mKolorAC.maxw, mKolorAC.max,
	  &mKolorAC.foff, &mKolorAC.poz);
  if (i_menu_color >= 16)
  {
	  strcpy(&sk, "");

	  sprintf(sk, u8"%s%#ld", _OTHER_COLOR_,sektory_arkusza_ext.sector_color);

	  sk1 = strpbrk(sk, " ");
  }
  else
  {
	  return_menu_par0((*mKolorAC.pola)[i_menu_color].txt, &sk);
	  sk1 = strpbrk(sk, " ");
  }
  menu_par_new((*mSektory_Arkusza.pola)[12].txt, ++sk1);

  mCzcionkaZ.max = WhNumberTextStyle + 1;

  strcpy(sk, (*mCzcionkaZ.pola)[sektory_arkusza_ext.font].txt);
  menu_par_new((*mSektory_Arkusza.pola)[14].txt, sk);

    sprintf(sk, "%lg", force_magnitude);
    menu_par_new((*mMagnitude.pola)[0].txt, sk);
    sprintf(sk, "%lg", moment_magnitude);
    menu_par_new((*mMagnitude.pola)[1].txt, sk);
    sprintf(sk, "%lg", displacement_magnitude);
    menu_par_new((*mMagnitude.pola)[2].txt, sk);
    sprintf(sk, "%lg", rotation_magnitude);
    menu_par_new((*mMagnitude.pola)[3].txt, sk);
    sprintf(sk, "%lg/%lg", load_magnitude, flood_magnitude);
    menu_par_new((*mMagnitude.pola)[4].txt, sk);

    sprintf(sk, "%lg", load_magnitude);
    menu_par_new((*mLoadMagnitude.pola)[0].txt, sk);
    sprintf(sk, "%lg", flood_magnitude);
    menu_par_new((*mLoadMagnitude.pola)[1].txt, sk);

    sprintf(sk, "%lg", thermal_magnitude);
    menu_par_new((*mMagnitude.pola)[5].txt, sk);

    sprintf(sk, "%lg", n_magnitude);
    menu_par_new((*mMagnitude.pola)[6].txt, sk);
    sprintf(sk, "%lg", v_magnitude);
    menu_par_new((*mMagnitude.pola)[7].txt, sk);
    sprintf(sk, "%lg", m_magnitude);
    menu_par_new((*mMagnitude.pola)[8].txt, sk);
    sprintf(sk, "%lg", d_magnitude);
    menu_par_new((*mMagnitude.pola)[9].txt, sk);
    sprintf(sk, "%lg", r_magnitude);
    menu_par_new((*mMagnitude.pola)[10].txt, sk);
    sprintf(sk, "%lg", rm_magnitude);
    menu_par_new((*mMagnitude.pola)[11].txt, sk);
    sprintf(sk, "%lg/%lg", s_magnitude, src_magnitude);
    menu_par_new((*mMagnitude.pola)[12].txt, sk);

    sprintf(sk, "%lg", s_magnitude);
    menu_par_new((*mStressMagnitude.pola)[0].txt, sk);
    sprintf(sk, "%lg", src_magnitude);
    menu_par_new((*mStressMagnitude.pola)[1].txt, sk);

    sprintf(sk, "%lg", p_magnitude);   //% of reinforcement
    menu_par_new((*mMagnitude.pola)[13].txt, sk);
    sprintf(sk, "%lg", q_magnitude);  //exaggerate modal modes of vibrations
    menu_par_new((*mMagnitude.pola)[14].txt, sk);

    sprintf(sk, "%lg", force_precision);
    menu_par_new((*mPrecision.pola)[0].txt, sk);
    sprintf(sk, "%lg", moment_precision);
    menu_par_new((*mPrecision.pola)[1].txt, sk);
    sprintf(sk, "%lg", displacement_precision);
    menu_par_new((*mPrecision.pola)[2].txt, sk);
    sprintf(sk, "%lg", rotation_precision);
    menu_par_new((*mPrecision.pola)[3].txt, sk);
    sprintf(sk, "%lg", load_precision);
    menu_par_new((*mPrecision.pola)[4].txt, sk);
    sprintf(sk, "%lg", thermal_precision);
    menu_par_new((*mPrecision.pola)[5].txt, sk);
    sprintf(sk, "%lg", stress_precision);
    menu_par_new((*mPrecision.pola)[6].txt, sk);

    unsigned int static_colors_ptr[]={static_colors.node_element_color, static_colors.tension_color, static_colors.compression_color, static_colors.shear_color, static_colors.moment_color, static_colors.deformation_color, static_colors.reaction_color,
                                      static_stress_colors.axial_stress_plus_color, static_stress_colors.axial_stress_minus_color, static_stress_colors.shear_stress_color, static_colors.dynamic_color };

    for (i=0; i<11; i++)
    {
        i_menu_color = Color_No_to_Menu_No(static_colors_ptr[i]);
        ////Test_Menu(&mKolorSTATIC);
        if (i_menu_color >= 16) {
            i_menu_color = 16;
        }
        ////Get_Menu_Param(i_menu_color, mKolorSTATIC.maxw, mKolorSTATIC.max, &mKolorSTATIC.foff, &mKolorSTATIC.poz);
        if (i_menu_color >= 16) {
            strcpy(&sk, "");

            sprintf(sk, u8"%s%#ld", _OTHER_COLOR_, static_colors_ptr[i]);

            sk1 = strpbrk(sk, " ");
        } else {
            return_menu_par0((*mKolorSTATIC.pola)[i_menu_color].txt, &sk);
            sk1 = strpbrk(sk, " ");
        }
        menu_par_new((*mStaticColors.pola)[i].txt, ++sk1);
    }

}

static void redcr0(char typ)
{ static int KR,KR0;
  if (typ==0)
   { KR=Komunikat_R;
     KR0=Komunikat_R0;
     komunikat(0);
     komunikat0(0);
     uaktualnij_polap();
     //CUR_OFF(X,Y);
   }
  else if (typ==1)
   { d_2d();

     Xp_Yp();

	 if (asp || X_max<=X2d || Y_max<=Y2d)
	  { setfillstyle_(EMPTY_FILL,0);
   	    bar(xp,yp,maxX-xk,maxY-yk);
	  }
	 if(!snap_)krok_s=jednostkiY(1);
    if (Check_if_Equal(DokladnoscF,DokF_)==FALSE) regen_ctx=TRUE;
    if (Check_if_Equal(FormatX,formatx0)==FALSE) regen_ctx=TRUE;
    if (Check_if_Equal(FormatY,formaty0)==FALSE) regen_ctx=TRUE;


	 ClearScreen();

	 redraw();
	 komunikat(KR);
	 komunikat0(KR0);
   }
    else
     {
      d_2d();
	   Xk_Yk();
   	
	   if(!snap_)krok_s=jednostkiY(1);
   
  	   komunikat(KR);
	   komunikat0(KR0);
     }
}

void  PointOrigin(void)
{
 Point_Origin ();
}

void  LocalAngle(void)
{
 Local_Angle ();
}

void  LocalAngleNum(void)
{
 Local_Angle_Num ();
}


void  Edycja_ALFACAD_INI(void)
{ BOOL ret_edit;
  preview_blocked = TRUE;
#ifndef LINUX
  ////FreeMouse();
#endif
  ret_edit = EditFile(FILENAME_INI, 0, mynCmdShow);
#ifndef LINUX
  ////LockMouse();
#endif
  preview_blocked = FALSE;
  return;
}


static BOOL add_block (char kod_bloku, char obiektt2)
/*-------------------------------------------------*/
{
#ifndef LINUX
  BLOKD s_blockd = BDdef ;
#else
    BLOK s_blockd = BDdef ;
#endif
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = kod_bloku;  //B_PLINE ;
  s_blockd.obiektt2 = obiektt2;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  c_pltype = PL_OTHER ;
  memmove (&(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  return TRUE ;
}


static int usuwanie_ramki_sektorow(void)   //wraz z opisami numerow
{
	char *adp0, *adk0;
	//usuniecie linii L.obiektt2 = O2BlockSpecial;
	InfoList(2);
	ADP = dane;
	ADK = dane + dane_size;
	usun_blok(ADP, ADK);

	ADP = NULL; ADK = NULL;
	get_sectors_blocks(&ADP, &ADK);
	adp0 = ADP;
	adk0 = ADK;
	if ((ADP != NULL) && (ADK != NULL))
	{
		delete_block();
		ADP = adp0;
		ADK = adk0;
		usun_blok(ADP, ADK);
		InfoList(0);
		return 1;
	}
	else
	{
		InfoList(0);
		return 0;
	}
}

int Lin0(double x1, double y1, double x2, double y2, int typ, int kolor)
{
	LINIA L = Ldef;
	L.warstwa = 254; //Current_Layer;
	L.x1 = x1; L.y1 = y1;
	L.x2 = x2; L.y2 = y2;
	L.typ = typ;
	L.kolor = kolor;
	L.blok = 1;
	L.obiektt1 = 0;
	L.obiektt2 = O2BlockPline;
	if (dodaj_obiekt((BLOK*)dane, (void*)&L) == NULL) return 0;
	else return 1;
}

int generowanie_ramki_sektorow()
/*----------------------------*/
{
	double odl;
	int i;
	char tekst_p[60];
	char tekst_p1[60];
	int l_kr;
	int LengthT;
	TEXT T = Tdef;
	int i_sektor;
	double del_text, del_char;
	unsigned char kolor_ramka;
	unsigned char kolor_sektor;
	float max_wspx, min_wspx, sektory_arkusza_dx;
	int f_type;

	if (FALSE == add_block(B_SEKTORY, 0))
	{
		return 0;
	}

	T.obiektt2 = O2BlockPline;
	T.wysokosc = sektory_arkusza_ext.margin_width  * 0.75; //t_sektor_h;
	T.czcionka = sektory_arkusza_ext.font;  //t_sektor_czcionka;
	T.warstwa = 254;
	T.blok = 1;
	T.kolor = sektory_arkusza_ext.sector_color;
	T.justowanie = j_centralnie;
	T.width = 0.0;
	T.height = 0.0;


	linia_ramka = sektory_arkusza_ext.frame_line * 32;
	kolor_ramka = sektory_arkusza_ext.frame_color;
	linia_sektor = sektory_arkusza_ext.sector_line * 32;
	kolor_sektor = sektory_arkusza_ext.sector_color;
	h_sektor = sektory_arkusza_ext.margin_width;
	del_sektor = sektory_arkusza_ext.frame_offset;
	
	if (sektory_arkusza.ramka == 1)
	{
		if (!Lin0(del_sektor, del_sektor, del_sektor, FormatY - del_sektor, linia_ramka, kolor_ramka)) return 0;
		if (!Lin0(del_sektor, FormatY - del_sektor, FormatX - del_sektor, FormatY - del_sektor, linia_ramka, kolor_ramka)) return 0;
		if (!Lin0(del_sektor, del_sektor, FormatX - del_sektor, del_sektor, linia_ramka, kolor_ramka)) return 0;
		if (!Lin0(FormatX - del_sektor, del_sektor, FormatX - del_sektor, FormatY - del_sektor, linia_ramka, kolor_ramka)) return 0;
	}

	if (sektory_arkusza.draw == 1)
	{
		if (sektory_arkusza.styl != 1)  //ramka gorna
		{
			if (sektory_arkusza.ramka == 0)
			{
				if (!Lin0(del_sektor, FormatY - del_sektor, FormatX - del_sektor, FormatY - del_sektor, linia_sektor, kolor_sektor)) return 0;
				if (!Lin0(del_sektor, FormatY - del_sektor - h_sektor, del_sektor, FormatY - del_sektor, linia_sektor, kolor_sektor)) return 0;
				if (!Lin0(FormatX - del_sektor, FormatY - del_sektor - h_sektor, FormatX - del_sektor, FormatY - del_sektor, linia_sektor, kolor_sektor)) return 0;
			}
			if (sektory_arkusza.styl == 3)
			{
				if (!Lin0(del_sektor + h_sektor, FormatY - del_sektor - h_sektor, FormatX - del_sektor - h_sektor, FormatY - del_sektor - h_sektor, linia_sektor, kolor_sektor)) return 0;
			}
			else
			{
				if (!Lin0(del_sektor, FormatY - del_sektor - h_sektor, FormatX - del_sektor, FormatY - del_sektor - h_sektor, linia_sektor, kolor_sektor)) return 0;
			}
			//linie sektorow
			i_sektor = 0;

			max_wspx = FormatX - del_sektor;
			min_wspx = del_sektor;
			if (sektory_arkusza.styl == 3)  //also vertical zones
			{
				max_wspx -= h_sektor;
				min_wspx += h_sektor;
			}

			if (sektory_arkusza_ext.reversed)
			{
				sektory_arkusza_dx = -sektory_arkusza.dx;
				odl = FormatX - del_sektor;
				if (sektory_arkusza.styl == 3)
					odl -= h_sektor;
			}
			else
			{
				sektory_arkusza_dx = sektory_arkusza.dx;
				odl = del_sektor;
				if (sektory_arkusza.styl == 3)
					odl += h_sektor;
			}

			while ((odl <= max_wspx) && (odl >= min_wspx))
			{
				odl += sektory_arkusza_dx;

				if ((odl < max_wspx) && (odl > min_wspx))
					if (!Lin0(odl, FormatY - del_sektor - h_sektor, odl, FormatY - del_sektor, linia_sektor, kolor_sektor)) return 0;
				//tekst

				T.x = odl - (sektory_arkusza_dx / 2);
				if (sektory_arkusza.prefix > 0) sprintf(tekst_p, "%ld.%ld", sektory_arkusza.prefix, sektory_arkusza.first_number + i_sektor);
				else sprintf(tekst_p, "%ld", sektory_arkusza.first_number + i_sektor);
				strcpy(&T.text[0], tekst_p);
				LengthT = strlen(T.text);
				T.dl = LengthT;
				T.n = T18 + T.dl;
                GetTextLen(&T, &del_text);
				GetCharLeft(&T, &del_char);

				if ((T.x < (max_wspx - ((del_text - del_char) / 2))) && (T.x > min_wspx + ((del_text - del_char) / 2)))
				{
					T.y = FormatY - del_sektor - 0.5 * h_sektor;

					if (dodaj_obiekt((BLOK*)dane, (void*)&T) == NULL) return 0;
				}
				
				i_sektor++;
			}
		}
		if (sektory_arkusza.styl != 0)  //ramka dolna
		{
			if (sektory_arkusza.ramka == 0)
			{
				if (!Lin0(del_sektor, del_sektor, FormatX - del_sektor, del_sektor, linia_sektor, kolor_sektor)) return 0;
				if (!Lin0(del_sektor, del_sektor, del_sektor, del_sektor + h_sektor, linia_sektor, kolor_sektor)) return 0;
				if (!Lin0(FormatX - del_sektor, del_sektor, FormatX - del_sektor, del_sektor + h_sektor, linia_sektor, kolor_sektor)) return 0;
			}
			if (sektory_arkusza.styl == 3)
			{
				if (!Lin0(del_sektor + h_sektor, del_sektor + h_sektor, FormatX - del_sektor - h_sektor, del_sektor + h_sektor, linia_sektor, kolor_sektor)) return 0;
			}
			else
			{
				if (!Lin0(del_sektor, del_sektor + h_sektor, FormatX - del_sektor, del_sektor + h_sektor, linia_sektor, kolor_sektor)) return 0;
			}
			//linie sektorow
			
			i_sektor = 0;
			max_wspx = FormatX - del_sektor;
			min_wspx = del_sektor;
			if (sektory_arkusza.styl == 3)  //also vertical zones
			{
				max_wspx -= h_sektor;
				min_wspx += h_sektor;
			}

			if (sektory_arkusza_ext.reversed)
			{
				sektory_arkusza_dx = -sektory_arkusza.dx;
				odl = FormatX - del_sektor;
				if (sektory_arkusza.styl == 3)
					odl -= h_sektor;
			}
			else
			{
				sektory_arkusza_dx = sektory_arkusza.dx;
				odl = del_sektor;
				if (sektory_arkusza.styl == 3)
					odl += h_sektor;
			}

			while ((odl <= max_wspx) && (odl >= min_wspx))
			{
				odl += sektory_arkusza_dx;

				if ((odl < max_wspx) && (odl > min_wspx))
					if (!Lin0(odl, del_sektor, odl, del_sektor + h_sektor, linia_sektor, kolor_sektor)) return 0;
				//tekst
				T.x = odl - (sektory_arkusza_dx / 2);
				if (sektory_arkusza.prefix > 0) sprintf(tekst_p, "%ld.%ld", sektory_arkusza.prefix, sektory_arkusza.first_number + i_sektor);
				else sprintf(tekst_p, "%ld", sektory_arkusza.first_number + i_sektor);
				strcpy(&T.text[0], tekst_p);
				LengthT = strlen(T.text);
				T.dl = LengthT;
				T.n = T18 + T.dl;
				GetTextLen(&T, &del_text);
				GetCharLeft(&T, &del_char);

				if ((T.x < (max_wspx - ((del_text - del_char) / 2))) && (T.x > min_wspx + ((del_text - del_char) / 2)))
				{
					T.y = del_sektor + 0.5 * h_sektor;

					if (dodaj_obiekt((BLOK*)dane, (void*)&T) == NULL) return 0;
				}
			
				i_sektor++;
			}
		}
		if (sektory_arkusza.styl == 3)  //ramka boczna A B C D E F ...
		{
			if (sektory_arkusza.ramka == 0)
			{
				if (!Lin0(del_sektor, del_sektor, del_sektor, FormatY - del_sektor, linia_sektor, kolor_sektor)) return 0;
				if (!Lin0(FormatX - del_sektor, del_sektor, FormatX - del_sektor, FormatY - del_sektor, linia_sektor, kolor_sektor)) return 0;
			}
			//lewa ramka
			if (!Lin0(del_sektor + h_sektor, del_sektor + h_sektor, del_sektor + h_sektor, FormatY - del_sektor - h_sektor, linia_sektor, kolor_sektor)) return 0;
			//linie sektorow
			odl = sektory_arkusza_ext.dy + h_sektor + del_sektor;
		
			i_sektor = 0;
			while (odl < (FormatY - h_sektor - del_sektor + sektory_arkusza_ext.dy))
			{
				if (odl < (FormatY - h_sektor - del_sektor))
					if (!Lin0(del_sektor, odl, del_sektor + h_sektor, odl, linia_sektor, kolor_sektor)) return 0;
				//tekst
				T.y = odl - (sektory_arkusza_ext.dy / 2);
				if (T.y < (FormatY - h_sektor - del_sektor - 2))
				{
					if (Get_PTRS__Text_Style_type(T.czcionka)==2) T.x = del_sektor + (h_sektor * 0.5);
					else T.x = del_sektor + (h_sektor * 0.625);
					if (i_sektor < 52) strcpy(&T.text[0], tab_char_ASCII[i_sektor]);
					else strcpy(&T.text[0], "*");
					T.text[1] = '\0';
					LengthT = strlen(T.text);
					T.dl = LengthT;
					T.n = T18 + T.dl;
					T.width = 0.0;
					T.height = 0.0;
					if (dodaj_obiekt((BLOK*)dane, (void*)&T) == NULL) return 0;
				}
				odl += sektory_arkusza_ext.dy;
				i_sektor++;
			}

			//prawa ramka
			if (!Lin0(FormatX - del_sektor - h_sektor, del_sektor + h_sektor, FormatX - del_sektor - h_sektor, FormatY - del_sektor - h_sektor, linia_sektor, kolor_sektor)) return 0;
			//linie sektorow
			odl = sektory_arkusza_ext.dy + h_sektor + del_sektor;
			
			i_sektor = 0;
			while (odl < (FormatY - h_sektor - del_sektor + sektory_arkusza_ext.dy))
			{
				if (odl < (FormatY - h_sektor - del_sektor))
					if (!Lin0(FormatX - del_sektor - h_sektor, odl, FormatX - del_sektor, odl, linia_sektor, kolor_sektor)) return 0;
				//tekst
				T.y = odl - (sektory_arkusza_ext.dy / 2);
				if (T.y < (FormatY - h_sektor - del_sektor - 2))
				{
					if (Get_PTRS__Text_Style_type(T.czcionka) == 2) T.x = FormatX - del_sektor - (h_sektor * 0.5);
					else T.x = FormatX - del_sektor - (h_sektor * 0.375);
					if (i_sektor < 52) strcpy(&T.text[0], tab_char_ASCII[i_sektor]);
					else strcpy(&T.text[0], "*");
					T.text[1]='\0';
					LengthT = strlen(T.text);
					T.dl = LengthT;
					T.n = T18 + T.dl;
					T.width = 0.0;
					T.height = 0.0;
					if (dodaj_obiekt((BLOK*)dane, (void*)&T) == NULL) return 0;
				}
				odl += sektory_arkusza_ext.dy;
				i_sektor++;
			}

		}
	}

	Layers[254].edit = FALSE;
	return 1;
}


static /*const*/ void(*COMND1[])(void) = {nooop, nooop, sektory_prefix, sektory_first_number, nooop,
                                      sektory_dy, sektory_dx, margin_dh, offset_d, nooop, nooop, nooop, nooop, nooop, nooop,
							/*15*/	  ramka_on, ramka_off, 
							/*17*/    sektory_on, sektory_off,
	                        /*19*/    reversed_on, reversed_off, 
							/*21*/	  grubosc_linii_r, grubosc_linii_r, grubosc_linii_r, grubosc_linii_r, grubosc_linii_r,
							/*26*/	  grubosc_linii_z, grubosc_linii_z, grubosc_linii_z, grubosc_linii_z, grubosc_linii_z,
							/*31*/	  kolorF,kolorF,kolorF,kolorF,kolorF,kolorF,kolorF,kolorF,
									  kolorF,kolorF,kolorF,kolorF,kolorF,kolorF,kolorF,kolorF,kolorFX,
							/*48*/	  kolorZ,kolorZ,kolorZ,kolorZ,kolorZ,kolorZ,kolorZ,kolorZ,
									  kolorZ,kolorZ,kolorZ,kolorZ,kolorZ,kolorZ,kolorZ,kolorZ,kolorZX,
							/*65*/	  style_up, style_down, style_up_down, style_around,
	                        /*69*/    czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,czcionkaZ,
									czcionkaZ,czcionkaZ };

void SektoryArkusza(void)
{
	int n;
	SEKTORY sektory_arkusza0;
	SEKTORY_EXT sektory_arkusza_ext0;
	int l_kr;

	DokF_ = DokladnoscF;
	FormatX_ = FormatX;
	FormatY_ = FormatY;

	memcpy(&sektory_arkusza0, &sektory_arkusza, sizeof(SEKTORY));
	memcpy(&sektory_arkusza_ext0, &sektory_arkusza_ext, sizeof(SEKTORY_EXT));

    frame_off(&mParametry);
    deactiv_menu(&mParametry);

    Test_Menu(&mSektory_Arkusza);

	while (1)
    {
        if ((n = getwsp1(&mSektory_Arkusza) - 1) == -1)
        {
            if ((memcmp(&sektory_arkusza, &sektory_arkusza0, sizeof(SEKTORY)) != 0) ||
                (memcmp(&sektory_arkusza_ext, &sektory_arkusza_ext0, sizeof(SEKTORY_EXT)) != 0))
            {
                closew(&mParametry);
                //usuwanie starego bloku sektorow
                l_kr = usuwanie_ramki_sektorow();
                //generowanie nowego bloku sektorow
                l_kr = generowanie_ramki_sektorow();
                BOOL dynamic_menu_=dynamic_menu;
                dynamic_menu=FALSE;
                Error=0;
                Info=0;
                openw(&mParametry);
                dynamic_menu=dynamic_menu_;
                go_refresh=TRUE;
                break;
            }
            break;
        }
        else
        {
            (*COMND1[n])();
            Restore_Pointer();
        }
    }

    activate_menu(&mParametry);
    frame_on(&mParametry);
    Restore_Pointer();
}


/*----------------------------------------------------*/
static void (* COMND[])(void)={
/*0 param*/          nooop,nooop,nooop,nooop,nooop ,nooop, SektoryArkusza, nooop, Magnitudes,Edycja_ALFACAD_INI, nooop,
/*11 format*/        formatX,formatA,formatA,formatA,formatA,formatA,
                     formatA,formatA,formatA,formatA,formatA,
                     formatA,formatA,formatA,formatA,formatA,
                     formatA,formatA,formatA,formatA,formatA,
                     formatA,formatA,formatA,formatA,formatA,
                     formatA,formatA,formatA,formatA,formatA,
                     formatA,formatA,formatA,formatA,formatA,
                     formatA,formatA,formatA,formatA,formatA,
                     formatA,formatA,formatA,formatA,formatA,
/*57 skala */        skalaF,skalaF,skalaF,skalaF,skalaF,skalaF,skalaF,
                     skalaF,skalaF,skalaF,skalaF,skalaF,skalaF,skalaF,skalaFX,
/*72 dokladnosc */   Doklad,Doklad,Doklad,Doklad,Doklad,DokladX,
/*78 jednostki  */   Jednost,Jednost,Jednost,Jednost,Jednost, Jednost, Jednost, Jednost,
/*86 origin,local*/  PointOrigin, LocalAngle, LocalAngleNum,
/*89 kursor */       kursorS, kursorS, kursorS, kursorS, kursorS, kursorS, kursorS, kursorS, kursorS, kursorS,
/*99 kursorB */	     kursorSB, kursorSB, kursorSB, kursorSB, kursorSB, kursorSB, kursorSB, kursorSB, kursorSB, kursorSB,
/*109 uklad */       Uklad_kartezjanski, Uklad_geodezyjny,
                     Force_Magnitude, Moment_Magnitude, Displacement_Magnitude, Rotation_Magnitude, nooop/*Load_Magnitude*/, Thermal_Magnitude,
                     N_Magnitude, V_Magnitude, M_Magnitude, D_Magnitude, R_Magnitude, RM_Magnitude, nooop, P_Magnitude, Q_Magnitude,
                     nooop,nooop, nooop,

                     Force_Precision, Moment_Precision, Displacement_Precision, Rotation_Precision, Load_Precision, Thermal_Precision, Stress_Precision,
                     kolorS,kolorS,kolorS,kolorS,kolorS,kolorS,kolorS,kolorS,
                     kolorS,kolorS,kolorS,kolorS,kolorS,kolorS,kolorS,kolorS,kolorSX,
                     Load_Magnitude, Flood_Magnitude, S_Magnitude, SRC_Magnitude, reset_magnitude_SI, reset_magnitude_IMP,
};

/*----------------------------------------------------*/


void Magnitudes(void)
{
    int n, n0, n1, n2;
    int m_level;
    int x_cur, y_cur;
    n0 = mMagnitude.off;
    n1 = mPrecision.off;
    n2 = mStaticColors.off;

    frame_off(&mParametry);
    deactiv_menu(&mParametry);

    Test_Menu(&mMagnitude);

    while (1) {
        n = getwsp1(&mMagnitude);
        if (n == 0)
        {
            set_menu_level(get_menu_level()+1);  //just in case size of the window was changed
            closew(&mMagnitude);
            set_menu_level(get_menu_level()-1);
            break;
        }
        else if (n == 127)  //Precision
        {
            frame_off(&mMagnitude);
            deactiv_menu(&mMagnitude);
            Test_Menu(&mPrecision);
            while (1) {
                n = getwsp1(&mPrecision);
                if (n == 0)
                {
                    //closew(&mPrecision);
                    //inc_menu_level(&mPrecision);
                    activate_menu(&mMagnitude);
                    break;
                }
                else
                {
                    x_cur = mouse_x;
                    y_cur = mouse_y;
                    (*COMND[n - 1 + n0])();
                    position_mouse(x_cur, y_cur);
                }
            }
            frame_on(&mMagnitude);
        }
        else if (n == 128)  //Colors
        {
            frame_off(&mMagnitude);
            deactiv_menu(&mMagnitude);
            Test_Menu(&mStaticColors);
            while (1)
            {
                n = getwsp1(&mStaticColors);
                if (n == 0)
                {
                    //closew(&mStaticColors);
                    //inc_menu_level(&mMagnitude);
                    activate_menu(&mMagnitude);
                    break;
                }
                else
                {
                    x_cur = mouse_x;
                    y_cur = mouse_y;
                    (*COMND[n - 1 + n0])();
                    Restore_Pointer();
                    position_mouse(x_cur, y_cur);
                }
            }
            frame_on(&mMagnitude);
        }
        else
        {
            x_cur = mouse_x;
            y_cur = mouse_y;
            (*COMND[n - 1])();
            position_mouse(x_cur, y_cur);
        }
    }


    inc_menu_level(&mParametry);
    activate_menu(&mParametry);
    frame_on(&mParametry);
}



double Jednost_(void)
{
  int n;
  double Jednostki1;
  uaktualnij_polap ();
  if (Error)
  { komunikat(Komunikat_R);   
    Error=0;
  }
  komunikat(100);
  if ((n=getwsp1(&mJednostki)-1)==-1)
   {
    komunikat(0);
    return 0;  
   } 
  komunikat(0); 
  n-=52;
  Jednostki1=funits[n];
  return Jednostki1;
}

double SkalaF_(void)
{ static double fscales[14]={1,2,5,10,20,25,50,100,500,1000,5000,10000,0.5,0.2};
  int n;
  char sk [MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double SkalaF1;
  
  uaktualnij_polap ();
  if (Error)
  { komunikat(Komunikat_R);   
    Error=0;
  }
  komunikat(101);
  if ((n=getwsp1(&mSkalaF)-1)==-1)
   {
    komunikat(0);
    return 0.0;  
   }
  komunikat(0);
  n-=31;
  if (n<14)
   {
    SkalaF1=fscales[n];
    return SkalaF1;
   }
    else
     {
      strcpy(sk,"100") ;
      if (!get_string (sk, "", MaxTextLen, 0, 6)) return 0.0;
      if (FALSE == calculator (sk, &retval_no, buf_ret)  ||
         (Check_if_Equal(retval_no,0) == TRUE))
       {
        return 0.0;
       }
      if ( buf_ret [0] <= 0 || (buf_ret [0] >= MAXINT))
       {
        ErrList (9) ;
        return 0.0;
       }
      SkalaF1 = buf_ret [0] ;
      return SkalaF1;  
     }
   return 0.0;  
}


void Parametry(void)
{ int n;
  int l_kr;
    int m_level;

	Semaphore = FALSE;

    go_refresh=FALSE;
  vector_refresh=FALSE;

 DokF_=DokladnoscF;
 formatx0=FormatX; formaty0=FormatY;
 CUR_OFF(X, Y);
 redcr0(0);
 while(1)
  if((n=getwsp1(&mParametry)-1)==-1)  /* n=-1 -> esc */
   {
      if (vector_refresh) Resize_Vector ();
	 redcr0(go_refresh);
	 if (!go_refresh) CUR_ON(X, Y);
     return;
   }
  else
  {
   if ((n==86) || (n==87) || (n == 88) || (n == 9))  //9 is edit INI file
	{
	closew(&mParametry);
	redcr0(0);
	(*COMND[n])();
	go_refresh = TRUE;
	return;
	}

   else if (n==8)
   {       //vectors
       (*COMND[n])();
       m_level=get_menu_level();
       Restore_Pointer();
       if (get_menu_level()==0)
           return;
   }

    else if (n==6)  //temporary
     {       //sektory arkusza
      (*COMND[n])();
     }
  	   else
	   {
           if (((n > 10) && (n <= 56)) || ((n > 71) && (n <= 77)) /*|| (n == 67) || (n == 68)*/)  go_refresh = TRUE;
           redcr0(2);
           (*COMND[n])();
	   }
	   if (((formatx0 != FormatX) || (formaty0 != FormatY))
		   && ((sektory_arkusza.draw == 1) || (sektory_arkusza.ramka == 1)))
	   {
           closew(&mParametry);
		   //usuwanie starego bloku sektorow
		   l_kr = usuwanie_ramki_sektorow();
		   //generowanie nowego bloku sektorow
		   l_kr = generowanie_ramki_sektorow();
           BOOL dynamic_menu_=dynamic_menu;
           dynamic_menu=FALSE;
           Error=0;
           Info=0;
           openw(&mParametry);
           activate_menu(&mParametry);
           dynamic_menu=dynamic_menu_;

           Restore_Pointer();
	   }
  }
 CUR_ON(X, Y);
 return;
}

typedef enum
{
	Linia_ramka = 0,
	Linia_sektor,
}ID_Sectors_Config;

static int no_config_sectors = sizeof(config_sectors) / sizeof(config_sectors[0]);

static BOOL get_config_sectors_(T_Fstring key_name, T_Fstring ret_string)
/*----------------------------------------------------------------------*/
{
	int i, val_int;
	double val_double;

	for (i = 0; i < no_config_sectors; i++)
	{
		if (stricmp(key_name, config_sectors[i]) == 0)
		{
			break;
		}
	}
	if (i >= no_config_sectors)
	{
		return FALSE;
	}
	switch (i)
	{
	case Linia_ramka:
		if (sscanf(ret_string, "%d", &val_int) == 1)
		{
			if ((val_int >= 0) && (val_int <= 160)) linia_ramka_ini = val_int;
		}
		break;
	case Linia_sektor:
		if (sscanf(ret_string, "%d", &val_int) == 1)
		{
			if ((val_int >= 0) && (val_int <= 160)) linia_sektor_ini = val_int;
		}
		break;
	default:
		break;
	}
	return TRUE;
}


BOOL get_config_sectors(void)
/*--------------------------*/
{
	return Get_Private_Profile_Strings((T_Fstring)_SECTORS_, get_config_sectors_);
}

#undef __O_PAR_B__
