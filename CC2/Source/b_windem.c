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

#define __B_WINDEM__

#include "alfversion.h"
#include "menu.h"

#define CENER_T 0x10

static void noooph(void)
{}
#define nooph (void ( *)(int x,int y))noooph

extern int Get_Key (void) ;
extern int openwh(TMENU *menu) ;
extern void  closewh(TMENU *menu) ;
extern int  kbrdy(void);
extern int getchp(void);
extern void  kbflush(void);
extern void  Test_przycisniec(int *keys);
extern void delay(int time);
extern char* decodeString(char *pass, char *key);
extern int get_menu_level(void);
extern void set_menu_level(int menu_l);


static POLE pmDem[]={
/*0*/	 {u8"",' ',0,NULL},
/*1*/	 {_ALFACAD_,' ',0,NULL},
/*2*/	 {u8"",' ',0,NULL},
/*3*/    {u8"© Marek Ratajczak, 2020-2025",' ',0,NULL},
 /*4*/  	 {u8"",' ',0,NULL},
/*5*/	// {u8"marek.ratajczak@markventurecapital.com",' ',0,NULL},
/*5*/	 {u8"author@alfacad.net",' ',0,NULL},
 /*6*/  	 {u8"",' ',0,NULL},
 /*7*/{u8"_______________________________________________",' ',0,NULL},
 /*8 user*/   {u8"",' ',0,NULL},

 /*9 licensed for*/	 {_NOT_LICENSED_,' ',0,NULL},
  ///*9 user*/   {u8"",' ',0,NULL},
  /*10*/   {_NOT_LICENSED1_,' ',0,NULL},
  /*10*/   {"",' ',0,NULL},
      };


static TMENU mDem={ _DEM_ROWS_NO_,0,0,50,4,17,CENER_T,CMNU,CMBR,CMTX,0,0,0,0,0,&pmDem,NULL,NULL};

TMENU mDem1 = { 4,0,0,70,4,17,CENER_T,CMNU,CMBR,CMTX,0,0,0,0,0,&pmDem1,NULL,NULL };

TMENU mPrint={4,0,0,36,4,17,CENER_T,CMNU,CMBR,CMTX,0,0,0,0,0,&pmPrint,NULL,NULL};
      
TMENU mPlot={3,0,0,36,4,17,CENER_T,CMNU,CMBR,CMTX,0,0,0,0,0,&pmPlot,NULL,NULL};

TMENU mPlot_to_com={3,0,0,36,4,17,CENER_T,CMNU,CMBR,CMTX,0,0,0,0,0,&pmPlot_to_com,NULL,NULL};

static void decod_fn (POLE *pmuser, POLE *pmuserno, int *mmax)
{
  int i , len ;

  char enc_pwd[255];
  char dec_pwd[255];
  char key[255] = "";
  char *key_;
  FILE *f_handle;
  char *ptr_alfa1, *ptr_alfa2;

  key_ = key;

  strcpy(enc_pwd, "");
  //reading
  f_handle = fopen("license.alfacad", "rb");
  if (f_handle == NULL)
  {
	  return;
  }
  else
  {
	  fread(&enc_pwd, 1, 128, f_handle);

	  fclose(f_handle);

  }

  strcpy(dec_pwd, decodeString(enc_pwd, key_));

  ptr_alfa1 = strstr(&dec_pwd, "@Al");
  if (ptr_alfa1 != NULL)
  {
	  ptr_alfa2 = strstr(ptr_alfa1, "AD");
	  if (ptr_alfa2!=NULL)
		  {
		      *ptr_alfa1 = '\0';
			  sprintf(&s_prot_date.sz_user_name[0], dec_pwd);
			  strcpy(&s_prot_date.sz_series[0], "01010101");
			  sprintf(pmuser->txt, "%s%s", lic_for, &s_prot_date.sz_user_name[0]);
			  sprintf(pmuserno->txt, "");
			  //strcpy(pmuserno->txt, &s_prot_date.sz_series[0]);
			  *mmax = 11;
		  }
  }

}


T_Prot_Date *Get_Prot_Date (void)
/*-----------------------------*/
{
  return &s_prot_date ;
}

