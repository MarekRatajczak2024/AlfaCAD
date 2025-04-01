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
};

TMENU mChangeProperties = { 3, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 18, 0, 0,0,(POLE(*)[])&pmChangeProperties, NULL, NULL };

POLE pmWoodMoistureUS[] = {
        {u8"Green",L' ',0,&mWoodSpeciesUS},
        {u8"12%",L' ',0,&mWoodSpeciesUS}};

TMENU mWoodMoistureUS = { 2,0,0,6,16,8,0,CMNU,CMBR,CMTX,0,3,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

TMENU mWoodMoistureCA = { 2,0,0,6,16,8,0,CMNU,CMBR,CMTX,0,169,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

POLE pmMaterialUS[] = {
        {u8"Acero",'A',775, NULL}, //&mSteelUS
        {u8"Madera", 'M',785, &mWoodMoistureUS}, //&mTimberUS
};

TMENU mMaterialUS = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 5, 0, 0,0,(POLE(*)[])&pmMaterialUS, NULL, NULL };

POLE pmMaterialCA[] = {
        {u8"Acero",'A',775, NULL}, //&mSteelCA
        {u8"Madera", 'M',785, &mWoodMoistureCA}, //&mTimberCA
};

TMENU mMaterialCA = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 7, 0, 0,0,(POLE(*)[])&pmMaterialCA, NULL, NULL };

POLE pmRegion[] = {
        {u8"EE.UU",'E',772, &mMaterialUS},
        {u8"UE", 'U',773, NULL}, //&mSteelEU
        {u8"Reino Unido", 'R',774, NULL}, //&mSteelUK
        {u8"Canadá", 'C',829, &mMaterialCA},
        {u8"Australia", 'A',830, NULL}, //&mSteelAU
};

TMENU mRegion = { 5, 0, 0, 12, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 2, 0, 0,0,(POLE(*)[])&pmRegion, NULL, NULL };

POLE pmApplications[] = {
        {u8"Análisis estático y dinámico",L'A',733, NULL},
        {u8"Fuerzas resultantes en secciones transversales",L'F',824, NULL},
        {u8"animar la Dinámica",L'D',817, NULL},
        {u8"Selección de sección transversal del elemento",L'S',775, &mRegion},
};

TMENU mApplications = { 4, 0, 0, 46, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 21, 0, 0,0,(POLE(*)[])&pmApplications, NULL, NULL };

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

#endif
