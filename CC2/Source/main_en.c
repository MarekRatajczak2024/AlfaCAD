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
	{u8"Search text",'S',3,NULL},
	{u8"Change texts",'C',90, NULL},
	{u8"Automatic numbering", 'A',91, NULL},
	//	{u8"Przesunięcie numeracji", 'P', 0,NULL},
};

TMENU mZmienTeksty = { 3, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 15, 0, 0,0,(POLE(*)[])&pmZmienTeksty, NULL, NULL };

POLE pmChangeProperties[] = {
	{u8"Change",'C',43, NULL},
	{u8"Amend splines", 'A',648, NULL},
    {u8"Vectors", 'V',770, NULL},
};

TMENU mChangeProperties = { 3, 0, 0, 22, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 18, 0, 0,0,(POLE(*)[])&pmChangeProperties, NULL, NULL };

POLE pmWoodMoistureUS[] = {
{u8"Green",L'G',0,&mWoodSpeciesUS},
{u8"12%",L'1',0,&mWoodSpeciesUS}};

TMENU mWoodMoistureUS = { 2,0,0,6,16,8,0,CMNU,CMBR,CMTX,0,2,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

TMENU mWoodMoistureCA = { 2,0,0,6,16,8,0,CMNU,CMBR,CMTX,0,168,0,0,0,(POLE(*)[]) &pmWoodMoistureUS,NULL,NULL };

POLE pmMaterialUS[] = {
        {u8"Steel",'S',775, NULL}, //&mSteelUS
        {u8"Timber", 'T',785, &mWoodMoistureUS}, //NULL &mTimberUS
};

TMENU mMaterialUS = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 6, 0, 0,0,(POLE(*)[])&pmMaterialUS, NULL, NULL };

POLE pmMaterialCA[] = {
        {u8"Steel",'S',775, NULL}, //&mSteelCA
        {u8"Timber", 'T',785, &mWoodMoistureCA}, //NULL &mTimberCA
};

TMENU mMaterialCA = { 2, 0, 0, 12, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 8, 0, 0,0,(POLE(*)[])&pmMaterialCA, NULL, NULL };

POLE pmRegion[] = {
        {u8"US", 'U',772, &mMaterialUS},
        {u8"EU", 'E',773, NULL}, //&mSteelEU
        {u8"UK", 'B',774, NULL}, //&mSteelUK
        {u8"CA", 'C',829, &mMaterialCA},
        {u8"AU", 'A',830, NULL}, //&mSteelAU
        {u8"P.R.C", 'P',831, NULL}, //&mSteelCN
};

TMENU mRegion = { 6, 0, 0, 8, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 4, 0, 0,0,(POLE(*)[])&pmRegion, NULL, NULL };

POLE pmStatic[] = {
        {u8"Frame and truss",L'F',733, NULL},
        {u8"Plate",L'P',855, NULL},
};

TMENU mStatic = { 2, 0, 0, 32, 15, 9, ICONS, CMNU, CMBR, CMTX, 0, 3, 0, 0,0,(POLE(*)[])&pmStatic, NULL, NULL };

POLE pmApplications[] = {
        {u8"Static and dynamic analysis",L'S',854, &mStatic},
        {u8"Result forces in cross sections",L'R',824, NULL},
        {u8"Animate the dynamics",L'A',817, NULL},
        {u8"Element cross-section selection",L'E',775, &mRegion},
};

TMENU mApplications = { 4, 0, 0, 32, 15, 9, TADD | ICONS, CMNU, CMBR, CMTX, 0, 21, 0, 0,0,(POLE(*)[])&pmApplications, NULL, NULL };

POLE pmenug[] = {
	{u8"Draw\0",'D',57,NULL},
	{u8"Block\0",'B',1,NULL},
	{u8"Edit\0",'E',2,NULL},
	{u8"Change properties",'C',43,&mChangeProperties},
	{u8"Texts search/change\0",'T',4,&mZmienTeksty},
	{u8"diMensioning\0",'M',5,NULL},
	{u8"Hatching\0",'H',6,NULL},
	{u8"Geometry\0",'G',7,NULL},
	{u8"macRo\0",'R',8,NULL},
	{u8"Parameters\0",'P',9,NULL},
	{u8"Options\0",'O',10,NULL},
    {u8"Applications\0",'A',753,&mApplications},
	{u8"File\0",'F',11,NULL},
	{u8"eXit\0",'X',12,NULL},
	{u8"close Window\0",'W',598,NULL},
};

#define _BITMAPS_ u8"Bitmaps"
#define _PLUGINS_ u8"plugins"
#define _PLUGIN_BITMAPS_ u8"plugins/bitmaps"

#endif
