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

#ifdef __ALFMAIN__

#define __EDIT_MULTILINE__ L"Edycja tekstu wielowierszowego";

#define FILENAME_INI L"ALFACADPL.INI"
LPWSTR filename_ini = (LPWSTR)L"Edycja ALFACADPL.INI";

// Create a locale object representing the German (Switzerland) locale
#define _LOCALE_ "pl-PL"

char __BOLD__[]=u8"Pogrubiony";
char __UNDERLINED__[]=u8"Podkreślony";
char __ITALIC__[]=u8"Kursywa";
char __A_TO_LEFT__[]=u8"Adjustowany do lewej";
char __A_TO_RIGHT__[]=u8"Adjustowany do prawej";
char __A_TO_MIDDLE__[]=u8"Adjustowany do środka";
char __A_TO_CENTRE__[]=u8"Adjustowany do centrum";
char __HIDDEN__[]=u8"Ukryty";
char __COPY_TEXT__[]=u8"Kopiuj tekst ze schowka";

char _EDIT_TEXT_[]=u8"Edycja tekstu";

#endif

#ifdef __WINPRINT__

#define _NEW_WINDOW_ u8"Czy chcesz otworzyć nowe okno rysunku?"
#define _NEW_WINDOW_T_ u8"Nowe okno"

int max_quote = 586;
#define _QUOTE_ u8"quotespl.dat"
char about_pre[32] = u8"\n\nCytat dnia ";
char _about_[16] = u8" ";
#endif

#ifdef __BIBRYS__

POLE pmNie_Tak_[] = {
	{u8"Nie",'N',113,NULL},
	{u8"Tak",'T',112,NULL} };

POLE pmEditPCX[] = {
	{u8"Zmień dx, dy", 'Z',114, NULL},
   {u8"zmień Parametry",'P',54, NULL},
   {u8"Kalibracja",'K',115,NULL},
   //  {u8"Edycja bloku PCX", 'E',0, NULL},
};

TMENU mEditPCX = { 3,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmEditPCX,NULL,NULL };
TMENU mSaveLayer = { 2,0,0,7,16,8,ICONS,CMNU,CMBR,CMTX,0,18,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmInsertPCX[] = {
   {u8"wstaw Mapę", 'M',600, NULL},
   {u8"wstaw zdjęcie jako PNG",'P',601, NULL},
   {u8"wstaw zdjęcie jako JPG",'J',624, NULL},
   {u8"zwektoryzuj  do ALX i importuj\0\0 ",'A',698,NULL},
};

POLE pmElliptical[] = {
   {u8"Elipsa",'E',19,NULL},
   {u8"Dysk eliptyczny",'D',707,NULL},
   {u8"łUk eliptyczny",'U',706,NULL},
};

TMENU mElliptical = { 3,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmElliptical,NULL,NULL };

TMENU mInsertPCX = { 4,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,23,0,0,0,(POLE(*)[]) &pmInsertPCX,NULL,NULL };

POLE pmInsetDraw[] = {
	//{u8"Rebar",L'R',29,NULL},
    {u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},

};

TMENU mInsetDraw = { 0,0,0,6,16,8,/*ICONS*/0,CMNU,CMBR,CMTX,0,23,0,0,0,(POLE(*)[]) &pmInsetDraw,NULL,NULL };

POLE pmInsetEdit[] = {
    {u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},

};

TMENU mInsetEdit = { 0,0,0,6,16,8,/*ICONS*/0,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmInsetEdit,NULL,NULL };

POLE pmInsetBlock[] = {
    {u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},

};

TMENU mInsetBlock = { 0,0,0,6,16,8,/*ICONS*/0,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmInsetBlock,NULL,NULL };

POLE pmArray[] = {
	{u8"Prostokątny",	'P', 116,NULL},
	{u8"Kołowy", 'K', 117,NULL},
	{u8"kołowy z Obrotem", 'O',118, NULL},
};

TMENU mArray = { 3, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, 20, 0, 0,0,(POLE(*)[]) &pmArray, NULL, NULL };

POLE pmMirrorBlock[] = {
	{u8"lustro bloku X",'X',40,NULL},
	{u8"lustro bloku Y",'Y',41,NULL},
};

TMENU mMirrorBlock = { 2, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, 23, 0, 0,0,(POLE(*)[]) &pmMirrorBlock, NULL, NULL };

POLE  pmEdycja[] = {
	{u8"Przesuń\0\0",       'P',30,NULL},
	{u8"Kopiuj\0\0",        'K',31,NULL},
	{u8"Usuń\0\0",          'U',32,NULL},
	{u8"- oddaj\0\0",       '-',33,NULL},
	{u8"Obróć\0\0",         'O',34,NULL},
	{u8"oBróć blok*\0\0",   'B',35,NULL},
	{u8"Skala\0\0",         'S',36,NULL},
	{u8"Ciągnij\0\0",       'C',37,NULL},
	{u8"Edycja tekstu\0\0", 'E',38,NULL},
	{u8"Lustro\0\0",        'L',39,NULL},
	{u8"lustro bloku*\0\0", 'X',535,&mMirrorBlock},
	{u8"sZyk\0\0",          'Z',42,&mArray},
	{u8"Wytnij\0\0",        'W',44,NULL},
	{u8"uTnij\0\0",         'T',45,NULL},
	{u8"wyDłuż\0\0",        'D',46,NULL},
	{u8"proFilowanie\0\0",  'F',47,NULL},
	{u8"fazowAnie\0\0",     'A',48,NULL},
	{u8"odsuŃ\0\0",         'N',49,NULL},
	{u8"podzIel\0\0",       'I',50,NULL},
	{u8"@ cofnij\0\0  ",    '@',51,NULL},
    {u8"wtYczki\0\0  ",     'Y',534,&mInsetEdit},

};

POLE pmRysuj[] =
{
	{u8"Linia",         'L',13,NULL},
	{u8"Okrąg",         'O',14,NULL},
	{u8"łUk",           'U',15,NULL},
	{u8"Tekst",         'T',16,NULL},
	{u8"Multi tekst",   'M',408,NULL},
	{u8"Wielokąt",      'W',17,NULL},
	{u8"Koło",          'K',18,NULL},
	{u8"Eliptyczny",    'E',705,&mElliptical},
	{u8"Polilinia",     'P',20,NULL},
	{u8"oBszar",        'B',21,NULL},
	{u8"krzywa BéZiera",'Z',516,NULL},
	{u8"pRostokąt",     'R',22,NULL},
	{u8"ślaD",          'D',23,NULL},
	{u8"2-linia",       '2',24,NULL},
	{u8"3-linia z osią",'3',25,NULL},
	{u8"Szkic",         'S',26,NULL},
	{u8"puNkt",         'N',27,NULL},
    {u8"wektor",        'V',722,NULL},
	{u8"wykres Funkcji",'F',28,NULL},
	{u8"wtYczki",       'Y',534,&mInsetDraw},

};

POLE pmBlok[] = {
	{u8"Zapamiętaj\0\0 ",           'Z',58,NULL},
	{u8"Wstaw blok\0\0 ",           'W',59,NULL},
	{u8"Eksport\0\0 ",              'E',61,NULL},
	{u8"Import\0\0 ",               'I',60,NULL},
	{u8"Otwórz katalog\0\0 ",       'O',460,NULL},
	{u8"iMport blok DXF\0\0 ",      'M',84,NULL},
	{u8"wstaw oBraz\0\0 ",          'B',62,&mInsertPCX},
	{u8"eDycja bloku obrazu\0\0 ",  'D',63,&mEditPCX},
	{u8"zmień wzór bloku kreskowania\0\0 ",'H',599,NULL},
	{u8"eksporTuj obraz\0\0 ",      'T',64,NULL},
	{u8"Grupuj\0\0 ",               'G',65,NULL},
	{u8"Rozbij\0\0 ",               'R',66,NULL},
    {u8"transformacja Czworokątna\0\0 ",'C',682,NULL},
	{u8"Lista\0\0 ",                'L',67,NULL},
	{u8"* rozbij zamrożony\0\0 ",   '*',68,NULL},
	{u8"usuń bloki DXF\0\0 ",       'X',69,NULL},
	{u8"Korekta bloków\0\0 ",       'K',70,NULL},
	{u8"Stałe warstwy\0 N\0   ",    'S',71,&mSaveLayer},
    {u8"wtYczki\0\0  ",             'Y',534,&mInsetBlock},
};

POLE  pmGeometria[] = {
	{u8"pole powierzchni",          'A',72,NULL},
	{u8"środek Ciężkości",          'C',73,NULL},
	{u8"moment Statyczny",          'S',74,NULL},
	{u8"Moment bezwładności",       'M',75,NULL},
	{u8"centr.mom.Bezwładności",    'B',76,NULL},
	{u8"Test obszaru zamkniętego",  'T',77,NULL},
};

char _YES_[4] = "Т";
char _NO_[4] = "N";

const char *_EDIT_[] = {u8"Przesuń", u8"Przesuń Z", u8"Obrót", u8"Obrót XZ", u8"Obrót YZ", u8"Skala", u8"Lustro", u8"->pomiń", u8"-----"};

char* loading_program[] = { u8"Ładowanie programu", u8"Ładowanie okna..." , u8"Proszę czekać. Trwa pobieranie pliku: ", u8"Łączenie z chmurą..."};

#define _NO_MOUSE_ "Nie jest zainstalowana mysz !!!!!\n"

#endif

#ifdef __O_WYJ__

#ifdef LINUX
#define ALF_PROGRAM "AlfaCADPL"
#define ALF_TITLE "AlfaCADPL4Linux"
#else
#ifdef BIT64
#define ALF_PROGRAM "AlfaCADPL4Win64.exe"
#define ALF_TITLE "AlfaCADPL4Win64"
#else
#define ALF_PROGRAM "AlfaCADPL4Win.exe"
#define ALF_TITLE "AlfaCADPL4Win"
#endif
#endif

char err_message[] = u8"Błąd przetwarzania DWG pliku";
char err_message_code[] = u8"Kod błędu :";
char err_message1[] = u8"Błąd przetwarzania pliku roboczego.";
static char confirm[] = u8"Potwierdź";

POLE pmNewDrawing[] = {
	{u8"nowy Rysunek",'R',79,NULL},
	{u8"nowy z Prototypu",'P',533,NULL},
};

static TMENU mNewDrawing = { 2,0,0,32,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,18,0,0,0, (POLE(*)[]) &pmNewDrawing,NULL,NULL };

POLE pmNewWindow[] = {
	{u8"Nowy rysunek w nowym oknie",'N',79,NULL},
	{u8"nowy z Prototypu w nowym oknie",'P',533,NULL},
	{u8"Otwórz rysunek w nowym oknie",'O',78,NULL},
};

static TMENU mNewWindow = { 3,0,0,32,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmNewWindow,NULL,NULL };


POLE pmExportCAD[] = {
	{u8"DXF",'X',83,NULL},
	{u8"DWG",'W',506,NULL},
};

static TMENU mExportCAD = { 1,0,0,16,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,14,0,0,0,(POLE(*)[]) &pmExportCAD,NULL,NULL };


POLE pmImportCAD[] = {
	{u8"DXF",'X',84,NULL},
	{u8"DWG",'W',507,NULL},
};

static TMENU mImportCAD = { 2,0,0,16,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,16,0,0,0,(POLE(*)[]) &pmImportCAD,NULL,NULL };

POLE pmTree[] = {
	{u8"Wejście",'W',537,NULL},
	{u8"Powrót",'P',538,NULL},
};

static TMENU mTree = { 2,0,0,16,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmTree,NULL,NULL };

POLE pmLastFiles[] = {
    {u8"1   \0 \0",'1',0,NULL},
	{u8"2   \0 \0",'2',0,NULL},
	{u8"3   \0 \0",'3',0,NULL},
	{u8"4   \0 \0",'4',0,NULL},
	{u8"5   \0 \0",'5',0,NULL},
	{u8"6   \0 \0",'6',0,NULL},
	{u8"7   \0 \0",'7',0,NULL},
	{u8"8   \0 \0",'8',0,NULL},
	{u8"9   \0 \0",'9',0,NULL},
    {u8"0   \0 \0",'0',0,NULL},
	{u8"Q   \0 \0",'Q',0,NULL},
	{u8"W   \0 \0",'W',0,NULL},
	{u8"E   \0 \0",'E',0,NULL},
	{u8"R   \0 \0",'R',0,NULL},
	{u8"T   \0 \0",'T',0,NULL},
	{u8"Y   \0 \0",'Y',0,NULL},
};

static TMENU mLastFiles = { 16,0,0,33,20,7,TADD,CMNU,CMBR,CMTX,0,25,0,0,0,(POLE(*)[]) &pmLastFiles,NULL,NULL };

POLE pmQuit[] = {
	 {u8"Otwórz rysunek\0",'O',78,NULL},
	 {u8"nowy Rysunek\0",'R',79,&mNewDrawing},
	 {u8"Nowe okno\0",'N',80,&mNewWindow},
	 {u8"Uaktualnij\0",'U',81,NULL},
	 {u8"Zapisz jako\0",'Z',82,NULL},
	 {u8"Struktura drzewiasta\0",'S',536,&mTree},
	 {u8"Eksport\0",'E',504,&mExportCAD},
	 {u8"Import\0",'I',505,&mImportCAD},
	 {u8"edycja Tabliczki\0", 'T',85, NULL},
	 /* {u8"Skok do systemu\0",'S',86,NULL},*/
	 {u8"Drukowanie\0",'D',87,NULL},
	 {u8"Plotowanie (wektorowe)\0",'P',394,NULL},
	 {u8"Info\0",'?',88,NULL},
	 {u8"pokaż Biurko\0",'B',528,NULL},
	 {u8"Wcześniejsze rysunki\0",'W',89,&mLastFiles}
};

static POLE pmTTF_OTF[] = {
   {u8"czcionki TrueType",'T',703,NULL},
   {u8"czcionki OpenType",'O',704,NULL},
};

static TMENU mTTF_OTF = { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmTTF_OTF,NULL,NULL };

static POLE pmCzcionkaEkranTTF[] = {
   {u8"Czcionka\0\0 ",'C', 111,&mTTF_OTF},
   {u8"Wysokość\0\0 ",'W',179,NULL},
   {u8"Szerokość wsp.\0\0 ",'S',230,NULL},      //25
};

TMENU mCzcionkaEkranTTF = { 3,0,0,30,20,7,ICONS | TADD,CMNU,CMBR,CMTX,0,28,0,0,0,(POLE(*)[]) &pmCzcionkaEkranTTF,NULL,NULL };  //26

static POLE pmWindow[] = {
   {u8"rozszerz Horyzontalnie",'H',467,NULL},
   {u8"rozszerz Wertykalnie",'W',468,NULL},
   {u8"rozszerz Diagonalnie",'D',469,NULL},
   {u8"rozszerz Okno",'O',470,NULL},
   {u8"przywróć Poprzednie",'P',471,NULL},
};

static TMENU mWindow = { 5,0,0,16,22,9,ICONS,CMNU,CMBR,CMTX,0,11,0,0,0,(POLE(*)[]) &pmWindow,NULL,NULL };  //9


static POLE pmDialogCursor[] = {
   {u8"Mały",'M',591,NULL},
   {u8"Duży",'D',592,NULL},
};

#define smallcursor u8"Mały"
#define bigcursor u8"Duży"

static TMENU mDialogCursor = { 2,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,24,0,0,0,(POLE(*)[]) &pmDialogCursor,NULL,NULL };  //22

static POLE pmMenuCursor[] = {
   {u8"Ramka",'R',821,NULL},
   {u8"Kursor",'K',822,NULL},
};

static TMENU mMenuCursor = { 2,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,33,0,0,0,(POLE(*)[]) &pmMenuCursor,NULL,NULL };  //22

#define barstyle u8"Ramka"
#define cursorstyle u8"Kursor"

static POLE pmTranslucency[] = {
   {u8"100%",'0',546,NULL},
   {u8"90%",'9',545,NULL},
   {u8"80%",'8',544,NULL},
   {u8"70%",'7',543,NULL},
   {u8"60%",'6',542,NULL},
   {u8"50%",'5',541,NULL},
   {u8"40%",'4',540,NULL},
   {u8"30%",'3',539,NULL},
};

static TMENU mTranslucency = { 8,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,16,0,0,0,(POLE(*)[]) &pmTranslucency,NULL,NULL };  //14

static TMENU mDemoSelect = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,26,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };  //24

static TMENU mAutoPan = { 2,0,0,7,33,11,ICONS,CMNU,CMBR,CMTX,0,31,0,0,0,(POLE(*)[]) & pmTak_Nie,NULL,NULL };  //new

POLE pmOpcje[] = {
	{u8"konfiguracja kolorów Pulpitu\0",'P',109,NULL},
	{u8"auto Panoramowanie \0Yes\0 ",'P',145, &mAutoPan},  //PAN
	{u8"wielkość Auto-panoramy \0 ",'A',110,NULL},
	{u8"Nieprzezroczystość wypełnień \0",'N',542,&mTranslucency},
	{u8"Czcionka pulpitu \0 ",'C',111,&mCzcionkaEkranTTF},
	//{u8"Czcionka pulpitu \0 ",'C',111,&mCzcionkaEkran},
	{u8"Tło pulpitu \0 ",'T',527,NULL},
	{u8"Kursor w oknach dialogowych \0",'K',590,&mDialogCursor},
    {u8"styl Menu\0",'M',820,&mMenuCursor},
	{u8"Edukacyjny tryb demonstracyjny \0 N\0",'E',661,&mDemoSelect},
	{u8"Zapisz ustawienie okna \0",'Z',530,NULL},
	{u8"ustawienie Okna \0",'O',478,&mWindow},
};

#define _FILE_ u8"Plik "
#define _FILE_EXISTS_ u8" istnieje. Zapisać ? (Tak/Nie/Esc):"

#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "T"
#define _NO__ "N"
#define _INTERRUPT_ u8"Brak bloku '%s' ! Wydruk zatrzymany "
#define _SPOOLED_INTERRUPT_ u8"Błąd zapisu rysunku do zbioru roboczego ! Wydruk zatrzymany. "
#define _FORMAT_INTERRUPT_ u8"Brak bloku '*FORMAT' ! Wydruk zatrzymany "
#define _PAGE_INTERRUPT_ u8"Brak bloku '*ARKUSZ' ! Wydruk zatrzymany "
#define _CONVERSION_TERMINATION_ u8"CZEKAJ !!! Konwersja zbioru. Przerwanie -> Esc... "
#define _ERROR_DXF_ u8"Błąd w zbiorze DXF lub przerwanie  "
#define _LINE_NO_ u8" -> linia %#ld"

#define _TO_MANY_WIN_ u8"Zbyt dużo otwartych okien"
#define _CLOSE_WINDOW_  u8"Czy napewno zamknąć okno ?"

#define _DRAWING_NOT_SAVED_ u8"Rysunek nie uaktualniony"

#define _Yes_ u8"Tak"
#define _No_ u8"Nie"

#define _SOME_OPEN_DRAWING_NOT_SAVED_ u8"Otwarte rysunki nie są uaktualnione"
#define _SAVE_IT_ u8"Uaktualnić ? "
#define _SAVE_THEM_ u8"Uaktualnić ? "

#define _NO_MORE_DRAWINGS_ u8"Nie ma więcej rysunków"

#define _NEW_DRAWING_ u8"Nowy rysunek"

#define _SAVING_CONF_ u8"Zapis konfiguracji"

#define _WANT_EXIT_ u8"Czy napewno zakończyć program ?"

#define _WANT_CLOSE_WINDOW_ u8"Czy napewno zamknąć okno ?"

#define _SAVING_PREFILES_ "Zapis HISTORII"

#define _FONT_CHANGED_ u8"Czcionka została zmieniona"

#define desktop_tip "@Wybierz rysunek"

#define _DEMO_MODE_ "W DEMO wciśnięte klawisze i przyciski myszy"
#define _DEMO_MODE1_ "są wyświetlane na dole okna programu"

#endif

#ifdef __ASKING__

#define _Yes_ u8"Tak"
#define _No_ u8"Nie"

#define _NOTICE_ u8"Uwaga"
#define _AT_NOTICE_  u8"@Uwaga"
#define _COMBINATION_  u8"Numer kombinacji obciążeń"
#define _GEOM_STIFFNESS_ u8"Sztywność geometryczna"
#define _INERTIA_ u8"Uwzględnić obciążenie stałe (DL) w drganiach"
#define _VIBRATIONS_ u8"Liczba trybów drgań własnych"

#define _INSTALATION_ u8"Instalacja"

static char* combination_txt[8] = {
					u8"1",
					u8"2",
					u8"3",
					u8"4",
					u8"5",
					u8"6",
					u8"7",
					u8"8"};
/*
					u8"9",
					u8"10",
					u8"11",
					u8"12",
					u8"13",
					u8"14",
					u8"15",
					u8"16",
					u8"17",
					u8"18",
					u8"19",
					u8"20",
					};
*/

static char* modes_number_txt[] = {
                    u8"0",
					u8"1",
					u8"2",
					u8"3",
					u8"4",
					u8"5",
					u8"6",
					u8"7"};
/*
					u8"8",
					u8"9",
					u8"10",
					u8"11",
					u8"12",
					u8"13",
					u8"14",
					u8"15",
					u8"16",
					u8"17",
					u8"18",
					u8"19",
					u8"20",
					};
*/
#endif

