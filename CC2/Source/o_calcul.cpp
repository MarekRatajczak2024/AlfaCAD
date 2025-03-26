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

#include "forwin.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#ifndef LINUX
#include <dos.h>
#endif
#include <math.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "forlinux.h"

#include "leak_detector_cpp.hpp"

extern char* strlwr_(char* s);

extern "C" {
extern BOOL Check_if_Equal (double , double ) ;
extern BOOL Check_if_Equal2(double, double);
extern char *fillet_line_to_line(double df_r, LINIA* L1, LINIA* L2);



extern LINIA LG;
extern char* LG1, * LG2;
extern LUK AG;
extern OKRAG CG;
extern ELLIPSE EG;
extern ELLIPTICALARC EAG;
extern char* OG;
extern char* Section_Units_System;
extern char* units_system_si;
extern char* units_system_imp;
static double xg = 0, yg = 0;



}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
//static LINIA LG=Ldef;
//static LUK AG=ldef;
//static char *OG;
/*
extern LINIA LG;
extern char *LG1, *LG2;
extern LUK AG;
extern OKRAG CG;
extern ELLIPSE EG;
extern ELLIPTICALARC EAG;
extern char *OG;
extern char *Section_Units_System;
extern char *units_system_si;
extern char *units_system_imp;
static double xg=0, yg=0;
*/

#define MaxNameSize 50
const int variable = -1 ;
const int maxargs = 9 ;
typedef enum
{
  NAME, NUMBER, END, COMMA=',',
  PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
  PRINT = ';', ASSIGN = '=', LP = '(', RP = ')'
} t_token_value ;

typedef  double (*d_f_va)(double []) ;

struct name
{
  char 		*string ;
  struct 	name *next ;
  int		nargs ;
  int		insert ;
  union
  {
    double 	value ;
    d_f_va	funcptr ;
  } ;
} ;

const int TBLSZ = 23 ;
static name *table [TBLSZ] ;

static t_token_value 	curr_tok ;
static double 		number_value ;
static char 		name_string [MaxNameSize] ;
static int		b_err ;
static BOOL		b_insert_expr ;

static int poz_buf, len_buf ;
static char  *ptr_buf ;

static BOOL 		ini_buf 	(char *buf) ;
static void 		put_back 	(void) ;
static char 		get_char 	(void) ;
static BOOL 		get_buf_state 	(void) ;
static double    	get_number_value(void) ;
static void	      	skip_whitespace (void) ;

static double 		expr 		(void) ;
static double 		term 		(void) ;
static double 		prim 		(void) ;
static t_token_value 	get_token 	(void) ;
static name 		*look 		(char *, int);
static void 		free_store 	(void);
static double 		error 		(int no_err) ;
static BOOL		insertfunction  (char *funcname, int nargs, d_f_va funcptr) ;

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

#define DEG_RAD (Pi2 / 360.)
#define RAD_to_DEG (360. / Pi2)
static double do_sqrt (double e [])
/*--------------------------------*/
{
  return (e [0] < 0)  ?  error (62) : sqrt (e[0]) ;
}

static double do_sin (double e [])
/*------------------------------*/
{
  return  sin (e[0] * DEG_RAD) ;
}

static double do_cos (double e [])
/*------------------------------*/
{
  return  cos (e[0] * DEG_RAD) ;
}

static double do_tan (double e [])
/*--------------------------------*/
{
  e [0] = Angle_Normal (e [0] * DEG_RAD) ;
  if (TRUE == Check_if_Equal (e [0], Pi2 / 4) ||
     TRUE == Check_if_Equal (e [0], Pi2 * 3 / 4) )
  {
    return error (65) ;
  }
  return tan (e[0]) ;
}

static double do_log (double e [])
/*--------------------------------*/
{
  return (e [0] <= 0)  ?  error (65) : log (e[0]) ;
}

static double do_log10 (double e [])
/*--------------------------------*/
{
  return (e [0] <= 0)  ?  error (65) : log10 (e[0]) ;
}

