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

#define __EDIT_MULTILINE__ L"Edit multiline text"

#define FILENAME_INI L"ALFACAD.INI"
LPWSTR filename_ini = (LPWSTR)L"Edit ALFACAD.INI";

// Create a locale object representing the locale
#define _LOCALE_ "en-US"

char __BOLD__[]=u8"Bold";
char __UNDERLINED__[]=u8"Underlined";
char __ITALIC__[]=u8"Italics";
char __A_TO_LEFT__[]=u8"Adjust to the left";
char __A_TO_RIGHT__[]=u8"Adjust to the right";
char __A_TO_MIDDLE__[]=u8"Adjust to the middle";
char __A_TO_CENTRE__[]=u8"Adjust to the centre";
char __HIDDEN__[]=u8"Hidden";
char __COPY_TEXT__[]=u8"Copy text from clipboard";

char _EDIT_TEXT_[]=u8"Edit text";
char _EDIT_FILE_[]=u8"Edit file";

#endif

#ifdef __WINPRINT__

#define _NEW_WINDOW_ u8"Do you want to open new drawing window?"
#define _NEW_WINDOW_T_ u8"New window"

int max_quote = 21675;
#define _QUOTE_ u8"quotes.dat"
char about_pre[32] = u8"\n\nQuote of the day ";
char _about_[16] = u8"about ";
#endif

#ifndef NULL
#define NULL		((void *)(0))
#endif

#ifdef __BIBRYS__

POLE pmNie_Tak_[] = {
	{u8"No",'N',113,NULL},
	{u8"Yes",'Y',112,NULL} };

POLE pmEditPCX[] = {
   {u8"Modify dx, dy", 'M',114, NULL},
   {u8"change Parameters",'P',54, NULL},
   {u8"Calibration",'C',115,NULL},
   // {u8"Edit block", 'E',0, NULL},
};

