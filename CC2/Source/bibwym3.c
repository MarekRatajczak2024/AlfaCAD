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

double round_to_fraction(double dimbase, double fraction)
{
    double half_fraction;
    double dimbase_plus;
    double result;

    result = copysign(1.0, dimbase);

    half_fraction = fraction / 2.0;
    dimbase_plus = fabs(dimbase) + half_fraction;

    dimbase_plus /= fraction;
    dimbase_plus = (long long)dimbase_plus;
    dimbase_plus *= fraction;

    dimbase_plus *= result;

    return dimbase_plus;
}

static void get_decimal_format(double l)
{
    int len ;
    int lx,z;

    l = round_to_fraction(l, zmwym.dokladnosc);
    if (force_dim_text == FALSE) {
        sprintf(T.text, "%.12f", l);
        lx = strlen(T.text);
    } else {
        strcpy(T.text, Dim_Text_Adr);
        lx = strlen(T.text);
    }

    if (NULL != strchr(T.text, '.')) {
        len = strlen(T.text);
        while (T.text[len - 1] == '0') {
            T.text[--len] = '\0';
        }
        if (T.text[len - 1] == '.') {
            T.text[len - 1] = '\0';
        }
    }
}

void get_fractional_format(double l)
{    int inchorfeet;
     double fraction;
     int ifraction, denominator;
     //assumed all is in inches  e.g. 14.4311
     inchorfeet=(int)l;  //14
     fraction=(l-inchorfeet)*zmwym_fraction;  //0.4311*64 = 27.5904  del=0.4311 - 28/64 = −0.0064
                                          //0.4311*32 = 13.7952  del=0.4311 - 14/32 = −0.0064
                                          //0.4311*16 = 6.8976  del=0.4311  - 7/16  = −0.0064
     ifraction=(int)(fraction+0.5);  //28
     denominator=zmwym_fraction;

     while (ifraction % 2 ==0)
     {
         ifraction=ifraction/2;
         denominator=denominator/2;
     }
    sprintf(T.text, "%d %d/%d", inchorfeet, ifraction, denominator);
}


void get_engineering_format(double l)
{   int feet;
    double dinches;
    int inches;
    double fraction;
    int ifraction, denominator;
    char tinches[32];
    int len ;

    if (Jednostki==304.8)   //all in feet
    {
        feet = (int) l;
        dinches = (l - feet) * 12;
    }
    else //all in inches
    {
        feet = (int)(l/12.0);
        dinches = (l - feet*12);
    }

    dinches = round_to_fraction(dinches, zmwym.dokladnosc);
    sprintf(tinches, "%.12f", dinches);
    len = strlen(tinches);
    while (tinches[len - 1] == '0') {
        tinches[--len] = '\0';
    }
    if (tinches[len - 1] == '.') {
        tinches[len - 1] = '\0';
    }

      sprintf(T.text, "%d'-%s\"", feet, tinches);
}

void get_architectural_format(double l)
{   int feet;
    double dinches;
    int inches;
    double fraction;
    int ifraction, denominator;

    if (Jednostki==304.8)   //all in feet
    {
        feet = (int) l;
        dinches = (l - feet) * 12;
    }
    else //all in inches
    {
        feet = (int)(l/12.0);
        dinches = (l - feet*12);
    }

    inches = (int) dinches;

    fraction=(dinches-inches)*zmwym_fraction;  //0.4311*64 = 27.5904  del=0.4311 - 28/64 = −0.0064
    ifraction=(int)(fraction+0.5);  //28
    denominator=zmwym_fraction;
    while (ifraction % 2 ==0)
    {
        ifraction=ifraction/2;
        denominator=denominator/2;
    }
    sprintf(T.text, "%d'-%d %d/%d\"", feet, inches, ifraction, denominator);
}

void get_dim_round_text(double l, char *text)
{
    switch (zmwym.format) {
        case 0: //Decimal: all in decimal
            get_decimal_format(l);
            break;
        case 1:  //Engineering:  feet-inches with decimals:   1'-2.4311"
            if ((Jednostki==25.4) || (Jednostki==304.8)) //inch or foot
            {
                get_engineering_format(l);
            }
            else get_decimal_format(l);  //all other units, it doesn't make sense showing mm with fractions
            break;

        case 2:  //Architectural: feet-inches with fraction:  1'-2 7/16"
            if ((Jednostki==25.4) || (Jednostki==304.8)) //inch or foot
            {
                get_architectural_format(l);
            }
            else get_decimal_format(l);  //all other units, no fractions for e.g. mm
            break;

        case 3:  //Fractional: for inches and feet fractional, for other decimal
            if (Jednostki==25.4)  //inch
            {
                get_fractional_format(l);
                strcat(T.text,u8"\"");
            }
            else if (Jednostki==304.8) //foot
            {
                get_fractional_format(l);
                strcat(T.text,u8"'");
            }
            else get_decimal_format(l);  //all other units
            break;
    }
    strcpy(text, T.text);
}

