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

#define __EDIT_MULTILINE__ L"Editar texto de varias líneas"

#define FILENAME_INI L"ALFACADES.INI"
LPWSTR filename_ini = (LPWSTR)L"Editar ALFACADES.INI";

// Create a locale object representing the Spanish (Spain) locale
#define _LOCALE_ "es-ES"

char __BOLD__[]=u8"Negrita";
char __UNDERLINED__[]=u8"Subrayado";
char __ITALIC__[]=u8"Cursiva";
char __A_TO_LEFT__[]=u8"Ajustar a la izquierda";
char __A_TO_RIGHT__[]=u8"Ajustar a la derecha";
char __A_TO_MIDDLE__[]=u8"Ajustar al medio";
char __A_TO_CENTRE__[]=u8"Ajustar al centro";
char __HIDDEN__[]=u8"Oculto";
char __COPY_TEXT__[]=u8"Copiar texto del portapapeles";

char _EDIT_TEXT_[]=u8"Edición de texto";

#endif

#ifdef __WINPRINT__

#define _NEW_WINDOW_ u8"¿Quieres abrir una nueva ventana de dibujo?"
#define _NEW_WINDOW_T_ u8"Nueva ventana"

int max_quote = 541;
#define _QUOTE_ u8"quoteses.dat"
char about_pre[32] = u8"\n\nCita del día ";
char _about_[16] = u8"sobre ";

#endif

#ifdef __BIBRYS__

POLE pmNie_Tak_[] = {
	{u8"No",'N',113,NULL},
	{u8"Sí",'S',112,NULL} };

POLE pmEditPCX[] = {

	{u8"Modificar dx, dy", 'M',114, NULL},
	{u8"cambiar Parámetros",'P',54, NULL},
	{u8"Calibración",'C',115,NULL},
	// {u8"Editar bloque", 'E',0, NULL},
};

TMENU mEditPCX = { 3,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmEditPCX,NULL,NULL };
TMENU mSaveLayer = { 2,0,0,7,16,8,ICONS,CMNU,CMBR,CMTX,0,18,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmInsertPCX[] = {
    {u8"insertar Mapa\0\0 ",'M',600,NULL},
	{u8"insertar foto como PNG\0\0 ",'P',601,NULL},
	{u8"insertar foto como JPG\0\0 ",'J',624,NULL},
    {u8"vectorizar a ALX e importar\0\0 ",'A',698,NULL},
};

TMENU mInsertPCX = { 4,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,23,0,0,0,(POLE(*)[]) &pmInsertPCX,NULL,NULL };

POLE pmElliptical[] = {
	{u8"Elipse", 'E',19,NULL},
    {u8"Disco elíptico", 'D',707,NULL},
    {u8"Arco elíptico", 'A',706,NULL},
};

TMENU mElliptical = { 3,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmElliptical,NULL,NULL };

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
	{u8"Rectangular",	'R', 116,NULL},
	{u8"Circular", 'C', 117,NULL},
	{u8"circular Más rotación", 'M', 118,NULL},
};

TMENU mArray = { 3, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, 20, 0, 0,0,(POLE(*)[]) &pmArray, NULL, NULL };

POLE pmMirrorBlock[] = {
	{u8"bloque espejo por X",'X',40,NULL}, 	   
	{u8"bloque espejo por Y",'Y',41,NULL}, 	   
};

TMENU mMirrorBlock = { 2, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, 23, 0, 0,0,(POLE(*)[]) &pmMirrorBlock, NULL, NULL };

POLE  pmEdycja[] = {
	{u8"Mover\0\0",         'M',30,NULL},
	{u8"Copiar\0\0",        'C',31,NULL},
	{u8"Borrar\0\0",        'B',32,NULL},
	{u8"< restaurar\0\0",   '<',33,NULL},
	{u8"Rotar\0\0",         'R',34,NULL},
	{u8"*rotar bloque\0\0", '*',35,NULL},
	{u8"escaLa\0\0",        'L',36,NULL},
	{u8"eStiramiento\0\0",  'S',37,NULL},
	{u8"Editar texto\0\0",  'E',38,NULL},
	{u8"esPejo\0\0",        'P',39,NULL},
	{u8"bloque espejo\0\0", 'X',535,&mMirrorBlock},
	{u8"mAtriz\0\0",        'A',42,&mArray},
	{u8"roMper\0\0",        'M',44,NULL},
	{u8"recorTar\0\0",      'T',45,NULL},
	{u8"exteNder\0\0",      'N',46,NULL},
	{u8"Filete\0\0",        'F',47,NULL},
	{u8"chaflán\0\0",       '/',48,NULL},
	{u8"Desplazamiento\0\0",'D',49,NULL},
	{u8"diVidir\0\0",       'V',50,NULL},
	{u8"@ deshacer\0 \0",   '@',51,NULL},
    {u8"complementos\0 \0", 'U',534,&mInsetEdit},

};

POLE pmRysuj[] =
{
	{u8"Línea",             'L',13,NULL},
	{u8"Círculo",           'C',14,NULL},
	{u8"Arco",              'A',15,NULL},
	{u8"Texto",             'T',16,NULL},
	{u8"texto Multilínea",  'M',408,NULL},
	{u8"políGono",          'G',17,NULL},
	{u8"Disco",             'D',18,NULL},
	{u8"Elíptico",          'E',705,&mElliptical},
	{u8"Polilínea",         'P',20,NULL},
	{u8"Sólido",            'S',21,NULL},
	{u8"Bézier spline",     'B',516,NULL},
	{u8"Rectángulo",        'R',22,NULL},
	{u8"trazo",             '=',23,NULL},
	{u8"2 líneas",          '2',24,NULL},
	{u8"3 líneas",          '3',25,NULL},
	{u8"bosQuejo",          'Q',26,NULL},
	{u8"puntO",             'O',27,NULL},
    {u8"Vector",            'V',722,NULL},
	{u8"gráfico de Función",'F',28,NULL},
	{u8"complementos",      'U',534,&mInsetDraw},

};

POLE pmBlok[] = {
	{u8"Copiar bloque\0\0 ",        'C',58,NULL},
	{u8"Pegar bloque\0\0 ",         'P',59,NULL},
	{u8"Exportar bloque\0\0 ",      'E',61,NULL},
	{u8"Importar bloque\0\0 ",      'I',60,NULL},
	{u8"Catálogo abierto\0\0 ",     'O',460,NULL},
	{u8"iMportar bloque DXF\0\0 ",  'M',84,NULL},
	{u8"iNsertar imagen\0\0 ",      'N',62,&mInsertPCX},
	{u8"eDitar bloque de imagen\0\0 ",'D',63,&mEditPCX},
	{u8"cambiar patrón de bloque de Sombreado\0\0 ",'S',599,NULL},
	{u8"exporTación de imagen\0\0 ",'T',64,NULL},
	{u8"Grupo\0\0 ",                'G',65,NULL},
	{u8"eXplotar\0\0 ",             'X',66,NULL},
    {u8"transformación de cUadrilátero\0\0 ",'U',682,NULL},
	{u8"Lista\0\0 ",                'L',67,NULL},
	{u8"* explotar congelado\0\0 ", '*',68,NULL},
	{u8"Borrar bloques DXF\0\0 ",   'B',69,NULL},
	{u8"bloques coRrectos\0\0 ",    'R',70,NULL},
	{u8"capas Fijas\0 N\0 ",        'F',71,&mSaveLayer},
    {u8"complementos\0 \0",         'U',534,&mInsetBlock},
};

POLE  pmGeometria[] = {
	{u8"Área de superficie", 'A',72,NULL},
	{u8"Centro del área", 'C',73,NULL},
	{u8"Primer momento de área", 'P',74,NULL},
	{u8"área Momento de inercia", 'M',75,NULL},
	{u8"área central madre de Inercia",'I',76,NULL},
	{u8"Test de área cerrada", 'T',77,NULL},
};

char _YES_[4] = "S";
char _NO_[4] = "N";

const char *_EDIT_[] = { u8"Mover", u8"mover Z", u8"Girar", u8"girar XZ", u8"girar YZ", u8"Escala", u8"Espejo", u8"->saltar", u8"-----" }; 

char* loading_program[] = { u8"Cargando el programa", u8"Cargando recursos..." , u8"Por favor espere. Descargando archivo: ", u8"Conectándose a la nube..."};

#define _NO_MOUSE_ "¡El mouse no está instalado!\n"

#endif

#ifdef __O_WYJ__

#ifdef LINUX
#define ALF_PROGRAM "AlfaCADES"
#define ALF_TITLE "AlfaCADES4Linux"
#else
#ifdef BIT64
#define ALF_PROGRAM "AlfaCADES4Win64.exe"
#define ALF_TITLE "AlfaCADES4Win64"
#else
#define ALF_PROGRAM "AlfaCADES4Win.exe"
#define ALF_TITLE "AlfaCADES4Win"
#endif
#endif
static char err_message[] = u8"Error al procesar el archivo DWG";
char err_message_code[] = u8"Código de salida :";
char err_message1[] = u8"Error al procesar el archivo provisional.";
static char confirm[] = u8"Confirmar";

POLE pmNewDrawing[] = {
	{u8"Nuevo dibujo",'N',79,NULL},
	{u8"nuevo de Plantilla",'P',533,NULL},
};

static TMENU mNewDrawing = { 2,0,0,32,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,18,0,0,0, (POLE(*)[]) &pmNewDrawing,NULL,NULL };

POLE pmNewWindow[] = {
	{u8"Nuevo dibujo en nueva ventana",'N',79,NULL},
	{u8"nuevo de Plantilla en ventana nueva",'P',533,NULL},
	{u8"Abrir archivo en ventana nueva",'A',78,NULL},
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
	{u8"Entrar",'E',537,NULL},
	{u8"Regresar",'R',538,NULL},
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
	 {u8"Dibujo abierto\0",'D',78,NULL},
	 {u8"Nuevo dibujo\0",'N',79,&mNewDrawing},
	 {u8"nueva Ventana\0",'V',80,&mNewWindow},
	 {u8"Guardar\0",'G',81,NULL},
	 {u8"guardar Como\0",'C',82,NULL},
	 {u8"Estructura de Árbol\0",'A',536,&mTree},
	 {u8"Exportar\0",'E',504,&mExportCAD},
	 {u8"Importar\0",'I',505,&mImportCAD},
	 {u8"editar Bloque de título\0", 'B',85, NULL},
	 /* {u8"Saltar al sistema\0",'S',86,NULL},*/
	 {u8"imPresión\0",'P',87,NULL},
	 {u8"Trazando en un plotter\0",'T',394,NULL},
	 {u8"información\0",'?',88,NULL},
	 {u8"Mostrar escritorio\0", 'M',528, NULL},
	 {u8"Últimos archivos\0",'U',89,&mLastFiles}
};

static POLE pmTTF_OTF[] = {
   {u8"fuentes TrueType",'T',703,NULL},
   {u8"fuentes OpenType",'O',704,NULL},
};

static TMENU mTTF_OTF = { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmTTF_OTF,NULL,NULL };

static POLE pmCzcionkaEkranTTF[] = {
   {u8"Tipo de fuente  \0", 'T',111,&mTTF_OTF},
   {u8"Altura \0\0", 'A',179,NULL},
   {u8"Factor de anchura \0 \0", 'F',230,NULL},
};

TMENU mCzcionkaEkranTTF = { 3,0,0,30,20,7,ICONS | TADD,CMNU,CMBR,CMTX,0,28,0,0,0,(POLE(*)[]) &pmCzcionkaEkranTTF,NULL,NULL };  //26

static POLE pmWindow[] = {
   {u8"expandir Horizontalmente",'H',467,NULL},
   {u8"expandir Verticalmente",'V',468,NULL},
   {u8"expandir en Diagonalmente",'D',469,NULL},
   {u8"expandir Ventana",'V',470,NULL},
   {u8"restaurar Último",'U',471,NULL},
};

static TMENU mWindow = { 5,0,0,16,22,9,ICONS,CMNU,CMBR,CMTX,0,11,0,0,0,(POLE(*)[]) &pmWindow,NULL,NULL };  //9


static POLE pmDialogCursor[] = {
   {u8"Pequeño",'P',591,NULL},
   {u8"Grande",'G',592,NULL},
};

#define smallcursor u8"Pequeño"
#define bigcursor u8"Grande"

static TMENU mDialogCursor = { 2,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,24,0,0,0,(POLE(*)[]) &pmDialogCursor,NULL,NULL };  //22

static POLE pmMenuCursor[] = {
   {u8"Barra",'B',821,NULL},
   {u8"Cursor",'C',822,NULL},
};

static TMENU mMenuCursor = { 2,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,33,0,0,0,(POLE(*)[]) &pmMenuCursor,NULL,NULL };  //22

#define barstyle u8"Barra"
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
};


static TMENU mTranslucency = { 8,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,16,0,0,0,(POLE(*)[]) &pmTranslucency,NULL,NULL };  //14


static TMENU mDemoSelect = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,26,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };  //24

static TMENU mAutoPan = { 2,0,0,7,33,11,ICONS,CMNU,CMBR,CMTX,0,31,0,0,0,(POLE(*)[]) & pmTak_Nie,NULL,NULL };  //new

POLE pmOpcje[] = {
	{u8"configuración del Tablero de dibujo\0",'T',109,NULL},
	{u8"Panorámica automática\0Sí\0 ",'P',145, &mAutoPan}, //PAN
	{u8"factor de panoramización Automática\0 ",'A',110,NULL},
	{u8"Opacidad de los rellenos\0 ",'O',542,&mTranslucency},
	{u8"Fuente de escritorio\0 ",'F',111,&mCzcionkaEkranTTF},
	//{u8"fuente de Escritorio\0 ",'E',111,NULL},
	{u8"fondo de Escritorio\0 ",'E',527,NULL},
	{u8"Cursor de diálogo\0 ",'C',590,&mDialogCursor},
    {u8"estilo de Menú\0",'M',820,&mMenuCursor},
	{u8"modo de Demostración educativo\0 No\0",'D',661,&mDemoSelect},
	{u8"Guardar configuración de ventana\0",'G',530,NULL},
	{u8"configuración de Ventana\0",'V',478,&mWindow},
};

#define _FILE_ u8"El archivo "
#define _FILE_EXISTS_ u8" existe. ¿Sobrescribirlo? (Sí/No/Esc):"

#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "S"
#define _NO__ "N"
#define _INTERRUPT_ u8"¡El bloque '%s' no existe! Impresión interrumpida. "
#define _SPOOLED_INTERRUPT_ u8"¡Error al escribir el archivo en spool! Impresión interrumpida. "
#define _FORMAT_INTERRUPT_ u8"¡No hay bloque '*FORMAT'! Impresión terminada"
#define _PAGE_INTERRUPT_ u8"¡No hay bloque '*PAGE'! Impresión terminada"
#define _CONVERSION_TERMINATION_ u8"¡ESPERE! Convirtiendo archivo. Terminación -> Esc... "
#define _ERROR_DXF_ u8"Error en archivo DXF o terminación  "
#define _LINE_NO_ u8" -> fila %#ld"

#define _TO_MANY_WIN_ u8"Demasiadas ventanas abiertas"
#define _CLOSE_WINDOW_ u8"¿Quieres cerrar la ventana?"

#define _DRAWING_NOT_SAVED_ u8"Dibujo no guardado"

#define _Yes_ u8"Sí"
#define _No_ u8"No"

#define _SOME_OPEN_DRAWING_NOT_SAVED_ u8"Algunos dibujos abiertos no se guardan"

#define _SAVE_IT_ u8"¿Guardarlo?"
#define _SAVE_THEM_ u8"¿Guardarlos?"

#define _NO_MORE_DRAWINGS_ u8"No mas dibujos"

#define _NEW_DRAWING_ u8"Nuevo dibujo"

#define _SAVING_CONF_ u8"Guardar configuración";

#define _WANT_EXIT_ u8"¿Quieres salir del programa?"

#define _WANT_CLOSE_WINDOW_ u8"¿Quieres cerrar la ventana?"

#define _SAVING_PREFILES_ "Guardando HISTORIAL"

#define _FONT_CHANGED_ u8"Se ha cambiado la fuente"

#define desktop_tip "@Seleccionar dibujo"

#define _DEMO_MODE_ "En DEMO teclas presionadas y botones del mouse"
#define _DEMO_MODE1_ "se muestran en la parte inferior de la ventana"

#endif

#ifdef __ASKING__

#define _Yes_ u8"Sí"
#define _No_ u8"No"

