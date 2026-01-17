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

extern void najblizszyl_(double *x, double *y, void *adr);
extern void srodekl_(double *x, double *y, void *adr);


static void luk_w (void  *adr)
/*----------------------------*/
{ LUK *luk;
  double x1,y1,x2,y2;
  LINIA l;
  PLINIA PL;
#ifndef LINUX
  BLOKD B=BDdef;
#else
  BLOK B=Bdef;
#endif
  BLOK *nb;
  float kat1;
  WymNowy();
  luk = (LUK*)adr;
  najblizszyl_(&x2, &y2, adr);
  x1 = luk->x ;
  y1 = luk->y;
  l.x1=x1;
  l.y1=y1;
  l.x2=x2;
  l.y2=y2;
  parametry_lini(&l,&PL);
  katkat(PL.kat);
  w.x0 = x1;
  w.y0 = y1;
  w.x2 = x2;
  w.y2 = y2;
  B.n=B3+sizeof(kat1);
  B.kod_obiektu=0x01;
  B.dlugosc_opisu_obiektu=sizeof(kat1);
  typ_wymiar = Oluk;
  kat_w_now = 0;
  Continue=1;
  if( (nb=(BLOK*)dodaj_obiekt(NULL,&B))==NULL) return ;
  kat1=kat.kat;
  memmove(&(nb->opis_obiektu[0]),&kat1,sizeof(kat1));
  outs(TRUE);
  CUR_OFF(X,Y);
  CUR_ON(X,Y);
}

void *luk_w_in_block (void  *adr, BOOL draw)
{   BLOK *b,*B1;
    size_t b_size;
    BLOK *nb1;
    char *ADP1, *ADK1;

    LUK *luk;
    double x1,y1,x2,y2;
    LINIA l;
    PLINIA PL;
#ifndef LINUX
    BLOKD B=BDdef;
#else
    BLOK B=Bdef;
#endif
    BLOK *nb;
    float kat1;

    //creating outside block
    //luk_w (adr);
    WymNowy();
    luk = (LUK*)adr;
    //najblizszyl_(&x2, &y2, adr);
    srodekl_(&x2,&y2, adr);

    x1 = luk->x ;
    y1 = luk->y;
    l.x1=x1;
    l.y1=y1;
    l.x2=x2;
    l.y2=y2;
    parametry_lini(&l,&PL);
    katkat(PL.kat);
    w.x0 = x1;
    w.y0 = y1;
    w.x2 = x2;
    w.y2 = y2;
    B.n=B3+sizeof(kat1);
    B.kod_obiektu=0x01;
    B.dlugosc_opisu_obiektu=sizeof(kat1);
    typ_wymiar = Oluk;
    kat_w_now = 0;
    Continue=1;
    if( (nb=(BLOK*)dodaj_obiekt(NULL,&B))==NULL) return NULL;
    kat1=kat.kat;
    memmove(&(nb->opis_obiektu[0]),&kat1,sizeof(kat1));
    outs(draw);

    //creating buffer
    b=(BLOK*)dane;
    if (b->kod_obiektu!=0x01) return NULL;  //it's not dim block
    b_size=b->n+sizeof(NAGLOWEK);
    B1=malloc(b_size);
    if (B1==NULL) return NULL;
    //buffering block
    memmove(B1, b, b_size);
    //deleting block from data
    ADP1=(char*)b;
    ADK1=(char*)b+b_size-1;
    zmien_atrybut (ADP1, ADK1,	ANieOkreslony, Ausuniety)	;
    usun_blok(ADP1,ADK1);
    //inserting block into stirrup block
    B1->blok=Ablok;
    nb1=(BLOK*)dodaj_obiekt(dane,B1);
    //freeing buffer
    free(B1);
    return nb1;
}

