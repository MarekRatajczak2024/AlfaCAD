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

#define __O_TEXT_B__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include <stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "alffont.h"
#include "o_loadf.h"
#include "o_dialog.h"

#include "menu.h"

#include "leak_detector_c.h"

#define cursor_off() out_cur_off(X,Y)
#define cursor_on() out_cur_on(X,Y)
#define TADD 0x04	   /*additional texts*/

#define r60 128

#define LINE myline

void view_font_name(TEXT *);

extern BOOL Add_String_To_List (char *) ;
extern GrContext *get_second_screen(void);
extern GrContext *second_screen_back;
extern BOOL TTF_redraw;
extern void outtextxy_w_(TEXT  *t,int mode);
extern double Angle_Normal_Grid (double angle) ;
extern void Out_Edited_Draw_Param (ESTR *, BOOL ) ;
extern BOOL Check_if_Equal (double , double ) ;
extern BOOL Check_if_Equal2(double, double);
extern BOOL Check_if_GT (double , double ) ;
extern BOOL Check_if_LT (double , double ) ;
extern void DF_to_String (char *, char *, double, int) ;
extern void Uaktualnij(void) ;
extern void regeneracja_profilu(void);
extern void setfillstyle_(int pattern, int color);
extern void setcolor(int kolor);
extern void setwritemode( int mode );
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void myline(int x1, int y1, int x2, int y2);
extern void delete_instal_tab(void);
extern void Clear_View (void);
extern void Ini_Layers_Dlg (void);
extern BLOK *FIRSTB(char  *ado);
extern int delete_block1 (void);
extern int add_000(char *tekst_p, int n_digit);
extern void sprintf_prec(char *string, double dokladnosc, double l);
extern int get_lock_prof_aktual(void);
extern void *find_block(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu);
extern char *find_obj1(char *adrp, char *adrk, int obiekt, int typ_obiektu, char obiektt2);
extern char *find_obj(char *adrp, char *adrk, int obiekt, int typ_obiektu, int info_p);
extern BLOK *LASTB(char  *ado);
extern void przesuniecie_profilu_(void);
extern void delay(int time);
extern int EditText(char *, int adjust, int nCmdShow, int *single, int *tab);
extern int mynCmdShow;
extern BOOL preview_blocked;
extern BOOL ttf_refresh;
extern int ttf_digits27_len;
extern float ttf_width_w;
extern int Add_New_Font_TTF_OTF(char *mame, int type, int cur);
extern int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext);
extern void unicode2win(char *unicodetext, char *wintext, int count);
extern void utf82win(char *utf8text, char *wintext);

extern char digits[16];
extern char numbers[16];
extern char xdigits[16];

extern TMENU mTTF_OTF;
extern void najblizszyl_ (double *x, double *y, void *adr);
extern void najblizszyL_ (double *x, double *y, void *adr);
extern void wielokat_linia_ (double *x, double *y, void *adr, void *adr1, void (*fun)(double*, double*, void*));
extern void najblizszyE (double *x, double *y, void *adr);
extern BOOL najblizszyEA_(double *x,double *y,void *adr);
extern int mwspline_(double* x, double* y, void* adr, int(*fun)(double*, double*, void*));
extern int najblizszyLx_(double* x, double* y, void* adr);
double get_angle_tangent_to_object_ (void *ptr_ob, double df_x, double df_y);
extern void DonePatternBitmapScaled(void);

/*-----------------------------------------------------------------------*/

static char *ADP1;
char GMultiText[MaxMultitextLen + 1];
int GMultiParams;

BOOL text_edit_dialog = TRUE;

POLE pmLineSpacing[] = {
		{u8"1.0",' ',0,NULL},
		{u8"1.2",' ',0,NULL},
		{u8"1.4",' ',0,NULL},
		{u8"1.6",' ',0,NULL},
		{u8"1.8",' ',0,NULL},
		{u8"2.0",' ',0,NULL},
		{u8"2.2",' ',0,NULL},
		{u8"2.4",' ',0,NULL},
		{u8"2.6",' ',0,NULL},
		{u8"2.8",' ',0,NULL},
		{u8"3.0",' ',0,NULL},
		{u8"3.2",' ',0,NULL},
		{u8"3.4",' ',0,NULL},
		{u8"3.6",' ',0,NULL},
		{u8"3.8",' ',0,NULL},
		{u8"4.0",' ',0,NULL},
		};

TMENU mLineSpacing = { 16,0,0,5,74,6,0,CMNU,CMBR,CMTX,0,47,0,0,0,&pmLineSpacing,NULL,NULL };

PPOLE pmCzcionka[256];

PTMENU mCzcionka={1,MaxMenuWinFont/*0*/,MaxMenuWinFont,FONTNAMELEN + 1,74,10,FIXED,CMNU,CMBR,CMTX,0,63,0,0,0,&pmCzcionka,NULL,NULL};

TMENU mTekstm={14,0,0,23,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmTekstm,NULL,NULL};

int get_dialog_string(char *tekst, char *legal, int maxlength, int width0, int kom, int *tab)
{
    int edit_params;
    int ret;
    int single=1;

    edit_params = TextG.bold * 2 + TextG.italics * 4 + TextG.underline * 8 + TextG.justowanie * 16 + TextG.ukryty * 64;
    preview_blocked = TRUE;
#ifndef LINUX
    ////FreeMouse();
#endif
    ret = EditText(tekst, edit_params, mynCmdShow, &single, tab);
#ifndef LINUX
    ////LockMouse();
#endif
    preview_blocked = FALSE;
    if (ret<1) return ret;

    TextG.bold = (BOOL) (ret & 2) / 2;
    TextG.italics = (BOOL) (ret & 4) / 4;
    TextG.underline = (BOOL) (ret & 8) / 8;
    TextG.justowanie = (int) ((ret & 48) / 16);
    TextG.ukryty = (BOOL)((ret & 64) / 64);

	Add_String_To_List(tekst);

    return 1;
}

void Re_Edit(void)
{
	return;  //do nothing, just let it re-edit
}

void  srodkowo(void)
{
  Cur_offd (X, Y) ;
  TextG.justowanie=j_srodkowo;
  menu_par_new ((*mTekstm.pola) [9].txt, "><") ;
  Cur_ond (X, Y) ;
  view_font_name(&TextG);
}      

void  centralnie(void)
{
  Cur_offd (X, Y) ;
  TextG.justowanie=j_centralnie;
  menu_par_new ((*mTekstm.pola) [9].txt, ">|<") ;
  Cur_ond (X, Y) ;
  view_font_name(&TextG);
}      

void  do_prawej(void)
{
  Cur_offd (X, Y) ;
  TextG.justowanie=j_do_prawej;
  menu_par_new ((*mTekstm.pola) [9].txt, ">|") ;
  Cur_ond (X, Y) ;
  view_font_name(&TextG);
}      
                    
void  do_lewej(void)
{
  Cur_offd (X, Y) ;
  TextG.justowanie=j_do_lewej;
  menu_par_new ((*mTekstm.pola) [9].txt, "|<") ;
  Cur_ond (X, Y) ;
  view_font_name(&TextG);
}

void  etykieta_sklad(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_etykieta_sklad;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[2]) ;
  view_font_name(&TextG);
}   

void  etykieta_wew(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_port;
  Cur_ond (X, Y) ;
  TextG.typ=n_etykieta_wew;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[1]) ;
  view_font_name(&TextG);
}   

void  typ_siec(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_siec;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[11]) ;
  view_font_name(&TextG);
}   

void  typ_plik(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_plik;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[10]) ;
  view_font_name(&TextG);
}   

void  typ_schemat(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_schemat;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[9]) ;
  view_font_name(&TextG);
}   

void  typ_opis_drabinki(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_opis_drabinki;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[15]) ;
  view_font_name(&TextG);
}   

void  typ_symbol_drabinki(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_symbol_drabinki;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[14]) ;
  view_font_name(&TextG);
}   

void  typ_atrybut(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_atrybut;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[8]) ;
  view_font_name(&TextG);
}   

void  typ_port(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_port;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[7]) ;
  view_font_name(&TextG);
}   

void  typ_zasilanie(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_zasilanie;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[6]) ;
  view_font_name(&TextG);
}   

void  typ_zmienna(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_zmienna;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[13]) ;
  view_font_name(&TextG);
}   

void  typ_komentarz(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_komentarz;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[12]) ;
  view_font_name(&TextG);
}   
 
void  typ_pin_name(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_pin_name;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[5]) ;
  view_font_name(&TextG);
}    
                    
void  typ_typ(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_typ;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[4]) ;
  view_font_name(&TextG);
}                        
                    
void  typ_symbol(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_symbol;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[3]) ;
  view_font_name(&TextG);
}                       
                    
                    
void  typ_normal(void)
{
  Cur_offd (X, Y) ;
  TextG.typ=n_typ_normal;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola) [5].txt, t_t[0]) ;
  view_font_name(&TextG);
}

void  katT0(void)
{
  if(TRUE == Check_if_Equal (TextG.kat, 0)) return;
  Cur_offd (X, Y) ;
  TextG.kat=0;
  menu_par_new ((*mTekstm.pola) [1].txt, "0") ;
  view_font_name(&TextG);
  Cur_ond (X, Y) ;
}


void  katT90(void)
{
  if(TRUE == Check_if_Equal (TextG.kat, Pi / 2)) return;
  Cur_offd(X,Y);
  TextG.kat = Pi / 2 ;
  menu_par_new ((*mTekstm.pola) [1].txt, "90") ;
  view_font_name(&TextG);
  Cur_ond(X,Y);
}


