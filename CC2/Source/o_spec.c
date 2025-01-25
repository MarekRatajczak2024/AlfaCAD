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
#ifndef LINUX
#include <dos.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_loadf.h"
#include "o_libfun.h"
#include "o_lfile.h"
#include "o_spec.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern BOOL is_utf8(const char* string);

extern T_spec_name * Get_Local_Spec (void) ;
extern T_spec_name * Get_Local_Spec_copy(void);
extern int read_i(int _f, int *int_buf, int l_char);
extern void getdate(date *aktualna_data);
extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
extern void fnmerge (char *path, const char *drive, const char *dir, const char *name, const char *ext);
extern BOOL Edit_Local_Spec(int type, BOOL cur);
extern char *strupr2(char *_s);
extern void decodingwin(char *text);
extern void decodingwin_true(wchar_t text);
extern void ver4_0_to_4_1_Spec(T_spec_name *ptrs_specs, int i_spec_no);
extern void utf8Upper(char* text);
extern int Copy_File_ (int fdest, int fsrc, long nbyte);

static T_spec_name  s__table_spec [Max_No_Spec] ;   /*tablica wartosci parametrow specyfikacji*/
T_spec_name  s__table_spec_copy[Max_No_Spec];   /*tablica wartosci parametrow specyfikacji*/
static int i__specno, i__specno_copy;

static BOOL get_spec_off (int, int *, long *, long *) ;
static BOOL get_spec_param (T_Fstring, T_Fstring) ;
static char add_file[80] ;
static char sz_file [MAXPATH] ;


static BOOL get_spec_param (T_Fstring key_name, T_Fstring ret_string)
/*------------------------------------------------------*/
{
  T_spec_name *ptrs_spec ;


  if (i__specno > Max_No_Spec)
  {
    return FALSE ;
  }
  if (ret_string == NULL)
  {
    ret_string = "" ;
  }
  ptrs_spec = Get_Local_Spec () ;
  strncpy (ptrs_spec [i__specno].spec_name, key_name, Max_Spec_Name) ;
  if (!is_utf8(ptrs_spec[i__specno].spec_name)) decodingwin(ptrs_spec[i__specno].spec_name);
  ptrs_spec [i__specno].spec_name [Max_Spec_Name - 1] = '\0' ;
  
  strncpy (ptrs_spec [i__specno].spec_value, ret_string, Max_Spec_Value) ;
    if (!is_utf8(ptrs_spec[i__specno].spec_value)) decodingwin(ptrs_spec[i__specno].spec_value);
  ptrs_spec [i__specno].spec_value [Max_Spec_Value - 1] = '\0' ;
  
  i__specno++ ;
  return TRUE ;
}

static BOOL get_spec_param_copy(T_Fstring key_name, T_Fstring ret_string)
/*----------------------------------------------------------------------*/
{
	T_spec_name *ptrs_spec;


	if (i__specno > Max_No_Spec)
	{
		return FALSE;
	}
	if (ret_string == NULL)
	{
		ret_string = "";
	}
	ptrs_spec = Get_Local_Spec_copy();
	strncpy(ptrs_spec[i__specno].spec_name, key_name, Max_Spec_Name);
	ptrs_spec[i__specno].spec_name[Max_Spec_Name - 1] = '\0';
	strncpy(ptrs_spec[i__specno].spec_value, ret_string, Max_Spec_Value);
	ptrs_spec[i__specno].spec_value[Max_Spec_Value - 1] = '\0';
	i__specno++;
	return TRUE;
}


void Ini_Spec_Param (void)
//------------------------
{
  i__specno = 0 ;
  Get_Private_Profile_Strings ((T_Fstring)IC_TABLICZKA, get_spec_param) ;
}

void Ini_Spec_Param_copy(void)
//-----------------------------------------------
{
	i__specno_copy = 0;
	Get_Private_Profile_Strings((T_Fstring)IC_TABLICZKA, get_spec_param_copy);
}

static BOOL get_spec_off (int f,
                          int *ptri_spec_no,
                          long *ptrl_spec_no_off,
                          long *ptrl_spec_off)