#define _NOTICE_ u8"La noticia"
#define _AT_NOTICE_  u8"@La noticia"
#define _COMBINATION_  u8"Número de combinación de carga"
#define _GEOM_STIFFNESS_ u8"Rigidez geométrica"
#define _INERTIA_ u8"Incluir carga muerta (DL) en las vibraciones"
#define _VIBRATIONS_ u8"Número de modos dinámicos de vibración"

#define _INSTALATION_ u8"Instalación"

static char* combination_txt[9] = {
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
					u8"8"
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

char instruct_txt[][80]={u8"Abrir menú / Aceptar",
                        u8"Cerrar menú / Escapar",
                        u8"Liberación el mouse (o agitar hacia la izquierda y la derecha)",
                        u8"Menú auxiliar",
                        u8"Menú contextual auxiliar",
                        u8"Ortogonalidad activada/desactivada",
                        u8"Zoom/desplazamiento",
                        u8"Panorámica (mano de Mickey Mouse)",
                        u8"Repetición de comando",
                        u8"Selección del mango (hay más)",
                        u8"Mano amiga (todas las teclas rápidas)",
                        u8"(izquierda) cambiando el diseño del teclado en la edición",
                        u8"para letras griegas y símbolos matemáticos",
                        u8"son teclas pegadas. En caso de conflicto con",
                        u8"teclas abreviadas del sistema, presiónela primero y suéltela",
                        u8"luego presione otra tecla de la combinación",
                        u8"Edición de valores numéricos o reedición de texto",
                        u8"  o"};

#define osnap_icon0 156
#define osnap_icon1 164
#define osnap_icon2 160
#define osnap_icon3 154
#define osnap_icon4 167
#define osnap_icon5 171

char stay_string[64]="No mostrarlo de nuevo";

#define _Yes_ u8"Sí"
#define _No_ u8"No"

#define _INSTRUCTION_ u8"La instrucción"
#define _AT_INSTRUCTION_  u8"@La instrucción"

#endif

#ifdef __DIALOG__

#define _OTHER_ u8"֍otro >>>"
#define _LAYERS_ u8"Capas"

#endif

#ifdef __ADDMENU__

#define MAXPLACEMARKER 5

#define _YES_ "S"
#define _NO_ "N"
static char ggsel[10][16] = { u8"punto Final", u8"Extremo más cercano", u8"Medio", u8"Intersección", u8"Centro", u8"Perpendicular",
"Tangencial", "Adyacente", "punto","----" };

POLE pmKolor[] = {
	/*1*/ {u8"֍[A] rojo",'A',0,NULL}, /*12*/
	/*2*/ {u8"֍[B] amarillo",'B',0,NULL}, /*14*/
	/*3*/ {u8"֍[C] verde",'C',0,NULL}, /*10*/
	/*4*/ {u8"֍[D] turquesa" ,'D',0,NULL}, /*11*/
	/*5*/ {u8"֍[E] azul",'E',0,NULL}, /*9*/
	/*6*/ {u8"֍[F] magenta",'F',0,NULL}, /*13*/
	/*7*/ {u8"֍[G] blanco",'G',0,NULL}, /*15*/
	/*8*/ {u8"֍[H] gris oscuro",'H',0,NULL}, /*8*/
	/*9*/ {u8"֍[I] rojo oscuro",'I',0,NULL}, /*4*/
	/*10*/ {u8"֍[J] marrón",'J',0,NULL}, /*6*/
	/*11*/ {u8"֍[K] verde oscuro",'K',0,NULL}, /*2*/
	/*12*/ {u8"֍[L] turquesa oscuro",'L',0,NULL}, /*3*/
	/*13*/ {u8"֍[M] azul oscuro",'M',0,NULL}, /*1*/
	/*14*/ {u8"֍[N] magenta oscuro",'N',0,NULL}, /*5*/
	/*15*/ {u8"֍[O] gris",'O',0,NULL}, /*7*/
	/*16*/ {u8"֍[P] negro",'P',0,NULL}, /*0*/
	/*17*/ {u8"֍[X] otro\0 0\0 ",'X',0,NULL}
};

POLE pmKolorL_[] = {
	/*1*/ {u8"֍[A] rojo",'A',0,NULL}, /*12*/
	/*2*/ {u8"֍[B] amarillo",'B',0,NULL}, /*14*/
	/*3*/ {u8"֍[C] verde",'C',0,NULL}, /*10*/
	/*4*/ {u8"֍[D] turquesa" ,'D',0,NULL}, /*11*/
	/*5*/ {u8"֍[E] azul",'E',0,NULL}, /*9*/
	/*6*/ {u8"֍[F] magenta",'F',0,NULL}, /*13*/
	/*7*/ {u8"֍[G] blanco",'G',0,NULL}, /*15*/
	/*8*/ {u8"֍[H] gris oscuro",'H',0,NULL}, /*8*/
	/*9*/ {u8"֍[I] rojo oscuro",'I',0,NULL}, /*4*/
	/*10*/ {u8"֍[J] marrón",'J',0,NULL}, /*6*/
	/*11*/ {u8"֍[K] verde oscuro",'K',0,NULL}, /*2*/
	/*12*/ {u8"֍[L] turquesa oscuro",'L',0,NULL}, /*3*/
	/*13*/ {u8"֍[M] azul oscuro",'M',0,NULL}, /*1*/
	/*14*/ {u8"֍[N] magenta oscuro",'N',0,NULL}, /*5*/
	/*15*/ {u8"֍[O] gris",'O',0,NULL}, /*7*/
	/*16*/ {u8"֍[P] negro",'P',0,NULL}, /*0*/
	/*17*/ {u8"֍[X] otro\0 0\0 ",'X',0,NULL}
};

POLE pmKolorT_[] = {
	/*1*/ {u8"֍[A] rojo",'A',0,NULL}, /*12*/ //֍rojo
	/*2*/ {u8"֍[B] amarillo",'B',0,NULL}, /*14*/
	/*3*/ {u8"֍[C] verde",'C',0,NULL}, /*10*/
	/*4*/ {u8"֍[D] turquesa" ,'D',0,NULL}, /*11*/
	/*5*/ {u8"֍[E] azul",'E',0,NULL}, /*9*/
	/*6*/ {u8"֍[F] magenta",'F',0,NULL}, /*13*/
	/*7*/ {u8"֍[G] blanco",'G',0,NULL}, /*15*/
	/*8*/ {u8"֍[H] gris oscuro",'H',0,NULL}, /*8*/
	/*9*/ {u8"֍[I] rojo oscuro",'I',0,NULL}, /*4*/
	/*10*/ {u8"֍[J] marrón",'J',0,NULL}, /*6*/
	/*11*/ {u8"֍[K] verde oscuro",'K',0,NULL}, /*2*/
	/*12*/ {u8"֍[L] turquesa oscuro",'L',0,NULL}, /*3*/
	/*13*/ {u8"֍[M] azul oscuro",'M',0,NULL}, /*1*/
	/*14*/ {u8"֍[N] magenta oscuro",'N',0,NULL}, /*5*/
	/*15*/ {u8"֍[O] gris",'O',0,NULL}, /*7*/
	/*16*/ {u8"֍[P] negro",'P',0,NULL}, /*0*/
	/*17*/ {u8"֍[X] otro\0 0\0 ",'X',0,NULL}
};

TMENU mKolorAC = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,23,56,5,0,CMNU,CMBR,CMTX,15,72,0,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };

#define _OTHER_NO_ u8"֍[X] otro >>> %#ld"

static POLE pmKrok_k[9] = {
	 {u8"[A]\0  2.5  *",'A',0,NULL},
	 {u8"[B]\0  1.25 *",'B',0,NULL},
	 {u8"[C]\0  0.25 *",'C',0,NULL},
	 {u8"[D]\0  0.5",'D',0,NULL},
	 {u8"[E]\0  1",'E',0,NULL},
	 {u8"[F]\0  5",'F',0,NULL},
	 {u8"[G]\0 10",'G',0,NULL},
	 {u8"[X]\0 X\0",'X',0,NULL},
	 {u8"Mínimo\0",'M',0,NULL} };

static TMENU mKrok_k = { 9,0,0,13,52 ,6,TADD,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmKrok_k,NULL,NULL };

POLE pmTak_Nie[2] = {
	{u8"Sí",'S',112,NULL},
	{u8"No",'N',113,NULL} };

POLE pmGruboscLinii[] = {
	{u8"҂muy Delgada",'D',0,NULL},
	{u8"҂Fina",'F',0,NULL},
	{u8"҂Gruesa",'G',0,NULL},
	{u8"҂Muy gruesa",'M',0,NULL},
	{u8"҂Extra gruesa",'E',0,NULL},
};

POLE pmGruboscLiniiS[] = {
    {u8"Relleno",'R',0,NULL},
	{u8"҂muy Delgada",'D',0,NULL},
	{u8"҂Fina",'F',0,NULL},
	{u8"҂Gruesa",'G',0,NULL},
	{u8"҂Muy gruesa",'M',0,NULL},
	{u8"҂Extra gruesa",'E',0,NULL},
};

static POLE pmTypyLinii[] = {
    {u8"֏sólida",'1',0,NULL},
	{u8"֏discontinua", '2', 0, NULL},
	{u8"֏punteada", '3', 0, NULL},
	{u8"֏2-punteada",'4',0,NULL},
	{u8"֏multipunteada", '5', 0, NULL},
	{u8"֏borde",'6',0,NULL},
	{u8"֏mediana",'7',0,NULL},
	{u8"֏escondida",'8',0,NULL},
	{u8"֏fantasma",'9',0,NULL},
	{u8"֏discontinua_2", 'A', 0, NULL},
	{u8"֏punteada_2", 'B', 0, NULL},
	{u8"֏2-punteada_2",'C',0,NULL},
	{u8"֏multipunteada_2",'D',0,NULL},
	{u8"֏borde_2",'E',0,NULL},
	{u8"֏mediana_2",'F',0,NULL},
	{u8"֏escondida_2",'G',0,NULL},
	{u8"֏fantasma_2", 'H', 0, NULL},
	{u8"֏discontinua_x2", 'I', 0, NULL},
	{u8"֏punteada_x2", 'J', 0, NULL},
	{u8"֏2-punteada_x2",'K',0,NULL},
	{u8"֏multipunteada_x2",'L',0,NULL},
	{u8"֏borde_x2",'M',0,NULL},
	{u8"֏mediana_x2",'N',0,NULL},
	{u8"֏escondida_x2",'O',0,NULL},
	{u8"֏fantasma_x2", 'P', 0, NULL},
	{u8"֏3-punteada",'Q',0,NULL},
	{u8"֏2-mediana",'R',0,NULL},
	{u8"֏3-discontinua",'S',0,NULL},
	{u8"֏fantasma_3", 'T', 0, NULL},
	{u8"֏2-mediana_x2",'U',0,NULL},
	{u8"֏3-discontinua_x2", 'V', 0, NULL},
	{u8"֏multidiscontinua",'W',0,NULL},
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
	    {u8"paso de Puntero\0 ", 'P',154,&mKrok_k}, /*.25*/
		{u8"Densidad de cuadrícula\0 ", 'D',155,&mGestosc }, /*2.5*/
		{u8"pUntos de cuadrícula\0 ", 'U',156,&mPunkty_siatki},
		{u8"Ortogonalidad (F8)\0 ", 'O',157,&mOrto},
		{u8"֎Color\0Blanco\0 ", 'C',158,&mKolorAC}, /*Biały*/
		{u8"Grosor de línea\0Grueso\0 ", 'G',159,&mGruboscLinii},
		{u8"Tipo de línea\0Continua\0 ", 'T',160,&mTypyLinii}, /*Ciągła*/
		{u8"puntos y contornos Visibilidad\0 Y\0 ", 'V', 161,&mPoint_View},
		{u8"taMaño en puntos\0 2.5\0 ", 'M',162,NULL},
		{u8"obtener color (Alt-F8)\0", 'K',163,NULL},
		{u8"obtener tipo (Alt-F9)\0", 'Y',164,NULL},
		{u8"obtener capa (Alt-F10)\0", 'Q',165,NULL},
		{u8"bloque Libre \0 ", 'L',167,&mSwobodnyBlok},
		{u8"texto libre \0 ", 'F',166,&mSwobodnyTekst},
		{u8"Escalado de bloque de dimensión\0 N\0 ", 'E',168,&mScale_DIM},
		{u8"estirar líneas en Nodos\0 T\0 ",'N',169,&mCiagniecieLinii},
		{u8"Romper líneas en nodos\0 T\0 ",'R',170,&mRozcinanieLinii},
		{u8"ocultar tipoS\0 T\0 ", 'S',171,&mUkrywanieTypu},
		{u8"visibilidad de teXto oculto\0 N\0 ",'X',172,&mWidocznoscTypu},
		{u8"llenando puntos de Imagen\0 N\0 ", 'I',173,&mWypelnianiePCX},
		{u8"resaltar la capa Actual\0 N\0 ",'A',174,&mView_Only_C_L},
		{u8"el Buffer de imágenes\0 S\0 ", 'B',175,&mBuffering_PCX},
		// {u8"visibilidad de los bloques de hoja\0 N\0 ",'H',0,&mView_Sheets},
		{u8"normaliZación de texto\0 ", 'Z',176,&mNormalizacjaTekstu},
};

TMENU mParametrym = { 23,0,0,38,23,4,ICONS | TADD,CMNU,CMBR,CMTX,0,59,0,0,0,(POLE(*)[]) &pmParametry,NULL,NULL };

POLE pmParametry_view[] = {
	 {u8"paso de Puntero\0",  'P',154,&mKrok_k},   /*.25*/
	 {u8"Densidad de cuadrícula\0", 'D',155,&mGestosc }, /*2.5*/
	 {u8"pUntos de cuadrícula\0", 'U',156,&mPunkty_siatki},
	 {u8"punto Visibilidad\0 S\0",  'V', 161,&mPoint_View},
	 {u8"taMaño de punto\0 2.5\0", 'M',162,NULL},
	 {u8"visibilidad de teXto oculto\0 N\0",'X',172,&mWidocznoscTypu},
	 {u8"resaltar la capa Actual\0 N\0",'A',174,&mView_Only_C_L},
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
	 {u8"Actualizar\0",'A',142,NULL},
	 {u8"Guardar vista\0", 'G', 140,&mSetMarker},
	 {u8"Buscar vista\0", 'B', 141,&mFindMarker},
	 {u8"Todos\0",'T',143,NULL},
	 {u8"Centro\0",'C',144,NULL},
	 ////{u8"auto Pan\0Yes\0     ",'P',145, &mAutoPan},  //PAN
	 {u8"Pan (Ctrl-Shift)\0",'P',145, NULL},
	 {u8"[1] *1.0 \0",'1',146,NULL},
	 {u8"[5] *0.5\0",'5',147,NULL},
	 {u8"[2] *0.2\0",'2',148,NULL},
	 {u8"[0] *0.1\0",'0',149,NULL},
	 {u8"*x\0",'*',150,NULL},
	 {u8"Ventana\0",'V',151,NULL},
	 {u8"Dibujo\0",'D',152,NULL},
	 {u8"Último\0",'U',153,NULL},
	 {u8"move to x;y\0",'X',129,NULL},
	 {u8"move by dx;dy\0",'Y',130,NULL},
	 {u8"move polar L;a\0",'L',131,NULL},
	   }; /*rownolegle nalezy modyfikowac funkcje SkalaZ  (1.,.5,.2,.1)*/

TMENU mSkala = { 17,0,0,20,12,4,ICONS | TADD,CMNU,CMBR,CMTX,0, 30, 0,0,0,(POLE(*)[]) &pmSkala,NULL,NULL };

static POLE pmTryb[] = {
	{u8"punto Final\0",'F',120,NULL},
	{u8"Extremo más cercano\0",'E',121,NULL},
	{u8"Medio\0",'M',122,NULL},
	{u8"Intersección\0",'I',123,NULL},
	{u8"Centro\0",'C',124,NULL},
	{u8"Perpendicular\0",'P',125,NULL},
	{u8"Tangencial\0",'T',126,NULL},
	{u8"Adyacente\0",'A',127,NULL},
	{u8"puntO\0",'O',128,NULL},
	{u8"ninguno\0",'X',132,NULL} };

static TMENU mTryb = { 10,0,0,20,20,6,ICONS,CMNU,CMBR,CMTX,0, 13, 0,0,0,(POLE(*)[]) &pmTryb,NULL,NULL };

