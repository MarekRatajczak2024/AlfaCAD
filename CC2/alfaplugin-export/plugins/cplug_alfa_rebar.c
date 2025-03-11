#include "../alfaplugin_sys.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "alfaplugin.h"
#include "../alfaplugin_enum.h"
#include "../bib_e_part.h"

#define COPY_PUT 0
#define XOR_PUT  1
#define OR_PUT   2

char main_reinforcement[30];
char reinforcement[64];
double radius = 0.0;  //[units]
static int language=-1;
static int iconno_plugin=0;
double SkalaF, Jednostki;
char Units[6];
void *(*plugin_ptr)(int plugin_function, char *param1, char *param2, char *param3);

static POLE pmNets0[]={
        {u8"Ø6",' ',0,NULL},
        {u8"Ø8",' ',0,NULL},
        {u8"Ø10",' ',0,NULL},
        {u8"Ø12",' ',0,NULL},
        {u8"Ø14",' ',0,NULL},
        {u8"Ø16",' ',0,NULL},
        {u8"Ø18",' ',0,NULL},
        {u8"Ø20",' ',0,NULL},
        {u8"Ø24",' ',0,NULL},
        {u8"Ø25",' ',0,NULL},
        {u8"Ø28",' ',0,NULL},
        {u8"Ø32",' ',0,NULL},
        {u8"Ø36",' ',0,NULL},
        {u8"Ø40",' ',0,NULL},
        {u8"Ø50",' ',0,NULL},
        {u8"#2",' ',0,NULL},
        {u8"#3",' ',0,NULL},
        {u8"#4",' ',0,NULL},
        {u8"#5",' ',0,NULL},
        {u8"#6",' ',0,NULL},
        {u8"#7",' ',0,NULL},
        {u8"#8",' ',0,NULL},
        {u8"#9",' ',0,NULL},
        {u8"#10",' ',0,NULL},
        {u8"#11",' ',0,NULL},
        {u8"#14",' ',0,NULL},
        {u8"#18",' ',0,NULL},
        {u8"#20",' ',0,NULL},
        {u8"#24",' ',0,NULL},
        {u8"#28",' ',0,NULL},
};

static TMENU mNets0={30,0,0,6,1,3,0,CMNU,CMBR,CMTX,0,0,0,0,0,&pmNets0,NULL,NULL};

static POLE pmRebarEN[] =
        {
                { u8"Diameter\0", L'D', 0, &mNets0 },
                { u8"Bending radius \0", L'B', 0, NULL }
        };

static POLE pmRebarPL[] =
        {
                { u8"Średnica\0", L'S', 0, &mNets0 },
                { u8"Promień gięcia \0", L'P', 0, NULL }
        };

static POLE pmRebarUA[] =
        {
                { u8"Діаметр\0", L'Д', 0, &mNets0 },
                { u8"Радіус вигину \0", L'Р', 0, NULL }
        };

static POLE pmRebarES[] =
        {
                { u8"Diámetro\0", L'D', 0, &mNets0 },
                { u8"Radio de curvatura \0", L'R', 0, NULL }
        };


static POLE *pmRebar_L[]={pmRebarEN, pmRebarPL, pmRebarUA, pmRebarES};


static TMENU mRebar={2,0,0,14,1,3,TADD | ICONS,CMNU,CMBR,CMTX,0,CUSTOM_COMND_OFF,0,0,0,&pmRebarEN,NULL,NULL};

typedef struct X0Y0 {
    double X0;
    double *Y0;
} X0Y0;

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

plugin_type_t plugin_type(void)
{
    return PLUGIN_TYPE_ALFA;
}

plugin_part_t plugin_part(void)
{
    return PLUGIN_PART_DRAW;
}

bool init(char *plug_ptr)
{
    plugin_ptr=(void*)plug_ptr;
    language = VOID_TO_INT(plugin_ptr(GETLANGUAGE, NULL, NULL, NULL));
    return true;
}

void deinit(void)
{
    //char goodbye[32]= "Bye bye\n";
    //int ret = VOID_TO_INT(plugin_ptr(NOTICE_STR_SHORT, (char*)goodbye, NULL, NULL));
    int ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_plugin, NULL, NULL));
    return;
}