/*---------------------------------------------------------------------*/
{
  long l_off, l_off1, l_date_size ;
  int i_layersno ;
  BOOL b_ret ;
  double l_add_file;
  int l_add_file1;
  char block_header [VERB_LEN] ;
  int destLenInt;
  float v_float;
  int v_int;
  double v_double;
  int zn_b1, zn_b2 ;
  int marker;

  b_ret = FALSE ;
  l_off=0;
  if (l_off != lseek (f, l_off, SEEK_SET)) return b_ret ;
  
  if (read (f, block_header, VER_LEN) != VER_LEN) return b_ret ;
  block_header [VER_LEN] = 0 ;   /*pisz_zbior nie konczy block_header NULL'em*/
  if ((strcmp (block_header, VER2_0) != 0) && (strcmp (block_header, VER2_1) != 0) &&
	  (strcmp(block_header, VER3_0) != 0) && (strcmp(block_header, VER3_1) != 0) && 
	  (strcmp(block_header, VER4_0) != 0) && (strcmp(block_header, VER4_1) != 0)) return b_ret;
  
  l_off = sizeof(double) * 11 + sizeof(int) * 3 + VER_LEN ;
  if (l_off != lseek (f, l_off, SEEK_SET)) return b_ret ;
  if (read (f, &i_layersno, sizeof(int)) != sizeof(int)) return FALSE ;
  i_layersno = (i_layersno <= MAX_OLD_NUMBER_OF_LAYERS) ? MAX_OLD_NUMBER_OF_LAYERS : i_layersno ;
  if (strcmp (block_header, VER2_0) == 0) l_off = sizeof(double) * 19 + sizeof(int) * 9 + i_layersno * sizeof(LAYER2_0) + sizeof(ZMIENNE) + VER_LEN ;
  else if (strcmp(block_header, VER4_1) != 0) l_off = sizeof(double) * 19 + sizeof(int) * 9 + i_layersno * sizeof(LAYER3) + sizeof(ZMIENNE) + VER_LEN;
  else
  {
      if (read(f, &destLenInt, sizeof(int)) != sizeof(int)) return FALSE;
      l_off = sizeof(double) * 19 + sizeof(int) * 10 + destLenInt + sizeof(ZMIENNE) + VER_LEN;
  }
 
  *ptrl_spec_no_off = l_off ;
  if (l_off != lseek (f, l_off, SEEK_SET)) return b_ret ;
  if (read (f, ptri_spec_no, sizeof(int)) != sizeof(int)) return b_ret ;
  if (read (f, &l_date_size, sizeof(int)) != sizeof(int)) return b_ret ;
  if (*ptri_spec_no > Max_No_Spec||
      *ptri_spec_no < 0 ||
      l_date_size < 0) return b_ret ;

  long  l_off1_ = l_off + sizeof(int) * 4 + sizeof(float) * 2 + sizeof(double) * 7; //31 ;

  if (l_off1_ != lseek (f, l_off1_, SEEK_SET)) return b_ret ;

  if (read(f, &marker, sizeof(int)) != sizeof(int)) return b_ret;

    if (marker==1234567)
        l_off1 = l_off + sizeof(int) * 5 + sizeof(float) * 3 + sizeof(double) * 32 ;
    else
        l_off1 = l_off + sizeof(int) * 4 + sizeof(float) * 2 + sizeof(double) * 31 ;

    if (l_off1 != lseek (f, l_off1, SEEK_SET)) return b_ret ;
  if (read (f, &l_add_file, sizeof(double)) != sizeof(double)) return b_ret ;
  if (l_add_file>=1)
  {    
   if (read (f, &add_file, l_add_file + 1) != (l_add_file + 1)) return b_ret ;
    l_add_file1 = 1;  
   }   
   else  l_add_file1 = 0;  
  *ptrl_spec_off = l_off1 + sizeof(double) + l_add_file1 + l_date_size ;
  b_ret = TRUE ;
  return b_ret ;
}