static POLE pmPunkt[] = {
	{u8"modo de Repetición\0",'R',119,&mTryb},
	{u8"Extremo más cercano\0",'E',121,NULL},
	{u8"Medio\0",'M',122,NULL},
	{u8"Intersección\0",'I',123,NULL},
	{u8"Centro\0",'C',124,NULL},
	{u8"Perpendicular\0",'P',125,NULL},
	{u8"Tangencial\0",'T',126,NULL},
	{u8"Adyacente\0",'A',127,NULL},
	{u8"puntO\0",'O',128,NULL},
	{u8"mover a X, Y\0",'X',129,NULL},
	{u8"movimiento por dX, dY\0",'Y',130,NULL},
	{u8"mover polar L, a\0",'L',131,NULL}, };

TMENU mPunkt = { 13,0,0,30,1,4,ICONS | TADD,CMNU,CMBR,CMTX,0, 7, 0,0,0,(POLE(*)[]) &pmPunkt,NULL,NULL };

static POLE pmSum[] = {
	 {u8"Dejar de agregar\0 F10",'D',139,NULL},
     {u8"Sumar/restar\0 F9",'S',137,NULL},
};

static TMENU mSum = { 2, 0,0,23,56,9,ICONS | TADD,CMNU,CMBR,CMTX,0, 7, 0, 0,0,(POLE(*)[]) &pmSum, NULL,NULL };

POLE pmMeasure[] = {
	{u8"Calculadora", 'C', 133,NULL},
	{u8"Punto",      'P',134,NULL},
	{u8"Distancia",   'D',135,NULL},
	{u8"Ángulo",      'A',136,NULL},
	{u8"Sumatoria",    'S', 137,&mSum},
	{u8"Lista de propiedades", 'L', 138,NULL},
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
	{u8"Punto\0\0",L'P',52, &mPunkt},
	{u8"Vista\01\0",L'V',53, &mSkala},
	{u8"pArámetros\0\0",L'A',54,&mParametrym},
	{u8"Capas\0\0",L'C',55,NULL},
	{u8"Medida\0\0", L'M',56, &mMeasure},
	{u8"Buscar\0\0",L'B',3,NULL},
    {u8"complementos\0\0",L'U',534,&mInsetAux},
	{u8"\0\0",L' ',0,NULL} };

TMENU menup = { 8,0,0,12,1,2,ICONS | TADD | NVERT,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmenup,NULL,NULL };   //7,0,11,

#endif

#ifdef __LAYERS__

#define NoWidthLines	5 /*3*/
#define NoTypeLines	32 /*5*/
static char* width_line_txt_L[NoWidthLines + 1] = { u8"҂muy delgada",u8"҂fina",u8"҂gruesa",u8"҂muy gruesa",u8"҂extra gruesa",u8"" };

static char* Colors_Name_txt_L[MAXCOLOR + 1] =
{
	u8"֍rojo" ,
	u8"֍amarillo" ,
	u8"֍verde" ,
	u8"֍turquesa",
	u8"֍azul",
	u8"֍magenta",
	u8"֍blanco",
	u8"֍gris oscuro",
	u8"֍rojo oscuro" ,
	u8"֍marrón" ,
	u8"֍verde oscuro" ,
	u8"֍turquesa oscuro" ,
	u8"֍azul oscuro" ,
	u8"֍magenta oscuro" ,
	u8"֍gris" ,
	u8"֍negro" ,
	u8"֍otro >>>",
	u8"",
};

static char* type_line_txt_L[NoTypeLines + 1] = {
				u8"֏sólida",
				u8"֏discontinua",
				u8"֏punteada",
				u8"֏2-punteada",
				u8"֏multipunteada",
				u8"֏borde",
				u8"֏mediana",
				u8"֏escondida",
				u8"֏fantasma",
				u8"֏discontinua_2",
				u8"֏punteada_2",
				u8"֏2-punteada_2",
				u8"֏multipunteada_2",
				u8"֏borde_2",
				u8"֏mediana_2",
				u8"֏escondida_2",
				u8"֏fantasma_2",
				u8"֏discontinua_x2",
				u8"֏punteada_x2",
				u8"֏2-punteada_x2",
				u8"֏multipunteada_x2",
				u8"֏borde_x2",
				u8"֏mediana_x2",
				u8"֏escondida_x2",
				u8"֏fantasma_x2",
				u8"֏3-punteada",
				u8"֏2-mediana",
				u8"֏3-discontinua",
				u8"֏fantasma_3",
				u8"֏2-mediana_x2",
				u8"֏3-discontinua_x2",
				u8"֏multidiscontinua",
				u8"" 
};

#define layer_comm0 "PgUp"
#define layer_comm1 "PgDn"
#define layer_comm2 "Nuevo"
#define layer_comm3 "OK"
#define  layer_comm4 "Esc"
#define  layer_comm5 "+"
#define  layer_comm6 "+"
#define  layer_comm7 "+"
#define  layer_comm8 "-"
#define  layer_comm9 "-"
#define  layer_comm10 "-"
#define  layer_comm11 "+"
#define  layer_comm12 "+"
#define  layer_comm13 "-"
#define  layer_comm14 "-"
#define layer_comm15 u8"Actual"
#define layer_comm16 u8"Nombre de capa"
#define  layer_comm17 u8""
#define layer_comm18 u8"Visible"
#define layer_comm19 u8"Editable"
#define layer_comm20 u8"Apuntable"
#define layer_comm21 u8"Color"
#define layer_comm22 u8"Línea"
#define layer_comm23 u8"Grosor"
#define layer_comm24 u8"Línea"
#define layer_comm25 u8"Tipo"
#define layer_comm26 u8"Todas las capas"
#define layer_comm27 u8"Todas las capas"
#define layer_comm28 u8"Gris"
#define layer_comm29 u8"B/N"
#define layer_comm30 u8"Activado"
#define layer_comm31 u8"Desactivado"
#define layer_comm32 u8"Modificar capas"
#define layer_comm33 u8"Borrar"
#define layer_comm34 u8"Capas"
#define layer_comm35 u8"Restaurar"

#define layer_tip0 u8"@Visibilidad"
#define layer_tip1 u8"Editabilidad"
#define layer_tip2 u8"Localizabilidad"
#define layer_tip3 u8"Visibilidad de todas las capas"
#define layer_tip4 u8"Editabilidad de todas las capas"
#define layer_tip5 u8"Localizabilidad de todas las capas"
#define layer_tip6 u8"Color"
#define layer_tip7 u8"Ancho de línea"
#define layer_tip8 u8"Tipo de línea"
#define layer_tip9 u8"en impresora y trazador"
#define layer_tip10 u8"en trazador y impresora"
#define layer_tip11 u8"Gris en la impresión"
#define layer_tip12 u8"Blanco y negro en la impresión"
#define layer_tip13 u8"Todas las capas son grises en la impresión"
#define layer_tip14 u8"Todas las capas en blanco y negro"
#define layer_tip15 u8"Todas las capas"
#define layer_tip16 u8"Borrar capa"
#define layer_tip17 u8"Marcar bloque de capas"

#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'
#define _YES__ "S"
#define _NO__ "N"
#define _Yes_ u8"Sí"
#define _No_ u8"No"
#define _ERASE_LAYER_ u8"ADVERTENCIA: ¿Quieres eliminar toda la capa con todo su contenido?"
#define _FROM_ u8"De"
#define _TO_ u8"A"

#endif

#ifdef __CHPROP__

#define NoWidthLines	6 /*5*/ /*3*/
#define NoTypeLines	32/*5*/
#define NoTextType 16
#define NoTextJust 4

char* Colors_Name_txt[MAXCOLOR + 1] =
{
	u8"֍rojo" ,
	u8"֍amarillo" ,
	u8"֍verde" ,
	u8"֍turquesa",
	u8"֍azul",
	u8"֍magenta",
	u8"֍blanco",
	u8"֍gris oscuro",
	u8"֍rojo oscuro" ,
	u8"֍marrón" ,
	u8"֍verde oscuro" ,
	u8"֍turquesa oscuro" ,
	u8"֍azul oscuro" ,
	u8"֍magenta oscuro" ,
	u8"֍gris" ,
	u8"֍negro" ,
	u8"֍otro >>>",
	u8"",
};

#define chprop_comm0 u8"Cambio de propiedades de objetos"
#define chprop_comm1 u8"Cambiando"
#define chprop_comm2 u8"otras propiedades"

static char NoChange[] = "Sin cambios";

static char* width_line_txt[NoWidthLines + 1] = { u8"҂muy delgada", u8"҂fina", u8"҂gruesa", u8"҂muy gruesa", u8"҂extra gruesa", u8"҂Rellenos", NoChange };

char* type_line_txt[NoTypeLines + 1] = {
				u8"֏sólida",
				u8"֏discontinua",
				u8"֏punteada",
				u8"֏2-punteada",
				u8"֏multipunteada",
				u8"֏borde",
				u8"֏mediana",
				u8"֏escondida",
				u8"֏fantasma",
				u8"֏discontinua_2",
				u8"֏punteada_2",
				u8"֏2-punteada_2",
				u8"֏multipunteada_2",
				u8"֏borde_2",
				u8"֏mediana_2",
				u8"֏escondida_2",
				u8"֏fantasma_2",
				u8"֏discontinua_x2",
				u8"֏punteada_x2",
				u8"֏2-punteada_x2",
				u8"֏multipunteada_x2",
				u8"֏borde_x2",
				u8"֏mediana_x2",
				u8"֏escondida_x2",
				u8"֏fantasma_x2",
				u8"֏3-punteada",
				u8"֏2-mediana",
				u8"֏3-discontinua",
				u8"֏fantasma_3",
				u8"֏2-mediana_x2",
				u8"֏3-discontinua_x2",
				u8"֏multidiscontinua",
				NoChange };

static char* tab_type_txt[NoTextType + 1] = {
				u8"Normal",
				u8"Etiqueta interna",
				u8"Etiqueta de componente",
				u8"Símbolo",
				u8"Tipo",
				u8"*nombre del pin",
				u8"fuente de alimentación",
				u8"Puerto",
				u8"Atributo",
				u8"+Esquema",
				u8"Nombre de archivo",
				u8"red",
				u8"Comentario",
				u8"%variable",
				u8"aUx.símbolo",
				u8"auX.descripción",
				NoChange };


