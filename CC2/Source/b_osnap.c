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

#include <forwin.h>
#include <math.h>
#include <stdlib.h>
#include "bib_e.h"
#include "allegext.h"
#include "rysuj_e.h"
#include "o_osnap.h"

enum OSNAP { SKonS,SKon,SSro,SPrz,SCen,SPro,SSty,SNaj, SPoint, SZad };

static int selectq(double *x,double *y)
{
  if(sel.wyj)
  {
    Get_Sel_XY (x, y) ;
    return 1 ;
  }
  *x=X; *y=Y;
  if(!sel.gor || !sel.akt) return 0;
  Dzwonek=0;
  switch(sel.nr)
   { case SKon  : qkoniec (x, y) ;
		  break;
     case SKonS : qkoniecS (x, y) ;
		  break;
     case SSro  : qsrodek (x, y) ;
		  break;
     case SPrz  : qprzeciecie (x, y) ;
		  break;
     case SCen  : qcentrum (x, y) ;
		  break;
     case SNaj  : qnajblizszy (x, y) ;
		  break;
     case SPro  : qprostopadle (x, y) ;
		  break;
     case SSty  : qstycznie (x, y) ;
		  break;
     case SPoint: qpoint (x, y) ;
		  break;
     case SZad  : break;
     default    : break;
   }
  Dzwonek=1;
  return 1;
}


static int SelWyj_(double *X0, double *Y0)
{ double DX,DY;
  int k;
  if((k=selectq(X0, Y0))==1)
   { DX=*X0-X; DY=*Y0-Y;
     if (DX!=0 || DY!=0) mvcurp(DX,DY); //MOVING CURSOR
   }
  return k;
}

extern void *PTR__Sel_Adr,*PTR__Sel_Adr1 ;


EVENT *Get_Event_Point (TMENU *menu , double *X0, double *Y0)
/*----------------------------------------------------------*/
{
	EVENT* ev;
	
  sel.wyj=0;
  sel.jump = 0 ;
  PTR__Sel_Adr = PTR__Sel_Adr1 = NULL ;	/*by wiedziec do ktorego elementu nalezy wybrany punkt*/

  ev=GetEvent(menu);

 if( ev->What==evKeyDown  && ev->Number== ENTER)
   {
     SelWyj_(X0, Y0);
   }
  else
   {
     if( ev->What==evCommandP )
      {
		if( ev->Number==-1 )
		 { if( SelWyj_ (X0, Y0) )
			{
			  ev->What=evKeyDown;
			  ev->Number= ENTER;
			}
		   else ev->What=evNothing;
		 }
		else
		    ev->Number=-(ev->Number+2);
      }
   }
  return ev;
}

