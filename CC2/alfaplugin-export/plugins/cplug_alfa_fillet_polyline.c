#include "../alfaplugin_sys.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "alfaplugin.h"
#include "../alfaplugin_enum.h"
#include "../bib_e_part.h"

#define COPY_PUT 0
#define XOR_PUT  1
#define OR_PUT   2

char filletpolyline[64];
double radius = 0.0;  //[units]

static int language=-1;
static int iconno_plugin=0;
double SkalaF, Jednostki;
char Units[6];
static BLOK *block_bak=NULL;

BOOL edit_str_exists=FALSE;

void *(*plugin_ptr)(int plugin_function, char *param1, char *param2, char *param3);

static POLE pmFPolylineEN[] =
        {
                { u8"Bending radius\0", L'B', 0, NULL },
                { u8"Undo (Backspace)\0", L'U', 0, NULL }
        };

static POLE pmFPolylinePL[] =
        {
                { u8"Promień gięcia\0", L'P', 0, NULL },
                { u8"Cofnij (Backspace)\0", L'C', 0, NULL }
        };

static POLE pmFPolylineUA[] =
        {
                { u8"Радіус вигину\0", L'Р', 0, NULL },
                { u8"Скасувати (Backspace)\0", L'С', 0, NULL }
        };

static POLE pmFPolylineES[] =
        {
                { u8"Radio de curvatura\0", L'R', 0, NULL },
                { u8"Deshacer (Backspace)\0", L'D', 0, NULL }
        };

static POLE *pmFPolyline_L[]={pmFPolylineEN, pmFPolylinePL, pmFPolylineUA, pmFPolylineES};

static TMENU mFPolyline={2,0,0,14,1,1,TADD | ICONS,CMNU,CMBR,CMTX,0,CUSTOM_COMND_OFF,0,0,0,&pmFPolylineEN,NULL,NULL};

union void_cast {
    void* ptr;
    int value;
    long long_value;
};

int VOID_TO_INT(void* ptr) {
    union void_cast u;
    u.ptr = ptr;
    return u.value;
}

int VOID_TO_LONG(void* ptr) {
    union void_cast u;
    u.ptr = ptr;
    return u.long_value;
}

plugin_type_t plugin_type(void)
{
    return PLUGIN_TYPE_ALFA;
}

plugin_part_t plugin_part(void)
{
    return PLUGIN_PART_EDIT;
}

bool init(char *plug_ptr)
{
    plugin_ptr=plug_ptr;
    language = VOID_TO_INT(plugin_ptr(GETLANGUAGE, NULL, NULL, NULL));
    return true;
}

void deinit(void)
{
    //char goodbye[32]= u8"Bye bye\n";
    //int ret = VOID_TO_INT(plugin_ptr(NOTICE_STR_SHORT, (char*)goodbye, NULL, NULL));
    int ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, &iconno_plugin, NULL, NULL));
    return;
}

const char *plugin_name(void)
{
    switch (language)
    {
        case ENGLISH:
        return u8"Fillet polyline (plugin)";
        break;
        case POLISH:
        return u8"Zaokrąglij polilinię (wtyczka)";
        break;
        case UKRAINIAN:
        return u8"Скругленни полілінію (підключати)";
        break;
        case SPANISH:
        return u8"Filete de polilínea (enchufar)";
        break;
        default:
        return u8"Fillet polyline (plugin)";
        break;
    }
}

int plugin_icon(void)
{
    iconno_plugin=VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "fillet_polyline.png", NULL, NULL));
    return iconno_plugin;
}

int plugin_version(void)
{
    return 1;
}

int plugin_api_version(void)
{
    return 2;
}

static void fillet_undo(void)
{   char *block_cur;
    char *block_pos;
    int ret;

    if (block_bak!=NULL)
    {
        block_pos=(char*)plugin_ptr(GET_PTR__GTMPBLOCK, NULL, NULL, NULL);
        ret= VOID_TO_INT(plugin_ptr(DRAWBLOCK, (char*)block_pos, (int*)0, NULL));

        block_cur = (char *) (plugin_ptr(CORRECT_OBJECT, (char*)block_pos, (char*)block_bak, NULL));
        ret= VOID_TO_INT(plugin_ptr(DRAWBLOCK, (char*)block_cur, (int*)1, NULL));

        ret= VOID_TO_INT(plugin_ptr(FLIPSCREEN, NULL, NULL, NULL));
    }

}