static char* font_just_txt[NoTextJust + 1] = {
				u8"Izquierda",
				u8"Derecha",
				u8"Medio",
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

#define chprop_tips0 u8"@Capa"
#define chprop_tips1 u8"Color"
#define chprop_tips2 u8"Ancho de línea"
#define chprop_tips3 u8"Tipo de línea"
#define chprop_tips4 u8"Tipo de fuente"
#define chprop_tips5 u8"Tipo de texto"
#define chprop_tips6 u8"Texto oculto"
#define chprop_tips7 u8"Alineación del texto"
#define chprop_tips8 u8"Altura de fuente"
#define chprop_tips9 u8"Ancho de fuente"
#define chprop_tips10 u8"Fuente cursiva"
#define chprop_tips11 u8"Fuente en negrita"
#define chprop_tips12 u8"Texto subrayado"
#define chprop_tips13 u8"Sí"
#define chprop_tips14 u8"No"
#define chprop_tips15 u8"Voltear horizontalmente"
#define chprop_tips16 u8"Voltear verticalmente"
#define chprop_tips17 u8"Arriba"
#define chprop_tips18 u8"Opacidad de sólidos, trazos y sombreado de colores sólidos"

char* add_new_font_c = u8"añadir nueva fuente";

#endif

#ifdef __LOADF__

static char copy_button[] = u8"Copiar\0del archivo";
static char parametry_p[] = u8"Parámetros:";
static char edycja_n_p[] = u8"Nombre del parámetro:";
static char edycja_p[] = u8"Valor del parámetro:";
static char lista_p[] = u8"Lista de parámetros:";
static char nowy_katalog[] = u8"nueva carpeta";
static char usun_zbior[] = u8"eliminar archivo";
static char usun_katalog[] = u8"eliminar carpeta";

char* dlg_name[] =
{
   u8"Cargar dibujo desde archivo",
   u8"Guardar dibujo en archivo",
   u8"Importar bloque",
   u8"Importar archivo de imagen",
   u8"Convertir imagen a ALX e importar",
   u8"Exportar PCX",
   u8"Exportar BMP",
   u8"Exportar PNG",
   u8"Exportar JPEG",
   u8"Exportar TIFF",
   u8"Exportar EPS",
   u8"Exportar PDF",
   u8"Importar perfil",
   u8"Importar objeto 3D",
   u8"Guardar bloque",
   u8"Conversión de dibujo a nuevo formato",
   u8"Bloque de conversión a nuevo formato",
   u8"Editar tabla",
   u8"Convertir dibujo de formato DXF",
   u8"Guardar dibujo en formato DXF",
   u8"Guardar archivo makro",
   u8"Cargar archivo Makro",
   u8"Guardar configuración de impresora",
   u8"Cargar configuración de impresora",
   u8"Guardar archivo de catálogo",
   u8"Cargar archivo de catálogo",
   u8"Guardar archivo de coordenadas",
   u8"Guardar registro de ruta de red",
   u8"Cargar ruta desde archivo de coordenadas",
   u8"Abrir catálogo",
   u8"Guardar configuración de trazador",
   u8"Cargar configuración de plóter",
   u8"Importar bloque DXF",
   u8"Convertir dibujo de formato DWG",
   u8"Guardar dibujo en formato DWG",
   u8"Importar bloque DWG",
   u8"Cargar fuente de escritorio",
   u8"Cargar nueva fuente",
   u8"Guardar archivo de resultados del análisis estático",
};

#define nameplate_tip u8"@Placa de identificación del dibujo"
#define  edit_table u8"Editar tabla"

#define _BLOCK_DESCRIPTION_ u8"Descripción del bloque:"
#define _BLOCK_TYPE u8"Tipo de bloque:"

#define _Yes_ u8"Sí"
#define _No_ u8"No"

#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'

#define _FILE_EXISTS_ u8"El archivo existe"
#define _OVERWRITE_IT_ "¿Sobrescribirlo? "

#endif // __LOADF__

#ifdef __INFO__
/*
char* typ_punktu_tab[] = { u8"Sencillo",u8"Punto base",u8"Pin","?","?","?","?","?","?",
						"?","?","?","?","?","?","?"};
*/
char* vector_style_tab[] = { u8"rígido-rígido",u8"rígido-con bisagras",u8"con bisagras-rígido",u8"con bisagras-con bisagras",
                            u8"Fuerza",u8"Momento+",u8"Momento-",
                            u8"Desplazamiento", u8"Rotación+", u8"Rotación-",
                            u8"carga vertical trapezoidal",
                            u8"carga horizontal trapezoidal",
                            u8"carga perpendicular trapezoidal",
                            u8"proyección horizontal de una carga vertical",
                            u8"proyección vertical de una carga horizontal",
                            u8"carga térmica",
                            u8"tamaño del nodo (radio)",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            };

char* view_width_tab1[] = { u8"muy delgada", u8"fina", u8"gruesa", u8"muy gruesa", u8"extra gruesa", u8"relleno", u8"", u8"invisible" };

char* tab_typ_tekstu[] = { u8"Normal", u8"Etiqueta interna",u8"Etiqueta de componente",u8"Símbolo",u8"Tipo",
u8"*nombre pin",u8"fuente de alimentación",u8"Puerto",u8"Atributo",u8"+Esquema",u8"Nombre de archivo",u8"red",
u8"Comentario",u8"%variable",u8"aUx.símbolo",u8"auX.descripción" };

char* tab_justowanie[] = { u8"a la izquierda |<",u8"a la derecha >|",u8"Medio ><",u8"Central _><_" };

static POLE pmInfoAbout[] = {
	 {u8"tipo de Objeto\0",'O',320,NULL},      //0
	 {u8"cApa\0",'A',305,NULL},      //1
	 {u8"֎Color\0 ",'C',158,NULL},      //2
	 {u8"Tipo de línea\0 ",'T',160,NULL},      //3
	 {u8"ancho de Línea\0",'L',159,NULL},      //4
     {u8"tipo de Punto\0",'P',27,NULL},       //5
     {u8"tipe de vector\0 ",'V',770,NULL},      //6
	 {u8"X1 \0",'1',306,NULL},      //7
	 {u8"Y1 \0",'2',307,NULL},      //8
	 {u8"X2 \0",'3',308,NULL},      //9
	 {u8"Y2 \0",'4',309,NULL},      //10
	 {u8"X3 \0",'5',310,NULL},      //11
	 {u8"Y3 \0",'6',311,NULL},      //12
	 {u8"X4 \0",'7',312,NULL},      //13
	 {u8"Y4 \0",'8',313,NULL},      //14
	 {u8"Radio \0 ",'R',212,NULL},      //15
     {u8"Radio Y \0 ",'r',710,NULL},      //16
     {u8"ángulo \0",'@',107,NULL},      //17
	 {u8"ángulo 1 \0",'<',314,NULL},      //18
	 {u8"ángulo 2 \0",'>',315,NULL},      //19

     {u8"Ancho inicial \0 ",'-',249,NULL},      //20
     {u8"Ancho final \0 ",'=',249,NULL},      //21
     {u8"Intensidad \0 ",'^',727,NULL},      //22
     {u8"Intensidad inicial \0 ",'(',766,NULL},      //23
     {u8"Intensidad final \0 ",')',767,NULL},      //24
     {u8"Desplazamiento inicial del eje \0",'/',250,NULL},      //25
     {u8"Desplazamiento final del eje \0",'\\',250,NULL},      //26

     {u8"Cargar carácter y variante\0 ",'?',798,NULL},     //27
     //{u8"Variante de carga \0 ",'?',799,NULL},     //28

     {u8"Opacidad\0",'%',542,NULL},      //29
	 {u8"longitud / perímetro\0",'D',210,NULL},      //30
	 {u8"DX\0",'X',316,NULL},      //31
	 {u8"DY\0",'Y',317,NULL},      //32
	 {u8"área de Superficie\0",'S',7,NULL},        //33
	 {u8"Fuente\0",'F',111,NULL},      //34
	 {u8"tipo de tExto\0 ",'E',231,NULL},      //35
	 {u8"ocUlto\0",'U',232,NULL},      //36
	 {u8"alineación\0",'J',233,NULL},      //37
	 {u8"Altura \0",'H',179,NULL},      //38
	 {u8"Factor de ancho \0 ",'*',230,NULL},      //39
	 {u8"cursIva\0 ",'I',181,NULL},      //40
     {u8"Negrita\0 ",'N',182,NULL},      //41
	 {u8"suBrayada\0 ",'B',409,NULL},      //42
	 {u8"espaciado entre líneas\0",'#',410,NULL},      //43
     {u8"tamaño de píxel dx \0",'Q',689,NULL},      //44
     {u8"tamaño de píxel dy \0",'Z',690,NULL},      //45
	 {u8"Nombre del bloque interno\0",'N',318,NULL},      //46
	 {u8"nombre del Bloque exterior\0",'B',319,NULL},      //47
};
TMENU mInfoAbout = { 47,0,0,40,2, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmInfoAbout, NULL, NULL };

static POLE pmInfo[] = {
	 {u8"Tamaño de memoria para dibujo / predeterminado[KB] \0 ",'T',297,NULL},
	 {u8"memoria Virtual de la imagen/usado [MB] \0 ",'V',298,NULL},
	 {u8"tamaño de datos de Dibujo [ B] \0 ",'D',299,NULL},
	 {u8"búfer de Impresión[KB] \0 ",'I',300,NULL},
	 {u8"búfer de Macros [B] \0 ",'M',301,NULL},
	 {u8"Búfer de imagen/segmento de pantalla virtual [KB]\0 ",'B',302,NULL},
	 {u8"carpeta Actual \0 ",'A',303,NULL},
	 {u8"modo Gráfico \0 ",'G',304,NULL},
	 {u8"Fuente de escritorio \0 ",'F',111,NULL},
     {u8"Logo \0 ",'L',812,NULL},
};

TMENU mInfo = { 10,0,0,64,1, 3, ICONS | TADD, CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmInfo, NULL, NULL, NULL };

char* objects[] = { u8"Línea", u8"Línea 3D", u8"Texto", u8"Arco", u8"Círculo", u8"Disco", u8"Polígono/Sólido", u8"Sólido 3D", u8"Punto", u8"Spline", u8"Imagen",u8"Polilínea",u8"Rastro",u8"Sombreado", u8"Arco elíptico",u8"Elipse", u8"Disco elíptico", u8"Arco sólido", u8"Vector"};

#define _FILLING_ u8"relleno"

char *vector_txt[]={u8"Vector: rígido-rígido",u8"Vector: rígido-pasador",u8"Vector: pasador-rígido",u8"Vector: pasador-pasador",u8"Vector: Fuerza",u8"Vector: Momento" ,u8"Vector: -Momento",
 u8"Vector: Desplazamiento",u8"Vector: Rotación",u8"Vector: -Rotación",u8"Vector: carga trapecio Y",u8"Vector: carga trapecio X",u8"Vector: carga trapecio N",u8"Vector: carga trapecio H",u8"Vector: carga trapecio V",u8"Vector: carga térmica", u8"Vector: tamaño del nodo (Radio)"};

//char *point_txt[]={u8"Simple",u8"punto Base",'','','','','',u8"Unión",u8"Pasador",'','','',u8"apoyo Empotrado",u8"apoyo empotrado I",u8"apoyo empotrado D",u8"apoyo empotrado A",u8"apoyo Articulado fijo",
// u8"apoyo articulado fijo I",u8"apoyo articulado fijo D",u8"apoyo articulado fijo A",u8"apoyo Deslizadera horizontalmente",u8"apoyo deslizadera verticalmente I",u8"apoyo deslizadera verticalmente D",
 //u8"apoyo deslizadera horizontalmente A",u8"apoyo articulado deslizadera horizontalmente",u8"apoyo articulado deslizadera verticalmente I",u8"apoyo articulado deslizadera verticalmente D",u8"apoyo articulado deslizadera horizontalmente A"};

unsigned short vector_wcod[]={L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', L'Y', L'X', L'N', L'V', L'H', L'Q', L'R'};
unsigned short point_wcod[]={L'S', L'B', ' ', ' ', ' ', ' ',' ', L'U', L'P', ' ', ' ', ' ',L'E', L'1', L'2', L'3', L'A', L'3', L'4', L'5', L'D', L'7', L'8', L'9', L'O', '0', '-', '=', '+'};
unsigned short object_wcod[]={L'I', L'L', L'T', L'A', L'C', L'D', L'S', L'P', L'*', L'G', L'O', L'E', L'F', 'B', L'V', ' '};
char *object_txt[]={u8"Imagen",u8"Línea",u8"Texto",u8"Arco",u8"Círculo",u8"Disco",u8"Sólido",u8"Punto",u8"Bloque",u8"Arco sólido",u8"Arco elíptico",u8"Elipse",u8"Elipse rellena",u8"Bézier Spline",u8"Vector",""};

#endif

#ifdef __ALLEGROEXT__

static char* desktop_data_param[] =
{
   (char*)u8"Fondo de pantalla",
   (char*)u8"Panorama",
   (char*)u8"Menú dinámico",
   (char*)u8"Cursor de escritorio",
   (char*)u8"Estilo de menú",
   (char*)u8"Instrucción",
};

static int no_desktop_data_param = sizeof(desktop_data_param) / sizeof(desktop_data_param[0]);

static char* desktop_data_param_comment[] =
{
	";",
	//";Screen saver delay in min or 0 (off)",
	(char*)u8";factor de panoramización automática de escritorio",
	";",
	(char*)u8"; pequeño 0, grande 1",
    (char*)u8"; barra 0, puntero 1",
    "; mostrando instrucciones breves sobre el inicio",
};


#define _UNABLE_INSTALL_GRAPHIC_ u8"No se puede inicializar el modo de gráficos"
#define _ERROR_READING_DAT_ u8"Error al leer el archivo: alfacad.dat"

#define _MACRO_C__ L'a'
#define _EXECUTE_C__ L'e'
#define _Go_AHEAD_ u8"OK. ¡Adelante!"

#define _UPGRADE_READY_ u8"La actualización está lista para descargarse e instalarse"
#define _UPGRADE_RESTART_ u8"¿Quieres actualizar y reiniciar ahora?"
#define _TOOLS_TO_INSTALL_ u8"No instalado"
#define _INSTALL_TOOLS_ u8"Instalar los paquetes necesarios"

char _Yes_[]="Si";
char _No_[]="No";
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'

#endif

#ifdef __O_LINE__

static POLE pmAxis1000[] = {
	      {u8"Longitud del eje\0      \0",L'L',267,NULL} };

static POLE pmAxis50[] = {
		  {u8"Longitud del eje\0      \0",L'L',267,NULL} };

static POLE pmLine[] = {
		  {u8"Cerrar\0",L'C',217,NULL},
		  {u8"Eliminar\0Del\0",L'E',218,NULL},
		  {u8"Longitud\0             0\0",L'L',219,NULL} };

static POLE pmPLine[] = {
		  {u8"Cerrar\0",L'C',217,NULL},
		  {u8"Eliminar\0Del\0",L'E',218,NULL},
		  {u8"Longitud\0 0\0",L'L',219,NULL},
		  {u8"Arco\0", L'A', 15,NULL} };

static POLE pmLine_Con[] = {
		  {u8"Continuación",L'C',220,NULL} };

static POLE pmPLineObrys[] = {
		  {u8"Cerrar\0",L'C',217,NULL},
		  {u8"Eliminar\0Del\0",L'E',218,NULL},
		  {u8"Longitud\0 0\0",L'L',219,NULL},
		  {u8"Arco\0\0", L'A', 15,NULL},
		  {u8"Punteada\0\0",L'P',268,NULL},
		  {u8"Discontinua\0\0",L'D',269,NULL} };

static POLE pmLine_ConObrys[] = {
		  {u8"Continuar",L'C',217,NULL},
		  {u8"Punteada\0\0",L'P',268,NULL},
		  {u8"Discontinua\0\0",L'D',269,NULL} };

#define _AXIS_ u8"Axias"
#define _AXIS_C_ L'A'
#define _AXIS_H_ u8"Axias #"
#define _AXIS_H_C_ L'A'
#define _LINE_ u8"Línea"
#define _LINE_C_ L'L'

#define _LINE_3D_ u8"Línea3D"

#define _POLYLINE_ u8"Polilínea"
#define _POLYLINE_C_ L'P'

#define _POLYLINE_3D_ u8"Polilínea3D"
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
TMENU mLoadVariant = { 35, 0,0, 3, 88, 12, 0, CMNU,CMBR,CMTX,0, 30, 0, 0,0,(POLE(*)[]) &pmLoadVariant, NULL, NULL };

POLE pmLoad_Char[] = {
	{u8"ninguno",L'0',797,NULL},
	{u8"carga Muerta",L'M',787,&mLoadVariant},
    {u8"Carga viva",L'C',788,&mLoadVariant},
    {u8"carga de Mantenimiento",L'M',789,&mLoadVariant},
    {u8"carga de Viento",L'V',790,&mLoadVariant},
    {u8"carga de Nieve",L'N',791,&mLoadVariant},
    {u8"carga Sísmica",L'S',792,&mLoadVariant},
    {u8"carga de hielo o Agua de lluvia",L'A',793,&mLoadVariant},
    {u8"carga Hidráulica del suelo",L'H',794,&mLoadVariant},
    {u8"carga hidráulica de Fluido",L'F',795,&mLoadVariant},
    //{u8"carga Térmica",L'T',796,&mLoadVariant},
};

//static
TMENU mLoad_Char = { 10, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 18, 0, 0,0,(POLE(*)[]) &pmLoad_Char, NULL, NULL };

POLE pmLoad_Char_Thermal[] = {
	{u8"ninguno",L'0',797,NULL},
    {u8"carga Térmica",L'T',796,&mLoadVariant},
};

//static
TMENU mLoad_Char_Thermal = { 2, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 18, 0, 0,0,(POLE(*)[]) &pmLoad_Char_Thermal, NULL, NULL };

static POLE pmVector[] = {
		  {u8"Eliminar\0Del\0",L'E',218,NULL},
          {u8"rígido - rígido\0\0",L'A',723,NULL},
          {u8"rígido - pasador\0\0",L'B',724,NULL},
          {u8"pasador - rígido\0\0",L'C',725,NULL},
          {u8"pasador - pasador\0\0",L'D',726,NULL},
          {u8"Fuerza\0\0",L'F',727,&mLoad_Char},
          {u8"Momento\0\0",L'M',728,&mLoad_Char},
          {u8"-momento\0\0",L'-',729,&mLoad_Char},
          {u8"desplazamiento\0\0",L'Z',730,NULL}, //&mLoad_Char}, //NULL
          {u8"Rotación\0\0",L'R',731,NULL}, //&mLoad_Char},  //NULL
          {u8"-rotación\0\0",L'O',732,NULL}, //&mLoad_Char},  //NULL
          {u8"carga trapezoidal Y\0\0",L'Y',733,&mLoad_Char},
          {u8"carga trapezoidal X\0\0",L'X',734,&mLoad_Char},
          {u8"carga trapezoidal N\0\0",L'N',735,&mLoad_Char},
          {u8"carga trapezoidal H\0\0",L'H',736,&mLoad_Char},
          {u8"carga trapezoidal V\0\0",L'V',737,&mLoad_Char},
          {u8"carga Térmica\0\0",L'T',752,&mLoad_Char_Thermal},
          {u8"tamaño del nodo (radio)\0\0",L'0',786,NULL},
};

static POLE pmVector_Con[] = {
		  {u8"Continuation",L'C',220,NULL} };

static POLE pmMember_style[] = {
          {u8"rígido - rígido\0\0",L'A',723,NULL},
          {u8"rígido - pasador\0\0",L'B',724,NULL},
          {u8"pasador - rígido\0\0",L'C',725,NULL},
          {u8"pasador - pasador\0\0",L'D',726,NULL},
};

static POLE pmForce_Displacement_style[] = {
          {u8"Fuerza\0\0",L'F',727,NULL},
          {u8"desplazamiento\0\0",L'Z',730,NULL},
};

static POLE pmMoment_Rotation_style[] = {
          {u8"Momento\0\0",L'M',728,NULL},
          {u8"-momento\0\0",L'-',729,NULL},
          {u8"Rotación\0\0",L'R',731,NULL},
          {u8"-rotación\0\0",L'O',732,NULL},
};

static POLE pmLoad_style[] = {
          {u8"carga trapezoidal Y\0\0",L'Y',733,NULL},
          {u8"carga trapezoidal X\0\0",L'X',734,NULL},
          {u8"carga trapezoidal N\0\0",L'N',735,NULL},
          {u8"carga trapezoidal H\0\0",L'H',736,NULL},
          {u8"carga trapezoidal V\0\0",L'V',737,NULL},
};

#define _AXIS_ u8"Axias"
#define _AXIS_C_ L'A'
#define _AXIS_H_ u8"Axias #"
#define _AXIS_H_C_ L'A'
#define _VECTOR_ u8"Vector"
#define _VECTOR_C_ L'V'

#endif

#ifdef __O_LUK__

#define _YES_ "S"
#define _NO_ "N"

static TMENU mArcReversed = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0,14,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmLukm[] = {
		{u8"[1] tres puntos", L'1', 221, NULL},
		{u8"[2] inicio-medio-final", L'2', 222, NULL},
		{u8"[3] inicio-centro-ángulo", L'3' , 223, NULL},
		{u8"[4] inicio-medio-acorde", L'4', 224, NULL},
		{u8"[5] inicio-final-radio", L'5', 225, NULL},
		{u8"[6] inicio-final-ángulo", L'6', 226, NULL},
		{u8"[7] inicio-final-dirección", L'7', 227, NULL},
		{u8"[8] continuación", L'8', 228, NULL},
        {u8"dirección del arco invertida",L'0',729,&mArcReversed}};

static POLE pmPLukm[] = {
		{u8"[1] tres puntos", L'1', 221, NULL},
		{u8"[2] inicio-medio-final", L'2', 222, NULL},
		{u8"[3] inicio-centro-ángulo", L'3', 223, NULL},
		{u8"[4] inicio-medio-acorde", L'4', 224, NULL},
		{u8"[5] inicio-final-radio", L'5', 225, NULL},
		{u8"[6] inicio-final-ángulo", L'6', 226, NULL},
		{u8"[7] inicio-final-dirección", L'7', 227, NULL},
		{u8"[8] continuación", L'8', 228, NULL},
        {u8"dirección del arco invertida",L'0',729,&mArcReversed},
		{u8"Cerrar",L'C',247,NULL},
		{u8"Remover: Del",L'R',248,NULL},
		{u8"Línea", L'L',13,NULL} };

static POLE pmPLukmObrys[] = {

		{u8"[1] tres puntos", L'1', 221, NULL},
		{u8"[2] inicio-medio-final", L'2', 222, NULL},
		{u8"[3] inicio-centro-ángulo", L'3', 223, NULL},
		{u8"[4] inicio-medio-acorde", L'4', 224, NULL},
		{u8"[5] inicio-final-radio", L'5', 225, NULL},
		{u8"[6] inicio-final-ángulo", L'6', 226, NULL},
		{u8"[7] inicio-final-dirección", L'7', 227, NULL},
		{u8"[8] continuación", L'8', 228, NULL},
        {u8"dirección del arco invertida",L'0',729,&mArcReversed},
		{u8"Cerrar\0",L'C',247,NULL},
		{u8"Remove\0Del\0",L'R',248,NULL},
		{u8"Línea\0", L'L',13,NULL},
		{u8"Punteada\0    \0",L'P',268,NULL},
		{u8"Discontinua\0    \0",L'D',269,NULL}};

#define _ARC_ u8"Arco"
#define _ARC_C_ L'A'

#endif

#ifdef __O_OKRAG__

static const char CIRCLE[] = u8"Circulo";
static const char CIRCLE_FILL[] = u8"Disco";

static POLE pmCircle[] = {
	 {u8"Centro-radio\0",L'C',213,NULL},
	 {u8"centro-Diámetro\0",L'D',214,NULL},
	 {u8"2 puntos\0",L'2',215,NULL},
	 {u8"3 puntos\0",L'3',216,NULL},
	 {u8"Tangencial\0",L'T',266,NULL},
};

#define _CIRCLE_C_ L'C'
#define _DISC_C_ L'D'

#endif

#ifdef __O_POLYGN__

static POLE pmPolygon[] = {
	 {u8"Arista\0 \0",L'A',235,NULL},
	 {u8"Inscrito\0 \0", L'I',237, NULL},
	 {u8"Circunscrito\0  \0",L'C',236, NULL},
	 {u8"Número de aristas\0    \0",L'N',606,NULL},
};

#define _POLYGON_ u8"políGono"
#define _POLYGON_C_ L'G'

#endif

#ifdef __O_ELLIPS__

static POLE pm_ell__draw_type[] = {
		{u8"inicio de Eje", L'E', 240, NULL},
        {u8"Centro", L'C', 241, NULL},
};

static TMENU m_ell__draw_type = { 2,0,0,13,79,7,ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pm_ell__draw_type,NULL,NULL };

static POLE pm_ell_y_draw_method[] = {
		{u8"Distancia",L'D',711,NULL},
		{u8"Punto",L'P',712,NULL},
        {u8"Tangencial",L'T',713,NULL},
};

static TMENU m_ell_y_draw_method = { 3,0,0,13,79,7,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pm_ell_y_draw_method,NULL,NULL };


POLE pm__ellipse[] = {
		//{u8"Aproximación\0 20\0  ",L'A',238, NULL},
        {u8"el Método del segundo semieje\0 Distancia",L'M',710, &m_ell_y_draw_method},
		{u8"inicio del eje/Centro\0 inicio del eje",L'C',239, &m_ell__draw_type},
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
		//{u8"Aproximación\0 20\0  ",L'A',238, NULL},
        {u8"el Método del segundo semieje\0  Distancia",L'M',710, &m_ell_y_draw_method},
		{u8"Opacidad\0",'O',542,&mTranslucencyFE},
					};

#define _ELLIPSE_ u8"Elipse"
#define _ELLIPSE_C_ L'E'

POLE pm__ellipticalarc[] = {
		{u8"ángulo en coordenadas Locales",L'L',708, NULL},
		{u8"ángulo en coordenadas Globales",L'G',709, NULL},
					};

#define _ELLIPSE_ANGLE_ u8"Ángulo"
#define _ELLIPSE_ANGLE_C_ L'A'

#define _ELLIPTICALDISC_ u8"Disco"
#define _ELLIPTICALDISC_C_ L'D'

static char* EllipticalAngle[] = {u8"Local",u8"Global"};

#endif

#ifdef __O_PLINE__

#endif

#ifdef __O_SOLID__

static char filltyp_tab[7][5] = { u8"R","md","f","G","mG","eG","P" };

static POLE pmS34[] = {
		{u8"Triangular",'T',246,NULL},
		{u8"Cuadrangular",'C',245,NULL},
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
		{u8"Patrón\0 ",'P',597,NULL},
		{u8"Escala\0 1.0",'E',436,&mPatternScale},
		{u8"Rotar\0 0",'R',35,&mPatternAngle},
		{u8"dX\0 0",'X',622,&mPatternDx},
		{u8"dY\0 0",'Y',623,&mPatternDy},
		{u8"Toma el patrón\0",'T',164,NULL},
		{u8"Cambiar patrón existente\0",'C',599,NULL},
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
	   {u8"Relleno\0",'R',276,&mTranslucency},
	   {u8"muy Delgada\0",'D',277,NULL},
	   {u8"Fina\0",'F',278,NULL},
	   {u8"Gruesa\0",'G',279,NULL},
	   {u8"Muy gruesa\0",'M',280,NULL},
	   {u8"Extra gruesa\0",'E',281,NULL},
	   {u8"Patrón\0",'P',617, &mSolidPattern},
	   //  {u8"- área deshabilitada",'-',0,NULL},
	   //  {u8"+ área activa",'+',0,NULL},
};

static TMENU mFillTyp = { 7,0,0,15,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,5,0,0,0,(POLE(*)[]) &pmFillTyp,NULL,NULL };

POLE pmSolid[] = {
		{u8"Cuadrangular/triangular\0 4",'C',242,&mS34},
		{u8"Triangular\0",'T',243,NULL},
		{u8"Relleno/tipo de línea \0 R\0 ",'R',244,&mFillTyp},
};

#define _SOLID_ u8"Sólido"
#define _SOLID_3D_ u8"Sólido 3D"
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
		{u8"curva de Bézier cuadrática",'Q',520,NULL},
		{u8"curva de Bézier Cúbica",'C',519,NULL},
		{u8"spline cardinal Multipunto",'M',646,NULL},
		{u8"Bézier Spline",'S',662,NULL}
};

static TMENU mS34 = { 4,0,0,11,79,6,ICONS,CMNU,CMBR,CMTX,0,8,0,0,0,(POLE(*)[]) &pmS34,NULL,NULL };

static TMENU mD_T_Pline = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmSpline[] = {
		{u8"Cuadrática/cúbica/multipunto\0 C\0",'Q',517,&mS34},
		{u8"cuadrático Ahora\0\0",'A',518,NULL},
		{u8"Polilínea\0 Y\0",'P',251,&mD_T_Pline},
		{u8"Remove\0Del\0",'R',218,NULL},
		{u8"continuación\0\0",'+',228,NULL},
		{u8"curva final\0End\0",'F',649,NULL},
		{u8"Tensión\0\0",'T',648,NULL},
		{u8"Cerrar curva multipunto Home\0 N\0",'C',647,NULL},
};

#define _SPLINE_ u8"Spline"
#define _SPLINE_C_ L'S'

#define _YES__ u8"S"
#define _NO__ u8"N"

#endif

#ifdef __O_TRACE__

extern TMENU mPatternScale;
extern TMENU mPatternAngle;
extern TMENU mPatternDx;
extern TMENU mPatternDy;
extern TMENU mTranslucency;

static char tracefilltyp_tab[7][5] = { u8"R","md","f","G","MG","EG","P" };

static TMENU mD_T_Pline = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmTracePattern[] = {
		{u8"Patrón\0 ",'P',597,NULL},
		{u8"Escala\0 1.0",'E',436,&mPatternScale},
		{u8"Rotar\0 0",'R',35,&mPatternAngle},
		{u8"dX\0 0",'X',622,&mPatternDx},
		{u8"dY\0 0",'Y',623,&mPatternDy},
		{u8"Tomar patrón\0",'T',164,NULL},
		{u8"Cambiar patrón existente\0",'C',599,NULL},
};

static TMENU mTracePattern = { 7,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTracePattern,NULL,NULL };


static POLE pmFillTyp[] = {
	   {u8"Relleno\0",'R',276,&mTranslucency},
	   {u8"muy Delgada\0",'D',277,NULL},
	   {u8"Fina\0",'F',278,NULL},
	   {u8"Gruesa\0",'G',279,NULL},
	   {u8"Muy gruesa\0",'M',280,NULL},
	   {u8"Extra gruesa\0",'E',281,NULL},
	   {u8"Patrón\0",'P',617, &mTracePattern},
};

static TMENU mFillTyp = { 7,0,0,15,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,9,0,0,0,(POLE(*)[]) &pmFillTyp,NULL,NULL };

#define _YES_ "S"
#define _NO_ "N"

static TMENU mTraceArcReversed = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0,11,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmPTLukm[] = {
        {u8"[0] continuación con arco", L'0', 714, NULL},
        {u8"[1] tres puntos", L'1', 221, NULL},
		{u8"[2] inicio-medio-final", L'2', 222, NULL},
		{u8"[3] inicio-centro-ángulo", L'3', 223, NULL},
		{u8"[4] inicio-medio-acorde", L'4', 224, NULL},
		{u8"[5] inicio-final-radio", L'5', 225, NULL},
		{u8"[6] inicio-final-ángulo", L'6', 226, NULL},
		{u8"[7] inicio-final-dirección", L'7', 227, NULL},
		//{u8"[8] continuación", L'8', 228, NULL},
        {u8"dirección del arco invertida",L'0',729,&mTraceArcReversed},
		//{u8"Cerrar",L'C',247,NULL},
		//{u8"Remover: Del",L'R',248,NULL},
		{u8"Línea", L'L',13,NULL},
        {u8"Continuación con línea", L'C',715,NULL},
};

static TMENU mPTLukm={11, 0, 0, 30, 56, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,75,0,0,0,&pmPTLukm,NULL,NULL};


static POLE pmFTrace[] = {
	{u8"Ancho\0 1\0        ",'A',249,NULL},
	{u8"Desplazamiento del eje\0 1\0        ",'D',250,NULL},
	{u8"Polilínea\0 S\0        ",'P',251,&mD_T_Pline},
	{u8"relleno/ancho de Línea\0 R\0     ",'L',252,&mFillTyp},
	{u8"Remove\0Del\0 ",'R',253,NULL},
	{u8"Suspender\0Home\0 ",'S',633,NULL},
	{u8"Cerrar\0End\0 ",'C',632,NULL},
    {u8"Arco\0\0 ",L'A',15,&mPTLukm},
};

static POLE pmMLine[] = {
	{u8"Ancho\0 1\0        ",'A',249,NULL},
	{u8"Desplazamiento del eje\0 1\0        ",'D',250,NULL},
	{u8"Polilínea\0 S\0        ",'P',251,&mD_T_Pline},
	{u8"Remove\0Del\0 ",'R',253,NULL},
	{u8"Suspender\0Home\0 ",'S',633,NULL},
	{u8"Cerrar\0End\0 ",'C',632,NULL},
    {u8"Arco\0\0 ",L'A',15,&mPTLukm},
};

#define _TRACE_ u8"Trazo"
#define _TRACE_C_ L'T'

#define _DLINE_ u8"2 líneas"
#define _DLINE_C_ L'2'

#define _TLINE_ u8"3 líneas"
#define _TLINE_C_ L'3'

#define _YES__ "S"
#define _NO__ "N"

#define _BREAK_ u8"Romper"
#endif

#ifdef __O_SKETCH__

static POLE pmSketch[] = {
	{u8"Longitud del segmento\0   1mm",L'L',531, NULL},
	{u8"Remover último\0Del\0 ",L'R',532, NULL},
};

#define _SKETCH_ u8"Boceto"
#define _SKETCH_C_ L'B'

#define _MM_ u8"mm"

#endif

#ifdef __O_POINT__

#define _POINT_TYPE_ u8"Tipo de punto"
#define _POINT_TYPE_C_ L'T'

static POLE pmTyp_punktu[] = {
	 {u8"Simple\0",L'S',294,NULL},
	 {u8"punto Base\0",L'B',295,NULL},
	 {u8"Pasador\0",L'P',296,NULL},
	 {u8"Unión\0",L'U',529,NULL},
     {u8"apoyo Empotrado\0",L'E',738,NULL},
     {u8"apoyo empotrado I\0",L'1',739,NULL},
     {u8"apoyo empotrado D\0",L'2',740,NULL},
     {u8"apoyo empotrado A\0",L'3',754,NULL},
     {u8"apoyo Articulado fijo\0",L'A',741,NULL},
     {u8"apoyo articulado fijo I\0",L'4',742,NULL},
     {u8"apoyo articulado fijo D\0",L'5',743,NULL},
     {u8"apoyo articulado fijo A\0",L'6',755,NULL},
     {u8"apoyo Deslizadera horizontalmente\0",L'D',744,NULL},
     {u8"apoyo deslizadera verticalmente I\0",L'7',745,NULL},
     {u8"apoyo deslizadera verticalmente D\0",L'8',746,NULL},
     {u8"apoyo deslizadera horizontalmente A\0",L'9',756,NULL},
     {u8"apoyo articulado deslizadera horizontalmente\0",L'O',747,NULL},
     {u8"apoyo articulado deslizadera verticalmente I\0",L'0',748,NULL},
     {u8"apoyo articulado deslizadera verticalmente D\0",L'-',749,NULL},
     {u8"apoyo articulado deslizadera horizontalmente A\0",L'=',757,NULL},
     {u8"sin rotación Z\0",L'+',818,NULL},
};

#endif

#ifdef __O_GRAPHF__

#define MAX_GRAPHFNO 16
#define MAX_GRAPHF_NAME_LEN 32
#define MAX_GRAPHF_FORMULA_LEN  (MAX_GRAPHFNO * MaxTextLen)    //4096

#define GRAPHF "Gráfico"
#define _GRAPHF_ "Gráfico"
#define _GRAPHF_C_ L'G'

static POLE pmGraphf[MAX_GRAPHFNO + 1];

static TMENU mGraphf = { 16,0,0,MAX_GRAPHF_NAME_LEN + 1,70,7,0,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmGraphf,NULL,NULL };

static POLE pmGraph[] = {
		  {u8"Paso dx\0       ",L'S',254,NULL},
		  {u8"Función\0",L'F',255, &mGraphf} };

#define _CURRENT_ u8"Actual"

#endif

#ifdef __B_MENU__

#define _ENDPOINT_ L'F'
#define _endpoint_ L'f'
#define _CTRL_ENDPOINT_ 1926 //6
#define _NEAREST_ L'E'
#define _nearest_ L'e'
#define _CTRL_NEAREST_ 1925 //5
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

#define _CTRL_0_ 0
#define _CTRL_1_ _CTRL_ADJACENT_
#define _CTRL_2_ 2
#define _CTRL_3_ _CTRL_CENTER_
#define _CTRL_4_ 4
#define _CTRL_5_ _CTRL_ENDPOINT_
#define _CTRL_6_ 6
#define _CTRL_7_ 7
#define _CTRL_8_ 8
#define _CTRL_9_ _CTRL_INTERSECTION_
#define _CTRL_10_ 10
#define _CTRL_11_ 11
#define _CTRL_12_ 12
#define _CTRL_13_ _CTRL_MIDDLE_
#define _CTRL_14_ _CTRL_NEAREST_
#define _CTRL_15_ _CTRL_POINT_
#define _CTRL_16_ _CTRL_PERPENDICULAR_
#define _CTRL_17_ 17
#define _CTRL_18_ 18
#define _CTRL_19_ 19
#define _CTRL_20_ _CTRL_TANGENTIAL_
#define _CTRL_21_ 21
#define _CTRL_22_ 22
#define _CTRL_23_ 23
#define _CTRL_24_ _CTRL_NOPE_
#define _CTRL_25_ 25
#define _CTRL_26_ 26
#define _CTRL_27_ 27
#define _CTRL_28_ 28
#define _CTRL_29_ 29
#define _CTRL_30_ 30
#define _CTRL_31_ 31

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


#define HATCH_PATTERN u8"PATRONES"
#define PATTERNS_INI u8"PATRONES.INI"

#define PATTERN_NAMES_MAXNO 255

static PPOLE pm__list_hatch[PATTERN_NAMES_MAXNO];

static POLE pm__list_hatch_predef[] = {
		{u8"֎SÓLIDO\0",'S',0,&mHTranslucency},
		{u8"////////\0",'/',0,NULL},
		{u8"XXXXXXXX\0",'X',0,NULL} };

POLE pmKatH[] = {
		{u8"Ángulo",'A',108,NULL },
		{u8"paRalelo a",'R',580,NULL},
		{u8"Perpendicular a",'P',581,NULL},
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
		{u8"Patrón\0 ",'P',597,NULL},
		{u8"Escala\0 1.0",'E',436,&mSolidHatchPatternScale},
		{u8"Rotar\0 0",'R',35,&mSolidHatchPatternAngle},
		{u8"dX\0 0",'X',622,&mSolidHatchPatternDx},
		{u8"dY\0 0",'Y',623,&mSolidHatchPatternDy},
		{u8"Tomar patrón\0",'T',164,NULL},
};

static TMENU mSolidHatchPattern = { 6,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,ID_IMAGE_PATTERN,0,0,0,(POLE(*)[]) & pmSolidHatchPattern,NULL,NULL };

POLE pmSelect_Ins[] = {
	{u8"Sencillo",'S',680,NULL},
	{u8"Complejo",'C',681,NULL},
};

TMENU mSelect_Ins = { 2, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, ID_INSULATION, 0, 0,0,(POLE(*)[]) &pmSelect_Ins, NULL, NULL };

POLE pmHatch[] = {
	 {u8"Patrón\0//////////\0      ",'P',434,&m__list_hatch},
	 {u8"Carpeta de patrones\0 \0      ",'C',597,NULL},
	 {u8"ángulo de Rotación\0 0\0      ", 'R', 435,&mKatH},
	 {u8"Escala\0 1\0     ", 'E', 436,NULL},
	 {u8"punto Base\0 0,0\0              ", 'B', 437,NULL},
	 {u8"Mostrar punto base\0 \0", 'M', 438,NULL},
	 {u8"Distancia de la línea del patrón/longitud del segmento spline\0 1\0             ", 'D', 439,NULL},
	 {u8"aislamiento Térmico\0 \0", 'T', 679, &mSelect_Ins},
	 {u8"patrón de Imagen\0 \0",'I',617,&mSolidHatchPattern}
	  };

#define _HATCHING_ u8"Sombreado"
#define _HATCHING_C_ L'S'

#define _SOLID_SUFFIX_ "ES"
#define __IMAGE u8"__IMAGEN"

#endif

#ifdef __O_DXF_IN__

#define Linie_DXF u8"LÍNEAS-DXF"
static char line_width_dxf_param[5][16] =
{
   "muy delgada",
   "fina",
   "gruesa",
   "muy gruesa",
   "extra gruesa",
};

#define _LINE_NO_ u8"fila: "
#define _SYSTEM_MESSAGE_ u8"MENSAJE DEL SISTEMA"
#define _INTERRAPTED_ u8"Interrumpido"

#endif

#ifdef __O_BLOK__

static char	 sz__last_bl[] = u8"último Bloque\0 ";
static char	sz__wszystko[] = u8"Todos\0 ";
static char	sz__wszystkie_warstwy[] = u8"todas las Capas\0 ";
static char	sz__wszystkie_teksty[] = u8"todos los Textos\0 ";
static char	sz__wszystkie_symbole[] = u8"todos los Simbolos\0 ";
static char	 sz__cross_win[] = u8"Cruz/ventana\0  F9";
static char	sz__add_del[] = u8"agregar/Remover\0  F10";
static char	sz_undo_del[] = u8"retorno todos\0 ";

static TMENU mAuto = { 2,0,0,5,72,9,ICONS,CMNU,CMBR,CMTX,0, 7, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAutoc = { 2,0,0,5,72,8,ICONS,CMNU,CMBR,CMTX,0,	5,	0,	0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dq = { 2,0,0,5,72,7,ICONS,CMNU,CMBR,CMTX,0, 4, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dn = { 2,0,0,5,72,9,ICONS,CMNU,CMBR,CMTX,0, 9 /*7*/, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmBlokm[] = {
	 {u8"último Bloque\0 ",'B',256,NULL},
	 {u8"toDos\0 ",'D',257,NULL},
	 {u8"todas Las Capas\0 ",'L',258,NULL},
	 {u8"Cruz/ventana\0  F9", 'V',259,NULL},
	 {u8"agregar/Remover\0  F10" ,'R', 260,NULL},
	 {u8"instante\0 Y", 'I',261,&mAuto},
};

POLE pmScalem[] = {
		{u8"Aproximación\0 20\0  ",'A',238, NULL},
};

POLE pmBlokmc[] = {
	 {u8"Cruz/ventana\0  F9", 'C',259,NULL},
	 {u8"agregar/Remover\0  F10" ,'R',260, NULL},
	 {u8"Instante\0 Y", 'I',261,&mAutoc},
};

POLE pmSelectMode[] = {
	 {u8"Cruz", L'C',259,NULL},
	 {u8"Objeto" , L'O',720, NULL},
};

POLE pmBlock_wez_w[] =
{
  {u8"Cruz/ventana\0  F9",	'C',259,NULL},
  {u8"Instant\0 Y",	'I',261,&mAuto_dq},
};

POLE pmBlock_wez_info[] =
{
  {u8"Toda la polilínea",	'T',687,NULL},
  {u8"Objeto único",	'O',688,NULL},
};

POLE pmBlock_dq[] =
{
  {u8"Cruz/ventana\0  F9",	'C',259,NULL},
  {u8"Instante\0Y",	'I',261,&mAuto_dq},
  {u8"Retorno todos\0 ", 'R',270,NULL},
  {u8"cambiar el Modo\0 ", 'M',271,NULL},
};

POLE pmBlock_dn[] =
{
  {u8"último Bloque\0 ",'B',256,NULL},
  {u8"toDos\0 ",'D',257,NULL},
  {u8"todas Las Capas\0 ",'L',258,NULL},
  {u8"Cruz/ventana\0  F9", 'C',259,NULL},
  {u8"agregar/Remover\0  F10" ,'R', 260,NULL},
  {u8"Instante\0Y", 'I',261,&mAuto_dn},
  {u8"Borrar\0 Del", 'D',32,NULL},
  {u8"Retorno todos\0 ", 'R',270,NULL},
  {u8"cambiar el Modo\0", 'M',271,NULL},
};

POLE pmBlok_ex[] = {
	 {u8"último Bloque\0 ",'B',256,NULL},
	 {u8"toDos\0 ",'D',257,NULL},
	 {u8"todas Las Capas\0 ",'L',258,NULL},
	 {u8"Cruz/ventana\0  F9", 'C',259,NULL},
	 {u8"agregar/Remover\0  F10" ,'R', 260,NULL},
	 {u8"Instante\0 Y", 'I',261,&mAuto},
	 {u8"Explotar\0",'E',66,NULL},
};

POLE pmBlok_chp[] = {
	 {u8"último Bloque\0 ",'B',256,NULL},
	 {u8"toDos\0 ",'D',257,NULL},
	 {u8"todas Las Capas\0 ",'L',258,NULL},
	 {u8"todo de la capa N°\0 ",'N',174,NULL},
	 {u8"todos los Textos\0 ",'T',273,NULL},
	 {u8"Cruz/ventana\0  F9", 'C',259,NULL},
	 {u8"agregar/Remover\0  F10" ,'R', 260,NULL},
	 {u8"Instante\0 Y", 'I',261,&mAuto},
	 {u8"cambiar Propiedades\0",'P',272,NULL},
};

POLE pmBlok_cht[] = {
	 {u8"último Bloque\0 ",'B',256,NULL},
	 {u8"todos los Textos\0 ",'T',273,NULL},
	 {u8"Cruz/ventana\0  F9", 'C',259,NULL},
	 {u8"agregar/Remover\0  F10" ,'R',260, NULL},
	 {u8"Instante\0 Y", 'I',261,&mAuto},
};

POLE pmBlok_chts[] = {
	 {u8"último Bloque\0 ",'B',256,NULL},
	 {u8"todos los Simbolos\0 ",'S',257,NULL},
	 {u8"todos los Textos\0 ",'T',273,NULL},
	 {u8"Cruz/ventana\0  F9", 'C',259,NULL},
	 {u8"agregar/Remover\0  F10" ,'R', 260,NULL},
	 {u8"Instante\0 Y", 'I',261,&mAuto},
};

POLE pmCopy_Divide[] = {
	 {u8"Dividir",'D',50,NULL},
};

static POLE	pmWszystko_Okno[] = {
	 {u8"Todos",'T',257,NULL},
	 {u8"Ventana",'V',274,NULL},
};

char window[16] = u8"ventana";
char  cross[16] = u8"cruz";

char _addto_[16] = u8"agregar";
char  _remove_[16] = u8"retirar";

char Yes[2] = "Y";

#define _DIVIDE_ u8"Dividir"
#define _DIVIDE_C_ L'D'

#define _BLOCK_ u8"Bloque"
#define _BLOCKF_ u8"* Bloque"
#define _BLOCK_C_ L'B'

#define _BLOCK_SELECT_ u8"Modo de selección"
#define _BLOCK_SELECT_C_ L'M'

#define _FROZEN_ u8"helado "
#define _EXPLODE_BLOCKS_ u8"¿Quieres explotar los bloques marcados %s?"
#define _Yes_ "Sí"
#define _No_ "No"

#define _SCALE_ u8"Escala"
#define _SCALE_C_ L'E'

#define _FILE_ u8"Archivo "
#define _FILE_EXISTS_ u8" existe. ¿Sobrescribir? (Sí/No/Esc):"

#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'

#define _SAVED_BLOCK_ u8"< bloque guardado >"

#define _CHANGE_ u8"Cambio"
#define _CHANGE_C_ L'C'

#define _POINT_IN_AREA_ u8"Está bien. Punto en área cerrada."
#define _POINT_NOT_IN_AREA_ u8"Malas noticias. Punto fuera del área cerrada o área no cerrada."

#define _OBJECT_ u8"Objeto"
#define _OBJECT_C_ L'O'

static char _entire_polyline_[32]=u8"Toda la polilínea";
static char _single_object_[32]=u8"Objeto único";

#endif

#ifdef __O_PROF__

POLE pmChamfer[] =
{
	 {u8"Distancia(s)\0       \0",L'D', 316, NULL},
	 {u8"Anular\0     \0",L'A', 459, NULL},
};

POLE pmFillet[] =
{
	 {u8"Radio\0      \0",L'R', 209, NULL},
	 {u8"Anular\0    \0",L'A', 459, NULL},
};

#define _FILLET_ u8"Empalme"
#define _FILLET_C_ L'E'

#define _CHAMFER_ u8"Chaflan"
#define _CHAMFER_C_ L'C'

#endif

#ifdef __O_OFFSET__

#define OFFSET_NORMAL u8"Normal"
#define OFFSET_SMOOTH u8"Suave"

static POLE pmMethod[] = {
	 {u8"Normal",L'N',630,NULL},
	 {u8"Suave",L'S',631,NULL},
};

static TMENU mMethod = { 2, 0,0,19,56,4,ICONS,CMNU,CMBR,CMTX,0,4, 0,0,0,(POLE(*)[]) &pmMethod, NULL,NULL };

static POLE pmOffset[] = {
	 {u8"Punto\0     ",L'P',457,NULL},
	 {u8"Distancia\0     ", L'D', 458,NULL},
	 {u8"Establecer distancia\0     ", L'E', 605,NULL},
	 {u8"Método para polilíneas\0     ", L'M', 629,&mMethod},
};

#define _OFFSET_ u8"Desfase"
#define _OFFSET_C_ L'D'

#endif

#ifdef __O_MIRROR__

POLE pmMirror[] = {
		{u8"Dejar originales\0 Del",L'D',603,NULL},
		{u8"Reemplazar original\0 Del",L'R',604,NULL},
};

char st_del[] = u8"Reemplazar", st_nodel[] = u8"Dejar";

#define _MIRROR_ u8"Espejo"
#define _MIRROR_C_ L'E'

#endif

#ifdef __O_IMPORT__

#define PATTERNS u8"Patrones"
#define __PATTERNS__ u8"Patrones"
#define BACKGROUND u8"Fondo"
#define __BACKGROUND__ u8"Fondo"
char *BLOCKLIST=u8"Lista de bloques";

static POLE	pmObrot[] = {
	 {u8"rOtar por ángulo",'O',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",   '0',578,NULL},
	 {u8"roTar al ángulo",'T',440,NULL},
	 {u8"paRalelo a",'R',580,NULL},
	 {u8"Perpendicular a",'P',581,NULL},
	 {u8"en Ángulo a",'A',582,NULL},
};

static TMENU mObrot = { 13, 0, 0, 16, 66, 8, ICONS, CMNU, CMBR, CMTX, 0, 7, 0,0,0,(POLE(*)[]) & pmObrot, NULL, NULL };

static POLE pmObrot_GEO[] = {
	 {u8"rOtar por ángulo",	'O',440,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8" 315°",'3',447,NULL},
	 {u8" 0°",  '0',578,NULL},
	 {u8"roTar al ángulo",'T',440,NULL},
	 {u8"------",	' ',0,NULL},
	 {u8"kXZ",		'Z',0,NULL},
	 {u8"kYZ",		'Y',0,NULL},
};

static TMENU mObrot_GEO = { 13, 0, 0, 16, 66, 8, ICONS, CMNU, CMBR, CMTX, 0, 6, 0,0,0,(POLE(*)[]) & pmObrot_GEO, NULL, NULL };


static POLE	pmBlok_Imp[] = {
	 {u8"Escala\0",'E',36,NULL},
     {u8"escala Fija\0",'F',695,NULL},
	 {u8"Rotar\0",'R',35,&mObrot},
	 {u8"espejo Y\0",'Y',41,NULL},
	 {u8"espejo X\0",'X',40,NULL},
     {u8"Centro al cursor\0  ",'C',717,NULL},
	 {u8"Dividir\0",'D',50,NULL},
};

static POLE	pmBlok_Divide[] = {
	 {u8"Dividir",'D',50,NULL},
};

static POLE	pmBlok_Imp_GEO[] = {
	 {u8"Escala",'E',36,NULL},
	 {u8"Rotar",'R',35,&mObrot_GEO},
	 {u8"espejo Y",'Y',41,NULL},
	 {u8"espejo X",'X',40,NULL},
	 {u8"Dividir",'D',50,NULL},
};

#define patterns_tip u8"@Seleccionar patrón de sombreado"
#define background_tip u8"@Seleccionar fondo de escritorio"
#define blocklist_tip u8"@Seleccionar bloque de la lista"
#define alpha_sorting_tip u8"Orden alfabético"
#define timelapse_tip u8"Ordenar por tiempo"
#define catalog_tip u8"@Seleccionar bloque"

char catalog[16] = u8"Catálogo";
char background[16] = u8"Fondo";
char blocklist[16] = u8"Bloques";
char patterns[16] = u8"Patrones";

#define _BLOCK_ u8"Bloque"
#define _BLOCK_C_ L'B'

#define _BLOCK_3D_ u8"Bloque 3D"

#define _BLOCK__ u8"Bloque : ";
#define _BLOCK_3D__ u8"Bloque : ";

#define _BACKGROUND_ u8"Fondo : "
#define _SOLID_IMAGE_ u8"Imagen del sólido : "
#define _HATCH_IMAGE_ u8"Imagen de sombreada : "
#define _TRACE_IMAGE_ u8"Imagen de trazo : "
#define _IMAGE_ u8"Imagen : "

#define _PATTERN_ u8"Patrón : "

#define _PCX_WRITE_ERROR_ u8"Error de escritura del archivo PCX"
#define _PNG_WRITE_ERROR_ u8"Error de escritura del archivo PNG"
#define _JPG_WRITE_ERROR_ u8"Error de escritura del archivo JPG"

#define _NO_PATTERN_ u8"SINPATRÓN.axx"

#define _NO_BLOCK_IN_CATALOG_ u8"Ningún símbolo en el catálogo ("
#define AUTOSCALE u8"   AUTOESCALA"

#define FOLDER u8"\023<TODOS>"

#endif

#ifdef __O_BREAK__

static POLE pmBreakOptions[] = {
		 {u8"romper y Dejar",L'D',692,NULL},
		 {u8"romper y Cortar",L'C',693,NULL},
         };

static TMENU mBreakOptions = { 2,0,0,4,24,8,TADD | ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pmBreakOptions,NULL,NULL };


POLE pmBreak[] =
{
	 {u8"Anular",'A',459,NULL},
	 {u8"Dividir",'D',50,NULL},
     {u8"Opciones de romper\0                                       ",L'O',691,&mBreakOptions},
};

#define _BREAK_ u8"Romper"
#define _BREAK_C_ L'R'

static char _break_and_leave_[] = u8"romper y Dejar";
static char _break_and_cut_[] = u8"romper y Cortar";

#endif

#ifdef __O_TRIM__

static POLE pmTrim[] =
{
 {u8"Deshacer",'D',459, NULL},
};

#define _TRIM_ "Recortar"
#define _TRIM_C_ L'R'

static POLE pmExtend[] =
{
 {u8"Deshacer",'D',459, NULL},
};

#define _EXTEND_ "Extender"
#define _EXTEND_C_ L'E'

#endif

#ifdef __O_EXTEND__

#endif

#ifdef __B_HLP__

#define _1920_ 1920

POLE pmHelp[] = {
	 {u8"ÚTILES",31,351,NULL},
	 {u8"Menú auxiliar (con útiles)",30,391,NULL},
	 {u8"Menú auxiliar (con útiles)",26,359,NULL},
	 {u8"Repetir última función",548 + _1920_,547,NULL},
	 {u8"Editar valores numéricos",' ',363,NULL},
	 {u8"Aumentar el tamaño del puntero",' ',361,NULL},
	 {u8"Reducir el tamaño del puntero",' ',360,NULL},
	 {u8"Libera el mouse",' ',386,NULL},
	 {u8"Libera el mouse",24,353,NULL},
     {u8"Libera el mouse (mantenga presionado el derecho 0,5 s)",819 + _1920_, 658,NULL},
	 {u8"Cambiar el tamaño y la posición de la ventana",25,353,NULL},
	 {u8"Detener la grabación de macros",28,353,NULL},
	 {u8"Ortogonalidad",565 + _1920_,353,NULL},
     {u8"Copia el objeto debajo del cursor",566 + _1920_,353,NULL},
     {u8"Elige una capa",571 + _1920_,353,NULL},
	 {u8"EDITAR",31,321,NULL},
	 {u8"Editar texto",' ',377,NULL},
	 {u8"Mover texto",15,378,NULL},
	 {u8"Eliminar objeto",' ',380,NULL},
	 {u8"Restaurar objeto eliminado",' ',381,NULL},
	 {u8"Ortogonalidad",' ',383,NULL},
	 {u8"Ventana / Cruz",' ',384,NULL},
	 {u8"Agregar / eliminar",' ',385,NULL},
	 {u8"VISTA",31,53,NULL},
	 {u8"Acercar / alejar",' ',389,NULL},

	 {u8"Panorámica",355+1920,353,NULL},
	 {u8"Panorámica",354+1920,353,NULL},

	 {u8"Ventana",12,353,NULL},
	 {u8"Última vista",13,353,NULL},
	 {u8"Vista: all",14,353,NULL},
	 {u8"Vista: center",15,353,NULL},
	 {u8"Vista: escala 1",16,353,NULL},
	 {u8"Vista: escala 0.5",17,353,NULL},
	 {u8"Vista: escala 0.2",18,353,NULL},
	 {u8"Vista: escala 0.1",19,353,NULL},
	 {u8"Redibujar",20,353,NULL},
	 {u8"Vista: dibujo completo",21,353,NULL},
	 {u8"CARACTERÍSTICAS",31,352,NULL},
	 {u8"Color",12,354,NULL},
	 {u8"Tipo de línea",13,354,NULL},
	 {u8"Ancho de línea",14,354,NULL},
	 {u8"Capa",15,354,NULL},
	 {u8"Tomar color",19,354,NULL},
	 {u8"Tomar tipo y ancho de línea",20,354,NULL},
	 {u8"Tomar capa",21,354,NULL},
	 {u8"Escondir capa",22,354,NULL},
	 {u8"MEDIDA",31,390,NULL},
	 {u8"Coordenadas del punto",16,354,NULL},
	 {u8"Distancia",17,354,NULL},
	 {u8"Ángulo",18,354,NULL},
	 {u8"CAPAS",31,55,NULL},
	 {u8"Capa 1",1,354,NULL},
	 {u8"Capa 2 ...",2,354,NULL},
	 {u8"Capa 10",0,354,NULL},
	 {u8"Próxima capa",10,354,NULL},
	 {u8"Capa anterior",11,354,NULL},
	 {u8"Todas las capas activadas",22,353,NULL},
	 {u8"Capas inversas",23,353,NULL},
	 {u8"PASO DE PUNTERO",31,154,NULL},
	 {u8"Paso mínimo / grande",12,355,NULL},
	 {u8"BUSCADOR DE PUNTOS",31,52,NULL},

	 {u8"Punto final",13,355,NULL},
	 {u8"Extremo más cercano",14,355,NULL},
	 {u8"Punto medio",15,355,NULL},
	 {u8"Intersección",16,355,NULL},
	 {u8"Punto central",17,355,NULL},
	 {u8"Perpendicular",18,355,NULL},
	 {u8"Tangencial",19,355,NULL},
	 {u8"Adyacente",20,355,NULL},
	 {u8"Punto",21,355,NULL},

	 {u8"Punto final",' ',553,NULL},
	 {u8"Extremo más cercano",' ' ,562,NULL},
	 {u8"Punto medio",' ',561,NULL},
	 {u8"Intersección",' ',557,NULL},
	 {u8"Punto central",' ',551,NULL},
	 {u8"Perpendicular",' ',564,NULL},
	 {u8"Tangencial",' ' ,568,NULL},
	 {u8"Adyacente",' ',549,NULL},
	 {u8"Punto",' ',563,NULL},

	 {u8"Modo de punto final",553+1920,353,NULL},
	 {u8"Modo extremo más cercano",562+1920,353,NULL},
	 {u8"Modo punto medio",561+1920,353,NULL},
	 {u8"Modo intersección",557+1920,353,NULL},
	 {u8"Modo punto central",551+1920,353,NULL},
	 {u8"Modo perpendicular",564+1920,353,NULL},
	 {u8"Modo tangencial",568+1920,353,NULL},
	 {u8"Modo adyacente",549+1920,353,NULL},
	 {u8"Mode punto",563+1920,353,NULL},
	 {u8"Modo ninguno",572 + 1920,353,NULL},
     {u8"Instrucción breve",702 + 1920,534,NULL},
};

//#define _ENDPOINT_ L'E'
//#define _ENDPOINT_SC_ 5
//#define _NEAREST_ L'N'
//#define _NEAREST_SC_ 14

#define _ENDPOINT_ L'F'
#define _ENDPOINT_SC_ 6
#define _NEAREST_ L'E'
#define _NEAREST_SC_ 5

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
	{u8"Registrar. Final-> Ctrl-End",L'R',92,NULL},
	{u8"Ejecutar",L'E',93,NULL},
	{u8"Cargar archivo",L'C',94,NULL},
	{u8"Guardar en archivo",L'G',95,NULL}  };

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
PTMENU mCzcionkaZ = { 1,/*MaxMenuWinFont*/ 0, /*MaxMenuWinFont*/ 0,FONTNAMELEN + 1,74,10,FIXED,CMNU,CMBR,CMTX,0,69,0,0,0,(POLE(*)[]) & pmCzcionka,NULL,NULL};

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
		 {u8"Fino\0",'F',594,&mKursorSize},
		 {u8"Dúplex\0",'D',595,&mKursorSizeB} };

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
		 {u8"[1] milímetro \0[mm]",'1',0,NULL},
		 {u8"[2] centímetro \0[cm]",'2',0,NULL},
		 {u8"[3] metro \0[m]",'3',0,NULL},
         {u8"[4] kilómetro \0[km]",'4',0,NULL},
		 {u8"[5] pulgada \0[\"]",'5',0,NULL},
		 {u8"[6] pie \0[']",'6',0,NULL},
         {u8"[7] yarda \0[yd]",'7',0,NULL},
         {u8"[8] milla \0[mi]",'8',0,NULL}};

static TMENU mJednostki = { 8,0,0,18,24,8,TADD,CMNU,CMBR,CMTX,0,78,0,0,0,(POLE(*)[]) &pmJednostki,NULL,NULL };

static POLE pmKartezjanskiGeodezyjny[2] = {
		 {u8"Cartesianas",'C',104,NULL},
		 {u8"Geodésicas",'G',105,NULL} };
	 
static TMENU mKartezjanskiGeodezyjny = { 2,0,0,12,24,8,ICONS,CMNU,CMBR,CMTX,0,109,0,0,0,(POLE(*)[]) &pmKartezjanskiGeodezyjny,NULL,NULL };

static POLE pmPointOrigin[3] = {
		 {u8"Punto de origen\0 ",'P',106,NULL},
		 {u8"Eje de coord. locales\0",'E',107,NULL},
		 {u8"Ángulo de rotación\0",'A',108,NULL } };


static TMENU mPointOrigin = { 3,0,0,19,24,9,ICONS,CMNU,CMBR,CMTX,0,86,0,0,0,(POLE(*)[]) &pmPointOrigin,NULL,NULL };

static char* tablica_styli[] = { u8"Superior", u8"Inferior", u8"Superior y Inferior", u8"Alrededor" };

static POLE pmSektory_Styl[4] = {
		 {u8"Superior",'S',498,NULL},
		 {u8"Inferior",'I',499,NULL},
		 {u8"superior Y inferior",'Y',500,NULL},
		 {u8"Alrededor",'A',486,NULL} };

static TMENU mSektory_Styl = { 4,0,0,20,30,7,ICONS,CMNU,CMBR,CMTX,0,65,0,0,0,(POLE(*)[]) &pmSektory_Styl,NULL,NULL };

static POLE pmSektory_Arkusza[15] = {
	   {u8"Cuadro\0 N\0           ",'C',485,&mRamka_on_off},
	   {u8"Zonas\0 N\0          ",'Z',486,&mSektory_on_off},
	   {u8"Prefijo del número de zona\0 \0     ",'P',487,NULL},
	   {u8"Numeración desde\0 \0   ",'N',488,NULL},
	   {u8"numeración Inversa\0 N\0           ",'I',489,&mReversed_on_off},
	   {u8"Altura de zona [mm]\0 \0   ",'A',490,NULL},
	   {u8"anchO de zona [mm]\0 \0   ",'O',491,NULL},
	   {u8"ancho de Margen [mm]\0 \0   ",'M',492,NULL},
	   {u8"Desplazamiento de cuadro [mm]\0 \0   ",'D',501,NULL},
	   {u8"grosor de Línea de cuadro\0Fina\0          ", 'L',493,&mGruboscLiniiF},
	   {u8"Grosor de línea de zonas\0Gruesa\0          ", 'G',494,&mGruboscLiniiZ},
	   {u8"֎color de cUadro\0blanco\0              ",'U',495,&mKolorACF},
	   {u8"֎coloR de zonas\0blanco\0              ",'R',496,&mKolorACZ},
	   {u8"Estilo\0Superior\0              ",'E',497,&mSektory_Styl},
	   {u8"Fuente\0 \0             ", 'F', 183,&mCzcionkaZ}};

static POLE pmPrecision[] = {
        {u8"precisión de Forzar\0 \0",          L'F',727,NULL},
        {u8"precisión de Momento\0 \0",         L'M',728,NULL},
        {u8"precisión de Desplazamiento\0 \0",  L'D',730,NULL},
        {u8"precisión de Rotación\0 \0",        L'R',731,NULL},
        {u8"precisión de Carga\0 \0",           L'C',733,NULL},
        {u8"precisión de carga Térmica\0 \0",   L'T',752,NULL},
        {u8"precisión de tensiones\0 \0",       L'S',800,NULL},
};

static TMENU mPrecision = { 7,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,17,0,0,0,(POLE(*)[]) &pmPrecision,NULL,NULL };

static POLE pmThermal[] = {
       {u8"Profundidad de la sección\0 \0",L'P',764,NULL},
	   {u8"cambio de Temperatura\0 \0",L'T',765,NULL},
	  };

static TMENU mThermal = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmThermal,NULL,NULL };

static POLE pmStaticColors[] = {
       {u8"֎Nodos y elementos\0gris\0",         L'N',723,&mKolorSTATIC},
       {u8"֎fuerzas de Tracción\0rojo\0",       L'T',771,&mKolorSTATIC},
	   {u8"֎fuerzas de Compresión\0azul\0",     L'C',758,&mKolorSTATIC},
       {u8"֎Fuerzas cortantes\0verde\0",        L'F',759,&mKolorSTATIC},
       {u8"֎Momentos flectores\0magenta\0",     L'M',760,&mKolorSTATIC},
       {u8"֎Deformaciones\0amarillo\0",         L'D',761,&mKolorSTATIC},
       {u8"֎Reacciones\0cian\0",                L'R',763,&mKolorSTATIC},
       {u8"֎tensión Axial+\0rojo oscuro\0",     L'A',801,&mKolorSTATIC},
       {u8"֎tensión aXial-\0azul oscuro\0",     L'X',802,&mKolorSTATIC},
       {u8"֎tensión cortante\0verde oscuro\0",  L'H',803,&mKolorSTATIC},
       {u8"֎Vibraciones\0otro\0",               L'V',814,&mKolorSTATIC},
	  };

static TMENU mStaticColors = { 11,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,24,0,0,0,(POLE(*)[]) &pmStaticColors,NULL,NULL };

static POLE pmMagnitude[] = {
       {u8"reescalado de Forzar\0 \0",              L'F',727,NULL},
	   {u8"reescalado de Momento\0 \0",             L'M',728,NULL},
	   {u8"reescalado de Desplazamiento\0 \0",      L'D',730,NULL},
	   {u8"reescalado de Rotación\0 \0",            L'R',731,NULL},
       {u8"reescalado de Carga\0 \0",               L'C',733,NULL},
       {u8"reescalado de carga Térmica\0 \0",       L'T',752,NULL}, //&mThermal}
       {u8"reescalado de fuerzas axiales\0 \0",     L'A',758,NULL},
       {u8"reescalado de fuerzas cortantes\0 \0",   L'V',759,NULL},
       {u8"reescalado de momentos flectores\0 \0",  L'B',760,NULL},
       {u8"reescalado de deformación\0 \0",         L'U',761,NULL},
       {u8"reescalado de fuerzas de reacción\0 \0", L'X',762,NULL},
       {u8"momentos de reacción reescalado\0 \0",   L'Z',763,NULL},
       {u8"reescalado de tensión \0 \0",            L'S',800,NULL},
       {u8"reescalado % de refuerzo\0 \0",          L'%',823,NULL},
       {u8"Exageración de las vibraciónes\0 \0",    L'E',814,NULL},
       {u8"Precisión\0 \0",                         L'P',184,NULL}, //&mPrecision},
       {u8"Coloros de Graficas\0 \0",               L'G',495,NULL}, //&mStaticColors},
       };

static TMENU mMagnitude = { 17,0,0,10,30,7,TADD | ICONS | NOWCLOSE,CMNU,CMBR,CMTX,0,111,0,0,0,(POLE(*)[]) &pmMagnitude,NULL,NULL };

static POLE pmParametry[] = {
	 {u8"Formato\0 A4\0     ",'F',96,&mFormat_r},
	 {u8"Escala\0 1:5\0     ",'E',97,&mSkalaF},
	 {u8"Unidades\0       mm\0    ",'U',99,&mJednostki},
     {u8"Precisión\0 0.25mm\0    ",'P',98,&mDokladnosc},
	 {u8"Sistema coordinado\0 C\0 ",'S',100,&mKartezjanskiGeodezyjny},
	 {u8"sistema de coordenadas Locales\0 \0       ",'L',101,&mPointOrigin},
	 {u8"Zonas\0 \0      ",'Z',486,NULL},
	 {u8"Tipo y tamaño de puntero\0 Max\0  ",'T',102,&mKursorTyp},
     {u8"reescalado de los Vectores,precisión y colores\0\0  ",'V',751, NULL}, //&mMagnitude},
	 {u8"editar ALFACADES.INI\0 \0     ",'A',103,NULL},
};

static char* ParTxt[] = { u8"C", "G", "Y", "N", };

#define _CARTESIAN_ u8"C"
#define _GEODETIC_ u8"G"
#define _MAX_ "Max"
#define _PLUSMAX_ "+Max"
#define _OTHER_COLOR_ u8"֍[X] otro >>> "

#define _SECTORS_        "SECTORES"

static char config_sectors[11][32] =
{
	"Grosor de la línea del quadro",
	"Grosor de línea de zona",
	"Color del quadro",
	"Color de la zona",
	"El offset del marco",
	"Altura de la zona",
	"Ancho de zona",
	"Ancho de margen",
	"Fuente de zona",
	"Estilo de zona",
	"Numeración invertida",
};

#endif

#ifdef __O_LIBFUN__

char bloki_dir[MAXPATH] = "BLOQUES";
char katalogi_dir[MAXPATH] = "CATÁLOGOS";
char bloki_cfg[MAXPATH] = "BLOQUES.CFG";
char biblioteka_cfg[MAXPATH] = "APARATOS.AXX";

#endif

#ifdef __O_FOLDERSDIALOG__

#define _SELECT_FOLDER_  L"Seleccione la carpeta o escriba .. o / o // para más"
char error_message_block[64] = u8"No hay archivos de bloque en esta carpeta";
char error_message_background[64] = u8"No hay archivos de fondo en esta carpeta";

#endif

#ifdef __BIB_E__

char* view_width_tab[] = { u8"m.d.",u8"d.",u8"G.",u8"M.G.",u8"E.G.", "", "", u8"escondida " };

char* view_type_tab[] = {
			    u8"sólida",
				u8"discontinua",
				u8"punteada",
				u8"2-punteada",
				u8"multipunteada",
				u8"borde",
				u8"mediana",
				u8"escondida",
				u8"fantasma",
				u8"discontinua_2",
				u8"punteada_2",
				u8"2-punteada_2",
				u8"multipunteada_2",
				u8"borde_2",
				u8"mediana_2",
				u8"escondida_2",
				u8"fantasma_2",
				u8"discontinua_x2",
				u8"punteada_x2",
				u8"2-punteada_x2",
				u8"multipunteada_x2",
				u8"borde_x2",
				u8"mediana_x2",
				u8"escondida_x2",
				u8"fantasma_x2",
				u8"3-punteada",
				u8"2-mediana",
				u8"3-discontinua",
				u8"fantasma_3",
				u8"2-mediana_x2",
				u8"3-discontinua_x2",
				u8"multidiscontinua",
			    u8"" };


T_Prototype      s__prot = { u8"prototype.alf", TRUE };

#define _SYSTEM_MESSAGE_ u8"MENSAJE DEL SISTEMA"
#define _INTERRAPTED_ u8"Interrumpido"
#define _TOO_MANY_BLOCKS_ u8"Demasiados bloques para comprobar todos"
#define _BLOCKS_CORRECTED_ u8"%ld bloques, %ld objetos han sido corregidos"
#define _BLOCKS_TESTED_ u8"Todo bien. %d bloques probados"

char* typ_punktu_inf[] = { u8"Simple",u8"Punto base","","","","","",u8"Unión",u8"Pasador","","","",
                           u8"Empotrado",u8"Empotrado I",u8"Empotrado D",u8"Empotrado A", u8"Articulado", u8"Articulado I", u8"Articulado D", u8"Articulado A",
                           u8"Empotrado Deslizadera", u8"Empotrado Deslizadera I", u8"Empotrado Deslizadera D", u8"Empotrado Deslizadera A", u8"Articulado Deslizadera", u8"Articulado Deslizadera I", u8"Articulado Deslizadera D", u8"Articulado Deslizadera A", u8"sin rotación Z",
                           "","","","","","","","",
};

#endif

#ifdef __O_LISTBL__

#define _NUMBER_OF_BLOCKS_ u8"El número de bloques ("
#define _EXCEEDS_NO_ u8") supera los 5000"

#endif

#ifdef __O_ETEXT__

#define _YES_NO_ESC_ u8"SNsn\033"

POLE pmenu_typ[] = {
			   {u8"*ALL",'*',0,NULL},       //255
			   {u8"Normal",'N',0,NULL},
			   {u8"etiqueta Interna",'I',0,NULL},
			   {u8"Etiqueta de componente",'E',0,NULL},
			   {u8"Símbolo",'S',0,NULL},
			   {u8"Tipo",'T',0,NULL},
			   {u8"?nombre del pin",'?',0,NULL},
			   {u8"Fuente de alimentación",'F',0,NULL},
			   {u8"Puerto",'P',0,NULL},
			   {u8"Atributo",'A',0,NULL},
			   {u8"+Esquema",'+',0,NULL},
			   {u8"nombre De archivo",'D',0,NULL},
			   {u8"Red",'R',0,NULL},
			   {u8"Comentario",'C',0,NULL},
			   {u8"%variable",'%',0,NULL},
			   {u8"aUx.símbolo",'U',0,NULL},
			   {u8"auX.descripción",'X',0,NULL},
};

POLE pmenu_typ1[] = {
			   {u8"SÍMBOLO",'S',0,NULL},           //3 
			   {u8"*ALL",'*',0,NULL},       //255
			   {u8"Normal",'N',0,NULL},
			   {u8"etiqueta Interna",'I',0,NULL},
			   {u8"Etiqueta de componente",'E',0,NULL},
			   {u8"Símbolo",'S',0,NULL},
			   {u8"Tipo",'T',0,NULL},
			   {u8"?nombre del pin",'?',0,NULL},
			   {u8"Fuente de alimentación",'F',0,NULL},
			   {u8"Puerto",'P',0,NULL},
			   {u8"Atributo",'A',0,NULL},
			   {u8"+Esquema",'+',0,NULL},
			   {u8"nombre De archivo",'D',0,NULL},
			   {u8"Red",'R',0,NULL},
			   {u8"Comentario",'C',0,NULL},
			   {u8"%variable",'%',0,NULL},
			   {u8"aUx.símbolo",'U',0,NULL},
			   {u8"auX.descripción",'X',0,NULL},
};

POLE pmenu_auto[] = {
			   {u8"cambia Todo",'T',615,NULL},
			   {u8"Confirmar cada",'C',616,NULL},
};

POLE pmenu_change[] = {
			   {u8"Cambio",'C',112,NULL},
			   {u8"Saltar",'S',113,NULL},
};

#endif

#ifdef __O_TEXT_B__

#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ L'S'
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
			   {u8"etiqueta Interna",'I',0,NULL},
			   {u8"Etiqueta de componente",'E',0,NULL},
			   {u8"Símbolo",'S',0,NULL},
			   {u8"Tipo",'T',0,NULL},
			   {u8"?nombre del pin",'?',0,NULL},
			   {u8"Fuente de alimentación",'F',0,NULL},
			   {u8"Puerto",'P',0,NULL},
			   {u8"Atributo",'A',0,NULL},
			   {u8"+Esquema",'+',0,NULL},
			   {u8"nombre De archivo",'D',0,NULL},
			   {u8"Red",'R',0,NULL},
			   {u8"Comentario",'C',0,NULL},
			   {u8"%variable",'%',0,NULL},
			   {u8"aUx.símbolo",'U',0,NULL},
			   {u8"auX.descripción",'X',0,NULL},
};

TMENU mTypTekstu = { 16,0,0,16,74,6,0,CMNU,CMBR,CMTX,0,15,0,0,0,(POLE(*)[]) &pmTypTekstu,NULL,NULL };

POLE pmJustowanie[] = {
					{u8"a la Izquierda",'I',262,NULL},
					{u8"a la Derecha",'D',263,NULL},
					{u8"Medio",'M',264,NULL},
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
	{u8"Editar otra vez  \0        ", 'E',38,NULL},
	{u8"Ángulo   \0            ", 'A',229,&mKatT},
	{u8"altura\0 3mm\0         ", 'H',179,NULL},
	{u8"factoR de ancho\0 1.0\0     ", 'R',230,NULL},
	{u8"Cursiva\0 N\0          ", 'C', 181,&mItalics},
	{u8"Tipo\0 \0             ", 'T', 231,&mTypTekstu},
	{u8"escOndido\0 N\0          ", 'O', 232,&mUkryty},
	{u8"Negrita\0 N\0            ", 'N', 182,&mBold},
	{u8"sUbrayada\0 N\0      ", 'U', 409,&mUnderlined},
	{u8"aLinear\0 |<\0            ", 'L', 233,&mJustowanie},
	{u8"eSpaciado entre líneas\0 N\0    ", 'S', 410,&mLineSpacing},
	{u8"Intervalo automático\0 N\0   ", '+', 91,&mAutoInterwal},
	{u8"Intervalo\0 1\0        ", '*', 234,NULL},
	{u8"Fuente\0 \0             ", 'F', 183,&mCzcionka},
};

static char t_t[16][32] = {
u8"Normal", u8"Etiqueta interna", u8"Etiqueta de comp.", u8"Símbolo", u8"Tipo",
u8"nombre pin", u8"fuente de alim.", u8"Puerto", u8"Atributo", u8"+Esquema", u8"nombre de archivo", u8"red",
u8"Comentario", u8"%variable", u8"aUx.símbolo", u8"auX.descrpción" };

#define _TEXT_ u8"Texto"
#define _TEXT_C_ L'T'
#define _HIDDEN_ u8" escondido"

#endif

#ifdef __B_WINDEM__

#define _NOT_LICENSED_ u8"Al hacer una donación apoyas el proyecto"
#define _NOT_LICENSED1_ u8" Gracias."
#define _DEM_ROWS_NO_ 12
char lic_for[16] = { u8"Usuario: " };

POLE pmDem1[] = {
	/*0*/	 {u8"",' ',0,NULL},
#ifdef BIT64
	/*1*/	 {u8"Creando nueva ventana de AlfaCAD (x64)",' ',0,NULL},
#else
	/*1*/	 {u8"Creando nueva ventana de AlfaCAD",' ',0,NULL},
#endif
	/*7*/{u8"_______________________________________________",' ',0,NULL},
	/*10*/   {u8"",' ',0,NULL},
};

POLE pmPrint[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8"  I M P R E S I Ó N  ",' ',0,NULL},
	/*2*/	 {u8"   Interrumpir -> Esc    ",' ',0,NULL},
	/*3*/  {u8"",' ',0,NULL},
};

