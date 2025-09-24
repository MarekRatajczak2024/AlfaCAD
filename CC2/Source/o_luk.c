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

#define __O_LUK__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_osnap.h"
#include "bib_edit.h"
#include "o_pline.h"

#include "message.h"
#include "menu.h"

#define YES _YES_
#define NO _NO_

extern void setwritemode( int mode );
extern int linestyle_xor(int typ);
extern void setfillstyle_(int pattern, int color);
extern void setcolor(int kolor);
extern void view_line_type(LINIA *L);
extern BOOL get_pattern_count(void);
extern void set_pattern_count(BOOL p_count);
extern void set_pattern_offset(int offs);
extern int TTF_text_len(char *text);
extern BOOL Get_End_Trace(void* ptr_pl,
                          void** ptr_ob,	/*adres ostatniego  obiektu*/
                          BOOL* b_first_end,	/*ktory koniec objektu jest ostatni*/
                          double 	df_x_pl_beg,
                          double 	df_y_pl_beg,
                          double* df_x_pl_end,
                          double* df_y_pl_end);

extern double get_trace_width(int side);
extern double get_trace_axis(int side);
extern int trace_command(int ev_Number, double X0, double Y0);
extern int com_no[3][7];
extern void update_trace_width(void);
extern int get_trace_blok(void);
extern int get_trace_obiektt2(void);

extern int get_Tbreak(void);
extern void set_Tbreak(int tbreak);
extern int get_AfterTbreak(void);
extern void set_AfterTbreak(int aftertbreak);

extern int last_trace_delete(void);
extern int close_trace(void);
extern int break_trace(void);
extern int get_pline_mode(void);

extern int pline_trace;

extern double get_d__luk(void);
extern void outvectoror (LINIA *L, AVECTOR *V, int mode,int pl);
extern void outarcvectoror (LUK *l, AVECTOR *V, int mode,int pl);

//extern void redraw_trace (void);

enum PLINE_MODE {PL_MODE_CONTINUE = 1, PL_MODE_LINE , PL_MODE_ARC,
	PL_MODE_END, PL_MODE_UNDO, PL_MODE_ARC_ESC, PL_MODE_ARC_CR,
	PL_MODE_UNDO_CONT, PL_MODE_ARC_CONTINUE, PL_MODE_BREAK, PL_MODE_LINE_CONTINUE, PL_MODE_LINE_BAK, PL_MODE_ARC_EDIT} ;

enum DRAW_ARC_TYPE
{ ARC_P3 = 0, ARC_SCE, ARC_SCA, ARC_SCL, ARC_SER, ARC_SEA, ARC_SED, ARC_Con, ARC_rev,
	IDM_CLOSE, IDM_UNDO, IDM_LINE, IDM_FREE_EDGE, IDM_PINNED_EDGE, IDM_FIXED_EDGE, IDM_FLIP_SUPPORT, /*IDM_CONTINUOUS_LINE, IDM_DASHED_LINE*/ ARC_rev_Y, ARC_rev_N } ;

extern int last_pline_delete (void) ;
static int last_parc_delete (void) ;
static LUK lpom;
static LINIA L=Ldef;
static int strwyj,ns__pl = ARC_Con, ns__arc = ARC_P3 ;
int ns_arc=0;
static double Rmin,dlc;
static BOOL b__pline ;
static double df__xbeg, df__ybeg ;

static void (*cursor_on)(double ,double)=out_cur_on;
static void (*cursor_off)(double ,double)=out_cur_off;

static int el_ra(BOOL b_graph_value);
static int el_lc(BOOL b_graph_value);

static BOOL arc_reversed=FALSE;
static BOOL global_ra=FALSE;

static KOM_BAK komunikat_arc_bak={-1,0,0,0};
static char stp[30],*stp0;
static int l_pxl, k_pxl;

static BLOK *BLK_ADR;

typedef struct SA_BREAK {
    SOLIDARC sa;
    double df_xend;
    double df_yend;
    double angle;
    BOOL b_second_pl_seg;
    BOOL b_first_end;
}  SA_BREAK;

#define SOLIDARC_N 60
static SA_BREAK sa_break;

#define r18 18
#define r22 16

#define OZero 1.0E-6
#define OCon  1.0E+6      /*1.0E+6*/

#define ID_ARC_CONT 75   //duplicated with o_trace.c
#define ID_LINE_CONTINUE 85 //duplicated with o_trace.c   //was 84

/*
typedef
  struct
   { double xs,ys;
     double xc,yc;
     double xe,ye;
     double x2,y2;
     double a;
     double L;
     double r;
     double d;
     void *ptr_con ; //kontynuacja, xs, ys : wsp. konca, do kontynuacji
     BOOL b_first_end ;	//ktory koniec objektu jest ostatni
     double con_angle ; //kat kontynuacji
   } PLUK;
*/

static PLUK pl;

static const void near pSER(PLUK *pl,LUK *l,double X, double Y,double ws, BOOL b_edit, BOOL *reversed);
static void redcr(char ) ;
int PLine_Arc_Command_Proc (int ev_nr, BLOK *blk_adr) ;
static BOOL set_arc_continue_param (void) ;



/*-----------------------------------------------------------------------*/

static TMENU mLukm={9, 0, 0, 30, 56, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmLukm,NULL,NULL};

static TMENU mPLukm={12, 0, 0, 30, 56, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmPLukm,NULL,NULL};
static TMENU mPLukmSlab={16, 0, 0, 30, 56, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmPLukm,NULL,NULL};

static TMENU mPLukmObrys={14, 0, 0, 30, 56, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmPLukmObrys,NULL,NULL};

BOOL Get_reversed(void)
{
    return arc_reversed;
}

void Set_reversed(BOOL reversed)
{
    arc_reversed=reversed;
}

void set_pl(double x, double y)
{
    pl.xs=x;
    pl.ys=y;
}