static int wyznacz_tekst(BOOL draw)
{ double l,l1;
  double x1,x2,y1,y2,x,y,wysokosc,kat2_1,sin_kat2_1,cos_kat2_1;
  int lx,z;
  int len ;
  double kat2_kat1, ex_r ;
  double pl_dx, pl_dy;
  int ret;
  double tangent;

  if (zmwym.dokladnosc == 0.0) zmwym.dokladnosc = 0.01;  //for not well converted DXF files

  T.justowanie=j_do_lewej;
  T.wysokosc=zmwym.wysokosc;
  if ((wym_luku==1)&&(typ_wymiar==Oluk)&&(kat_w_now==0))
   {
    if (wl.kat2<wl.kat1)
    {
    wl.kat1=wl.kat1-(2*Pi);
    }
    l=(wl.kat2-wl.kat1)*(wl.r-zmwym.linia_ob);
    l=milimetryob(l);
    kat2_kat1=0.5*(wl.kat2+wl.kat1);
   }
    else
     if ((wym_kata==1)&&(kat_w_now==1))
     {
      if (wl.kat2<wl.kat1)
      {
      wl.kat1=wl.kat1-(2*Pi);
      }
      if (!options1.uklad_izometryczny)
           l=((wl.kat2-wl.kat1)/(2*Pi))*360;
      else {
          l=(cartesian_angle_to_isometric_angle(wl.kat2)-cartesian_angle_to_isometric_angle(wl.kat1))/M_PI*180.;
      }
      if (l<0.) l+=360.;
      kat2_kat1=0.5*(wl.kat2+wl.kat1);
     }
      else
	{
        if (!options1.uklad_izometryczny)
        {
            l=sqrt((w.x2-w.x0)*(w.x2-w.x0)+(w.y2-w.y0)*(w.y2-w.y0));
        }
        else
        {
            l=isometric_vector_length(w.x0, w.y0, w.x2, w.y2, &pl_dx, &pl_dy);
        }
        l=milimetryob(l);
	}


  if ((wym_kata==1)&&(kat_w_now==1))
  {
      get_decimal_format(l);
      strcat(T.text,u8"°");
  }
 else {
      switch (zmwym.format) {
          case 0: //Decimal: all in decimal
              get_decimal_format(l);
              break;
          case 1:  //Engineering:  feet-inches with decimals:   1'-2.4311"
              if ((Jednostki==25.4) || (Jednostki==304.8)) //inch or foot
              {
                  get_engineering_format(l);
              }
              else get_decimal_format(l);  //all other units, it doesn't make sense showing mm with fractions
              break;

          case 2:  //Architectural: feet-inches with fraction:  1'-2 7/16"
              if ((Jednostki==25.4) || (Jednostki==304.8)) //inch or foot
              {
                  get_architectural_format(l);
              }
              else get_decimal_format(l);  //all other units, no fractions for e.g. mm
              break;

          case 3:  //Fractional: for inches and feet fractional, for other decimal
              if (Jednostki==25.4)  //inch
              {
                  get_fractional_format(l);
                  strcat(T.text,u8"\"");
              }
              else if (Jednostki==304.8) //foot
              {
                  get_fractional_format(l);
                  strcat(T.text,u8"'");
              }
              else get_decimal_format(l);  //all other units
              break;
      }
  }

  
  if (((wym_luku==1)&&(typ_wymiar==Oluk)) ||
      (kat_w_now==1))
  {
      if (zmwym.collinear == 0) ex_r=1.5;
      else ex_r=0.0;
      if (zmwym.linia_ob > 0) {
          x1 = wl.x + (cos(kat2_kat1) * (wl.r + ex_r));
          y1 = wl.y + (sin(kat2_kat1) * (wl.r + ex_r));
      } else {
          x1 = wl.x + (cos(kat2_kat1) * (wl.r + ex_r));
          y1 = wl.y + (sin(kat2_kat1) * (wl.r + ex_r));
      }
      wysokosc = zmwym.wysokosc;
      l = 0;
      T.justowanie = j_srodkowo;
      if (!options1.uklad_izometryczny) {
          if (zmwym.kierunek_t == 1) {
              T.kat = 0;
              if (kat2_kat1 <= Pi) y = y1;
              else y = y1 - wysokosc;

              if (kat2_kat1 <= (Pi * 0.5)) x = x1;
              else if (kat2_kat1 > (Pi * 1.5)) x = x1;
              else x = x1 - l;
          } else {
              T.kat = (kat2_kat1) - (Pi * 0.5);
              if (zmwym.collinear == 0) {
                  y = y1 - (sin(T.kat) * 0.5 * l);
                  x = x1 - (cos(T.kat) * 0.5 * l);
              } else {
                  y = y1 - (sin(T.kat) * 0.5 * l) - T.wysokosc * 0.5 * fabs(cos(T.kat));
                  x = x1 - (cos(T.kat) * 0.5 * l) + T.wysokosc * 0.5 * sin(T.kat);
              }
          }
      }
      else
      {
          srodekea_(&x1, &y1, &tangent, &ea);
          tangent = fmod(tangent + M_PI, 2.0 * M_PI);
          double kats = tangent + Pi_ / 2.;
          if (kats < 0) kats += 2.0 * M_PI;
          x=(float)(x1+0.5*cos(kats));
          y=(float)(y1+0.5*sin(kats));
          T.kat=(float)tangent;
      }
  }
  else {
      if ((w.x2 > w.x0) || (w.x2 == w.x0 && w.y0 > w.y2)) {
          x1 = w.x0;
          y1 = w.y0;
          x2 = w.x2;
          y2 = w.y2;
      } else {
          x1 = w.x2;
          y1 = w.y2;
          x2 = w.x0;
          y2 = w.y0;
      }
      wysokosc = zmwym.wysokosc;
      l = 0;
      T.justowanie = j_srodkowo;

      if (zmwym.kierunek_t == 1) {
          T.kat = 0;
          y = 0.5 * (y1 + y2) + K1 * kat.cos - 0.5 * wysokosc * fabs(kat.sin);
          x = 0.5 * (x1 + x2) - K1 * kat.sin;
          if (kat.sin * K1 >= 0)
              x -= 0.5 * l * (1 + fabs(kat.sin));
      } else {
          T.kat = kat.kat * Pi / 180;
          if (zmwym.collinear == 0) {
              y = y1 + 0.5 * (y2 - y1 - l * kat.sin) + K1 * fabs(kat.cos);
              x = x1 + 0.5 * (x2 - x1 - l * kat.cos) - K1 * kat.sin;
          } else {
              y = y1 + 0.5 * (y2 - y1 - l * kat.sin) - T.wysokosc * 0.5 * fabs(kat.cos);
              x = x1 + 0.5 * (x2 - x1 - l * kat.cos) + T.wysokosc * 0.5 * kat.sin;
          }
      }


  }
  T.warstwa=Current_Layer;
  T.x=x;
  T.y=y;
  T.dl=strlen(&T.text[0]);
  T.n=T18+T.dl;
  T.width = 0;
  T.height = 0;

  normalize_txt(&T);

  if (dodaj_obiekt((BLOK*)dane,(void*)&T)==NULL) return 0;
  else if(WymInter)
  {

      //breaking dim line
      break_dim_line(dane, Anormalny, 1, 1, draw);

      //outtextxy_w(&T,COPY_PUT);
      if (draw) rysuj_obiekt(&T, COPY_PUT, 1);

  }
  return 1;
}

