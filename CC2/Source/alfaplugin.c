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

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#ifdef LINUX
#include <ds/list.h>
#else
#include <../ds/list.h>
#endif
#include "alfaplugin_manager.h"
#include "forwin.h"
#include "bib_e.h"
#include "rysuj_e.h"
#include "ds/list_str.h"

#include <stdint.h>
#include "unicode.h"

#ifdef LINUX
#include "../alfaplugin-export/alfaplugin_enum.h"
#else
#include "../alfaplugin-export/alfaplugin_enum.h"
#endif

#define r14 14
#define r22 16 

#ifndef COPY_PUT
#define COPY_PUT 0
#endif
#ifndef XOR_PUT
#define XOR_PUT  1
#endif

extern void redraw(void);
extern int  getwsp1(TMENU *menu);
extern void komunikat(int n);
extern void komunikat0(int n);
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);
extern void PlineW1 (char *blok_type);
extern void set_pline_param (BOOL b_pline);
extern BOOL add_block_ (void) ;
extern BOOL add_trace_ (void) ;
extern BOOL get_start_pline_ (double *X0, double *Y0);
extern void erase_pline_ (void);
extern int Pline_Line (double df_xbeg, double df_ybeg);
extern int Pline_Arc (double df_xbeg, double df_ybeg);
extern void Wymiarowanie_polilinii1(char *adr0, int opcja);
extern BOOL group_block_with_type (long dane_size000, double x, double y, char *blok_type0, char obiektt2, char *prefix, char kod_obiektu);
extern BOOL Check_if_Equal (double x, double y);
extern void rysuj_obiekt (char *ad, int mode, int kolor);
extern char* fillet_line_to_line(double df_r, LINIA* L1, LINIA* L2, BOOL inverted);
extern int load_client_bitmap(char *bitmap_file);
extern int delete_client_bitmap(int iconno);
extern int invoke_custom_comnd(void *c_comnd);
extern int reject_custom_comnd(void);
extern void DF_to_String (char *, char *, double, int) ;
extern int calculator (char *buf, int *retval_no, double *buf_ret);
extern void remove_short_notice(void);
extern int getlanguage(void);
extern BLOK *FIRSTB(char  *ado);
extern char *Get_Object (unsigned int typ, char *kom);
extern char *Get_PTR__GTMPBLOCK(void);
extern void Set_PTR__GTMPBLOCK(char *block);
extern char *Get_PTR__GTMPFIRST(void);
extern void Set_PTR__GTMPFIRST(char *first);
extern char *Get_PTR__GTMPLAST(void);
extern void Set_PTR__GTMPLAST(char *last);
extern void set_komunikat0(int n, char *message);
extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;
extern void moveto(int x, int y);
extern BOOL Cust_Semaphore;
extern void draw_ramka(int opt);
extern void simulate_keypress(int key);
extern void change_angle_l(double angle);
extern double normalize_txt_angle(double angle);
extern void normalize_txt(TEXT *ptrs_text);
extern void get_posXY(double *pozx, double *pozy);
extern void adrem_blok1 (BLOK *adb, int atrybut);
extern int move_block_or_forget(char* adp,char* adk);
extern void Place_Import_Block (int opcja, char *blockfile);
extern void *luk_w_in_block (void  *adr, BOOL draw);
extern void get_dim_round_text(double l, char *text);
//extern int32_t ucs2_to_utf8 (int32_t ucs2, uint8_t * utf8)

extern LINIA LiniaG;
extern LUK LukG;

extern TMENU mInsetDraw;
extern POLE pmInsetDraw;
extern TMENU mInsetEdit;
extern POLE pmInsetEdit;
extern TMENU mInsetBlock;
extern POLE pmInsetBlock;
extern TMENU mInsetAux;
extern POLE pmInsetAux;


extern void add_plugin_function(void *plugin_func, int part);
extern int utf8len(const char *s);
extern int delete_all_client_bitmaps(void);

extern TMENU mRysuj;
extern TMENU mEdycja;
extern TMENU mBlok;

static pm_manager_t *pm        = NULL;
static int           the_num   = 7;
static int           other_num = 19;