static void out_luk (LUK *l, int mode)
/*----------------------------------*/
{
  if (l == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  
  setwritemode(mode);
  if (mode == XOR_PUT)
  {
	  linestyle_xor(l->typ);
	  if (l->kolor == 0) SetColorAC(15); else SetColorAC(l->kolor);
  }
  else
  {
	  linestyle(l->typ);
	  SetColorAC(l->kolor);
  }

  okno_r();
  set_pattern_count(TRUE);
  DrawArc(pikseleX0(l->x),pikseleY0(l->y),l->kat1,l->kat2,pikseleDX(l->r), mode);
  set_pattern_count(FALSE);
  set_pattern_offset(0);
  okno_all();
}

void kom_bak (int ns_arc /*char *str*/, int ch, int d, int i)
{
    komunikat_arc_bak.ns_arc=ns_arc;
    komunikat_arc_bak.ch=ch;
    komunikat_arc_bak.d=d;
    komunikat_arc_bak.i=i;
}

void kom (int ns_arc /*char *str*/, int ch, int d, int i)
/*---------------------------------------*/
{ char *str, *st,*st0;
  //static char stp[30],*stp0;
  static int l,k; // l_pxl, k_pxl;
  int len_st, len_st0, len_str, len_st_pxl, len_st0_pxl, len_str_pxl;

  str=komunikaty_arc [ns_arc];

  if (i>=0)
  {
      if (i) {
          setfillstyle_(SOLID_FILL, BKCOLOR);
          bar(l_pxl, 1, k_pxl + l_pxl, HEIGHT - 1);
          setcolor(kolory.ink);
          moveto(l_pxl + 1, 1);
          outtext_r(stp);
          st0 = strchr(stp0, ch);
          if (st0 != NULL) st0 += 1;
          else st0 = stp0 + strlen(stp0);
      } else st0 = &str[d];


      if (strlen(st0)>0)
      {
          st=strchr(st0,ch);
          if (st==NULL) st=st0+strlen(st0);
      }
      else
      {
          st0=stp0;
          st=st0+strlen(st0);
      }

      if (st0 != NULL) {len_st0 = strlen(st0); len_st0_pxl = TTF_text_len(st0);}
      else {len_st0 = 0; len_st0_pxl = 0;}
      if (st != NULL) { len_st = strlen(st); len_st_pxl = TTF_text_len(st); }
      else { len_st = 0; len_st_pxl = 0; }
      if (str != NULL) { len_str = strlen(str); len_str_pxl = TTF_text_len(str); }
      else { len_str = 0; len_str_pxl = 0; }

      k=len_st0-len_st;
      l=len_str-len_st0;
      k_pxl = len_st0_pxl - len_st_pxl;
      l_pxl = len_str_pxl - len_st0_pxl;
      stp0=st0;
      memmove(stp,st0,k);
      stp[k]=0;
      setfillstyle_(SOLID_FILL,kolory.paperk);
      bar(l_pxl,1,k_pxl+l_pxl,HEIGHT-1);
      setcolor(kolory.inkk);
      moveto(l_pxl+1,1);
      outtext_r(stp);

  }
  else
  {
      setfillstyle_(SOLID_FILL,kolory.paperk);
      bar(l_pxl,1,k_pxl+l_pxl,HEIGHT-1);
      setcolor(kolory.inkk);
      moveto(l_pxl+1,1);
      outtext_r(stp);
  }


  kom_bak(ns_arc, ch, d, i);
}

BOOL kom_arc_bak(void)
{
    if (komunikat_arc_bak.ns_arc < 0) return FALSE;

    komunikat0_str (komunikat_arc_bak.ns_arc, u8"") ;
    kom(komunikat_arc_bak.ns_arc, komunikat_arc_bak.ch, komunikat_arc_bak.d, -1 /*komunikat_arc_bak.i*/);
    return TRUE;
}

int get_komunikat_arc_bak(void)
{
    return komunikat_arc_bak.ns_arc;
}

void type_arc_pl (int ev_nr)
/*-------------------------------*/
{
    static int np;

    if (ev_nr == ARC_rev_Y) {
        Set_reversed(TRUE);
        menu_par_new ((*mLukm.pola)[8].txt, YES);
        menu_par_new ((*mPLukm.pola)[8].txt, YES);
        menu_par_new ((*mPLukmObrys.pola)[8].txt, YES);
    }
    else if (ev_nr == ARC_rev_N) {
        Set_reversed(FALSE);
        menu_par_new ((*mLukm.pola)[8].txt, NO);
        menu_par_new ((*mPLukm.pola)[8].txt, NO);
        menu_par_new ((*mPLukmObrys.pola)[8].txt, NO);
    }
    else if (ns_arc == ARC_Con)
  {
    redcr (1) ;
  }
  ns_arc = ev_nr ;
  ns__pl = ns_arc;
  if (ns_arc == ARC_Con)
  {
    ////redcr (0) ;
  }
  komunikat0_str (ns_arc, u8"") ;
  kom (ns_arc, '-', r6, 0) ;
  if (ns_arc != ARC_Con)
  {
    kom (ns_arc, '-', r6, 1) ;
  }

  switch (ns_arc) {
      case 7:
          elc.ESTRF = el_ra;
          elc.mode = GV_VECTOR;
          elc.extend = 0;
          elc.val_no_max = 2;
          elc.format = "%#10.8lg\0%#10.8lg";
          //np=dodajstr(&el);
          break;
      default:
          el.ESTRF = el_lc;
          el.mode = GV_DIST;
          el.extend = 0;
          el.val_no_max = 1;
          el.format = "%#12.9lg";
          break;
    }

    if (el.st!=NULL) {
        strcpy(el.st, "");
        Out_Edited_Draw_Param((ESTR *) &el, TRUE);
    }
}


static int type_arc (int ev_nr)
/*----------------------------*/
{
  int ret ;
    static int np;

  if (ev_nr == ARC_rev_Y) {
      Set_reversed(TRUE);
      menu_par_new ((*mLukm.pola)[8].txt, YES);
      menu_par_new ((*mPLukm.pola)[8].txt, YES);
      menu_par_new ((*mPLukmObrys.pola)[8].txt, YES);
  }
  else if (ev_nr == ARC_rev_N) {
      Set_reversed(FALSE);
      menu_par_new ((*mLukm.pola)[8].txt, NO);
      menu_par_new ((*mPLukm.pola)[8].txt, NO);
      menu_par_new ((*mPLukmObrys.pola)[8].txt, NO);
  }
  else if (ev_nr != ARC_Con  ||
      (ev_nr == ARC_Con && TRUE == set_arc_continue_param ()))
  {
    ns_arc = ev_nr ;
    komunikat0_str (ns_arc, u8"") ;
    kom (ns_arc, '-', r6, 0) ;
  }
  ret = PL_MODE_CONTINUE ;
  if (ns_arc == ARC_Con)
  {
    ret = PL_MODE_ARC_CONTINUE ;
  }

    switch (ns_arc) {
        case 7:
            elc.ESTRF = el_ra;
            elc.mode = GV_VECTOR;
            elc.extend = 0;
            elc.val_no_max = 2;
            elc.format = "%#10.8lg\0%#10.8lg";
            //np=dodajstr(&el);
            break;
        default:
            el.ESTRF = el_lc;
            el.mode = GV_DIST;
            el.extend = 0;
            el.val_no_max = 1;
            el.format = "%#12.9lg";
            break;
    }

    if (el.st!=NULL) {
        strcpy(el.st, "");
        Out_Edited_Draw_Param((ESTR *) &el, TRUE);
    }
  return ret ;
}


/*-----------------------------------------------------------------------*/

static const void near (*poczatekl[8])(double , double );
static const void near (*parl[8])(PLUK *pl,LUK *l,double x, double y, double ws, BOOL b_edit, BOOL *reversed);

static const void near out_parametry_luku1(int ns)
{
  double xx, xxx;
  double angle_l;
  static char buf [100];

  if(ns==0 || ns==1 /*|| ns == ARC_Con*/)
  {
      //strcpy(el.st, "");
      //Out_Edited_Draw_Param((ESTR *) &el, TRUE);
      return;
  }
  switch(ns)
  {
     case 2 :
     case 5 : xx=(LukG.kat2-LukG.kat1)*180/Pi;
	      if(xx<0) xx+=360;
	      break;
     case 3 : xx=milimetryob(dlc);
	      break;
     case 4 : xx=milimetryob(LukG.r);
	      break;
     case 6 :
         if (Get_reversed()) xx=Atan2(Y-pl.ye,X-pl.xe)*180/Pi;
         else xx=Atan2(Y-pl.ys,X-pl.xs)*180/Pi;
	      angle_l=get_angle_l();
	      if (angle_l!=0)
	       {
		     xx -= angle_l;
	       }
	      if(xx<0) xx+=360;
	      break;
      case 7 :
          xxx=milimetryob(LukG.r);

          xx=(LukG.kat2-LukG.kat1)*180/Pi;
          if(xx<0) xx+=360;
          break;
    default : break;
  }
  buf [0] = '\0' ;

  if (ns == 7)
  {
      if (elc.st != NULL)
      {
          sprintf(buf, "%#10.8lg; %#10.8lg", xxx, xx);
          strncpy(elc.st, buf, 24);
          Out_Edited_Draw_Param((ESTR *) &elc, TRUE);
      }
  }
  else {
      if (el.st!=NULL)
      {
          sprintf(buf, "%#12.9lg", xx);
          strncpy(el.st, buf, r22);
          Out_Edited_Draw_Param((ESTR *) &el, TRUE);
      }
  }
}


static void  cur_off(double x,double y)
{
    flip_screen();
}

static void  cur_on(double x,double y)
{
    BOOL reversed=Get_reversed();
    //redraw_trace();
    (*parl[ns_arc])(&pl,&LukG,x,y,0, FALSE, &reversed);
    if (ns_arc != ARC_Con)
    {
        outline(&L,COPY_PUT,0);
    }
    if(fabs(LukG.kat1-LukG.kat2)>OZero)
    { mvcurb.mvc=0;
        switch (LukG.obiektt2)
        {
            case 6:
                if (LiniaG.obiektt3 == 0) VectorG.style = 22;
                else VectorG.style = 23;
                VectorG.typ=LiniaG.typ;
                outarcvectoror(&LukG, &VectorG, COPY_PUT, 1);
                break;
            case 7:
                if (LiniaG.obiektt3 == 0) VectorG.style = 24;
                else VectorG.style = 25;
                VectorG.typ=LiniaG.typ;
                outarcvectoror(&LukG, &VectorG, COPY_PUT, 1);
                break;
            default:
                out_luk(&LukG, COPY_PUT);
                break;
        }
        out_parametry_luku1 (ns_arc) ;
    }

    cursor_on(x,y);
}

static void redcr(char typ)
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  if (typ==0)
  {
      mvcurb.l=1;
     CUR_OFF(X,Y);
     CUR_oFF=CUR_OFF;  CUR_OFF=cur_off;
     CUR_oN=CUR_ON;   CUR_ON=cur_on;
      CUR_ON(X, Y);
  }
  else
  {
     CUR_OFF(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
      CUR_ON(X, Y);
  }
}

/*-----------------------------------------------------------------------*/

int getwsp_pl (double *X0, double *Y0)
/*------------------------------------*/
{
  int ret_command ;
  EVENT *ev;

  while(1)
  {
     view_line_type(&LiniaG);
     ev = Get_Event_Point (NULL, X0, Y0) ;
      if ((ev -> Number < -1) && (ev -> Number > -7))  //np from editstr
      {
          ret_command = PL_MODE_ARC_CONTINUE ;
          break;
      }
     if (ev -> Number == -83)
      {
       ret_command = PL_MODE_UNDO_CONT ;
       break;
      }
     if (ev->What == evKeyDown)
     {
       if (ev->Number == 0)
       {
	      ret_command = PL_MODE_ARC_ESC ;
	      break ;
       }
        if (ev->Number == ENTER)
       {
	     ret_command = PL_MODE_ARC_CR ;
	     break;
       }
     }
     if ( ev->What == evCommandP)
     {
		 if (ev->Number >= 0)
		 {
			 if (PL_MODE_CONTINUE != (ret_command = PLine_Arc_Command_Proc(ev->Number, BLK_ADR)))
			 {
				 break;
			 }
		 }
     }
  }
  return ret_command ;
}


int arc_command[]={ARC_Con, ARC_P3, ARC_SCE, ARC_SCA, ARC_SCL, ARC_SER, ARC_SEA, ARC_SED, ARC_rev, IDM_LINE, IDM_FREE_EDGE, IDM_PINNED_EDGE, IDM_FIXED_EDGE, IDM_FLIP_SUPPORT, ARC_rev_Y, ARC_rev_N};

int getwsp_tr (double *X0, double *Y0, int d_line)
/*----------------------------------------------*/
{
    int ret_command ;
    EVENT *ev;
    int ev_Number;
    int ret;

    while(1)
    {
        view_line_type(&LiniaG);
         ev = Get_Event_Point (NULL, X0, Y0) ;
        if ((ev -> Number < -1) && (ev -> Number > -7))  //np from editstr
        {
            ret_command = PL_MODE_ARC_EDIT ;
            break;
        }
        if (ev -> Number == -83)
        {
            ret_command = PL_MODE_UNDO_CONT ;
            break;
        }
        if (ev->What == evKeyDown)
        {
            if (ev->Number == 0)
            {
                ret_command = PL_MODE_ARC_ESC ;
                break ;
            }
            if (ev->Number == ENTER)
            {
                ret_command = PL_MODE_ARC_CR ;
                break;
            }
        }
        if ( ev->What == evCommandP)
        {
            if (ev->Number >= 0)
            {
                if (ev->Number<=6) ev_Number=com_no[d_line][ev->Number];
                else ev_Number=ev->Number;
                if ((ev_Number<ID_ARC_CONT) || (ev_Number==ID_LINE_CONTINUE))
                {
                    ret= trace_command(ev_Number, X, Y);
                    if (ret)
                    {
                        ret_command = ret;
                        break ;
                    }
                }
                else if (PL_MODE_CONTINUE != (ret_command = PLine_Arc_Command_Proc(arc_command[ev->Number-ID_ARC_CONT], BLK_ADR)))
                {
                    break;
                }
            }
        }
    }
    return ret_command ;
}


int getwsp_arc (double *X0, double *Y0)
/*------------------------------------*/
{
  int ret_command ;
  EVENT *ev;

  while (1)
  {
    view_line_type(&LiniaG);
    ev = Get_Event_Point (NULL, X0, Y0) ;
    if (ev->What == evKeyDown)
    {
      if(ev->Number == 0)
      {
	ret_command = PL_MODE_ARC_ESC ;
	break;
      }
      if(ev->Number == ENTER)
      {
	ret_command = PL_MODE_ARC_CR ;
	break;
      }
    }
    if( ev->What == evCommandP)
    {
		if (ev->Number >= 0)
		{
			if (PL_MODE_ARC_CONTINUE == (ret_command = type_arc(ev->Number)))
			{
				break;
			}
		}
      continue ;
    }
  }
  return ret_command ;
}


static BOOL add_arc (double X0, double Y0, BOOL strwyj)
/*----------------------------------------------------*/
{
  BOOL b_ret ;
  double x1, y1;
  BOOL reversed_bak;


  BOOL reversed=Get_reversed();

  b_ret = FALSE ;
  if(!strwyj)
   {
       x1 = LukG.x + LukG.r * cos (LukG.kat1) ;
       y1 = LukG.y + LukG.r * sin (LukG.kat1) ;

       if ((Check_if_Equal(X, x1)) && (Check_if_Equal(Y, y1)))  reversed=1;
       else reversed=0;

       //reversed_bak=reversed;
       reversed=0;

     (*parl[ns_arc])(&pl,&LukG,X0,Y0,0, FALSE, &reversed);
       //reversed=reversed_bak;
   }
  else
   {
      if (!global_ra)  memmove(&LukG,&lpom,sizeof(LUK));
      else (*parl[ns_arc])(&pl,&LukG,X0,Y0,0, TRUE, &reversed);
       global_ra=FALSE;
   }
  LukG.kat1 = Angle_Normal (LukG.kat1) ;
  LukG.kat2 = Angle_Normal (LukG.kat2) ;
 if (TRUE == Check_if_Equal (LukG.kat1, LukG.kat2))
  {
     ErrList (66) ;
     return FALSE ;
  }
    PTR__GTMPBLOCK=(char*)BLK_ADR;
    if (FALSE == Check_if_Equal (LukG.r, 0) &&
      NULL != dodaj_obiekt (((b__pline == TRUE) ? BLK_ADR /*(BLOK*)dane*/ : NULL), (void *)&LukG))
  {
    b_ret = TRUE ;
      rysuj_obiekt(&LukG, COPY_PUT, 1) ;
  }

    BLK_ADR=(BLOK*)PTR__GTMPBLOCK;

  return b_ret ;
}

static BOOL add_solidarc (double X0, double Y0, BOOL strwyj, int Tbreak)
/*--------------------------------------------------------------------*/
{
    BOOL b_ret ;
    double x1, y1;

    BOOL reversed=Get_reversed();

    b_ret = FALSE ;
    if(!strwyj)
    {
        x1 = LukG.x + LukG.r * cos (LukG.kat1) ;
        y1 = LukG.y + LukG.r * sin (LukG.kat1) ;

        if ((Check_if_Equal(X, x1)) && (Check_if_Equal(Y, y1)))  reversed=1;
        else reversed=0;

        reversed=0;

        (*parl[ns_arc])(&pl,&LukG,X0,Y0,0, FALSE, &reversed);
    }
    else
    {
        if (!global_ra) memmove(&LukG,&lpom,sizeof(LUK));
        else (*parl[ns_arc])(&pl,&LukG,X0,Y0,0, TRUE, &reversed);
        global_ra=FALSE;
    }
    LukG.kat1 = Angle_Normal (LukG.kat1) ;
    LukG.kat2 = Angle_Normal (LukG.kat2) ;
    if (TRUE == Check_if_Equal (LukG.kat1, LukG.kat2))
    {
        ErrList (66) ;
        return FALSE ;
    }

    SOLIDARC sa=sadef;

    sa.warstwa=LukG.warstwa;
    sa.kolor=LukG.kolor;
    sa.widoczny=1;
    sa.x=LukG.x;
    sa.y=LukG.y;
    sa.r=LukG.r;
    sa.kat1=LukG.kat1;
    sa.kat2=LukG.kat2;

    if (strwyj) LukG.kat1=LukG.kat2;

    sa.reversed=reversed;

    if (reversed) {
        sa.width2 = get_trace_width(1);
        sa.width1 = get_trace_width(2);

        sa.axis2 = get_trace_axis(1);
        sa.axis1 = get_trace_axis(2);

    }
    else
    {
        sa.width1 = get_trace_width(1);
        sa.width2 = get_trace_width(2);

        sa.axis1 = -get_trace_axis(1);
        sa.axis2 = -get_trace_axis(2);
    }
    sa.translucent=0;

    sa.empty_typ=SolidFillTyp;
    sa.pattern = TracePattern;

    sa.blok = get_trace_blok();
    sa.obiektt2 = get_trace_obiektt2();

    if (sa.pattern == 1)
    {
        sa.scale=TracePatternScale; //sizeof(short int)
        sa.dx=TracePatternDx;  //sizeof(short int)
        sa.dy=TracePatternDy;  //sizeof(short int)
        sa.angle=TracePatternAngle;  //sizeof(short int)
        memmove(sa.patternname, &TracePatternName, strlen(TracePatternName) + 1);

        sa.n = SOLIDARC_N + (int)strlen(TracePatternName) ;
    }
    else
    {
        sa.translucent = TraceTranslucent;
        sa.translucency = TraceTranslucency;
    }

      if (FALSE == Check_if_Equal (sa.r, 0))
      {
          if (!Tbreak)
          {

              if (NULL != dodaj_obiekt(((b__pline == TRUE) ? (BLOK *) dane : NULL), (void *) &sa)) {

                  b_ret = TRUE;
                  rysuj_obiekt(&sa, COPY_PUT, 1);
              }

              update_trace_width();
          }
          else
          {
              memmove(&sa_break.sa, &sa, sizeof(SOLIDARC));
              sa_break.df_xend=X0;
              sa_break.df_yend=Y0;
              sa_break.b_second_pl_seg=FALSE;
              sa_break.b_first_end=reversed;
              if (reversed) sa_break.angle=sa.kat1;
              else sa_break.angle=sa.kat2;

              update_trace_width();
              b_ret = TRUE;
          }
      }

    /////////////////

    return b_ret ;
}


int Getwsp1(void)
{
  double X0,Y0;
  EVENT *ev;

  redcr(0);
  strwyj=0;
  while(1)
   { view_line_type(&LiniaG);
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown && ev->Number==0)
       {
	 redcr(1);
	 return 0;
       }
     if((ev->What == evKeyDown && ev->Number== ENTER) || strwyj)
       {
	 Cur_offd(X,Y);
	 add_arc (X0, Y0, strwyj) ;
	 Cur_ond(X,Y);
	 redcr(1);
	 return 1;
       }
   }
}