static int wyznacz_tekst_clock(void)
{ double l,l1;
  double x1,x2,y1,y2,x,y,wysokosc,kat2_1,sin_kat2_1,cos_kat2_1;
  int lx,z;
  int len ;
  double kat2_kat1 ;

  if (zmwym.dokladnosc == 0.0) zmwym.dokladnosc = 0.01;  //for not well converted DXF files

  T.justowanie=j_do_lewej;
  T.wysokosc=zmwym.wysokosc;
  if ((wym_luku==1)&&(typ_wymiar==Oluk)&&(kat_w_now==0))
   {
    if (wl.kat2<wl.kat1)
    {
    wl.kat1=wl.kat1-(2*Pi);
    }
    l=(wl.kat2-wl.kat1)*(wl.r-zmwym.linia_ob);
    l=milimetryob(l);
    kat2_kat1=0.5*(wl.kat2+wl.kat1);
   }
    else
     if ((wym_kata==1)&&(kat_w_now==1))
     {
      if (wl.kat2<wl.kat1)
      {
      wl.kat1=wl.kat1-(2*Pi);
      }
      l=((wl.kat2-wl.kat1)/(2*Pi))*360;
      kat2_kat1=0.5*(wl.kat2+wl.kat1);
     }
      else
	{
	l=sqrt((w.x2-w.x0)*(w.x2-w.x0)+(w.y2-w.y0)*(w.y2-w.y0));
	l=milimetryob(l);
	}

  l = round_to_fraction(l, zmwym.dokladnosc);


  if (force_dim_text==FALSE)
   {
     sprintf(T.text, "%f", l);
     lx=strlen(T.text);
   }
    else
     {
      strcpy(T.text, Dim_Text_Adr);
      lx=strlen(T.text);
     }

  if ( NULL != strchr (T.text, '.'))
  {
    len = strlen (T.text) ;
    while (T.text [len - 1] == '0')
    {
      T.text [--len] = '\0' ;
    }
    if (T.text [len - 1] == '.')
    {
       T.text [len - 1] = '\0' ;
    }
  }

  
  if (((wym_luku==1)&&(typ_wymiar==Oluk)) ||
      (kat_w_now==1))
  {
  if (zmwym.linia_ob>0)
   { x1=wl.x+(cos(kat2_kat1)*(wl.r+0.5));
     y1=wl.y+(sin(kat2_kat1)*(wl.r+0.5));
   }
  else
     { x1=wl.x+(cos(kat2_kat1)*(wl.r+0.5));
     y1=wl.y+(sin(kat2_kat1)*(wl.r+0.5));
     }
  wysokosc = zmwym.wysokosc ;
  l=0; T.justowanie=j_srodkowo;
  if(zmwym.kierunek_t==1)
   { T.kat=0;
     if (kat2_kat1<=Pi) y=y1;
       else y=y1-wysokosc;

     if (kat2_kat1<=(Pi*0.5)) x=x1;
       else
	if (kat2_kat1>(Pi*1.5)) x=x1;
	  else x=x1-l;
   }
  else
  {
    T.kat = (kat2_kat1) - (Pi * 0.5);
    y=y1-(sin(T.kat)*0.5*l);
    x=x1-(cos(T.kat)*0.5*l);
  }
  }
  else
  {
   if((w.x2>w.x0) || (w.x2==w.x0 && w.y0>w.y2))
   { x1=w.x0;
     y1=w.y0;
     x2=w.x2;
     y2=w.y2;
   }
  else
   { x1=w.x2;
     y1=w.y2;
     x2=w.x0;
     y2=w.y0;
   }
  wysokosc = zmwym.wysokosc ;
  l=0; T.justowanie=j_srodkowo;

  if(zmwym.kierunek_t==1)
   { T.kat=0;
     y=0.5*(y1+y2)+K1*kat.cos-0.5*wysokosc*fabs(kat.sin);
     x=0.5*(x1+x2)-K1*kat.sin;
     if(kat.sin*K1>=0)
      x-=0.5*l*(1+fabs(kat.sin));
   }
  else
  {
    T.kat = kat.kat * Pi / 180 ;
    y=y1+0.5*(y2-y1-l*kat.sin)+K1*fabs(kat.cos);
    x=x1+0.5*(x2-x1-l*kat.cos)-K1*kat.sin;
  }

   }
  T.warstwa=Current_Layer;
  T.x=(float)x;
  T.y=(float)y;
  T.dl=strlen(&T.text[0]);
  T.n=T18+T.dl;
  T.width = 0;
  T.height = 0;
  if (dodaj_obiekt((BLOK*)dane,(void*)&T)==NULL) return 0;
  else if(WymInter)
  {
      rysuj_obiekt(&T, COPY_PUT, 1);
  }
  return 1;
}


