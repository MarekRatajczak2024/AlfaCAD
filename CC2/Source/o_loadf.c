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

#define __LOADF__
#include <forwin.h>
#ifndef LINUX
#include <dos.h>
#include <io.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <values.h>
#define ALLEGWIN
#include <allegext.h>
#include <math.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_dialog.h"
#include "o_libfun.h"
#include "o_loadf.h"
#include "o_lfile.h"
#include "o_spec.h"
#include "b_menu.h"
#include "o_image.h"
#include "alf_res.h"
#include "b_bibe.h"
#include "jpgalleg.h"

#include "menu.h"

#include "o_prn.h"

#include "leak_detector_c.h"

#define FA_NORMAL 0x00
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#define TRUE 1
#define FALSE 0

#define ttf_h 1.6
#define ttf_v 0.9

static char noblock[]="NOBLOK.AXX";

int movex0;
int deltax0;
int movey0;
int deltay0;

static BOOL perm_view_block;

static TEXT *POZ;
static int was_block;

extern void utf8Upper(char* text);
extern char *winfont;
extern char *otffont;
extern char Desktop_Font_File[MAXPATH];

extern int My_GetOpenFolder(char *f_name, char *sz__current_path_file, char *sz__default_path_file, char *sz__current_mask, char *dlg_name); // , int font_height, int font_width, char *font_name)
extern int My_GetOpenFileName(char *f_name, char *sz__current_path_file, char *sz__current_mask, char *dlg_name, BOOL in_out, int cur);
extern char *strupr2(char *_s);
extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
extern void fnmerge (char *path, const char *drive, const char *dir, const char *name, const char *ext);
extern int view_block (void);
extern int ReadBlock_ (char *fn,double *Px,double *Py,RYSPOZ *adp,RYSPOZ *adk,	char *buf, int lenmax, int *object_no, BOOL b_current_ver);
extern void getviewsettings(struct viewporttype  *viewport);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern int setdisk(int _drive);
extern int getdisk(void);
extern void free_mouse(void);
extern void lock_mouse(void);
extern void GrMouseEraseCursor(void);
extern void GrMouseUnEraseCursor(void);
extern void GrMouseDisplayCursor(void);
extern int	findfirst(const char *_pathname, struct ffblk *_ffblk, int _attrib);
extern int	findnext(struct ffblk *_ffblk);
extern char *strlwr2(char *_s);
extern char * AddSlash (char * ptr_path);
extern int Save_Screen(void);
extern int Restore_Screen(void);
extern void setfillstyle_(int pattern, int color);

extern double jednostkiX0(double xx_);
extern double jednostkiY0(double yy_);

extern BOOL preview_blocked;
extern T_spec_name  s__table_spec_copy[Max_No_Spec];
extern char *strlwr2(char *_s);
extern BOOL glb_silent;

extern void ClearInfo (void);

extern void Get_Limits (long off, long offk, int atrybut,
	double *xmin, double *xmax, double *ymin, double *ymax);


extern void d_2d(void);
extern int  cpgnext(LISTBOX *listbox);
extern int  cpgprev(LISTBOX *listbox);
extern int  cbegin(LISTBOX *listbox);
extern int  cend(LISTBOX *listbox);

extern char digits[16];
extern char numbers[16];

extern BOOL Load_File_A(char* f_name, int type, BOOL cur);
extern int czytaj_dxf_blok(char *fn, double *X0, double *Y0, RYSPOZ *adp, RYSPOZ *adk, char *buf, double Jednostki_dxf, double SkalaF_dxf, int latin2_dxf, int block_view);
extern void backup_hatch_pattern(void);
extern void restore_hatch_pattern(void);

static BOOL get_dlg_control (void);
static int proc_dlg(int n);
static int get_disk_type (char drive);   /*0 - A, 1 - B,...*/
static BOOL test_drive (int n);
#ifdef FORWIN32
static BOOL test_drive_win32 (int n);
#endif
static BOOL get_dir_names (void);
static BOOL get_file_names (void);
static BOOL get_file_list (BOOL b_dir);
static BOOL get_drive_names (void);
static BOOL init_dlg_control (void);
static BOOL test_dir (char * , char * , char * );
static BOOL test_file_name (char *);
static void change_sel_in_combobox_drive (void);
static BOOL get_config_param (T_Fstring , T_Fstring );
static void set_default_param (char *, int * ,  char *);
static BOOL ini_drive_control (void);
static BOOL enable_dlg_control_spec (BOOL enable, BOOL b_draw) ;
static BOOL ini_dlg_control_spec (void);
static BOOL list_spec (void);
static BOOL edit_spec_name (void) ;
static BOOL edit_spec (void);
static BOOL save_spec (void);
static BOOL read_spec (void);
static void spec_name_to_spec_list (int) ;
static void spec_names_to_spec_lists (void) ;
static void save_local_spec (void) ;
static BOOL get_prot_control (void) ;
static void make_directory_lab (char *) ;
BOOL read_spec_ini(void);

extern void sw_callback_locked_proc(void);
extern void redrawPreview(int x01, int y01, int x02, int y02, int x0, int y0, BOOL gray, BOOL bw, int gray_sat, int* real_x1, int* real_y1, int* real_x2, int* real_y2, unsigned char adjust, BOOL fill_background, T_Prn_Ini_Date *prn_ini_date);
extern void _normal_rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
extern void change_color(char  *adr, char  *adrk, int kolor_elementu);
extern void blokzap_v(char  *adp, char  *adk, int atrybut, int mode, int kolor);

extern BITMAP *Get_Screenplay(void);

extern double view_vector_scale;
extern void set_global_hidden_blocks_visibility(BOOL block_visibility);

extern void Set_Pointer(void);

#define XpGrSpecification 10
#define XpGrName 10
#define XpGrValue 115
#define YpGrSpecification 10
#define DXGrSpecification 500
#define DXGrName 100
#define DXGrValue 380
#define DYGrSpecification DYLabS*17 + 10
#define DXDialog 590
#define DYDialog DYLabS*17 + 20


#define XpGrFileName (XpGrDirectories + DXGrDirectories + 10)
#define YpGrFileName YpGrFiles
#define DXGrFileName 300
#define DYGrFileName (5 * DYLabS + 2 * DYIL)


#define XpGrFiles 10
#define YpGrFiles 124
#define DXGrFiles (8 * 16) + 80
#define DYGrFiles 165

#define XpGrDirectories (XpGrFiles + DXGrFiles + 10)
#define YpGrDirectories YpGrFiles
#define DXGrDirectories DXGrFiles - 160
#define DYGrDirectories DYGrFiles


#define XpOK    XpSPEC_SAVE
#define YpOK    DYGrSpecification - 25 //YpSPEC_READ + 40 
#define XpCANCEL  XpSPEC_SAVE 
#define YpCANCEL  YpOK - 35 //YpSPEC_READ + 80 
#define XpSPEC_READ   XpSPEC_SAVE
#define YpSPEC_READ   (YpGrSpecification) - 5
#define XpSPEC_SAVE   (XpGrSpecification + DXGrSpecification + 10)
#define YpSPEC_SAVE   (YpSPEC_READ + 50)

#define ID_OK		   1
#define ID_CANCEL      	   2
#define ID_FILE		   3
#define ID_DRIVE	   4
#define ID_EDIT		   5
#define ID_LAB_PATH        6
#define ID_SPECIFICATION   7
//#define ID_EDIT_SPEC   	   8
#define ID_SAVE_SPEC       9
#define ID_READ_SPEC      10
#define ID_READ_SPEC_INI      11
//#define ID_EDIT_SPEC_NAME 11
#define ID_EDIT_PROT 12
#define ID_FILE_RADIO 13
#define ID_PROTOTYPE_RADIO 14
#define ID_PROTOTYPE_CHECK 15
#define ID_VIEW            16
#define ID_MKDIR           17
#define ID_REMOVE          18 
#define ID_RMDIR           19

#define DXBut DXBut0 -20
#define DYBut DYBut0 -10
#define DXBut1 (DXBut0/2)
#define DYBut1 (DYBut0/2)
#define DXBut2 (DXBut0/4)
#define DYBut2 (DYBut0/4)
#define DYBut3 	35  
#define DXBut3 	80  

#define LIST_FILE_MAX 10
#define LIST_DRIVE_MAX 9
#define SPEC_KOR (1)

#define DYLabS  	12 //12
/*------------------------------------------*/

#define LIST_FILES 0
#define LIST_DRIVE 1
#define LIST_SPECIFICATION 2
#define COMBO_SPECIFICATION 1
#define COMBO_SPECIFICATION_COMBO 0
#define Max_No_Files 2000 /*1000*/ /*640*/ /*500*/
#define Max_No_Drive 32  //16
#define Max_File_Name MAXFILE
#define Max_Spec_List_Len 120 //100 //82 //61
#define SPEC_BLOCK_NO 2
#define r10 13

#define WIDTH_DISPLAY 640
#define HEIGHT_DISPLAY 348

typedef struct
{
  char drive_name [MAXDRIVE];
} t_drive_name;

typedef struct
{
  char file_name [Max_File_Name];
} t_file_name;

typedef struct
{
  char dir_name [MAXDIR];
} t_dir_name;


typedef struct
{
  char  spec_list [Max_Spec_List_Len] ;
} t_spec_list ;

static char          drives_current_dir [MAXPATH] ;
static char          sz__current_path_file [MAXPATH] ;
static char          sz_current_path0 [MAXPATH];
static char          sz__current_path_prot [MAXPATH] ;
static char 		sz__edit_file_name [MAXPATH] ;
static char 		sz__edit_prot_name [MAXPATH] ;
static char 		*ptrsz__sheet_name;
static char 		**ptrptrsz__files_txt;
static char 		*ptrsz__drive_txt [Max_No_Drive];  /*tablica pointer'ow do string'ow z table_drive_name */
static char 		*ptrsz__spec_txt [Max_No_Spec] ;
static char 		sz__current_mask [MAXFILE + MAXEXT] ;
static t_file_name 	*ptrs__table_file_name;
static t_drive_name 	s__table_drive_name [Max_No_Drive] ; /*tablica nazw kolejnych dostepnych dyskow dyskow (np. A, C, D,..)*/
static T_spec_name 	s__table_spec_name [Max_No_Spec] ;   /*tablica wartosci parametrow specyfikacji*/
static t_spec_list   s__table_spec_list [Max_No_Spec] ;   /*tablica pamieci na teksty specyfikacji listbox'a*/
static int 		i__no_drives, i__current_drive, i__drive_max; /*liczba dyskow, aktualny dysk, maksymalny numer dysku*/
static long		l__max_no_file_name;
static int 		i__no_file, i__no_dir;
static t_dir_name	*ptrs__drives_current_dir;  /*aktualne sciezki kolejnych dyskow*/
static int 		i__tab_drive_no [Max_No_Drive];  /*tablica numerow kolejnych dyskow (np. 0-A, 1-C, 2-D,..)*/
static BOOL 		b__root;
static char          sz__directory_lab [MAXPATH + 10] ;
static char          sz__directory_prot_lab [MAXPATH + 10] ;
static int 		i__type_load;	/*typ wyboru pliku*/
static BOOL             b__load_file ;  //wybor pliku czy bloku
static int              i__specno=16 ;     //liczba specyfikacji


static int default_drive_cat, default_drive_cat0, default_drive_bl, default_drive_file, default_drive_dxf, default_drive_dwg, default_drive_pcx, default_drive_pro, default_drive_geo, default_drive_cfg, default_drive_txt;
static int foff_menu_file, foff_menu_blok, foff_menu_pcx, foff_menu_pro, foff_menu_geo, foff_menu_cfg, foff_menu_txt;
static int poz_menu_file, poz_menu_blok, poz_menu_pcx, poz_menu_pro, poz_menu_geo, poz_menu_cfg, poz_menu_txt;
static char default_path_bl[MAXPATH], default_path_bl_bak[MAXPATH], default_path_file[MAXPATH], default_path_dxf[MAXPATH], default_path_dwg[MAXPATH], default_path_pcx[MAXPATH], default_path_mak[MAXPATH], default_path_pro[MAXPATH], default_path_geo[MAXPATH], default_path_cfg[MAXPATH], default_path_out[MAXPATH], default_path_txt[MAXPATH], default_path_cat[MAXPATH], default_path_cat0[MAXPATH], default_path_cat_bak[MAXPATH], default_path_cat0_bak[MAXPATH];

static char  edit_specification[16][Max_Spec_Value] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
static char  edit_specification_name [16][Max_Spec_Name] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };

static DARK_LIGHT_LINE line_d_l [1] =
{
  {XpGrFileName + 1, YpGrFileName + 2.5 * DYLabS + DYIL + 6, DXGrFileName -1, 1, LINE_NULL, COLOR_NULL},
} ;

static BUTTON button[4 /*11*//*8*/] =
{

#define ENTER_BUT  0
#define ESC_BUT    1
#define SAVE_SPEC  10
#define READ_SPEC  6
#define BUT_FILE_RADIO 7
#define BUT_PROT_RADIO 8
#define BUT_PROT_CHECK 9
#define BUT_VIEW 2
#define BUT_MKDIR 3
#define BUT_REMOVE 4 
#define BUT_RMDIR 5


  { XpOK, YpOK, 60, 30, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, u8"OK", 0,B_PUSHBUTTON, 87, 1,0, ID_OK, 0, 0,
  },
  { XpCANCEL, YpCANCEL, 60, 30, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, u8"ESC", 0,B_PUSHBUTTON, 83, 1,0, ID_CANCEL, 0, 0,
  },
  { XpSPEC_READ, YpSPEC_READ, 60, 30, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, copy_button, 0,B_PUSHBUTTON, 103, 1,0, ID_READ_SPEC, 0, 0,
  },

  { XpSPEC_READ, YpSPEC_READ + 35, 60, 30, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, copy_button, 0,B_PUSHBUTTON, 105, 1,0, ID_READ_SPEC_INI, 0, 0,
  },

};

