
typedef struct {
	int x1, y1, dx, dy;
	int ligth, dark;
} DARK_LIGHT_LINE;


typedef struct {
	int x, y;
	int dx, dy;        /*polorzenie i wielkosc tekstu*/
	int ink;	             /*kolor pisaka*/
	int paper;	             /*kolor tla*/
	char* txt;		         /*adres lancucha tresci pozycji*/
	int shadow;
	int s_ink;
} LABEL;

typedef struct {
	int x, y, dx, dy;        /*polorzenie i wielkosc box'a*/
	int ink, paper;     	 /*kolory */
	char* txt;		         /*adres lancucha tresci pozycji*/
	int id;
	void(*process)(void);
} GROUP_BOX;

typedef struct {
	int x, y, dx, dy;        /*polorzenie i wielkosc okienka*/
	int border;
	int ink, paper;
	int ink_ini, paper_ini;     	 /*kolory */
	int MaxLen;
	int width;
	unsigned enable;		/*edycja nie aktywna*/
	char* txt;		         /*adres lancucha tresci pozycji*/
	int id;
	char* legal;
} INPUTLINE;



typedef struct {
	int x, y, dx, dy;      /*polorzenie i wielkosc okienka*/
	int max;		/*liczba pozycji listbox'a*/
	int maxw;		/*rozmiar okna ( 0 dla nie scrolowanych )*/
    int maxw0;
	char flags;		/*rozne opcje*/
	int poz;		/*pozycja kursora */
	int foff;		/*pierwsza pozycja w okienku*/
	int max_text_len;
	int border;
	int ink, paper;
	unsigned enable;	/*edycja nie aktywna*/
	void* back;	/*zawartosc tla*/
	char** txt;  	/*tablica adresow tekstow w listbox*/
	int id;
	int wartosc;
} LISTBOX;

typedef struct {
	int x, y, dx, dy;      /*polorzenie i wielkosc okienka*/
	LISTBOX* listbox;
} COMBOBOX;


typedef struct {
	int x1, y1, x2, y2;   /*polorzenie i wielkosc obrazu*/
	int iconno;
	char* txt;
} IMAGE;

typedef struct {
	int x, y, dx, dy;        /*polorzenie i wielkosc przycisku*/
	int border, ink, paper;	 /*kolory */
	char* txt;
	unsigned check : 1;            /*stan przycisku (wcisniety, zwolniony)*/
	unsigned type : 3;		/*typ przycisku*/
	unsigned char name2;		/*tekst na batonie w 2 wierszach or bitmap if > 1*/
	unsigned enable : 1;		/*baton nie aktywny*/
	unsigned flags : 10;       /* hidden = 1*/
	int id;
    int reference;
    long adr;
} BUTTON;


//typedef int(*MYDIALOG_PROC)(int msg, SLIDER_ *d, int c);

typedef int(*MYDIALOG_PROC)(int msg, void *d, int c);

typedef struct {
    MYDIALOG_PROC proc;
    int x, y, w, h;       // position and size of the object
    int fg, bg;           // foreground and background colors
    int key;              // keyboard shortcut (ASCII code)
    int flags;            // flags about the object state
    int d1;               // max possible value  (minimum value is always 0)
    int d2;               // maximum of presented data
    void *dp, *dp2, *dp3;
	void* slider;
} SLIDER;

typedef struct TDIALOG {
	int x, y;              	/*punkt poczatkowy w jednostkach logicznych*/
	int dx, dy;           		/*rozmiar pola roboczego */
	int border, paper;	        /*kolory*/
	int caption, ink_caption;	/*kolory tytulu*/
	unsigned flags;	        /*rozne opcje*/
	int pozx, pozy;		        /*pozycja kursora*/
	char* txt;			/*tytul*/
	int SizeLinT;
	DARK_LIGHT_LINE(*Lines)[];
	int SizeLabT;
	LABEL(*Labels)[];     /*tablica opisow etykiet w oknie*/
	int SizeGroupT;
	GROUP_BOX(*Groups)[];     /*tablica opisow etykiet w oknie*/
	int SizeInLinT;
	INPUTLINE(*InputLines)[]; /*tablica opisow edycji stringow  w oknie*/
	int SizeImageT;
	IMAGE(*Images)[];  /*tablica opisow obrazow w oknie*/
	int SizeButtonT;
	BUTTON(*Buttons)[]; /*tablica opisow przyciskow */
	int SizeListBoxT;
	LISTBOX(*ListBoxes)[]; /*tablica opisow listbox */
	int SizeComboBoxT;
	COMBOBOX(*ComboBoxes)[]; /*tablica opisow combobox */
    int SizeSliderT;
    SLIDER(*Sliders)[]; /*sliders */
	void(*process)(void);
	void* back;
	int xb;
	int yb;
    void* background;
} TDIALOG;

