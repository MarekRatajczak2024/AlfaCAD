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

static void okrag_w (void  *adr)
/*-------------------------------*/
{ OKRAG *o;
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
  double df_angle ;

  WymNowy();
  o = (OKRAG*)adr;
  if (wym_okregu==0)
  {
  if (kierunek == Rownolegly)
  {
    najblizszyO_(&x1, &y1, adr);
  }
  else
  {
    df_angle = kat.kat * Pi / 180 ;
    x1 = o->x + o->r * cos (df_angle) ;
    y1 = o->y + o->r * sin (df_angle) ;
  }
  x2 = 2 * o->x - x1;
  y2 = 2 * o->y - y1;
  }
  else
  {
  najblizszyO_(&x2, &y2, adr);
  x1 = o->x ;
  y1 = o->y;
  }

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
  if (wym_okregu==0)  B.kod_obiektu=B_DIM2;
   else B.kod_obiektu=B_DIM1;
  B.dlugosc_opisu_obiektu=sizeof(kat1);

  typ_wymiar = Ookrag;
  Continue=1;
  if( (nb=(BLOK*)dodaj_obiekt(NULL,&B))==NULL) return ;
  kat1=kat.kat;
  memmove(&(nb->opis_obiektu[0]),&kat1,sizeof(kat1));
  outs(TRUE);
  CUR_OFF(X,Y);
  CUR_ON(X,Y);
}