static double do_pow (double e [])
/*--------------------------------*/
{
  if (e [0] == 0 && e [1] <= 0)
  {
    return error (65) ;
  }
  if (e [0] < 0 && (int) e [1] != e [1])
  {
    return error (65) ;
  }
  return pow (e [0], e[1]) ;
}

static double do_min (double e [])
/*--------------------------------*/
{
  return min(e[0],e[1]) ;
}

static double do_min3 (double e [])
/*--------------------------------*/
{ double min1;
  min1=min(e[0],e[1]);
  return min(min1,e[2]) ;
}

static double do_max (double e [])
/*--------------------------------*/
{
  return max(e[0],e[1]) ;
}

static double do_max3 (double e [])
/*--------------------------------*/
{ double max1;
  max1=max(e[0],e[1]);
  return max(max1,e[2]) ;
}

static double do_asin (double e [])
/*--------------------------------*/
{
  return (e [0] > -1 && e [0] < 1) ? asin (e [0]) * RAD_to_DEG: error (65);
}

static double do_asinr (double e [])
/*--------------------------------*/
{
    return (e [0] > -1 && e [0] < 1) ? asin (e [0]): error (65);
}

static double do_acos (double e [])
/*--------------------------------*/
{
  return (e [0] > -1 && e [0] < 1) ? acos (e [0]) * RAD_to_DEG : error (65);
}

static double do_acosr (double e [])
/*--------------------------------*/
{
    return (e [0] > -1 && e [0] < 1) ? acos (e [0]) : error (65);
}

static double do_atan (double e [])
/*--------------------------------*/
{
  return atan (e [0]) * RAD_to_DEG ;
}

static double do_atanr (double e [])
/*--------------------------------*/
{
    return atan (e [0]);
}

static double do_x (double e [])
/*--------------------------------*/
{
    xg=e [0];
    return xg;
}

static double do_y (double e [])
/*--------------------------------*/
{
    yg=e [0];
    return yg;
}

static double do_xy (double e [])
/*--------------------------------*/
{   double factor;
    if (strcmp(Section_Units_System, units_system_si)==0)
        factor=1./SkalaF;
    else factor=25.4/SkalaF;

    xg=e [0]*factor;
    yg=e [1]*factor;
    OG=NULL;
    return xg;
}

static double do_LIN (double e [])
/*--------------------------------*/
{   double length, x1, y1, x2, y2;
    double factor;

    if (strcmp(Section_Units_System, units_system_si)==0)
      factor=1./SkalaF;
    else factor=25.4/SkalaF;

    LG.x1=(float)xg;
    LG.y1=(float)yg;
    x1=xg;
    y1=yg;
    xg+=e[0]*factor;
    yg+=e[1]*factor;
    x2=xg;
    y2=yg;
    LG.x2=(float)xg;
    LG.y2=(float)yg;
    length=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    OG=(char*)&LG;

    return length;
}

static double do_VEC (double e [])
{
    double length, x0, y0, x1, y1, x2, y2;
    double factor;
    double l,a;
    double kos, koc;

    if (strcmp(Section_Units_System, units_system_si)==0)
        factor=1./SkalaF;
    else factor=25.4/SkalaF;

    x0=xg;
    y0=yg;

    if (e[2]==0) {
        LG.x1=(float)xg;
        LG.y1=(float)yg;
    }
    else  //reverted
    {
        LG.x2=(float)xg;
        LG.y2=(float)yg;
    }

    x1=xg;
    y1=yg;

    l=e[0]*factor;
    a=e[1];

    kos = sin(a);
    koc = cos(a);

    Rotate_Point(kos, koc, x0, y0, x0+l, y0, &x2, &y2);
    if (e[2]==0) {
        LG.x2=(float)x2;
        LG.y2=(float)y2;
    }
    else  //reverted
    {
        LG.x1=(float)x2;
        LG.y1=(float)y2;
    }

    length=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    OG=(char*)&LG;

    return length;
}