const char *plugin_name(void)
{
    switch (language)
    {
        case ENGLISH:
        return u8"Reinforcing bar (plugin)";
        break;
        case POLISH:
        return u8"Pręt zbrojeniowy (wtyczka)";
        break;
        case UKRAINIAN:
        return u8"Арматурний прут (плагін)";
        break;
        case SPANISH:
        return u8"Barra de refuerzo (enchufar)";
        break;
        default:
        return u8"Reinforcing bar (plugin)";
        break;
    }
}

int plugin_icon(void)
{
    iconno_plugin=VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "reinforcing.png", NULL, NULL));
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

void finish_pline(unsigned int Ltype) {
    int ret;

    ret = VOID_TO_INT(plugin_ptr(SET_LINE_G_TYPE, (char *) &Ltype, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(SET_ARC_G_TYPE, (char *) &Ltype, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(NOTICE, 0, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(REMOVE_SHORT_NOTICE, NULL, NULL, NULL));
}

#define max_new_texts 31

void Dimensioning_polyline(char *adr0, int opcja)
/*----------------------------------------------*/
{ BLOK *ptrs_block, *ptrs_block1;
    char *adp, *adk;
    NAGLOWEK *nag;
    LINIA *L;
    LUK *l;
    int new_texts;
    TEXT Tn[32];
    TEXT Tn0=Tdef, Tn1=Tdef;
    char Tl[MaxTextLen];
    double max_length, c_x, c_y, c_angle;
    double poliline_length;
    double len_pline;
    double line_angle;
    int i;
    double del_kat;
    double l_x1,l_y1,l_x2,l_y2;
    double dh_text;     //0.5 [mm]
    double dh1_text;    //3.0 [mm]
    int ret;
    double precision, precision1;

    ZMIENNE *zmwym;
    int Current_Layer;
    char *dane;
    char *adr;
    double L_x1, L_x2;

    zmwym = (ZMIENNE*)(plugin_ptr(GET_DIM_VARIABLES, NULL, NULL, NULL));

    precision=zmwym->dokladnosc;

    if (Jednostki==1) precision1=1;
    else if (Jednostki==10) precision1=0.1;
    else if (Jednostki==1000) precision1=0.001;
    else if (Jednostki==1000000) precision1=0.000001;
    else if (Jednostki==25.4) precision1=0.05; //"
    else if (Jednostki==304.8) precision1=0.0025; //'
    else if (Jednostki==914.4) precision1=0.001;//yd
    else if (Jednostki==1609344) precision1=0.0000005; //mi

    ret=VOID_TO_INT(plugin_ptr(SET_DIM_PRECISION, (char*)&precision1, NULL, NULL));

    Current_Layer=*(int*)(plugin_ptr(GET_CURRENT_LAYER, NULL, NULL, NULL));

    dh_text=0.75;
    dh1_text=zmwym->wysokosc+1.25;

    Tn0.kolor=zmwym->Tkolor;
    Tn0.wysokosc=zmwym->wysokosc;
    Tn0.width_factor=zmwym->width_factor;
    Tn0.czcionka=zmwym->czcionka;
    Tn0.bold=zmwym->bold;
    Tn0.italics=zmwym->italics;

    new_texts=0;
    len_pline=0;
    poliline_length=0;

    ptrs_block = (BLOK*)adr0;
    adp=adr0+sizeof(NAGLOWEK)+B3+ptrs_block->dlugosc_opisu_obiektu;
    adk=adr0+sizeof(NAGLOWEK)+ptrs_block->n-1;
    nag = (NAGLOWEK*)adp;

    while ((nag->obiekt != Okoniec) && (adp < adk))
    {
        if (nag->obiekt == OdBLOK)
        {
            ptrs_block1 = (BLOK*)adp;
            adp+=(sizeof(NAGLOWEK)+B3+ptrs_block1->dlugosc_opisu_obiektu);
        }
        else if (nag->obiekt == Olinia)
        {
            L = (LINIA*)adp;
            //dimensioning of polyline section;
            if (new_texts<max_new_texts)
            {
                new_texts++;
                memmove(&Tn[new_texts-1], &Tn0, sizeof(TEXT));
                Tn[new_texts-1].x=(L->x1+L->x2)/2;
                Tn[new_texts-1].y=(L->y1+L->y2)/2;
                //length calculation
                len_pline=sqrt(((L->x2-L->x1)*(L->x2-L->x1))+((L->y2-L->y1)*(L->y2-L->y1)));
                len_pline*=(SkalaF/Jednostki);  //w cm
                poliline_length+=len_pline;
                L_x2=(double)(L->x2);
                L_x1=(double)(L->x1);
                if (VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, &L_x2, &L_x1, NULL)))  line_angle=M_PI/2;
                else line_angle=atan((L->y2-L->y1)/(L->x2-L->x1));
                Tn[new_texts-1].kat=line_angle;
                Tn[new_texts-1].x-=(dh_text*sin(line_angle));
                Tn[new_texts-1].y+=(dh_text*cos(line_angle));
                Tn[new_texts-1].justowanie=j_srodkowo;
                //sprintf(Tn[new_texts-1].text,"%.0f",len_pline);
                ret=VOID_TO_INT(plugin_ptr(GET_DIM_ROUND_TEXT, (char*)&len_pline, (char*)&Tn[new_texts-1].text, NULL));
                Tn[new_texts-1].dl=strlen(Tn[new_texts-1].text);
                Tn[new_texts-1].n=T18+Tn[new_texts-1].dl;
                Tn[new_texts - 1].width = 0.0;
                Tn[new_texts - 1].height = 0.0;
                Tn[new_texts-1].blok=1;
                Tn[new_texts-1].warstwa=Current_Layer;
                Tn[new_texts-1].ukryty=0;
                if (new_texts==1)
                {
                    max_length=len_pline;
                    c_x=(L->x1+L->x2)/2;
                    c_y=(L->y1+L->y2)/2;
                    c_angle=line_angle;
                }
                else
                {
                    if (len_pline>max_length)
                    {
                        max_length=len_pline;
                        c_x=(L->x1+L->x2)/2;
                        c_y=(L->y1+L->y2)/2;
                        c_angle=line_angle;
                    }
                }
            }
            adp+=(sizeof(NAGLOWEK)+L->n);
        }
        else if (nag->obiekt == Oluk)
        {
            l = (LUK*)adp;
            //dimensioning of a polyline arc
            if (new_texts<max_new_texts)
            {
                new_texts++;
                l_x1=l->x+l->r*cos(l->kat1);
                l_y1=l->y+l->r*sin(l->kat1);
                l_x2=l->x+l->r*cos(l->kat2);
                l_y2=l->y+l->r*sin(l->kat2);
                memmove(&Tn[new_texts-1], &Tn0, sizeof(TEXT));
                Tn[new_texts-1].x=(l_x1+l_x2)/2;
                Tn[new_texts-1].y=(l_y1+l_y2)/2;
                //counting length
                if (l->kat1 > l->kat2) del_kat=(l->kat2 + Pi2) - l->kat1;
                else del_kat = l->kat2 - l->kat1;
                len_pline = Pi2 * l->r * (del_kat/Pi2);

                len_pline*=(SkalaF/Jednostki);  //w cm
                poliline_length+=len_pline;
                if (VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, &(l_x2), &(l_x1), NULL))) line_angle=M_PI/2;
                else line_angle=atan((l_y2-l_y1)/(l_x2-l_x1));
                Tn[new_texts-1].kat=line_angle;
                Tn[new_texts-1].x-=(dh_text*sin(line_angle));
                Tn[new_texts-1].y+=(dh_text*cos(line_angle));
                Tn[new_texts-1].justowanie=j_srodkowo;
                //sprintf(Tn[new_texts-1].text,"%.0f",len_pline);
                ret=VOID_TO_INT(plugin_ptr(GET_DIM_ROUND_TEXT, (char*)&len_pline, (char*)&Tn[new_texts-1].text, NULL));
                Tn[new_texts-1].dl=strlen(Tn[new_texts-1].text);
                Tn[new_texts-1].n=T18+Tn[new_texts-1].dl;
                Tn[new_texts - 1].width = 0.0;
                Tn[new_texts - 1].height = 0.0;
                Tn[new_texts-1].blok=1;
                Tn[new_texts-1].warstwa=Current_Layer;
                Tn[new_texts-1].ukryty=0;
                if (new_texts==1)
                {
                    max_length=len_pline;
                    c_x=(l_x1+l_x2)/2;
                    c_y=(l_y1+l_y2)/2;
                    c_angle=line_angle;
                }
                else
                {
                    if (len_pline>max_length)
                    {
                        max_length=len_pline;
                        c_x=(l_x1+l_x2)/2;
                        c_y=(l_y1+l_y2)/2;
                        c_angle=line_angle;
                    }
                }
            }
            adp+=(sizeof(NAGLOWEK)+l->n);
        }
        else
        {
            L = (LINIA*)adp;
            adp+=(sizeof(NAGLOWEK)+L->n);
        }
        nag = (NAGLOWEK*)adp;
    }
    if (new_texts>0)
    {
        for (i=0; i<new_texts; i++)
        {
            dane = (char *)plugin_ptr(GET_DATA, NULL, NULL, NULL);
            adr = (char*)plugin_ptr(ADD_OBJECT, dane, (void*)&Tn[i], NULL);
            if (adr==NULL)
            {
                ret=VOID_TO_INT(plugin_ptr(SET_DIM_PRECISION, (char*)&precision, NULL, NULL));
                return;
            }
            ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char*)&Tn[i], COPY_PUT, (int*)1));
        }
        //summ length
        memmove(&Tn1, &Tn0, sizeof(TEXT));
        Tn1.x=c_x;
        Tn1.y=c_y;
        Tn1.kat=c_angle;
        Tn1.x+=(dh1_text*sin(c_angle));
        Tn1.y-=(dh1_text*cos(c_angle));
        Tn1.justowanie=j_srodkowo;
        //sprintf(Tn1.text,"%s l=%.0f",main_reinforcement,poliline_length);
        ret=VOID_TO_INT(plugin_ptr(GET_DIM_ROUND_TEXT, (char*)&poliline_length, (char*)&Tl, NULL));
        sprintf(Tn1.text,"%s l=%s",main_reinforcement,Tl);
        Tn1.dl=strlen(Tn1.text);
        Tn1.n=T18+Tn1.dl;
        Tn1.width = 0.0;
        Tn1.height = 0.0;
        Tn1.blok=1;
        Tn1.warstwa=Current_Layer;

        Tn1.ukryty=0;
        dane = (char *)plugin_ptr(GET_DATA, NULL, NULL, NULL);
        adr=plugin_ptr(ADD_OBJECT, dane, (void*)&Tn1, NULL);
        if (adr==NULL)
        {
            ret=VOID_TO_INT(plugin_ptr(SET_DIM_PRECISION, (char*)&precision, NULL, NULL));
            return;
        }
        ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char*)&Tn1, COPY_PUT, (int *)1));
    }
    ret=VOID_TO_INT(plugin_ptr(SET_DIM_PRECISION, (char*)&precision, NULL, NULL));
    return;
}