BOOL Read_Spec1_1 (int f, T_spec_name *ptrs_specs, int i_spec_no)
//------------------------------------------------------------
{
  int i;
  int i_len ;
  char cc;

  for (i = 0 ; i < i_spec_no ; i++)
  {
    if (read_i (f, &i_len, 2) != 2) return FALSE ;
    if (i_len > Max_Spec_Name || i_len < 1) return FALSE ;
    if (read (f, ptrs_specs [i].spec_name, i_len) != i_len) return FALSE ;
    if (read_i (f, &i_len, 2) != 2) return FALSE ;
    if (i_len > Max_Spec_Value || i_len < 1) return FALSE ;
    if (read (f, ptrs_specs [i].spec_value, i_len) != i_len) return FALSE ;
  }
  for (i = i_spec_no ; i < Max_No_Spec ; i++) //wyzerowanie niewypelnionych pol
  {
    strcpy (ptrs_specs [i].spec_name, "") ;
    strcpy (ptrs_specs [i].spec_value, "") ;
  }
  return TRUE ;
}

BOOL Read_Spec (int f, T_spec_name *ptrs_specs, int i_spec_no)
//------------------------------------------------------------
{
  int i, i_len ;


  for (i = 0 ; i < i_spec_no ; i++)
  {
    if (read (f, &i_len, sizeof(int)) != sizeof(int)) return FALSE ;
    if (i_len > Max_Spec_Name || i_len < 1) return FALSE ;
    if (read (f, ptrs_specs [i].spec_name, i_len) != i_len) return FALSE ;
    if (read (f, &i_len, sizeof(int)) != sizeof(int)) return FALSE ;
    if (i_len > Max_Spec_Value || i_len < 1) return FALSE ;
    if (read (f, ptrs_specs [i].spec_value, i_len) != i_len) return FALSE ;
  }
  for (i = i_spec_no ; i < Max_No_Spec ; i++) //wyzerowanie niewypelnionych pol
  {
    strcpy (ptrs_specs [i].spec_name, "") ;
    strcpy (ptrs_specs [i].spec_value, "") ;
  }
  return TRUE ;
}

BOOL Read_Spec_File (char *fn, T_spec_name *ptrs_specs, BOOL b_err)
/*----------------------------------------------------------------*/
{
  char block_header [VERB_LEN] ;
  int f;
  int i_spec_no ;       //liczba specyfikacji
  long l_spec_no_off, l_spec_off ;     //adres ilosci i specyfikacji w pliku
  BOOL b_ret ;
  char stt[20];

  b_ret = FALSE ;
  f = open (fn, O_RDWR|O_BINARY) ;
  if (f != -1)
  {
    if (read (f, block_header, VER_LEN) != VER_LEN) goto error ;
    block_header [VER_LEN] = 0 ;   /*pisz_zbior nie konczy block_header NULL'em*/
    if ((strcmp (block_header, VER2_0) != 0) && (strcmp (block_header, VER2_1) != 0) &&
		(strcmp(block_header, VER3_0) != 0) && (strcmp(block_header, VER3_1) != 0) &&
		(strcmp(block_header, VER4_0) != 0) && (strcmp(block_header, VER4_1) != 0))
      goto error ;  /*VER2_0 i VER2_1 aktualna wersja*/
    if (FALSE == get_spec_off (f, &i_spec_no, &l_spec_no_off, &l_spec_off)) goto error ;
    if (l_spec_off != lseek (f, l_spec_off, SEEK_SET)) goto error ;

    if (FALSE == Read_Spec (f, ptrs_specs, i_spec_no)) goto error ;

	if (strcmp(block_header, VER4_1) != 0) ver4_0_to_4_1_Spec(ptrs_specs, i_spec_no);

    b_ret = TRUE ;
    close (f) ;
    return b_ret ;
  }
  else
  {
    if (TRUE == b_err)
    {
      ErrList (49) ;
    }
    return FALSE ;
  }
error:
  b_ret = FALSE ;
  close (f) ;
  if (b_err == TRUE)
  {
    ErrList (45) ;
  }
  return b_ret ;
}

BOOL Read_Spec_Block (char *fn,
                     char *buf_name, int i_name_len,
                     char *buf_type, int i_type_len,
                     BOOL b_err)
