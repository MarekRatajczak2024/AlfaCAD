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

#define  __PLTDLG__
#include <forwin.h>
#include <math.h>
#include <string.h>
#ifndef LINUX
#include <io.h>
#endif
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdio.h>
#define ALLEGWIN
#include <allegext.h>
#include "bib_e.h"
#include "o_dialog.h"
#include "alf_res.h"
#include "o_plt.h"
#include "o_loadf.h"
#include "o_lfile.h"
#include "o_image.h"
#include "o_inicnf.h"
#include "b_bibe.h"
#include "rysuj_e.h"
#include "bgiext.h"

#include "dialog.h"

extern void utf8Upper(char* text);
extern void setrgb2cmyk(int *color_c, int *color_m, int *color_y, int i);
extern int GetColorAC1(int color);
extern int GetColorACPlt(int color);
#ifdef LINUX
extern int strnicmp(const char *a, const char *b, int n);
#endif

extern char digits[16];
extern char numbers[16];
char xdigits[16]= "xX.-+0123456789";

#define def_speed 20
#define def_width_ink 0.25

static double def_line_width_ink[5]={0.15, 0.25, 0.35, 0.50, 0.75};

typedef enum
{
	IDPlt_type = 0,        //1,2,99
    IDPlt_border,
    IDPlt_rozdz,				/*rozdzielczosc (kroki / mm)*/
    IDPlt_origin,
    IDPlt_ini,
    IDPlt_end,
    IDPlt_pen_sel,
    IDPlt_pen_speed,
    IDPlt_pen_width_ink,
    IDPlt_pen_up,
    IDPlt_pen_dn,
    IDPlt_pen_abs,
    IDPlt_pen_line,
    IDPlt_pen_move,
    IDPlt_ter_wsp,
    IDPlt_ter_kom,
    IDPlt_l_luk,
}ID_Plt_Config;


typedef enum
{
    //IDPlt_type = 0,				/*1, 2 */
    IDPlt_dx_dy=0,
    IDPlt_rotation,
    IDPlt_scale,
    IDPlt_print_window,
    IDPlt_print_sheet,
    IDPlt_ink_plotter,
    IDPlt_port,
    IDPlt_pen_width,
    IDPlt_fixed_name,
    IDPlt_page_size,
    IDPlt_color_print,
    IDPlt_color_255,
    IDPlt_tab_pen_speed,
    IDPlt_tab_line_width_ink,
    IDPlt_number_of_pen,
    IDPlt_color1,IDPlt_color2,IDPlt_color3, IDPlt_color4, IDPlt_color5,
    IDPlt_color6, IDPlt_color7, IDPlt_color8, IDPlt_color9, IDPlt_color10,
    IDPlt_color11, IDPlt_color12, IDPlt_color13, IDPlt_color14, IDPlt_color15,
    IDPlt_color16, IDPlt_color17, IDPlt_color18, IDPlt_color19, IDPlt_color20,
    IDPlt_color21, IDPlt_color22, IDPlt_color23, IDPlt_color24, IDPlt_color25,
    IDPlt_color26, IDPlt_color27, IDPlt_color28, IDPlt_color29, IDPlt_color30,
    IDPlt_color31, IDPlt_color32, IDPlt_color33, IDPlt_color34, IDPlt_color35,
    IDPlt_color36, IDPlt_color37, IDPlt_color38, IDPlt_color39, IDPlt_color40,
    IDPlt_color41, IDPlt_color42, IDPlt_color43, IDPlt_color44, IDPlt_color45,
    IDPlt_color46, IDPlt_color47, IDPlt_color48, IDPlt_color49, IDPlt_color50,
    IDPlt_color51, IDPlt_color52, IDPlt_color53, IDPlt_color54, IDPlt_color55,
    IDPlt_color56, IDPlt_color57, IDPlt_color58, IDPlt_color59, IDPlt_color60,
    IDPlt_color61, IDPlt_color62, IDPlt_color63, IDPlt_color64, IDPlt_color65,
    IDPlt_color66, IDPlt_color67, IDPlt_color68, IDPlt_color69, IDPlt_color70,
    IDPlt_color71, IDPlt_color72, IDPlt_color73, IDPlt_color74, IDPlt_color75,
    IDPlt_color76, IDPlt_color77, IDPlt_color78, IDPlt_color79, IDPlt_color80,
    IDPlt_color81, IDPlt_color82, IDPlt_color83, IDPlt_color84, IDPlt_color85,
    IDPlt_color86, IDPlt_color87, IDPlt_color88, IDPlt_color89, IDPlt_color90,
    IDPlt_color91, IDPlt_color92, IDPlt_color93, IDPlt_color94, IDPlt_color95,
    IDPlt_color96, IDPlt_color97, IDPlt_color98, IDPlt_color99, IDPlt_color100,
    IDPlt_color101,IDPlt_color102,IDPlt_color103, IDPlt_color104, IDPlt_color105,
    IDPlt_color106, IDPlt_color107, IDPlt_color108, IDPlt_color109, IDPlt_color110,
    IDPlt_color111, IDPlt_color112, IDPlt_color113, IDPlt_color114, IDPlt_color115,
    IDPlt_color116, IDPlt_color117, IDPlt_color118, IDPlt_color119, IDPlt_color120,
    IDPlt_color121, IDPlt_color122, IDPlt_color123, IDPlt_color124, IDPlt_color125,
    IDPlt_color126, IDPlt_color127, IDPlt_color128, IDPlt_color129, IDPlt_color130,
    IDPlt_color131, IDPlt_color132, IDPlt_color133, IDPlt_color134, IDPlt_color135,
    IDPlt_color136, IDPlt_color137, IDPlt_color138, IDPlt_color139, IDPlt_color140,
    IDPlt_color141, IDPlt_color142, IDPlt_color143, IDPlt_color144, IDPlt_color145,
    IDPlt_color146, IDPlt_color147, IDPlt_color148, IDPlt_color149, IDPlt_color150,
    IDPlt_color151, IDPlt_color152, IDPlt_color153, IDPlt_color154, IDPlt_color155,
    IDPlt_color156, IDPlt_color157, IDPlt_color158, IDPlt_color159, IDPlt_color160,
    IDPlt_color161, IDPlt_color162, IDPlt_color163, IDPlt_color164, IDPlt_color165,
    IDPlt_color166, IDPlt_color167, IDPlt_color168, IDPlt_color169, IDPlt_color170,
    IDPlt_color171, IDPlt_color172, IDPlt_color173, IDPlt_color174, IDPlt_color175,
    IDPlt_color176, IDPlt_color177, IDPlt_color178, IDPlt_color179, IDPlt_color180,
    IDPlt_color181, IDPlt_color182, IDPlt_color183, IDPlt_color184, IDPlt_color185,
    IDPlt_color186, IDPlt_color187, IDPlt_color188, IDPlt_color189, IDPlt_color190,
    IDPlt_color191, IDPlt_color192, IDPlt_color193, IDPlt_color194, IDPlt_color195,
    IDPlt_color196, IDPlt_color197, IDPlt_color198, IDPlt_color199, IDPlt_color200,
    IDPlt_color201,IDPlt_color202,IDPlt_color203, IDPlt_color204, IDPlt_color205,
    IDPlt_color206, IDPlt_color207, IDPlt_color208, IDPlt_color209, IDPlt_color210,
    IDPlt_color211, IDPlt_color212, IDPlt_color213, IDPlt_color214, IDPlt_color215,
    IDPlt_color216, IDPlt_color217, IDPlt_color218, IDPlt_color219, IDPlt_color220,
    IDPlt_color221, IDPlt_color222, IDPlt_color223, IDPlt_color224, IDPlt_color225,
    IDPlt_color226, IDPlt_color227, IDPlt_color228, IDPlt_color229, IDPlt_color230,
    IDPlt_color231, IDPlt_color232, IDPlt_color233, IDPlt_color234, IDPlt_color235,
    IDPlt_color236, IDPlt_color237, IDPlt_color238, IDPlt_color239, IDPlt_color240,
    IDPlt_color241, IDPlt_color242, IDPlt_color243, IDPlt_color244, IDPlt_color245,
    IDPlt_color246, IDPlt_color247, IDPlt_color248, IDPlt_color249, IDPlt_color250,
    IDPlt_color251, IDPlt_color252, IDPlt_color253, IDPlt_color254, IDPlt_color255,

    IDPlt_pcolor1,IDPlt_pcolor2,IDPlt_pcolor3, IDPlt_pcolor4, IDPlt_pcolor5,
    IDPlt_pcolor6, IDPlt_pcolor7, IDPlt_pcolor8, IDPlt_pcolor9, IDPlt_pcolor10,
    IDPlt_pcolor11, IDPlt_pcolor12, IDPlt_pcolor13, IDPlt_pcolor14, IDPlt_pcolor15,
    IDPlt_pcolor16, IDPlt_pcolor17, IDPlt_pcolor18, IDPlt_pcolor19, IDPlt_pcolor20,
    IDPlt_pcolor21, IDPlt_pcolor22, IDPlt_pcolor23, IDPlt_pcolor24, IDPlt_pcolor25,
    IDPlt_pcolor26, IDPlt_pcolor27, IDPlt_pcolor28, IDPlt_pcolor29, IDPlt_pcolor30,
    IDPlt_pcolor31, IDPlt_pcolor32, IDPlt_pcolor33, IDPlt_pcolor34, IDPlt_pcolor35,
    IDPlt_pcolor36, IDPlt_pcolor37, IDPlt_pcolor38, IDPlt_pcolor39, IDPlt_pcolor40,
    IDPlt_pcolor41, IDPlt_pcolor42, IDPlt_pcolor43, IDPlt_pcolor44, IDPlt_pcolor45,
    IDPlt_pcolor46, IDPlt_pcolor47, IDPlt_pcolor48, IDPlt_pcolor49, IDPlt_pcolor50,
    IDPlt_pcolor51, IDPlt_pcolor52, IDPlt_pcolor53, IDPlt_pcolor54, IDPlt_pcolor55,
    IDPlt_pcolor56, IDPlt_pcolor57, IDPlt_pcolor58, IDPlt_pcolor59, IDPlt_pcolor60,
    IDPlt_pcolor61, IDPlt_pcolor62, IDPlt_pcolor63, IDPlt_pcolor64,
    IDPlt_pcolor65,
    IDPlt_pcolor66, IDPlt_pcolor67, IDPlt_pcolor68, IDPlt_pcolor69, IDPlt_pcolor70,
    IDPlt_pcolor71, IDPlt_pcolor72, IDPlt_pcolor73, IDPlt_pcolor74, IDPlt_pcolor75,
    IDPlt_pcolor76, IDPlt_pcolor77, IDPlt_pcolor78, IDPlt_pcolor79, IDPlt_pcolor80,
    IDPlt_pcolor81, IDPlt_pcolor82, IDPlt_pcolor83, IDPlt_pcolor84, IDPlt_pcolor85,
    IDPlt_pcolor86, IDPlt_pcolor87, IDPlt_pcolor88, IDPlt_pcolor89, IDPlt_pcolor90,
    IDPlt_pcolor91, IDPlt_pcolor92, IDPlt_pcolor93, IDPlt_pcolor94, IDPlt_pcolor95,
    IDPlt_pcolor96, IDPlt_pcolor97, IDPlt_pcolor98, IDPlt_pcolor99, IDPlt_pcolor100,
    IDPlt_pcolor101,IDPlt_pcolor102,IDPlt_pcolor103, IDPlt_pcolor104, IDPlt_pcolor105,
    IDPlt_pcolor106, IDPlt_pcolor107, IDPlt_pcolor108, IDPlt_pcolor109, IDPlt_pcolor110,
    IDPlt_pcolor111, IDPlt_pcolor112, IDPlt_pcolor113, IDPlt_pcolor114, IDPlt_pcolor115,
    IDPlt_pcolor116, IDPlt_pcolor117, IDPlt_pcolor118, IDPlt_pcolor119, IDPlt_pcolor120,
    IDPlt_pcolor121, IDPlt_pcolor122, IDPlt_pcolor123, IDPlt_pcolor124, IDPlt_pcolor125,
    IDPlt_pcolor126, IDPlt_pcolor127, IDPlt_pcolor128, IDPlt_pcolor129, IDPlt_pcolor130,
    IDPlt_pcolor131, IDPlt_pcolor132, IDPlt_pcolor133, IDPlt_pcolor134, IDPlt_pcolor135,
    IDPlt_pcolor136, IDPlt_pcolor137, IDPlt_pcolor138, IDPlt_pcolor139, IDPlt_pcolor140,
    IDPlt_pcolor141, IDPlt_pcolor142, IDPlt_pcolor143, IDPlt_pcolor144, IDPlt_pcolor145,
    IDPlt_pcolor146, IDPlt_pcolor147, IDPlt_pcolor148, IDPlt_pcolor149, IDPlt_pcolor150,
    IDPlt_pcolor151, IDPlt_pcolor152, IDPlt_pcolor153, IDPlt_pcolor154, IDPlt_pcolor155,
    IDPlt_pcolor156, IDPlt_pcolor157, IDPlt_pcolor158, IDPlt_pcolor159, IDPlt_pcolor160,
    IDPlt_pcolor161, IDPlt_pcolor162, IDPlt_pcolor163, IDPlt_pcolor164, IDPlt_pcolor165,
    IDPlt_pcolor166, IDPlt_pcolor167, IDPlt_pcolor168, IDPlt_pcolor169, IDPlt_pcolor170,
    IDPlt_pcolor171, IDPlt_pcolor172, IDPlt_pcolor173, IDPlt_pcolor174, IDPlt_pcolor175,
    IDPlt_pcolor176, IDPlt_pcolor177, IDPlt_pcolor178, IDPlt_pcolor179, IDPlt_pcolor180,
    IDPlt_pcolor181, IDPlt_pcolor182, IDPlt_pcolor183, IDPlt_pcolor184, IDPlt_pcolor185,
    IDPlt_pcolor186, IDPlt_pcolor187, IDPlt_pcolor188, IDPlt_pcolor189, IDPlt_pcolor190,
    IDPlt_pcolor191, IDPlt_pcolor192, IDPlt_pcolor193, IDPlt_pcolor194, IDPlt_pcolor195,
    IDPlt_pcolor196, IDPlt_pcolor197, IDPlt_pcolor198, IDPlt_pcolor199, IDPlt_pcolor200,
    IDPlt_pcolor201,IDPlt_pcolor202,IDPlt_pcolor203, IDPlt_pcolor204, IDPlt_pcolor205,
    IDPlt_pcolor206, IDPlt_pcolor207, IDPlt_pcolor208, IDPlt_pcolor209, IDPlt_pcolor210,
    IDPlt_pcolor211, IDPlt_pcolor212, IDPlt_pcolor213, IDPlt_pcolor214, IDPlt_pcolor215,
    IDPlt_pcolor216, IDPlt_pcolor217, IDPlt_pcolor218, IDPlt_pcolor219, IDPlt_pcolor220,
    IDPlt_pcolor221, IDPlt_pcolor222, IDPlt_pcolor223, IDPlt_pcolor224, IDPlt_pcolor225,
    IDPlt_pcolor226, IDPlt_pcolor227, IDPlt_pcolor228, IDPlt_pcolor229, IDPlt_pcolor230,
    IDPlt_pcolor231, IDPlt_pcolor232, IDPlt_pcolor233, IDPlt_pcolor234, IDPlt_pcolor235,
    IDPlt_pcolor236, IDPlt_pcolor237, IDPlt_pcolor238, IDPlt_pcolor239, IDPlt_pcolor240,
    IDPlt_pcolor241, IDPlt_pcolor242, IDPlt_pcolor243, IDPlt_pcolor244, IDPlt_pcolor245,
    IDPlt_pcolor246, IDPlt_pcolor247, IDPlt_pcolor248, IDPlt_pcolor249, IDPlt_pcolor250,
    IDPlt_pcolor251, IDPlt_pcolor252, IDPlt_pcolor253, IDPlt_pcolor254, IDPlt_pcolor255,
    } ID_Plt_Ini_Data;


