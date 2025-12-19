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

static void koniecL(void *adr) ;
static double get_add_line_len (double x, double y) ;


static void koniecW (void * adr)
/*-----------------------------*/
{
  LINIA L = Ldef ;
  WIELOKAT *w ;
  double x, y ;
  double x1, x2, y1, y2 ;

  int i;
  w = adr ;
  x1 = w->xy [w->lp - 2] ;
  y1 = w->xy [w->lp - 1] ;
  for (i = 0 ; i < w->lp ; i += 2)
  {
    x2 = w->xy [i + 0] ;
    y2 = w->xy [i + 1] ;
    L.x1 = x1 ;
    L.y1 = y1 ;
    L.x2 = x2 ;
    L.y2 = y2 ;
    if (0 != linia_wybrana (&L))
    {
      x = X ;
      y = Y ;
      koniecL_ (&x, &y, &L) ;
      if (0 != punkt_w_prostokacie (x, y))
      {
	WymPunkt (x, y, 1) ;
      }
      else
      {
	koniecL (&L) ;
      }
      return ;
    }
    x1 = x2 ;
    y1 = y2 ;
  }
}


static double Xkat,Ykat;
/*static*/ int near punkt0(double x, double y)
{
  BLOK B=Bdef;
  BLOK *nb;
  float kat1;
  int n1,n2;

  if(Continue==0)
  {
     Continue++;
     new_dim_info (FALSE) ;
     B.n=B3+sizeof(kat1);
     B.kod_obiektu='W';
     B.dlugosc_opisu_obiektu=sizeof(kat1);
     if( (nb=(BLOK*)dodaj_obiekt(NULL,&B))==NULL) return 0;
     kat1=kat.kat;
     memmove(&(nb->opis_obiektu[0]),&kat1,sizeof(kat1));
     n1=kat.kat>90 ?  1 : -1;
     n2=kat.kat>90 ?  1 :  1;
     w.x0=w.x2=x+n1*zmwym.linia_ob*fabs(kat.sin);
     w.y0=w.y2=y+n2*zmwym.linia_ob*fabs(kat.cos);
   }
  else
   { Continue++;
     w.x0=w.x2;
     w.y0=w.y2;
     if (kat.kat==0)
      { w.x2=x;
	w.y2=w.y0;
      }
     else
      if (kat.kat==90)
      { w.x2=w.x0;
	w.y2=y;
      }
      else
       { double a;
	 a=kat.sin/kat.cos;
	 w.x2=(y-w.y0+(1/a)*x+a*w.x0)/(a+1/a);
	 w.y2=w.y0+a*(w.x2-w.x0);
       }
     if( !outs(TRUE) ) return 0;
   }
  n1=x>w.x2 ?  1 : -1;
  n2=y>w.y2 ? -1 :  1;
  Lr.warstwa=Current_Layer;
  Lr.x1=w.x2+n1 * get_add_line_len (x, y) * fabs(kat.sin);
  Lr.y1=w.y2-n2 * get_add_line_len (x, y) * fabs(kat.cos);
  Lr.x2=w.x2-n1*K1_5*fabs(kat.sin);
  Lr.y2=w.y2+n2*K1_5*fabs(kat.cos);
  if(Lr.x1!=Lr.x2 || Lr.y1!=Lr.y2)
   if(dodaj_obiekt((BLOK *)dane, (void*)&Lr)==NULL) return 0;
   else if(WymInter) {
       rysuj_obiekt(&Lr, COPY_PUT, 1);
       CUR_OFF(x,y);
   }
  return 1;
}

static double get_add_line_len (double x, double y)
/*------------------------------------------------*/
{
  double df_ret ;

  df_ret = K5 ;
  if (zmwym.b_add_line_const == FALSE)
  {
    df_ret = Dist_PP (w.x2, w.y2, x, y) ;
  }
  return df_ret ;
}


static void  koniecL(void *adr)
{ PLINIA PL;
  koniecL_(&X,&Y,adr);
  if (kierunek==NoRownolegly) punkt0(X,Y);
  else
   switch(katkier)
    { case NoNoZnany :
		       parametry_lini((LINIA*)adr,&PL);
		       katkat(PL.kat);
		       katkier=Znany;
		       punkt0(X,Y);
		       break;
	  case NoZnany   :
		     { LINIA l;
		       l.x1=Xkat;
		       l.y1=Ykat;
		       l.x2=X;
		       l.y2=Y;
		       parametry_lini(&l,&PL);
		       katkat(PL.kat);
		       katkier=Znany;
		       punkt0(Xkat,Ykat);
		       punkt0(X,Y);
		       break;
		     }
      case Znany     : punkt0(X,Y);
		       break;
    }
}

extern int przeciecieLl_(double *x, double *y, void *adr, void *adr1) ;
extern int przecieciell_(double *x, double *y, void *adr, void *adr1) ;
extern int przeciecielO_(double *x, double *y, void *adr, void *adr1) ;
extern int przeciecieLW_(double *x, double *y, void *adr, void *adr1) ;
extern int przeciecielW_(double *x, double *y, void *adr, void *adr1) ;
extern int przeciecieWO_(double *x, double *y, void *adr, void *adr1) ;
extern int przeciecieWW_(double *x, double *y, void *adr, void *adr1) ;

