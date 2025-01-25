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

extern PPOLE pmCzcionka [] ;
extern int Add_New_Font_TTF_OTF(char *name, int type, int cur);
extern void draww(TMENU *menu);
extern void baronoff(TMENU  * menu);
extern int colors1(int old_color);
extern TMENU mTTF_OTF;
extern void Restore_Pointer(void);

#define MaxMenuWinFont 32

PTMENU mCzcionkaW = { 1,0,0,FONTNAMELEN + 1,50,10,FIXED,CMNU,CMBR,CMTX,0,72,0,0,0,&pmCzcionka,NULL,NULL};
TMENU mZmienne={12,0,0,34,1,3,2|ICONS|TADD,CMNU,CMBR,CMTX,0,0,0,0,0,&pmZmienne,NULL,NULL};

static void drawwZmienne(void)
{
	if (!(mZmienne.flags & ICONS))
	{
		draww(&mZmienne);
	}
}

static void kolorL (void)
/*----------------------*/
{
  
  int i_poz, i_color ;
  char  sk[24], *sk1;

  i_poz = mKolorL.foff + mKolorL.poz ;
  i_color = Menu_No_to_Color_No (i_poz) ;
  Cur_offd(X, Y);
  if (zmwym.Lkolor == i_color) return ;
  zmwym.Lkolor = i_color ;
  L.kolor=Ls.kolor=Ls1.kolor=Ko.kolor=St.kolor=
  Lr.kolor=Lw.kolor=zmwym.Lkolor;
  Cur_ond(X, Y);
  strcpy(sk,(*mKolorL.pola)[i_poz].txt) ;
  sk1 = strpbrk (sk, " ") ;
  menu_par_new ((*mZmienne.pola)[0].txt, ++sk1) ;
  baronoff(&mZmienne);
  draww(&mZmienne);
  baronoff(&mZmienne);

  return ;
}

static void kolorLX(void)
/*-----------------------------*/
{
  int i_poz, i_color, i_color0 ;
  char  sk[24], *sk1;

  i_color0 = zmwym.Lkolor;
  i_color = colors1(i_color0);
  
  Error = FALSE;
  Info = FALSE;
  if (zmwym.Lkolor == i_color)
  {
	  baronoff(&mZmienne);
	  draww(&mZmienne);
	  baronoff(&mZmienne);

	  return_back = 1;
	  return;
  }
  zmwym.Lkolor = i_color ;
  L.kolor=Ls.kolor=Ls1.kolor=Ko.kolor=St.kolor=
  Lr.kolor=Lw.kolor=zmwym.Lkolor;
  
  if (zmwym.Lkolor>=16)
   {
     sprintf(sk,u8"֍[X]  %#ld",zmwym.Lkolor);
     sk1=strpbrk(sk," ");
   }  
   else
    {
     strcpy(sk,(*mKolorL.pola) [zmwym.Lkolor - 1].txt);
     sk1=strpbrk(sk," ");
    } 
  
  menu_par_new ((*mZmienne.pola)[0].txt, ++sk1) ;
  baronoff(&mZmienne);
  draww(&mZmienne) ;
  baronoff(&mZmienne);
  return_back = 1;  
  
  return;
}

static void kolorT (void)
/*-----------------------*/
{
	char  sk[24], *sk1;
  int i_poz, i_color ;

  i_poz = mKolorT.foff + mKolorT.poz ;
  i_color = Menu_No_to_Color_No (i_poz) ;
  Cur_offd(X, Y);
  if (zmwym.Tkolor == i_color) return ;
  zmwym.Tkolor = i_color ;
  T.kolor=zmwym.Tkolor;
  Cur_ond(X, Y);
  strcpy(sk,(*mKolorT.pola)[i_poz].txt);
  sk1=strpbrk(sk," ");
  menu_par_new((*mZmienne.pola)[1].txt,++sk1);
  baronoff(&mZmienne);
  draww(&mZmienne);
  baronoff(&mZmienne);
  return ;
}