static LABEL label[2 /*3*/ /*8*/ /*5*/] =
{
#define LAB_PATH 0
	  { XpGrName + DXIL, YpGrSpecification +3, 0, 0, COLOR_NULL,COLOR_NULL, edycja_n_p,0,0},
	  { XpGrValue + DXIL, YpGrSpecification +3, 0, 0, COLOR_NULL,COLOR_NULL, edycja_p,0,0},
#define LAB_PATH_PROT 4
};

static INPUTLINE edit[32] =
{
#define EDIT_FILE_NAME   0
#define EDIT_SPEC  1 //2
#define EDIT_PROT  3
#define DYComboBoxS	12 //12   //(ED_INF_HEIGHT + 2)

#define ID_EDIT_SPEC_NAME0 21
#define ID_EDIT_SPEC0 22
   { XpGrName + DXIL , YpGrSpecification + 1 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[0], ID_EDIT_SPEC_NAME0,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 1 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[1], ID_EDIT_SPEC0,"",
   },
#define ID_EDIT_SPEC_NAME1 23
#define ID_EDIT_SPEC1 24
   { XpGrName + DXIL , YpGrSpecification + 2 * DYLabS*SPEC_KOR, DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[1], ID_EDIT_SPEC_NAME1,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 2 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[1], ID_EDIT_SPEC1,"",
   },
#define ID_EDIT_SPEC_NAME2 25
#define ID_EDIT_SPEC2 26
	{ XpGrName + DXIL , YpGrSpecification + 3 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[2], ID_EDIT_SPEC_NAME2,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 3 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[2], ID_EDIT_SPEC2,"",
   },
#define ID_EDIT_SPEC_NAME3 27
#define ID_EDIT_SPEC3 28
	{ XpGrName + DXIL , YpGrSpecification + 4 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[3], ID_EDIT_SPEC_NAME3,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 4 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[3], ID_EDIT_SPEC3,"",
   },
#define ID_EDIT_SPEC_NAME4 29
#define ID_EDIT_SPEC4 30
	{ XpGrName + DXIL , YpGrSpecification + 5 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[4], ID_EDIT_SPEC_NAME4,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 5 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[4], ID_EDIT_SPEC4,"",
   },
#define ID_EDIT_SPEC_NAME5 31
#define ID_EDIT_SPEC5 32
	{ XpGrName + DXIL , YpGrSpecification + 6 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[5], ID_EDIT_SPEC_NAME5,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 6 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[5], ID_EDIT_SPEC5,"",
   },
#define ID_EDIT_SPEC_NAME6 33
#define ID_EDIT_SPEC6 34
	{ XpGrName + DXIL , YpGrSpecification + 7 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[6], ID_EDIT_SPEC_NAME6,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 7 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[6], ID_EDIT_SPEC6,"",
   },
#define ID_EDIT_SPEC_NAME7 35
#define ID_EDIT_SPEC7 36
	{ XpGrName + DXIL , YpGrSpecification + 8 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[7], ID_EDIT_SPEC_NAME7,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 8 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[7], ID_EDIT_SPEC7,"",
   },
#define ID_EDIT_SPEC_NAME8 37
#define ID_EDIT_SPEC8 38
	{ XpGrName + DXIL , YpGrSpecification + 9 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[8], ID_EDIT_SPEC_NAME8,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 9 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[8], ID_EDIT_SPEC8,"",
   },
#define ID_EDIT_SPEC_NAME9 39
#define ID_EDIT_SPEC9 40
	{ XpGrName + DXIL , YpGrSpecification + 10 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[9], ID_EDIT_SPEC_NAME9,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 10 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[9], ID_EDIT_SPEC9,"",
   },
#define ID_EDIT_SPEC_NAME10 41
#define ID_EDIT_SPEC10 42
	{ XpGrName + DXIL , YpGrSpecification + 11 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[10], ID_EDIT_SPEC_NAME10,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 11 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[10], ID_EDIT_SPEC10,"",
   },
#define ID_EDIT_SPEC_NAME11 43
#define ID_EDIT_SPEC11 44
	{ XpGrName + DXIL , YpGrSpecification + 12 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[11], ID_EDIT_SPEC_NAME11,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 12 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[11], ID_EDIT_SPEC11,"",
   },
#define ID_EDIT_SPEC_NAME12 45
#define ID_EDIT_SPEC12 46
	{ XpGrName + DXIL , YpGrSpecification + 13 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[12], ID_EDIT_SPEC_NAME12,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 13 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[12], ID_EDIT_SPEC12,"",
   },
#define ID_EDIT_SPEC_NAME13 47
#define ID_EDIT_SPEC13 48
	{ XpGrName + DXIL , YpGrSpecification + 14 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[13], ID_EDIT_SPEC_NAME13,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 14 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[13], ID_EDIT_SPEC13,"",
   },
#define ID_EDIT_SPEC_NAME14 49
#define ID_EDIT_SPEC14 50
	{ XpGrName + DXIL , YpGrSpecification + 15 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[14], ID_EDIT_SPEC_NAME14,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 15 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0, 1, edit_specification[14], ID_EDIT_SPEC14,"",
   },
#define ID_EDIT_SPEC_NAME15 51
#define ID_EDIT_SPEC15 52
	{ XpGrName + DXIL , YpGrSpecification + 16 * DYLabS*SPEC_KOR , DXGrName, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Name, 17, 1, edit_specification_name[15], ID_EDIT_SPEC_NAME15,"",
   },
   { XpGrValue + DXIL , YpGrSpecification + 16 * DYLabS*SPEC_KOR , DXGrValue, DYComboBoxS,COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, Max_Spec_Value, 0 /*72*/, 1, edit_specification[15], ID_EDIT_SPEC15,"",
   },
   
};

static GROUP_BOX gr_box[1] =
{
  { XpGrSpecification, 5, DXGrSpecification, DYGrSpecification,
	COLOR_NULL ,COLOR_NULL, parametry_p, 0, NULL,
  },
};


static LISTBOX listbox [3] =
{
  { XpGrFiles + DXIL, YpGrFiles + DYLabS - DYIL,
    DXGrFiles - 2 * DXIL , DYListBox ,
    10, 10, LIST_DRIVE_MAX, FLB_BAROFF | FLB_LIST, 0, 0, 14,
    COLOR_NULL, COLOR_NULL, COLOR_NULL, 1, NULL, NULL , ID_FILE},

  { XpGrDirectories+ DXIL, YpGrDirectories+ DYLabS - DYIL,
    DXGrDirectories - 2 * DXIL , DYListBox ,
    10, 10, 0, 0, 0, 0, 2,
    COLOR_NULL, COLOR_NULL, COLOR_NULL, 1, NULL, NULL , ID_DRIVE},

  { XpGrSpecification+ DXIL, YpGrSpecification+ DYLabS - DYIL,
    DXGrSpecification- 2 * DXIL , DYListBox ,
    2, 2, 0, 0, 0, 0, Max_Spec_List_Len - 1 ,
    COLOR_NULL, COLOR_NULL, COLOR_NULL, 1, NULL, NULL, ID_SPECIFICATION},
};

static COMBOBOX combobox[1] = {
  { XpGrSpecification + DXIL, YpGrSpecification + 6 * DYLabS*SPEC_KOR - DYIL + 6,
	DXGrSpecification - 2 * DXIL , DYListBox , &listbox[LIST_SPECIFICATION]},
};

static IMAGE images_t[] =
{
 { XpOK , (YpSPEC_READ + 35 + 30 + YpCANCEL) / 2, 64, 64, 106,nameplate_tip},
};

static TDIALOG load_file_dlg =
{
	5, 5, DXDialog, DYDialog+5, //HEIGHT_WORLD - 1,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL,
	0, XpGrFileName + 2 * DXIL + 15 * 8, YpGrFileName + DYLabS + 2 * DYIL,
	edit_table,
	0, &line_d_l,		/*line*/
	2, &label,
	1, &gr_box,
	32,&edit,	/*edit*/
	1, images_t,		/*image*/
	4, &button,
	0, NULL, //&listbox,		/*listbox*/
	0, &combobox,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

long_long min_l(long_long value1, long_long value2)
{
   return ( (value1 < value2) ? value1 : value2);
}


static void prot_file (BOOL b_draw)
//---------------------------------
{
  if (TRUE == Get_Check_Button (&load_file_dlg, ID_FILE_RADIO))
  {
    strcpy (sz__current_path_file, ptrs__drives_current_dir [i__current_drive].dir_name) ;

    if (sz__current_path_file [strlen (sz__current_path_file) - 1] != Slash)
    {
      strcat (sz__current_path_file, SSlash);
    }

      strupr2(sz__current_path_file);
      
    if (b_draw == TRUE)
    {
      Draw_Label(&label [LAB_PATH],TRUE) ;
    }
  }
  else
  {
    strcpy (sz__current_path_prot, ptrs__drives_current_dir [i__current_drive].dir_name) ;

    if (sz__current_path_prot [strlen (sz__current_path_prot) - 1] != Slash)
    {
      strcat (sz__current_path_prot, SSlash);
    }

      strupr2(sz__current_path_prot);
    if (b_draw == TRUE)
    {
      Draw_Label (&label [LAB_PATH_PROT], TRUE) ;
    }
  }
}


#ifdef aaaasaa
static int change_ver (void)
/*-------------------------*/
{
  BOOL b_file ;

  if (i__type_load != CONVERT_OLD_FILE && i__type_load != CONVERT_OLD_BLOCK)
  {
    return 0 ;
  }
  b_file = ((i__type_load == CONVERT_OLD_FILE) ? TRUE : FALSE) ;
  strcpy (ptrsz__sheet_name ,ptrs__drives_current_dir [i__current_drive].dir_name);
  strcat (ptrsz__sheet_name, sz__edit_file_name);
  Change_Version (ptrsz__sheet_name, b_file) ;
  return 0 ;
}
#endif

void save_path_bl(void)
{
	strcpy(default_path_bl_bak, default_path_bl);
}

void save_path_cat(void)
{
	strcpy(default_path_cat_bak, default_path_cat);
}

void save_path_cat0(void)
{
	strcpy(default_path_cat0_bak, default_path_cat0);
}

void null_path_bl(void)
{
	strcpy(default_path_bl, u8"");
}

void null_path_cat(void)
{
	strcpy(default_path_cat, u8"");
}

void null_path_cat0(void)
{
	strcpy(default_path_cat0, u8"");
}


void restore_path_bl(void)
{
	strcpy(default_path_bl, default_path_bl_bak);
}

void restore_path_cat(void)
{
	strcpy(default_path_cat, default_path_cat_bak);
}


static BOOL
get_config_param (T_Fstring key_name, T_Fstring ret_string)
/*-------------------------------------------------------*/
{

   //strupr2 (key_name);
   utf8Upper(key_name);

   if (stricmp (key_name, IC_SHEET) == 0 )

   {
     set_default_param (ret_string, &default_drive_file,
			    default_path_file);
	 set_default_param(ret_string, &default_drive_dxf,
		 default_path_dxf);
	 set_default_param(ret_string, &default_drive_dwg,
		 default_path_dwg);
   }
   else
   if (stricmp (key_name, IC_BLOK) == 0)
   {
     set_default_param (ret_string, &default_drive_bl,
			    default_path_bl);
	 save_path_bl();
   }
   else if (stricmp(key_name, IC_KATALOG) == 0)
	{
		set_default_param(ret_string, &default_drive_cat,
			default_path_cat);
		save_path_cat();
		set_default_param(ret_string, &default_drive_cat0,
			default_path_cat0);
		save_path_cat0();
	}
   else
   if (stricmp (key_name, IC_PCX) == 0)
   {
     set_default_param (ret_string, &default_drive_pcx,
			    default_path_pcx);
   }
   else
   if (stricmp (key_name, IC_PRO) == 0)
   {
     set_default_param (ret_string, &default_drive_pro,
			    default_path_pro);
   }
   else
   if (stricmp (key_name, IC_GEO) == 0)
   {
     set_default_param (ret_string, &default_drive_geo,
			    default_path_geo);
   }
   return TRUE;
}

static void set_default_param (char     * param_val,
			       int      * def_drive,
			       char     * def_path)
/*---------------------------------------------------*/
{

  Set_Full_File_Name (def_path, param_val) ;
#ifndef LINUX
  AddBackslash (def_path) ;
#endif
  //strupr2 (def_path) ;  //TO CHECK
  *def_drive = def_path [0] - 'A' ;
  return ;
}


#ifdef daddw
static void set_default_param (char     * param_val,
			       int      * def_drive,
			       char     * def_path)
/*---------------------------------------------------*/
{
  char dir [MAXDIR]="";
  char drive[MAXDRIVE]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;
  char buf [200];

  strncpy (buf, param_val, 200);
  buf [200-1] = '\0';

  if (buf [strlen (buf) - 1] != Slash)
  {
    strcat (buf, SSlash);
  }

  strupr2(buf);
  flags=fnsplit (buf, drive, dir, file, ext);
  if (flags & DIRECTORY &&
      flags & DRIVE &&

      dir [0] == Slash &&

      test_dir (drive, dir, def_path))
  {
    *def_drive = def_path[0] - 'A';
    return ;
  }
  else
  {
    *def_drive = my_getdisk ();
    Current_Directory (*def_drive + 1, def_path);
  }
  return;
}
#endif

static BOOL get_dlg_control (void)
/*----------------------------------------*/
{
  if (sz__edit_file_name [0] == '\0' ||
      strchr (sz__edit_file_name, '?') != NULL ||
      strchr (sz__edit_file_name, '*') != NULL)
      {
        strcpy (ptrsz__sheet_name , u8"");
        return FALSE;
      }  
  else
  {
    strcpy (ptrsz__sheet_name , sz__current_path_file) ;
    strcat (ptrsz__sheet_name, sz__edit_file_name);
    return TRUE;
  }
}


static BOOL get_prot_control (void)
//--------------------------------
{
  char dir [MAXDIR]="" ;
  char path_full [MAXPATH]="";
  char drive[MAXDRIVE]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;
  T_PTR_Prototype ptrs_prot ;
  
  ptrs_prot = Get_Prototype () ;

  flags=fnsplit (sz__edit_prot_name, drive, dir, file, ext);
  fnmerge (sz__edit_prot_name, drive, dir, file, ext); /*ewentualne obciecie nazwy zbioru do 8 znakow*/
  if ((flags & FILENAME) == 0 || FALSE == test_file_name (file) )
  {
    return FALSE ;
  }
  if (flags & DIRECTORY || flags & DRIVE)
  {
    if (FALSE == test_dir (drive, dir, path_full))
    {
      return FALSE ;
    }

    if (path_full [strlen (path_full) - 1] != Slash)
    {
      strcat (path_full, SSlash);
    }

    strupr2(path_full);
  }
  if(flags & WILDCARDS)
  {
    return FALSE ;
  }
  ptrs_prot->b_prot = Get_Check_Button (&load_file_dlg, ID_PROTOTYPE_CHECK) ;
  strcpy (ptrs_prot->sz_prot, sz__current_path_prot) ;
  strcat (ptrs_prot->sz_prot, file) ;
  strcat (ptrs_prot->sz_prot, ext) ;
  return TRUE ;
}

static int branch_enter (void)
//------------------------------
{
  int ret = Dlg_Ret_Val_Continue ;
  switch (i__type_load)
  {
  case INI_FILE:
  case SAVE_FILE_AS: 
  case CONVERT_OLD_FILE:
  case EDIT_TABLE : strcpy(old_mask_file, sz__current_mask);
                    break;
  
  case INI_BLOK:
  case SAVE_BLOK: 
  case CONVERT_OLD_BLOCK :strcpy(old_mask_blok, sz__current_mask);
                    break;
  case INI_PCX :
  case INI_PCX2BMP :
  case SAVE_PCX:   
  case SAVE_BMP:
  case SAVE_PNG:
  case SAVE_JPEG:
  case SAVE_TIFF:
  case SAVE_EPS:
  case SAVE_PDF:
					strcpy(old_mask_pcx, sz__current_mask);
                    break;
  case INI_PRO:
  case ZAPIS_PRO: strcpy(old_mask_pro, sz__current_mask);
                    break;
  case INI_GEO :strcpy(old_mask_geo, sz__current_mask);
                    break; 
  default: break;                    
  }

  if (i__type_load == INI_FILE)
  {
    get_prot_control () ;
  }
  if (i__type_load == CONVERT_OLD_FILE || i__type_load == CONVERT_OLD_BLOCK)
  {
    strcpy (ptrsz__sheet_name , sz__current_path_file) ;
    strcat (ptrsz__sheet_name, sz__edit_file_name);
    ret = Dlg_Ret_Val_OK ;
  }
  else
  if (get_dlg_control () == TRUE || i__type_load == EDIT_TABLE)
  {
    if (i__type_load == SAVE_FILE_AS || i__type_load == EDIT_TABLE)
    {
      memcpy (Get_Local_Spec (),
		s__table_spec_name,
		sizeof(T_spec_name) * Max_No_Spec) ;
    }
    ret = Dlg_Ret_Val_OK ;
  }
  
  return ret ;
}

static int branch_edit (void)
//----------------------------
{
  int ret = Dlg_Ret_Val_Continue ;
  char dir [MAXDIR]="" ;
  char path_full [MAXPATH]="";
  char drive[MAXDRIVE]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;

  if (i__type_load != SAVE_FILE_AS && i__type_load != EDIT_TABLE)
  {
    enable_dlg_control_spec (0, TRUE) ;
  }
  flags=fnsplit (sz__edit_file_name, drive, dir, file, ext);
  fnmerge (sz__edit_file_name, drive, dir, file, ext); /*ewentualne obciecie nazwy zbioru do 8 znakow*/
  if ((flags & FILENAME) == 0 || FALSE == test_file_name (file) )
  {
    ErrList (46);
    return ret ;
  }
  if (flags & DIRECTORY || flags & DRIVE)
  {
    if (FALSE == test_dir (drive, dir, path_full))
    {
      ErrList (46);
      return ret ;
    }

	 if (path_full [strlen (path_full) - 1] != Slash)
    {
      strcat (path_full, SSlash);
    }

    strupr2(path_full);
    
    i__current_drive = path_full [0] - 'A';
    strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, path_full);
    strcpy (sz__current_path_file, path_full);
  }
  if(flags & WILDCARDS)
  {
    strcpy (sz__current_mask, file);
    if (flags & EXTENSION)
    {
      strcat (sz__current_mask, ext);
    }
    get_file_list (TRUE);
    Draw_Label(&label [LAB_PATH],TRUE);
    change_sel_in_combobox_drive ();
  }
  else
  {
    if (flags & DIRECTORY || flags & DRIVE)
    {
      strcpy (ptrsz__sheet_name, path_full) ;
      strcat (ptrsz__sheet_name, file);
      strcat (ptrsz__sheet_name, ext);
    }
    else
    {
      strcpy (ptrsz__sheet_name , sz__current_path_file) ;
      strcat (ptrsz__sheet_name, sz__edit_file_name) ;
    }
    if (i__type_load == INI_FILE)
    {
      get_prot_control () ;
    }
    switch (i__type_load)
    {
     case INI_FILE:
     case SAVE_FILE_AS: 
     case CONVERT_OLD_FILE:
     case EDIT_TABLE : strcpy(old_mask_file, sz__current_mask);
                    break;
     case INI_BLOK:
     case SAVE_BLOK:
     case CONVERT_OLD_BLOCK :strcpy(old_mask_blok, sz__current_mask);
                    break;
     case INI_PCX:
     case INI_PCX2BMP:
     case SAVE_PCX: 
	 case SAVE_BMP:
	 case SAVE_PNG:
	 case SAVE_JPEG:
	 case SAVE_TIFF:
	 case SAVE_EPS:
	 case SAVE_PDF:
					strcpy(old_mask_pcx, sz__current_mask);
                    break;
     case INI_PRO:
     case ZAPIS_PRO: strcpy(old_mask_pro, sz__current_mask);
                    break;
     case INI_GEO: strcpy(old_mask_geo, sz__current_mask);
                    break;
     default: break;                    
    }
    ret = Dlg_Ret_Val_OK;
  }
  return ret ;
}

