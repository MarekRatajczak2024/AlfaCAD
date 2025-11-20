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

#define __EDIT_MULTILINE__ L"Редагувати багаторядковий текст"

#define FILENAME_INI L"ALFACADUA.INI"
LPWSTR filename_ini = (LPWSTR)L"Редагувати ALFACADUA.INI";

// Create a locale object representing the German (Switzerland) locale
#define _LOCALE_ "ua-UA"

char __BOLD__[]=u8"Жирний";
char __UNDERLINED__[]=u8"Підкреслено";
char __ITALIC__[]=u8"Курсив";
char __A_TO_LEFT__[]=u8"Змінити вліво";
char __A_TO_RIGHT__[]=u8"Змінити праворуч";
char __A_TO_MIDDLE__[]=u8"Відрегулювати до середини";
char __A_TO_CENTRE__[]=u8"Відрегулювати до центру";
char __HIDDEN__[]=u8"Прихований";
char __COPY_TEXT__[]=u8"Копіювати текст із буфера обміну";

char _EDIT_TEXT_[]=u8"Редагування тексту";
char _EDIT_FILE_[]=u8"Редагування файла";

#endif

#ifdef __WINPRINT__

#define _NEW_WINDOW_ u8"Бажаєте відкрити нове вікно креслення?"
#define _NEW_WINDOW_T_ u8"Нове вікно"

int max_quote = 293;
#define _QUOTE_ u8"quotesua.dat"
char about_pre[32] = u8"\n\nЦитата дня ";
char _about_[16] = u8"про ";
#endif

#ifdef __BIBRYS__

POLE pmNie_Tak_[] = {
	{u8"Ні",L'Н',113,NULL},
	{u8"Так",L'Т',112,NULL} };

POLE pmEditPCX[] = {
   {u8"Змінити dx, dy", L'З',114, NULL},
   {u8"змінити Параметри", L'П',54, NULL},
   {u8"Калібрування", L'К',115,NULL},
   // {u8"Редагувати блок", 'Р',0, NULL},
};

TMENU mEditPCX = { 3,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,20,0,0,0,(POLE(*)[]) &pmEditPCX,NULL,NULL };
TMENU mSaveLayer = { 2,0,0,7,16,8,ICONS,CMNU,CMBR,CMTX,0,18,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmInsertPCX[] = {
   {u8"вставити Карту\0\0 ", L'К',600,NULL},
   {u8"вставити фотографію як ПНҐ\0\0 ", L'П',601,NULL},
   {u8"вставити фотографію як ЙПҐ\0\0 ", L'Й',624,NULL},
   {u8"векторизувати і імпортувати\0\0 ",L'А',698,NULL},
};

TMENU mInsertPCX = { 4,0,0,19,16,8,ICONS,CMNU,CMBR,CMTX,0,23,0,0,0,(POLE(*)[]) &pmInsertPCX,NULL,NULL };

POLE pmElliptical[] = {
	{u8"Еліпс", L'Е',19,NULL},
    {u8"еліптичний Диск",  L'Д',707,NULL},
    {u8"еліптична  дУга",  L'У',706,NULL},
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
	{u8"Прямокутний",	L'П', 116,NULL},
	{u8"Круговий", L'К', 117,NULL},
	{u8"круговий з Обертанням", L'О', 118,NULL},
};

TMENU mArray = { 3, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, 20, 0, 0,0,(POLE(*)[]) &pmArray, NULL, NULL };

POLE pmMirrorBlock[] = {
	{u8"дзеркальне блоку Х", L'Х',40,NULL}, 	  
	{u8"дзеркальне блоку У", L'У',41,NULL}, 	  
};

TMENU mMirrorBlock = { 2, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, 23, 0, 0,0,(POLE(*)[]) &pmMirrorBlock, NULL, NULL };

POLE  pmEdycja[] = {
	{u8"Рухатити\0\0",              L'Р',30,NULL},
	{u8"Копія\0\0",                 L'К',31,NULL},
	{u8"Видалити\0\0",              L'В',32,NULL},
	{u8"відновленитИ\0\0",          L'И',33,NULL},
	{u8"Обертати\0\0",              L'О',34,NULL},
	{u8"*обертати блок\0\0",        L'*',35,NULL},
	{u8"Масштаб\0\0",               L'М',36,NULL},
	{u8"Тягнути\0\0",               L'Т',37,NULL},
	{u8"рЕдагувати текст\0\0",      L'Е',38,NULL},
	{u8"Дзеркало\0\0",              L'Д',39,NULL},
	{u8"дзеркало Блоку\0\0",        L'Б',535,&mMirrorBlock},
	{u8"Матриця\0\0",               L'М',42,&mArray},
	{u8"Перервити\0\0",             L'П',44,NULL},
	{u8"обрІзати\0\0",              L'І',45,NULL},
	{u8"подовЖити\0\0",             L'Ж',46,NULL},
	{u8"Філе\0\0",                  L'Ф',47,NULL},
	{u8"фАска\0\0",                 L'А',48,NULL},
	{u8"офСетний\0\0",              L'С',49,NULL},
	{u8"подіЛяти\0\0",              L'Л',50,NULL},
	{u8"@ скасувати\0\0",L'@',51,NULL},
    {u8"плагіНи",                   L'Н',534,&mInsetEdit},
};

POLE pmRysuj[] =
{
	{u8"Лінія",                 L'Л',13,NULL},
	{u8"Коло",                  L'К',14,NULL},
	{u8"дУга",                  L'У',15,NULL},
	{u8"Текст",                 L'Т',16,NULL},
	{u8"Мультйрядковий текст",  L'М',408,NULL},
	{u8"баГатокутник",          L'Г',17,NULL},
	{u8"Диск",                  L'Д',18,NULL},
	{u8"Еліптичний",            L'Е',705,&mElliptical},
	{u8"Полилінія",             L'П',20,NULL},
	{u8"Солід",                 L'С',21,NULL},
	{u8"крива Без'є",           L'Б',516,NULL},
	{u8"пРямокутник",           L'Р',22,NULL},
	{u8"слід",                  L'=',23,NULL},
	{u8"2-лінія",               L'2',24,NULL},
	{u8"3-лінія",               L'3',25,NULL},
	{u8"ескіЗ",                 L'З',26,NULL},
	{u8"тОчка",                 L'О',27,NULL},
    {u8"Вектор",                L'В',722,NULL},
	{u8"діаграма Функцій",      L'Ф',28,NULL},
	{u8"плагіНи",               L'Н',534,&mInsetDraw},

};

POLE pmBlok[] = {
	{u8"Копія блок\0\0 ",           L'К',58,NULL},
	{u8"Вставити блок\0\0 ",        L'В',59,NULL},
	{u8"Експортувати блок\0\0 ",    L'Е',61,NULL},
	{u8"Імпортувати блок\0\0 ",     L'І',60,NULL},
	{u8"відкрити катаЛог\0\0 ",     L'Л',460,NULL}, //???
	{u8"іМпортувати блок DXF\0\0 ", L'М',84,NULL},
	{u8"вставити Образ\0\0 ",       L'О',62,&mInsertPCX},
	{u8"єдитуй Блок образу\0\0 ",   L'Б',63,&mEditPCX},
	{u8"змінити шаблон Штрихування\0\0",L'Ш',599,NULL},
	{u8"ексПортувати образ\0\0 ",   L'П',64,NULL},
	{u8"Згрупувати в блок\0\0 ",    L'З',65,NULL},
	{u8"Розбити блок\0\0 ",         L'Р',66,NULL},
    {u8"Чотирикутне перетворення\0\0",L'Ч',682,NULL},
	{u8"Список\0\0 ",               L'С',67,NULL},
	{u8"* розбити заморожений блок\0\0", L'*',68,NULL},
	{u8"виДалити блоки DXF\0\0 ",   L'Д',69,NULL},
	{u8"випрАвити блоки\0\0 ",      L'А',70,NULL},
	{u8"Фіксовані шари\0 Н\0   ",   L'Ф',71,&mSaveLayer},
    {u8"плагіНи",                   L'Н',534,&mInsetBlock},
	};

POLE  pmGeometria[] = {
	{u8"Область поверхні",              L'О',72,NULL},
	{u8"Центр області",            L'Ц',73,NULL},
	{u8"Статичний момент поверхні",      L'F',74,NULL},     
	{u8"Момент інерції",    L'М',75,NULL},
	{u8"центр. момент Інерції",L'І',76,NULL},
	{u8"Тест закритої зони",       L'Т',77,NULL},
};


char _YES_[4] = "Т";
char _NO_[4] = "Н";

const char *_EDIT_[] = { u8"Рухатити", u8"Рухатити Z", u8"Обертати", u8"Обертати XZ", u8"Обертати YZ", u8"Масштаб", u8"Дзеркало", u8"->пропустити", u8"-----"};

//char* loading_program[] = { u8"Програма завантаження", u8"Завантаження ресурсів..." , u8"Будь ласка, зачекайте. Завантаження файлу: ", u8"Підключення до хмари..."};

#define _NO_MOUSE_ u8"Миша не встановлена !!!!!\n"

#endif

#ifdef __O_WYJ__

#ifdef LINUX
#define ALF_PROGRAM "AlfaCADUA"
#define ALF_TITLE "AlfaCADUA4Linux"
#else
#ifdef BIT64
#define ALF_PROGRAM "AlfaCADUA4Win64.exe"
#define ALF_TITLE "AlfaCADUA4Win64"
#else
#define ALF_PROGRAM "AlfaCADUA4Win.exe"
#define ALF_TITLE "AlfaCADUA4Win"
#endif
#endif

char err_message[] = u8"Помилка обробки файлу DWG";
char err_message_code[] = u8"Код виходу:";
char err_message1[] = u8"Помилка обробки проміжного файлу.";
static char confirm[] = u8"Підтвердьте";

POLE pmNewDrawing[] = {
	{u8"Новий креслення",L'Н',79,NULL},
	{u8"нове з Шаблону",L'Ш',533,NULL},
};

static TMENU mNewDrawing = { 2,0,0,32,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,18,0,0,0, (POLE(*)[]) &pmNewDrawing,NULL,NULL };

POLE pmNewWindow[] = {
	{u8"Новий креслення в новому вікні",L'Н',79,NULL},
	{u8"новий із Шаблону в новому вікні",L'Ш',533,NULL},
	{u8"Відкрити файл у новому вікні",L'В',78,NULL},
};

static TMENU mNewWindow = { 3,0,0,32,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmNewWindow,NULL,NULL };


POLE pmExportCAD[] = {
	{u8"DXF",L'X',83,NULL},
	{u8"DWG",L'W',506,NULL},
};

static TMENU mExportCAD = { 1,0,0,16,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,14,0,0,0,(POLE(*)[]) &pmExportCAD,NULL,NULL };


POLE pmImportCAD[] = {
	{u8"DXF",L'Д',84,NULL},
	{u8"DWG",L'В',507,NULL},
};

static TMENU mImportCAD = { 2,0,0,16,20,7,TADD | ICONS,CMNU,CMBR,CMTX,0,16,0,0,0,(POLE(*)[]) &pmImportCAD,NULL,NULL };

POLE pmTree[] = {
	{u8"Вхід",L'В',537,NULL},
	{u8"Повернення",L'П',538,NULL},
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
	{u8"Й   \0 \0",L'Й',0,NULL},
	{u8"Ц   \0 \0",L'Ц',0,NULL},
	{u8"У   \0 \0",L'У',0,NULL},
	{u8"К   \0 \0",L'К',0,NULL},
	{u8"Е   \0 \0",L'Е',0,NULL},
	{u8"Н   \0 \0",L'Н',0,NULL},
};

static TMENU mLastFiles = { 16,0,0,33,20,7,TADD,CMNU,CMBR,CMTX,0,25,0,0,0,(POLE(*)[]) &pmLastFiles,NULL,NULL };

POLE pmQuit[] = {
	 {u8"Відкритий креслення\0",L'В',78,NULL},
	 {u8"новий Креслення\0",L'К',79,&mNewDrawing},
	 {u8"Нове вікно\0",L'Н',80,&mNewWindow},
	 {u8"Зберегти\0",L'З',81,NULL},
	 {u8"зберегти Як\0",L'Я',82,NULL},
	 {u8"Структура дерева\0",L'С',536,&mTree},
	 {u8"Експорт\0",L'Е',504,&mExportCAD},
	 {u8"Імпорт\0",L'І',505,&mImportCAD},
	 {u8"редагування Тега креслення\0", L'Т',85, NULL},
	 /* {u8"Jump to system\0",'J',86,NULL},*/
	 {u8"Друкування\0",L'Д',87,NULL},
	 {u8"креслення на Плотері\0",L'П',394,NULL},
	 {u8"Інформація\0",L'?',88,NULL},
	 {u8"показати Робочий стіл\0", L'Р',528, NULL},
	 {u8"Останні файли\0",L'О',89,&mLastFiles}
};

static POLE pmTTF_OTF[] = {
   {u8"шрифту TrueType",L'Т',703,NULL},
   {u8"шрифту OpenType",L'О',704,NULL},
};

static TMENU mTTF_OTF = { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmTTF_OTF,NULL,NULL };

static POLE pmCzcionkaEkranTTF[] = {
   {u8"Тип шрифту  \0", L'Т',111,&mTTF_OTF},
   {u8"Висота\0\0", L'В',179,NULL},
   {u8"Коефіцієнт ширини\0\0", L'К',230,NULL},
};

TMENU mCzcionkaEkranTTF = { 3,0,0,30,20,7,ICONS | TADD,CMNU,CMBR,CMTX,0,30,0,0,0,(POLE(*)[]) &pmCzcionkaEkranTTF,NULL,NULL };  //26

static POLE pmWindow[] = {
   {u8"розгорнути по Горизонталі",L'Г',467,NULL},
   {u8"розгорнути по Вертикалі",L'В',468,NULL},
   {u8"розгорнути по Діагоналі",L'Д',469,NULL},
   {u8"Розгорнути вікно",L'Р',470,NULL},
   {u8"відновити Останній",L'О',471,NULL},
};

static TMENU mWindow = { 5,0,0,16,22,9,ICONS,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmWindow,NULL,NULL };  //9

static POLE pmDialogCursor[] = {
   {u8"Маленький",L'М',591,NULL},
   {u8"Великий",L'В',592,NULL},
   {u8"Колосальний",L'К',592,NULL},
};

static TMENU mDialogCursor = { 3,0,0,8,23,9,ICONS,CMNU,CMBR,CMTX,0,25,0,0,0,(POLE(*)[]) &pmDialogCursor,NULL,NULL }; //22

#define smallcursor u8"Маленький"
#define bigcursor u8"Великий"
#define hugecursor u8"Колосальний"

static POLE pmMenuCursor[] = {
   {u8"Брус",L'Б',821,NULL},
   {u8"Курсор",L'К',822,NULL},
};

static TMENU mMenuCursor = { 2,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,35,0,0,0,(POLE(*)[]) &pmMenuCursor,NULL,NULL };  //22

#define barstyle u8"Брус"
#define cursorstyle u8"Курсор"

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

static TMENU mTranslucency = { 8,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,17,0,0,0,(POLE(*)[]) &pmTranslucency,NULL,NULL };  //14

static TMENU mDemoSelect = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,28,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };  //24

static TMENU mAutoPan = { 2,0,0,7,33,11,ICONS,CMNU,CMBR,CMTX,0,33,0,0,0,(POLE(*)[]) & pmTak_Nie,NULL,NULL };  //new

static POLE pmMouseWheel[] = {
   {u8"Натуральне",L'Н',834,NULL},
   {u8"Звичайне",L'З',835,NULL},
};

static TMENU mMouseWheel = { 2,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,39,0,0,0,(POLE(*)[]) &pmMouseWheel,NULL,NULL };  //22

POLE pmOpcje[] = {
   {u8"конфігурація Кольорів\0",L'К',109,NULL},
   {u8"авто-Панорамування\0Т\0",L'П',145, &mAutoPan},  //PAN
   {u8"приріст Авто-панорами\0",L'А',110,NULL},
   {u8"Непрозорість заливок\0",L'Н',542,&mTranslucency},
   {u8"Шрифт меню\0",L'Ш',111,&mCzcionkaEkranTTF},
   {u8"Фон\0",L'Ф',527,NULL},
   {u8"Діалоговий Курсор\0",L'Д',590,&mDialogCursor},
   {u8"стиль Меню\0",L'М',820,&mMenuCursor},
   {u8"колесо миші\0",L'Х',833,&mMouseWheel},
   {u8"демо Режим\0Н\0",L'Р',661,&mDemoSelect},
   {u8"Зберегти налаштування вікна\0",L'З',530,NULL},
   {u8"налаштування Вікна\0",L'В',478,&mWindow},
};

#define _NATURAL__ u8"Натуральне"
#define _REGULAR__ u8"Звичайне"

#define _FILE_ u8"Файл "
#define _FILE_EXISTS_ u8" існує. Перезаписати це? (Так/Ні/Esc):"

#define _YES_NO_ESC_ u8"ТтНн\033"
#define _YES_ L'Т'
#define _yes_ L'т'
#define _NO_ L'Н'
#define _no_ L'н'
#define _YES__ u8"Т"
#define _NO__ u8"Н"
#define _INTERRUPT_ u8"Блок '%s' не існує! Друк перервано. "
#define _SPOOLED_INTERRUPT_ u8"Помилка запису буферного файлу! Друк перервано."
#define _FORMAT_INTERRUPT_ u8"Немає блоку '*FORMAT'. Друк припинено"
#define _PAGE_INTERRUPT_ u8"Немає блоку '*PAGE'. Друк припинено"
#define _CONVERSION_TERMINATION_ u8"ЗАЧЕКАЙТЕ!!! Перетворення файлу. Припинення -> Esc..."
#define _ERROR_DXF_ u8"Помилка у файлі DXF або припинення "
#define _LINE_NO_ u8" -> лінія %#ld"

#define _TO_MANY_WIN_ u8"Забагато відкритих вікон"
#define _CLOSE_WINDOW_ u8"Ви хочете закрити вікно ?"

#define _DRAWING_NOT_SAVED_ u8"Креслення не збережено"

#define _Yes_ u8"Так"
#define _No_ u8"Ні"

#define _SOME_OPEN_DRAWING_NOT_SAVED_ u8"Деякі відкриті малюнки не зберігаються"

#define _SAVE_IT_ u8"Збережи це ? "
#define _SAVE_THEM_ u8"Збережіть їх ? "

#define _NO_MORE_DRAWINGS_ u8"Креслень більше немає"

#define _NEW_DRAWING_ u8"Новий креслення"

#define _SAVING_CONF_ u8"Збереження конфігурації";

#define _WANT_EXIT_ u8"Ви хочете вийти з програми?"

#define _WANT_CLOSE_WINDOW_ u8"Ви хочете закрити вікно?"

#define _SAVING_PREFILES_ "Збереження ІСТОРІЇ"

#define _FONT_CHANGED_ u8"Шрифт змінено"

#define desktop_tip u8"@Виберіть креслення"

#define _DEMO_MODE_ u8"У DEMO натиснуті клавіші та кнопки миші"
#define _DEMO_MODE1_ u8"відображаються внизу вікна програми"

#endif

#ifdef __ASKING__

#define _Yes_ u8"Так"
#define _No_ u8"Ні"

#define _NOTICE_ u8"Увага"
#define _AT_NOTICE_ u8"@Увага"
#define _COMBINATION_  u8"Номер комбінації навантаження"
#define _GEOM_STIFFNESS_ u8"Геометрична жорсткість"
#define _INERTIA_ u8"Включити постійне навантаження (DL) у вібрації"
#define _VIBRATIONS_ u8"Кількість динамічних режимів вібрації"
#define _PINNABLE_ u8"Використовуйте замінники штифтових з'єднань як віртуальні елементи з майже нульовою жорсткістю \n(безпечна альтернатива обчисленням, особливо для динамічних аналізів)"

