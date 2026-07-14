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

#ifdef __RYSUJ__
extern TMENU mSteelUS;
extern TMENU mSteelEU;
extern TMENU mSteelUK;
extern TMENU mTimberUS;
extern TMENU mWoodSpeciesUS;

POLE pmZmienTeksty[] = {
	{u8"Відшукувати текст",L'В',3,NULL},
	{u8"Змінити тексти",L'З',90, NULL},
	{u8"Автоматична нумерація", L'А',91, NULL},
	//	{u8"Przesunięcie numeracji", 'P', 0,NULL},
};

TMENU mZmienTeksty = { 3, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 15, 0, 0,0, (POLE(*)[]) & pmZmienTeksty, NULL, NULL };

POLE pmChangeProperties[] = {
	{u8"Змініти властивості",L'З',43, NULL},
	{u8"правильні криві Безьє", L'Б',648, NULL},
    {u8"Вектори", L'В',770, NULL},
    {u8"Особливості", L'О',138, NULL},
};

TMENU mChangeProperties = { 4, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 18, 0, 0,0,(POLE(*)[]) & pmChangeProperties, NULL, NULL };

POLE pmWoodMoistureUS[] = {
        {u8"Зелений",L'З',0,&mWoodSpeciesUS},
        {u8"12%",L'1',0,&mWoodSpeciesUS}};

TMENU mWoodMoistureUS = { 2,0,0,7,16,8,0,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

POLE pmSteelGradeUS[] = {
	{u8"Grade 40",L'4',0,NULL},
	{u8"Grade 60",L'6',0,NULL},
	{u8"Grade 75",L'7',0,NULL},
	{u8"Grade 80",L'8',0,NULL},
	};

TMENU mSteelGradeUS = { 4,0,0,10,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelGradeUS,NULL,NULL };

POLE pmSteelGradeEU[] = {
	{u8"B400B",L'1',0,NULL},
	{u8"B500B",L'2',0,NULL},
	{u8"B500C",L'3',0,NULL},
	};

TMENU mSteelGradeEU = { 3,0,0,10,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelGradeEU,NULL,NULL };

POLE pmConcreteClassUS[] = {
	{u8"3000 psi",L'3',0,&mSteelGradeUS},
	{u8"4000 psi",L'4',0,&mSteelGradeUS},
	{u8"5000 psi",L'5',0,&mSteelGradeUS},
	{u8"6000 psi",L'6',0,&mSteelGradeUS},
	{u8"8000 psi",L'8',0,&mSteelGradeUS},
	};

TMENU mConcreteClassUS = { 5,0,0,10,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmConcreteClassUS,NULL,NULL };

POLE pmConcreteClassEU[] = {
	{u8"C20/25",L'1',0,&mSteelGradeEU},
	{u8"C25/30",L'2',0,&mSteelGradeEU},
	{u8"C30/37",L'3',0,&mSteelGradeEU},
	{u8"C35/45",L'4',0,&mSteelGradeEU},
	{u8"C40/50",L'5',0,&mSteelGradeEU},
	{u8"C45/55",L'6',0,&mSteelGradeEU},
	{u8"C50/60",L'7',0,&mSteelGradeEU},
	};

TMENU mConcreteClassEU = { 7,0,0,10,16,8,TADD,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmConcreteClassEU,NULL,NULL };


POLE pmConcreteUS[] = {
	{u8"Прямокутник", L'П', 905, &mConcreteClassUS},
	{u8"Двотавр", L'Д', 906, &mConcreteClassUS},
	{u8"Тавр", L'Т', 907, &mConcreteClassUS},
	{u8"Циліндрична труба", L'Ц', 908, &mConcreteClassUS},
	{u8"Квадратна труба", L'К', 909, &mConcreteClassUS},
	{u8"Балка коробчаста", L'Б', 910, &mConcreteClassUS},
    {u8"плита", L'=', 911, &mConcreteClassUS},
    {u8"стіна-балка", L'Н', 912, &mConcreteClassUS},
};

TMENU mConcreteUS = { 8,0,0,32,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmConcreteUS,NULL,NULL };

POLE pmConcreteEU[] = {
	{u8"Прямокутник", L'П', 905, &mConcreteClassEU},
	{u8"Двотавр", L'Д', 906, &mConcreteClassEU},
	{u8"Тавр", L'Т', 907, &mConcreteClassEU},
	{u8"Циліндрична труба", L'Ц', 908, &mConcreteClassEU},
	{u8"Квадратна труба", L'К', 909, &mConcreteClassEU},
	{u8"Балка коробчаста", L'Б', 910, &mConcreteClassEU},
    {u8"плита", L'=', 911, &mConcreteClassEU},
    {u8"стіна-балка", L'Н', 912, &mConcreteClassEU},
};

TMENU mConcreteEU = { 8,0,0,32,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmConcreteEU,NULL,NULL };

TMENU mWoodMoistureCA = { 2,0,0,7,16,8,0,CMNU,CMBR,CMTX,0,168,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

POLE pmMaterialUS[] = {
        {u8"Сталь",L'С',775, NULL}, //&mSteelUS
        {u8"Залізобетон", L'З',904, NULL}, //ConcreteUS
        {u8"Деревина", L'Д',785, &mWoodMoistureUS}, //&mTimberUS
};

TMENU mMaterialUS = { 3, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 6, 0, 0,0,(POLE(*)[])&pmMaterialUS, NULL, NULL };

POLE pmMaterialCA[] = {
        {u8"Сталь",L'С',775, NULL}, //&mSteelCA
        {u8"Деревина", L'Д',785, &mWoodMoistureCA}, //&mTimberCA
};

TMENU mMaterialCA = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 8, 0, 0,0,(POLE(*)[])&pmMaterialCA, NULL, NULL };

POLE pmMaterialEU[] = {
        {u8"Сталь",L'С',775, NULL}, //&mSteelEU
        {u8"Залізобетон", L'З',904, NULL}, //ConcreteEU
};

TMENU mMaterialEU = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 0, 0, 0,0,(POLE(*)[])&pmMaterialEU, NULL, NULL };