typedef struct
 {
   int dlg_border;
   int dlg_paper;
   int dlg_caption;
   int dlg_ink_caption;
   int dlg_label_ink;
   int dlg_label_paper;
   int dlg_lin_ligth;
   int dlg_lin_dark;
   int dlg_group_ink;
   int dlg_group_paper;
   int dlg_in_line_border;
   int dlg_in_line_ink;
   int dlg_in_line_paper;
   int dlg_in_line_ink_ini;
   int dlg_in_line_paper_ini;
   int dlg_button_border;
   int dlg_button_ink;
   int dlg_button_paper;
   int dlg_listbox_paper;
   int dlg_listbox_ink;
   int dlg_listbox_border;
   int dlg_list_combo_paper;
   int dlg_list_combo_ink;
   int dlg_list_combo_border;
   int dlg_combo_box_paper;
   int dlg_combo_box_ink;
   int dlg_combo_box_border;
 } DLG_COLOR;

typedef enum
{
  B_PUSHBUTTON = 0,
  B_CHECKBOX,
  B_RADIOBUTTON,
  B_FLATPUSHBUTTON,
  B_ROUNDRADIOBUTTON
} T_Buton_Type;

typedef enum
{
  Dlg_Ret_Val_Cancel = 0,
  Dlg_Ret_Val_Err,
  Dlg_Ret_Val_OK ,
  Dlg_Ret_Val_Continue,
  Dlg_Ret_Val_Continue_LB,
  Dlg_Ret_Val_Reset,
  Dlg_Ret_Val_IMPORT,
  Dlg_Ret_Val_Repeat,
  Dlg_Ret_Val_Zapis_Cfg,
  Dlg_Ret_Val_Odczyt_Cfg,
  Dlg_Ret_Val_Return,
  Dlg_Ret_Val_Global,
  Dlg_Ret_Val_Local,
  Dlg_Ret_Val_Save,
  Dlg_Ret_Val_Redraw,
  Dlg_Ret_Val_UP,
  Dlg_Ret_Val_UP_END,
  Dlg_Ret_Val_DN,
  Dlg_Ret_Val_DN_END,
  Dlg_Ret_Val_Folder,
  Dlg_Ret_Val_Folder0,
  Dlg_Ret_Val_Bigger,
  Dlg_Ret_Val_Smaller,
  Dlg_Ret_Set_Range,
  Dlg_Ret_Val_Alphabet,
  Dlg_Ret_Val_Time,
  Dlg_Ret_Val_IMPORT_BLOCK,
  Dlg_Ret_Val_INSERT_BLOCK,
}
Dlg_Ret_Val;

#define BUTTON_NORMAL 1
#define BUTTON_GROUP  2
#define BUTTON_FOLDER 4
#define BUTTON_HIDDEN 8

#define DYLab  	23
#define DYBut0 	40
#define DXBut0 	90
#define DYBut1 	20  //40
//#define DYBut2 	20  //40
#define DXBut1 	45  //90
#define DXIL 	8 //6  //0
#define DYIL 	3
#define DYL     2
#define COLOR_NULL  -1
#define LINE_NULL  -2
#define WIDTH_WORLD 630
#define HEIGHT_WORLD  300 //325  bez 2 gornych linii na komunikacje
#define DYListBox	14 /*(ED_INF_HEIGHT + 2)*/
#define DXCheckBox 	15
#define DYCheckBox 14 /*(ED_INF_HEIGHT + 2)*/

#define FLB_BAROFF 1  /*listbox->flags*/
#define FLB_LIST  2
#define FLB_BACK_MEM  5 /* 4 */

#define FLDLG_NOIMAGE  0
#define FLDLG_IMAGE 1

#ifdef __cplusplus
extern "C" {
#endif

extern int Dialog(TDIALOG *dlg, DLG_COLOR*kolory, int (*fun) (int), BOOL m);
extern void Draw_Button (BUTTON *Button);
extern void Draw_ComboBox (COMBOBOX *); //, int cb_no);
extern void Draw_ListBox (LISTBOX *listbox);
extern void Draw_Input_Line (INPUTLINE *);
extern void Draw_Label (LABEL *, BOOL);
extern void Close_Dlg(TDIALOG *dlg, char typ);
extern void Redraw_Dlg(TDIALOG *dlg);
extern void Get_Default_Color_Dlg (DLG_COLOR * );
extern void Check_Radio_Button ( TDIALOG *, int, int, int, BOOL);
extern void Set_Check_Button ( TDIALOG *, int, BOOL);
extern void Set_Enable_Button(TDIALOG *dlg, int id_button, BOOL enable);
extern BOOL Get_Check_Button ( TDIALOG *, int);
extern void Get_Dlg_Rect (TDIALOG *, int *, int *, int *, int *) ;
extern TDIALOG *Get_Dlg (void) ;

#ifdef __cplusplus
}
#endif