static int branch_edit_prot (void)
//--------------------------------
{
  int ret = Dlg_Ret_Val_Continue ;
  char dir [MAXDIR] ="";
  char path_full [MAXPATH]="";
  char drive[MAXDRIVE]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;

  flags=fnsplit (sz__edit_prot_name, drive, dir, file, ext);
  fnmerge (sz__edit_prot_name, drive, dir, file, ext); /*ewentualne obciecie nazwy zbioru do 8 znakow*/
  if ((flags & FILENAME) == 0 || FALSE == test_file_name (file) )
  {
    ErrList (46);
    return ret ;
  }
  if (flags & DIRECTORY || flags & DRIVE)
  {
    if (FALSE == test_dir (drive, dir, path_full))
    {
      ErrList (46);
      return ret ;
    }

    if (path_full [strlen (path_full) - 1] != Slash)
    {
      strcat (path_full, SSlash);
    }

    strupr2(path_full);
    i__current_drive = path_full [0] - 'A';
    strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, path_full);
    strcpy (sz__current_path_prot, path_full);

    if (sz__current_path_prot [strlen (sz__current_path_prot) - 1] != Slash)
    {
     strcat (sz__current_path_prot, SSlash);
    }

     strupr2(sz__current_path_prot);
  }
  if(flags & WILDCARDS)
  {
    strcpy (sz__current_mask, file);
    if (flags & EXTENSION)
    {
      strcat (sz__current_mask, ext);
    }
    get_file_list (TRUE);
    Draw_Label(&label [LAB_PATH_PROT], TRUE) ;
    change_sel_in_combobox_drive () ;
  }
  return ret ;
}


static int branch_file (void)
//---------------------------
{
  int ret = Dlg_Ret_Val_Continue;
  char *ptr;
  char dir [MAXDIR], dir_old [MAXDIR];
  int poz;

  if (i__type_load != SAVE_FILE_AS && i__type_load != EDIT_TABLE)
  {
    enable_dlg_control_spec (0, TRUE) ;
  }
  poz = listbox[LIST_FILES].poz + listbox[LIST_FILES].foff;
  strcpy (dir_old, ptrs__drives_current_dir [i__current_drive].dir_name);
  if (poz < i__no_dir)             /*dir*/
  {
    if (poz == 0  && b__root == FALSE)        /*..*/
    {
      strcpy (dir, ptrs__drives_current_dir [i__current_drive].dir_name);
      dir [strlen (dir) - 1] = '\0';

	  if (NULL == (ptr = strrchr (dir, Slash)))
      {
	    return ret ;    /*tylko w przypadku bledu programu*/
      }

      else
      {
	ptr [1] = '\0';
	strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, dir);
      }
    }
    else
    {
      strcpy(drives_current_dir,ptrs__drives_current_dir [i__current_drive].dir_name);

      if (drives_current_dir[strlen (drives_current_dir) - 1] != Slash)
       {
         strcat (drives_current_dir, SSlash);
         strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, drives_current_dir);
       }

      
      strcat (ptrs__drives_current_dir [i__current_drive].dir_name,
	      listbox[LIST_FILES].txt[poz]);
    }
    if (get_file_list (TRUE) == FALSE)
    {
      strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, dir_old);
      return ret ;
    }
    if (TRUE == Get_Check_Button (&load_file_dlg, ID_FILE_RADIO))
    {
      strcpy (sz__edit_file_name, sz__current_mask) ;
    }
    else
    {
      strcpy (sz__edit_prot_name, sz__current_mask) ;
    }
    prot_file (TRUE) ;
    Draw_ListBox (&listbox[LIST_FILES]);
    ret = Dlg_Ret_Val_Continue_LB;
  }
  else
  if (poz >= i__no_dir  && poz < i__no_dir + i__no_file)  /*file*/
  {
    if (TRUE == Get_Check_Button (&load_file_dlg, ID_FILE_RADIO))
    {
      strcpy (sz__edit_file_name, listbox[LIST_FILES].txt[poz]) ;
      if (strchr (sz__edit_file_name, '.') == NULL)
      {
	strcat (sz__edit_file_name, ".") ;
      }

      Draw_Label (&label [LAB_PATH], TRUE) ;
      
      if (perm_view_block==TRUE)
       {
        if (view_block() != 5) read_spec ();
       }  
    }
    else
    {
      strcpy (sz__edit_prot_name, listbox[LIST_FILES].txt[poz]) ;
      if (strchr (sz__edit_prot_name, '.') == NULL)
      {
	strcat (sz__edit_prot_name, ".") ;
      }
      strcpy (sz__current_path_prot, ptrs__drives_current_dir [i__current_drive].dir_name) ;

      if (sz__current_path_prot [strlen (sz__current_path_prot) - 1] != Slash)
      {
       strcat (sz__current_path_prot, SSlash);
      }

      strupr2(sz__current_path_prot);
      make_directory_lab (sz__current_path_prot) ;
      Draw_Label (&label [LAB_PATH_PROT], TRUE) ;
    }
  }
  return ret ;
}

static int branch_drive (void)
//-----------------------------
{
  int ret = Dlg_Ret_Val_Continue;
  int current_drive_old;
  char  dir_old [MAXDIR];
  int poz;

  if (i__type_load != SAVE_FILE_AS && i__type_load != EDIT_TABLE)
  {
    enable_dlg_control_spec (0, TRUE) ;
  }
  if (TRUE == Get_Check_Button (&load_file_dlg, ID_FILE_RADIO))
  {
    strcpy (sz__edit_file_name, sz__current_mask) ;
  }
  else
  {
    strcpy (sz__edit_prot_name, sz__current_mask) ;
  }
  poz = listbox[LIST_DRIVE].poz + listbox[LIST_DRIVE].foff;
  current_drive_old = i__current_drive;
  strcpy (dir_old, ptrs__drives_current_dir [i__current_drive].dir_name);
  i__current_drive = i__tab_drive_no [poz];
  if (get_file_list (TRUE) == FALSE)
  {
    i__current_drive = current_drive_old;
    strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, dir_old);
    change_sel_in_combobox_drive ();
    Draw_ComboBox (&combobox [0]);
    return ret ;
  }
  Draw_ListBox (&listbox[LIST_FILES]);
  prot_file (TRUE) ;
  return ret ;
}


static int jed_to_piks_x (int jednostki_x)
{ double wsp_x;
  wsp_x = 1.0 * (getmaxx () + 1) / 640 /*WIDTH_DISPLAY*/;
  return (0.5 + jednostki_x * wsp_x);
}

static int jed_to_piks_y (int jednostki_y)
{ double wsp_y;
  wsp_y = 1.0 * (getmaxy () + 1 - 37) / (480 /*HEIGHT_DISPLAY*/ - 37) ;
  return (0.5 + jednostki_y * wsp_y);
}


int ViewPattern(char *FileName, char *ext)
{
	BITMAP *background_pcx_test;
	PALLETE pal;
    BITMAP *screenplay=Get_Screenplay();

    if (strcmp(strupr(ext), ".PCX") == NULL) background_pcx_test = load_pcx(FileName, pal);
    else if (strcmp(strupr(ext), ".PNG") == NULL) background_pcx_test = load_png(FileName, pal);
    else if ((strcmp(strupr(ext), ".JPG") == NULL) || (strcmp(strupr(ext), ".JPEG") == NULL)) background_pcx_test = load_jpg(FileName, pal);
    else return 0;

	if (background_pcx_test)
	{
		drawing_mode(DRAW_MODE_COPY_PATTERN, background_pcx_test, 0, 0);
		_normal_rectfill(screenplay, movex0, movey0 - deltay0, movex0 + deltax0, movey0, palette_color[0]);
		destroy_bitmap(background_pcx_test);
	}
    return 1;
}