static int s0(void)
{ void *ad;
  int Wst0;
  static long MovAd;
  double kat1,n,n1,n2,l;

  L.warstwa=Current_Layer;
  Ls1.warstwa=Current_Layer;
  St.warstwa=Current_Layer;
  kat1=kat.kat;
  l=sqrt((w.x2-w.x0)*(w.x2-w.x0)+(w.y2-w.y0)*(w.y2-w.y0));
  Wst0=(l>2*Kp2s*cos(Pi*kat0/180.)) ? 0 : 1;
  if(Wst0) n=-1;
  else n=1;
  if(w.x0>=w.x2 && w.y0>=w.y2 && kat1<=90)n=-n;
  if(w.x0>=w.x2 && w.y0<=w.y2 && kat1>=180)n=-n;
  if (Wst0==0 && Wst==1)
   { usun_obiekt((void*)(dane+MovAd));
     okno_r();
     setwritemode(COPY_PUT);
     setcolor(kolory.paper);
     linestyle(Ls1.typ);
     lineC(pikseleX0(Ls1.x1),pikseleY0(Ls1.y1),pikseleX0(Ls1.x2),pikseleY0(Ls1.y2));
     okno_all();
   }
  if(Continue==2 && Wst0==1)
   { L.x1=(float)(w.x0-K1_5*cos(Pi*(kat.kat+45.)/180.));
     L.y1=(float)(w.y0-K1_5*sin(Pi*(kat.kat+45.)/180.));
     L.x2=(float)(w.x0+K1_5*cos(Pi*(kat.kat+45.)/180.));
     L.y2=(float)(w.y0+K1_5*sin(Pi*(kat.kat+45.)/180.));
     if(dodaj_obiekt((BLOK*)dane,(void*)&L)==NULL) return 0;
     else if(WymInter) outline(&L,COPY_PUT,0);
     n1=w.x2>w.x0 ? 1 : -1;
     n2=w.y2>w.y0 ? 1 : -1;
     L.x1=(float)w.x0;
     L.y1=(float)w.y0;
     L.x2=(float)(w.x0-n1*K1_5*fabs(kat.cos));
     L.y2=(float)(w.y0-n2*K1_5*fabs(kat.sin));
     if(dodaj_obiekt((BLOK*)dane,(void*)&L)==NULL) return 0;
     else if(WymInter) outline(&L,COPY_PUT,0);
   }
  if(Wst0==1)
   { Ls1.x1=(float)(w.x2-K1_5*cos(Pi*(kat.kat+45.)/180.));
     Ls1.y1=(float)(w.y2-K1_5*sin(Pi*(kat.kat+45.)/180.));
     Ls1.x2=(float)(w.x2+K1_5*cos(Pi*(kat.kat+45.)/180.));
     Ls1.y2=(float)(w.y2+K1_5*sin(Pi*(kat.kat+45.)/180.));
     if((ad=(void *)dodaj_obiekt((BLOK*)dane,(void*)&Ls1))==NULL) return 0;
     else if(WymInter) outline(&Ls1,COPY_PUT,0);
     MovAd=((char *)ad)-dane;
     n1=w.x2>w.x0 ? 1 : -1;
     n2=w.y2>w.y0 ? 1 : -1;
     Ls.x1=(float)w.x2;
     Ls.y1=(float)w.y2;
     Ls.x2=(float)(w.x2+n1*K1_5*fabs(kat.cos));
     Ls.y2=(float)(w.y2+n2*K1_5*fabs(kat.sin));
     Lsw=1;
   }
  if(Wst0==0)
   { St.xy[2]=(float)w.x0;
     St.xy[3]=(float)w.y0;
     St.xy[0]=(float)(w.x0+n*Kp2s*cos(Pi*(kat1-kat0)/180.));
     St.xy[1]=(float)(w.y0+n*Kp2s*sin(Pi*(kat1-kat0)/180.));
     St.xy[4]=(float)(w.x0+n*Kp2s*cos(Pi*(kat1+kat0)/180.));
     St.xy[5]=(float)(w.y0+n*Kp2s*sin(Pi*(kat1+kat0)/180.));
     St.lp = 6;
     St.n = 8 /*4*/ + St.lp * sizeof (float) ;
     if (options1.uklad_izometryczny)
     {
         izometrize_solid(XY_PLANE, &St);
     }
     if(dodaj_obiekt((BLOK*)dane,(void*)&St)==NULL) return 0;
     else
     if(WymInter)
       rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;
     St.xy[2]=(float)w.x2;
     St.xy[3]=(float)w.y2;
     St.xy[0]=(float)(w.x2-n*Kp2s*cos(Pi*(kat1-kat0)/180.));
     St.xy[1]=(float)(w.y2-n*Kp2s*sin(Pi*(kat1-kat0)/180.));
     St.xy[4]=(float)(w.x2-n*Kp2s*cos(Pi*(kat1+kat0)/180.));
     St.xy[5]=(float)(w.y2-n*Kp2s*sin(Pi*(kat1+kat0)/180.));
     St.lp = 6;
     St.n = 8 /*4*/ + St.lp * sizeof (float) ;
     if (options1.uklad_izometryczny)
     {
         izometrize_solid(XY_PLANE, &St);
     }
     if(dodaj_obiekt((BLOK*)dane,(void*)&St)==NULL) return 0;
     else if(WymInter)
       rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;
     Lsw=0;
   }
  Wst=Wst0;
  return 1;
}

static int s1(void)
{   int n1,n2;
    double ukat;

    if (!options1.uklad_izometryczny) ukat=45.;
    else
    {
        if (Check_if_Equal(kat.kat, 330.))
            ukat=30.;
        else if (Check_if_Equal(kat.kat, 30.))
            ukat=60.;
        else
            ukat=45.;
    }

  L.warstwa=Ls1.warstwa=Current_Layer;
  if (Continue==2)
   { L.x1=(float)(w.x0-Kp2*cos(Pi*(kat.kat+ukat)/180.));
     L.y1=(float)(w.y0-Kp2*sin(Pi*(kat.kat+ukat)/180.));
     L.x2=(float)(w.x0+Kp2*cos(Pi*(kat.kat+ukat)/180.));
     L.y2=(float)(w.y0+Kp2*sin(Pi*(kat.kat+ukat)/180.));
     L.typ = 64 /*5*/;
     if(dodaj_obiekt((BLOK*)dane,(void*)&L)==NULL) return 0;
     else if(WymInter)
     {
         rysuj_obiekt(&L, COPY_PUT, 1);
     }
     Ls1.typ = 32 /*0*/;
     n1=w.x2>w.x0 ? 1 : -1;
     n2=w.y2>w.y0 ? 1 : -1;
     Ls1.x1=(float)w.x0;
     Ls1.y1=(float)w.y0;
     Ls1.x2=(float)(w.x0-n1*K1_5*fabs(kat.cos));
     Ls1.y2=(float)(w.y0-n2*K1_5*fabs(kat.sin));
     if(dodaj_obiekt((BLOK*)dane,(void*)&Ls1)==NULL) return 0;
     else if(WymInter)
     {
         rysuj_obiekt(&Ls1, COPY_PUT, 1);
     }
   }
  L.x1=(float)(w.x2-Kp2*cos(Pi*(kat.kat+ukat)/180.));
  L.y1=(float)(w.y2-Kp2*sin(Pi*(kat.kat+ukat)/180.));
  L.x2=(float)(w.x2+Kp2*cos(Pi*(kat.kat+ukat)/180.));
  L.y2=(float)(w.y2+Kp2*sin(Pi*(kat.kat+ukat)/180.));
  L.typ = 64 /*5*/;
  if(dodaj_obiekt((BLOK*)dane,(void*)&L)==NULL) return 0;
  else if(WymInter)
  {
      rysuj_obiekt(&L, COPY_PUT, 1);
  }
  L.typ = 32 /*0*/;
  n1=w.x2>w.x0 ? 1 : -1;
  n2=w.y2>w.y0 ? 1 : -1;
  Ls.x1=(float)w.x2;
  Ls.y1=(float)w.y2;
  Ls.x2=(float)(w.x2+n1*K1_5*fabs(kat.cos));
  Ls.y2=(float)(w.y2+n2*K1_5*fabs(kat.sin));
  Lsw=1;
  return 1;
}