list_str_t *plugin_func_name_draw;
list_str_t *plugin_func_name_edit;
list_str_t *plugin_func_name_block;
list_str_t *plugin_func_name_aux;


static ESTR eCustom={0,ESTR_Y,0,-1, 0,r14 ,NULL, NULL, 0, 1, GV_DIST, NULL, NULL, NULL};
static int npCustom;

static void ( *LASTFUN_BAK)(void)=NULL;

static int sortstring( const void *str1, const void *str2 )
{
    char *const *pp1 = str1;
    char *const *pp2 = str2;
    return strcmp(*pp1, *pp2);
}

typedef struct X0Y0 {
    double X0;
    double Y0;
} X0Y0;

void plugin_func_draw(void)
{
    pm_plugin_alfa(pm, list_str_get(plugin_func_name_draw, mInsetDraw.foff+mInsetDraw.poz));
    //cleaning after plugin
    Cust_Semaphore=FALSE;
    if (eCustom.values!=NULL) usunstr(npCustom);
    //delete_all_client_bitmaps();
}

void plugin_func_edit(void)
{
    pm_plugin_alfa(pm, list_str_get(plugin_func_name_edit, mInsetEdit.foff+mInsetEdit.poz));
    //cleaning after plugin
    Cust_Semaphore=FALSE;
    if (eCustom.values!=NULL) usunstr(npCustom);
    //delete_all_client_bitmaps();
}

void plugin_func_block(void)
{
    pm_plugin_alfa(pm, list_str_get(plugin_func_name_block, mInsetBlock.foff+mInsetBlock.poz));
    //cleaning after plugin
    Cust_Semaphore=FALSE;
    if (eCustom.values!=NULL) usunstr(npCustom);
    //delete_all_client_bitmaps();
}

void plugin_func_aux(void)
{
    pm_plugin_alfa(pm, list_str_get(plugin_func_name_aux, mInsetAux.foff+mInsetAux.poz));
    //cleaning after plugin
    Cust_Semaphore=FALSE;
    if (eCustom.values!=NULL) usunstr(npCustom);
    //elete_all_client_bitmaps();
}

int the_num_get(void)
{
    return the_num;
}

int the_num_set(int a)
{
    the_num = a;
    return 1;
}

void out_eCustom(void) {
    if (eCustom.values!=NULL)
        Out_Edited_Draw_Param((ESTR *) &eCustom, TRUE);
}