int ViewBitmap(char* FileName, char *ext)
{
    BITMAP* background_pcx_test;
    PALLETE pal;


    if (strcmp(strupr(ext), ".PCX") == NULL) background_pcx_test = load_pcx(FileName, pal);
    else if (strcmp(strupr(ext), ".PNG") == NULL) background_pcx_test = load_png(FileName, pal);
    else if ((strcmp(strupr(ext), ".JPG") == NULL) || (strcmp(strupr(ext), ".JPEG") == NULL)) background_pcx_test = load_jpg(FileName, pal);
    
    if (background_pcx_test)
    {
        drawing_mode(DRAW_MODE_SOLID, background_pcx_test, 0, 0);
        _normal_rectfill(screen, movex0, movey0 - deltay0, movex0 + deltax0, movey0, palette_color[0]);
        destroy_bitmap(background_pcx_test);
    }

    return 1;
}


int ViewBlock(char *FileName, BOOL set_color)
{ int len;
  char buf [300];
  double X0,Y0;
  int object_no ;
  double xmin, xmax, ymin, ymax ;
  long off, offk;
  double skalaxy,skalax,skalay;
  double delx, dely;
  double deltax,deltay,movex,movey;
  int deltay00, movey00;
  int k;
  BOOL b_err;
  char *ADP1, *ADK1;
  int ret_read;
  BOOL options1_scale_DIM;
  BOOL change__;
  double ddx, ddy, DDx, DDy, dddx, dddy;
  char pathname[MAXPATH]="";
  char drive[MAXDRIVE]="";
  char dir[MAXDIR]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;
  long_long dane_size00, l;
  double Jednostki_, SkalaF_;
 
  len = 256 ;

  skala_punktu = TRUE;
  options1_scale_DIM = options1.scale_DIM;
  options1.scale_DIM = TRUE;
  change__ = Change;

  flags = fnsplit(FileName, drive, dir, file, ext);
   
  if (strcmp(strlwr2(ext), ".dxf") == 0)
  {
	///DXF option
   char blok_name[254];
   dane_size00 = dane_size;

   //backup_hatch_pattern();
   Jednostki_ = Jednostki;
   SkalaF_ = SkalaF;
      glb_silent = TRUE;
   ret_read = czytaj_dxf_blok(FileName, &X0, &Y0, &ADP1, &ADK1, &blok_name[8], 1.0 /*Jednostki_dxf*/, 1.0 /*SkalaF_dxf*/, 1 /*latin2_dxf*/, 1);
      glb_silent = FALSE;
   Jednostki = Jednostki_;
   SkalaF = SkalaF_;
   //restore_hatch_pattern();

   if ((ADP1 == NULL) || (ADK1 == NULL)) return 0;

   if (ret_read = 0) return 0;
   ///////
  }
  else
  {
	  glb_silent = TRUE;
	  ret_read = ReadBlock_(FileName, &X0, &Y0, &ADP1, &ADK1, buf, len, &object_no, TRUE);
	  glb_silent = FALSE;
	  if ((ret_read!=0) && (ret_read!=5)) return 0;
  }
   
   if (Error==1)
    {
    Info=0;
    Error=0;
    }

   off = 0;
   offk = dane_size ;


   deltax = jednostkiX((double)deltax0);
   deltay = jednostkiY((double)deltay0);

   movex = jednostkiX0((double)movex0); 
   movey = jednostkiY0((double)movey0);

  Get_Limits (ADP1-dane, ADK1-dane, ANieOkreslony, &xmin, &xmax, &ymin, &ymax) ;
  
  skalax=1;
  delx=xmax-xmin;
  if (delx>0) skalax=(deltax/delx);
  skalay=1;
  dely=ymax-ymin;
  if (dely>0) skalay=(deltay/dely);
  if (skalax<skalay) skalaxy=skalax; else skalaxy=skalay;


   ADP1=dane;
   ////ADK1=dane + dane_size;
   zmien_atrybut(ADP1,ADK1,Anormalny,Ablok); 


   transformacja_blok(ADP1,ADK1,-xmin,-ymin,0,0,Tprzesuw,0);

   //mamy deltax, deltay
   ddx = delx * skalaxy;
   ddy = dely * skalaxy;

   dddx = (deltax - ddx) / 2.0;
   dddy = (deltay - ddy) / 2.0;

   transformacja_blok(ADP1,ADK1,0,0,skalaxy,skalaxy,Tskala,0);
   transformacja_blok(ADP1,ADK1,movex+dddx/*+Xp*/ /*X-X0*/,movey+dddy/*+Yp*/ /*Y-Y0*/,0,0,Tprzesuw,0);
   if (set_color == TRUE) change_color((char *)ADP1, ADK1, LiniaG.kolor);
   //blokzap(ADP1,ADK1,Ablok,COPY_PUT,1);

   view_vector_scale=skalaxy;
   blokzap_v(ADP1, ADK1, Ablok, COPY_PUT, 1);

    view_vector_scale=1.0;

    zmien_atrybut(ADP1, ADK1, Ablok, Ausuniety);  //Ablok

   glb_silent = TRUE;
   usun_blok(ADP1, ADK1);
   glb_silent = FALSE;

   skala_punktu = FALSE;
   options1.scale_DIM = options1_scale_DIM;
   Change = change__;

  return ret_read;
}


int ViewInsBlock(long adr, BOOL set_color)
{ int len;
    char buf [300]; //[100];
    double X0,Y0;
    int object_no ;
    double xmin, xmax, ymin, ymax ;
    long off, offk;
    double skalaxy,skalax,skalay;
    double delx, dely;
    double deltax,deltay,movex,movey;
    int deltay00, movey00;
    int k;
    BOOL b_err;
    char *ADP1, *ADK1;
    BOOL options1_scale_DIM;
    double ddx, ddy, DDx, DDy, dddx, dddy;
    BLOK *ptr_blk;
    char *ptr_buf;
    long bufor_size;
    char *dane_buf;

    ptr_blk=(BLOK*)(adr+dane);
    bufor_size=sizeof(NAGLOWEK)+(ptr_blk->n);

    ptr_buf=(char*)malloc(bufor_size);
    if (ptr_buf==NULL) return 0;

    memmove(ptr_buf,ptr_blk,bufor_size);
    ptr_blk=(BLOK*)ptr_buf;

    dane_buf=dane;
    dane=ptr_buf;

    ADP1=ptr_blk;
    ADK1=(char*)ptr_blk+sizeof(NAGLOWEK)+(ptr_blk->n)-1;

    skala_punktu = TRUE;
    options1_scale_DIM = options1.scale_DIM;
    options1.scale_DIM = TRUE;

    off = 0;
    offk = dane_size ;

    deltax = jednostkiX((double)deltax0);
    deltay = jednostkiY((double)deltay0);

    movex = jednostkiX0((double)movex0);
    movey = jednostkiY0((double)movey0);

    set_global_hidden_blocks_visibility(TRUE);

    Get_Limits (ADP1-dane, ADK1-dane, ANieOkreslony, &xmin, &xmax, &ymin, &ymax) ;

    skalax=1;
    delx=xmax-xmin;
    if (delx>0) skalax=(deltax/delx);
    skalay=1;
    dely=ymax-ymin;
    if (dely>0) skalay=(deltay/dely);
    if (skalax<skalay) skalaxy=skalax; else skalaxy=skalay;

    zmien_atrybut(ADP1,ADK1,Anormalny,Ablok);

    transformacja_blok(ADP1,ADK1,-xmin,-ymin,0,0,Tprzesuw,0);

    ddx = delx * skalaxy;
    ddy = dely * skalaxy;

    dddx = (deltax - ddx) / 2.0;
    dddy = (deltay - ddy) / 2.0;

    transformacja_blok(ADP1,ADK1,0,0,skalaxy,skalaxy,Tskala,0);

    transformacja_blok(ADP1,ADK1,movex+dddx/*+Xp*/ /*X-X0*/,movey+dddy/*+Yp*/ /*Y-Y0*/,0,0,Tprzesuw,0);
    if (set_color == TRUE) change_color((char *)ADP1, ADK1, LiniaG.kolor);

    view_vector_scale=skalaxy;
    blokzap_v(ADP1, ADK1, Ablok, COPY_PUT, 1);

    set_global_hidden_blocks_visibility(FALSE);
    view_vector_scale=1.0;

    skala_punktu = FALSE;
    options1.scale_DIM = options1_scale_DIM;

    dane=dane_buf;

    free(ptr_buf);
    return 1;  // ret_read;
}

int ViewDrawing(int x1, int y1, int x2, int y2, int x0, int y0, BOOL gray, BOOL bw, int gray_sat, int *real_x1, int *real_y1, int *real_x2, int *real_y2, unsigned char adjust, BOOL fill_background, T_Prn_Ini_Date *prn_ini_date)
{
	int len;
	char buf[300];
	double X0, Y0;
	int object_no;
	double xmin, xmax, ymin, ymax;
	long off, offk;
	double skalaxy, skalax, skalay;
	double delx, dely;
	double deltax, deltay, movex, movey;
	int deltay00, movey00;
	int k;
	BOOL b_err;
	char *ADP1, *ADK1;
	int ret_read;
	BOOL options1_scale_DIM;
	BOOL change__;
	double ddx, ddy, DDx, DDy, dddx, dddy;
	int ret_view;

	ret_view = 1;
	
	redrawPreview(x1, y1, x2, y2, x0, y0, gray, bw, gray_sat, real_x1, real_y1, real_x2, real_y2, adjust, fill_background, prn_ini_date);

	return ret_view;
}



int view_block (void)
/*-------------------*/
{ int k; 
  BOOL b_err;
  int key;
  char *ext;
  static int xb1,yb1,xb2,yb2;
  struct viewporttype viewinfo;
  int ret;

  return 0;  //depreciated

  ret = 1;
  getviewsettings(&viewinfo); 
  
  if (getmaxy()>800)
   {
    yb1=jed_to_piks_y(436);
    yb2=jed_to_piks_y(256);
    xb1=jed_to_piks_x(165);
    xb2=jed_to_piks_x(291);
 /*   movex0=168;
    deltax0=120; */
   }
   else
   if (getmaxy()>700)  /*1024x768*/
   {
    yb1=jed_to_piks_y(439);
    yb2=jed_to_piks_y(256);
    xb1=jed_to_piks_x(166);
    xb2=jed_to_piks_x(291);
 /*   movex0=168;
    deltax0=120; */
   }
   else
    if (getmaxy()>500)  /*800x600*/
   {
    yb1=jed_to_piks_y(448);
    yb2=jed_to_piks_y(265/*269*/);
    xb1=jed_to_piks_x(167);
    xb2=jed_to_piks_x(291);
 /*   movex0=168;
    deltax0=120; */
   }
   else              /*640x480, 640x250 */
    {
     yb1=jed_to_piks_y(455);
     yb2=jed_to_piks_y(274/*277*/);
     xb1=jed_to_piks_x(168);
     xb2=jed_to_piks_x(292);
  /*   movex0=168;
     deltax0=120; */
    }
    
  movex0=169;
  deltax0=121;  


  setviewport(0,0,getmaxx(),getmaxy(),1); 

  if (was_block==0)
   {
    k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
   }
    else k=0;

    if ( k )
     {
     }
    else
     {
      was_block=1;
      zwolnienie_pamieci();
      if (get_dlg_control () == FALSE)
      {
        strcpy(ptrsz__sheet_name,noblock);
      }
      setfillstyle_(SOLID_FILL, 8);
      bar(xb1,yb1,xb2,yb2);
      ret=ViewBlock(ptrsz__sheet_name, FALSE);
     }
  setviewport(viewinfo.left,viewinfo.top,viewinfo.right,viewinfo.bottom,1);
  return ret;
}

void remove_file (void)
{
  char st[MAXPATH]="";
  int k, key, poz, foff;
  
  strcpy (ptrsz__sheet_name, sz__current_path_file) ;
  strcat (ptrsz__sheet_name, sz__edit_file_name);
  strcpy(st,u8"Delete file ");
  strcat(st, ptrsz__sheet_name);
  strcat(st," ? Y/N/Esc :");
  komunikat_str (st);
  
  key = Get_Legal_Key ("YNyn\033");
  komunikat (0);
  if (key == ESC) return;
  if (key == 'Y' || key == 'y')
    {
      poz = listbox[LIST_FILES].poz;
      foff = listbox[LIST_FILES].foff;
      k=unlink(ptrsz__sheet_name);
      if (k==-1) 
        {
          strcpy(st,u8"Błąd usuwania pliku ");
          strcat(st,ptrsz__sheet_name);
          komunikat_str (st);
          key = Get_Legal_Key ("\033\015");
          komunikat (0);
        }
        else
         {
          get_file_list (TRUE);
          if (foff>0)
           {
            listbox[LIST_FILES].poz = poz;
            listbox[LIST_FILES].foff = foff - 1;
           } 
           else
            {
             listbox[LIST_FILES].poz = poz - 1;
             listbox[LIST_FILES].foff = foff;
            }
          Draw_ListBox (&listbox [LIST_FILES]);
          /**/
          if (TRUE == Get_Check_Button (&load_file_dlg, ID_FILE_RADIO))
          {
           strcpy (sz__edit_file_name, sz__current_mask) ;
          }
           else
            {
              strcpy (sz__edit_prot_name, sz__current_mask) ;
            }
          /**/
         }
    }
}