int tGetwsp1(void)
{
    double X0,Y0;
    EVENT *ev;

    redcr(0);
    strwyj=0;
    while(1)
    { view_line_type(&LiniaG);
        ev=Get_Event_Point(NULL, &X0, &Y0);
        if(ev->What == evKeyDown && ev->Number==0)
        {
            redcr(1);
            return 0;
        }
        if((ev->What == evKeyDown && ev->Number== ENTER) || strwyj)
        {
            Cur_offd(X,Y);
            add_solidarc (X0, Y0, strwyj, 0) ;
            Cur_ond(X,Y);
            redcr(1);
            return 1;
        }
    }
}

/*-----------------------------------------------------------------------*/
static int el_r (BOOL b_graph_value)
{
  double r;
  BOOL reversed=Get_reversed();

  b_graph_value = b_graph_value ;
  if (el.val_no < 1)
  {
    return 0 ;
  }
  r = el.values [0] ;
  r = jednostkiOb (r);
  if(r >= Rmin)
  {
     if (r > OCon) r = OCon;
     memmove(&lpom,&LukG,sizeof(LUK));
     (*parl[ns_arc])(&pl,&lpom,X,Y,r, TRUE, &reversed);
     strwyj=1;
     return 1;
  }
  else
  {
    ErrList(34);
    return 0;
  }
}


static const void near SER(double X0, double Y0)  /*poczatek koniec promien*/
{
  int np;

  pl.xe = X0; pl.ye = Y0;
  out_krz(pl.xe,pl.ye);
  if(fabs(pl.xe-pl.xs)<OZero && fabs(pl.ye-pl.ys)<OZero) return;
  pl.r=dP1P2(pl.xs,pl.ys,pl.xe,pl.ye);
  Rmin=pl.r/2;
  L.x1=pl.xs;
  L.y1=pl.ys;
  L.x2=X;
  L.y2=Y;
  el.mode = GV_DIST ;
  el.ESTRF = el_r;
  el.extend = 0;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
  np=dodajstr(&el);
  kom(ns_arc,'-',r6,1);
  Getwsp1();
  usunstr(np);
}

static const void near tSER(double X0, double Y0)  /*poczatek koniec promien*/
{
    int np;

    pl.xe = X0; pl.ye = Y0;
    out_krz(pl.xe,pl.ye);
    if(fabs(pl.xe-pl.xs)<OZero && fabs(pl.ye-pl.ys)<OZero) return;
    pl.r=dP1P2(pl.xs,pl.ys,pl.xe,pl.ye);
    Rmin=pl.r/2;
    L.x1=pl.xs;
    L.y1=pl.ys;
    L.x2=X;
    L.y2=Y;
    el.mode = GV_DIST ;
    el.ESTRF = el_r;
    el.extend = 0;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
    np=dodajstr(&el);
    kom(ns_arc,'-',r6,1);
    tGetwsp1();
    usunstr(np);
}
/*---------------*/

static const void near pSER(PLUK *pl_,LUK *l,double X, double Y,double ws, BOOL b_edit, BOOL *reversed)
{
  double x0,y0,x,y,xr,yr,xe,ye,xs,ys,r,dl;
  double a1,a2,si,co;
  L.x2=X; L.y2=Y;
  if(b_edit) r=ws;
  else if((r=dP1P2(X,Y,L.x1,L.y1))<=Rmin) return;
  pl_->r=l->r=r;
  x0=(pl_->xe+pl_->xs)/2;
  y0=(pl_->ye+pl_->ys)/2;
  xs=pl_->xs-x0; ys=pl_->ys-y0;
  dl=sqrt(xs*xs+ys*ys);
  si=ys/dl;
  co=xs/dl;
  obru(si,co,xs,ys,&xs,&ys);
  obru(si,co,X-x0,Y-y0,&x,&y);
  xr=0;
  yr=sqrt (fabs (r*r-xs*xs));  /* zawsze r >= xs, dla  r == xs moze byc float, dlatego fabs*/
  if(y<0) yr=-yr;
  obrd(si,co,xr,yr,&xr,&yr);
  l->x=xr+x0; l->y=yr+y0;
  xs=pl_->xs-l->x;  xe=pl_->xe-l->x;
  ys=pl_->ys-l->y;  ye=pl_->ye-l->y;
  a1=Atan2(ys,xs);
  a2=Atan2(ye,xe);

    if (*reversed)
    {
        double a22=a2;
        a2=Angle_Normal(a1);
        a1= Angle_Normal(a22);
    }

  l->kat1=a1;
  l->kat2=a2;
  *reversed = FALSE;
}

void pSER_(PLUK *pl_, LUK *l, double X, double Y, double ws, BOOL b_edit, BOOL *reversed)
{
    pSER(pl_, l, X, Y, ws, b_edit, reversed);
}
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
static int el_a(BOOL b_graph_value)
{
  double a;
  BOOL reversed=Get_reversed();

  b_graph_value = b_graph_value ;
  if (el.val_no < 1)
  {
    return 0 ;
  }
  a = el.values [0] ;
  memmove(&lpom,&LukG,sizeof(LUK));
  (*parl[ns_arc])(&pl,&lpom,X,Y,a, TRUE, &reversed);
  strwyj=1;
  return 1;
}


static const void near SEA(double X0, double Y0)  /*poczatek koniec kat*/
{
  int np;

  pl.xe = X0; pl.ye = Y0;
  out_krz(pl.xe,pl.ye);
  if(fabs(pl.xe-pl.xs)<OZero && fabs(pl.ye-pl.ys)<OZero) return;
  L.x1=pl.xs;
  L.y1=pl.ys;
  L.x2=X;
  L.y2=Y;
  el.mode = GV_ANGLE ;
  el.ESTRF=el_a;
  el.extend = 0;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
  np=dodajstr(&el);
  kom(ns_arc,'-',r6,1);
  Getwsp1();
  usunstr(np);
}

static const void near tSEA(double X0, double Y0)  /*poczatek koniec kat*/
{
    int np;

    pl.xe = X0; pl.ye = Y0;
    out_krz(pl.xe,pl.ye);
    if(fabs(pl.xe-pl.xs)<OZero && fabs(pl.ye-pl.ys)<OZero) return;
    L.x1=pl.xs;
    L.y1=pl.ys;
    L.x2=X;
    L.y2=Y;
    el.mode = GV_ANGLE ;
    el.ESTRF=el_a;
    el.extend = 0;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
    np=dodajstr(&el);
    kom(ns_arc,'-',r6,1);
    tGetwsp1();
    usunstr(np);
}
/*---------------*/