static int s2(void)
{ int n1,n2;

  L.warstwa=Current_Layer;
  setfillstyle_(SOLID_FILL,zmwym.Lkolor);
  if (Continue==2)
   { Ko.warstwa=Current_Layer;
     Ko.x=(float)w.x0;
     Ko.y=(float)w.y0;
     Ko.r=(float)K0_5;
     if(dodaj_obiekt((BLOK*)dane,&Ko)==NULL) return 0;
     else if(WymInter)
       rysuj_obiekt ((char *)&Ko, COPY_PUT, 1) ;
     n1=w.x2>w.x0 ? 1 : -1;
     n2=w.y2>w.y0 ? 1 : -1;
     Ls1.x1=(float)w.x0;
     Ls1.y1=(float)w.y0;
     Ls1.x2=(float)(w.x0-n1*K1_5*fabs(kat.cos));
     Ls1.y2=(float)(w.y0-n2*K1_5*fabs(kat.sin));
     if(dodaj_obiekt((BLOK*)dane,&Ls1)==NULL) return 0;
     else if(WymInter) {
         rysuj_obiekt(&Ls1, COPY_PUT, 1);
     }
   }
  Ko.warstwa=Current_Layer;
  Ko.x=(float)w.x2;
  Ko.y=(float)w.y2;
  Ko.r=(float)K0_5;
  if(dodaj_obiekt((BLOK*)dane,&Ko)==NULL) return 0;
  else if(WymInter)
       rysuj_obiekt ((char *)&Ko, COPY_PUT, 1) ;
  n1=w.x2>w.x0 ? 1 : -1;
  n2=w.y2>w.y0 ? 1 : -1;
  Ls.x1=(float)w.x2;
  Ls.y1=(float)w.y2;
  Ls.x2=(float)(w.x2+n1*K1_5*fabs(kat.cos));
  Ls.y2=(float)(w.y2+n2*K1_5*fabs(kat.sin));
  Lsw=1;
  return 1;
 }


static int s_l(BOOL draw)
/*-----------------------*/
{ int Wst0;
  double kat1,n,l;

  kat1=kat.kat;
  l=sqrt((w.x2-w.x0)*(w.x2-w.x0)+(w.y2-w.y0)*(w.y2-w.y0));
  Wst0=(l>2*Kp2s*cos(Pi*kat0/180.)) ? 0 : 1;
  if(Wst0) n=-1;
  else n=1;
  if(w.x0>=w.x2 && w.y0>=w.y2 && kat1<=90)n=-n;
  if(w.x0>=w.x2 && w.y0<=w.y2 && kat1>=180)n=-n;
     St.warstwa=Current_Layer;
     St.xy[2]=(float)w.x2;
     St.xy[3]=(float)w.y2;
     St.xy[0]=(float)(w.x2-n*Kp2s*cos(Pi*(kat1-kat0)/180.));
     St.xy[1]=(float)(w.y2-n*Kp2s*sin(Pi*(kat1-kat0)/180.));
     St.xy[4]=(float)(w.x2-n*Kp2s*cos(Pi*(kat1+kat0)/180.));
     St.xy[5]=(float)(w.y2-n*Kp2s*sin(Pi*(kat1+kat0)/180.));
     St.lp = 6;
     St.n = 8  + St.lp * sizeof (float) ;
     if(dodaj_obiekt((BLOK*)dane,&St)==NULL) return 0;
     else if(WymInter) {
        if (draw) rysuj_obiekt((char *) &St, COPY_PUT, 1);
     }
     Ko.warstwa=Current_Layer;
     Ko.x=(float)w.x0;
     Ko.y=(float)w.y0;
     Ko.r=(float)K0_5;
     if(dodaj_obiekt((BLOK*)dane,&Ko)==NULL) return 0;
     else if(WymInter) {
         if (draw) rysuj_obiekt((char *) &Ko, COPY_PUT, 1);
     }
  return 1;
}

static int s_ll(void)
/*-----------------------*/
{ int Wst0;
  double kat1,n,l,xp,yp,r1,dkat;

  kat1=Llw.kat2-(Pi*0.5);

  /* sprawdzic dlugosc luku */
  Wst0=1;

  r1=Llw.r;
  xp=Llw.x+r1*cos(Llw.kat2);
  yp=Llw.y+r1*sin(Llw.kat2);


  if(Wst0) n=-1;
  else n=1;
     dkat=Pi*kat0/180;
     St.warstwa=Current_Layer;
    if (!options1.uklad_izometryczny) {
        St.xy[2] = (float) xp;
        St.xy[3] = (float) yp;
        St.xy[0] = (float) (xp - n * Kp2s * cos(kat1 - dkat));
        St.xy[1] = (float) (yp - n * Kp2s * sin(kat1 - dkat));
        St.xy[4] = (float) (xp - n * Kp2s * cos(kat1 + dkat));
        St.xy[5] = (float) (yp - n * Kp2s * sin(kat1 + dkat));
    }
    else
        make_dim_elliptical_arc_isometric_solid(&ea, ea_start_x, ea_start_y, ea_end_x, ea_end_y, Kp2s, kat0, 0, &St);

     St.lp = 6;
     St.n = 8  + St.lp * sizeof (float) ;
    if (options1.uklad_izometryczny)
    {
     //   izometrize_solid(XY_PLANE, &St);

    }
     if(dodaj_obiekt((BLOK*)dane,&St)==NULL) return 0;
     else if(WymInter)
       rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;


  kat1=Llw.kat1+(Pi*0.5);
  xp=Llw.x+r1*cos(Llw.kat1);
  yp=Llw.y+r1*sin(Llw.kat1);

    if (!options1.uklad_izometryczny) {
        St.xy[2] = (float) xp;
        St.xy[3] = (float) yp;
        St.xy[0] = (float) (xp - n * Kp2s * cos(kat1 - dkat));
        St.xy[1] = (float) (yp - n * Kp2s * sin(kat1 - dkat));
        St.xy[4] = (float) (xp - n * Kp2s * cos(kat1 + dkat));
        St.xy[5] = (float) (yp - n * Kp2s * sin(kat1 + dkat));
    }
    else
        make_dim_elliptical_arc_isometric_solid(&ea, ea_start_x, ea_start_y, ea_end_x, ea_end_y, Kp2s, kat0, 1, &St);

     St.lp = 6;
     St.n = 8  + St.lp * sizeof (float) ;
    if (options1.uklad_izometryczny)
    {
      //  izometrize_solid(XY_PLANE, &St);
    }
     if(dodaj_obiekt((BLOK*)dane,&St)==NULL) return 0;
     else if(WymInter)
       rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;

  return 1;
}