static void update_dia_and_rad_in_menu(void)
{
    char st1[64];
    char r_str[16];
    int ret;

    sprintf(st1, u8"%s %s R=%g", reinforcement, main_reinforcement, radius);
    ret = VOID_TO_INT(plugin_ptr(NOTICE_STR_SHORT, st1, NULL, NULL));

    sprintf(r_str, u8"%g", radius);
    mRebar.xdl = VOID_TO_INT(plugin_ptr(UTF8LEN, (char *) ((*mRebar.pola)[1].txt), NULL, NULL)) + 8;
    ret = VOID_TO_INT(plugin_ptr(MENU_PAR_NEW, (char *) ((*mRebar.pola)[0].txt), main_reinforcement, NULL));
    ret = VOID_TO_INT(plugin_ptr(MENU_PAR_NEW, (char *) ((*mRebar.pola)[1].txt), r_str, NULL));
}

static void  set_diameter (int n)
/*------------------------*/
{  char st1[60];
    int ret;

    strcpy(main_reinforcement, (*mNets0.pola)[n-2].txt);

    sprintf(st1,u8"%s %s R=%g",reinforcement, main_reinforcement, radius);

    ret = VOID_TO_INT(plugin_ptr(NOTICE_STR_SHORT, st1, NULL, NULL));
}

