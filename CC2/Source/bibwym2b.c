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

static void CUR_OFF_ON(void)
{
    CUR_OFF(X, Y);
    CUR_ON(X, Y);
}

static void katkat1(double kat1)
{ kat.kat=kat1;
  if(kat.kat>=360) kat.kat-=(long)(kat.kat/360)*360;
  if (kat.kat>=180) kat.kat-=180;
  if (kat.kat>90) kat.kat+=180;
  kat.sin=sin(kat.kat*Pi/180);
  kat.cos=cos(kat.kat*Pi/180);
}

double get_katkat(void)
{ double kat1;
  kat1=kat.kat;
 return kat1;
}

static void katkat(double kat1)
{
  kat.kat=kat1;
  if(kat.kat>=360) kat.kat-=(long)(kat.kat/360)*360;
  if (kat.kat>=180) kat.kat-=180;
  if (kat.kat>90) kat.kat+=180;
  kat.sin=sin(kat.kat*Pi/180);
  kat.cos=cos(kat.kat*Pi/180);
}

static void katkat90(double kat1)
{
	kat.kat = kat1+90;
	if (kat.kat >= 360) kat.kat -= (long)(kat.kat / 360) * 360;
	if (kat.kat >= 180) kat.kat -= 180;
	if (kat.kat > 90) kat.kat += 180;
	kat.sin = sin(kat.kat*Pi / 180);
	kat.cos = cos(kat.kat*Pi / 180);
    CUR_OFF_ON();
}

static void kierunek0(void)
{ double angle_l;
  angle_l=get_angle_l();
  if (angle_l!=0) katkat1(angle_l);
  else
   {
     kat.kat=0;
     kat.sin=0;
     kat.cos=1;
   }  
  kierunek=NoRownolegly;
  wym_kata=0;
  WymNowy();
  menu_par_new((*mWymiarujm.pola)[0].txt,"0");
  CUR_OFF_ON();
}

static void  kierunek90(void)
{ double angle_l;
  angle_l=get_angle_l();
  if (angle_l!=0) katkat1(angle_l + 90);
  else
  {
   kat.kat=90.0;
   kat.sin=1;
   kat.cos=0;
  } 
  kierunek=NoRownolegly;
  wym_kata=0;
  WymNowy();
  menu_par_new((*mWymiarujm.pola)[0].txt,"90");
  CUR_OFF_ON();
}


static void katselx(void)
{ unsigned typ;
  double kat1;
  void  *adr;
  PLINIA PL;
  char st[20];
  double katp;
  int retval_no = 1 ;
  double buf_ret [2] ;
  char sk2 [MaxTextLen] = "" ;
  
  kierunek=NoRownolegly;
  wym_kata=0;
  WymNowy();
  typ=Blinia;
  if ((adr=select_n(&typ,NULL,75))!=NULL)
   { 

     strcpy(sk2,"");
     if (!get_string (sk2, "", MaxTextLen, 0, 15)) return ;
     if (FALSE == calculator (sk2, &retval_no, buf_ret)  || retval_no < 1)
      {
       return ;
      }
     kat1 = buf_ret [0] ;
     if (kat1>0)
      {
       while (kat1>=360) kat1-=360;
      }
      else if (kat1<0)
       {
        while (kat1<=-360) kat1+=360;
       }

	parametry_lini((LINIA*)adr,&PL);
	katkat(kat1+PL.kat);
	katp=kat.kat;

	sprintf (st, "%lg", katp) ;
	menu_par_new((*mWymiarujm.pola)[0].txt,st);
   }
  CUR_OFF_ON();
}

static void  kierunekr(void)
{ kierunek=Rownolegly;
  wym_kata=0;
  WymNowy();
  menu_par_new((*mWymiarujm.pola)[0].txt,"=");
  CUR_OFF_ON();
}

static void  kierunekkat(void)
{
  WymNowy();
  wym_kata=1;
  menu_par_new((*mWymiarujm.pola)[0].txt,"<)");
  CUR_OFF_ON();
}

static void  kierunektext(void)
{
    WymNowy();
    wym_kata=2;
    menu_par_new((*mWymiarujm.pola)[0].txt,"t");
    CUR_OFF_ON();
}

static void  kierunekrad(void)
{
    WymNowy();
    wym_kata=3;
    menu_par_new((*mWymiarujm.pola)[0].txt,"R,D");
    CUR_OFF_ON();
}

static void  wymiarowanier(void)
{ wym_luku=0;
  menu_par_new((*mWymiarujm.pola)[5].txt,"R");
}

static void  wymiarowaniel(void)
{ wym_luku=1;
  menu_par_new((*mWymiarujm.pola)[5].txt,"L");
}

static void  wymiarowanied(void)
{ wym_okregu=0;
  menu_par_new((*mWymiarujm.pola)[6].txt,"D");
}

static void  wymiarowaniero(void)
{ wym_okregu=1;
  menu_par_new((*mWymiarujm.pola)[6].txt,"R");
}



static void  kierunekXk(void)
{ katselx(); }

static void  kierunekX(void)
{ char sk2 [MaxTextLen] = "" ;
  double d;
  char st[20];
  double katp;
  double angle_l;
  int retval_no = 1 ;
  double buf_ret [2] ;
  
  
  angle_l=get_angle_l();
  kierunek=NoRownolegly;
  WymNowy();
  
  strcpy(sk2,"");
  if (!get_string (sk2, "", MaxTextLen, 0, 15)) return ;
  if (FALSE == calculator (sk2, &retval_no, buf_ret)  || retval_no < 1)
   {
    return ;
   }
  d = buf_ret [0] ;
  if (d>0)
   {
    while (d>=360) d-=360;
   }
   else if (d<0)
    {
     while (d<0) d+=360;
    }

  katkat(d+angle_l);

  katp=kat.kat;
  sprintf (st, "%lg", katp) ;
  wym_kata=0;
  menu_par_new((*mWymiarujm.pola)[0].txt,st);
}


void
WymKierR(void)
	{
	kierunek=Rownolegly;
	katkier=NoNoZnany;
	}

void
WymKierX(double Kat)
	{
	kierunek=NoRownolegly;
	kat.kat=Kat;
	if(kat.kat>=360) kat.kat-=(long)(kat.kat/360)*360;
	if (kat.kat>=180) kat.kat-=180;
	if (kat.kat>90) kat.kat+=180;
	kat.sin=sin(kat.kat*Pi/180);
	kat.cos=cos(kat.kat*Pi/180);
	}

int GetKierunekWym(void)
{
  return kierunek;
}

double GetKatWym(void)
{ double kat1;
  kat1=kat.kat;
  return kat1;
}