POLE pmPlot[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8"   P L O T E A R   ",' ',0,NULL},
	/*2*/	 {u8"",' ',0,NULL},
};

POLE pmPlot_to_com[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8" ENVÍO DE DATOS AL PLOTTER ",' ',0,NULL},
	/*2*/	 {u8"",' ',0,NULL},
};

T_Prot_Date s_prot_date = { PROT_KEY, 0, 1, 0, 0, "Usuario : ", "Serie : ",
0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "",
0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "" };

#endif

#ifdef __O_OBJECT1__

#define _YES_NO_ESC_ u8"SNsn\033"
#define _YES_ 'S'
#define _yes_ 's'
#define _NO_ 'N'
#define _no_ 'n'

#endif

#ifdef __O_ARRAY__

static POLE pmArray_polar[] = {
	{u8"Número de elementos\0 1\0     ",'N',607,NULL},
	{u8"Ángulo de llenado [°]\0 360\0     ",'A',608,NULL} };


static POLE pmArray_polar_rot[] = {
	{u8"Número de elementos\0 1\0     ",'N',609,NULL},
	{u8"Ángulo de llenado [°]\0 360\0     ",'A',610,NULL} };

#define _ARRAY_WITH_ROTATION_ u8"Matriz con rotación"
#define _ARRAY_ u8"Matriz"
#define _ARRAY_C_ L'M'