static double do_VEC2 (double e [])
{
    double length, x0, y0, /*x1, y1,*/ x2, y2;
    double factor;
    double l,a;
    double kos, koc;
    LINIA LG01, LG02;

    if (strcmp(Section_Units_System, units_system_si)==0)
        factor=1./SkalaF;
    else factor=25.4/SkalaF;

    x0=xg;
    y0=yg;

    //part 1
    if (e[2]==0) {
        LG01.x1=(float)x0;
        LG01.y1=(float)y0;
    }
    else  //reverted
    {
        LG01.x2=(float)x0;
        LG01.y2=(float)y0;
    }

    //x1=xg;
    //y1=yg;

    l=e[0]*factor;
    a=e[1];

    kos = sin(a);
    koc = cos(a);

    Rotate_Point(kos, koc, x0, y0, x0+l, y0, &x2, &y2);
    if (e[2]==0) {
        LG01.x2=(float)x2;
        LG01.y2=(float)y2;
    }
    else  //reverted
    {
        LG01.x1=(float)x2;
        LG01.y1=(float)y2;
    }

    //part 2
    if (e[5]==0) {
        LG02.x1=(float)x0;
        LG02.y1=(float)y0;
    }
    else  //reverted
    {
        LG02.x2=(float)x0;
        LG02.y2=(float)y0;
    }

    //x1=xg;
    //y1=yg;

    l=e[3]*factor;
    a=e[4];

    kos = sin(a);
    koc = cos(a);

    Rotate_Point(kos, koc, x0, y0, x0+l, y0, &x2, &y2);
    if (e[5]==0) {
        LG02.x2=(float)x2;
        LG02.y2=(float)y2;
    }
    else  //reverted
    {
        LG02.x1=(float)x2;
        LG02.y1=(float)y2;
    }

    LG.x1=LG01.x1;
    LG.y1=LG01.y1;
    LG.x2=LG02.x2;
    LG.y2=LG02.y2;

    length=sqrt((LG.x2-LG.x1)*(LG.x2-LG.x1)+(LG.y2-LG.y1)*(LG.y2-LG.y1));
    OG=(char*)&LG;

    return length;
}

static double do_ARC (double e [])
/*--------------------------------*/
{   double kos, koc, x0, y0, kat1, kat2, r;
    double length;
    double factor;

    if (e[2]==0) return 0;

    if (strcmp(Section_Units_System, units_system_si)==0)
        factor=1./SkalaF;
    else factor=25.4/SkalaF;

    x0=xg+e[0]*factor;
    y0=yg+e[1]*factor;
    r=e[2]*factor;
    AG.x=(float)x0;
    AG.y=(float)y0;
    AG.r=(float)(r);
    AG.kat1=(float)e[3];
    AG.kat2=(float)e[4];

    if (e[5]==0) {
        kos = sin(AG.kat2);
        koc = cos(AG.kat2);
    }
    else
    {
        kos = sin(AG.kat1);
        koc = cos(AG.kat1);
    }
    Rotate_Point(kos, koc, x0, y0, x0+r, y0, &xg, &yg);

    kat1=e[3];
    kat2=e[4];

    if (kat2<kat1)
    {
        kat1=kat1-(2*Pi);
    }
    length=(kat2-kat1)*r;

    OG=(char*)&AG;
    return length;
}

static double do_CIR (double e [])
/*--------------------------------*/
{   double x0, y0, r;
    double length;
    double factor;

    if (e[0]==0) return 0;

    if (strcmp(Section_Units_System, units_system_si)==0)
        factor=1./SkalaF;
    else factor=25.4/SkalaF;

    x0=xg;
    y0=yg;
    r=e[0]*factor;
    CG.x=(float)x0;
    CG.y=(float)y0;
    CG.r=(float)(r);

    length=2*Pi_*r;

    OG=(char*)&CG;
    return length;
}