POLE pmRegion[] = {
        {u8"США",L'С',772, &mMaterialUS},
        {u8"ЄС", L'Є',773, &mMaterialEU},
        {u8"Великобританія", L'В',774, NULL}, //&mSteelUK
        {u8"Канада", L'К',829, &mMaterialCA},
        {u8"Австралія", L'А',830, NULL}, //&mSteelAU
        {u8"Китай", L'К',831, NULL}, //&mSteelCN
};

TMENU mRegion = { 6 , 0, 0, 16, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 7, 0, 0,0,(POLE(*)[])&pmRegion, NULL, NULL };

POLE pmStatic[] = {
        {u8"Каркас і ферма",L'К',733, NULL},
        {u8"Плита",L'П',855, NULL},
        {u8"Стіна-балка",L'С',865, NULL},
        {u8"Ростверк",L'Р',897, NULL},
};

TMENU mStatic = { 4, 0, 0, 32, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 3, 0, 0,0,(POLE(*)[])&pmStatic, NULL, NULL };

POLE pmApplications[] = {
        {u8"Статичний і динамічний аналіз",L'С',854, &mStatic},
        {u8"Результатні сили в перерізах",L'Р',824, NULL},
        {u8"Анімувати динаміку",L'А',817, NULL},
        {u8"Вибір перерізу елемента",L'В',775, &mRegion},
};

TMENU mApplications = { 4, 0, 0, 32, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 22, 0, 0,0,(POLE(*)[])&pmApplications, NULL, NULL };

POLE pmenug[] = {
	{u8"Намалюй\0", L'Н',57,NULL},
	{u8"Блок\0",L'Б',1,NULL},
	{u8"Змінити\0",L'З',2,NULL},
	{u8"Виправляти",L'В',43,&mChangeProperties},
	{u8"Обмін тексти\0",L'О',4,&mZmienTeksty},
	{u8"Розміровка\0",L'Р',5,NULL},
	{u8"Штрихування\0",L'Ш',6,NULL},
	{u8"Геометрія\0",L'Г',7,NULL},
	{u8"Макрос\0",L'М',8,NULL},
	{u8"Параметри\0",L'П',9,NULL},
	{u8"конфІг\0",L'І',10,NULL},
    {u8"Додатки\0",L'Д',753,&mApplications},
	{u8"Файл\0",L'Ф',11,NULL},
	{u8"Кінець роботи\0",L'К',12,NULL},
	{u8"Лишати вікно\0",L'Л',598,NULL},
};

#define _BITMAPS_ u8"Бітмапи"
#define _PLUGINS_ u8"плагіни"
#define _PLUGIN_BITMAPS_ u8"plugins/bitmaps"

#define _NO_MOUSE_ u8"Миша не встановлена !!!!!\n"

char* loading_program[] = { u8"Програма завантаження", u8"Завантаження ресурсів..." , u8"Будь ласка, зачекайте. Завантаження файлу: ", u8"Підключення до хмари..."};



#endif