#define _INSTALATION_ u8"Установка"

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

char instruct_txt[][128]={u8"Відкрити меню / Прийняти",
                         u8"Закрити меню / Вийти",
                         u8"Звільнення миші (або потрясіть її ліворуч і праворуч)",
                         u8"Допоміжне меню",
                         u8"Допоміжне контекстне меню",
                         u8"Ортогональність увімкнено/вимкнено",
                         u8"Масштабування / прокручування",
                         u8"Панорамування (рука Міккі Мауса)",
                         u8"Повторення команди",
                         u8"Вибір ручки (є ще)",
                         u8"Рука допомоги (всі гарячі клавіші)",
                         u8"(зліва) зміна розкладки клавіатури під час редагування",
                         u8"для грецьких літер і математичних символів",
                         u8"Клейкі ключі. У разі конфлікту з системні клавіші,",
                         u8"спочатку натисніть їх, відпустіть,",
                         u8"потім натисніть іншу клавішу комбінації",
                         u8"Редагування числових значень або знов редагування тексту",
                         u8"або"};

#define osnap_icon0 182
#define osnap_icon1 159
#define osnap_icon2 164
#define osnap_icon3 184
#define osnap_icon4 188
#define osnap_icon5 156

char stay_string[64]=u8"Більше не показувати";

#define _Yes_ u8"Так"
#define _No_ u8"Ні"

#define _INSTRUCTION_ u8"Інструкція"
#define _AT_INSTRUCTION_  u8"@Інструкція"

#endif

#ifdef __DIALOG__

#define _OTHER_ u8"֍інший>>>"
#define _LAYERS_ u8"Шари"

#endif

#ifdef __ADDMENU__

#define MAXPLACEMARKER 5

#define _YES_ u8"Т"
#define _NO_ u8"Н"

static char ggsel[10][36] = { u8"Кінець", u8"Найближчий кінець", u8"Медіана", u8"Перетин", u8"Центр", u8"пЕрпендикулярний",
u8"Дотичній", u8"Суміжний", u8"Точка","----" };

POLE pmKolor[] = {
	/*1*/ {u8"֍[А] червоний",L'А',0,NULL}, /*12*/
	/*2*/ {u8"֍[Б] жовтий",L'Б',0,NULL}, /*14*/
	/*3*/ {u8"֍[В] зелений",L'В',0,NULL}, /*10*/
	/*4*/ {u8"֍[Г] бірюза" ,L'Г',0,NULL}, /*11*/
	/*5*/ {u8"֍[Ґ] синій",L'Ґ',0,NULL}, /*9*/
	/*6*/ {u8"֍[Д] пурпурний",L'Д',0,NULL}, /*13*/
	/*7*/ {u8"֍[Е] білий",L'Е',0,NULL}, /*15*/
	/*8*/ {u8"֍[Є] темно-сірий",L'Є',0,NULL}, /*8*/
	/*9*/ {u8"֍[Ж] темно-червоний",L'Ж',0,NULL}, /*4*/
	/*10*/ {u8"֍[З] коричневий",L'З',0,NULL}, /*6*/
	/*11*/ {u8"֍[И] темно-зелений",L'И',0,NULL}, /*2*/
	/*12*/ {u8"֍[І] темно-бірюзовий",L'І',0,NULL}, /*3*/
	/*13*/ {u8"֍[Ї] темно-синій",L'Ї',0,NULL}, /*1*/
	/*14*/ {u8"֍[Й] темно-пурпуровий",L'Й',0,NULL}, /*5*/
	/*15*/ {u8"֍[К] сірий",L'К',0,NULL}, /*7*/
	/*16*/ {u8"֍[Л] чорний",L'Л',0,NULL}, /*0*/
	/*17*/ {u8"֍[М] інший\0 0\0 ",L'М',0,NULL}
};

POLE pmKolorL_[] = {
	/*1*/ {u8"֍[А] червоний",L'А',0,NULL}, /*12*/
	/*2*/ {u8"֍[Б] жовтий",L'Б',0,NULL}, /*14*/
	/*3*/ {u8"֍[В] зелений",L'В',0,NULL}, /*10*/
	/*4*/ {u8"֍[Г] бірюза" ,L'Г',0,NULL}, /*11*/
	/*5*/ {u8"֍[Ґ] синій",L'Ґ',0,NULL}, /*9*/
	/*6*/ {u8"֍[Д] пурпурний",L'Д',0,NULL}, /*13*/
	/*7*/ {u8"֍[Е] білий",L'Е',0,NULL}, /*15*/
	/*8*/ {u8"֍[Є] темно-сірий",L'Є',0,NULL}, /*8*/
	/*9*/ {u8"֍[Ж] темно-червоний",L'Ж',0,NULL}, /*4*/
	/*10*/ {u8"֍[З] коричневий",L'З',0,NULL}, /*6*/
	/*11*/ {u8"֍[И] темно-зелений",L'И',0,NULL}, /*2*/
	/*12*/ {u8"֍[І] темно-бірюзовий",L'І',0,NULL}, /*3*/
	/*13*/ {u8"֍[Ї] темно-синій",L'Ї',0,NULL}, /*1*/
	/*14*/ {u8"֍[Й] темно-пурпуровий",L'Й',0,NULL}, /*5*/
	/*15*/ {u8"֍[К] сірий",L'К',0,NULL}, /*7*/
	/*16*/ {u8"֍[Л] чорний",L'Л',0,NULL}, /*0*/
	/*17*/ {u8"֍[М] інший\0 0\0 ",L'М',0,NULL}
};

POLE pmKolorT_[] = {
	/*1*/ {u8"֍[A] червоний",L'A',0,NULL}, /*12*/ //֍червоний
	/*2*/ {u8"֍[Б] жовтий",L'Б',0,NULL}, /*14*/
	/*3*/ {u8"֍[В] зелений",L'В',0,NULL}, /*10*/
	/*4*/ {u8"֍[Г] бірюза" ,L'Г',0,NULL}, /*11*/
	/*5*/ {u8"֍[Ґ] синій",L'Ґ',0,NULL}, /*9*/
	/*6*/ {u8"֍[Д] пурпурний",L'Д',0,NULL}, /*13*/
	/*7*/ {u8"֍[Е] білий",L'Е',0,NULL}, /*15*/
	/*8*/ {u8"֍[Є] темно-сірий",L'Є',0,NULL}, /*8*/
	/*9*/ {u8"֍[Ж] темно-червоний",L'Ж',0,NULL}, /*4*/
	/*10*/ {u8"֍[З] коричневий",L'З',0,NULL}, /*6*/
	/*11*/ {u8"֍[И] темно-зелений",L'И',0,NULL}, /*2*/
	/*12*/ {u8"֍[І] темно-бірюзовий",L'І',0,NULL}, /*3*/
	/*13*/ {u8"֍[Ї] темно-синій",L'Ї',0,NULL}, /*1*/
	/*14*/ {u8"֍[Й] темно-пурпуровий",L'Й',0,NULL}, /*5*/
	/*15*/ {u8"֍[К] сірий",L'К',0,NULL}, /*7*/
	/*16*/ {u8"֍[Л] чорний",L'Л',0,NULL}, /*0*/
	/*17*/ {u8"֍[М] інший\0 0\0 ",L'М',0,NULL}
};

TMENU mKolorAC = { MAXCOLOR,MAXWCOLOR,MAXWCOLOR,23,56,5,0,CMNU,CMBR,CMTX,15,72,0,0,0,(POLE(*)[]) &pmKolor,NULL,NULL };

#define _OTHER_NO_ u8"֍[М] інше>>> %#ld"

static POLE pmKrok_k[9] = {
	 {u8"[А]\0  2.5  *",L'А',0,NULL},
	 {u8"[Б]\0  1.25 *",L'Б',0,NULL},
	 {u8"[В]\0  0.25 *",L'В',0,NULL},
	 {u8"[Г]\0  0.5",L'Г',0,NULL},
	 {u8"[Ґ]\0  1",L'Ґ',0,NULL},
	 {u8"[Д]\0  5",L'Д',0,NULL},
	 {u8"[Е]\0 10",L'Е',0,NULL},
	 {u8"[Є]\0 X\0     ",L'Є',0,NULL},
	 {u8"Мінімальний\0",L'М',0,NULL} };

static TMENU mKrok_k = { 9,0,0,13,52 ,6,TADD,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmKrok_k,NULL,NULL };

POLE pmTak_Nie[2] = {
	{u8"Так",L'Т',112,NULL},
	{u8"Ні",L'Н',113,NULL} };

POLE pmGruboscLinii[] = {
	   {u8"҂Дуже тонка",L'Д',0,NULL},
	   {u8"҂тоНка",L'Н',0,NULL},
	   {u8"҂Товста",L'Т',0,NULL},
	   {u8"҂дуже товстА",L'А',0,NULL},
	   {u8"҂Супер товста",L'С',0,NULL},
};

POLE pmGruboscLiniiS[] = {
       {u8"Заповнення",L'З',0,NULL},
	   {u8"҂Дуже тонка",L'Д',0,NULL},
	   {u8"҂тоНка",L'Н',0,NULL},
	   {u8"҂Товста",L'Т',0,NULL},
	   {u8"҂дуже товстА",L'А',0,NULL},
	   {u8"҂Супер товста",L'С',0,NULL},
};

static POLE pmTypyLinii[] = {
		{u8"֏безперервна",L'1',0,NULL},
		{u8"֏пунктирна",L'2',0,NULL},
		{u8"֏штрихпунктирна",L'3',0,NULL},
		{u8"֏2-крапкова",L'4',0,NULL},
		{u8"֏крапкова",L'5',0,NULL},
		{u8"֏кордон",L'6',0,NULL},
		{u8"֏центр",L'7',0,NULL},
		{u8"֏прихована",L'8',0,NULL},
		{u8"֏фантом",L'9',0,NULL},
		{u8"֏пунктирна_2",L'А',0,NULL},
		{u8"֏штрихпунктирна_2",L'Б',0,NULL},
		{u8"֏2-крапкова_2",L'В',0,NULL},
		{u8"֏крапкова_2",L'Г',0,NULL},
		{u8"֏кордон_2",L'Ґ',0,NULL},
		{u8"֏центр_2",L'Д',0,NULL},
		{u8"֏прихована_2",L'Е',0,NULL},
		{u8"֏фантом_2",L'Є',0,NULL},
		{u8"֏пунктирна_x2",L'Ж',0,NULL},
		{u8"֏штрихпунктирна_x2",L'З',0,NULL},
		{u8"֏2-крапкова_x2",L'И',0,NULL},
		{u8"֏крапкова_x2",L'І',0,NULL},
		{u8"֏кордон_x2",L'Ї',0,NULL},
		{u8"֏центр_x2",L'Й',0,NULL},
		{u8"֏прихована_x2",L'К',0,NULL},
		{u8"֏фантом_x2",L'Л',0,NULL},
		{u8"֏3-крапкова",L'М',0,NULL},
		{u8"֏2-центр",L'Н',0,NULL},
		{u8"֏3-пунктирна",L'О',0,NULL},
		{u8"֏фантом_3",L'П',0,NULL},
		{u8"֏2-центр_x2",L'Р',0,NULL},
		{u8"֏3-пунктирна_x2",L'С',0,NULL},
		{u8"֏багатоштрихова",L'Т',0,NULL},
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
	 {u8"[А]\0 2.5 (*)",L'А',0,NULL},
	 {u8"[Б]\0 1",L'Б',0,NULL},
	 {u8"[В]\0 5",L'В',0,NULL},
	 {u8"[Г]\0 10",L'Г',0,NULL},
	 {u8"[Ґ]\0 100",L'Ґ',0,NULL},
	 {u8"[Д]\0 X\0    ",L'Д',0,NULL} };
static TMENU mGestosc = { 6,0,0,13,52,7,TADD,CMNU,CMBR,CMTX,0,21,0,0,0,(POLE(*)[]) &pmGestosc,NULL,NULL };


POLE pmParametry[] = {
	 {u8"Крок вказівника\0",        L'К',154,&mKrok_k},   /*.25*/
	 {u8"Щільність сітки\0",        L'Щ',155,&mGestosc }, /*2.5*/
	 {u8"точки Сітки\0",               L'С',156,&mPunkty_siatki},
	 {u8"Ортогональність (F8)\0",            L'О',157,&mOrto},
	 {u8"֎колІр\0білий\0",     L'І',158,&mKolorAC},   /*Biały*/
	 {u8"Товщина лінії\0Товста\0",  L'Т',159,&mGruboscLinii},
	 {u8"тип Лінії\0безперервна\0", L'Л',160,&mTypyLinii},   /*Ciągła*/
	 {u8"точки та контури Видимість \0Т\0",  L'В', 161,&mPoint_View},
	 {u8"Розмір точки\02.5\0",      L'Р',162,NULL},
	 {u8"отримАти колір  (Alt-F8)\0",          L'А',163,NULL},
	 {u8"отримати тип    (Alt-F9)\0",          L'Г',164,NULL},
	 {u8"отримати шар    (Alt-F10)\0",         L'Ґ',165,NULL},
	 {u8"вільний Блок    \0",             L'Б',167,&mSwobodnyBlok},
	 {u8"вільний тЕкст * \0",             L'Е',166,&mSwobodnyTekst},
	 {u8"Масштабування блоку розмірів \0N\0", L'М',168,&mScale_DIM},
	 {u8"розтягування ліній у вузлаХ \0T\0",L'Х',169,&mCiagniecieLinii},
	 {u8"розрИвні лінії у вузлах \0T\0",L'И',170,&mRozcinanieLinii},
	 {u8"Приховування типів \0T\0",            L'П',171,&mUkrywanieTypu},
	 {u8"виДимість прихованого тексту \0N\0",L'Д',172,&mWidocznoscTypu},
	 {u8"Заповнення точок зображення \0N\0",   L'З',173,&mWypelnianiePCX},
	 {u8"виділіть поточний Шар \0N\0",L'Ш',174,&mView_Only_C_L},
	 {u8"буФеризація зображення \0T\0",           L'Ф',175,&mBuffering_PCX},
	 //  {u8"видимість листових блоків \0N\0",L' ',0,&mView_Sheets},
	   {u8"Нормалізація тексту \0",           L'Н',176,&mNormalizacjaTekstu},
};

TMENU mParametrym = { 23,0,0,38,23,4,ICONS | TADD,CMNU,CMBR,CMTX,0,59,0,0,0,(POLE(*)[]) &pmParametry,NULL,NULL };

POLE pmParametry_view[] = {
	 {u8"Крок вказівника\0",        L'К',154,&mKrok_k},   /*.25*/
	 {u8"Щільність сітки\0",        L'Щ',155,&mGestosc }, /*2.5*/
	 {u8"Точки сітки\0",               L'Т',156,&mPunkty_siatki},
	 {u8"Видимість точок\0 Т\0",  L'В', 161,&mPoint_View},
	 {u8"Розмір точки\0 2.5\0",      L'Р',162,NULL},
	 {u8"видимість Прихованого тексту\0 Н\0",L'П',172,&mWidocznoscTypu},
	 {u8"виділіть поточний Шар\0 Н\0",L'Ш',174,&mView_Only_C_L},
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
	 {u8"Оновити\0",L'О',142,NULL},
	 {u8"Зберегти перегляд\0", L'З', 140,&mSetMarker},
	 {u8"знайти Перегляд\0", L'П', 141,&mFindMarker},
	 {u8"Усе\0",L'У',143,NULL},
	 {u8"Центр\0",L'Ц',144,NULL},
	 //{u8"Автопанорамування\0Так\0     ",L'А',145, &mAutoPan},
	 {u8"Панорамування (Ctrl-Shift)\0",L'П',145, NULL},
	 {u8"[1] *1.0 \0",L'1',146,NULL},
	 {u8"[5] *0.5\0",L'5',147,NULL},
	 {u8"[2] *0.2\0",L'2',148,NULL},
	 {u8"[0] *0.1\0",L'0',149,NULL},
	 {u8"*x\0",L'*',150,NULL},
	 {u8"Вікно\0",L'В',151,NULL},
	 {u8"Креслярський\0",L'К',152,NULL},
	 {u8"останній\0",L'T',153,NULL},
	 {u8"перейти До x;y\0",L'Д',129,NULL},
	 {u8"рухатися На dx;dy\0",L'Н',130,NULL},
	 {u8"рухатися поЛярно L;a\0",L'Л',131,NULL},
}; /*rownolegle nalezy modyfikowac funkcje SkalaZ  (1.,.5,.2,.1)*/

TMENU mSkala = { 17,0,0,20,12,4,ICONS | TADD,CMNU,CMBR,CMTX,0, 30, 0,0,0,(POLE(*)[]) &pmSkala,NULL,NULL };



static POLE pmTryb[] = {
	{u8"Кінець\0",L'К',120,NULL},
	{u8"Найближчий кінець\0", L'Н', 121, NULL}, //
	{u8"Медіана\0",L'М',122,NULL },
	{u8"Перетин\0",L'П',123,NULL },
	{u8"Центр\0",L'Ц',124,NULL },
	{u8"пЕрпендикулярний\0",L'Е',125,NULL },
	{u8"Дотичній\0",L'Д',126,NULL },
	{u8"Суміжний\0",L'С',127,NULL },
	{u8"Точка\0",L'Т',128,NULL },
	{u8"Видалити\0",L'В',132,NULL } }; //

TMENU mTryb = { 10,0,0,20,20,6,ICONS,CMNU,CMBR,CMTX,0, 13, 0,0,0,(POLE(*)[]) &pmTryb,NULL,NULL };

static POLE pmPunkt[] = {
	{u8"Режим повторення \0",L'Р',119,&mTryb},
	{u8"Кінець\0",L'К',120,NULL},
	{u8"Найближчий кінець\0",L'Н',121,NULL},
	{u8"Медіана\0",L'М',122,NULL},
	{u8"Перетин\0",L'П',123,NULL},
	{u8"Центр\0",L'Ц',124,NULL},
	{u8"пЕрпендикулярний\0",L'Е',125,NULL},
	{u8"Дотичній\0",L'Д',126,NULL},
	{u8"Суміжний\0",L'С',127,NULL},
	{u8"Точка\0",L'Т',128,NULL},
	{u8"перейти В x;y\0",L'В',129,NULL},
	{u8"рухатися За dx;dy\0",L'З',130,NULL},
	{u8"рухатися поЛярно L;a\0",L'Л',131,NULL}, };

TMENU mPunkt = { 13,0,0,30,1,4,ICONS | TADD,CMNU,CMBR,CMTX,0, 7, 0,0,0,(POLE(*)[]) &pmPunkt,NULL,NULL };


static POLE pmSum[] = {
	 {u8"Припиніть додавати\0 F10",L'П',139,NULL},
	 {u8"Додати/відняти\0 F9",L'Д',137,NULL},
};

static TMENU mSum = { 2, 0,0,23,56,9,ICONS | TADD,CMNU,CMBR,CMTX,0, 7, 0, 0,0,(POLE(*)[]) &pmSum, NULL,NULL };

POLE pmMeasure[] = {
	{u8"кАлькулятор", L'А', 133,NULL},
	{u8"Точка",      L'Т',134,NULL},
	{u8"Відстань",   L'В',135,NULL},
	{u8"Кут",      L'К',136,NULL},
	{u8"Сума",    L'С', 137,&mSum},
	{u8"Характеристики", L'Х', 138,NULL},
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
	{u8"Точка\0\0",L'Т',52, &mPunkt},
	{u8"Вид\01\0",L'В',53, &mSkala},
	{u8"Налаштування\0\0",L'Н',54,&mParametrym},
	{u8"Шари\0\0",L'Ш',55,NULL},
	{u8"Міряти, інформація та зміни\0\0", L'М',56, &mMeasure},
	{u8"Знайти\0\0",L'З',3,NULL},
    {u8"Плагіни\0\0",L'П',534,&mInsetAux},
	{u8"\0\0",L' ',0,NULL} };