void katTsel(double kat)
{
  unsigned typ;
  void  *adr;
  PLINIA PL;
    LINIA L=Ldef;
  char sk [MaxTextLen] ;
    int l;
    double xx, yy, si, co, m;
    BOOL ret;

  typ=Blinia | Btekst | Bluk | Bokrag | Bkolo | Bwwielokat | Bellipse | Bfilledellipse | Bellipticalarc | Bspline | Bvector;
  if ((adr=select_n(&typ,NULL,75))!=NULL)
   {
     Cur_offd(X,Y);
       switch (typ) {
           case Btekst: TextG.kat = Angle_Normal(kat + ((TEXT *) adr)->kat);
               break;
           case Blinia:
               parametry_lini((LINIA *) adr, &PL);
               TextG.kat = Angle_Normal(kat + (PL.kat * Pi / 180.0));
               break;
           case Bvector:
               switch (((AVECTOR *)adr)->style)
               {
                   case 0:
                   case 1:
                   case 2:
                   case 3:
                   case 4:
                   case 18:
                   case 7:
                   case 10:
                   case 17:
                   case 11:
                   case 12:
                   case 13:
                   case 14:
                   case 15:
                   case 16:
                   parametry_lini((LINIA *) adr, &PL);
                   TextG.kat = Angle_Normal(kat + (PL.kat * Pi / 180.0));
                   break;
               }
               break;
           case Bluk:
               najblizszyl_(&X, &Y, adr);
               TextG.kat = Angle_Normal(Atan2(Y-((LUK*)adr)->y, X-((LUK*)adr)->x)+kat);
               break;
           case Bokrag:
           case Bkolo:
               najblizszyO_(&X, &Y, adr);
               TextG.kat = Angle_Normal(Atan2(Y-((OKRAG*)adr)->y, X-((OKRAG*)adr)->x)+kat);
               break;
           case Bwwielokat:
               wielokat_linia_ (&X, &Y, adr, &L, najblizszyL_) ;
               parametry_lini(&L, &PL);
               TextG.kat = Angle_Normal(kat + (PL.kat * Pi / 180.0));
               break;
           case Bellipse:
           case Bfilledellipse:
               najblizszyE(&X,&Y,adr);
               xx=X-((ELLIPSE*)adr)->x;
               yy=Y-((ELLIPSE*)adr)->y;
               si=sin(-((ELLIPSE*)adr)->angle);
               co=cos(-((ELLIPSE*)adr)->angle);
               Rotate_Point(si,co, 0,0,xx,yy,&xx,&yy);
               m=(-xx*((ELLIPSE*)adr)->ry*((ELLIPSE*)adr)->ry)/(yy*((ELLIPSE*)adr)->rx*((ELLIPSE*)adr)->rx);
               TextG.kat=Angle_Normal(atan(m)+((ELLIPSE*)adr)->angle + kat);
               if (yy<0) TextG.kat+=Pi;
               break;
           case Bellipticalarc:
               ret = najblizszyEA_(&X,&Y,adr);
               xx=X-((ELLIPSE*)adr)->x;
               yy=Y-((ELLIPSE*)adr)->y;
               si=sin(-((ELLIPSE*)adr)->angle);
               co=cos(-((ELLIPSE*)adr)->angle);
               Rotate_Point(si,co, 0,0,xx,yy,&xx,&yy);
               m=(-xx*((ELLIPSE*)adr)->ry*((ELLIPSE*)adr)->ry)/(yy*((ELLIPSE*)adr)->rx*((ELLIPSE*)adr)->rx);
               TextG.kat=Angle_Normal(atan(m)+((ELLIPSE*)adr)->angle + kat);
               if (yy<0) TextG.kat+=Pi;
               break;
           case Bspline:
               l = mwspline_(&X, &Y, adr, najblizszyLx_);
               if (l>0) TextG.kat=Angle_Normal(get_angle_tangent_to_object_ (adr, X, Y) + kat);
               break;
           default:
               break;
       }
     DF_to_String (sk, "%-6.2f", Rad_to_Grid (TextG.kat), 6) ;
     menu_par_new ((*mTekstm.pola) [1].txt, sk) ;
     view_font_name(&TextG);
     Cur_ond(X,Y);
   }
}

void katTselx(void)
{ unsigned typ;
  double kat;
  void  *adr;
  PLINIA PL;
  char sk [MaxTextLen]="";
  int retval_no = 1 ;
  double buf_ret [1] ;

 if (get_string (sk, "", MaxTextLen, 0, 15))
  {
    if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
    {
      return ;
    }
    kat = buf_ret [0] ;
    kat = Angle_Normal_Grid (kat) ;
    kat = kat * Pi / 180.0 ;

    katTsel(kat);
  }
}

void  katTr(void)
{ sel.akt=ASel;
  sel.gor = NGSel;
  katTsel(0);
  sel.akt=NASel;
  sel.gor = NGSel;
}

void  katTp(void)
{ sel.akt=ASel;
  sel.gor = NGSel;
  katTsel (Pi / 2.0) ;
  sel.akt=NASel;
  sel.gor = NGSel;
}

void  katTxk(void)
{ sel.akt=ASel;
  sel.gor = NGSel;
  katTselx();
  sel.akt=NASel;
  sel.gor = NGSel;
}

void  katTx(void)
{
  char sk[MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double kat;

  DF_to_String (sk, "%-6.2f", Rad_to_Grid (TextG.kat), 6) ;
  if (!get_string (sk, "", MaxTextLen, 0, 15)) return;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  Cur_offd(X,Y);
  kat = buf_ret [0] ;
  kat = Angle_Normal_Grid (kat) ;
  kat = kat * Pi / 180.0 ;
  if (kat >= Pi) kat -= Pi;
  if (kat > Pi / 2.0) kat += Pi;
  TextG.kat = kat;
  DF_to_String (sk, "%-6.2f", Rad_to_Grid (TextG.kat), 6) ;
  menu_par_new ((*mTekstm.pola) [1].txt, sk) ;
  view_font_name(&TextG);
  Cur_ond(X,Y);
}

void  wysokosc_znaku(void)
{
  char sk[MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;

  DF_to_String (sk, "%-6.2f", jednostki_d_to_plt_d (TextG.wysokosc), 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 10)) return;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if ( d <=0 )
  {
    ErrList (30);
    return ;
  }
  Cur_offd (X, Y) ;
  TextG.wysokosc = plt_d_to_jednostki_d (d) ;
  TextG.width = 0;
  TextG.height = 0;
  Cur_ond (X, Y) ;
  DF_to_String (sk, "%-6.2f", jednostki_d_to_plt_d (TextG.wysokosc), 6) ;
  strcat(sk,"mm");
  menu_par_new((*mTekstm.pola)[2].txt,sk);
  view_font_name(&TextG);
}

void  interwal(void)
{
  char sk[MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;
  char *bp;
  int new_interwal;

  sprintf(sk, "%ld", tekst_interwal) ;
  if (!get_string (sk, "", MaxTextLen, 0, 45)) return ;
  new_interwal=strtol(sk, &bp, 10);
  if (*bp=='\0')
   {
     tekst_interwal=new_interwal;
     menu_par_new((*mTekstm.pola)[12].txt,sk);
   }
   else
    {
      ErrList(16);
      return;
    }
}

static void width_factor (void)
/*----------------------------*/
{
  char sk[MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;

  DF_to_String (sk, "%-6.4f", TextG.width_factor, 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 33)) return;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if ( d <= 0 )
  {
    ErrList (30);
    return ;
  }
  if (d > 100)
  {
    d = 100 ;
  }
  Cur_offd (X, Y) ;
  TextG.width_factor= d ;
  TextG.width = 0;
  TextG.height = 0;
  Cur_ond (X, Y) ;
  DF_to_String (sk, "%-6.4f", TextG.width_factor, 6) ;
  menu_par_new((*mTekstm.pola)[3].txt,sk);
  view_font_name(&TextG);
}


static void italics_on (void)
/*--------------------------*/
{
  if (TextG.italics == 1) return;
  Cur_offd (X, Y) ;
  TextG.italics = 1 ;
  TextG.width = 0;
  TextG.height = 0;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola)[4].txt, SZ_YES) ;
  view_font_name(&TextG);
 }

static void italics_off (void)
/*--------------------------*/
{
  if (TextG.italics == 0) return;
  Cur_offd (X, Y) ;
  TextG.italics = 0 ;
  TextG.width = 0;
  TextG.height = 0;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola)[4].txt, SZ_NO) ;
  view_font_name(&TextG);
 }
 
 
static void ukryty_on (void)
/*--------------------------*/
{
  if (TextG.ukryty == 1) return;
  Cur_offd (X, Y) ;
  TextG.ukryty = 1 ;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola)[6].txt, SZ_YES) ;
  view_font_name(&TextG);
 }

static void ukryty_off (void)
/*--------------------------*/
{
  if (TextG.ukryty == 0) return;
  Cur_offd (X, Y) ;
  TextG.ukryty = 0 ;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola)[6].txt, SZ_NO) ;
  view_font_name(&TextG);
 }
 
static void bold_on (void)
/*--------------------------*/
{
  if (TextG.bold == 1) return;
  Cur_offd (X, Y) ;
  TextG.bold = 1 ;
  TextG.width = 0;
  TextG.height = 0;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola)[7].txt, SZ_YES) ;
  view_font_name(&TextG);
 }

static void bold_off (void)
/*--------------------------*/
{
  if (TextG.bold == 0) return;
  Cur_offd (X, Y) ;
  TextG.bold = 0 ;
  TextG.width = 0;
  TextG.height = 0;
  Cur_ond (X, Y) ;
  menu_par_new ((*mTekstm.pola)[7].txt, SZ_NO) ;
  view_font_name(&TextG);
 }

static void underlined_on(void)
/*--------------------------*/
{
	if (TextG.underline == 1) return;
	Cur_offd(X, Y);
	TextG.underline = 1;
	Cur_ond(X, Y);
	menu_par_new((*mTekstm.pola)[8].txt, SZ_YES);
	view_font_name(&TextG);
}

static void underlined_off(void)
/*--------------------------*/
{
	if (TextG.underline == 0) return;
	Cur_offd(X, Y);
	TextG.underline = 0;
	Cur_ond(X, Y);
	menu_par_new((*mTekstm.pola)[8].txt, SZ_NO);
	view_font_name(&TextG);
}

static void auto_interwal_on (void)
/*------------------------------*/
{
  if (auto_tekst == TRUE) return;
  auto_tekst = TRUE;
  menu_par_new ((*mTekstm.pola)[11].txt, SZ_YES) ;
}

static void auto_interwal_off (void)
/*--------------------------------*/
{
  if (auto_tekst == FALSE) return;
  auto_tekst = FALSE;
  menu_par_new ((*mTekstm.pola)[11].txt, SZ_NO) ;
}
 