#ifdef __O_INSTRUCT__


char instruct_txt[][64]={u8"Otwórz menu / Akceptuj",
                        u8"Zamknij menu / Zrezygnuj",
                        u8"Uwolnienie myszki (lub potrząśnij nią w lewo i prawo)",
                        u8"Menu pomocnicze",
                        u8"Pomocnicze menu kontekstowe",
                        u8"Ortogonalność włącz/wyłącz",
                        u8"Skalowanie / przewijanie",
                        u8"Panoramowanie (rączka Myszki Miki)",
                        u8"Powtórzenie polecenia",
                        u8"Wybór uchwytu (jest ich znacznie więcej)",
                        u8"Pomocna dłoń (wszystkie skróty klawiszowe)",
                        u8"(lewy) zmiana układu klawiatury podczas edycji",
                        u8"dla greckich liter i symboli matematycznych",
                        u8"są klawiszami \"przyklejającymi się\". W przypadku",
                        u8"konfliktu ze skrótami systemowymi, naciśnij taki klawisz,",
                        u8"następnie zwolnij go i wciśnij drugi klawisz kombinacji",
                        u8"Edycja wartości numerycznych lub reedycja tekstu",
                        u8"lub"};


#define osnap_icon0 162
#define osnap_icon1 153
#define osnap_icon2 170
#define osnap_icon3 167
#define osnap_icon4 154
#define osnap_icon5 169

char stay_string[64]=u8"Nie pokazuj tego więcej";

#define _Yes_ u8"Tak"
#define _No_ u8"Nie"

#define _INSTRUCTION_ u8"Instrukcja"
#define _AT_INSTRUCTION_  u8"@Instrukcja"

#endif

#ifdef __DIALOG__

#define _OTHER_ u8"֍inny>>> "

#define _LAYERS_ u8"Warstwy"

#endif

#ifdef __ADDMENU__

#define MAXPLACEMARKER 5

#define _YES_ "T"
#define _NO_ "N"

static char ggsel[10][16] = { u8"Koniec", u8"Bliższy koniec", u8"śrOdek", u8"Przecięcie", u8"Centrum", u8"pRostopadle",
						 u8"stycznie", u8"Najbliższy", u8"punkt", u8"----" };

POLE pmKolor[] = {
	/*1*/      {u8"֍[A] czerwony",'A',0,NULL},  		/*12*/
	/*2*/      {u8"֍[B] żółty",'B',0,NULL},                   /*14*/
	/*3*/      {u8"֍[C] zielony",'C',0,NULL},                 /*10*/
	/*4*/      {u8"֍[D] turkusowy" ,'D',0,NULL},               /*11*/
	/*5*/      {u8"֍[E] niebieski",'E',0,NULL},               /*9*/
	/*6*/      {u8"֍[F] karmazynowy",'F',0,NULL},             /*13*/
	/*7*/      {u8"֍[G] biały",'G',0,NULL},                   /*15*/
	/*8*/	   {u8"֍[H] ciemnoszary",'H',0,NULL},            /*8*/
	/*9*/      {u8"֍[I] ciemnoczerwony",'I',0,NULL},         /*4*/
	/*10*/	   {u8"֍[J] brązowy",'J',0,NULL},                /*6*/
	/*11*/     {u8"֍[K] ciemnozielony",'K',0,NULL},          /*2*/
	/*12*/     {u8"֍[L] ciemnoturkusowy",'L',0,NULL},        /*3*/
	/*13*/     {u8"֍[M] ciemnoniebieski",'M',0,NULL},        /*1*/
	/*14*/     {u8"֍[N] ciemnokarmazynowy",'N',0,NULL},      /*5*/
	/*15*/	   {u8"֍[O] szary",'O',0,NULL},                  /*7*/
	/*16*/	   {u8"֍[P] czarny",'P',0,NULL},                 /*0*/
	/*17*/     {u8"֍[X] inny\0 0\0  ",'X',0,NULL}
};

POLE pmKolorL_[] = {
	/*1*/      {u8"֍[A] czerwony",'A',0,NULL},  		/*12*/
	/*2*/      {u8"֍[B] żółty",'B',0,NULL},                   /*14*/
	/*3*/      {u8"֍[C] zielony",'C',0,NULL},                 /*10*/
	/*4*/      {u8"֍[D] turkusowy" ,'D',0,NULL},               /*11*/
	/*5*/      {u8"֍[E] niebieski",'E',0,NULL},               /*9*/
	/*6*/      {u8"֍[F] karmazynowy",'F',0,NULL},             /*13*/
	/*7*/      {u8"֍[G] biały",'G',0,NULL},                   /*15*/
	/*8*/	   {u8"֍[H] ciemnoszary",'H',0,NULL},            /*8*/
	/*9*/      {u8"֍[I] ciemnoczerwony",'I',0,NULL},         /*4*/
	/*10*/	   {u8"֍[J] brązowy",'J',0,NULL},                /*6*/
	/*11*/     {u8"֍[K] ciemnozielony",'K',0,NULL},          /*2*/
	/*12*/     {u8"֍[L] ciemnoturkusowy",'L',0,NULL},        /*3*/
	/*13*/     {u8"֍[M] ciemnoniebieski",'M',0,NULL},        /*1*/
	/*14*/     {u8"֍[N] ciemnokarmazynowy",'N',0,NULL},      /*5*/
	/*15*/	   {u8"֍[O] szary",'O',0,NULL},                  /*7*/
	/*16*/	   {u8"֍[P] czarny",'P',0,NULL},                 /*0*/
	/*17*/     {u8"֍[X] inny\0 0\0  ",'X',0,NULL}
};

POLE pmKolorT_[] = {
	/*1*/      {u8"֍[A] czerwony",'A',0,NULL},  		/*12*/
	/*2*/      {u8"֍[B] żółty",'B',0,NULL},                   /*14*/
	/*3*/      {u8"֍[C] zielony",'C',0,NULL},                 /*10*/
	/*4*/      {u8"֍[D] turkusowy" ,'D',0,NULL},               /*11*/
	/*5*/      {u8"֍[E] niebieski",'E',0,NULL},               /*9*/
	/*6*/      {u8"֍[F] karmazynowy",'F',0,NULL},             /*13*/
	/*7*/      {u8"֍[G] biały",'G',0,NULL},                   /*15*/
	/*8*/	   {u8"֍[H] ciemnoszary",'H',0,NULL},            /*8*/
	/*9*/      {u8"֍[I] ciemnoczerwony",'I',0,NULL},         /*4*/
	/*10*/	   {u8"֍[J] brązowy",'J',0,NULL},                /*6*/
	/*11*/     {u8"֍[K] ciemnozielony",'K',0,NULL},          /*2*/
	/*12*/     {u8"֍[L] ciemnoturkusowy",'L',0,NULL},        /*3*/
	/*13*/     {u8"֍[M] ciemnoniebieski",'M',0,NULL},        /*1*/
	/*14*/     {u8"֍[N] ciemnokarmazynowy",'N',0,NULL},      /*5*/
	/*15*/	   {u8"֍[O] szary",'O',0,NULL},                  /*7*/
	/*16*/	   {u8"֍[P] czarny",'P',0,NULL},                 /*0*/
	/*17*/     {u8"֍[X] inny\0 0\0  ",'X',0,NULL}
	//           {&C_INNY,'X',0,NULL},
};

TMENU mKolorAC = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,23,56,5,0,CMNU,CMBR,CMTX,15,72,0,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };

#define _OTHER_NO_ u8"֍[X] inny>>> %#ld"

static POLE pmKrok_k[9] = {
	 {u8"[A]\0  2.5  *",'A',0,NULL},
	 {u8"[B]\0  1.25 *",'B',0,NULL},
	 {u8"[C]\0  0.25 *",'C',0,NULL},
	 {u8"[D]\0  0.5",'D',0,NULL},
	 {u8"[E]\0  1",'E',0,NULL},
	 {u8"[F]\0  5",'F',0,NULL},
	 {u8"[G]\0 10",'G',0,NULL},
	 {u8"[X]\0 X\0     ",'X',0,NULL},
     {u8"Minimalny\0", 'M', 0, NULL}  };

static TMENU mKrok_k = { 9,0,0,13,52 ,6,TADD,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmKrok_k,NULL,NULL };

POLE pmTak_Nie[2] = {
	{u8"Tak",'T',112,NULL},
	{u8"Nie",'N',113,NULL} };


POLE pmGruboscLinii[] = {
	   {u8"҂bArdzo cienka",'A',0,NULL},
	   {u8"҂Cienka",'C',0,NULL},
	   {u8"҂Gruba",'G',0,NULL},
	   {u8"҂Bardzo gruba",'B',0,NULL},
	   {u8"҂Najgrubsza",'N',0,NULL},
};

POLE pmGruboscLiniiS[] = {
       {u8"Wypełnienie",'W',0,NULL},
	   {u8"҂bArdzo cienka",'A',0,NULL},
	   {u8"҂Cienka",'C',0,NULL},
	   {u8"҂Gruba",'G',0,NULL},
	   {u8"҂Bardzo gruba",'B',0,NULL},
	   {u8"҂Najgrubsza",'N',0,NULL},
};

static POLE pmTypyLinii[] = {
		   {u8"֏Ciągła",'1',0,NULL},
		   {u8"֏Kreskowa",'2',0,NULL},
		   {u8"֏Punktowa",'3',0,NULL},
		   {u8"֏Dwupunktowa",'4',0,NULL},
		   {u8"֏Wielopunktowa",'5',0,NULL},
		   {u8"֏Granica",'6',0,NULL},
		   {u8"֏Środkowa",'7',0,NULL},
		   {u8"֏Ukryta",'8',0,NULL},
		   {u8"֏widMo",'9',0,NULL},
		   {u8"֏kreskowa_2",'A',0,NULL},
		   {u8"֏punktowa_2",'B',0,NULL},
		   {u8"֏dwupunktowa_2",'C',0,NULL},
		   {u8"֏wielopunkt._2",'D',0,NULL},
		   {u8"֏granica_2",'E',0,NULL},
		   {u8"֏środkowa_2",'F',0,NULL},
		   {u8"֏ukryta_2",'G',0,NULL},
		   {u8"֏widmo_2",'H',0,NULL},
		   {u8"֏kreskowa_x2",'I',0,NULL},
		   {u8"֏punktowa_x2",'J',0,NULL},
		   {u8"֏dwupunktowa_x2",'K',0,NULL},
		   {u8"֏wielopunkt._x2",'L',0,NULL},
		   {u8"֏granica_x2",'M',0,NULL},
		   {u8"֏środkowa_x2",'N',0,NULL},
		   {u8"֏ukryta_x2",'O',0,NULL},
		   {u8"֏widmo_x2",'P',0,NULL},
		   {u8"֏trzypunktowa.",'Q',0,NULL},
		   {u8"֏dwurodkowa",'R',0,NULL},
		   {u8"֏trzykreskowa",'S',0,NULL},
		   {u8"֏widmo3",'T',0,NULL},
		   {u8"֏dwuŚrodkowa_x2",'U',0,NULL},
		   {u8"֏trzykresk._x2",'V',0,NULL},
		   {u8"֏wielokreskowa",'W',0,NULL},
};

static TMENU mPunkty_siatki = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0,28,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mOrto = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,30,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mSwobodnyBlok = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mSwobodnyTekst = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mScale_DIM = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,93,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mCiagniecieLinii = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,4,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mRozcinanieLinii = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUkrywanieTypu = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,89,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mWidocznoscTypu = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,91,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mWypelnianiePCX = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,105,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mView_Only_C_L = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,107,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mBuffering_PCX = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,109,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mNormalizacjaTekstu = { 2,0,0,7,52,9,ICONS,CMNU,CMBR,CMTX,0,112,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };


//static TMENU mType_View={2,0,0,7,52,9,0,CMNU,CMBR,CMTX,0,91,0,0,0,(POLE(*)[])&pmTak_Nie,NULL,NULL};
static TMENU mAutoPan = { 2,0,0,7,33,11,ICONS,CMNU,CMBR,CMTX,0,17,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mPoint_View = { 2, 0, 0, 7, 52, 13, ICONS, CMNU,CMBR,CMTX,0, 70, 0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };


TMENU mGruboscLinii = { 5,0,0,26,52,12,0,CMNU,CMBR,CMTX,0,65,0,0,0,(POLE(*)[]) &pmGruboscLinii,NULL,NULL };

TMENU mGruboscLiniiS = { 6,0,0,26,52,12,0,CMNU,CMBR,CMTX,0,65,0,0,0,(POLE(*)[]) &pmGruboscLiniiS,NULL,NULL };


TMENU mTypyLinii = { 32,16,16,26,52,12,0,CMNU,CMBR,CMTX,32,32,0,0,0,(POLE(*)[]) &pmTypyLinii,NULL,NULL };

static POLE pmGestosc[6] = {
	 {u8"[A]\0 2.5 (*)",'A',0,NULL},
	 {u8"[B]\0 1",'B',0,NULL},
	 {u8"[C]\0 5",'C',0,NULL},
	 {u8"[D]\0 10",'D',0,NULL},
	 {u8"[E]\0 100",'E',0,NULL},
	 {u8"[X]\0 X\0    ",'X',0,NULL} };
static TMENU mGestosc = { 6,0,0,13,52,7,TADD,CMNU,CMBR,CMTX,0,21,0,0,0,(POLE(*)[]) &pmGestosc,NULL,NULL };


POLE pmParametry[] = {
	 {u8"krok kursorA\0",        'A',154,&mKrok_k},   /*.25*/
	 {u8"gęstość siatkI\0",        'I',155,&mGestosc }, /*2.5*/
	 {u8"punkty siatki\0",                 'V',156,&mPunkty_siatki},
	 {u8"Prostopadłość (F8)\0",            'P',157,&mOrto},
	 {u8"֎Kolor\0Biały\0",     'K',158,&mKolorAC},   /*Biały*/
	 {u8"Grubość linii\0Gruba\0",  'G',159,&mGruboscLinii},
	 {u8"typ Linii\0Ciągła\0",   'L',160,&mTypyLinii},   /*Ciągła*/
	 {u8"wiDoczność punktów i konturów\0 T\0", 'D', 161,&mPoint_View},
	 {u8"rozMiar punktu\0 2.5\0",      'M',162,NULL},
	 {u8"weź kOlor     (Alt-F8)\0",          'O',163,NULL},
	 {u8"weź tYp       (Alt-F9)\0",          'Y',164,NULL},
	 {u8"weź Warstwę   (Alt-F10)\0",         'W',165,NULL},
	 {u8"swobodny Blok    \0",             'B',167,&mSwobodnyBlok},
	 {u8"swobodny Tekst * \0",             'T',166,&mSwobodnyTekst},
	 {u8"Skalow. bloku wymiarowego\0 N\0", 'S',168,&mScale_DIM},
	 {u8"Ciągnięcie linii w węzłach\0 T\0",'C',169,&mCiagniecieLinii},
	 {u8"Rozcinanie linii w węzłach\0 T\0",'R',170,&mRozcinanieLinii},
	 {u8"Ukrywanie typu\0 T\0",            'U',171,&mUkrywanieTypu},
	 {u8"widoczNość ukrytego tekstu\0 N\0",'N',172,&mWidocznoscTypu},
	 {u8"wypełnianie punktów bitMap\0 N\0",'X',173,&mWypelnianiePCX},
	 {u8"wyróżnianiE bieżącej warstwy\0 N\0",'E',174,&mView_Only_C_L},
	 {u8"buForowanie bitmap\0 T\0",           'F',175,&mBuffering_PCX},
	 // {u8"widoczNość bloków arkuszy\0 N\0",'H',0,&mView_Sheets},
	  {u8"normaliZacja tekstu\0",           'Z',176,&mNormalizacjaTekstu},
};

TMENU mParametrym = { 23,0,0,38,23,4,ICONS | TADD,CMNU,CMBR,CMTX,0,59,0,0,0,(POLE(*)[]) &pmParametry,NULL,NULL };

POLE pmParametry_view[] = {
	 {u8"krok kursorA\0",        'A',154,&mKrok_k},   /*.25*/
	 {u8"gęstość siatkI\0",        'I',155,&mGestosc }, /*2.5*/
	 {u8"# punkty siatki\0",               '#',156,&mPunkty_siatki},
	 {u8"wiDoczność punktu\0 T\0",  'D', 161,&mPoint_View},
	 {u8"rozMiar punktu\0 2.5\0",      'M',162,NULL},
	 {u8"widoczNość ukrytego tekstu\0 N\0",'N',172,&mWidocznoscTypu},
	 {u8"wyróżnianiE bieżącej warstwy\0 N\0",'E',174,&mView_Only_C_L},
};

TMENU mParametrym_view = { 7,0,0,30,23,4,ICONS | TADD,CMNU,CMBR,CMTX,0,59,0,0,0,(POLE(*)[]) &pmParametry_view,NULL,NULL };

static POLE pmSetFindMarker[] = {
	   {u8"1\0  ",'1',0,NULL},
	   {u8"2\0  ",'2',0,NULL},
	   {u8"3\0  ",'3',0,NULL},
	   {u8"4\0  ",'4',0,NULL},
	   {u8"5\0  ",'5',0,NULL}, };

TMENU mSetMarker = { MAXPLACEMARKER,0,0,4,30,6,TADD,CMNU,CMBR,CMTX,0,19,0,0,0,(POLE(*)[]) &pmSetFindMarker,NULL,NULL };
TMENU mFindMarker = { MAXPLACEMARKER,0,0,4,30,7,TADD,CMNU,CMBR,CMTX,0,24,0,0,0,(POLE(*)[]) &pmSetFindMarker,NULL,NULL };


static POLE pmSkala[] = {
	 {u8"Odśwież\0",'O',142,NULL},
	 {u8"Zapamiętaj widok\0", 'Z', 140,&mSetMarker},
	 {u8"Przywróć widok\0", 'P', 141,&mFindMarker},
	 {u8"Wszystko\0",'W',143,NULL},
	 {u8"Centrum\0",'C',144,NULL},
	// {u8"Auto Pan\0Tak\0     ",'A',145, &mAutoPan},
	 {u8"Panoramuj (Ctrl-Shift)\0",'P',145, NULL},
	 {u8"[1] *1.0 \0",'1',146,NULL},
	 {u8"[5] *0.5\0",'5',147,NULL},
	 {u8"[2] *0.2\0",'2',148,NULL},
	 {u8"[0] *0.1\0",'0',149,NULL},
	 {u8"*X\0",'X',150,NULL},
	 {u8"oKno\0",'K',151,NULL},
	 {u8"Format\0",'F',152,NULL},
	 {u8"osTatni\0",'T',153,NULL},
	 {u8"Skok do x;y\0",'S',129,NULL},
	 {u8"skok o Dx;dy\0",'D',130,NULL},
	 {u8"skok o L;kąt\0",'L',131,NULL},
}; /*rownolegle nalezy modyfikowac funkcje SkalaZ  (1.,.5,.2,.1)*/

TMENU mSkala = { 17,0,0,20,12,4,ICONS | TADD,CMNU,CMBR,CMTX,0, 30, 0,0,0,(POLE(*)[]) &pmSkala,NULL,NULL };



static POLE pmTryb[] = {
	{u8"Koniec\0",'K',120,NULL},
	{u8"Bliższy koniec\0",'B',121,NULL},
	{u8"Środek\0",'S',122,NULL},
	{u8"Przecięcie\0",'P',123,NULL},
	{u8"Centrum\0",'C',124,NULL},
	{u8"pRostopadle\0",'R',125,NULL},
	{u8"sTycznie\0",'T',126,NULL},
	{u8"Najbliższy\0",'N',127,NULL},
	{u8"pUnkt\0",'U',128,NULL},
	{u8"żaden\0",'X',132,NULL} };

TMENU mTryb = { 10,0,0,20,20,6,ICONS,CMNU,CMBR,CMTX,0, 13, 0,0,0,(POLE(*)[]) &pmTryb,NULL,NULL };

static POLE pmPunkt[] = {
	{u8"Tryb powtarzania\0        ",'T',119,&mTryb},
	{u8"Koniec\0",'K',120,NULL},
	{u8"Bliższy koniec\0",'B',121,NULL},
	{u8"Środek\0",'S',122,NULL},
	{u8"Przecięcie\0",'P',123,NULL},
	{u8"Centrum\0",'C',124,NULL},
	{u8"pRostopadle\0",'R',125,NULL},
	{u8"sTycznie\0",'T',126,NULL},
	{u8"Najbliższy\0",'N',127,NULL},
	{u8"pUnkt\0",'U',128,NULL},
	{u8"skok do X, Y\0",'X',129,NULL},
	{u8"skok o dX, dY\0",'Y',130,NULL},
	{u8"skok o L, kąt\0",'L',131,NULL}, };

TMENU mPunkt = { 13,0,0,30,1,4,ICONS | TADD,CMNU,CMBR,CMTX,0, 7, 0,0,0,(POLE(*)[]) &pmPunkt,NULL,NULL };

static POLE pmSum[] = {
	 {u8"Przerwij sumowanie\0 F10",'P',139,NULL},
	 {u8"Dodawaj/odejmuj\0 F9",'D',137,NULL},
	 };

static TMENU mSum = { 2, 0,0,23,56,9,ICONS | TADD,CMNU,CMBR,CMTX,0, 7, 0, 0,0,(POLE(*)[]) &pmSum, NULL,NULL };

POLE pmMeasure[] = {
	{u8"kAlkulator", 'A', 133,NULL},
	{u8"Punkt",      'P',134,NULL},
	{u8"Odległość", 'O',135,NULL},
	{u8"Kąt",        'K',136,NULL},
	{u8"Sumowanie", 'S', 137,&mSum},
	{u8"Lista parametrów", 'L', 138,NULL},
};

static TMENU mMeasure = { 6,0,0,16,45,4,ICONS,CMNU,CMBR,CMTX,0, 154 , 0,0,0,(POLE(*)[]) &pmMeasure, NULL,NULL };

POLE pmInsetAux[] = {
    {u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
	{u8"",L' ',0,NULL},
};

TMENU mInsetAux = { 0,0,0,6,0,0,/*ICONS*/0,CMNU,CMBR,CMTX,0,173,0,0,0,(POLE(*)[]) &pmInsetAux,NULL,NULL };

//52 pointfinder, 53 camera, 54 parameters, 55 layers, 56 measure, 3 search
static POLE pmenup[] = {
	{u8"Punkt\0\0 ",L'P',52,&mPunkt},
	{u8"Ekran\0 1\0    ",L'E',53,&mSkala},
	{u8"pArametry\0\0 ",L'A',54,&mParametrym},
	{u8"Warstwy\0\0 ",L'W',55,NULL},
	{u8"Miara\0\0 ", L'M',56,&mMeasure},
	{u8"Znajdź\0\0 ",L'Z',3,NULL},
    {u8"wtYczki\0\0",L'Y',534,&mInsetAux},
	{u8"\0               \0 ",L' ',0,NULL} };

TMENU menup = { 8,0,0,12,1,2,ICONS | TADD | NVERT,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmenup,NULL,NULL };   //7,0,11,

#endif

#ifdef __LAYERS__

#define NoWidthLines	5 /*3*/
#define NoTypeLines	32 /*5*/
static char* width_line_txt_L[NoWidthLines + 1] = { u8"҂b.cienka",u8"҂Cienka",u8"҂Gruba",u8"҂B.Gruba",u8"҂Najgrubsza",u8"" };

static char* Colors_Name_txt_L[MAXCOLOR + 1] =
{
  u8"֍czerwony",
  u8"֍żółty" ,
  u8"֍zielony" ,
  u8"֍turkusowy" ,
  u8"֍niebieski" ,
  u8"֍karmazynowy" ,
  u8"֍biały" ,
  u8"֍ciemnoszary" ,
  u8"֍c.czerwony" ,
  u8"֍brązowy" ,
  u8"֍c.zielony" ,
  u8"֍c.turkusowy" ,
  u8"֍c.niebieski" ,
  u8"֍c.karmazynowy" ,
  u8"֍szary" ,
  u8"֍czarny" ,
  u8"֍inny>>> ",
  u8"",
};

static char* type_line_txt_L[NoTypeLines + 1] = {
				u8"֏Ciągła",
				u8"֏Kreskowa",
				u8"֏Punktowa",
				u8"֏Dwupunktowa",
				u8"֏Wielopunktowa",
				u8"֏Granica",
				u8"֏Śodkowa",
				u8"֏Ukryta",
				u8"֏widMo",
				u8"֏kreskowa_2",
				u8"֏punktowa_2",
				u8"֏dwupunktowa_2",
				u8"֏wielopunkt._2",
				u8"֏granica_2",
				u8"֏środkowa_2",
				u8"֏ukryta_2",
				u8"֏widmo_2",
				u8"֏kreskowa_x2",
				u8"֏punktowa_x2",
				u8"֏dwupunkt._x2",
				u8"֏wielopunk._x2",
				u8"֏granica_x2",
				u8"֏środkowa_x2",
				u8"֏ukryta_x2",
				u8"֏widmo_x2",
				u8"֏trzypunktowa",
				u8"֏dwuŚrodkowa",
				u8"֏trzykreskowa",
				u8"֏widmo_3",
				u8"֏d.Środkowa_x2",
				u8"֏trzykresk._x2",
				u8"֏wielokreskowa",
				u8"" };

#define layer_comm0 "PgUp"
#define layer_comm1 "PgDn"
#define layer_comm2 "Nowa"
#define layer_comm3 "OK"
#define layer_comm4 "Esc"
#define layer_comm5 "+"
#define layer_comm6 "+"
#define layer_comm7 "+"
#define layer_comm8 "-"
#define layer_comm9 "-"
#define layer_comm10 "-"
#define layer_comm11 "+"
#define layer_comm12 "+"
#define layer_comm13 "-"
#define layer_comm14 "-"
#define layer_comm15 u8"Bieżąca"
#define layer_comm16 "Nazwa Warstwy"
#define layer_comm17 ""
#define layer_comm18 "Widoczna"
#define layer_comm19 "Edycja"
#define layer_comm20 "Punkt"
#define layer_comm21 "Kolor"
#define layer_comm22 u8"Grubość"
#define layer_comm23 "Linii"
#define layer_comm24 "Typ"
#define layer_comm25 "Linii"
#define layer_comm26 "Wszystkie warstwy"
#define layer_comm27 "Wszystkie warstwy"
#define layer_comm28 "Gray"
#define layer_comm29 "B/W"
#define layer_comm30 u8"Załącz"
#define layer_comm31 u8"Wyłącz"
#define layer_comm32 u8"Modyfikacja Warstw"
#define layer_comm33 u8"Usuń"
#define layer_comm34 u8"Warstwy"
#define layer_comm35 u8"Przywróć"



#define layer_tip0 u8"@Widoczność"
#define layer_tip1 u8"Edytowalność"
#define layer_tip2 u8"Lokalizowalność"
#define layer_tip3 u8"Widoczność wszystkich warstw"
#define layer_tip4 u8"Edytowalność wszystkich warstw"
#define layer_tip5 u8"Lokalizowalność wszystkich warstw"
#define layer_tip6 u8"Kolor"
#define layer_tip7 u8"Grubość linii"
#define layer_tip8 u8"Typ linii"
#define layer_tip9 u8"na drukarce i ploterze"
#define layer_tip10 u8"na drukarce i ploterze"
#define layer_tip11 u8"Wydruk w odcieniach szarości"
#define layer_tip12 u8"Wydruk w trybie czarno-białym"
#define layer_tip13 u8"Wydruk wszystkich warstw w odcieniach szarości"
#define layer_tip14 u8"Wydruk wszystkich warstw w trybie czarno-białym"
#define layer_tip15 u8"Wszystkie warstwy"
#define layer_tip16 u8"Usuń warstwę"
#define layer_tip17 u8"Zaznacz blok warstw"

#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "T"
#define _NO__ "N"
#define _Yes_ u8"Tak"
#define _No_ u8"Nie"
#define _ERASE_LAYER_ u8"OSTRZEŻENIE: Czy chcesz usunąć całą warstwę wraz z całą zawartością?"
#define _FROM_ u8"Od"
#define _TO_ u8"Do"
#endif

#ifdef __CHPROP__

#define NoWidthLines	6 /*5*/ /*3*/
#define NoTypeLines	32/*5*/
#define NoTextType 16
#define NoTextJust 4

char* Colors_Name_txt[MAXCOLOR + 1] =
{
  u8"֍czerwony",
  u8"֍żółty" ,
  u8"֍zielony" ,
  u8"֍turkusowy" ,
  u8"֍niebieski" ,
  u8"֍karmazynowy" ,
  u8"֍biały" ,
  u8"֍ciemnoszary" ,
  u8"֍ciemnoczerwony" ,
  u8"֍brązowy" ,
  u8"֍ciemnozielony" ,
  u8"֍ciemnoturkusowy" ,
  u8"֍ciemnoniebieski" ,
  u8"֍ciemnokarmazynowy" ,
  u8"֍szary" ,
  u8"֍czarny" ,
  u8"֍inny>>> ",
  "",
};

#define chprop_comm0 u8"Modyfikacja Cech Obiektów"
#define chprop_comm1 u8"Zmiana"
#define chprop_comm2 u8"inne cechy"

static char NoChange[] = "Bez Zmian";

static char* width_line_txt[NoWidthLines + 1] = { u8"҂b.cienka",u8"҂Cienka",u8"҂Gruba",u8"҂B.Gruba",u8"҂Najgrubsza", u8"҂Wypełnienia", NoChange };

char* type_line_txt[NoTypeLines + 1] = {
				u8"֏Ciągła",
				u8"֏Kreskowa",
				u8"֏Punktowa",
				u8"֏Dwupunktowa",
				u8"֏Wielopunktowa",
				u8"֏Granica",
				u8"֏Środkowa",
				u8"֏Ukryta",
				u8"֏widMo",
				u8"֏kreskowa_2",
				u8"֏punktowa_2",
				u8"֏dwupunktowa_2",
				u8"֏wielopunktow_2",
				u8"֏granica_2",
				u8"֏środkowa_2",
				u8"֏ukryta_2",
				u8"֏widmo_2",
				u8"֏kreskowa_x2",
				u8"֏punktowa_x2",
				u8"֏dwupunktowa_x2",
				u8"֏wielopunkto_x2",
				u8"֏granica_x2",
				u8"֏środkowa_x2",
				u8"֏ukryta_x2",
				u8"֏widmo_x2",
				u8"֏trzypunktowa",
				u8"֏dwurodkowa",
				u8"֏trzykreskowa",
				u8"֏widmo_3",
				u8"֏dwuŚrodkowa_x2",
				u8"֏trzykreskowa_x2",
				u8"֏wielokreskowa",
				NoChange };

static char* tab_type_txt[NoTextType + 1] = {
			   u8"Normalny",
			   u8"Etykieta wew.",
			   u8" etYkieta skł.",
			   u8"Symbol",
			   u8"Typ",
			   u8"? nazwa zacisku*",
			   u8"Zasilanie      *",
			   u8"Port           *",
			   u8"Atrybut",
			   u8"+ schemat",
			   u8"nazwa pLiku+",
			   u8"<> sieć",
			   u8"Komentarz",
			   u8"% zmienna      *",
			   u8"symbol Drabinki*",
			   u8"Opis drabinki  *",
			   NoChange };

static char* font_just_txt[NoTextJust + 1] = {
					u8"do Lewej",
					u8"do Prawej",
					u8"Środkowo",
					u8"Centralnie",
					NoChange };

static char* translucency_txt[20 + 1] = {
					u8"100%",
					u8"95%",
					u8"90%",
					u8"85%",
					u8"80%",
					u8"75%",
					u8"70%",
					u8"65%",
					u8"60%",
					u8"55%",
					u8"50%",
					u8"45%",
					u8"40%",
					u8"35%",
					u8"30%",
					u8"25%",
					u8"20%",
					u8"15%",
					u8"10%",
					u8"5%",
					NoChange };

#define chprop_tips0 u8"@Warstwa"
#define chprop_tips1 u8"Kolor"
#define chprop_tips2 u8"Grubość linii"
#define chprop_tips3 u8"Typ linii"
#define chprop_tips4 u8"Typ czcionki"
#define chprop_tips5 u8"Typ tekstu"
#define chprop_tips6 u8"Ukryty tekst"
#define chprop_tips7 u8"Adjustacja tekstu"
#define chprop_tips8 u8"Wysokość czcionki"
#define chprop_tips9 u8"Szerokość czcionki"
#define chprop_tips10 u8"Czcionka pochylona (italik)"
#define chprop_tips11 u8"Czcionka pogrubiona"
#define chprop_tips12 u8"Podkreślenie tekstu"
#define chprop_tips13 u8"Tak"
#define chprop_tips14 u8"Nie"
#define chprop_tips15 u8"Odwróć horizontalnie"
#define chprop_tips16 u8"Odwróć wertykalnie"
#define chprop_tips17 u8"Na wierzchu"
#define chprop_tips18 u8"Nieprzezroczystość wielokątów, śladów i kreskowania jednolitym kolorem"

char* add_new_font_c = u8"dodaj nową czcionkę";

#endif

#ifdef __LOADF__

static char copy_button[] = u8"Kopiuj\0z pliku";
static char parametry_p[] = u8"Parametry:";
static char edycja_n_p[] = u8"Nazwa parametru:";
static char edycja_p[] = u8"Wartość parametru:";
static char lista_p[] = u8"Lista parametrów:";
static char nowy_katalog[] = u8"nowy katalog";
static char usun_zbior[] = u8"usuń zbiór";
static char usun_katalog[] = u8"usuń katalog";

char* dlg_name[] =
{
   u8"Otwarcie rysunku ze zbioru",
   u8"Zapis rysunku do zbioru",
   u8"Import bloku",
   u8"Import zbioru obrazu",
   u8"Konwersja pliku obraz na ALX i import",
   u8"Eksport PCX",
   u8"Export BMP",
   u8"Export PNG",
   u8"Export JPEG",
   u8"Export TIFF",
   u8"Export EPS",
   u8"Export PDF",
   u8"Import profile",
   u8"Import 3D object",
   u8"Eksport bloku",
   u8"Konwersja rysunków do nowego formatu",
   u8"Konwersja bloków do nowego formatu",
   u8"Edycja tabliczki",
   u8"Konwersja zbioru z formatu DXF",
   u8"Zapis zbioru w formacie DXF",
   u8"Zapis zbioru makra",
   u8"Odczyt zbioru makra",
   u8"Zapis konfiguracji drukarki",
   u8"Odczyt konfiguracji drukarki",
   u8"Zapis zbioru katalogu",
   u8"Odczyt zbioru katalogu",
   u8"Zapis zbioru współrzędnych",
   u8"Zapis tras sieci",
   u8"Odczyt trasy ze zbioru współrzędnych",
   u8"Otwórz katalog",
   u8"Zapis konfiguracji plotera",
   u8"Odczyt konfiguracji plotera",
   u8"Import bloku DXF",
   u8"Konwersja zbioru z formatu DWG",
   u8"Zapis zbioru w formacie DWG",
   u8"Import bloku DWG",
   u8"Import czcionki pulpitu",
   u8"Import nowej czcionki",
   u8"Zapis pliku wyników analizy statycznej",
};

#define nameplate_tip u8"@Tabliczka rysunku (Identyfikator)"
#define  edit_table u8"Edycja tabliczki"

#define _BLOCK_DESCRIPTION_ u8"Opis bloku:"
#define _BLOCK_TYPE u8"Typ bloku:"

#define _Yes_ u8"Tak"
#define _No_ u8"Nie"

#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'

#define _FILE_EXISTS_ u8"Plik istnieje"
#define _OVERWRITE_IT_ "Nadpisac go? "

#endif // __LOADF__

#ifdef __INFO__
/*
char* typ_punktu_tab[] = { u8"Normalny",u8"punkt Bazowy",u8"Pin","?","?","?","?","?","?",
						"?","?","?","?","?","?","?" };
*/
char* vector_style_tab[] = { u8"sztywny-sztywny",u8"sztywny-przegubowy",u8"przegubowy-sztywny",u8"przegubowy-przegubowy",
                            u8"Siła",u8"Moment+",u8"Moment-",
                            u8"Przemieszczenie", u8"Obrót+", u8"Obrót-",
                            u8"trapezowe obciążenie pionowe",
                            u8"trapezowe obciążenie poziome",
                            u8"trapezowe obciążenie prostopadłe",
                            u8"rzut poziomy obciążenia pionowego",
                            u8"rzut pionowy obciążenia poziomego",
                            u8"obciążenie termiczne",
                            u8"rozmiar węzła (promień)",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
 };


char* view_width_tab1[] = { u8"bardzo cienka",u8"cienka",u8"gruba",u8"bardzo gruba",u8"najgrubsza",u8"",u8"",u8"niweidoczna" };

char* tab_typ_tekstu[] = { u8"Normalny",u8"Etykieta wew.",u8" etYkieta skł.",
						 u8"Symbol","Typ",u8"? nazwa zacisku *",u8"Zasilanie *",
						 u8"Port *",u8"Atrybut",u8"+ schemat",u8"nazwa pLiku+",u8"<> sieć",
						 u8"Komentarz",u8"% zmienna *",u8"symbol Drabinki*",u8"Opis drabinki *" };

char* tab_justowanie[] = { u8"do Lewej |<",u8"do Prawej >|",u8"śodkowo ><",u8"Centralnie _><_" };

static POLE pmInfoAbout[] = {
	 {u8"typ Obiektu\0 ",'O',320,NULL},      //0
	 {u8"Warstwa\0 ",'W',305,NULL},      //1
	 {u8"֎Kolor\0 ",'K',158,NULL},      //2
	 {u8"Typ linii\0 ",'T',160,NULL},      //3
	 {u8"Grubość linii\0 ",'G',159,NULL},      //4
     {u8"typ punktu\0 ",'.',27,NULL},       //5
     {u8"typ wektora\0",'V',770,NULL},      //6
	 {u8"X1 \0",'1',306,NULL},      //7
	 {u8"Y1 \0",'2',307,NULL},      //8
	 {u8"X2 \0",'3',308,NULL},      //9
	 {u8"Y2 \0",'4',309,NULL},      //10
	 {u8"X3 \0",'5',310,NULL},      //11
	 {u8"Y3 \0",'6',311,NULL},      //12
	 {u8"X4 \0",'7',312,NULL},      //13
	 {u8"Y4 \0",'8',313,NULL},      //14
	 {u8"pRomień \0 ",'R',212,NULL},      //15
     {u8"pRomień Y \0 ",'r',710,NULL},      //16
     {u8"Kąt \0 ",'@',107,NULL},      //17
	 {u8"Kąt 1 \0 ",'<',314,NULL},      //18
	 {u8"Kąt 2 \0 ",'>',315,NULL},      //19

     {u8"Szerokość początkowa \0",L'-',249,NULL},      //20
     {u8"Szerokość końcowa \0 ",L'=',249,NULL},      //21
      {u8"Intensywność \0 ",'^',727,NULL},      //22
     {u8"Intensywność początkowa \0 ",'(',766,NULL},      //23
     {u8"Intensywność końcowa \0 ",')',767,NULL},      //24
     {u8"Początkowe przesunięcie osi \0 ",L'/',250,NULL},      //25
     {u8"Końcowe przesunięcie osi \0",L'\\',250,NULL},      //26

     {u8"Character i wariant obciążenia\0 ",'?',798,NULL},     //27
     //{u8"Wariant \0 ",'?',799,NULL},     //28

     {u8"Opacity\0 ",'%',542,NULL},      //29
	 {u8"Długość (obwód)\0 ",'D',210,NULL},      //30
	 {u8"DX\0",'X',316,NULL},      //31
	 {u8"DY\0",'Y',317,NULL},      //32
	 {u8"Pole powierzchni\0",'P',7,NULL},        //33
	 {u8"Czcionka\0",'C',111,NULL},      //34
	 {u8"Typ\0 ",'E',231,NULL},      //35
	 {u8"Ukryty\0",'U',232,NULL},      //36
	 {u8"Justowanie\0",'J',233,NULL},      //37
	 {u8"Wysokość \0",'H',179,NULL},      //38
	 {u8"Szerokość wsp. \0",'*',230,NULL},      //39
	 {u8"Pochylone\0 ",'I',181,NULL},      //40
     {u8"pogruBiony\0",'B',182,NULL},      //41
	 {u8"podkreśLony\0 ",'L',409,NULL},      //42
	 {u8"Odstep linii\0",'#',410,NULL},      //43
     {u8"wielkość punktu dx \0",'Q',689,NULL},      //44
     {u8"wielkość punktu dy \0",'Z',690,NULL},      //45
	 {u8"Nazwa bloku wewn.\0",'N',318,NULL},      //46
	 {u8"Nazwa bloku zewn.\0",'B',319,NULL},      //47
};
TMENU mInfoAbout = { 47,0,0,40,2, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmInfoAbout, NULL, NULL };

static POLE pmInfo[] = {
	 {u8"pamięć RAM dla rysunku/domyślna [KB] \0 ",'R',297,NULL},
	 {u8"Pamięć wirtualna obrazu [KB] \0 ",'P',298,NULL},
	 {u8"Wielkość rysunku [ B] \0",'W',299,NULL},
	 {u8"Bufor wydruku [KB] \0 ",'B',300,NULL},
	 {u8"bufor Makra [B] \0",'M',301,NULL},
	 {u8"bufor obrazu / płat ekranu [KB] \0",'X',302,NULL},
	 {u8"Aktualny katalog \0 ",'A',303,NULL},
	 {u8"Tryb graficzny \0 ",'T',304,NULL},
	 {u8"Czcionka pulpitu \0 ",'C',111,NULL},
     {u8"Logo \0 ",'L',812,NULL},
};

TMENU mInfo = { 10,0,0,64,1, 3, ICONS | TADD, CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmInfo, NULL, NULL, NULL };

char* objects[] = { u8"Linia",u8"Linia 3D",u8"Tekst",u8"Łuk",u8"Okrąg",u8"Koło",u8"Wielokąt/Obszar",u8"Obszar 3D",u8"Punkt",u8"Spline",u8"Obraz",u8"Polilinia",u8"Ślad",u8"Kreskowanie",u8"Łuk Eliptyczny",u8"Elipsa",u8"Dysk Elliptyczny", u8"Obszar łukowy", u8"Wektor"};

#define _FILLING_ u8"wypełnienie"

char *vector_txt[]={u8"Wektor: sztywno-sztywny",u8"Wektor: sztywno-przegubowy",u8"Wektor: przegubowo-sztywny",u8"Wektor: przegubowo-przegubowy",u8"Wektor: Siła",u8"Wektor: Moment" ,u8"Wektor: -Moment",
 u8"Wektor: Przemieszczenie",u8"Wektor: Obrót",u8"Wektor: -Obrót",u8"Wektor: obciążenie trapezowe Y",u8"Wektor: obciążenie trapezowe X",u8"Wektor: obciążenie trapezowe N",u8"Wektor: obciążenie trapezowe H",u8"Wektor: obciążenie trapezowe V",u8"Wektor: obciążenie termiczne", u8"Wektor: Rozmiar węzła (promień)"};

//char *point_txt[]={u8"Normalny",u8"punkt Bazowy",'','','','','',u8"połączenie",u8"Zacisk",'','','',u8"Utwierdzenie",u8"utwierdzenie L",u8"utwierdzenie P",u8"utwierdzenie G",u8"Przegubowe",
 //    u8"przegubowe L",u8"przegubowe P",u8"przegubowe G",u8"utwierdzenie przesuwne poziomo",u8"utwierdzenie przesuwne pionowo L",u8"utwierdzenie przesuwne pionowo P",u8"utwierdzenie przesuwne poziomo G",
  //   u8"przegubowe przesuwne poziomo",u8"przegubowe przesuwne pionowo L",u8"przegubowe przesuwne pionowo P",u8"przegubowe przesuwne poziomo G",

unsigned short vector_wcod[]={L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', L'Y', L'X', L'N', L'V', L'H', L'Q', L'R'};
unsigned short point_wcod[]={L'N', L'B', ' ', ' ', ' ', ' ',' ', L'J', L'Z', ' ', ' ', ' ',L'U', L'1', L'2', L'3', L'P', L'3', L'4', L'5', L'R', L'7', L'8', L'9', L'O', '0', '-', '=', '+'};
unsigned short object_wcod[]={L'I', L'L', L'T', L'U', L'O', L'D', L'S', L'P', L'*', L'G', L'C', L'E', L'F', 'B', L'W', ' '};
char *object_txt[]={u8"Obraz", u8"Linia", u8"Tekst", u8"Łuk", u8"Okrąg", u8"Dysk", u8"Obszar", u8"Punkt", u8"Blok", u8"Obszar łukowy",u8"Łuk eliptyczny",u8"Elipsa",u8"Wypełniona elipsa",u8"Splajn Beziera",u8"Wektor",""};


#endif

#ifdef __ALLEGROEXT__

static char* desktop_data_param[] =
{
  "Tapeta",
  "Panorama",
  "Dynamiczne menu",
  "Kursor",
  "Styl menu",
  "Instrukcja",
};

static int no_desktop_data_param = sizeof(desktop_data_param) / sizeof(desktop_data_param[0]);

static char* desktop_data_param_comment[] =
{
	";",
	//";min or 0 (off)",
	";faktor autopanoramy",
	";",
	(char*)u8"; mały 0, duży 1",
    (char*)u8"; ramka 0, wskaźnik 1",
    (char*)u8"; wyświetlanie krótkiej instrukcji na starcie",
};


#define _UNABLE_INSTALL_GRAPHIC_ u8"Nie można zainicjować trybu graficznego"
#define _ERROR_READING_DAT_ u8"Błąd odczytu pliku: alfacad.dat\n"

#define _MACRO_C__ L'm'
#define _EXECUTE_C__ L'w'
#define _Go_AHEAD_ u8"OK. Zaczynajmy."

#define _UPGRADE_READY_ u8"Aktualizacja jest gotowa do pobrania i zainstalowania"
#define _UPGRADE_RESTART_ u8"Czy chcesz teraz dokonać aktualizacji i ponownie uruchomić?"
#define _TOOLS_TO_INSTALL_ u8"nie jest zainstalowane"
#define _INSTALL_TOOLS_ u8"Zainstaluj niezbędne pakiety"

char _Yes_[]="Tak";
char _No_[]="Nie";
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'

#endif


#ifdef __O_LINE__

static POLE pmAxis1000[] = {
		  {u8"Długość osi\0      \0",'D',267,NULL} };

static POLE pmAxis50[] = {
		  {u8"Długość osi\0      \0",'D',267,NULL} };

static POLE pmLine[] = {
		  {u8"Zamknij\0",'Z',217,NULL},
		  {u8"Usuń\0Del\0",'U',218,NULL},
		  {u8"Długość\0             0\0",'D',219,NULL} };

static POLE pmPLine[] = {
		  {u8"Zamknij\0",'Z',217,NULL},
		  {u8"Usuń\0Del\0",'U',218,NULL},
		  {u8"Długość\0             0\0",'D',219,NULL},
		  {u8"Łuk\0", 'L', 15,NULL} };

static POLE pmLine_Con[] = {
		  {u8"Kontynuacja",'K',220,NULL} };

static POLE pmPLineObrys[] = {
		  {u8"Zamknij\0",'Z',217,NULL},
		  {u8"Usuń\0Del\0",'U',218,NULL},
		  {u8"Długość\0 0\0",'D',219,NULL},
		  {u8"Łuk\0", 'L', 15,NULL},
		  {u8"l. Przekrojowa\0    \0",'P',268,NULL},
		  {u8"l. Niewidoczna\0    \0",'N',269,NULL} };

static POLE pmLine_ConObrys[] = {
		  {u8"Kontynuacja",'K',217,NULL},
		 {u8"l. Przekrojowa\0    \0",'P',268,NULL},
		 {u8"l. Niewidoczna\0    \0",'N',269,NULL} };

#define _AXIS_ u8"Oś"
#define _AXIS_C_ L'O'
#define _AXIS_H_ u8"Oś #"
#define _AXIS_H_C_ L'O'
#define _LINE_ u8"Linia"
#define _LINE_C_ L'L'

#define _LINE_3D_ u8"Linia3D"

#define _POLYLINE_ u8"Polilinia"
#define _POLYLINE_C_ L'P'

#define _POLYLINE_3D_ u8"Polilinia3D"
#define _POLYLINE_3D_C_ L'P'

#endif

#ifdef __O_VECTOR__


POLE pmLoadVariant[] = {
        {u8"",L'0',0,NULL},
        {u8"1",L'1',0,NULL},
        {u8"2",L'2',0,NULL},
        {u8"3",L'3',0,NULL},
        {u8"4",L'4',0,NULL},
        {u8"5",L'5',0,NULL},
        {u8"6",L'6',0,NULL},
        {u8"7",L'7',0,NULL},
        {u8"8",L'8',0,NULL},
        {u8"9",L'9',0,NULL},
        {u8"10",L'A',0,NULL},
        {u8"11",L'B',0,NULL},
        {u8"12",L'C',0,NULL},
        {u8"13",L'D',0,NULL},
        {u8"14",L'E',0,NULL},
        {u8"15",L'F',0,NULL},
        {u8"16",L'G',0,NULL},
        {u8"17",L'H',0,NULL},
        {u8"18",L'I',0,NULL},
        {u8"19",L'J',0,NULL},
        {u8"20",L'K',0,NULL},
        {u8"21",L'L',0,NULL},
        {u8"22",L'M',0,NULL},
        {u8"23",L'N',0,NULL},
        {u8"24",L'O',0,NULL},
        {u8"25",L'P',0,NULL},
        {u8"26",L'Q',0,NULL},
        {u8"27",L'R',0,NULL},
        {u8"28",L'S',0,NULL},
        {u8"29",L'T',0,NULL},
        {u8"30",L'U',0,NULL},
        {u8"31",L'V',0,NULL},
        {u8"32",L'W',0,NULL},
        {u8"33",L'X',0,NULL},
        {u8"34",L'Y',0,NULL},
        {u8"35",L'Z',0,NULL},
        };

//static
TMENU mLoadVariant = { 36, 0,0, 3, 88, 12, 0, CMNU,CMBR,CMTX,0, 30, 0, 0,0,(POLE(*)[]) &pmLoadVariant, NULL, NULL };

POLE pmLoad_Char[] = {
	{u8"nieustalone",L'0',797,NULL},
	{u8"obciążenie Ciężarem własnym",L'C',787,&mLoadVariant},
    {u8"obciążenie Użytkowe",L'U',788,&mLoadVariant},
    {u8"obciążenie Montażowe",L'M',789,&mLoadVariant},
    {u8"obciążenie Wiatrem",L'W',790,&mLoadVariant},
    {u8"obciążenie Śniegiem",L'S',791,&mLoadVariant},
    {u8"obciążenie sejsmiczne",L'E',792,&mLoadVariant},
    {u8"obciążenie Deszczem i lodem",L'D',793,&mLoadVariant},
    {u8"obciążenie Parciem gruntu",L'P',794,&mLoadVariant},
    {u8"obciążenie Hydrauliczne cieczami",L'H',795,&mLoadVariant},
    //{u8"obciążenie Termiczne",L'T',796,&mLoadVariant},
};

//static
TMENU mLoad_Char = { 10, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 18, 0, 0,0,(POLE(*)[]) &pmLoad_Char, NULL, NULL };

POLE pmLoad_Char_Thermal[] = {
	{u8"nieustalone",L'0',797,NULL},
    {u8"obciążenie Termiczne",L'T',796,&mLoadVariant},
};

//static
TMENU mLoad_Char_Thermal = { 2, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 18, 0, 0,0,(POLE(*)[]) &pmLoad_Char_Thermal, NULL, NULL };


static POLE pmVector[] = {
		  {u8"Usuń\0Del\0",L'U',218,NULL},
          {u8"sztywno - sztywny\0\0",L'A',723,NULL},
          {u8"sztywno - przegubowy\0\0",L'B',724,NULL},
          {u8"przegubowo - sztywny\0\0",L'C',725,NULL},
          {u8"przegubowo - przegubowy\0\0",L'D',726,NULL},
          {u8"Siła\0\0",L'S',727,&mLoad_Char},
          {u8"Moment siły\0\0",L'M',728,&mLoad_Char},
          {u8"-moment siły\0\0",L'-',729,&mLoad_Char},
          {u8"Przesunięcie\0\0",L'P',730,NULL}, //&mLoad_Char}, //NULL
          {u8"Obrót\0\0",L'O',731,NULL}, //&mLoad_Char}, //NULL
          {u8"-obrót\0\0",L'R',732,NULL}, //&mLoad_Char}, //NULL
          {u8"obciążenie trapezowe Y\0\0",L'Y',733,&mLoad_Char},
          {u8"obciażenie trapezowe X\0\0",L'X',734,&mLoad_Char},
          {u8"obciażenie trapezowe N\0\0",L'N',735,&mLoad_Char},
          {u8"obciażenie trapezowe H\0\0",L'H',736,&mLoad_Char},
          {u8"obciażenie trapezowe V\0\0",L'V',737,&mLoad_Char},
          {u8"obciążenie Termiczne\0\0",L'T',752,&mLoad_Char_Thermal},
          {u8"Rozmiar węzła (promień)\0\0",L'0',786,NULL},
};

static POLE pmVector_Con[] = {
		  {u8"Kontynuacja",L'K',220,NULL} };

static POLE pmMember_style[] = {
          {u8"sztywno - sztywny\0\0",L'A',723,NULL},
          {u8"sztywno - przegubowy\0\0",L'B',724,NULL},
          {u8"przegubowo - sztywny\0\0",L'C',725,NULL},
          {u8"przegubowo - przegubowy\0\0",L'D',726,NULL},
};

static POLE pmForce_Displacement_style[] = {
          {u8"Siła\0\0",L'S',727,NULL},
          {u8"Przesunięcie\0\0",L'P',730,NULL},
};

static POLE pmMoment_Rotation_style[] = {
          {u8"Moment siły\0\0",L'M',728,NULL},
          {u8"-moment siły\0\0",L'-',729,NULL},
          {u8"Obrót\0\0",L'O',731,NULL},
          {u8"-obrót\0\0",L'R',732,NULL},
};

static POLE pmLoad_style[] = {
          {u8"obciążenie trapezowe Y\0\0",L'Y',733,NULL},
          {u8"obciażenie trapezowe X\0\0",L'X',734,NULL},
          {u8"obciażenie trapezowe N\0\0",L'N',735,NULL},
          {u8"obciażenie trapezowe H\0\0",L'H',736,NULL},
          {u8"obciażenie trapezowe V\0\0",L'V',737,NULL},
};

#define _AXIS_ u8"Oś"
#define _AXIS_C_ L'O'
#define _AXIS_H_ u8"Oś #"
#define _AXIS_H_C_ L'O'
#define _VECTOR_ u8"Wektor"
#define _VECTOR_C_ L'V'

#endif


#ifdef __O_LUK__

#define _YES_ "T"
#define _NO_ "N"

static TMENU mArcReversed = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0,14,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };


static POLE pmLukm[] = {
		  {u8"[1] trzy punkty",'1',221,NULL},
		  {u8"[2] początek-środek-koniec",'2',222,NULL},
		  {u8"[3] początek-środek-kąt",'3',223,NULL},
		  {u8"[4] początek-środek-cięciwa",'4',224,NULL},
		  {u8"[5] początek-koniec-promień",'5',225,NULL},
		  {u8"[6] początek-koniec-kąt",'6',226,NULL},
		  {u8"[7] początek-koniec-kierunek",'7',227,NULL},
		  {u8"[8] kontynuacja",'8',228,NULL},
          {u8"odwrócony kierunek łuku",L'0',729,&mArcReversed}};

static POLE pmPLukm[] = {
		  {u8"[1] trzy punkty",'1',221,NULL},
		  {u8"[2] początek-środek-koniec",'2',222,NULL},
		  {u8"[3] początek-środek-kąt",'3',223,NULL},
		  {u8"[4] początek-środek-cięciwa",'4',224,NULL},
		  {u8"[5] początek-koniec-promień",'5',225,NULL},
		  {u8"[6] początek-koniec-kąt",'6',226,NULL},
		  {u8"[7] początek-koniec-kierunek",'7',227,NULL},
		  {u8"[8] kontynuacja",'8',228,NULL},
          {u8"odwrócony kierunek łuku",L'0',729,&mArcReversed},
		  {u8"Zamknij",'Z',247,NULL},
		  {u8"Usuń: Del",'U',248,NULL},
		  {u8"Linia", 'L',13,NULL} };

static POLE pmPLukmObrys[] = {
		  {u8"[1] trzy punkty\0",'1',221,NULL},
		  {u8"[2] początek-środek-koniec\0",'2',222,NULL},
		  {u8"[3] początek-środek-kąt\0",'3',223,NULL},
		  {u8"[4] początek-środek-cięciwa\0",'4',224,NULL},
		  {u8"[5] początek-koniec-promień\0",'5',225,NULL},
		  {u8"[6] początek-koniec-kąt\0\0",'6',226,NULL},
		  {u8"[7] początek-koniec-kierunek\0",'7',227,NULL},
		  {u8"[8] kontynuacja\0",'8',228,NULL},
          {u8"odwrócony kierunek łuku",L'0',729,&mArcReversed},
		  {u8"Zamknij\0",'Z',247,NULL},
		  {u8"Usuń\0Del\0",'U',248,NULL},
		  {u8"Linia\0", 'L', 13,NULL},
		  {u8"l. Przekrojowa\0     \0",'P',268,NULL},
		  {u8"l. Niewidoczna\0     \0",'N',269,NULL} };

#define _ARC_ u8"Łuk"
#define _ARC_C_ L'L'

#endif

#ifdef __O_OKRAG__

static const char CIRCLE[] = u8"Okrąg";
static const char CIRCLE_FILL[] = u8"Koło";

static POLE pmCircle[] = {
	 {u8"Środek-promień\0",'S',213,NULL},
	 {u8"śRodek-średnica\0",'R',214,NULL},
	 {u8"2 punkty\0",'2',215,NULL},
	 {u8"3 punkty\0",'3',216,NULL},
	 {u8"sTyczny\0",'T',266,NULL},
};

#define _CIRCLE_C_ L'O'
#define _DISC_C_ L'K'

#endif

#ifdef __O_POLYGN__

static POLE pmPolygon[] = {
	 {u8"Krawędź\0",L'K',235,NULL},
	 {u8"Wpisany w okrąg\0", L'W',236, NULL},
	 {u8"Opisany na okręgu\0 ",L'O',237,NULL},
	 {u8"liczba boków\0    \0",L'N',606,NULL},
};

#define _POLYGON_ u8"wieloKąt"
#define _POLYGON_C_ L'K'

#endif

#ifdef __O_ELLIPS__

static POLE pm_ell__draw_type[] = {
		{u8"Początek osi",L'P',240,NULL},
		{u8"Centrum",L'C',241,NULL},
};

static TMENU m_ell__draw_type = { 2,0,0,13,79,7,ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pm_ell__draw_type,NULL,NULL };

static POLE pm_ell_y_draw_method[] = {
		{u8"Dystans",L'D',711,NULL},
		{u8"Punkt",L'P',712,NULL},
        {u8"Stycznie",L'S',713,NULL},
};

static TMENU m_ell_y_draw_method = { 3,0,0,13,79,7,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pm_ell_y_draw_method,NULL,NULL };


POLE pm__ellipse[] = {
		//{u8"Aproksymacja\0 20\0  ",L'A',238, NULL},
        {u8"Metoda drugiej półosi\0 Dystans",L'M',710, &m_ell_y_draw_method},
		{u8"początek osi/Centrum\0  początek osi",L'C',239, &m_ell__draw_type},
					};

static POLE pmTranslucencyFE[] = {
   {u8"100%",'0',546,NULL},
   {u8"95%",'1',674,NULL},
   {u8"90%",'2',545,NULL},
   {u8"85%",'3',673,NULL},
   {u8"80%",'4',544,NULL},
   {u8"75%",'5',672,NULL},
   {u8"70%",'6',543,NULL},
   {u8"65%",'7',671,NULL},
   {u8"60%",'8',542,NULL},
   {u8"55%",'9',670,NULL},
   {u8"50%",'A',541,NULL},
   {u8"45%",'B',669,NULL},
   {u8"40%",'C',540,NULL},
   {u8"35%",'D',668,NULL},
   {u8"30%",'E',539,NULL},
   {u8"25%",'F',667,NULL},
   {u8"20%",'G',666,NULL},
   {u8"15%",'H',665,NULL},
   {u8"10%",'I',664,NULL},
   {u8"5%",'J',663,NULL},

};

TMENU mTranslucencyFE = { 20,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) & pmTranslucencyFE,NULL,NULL };

POLE pm__ellipse_d[] = {
		//{u8"Aproksymacja\0 20\0  ",L'A',238, NULL},
        {u8"Metoda drugiej półosi\0 Dystans",L'M',710, &m_ell_y_draw_method},
		{u8"Nieprzezroczystość\0",'N',542,&mTranslucencyFE},
					};

#define _ELLIPSE_ u8"Elipsa"
#define _ELLIPSE_C_ L'E'

POLE pm__ellipticalarc[] = {
		{u8"kąt we współrzędnych Llobalnych",L'L',708, NULL},
		{u8"kąt we współrzędnych Globalnych",L'G',709, NULL},
					};

#define _ELLIPSE_ANGLE_ u8"Kąt"
#define _ELLIPSE_ANGLE_C_ L'K'

#define _ELLIPTICALDISC_ u8"Dysk"
#define _ELLIPTICALDISC_C_ L'D'

static char* EllipticalAngle[] = {u8"Localny",u8"Globalny"};

#endif

#ifdef __O_PLINE__

#endif

#ifdef __O_SOLID__

static char filltyp_tab[7][5] = { u8"W","b.c.","c.","g.","b.g.","ng.","O" };

static POLE pmS34[] = {
		{u8"Trójkąty",'T',246,NULL},
		{u8"Czworokąty",'C',245,NULL},
};

static TMENU mS34 = { 2,0,0,11,79,6,ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) & pmS34,NULL,NULL };