TMENU menup = { 8,0,0,12,1,2,ICONS | TADD | NVERT,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmenup,NULL,NULL };   //7,0,11,

#endif

#ifdef __LAYERS__

#define NoWidthLines 5
#define NoTypeLines	32
static char* width_line_txt_L[NoWidthLines + 1] = { u8"҂дуже тонка",u8"҂тонка",u8"҂товста",u8"҂дуже товста",u8"҂супер товста",u8"" };

static char* Colors_Name_txt_L[MAXCOLOR + 1] =
{
	u8"֍червоний" ,
	u8"֍жовтий" ,
	u8"֍зелений" ,
	u8"֍бірюза",
	u8"֍блакитний",
	u8"֍пурпурний",
	u8"֍білий",
	u8"֍темно-сірий" ,
	u8"֍темно-червоний" ,
	u8"֍коричневий" ,
	u8"֍темно-зелений" ,
	u8"֍темно-бірюзовий" ,
	u8"֍темно-синій" ,
	u8"֍темно-пурпурний" ,
	u8"֍сірий" ,
	u8"֍чорний" ,
	u8"֍інший>>>",
	u8"",
};

static char* type_line_txt_L[NoTypeLines + 1] = {
	u8"֏безперервна",
	u8"֏пунктирна",
	u8"֏штрихпунктирна",
	u8"֏2-крапкова",
	u8"֏Крапкова",
	u8"֏кордон",
	u8"֏центр",
	u8"֏прихована",
	u8"֏фантом",
	u8"֏пунктирна_2",
	u8"֏штрихпунктирна_2",
	u8"֏2-крапкова_2",
	u8"֏крапкова_2",
	u8"֏кордон_2",
	u8"֏центр_2",
	u8"֏прихованф_2",
	u8"֏фантом_2",
	u8"֏пунктирна_x2",
	u8"֏штрихпунктирна_x2",
	u8"֏2-крапкова_x2",
	u8"֏крапкова_x2",
	u8"֏кордон_x2",
	u8"֏центр_x2",
	u8"֏прихована_x2",
	u8"֏фантом_x2",
	u8"֏3-крапкова",
	u8"֏2-центр",
	u8"֏3-пунктирна",
	u8"֏фантом_3",
	u8"֏2-центр_x2",
	u8"֏3-пунктирна_x2",
	u8"֏багатоштрихова",
	u8"",
};

#define layer_comm0 u8"PgUp"
#define layer_comm1 u8"PgDn"
#define layer_comm2 u8"Новий"
#define layer_comm3 u8"OK"
#define layer_comm4 u8"Esc"
#define layer_comm5 u8"+"
#define layer_comm6 u8"+"
#define layer_comm7 u8"+"
#define layer_comm8 u8"-"
#define layer_comm9 u8"-"
#define layer_comm10 u8"-"
#define layer_comm11 u8"+"
#define layer_comm12 u8"+"
#define layer_comm13 u8"-"
#define layer_comm14 u8"-"
#define layer_comm15 u8"Поточний"
#define layer_comm16 u8"Назва шару"
#define layer_comm17 u8""
#define layer_comm18 u8"Видимо"
#define layer_comm19 u8"Можна редагувати"
#define layer_comm20 u8"Точний"
#define layer_comm21 u8"Колір"
#define layer_comm22 u8"Лінія"
#define layer_comm23 u8"Товщина"
#define layer_comm24 u8"Лінія"
#define layer_comm25 u8"Тип"
#define layer_comm26 u8"Усі шари"
#define layer_comm27 u8"Усі шари"
#define layer_comm28 u8"Сірий"
#define layer_comm29 u8"Ч/Б"
#define layer_comm30 u8"Увімкнено"
#define layer_comm31 u8"Вимкнено"
#define layer_comm32 u8"Змінити шари"
#define layer_comm33 u8"Видалити"
#define layer_comm34 u8"Шари"
#define layer_comm35 u8"Відновити"

#define layer_tip0 u8"@Видимість"
#define layer_tip1 u8"Можливість редагування"
#define layer_tip2 u8"Локалізація"
#define layer_tip3 u8"Видимість усіх шарів"
#define layer_tip4 u8"Редагувати всіх шарів"
#define layer_tip5 u8"Локалізація всіх шарів"
#define layer_tip6 u8"Колір"
#define layer_tip7 u8"Ширина лінії"
#define layer_tip8 u8"Тип лінії"
#define layer_tip9 u8"на принтері та плоттері"
#define layer_tip10 u8"на плоттері та принтері"
#define layer_tip11 u8"Сірий на друк"
#define layer_tip12 u8"Чорно-біле на друк"
#define layer_tip13 u8"Усі шари сірі під час друку"
#define layer_tip14 u8"Усі шари чорно-білі"
#define layer_tip15 u8"Усі шари"
#define layer_tip16 u8"Стерти шар"
#define layer_tip17 u8"Позначити блок шарів"

#define _YES_NO_ESC_ u8"ТтНн\033"
#define _YES_ L'Т'
#define _yes_ L'т'
#define _NO_ L'Н'
#define _no_ L'н'
#define _YES__ u8"Т"
#define _NO__ u8"Н"
#define _Yes_ u8"Так"
#define _No_ u8"Ні"
#define _ERASE_LAYER_ u8"ПОПЕРЕДЖЕННЯ: Ви бажаєте видалити весь шар з усім його вмістом?"
#define _FROM_ u8"Від"
#define _TO_ u8"До"
#endif

#ifdef __CHPROP__

#define NoWidthLines	6 /*5*/ /*3*/
#define NoTypeLines	32/*5*/
#define NoTextType 16
#define NoTextJust 4


char* Colors_Name_txt[MAXCOLOR + 1] =
{
	u8"֍червоний" ,
	u8"֍жовтий" ,
	u8"֍зелений" ,
	u8"֍бірюза",
	u8"֍блакитний",
	u8"֍пурпурний",
	u8"֍білий",
	u8"֍темно-сірий" ,
	u8"֍темно-червоний" ,
	u8"֍коричневий" ,
	u8"֍темно-зелений" ,
	u8"֍темно-бірюзовий" ,
	u8"֍темно-синій" ,
	u8"֍темно-пурпурний" ,
	u8"֍сірий" ,
	u8"֍чорний" ,
	u8"֍інший>>>",
	u8"",
};

#define chprop_comm0 u8"Зміна властивостей об'єктів"
#define chprop_comm1 u8"Зміна"
#define chprop_comm2 "u8інші властивості"

static char NoChange[] = u8"Без змін";

static char* width_line_txt[NoWidthLines + 1] = { u8"҂дуже тонка",u8"҂тонка",u8"҂товста",u8"҂дуже товста",u8"҂супер товста", u8"҂Начинки", NoChange };

char* type_line_txt[NoTypeLines + 1] = {
	u8"֏безперервна",
	u8"֏пунктирна",
	u8"֏штрихпунктирна",
	u8"֏2-крапкова",
	u8"֏крапкова",
	u8"֏кордон",
	u8"֏центр",
	u8"֏прихована",
	u8"֏фантом",
	u8"֏пунктирна_2",
	u8"֏штрихпунктирна_2",
	u8"֏2-крапкова_2",
	u8"֏крапкова_2",
	u8"֏кордон_2",
	u8"֏центр_2",
	u8"֏прихована_2",
	u8"֏фантом_2",
	u8"֏пунктирна_x2",
	u8"֏штрихпунктирна_x2",
	u8"֏2-крапкова_x2",
	u8"֏крапкова_x2",
	u8"֏кордон_x2",
	u8"֏центр_x2",
	u8"֏прихована_x2",
	u8"֏фантом_x2",
	u8"֏3-крапкова",
	u8"֏2-центр",
	u8"֏3-пунктирна",
	u8"֏фантом_3",
	u8"֏2-центр_x2",
	u8"֏3-пунктирна_x2",
	u8"֏багатоштрихова",
	NoChange };

static char* tab_type_txt[NoTextType + 1] = {
	u8"Звичайний",
	u8"Внутрішня етикетка",
	u8"мітка компонента",
	u8"Символ",
	u8"Тип",
	u8"*ім'я шпильки",
	u8"блок живлення",
	u8"Порт",
	u8"Атрибут",
	u8"+Схема",
	u8"Ім'я файлу",
	u8"мережа",
	u8"Коментар",
	u8"%змінна",
	u8"допом.символ",
	u8"допом.опис",
	NoChange };