void view_font_name(TEXT *t)
/*---------------------------------*/
{ char font_name[64];
  char font_high[11];
  char font_angle[9];
  char font_typ[24];
  char font_just[6];

  strcpy(&font_name,PTRS__Text_Style[t->czcionka]->font_name);
  sprintf(font_high,"%-6.2f",t->wysokosc);
  sprintf(font_angle,"%-6.2f",t->kat * (360 / (2 * Pi)));
  strcat(font_name," ");
  if (t->bold==1)
   {
     strcat(font_name,"B ");
   }
  strcat(font_name,font_high);
  strcat(font_name," @");
  strcat(font_name,font_angle);
  strcat(font_name," ");

  strcpy(font_typ, t_t[t->typ]);

  strcat(font_name,font_typ);
  switch (t->justowanie)
  {
  case 0 : strcpy(font_just," |<"); break;
  case 1 : strcpy(font_just," >|"); break;
  case 2 : strcpy(font_just," ><"); break;
  case 3 : strcpy(font_just," >|<"); break;
  default : strcpy(font_just," |<");break;
  }
  strcat(font_name,font_just);
  if (t->ukryty==1)
   {
    strcat(font_name, _HIDDEN_);
   }

  setfillstyle_(SOLID_FILL, BKCOLOR);
  bar(maxX / 2 + 20, HEIGHT + 1, maxX-(WIDTH*16), 2 * HEIGHT + 0);
  moveto(maxX/2+20,HEIGHT+2);
  setcolor(kolory.ink);
  outtext_r(font_name);
}   

static void lspacing (void)
//------------------------
{
char *sk;
int i_poz;

i_poz = mLineSpacing.poz + mLineSpacing.foff;
if (TextG.spacing == (unsigned char)(i_poz*2)) return;
if (i_poz >= 31) return;
Cur_offd(X, Y);
TextG.spacing = (unsigned char)(i_poz*2);
Cur_ond(X, Y);
sk = (*mLineSpacing.pola)[i_poz].txt;
menu_par_new((*mTekstm.pola)[10].txt, sk);
}

static void czcionka (void)
//-------------------------
{
  char *sk;
  int i_poz, i_poz1 ;
  int xspX, xspY;

  i_poz = mCzcionka.poz + mCzcionka.foff ;
  i_poz1 = mTTF_OTF.poz + mTTF_OTF.foff;
  if (i_poz >= WhNumberTextStyle)
  {
	  i_poz = Add_New_Font_TTF_OTF(NULL, i_poz1, 1);
	  if (i_poz == -1) return;
  }
  if (TextG.czcionka == i_poz) return ;
  if(i_poz >= WhNumberTextStyle) return ;
  if (pmCzcionka [i_poz].txt [0] == '\0')
  {
    return ;
  }
  Cur_offd (X, Y) ;
  TextG.czcionka = i_poz ;
  TextG.width = 0;
  TextG.height = 0;
  Cur_ond (X, Y) ;
  sk=(*mCzcionka.pola)[i_poz].txt;
  menu_par_new((*mTekstm.pola)[13].txt,sk);  
  view_font_name(&TextG);
}

int  kat_tt(void)
{
  Cur_offd (X, Y) ;
  TextG.kat+= 3 * Pi / 180.0 ;
  if (TextG.kat >= (2.0 * Pi)) TextG.kat -= (2 * Pi);
  Cur_ond (X, Y) ;
  view_font_name(&TextG);
  return 0;
}

int  kat_dt(void)
{
  Cur_offd (X, Y) ;
  TextG.kat-= 3 * Pi / 180.0 ;
  if (TextG.kat < 0) TextG.kat = (2 * Pi) + TextG.kat;
  Cur_ond (X, Y) ;
  view_font_name(&TextG);
  return 0;
}

int  kat_t90(void)  //adding 90 deg
{ Cur_offd (X, Y) ;
  if (TRUE==Check_if_LT(TextG.kat,(Pi / 2))) TextG.kat = Pi / 2 ;
    else if (TRUE==Check_if_LT(TextG.kat,Pi)) TextG.kat = Pi ;
      else if (TRUE==Check_if_LT(TextG.kat,(3 * Pi / 2))) TextG.kat = (3 * Pi / 2) ;
       else if (TRUE==Check_if_LT(TextG.kat,(2 * Pi))) TextG.kat = 0 ;

  Cur_ond (X, Y) ;
  view_font_name(&TextG);
  return 0;
}

int  kat_t0(void)
{
  Cur_offd (X, Y) ;
  if (TextG.kat < 0) TextG.kat = (2 * Pi) + TextG.kat;

  if (TRUE==Check_if_GT(TextG.kat,(3 * Pi / 2)) || (TextG.kat == 0)) TextG.kat = (3 * Pi / 2) ;
    else if (TRUE==Check_if_GT(TextG.kat,Pi)) TextG.kat = Pi ;
      else if (TRUE==Check_if_GT(TextG.kat,(Pi / 2))) TextG.kat = (Pi / 2) ;
       else if (TRUE==Check_if_GT(TextG.kat,0)) TextG.kat = 0 ;
  Cur_ond (X, Y) ;
  view_font_name(&TextG);
  return 0;
}


static void  cur_off(double x,double y)
{
    flip_screen();
}

static void  cur_on(double x,double y)
{int x0,y0,kursor05;

    TextG.x = x ;
    TextG.y = y ;
    mvcurb.mvc = 0 ;
    okno_r();
    outtextxy_w(&TextG,COPY_PUT);
    okno_all();
    cursel_on(x, y);
}


static void  draw_TextG_TTF(double x, double y)
{	
	okno_all();
	TextG.x = x;
	TextG.y = y;
	mvcurb.mvc = 0;
	outtextxy_wP(&TextG, COPY_PUT);
}

static void  nooop(void)
{}


static void (*COMNDmt[])(void)={
/*0*/	            Re_Edit,
/*0+1 Tekst  5*/    nooop,wysokosc_znaku, width_factor, ukryty_on, ukryty_off,
/*5  5 Kat  7*/     auto_interwal_on,auto_interwal_off,nooop,nooop,nooop,nooop,interwal,
                   
/*12 12 Italics 2*/ italics_on, italics_off,
/*14 14 Typ 15  */  typ_normal, etykieta_wew, etykieta_sklad, typ_symbol,
                    typ_typ, typ_pin_name, typ_zasilanie, typ_port,
                    typ_atrybut, typ_schemat, typ_plik, typ_siec,
                    typ_komentarz, typ_zmienna, typ_symbol_drabinki,
                    typ_opis_drabinki,  
/*30 31 Just. 8 */  do_lewej, do_prawej, srodkowo, centralnie, nooop,
                    nooop, bold_on, bold_off,
/*38*/				underlined_on, underlined_off,
/*40 */             katT0,katT90,katTr,katTp,katTxk,katTx,
/*46*/              lspacing, lspacing, lspacing, lspacing, lspacing,
					lspacing, lspacing, lspacing, lspacing, lspacing,
					lspacing, lspacing, lspacing, lspacing, lspacing,
					lspacing,
/*62 */             czcionka,czcionka,czcionka,czcionka,czcionka,
           		     czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,czcionka,czcionka,
           		     czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,czcionka,czcionka,
           		     czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,czcionka,czcionka,
           		     czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka,czcionka,czcionka,czcionka,
		              czcionka,czcionka };

static void set_menu_string (void)
/*-------------------------------*/
{
  char sk [MaxTextLen] ;

  DF_to_String (sk, "%-6.2f", jednostki_d_to_plt_d (TextG.wysokosc), 6) ;
  strcat(sk,"mm");
  menu_par_new ((*mTekstm.pola)[2].txt,sk); 
  DF_to_String (sk, "%-6.4f", TextG.width_factor, 6) ;
  menu_par_new ((*mTekstm.pola)[3].txt,sk); 
  strcpy (sk, SZ_NO) ;
  if (TextG.italics == 1)
  {
    strcpy (sk, SZ_YES) ;
  }
  menu_par_new ((*mTekstm.pola)[4].txt, sk) ;

  switch (TextG.typ)
  {
   case n_typ_normal: strcpy(sk,t_t[0]); break;
   case n_etykieta_wew: strcpy(sk,t_t[1]); break;
   case n_etykieta_sklad: strcpy(sk,t_t[2]); break;
   case n_typ_symbol: strcpy(sk,t_t[3]); break;
   case n_typ_typ: strcpy(sk,t_t[4]); break;
   case n_typ_pin_name: strcpy(sk,t_t[5]); break;
   case n_typ_zasilanie: strcpy(sk,t_t[6]); break;
   case n_typ_port: strcpy(sk,t_t[7]); break;
   case n_typ_atrybut: strcpy(sk,t_t[8]); break;
   case n_typ_schemat: strcpy(sk,t_t[9]); break;
   case n_typ_plik: strcpy(sk,t_t[10]); break;
   case n_typ_siec: strcpy(sk,t_t[11]); break;
   case n_typ_komentarz: strcpy(sk,t_t[12]); break;
   case n_typ_zmienna: strcpy(sk,t_t[13]); break;
   case n_typ_symbol_drabinki: strcpy(sk,t_t[14]); break;
   case n_typ_opis_drabinki: strcpy(sk,t_t[15]); break;
   default : strcpy(sk,t_t[0]); break;
  }

  menu_par_new ((*mTekstm.pola)[5].txt, sk) ;
  switch (TextG.justowanie)
  {
  case 0 : strcpy(sk,"|<"); break;
  case 1 : strcpy(sk,">|"); break;
  case 2 : strcpy(sk,"><"); break;
  case 3 : strcpy(sk,">|<"); break;
  default : strcpy(sk,"|<");break;
  }
  menu_par_new ((*mTekstm.pola)[9].txt, sk) ;
  
  strcpy (sk, SZ_NO) ;
  if (TextG.ukryty == 1)
  {
    strcpy (sk, SZ_YES) ;
  }
  menu_par_new ((*mTekstm.pola)[6].txt, sk) ;  
  
  strcpy (sk, SZ_NO) ;
  if (TextG.bold == 1)
  {
    strcpy (sk, SZ_YES) ;
  }
  menu_par_new ((*mTekstm.pola)[7].txt, sk) ; 

  strcpy(sk, SZ_NO);
  if (TextG.underline == 1)
  {
	  strcpy(sk, SZ_YES);
  }
  menu_par_new((*mTekstm.pola)[8].txt, sk);

  sprintf(sk, "%.1f", (float)(TextG.spacing/10.0+1.0));
  
  menu_par_new((*mTekstm.pola)[10].txt, sk);

  strcpy (sk, SZ_NO) ;
  if (auto_tekst == TRUE)
  {
    strcpy (sk, SZ_YES) ;
  }
  menu_par_new ((*mTekstm.pola)[11].txt, sk) ;

  sprintf(sk, "%ld", tekst_interwal) ;
  menu_par_new ((*mTekstm.pola)[12].txt,sk);

  strcpy(sk,(*mCzcionka.pola)[TextG.czcionka].txt);
  menu_par_new((*mTekstm.pola)[13].txt,sk);
}


