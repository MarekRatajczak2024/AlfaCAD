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

#define __O_ETEXT__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include <ctype.h> //needed by CLion
#include "bib_e.h"
#include "rysuj_e.h"
#include "alffont.h"

#include "menu.h"

extern void Mark_Texts (int opcja);
extern void Unmark_Texts (int opcja);
extern BOOL Wejscie_Text(void *ad);
extern void  edit_text_tab_f2(void  *ad);
extern void  edit_text_f3(void  *ad);
extern void setcolor(int kolor);
extern void outtextxy_w_ (TEXT *t0, int mode);
extern void delay(int time);
extern void setwritemode( int mode );
extern BOOL TTF_redraw;
extern T_Font_Header *PTRS__Text_Style [MaxNumberTextStyle] ;
extern void DF_to_String (char *, char *, double, int) ;

extern BOOL Semaphore;

void  Edit_Text(void  *ad);
static int  nooop1 (void)
{return 0;}

static void *obiekt_wybrany(unsigned *typ)
/*---------------------------------------*/
{
  return select_w(typ, NULL);
}


static void redcr(char typ)
/*------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[9])(),sel_akt,sel_cur;

  if (typ==0)
   {
     CUR_OFF (X,Y);
	 sel_akt = sel.akt;
     CUR_oFF=CUR_OFF; CUR_OFF=out_sel_off;
     CUR_oN=CUR_ON;   CUR_ON=out_sel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;

     SW [2] = SERV [58];
     SW [3] = SERV [59];
     SW [4] = SERV [60];
     SW [5] = SERV [61];
     SW [6] = SERV [62];
     SW [7] = SERV [63];
     SW [8] = SERV [64];

     SERV [58] = nooop1;
     SERV [59] = nooop1;
     SERV [60] = nooop1;
     SERV [61] = nooop1;
     SERV [62] = nooop1;
     SERV [63] = nooop1;
     SERV [64] = nooop1;

     komunikat (0) ;
     komunikat0 (39) ;
     CUR_ON(X,Y);
   }
  else
   {
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];

     SERV [58] = SW [2];
     SERV [59] = SW [3];
     SERV [60] = SW [4];
     SERV [61] = SW [5];
     SERV [62] = SW [6];
     SERV [63] = SW [7];
     SERV [64] = SW [8];

     sel.akt=sel_akt;
     komunikat (0) ;
     komunikat0 (0) ;
     CUR_OFF(X,Y);
     CUR_ON (X, Y) ;
   }
}

static void redcrwe(char typ)
/*------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[9])(),sel_akt,sel_cur;

  if (typ==0)
   {
     CUR_OFF (X,Y);
     sel_akt=sel.akt; sel.akt=0;
     sel_cur=sel.cur; sel.cur=1;
     CUR_oFF=CUR_OFF; CUR_OFF=out_sel_off;
     CUR_oN=CUR_ON;   CUR_ON=out_sel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;

     SW [2] = SERV [58];
     SW [3] = SERV [59];
     SW [4] = SERV [60];
     SW [5] = SERV [61];
     SW [6] = SERV [62];
     SW [7] = SERV [63];
     SW [8] = SERV [64];

     SERV [58] = nooop1;
     SERV [59] = nooop1;
     SERV [60] = nooop1;
     SERV [61] = nooop1;
     SERV [62] = nooop1;
     SERV [63] = nooop1;
     SERV [64] = nooop1;


     komunikat (0) ;
     komunikat0 (113) ;
     CUR_ON(X,Y);
   }
  else
   { CUR_OFF(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];

     SERV [58] = SW [2];
     SERV [59] = SW [3];
     SERV [60] = SW [4];
     SERV [61] = SW [5];
     SERV [62] = SW [6];
     SERV [63] = SW [7];
     SERV [64] = SW [8];

     sel.akt=sel_akt;
     sel.cur=sel_cur;
     komunikat (0) ;
     komunikat0 (0) ;
     CUR_ON (X, Y) ;
   }
}


void Wejscie(void)


{
  void *ad;
  unsigned typ;
  EVENT *ev;
  double X0, Y0;

  redcrwe(0);
  while(1)
   {  
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What==evKeyDown)
      { if(ev->Number==0)
	 { redcrwe(1);
	   return;
	 }
	if(ev->Number==ENTER)
	 { CUR_OFF(X,Y);
	   typ = Btekst ; //| Blinia | Bluk | Bokrag | Bkolo | Bwwielokat
	   if( (ad=obiekt_wybrany(&typ))!=NULL)
	   {
	     switch (typ)
	     {
		case Btekst : if (FALSE == Wejscie_Text (ad)) CUR_ON(X,Y);
		  break;
		default : CUR_ON(X,Y);
		  break;
	     }
	   }
	   else
	    {
	     CUR_ON(X,Y);
	    } 
	 }
      }
   }  
}

int chage_element_no(AVECTOR *V) {
    char sk[MaxTextLen];
    int retval_no = 1;
    double buf_ret[1];
    int d;

    d=(int)V->property_no;
    sprintf(sk, "%d", d);
    if (!get_string(&sk, "", MaxTextLen, 0, 211)) return -1;
    if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1) {
        return -1;
    }
    d = buf_ret[0];
    if (d < 0) {
        ErrList(16);
        return -1;
    }

    return d;
}

void Edit_Obiekt(void)
/*-----------------*/
{
  void *ad;
  unsigned int typ;
  EVENT *ev;
  double X0, Y0;
  TEXT *T, t=Tdef;
  AVECTOR *V;
  int prop_no_bak, prop_no;

  TTF_redraw=FALSE;
  redcr(0);
  while (1)
  {
	  ev = Get_Event_Point(NULL, &X0, &Y0);
      
	  if (ev->What == evKeyDown)
	  {
		  if (ev->Number == 0)
		  {
			  redcr(1);
			  return;
		  }
		  if (ev->Number == ENTER)
		  {
			  CUR_OFF(X, Y);
			  typ = Btekst | Bvector;
			  if ((ad = obiekt_wybrany(&typ)) != NULL)
			  {
				  switch (typ)
				  {
				  case Btekst:
					  if (Layers[((TEXT*)ad)->warstwa].edit != 0)
					  {
						  T = (TEXT*)ad;
						  if (((T->typ == n_typ_atrybut) || (T->typ == n_typ_komentarz)) && (T->blok == 1) && (T->obiektt2 == O2BlockSpecial))
						  {

                              TTF_redraw = FALSE;
                              CUR_OFF(X, Y);
							  Edit_Text(ad);
							  if (TTF_redraw) redraw();
						  }
						  else
						  {
                             
                              TTF_redraw = FALSE;
                              CUR_OFF(X, Y);
							  Edit_Text(ad);
							  if (TTF_redraw) redraw();

							  if (Break_Edit_Text == TRUE)
							  {
								  redcr(1);
								  return;
							  }
						  }
                      }
                      CUR_ON(X, Y);
					  break;
                      case Bvector:
                          if (Layers[((AVECTOR*)ad)->warstwa].edit != 0) {
                              V = (AVECTOR*)ad;
                              switch (V->style)
                              {
                                  case 0:
                                  case 1:
                                  case 2:
                                  case 3:
                                      prop_no_bak=V->property_no;
                                      sprintf(t.text, "%d", V->property_no);
                                      prop_no=chage_element_no(V);
                                      if ((prop_no>=0) && (prop_no != prop_no_bak))
                                      {
                                          rysuj_obiekt(V, 0, 0);
                                          V->property_no=(short)prop_no;
                                          rysuj_obiekt(V, 0, 1);
                                          if (PTRS__Text_Style[zmwym.czcionka]->type == 2) redraw();
                                          else CUR_OFF(X, Y);
                                      }
                                      CUR_ON(X, Y);
                                      break;
                                  default:
                                      CUR_ON(X, Y);
                                      break;
                              }
                          }
				  default:
					  CUR_ON(X, Y);
					  break;
				  }
			  }
			  else CUR_ON(X, Y);
		  }
	  }
  }
}