static POLE pmPatternDxy[] = {
	   {u8"0",'0',0,NULL},
		{u8"1/32",'1',0,NULL},
		{u8"1/16",'2',0,NULL},
		{u8"1/8",'3',0,NULL},
		{u8"1/4",'4',0,NULL},
		{u8"3/8",'5',0,NULL},
		{u8"1/2",'6',0,NULL},
		{u8"5/8",'7',0,NULL},
		{u8"3/4",'8',0,NULL},
		{u8"7/8",'9',0,NULL},
};

TMENU mPatternDx = { 10,0,0,4,79,6,0,CMNU,CMBR,CMTX,0,18,0,0,0,(POLE(*)[]) &pmPatternDxy,NULL,NULL };

TMENU mPatternDy = { 10,0,0,4,79,6,0,CMNU,CMBR,CMTX,0,28,0,0,0,(POLE(*)[]) &pmPatternDxy,NULL,NULL };


static POLE pmPatternScale[] = {
		{u8"0.125",'0',0,NULL},
		{u8"0.25",'-',0,NULL},
		{u8"0.5",'5',0,NULL},
		{u8"1.0",'1',0,NULL},
		{u8"2.0",'2',0,NULL},
		{u8"4.0",'4',0,NULL},
		{u8"8.0",'8',0,NULL},
};
//static
TMENU mPatternScale = { 7,0,0,5,79,6,0,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmPatternScale,NULL,NULL };