static void luk_wdl (void  *adr)
/*----------------------------*/
{ LUK *luk;
  double x1,y1,x2,y2;
  LINIA l;
  LUK luk1;
  PLINIA PL;
#ifndef LINUX
    BLOKD B=BDdef;
#else
    BLOK B=Bdef;
#endif
  BLOK *nb;
  float kat3;
  WymNowy();
  luk = (LUK*)adr;

  srodekl_(&x2, &y2, adr);

  x1 = luk->x ;
  y1 = luk->y;
  l.x1=x1;
  l.y1=y1;
  l.x2=x2;
  l.y2=y2;

  luk1.x = luk->x;
  luk1.y = luk->y;
  luk1.kat1 = luk->kat1;
  luk1.kat2 = luk->kat2;
  luk1.r = luk->r + zmwym.linia_ob;

  parametry_lini(&l,&PL);
  katkat(PL.kat);
  wl.x = luk1.x;
  wl.y = luk1.y;
  wl.kat1 = luk1.kat1;
  wl.kat2 = luk1.kat2;
  wl.r = luk1.r;

  B.n=B3+sizeof(kat3);
  B.kod_obiektu=0x01;
  B.dlugosc_opisu_obiektu=sizeof(kat3);
  typ_wymiar = Oluk;
  kat_w_now = 0;
  Continue=1;

  if( (nb=(BLOK*)dodaj_obiekt(NULL,&B))==NULL) return ;
  kat3=kat.kat;
  memmove(&(nb->opis_obiektu[0]),&kat3,sizeof(kat3));
  outss();
  CUR_OFF(X,Y);
  CUR_ON(X,Y);
}

static void kat_w (LINIA line_gee, LINIA line_gee1)
/*-----------------------------------------------*/
{ LUK *luk;
  double x1,y1,x2,y2;
  LINIA l;
  LUK luk1;
  PLINIA PL;
#ifndef LINUX
    BLOKD B=BDdef;
#else
    BLOK B=Bdef;
#endif
  BLOK *nb;
  float kat3;
  WymNowy();

  luk1.x = line_gee.x2;
  luk1.y = line_gee.y2;
  luk1.kat1 = Atan2 (line_gee.y1 - line_gee.y2, line_gee.x1 - line_gee.x2) ;

  luk1.kat2 = Atan2 (line_gee1.y2 - line_gee1.y1, line_gee1.x2 - line_gee1.x1) ;

  luk1.r = min(sqrt(((line_gee1.x2-line_gee1.x1) * (line_gee1.x2-line_gee1.x1)) +
                        ((line_gee1.y2-line_gee1.y1) * (line_gee1.y2-line_gee1.y1))),
                        sqrt(((line_gee.x2-line_gee.x1) * (line_gee.x2-line_gee.x1)) +
                        ((line_gee.y2-line_gee.y1) * (line_gee.y2-line_gee.y1))));

  parametry_lini(&l,&PL);
  katkat(PL.kat);
  wl.x = luk1.x;
  wl.y = luk1.y;
  wl.kat1 = luk1.kat1;
  wl.kat2 = luk1.kat2;
  wl.r = luk1.r;

  B.n=B3+sizeof(kat3);
  B.kod_obiektu=0x01;
  B.dlugosc_opisu_obiektu=sizeof(kat3);
  typ_wymiar = Oluk;
  kat_w_now = 1;
  Continue=1;

  if( (nb=(BLOK*)dodaj_obiekt(NULL,&B))==NULL) return ;
  kat3=kat.kat;
  memmove(&(nb->opis_obiektu[0]),&kat3,sizeof(kat3));
  outss();
  kat_w_now = 0;
}


void kat_w_clock (double kat1, double kat2, double x, double y, double r)
/*---------------------------------------------------------------------*/
{ LUK *luk;
  double x1,y1,x2,y2;
  LINIA l;
  LUK luk1;
  PLINIA PL;
#ifndef LINUX
    BLOKD B=BDdef;
#else
    BLOK B=Bdef;
#endif
  BLOK *nb;
  float kat3;
  WymNowy();

  luk1.x=x;
  luk1.y=y;
  luk1.kat1=kat1;
  luk1.kat2=kat2;

  luk1.r = r;

  parametry_lini(&l,&PL);
  katkat(PL.kat);
  wl.x = luk1.x;
  wl.y = luk1.y;
  wl.kat1 = luk1.kat1;
  wl.kat2 = luk1.kat2;
  wl.r = luk1.r;

  B.n=B3+sizeof(kat3);
  B.kod_obiektu=0x01;
  B.dlugosc_opisu_obiektu=sizeof(kat3);
  typ_wymiar = Oluk;
  kat_w_now = 1;
  Continue=1;

  if( (nb=(BLOK*)dodaj_obiekt(NULL,&B))==NULL) return ;
  kat3=kat.kat;
  memmove(&(nb->opis_obiektu[0]),&kat3,sizeof(kat3));
  outss_clock();
  kat_w_now = 0;
  CUR_OFF(X,Y);
  CUR_ON(X,Y);
}