static void update_radius_in_menu(void) {
    char st1[60];
    char r_str[16];
    int ret;

    sprintf(st1,u8"%s   R=%g",filletpolyline, radius);
    ret = VOID_TO_INT(plugin_ptr(NOTICE_STR_SHORT, st1, NULL, NULL));

    if (edit_str_exists==TRUE)  ret = VOID_TO_INT(plugin_ptr(UPDATE_EDIT_STR, (double*)&radius, NULL, NULL));

    sprintf(r_str, u8"%g", radius);
    mFPolyline.xdl = VOID_TO_INT(plugin_ptr(UTF8LEN, (char *) ((*mFPolyline.pola)[0].txt), NULL, NULL)) + 8;
    ret = VOID_TO_INT(plugin_ptr(MENU_PAR_NEW, (char *) ((*mFPolyline.pola)[0].txt), r_str, NULL));
}

static void fillet_radius(void)
{
    char radius_str[16];
    char prompt[64];
    int ret;
    int retval_no = 1;
    double buf_ret[1];

    switch (language)
    {
        case ENGLISH:
            sprintf(prompt,u8"bending radius %s: ", Units);
        break;
        case POLISH:
            sprintf(prompt,u8"promień gięcia %s: ", Units);
        break;
        case UKRAINIAN:
            sprintf(prompt,u8"радіус вигину %s: ", Units);
        break;
        case SPANISH:
            sprintf(prompt,u8"radio de curvatura %s: ", Units);
        break;
        default:
            sprintf(prompt,u8"bending radius %s: ", Units);
        break;
    }

    ret = VOID_TO_INT(plugin_ptr(DF_TO_STRING, (char*)radius_str, "%-7.3f", (double *)&radius));
    ret = VOID_TO_INT(plugin_ptr(GET_STRING_STR, (char*)&radius_str, "", (char*)prompt));

    ret = VOID_TO_INT(plugin_ptr(CALCULATOR, (char*)radius_str, (int*)&retval_no, (double*)&buf_ret));

    if ((ret==FALSE) || (retval_no < 1))
    {
        return ;
    }
    radius = buf_ret [0] ;

    if (radius==0) return;

    update_radius_in_menu();
}

static int edit_radius_value (BOOL b_graph_value)
/*---------------------------------------------*/
{
    double df_edit_val ;
    int val_no;
    int ret;

    b_graph_value = b_graph_value ;
    val_no=VOID_TO_INT(plugin_ptr(ADD_EDIT_VAL_NO, NULL, NULL, NULL));
    if (val_no < 1)
    {
        return 0 ;
    }
    df_edit_val=*(double*)plugin_ptr(ADD_EDIT_DOUBLE_VALUES, (int*)0, NULL, NULL);

    if (df_edit_val > 0)
    {
        //radius = *(double*)plugin_ptr(MM2UNITS, &df_edit_val, NULL, NULL);
        radius=df_edit_val;

        update_radius_in_menu();

        return 1 ;
    }
    else
    {
        ret=VOID_TO_INT(plugin_ptr(ERRLIST, (int*)30, NULL, NULL)); //ErrList (30) ;
        return 0 ;
    }
}

void COMND_MANAGER(int comnd) {
    if (comnd==0) fillet_radius();
    else if (comnd==1) fillet_undo();
}