static int pisz(BOOL repeat)
/*-------------------------*/
{
	int LengthT;
	char tekst[MaxTextLen];
	char *ptr_tekst;
	int len;
	BOOL bad_char;
	int i;
	int sufiks1;
	int sufiks2;
	char *bp;
    int ret;
	int tab = 0;
	char* ptr_n;

	if (WhNumberTextStyle == 0) { ErrList(17); return 1; }
	if (TextG.dl) {
        if (dodaj_obiekt(NULL, &TextG) == NULL) return 1;
        rysuj_obiekt(&TextG, COPY_PUT, 1);
    }


    else outtextxy_w(&TextG, COPY_PUT);

    flip_screen();

    if (repeat) {

        if (auto_tekst == FALSE)
        {
            strcpy(&TextG.text[0], "");
            komunikat0(1);
            if (text_edit_dialog == FALSE) ret = get_string(&TextG.text[0], "", MaxTextLen * 2, 0, 13);
            else ret = get_dialog_string(&TextG.text[0], "", MaxTextLen * 2, 0, 13, &tab);
            if (!ret) return 0;

            ptr_n = strstr(TextG.text, "\\n");
            while ((ptr_n != NULL) && (*ptr_n != 0)) {
                *(ptr_n) = '\r';
                *(ptr_n + 1) = '\n';
                ptr_n += 2;
                ptr_n = strstr(ptr_n, "\\n");
                TextG.multiline = 1;
            }

            komunikat0(121);
        } else {

            strcpy(&tekst, &TextG.text);
            ptr_tekst = tekst;
            //poszukiwanie liczby calkowitej na koncu lancucha
            len = strlen(TextG.text);
            i = 0;
            bad_char = TRUE;
            while ((i < len) && (bad_char == TRUE)) {
                sufiks1 = strtol(ptr_tekst, &bp, 10);
                if (*bp == '\0') {
                    sufiks2 = sufiks1;
                    bad_char = FALSE;
                } else {
                    ptr_tekst += 1;
                    i++;
                }
            }
            if (bad_char == FALSE) {
                *ptr_tekst = '\0';
                strcpy(&TextG.text, tekst);
                sufiks2 += tekst_interwal;
                sprintf(&TextG.text, "%s%ld", tekst, sufiks2);
            } else {
                strcpy(&TextG.text[0], "");
                komunikat0(0);

                if (text_edit_dialog == FALSE) ret = get_string(&TextG.text[0], "", MaxTextLen * 2, 0, 13);
                else ret = get_dialog_string(&TextG.text[0], "", MaxTextLen * 2, 0, 13, &tab);
                if (!ret) return 0;

                ptr_n = strstr(TextG.text, "\\n");
                while ((ptr_n != NULL) && (*ptr_n != 0)) {
                    *(ptr_n) = '\r';
                    *(ptr_n + 1) = '\n';
                    ptr_n += 2;
                    ptr_n = strstr(ptr_n, "\\n");
                    TextG.multiline = 1;
                }

                komunikat0(Komunikat_R0);
            }
        }

        LengthT = strlen(TextG.text);
        TextG.x = X;
        TextG.y = Y;
        TextG.dl = LengthT;
        TextG.n = T18 + TextG.dl;
        TextG.width = 0;
        TextG.height = 0;

        return 1;
    }
    else
    {

        return 0;
    }
}

static int multipisz(void)
/*-------------------*/
{
	int LengthT;
	char tekst[MaxTextLen];
	char *ptr_tekst;
	int len;
	BOOL bad_char;
	int i;
	int sufiks1;
	int sufiks2;
	char *bp;
	int ret;
	int edit_params;
    int single=0;
	int tab = 0;

	if (WhNumberTextStyle == 0) { ErrList(17); return 1; }
	if (TextG.dl) {
        if (dodaj_obiekt(NULL, &TextG) == NULL) return 1;
        rysuj_obiekt(&TextG, COPY_PUT, 1);
        flip_screen();
    }
	if (auto_tekst == FALSE)
	{
		strcpy(&TextG.text[0], "");
		komunikat0(1);

		edit_params = TextG.bold * 2 + TextG.italics * 4 + TextG.underline * 8 + TextG.justowanie * 16 + TextG.ukryty * 64;
		preview_blocked = TRUE;

#ifndef LINUX
		////FreeMouse();
#endif

		ret = EditText(&TextG.text[0], edit_params, mynCmdShow, &single, &tab);
#ifndef LINUX
		////LockMouse();
#endif
		preview_blocked = FALSE;
		if (!ret) return 0;
		TextG.bold = (BOOL)(ret & 2) / 2;
		TextG.italics = (BOOL)(ret & 4) / 4;
		TextG.underline = (BOOL)(ret & 8) / 8;
		TextG.justowanie = (int)((ret & 48) / 16);
        TextG.ukryty = (BOOL)((ret & 64) / 64);

		Add_String_To_List(&TextG.text[0]);

		komunikat0(121);
	}
	else
	{

		strcpy(&tekst, &TextG.text);
		ptr_tekst = tekst;
		//searching integer number at the end of the string
		len = strlen(TextG.text);
		i = 0;
		bad_char = TRUE;
		while ((i < len) && (bad_char == TRUE))
		{

			sufiks1 = strtol(ptr_tekst, &bp, 10);
			if (*bp == '\0')
			{
				sufiks2 = sufiks1;
				bad_char = FALSE;
			}
			else
			{
				ptr_tekst += 1;
				i++;
			}
		}
		if (bad_char == FALSE)
		{
			while ((*ptr_tekst == '\n') || (*ptr_tekst == '\r'))
				ptr_tekst++;
			*ptr_tekst = '\0';
			strcpy(&TextG.text, tekst);
			sufiks2 += tekst_interwal;
			sprintf(&TextG.text, "%s%ld", tekst, sufiks2);
		}
		else
		{
			strcpy(&TextG.text[0], "");
			komunikat0(0);

			edit_params = TextG.bold * 2 + TextG.italics * 4 + TextG.underline * 8 + TextG.justowanie * 16 + TextG.ukryty * 64;
			preview_blocked = TRUE;

#ifndef LINUX
			////FreeMouse();
#endif

			ret = EditText(&TextG.text[0], edit_params, mynCmdShow, &single, &tab);
#ifndef LINUX
			////LockMouse();
#endif
			preview_blocked = FALSE;
			if (!ret) return 0;
			TextG.bold = (BOOL)(ret & 2) / 2;
			TextG.italics = (BOOL)(ret & 4) / 4;
			TextG.underline = (BOOL)(ret & 8) / 8;
			TextG.justowanie = (int)((ret & 48) / 16);
            TextG.ukryty = (BOOL)((ret & 64) / 64);

			Add_String_To_List(&TextG.text[0]);

			komunikat0(Komunikat_R0);
		}
	}
	LengthT = strlen(TextG.text);
	TextG.x = X; TextG.y = Y; TextG.dl = LengthT;
	TextG.n = T18 + TextG.dl;
	TextG.width = 0;
	TextG.height = 0;
	return 1;
}


static void redcr(char typ)
{
	static void(*CUR_N)(double, double);
	static int(*SW[4])();
	static int sel_akt, sel_gor, sel_cur;
	static void(*CUR_F)(double, double);
	if (typ == 0)
	{
		CUR_OFF(X, Y);
		mvcurb.T = 1;
		CUR_F = CUR_OFF;  CUR_OFF = cur_off;
		CUR_N = CUR_ON;   CUR_ON = cur_on;
		SW[0] = SERV[73];  SERV[73] = kat_tt;  //PdUp
		SW[1] = SERV[81];  SERV[81] = kat_dt;  //PgDn
		SW[2] = SERV[71];  SERV[71] = kat_t90;  //Home  kat 0  t0
		SW[3] = SERV[79];  SERV[79] = kat_t0;  //End   kat 90 t90
		sel_akt = sel.akt;

		sel.akt = ASel;

		Cur_ond(X, Y);

		komunikat(0);
		komunikat0(1);

		menupini(&mTekstm, _TEXT_, _TEXT_C_, 16);
	}
	else
	{
		CUR_OFF = CUR_F;
		CUR_ON = CUR_N;
		SERV[73] = SW[0];
		SERV[81] = SW[1];
		SERV[71] = SW[2];
		SERV[79] = SW[3];
		komunikat(0);
		komunikat0(0);
		sel.akt = sel_akt;
		CUR_ON(X, Y);
		menupini(NULL, "", ' ', 0);
		TextG.multiline = 0;
	}
}

static void redcr___(char typ)
{ static void (*CUR_N)(double ,double);
  static int ( *SW[4])();
  static int sel_akt, sel_gor, sel_cur;
  static void (*CUR_F)(double ,double);
  if (typ==0)
   {
	 Cur_offd(X, Y);
     mvcurb.T = 1 ;
	 CUR_F = CUR_OFF;  CUR_OFF = cur_off;
	 CUR_N = CUR_ON;   CUR_ON = cur_on;
     SW[0]=SERV[73];  SERV[73]=kat_tt;  //PdUp
     SW[1]=SERV[81];  SERV[81]=kat_dt;  //PgDn
     SW[2]=SERV[71];  SERV[71]=kat_t90;  //Home  kat 0  t0
     SW[3]=SERV[79];  SERV[79]=kat_t0;  //End   kat 90 t90
     sel_akt=sel.akt;

	 sel.akt = ASel;

     komunikat (0);
     komunikat0 (1);

     menupini(&mTekstm, _TEXT_, _TEXT_C_, 16);

   }
  else
   {

     CUR_OFF=CUR_F;
     CUR_ON=CUR_N;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
     SERV[71]=SW[2];
     SERV[79]=SW[3];
     komunikat (0);
     komunikat0 (0) ;
     sel.akt=sel_akt;

     Cur_ond (X, Y) ;

     menupini(NULL,"",' ', 0);
   }
}


void avoid_smear(void)
{
	redcr(0);
	redcr(1);
}