void make_dir (void)
{
  char st[MAXDIR];
  int k, key;
  char dir [MAXPATH], dir_old [MAXPATH];
  char *ptr;
  struct viewporttype viewinfo;

  GrMouseEraseCursor();

  getviewsettings(&viewinfo);
  setviewport(0,0,getmaxx(),getmaxy(),1);  
  
  strcpy(st, "");

  if (!get_string (st, "", MAXDIR, 0, 108)) 
   {
    setviewport(viewinfo.left,viewinfo.top,viewinfo.right,viewinfo.bottom,1);
	GrMouseUnEraseCursor();
	GrMouseDisplayCursor();
    return;
   } 

  GrMouseUnEraseCursor();
  GrMouseDisplayCursor();

  setviewport(viewinfo.left,viewinfo.top,viewinfo.right,viewinfo.bottom,1); 
   
  strcpy (dir_old, ptrs__drives_current_dir [i__current_drive].dir_name);
  strcpy (dir, ptrs__drives_current_dir [i__current_drive].dir_name);
  strcat (dir, st);

  if (dir [strlen (dir) - 1] != Slash)
    {
      strcat (dir, SSlash);
    }

#ifdef LINUX
  k=mkdir(dir, S_IWUSR);
#else
   k=mkdir(dir);
#endif
   if (k==-1) 
     {
          strcpy(st,u8"Błąd tworzenia katalogu ");
          strcat(st,dir);
          komunikat_str (st);
          key = Get_Legal_Key ("\033\015");
          komunikat (0);
     }
        else
         {
         /**/
	  strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, dir);
          if (get_file_list (TRUE) == FALSE)
          {
            strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, dir_old);
            /*sygnalizacja bledu*/
            return ;
          }
          Draw_ListBox (&listbox[LIST_FILES]);
          prot_file (TRUE) ;  
          return;  
         /**/
         }
}

void remove_dir (void)
{
  char st[MAXPATH];
  int k, key;
  char dir [MAXPATH], dir_old [MAXPATH];
  char *ptr;
 
 if (b__root==FALSE)
 {
  strcpy(st,u8"Delete current folder ? ");
  strcat(st, sz__current_path_file);
  strcat(st," ? Y/N/Esc :");
  komunikat_str (st);
  
  key = Get_Legal_Key ("YNyn\033");
  komunikat (0);
  if (key == ESC) return;
  if (key == 'T' || key == 't')
    {
      k=rmdir(sz__current_path_file);
      if (k==-1) 
        {
          strcpy(st,u8"Deleting folder error ");
          strcat(st,sz__current_path_file);
          komunikat_str (st);
          key = Get_Legal_Key ("\033\015");
          komunikat (0);
        }
        else
         {
         /**/
          strcpy (dir_old, ptrs__drives_current_dir [i__current_drive].dir_name);
          strcpy (dir, ptrs__drives_current_dir [i__current_drive].dir_name);
          dir [strlen (dir) - 1] = '\0';

		  if (NULL == (ptr = strrchr (dir, Slash)))
           {
    	     return ;
           }

            else
             {
              ptr [1] = '\0';
	      strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, dir);
             }
          if (get_file_list (TRUE) == FALSE)
          {
            strcpy (ptrs__drives_current_dir [i__current_drive].dir_name, dir_old);
            /*error*/
            return ;
          }
          Draw_ListBox (&listbox[LIST_FILES]);
          prot_file (TRUE) ;  
          return;  
         /**/
         }
    }
 }
}

void redraw_primary (void)
/*---------------------*/
{ int k;
  BOOL b_err=TRUE;

  zwolnienie_pamieci();
  k = czytaj_zbior(RYSUJ$1, b_err, TRUE, TRUE);
   if (k)
   {
     if (k == 1 && b_err == TRUE)
     {
      ErrList (101) ;
#ifdef LINUX
      sleep(1);
#else
      _sleep(1000);
#endif
     }
   }
}

static int proc_dlg(int n)
/*------------------------------*/
{
  int ret = Dlg_Ret_Val_Continue;

  switch (n)
  {
    case ID_OK:
	  save_spec();
	  ret = Dlg_Ret_Val_OK;
      break ;
    case ID_EDIT:
      ret = branch_edit () ;
      break ;
    case ID_EDIT_PROT :
      ret = branch_edit_prot () ;
      break ;
    case ID_FILE:
      ret = branch_file () ;
      break ;
    case ID_DRIVE:
      ret = branch_drive () ;
      break ;
    case ID_SPECIFICATION :
	list_spec ();
	break;
	case ID_EDIT_SPEC_NAME0:
	case ID_EDIT_SPEC_NAME1:
	case ID_EDIT_SPEC_NAME2:
	case ID_EDIT_SPEC_NAME3:
	case ID_EDIT_SPEC_NAME4:
	case ID_EDIT_SPEC_NAME5:
	case ID_EDIT_SPEC_NAME6:
	case ID_EDIT_SPEC_NAME7:
	case ID_EDIT_SPEC_NAME8:
	case ID_EDIT_SPEC_NAME9:
	case ID_EDIT_SPEC_NAME10:
	case ID_EDIT_SPEC_NAME11:
	case ID_EDIT_SPEC_NAME12:
	case ID_EDIT_SPEC_NAME13:
	case ID_EDIT_SPEC_NAME14:
	case ID_EDIT_SPEC_NAME15:
	edit_spec_name () ;
	break;
    case ID_EDIT_SPEC0 :
	case ID_EDIT_SPEC1:
	case ID_EDIT_SPEC2:
	case ID_EDIT_SPEC3:
	case ID_EDIT_SPEC4:
	case ID_EDIT_SPEC5:
	case ID_EDIT_SPEC6:
	case ID_EDIT_SPEC7:
	case ID_EDIT_SPEC8:
	case ID_EDIT_SPEC9:
	case ID_EDIT_SPEC10:
	case ID_EDIT_SPEC11:
	case ID_EDIT_SPEC12:
	case ID_EDIT_SPEC13:
	case ID_EDIT_SPEC14:
	case ID_EDIT_SPEC15:
	edit_spec ();
	break;
    case ID_SAVE_SPEC :
	save_spec ();
	break;
    case ID_READ_SPEC :
	read_spec ();
	break;
	case ID_READ_SPEC_INI:
	read_spec_ini();
	break;
    case ID_CANCEL:
      ret = Dlg_Ret_Val_Cancel;
      break;
    case ID_VIEW :
   if (view_block() != 5) read_spec ();
	perm_view_block=TRUE;
	break;
    case ID_REMOVE :
        remove_file ();
        break;
    case ID_MKDIR :
        make_dir ();
        break;      
    case ID_RMDIR :
        remove_dir ();
        break;      	
    case ID_FILE_RADIO :
      
      Check_Radio_Button (&load_file_dlg, ID_FILE_RADIO, ID_PROTOTYPE_RADIO,
	                 ID_FILE_RADIO, TRUE) ;
      break ;
    case ID_PROTOTYPE_RADIO :
      Check_Radio_Button (&load_file_dlg, ID_FILE_RADIO, ID_PROTOTYPE_RADIO,
			 ID_PROTOTYPE_RADIO, TRUE) ;
      break;
    default:
      ret = Dlg_Ret_Val_Continue;
      break;
  }
  return ret;
}

#ifndef FORWIN32

static int get_disk_type (char drive)   /*0 - A, 1 - B,...*/
//-----------------------------------
{
  union REGS InOut;

  InOut.h.ah=0x15;
  InOut.h.dl=drive;			/*numer dysku*/
  int86(0x13,&InOut,&InOut);
  if (InOut.x.cflag)
    return -1;
  else
   return InOut.h.ah;   /*0 - not exist , 1 - 360, 2 - 1.2,  3 - fixed*/
}

#else    //win32todo

static int get_disk_type (char drive)   /*0 - A, 1 - B,...*/
//-----------------------------------
{
  return 3;  //0 - not exist , 1 - 360, 2 - 1.2,  3 - fixed*/
}

#endif


static BOOL test_drive1 (int n)
/*------------------------*/
{
  BOOL b_ret ;

  b_ret = FALSE ;
  setdisk (n) ;
  if (n == getdisk ())
  {
    b_ret = TRUE ;
  }
  return b_ret ;
}


static BOOL test_drive (int n)
/*------------------------*/
{
  BOOL b_retval ;

  if (n == 0 || n == 1)
  {
    b_retval = (get_disk_type (n) != 0 && test_drive1 (n) == TRUE) ? TRUE : FALSE ;
  }
  else
  {
    b_retval = test_drive1 (n) ;
  }
  return b_retval ;
}



#ifdef FORWIN32
#ifndef LINUX

static BOOL check_if_disk_exists (int n)
{ int nn;
  long npoz;

  nn=n; //+1;
  npoz=(long)pow(2,nn);
  if ((npoz & _getdrives())>0)
  return TRUE;
    else return FALSE;

}


static BOOL test_drive_win32 (int n)
/*--------------------------------*/
{ BOOL b_retval ;
  
  if (n == 0 || n == 1)  //a:  lub   b:
  {
    b_retval = check_if_disk_exists (n);
  }
  else
  {
    b_retval = check_if_disk_exists (n);
  }
  
  return b_retval;
}

#endif
#endif


static BOOL test_dir (char * drive, char * dir, char *path_full)
/*---------------------------------------------------------*/
{
  char path_old [MAXPATH];
  char path [MAXPATH];
  BOOL ret;
  int k;
  int drive_no;

  ret = TRUE;
  if (drive [0] == '\0')
  {
    strcpy (drive, "X:");
    drive [0] = 'A' + i__current_drive ;
  }
  strupr2 (drive);
  if (drive [0] - 'A' >= i__drive_max)
  {
    return FALSE;
  }
  Current_Directory (0, path_old) ;
  strcpy (path, drive);
  k = 1;
  if (dir [0] == 0 || dir [0] != Slash)
  {
    drive_no = drive [0] - 'A';
    if (ptrs__drives_current_dir [drive_no].dir_name [0] == '\0')
    {
      if (NULL == Current_Directory (drive_no + 1, ptrs__drives_current_dir [drive_no].dir_name))
      {
	ErrList (46);
	return FALSE;
      }
    }
    k = 0;
    strcat (path, &ptrs__drives_current_dir [drive [0] - 'A'].dir_name [2]);
  }
  if (path [strlen (path) - 1] != Slash)
  {
    strcat (path, SSlash);
  }
  strupr2(path);
  strcat (path, &dir [k]);
  if (path [strlen (path) - 1] == Slash  && strlen (path) > 3)
  {
    path [strlen (path) - 1] = '\0';
  }
  if (0 == chdir (path))
  {
    Current_Directory (path [0] - 'A' + 1, path_full);
    ret = TRUE;
  }
  else
  {
    ret = FALSE;
  }
aaa:
  Del_Backslash (path_old) ;
  chdir(path_old);
  return ret;
}

static BOOL test_file_name0 (char *file_name)   //dla krotkich nazw
/*----------------------------------------*/
{
  int len,i, zn;

  file_name = strupr2 (file_name);
  len = strlen (file_name);
  if (len > 8)
  {
     file_name [8] = '\0';
     len = strlen (file_name);
  }
  for (i = 0; i < len; i++)
  {
    zn = file_name [i];
    if (! ( (zn >= 'A' && zn <= 'Z') ||
	 (zn >= '0' && zn <= '9') ||
	 zn == '_' ||  zn == '-'   ||   zn == '*'   ||  zn == '?') )
    {
      break;
    }
  }
  if ( i < len)
   return FALSE;
  else
   return TRUE;
}

static BOOL test_file_name (char *file_name)
/*----------------------------------------*/
{
  int len,i, zn;

  file_name = strupr2 (file_name);
  len = strlen (file_name);
  if (len > 255)
  {
     file_name [256] = '\0';
     len = strlen (file_name);
  }
  for (i = 0; i < len; i++)
  {
    zn = file_name [i];
    if (! (zn >= ' ') || zn == '<' || zn == '>' || zn == '|' || zn == ':' || zn == Slash || zn == BSlash )
    {
      break;
    }
  }
  if ( i < len)
   return FALSE;
  else
   return TRUE;
}


static void make_dir_lab (char * pathname, BOOL b_prot)
/*-----------------------------------------------------*/
{
  char * st ;
  int len ;        /*maxlen = 36*/
  char *ptrsz_lab ;

  ptrsz_lab = sz__directory_lab ;
  if (TRUE == b_prot)
  {
    ptrsz_lab = sz__directory_prot_lab ;
  }
  strcpy (ptrsz_lab, "Ścieżka: ") ;
  strcat (ptrsz_lab, pathname) ;
  len = strlen (ptrsz_lab) ;
  if (len > 36)
  {
    st = &ptrsz_lab [len - (36 - 16)] ;
    st = strchr (st, Slash) ;
    memmove ( &ptrsz_lab [15], st, strlen (st) + 1) ; /*z '\0' na koncu*/
    ptrsz_lab [12] = '.' ;
    ptrsz_lab [13] = '.' ;
    ptrsz_lab [14] = '.' ;
    ptrsz_lab [36] = '\0' ;
  }
}


static void make_directory_lab (char * pathname)
/*--------------------------------------*/
{
  BOOL b_prot ;

  b_prot = Get_Check_Button (&load_file_dlg, ID_PROTOTYPE_RADIO) ;
  make_dir_lab (pathname, b_prot) ;
}


static BOOL get_dir_names (void)
/*-----------------------------*/
{ char st[60];
  struct ffblk ffblk_;   
  int done;
  char pathname[MAXPATH]="";
  char drive[MAXDRIVE]="";
  char dir[MAXDIR]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;


  i__no_dir = 0 ;
  i__no_file = 0 ;

  if (ptrs__drives_current_dir [i__current_drive].dir_name [0] == '\0')
  {
    if (NULL == Current_Directory (i__current_drive + 1, ptrs__drives_current_dir [i__current_drive].dir_name))
    {
	  free_mouse();
      ErrList (46);
      return FALSE;
    }
  }

  free_mouse();

  strcpy (pathname, ptrs__drives_current_dir [i__current_drive].dir_name);
  if (pathname[strlen(pathname) -1 ] != Slash)
   {
    strcat (pathname, SSlash);
   }
  strupr2(pathname);
  make_directory_lab (pathname) ;
  flags=fnsplit(pathname, drive, dir, file, ext);
  b__root = TRUE;
  if(flags & DIRECTORY && strlen (dir) > 1)
  {
    b__root = FALSE;
  }
  if (b__root == FALSE)
  {
      strcpy (ptrs__table_file_name [i__no_file + i__no_dir++].file_name, SDirUp);
  }
  strcat (pathname, "*.*");
  done = findfirst (pathname, &ffblk_, FA_NORMAL | FA_ARCH | FA_RDONLY | FA_DIREC );
  while (!done)
  {
    if ( ffblk_.ff_attrib & FA_DIREC && ffblk_.ff_name [0] != '.' )
    {
      if (i__no_dir + i__no_file >= l__max_no_file_name -2)  /* -2 zapas*/
      {
       if (l__max_no_file_name == Max_No_Files) ErrList (173);
         else
          {
           sprintf(st,u8"Brak pamięci na nazwy plików (max. %ld)",l__max_no_file_name);
           komunikat_str(st);
          }
    	 return TRUE ; /*mimo braku pamieci*/
      }
      strcpy (ptrs__table_file_name [i__no_file + i__no_dir].file_name, strupr2 (ffblk_.ff_name));
      strcat (ptrs__table_file_name[i__no_file + i__no_dir].file_name, SSlash);
      i__no_dir++;
    }
     done = findnext(&ffblk_);
  }
  return TRUE;
}