int Edit_Text_Tab_F2(void)
/*-----------------------*/
{
  void *ad;
  unsigned int typ;
  EVENT *ev;
  double X0, Y0;

   redcr(0);
   typ = Btekst;
   if( (ad=obiekt_wybrany(&typ))!=NULL)
   {
     switch (typ)
     {
		case Btekst :
		  if (Layers [((TEXT*)ad)->warstwa].edit != 0)
		  {
			TTF_redraw = FALSE;
		    edit_text_tab_f2 (ad);
		  }
		  break;
		default :
		  break;
	  }
     CUR_ON(X, Y);
	}
   redcr(1);
   if (TTF_redraw) redraw();
   return 0;
}

static int nooop(void)
{
 return 0;
}

int Edit_Text_F3(void)
/*------------------*/
{
  void *ad;
  unsigned int typ;
  EVENT *ev;
  double X0, Y0;

   //zablokowanie funkcji TAB dla profili

   redcr(0);
   typ = Btekst;
   if( (ad=obiekt_wybrany(&typ))!=NULL)
   {
     switch (typ)
     {
		case Btekst :
		  if (Layers [((TEXT*)ad)->warstwa].edit != 0)
		  {
			TTF_redraw = FALSE;
		    edit_text_f3 (ad);
			
		  }
		  break;
		default :
		  break;
	  }
     CUR_ON(X, Y);
	}
   redcr(1);
   if (TTF_redraw) redraw();
   return 0;
}