int demo(int alf_window_number, BOOL wait_a_bit)
{
	int k_pressed, keys, mkeys, i;
	static char lastk = '\0';
	register char lk;
	TMENU *mWinDem;
    int menu_level;


	decod_fn(&pmDem[9], &pmDem[10], &mDem.max);


	if (alf_window_number == 0)
	{
		mWinDem = &mDem;
	}
    else  mWinDem = &mDem1;

  mWinDem->xpcz = 1 + (getmaxx() - mWinDem->xdl*WIDTH) / (WIDTH * 2);
  mWinDem->ypcz = 1 + (getmaxy() - mWinDem->max*HEIGHT) / (HEIGHT * 2);

  menu_level=get_menu_level();
  menu_level++;
  set_menu_level(menu_level);

  if(openwh (mWinDem))
  {
  if(kbrdy())
      {lk=getchp();
       if(lk=='\0') lastk=getchp();
       kbflush();
      } 
  Test_przycisniec(&keys);  
  k_pressed=0;
  keys=0;
  i=0;  

  if (wait_a_bit)
  {
	  while (1)
	  {
		  delay(100);
		  if (kbrdy())
		  {
			  lk = getchp();
			  if (lk == '\0') lastk = getchp();
			  kbflush();
			  break;
		  }
		  Test_przycisniec(&keys);
		  if (keys > 0)
		  {
			  mkeys = keys; keys = 0;
			  if ((mkeys)&'\01') break;
			  if ((mkeys)&'\02') break;
			  if ((mkeys)&'\04') break;
		  }
		  i++;
		  if (i > 10) break;
	  }
  }
  
  }

  menu_level--;
  set_menu_level(menu_level);

  return 0 ;
}


int demo_out(int alf_window_number)
{
	TMENU *mWinDem;

	if (alf_window_number == 0)
	{
		mWinDem = &mDem;
		decod_fn(&pmDem[9], &pmDem[10], &mDem.max);
	}
    else  mWinDem = &mDem;

 
  mWinDem->xpcz = 1 + (getmaxx () - mWinDem->xdl*WIDTH) / (2*WIDTH) ;
  mWinDem->ypcz = 1 + (getmaxy () - mWinDem->max*HEIGHT) / (2*HEIGHT) ;

  closewh (mWinDem) ;

  lock_mouse();


  return 0 ;
}


int demo_print(int n)
{
  static void (*CUR)(int ,int);

  CUR = MVCUR ; MVCUR = nooph ;
  mPrint.xpcz = 1 + (getmaxx () - 36*WIDTH) / (2*WIDTH) ;
  mPrint.ypcz = 4 + (getmaxy () - 3*HEIGHT) / (2*HEIGHT) ;
  if (n)
  {
  (openwh (&mPrint)) ;
  }
   else
    {
     closewh (&mPrint) ;
    }

  MVCUR = CUR ;
  return 0 ;
}

int demo_plot(int n)
{
  static void (*CUR)(int ,int);

  CUR = MVCUR ; MVCUR = nooph ;
  mPlot.xpcz = 1 + (getmaxx () - 36*WIDTH) / (2*WIDTH) ;
  mPlot.ypcz = 14 + (getmaxy () - 3*HEIGHT) / (2*HEIGHT) ;
  if (n)
  {
  (openwh (&mPlot)) ;
  }
   else
    {
     closewh (&mPlot) ;
    }

  MVCUR = CUR ;
  return 0 ;
}

int demo_plot_to_com(int n)
{
  static void (*CUR)(int ,int);

  CUR = MVCUR ; MVCUR = nooph ;
  mPlot_to_com.xpcz = 1 + (getmaxx () - 36*WIDTH) / (2*WIDTH) ;
  mPlot_to_com.ypcz = 14 + (getmaxy () - 3*HEIGHT) / (2*HEIGHT) ;
  if (n)
  {
  (openwh (&mPlot_to_com)) ;
  }
   else
    {
     closewh (&mPlot_to_com) ;
    }

  MVCUR = CUR ;
  return 0 ;
}

#undef __B_WINDEM__