static BOOL get_file_names (void)
/*-----------------------------*/
{ char st[60];
  struct ffblk /*_find_t*/ ffblk_;
  int done;
  char pathname[MAXPATH];


  i__no_file = 0;
  if (ptrs__drives_current_dir [i__current_drive].dir_name [0] == '\0')
  {
    if (NULL == Current_Directory (i__current_drive + 1, ptrs__drives_current_dir [i__current_drive].dir_name))
    {
	  free_mouse();
      ErrList (46);
      return FALSE;
    }
  }
  free_mouse();
  
  strcpy (pathname, ptrs__drives_current_dir [i__current_drive].dir_name);
  if (pathname[strlen(pathname) -1 ] != Slash)
   {
    strcat (pathname, SSlash);
   }
  strupr2(pathname);
  strcat (pathname, sz__current_mask);
    done = findfirst (pathname, &ffblk_, FA_NORMAL | FA_ARCH | FA_RDONLY);
  
  while (!done)
  {
    if (ffblk_.ff_attrib & FA_ARCH ||
	ffblk_.ff_attrib & FA_NORMAL ||
	ffblk_.ff_attrib & FA_RDONLY )
    {
      if (i__no_dir + i__no_file >= l__max_no_file_name -1)  /* -1 zapas*/
      {
        if (l__max_no_file_name == Max_No_Files) ErrList (173);
         else
          {
           sprintf(st,u8"Brak pamięci na nazwy plików (max. %ld)",l__max_no_file_name);
           komunikat_str(st);
          }
       return TRUE ; /*mimo braku pamieci*/
      }
      strcpy (ptrs__table_file_name[i__no_dir + i__no_file++].file_name, strlwr2 (ffblk_.ff_name));
    }
    done = findnext(&ffblk_);
  }
  return TRUE;
}

static int sort_function( const char **a, const char **b)
/*---------------------------------------------------*/
{
   return (strcmp (*a, *b)) ;
}


static void sort_by_name (void)
/*----------------------------*/
{
  qsort((void *)ptrptrsz__files_txt, i__no_dir,  sizeof(char *),
		(int(*)(const void *, const void *))sort_function) ;
  qsort((void *)(&ptrptrsz__files_txt [i__no_dir]), i__no_file,  sizeof(ptrptrsz__files_txt [0]),
		(int(*)(const void *, const void *))sort_function) ;
}


static BOOL get_file_list (BOOL b_dir)
/*-----------------------------------*/
{
  int i;

  if ( (b_dir == TRUE  && get_dir_names () == FALSE) ||
       (get_file_names () == FALSE))
  {
    return FALSE ;
  }
  for (i = 0; i < i__no_dir + i__no_file; i++)
  {
    ptrptrsz__files_txt [i] = ptrs__table_file_name [i].file_name;
  }
  sort_by_name () ;
  listbox [LIST_FILES].max = i__no_dir + i__no_file;
  listbox [LIST_FILES].maxw = LIST_FILE_MAX;
  listbox [LIST_FILES].poz = 0;		/*pozycja kursora */
  listbox [LIST_FILES].foff = 0;	/*pierwsza pozycja w okienku*/
  return TRUE ;
}



static void change_sel_in_combobox_drive (void)
/*--------------------------------------------*/
{
  int cur_index, i;

  for (i = 0; i < i__no_drives; i++)
  {
    if (i__current_drive == i__tab_drive_no [i])
    {
      cur_index = i;
      break;
    }
  }
  listbox [LIST_DRIVE].poz = cur_index % LIST_DRIVE_MAX; /*pozycja kursora */
  listbox [LIST_DRIVE].foff = cur_index - listbox [LIST_DRIVE].poz; /*pierwsza pozycja w okienku*/
  return ;
}


#ifndef LINUX
static BOOL get_drive_names (void)
/*-------------------------------*/
{
  int i ;
  int first_i;
  int curdisk ;
  char buf_drives[256];
  long retSize;
  char buf[32];


  i__no_drives = 0;
  


  curdisk = getdisk () ;
  i__drive_max= setdisk (curdisk);
  if (i__drive_max <= 0)
  {
    i__drive_max = Max_No_Drive;
  }
  if (i__drive_max > Max_No_Drive)
  {
    i__drive_max = Max_No_Drive;
  }
/*  i__drive_max = Max_No_Drive ;*/	/*VCAD TEST*/
  for (i = 0, i__no_drives = 0; i < i__drive_max; i++)
  {
    ptrs__drives_current_dir [i].dir_name [0] = '\0';
#ifdef FORWIN32
    if (test_drive_win32 (i) == TRUE)
#else
	if (test_drive (i) == TRUE)
#endif
    {
      i__tab_drive_no [i__no_drives++] = i;
    }
  }
  setdisk (curdisk) ; 	/*na wypadek gdyby test_drive cos zmienil*/

  for (i = 0; i < i__no_drives; i++)
  {
    char buf [3];

    strcpy (buf , "X:");
    buf [0] = 'A' + i__tab_drive_no [i];
    strcpy (s__table_drive_name [i].drive_name, buf);
    ptrsz__drive_txt [i] = s__table_drive_name [i].drive_name;
  }
  listbox [LIST_DRIVE].max = i__no_drives;
  listbox [LIST_DRIVE].maxw = LIST_DRIVE_MAX;
  listbox [LIST_DRIVE].txt = ptrsz__drive_txt;
  return TRUE;


}
#else
static BOOL get_drive_names (void)
{
    return TRUE;
}
#endif

static BOOL ini_drive_control (void)
/*-------------------------------*/
{
  int i, cur_index;

  cur_index = 0;
  for (i = 0; i < i__no_drives; i++)
  {
    if (i__current_drive == i__tab_drive_no [i])
    {
      cur_index = i;
      break;
    }
  }
  listbox [LIST_DRIVE].poz = cur_index % LIST_DRIVE_MAX; /*pozycja kursora */
  listbox [LIST_DRIVE].foff = cur_index - listbox [LIST_DRIVE].poz; /*pierwsza pozycja w okienku*/
  return TRUE;
}


static BOOL ini_dlg_control_prot (void)
/*-----------------------------------*/
{
  char dir [MAXDIR]="" ;
  char path_full [MAXPATH]="";
  char drive[MAXDRIVE]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;
  T_PTR_Prototype ptrs_prot ;
  char *ptr;

  Check_Radio_Button (&load_file_dlg, ID_FILE_RADIO, ID_PROTOTYPE_RADIO,
                     ID_FILE_RADIO, FALSE) ;
  button [BUT_PROT_RADIO].enable = 1 ;
  Set_Check_Button (&load_file_dlg, ID_PROTOTYPE_CHECK, TRUE) ;
  button [BUT_PROT_CHECK].enable = 1 ;
  if (i__type_load != INI_FILE)
  {
    Set_Check_Button (&load_file_dlg, ID_PROTOTYPE_CHECK, FALSE) ;
    button [BUT_PROT_CHECK].enable = 0 ;
    sz__edit_prot_name [0] = '\0' ;
    sz__directory_prot_lab [0] = '\0' ;
    button [BUT_PROT_RADIO].enable = 0 ;
    return TRUE ;
  }


  
  ptrs_prot = Get_Prototype () ;
  flags=fnsplit (Get_Prototype ()->sz_prot, drive, dir, file, ext);
  
  strcpy (sz__edit_prot_name, file) ;
  strcat (sz__edit_prot_name, ext) ;
  ptr=Current_Directory (0, sz__current_path_prot) ;

  if (sz__current_path_prot [strlen (sz__current_path_prot) - 1] != Slash)
  {
    strcat (sz__current_path_prot, SSlash);
  }

  
  strupr2(sz__current_path_prot);
  
  if (flags & DIRECTORY || flags & DRIVE)
  {
    if (TRUE == test_dir (drive, dir, path_full))
    {

	  if (path_full [strlen (path_full) - 1] != Slash)
      {
        strcat (path_full, SSlash);
      }

      strupr2(path_full);
      
      strcpy (sz__current_path_prot, path_full) ;
    }
  }
  
  make_dir_lab (sz__current_path_prot, TRUE) ;
  return TRUE ;
}

static BOOL ini_dlg_control_spec (void)
/*-----------------------------------*/
{
  int i;

  for ( i = 0 ; i < Max_No_Spec; i++)
  {
    s__table_spec_list [i].spec_list [0] = '\0';
    ptrsz__spec_txt [i] = s__table_spec_list [i].spec_list ;
  }
  if (i__type_load == SAVE_FILE_AS || i__type_load == EDIT_TABLE)
  {
	  memcpy(s__table_spec_name,
		  Get_Local_Spec(),
		  sizeof(T_spec_name) * Max_No_Spec);
	  for (i = 0; i < Max_No_Spec; i++)
	  {
		  edit[i*2].txt = s__table_spec_name[i].spec_name;
		  edit[i*2+1].txt = s__table_spec_name[i].spec_value;
	  }
		
   
  }
  else
  {
   ;
  }

  return TRUE;
}

static BOOL enable_dlg_control_spec (BOOL enable, BOOL b_redraw)
/*-------------------------------------------------------------*/
{
  int i;
  return TRUE;
}


static BOOL list_spec (void)
/*----------------------*/
{
  int poz ;
  char *lpsz_txt ;

  poz = listbox [LIST_SPECIFICATION].poz + listbox [LIST_SPECIFICATION].foff ;
  lpsz_txt = s__table_spec_name [poz].spec_value ;
  strcpy (edit_specification, lpsz_txt) ;
  ////Draw_Input_Line (&edit [EDIT_SPEC]) ;
  lpsz_txt = s__table_spec_name [poz].spec_name ;
  strcpy (edit_specification_name, lpsz_txt) ;

  return TRUE ;
}

static BOOL edit_spec_name (void)
/*------------------------------*/
{
  int poz ;
  char *lpsz_txt ;

  return TRUE ;
}

static BOOL edit_spec (void)
/*----------------------*/
{
  int poz ;
  char *lpsz_txt ;

  return TRUE ;
}


static BOOL save_spec (void)
/*----------------------*/
{

  save_local_spec();

  return TRUE ;
}


static BOOL read_spec_file (void)
/*------------------------------*/
{
  return Read_Spec_File (ptrsz__sheet_name, s__table_spec_name, TRUE) ;
}

static BOOL read_spec_block (void)
/*------------------------------*/
{
  char *lpsz_name, *lpsz_type;

  lpsz_name = s__table_spec_name [0].spec_value ;
  lpsz_type = s__table_spec_name [1].spec_value ;

  strcpy (s__table_spec_name [0].spec_name, _BLOCK_DESCRIPTION_);
  strcpy (s__table_spec_name [1].spec_name, _BLOCK_TYPE);

  return Read_Spec_Block (ptrsz__sheet_name,
			 lpsz_name, Max_Spec_Value,
			 lpsz_type, Max_Spec_Name,
			 TRUE) ;
}

static void spec_name_to_spec_list (int i_spec_no)
//----------------------------------------------
{
  sprintf (s__table_spec_list [i_spec_no].spec_list,
	   //"%-16s : %-39.39s",
	   u8"%-16s : %-64.64s",
            s__table_spec_name [i_spec_no].spec_name,
            s__table_spec_name [i_spec_no].spec_value) ;
}

static void spec_names_to_spec_lists (void)
//-------------------------------------------
{
  int i ;

  for (i = 0; i < i__specno ; i++)
  {
    spec_name_to_spec_list (i) ;
	Draw_Input_Line(&edit[2 * i]);
	Draw_Input_Line(&edit[2 * i + 1]);
  }
  
}

BOOL read_spec_ini(void)
/*----------------------*/
{
	int i;

	Ini_Spec_Param_copy();
	for (i = 0; i < 16; i++)
	{
		strcpy(s__table_spec_name[i].spec_name, s__table_spec_copy[i].spec_name);
		strcpy(s__table_spec_name[i].spec_value, s__table_spec_copy[i].spec_value);
	}
	spec_names_to_spec_lists();
	return TRUE;
}


static BOOL read_spec (void)
/*----------------------*/
{
 char sk[MAXPATH] = "";
 //if (Load_File_A(sk, EDIT_TABLE, FALSE) == FALSE)
 if (Load_File(sk, EDIT_TABLE, FALSE) == FALSE)
	{
		//    if (((bitmap_exist) && (bitmap_view)) || (dane_size > HIGH_SIZE))
		//     {
		//       cursor_on();
		//     }
		/*     cursor_on();  */
        Set_Pointer();
		return FALSE;
	}

  Set_Pointer();

 strcpy(ptrsz__sheet_name, sk);

  if (b__load_file == TRUE)
  {
    if (FALSE == read_spec_file ())
    {
      return FALSE ;
    }
  }
  else
  {
    if (FALSE == read_spec_block ())
    {
      return FALSE ;
    }
  }
  spec_names_to_spec_lists () ;
  return TRUE;
}