static const void near pSEA(PLUK *pl,LUK *l,double X, double Y,double ws, BOOL b_edit, BOOL *reversed)
{ double x0,y0,xr,yr,xe,ye,xs,ys,dl,t;
  double a1,a2,si,co;
  double dkat;
  int znak_kata=-1;
  
  L.x2=X; L.y2=Y;
  if(b_edit)
  {
    dkat = Grid_to_Rad (ws) ;
    if (dkat<0) znak_kata=1;
    dkat = Angle_Normal (dkat) ;
      if (*reversed) dkat*=-1;
  }
  else dkat=Atan2(Y-pl->ys,X-pl->xs);

  //if (*reversed) znak_kata*=-1;
  
  x0=(pl->xe+pl->xs)/2;
  y0=(pl->ye+pl->ys)/2;
  xs=pl->xs-x0; ys=pl->ys-y0;
  dl=sqrt(xs*xs+ys*ys);
  si=ys/dl;
  co=xs/dl;
  obru(si,co,xs,ys,&xs,&ys);
  xr=0;
  t=tan(dkat/2);
  if(fabs(t)<OZero)  t=(t>=0 ? OZero : -OZero);
  yr=-xs/t;
  pl->r=l->r=sqrt(xs*xs+yr*yr);
  obrd(si,co,xr,yr,&xr,&yr);
  l->x=xr+x0; l->y=yr+y0;
  xs=pl->xs-l->x;  xe=pl->xe-l->x;
  ys=pl->ys-l->y;  ye=pl->ye-l->y;
  a1=Atan2(ys,xs);
  a2=Atan2(ye,xe);

    if (*reversed)
    {
        double a22=a2;
        a2=Angle_Normal(a1);
        a1= Angle_Normal(a22);
    }

  if (znak_kata==-1)
   {
    l->kat1=a1;
    l->kat2=a2;
    *reversed=FALSE;
   }
   else
    {
     l->kat1=a2;
     l->kat2=a1;
     *reversed=TRUE;
    } 
}

void INVPOLYARC(SOLIDARC *sa, double *x_s, double *y_s, double *x_e, double *y_e, double *wpsc)
{

}

typedef struct {
    double x;
    double y;
} Point;