#endif

#ifdef  __BIBWYM4__

#define _DIMENSIONING_ u8"Dimensionamiento"
#define _DIMENSIONING_C_ L'D'

#endif

#ifdef __BIBWYM_B_

static POLE pmKierunek[7] = {
		{u8"0°",'0',203,NULL},
		{u8"90°",'9',204,NULL},
		{u8"Paralelo a",'P',205,NULL},
		{u8"en ángulo con el Objeto",'O',206,NULL},
		{u8"en ángulo X",'X',207,NULL},
		{u8"dim. Ángulo",'A',208,NULL},
        {u8"Líder",'L',678,NULL} };

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
		{u8"Dirección\0=\0        ",'D',196,&mKierunek},
		{u8"Nueva\0End",'N',197,NULL},
		{u8"Remover\0Del",'R',198,NULL},
		{u8"Editar\0F10",'E',199,NULL},
		{u8"Parámetros\0",'P',200,NULL},
		{u8"Arco\0R\0             ",'A',201,&mWym_luk_o},
		{u8"Círculo\0D\0           ",'C',202,&mWym_okrag_o}, };

#define _NEW_ u8"nueva"

static POLE pmStrzalka[3] = {
	  {u8"Flechas",'F',189,NULL},
	  {u8"Trazos",'T',190,NULL},
	  {u8"Puntos",'P',191,NULL} };