void *plugin_functions(int plug_function, void *param1, void *param2, void *param3)
{
    static double *p1_;
    static double *p2_;
    static double p1;
    static double p2;

    switch (plug_function) {
        case REDRAW:
            redraw();
            return (int*)1;
            break;
        case GETLANGUAGE:
            return (int*)getlanguage();
            break;
        case GETWSP1:
            return (int*)getwsp1(param1);
            break;
        case NOTICE:
            komunikat((int)param1);
            return 0;
            break;
        case NOTICE_STR:
            komunikat_str((char*)param1);
            return 0;
            break;
        case NOTICE0:
            komunikat0((int)param1);
            return 0;
            break;
        case NOTICE0_STR:
            set_komunikat0(CUSTOM_KOM, param1);
            komunikat0(CUSTOM_KOM);
            return 0;
            break;
        case NOTICE_STR_SHORT:
            komunikat_str_short((char*)param1, TRUE, FALSE);
            return 0;
            break;
        case GET_LINE_G:
            return &LiniaG;
            break;
        case SET_LINE_G_TYPE:
            LiniaG.typ=*(int*)param1;
            return 0;
            break;
        case SET_ARC_G_TYPE:
            LukG.typ=*(int*)param1;
            return 0;
            break;
        case PLINEW:
            PlineW1 ((char*)param1);
            return 0;
            break;
        case SET_PLINE_PARAM:
            set_pline_param (*(int*)param1);
            return 0;
            break;
        case GET_DATA:
            return dane;
            break;
        case GET_DATA_SIZE:
            return (void*)dane_size;
            break;
        case ADD_BLOCK:
            return (BOOL*)add_block_();
            break;
        case ADD_TRACE:
            return (BOOL*)add_trace_();
            break;
        case GET_START_PLINE:
            return (BOOL*)get_start_pline_(param1, param2);
            break;
        case ERASE_PLINE:
            erase_pline_();
            return 0;
            break;
        case PLINE_LINE:
        {
            p1_=(double*)param1;
            p2_=(double*)param2;
            p1=*p1_;
            p2=*p2_;
            return (int*)Pline_Line(p1, p2);
        }
        break;
        case PLINE_ARC:
        {
            p1_=(double*)param1;
            p2_=(double*)param2;
            p1=*p1_;
            p2=*p2_;
            return (int*)Pline_Arc((double)p1, (double)p2);
        }
        break;
        case DIMENSIONING_POLYLINE:
            Wymiarowanie_polilinii1((char*)param1, (int)param2);
            return 0;
            break;
        case GROUP_BLOCK_WITH_TYPE:
            group_block_with_type(*(long*)param1,0,0,param2, O2NoBlockS, "", B_WIRE);
            return 0;
            break;
        case GET_DIM_VARIABLES:
            return &zmwym;
            break;
        case GET_SCALE:
            return &SkalaF;
            break;
        case GET_UNITS:
            return &Jednostki;
            break;
        case GET_UNITS_STR:
            return &st_jedn;
            break;
        case GET_CURRENT_LAYER:
            return &Current_Layer;
            break;
        case CHECK_IF_EQUAL:
            return (BOOL *)Check_if_Equal(*(double*)param1, *(double*)param2);
            break;
        case ADD_OBJECT:
            return dodaj_obiekt(param1, param2);
            break;
        case DRAW_OBJECT:
            rysuj_obiekt((char*)param1, (int)param2, (int)param3);
            return 0;
            break;
        case DRAW_BLOCK:
            blokzap((char*)param1, (char*)param2, (int)param3, COPY_PUT, 1);
            return 0;
        case FILLET_LINE_TO_LINE:
            return (char*)fillet_line_to_line(*(double *) param1, (LINIA *) param2, (LINIA *) param3, FALSE);
            break;
        case GETMENUP:
            getmenupini (&((MENUPSTRUCT *)param1)->menu, &((MENUPSTRUCT *)param1)->st, (unsigned short*)&((MENUPSTRUCT *)param1)->cod, (int*)&((MENUPSTRUCT *)param1)->iconno);
            return 0;
            break;
        case SETMENUP:
              menupini (((MENUPSTRUCT *)param1)->menu, ((MENUPSTRUCT *)param1)->st, (unsigned short)((MENUPSTRUCT *)param1)->cod, (int)((MENUPSTRUCT *)param1)->iconno);
            return 0;
            break;
        case LOAD_CLIENT_BITMAP:
            return (int*)load_client_bitmap(param1);
            break;
        case DELETE_CLIENT_BITMAP:
            return (int*)delete_client_bitmap(*(int*)param1);
            break;
        case INVOKE_CUSTOM_COMND:
            return (int*)invoke_custom_comnd((void*)param1);
            break;
        case REJECT_CUSTOM_COMND:
            return (int*)reject_custom_comnd();
            break;
        case GET_STRING_STR:
            return (int*)get_string_str ((char*)param1, (char*)param2, MaxTextLen, 0, (char*)param3);
            break;
        case DF_TO_STRING:
            DF_to_String ((char *)param1, (char *)param2, *(double *)param3, 16);
            return 0;
            break;
        case CALCULATOR:
            return (int*)calculator ((char *)param1, (int *)param2, (double*) param3);
            break;
        case REMOVE_SHORT_NOTICE:
            remove_short_notice();
            return 0;
        case SELECT_OBJECT: {
            unsigned int typ;
            typ=*(unsigned int*)param1;
            typ=typ<<8;
            return Get_Object(typ, (char *) param2);
        }
            break;
        case SELECT_SINGLE_OBJECT: {
            unsigned int typ;
            int sel_akt; 
            char *ret;
            typ=*(unsigned int*)param1;
            strcpy(global_kom, (char *) param2);
            sel_akt=sel.akt;
            sel.akt=1;
            ret=select_n(&typ, NULL, 255);
            sel.akt=sel_akt;
            return ret;
        }
            break;
        case FIRSTBLOCK:
            return FIRSTB((char*)param1);
            break;
        case SET_PTR__GTMPBLOCK:
            Set_PTR__GTMPBLOCK((char*)param1);
            return 0;
            break;
        case SET_PTR__GTMPFIRST:
            Set_PTR__GTMPFIRST((char*)param1);
            return 0;
            break;
        case SET_PTR__GTMPLAST:
            Set_PTR__GTMPLAST((char*)param1);
            return 0;
            break;
        case GET_PTR__GTMPBLOCK:
            return Get_PTR__GTMPBLOCK();
            break;
        case GET_PTR__GTMPFIRST:
            return Get_PTR__GTMPFIRST();
            break;
        case GET_PTR__GTMPLAST:
            return Get_PTR__GTMPLAST();
            break;
        case MENU_PAR_NEW:
            menu_par_new((char*)param1, (char*)param2);
            return 0;
        case UTF8LEN:
            return (int*) utf8len((char*)param1);
            break;
        case ADD_EDIT_DOUBLE_STR:
            {
                eCustom.x = maxX-PL266; 
                eCustom.lmax = r14;
                eCustom.y = ESTR_Y;
                eCustom.val_no_max = 1;
                eCustom.mode = GV_DIST;
                eCustom.format = "%-10.8lg";
                eCustom.ESTRF = (char*)param1;
                eCustom.extend = 0;
                if (-1 != (npCustom = dodajstr(&eCustom)) && (EOF != sprintf(eCustom.st, "%10.8lg", *(double *) param2)))
                {
                    eCustom.val_no = 1;
                Out_Edited_Draw_Param ((ESTR *)&eCustom, TRUE) ;
                return (int*)npCustom;
                }
                else return (int*)-1;
            }
            break;
        case ADD_EDIT_DOUBLE_DOUBLE_STR:
            {
                eCustom.x = maxX-PL266;
                eCustom.lmax = r22; //r14;
                eCustom.y = ESTR_Y;
                eCustom.val_no_max = 2;
                eCustom.mode = GV_VECTOR ;
                eCustom.format = "%#12.9lg\0%#12.9lg;%#8.3lf" ;
                eCustom.ESTRF = (char*)param1;
                eCustom.extend = 0;
                if (-1 != (npCustom = dodajstr(&eCustom)) && (EOF != sprintf(eCustom.st, "%#12.9lg;%#8.3lf", *(double *) param2, *(double *) param3)))
                {
                    eCustom.val_no = 2;
                    Out_Edited_Draw_Param ((ESTR *)&eCustom, TRUE) ;
                    return &npCustom;
                }
                else return (int*)-1;
            }
            break;
        case OUT_EDIT_STR:
             Out_Edited_Draw_Param ((ESTR *)&eCustom, TRUE) ;
             return 0;
             break;
        case UPDATE_EDIT_STR: {
            if (eCustom.values!=NULL)
            {
                if (eCustom.val_no == 1) {
                    eCustom.values[0] = *(double *) param1;
                    sprintf(eCustom.st, "%10.8lg", *(double *) param1);
                } else {
                    eCustom.values[0] = *(double *) param1;
                    eCustom.values[1] = *(double *) param2;
                    sprintf(eCustom.st, "%#12.9lg;%#8.3lf", *(double *) param1, *(double *) param2);
                }
                Out_Edited_Draw_Param((ESTR *) &eCustom, TRUE);
            }
            return 0;
        }
        break;
        case REMOVE_EDIT_STR:
            usunstr(*(int*)param1);
            return 0;
            break;
        case ADD_EDIT_VAL_NO:
            return (int*)eCustom.val_no;
            break;
        case ADD_EDIT_DOUBLE_VALUES:
            {
                p1=eCustom.values[(int) param1];
                return &p1;
            }
            break;
        case ERRLIST:
            ErrList ((int)param1);
            return 0;
            break;
        case ERRLISTSTR:
            ErrListStr ((char*)param1);
            return 0;
            break;
        case MM2UNITS:
            p1_=(double*)param1;
            p1=*p1_;
            p2 = jednostkiOb(p1) ;
            return &p2;
            break;
        case CORRECT_OBJECT:
            return (char*)korekta_obiekt((char*)param1, (char*)param2);
            break;
        case DRAWBLOCK:
            blokzap((char *)param1, (char*)param1 + ((NAGLOWEK *)param1)->n, Anormalny, COPY_PUT, (int)param2) ;
            return 0;
            break;
        case FLIPSCREEN:
            flip_screen();
            return 0;
            break;
        case SETBACKSPACEKEY:
            LASTFUN_BAK=LASTFUN;
            LASTFUN=(char*)param1;
            Cust_Semaphore=TRUE;
            return 0;
            break;
        case RESTOREBACKSPACEKEY:
            LASTFUN=LASTFUN_BAK;
            Cust_Semaphore=FALSE;
            return 0;
            break;
        case LINEPARAMETERS:
            parametry_lini ((char *)param1, (PLINIA *)param2) ;
            return 0;
            break;
        case SET_LCSANGLE:
        {
            change_angle_l(*(double *) param1);
            return (int*)1;
        }
            break;
        case GET_LCSANGLE:
            *(double*)param1=get_angle_l();
            return (int*)1;
            break;
        case SIMULATE_KEYPRESSED:
            simulate_keypress(*(int*)param1);
            return 0;
            break;
        case NORMALIZE_TXT_ANGLE:
            p1 = normalize_txt_angle(*(double*)param1);
            return &p1;
            break;
        case NORMALIZE_TXT:
            normalize_txt((TEXT*)param1);
            return 0;
            break;
        case SET_ATTRIBUTE:
            ((NAGLOWEK*)param1)->atrybut=*(int*)param2;
            return 0;
            break;
        case RESET_ATTRIBUTES:
            zmien_atrybut((char*)param1,(char*)param2,Ablok,Anormalny);
            return 0;
            break;
        case BLOKZAP0:
            blokzap((char*)param1,(char*)param2, *(int*)param3, COPY_PUT, 0);
            return 0;
            break;
        case BLOKZAP1:
            blokzap((char*)param1,(char*)param2, *(int*)param3, COPY_PUT, 1);
            return 0;
            break;
        case GET_POSXY:
            get_posXY((double *)param1, (double *)param2);
            return 0;
            break;
        case ADREM_BLOK1:
            adrem_blok1 ((BLOK *)param1, *(int*)param2);
            return 0;
            break;
        case PLACE_BLOCK:
            //return (int*)move_block_or_forget((char*)param1,(char*)param2);
            Place_Import_Block(5, (char *)param1);
            return 0;
            break;
        case DELETE_BLOCK:
            zmien_atrybut((char*)param1,(char*)param2,Ablok,Ausuniety);
            usun_blok((char*)param1,(char*)param2);
            return 0;
            break;
        case LUK_W_IN_BLOCK:
            WymInterF(1);
            return luk_w_in_block((char*)param1, *(int*)param2);
            //return 0;
            break;
        case GET_DIM_ROUND_TEXT:
            get_dim_round_text(*(double*)param1, param2);
            return 0;
            break;
        case GET_DIM_PRECISION:
            return &zmwym.dokladnosc;
            break;
        case SET_DIM_PRECISION:
            zmwym.dokladnosc=*(double*)param1;
            return 0;
        default:
            return 0;
            break;
    }
}