void fill_polyline(char *dane000, double radius)
{  BLOK* block_pl;
    LINIA* line1, *line2;
    char *adr0, *adr, *adrk, *adr1;
    int e_no,l_no;
    BOOL closed=FALSE;
    double L_x1, L_y1, L_x2, L_y2;
    BOOL first_line=FALSE;
    BOOL last_line=FALSE;
    BOOL line=FALSE;
    char *first, *last;
    int ret;

    block_pl = (BLOK *) dane000;
    adrk=(char *) block_pl + sizeof(NAGLOWEK) + block_pl->n - 1;
    adr0=(char *) block_pl + sizeof(NAGLOWEK) + B3 + block_pl->dlugosc_opisu_obiektu;

    adr=adr0; //starting from first object in polyline block

    //counting lines and checking if closed polyline
    l_no=0;
    e_no=0;
    NAGLOWEK *nag;
    while (1)
    {
        nag = (NAGLOWEK *) adr;
        if ((nag>adrk) || (nag->obiekt==Okoniec)) break;
        if (nag->obiekt==Olinia) {
            line=TRUE;
            if (l_no==0)
            {
                L_x1=(double)((LINIA*)nag)->x1;
                L_y1=(double)((LINIA*)nag)->y1;
                first=nag;
            }
            else
            {
                L_x2=(double)((LINIA*)nag)->x2;
                L_y2=(double)((LINIA*)nag)->y2;
                last=nag;
            }
            l_no++;
            if (e_no==0) first_line=TRUE;
        }
        adr+=(sizeof(NAGLOWEK)+((NAGLOWEK*)adr)->n);
        e_no++;
    }
    if (line) last_line=TRUE;
    if ((l_no>2) && (first_line) && (last_line))
    {
        if ((VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, &L_x2, &L_x1, NULL))) &&
        (VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, &L_y2, &L_y1, NULL))))
        {
            closed = TRUE;
            //setting globals
            ret=VOID_TO_INT(plugin_ptr(SET_PTR__GTMPFIRST, first, NULL, NULL));
            ret=VOID_TO_INT(plugin_ptr(SET_PTR__GTMPLAST, last, NULL, NULL));
        }
    }

    adr=adr0;  //going back to the first object

    if (l_no>1)
    {
        int i_no=0;
        while (1) {
            if (i_no == l_no) break;
            line1 = (LINIA *) adr;
            adr += (sizeof(NAGLOWEK) + ((NAGLOWEK *) adr)->n);
            if (line1->obiekt == Olinia) i_no++;
            else continue;

            if (i_no == l_no) break;

            line2 = (LINIA *) adr;
            if (line2->obiekt == Olinia) i_no++;
            else
            {
                adr += (sizeof(NAGLOWEK) + ((NAGLOWEK *) adr)->n);
                continue;
            }
            adr1 = (char *) (plugin_ptr(FILLET_LINE_TO_LINE, &radius, line1, line2));
            if (adr1!=NULL) adr=adr1;
            i_no--;

            if (i_no >= (l_no-1)) break;  //there is just one line left, so no filling possible
            adr += (sizeof(NAGLOWEK) + ((NAGLOWEK *) adr)->n);
        }
        if (closed)
        {  //taking first and last line
            first=(char*)plugin_ptr(GET_PTR__GTMPFIRST, NULL, NULL, NULL);
            last=(char*)plugin_ptr(GET_PTR__GTMPLAST, NULL, NULL, NULL);
            adr1 = (char *) (plugin_ptr(FILLET_LINE_TO_LINE, &radius, last, first));
        }
    }
}

