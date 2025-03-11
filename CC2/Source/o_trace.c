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

#define __O_TRACE__
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
#include "o_libfun.h"

#include "o_pline.h"

#include "menu.h"

#define YES _YES_
#define NO _NO_

extern double Grid_to_Rad (double angle) ;
extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;
extern int Get_Current_Color (void) ;
extern BOOL Check_if_Equal (double , double ) ;
extern BOOL Check_if_Equal2(double, double);
extern void Normalize_Solid (WIELOKAT *) ;
extern void orto_l(LINIA *L, int *Orto_Dir);
extern float solidpatternscales[7];
extern int solidpatternangles[4];
extern char solidpatterndxy[9][5];

extern void Wez_Trace_Pattern(void);
extern void Set_Trace_Pattern(void);
extern int Open_TracePatterns(void);
extern int przeciecieLL_tt (double *x, double *y, void *adr, void *adr1, double *t1, double *t2);
extern void set_np(int np0);

extern int Pline_Arc (double df_xbeg, double df_ybeg);
extern int Pline_Line (double df_xbeg, double df_ybeg);
extern int start_trace_arc (double df_xbeg, double df_ybeg, LINIA *s_strace_line, BOOL b_line, int d_line);
extern void Usun_Object (void *ad, BOOL b_simple);
extern void type_arc_pl (int ev_nr);

extern int pline_trace;
extern void set_pl(double x, double y);

extern void change_angle_l(double angle);
extern BOOL Get_reversed(void);
extern void Set_reversed(BOOL reversed);
extern BLOK *FIRSTB(char  *ado);
extern void blokzap_deep(char  *adp,char  *adk,int atrybut,int mode, int kolor);
extern void reset_trace_block(void);
extern void get_solidarc_ends(SOLIDARC *sa, double *xy);
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);
extern void remove_short_notice(void);

static BOOL check_head_trace(BLOK* ptrs_block);

static BOOL add_block(void);
int trace_command(int ev_Number, double X0, double Y0, int strwyj);

double trace_angle_orig=0.0;
BOOL trace_angle_changed=FALSE;
BOOL trace_orto_orig=FALSE;

static POINTD point1,point2;
static unsigned char b_ptr_before_last=0;
static WIELOKAT s_trace_solid;

enum DRAW_ARC_TYPE { ARC_P3 = 0, ARC_SCE, ARC_SCA, ARC_SCL, ARC_SER, ARC_SEA, ARC_SED, ARC_Con, ARC_rev,
IDM_CLOSE, IDM_UNDO, IDM_LINE, IDM_CONTINUOUS_LINE, IDM_DASHED_LINE, ARC_rev_Y, ARC_rev_N } ;

#define ID_WIDTH 0
#define ID_AXIS 1

#define ID_ERASE 4
#define ID_BREAK 5
#define ID_CLOSE 6
#define ID_MODE 7
#define ID_D_T_PLINE_ON 8
#define ID_D_T_PLINE_OFF 9
#define ID_FILLEMPTY 10
#define ID_FILL_BC 11
#define ID_FILL_C 12
#define ID_FILL_G 13
#define ID_FILL_BG 14
#define ID_FILL_NG 15
#define ID_PATTERN 16


#define ID_GRAB_PATTERN 21
#define ID_CHANGE_PATTERN 22
#define ID_PATTERN_SCALE 23

#define ID_PATTERN_ANGLE 30
#define ID_PATTERN_DX 34
#define ID_PATTERN_DY 44
#define ID_TRANSLUCENCY 55

#define ID_ARC_CONT 75
#define ID_ARC_P3 76
#define ID_ARC_SCE 77
#define ID_ARC_SCA 78
#define ID_ARC_SCL 79
#define ID_ARC_SER 80
#define ID_ARC_SEA 81
#define ID_ARC_SED  82
#define ID_ARC_REV_Y  86
#define ID_ARC_REV_N  87
#define ID_LINE_MODE 84
#define ID_LINE_CONTINUE 85

enum PLINE_MODE {PL_MODE_CONTINUE = 1 , PL_MODE_LINE , PL_MODE_ARC,
    PL_MODE_END, PL_MODE_UNDO, PL_MODE_ARC_ESC, PL_MODE_ARC_CR,
    PL_MODE_UNDO_CONT, PL_MODE_ARC_CONTINUE, PL_MODE_BREAK, PL_MODE_LINE_CONTINUE, PL_MODE_LINE_BAK} ;

static int pline_mode;

typedef struct
{
  BOOL b_line, b_solid ; 	//narysowano linie, narysowano obszar
  double width1 ;		//szerokosc poczatkowa pierwszego segmentu
  double width ;		//szerokosc bierzaca
  double width0;		//kopia szerokosci poczatkowej pierwszego segmentu
  double axis1 ;        //previous offset
  double axis ;         //current offset
  LINIA line ;
  int color_line ;
  WIELOKAT solid ;
}  t_trace ;

static t_trace  s_trace = {FALSE, FALSE, .2, .2, .2, 0,  0,Ldef, 7, S4def} ; /*S4def czworokat*/
static LINIA line_g = Ldef ;
#define r18 18
#define r22 16
#define r14 14
#define r16 16
#define r12 12
static int strwyj ;
static int np_trace_width ;
static double Lx2, Ly2 ;
static ESTR e_trace_width ;
static int d_line = 0 ;
static BOOL block_added = FALSE;
static LINIA LiniaG1 = Ldef;

static void (*cursor_on)(double ,double)=out_cur_on;
static void (*cursor_off)(double ,double)=out_cur_off;
static void set_next_solid (BOOL b_solid, BOOL b_end) ;

static double df__xbeg, df__ybeg;
static double df__len = 0;

static int Tbreak = 0;
static int AfterTbreak = 0;

static WIELOKAT ptr_archive;
static LINIA ptr_archive_line;
static LINIA ptr_archive_line1;
static LINIA ptr_archive_line2;
static BOOL ptr_archive_set = FALSE;

void* Get_Trace_First_Ob(void* ptr_pl);
int close_trace(void);
int break_trace(void);

static POLE pmTrace;

#define Trace_min 7;
#define Trace_max 8;

#define Dline_min 6;
#define Dline_max 6;

#define Tline_min 6;
#define Tline_max 6;

int mTrace_min, mTrace_max;

TMENU mTrace = {7,0,0,28,52,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr, 0, 0,0,&pmTrace, NULL, NULL} ;

static void  cur_off (double x, double y) ;
static void  cur_on (double x, double y) ;

unsigned char s_l_type = 34;

/*-----------------------------------------------------------------------*/

int com_no[3][8]={{0,1,2,3,4,5,6,7},{0,1,2,4,5,6,7,0},{0,1,2,4,5,6,7,0}};

int get_pline_mode(void)
{
    return pline_mode;
}

int get_Tbreak(void)
{
    return Tbreak;
}

void set_Tbreak(int tbreak)
{
    Tbreak=tbreak;
    komunikat_str_short((tbreak==0 ? "" : _BREAK_), (tbreak==0 ? FALSE : TRUE), TRUE);
    if (!tbreak) remove_short_notice();
}

int get_AfterTbreak(void)
{
    return AfterTbreak;
}

void set_AfterTbreak(int aftertbreak)
{
    AfterTbreak=aftertbreak;
}

void Normalize_Solid0(WIELOKAT* solid)
{
    return;
}

void set_trace_pattern_name()
{
    menu_par_new((*mTracePattern.pola)[0].txt, TracePatternName);
}

static void set_tracefilltype (int poz)
/*-----------------------------------*/
{
  char sk [10] ;

  strcpy(sk,tracefilltyp_tab[poz- ID_FILLEMPTY]);
  SolidFillTyp=poz- ID_FILLEMPTY;
  menu_par_new (pmFTrace[3].txt, sk) ;
  TracePattern = 0;
}


static void d_t_pline_on (void)
/*---------------------------*/
{
 char sk [MaxTextLen] = "";

 if (block_added==FALSE)
 { 
  if (FALSE == add_block ())
    {
     return ;
    }
   block_added=TRUE;
  }
 s_trace.solid.blok = ElemBlok ;
 s_trace.solid.obiektt2 = O2BlockPline ; 
 LiniaG1.blok = ElemBlok ;
 LiniaG1.obiektt2 = O2BlockPline ;
 
 d_t__pline=TRUE;

 strcpy(sk, _YES__);

 menu_par_new ((*mTrace.pola)[2].txt, sk) ;
}

static void d_t_pline_off (void)
/*---------------------------*/
{
 char sk [MaxTextLen] = "";

 s_trace.solid.blok = NoElemBlok ;
 s_trace.solid.obiektt2 = O2NoBlockS ; 
 LiniaG1.blok = NoElemBlok ;
 LiniaG1.obiektt2 = O2NoBlockS ;
 
 d_t__pline=FALSE;
 block_added=FALSE;
 strcpy(sk,_NO__);
 menu_par_new ((*mTrace.pola)[2].txt, sk) ;
}