static void kolorTX(void)
/*-----------------------------*/
{
	int i_poz, i_color, i_color0;
	char  sk[24], *sk1;


	i_color0 = zmwym.Tkolor;
	i_color = colors1(i_color0);

	Error = FALSE;
	Info = FALSE;
	if (zmwym.Tkolor == i_color)
	{
	    baronoff(&mZmienne);
		draww(&mZmienne);
		baronoff(&mZmienne);

		return_back = 1;
		return;
    }
  zmwym.Tkolor = i_color ;
  T.kolor=zmwym.Tkolor;
  
  if (zmwym.Tkolor>=16)
   {
     sprintf(sk,u8"֍[X] %#ld",zmwym.Tkolor);
     sk1=strpbrk(sk," ");
   }  
   else
    {
     strcpy(sk,(*mKolorT.pola) [zmwym.Tkolor - 1].txt);
     sk1=strpbrk(sk," ");
    } 
 
  menu_par_new((*mZmienne.pola)[1].txt,++sk1);
  baronoff(&mZmienne);
  draww(&mZmienne);
  baronoff(&mZmienne);
  return_back = 1; 
  
  return;
}


static void wysokosc_t (void)
/*-------------------------*/
{
  char sk[MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;

  DF_to_String (sk, "%-6.2f", jednostki_d_to_plt_d (zmwym.wysokosc), 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 34)) return ;
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
  zmwym.wysokosc = T.wysokosc = plt_d_to_jednostki_d (d) ;
  DF_to_String (sk, "%-6.2f", jednostki_d_to_plt_d (zmwym.wysokosc), 6) ;
  strcat(sk,"mm");
  menu_par_new((*mZmienne.pola)[2].txt,sk);
  drawwZmienne();
  Restore_Pointer();
}


static void width_factor (void)
/*----------------------------*/
{
  char sk[MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;

  DF_to_String (sk, "%-6.4f", zmwym.width_factor, 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 33)) return ;
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
  zmwym.width_factor= T.width_factor= d ;
  DF_to_String (sk, "%-6.4f", zmwym.width_factor, 6) ;
  menu_par_new((*mZmienne.pola)[3].txt,sk);
  drawwZmienne();
  Restore_Pointer();
  
}

static void italics_on (void)
/*--------------------------*/
{
  if (zmwym.italics == 1) return ;
  zmwym.italics = T.italics = 1 ;
  menu_par_new ((*mZmienne.pola)[4].txt, SZ_YES) ;
  drawwZmienne();
  
}

static void italics_off (void)
/*--------------------------*/
{
  if (zmwym.italics == 0) return ;
  zmwym.italics = T.italics = 0 ;
  menu_par_new ((*mZmienne.pola)[4].txt, SZ_NO) ;
  drawwZmienne();
}

static void bold_on (void)
/*--------------------------*/
{
  if (zmwym.bold == 1) return ;
  zmwym.bold = T.bold = 1 ;
  menu_par_new ((*mZmienne.pola)[5].txt, SZ_YES) ;
  drawwZmienne();
}

static void bold_off (void)
/*--------------------------*/
{
  if (zmwym.bold == 0) return ;
  zmwym.bold = T.bold = 0 ;
  menu_par_new ((*mZmienne.pola)[5].txt, SZ_NO) ;
  drawwZmienne();
}


static void set_font (void)
/*-----------------------*/
{
  char  *sk;
  int i_font_no, i_poz1 ;

  i_font_no = mCzcionkaW.poz + mCzcionkaW.foff ;
  i_poz1 = mTTF_OTF.poz + mTTF_OTF.foff;
  if (i_font_no >= WhNumberTextStyle)
  {
	  i_font_no = Add_New_Font_TTF_OTF(NULL, i_poz1, 1);
	  if (i_font_no==-1) return;
  }
  if (zmwym.czcionka == i_font_no) return;
  if (pmCzcionka [i_font_no].txt [0] == '\0')
  {
    return ;
  }
  zmwym.czcionka = T.czcionka = i_font_no ;
  sk=(*mCzcionkaW.pola)[zmwym.czcionka].txt;
  menu_par_new ((*mZmienne.pola)[6].txt, sk) ;
  drawwZmienne();
}


