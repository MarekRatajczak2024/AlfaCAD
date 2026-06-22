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
	{u8"Szukaj tekstu\0",'S',3,NULL},
	{u8"Zmień teksty",'Z',90, NULL},
	{u8"Automatyczna numeracja", 'A', 91, NULL},
	//	{u8"Przesunięcie numeracji", 'P', 0,NULL},
};

TMENU mZmienTeksty = { 3, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 15, 0, 0,0,(POLE(*)[]) & pmZmienTeksty, NULL, NULL };

POLE pmChangeProperties[] = {
	{u8"Zmień",'Z',43, NULL},
	{u8"popraw krzywe Béziera", 'B',648, NULL},
    {u8"Wektory", 'W',770, NULL},
    {u8"Cechy", L'C',138, NULL},
};

TMENU mChangeProperties = { 4, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 18, 0, 0,0,(POLE(*)[]) & pmChangeProperties, NULL, NULL };

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
	{u8"Prostokąt", L'P', 905, &mConcreteClassUS},
	{u8"Dwuteownik", L'D', 906, &mConcreteClassUS},
	{u8"Teownik", L'T', 907, &mConcreteClassUS},
	{u8"Okrągły profil drążony", L'O', 908, &mConcreteClassUS},
	{u8"Kwadratowy profil drążony", L'K', 909, &mConcreteClassUS},
	{u8"Skrzynkowy profil drążony", L'S', 910, &mConcreteClassUS},
    {u8"płyta",L'=',911,&mConcreteClassUS},
    {u8"tarcza",L'H',912,&mConcreteClassUS},
};

TMENU mConcreteUS = { 8,0,0,32,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmConcreteUS,NULL,NULL };

POLE pmConcreteEU[] = {
	{u8"Prostokąt", L'P', 905, &mConcreteClassEU},
	{u8"Dwuteownik", L'D', 906, &mConcreteClassEU},
	{u8"Teownik", L'T', 907, &mConcreteClassEU},
	{u8"Okrągły profil drążony", L'O', 908, &mConcreteClassEU},
	{u8"Kwadratowy profil drążony", L'K', 909, &mConcreteClassEU},
	{u8"Skrzynkowy profil drążony", L'S', 910, &mConcreteClassEU},
    {u8"płyta",L'=',911,&mConcreteClassEU},
    {u8"tarcza",L'H',912,&mConcreteClassEU},
};

TMENU mConcreteEU = { 8,0,0,32,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmConcreteEU,NULL,NULL };

POLE pmWoodMoistureUS[] = {
        {u8"Zielone",L'Z',0,&mWoodSpeciesUS},
        {u8"12%",L'1',0,&mWoodSpeciesUS}};

TMENU mWoodMoistureUS = { 2,0,0,7,16,8,0,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

TMENU mWoodMoistureCA = { 2,0,0,7,16,8,0,CMNU,CMBR,CMTX,0,168,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

POLE pmMaterialUS[] = {
        {u8"Stal",'S',775, NULL}, //&mSteelUS
        {u8"Żelbet", 'Z',904, NULL}, //ConcreteUS
        {u8"Drewno", 'D',785, &mWoodMoistureUS}, //&mTimberUS
};

TMENU mMaterialUS = { 3, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 6, 0, 0,0,(POLE(*)[])&pmMaterialUS, NULL, NULL };

POLE pmMaterialCA[] = {
        {u8"Stal",'S',775, NULL}, //&mSteelCA
        {u8"Drewno", 'D',785, &mWoodMoistureCA}, //&mTimberCA
};

TMENU mMaterialCA = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 8, 0, 0,0,(POLE(*)[])&pmMaterialCA, NULL, NULL };

POLE pmMaterialEU[] = {
        {u8"Stal",'S',775, NULL}, //&mSteelCA
        {u8"Żelbet", 'Z',904, NULL}, //ConcreteEU
};

TMENU mMaterialEU = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 0, 0, 0,0,(POLE(*)[])&pmMaterialEU, NULL, NULL };

POLE pmRegion[] = {
        {u8"US",'U',772, &mMaterialUS},
        {u8"EU", 'E',773, &mMaterialEU},
        {u8"UK", 'B',774, NULL}, //&mSteelUK
        {u8"CA", 'C',829, &mMaterialCA},
        {u8"AU", 'A',830, NULL}, //&mSteelAU
        {u8"P.R.C", 'P',831, NULL}, //&mSteelCN
};

TMENU mRegion = { 6 , 0, 0, 8, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 7, 0, 0,0,(POLE(*)[])&pmRegion, NULL, NULL };

POLE pmStatic[] = {
        {u8"Rama i kratownica",L'R',733, NULL},
        {u8"Płyta",L'P',855, NULL},
        {u8"Tarcza",L'T',865, NULL},
        {u8"ruszt Belkowy",L'B',897, NULL},
};

TMENU mStatic = { 4, 0, 0, 32, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 3, 0, 0,0,(POLE(*)[])&pmStatic, NULL, NULL };

POLE pmApplications[] = {
        {u8"Statyka i dynamika",L'S',854, &mStatic},
        {u8"siły wynikowe w Przekrojach poprzecznych",L'P',824, NULL},
        {u8"Animuj dynamikę",L'A',817, NULL},
        {u8"Wybór przekrojów elementów",L'W',775, &mRegion},
};

TMENU mApplications = { 4, 0, 0, 22, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 22, 0, 0,0,(POLE(*)[])&pmApplications, NULL, NULL };


POLE pmenug[] = {
	{u8"Rysuj\0",'R',57,NULL},
	{u8"Blok\0",'B',1,NULL},
	{u8"Edycja\0",'E',2,NULL},
	{u8"Zmień właściwości\0\0",'Z',43,&mChangeProperties},
	{u8"szukaj / zmień Teksty\0",'T',4,&mZmienTeksty},
	{u8"Wymiarowanie\0",'W',5,NULL},
	{u8"kreSkowanie\0",'S',6,NULL},
	{u8"Geometria\0",'G',7,NULL},
	{u8"Makro\0",'M',8,NULL},
	{u8"Parametry\0",'P',9,NULL},
	{u8"Opcje\0",'O',10,NULL},
    {u8"Aplikacje\0",'A',753,&mApplications},
	{u8"Dysk\0",'D',81,NULL},
	{u8"Koniec pracy\0",'K',12,NULL},
	{u8"zamknij okno\0",'X',598,NULL},
};

#define _BITMAPS_ u8"Bitmaps"
#define _PLUGINS_ u8"plugins"
#define _PLUGIN_BITMAPS_ u8"plugins/bitmaps"

#define _NO_MOUSE_ "Nie jest zainstalowana mysz !!!!!\n"


char* loading_program[] = { u8"Ładowanie programu", u8"Ładowanie okna..." , u8"Proszę czekać. Trwa pobieranie pliku: ", u8"Łączenie z chmurą..."};

#endif