static void save_local_spec (void)
//--------------------------------
{
  memcpy (Get_Local_Spec (),
            s__table_spec_name,
            sizeof(T_spec_name) * Max_No_Spec) ;
  Change = TRUE;
}


BOOL Load_File_A (char * f_name, int type, BOOL cur)
{  int ret, ret1, k;
   char *ext__;
   char *ext_;
   //char *image_mask;
   t_dir_name drives_current_dir_ [Max_No_Drive];  /*aktualne sciezki kolejnych dyskow*/
   char st1[30];
   double max_text_len;
   int disk0;
   char current_path0[MAXPATH];
   char *path0;
   BOOL path_changed;
   int i;
   char ptrs__drives_current_dir_win[255];
   int mouse_x_, mouse_y_;
   int key;
   char *ptr_slash;
   BOOL IN_OUT;

/////////////////////////////////

  disk0 = getdisk();
  path0=Current_Directory (disk0+1, current_path0) ;

  was_block=0;
  perm_view_block=FALSE;

  i__type_load = type;
  ptrs__drives_current_dir = drives_current_dir_;
  get_drive_names ();
  if (b_first_open == TRUE)
  {
     b_first_open = FALSE;
	 default_drive_cat = default_drive_bl = default_drive_file = default_drive_dxf = default_drive_dwg = default_drive_pcx = default_drive_pro = default_drive_geo = default_drive_cfg= default_drive_txt= getdisk ();
     Current_Directory (default_drive_file + 1, default_path_file);

#ifndef LINUX
     strcat(default_path_file,SSlash);
#endif
	 strcpy(default_path_dxf, default_path_file);
	 strcpy(default_path_dwg, default_path_file);
	 strcpy(default_path_cat, default_path_file);
	 strcpy(default_path_cat0, default_path_file);
     strcpy (default_path_bl, default_path_file);
     strcpy (default_path_pcx, default_path_file);
     strcpy (default_path_pro, default_path_file);
     strcpy (default_path_geo, default_path_file);
     strcpy (default_path_cfg, default_path_file);
      strcpy (default_path_out, default_path_file);
     strcpy (default_path_txt, default_path_file);
     Get_Private_Profile_Strings ((T_Fstring)IC_DIRECTORY, get_config_param);

  }
   else
    {
#ifndef LINUX
	  AddSlash (default_path_file) ;
	  AddSlash(default_path_dxf);
	  AddSlash(default_path_dwg);
      AddSlash (default_path_bl) ;
	  AddSlash(default_path_cat);
	  AddSlash(default_path_cat0);
      AddSlash (default_path_pcx) ;
      AddSlash (default_path_pro) ;
      AddSlash (default_path_geo) ;
      AddSlash (default_path_cfg) ;
      AddSlash (default_path_out) ;
      AddSlash (default_path_txt) ;
#endif

	}


    switch (type) {
        case INI_DXF:
        case INI_BLOK_DXF:
        case INI_DWG:
        case INI_BLOK_DWG:
        case READ_MAK:
        case INI_FILE:
        case INI_BLOK:
        case OPEN_CATALOG:
        case INI_PCX:
        case INI_PCX2BMP:
        case INI_PRO:
        case INI_GEO:
        case ODCZYT_CFG:
        case ODCZYT_PLT_CFG:
        case ODCZYT_TXT:
        case ODCZYT_PXYZ:
        case INI_TTF_FONT:
        case INI_OTF_FONT:
        case EDIT_TABLE:
            IN_OUT = 0;  //in
        break;
        default:
        IN_OUT = 1;
        break;
    }

switch (type)
  {
    case INI_DXF:
    case SAVE_DXF:
    case INI_BLOK_DXF:

      b__load_file = TRUE ;
      ext__ = EXT_DXF;
	  ext_ = EXT__DXF;

	  strcpy (ptrs__drives_current_dir_win, default_path_dxf);
      break;
	case INI_DWG:
	case SAVE_DWG:
	case INI_BLOK_DWG:

		b__load_file = TRUE;
		ext__ = EXT_DWG;
		ext_ = EXT__DWG;

		strcpy(ptrs__drives_current_dir_win, default_path_dwg);
		break;
    case WRITE_MAK:
    case READ_MAK:

      b__load_file = TRUE ;
      ext__ = EXT_MAK;
	  ext_ = EXT__MAK;

	  strcpy (ptrs__drives_current_dir_win, default_path_mak);
      break;  
    case INI_FILE:
    case  SAVE_FILE_AS:
    case CONVERT_OLD_FILE:
    case EDIT_TABLE :

      b__load_file = TRUE ;
      ext__ = EXT_FILE;
	  ext_ = EXT__FILE;

	  strcpy (ptrs__drives_current_dir_win, default_path_file);
      break;
    
    case SAVE_BLOK: 
    case CONVERT_OLD_BLOCK :

      b__load_file = FALSE ;
      ext__ = EXT_BLOK;
	  ext_ = EXT__BLOK;

	  strcpy (ptrs__drives_current_dir_win, default_path_bl);
      break;   
    case INI_BLOK: 

      b__load_file = FALSE ;
      ext__ = EXT_BLOK;
	  ext_ = EXT__BLOK;

	  strcpy (ptrs__drives_current_dir_win, default_path_bl);
    break;
	case OPEN_CATALOG:

		b__load_file = FALSE;
		ext__ = EXT_BLOK;
		ext_ = EXT__BLOK;

        if (strlen(f_name) == 0) strcpy(ptrs__drives_current_dir_win, default_path_cat);
        else strcpy(ptrs__drives_current_dir_win, f_name);
		break;
    case INI_PCX:
		ext__ = EXT_IMAGE;
		ext_ = EXT__IMAGE;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
    case INI_PCX2BMP:
          ext__ = EXT_IMAGE2BMP;
          ext_ = EXT__IMAGE2BMP;
          b__load_file = FALSE;
          strcpy(ptrs__drives_current_dir_win, default_path_pcx);
    break;
    case SAVE_PCX:
		ext__ = EXT_PCX;
		ext_ = EXT__PCX;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_BMP:
		ext__ = EXT_BMP;
		ext_ = EXT__BMP;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_PNG:
		ext__ = EXT_PNG;
		ext_ = EXT__PNG;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_JPEG:
		ext__ = EXT_JPEG;
		ext_ = EXT__JPEG;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_TIFF:
		ext__ = EXT_TIFF;
		ext_ = EXT__TIFF;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_EPS:
		ext__ = EXT_EPS;
		ext_ = EXT__EPS;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_PDF:
		ext__ = EXT_PDF;
		ext_ = EXT__PDF; 
		b__load_file = FALSE ;
		strcpy (ptrs__drives_current_dir_win, default_path_pcx);
	break;
    case INI_PRO:
    case ZAPIS_PRO:

      b__load_file = FALSE ;
      ext__ = EXT_PRO;
	  ext_ = EXT__PRO;

	  strcpy (ptrs__drives_current_dir_win, default_path_pro);
    break;
    case INI_GEO:

      b__load_file = FALSE ;
      ext__ = EXT_GEO;
	  ext_ = EXT__GEO;

	  strcpy (ptrs__drives_current_dir_win, default_path_geo);
    break;
    case ZAPIS_CFG:
    case ODCZYT_CFG:

      b__load_file = TRUE ;
      ext__ = EXT_CFG;
	  ext_ = EXT__CFG;

	  strcpy (ptrs__drives_current_dir_win, default_path_cfg);
      break;
	case ZAPIS_PLT_CFG:
	case ODCZYT_PLT_CFG:

		b__load_file = TRUE;
		ext__ = EXT_PLT_CFG;
		ext_ = EXT__PLT_CFG;

		strcpy(ptrs__drives_current_dir_win, default_path_cfg);
		break;
    case ZAPIS_XYZ:

      b__load_file = TRUE ;
      ext__ = EXT_XYZ;
	  ext_ = EXT__XYZ;

	  strcpy (ptrs__drives_current_dir_win, default_path_txt);
      break; 
    case ZAPIS_TXT:
    case ODCZYT_TXT:

      b__load_file = TRUE ;
      ext__ = EXT_TXT;
	  ext_ = EXT__TXT;

	  strcpy (ptrs__drives_current_dir_win, default_path_txt);
      break;
    case ODCZYT_PXYZ:

      b__load_file = TRUE ;
      ext__ = EXT_PXYZ;
	  ext_ = EXT__PXYZ;

	  strcpy (ptrs__drives_current_dir_win, default_path_txt);
      break;
	case INI_TTF_FONT:
		b__load_file = FALSE;
		ext__ = EXT_FONT;
		ext_ = EXT__FONT;
		
		strcpy(&ptrs__drives_current_dir_win, winfont);
		break;
      case INI_OTF_FONT:
          b__load_file = FALSE;
          ext__ = EXT_FONT;
          ext_ = EXT__FONT;

          strcpy(&ptrs__drives_current_dir_win, otffont);
          break;
      case ZAPIS_OUT:
          b__load_file = FALSE ;
          ext__ = EXT_OUT;
          ext_ = EXT__OUT;
          strcpy (ptrs__drives_current_dir_win, default_path_out);
          break;
    default :
      ext__ = "";
      break;
  }

  strcpy (sz__current_path_file, ptrs__drives_current_dir_win) ;

  switch (type)
  { case INI_FILE:
    case  SAVE_FILE_AS:
    case CONVERT_OLD_FILE:
	case EDIT_TABLE : 
					if (strlen(old_mask_file)==0)
					{        
                         strcpy (&sz__current_mask, ext__);
                       }
                         else strcpy (sz__current_mask, old_mask_file); 
             break;
    case INI_BLOK:
    case SAVE_BLOK:
	case OPEN_CATALOG:
    case CONVERT_OLD_BLOCK : if (strlen(old_mask_blok)==0)
                               {
                               
                                strcpy (sz__current_mask, ext__);
                               }
                                 else strcpy (sz__current_mask, old_mask_blok);
                       
             break;   
    case INI_PCX:
    case INI_PCX2BMP:
    case SAVE_PCX:  
	case SAVE_BMP:
	case SAVE_PNG:
	case SAVE_JPEG:
	case SAVE_TIFF:
	case SAVE_EPS:
	case SAVE_PDF:
							if (strlen(old_mask_pcx)==0)
                               {
                               
                                strcpy (sz__current_mask, ext__);
								
                               }
                                 else strcpy (sz__current_mask, old_mask_pcx);
                       
             break;
    case INI_PRO:
    case ZAPIS_PRO:
                 if (strlen(old_mask_pro)==0)
                               {
                               
                                strcpy (sz__current_mask, ext__);
                               }
                                 else strcpy (sz__current_mask, old_mask_pro);
                       
             break;
    case INI_GEO: if (strlen(old_mask_geo)==0)
                               {
                                
                                strcpy (sz__current_mask, ext__);
                               }
                                 else strcpy (sz__current_mask, old_mask_geo);
                       
             break;
	case INI_TTF_FONT:
    case INI_OTF_FONT:
		   strcpy(sz__current_mask, ext__);
		break;
    default: 
             strcpy (sz__current_mask, ext__);
             break;
  }
  load_file_dlg.txt = dlg_name [type];
  ptrsz__sheet_name = f_name;

  mouse_x_=mouse_x;
  mouse_y_=mouse_y;

  if (type==OPEN_CATALOG)
	  ret = My_GetOpenFolder(f_name, sz__current_path_file, default_path_cat0, sz__current_mask, dlg_name[type]); // , HEIGHT, WIDTH, Desktop_Font_File);
  else
      ret = My_GetOpenFileName(f_name, sz__current_path_file, sz__current_mask, dlg_name [type], IN_OUT, cur);
  if (cur)
      position_mouse(mouse_x_, mouse_y_);

  if (ret == TRUE)
  {
	//tutaj trzeba ponownie wyluskac sciezke ptrs__drives_current_dir_win z nazwy pliku f_name
	if (strlen(f_name)>0)  strcpy(ptrs__drives_current_dir_win, f_name);

#ifndef LINUX
	ptr_slash = strrchr(ptrs__drives_current_dir_win, '\\');
#else
      ptr_slash = strrchr(ptrs__drives_current_dir_win, '/');
#endif

	if (ptr_slash) *ptr_slash = '\0';

	if (type!=OPEN_CATALOG)
        File_Ext (f_name, ext_) ;

    switch (type)
    {
      case INI_FILE:
      case  SAVE_FILE_AS:
      case CONVERT_OLD_FILE:
      case EDIT_TABLE :
	  strcpy (default_path_file, ptrs__drives_current_dir_win);
	break;
	  case INI_DXF:
	  case INI_BLOK_DXF:
	  case SAVE_DXF:
		  strcpy(default_path_dxf, ptrs__drives_current_dir_win);
		  break;
	  case INI_DWG:
	  case INI_BLOK_DWG:
	  case SAVE_DWG:
		  strcpy(default_path_dwg, ptrs__drives_current_dir_win);
		  break;
      case INI_BLOK:
      case SAVE_BLOK:
      case CONVERT_OLD_BLOCK :
	  strcpy (default_path_bl, ptrs__drives_current_dir_win);
      break;
	  case OPEN_CATALOG:
		  break;
      case INI_PCX:
      case INI_PCX2BMP:
      case SAVE_PCX:
	  case SAVE_BMP:
	  case SAVE_PNG:
	  case SAVE_JPEG:
	  case SAVE_TIFF:
	  case SAVE_EPS:
	  case SAVE_PDF:
	  strcpy (default_path_pcx, ptrs__drives_current_dir_win);
      break;
      case INI_PRO:
      case ZAPIS_PRO:
	  strcpy (default_path_pro, ptrs__drives_current_dir_win);
      break;
      case INI_GEO:
	  strcpy (default_path_geo, ptrs__drives_current_dir_win);
      break;
      case ZAPIS_CFG:
      case ODCZYT_CFG:
	  strcpy (default_path_cfg, ptrs__drives_current_dir_win);
      break;
      case ZAPIS_XYZ:
      case ZAPIS_TXT:
      case ODCZYT_TXT:
      case ODCZYT_PXYZ:
	  strcpy (default_path_txt, ptrs__drives_current_dir_win);
      break;
        case ZAPIS_OUT:
            strcpy (default_path_out, ptrs__drives_current_dir_win);
      default :
	break;
    }
   }

  setdisk (disk0) ;     

  chdir (current_path0) ;  


  if (ret == TRUE && (type == CONVERT_OLD_FILE || type == CONVERT_OLD_BLOCK))
  {
    return ret ;
  }
  if (ret == TRUE && type == INI_FILE)
  {
    Image_Free () ;
  }
  
   return ret;

}