void Multitekst(void)
{
	char sk[MaxMultitextLen] = "";
	int LengthT;
	EVENT *ev;
	double X0, Y0;
	int ret;
	int dll;
	//TEXT test_text;
	int edit_params;
	BOOL re_EDIT = FALSE;
    int single=0;
	int tab = 0;

	edit_functions = TRUE;


	setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
	strcpy(&TextG.text[0], "");
	TextG.multiline = 1;
	redcr(0);
	Cur_offd(X, Y);
re_edit:
	re_EDIT = FALSE;

	edit_params = TextG.bold * 2 + TextG.italics * 4 + TextG.underline * 8 + TextG.justowanie * 16 + TextG.ukryty * 64;
	preview_blocked = TRUE;
#ifndef LINUX
	////FreeMouse();
#endif
	ret = EditText(&TextG.text[0], edit_params, mynCmdShow, &single, &tab);
#ifndef LINUX
	////LockMouse();
#endif
	preview_blocked = FALSE;
	if (!ret)
	{
	   redcr(1);
	   edit_functions = FALSE;
	   return;
    }
	TextG.bold = (BOOL)(ret & 2)/2;
	TextG.italics = (BOOL)(ret & 4)/4;
	TextG.underline = (BOOL)(ret & 8)/8;
	TextG.justowanie = (int)((ret & 48)/16);
    TextG.ukryty = (BOOL)((ret & 64) / 64);

	Add_String_To_List(&TextG.text[0]);

	komunikat0(121);
	LengthT = strlen(TextG.text);
	TextG.x = X; TextG.y = Y; TextG.dl = LengthT;
	TextG.n = T18 + TextG.dl;

	TextG.width = 0;
	TextG.height = 0;
	TextG.obiektt2 = O2NoBlockS;

	DF_to_String(sk, "%-6.2f", Rad_to_Grid(TextG.kat), 6);
	menu_par_new((*mTekstm.pola)[1].txt, sk);

	set_menu_string();
	Cur_ond(X, Y);
	sel.akt = 1;

	while (1)
	{
		view_font_name(&TextG);
		sel.akt = 1;
		ev = Get_Event_Point(NULL, &X0, &Y0);
		switch (ev->What)
		{
		case evKeyDown: if (ev->Number == 0)
		{
			Cur_offd(X, Y);
			redcr(1);
			edit_functions = FALSE;
			return;
		}
						if (ev->Number == ENTER)
						{
							Cur_offd(X, Y);
							if ((PTRS__Text_Style[TextG.czcionka]->type == 2))
							{
								;
							}
							if (!multipisz())
							{
								redcr(1);
								edit_functions = FALSE;
								return;
							}

							Cur_ond(X, Y);
						}
						if ((ev->Number == 69) || (ev->Number == 101))
						{
							Cur_offd(X, Y);
							goto re_edit;
						}
						break;
		case evCommandP: (*COMNDmt[ev->Number]) ();
			if (ev->Number == 0) re_EDIT = TRUE;
			break;
		default: break;
		}
		if (re_EDIT == TRUE)
		{
			Cur_offd(X, Y);
			goto re_edit;
		}
	}
	edit_functions = FALSE;
}

int Tekst_factory(char *prefix, BOOL repeat)
{
	char sk[MaxTextLen] = "";
	int LengthT;
	EVENT *ev;
	double X0, Y0;
	BOOL re_EDIT = FALSE;
	char* ptr_n;
    int edit_params;
    int ret;
	int tab = 0;

    if (aktmakro!=NULL) return 0; //TEMPORARY due to makro

	edit_functions = TRUE;
	setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
	strcpy(&TextG.text, prefix);
	TextG.multiline = 0;

	redcr(0);
	//Cur_offd(X, Y);
    CUR_OFF(X, Y);
re_edit:

	re_EDIT = FALSE;

    if (text_edit_dialog==FALSE) ret=get_string(&TextG.text[0], "", MaxTextLen*2, 0, 13);
    else ret=get_dialog_string(&TextG.text[0], "", MaxTextLen*2, 0, 13, &tab);

	ptr_n = strstr(TextG.text, "\\n");
	while ((ptr_n != NULL) && (*ptr_n != 0))
	{
		*(ptr_n) = '\r';
		*(ptr_n + 1) = '\n';
		ptr_n += 2;
		ptr_n = strstr(ptr_n, "\\n");
		TextG.multiline = 1;
	}


    if (!ret)
    {
        redcr(1);
        edit_functions = FALSE;
        return 0;
    }

	komunikat0(121);
	LengthT = strlen(TextG.text);
	TextG.x = X; TextG.y = Y; TextG.dl = LengthT;
	TextG.n = T18 + TextG.dl;
	TextG.width = 0;
	TextG.height = 0;
	TextG.obiektt2 = O2NoBlockS;
	///////////
    CUR_OFF(X, Y);


	DF_to_String(sk, "%-6.2f", Rad_to_Grid(TextG.kat), 6);
	menu_par_new((*mTekstm.pola)[1].txt, sk);

	set_menu_string();

	//Cur_ond(X, Y);
    CUR_ON(X, Y);

	sel.akt=1;

	while (1)
	{
		view_font_name(&TextG);
		sel.akt=1;
		ev = Get_Event_Point(NULL, &X0, &Y0);
		switch (ev->What)
		{
		case evKeyDown: if (ev->Number == 0)
		{
			//Cur_offd(X, Y);
            CUR_OFF(X, Y);
			redcr(1);
			edit_functions = FALSE;
			return 0;
		}
						if (ev->Number == ENTER)
						{
							//Cur_offd(X, Y);
                            CUR_OFF(X, Y);
							if ((PTRS__Text_Style[TextG.czcionka]->type == 2))
							{
								;
							}
							if (!pisz(repeat))
							{
								redcr(1);
								edit_functions = FALSE;
								return 1;
							}
							//Cur_ond(X, Y);
                            CUR_ON(X, Y);
						}
						if ((ev->Number == _EDIT_) || (ev->Number == _edit_))
						{
							//Cur_offd(X, Y);
                            CUR_OFF(X, Y);
							goto re_edit;
						}
						break;
  		case evCommandP: (*COMNDmt[ev->Number]) ();
			if (ev->Number == 0) re_EDIT = TRUE;
			break;
		default: break;
		}
		if (re_EDIT == TRUE)
		{
			//Cur_offd(X, Y);
            CUR_OFF(X, Y);
			goto re_edit;
		}
	}
	edit_functions = FALSE;
    return 1;
}


void Tekst(void)
{
    int ret=Tekst_factory("", TRUE);
}


void Tekst____(BOOL repeat)
{
  char sk [MaxTextLen]="";

  int LengthT ;
  EVENT *ev ;
  double X0, Y0 ;
  BOOL re_EDIT = FALSE;

  edit_functions = TRUE;
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  strcpy(&TextG.text[0],"");
  TextG.multiline = 0;

  redcr (0) ;

re_edit:


  re_EDIT = FALSE;
  
  if (!get_string (&TextG.text[0], "", MaxTextLen, 0, 13))
  {
	Cur_offd(X, Y);
    redcr (1) ;
	edit_functions = FALSE;
    return ;
  }
  
  komunikat0(121);
  LengthT=strlen(TextG.text);
  TextG.x=X; TextG.y=Y; TextG.dl=LengthT;
  TextG.n=T18+TextG.dl;
  TextG.width = 0;
  TextG.height = 0;
  //TextG.kat=0;
  TextG.obiektt2 = O2NoBlockS;
///////////
  

  DF_to_String (sk, "%-6.2f", Rad_to_Grid (TextG.kat), 6) ;
  menu_par_new ((*mTekstm.pola) [1].txt, sk) ;

  set_menu_string () ;

  while(1)
   {
     view_font_name(&TextG);

     ev=Get_Event_Point(NULL, &X0, &Y0);
     switch(ev->What)
      { case evKeyDown  : if (ev->Number == 0)
			  {

			     redcr (1) ;
				 edit_functions = FALSE;
			     return ;
			   }
			  if (ev->Number == ENTER)
			  {

  			    if (!pisz (repeat))
			      {
			        redcr (1) ;
					edit_functions = FALSE;
			        return ;
			      }
			    Cur_ond (X, Y) ;
			  }
			  if ((ev->Number == 69) || (ev->Number == 101))
			  {
			   Cur_offd (X, Y) ;
			   goto re_edit;
			  }
			  break;
	case evCommandP : (*COMNDmt[ev->Number]) () ;
		      if (ev->Number==0) re_EDIT=TRUE;
			  break ;
	default         : break ;
      }
	 if (re_EDIT == TRUE)
	 {

		 redcr(1);
		 goto re_edit;
	 }
   }
  edit_functions = FALSE;
}

/*---Edit_Text-------------------------------------------*/

static ESTR eT;
static BOOL strwyj;

void reset_strwyj(void)
{
    strwyj=FALSE;
}

void set_strwyj(void)
{
    strwyj=TRUE;
}

BOOL get_strwyj(void)
{
    return strwyj;
}

static int change_text (BOOL b_graph_value)
/*--------------------------*/
{ TEXT Tp, *t;
  char* ptr_n;

  b_graph_value = b_graph_value ;
  memmove (&Tp, &TextG, sizeof(NAGLOWEK)+((TEXT *)ADP1)->n);
  strcpy (Tp.text, eT.st);
  Tp.dl = strlen(eT.st);
  Tp.n = T18 + Tp.dl;
  Tp.width = 0;
  Tp.height = 0;

  ptr_n = strstr(Tp.text, "\\n");
  while ((ptr_n != NULL) && (*ptr_n != 0))
  {
	  *(ptr_n) = '\r';
	  *(ptr_n + 1) = '\n';
	  ptr_n += 2;
	  ptr_n = strstr(ptr_n, "\\n");
	  Tp.multiline = 1;
  }

  if ( (t = korekta_obiekt((void *)ADP1, (void *)&Tp)) == NULL) return 0;
  Set_Second_Screen();
  okno_r_second();
  outtextxy_w (t, COPY_PUT);
  okno_all_second();
  Set_Screen();
  Cur_offd (X, Y) ;
  strwyj = 1;
  return 1;
}

static void redraw_multitext(void)
{
	((NAGLOWEK*)ADP1)->atrybut = Anormalny;
	Set_Second_Screen();
	okno_r_second();
	outtextxy_w((TEXT*)ADP1, COPY_PUT);
	okno_all_second();
	Set_Screen();
	Cur_offd(X, Y);
	strwyj = 1;
}