static double do_ELP (double e [])
/*--------------------------------*/
{   double kos, koc, x0, y0, kat1, kat2, rx, ry;
    double length;
    double factor;

    if ((e[0]==0) || (e[1]==0)) return 0;

    if (strcmp(Section_Units_System, units_system_si)==0)
        factor=1./SkalaF;
    else factor=25.4/SkalaF;

    x0=xg;
    y0=yg;
    rx=e[0]*factor;
    ry=e[1]*factor;
    EG.x=(float)x0;
    EG.y=(float)y0;
    EG.rx=(float)rx;
    EG.ry=(float)ry;
    EG.angle=(float)e[2];

    length=rx+ry;

    OG=(char*)&EG;
    return length;
}

static double do_ELPA (double e [])
/*--------------------------------*/
{   double x0, y0, kat1, kat2, rx, ry;
    double length;
    double factor;

    if ((e[0]==0) || (e[1]==0)) return 0;

    if (strcmp(Section_Units_System, units_system_si)==0)
        factor=1./SkalaF;
    else factor=25.4/SkalaF;

    x0=xg;
    y0=yg;
    rx=e[0]*factor;
    ry=e[1]*factor;
    kat1=e[3];
    kat2=e[4];

    EAG.x=(float)x0;
    EAG.y=(float)y0;
    EAG.rx=(float)rx;
    EAG.ry=(float)ry;
    EAG.angle=(float)e[2];
    EAG.kat1=(float)kat1;
    EAG.kat2=(float)kat2;

    length=rx+ry;

    OG=(char*)&EAG;
    return length;
}

static double do_FIL (double e [])
{  //fillet
    char *adr;
    double r;
    double factor;

    if (strcmp(Section_Units_System, units_system_si)==0)
        factor=1./SkalaF;
    else factor=25.4/SkalaF;

    r=e[0]*factor;

    adr = fillet_line_to_line(r, (LINIA*)LG1, (LINIA*)LG2);
    OG=NULL;
   return 0;
}