//---------------------------------------------------
{
  char blok_naglowka [VERB_LEN] ;
  int f;
  double Px, Py;
  float f_xip, f_yip ;
#ifndef LINUX
  BLOKD blok = BDdef;
#else
    BLOK blok = Bdef;
#endif
  int len_name, len_type;
  int flags ;

  f=open(fn, O_RDWR|O_BINARY);
  if (f!=-1)
   { if (read(f,blok_naglowka, VERB_LEN) != VERB_LEN) goto error;
     if (
	 (strcmp (blok_naglowka, VERB2_0) != 0 ) &&
	 (strcmp (blok_naglowka, VERB2_1_) != 0 ) &&
     (strcmp (blok_naglowka, VERB2_1) != 0 ) &&
	 (strcmp(blok_naglowka, VERB3_0) != 0) &&
	 (strcmp(blok_naglowka, VERB4_0) != 0) &&
	 (strcmp(blok_naglowka, VERB4_1) != 0))
	    goto error;
     if (read(f,&Px,sizeof(double))!=sizeof(double)) goto error;
     if (read(f,&Py,sizeof(double))!=sizeof(double)) goto error;
     if (read(f,&len_name,sizeof(int))!=sizeof(int)) goto error;
     if (len_name > i_name_len)
     {
       len_name = i_name_len ;
     }
     if (read(f,buf_name,len_name)!=len_name) goto error;
     buf_name [i_name_len - 1] = '\0';
     if (read (f, &blok, sizeof(blok)) != sizeof(blok)) goto error;
     if (blok.obiekt != OdBLOK || blok.dlugosc_opisu_obiektu == 0)
     {
       buf_type [0] = '\0'; /*dotyczy blokow >=64k lub pierwszej wersji blokow nie zawierajacej typu*/
     }
     else
     {
#ifdef LINUX
         lseek(f, -4, SEEK_CUR);
#endif
       if (read (f, &flags, sizeof(flags))!=sizeof(flags))goto error;
       if (flags == EBF_IP)
       {
	 if (read (f, &f_xip, sizeof(f_xip)) != sizeof(f_xip))goto error;
	 if (read (f, &f_yip, sizeof(f_yip)) != sizeof(f_yip))goto error;
       }
       if (read (f, &len_type, sizeof(len_type))!=sizeof(len_type))goto error;
       if (len_type > i_type_len)
       {
         len_type = i_type_len ;
       }
       if (read(f,buf_type ,len_type)!=len_type) goto error;
       buf_type [i_type_len - 1] = '\0';
     }
    close(f);
    return TRUE;
   }
  else
  {
      if ( TRUE == b_err)
      {
	ErrList (49);
      }
      return FALSE;
  }
  error:
  {
      close(f);
      if ( TRUE == b_err)
      {
	ErrList(45);
      }
      return FALSE;
  }
}


BOOL Save_Spec (int f, T_spec_name *ptrs_specs)
//----------------------------------------------
{
  int i ;
  int i_value_len, i_name_len ;
  date aktualna_data;  
  char dzien[6];
  char miesiac[6];
  char rok[6];
  char nowa_data[16];
  char ptrs_specs13[16];

  strcpy(ptrs_specs13, ptrs_specs[13].spec_name);

  //strupr2(ptrs_specs13);
  utf8Upper(ptrs_specs13);


   if ((strcmp(ptrs_specs13,u8"DATA")==0) ||
       (strcmp(ptrs_specs13, u8"DATE") == 0) ||
       (strcmp(ptrs_specs13, u8"ДАТА") == 0) ||
       (strcmp(ptrs_specs13, u8"FECHA") == 0))
     {
      getdate(&aktualna_data);
      sprintf(nowa_data,"%#ld.%#ld.%#ld",aktualna_data.da_day,aktualna_data.da_mon,
              aktualna_data.da_year);
      strcpy(ptrs_specs[13].spec_value,nowa_data);
     }


     
  for (i = 0 ; i < Max_No_Spec ; i++)
  {
    i_name_len = 1 + strlen (ptrs_specs [i].spec_name) ;
    if (write (f, &i_name_len, sizeof(int)) != sizeof(int)) return FALSE ;
    if (write (f, ptrs_specs [i].spec_name, i_name_len) != i_name_len) return FALSE ;
    i_value_len = 1 + strlen (ptrs_specs [i].spec_value) ;
    if (write (f, &i_value_len, sizeof(int)) != sizeof(int)) return FALSE ;
    if (write (f, ptrs_specs [i].spec_value, i_value_len) != i_value_len) return FALSE ;
  }
  return TRUE ;
}