static char* font_just_txt[NoTextJust + 1] = {
					 u8"Ліворуч",
					 u8"Праворуч",
					 u8"Середній",
					 u8"Центральний",
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

#define chprop_tips0 u8"@Шар"
#define chprop_tips1 u8"Колір"
#define chprop_tips2 u8"Ширина лінії"
#define chprop_tips3 u8"Тип лінії"
#define chprop_tips4 u8"Тип шрифту"
#define chprop_tips5 u8"Тип тексту"
#define chprop_tips6 u8"Прихований текст"
#define chprop_tips7 u8"Вирівнювання тексту"
#define chprop_tips8 u8"Висота шрифту"
#define chprop_tips9 u8"Ширина шрифту"
#define chprop_tips10 u8"Курсив шрифт"
#define chprop_tips11 u8"Жирний шрифт"
#define chprop_tips12 u8"Підкреслений текст"
#define chprop_tips13 u8"Так"
#define chprop_tips14 u8"Ні"
#define chprop_tips15 u8"Перевернути по горизонталі"
#define chprop_tips16 u8"Перевернути вертикально"
#define chprop_tips17 u8"Зверху"
#define chprop_tips18 u8"Непрозорість кольорового заповнення"

char* add_new_font_c = u8"додати новий шрифт";

#endif

#ifdef __LOADF__

static char copy_button[] = u8"Скопіюйте\0з файлу";
static char parametry_p[] = u8"Параметри:";
static char edycja_n_p[] = u8"Назва параметра :";
static char edycja_p[] = u8"Значення параметра:";
static char lista_p[] = u8"Список параметрів:";
static char nowy_katalog[] = u8"новий каталог";
static char usun_zbior[] = u8"видалити файл";
static char usun_katalog[] = u8"видалити каталог";

char* dlg_name[] =
{
   u8"Завантажити креслення з файлу",
   u8"Зберегти креслення у файл",
   u8"Блок імпорту",
   u8"Імпортувати файл зображення",
   u8"Перетворити файл зображення в ALX та імпортувати",
   u8"Експорт PCX",
   u8"Експорт BMP",
   u8"Експорт PNG",
   u8"Експортувати JPEG",
   u8"Експортувати TIFF",
   u8"Експорт EPS",
   u8"Експорт PDF",
   u8"Імпорт профілю",
   u8"Імпортувати 3D-об'єкт",
   u8"Зберегти блок",
   u8"Перетворення креслення в новий формат",
   u8"Блок перетворення в новий формат",
   u8"Редагувати таблицю",
   u8"Перетворити креслення з формату DXF",
   u8"Зберегти креслення у форматі DXF",
   u8"Зберегти файл макро",
   u8"Завантажити файл макро",
   u8"Зберегти конфігурацію принтера",
   u8"Завантажити конфігурацію принтера",
   u8"Зберегти файл каталогу",
   u8"Завантажити файл каталогу",
   u8"Зберегти файл координат",
   u8"Зберегти запис мережевого маршруту",
   u8"Завантажити маршрут з файлу координат",
   u8"Відкритий каталог",
   u8"Зберегти конфігурацію плоттера",
   u8"Завантаження конфігурації плоттера",
   u8"Імпорт блоку DXF",
   u8"Перетворити креслення з формату DWG",
   u8"Зберегти креслення у форматі DWG",
   u8"Імпорт блоку DWG",
   u8"Завантажити шрифт робочого столу",
   u8"Завантажити новий шрифт",
   u8"Зберегти файл результатів статичного аналізу",
};

#define nameplate_tip u8"@Шильдик креслення"
#define  edit_table u8"Редагувати таблицю"

#define _BLOCK_DESCRIPTION_ u8"Опис блоку:"
#define _BLOCK_TYPE u8"Тип блоку:"

#define _Yes_ u8"Так"
#define _No_ u8"Ні"

#define _YES_ L'Т'
#define _yes_ L'т'
#define _NO_ L'Н'
#define _no_ L'н'

#define _FILE_EXISTS_ u8"Файл існує"
#define _OVERWRITE_IT_ u8"Перезаписати це ? "

#endif // __LOADF__

#ifdef __INFO__
/*
char* typ_punktu_tab[] = { u8"Простий",u8"Базова точка",u8"Штифт","?","?","?","?","?","?",
						"?","?","?","?","?","?","?" };
*/
char* vector_style_tab[] = { u8"жорсткий-жорсткий",u8"жорсткий-шарнірний",u8"шарнірний-жорсткий",u8"шарнірний-жорсткий",
                            u8"Сила",u8"Момент+",u8"Момент-",
                            u8"Переміщення", u8"Обертання+", u8"Обертання-",
                            u8"вертикальне навантаж.",
                            u8"горизонтальне навантаж.",
                            u8"перпендикулярне навантаж.",
                            u8"горизонт.проекція вертик.",
                            u8"вертик.проекція горизонт.",
                            u8"теплове навантаж.",
                            u8"розмір вузла (радіус)",
                            u8"поверхневе навантаження",
                            u8"зосереджене навантаження",
                            u8"?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
                            u8"?","?","?","?","?","?","?","?",
 };

char* view_width_tab1[] = { u8"дуже тонкий",u8"тонкий",u8"товстий",u8"дуже товстий",u8"супер товстий",u8"заповнення",u8"",u8"невидимий" };

char* tab_typ_tekstu[] = { u8"Нормальний", u8"Внутрішня мітка",u8"мІтка компонента",u8"Символ",u8"Тип",
u8"*назва контакту",u8"джерело живлення",u8"Порт",u8"Атрибут",u8"+Схема",u8"назва Файлу",u8"Мережа",
u8"Коментар",u8"%змінна",u8"Допом.символ",u8"дОпом.опис" };

char* tab_justowanie[] = { u8"ліворуч |<",u8"праворуч >|",u8"Середній ><",u8"Центральний _><_" };

static POLE pmInfoAbout[] = {
	{u8"тип Об'єкта\0",L'О',320,NULL}, //0
	{u8"Шар\0",L'Ш',305,NULL}, //1
	{u8"֎Колір\0",L'К',158,NULL}, //2
	{u8"Тип лінії\0",L'Т',160,NULL}, //3
	{u8"Ширина Лінії\0",L'Л',159,NULL}, //4
    {u8"тип опори краю\0",L'Б',850,NULL},      //4
    {u8"Інверсія опори краю\0",L'І',853,NULL},      //4
    {u8"тип\0 ",L'.',27,NULL},   //5
    {u8"тип\0 ",'>',770,NULL},      //6
	{u8"X1 \0 ",L'1',306,NULL}, //7
	{u8"Y1 \0 ",L'2',307,NULL}, //8
	{u8"X2 \0 ",L'3',308,NULL}, //9

	{u8"Y2 \0 ",L'4',309,NULL}, //10
	{u8"X3 \0 ",L'5',310,NULL}, //11
	{u8"Y3 \0 ",L'6',311,NULL}, //12
	{u8"X4 \0 ",L'7',312,NULL}, //13
	{u8"Y4 \0 ",L'8',313,NULL}, //14
	{u8"Радіус \0",L'Р',212,NULL}, //15
    {u8"Радіус Y \0",L'р',710,NULL}, //16
    {u8"Кут \0",L'@',107,NULL}, //17
	{u8"Кут 1 \0",L'(',314,NULL}, //18
	{u8"Кут 2 \0",L')',315,NULL}, //19

    {u8"Початкова ширина \0",L'-',249,NULL},      //20
    {u8"Кінцева ширина \0",L'=',249,NULL},      //21
    {u8"інтенсивність \0",'^',727,NULL},      //22
    {u8"Початкова інтенсивність \0",'(',766,NULL},      //23
    {u8"Кінцева інтенсивність \0",')',767,NULL},      //24
    {u8"Початкове зміщення осі \0",L'/',250,NULL},      //25
    {u8"Останнє зміщення осі \0",L'\\',250,NULL},      //26

    {u8"Характер і варіант\0 ",'?',798,NULL},     //27
    //{u8"Варіант навантаження \0 ",'?',799,NULL},     //28

    {u8"Непрозорість\0",'%',542,NULL},  //29
	{u8"Довжина/периметр\0",L'Д',210,NULL}, //30
	{u8"DX\0 ",L'Х',316,NULL}, //31
	{u8"DY\0 ",L'У',317,NULL}, //32
	{u8"Площа поверхні\0",L'П',7,NULL},   //33
	{u8"шриФт\0",L'Ф',111,NULL}, //34
	{u8"тИп\0",L'И',231,NULL}, //35
	{u8"приховАний\0",L'А',232,NULL}, //36
	{u8"вирівнЮвання\0",L'Ю',233,NULL}, //37
	{u8"Висота \0",L'В',179,NULL}, //38
	{u8"коефіцієнт ширини \0",L'*',230,NULL}, //39
	{u8"курСив\0",L'С',181,NULL}, //40
    {u8"Жирний\0",L'Ж',182,NULL},  //41

	{u8"підкрЕслено\0",L'Е',409,NULL},  //42
	{u8"Міжрядковий інтервал\0",L'М',410,NULL},  //43
    {u8"розмір пікселя dx \0",L'Й',689,NULL},   //44
    {u8"розмір пікселя dy \0",L'Я',690,NULL},   //45
	{u8"Назва внут. блоку\0",L'Н',318,NULL}, //46
	{u8"назва Зовн. блоку\0",L'З',319,NULL}, //47
};
TMENU mInfoAbout = { 47,0,0,40,2, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmInfoAbout, NULL, NULL };

static POLE pmInfo[] = {
	{u8"Розмір креслення КБ\0 ",L'Р',297,NULL},
	{u8"Пам'ять зображень МБ\0 ",L'В',298,NULL},
	{u8"розмір даних Креслення Б\0 ",L'К',299,NULL},
	{u8"буфер Друку КБ\0 ",L'Д',300,NULL},
	{u8"буфер Макросу [Б]\0 ",L'М',301,NULL},
	{u8"Буфер зображення КБ\0 ",L'Б',302,NULL},
	{u8"каталог \0 ",L'/',303,NULL},
	{u8"Графічний режим \0 ",L'Г',304,NULL},
	{u8"Шрифт меню \0 ",L'Ш',111,NULL},
    {u8"Лого \0 ",L'Л',812,NULL},
};

TMENU mInfo = { 10,0,0,64,1, 3, ICONS | TADD, CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmInfo, NULL, NULL, NULL };

char* objects[] = { u8"Лінія",u8"Лінія 3D",u8"Текст",u8"Дуга",u8"Коло",u8"Диск",u8"Багатокутник/суцільний",u8"Суцільний 3D",u8"Точка", u8"Сплайн",u8"Зображення",u8"Полилінія",u8"Слід",u8"Штрихування",u8"Еліптична дуга",u8"Еліпс",u8"Еліптичний диск", u8"Суцільна дуга", u8"Вектор"};

#define __PLATE__ u8" (ПЛИТА)"
#define __HOLE__  u8" (ОТВІР)"
#define __WALL__  u8" (СТІНА)"
#define __ZONE__  u8" (ЗОНА)"

#define _FILLING_ u8"Наповнення"

char *vector_txt[]={u8"Вектор: жорсткий-жорсткий",u8"Вектор: жорсткий-штифт",u8"Вектор: pin-жорсткий",u8"Вектор: pin-pin",u8"Вектор: Сила",u8"Вектор: Момент" ,u8"Вектор: -Момент",
 u8"Вектор: переміщення",u8"Вектор: обертання",u8"Вектор: -обертання",u8"Вектор: навантаження трапеції Y",u8"Вектор: навантаження трапеції X",u8"Вектор: навантаження N трапеції",u8"Вектор: трапеція H навантаження",u8"Вектор: трапеція V навантаження",u8"Вектор: Теплове навантаження", u8"Вектор: розмір Вузла", u8"Поверхневе навантаження", u8"Зосереджене навантаження"};

//char *point_txt[]={u8"Простий",u8"Базова точка",'','','','','',u8"З'єднання",u8"Термінал",'','','',u8"Жорстке кріплення",u8"жорстке кріплення Л",u8"жорстке кріплення П",u8"Жорстке кріплення В",u8"Шарнірне, нерухоме кріплення",
 //u8"шарнірне, нерухоме кріплення Л",u8"шарнірне, нерухоме кріплення П",u8"шарнірне, нерухоме кріплення В",u8"жорстке, Горизонт. ковзання",u8"жорстке, вертик. ковзання Л",
 //u8"жорстке, вертик. ковзання П",u8"жорстке, горизонт. ковзання В",u8"шарнірне, горизонт. Ковзання",u8"шарнірне, вертик. ковзання Л",u8"шарнірне, вертик. ковзання П",u8"шарнірне, горизонт. ковзання В"};

unsigned short vector_wcod[]={L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', L'У', L'Х', L'И', L'V', L'Ц', L'Ф', L'В', L'П', L'З'};
unsigned short point_wcod[]={L'П', L'Б', L' ', L' ', L' ', L' ',L' ', L'З', L'Т', ' ', ' ', ' ',L'Ж', L'1', L'2', L'3', L'Ш', L'3', L'4', L'5', L'Г', L'7', L'8', L'9', L'К', L'0', L'-', L'=', L'+'};
unsigned short object_wcod[]={L'З', L'Л', L'Т', L'Д', L'К', L'Г', L'П', L'Т', L'*', L'С', L'O', L'Р', L'Е', L'Б', L'В', ' '};
char *object_txt[]={u8"Зображення",u8"Лінія",u8"Текст",u8"Дуга",u8"Коло",u8"Диск",u8"Площа",u8"Точка",u8"Блок",u8"Суцільна дуга ",u8"Еліптична дуга",u8"Еліпс",u8"Заповнений еліпс",u8"Сплайн Безьє",u8"Вектор",""};

#endif

#ifdef __ALLEGROEXT__

static char* desktop_data_param[] =
{
  (char*)u8"Шпалери",
  (char*)u8"Панорама",
  (char*)u8"Динамічне меню",
  (char*)u8"Курсор",
  (char*)u8"Стиль меню",
  (char*)u8"Стиль колеса",
  (char*)u8"Інструкція",
};

static int no_desktop_data_param = sizeof(desktop_data_param) / sizeof(desktop_data_param[0]);

static char* desktop_data_param_comment[] =
{
	(char*)u8";",
	//";Screen saver delay in min or 0 (off)",
	(char*)u8";коефіцієнт авторозгортання робочого столу",
	(char*)u8";",
	(char*)u8"; маленький 0, великий 1, величезний 2",
    (char*)u8"; рамка 0, курсор 1",
    (char*)u8"; 0-натуральний, 1-звичайний",
    (char*)u8"; показ коротких інструкцій при запуску",
};


#define _UNABLE_INSTALL_GRAPHIC_ u8"Не вдається ініціалізувати графічний режим"
#define _ERROR_READING_DAT_ u8"Помилка читання файлу: alfacad.dat\n"

#define _MACRO_C__ L'м'
#define _EXECUTE_C__ L'в'
#define _Go_AHEAD_ u8"ДОБРЕ. Іди вперед."

#define _UPGRADE_READY_ u8"Оновлення готове до завантаження та встановлення"
#define _UPGRADE_RESTART_ u8"Ви хочете оновити та перезапустити зараз?"
#define _TOOLS_TO_INSTALL_ u8"не встановлено"
#define _INSTALL_TOOLS_ u8"Встановити необхідні пакети"

#define _YES_NO_ESC_ u8"ТтНн\033"
#define _YES_ L'Т'
#define _yes_ L'т'
#define _NO_ L'Н'
#define _no_ L'н'
#define _YES__ u8"Т"
#define _NO__ u8"Н"

char _Yes_[]=u8"Так";
char _No_[]=u8"Ні";

#endif

#ifdef __O_LINE__

static POLE pmAxis1000[] = {
		  {u8"Довжина осі\0\0",L'Д',267,NULL}};

static POLE pmAxis50[] = {
		  {u8"Довжина осі\0\0",L'Д',267,NULL} };

static POLE pmLine[] = {
		{u8"Закрити\0",L'З',217,NULL},
		{u8"Видалити\0Del\0",L'В',218,NULL},
		{u8"Довжина\0 0\0",L'Д',219,NULL} };

static POLE pmPLine[] = {
		{u8"Закрити\0",L'З',217,NULL},
		{u8"Видалити\0Del\0",L'В',218,NULL},
		{u8"Довжина\0 0\0",L'Д',219,NULL},
		{u8"дуГа\0", L'Г', 15,NULL},
        {u8"Без опори\0", L'Б', 851,NULL},
        {u8"Шарнірно-нерухома опора\0", L'Ш', 850,NULL},
        {u8"Жорстке затиснення\0", L'Ж', 852,NULL},
        //{u8"шарнірно-Рухома опора\0", L'Р', 849,NULL},
        {u8"Перевернути опору\0", L'П', 853,NULL},
};

POLE pmEdgeType[] = {
        {u8"Без опори\0", L'Б', 851,NULL},
        {u8"Шарнірно-нерухома опора\0", L'Ш', 850,NULL},
        {u8"Жорстке затиснення\0", L'Ж', 852,NULL}
        };

static POLE pmLine_Con[] = {
		  {u8"Продовження",L'П',220,NULL} };

static POLE pmPLineObrys[] = {
		  {u8"Закрити\0",L'З',217,NULL},
		  {u8"Видалити\0Del\0",L'В',218,NULL},
		  {u8"Довжина\0 0\0",L'Д',219,NULL},
		  {u8"дуГа\0", L'Г', 15,NULL},
		  {u8"Тире-пунктир \0 \0",L'Т',268,NULL},
          {u8"Штрих\0 \0",L'Ш',269,NULL} };

static POLE pmLine_ConObrys[] = {
		  {u8"Продовжити",L'П',217,NULL},
		  {u8"Тире-пунктир \0 \0",L'Т',268,NULL},
		  {u8"Штрих\0 \0",L'Ш',269,NULL} };

#define _AXIS_ u8"Вісь"
#define _AXIS_C_ L'В'
#define _AXIS_H_ u8"Вісь #"
#define _AXIS_H_C_ L'В'
#define _LINE_ u8"Лінія"
#define _LINE_C_ L'Л'

#define _LINE_3D_ u8"Лінія3D"

#define _POLYLINE_ u8"Полилінія"
#define _POLYLINE_C_ L'П'

#define _POLYLINE_3D_ u8"Полилінія3D"
#define _POLYLINE_3D_C_ L'П'

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
	{u8"невизначений",L'0',797,NULL},
	{u8"Навантаження власною вагою",L'Н',787,&mLoadVariant},
    {u8"Корисне навантаження",L'К',788,&mLoadVariant},
    {u8"Монтажне навантаження",L'М',789,&mLoadVariant},
    {u8"Вітрове навантаження",L'В',790,&mLoadVariant},
    {u8"Снігове навантаження",L'С',791,&mLoadVariant},
    {u8"Землетрусне навантаження",L'З',792,&mLoadVariant},
    {u8"навантаження Дощу та льоду",L'Д',793,&mLoadVariant},
    {u8"навантаження з тиском землі",L'>',794,&mLoadVariant},
    {u8"Гідравлічне навантаження рідинами",L'Г',795,&mLoadVariant},
    //{u8"Теплове навантаження",L'Т',796,&mLoadVariant},
};

//static
TMENU mLoad_Char = { 10, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 18, 0, 0,0,(POLE(*)[]) &pmLoad_Char, NULL, NULL };

POLE pmLoad_Char_Thermal[] = {
	{u8"невизначений",L'0',797,NULL},
    {u8"Теплове навантаження",L'T',796,&mLoadVariant},
};

//static
TMENU mLoad_Char_Thermal = { 2, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 18, 0, 0,0,(POLE(*)[]) &pmLoad_Char_Thermal, NULL, NULL };

static POLE pmVector[] = {
		  {u8"Зняти\0Del\0",L'З',218,NULL},
          {u8"жорсткий - жорсткий\0\0",L'А',723,NULL},
          {u8"жорсткий - штифт\0\0",L'Б',724,NULL},
          {u8"штифт - жорсткий\0\0",L'В',725,NULL},
          {u8"штифт - штифт\0\0",L'Г',726,NULL},
          {u8"Сила\0\0",L'С',727,&mLoad_Char},
          {u8"Момент\0\0",L'М',728,&mLoad_Char},
          {u8"-момент\0\0",L'-',729,&mLoad_Char},
          {u8"Переміщення\0\0",L'П',730,NULL}, //&mLoad_Char},  //NULL
          {u8"Обертання\0\0",L'О',731,NULL}, //&mLoad_Char}, //NULL
          {u8"-обертання\0\0",L'Е',732,NULL}, //&mLoad_Char}, //NULL
          {u8"трапеція Y\0\0",L'У',733,&mLoad_Char},
          {u8"трапеція X\0\0",L'Х',734,&mLoad_Char},
          {u8"трапеція N\0\0",L'N',735,&mLoad_Char},
          {u8"трапеція H\0\0",L'Н',736,&mLoad_Char},
          {u8"трапеція V\0\0",L'Ч',737,&mLoad_Char},
          {u8"Теплове навантаження\0\0",L'Т',752,&mLoad_Char_Thermal},
          {u8"розмір вузла (радіус)\0\0",L'0',786,NULL},

          {u8"Контур плити\0\0",L'К',846,NULL},
          {u8"Дупло у плиті\0\0",L'Д',844,NULL},
          {u8"стІна\0\0",L'І',843,NULL},
          {u8"Район плити\0\0",L'Р',842,NULL},
          {u8"поверхневе навантаження плити\0\0",L'Й',845,&mLoad_Char},
          {u8"зосереджене навантаження плити\0\0",L'К',856,&mLoad_Char},
};

static POLE pmVector_Con[] = {
		  {u8"Продовження",L'П',220,NULL} };

static POLE pmMember_style[] = {
		  {u8"зняти\0Del\0",L'З',218,NULL},
          {u8"жорсткий - жорсткий\0\0",L'А',723,NULL},
          {u8"жорсткий - штифт\0\0",L'Б',724,NULL},
          {u8"штифт - жорсткий\0\0",L'В',725,NULL},
          {u8"штифт - штифт\0\0",L'Г',726,NULL},
};

static POLE pmForce_Displacement_style[] = {
          {u8"Сила\0\0",L'С',727,NULL},
          {u8"Переміщення\0\0",L'П',730,NULL},
};

static POLE pmMoment_Rotation_style[] = {
          {u8"Момент\0\0",L'М',728,NULL},
          {u8"-момент\0\0",L'-',729,NULL},
          {u8"Обертання\0\0",L'О',731,NULL},
          {u8"-обертання\0\0",L'Р',732,NULL},
};

static POLE pmLoad_style[] = {
          {u8"трапеція Y\0\0",L'У',733,NULL},
          {u8"трапеція X\0\0",L'Х',734,NULL},
          {u8"трапеція N\0\0",L'N',735,NULL},
          {u8"трапеція H\0\0",L'Н',736,NULL},
          {u8"трапеція V\0\0",L'Ч',737,NULL},
};

#define _AXIS_ u8"Вісь"
#define _AXIS_C_ L'В'
#define _AXIS_H_ u8"Вісь #"
#define _AXIS_H_C_ L'В'
#define _VECTOR_ u8"Вектор"
#define _VECTOR_C_ L'В'

#endif

#ifdef __O_LUK__

#define _YES_ u8"Т"
#define _NO_ u8"Н"

static TMENU mArcReversed = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0, 16 /*14*/,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmLukm[] = {
		{u8"[1] три точки",'1',221,NULL},
		{u8"[2] початок-центр-кінець",'2',222,NULL},
		{u8"[3] початок-центр-кут",'3',223,NULL},
		{u8"[4] початок-центр-хорда",'4',224,NULL},
		{u8"[5] початок-кінець-радіус",'5',225,NULL},
		{u8"[6] початок-кінець-кут",'6',226,NULL},
		{u8"[7] початок-кінець-напрямок",'7',227,NULL},
		{u8"[8] продовження",'8',228,NULL},
        {u8"зворотний напрямок дуги",L'0',729,&mArcReversed}};

static POLE pmPLukm[] = {
		{u8"[1] три точки",'1',221,NULL},
		{u8"[2] початок-центр-кінець",'2',222,NULL},
		{u8"[3] початок-центр-кут",'3',223,NULL},
		{u8"[4] початок-центр-хорда",'4',224,NULL},
		{u8"[5] початок-кінець-радіус",'5',225,NULL},
		{u8"[6] початок-кінець-кут",'6',226,NULL},
		{u8"[7] початок-кінець-напрямок",'7',227,NULL},
		{u8"[8] продовження",'8',228,NULL},
        {u8"зворотний напрямок дуги",L'0',729,&mArcReversed},
		{u8"Закрити", L'З', 247, NULL},
		{u8"Видалити: Del", L'В', 248, NULL},
		{u8"Лінія", L'Л', 13, NULL},

        {u8"Без опори\0", L'Б', 851,NULL},
        {u8"Шарнірно-нерухома опора\0", L'Ш', 850,NULL},
        {u8"Жорстке затиснення\0", L'Ж', 852,NULL},
        //{u8"шарнірно-Рухома опора\0", L'Р', 849,NULL},
        {u8"Перевернути опору\0", L'П', 853,NULL},
};

static POLE pmPLukmObrys[] = {

		{u8"[1] три точки",'1',221,NULL},
		{u8"[2] початок-центр-кінець",'2',222,NULL},
		{u8"[3] початок-центр-кут",'3',223,NULL},
		{u8"[4] початок-центр-хорда",'4',224,NULL},
		{u8"[5] початок-кінець-радіус",'5',225,NULL},
		{u8"[6] початок-кінець-кут",'6',226,NULL},
		{u8"[7] початок-кінець-напрямок",'7',227,NULL},
		{u8"[8] продовження",'8',228,NULL},
        {u8"зворотний напрямок дуги",L'0',729,&mArcReversed},
		{u8"Закрити", L'З', 247, NULL},
        {u8"Видалити: Del", L'В', 248, NULL},
        {u8"Лінія", L'Л', 13, NULL},
		{u8"Тире-пунктир \0 \0",L'Т',268,NULL},
		{u8"Штрих\0 \0",L'Ш',269,NULL} };

#define _ARC_ u8"Дуга"
#define _ARC_C_ L'Д'

#endif

#ifdef __O_OKRAG__

static const char CIRCLE[] = u8"Коло";
static const char CIRCLE_FILL[] = u8"Диск";

static POLE pmCircle[] = {
	{u8"Центр-радіус\0",L'Ц',213,NULL},
	{u8"центр-Діаметр\0",L'Д',214,NULL},
	{u8"2 точки\0",L'2',215,NULL},
	{u8"3 точки\0",L'3',216,NULL},
	{u8"Тангенціальний \0",L'Т',266,NULL},
	};

#define _CIRCLE_C_ L'К'
#define _DISC_C_ L'Д'

#endif

#ifdef __O_POLYGN__

static POLE pmPolygon[] = {
	{u8"Сторона\0 \0",L'С',235,NULL},
	{u8"Вписаний\0 \0", L'В',237, NULL},
	{u8"Описаний\0 \0",L'О',236, NULL},
	{u8"Кількість країв\0 \0",L'К',606,NULL},
};

#define _POLYGON_ u8"Багатокутник"
#define _POLYGON_C_ L'Б'

#endif

#ifdef __O_ELLIPS__

static POLE pm_ell__draw_type[] = {
		{u8"Початок осі",L'П',240,NULL},
        {u8"Центр ",L'Ц',241,NULL},
};

static TMENU m_ell__draw_type = { 2,0,0,13,79,7,ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pm_ell__draw_type,NULL,NULL };

static POLE pm_ell_y_draw_method[] = {
		{u8"Відстань",L'В',711,NULL},
		{u8"тОчка",L'О',712,NULL},
        {u8"Тангенціальний",L'Т',713,NULL},
};

static TMENU m_ell_y_draw_method = { 3,0,0,13,79,7,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pm_ell_y_draw_method,NULL,NULL };


POLE pm__ellipse[] = {
		//{u8"Апроксимація\0 20\0 ",L'А',238, NULL},
        {u8"Метод другої півосі\0 Відстань",L'М',710, &m_ell_y_draw_method},
        {u8"початок осі/Центр\0  початок осі",L'Ц',239, &m_ell__draw_type},
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
		//{u8"Апроксимація\0 20\0  ",L'А',238, NULL},
        {u8"Метод другої півосі\0  Відстань",L'М',710, &m_ell_y_draw_method},
		{u8"Непрозорість\0",L'Н',542,&mTranslucencyFE},
					};

#define _ELLIPSE_ u8"Еліпс"
#define _ELLIPSE_C_ L'Е'

POLE pm__ellipticalarc[] = {
		{u8"кут у Місцевих координатах",L'М',708, NULL},
		{u8"кут у Глобальних координатах",L'Г',709, NULL},
					};

#define _ELLIPSE_ANGLE_ u8"Кут"
#define _ELLIPSE_ANGLE_C_ L'К'

#define _ELLIPTICALDISC_ u8"Диск"
#define _ELLIPTICALDISC_C_ L'Д'

static char* EllipticalAngle[] = {u8"Місцевий",u8"Глобальний"};

#endif

#ifdef __O_PLINE__

static POLE pmPLine_Con[] = {
		  {u8"Продовження",L'П',220,NULL} };

extern TMENU mVector;

static POLE pmPLine_Con_Slab[] = {
		  {u8"Продовження",L'П',220,NULL},
          {u8"Вектори",L'В',722,&mVector},
};

#define _POLYLINE_ u8"Полилінія"
#define _POLYLINE_C_ L'П'

#define _Yes_ u8"Так"
#define _No_ u8"Ні"

#define _CHANGE_POLYLINE_TYPE_ u8"Вибрано інший тип полілінії. Чи варто змінити тип на: "

#define __PLATE__ u8"ПЛИТА"
#define __HOLE__  u8"ОТВІР"
#define __WALL__  u8"СТІНА"
#define __ZONE__  u8"ЗОНА"

#endif

#ifdef __O_SOLID__

static char filltyp_tab[7][5] = { u8"Н",u8"дт",u8"т",u8"Т",u8"ДТ",u8"СТ",u8"І" };

static POLE pmS34[] = {
		{u8"Трикутний",L'Т',246,NULL},
		{u8"Чотирикутний",L'Ч',245,NULL},
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
		{u8"Імідж\0",L'І',597,NULL},
		{u8"Масштаб\0 1.0",L'М',436,&mPatternScale},
		{u8"Поворот\0 0",L'П',35,&mPatternAngle},
		{u8"dX\0 0",L'Х',622,&mPatternDx},
		{u8"dY\0 0",L'У',623,&mPatternDy},
		{u8"Отримати імідж\0",L'О',164,NULL},
		{u8"Змінити існуючий імідж\0",L'З',599,NULL},
};

static TMENU mSolidPattern = { 7,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmSolidPattern,NULL,NULL };

static POLE pmTranslucency[] = {
   {u8"100%",L'0',546,NULL},
   {u8"95%",L'1',673,NULL},
   {u8"90%",L'2',545,NULL},
   {u8"85%",L'3',672,NULL},
   {u8"80%",L'4',544,NULL},
   {u8"75%",L'5',671,NULL},
   {u8"70%",L'6',543,NULL},
   {u8"65%",L'7',670,NULL},
   {u8"60%",L'8',542,NULL},
   {u8"55%",L'9',669,NULL},
   {u8"50%",L'А',541,NULL},
   {u8"45%",L'Б',668,NULL},
   {u8"40%",L'В',540,NULL},
   {u8"35%",L'Г',667,NULL},
   {u8"30%",L'Ґ',539,NULL},
   {u8"25%",L'Д',666,NULL},
   {u8"20%",L'Е',667,NULL},
   {u8"15%",L'Є',666,NULL},
   {u8"10%",L'Ж',665,NULL},
   {u8"5%",L'З',664,NULL},

};

TMENU mTranslucency = { 20,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,45,0,0,0,(POLE(*)[]) & pmTranslucency,NULL,NULL };


static POLE pmFillTyp[] = {
		{u8"Заповненийя\0", L'З', 276, &mTranslucency},
		{u8"Дуже тонка\0", L'Д', 277, NULL},
		{u8"тоНка\0", L'Н', 278, NULL},
		{u8"Товста\0", L'Т', 279, NULL},
		{u8"дуже товстА\0", L'А', 280, NULL},
		{u8"Супер товста\0", L'С', 281, NULL},
		{u8"Імідж\0", L'І', 836, &mSolidPattern},
	   //  {u8"- wyłącz obszar",'-',0,NULL},
	   //  {u8"+ Aktywny obszar",'+',0,NULL},
};

static TMENU mFillTyp = { 7,0,0,15,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,5,0,0,0,(POLE(*)[]) &pmFillTyp,NULL,NULL };

POLE pmSolid[] = {
		{u8"Чотирикутна/трикутна\0  4",L'Ч',242,&mS34},
		{u8"Трикутний\0",L'Т',243,NULL},
		{u8"Заповнення/тип лінії\0 З\0     ",L'З',244,&mFillTyp},
};

#define _SOLID_ u8"Солід"
#define _SOLID_3D_ u8"Солід 3D"
#define _SOLID_C_ L'С'

#endif

#ifdef __O_SPLINE__

#define _Q_ u8"К"
#define _C_ u8"Б"
#define _M_ u8"М"
#define _B_ u8"С"

//char _YES__[4] = u8"Т";
//char _NO__[4] = u8"Н";

static POLE pmS34[] = {
		{u8"Квадратна крива Bézier",L'К',520,NULL},
		{u8"куБічна крива Bézier",L'Б',519,NULL},
		{u8"Мультиточковий сплайн",L'М',646,NULL},
		{u8"Сплайн Bézier",L'С',662,NULL}
};

static TMENU mS34 = { 4,0,0,36,79,6,ICONS,CMNU,CMBR,CMTX,0,8,0,0,0,(POLE(*)[]) &pmS34,NULL,NULL };

static TMENU mD_T_Pline = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,12,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmSpline[] = {
		{u8"Кубіc/квадрат./м.точкова\0 К\0",L'К',517,&mS34},
		{u8"квадратний цього Разу\0\0",L'Р',518,NULL},
		{u8"Полилінія\0 Т\0",L'П',251,&mD_T_Pline},
		{u8"Видалити\0Del\0",L'В',218,NULL},
		{u8"продовження\0\0",L'+',228,NULL},
		{u8"Фініш криву\0End\0",L'Ф',649,NULL},
		{u8"Напруги\0\0",L'Н',648,NULL},
		{u8"Закрити криву Home\0 Н\0",L'З',647,NULL},
};

#define _SPLINE_ u8"Сплайн"
#define _SPLINE_C_ L'С'

#define _YES__ u8"Т"
#define _NO__ u8"Н"

#endif

#ifdef __O_TRACE__

extern TMENU mPatternScale;
extern TMENU mPatternAngle;
extern TMENU mPatternDx;
extern TMENU mPatternDy;
extern TMENU mTranslucency;

static char tracefilltyp_tab[7][5] = { u8"Н",u8"дт",u8"т",u8"Т",u8"ДТ",u8"СТ",u8"І" };

static TMENU mD_T_Pline = { 2,0,0,7,62,9,ICONS,CMNU,CMBR,CMTX,0,7,0,0,0,&pmTak_Nie,NULL,NULL };

static POLE pmTracePattern[] = {
		{u8"Імідж\0",L'І',597,NULL},
		{u8"Масштаб\0 1.0",L'М',436,&mPatternScale},
		{u8"Поворот\0 0",L'П',35,&mPatternAngle},
		{u8"dX\0 0",L'Х',622,&mPatternDx},
		{u8"dY\0 0",L'У',623,&mPatternDy},
		{u8"Отримати імідж\0",L'О',164,NULL},
		{u8"Змінити існуючий імідж\0",L'З',599,NULL},
};

static TMENU mTracePattern = { 7,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTracePattern,NULL,NULL };

static POLE pmFillTyp[] = {
	    {u8"Заповненийя", L'З', 288, &mTranslucency},
		{u8"Дуже тонка", L'Д', 289, NULL},
		{u8"тоНка", L'Н', 290, NULL},
		{u8"Товста", L'Т', 291, NULL},
		{u8"дуже товстА", L'А', 292, NULL},
		{u8"Супер товста", L'С', 293, NULL},
		{u8"Імідж", L'І', 836, &mTracePattern},
};

static TMENU mFillTyp = { 7,0,0,15,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,9,0,0,0,(POLE(*)[]) &pmFillTyp,NULL,NULL };

#define _YES_ u8"Т"
#define _NO_ u8"Н"

static TMENU mTraceArcReversed = { 2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0, 14 /*11*/,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmPTLukm[] = {
        {u8"[0] продовження дугою",'0',714,NULL},
        {u8"[1] три точки",'1',221,NULL},
		{u8"[2] початок-центр-кінець",'2',222,NULL},
		{u8"[3] початок-центр-кут",'3',223,NULL},
		{u8"[4] початок-центр-хорда",'4',224,NULL},
		{u8"[5] початок-кінець-радіус",'5',225,NULL},
		{u8"[6] початок-кінець-кут",'6',226,NULL},
		{u8"[7] початок-кінець-напрямок",'7',227,NULL},
		//{u8"[8] продовження",'8',228,NULL},
        {u8"зворотний напрямок дуги",L'0',729,&mTraceArcReversed},
		//{u8"Закрити", L'З', 247, NULL},
		//{u8"Видалити: Del", L'В', 248, NULL},
		{u8"Лінія", L'Л', 13, NULL},
        {u8"Продовження з лінією", L'П',715,NULL},
};

static TMENU mPTLukm={11, 0, 0, 30, 56, 4, ICONS | TADD, CMNU,CMBR,CMTX,0,75,0,0,0,&pmPTLukm,NULL,NULL};

static POLE pmFTrace[] = {
	{u8"Ширина\0 1\0", L'Ш', 249, NULL},
	{u8"Зсув осі\0 1\0", L'З', 250, NULL},
	{u8"Полілінія\0 Т\0", L'П', 251, &mD_T_Pline},
	{u8"Наповнення/товщина лінії\0 H\0", L'Н', 252, &mFillTyp},
	{u8"Видалити\0Del\0", L'В', 253, NULL},
	{u8"Розрив\0Home\0", L'Р', 633, NULL},
	{u8"Закрити\0End\0", L'З', 632, NULL},
    {u8"Дуга\0\0 ",L'Д',15,&mPTLukm},
};

static POLE pmMLine[] = {
	{u8"Ширина\0 1\0", L'Ш', 249, NULL},
	{u8"Зсув осі\0 1\0", L'З', 250, NULL},
	{u8"Полілінія\0 Т\0", L'П', 251, &mD_T_Pline},
	{u8"Видалити\0Del\0", L'В', 253, NULL},
	{u8"Розрив\0Home\0", L'Р', 633, NULL},
	{u8"Закрити\0End\0", L'З', 632, NULL},
    {u8"Дуга\0\0 ",L'Д',15,&mPTLukm},
};

#define _TRACE_ u8"Слід"
#define _TRACE_C_ L'С'

#define _DLINE_ u8"2-лінія"
#define _DLINE_C_ L'2'

#define _TLINE_ u8"3-лінія"
#define _TLINE_C_ L'3'

#define _YES__ u8"Т"
#define _NO__ u8"Н"

#define _BREAK_ u8"Перерва"

#endif

#ifdef __O_SKETCH__

static POLE pmSketch[] = {
	{u8"Довжина сегмента\0   1мм",L'Д',531, NULL},
	{u8"Видаліть останній\0Del\0 ",L'В',532, NULL},
};

#define _SKETCH_ u8"Ескіз"
#define _SKETCH_C_ L'Е'

#define _MM_ u8"мм"

#endif

#ifdef __O_POINT__

#define _POINT_TYPE_ u8"Форма точки"
#define _POINT_TYPE_C_ L'Ф'

static POLE pmTyp_punktu[] = {
	 {u8"Простий\0", L'П', 294, NULL},
	 {u8"Базова точка\0", L'Б', 295, NULL},
	 {u8"Термінал\0", L'Т', 296, NULL},
	 {u8"З'єднання\0", L'З', 529, NULL},
     {u8"Жорстке кріплення\0",L'Ж',738,NULL},
     {u8"жорстке кріплення Л\0",L'1',739,NULL},
     {u8"жорстке кріплення П\0",L'2',740,NULL},
     {u8"Жорстке кріплення В\0",L'3',754,NULL},
     {u8"Шарнірне, нерухоме кріплення\0",L'Ш',741,NULL},
     {u8"шарнірне, нерухоме кріплення Л\0",L'4',742,NULL},
     {u8"шарнірне, нерухоме кріплення П\0",L'5',743,NULL},
     {u8"шарнірне, нерухоме кріплення В\0",L'6',755,NULL},
     {u8"жорстке, Горизонт. ковзання\0",L'Г',744,NULL},
     {u8"жорстке, вертик. ковзання Л\0",L'7',745,NULL},
     {u8"жорстке, вертик. ковзання П\0",L'8',746,NULL},
     {u8"жорстке, горизонт. ковзання В\0",L'9',756,NULL},
     {u8"шарнірне, горизонт. Ковзання\0",L'К',747,NULL},
     {u8"шарнірне, вертик. ковзання Л\0",L'0',748,NULL},
     {u8"шарнірне, вертик. ковзання П\0",L'-',749,NULL},
     {u8"шарнірне, горизонт. ковзання В\0",L'=',757,NULL},
     {u8"немає обертання Z\0",L'+',818,NULL},
};

#endif

#ifdef __O_GRAPHF__

#define MAX_GRAPHFNO 16
#define MAX_GRAPHF_NAME_LEN 16
#define MAX_GRAPHF_FORMULA_LEN  (MAX_GRAPHFNO * MaxTextLen)    //4096

#define GRAPHF u8"Графік"
#define _GRAPHF_ u8"Графік"
#define _GRAPHF_C_ L'Г'

static POLE pmGraphf[MAX_GRAPHFNO + 1];

static TMENU mGraphf = { 16,0,0,MAX_GRAPHF_NAME_LEN + 1,70,7,0,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmGraphf,NULL,NULL };

static POLE pmGraph[] = {
		  {u8"Крок dx\0",L'К',254,NULL},
		  {u8"Функція\0",L'Ф',255, &mGraphf} };

#define _CURRENT_ u8"Поточна"

#endif

#ifdef __B_MENU__

#define _ENDPOINT_ L'К'
#define _endpoint_ L'к'
#define _CTRL_ENDPOINT_ 1938 //R 18
#define _NEAREST_ L'Н'
#define _nearest_ L'н'
#define _CTRL_NEAREST_ 1945 //Y 25
#define _MIDDLE_ L'М'
#define _middle_ L'м'
#define _CTRL_MIDDLE_ 1942 //V 22
#define _INTERSECTION_ L'П'
#define _intersection_ L'п'
#define _CTRL_INTERSECTION_ 1927 //G 7  
#define _CENTER_ L'Ц' 
#define _center_ L'ц'
#define _CTRL_CENTER_ 1943 //W 23
#define _PERPENDICULAR_ L'Е'
#define _perpendicular_ L'е'
#define _CTRL_PERPENDICULAR_ 1940 //T 20
#define _TANGENTIAL_ L'Д'
#define _tangential_ L'д'
#define _CTRL_TANGENTIAL_ 1932 //L 12
#define _ADJACENT_ L'С'
#define _adjacent_ L'с'
#define _CTRL_ADJACENT_ 1923 //C 3
#define _POINT_ L'Т'
#define _point_ L'т'
#define _CTRL_POINT_ 1934 //N 14
#define _EDIT_ L'Р'
#define _edit_ L'р'
#define _CTRL_EDIT_ 1928 //H 8

#define _CTRL_NOPE_ 1924 //D 4

#define _CTRL_0_ 0  //SPC
#define _CTRL_1_ 23 //1  //A
#define _CTRL_2_ 2   //B
#define _CTRL_3_ _CTRL_ADJACENT_  //C
#define _CTRL_4_ _CTRL_NOPE_   //D
#define _CTRL_5_ 5  //E
#define _CTRL_6_ 6  //F
#define _CTRL_7_ _CTRL_INTERSECTION_  //G
#define _CTRL_8_ _CTRL_EDIT_  //H
#define _CTRL_9_ 23  //I  Ш   Layers
#define _CTRL_10_ 18   //J Останній Copy Object
#define _CTRL_11_ 11  //K
#define _CTRL_12_ _CTRL_TANGENTIAL_  //L
#define _CTRL_13_ 13  //M
#define _CTRL_14_ _CTRL_POINT_  //N
#define _CTRL_15_ 15  //O
#define _CTRL_16_ 16  //P
#define _CTRL_17_ 17  //Q
#define _CTRL_18_ _CTRL_ENDPOINT_  //R
#define _CTRL_19_ 19  //S scale down
#define _CTRL_20_ _CTRL_PERPENDICULAR_  //T
#define _CTRL_21_ 21  //U
#define _CTRL_22_ _CTRL_MIDDLE_  //V
#define _CTRL_23_ _CTRL_CENTER_  //W
#define _CTRL_24_ 24  //X
#define _CTRL_25_ _CTRL_NEAREST_  //Y
#define _CTRL_26_ 26  //Z  scale up
#define _CTRL_27_ 27
#define _CTRL_28_ 28
#define _CTRL_29_ 29
#define _CTRL_30_ 30
#define _CTRL_31_ 31

#define _PRINTING__ u8"Друк"

#endif

#ifdef __O_HATCH__

//extern TMENU mTranslucency;
static POLE pmHTranslucency[] = {
   {u8"100%",L'0',546,NULL},
   {u8"95%",L'1',673,NULL},
   {u8"90%",L'2',545,NULL},
   {u8"85%",L'3',672,NULL},
   {u8"80%",L'4',544,NULL},
   {u8"75%",L'5',671,NULL},
   {u8"70%",L'6',543,NULL},
   {u8"65%",L'7',670,NULL},
   {u8"60%",L'8',542,NULL},
   {u8"55%",L'9',669,NULL},
   {u8"50%",L'А',541,NULL},
   {u8"45%",L'Б',668,NULL},
   {u8"40%",L'В',540,NULL},
   {u8"35%",L'Г',667,NULL},
   {u8"30%",L'Ґ',539,NULL},
   {u8"25%",L'Д',666,NULL},
   {u8"20%",L'Е',667,NULL},
   {u8"15%",L'Є',666,NULL},
   {u8"10%",L'Ж',665,NULL},
   {u8"5%",L'З',664,NULL},

};

TMENU mHTranslucency = { 20,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0, 512,0,0,0,(POLE(*)[]) & pmHTranslucency,NULL,NULL };


#define HATCH_PATTERN u8"ВІЗЕРУНКИ"
#define PATTERNS_INI u8"ВІЗЕРУНКИ.INI"

#define PATTERN_NAMES_MAXNO 255

static PPOLE pm__list_hatch[PATTERN_NAMES_MAXNO];

static POLE pm__list_hatch_predef[] = {
		{u8"֎СОЛІД\0",L'С',0,&mHTranslucency},
		{u8"////////\0",L'/',0,NULL},
		{u8"XXXXXXXX\0",L'Х',0,NULL} };

POLE pmKatH[] = {
		{u8"Кут",L'К',108,NULL },
		{u8"Паралельно до",L'П',580,NULL},
		{u8"пеРпендикулярно до",L'Р',581,NULL},
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
		{u8"Імідж фолдєр\0",L'І',597,NULL},
		{u8"Масштаб\0 1.0",L'М',436,&mSolidHatchPatternScale},
		{u8"Поворот\0 0",L'П',35,&mSolidHatchPatternAngle},
		{u8"dX\0 0",L'Х',622,&mSolidHatchPatternDx},
		{u8"dY\0 0",L'У',623,&mSolidHatchPatternDy},
		{u8"Отримати імідж\0",L'О',164,NULL},
};

static TMENU mSolidHatchPattern = { 6,0,0,40,79,6,ICONS | TADD,CMNU,CMBR,CMTX,0,ID_IMAGE_PATTERN,0,0,0,(POLE(*)[]) & pmSolidHatchPattern,NULL,NULL };

POLE pmSelect_Ins[] = {
	{u8"Простий",L'П',680,NULL},
	{u8"Складний",L'С',681,NULL},
};

TMENU mSelect_Ins = { 2, 0, 0, 18, 17, 14, ICONS, CMNU, CMBR, CMTX, 0, ID_INSULATION, 0, 0,0,(POLE(*)[]) &pmSelect_Ins, NULL, NULL };

POLE pmHatch[] = {
	 {u8"Візерунок\0//////////\0",L'В',434,&m__list_hatch},
	 {u8"Фолдєр з візерунками\0\0",L'Ф',597,NULL},
	 {u8"кут Повороту\00\0", L'П', 435,&mKatH},
	 {u8"Масштаб\01\0", L'М', 436,NULL},
	 {u8"Координати б. точки\00,0\0", L'К', 437,NULL},
	 {u8"вкажіть Базову Точку\0\0", L'Б', 438,NULL},
	 {u8"відстань Лінії/відрізок\01\0", L'Л', 439,NULL},
	 {u8"Термоізоляція\0\0",L'Т',679, &mSelect_Ins},
	 {u8"Імідж візерунок\0\0",L'І',836,&mSolidHatchPattern}
};

#define _HATCHING_ u8"Хетчінг"
#define _HATCHING_C_ L'Х'

#define _SOLID_SUFFIX_ "UA"

#define __IMAGE u8"__ІМІДЖ"

#endif

#ifdef __O_DXF_IN__

#define Linie_DXF u8"ЛІНІЇ-DXF"
static char line_width_dxf_param[5][32] =
{
  u8"дуже тонка",
  u8"тонка",
  u8"товста",
  u8"дуже товста",
  u8"супер товста",
};

#define _LINE_NO_ u8"лінія: "
#define _SYSTEM_MESSAGE_ u8"СИСТЕМНЕ ПОВІДОМЛЕННЯ"
#define _INTERRAPTED_ u8"Перерваний"

#endif

#ifdef __O_BLOK__

static char	 sz__last_bl[] = u8"останній Блок\0 ";
static char	sz__wszystko[] = u8"Всі\0 ";
static char	sz__wszystkie_warstwy[] = u8"всі Шари\0 ";
static char	sz__wszystkie_teksty[] = u8"всі Тексти\0 ";
static char	sz__wszystkie_symbole[] = u8"всі Символи\0 ";
static char	 sz__cross_win[] = u8"Крос/вікно\0  F9";
static char	sz__add_del[] = u8"додати/Видалити\0  F10";
static char	sz_undo_del[] = u8"Повернути все\0 ";

static TMENU mAuto = { 2,0,0,5,72,9,ICONS,CMNU,CMBR,CMTX,0, 7, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAutoc = { 2,0,0,5,72,8,ICONS,CMNU,CMBR,CMTX,0,	5,	0,	0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dq = { 2,0,0,5,72,7,ICONS,CMNU,CMBR,CMTX,0, 4, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAuto_dn = { 2,0,0,5,72,9,ICONS,CMNU,CMBR,CMTX,0, 9 /*7*/, 0, 0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmBlokm[] = {
	 {u8"останній Блок\0 ",L'Б',256,NULL},
	 {u8"Всі\0 ",L'В',257,NULL},
	 {u8"всі Шари\0 ",L'Ш',258,NULL},
	 {u8"Крос/вікно\0  F9", L'К',259,NULL},
	 {u8"Додати/видалити\0  F10" ,L'Д', 260,NULL},
	 {u8"Авто\0 Y", L'А',261,&mAuto},
};

POLE pmScalem[] = {
		{u8"Апроксимація\0 20\0  ",L'А',238, NULL},
};

POLE pmBlokmc[] = {
	 {u8"Крос/вікно\0  F9", L'К',259,NULL},
	 {u8"Додати/видалити\0  F10" ,L'Д',260, NULL},
	 {u8"Авто\0 Y", L'А',261,&mAutoc},
};

POLE pmSelectMode[] = {
	 {u8"Крос", L'К',259,NULL},
	 {u8"Об'єкт" , L'О',720, NULL},
};


POLE pmBlock_wez_w[] =
{
  {u8"Крос/вікно\0  F9",	L'К',259,NULL},
  {u8"Авто\0 Y",	L'А',261,&mAuto_dq},
};

POLE pmBlock_wez_info[] =
{
  {u8"Вся полілінія",	L'В',687,NULL},
  {u8"Одиночний об'єкт", L'О',688,NULL},
};

POLE pmBlock_dq[] =
{
  {u8"Крос/вікно\0  F9",	L'К',259,NULL},
  {u8"Авто\0Y",	L'А',261,&mAuto_dq},
  {u8"Повернути все\0 ", L'П',270,NULL},
  {u8"змінити Режим\0 ", L'Р',271,NULL},
};

POLE pmBlock_dn[] =
{
  {u8"останній Блок\0 ",L'Б',256,NULL},
  {u8"Всі\0 ",L'В',257,NULL},
  {u8"всі Шари\0 ",L'Ш',258,NULL},
  {u8"Крос/вікно\0  F9", L'К',259,NULL},
  {u8"Додати/видалити\0  F10" ,L'Д', 260,NULL},
  {u8"Авто\0 Y", L'А',261,&mAuto},
  {u8"Стирати\0 Del", L'С',32,NULL},
  {u8"Повернути все\0 ", L'П',270,NULL},
  {u8"змінити Режим\0", L'Р',271,NULL},
};

POLE pmBlok_ex[] = {
	 {u8"останній Блок\0 ",L'Б',256,NULL},
	 {u8"Всі\0 ",L'В',257,NULL},
	 {u8"всі Шари\0 ",L'Ш',258,NULL},
	 {u8"Крос/вікно\0  F9", L'К',259,NULL},
	 {u8"Додати/видалити\0  F10" ,L'Д', 260,NULL},
	 {u8"Авто\0 Y", L'А',261,&mAuto},
	 {u8"Розірвати\0",L'Р',66,NULL},
};

POLE pmBlok_chp[] = {
	 {u8"останній Блок\0 ",L'Б',256,NULL},
	 {u8"Всі\0 ",L'В',257,NULL},
	 {u8"всі Шари\0 ",L'Ш',258,NULL},
	 {u8"все з шару Но:\0 ",L'Н',174,NULL},
	 {u8"всі Тексти\0 ",L'Т',273,NULL},
	 {u8"Крос/вікно\0  F9", L'К',259,NULL},
	 {u8"Додати/видалити\0  F10" ,L'Д', 260,NULL},
	 {u8"Авто\0 Y", L'А',261,&mAuto},
	 {u8"Змінити властивості\0",L'З',272,NULL},
};

POLE pmBlok_cht[] = {
	 {u8"останній Блок\0 ",L'Б',256,NULL},
	 {u8"всі Тексти\0 ",L'Т',273,NULL},
	 {u8"Крос/вікно\0  F9", L'К',259,NULL},
	 {u8"Додати/видалити\0  F10" ,L'Д',260, NULL},
	 {u8"Авто\0 Y", L'А',261,&mAuto},
};

POLE pmBlok_chts[] = {
	 {u8"останній Блок\0 ",L'Б',256,NULL},
	 {u8"всі Символи\0 ",L'С',257,NULL},
	 {u8"всі Тексти\0 ",L'Т',273,NULL},
	 {u8"Крос/вікно\0  F9", L'К',259,NULL},
	 {u8"Додати/видалити\0  F10" ,L'Д',260, NULL},
	 {u8"Авто\0 Y", L'А',261,&mAuto},
};

POLE pmCopy_Divide[] = {
	 {u8"подіЛяти",L'Л',50,NULL},
};

static POLE	pmWszystko_Okno[] = {
	 {u8"Всі",L'В',257,NULL},
	 {u8"вікнО",L'О',274,NULL},
};

char window[16] = u8"вікно";
char  cross[16] = u8"крос";

char _addto_[16] = u8"додати";
char  _remove_[16] = u8"видалити";

char Yes[2] = u8"Т";

#define _DIVIDE_ u8"Поділяти"
#define _DIVIDE_C_ L'П'

#define _BLOCK_ u8"Блок"
#define _BLOCKF_ u8"* Блок"
#define _BLOCK_C_ L'Б'

#define _BLOCK_SELECT_ u8"Режим вибору"
#define _BLOCK_SELECT_C_ L'Р'

#define _FROZEN_ u8"заморожений "
#define _EXPLODE_BLOCKS_ u8"Ви хочете розбити позначені %sблоки?"
#define _Yes_ u8"Так"
#define _No_ u8"Ні"

#define _SCALE_ u8"Масштаб"
#define _SCALE_C_ L'М'

#define _FILE_ u8"Файл "
#define _FILE_EXISTS_ u8" існує. Перезаписати ? (Так/Ні/Esc):"

#define _YES_NO_ESC_ u8"ТНтн\033"
#define _YES_ L'Т'
#define _yes_ L'т'
#define _NO_ L'Н'
#define _no_ L'н'

#define _SAVED_BLOCK_ u8"< збережений блок > "

#define _CHANGE_ u8"Обмін"
#define _CHANGE_C_ L'О'

#define _POINT_IN_AREA_ u8"ДОБРЕ. Точка в закритій зоні."
#define _POINT_NOT_IN_AREA_ u8"Не добре. Точка за межами замкнутої зони або незакритої зони."

#define _OBJECT_ u8"Об'єкт"
#define _OBJECT_C_ L'О'

static char _entire_polyline_[32]=u8"Вся полілінія";
static char _single_object_[32]=u8"Одиночний об'єкт";

#endif

#ifdef __O_PROF__

POLE pmChamfer[] =
{
	 {u8"Відстань(и)\0       \0",L'В', 316, NULL},
	 {u8"Скасувати\0     \0",L'С', 459, NULL},
};

POLE pmFillet[] =
{
	 {u8"Радіус\0      \0",L'Р', 209, NULL},
	 {u8"Скасувати\0    \0",L'С', 459, NULL},
};

#define _FILLET_ u8"філе"
#define _FILLET_C_ L'ф'

#define _CHAMFER_ u8"Фаска"
#define _CHAMFER_C_ L'Ф'

#endif

#ifdef __O_OFFSET__

#define OFFSET_NORMAL u8"Нормальний"
#define OFFSET_SMOOTH u8"Гладкий"

static POLE pmMethod[] = {
	 {u8"Нормальний",L'Н',630,NULL},
	 {u8"Гладкий",L'Г',631,NULL},
};

static TMENU mMethod = { 2, 0,0,19,56,4,ICONS,CMNU,CMBR,CMTX,0,4, 0,0,0,(POLE(*)[]) &pmMethod, NULL,NULL };

static POLE pmOffset[] = {
	 {u8"Точка\0     ",L'Т',457,NULL},
	 {u8"Відстань\0     ", L'В', 458,NULL},
	 {u8"Набір відстань\0     ", L'Н', 605,NULL},
	 {u8"Метод поліліній\0     ", L'М', 629,&mMethod},
};

#define _OFFSET_ u8"Офсетний"
#define _OFFSET_C_ L'О'

#endif

#ifdef __O_MIRROR__

POLE pmMirror[] = {
		{u8"Залиште оригінал\0 Del",L'З',603,NULL},
		{u8"Видалити оригінал\0 Del",L'В',604,NULL},
};

char st_del[] = u8"Видалити", st_nodel[] = u8"Залишати";

#define _MIRROR_ u8"Дзеркало"
#define _MIRROR_C_ L'Д'

#endif

#ifdef __O_IMPORT__

#define PATTERNS u8"Візерунки"
#define __PATTERNS__ u8"Візерунки"
#define BACKGROUND u8"Фони"
#define __BACKGROUND__ u8"Фони"
char *BLOCKLIST=u8"Список блоків";


static POLE	pmObrot[] = {
	 {u8"повернути На кут",L'Н',579,NULL},
	 {u8"45°",	L'4',441,NULL},
	 {u8"90°",	L'9',442,NULL},
	 {u8"135°",	L'1',443,NULL},
	 {u8"180°",	L'8',444,NULL},
	 {u8"225°",	L'2',445,NULL},
	 {u8"270°",	L'7',446,NULL},
	 {u8"315°",	L'3',447,NULL},
	 {u8"0°",	    L'0',578,NULL},
	 {u8"повернути До кута",L'Д',440,NULL},
	 {u8"Паралельно до",L'П',580,NULL},
	 {u8"пеРпендикулярно до",L'Р',581,NULL},
	 {u8"під Кутом до",L'К',582,NULL},
};

static TMENU mObrot = { 13, 0, 0, 16, 66, 8, ICONS, CMNU, CMBR, CMTX, 0, 7, 0,0,0,(POLE(*)[]) & pmObrot, NULL, NULL };

static POLE pmObrot_GEO[] = {
	 {u8"повернути На кут",L'Н',579,NULL},
	 {u8"45°",	L'4',441,NULL},
	 {u8"90°",	L'9',442,NULL},
	 {u8"135°",	L'1',443,NULL},
	 {u8"180°",	L'8',444,NULL},
	 {u8"225°",	L'2',445,NULL},
	 {u8"270°",	L'7',446,NULL},
	 {u8"315°",	L'3',447,NULL},
	 {u8"0°",	    L'0',578,NULL},
	 {u8"повернути До кута",L'Д',440,NULL},
	 {u8"------",	L' ',0,NULL},
	 {u8"kXZ",		L'Z',0,NULL},
	 {u8"kYZ",		L'Y',0,NULL},
};

static TMENU mObrot_GEO = { 13, 0, 0, 16, 66, 8, ICONS, CMNU, CMBR, CMTX, 0, 5, 0,0,0,(POLE(*)[]) & pmObrot_GEO, NULL, NULL };


static POLE	pmBlok_Imp[] = {
	 {u8"Масштаб\0",L'М',36,NULL},
     {u8"Фіксований масштаб\0",L'Ф',695,NULL},
	 {u8"Поворот\0",L'П',35,&mObrot},
	 {u8"дзеркало Y\0",L'У',41,NULL},
	 {u8"дзеркало X\0",L'Х',40,NULL},
     {u8"Центр до курсору\0  ",L'Ц',717,NULL},
	 {u8"подіЛяти\0",L'Л',50,NULL},
};

static POLE	pmBlok_Divide[] = {
	 {u8"Divide",'D',50,NULL},
};

static POLE	pmBlok_Imp_GEO[] = {
	{u8"Масштаб",L'М',36,NULL},
	 {u8"Поворот",L'П',35,&mObrot},
	 {u8"дзеркало Y",L'У',41,NULL},
	 {u8"дзеркало X",L'Х',40,NULL},
	 {u8"подіЛяти",L'Л',50,NULL},
};

#define patterns_tip u8"@Виберіть шаблон штрихування"
#define background_tip u8"@Виберіть фон робочого столу"
#define blocklist_tip u8"@Виберіть блок зі списку"
#define alpha_sorting_tip u8"Алфавітний порядок"
#define timelapse_tip u8"Сортування за часом"
#define catalog_tip u8"@Виберіть блок"

char catalog[32] = u8"Каталог";
char background[32] = u8"Фон";
char blocklist[32] = u8"Блоки";
char patterns[32] = u8"Візерунки";

#define _BLOCK_ u8"Блок"
#define _BLOCK_C_ L'Б'

#define _BLOCK_3D_ u8"Блок 3D"

#define _BLOCK__ u8"Блок : ";
#define _BLOCK_3D__ u8"Блок : ";

#define _BACKGROUND_ u8"Фон : "
#define _SOLID_IMAGE_ u8"Імідж соліда: "
#define _HATCH_IMAGE_ u8"Імідж штрихування : "
#define _TRACE_IMAGE_ u8"Імідж сліду : "
#define _IMAGE_ u8"Імідж : "

#define _PATTERN_ u8"Візерунок: "

#define _PCX_WRITE_ERROR_ u8"Помилка запису файлу PCX"
#define _PNG_WRITE_ERROR_ u8"Помилка запису файлу PNG"
#define _JPG_WRITE_ERROR_ u8"Помилка запису файлу JPG"

#define _NO_PATTERN_ u8"НЕМАЄВИЗОРУ.axx"

#define _NO_BLOCK_IN_CATALOG_ u8"У каталозі немає символу ("
#define AUTOSCALE u8"   АВТОМАСШТАБ"

#define FOLDER u8"\023<ВСЕ>"

#endif

#ifdef __O_BREAK__

static POLE pmBreakOptions[] = {
		 {u8"вирізати і Залишити",L'З',692,NULL},
		 {u8"вирізати і Видалити",L'В',693,NULL},
         };

static TMENU mBreakOptions = { 2,0,0,4,24,8,TADD | ICONS,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pmBreakOptions,NULL,NULL };

POLE pmBreak[] =
{
	 {u8"Скасувати",L'С',459,NULL},
	 {u8"подіЛяти",L'Л',50,NULL},
     {u8"варіанти Вирізання\0 ",L'В',691,&mBreakOptions},
};

#define _BREAK_ u8"Перервити"
#define _BREAK_C_ L'П'

static char _break_and_leave_[] = u8"вирізати і Залишити";
static char _break_and_cut_[] = u8"вирізати і Видалити";

#endif

#ifdef __O_TRIM__

static POLE pmTrim[] =
{
 {u8"Скасувати",L'С',459,NULL},
};

#define _TRIM_ u8"Обрізати"
#define _TRIM_C_ L'О'

static POLE pmExtend[] =
{
 {u8"Скасувати",L'С',459, NULL},
};

#define _EXTEND_ u8"Подовжити"
#define _EXTEND_C_ L'П'

#endif

#ifdef __O_EXTEND__

#endif

#ifdef __B_HLP__

#define _1920_ 1920

POLE pmHelp[] = {
{u8"ІНСТРУМЕНТИ",31,351,NULL},
{u8"Допоміжне меню",30,391,NULL},
{u8"Допоміжне меню",26,359,NULL},
{u8"Повторити останню функцію",548 + _1920_,547,NULL},
{u8"Редагувати числові значення",' ',363,NULL},
{u8"Збільшити розмір указателя",' ',361,NULL},
{u8"Зменшити розмір указателя",' ',360,NULL},
#ifdef MACOS
	{u8"Звільнити мишку",' ',387,NULL},
#else
	{u8"Звільнити мишку",' ',386,NULL},
#endif
//{u8"Звільнити мишку",24,353,NULL},
{u8"Звільнити мишку",840 + _1920_,353,NULL},
{u8"Звільнити мишку (Linux)",363 + _1920_,353,NULL},
{u8"Звільнити мишку (утримуйте П 0,5c)",819 + _1920_, 658,NULL},
{u8"Розмір і положення вікна",25,353,NULL},
{u8"Зупинити запис макросу",28,353,NULL},
{u8"Ортогональність",638 + _1920_,353,NULL},
{u8"Скопіюйте об'єкт під курсором",574 + _1920_,353,NULL},
{u8"Виберіть шар",549 + _1920_,353,NULL},
{u8"РЕДАГУВАННЯ",31,321,NULL},
{u8"Редагувати текст",' ',377,NULL},
{u8"Перемістити текст",15,378,NULL},
{u8"Видалити об'єкт",' ',380,NULL},
{u8"Відновити видалений об'єкт",' ',381,NULL},
{u8"Ортогональність",' ',383,NULL},
{u8"Вікно / Крос",' ',384,NULL},
{u8"Додати/Вилучити",' ',385,NULL},
{u8"ПЕРЕГЛЯД",31,53,NULL},
{u8"Збільшити/зменшити масштаб",' ',389,NULL},

{u8"Панорамування",355 + 1920,353,NULL},
{u8"Панорамування",354 + 1920,353,NULL},

{u8"Вікно",12,353,NULL},
{u8"Останній перегляд",13,353,NULL},
{u8"Переглянути всі",14,353,NULL},
{u8"Центр перегляду",15,353,NULL},
{u8"Масштаб перегляду 1",16,353,NULL},
{u8"Масштаб перегляду 0,5",17,353,NULL},
{u8"Масштаб перегляду 0,2",18,353,NULL},
{u8"Масштаб перегляду 0,1",19,353,NULL},
{u8"Перемалювати",20,353,NULL},
{u8"Переглянути повний малюнок",21,353,NULL},

{u8"Збільшити масштаб",837 + _1920_,353,NULL},
{u8"Зменшити масштаб",838 + _1920_,353,NULL},

{u8"ОСОБЛИВОСТІ",31,352,NULL},
{u8"Колір",12,354,NULL},
{u8"Тип лінії",13,354,NULL},
{u8"Ширина лінії",14,354,NULL},
{u8"Шар",15,354,NULL},
{u8"Отримати колір",19,354,NULL},
{u8"Отримати тип і ширину лінії",20,354,NULL},
{u8"Отримати шар",21,354,NULL},
{u8"Сховати шар",22,354,NULL},
{u8"МІРА ",31,390,NULL},
{u8"Координати точки",16,354,NULL},
{u8"Відстань",17,354,NULL},
{u8"Кут ",18,354,NULL},
{u8"ШАРІ ",31,55,NULL},
{u8"Шар 1",1,354,NULL},
{u8"Шар 2 ...",2,354,NULL},
{u8"Шар 10",0,354,NULL},
{u8"Наступний шар",10,354,NULL},
{u8"Попередній шар",11,354,NULL},
{u8"Усі шари УВІМКНЕНО",22,353,NULL},
{u8"Зворотні шари",23,353,NULL},
{u8"КРОК ПОКАЖНИКА",31,154,NULL},
{u8"Мінімальний / Великий крок",12,355,NULL},
{u8"ТОЧКОШУКАЧ",31,52,NULL},

{u8"Кінець",13,355,NULL},
{u8"Ближчий кінець",14,355,NULL},
{u8"Середній",15,355,NULL},
{u8"Перетин",16,355,NULL},
{u8"Центр",17,355,NULL},
{u8"пЕрпендикулярний",18,355,NULL},
{u8"Дотичній",19,355,NULL},
{u8"Найближчий",20,355,NULL},
{u8"Точка ",21,355,NULL},

{u8"Кінець",' ',639,NULL},
{u8"Найближчий кінець",' ' ,635,NULL},
{u8"Медіана",' ',643,NULL},
{u8"Перетин",' ',641,NULL},
{u8"Центр",' ',645,NULL},
{u8"пЕрпендикулярний",' ',637,NULL},
{u8"Дотичній",' ' ,636,NULL},
{u8"Суміжний",' ',640,NULL},
{u8"Точка ",' ',644,NULL},

{u8"Кінець режим",639+1920,353,NULL},
{u8"Найближчий кінець режим",640+1920,353,NULL},
{u8"Медіана режим",561+1920,353,NULL},
{u8"Перетин режим",641+1920,353,NULL},
{u8"Центр режим",645+1920,353,NULL},
{u8"пЕрпендикулярний режим",637+1920,353,NULL},
{u8"Дотичній режим",636+1920,353,NULL},
{u8"Суміжний режим",643+1920,353,NULL},
{u8"Точка режим",644+1920,353,NULL},
{u8"Видалити режим",550+1920,353,NULL},

{u8"Копіювати об'єкт під курсором",839 + _1920_,353,NULL},
{u8"Налаштування шарів",636 + _1920_,353,NULL},
{u8"Налаштування шарів (варіант)",645 + _1920_,353,NULL},

{u8"Коротка інструкція",702 + 1920,534,NULL},

};


#define _ENDPOINT_ L'К'
#define _ENDPOINT_SC_ 18
#define _NEAREST_ L'Б'
#define _NEAREST_SC_ 72
#define _MIDDLE_ L'С'
#define _MIDDLE_SC_ 3
#define _INTERSECTION_ L'П'
#define _INTERSECTION_SC_ 7
#define _CENTER_ L'Ц'
#define _CENTER_SC_ 23
#define _PERPENDICULAR_ L'Е'
#define _PERPENDICULAR_SC_ 20
#define _TANGENTIAL_ L'Д'
#define _TANGENTIAL_SC_ 12
#define _ADJACENT_ L'Н'
#define _ADJACENT_SC_ 25
#define _POINT_ L'Т'
#define _POINT_SC_ 14
#define _NOPE_ L'В'
#define _NOPE_SC_ 4

#endif

#ifdef __O_MAK__

#define _MACRO_ u8" МАКРО "

static POLE pmMakro[] = {
	{u8"Запис. Кінець-> Ctrl-End",L'З',92,NULL},
	{u8"Виконання",L'В',93,NULL},
	{u8"Написати",L'Н',94,NULL},
	{u8"Прочитати",L'П',95,NULL} };

#define _DRAW__ L'н';
#define _TEXT__ L'т';

#endif

#ifdef __O_PAR_B__

extern TMENU mGruboscLinii;
extern POLE pmGruboscLinii[];
extern PPOLE pmCzcionka[];

#define MaxMenuWinFont 32
#define FONTNAMELEN 33 //12

char *punits[] = { u8"[мм]",u8"[см]",u8"[м]",u8"[км]","[\"]","[']","[yd]","[mi]" };
char* upunits[] = { u8"мм",u8"см",u8"м",u8"км","\"","'","yd","mi"};

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
    {"US Legal\0  215 *  355",' ',0,NULL}
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
		 {u8"Макс\0",L'М',0,NULL} };

static TMENU mKursorSize = { 10,0,0,5,24,8,TADD,CMNU,CMBR,CMTX,0,89,0,0,0,(POLE(*)[]) &pmKursorSize,NULL,NULL };
static TMENU mKursorSizeB = { 10,0,0,5,24,8,TADD,CMNU,CMBR,CMTX,0,99,0,0,0,(POLE(*)[]) &pmKursorSize,NULL,NULL };

static POLE pmKursorTyp[2] = {
		 {u8"Хрест\0",L'Х',594,&mKursorSize},
		 {u8"Дуплекс\0",L'Д',595,&mKursorSizeB} };

static TMENU mKursorTyp = { 2,0,0,4,24,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmKursorTyp,NULL,NULL };

static POLE pmSkalaF[15] = {
		 {u8"[А] 1 :\0 1",L'А',0,NULL},
		 {u8"[Б] 1 :\0 2",L'Б',0,NULL},
		 {u8"[В] 1 :\0 5",L'В',0,NULL},
		 {u8"[Г] 1 :\0 10",L'D',0,NULL},
		 {u8"[Ґ] 1 :\0 20",L'E',0,NULL},
		 {u8"[Д] 1 :\0 25",L'F',0,NULL},
		 {u8"[Е] 1 :\0 50",L'G',0,NULL},
		 {u8"[Є] 1 :\0 100",L'H',0,NULL},
		 {u8"[Ж] 1 :\0 500",L'I',0,NULL},
		 {u8"[З] 1 :\0 1000",L'J',0,NULL},
		 {u8"[И] 1 :\0 5000",L'K',0,NULL},
		 {u8"[І] 1 :\0 10000",L'L',0,NULL},
		 {u8"[Ї]    \0 2 : 1",L'M',0,NULL},
		 {u8"[Й]    \0 5 : 1",L'N',0,NULL},
		 {u8"[Х] 1 :\0 X\0         ",L'Х',0,NULL} };

static TMENU mSkalaF = { 15,0,0,15,24,6,TADD,CMNU,CMBR,CMTX,0,57,0,0,0,(POLE(*)[]) &pmSkalaF,NULL,NULL };

static POLE pmDokladnosc[6] = {
		 {u8"[1]\0 0.05 мм",'1',0,NULL},
		 {u8"[2]\0 0.1 мм",'2',0,NULL},
		 {u8"[3]\0 0.2 мм",'3',0,NULL},
		 {u8"[4]\0 0.25 мм",'4',0,NULL},
		 {u8"[5]\0 0.5 мм",'5',0,NULL},
		 {u8"[X]\0 X мм\0",L'Х',0,NULL} };

static TMENU mDokladnosc = { 6,0,0,13,24,7,TADD,CMNU,CMBR,CMTX,0,72,0,0,0,(POLE(*)[]) &pmDokladnosc,NULL,NULL };

static POLE pmJednostki[8] = {
		 {u8"[1] міліметр\0[мм]",'1',0,NULL},
		 {u8"[2] сантиметр\0[см]",'2',0,NULL},
		 {u8"[3] метр\0[м]",'3',0,NULL},
         {u8"[4] кілометр\0[км]",'4',0,NULL},
		 {u8"[5] дюйм\0[\"]",'5',0,NULL},
		 {u8"[6] фут\0[']",'6',0,NULL},
         {u8"[7] ярд\0[yd]",'7',0,NULL},
         {u8"[8] миля\0[mi]",'8',0,NULL}};

static TMENU mJednostki = { 8,0,0,18,24,8,TADD,CMNU,CMBR,CMTX,0,78,0,0,0,(POLE(*)[]) &pmJednostki,NULL,NULL };

static POLE pmKartezjanskiGeodezyjny[2] = {
		 {u8"Декартова",L'Д',104,NULL},
		 {u8"Геодезична",L'Г',105,NULL} };
	 
static TMENU mKartezjanskiGeodezyjny = { 2,0,0,12,24,8,ICONS,CMNU,CMBR,CMTX,0,109,0,0,0,(POLE(*)[]) &pmKartezjanskiGeodezyjny,NULL,NULL };

static POLE pmPointOrigin[3] = {
		 {u8"Початкова точка\0 ",L'П',106,NULL},
		 {u8"Вісь локальний координат.\0",L'В',107,NULL},
		 {u8"Кут повороту\0",L'К',108,NULL } };

static TMENU mPointOrigin = { 3,0,0,19,24,9,ICONS,CMNU,CMBR,CMTX,0,86,0,0,0,(POLE(*)[]) &pmPointOrigin,NULL,NULL };

static char* tablica_styli[] = { u8"Верх",u8"Низ",u8"Верх&Низ",u8"Довкола" };

static POLE pmSektory_Styl[4] = {
		 {u8"Верх",L'В',498,NULL},
		 {u8"Низ",L'Н',499,NULL},
		 {u8"верх & низ",L'Г',500,NULL},
		 {u8"Довкола",L'Д',486,NULL} };

static TMENU mSektory_Styl = { 4,0,0,10,30,7,ICONS,CMNU,CMBR,CMTX,0,65,0,0,0,(POLE(*)[]) &pmSektory_Styl,NULL,NULL };

static POLE pmSektory_Arkusza[15] = {
	   {u8"Рамка ввімкнена\0 Н\0",L'Р',485,&mRamka_on_off},
	   {u8"Зони ввімкнено\0 Н\0",L'З',486,&mSektory_on_off},
	   {u8"Префікс номера зони\0\0",L'П',487,NULL},
	   {u8"Нумерація від\0\0",L'Н',488,NULL},
	   {u8"пЕревернутий\0 Н\0",L'Е',489,&mReversed_on_off},
	   {u8"Висота сектора [мм]\0\0",L'В',490,NULL},
	   {u8"Довжина [мм]\0\0",L'Д',491,NULL},
	   {u8"Ширина сектора [мм]\0\0",L'Ш',492,NULL},
	   {u8"зМіщення рамки [мм]\0\0",L'М',501,NULL},
	   {u8"Товщина лінії рамки\0Тонка\0",  L'Т',493,&mGruboscLiniiF},
	   {u8"товщина Лінії сектора\0Товста\0",  L'Л',494,&mGruboscLiniiZ},
	   {u8"֎Колір рамки\0білий\0",     L'К',495,&mKolorACF},
	   {u8"֎кОлір зони\0білий\0",     L'О',496,&mKolorACZ},
	   {u8"Стиль\0Верх\0",L'С',497,&mSektory_Styl},
	   {u8"шриФт\0\0", L'Ф', 183,&mCzcionkaZ} };

static POLE pmPrecision[] = {
        {u8"точність Сили\0\0",         L'С',727,NULL},
        {u8"точність Моменту\0\0",      L'М',728,NULL},
        {u8"точність Зміщення\0\0",     L'З',730,NULL},
        {u8"точність Обертання\0\0",    L'О',731,NULL},
        {u8"точність Навантаження\0\0", L'Н',733,NULL},
        {u8"точність Теплового навантаження\0\0",L'Т',752,NULL},
        {u8"точність наПруги\0 \0",     L'П',800,NULL},
        {u8"точність Реакції\0 \0",L'Р',763,NULL},
};

static TMENU mPrecision = { 8,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,18,0,0,0,(POLE(*)[]) &pmPrecision,NULL,NULL };

static POLE pmThermal[] = {
       {u8"Глибина розрізу\0 \0",L'Г',764,NULL},
	   {u8"зміна Температури\0 \0",L'Т',765,NULL},
	  };

static TMENU mThermal = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,22,0,0,0,(POLE(*)[]) &pmThermal,NULL,NULL };

static POLE pmStaticColors[] = {
       {u8"֎Геометрія\0сірий\0",                    L'Г',723,&mKolorSTATIC},
       {u8"֎сили Розтягування\0червоні\0",          L'Р',771,&mKolorSTATIC},
	   {u8"֎сили Стискаючі\0сині\0",                L'С',758,&mKolorSTATIC},
       {u8"֎сили Зсуву\0зелений\0",                 L'З',759,&mKolorSTATIC},
       {u8"֎Моменти\0пурпуровий\0",                 L'М',760,&mKolorSTATIC},
       {u8"֎Деформації\0жовті\0",                   L'Д',761,&mKolorSTATIC},
       {u8"֎реакції Підтримки\0ціан\0",             L'П',763,&mKolorSTATIC},
       {u8"֎розтягуючі Напруги\0темно-червоний\0",  L'Н',801,&mKolorSTATIC},
       {u8"֎сТискаючі напруги\0темно-синій\0",      L'Т',802,&mKolorSTATIC},
       {u8"֎напруги зсУву\0темно-зелений\0",        L'У',803,&mKolorSTATIC},
       {u8"֎Вібрації\0інше\0",                      L'В',814,&mKolorSTATIC},
	  };

static TMENU mStaticColors = { 11,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,26,0,0,0,(POLE(*)[]) &pmStaticColors,NULL,NULL };

static POLE pmLoadMagnitude[] = {
       {u8"Лінійне навантаження\0 \0",L'Л',733,NULL},
	   {u8"Поверхневе навантаження\0 \0",L'П',845,NULL},
	  };

static TMENU mLoadMagnitude = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,43,0,0,0,(POLE(*)[]) &pmLoadMagnitude,NULL,NULL };

static POLE pmReactionMagnitude[] = {
       {u8"Зосереджені сили реакції\0 \0",L'З',762,NULL},
	   {u8"Розподілені сили реакції\0 \0",L'Р',864,NULL},
	  };

static TMENU mReactionMagnitude = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,49,0,0,0,(POLE(*)[]) &pmReactionMagnitude,NULL,NULL };


static POLE pmStressMagnitude[] = {
       {u8"напруження в Сталі/деревині\0 \0",L'С',775,NULL},
	   {u8"напруження в Залізобетоні\0 \0",L'З',6,NULL},
	  };

static TMENU mStressMagnitude = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,45,0,0,0,(POLE(*)[]) &pmStressMagnitude,NULL,NULL };

static POLE pmResetMagnitude[] = {
       {u8"SI\0 \0",L'S',858,NULL},
	   {u8"Imperial\0 \0",L'I',859,NULL},
	  };

static TMENU mResetMagnitude = { 2,0,0,10,30,7,TADD | ICONS,CMNU,CMBR,CMTX,0,47,0,0,0,(POLE(*)[]) &pmResetMagnitude,NULL,NULL };

static POLE pmMagnitude[] = {
       {u8"масштаб Сили\0\0",           L'С',727,NULL},
	   {u8"масштаб Моменту\0\0",        L'М',728,NULL},
	   {u8"масштаб Зміщення\0\0",       L'З',730,NULL},
	   {u8"масштаб Обертання\0\0",      L'О',731,NULL},
       {u8"масштаб Навантаження\0\0",   L'Н',733,&mLoadMagnitude},
       {u8"масштаб Теплові\0\0",        L'Т',752,NULL}, //&mThermal},
       {u8"масштаб осьових сил\0\0",    L'А',758,NULL},
       {u8"масштаб сил зсуву\0\0",      L'Б',759,NULL},
       {u8"масштаб момЕнтів\0\0",       L'Е',760,NULL},
       {u8"масштаб Деформації\0\0",     L'Д',761,NULL},
       {u8"масштаб сил Реакції\0\0",    L'Р',762,&mReactionMagnitude},
       {u8"масштаб моментів опор\0\0",  L'У',763,NULL},
       {u8"масштаб наПружень\0 \0",     L'П',800,&mStressMagnitude},
       {u8"масштаб % армування\0 \0",   L'%',823,NULL},
       {u8"перебільшення Вібрацій\0 \0",L'В',814,NULL},
       {u8"точність Чисел\0\0",         L'Ч',184,NULL}, //&mPrecision},
       {u8"Кольори\0\0",                L'К',495,NULL}, //&mStaticColors},
       {u8"скинути масштабування\0 \0", L'І',860,&mResetMagnitude},
};

static TMENU mMagnitude = { 18,0,0,10,30,7,TADD | ICONS | NOWCLOSE,CMNU,CMBR,CMTX,0,111,0,0,0,(POLE(*)[]) &pmMagnitude,NULL,NULL };

static POLE pmParametry[] = {
	 {u8"Формат\0 A4\0     ",L'Ф',96,&mFormat_r},
	 {u8"Масштаб\0 1:5\0     ",L'М',97,&mSkalaF},
	 {u8"Одиниці\0       мм\0    ",L'О',99,&mJednostki},
     {u8"Точність\0 0.25мм\0    ",L'Т',98,&mDokladnosc},
	 {u8"Система координат\0 K\0 ",L'C',100,&mKartezjanskiGeodezyjny},
	 {u8"Локальний система координат\0 \0       ",L'Л',101,&mPointOrigin},
	 {u8"Зони\0 \0      ",L'З',486,NULL},
	 {u8"тип і розмір Покажчика\0 Макс\0  ",L'П',102,&mKursorTyp},
     {u8"масштаб Векторів,точність,колір\0\0  ",L'В',751, NULL}, //&mMagnitude},
	 {u8"Редагувати ALFACADUA.INI\0 \0     ",L'Р',103,NULL},
};

static char* ParTxt[] = { u8"Д", "Г", "Т", "Н", };

#define _CARTESIAN_ u8"Д"
#define _GEODETIC_ u8"Г"
#define _MAX_ u8"Макс"
#define _PLUSMAX_ u8"+Макс"
#define _OTHER_COLOR_ u8"֍[М] інший>>> "

#define _SECTORS_        u8"ЗОНИ"

static char config_sectors[11][48] =
{
	u8"Товщина лінії рамки",
	u8"Товщина лінії зони",
	u8"Колір рамки",
	u8"Колір зони",
	u8"Зміщення рамки",
	u8"Висота зони",
	u8"Ширина зони",
	u8"Ширина поля",
	u8"Шрифт зони",
	u8"Зонний стиль",
	u8"Зворотна нумерація",
};

#define _Yes_ u8"Так"
#define _No_ u8"Ні"
#define _YES_ L'Т'
#define _yes_ L'т'
#define _NO_ L'Н'
#define _no_ L'н'

#define _reset_mgnitude_to_ u8"Бажаєте відновити коефіцієнти масштабування за замовчуванням для системи вимірювання:"
#define _SI_ u8"SI"
#define _IMP_ u8"Imperial"

#endif

#ifdef __O_LIBFUN__

char bloki_dir[MAXPATH] = u8"БЛОКИ";
char katalogi_dir[MAXPATH] = u8"КАТАЛОГИ";
char bloki_cfg[MAXPATH] = u8"БЛОКИ.CFG";
char biblioteka_cfg[MAXPATH] = u8"АПАРАТУРА.AXX";

#endif

#ifdef __O_FOLDERSDIALOG__

#define _SELECT_FOLDER_  L"Виберіть фолдєр або введіть .. або / або // для більш"
char error_message_block[64] = u8"У цьому каталозі немає блоків";
char error_message_background[64] = u8"У цьому каталозі немає шаблонів";

#endif

#ifdef __BIB_E__

char* view_width_tab[] = { u8"д.т.",u8"т.",u8"Т.",u8"Д.Т.",u8"С.Т.", "", "", u8"прихований " };

char* view_edge_tab[] = {u8"Без опори", u8"", u8"", u8"", u8"", u8"Без опори", u8"Шарнірно-нерухома опора", u8"Жорстке затиснення"};
char* view_edgeinverted_tab[] = {u8"Звичайний", u8"Перевернутий"};

char* view_type_tab[] = {
	u8"безперервна",
	u8"пунктирна",
	u8"штрихпунктирна",
	u8"2-крапкова",
	u8"крапкова",
	u8"кордон",
	u8"центр",
	u8"прихована",
	u8"фантом",
	u8"пунктирна_2",
	u8"штрихпунктирна_2",
	u8"2-крапкова_2",
	u8"крапкова_2"
	u8"кордон_2",
	u8"центр_2",
	u8"прихована_2",
	u8"фантом_2",
	u8"пунктирна_x2",
	u8"штрихпунктирна_x2",
	u8"2-крапкова_x2",
	u8"крапкова_x2",
	u8"кордон_x2",
	u8"центр_x2",
	u8"прихована_x2",
	u8"фантом_x2",
	u8"3-крапкова",
	u8"2-центр",
	u8"3-пунктирна",
	u8"фантом_3",
	u8"2-центр_x2",
	u8"3-пунктирна_x2",
	u8"багатоштрихова",
	u8"" };

T_Prototype      s__prot = { u8"прототип.alf", TRUE };

#define _SYSTEM_MESSAGE_ u8"СИСТЕМНЕ ПОВІДОМЛЕННЯ"
#define _INTERRAPTED_ u8"Перерваний"
#define _TOO_MANY_BLOCKS_ u8"Забагато блоків, щоб перевірити всі"
#define _BLOCKS_CORRECTED_ u8"%ld блоків, %ld об'єктів виправлено"
#define _BLOCKS_TESTED_ u8"Все добре. Перевірено %d блоків"


char* typ_punktu_inf[] = { u8"Простий",u8"Базова точка","","","","","",u8"Соединение",u8"Штифт","","","",
                           u8"Жорстке",u8"Жорстке Л",u8"Жорстке П", u8"Жорстке В", u8"Шарнірне", u8"Шарнірне Л", u8"Шарнірне П", u8"Шарнірне В",
                           u8"Жорстке-ковзання", u8"Жорстке-ковзання Л", u8"Жорстке-ковзання П", u8"Жорстке-ковзання В", u8"Шарнірне-ковзання", u8"Шарнірне-ковзання Л", u8"Шарнірне-ковзання П", u8"Шарнірне-ковзання В", u8"немає обертання Z",
                           "","","","","","","","",
                           };

#endif

#ifdef __O_LISTBL__

#define _NUMBER_OF_BLOCKS_ u8"Кількість блоків ("
#define _EXCEEDS_NO_ u8") перевищує 5000"

#endif

#ifdef __O_ETEXT__

#define _YES_NO_ESC_ u8"ТтНн\033"

POLE pmenu_typ[] = {
			   {u8"*ALL",L'*',0,NULL},       //255
			   {u8"Звичайний",L'З',0,NULL},
			   {u8"Внутрішня етикетка",L'В',0,NULL},
			   {u8"мІтка компонента",L'І',0,NULL},
			   {u8"Символ",L'С',0,NULL},
			   {u8"Тип",L'Т',0,NULL},
			   {u8"?ім'я шпильки",L'?',0,NULL},
			   {u8"блок Живлення",L'Ж',0,NULL},
			   {u8"Порт",L'П',0,NULL},
			   {u8"Атрибут",L'А',0,NULL},
			   {u8"+Схема",L'+',0,NULL},
			   {u8"ім'я Файлу",L'Ф',0,NULL},
			   {u8"Мережа",L'М',0,NULL},
			   {u8"Коментар",L'К',0,NULL},
			   {u8"%змінна",L'%',0,NULL},
			   {u8"Допом. символ",L'Д',0,NULL},
			   {u8"допом. Опис",L'О',0,NULL},
};

POLE pmenu_typ1[] = {
			   {u8"СИМВОЛ",L'С',0,NULL},           //3 
			   {u8"*ALL",L'*',0,NULL},       //255
			   {u8"Звичайний",L'З',0,NULL},
			   {u8"Внутрішня етикетка",L'В',0,NULL},
			   {u8"мІтка компонента",L'І',0,NULL},
			   {u8"Символ",L'С',0,NULL},
			   {u8"Тип",L'Т',0,NULL},
			   {u8"?ім'я шпильки",L'?',0,NULL},
			   {u8"блок Живлення",L'Ж',0,NULL},
			   {u8"Порт",L'П',0,NULL},
			   {u8"Атрибут",L'А',0,NULL},
			   {u8"+Схема",L'+',0,NULL},
			   {u8"ім'я Файлу",L'Ф',0,NULL},
			   {u8"Мережа",L'М',0,NULL},
			   {u8"Коментар",L'К',0,NULL},
			   {u8"%змінна",L'%',0,NULL},
			   {u8"Допом. символ",L'Д',0,NULL},
			   {u8"допом. Опис",L'О',0,NULL},
};

POLE pmenu_auto[] = {
			   {u8"змінити Все",L'В',615,NULL},
			   {u8"Підтвердьте кожен",L'П',616,NULL},
};

POLE pmenu_change[] = {
			   {u8"Змінити",L'З',112,NULL},
			   {u8"Пропустити",L'П',113,NULL},
};

#endif

#ifdef __O_TEXT_B__

#define _YES_NO_ESC_ u8"ТтНн\033"
#define _YES_ L'Т'
#define _yes_ L'т'
#define _EDIT_ L'Р'
#define _edit_ L'р'

#define _EDIT_SC_ 8

static TMENU mItalics = { 2,0,0,7,79,9,ICONS,CMNU,CMBR,CMTX,0,13,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUkryty = { 2,0,0,7,79,11,ICONS,CMNU,CMBR,CMTX,0,4,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mBold = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,37,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUnderlined = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,39,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAutoInterwal = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmTypTekstu[] = {
			   {u8"Звичайний",L'З',0,NULL},
			   {u8"Внутрішня етикетка",L'В',0,NULL},
			   {u8"етИкетка компонента",L'И',0,NULL},
			   {u8"Символ",L'С',0,NULL},
			   {u8"Тип",L'Т',0,NULL},
			   {u8"Ім'я шпильки",L'І',0,NULL},
			   {u8"блок Живлення",L'Ж',0,NULL},
			   {u8"Порт",L'П',0,NULL},
			   {u8"Атрибут",L'А',0,NULL},
			   {u8"+Схема",L'+',0,NULL},
			   {u8"ім'я Файлу",L'Ф',0,NULL},
			   {u8"Мережа",L'М',0,NULL},
			   {u8"Коментар",L'К',0,NULL},
			   {u8"%змінна",L'%',0,NULL},
			   {u8"Допом. символ",L'Д',0,NULL},
			   {u8"допом. Опис",L'О',0,NULL},
};

TMENU mTypTekstu = { 16,0,0,16,74,6,0,CMNU,CMBR,CMTX,0,15,0,0,0,(POLE(*)[]) &pmTypTekstu,NULL,NULL };

POLE pmJustowanie[] = {
					{u8"Зліва",L'Щ',262,NULL},
					{u8"Направо",L'Н',263,NULL},
					{u8"Середній",L'С',264,NULL},
					{u8"Центральний",L'Ц',265,NULL},
};

TMENU mJustowanie = { 4,0,0,13,74,6,ICONS,CMNU,CMBR,CMTX,0,31,0,0,0,(POLE(*)[]) &pmJustowanie,NULL,NULL };


POLE pmKatT[] = {
		{u8"0",L'0',282,NULL},
		{u8"90",L'9',283,NULL},
		{u8"=",L'=',284,NULL},
		{u8"+",L'+',285,NULL},
		{u8"До",L'Д',286,NULL},
		{u8"Кут",L'К',287,NULL} };

TMENU mKatT = { 6,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,41,0,0,0,(POLE(*)[]) &pmKatT,NULL,NULL };

extern TMENU mLineSpacing;
extern PTMENU mCzcionka;

static POLE pmTekstm[] = {
	{u8"Редагувати знову  \0", L'Р',38,NULL},
	{u8"Кут   \0", L'К',229,&mKatT},
	{u8"Висота\03мм\0", L'В',179,NULL},
	{u8"Факторширини\01.0\0", L'Ф',230,NULL},
	{u8"кУрсив\0Н\0", L'У', 181,&mItalics},
	{u8"Тип\0\0", L'Т', 231,&mTypTekstu},
	{u8"Схований\0Н\0", L'С', 232,&mUkryty},
	{u8"Жирний\0Н\0", L'Ж', 182,&mBold},
	{u8"Підкреслено\0Н\0", L'П', 409,&mUnderlined},
	{u8"вИрівняти\0|<\0", L'И', 233,&mJustowanie},
	{u8"Міжрядковий інтервал\01.0\0", L'М', 410,&mLineSpacing},
	{u8"Автоінтервал\0Н\0", L'А', 91,&mAutoInterwal},
	{u8"Інтервал\01\0", L'І', 234,NULL},
	{u8"Шрифт\0\0", L'Ш', 183,&mCzcionka},
};

static char t_t[16][32] = {
u8"Звичайний",u8"етикетка",u8"ет.компонента",u8"Символ",u8"Тип","Ім'я шпильки",u8"блок Живлення",u8"Порт",
u8"Атрибут",u8"+Схема",u8"ім'я Файлу",u8"Мережа",u8"Коментар",u8"%змінна",u8"Допом.символ",u8"допом.Опис" };

#define _TEXT_ u8"тЕкст"
#define _TEXT_C_ L'Е'
#define _HIDDEN_ u8" схований"

#endif

#ifdef __B_WINDEM__

#define _NOT_LICENSED_ u8"Ваша пожертва підтримує проект"
#define _NOT_LICENSED1_ u8" Дякую."
#define _DEM_ROWS_NO_ 12
char lic_for[32] = { u8"Користувач: " };

POLE pmDem1[] = {
	/*0*/	 {u8"",' ',0,NULL},
#ifdef BIT64
	/*1*/	 {u8"Створення нового вікна AlfaCAD (x64)",' ',0,NULL},
#else
	/*1*/	 {u8"Створення нового вікна AlfaCAD",' ',0,NULL},
#endif
	/*7*/{u8"_______________________________________________",' ',0,NULL},
	/*10*/   {u8"",' ',0,NULL},
};

POLE pmPrint[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8"   друкування   ",' ',0,NULL},
	/*2*/	 {u8"    Переривання -> Esc    ",' ',0,NULL},
	/*3*/  {u8"",' ',0,NULL},
};

POLE pmPlot[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8"   Малювання   ",' ',0,NULL},
	/*2*/	 {u8"",' ',0,NULL},
};

POLE pmPlot_to_com[] = {
	/*0*/	 {u8"",' ',0,NULL},
	/*1*/	 {u8" НАДСИЛАННЯ ДАНИХ НА ПЛОТТЕР ",' ',0,NULL},
	/*2*/	 {u8"",' ',0,NULL},
};

T_Prot_Date s_prot_date = { PROT_KEY, 0, 1, 0, 0,u8"Користувач: ", u8"Серія: ",
0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "",
0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "", 0, 0, "" };

#endif

#ifdef __O_OBJECT1__

#define _YES_NO_ESC_ u8"ТтНн\033"
#define _YES_ L'Т'
#define _yes_ L'т'
#define _NO_ L'Н'
#define _no_ L'н'

#endif

#ifdef __O_ARRAY__

static POLE pmArray_polar[] = {
	{u8"Кількість елементів\0 1\0     ",L'К',607,NULL},
	{u8"кут Заповнення [°]\0 360\0     ",L'З',608,NULL} };

static POLE pmArray_polar_rot[] = {
	{u8"Кількість елементів\0 1\0     ",L'К',609,NULL},
	{u8"кут Заповнення [°]\0 360\0     ",L'З',610,NULL} };

#define _ARRAY_WITH_ROTATION_ u8"Матриця з обертанням"
#define _ARRAY_ u8"Матриця"
#define _ARRAY_C_ L'М'

#endif

#ifdef  __BIBWYM4__

#define _DIMENSIONING_ u8"Розміровка"
#define _DIMENSIONING_C_ L'Р'

#endif

#ifdef __BIBWYM_B_

static POLE pmKierunek[7] = {
		{u8"0°",L'0',203,NULL},
		{u8"90°",L'9',204,NULL},
		{u8"Паралельно з",L'П',205,NULL},
		{u8"До",L'Д',206,NULL},
		{u8"Кут",L'К',207,NULL},
		{u8"Розміровка кута",L'Р',208,NULL},
        {u8"провідна Лінія",L'Л',678,NULL} };

static TMENU mKierunek = { 7,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,9,0,0,0,(POLE(*)[]) &pmKierunek,NULL,NULL };

static POLE pmWym_luk_o[2] = {
		{u8"Радіус",L'Р',209,NULL},
		{u8"Довжина дуги",L'Д',201,NULL} };

static TMENU mWym_luk_o = { 2,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,5,0,0,0,(POLE(*)[]) &pmWym_luk_o,NULL,NULL };


static POLE pmWym_okrag_o[2] = {
		{u8"Діаметр",L'Д',211,NULL},
		{u8"Радіус",L'Р',212,NULL} };

static TMENU mWym_okrag_o = { 2,0,0,5,72,6,ICONS,CMNU,CMBR,CMTX,0,7,0,0,0,(POLE(*)[]) &pmWym_okrag_o,NULL,NULL };

static POLE pmWymiarujm[] = {
		{u8"Напрямок\0=\0",L'Н',196,&mKierunek},
		{u8"нОвий\0End",L'О',197,NULL},
		{u8"Видалити\0Del",L'В',198,NULL},
		{u8"Редагувати\0F10",L'Р',199,NULL},
		{u8"конФіг\0",L'Ф',200,NULL},
		{u8"Дуга\0R\0",L'Д',201,&mWym_luk_o},
		{u8"Коло\0D\0",L'К',202,&mWym_okrag_o}, };

#define _NEW_ u8"новий"

static POLE pmStrzalka[3] = {
	  {u8"Наконечники стріл",L'A',189,NULL},
	  {u8"Галочка",L'Г',190,NULL},
	  {u8"Точки",L'Т',191,NULL} };

static POLE pmKierunekT[3] = {
	{u8"Паралельний",L'П',192,NULL},
    {u8"Колінеарний",L'К',696,NULL},
	{u8"Горизонтальний",L'Г',193,NULL} };

static POLE pmAddLine[2] = {
	{u8"Фіксована довжина",L'Ф',194,NULL},
	{u8"Вирівняти за об’єктом",L'В',195,NULL} };

static POLE pmLFormat[4] = {
	{u8"Десятковий",L'Д',0,NULL},
    {u8"Інженерний",L'І',0,NULL},
	{u8"Архітектурний",L'А',0,NULL},
    {u8"дРобовий",L'Р',0,NULL},
};

static POLE pmFractions[9] = {
    {u8"0",   L'0',0,NULL},
	{u8"1/2",  L'2',0,NULL},
    {u8"1/4",  L'4',0,NULL},
    {u8"1/8",  L'8',0,NULL},
    {u8"1/16", L'1',0,NULL},
    {u8"1/32", L'3',0,NULL},
    {u8"1/64", L'6',0,NULL},
    {u8"1/128",L'А',0,NULL},
    {u8"1/256",L'Б',0,NULL},
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
	  {u8"֎колір Лінії\0", L'Л',177,&mKolorL},
	  {u8"֎колір Тексту\0", L'Т',178,&mKolorT},
	  {u8"Висота тексту\0", L'В',179,NULL},
	  {u8"Фактор ширини\0", L'Ф',180,NULL},
	  {u8"текст Курсивом\0", L'К',181, &mItalics},
	  {u8"Жирний текст\0", L'Ж',182, &mBold},
	  {u8"Шрифт\0", L'Ш',183, &mCzcionkaW},
      {u8"Числовий формат\0", L'Ч',697, &mLFormat},
	  {u8"тОчність\0", L'О',184,NULL},
	  {u8"тЕрмінатори\0", L'Е',185,&mStrzalka},
	  {u8"Зміщення лінії\0", L'З',186,NULL},
	  {u8"Напрямок тексту\0", L'Н',187,&mKierunekT} ,
	  {u8"вИносні лінії\0", L'И',188,&mAddLine} };

char *_LFORMAT_[]={u8"Десятковий",u8"Інженерний",u8"Архітектурний",u8"Дробовий"};

#endif

#ifdef __O_DIVIDE__

POLE pmDiv_Pline[] = {
	{u8"Полилінія",L'П',454,NULL},
	{u8"Сегмент",L'С',455,NULL} };

static TMENU mDiv_Pline = { 2, 0,0, 12, 88, 12, ICONS, CMNU,CMBR,CMTX,0, 8, 0, 0,0,(POLE(*)[]) &pmDiv_Pline, NULL, NULL };
static TMENU mAlign_Block = { 2, 0,0, 7, 83, 13, ICONS, CMNU,CMBR,CMTX,0, 10, 0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

static POLE pmDivide[] = {
	 {u8"Поділяти\0",L'П',448,NULL},
	 {u8"Кількість сегментів\0 5\0      ", L'К', 449,NULL},
	 {u8"Виміряти\0", L'В',450, NULL},
	 {u8"Довжина сегмента\0 100.0\0       ", L'Д',451, NULL},
	 {u8"Максимум\0 1000\0",L'М', 452,NULL},
	 {u8"полилінія/Сегмент\0  полілінія\0  ",L'С',453, &mDiv_Pline},
	 {u8"видаЛити\0Del\0",L'Л', 270,NULL},
	 {u8"матч Блок\0 Y\0",L'Б', 456,&mAlign_Block} };

#define _POLYLINE_ u8"полілінія"
#define _SEGMENT_ u8"сегмент"

char _YES__[4] = u8"Т";
char _NO__[4] = u8"Н";

#define _DIVIDE_ u8"Поділяти"
#define _DIVIDE_C_ L'П'

#endif

#ifdef __O_KONSTR__

static char* opis_zbrojenia[] = { u8"Арматурний стержень" };

#endif

#ifdef __O_RECTA__

static POLE	pmKatR[]= {
	 {u8"повернути На кут",L'Н',579,NULL},
	 {u8"45°",	L'4',441,NULL},
	 {u8"90°",	L'9',442,NULL},
	 {u8"135°",	L'1',443,NULL},
	 {u8"180°",	L'8',444,NULL},
	 {u8"225°",	L'2',445,NULL},
	 {u8"270°",	L'7',446,NULL},
	 {u8"315°",	L'3',447,NULL},
	 {u8"0°",	    L'0',578,NULL},
	 {u8"повернути До кута",L'Д',440,NULL},
	 {u8"Паралельно до",L'П',580,NULL},
	 {u8"пеРпендикулярно до",L'Р',581,NULL},
	 {u8"під Кутом до",L'К',582,NULL},
};

TMENU mKatR = { 13,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmKatR,NULL,NULL };

static POLE	pmRectOpt[]= {
	 {u8"Дві точки",L'Д',22,NULL},
     {u8"дві точки та точка/кут Вставки",L'В',684,NULL},
};

TMENU mRectOpt = { 2,0,0,10,74,6,ICONS,CMNU,CMBR,CMTX,0,13,0,0,0,(POLE(*)[]) &pmRectOpt,NULL,NULL };

static POLE	pmKatOptR[]= {
	 {u8"повернути На кут",L'Н',579,NULL},
	 {u8"45°",	L'4',441,NULL},
	 {u8"90°",	L'9',442,NULL},
	 {u8"135°",	L'1',443,NULL},
	 {u8"180°",	L'8',444,NULL},
	 {u8"225°",	L'2',445,NULL},
	 {u8"270°",	L'7',446,NULL},
	 {u8"315°",	L'3',447,NULL},
	 {u8"0°",	    L'0',578,NULL},
	 {u8"повернути До кута",L'Д',440,NULL},
	 {u8"Паралельно до",L'П',580,NULL},
	 {u8"пеРпендикулярно до",L'Р',581,NULL},
	 {u8"під Кутом до",L'К',582,NULL},
     {u8"Варіант малювання",L'В',685,&mRectOpt},
};

TMENU mKatOptR = { 14,0,0,10,74,6,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmKatOptR,NULL,NULL };

#define _RECTA_A_ u8"Прямокутник: кут осі і варіанти малювання"
#define _RECTA_R_ u8"Прямокутник: обертання"
#define _RECTA_C_ L'П'

#endif

#ifdef __O_LTTYPE__

POLE pmTTF_OTF[] = {
   {u8"шрифту TrueType",L'Т',703,NULL},
   {u8"шрифту OpenType",L'О',704,NULL},
};

TMENU mTTF_OTF = { 2,0,0,8,22,9,ICONS | TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmTTF_OTF,NULL,NULL };

#endif