double distance(Point p1, Point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

double angle(Point p1, Point p2) {
    return atan2(p2.y - p1.y, p2.x - p1.x);
}

Point polar(Point p, double angle, double distance) {
    Point result;
    result.x = p.x + distance * cos(angle);
    result.y = p.y + distance * sin(angle);
    return result;
}

void BulgeToArc(Point p1, Point p2, double b, LUK *l) {
    double a = 2 * atan(b);
    double r = distance(p1, p2) / (2 * sin(a));
    double angleOffset = angle(p1, p2);
    Point c = polar(p1, (-M_PI / 2 + a) + angleOffset, r);

    if (b < 0) {
        l->x=c.x;
        l->y=c.y;
        l->kat1=angle(c, p2);
        l->kat2=angle(c, p1);
    } else {
        l->x=c.x;
        l->y=c.y;
        l->kat1=angle(c, p1);
        l->kat2=angle(c, p2);
    }
    l->r=fabs(r);
}

void POLYARC(double x_s, double y_s, double x_e, double y_e, double wpsc, LUK *l)
{ double x0,y0,xr,yr,xe,ye,xs,ys,dl,t;
  double a1,a2,si,co;
  double dkat;
  int znak_kata=0;
  double ws;
  
  /*wpsc stanowi wypuklosc*/
  /*kat ws jest wyznaczony na podstawie wypuklosci wg wzoru*/

  if (wpsc<0) znak_kata=1;
  
  ws = atan(fabs(wpsc)) * 4;
  if (znak_kata==1) dkat=Pi2-ws; else dkat=ws;

  x0=(x_e+x_s)/2;
  y0=(y_e+y_s)/2;
  xs=x_s-x0; ys=y_s-y0;
  dl=sqrt(xs*xs+ys*ys);
  si=ys/dl;
  co=xs/dl;
  obru(si,co,xs,ys,&xs,&ys);
  xr=0;
  t=tan(dkat/2);
  if(fabs(t)<OZero)  t=(t>=0 ? OZero : -OZero);
  yr=-xs/t;
  l->r=sqrt(xs*xs+yr*yr);
  obrd(si,co,xr,yr,&xr,&yr);
  l->x=xr+x0; l->y=yr+y0;
  xs=x_s-l->x;  xe=x_e-l->x;
  ys=y_s-l->y;  ye=y_e-l->y;
  a1=Atan2(ys,xs);
  a2=Atan2(ye,xe);
  if (znak_kata==0)
   {
    l->kat1=a1;
    l->kat2=a2;
   }
   else
    {
     l->kat1=a2;
     l->kat2=a1;
    }

}

/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
static int el_d(BOOL b_graph_value)
{
  double a;
  BOOL reversed=Get_reversed();

  b_graph_value = b_graph_value ;
  if (el.val_no < 1)
  {
    return 0 ;
  }
  a = el.values [0] ;
  memmove(&lpom,&LukG,sizeof(LUK));
  (*parl[ns_arc])(&pl,&lpom,X,Y,a, TRUE, &reversed);
  strwyj=1;
  return 1;
}

static const void near SED(double X0, double Y0)  /*poczatek koniec kierunek*/
{
  int np;
  BOOL reversed=Get_reversed();

  pl.xe = X0; pl.ye = Y0;
  out_krz(pl.xe,pl.ye);
  if(fabs(pl.xe-pl.xs)<OZero && fabs(pl.ye-pl.ys)<OZero) return;
  if (reversed)
  {
      L.x1 = pl.xe;
      L.y1 = pl.ye;
  }
  else
  {
      L.x1 = pl.xs;
      L.y1 = pl.ys;
  }

  L.x2=X;
  L.y2=Y;
  el.ESTRF=el_d;
  el.mode = GV_ANGLE ;
  el.extend = 0;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
  np=dodajstr(&el);
  kom(ns_arc,'-',r6,1);
  Getwsp1();
  usunstr(np);
}

static const void near tSED(double X0, double Y0)  /*poczatek koniec kierunek*/
{
    int np;

    pl.xe = X0; pl.ye = Y0;
    out_krz(pl.xe,pl.ye);
    if(fabs(pl.xe-pl.xs)<OZero && fabs(pl.ye-pl.ys)<OZero) return;
    L.x1=pl.xs;
    L.y1=pl.ys;
    L.x2=X;
    L.y2=Y;
    el.ESTRF=el_d;
    el.mode = GV_ANGLE ;
    el.extend = 0;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
    np=dodajstr(&el);
    kom(ns_arc,'-',r6,1);
    tGetwsp1();
    usunstr(np);
}
/*---------------*/

static const void near pSED(PLUK *pl,LUK *l,double X, double Y,double ws, BOOL b_edit, BOOL *reversed)
{
  double x0,y0,x,y,xr,yr,xe,ye,xs,ys,dl ;
  double a1,a2,si,co;
  double ctgkat;

  L.x2=X; L.y2=Y;
  x0=(pl->xe+pl->xs)/2;
  y0=(pl->ye+pl->ys)/2;

  if (*reversed)
  {
      xs=pl->xe-x0;
      ys=pl->ye-y0;

      //xs=x0-pl->xe;
      //ys=y0-pl->ye;
  }
  else
  {
      xs = pl->xs - x0;
      ys = pl->ys - y0;
  }

  dl=sqrt(xs*xs+ys*ys);
  si=ys/dl;
  co=xs/dl;
  obru(si,co,xs,ys,&xs,&ys);
  obru(si,co,X-x0,Y-y0,&x,&y);
  if (b_edit)
  {
    double angle, angle_l ;
    double x_temp, y_temp ;
#define DL 1
    angle_l=get_angle_l();
    if (angle_l!=0)
     {
      ws += angle_l;
     }
    if(ws<0) ws+=360;

    ws = ws * Pi / 180 ;
    if (*reversed)
    {
        x_temp = pl->xe + DL * cos(ws);
        y_temp = pl->ye + DL * sin(ws);
    }
    else
    {
        x_temp = pl->xs + DL * cos(ws);
        y_temp = pl->ys + DL * sin(ws);
    }
    obru (si, co, x_temp - x0, y_temp - y0, &x_temp, &y_temp) ;
    ws = Atan2 (y_temp - ys, x_temp - xs) ;
    ws = Angle_Normal (ws) ;
    if (TRUE == Check_if_Equal (ws, 0) || TRUE == Check_if_Equal (ws, Pi))
    {
      l->r = 1 ;
      l->kat1 = 0 ;
      l->kat2 = 0 ;
      return ;
    }
    if (ws > Pi)
    {
      angle = ws - Pi / 2 ;
    }
    else
    {
      angle = ws + Pi / 2 ;
    }
    xr=0;
    yr = fabs (xs * tan (angle)) ;
    if (angle > Pi)
    {
      yr = -yr ;
    }
  }
  else
  {
    if(fabs(y)<OZero)  y=(y>=0 ? OZero : -OZero);
    ctgkat = (x - xs) / y ;
    xr = 0 ;
    yr = xs *ctgkat ;
  }
  pl->r=l->r=sqrt(yr*yr+xs*xs);
  obrd(si,co,xr,yr,&xr,&yr);
  l->x=xr+x0; l->y=yr+y0;
  xs=pl->xs-l->x;  xe=pl->xe-l->x;
  ys=pl->ys-l->y;  ye=pl->ye-l->y;
  a1=Atan2(ys,xs);
  a2=Atan2(ye,xe);
  if (b_edit == TRUE)
  {
      if (*reversed)
      {
          l->kat2 = (ws < Pi ? a1 : a2);
          l->kat1 = (ws < Pi ? a2 : a1);
      }
      else {
          l->kat1 = (ws < Pi ? a1 : a2);
          l->kat2 = (ws < Pi ? a2 : a1);
      }
    *reversed = (ws < Pi ? FALSE : TRUE);
  }
  else
  {
      if (*reversed)
      {
          l->kat2 = (y > 0 ? a1 : a2);
          l->kat1 = (y > 0 ? a2 : a1);
      }
      else {
          l->kat1 = (y > 0 ? a1 : a2);
          l->kat2 = (y > 0 ? a2 : a1);
      }
   *reversed = (y>0 ? FALSE : TRUE);
  }


}

/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/

static const void near P3(double X0, double Y0)  /*trzy punkty*/
{

  pl.x2 = X0; pl.y2 = Y0;
  out_krz(pl.x2,pl.y2);
  if(fabs(pl.x2-pl.xs)<OZero && fabs(pl.y2-pl.ys)<OZero) return;
  L.x1=pl.xs;
  L.y1=pl.ys;
  L.x2=pl.xs;
  L.y2=pl.ys;
  kom(ns_arc,'-',r6,1);
  Getwsp1();
}
/*---------------*/

static const void near tP3(double X0, double Y0)  /*trzy punkty*/
{

    pl.x2 = X0; pl.y2 = Y0;
    out_krz(pl.x2,pl.y2);
    if(fabs(pl.x2-pl.xs)<OZero && fabs(pl.y2-pl.ys)<OZero) return;
    L.x1=pl.xs;
    L.y1=pl.ys;
    L.x2=pl.xs;
    L.y2=pl.ys;
    kom(ns_arc,'-',r6,1);
    tGetwsp1();
}
/*---------------*/

static const void near pP3(PLUK *pl,LUK *l,double X, double Y,double ws, BOOL b_edit, BOOL *reversed)
{ double x0,y0,x,y,xe,ye,xs,ys,dl,si,co,xp,yp,ss;
  double a1,a2;

  ws = ws;
  pl->xe = X ;
  pl->ye = Y ;
  b_edit = b_edit;
  x0=(pl->x2+pl->xs)/2;
  y0=(pl->y2+pl->ys)/2;
  xs=pl->xs-x0; ys=pl->ys-y0;
  dl=sqrt(xs*xs+ys*ys);
  si=ys/dl;
  co=xs/dl;
  obru(si,co,xs,ys,&xs,&ys);
  obru(si,co,X-x0,Y-y0,&xe,&ye);  ss=ye;
  if(fabs(ye)<OZero) { l->kat1=l->kat2=0; return; }
  x=0;
  y=(xe*xe+ye*ye-xs*xs)/(2*ye);
  l->r=sqrt(xs*xs+y*y);
  obrd(si,co,x,y,&xp,&yp);
  l->x=xp+x0; l->y=yp+y0;
  xs=pl->xs-l->x;  xe=X-l->x;
  ys=pl->ys-l->y;  ye=Y-l->y;
  a1=Atan2(ys,xs);
  a2=Atan2(ye,xe);
  l->kat1=(ss<0 ? a1 : a2);
  l->kat2=(ss<0 ? a2 : a1);
  *reversed=(ss<0 ? FALSE : TRUE);
}

int get_3p_arc(LUK *l, POINTD *p1, POINTD *p2, POINTD *p3)
{
    PLUK pl;
    BOOL reversed=Get_reversed();

    pl.xs=p1->x; pl.ys=p1->y;
    pl.x2=p2->x; pl.y2=p2->y;

    pP3(&pl, l, p3->x, p3->y,0, FALSE, &reversed);
    l->kat1= Angle_Normal(l->kat1);
    l->kat2= Angle_Normal(l->kat2);
    return 1;
}


#define NPAS 64

int get_arc_points(LUK *l, double *xy, int n0, BOOL reversed, BOOL printer) {
    long Num, N;
    double da, dda;	/*arch opening angle, arch division angle*/
    double si, co; 	/*sine and cosine of the arc division angle*/
    double xdiff, ydiff;	/*auxilioary coordinates*/
    double Xp1,Yp1,Xp2,Yp2;
    double d__luk=1.0;  //1 mm
    int k;
    int n=n0;
    double sta, ea;

    sta=l->kat1;
    ea=l->kat2;

    if (TRUE == Check_if_Equal (sta, 0) &&   /*  dla okregu da =  Pi2 */
        TRUE == Check_if_Equal (ea, Pi2))
    {
        sta = 0 ;
        ea = Pi2 - o_male ;
        da = ea ;
    }
    else
    {
        sta = Angle_Normal (sta) ;
        ea = Angle_Normal (ea) ;
        da = ea - sta ;
        if (sta > ea)
        {
            da += Pi2 ;
        }
    }

    if (printer)
    {
        d__luk=get_d__luk();
        Num = Pi2 * l->r / d__luk;
    }
    else {
        Num = 8 + pikseleDX(l->r) / 5;
    }

    if (Num < 12) Num = 12;
    else if (Num > NPAS) Num = NPAS;

    dda = Pi2 / Num;
    if (da<(dda*4)) dda=da/4.0;
    si = sin(dda);
    co = cos(dda);
    N = (int) (da / dda);
    Xp1 = l->x + l->r * cos(sta);
    Yp1 = l->y + l->r * sin(sta);
    for (k = 0; k < N; k++) {
        xdiff = Xp1 - l->x;
        ydiff = Yp1 - l->y;
        Xp2 = l->x + xdiff * co - ydiff * si;
        Yp2 = l->y + xdiff * si + ydiff * co;
        if (!reversed)
        {
            xy[n] = Xp2;
            xy[n + 1] = Yp2;
        }
        else
        {
            xy[n0+2*N-2-(n-n0)] = Xp2;
            xy[n0+2*N-1-(n-n0)] = Yp2;
        }
        n+=2;
        Xp1 = Xp2;
        Yp1 = Yp2;
    }

    return n;
}


/*-----------------------------------------------------------------------*/

static const void near SCE(double X0, double Y0)  /* poczatek srodek koniec */
{

  LukG.x=X0; LukG.y=Y0;
  out_krz(LukG.x,LukG.y);
  if(fabs(LukG.x-pl.xs)<OZero && fabs(LukG.y-pl.ys)<OZero) return;
  LukG.r=dP1P2(pl.xs,pl.ys,LukG.x,LukG.y);
  LukG.kat1=LukG.kat2=0;
  L.x1=LukG.x;
  L.y1=LukG.y;
  L.x2=X;
  L.y2=X;
  kom(ns_arc,'-',r6,1);
  Getwsp1();
}

static const void near tSCE(double X0, double Y0)  /* poczatek srodek koniec */
{

    LukG.x=X0; LukG.y=Y0;
    out_krz(LukG.x,LukG.y);
    if(fabs(LukG.x-pl.xs)<OZero && fabs(LukG.y-pl.ys)<OZero) return;
    LukG.r=dP1P2(pl.xs,pl.ys,LukG.x,LukG.y);
    LukG.kat1=LukG.kat2=0;
    L.x1=LukG.x;
    L.y1=LukG.y;
    L.x2=X;
    L.y2=X;
    kom(ns_arc,'-',r6,1);
    tGetwsp1();
}
/*---------------*/

static const void near pSCE(PLUK *pl,LUK *l,double X, double Y,double ws, BOOL b_edit, BOOL *reversed)
{ double xe,ye,xs,ys,dl;
  double a1,a2,si,co;

  ws = ws;
  b_edit = b_edit;
  L.x2=X; L.y2=Y;
  xs=X-l->x; ys=Y-l->y;
  dl=sqrt(xs*xs+ys*ys);
  if(fabs(dl)<OZero) return;

  si=ys/dl;
  co=xs/dl;
  pl->xe=l->x+l->r*co;
  pl->ye=l->y+l->r*si;

  xs=pl->xs-l->x;  xe=pl->xe-l->x;
  ys=pl->ys-l->y;  ye=pl->ye-l->y;
  a1=Atan2(ys,xs);
  a2=Atan2(ye,xe);
  if (*reversed)
  {
      l->kat2 = a1;
      l->kat1 = a2;
  }
  else {
      l->kat1 = a1;
      l->kat2 = a2;
  }
  if(fabs(l->kat1-l->kat2)<OZero) { l->kat1=0; l->kat2=Pi2;}
  *reversed = FALSE;
}


/*-----------------------------------------------------------------------*/
static int el_ac(BOOL b_graph_value)
{
  double a;
  BOOL reversed=Get_reversed();

  b_graph_value = b_graph_value ;
  if (el.val_no < 1)
  {
    return 0 ;
  }
  a = el.values [0] ;
  memmove(&lpom,&LukG,sizeof(LUK));
  (*parl[ns_arc])(&pl,&lpom,X,Y,a, TRUE, &reversed);
  strwyj=1;
  return 1;
}

static const void near SCA(double X0, double Y0)  /*poczatek srodek kat*/
{
  int np;

  LukG.x=X0; LukG.y=Y0;
  out_krz(LukG.x,LukG.y);
  if(fabs(LukG.x-pl.xs)<OZero && fabs(LukG.y-pl.ys)<OZero) return;
  LukG.r=dP1P2(pl.xs,pl.ys,LukG.x,LukG.y);
  LukG.kat1=LukG.kat2=0;
  L.x1=LukG.x;
  L.y1=LukG.y;
  L.x2=X;
  L.y2=Y;
  el.ESTRF=el_ac;
  el.extend = 0;
  el.mode = GV_ANGLE ;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
  np=dodajstr(&el);
  kom(ns_arc,'-',r6,1);
  Getwsp1();
  usunstr(np);
}

static const void near tSCA(double X0, double Y0)  /*poczatek srodek kat*/
{
    int np;

    LukG.x=X0; LukG.y=Y0;
    out_krz(LukG.x,LukG.y);
    if(fabs(LukG.x-pl.xs)<OZero && fabs(LukG.y-pl.ys)<OZero) return;
    LukG.r=dP1P2(pl.xs,pl.ys,LukG.x,LukG.y);
    LukG.kat1=LukG.kat2=0;
    L.x1=LukG.x;
    L.y1=LukG.y;
    L.x2=X;
    L.y2=Y;
    el.ESTRF=el_ac;
    el.extend = 0;
    el.mode = GV_ANGLE ;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
    np=dodajstr(&el);
    kom(ns_arc,'-',r6,1);
    tGetwsp1();
    usunstr(np);
}
/*---------------*/

static const void near pSCA(PLUK *pl,LUK *l,double X, double Y,double ws, BOOL b_edit, BOOL *reversed)
{ double xs, ys;
  double a1,a2;
  double dkat;

  L.x2=X; L.y2=Y;

    xs=pl->xs-l->x;
    ys=pl->ys-l->y;

  if(b_edit)
   {
      if (*reversed)
      {
          a2=Angle_Normal(Atan2(ys,xs));
          dkat = Angle_Normal(a2-Grid_to_Rad(ws));
      }
      else
      {
          a1=Angle_Normal(Atan2(ys,xs));
          dkat = Angle_Normal(Grid_to_Rad(ws) + a1);
      }
   }
  else
   { if(fabs(Y-l->y)<OZero && fabs(X-l->x)<OZero) return;
     dkat=Atan2(Y-l->y,X-l->x);
   }

  if (*reversed)
  {
      a2=Angle_Normal(Atan2(ys,xs));
      a1=Angle_Normal(dkat);

      l->kat1 = a1;
      l->kat2 = a2;
  }
  else
  {
    a1=Angle_Normal(Atan2(ys,xs));
    a2=Angle_Normal(dkat);

    l->kat1 = a1;
    l->kat2 = a2;
  }

  *reversed=FALSE;
}

/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
static int el_lc(BOOL b_graph_value)
{
  double a;
  BOOL reversed=Get_reversed();

  b_graph_value = b_graph_value ;
  if (el.val_no < 1)
  {
    return 0 ;
  }
  a = el.values [0] ;
   if(a!=0)
    { memmove(&lpom,&LukG,sizeof(LUK));
      (*parl[ns_arc])(&pl,&lpom,X,Y,a, TRUE, &reversed);
      strwyj=1;
      return 1;
    }
  else
  {
    ErrList(35);
    return 0;
  }
}

static int el_ra(BOOL b_graph_value)
{
    double r, a;
    float LukG_kat1;
    BOOL reversed;  //=Get_reversed();
    float d_kat;
    double x1, y1, x2, y2;
    BOOL reversed1;

    reversed=0;

    x1 = LukG.x + LukG.r * cos (LukG.kat1) ;
    y1 = LukG.y + LukG.r * sin (LukG.kat1) ;

    if ((Check_if_Equal(X, x1)) && (Check_if_Equal(Y, y1)))  reversed=1;
    else reversed=0;

    //d_kat=LukG.kat2-LukG.kat1;

    b_graph_value = b_graph_value ;
    if (elc.val_no < 2)
    {
        return 0 ;
    }
    r = elc.values [0] ;
    a = elc.values [1] ;
    if ((r>0) && (a!=0))
    {
        memmove(&lpom,&LukG,sizeof(LUK));
        (*parl[ns_arc])(&pl, &lpom, r, a, 0, TRUE, &reversed);


        ////strwyj=1;
        ////global_ra=TRUE;
        ////if (elc.flag==0) add_arc(r, a, strwyj);
        ////else add_solidarc( r, a, strwyj, get_Tbreak());

        if(reversed) {
            x2 = lpom.x + lpom.r * cos(lpom.kat1);
            y2 = lpom.y + lpom.r * sin(lpom.kat1);
        }
        else
        {
            x2 = lpom.x + lpom.r * cos(lpom.kat2);
            y2 = lpom.y + lpom.r * sin(lpom.kat2);
        }

        strwyj=0;
        global_ra=FALSE;
        if (elc.flag==0) add_arc(x2, y2, strwyj);
        else add_solidarc( x2, y2, strwyj, get_Tbreak());


        pl.xs=pl.xe;
        pl.ys=pl.ye;
        //strwyj=0;
        return 1;
    }
    else
    {
        ErrList(35);
        return 0;
    }
}

static const void near SCL(double X0, double Y0)  /*poczatek srodek dl cieciwy*/
{
  int np;

  LukG.x=X0; LukG.y=Y0;
  out_krz(LukG.x,LukG.y);
  if(fabs(LukG.x-pl.xs)<OZero && fabs(LukG.y-pl.ys)<OZero) return;
  LukG.r=dP1P2(pl.xs,pl.ys,LukG.x,LukG.y);
  LukG.kat1=LukG.kat2=0;
  L.x1=pl.xs;
  L.y1=pl.ys;
  L.x2=X;
  L.y2=Y;
  el.ESTRF=el_lc;
  el.mode = GV_DIST ;
  el.extend = 0;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
  np=dodajstr(&el);
  kom(ns_arc,'-',r6,1);
  Getwsp1();
  usunstr(np);
}

static const void near tSCL(double X0, double Y0)  /*poczatek srodek dl cieciwy*/
{
    int np;

    LukG.x=X0; LukG.y=Y0;
    out_krz(LukG.x,LukG.y);
    if(fabs(LukG.x-pl.xs)<OZero && fabs(LukG.y-pl.ys)<OZero) return;
    LukG.r=dP1P2(pl.xs,pl.ys,LukG.x,LukG.y);
    LukG.kat1=LukG.kat2=0;
    L.x1=pl.xs;
    L.y1=pl.ys;
    L.x2=X;
    L.y2=Y;
    el.ESTRF=el_lc;
    el.mode = GV_DIST ;
    el.extend = 0;
    el.val_no_max = 1 ;
    el.format = "%#12.9lg" ;
    np=dodajstr(&el);
    kom(ns_arc,'-',r6,1);
    tGetwsp1();
    usunstr(np);
}
/*---------------*/

static const void near pSCL(PLUK *pl,LUK *l,double X, double Y,double ws, BOOL b_edit, BOOL *reversed)
{ double xe,ye,xs,ys,dl0;
  double a1,a2,si,co;
  L.x2=X; L.y2=Y;
  if(b_edit) dlc=jednostkiOb(ws);
  else dlc=dP1P2(pl->xs,pl->ys,X,Y);
  if(fabs(dlc)>2*l->r) dlc=2*l->r;
  xs=pl->xs-l->x;   ys=pl->ys-l->y;
  dl0=sqrt(xs*xs+ys*ys);

  si = ys / dl0;
  co = xs / dl0;

  //if (*reversed) si*=-1;  //sin(-a) = -sin(a);  cos(-a) = coa(a)

  if ((L.x2<L.x1) && (dlc>0)) dlc*=-1;

  obru(si,co,xs,ys,&xs,&ys);
  xe=(-dlc*dlc+(l->r)*(l->r)+xs*xs)/(2*xs);
  ye=sqrt(fabs(l->r*l->r-xe*xe));
  obrd(si,co,xe,ye,&xe,&ye);
  xs=pl->xs-l->x;
  ys=pl->ys-l->y;
  a1=Atan2(ys,xs);
  a2=Atan2(ye,xe);

    if (*reversed)
    {
        if (a2<a1) a2+=Pi2;
        double dkat=a2 - a1;
        a2=a1;
        a1= Angle_Normal(a2-dkat);
    }

  l->kat1 = (dlc < 0 ? a2 : a1);
  l->kat2 = (dlc < 0 ? a1 : a2);

  *reversed=(dlc<0 ? TRUE : FALSE);
}

/*-----------------------------------------------------------------------*/

static double get_continue_angle (void *ptr_con, BOOL b_first_end)
/*--------------------------------------------------------*/
{
  double angle, x1, y1, x2, y2 ;
  LINIA *ptrs_line ;
  LUK  *ptrs_arc ;
  WIELOKAT *ptrs_solid;
  SOLIDARC *ptrs_solidarc;
  double df_x_start, df_y_start, df_x_end, df_y_end;

  if ((Olinia == ((NAGLOWEK *)ptr_con)->obiekt))
  {
    ptrs_line =  (LINIA *)ptr_con ;
    if (TRUE == b_first_end)
    {
      angle = Atan2 (ptrs_line->y1 - ptrs_line->y2, ptrs_line->x1 - ptrs_line->x2) ;
    }
    else
    {
      angle = Atan2 (ptrs_line->y2 - ptrs_line->y1, ptrs_line->x2 - ptrs_line->x1) ;
    }
  }
  else if ((Owwielokat == ((NAGLOWEK *)ptr_con)->obiekt))
    {
        ptrs_solid =  (WIELOKAT *)ptr_con ;
        df_x_start = (ptrs_solid->xy[0] + ptrs_solid->xy[2])/2;
        df_y_start = (ptrs_solid->xy[1] + ptrs_solid->xy[3])/2;
        df_x_end = (ptrs_solid->xy[ptrs_solid->lp - 2] + ptrs_solid->xy[ptrs_solid->lp - 4])/2;
        df_y_end = (ptrs_solid->xy[ptrs_solid->lp - 1] + ptrs_solid->xy[ptrs_solid->lp - 3])/2;
        if (TRUE == b_first_end)
        {
            angle = Atan2 (df_y_start - df_y_end, df_x_start - df_x_end) ;
        }
        else
        {
            angle = Atan2 (df_y_end - df_y_start, df_x_end - df_x_start) ;
        }
    }
    else if ((Osolidarc == ((NAGLOWEK *)ptr_con)->obiekt))  //solidarc
    {
        ptrs_solidarc =  (SOLIDARC *)ptr_con ;
        angle = ((b_first_end == TRUE) ? ptrs_solidarc->kat1 : ptrs_solidarc->kat2) ;
        obrd (sin (angle), cos (angle), ptrs_solidarc->r,
              ((b_first_end == TRUE) ? -1 : 1) * 1, &x2, &y2) ;
        x2 += ptrs_solidarc->x ;
        y2 += ptrs_solidarc->y ;
        x1 = ptrs_solidarc->x + ptrs_solidarc->r * cos (ptrs_solidarc->kat1) ;
        y1 = ptrs_solidarc->y + ptrs_solidarc->r * sin (ptrs_solidarc->kat1) ;
        if (b_first_end == FALSE)
        {
            x1 = ptrs_solidarc->x + ptrs_solidarc->r * cos (ptrs_solidarc->kat2) ;
            y1 = ptrs_solidarc->y + ptrs_solidarc->r * sin (ptrs_solidarc->kat2) ;
        }
        angle = Atan2 (y2 - y1, x2 - x1) ;
    }
  else if ((Oluk == ((NAGLOWEK *)ptr_con)->obiekt))  //arc
  {
    ptrs_arc =  (LUK *)ptr_con ;
    angle = ((b_first_end == TRUE) ? ptrs_arc->kat1 : ptrs_arc->kat2) ;
    obrd (sin (angle), cos (angle), ptrs_arc->r,
	 ((b_first_end == TRUE) ? -1 : 1) * 1, &x2, &y2) ;
    x2 += ptrs_arc->x ;
    y2 += ptrs_arc->y ;
    x1 = ptrs_arc->x + ptrs_arc->r * cos (ptrs_arc->kat1) ;
    y1 = ptrs_arc->y + ptrs_arc->r * sin (ptrs_arc->kat1) ;
    if (b_first_end == FALSE)
    {
      x1 = ptrs_arc->x + ptrs_arc->r * cos (ptrs_arc->kat2) ;
      y1 = ptrs_arc->y + ptrs_arc->r * sin (ptrs_arc->kat2) ;
    }
    angle = Atan2 (y2 - y1, x2 - x1) ;
  }
  else angle=0;  //something else

  angle *= 180 / Pi_ ;
  return angle ;
}


static void near Con_pl (double X0, double Y0)
/*-----------------------------------------*/
{		/*kontynuacja linii lub luku*/
    int np;

  pl.xe = X0 ;
  pl.ye = Y0 ;

  /*
    el.ESTRF=el_ra;
    el.mode = GV_VECTOR ;
    el.extend = 0;
    el.val_no_max = 2 ;
    el.format = "%#10.8lg\0%#10.8lg" ;
    np=dodajstr(&el);
    */

  add_arc (X0, Y0, 0) ;

    //usunstr(np);

  Cur_offd (X, Y) ;
  Cur_ond (X, Y) ;
}

static void near tCon_pl (double X0, double Y0)
/*-----------------------------------------*/
{		/*kontynuacja linii lub luku*/
    int np;

    pl.xe = X0 ;
    pl.ye = Y0 ;

    /*
    el.ESTRF=el_ra;
    el.mode = GV_VECTOR ;
    el.extend = 0;
    el.val_no_max = 2 ;
    el.format = "%#10.8lg\0%#10.8lg" ;
    np=dodajstr(&el);
     */

    int tbreak=get_Tbreak();
    add_solidarc (X0, Y0, 0, tbreak);

    ////usunstr(np);

    Cur_offd (X, Y) ;
    Cur_ond (X, Y) ;
}

static void near Con_arc (double X0, double Y0)
/*-----------------------------------------*/
{		/*kontynuacja linii lub luku*/
    int np;

  pl.xe = X0 ;
  pl.ye = Y0 ;

  /*
  el.ESTRF=el_ra;
  el.mode = GV_VECTOR ;
  el.extend = 0;
  el.val_no_max = 2 ;
  el.format = "%#10.8lg\0%#10.8lg" ;
  np=dodajstr(&el);
   */

  Getwsp1();

    //usunstr(np);
}

static void near tCon_arc (double X0, double Y0)
/*-----------------------------------------*/
{		/*kontynuacja linii lub luku*/
    int np;

    pl.xe = X0 ;
    pl.ye = Y0 ;


    /*
    el.ESTRF=el_ra;
    el.mode = GV_VECTOR ;
    el.extend = 0;
    el.val_no_max = 2 ;
    el.format = "%#10.8lg\0%#10.8lg" ;
    np=dodajstr(&el);
     */

    tGetwsp1();

    //usunstr(np);
}

static const void near Con (double X0, double Y0)
/*-----------------------------------------*/
{
  if (TRUE == b__pline)
  {
    Con_pl (X0, Y0) ;
  }
  else
  {
    Con_arc (X0, Y0) ;
  }
}

static const void near tCon (double X0, double Y0)
/*----------------------------------------------*/
{
    if (TRUE == b__pline)
    {
        tCon_pl (X0, Y0) ;
    }
    else
    {
        tCon_arc (X0, Y0) ;
    }
}


/*---------------*/


static const void near pCon (PLUK *pl,LUK *l,double par1, double par2, double ws, BOOL b_edit, BOOL *reversed)
/*---------------------------------------------------------------------------------------------------*/
{
    double r, a;
    double x1, y1, xs, ys;
    BOOL reversed_bak;

  ws = ws ;
  b_edit = b_edit ;
  if (b_edit)
  {
      r=jednostkiOb(par1);  //which is radius in object units
      a=par2;  //which is internal angle in deg

      if (*reversed)
      {
          x1 = l->x + l->r * cos(l->kat2);
          y1 = l->y + l->r * sin(l->kat2);

          l->x = x1 - r * cos(l->kat2);
          l->y = y1 - r * sin(l->kat2);
      }
      else
      {
          x1 = l->x + l->r * cos(l->kat1);
          y1 = l->y + l->r * sin(l->kat1);

          l->x = x1 - r * cos(l->kat1);
          l->y = y1 - r * sin(l->kat1);
      }

      l->r=r;

      reversed_bak=*reversed;
      pSCA(pl, l, par1, par2, a, TRUE, reversed);
      *reversed=reversed_bak;


      if (*reversed)
      {
          pl->xe = l->x + l->r * cos(l->kat1);
          pl->ye = l->y + l->r * sin(l->kat1);
      }
      else
      {
          pl->xe = l->x + l->r * cos(l->kat2);
          pl->ye = l->y + l->r * sin(l->kat2);
      }
      pl->r=l->r;

      X=pl->xe;
      Y=pl->ye;
  }
  else
  {
      pl->xe = par1;  //which is X
      pl->ye = par2;  //which is Y
      if (TRUE == Check_if_Equal(pl->xe, pl->xs) &&
          TRUE == Check_if_Equal(pl->ye, pl->ys)) {
          LukG.kat1 = LukG.kat2 = 0;
          return;
      }
      ////reversed_bak=*reversed;
      pSED(pl, l, par1, par2, pl->con_angle, TRUE, reversed);
      ////*reversed=reversed_bak;
  }
}

/*-----------------------------------------------------------------------*/

static const void near (*poczatekl[8])(double, double)=
	      {P3,SCE,SCA,SCL,SER,SEA,SED,Con};

static const void near (*poczatekt[8])(double, double)=
        {tP3,tSCE,tSCA,tSCL,tSER,tSEA,tSED,tCon};

static const void near (*parl[8])(PLUK *pl,LUK *l,double x, double y,double ws, BOOL b_edit, BOOL *reversed)=
	      {pP3,pSCE,pSCA,pSCL,pSER,pSEA,pSED,pCon};

/*-----------------------------------------------------------------------*/

static void redcr_continue (char typ)
/*----------------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(),akt,sel_cur;

  if (typ==0)
  {
     komunikat0 (81) ;
     komunikat (0) ;
     akt=sel.akt; sel.akt=ASel;
     sel_cur=sel.cur; sel.cur=1;
     Cur_offd(X,Y);
     CUR_oFF=CUR_OFF;  CUR_OFF=out_sel_off;
     CUR_oN=CUR_ON;   CUR_ON=out_sel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     Cur_ond (X, Y) ;
   }
  else
   {
     Cur_offd(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
     Cur_ond (X, Y) ;
     sel.akt=akt;
     sel.cur=sel_cur;
   }
}

static void get_param_end_ob (void   *ptr_object,
			      BOOL    *b_first_end,
			      double  *df_xend,
			      double  *df_yend)
/*-----------------------------------------------*/
{
  LINIA * ptrs_line ;
  LUK * ptrs_arc ;
  double x1, y1 ;

  ptrs_line = (LINIA*)ptr_object ;
  ptrs_arc = (LUK*)ptr_object ;
  *df_xend = X ; 	/*dla koniecL_, koniecl_ */
  *df_yend = Y ;
  if ((((NAGLOWEK*)ptr_object)->obiekt == Olinia))
  {
    koniecL_ (df_xend, df_yend, ptr_object) ;
    x1 = ptrs_line->x1 ;
    y1 = ptrs_line->y1 ;
  }
  else
  {
    koniecl_ (df_xend, df_yend, ptr_object) ;
    x1 = ptrs_arc->x + ptrs_arc->r * cos (ptrs_arc->kat1) ;
    y1 = ptrs_arc->y + ptrs_arc->r * sin (ptrs_arc->kat1) ;
  }
  *b_first_end = FALSE ;
  if (TRUE == Check_if_Equal (x1, *df_xend) &&
      TRUE == Check_if_Equal (y1, *df_yend))
  {
    *b_first_end = TRUE ;
  }
}


static void *get_obiect_to_continue (BOOL    *b_first_end,
				     double  *df_xend,
				     double  *df_yend)
/*---------------------------------------------------*/
{
  EVENT *ev;
  double X0, Y0;
  void *ptr_object ;
  unsigned type_object ;

  redcr_continue (0) ;
  ptr_object = NULL ;
  while (1)
  {
    view_line_type(&LiniaG);
    ev=Get_Event_Point (NULL, &X0, &Y0);
    if ( ev->What == evKeyDown  && ev->Number== 0 )
    {
      break ;
    }
    if ( ev->What == evKeyDown  && ev->Number == ENTER)
    {
      type_object = Blinia  | Bluk ;
      if (NULL != (ptr_object = select_w (&type_object, NULL)))
      {
	 get_param_end_ob (ptr_object, b_first_end, df_xend, df_yend) ;
	 break ;
      }
    }
  }
  redcr_continue (1) ;
  return ptr_object ;
}

static BOOL set_arc_continue_param (void)
/*--------------------------------------*/
{
  double df_xend, df_yend ;
  BOOL b_ret ;

  b_ret = FALSE ;
  menupini (NULL, u8"", ' ',0) ;
  if (NULL != (pl.ptr_con =
      get_obiect_to_continue (&(pl.b_first_end), &df_xend, &df_yend)))
  {
    pl.xs = df_xend ;
    pl.ys = df_yend ;
    pl.xe = pl.xs ;
    pl.ye = pl.ys ;
    pl.con_angle = get_continue_angle (pl.ptr_con, pl.b_first_end) ;
    out_krz (pl.xs, pl.ys) ;
    b_ret = TRUE ;
  }
  return b_ret ;
}


static void redcr0 (char typ)
/*---------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[5])(), akt;
  static int np;
  if (typ==0)
  {
     if (b__pline == FALSE)
     {

       ns_arc = ns__arc ;
       LukG.blok = NoElemBlok ;
     }
     else
     {
       SW[4] = SERV [83];  SERV [83] = last_parc_delete ;
       ns_arc = ns__pl ;
       LukG.blok = ElemBlok ;
     }
     komunikat(0);
	 CUR_OFF(X, Y);

    akt=sel.akt; sel.akt=ASel;

     cursor_off= cursel_off;
	 cursor_on=cursel_on;

      el.x=maxX-PL266;
      el.y=ESTR_Y;
      el.lmax=r22;
      el.val_no_max = 1 ;
      el.format = "%#12.9lg" ;
     
     elc.x=maxX-PL266;
     elc.y=ESTR_Y;
     elc.lmax=24;
     elc.val_no_max = 2 ;
     elc.format = "%#10.8lg\0%#10.8lg" ;
     elc.ESTRF=el_ra;
     elc.mode = GV_VECTOR ;
     elc.extend=0;
     elc.flag=0;
     np=dodajstr(&elc);

     CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;   CUR_ON=cursel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
	 CUR_ON(X, Y);
   }
   else if (typ==10)
    {
        if (b__pline == FALSE)
        {

            ns_arc = ns__arc ;
            LukG.blok = NoElemBlok ;
        }
        else
        {
            ns_arc = ns__pl ;
            LukG.blok = ElemBlok ;
        }

        SW[4] = SERV[83];  SERV[83] = last_trace_delete;  //del
        SW[3] = SERV[71];  SERV[71] = break_trace;  //Home
        SW[2] = SERV[79];  SERV[79] = close_trace;  //End

        komunikat(0);
        CUR_OFF(X, Y);

        akt=sel.akt; sel.akt=ASel;

        cursor_off= cursel_off;
        cursor_on=cursel_on;

        el.x=maxX-PL266;
        el.y=ESTR_Y;
        el.lmax=r22;
        el.val_no_max = 1 ;
        el.format = "%#12.9lg" ;

        elc.x=maxX-PL266;
        elc.y=ESTR_Y;
        elc.lmax=24;
        elc.val_no_max = 2 ;
        elc.format = "%#10.8lg\0%#10.8lg" ;
        elc.ESTRF=el_ra;
        elc.mode = GV_VECTOR ;
        elc.extend=0;
        elc.flag=1;
        np=dodajstr(&elc);

        CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
        CUR_oN=CUR_ON;   CUR_ON=cursel_on;
        ////SW[0]=SERV[73];  SERV[73]=sel_t;
        ////SW[1]=SERV[81];  SERV[81]=sel_d;
        CUR_ON(X, Y);
    }
  else if (typ==1)
   {
	 CUR_OFF(X, Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];

     sel.akt=akt;
	 CUR_ON(X, Y);
     menupini (NULL,"",' ',0) ;
     komunikat (0) ;
     komunikat0 (0) ;
     komunikat_arc_bak.ns_arc=-1;
     if (b__pline == FALSE)
     {
       ns__arc = ns_arc ;
     }
     else
     {
       SERV[83]=SW[4];
       ns__pl = ns_arc ;
       LukG.blok = NoElemBlok ;
     }
       usunstr(np);
   }
  else if (typ==11)
  {
      CUR_OFF(X, Y);
      CUR_OFF=CUR_oFF;
      CUR_ON=CUR_oN;
      ////SERV[73]=SW[0];
      ////SERV[81]=SW[1];

      sel.akt=akt;
      CUR_ON(X, Y);
      komunikat (0) ;
      komunikat0 (0) ;
      komunikat_arc_bak.ns_arc=-1;
      if (b__pline == FALSE)
      {
          ns__arc = ns_arc ;
      }
      else
      {
          ////SERV[83]=SW[4];
          ns__pl = ns_arc ;
          LukG.blok = NoElemBlok ;
      }
      SERV[83] = SW[4];
      SERV[71]= SW[3];
      SERV[79] = SW[2];

      usunstr(np);
  }
}


void Luk (void)
/*-----------*/
{
  double X0, Y0 ;
  int ret_command ;
  char st[6];
  static char stY [2]=YES, stN [2]=NO;

  b__pline = FALSE ;
  redcr0 (0) ;
  while (1)
  {
      if (Get_reversed()) strcpy(st,stY);
      else strcpy(st,stN);
     menupini (&mLukm, _ARC_, _ARC_C_, 15) ;
     menu_par_new ((*mLukm.pola)[8].txt, st);
     komunikat0_str (ns_arc, u8"") ;
     kom (ns_arc, '-', r6, 0) ;
     if (ns_arc == ARC_Con)
     {
       if (TRUE == set_arc_continue_param ())
       {
	 komunikat0_str (ns_arc, u8"") ;
	 kom (ns_arc, '-', r6, 0) ;
	 X0 = X ;
	 Y0 = Y ;
       }
       else
       {
	 ns_arc = ARC_P3 ;
	 continue ;
       }
     }
     else
     {
     
       ret_command = getwsp_arc (&X0, &Y0) ;
       if (PL_MODE_ARC_ESC == ret_command)
       {
	 break ;
       }
       else
       if (PL_MODE_ARC_CR == ret_command)
       {
	 menupini (NULL, u8"", ' ',0) ;
	 pl.xs = X0 ;
	 pl.ys = Y0 ;
	 out_krz (pl.xs, pl.ys) ;
	 kom (ns_arc, '-', r6, 1) ;
	 if (PL_MODE_ARC_ESC == getwsp_arc (&X0, &Y0))
	 {
	   continue ;
	 }
       }
     }
     (*poczatekl [ns_arc]) (X0, Y0) ;
  }
  redcr0 (1) ;
}


int PLine_Arc_Command_Proc (int ev_nr, BLOK *blk_adr)
/*-------------------------------------------------*/
{
  int ret_val ;
  double df_xend, df_yend ;
  void *ptr_temp, *ptr_ob ;
  BOOL b_first_end ;
  int ret;

  BLK_ADR=blk_adr;

    if (ev_nr == ARC_rev_Y) {
        Set_reversed(TRUE);
        menu_par_new ((*mLukm.pola)[8].txt, YES);
        menu_par_new ((*mPLukm.pola)[8].txt, YES);
        menu_par_new ((*mPLukmObrys.pola)[8].txt, YES);
    }
    else if (ev_nr == ARC_rev_N) {
        Set_reversed(FALSE);
        menu_par_new ((*mLukm.pola)[8].txt, NO);
        menu_par_new ((*mPLukm.pola)[8].txt, NO);
        menu_par_new ((*mPLukmObrys.pola)[8].txt, NO);
    }
  else if (ev_nr < IDM_CLOSE)
  {
    type_arc_pl (ev_nr) ;
    ret_val = PL_MODE_CONTINUE ;
    return ret_val ;
  }
  switch (ev_nr)
  {
    case IDM_CLOSE :

      ret=Get_Begin_Pline (BLK_ADR, &ptr_ob, &b_first_end, &df__xbeg, &df__ybeg);

      if (ns_arc != ARC_Con || FALSE == Get_End_Pline (BLK_ADR /*(void*)dane*/,  &ptr_ob, &b_first_end,
      df__xbeg, df__ybeg, &df_xend, &df_yend))
      {
	ret_val = PL_MODE_CONTINUE ;
      }
      else
      {
	Cur_offd (X, Y) ;
	add_arc (df__xbeg, df__ybeg, 0) ;
	Cur_ond (X, Y) ;
	ret_val = PL_MODE_END ;
      }
      break ;
    case IDM_UNDO :
      if (NULL == (ptr_temp = Get_Pline_Last_Ob (BLK_ADR /*(void*)dane*/)))
      {
	ret_val = PL_MODE_UNDO ;
      }
      else
      {
	Cur_offd(X,Y);
	rysuj_obiekt ((char*)ptr_temp, COPY_PUT, 0) ;
	Usun_Object (ptr_temp, FALSE) ;
	Get_End_Pline (BLK_ADR /*(void*)dane*/, &ptr_ob, &b_first_end,
		df__xbeg, df__ybeg, &df_xend, &df_yend) ;
	pl.xs = df_xend ;
	pl.ys = df_yend ;

	Cur_ond (X, Y) ;
	ret_val = PL_MODE_UNDO_CONT ;
      }
      break ;
    case IDM_LINE :
      ret_val = PL_MODE_LINE ;
      break ;
      /*
    case IDM_CONTINUOUS_LINE :
      Cur_offd (X, Y) ;
      LiniaG.typ=LukG.typ=96;
      view_line_type( &LukG ) ;
      Cur_ond (X, Y) ;
      ret_val = PL_MODE_CONTINUE ;
      break;
    case IDM_DASHED_LINE :
      Cur_offd (X, Y) ;
      LiniaG.typ=LukG.typ=65;
      view_line_type( &LukG ) ;
      Cur_ond (X, Y) ;
      ret_val = PL_MODE_CONTINUE ;
      break;
       */
      case IDM_FREE_EDGE:
          LiniaG.obiektt2=O2FREE_EDGE;
          LiniaG.obiektt3=O3REGULAR_EDGE;
          LukG.obiektt2=O2FREE_EDGE;
          LukG.obiektt3=O3REGULAR_EDGE;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_PINNED_EDGE:
          LiniaG.obiektt2=O2HINGED_EDGE;
          LiniaG.obiektt3=O3REGULAR_EDGE;
          LukG.obiektt2=O2HINGED_EDGE;
          LukG.obiektt3=O3REGULAR_EDGE;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_FIXED_EDGE:
          LiniaG.obiektt2=O2FIXED_EDGE;
          LiniaG.obiektt3=O3REGULAR_EDGE;
          LukG.obiektt2=O2FIXED_EDGE;
          LukG.obiektt3=O3REGULAR_EDGE;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_FLIP_SUPPORT:
          LiniaG.obiektt3=!LiniaG.obiektt3;
          LukG.obiektt3=!LukG.obiektt3;
          ret_val = PL_MODE_CONTINUE ;
          break;
    default :
      ret_val = PL_MODE_CONTINUE ;
      break ;
  }
  return ret_val ;
}

static int last_parc_delete (void)
/*-----------------------------*/
{
  PLine_Arc_Command_Proc (IDM_UNDO, BLK_ADR);
  return -83;
}


int Pline_Arc (double df_xbeg, double df_ybeg, BLOK **blk_adr, int mode)
/*-------------------------------------------------------------------*/
{
  double X0, Y0, df_xend, df_yend ;
  BOOL b_first_end, b_second_pl_seg ;
  void *ptr_ob ;
  int ret_command ;
  char st[6];
  static char stY [2]=YES, stN [2]=NO;

  BLK_ADR=*blk_adr;

  b__pline = TRUE ;
  redcr0 (0) ;
  df__xbeg = df_xbeg ;
  df__ybeg = df_ybeg ;
  while (1)
  {
      if (Get_reversed()) strcpy(st,stY);
      else strcpy(st,stN);
	 if (OBRYS==TRUE)
     {
         menupini (&mPLukmObrys, _ARC_, _ARC_C_, 15) ;
         menu_par_new ((*mPLukmObrys.pola)[8].txt, st);
     }
       else
     {
           if (mode==1) menupini (&mPLukmSlab, _ARC_, _ARC_C_, 15) ;
           else menupini (&mPLukm, _ARC_, _ARC_C_, 15) ;
           menu_par_new ((*mPLukm.pola)[8].txt, st);
     }
     komunikat0_str (ns_arc, u8"") ;
     kom (ns_arc, '-', r6, 0) ;
     BLK_ADR=(BLOK*)PTR__GTMPBLOCK;
     b_second_pl_seg = Get_End_Pline (BLK_ADR /*(void*)dane*/, &ptr_ob, &b_first_end,
     df_xbeg, df_ybeg, &df_xend, &df_yend) ;
     pl.xs = df_xend ;
     pl.ys = df_yend ;
     out_krz (pl.xs, pl.ys) ;
     pl.ptr_con = ptr_ob ;
     pl.b_first_end = b_first_end ;
     pl.con_angle = 0 ;
     if (TRUE == b_second_pl_seg)
     {
       pl.con_angle = get_continue_angle (pl.ptr_con, pl.b_first_end) ;
     }
     if (ns_arc == ARC_Con)
     {
       redcr (0) ;
     }
     else
     {
       kom (ns_arc, '-', r6, 1) ;
     }
     ret_command = getwsp_pl (&X0, &Y0) ;

     if (ns_arc == ARC_Con)
     {
       redcr (1) ;
     }
     if (ret_command == PL_MODE_ARC_ESC)
     {
       ret_command = PL_MODE_END ;
       break ;
     }
     else
     if (ret_command == PL_MODE_ARC_CR)
     {
       menupini (NULL, u8"", ' ',0) ;
       (*poczatekl [ns_arc]) (X0, Y0) ;
     }
     else
     if (ret_command == PL_MODE_UNDO_CONT)
     {
       continue ;
     }
     else
     {
	break ;
     }
  }
  redcr0 (1) ;

  *blk_adr=BLK_ADR;

  return ret_command ;
}


int start_trace_arc (double df_xbeg, double df_ybeg, LINIA *s_trace_line, BOOL b_line, int d_line)
/*--------------------------------------------------------------------------------------------*/
{
    double X0, Y0, df_xend, df_yend ;
    BOOL b_first_end, b_second_pl_seg ;
    void *ptr_ob ;
    int ret_command ;

    b__pline = TRUE ;
    redcr0 (10) ;
    df__xbeg = df_xbeg ;
    df__ybeg = df_ybeg ;
    while (1)
    {
        komunikat0_str (ns_arc, u8"") ;
        kom (ns_arc, '-', r6, 0) ;

        if(get_AfterTbreak())
        {
            ptr_ob=&sa_break.sa;
            df_xend=sa_break.df_xend;
            df_yend=sa_break.df_yend;

            b_second_pl_seg=sa_break.b_second_pl_seg;
            b_first_end=sa_break.b_first_end;
            set_AfterTbreak(0);

            pl.xs = df_xend ;
            pl.ys = df_yend ;

            out_krz (pl.xs, pl.ys) ;
            pl.ptr_con = ptr_ob ;
            pl.b_first_end = b_first_end ;

            if (pl.b_first_end) pl.con_angle = Angle_Normal(sa_break.angle - Pi_/2)*180/Pi_;
            else  pl.con_angle = Angle_Normal(sa_break.angle + Pi_/2)*180/Pi_;
        }
        else
        {
            if ((s_trace_line != NULL) && (b_line))
            {
                ptr_ob = s_trace_line;
                df_xend = s_trace_line->x2;
                df_yend = s_trace_line->y2;
                b_second_pl_seg = TRUE;
                b_first_end = FALSE;
                s_trace_line = NULL;
            }
            else

                b_second_pl_seg = Get_End_Trace((void *) dane, &ptr_ob, &b_first_end, df_xbeg, df_ybeg, &df_xend,
                                                &df_yend);

            pl.xs = df_xend ;
            pl.ys = df_yend ;
            out_krz (pl.xs, pl.ys) ;
            pl.ptr_con = ptr_ob ;
            pl.b_first_end = b_first_end ;
            pl.con_angle = 0 ;
            if (TRUE == b_second_pl_seg)
            {
                pl.con_angle = get_continue_angle (pl.ptr_con, pl.b_first_end) ;
            }
        }

        if (ns_arc == ARC_Con)
        {
            redcr (0) ;
        }
        else
        {
            kom (ns_arc, '-', r6, 1) ;
        }
        ret_command = getwsp_tr (&X0, &Y0, d_line) ;

        if (ns_arc == ARC_Con)
        {
            redcr (1) ;
        }
        if (ret_command == PL_MODE_ARC_ESC)
        {
            ret_command = PL_MODE_END ;
            set_Tbreak(0);
            set_AfterTbreak(0);

            s_trace_line=NULL;

            break ;
        }
        else if (ret_command == PL_MODE_ARC_CR)
        {
             (*poczatekt [ns_arc]) (X0, Y0) ;

            s_trace_line=NULL;

             if (get_Tbreak())
             {
                 set_Tbreak(0);
                 set_AfterTbreak(1);
             }
        }
        else if (ret_command == PL_MODE_ARC_EDIT)
        {
            s_trace_line=NULL;

            if (get_Tbreak())
            {
                set_Tbreak(0);
                set_AfterTbreak(1);
            }
        }
        else if (ret_command == PL_MODE_UNDO_CONT)
        {
            //check if mode is changed
            if (get_pline_mode()==PL_MODE_LINE)
            {
                ret_command=PL_MODE_LINE_BAK;
                break;
            }
            continue ;
        }
        else if (ret_command == PL_MODE_UNDO)
        {
            continue ;
        }
        else if (ret_command == PL_MODE_BREAK)
        {

            set_Tbreak(1);

        }
        else
        {
            break ;
        }
    }
    redcr0 (11) ;
    return ret_command ;
}

#undef __O_LUK__