static BOOL insert_function (void)
/*------------------------------*/
{
  BOOL b_ret ;

  b_ret = TRUE ;
  if (
  insertfunction ("sqrt", 1, do_sqrt) == FALSE ||
  insertfunction ("sin", 1, do_sin) == FALSE ||
  insertfunction ("cos", 1, do_cos) == FALSE ||
  insertfunction ("tan", 1, do_tan) == FALSE ||
  insertfunction ("log", 1, do_log) == FALSE ||
  insertfunction ("log10", 1, do_log10) == FALSE ||
  insertfunction ("pow", 2, do_pow) == FALSE ||
  insertfunction ("asin", 1, do_asin) == FALSE ||
  insertfunction ("asinr", 1, do_asinr) == FALSE ||
  insertfunction ("acos", 1, do_acos) == FALSE ||
  insertfunction ("acosr", 1, do_acosr) == FALSE ||
  insertfunction ("atan", 1, do_atan) == FALSE ||
  insertfunction ("atanr", 1, do_atanr) == FALSE ||
  insertfunction ("min", 2, do_min) == FALSE ||
  insertfunction ("min2", 2, do_min)== FALSE ||
  insertfunction ("min3", 3, do_min3)==FALSE ||
  insertfunction ("max", 2, do_max)== FALSE ||
  insertfunction ("max2", 2, do_max)== FALSE ||
  insertfunction ("max3", 3, do_max3)==FALSE ||
  insertfunction ("xy", 2, do_xy)==FALSE ||
  insertfunction ("lin", 2, do_LIN)==FALSE ||
  insertfunction ("arc", 6, do_ARC)==FALSE ||
  insertfunction ("vec", 3, do_VEC)==FALSE ||
  insertfunction ("vec2", 6, do_VEC2)==FALSE ||
  insertfunction ("fil", 1, do_FIL)==FALSE ||
  insertfunction ("cir", 1, do_CIR)==FALSE ||
  insertfunction ("elp", 3, do_ELP)==FALSE ||
  insertfunction ("elpa", 5, do_ELPA)==FALSE)
  {
    b_ret = FALSE ;
  }
  return b_ret ;
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

static BOOL ini_buf (char *buf)
/*-----------------------------*/
{
  if (buf == NULL)
  {
    return FALSE ;
  }
  ptr_buf = buf ;
  poz_buf = 0 ;
  len_buf = (int)strlen (ptr_buf) ;
  return TRUE ;
}

static void put_back (void)
/*-------------------------*/
{
  if (poz_buf > 0)
  {
    poz_buf-- ;
  }
}

static BOOL get_buf_state (void)
/*----------------------------*/
{
  BOOL retval ;

  retval = FALSE ;
  if (poz_buf < len_buf)
  {
    retval = TRUE ;
  }
  return retval ;
}

static char get_char (void)
/*------------------------*/
{
  char ch;

  ch = '\0' ;
  if (poz_buf < len_buf)
  {
     ch = ptr_buf [poz_buf++] ;
  }
  return ch ;
}

static double get_number_value (void)
/*---------------------------------*/
{
  double 	ret_val = 0 ;
  char 	*endptr ;

  ret_val = strtod ((char *)&ptr_buf [poz_buf], (char **)&endptr) ;
  poz_buf += (int)(endptr - ptr_buf - poz_buf) ;
  return ret_val ;
}

static void skip_whitespace (void)
/*-------------------------------*/
{
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

static double 		expr 		(void) ;
static double 		term 		(void) ;
static double 		prim 		(void) ;
static t_token_value 	get_token 	(void) ;
static name 		*look 		(char *, int);
static void 		free_store 	(void);
static double 		error 		(int no_err) ;

static double error (int no_err)
/*------------------------------*/
{
  b_err++;
  no_err = no_err;
  ErrList (no_err) ;
  return 1;
}


static name * look (char *p, int ins = 0)
/*-------------------------------------*/
{
  int ii = 0 ;						//hash
  char *pp = p ;
  while (*pp)
  {
    ii <<= 1 ;
    ii ^= *pp++ ;
  }
  if (ii < 0) ii = -ii;
  ii %= TBLSZ ;

  for (name * n = table [ii]; n; n=n->next)           //search
    if (strcmp (p,n->string) == 0) return n;

  if (ins == 0)  error (63) ;

  name *nn = new name ; 					//insert
  if (nn == NULL)
  {
    error (18) ;
    return NULL ;
  }
  nn->string = new char [strlen (p) + 1];
  if (nn->string == NULL)
  {
    error (18) ;
    return NULL ;
  }
  strcpy (nn->string, p) ;
  nn->nargs = variable ;
  nn->insert = 0;
  nn->value = 1; 		//niezbedne ze wzgledu na dzielenie przez 0
  nn->next = table [ii] ;
  table [ii] = nn ;
  return nn ;
}

static inline name *insert (char *s)
/*----------------------------------*/
{
  return look (s, 1) ;
}

static BOOL insertfunction (char *funcname, int nargs, d_f_va funcptr)
/*-------------------------------------------------------------------*/
{
  BOOL b_ret ;
  b_ret = FALSE ;
  name *n = insert (funcname) ;
  if (n != NULL)
  {
    b_ret = TRUE ;
    n->nargs = nargs ;
    n->funcptr = funcptr ;
  }
  return b_ret ;
}


static void free_store (void)
/*-------------------------*/
{
  int i;
  for (i = 0 ; i < TBLSZ ; i++)
  {
    name * n, * m;
    n = table [i] ;
    while (n)
    {
      delete n->string;
      m = n->next ;
      delete n;
      n = m;
    }
    table [i] = NULL;
  }
}


static double expr (void)
/*-----------------------*/
{
  double left ;

  left = term () ;
  for (;;)
  {
    switch (curr_tok)
    {
      case PLUS :
	get_token () ;
	left += term () ;
	break ;
      case MINUS :
	get_token () ;
	left -= term () ;
	break ;
      default :
	return left ;
    }
  }
}

static double term (void)
/*-----------------------*/
{
  double left, d ;

  left = prim () ;
  for (;;)
  {
    switch (curr_tok)
    {
      case MUL :
	get_token () ;
	left *= prim () ;
	break ;
      case DIV :
	get_token () ;
	d = prim () ;
	if (Check_if_Equal (d, 0) == TRUE)
	{
	  error (58) ;
	  return 1 ;
	}
	left /= d ;
	break ;
      default :
	return left ;
    }
  }
}

static double prim (void)
/*-------------------------*/
{
  double e ;

  switch (curr_tok)
  {
    case NUMBER :
      get_token () ;
      return number_value ;

    case NAME :
      if (get_token () == ASSIGN)
      {
	name * n = insert (name_string) ;
	get_token () ;
	n->value = expr () ;
	n->insert = 1 ;
	b_insert_expr = TRUE ;
	return n->value ;
      }
      else
      {
	if (curr_tok == LP)	//function calls
	{
	  name *n = look (name_string) ;
	  if (n == NULL)
	  {
	    return error (18) ;
	  }
	  if (n->nargs == variable)
	  {
	    return error (60) ;
	  }
	  get_token () ;	//skip LP

	  double e [maxargs] ;
	  int i = 0 ;
	  if (curr_tok != RP)
	  {
	    e [i++] = expr () ;
	  }

	  while (curr_tok == COMMA)
	  {
	    get_token () ;	//skip COMMA
	    e [i++] = expr () ;
	    if (i == maxargs)
	    {
	      break ;
	    }
	  }
	  if (curr_tok != RP)
	  {
	    return error (59) ;
	  }
	  get_token () ;    	//skip RP
	  if (n->nargs != i)
	  {
	    return error (61) ;
	  }
	  return (*n->funcptr)(e) ;
	}
	else
	{
	  name * n = look (name_string) ;
	  if (n == NULL)
	  {
	    return error (18) ;
	  }
	  if (n->nargs == variable)
	  {
	    if (n->insert == 0)
	    { error (57);
	    }
	    return n->value ;
	  }
	  else
	  {
	    return error (64) ;
	  }
	}
      }
      case MINUS :
      get_token () ;
      return - prim () ;
    case LP :
      get_token () ;
      e = expr () ;
      if (curr_tok != RP)
      {
	error (59) ;
	return 1 ;
      }
      get_token () ;
      return e ;
    case END :
      return 1 ;
    default :
      {	error (57) ;
	return 1 ;
      }
  }
}


static t_token_value get_token (void)
/*---------------------------------*/
{
  unsigned char ch ;

  do
  {
    if ( '\0' == (ch = get_char ()))
    {
      return curr_tok = END ;
    }
  }
  while (ch != '\n' && isspace(ch));

  switch (ch)
  {
    case ';' :
	case '|':
    case '\n' :
   skip_whitespace () ;
      return curr_tok = PRINT ;

    case '*' :
    case '/' :
    case '+' :
    case '-' :
    case '(' :
    case ')' :
    case '=' :
    case ',' :
      return curr_tok = (t_token_value)ch;

    case '0' : case '1' : case '2' :  case '3' :  case '4' :
    case '5' : case '6' : case '7':  case '8' :  case '9' :
    case '.' :
    put_back () ;
    number_value = get_number_value () ;
    return curr_tok = NUMBER ;

    default :	       // NAME, NAME= , or error
      if (isalpha (ch))
      {
	char *p = name_string ;
	*p++ = ch ;
	while ( '\0' != (ch = get_char ())  && isalnum (ch))
	{
	   *p++ = ch ;
	}
	put_back () ;
	*p = 0;
	return curr_tok = NAME ;
      }
      error (57) ;
      return curr_tok = PRINT ;
  }
}

static BOOL b__float = TRUE ;

extern "C" {

void Set_Float_Flag (BOOL b_float)
//--------------------------------
{
  b__float = b_float ;
}

int calculator (char *buf, int *retval_no, double *buf_ret)
{
  int no;
  double ret_expr;
  name * n ;
  int i,iii;
  char *pow_, *min_, *min2_, *min3_, *min4_, *min5_, *max_, *max2_, *max3_, *max4_, *max5_, *xy_, *LIN_, *ARC_, *VEC_, *VEC2_, *FIL_, *CIR_, *ELP_, *ELPA_;

  //tutaj mozna sprawdzic czy wszystkie znaki zawieraja cyfry, kropke i przecinek
  //jezeli tak, i jezeli wystapil przecinek, mozna go zamienic na kropke
  pow_=strstr(buf,"pow(");
  min_=strstr(buf,"min(");
  min2_=strstr(buf,"min2(");
  min3_=strstr(buf,"min3(");
  min4_=strstr(buf,"min4(");
  min5_=strstr(buf,"min5(");
  max_=strstr(buf,"max(");
  max2_=strstr(buf,"max2(");
  max3_=strstr(buf,"max3(");
  max4_=strstr(buf,"max4(");
  max5_=strstr(buf,"max5(");
  xy_=strstr(buf,"xy(");
  LIN_=strstr(buf,"lin(");
  ARC_=strstr(buf,"arc(");
  VEC_=strstr(buf,"vec(");
  VEC2_=strstr(buf,"vec2(");
  FIL_=strstr(buf,"fil(");
  CIR_=strstr(buf,"cir(");
  ELP_=strstr(buf,"elp(");
  ELPA_=strstr(buf,"elpa(");

  if ((pow_==NULL) && (min_==NULL) && (min2_==NULL) && (min3_==NULL) && (min4_==NULL) && (min5_==NULL) &&
	  (max_==NULL) && (max2_==NULL) && (max3_==NULL) && (max4_==NULL) && (max5_==NULL) && (xy_==NULL) && (LIN_==NULL) && (ARC_==NULL)
      && (VEC_==NULL) && (VEC2_==NULL) && (FIL_==NULL) && (CIR_==NULL) && (ELP_==NULL) && (ELPA_==NULL))
  {
	  if (strchr(buf, ',')!=NULL)
	  {
		  iii=(int)strlen(buf);
		  for (i=0; i<iii; i++)  if (buf[i]==',') buf[i]='.';
	  }
  }

  b_err = 0;
  strlwr_ (buf) ;
  if (buf[strlen(buf)]!=';') strncat(buf,";",1);
  
  if (*retval_no <= 0   ||   buf_ret == NULL        ||
      buf_ret == NULL   || ini_buf (buf) == FALSE)
  {
    return 0;
  }
  n = insert ("pi") ;
  n->value =  3.1415926535897932385;
  n->insert = 1 ;
  n = insert ("e") ;
  n->value = 2.7182818284590452354;
  n->insert = 1 ;
  if (FALSE == insert_function ())
  {
    return 0 ;
  }
  no = 0;
  curr_tok = PRINT;
  while (get_buf_state () == TRUE)
  {
     get_token ();
    if (curr_tok == END) break ;
    if (curr_tok == PRINT) continue ;
    b_insert_expr = FALSE ;
    ret_expr = expr () ;
    if (curr_tok != END && curr_tok != PRINT && curr_tok != NAME)
    {
      error (57);
    }
    if (b_err > 0)
    {
      break;
    }
    if (b_insert_expr == FALSE)
    {
       buf_ret [no] = ret_expr ;
       if (b__float == TRUE)
       {
         buf_ret [no] = Double_to_Float (buf_ret [no]) ;
       }
       no++ ;
    }
    if (no >= *retval_no)
    {
      break;
    }
  }
  free_store ();
  *retval_no = no ;
  if (b_err > 0)
  {
    *retval_no = 0 ;
  }
  return ((b_err > 0 || no == 0) ? 0 : 1) ;
}

}