static void width_trace (void)
/*---------------------------*/
{
  char sk [MaxTextLen] = "", *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;
  
  
  if (EOF == sprintf(sk, "%-6.5lg", milimetryob (s_trace.width)))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (!get_string (sk, "", MaxTextLen, 0, 24)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if (d <= 0)
  {
    ErrList (30) ;
    return ;
  }
  s_trace.width = jednostkiOb (d) ;
  sprintf(sk,"%lg",d);
  if (s_trace.axis == 0)
   {
    sprintf (e_trace_width.st, "%-6.5lg", d);
    e_trace_width.format = "%-6.5lg" ;
    e_trace_width.val_no = 1;
   } 
   else
    {
     e_trace_width.format = "%-6.5lg\0%-6.5lg;%-6.5lg" ;
     sprintf (e_trace_width.st, "%-6.5lg;%-6.5lg", d, milimetryob(s_trace.axis));
     e_trace_width.val_no = 2;
    }
  Out_Edited_Draw_Param ((ESTR *)&e_trace_width, TRUE) ;  
  e_trace_width.format = "%-6.5lg" ;  
  menu_par_new ((*mTrace.pola)[0].txt, sk) ;
}

static void axis_trace (void)
/*---------------------------*/
{
  char sk [MaxTextLen] = "", *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;

  if (EOF == sprintf(sk, "%-6.5lg", milimetryob (s_trace.axis)))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (!get_string (sk, "", MaxTextLen, 0, 106)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  s_trace.axis = jednostkiOb (d) ;
  sprintf(sk,"%lg",d);
  if (s_trace.axis == 0)
   {
    sprintf (e_trace_width.st, "%-6.5lg", d);
    e_trace_width.format = "%-6.5lg" ;
    e_trace_width.val_no = 1;
   } 
   else
    {
     e_trace_width.format = "%-6.5lg\0%-6.5lg;%-6.5lg" ;
     sprintf (e_trace_width.st, "%-6.5lg;%-6.5lg", milimetryob(s_trace.width),d);
     e_trace_width.val_no = 2;
    }
  Out_Edited_Draw_Param ((ESTR *)&e_trace_width, TRUE) ;
  menu_par_new ((*mTrace.pola)[1].txt, sk) ;
}

static int edit_width_trace (BOOL b_graph_value)
/*--------------------------------*/
{
  char sk [MaxTextLen] = "", *str ;

  b_graph_value = b_graph_value ;
  if (e_trace_width.val_no < 1 ||
      fabs (e_trace_width.values [0]) <= 0)
  {
    ErrList (30) ;
    return 0 ;
  }
  s_trace.width = jednostkiOb (e_trace_width.values [0]) ;

  df__Trace_Width=s_trace.width; ////

  if (e_trace_width.val_no > 1)
   {
    s_trace.axis = jednostkiOb (e_trace_width.values [1]) ;

   df__Trace_Axis=s_trace.axis; ////

   }
  
  sprintf (sk, "%-6.5lg", e_trace_width.values [0]) ;
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (strlen (sk) > 7) sk [7] = '\0' ;
  menu_par_new ((*mTrace.pola)[0].txt, sk) ;
  
  if (e_trace_width.val_no > 1)
   {
     sprintf (sk, "%-6.5lg", e_trace_width.values [1]) ;
     if (NULL != (str = strchr (sk, ' ')))
     {
       str [0] = '\0';
     }
     if (strlen (sk) > 7) sk [7] = '\0' ;
     menu_par_new ((*mTrace.pola)[1].txt, sk) ;
   }
  return 1 ;
}


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


static void erase_trace (void)
/*---------------------------*/
{
  BLOK *ptrs_block ;
  NAGLOWEK *nag ;

  nag = (NAGLOWEK*)dane ;
  ptrs_block = (BLOK*)dane ;
  if (nag->obiekt == OdBLOK &&
      ptrs_block->kod_obiektu == B_PLINE &&
     ptrs_block->dlugosc_opisu_obiektu == 1 &&
     ptrs_block->opis_obiektu [0] == PL_TRACE)
  {
    blokzap(dane, (char  *)(dane + nag->n), Anormalny, COPY_PUT, 0) ;
    usun_obiekt ((void *)dane) ;
  }
  else
  {
    ErrList (76) ;
  }
}


static BOOL add_2line_end (int opcja)
/*---------------------------------*/
{
  BOOL b_ret = FALSE ;
  WIELOKAT solid_temp ;
  unsigned char obiektt2;

Set_Second_Screen();

  if (s_trace.b_line == TRUE)
  {
    set_next_solid (s_trace.b_solid, TRUE) ;
    memcpy (&solid_temp, &s_trace.solid, sizeof (solid_temp)) ;
    Normalize_Solid0 (&solid_temp) ;
    solid_temp.empty_typ= SolidFillTyp;

   if (opcja == 1)  /*dopisanie osi*/
   {
    /* ustalenie koloru, typu i grubosci = 2*/
    LiniaG1.x1=s_trace.line.x1;
    LiniaG1.y1=s_trace.line.y1;
    LiniaG1.x2=s_trace.line.x2;
    LiniaG1.y2=s_trace.line.y2;
    LiniaG1.typ=s_l_type;
    LiniaG1.kolor=LiniaG.kolor;
    LiniaG1.warstwa=LiniaG.warstwa;

    if (NULL != dodaj_obiekt (((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
    {
    rysuj_obiekt ((char*)&s_trace.line, COPY_PUT, 0) ;
    rysuj_obiekt ((char*)&LiniaG1, COPY_PUT, 1) ;
    }
   }

   //dopisanie 1 linii

   //parametry pierwszej linii
   LiniaG1.typ = LiniaG.typ;
   LiniaG1.kolor = LiniaG.kolor;
   LiniaG1.warstwa=LiniaG.warstwa;
   LiniaG1.x1 = s_trace.solid.xy [0];
   LiniaG1.y1 = s_trace.solid.xy [1];
   LiniaG1.x2 = s_trace.solid.xy [6];
   LiniaG1.y2 = s_trace.solid.xy [7];

   if (NULL != dodaj_obiekt (((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
    {
    if (opcja==0) rysuj_obiekt ((char*)&s_trace.line, COPY_PUT, 0) ;
    rysuj_obiekt ((char*)&LiniaG1, COPY_PUT, 1) ;
    }
    //dopisanie drugiej linii
   //parametry drugiej linii
   LiniaG1.x1 = s_trace.solid.xy [2];
   LiniaG1.y1 = s_trace.solid.xy [3];
   LiniaG1.x2 = s_trace.solid.xy [4];
   LiniaG1.y2 = s_trace.solid.xy [5];

   if (NULL != dodaj_obiekt (((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
    {
    rysuj_obiekt ((char*)&LiniaG1, COPY_PUT, 1) ;
    b_ret = TRUE ;
    }
   //koniec dopisania linii
  }

  if (Tbreak)
  {
      s_trace.b_line = FALSE;
      //perpendicular line
      LiniaG1.x1 = s_trace.solid.xy[4];
      LiniaG1.y1 = s_trace.solid.xy[5];
      LiniaG1.x2 = s_trace.solid.xy[6];
      LiniaG1.y2 = s_trace.solid.xy[7];

      obiektt2 = LiniaG1.obiektt2;
      LiniaG1.obiektt2 = O2BlockSpecial;

      if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
      {
          // rysuj_obiekt ((char*)&s_trace.line, COPY_PUT, 0) ;
          rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
          b_ret = TRUE;
      }
      LiniaG1.obiektt2 = obiektt2;
  }

      if (AfterTbreak == 1)
      {
          //add perpendicular line

          LiniaG1.x1 = s_trace.solid.xy[0];
          LiniaG1.y1 = s_trace.solid.xy[1];
          LiniaG1.x2 = s_trace.solid.xy[2];
          LiniaG1.y2 = s_trace.solid.xy[3];
          LiniaG1.typ = LiniaG.typ;
          LiniaG1.kolor = LiniaG.kolor;
          LiniaG1.warstwa = LiniaG.warstwa;

          obiektt2 = LiniaG1.obiektt2;
          LiniaG1.obiektt2 = O2BlockSpecial;

          AfterTbreak = 0;

          if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
          {
              //rysuj_obiekt ((char*)&s_trace.line, COPY_PUT, 0) ;
              rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
          }

          LiniaG1.obiektt2 = obiektt2;

      }


      Set_Screen();
      flip_screen();
  return b_ret ;
}


static BOOL add_trace_line_end (void )
/*----------------------------------*/
{
  BOOL b_ret = FALSE ;
  WIELOKAT solid_temp ;
  char* scale_ptr;
  char* dx_ptr;
  char* angle_ptr;
  char* dy_ptr;
  char* name_ptr;
  char* translucency_ptr;
  BLOK *b;
  WIELOKAT *sadr;
  char *adk_b, *adp_b;

  if (s_trace.b_line == TRUE)
  {
      Set_Second_Screen();
    set_next_solid (s_trace.b_solid, TRUE) ;
    memcpy (&solid_temp, &s_trace.solid, sizeof (solid_temp)) ;
    Normalize_Solid0 (&solid_temp) ;
    solid_temp.empty_typ= SolidFillTyp;

    solid_temp.pattern = TracePattern;

    if (solid_temp.pattern == 1)
    {
        scale_ptr = solid_temp.xy;
        scale_ptr += (solid_temp.lp * sizeof(float));
        dx_ptr = scale_ptr;
        dx_ptr += sizeof(short int);
        angle_ptr = dx_ptr;
        angle_ptr += sizeof(short int);
        dy_ptr = angle_ptr;
        dy_ptr += sizeof(short int);
        name_ptr = dy_ptr;
        name_ptr += sizeof(short int);
        memmove(scale_ptr, &TracePatternScale, sizeof(short int));
        memmove(dx_ptr, &TracePatternDx, sizeof(short int));
        memmove(angle_ptr, &TracePatternAngle, sizeof(short int));
        memmove(dy_ptr, &TracePatternDy, sizeof(short int));
        memmove(name_ptr, &TracePatternName, strlen(TracePatternName) + 1);

        solid_temp.n = 8 + solid_temp.lp * sizeof(float) + 4 * sizeof(short int) + (int)strlen(TracePatternName) + 1;
    }
    else
    {
        solid_temp.translucent = TraceTranslucent;
        if (solid_temp.translucent == 0)
            solid_temp.n = 8 + solid_temp.lp * sizeof(float);
        else
        {
            translucency_ptr = solid_temp.xy;
            translucency_ptr += (solid_temp.lp * sizeof(float));
            memmove(translucency_ptr, &TraceTranslucency, sizeof(unsigned char));

            solid_temp.n = 8 + solid_temp.lp * sizeof(float) + sizeof(unsigned char);
        }
    }

    sadr = (WIELOKAT*)dodaj_obiekt (((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &solid_temp);
    if (sadr !=NULL)
    {
      rysuj_obiekt ((char*)&s_trace.line, COPY_PUT, 0) ;
      rysuj_obiekt ((char *)&solid_temp, COPY_PUT, 1) ;
      b_ret = TRUE ;

      if ((sadr->blok==ElemBlok) && (sadr->empty_typ>0) && (sadr->empty_typ<6))
        {
            b=FIRSTB((char*)sadr);
            if ((b->kod_obiektu == B_PLINE) && (b->opis_obiektu[0]== PL_TRACE))
            {
                adk_b=(char*)b + sizeof(NAGLOWEK) + b->n;
                adp_b=(char*)b + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                reset_trace_block();
                blokzap(adp_b,adk_b,ANieOkreslony,COPY_PUT,0);
                blokzap(b, adk_b, ANieOkreslony, COPY_PUT, 1);
            }
        }
    }


    Set_Screen();
    flip_screen();
  }
  if (Tbreak) s_trace.b_line = FALSE;

  s_trace.width1 = s_trace.width;  //This is for solidarc purpose, which is drawed after the solid
  s_trace.axis1 = s_trace.axis;  //This is for solidarc purpose, which is drawed after the solid

  set_pl(s_trace.line.x2, s_trace.line.y2);

  return b_ret ;
}


static void get_solid_trace_axis(double *x1, double *y1, double *x2, double *y2)
{  double dx1, dy1, dx2, dy2, df_l1 ;
    double angle_r, si_r, co_r, angle_b, si_b, co_b ;
    double df_sx1, df_sy1, df_sx2, df_sy2 ;

    dx1 = *x2 - *x1 ;
    dy1 = *y2 - *y1 ;
    df_l1 = sqrt (dx1 * dx1 + dy1 * dy1) ;
    angle_r = Atan2 (dy1, dx1) ;
    si_r = sin (angle_r) ;
    co_r = cos (angle_r) ;

    df_sx1 = 0; //df_l1 ;
    df_sx2 = 0; //df_l1 ;

    df_sy1 = s_trace.axis;
    df_sy2 = s_trace.axis;

    obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
    obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;

    *x1 = *x1 + df_sx1;
    *y1 = *y1 + df_sy1;

    *x2 = *x2 + df_sx2;
    *y2 = *y2 + df_sy2;

}

static void set_next_solid (BOOL b_solid, BOOL b_end)
/*--------------------------------------------------*/
{
  double dx1, dy1, dx2, dy2, df_l1 ;
  double angle_r, si_r, co_r, angle_b, si_b, co_b ;
  double df_sx1, df_sy1, df_sx2, df_sy2 ;

  dx1 = s_trace.line.x2 - s_trace.line.x1 ;
  dy1 = s_trace.line.y2 - s_trace.line.y1 ;
  df_l1 = sqrt (dx1 * dx1 + dy1 * dy1) ;
  angle_r = Atan2 (dy1, dx1) ;
  si_r = sin (angle_r) ;
  co_r = cos (angle_r) ;
  if (FALSE == b_solid)
  {
    df_sx1 = 0 ;
    df_sx2 = 0 ;
    
    df_sy1 = (s_trace.width1 / 2) - s_trace.axis;
    df_sy2 = (-s_trace.width1 / 2) - s_trace.axis;
      
    obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
    obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;
    s_trace.solid.xy [0] = df_sx1 + s_trace.line.x1 ;
    s_trace.solid.xy [1] = df_sy1 + s_trace.line.y1 ;
    s_trace.solid.xy [2] = df_sx2 + s_trace.line.x1 ;
    s_trace.solid.xy [3] = df_sy2 + s_trace.line.y1 ;
  }
  else
  {
    s_trace.solid.xy [0] = s_trace.solid.xy [6] ;
    s_trace.solid.xy [1] = s_trace.solid.xy [7] ;
    s_trace.solid.xy [2] = s_trace.solid.xy [4] ;
    s_trace.solid.xy [3] = s_trace.solid.xy [5] ;
  }
  if (FALSE == b_end)
  {
    dx2 = line_g.x2 - s_trace.line.x1 ;
    dy2 = line_g.y2 - s_trace.line.y1 ;
    obru (si_r, co_r, dx2, dy2, &dx2, &dy2) ;
    angle_b = Atan2 (dy2 - 0, dx2 - df_l1) ;
    if (angle_b > 0)
    {
      angle_b = (Pi2 / 2 - angle_b) / 2 + angle_b ;
    }
    else
    {
      angle_b = (Pi2 / 2 + angle_b) / 2 + Pi2 / 2 ;
    }
    si_b = sin (angle_b) ;
    co_b = cos (angle_b) ;
    if (TRUE == Check_if_Equal (si_b, 0))
    {
      df_sx1 = df_l1 ;
      df_sx2 = df_l1 ;
    }
    else
    {
      df_sx1 =  df_l1 - (co_b / si_b) * ((s_trace.width / 2) + s_trace.axis);
      df_sx2 = df_l1 + (co_b / si_b) * ((s_trace.width / 2) - s_trace.axis);
    }
  }
  else
  {
    df_sx1 = df_l1 ;
    df_sx2 = df_l1 ;
  }
  df_sy1 = (- s_trace.width / 2) - s_trace.axis;
  df_sy2 = (s_trace.width / 2) - s_trace.axis;
  obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
  obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;
  s_trace.solid.xy [4] = df_sx1 + s_trace.line.x1 ;
  s_trace.solid.xy [5] = df_sy1 + s_trace.line.y1 ;
  s_trace.solid.xy [6] = df_sx2 + s_trace.line.x1 ;
  s_trace.solid.xy [7] = df_sy2 + s_trace.line.y1 ;
  s_trace.solid.kolor = s_trace.color_line ;
  s_trace.solid.warstwa = Current_Layer ;
}


static BOOL add_2line (BOOL b_strwyj, int opcja)
/*--------------------------------------------*/
{
  BOOL b_ret = FALSE ;
  WIELOKAT solid_temp ;
  unsigned char obiektt2;

  if(b_strwyj)
  {
    line_g.x2 = Double_to_Float (Lx2) ;
    line_g.y2 = Double_to_Float (Ly2) ;
  }
  else
  {
    line_g.x2 = X ;
    line_g.y2 = Y ;
  }
  if(orto)
  {
    orto_l(&line_g, &Orto_Dir);
    Orto_Dir = I_Orto_NoDir;
  }
  if(line_g.x1 == line_g.x2 && line_g.y1 == line_g.y2)
  {
    return TRUE ;
  }

  if (Tbreak)
  {
      s_trace.b_line = FALSE;
      AfterTbreak = 1;
  }

  if (s_trace.b_line == TRUE)
  {
      Set_Second_Screen();

    rysuj_obiekt((char*)&s_trace.line, COPY_PUT, 0);

    set_next_solid (s_trace.b_solid, FALSE) ;
    memcpy (&solid_temp, &s_trace.solid, sizeof (solid_temp)) ;
    Normalize_Solid0 (&solid_temp) ;
    solid_temp.empty_typ= SolidFillTyp;

   if (opcja==1) /* dopisanie osi */
   {
    
    LiniaG1.x1=s_trace.line.x1;
    LiniaG1.y1=s_trace.line.y1;
    LiniaG1.x2=s_trace.line.x2;
    LiniaG1.y2=s_trace.line.y2;
    LiniaG1.typ=s_l_type;
    LiniaG1.kolor=LiniaG.kolor;
    LiniaG1.warstwa=LiniaG.warstwa;


    if (NULL != dodaj_obiekt (((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
    {
    rysuj_obiekt ((char*)&LiniaG1, COPY_PUT, 1) ;
    }
   }

   //dopisanie 1 linii

   //parametry pierwszej linii
   LiniaG1.typ = LiniaG.typ;
   LiniaG1.kolor = LiniaG.kolor;
   LiniaG1.warstwa=LiniaG.warstwa;
   LiniaG1.x1 = s_trace.solid.xy [0];
   LiniaG1.y1 = s_trace.solid.xy [1];
   LiniaG1.x2 = s_trace.solid.xy [6];
   LiniaG1.y2 = s_trace.solid.xy [7];

   if (NULL != dodaj_obiekt (((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
    {
    rysuj_obiekt ((char*)&LiniaG1, COPY_PUT, 1) ;
    }
    //dopisanie drugiej linii
   //parametry drugiej linii
   LiniaG1.x1 = s_trace.solid.xy [2];
   LiniaG1.y1 = s_trace.solid.xy [3];
   LiniaG1.x2 = s_trace.solid.xy [4];
   LiniaG1.y2 = s_trace.solid.xy [5];

   if (NULL != dodaj_obiekt (((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
    {
    rysuj_obiekt ((char*)&LiniaG1, COPY_PUT, 1) ;
    b_ret = TRUE ;
    }
   //koniec dopisania linii

       if (AfterTbreak == 1)
       {
           //add perpendicular line
           LiniaG1.x1 = s_trace.solid.xy[0];
           LiniaG1.y1 = s_trace.solid.xy[1];
           LiniaG1.x2 = s_trace.solid.xy[2];
           LiniaG1.y2 = s_trace.solid.xy[3];
           LiniaG1.typ = LiniaG.typ;
           LiniaG1.kolor = LiniaG.kolor;
           LiniaG1.warstwa = LiniaG.warstwa;

           obiektt2 = LiniaG1.obiektt2;
           LiniaG1.obiektt2 = O2BlockSpecial;

           AfterTbreak = 0;

           if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
           {
               rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
           }

           LiniaG1.obiektt2 = obiektt2;

       }

    s_trace.b_solid = TRUE ;

       Set_Screen();
       flip_screen();
  }

  if (!Tbreak)
  {
      memcpy(&s_trace.line, &line_g, sizeof(line_g));
      s_trace.color_line = line_g.kolor;

      s_trace.line.typ = 96;
      outline(&s_trace.line, COPY_PUT, 1);

      s_trace.b_line = TRUE;
  }
  else
  {
      memcpy(&s_trace.line, &line_g, sizeof(line_g));
      s_trace.b_solid = FALSE;
      s_trace.width1 = s_trace.width;
      s_trace.axis1 = s_trace.axis;  //This is for solidarc purpose, which is drawed after the solid
  }


  line_g.x1=line_g.x2;
  line_g.y1=line_g.y2;
  line_g.x2=X;
  line_g.y2=Y;

  LiniaG.x1=line_g.x1;
  LiniaG.y1=line_g.y1;
  LiniaG.x2=line_g.x2;
  LiniaG.y2=line_g.y2;

  return b_ret ;
}


static BOOL add_trace_line(BOOL b_strwyj)
/*--------------------------------------*/
{
  BOOL b_ret = FALSE ;
  WIELOKAT solid_temp ;
  char* scale_ptr;
  char* dx_ptr;
  char* angle_ptr;
  char* dy_ptr;
  char* name_ptr;
  char* translucency_ptr;

if(b_strwyj)
  {
    line_g.x2 = Double_to_Float (Lx2) ;
    line_g.y2 = Double_to_Float (Ly2) ;
  }
  else
  {
    line_g.x2 = X ;
    line_g.y2 = Y ;
  }
  if(orto)
  {
    orto_l(&line_g, &Orto_Dir);
    Orto_Dir = I_Orto_NoDir;
  }
  if(line_g.x1 == line_g.x2 && line_g.y1 == line_g.y2)
  {
    return TRUE ;
  }

  if (Tbreak)
  {
      s_trace.b_line = FALSE;
  }

  if (s_trace.b_line == TRUE)
  {
      Set_Second_Screen();
    set_next_solid (s_trace.b_solid, FALSE) ;

    memcpy (&solid_temp, &s_trace.solid, sizeof (solid_temp)) ;
    Normalize_Solid0 (&solid_temp) ;
    solid_temp.empty_typ= SolidFillTyp;

    solid_temp.pattern = TracePattern;

    if (solid_temp.pattern == 1)
    {
        scale_ptr = solid_temp.xy;
        scale_ptr += (solid_temp.lp * sizeof(float));
        dx_ptr = scale_ptr;
        dx_ptr += sizeof(short int);
        angle_ptr = dx_ptr;
        angle_ptr += sizeof(short int);
        dy_ptr = angle_ptr;
        dy_ptr += sizeof(short int);
        name_ptr = dy_ptr;
        name_ptr += sizeof(short int);
        memmove(scale_ptr, &TracePatternScale, sizeof(short int));
        memmove(dx_ptr, &TracePatternDx, sizeof(short int));
        memmove(angle_ptr, &TracePatternAngle, sizeof(short int));
        memmove(dy_ptr, &TracePatternDy, sizeof(short int));
        memmove(name_ptr, &TracePatternName, strlen(TracePatternName) + 1);

        solid_temp.n = 8 + solid_temp.lp * sizeof(float) + 4 * sizeof(short int) + (int)strlen(TracePatternName) + 1;
    }
    else
    {
        solid_temp.translucent = TraceTranslucent;
        if (solid_temp.translucent == 0)
            solid_temp.n = 8 + solid_temp.lp * sizeof(float);
        else
        {
            translucency_ptr = solid_temp.xy;
            translucency_ptr += (solid_temp.lp * sizeof(float));
            memmove(translucency_ptr, &TraceTranslucency, sizeof(unsigned char));

            solid_temp.n = 8 + solid_temp.lp * sizeof(float) + sizeof(unsigned char);
        }
    }

    s_trace.solid.n = solid_temp.n;

    if (NULL != dodaj_obiekt (((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &solid_temp))
    {
      rysuj_obiekt ((char*)&s_trace.line, COPY_PUT, 0) ;
      rysuj_obiekt ((char *)&solid_temp, COPY_PUT, 1) ;
      b_ret = TRUE ;
    }
    s_trace.b_solid = TRUE ;
    Set_Screen();
    flip_screen();
  }

  if (!Tbreak)
  {
      memcpy(&s_trace.line, &line_g, sizeof(line_g));
      s_trace.color_line = line_g.kolor;

      s_trace.line.typ = 96;
      outline(&s_trace.line, COPY_PUT, 1);

      s_trace.b_line = TRUE;
  }
  else
  {
      memcpy(&s_trace.line, &line_g, sizeof(line_g));
      s_trace.b_solid = FALSE;
      s_trace.width1 = s_trace.width;
      s_trace.axis1 = s_trace.axis;  //This is for solidarc purpose, which is drawed after the solid
  }


  line_g.x1=line_g.x2;
  line_g.y1=line_g.y2;
  line_g.x2=X;
  line_g.y2=Y;

  LiniaG.x1=line_g.x1;
  LiniaG.y1=line_g.y1;
  LiniaG.x2=line_g.x2;
  LiniaG.y2=line_g.y2;
  
  return b_ret ;
}


static BOOL add_trace_line_close(void)
/*----------------------------------*/
{
    BOOL b_ret = FALSE;
    WIELOKAT solid_temp;
    char* scale_ptr;
    char* dx_ptr;
    char* angle_ptr;
    char* dy_ptr;
    char* name_ptr;
    void* ptr_temp, *ptr_temp1, *ptr_temp2;
    double first_x1, first_y1, first_x2, first_y2;
    double dx1, dy1;
    double angle_r, si_r, co_r;
    double df_sx1, df_sy1;
    double s_trace_width, s_trace_width1;
    unsigned char obiektt2;
    char* translucency_ptr;

    if (NULL == (ptr_temp = Get_Trace_First_Ob((void*)dane)))
    {
        return FALSE;
    }

    if (d_line == 0)
    {

        first_x1 = (((WIELOKAT*)ptr_temp)->xy[0] + ((WIELOKAT*)ptr_temp)->xy[2]) / 2.0;
        first_y1 = (((WIELOKAT*)ptr_temp)->xy[1] + ((WIELOKAT*)ptr_temp)->xy[3]) / 2.0;

        first_x2 = (((WIELOKAT*)ptr_temp)->xy[4] + ((WIELOKAT*)ptr_temp)->xy[6]) / 2.0;
        first_y2 = (((WIELOKAT*)ptr_temp)->xy[5] + ((WIELOKAT*)ptr_temp)->xy[7]) / 2.0;

    }
    else if (d_line == 1) //2-line
    {
        ptr_temp1 = (char*)ptr_temp + ((LINIA*)ptr_temp)->n + sizeof(NAGLOWEK);

        first_x1 = (((LINIA*)ptr_temp)->x1 + ((LINIA*)ptr_temp1)->x1) / 2.0;
        first_y1 = (((LINIA*)ptr_temp)->y1 + ((LINIA*)ptr_temp1)->y1) / 2.0;

        first_x2 = (((LINIA*)ptr_temp)->x2 + ((LINIA*)ptr_temp1)->x2) / 2.0;
        first_y2 = (((LINIA*)ptr_temp)->y2 + ((LINIA*)ptr_temp1)->y2) / 2.0;

    }
    else if (d_line == 2) //3-line
    {

        ptr_temp1 = (char*)ptr_temp + ((LINIA*)ptr_temp)->n + sizeof(NAGLOWEK);
        ptr_temp2 = (char*)ptr_temp1 + ((LINIA*)ptr_temp1)->n + sizeof(NAGLOWEK);

        first_x1 = (((LINIA*)ptr_temp1)->x1 + ((LINIA*)ptr_temp2)->x1) / 2.0;
        first_y1 = (((LINIA*)ptr_temp1)->y1 + ((LINIA*)ptr_temp2)->y1) / 2.0;

        first_x2 = (((LINIA*)ptr_temp1)->x2 + ((LINIA*)ptr_temp2)->x2) / 2.0;
        first_y2 = (((LINIA*)ptr_temp1)->y2 + ((LINIA*)ptr_temp2)->y2) / 2.0;
    }

    dx1 = first_x2 - first_x1;
    dy1 = first_y2 - first_y1;

    angle_r = Atan2(dy1, dx1);
    si_r = sin(angle_r);
    co_r = cos(angle_r);

    df_sx1 = 0.0;
    df_sy1 = s_trace.axis;

    obrd(si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1);

    first_x1 += df_sx1;
    first_y1 += df_sy1;

    first_x2 += df_sx1;
    first_y2 += df_sy1;

    line_g.x2 = first_x1; // X;
    line_g.y2 = first_y1; // Y;

    if (line_g.x1 == line_g.x2 && line_g.y1 == line_g.y2)
    {
        return FALSE;
    }

    Set_Second_Screen();
    if (d_line == 0)
    {
        if (s_trace.b_line == TRUE)
        {
            set_next_solid(s_trace.b_solid, FALSE);
            memcpy(&solid_temp, &s_trace.solid, sizeof(solid_temp));
            Normalize_Solid0(&solid_temp);

            s_trace.b_solid = TRUE;

            solid_temp.empty_typ = SolidFillTyp;

            solid_temp.pattern = TracePattern;

            if (solid_temp.pattern == 1)
            {
                scale_ptr = solid_temp.xy;
                scale_ptr += (solid_temp.lp * sizeof(float));
                dx_ptr = scale_ptr;
                dx_ptr += sizeof(short int);
                angle_ptr = dx_ptr;
                angle_ptr += sizeof(short int);
                dy_ptr = angle_ptr;
                dy_ptr += sizeof(short int);
                name_ptr = dy_ptr;
                name_ptr += sizeof(short int);
                memmove(scale_ptr, &TracePatternScale, sizeof(short int));
                memmove(dx_ptr, &TracePatternDx, sizeof(short int));
                memmove(angle_ptr, &TracePatternAngle, sizeof(short int));
                memmove(dy_ptr, &TracePatternDy, sizeof(short int));
                memmove(name_ptr, &TracePatternName, strlen(TracePatternName) + 1);

                solid_temp.n = 8 + solid_temp.lp * sizeof(float) + 4 * sizeof(short int) + (int)strlen(TracePatternName) + 1;
            }
            else
            {
                solid_temp.translucent = TraceTranslucent;
                if (solid_temp.translucent == 0)
                    solid_temp.n = 8 + solid_temp.lp * sizeof(float);
                else
                {
                    translucency_ptr = solid_temp.xy;
                    translucency_ptr += (solid_temp.lp * sizeof(float));
                    memmove(translucency_ptr, &TraceTranslucency, sizeof(unsigned char));

                    solid_temp.n = 8 + solid_temp.lp * sizeof(float) + sizeof(unsigned char);
                }
            }

            s_trace.solid.n = solid_temp.n;

            if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &solid_temp))
            {
                rysuj_obiekt((char*)&s_trace.line, COPY_PUT, 0);
                rysuj_obiekt((char*)&solid_temp, COPY_PUT, 1);
                b_ret = TRUE;
            }
            s_trace.b_solid = TRUE;
        }

        s_trace.color_line = line_g.kolor;

        s_trace.line.x1 = s_trace.line.x2;
        s_trace.line.y1 = s_trace.line.y2;
        s_trace.line.x2 = first_x1;
        s_trace.line.y2 = first_y1;

        line_g.x1 = first_x1;
        line_g.y1 = first_y1;
        line_g.x2 = first_x2;
        line_g.y2 = first_y2;


        //first determine s_trace.width0 taken from first segment (can be different)
        s_trace_width = s_trace.width;
        s_trace.width = s_trace.width0; //s_trace.width1;

        set_next_solid(s_trace.b_solid, FALSE);

        memcpy(&solid_temp, &s_trace.solid, sizeof(solid_temp));
        Normalize_Solid0(&solid_temp);
        solid_temp.empty_typ = SolidFillTyp;

        solid_temp.pattern = TracePattern;

        if (solid_temp.pattern == 1)
        {
            scale_ptr = solid_temp.xy;
            scale_ptr += (solid_temp.lp * sizeof(float));
            dx_ptr = scale_ptr;
            dx_ptr += sizeof(short int);
            angle_ptr = dx_ptr;
            angle_ptr += sizeof(short int);
            dy_ptr = angle_ptr;
            dy_ptr += sizeof(short int);
            name_ptr = dy_ptr;
            name_ptr += sizeof(short int);
            memmove(scale_ptr, &TracePatternScale, sizeof(short int));
            memmove(dx_ptr, &TracePatternDx, sizeof(short int));
            memmove(angle_ptr, &TracePatternAngle, sizeof(short int));
            memmove(dy_ptr, &TracePatternDy, sizeof(short int));
            memmove(name_ptr, &TracePatternName, strlen(TracePatternName) + 1);

            solid_temp.n = 8 + solid_temp.lp * sizeof(float) + 4 * sizeof(short int) + (int)strlen(TracePatternName) + 1;
        }
        else
        {
            solid_temp.translucent = TraceTranslucent;
            if (solid_temp.translucent == 0)
                solid_temp.n = 8 + solid_temp.lp * sizeof(float);
            else
            {
                translucency_ptr = solid_temp.xy;
                translucency_ptr += (solid_temp.lp * sizeof(float));
                memmove(translucency_ptr, &TraceTranslucency, sizeof(unsigned char));

                solid_temp.n = 8 + solid_temp.lp * sizeof(float) + sizeof(unsigned char);
            }
        }

        s_trace.solid.n = solid_temp.n;

        if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &solid_temp))
        {

            b_ret = TRUE;

            //correction of the first element
            if (NULL != (ptr_temp = Get_Trace_First_Ob((void*)dane)))
            {
                rysuj_obiekt(ptr_temp, COPY_PUT, 0);

                memcpy(&ptr_archive.xy, ((WIELOKAT*)ptr_temp)->xy, ((WIELOKAT*)ptr_temp)->lp*sizeof(float));

                ptr_archive_set = TRUE;

                rysuj_obiekt((char*)&solid_temp, COPY_PUT, 1);

                ((WIELOKAT*)ptr_temp)->xy[0] = s_trace.solid.xy[6];
                ((WIELOKAT*)ptr_temp)->xy[1] = s_trace.solid.xy[7];
                ((WIELOKAT*)ptr_temp)->xy[2] = s_trace.solid.xy[4];
                ((WIELOKAT*)ptr_temp)->xy[3] = s_trace.solid.xy[5];


                rysuj_obiekt(ptr_temp, COPY_PUT, 1);
            }
        }

        s_trace.width = s_trace_width;
    }
    else 
    {

        if (s_trace.b_line == TRUE)
        {
            set_next_solid(s_trace.b_solid, FALSE);
            memcpy(&solid_temp, &s_trace.solid, sizeof(solid_temp));
            Normalize_Solid0(&solid_temp);

            s_trace.b_solid = TRUE;

            rysuj_obiekt((char*)&s_trace.line, COPY_PUT, 0);

            if (d_line == 2) //3-line
            {


                LiniaG1.x1 = s_trace.line.x1;
                LiniaG1.y1 = s_trace.line.y1;
                LiniaG1.x2 = s_trace.line.x2;
                LiniaG1.y2 = s_trace.line.y2;
                LiniaG1.typ = s_l_type;
                LiniaG1.kolor = LiniaG.kolor;
                LiniaG1.warstwa = LiniaG.warstwa;

                if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
                {
                    rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
                }

            }

            LiniaG1.typ = LiniaG.typ;
            LiniaG1.kolor = LiniaG.kolor;
            LiniaG1.warstwa = LiniaG.warstwa;
            LiniaG1.x1 = s_trace.solid.xy[0];
            LiniaG1.y1 = s_trace.solid.xy[1];
            LiniaG1.x2 = s_trace.solid.xy[6];
            LiniaG1.y2 = s_trace.solid.xy[7];

            if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
            {
                rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
            }
            //dopisanie drugiej linii
            //parametry drugiej linii
            LiniaG1.x1 = s_trace.solid.xy[2];
            LiniaG1.y1 = s_trace.solid.xy[3];
            LiniaG1.x2 = s_trace.solid.xy[4];
            LiniaG1.y2 = s_trace.solid.xy[5];

            if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
            {
                rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
                b_ret = TRUE;
            }
            //koniec dopisania linii

            if (AfterTbreak == 1)
            {
                //add perpendicular line

                LiniaG1.x1 = s_trace.solid.xy[0];
                LiniaG1.y1 = s_trace.solid.xy[1];
                LiniaG1.x2 = s_trace.solid.xy[2];
                LiniaG1.y2 = s_trace.solid.xy[3];
                LiniaG1.typ = LiniaG.typ;;
                LiniaG1.kolor = LiniaG.kolor;
                LiniaG1.warstwa = LiniaG.warstwa;
                obiektt2 = LiniaG1.obiektt2;
                LiniaG1.obiektt2 = O2BlockSpecial;
                AfterTbreak = 0;

                if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
                {
                    rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
                }
                LiniaG1.obiektt2 = obiektt2;
            }

        }

        //now closing line
        //first determine s_trace.width0 taken from first segment (can be different)
        s_trace_width = s_trace.width;
        s_trace.width = s_trace.width0; // s_trace.width1;

        s_trace.line.x1 = s_trace.line.x2;
        s_trace.line.y1 = s_trace.line.y2;
        s_trace.line.x2 = first_x1;
        s_trace.line.y2 = first_y1;

        line_g.x1 = first_x1;
        line_g.y1 = first_y1;
        line_g.x2 = first_x2;
        line_g.y2 = first_y2;

        set_next_solid(s_trace.b_solid, FALSE);

        memcpy(&solid_temp, &s_trace.solid, sizeof(solid_temp));
        Normalize_Solid0(&solid_temp);

        if (d_line == 2) //3-line
        {
            LiniaG1.x1 = s_trace.line.x1;
            LiniaG1.y1 = s_trace.line.y1;
            LiniaG1.x2 = s_trace.line.x2;
            LiniaG1.y2 = s_trace.line.y2;
            LiniaG1.typ = s_l_type;
            LiniaG1.kolor = LiniaG.kolor;
            LiniaG1.warstwa = LiniaG.warstwa;

            if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
            {
                rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
            }
        }

        LiniaG1.typ = LiniaG.typ;
        LiniaG1.kolor = LiniaG.kolor;
        LiniaG1.warstwa = LiniaG.warstwa;
        LiniaG1.x1 = s_trace.solid.xy[0];
        LiniaG1.y1 = s_trace.solid.xy[1];
        LiniaG1.x2 = s_trace.solid.xy[6];
        LiniaG1.y2 = s_trace.solid.xy[7];

        if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
        {
            rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
        }
        //dopisanie drugiej linii
        //parametry drugiej linii
        LiniaG1.x1 = s_trace.solid.xy[2];
        LiniaG1.y1 = s_trace.solid.xy[3];
        LiniaG1.x2 = s_trace.solid.xy[4];
        LiniaG1.y2 = s_trace.solid.xy[5];

        if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
        {
            rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
            b_ret = TRUE;
        }
        //koniec dopisania linii

        if (AfterTbreak == 1)
        {
            //add perpendicular line

            LiniaG1.x1 = s_trace.solid.xy[0];
            LiniaG1.y1 = s_trace.solid.xy[1];
            LiniaG1.x2 = s_trace.solid.xy[2];
            LiniaG1.y2 = s_trace.solid.xy[3];
            LiniaG1.typ = LiniaG.typ;;
            LiniaG1.kolor = LiniaG.kolor;
            LiniaG1.warstwa = LiniaG.warstwa;
            obiektt2 = LiniaG1.obiektt2;
            LiniaG1.obiektt2 = O2BlockSpecial;
            AfterTbreak = 0;


            if (NULL != dodaj_obiekt(((d_t__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG1))
            {
                rysuj_obiekt((char*)&LiniaG1, COPY_PUT, 1);
            }
            LiniaG1.obiektt2 = obiektt2;
        }

        //correction of the first element
        if (NULL != (ptr_temp = Get_Trace_First_Ob((void*)dane)))
        {

            if (d_line == 1) //2-line
            {
                ptr_temp1 = (char*)ptr_temp + ((LINIA*)ptr_temp)->n + sizeof(NAGLOWEK);

                //archiving
                memcpy(&ptr_archive_line, (LINIA*)ptr_temp, sizeof(LINIA));
                memcpy(&ptr_archive_line1, (LINIA*)ptr_temp1, sizeof(LINIA));
                ptr_archive_set = TRUE;

                rysuj_obiekt(ptr_temp, COPY_PUT, 0);
                rysuj_obiekt(ptr_temp1, COPY_PUT, 0);

                ((LINIA*)ptr_temp)->x1 = s_trace.solid.xy[6];
                ((LINIA*)ptr_temp)->y1 = s_trace.solid.xy[7];

                ((LINIA*)ptr_temp1)->x1 = s_trace.solid.xy[4];
                ((LINIA*)ptr_temp1)->y1 = s_trace.solid.xy[5];

                rysuj_obiekt(ptr_temp, COPY_PUT, 1);
                rysuj_obiekt(ptr_temp1, COPY_PUT, 1);

            }
            else if (d_line == 2) //3-line
            {

                ptr_temp1 = (char*)ptr_temp + ((LINIA*)ptr_temp)->n + sizeof(NAGLOWEK);
                ptr_temp2 = (char*)ptr_temp1 + ((LINIA*)ptr_temp1)->n + sizeof(NAGLOWEK);

                //archiving
                memcpy(&ptr_archive_line, (LINIA*)ptr_temp, sizeof(LINIA));
                memcpy(&ptr_archive_line1, (LINIA*)ptr_temp1, sizeof(LINIA));
                memcpy(&ptr_archive_line2, (LINIA*)ptr_temp2, sizeof(LINIA));
                ptr_archive_set = TRUE;

                rysuj_obiekt(ptr_temp, COPY_PUT, 0);
                rysuj_obiekt(ptr_temp1, COPY_PUT, 0);
                rysuj_obiekt(ptr_temp2, COPY_PUT, 0);


                ((LINIA*)ptr_temp)->x1 = s_trace.line.x2;
                ((LINIA*)ptr_temp)->y1 = s_trace.line.y2;

                ((LINIA*)ptr_temp1)->x1 = s_trace.solid.xy[6];
                ((LINIA*)ptr_temp1)->y1 = s_trace.solid.xy[7];

                ((LINIA*)ptr_temp2)->x1 = s_trace.solid.xy[4];
                ((LINIA*)ptr_temp2)->y1 = s_trace.solid.xy[5];

                rysuj_obiekt(ptr_temp, COPY_PUT, 1);
                rysuj_obiekt(ptr_temp1, COPY_PUT, 1);
                rysuj_obiekt(ptr_temp2, COPY_PUT, 1);

            }
        }

        s_trace.width = s_trace_width;

    }

    s_trace.b_line = FALSE;

    Set_Screen();
    flip_screen();

    return b_ret;
}


static BOOL add_block (void)
/*------------------------*/
{
  BLOK s_blockd = Bdef ; //TO CHECK
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = B_PLINE ;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  c_pltype = PL_TRACE ;
  memmove (&(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  return TRUE ;
}


BOOL add_trace_ (void)
{
    return add_block ();
}


//static
void redraw_trace (void)
/*----------------------------*/
{
   if ( s_trace.b_line == TRUE)
   {
     rysuj_obiekt ((char*)&s_trace.line, COPY_PUT, 1) ;  //TUTAJ
   }
}

void set_trace_pattern_trans(char* sk)
{
    menu_par_new((*mFillTyp.pola)[0].txt, sk);
}

static void set_grab_tracepattern(void)
{
    Wez_Trace_Pattern();
    return;
}

static void set_change_tracepattern(void)
{
    Set_Trace_Pattern();
    return;
}

static void set_tracepattern(void)
{
    char sk[10];
    int ret;
    ret = Open_TracePatterns();
    if (TracePattern == 1)
    {
        strcpy(sk, tracefilltyp_tab[6]);
        SolidFillTyp = 0;
        menu_par_new((*mTracePattern.pola)[0].txt, TracePatternName);
    }
    else
    {
        ;
    }
}

void trace_image_pattern_scale(int poz)
{
    char sk[16];

    TracePatternScale = poz - ID_PATTERN_SCALE;

    sprintf(sk, "%5.3f", solidpatternscales[TracePatternScale]);

    menu_par_new((*mTracePattern.pola)[1].txt, sk);
}

void trace_image_pattern_angle(int poz)
{
    char sk[16];

    TracePatternAngle = poz - ID_PATTERN_ANGLE;

    sprintf(sk, "%d", solidpatternangles[TracePatternAngle]);

    menu_par_new((*mTracePattern.pola)[2].txt, sk);
}

void trace_image_pattern_dx(int poz)
{
    char sk[16];

    TracePatternDx = poz - ID_PATTERN_DX;

    sprintf(sk, "%s", solidpatterndxy[TracePatternDx]);

    menu_par_new((*mTracePattern.pola)[3].txt, sk);
}

void trace_image_pattern_dy(int poz)
{
    char sk[16];

    TracePatternDy = poz - ID_PATTERN_DY;

    sprintf(sk, "%s", solidpatterndxy[TracePatternDy]);

    menu_par_new((*mTracePattern.pola)[4].txt, sk);
}

static void set_tracetranslucency(int poz0)
{
    char sk[10];
    int poz = poz0;

    int d_trans = 12;
    int transluc;

    TracePattern = 0;
    SolidFillTyp = 0;

    if (poz0 == 54) TraceTranslucent = 0;
    else TraceTranslucent = 1;

    TraceTranslucency = 255 - (poz0 - 54) * d_trans;

    transluc = (TraceTranslucency / d_trans) - 1;
    sprintf(sk, "%d%%", transluc * 5);

    menu_par_new((*mFillTyp.pola)[0].txt, sk);
}

void* Get_Trace_Last_Ob(void* ptr_pl, int *objects_no)
/*--------------------------------------------------*/
{
    BLOK* ptrs_block;
    long_long off, offk, ad;
    NAGLOWEK* nag;
    void* ptr_last;
    int ob_number;

    ptrs_block = (BLOK*)ptr_pl;
    if (FALSE == check_head_trace(ptrs_block))
    {
        return NULL;
    }
    ptr_last = NULL;
    off = (char*)ptrs_block - dane;
    offk = off + ptrs_block->n + sizeof(NAGLOWEK) - 1;
    ob_number = 0;
    ObiektTok(&off, offk, &ad, ONieOkreslony);
    while (ad != -1)
    {
        nag = (NAGLOWEK*)(dane + ad);
        switch (nag->obiekt)
        {
        case Olinia:
            if (d_line>0)
                ptr_last = (void*)nag;
            ob_number++;
            break;
        case Owwielokat:
            if (d_line==0)
                ptr_last = (void*)nag;
            ob_number++;
            break;
        case Osolidarc:
           if (d_line==0)
                ptr_last = (void*)nag;
            ob_number++;
            break;
        default:
            break;
        }
        ObiektTok(&off, offk, &ad, ONieOkreslony);
    }

    *objects_no = ob_number;
    return ptr_last;
}

static BOOL check_head_trace(BLOK* ptrs_block)
/*------------------------------------------*/
{
    BOOL b_ret;

    b_ret = FALSE;
    if (NULL != ptrs_block &&
        ptrs_block->obiekt == OdBLOK &&
        ((ptrs_block->kod_obiektu == B_PLINE) || (ptrs_block->kod_obiektu == 'E')) &&
        (ptrs_block->opis_obiektu[0] == PL_TRACE))
    {
        b_ret = TRUE;
    }
    return b_ret;
}

void* Get_Trace_First_Ob(void* ptr_pl)
/*-----------------------------------*/
{
    BLOK* ptrs_block;
    char* ptr_first;

    ptrs_block = (BLOK*)ptr_pl;
    if (FALSE == check_head_trace(ptrs_block))
    {
        return NULL;
    }
    ptr_first = (char*)ptrs_block;
    ptr_first += sizeof(NAGLOWEK) + B3 + ptrs_block->dlugosc_opisu_obiektu;

    return (void*)ptr_first;
}


BOOL Get_End_Trace(void* ptr_pl,
    void** ptr_ob,	/*adres ostatniego  obiektu*/
    BOOL* b_first_end,	/*ktory koniec objektu jest ostatni*/
    double 	df_x_pl_beg,
    double 	df_y_pl_beg,
    double* df_x_pl_end,
    double* df_y_pl_end)
    /*-------------------------------------------*/
{
    BLOK* ptrs_block;
    long_long off, offk, ad;
    NAGLOWEK* nag;
    LUK* ptrs_arc_l, * ptrs_arc_bl;
    LINIA* ptrs_line_bl;
    SPLINE* ptrs_spline_bl;
    WIELOKAT* ptrs_solid_bl;
    SOLIDARC *ptrs_solidarc_bl;
    void* ptr_before_last, * ptr_last;
    double xy[8];

    double x1, y1, x2, y2, x1l, y1l, x2l, y2l;
    double df_x_beg, df_y_beg;

    *df_x_pl_end = df_x_pl_beg;
    *df_y_pl_end = df_y_pl_beg;
    *ptr_ob = NULL;
    ptrs_block = (BLOK*)ptr_pl;
    if (FALSE == check_head_trace(ptrs_block))
    {
        return FALSE;
    }
    ptr_before_last = NULL;
    ptr_last = NULL;
    off = (char*)ptrs_block - dane;
    offk = off + ptrs_block->n + sizeof(NAGLOWEK) - 1;
    ObiektTok(&off, offk, &ad, ONieOkreslony);
    while (ad != -1)
    {
        nag = (NAGLOWEK*)(dane + ad);
        switch (nag->obiekt)
        {
        case Owwielokat:
        case Osolidarc:
            if (ptr_last != NULL)
            {
                ptr_before_last = ptr_last;
            }
            ptr_last = (void*)nag;
            break;
        default:
            break;
        }
        ObiektTok(&off, offk, &ad, ONieOkreslony);
    }
    if (ptr_last == NULL)
    {
        return FALSE;
    }
    *ptr_ob = ptr_last;

    if (((NAGLOWEK*)ptr_last)->obiekt == Owwielokat)
    {
       ptrs_solid_bl = (WIELOKAT*)ptr_last;

        df_x_beg = (ptrs_solid_bl->xy[ptrs_solid_bl->lp - 8] + ptrs_solid_bl->xy[ptrs_solid_bl->lp - 6])/2;
        df_y_beg = (ptrs_solid_bl->xy[ptrs_solid_bl->lp - 7] + ptrs_solid_bl->xy[ptrs_solid_bl->lp - 5])/2;

        *df_x_pl_end = (ptrs_solid_bl->xy[ptrs_solid_bl->lp - 2] + ptrs_solid_bl->xy[ptrs_solid_bl->lp - 4])/2;
        *df_y_pl_end = (ptrs_solid_bl->xy[ptrs_solid_bl->lp - 1] + ptrs_solid_bl->xy[ptrs_solid_bl->lp -3 ])/2;
        get_solid_trace_axis(&df_x_beg, &df_y_beg, df_x_pl_end, df_y_pl_end);


        memmove(&s_trace_solid, &ptrs_solid_bl, sizeof(ptrs_solid_bl));

        point1.x = df_x_beg;
        point1.y = df_y_beg;
        point2.x = *df_x_pl_end;
        point2.y = *df_y_pl_end;

       *b_first_end = FALSE;
        return TRUE;
    }
    else if (((NAGLOWEK*)ptr_last)->obiekt == Osolidarc)
    {
        ptrs_solidarc_bl=(SOLIDARC *)ptr_last;

        x1l = ptrs_solidarc_bl->x + ptrs_solidarc_bl->r * cos(ptrs_solidarc_bl->kat1);
        y1l = ptrs_solidarc_bl->y + ptrs_solidarc_bl->r * sin(ptrs_solidarc_bl->kat1);
        x2l = ptrs_solidarc_bl->x + ptrs_solidarc_bl->r * cos(ptrs_solidarc_bl->kat2);
        y2l = ptrs_solidarc_bl->y + ptrs_solidarc_bl->r * sin(ptrs_solidarc_bl->kat2);

        if (ptrs_solidarc_bl->reversed)
        {
            *df_x_pl_end = x1l ;
            *df_y_pl_end = y1l ;
            *b_first_end = TRUE ;
        }
        else
        {
            *df_x_pl_end = x2l ;
            *df_y_pl_end = y2l ;
            *b_first_end = FALSE ;
        }


        /*
        //checking if solidarc is reversed
        if (ptr_before_last == NULL)
        {
            x1 = x2 = df_x_pl_beg ;
            y1 = y2 = df_y_pl_beg ;
        }
        else
        {
            if (((NAGLOWEK*)ptr_before_last)->obiekt == Owwielokat)
            {
               ptrs_solid_bl = (WIELOKAT*)ptr_before_last;

                x1 = (ptrs_solid_bl->xy[ptrs_solid_bl->lp - 8] + ptrs_solid_bl->xy[ptrs_solid_bl->lp - 6])/2;
                y1 = (ptrs_solid_bl->xy[ptrs_solid_bl->lp - 7] + ptrs_solid_bl->xy[ptrs_solid_bl->lp - 5])/2;
                x2 = (ptrs_solid_bl->xy[ptrs_solid_bl->lp - 2] + ptrs_solid_bl->xy[ptrs_solid_bl->lp - 4])/2;
                y2 = (ptrs_solid_bl->xy[ptrs_solid_bl->lp - 1] + ptrs_solid_bl->xy[ptrs_solid_bl->lp - 3])/2;

                get_solid_trace_axis(&x1, &y1, &x2, &y2);

                b_ptr_before_last=Owwielokat;

           }
            else if (((NAGLOWEK*)ptr_before_last)->obiekt == Osolidarc)
            {
             ptrs_solidarc_bl = (SOLIDARC*)ptr_before_last;
                //get_solidarc_ends(ptrs_solidarc_bl, &xy);

                if (ptrs_solidarc_bl->reversed) {
                    x1 = ptrs_solidarc_bl->x + ptrs_solidarc_bl->r * cos(ptrs_solidarc_bl->kat1);
                    y1 = ptrs_solidarc_bl->y + ptrs_solidarc_bl->r * sin(ptrs_solidarc_bl->kat1);
                    x2 = ptrs_solidarc_bl->x + ptrs_solidarc_bl->r * cos(ptrs_solidarc_bl->kat2);
                    y2 = ptrs_solidarc_bl->y + ptrs_solidarc_bl->r * sin(ptrs_solidarc_bl->kat2);
                }
                else {
                    x2 = ptrs_solidarc_bl->x + ptrs_solidarc_bl->r * cos(ptrs_solidarc_bl->kat1);
                    y2 = ptrs_solidarc_bl->y + ptrs_solidarc_bl->r * sin(ptrs_solidarc_bl->kat1);
                    x1 = ptrs_solidarc_bl->x + ptrs_solidarc_bl->r * cos(ptrs_solidarc_bl->kat2);
                    y1 = ptrs_solidarc_bl->y + ptrs_solidarc_bl->r * sin(ptrs_solidarc_bl->kat2);
                }
            }
        }
        *df_x_pl_end = x1l ;
        *df_y_pl_end = y1l ;
        *b_first_end = TRUE ;
        if ((TRUE == Check_if_Equal (x1l, x1) &&
             TRUE == Check_if_Equal (y1l, y1)) ||
            (TRUE == Check_if_Equal (x1l, x2) &&
             TRUE == Check_if_Equal (y1l, y2)))
        {
            *df_x_pl_end = x2l ;
            *df_y_pl_end = y2l ;
            *b_first_end = FALSE ;
        }
        */
        return TRUE;
    }
    
    return FALSE;
}



int last_trace_delete_factory(void)
/*-------------------------------------*/
{
    void* ptr_temp, *ptr_temp1, *ptr_temp2, *ptr_temp0, * ptr_temp02, * ptr_ob;
    double df_xend, df_yend;
    BOOL b_first_end;
    LINIA s_trace_line;
    long head, tail, tail1, tail0, tail02;
    BOOL break_line = FALSE;
    BOOL break_line2 = FALSE;
    float xy[4];
    double dx1, dy1;
    double angle_r, si_r, co_r;
    double df_sx1, df_sy1;
    LINIA s_line = Ldef;
    LINIA s_line2 = Ldef;
    double x1, y1, x2, y2, t1, t2;
    int inter1, inter2;
    int ob_number, ob_number_min;
    BOOL found_break;
    int ret;
    BOOL catch_second_end=FALSE;
    static LINIA line_g_bak = Ldef ;
    BOOL b_second_pl_seg;
    double df_xbeg, df_ybeg;

    Tbreak = 0;
    komunikat_str_short("", FALSE, TRUE);
    remove_short_notice();

   if (!block_added) return -83;

    repeat_del:

    if (NULL == (ptr_temp = Get_Trace_Last_Ob((void*)dane, &ob_number)))
    {
       
        if (s_trace.b_line)
        {

            rysuj_obiekt((char*)&s_trace.line, COPY_PUT, 0);

            line_g.x1 = s_trace.line.x1; // df_xend;
            line_g.y1 = s_trace.line.y1; // df_yend;


            line_g.x2 = X;
            line_g.y2 = Y;

            s_trace.b_line = FALSE;
        }

        CUR_OFF(X, Y);
        CUR_ON(X, Y);

    }
    else
    {
            if (d_line == 0)
            {

                if (((NAGLOWEK*)ptr_temp)->obiekt==Owwielokat)
                {
                   if (s_trace.b_line) rysuj_obiekt((char *) &s_trace.line, COPY_PUT, 0); ////

                    if (pline_mode == PL_MODE_ARC)
                        pline_mode = PL_MODE_LINE;

                    memcpy(&s_trace.solid, ptr_temp, sizeof(s_trace.solid));
                    memcpy(&s_trace_line, &s_trace.line, sizeof(s_trace_line));

                    s_trace.line.x1 = (((WIELOKAT *) ptr_temp)->xy[0] + ((WIELOKAT *) ptr_temp)->xy[2]) / 2.0;
                    s_trace.line.y1 = (((WIELOKAT *) ptr_temp)->xy[1] + ((WIELOKAT *) ptr_temp)->xy[3]) / 2.0;
                    s_trace.line.x2 = (((WIELOKAT *) ptr_temp)->xy[4] + ((WIELOKAT *) ptr_temp)->xy[6]) / 2.0;
                    s_trace.line.y2 = (((WIELOKAT *) ptr_temp)->xy[5] + ((WIELOKAT *) ptr_temp)->xy[7]) / 2.0;


                    dx1 = s_trace.line.x2 - s_trace.line.x1;
                    dy1 = s_trace.line.y2 - s_trace.line.y1;

                    angle_r = Atan2(dy1, dx1);
                    si_r = sin(angle_r);
                    co_r = cos(angle_r);

                    df_sx1 = 0.0;
                    df_sy1 = s_trace.axis;

                    obrd(si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1);

                    s_trace.line.x1 += df_sx1;
                    s_trace.line.y1 += df_sy1;

                    s_trace.line.x2 += df_sx1;
                    s_trace.line.y2 += df_sy1;

                    //intersection with first line
                    s_line2.x1 = ((WIELOKAT *) ptr_temp)->xy[0];
                    s_line2.y1 = ((WIELOKAT *) ptr_temp)->xy[1];
                    s_line2.x2 = ((WIELOKAT *) ptr_temp)->xy[2];
                    s_line2.y2 = ((WIELOKAT *) ptr_temp)->xy[3];
                    inter1 = przeciecieLL_tt(&x1, &y1, &s_trace.line, &s_line2, &t1, &t2);

                    //intersection with second line
                    s_line2.x1 = ((WIELOKAT *) ptr_temp)->xy[4];
                    s_line2.y1 = ((WIELOKAT *) ptr_temp)->xy[5];
                    s_line2.x2 = ((WIELOKAT *) ptr_temp)->xy[6];
                    s_line2.y2 = ((WIELOKAT *) ptr_temp)->xy[7];
                    inter2 = przeciecieLL_tt(&x2, &y2, &s_trace.line, &s_line2, &t1, &t2);

                    if (inter1 != 0) {
                        s_trace.line.x1 = x1;
                        s_trace.line.y1 = y1;
                    }

                    if (inter2 != 0) {
                        s_trace.line.x2 = x2;
                        s_trace.line.y2 = y2;
                    }

                    rysuj_obiekt((char *) ptr_temp, COPY_PUT, 0);
                    Usun_Object(ptr_temp, FALSE);

                    //correction of first object
                    if (ptr_archive_set) {
                        if (NULL != (ptr_temp = Get_Trace_First_Ob((void *) dane))) {
                            rysuj_obiekt(ptr_temp, COPY_PUT, 0);

                            memcpy(((WIELOKAT *) ptr_temp)->xy, &ptr_archive.xy,
                                   ((WIELOKAT *) ptr_temp)->lp * sizeof(float));

                            rysuj_obiekt(ptr_temp, COPY_PUT, 1);
                        }
                        ptr_archive_set = FALSE;
                    }
                }
                else if (((NAGLOWEK*)ptr_temp)->obiekt==Osolidarc)
                {
                    SOLIDARC *sa;
                    sa=(SOLIDARC*)ptr_temp;
                    POINTD p1l, p2l;


                    if (pline_mode == PL_MODE_LINE)
                        pline_mode = PL_MODE_ARC;

                    //finding points 1 and 2 (base arc)
                    p1l.x = sa->x + sa->r * cos(sa->kat2);
                    p1l.y = sa->y + sa->r * sin(sa->kat2);
                    p2l.x = sa->x + sa->r * cos(sa->kat1);
                    p2l.y = sa->y + sa->r * sin(sa->kat1);

                    if (sa->reversed) {
                        s_trace.line.x1 = p1l.x;
                        s_trace.line.y1 = p1l.y;
                    }
                    else {
                        s_trace.line.x1 = p2l.x;
                        s_trace.line.y1 = p2l.y;
                    }

                    rysuj_obiekt((char *) ptr_temp, COPY_PUT, 0);
                    Usun_Object(ptr_temp, FALSE);
                }
            }
            else if (d_line == 1)  //2 lines
            {

                rysuj_obiekt((char*)&s_trace.line, COPY_PUT, 0);

                AfterTbreak = 0;
                ob_number_min = 3;
                found_break = FALSE;

                if (((LINIA *) ptr_temp)->obiektt2 == O2BlockSpecial) {
                    break_line = TRUE;
                    ptr_temp0 = ptr_temp;
                    ptr_temp = (char *) ptr_temp0 - ((LINIA *) ptr_temp0)->n - sizeof(NAGLOWEK);
                    ob_number_min++;
                    found_break = TRUE;
                }
                if (((LINIA *) ptr_temp)->obiektt2 == O2BlockSpecial) {
                    break_line2 = TRUE;
                    ptr_temp02 = ptr_temp;
                    ptr_temp = (char *) ptr_temp02 - ((LINIA *) ptr_temp02)->n - sizeof(NAGLOWEK);
                    ob_number_min++;
                    found_break = TRUE;
                }

                if ((found_break == TRUE) && (ob_number > ob_number_min)) AfterTbreak = 1;

                ptr_temp1 = (char *) ptr_temp - ((LINIA *) ptr_temp)->n - sizeof(NAGLOWEK);

                s_trace.solid.xy[0] = ((LINIA *) ptr_temp)->x2;
                s_trace.solid.xy[1] = ((LINIA *) ptr_temp)->y2;
                s_trace.solid.xy[2] = ((LINIA *) ptr_temp1)->x2;
                s_trace.solid.xy[3] = ((LINIA *) ptr_temp1)->y2;

                s_trace.solid.xy[4] = ((LINIA *) ptr_temp)->x1;
                s_trace.solid.xy[5] = ((LINIA *) ptr_temp)->y1;
                s_trace.solid.xy[6] = ((LINIA *) ptr_temp1)->x1;
                s_trace.solid.xy[7] = ((LINIA *) ptr_temp1)->y1;

                s_trace.line.x1 = (((LINIA *) ptr_temp)->x1 + ((LINIA *) ptr_temp1)->x1) / 2.0;
                s_trace.line.y1 = (((LINIA *) ptr_temp)->y1 + ((LINIA *) ptr_temp1)->y1) / 2.0;
                s_trace.line.x2 = (((LINIA *) ptr_temp)->x2 + ((LINIA *) ptr_temp1)->x2) / 2.0;
                s_trace.line.y2 = (((LINIA *) ptr_temp)->y2 + ((LINIA *) ptr_temp1)->y2) / 2.0;


                dx1 = s_trace.line.x2 - s_trace.line.x1;
                dy1 = s_trace.line.y2 - s_trace.line.y1;

                angle_r = Atan2(dy1, dx1);
                si_r = sin(angle_r);
                co_r = cos(angle_r);

                df_sx1 = 0.0;
                df_sy1 = s_trace.axis;

                obrd(si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1);

                s_trace.line.x1 += df_sx1;
                s_trace.line.y1 += df_sy1;

                s_trace.line.x2 += df_sx1;
                s_trace.line.y2 += df_sy1;


                //intersection with first line
                s_line2.x1 = ((LINIA *) ptr_temp)->x1;
                s_line2.y1 = ((LINIA *) ptr_temp)->y1;
                s_line2.x2 = ((LINIA *) ptr_temp1)->x1;
                s_line2.y2 = ((LINIA *) ptr_temp1)->y1;
                inter1 = przeciecieLL_tt(&x1, &y1, &s_trace.line, &s_line2, &t1, &t2);

                //intersection with second line
                s_line2.x1 = ((LINIA *) ptr_temp)->x2;
                s_line2.y1 = ((LINIA *) ptr_temp)->y2;
                s_line2.x2 = ((LINIA *) ptr_temp1)->x2;
                s_line2.y2 = ((LINIA *) ptr_temp1)->y2;
                inter2 = przeciecieLL_tt(&x2, &y2, &s_trace.line, &s_line2, &t1, &t2);

                if (inter1 != 0) {
                    s_trace.line.x1 = x1;
                    s_trace.line.y1 = y1;
                }

                if (inter2 != 0) {
                    s_trace.line.x2 = x2;
                    s_trace.line.y2 = y2;
                }


                if (break_line) tail0 = dane + dane_size - (char *) ptr_temp0;
                if (break_line2) tail02 = dane + dane_size - (char *) ptr_temp02;
                tail = dane + dane_size - (char *) ptr_temp;

                if (break_line) rysuj_obiekt((char *) ptr_temp0, COPY_PUT, 0);
                if (break_line2) rysuj_obiekt((char *) ptr_temp02, COPY_PUT, 0);
                rysuj_obiekt((char *) ptr_temp, COPY_PUT, 0);
                rysuj_obiekt((char *) ptr_temp1, COPY_PUT, 0);


                Usun_Object(ptr_temp1, FALSE);
                ptr_temp = dane + dane_size - tail;
                Usun_Object(ptr_temp, FALSE);
                if (break_line2) {
                    ptr_temp02 = dane + dane_size - tail02;
                    Usun_Object(ptr_temp02, FALSE);
                }
                if (break_line) {
                    ptr_temp0 = dane + dane_size - tail0;
                    Usun_Object(ptr_temp0, FALSE);
                }

                //correction of first objects
                if (ptr_archive_set) {
                    if (NULL != (ptr_temp = Get_Trace_First_Ob((void *) dane))) {
                        ptr_temp1 = (char *) ptr_temp + ((LINIA *) ptr_temp)->n + sizeof(NAGLOWEK);

                        rysuj_obiekt(ptr_temp, COPY_PUT, 0);
                        rysuj_obiekt(ptr_temp1, COPY_PUT, 0);

                        memcpy((LINIA *) ptr_temp, &ptr_archive_line, sizeof(LINIA));
                        memcpy((LINIA *) ptr_temp1, &ptr_archive_line1, sizeof(LINIA));

                        rysuj_obiekt(ptr_temp, COPY_PUT, 1);
                        rysuj_obiekt(ptr_temp1, COPY_PUT, 1);
                    }
                    ptr_archive_set = FALSE;
                }

            } else if (d_line == 2) //3 linia
            {

                rysuj_obiekt((char*)&s_trace.line, COPY_PUT, 0);

                AfterTbreak = 0;
                ob_number_min = 4;
                found_break = FALSE;

                if (((LINIA *) ptr_temp)->obiektt2 == O2BlockSpecial) {
                    break_line = TRUE;
                    ptr_temp0 = ptr_temp;
                    ptr_temp = (char *) ptr_temp0 - ((LINIA *) ptr_temp0)->n - sizeof(NAGLOWEK);
                    ob_number_min++;
                    found_break = TRUE;
                }
                if (((LINIA *) ptr_temp)->obiektt2 == O2BlockSpecial) {
                    break_line2 = TRUE;
                    ptr_temp02 = ptr_temp;
                    ptr_temp = (char *) ptr_temp02 - ((LINIA *) ptr_temp02)->n - sizeof(NAGLOWEK);
                    ob_number_min++;
                    found_break = TRUE;
                }

                if ((found_break == TRUE) && (ob_number > ob_number_min)) AfterTbreak = 1;

                ptr_temp1 = (char *) ptr_temp - ((LINIA *) ptr_temp)->n - sizeof(NAGLOWEK);
                ptr_temp2 = (char *) ptr_temp1 - ((LINIA *) ptr_temp1)->n - sizeof(NAGLOWEK);

                s_trace.solid.xy[0] = ((LINIA *) ptr_temp)->x2;
                s_trace.solid.xy[1] = ((LINIA *) ptr_temp)->y2;
                s_trace.solid.xy[2] = ((LINIA *) ptr_temp1)->x2;
                s_trace.solid.xy[3] = ((LINIA *) ptr_temp1)->y2;

                s_trace.solid.xy[4] = ((LINIA *) ptr_temp)->x1;
                s_trace.solid.xy[5] = ((LINIA *) ptr_temp)->y1;
                s_trace.solid.xy[6] = ((LINIA *) ptr_temp1)->x1;
                s_trace.solid.xy[7] = ((LINIA *) ptr_temp1)->y1;

                s_trace.line.x1 = (((LINIA *) ptr_temp)->x1 + ((LINIA *) ptr_temp1)->x1) / 2.0;
                s_trace.line.y1 = (((LINIA *) ptr_temp)->y1 + ((LINIA *) ptr_temp1)->y1) / 2.0;
                s_trace.line.x2 = (((LINIA *) ptr_temp)->x2 + ((LINIA *) ptr_temp1)->x2) / 2.0;
                s_trace.line.y2 = (((LINIA *) ptr_temp)->y2 + ((LINIA *) ptr_temp1)->y2) / 2.0;

                dx1 = s_trace.line.x2 - s_trace.line.x1;
                dy1 = s_trace.line.y2 - s_trace.line.y1;

                angle_r = Atan2(dy1, dx1);
                si_r = sin(angle_r);
                co_r = cos(angle_r);

                df_sx1 = 0.0;
                df_sy1 = s_trace.axis;

                obrd(si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1);

                s_trace.line.x1 += df_sx1;
                s_trace.line.y1 += df_sy1;

                s_trace.line.x2 += df_sx1;
                s_trace.line.y2 += df_sy1;

                //intersection with first line
                s_line2.x1 = ((LINIA *) ptr_temp)->x1;
                s_line2.y1 = ((LINIA *) ptr_temp)->y1;
                s_line2.x2 = ((LINIA *) ptr_temp1)->x1;
                s_line2.y2 = ((LINIA *) ptr_temp1)->y1;
                inter1 = przeciecieLL_tt(&x1, &y1, &s_trace.line, &s_line2, &t1, &t2);

                //intersection with second line
                s_line2.x1 = ((LINIA *) ptr_temp)->x2;
                s_line2.y1 = ((LINIA *) ptr_temp)->y2;
                s_line2.x2 = ((LINIA *) ptr_temp1)->x2;
                s_line2.y2 = ((LINIA *) ptr_temp1)->y2;
                inter2 = przeciecieLL_tt(&x2, &y2, &s_trace.line, &s_line2, &t1, &t2);

                if (inter1 != 0) {
                    s_trace.line.x1 = x1;
                    s_trace.line.y1 = y1;
                }

                if (inter2 != 0) {
                    s_trace.line.x2 = x2;
                    s_trace.line.y2 = y2;
                }

                if (break_line) tail0 = dane + dane_size - (char *) ptr_temp0;
                if (break_line2) tail02 = dane + dane_size - (char *) ptr_temp02;
                tail = dane + dane_size - (char *) ptr_temp;
                tail1 = dane + dane_size - (char *) ptr_temp1;

                if (break_line) rysuj_obiekt((char *) ptr_temp0, COPY_PUT, 0);
                if (break_line2) rysuj_obiekt((char *) ptr_temp02, COPY_PUT, 0);
                rysuj_obiekt((char *) ptr_temp, COPY_PUT, 0);
                rysuj_obiekt((char *) ptr_temp1, COPY_PUT, 0);
                rysuj_obiekt((char *) ptr_temp2, COPY_PUT, 0);

                Usun_Object(ptr_temp2, FALSE);
                ptr_temp1 = dane + dane_size - tail1;
                Usun_Object(ptr_temp1, FALSE);
                ptr_temp = dane + dane_size - tail;
                Usun_Object(ptr_temp, FALSE);
                if (break_line2) {
                    ptr_temp02 = dane + dane_size - tail02;
                    Usun_Object(ptr_temp02, FALSE);
                }
                if (break_line) {
                    ptr_temp0 = dane + dane_size - tail0;
                    Usun_Object(ptr_temp0, FALSE);
                }

                //correction of first objects
                if (ptr_archive_set) {
                    if (NULL != (ptr_temp = Get_Trace_First_Ob((void *) dane))) {
                        ptr_temp1 = (char *) ptr_temp + ((LINIA *) ptr_temp)->n + sizeof(NAGLOWEK);
                        ptr_temp2 = (char *) ptr_temp1 + ((LINIA *) ptr_temp1)->n + sizeof(NAGLOWEK);

                        rysuj_obiekt(ptr_temp, COPY_PUT, 0);
                        rysuj_obiekt(ptr_temp1, COPY_PUT, 0);
                        rysuj_obiekt(ptr_temp2, COPY_PUT, 0);

                        memcpy((LINIA *) ptr_temp, &ptr_archive_line, sizeof(LINIA));
                        memcpy((LINIA *) ptr_temp1, &ptr_archive_line1, sizeof(LINIA));
                        memcpy((LINIA *) ptr_temp2, &ptr_archive_line2, sizeof(LINIA));

                        rysuj_obiekt(ptr_temp, COPY_PUT, 1);
                        rysuj_obiekt(ptr_temp1, COPY_PUT, 1);
                        rysuj_obiekt(ptr_temp2, COPY_PUT, 1);
                    }
                    ptr_archive_set = FALSE;
                }
            }

       Get_End_Trace((void*)dane, &ptr_ob, &b_first_end,
            df__xbeg, df__ybeg, &df_xend, &df_yend);

        if (ptr_ob!=NULL)
        {
             if (((NAGLOWEK*)ptr_ob)->obiekt==Owwielokat)
             {
                s_trace.b_line = TRUE;
            }
            else if ((ptr_ob!=NULL) && (((NAGLOWEK*)ptr_ob)->obiekt==Osolidarc))
            {
              if (b_ptr_before_last==Owwielokat) s_trace.b_line = TRUE;
              else s_trace.b_line = FALSE;

                if (pline_mode == PL_MODE_LINE)
                    pline_mode = PL_MODE_ARC;

                s_trace.b_solid = FALSE;
                s_trace.b_line=0; ////

                SOLIDARC *sa;
                sa = (SOLIDARC *) ptr_ob;
                POINTD p1l, p2l;

                //finding points 1 and 2 (base arc)
                p1l.x = sa->x + sa->r * cos(sa->kat2);
                p1l.y = sa->y + sa->r * sin(sa->kat2);
                p2l.x = sa->x + sa->r * cos(sa->kat1);
                p2l.y = sa->y + sa->r * sin(sa->kat1);

                if (sa->reversed) {
                    s_trace.line.x2 = p2l.x;
                    s_trace.line.y2 = p2l.y;
                } else {
                    s_trace.line.x2 = p1l.x;
                    s_trace.line.y2 = p1l.y;
                }
            }
        }

        line_g.x1 = s_trace.line.x2; // df_xend;
        line_g.y1 = s_trace.line.y2; // df_yend;
        line_g.x2 = X;
        line_g.y2 = Y;

        if (catch_second_end==TRUE)
        {
            memmove(&line_g_bak,&line_g, sizeof(line_g));
        }

        LiniaG.x1 = line_g.x1;
        LiniaG.y1 = line_g.y1;
        LiniaG.x2 = line_g.x2;
        LiniaG.y2 = line_g.y2;

        //get information about previous solid
        if ((NULL != (ptr_temp = Get_Trace_Last_Ob((void*)dane, &ob_number))) && (!catch_second_end))
        {
            s_trace.b_solid = FALSE;

            if (d_line == 0)
            {
                if (((NAGLOWEK*)ptr_temp)->obiekt==Owwielokat)
                {

                    memcpy(&s_trace.solid, ptr_temp, sizeof(s_trace.solid));

                    s_line.x1 = (s_trace.solid.xy[0] + s_trace.solid.xy[2]) / 2.0;
                    s_line.y1 = (s_trace.solid.xy[1] + s_trace.solid.xy[3]) / 2.0;
                    s_line.x2 = (s_trace.solid.xy[4] + s_trace.solid.xy[6]) / 2.0;
                    s_line.y2 = (s_trace.solid.xy[5] + s_trace.solid.xy[7]) / 2.0;

                    dx1 = s_line.x2 - s_line.x1;
                    dy1 = s_line.y2 - s_line.y1;

                    angle_r = Atan2(dy1, dx1);
                    si_r = sin(angle_r);
                    co_r = cos(angle_r);

                    df_sx1 = 0.0;
                    df_sy1 = s_trace.axis;

                    obrd(si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1);

                    s_line.x1 += df_sx1;
                    s_line.y1 += df_sy1;

                    s_line.x2 += df_sx1;
                    s_line.y2 += df_sy1;

                    //intersection with first line
                    s_line2.x1 = s_trace.solid.xy[0];
                    s_line2.y1 = s_trace.solid.xy[1];
                    s_line2.x2 = s_trace.solid.xy[2];
                    s_line2.y2 = s_trace.solid.xy[3];
                    inter1 = przeciecieLL_tt(&x1, &y1, &s_line, &s_line2, &t1, &t2);

                    //intersection with second line
                    s_line2.x1 = s_trace.solid.xy[4];
                    s_line2.y1 = s_trace.solid.xy[5];
                    s_line2.x2 = s_trace.solid.xy[6];
                    s_line2.y2 = s_trace.solid.xy[7];
                    inter2 = przeciecieLL_tt(&x2, &y2, &s_line, &s_line2, &t1, &t2);

                    if (inter1 != 0) {
                        s_line.x1 = x1;
                        s_line.y1 = y1;
                    }

                    if (inter2 != 0) {
                        s_line.x2 = x2;
                        s_line.y2 = y2;
                    }
                }
                else if (((NAGLOWEK*)ptr_temp)->obiekt==Osolidarc)
                {
                    SOLIDARC *sa;
                    sa = (SOLIDARC *) ptr_temp;
                    POINTD p1l, p2l;

                    //finding points 1 and 2 (base arc)
                    p1l.x = sa->x + sa->r * cos(sa->kat2);
                    p1l.y = sa->y + sa->r * sin(sa->kat2);
                    p2l.x = sa->x + sa->r * cos(sa->kat1);
                    p2l.y = sa->y + sa->r * sin(sa->kat1);

                    if (sa->reversed) {
                        s_line.x1 = p1l.x;
                        s_line.y1 = p1l.y;
                        s_line.x2 = p2l.x;
                        s_line.y2 = p2l.y;
                    } else {
                        s_line.x1 = p2l.x;
                        s_line.y1 = p2l.y;
                        s_line.x2 = p1l.x;
                        s_line.y2 = p1l.y;
                    }
                }
            }
            else if (d_line == 1)  //2 lines
            {
                if (((LINIA *) ptr_temp)->obiektt2 == O2BlockSpecial) {
                    break_line = TRUE;
                    ptr_temp0 = ptr_temp;
                    ptr_temp = (char *) ptr_temp0 - ((LINIA *) ptr_temp0)->n - sizeof(NAGLOWEK);
                }
                if (((LINIA *) ptr_temp)->obiektt2 == O2BlockSpecial) {
                    break_line2 = TRUE;
                    ptr_temp02 = ptr_temp;
                    ptr_temp = (char *) ptr_temp02 - ((LINIA *) ptr_temp02)->n - sizeof(NAGLOWEK);
                }

                ptr_temp1 = (char *) ptr_temp - ((LINIA *) ptr_temp)->n - sizeof(NAGLOWEK);

                s_trace.solid.xy[0] = ((LINIA *) ptr_temp)->x1;
                s_trace.solid.xy[1] = ((LINIA *) ptr_temp)->y1;
                s_trace.solid.xy[2] = ((LINIA *) ptr_temp1)->x1;
                s_trace.solid.xy[3] = ((LINIA *) ptr_temp1)->y1;

                s_trace.solid.xy[4] = ((LINIA *) ptr_temp)->x2;
                s_trace.solid.xy[5] = ((LINIA *) ptr_temp)->y2;
                s_trace.solid.xy[6] = ((LINIA *) ptr_temp1)->x2;
                s_trace.solid.xy[7] = ((LINIA *) ptr_temp1)->y2;

                s_line.x1 = (((LINIA *) ptr_temp)->x1 + ((LINIA *) ptr_temp1)->x1) / 2.0;
                s_line.y1 = (((LINIA *) ptr_temp)->y1 + ((LINIA *) ptr_temp1)->y1) / 2.0;
                s_line.x2 = (((LINIA *) ptr_temp)->x2 + ((LINIA *) ptr_temp1)->x2) / 2.0;
                s_line.y2 = (((LINIA *) ptr_temp)->y2 + ((LINIA *) ptr_temp1)->y2) / 2.0;

                dx1 = s_line.x2 - s_line.x1;
                dy1 = s_line.y2 - s_line.y1;

                angle_r = Atan2(dy1, dx1);
                si_r = sin(angle_r);
                co_r = cos(angle_r);

                df_sx1 = 0.0;
                df_sy1 = s_trace.axis;

                obrd(si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1);

                s_line.x1 += df_sx1;
                s_line.y1 += df_sy1;
                s_line.x2 += df_sx1;
                s_line.y2 += df_sy1;

                //intersection with first line
                s_line2.x1 = s_trace.solid.xy[0];
                s_line2.y1 = s_trace.solid.xy[1];
                s_line2.x2 = s_trace.solid.xy[2];
                s_line2.y2 = s_trace.solid.xy[3];
                inter1 = przeciecieLL_tt(&x1, &y1, &s_line, &s_line2, &t1, &t2);

                //intersection with second line
                s_line2.x1 = s_trace.solid.xy[4];
                s_line2.y1 = s_trace.solid.xy[5];
                s_line2.x2 = s_trace.solid.xy[6];
                s_line2.y2 = s_trace.solid.xy[7];
                inter2 = przeciecieLL_tt(&x2, &y2, &s_line, &s_line2, &t1, &t2);

                if (inter1 != 0) {
                    s_line.x1 = x1;
                    s_line.y1 = y1;
                }

                if (inter2 != 0) {
                    s_line.x2 = x2;
                    s_line.y2 = y2;
                }

            } else if (d_line == 2) //3 linia
            {
                if (((LINIA *) ptr_temp)->obiektt2 == O2BlockSpecial) {
                    break_line = TRUE;
                    ptr_temp0 = ptr_temp;
                    ptr_temp = (char *) ptr_temp0 - ((LINIA *) ptr_temp0)->n - sizeof(NAGLOWEK);
                }
                if (((LINIA *) ptr_temp)->obiektt2 == O2BlockSpecial) {
                    break_line2 = TRUE;
                    ptr_temp02 = ptr_temp;
                    ptr_temp = (char *) ptr_temp02 - ((LINIA *) ptr_temp02)->n - sizeof(NAGLOWEK);
                }

                ptr_temp1 = (char *) ptr_temp - ((LINIA *) ptr_temp)->n - sizeof(NAGLOWEK);
                ptr_temp2 = (char *) ptr_temp1 - ((LINIA *) ptr_temp1)->n - sizeof(NAGLOWEK);

                s_trace.solid.xy[0] = ((LINIA *) ptr_temp)->x1;
                s_trace.solid.xy[1] = ((LINIA *) ptr_temp)->y1;
                s_trace.solid.xy[2] = ((LINIA *) ptr_temp1)->x1;
                s_trace.solid.xy[3] = ((LINIA *) ptr_temp1)->y1;

                s_trace.solid.xy[4] = ((LINIA *) ptr_temp)->x2;
                s_trace.solid.xy[5] = ((LINIA *) ptr_temp)->y2;
                s_trace.solid.xy[6] = ((LINIA *) ptr_temp1)->x2;
                s_trace.solid.xy[7] = ((LINIA *) ptr_temp1)->y2;

                s_line.x1 = (((LINIA *) ptr_temp)->x1 + ((LINIA *) ptr_temp1)->x1) / 2.0;
                s_line.y1 = (((LINIA *) ptr_temp)->y1 + ((LINIA *) ptr_temp1)->y1) / 2.0;
                s_line.x2 = (((LINIA *) ptr_temp)->x2 + ((LINIA *) ptr_temp1)->x2) / 2.0;
                s_line.y2 = (((LINIA *) ptr_temp)->y2 + ((LINIA *) ptr_temp1)->y2) / 2.0;

                dx1 = s_line.x2 - s_line.x1;
                dy1 = s_line.y2 - s_line.y1;

                angle_r = Atan2(dy1, dx1);
                si_r = sin(angle_r);
                co_r = cos(angle_r);

                df_sx1 = 0.0;
                df_sy1 = s_trace.axis;

                obrd(si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1);

                s_line.x1 += df_sx1;
                s_line.y1 += df_sy1;
                s_line.x2 += df_sx1;
                s_line.y2 += df_sy1;

                //intersection with first line
                s_line2.x1 = s_trace.solid.xy[0];
                s_line2.y1 = s_trace.solid.xy[1];
                s_line2.x2 = s_trace.solid.xy[2];
                s_line2.y2 = s_trace.solid.xy[3];
                inter1 = przeciecieLL_tt(&x1, &y1, &s_line, &s_line2, &t1, &t2);

                //intersection with second line
                s_line2.x1 = s_trace.solid.xy[4];
                s_line2.y1 = s_trace.solid.xy[5];
                s_line2.x2 = s_trace.solid.xy[6];
                s_line2.y2 = s_trace.solid.xy[7];
                inter2 = przeciecieLL_tt(&x2, &y2, &s_line, &s_line2, &t1, &t2);

                if (inter1 != 0) {
                    s_line.x1 = x1;
                    s_line.y1 = y1;
                }

                if (inter2 != 0) {
                    s_line.x2 = x2;
                    s_line.y2 = y2;
                }
            }

            if (Check_if_Equal(s_trace.line.x1, s_line.x2) && Check_if_Equal(s_trace.line.y1, s_line.y2))
                s_trace.b_solid = TRUE;

        }
        else s_trace.b_solid = FALSE;

        CUR_OFF(X, Y);
        CUR_ON(X, Y);
    }

    if (b_ptr_before_last)
    {
       b_ptr_before_last=0;
    }

    if (catch_second_end==TRUE)
    {
        memmove(&line_g,&line_g_bak, sizeof(line_g));
    }

    CUR_OFF(X, Y);
    CUR_ON(X, Y);
    return -83;
}

int last_trace_delete(void)
{
    if (trace_angle_changed)
    {
        change_angle_l(trace_angle_orig);
        orto=trace_orto_orig;
        trace_angle_changed=FALSE;
    }
    return last_trace_delete_factory();
}

static void  nooop(void)
{}

static void redcr (char typ)
/*--------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int KR0, np ;
  static void (*REDdod)(void)=nooop;
  static int (*SW[5])();

  switch (typ)
  {
    case 0 :
      KR0 = Komunikat_R0 ;
      if ( d_line == 0 )
       {
       komunikat0 (79);
       }
       else if (d_line == 1 )
       {
       komunikat0 (101);
       }
       else
       {
       komunikat0 (105);
       }
	/*komunikat niszczy pole edycji*/

      CUR_OFF (X, Y) ;
      CUR_oFF = CUR_OFF ;  CUR_OFF = cur_off ;
      CUR_oN = CUR_ON ;    CUR_ON = cur_on ;
      poczL = 1 ;

      usunstr(np_trace_width);
      np = dodajstr (&eL) ;

      e_trace_width.x = maxX - PL366;
      e_trace_width.y= ESTR_Y;
      e_trace_width.lmax = r12; // r16;
      e_trace_width.val_no_max = 2 ;
      e_trace_width.mode = GV_DIST ;
      e_trace_width.format = "%-8.5lg\0%-8.5lg;%-8.5lg" ;
      e_trace_width.ESTRF = edit_width_trace ;
      e_trace_width.extend = 0;
      if (-1 != (np_trace_width = dodajstr(&e_trace_width)))
      {
          if (s_trace.axis==0)
          {
              sprintf (e_trace_width.st, "%-6.5lg", milimetryob (s_trace.width)) ;
              e_trace_width.val_no = 1;
          }
          else
          {
              sprintf (e_trace_width.st, "%-6.5lg;%-6.5lg", milimetryob (s_trace.width), milimetryob(s_trace.axis)) ;
              e_trace_width.val_no = 2;
          }
          Out_Edited_Draw_Param ((ESTR *)&e_trace_width, TRUE) ;
      }

      set_np(2);

      SW[4] = SERV[83];  SERV[83] = last_trace_delete;
      SW[3] = SERV[71];  SERV[71] = break_trace;  //Home
      SW[2] = SERV[79];  SERV[79] = close_trace;  //End

     
      CUR_ON (X, Y) ;
      s_trace.b_line = FALSE ;
      s_trace.b_solid = FALSE ;
      s_trace.width1 = s_trace.width ;
      s_trace.width0 = s_trace.width;
      s_trace.axis1 = s_trace.axis;  //This is for solidarc purpose, which is drawed after the solid
      REDdod = REDDOD ; REDDOD = redraw_trace ;
      break ;
    case 1 :
    case 2 :
      CUR_OFF = CUR_oFF ;
      CUR_ON = CUR_oN ;
      REDDOD=REDdod;
      SERV[83] = SW[4];
      SERV[71]= SW[3];
      SERV[79] = SW[2];

      CUR_ON (X, Y) ;
      poczL = 0 ;
      komunikat0 (KR0) ;
      Out_Edited_Draw_Param ((ESTR *)&e_trace_width, TRUE) ;
      //usunstr(np_trace_width);
      usunstr (np) ;
      if (np<np_trace_width) np_trace_width--;
      break ;
      case 3 :
          CUR_OFF = CUR_oFF ;
          CUR_ON = CUR_oN ;
          REDDOD=REDdod;
          SERV[83] = SW[4];
          SERV[71]= SW[3];
          SERV[79] = SW[2];

          CUR_ON (X, Y) ;
          poczL = 0 ;
          komunikat0 (KR0) ;
          Out_Edited_Draw_Param ((ESTR *)&e_trace_width, TRUE) ;
          usunstr(np_trace_width);
          if (np_trace_width<np) np--;
          //usunstr (np) ;
      break ;
      case 4 :
          CUR_OFF = CUR_oFF ;
          CUR_ON = CUR_oN ;
          REDDOD=REDdod;
          SERV[83] = SW[4];
          SERV[71]= SW[3];
          SERV[79] = SW[2];

          CUR_ON (X, Y) ;
          poczL = 0 ;
          komunikat0 (KR0) ;
          Out_Edited_Draw_Param ((ESTR *)&e_trace_width, TRUE) ;
          usunstr(np_trace_width);
          usunstr (np) ;
          break ;
  }
}

int close_trace(void)
{
    CUR_OFF(X, Y);
    if (add_trace_line_close())
    {
        Tbreak = 0;
        komunikat_str_short("", FALSE, TRUE);
        remove_short_notice();
        block_added = FALSE;
        redcr(1);
        return -183;
    }
    CUR_ON(X, Y);
    return 0;
}

int break_trace(void)
{
    Tbreak = 1;
    komunikat_str_short(_BREAK_, TRUE, TRUE);
    if (pline_mode==PL_MODE_LINE) {
        if (d_line == 0) {
            add_trace_line_end();
        } else if (d_line == 1) {
            add_2line_end(0);
        } else {
            add_2line_end(1);
        }
    }
    else
    {
        ;
    }
    CUR_OFF(X, Y);
    CUR_ON(X, Y);
    return 0;
}

int start_trace (double X0, double Y0, BOOL delete_last, BOOL join_last, BOOL continue_last)
/*----------------------------------------------------------------------------------------*/
{
  EVENT *ev;
  int ret;
  int ev_Number;
  void *ptr_ob;
  BOOL b_first_end;
  double df_xend, df_yend;
  int pline_mode_bak;
  t_trace s_trace_bak;

  if (!delete_last) ptr_archive_set = FALSE;

  if (join_last)
  {
      Get_End_Trace((void *) dane, &ptr_ob, &b_first_end, df__xbeg, df__ybeg, &df_xend, &df_yend);
      line_g.x1 = df_xend;
      line_g.y1 = df_yend;
  }
  else if (continue_last)
  {
     //protecting s_trace
     memmove(&s_trace_bak, &s_trace, sizeof(s_trace));
  }
  else {
      line_g.x1 = X0;
      line_g.y1 = Y0;
  }

  line_g.x2 = X ;
  line_g.y2 = Y ;

  LiniaG.x1=line_g.x1;
  LiniaG.y1=line_g.y1;
  LiniaG.x2=line_g.x2;
  LiniaG.y2=line_g.y2;


  Orto_Dir = I_Orto_NoDir ;
  if (d_t__pline==TRUE) 
  {
   if (block_added==FALSE)
   { 
    if (FALSE == add_block ())
     {
      return 0;
     }
    block_added=TRUE;
   }
  } 
  redcr (0) ;

  flip_screen();

  if (continue_last)
  {
      //restoring s_trace
      memmove(&s_trace, &s_trace_bak, sizeof(s_trace));
  }

  if (delete_last) ret = last_trace_delete();
  while (1)
  {
    strwyj = 0 ;
    ev = Get_Event_Point (NULL, &X0, &Y0) ;

    if (strwyj == 1)
    {
       goto aaa ;
    }

    switch (ev->What)
    {
    case evNothing:
        if (ev->Number == -183) 
            return 0;
        if (ev -> Number == -83)
            {
                if (pline_mode == PL_MODE_ARC) return 3;
            }
        break;
      case evKeyDown :
	 if(ev->Number == 0)
	 {
	   CUR_OFF (X,Y) ;
	   if ( d_line == 0 )
	    {
	     add_trace_line_end () ;
	     }
	       else if ( d_line == 1 )
		 {
		 add_2line_end (0) ;
		 }
		 else
		 {
		 add_2line_end (1) ;
		 }
         Tbreak = 0;
         komunikat_str_short("", FALSE, TRUE);
         remove_short_notice();

       s_trace.b_line = FALSE;
	   redcr (1) ;

       if (trace_angle_changed)
       {
         change_angle_l(trace_angle_orig);
         orto=trace_orto_orig;
         trace_angle_changed=FALSE;
       }
	   return 0;
	 }
aaa:	 if (ev->Number == ENTER || strwyj)
	 {
	   CUR_OFF (X,Y) ;

	   if ( d_line == 0 )
	     {
          add_trace_line(strwyj) ;
	      }
        else if (d_line == 1)
		{
		add_2line (strwyj,0) ;
		}
		else
		{
		add_2line (strwyj,1) ;
		}
        if (Tbreak) {
            Tbreak = 0;
            komunikat_str_short("", FALSE, TRUE);
            remove_short_notice();
        }

        if (trace_angle_changed)
         {
             change_angle_l(trace_angle_orig);
             orto=trace_orto_orig;
             trace_angle_changed=FALSE;
         }
	   CUR_ON (X,Y) ;
	   break;
	 }
      case evCommandP :
          if (ev->Number>-1) {
              if (ev->Number <= 6) ev_Number = com_no[d_line][ev->Number];
              else ev_Number = ev->Number;
              pline_mode_bak = pline_mode;
              ret = trace_command(ev_Number, X0, Y0, strwyj);
              if ((pline_mode_bak == PL_MODE_LINE) && (pline_mode == PL_MODE_LINE_CONTINUE)) pline_mode = PL_MODE_LINE;

              if (pline_mode_bak != pline_mode)
                  if (ret) {
                      s_trace.b_line = FALSE;
                      //if (pline_mode_bak != pline_mode)
                      return ret;
                  }
          }
	 break ;
      default :
	 break ;
    }
  }

  return 0;
}

static void  cur_off(double x,double y)
{
    flip_screen();
}

static void  cur_on(double x,double y)
{ line_g.x2=x; line_g.y2=y;
  line_g.kolor = Get_Current_Color () ;
   redraw_trace();
  outlineor(&line_g,COPY_PUT,1);
  cursor_on(x, y);
}

static void redcr0(char typ)
/*--------------------------*/
{
  static void (*CUR_oN)(double ,double) ;
  static void (*CUR_oFF)(double ,double) ;
  static int ( *SW[3])(), akt, cur ;
  static TMENU *menu ;
  static char st[32];
  unsigned short cod = L' ';
  char sz_temp [MaxTextLen] ;
  int iconno;
  static char sk[16];
  int d_trans = 12;
  int transluc;

  iconno = 0;

  if (typ==0)
   {
	 CUR_OFF(X, Y);

     komunikat(0);
     if ( d_line == 0 )
     {
     komunikat0 (78);
     } else if ( d_line == 1 )
     {
     komunikat0 (100) ;
     }
     else
     {
     komunikat0 (104) ;
     }
     akt=sel.akt; sel.akt=ASel;
     cur =sel.cur ; sel.cur = 0 ;
     cursor_off= cursel_off;
	 cursor_on= cursel_on;
     
     s_trace.width = df__Trace_Width ;
     sz_temp [0] = '\0' ;
     sprintf (sz_temp, "%lg", milimetryob (s_trace.width)) ;
     if (strlen (sz_temp) > 7) sz_temp [7] = '\0' ;
     menu_par_new ((*mTrace.pola)[0].txt, sz_temp) ;
     
     s_trace.axis = df__Trace_Axis ;
     sz_temp [0] = '\0' ;
     sprintf (sz_temp, "%lg", milimetryob (s_trace.axis)) ;
     if (strlen (sz_temp) > 7) sz_temp [7] = '\0' ;
     menu_par_new ((*mTrace.pola)[1].txt, sz_temp) ;


     if (Get_reversed()) strcpy(st,YES);
     else strcpy(st,NO);
     menu_par_new ((*mPTLukm.pola)[8].txt, YES);

     e_trace_width.x = maxX - PL366;
     e_trace_width.y= ESTR_Y;
     e_trace_width.lmax = r12; // r16;
     e_trace_width.val_no_max = 2 ;
     e_trace_width.mode = GV_DIST ;
     e_trace_width.format = "%-8.5lg\0%-8.5lg;%-8.5lg" ;
     e_trace_width.ESTRF = edit_width_trace ;
	 e_trace_width.extend = 0;
     if (-1 != (np_trace_width = dodajstr(&e_trace_width)))
     {
      if (s_trace.axis==0)
       {
        sprintf (e_trace_width.st, "%-6.5lg", milimetryob (s_trace.width)) ;
        e_trace_width.val_no = 1;
       }
       else
         {
          sprintf (e_trace_width.st, "%-6.5lg;%-6.5lg", milimetryob (s_trace.width), milimetryob(s_trace.axis)) ;
          e_trace_width.val_no = 2;
         }
       Out_Edited_Draw_Param ((ESTR *)&e_trace_width, TRUE) ;
     }
     eL.x = maxX - PL266 ;
     eL.y= ESTR_Y;
     eL.val_no_max = 2;
     eL.mode = GV_VECTOR ;
     eL.format = "%#12.9lg\0%#12.9lg;%#8.3lg" ;
     eL.lmax=r22;
     eL.ESTRF=L_p;
	 eL.extend = 0;

     menu_par_new((*mTracePattern.pola)[0].txt, TracePatternName);
     sprintf(sk, "%5.3f", solidpatternscales[TracePatternScale]);
     menu_par_new((*mTracePattern.pola)[1].txt, sk);
     sprintf(sk, "%d", solidpatternangles[TracePatternAngle]);
     menu_par_new((*mTracePattern.pola)[2].txt, sk);

     transluc = (TraceTranslucency / d_trans) - 1;
     sprintf(sk, "%d%%", transluc * 5);
     menu_par_new((*mFillTyp.pola)[0].txt, sk);

     switch (d_line) {
         case 0:menupini(&mTrace, _TRACE_, _TRACE_C_, 23);
         break;
         case 1:menupini(&mTrace, _DLINE_, _DLINE_C_, 24);
         break;
         case 2:menupini(&mTrace, _TLINE_, _TLINE_C_, 25);
         break;
     }

     s_trace.solid.blok = ElemBlok ;
     s_trace.solid.obiektt2 = O2BlockPline ;
   
     CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;   CUR_ON=cursel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     CUR_ON(X,Y);
   }
  else if (typ==1)
   {
     CUR_OFF(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
     sel.akt=akt;
     sel.cur = cur ;
	 CUR_ON(X, Y);
     usunstr (np_trace_width) ;
     menupini (menu, st, ' ', iconno);
     df__Trace_Width = s_trace.width ;   /*ustawienie zmiennej globalnej*/
     df__Trace_Axis = s_trace.axis ;
     komunikat0 (0) ;
   }
  else if (typ==2)
  {
      CUR_OFF(X,Y);
      CUR_OFF=CUR_oFF;
      CUR_ON=CUR_oN;
      SERV[73]=SW[0];
      SERV[81]=SW[1];
      sel.akt=akt;
      sel.cur = cur ;
      CUR_ON(X, Y);
      df__Trace_Width = s_trace.width ;   /*ustawienie zmiennej globalnej*/
      df__Trace_Axis = s_trace.axis ;
      komunikat0 (0) ;
  }
}

double get_trace_width(int side)
{
switch (side)
    {
        case 1:
            return s_trace.width;
        break;
        case 2:
            return s_trace.width1;
        break;
    }
}

double get_trace_axis(int side) {
    switch (side) {
        case 1:
            return s_trace.axis;
            break;
        case 2:
            return s_trace.axis1;
            break;
    }
}

void update_trace_width(void)
{
    s_trace.width1 = s_trace.width;
    s_trace.axis1 = s_trace.axis;  //This is for solidarc purpose, which is drawed after the solid
}

int get_trace_blok(void)
{
    return s_trace.solid.blok;
}

int get_trace_obiektt2(void)
{
    return s_trace.solid.obiektt2;
}

int trace_command(int ev_Number, double X0, double Y0, int strwyj)
{
    int ob_number;
    int ret;

    switch (ev_Number)
    {
        case ID_WIDTH : //0
            width_trace () ;
            break ;
        case ID_AXIS :  //1
            axis_trace () ;
            break ;
        case ID_D_T_PLINE_ON :  //8
            d_t_pline_on ();
            break;
        case ID_D_T_PLINE_OFF :  //9
            d_t_pline_off ();
            break;
        case ID_ERASE : //4
             ret = last_trace_delete();
             if (pline_mode!=PL_MODE_LINE)
             {
                 pline_mode = PL_MODE_UNDO;
                 return pline_mode;
             }
            break ;
        case -83:
            return 1;
            break;
        case ID_CLOSE:  //6
            CUR_OFF(X, Y);
            if (add_trace_line_close())
            {
                block_added = FALSE;
                Tbreak = 0;
                komunikat_str_short("", FALSE, TRUE);
                remove_short_notice();
            }
            CUR_ON(X, Y);
            break;
        case ID_LINE_MODE:  //85
            pline_mode = PL_MODE_LINE ;
            return pline_mode;
            break;
        case ID_LINE_CONTINUE:  //86
            pline_mode = PL_MODE_LINE_CONTINUE ;
            return pline_mode;
            break;
        case ID_ARC_CONT:  //75
            add_trace_line_end();
            type_arc_pl (ARC_Con);
            pline_mode = PL_MODE_ARC ;
            return pline_mode;
            break;
        case ID_ARC_REV_Y:  //83
            Set_reversed(TRUE);
            menu_par_new ((*mPTLukm.pola)[8].txt, YES);
            return pline_mode;
            break;
        case ID_ARC_REV_N:  //84
            Set_reversed(FALSE);
            menu_par_new ((*mPTLukm.pola)[8].txt, NO);
            return pline_mode;
            break;
        case ID_ARC_P3:  //76
            add_trace_line_end();
            type_arc_pl (ARC_P3);
            pline_mode = PL_MODE_ARC ;
            return pline_mode;
            break;
        case ID_ARC_SCE:  //77
            add_trace_line_end();
            type_arc_pl (ARC_SCE);
            pline_mode = PL_MODE_ARC ;
            return pline_mode;
            break;
        case ID_ARC_SCA:  //78
            add_trace_line_end();
            type_arc_pl (ARC_SCA);
            pline_mode = PL_MODE_ARC ;
            return pline_mode;
            break;
        case ID_ARC_SCL:  //79
            add_trace_line_end();
            type_arc_pl (ARC_SCL);
            pline_mode = PL_MODE_ARC ;
            return pline_mode;
            break;
        case ID_ARC_SER:  //80
            add_trace_line_end();
            type_arc_pl (ARC_SER);
            pline_mode = PL_MODE_ARC ;
            return pline_mode;
            break;
        case ID_ARC_SEA:  //81
            add_trace_line_end();
            type_arc_pl (ARC_SEA);
            pline_mode = PL_MODE_ARC ;
            return pline_mode;
            break;
        case ID_ARC_SED:  //82
            add_trace_line_end();
            type_arc_pl (ARC_SED);
            pline_mode = PL_MODE_ARC ;
            return pline_mode;
            break;
        case ID_BREAK:  //5
            Tbreak = 1;
            komunikat_str_short(_BREAK_, TRUE, TRUE);
            if (pline_mode==PL_MODE_LINE) {
                if (d_line == 0) {
                    add_trace_line_end();
                } else if (d_line == 1) {
                    add_2line_end(0);
                } else {
                    add_2line_end(1);
                }
            }
            else  //PL_MODE_ARC
            {
                return PL_MODE_BREAK;
            }
            CUR_OFF(X, Y);
            CUR_ON(X, Y);
            break;
        case ID_FILLEMPTY:  //10
        case ID_FILL_BC:  //11
        case ID_FILL_C:  //12
        case ID_FILL_G:  //13
        case ID_FILL_BG:  //14
        case ID_FILL_NG:  //15
            set_tracefilltype(ev_Number);
            break;
        case ID_PATTERN:  //16
            set_tracepattern();
            komunikat0(Komunikat_R0);
            break;
        case ID_GRAB_PATTERN:  //21
            komunikat0(0);
            set_grab_tracepattern();
            komunikat0(Komunikat_R0);
            break;
        case ID_CHANGE_PATTERN:
            komunikat0(0);
            set_change_tracepattern();
            komunikat0(Komunikat_R0);
            break;
        case ID_PATTERN_SCALE:
        case ID_PATTERN_SCALE + 1:
        case ID_PATTERN_SCALE + 2:
        case ID_PATTERN_SCALE + 3:
        case ID_PATTERN_SCALE + 4:
        case ID_PATTERN_SCALE + 5:
            trace_image_pattern_scale(ev_Number);
            break;
        case ID_PATTERN_ANGLE:
        case ID_PATTERN_ANGLE + 1:
        case ID_PATTERN_ANGLE + 2:
        case ID_PATTERN_ANGLE + 3:
            trace_image_pattern_angle(ev_Number);
            break;
        default:
            if ((ev_Number >= ID_TRANSLUCENCY) && (ev_Number < ID_TRANSLUCENCY + 20))
                set_tracetranslucency(ev_Number);
            else if ((ev_Number >= ID_PATTERN_DX) && (ev_Number < (ID_PATTERN_DX + 10)))
                trace_image_pattern_dx(ev_Number);
            else if ((ev_Number >= ID_PATTERN_DY) && (ev_Number < (ID_PATTERN_DY + 10)))
                trace_image_pattern_dy(ev_Number);
            break;
    }
    return 0;
}

void Trace_Dline_Tline (void)
{
  EVENT *ev ;
  double X0, Y0, df_xbeg, df_ybeg, df_xend, df_yend ;
  int ret;
  int ob_number;
  int ev_Number;
  int pline_mode0 ;
  BOOL delete_last=FALSE;
  BOOL join_last=FALSE;
  BOOL continue_last=FALSE;
  double angle;
  BOOL beginning;

    void *ptr_ob ;
    BOOL b_first_end, b_second_pl_seg ;

  pline_trace=1; //trace

  s_trace.b_line=0;

  pline_mode = PL_MODE_LINE ;
  pline_mode0 = pline_mode;

  ptr_archive_set = FALSE;

  if (d_t__pline == TRUE)
   {
    s_trace.solid.blok = ElemBlok ;
	s_trace.solid.obiektt2 = O2BlockPline;
    LiniaG1.blok = ElemBlok ;
	LiniaG1.obiektt2 = O2BlockPline;
   }
   else
    { 
     s_trace.solid.blok = NoElemBlok ;
     s_trace.solid.obiektt2 = O2NoBlockS ; 
     LiniaG1.blok = NoElemBlok ;
     LiniaG1.obiektt2 = O2NoBlockS ;
    } 

  block_added=FALSE;
  mTrace.max=mTrace_min;
  while (1)
   {
      beginning=TRUE;
       redcr0 (0) ;
   beg0:
      ev = Get_Event_Point (NULL, &X0, &Y0) ;

       df_xbeg = X0 ;
       df_ybeg = Y0 ;

     switch (ev->What)
      {
    
	case evKeyDown  :
	   if (ev->Number == 0)
	   {
	     redcr0 (1) ;
	     return ;
	   }
	   if(ev->Number == ENTER)
	   {
           beginning=FALSE;
           //changing size of menu to enagle solidarc
           mTrace.max=mTrace_max;

   beg:
        if (pline_mode == PL_MODE_LINE_CONTINUE)
         {
           //saving existing coordinates system
           //define local coordinates system
           //record flag of changed coordinates system to return after next segment
           b_second_pl_seg = Get_End_Pline ((void*)dane, &ptr_ob, &b_first_end, df_xbeg, df_ybeg, &df_xend, &df_yend) ;
           if ((ptr_ob!=NULL) && (((NAGLOWEK*)ptr_ob)->obiekt==Osolidarc))
           {
               if (((SOLIDARC*)ptr_ob)->reversed) angle = ((SOLIDARC*)ptr_ob)->kat1+Pi_;
               else angle = ((SOLIDARC*)ptr_ob)->kat2+Pi_;
               angle=(angle*360)/Pi2;
               trace_angle_orig=get_angle_l();
               change_angle_l(angle);
               trace_orto_orig=orto;
               orto=1;
               trace_angle_changed=TRUE;
           }

           pline_mode = PL_MODE_LINE;

           s_trace.b_line=0; ////

           join_last=TRUE;
           goto beg;
         }
         if (pline_mode == PL_MODE_LINE)
         {
             ret=start_trace(df_xbeg, df_ybeg, delete_last, join_last, continue_last);

             if (ret==0) {
                 redcr0 (1) ;
                 block_added = FALSE;

                 continue_last=FALSE;
                 break;
             }

             join_last=FALSE;
             continue_last=FALSE;

             if (pline_mode==PL_MODE_ARC)
             {
                 redcr (2) ; //// ????
                 goto beg;
             }
         }
         else
         {
             s_trace.b_line=0;
             pline_mode = start_trace_arc (df_xbeg, df_ybeg, &s_trace.line, s_trace.b_line, d_line) ;
              if (pline_mode == PL_MODE_END) {
                 redcr0 (1) ;

                 BLOK *b;
                 char *adp_b, *adk_b;
                 NAGLOWEK *nag;
                 WIELOKAT *w;
                 SOLIDARC *sa;
                 BOOL redraw_trace=FALSE;

                 nag=(NAGLOWEK*)dane;
                 if (nag->obiekt==OdBLOK) {
                     b=(BLOK*)nag;
                     if ((b->kod_obiektu == B_PLINE) && (b->opis_obiektu[0] == PL_TRACE))
                     {
                         adk_b = (char *) b + sizeof(NAGLOWEK) + b->n;
                         adp_b = (char *) b + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                         nag=(NAGLOWEK*)adp_b;
                         if (nag->obiekt==Owwielokat)
                         {
                             w=(WIELOKAT*)nag;
                             if ((w->blok == ElemBlok) && (w->empty_typ > 0) && (w->empty_typ < 6))
                             {
                                 redraw_trace=TRUE;
                             }
                         }
                         if (nag->obiekt==Osolidarc)
                         {
                             sa=(SOLIDARC*)nag;
                             if ((sa->blok == ElemBlok) && (sa->empty_typ > 0) && (sa->empty_typ < 6))
                             {
                                 redraw_trace=TRUE;
                             }
                         }
                         if (redraw_trace==TRUE)
                         {
                             reset_trace_block();
                             blokzap(adp_b, adk_b, ANieOkreslony, COPY_PUT, 0);
                             blokzap((char*)b, adk_b, ANieOkreslony, COPY_PUT, 1);
                             flip_screen();
                         }
                     }
                 }

                 pline_mode = PL_MODE_LINE;
                 block_added = FALSE;
                 break;
             }
             if ((pline_mode == PL_MODE_LINE) || (pline_mode == PL_MODE_LINE_CONTINUE))
             {

                 join_last=TRUE;
                 goto beg;
             }
             if (pline_mode == PL_MODE_LINE_BAK)
             {
                 pline_mode = PL_MODE_LINE;
                 continue_last=TRUE;
                 goto beg;
             }
         }

	   }
	   break ;
	case evCommandP :
        if (ev->Number<=6) ev_Number=com_no[d_line][ev->Number];
        else ev_Number=ev->Number;

        ret=trace_command(ev_Number, X0, Y0, strwyj);

        if (beginning) goto  beg0;
        else goto  beg;

	   break ;
	default :
        redcr0 (1) ;
	     break;
      }
   }
}

void Trace (void)
/*--------------*/
{
mTrace_min=Trace_min;
mTrace_max=Trace_max;

d_line=0;
mTrace.pola=pmFTrace;
mTrace.max=mTrace_min;
Trace_Dline_Tline();
}

void Dline (void)
/*--------------*/
{
mTrace_min=Dline_min;
mTrace_max=Dline_max;

d_line=1;
mTrace.pola=pmMLine;
mTrace.max=mTrace_min;  //TEMPORARY, goal is 7 to include arcs
Trace_Dline_Tline();
}

void Tline (void)
/*--------------*/
{
mTrace_min=Tline_min;
mTrace_max=Tline_max;

d_line=2;
mTrace.pola=pmMLine;
mTrace.max=mTrace_min;  //TEMPORARY, goal is 7 to includ arcs
Trace_Dline_Tline();
}


#undef __O_TRACE__