static void  przeciecieLOlw (unsigned typ ,void *adr, void *adr1)
{ PLINIA PL;

  new_dim_info (FALSE) ;
  switch(typ)
   { case Blinia | Blinia : przeciecieLL_(&X,&Y,adr,adr1);
			     break;
     case Blinia | Bokrag : przeciecieLO_(&X,&Y,adr,adr1);
			     break;
     case Bokrag | Bokrag : przeciecieOO_(&X,&Y,adr,adr1);
			     break;

     case Blinia | Bluk   : przeciecieLl_(&X,&Y,adr,adr1);
			     break;
     case Bluk | Bokrag   : przeciecielO_(&X,&Y,adr,adr1);
			     break;
     case Bluk | Bluk     : przecieciell_(&X,&Y,adr,adr1);
			     break;

     case Blinia | Bwwielokat: przeciecieLW_(&X, &Y, adr,adr1) ;
			     break;
     case Bluk   | Bwwielokat: przeciecielW_(&X, &Y, adr, adr1) ;
			     break;
     case Bwwielokat | Bokrag : przeciecieWO_(&X, &Y, adr, adr1) ;
			     break;
     case Bwwielokat | Bwwielokat: przeciecieWW_(&X, &Y, adr, adr1) ;
			     break;
      default              : break;
   }
  if (kierunek==NoRownolegly) punkt0(X,Y);
  else
   switch(katkier)
    { case NoNoZnany :
		       Xkat=X;
		       Ykat=Y;
		       katkier=NoZnany;
		       break;
      case NoZnany   :
		     { LINIA l;
		       l.x1=Xkat;
		       l.y1=Ykat;
		       l.x2=X;
		       l.y2=Y;
		       parametry_lini(&l,&PL);
		       katkat(PL.kat);
		       katkier=Znany;
		       punkt0(Xkat,Ykat);
		       punkt0(X,Y);
		       break;
		     }
      case Znany     : punkt0(X,Y);
		       break;
    }
}

int WymPunkt(double X, double Y, int tryb)
{
  PLINIA PL;
  int ret=1;
  unsigned typ;
  void *adr,*adr1;

  if (tryb==1)
   {
    if (!sel.wyj && !sel.gor) {
        typ = Blinia | Bluk | Bokrag | Bkolo | Bwymiarowanie | Btekst | Bwwielokat;
        if ((adr = select_w(&typ, &adr1)) == NULL) {
            return 0;   //brak obiektu
        }
        if (adr1 == NULL) {
            if ((((NAGLOWEK *) adr)->obiektt2 == O2BlockDim) && ((sel.akt == 0) || (sel.gor == 0))) {
                if (((NAGLOWEK *) adr)->blok == 1) {
                    Edit_Wym(typ, adr);
                    return 1;   //edycja obiektu
                }
            }
        }
    }
   }
  //****************
  if (kierunek==NoRownolegly) punkt0(X,Y);
  else
   switch(katkier)
	{ case NoNoZnany :
		       Xkat=X;
		       Ykat=Y;
		       katkier=NoZnany;
		       break;
      case NoZnany   :
		     { LINIA l;
		       l.x1=Xkat;
		       l.y1=Ykat;
		       l.x2=X;
		       l.y2=Y;
		       parametry_lini(&l,&PL);
		       katkat(PL.kat);
		       katkier=Znany;
		       ret=punkt0(Xkat,Ykat);
		       ret=punkt0(X,Y);
		       break;
		     }
	  case Znany     : ret=punkt0(X,Y);
			   break;
	}
 return ret;
}

static void punktsel(double X0, double Y0)
{
  X = X0 ;
  Y = Y0 ;
  WymPunkt ( X ,Y , 1 ) ;
}

#include "bibwymo.c"
#include "bibwyml.c"

static void  punkt(void)
{ unsigned typ;
  LINIA line_gee ;
  LINIA line_gee1 ;
  int out_kat ;

  void *adr,*adr1;
  if (wym_kata==3)
      typ=Bluk | Bokrag | Bkolo | Bwymiarowanie | Btekst;
  else typ=Blinia | Bluk | Bokrag | Bkolo | Bwymiarowanie | Btekst | Bwwielokat ;
  if ((adr=select_w(&typ,&adr1))==NULL)
  {
    return;
  }

  if (adr1==NULL)
  {
    if (((NAGLOWEK*)adr)->obiektt2 == O2BlockDim)
    {
     if (((NAGLOWEK*)adr)->blok == 1)
     {
      Edit_Wym (typ, adr) ;
      return ;
     } 
    }
    else if (wym_kata!=2)
    {
        if ((typ == Blinia) && (wym_kata == 1))
        {
            WymNowy();
            out_kat == 0;
            Angle_Measure(&line_gee, &line_gee1, &out_kat, 1);

            if (out_kat == 1)
            {
                kat_w(line_gee, line_gee1);
                CUR_OFF_ON();
            }
        }
        else if (typ == Blinia) koniecL(adr);
        else if (typ == Bluk) {
            if (wym_luku == 0) {
                luk_w(adr);
            } else {
                luk_wdl(adr);
            }
        } else if (typ == Bokrag) okrag_w(adr);
        else if (typ == Bwwielokat) koniecW(adr);
    }
  }
  else
  {
    przeciecieLOlw (typ, adr, adr1) ; /*L O l W*/
  }
}