static void dokladnosc (void)
/*-------------------------*/
{
  char sk [MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;

  DF_to_String (sk, "%.12f", zmwym.dokladnosc, 0) ;  //"%-10.6f"
  if (!get_string (sk, "", MaxTextLen, 0, 35)) return ;
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
  zmwym.dokladnosc = d ;
  DF_to_String (sk, "%.12f", zmwym.dokladnosc, 16) ;
  menu_par_new((*mZmienne.pola)[8].txt,sk);
  drawwZmienne();
  Restore_Pointer();
}


static void linia_ob (void)
/*-----------------------*/
{
  char sk [MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;

  DF_to_String (sk, "%-10.6f", -zmwym.linia_ob, 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 36)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if (TRUE == Check_if_Equal (d, 0))
  {
    ErrList (79);
    return ;
  }
  DF_to_String (sk, "%-10.6f", d, 10) ;
  zmwym.linia_ob = -d ;
  menu_par_new((*mZmienne.pola)[10].txt,sk);
  drawwZmienne();
  Restore_Pointer();
}


static void  kierunek_t (void)
/*-----------------------------*/
{
  char  *sk;
  static char *P[] = {u8"_A_",u8"-A-", u8"\\A" };

  switch (mKierunekT.poz)
  {
      case 0:
          zmwym.kierunek_t=0;
          zmwym.collinear=0;
          sk=P[0];
          break;
      case 1:
          zmwym.kierunek_t=0;
          zmwym.collinear=1;
          sk=P[1];
          break;
      case 2:
          zmwym.kierunek_t=1;
          zmwym.collinear=1;
          sk=P[2];
          break;
  }
  menu_par_new((*mZmienne.pola)[11].txt, sk);
  drawwZmienne();
  Restore_Pointer();
}

static void  add_line (void)
/*---------------------------*/
{
  char  *sk ;
  static char *E[] = {u8"A", "F" };

  zmwym.b_add_line_const = TRUE ;
  if (mAddLine.poz == 1)
  {
    zmwym.b_add_line_const = FALSE ;
  }

  menu_par_new((*mZmienne.pola)[12].txt, E[zmwym.b_add_line_const]);

  drawwZmienne() ;
}

static void LFormat(void)
{   char sk [MaxTextLen]="" ;
    zmwym.format=mLFormat.poz;
    menu_par_new((*mZmienne.pola)[7].txt, _LFORMAT_[zmwym.format]);
    if ((zmwym.format>1) && ((Jednostki==25.4) || (Jednostki==304.8)))  //Architectural or Fractional  inches or feet
    {
        pmZmienne[8].menu=&mFractions;
        sprintf(sk, "1/%d", zmwym_fraction) ;
        menu_par_new((*mZmienne.pola)[8].txt, sk);
    }
    else
    {
        pmZmienne[8].menu=NULL;
        DF_to_String (sk, "%-10.6f", zmwym.dokladnosc, 10) ;
        menu_par_new((*mZmienne.pola)[8].txt,sk);
    }
    drawwZmienne();
}

static void Fractions(void)
{
    char sk [MaxTextLen]="" ;

    zmwym_fraction=pow(2, mFractions.poz);

    if ((zmwym.format>1) && ((Jednostki==25.4) || (Jednostki==304.8)))  //Architectural or Fractional  inches or feet
    {
        sprintf(sk, "1/%d", zmwym_fraction);
        menu_par_new((*mZmienne.pola)[8].txt, sk);
    }
}

static void  strzalka(void)
{ char  *sk,*sk1;
static char *S[] = {u8"<--->|","-/---/-","-*---*-" };
  if (zmwym.strzalka==mStrzalka.poz) return;
  zmwym.strzalka=mStrzalka.poz;
  menu_par_new((*mZmienne.pola)[9].txt, S[zmwym.strzalka]);
  drawwZmienne();
}

static void near uaktualnij_polaw (void)
/*------------------------------------*/
{
  int i_menu_color ;
  char sk [MaxTextLen], *sk1, *ptrsz_temp ;
  static char *S[]={u8"<--->|","-/---/-","-*---*-"};
  static char *P[] = {u8"_A_","-A-", "\\A" };
  static char *E[] = {u8"A", "F"};

  i_menu_color = Color_No_to_Menu_No (zmwym.Lkolor) ;
  if (i_menu_color>=16) 
   {
    i_menu_color=16;
   }
  Get_Menu_Param (i_menu_color, MAXWCOLOR, MAXCOLOR,
		 &mKolorL.foff, &mKolorL.poz) ;
		 
  if (i_menu_color>=16)
   {
	 strcpy(&sk,"");
     sprintf(sk,u8"֍[X] %#ld",zmwym.Lkolor);
     sk1=strpbrk(sk," ");
   }  
   else
    {
	 return_menu_par0((*mKolorL.pola)[i_menu_color].txt,&sk);
	 sk1=strpbrk(sk," ");
    }
    
  menu_par_new((*mZmienne.pola)[0].txt,++sk1);

  i_menu_color = Color_No_to_Menu_No (zmwym.Tkolor) ;
  if (i_menu_color>=16) 
   {
    i_menu_color=16;
   }
  Get_Menu_Param (i_menu_color, MAXWCOLOR, MAXCOLOR,
		 &mKolorT.foff, &mKolorT.poz) ;
  if (i_menu_color>=16)
   {
	 strcpy(&sk,"");
     sprintf(sk,u8"֍[X] %#ld",zmwym.Tkolor);
     sk1=strpbrk(sk," ");
   }  
   else
    {
	 return_menu_par0((*mKolorT.pola)[i_menu_color].txt,&sk);
	 sk1=strpbrk(sk," ");
    } 
    
  menu_par_new((*mZmienne.pola)[1].txt,++sk1);

  DF_to_String (sk, "%-6.2f", jednostki_d_to_plt_d (zmwym.wysokosc), 6) ;
  menu_par_new((*mZmienne.pola)[2].txt,sk);

  DF_to_String (sk, "%-6.4f", zmwym.width_factor, 6) ;
  menu_par_new ((*mZmienne.pola)[3].txt, sk) ;

  menu_par_new ((*mZmienne.pola)[4].txt,
    ((zmwym.italics == 1) ? SZ_YES : SZ_NO)) ;
    
  menu_par_new ((*mZmienne.pola)[5].txt,
    ((zmwym.bold == 1) ? SZ_YES : SZ_NO)) ;

  mCzcionkaW.max = WhNumberTextStyle + 1;
  if (zmwym.czcionka >= WhNumberTextStyle)
  {
    zmwym.czcionka = 0 ;
  }
  ptrsz_temp=(*mCzcionkaW.pola)[zmwym.czcionka].txt;
  menu_par_new ((*mZmienne.pola)[6].txt, ptrsz_temp) ;

  menu_par_new((*mZmienne.pola)[7].txt, _LFORMAT_[zmwym.format]);

  if ((zmwym.format>1) && ((Jednostki==25.4) || (Jednostki==304.8)))  //Architectural or Fractional  inches or feet
  {
      pmZmienne[8].menu=&mFractions;
      sprintf (sk, "1/%d", zmwym_fraction) ;
      menu_par_new((*mZmienne.pola)[8].txt, sk);
  }
   else
  {
      pmZmienne[8].menu=NULL;
      DF_to_String (sk, "%.12f", zmwym.dokladnosc, 10) ;
      menu_par_new((*mZmienne.pola)[8].txt,sk);
  }

  menu_par_new((*mZmienne.pola)[9].txt,S[zmwym.strzalka]) ;

  DF_to_String (sk, "%-10.6f", -zmwym.linia_ob, 10) ;
  menu_par_new ((*mZmienne.pola)[10].txt,sk) ;


  if (zmwym.kierunek_t==0)
  {
      if (zmwym.collinear==0) sk1=P[0];
      else sk1=P[1];
  } else sk1=P[2];
  menu_par_new ((*mZmienne.pola)[11].txt,sk1) ;

  mAddLine.poz = 0 ;
  if (zmwym.b_add_line_const == FALSE)
  {
    mAddLine.poz = 1 ;
  }
  ptrsz_temp = (*mAddLine.pola) [mAddLine.poz].txt ;
  menu_par_new((*mZmienne.pola)[12].txt, E[zmwym.b_add_line_const]);
}

static void  (*COMNDz[])(void)={
         nooop,nooop,wysokosc_t, width_factor,
         nooop, nooop,nooop,nooop, dokladnosc,nooop, linia_ob, nooop, nooop,
         kolorL,kolorL,kolorL,kolorL,kolorL,kolorL,kolorL,kolorL,
         kolorL,kolorL,kolorL,kolorL,kolorL,kolorL,kolorL,kolorL,kolorLX,
         kolorT,kolorT,kolorT,kolorT,kolorT,kolorT,kolorT,kolorT,
         kolorT,kolorT,kolorT,kolorT,kolorT,kolorT,kolorT,kolorT,kolorTX,
         italics_on, italics_off,
         bold_on, bold_off,
         LFormat, LFormat, LFormat, LFormat,
         Fractions,Fractions,Fractions,Fractions,Fractions,Fractions,Fractions,Fractions,Fractions,
         strzalka,strzalka,strzalka,
         kierunek_t,kierunek_t,kierunek_t,
         add_line, add_line,
         set_font,set_font,set_font,set_font,set_font,
         set_font,set_font,set_font,set_font,set_font,
         set_font,set_font,set_font,set_font,set_font,
         set_font,

} ;

static void  zmienne(void)
{ int n;
  komunikat(0);
  uaktualnij_polaw();
 while(1)
   if((n=getwsp1(&mZmienne)-1)==-1) 
    {
      Error = FALSE;
      Info = FALSE;
      return;
    }  
   else 
    {
	  if (n>72) (*COMNDz[72])();
	  else (*COMNDz[n])();
    }  
}