void alfa_func(int a) {
    int ret, ret1;
    char kom[64];
    char *obj_ptr;
    char *block_ptr;
    double radius_sk;
    int iconno_aux, iconno_radius, iconno_undo;
    unsigned short filletpolyline_c;
    unsigned int typ;
    int npCustom;
    long_long block_size;
    static MENUPSTRUCT menup={NULL, "", 32, 0};
    static MENUPSTRUCT menup_plugin={NULL, "", 32, 0};
    MENUPSTRUCT *menup_p;
    MENUPSTRUCT *menup_plugin_p;

    SkalaF = *(double*)plugin_ptr(GET_SCALE, NULL, NULL, NULL);
    Jednostki = *(double*)plugin_ptr(GET_UNITS, NULL, NULL, NULL);
    strcpy(&Units,(char*)plugin_ptr(GET_UNITS_STR, NULL, NULL, NULL));

    switch (language) {
        case ENGLISH:
            strcpy(filletpolyline, u8"Fillet polyline");
            strcpy(kom, u8"Select a polyline");
            filletpolyline_c=L'F';
            break;
        case POLISH:
            strcpy(filletpolyline, u8"Zaokrąglij polilinię");
            strcpy(kom, u8"Wskaż polilinię");
            filletpolyline_c=L'Z';
            break;
        case UKRAINIAN:
            strcpy(filletpolyline, u8"Скругленни полілінію");
            strcpy(kom, u8"Виберіть полілінію");
            filletpolyline_c=L'С';
            break;
        case SPANISH:
            strcpy(filletpolyline, u8"Filete de polilínea");
            strcpy(kom, u8"Seleccione una polilínea");
            filletpolyline_c=L'F';
            break;
        default:
            strcpy(filletpolyline, u8"Fillet polyline");
            strcpy(kom, u8"Select a polyline");
            filletpolyline_c=L'F';
            break;
    }

    mFPolyline.pola=pmFPolyline_L[language];

    ret = VOID_TO_INT(plugin_ptr(NOTICE, 0, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(NOTICE0, 0, NULL, NULL));

    if (radius==0)
    {
        fillet_radius();
        if (radius==0) return;
    }
    else {
        update_radius_in_menu();
    }

    menup_p=&menup;
    menup_plugin_p=&menup_plugin;

    //saving aux menu

    ret = VOID_TO_INT(plugin_ptr(GETMENUP, (char*)menup_p, NULL, NULL));
    iconno_aux = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, u8"fillet_polyline.png", NULL, NULL));
    iconno_radius = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, u8"dim_arc_radial.png", NULL, NULL));
    iconno_undo = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, u8"return.png", NULL, NULL));

    menup_plugin.menu=&mFPolyline;
    strcpy(menup_plugin.st, filletpolyline);
    menup_plugin.cod=filletpolyline_c;
    menup_plugin.iconno=iconno_aux;

    (*mFPolyline.pola)[0].iconno=iconno_radius;
    (*mFPolyline.pola)[1].iconno=iconno_undo;


    ret = VOID_TO_INT(plugin_ptr(SETMENUP, (char*)menup_plugin_p, NULL, NULL));

    ret = VOID_TO_INT(plugin_ptr(INVOKE_CUSTOM_COMND, (char*)COMND_MANAGER, NULL, NULL));

    npCustom=VOID_TO_INT(plugin_ptr(ADD_EDIT_DOUBLE_STR, edit_radius_value, (double*)&radius, NULL));

    if (npCustom>-1) edit_str_exists=TRUE;

    //ret=VOID_TO_INT(plugin_ptr(OUT_EDIT_STR, NULL, NULL, NULL));

    typ=(Blinia | Bluk);

    ret=VOID_TO_INT(plugin_ptr(SETBACKSPACEKEY, (char*)fillet_undo, NULL, NULL));

    while (1)
    {
        obj_ptr=plugin_ptr(SELECT_OBJECT, (unsigned int*)&typ, kom, NULL);
        if (obj_ptr ==NULL) break;

        ret=VOID_TO_INT(plugin_ptr(OUT_EDIT_STR, NULL, NULL, NULL));

        if (((NAGLOWEK*)obj_ptr)->obiekt==OdBLOK) block_ptr=obj_ptr;
        else block_ptr=plugin_ptr(FIRSTBLOCK, obj_ptr, NULL, NULL);
        if (block_ptr!=NULL)
        {

            if (((BLOK*)block_ptr)->kod_obiektu==B_PLINE) {
                radius_sk = radius * Jednostki / SkalaF;

                //freeing previous block_bak
                if (block_bak!=NULL)
                {
                    free(block_bak);
                    block_bak=NULL;
                }
                //making the copy of entire polyline block
                ret=VOID_TO_INT(plugin_ptr(SET_PTR__GTMPBLOCK, block_ptr, NULL, NULL));
                block_size=sizeof(NAGLOWEK)+((BLOK*)block_ptr)->n;
                block_bak=(char*)malloc(block_size);
                memmove(block_bak, block_ptr, block_size);

                if (radius > 0.0) fill_polyline(block_ptr, radius_sk);
            }
        }
    }

    ret=VOID_TO_INT(plugin_ptr(RESTOREBACKSPACEKEY, NULL, NULL, NULL));
    ret=VOID_TO_INT(plugin_ptr(REMOVE_EDIT_STR, &npCustom, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(REMOVE_SHORT_NOTICE, NULL, NULL, NULL));
    ret1 = VOID_TO_INT(plugin_ptr(REJECT_CUSTOM_COMND, NULL, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(SETMENUP, (char*)menup_p, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, &iconno_aux, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, &iconno_radius, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, &iconno_undo, NULL, NULL));

    //freeing last block_bak
    if (block_bak!=NULL)
    {
        free(block_bak);
        block_bak=NULL;
    }

    return;
}