static int s_ll_clock(void)
/*-----------------------*/
{ int Wst0;
  double kat1,n,l,xp,yp,r1,dkat;
  double dl_st=1.75;

  kat1=Llw.kat2-(Pi*0.5);

  /* sprawdzic dlugosc luku */
  Wst0=1;

  r1=Llw.r;
  xp=Llw.x+r1*cos(Llw.kat2);
  yp=Llw.y+r1*sin(Llw.kat2);

  if(Wst0) n=-1;
  else n=1;
     dkat=Pi*kat0/180;
     St.warstwa=Current_Layer;
     St.xy[2]=(float)xp;
     St.xy[3]=(float)yp;
     St.xy[0]=(float)(xp-n*dl_st*cos(kat1-dkat));
     St.xy[1]=(float)(yp-n*dl_st*sin(kat1-dkat));
     St.xy[4]=(float)(xp-n*dl_st*cos(kat1+dkat));
     St.xy[5]=(float)(yp-n*dl_st*sin(kat1+dkat));
     St.lp = 6;
     St.n = 8  + St.lp * sizeof (float) ;
    if (options1.uklad_izometryczny)
    {
        izometrize_solid(XY_PLANE, &St);
    }
     if(dodaj_obiekt((BLOK*)dane,&St)==NULL) return 0;
     else if(WymInter)
       rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;

  kat1=Llw.kat1+(Pi*0.5);
  xp=Llw.x+r1*cos(Llw.kat1);
  yp=Llw.y+r1*sin(Llw.kat1);

     St.xy[2]=(float)xp;
     St.xy[3]=(float)yp;
     St.xy[0]=(float)(xp-n*dl_st*cos(kat1-dkat));
     St.xy[1]=(float)(yp-n*dl_st*sin(kat1-dkat));
     St.xy[4]=(float)(xp-n*dl_st*cos(kat1+dkat));
     St.xy[5]=(float)(yp-n*dl_st*sin(kat1+dkat));
     St.lp = 6;
     St.n = 8  + St.lp * sizeof (float) ;
    if (options1.uklad_izometryczny)
    {
        izometrize_solid(XY_PLANE, &St);
    }
     if(dodaj_obiekt((BLOK*)dane,&St)==NULL) return 0;
     else if(WymInter)
       rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;

  return 1;
}

static int s_o(void)
/*-----------------------*/
{ int Wst0;
  double kat1,n,l;

  St.warstwa=Current_Layer;
  kat1=kat.kat;
  l=sqrt((w.x2-w.x0)*(w.x2-w.x0)+(w.y2-w.y0)*(w.y2-w.y0));
  Wst0=(l>2*Kp2s*cos(Pi*kat0/180.)) ? 0 : 1;
  if(Wst0) n=-1;
  else n=1;
  if(w.x0>=w.x2 && w.y0>=w.y2 && kat1<=90)n=-n;
  if(w.x0>=w.x2 && w.y0<=w.y2 && kat1>=180)n=-n;
     St.xy[2]=(float)w.x2;
     St.xy[3]=(float)w.y2;
     St.xy[0]=(float)(w.x2-n*Kp2s*cos(Pi*(kat1-kat0)/180.));
     St.xy[1]=(float)(w.y2-n*Kp2s*sin(Pi*(kat1-kat0)/180.));
     St.xy[4]=(float)(w.x2-n*Kp2s*cos(Pi*(kat1+kat0)/180.));
     St.xy[5]=(float)(w.y2-n*Kp2s*sin(Pi*(kat1+kat0)/180.));
     St.lp = 6;
     St.n = 8  + St.lp * sizeof (float) ;
    if (options1.uklad_izometryczny)
    {
        izometrize_solid(XY_PLANE, &St);
    }
     if(dodaj_obiekt((BLOK*)dane,&St)==NULL) return 0;
     else if(WymInter)
       rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;
     St.xy[2]=(float)w.x0;
     St.xy[3]=(float)w.y0;
     St.xy[0]=(float)(w.x0+n*Kp2s*cos(Pi*(kat1-kat0)/180.));
     St.xy[1]=(float)(w.y0+n*Kp2s*sin(Pi*(kat1-kat0)/180.));
     St.xy[4]=(float)(w.x0+n*Kp2s*cos(Pi*(kat1+kat0)/180.));
     St.xy[5]=(float)(w.y0+n*Kp2s*sin(Pi*(kat1+kat0)/180.));
     St.lp = 6;
     St.n = 8  + St.lp * sizeof (float) ;
    if (options1.uklad_izometryczny)
    {
        izometrize_solid(XY_PLANE, &St);
    }
     if(dodaj_obiekt((BLOK*)dane,&St)==NULL) return 0;
     else if(WymInter)
       rysuj_obiekt ((char *)&St, COPY_PUT, 1) ;
  return 1;
}