static void rebar_radius(void)
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

    update_dia_and_rad_in_menu();
}

void COMND_MANAGER(int comnd) {
    if (comnd==1) rebar_radius();
    else set_diameter(comnd);
}

void fill_polyline(char *dane000, double radius_)
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
        if ((VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&L_x2, (char*)&L_x1, NULL))) &&
            (VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&L_y2, (char*)&L_y1, NULL))))
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
        while (1)
        {
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

            adr1 = (char *) (plugin_ptr(FILLET_LINE_TO_LINE, (char*)&radius_, (char*)line1, (char*)line2));
            if (adr1!=NULL) adr=adr1;
            i_no--;

            if (i_no >= (l_no-1)) break;  //there is just one line left, so no filling possible
            adr += (sizeof(NAGLOWEK) + ((NAGLOWEK *) adr)->n);
        }
        if (closed)
        {  //taking first and last line
            first=(char*)plugin_ptr(GET_PTR__GTMPFIRST, NULL, NULL, NULL);
            last=(char*)plugin_ptr(GET_PTR__GTMPLAST, NULL, NULL, NULL);
            adr1 = (char *) (plugin_ptr(FILLET_LINE_TO_LINE, (char*)&radius_, last, first));
        }
    }
}

void alfa_func(int a) {
    int ret, ret1, n;
    unsigned int Ltype;
    char blok_type[31];
    double X0, Y0, df_xbeg, df_ybeg;
    int pline_mode;
    long dane_size000, dane_size0000;
    void *dane_size0000_;
    char *dane000;
    X0Y0 x0y0;
    double radius_sk;
    static MENUPSTRUCT menup={NULL, "", 32, 0};
    static MENUPSTRUCT menup_plugin={NULL, "", 32, 0};
    MENUPSTRUCT *menup_p;
    MENUPSTRUCT *menup_plugin_p;
    int p1;

    //TMENU mNets0={20,0,0,6,1,3,0,CMNU,CMBR,CMTX,0,0,0,0,0,&pmNets0,NULL,NULL};

    int iconno_aux, iconno_radius, iconno_diameter;
    unsigned short reinforcement_c;

    SkalaF = *(double*)plugin_ptr(GET_SCALE, NULL, NULL, NULL);
    Jednostki = *(double*)plugin_ptr(GET_UNITS, NULL, NULL, NULL);
    strcpy((char*)&Units,(char*)plugin_ptr(GET_UNITS_STR, NULL, NULL, NULL));

    enum PLINE_MODE {
        PL_MODE_CONTINUE = 1, PL_MODE_LINE, PL_MODE_ARC,
        PL_MODE_END, PL_MODE_UNDO
    };

    switch (language) {
        case ENGLISH:
            strcpy(reinforcement, u8"Reinforcing bar");
            reinforcement_c=L'R';
            break;
        case POLISH:
            strcpy(reinforcement, u8"Zbrojenie");
            reinforcement_c=L'Z';
            break;
        case UKRAINIAN:
            strcpy(reinforcement, u8"Арматурний прут");
            reinforcement_c=L'А';
            break;
        case SPANISH:
            strcpy(reinforcement, u8"Barra de refuerzo");
            reinforcement_c=L'B';
            break;
        default:
            strcpy(reinforcement, u8"Reinforcing bar");
            reinforcement_c=L'R';
            break;
    }

    mRebar.pola=pmRebar_L[language];

    mNets0.off=0;
    n = VOID_TO_INT(plugin_ptr(GETWSP1, &mNets0, NULL, NULL))-1;

    if (n==-1) return;

    ret = VOID_TO_INT(plugin_ptr(NOTICE, 0, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(NOTICE0, 0, NULL, NULL));
    Ltype = (((LINIA*)(plugin_ptr(GET_LINE_G, 0, NULL, NULL)))->typ);

    strcpy(main_reinforcement, (*mNets0.pola)[n].txt);

    update_dia_and_rad_in_menu();

    p1=128;
    ret = VOID_TO_INT(plugin_ptr(SET_LINE_G_TYPE, (char*)&p1, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(SET_ARC_G_TYPE, (char*)&p1, NULL, NULL));

    sprintf(blok_type,"%s", main_reinforcement);

    //ret = VOID_TO_INT(plugin_ptr(PLINEW, &blok_type, NULL, NULL));  //it's alternative

    p1=1;
    ret = VOID_TO_INT(plugin_ptr(SET_PLINE_PARAM, (char*)&p1, NULL, NULL));

    iconno_aux = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "reinforcing.png", NULL, NULL));
    iconno_diameter = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "rebar-diameter.png", NULL, NULL));
    iconno_radius = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "dim_arc_radial.png", NULL, NULL));

    //changing offset of diameter above binding radius
    mNets0.off=2;
    menup_plugin.menu=(char*)&mRebar;
    strcpy(menup_plugin.st, reinforcement); //"Bent rebar");
    menup_plugin.cod=reinforcement_c; //L'R';
    menup_plugin.iconno=iconno_aux;

    (*mRebar.pola)[0].iconno=iconno_diameter;
    (*mRebar.pola)[1].iconno=iconno_radius;

    menup_p=&menup;
    menup_plugin_p=&menup_plugin;

    //saving aux menu
    ret = VOID_TO_INT(plugin_ptr(GETMENUP, (char*)menup_p, NULL, NULL));