static POLE pmPatternAngle[] = {
		{u8"0°",'0',578,NULL},
		{u8"90°",'9',442,NULL},
		{u8"180°",'8',444,NULL},
		{u8"270°",'7',446,NULL},
};
//static
TMENU mPatternAngle = { 4,0,0,4,79,6,ICONS,CMNU,CMBR,CMTX,0,14,0,0,0,(POLE(*)[]) &pmPatternAngle,NULL,NULL };


static POLE pmSolidPattern[] = {
		{u8"Wzór\0 ",'W',597,NULL},
		{u8"Skala\0 1.0",'S',436,&mPatternScale},
		{u8"Obrót\0 0",'O',35,&mPatternAngle},
		{u8"dX\0 0",'X',622,&mPatternDx},
		{u8"dY\0 0",'Y',623,&mPatternDy},
		{u8"Pobierz wzór\0",'P',164,NULL},
		{u8"Zmień istniejący wzór\0",'P',599,NULL},
};

static TMENU mSolidPattern = { 7,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmSolidPattern,NULL,NULL };

static POLE pmTranslucency[] = {
   {u8"100%",'0',546,NULL},
   {u8"95%",'1',673,NULL},
   {u8"90%",'2',545,NULL},
   {u8"85%",'3',672,NULL},
   {u8"80%",'4',544,NULL},
   {u8"75%",'5',671,NULL},
   {u8"70%",'6',543,NULL},
   {u8"65%",'7',670,NULL},
   {u8"60%",'8',542,NULL},
   {u8"55%",'9',669,NULL},
   {u8"50%",'A',541,NULL},
   {u8"45%",'B',668,NULL},
   {u8"40%",'C',540,NULL},
   {u8"35%",'D',667,NULL},
   {u8"30%",'E',539,NULL},
   {u8"25%",'F',666,NULL},
   {u8"20%",'G',667,NULL},
   {u8"15%",'H',666,NULL},
   {u8"10%",'I',665,NULL},
   {u8"5%",'J',664,NULL},

};

TMENU mTranslucency = { 20,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,45,0,0,0,(POLE(*)[]) & pmTranslucency,NULL,NULL };


static POLE pmFillTyp[] = {
	   {u8"Wypełnienie\0",'W',276,&mTranslucency},
	   {u8"bArdzo cienka\0",'A',277,NULL},
	   {u8"Cienka\0",'C',278,NULL},
	   {u8"Gruba\0",'G',279,NULL},
	   {u8"Bardzo Gruba\0",'B',280,NULL},
	   {u8"Najgrubsza\0",'N',281,NULL},
	   {u8"Obraz\0",'O',617,&mSolidPattern},
	   //  {u8"- wyłącz obszar",'-',0,NULL},
	   //  {u8"+ Aktywny obszar",'+',0,NULL},
};

static TMENU mFillTyp = { 7,0,0,15,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,5,0,0,0,(POLE(*)[]) &pmFillTyp,NULL,NULL };

POLE pmSolid[] = {
		{u8"Czworokąty/trójkąty\0  4",'C',242,&mS34},
		{u8"Trójkąt\0",'T',243,NULL},
	  {u8"Wypełnienie/typ linii\0 \0     ",'W',244,&mFillTyp},
};

#define _SOLID_ u8"Obszar"
#define _SOLID_3D_ u8"Obszar 3D"
#define _SOLID_C_ L'O'

#endif

#ifdef __O_SPLINE__

#define _Q_ "K"
#define _C_ "S"
#define _M_ "W"
#define _B_ "B"

//char _YES__[4] = u8"T";
//char _NO__[4] = u8"N";

static POLE pmS34[] = {
		{u8"Kwadratowa krzywa Bézier'a",'K',520,NULL},
		{u8"Sześcienna krzywa Bézier'a",'S',519,NULL},
		{u8"Wielopunktowy splajn kardynalny",'W',646,NULL},
		{u8"splajn Bézier'a",'B',662,NULL}
};

static TMENU mS34 = { 4,0,0,11,79,6,ICONS,CMNU,CMBR,CMTX,0,8,0,0,0,(POLE(*)[]) &pmS34,NULL,NULL };

static TMENU mD_T_Pline = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmSpline[] = {
		{u8"Sześcienna/kwadratowa/wielopunktowa\0  S\0",'S',517,&mS34},
		{u8"Kwadratowa tym razem\0\0",'K',518,NULL},
		{u8"Polilinia\0 T\0        ",'P',251,&mD_T_Pline},
		{u8"Usuń\0Del\0",'U',218,NULL},
		{u8"kontynuacja\0\0",'+',228,NULL},
		{u8"Dokończ krzywą\0End\0",'D',649,NULL},
		{u8"Napięcia\0\0",'N',648,NULL},
		{u8"Zamknij wielopunktową krzywą  Home\0 N\0",'Z',647,NULL},
};

#define _SPLINE_ u8"Spline"
#define _SPLINE_C_ L'S'

#define _YES__ u8"T"
#define _NO__ u8"N"

#endif

#ifdef __O_TRACE__

extern TMENU mPatternScale;
extern TMENU mPatternAngle;
extern TMENU mPatternDx;
extern TMENU mPatternDy;
extern TMENU mTranslucency;

static char tracefilltyp_tab[7][5] = { u8"W","b.c.","c.","g.","b.g.","ng.","O" };

static TMENU mD_T_Pline = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmTracePattern[] = {
		{u8"Wzór\0 ",L'W',597,NULL},
		{u8"Skala\0 1.0",L'S',436,&mPatternScale},
		{u8"Obrót\0 0",L'O',35,&mPatternAngle},
		{u8"dX\0 0",L'X',622,&mPatternDx},
		{u8"dY\0 0",L'Y',623,&mPatternDy},
		{u8"Pobierz wzór\0",L'P',164,NULL},
		{u8"Zmień istniejący wzór\0",L'P',599,NULL},
};

static TMENU mTracePattern = { 7,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTracePattern,NULL,NULL };

static POLE pmFillTyp[] = {
	   {u8"Wypełnienie",L'W',288,&mTranslucency},
	   {u8"bArdzo cienka",L'A',289,NULL},
	   {u8"Cienka",L'C',290,NULL},
	   {u8"Gruba",L'G',291,NULL},
	   {u8"Bardzo Gruba",L'B',292,NULL},
	   {u8"Najgrubsza",L'N',293,NULL},
	   {u8"Obraz",L'O',617,&mTracePattern},
};

static TMENU mFillTyp = { 7,0,0,15,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,9,0,0,0,(POLE(*)[]) &pmFillTyp,NULL,NULL };

#define _YES_ "T"
#define _NO_ "N"

static TMENU mTraceArcReversed = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0,11,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmPTLukm[] = {
          {u8"[0] kontynuacja łukiem",'0',714,NULL},
		  {u8"[1] trzy punkty",'1',221,NULL},
		  {u8"[2] początek-środek-koniec",'2',222,NULL},
		  {u8"[3] początek-środek-kąt",'3',223,NULL},
		  {u8"[4] początek-środek-cięciwa",'4',224,NULL},
		  {u8"[5] początek-koniec-promień",'5',225,NULL},
		  {u8"[6] początek-koniec-kąt",'6',226,NULL},
		  {u8"[7] początek-koniec-kierunek",'7',227,NULL},
		  //{u8"[8] kontynuacja",'8',228,NULL},
          {u8"odwrócony kierunek łuku",L'0',729,&mTraceArcReversed},
		  //{u8"Zamknij",'Z',247,NULL},
		  //{u8"Usuń: Del",'U',248,NULL},
		  {u8"Linia", 'L',13,NULL},
          {u8"Kontynuacja linią", L'K',715,NULL},
};

static TMENU mPTLukm={11, 0, 0, 30, 56, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,75,0,0,0,&pmPTLukm,NULL,NULL};

static POLE pmFTrace[] = {
	{u8"Szerokość\0 1\0        ",L'S',249,NULL},
	{u8"przesunięcie Osi\0 1\0        ",L'O',250,NULL},
	{u8"Polilinia\0 T\0        ",L'P',251,&mD_T_Pline},
	{u8"Wypełnienie/grubość linii\0 \0     ",L'W',252,&mFillTyp},
	{u8"Usuń\0Del\0 ",L'U',253,NULL},
	{u8"Przerwa\0Home\0 ",L'P',633,NULL},
	{u8"Zamknij\0End\0 ",L'Z',632,NULL},
    {u8"łUk\0\0 ",L'U',15,&mPTLukm},
	};

static POLE pmMLine[] = {
	{u8"Szerokość\0 1\0        ",L'S',249,NULL},
	{u8"przesunięcie Osi\0 1\0        ",L'O',250,NULL},
	{u8"Polilinia\0 T\0        ",L'P',251,&mD_T_Pline},
	{u8"Usuń\0Del\0 ",L'U',253,NULL},
	{u8"Przerwa\0Home\0 ",L'P',633,NULL},
	{u8"Zamknij\0End\0 ",L'Z',632,NULL},
    {u8"łUk\0\0 ",L'U',15,&mPTLukm},
	};

#define _TRACE_ u8"Ślad"
#define _TRACE_C_ L'S'

#define _DLINE_ u8"2-linia"
#define _DLINE_C_ L'2'

#define _TLINE_ u8"3-linia z osią"
#define _TLINE_C_ L'3'

#define _YES__ "T"
#define _NO__ "N"

#define _BREAK_ u8"Przerwa"

#endif

#ifdef __O_SKETCH__

static POLE pmSketch[] = {
	{u8"Minimalny odcinek\0   1mm",L'M',531, NULL},
	{u8"Usuń ostatni\0Del\0 ",L'U',532, NULL},
	};

#define _SKETCH_ u8"Szkic"
#define _SKETCH_C_ L'S'

#define _MM_ u8"mm"

#endif

#ifdef __O_POINT__

#define _POINT_TYPE_ u8"Typ punktu"
#define _POINT_TYPE_C_ L'T'

static POLE pmTyp_punktu[] = {
	 {u8"Normalny\0",'N',294,NULL},
	 {u8"punkt Bazowy\0",'B',295,NULL},
	 {u8"Zacisk\0",'Z',296,NULL},
	 {u8"połączenie\0",'J',529,NULL},
     {u8"Utwierdzenie\0",L'U',738,NULL},
     {u8"utwierdzenie L\0",L'1',739,NULL},
     {u8"utwierdzenie P\0",L'2',740,NULL},
     {u8"utwierdzenie G\0",L'3',754,NULL},
     {u8"Przegubowe\0",L'P',741,NULL},
     {u8"przegubowe L\0",L'4',742,NULL},
     {u8"przegubowe P\0",L'5',743,NULL},
     {u8"przegubowe G\0",L'6',755,NULL},
     {u8"utwierdzenie przesuwne poziomo\0",L'R',744,NULL},
     {u8"utwierdzenie przesuwne pionowo L\0",L'7',745,NULL},
     {u8"utwierdzenie przesuwne pionowo P\0",L'8',746,NULL},
     {u8"utwierdzenie przesuwne poziomo G\0",L'9',756,NULL},
     {u8"przegubowe przesuwne poziomo\0",L'O',747,NULL},
     {u8"przegubowe przesuwne pionowo L\0",L'0',748,NULL},
     {u8"przegubowe przesuwne pionowo P\0",L'-',749,NULL},
     {u8"przegubowe przesuwne poziomo G\0",L'=',757,NULL},
     {u8"bez obrotu Z\0",L'+',818,NULL},
};

#endif

#ifdef __O_GRAPHF__

#define MAX_GRAPHFNO 16
#define MAX_GRAPHF_NAME_LEN 16
#define MAX_GRAPHF_FORMULA_LEN  (MAX_GRAPHFNO * MaxTextLen)    //4096

#define GRAPHF "Wykres"
#define _GRAPHF_ "Wykres"
#define _GRAPHF_C_ L'W'

static POLE pmGraphf[MAX_GRAPHFNO + 1];

static TMENU mGraphf = { 16,0,0,MAX_GRAPHF_NAME_LEN + 1,70,7,0,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmGraphf,NULL,NULL };

static POLE pmGraph[] = {
		  {u8"Krok dx\0       ",'K',254,NULL},
		  {u8"Funkcja\0",'F', 255, &mGraphf} };

#define _CURRENT_ u8"Bieżąca"

#endif

#ifdef __B_MENU__

#define _ENDPOINT_ L'K'
#define _endpoint_ L'k'
#define _CTRL_ENDPOINT_ 1931 //11
#define _NEAREST_ L'B'
#define _nearest_ L'b'
#define _CTRL_NEAREST_ 1922 //2
#define _MIDDLE_ L'S'
#define _middle_ L's'
#define _CTRL_MIDDLE_ 1939 //19
#define _INTERSECTION_ L'P'
#define _intersection_ L'p'
#define _CTRL_INTERSECTION_ 1936 //16
#define _CENTER_ L'C'
#define _center_ L'c'
#define _CTRL_CENTER_ 1923 //3
#define _PERPENDICULAR_ L'R'
#define _perpendicular_ L'r'
#define _CTRL_PERPENDICULAR_ 1938 //18
#define _TANGENTIAL_ L'T'
#define _tangential_ L't'
#define _CTRL_TANGENTIAL_ 1940 //20
#define _ADJACENT_ L'N'
#define _adjacent_ L'n'
#define _CTRL_ADJACENT_ 1934 //14
#define _POINT_ L'U'
#define _point_ L'u'
#define _CTRL_POINT_ 1941 //21
#define _EDIT_ L'E'
#define _edit_ L'e'
#define _CTRL_EDIT_ 1925 //5

#define _CTRL_NOPE_ 1944 //24

#define _CTRL_0_ 0
#define _CTRL_1_ 1
#define _CTRL_2_ _CTRL_NEAREST_
#define _CTRL_3_ _CTRL_CENTER_
#define _CTRL_4_ 4
#define _CTRL_5_ _CTRL_ENDPOINT_
#define _CTRL_6_ 6
#define _CTRL_7_ 7
#define _CTRL_8_ 8
#define _CTRL_9_ 9
#define _CTRL_10_ 10
#define _CTRL_11_ _CTRL_ENDPOINT_
#define _CTRL_12_ 12
#define _CTRL_13_ 13
#define _CTRL_14_ _CTRL_ADJACENT_
#define _CTRL_15_ 15
#define _CTRL_16_ _CTRL_INTERSECTION_
#define _CTRL_17_ 17
#define _CTRL_18_ _CTRL_PERPENDICULAR_
#define _CTRL_19_ _CTRL_MIDDLE_
#define _CTRL_20_ _CTRL_TANGENTIAL_
#define _CTRL_21_ _CTRL_POINT_
#define _CTRL_22_ 22
#define _CTRL_23_ 23
#define _CTRL_24_ _CTRL_NOPE_
#define _CTRL_25_ 25
#define _CTRL_26_ 18 //26  CTRL Z  Copy Object
#define _CTRL_27_ 27
#define _CTRL_28_ 28
#define _CTRL_29_ 29
#define _CTRL_30_ 30
#define _CTRL_31_ 31

#define _PRINTING__ u8"Wydruk"

#endif

#ifdef __O_HATCH__

//extern TMENU mTranslucency;

static POLE pmHTranslucency[] = {
   {u8"100%",'0',546,NULL},
   {u8"95%",'1',673,NULL},
   {u8"90%",'2',545,NULL},
   {u8"85%",'3',672,NULL},
   {u8"80%",'4',544,NULL},
   {u8"75%",'5',671,NULL},
   {u8"70%",'6',543,NULL},
   {u8"65%",'7',670,NULL},
   {u8"60%",'8',542,NULL},
   {u8"55%",'9',669,NULL},
   {u8"50%",'A',541,NULL},
   {u8"45%",'B',668,NULL},
   {u8"40%",'C',540,NULL},
   {u8"35%",'D',667,NULL},
   {u8"30%",'E',539,NULL},
   {u8"25%",'F',666,NULL},
   {u8"20%",'G',667,NULL},
   {u8"15%",'H',666,NULL},
   {u8"10%",'I',665,NULL},
   {u8"5%",'J',664,NULL},

};

TMENU mHTranslucency = { 20,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0, 512,0,0,0,(POLE(*)[]) & pmHTranslucency,NULL,NULL };


#define HATCH_PATTERN u8"WZORCE"
#define PATTERNS_INI u8"WZORCE.INI"

#define PATTERN_NAMES_MAXNO 255

static PPOLE pm__list_hatch[PATTERN_NAMES_MAXNO];

static POLE pm__list_hatch_predef[] = {
		{u8"֎SOLIDNY\0",'S',0,&mHTranslucency},
		{u8"////////\0",'/',0,NULL},
		{u8"XXXXXXXX\0",'X',0,NULL} };

POLE pmKatH[] = {
		{u8"Kąt",'K',108,NULL },
		{u8"Równolegle do",'R',580,NULL},
		{u8"Prostopadle do",'P',581,NULL},
};

TMENU mKatH = { 3,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,ID_VPATTERN_ANGLE,0,0,0,(POLE(*)[]) &pmKatH,NULL,NULL };

static POLE pmSolidHatchPatternDxy[] = {
	   {u8"0",'0',0,NULL},
		{u8"1/32",'1',0,NULL},
		{u8"1/16",'2',0,NULL},
		{u8"1/8",'3',0,NULL},
		{u8"1/4",'4',0,NULL},
		{u8"3/8",'5',0,NULL},
		{u8"1/2",'6',0,NULL},
		{u8"5/8",'7',0,NULL},
		{u8"3/4",'8',0,NULL},
		{u8"7/8",'9',0,NULL},
};

TMENU mSolidHatchPatternDx = { 10,0,0,4,79,6,0,CMNU,CMBR,CMTX,0,17,0,0,0,(POLE(*)[]) & pmSolidHatchPatternDxy,NULL,NULL };

TMENU mSolidHatchPatternDy = { 10,0,0,4,79,6,0,CMNU,CMBR,CMTX,0,27,0,0,0,(POLE(*)[]) & pmSolidHatchPatternDxy,NULL,NULL };


static POLE pmSolidHatchPatternScale[] = {
		{u8"0.125",'0',0,NULL},
		{u8"0.25",'-',0,NULL},
		{u8"0.5",'5',0,NULL},
		{u8"1.0",'1',0,NULL},
		{u8"2.0",'2',0,NULL},
		{u8"4.0",'4',0,NULL},
		{u8"8.0",'8',0,NULL},
};
static TMENU mSolidHatchPatternScale = { 7,0,0,5,79,6,0,CMNU,CMBR,CMTX,0, 6 ,0,0,0,(POLE(*)[]) & pmSolidHatchPatternScale,NULL,NULL };

static POLE pmSolidHatchPatternAngle[] = {
		{u8"0°",'0',578,NULL},
		{u8"90°",'9',442,NULL},
		{u8"180°",'8',444,NULL},
		{u8"270°",'7',446,NULL},
};
static TMENU mSolidHatchPatternAngle = { 4,0,0,4,79,6,ICONS,CMNU,CMBR,CMTX,0, 13 ,0,0,0,(POLE(*)[]) & pmSolidHatchPatternAngle,NULL,NULL };

PTMENU m__list_hatch = { 0, 0, 0, PATERN_NAME_MAXLEN - 1,66, 6,TADD | FIXED , CMNU,CMBR,CMTX,0,   ID_END,   0, 0,0,(POLE(*)[]) & pm__list_hatch_predef, NULL,NULL };

static POLE pmSolidHatchPattern[] = {
		{u8"Wzór\0 ",'W',597,NULL},
		{u8"Skala\0 1.0",'S',436,&mSolidHatchPatternScale},
		{u8"Obrót\0 0",'O',35,&mSolidHatchPatternAngle},
		{u8"dX\0 0",'X',622,&mSolidHatchPatternDx},
		{u8"dY\0 0",'Y',623,&mSolidHatchPatternDy},
		{u8"Pobierz wzór\0",'P',164,NULL},
};

static TMENU mSolidHatchPattern = { 6,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,ID_IMAGE_PATTERN,0,0,0,(POLE(*)[]) & pmSolidHatchPattern,NULL,NULL };

POLE pmSelect_Ins[] = {
	{u8"Prosty",'P',680,NULL},
	{u8"Złożony",'Z',681,NULL},
};

TMENU mSelect_Ins = { 2, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, ID_INSULATION, 0, 0,0,(POLE(*)[]) &pmSelect_Ins, NULL, NULL };

POLE pmHatch[] = {
	 {u8"Wzorzec\0//////////\0      ",'W',434,&m__list_hatch},
	 {u8"Folder wzorców\0 \0      ",'F',597,NULL},
	 {u8"Kąt obrotu\0 0\0      ", 'K', 435,NULL},
	 {u8"Skala\0 1\0     ", 'S', 436,NULL},
	 {u8"punkt Bazowy\0 0,0\0              ", 'B', 437,NULL},
	 {u8"wskaż Punkt bazowy\0 \0", 'P', 438,NULL},
	 {u8"Dystans linii wzoru / segment splajnu\0 1\0 ", 'D', 439,NULL},
	 {u8"izolacja Termiczna\0 \0", 'T', 679, &mSelect_Ins},
	 {u8"Obraz\0 \0",'O',617,&mSolidHatchPattern}
};

#define _HATCHING_ u8"Kreskuj"
#define _HATCHING_C_ L'K'

#define _SOLID_SUFFIX_ "PL"

#define __IMAGE u8"__IMAGE"

#endif

#ifdef __O_DXF_IN__

#define Linie_DXF u8"LINIE-DXF"
static char line_width_dxf_param[5][16] =
{
   u8"bardzo cienka",
   u8"cienka",
   u8"gruba",
   u8"bardzo gruba",
   u8"najgrubsza",
};