void get_spec_string(int nr, char *st)
/*----------------------------------*/
{ T_spec_name *ptrs_spec ;

  ptrs_spec = Get_Local_Spec () ;
  if ((nr<0) || (nr>Max_No_Spec))
   {
    strcpy(st,"");
    return;
   }
  strcpy(st,ptrs_spec [nr].spec_value);
  return;
}

void get_spec_name(int nr, char *st)
/*----------------------------------*/
{
	T_spec_name *ptrs_spec;

	ptrs_spec = Get_Local_Spec();
	if ((nr < 0) || (nr > Max_No_Spec))
	{
		strcpy(st, "");
		return;
	}
	strcpy(st, ptrs_spec[nr].spec_name);
	return;
}

void put_spec_string(int nr, char *st)
/*----------------------------------*/
{ T_spec_name *ptrs_spec ;

  ptrs_spec = Get_Local_Spec () ;
  if ((nr<0) || (nr>Max_No_Spec))
   {
    return;
   }
  strcpy(ptrs_spec [nr].spec_value, st);
  return;
}

void put_spec_name(int nr, char *st)
/*----------------------------------*/
{
	T_spec_name *ptrs_spec;

	ptrs_spec = Get_Local_Spec();
	if ((nr < 0) || (nr > Max_No_Spec))
	{
		return;
	}
	strcpy(ptrs_spec[nr].spec_name, st);
	return;
}

BOOL Save_Spec_File (char *ptrsz_file, T_spec_name *ptrs_specs)
//------------------------------------------------------------
{
  int f ;
  char  fnd [MAXPATH], drive [MAXDRIVE], dir [MAXDIR], name [MAXFILE], ext [MAXEXT];
  int i_spec_no ;       //liczba specyfikacji
  long l_spec_no_off, l_spec_off ;     //adres ilosci i specyfikacji w pliku
  int i_specno = Max_No_Spec ;
  int k;

  k=fnsplit (ptrsz_file, drive, dir, name, ext);
  fnmerge (fnd, drive, dir, name, ".bak");
  if (FALSE == Copy_File (fnd, ptrsz_file))
  {
    return FALSE ;
  }
  if (-1 != (f = open (ptrsz_file, O_RDWR | O_BINARY)))
  {
    if (FALSE == get_spec_off (f, &i_spec_no, &l_spec_no_off, &l_spec_off)) goto error ;
    if (l_spec_no_off != lseek (f, l_spec_no_off, SEEK_SET)) goto error ;
    if (write (f, &i_specno, sizeof(int)) != sizeof(int)) goto error ;
    if (l_spec_off != lseek (f, l_spec_off, SEEK_SET)) goto error ;
    if (FALSE == Save_Spec (f, ptrs_specs)) goto error ;
    close (f) ;
    return TRUE ;
  }
  else
  {
    ErrList (49) ;
    return FALSE ;
  }
error:
  close (f) ;
  ErrList (45) ;
  return FALSE ;
}