TMENU mEditPCX = { 3,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmEditPCX,NULL,NULL };
TMENU mSaveLayer = { 2,0,0,7,16,8,ICONS,CMNU,CMBR,CMTX,0,18,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmInsertPCX[] = {
   {u8"insert Map\0\0 ",'M',600,NULL},
   {u8"insert photo as PNG\0\0 ",'P',601,NULL},
   {u8"insert photo as JPG\0\0 ",'J',624,NULL},
   {u8"vectorize to ALX and import\0\0 ",'A',698,NULL},
};

TMENU mInsertPCX = { 4,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,23,0,0,0,(POLE(*)[]) &pmInsertPCX,NULL,NULL };

POLE pmElliptical[] = {
   {u8"Ellipse",'E',19,NULL},
   {u8"elliptical Disc",'D',707,NULL},
   {u8"elliptical Arc",'A',706,NULL},
};

TMENU mElliptical = { 3,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmElliptical,NULL,NULL };

POLE pmInsetDraw[] = {
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

TMENU mInsetDraw = { 0,0,0,6,16,8,ICONS,CMNU,CMBR,CMTX,0,23,0,0,0,(POLE(*)[]) &pmInsetDraw,NULL,NULL };

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

TMENU mInsetEdit = { 0,0,0,6,16,8,ICONS,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmInsetEdit,NULL,NULL };

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

TMENU mInsetBlock = { 0,0,0,6,16,8,ICONS,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmInsetBlock,NULL,NULL };

POLE pmArray[] = {
	{u8"Rectangular",	'R', 116,NULL},
	{u8"Circular", 'C', 117,NULL},
	{u8"circular With rotation", 'W', 118,NULL},
};

TMENU mArray = { 3, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, 20, 0, 0,0,(POLE(*)[]) &pmArray, NULL, NULL };

POLE pmMirrorBlock[] = {
	{u8"mirror block X",'X',40,NULL}, 	   
	{u8"mirror block Y",'Y',41,NULL}, 	   
};

TMENU mMirrorBlock = { 2, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, 23, 0, 0,0,(POLE(*)[]) &pmMirrorBlock, NULL, NULL };

POLE  pmEdycja[] = {
	{u8"Move\0\0",           'M',30,NULL},
	{u8"Copy\0\0",           'C',31,NULL},
    {u8"Delete\0\0",         'D',32,NULL},
    {u8"< restore\0\0",      '<',33,NULL},
	{u8"Rotate\0\0",         'R',34,NULL},
    {u8"*block rotate\0\0",  '*',35,NULL},
	{u8"scaLe\0\0",          'L',36,NULL},
	{u8"Stretch\0\0",        'S',37,NULL},
	{u8"Edit text\0\0",      'E',38,NULL},
	{u8"mIrror\0\0",         'I',39,NULL},
	{u8"mirror block\0\0",   'X',535,&mMirrorBlock},
	{u8"Array\0\0",          'A',42,&mArray},
	{u8"Break\0\0",          'B',44,NULL},
	{u8"Trim\0\0",           'T',45,NULL},
	{u8"exteNd\0\0",         'N',46,NULL},     
	{u8"Fillet\0\0",         'F',47,NULL},
	{u8"chamfer\0\0",        '/',48,NULL},
	{u8"Offset\0\0",         'O',49,NULL},
	{u8"diVide\0\0",         'V',50,NULL},
	{u8"@ undo\0         \0",'@',51,NULL},
    {u8"plUgins",            'U',534,&mInsetEdit},

};

POLE pmRysuj[] =
{
	{u8"Line",           'L',13,NULL},
	{u8"Circle",         'C',14,NULL},
	{u8"Arc",            'A',15,NULL},
	{u8"Text",           'T',16,NULL},
	{u8"Multiline text", 'M',408,NULL},       
	{u8"polyGon",        'G',17,NULL},
	{u8"Disc",           'D',18,NULL},
	{u8"Elliptical",     'E',705,&mElliptical},
	{u8"Polyline",       'P',20,NULL},
	{u8"Solid",          'S',21,NULL},
	{u8"Bezier spline",  'B',516,NULL},
	{u8"Rectangle",      'R',22,NULL},
	{u8"trace",          '=',23,NULL},
	{u8"2-line",         '2',24,NULL},
	{u8"3-line",         '3',25,NULL},
	{u8"sKetch",         'K',26,NULL},
	{u8"pOint",          'O',27,NULL},
    {u8"Vector",         'V',722,NULL},
	{u8"Function chart", 'F',28,NULL},
    {u8"plUgins",        'U',534,&mInsetDraw},
};

POLE pmBlok[] = {
	{u8"Copy block\0\0 ",       'C',58,NULL},
	{u8"Paste block\0\0 ",      'P',59,NULL},
	{u8"Export block\0\0 ",     'E',61,NULL},
	{u8"Import block\0\0 ",     'I',60,NULL},
	{u8"Open catalog\0\0 ",     'O',460,NULL},
	{u8"iMport DXF block\0\0 ", 'M',84,NULL},
	{u8"iNsert image\0\0 ",     'N',62,&mInsertPCX},
	{u8"edit image Block\0\0 ", 'B',63,&mEditPCX},
	{u8"change Hatch block pattern\0\0 ",'H',599,NULL},
	{u8"image exporT\0\0 ",     'T',64,NULL},
	{u8"Group\0\0 ",            'G',65,NULL},
	{u8"eXplode\0\0 ",          'X',66,NULL},
    {u8"Quadrilateral transformation\0\0 ",'Q',682,NULL},
	{u8"List\0\0 ",             'L',67,NULL},
	{u8"* explode frozen\0\0 ", '*',68,NULL},
	{u8"Delete DXF blocks\0\0 ",'D',69,NULL},
	{u8"coRrect blocks\0\0 ",   'R',70,NULL},
	{u8"Fixed layers\0 N\0   ", 'F',71,&mSaveLayer},
    {u8"plUgins",               'U',534,&mInsetBlock},
};

POLE  pmGeometria[] = {
	{u8"surface Area",              'A',72,NULL},  	
	{u8"Center of area",            'C',73,NULL},     
	{u8"First moment of area",      'F',74,NULL},     
	{u8"area Moment of inertia",    'M',75,NULL},     
	{u8"centr.area mom. of Inertia",'I',76,NULL},     
	{u8"Test of closed area",       'T',77,NULL},     
};

char _YES_[4] = "Y";
char _NO_[4] = "N";

const char *_EDIT_[] = {u8"Move", u8"move Z", u8"Rotate", u8"rotate XZ", u8"rotate YZ", u8"Scale", u8"Mirror", u8"->skip", u8"-----"};

//char* loading_program[] = { u8"Loading program", u8"Loading resources...", u8"Please wait. Downloading file: ", u8"Connecting to the cloud..."};

#define _NO_MOUSE_ "Mouse not installed !!!!!\n"

#endif

#ifdef __O_WYJ__

#ifdef LINUX
#define ALF_PROGRAM "AlfaCAD"
#define ALF_TITLE "AlfaCAD4Linux"
#else
#ifdef BIT64
#define ALF_PROGRAM "AlfaCAD4Win64.exe"
#define ALF_TITLE "AlfaCAD4Win64"
#else
#define ALF_PROGRAM "AlfaCAD4Win.exe"
#define ALF_TITLE "AlfaCAD4Win"
#endif
#endif
static char err_message[] = u8"Error processing DWG file";
char err_message_code[] = u8"Exit code :";
char err_message1[] = u8"Error processing interim file.";
static char confirm[] = u8"Confirm";

POLE pmNewDrawing[] = {
	{u8"New drawing",'N',79,NULL},
	{u8"new from Template",'T',533,NULL},
};

static TMENU mNewDrawing = { 2,0,0,32,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,18,0,0,0, (POLE(*)[]) &pmNewDrawing,NULL,NULL };

POLE pmNewWindow[] = {
	{u8"New drawing in new window",'N',79,NULL},
	{u8"new from Template in new window",'T',533,NULL},
	{u8"Open file in new window",'O',78,NULL},
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
	{u8"Enter",'E',537,NULL},
	{u8"Return",'R',538,NULL},
};

static TMENU mTree = { 2,0,0,16,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmTree,NULL,NULL };

POLE pmLastFiles[] = {
	{u8"1 \0\0",'1',0,NULL},
	{u8"2 \0\0",'2',0,NULL},
	{u8"3 \0\0",'3',0,NULL},
	{u8"4 \0\0",'4',0,NULL},
	{u8"5 \0\0",'5',0,NULL},
	{u8"6 \0\0",'6',0,NULL},
	{u8"7 \0\0",'7',0,NULL},
	{u8"8 \0\0",'8',0,NULL},
	{u8"9 \0\0",'9',0,NULL},
    {u8"0 \0 0",'0',0,NULL},
	{u8"Q \0\0",'Q',0,NULL},
	{u8"W \0\0",'W',0,NULL},
	{u8"E \0\0",'E',0,NULL},
	{u8"R \0\0",'R',0,NULL},
	{u8"T \0\0",'T',0,NULL},
	{u8"Y \0\0",'Y',0,NULL},
};

static TMENU mLastFiles = { 16,0,0,33,20,7,TADD,CMNU,CMBR,CMTX,0,25,0,0,0,(POLE(*)[]) &pmLastFiles,NULL,NULL };

POLE pmQuit[] = {
	 {u8"Open drawing\0",'O',78,NULL},
	 {u8"New drawing\0",'N',79,&mNewDrawing},
	 {u8"new Window\0",'W',80,&mNewWindow},
	 {u8"Save\0",'S',81,NULL},
	 {u8"save As\0",'A',82,NULL},
	 {u8"Tree structure\0",'T',536,&mTree},
	 {u8"Export\0",'E',504,&mExportCAD},
	 {u8"Import\0",'I',505,&mImportCAD},
	 {u8"title Block edit\0", 'B',85, NULL},
	 /* {u8"Jump to system\0",'J',86,NULL},*/
	 {u8"Printing\0",'P',87,NULL},
	 {u8"Legacy plotting\0",'L',394,NULL},
	 {u8"Info\0",'?',88,NULL},
	 {u8"show Desktop\0", 'D',528, NULL},
	 {u8"last Files\0",'F',89,&mLastFiles}
};

static POLE pmTTF_OTF[] = {
   {u8"TrueType Fonts",'T',703,NULL},
   {u8"OpenType Fonts",'O',704,NULL},
};

static TMENU mTTF_OTF = { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmTTF_OTF,NULL,NULL };

static POLE pmCzcionkaEkranTTF[] = {
   {u8"Font type  \0", 'F',111,&mTTF_OTF},
   {u8"Hight\0\0", 'H',179,NULL},
   {u8"Width factor\0 \0", 'W',230,NULL},
};

TMENU mCzcionkaEkranTTF = { 3,0,0,30,20,7,ICONS | TADD,CMNU,CMBR,CMTX,0,32 /*30*/,0,0,0,(POLE(*)[]) &pmCzcionkaEkranTTF,NULL,NULL };

static POLE pmWindow[] = {
   {u8"expand Horizontally",'H',467,NULL},
   {u8"expand Vertically",'V',468,NULL},
   {u8"expand Diagonally",'D',469,NULL},
   {u8"expand Window",'W',470,NULL},
   {u8"restore Last",'L',471,NULL},
};

static TMENU mWindow = { 5,0,0,16,22,9,ICONS,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmWindow,NULL,NULL };


static POLE pmDialogCursor[] = {
   {u8"Small",'S',591,NULL},
   {u8"Big",'B',592,NULL},
   {u8"Huge",'H',832,NULL},
};

static TMENU mDialogCursor = { 3,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,27 /*25*/,0,0,0,(POLE(*)[]) &pmDialogCursor,NULL,NULL };

#define smallcursor u8"Small"
#define bigcursor u8"Big"
#define hugecursor u8"Huge"

static POLE pmMenuCursor[] = {
   {u8"Bar",'B',821,NULL},
   {u8"Cursor",'C',822,NULL},
};

static TMENU mMenuCursor = { 2,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,37 /*35*/,0,0,0,(POLE(*)[]) &pmMenuCursor,NULL,NULL };

#define barstyle u8"Bar"
#define cursorstyle u8"Cursor"

static POLE pmTranslucency[] = {
   {u8"100%",'0',546,NULL},
   {u8"90%",'9',545,NULL},
   {u8"80%",'8',544,NULL},
   {u8"70%",'7',543,NULL},
   {u8"60%",'6',542,NULL},
   {u8"50%",'5',541,NULL},
   {u8"40%",'4',540,NULL},
   {u8"30%",'3',539,NULL},
   {u8"20%",'2',666,NULL},
   {u8"10%",'1',664,NULL},
};


static TMENU mTranslucency = { 10,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,17,0,0,0,(POLE(*)[]) &pmTranslucency,NULL,NULL };
static TMENU mGTranslucency = { 10,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,17,0,0,0,(POLE(*)[]) &pmTranslucency,NULL,NULL };

static TMENU mDemoSelect = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,30 /*28*/,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static TMENU mAutoPan = { 2,0,0,7,33,11,ICONS,CMNU,CMBR,CMTX,0,35 /*33*/,0,0,0,(POLE(*)[]) & pmTak_Nie,NULL,NULL };

static POLE pmMouseWheel[] = {
   {u8"Natural",'N',834,NULL},
   {u8"Regular",'R',835,NULL},
};

static TMENU mMouseWheel = { 2,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,41 /*39*/,0,0,0,(POLE(*)[]) &pmMouseWheel,NULL,NULL };

static POLE pmOpacity[] = {
   {u8"Fills",'F',866,&mTranslucency},
   {u8"Gradients",'G',867,&mGTranslucency},
};

static TMENU mOpacity =  { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmOpacity,NULL,NULL };

POLE pmOpcje[] = {
{u8"Drawing board configuration\0",'D',109,NULL},
{u8"auto Pan\0Yes\0 ",'P',145, &mAutoPan},  //PAN
{u8"Auto-panning increment\0",'A',110,NULL},
//{u8"Opacity of fills\0",'O',542,&mTranslucency},
{u8"Opacity of fills/gradients\0",'O',542,&mOpacity},
{u8"desktop Font\0 ",'F',111,&mCzcionkaEkranTTF},
{u8"desktop Background\0",'B',527,NULL},
{u8"dialog Cursor\0",'C',590,&mDialogCursor},
{u8"Menu style\0",'M',820,&mMenuCursor},
{u8"Zoom/scroll wheel\0",'Z',833,&mMouseWheel},
{u8"Educational demo mode\0 N\0",'E',661,&mDemoSelect},
{u8"Save window setup\0",'S',530,NULL},
{u8"Window setup\0",'W',478,&mWindow},
};

#define _NATURAL__ u8"Natural"
#define _REGULAR__ u8"Regular"

#define _FILE_ u8"File "
#define _FILE_EXISTS_ u8" exists. Overwrite it ? (Yes/No/Esc):"

#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "Y"
#define _NO__ "N"
#define _INTERRUPT_ u8"Block '%s' doesn't exist! Printing interrupted. "
#define _SPOOLED_INTERRUPT_ u8"Error writing spooled file ! Printing interrupted. "
#define _FORMAT_INTERRUPT_ u8"No '*FORMAT' block. ! Printing terminated"
#define _PAGE_INTERRUPT_ u8"No '*PAGE' block ! Printing terminated"
#define _CONVERSION_TERMINATION_ u8"WAIT !!! Converting file. Termination -> Esc... "
#define _ERROR_DXF_ u8"Error in DXF file or termintion  "
#define _LINE_NO_ u8" -> line %#ld"

#define _TO_MANY_WIN_ u8"Too many open windows"
#define _CLOSE_WINDOW_ u8"Do you want to close the window ?"

#define _DRAWING_NOT_SAVED_ u8"Drawing not saved"

#define _Yes_ u8"Yes"
#define _No_ u8"No"

#define _SOME_OPEN_DRAWING_NOT_SAVED_ u8"Some open drawings are not saved"

#define _SAVE_IT_ u8"Save it ? "
#define _SAVE_THEM_ u8"Save them ? "

#define _NO_MORE_DRAWINGS_ u8"No more drawings"

#define _NEW_DRAWING_ u8"New drawing"

#define _SAVING_CONF_ u8"Saving configuration";

#define _WANT_EXIT_ u8"Do you want to exit program ?"

#define _WANT_CLOSE_WINDOW_ u8"Do you want to close the window ?"

#define _SAVING_PREFILES_ "Saving HISTORY"

#define _FONT_CHANGED_ u8"Font has been changed"

#define desktop_tip "@Select drawing"

#define _DEMO_MODE_ "In DEMO pressed keys and mouse buttons"
#define _DEMO_MODE1_ "are shown at the bottom of the window"


#endif

#ifdef __ASKING__

#define _Yes_ u8"Yes"
#define _No_ u8"No"

#define _NOTICE_ u8"Notice"
#define _AT_NOTICE_  u8"@Notice"
#define _COMBINATION_  u8"Load combination number"
#define _GEOM_STIFFNESS_ u8"Geometric stiffness"
#define _INERTIA_ u8"Include Dead Load (DL) in vibrations"
#define _VIBRATIONS_ u8"Number of dynamic modes of vibration"
#define _PINNABLE_ u8"Use pinned joint substitutes as virtual, near-zero stiffness members \n(safe computation alternative, especially for dynamic analyses)"

#define _INSTALATION_ u8"Installation"


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

char instruct_txt[][64]={u8"Open menu / Accept",
                        u8"Close menu / Escape",
                        u8"Freeing the mouse (or shake it left and right)",
                        u8"Auxiliary menu",
                        u8"Auxiliary context menu",
                        u8"Orthogonality on/off",
                        u8"Zooming / scrolling",
                        u8"Panning (Mickey Mouse hand)",
                        u8"Command repetition",
                        u8"Osnap choice (there are more)",
                        u8"Helping hand (all hotkeys)",
                        u8"(left) changing keyboard layout in editing",
                        u8"for Greek letters and math symbols",
                        u8"are sticking keys. In case of conflict with",
                        u8"system shortkeys, press them first, release,",
                        u8"then press another key of the combination",
                        u8"Editing numeric values or re-editing text",
                        u8"оr"};

#define osnap_icon0 156
#define osnap_icon1 164
#define osnap_icon2 165
#define osnap_icon3 160
#define osnap_icon4 154
#define osnap_icon5 167

char stay_string[64]="Do not show it again";

#define _Yes_ u8"Yes"
#define _No_ u8"No"

#define _INSTRUCTION_ u8"Instruction"
#define _AT_INSTRUCTION_  u8"@Instruction"

#endif

#ifdef __DIALOG__

#define _OTHER_ u8"֍other>>>"
#define _LAYERS_ u8"Layers"

#endif

#ifdef __ADDMENU__

#define MAXPLACEMARKER 5

#define _YES_ "Y"
#define _NO_ "N"
static char ggsel[10][16] = { u8"Endpoint", u8"Nearest end", u8"Middle", u8"Intersection", u8"Center", u8"Perpendicular",
				  "Tangential", "Adjacent", "point","----" };


POLE pmKolor[] = {
	/*1*/      {u8"֍[A] red",'A',0,NULL},  		/*12*/
	/*2*/      {u8"֍[B] yellow",'B',0,NULL},                   /*14*/
	/*3*/      {u8"֍[C] green",'C',0,NULL},                 /*10*/
	/*4*/      {u8"֍[D] turquoise" ,'D',0,NULL},               /*11*/
	/*5*/      {u8"֍[E] blue",'E',0,NULL},               /*9*/
	/*6*/      {u8"֍[F] magenta",'F',0,NULL},             /*13*/
	/*7*/      {u8"֍[G] white",'G',0,NULL},                   /*15*/
	/*8*/	   {u8"֍[H] dark grey",'H',0,NULL},            /*8*/
	/*9*/      {u8"֍[I] dark red",'I',0,NULL},         /*4*/
	/*10*/	   {u8"֍[J] brown",'J',0,NULL},                /*6*/
	/*11*/     {u8"֍[K] dark green",'K',0,NULL},          /*2*/
	/*12*/     {u8"֍[L] dark turquoise",'L',0,NULL},        /*3*/
	/*13*/     {u8"֍[M] dark blue",'M',0,NULL},        /*1*/
	/*14*/     {u8"֍[N] dark magenta",'N',0,NULL},      /*5*/
	/*15*/	   {u8"֍[O] grey",'O',0,NULL},                  /*7*/
	/*16*/	   {u8"֍[P] black",'P',0,NULL},                 /*0*/
	/*17*/     {u8"֍[X] other\0 0\0  ",'X',0,NULL}
};

POLE pmKolorL_[] = {
	/*1*/      {u8"֍[A] red",'A',0,NULL},  		/*12*/
	/*2*/      {u8"֍[B] yellow",'B',0,NULL},                   /*14*/
	/*3*/      {u8"֍[C] green",'C',0,NULL},                 /*10*/
	/*4*/      {u8"֍[D] turquoise" ,'D',0,NULL},               /*11*/
	/*5*/      {u8"֍[E] blue",'E',0,NULL},               /*9*/
	/*6*/      {u8"֍[F] magenta",'F',0,NULL},             /*13*/
	/*7*/      {u8"֍[G] white",'G',0,NULL},                   /*15*/
	/*8*/	   {u8"֍[H] dark grey",'H',0,NULL},            /*8*/
	/*9*/      {u8"֍[I] dark red",'I',0,NULL},         /*4*/
	/*10*/	   {u8"֍[J] brown",'J',0,NULL},                /*6*/
	/*11*/     {u8"֍[K] dark green",'K',0,NULL},          /*2*/
	/*12*/     {u8"֍[L] dark turquoise",'L',0,NULL},        /*3*/
	/*13*/     {u8"֍[M] dark blue",'M',0,NULL},        /*1*/
	/*14*/     {u8"֍[N] dark magenta",'N',0,NULL},      /*5*/
	/*15*/	   {u8"֍[O] grey",'O',0,NULL},                  /*7*/
	/*16*/	   {u8"֍[P] black",'P',0,NULL},                 /*0*/
	/*17*/     {u8"֍[X] other\0 0\0  ",'X',0,NULL}
};

POLE pmKolorT_[] = {
	/*1*/      {u8"֍[A] red",'A',0,NULL},  		/*12*/   //֍red
	/*2*/      {u8"֍[B] yellow",'B',0,NULL},                   /*14*/
	/*3*/      {u8"֍[C] green",'C',0,NULL},                 /*10*/
	/*4*/      {u8"֍[D] turquoise" ,'D',0,NULL},               /*11*/
	/*5*/      {u8"֍[E] blue",'E',0,NULL},               /*9*/
	/*6*/      {u8"֍[F] magenta",'F',0,NULL},             /*13*/
	/*7*/      {u8"֍[G] white",'G',0,NULL},                   /*15*/
	/*8*/	   {u8"֍[H] dark grey",'H',0,NULL},            /*8*/
	/*9*/      {u8"֍[I] dark red",'I',0,NULL},         /*4*/
	/*10*/	   {u8"֍[J] brown",'J',0,NULL},                /*6*/
	/*11*/     {u8"֍[K] dark green",'K',0,NULL},          /*2*/
	/*12*/     {u8"֍[L] dark turquoise",'L',0,NULL},        /*3*/
	/*13*/     {u8"֍[M] dark blue",'M',0,NULL},        /*1*/
	/*14*/     {u8"֍[N] dark magenta",'N',0,NULL},      /*5*/
	/*15*/	   {u8"֍[O] grey",'O',0,NULL},                  /*7*/
	/*16*/	   {u8"֍[P] black",'P',0,NULL},                 /*0*/
	/*17*/     {u8"֍[X] other\0 0\0  ",'X',0,NULL}
};

TMENU mKolorAC = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,23,56,5,0,CMNU,CMBR,CMTX,15,72,0,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };

#define _OTHER_NO_ u8"֍[X] other>>> %#ld"

static POLE pmKrok_k[9] = {
	 {u8"[A]\0  2.5  *",'A',0,NULL},
	 {u8"[B]\0  1.25 *",'B',0,NULL},
	 {u8"[C]\0  0.25 *",'C',0,NULL},
	 {u8"[D]\0  0.5",'D',0,NULL},
	 {u8"[E]\0  1",'E',0,NULL},
	 {u8"[F]\0  5",'F',0,NULL},
	 {u8"[G]\0 10",'G',0,NULL},
	 {u8"[X]\0 X\0     ",'X',0,NULL},
	 {u8"Minimal\0",'M',0,NULL} };

static TMENU mKrok_k = { 9,0,0,13,52 ,6,TADD,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmKrok_k,NULL,NULL };

POLE pmTak_Nie[2] = {
	{u8"Yes",'Y',112,NULL},
	{u8"No",'N',113,NULL} };

POLE pmGruboscLinii[] = {
	   {u8"҂Very thin",'V',0,NULL},
	   {u8"҂thiN",'N',0,NULL},
	   {u8"҂Thick",'T',0,NULL},
	   {u8"҂very thicK",'K',0,NULL},
	   {u8"҂Extra thick",'E',0,NULL},
};

POLE pmGruboscLiniiS[] = {
       {u8"Filling",'F',0,NULL},
	   {u8"҂Very thin",'V',0,NULL},
	   {u8"҂thiN",'N',0,NULL},
	   {u8"҂Thick",'T',0,NULL},
	   {u8"҂very thicK",'K',0,NULL},
	   {u8"҂Extra thick",'E',0,NULL},
};

static POLE pmTypyLinii[] = {
		   {u8"֏continuous",'1',0,NULL},
		   {u8"֏dashed",'2',0,NULL},
		   {u8"֏dashdotted",'3',0,NULL},
		   {u8"֏2-dotted",'4',0,NULL},
		   {u8"֏dotted",'5',0,NULL},
		   {u8"֏border",'6',0,NULL},
		   {u8"֏center",'7',0,NULL},
		   {u8"֏hidden",'8',0,NULL},
		   {u8"֏phantom",'9',0,NULL},
		   {u8"֏dashed_2",'A',0,NULL},
		   {u8"֏dashdotted_2",'B',0,NULL},
		   {u8"֏2-dotted_2",'C',0,NULL},
		   {u8"֏dotted_2",'D',0,NULL},
		   {u8"֏border_2",'E',0,NULL},
		   {u8"֏center_2",'F',0,NULL},
		   {u8"֏hidden_2",'G',0,NULL},
		   {u8"֏phantom_2",'H',0,NULL},
		   {u8"֏dashed_x2",'I',0,NULL},
		   {u8"֏dashdotted_x2",'J',0,NULL},
		   {u8"֏2-dotted_x2",'K',0,NULL},
		   {u8"֏dotted_x2",'L',0,NULL},
		   {u8"֏border_x2",'M',0,NULL},
		   {u8"֏center_x2",'N',0,NULL},
		   {u8"֏hidden_x2",'O',0,NULL},
		   {u8"֏phantom_x2",'P',0,NULL},
		   {u8"֏3-dotted",'Q',0,NULL},
		   {u8"֏2-center",'R',0,NULL},
		   {u8"֏3-dashed",'S',0,NULL},
		   {u8"֏phantod_3",'T',0,NULL},
		   {u8"֏2-center_x2",'U',0,NULL},
		   {u8"֏3-dashed_x2",'V',0,NULL},
		   {u8"֏multidashed",'W',0,NULL},
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
	 {u8"pointer Step\0",        'S',154,&mKrok_k},   /*.25*/
	 {u8"Grid density\0",        'G',155,&mGestosc }, /*2.5*/
	 {u8"grid points\0",               'K',156,&mPunkty_siatki},
	 {u8"Orthogonality (F8)\0",            'O',157,&mOrto},
	 {u8"֎Color\0white\0",     'C',158,&mKolorAC},   /*Biały*/
	 {u8"Line thickness\0Thick\0",  'L',159,&mGruboscLinii},
	 {u8"line Type\0Continuous\0",   'T',160,&mTypyLinii},   /*Ciągła*/
	 {u8"points & outlines Visibility\0 Y\0",  'V', 161,&mPoint_View},
	 {u8"Point size\0 2.5\0",      'P',162,NULL},
	 {u8"get coloR     (Alt-F8)\0",          'R',163,NULL},
	 {u8"get typE      (Alt-F9)\0",          'E',164,NULL},
	 {u8"get lAyer     (Alt-F10)\0",         'A',165,NULL},
	 {u8"Free block    \0",             'F',167,&mSwobodnyBlok},
	 {u8"free text   \0",             'J',166,&mSwobodnyTekst},
	 {u8"Dimension block scaling\0 N\0", 'D',168,&mScale_DIM},
	 {u8"stretching lines in Nodes\0 T\0",'N',169,&mCiagniecieLinii},
	 {u8"Breaking lines in nodes\0 T\0",'B',170,&mRozcinanieLinii},
	 {u8"Hiding types\0 T\0",            'H',171,&mUkrywanieTypu},
	 {u8"hidden teXt visibility\0 N\0",'X',172,&mWidocznoscTypu},
	 {u8"filling Image points\0 N\0",   'I',173,&mWypelnianiePCX},
	 {u8"highlight the current laYer\0 N\0",'Y',174,&mView_Only_C_L},
	 {u8"image bUffering\0 T\0",           'U',175,&mBuffering_PCX},
	 //  {u8"visibility of sheet blocks\0 N\0",' ',0,&mView_Sheets},
	 {u8"text normaliZation\0",           'Z',176,&mNormalizacjaTekstu},
};

TMENU mParametrym = { 23,0,0,38,23,4,ICONS | TADD,CMNU,CMBR,CMTX,0,59,0,0,0,(POLE(*)[]) &pmParametry,NULL,NULL };

POLE pmParametry_view[] = {
	 {u8"pointer step\0",        'A',154,&mKrok_k},   /*.25*/
	 {u8"Grid density\0",        'G',155,&mGestosc }, /*2.5*/
	 {u8"# grid points\0",               '#',156,&mPunkty_siatki},
	 {u8"point Visibility\0 T\0",  'V', 161,&mPoint_View},
	 {u8"point Size\0 2.5\0",      'S',162,NULL},
	 {u8"hidden teXt visibility\0 N\0",'X',172,&mWidocznoscTypu},
	 {u8"highlight the current laYer\0 N\0",'Y',174,&mView_Only_C_L},
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
	 {u8"Refresh\0",'R',142,NULL},
	 {u8"Save view\0", 'S', 140,&mSetMarker},
	 {u8"Find view\0", 'F', 141,&mFindMarker},
	 {u8"All\0",'A',143,NULL},
	 {u8"Center\0",'C',144,NULL},
	 ////{u8"auto Pan\0Yes\0     ",'P',145, &mAutoPan},  //PAN
	 {u8"Pan (Ctrl-Shift)\0",'P',145, NULL},
	 {u8"[1] *1.0 \0",'1',146,NULL},
	 {u8"[5] *0.5\0",'5',147,NULL},
	 {u8"[2] *0.2\0",'2',148,NULL},
	 {u8"[0] *0.1\0",'0',149,NULL},
	 {u8"*x\0",'*',150,NULL},
	 {u8"Window\0",'W',151,NULL},
	 {u8"Drawing\0",'D',152,NULL},
	 {u8"Last\0",'T',153,NULL},
	 {u8"move to x;y\0",'X',129,NULL},
	 {u8"move by dx;dy\0",'Y',130,NULL},
	 {u8"move polar L;a\0",'L',131,NULL},
	   }; /*rownolegle nalezy modyfikowac funkcje SkalaZ  (1.,.5,.2,.1)*/

TMENU mSkala = { 17,0,0,20,12,4,ICONS | TADD,CMNU,CMBR,CMTX,0, 30, 0,0,0,(POLE(*)[]) &pmSkala,NULL,NULL };

static POLE pmTryb[] = {
	{u8"Endpoint\0",'E',120,NULL},
	{u8"Nearest end\0",'N',121,NULL},
	{u8"Middle\0",'M',122,NULL},
	{u8"Intersection\0",'I',123,NULL},
	{u8"Center\0",'C',124,NULL},
	{u8"Perpendicular\0",'P',125,NULL},
	{u8"Tangential\0",'T',126,NULL},
	{u8"Adjacent\0",'A',127,NULL},
	{u8"pOint\0",'O',128,NULL},
	{u8"none\0",'X',132,NULL} };

static TMENU mTryb = { 10,0,0,20,20,6,ICONS,CMNU,CMBR,CMTX,0, 13, 0,0,0,(POLE(*)[]) &pmTryb,NULL,NULL };

static POLE pmPunkt[] = {
	{u8"Repeating mode\0               ",'R',119,&mTryb},
	{u8"Endpoint\0",'E',120,NULL},
	{u8"Nearest end\0",'N',121,NULL},
	{u8"Middle\0",'M',122,NULL},
	{u8"Intersection\0",'I',123,NULL},
	{u8"Center\0",'C',124,NULL},
	{u8"Perpendicular\0",'P',125,NULL},
	{u8"Tangential\0",'T',126,NULL},
	{u8"Adjacent\0",'A',127,NULL},
	{u8"point\0",'O',128,NULL},
	{u8"move to X, Y\0",'X',129,NULL},
	{u8"move by dX, dY\0",'Y',130,NULL},
	{u8"move polar L, a\0",'L',131,NULL}, };

TMENU mPunkt = { 13,0,0,30,1,4,ICONS | TADD,CMNU,CMBR,CMTX,0, 7, 0,0,0,(POLE(*)[]) &pmPunkt,NULL,NULL };


static POLE pmSum[] = {
	 {u8"Stop adding\0 F10",'S',139,NULL},
	 {u8"Add / subtract\0 F9",'A',137,NULL},
};

static TMENU mSum = { 2, 0,0,23,56,9,ICONS | TADD,CMNU,CMBR,CMTX,0, 7, 0, 0,0,(POLE(*)[]) &pmSum, NULL,NULL };

POLE pmMeasure[] = {
	{u8"Calculator", 'C', 133,NULL},
	{u8"Point",      'P',134,NULL},
	{u8"Distance",   'D',135,NULL},
	{u8"Angle",      'A',136,NULL},
	{u8"Summation",    'S', 137,&mSum},
	{u8"property List", 'L', 138,NULL},
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

TMENU mInsetAux = { 0,0,0,6,0,0,ICONS,CMNU,CMBR,CMTX,0,173,0,0,0,(POLE(*)[]) &pmInsetAux,NULL,NULL };

//52 pointfinder, 53 camera, 54 parameters, 55 layers, 56 measure, 3 search
static POLE pmenup[] = {
	{u8"Point\0\0 ",        L'P',52, &mPunkt},
	{u8"View\0 1\0    ",    L'V',53, &mSkala},
	{u8"pArameters\0\0 ",   L'A',54,&mParametrym},
	{u8"Layers\0\0 ",       L'L',55,NULL},
	{u8"Measure, info & changes\0\0 ",      L'M',56, &mMeasure},
	{u8"Find\0\0 ",         L'F',3,NULL},
    {u8"plUgins\0\0",       'U',534,&mInsetAux},
	{u8"\0              \0 ",L' ',0,NULL} };

TMENU menup = { 8,0,0,12,1,2,ICONS | TADD | NVERT,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmenup,NULL,NULL };   //7,0,11,

#endif

#ifdef __LAYERS__

#define NoWidthLines	5 /*3*/
#define NoTypeLines	32 /*5*/
static char* width_line_txt_L[NoWidthLines + 1] = { u8"҂very thin",u8"҂thin",u8"҂thick",u8"҂very thick",u8"҂extra thick",u8"" };

static char* Colors_Name_txt_L[MAXCOLOR + 1] =
{
	u8"֍red" ,
	u8"֍yellow" ,
	u8"֍green" ,
	u8"֍turquoise",
	u8"֍blue",
	u8"֍magenta",
	u8"֍white",
	u8"֍dark grey" ,
	u8"֍dark red" ,
	u8"֍brown" ,
	u8"֍dark green" ,
	u8"֍dark turquoise" ,
	u8"֍dark blue" ,
	u8"֍dark magenta" ,
	u8"֍grey" ,
	u8"֍black" ,
	u8"֍other>>>",
	u8"",
};

static char* type_line_txt_L[NoTypeLines + 1] = {
				u8"֏continuous",
				u8"֏dashed",
				u8"֏dashdotted",
				u8"֏2-dotted",
				u8"֏dotted",
				u8"֏border",
				u8"֏center",
				u8"֏hidden",
				u8"֏phantom",
				u8"֏dashed_2",
				u8"֏dashdotted_2",
				u8"֏2-dotted_2",
				u8"֏dotted_2",
				u8"֏border_2",
				u8"֏center_2",
				u8"֏hidden_2",
				u8"֏phantom_2",
				u8"֏dashed_x2",
				u8"֏dashdotted_x2",
				u8"֏2-dotted_x2",
				u8"֏dotted_x2",
				u8"֏border_x2",
				u8"֏center_x2",
				u8"֏hidden_x2",
				u8"֏phantom_x2",
				u8"֏3-dotted",
				u8"֏2-center",
				u8"֏3-dashed",
				u8"֏phantom_3",
				u8"֏2-center_x2",
				u8"֏3-dashed_x2",
				u8"֏multidashed",
				u8"" 
};

#define layer_comm0 "PgUp"
#define layer_comm1 "PgDn"
#define layer_comm2 "New"
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
#define layer_comm15 "Current"
#define layer_comm16 "Layer name"
#define layer_comm17 ""
#define layer_comm18 "Visible"
#define layer_comm19 "Editable"
#define layer_comm20 "Pointable"
#define layer_comm21 "Color"
#define layer_comm22 "Line"
#define layer_comm23 "Thickness"
#define layer_comm24 "Line" 
#define layer_comm25 "Type"
#define layer_comm26 "All layers"
#define layer_comm27 "All layers"
#define layer_comm28 "Gray"
#define layer_comm29 "B/W"
#define layer_comm30 "On"
#define layer_comm31 "Off"
#define layer_comm32 "Modify layers"
#define layer_comm33 "Erase"
#define layer_comm34 "Layers"
#define layer_comm35 "Restore"

#define layer_tip0 "@Visibility"
#define layer_tip1 "Editability"
#define layer_tip2 "Localizability"
#define layer_tip3 "Visibility of all layers"
#define layer_tip4 "Editability of all layers"
#define layer_tip5 "Localizability of all layers"
#define layer_tip6 "Color"
#define layer_tip7 "Line width"
#define layer_tip8 "Line type"
#define layer_tip9 "on Printer and plotter"
#define layer_tip10 "on Plotter and printer"
#define layer_tip11 "Gray on printing"
#define layer_tip12 "Black & White on printing"
#define layer_tip13 "All layers gray on printing"
#define layer_tip14 "All layers Black & White"
#define layer_tip15 "All layers"
#define layer_tip16 "Erase layer"
#define layer_tip17 "Mark block of layers"

#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "Y"
#define _NO__ "N"
#define _Yes_ u8"Yes"
#define _No_ u8"No"
#define _ERASE_LAYER_ u8"WARNING: Do you want to delete the entire layer with all its contents?"
#define _FROM_ u8"From"
#define _TO_ u8"To"
#endif

#ifdef __CHPROP__

#define NoWidthLines	6 /*5*/ /*3*/
#define NoTypeLines	32/*5*/
#define NoTextType 16
#define NoTextJust 4

char* Colors_Name_txt[MAXCOLOR + 1] =
{
	u8"֍red" ,
	u8"֍yellow" ,
	u8"֍green" ,
	u8"֍turquoise",
	u8"֍blue",
	u8"֍magenta",
	u8"֍white",
	u8"֍dark grey" ,
	u8"֍dark red" ,
	u8"֍brown" ,
	u8"֍dark green" ,
	u8"֍dark turquoise" ,
	u8"֍dark blue" ,
	u8"֍dark magenta" ,
	u8"֍grey" ,
	u8"֍black" ,
	u8"֍other>>>",
	"",
};

#define chprop_comm0 u8"Changing properties of objects"
#define chprop_comm1 u8"Changing"
#define chprop_comm2 u8"other properties"

static char NoChange[] = "No change";

static char* width_line_txt[NoWidthLines + 1] = { u8"҂very thin",u8"҂thin",u8"҂thick",u8"҂very thick",u8"҂extra thick",u8"҂Fillings", NoChange };

char* type_line_txt[NoTypeLines + 1] = {
				u8"֏continuous",
				u8"֏dashed",
				u8"֏dashdotted",
				u8"֏2-dotted",
				u8"֏dotted",
				u8"֏border",
				u8"֏center",
				u8"֏hidden",
				u8"֏phantom",
				u8"֏dashed_2",
				u8"֏dashdotted_2",
				u8"֏2-dotted_2",
				u8"֏dotted_2",
				u8"֏border_2",
				u8"֏center_2",
				u8"֏hidden_2",
				u8"֏phantom_2",
				u8"֏dashed_x2",
				u8"֏dashdotted_x2",
				u8"֏2-dotted_x2",
				u8"֏dotted_x2",
				u8"֏border_x2",
				u8"֏center_x2",
				u8"֏hidden_x2",
				u8"֏phantom_x2",
				u8"֏3-dotted",
				u8"֏2-center",
				u8"֏3-dashed",
				u8"֏phantom_3",
				u8"֏2-center_x2",
				u8"֏3-dashed_x2",
				u8"֏multidashed",
				NoChange };

static char* tab_type_txt[NoTextType + 1] = {
				u8"Normal",
				u8"Internal label",
				u8"component Label",
				u8"Symbol",
				u8"Type",
				u8"*pin name",
				u8"power supply",
				u8"Port",
				u8"Attribute",
				u8"+Scheme",
				u8"File name",
				u8"netWork",
				u8"Comment",
				u8"%variable",
				u8"aUx.symbol",
				u8"auX.description",
				NoChange };


static char* font_just_txt[NoTextJust + 1] = {
					u8"Left",
					u8"Right",
					u8"Middle",
					u8"Central",
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

static char* vector_txt[5] =
        {u8"rigid-rigid",
        u8"rigid-pinned",
        u8"pinned-rigid",
        u8"pinned-pinned",
        NoChange };

static char* springs_txt[3] =
         {u8"rigid beam",
         u8"Winkler beam",
         NoChange };

#define chprop_tips0 u8"@Layer"
#define chprop_tips1 u8"Color"
#define chprop_tips2 u8"Line width"
#define chprop_tips3 u8"Line type"
#define chprop_tips4 u8"Font type"
#define chprop_tips5 u8"Text type"
#define chprop_tips6 u8"Hidden text"
#define chprop_tips7 u8"Text alignment"
#define chprop_tips8 u8"Font height"
#define chprop_tips9 u8"Font width"
#define chprop_tips10 u8"Italic font"
#define chprop_tips11 u8"Bold font"
#define chprop_tips12 u8"Underlined text"
#define chprop_tips13 u8"Yes"
#define chprop_tips14 u8"No"
#define chprop_tips15 u8"Flip horizontally"
#define chprop_tips16 u8"Flip vertically"
#define chprop_tips17 u8"On top"
#define chprop_tips18 u8"Opacity of solids, traces and solid colour hatching"
#define chprop_tips19 u8"Style of the vector, appropriate to the coordinate system"

static char* add_new_font_c = u8"add new font";

#endif

#ifdef __LOADF__

static char copy_button[] = u8"Copy\0from file";
static char parametry_p[] = u8"Parameters:";
static char edycja_n_p[] = u8"Parameter name:";
static char edycja_p[] = u8"Parameter value:";
static char lista_p[] = u8"List of parameters:";
static char nowy_katalog[] = u8"new folder";
static char usun_zbior[] = u8"delete file";
static char usun_katalog[] = u8"delete folder";

char* dlg_name[] =
{
   u8"Load drawing from file",
   u8"Save drawing to file",
   u8"Import block",
   u8"Import image file",
   u8"Conver image to ALX and import",
   u8"Export PCX",
   u8"Export BMP",
   u8"Export PNG",
   u8"Export JPEG",
   u8"Export TIFF",
   u8"Export EPS",
   u8"Export PDF",
   u8"Import profile",
   u8"Import 3D object",
   u8"Save block",
   u8"Conversion drawing to new format",
   u8"Conversion block to new format",
   u8"Edit table",
   u8"Convert drawing from DXF format",
   u8"Save drawing in DXF format",
   u8"Save makro file",
   u8"Load makro file",
   u8"Save printer configuration",
   u8"Load printer configuration",
   u8"Save catalog file",
   u8"Load catalog file",
   u8"Save coordinates file",
   u8"Save network route record",
   u8"Load route from coordinates file",
   u8"Open catalog",
   u8"Save plotter configuration",
   u8"Load plotter configuration",
   u8"Import DXF block",
   u8"Convert drawing from DWG format",
   u8"Save drawing in DWG format",
   u8"Import DWG block",
   u8"Load desktop font",
   u8"Load new font",
   u8"Save static analysis result file",
};

#define nameplate_tip u8"@Nameplate of drawing (drawing ID)"
#define  edit_table u8"Edit table"

#define _BLOCK_DESCRIPTION_ u8"Block description:"
#define _BLOCK_TYPE u8"Block type:"

#define _Yes_ u8"Yes"
#define _No_ u8"No"

#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#define _FILE_EXISTS_ u8"File exists"
#define _OVERWRITE_IT_ "Overwrite it ? "

#endif // __LOADF__

#ifdef __INFO__

/*
char* typ_punktu_tab[] = { u8"Simple",u8"Base point",u8"Pin","?","?","?","?",u8"Pin",
                           u8"Junction","?","?","?","?","?","?","?",
                           u8"?","?","?","?","?","?","?","?",
                           u8"?","?","?","?","?","?","?","?",
                           u8"?","?","?","?","?","?","?","?",
                           u8"?","?","?","?","?","?","?","?",
                           u8"?","?","?","?","?","?","?","?",
                           u8"?","?","?","?","?","?","?","?",
                           };
*/

/*
char* vector_style_tab[] = { u8"rigid-rigid",u8"rigid-pin",u8"pin-rigid",u8"pin-pin",
                             u8"Force",u8"Moment+",u8"Moment-",
                             u8"Displacement", u8"Rotation+", u8"Rotation-",
                             u8"trapezoidal Vertical load",
                             u8"trapezoidal Horizontal load",
                             u8"trapezoidal Perpendicular load",
                             u8"horizontal projection of vertical load",
                             u8"vertical projection of a horizontal load",
                             u8"thermal load",
                             u8"node size (radius)",
                             u8"slab surface Load",
                             u8"slab concentrated Force load",
                             u8"?","?","?","?","?","?","?",
                             u8"?","?","?","?","?","?","?","?",
                             u8"?","?","?","?","?","?","?","?",
                             u8"?","?","?","?","?","?","?","?",
                             u8"?","?","?","?","?","?","?","?",
                             u8"?","?","?","?","?","?","?","?",
                           };
*/
char* view_width_tab1[] = { u8"very thin",u8"thin",u8"thick",u8"very thick",u8"extra thick",u8"filling",u8"",u8"invisible" };

char* tab_typ_tekstu[] = { u8"Normal", u8"Internal label",u8"component Label",u8"Symbol",u8"Type",
				u8"*pin name",u8"power supply",u8"Port",u8"Attribute",u8"+Scheme",u8"File name",u8"netWork",
				u8"Comment",u8"%variable",u8"aUx.symbol",u8"auX.description" };

char* tab_justowanie[] = { u8"to Left |<",u8"to Right >|",u8"Middle ><",u8"Central _><_" };

static POLE pmInfoAbout[] = {
	 {u8"Object type\0 ",'O',320,NULL},      //0
	 {u8"Layer\0 ",'L',305,NULL},      //1
	 {u8"֎Color\0",'C',158,NULL},      //2
	 {u8"line Type\0 ",'T',160,NULL},      //3
	 {u8"line Width\0",'W',159,NULL},      //4
     {u8"edge support type\0",'J',850,NULL},      //4
     {u8"edge support invertion\0",'K',853,NULL},      //4
     {u8"Point type\0",'P',27,NULL},       //5
     {u8"Vector type\0 ",'V',770,NULL},      //6
	 {u8"X1 \0 ",'1',306,NULL},      //7
	 {u8"Y1 \0 ",'2',307,NULL},      //8
	 {u8"X2 \0 ",'3',308,NULL},      //9
	 {u8"Y2 \0 ",'4',309,NULL},      //10
	 {u8"X3 \0 ",'5',310,NULL},      //11
	 {u8"Y3 \0 ",'6',311,NULL},      //12
	 {u8"X4 \0 ",'7',312,NULL},      //13
	 {u8"Y4 \0 ",'8',313,NULL},      //14
	 {u8"Radius \0 ",'R',212,NULL},      //15
     {u8"Radius Y \0 ",'r',710,NULL},      //16
     {u8"Angle \0",'@',107,NULL},      //17
	 {u8"Angle1 \0 ",'<',314,NULL},      //18
	 {u8"Angle2 \0 ",'>',315,NULL},      //19

     {u8"Initial width \0",'-',249,NULL},         //20
     {u8"Final width \0",'=',249,NULL},        //21
     {u8"Intensity \0",'^',727,NULL},        //22
     {u8"Initial Intensity \0",'(',766,NULL},       //23
     {u8"Final intensity \0",')',767,NULL},       //24
     {u8"Initial axis offset \0",'/',250,NULL},       //25
     {u8"Final axis offset \0",'\\',250,NULL},      //26

     {u8"Load character & variant\0 ",'?',798,NULL},     //27
     //{u8"Load variant \0 ",'?',799,NULL},     //28

     {u8"Opacity\0",'%',542,NULL},      //29
	 {u8"length / perimeter\0",'D',210,NULL},      //30
	 {u8"DX\0",'X',316,NULL},      //31
	 {u8"DY\0",'Y',317,NULL},      //32
	 {u8"Surface area\0",'S',7,NULL},        //33
	 {u8"Font\0",'F',111,NULL},      //34
	 {u8"Type\0",'E',231,NULL},      //35
	 {u8"hidden\0",'!',232,NULL},      //36
	 {u8"Allign\0",'A',233,NULL},      //37
	 {u8"Height \0",'H',179,NULL},     //38
	 {u8"Width factor \0",'*',230,NULL},     //39
	 {u8"Italic\0",'I',181,NULL},      //40
     {u8"Bold\0",'B',182,NULL},      //41
     {u8"Underlined\0",'U',409,NULL},      //42
     {u8"line Spacing\0",'#',410,NULL},      //43
     {u8"pixel size dx \0",'Q',689,NULL},      //44
     {u8"pixel size dy \0",'Z',690,NULL},      //45
	 {u8"Inner block name\0",'N',318,NULL},      //46
	 {u8"Outer block name\0",'G',319,NULL},      //47
};
TMENU mInfoAbout = { 49,0,0,40,2, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmInfoAbout, NULL, NULL };

static POLE pmInfo[] = {
	 {u8"memory Size for drawing/default [KB] \0 ",'S',297,NULL},
	 {u8"image's Virtual memory/Used [MB] \0 ",'V',298,NULL},
	 {u8"Drawing data size [ B] \0 ",'D',299,NULL},
	 {u8"Printing buffer [KB] \0 ",'P',300,NULL},
	 {u8"Macro buffer [B] \0 ",'M',301,NULL},
	 {u8"Image buffer/Virtual screen segment [KB]\0 ",'I',302,NULL},
	 {u8"Current folder \0 ",'C',303,NULL},
	 {u8"Graphic mode \0 ",'G',304,NULL},
	 {u8"desktop Font \0 ",'F',111,NULL},
     {u8"Logo \0 ",'L',812,NULL},
};

TMENU mInfo = { 10,0,0,64,1, 3, ICONS | TADD, CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmInfo, NULL, NULL, NULL };

char* objects[] = { u8"Line",u8"Line 3D",u8"Text",u8"Arc",u8"Circle",u8"Disc",u8"Polygon/Solid",u8"Solid 3D",u8"Point",u8"Spline",u8"Image",u8"Polyline",u8"Trace",u8"Hatch",u8"Elliptical Arc",u8"Ellipse", u8"Elliptical Disc", u8"Solid Arc", u8"Vector"};

#define __PLATE__ u8" (PLATE)"
#define __HOLE__  u8" (HOLE)"
#define __WALL__  u8" (WALL)"
#define __ZONE__  u8" (ZONE)"

#define _FILLING_ u8"filling"

char *vector_txt[]={u8"Vector: rigid-rigid",u8"Vector: rigid-pin",u8"Vector: pin-rigid",u8"Vector: pin-pin",u8"Vector: Force",u8"Vector: Moment",u8"Vector: -Moment",
                    u8"Vector: Displacement",u8"Vector: Rotation",u8"Vector: -Rotation",u8"Vector: trapezium Y load",u8"Vector: trapezium X load",u8"Vector: trapezium N load",u8"Vector: trapezium H load",u8"Vector: trapezium V load",u8"Vector: Thermal load", u8"Vector: node size (Radius)", u8"slab surface Load", u8"slab concentrated slab force Load",
                    u8"Vector: Force Z",u8"Vector: trapezium Z load", u8"Vector: Moment X",u8"Vector: -Moment X",u8"Vector: Moment Y",u8"Vector: -Moment Y",u8"Vector: Moment XY",u8"Vector: -Moment XY",
                    u8"Vector: Displacement Z", u8"Vector: Rotation X",u8"Vector: -Rotation X",u8"Vector: Rotation Y",u8"Vector: -Rotation Y",u8"Vector: Rotation XY",u8"Vector: -Rotation XY",
                    };

//char *point_txt[]={"Simple","Base point",'','','','','',"Junction","pin point",'','','',"Fixed","fixed L","fixed R","fixed U","Pinned","pinned L","pinned R","pinned U","fixed Roller","fixed roller L","fixed roller R","fixed roller U",
//     "pinned rOller","pinned roller L","pinned roller R","pinned roller U"};

unsigned short vector_wcod[]={L'A',L'B',L'C',L'D',L'F',L'M',L'-',L'E',L'R',L'O',L'Y',L'X',L'N',L'H',L'V',L'T',L'0',L'P',L'S',L'W',L'Z',L'Q',L'L',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'!',L'@',L'#',L'$',L'%',L'^',};

unsigned short point_wcod[]={L'S', L'B', ' ', ' ', ' ', ' ',' ', L'J', L'C', ' ', ' ', ' ',L'F', L'1', L'2', L'3', L'P', L'4', L'5', L'6', L'R', L'7', L'8', L'9', L'O', '0', '-', '=', '+', 'X', 'A', 'Y', 'C', 'V', 'E', 'N', 'D'};
unsigned short object_wcod[]={L'I', L'L', L'T', L'A', L'C', L'D', L'S', L'P', L'*', L'G', L'O', L'E', L'F', L'B', L'V', ' '};
char *object_txt[]={u8"Image",u8"Line",u8"Text",u8"Arc",u8"Circle",u8"Disc",u8"Solid",u8"Point",u8"Block",u8"Solid Arc",u8"Elliptical Arc",u8"Ellipse",u8"Filled Ellipse",u8"Bezier Spline",u8"Vector",""};

#endif

#ifdef __ALLEGROEXT__

static char* desktop_data_param[] =
{
  "Wallpaper",
  "Panorama",
  "Dynamic menu",
  "Desktop cursor",
  "Menu style",
  "Wheel style",
  "Instruction",
};

static int no_desktop_data_param = sizeof(desktop_data_param) / sizeof(desktop_data_param[0]);

static char* desktop_data_param_comment[] =
{
	";",
	//";Screen saver delay in min or 0 (off)",
	";desktop autopanning factor",
	";",
	"; small 0, big 1, huge 2",
    "; bar 0, pointer 1",
    "; 0-natural, 1-regular",
    "; showing short instruction on start",
};


#define _UNABLE_INSTALL_GRAPHIC_ u8"Unable to initialize graphics mode"
#define _ERROR_READING_DAT_ u8"Error reading file: alfacad.dat\n"

#define _MACRO_C__ L'a'
#define _EXECUTE_C__ L'e'
#define _Go_AHEAD_ u8"OK. Go ahead."

#define _UPGRADE_READY_ u8"Upgrade is ready to download and install"
#define _UPGRADE_RESTART_ u8"Do you want to upgrade and restart now?"
#define _TOOLS_TO_INSTALL_ u8"not installed"
#define _INSTALL_TOOLS_ u8"Install necessary packages"

char _Yes_[]="Yes";
char _No_[]="No";
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#endif

#ifdef __O_LINE__

static POLE pmAxis1000[] = {
	      {u8"Axis length\0      \0",L'A',267,NULL} };

static POLE pmAxis50[] = {
		  {u8"Axis length\0      \0",L'A',267,NULL} };

static POLE pmLine[] = {
		  {u8"Close\0",L'C',217,NULL},
		  {u8"Remove\0Del\0",L'R',218,NULL},
		  {u8"Length\0             0\0",L'L',219,NULL} };

static POLE pmPLine[] = {
		  {u8"Close\0",L'C',217,NULL},
		  {u8"Remove\0Del\0",L'R',218,NULL},
		  {u8"Length\0             0\0",L'L',219,NULL},
		  {u8"Arc\0", L'A', 15,NULL},
          {u8"No support edge\0", L'N', 851,NULL},
          {u8"Pinned support edge\0", L'P', 850,NULL},
          {u8"Fixed support edge\0", L'F', 852,NULL},
          {u8"Movable support edge\0", L'M', 849,NULL},
          {u8"flip Support\0", L'S', 853,NULL},
};

POLE pmEdgeType[] = {
            {u8"No support edge\0", L'N', 851,NULL},
            {u8"Pinned support edge\0", L'P', 850,NULL},
            {u8"Fixed support edge\0", L'F', 852,NULL},
            {u8"Movable support edge\0", L'M', 849,NULL}
          };

static POLE pmLine_Con[] = {
		  {u8"Continuation",L'C',220,NULL} };

static POLE pmPLineObrys[] = {
		  {u8"Close\0",L'C',217,NULL},
		  {u8"Remove\0Del\0",L'R',218,NULL},
		  {u8"Length\0             0\0",L'L',219,NULL},
		  {u8"Arc\0", L'A', 15,NULL},
		  {u8"daSh-dotted\0    \0",L'S',268,NULL},
		  {u8"Dashed\0    \0",L'D',269,NULL} };

static POLE pmLine_ConObrys[] = {
		  {u8"Continue",L'C',217,NULL},
		 {u8"daSh-dotted\0    \0",L'S',268,NULL},
		 {u8"Dashed\0    \0",L'D',269,NULL} };

#define _AXIS_ u8"Axis"
#define _AXIS_C_ L'A'
#define _AXIS_H_ u8"Axis #"
#define _AXIS_H_C_ L'A'
#define _LINE_ u8"Line"
#define _LINE_C_ L'L'

#define _LINE_3D_ u8"Line3D"

#define _POLYLINE_ u8"Polyline"
#define _POLYLINE_C_ L'P'

#define _POLYLINE_3D_ u8"Polyline3D"
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
	{u8"none",L'0',797,NULL},
	{u8"Dead load",L'D',787,&mLoadVariant},
    {u8"Live load",L'L',788,&mLoadVariant},
    {u8"live Roof load",L'R',789,&mLoadVariant},
    {u8"Wind load",L'W',790,&mLoadVariant},
    {u8"Snow load",L'S',791,&mLoadVariant},
    {u8"Earthquake load",L'E',792,&mLoadVariant},
    {u8"Ice or rainwater load",L'I',793,&mLoadVariant},
    {u8"Hydraulic load from soil",L'H',794,&mLoadVariant},
    {u8"hydraulic load from Fluid",L'F',795,&mLoadVariant},
    //{u8"Thermal load",L'T',796,&mLoadVariant},
};

//static
TMENU mLoad_Char = { 10, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 18, 0, 0,0,(POLE(*)[]) &pmLoad_Char, NULL, NULL };

POLE pmLoad_Char_Thermal[] = {
	{u8"none",L'0',797,NULL},
    {u8"Thermal load",L'T',796,&mLoadVariant},
};

//static
TMENU mLoad_Char_Thermal = { 2, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 18, 0, 0,0,(POLE(*)[]) &pmLoad_Char_Thermal, NULL, NULL };

char *Vector_txt[]={u8"rigid - rigid",  //0
                    u8"rigid - pin",
                    u8"pin - rigid",
                    u8"pin - pin",
                    u8"Force",
                    u8"Moment",
                    u8"-moment",
                    u8"displacement",
                    u8"Rotation",
                    u8"-rotation",
                    u8"trapezium Y load",
                    u8"trapezium X load",
                    u8"trapezium N load",
                    u8"trapezium H load",
                    u8"trapezium V load",
                    u8"Thermal load",
                    u8"node size (radius)",  //16

                    u8"plate surface load",  //17
                    u8"plate concentrated force Load",  //18
                    u8"Force Z",  //19
                    u8"trapezium Z load", //20
                    u8"Moment XZ",  //21
                    u8"-moment XZ",
                    u8"Moment YZ",
                    u8"-moment YZ",
                    u8"Moment XY",
                    u8"-moment XY",  //26
                    u8"displacement Z",  //27
                    u8"Rotation XZ",  //28
                    u8"-rotation XZ",
                    u8"Rotation YZ",
                    u8"-rotation YZ",
                    u8"Rotation XY",
                    u8"-rotation XY",  //33
                    u8"beam on a Winkler foundation"};  //34

static POLE pmVector_cartesian[] = {
		  {u8"Undo\0Del\0",L'U',218,NULL},
          {u8"rigid - rigid\0\0",L'A',723,NULL},
          {u8"rigid - pin\0\0",L'B',724,NULL},
          {u8"pin - rigid\0\0",L'C',725,NULL},
          {u8"pin - pin\0\0",L'D',726,NULL},
          {u8"Force\0\0",L'F',727,&mLoad_Char},
          {u8"Moment\0\0",L'M',728,&mLoad_Char},
          {u8"-moment\0\0",L'-',729,&mLoad_Char},
          {u8"displacement\0\0",L'E',730,NULL}, //&mLoad_Char},  //NULL
          {u8"Rotation\0\0",L'R',731,NULL}, //&mLoad_Char},  //NULL
          {u8"-rotation\0\0",L'O',732,NULL}, //&mLoad_Char},  //NULL
          {u8"trapezium Y load\0\0",L'Y',733,&mLoad_Char},
          {u8"trapezium X load\0\0",L'X',734,&mLoad_Char},
          {u8"trapezium N load\0\0",L'N',735,&mLoad_Char},
          {u8"trapezium H load\0\0",L'H',736,&mLoad_Char},
          {u8"trapezium V load\0\0",L'V',737,&mLoad_Char},
          {u8"Thermal load\0\0",L'T',752,&mLoad_Char_Thermal},
          {u8"node size (radius)\0\0",L'0',786,NULL},

          {u8"slab Plate or shield\0\0",L'P',846,NULL},
          {u8"Space in plate or shield\0\0",L'S',844,NULL},
          {u8"Wall under plate\0\0",L'W',843,NULL},
          {u8"Zone in plate or shield\0\0",L'Z',842,NULL},
          {u8"plate surface load\0\0",L'Q',845,&mLoad_Char},
          {u8"plate concentrated force Load\0\0",L'L',856,&mLoad_Char},  //18

          ////for grids
          ////{u8"Force Z\0\0",L'1',880,&mLoad_Char},
          ////{u8"trapezium Z load\0\0",L'2',881,&mLoad_Char},
          ////{u8"Moment XZ\0\0",L'3',882,&mLoad_Char},
          ////{u8"-moment XZ\0\0",L'4',883,&mLoad_Char},
          ////{u8"Moment YZ\0\0",L'5',884,&mLoad_Char},
          ////{u8"-moment YZ\0\0",L'6',885,&mLoad_Char},
          ////{u8"Moment XY\0\0",L'7',886,&mLoad_Char},
          ////{u8"-moment XY\0\0",L'8',887,&mLoad_Char},

          ////{u8"displacement Z\0\0",L'9',888,NULL},
          ////{u8"Rotation XZ\0\0",L'!',889,NULL},
          ////{u8"-rotation XZ\0\0",L'@',890,NULL},
          ////{u8"Rotation YZ\0\0",L'#',891,NULL},
          ////{u8"-rotation YZ\0\0",L'$',892,NULL},
          ////{u8"Rotation XY\0\0",L'%',893,NULL},
          ////{u8"-rotation XY\0\0",L'^',894,NULL},
};

static POLE pmVector_isometric[] = {
		  {u8"Undo\0Del\0",L'U',218,NULL},
          {u8"rigid - rigid\0\0",L'A',723,NULL},   //0
          {u8"beam on a Winkler foundation\0\0",L'W',878,NULL},  //39
          ////{u8"rigid - pin\0\0",L'B',724,NULL},  //1
          ////{u8"pin - rigid\0\0",L'C',725,NULL},  //2
          ////{u8"pin - pin\0\0",L'D',726,NULL},    //3
          {u8"Force\0\0",L'F',727,&mLoad_Char},     //4
          ////{u8"Moment\0\0",L'M',728,&mLoad_Char},  //5
          ////{u8"-moment\0\0",L'-',729,&mLoad_Char},  //6
          {u8"displacement\0\0",L'E',730,NULL}, //&mLoad_Char},  //7
          ////{u8"Rotation\0\0",L'R',731,NULL}, //&mLoad_Char},  //8
          ////{u8"-rotation\0\0",L'O',732,NULL}, //&mLoad_Char},  //9
          {u8"trapezium Y load\0\0",L'Y',733,&mLoad_Char},  //10
          {u8"trapezium X load\0\0",L'X',734,&mLoad_Char},  //11
          {u8"trapezium N load\0\0",L'N',735,&mLoad_Char},  //12
          {u8"trapezium H load\0\0",L'H',736,&mLoad_Char},  //13
          {u8"trapezium V load\0\0",L'V',737,&mLoad_Char},  //14
          {u8"Thermal load\0\0",L'T',752,&mLoad_Char_Thermal},  //15
          {u8"node size (radius)\0\0",L'0',786,NULL},  //16

          ////{u8"slab Plate or shield\0\0",L'P',846,NULL},  //NULL
          ////{u8"Space in plate or shield\0\0",L'S',844,NULL},  //NULL
          ////{u8"Wall under plate\0\0",L'W',843,NULL},  //NULL
          ////{u8"Zone in plate or shield\0\0",L'Z',842,NULL},  //NULL
          ////{u8"plate surface load\0\0",L'Q',845,&mLoad_Char},  //17
          ////{u8"plate concentrated force Load\0\0",L'L',856,&mLoad_Char},  //18

          //for grids
          {u8"Force Z\0\0",L'1',880,&mLoad_Char},  //19
          {u8"trapezium Z load\0\0",L'2',881,&mLoad_Char},  //20
          {u8"Moment XZ\0\0",L'3',882,&mLoad_Char},  //21
          {u8"-moment XZ\0\0",L'4',883,&mLoad_Char},  //22
          {u8"Moment YZ\0\0",L'5',884,&mLoad_Char},  //23
          {u8"-moment YZ\0\0",L'6',885,&mLoad_Char},  //24
          {u8"Moment XY\0\0",L'7',886,&mLoad_Char},  //25
          {u8"-moment XY\0\0",L'8',887,&mLoad_Char},  //26

          {u8"displacement Z\0\0",L'9',888,NULL},  //27
          {u8"Rotation XZ\0\0",L'!',889,NULL},  //28
          {u8"-rotation XZ\0\0",L'@',890,NULL},  //29
          {u8"Rotation YZ\0\0",L'#',891,NULL},  //30
          {u8"-rotation YZ\0\0",L'$',892,NULL},  //31
          {u8"Rotation XY\0\0",L'%',893,NULL},  //32
          {u8"-rotation XY\0\0",L'^',894,NULL},  //33

};

static POLE pmVector_Con[] = {
		  {u8"Continuation",L'C',220,NULL} };

static POLE pmMember_style_cartesian[] = {
          {u8"rigid - rigid\0\0",L'A',723,NULL},
          {u8"rigid - pin\0\0",L'B',724,NULL},
          {u8"pin - rigid\0\0",L'C',725,NULL},
          {u8"pin - pin\0\0",L'D',726,NULL},
};

static POLE pmMember_style_isometric[] = {
          {u8"rigid - rigid\0\0",L'A',723,NULL},
          {u8"beam on a Winkler foundation\0\0",L'W',878,NULL},
};

static POLE pmForce_Displacement_style[] = {
          {u8"Force\0\0",L'F',727,NULL},
          {u8"displacement\0\0",L'E',730,NULL},
};

static POLE pmForce_Displacement_style_isometric[] = {
          {u8"Force Z\0\0",L'F',880,NULL},
          {u8"displacement Z\0\0",L'E',888,NULL},
};

static POLE pmMoment_Rotation_style[] = {
          {u8"Moment\0\0",L'M',728,NULL},
          {u8"-moment\0\0",L'-',729,NULL},
          {u8"Rotation\0\0",L'R',731,NULL},
          {u8"-rotation\0\0",L'O',732,NULL},
};

static POLE pmMoment_Rotation_style_isometric[] = {
          {u8"Moment X\0\0",L'A',882,NULL},
          {u8"-moment X\0\0",L'B',883,NULL},
          {u8"Moment Y\0\0",L'C',884,NULL},
          {u8"-moment Y\0\0",L'D',885,NULL},
          {u8"Moment XY\0\0",L'E',886,NULL},
          {u8"-moment XY\0\0",L'F',887,NULL},
          {u8"Rotation X\0\0",L'G',889,NULL},
          {u8"-rotation X\0\0",L'H',890,NULL},
          {u8"Rotation Y\0\0",L'I',891,NULL},
          {u8"-rotation Y\0\0",L'J',892,NULL},
          {u8"Rotation XY\0\0",L'K',893,NULL},
          {u8"-rotation XY\0\0",L'L',894,NULL},
};

static POLE pmLoad_style[] = {
          {u8"trapezium Y load\0\0",L'Y',733,NULL},
          {u8"trapezium X load\0\0",L'X',734,NULL},
          {u8"trapezium N load\0\0",L'N',735,NULL},
          {u8"trapezium H load\0\0",L'H',736,NULL},
          {u8"trapezium V load\0\0",L'V',737,NULL},
          {u8"trapezium Z load\0\0",L'Z',881,NULL},
};

#define _AXIS_ u8"Axis"
#define _AXIS_C_ L'A'
#define _AXIS_H_ u8"Axis #"
#define _AXIS_H_C_ L'A'
#define _VECTOR_ u8"Vector"
#define _VECTOR_C_ L'V'

#endif

#ifdef __O_LUK__

#define _YES_ "Y"
#define _NO_ "N"

static TMENU mArcReversed = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0, 16 /*14*/,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmLukm[] = {
		  {u8"[1] three points",L'1',221,NULL},
		  {u8"[2] beginning-center-end",L'2',222,NULL},
		  {u8"[3] beginning-center-angle",L'3',223,NULL},
		  {u8"[4] beginning-center-chord",L'4',224,NULL},
		  {u8"[5] beginning-end-radius",L'5',225,NULL},
		  {u8"[6] beginning-end-angle",L'6',226,NULL},
		  {u8"[7] beginning-end-direction",L'7',227,NULL},
		  {u8"[8] continuation",L'8',228,NULL},
          {u8"reversed arc direction",L'0',729,&mArcReversed}};

static POLE pmPLukm[] = {
		  {u8"[1] three points",L'1',221,NULL},
		  {u8"[2] beginning-center-end",L'2',222,NULL},
		  {u8"[3] beginning-center-angle",L'3',223,NULL},
		  {u8"[4] beginning-center-chord",L'4',224,NULL},
		  {u8"[5] beginning-end-radius",L'5',225,NULL},
		  {u8"[6] beginning-end-angle",L'6',226,NULL},
		  {u8"[7] beginning-end-direction",L'7',227,NULL},
		  {u8"[8] continuation",L'8',228,NULL},
          {u8"reversed arc direction",L'0',729,&mArcReversed},
		  {u8"Close",L'C',247,NULL},
		  {u8"Remove: Del",L'R',248,NULL},
		  {u8"Line", L'L',13,NULL},

          {u8"No support edge\0", L'N', 851,NULL},
          {u8"Pinned support edge\0", L'P', 850,NULL},
          {u8"Fixed support edge\0", L'F', 852,NULL},
          {u8"Movable support edge\0", L'M', 849,NULL},
          {u8"flip Support\0", L'S', 853,NULL},
};

static POLE pmPLukmObrys[] = {

		  {u8"[1] three points\0",L'1',221,NULL},
		  {u8"[2] beginning-center-end\0",L'2',222,NULL},
		  {u8"[3] beginning-center-angle\0",L'3',223,NULL},
		  {u8"[4] beginning-center-chord\0",L'4',224,NULL},
		  {u8"[5] beginning-end-radius\0",L'5',225,NULL},
		  {u8"[6] beginning-end-angle\0",L'6',226,NULL},
		  {u8"[7] beginning-end-direction\0",L'7',227,NULL},
		  {u8"[8] continuation\0",L'8',228,NULL},
          {u8"reversed arc direction",L'0',729,&mArcReversed},
		  {u8"Close\0",L'C',247,NULL},
		  {u8"Remove\0Del\0",L'R',248,NULL},
		  {u8"Line\0", L'L',13,NULL},
		  {u8"daSh-dotted\0    \0",L'S',268,NULL},
		  {u8"Dashed\0    \0",L'D',269,NULL}};

#define _ARC_ u8"Arc"
#define _ARC_C_ L'A'

#endif

#ifdef __O_OKRAG__

static const char CIRCLE[] = u8"Circle";
static const char CIRCLE_FILL[] = u8"Disc";

static POLE pmCircle[] = {
	 {u8"Center-radius\0",L'C',213,NULL},
	 {u8"center-Diameter\0",L'D',214,NULL},
	 {u8"2 points\0",L'2',215,NULL},
	 {u8"3 points\0",L'3',216,NULL},
	 {u8"Tangential\0",L'T',266,NULL},
};

#define _CIRCLE_C_ L'C'
#define _DISC_C_ L'D'

#endif

#ifdef __O_POLYGN__

static POLE pmPolygon[] = {
	 {u8"Edge\0 \0",L'E',235,NULL},
	 {u8"Inscribed\0 \0", L'I',237, NULL},
	 {u8"Circumscribed\0  \0",L'C',236, NULL},
	 {u8"Number of edges\0    \0",L'N',606,NULL},
};

#define _POLYGON_ u8"polyGon"
#define _POLYGON_C_ L'G'

#endif

#ifdef __O_ELLIPS__

static POLE pm_ell__draw_type[] = {
		{u8"Axis start",L'A',240,NULL},
		{u8"Center",L'C',241,NULL},
};

static TMENU m_ell__draw_type = { 2,0,0,13,79,7,ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pm_ell__draw_type,NULL,NULL };

static POLE pm_ell_y_draw_method[] = {
		{u8"Distance",L'D',711,NULL},
		{u8"Point",L'P',712,NULL},
        {u8"Tangential",L'T',713,NULL},
};

static TMENU m_ell_y_draw_method = { 3,0,0,13,79,7,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pm_ell_y_draw_method,NULL,NULL };

POLE pm__ellipse[] = {
		//{u8"Approximation\0 20 ",L'A',238, NULL},
        {u8"second semi-axis Method\0 Distance",L'M',710, &m_ell_y_draw_method},
		{u8"axis start/Center\0  axis start",L'C',239, &m_ell__draw_type},
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
		//{u8"Approximation\0 20  ",L'A',238, NULL},
        {u8"second semi-axis Method\0  Distance",L'M',710, &m_ell_y_draw_method},
		{u8"Opacity\0",'O',542,&mTranslucencyFE},
					};

#define _ELLIPSE_ u8"Ellipse"
#define _ELLIPSE_C_ L'E'

POLE pm__ellipticalarc[] = {
		{u8"angle in Local coordinates",L'L',708, NULL},
		{u8"angle in Global coordinates",L'G',709, NULL},
					};

#define _ELLIPSE_ANGLE_ u8"Angle"
#define _ELLIPSE_ANGLE_C_ L'A'

#define _ELLIPTICALDISC_ u8"Disc"
#define _ELLIPTICALDISC_C_ L'D'

static char* EllipticalAngle[] = {u8"Local",u8"Global"};

#endif

#ifdef __O_PLINE__

static POLE pmPLine_Con[] = {
		  {u8"Continuation",L'C',220,NULL} };

extern TMENU *mVector;

static POLE pmPLine_Con_Slab[] = {
		  {u8"Continuation",L'C',220,NULL},
          {u8"Vectors",L'V',722,NULL},
};

#define _POLYLINE_ u8"Polyline"
#define _POLYLINE_C_ L'P'

#define _Yes_ u8"Yes"
#define _No_ u8"No"

#define _CHANGE_POLYLINE_TYPE_ u8"Selected different type of the polyline. Shall we change type to: "

#define __PLATE__ u8"PLATE"
#define __HOLE__  u8"HOLE"
#define __WALL__  u8"WALL"
#define __ZONE__  u8"ZONE"

#endif

#ifdef __O_SOLID__

static char filltyp_tab[7][5] = { u8"F","vt","t","T","VT","ET","P" };

static POLE pmS34[] = {
		{u8"Triangular",'T',246,NULL},
		{u8"Quadrangular",'Q',245,NULL},
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
		{u8"Pattern\0 ",'P',597,NULL},
		{u8"Scale\0 1.0",'S',436,&mPatternScale},
		{u8"Rotate\0 0",'R',35,&mPatternAngle},
		{u8"dX\0 0",'X',622,&mPatternDx},
		{u8"dY\0 0",'Y',623,&mPatternDy},
		{u8"Grab pattern\0",'G',164,NULL},
		{u8"Change existing pattern\0",'C',599,NULL},
};

static TMENU mSolidPattern = { 7,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmSolidPattern,NULL,NULL };

static POLE pmTranslucency[] = {
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

TMENU mTranslucency = { 20,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,45,0,0,0,(POLE(*)[]) & pmTranslucency,NULL,NULL };

static POLE pmFillTyp[] = {
	   {u8"Filled\0",'F',276,&mTranslucency},
	   {u8"Very thin\0",'V',277,NULL},
	   {u8"Thin\0",'T',278,NULL},
	   {u8"tHick\0",'H',279,NULL},
	   {u8"very thicK\0",'K',280,NULL},
	   {u8"Extra thick\0",'E',281,NULL},
	   {u8"Pattern\0",'P',836, &mSolidPattern},
	   //  {u8"- wyłącz obszar",'-',0,NULL},
	   //  {u8"+ Aktywny obszar",'+',0,NULL},
};

static TMENU mFillTyp = { 7,0,0,15,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,5,0,0,0,(POLE(*)[]) &pmFillTyp,NULL,NULL };

POLE pmSolid[] = {
		{u8"Quadrangular/triangular\0  4",'Q',242,&mS34},
		{u8"Triangular\0",'T',243,NULL},
		{u8"Fill/line type\0 F\0     ",'F',244,&mFillTyp},
};

#define _SOLID_ u8"Solid"
#define _SOLID_3D_ u8"Solid 3D"
#define _SOLID_C_ L'S'

#endif

#ifdef __O_SPLINE__

#define _Q_ "Q"
#define _C_ "C"
#define _M_ "M"
#define _B_ "B"

//char _YES__[4] = u8"Y";
//char _NO__[4] = u8"N";

static POLE pmS34[] = {
		{u8"Quadratic Bézier curve",'Q',520,NULL},
		{u8"Cubic Bézier curve",'C',519,NULL},
		{u8"Multi-point cardinal spline",'M',646,NULL},
		{u8"Bézier Spline",'S',662,NULL}
};

static TMENU mS34 = { 4,0,0,11,79,6,ICONS,CMNU,CMBR,CMTX,0,8,0,0,0,(POLE(*)[]) &pmS34,NULL,NULL };

static TMENU mD_T_Pline = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmSpline[] = {
		{u8"Quadratic/cubic/multi-point\0  C\0",'Q',517,&mS34},
		{u8"quadratic Now\0\0",'N',518,NULL},
		{u8"Polyline\0 Y\0",'P',251,&mD_T_Pline},
		{u8"Remove\0Del\0",'R',218,NULL},
		{u8"continuation\0\0",'+',228,NULL},
		{u8"End curve\0End\0",'E',649,NULL},
		{u8"Tension\0\0",'T',648,NULL},
		{u8"Close multi-point curve  Home\0 N\0",'C',647,NULL},
};

#define _SPLINE_ u8"Spline"
#define _SPLINE_C_ L'S'

#define _YES__ u8"Y"
#define _NO__ u8"N"

#endif

#ifdef __O_TRACE__

extern TMENU mPatternScale;
extern TMENU mPatternAngle;
extern TMENU mPatternDx;
extern TMENU mPatternDy;
extern TMENU mTranslucency;

static char tracefilltyp_tab[7][5] = { u8"F","vt","t","T","VT","ET","P" };

static TMENU mD_T_Pline = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,8,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmTracePattern[] = {
		{u8"Pattern\0 ",'P',597,NULL},
		{u8"Scale\0 1.0",'S',436,&mPatternScale},
		{u8"Rotate\0 0",'R',35,&mPatternAngle},
		{u8"dX\0 0",'X',622,&mPatternDx},
		{u8"dY\0 0",'Y',623,&mPatternDy},
		{u8"Grab pattern\0",'G',164,NULL},
		{u8"Change existing pattern\0",'C',599,NULL},
};

static TMENU mTracePattern = { 7,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTracePattern,NULL,NULL };


static POLE pmFillTyp[] = {
	   {u8"Filled\0",'F',288,&mTranslucency},
	   {u8"Very thin\0",'V',289,NULL},
	   {u8"Thin\0",'T',290,NULL},
	   {u8"tHick\0",'H',291,NULL},
	   {u8"very thicK\0",'K',292,NULL},
	   {u8"Extra thick\0",'E',293,NULL},
	   {u8"Pattern\0",'P',836, &mTracePattern},
};

static TMENU mFillTyp = { 7,0,0,15,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,10,0,0,0,(POLE(*)[]) &pmFillTyp,NULL,NULL };

#define _YES_ "Y"
#define _NO_ "N"

static TMENU mTraceArcReversed = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0, 14 /*11*/,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmPTLukm[] = {
          {u8"[0] continuation with arc",L'0',714,NULL},
		  {u8"[1] three points",L'1',221,NULL},
		  {u8"[2] beginning-center-end",L'2',222,NULL},
		  {u8"[3] beginning-center-angle",L'3',223,NULL},
		  {u8"[4] beginning-center-chord",L'4',224,NULL},
		  {u8"[5] beginning-end-radius",L'5',225,NULL},
		  {u8"[6] beginning-end-angle",L'6',226,NULL},
		  {u8"[7] beginning-end-direction",L'7',227,NULL},
		  //{u8"[8] continuation",L'8',228,NULL},
          {u8"reversed arc direction",L'0',729,&mTraceArcReversed},
		  //{u8"Close",L'C',247,NULL},
		  //{u8"Remove: Del",L'R',248,NULL},
		  {u8"Line", L'L',13,NULL},
          {u8"Continue with line", L'C',715,NULL},
};

static TMENU mPTLukm={11, 0, 0, 30, 56, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,75,0,0,0,&pmPTLukm,NULL,NULL};

static POLE pmFTrace[] = {
	{u8"Width\0 1\0        ",L'W',249,NULL},
	{u8"axis Offset\0 1\0        ",L'O',250,NULL},
	{u8"Polyline\0 T\0        ",L'P',251,&mD_T_Pline},
	{u8"Fill/linie width\0 F\0     ",L'F',252,&mFillTyp},
	{u8"Remove\0Del\0 ",L'R',253,NULL},
	{u8"Break\0Home\0 ",L'B',633,NULL},
	{u8"Close\0End\0 ",L'C',632,NULL},
    {u8"Arc\0\0 ",L'A',15,&mPTLukm},
};

static POLE pmMLine[] = {
	{u8"Width\0 1\0        ",L'W',249,NULL},
	{u8"axis Offset\0 1\0        ",L'O',250,NULL},
	{u8"Polyline\0 T\0        ",L'P',251,&mD_T_Pline},
	{u8"Remove\0Del\0 ",L'R',253,NULL},
	{u8"Break\0Home\0 ",L'B',633,NULL},
	{u8"Close\0End\0 ",L'C',632,NULL},
    {u8"Arc\0\0 ",L'A',15,&mPTLukm},
};

#define _TRACE_ u8"Trace"
#define _TRACE_C_ L'T'

#define _DLINE_ u8"2-line"
#define _DLINE_C_ L'2'

#define _TLINE_ u8"3-line"
#define _TLINE_C_ L'3'

#define _YES__ "Y"
#define _NO__ "N"

#define _BREAK_ u8"Break"

#endif

#ifdef __O_SKETCH__

static POLE pmSketch[] = {
	{u8"Segment length\0   1mm",L'L',531, NULL},
	{u8"Remove last\0Del\0 ",L'R',532, NULL},
};

#define _SKETCH_ u8"sKetch"
#define _SKETCH_C_ L'K'

#define _MM_ u8"mm"

#endif

#ifdef __O_POINT__

#define _POINT_TYPE_ u8"point Type"
#define _POINT_TYPE_C_ L'T'

static POLE pmTyp_punktu[] = {
	 {u8"Simple\0",L'S',294,NULL},
	 {u8"Base point\0",L'B',295,NULL},
	 {u8"pin point\0",L'C',296,NULL},
	 {u8"Junction\0",L'J',529,NULL},
     {u8"Fixed\0",L'F',738,NULL},
     {u8"fixed L\0",L'1',739,NULL},
     {u8"fixed R\0",L'2',740,NULL},
     {u8"fixed U\0",L'3',754,NULL},
     {u8"Pinned\0",L'P',741,NULL},
     {u8"pinned L\0",L'4',742,NULL},
     {u8"pinned R\0",L'5',743,NULL},
     {u8"pinned U\0",L'6',755,NULL},
     {u8"fixed Roller\0",L'R',744,NULL},
     {u8"fixed roller L\0",L'7',745,NULL},
     {u8"fixed roller R\0",L'8',746,NULL},
     {u8"fixed roller U\0",L'9',756,NULL},
     {u8"pinned rOller\0",L'O',747,NULL},
     {u8"pinned roller L\0",L'0',748,NULL},
     {u8"pinned roller R\0",L'-',749,NULL},
     {u8"pinned roller U\0",L'=',757,NULL},
     {u8"no rotation Z\0",L'+',818,NULL},

     {u8"fixed roller X\0",L'X',870,NULL},
     {u8"fixed roller XU\0",L'U',871,NULL},
     {u8"fixed roller Y\0",L'Y',872,NULL},
     {u8"fixed roller Y (Up)\0",L'T',873,NULL},
     {u8"pinned roller X\0",L'H',874,NULL},
     {u8"pinned roller X (Up)\0",L'A',875,NULL},
     {u8"pinned roller Y\0",L'V',876,NULL},
     {u8"pinned roller Y (Up)\0",L'W',877,NULL},
};

#endif

#ifdef __O_GRAPHF__

#define MAX_GRAPHFNO 16
#define MAX_GRAPHF_NAME_LEN 32
#define MAX_GRAPHF_FORMULA_LEN  (MAX_GRAPHFNO * MaxTextLen)    //4096

#define GRAPHF "Graph"
#define _GRAPHF_ "Graph"
#define _GRAPHF_C_ L'G'

static POLE pmGraphf[MAX_GRAPHFNO + 1];

static TMENU mGraphf = { 16,0,0,MAX_GRAPHF_NAME_LEN + 1,70,7,0,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmGraphf,NULL,NULL };

static POLE pmGraph[] = {
		  {u8"Step dx\0       ",L'S',254,NULL},
		  {u8"Function\0",L'F',255, &mGraphf} };

#define _CURRENT_ u8"Current"

#endif

#ifdef __B_MENU__

#define _ENDPOINT_ L'E'
#define _endpoint_ L'e'
#define _CTRL_ENDPOINT_ 1925 //5
#define _NEAREST_ L'N'
#define _nearest_ L'n'
#define _CTRL_NEAREST_ 1934 //14
#define _MIDDLE_ L'M'
#define _middle_ L'm'
#define _CTRL_MIDDLE_ 1933 //13
#define _INTERSECTION_ L'I'
#define _intersection_ L'i'
#define _CTRL_INTERSECTION_ 1929 //9
#define _CENTER_ L'C'
#define _center_ L'c'
#define _CTRL_CENTER_ 1923 //3
#define _PERPENDICULAR_ L'P'
#define _perpendicular_ L'p'
#define _CTRL_PERPENDICULAR_ 1936 //16
#define _TANGENTIAL_ L'T'
#define _tangential_ L't'
#define _CTRL_TANGENTIAL_ 1940 //20
#define _ADJACENT_ L'A'
#define _adjacent_ L'a'
#define _CTRL_ADJACENT_ 1921 //1
#define _POINT_ L'O'
#define _point_ L'o'
#define _CTRL_POINT_ 1935 //15
#define _EDIT_ L'E'
#define _edit_ L'e'
#define _CTRL_EDIT_ 1925 //5

#define _CTRL_NOPE_ 1944 //24

#define _CTRL_0_ 0    //SPC
#define _CTRL_1_ _CTRL_ADJACENT_  //A
#define _CTRL_2_ 2  //B
#define _CTRL_3_ _CTRL_CENTER_  //C
#define _CTRL_4_ 4  //D
#define _CTRL_5_ _CTRL_ENDPOINT_  //E
#define _CTRL_6_ 6  //F
#define _CTRL_7_ 7  //G
#define _CTRL_8_ 8  //H
#define _CTRL_9_ _CTRL_INTERSECTION_  //I
#define _CTRL_10_ 10  //J
#define _CTRL_11_ 11  //K
#define _CTRL_12_ 23 // Warstwy=Layesr12  //L->W
#define _CTRL_13_ _CTRL_MIDDLE_  //M
#define _CTRL_14_ _CTRL_NEAREST_  //N
#define _CTRL_15_ _CTRL_POINT_  //O
#define _CTRL_16_ _CTRL_PERPENDICULAR_  //P
#define _CTRL_17_ 17  //Q
#define _CTRL_18_ 18  //R
#define _CTRL_19_ 19  //S scale down
#define _CTRL_20_ _CTRL_TANGENTIAL_  //T
#define _CTRL_21_ 21  //U
#define _CTRL_22_ 22  //V
#define _CTRL_23_ 23  //W
#define _CTRL_24_ _CTRL_NOPE_    //X  or 24 scale down
#define _CTRL_25_ 25  //Y
#define _CTRL_26_ 26             //Z  scale up
#define _CTRL_27_ 27  //0
#define _CTRL_28_ 28  //1
#define _CTRL_29_ 29  //2
#define _CTRL_30_ 30  //3
#define _CTRL_31_ 31  //4
//#define _CTRL_32_ 32  //5
//#define _CTRL_33_ 33  //6
//#define _CTRL_34_ 34  //7
//#define _CTRL_35_ 35  //8
//#define _CTRL_36_ 36  //9



                         //@,A,B,C,D,E,F,G,H,I,J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
//int CONTROL_KEYS[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 }


#define _PRINTING__ u8"Print"

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


#define HATCH_PATTERN u8"PATTERNS"
#define PATTERNS_INI u8"PATTERNS.INI"

#define PATTERN_NAMES_MAXNO 255

static PPOLE pm__list_hatch[PATTERN_NAMES_MAXNO];

static POLE pm__list_hatch_predef[] = {
		{u8"֎SOLID\0",'S',0,&mHTranslucency},
		{u8"////////\0",'/',0,NULL},
		{u8"XXXXXXXX\0",'X',0,NULL} };

POLE pmKatH[] = {
		{u8"Angle",'A',108,NULL },
		{u8"paRallel to",'R',580,NULL},
		{u8"Perpendicular to",'P',581,NULL},
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

PTMENU m__list_hatch = { 0, 0, 0, PATERN_NAME_MAXLEN - 1,66, 6,TADD | FIXED, CMNU,CMBR,CMTX,0,   ID_END,   0, 0,0,(PPOLE(*)[]) & pm__list_hatch_predef, NULL,NULL };

static POLE pmSolidHatchPattern[] = {
		{u8"Pattern\0 ",'P',597,NULL},
		{u8"Scale\0 1.0",'S',436,&mSolidHatchPatternScale},
		{u8"Rotate\0 0",'R',35,&mSolidHatchPatternAngle},
		{u8"dX\0 0",'X',622,&mSolidHatchPatternDx},
		{u8"dY\0 0",'Y',623,&mSolidHatchPatternDy},
		{u8"Grab pattern\0",'G',164,NULL},
};

static TMENU mSolidHatchPattern = { 6,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,ID_IMAGE_PATTERN,0,0,0,(POLE(*)[]) & pmSolidHatchPattern,NULL,NULL };

POLE pmSelect_Ins[] = {
	{u8"Simple",'S',680,NULL},
	{u8"Complex",'C',681,NULL},
};

TMENU mSelect_Ins = { 2, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, ID_INSULATION, 0, 0,0,(POLE(*)[]) &pmSelect_Ins, NULL, NULL };

POLE pmHatch[] = {
	 {u8"Pattern\0//////////\0      ",'P',434,&m__list_hatch},
	 {u8"pattern Folder\0 \0      ",'F',597,NULL},
	 {u8"Rotation angle\0 0\0      ", 'R', 435,&mKatH},
	 {u8"Scale\0 1\0     ", 'S', 436,NULL},
	 {u8"base point Coordinates\0 0,0\0              ", 'C', 437,NULL},
	 {u8"indicate Base point\0 \0", 'B', 438,NULL},
	 {u8"pattern line Distance / spline segment length\0 1\0", 'D', 439,NULL},
	 {u8"Thermal insulation\0 \0", 'T', 679,&mSelect_Ins},
	 {u8"Image pattern\0 \0",'I',836,&mSolidHatchPattern}
	  };

#define _HATCHING_ u8"Hatching"
#define _HATCHING_C_ L'H'

#define _SOLID_SUFFIX_ ""
#define __IMAGE u8"__IMAGE"

#endif

#ifdef __O_DXF_IN__

#define Linie_DXF u8"LINES-DXF"
static char line_width_dxf_param[5][16] =
{
   "very thin",
   "thin",
   "thick",
   "very thick",
   "extra thick",
};

#define _LINE_NO_ u8"line: "
#define _SYSTEM_MESSAGE_ u8"SYSTEM MESSAGE"
#define _INTERRAPTED_ u8"Interrupted"

#endif

#ifdef __O_BLOK__

static char	 sz__last_bl[] = u8"last Block\0 ";
static char	sz__wszystko[] = u8"All\0 ";
static char	sz__wszystkie_warstwy[] = u8"all Layers\0 ";
static char	sz__wszystkie_teksty[] = u8"all Texts\0 ";
static char	sz__wszystkie_symbole[] = u8"all Symbols\0 ";
static char	 sz__cross_win[] = u8"Cross/window\0  F9";
static char	sz__add_del[] = u8"add/Remove\0  F10";
static char	sz_undo_del[] = u8"return all\0 ";

static TMENU mAuto = { 2,0,0,5,72,9,ICONS,CMNU,CMBR,CMTX,0, 7, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAutoc = { 2,0,0,5,72,8,ICONS,CMNU,CMBR,CMTX,0,	5,	0,	0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dq = { 2,0,0,5,72,7,ICONS,CMNU,CMBR,CMTX,0, 4, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dw = { 2,0,0,5,72,7,ICONS,CMNU,CMBR,CMTX,0, 2, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dn = { 2,0,0,5,72,9,ICONS,CMNU,CMBR,CMTX,0, 9 /*7*/, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmBlokm[] = {
	 {u8"last Block\0 ",'B',256,NULL},
	 {u8"All\0 ",'A',257,NULL},
	 {u8"all Layers\0 ",'L',258,NULL},
	 {u8"Cross/window\0  F9", 'C',259,NULL},
	 {u8"add/Remove\0  F10" ,'R', 260,NULL},
	 {u8"Instant\0 Y", 'I',261,&mAuto},
};

POLE pmScalem[] = {
		{u8"Approximation\0 20\0  ",'A',238, NULL},
};

POLE pmBlokmc[] = {
	 {u8"Cross/window\0  F9", 'C',259,NULL},
	 {u8"add/Remove\0  F10" ,'R',260, NULL},
	 {u8"Instant\0 Y", 'I',261,&mAutoc},
};

POLE pmSelectMode[] = {
	 {u8"Cross", L'C',259,NULL},
	 {u8"Object" , L'O',720, NULL},
};

POLE pmBlock_wez_w[] =
{
  {u8"Cross/window\0  F9",	'C',259,NULL},
  {u8"Instant\0 Y",	'I',261,&mAuto_dw},
};

POLE pmBlock_wez_info[] =
{
  {u8"Entire polyline",	'E',687,NULL},
  {u8"Single object",	'S',688,NULL},
};

POLE pmBlock_dq[] =
{
  {u8"Cross/window\0  F9",	'C',259,NULL},
  {u8"Instant\0Y",	'I',261,&mAuto_dq},
  {u8"Return all\0 ", 'R',270,NULL},
  {u8"change Mode\0 ", 'M',271,NULL},
};

POLE pmBlock_dn[] =
{
  {u8"last Block\0 ",'B',256,NULL},
  {u8"All\0 ",'A',257,NULL},
  {u8"all Layers\0 ",'L',258,NULL},
  {u8"Cross/window\0  F9", 'C',259,NULL},
  {u8"add/Remove\0  F10" ,'R', 260,NULL},
  {u8"Instant\0Y", 'I',261,&mAuto_dn},
  {u8"Delete\0 Del", 'D',32,NULL},
  {u8"Return all\0 ", 'R',270,NULL},
  {u8"change Mode\0", 'M',271,NULL},
};

POLE pmBlok_ex[] = {
	 {u8"last Block\0 ",'B',256,NULL},
	 {u8"All\0 ",'A',257,NULL},
	 {u8"all Layers\0 ",'L',258,NULL},
	 {u8"Cross/window\0  F9", 'C',259,NULL},
	 {u8"add/Remove\0  F10" ,'R', 260,NULL},
	 {u8"Instant\0 Y", 'I',261,&mAuto},
	 {u8"Explode\0",'E',66,NULL},
};

POLE pmBlok_chp[] = {
	 {u8"last Block\0 ",'B',256,NULL},
	 {u8"All\0 ",'A',257,NULL},
	 {u8"all Layers\0 ",'L',258,NULL},
	 {u8"all from layer No\0 ",'N',174,NULL},
	 {u8"all Texts\0 ",'T',273,NULL},
	 {u8"Cross/window\0  F9", 'C',259,NULL},
	 {u8"add/Remove\0  F10" ,'R', 260,NULL},
	 {u8"Instant\0 Y", 'I',261,&mAuto},
	 {u8"change Properties\0",'P',272,NULL},
};

POLE pmBlok_cht[] = {
	 {u8"last Block\0 ",'B',256,NULL},
	 {u8"all Texts\0 ",'T',273,NULL},
	 {u8"Cross/window\0  F9", 'C',259,NULL},
	 {u8"add/Remove\0  F10" ,'R',260, NULL},
	 {u8"Instant\0 Y", 'I',261,&mAuto},
};

POLE pmBlok_chts[] = {
	 {u8"last Block\0 ",'B',256,NULL},
	 {u8"all Symbols\0 ",'S',257,NULL},
	 {u8"all Texts\0 ",'T',273,NULL},
	 {u8"Cross/window\0  F9", 'C',259,NULL},
	 {u8"add/Remove\0  F10" ,'R', 260,NULL},
	 {u8"Instant\0 Y", 'I',261,&mAuto},
};

POLE pmCopy_Divide[] = {
	 {u8"Divide",'D',50,NULL},
};

static POLE	pmWszystko_Okno[] = {
	 {u8"All",'A',257,NULL},
	 {u8"Window",'W',274,NULL},
};

char window[16] = "window";
char  cross[16] = "cross";

char _addto_[16] = u8"add";
char  _remove_[16] = u8"remove";

char Yes[2] = "Y";

#define _DIVIDE_ u8"Divide"
#define _DIVIDE_C_ L'D'

#define _BLOCK_ u8"Block"
#define _BLOCKF_ u8"* Block"
#define _BLOCK_C_ L'B'

#define _BLOCK_SELECT_ u8"Selection mode"
#define _BLOCK_SELECT_C_ L'S'

#define _FROZEN_ u8"frozen "
#define _EXPLODE_BLOCKS_ u8"Do you want to explode marked %sblocks?"
#define _Yes_ "Yes"
#define _No_ "No"

#define _SCALE_ u8"Scale"
#define _SCALE_C_ L'S'

#define _FILE_ u8"File "
#define _FILE_EXISTS_ u8" exists. Overwrite ? (Yes/No/Esc):"

#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#define _SAVED_BLOCK_ u8"< saved block > "

#define _CHANGE_ u8"Change"
#define _CHANGE_C_ L'C'

#define _POINT_IN_AREA_ u8"OK. Point in enclosed area."
#define _POINT_NOT_IN_AREA_ u8"Bad news. Point outside enclosed area or area unclosed."

#define _OBJECT_ u8"Object"
#define _OBJECT_C_ L'O'

static char _entire_polyline_[32]=u8"Entire polyline";
static char _single_object_[32]=u8"Single object";

#endif

#ifdef __O_PROF__

POLE pmChamfer[] =
{
	 {u8"Distance(s)\0       \0",L'D', 316, NULL},
	 {u8"Undo\0     \0",L'U', 459, NULL},
};

POLE pmFillet[] =
{
	 {u8"Radius\0      \0",L'R', 209, NULL},
	 {u8"Undo\0    \0",L'U', 459, NULL},
};

#define _FILLET_ u8"Fillet"
#define _FILLET_C_ L'F'

#define _CHAMFER_ u8"Chamfer"
#define _CHAMFER_C_ L'C'

#endif

#ifdef __O_OFFSET__

#define OFFSET_NORMAL u8"Normal"
#define OFFSET_SMOOTH u8"Smooth"

static POLE pmMethod[] = {
	 {u8"Normal",L'N',630,NULL},
	 {u8"Smooth",L'S',631,NULL},
};

static TMENU mMethod = { 2, 0,0,19,56,4,ICONS,CMNU,CMBR,CMTX,0,4, 0,0,0,(POLE(*)[]) &pmMethod, NULL,NULL };

static POLE pmOffset[] = {
	 {u8"Point\0     ",L'P',457,NULL},
	 {u8"Distance\0     ", L'D', 458,NULL},
	 {u8"Set distance\0     ", L'S', 605,NULL},
	 {u8"Method for polylines\0     ", L'M', 629,&mMethod},
};

#define _OFFSET_ u8"Offset"
#define _OFFSET_C_ L'O'

#endif

#ifdef __O_MIRROR__

POLE pmMirror[] = {
		{u8"Leave original\0 Del",L'L',603,NULL},
		{u8"Delete original\0 Del",L'D',604,NULL},
};

char st_del[] = u8"Replace", st_nodel[] = u8"Leave";

#define _MIRROR_ u8"mIrror"
#define _MIRROR_C_ L'I'

#endif

#ifdef __O_IMPORT__

#define PATTERNS u8"Patterns"
#define __PATTERNS__ u8"Patterns"
#define BACKGROUND u8"Background"
#define __BACKGROUND__ u8"Background"
char *BLOCKLIST=u8"List of blocks";

static POLE	pmObrot[] = {
	 {u8"rotate By angle",'B',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",	    '0',578,NULL},
	 {u8"rotate To angle",'T',440,NULL},
	 {u8"paRallel to",'R',580,NULL},
	 {u8"Perpendicular to",'P',581,NULL},
	 {u8"at Angle to",'A',582,NULL},
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
	 {u8"Scale\0  ",'S',36,NULL},
     {u8"Fixed scale\0  ",'F',695,NULL},
	 {u8"Rotate\0  ",'R',35,&mObrot},
	 {u8"mirror Y\0  ",'Y',41,NULL},
	 {u8"mirror X\0  ",'X',40,NULL},
     {u8"Center to cursor\0  ",'C',717,NULL},
	 {u8"Divide\0  ",'D',50,NULL},
};

static POLE	pmBlok_Divide[] = {
	 {u8"Divide",'D',50,NULL},
};

static POLE	pmBlok_Imp_GEO[] = {
	 {u8"Scale",'S',36,NULL},
	 {u8"Rotate",'R',35,&mObrot_GEO},
	 {u8"mirror Y",'Y',41,NULL},
	 {u8"mirror X",'X',40,NULL},
	 {u8"Divide",'D',50,NULL},
};

#define patterns_tip u8"@Select hatching pattern"
#define background_tip u8"@Select desktop background"
#define blocklist_tip u8"@Select block from list"
#define alpha_sorting_tip u8"Alphabetical order"
#define timelapse_tip u8"Sorting by time"
#define catalog_tip u8"@Select block"

char catalog[16] = u8"Catalogue";
char background[16] = u8"Background";
char blocklist[16] = u8"Blocks";
char patterns[16] = u8"Patterns";

#define _BLOCK_ u8"Block"
#define _BLOCK_C_ L'B'

#define _BLOCK_3D_ u8"Block 3D"

#define _BLOCK__ u8"Block : ";
#define _BLOCK_3D__ u8"Block : ";

#define _BACKGROUND_ u8"Background : "
#define _SOLID_IMAGE_ u8"Solid image : "
#define _HATCH_IMAGE_ u8"Hatching image : "
#define _TRACE_IMAGE_ u8"Trace image : "
#define _IMAGE_ u8"Image : "

#define _PATTERN_ u8"Pattern : "

#define _PCX_WRITE_ERROR_ u8"PCX file write error"
#define _PNG_WRITE_ERROR_ u8"PNG file write error"
#define _JPG_WRITE_ERROR_ u8"JPG file write error"

#define _NO_PATTERN_ u8"NOPATTERN.axx"

#define _NO_BLOCK_IN_CATALOG_ u8"No symbol in the catalogue ("
#define AUTOSCALE "   AUTOSCALE"

#define FOLDER u8"\023<ALL>"

#endif

#ifdef __O_BREAK__

static POLE pmBreakOptions[] = {
		 {u8"break and Leave",L'L',692,NULL},
		 {u8"break and Cut",L'C',693,NULL},
         };

static TMENU mBreakOptions = { 2,0,0,4,24,8,TADD | ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pmBreakOptions,NULL,NULL };

POLE pmBreak[] =
{
	 {u8"Undo\0 ",L'U',459,NULL},
	 {u8"Divide\0 ",L'D',694,NULL},
     {u8"break Options\0                                       ",L'O',691,&mBreakOptions},
};


#define _BREAK_ u8"Break"
#define _BREAK_C_ L'B'

static char _break_and_leave_[] = u8"break and Leave";
static char _break_and_cut_[] = u8"break and Cut";

#endif

#ifdef __O_TRIM__

static POLE pmTrim[] =
{
 {u8"Undo",'U',459, NULL},
};

#define _TRIM_ "Trim"
#define _TRIM_C_ L'T'

static POLE pmExtend[] =
{
 {u8"Undo",'U',459, NULL},
};

#define _EXTEND_ "Extend"
#define _EXTEND_C_ L'E'

#endif

#ifdef __O_EXTEND__

#endif

#ifdef __B_HLP__

#define _1920_ 1920

POLE pmHelp[] = {
	 {u8"TOOLS",31,351,NULL},
	 {u8"Auxiliary menu of tools",30,391,NULL},
	 {u8"Auxiliary menu of tools",26,359,NULL},
	 {u8"Repeat last Function",548 + _1920_,547,NULL},
	 {u8"Edit numeric values",' ',363,NULL},
	 {u8"Increase pointfinder size",' ',361,NULL},
	 {u8"Decrease pointfinder size",' ',360,NULL},
#ifdef MACOS
     {u8"Free mouse",' ',387,NULL},
#else
	 {u8"Free mouse",' ',386,NULL},
#endif
	 //{u8"Free mouse",24,353,NULL},
	 {u8"Free mouse",840 + _1920_,353,NULL},
     {u8"Free mouse (Linux)",363 + _1920_,353,NULL},
     {u8"Free mouse (hold the right button for 0,5 s)",819 + _1920_, 658,NULL},
	 {u8"Resize and position the window",25,353,NULL},
	 {u8"Stop macro recording",28,353,NULL},
	 {u8"Orthogonality",565 + _1920_,353,NULL},
     {u8"Copy the object under the cursor",566 + _1920_,353,NULL},
     {u8"Choose a layer",571 + _1920_,353,NULL},
	 {u8"EDIT",31,321,NULL},
	 {u8"Edit text",' ',377,NULL},
	 {u8"Move text",15,378,NULL},
	 {u8"Delete object",' ',380,NULL},
	 {u8"Restore deleted object",' ',381,NULL},
	 {u8"Orthogonality",' ',383,NULL},
	 {u8"Window / Cross",' ',384,NULL},
	 {u8"Add / Remove",' ',385,NULL},
	 {u8"VIEW",31,53,NULL},
	 {u8"Zoom in / out",' ',389,NULL},

	 {u8"Pan",355+1920,353,NULL},
	 {u8"Pan",354+1920,353,NULL},

	 {u8"Window",12,353,NULL},
	 {u8"Last view",13,353,NULL},
	 {u8"View all",14,353,NULL},
	 {u8"View center",15,353,NULL},
	 {u8"View scale 1",16,353,NULL},
	 {u8"View scale 0.5",17,353,NULL},
	 {u8"View scale 0.2",18,353,NULL},
	 {u8"View scale 0.1",19,353,NULL},
	 {u8"Redraw",20,353,NULL},
	 {u8"View full drawing",21,353,NULL},

     {u8"Zoom in",574 + _1920_,353,NULL},
     {u8"Zoom out",567 + _1920_,353,NULL},

	 {u8"FEATURES",31,352,NULL},
	 {u8"Color",12,354,NULL},
	 {u8"Line type",13,354,NULL},
	 {u8"Line width",14,354,NULL},
	 {u8"Layer",15,354,NULL},
	 {u8"Get color",19,354,NULL},
	 {u8"Get line type and width",20,354,NULL},
	 {u8"Get layer",21,354,NULL},
	 {u8"Hide layer",22,354,NULL},
	 {u8"MEASURE",31,390,NULL},
	 {u8"Point coordinates",16,354,NULL},
	 {u8"Distance",17,354,NULL},
	 {u8"Angle",18,354,NULL},
	 {u8"LAYERS",31,55,NULL},
	 {u8"Layer 1",1,354,NULL},
	 {u8"Layer 2 ...",2,354,NULL},
	 {u8"Layer 10",0,354,NULL},
	 {u8"Next layer",10,354,NULL},
	 {u8"Previous layer",11,354,NULL},
	 {u8"All layers ON",22,353,NULL},
	 {u8"Reverse layers",23,353,NULL},
	 {u8"POINTER STEP",31,154,NULL},
	 {u8"Minimal / Large step",12,355,NULL},
	 {u8"POINTFINDER",31,52,NULL},

	 {u8"Endpoint",13,355,NULL},
	 {u8"Nearest end",14,355,NULL},
	 {u8"Middpoint",15,355,NULL},
	 {u8"Intersection",16,355,NULL},
	 {u8"Center point",17,355,NULL},
	 {u8"Perpendicular",18,355,NULL},
	 {u8"Tangential",19,355,NULL},
	 {u8"Adjacent",20,355,NULL},
	 {u8"Point",21,355,NULL},

	 {u8"Endpoint",' ',553,NULL},
	 {u8"Nearest end",' ' ,562,NULL},
	 {u8"Midpoint",' ',561,NULL},
	 {u8"Intersection",' ',557,NULL},
	 {u8"Center point",' ',551,NULL},
	 {u8"Perpendicular",' ',564,NULL},
	 {u8"Tangential",' ' ,568,NULL},
	 {u8"Adjacent",' ',549,NULL},
	 {u8"Point",' ',563,NULL},

	 {u8"Endpoint mode",553+1920,353,NULL},
	 {u8"Nearest end mode",562+1920,353,NULL},
	 {u8"Middpoint mode",561+1920,353,NULL},
	 {u8"Intersection mode",557+1920,353,NULL},
	 {u8"Center point mode",551+1920,353,NULL},
	 {u8"Perpendicular mode",564+1920,353,NULL},
	 {u8"Tangential mode",568+1920,353,NULL},
	 {u8"Adjacent mode",549+1920,353,NULL},
	 {u8"Point mode",563+1920,353,NULL},
	 {u8"None mode",572 + 1920,353,NULL},

     {u8"Copy object under cursor",566 + _1920_,353,NULL},
     {u8"Layers setting",560 + _1920_,353,NULL},
     {u8"Layers setting (optional)",571 + _1920_,353,NULL},

     {u8"Short instruction",702 + 1920,534,NULL},

};

#define _ENDPOINT_ L'E'
#define _ENDPOINT_SC_ 5
//#define _CTRL_ENDPOINT_ L'E'
//#define _CTRL_ENDPOINT_SC_ 5
#define _NEAREST_ L'N'
#define _NEAREST_SC_ 14
#define _MIDDLE_ L'M' 
#define _MIDDLE_SC_ 13
#define _INTERSECTION_ L'I'
#define _INTERSECTION_SC_ 9
#define _CENTER_ L'C'
#define _CENTER_SC_ 3
#define _PERPENDICULAR_ L'P'
#define _PERPENDICULAR_SC_ 16
#define _TANGENTIAL_ L'T'
#define _TANGENTIAL_SC_ 20
#define _ADJACENT_ L'A'
#define _ADJACENT_SC_ 1
#define _POINT_ L'O'
#define _POINT_SC_ 15
#define _NOPE_ L'X'
#define _NOPE_SC_ 24

#endif

#ifdef __O_MAK__

#define _MACRO_ u8" MACRO "

static POLE pmMakro[] = {
	{u8"Recording. End-> Ctrl-End",L'R',92,NULL},
	{u8"Execute",L'E',93,NULL},
	{u8"Load",L'L',94,NULL},
	{u8"Save",L'S',95,NULL}  };

#define _DRAW__ L'd';
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
//#  define NULL 0

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
		 {u8"Fine\0",'F',594,&mKursorSize},
		 {u8"Duplex\0",'D',595,&mKursorSizeB} };

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
		 {u8"[1] millimeter \0[mm]",'1',0,NULL},
		 {u8"[2] centimeter \0[cm]",'2',0,NULL},
		 {u8"[3] meter \0[m]",'3',0,NULL},
         {u8"[4] kilometer \0[km]",'4',0,NULL},
		 {u8"[5] inch \0[\"]",'5',0,NULL},
		 {u8"[6] foot \0[']",'6',0,NULL},
         {u8"[7] yard \0[yd]",'7',0,NULL},
         {u8"[8] mile \0[mi]",'8',0,NULL}};

static TMENU mJednostki = { 8,0,0,18,24,8,TADD,CMNU,CMBR,CMTX,0,78,0,0,0,(POLE(*)[]) &pmJednostki,NULL,NULL };

static POLE pmKartezjanskiGeodezyjny[] = {
		 {u8"Cartesian",'C',104,NULL},
		 {u8"Geodetic",'G',105,NULL},
         {u8"Isometric",'I',868,NULL},
};
	 
static TMENU mKartezjanskiGeodezyjny = { 3,0,0,12,24,8,ICONS,CMNU,CMBR,CMTX,0,162 /*109*/,0,0,0,(POLE(*)[]) &pmKartezjanskiGeodezyjny,NULL,NULL };

static POLE pmPointOrigin[3] = {
		 {u8"Point origin\0 ",'P',106,NULL},
		 {u8"Axis of local coord.\0",'A',107,NULL},
		 {u8"Rotation angle\0",'R',108,NULL } };

static TMENU mPointOrigin = { 3,0,0,19,24,9,ICONS,CMNU,CMBR,CMTX,0,86,0,0,0,(POLE(*)[]) &pmPointOrigin,NULL,NULL };

static char* tablica_styli[] = { u8"Top",u8"Bottom",u8"Top&Bottom",u8"Around" };

static POLE pmSektory_Styl[4] = {
		 {u8"Top",'T',498,NULL},
		 {u8"Bottom",'B',499,NULL},
		 {u8"top & bottom",'H',500,NULL},
		 {u8"Around",'A',486,NULL} };

static TMENU mSektory_Styl = { 4,0,0,10,30,7,ICONS,CMNU,CMBR,CMTX,0,65,0,0,0,(POLE(*)[]) &pmSektory_Styl,NULL,NULL };

static POLE pmSektory_Arkusza[15] = {
	   {u8"frAme on\0 N\0           ",'A',485,&mRamka_on_off},
	   {u8"Zones on\0 N\0          ",'Z',486,&mSektory_on_off},
	   {u8"zone number Prefix\0 \0     ",'P',487,NULL},
	   {u8"Numbering from\0 \0   ",'N',488,NULL},
	   {u8"Reversed\0 N\0           ",'R',489,&mReversed_on_off},
	   {u8"zone Height [mm]\0 \0   ",'H',490,NULL},
	   {u8"zone Width [mm]\0 \0   ",'W',491,NULL},
	   {u8"Margin width [mm]\0 \0   ",'M',492,NULL},
	   {u8"frame offset [mm]\0 \0   ",'D',501,NULL},
	   {u8"frame Line thickness\0Thin\0          ",  'L',493,&mGruboscLiniiF},
	   {u8"zones line Thickness\0Thick\0          ",  'T',494,&mGruboscLiniiZ},
	   {u8"֎frame Color\0white\0              ",     'C',495,&mKolorACF},
	   {u8"֎zones cOlor\0white\0              ",     'O',496,&mKolorACZ},
	   {u8"Style\0Top\0              ",'S',497,&mSektory_Styl},
	   {u8"Font\0 \0             ", 'F', 183,&mCzcionkaZ}};


static POLE pmPrecision[] = {
       {u8"Force precision\0 \0",L'F',727,NULL},
	   {u8"Moment precision\0 \0",L'M',728,NULL},
	   {u8"Displacement precision\0 \0",L'D',730,NULL},
	   {u8"Prescribed rotation precision\0 \0",L'P',731,NULL},
       {u8"Load precision\0 \0",L'L',733,NULL},
       {u8"Thermal load precision\0 \0",L'T',752,NULL},
       {u8"Stress precision\0 \0",L'S',800,NULL},
       {u8"Reaction precision\0 \0",L'R',763,NULL},
       };

static TMENU mPrecision = { 8,0,0,10,30,7,TADD | ICONS ,CMNU,CMBR,CMTX,0,18,0,0,0,(POLE(*)[]) &pmPrecision,NULL,NULL };


static POLE pmThermal[] = {
       {u8"Depth of the section\0 \0",L'D',764,NULL},
	   {u8"Temperature change\0 \0",L'T',765,NULL},
	  };

static TMENU mThermal = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmThermal,NULL,NULL };


static POLE pmStaticColors[] = {
       {u8"֎Nodes&elements\0grey\0",        L'N',723,&mKolorSTATIC},
       {u8"֎Tensions\0red\0",               L'T',771,&mKolorSTATIC},
	   {u8"֎Compressions\0blue\0",          L'C',758,&mKolorSTATIC},
       {u8"֎Shear\0green\0",                L'S',759,&mKolorSTATIC},
       {u8"֎Moments\0magenta\0",            L'M',760,&mKolorSTATIC},
       {u8"֎Deformation\0yellow\0",         L'D',761,&mKolorSTATIC},
       {u8"֎Reactions\0cyan\0",             L'R',763,&mKolorSTATIC},
       {u8"֎Axial stress+\0dark red\0",     L'A',801,&mKolorSTATIC},
       {u8"֎aXial stress-\0dark blue\0",    L'X',802,&mKolorSTATIC},
       {u8"֎sHear stress\0dark green\0",    L'H',803,&mKolorSTATIC},
       {u8"֎Vibrations\0other\0",           L'V',814,&mKolorSTATIC},
	  };

static TMENU mStaticColors = { 11,0,0,10,30,7,TADD | ICONS ,CMNU,CMBR,CMTX,0, 26,0,0,0,(POLE(*)[]) &pmStaticColors,NULL,NULL };

static POLE pmLoadMagnitude[] = {
       {u8"Linear load\0 \0",L'L',733,NULL},
	   {u8"Surface load\0 \0",L'S',845,NULL},
	  };

static TMENU mLoadMagnitude = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0, 43,0,0,0,(POLE(*)[]) &pmLoadMagnitude,NULL,NULL };

static POLE pmReactionMagnitude[] = {
       {u8"Concentrated reaction forces rescaling\0 \0",L'C',762,NULL},
	   {u8"Distributed reaction forces rescaling\0 \0",L'D',864,NULL},
	  };

static TMENU mReactionMagnitude = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0, 49,0,0,0,(POLE(*)[]) &pmReactionMagnitude,NULL,NULL };

static POLE pmStressMagnitude[] = {
       {u8"stress in Steel/timber\0 \0",L'S',775,NULL},
	   {u8"stress in reinforced Concrete\0 \0",L'C',6,NULL},
	  };

static TMENU mStressMagnitude = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0, 45,0,0,0,(POLE(*)[]) &pmStressMagnitude,NULL,NULL };

static POLE pmResetMagnitude[] = {
       {u8"SI\0 \0",L'S',858,NULL},
	   {u8"Imperial\0 \0",L'I',859,NULL},
	  };

static TMENU mResetMagnitude = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,47,0,0,0,(POLE(*)[]) &pmResetMagnitude,NULL,NULL };

static POLE pmMagnitude[] = {
       {u8"Force rescaling\0 \0",                       L'F',727,NULL},
	   {u8"Moment rescaling\0 \0",                      L'M',728,NULL},
	   {u8"Displacement rescaling\0 \0",                L'D',730,NULL},
	   {u8"Rotation rescaling\0 \0",                    L'R',731,NULL},
       {u8"Load rescaling\0 \0",                        L'L',733,&mLoadMagnitude},
       {u8"Thermal load rescaling\0 \0",                L'T',752,NULL}, //&mThermal},
       {u8"axial forces rescaling\0 \0",                L'N',758,NULL},
       {u8"shear forces rescaling\0 \0",                L'V',759,NULL},
       {u8"Bending moments rescaling\0 \0",             L'B',760,NULL},
       {u8"deformation rescaling\0 \0",                 L'U',761,NULL},
       {u8"reaction forces rescaling\0 \0",             L'X',762,&mReactionMagnitude},
       {u8"reaction moments rescaling\0 \0",            L'Z',763,NULL},
       {u8"Stress rescaling\0 \0",                      L'S',800,&mStressMagnitude},
       {u8"reinforcement % rescaling\0 \0",             L'%',823,NULL},
       {u8"Exaggeratation of vibrations\0 \0",          L'E',814,NULL},
       {u8"Precision\0 \0",                             L'P',184,NULL},  //&mPrecision},
       {u8"Colors\0 \0",                                L'C',495,NULL}, //&mStaticColors},
       {u8"reset rescaling\0 \0",                       L'I',860,&mResetMagnitude},
};

static TMENU mMagnitude = { 18,0,0,10,30,7,TADD | ICONS | NOWCLOSE ,CMNU,CMBR,CMTX,0,111,0,0,0,(POLE(*)[]) &pmMagnitude,NULL,NULL };

static POLE pmParametry[] = {
	 {u8"Format\0 A4\0     ",'F',96,&mFormat_r},
	 {u8"Scale\0 1:5\0     ",'S',97,&mSkalaF},
	 {u8"Units\0       mm\0    ",'U',99,&mJednostki},
     {u8"Accuracy\0 0.25mm\0    ",'A',98,&mDokladnosc},
	 {u8"Coordinate system\0 C\0 ",'C',100,&mKartezjanskiGeodezyjny},
	 {u8"Local coordynate system\0 \0       ",'L',101,&mPointOrigin},
	 {u8"Zones\0 \0      ",'Z',486,NULL},
	 {u8"Pointer type & size\0 Max\0  ",'P',102,&mKursorTyp},
     {u8"Vectors rescaling, precision and colors\0\0  ",'V',751, NULL}, //&mMagnitude},
	 {u8"Edit ALFACAD.INI\0 \0     ",'E',103,NULL},
};

static char* ParTxt[] = { u8"C", u8"G", u8"Y", u8"N", u8"I"};

#define _CARTESIAN_ u8"C"
#define _GEODETIC_ u8"G"
#define _ISOMETRIC_ u8"I"
#define _MAX_ "Max"
#define _PLUSMAX_ "+Max"
#define _OTHER_COLOR_ u8"֍[X] other>>> "

#define _SECTORS_        "SECTORS"

static char config_sectors[11][32] =
{
	"Frame line thickness",
	"Zone line thickness",
	"Frame color",
	"Zone color",
	"Frame offset",
	"Zone height",
	"Zone width",
	"Margin width",
	"Zone font",
	"Zone style",
	"Reversed numbering",
};

#define _Yes_ u8"Yes"
#define _No_ u8"No"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#define _reset_mgnitude_to_ u8"Would you like to restore the default scaling factors for the measurement system:"
#define _SI_ u8"SI"
#define _IMP_ u8"Imperial"

#endif

#ifdef __O_LIBFUN__

char bloki_dir[MAXPATH] = "BLOCKS";
char katalogi_dir[MAXPATH] = "CATALOGS";
char bloki_cfg[MAXPATH] = "BLOCKS.CFG";
char biblioteka_cfg[MAXPATH] = "APARATY.AXX";

#endif

#ifdef __O_FOLDERSDIALOG__

#define _SELECT_FOLDER_  L"Select folder or type ..  or / or  // for more"
char error_message_block[64] = u8"No block files in this folder";
char error_message_background[64] = u8"No background files in this folder";

#endif

#ifdef __BIB_E__

char* view_width_tab[] = { u8"v.t.",u8"t.",u8"T.",u8"v.T.",u8"e.T.", "", "", u8"hidden " };

char* view_edge_tab[] = {u8"No support edge", u8"", u8"", u8"", u8"Rolled edge", u8"No support edge", u8"Pinned support edge", u8"Fixed support edge"};
char* view_edgeinverted_tab[] = {u8"Regular", u8"Inverted"};

char* view_type_tab[] = {
			   u8"continuous",
			   u8"dashed",
			   u8"dashdotted",
			   u8"2-dotted",
			   u8"dotted",
			   u8"border",
			   u8"center",
			   u8"hidden",
			   u8"phantom",
			   u8"dashed_2",
			   u8"dashdotted_2",
			   u8"2-dotted_2",
			   u8"dotted_2",
			   u8"border_2",
			   u8"center_2",
			   u8"hidden_2",
			   u8"phantom_2",
			   u8"dashed_x2",
			   u8"dashdotted_x2",
			   u8"2-dotted_x2",
			   u8"dotted_x2",
			   u8"border_x2",
			   u8"center_x2",
			   u8"hidden_x2",
			   u8"phantom_x2",
			   u8"3-dotted",
			   u8"2-center",
			   u8"3-dashed",
			   u8"phantom_3",
			   u8"2-center_x2",
			   u8"3-dashed_x2",
			   u8"multidashed",
			   u8"" };


T_Prototype      s__prot = { u8"prototype.alf", TRUE };

#define _SYSTEM_MESSAGE_ u8"SYSTEM MESSAGE"
#define _INTERRAPTED_ u8"Interrupted"
#define _TOO_MANY_BLOCKS_ u8"Too many blocks to check all"
#define _BLOCKS_CORRECTED_ u8"%ld blocks, %ld objects have been corrected"
#define _BLOCKS_TESTED_ u8"All OK. %d blocks tested"

char* typ_punktu_inf[] = { u8"Simple",u8"Base point","","","","","",u8"Junction",u8"Pin","","","",
                           u8"Fixed",u8"Fixed L",u8"Fixed R",u8"Fixed U",u8"Pinned", u8"Pinned L", u8"Pinned R", u8"Pinned U",
                           u8"Fixed Roller", u8"Fixed Roller L", u8"Fixed Roller R", u8"Fixed Roller U", u8"Pinned Roller", u8"Pinned Roller L", u8"Pinned Roller R", u8"Pinned Roller U", u8"no rotation Z",
                           u8"Fixed Roller X", u8"Fixed Roller X U", u8"Fixed Roller Y", u8"Fixed Roller Y U", u8"Pinned Roller X", u8"Pinned Roller X U", u8"Pinned Roller Y", u8"Pinned Roller Y U"};

#endif

#ifdef __O_LISTBL__

#define _NUMBER_OF_BLOCKS_ u8"Number of blocks ("
#define _EXCEEDS_NO_ u8") exceeds 5000"

#endif

#ifdef __O_ETEXT__

#define _YES_NO_ESC_ u8"YNyn\033"

POLE pmenu_typ[] = {
			   {u8"*ALL",'*',0,NULL},       //255
			   {u8"Normal",'N',0,NULL},
			   {u8"Internal label",'I',0,NULL},
			   {u8"component Label",'L',0,NULL},
			   {u8"Symbol",'S',0,NULL},
			   {u8"Type",'T',0,NULL},
			   {u8"?pin name",'?',0,NULL},
			   {u8"power supply",'V',0,NULL},
			   {u8"Port",'P',0,NULL},
			   {u8"Attribute",'A',0,NULL},
			   {u8"+Scheme",'+',0,NULL},
			   {u8"File name",'F',0,NULL},
			   {u8"netWork",'W',0,NULL},
			   {u8"Comment",'C',0,NULL},
			   {u8"%variable",'%',0,NULL},
			   {u8"aUx.symbol",'U',0,NULL},
			   {u8"auX.description",'X',0,NULL},
};

POLE pmenu_typ1[] = {
			   {u8"SYMBOL",'S',0,NULL},           //3 
			   {u8"*ALL",'*',0,NULL},       //255
			   {u8"Normal",'N',0,NULL},
			   {u8"Internal label",'I',0,NULL},
			   {u8"component Label",'L',0,NULL},
			   {u8"Symbol",'S',0,NULL},
			   {u8"Type",'T',0,NULL},
			   {u8"?pin name",'?',0,NULL},
			   {u8"power supply",'V',0,NULL},
			   {u8"Port",'P',0,NULL},
			   {u8"Attribute",'A',0,NULL},
			   {u8"+Scheme",'+',0,NULL},
			   {u8"File name",'F',0,NULL},
			   {u8"netWork",'W',0,NULL},
			   {u8"Comment",'C',0,NULL},
			   {u8"%variable",'%',0,NULL},
			   {u8"aUx.symbol",'U',0,NULL},
			   {u8"auX.description",'X',0,NULL},
};

POLE pmenu_auto[] = {
			   {u8"change All",'A',615,NULL},
			   {u8"Confirm each",'C',616,NULL},
};

POLE pmenu_change[] = {
			   {u8"Change",'C',112,NULL},
			   {u8"Skip",'S',113,NULL},
};

#endif

#ifdef __O_TEXT_B__

#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ L'Y'
#define _yes_ L'y'
#define _EDIT_ L'E'
#define _edit_ L'e'

#define _EDIT_SC_ 5

static TMENU mItalics = { 2,0,0,7,79,9,ICONS,CMNU,CMBR,CMTX,0,13,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUkryty = { 2,0,0,7,79,11,ICONS,CMNU,CMBR,CMTX,0,4,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mBold = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,37,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUnderlined = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,39,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAutoInterwal = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmTypTekstu[] = {
			   {u8"Normal",'N',0,NULL},
			   {u8"Internal label",'I',0,NULL},
			   {u8"component Label",'L',0,NULL},
			   {u8"Symbol",'S',0,NULL},
			   {u8"Type",'T',0,NULL},
			   {u8"*pin name",'*',0,NULL},
			   {u8"power supply",'V',0,NULL},
			   {u8"Port",'P',0,NULL},
			   {u8"Attribute",'A',0,NULL},
			   {u8"+Scheme",'+',0,NULL},
			   {u8"File name",'F',0,NULL},
			   {u8"netWork",'W',0,NULL},
			   {u8"Comment",'C',0,NULL},
			   {u8"%variable",'%',0,NULL},
			   {u8"aUx.symbol",'U',0,NULL},
			   {u8"auX.description",'X',0,NULL},
};

TMENU mTypTekstu = { 16,0,0,16,74,6,0,CMNU,CMBR,CMTX,0,15,0,0,0,(POLE(*)[]) &pmTypTekstu,NULL,NULL };

POLE pmJustowanie[] = {
					{u8"to Left",'L',262,NULL},
					{u8"to Right",'R',263,NULL},
					{u8"Middle",'M',264,NULL},
					{u8"Central",'C',265,NULL},
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
	{u8"Edit again  \0        ", 'E',38,NULL},
	{u8"Angle   \0            ", 'A',229,&mKatT},
	{u8"Hight\0 3mm\0         ", 'H',179,NULL},
	{u8"Width factor\0 1.0\0     ", 'W',230,NULL},
	{u8"Italics\0 N\0          ", 'I', 181,&mItalics},
	{u8"Type\0 \0             ", 'T', 231,&mTypTekstu},
	{u8"hiddeN\0 N\0          ", 'N', 232,&mUkryty},
	{u8"Bold\0 N\0            ", 'B', 182,&mBold},
	{u8"Underlined\0 N\0      ", 'U', 409,&mUnderlined},
	{u8"aLign\0 |<\0            ", 'L', 233,&mJustowanie},
	{u8"line Spacing\0 N\0    ", 'S', 410,&mLineSpacing},
	{u8"Auto-interval\0 N\0   ", '+', 91,&mAutoInterwal},
	{u8"Interval\0 1\0        ", '*', 234,NULL},
	{u8"Font\0 \0             ", 'F', 183,&mCzcionka},
};

static char t_t[16][12] = {
 u8"Normal",u8"Label",u8"Comp.label",u8"Symbol",u8"Type",u8"Pin name",u8"Power",u8"Port",u8"Attrib.",u8"Schem+",u8"Filename",u8"Network",u8"Comment",
 u8"%variable",u8"Aux.symbol",u8"Aux.descr." };

#define _TEXT_ u8"Text"
#define _TEXT_C_ L'T'
#define _HIDDEN_ u8" hidden"

#endif

#ifdef __B_WINDEM__

#define _NOT_LICENSED_ u8"By making a donation, you support the project"
#define _NOT_LICENSED1_ u8" Thank you."
#define _DEM_ROWS_NO_ 12
char lic_for[16] = { u8"User: " };

POLE pmDem1[] = {
	/*0*/	 {u8"",' ',0,NULL},
#ifdef BIT64
	/*1*/	 {u8"Creating new AlfaCAD (x64) window",' ',0,NULL},
#else
	/*1*/	 {u8"Creating new AlfaCAD window",' ',0,NULL},
#endif
	/*7*/{u8"_______________________________________________",' ',0,NULL},
	/*10*/   {u8"",' ',0,NULL},
};

POLE pmPrint[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8"   P R I N T I N G   ",' ',0,NULL},
	/*2*/	 {u8"    Interrupt -> Esc    ",' ',0,NULL},
	/*3*/  {u8"",' ',0,NULL},
};

POLE pmPlot[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8"   P L O T T I N G   ",' ',0,NULL},
	/*2*/	 {u8"",' ',0,NULL},
};

POLE pmPlot_to_com[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8" SENDING DATA TO THE PLOTTER ",' ',0,NULL},
	/*2*/	 {u8"",' ',0,NULL},
};

T_Prot_Date s_prot_date = { PROT_KEY, 0, 1, 0, 0, "User : ", "Series : ",
0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "",
0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "" };

#endif

#ifdef __O_OBJECT1__

#define _YES_NO_ESC_ u8"YNyn\033"
#define _YES_ 'Y'
#define _yes_ 'y'
#define _NO_ 'N'
#define _no_ 'n'

#endif

#ifdef __O_ARRAY__

static POLE pmArray_polar[] = {
	{u8"Number of elements\0 1\0     ",'N',607,NULL},
	{u8"filling Angle [°]\0 360\0     ",'A',608,NULL} };


static POLE pmArray_polar_rot[] = {
	{u8"Number of elements\0 1\0     ",'N',609,NULL},
	{u8"filling Angle [°]\0 360\0     ",'A',610,NULL} };

#define _ARRAY_WITH_ROTATION_ u8"Array with rotation"
#define _ARRAY_ u8"Array"
#define _ARRAY_C_ L'A'

#endif

#ifdef  __BIBWYM4__

#define _DIMENSIONING_ u8"Dimensioning"
#define _DIMENSIONING_C_ L'D'

#endif

#ifdef __BIBWYM_B_

static POLE pmKierunek[] = {
		{u8"0°",'0',203,NULL},
		{u8"90°",'9',204,NULL},
		{u8"Parallel to",'P',205,NULL},
		{u8"at an angle To",'T',206,NULL},
		{u8"at an angle X",'X',207,NULL},
		{u8"Angle dim.",'A',208,NULL},
        {u8"Circle/arc dim.",'C',202,NULL},
        {u8"Leader dim.",'L',678,NULL} };

static TMENU mKierunek = { 8,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,9,0,0,0,(POLE(*)[]) &pmKierunek,NULL,NULL };

static POLE pmWym_luk_o[] = {
		{u8"R",'R',209,NULL},
		{u8"L",'L',201,NULL} };

static TMENU mWym_luk_o = { 2,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,5,0,0,0,(POLE(*)[]) &pmWym_luk_o,NULL,NULL };

static POLE pmWym_okrag_o[] = {
		{u8"D",'D',211,NULL},
		{u8"R",'R',212,NULL} };

static TMENU mWym_okrag_o = { 2,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmWym_okrag_o,NULL,NULL };

static POLE pmWymiarujm[] = {
		{u8"Direction\0=\0        ",'D',196,&mKierunek},
		{u8"New\0End",'N',197,NULL},
		{u8"Remove\0Del",'R',198,NULL},
		{u8"Edit\0F10",'E',199,NULL},
		{u8"Settings\0",'S',200,NULL},
		{u8"Arc\0R\0             ",'A',201,&mWym_luk_o},
		{u8"Circle\0D\0           ",'C',202,&mWym_okrag_o}, };

#define _NEW_ u8"new"

static POLE pmStrzalka[] = {
	  {u8"Arrowheads",'A',189,NULL},
	  {u8"Ticks",'T',190,NULL},
	  {u8"Dots",'D',191,NULL} };

static POLE pmKierunekT[] = {
	{u8"Parallel",'P',192,NULL},
    {u8"Collinear",'C',696,NULL},
	{u8"Horizontal",'H',193,NULL} };

static POLE pmAddLine[] = {
	{u8"Fixed length",'F',194,NULL},
	{u8"Align to object",'A',195,NULL} };


static POLE pmLFormat[] = {
	{u8"Decimal",'D',0,NULL},
    {u8"Engineering",'E',0,NULL},
	{u8"Architectural",'A',0,NULL},
    {u8"Fractional",'F',0,NULL},
};

static POLE pmFractions[] = {
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
	  {u8"֎Line color\0            ", 'L',177,&mKolorL},  //0
	  {u8"֎Text color\0            ", 'T',178,&mKolorT},   //1
	  {u8"text Height\0            ", 'H',179,NULL},       //2
	  {u8"text Width factor\0      ", 'W',180,NULL},      //3
	  {u8"Italic text\0            ", 'I',181, &mItalics},   //4
	  {u8"Bold text\0              ", 'B',182, &mBold},    //5
	  {u8"Font\0                   ", 'F',183, &mCzcionkaW},  //6
      {u8"Numeric format\0         ", 'N',697, &mLFormat},   //7
	  {u8"Precision\0              ", 'P',184,NULL},   //8
	  {u8"teRminators\0            ", 'R',185,&mStrzalka},  //9
	  {u8"line Offset\0            ", 'O',186,NULL},  //10
	  {u8"text Direction\0         ", 'D',187,&mKierunekT} ,  //11
	  {u8"Extension lines\0        ", 'E',188,&mAddLine} };  //12


  char *_LFORMAT_[]={u8"Decimal",u8"Engineering",u8"Architectural",u8"Fractional"};

#endif

#ifdef __O_DIVIDE__

POLE pmDiv_Pline[] = {
	{u8"Polyline",'P',454,NULL},
	{u8"Segment",'S',455,NULL} };

static TMENU mDiv_Pline = { 2, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 8, 0, 0,0,(POLE(*)[]) &pmDiv_Pline, NULL, NULL };
static TMENU mAlign_Block = { 2, 0,0, 7, 83, 13, ICONS, CMNU,CMBR,CMTX,0, 10, 0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmDivide[] = {
	 {u8"Divide\0",'D',448,NULL},
	 {u8"No of segments\0 5\0      ", 'N', 449,NULL},
	 {u8"Measure\0", 'M',450, NULL},
	 {u8"segment Length\0 100.0\0       ", 'L',451, NULL},
	 {u8"maXimum\0 1000\0",'X', 452,NULL},
	 {u8"Polyline/segment\0   polyline\0  ",'P',453, &mDiv_Pline},
	 {u8"Remove\0Del\0",'R', 270,NULL},
	 {u8"Align block\0 Y\0",'A', 456,&mAlign_Block} };

#define _POLYLINE_ u8"polyline"
#define _SEGMENT_ u8"segment"

char _YES__[4] = u8"Y";
char _NO__[4] = u8"N";

#define _DIVIDE_ u8"Divide"
#define _DIVIDE_C_ L'D'

#endif

#ifdef __O_KONSTR__

static char* opis_zbrojenia[] = { u8"Reinforcing bar" };

#endif

#ifdef __O_RECTA__

static POLE	pmKatR[]= {
	 {u8"rotate By angle",'B',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",	    '0',578,NULL},
	 {u8"rotate To angle",'T',440,NULL},
	 {u8"paRallel to",'R',580,NULL},
	 {u8"Perpendicular to",'P',581,NULL},
	 {u8"at Angle to",'A',582,NULL},
};

TMENU mKatR = { 13,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmKatR,NULL,NULL };

static POLE	pmRectOpt[]= {
	 {u8"Two points",L'T',22,NULL},
     {u8"two points and an Insertion point/angle",L'I',684,NULL},
};

TMENU mRectOpt = { 2,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,13,0,0,0,(POLE(*)[]) &pmRectOpt,NULL,NULL };

static POLE	pmKatOptR[]= {
	 {u8"rotate By angle",'B',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",	    '0',578,NULL},
	 {u8"rotate To angle",'T',440,NULL},
	 {u8"paRallel to",'R',580,NULL},
	 {u8"Perpendicular to",'P',581,NULL},
	 {u8"at Angle to",'A',582,NULL},
     {u8"Drawing option",L'D',685,&mRectOpt},
};

TMENU mKatOptR = { 14,0,0,10,74,6,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmKatOptR,NULL,NULL };


#define _RECTA_A_ u8"Rectangle axis angle and drawing options"
#define _RECTA_R_ u8"Rectangle rotation"
#define _RECTA_C_ L'R'

#endif

#ifdef __O_LTTYPE__

POLE pmTTF_OTF[] = {
   {u8"TrueType Fonts",'T',703,NULL},
   {u8"OpenType Fonts",'O',704,NULL},
};

TMENU mTTF_OTF = { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmTTF_OTF,NULL,NULL };

#endif