#define _LINE_NO_ u8"linia: "
#define _SYSTEM_MESSAGE_ u8"KOMUNIKAT SYSTEMOWY"
#define _INTERRAPTED_ u8"Przerwano"

#endif

#ifdef __O_BLOK__

static char	 sz__last_bl[] = u8"Ostatni blok\0 ";
static char	sz__wszystko[] = u8"Wszystko\0 ";
static char	sz__wszystkie_warstwy[] = u8"wszystKie warstwy\0 ";
static char	sz__wszystkie_teksty[] = u8"wszystkie Teksty\0 ";
static char	sz__wszystkie_symbole[] = u8"wszystkie Symbole\0 ";
static char	 sz__cross_win[] = u8"Cross/okno\0  F9";
static char	sz__add_del[] = u8"Dodaj/usuń\0  F10";
static char	sz_undo_del[] = u8"- oddaj wszystko\0 ";

static TMENU mAuto = { 2,0,0,5,72,9,ICONS,CMNU,CMBR,CMTX,0, 7, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAutoc = { 2,0,0,5,72,8,ICONS,CMNU,CMBR,CMTX,0,	5,	0,	0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dq = { 2,0,0,5,72,7,ICONS,CMNU,CMBR,CMTX,0, 4, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dn = { 2,0,0,5,72,9,ICONS,CMNU,CMBR,CMTX,0, 9 /*7*/, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmBlokm[] = {
	 {u8"Ostatni blok\0 ",'O',256,NULL},
	 {u8"Wszystko\0 ",'W',257,NULL},
	 {u8"wszystKie warstwy\0 ",'K',258,NULL},
	 {u8"Cross/okno\0  F9", 'C',259,NULL},
	 {u8"Dodaj/usuń\0  F10" ,'D', 260,NULL},
	 {u8"Auto\0 T", 'A',261,&mAuto},
};

POLE pmScalem[] = {
		{u8"Aproksymacja\0 20\0  ",'A',238, NULL},
};

POLE pmBlokmc[] = {
	 {u8"Cross/okno\0  F9", 'C',259,NULL},
	 {u8"Dodaj/usuń\0  F10" ,'D',260, NULL},
	 {u8"Auto\0 T", 'A',261,&mAutoc},
};

POLE pmSelectMode[] = {
	 {u8"Cross", L'C',259,NULL},
	 {u8"Obiekt" , L'O',720, NULL},
};

POLE pmBlock_wez_w[] =
{
  {u8"Cross/okno\0  F9",	'C',259,NULL},
  {u8"Auto\0 T",	'A',261,&mAuto_dq},
};

POLE pmBlock_wez_info[] =
{
  {u8"Cała polilinia",	'C',687,NULL},
  {u8"Pojedynczy obiekt",'P',688,NULL},
};

POLE pmBlock_dq[] =
{
  {u8"Cross/okno\0  F9",	'C',259,NULL},
  {u8"Auto\0T",	'A',261,&mAuto_dq},
  {u8"- oddaj wszystko\0 ", 'W',270,NULL},
  {u8"Zmień tryb\0 ", 'Z',271,NULL},
};

POLE pmBlock_dn[] =
{
  {u8"Ostatni blok\0 ",'O',256,NULL},
  {u8"Wszystko\0 ",'W',257,NULL},
  {u8"wszystKie warstwy\0 ",'K',258,NULL},
  {u8"Cross/okno\0  F9",	'C',259,NULL},
  {u8"Dodaj/usuń\0  F10"	,'D',260,	NULL},
  {u8"Auto\0T",	'A',261,&mAuto_dn},
  {u8"Usuń\0 Del", 'U',32,NULL},
  {u8"- oddaj wszystko\0 ", '-',270,NULL},
  {u8"Zmień tryb\0", 'Z',271,NULL},
};

POLE pmBlok_ex[] = {
	 {u8"Ostatni blok\0 ",'O',256,NULL},
	 {u8"Wszystko\0 ",'W',257,NULL},
	 {u8"wszystKie warstwy\0 ",'K',258,NULL},
	 {u8"Cross/okno\0  F9", 'C',259,NULL},
	 {u8"Dodaj/usuń\0  F10" ,'D',260, NULL},
	 {u8"Auto\0 T", 'A',261,&mAuto},
	 {u8"Eksplozja\0",'E',66,NULL},
};

POLE pmBlok_chp[] = {
	 {u8"Ostatni blok\0 ",'O',256,NULL},
	 {u8"Wszystko\0 ",'W',257,NULL},
	 {u8"wszystKie warstwy\0 ",'K',258,NULL},
	 {u8"wszystko z warstwy Numer\0 ",'N',174,NULL},
	 {u8"wszystkie Teksty\0 ",'T',273,NULL},
	 {u8"Cross/okno\0  F9", 'C',259,NULL},
	 {u8"Dodaj/usuń\0  F10" ,'D',260, NULL},
	 {u8"Auto\0 T", 'A',261,&mAuto},
	 {u8"Zmień cechy\0",'Z',272,NULL},
};

POLE pmBlok_cht[] = {
	 {u8"Ostatni blok\0 ",'O',256,NULL},
	 {u8"wszystkie Teksty\0 ",'T',273,NULL},
	 {u8"Cross/okno\0  F9", 'C',259,NULL},
	 {u8"Dodaj/usuń\0  F10" ,'D',260, NULL},
	 {u8"Auto\0 T", 'A',261,&mAuto},
};

POLE pmBlok_chts[] = {
	 {u8"Ostatni blok\0 ",'O',256,NULL},
	 {u8"wszystkie Symbole\0 ",'S',257,NULL},
	 {u8"wszystkie Teksty\0 ",'T',273,NULL},
	 {u8"Cross/okno\0  F9", 'C',259,NULL},
	 {u8"Dodaj/usuń\0  F10" ,'D', 260,NULL},
	 {u8"Auto\0 T", 'A',261,&mAuto},
};

POLE pmCopy_Divide[] = {
	 {u8"Podziel",'P',50,NULL},
};

static POLE	pmWszystko_Okno[] = {
	 {u8"Wszystko",'W',257,NULL},
	 {u8"Okno",'O',274,NULL},
			 };

char window[16] = "okno";
char  cross[16] = "cross";

char _addto_[16] = u8"dodaj";
char  _remove_[16] = u8"usuń";

char Yes[2] = "T";

#define _DIVIDE_ u8"Podziel"
#define _DIVIDE_C_ L'P'

#define _BLOCK_ u8"Blok"
#define _BLOCKF_ u8"* Blok"
#define _BLOCK_C_ L'B'

#define _BLOCK_SELECT_ u8"Tryb selekcji"
#define _BLOCK_SELECT_C_ L'T'

#define _FROZEN_ u8"zamrożone "
#define _EXPLODE_BLOCKS_ u8"Czy chcesz rozbić zaznaczone %sbloki?"
#define _Yes_ "Tak"
#define _No_ "Nie"

#define _SCALE_ u8"Skala"
#define _SCALE_C_ L'S'

#define _FILE_ u8"Zbiór "
#define _FILE_EXISTS_ u8" istnieje. Zapisać ? (Tak/Nie/Esc):"

#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'

#define _SAVED_BLOCK_ u8"< zapamiętany blok >"

#define _CHANGE_ u8"Zmień"
#define _CHANGE_C_ L'Z'

#define _POINT_IN_AREA_ u8"OK. Punkt w obszarze zamkniętym."
#define _POINT_NOT_IN_AREA_ u8"Niedobrze. Punkt poza obszarem lub obszar niezamknięty."

#define _OBJECT_ u8"Obiekt"
#define _OBJECT_C_ L'O'

static char _entire_polyline_[32]=u8"Cała polilinia";
static char _single_object_[32]=u8"Pojedynczy obiekt";

#endif

#ifdef __O_PROF__

POLE pmChamfer[] =
{
	 {u8"Dystans(e)\0      \0",'D', 316, NULL},
	 {u8"Oddaj\0    \0",'O', 459, NULL},
};

POLE pmFillet[] =
{
	 {u8"Promień\0      \0",'P', 209, NULL},
	 {u8"Oddaj\0    \0",'O', 459, NULL},
};

#define _FILLET_ u8"proFiluj"
#define _FILLET_C_ L'F'

#define _CHAMFER_ u8"Fazuj"
#define _CHAMFER_C_ L'F'

#endif

#ifdef __O_OFFSET__

#define OFFSET_NORMAL u8"Normalna"
#define OFFSET_SMOOTH u8"Gładka"

static POLE pmMethod[] = {
	 {u8"Normalna",L'N',630,NULL},
	 {u8"Gładka",L'R',631,NULL},
};

static TMENU mMethod = { 2, 0,0,19,56,4,ICONS,CMNU,CMBR,CMTX,0,4, 0,0,0,(POLE(*)[]) &pmMethod, NULL,NULL };

static POLE pmOffset[] = {
	 {u8"Punkt\0     ",L'P',457,NULL},
	 {u8"Dystans\0     ", L'D', 458,NULL},
	 {u8"Ustaw dystans\0     ", L'U', 605,NULL},
	 {u8"Metoda dla polilinii\0     ", L'M', 629,&mMethod},
};

#define _OFFSET_ u8"Odsuń"
#define _OFFSET_C_ L'O'

#endif

#ifdef __O_MIRROR__

POLE pmMirror[] = {
		{u8"Pozostaw oryginał\0 Del",'P',603,NULL},
		{u8"Usuń Orginał\0 Del",'U',604,NULL},
					};

char st_del[] = u8"Usuń", st_nodel[] = u8"Pozostaw";

#define _MIRROR_ u8"Lustro"
#define _MIRROR_C_ L'L'

#endif

#ifdef __O_IMPORT__

#define PATTERNS u8"Patterns" //u8"Wzory"
#define __PATTERNS__ u8"Patterns"
#define BACKGROUND u8"Background" //#define BACKGROUND u8"Tła"
#define __BACKGROUND__ u8"Background"
char *BLOCKLIST=u8"Lista bloków";

static POLE	pmObrot[] = {
	 {u8"obróć O kąt",'O',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",   	'0',578,NULL},
	 {u8"obróć pod Kątem",'K',440,NULL},
	 {u8"Równolegle do",'R',580,NULL},
	 {u8"Prostopadle do",'P',581,NULL},
	 {u8"pod kątem Do",'D',582,NULL},
};

static TMENU mObrot = { 13, 0, 0, 16, 66, 8, ICONS, CMNU, CMBR, CMTX, 0, 7, 0,0,0,(POLE(*)[]) & pmObrot, NULL, NULL };

static POLE pmObrot_GEO[] = {
	 {u8"rotate By angle",		'B',440,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8" 315°",	'3',447,NULL},
	 {u8" 0°",   	'0',578,NULL},
	 {u8"rotate To angle",'T',440,NULL},
	 {u8"------",	' ',0,NULL},
	 {u8"kXZ",		'Z',0,NULL},
	 {u8"kYZ",		'Y',0,NULL},
};

static TMENU mObrot_GEO = { 13, 0, 0, 16, 66, 8, ICONS, CMNU, CMBR, CMTX, 0, 5, 0,0,0,(POLE(*)[]) & pmObrot_GEO, NULL, NULL };

static POLE	pmBlok_Imp[] = {
	 {u8"Skala\0",'S',36,NULL},
     {u8"Ustalona skala\0",'U',695,NULL},
	 {u8"Obrót\0",'O',35,&mObrot},
	 {u8"lustro Y\0",'Y',41,NULL},
	 {u8"lustro X\0",'X',40,NULL},
     {u8"Wyśrodkuj do kursora\0  ",'W',717,NULL},
	 {u8"Podziel\0",'P',50,NULL},
};

static POLE	pmBlok_Divide[] = {
	 {u8"Podziel",'P',50,NULL},
};

static POLE	pmBlok_Imp_GEO[] = {
	 {u8"Skala",'S',36,NULL},
	 {u8"Obrót",'O',35,&mObrot_GEO},
	 {u8"lustro Y-Y",'Y',41,NULL},
	 {u8"lustro X-X",'X',40,NULL},
	 {u8"Podziel",'P',50,NULL},
};

#define patterns_tip u8"@Wybierz wzorzec kreskowania"
#define background_tip u8"@Wybierz tło pulpitu"
#define blocklist_tip u8"@Wybierz blok z listy"
#define alpha_sorting_tip u8"Kolejność alfabetyczna"
#define timelapse_tip u8"Sortowanie według czasu"
#define catalog_tip u8"@Wybierz blok"

char catalog[16] = u8"Katalog";
char background[16] = u8"Tło pulpitu";
char blocklist[16] = u8"Bloki";
char patterns[16] = u8"Wzorce";

#define _BLOCK_ u8"Blok"
#define _BLOCK_C_ L'B'

#define _BLOCK_3D_ u8"Blok 3D"

#define _BLOCK__ u8"Blok : ";
#define _BLOCK_3D__ u8"Blok 3D: ";

#define _BACKGROUND_ u8"Tło pulpitu : "
#define _SOLID_IMAGE_ u8"Obraz wypełnienia wielokąta: "
#define _HATCH_IMAGE_ u8"Obraz wypełnienia obszaru kreskowania : "
#define _TRACE_IMAGE_ u8"Obraz wypełnienia śladu: "
#define _IMAGE_ u8"Obraz : "

#define _PATTERN_ u8"Wzorzec  : "

#define _PCX_WRITE_ERROR_ u8"Błąd zapisu pliku PCX"
#define _PNG_WRITE_ERROR_ u8"Błąd zapisu pliku PNG"
#define _JPG_WRITE_ERROR_ u8"Błąd zapisu pliku JPG"

#define _NO_PATTERN_ u8"BRAKWZORU.axx"

#define _NO_BLOCK_IN_CATALOG_ u8"Brak symbolu w katalogu  ("
#define AUTOSCALE u8"   AUTOSKALOWANIE"

#define FOLDER u8"\023<WSZYSTKO>"

#endif

#ifdef __O_BREAK__

static POLE pmBreakOptions[] = {
		 {u8"wytnij i Pozostaw",L'P',692,NULL},
		 {u8"wytnij i Usuń",L'U',693,NULL},
         };

static TMENU mBreakOptions = { 2,0,0,4,24,8,TADD | ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pmBreakOptions,NULL,NULL };

POLE pmBreak[] =
{
	 {u8"Oddaj",'O',459,NULL},
	 {u8"Podziel",'P',50,NULL},
     {u8"Opcje wycinania\0                                       ",L'O',691,&mBreakOptions},
};

#define _BREAK_ u8"wYtnij"
#define _BREAK_C_ L'Y'

static char _break_and_leave_[] = u8"wytnij i Pozostaw";
static char _break_and_cut_[] = u8"wytnij i Usuń";

#endif

#ifdef __O_TRIM__

static POLE pmTrim[] =
{
 {u8"Cofnij",'C',459, NULL},
};

#define _TRIM_ "Utnij"
#define _TRIM_C_ L'U'

static POLE pmExtend[] =
{
 {u8"Cofnij",'C',459, NULL},
};

#define _EXTEND_ "Wydłuż"
#define _EXTEND_C_ L'W'

#endif

#ifdef __O_EXTEND__

#endif

#ifdef __B_HLP__

#define _1920_ 1920

POLE pmHelp[] = {
	 {u8"NARZĘDZIA",' ',351,NULL},
	 {u8"Pomocnicze menu narzędzi",' ',362,NULL},
	 {u8"Pomocnicze menu narzędzi",26,359,NULL},
	 {u8"Powtórz ostatnią funkcję",548 + _1920_,547,NULL},
	 {u8"Edycja wartości numerycznych",' ',363,NULL},
	 {u8"Zwiększ rozmiar celownika",' ',361,NULL},
	 {u8"Pomniejsz rozmiar celownika",' ',360,NULL},
	 {u8"Uwolnij myszkę",' ',386,NULL},
	 {u8"Uwolnij myszkę",24,353,NULL},
     {u8"Uwolnij myszkę (przytrzymaj prawy przycisk przez 0,5 s)",819 + _1920_, 658,NULL},
	 {u8"Zmień rozmiar i położenie okna",25,353,NULL},
	 {u8"Zakończ zapis makra",28,353,NULL},
	 {u8"Ortogonalność",565 + _1920_,353,NULL},
     {u8"Skopiuj obiekt pod kursorem",574 + _1920_,353,NULL},
     {u8"Wybierz warstwę",571 + _1920_,353,NULL},
	 {u8"EDYCJA",' ',321,NULL},
	 {u8"Edytuj tekst",' ',377,NULL},
	 {u8"Przesuń tekst",15,378,NULL},
	 {u8"Usuń tekst",' ',380,NULL},
	 {u8"Przywróć usunięty tekst",' ',381,NULL},
	 {u8"Ortogonalność",' ',383,NULL},
	 {u8"Okno / Cross",' ',384,NULL},
	 {u8"Dodaj / Usuń",' ',385,NULL},
	 {u8"EKRAN",' ',53,NULL},
	 {u8"Zwiększ / Pomniejsz",' ',389,NULL},

	 {u8"Panoramuj",355 + 1920,353,NULL},
	 {u8"Panoramuj",354 + 1920,353,NULL},

	 {u8"Okno",12,353,NULL},
	 {u8"Ostatni widok",13,353,NULL},
	 {u8"Wszystkie obiekty",14,353,NULL},
	 {u8"Centrum",15,353,NULL},
	 {u8"Skala 1",16,353,NULL},
	 {u8"Skala 0.5",17,353,NULL},
	 {u8"Skala 0.2",18,353,NULL},
	 {u8"Skala 0.1",19,353,NULL},
	 {u8"Przerysuj",20,353,NULL},
	 {u8"Cały rysunek",21,353,NULL},
	 {u8"CECHY",' ',352,NULL},
	 {u8"Kolor",12,354,NULL},
	 {u8"Typ linii",13,354,NULL},
	 {u8"Grubość linii",14,354,NULL},
	 {u8"Warstwy",15,354,NULL},
	 {u8"Weź kolor",19,354,NULL},
	 {u8"Weź typ i grubość linii",20,354,NULL},
	 {u8"Weź warstwę",21,354,NULL},
	 {u8"Ukryj warstwę",22,354,NULL},
	 {u8"MIARA",' ',390,NULL},
	 {u8"Współrzędne punktu",16,354,NULL},
	 {u8"Dystans",17,354,NULL},
	 {u8"Kąt",18,354,NULL},
	 {u8"WARSTWY",' ',55,NULL},
	 {u8"Warstwa 1",1,354,NULL},
	 {u8"Warstwa 2 ...",2,354,NULL},
	 {u8"Warstwa 10",0,354,NULL},
	 {u8"Następna warstwa",10,354,NULL},
	 {u8"Poprzednia warstwa",11,354,NULL},
	 {u8"Wszystkie warstwy T",22,353,NULL},
	 {u8"Inversja warstw",23,353,NULL},
	 {u8"KROK WSKAŹNIKA",31,154,NULL},
	 {u8"Minimalny / zadany",12,355,NULL},
	 {u8"CELOWNIK",31,52,NULL},

	 {u8"Koniec",13,355,NULL},
	 {u8"Bliższy koniec",14,355,NULL},
	 {u8"Środek",15,355,NULL},
	 {u8"Przecięcie",16,355,NULL},
	 {u8"Centrum",17,355,NULL},
	 {u8"pRostopadle",18,355,NULL},
	 {u8"Stycznie",19,355,NULL},
	 {u8"Najbliższy",20,355,NULL},
	 {u8"pUnkt",21,355,NULL},

	 {u8"Koniec",' ',559,NULL},
	 {u8"Bliższy koniec",' ' ,550,NULL},
	 {u8"Środek",' ',567,NULL},
	 {u8"Przecięcie",' ',564,NULL},
	 {u8"Centrum",' ',551,NULL},
	 {u8"pRostopadle",' ',566,NULL},
	 {u8"Stycznie",' ' ,568,NULL},
	 {u8"Najbliższy",' ',562,NULL},
	 {u8"pUnkt",' ',569,NULL},

	 {u8"Tryb Koniec",559+1920,353,NULL},
	 {u8"Tryb Bliższy koniec",550+1920,353,NULL},
	 {u8"Tryb Środek",567+1920,353,NULL},
	 {u8"Tryb Przecięcie",564+1920,353,NULL},
	 {u8"Tryb Centrum",551+1920,353,NULL},
	 {u8"Tryb pRostopadle",566+1920,353,NULL},
	 {u8"Tryb Stycznie",568+1920,353,NULL},
	 {u8"Tryb Najbliższy",562+1920,353,NULL},
	 {u8"Tryb pUnkt",569+1920,353,NULL},
	 {u8"Tryb żaden",572+1920,353,NULL},
     {u8"Krótka instrukcja",702 + 1920,534,NULL},

};

#define _ENDPOINT_ L'K'
#define _ENDPOINT_SC_ 11
#define _NEAREST_ L'B'
#define _NEAREST_SC_ 2
#define _MIDDLE_ L'S'
#define _MIDDLE_SC_ 19
#define _INTERSECTION_ L'P'
#define _INTERSECTION_SC_ 16
#define _CENTER_ L'C'
#define _CENTER_SC_ 3
#define _PERPENDICULAR_ L'R'
#define _PERPENDICULAR_SC_ 18
#define _TANGENTIAL_ L'T'
#define _TANGENTIAL_SC_ 20
#define _ADJACENT_ L'N'
#define _ADJACENT_SC_ 14
#define _POINT_ L'U'
#define _POINT_SC_ 21
#define _NOPE_ L'X'
#define _NOPE_SC_ 24

#endif

#ifdef __O_MAK__

#define _MACRO_ u8" MAKRO "

static POLE pmMakro[] = {
	{u8"Inicjacja. Stop-> Ctrl-End",L'I',92,NULL},
	{u8"Wykonanie",L'W',93,NULL},
	{u8"Читати",L'C',94,NULL},
	{u8"Zapisz",L'Z',95,NULL} };

#define _DRAW__ L'r';
#define _TEXT__ L't';

#endif

#ifdef __O_PAR_B__

extern TMENU mGruboscLinii;
extern POLE pmGruboscLinii[];
extern PPOLE pmCzcionka[];


#define MaxMenuWinFont 32
#define FONTNAMELEN 33 //12

char* punits[] = { u8"[mm]","[cm]","[m]","[km]","[\"]","[']","[yd]", "[mi]" };
char *upunits[] = { u8"mm","cm","m","km", "\"","'","yd","mi" };

static TMENU mRamka_on_off = { 2,0,0,7,30,7,ICONS,CMNU,CMBR,CMTX,0,15,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mSektory_on_off = { 2,0,0,7,30,7,ICONS,CMNU,CMBR,CMTX,0,17,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mReversed_on_off = { 2,0,0,7,30,7,ICONS,CMNU,CMBR,CMTX,0,19,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

TMENU mGruboscLiniiF = { 5,0,0,26,52,12,0,CMNU,CMBR,CMTX,0,21,0,0,0,(POLE(*)[]) &pmGruboscLinii,NULL,NULL };
TMENU mGruboscLiniiZ = { 5,0,0,26,52,12,0,CMNU,CMBR,CMTX,0,26,0,0,0,(POLE(*)[]) &pmGruboscLinii,NULL,NULL };
TMENU mKolorACF = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,23,56,5,0,CMNU,CMBR,CMTX,15,31,0,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };
TMENU mKolorACZ = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,23,56,5,0,CMNU,CMBR,CMTX,15,48,0,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };
TMENU mKolorSTATIC = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,23,56,5,0,CMNU,CMBR,CMTX,15,0,0,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };
PTMENU mCzcionkaZ = { 1,/*MaxMenuWinFont*/ 0,/*MaxMenuWinFont*/ 0,FONTNAMELEN + 1,74,10,FIXED,CMNU,CMBR,CMTX,0,69,0,0,0,(POLE(*)[]) & pmCzcionka,NULL,NULL};

/*
static POLE pmFormat_r[21] = {
   {u8"X \0xxxxx * yyyyy\0        ",'X',0,NULL},
	{u8"A4\0  210 *   297",'4',0,NULL},
	{u8"A4_\0 297 *   210",'4',0,NULL},
	{u8"A3\0  420 *   297",'3',0,NULL},
	{u8"A2\0  594 *   420",'2',0,NULL},
	{u8"A1\0  841 *   594",'1',0,NULL},
	{u8"A0\0 1189 *   841",'0',0,NULL},
	{u8"a4\0  185 *   280",' ',0,NULL},
	{u8"a4_\0  280 *   185",' ',0,NULL},
	{u8"a3\0  395 *   280",' ',0,NULL},
	{u8"a2\0  565 *   400",' ',0,NULL},
	{u8"a1\0  810 *   565",' ',0,NULL},
	{u8"a0\0 1160 *   810",' ',0,NULL},
	{u8"3*A4\0  630 *   297",' ',0,NULL},
	{u8"4*A4\0  841 *   297",' ',0,NULL},
	{u8"5*A4\0 1050 *   297",' ',0,NULL},
	{u8"6*A4\0 1260 *   297",' ',0,NULL},
	{u8"3*a4\0  600 *   280",' ',0,NULL},
	{u8"4*a4\0  810 *   280",' ',0,NULL},
	{u8"5*a4\0 1020 *   280",' ',0,NULL},
	{u8"6*a4\0 1230 *   280",' ',0,NULL},
};

static TMENU mFormat_r = { 21,16,16,18,24,5,TADD,CMNU,CMBR,CMTX,0,11,0,0,0,(POLE(*)[]) &pmFormat_r,NULL,NULL };
*/

 static POLE pmFormat_r[46]={
    {"X\0 xxxxx * yyyyy\0        ",'X',0,NULL},
	{"ISO A4\0  210 *  297",'4',0,NULL},
	{"ISO A4_\0 297 *  210",' ',0,NULL},
	{"ISO A3\0  420 *  297",'3',0,NULL},
    {"ISO A3_\0  297 * 420",' ',0,NULL},
	{"ISO A2\0  594 *  420",'2',0,NULL},
    {"ISO A2_\0  420  *  594",' ',0,NULL},
	{"ISO A1\0  841 *  594",'1',0,NULL},
    {"ISO A1_\0  594  *  841",' ',0,NULL},
	{"ISO A0\0 1189 *  841",'0',0,NULL},
    {"ISO A0_\0  841 * 1189",' ',0,NULL},
	{"a4\0  185 *  280",' ',0,NULL},
	{"a4_\0  280 *  185",' ',0,NULL},
	{"a3\0  395 *  280",' ',0,NULL},
	{"a2\0  565 *  400",' ',0,NULL},
	{"a1\0  810 *  565",' ',0,NULL},
	{"a0\0 1160 *  810",' ',0,NULL},
	{"3*A4\0  630 *  297",' ',0,NULL},
	{"4*A4\0  841 *  297",' ',0,NULL},
	{"5*A4\0 1050 *  297",' ',0,NULL},
	{"6*A4\0 1260 *  297",' ',0,NULL},
	{"3*a4\0  600 *  280",' ',0,NULL},
	{"4*a4\0  810 *  280",' ',0,NULL},
	{"5*a4\0 1020 *  280",' ',0,NULL},
	{"6*a4\0 1230 *  280",' ',0,NULL},
    {"ISO B4\0  250 *  176",' ',0,NULL},
    {"ISO B4_\0  176 *  250",' ',0,NULL},
    {"ISO B3\0  500 *  353",' ',0,NULL},
    {"ISO B3_\0  353 *  500",' ',0,NULL},
    {"ISO B2\0  707 *  500",' ',0,NULL},
    {"ISO B2_\0  500 *  707",' ',0,NULL},
    {"ISO B1\0 1000 *  707",' ',0,NULL},
    {"ISO B1_\0  707 * 1000",' ',0,NULL},
    {"ISO B0\0 1414 * 1000",' ',0,NULL},
    {"ISO B0_\0 1000 * 1414",' ',0,NULL},
    {"ANSI A\0  279 *  215",' ',0,NULL},
    {"ANSI A_\0  215 *  279",' ',0,NULL},
    {"ANSI B\0  431 *  279",' ',0,NULL},
    {"ANSI B_\0  279 *  431",' ',0,NULL},
    {"ANSI C\0  559 *  432",' ',0,NULL},
    {"ANSI C_\0  432 *  559",' ',0,NULL},
    {"ANSI D\0  864 *  559",' ',0,NULL},
    {"ANSI D_\0  559 *  864",' ',0,NULL},
    {"ANSI E\0 1118 *  864",' ',0,NULL},
    {"ANSI E_\0  864 * 1118",' ',0,NULL},
    {"US legal\0  215 *  355",' ',0,NULL}
	 };  //21->46

static TMENU mFormat_r = { 46,0,0,18,24,5,TADD,CMNU,CMBR,CMTX,0,11,0,0,0,(POLE(*)[]) &pmFormat_r,NULL,NULL };



static POLE pmKursorSize[10] = {
		 {u8"10\0",'1',0,NULL},
		 {u8"20\0",'2',0,NULL},
		 {u8"30\0",'3',0,NULL},
		 {u8"40\0",'4',0,NULL},
		 {u8"50\0",'5',0,NULL},
		 {u8"60\0",'6',0,NULL},
		 {u8"70\0",'7',0,NULL},
		 {u8"80\0",'8',0,NULL},
		 {u8"90\0",'9',0,NULL},
		 {u8"Max\0",'M',0,NULL}};

static TMENU mKursorSize = { 10,0,0,4,24,8,TADD,CMNU,CMBR,CMTX,0,89,0,0,0,(POLE(*)[]) &pmKursorSize,NULL,NULL };
static TMENU mKursorSizeB = { 10,0,0,4,24,8,TADD,CMNU,CMBR,CMTX,0,99,0,0,0,(POLE(*)[]) &pmKursorSize,NULL,NULL };

static POLE pmKursorTyp[2] = {
		 {u8"Krzyż\0",'K',594,&mKursorSize},
		 {u8"Dupleks\0",'D',595,&mKursorSizeB} };

static TMENU mKursorTyp = { 2,0,0,4,24,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmKursorTyp,NULL,NULL };

static POLE pmSkalaF[15] = {
		 {u8"[A] 1 :\0 1",L'A',0,NULL},
		 {u8"[B] 1 :\0 2",L'B',0,NULL},
		 {u8"[C] 1 :\0 5",L'B',0,NULL},
		 {u8"[D] 1 :\0 10",L'D',0,NULL},
		 {u8"[E] 1 :\0 20",L'E',0,NULL},
		 {u8"[F] 1 :\0 25",L'F',0,NULL},
		 {u8"[G] 1 :\0 50",L'G',0,NULL},
		 {u8"[H] 1 :\0 100",L'H',0,NULL},
		 {u8"[I] 1 :\0 500",L'I',0,NULL},
		 {u8"[J] 1 :\0 1000",L'J',0,NULL},
		 {u8"[K] 1 :\0 5000",L'K',0,NULL},
		 {u8"[L] 1 :\0 10000",L'L',0,NULL},
		 {u8"[M]    \0 2 : 1",L'M',0,NULL},
		 {u8"[N]    \0 5 : 1",L'N',0,NULL},
		 {u8"[X] 1 :\0 X\0         ",L'X',0,NULL} };

static TMENU mSkalaF = { 15,0,0,15,24,6,TADD,CMNU,CMBR,CMTX,0,57,0,0,0,(POLE(*)[]) &pmSkalaF,NULL,NULL };

static POLE pmDokladnosc[6] = {
		 {u8"[1]\0 0.05 mm",'1',0,NULL},
		 {u8"[2]\0 0.1 mm",'2',0,NULL},
		 {u8"[3]\0 0.2 mm",'3',0,NULL},
		 {u8"[4]\0 0.25 mm",'4',0,NULL},
		 {u8"[5]\0 0.5 mm",'5',0,NULL},
		 {u8"[X]\0 X mm\0        ",'X',0,NULL} };

static TMENU mDokladnosc = { 6,0,0,13,24,7,TADD,CMNU,CMBR,CMTX,0,72,0,0,0,(POLE(*)[]) &pmDokladnosc,NULL,NULL };


static POLE pmJednostki[8] = {
         {u8"[1] milimetr \0[mm]",'1',0,NULL},
		 {u8"[2] centymetr \0[cm]",'2',0,NULL},
		 {u8"[3] metr \0[m]",'3',0,NULL},
         {u8"[4] kilometr \0[km]",'4',0,NULL},
		 {u8"[5] cal \0[\"]",'5',0,NULL},
		 {u8"[6] stopa \0[']",'6',0,NULL},
         {u8"[7] jard \0[yd]",'7',0,NULL},
         {u8"[8] mila \0[mi]",'8',0,NULL}};

static TMENU mJednostki = { 8,0,0,18,24,8,TADD,CMNU,CMBR,CMTX,0,78,0,0,0,(POLE(*)[]) &pmJednostki,NULL,NULL };

static POLE pmKartezjanskiGeodezyjny[2] = {
		 {u8"Kartezjański",'K',104,NULL},
		 {u8"Geodezyjny",'G',105,NULL} };

static TMENU mKartezjanskiGeodezyjny = { 2,0,0,12,24,8,ICONS,CMNU,CMBR,CMTX,0,109,0,0,0,(POLE(*)[]) &pmKartezjanskiGeodezyjny,NULL,NULL };


static POLE pmPointOrigin[3] = {
		 {u8"Punkt <0,0>\0 ",'P',106,NULL},
		 {u8"Oś układu lokalnego\0",'O',107,NULL},
		 {u8"Kąt obrotu osi\0",'K',108,NULL } };

static TMENU mPointOrigin = { 3,0,0,19,24,9,ICONS,CMNU,CMBR,CMTX,0,86,0,0,0,(POLE(*)[]) &pmPointOrigin,NULL,NULL };

static char* tablica_styli[] = { u8"góra",u8"dół",u8"góra + dół",u8"dookoła" };

static POLE pmSektory_Styl[4] = {
		 {u8"Góra",'G',498,NULL},
		 {u8"Dół",'D',499,NULL},
		 {u8"góra + dół",'P',500,NULL},
		 {u8"doOkoła",'O',486,NULL} };

static TMENU mSektory_Styl = { 4,0,0,10,30,7,ICONS,CMNU,CMBR,CMTX,0,65,0,0,0,(POLE(*)[]) &pmSektory_Styl,NULL,NULL };

static POLE pmSektory_Arkusza[15] = {
		{u8"Ramka włączona\0 N\0           ", 'R', 485, & mRamka_on_off},
		{ u8"Sektory włączone\0 N\0          ",'S',486,&mSektory_on_off },
		{ u8"Prefiks numeru sektora\0 \0     ",'P',487,NULL },
		{ u8"Numeracja od\0 \0   ",'N',488,NULL },
		{ u8"Odwrócona numeracja\0 N\0           ",'O',489,&mReversed_on_off },
		{ u8"Wysokość sektora [mm]\0 \0   ",'W',490,NULL },
		{ u8"Długość sektora [mm]\0 \0   ",'D',491,NULL },
		{ u8"sZerokość sektora [mm]\0 \0   ",'Z',492,NULL },
		{ u8"odsunięcie rAmki [mm]\0 \0   ",'A',501,NULL },
		{ u8"grubość Linii ramki\0Cieńka\0          ",  'L',493,&mGruboscLiniiF },
		{ u8"Grubość linii sektora\0Gruba\0          ",  'G',494,&mGruboscLiniiZ },
		{ u8"֎Kolor ramki\0biały\0              ", 'K',495,&mKolorACF },
		{ u8"֎kolor sekTora\0biały\0              ", 'T',496,&mKolorACZ },
		{ u8"stYl\0 góra\0              ",'Y',497,&mSektory_Styl },
		{ u8"Czcionka\0 \0             ", 'C', 183,&mCzcionkaZ }};


static POLE pmPrecision[] = {
        {u8"dokładność Siły\0 \0",L'S',727,NULL},
        {u8"dokładność Momentu\0 \0",L'M',728,NULL},
        {u8"dokładność Przemieszczenia\0 \0",L'P',730,NULL},
        {u8"dokładność Rotacji\0 \0",L'R',731,NULL},
        {u8"dokładność Obciążenia\0 \0",L'O',733,NULL},
        {u8"dokładność obciążenia Termicznego\0 \0",L'T',752,NULL},
        {u8"dokładność naprężeń\0 \0",L'N',800,NULL},
};

static TMENU mPrecision = { 7,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,17,0,0,0,(POLE(*)[]) &pmPrecision,NULL,NULL };

static POLE pmThermal[] = {
       {u8"Głębokość przekroju\0 \0",L'G',764,NULL},
	   {u8"zmiana Temperatury\0 \0",L'T',765,NULL},
	  };

static TMENU mThermal = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmThermal,NULL,NULL };

static POLE pmStaticColors[] = {
       {u8"֎Geometria (węzły i elementy)\0szary\0",     L'G',723,&mKolorSTATIC},
       {u8"֎siły Rozciągające\0czerwony\0",             L'R',771,&mKolorSTATIC},
	   {u8"֎siły Ściskające\0niebieski\0",              L'S',758,&mKolorSTATIC},
       {u8"֎siły Tnące\0zielony\0",                     L'T',759,&mKolorSTATIC},
       {u8"֎Momenty\0purpurowy\0",                      L'M',760,&mKolorSTATIC},
       {u8"֎Deformacje\0żółty\0",                       L'D',761,&mKolorSTATIC},
       {u8"֎reacje Podpór\0turkusowy\0",                L'P',763,&mKolorSTATIC},
       {u8"֎Naprężenia rozciągające\0ciemnoczerwony\0", L'N',801,&mKolorSTATIC},
       {u8"֎nAprężenia ściskające\0ciemnoniebieski\0",  L'A',802,&mKolorSTATIC},
       {u8"֎naprężenia ścinające\0ciemnozielony\0",     L'H',803,&mKolorSTATIC},
       {u8"֎Wibracje\0inny\0",                          L'W',814,&mKolorSTATIC},
	  };

static TMENU mStaticColors = { 11,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,24,0,0,0,(POLE(*)[]) &pmStaticColors,NULL,NULL };

static POLE pmMagnitude[] = {
       {u8"przeskalowanie Siły\0 \0",                   L'S',727,NULL},
	   {u8"przeskalowanie Momentu\0 \0",                L'M',728,NULL},
	   {u8"przeskalowanie Przemieszczenia\0 \0",        L'P',730,NULL},
	   {u8"przeskalowanie Rotacji\0 \0",                L'R',731,NULL},
       {u8"przeskalowanie Obciążenia\0 \0",             L'O',733,NULL},
       {u8"przeskalowanie obciążenia Termicznego\0 \0", L'T',752,NULL}, //&mThermal},
       {u8"przeskalowanie sił osiowych\0 \0",           L'N',758,NULL},
       {u8"przeskalowanie sił ścinających\0 \0",        L'V',759,NULL},
       {u8"przeskalowanie momentów Zginających\0 \0",   L'B',760,NULL},
       {u8"przeskalowanie deformacji\0 \0",             L'U',761,NULL},
       {u8"przeskalowanie sił reakcji\0 \0",            L'X',762,NULL},
       {u8"przeskalowanie momentów reakcji\0 \0",       L'Z',763,NULL},
       {u8"przeskalowanie naprężeń\0 \0",               L'A',800,NULL},
       {u8"przeskalowanie % zbrojenia\0 \0",            L'%',823,NULL},
       {u8"przeskalowanie Wibracji\0 \0",               L'W',814,NULL},
       {u8"Dokładność\0 \0",                            L'D',184,NULL}, //&mPrecision},
       {u8"Kolory\0 \0",                                L'K',495,NULL}, //&mStaticColors},
};

static TMENU mMagnitude = { 17,0,0,10,30,7,TADD | ICONS| NOWCLOSE ,CMNU,CMBR,CMTX,0,111,0,0,0,(POLE(*)[]) &pmMagnitude,NULL,NULL };

static POLE pmParametry[] = {
	 {u8"Format rysunku\0 A4\0    ",'F',96,&mFormat_r},
	 {u8"Skala\0 1:5\0     ",'S',97,&mSkalaF},
     {u8"Jednostki\0       mm\0    ",'J',99,&mJednostki},
	 {u8"Dokładność\0 0.25mm\0    ",'D',98,&mDokladnosc},
	 {u8"Układ współrzędnych\0 K\0 ",'U',100,&mKartezjanskiGeodezyjny},
	 {u8"układ Lokalny\0 \0        ",'L',101,&mPointOrigin},
	 {u8"sektory Arkusza\0 \0      ",'A',486,NULL},
	 {u8"typ i wielkość Wskaźnika\0 Max\0  ",'W',102,&mKursorTyp},
     {u8"Przeskalowanie wektorów, precyzja i kolory\0\0  ",'P',751,NULL}, //&mMagnitude},
	 {u8"Edycja ALFACADPL.INI\0 \0     ",'E',103,NULL},
};

static char* ParTxt[] = { u8"K", "G", "T", "N", };

#define _CARTESIAN_ u8"C"
#define _GEODETIC_ u8"G"
#define _MAX_ "Max"
#define _PLUSMAX_ "+Max"
#define _OTHER_COLOR_ u8"֍[X] inny>>> "

#define _SECTORS_        "Sektory"

static char config_sectors[11][32] =
{
	"Grubosc linii ramki",
	"Grubosc linii sektorow",
	"Kolor ramki",
	"Kolor sektorow",
	"Odsuniecie ramki",
	"Wysokosc sektorow",
	"Dlugosc sektora pionowego",
	"Dlugosc sektora poziomego",
	"Czcionka opisu sektora",
	"Wysokosc opisu sektora",
	"Odwrocona numeracja",
};

#endif

#ifdef __O_LIBFUN__

char bloki_dir[MAXPATH] = "BLOKI";
char katalogi_dir[MAXPATH] = "KATALOGI";
char bloki_cfg[MAXPATH] = "BLOKI.CFG";
char biblioteka_cfg[MAXPATH] = "APARATY.AXX";

#endif

#ifdef __O_FOLDERSDIALOG__

#define _SELECT_FOLDER_  L"Wybierz folder lub wpisz ..  lub  / lub //"
char error_message_block[64] = u8"Brak bloków w tym katalogu";
char error_message_background[64] = u8"Brak wzorców w tym katalogu";

#endif

#ifdef __BIB_E__

char* view_width_tab[] = { u8"B.C.",u8"C.",u8"G.",u8"B.G.",u8"NG.", "", "", u8"ukryta " };
char* view_type_tab[] = {
			   u8"Ciągła",
			   u8"Kreskowa",
			   u8"Punktowa",
			   u8"Dwupunktowa",
			   u8"Wielopunktowa",
			   u8"Granica",
			   u8"Środkowa",
			   u8"Ukryta",
			   u8"widMo",
			   u8"kreskowa_",
			   u8"punktowa_2",
			   u8"dwupunktowa_2",
			   u8"wielopunktowa_2",
			   u8"granica_2",
			   u8"środkowa_2",
			   u8"ukryta_2",
			   u8"widmo_2",
			   u8"kreskowa_x2",
			   u8"punktowa_x2",
			   u8"dwupunktowa_x2",
			   u8"wielopunktowa_x2",
			   u8"granica_x2",
			   u8"środkowa_x2",
			   u8"ukryta_x2",
			   u8"widmo_x2",
			   u8"trzypunktowa",
			   u8"dwuśrodkowa",
			   u8"trzykreskowa",
			   u8"widmo_3",
			   u8"dwuśrodkowa_x2",
			   u8"trzykreskowa_x2",
			   u8"wielokreskowa",
			   u8""};

T_Prototype      s__prot = { u8"prototyp.alf", TRUE };

#define _SYSTEM_MESSAGE_ u8"KOMUNIKAT SYSTEMOWY"
#define _INTERRAPTED_ u8"Przerwano"
#define _TOO_MANY_BLOCKS_ u8"Zbyt duża liczba bloków aby sprawdzić wszystkie"
#define _BLOCKS_CORRECTED_ u8"Skorygowano %ld bloków, %ld obiektów"
#define _BLOCKS_TESTED_ u8"Wszystko OK. Przetestowano %d bloków"


char* typ_punktu_inf[] = { u8"Normalny",u8"Punkt bazowy","","","","","",u8"Węzeł",u8"Zacisk","","","",
                           u8"Utwierdzony",u8"Utwierdzony L",u8"Utwierdzony P", u8"Utwierdzony G", u8"Przegubowy", u8"Przegubowy L", u8"Przegubowy P", u8"Przegubowy G",
                           u8"Sztywny Przesuwny", u8"Sztywny Przesuwny L", u8"Sztywny Przesuwny P", u8"Sztywny Przesuwny G", u8"Przegubowy Przesuwny", u8"Przegubowy Przesuwny L", u8"Przegubowy Przesuwny P", u8"Przegubowy Przesuwny G", u8"bez obrotu Z",
                           "","","","","","","","",
};

#endif

#ifdef __O_LISTBL__

#define _NUMBER_OF_BLOCKS_ u8"Liczba bloków ("
#define _EXCEEDS_NO_ u8") przekracza 5000"

#endif

#ifdef __O_ETEXT__

#define _YES_NO_ESC_ u8"TNtn\033"

POLE pmenu_typ[] = {
			   {u8"*WSZYSTKIE",'*',0,NULL},       //255
			   {u8"Normalny",'N',0,NULL},         //0
			   {u8"Etykieta wew.",'W',0,NULL},    //1
			   {u8"etYkieta skł.",'Y',0,NULL},    //2
			   {u8"Symbol",'S',0,NULL},           //3
			   {u8"Typ",'T',0,NULL},              //4
			   {u8"? nazwa zacisku*",'?',0,NULL}, //5
			   {u8"Zasilanie      *",'Z',0,NULL}, //6
			   {u8"Port           *",'P',0,NULL}, //7
			   {u8"Atrybut",'A',0,NULL},          //8
			   {u8"+ schemat",'+',0,NULL},        //9
			   {u8"nazwa pLiku+",'L',0,NULL},     //10
			   {u8"<> sieć",'<',0,NULL},          //11
			   {u8"Komentarz",'K',0,NULL},        //12
			   {u8"% zmienna      *",'%',0,NULL}, //13
			   {u8"symbol Drabinki*",'D',0,NULL}, //14
			   {u8"Opis drabinki  *",'O',0,NULL}, //15
};

POLE pmenu_typ1[] = {
			   {u8"SYMBOL",'S',0,NULL},           //3
			   {u8"*WSZYSTKIE",'*',0,NULL},       //255
			   {u8"Normalny",'N',0,NULL},         //0
			   {u8"Etykieta wew.",'W',0,NULL},    //1
			   {u8"etYkieta skł.",'Y',0,NULL},    //2
			   {u8"Symbol",'S',0,NULL},           //3
			   {u8"Typ",'T',0,NULL},              //4
			   {u8"? nazwa zacisku*",'?',0,NULL}, //5
			   {u8"Zasilanie      *",'Z',0,NULL}, //6
			   {u8"Port           *",'P',0,NULL}, //7
			   {u8"Atrybut",'A',0,NULL},          //8
			   {u8"+ schemat",'+',0,NULL},        //9
			   {u8"nazwa pLiku+",'L',0,NULL},     //10
			   {u8"<> sieć",'<',0,NULL},          //11
			   {u8"Komentarz",'K',0,NULL},        //12
			   {u8"% zmienna      *",'%',0,NULL}, //13
			   {u8"symbol Drabinki*",'D',0,NULL}, //14
			   {u8"Opis drabinki  *",'O',0,NULL}, //15
};

POLE pmenu_auto[] = {
			   {u8"zmień Wszystkie",'W',615,NULL},
			   {u8"Potwierdzenie",'P',616,NULL},
};

POLE pmenu_change[] = {
			   {u8"Zmień",'Z',112,NULL},
			   {u8"Pomiń",'P',113,NULL},
};

#endif

#ifdef __O_TEXT_B__

#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ L'T'
#define _yes_ L't'
#define _EDIT_ L'E'
#define _edit_ L'e'

#define _EDIT_SC_ 5

static TMENU mItalics = { 2,0,0,7,79,9,ICONS,CMNU,CMBR,CMTX,0,13,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUkryty = { 2,0,0,7,79,11,ICONS,CMNU,CMBR,CMTX,0,4,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mBold = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,37,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUnderlined = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,39,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAutoInterwal = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmTypTekstu[] = {
			   {u8"Normalny",'N',0,NULL},
			   {u8"Etykieta wew.",'W',0,NULL},
			   {u8" etYkieta skł.",'Y',0,NULL},
			   {u8"Symbol",'S',0,NULL},
			   {u8"Typ",'T',0,NULL},
			   {u8"? nazwa zacisku*",'?',0,NULL},
			   {u8"Zasilanie      *",'Z',0,NULL},
			   {u8"Port           *",'P',0,NULL},
			   {u8"Atrybut",'A',0,NULL},
			   {u8"+ schemat",'+',0,NULL},
			   {u8"nazwa pLiku+",'L',0,NULL},
			   {u8"<> sieć",'<',0,NULL},
			   {u8"Komentarz",'K',0,NULL},
			   {u8"% zmienna      *",'%',0,NULL},
			   // {u8"symbol Drabinki*",'D',0,NULL},
			   // {u8"Opis drabinki  *",'O',0,NULL},
				{u8"symbol pomocniczy*",'D',0,NULL},
				{u8"Opis pomocniczy*",'O',0,NULL},
};

TMENU mTypTekstu = { 16,0,0,16,74,6,0,CMNU,CMBR,CMTX,0,15,0,0,0,(POLE(*)[]) &pmTypTekstu,NULL,NULL };

POLE pmJustowanie[] = {
					{u8"do Lewej",'L',262,NULL},
					{u8"do Prawej",'P',263,NULL},
					{u8"Środkowo",'S',264,NULL},
					{u8"Centralnie",'C',265,NULL},
};

TMENU mJustowanie = { 4,0,0,13,74,6,ICONS,CMNU,CMBR,CMTX,0,31,0,0,0,(POLE(*)[]) &pmJustowanie,NULL,NULL };


POLE pmKatT[] = {
		{u8"0",'0',282,NULL},
		{u8"90",'9',283,NULL},
		{u8"=",'=',284,NULL},
		{u8"+",'+',285,NULL},
		{u8"Xk",'K',286,NULL},
		{u8"X",'X',287,NULL} };

TMENU mKatT = { 6,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,41,0,0,0,(POLE(*)[]) &pmKatT,NULL,NULL };


extern TMENU mLineSpacing;
extern PTMENU mCzcionka;

static POLE pmTekstm[] = {
	{u8"Edytuj ponownie\0 0\0	",'E',38,NULL},
	{u8"Kąt\0 0\0				",'K',229,&mKatT},
	{u8"Wysokość\0 3mm\0         ",'W',179,NULL},
	{u8"Szerokść-wsp.\0 1.0\0         ", 'S',230,NULL},
	{u8"tekst Pochylony\0 N\0   ", 'P', 181,&mItalics},
	{u8"Typ tekstu\0 \0   ", 'T', 231,&mTypTekstu},
	{u8"Ukryty\0 N\0   ", 'U', 232,&mUkryty},
	{u8"poGrubiony\0 N\0   ", 'G', 182,&mBold},
	{u8"podkreśLony\0 N\0      ", 'L', 409,&mUnderlined},
	{u8"Justowanie\0 |<\0   ", 'J', 233,&mJustowanie},
	{u8"Odstep linii\0 N\0    ", 'O', 410,&mLineSpacing},
	{u8"Auto-interwał\0   N\0   ", 'A', 91,&mAutoInterwal},
	{u8"Interwał\0 1\0          ", 'I', 234,NULL},
	{u8"Czcionka\0 \0             ",'C', 183,&mCzcionka},
};

static char t_t[16][12] = {
 u8"Normal",u8"Etyk.w",u8"Etyk.s",u8"Symbol",u8"Typ",u8"zacisk",u8"Zasil.",u8"Port",u8"Atryb.",u8"Schem+",u8"Plik",u8"sieć",u8"Koment.",u8"%zmien.",
 u8"Symbol+",u8"Opis+" };

#define _TEXT_ u8"Tekst"
#define _TEXT_C_ L'T'

#define _HIDDEN_ u8" ukryty"

#endif

#ifdef __B_WINDEM__

#define _NOT_LICENSED_ u8"Przekazując darowiznę wspierasz projekt"
#define _NOT_LICENSED1_ u8"Dziękuję."
#define _DEM_ROWS_NO_ 12
char lic_for[16] = { u8"Użytkownik: " };

POLE pmDem1[] = {
	/*0*/	 {u8"",' ',0,NULL},
#ifdef BIT64
	/*1*/	 {u8"Tworzenie nowego okna AlfaCAD (x64) ...",' ',0,NULL},
#else
	/*1*/	 {u8"Tworzenie nowego okna AlfaCAD ...",' ',0,NULL},
#endif
	/*7*/{u8"_______________________________________________",' ',0,NULL},
	/*10*/   {u8"",' ',0,NULL},
};

POLE pmPrint[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8"   D R U K O W A N I E   ",' ',0,NULL},
	/*2*/	 {u8"    Przerwanie -> Esc    ",' ',0,NULL},
	/*3*/  {u8"",' ',0,NULL},
};

POLE pmPlot[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8"   K R E Ś L E N I E   ",' ',0,NULL},
	/*2*/	 {u8"",' ',0,NULL},
};

POLE pmPlot_to_com[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8" PRZESYŁANIE DANYCH DO PLOTERA ",' ',0,NULL},
	/*2*/	 {u8"",' ',0,NULL},
};

T_Prot_Date s_prot_date = { PROT_KEY, 0, 1, 0, 0, "Użytkownik : ", "Seria : ",
0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "",
0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "" };


#endif

#ifdef __O_OBJECT1__

#define _YES_NO_ESC_ u8"TNtn\033"
#define _YES_ 'T'
#define _yes_ 't'
#define _NO_ 'N'
#define _no_ 'n'

#endif

#ifdef __O_ARRAY__

static POLE pmArray_polar[] = {
	{u8"Liczba elementów\0 1\0     ",'L',607,NULL},
	{u8"Kąt wypełnienia [°]\0 360\0     ",'K',608, NULL} };

static POLE pmArray_polar_rot[] = {
	{u8"Liczba elementów\0 1\0     ",'L',609,NULL},
	{u8"Kąt wypełnienia [°]\0 360\0     ",'K',610, NULL} };

#define _ARRAY_WITH_ROTATION_ u8"Szyk z obrotem"
#define _ARRAY_ u8"Szyk"
#define _ARRAY_C_ L'A'

#endif

#ifdef  __BIBWYM4__

#define _DIMENSIONING_ u8"wYmiaruj"
#define _DIMENSIONING_C_ L'Y'

#endif

#ifdef __BIBWYM_B_

static POLE pmKierunek[7] = {
		{u8"0°",'0',203,NULL},
		{u8"90°",'9',204,NULL},
		{u8"Równolegle",'R',205,NULL},
		{u8"pod kątem Do",'D',206,NULL},
		{u8"pod kątem X",'X',207,NULL},
		{u8"Kąt",'K',208,NULL},
        {u8"Linia odniesienia",'L',678,NULL} };

static TMENU mKierunek = { 7,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,9,0,0,0,(POLE(*)[]) &pmKierunek,NULL,NULL };

static POLE pmWym_luk_o[2] = {
		{u8"R",'R',209,NULL},
		{u8"L",'L',201,NULL} };

static TMENU mWym_luk_o = { 2,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,5,0,0,0,(POLE(*)[]) &pmWym_luk_o,NULL,NULL };

static POLE pmWym_okrag_o[2] = {
		{u8"D",'D',211,NULL},
		{u8"R",'R',212,NULL} };

static TMENU mWym_okrag_o = { 2,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmWym_okrag_o,NULL,NULL };

static POLE pmWymiarujm[] = {
		{u8"Kierunek\0=\0        ",'K',196,&mKierunek},
		{u8"Nowy\0End",'N',197,NULL},
		{u8"Usuń\0Del",'U',198,NULL},
		{u8"Edycja\0F10",'E',199,NULL},
		{u8"Zmienne\0",'Z',200,NULL},
		{u8"Łuk\0R\0             ",'L',201,&mWym_luk_o},
		{u8"Okrąg\0D\0           ",'O',202,&mWym_okrag_o}, };

#define _NEW_ u8"nowy"

static POLE pmStrzalka[3] = {
	  {u8"[A] <---------->",'A',189,NULL},
	  {u8"[T] -/--------/-",'T',190,NULL},
	  {u8"[D] -*--------*-",'D',191,NULL} };

static POLE pmKierunekT[3] = {
	{u8"Równoległy",'R',192,NULL},
    {u8"Współliniowy",'W',696,NULL},
	{u8"Poziomy",'P',193,NULL} };

static POLE pmAddLine[2] = {
	{u8"stała długość",'S',194,NULL},
	{u8"dop. do obiektu",'D',195,NULL} };

static POLE pmLFormat[4] = {
	{u8"Dziesiętny",'D',0,NULL},
    {u8"Inżynieryjny",'I',0,NULL},
	{u8"Architektoniczny",'A',0,NULL},
    {u8"Ułamkowy",'U',0,NULL},
};

static POLE pmFractions[9] = {
    {u8"0",   '0',0,NULL},
	{u8"1/2",  '2',0,NULL},
    {u8"1/4",  '4',0,NULL},
    {u8"1/8",  '8',0,NULL},
    {u8"1/16", '1',0,NULL},
    {u8"1/32", '3',0,NULL},
    {u8"1/64", '6',0,NULL},
    {u8"1/128",'A',0,NULL},
    {u8"1/256",'B',0,NULL},
};

static TMENU mKolorL = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,20,37,5,0,CMNU,CMBR,CMTX,7,13,8,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };   //pmKolorL
static TMENU mKolorT = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,20,37,6,0,CMNU,CMBR,CMTX,7,30,8,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };   //pmKolorT

static TMENU mStrzalka = { 3,0,0,19,37,12,ICONS,CMNU,CMBR,CMTX,0,64,0,0,0,(POLE(*)[]) &pmStrzalka,NULL,NULL };

static TMENU mKierunekT = { 3,0,0,14,37,14,ICONS,CMNU,CMBR,CMTX,0,67,0,0,0,(POLE(*)[]) &pmKierunekT,NULL,NULL };

static TMENU mAddLine = { 2,0,0,16,37,15,ICONS,CMNU,CMBR,CMTX,0,70,0,0,0,(POLE(*)[]) &pmAddLine,NULL,NULL };

static TMENU mItalics = { 2,0,0,7,37,9,ICONS,CMNU,CMBR,CMTX,0,47,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static TMENU mBold = { 2,0,0,7,37,10,ICONS,CMNU,CMBR,CMTX,0,49,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static TMENU mLFormat = { 4,0,0,14,37,14,0,CMNU,CMBR,CMTX,0,51,0,0,0,(POLE(*)[]) &pmLFormat,NULL,NULL };

static TMENU mFractions = { 9,0,0,14,37,14,0,CMNU,CMBR,CMTX,0,55,0,0,0,(POLE(*)[]) &pmFractions,NULL,NULL };

extern PTMENU mCzcionkaW;

static POLE pmZmienne[] = {
	  {u8"֎kolor Linii\0 Biały\0              ", 'L',177,&mKolorL},
	  {u8"֎kolor Tekstu\0 Biały\0             ", 'T',178,&mKolorT},
	  {u8"Wysokość tekstu\0 3\0               ", 'W',179,NULL},
	  {u8"wsp. Szerokości tekstu\0            ", 'S',180,NULL},
	  {u8"tekst Pochylony\0 N\0               ", 'P',181, &mItalics},
	  {u8"tekst poGrubiony\0 N\0              ", 'G',182,&mBold},
	  {u8"Czcionka\0                          ", 'C',183, &mCzcionkaW},
      {u8"Format numeryczny\0                 ", 'F',697, &mLFormat},
	  {u8"Dokładność\0 .01\0                  ", 'D',184,NULL},
	  {u8"ogRaniczniki\0 <--->\0              ", 'R',185,&mStrzalka},
	  {u8"Odsunięcie linii\0 10\0             ", 'O',186,NULL},
	  {u8"Kierunek tekstu\0  R\0              ", 'K',187,&mKierunekT} ,
	  {u8"liniA pomocnicza\0 S\0              ", 'A',188,&mAddLine} };

char *_LFORMAT_[]={u8"Dziesiętny",u8"Inżynieryjny",u8"Architektoniczny",u8"Ułamkowy"};

#endif

#ifdef __O_DIVIDE__

POLE pmDiv_Pline[] = {
	{u8"Polilinia",'P',0,NULL},
	{u8"Segment",'S',0,NULL} };

static TMENU mDiv_Pline = { 2, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 8, 0, 0,0,(POLE(*)[]) &pmDiv_Pline, NULL, NULL };
static TMENU mAlign_Block = { 2, 0,0, 7, 83, 13, ICONS, CMNU,CMBR,CMTX,0, 10, 0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmDivide[] = {
	 {u8"Podziel\0",'P',448,NULL},
	 {u8"Liczba segmentów\0 5\0      ", 'L', 449,NULL},
	 {u8"Zmierz\0", 'Z', 450,NULL},
	 {u8"Długość segmentu\0 100.0\0       ", 'D', 451,NULL},
	 {u8"Maximum\0 1000\0",'M', 452,NULL},
	 {u8"polilinia/Segment\0   polilinia\0  ",'S',453, &mDiv_Pline},
	 {u8"Usuń\0Del\0",'U', 270,NULL},
	 {u8"dopasować Blok\0 T\0",'B', 456,&mAlign_Block} };

#define _POLYLINE_ u8"polilinia"
#define _SEGMENT_ u8"segment"

char _YES__[4] = u8"Т";
char _NO__[4] = u8"N";

#define _DIVIDE_ u8"poDziel"
#define _DIVIDE_C_ L'D'

#endif

#ifdef __O_KONSTR__

static char* opis_zbrojenia[] = { u8"Wkładka zbrojeniowa" };

#endif

#ifdef __O_RECTA__

static POLE	pmKatR[]= {
     {u8"obróć O kąt",'O',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",   	'0',578,NULL},
	 {u8"obróć pod Kątem",'K',440,NULL},
	 {u8"Równolegle do",'R',580,NULL},
	 {u8"Prostopadle do",'P',581,NULL},
	 {u8"pod kątem Do",'D',582,NULL},
};

TMENU mKatR = { 13,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmKatR,NULL,NULL };

static POLE	pmRectOpt[]= {
	 {u8"Dwa punkty",L'D',22,NULL},
     {u8"dwa punkty i punkt Wstawienia/kąt",L'W',684,NULL},
};

TMENU mRectOpt = { 2,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,13,0,0,0,(POLE(*)[]) &pmRectOpt,NULL,NULL };

static POLE	pmKatOptR[]= {
     {u8"obróć O kąt",'O',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",   	'0',578,NULL},
	 {u8"obróć pod Kątem",'K',440,NULL},
	 {u8"Równolegle do",'R',580,NULL},
	 {u8"Prostopadle do",'P',581,NULL},
	 {u8"pod kątem Do",'D',582,NULL},
     {u8"Opcja rysowania",L'O',685,&mRectOpt},
};

TMENU mKatOptR = { 14,0,0,10,74,6,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmKatOptR,NULL,NULL };

#define _RECTA_A_ u8"Prostokąt: kąt osi i opcje rysowania"
#define _RECTA_R_ u8"Prostokąt: obrót"
#define _RECTA_C_ L'P'

#endif


#ifdef __O_LTTYPE__

POLE pmTTF_OTF[] = {
   {u8"czcionki TrueType",'T',703,NULL},
   {u8"czcionki OpenType",'O',704,NULL},
};

TMENU mTTF_OTF = { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmTTF_OTF,NULL,NULL };

#endif