beginning:

    ret = VOID_TO_INT(plugin_ptr(SETMENUP, (char*)menup_plugin_p, NULL, NULL));

    dane_size0000 = (long)plugin_ptr(GET_DATA_SIZE, NULL, NULL, NULL);

    ret = VOID_TO_INT(plugin_ptr(ADD_BLOCK, NULL, NULL, NULL));
    if (ret==FALSE)
    {
        p1=1;
        ret = VOID_TO_INT(plugin_ptr(SET_PLINE_PARAM, (char*)&p1, NULL, NULL));
        finish_pline(Ltype);
        ret = VOID_TO_INT(plugin_ptr(SETMENUP, (char*)menup_p, NULL, NULL));
        ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_aux, NULL, NULL));
        ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_diameter, NULL, NULL));
        ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_radius, NULL, NULL));
        return ;
    }

    ret = VOID_TO_INT(plugin_ptr(INVOKE_CUSTOM_COMND, (char*)COMND_MANAGER, NULL, NULL));

start:
    ret = VOID_TO_INT(plugin_ptr(GET_START_PLINE, (char*)&X0, (char*)&Y0, NULL));

    ret1 = VOID_TO_INT(plugin_ptr(REJECT_CUSTOM_COMND, NULL, NULL, NULL));

    if (ret==FALSE)
    {
        p1=1;
        ret = VOID_TO_INT(plugin_ptr(SET_PLINE_PARAM, (char*)&p1, NULL, NULL));
        ret = VOID_TO_INT(plugin_ptr(ERASE_PLINE, NULL, NULL, NULL));
        finish_pline(Ltype);
        ret = VOID_TO_INT(plugin_ptr(SETMENUP, (char*)menup_p, NULL, NULL));
        ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_radius, NULL, NULL));
        ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_diameter, NULL, NULL));
        ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_radius, NULL, NULL));
        return ;
    }
    pline_mode = PL_MODE_LINE ;
    df_xbeg = X0 ;
    df_ybeg = Y0 ;
    while (1)
    {
        if (pline_mode == PL_MODE_LINE)
        {
            pline_mode = VOID_TO_INT(plugin_ptr(PLINE_LINE, (char*)&df_xbeg, (char*)&df_ybeg, NULL));
        }
        else
        {
            pline_mode = VOID_TO_INT(plugin_ptr(PLINE_ARC, (char*)&df_xbeg, (char*)&df_ybeg, NULL));
        }
        if (PL_MODE_END == pline_mode)
        {
            dane000 = (char*)plugin_ptr(GET_DATA, NULL, NULL, NULL);
            //ret = VOID_TO_INT(plugin_ptr(DIMENSIONING_POLYLINE, dane000, 0, NULL));  //alternative
            //optional filleter

            radius_sk = radius *Jednostki / SkalaF;
            if (radius>0.0) fill_polyline(dane000, radius_sk);
            dane000 = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);
            Dimensioning_polyline(dane000, 0);
            dane_size000 = (long)plugin_ptr(GET_DATA_SIZE, NULL, NULL, NULL) - dane_size0000;
            if (dane_size000 > 0)
            {
                ret = VOID_TO_INT(plugin_ptr(GROUP_BLOCK_WITH_TYPE, (char*)&dane_size000, blok_type, NULL));
            }
            goto beginning ;
        }
        else
        if (PL_MODE_UNDO == pline_mode)
        {
            goto start ;
        }
    }
    return;
}