void Edit_Obiekt_F3(void)
{
    int ret = Edit_Text_F3();
}


/**/
void strupr1(char *st)
/*-------------------*/
{int i;
 int st_len;
 
 st_len=strlen(st);
 if (st_len==0) return;
 for (i=0; i<st_len; i++)
 {
 switch (st[i])
  {   
   case '\206' /*134*/ : st[i]='\217' /*143*/;
   break;
   case '\215' /*141*/ : st[i]='\225' /*149*/;
   break;
   case '\221' /*145*/ : st[i]='\220' /*144*/;
   break;
   case '\222' /*146*/ : st[i]='\234' /*156*/;
   break;
   case '\244' /*164*/ : st[i]='\245' /*165*/;
   break;
   case '\242' /*162*/ : st[i]='\243' /*163*/;
   break;
   case '\236' /*158*/ : st[i]='\230' /*152*/;
   break;
   case '\247' /*167*/ : st[i]='\241' /*161*/;
   break;
   case '\246' /*166*/ : st[i]='\240' /*160*/;
   break;
   default : st[i]=toupper(st[i]);
   break;
  }   
 }    
}     
/**/

void Find_Text (void)
/*--------------------------------------------------------------*/
{ char st[MaxTextLen];
  char st_source[MaxTextLen];
  BOOL b_retval;
  double xd, yd;
  TEXT  *T;
  BLOK  *b;
  //LINIA *L;
  NAGLOWEK *nag;
  void  *ad;
  char  *adp,  *adk;
  int key;
  BOOL once_again;
  char *p1;

  Semaphore=FALSE;

  strcpy(st_source,find_text);
  if (!get_string (&st_source, "", MaxTextLen, 0, 102))
   {
    return;
   }
  if (strlen(st_source)==0) 
   {
    return;
   }
  strcpy(find_text,st_source); 
  strupr1(st_source); 
 
  
find_again:
  
  once_again=FALSE;

  adp = dane ;
  adk = dane + dane_size ;
  
  if (last_find_addr==NULL) last_find_addr = adp;
  
  if (last_find_addr>adp)
   {
    once_again=TRUE;
   }
  
  if (last_find_addr<adp)
   {
    last_find_addr=adp;
   }
   
  if (last_find_addr>adk)
   {
    last_find_addr=adp;
   } 
   
  nag = (NAGLOWEK*)adp;
  while (nag->obiekt != Okoniec && adp <= adk)
  {
    if (nag->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    { if (adp>=last_find_addr) 
      {
      if ((nag->obiekt == Otekst) && (nag->atrybut != Abad) && (nag->atrybut != Ausuniety))
       {
        T = (TEXT*)adp;
        if (Layers[T->warstwa].on==TRUE)
           {
               strcpy(st, T->text);
               strupr1(st);
                if (strlen(st) > 0) {
                   p1 = strstr(st, st_source);
                   if (p1 != NULL) {
                       xd = T->x;
                       yd = T->y;
                       mvcurp(xd - X, yd - Y);
                       last_find_addr = (adp + 1);
                       return;
                   }
               }
           }
       }
      } 
      adp += nag->n + sizeof(NAGLOWEK) ;
    }
    nag = (NAGLOWEK*)adp;
  }
  last_find_addr = dane;
  if (once_again==TRUE)
   {
   goto find_again;
   }
  komunikat(88);

  key = Get_Legal_Key (_YES_NO_ESC_); 

  komunikat(0); 
  return;
}

POLE pmenu_opcje[]={
    {u8"*A* -> *B*",'1',611,NULL},
	{u8"*A  -> *B",'2',612,NULL},
	{u8" A* ->  B*",'3',613,NULL},
	{u8" A  ->  B",'4',614,NULL},
        };
TMENU menu_opcje_=
	{4,0,0,17,40,3,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmenu_opcje,NULL,NULL};

TMENU menu_typ_={17,0,0,16,45,3,0,CMNU,CMBR,CMTX,0,0,0,0,0,&pmenu_typ,NULL,NULL};

TMENU menu_typ1_={18,0,0,16,45,3,0,CMNU,CMBR,CMTX,0,0,0,0,0,&pmenu_typ1,NULL,NULL};

TMENU menu_auto_={2,0,0,16,45,3,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmenu_auto,NULL,NULL};

TMENU menu_change_={2,0,0,5,45,3,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmenu_change,NULL,NULL};

void Find_and_Change_Text (void)
/*-----------------------------*/
{ char st[MaxTextLen];
  char st_source[MaxTextLen];
  char st_dest[MaxTextLen];
  char st_dest1[MaxTextLen];
  char prefiks[MaxTextLen];
  char sufiks[MaxTextLen];
  BOOL b_retval;
  double xd, yd;
  TEXT  *T;
  BLOK  *b;
  LINIA *L;
  NAGLOWEK *nag;
  void  *ad;
  char  *adp,  *adk;
  int key;
  BOOL once_again;
  int l_changed, l_founded;
  BOOL auto_change;
  TEXT Tp, *t;
  long_long del_adp;
  TMENU *menu_opcje=&menu_opcje_;
  TMENU *menu_typ=&menu_typ_;
  TMENU *menu_auto=&menu_auto_;
  TMENU *menu_change=&menu_change_;
  int n1;
  int typ_tekstu;
  int change_option;
  BOOL founded;
  char *p1;

  Semaphore=FALSE;

  Mark_Texts(0);
  komunikat0 (0) ;
  
  l_changed=0;
  l_founded=0;
  auto_change=TRUE;
  strcpy(st_source,change_text_source);
  if (!get_string (&st_source, "", MaxTextLen, 0, 103))
   {
    Unmark_Texts(3);
    return;
   }

  strcpy(change_text_source,st_source);
  
  strcpy(st_dest,change_text_dest);
  if (!get_string (&st_dest, "", MaxTextLen, 0, 104))
   {
    Unmark_Texts(3);
    return;
   }

  strcpy(change_text_dest,st_dest); 

  if (strlen(st_source)==0)
   {
    change_option = 1;
   }
   else
   {
  /*1.wybor opcji zmiany:  jest->ma byc, jest?-ma_byc?, ?jest->?ma_byc, ?jest?->?ma_byc?*/ 
    n1 = getwsp1(menu_opcje) ;
    if (n1==0)
     {
      Unmark_Texts(3);
      return;
     }
    change_option = n1;
   }
  
  /*2.wybor typu tekstu*/
  typ_tekstu=-1;


  /*3.zmiana automatyczna lub potwierdzana*/
  n1 = getwsp1(menu_auto) ;
  if (n1==0) 
   {
    Unmark_Texts(3);
    return;
   } 
  if (n1==1) auto_change=TRUE; else auto_change=FALSE;
  
  adp = dane ;
  adk = dane + dane_size ;
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk)
  {
    if (L->obiekt == OdBLOK)
    {
  	  b=(BLOK *)adp;
	  adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    { 
      if ((L->obiekt == Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE) && (L->atrybut == Ablok) /*blok zaznaczony*/)
       {
        T = (TEXT*)adp;
        if ((typ_tekstu<0) || (typ_tekstu==T->typ))
        {
        strcpy(st,T->text);
        if (strlen(st)>0)
          {
           founded=FALSE;
           /*sprawdzenie zgodnosci tekstow wg. change_option*/
           switch (change_option)
           {
           case 1:   // *A*
                  if (strlen(st_source)==0)
                  {
                   founded=TRUE;
                   strcpy(prefiks,"");
                   strcpy(sufiks,"");
                  }
                   else
                  {
                   p1=strstr(st,st_source);
                   if (p1!=NULL)
                   {
                    founded=TRUE;
                    if ((p1 - st)==0) strcpy(prefiks,"");
                     else
                      {
                       // strncpy(prefiks,st,p1 - st);
                        strcpy(prefiks,st);
                        prefiks[p1-st]='\0';
                      }
                    strcpy(sufiks,p1 + strlen(st_source));
                   }
                  }
                  break;
           case 2:   // *A
                  p1=strstr(st,st_source);
                  if (p1!=NULL)
                  {
                   if ((p1 - st + strlen(st_source))==strlen(st))
                    {
                     founded=TRUE;
                     if ((p1 - st)==0) strcpy(prefiks,""); 
                       else 
                         {
                           //strncpy(prefiks,st,p1 - st);
                           strcpy(prefiks,st);
                           prefiks[p1-st]='\0';
                         }  
                     strcpy(sufiks,"");
                    }
                  } 
                  break;        
           case 3:   // A*
                  if (strncmp(st_source,st,strlen(st_source))==0)
                   {
                    founded=TRUE;
                    strcpy(prefiks,"");
                    if (strlen(st_source)==strlen(st)) strcpy(sufiks,"");
                     else
                     {
                      p1 = st + strlen(st_source);
                      strcpy(sufiks,p1); 
                     } 
                   } 
                  break; 
           case 4:   // A
                  if (strcmp(st_source,st)==0)
                  {
                   founded=TRUE;
                   strcpy(prefiks,"");
                   strcpy(sufiks,"");
                  } 
                  
                  break;         
           default:  // A
                  if (strcmp(st_source,st)==0)
                  {
                   founded=TRUE;
                   strcpy(prefiks,"");
                   strcpy(sufiks,"");
                  } 
                  break;       
           }
           if (founded==TRUE)
           {
            strcpy(st_dest1,prefiks);
            strcat(st_dest1,st_dest);
            strcat(st_dest1,sufiks);
            l_founded+=1; 
            del_adp=adp-dane;
            if (auto_change==FALSE)
             {
              xd=T->x;
              yd=T->y;
              mvcurp( xd - X, yd - Y ) ;
              /**/
              n1 = getwsp1(menu_change) ;
              if (n1==0) 
               {
                 if (l_changed>0) 
                  {
                   ADP=dane;
                   ADK=dane + dane_size;
                   Unmark_Texts(4); 
                   redraw();
                  }
                  else Unmark_Texts(3);  
                 return;
               }
               
              if (n1==1)
               {   
                 Cur_offd (X, Y) ;
                 setcolor(kolory.paper);  okno_r ();
                 outtextxy_w_(T,COPY_PUT); okno_all ();
                 memmove (&Tp, T, sizeof(NAGLOWEK)+T->n);
                 strcpy (Tp.text, st_dest1);
                 Tp.dl = strlen(Tp.text);
                 Tp.n = T18 + Tp.dl;
				 Tp.width = 0;
				 Tp.height = 0;
                 if ( (t = korekta_obiekt((void *)adp, (void *)&Tp)) == NULL) return;
                 adp=(char *)t;
                 adk=dane+dane_size;
                 L=(LINIA*)adp;

                 l_changed+=1; 
                 Cur_offd (X, Y) ;
                 outtextxy_w (t, COPY_PUT);
               } 
             } 
              else
               {
                 memmove (&Tp, T, sizeof(NAGLOWEK)+T->n);
                 strcpy (Tp.text, st_dest1);
                 Tp.dl = strlen(Tp.text);
                 Tp.n = T18 + Tp.dl;
				 Tp.width = 0;
				 Tp.height = 0;
                 if ( (t = korekta_obiekt((void *)adp, (void *)&Tp)) == NULL) return;
                 adp=(char *)t;
                 adk=dane+dane_size;
                 L=(LINIA*)adp;

                 l_changed+=1; 
               }
           } 
          }
        }  
       }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (NAGLOWEK*)adp;
  }
  /*jezeli nie znaleziono zadnego*/
  if (l_founded==0)
   {
    Unmark_Texts(3); 
    komunikat(88);

	key = Get_Legal_Key(_YES_NO_ESC_);

    komunikat(0); 
   }
   else
    {
     if (l_changed>0) 
      {
       ADP=dane;
       ADK=dane + dane_size;
       Unmark_Texts(4); 
       redraw();
       komunikat(105);
       delay(1000);
       komunikat(0);
      } 
       else Unmark_Texts(3); 
    } 
  return;
}

void Automatic_numbering (void)
/*---------------------------*/
{ char st[MaxTextLen];
  char st_source[MaxTextLen];
  char st_dest[MaxTextLen];
  char st_dest1[MaxTextLen];
  char prefiks[MaxTextLen];
  char sufiks[MaxTextLen];
  char st_dest_symbol[MaxTextLen];
  char st_dest_numer[MaxTextLen];
  
  char tekst1[MaxTextLen];

  Semaphore = FALSE;
  
  int st_dest_numer_i, ii, end_char;
  BOOL b_retval;
  double xd, yd;
  TEXT  *T;
  BLOK  *b;
  LINIA *L;
  NAGLOWEK *nag;
  void  *ad;
  char  *adp,  *adk;
  int key;
  BOOL once_again;
  int l_changed, l_founded, li_founded;
  BOOL auto_change;
  TEXT Tp, *t;
  long_long del_adp;
  TMENU *menu_opcje=&menu_opcje_;
  TMENU *menu_typ=&menu_typ_;
  TMENU *menu_typ1=&menu_typ1_;
  TMENU *menu_auto=&menu_auto_;
  TMENU *menu_change=&menu_change_;
  int n1;
  int typ_tekstu;
  int change_option;
  BOOL founded;
  char *p1;
  int menu_typ_poz, menu_typ_off;


  Mark_Texts(1);
  komunikat0 (0) ;
  
  l_changed=0;
  l_founded=0;
  li_founded=0;
  
  auto_change=TRUE;
  strcpy(st_source,change_text_source);
  if (!get_string (&st_source, "", MaxTextLen, 0, 103))
   {
    Unmark_Texts(3);
    return;
   }

  strcpy(change_text_source,st_source);
  
  strcpy(st_dest,change_text_dest);
  if (!get_string (&st_dest, "", MaxTextLen, 0, 118 /*104*/))
   {
    Unmark_Texts(3);
    return;
   }
  if (strlen(st_dest)==0) 
   {
    Unmark_Texts(3);
    return;
   }
   
  strcpy(change_text_dest,st_dest); 
  /*rozbior st_dest ma symbol o numer*/
  /*poszukiwanie ostatniej cyfry*/
  end_char = strlen(st_dest) - 1;
  
  while ((end_char>=0)  && (st_dest[end_char] > '\057') && (st_dest[end_char] < '\072'))
   {
     end_char--;  
   }
   
  if (end_char==-1) 
   {
    strcpy(st_dest_symbol,"");
    st_dest_numer_i=atoi(st_dest);
   }
   else
   {
     strcpy(st_dest_symbol,st_dest);
     st_dest_symbol[end_char+1]='\0';
     
     strcpy(st_dest_numer, st_dest + end_char + 1);
     st_dest_numer_i=atoi(st_dest_numer); 
   } 
   
  
  if (strlen(st_source)==0)
   {
    change_option = 1;
   }
   else
   {
    /*1.wybor opcji zmiany:  jest->ma byc, jest?-ma_byc?, ?jest->?ma_byc, ?jest?->?ma_byc?*/ 
    n1 = getwsp1(menu_opcje) ;
    if (n1==0) 
     {
      Unmark_Texts(3);
      return;
     } 
    change_option = n1;
   } 
  
  /*2.wybor typu tekstu*/
  typ_tekstu=-1;
  

    
//  sprintf(tekst1,"%#ld",typ_tekstu);
//  komunikat_str(tekst1);  delay(2000);  
  
  /*3.zmiana automatyczna lub potwierdzana*/
//  n1 = getwsp1(menu_auto) ;
//  if (n1==0) 
//   {
//    Unmark_Texts(3);
//    return;
//   } 
//  if (n1==1) auto_change=TRUE; else auto_change=FALSE;
  
  /*****************************************************************/ 
  /*szukanie liczby oraz wspolrzednej X kolejnych symboli do zmiany*/
  adp = dane ;
  adk = dane + dane_size ;
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    { 
      if ((L->obiekt == Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE) && (L->atrybut == Ablok) /*blok zaznaczony*/)
       {
        T = (TEXT*)adp;
        if ((typ_tekstu<0) || (typ_tekstu==T->typ))
        {
        strcpy(st,T->text);
        if (strlen(st)>0)
          {
           founded=FALSE;
           /*sprawdzenie zgodnosci tekstow wg. change_option*/
           switch (change_option)
           {
           case 1:   // *A*
                  if (strlen(st_source)==0) founded=TRUE;
                   else
                   { 
                    p1=strstr(st,st_source);
                    if (p1!=NULL)
                     {
                      founded=TRUE;
                      if ((p1 - st)==0) strcpy(prefiks,""); 
                      else 
                        {
                          //strncpy(prefiks,st,p1 - st);
                          strcpy(prefiks,st);
                          prefiks[p1-st]='\0';
                        }  
                      strcpy(sufiks,p1 + strlen(st_source));
                     } 
                   }  
                  break;
           case 2:   // *A
                  p1=strstr(st,st_source);
                  if (p1!=NULL)
                  {
                   if ((p1 - st + strlen(st_source))==strlen(st))
                    {
                     founded=TRUE;
                     if ((p1 - st)==0) strcpy(prefiks,""); 
                       else 
                        {
                         //strncpy(prefiks,st,p1 - st);
                         strcpy(prefiks,st);
                         prefiks[p1-st]='\0';
                        } 
                     strcpy(sufiks,"");
                    }
                  } 
                  break;        
           case 3:   // A*
                  if (strncmp(st_source,st,strlen(st_source))==0)
                   {
                    founded=TRUE;
                    strcpy(prefiks,"");
                    if (strlen(st_source)==strlen(st)) strcpy(sufiks,"");
                     else
                     {
                      p1 = st + strlen(st_source);
                      strcpy(sufiks,p1); 
                     } 
                   } 
                  break; 
           case 4:   // A
                  if (strcmp(st_source,st)==0)
                  {
                   founded=TRUE;
                   strcpy(prefiks,"");
                   strcpy(sufiks,"");
                  } 
                  break;         
           default:  // A
                  if (strcmp(st_source,st)==0)
                  {
                   founded=TRUE;
                   strcpy(prefiks,"");
                   strcpy(sufiks,"");
                  } 
                  break;       
           }
           if (founded==TRUE)
           {
            li_founded+=1;
           } 
          }
        }  
       }
      L = (LINIA*)adp;
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (NAGLOWEK*)adp;
  }
  /*****************************/
  if (li_founded==0)
   {
    Unmark_Texts(3); 
    komunikat(88);
    key = Get_Legal_Key ("TNtn\033"); 
    komunikat(0); 
    return;
   }
   
   st_dest_numer_i+=(li_founded-1);
  /*****************************/
  
  adp = dane ;
  adk = dane + dane_size ;
  
  L = (LINIA*)adp;
  while (L->obiekt != Okoniec && adp <= adk)
  {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    { 
      if ((L->obiekt == Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety) 
         && (Layers[L->warstwa].on==TRUE) && (L->atrybut == Ablok) /*blok zaznaczony*/)
       {
        T = (TEXT*)adp;
        if ((typ_tekstu<0) || (typ_tekstu==T->typ))
        {
        strcpy(st,T->text);
        if (strlen(st)>0)
          {
           founded=FALSE;
           /*sprawdzenie zgodnosci tekstow wg. change_option*/
           switch (change_option)
           {
           case 1:   // *A*
                  if (strlen(st_source)==0)
                   {
                    founded=TRUE;
                    strcpy(prefiks,"");
                    strcpy(sufiks,"");
                   }
                   else
                   { 
                    p1=strstr(st,st_source);
                    if (p1!=NULL)
                     {
                      founded=TRUE;
                      if ((p1 - st)==0) strcpy(prefiks,""); 
                      else 
                        {
                          //strncpy(prefiks,st,p1 - st);
                          strcpy(prefiks,st);
                          prefiks[p1-st]='\0';
                        }  
                      strcpy(sufiks,p1 + strlen(st_source));
                     } 
                   }  
                  break;
           case 2:   // *A
                  p1=strstr(st,st_source);
                  if (p1!=NULL)
                  {
                   if ((p1 - st + strlen(st_source))==strlen(st))
                    {
                     founded=TRUE;
                     if ((p1 - st)==0) strcpy(prefiks,""); 
                       else 
                        {
                         //strncpy(prefiks,st,p1 - st);
                         strcpy(prefiks,st);
                         prefiks[p1-st]='\0';
                        } 
                     strcpy(sufiks,"");
                    }
                  } 
                  break;        
           case 3:   // A*
                  if (strncmp(st_source,st,strlen(st_source))==0)
                   {
                    founded=TRUE;
                    strcpy(prefiks,"");
                    if (strlen(st_source)==strlen(st)) strcpy(sufiks,"");
                     else
                     {
                      p1 = st + strlen(st_source);
                      strcpy(sufiks,p1); 
                     } 
                   } 
                  break; 
           case 4:   // A
                  if (strcmp(st_source,st)==0)
                  {
                   founded=TRUE;
                   strcpy(prefiks,"");
                   strcpy(sufiks,"");
                  } 
                  break;         
           default:  // A
                  if (strcmp(st_source,st)==0)
                  {
                   founded=TRUE;
                   strcpy(prefiks,"");
                   strcpy(sufiks,"");
                  } 
                  break; 
           }
           if (founded==TRUE)
           {
            strcpy(st_dest1,st_dest_symbol);
            sprintf(sufiks,"%ld",st_dest_numer_i);
            strcat(st_dest1,sufiks);
            st_dest_numer_i--;
            
            l_founded+=1; 
            del_adp=adp-dane;
            if (auto_change==FALSE)
             {
              ;
             } 
              else
               {
                 memmove (&Tp, T, sizeof(NAGLOWEK)+T->n);
                 strcpy (Tp.text, st_dest1);
                 Tp.dl = strlen(Tp.text);
                 Tp.n = T18 + Tp.dl;
				 Tp.width = 0;
				 Tp.height = 0;
                 if ( (t = korekta_obiekt((void *)adp, (void *)&Tp)) == NULL) return;
                 adp=(char *)t;
                 adk=dane+dane_size;
                 L=(LINIA*)adp;

                 l_changed+=1; 
               }
           } 
          }
        }  
       }
      adp += L->n + sizeof(NAGLOWEK) ;
    }
    L = (NAGLOWEK*)adp;
  }
  /*jezeli nie znaleziono zadnego*/
  if (l_founded==0)
   {
    Unmark_Texts(3); 
    komunikat(88);
    key = Get_Legal_Key ("TNtn\033"); 
    komunikat(0); 
   }
   else
    {
     if (l_changed>0) 
      {
       ADP=dane;
       ADK=dane + dane_size;
       Unmark_Texts(4);  
       redraw();
       komunikat(105);
       delay(1000);
       komunikat(0);
      } 
       else Unmark_Texts(3); 
    } 
  return;
}


void Redraw_Text (BOOL b_draw)
/*----------------------------*/
{
  TEXT *ptrs_tekst ;
  NAGLOWEK *nag ;
  long_long ad, off, offk ;

  okno_r () ;
  setwritemode (COPY_PUT) ;
  if (b_draw == FALSE)
  {
    off = 0 ;
    offk = dane_size - 1 ;
    ObiektTok (&off, offk, &ad, Otekst) ;
    while (ad != -1)
    {
      ptrs_tekst = (TEXT*)(dane + ad) ;
      if ((ptrs_tekst->widoczny == 1) && 
          (strlen(ptrs_tekst->text)==0) &&
          (ptrs_tekst->typ> 2 ))
      {
	rysuj_obiekt_ ((char *)ptrs_tekst, COPY_PUT, 0) ;
      }
      ObiektTok (&off, offk, &ad, Otekst);
    }
  }
  else
  {
    off = 0 ;
    offk = dane_size - 1 ;
    ObiektTok (&off, offk, &ad, Otekst) ;
    while (ad != -1)
    {
      ptrs_tekst = (T_Point*)(dane + ad) ;
      if ((1 == (ptrs_tekst->widoczny = tekst_wybrany (ptrs_tekst))) &&
                (strlen(ptrs_tekst->text)==0) &&
                (ptrs_tekst->typ>2))
      {
	rysuj_obiekt_ ((char *)ptrs_tekst, COPY_PUT, 1) ;
      }
      ObiektTok (&off, offk, &ad, Otekst);
    }
  }
  okno_all () ;
}

#undef __O_ETEXT__