static int change_multitext(BOOL b_graph_value, int new_params)
/*------------------------------------------------------------*/
{
	TEXT Tp, *t;

	b_graph_value = b_graph_value;
	TextG.atrybut = Anormalny;
	memmove(&Tp, &TextG, sizeof(NAGLOWEK) + ((TEXT *)ADP1)->n);
	strcpy(Tp.text, eT.st);
	Tp.dl = strlen(eT.st);
	Tp.justowanie = (int)((new_params &48)/16);
	Tp.bold = (BOOL)((new_params & 2) / 2);
	Tp.italics = (BOOL)((new_params & 4) / 4);
	Tp.underline = (BOOL)((new_params & 8) / 8);
	Tp.n = T18 + Tp.dl;
	Tp.width = 0;
	Tp.height = 0;
	if ((t = korekta_obiekt((void *)ADP1, (void *)&Tp)) == NULL) return 0;
    Set_Second_Screen();
    okno_r_second();
    outtextxy_w (t, COPY_PUT);
    okno_all_second();
    Set_Screen();
    Cur_offd (X, Y) ;
	strwyj = 1;
	return 1;
}

static int get_typ_tekstu(TEXT *t)
{
  return t->typ;
}

static int get_text_in_block(TEXT *t)
{
  return t->blok;
}


static unsigned int get_normal_special_text(TEXT *t)
{
  return t->obiektt3;
}

static void redcrET(char typ, TEXT  *t, int opcja_bak, unsigned char multiline)
/*---------------------------------------------------------------------------*/
{ static void (*CUR_N)(double ,double);
  static void (*CUR_F)(double ,double);
  static int ( *SW[4])();
  TEXT  * t_change;
  static int np;
  static int sel_akt, sel_gor, sel_cur;
  static TEXT_NAG t_nag;
  static int komunikat_R,komunikat_R0;
  int width_w;
  char* ptr_n;

  ADP=ADK=ADP1=(char *)t;

  switch (typ)
  {
  case 0:
	  mvcurb.T = 1;
	  CUR_F = CUR_OFF;  CUR_OFF = cur_off;
	  CUR_N = CUR_ON;   CUR_ON = cur_on;
	  SW[0] = SERV[73];  SERV[73] = kat_tt;  //PdUp
	  SW[1] = SERV[81];  SERV[81] = kat_dt;  //PgDn
	  SW[2] = SERV[71];  SERV[71] = kat_t90;  //Home  kat 0  t0
	  SW[3] = SERV[79];  SERV[79] = kat_t0;  //End   kat 90  t90
	  komunikat_R = Komunikat_R;
	  komunikat_R0 = Komunikat_R0;
	  komunikat(0);
	  komunikat0(0);
	  BlokM = 1;
	  memmove(&t_nag, &TextG, sizeof(TEXT_NAG));
	  memmove(&TextG, t, sizeof(NAGLOWEK) + t->n);
	  set_menu_string();

	  t->atrybut = Ablok;

      Set_Second_Screen();
	  setcolor(kolory.paper);
      okno_r_second();
	  outtextxy_w_(t, COPY_PUT);
      okno_all_second();
      Set_Screen();
      ////Cur_offd(X, Y);

	  eT.x = 1;
	  eT.y = ESTR_Y;
	  eT.val_no_max = 0;

      eT.lmax = (float)(getmaxx() - 27*WIDTH - ttf_width_w) / ttf_width_w; // width_w;

	  eT.mode = GV_STRING;
	  eT.format = "%s";
	  eT.ESTRF = change_text;
	  eT.extend = 1;
	  if (t->multiline == 0)
	  {
		if (-1 != (np = dodajstr(&eT)))
		{
			strcpy(eT.st, t->text);
			Out_Edited_Draw_Param((ESTR *)&eT, TRUE);
		}
		
	  }
	
      strwyj = FALSE;
      sel_akt=sel.akt;
      sel_gor = sel.gor;
      sel_cur=sel.cur;

      menupini(&mTekstm, _TEXT_, _TEXT_C_, 16);

      if ((t->x < Xp) || (t->x > Xp+X2d) || (t->y < Yp) || (t->y > Yp+Y2d))
	mvcurp (t->x - X, t->y - Y);
      else
      Cur_ond ( X = t->x , Y = t->y );  
      pozycja_kursora();

	  if (PTRS__Text_Style[t->czcionka]->type == 2)
      {
          redraw();
#ifdef ALLEGRO5
          flip_screen();
#endif
      }
      break;
    case 1 :

      CUR_OFF=CUR_F;
      CUR_ON=CUR_N;
      SERV[73]=SW[0]; 
      SERV[81]=SW[1];
      SERV[71]=SW[2];
      SERV[79]=SW[3];
      BlokM = 0;

      sel.akt=sel_akt;
      sel.gor = sel_gor;
      sel.cur=sel_cur;
      menupini(NULL,"",' ', 0);
	  if (multiline == 0) usunstr(np);		/*usunstr musi byc wywolane przed komunikat0 */
      komunikat (komunikat_R) ;
      komunikat0 (komunikat_R0) ;
      Set_Second_Screen();
      okno_r_second();
      outtextxy_w(t,COPY_PUT);
      okno_all_second();
      Set_Screen();
      Cur_offd (X, Y) ;
      t->atrybut = Anormalny;
      if (opcja_bak==1) memmove (&TextG, &t_nag, sizeof(TEXT_NAG));
      break;
    case 2 :
      ADP=ADP1;
      ADK=ADP1;
      BlokM = 0;

      menupini(NULL,"",' ', 0);
	  if (multiline == 0) usunstr(np);
      komunikat (komunikat_R) ;
      komunikat0 (komunikat_R0) ;
	  TextG.width = 0;
	  TextG.height = 0;

	  ptr_n = strstr(TextG.text, "\\n");
	  while ((ptr_n != NULL) && (*ptr_n != 0))
	  {
		  *(ptr_n) = '\r';
		  *(ptr_n + 1) = '\n';
		  ptr_n += 2;
		  ptr_n = strstr(ptr_n, "\\n");
		  TextG.multiline = 1;
	  }

      if (strwyj == 0)
      {

	    if ( (t_change = korekta_obiekt((void *) ADP1, (void *)&TextG)) == NULL)
  	    ErrList (15);
        Set_Second_Screen();
        okno_r_second();
	    outtextxy_w (t_change, COPY_PUT);
        okno_all_second();

        Set_Screen();
        Cur_offd (X, Y) ;
      }
      else
      {

         ;
      }
	  TextG.multiline = 0;
      if (opcja_bak==1) memmove (&TextG, &t_nag, sizeof(TEXT_NAG));
      CUR_OFF=CUR_F;
      CUR_ON=CUR_N;
      SERV[73]=SW[0];
      SERV[81]=SW[1];
      SERV[71]=SW[2];
      SERV[79]=SW[3];

      break;
    default :
      break;
  }
}

static BOOL zapisz_do(void)
/*-------------------------*/
{
  char sk[MAXPATH]="";
  BOOL ret = FALSE;

  cursor_off();

  if (Load_File (sk, SAVE_FILE_AS, FALSE) == FALSE)
  {

     return FALSE;
  }
  if (pisz_zbior(sk, FALSE, 0) == 0)  /*brak bledu*/
  {
     ret = TRUE;

  }

  if (ret == TRUE)
  {
    Change = FALSE;
  }
  return ret;
}



static BOOL uaktualnij(void)
/*----------------------------*/
{
  BOOL ret = FALSE;

  if(!strlen(zbior_danych))
  {
    ret = zapisz_do();
  }
  else
  {
    cursor_off();
    if (0 == pisz_zbior(zbior_danych, FALSE, 0))
    {
      ret = TRUE;
    }
    cursor_on();
  }
  if (ret == TRUE)
  {
    Change = FALSE;
  }
  return ret;
}


BOOL Wejscie_Text(void *ad)
/*------------------------*/
{
 EVENT *ev;
  double X0, Y0;
  int font_no;
  TEXT *ad_text;
  int typ_edyt_tekst;
  char sk[MaxTextLen] = "";
  char sk_f[MaxTextLen] = "", unicodetext[MaxTextLen*2]="", utf8text[MaxTextLen * 2] = "";;
  int key;
  char *ptr;
  unsigned char multiline;
  int count;

  multiline = ((TEXT*)ad)->multiline;
  redcrET(0, (TEXT*)ad, 1, multiline);
  view_font_name((TEXT*)ad);
  typ_edyt_tekst = get_typ_tekstu((TEXT*)ad);
  if (typ_edyt_tekst==n_typ_plik)
  {

   redcrET(1, (TEXT*)ad, 1, multiline);
   /*************/
   if (Change)
   {
    komunikat (3);

    key = Get_Legal_Key (_YES_NO_ESC_);

    komunikat (0);
    if (key == ESC) return FALSE;

    if (key == _YES_ || key == _yes_)
    {
      uaktualnij();
    }
   }
  ad_text=ad; 
  /*sprawdzenie czy nie jest to pelna sciezka*/
  if ((ad_text->text[1]!='/') && (NULL == (ptr = strchr (ad_text->text, ':'))))
   {
    strcpy(sk,Current_File_Directory);
	strncpy(utf8text, ad_text->text, MaxTextLen-1);
	utf8text[MaxTextLen - 1] = '\0';
	count=utf82unicode(&utf8text, &unicodetext);
	unicode2win(&unicodetext, &sk_f, count);

    strcat(sk, sk_f);
   }
  else
  {
	  strncpy(sk, ad_text->text, MaxTextLen - 1);
	  sk[MaxTextLen - 1] = '\0';
	  count = utf82unicode(&sk, &unicodetext);
	  unicode2win(&unicodetext, &sk, count);
  }
  strcpy(zbior_danych_2,""); /*kasowanie rysunku dolaczonego*/

  if (bitmap_pattern_exist) DonePatternBitmapScaled();

  zwolnienie_pamieci();

  bitmap_view=TRUE;

  /*katalog biezacy*/
  strcpy(zbior_danych,sk);
  Clear_View () ;
  czytaj_rysunek(sk, TRUE);
  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  Set_Auto_Backup (FALSE);
  /*dopisanie nazwy zbiory do tablicy hierarchicznej*/
  /*jezeli numer zbioru hierarchii == 255 to pamietany jest ostatni*/
   if (numer_zbioru_hierarchii<255)
    {
    numer_zbioru_hierarchii += 1;
    strcpy(nazwa_zbioru_hierarchii [numer_zbioru_hierarchii], sk);
    }
    else
     {
      strcpy(nazwa_zbioru_hierarchii [255], sk);
     }
   /*************/
   return TRUE;
  }
  else
   {
   redcrET(1, (TEXT*)ad, 1, multiline);
   komunikat(86);   
   return FALSE;
   }
}