static int outs(BOOL draw)
/*----------------------*/
{
  int ret ;
  double x1p, y1p, x1k, y1k, x2p, y2p, x2k, y2k;
  PLINIA PL;
  LINIA l;
  double r1, r2;

  //counting the angle
  l.x1 = (float)w.x0;
  l.y1 = (float)w.y0;
  l.x2 = (float)w.x2;
  l.y2 = (float)w.y2;
  parametry_lini(&l, &PL);

  if (wym_okregu == 0)
  {
      if(typ_wymiar == Olinia) {
          if (zmwym.strzalka == 0)
          {
              if (PL.dl >= 5.0) r2 = 2.5; //1.5;
              else r2 = 0.0;
          }
          else
          {
              //if (PL.dl >= 5.0) r2 = 2.5; //1.5;
              //else r2 = 0.0;
              r2 = 0;
          }
      }
      else {
          if (PL.dl >= 5.0) r2 = 2.5; //1.5;
          else r2 = 0.0;
      }

	  katkat90(PL.kat);
	  //first aux line

	  if (zmwym.linia_ob > 0) r1 = 1.5; else r1 = -1.5;

	  x1p = w.x0 - r1 * kat.cos;
	  y1p = w.y0 - r1 * kat.sin;
	  x1k = w.x2 - r1 * kat.cos;
	  y1k = w.y2 - r1 * kat.sin;

	  x2p = w.x0 + r1 * kat.cos;
	  y2p = w.y0 + r1 * kat.sin;
	  x2k = w.x2 + r1 * kat.cos;
	  y2k = w.y2 + r1 * kat.sin;

	  Lw.warstwa = Current_Layer;
	  Lw.x1 = (float)x1p;
	  Lw.y1 = (float)y1p;
	  Lw.x2 = (float)x2p;
	  Lw.y2 = (float)y2p;
	  Lw.obiektt2 = O2BlockDim;
	  Lw.obiektt3 = O3WymRoz;
      if(typ_wymiar != Oluk) ////
      {
          if (Lw.x1 != Lw.x2 || Lw.y1 != Lw.y2)
              if (dodaj_obiekt((BLOK *) dane, &Lw) == NULL) return 0;
              else if (WymInter)
              {
                  //outline(&Lw, COPY_PUT, 0);
                  if (draw) rysuj_obiekt(&Lw, COPY_PUT, 1);
              }
      }
  }
  else
  {
      if(typ_wymiar == Olinia) {
          if (zmwym.strzalka == 0)
          {
              if (PL.dl >= 5.0) r2 = 2.5 ; //1.5;
              else r2 = 0.0;
          }
      }
      else {
          if (PL.dl >= 5.0) r2 = 2.5; //1.5;
          else r2 = 0.0;
      }
  }

  Lw.warstwa=Current_Layer;
  Lw.obiektt3 = O3NoWymRoz;

    katkat(PL.kat);

    if(typ_wymiar == Ookrag)
    {

        if (wym_okregu == 0) {

            if (w.x2>w.x0) r2*=-1;

            Lw.x1 = (float)(w.x0 - r2 * kat.cos);
            Lw.y1 = (float)(w.y0 - r2 * kat.sin);
            Lw.x2 = (float)(w.x2 + r2 * kat.cos);
            Lw.y2 = (float)(w.y2 + r2 * kat.sin);
        }
        else
        {
            if (w.x2<w.x0) {

                r2 *= -1;
            }
            else if (w.x2==w.x0) {
                if (w.y2 < w.y0) r2 *= -1;
            }

            Lw.x1 = (float)w.x0;
            Lw.y1 = (float)w.y0;
            Lw.x2 = (float)(w.x2 - r2 * kat.cos);
            Lw.y2 = (float)(w.y2 - r2 * kat.sin);
        }
    }
    else if(typ_wymiar == Oluk)
    {

        if (w.x2<w.x0) {
            r2 *= -1;
        }
        else if (w.x2==w.x0) {
            if (w.y2 < w.y0) r2 *= -1;
        }

        Lw.x1 = (float)w.x0;
        Lw.y1 = (float)w.y0;
        Lw.x2 = (float)(w.x2 - r2 * kat.cos);
        Lw.y2 = (float)(w.y2 - r2 * kat.sin);
    }
    else //Olinia
    {
        if (w.x2<w.x0) {
               r2 *= -1;
        }
        else if (w.x2==w.x0) {
            if (w.y2 < w.y0) r2 *= -1;
        }

        Lw.x1 = (float)(w.x0 + r2 * kat.cos);
        Lw.y1 = (float)(w.y0 + r2 * kat.sin);
        Lw.x2 = (float)(w.x2 - r2 * kat.cos);
        Lw.y2 = (float)(w.y2 - r2 * kat.sin);
    }

    if (Lw.x1 != Lw.x2 || Lw.y1 != Lw.y2)
        if (dodaj_obiekt((BLOK *) dane, &Lw) == NULL) return 0;
        else if (WymInter)
        {
            if (draw) rysuj_obiekt(&Lw, COPY_PUT, 1);
        }

  if (wym_okregu == 0)
  {
	  Lw.warstwa = Current_Layer;
	  Lw.x1 = (float)x1k;
	  Lw.y1 = (float)y1k;
	  Lw.x2 = (float)x2k;
	  Lw.y2 = (float)y2k;
	  Lw.obiektt2 = O2BlockDim;
	  Lw.obiektt3 = O3WymRoz;
      if(typ_wymiar != Oluk) ////
      {
          if (Lw.x1 != Lw.x2 || Lw.y1 != Lw.y2)
              if (dodaj_obiekt((BLOK *) dane, &Lw) == NULL) return 0;
              else if (WymInter)
              {
                  if (draw) rysuj_obiekt(&Lw, COPY_PUT, 1);
              }
      }
  }

  katkat(PL.kat);

  if(!wyznacz_tekst(draw)) return 0;
  if(typ_wymiar == Oluk)
    return s_l(draw);
  if(typ_wymiar == Ookrag)
   {
   if (wym_okregu==0)
    return s_o();
     else return s_l(draw);
   }


  switch(zmwym.strzalka)
  {  case 0 : ret=s0();break;
     case 1 : ret=s1();break;
     case 2 : ret=s2();break;
   }
  return ret;
}



