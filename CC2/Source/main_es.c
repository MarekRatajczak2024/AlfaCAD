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
	{u8"Buscar texto",'B',3,NULL},
    {u8"Cambiar textos",'C',90, NULL},
    {u8"Numeración automática", 'N',91, NULL},
    // {u8"Cambio de numeración", 'C', 0,NULL},
};

TMENU mZmienTeksty = { 3, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 15, 0, 0,0,(POLE(*)[])&pmZmienTeksty, NULL, NULL };

POLE pmChangeProperties[] = {
	{u8"Cambiar",'C',43, NULL},
    {u8"Modificar splines", 'M',648, NULL},
    {u8"Vectores", 'V',770, NULL},
    {u8"Propiedades", L'P',138, NULL},
};

TMENU mChangeProperties = { 4, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 18, 0, 0,0,(POLE(*)[])&pmChangeProperties, NULL, NULL };

POLE pmWoodMoistureUS[] = {
        {u8"Verde",L'V',0,&mWoodSpeciesUS},
        {u8"12%",L'1',0,&mWoodSpeciesUS}};

TMENU mWoodMoistureUS = { 2,0,0,6,16,8,0,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

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
	{u8"Rectángulo", L'R', 905, &mConcreteClassUS},
	{u8"Doble T (I)", L'D', 906, &mConcreteClassUS},
	{u8"Perfil en T", L'T', 907, &mConcreteClassUS},
	{u8"Circular hueco", L'C', 908, &mConcreteClassUS},
	{u8"Sección cuadrada", L'S', 909, &mConcreteClassUS},
	{u8"Perfil de caja", L'P', 910, &mConcreteClassUS},
    {u8"losa",L'=',911,&mConcreteClassUS},
    {u8"diafragma",L'H',912,&mConcreteClassUS},
};

TMENU mConcreteUS = { 8,0,0,32,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmConcreteUS,NULL,NULL };

POLE pmConcreteEU[] = {
	{u8"Rectángulo", L'R', 905, &mConcreteClassEU},
	{u8"Doble T (I)", L'D', 906, &mConcreteClassEU},
	{u8"perfil en T", L'T', 907, &mConcreteClassEU},
	{u8"Circular hueco", L'C', 908, &mConcreteClassEU},
	{u8"Sección cuadrada", L'S', 909, &mConcreteClassEU},
	{u8"Perfil de caja", L'P', 910, &mConcreteClassEU},
    {u8"losa",L'=',911,&mConcreteClassEU},
    {u8"diafragma",L'H',912,&mConcreteClassEU},
};

TMENU mConcreteEU = { 8,0,0,32,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmConcreteEU,NULL,NULL };

TMENU mWoodMoistureCA = { 2,0,0,6,16,8,0,CMNU,CMBR,CMTX,0,168,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

POLE pmMaterialUS[] = {
        {u8"Acero",'A',775, NULL}, //&mSteelUS
        {u8"Hormigón / concreto armado", 'H',904, NULL}, //ConcreteUS
        {u8"Madera", 'M',785, &mWoodMoistureUS}, //&mTimberUS
};

TMENU mMaterialUS = { 3, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 6, 0, 0,0,(POLE(*)[])&pmMaterialUS, NULL, NULL };

POLE pmMaterialCA[] = {
        {u8"Acero",'A',775, NULL}, //&mSteelCA
        {u8"Madera", 'M',785, &mWoodMoistureCA}, //&mTimberCA
};

TMENU mMaterialCA = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 8, 0, 0,0,(POLE(*)[])&pmMaterialCA, NULL, NULL };

POLE pmMaterialEU[] = {
        {u8"Acero",'A',775, NULL},  //&mSteelEU
        {u8"Hormigón / concreto armado", 'H',904, NULL}, //ConcreteEU
};

TMENU mMaterialEU = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 0, 0, 0,0,(POLE(*)[])&pmMaterialEU, NULL, NULL };

POLE pmRegion[] = {
        {u8"EE.UU",'E',772, &mMaterialUS},
        {u8"UE", 'U',773, &mMaterialEU},
        {u8"Reino Unido", 'R',774, NULL}, //&mSteelUK
        {u8"Canadá", 'C',829, &mMaterialCA},
        {u8"Australia", 'A',830, NULL}, //&mSteelAU
        {u8"P.R.C", 'P',831, NULL}, //&mSteelCN
};

TMENU mRegion = { 6 , 0, 0, 12, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 7, 0, 0,0,(POLE(*)[])&pmRegion, NULL, NULL };

POLE pmStatic[] = {
        {u8"Pórtico y cercha",L'P',733, NULL},
        {u8"Losa",L'L',855, NULL},
        {u8"Diafragma",L'D',865, NULL},
        {u8"Emparrillado de vigas",L'E',897, NULL},
};

TMENU mStatic = { 4, 0, 0, 32, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 3, 0, 0,0,(POLE(*)[])&pmStatic, NULL, NULL };

POLE pmApplications[] = {
        {u8"Análisis estático y dinámico",L'A',854, &mStatic},
        {u8"Fuerzas resultantes en secciones transversales",L'F',824, NULL},
        {u8"animar la Dinámica",L'D',817, NULL},
        {u8"Selección de sección transversal del elemento",L'S',775, &mRegion},
};

TMENU mApplications = { 4, 0, 0, 46, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 22, 0, 0,0,(POLE(*)[])&pmApplications, NULL, NULL };

POLE pmenug[] = {
	{u8"Dibujar\0",'D',57,NULL},
	{u8"Bloque\0",'B',1,NULL},
	{u8"Editar\0",'E',2,NULL},
	{u8"Cambiar propiedades",'C',43,&mChangeProperties},
	{u8"búsqueda/cambio de Texto\0",'T',4,&mZmienTeksty},
	{u8"diMensionamiento\0",'M',5,NULL},
	{u8"Sombreado\0",'S',6,NULL},
	{u8"Geometría\0",'G',7,NULL},
	{u8"macRo\0",'R',8,NULL},
	{u8"Parámetros\0",'P',9,NULL},
	{u8"Opciones\0",'O',10,NULL},
    {u8"Implementaciones\0",'I',753,&mApplications},
	{u8"Archivo\0",'A',11,NULL},
	{u8"salir\0",'X',12,NULL},
	{u8"cerrar Ventana\0",'V',598,NULL},
};

#define _BITMAPS_ u8"Bitmaps"
#define _PLUGINS_ u8"plugins"
#define _PLUGIN_BITMAPS_ u8"plugins/bitmaps"

#define _NO_MOUSE_ "¡El mouse no está instalado!\n"

char* loading_program[] = { u8"Cargando el programa", u8"Cargando recursos..." , u8"Por favor espere. Descargando archivo: ", u8"Conectándose a la nube..."};


#endif