#ifndef aster

static double Px, Py,DX,DY;

static BOOL add_block (double x, double y, char *blok_type0)
/*--------------------------------------------------------*/
{
  unsigned size_block = B3 ; 
  BLOK s_blockd = Bdef;
#ifndef LINUX
  BLOKD blokd= BDdef ;
#else
    BLOK blokd= BDdef ;
#endif
  BLOK *ptrs_block, *buf_block ;
  int  len_type, len_desc ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  char blok_type[31];


  strcpy(blok_type,"$");
  strcat(blok_type,blok_type0);
  len_type = strlen (blok_type) + 1 ;
  len_desc = sizeof(unsigned) + 2 * sizeof (float) + sizeof(len_type) + len_type ;
  size_block += len_desc ;
  
  if (NULL == (buf_block = (BLOK*) malloc (sizeof(NAGLOWEK) + size_block)))
  {
    return FALSE;
  }
  memcpy (buf_block, &blokd, sizeof (blokd)) ;
  buf_block->n = size_block ;
  buf_block->kod_obiektu = 'E' ;
  buf_block->dlugosc_opisu_obiektu = len_desc ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&buf_block->opis_obiektu [0]) ;
  ptrs_desc_bl->flags = EBF_IP ;
  ptrs_desc_bl->x = x ;
  ptrs_desc_bl->y = y ;
  ptrs_desc_bl->len = len_type ;
  strcpy (&ptrs_desc_bl->sz_type [0], blok_type) ;
  if ((ptrs_block = (BLOK*)dodaj_obiekt (NULL, buf_block)) == NULL)
  {
	  free(buf_block);
    return FALSE;
  }
  free(buf_block);
  return TRUE ;
}

static void  cur_offPB(double x,double y)
{ out_blok1(DX,DY,0,0,Tprzesuw,0);
  cursel_off(x,y);
}

static void  cur_onPB(double x,double y)
{ cursel_on(x,y);
  DX=x-Px;  DY=y-Py;
  out_blok1(DX,DY,0,0,Tprzesuw,0);
  cursel_on(x,y);
}


static void redcrPB(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
 if (typ==0)
   { BlokM=1;
     CUR_oFF=CUR_OFF;    CUR_OFF=cur_offPB;
     CUR_oN=CUR_ON;      CUR_ON=cur_onPB;
     komunikat0(37);
     Cur_ond(X,Y);
   }
  else
  { Cur_offd(X,Y);
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;
    BlokM=0;
    komunikat0(0);
  }
}


static int PrzesunB(double X0, double Y0)
{
  EVENT *ev;

  Px=X0;Py=Y0;
  DX=X-X0; DY=Y-Y0;
  sel.akt=1;
  redcrPB(0);
  while(1)
   { ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0) { redcrPB(1); return ESC; }
     if (ev->What == evKeyDown &&  ev->Number== ENTER)
      { redcrPB(1);
	return ENTER;
      }
   }
}

static int Ins_atrybut(TEXT *Td)           //atrybut
{ char tekst_p[60];
  char tekst_p1[60];
  int i;
  int l_kr;
  int LengthT;
  TEXT TextGa=Tdef;
  TEXT Tdd;
  double angle_l, sina, cosa;
  double dy_typ = 3;
  double dy_komentarz = 3;
  char typ_bloku [30] = "";
  double xx, yy;
  FILE *stru;
  char 	buf [256];
  BOOL first_row;
  int k;

  strcpy(typ_bloku, "ATRYBUT");
  xx=0;
  yy=0;

  memmove(&Tdd, Td, sizeof(NAGLOWEK) + Td->n);

  stru = fopen ( "ATRYBUT.$1", "rt" ) ;
  if ( stru == NULL )
   {
     return 0;
   }

  first_row=FALSE;

  while ( !feof(stru))
   {
	 if (fgets( buf, 256 , stru ) == NULL ) goto error_stru;
    if (buf[strlen(buf)-1]=='\012' )
        buf[strlen(buf)-1]='\0';

    if (first_row==FALSE)
     {
     first_row=TRUE;

      if (FALSE == add_block (xx, yy, typ_bloku))
       {
        return 0;
       }

    //attribute symbol
    TextGa.kat=Tdd.kat;
    TextGa.wysokosc=Tdd.wysokosc;
    TextGa.italics=Tdd.italics;
    TextGa.width_factor=Tdd.width_factor;
    TextGa.warstwa=Tdd.warstwa;
    TextGa.kolor=Tdd.kolor;
    TextGa.czcionka=Tdd.czcionka;
    TextGa.typ=n_typ_atrybut;
    TextGa.justowanie=Tdd.justowanie;
    TextGa.ukryty=0;
    TextGa.obiektt1 = 0;
    TextGa.obiektt2 = O2BlockSpecial;
    TextGa.blok=1;

    TextGa.x=0;
    TextGa.y=0;
  
    strcpy(tekst_p,buf);
    strcpy(&TextGa.text[0],tekst_p);
    LengthT=strlen(TextGa.text);
    TextGa.dl=LengthT;
    TextGa.n=T18+TextGa.dl;

    if (dodaj_obiekt((BLOK*)dane, (void*)&TextGa)==NULL) return 0;
        rysuj_obiekt(&TextGa, COPY_PUT, 1);
   }
   else
    {
     //next row
     TextGa.kat= 0;
     TextGa.wysokosc=2;
     TextGa.italics=0;
     TextGa.width_factor=1;
     TextGa.warstwa=Tdd.warstwa;
     TextGa.kolor=7; //40;
     TextGa.czcionka=0;
     TextGa.typ=n_typ_komentarz;
     TextGa.justowanie=j_do_lewej;
     TextGa.ukryty=1;
     TextGa.obiektt1 = 0;
     TextGa.obiektt2 = O2BlockSpecial;
     TextGa.blok=1;

     yy-=2.5;
     TextGa.x=0;
     TextGa.y=yy;
  
     strcpy(tekst_p,buf);
     strcpy(&TextGa.text[0],tekst_p);
     LengthT=strlen(TextGa.text);
     TextGa.dl=LengthT;
     TextGa.n=T18+TextGa.dl;
  
     if (dodaj_obiekt((BLOK*)dane, (void*)&TextGa)==NULL) return 0;
        rysuj_obiekt(&TextGa, COPY_PUT, 1);
    }
   }
error_stru:
  k=fclose(stru);
  return 1;
}


void Wstaw_Atrybut(TEXT *Td, int opcja, double x_b, double y_b)
{ int n;
  int l_kr;
  long ds;
  int dynamic_block_x;
  char symbol_ap[30];
  char typ_ap[30];
  char komentarz_ap[30];
  int punkt = 3;

  //generowanie atrybutu
  ds=dane_size;
  Ins_atrybut(Td);

  
  ADP=dane; ADK=dane+(dane_size-ds)-1;
  CUR_OFF(X,Y);
  zmien_atrybut(ADP,ADK,Anormalny,Ablok);
     // z przesuwaniem
  if (opcja==0)
   {
   if(PrzesunB(0,0)==ESC)
    { zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
	  usun_blok(ADP,ADK);
	  CUR_ON(X,Y);
	  return;
    }
     else
	{
	 transformacja_blok(ADP,ADK,X-0,Y-0,0,0,Tprzesuw,0);
	}
  }
   else transformacja_blok(ADP,ADK,x_b,y_b,0,0,Tprzesuw,0);

     blokzap(ADP,ADK,Ablok,COPY_PUT,1);
     zmien_atrybut(dane, dane + dane_size, Aoblok, Anormalny);
     zmien_atrybut(ADP,ADK,Ablok,Aoblok);
     CUR_ON(X,Y);
}
#endif


int GoEditText(void)
{   
	int ret, ret1;
    int single=0;
	int tab = 0;
	preview_blocked = TRUE;
#ifndef LINUX
	////FreeMouse();
#endif
	TTF_redraw = FALSE;
	ret = EditText(&GMultiText, GMultiParams, mynCmdShow, &single, &tab);
#ifndef LINUX
	////LockMouse();
#endif
	preview_blocked = FALSE;
	if (!ret)
	{
		redraw_multitext();
		return 0;

	}
	else
	{
		Add_String_To_List(&GMultiText);

		eT.st=GMultiText;
		ret1=change_multitext(0, ret);
		if (TTF_redraw) redraw();
		return ret1;
	}
}

void TEXT_TEST(void)
{
	TEXT T = Tdef;
	T.warstwa = 0;
	T.czcionka = 0;
	T.wysokosc = 4.0;
	T.kat = 0.0;
	T.justowanie = j_do_lewej;
	T.typ = n_typ_normal;
	T.x = 100;
	T.y = 100;
	T.bold = 0;
	T.obiektt2 = O2NoBlockS;
	strcpy(T.text, "TEST");
	T.dl = strlen(T.text);
	T.n = T18 + T.dl;
	T.width = 0.0;
	T.height = 0.0;
	outtextxy_w_(&T, XOR_PUT);

}