void set_wcod(unsigned short *wcod, char *name)
{
    const uint8_t *next;

    if ((unsigned char)name[0]<127)
        *wcod=(unsigned char)name[0];
    else
    {
        *wcod = utf8_to_ucs2 ((uint8_t*)name, &next);
    }
}

int alfaplugins(void)
{
    char *name;
    int icon;
    Plugs_Parts *plugs_parts;
    char       *out;
    size_t      len;
    size_t      i;
    char *plug_ptr;
    int id=0, ie=0, ib=0;
    BOOL ret;

    char part_name[][8]={"UNKNOWN", "DRAW", "EDIT", "BLOCK", "AUX"};

    plugin_func_name_draw = list_str_create(LIST_STR_NONE);
    plugin_func_name_edit = list_str_create(LIST_STR_NONE);
    plugin_func_name_block = list_str_create(LIST_STR_NONE);
    plugin_func_name_aux = list_str_create(LIST_STR_NONE);

    plug_ptr=&plugin_functions;

    pm = pm_load(plug_ptr);
    
    plugs_parts = pm_plugins_alfa(pm);

    len = list_str_len(plugs_parts->plugs);

    printf("Alfa Plugins (%zu):\n", len);
    for (i=0; i<len; i++) {
        name = list_str_get(plugs_parts->plugs, i);
        icon=plugs_parts->icons[i];
        printf("PLUGIN: %s   %s\n", name, part_name[plugs_parts->parts[i]]);
        switch (plugs_parts->parts[i])
        {
            case PLUGIN_PART_UNKNOWN:
                break;
            case PLUGIN_PART_DRAW:
                strcpy((*mInsetDraw.pola)[mInsetDraw.max].txt,name);
                //(*mInsetDraw.pola)[mInsetDraw.max].wcod=name[0];
                set_wcod(&(*mInsetDraw.pola)[mInsetDraw.max].wcod,name);
                (*mInsetDraw.pola)[mInsetDraw.max].iconno=icon;
                (*mInsetDraw.pola)[mInsetDraw.max].menu=NULL;
                if ((utf8len(name)+1)>mInsetDraw.xdl) mInsetDraw.xdl=(utf8len(name)+1);
                ret=list_str_append(plugin_func_name_draw, name);
                add_plugin_function(plugin_func_draw, PLUGIN_PART_DRAW);
                mInsetDraw.max++;

                mRysuj.max=MRYSUJMAX+1;
                break;
            case PLUGIN_PART_EDIT:
                strcpy((*mInsetEdit.pola)[mInsetEdit.max].txt,name);
                //(*mInsetEdit.pola)[mInsetEdit.max].wcod=name[0];
                set_wcod(&(*mInsetEdit.pola)[mInsetEdit.max].wcod,name);
                (*mInsetEdit.pola)[mInsetEdit.max].iconno=icon;
                (*mInsetEdit.pola)[mInsetEdit.max].menu=NULL;
                if ((utf8len(name)+1)>mInsetEdit.xdl) mInsetEdit.xdl=(utf8len(name)+1);
                ret=list_str_append(plugin_func_name_edit, name);
                add_plugin_function(plugin_func_edit, PLUGIN_PART_EDIT);
                mInsetEdit.max++;

                mEdycja.max=MEDITMAX+1;
                break;
            case PLUGIN_PART_BLOCK:
                strcpy((*mInsetBlock.pola)[mInsetBlock.max].txt,name);
                //(*mInsetBlock.pola)[mInsetBlock.max].wcod=name[0];
                set_wcod(&(*mInsetBlock.pola)[mInsetBlock.max].wcod,name);
                (*mInsetBlock.pola)[mInsetBlock.max].iconno=icon;
                (*mInsetBlock.pola)[mInsetBlock.max].menu=NULL;
                if ((utf8len(name)+1)>mInsetBlock.xdl) mInsetBlock.xdl=(utf8len(name)+1);
                ret=list_str_append(plugin_func_name_block, name);
                add_plugin_function(plugin_func_block, PLUGIN_PART_BLOCK);
                mInsetBlock.max++;

                mBlok.max=MBLOCKMAX+1;
                break;
            case PLUGIN_PART_AUX:
                strcpy((*mInsetAux.pola)[mInsetAux.max].txt,name);
                //(*mInsetAux.pola)[mInsetAux.max].wcod=name[0];
                set_wcod(&(*mInsetAux.pola)[mInsetAux.max].wcod,name);
                (*mInsetAux.pola)[mInsetAux.max].iconno=icon;
                (*mInsetAux.pola)[mInsetAux.max].menu=NULL;
                if ((utf8len(name)+1)>mInsetAux.xdl) mInsetAux.xdl=(utf8len(name)+1);
                ret=list_str_append(plugin_func_name_aux, name);
                add_plugin_function(plugin_func_aux, PLUGIN_PART_AUX);
                mInsetAux.max++;
                break;
            default:
                break;
        }

    }
    list_str_destroy(plugs_parts->plugs);

    return 0;
}

int alfaplugins_off(void)
{
    pm_unload(pm);
    list_str_destroy(plugin_func_name_draw);
    list_str_destroy(plugin_func_name_edit);
    list_str_destroy(plugin_func_name_block);
    list_str_destroy(plugin_func_name_aux);
    delete_all_client_bitmaps();
    return 0;
}