static int outss(void)
/*---------------------*/
{
  int ret ;
  double x1p,y1p,x1k,y1k,x2p,y2p,x2k,y2k;
  double r1;
  double k1, l1;
  //ELLIPTICALARC ea0=eldef, ea=eldef;
  LUK Llw0=ldef;

  Llw.typ=Lw.typ;
  Llw.warstwa=Current_Layer;
  Llw.obiektt2=1;
  Llw.blok=1;
  Llw.x=(float)wl.x;
  Llw.y=(float)wl.y;

    Llw0.x=(float)wl.x;
    Llw0.y=(float)wl.y;

  if (wl.kat2<wl.kat1)
  {
    //wl.kat1=wl.kat1-(2*Pi);
    wl.kat2+=(float)(2*Pi);
  }
  l1=(wl.kat2-wl.kat1)*(wl.r); //-zmwym.linia_ob);
  if (l1>5.0) k1=2.49/wl.r; else k1=0.0;    //1.5

    Llw0.kat1=(float)(wl.kat1);
    Llw0.kat2=(float)(wl.kat2);
    Llw0.r=(float)wl.r;

    if (wl.kat2 < wl.kat1) wl.kat2+=(float)2*M_PI;

  Llw.kat1=(float)(wl.kat1+k1);
  Llw.kat2=(float)(wl.kat2-k1);

    if (Llw.kat1 > (float)2*M_PI) Llw.kat1-=(float)2*M_PI;
    if (Llw.kat2 > (float)2*M_PI) Llw.kat2-=(float)2*M_PI;

    Llw.r=(float)wl.r;

  if(Llw.kat1!=Llw.kat2 || Llw.r!=0)
  {
      if (!options1.uklad_izometryczny)
      {
          if (dodaj_obiekt((BLOK *) dane, &Llw) == NULL) return 0;
          else if (WymInter) rysuj_obiekt((char *) &Llw, COPY_PUT, 1);
      }
      else
      {
          ret = arc_to_isometric_ellipticalarc_a_ea(XY_PLANE, &Llw0, &ea0);
          Get_EllipticalArc_EndPoints (ea0.x, ea0.y, ea0.rx, ea0.ry, ea0.angle, ea0.kat1, ea0.kat2, &ea_start_x, &ea_start_y, &ea_end_x, &ea_end_y);

          ret = arc_to_isometric_ellipticalarc_a_ea(XY_PLANE, &Llw, &ea);
          //Get_EllipticalArc_EndPoints (ea.x, ea.y, ea.rx, ea.ry, ea.angle, ea.kat1, ea.kat2, &ea_start_x, &ea_start_y, &ea_end_x, &ea_end_y);

          ea.typ=Lw.typ;
          ea.warstwa=Current_Layer;
          ea.obiektt2=1;
          ea.blok=1;
          if (dodaj_obiekt((BLOK *) dane, &ea) == NULL) return 0;
          else if (WymInter) rysuj_obiekt((char *) &ea, COPY_PUT, 1);
      }
  }

  Llw.kat1=(float)wl.kat1;
  Llw.kat2=(float)wl.kat2;

  r1=wl.r-zmwym.linia_ob;
  x1p=Llw.x+r1*cos(Llw.kat1);
  y1p=Llw.y+r1*sin(Llw.kat1);
  x1k=Llw.x+r1*cos(Llw.kat2);
  y1k=Llw.y+r1*sin(Llw.kat2);
  if (zmwym.linia_ob>0) r1=wl.r+1.5; else r1=wl.r-1.5;
  x2p=Llw.x+r1*cos(Llw.kat1);
  y2p=Llw.y+r1*sin(Llw.kat1);
  x2k=Llw.x+r1*cos(Llw.kat2);
  y2k=Llw.y+r1*sin(Llw.kat2);

  Lw.warstwa=Current_Layer;
  Lw.x1=(float)x1p;
  Lw.y1=(float)y1p;
  Lw.x2=(float)x2p;
  Lw.y2=(float)y2p;
  Lw.obiektt2 = O2BlockDim;
  Lw.obiektt3 = O3WymRoz;
  if(Lw.x1!=Lw.x2 || Lw.y1!=Lw.y2)
   if(dodaj_obiekt((BLOK*)dane,&Lw)==NULL) return 0;
   else if(WymInter)
   {
       rysuj_obiekt(&Lw, COPY_PUT, 1);
   }

/*  Lw.warstwa=Current_Layer; */
  Lw.x1=(float)x1k;
  Lw.y1=(float)y1k;
  Lw.x2=(float)x2k;
  Lw.y2=(float)y2k;
  if(Lw.x1!=Lw.x2 || Lw.y1!=Lw.y2)
   if(dodaj_obiekt((BLOK*)dane,&Lw)==NULL) return 0;
   else if(WymInter)
   {
       rysuj_obiekt(&Lw, COPY_PUT, 1);
   }


  if(!wyznacz_tekst(TRUE)) return 0;

    return s_ll();

  switch(zmwym.strzalka)
  {  case 0 : ret=s0();break;
     case 1 : ret=s1();break;
     case 2 : ret=s2();break;
   }
  return ret;
}

static int outss_clock(void)
/*------------------------*/
{
  int ret ;
  double x1p,y1p,x1k,y1k,x2p,y2p,x2k,y2k;
  double r1;

  Llw.typ=Lw.typ;
  Llw.warstwa=Current_Layer;
  Llw.obiektt2=1;
  Llw.blok=1;
  Llw.x=(float)wl.x;
  Llw.y=(float)wl.y;
  Llw.kat1=(float)wl.kat1;
  Llw.kat2=(float)wl.kat2;
  Llw.r=(float)wl.r;

  if(Llw.kat1!=Llw.kat2 || Llw.r!=0)
   if(dodaj_obiekt((BLOK*)dane,&Llw)==NULL) return 0;
    else if(WymInter) rysuj_obiekt ((char *)&Llw, COPY_PUT, 1) ;

  r1=wl.r-zmwym.linia_ob;
  x1p=Llw.x+r1*cos(Llw.kat1);
  y1p=Llw.y+r1*sin(Llw.kat1);
  x1k=Llw.x+r1*cos(Llw.kat2);
  y1k=Llw.y+r1*sin(Llw.kat2);
  if (zmwym.linia_ob>0) r1=wl.r+1.5; else r1=wl.r-1.5;
  x2p=Llw.x+r1*cos(Llw.kat1);
  y2p=Llw.y+r1*sin(Llw.kat1);
  x2k=Llw.x+r1*cos(Llw.kat2);
  y2k=Llw.y+r1*sin(Llw.kat2);


  if(!wyznacz_tekst_clock()) return 0;

    return s_ll_clock();

  switch(zmwym.strzalka)
  {  case 0 : ret=s0();break;
     case 1 : ret=s1();break;
     case 2 : ret=s2();break;
   }
  return ret;
}