static POLE pmKierunekT[3] = {
	{u8"Paralelo",'P',192,NULL},
    {u8"Colineal",'C',696,NULL},
	{u8"Horizontal",'H',193,NULL} };

static POLE pmAddLine[2] = {
	{u8"Longitud Fija",'F',194,NULL},
	{u8"Alinear al objeto",'A',195,NULL} };

static POLE pmLFormat[4] = {
	{u8"Decimal",'D',0,NULL},
    {u8"Ingeniería",'I',0,NULL},
	{u8"Arquitectónico",'A',0,NULL},
    {u8"Fraccionario",'F',0,NULL},
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
	  {u8"֎color de Linea\0              ", 'L',177,&mKolorL},
	  {u8"֎color de Texto\0              ", 'T',178,&mKolorT},
	  {u8"Altura del texto\0             ", 'A',179,NULL},
	  {u8"factor de ancho de teXto\0     ", 'X',180,NULL},
	  {u8"texto en Cursiva\0             ", 'C',181, &mItalics},
	  {u8"texto en negrita\0             ", 'N',182, &mBold},
	  {u8"Fuente\0                       ", 'F',183, &mCzcionkaW},
      {u8"formato de Rótulo\0            ", 'R',697, &mLFormat},
	  {u8"Precisión\0                    ", 'P',184,NULL},
	  {u8"terminadores\0                 ", 'I',185,&mStrzalka},
	  {u8"Desplazamiento de línea\0      ", 'D',186,NULL},
	  {u8"dirección del texto\0          ",' K',187,&mKierunekT} ,
	  {u8"líneas de Extensión\0          ", 'E',188,&mAddLine} };