void  Edit_Text(void  *ad)
/*----------------------*/
{
  EVENT *ev, ev0;
  BLOK *ptrs_block, *ptrs_block1, *ptrs_block2 ;
  LINIA *L1;
  double X0, Y0;
  int font_no;
  TEXT *ad_text, *T, *T1, *t, Tp;
  T_Point *P, *P1, *P2;
  int typ_edyt_tekst;
  int special_text;
  int text_in_block;
  double krok_s0;
  double xt,yt;
  int snap_0;
  char *ad1, *ad2, *adp1, *adp2, *adp3, *adp4, *adp5, *adp6, *adp7, *adp8;
  double wsp_sk, orig_x, orig_y;
  double old_var, old_var1, new_var, new_var1, del_var;
  char o_text[MaxTextLen];
  double skala_profilu;
  char *bp, *bp1, *bp2;
  int typ_tekstu, typ_punktu1, typ_punktu2;
  double srednica, zaglebienie;
  BOOL was_changes;
  char *srednica_t;
  char srednica_tt[60];
  double srednica_s;
  char *zaglebienie_t;
  char zaglebienie_tt[60];
  double zaglebienie_s;
  double poziom_z;
  int l_kr;
  double Y_os;
  unsigned char multiline;
  int(*MEDITFUN_BAK)(void) = MEDITFUN;
  static int sel_akt, sel_cur;
  
  ad_text=ad;
  multiline = ad_text->multiline;

  CUR_OFF(X, Y);
  sel_akt = sel.akt;
  sel.akt = 1;
  CUR_ON(X, Y);

  redcrET(0, (TEXT*)ad, 1, multiline);
  
  Break_Edit_Text=FALSE;
  
  xt=ad_text->x;
  yt=ad_text->y;
  
  view_font_name((TEXT*)ad);
  typ_edyt_tekst = get_typ_tekstu((TEXT*)ad);
  special_text = get_normal_special_text((TEXT*)ad);
  text_in_block = get_text_in_block((TEXT*)ad);

  if (multiline == 1)
  {
	  strcpy(&GMultiText, ad_text->text);
	  GMultiParams = ad_text->bold * 2 + ad_text->italics * 4 + ad_text->underline * 8 + ad_text->justowanie * 16 + ad_text->ukryty * 64;
	  MEDITFUN = GoEditText;
	  edit_functions = TRUE;
	  komunikat0(121);
  }

  if (((typ_edyt_tekst==n_typ_pin_name) || 
      (typ_edyt_tekst== n_typ_port) ||
      (typ_edyt_tekst== n_typ_zasilanie) ||
      (typ_edyt_tekst== n_typ_zmienna) ||
      (typ_edyt_tekst== n_typ_symbol_drabinki) ||
      (typ_edyt_tekst== n_typ_opis_drabinki)) && (swobodny_tekst==FALSE))

  {

   if (edycjastr()==0)
    {
      redcrET(2, (TEXT*)ad, 1, multiline);
	  MEDITFUN = MEDITFUN_BAK;
	  edit_functions = FALSE;
      return;
    }

     redcrET(2, (TEXT*)ad, 1, multiline);
	 MEDITFUN = MEDITFUN_BAK;
	 edit_functions = FALSE;
   return;
  }
  else
  {
  krok_s0 = krok_s;
  snap_0 = snap_;
  
  if ((typ_edyt_tekst== n_typ_symbol) ||
     (typ_edyt_tekst== n_typ_typ) ||
     (typ_edyt_tekst== n_typ_komentarz) || 
     (typ_edyt_tekst== n_typ_zasilanie) ||
     (typ_edyt_tekst== n_typ_port) ||
     (typ_edyt_tekst== n_typ_pin_name) ||
     (typ_edyt_tekst== n_typ_zasilanie) ||
     (typ_edyt_tekst== n_typ_symbol_drabinki) ||
     (typ_edyt_tekst== n_typ_opis_drabinki))
     {
      kasowanie_licznikow=TRUE;
     }
  CUR_ON(X,Y);
  while(1)
   {
     ev=Get_Event_Point(NULL, &X0, &Y0);
	  
     if (ev->What == evKeyDown && ev->Number == 0)
      {
  	    redcrET(1, (TEXT*)ad, 1, multiline);
		TTF_redraw = FALSE; //it was set to TRUE in XOR mode
	    krok_s = krok_s0;
	    snap_ = snap_0;
	    kasowanie_licznikow=TRUE;
		MEDITFUN = MEDITFUN_BAK;
		edit_functions = FALSE;
		sel.akt = sel_akt;
	    return;
      }
     if ((ev->What == evKeyDown && ev->Number == ENTER) || strwyj == TRUE)
      {
 
        redcrET(2, (TEXT*)ad, 1, multiline);
	    krok_s = krok_s0;
	    snap_ = snap_0;
	    kasowanie_licznikow=TRUE;
		MEDITFUN = MEDITFUN_BAK;
		edit_functions = FALSE;
		sel.akt = sel_akt;
	    return;
      }
      if (ev->What == evCommandP)
       {
	     (*COMNDmt[ev->Number])();
		 if (ev->Number == 0)
			 if (multiline) simulate_ukeypress(_EDIT_, _EDIT_SC_);
			 else simulate_keypress(9);
       }
   }
  } 
  krok_s = krok_s0;
  snap_ = snap_0;
  kasowanie_licznikow=TRUE;
  MEDITFUN = MEDITFUN_BAK;
  edit_functions = FALSE;
  sel.akt = sel_akt;
}

void  edit_text_tab_f2(void  *ad)
/*-----------------------------*/
{
  EVENT *ev;
  BLOK *ptrs_block, *ptrs_block1, *ptrs_block2 ;
  LINIA *L1;
  double X0, Y0;
  int font_no;
  TEXT Tp, *T, *T1;
  TEXT *ad_text;
  T_Point *P, *P1, *P2;
  int typ_edyt_tekst;
  int special_text;
  int text_in_block;
  double krok_s0;
  double xt,yt;
  int snap_0;
  char *ad1, *ad2, *adp1, *adp2, *adp3, *adp4, *adp5, *adp6, *adp7, *adp8;
  double wsp_sk, orig_x, orig_y;
  double old_var, old_var1, new_var, new_var1, del_var;
  char o_text[MaxTextLen];
  double skala_profilu;
  char *bp, *bp1, *bp2;
  int typ_tekstu, typ_punktu1, typ_punktu2;
  double srednica, zaglebienie;
  BOOL was_changes;
  char *srednica_t;
  char srednica_tt[60];
  double srednica_s;
  char *zaglebienie_t;
  char zaglebienie_tt[60];
  double zaglebienie_s;
  double poziom_z;
  int l_kr;
  double Y_os;
  int ret;
  unsigned char multiline;

  ad_text=ad;
  multiline = ad_text->multiline;
  redcrET(0, (TEXT*)ad, 1, multiline);

  xt=ad_text->x;
  yt=ad_text->y;
  
  view_font_name((TEXT*)ad);
  typ_edyt_tekst = get_typ_tekstu((TEXT*)ad);
  special_text = get_normal_special_text((TEXT*)ad);
  text_in_block = get_text_in_block((TEXT*)ad);


  if (ad_text->multiline)
  {
	  memmove(&TextG, ad_text, sizeof(NAGLOWEK) + ad_text->n);
	  strcpy(&GMultiText, ad_text->text);
	  GMultiParams = ad_text->bold * 2 + ad_text->italics * 4 + ad_text->underline * 8 + ad_text->justowanie * 16  + ad_text->ukryty * 64;
	  edit_functions = TRUE;
	  komunikat0(39);
	  ret=GoEditText();
  }
  else
   if (edycjastr()==0)
    {
      redcrET(2, (TEXT*)ad, 1, multiline);
      return;
    }
    redcrET(2, (TEXT*)ad, 1, multiline);
	
   return;

}

void  edit_text_f3(void  *ad)
/*-----------------------------*/
{
  EVENT *ev;
  double X0, Y0;
  int font_no;
  TEXT *ad_text;
  int typ_edyt_tekst;
  double krok_s0;
  double xt,yt;
  int snap_0;
  int(*MEDITFUN_BAK)(void) = MEDITFUN;
  int n_text;
  int n_line;
  int n_text_nag;
  int n_nag;
  int n_nag_ext;
  unsigned char multiline;
  static int sel_akt, sel_cur;

  n_text = sizeof(TEXT);
  n_line = sizeof(LINIA);
  n_nag = sizeof(NAGLOWEK);
  n_nag_ext = sizeof(NAGLOWEK_EXT);
  n_text_nag = sizeof(TEXT_NAG);

  ad_text = ad;
  multiline = ad_text->multiline;

  CUR_OFF(X, Y);
  sel_akt = sel.akt;
  sel.akt = 1;
  CUR_ON(X, Y);

  redcrET(0, (TEXT*)ad, 1, multiline);
  
  if (ad_text->multiline)
  {
	  strcpy(&GMultiText, ad_text->text);
	  GMultiParams = ad_text->bold * 2 + ad_text->italics * 4 + ad_text->underline * 8 + ad_text->justowanie * 16 + ad_text->ukryty * 64;
	  MEDITFUN = GoEditText;
	  edit_functions = TRUE;
	  komunikat0(121);
  }
  
  xt=ad_text->x;
  yt=ad_text->y;
  
  view_font_name((TEXT*)ad);
  typ_edyt_tekst = get_typ_tekstu((TEXT*)ad);

  krok_s0 = krok_s;
  snap_0 = snap_;
  if ((typ_edyt_tekst== n_typ_symbol) ||
     (typ_edyt_tekst== n_typ_typ) ||
     (typ_edyt_tekst== n_typ_komentarz) ||
     (typ_edyt_tekst== n_typ_zasilanie) ||
     (typ_edyt_tekst== n_typ_port) ||
     (typ_edyt_tekst== n_typ_pin_name) ||
     (typ_edyt_tekst== n_typ_zasilanie) ||
     (typ_edyt_tekst== n_typ_symbol_drabinki) ||
     (typ_edyt_tekst== n_typ_opis_drabinki))
     {
      krok_s = jednostkiY(1);
      snap_ = 0;
      kasowanie_licznikow=TRUE;
     }
   CUR_ON(X,Y);
   while(1)
   { ev=Get_Event_Point(NULL, &X0, &Y0);
     if (ev->What == evKeyDown && ev->Number == 0)
      {
	    redcrET(1, (TEXT*)ad, 1, multiline);
	    krok_s = krok_s0;
	    snap_ = snap_0;
	    kasowanie_licznikow=TRUE;
		MEDITFUN = MEDITFUN_BAK;
		sel.akt = sel_akt;
	    return;
      }
     if ((ev->What == evKeyDown && ev->Number == ENTER) || strwyj == TRUE)
      {

	    redcrET(2, (TEXT*)ad, 1, multiline);
	    krok_s = krok_s0;
	    snap_ = snap_0;
	    kasowanie_licznikow=TRUE;
		MEDITFUN = MEDITFUN_BAK;
		sel.akt = sel_akt;
	    return;
      }
      if (ev->What == evCommandP)
       {
	    (*COMNDmt[ev->Number])();
		if (ev->Number == 0)
			if (multiline) simulate_keypress('E');
			else simulate_keypress(9);
       }
      }
  krok_s = krok_s0;
  snap_ = snap_0;
  kasowanie_licznikow=TRUE;
  MEDITFUN = MEDITFUN_BAK;
  sel.akt = sel_akt;
}


#undef __O_TEXT_B__