BOOL Edit_Local_Spec(int type, BOOL cur)
/*-------------------------------------*/
{
	int k;
	t_dir_name drives_current_dir_[Max_No_Drive];  /*aktualne sciezki kolejnych dyskow*/
	DLG_COLOR color_dlg;
	BOOL ret;
	char *ext;
	char *ext_;
	char st1[30];
	double max_text_len;
	int disk0;
	char current_path0[MAXPATH];
	char *path0;
	double wsp_x, wsp_y;
    static int curr_h, curr_v;

	was_block = 0;
	perm_view_block = FALSE;
	b__load_file = TRUE;

	load_file_dlg.txt = dlg_name[type];

	i__type_load = type;

	ini_dlg_control_spec();

	Get_Default_Color_Dlg(&color_dlg);

	wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
	wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

	images_t[0].x1 = (XpOK + 30) - (32 / wsp_x);

	images_t[0].y1 = (YpSPEC_READ + 35 + 30 + YpCANCEL) / 2  - (32 / wsp_y);

    Save_Update_flex(0, &curr_h, &curr_v);
	ret = Dialog(&load_file_dlg, &color_dlg, proc_dlg, bMouse);
    Save_Update_flex(1, &curr_h, &curr_v);

	ret = (ret == Dlg_Ret_Val_OK) ? TRUE : FALSE;

	return ret;

}


BOOL Load_File_C (char * f_name, int type, BOOL cur)
{  int ret;
   char *ext;
   char *ext_;
   t_dir_name drives_current_dir_ [Max_No_Drive];  /*aktualne sciezki kolejnych dyskow*/
   char st1[30];
   double max_text_len;
   int disk0;
   char current_path0[MAXPATH];
   char *path0;
   BOOL path_changed;
   int i;
   char ptrs__drives_current_dir_win[255];
   static char fname[80*6] = EMPTY_STRING; /* 80 chars * max UTF8 char width */

/////////////////////////////////

  disk0 = getdisk();
  path0=Current_Directory (disk0+1, current_path0) ;

  was_block=0;
  perm_view_block=FALSE;

  i__type_load = type;
  ptrs__drives_current_dir = drives_current_dir_;
  get_drive_names ();
  if (b_first_open == TRUE)
  {
     b_first_open = FALSE;
	 default_drive_cat = default_drive_bl = default_drive_file = default_drive_pcx = default_drive_pro = default_drive_geo = default_drive_cfg= default_drive_txt= getdisk ();
     Current_Directory (default_drive_file + 1, default_path_file);

     strcat(default_path_file,SSlash);

     
	 strcpy(default_path_cat, default_path_file);
     strcpy (default_path_bl, default_path_file);
     strcpy (default_path_pcx, default_path_file);
     strcpy (default_path_pro, default_path_file);
     strcpy (default_path_geo, default_path_file);
     strcpy (default_path_cfg, default_path_file);
      strcpy (default_path_out, default_path_file);
     strcpy (default_path_txt, default_path_file);
     Get_Private_Profile_Strings ((T_Fstring)IC_DIRECTORY, get_config_param);
  }
   else
    {
#ifdef FORWIN32
	  AddSlash (default_path_file) ;
	  AddSlash(default_path_cat);
      AddSlash (default_path_bl) ;
      AddSlash (default_path_pcx) ;
      AddSlash (default_path_pro) ;
      AddSlash (default_path_geo) ;
      AddSlash (default_path_cfg) ;
      AddSlash (default_path_out) ;
      AddSlash (default_path_txt) ;
#else
      AddBackslash (default_path_file) ;
	  AddBackslash(default_path_cat);
      AddBackslash (default_path_bl) ;
      AddBackslash (default_path_pcx) ;
      AddBackslash (default_path_pro) ;
      AddBackslash (default_path_geo) ;
      AddBackslash (default_path_cfg) ;
      AddBackslash (default_path_out) ;
      AddBackslash (default_path_txt) ;
#endif
	}


switch (type)
  {
    case INI_DXF:
    case SAVE_DXF:
	case INI_BLOK_DXF:
      b__load_file = TRUE ;
      ext = EXT__DXF;
	  strcpy (ptrs__drives_current_dir_win, default_path_dxf);
      break;
	case INI_DWG:
	case SAVE_DWG:
	case INI_BLOK_DWG:
		b__load_file = TRUE;
		ext = EXT__DWG;
		strcpy(ptrs__drives_current_dir_win, default_path_dwg);
		break;
    case WRITE_MAK:
    case READ_MAK:
      b__load_file = TRUE ;
      ext = EXT__MAK;
	  strcpy (ptrs__drives_current_dir_win, default_path_mak);
      break;  
    case INI_FILE:
    case  SAVE_FILE_AS:
    case CONVERT_OLD_FILE:
    case EDIT_TABLE :
      b__load_file = TRUE ;
      ext = EXT__FILE;
	  strcpy (ptrs__drives_current_dir_win, default_path_file);
      break;
    
    case SAVE_BLOK: 
    case CONVERT_OLD_BLOCK :
      b__load_file = FALSE ;
      ext = EXT__BLOK;
	  strcpy (ptrs__drives_current_dir_win, default_path_bl);
      break;   
    case INI_BLOK:
      b__load_file = FALSE ;
      ext = EXT__BLOK;
	  strcpy (ptrs__drives_current_dir_win, default_path_bl);
    break;
	case INI_PCX:
		ext = EXT_IMAGE;
		ext_ = EXT__IMAGE;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
    case INI_PCX2BMP:
          ext = EXT_IMAGE2BMP;
          ext_ = EXT__IMAGE2BMP;
          b__load_file = FALSE;
          strcpy(ptrs__drives_current_dir_win, default_path_pcx);
    break;
	case SAVE_PCX:
		ext = EXT_PCX;
		ext_ = EXT__PCX;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_BMP:
		ext = EXT_BMP;
		ext_ = EXT__BMP;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_PNG:
		ext = EXT_PNG;
		ext_ = EXT__PNG;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_JPEG:
		ext = EXT_JPEG;
		ext_ = EXT__JPEG;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_TIFF:
		ext = EXT_TIFF;
		ext_ = EXT__TIFF;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_EPS:
		ext = EXT_EPS;
		ext_ = EXT__EPS;
		b__load_file = FALSE;
		strcpy(ptrs__drives_current_dir_win, default_path_pcx);
	break;
	case SAVE_PDF:
		ext = EXT_PDF;
		ext_ = EXT__PDF;
        b__load_file = FALSE ;
        strcpy (ptrs__drives_current_dir_win, default_path_pcx);
    break;
    case INI_PRO:
    case ZAPIS_PRO:
      b__load_file = FALSE ;
      ext = EXT__PRO;
	  strcpy (ptrs__drives_current_dir_win, default_path_pro);
    break;
    case INI_GEO:
      b__load_file = FALSE ;
      ext = EXT__GEO;
	  strcpy (ptrs__drives_current_dir_win, default_path_geo);
    break;
    case ZAPIS_CFG:
    case ODCZYT_CFG:
      b__load_file = TRUE ;
      ext = EXT__CFG;
	  strcpy (ptrs__drives_current_dir_win, default_path_cfg);
      break;
    case ZAPIS_XYZ:
      b__load_file = TRUE ;
      ext = EXT__XYZ;
	  strcpy (ptrs__drives_current_dir_win, default_path_txt);
      break; 
    case ZAPIS_TXT:
    case ODCZYT_TXT:
      b__load_file = TRUE ;
      ext = EXT__TXT;
	  strcpy (ptrs__drives_current_dir_win, default_path_txt);
      break;
    case ODCZYT_PXYZ:
      b__load_file = TRUE ;
      ext = EXT__PXYZ;
	  strcpy (ptrs__drives_current_dir_win, default_path_txt);
      break;
      case ZAPIS_OUT:
          b__load_file = FALSE ;
          ext = EXT__OUT;
          strcpy (ptrs__drives_current_dir_win, default_path_out);
          break;
    default :
      ext = "";
      break;
  }

  strcpy (sz__current_path_file, ptrs__drives_current_dir_win) ;

  switch (type)
  { case INI_FILE:
    case  SAVE_FILE_AS:
    case CONVERT_OLD_FILE:
    case EDIT_TABLE : if (strlen(old_mask_file)==0)
                       {
                         strcpy (sz__current_mask, "");
                         strcat (sz__current_mask, ext);
                       }
                         else strcpy (sz__current_mask, old_mask_file);
                       
             break;
    case INI_BLOK:
    case SAVE_BLOK:
    case CONVERT_OLD_BLOCK : if (strlen(old_mask_blok)==0)
                               {
                                strcpy (sz__current_mask, "");
                                strcat (sz__current_mask, ext);
                               }
                                 else strcpy (sz__current_mask, old_mask_blok);
                       
             break;   
    case INI_PCX:
    case INI_PCX2BMP:
    case SAVE_PCX:     
	case SAVE_BMP:
	case SAVE_PNG:
	case SAVE_JPEG:
	case SAVE_TIFF:
	case SAVE_EPS:
	case SAVE_PDF:
							if (strlen(old_mask_pcx)==0)
                               {
                                strcpy (sz__current_mask, "");
                                strcat (sz__current_mask, ext);
                               }
                                 else strcpy (sz__current_mask, old_mask_pcx);
                       
             break;
    case INI_PRO:
    case ZAPIS_PRO:
                 if (strlen(old_mask_pro)==0)
                               {
                                strcpy (sz__current_mask, "");
                                strcat (sz__current_mask, ext);
                               }
                                 else strcpy (sz__current_mask, old_mask_pro);
                       
             break;
    case INI_GEO: if (strlen(old_mask_geo)==0)
                               {
                                strcpy (sz__current_mask, "");
                                strcat (sz__current_mask, ext);
                               }
                                 else strcpy (sz__current_mask, old_mask_geo);
                       
             break;           
    default: strcpy (sz__current_mask, "");
             strcat (sz__current_mask, ext);
             break;
  }
  load_file_dlg.txt = dlg_name [type];
  ptrsz__sheet_name = f_name;

  strcpy(fname,sz__current_path_file);

  gui_bg_color=15;
  gui_fg_color=0;

  ret = (file_select_ex(dlg_name[type], fname, sz__current_mask, sizeof(fname), 640, 480));

  strcpy(f_name, fname);


  if (ret == TRUE)
  {
	//tutaj trzeba ponownie wyluskac sciezke ptrs__drives_current_dir_win z nazwy pliku f_name
	strcpy(ptrs__drives_current_dir_win, f_name);
	i=strlen(ptrs__drives_current_dir_win);
	path_changed=FALSE;
	while ((i>=0) && (path_changed==FALSE))
	{
		if (ptrs__drives_current_dir_win[i]=='\\')
		{
           ptrs__drives_current_dir_win[i]='\0';
           path_changed=TRUE;
		}

		i--;
	}

    File_Ext (f_name, ext_) ;
    switch (type)
    {
      case INI_FILE:
      case  SAVE_FILE_AS:
      case CONVERT_OLD_FILE:
      case EDIT_TABLE :
	  strcpy (default_path_file, ptrs__drives_current_dir_win);
	break;
	  case INI_DXF:
	  case INI_BLOK_DXF:
	  case SAVE_DXF:
		  strcpy(default_path_dxf, ptrs__drives_current_dir_win);
		  break;
	  case INI_DWG:
	  case INI_BLOK_DWG:
	  case SAVE_DWG:
		  strcpy(default_path_dwg, ptrs__drives_current_dir_win);
		  break;
      case INI_BLOK:
      case SAVE_BLOK:
      case CONVERT_OLD_BLOCK :
	  strcpy (default_path_bl, ptrs__drives_current_dir_win);
      break;
      case INI_PCX:
      case INI_PCX2BMP:
      case SAVE_PCX:
	  case SAVE_BMP:
	  case SAVE_PNG:
	  case SAVE_JPEG:
	  case SAVE_TIFF:
	  case SAVE_EPS:
	  case SAVE_PDF:
	  strcpy (default_path_pcx, ptrs__drives_current_dir_win);
      break;
      case INI_PRO:
      case ZAPIS_PRO:
	  strcpy (default_path_pro, ptrs__drives_current_dir_win);
      break;
      case INI_GEO:
	  strcpy (default_path_geo, ptrs__drives_current_dir_win);
      break;
      case ZAPIS_CFG:
      case ODCZYT_CFG:
	  strcpy (default_path_cfg, ptrs__drives_current_dir_win);
      break;
      case ZAPIS_XYZ:
      case ZAPIS_TXT:
      case ODCZYT_TXT:
      case ODCZYT_PXYZ:
	  strcpy (default_path_txt, ptrs__drives_current_dir_win);
      break;
        case ZAPIS_OUT:
            strcpy (default_path_out, ptrs__drives_current_dir_win);
      default :
	break;
    }
   }

  setdisk (disk0) ;     

  chdir (current_path0) ;  


  if (ret == TRUE && (type == CONVERT_OLD_FILE || type == CONVERT_OLD_BLOCK))
  {
    return ret ;
  }
  if (ret == TRUE && type == INI_FILE)
  {
    Image_Free () ;
  }

   return ret;

}


BOOL Load_File (char * f_name, int type, BOOL cur)
{ BOOL ret;
  preview_blocked = TRUE;
  sw_callback_locked_proc();
  ret=Load_File_A(f_name, type, cur);
  preview_blocked = FALSE;
  return ret;
}

#undef __LOADF__