/*

			    LOCAL DECLARATIONS		     

*/



static T_Plt_Config  plt_config =  /*parametry konfiguracyjne plotera*/
{
	1, 		/*typ */
	-16420,    	/*plt_config.xmin*/
	- 9800,		/*plt_config.ymin*/
	18800,		/*plt_config.xmax*/
	10200,		/*plt_config.ymax*/
	40,		/*plt_config.rozdz*/
	0,              /*plt_config.origin*/
	"IN",		/*plt_config.ini*/
	"SP0;VS0;NR",	/*plt_config.end*/
	"SP",		/*plt_config.pen_sel*/
	"VS",		/*plt_config.pen_speed*/
	"PW",           /*plt_config.pen_width_ink*/
	"PU",		/*plt_config.pen_up*/
	"PD",		/*plt_config.pen_dn*/
	"PA",		/*plt_config.pen_abs*/
	"D",  		/*plt_config.pen_line*/
	"M",    	/*plt_config.pen_move*/
	",",		/*plt_config.ter_wsp*/
	";",		/*plt_config.ter_kom*/		/*CHR$(13) MERA POLTIC*/
	1,		/*plt_config.l_luk*/
};

static T_Plt_Ini_Date  plt_ini_data =
{
	//1, 		/*typ */
	0, 0,           /*przesuniecie*/
	0, 0, 0, 0,    /*okno do plotowania*/
   0, 0, 0, 0,    /*wewnetrzne okno do plotowania*/
	0,
	1.0,            /*skala*/
	0,              /*wydruk okna*/
   0,              /*wydruk arkusza*/
   NULL,           /*adres arkusza*/
	0,              /*ploter atramentowy*/
	"",
	0.25,
   0,              /*fixed_name*/
   0,              /*page size*/
   0,              /*color print*/
   0,              /*color 255*/
	{20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,},
	{0.15,0.25,0.35,0.5,0.75,},
   255,             /*liczba pisakow kolorowych*/
	{	 {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
         {1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
	{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
	}
};

static T_Plt_Ini_Date  plt_ini_data_save;
static T_Plt_Ini_Date  plt_ini_data_bak;

static BOOL first = TRUE;         	/*pierwsze wejscie do programu*/

static BOOL add_value (FILE *stru);
static BOOL get_config_param_from_ini_file (T_Fstring, T_Fstring);
static BOOL get_plt_init_data_from_ini_file (T_Fstring, T_Fstring);

/*-----------------------------------------------------*/
/*-----------------------------------------------------*/
/*------     DIALOG,  INI - FILE   ---------------------*/
/*-----------------------------------------------------*/
/*-----------------------------------------------------*/

#define XpOK 427
#define YpCANCEL ( YpMove + 1)  
#define XpCANCEL XpOK
#define YpOK (YpCANCEL + 60)  

#define DXGroup  110
#define DXGroup1 180
#define DXGroup2 85
#define DYBut (DYLab -3)   /*-4*/
#define DXBut 140
#define DXBut1 20
#define XpColor 123
#define YpColor 5
#define XpSpeed 5
#define YpSpeed 5
#define XpMove 310
#define YpMove 102
#define XpFileName 5
#define YpFileName 175
#define XpRotation XpColor
#define YpRotation YpMove

#define XpScale ( XpColor + 65 )
#define YpScale YpMove

#define XpPenWidth XpMove
#define YpPenWidth YpFileName
#define DXColor /*110*/ 67
#define DYColor (2 * DYLab-DYColorEdit)
#define DXSpeed 60 /*80*/ /*70*/
#define DYSpeed (2 * DYLab-DYSpeedEdit)
#define DXColorEdit  22 //24
#define DYColorEdit 12 // 15
#define DXSpeedEdit 36 //46
#define DYSpeedEdit DYColorEdit
#define DXMoveEdit  52
#define DYMoveEdit  DYColorEdit
#define DYEdit 12
#define No_Color 16
#define No_Pen 8


#define ID_OK		  	2
#define ID_CANCEL	  	1
#define ID_ROTATION 	  	3
#define ID_PRINT_WINDOW         4
#define ID_PRINT_SHEET          5
#define ID_INK_PLOTTER          6
#define ID_FIXED_NAME           7
#define ID_PAGE_SIZE            8
#define ID_COLOR_PRINT          9
#define ID_COLOR_255            10
#define LAB_SPEED		         8
#define LAB_COLOR		(LAB_SPEED + No_Pen)
#define EDIT_SPEED              5
#define EDIT_WIDTH_INK  (EDIT_SPEED + No_Pen)
#define EDIT_COLOR		(EDIT_WIDTH_INK + 5 )
#define LAB_SIZE                (LAB_COLOR +  No_Color +1)
#define EDIT_SIZE		(EDIT_COLOR + 5 * No_Color)

static GROUP_BOX  gr_box [10]=
{
  { XpColor-2, YpColor, No_Color * DXColorEdit + 2 * DXIL + 27,
	1.2 * DYLab + 5 /*3*/ * DYColorEdit + DYIL +5,
	COLOR_NULL ,COLOR_NULL, plt_dlg_labels[0], 0, NULL,
  },
  { XpSpeed, YpSpeed, DXSpeed + DXSpeedEdit + 3 * DXIL-10,
      2 * DYLab + No_Pen * DYSpeedEdit+ 2 * DYIL + 21,
	COLOR_NULL ,COLOR_NULL, plt_dlg_labels[1], 0, NULL,
  },
  { XpMove, YpMove, DXGroup, 3 * DYLab + DYIL,
	COLOR_NULL ,COLOR_NULL, plt_dlg_labels[2], 0, NULL,
  },
  { XpRotation-2, YpRotation, DXGroup1+4, 3 * DYLab + DYIL,
	COLOR_NULL ,COLOR_NULL, plt_dlg_labels[4], 0, NULL,
  },
  { XpFileName, YpFileName, 415, DYLab + DYIL,
	COLOR_NULL ,COLOR_NULL, plt_dlg_labels[5], 0, NULL,
  },
};



static BUTTON buttons [35] =
{
  { XpRotation + DXIL, YpRotation+ 0.45 * DYLab, DXBut1, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0, B_CHECKBOX, 0, 1,0, ID_ROTATION, 0, 0,
  },
  { XpOK, YpOK, DXBut0, DYBut0-2, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0,B_PUSHBUTTON, 81, 1,0, ID_OK, 0, 0,
  },
  { XpCANCEL, YpCANCEL, DXBut0, DYBut0-2, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0,B_PUSHBUTTON, 83, 1,0, ID_CANCEL, 0, 0,
  },
  {  XpRotation + DXIL + 60, YpRotation + 0.45 * DYLab, DXBut1, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0, B_CHECKBOX, 0, 1,0, ID_PRINT_WINDOW, 0, 0,
  },
  {  XpRotation + DXIL + 120, YpRotation + 0.45 * DYLab, DXBut1, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0, B_CHECKBOX, 0, 1,0, ID_PRINT_SHEET, 0, 0,
  },
  { XpRotation + DXIL + 60, YpRotation + 1.35 * DYLab, DXBut1, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0, B_CHECKBOX, 0, 1,0, ID_INK_PLOTTER, 0, 0,
  },
  { XpFileName+ DXIL+330, YpFileName + 0.30* DYLab, DXBut1, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, plt_dlg_buttons[2], 0, B_CHECKBOX, 0, 1,0, ID_FIXED_NAME, 0, 0,
  },
  { XpRotation + DXIL, YpRotation + 2.25 * DYLab, 100, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, plt_dlg_buttons[3], 0, B_CHECKBOX, 0, 1,0, ID_PAGE_SIZE, 0, 0,
  },
  {  XpRotation + DXIL, YpRotation + 1.35 * DYLab, DXBut1, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0, B_CHECKBOX, 0, 1,0, ID_COLOR_PRINT, 0, 0,
  },
  {  XpRotation + DXIL + 120, YpRotation + 1.35 * DYLab, DXBut1, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0, B_CHECKBOX, 0, 1,0, ID_COLOR_255, 0, 0,
  },
};

const IMAGE images[] =
{
 {  XpColor+2, YpColor + 2* DYColorEdit, 32, 32, 34,image_pl_tips0}, //line thickness
 {  XpColor+2, YpColor + 3* DYColorEdit, 32, 32, 35,image_pl_tips1},
 {  XpColor+2, YpColor + 4* DYColorEdit, 32, 32, 36,image_pl_tips2},
 {  XpColor+2, YpColor + 5* DYColorEdit, 32, 32, 37,image_pl_tips3},
 {  XpColor+2, YpColor + 6* DYColorEdit-1, 32, 32, 38,image_pl_tips4},
 {  XpRotation + DXIL + 75, YpRotation + 1.35 * DYLab, 48, 48, 39,image_pl_tips5}, //ink
 {  XpRotation + DXIL +15, YpRotation + 0.4 * DYLab, 48, 48, 18,image_pl_tips6}, //rotation
 {  XpRotation + DXIL +75, YpRotation + 0.4 * DYLab, 48, 48, 24,image_pl_tips7},//okno
 {  XpRotation + DXIL +135, YpRotation + 0.4 * DYLab, 48, 48, 25,image_pl_tips8}, //sheet
 {  XpRotation + DXIL +15, YpRotation + 1.35 * DYLab, 48, 48, 28,image_pl_tips9}, //color
 {  XpRotation + DXIL +135, YpRotation + 1.35 * DYLab, 48, 48, 40,image_pl_tips10}, //255 color
 {  XpMove + DXIL, YpRotation + 0.4 * DYLab, 48, 48, 26,image_pl_tips11}, //scale
 {  XpMove + DXIL, YpRotation + 1.35 * DYLab, 48, 48, 107,image_pl_tips12}, //letf offset
 {  XpMove + DXIL, YpRotation + 2.25 * DYLab, 48, 48, 108,image_pl_tips13}, //bottom offset
};


static INPUTLINE  edit_table [EDIT_SIZE];
static char back_table[80][3];
static char il_move_dx[6] = "0" ;
static char il_move_dy[6] = "0" ;
static char il_pen_width[6] = "0" ;
static char il_scale[6] = "0" ;
static char *plt_file ;
static char port0[MAXPATH] ;
static BOOL back_saved = FALSE;

static INPUTLINE edit_move_dx =
{
  XpMove +50, YpMove + 1.5*DYLab, DXMoveEdit, DYEdit ,
	COLOR_NULL,COLOR_NULL, COLOR_NULL,COLOR_NULL,COLOR_NULL,
	5,0,1, il_move_dx, -1,&digits,
};
static INPUTLINE edit_move_dy =
{
  XpMove + 50, YpMove + 2.4*DYLab, DXMoveEdit, DYEdit ,
	COLOR_NULL,COLOR_NULL, COLOR_NULL,COLOR_NULL,COLOR_NULL,
	5,0,1, il_move_dy, -1,&digits,
};
static INPUTLINE edit_file =
{ XpFileName+ DXIL, YpFileName+ 0.45 * DYLab, 320, DYEdit, COLOR_NULL,COLOR_NULL,
      COLOR_NULL,COLOR_NULL,COLOR_NULL,MAXPATH, 22, 1, NULL,-1,"", /* prn_file,*/
};
static INPUTLINE edit_pen_width =
{
  XpSpeed + DXIL + DXSpeed, YpSpeed + 2 * DYLab + No_Pen * DYSpeedEdit + 2 * DYIL - 17, DXSpeedEdit, DYEdit ,
	COLOR_NULL,COLOR_NULL, COLOR_NULL,COLOR_NULL,COLOR_NULL,
	4,0,1, il_pen_width, -1,&digits,
};

static INPUTLINE edit_scale =
{
  XpMove + 50, YpMove + 0.6 * DYLab, DXMoveEdit, DYEdit ,
	COLOR_NULL,COLOR_NULL, COLOR_NULL,COLOR_NULL,COLOR_NULL,
	4,0,1, il_scale, -1,&digits,
};

static LABEL  lab_table [LAB_SIZE];

static LABEL lab_speed1=
  { XpSpeed+ DXIL -5, YpSpeed + 2 * DYLab + No_Pen * DYSpeedEdit+ 2 * DYIL - 25,
	0, 0, COLOR_NULL,COLOR_NULL,plt_dlg_labs1,0,0};

static LABEL lab_speed2=
  { XpSpeed, YpSpeed + 1 * DYLab -10, 0, 0, COLOR_NULL,COLOR_NULL,_NR_,0,0};
static LABEL lab_speed3=
  { XpSpeed+ DXIL + 10 , YpSpeed + 1 * DYLab -10, 0, 0, COLOR_NULL,COLOR_NULL,_CM_S_,0,0};
static LABEL lab_speed4=
  { XpSpeed+ DXIL + DXSpeed, YpSpeed + 1 * DYLab -10, 0, 0, COLOR_NULL,COLOR_NULL,_MM_,0,0};
  
static LABEL lab_move_dx=
  { XpMove+ 10, YpMove + 1.5 * DYLab+ DYIL, 0, 0, COLOR_NULL,COLOR_NULL,"",0,0};  //"dx[mm]"
  
static LABEL lab_move_dy=
  { XpMove+ 10, YpMove + 2.4 * DYLab+ DYIL, 0, 0, COLOR_NULL,COLOR_NULL,"",0,0}; //"dy[mm]"

static LABEL lab_pen_width=
  { XpSpeed + DXIL, YpSpeed + 2 * DYLab + No_Pen * DYSpeedEdit + 2 * DYIL - 14, 0, 0, COLOR_NULL,COLOR_NULL,_FILLINGS_,0,0};


static LABEL lab_color=
  { XpColor, YpColor + DYColor + 5 * DYColorEdit+ DYIL -10,
	0, 0, COLOR_NULL,COLOR_NULL,plt_dlg_labs2,0,0};


static TDIALOG ploter_dlg =
{
	7, 10, 523,209,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,0,0,
	plt_dlg_labs0,
	0, NULL,
	LAB_SIZE, &lab_table,
	5, &gr_box,
	EDIT_SIZE, &edit_table,
	14, &images,
	35, &buttons,
	0,NULL,
	0,NULL,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

static char speed_text [No_Pen][5] =

{
	"x", "x", "x", "x",
		"x", "x", "x", "x",
};

static char width_ink_text [No_Pen][5] =

{
	"x", "x", "x", "x",
		"x", "x", "x", "x",
};

static char color_text [No_Color * 5/*3*/][5] =
{

  "1", "1", "1", "1", "1", "1", "1", "1",
  "1", "1", "1", "1", "1", "1", "1", "1",

  "1", "1", "1", "1", "1", "1", "1", "1",
  "1", "1", "1", "1", "1", "1", "1", "1",

  "1", "1", "1", "1", "1", "1", "1", "1",
  "1", "1", "1", "1", "1", "1", "1", "1",

  "1", "1", "1", "1", "1", "1", "1", "1",
  "1", "1", "1", "1", "1", "1", "1", "1",

  "1", "1", "1", "1", "1", "1", "1", "1",
  "1", "1", "1", "1", "1", "1", "1", "1",
};


static void set_struct_dialog_control (void)
/*--------------------------------------*/
{
  int i, j, k;
  static char * speeds [] =
  {u8"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"};
  static char * colors [] =
   {u8" 1 ", " 2 ", " 3 ", " 4 ", " 5 ", " 6 ", " 7 "," 8 ",
     " 9 ", " 10", " 11", " 12", " 13", " 14", " 15", " 16" } ;

/*labels*/
  memcpy (lab_table + 0, &lab_speed1, sizeof (LABEL));
  memcpy (lab_table + 1, &lab_speed2, sizeof (LABEL));
  memcpy (lab_table + 2, &lab_speed3, sizeof (LABEL));
  memcpy (lab_table + 3, &lab_speed4, sizeof (LABEL));
  memcpy (lab_table + 4, &lab_move_dx, sizeof (LABEL));
  memcpy (lab_table + 5, &lab_move_dy, sizeof (LABEL));
  memcpy (lab_table + 6, &lab_color, sizeof (LABEL));
  memcpy (lab_table + 7, &lab_pen_width, sizeof (LABEL));

  for ( i = LAB_SPEED; i < LAB_SPEED + No_Pen; i++)
  {
    lab_table[i].x =  XpSpeed + DXIL -5;
    lab_table[i].y =  YpSpeed + DYSpeed + (i-LAB_SPEED) * DYSpeedEdit - 8;
    lab_table[i].dx = lab_table[i].dy = 0;
    lab_table[i].ink = COLOR_NULL;
    lab_table[i].paper = COLOR_NULL;
    lab_table[i].txt = speeds [i-LAB_SPEED];
    lab_table[i].shadow=0;
    lab_table[i].s_ink=0;
  }
  for ( i = LAB_COLOR; i < LAB_COLOR+ No_Color; i++)
  {
    lab_table[i].x =  XpColor+  DXColor - DXIL + (i-LAB_COLOR) * DXColorEdit - 40;
    lab_table[i].y =  YpColor + 1 * DYLab - 10;
    lab_table[i].dx = lab_table[i].dy = 0;
    lab_table[i].ink = COLOR_NULL;
    lab_table[i].paper = COLOR_NULL;
    lab_table[i].txt = colors [i-LAB_COLOR];
    lab_table[i].shadow=0;
    lab_table[i].s_ink=0;
  }
/*edit*/
  memcpy (edit_table + 0, &edit_move_dx, sizeof (INPUTLINE));
  memcpy (edit_table + 1, &edit_move_dy, sizeof (INPUTLINE));
  edit_file.txt = plt_file;
  memcpy (edit_table + 2, &edit_file, sizeof (INPUTLINE));
  memcpy (edit_table + 3, &edit_pen_width, sizeof (INPUTLINE));
  memcpy (edit_table + 4, &edit_scale, sizeof (INPUTLINE));
  for ( i = EDIT_SPEED; i < EDIT_SPEED+ No_Pen; i++)
  {
    edit_table[i].x =  XpSpeed+ DXIL + 15 /*DXSpeed*/;
    edit_table[i].y =  YpSpeed + DYSpeed + (i-EDIT_SPEED) * DYSpeedEdit -10;
    edit_table[i].dx = DXSpeedEdit;
    edit_table[i].dy = DYEdit;
    edit_table[i].border = edit_table[i].ink = edit_table[i].paper =
    edit_table[i].ink_ini = edit_table[i].paper_ini = COLOR_NULL;
    edit_table[i].MaxLen = 4;
    edit_table[i].width = 0;
    edit_table[i].enable = 1;
	edit_table[i].id = -1;
    edit_table[i].txt = (char *)speed_text [i-EDIT_SPEED];
	edit_table[i].legal = &xdigits;
  }
  
  for ( i = EDIT_WIDTH_INK; i < EDIT_WIDTH_INK + No_Pen; i++)
  {
    edit_table[i].x =  XpSpeed+ DXIL + DXSpeed;
    edit_table[i].y =  YpSpeed + DYSpeed + (i-EDIT_WIDTH_INK) * DYSpeedEdit -10;
    edit_table[i].dx = DXSpeedEdit;
    edit_table[i].dy = DYEdit;
    edit_table[i].border = edit_table[i].ink = edit_table[i].paper =
    edit_table[i].ink_ini = edit_table[i].paper_ini = COLOR_NULL;
    edit_table[i].MaxLen = 4;
    edit_table[i].width = 0;
    edit_table[i].enable = 1;
	edit_table[i].id = 999; // -1;
    edit_table[i].txt = (char *)width_ink_text [i-EDIT_WIDTH_INK];
	edit_table[i].legal = &digits;
  }
  
  i = EDIT_COLOR;
  for ( k = 0; k < No_Color; k++)
  {
    for ( j = 0; j < 5/*3*/; j++, i++)
    {                                     /* */
      edit_table[i].x =  XpColor + DXColor - 43 + k * DXColorEdit;
      edit_table[i].y =  YpColor + DYColor + j * DYColorEdit -10;
      edit_table[i].dx = DXColorEdit;
      edit_table[i].dy = DYEdit;
      edit_table[i].border = edit_table[i].ink =
      edit_table[i].ink_ini = edit_table[i].paper_ini = COLOR_NULL;
	  if (k == 15) edit_table[i].ink = 15;
	  edit_table[i].paper = GetColorACPlt(k+1);
      edit_table[i].MaxLen = 2 /* 1 */;
	  edit_table[i].width = 2;
      edit_table[i].enable = 1;
      edit_table[i].id = -1;
      edit_table[i].txt = (char *)color_text [j * No_Color + k];
	  edit_table[i].legal = &numbers;
    }
  }

  i = ID_COLOR_255;
  for (j = 0; j < No_Color; j++, i++)
  {

	  //BUTTON 
	  buttons[i].x = XpColor + DXColor - 35 + j * DXColorEdit;
	  buttons[i].y = YpColor + DYColor + 4 * DYColorEdit + 4;
	  buttons[i].dx = 8;
	  buttons[i].dy = 6;
	  buttons[i].border = COLOR_NULL;
	  buttons[i].ink = COLOR_NULL;
	  buttons[i].paper = COLOR_NULL;
	  buttons[i].txt = "";
	  buttons[i].check = 0;
	  buttons[i].type = B_FLATPUSHBUTTON;
	  buttons[i].name2 = 0;
	  buttons[i].enable = 1;
	  buttons[i].flags = 0;
	  buttons[i].id = ID_COLOR_255 + j + 1;
  }

  i = ID_COLOR_255 + No_Color;
  for (j = 0; j < 5; j++, i++)
  {

	  //BUTTON 
	  buttons[i].x = XpColor + DXColor - 39 + 16 * DXColorEdit;
	  buttons[i].y = YpColor + DYColor + j * DYColorEdit - 7;
	  buttons[i].dx = 8;
	  buttons[i].dy = 6;
	  buttons[i].border = COLOR_NULL;
	  buttons[i].ink = COLOR_NULL;
	  buttons[i].paper = COLOR_NULL;
	  buttons[i].txt = "";
	  buttons[i].check = 0;
	  buttons[i].type = B_FLATPUSHBUTTON;
	  buttons[i].name2 = 0;
	  buttons[i].enable = 1;
	  buttons[i].flags = 0;
	  buttons[i].id = ID_COLOR_255 + j + No_Color + 1;
  }

  i = ID_COLOR_255 + No_Color + 5;
  
  buttons[i].x = XpColor + 4;
  buttons[i].y = YpColor + 10;
  buttons[i].dx = 16;
  buttons[i].dy = 12;
  buttons[i].border = COLOR_NULL;
  buttons[i].ink = COLOR_NULL;
  buttons[i].paper = COLOR_NULL;
  buttons[i].txt = "";
  buttons[i].check = 0;
  buttons[i].type = B_FLATPUSHBUTTON;
  buttons[i].name2 = 86;
  buttons[i].enable = 1;
  buttons[i].flags = 0;
  buttons[i].id = i+1;

  i = ID_COLOR_255 + No_Color + 6;

  buttons[i].x = XpColor + DXColor - 19 + 15 * DXColorEdit;
  buttons[i].y = YpColor + DYColor + 4 * DYColorEdit + 2;
  buttons[i].dx = 12;
  buttons[i].dy = 9;
  buttons[i].border = COLOR_NULL;
  buttons[i].ink = COLOR_NULL;
  buttons[i].paper = COLOR_NULL;
  buttons[i].txt = "#";
  buttons[i].check = 0;
  buttons[i].type = B_FLATPUSHBUTTON;
  buttons[i].name2 = 0;
  buttons[i].enable = 1;
  buttons[i].flags = 0;
  buttons[i].id = i+1;

  i = ID_COLOR_255 + No_Color + 7;

  buttons[i].x = XpOK;
  buttons[i].y = YpOK - 19;
  buttons[i].dx = 43;
  buttons[i].dy = 17;
  buttons[i].border = COLOR_NULL;
  buttons[i].ink = COLOR_NULL;
  buttons[i].paper = COLOR_NULL;
  buttons[i].txt = "";
  buttons[i].check = 0;
  buttons[i].type = B_PUSHBUTTON;
  buttons[i].name2 = 84;
  buttons[i].enable = 1;
  buttons[i].flags = 0;
  buttons[i].id = i + 1;

  i = ID_COLOR_255 + No_Color + 8;

  buttons[i].x = XpOK+47;
  buttons[i].y = YpOK - 19;
  buttons[i].dx = 43;
  buttons[i].dy = 17;
  buttons[i].border = COLOR_NULL;
  buttons[i].ink = COLOR_NULL;
  buttons[i].paper = COLOR_NULL;
  buttons[i].txt = "";
  buttons[i].check = 0;
  buttons[i].type = B_PUSHBUTTON;
  buttons[i].name2 = 85;
  buttons[i].enable = 1;
  buttons[i].flags = 0;
  buttons[i].id = i + 1;
  
}



static void init_ploter_dlg_control (void)
/*--------------------------------------*/
{
  int i, k;

/*buttons*/
  
  Set_Check_Button (&ploter_dlg, ID_ROTATION, plt_ini_data.rotation);
  Set_Check_Button (&ploter_dlg, ID_PRINT_WINDOW, plt_ini_data.print_window) ;
  Set_Check_Button (&ploter_dlg, ID_PRINT_SHEET, plt_ini_data.print_sheet) ;
  Set_Check_Button (&ploter_dlg, ID_INK_PLOTTER, plt_ini_data.ink_plotter) ;
  Set_Check_Button (&ploter_dlg, ID_FIXED_NAME, plt_ini_data.fixed_name) ;
  Set_Check_Button (&ploter_dlg, ID_PAGE_SIZE, plt_ini_data.page_size) ;
  Set_Check_Button (&ploter_dlg, ID_COLOR_PRINT, plt_ini_data.color_print) ;
  Set_Check_Button (&ploter_dlg, ID_COLOR_255, plt_ini_data.color_255) ;


/*edit*/
  sprintf (edit_move_dx.txt, "%ld",  plt_ini_data.dx);
  sprintf (edit_move_dy.txt, "%ld",  plt_ini_data.dy);
  sprintf (edit_pen_width.txt, "%.2lf",  plt_ini_data.pen_width);
  sprintf (edit_scale.txt, "%.2lf",  plt_ini_data.scale);
  edit_file.txt = plt_file;
  for ( i = EDIT_SPEED; i < EDIT_SPEED+ No_Pen; i++)
  {
    if (plt_ini_data.tab_pen_speed [i - EDIT_SPEED] == def_speed)
	 {
      strcpy (edit_table[i].txt, "X");
    }
    else
    sprintf (edit_table[i].txt, "%d",  plt_ini_data.tab_pen_speed [i - EDIT_SPEED]);
  }


  for ( i = EDIT_WIDTH_INK; i < EDIT_WIDTH_INK+ 5; i++)
   {
    sprintf (edit_table[i].txt, "%.2f",  plt_ini_data.tab_line_width_ink [i - EDIT_WIDTH_INK]);
   }
  
  i = EDIT_COLOR;
  for ( k = 0; k < No_Color; k++)
  { sprintf (edit_table[i++].txt,"%d", plt_ini_data.tab_pen_number [k].line_bc);
    sprintf (edit_table[i++].txt,"%d", plt_ini_data.tab_pen_number [k].line_c);
    sprintf (edit_table[i++].txt,"%d", plt_ini_data.tab_pen_number [k].line_g);
    sprintf (edit_table[i++].txt,"%d", plt_ini_data.tab_pen_number [k].line_bg);
    sprintf (edit_table[i++].txt,"%d", plt_ini_data.tab_pen_number [k].line_ng);
  }
}


static BOOL get_plt_dlg_control (T_Plt_Ini_Date *ptr_plt_ini_data)
/*----------------------------------------------------------------*/
{
  int i, k;

/*buttons*/
  ptr_plt_ini_data->rotation = Get_Check_Button ( &ploter_dlg, ID_ROTATION);
  ptr_plt_ini_data->print_window = Get_Check_Button ( &ploter_dlg, ID_PRINT_WINDOW) ;
  ptr_plt_ini_data->print_sheet = Get_Check_Button ( &ploter_dlg, ID_PRINT_SHEET) ;
  ptr_plt_ini_data->ink_plotter = Get_Check_Button ( &ploter_dlg, ID_INK_PLOTTER) ;
  ptr_plt_ini_data->fixed_name = Get_Check_Button ( &ploter_dlg, ID_FIXED_NAME) ;
  ptr_plt_ini_data->page_size = Get_Check_Button ( &ploter_dlg, ID_PAGE_SIZE) ;
  ptr_plt_ini_data->color_print = Get_Check_Button ( &ploter_dlg, ID_COLOR_PRINT) ;
  ptr_plt_ini_data->color_255 = Get_Check_Button ( &ploter_dlg, ID_COLOR_255) ;

/*edit*/
  if ( sscanf ( edit_move_dx.txt, "%ld", &ptr_plt_ini_data->dx) < 1 ||
	sscanf ( edit_move_dy.txt, "%ld", &ptr_plt_ini_data->dy) < 1 ||
	sscanf ( edit_pen_width.txt, "%lf", &ptr_plt_ini_data->pen_width) < 1 ||
		 ptr_plt_ini_data->pen_width <= 0  ||
	sscanf ( edit_scale.txt, "%lf", &ptr_plt_ini_data->scale) < 1 ||
	   ptr_plt_ini_data->scale <= 0)
  {
     ErrList (16) ;
     return FALSE;
  }
  for ( i = EDIT_SPEED; i < EDIT_SPEED+ No_Pen; i++)
  { if (edit_table[i].txt [0] == 'x' || edit_table[i].txt [0] == 'X')
    {
      ptr_plt_ini_data->tab_pen_speed [i - EDIT_SPEED] = def_speed;
    }
    else
    if (sscanf ( edit_table[i].txt, "%d", &ptr_plt_ini_data->tab_pen_speed [i - EDIT_SPEED]) < 1
      || ptr_plt_ini_data->tab_pen_speed [i - EDIT_SPEED] < 0)
    {
      ErrList (9) ;
      return FALSE;
    }
  }
  for ( i = EDIT_WIDTH_INK; i < EDIT_WIDTH_INK + 5; i++)
  { if (sscanf ( edit_table[i].txt, "%lf", &ptr_plt_ini_data->tab_line_width_ink [i - EDIT_WIDTH_INK]) < 1
      || ptr_plt_ini_data->tab_line_width_ink [i - EDIT_WIDTH_INK] < 0)
    {
      ErrList (9) ;
      return FALSE;
    }
  }
  
  i = EDIT_COLOR;
  for ( k = 0; k < No_Color; k++)
  {
    if (sscanf ( edit_table[i++].txt, "%d", &ptr_plt_ini_data->tab_pen_number [k].line_bc) < 1 ||
        sscanf ( edit_table[i++].txt, "%d", &ptr_plt_ini_data->tab_pen_number [k].line_c) < 1 ||
	sscanf ( edit_table[i++].txt, "%d", &ptr_plt_ini_data->tab_pen_number [k].line_g) < 1 ||
	sscanf ( edit_table[i++].txt, "%d", &ptr_plt_ini_data->tab_pen_number [k].line_bg) < 1 ||
	sscanf ( edit_table[i++].txt, "%d", &ptr_plt_ini_data->tab_pen_number [k].line_ng) < 1)
    {
		ErrList (9) ;
      return FALSE;
    }
  }
  return TRUE;
}

static int proc_dlg_plt_ini_data( int n)
/*-------------------------------------*/
{
  int edit_no, pen_no, i;
  int ret = Dlg_Ret_Val_Continue;

  switch (n)
  {
    case ID_OK:
      if (get_plt_dlg_control (&plt_ini_data) == TRUE)
      {
	ret = Dlg_Ret_Val_OK;
      }
      break;
    case ID_CANCEL:
      ret = Dlg_Ret_Val_Cancel;
      break;
	case  ID_COLOR_255 + 1:
	case  ID_COLOR_255 + 2:
	case  ID_COLOR_255 + 3:
	case  ID_COLOR_255 + 4:
	case  ID_COLOR_255 + 5:
	case  ID_COLOR_255 + 6:
	case  ID_COLOR_255 + 7:
	case  ID_COLOR_255 + 8:
	case  ID_COLOR_255 + 9:
	case  ID_COLOR_255 + 10:
	case  ID_COLOR_255 + 11:
	case  ID_COLOR_255 + 12:
	case  ID_COLOR_255 + 13:
	case  ID_COLOR_255 + 14:
	case  ID_COLOR_255 + 15:
	case  ID_COLOR_255 + 16:
		  edit_no = EDIT_COLOR + (n - ID_COLOR_255 - 1) * 5;
		  if (sscanf(edit_table[edit_no].txt, "%d", &pen_no) < 1) break;

		  //saving grid
		  for (i = 0; i < 80; i++)
		  {
			  strncpy(back_table[i], edit_table[18 + i].txt, 3);
		  }
		  back_saved = TRUE;

		  for (i = 0; i < 5; i++)
		  {
			  sprintf(edit_table[edit_no+i].txt, "%d", pen_no);
			  Draw_Input_Line(&edit_table[edit_no+i]);
		  }

		  break;
	case  ID_COLOR_255 + 17:
	case  ID_COLOR_255 + 18:
	case  ID_COLOR_255 + 19:
	case  ID_COLOR_255 + 20:
	case  ID_COLOR_255 + 21:

		edit_no = EDIT_COLOR + (n - ID_COLOR_255 - 17);
		if (sscanf(edit_table[edit_no].txt, "%d", &pen_no) < 1) break;

		//saving grid
		for (i = 0; i < 80; i++)
		{
			strncpy(back_table[i], edit_table[18 + i].txt, 3);
		}
		back_saved = TRUE;

		for (i=0; i<80; i+=5)
		{
			sprintf(edit_table[edit_no + i].txt, "%d", pen_no);
			Draw_Input_Line(&edit_table[edit_no + i]);
		}

		break;
	case  ID_COLOR_255 + 22:  //back
		//saving grid
		edit_no = EDIT_COLOR + (n - ID_COLOR_255 - 22);

		if (back_saved == FALSE) break;
		for (i = 0; i < 80; i++)
		{
			strcpy( edit_table[edit_no + i].txt, back_table[i]);
			Draw_Input_Line(&edit_table[edit_no + i]);
		}
		break;
	case  ID_COLOR_255 + 23:   //all

		edit_no = EDIT_COLOR + (n - ID_COLOR_255 - 23);
		if (sscanf(edit_table[edit_no].txt, "%d", &pen_no) < 1) break;

		//saving grid
		for (i = 0; i < 80; i++)
		{
			strncpy(back_table[i], edit_table[edit_no + i].txt, 3);
		}
		back_saved = TRUE;
		
		for (i = 0; i < 80; i ++)
		{
			sprintf(edit_table[edit_no + i].txt, "%d", pen_no);
			Draw_Input_Line(&edit_table[edit_no + i]);
		}
		break;
	case  ID_COLOR_255 + 24:  //save
		if (get_plt_dlg_control(&plt_ini_data_save) == TRUE)
		{
			ret = Dlg_Ret_Val_Zapis_Cfg;
		}
		break;
	case  ID_COLOR_255 + 25:  //load
		ret = Dlg_Ret_Val_Odczyt_Cfg;
		break;
    default:
      ret = Dlg_Ret_Val_Continue;
      break;
  }
  return ret;
}


static BOOL set_plt_ini_dane (void)
/*----------------------------------*/
{
  T_Plt_Ini_Date plt_ini_date_old;
  DLG_COLOR color_dlg;
  char speed_text_ [No_Pen];
  char width_ink_text_ [No_Pen];
  char color_text_ [No_Color * 5/*3*/];
  int i;
  int k;
  int Ret_Val;
  char sk[MAXPATH] = "";
  char st[MAXPATH] = "";
  int i_f_handle;
  int key;
  int identyfikator;
  BOOL error_cfg;
  static int curr_h, curr_v;
  
  if (first == TRUE)
  {
    set_struct_dialog_control ();
    Get_Private_Profile_Strings ((T_Fstring)PLOTER_DATA, get_plt_init_data_from_ini_file);
    //dla zainicjowania

    for (i=0; i<255; i++)
     {
       setrgb2cmyk(&plt_ini_data.tab_pen_color[i].c255,
                   &plt_ini_data.tab_pen_color[i].m255,
                   &plt_ini_data.tab_pen_color[i].y255, GetColorAC1(i+1));
     }

    //
    Get_Default_Color_Dlg (&color_dlg);
  }
  plt_ini_data.xp = Xmin;               /* okno do plotowania*/
  plt_ini_data.yp = Ymin;               /* okno do plotowania*/
  plt_ini_data.xk = Xmax;               /* okno do plotowania*/
  plt_ini_data.yk = Ymax;               /* okno do plotowania*/
  init_ploter_dlg_control ();
  memcpy ( &plt_ini_date_old, &plt_ini_data,  sizeof (T_Plt_Ini_Date));
  Get_Default_Color_Dlg (&color_dlg);
  strcpy(port0,plt_file);

  okno_dial:

  Save_Update_flex(0, &curr_h, &curr_v);
  Ret_Val = Dialog(&ploter_dlg, &color_dlg, proc_dlg_plt_ini_data, bMouse);
  Save_Update_flex(1, &curr_h, &curr_v);
	if (Ret_Val == Dlg_Ret_Val_Zapis_Cfg)
	{
		/*zapis konfiguracji i powrot do okna dialogowego*/

		strcpy(sk, "");
		if (Load_File(sk, ZAPIS_PLT_CFG, TRUE) == FALSE)
		{
			goto okno_dial;
		}

#ifndef LINUX
        i_f_handle=open(sk, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
        i_f_handle=open(sk, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
		if (i_f_handle == -1)
		{
			strcpy(st, _FILE_OPENING_ERROR_);
			strcat(st, sk);
			komunikat_str(st);
			key = Get_Legal_Key("\033\015");
			komunikat(0);
			goto okno_dial;
		}
		else
		{
			error_cfg = FALSE;
			identyfikator = 234567890;

			if (write(i_f_handle, &identyfikator, sizeof(int)) != sizeof(int)) error_cfg = TRUE;
			else if (write(i_f_handle, &plt_ini_data_save, sizeof(T_Plt_Ini_Date)) != sizeof(T_Plt_Ini_Date)) error_cfg = TRUE;

			if (error_cfg == TRUE)
			{
				strcpy(st, _FILE_WRITING_ERROR_);
				strcat(st, sk);
				komunikat_str(st);
				key = Get_Legal_Key("\033\015");
				komunikat(0);
			}
			close(i_f_handle);
		}
		goto okno_dial;
	}
	else if (Ret_Val == Dlg_Ret_Val_Odczyt_Cfg)
	{
		/*odczyt konfiguracji i powrot do okna dialogowego*/
		memcpy(&plt_ini_data_bak, &plt_ini_data, sizeof(T_Plt_Ini_Date));

		strcpy(sk, "");
		if (Load_File(sk, ODCZYT_PLT_CFG, TRUE) == FALSE)
		{
			goto okno_dial;
		}
		i_f_handle = open(sk, O_BINARY | O_RDONLY, S_IREAD);
		if (i_f_handle == -1)
		{
			strcpy(st, _FILE_OPENING_ERROR_);
			strcat(st, sk);
			komunikat_str(st);
			key = Get_Legal_Key("\033\015");
			komunikat(0);
			goto okno_dial;
		}
		else
		{
			error_cfg = FALSE;
			identyfikator = 0; //234567890;

			if (read(i_f_handle, &identyfikator, sizeof(int)) != sizeof(int)) error_cfg = TRUE;
			{
				if (identyfikator != 234567890) error_cfg = TRUE;
				else if (read(i_f_handle, &plt_ini_data, sizeof(T_Plt_Ini_Date)) != sizeof(T_Plt_Ini_Date)) error_cfg = TRUE;
			}

			if (error_cfg == TRUE)
			{
				strcpy(st, _FILE_READING_ERROR_);
				strcat(st, sk);
				komunikat_str(st);
				key = Get_Legal_Key("\033\015");
				komunikat(0);

				memcpy(&plt_ini_data, &plt_ini_data_bak, sizeof(T_Plt_Ini_Date));
			}
			else init_ploter_dlg_control();

			close(i_f_handle);
		}
		goto okno_dial;
	}
	else
		if (Ret_Val != Dlg_Ret_Val_OK)  /*mouse ???????*/
		{
			return FALSE;
		}


  if (( 0 != memcmp ( &plt_ini_date_old, &plt_ini_data,  sizeof (T_Plt_Ini_Date)))
     || (strcmp(port0,plt_file)!=0))
  {
    Add_Private_Profile_Group ((T_Fstring)PLOTER_DATA, add_value);
  }
  return TRUE;
}


static BOOL
get_config_param_from_ini_file (T_Fstring key_name, T_Fstring ret_string)
/*---------------------------------------------------------------------*/
{
   int i;
   int int_val;
    char plt_conf_par[128];

   utf8Upper(key_name);
   for (i = 0; i < no_plt_config_param; i++)
   {
       strcpy(plt_conf_par, plt_config_param [i]);
       utf8Upper(&plt_conf_par);

      if (stricmp (key_name, plt_conf_par) == 0)
      {
	 break;
      }
   }
   if (i >= no_plt_config_param)
   {
		return TRUE ;
   }
  switch (i)
  {
	case IDPlt_type:
		if (sscanf(ret_string, "%d", &int_val) == 1)
		{
			if (int_val == 1 || int_val == 2 || int_val == 99)
				plt_config.type = int_val;
		}
		break;
    case IDPlt_border:
      if (sscanf (ret_string, "%ld %ld %ld %ld", &plt_config.xmin,
      &plt_config.ymin, &plt_config.xmax, &plt_config.ymax) < 4) goto aa;
      break;
    case IDPlt_rozdz:
      if ( sscanf ( ret_string, "%d", &plt_config.rozdz ) < 1 ) goto aa;
      if ( plt_config.rozdz <= 0 ) goto aa;
      break;
    case IDPlt_origin:
      if ( sscanf ( ret_string, "%d", &plt_config.origin ) < 1 ) goto aa;
      if (( plt_config.origin != 0 ) && (plt_config.origin != 1 )) plt_config.origin = 0 ;
      if (plt_config.origin==1)
      {
       plt_config.dx0 = 0.5 * (Xmax - Xmin);
       plt_config.dy0 = 0.5 * (Ymax - Ymin);
       }
	  else
	  {
	  plt_config.dx0 = 0;
	  plt_config.dy0 = 0;
	  }
      break;
    case IDPlt_ini:
		strncpy (plt_config.ini, ret_string, MAXWZLEN);
      plt_config.ini [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_end:
      strncpy (plt_config.end, ret_string, MAXWZLEN);
      plt_config.end [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_pen_sel:
      strncpy (plt_config.pen_sel, ret_string, MAXWZLEN);
      plt_config.pen_sel [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_pen_speed:
      strncpy (plt_config.pen_speed, ret_string, MAXWZLEN);
      plt_config.pen_speed [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_pen_width_ink:
      strncpy (plt_config.pen_width_ink, ret_string, MAXWZLEN);
      plt_config.pen_width_ink [MAXWZLEN -1] = '\0';
      break;  
    case IDPlt_pen_up:
      strncpy (plt_config.pen_up, ret_string, MAXWZLEN);
      plt_config.pen_up [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_pen_dn:
      strncpy (plt_config.pen_dn, ret_string, MAXWZLEN);
      plt_config.pen_dn [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_pen_abs:
      strncpy (plt_config.pen_abs, ret_string, MAXWZLEN);
      plt_config.pen_abs [MAXWZLEN -1] = '\0';
      break;
	 case IDPlt_pen_line:
      strncpy (plt_config.pen_line, ret_string, MAXWZLEN);
      plt_config.pen_line [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_pen_move:
      strncpy (plt_config.pen_move, ret_string, MAXWZLEN);
      plt_config.pen_move [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_ter_wsp:
      strncpy (plt_config.ter_wsp, ret_string, MAXWZLEN);
      plt_config.ter_wsp [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_ter_kom:
      strncpy (plt_config.ter_kom, ret_string, MAXWZLEN);
      plt_config.ter_kom [MAXWZLEN -1] = '\0';
      break;
    case IDPlt_l_luk:
      if ( sscanf ( ret_string, "%lf", &plt_config.l_luk ) < 1 ) goto aa;
      if (plt_config.l_luk <= 0 ) goto aa;
      break;
    default:
      break;
  }
  return TRUE;
aa:
  return FALSE;
}


static BOOL get_plt_init_data_from_ini_file (T_Fstring key_name, T_Fstring ret_string)
/*-----------------------------------------------------------------------------------------*/
{
  int i, j;
  double double_val;
  BOOL bool_val;
  int int_val, int_val_table [MAXNPEN], no_speed, no_width_ink;
  double double_val_table [MAXNPEN];
  long long_val1, long_val2;

  //utf8Upper(key_name);
  for (i = 0; i < no_plt_ini_data_param; i++)
  {
    if (stricmp (key_name, plt_ini_data_param[i]) == 0)
    {
      break;
    }
  }
  if (i >= no_plt_ini_data_param)
  {
    return FALSE;
  }
  switch (i)
  {
    case IDPlt_dx_dy:
       if ( sscanf ( ret_string, "%ld%ld", &long_val1, &long_val2) == 2 )
       {
	 plt_ini_data.dx = long_val1;
	 plt_ini_data.dy = long_val2;
       }
       break;
    case IDPlt_rotation:
       if ( sscanf ( ret_string, "%d", &int_val) == 1 )
       {
	 plt_ini_data.rotation = int_val;
       }
       break;
    case IDPlt_pen_width:
       if ( sscanf ( ret_string, "%lf", &double_val) == 1 )
	 if (double_val > 0)
	 {
	   plt_ini_data.pen_width= double_val;
	 }
       break;
    case IDPlt_scale:
       if ( sscanf ( ret_string, "%lf", &double_val) == 1 )
	 if (double_val > 0)
	 {
	   plt_ini_data.scale= double_val;
	 }
       break;
    case IDPlt_print_window :
	if ( sscanf ( ret_string, "%d", &int_val) == 1)
	{
	  if (int_val != 0) int_val = 1 ;
	  plt_ini_data.print_window = int_val;
	}
	break;
   case IDPlt_print_sheet :
	if ( sscanf ( ret_string, "%d", &int_val) == 1)
	{
	  if (int_val != 0) int_val = 1 ;
	  plt_ini_data.print_sheet = int_val;
	}
	break;
    case IDPlt_ink_plotter :
	if ( sscanf ( ret_string, "%d", &int_val) == 1)
	{
	  if (int_val != 0) int_val = 1 ;
	  plt_ini_data.ink_plotter = int_val;
	}
	break;
    case IDPlt_fixed_name :
	if ( sscanf ( ret_string, "%d", &int_val) == 1)
	{
	  if (int_val != 0) int_val = 1 ;
	  plt_ini_data.fixed_name = int_val;
	}
	break;
    case IDPlt_page_size :
	if ( sscanf ( ret_string, "%d", &int_val) == 1)
	{
	  if (int_val != 0) int_val = 1 ;
	  plt_ini_data.page_size = int_val;
	}
	break;
    case IDPlt_color_print :
	if ( sscanf ( ret_string, "%d", &int_val) == 1)
	{
	  if (int_val != 0) int_val = 1 ;
	  plt_ini_data.color_print = int_val;
	}
	break;
    case IDPlt_color_255 :
	if ( sscanf ( ret_string, "%d", &int_val) == 1)
	{
	  if (int_val != 0) int_val = 1 ;
	  plt_ini_data.color_255 = int_val;
	}
	break;
    case IDPlt_port:
        strncpy (plt_ini_data.port, ret_string, MaxTextLen);
        plt_ini_data.port [MaxTextLen -1] = '\0';
        
        if (strlen(plt_ini_data.port)>0)
         {
         strcpy(plt_file,plt_ini_data.port);
         }
        break;
    case IDPlt_tab_pen_speed:
       if ((no_speed = sscanf ( ret_string,
	    "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d ",
	    &int_val_table [0], &int_val_table [1], &int_val_table [2],
	    &int_val_table [3], &int_val_table [4], &int_val_table [5],
	    &int_val_table [6], &int_val_table [7], &int_val_table [8],
	    &int_val_table [9], &int_val_table [10], &int_val_table [11],
	    &int_val_table [12],&int_val_table [13], &int_val_table [14],
	    &int_val_table [15] )) >=1 )
       {
	 memcpy (plt_ini_data.tab_pen_speed, int_val_table, no_speed * sizeof(int)) ;
       }
       for ( j = 0; j < No_Pen; j++)
       {
	 if (plt_ini_data.tab_pen_speed [j] <= 0)
	   plt_ini_data.tab_pen_speed [j] = def_speed;
       }
       break;
   case IDPlt_tab_line_width_ink:
       if ((no_width_ink = sscanf ( ret_string,
	    "%lf%lf%lf%lf%lf ",
	    &double_val_table [0], &double_val_table [1], &double_val_table [2],
	    &double_val_table [3], &double_val_table [4] )) >=1 )
       {
	 memcpy (plt_ini_data.tab_line_width_ink, double_val_table, no_width_ink * sizeof(double)) ;
       }
       for ( j = 0; j < 5; j++)
       {
     	   if (plt_ini_data.tab_line_width_ink [j] <= 0)
	      plt_ini_data.tab_line_width_ink [j] = def_line_width_ink[j];
       }
       break;
       case IDPlt_number_of_pen :
       if ( sscanf ( ret_string, "%d", &int_val) == 1)
	      {
	       if ((int_val <= 0) || (int_val > 255)) int_val=255;
	       plt_ini_data.number_of_pen = int_val;
	      }
	    break;
       case IDPlt_color1:  case IDPlt_color2:  case IDPlt_color3:
       case IDPlt_color4:  case IDPlt_color5:  case IDPlt_color6:
       case IDPlt_color7:  case IDPlt_color8:  case IDPlt_color9:
       case IDPlt_color10: 
       case IDPlt_color11: case IDPlt_color12: case IDPlt_color13:
       case IDPlt_color14: case IDPlt_color15: case IDPlt_color16: 
	   /*
       case IDPlt_color17: case IDPlt_color18: case IDPlt_color19:
       case IDPlt_color20: 
       case IDPlt_color21:  case IDPlt_color22:  case IDPlt_color23:
       case IDPlt_color24:  case IDPlt_color25:  case IDPlt_color26:
       case IDPlt_color27:  case IDPlt_color28:  case IDPlt_color29:
       case IDPlt_color30: 
       case IDPlt_color31: case IDPlt_color32:
       case IDPlt_color33: case IDPlt_color34: case IDPlt_color35:
       case IDPlt_color36: case IDPlt_color37: case IDPlt_color38: 
       case IDPlt_color39: case IDPlt_color40: 
       case IDPlt_color41: case IDPlt_color42:
       case IDPlt_color43: case IDPlt_color44: case IDPlt_color45:
       case IDPlt_color46: case IDPlt_color47: case IDPlt_color48: 
       case IDPlt_color49: case IDPlt_color50: 
       case IDPlt_color51: case IDPlt_color52:
       case IDPlt_color53: case IDPlt_color54: case IDPlt_color55:
       case IDPlt_color56: case IDPlt_color57: case IDPlt_color58: 
       case IDPlt_color59: case IDPlt_color60: 
       case IDPlt_color61: case IDPlt_color62:
       case IDPlt_color63: case IDPlt_color64: case IDPlt_color65:
       case IDPlt_color66: case IDPlt_color67: case IDPlt_color68: 
       case IDPlt_color69: case IDPlt_color70: 
       case IDPlt_color71: case IDPlt_color72:
       case IDPlt_color73: case IDPlt_color74: case IDPlt_color75:
       case IDPlt_color76: case IDPlt_color77: case IDPlt_color78: 
       case IDPlt_color79: case IDPlt_color80: 
       case IDPlt_color81: case IDPlt_color82:
       case IDPlt_color83: case IDPlt_color84: case IDPlt_color85:
       case IDPlt_color86: case IDPlt_color87: case IDPlt_color88: 
       case IDPlt_color89: case IDPlt_color90: 
       case IDPlt_color91: case IDPlt_color92:
       case IDPlt_color93: case IDPlt_color94: case IDPlt_color95:
       case IDPlt_color96: case IDPlt_color97: case IDPlt_color98: 
       case IDPlt_color99: case IDPlt_color100: 
       case IDPlt_color101:  case IDPlt_color102:  case IDPlt_color103:
       case IDPlt_color104:  case IDPlt_color105:  case IDPlt_color106:
       case IDPlt_color107:  case IDPlt_color108:  case IDPlt_color109:
       case IDPlt_color110: 
       case IDPlt_color111: case IDPlt_color112: case IDPlt_color113:
       case IDPlt_color114: case IDPlt_color115: case IDPlt_color116: 
       case IDPlt_color117: case IDPlt_color118: case IDPlt_color119:
       case IDPlt_color120: 
       case IDPlt_color121:  case IDPlt_color122:  case IDPlt_color123:
       case IDPlt_color124:  case IDPlt_color125:  case IDPlt_color126:
       case IDPlt_color127:  case IDPlt_color128:  case IDPlt_color129:
       case IDPlt_color130: 
       case IDPlt_color131: case IDPlt_color132:
       case IDPlt_color133: case IDPlt_color134: case IDPlt_color135:
       case IDPlt_color136: case IDPlt_color137: case IDPlt_color138: 
       case IDPlt_color139: case IDPlt_color140: 
       case IDPlt_color141: case IDPlt_color142:
       case IDPlt_color143: case IDPlt_color144: case IDPlt_color145:
       case IDPlt_color146: case IDPlt_color147: case IDPlt_color148: 
       case IDPlt_color149: case IDPlt_color150: 
       case IDPlt_color151: case IDPlt_color152:
       case IDPlt_color153: case IDPlt_color154: case IDPlt_color155:
       case IDPlt_color156: case IDPlt_color157: case IDPlt_color158: 
       case IDPlt_color159: case IDPlt_color160: 
       case IDPlt_color161: case IDPlt_color162:
       case IDPlt_color163: case IDPlt_color164: case IDPlt_color165:
       case IDPlt_color166: case IDPlt_color167: case IDPlt_color168: 
       case IDPlt_color169: case IDPlt_color170: 
       case IDPlt_color171: case IDPlt_color172:
       case IDPlt_color173: case IDPlt_color174: case IDPlt_color175:
       case IDPlt_color176: case IDPlt_color177: case IDPlt_color178: 
       case IDPlt_color179: case IDPlt_color180: 
       case IDPlt_color181: case IDPlt_color182:
       case IDPlt_color183: case IDPlt_color184: case IDPlt_color185:
       case IDPlt_color186: case IDPlt_color187: case IDPlt_color188: 
       case IDPlt_color189: case IDPlt_color190: 
       case IDPlt_color191: case IDPlt_color192:
       case IDPlt_color193: case IDPlt_color194: case IDPlt_color195:
       case IDPlt_color196: case IDPlt_color197: case IDPlt_color198: 
       case IDPlt_color199: case IDPlt_color200:
       case IDPlt_color201:  case IDPlt_color202:  case IDPlt_color203:
       case IDPlt_color204:  case IDPlt_color205:  case IDPlt_color206:
       case IDPlt_color207:  case IDPlt_color208:  case IDPlt_color209:
       case IDPlt_color210: 
       case IDPlt_color211: case IDPlt_color212: case IDPlt_color213:
       case IDPlt_color214: case IDPlt_color215: case IDPlt_color216: 
       case IDPlt_color217: case IDPlt_color218: case IDPlt_color219:
       case IDPlt_color220: 
       case IDPlt_color221:  case IDPlt_color222:  case IDPlt_color223:
       case IDPlt_color224:  case IDPlt_color225:  case IDPlt_color226:
       case IDPlt_color227:  case IDPlt_color228:  case IDPlt_color229:
       case IDPlt_color230: 
       case IDPlt_color231: case IDPlt_color232:
       case IDPlt_color233: case IDPlt_color234: case IDPlt_color235:
       case IDPlt_color236: case IDPlt_color237: case IDPlt_color238: 
       case IDPlt_color239: case IDPlt_color240: 
       case IDPlt_color241: case IDPlt_color242:
       case IDPlt_color243: case IDPlt_color244: case IDPlt_color245:
       case IDPlt_color246: case IDPlt_color247: case IDPlt_color248: 
       case IDPlt_color249: case IDPlt_color250: 
       case IDPlt_color251: case IDPlt_color252:
       case IDPlt_color253: case IDPlt_color254: case IDPlt_color255:
       */
       
       if (sscanf ( ret_string, "%d%d%d%d%d ", &int_val_table [0],
      	 &int_val_table [1], &int_val_table [2], &int_val_table [3], &int_val_table [4]) == 5/*3*/ )
       {
         if ( int_val_table [0] < 0 || int_val_table [0] > 255) int_val_table [0] = 1;
	      if ( int_val_table [1] < 0 || int_val_table [1] > 255) int_val_table [1] = 1;
         if ( int_val_table [2] < 0 || int_val_table [2] > 255) int_val_table [2] = 1;
	      if ( int_val_table [3] < 0 || int_val_table [3] > 255) int_val_table [3] = 1;
	      if ( int_val_table [4] < 0 || int_val_table [4] > 255) int_val_table [4] = 1;
	      memcpy (&(plt_ini_data.tab_pen_number [i-IDPlt_color1]), int_val_table, 5 * sizeof(int)) ;
       }
        break;
		/*
       case IDPlt_pcolor1:  case IDPlt_pcolor2:  case IDPlt_pcolor3:
       case IDPlt_pcolor4:  case IDPlt_pcolor5:  case IDPlt_pcolor6:
       case IDPlt_pcolor7:  case IDPlt_pcolor8:  case IDPlt_pcolor9:
       case IDPlt_pcolor10:
       case IDPlt_pcolor11: case IDPlt_pcolor12: case IDPlt_pcolor13:
       case IDPlt_pcolor14: case IDPlt_pcolor15: case IDPlt_pcolor16:
       case IDPlt_pcolor17: case IDPlt_pcolor18: case IDPlt_pcolor19:
       case IDPlt_pcolor20:
       case IDPlt_pcolor21:  case IDPlt_pcolor22:  case IDPlt_pcolor23:
       case IDPlt_pcolor24:  case IDPlt_pcolor25:  case IDPlt_pcolor26:
       case IDPlt_pcolor27:  case IDPlt_pcolor28:  case IDPlt_pcolor29:
       case IDPlt_pcolor30:
       case IDPlt_pcolor31: case IDPlt_pcolor32:
       case IDPlt_pcolor33: case IDPlt_pcolor34: case IDPlt_pcolor35:
       case IDPlt_pcolor36: case IDPlt_pcolor37: case IDPlt_pcolor38:
       case IDPlt_pcolor39: case IDPlt_pcolor40:
       case IDPlt_pcolor41: case IDPlt_pcolor42:
       case IDPlt_pcolor43: case IDPlt_pcolor44: case IDPlt_pcolor45:
       case IDPlt_pcolor46: case IDPlt_pcolor47: case IDPlt_pcolor48: 
       case IDPlt_pcolor49: case IDPlt_pcolor50: 
       case IDPlt_pcolor51: case IDPlt_pcolor52:
       case IDPlt_pcolor53: case IDPlt_pcolor54: case IDPlt_pcolor55:
       case IDPlt_pcolor56: case IDPlt_pcolor57: case IDPlt_pcolor58: 
       case IDPlt_pcolor59: case IDPlt_pcolor60: 
       case IDPlt_pcolor61: case IDPlt_pcolor62:
       case IDPlt_pcolor63: case IDPlt_pcolor64:
       case IDPlt_pcolor65:
       case IDPlt_pcolor66: case IDPlt_pcolor67: case IDPlt_pcolor68: 
       case IDPlt_pcolor69: case IDPlt_pcolor70: 
       case IDPlt_pcolor71: case IDPlt_pcolor72:
       case IDPlt_pcolor73: case IDPlt_pcolor74: case IDPlt_pcolor75:
       case IDPlt_pcolor76: case IDPlt_pcolor77: case IDPlt_pcolor78: 
       case IDPlt_pcolor79: case IDPlt_pcolor80: 
       case IDPlt_pcolor81: case IDPlt_pcolor82:
       case IDPlt_pcolor83: case IDPlt_pcolor84: case IDPlt_pcolor85:
       case IDPlt_pcolor86: case IDPlt_pcolor87: case IDPlt_pcolor88: 
       case IDPlt_pcolor89: case IDPlt_pcolor90: 
       case IDPlt_pcolor91: case IDPlt_pcolor92:
       case IDPlt_pcolor93: case IDPlt_pcolor94: case IDPlt_pcolor95:
       case IDPlt_pcolor96: case IDPlt_pcolor97: case IDPlt_pcolor98: 
       case IDPlt_pcolor99: case IDPlt_pcolor100:
       case IDPlt_pcolor101:  case IDPlt_pcolor102:  case IDPlt_pcolor103:
       case IDPlt_pcolor104:  case IDPlt_pcolor105:  case IDPlt_pcolor106:
       case IDPlt_pcolor107:  case IDPlt_pcolor108:  case IDPlt_pcolor109:
       case IDPlt_pcolor110: 
       case IDPlt_pcolor111: case IDPlt_pcolor112: case IDPlt_pcolor113:
       case IDPlt_pcolor114: case IDPlt_pcolor115: case IDPlt_pcolor116: 
       case IDPlt_pcolor117: case IDPlt_pcolor118: case IDPlt_pcolor119:
       case IDPlt_pcolor120: 
       case IDPlt_pcolor121:  case IDPlt_pcolor122:  case IDPlt_pcolor123:
       case IDPlt_pcolor124:  case IDPlt_pcolor125:  case IDPlt_pcolor126:
       case IDPlt_pcolor127:  case IDPlt_pcolor128:  case IDPlt_pcolor129:
       case IDPlt_pcolor130: 
       case IDPlt_pcolor131: case IDPlt_pcolor132:
       case IDPlt_pcolor133: case IDPlt_pcolor134: case IDPlt_pcolor135:
       case IDPlt_pcolor136: case IDPlt_pcolor137: case IDPlt_pcolor138: 
       case IDPlt_pcolor139: case IDPlt_pcolor140: 
       case IDPlt_pcolor141: case IDPlt_pcolor142:
       case IDPlt_pcolor143: case IDPlt_pcolor144: case IDPlt_pcolor145:
       case IDPlt_pcolor146: case IDPlt_pcolor147: case IDPlt_pcolor148: 
       case IDPlt_pcolor149: case IDPlt_pcolor150: 
       case IDPlt_pcolor151: case IDPlt_pcolor152:
       case IDPlt_pcolor153: case IDPlt_pcolor154: case IDPlt_pcolor155:
       case IDPlt_pcolor156: case IDPlt_pcolor157: case IDPlt_pcolor158: 
       case IDPlt_pcolor159: case IDPlt_pcolor160: 
       case IDPlt_pcolor161: case IDPlt_pcolor162:
       case IDPlt_pcolor163: case IDPlt_pcolor164: case IDPlt_pcolor165:
       case IDPlt_pcolor166: case IDPlt_pcolor167: case IDPlt_pcolor168: 
       case IDPlt_pcolor169: case IDPlt_pcolor170: 
       case IDPlt_pcolor171: case IDPlt_pcolor172:
       case IDPlt_pcolor173: case IDPlt_pcolor174: case IDPlt_pcolor175:
       case IDPlt_pcolor176: case IDPlt_pcolor177: case IDPlt_pcolor178: 
       case IDPlt_pcolor179: case IDPlt_pcolor180: 
       case IDPlt_pcolor181: case IDPlt_pcolor182:
       case IDPlt_pcolor183: case IDPlt_pcolor184: case IDPlt_pcolor185:
       case IDPlt_pcolor186: case IDPlt_pcolor187: case IDPlt_pcolor188: 
       case IDPlt_pcolor189: case IDPlt_pcolor190: 
       case IDPlt_pcolor191: case IDPlt_pcolor192:
       case IDPlt_pcolor193: case IDPlt_pcolor194: case IDPlt_pcolor195:
       case IDPlt_pcolor196: case IDPlt_pcolor197: case IDPlt_pcolor198: 
       case IDPlt_pcolor199: case IDPlt_pcolor200:
       case IDPlt_pcolor201:  case IDPlt_pcolor202:  case IDPlt_pcolor203:
       case IDPlt_pcolor204:  case IDPlt_pcolor205:  case IDPlt_pcolor206:
       case IDPlt_pcolor207:  case IDPlt_pcolor208:  case IDPlt_pcolor209:
       case IDPlt_pcolor210: 
       case IDPlt_pcolor211: case IDPlt_pcolor212: case IDPlt_pcolor213:
       case IDPlt_pcolor214: case IDPlt_pcolor215: case IDPlt_pcolor216: 
       case IDPlt_pcolor217: case IDPlt_pcolor218: case IDPlt_pcolor219:
       case IDPlt_pcolor220: 
       case IDPlt_pcolor221:  case IDPlt_pcolor222:  case IDPlt_pcolor223:
       case IDPlt_pcolor224:  case IDPlt_pcolor225:  case IDPlt_pcolor226:
       case IDPlt_pcolor227:  case IDPlt_pcolor228:  case IDPlt_pcolor229:
       case IDPlt_pcolor230: 
       case IDPlt_pcolor231: case IDPlt_pcolor232:
       case IDPlt_pcolor233: case IDPlt_pcolor234: case IDPlt_pcolor235:
       case IDPlt_pcolor236: case IDPlt_pcolor237: case IDPlt_pcolor238: 
       case IDPlt_pcolor239: case IDPlt_pcolor240: 
       case IDPlt_pcolor241: case IDPlt_pcolor242:
       case IDPlt_pcolor243: case IDPlt_pcolor244: case IDPlt_pcolor245:
       case IDPlt_pcolor246: case IDPlt_pcolor247: case IDPlt_pcolor248: 
       case IDPlt_pcolor249: case IDPlt_pcolor250: 
       case IDPlt_pcolor251: case IDPlt_pcolor252:
       case IDPlt_pcolor253: case IDPlt_pcolor254: case IDPlt_pcolor255:
       */
        /*
       if (sscanf ( ret_string, "%d%d%d,%d%d%d ", &int_val_table [0], &int_val_table [1], &int_val_table [2],
                                                  &int_val_table [3], &int_val_table [4], &int_val_table [5]) == 6 )
       {
         if ( int_val_table [0] < 0 || int_val_table [0] > 255) int_val_table [0] = 0;
	      if ( int_val_table [1] < 0 || int_val_table [1] > 255) int_val_table [1] = 0;
         if ( int_val_table [2] < 0 || int_val_table [2] > 255) int_val_table [2] = 0;
         if ( int_val_table [3] < 0 || int_val_table [3] > 255) int_val_table [3] = 0;
	      if ( int_val_table [4] < 0 || int_val_table [4] > 255) int_val_table [4] = 0;
         if ( int_val_table [5] < 0 || int_val_table [5] > 255) int_val_table [5] = 0;
	      memcpy (&(plt_ini_data.tab_pen_color [i-IDPlt_pcolor1]), int_val_table, 6 * sizeof(int)) ;
       }
	   break;
	   */
  }
  return TRUE;
}

static BOOL add_value (FILE *stru)
/*-------------------------------*/
{
  T_Fstring key_name;
  T_Fstring comment;
  int i;
  int port_num ;


   for (i = 0; i < no_plt_ini_data_param; i++)
   {
     key_name = plt_ini_data_param [i];
     comment = plt_ini_data_param_comment [i];
     switch (i)
     {
       case IDPlt_dx_dy:
	  if (EOF == fprintf (stru, "%s=%ld %ld\t\t%s\n",
			key_name, plt_ini_data.dx, plt_ini_data.dy, comment))
	    return FALSE;
	  break;
       case IDPlt_rotation:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, plt_ini_data.rotation, comment))
	    return FALSE;
	  break;
       case IDPlt_pen_width:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, plt_ini_data.pen_width, comment))
	    return FALSE;
	  break;
      case IDPlt_scale:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, plt_ini_data.scale, comment))
	    return FALSE;
	  break;
      case IDPlt_print_window:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, plt_ini_data.print_window, comment))
	    return FALSE;
	break;
   case IDPlt_print_sheet:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, plt_ini_data.print_sheet, comment))
	    return FALSE;
	break;
      case IDPlt_ink_plotter:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, plt_ini_data.ink_plotter, comment))
	    return FALSE;
	break;
     case IDPlt_fixed_name:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, plt_ini_data.fixed_name, comment))
	    return FALSE;
	break;
   case IDPlt_page_size:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, plt_ini_data.page_size, comment))
	    return FALSE;
	break;
   case IDPlt_color_print:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, plt_ini_data.color_print, comment))
	    return FALSE;
	break;
   case IDPlt_color_255:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, plt_ini_data.color_255, comment))
	    return FALSE;
	break;
       case IDPlt_port:
        if (strnicmp (plt_file, "LPT", 3) == 0) 
        {
         if (sscanf (&plt_file [3] , "%d", &port_num) == 1  && (port_num > 0))
         {
         strcpy(plt_ini_data.port,plt_file);
         }
        }
       else 
        if (strnicmp (plt_file, "COM", 3) == 0)
         { 
         if (sscanf (&plt_file [3] , "%d", &port_num) == 1  && (port_num > 0))
          {
          strcpy(plt_ini_data.port,plt_file);
          }
         }
       else if (plt_ini_data.fixed_name == 1)
         { 
          strcpy(plt_ini_data.port,plt_file);
         }
       else
        strcpy(plt_ini_data.port,"");
      
          if (EOF == fprintf (stru, "%s=%s\t\t%s\n",	 
                        key_name, plt_ini_data.port, comment))
	    return FALSE;
	break;
       case IDPlt_tab_pen_speed:
	  if (EOF == fprintf (stru, "%s=%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\t\t%s\n",
			key_name,
			plt_ini_data.tab_pen_speed [0], plt_ini_data.tab_pen_speed [1],
			plt_ini_data.tab_pen_speed [2], plt_ini_data.tab_pen_speed [3],
			plt_ini_data.tab_pen_speed [4], plt_ini_data.tab_pen_speed [5],
			plt_ini_data.tab_pen_speed [6], plt_ini_data.tab_pen_speed [7],
			plt_ini_data.tab_pen_speed [8], plt_ini_data.tab_pen_speed [9],
			plt_ini_data.tab_pen_speed [10], plt_ini_data.tab_pen_speed [11],
			plt_ini_data.tab_pen_speed [12], plt_ini_data.tab_pen_speed [13],
			plt_ini_data.tab_pen_speed [14], plt_ini_data.tab_pen_speed [15],
			comment))
	    return FALSE;
	  break;
       case IDPlt_tab_line_width_ink:
	  if (EOF == fprintf (stru, "%s=%.2lf %.2lf %.2lf %.2lf %.2lf\t\t%s\n",
			key_name,
			plt_ini_data.tab_line_width_ink [0], plt_ini_data.tab_line_width_ink [1],
			plt_ini_data.tab_line_width_ink [2], plt_ini_data.tab_line_width_ink [3],
			plt_ini_data.tab_line_width_ink [4], comment))
	    return FALSE;
     break;
     case IDPlt_number_of_pen:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",	key_name, plt_ini_data.number_of_pen, comment))
	    return FALSE;
	  break;	  
       case IDPlt_color1:  case IDPlt_color2:  case IDPlt_color3:
       case IDPlt_color4:  case IDPlt_color5:  case IDPlt_color6:
       case IDPlt_color7:  case IDPlt_color8:  case IDPlt_color9:
       case IDPlt_color10: 
       case IDPlt_color11: case IDPlt_color12: case IDPlt_color13:
       case IDPlt_color14: case IDPlt_color15: case IDPlt_color16: 
       /*
       case IDPlt_color17: case IDPlt_color18: case IDPlt_color19:
       case IDPlt_color20: 
       case IDPlt_color21:  case IDPlt_color22:  case IDPlt_color23:
       case IDPlt_color24:  case IDPlt_color25:  case IDPlt_color26:
       case IDPlt_color27:  case IDPlt_color28:  case IDPlt_color29:
       case IDPlt_color30: 
       case IDPlt_color31: case IDPlt_color32:
       case IDPlt_color33: case IDPlt_color34: case IDPlt_color35:
       case IDPlt_color36: case IDPlt_color37: case IDPlt_color38: 
       case IDPlt_color39: case IDPlt_color40: 
       case IDPlt_color41: case IDPlt_color42:
       case IDPlt_color43: case IDPlt_color44: case IDPlt_color45:
       case IDPlt_color46: case IDPlt_color47: case IDPlt_color48: 
       case IDPlt_color49: case IDPlt_color50: 
       case IDPlt_color51: case IDPlt_color52:
       case IDPlt_color53: case IDPlt_color54: case IDPlt_color55:
       case IDPlt_color56: case IDPlt_color57: case IDPlt_color58: 
       case IDPlt_color59: case IDPlt_color60: 
       case IDPlt_color61: case IDPlt_color62:
       case IDPlt_color63: case IDPlt_color64: case IDPlt_color65:
       case IDPlt_color66: case IDPlt_color67: case IDPlt_color68: 
       case IDPlt_color69: case IDPlt_color70: 
       case IDPlt_color71: case IDPlt_color72:
       case IDPlt_color73: case IDPlt_color74: case IDPlt_color75:
       case IDPlt_color76: case IDPlt_color77: case IDPlt_color78: 
       case IDPlt_color79: case IDPlt_color80: 
       case IDPlt_color81: case IDPlt_color82:
       case IDPlt_color83: case IDPlt_color84: case IDPlt_color85:
       case IDPlt_color86: case IDPlt_color87: case IDPlt_color88: 
       case IDPlt_color89: case IDPlt_color90: 
       case IDPlt_color91: case IDPlt_color92:
       case IDPlt_color93: case IDPlt_color94: case IDPlt_color95:
       case IDPlt_color96: case IDPlt_color97: case IDPlt_color98: 
       case IDPlt_color99: case IDPlt_color100: 
       case IDPlt_color101:  case IDPlt_color102:  case IDPlt_color103:
       case IDPlt_color104:  case IDPlt_color105:  case IDPlt_color106:
       case IDPlt_color107:  case IDPlt_color108:  case IDPlt_color109:
       case IDPlt_color110: 
       case IDPlt_color111: case IDPlt_color112: case IDPlt_color113:
       case IDPlt_color114: case IDPlt_color115: case IDPlt_color116: 
       case IDPlt_color117: case IDPlt_color118: case IDPlt_color119:
       case IDPlt_color120: 
       case IDPlt_color121:  case IDPlt_color122:  case IDPlt_color123:
       case IDPlt_color124:  case IDPlt_color125:  case IDPlt_color126:
       case IDPlt_color127:  case IDPlt_color128:  case IDPlt_color129:
       case IDPlt_color130: 
       case IDPlt_color131: case IDPlt_color132:
       case IDPlt_color133: case IDPlt_color134: case IDPlt_color135:
       case IDPlt_color136: case IDPlt_color137: case IDPlt_color138: 
       case IDPlt_color139: case IDPlt_color140: 
       case IDPlt_color141: case IDPlt_color142:
       case IDPlt_color143: case IDPlt_color144: case IDPlt_color145:
       case IDPlt_color146: case IDPlt_color147: case IDPlt_color148: 
       case IDPlt_color149: case IDPlt_color150: 
       case IDPlt_color151: case IDPlt_color152:
       case IDPlt_color153: case IDPlt_color154: case IDPlt_color155:
       case IDPlt_color156: case IDPlt_color157: case IDPlt_color158: 
       case IDPlt_color159: case IDPlt_color160: 
       case IDPlt_color161: case IDPlt_color162:
       case IDPlt_color163: case IDPlt_color164: case IDPlt_color165:
       case IDPlt_color166: case IDPlt_color167: case IDPlt_color168: 
       case IDPlt_color169: case IDPlt_color170: 
       case IDPlt_color171: case IDPlt_color172:
       case IDPlt_color173: case IDPlt_color174: case IDPlt_color175:
       case IDPlt_color176: case IDPlt_color177: case IDPlt_color178: 
       case IDPlt_color179: case IDPlt_color180: 
       case IDPlt_color181: case IDPlt_color182:
       case IDPlt_color183: case IDPlt_color184: case IDPlt_color185:
       case IDPlt_color186: case IDPlt_color187: case IDPlt_color188: 
       case IDPlt_color189: case IDPlt_color190: 
       case IDPlt_color191: case IDPlt_color192:
       case IDPlt_color193: case IDPlt_color194: case IDPlt_color195:
       case IDPlt_color196: case IDPlt_color197: case IDPlt_color198: 
       case IDPlt_color199: case IDPlt_color200:
       case IDPlt_color201:  case IDPlt_color202:  case IDPlt_color203:
       case IDPlt_color204:  case IDPlt_color205:  case IDPlt_color206:
       case IDPlt_color207:  case IDPlt_color208:  case IDPlt_color209:
       case IDPlt_color210: 
       case IDPlt_color211: case IDPlt_color212: case IDPlt_color213:
       case IDPlt_color214: case IDPlt_color215: case IDPlt_color216: 
       case IDPlt_color217: case IDPlt_color218: case IDPlt_color219:
       case IDPlt_color220: 
       case IDPlt_color221:  case IDPlt_color222:  case IDPlt_color223:
       case IDPlt_color224:  case IDPlt_color225:  case IDPlt_color226:
       case IDPlt_color227:  case IDPlt_color228:  case IDPlt_color229:
       case IDPlt_color230: 
       case IDPlt_color231: case IDPlt_color232:
       case IDPlt_color233: case IDPlt_color234: case IDPlt_color235:
       case IDPlt_color236: case IDPlt_color237: case IDPlt_color238: 
       case IDPlt_color239: case IDPlt_color240: 
       case IDPlt_color241: case IDPlt_color242:
       case IDPlt_color243: case IDPlt_color244: case IDPlt_color245:
       case IDPlt_color246: case IDPlt_color247: case IDPlt_color248: 
       case IDPlt_color249: case IDPlt_color250: 
       case IDPlt_color251: case IDPlt_color252:
       case IDPlt_color253: case IDPlt_color254: case IDPlt_color255:
	   */

	  if (EOF == fprintf (stru, "%s=%d %d %d %d %d\t\t%s\n", key_name,
	  	plt_ini_data.tab_pen_number [i - IDPlt_color1].line_bc,
		plt_ini_data.tab_pen_number [i - IDPlt_color1].line_c,
		plt_ini_data.tab_pen_number [i - IDPlt_color1].line_g,
		plt_ini_data.tab_pen_number [i - IDPlt_color1].line_bg,
		plt_ini_data.tab_pen_number [i - IDPlt_color1].line_ng,
		comment))
	    return FALSE;
	  break;
	   /*
       case IDPlt_pcolor1:  case IDPlt_pcolor2:  case IDPlt_pcolor3:
       case IDPlt_pcolor4:  case IDPlt_pcolor5:  case IDPlt_pcolor6:
       case IDPlt_pcolor7:  case IDPlt_pcolor8:  case IDPlt_pcolor9:
       case IDPlt_pcolor10:
       case IDPlt_pcolor11: case IDPlt_pcolor12: case IDPlt_pcolor13:
       case IDPlt_pcolor14: case IDPlt_pcolor15: case IDPlt_pcolor16:
       case IDPlt_pcolor17: case IDPlt_pcolor18: case IDPlt_pcolor19:
       case IDPlt_pcolor20:
       case IDPlt_pcolor21:  case IDPlt_pcolor22:  case IDPlt_pcolor23:
       case IDPlt_pcolor24:  case IDPlt_pcolor25:  case IDPlt_pcolor26:
       case IDPlt_pcolor27:  case IDPlt_pcolor28:  case IDPlt_pcolor29:
       case IDPlt_pcolor30:
       case IDPlt_pcolor31: case IDPlt_pcolor32:
       case IDPlt_pcolor33: case IDPlt_pcolor34: case IDPlt_pcolor35:
       case IDPlt_pcolor36: case IDPlt_pcolor37: case IDPlt_pcolor38:
       case IDPlt_pcolor39: case IDPlt_pcolor40:
       case IDPlt_pcolor41: case IDPlt_pcolor42:
       case IDPlt_pcolor43: case IDPlt_pcolor44: case IDPlt_pcolor45:
       case IDPlt_pcolor46: case IDPlt_pcolor47: case IDPlt_pcolor48: 
       case IDPlt_pcolor49: case IDPlt_pcolor50: 
       case IDPlt_pcolor51: case IDPlt_pcolor52:
       case IDPlt_pcolor53: case IDPlt_pcolor54: case IDPlt_pcolor55:
       case IDPlt_pcolor56: case IDPlt_pcolor57: case IDPlt_pcolor58: 
       case IDPlt_pcolor59: case IDPlt_pcolor60: 
       case IDPlt_pcolor61: case IDPlt_pcolor62:
       case IDPlt_pcolor63: case IDPlt_pcolor64:
       case IDPlt_pcolor65:
       case IDPlt_pcolor66: case IDPlt_pcolor67: case IDPlt_pcolor68: 
       case IDPlt_pcolor69: case IDPlt_pcolor70: 
       case IDPlt_pcolor71: case IDPlt_pcolor72:
       case IDPlt_pcolor73: case IDPlt_pcolor74: case IDPlt_pcolor75:
       case IDPlt_pcolor76: case IDPlt_pcolor77: case IDPlt_pcolor78: 
       case IDPlt_pcolor79: case IDPlt_pcolor80: 
       case IDPlt_pcolor81: case IDPlt_pcolor82:
       case IDPlt_pcolor83: case IDPlt_pcolor84: case IDPlt_pcolor85:
       case IDPlt_pcolor86: case IDPlt_pcolor87: case IDPlt_pcolor88: 
       case IDPlt_pcolor89: case IDPlt_pcolor90: 
       case IDPlt_pcolor91: case IDPlt_pcolor92:
       case IDPlt_pcolor93: case IDPlt_pcolor94: case IDPlt_pcolor95:
       case IDPlt_pcolor96: case IDPlt_pcolor97: case IDPlt_pcolor98: 
       case IDPlt_pcolor99: case IDPlt_pcolor100:
       case IDPlt_pcolor101:  case IDPlt_pcolor102:  case IDPlt_pcolor103:
       case IDPlt_pcolor104:  case IDPlt_pcolor105:  case IDPlt_pcolor106:
       case IDPlt_pcolor107:  case IDPlt_pcolor108:  case IDPlt_pcolor109:
       case IDPlt_pcolor110: 
       case IDPlt_pcolor111: case IDPlt_pcolor112: case IDPlt_pcolor113:
       case IDPlt_pcolor114: case IDPlt_pcolor115: case IDPlt_pcolor116: 
       case IDPlt_pcolor117: case IDPlt_pcolor118: case IDPlt_pcolor119:
       case IDPlt_pcolor120: 
       case IDPlt_pcolor121:  case IDPlt_pcolor122:  case IDPlt_pcolor123:
       case IDPlt_pcolor124:  case IDPlt_pcolor125:  case IDPlt_pcolor126:
       case IDPlt_pcolor127:  case IDPlt_pcolor128:  case IDPlt_pcolor129:
       case IDPlt_pcolor130: 
       case IDPlt_pcolor131: case IDPlt_pcolor132:
       case IDPlt_pcolor133: case IDPlt_pcolor134: case IDPlt_pcolor135:
       case IDPlt_pcolor136: case IDPlt_pcolor137: case IDPlt_pcolor138: 
       case IDPlt_pcolor139: case IDPlt_pcolor140: 
       case IDPlt_pcolor141: case IDPlt_pcolor142:
       case IDPlt_pcolor143: case IDPlt_pcolor144: case IDPlt_pcolor145:
       case IDPlt_pcolor146: case IDPlt_pcolor147: case IDPlt_pcolor148: 
       case IDPlt_pcolor149: case IDPlt_pcolor150: 
       case IDPlt_pcolor151: case IDPlt_pcolor152:
       case IDPlt_pcolor153: case IDPlt_pcolor154: case IDPlt_pcolor155:
       case IDPlt_pcolor156: case IDPlt_pcolor157: case IDPlt_pcolor158: 
       case IDPlt_pcolor159: case IDPlt_pcolor160: 
       case IDPlt_pcolor161: case IDPlt_pcolor162:
       case IDPlt_pcolor163: case IDPlt_pcolor164: case IDPlt_pcolor165:
       case IDPlt_pcolor166: case IDPlt_pcolor167: case IDPlt_pcolor168: 
       case IDPlt_pcolor169: case IDPlt_pcolor170: 
       case IDPlt_pcolor171: case IDPlt_pcolor172:
       case IDPlt_pcolor173: case IDPlt_pcolor174: case IDPlt_pcolor175:
       case IDPlt_pcolor176: case IDPlt_pcolor177: case IDPlt_pcolor178: 
       case IDPlt_pcolor179: case IDPlt_pcolor180: 
       case IDPlt_pcolor181: case IDPlt_pcolor182:
       case IDPlt_pcolor183: case IDPlt_pcolor184: case IDPlt_pcolor185:
       case IDPlt_pcolor186: case IDPlt_pcolor187: case IDPlt_pcolor188: 
       case IDPlt_pcolor189: case IDPlt_pcolor190: 
       case IDPlt_pcolor191: case IDPlt_pcolor192:
       case IDPlt_pcolor193: case IDPlt_pcolor194: case IDPlt_pcolor195:
       case IDPlt_pcolor196: case IDPlt_pcolor197: case IDPlt_pcolor198: 
       case IDPlt_pcolor199: case IDPlt_pcolor200:
       case IDPlt_pcolor201:  case IDPlt_pcolor202:  case IDPlt_pcolor203:
       case IDPlt_pcolor204:  case IDPlt_pcolor205:  case IDPlt_pcolor206:
       case IDPlt_pcolor207:  case IDPlt_pcolor208:  case IDPlt_pcolor209:
       case IDPlt_pcolor210: 
       case IDPlt_pcolor211: case IDPlt_pcolor212: case IDPlt_pcolor213:
       case IDPlt_pcolor214: case IDPlt_pcolor215: case IDPlt_pcolor216: 
       case IDPlt_pcolor217: case IDPlt_pcolor218: case IDPlt_pcolor219:
       case IDPlt_pcolor220: 
       case IDPlt_pcolor221:  case IDPlt_pcolor222:  case IDPlt_pcolor223:
       case IDPlt_pcolor224:  case IDPlt_pcolor225:  case IDPlt_pcolor226:
       case IDPlt_pcolor227:  case IDPlt_pcolor228:  case IDPlt_pcolor229:
       case IDPlt_pcolor230: 
       case IDPlt_pcolor231: case IDPlt_pcolor232:
       case IDPlt_pcolor233: case IDPlt_pcolor234: case IDPlt_pcolor235:
       case IDPlt_pcolor236: case IDPlt_pcolor237: case IDPlt_pcolor238: 
       case IDPlt_pcolor239: case IDPlt_pcolor240: 
       case IDPlt_pcolor241: case IDPlt_pcolor242:
       case IDPlt_pcolor243: case IDPlt_pcolor244: case IDPlt_pcolor245:
       case IDPlt_pcolor246: case IDPlt_pcolor247: case IDPlt_pcolor248: 
       case IDPlt_pcolor249: case IDPlt_pcolor250: 
       case IDPlt_pcolor251: case IDPlt_pcolor252:
       case IDPlt_pcolor253: case IDPlt_pcolor254: case IDPlt_pcolor255:
       */
       /*
	  if (EOF == fprintf (stru, "%s=%d %d %d, %d %d %d\t\t%s\n", key_name,
	  	plt_ini_data.tab_pen_color [i - IDPlt_pcolor1].c,
		plt_ini_data.tab_pen_color [i - IDPlt_pcolor1].m,
		plt_ini_data.tab_pen_color [i - IDPlt_pcolor1].y,
      plt_ini_data.tab_pen_color [i - IDPlt_pcolor1].c255,
		plt_ini_data.tab_pen_color [i - IDPlt_pcolor1].m255,
		plt_ini_data.tab_pen_color [i - IDPlt_pcolor1].y255,
		comment))
	    return FALSE;
	  break;
	  */
       default:
	 break;
	  }
   }
   if (EOF == fprintf (stru, "\n"))
     return FALSE;
	return TRUE;
}



BOOL Plt_Draw_Param (char *ptrsz_file_name,
		     T_PTR_Plt_Config *ptrs_config,
		     T_PTR_Plt_Ini_Date *ptrs_ini_date)
//-----------------------------------------------------
{ int i;

  plt_file = ptrsz_file_name ;
  *ptrs_config = &plt_config ;
  *ptrs_ini_date = &plt_ini_data ;

  Get_Private_Profile_Strings ((T_Fstring)PLOTER, get_config_param_from_ini_file);
  
  if ( set_plt_ini_dane () == FALSE)
  {
    return FALSE ;
  }
  
  return TRUE ;
}

#undef __PLTDLG__