BOOL Save_Spec_Block (char *fn, char *buf_name, char *buf_type)
/*----------------------------------------------------------*/
{
  char blok_naglowka [VERB_LEN] ;
  int f, fb;
  double Px, Py;
  float f_xip, f_yip ;
  int ret;
#ifndef LINUX
  BLOKD blok = BDdef;
#else
    BLOK blok = BDdef;
#endif
  int len_name, len_type, len_typeb, len_desc_b ;
  int flags ;
  char  fn1 [MAXPATH], drive [MAXDRIVE], dir [MAXDIR], name [MAXFILE], ext [MAXEXT];

  flags = fnsplit (fn, drive, dir, name, ext);
  fnmerge(fn1, drive, dir, name, ".bak");

  file_bak (fn, EXT__BLOK);
  len_type = strlen (buf_type) + 1;
  fb=open(fn1, O_RDWR|O_BINARY);
#ifndef LINUX
    f=open(fn, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    f=open(fn, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
  if (f!=-1  &&  fb != -1)
   {
     if (read (fb, blok_naglowka, VERB_LEN) != VERB_LEN) goto error ;
     if (  /*strcmp (blok_naglowka, VERB1_0) != 0 &&  */
	 (strcmp (blok_naglowka, VERB2_0) != 0) &&
	 (strcmp (blok_naglowka, VERB2_1_) != 0) &&
	 (strcmp (blok_naglowka, VERB2_1) != 0))
   	   goto error;
     if (write (f, blok_naglowka, VERB_LEN) != VERB_LEN) goto error ;
     if (read(fb,&Px,sizeof(double))!=sizeof(double)) goto error;
     if (write(f,&Px,sizeof(double))!=sizeof(double)) goto error;

     if (read(fb,&Py,sizeof(double))!=sizeof(double)) goto error;
     if (write(f,&Py,sizeof(double))!=sizeof(double)) goto error;

     if (read(fb,&len_name,sizeof(int))!=sizeof(int)) goto error;
     lseek (fb, len_name, SEEK_CUR);
     len_name = strlen (buf_name) + 1;
     if (write(f,&len_name,sizeof(int))!=sizeof(int)) goto error;
     if (write(f,buf_name,len_name)!=len_name) goto error;

     if (read (fb, &blok, sizeof(blok)) != sizeof(blok)) goto error;
     len_desc_b = blok.dlugosc_opisu_obiektu ;
     f_xip = Px ;
     f_yip = Py ;
     len_typeb = 0 ;
     if (0 < len_desc_b) /*najstarszy typ bloku nie zawieral typu bloku ani flagi*/
     {
       if (read (fb, &flags, sizeof(flags)) != sizeof(flags)) goto error ;
       if (flags == EBF_IP)
       {
	 if (read (fb, &f_xip, sizeof(f_xip)) != sizeof(f_xip))goto error;
	 if (read (fb, &f_yip, sizeof(f_yip)) != sizeof(f_yip))goto error;
       }
       if (read (fb, &len_typeb, sizeof(len_typeb)) != sizeof(len_typeb)) goto error ;
     }
     flags = EBF_IP ;
     blok.dlugosc_opisu_obiektu = sizeof(flags) + 2 * sizeof (f_xip) +
			   sizeof(len_type) + len_type ;
     blok.n += - len_desc_b + blok.dlugosc_opisu_obiektu ;
     if (write (f, &blok, sizeof(blok)) != sizeof(blok)) goto error;
     if (write (f, &flags, sizeof(flags))!=sizeof(flags))goto error;
     if (write (f, &f_xip, sizeof(f_xip)) != sizeof(f_xip))goto error;
     if (write (f, &f_yip, sizeof(f_yip)) != sizeof(f_yip))goto error;
     if (write (f, &len_type, sizeof(len_type))!=sizeof(len_type))goto error;
     if (write(f,buf_type ,len_type)!=len_type) goto error;
     lseek (fb, len_typeb, SEEK_CUR);
     Copy_File_ (f, fb, -1) ;
     close(f);
     close(fb);
     return TRUE;
   }
  else
  {
      ErrList (49);
      if (f != -1) close(f);
      if (fb != -1) close(fb);
      ret=unlink (fn);
      rename (fn1, fn);
      return FALSE;
  }
  error:
  {
      close(f);
      close(fb);
      ErrList(45);
      ret=unlink (fn);
      rename (fn1, fn);
      return FALSE;
  }
}


T_spec_name * Get_Local_Spec (void)
//-----------------------------------
{
  return s__table_spec ;
}

T_spec_name * Get_Local_Spec_copy(void)
//-----------------------------------
{
	return s__table_spec_copy;
}


void Clear_Local_Spec (void)
//--------------------------
{
  int i ;

  for (i = 0 ; i < Max_No_Spec ; i++)
  {
    strcpy (s__table_spec [i].spec_name, "") ;
    strcpy (s__table_spec [i].spec_value, "") ;
  }
}


void Edit_Spec (void)
/*--------------------*/
{
	BOOL ret;
 
  ret = Edit_Local_Spec (EDIT_TABLE, TRUE) ;
}