char *_LFORMAT_[]={u8"Decimal",u8"Ingeniería",u8"Arquitectónico",u8"Fraccionario"};

#endif

#ifdef __O_DIVIDE__

POLE pmDiv_Pline[] = {
	{u8"Polilínea",'P',454,NULL},
	{u8"Segmento",'S',455,NULL} };

static TMENU mDiv_Pline = { 2, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 8, 0, 0,0,(POLE(*)[]) &pmDiv_Pline, NULL, NULL };
static TMENU mAlign_Block = { 2, 0,0, 7, 83, 13, ICONS, CMNU,CMBR,CMTX,0, 10, 0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmDivide[] = {
	 {u8"Dividir\0",'D',448,NULL},
	 {u8"N° de segmentos\0 5\0      ", 'N', 449,NULL},
	 {u8"Medida\0", 'M',450, NULL},
	 {u8"Longitud del segmento\0 100.0\0       ", 'L',451, NULL},
	 {u8"máXimo\0 1000\0",'X', 452,NULL},
	 {u8"Polilínea/segmento\0   polyline\0  ",'P',453, &mDiv_Pline},
	 {u8"Remover\0Del\0",'R', 270,NULL},
	 {u8"Alinear bloque\0 Y\0",'A', 456,&mAlign_Block} };

#define _POLYLINE_ u8"polilinea"
#define _SEGMENT_ u8"segmento"

char _YES__[4] = u8"S";
char _NO__[4] = u8"N";

#define _DIVIDE_ u8"Dividir"
#define _DIVIDE_C_ L'D'

#endif

#ifdef __O_KONSTR__

static char* opis_zbrojenia[] = { u8"Barra de refuerzo" };

#endif

#ifdef __O_RECTA__

static POLE	pmKatR[]= {
	{u8"rOtar por ángulo",'O',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",   '0',578,NULL},
	 {u8"roTar al ángulo",'T',440,NULL},
	 {u8"paRalelo a",'R',580,NULL},
	 {u8"Perpendicular a",'P',581,NULL},
	 {u8"en Ángulo a",'A',582,NULL},
};

TMENU mKatR = { 13,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmKatR,NULL,NULL };

static POLE	pmRectOpt[]= {
	 {u8"Dos puntos",L'D',22,NULL},
     {u8"dos puntos y un punto/ángulo de Inserción",L'I',684,NULL},
};

TMENU mRectOpt = { 2,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,13,0,0,0,(POLE(*)[]) &pmRectOpt,NULL,NULL };

static POLE	pmKatOptR[]= {
	 {u8"rOtar por ángulo",'O',579,NULL},
	 {u8"45°",	'4',441,NULL},
	 {u8"90°",	'9',442,NULL},
	 {u8"135°",	'1',443,NULL},
	 {u8"180°",	'8',444,NULL},
	 {u8"225°",	'2',445,NULL},
	 {u8"270°",	'7',446,NULL},
	 {u8"315°",	'3',447,NULL},
	 {u8"0°",   '0',578,NULL},
	 {u8"roTar al ángulo",'T',440,NULL},
	 {u8"paRalelo a",'R',580,NULL},
	 {u8"Perpendicular a",'P',581,NULL},
	 {u8"en Ángulo a",'A',582,NULL},
     {u8"Opciones de dibujo",L'O',685,&mRectOpt},
};

TMENU mKatOptR = { 14,0,0,10,74,6,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmKatOptR,NULL,NULL };

#define _RECTA_A_ u8"Rectángulo: ángulo del eje y opciones de dibujo"
#define _RECTA_R_ u8"Rectángulo: rotación"
#define _RECTA_C_ L'R'

#endif

#ifdef __O_LTTYPE__

POLE pmTTF_OTF[] = {
   {u8"fuentes TrueType",'T',703,NULL},
   {u8"fuentes OpenType",'O',704,NULL},
};

TMENU mTTF_OTF = { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmTTF_OTF,NULL,NULL };

#endif