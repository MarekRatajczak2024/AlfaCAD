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

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#define SOLIDARC_N 60
#define sadef {Anormalny,Osolidarc,0,0,0,1,0,0, SOLIDARC_N /*52*/,  0,7,0, 0, 0, 0,0,0, 255,0,0,0,0,0,0,0,0,0,1,0,0,0,""};

char main_reinforcement[30];
char reinforcement[64];
float diameter = 8;
double radius_factor = 2;  //[units]
double anchor_factor = 5;  //[units] //5 for Eurocode 2, 6 for ACI 318
char *anchor_eu = "Eurocode";
char *anchor_aci = "ACI";
char *anchor_std = "Eurocode";
double anchor_length = 50;  //[mm]  //50 for Eurocode 2, 75 for ACI
double anchorage;
static int language=-1;
static int iconno_plugin=0;
double SkalaF, Jednostki;
char Units[6];
void *(*plugin_ptr)(int plugin_function, char *param1, char *param2, char *param3);

float open_arm_angle=Pi_/8;

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
        {u8"#2",' ',0,NULL},
        {u8"#3",' ',0,NULL},
        {u8"#4",' ',0,NULL},
        {u8"#5",' ',0,NULL},
        {u8"#6",' ',0,NULL},
        {u8"#7",' ',0,NULL},
        {u8"#8",' ',0,NULL},
};

static TMENU mNets0={17,0,0,6,1,3,0,CMNU,CMBR,CMTX,0,0,0,0,0,&pmNets0,NULL,NULL};

float diameters[]={6, 8, 10, 12, 14, 16, 18, 20, 22, 25, 6.4, 9.525, 12.7, 15.875, 19.05, 22.225, 25.4};

static POLE pmRebarEN[] =
        {
                { u8"Diameter\0", L'D', 0, &mNets0 },
                { u8"minimum bending Radius\0", L'R', 0, NULL },
                { u8"minimum Anchorage length\0", L'A', 0, NULL }
        };

static POLE pmRebarPL[] =
        {
                { u8"Średnica\0", L'S', 0, &mNets0 },
                { u8"minimalny Promień gięcia\0", L'P', 0, NULL },
                { u8"minimalna długość Zakotwienia\0", L'Z', 0, NULL }
        };

static POLE pmRebarUA[] =
        {
                { u8"Діаметр\0", L'Д', 0, &mNets0 },
                { u8"мінімум Радіус вигину\0", L'Р', 0, NULL },
                { u8"мінімальна довжина Кріплення\0", L'К', 0, NULL }
        };

static POLE pmRebarES[] =
        {
                { u8"Diámetro\0", L'D', 0, &mNets0 },
                { u8"mínimo Radio de curvatura\0", L'R', 0, NULL },
                { u8"longitud mínima de Anclaje\0", L'A', 0, NULL }
        };


static POLE *pmRebar_L[]={pmRebarEN, pmRebarPL, pmRebarUA, pmRebarES};

static TMENU mRebar={3,0,0,14,1,3,TADD | ICONS,CMNU,CMBR,CMTX,0,CUSTOM_COMND_OFF,0,0,0,&pmRebarEN,NULL,NULL};

static POLE pmAnchor[] =
        {
                { u8"Eurocode 2 (2D/50mm)\0", L'E', 0, NULL },
                { u8"ACI 315 (3D/75mm) \0", L'A', 0, NULL },
        };

static TMENU mAnchor={2,0,0,14,1,3,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmAnchor,NULL,NULL};

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
        return u8"Stirrup reinforcement (plugin)";
        break;
        case POLISH:
        return u8"Strzemię zbrojeniowe (wtyczka)";
        break;
        case UKRAINIAN:
        return u8"Стремена арматури (плагін)";
        break;
        case SPANISH:
        return u8"Estribo de refuerzo (enchufar)";
        break;
        default:
        return u8"Stirrup reinforcement (plugin)";
        break;
    }
}

int plugin_icon(void)
{
    iconno_plugin=VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "stirrup.png", NULL, NULL));
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

void finish_stirrup(unsigned int Ltype) {
    int ret;

    ret = VOID_TO_INT(plugin_ptr(SET_LINE_G_TYPE, (char *) &Ltype, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(SET_ARC_G_TYPE, (char *) &Ltype, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(NOTICE, 0, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(REMOVE_SHORT_NOTICE, NULL, NULL, NULL));
}

#define max_new_texts 31

static double Atan2 (double y, double x)
/*------------------------------*/
{
    if (fabs(y) < 1.0E-10 && fabs (x) < 1.0E-10)
    {
/*   Internal_Error (__LINE__,__FILE__) ;*/
        return 0 ;
    }
    return atan2 (y, x) ;
}

void Dimensioning_stirrup(char *adr0, int opcja, int clockwise, BOOL draw)
/*----------------------------------------------------------------------*/
{ BLOK *ptrs_block, *ptrs_block1;
    char *adp, *adk;
    NAGLOWEK *nag;
    WIELOKAT *w;
    SOLIDARC *sa;
    LINIA L;
    LUK l;
    int new_texts;
    int new_arcs;
    double last_r, this_r;
    TEXT Tn[32];
    LUK ln[32];
    TEXT Tn0=Tdef, Tn1=Tdef;
    char Tl[MaxTextLen];
    double max_length;
    double lowest_y, c_x, c_y, c_angle, c_x_l, c_y_l, c_angle_l;
    double poliline_length;
    double len_pline, len_pline_l;
    double line_angle;
    int i;
    double del_kat;
    double l_x1,l_y1,l_x2,l_y2;
    float dh_text0=0.75, dh_text;     //0.5 [mm]
    float dh1_text0=1.5, dh2_text, dh1_text;    //3.0 [mm]
    int ret;

    ZMIENNE *zmwym;
    int Current_Layer;
    char *dane;
    char *adr;
    double L_x1, L_x2;
    double x11, y11;
    double r1, angle;
    double precision, precision1;

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

    dh_text=(float)zmwym->wysokosc + dh_text0;
    dh1_text=2 * zmwym->wysokosc + dh1_text0;
    dh2_text=(float)zmwym->wysokosc + dh1_text0;

    Tn0.kolor=zmwym->Tkolor;
    Tn0.wysokosc=(float)zmwym->wysokosc;
    Tn0.width_factor=(float)zmwym->width_factor;
    Tn0.czcionka=zmwym->czcionka;
    Tn0.bold=zmwym->bold;
    Tn0.italics=zmwym->italics;

    new_texts=0;
    new_arcs=0;
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
        else if (nag->obiekt == Owwielokat)
        {
            w = (WIELOKAT*)adp;

            if (clockwise==-1) //clockwise order
            {
                L.x1 = w->xy[0];
                L.y1 = w->xy[1];
                L.x2 = w->xy[6];
                L.y2 = w->xy[7];
            }
            else
            {
                L.x1 = w->xy[2];
                L.y1 = w->xy[3];
                L.x2 = w->xy[4];
                L.y2 = w->xy[5];
            }

            //dimensioning of polyline section;
            if (new_texts<max_new_texts)
            {
                new_texts++;
                memmove(&Tn[new_texts-1], &Tn0, sizeof(TEXT));
                Tn[new_texts-1].x=(L.x1+L.x2)/2;
                Tn[new_texts-1].y=(L.y1+L.y2)/2;
                //length calculation
                len_pline=sqrt(((L.x2-L.x1)*(L.x2-L.x1))+((L.y2-L.y1)*(L.y2-L.y1)));
                len_pline*=(SkalaF/Jednostki);  //w cm
                poliline_length+=len_pline;
                L_x2=(double)(L.x2);
                L_x1=(double)(L.x1);
                if (VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&L_x2, (char*)&L_x1, NULL)))
                {
                    if (L.y2<L.y1) line_angle=-M_PI/2;
                    else line_angle=M_PI/2;
                }
                else //line_angle=atan((L.y2-L.y1)/(L.x2-L.x1));
                    line_angle=atan2((L.y2-L.y1), (L.x2-L.x1));
                Tn[new_texts-1].kat=line_angle;

                if (clockwise==-1) //clockwise order
                {
                    Tn[new_texts - 1].x -= (float)(dh_text0 * sin(line_angle));
                    Tn[new_texts - 1].y += (float)(dh_text0 * cos(line_angle));
                }
                else
                {
                    Tn[new_texts - 1].x += (float)(dh_text * sin(line_angle));
                    Tn[new_texts - 1].y -= (float)(dh_text * cos(line_angle));
                }
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

                ret=VOID_TO_INT(plugin_ptr(NORMALIZE_TXT, (char*)&Tn[new_texts-1], NULL, NULL));

                if (new_texts==1)
                {
                    max_length=len_pline;
                    lowest_y=(L.y1+L.y2)/2;
                    c_x=(L.x1+L.x2)/2;
                    c_y=(L.y1+L.y2)/2;
                    c_angle=line_angle;
                    c_x_l=(L.x1+L.x2)/2;
                    c_y_l=(L.y1+L.y2)/2;
                    c_angle_l=line_angle;
                }
                else
                {
                    if (((L.y1+L.y2)/2)<lowest_y)
                    {
                        lowest_y=(L.y1+L.y2)/2;
                        c_x=(L.x1+L.x2)/2;
                        c_y=(L.y1+L.y2)/2;
                        c_angle=line_angle;
                    }
                    if (len_pline>max_length)
                    {
                        max_length=len_pline;
                        c_x_l=(L.x1+L.x2)/2;
                        c_y_l=(L.y1+L.y2)/2;
                        c_angle_l=line_angle;
                    }
                }
            }
            adp+=(sizeof(NAGLOWEK)+w->n);
        }
        else if (nag->obiekt == Osolidarc)
        {
            sa = (SOLIDARC*)adp;
            l.x=sa->x;
            l.y=sa->y;
            l.r=sa->r;
            l.kat1=sa->kat1;
            l.kat2=sa->kat2;
            //dimensioning of a polyline arc
            if (new_texts<max_new_texts)
            {
                new_texts++;
                r1=(l.r+sa->width1/2);
                l_x1=l.x+r1*cos(l.kat1);
                l_y1=l.y+r1*sin(l.kat1);
                l_x2=l.x+r1*cos(l.kat2);
                l_y2=l.y+r1*sin(l.kat2);

                if (l.kat2<l.kat1) l.kat2+=Pi2;
                angle=(l.kat1+l.kat2)/2;

                x11=l.x+r1*cos(angle);
                y11=l.y+r1*sin(angle);


                new_arcs++;
                l.r-=sa->width1/2;
                memmove(&ln[new_arcs-1], &l, sizeof(LUK));

                memmove(&Tn[new_texts-1], &Tn0, sizeof(TEXT));

                //Tn[new_texts-1].x=(l_x1+l_x2)/2;
                //Tn[new_texts-1].y=(l_y1+l_y2)/2;

                Tn[new_texts-1].x=(float)x11;
                Tn[new_texts-1].y=(float)y11;

                //counting length
                if (l.kat1 > l.kat2) del_kat=(l.kat2 + Pi2) - l.kat1;
                else del_kat = l.kat2 - l.kat1;
                len_pline = Pi2 * l.r * (del_kat/Pi2);

                len_pline*=(SkalaF/Jednostki);  //w cm
                poliline_length+=len_pline;
                if (VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&l_x2, (char*)&l_x1, NULL)))
                {
                    if (L.y2<L.y1) line_angle=-M_PI/2;
                    else line_angle=M_PI/2;
                }
                else //line_angle=atan((l_y2-l_y1)/(l_x2-l_x1));
                    line_angle=atan2((l_y2-l_y1), (l_x2-l_x1));
                Tn[new_texts-1].kat=(float)line_angle;

                Tn[new_texts-1].x+=(float)(dh_text*sin(line_angle));
                Tn[new_texts-1].y-=(float)(dh_text*cos(line_angle));

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

                ret=VOID_TO_INT(plugin_ptr(NORMALIZE_TXT, (char*)&Tn[new_texts-1], NULL, NULL));

                if (new_texts==1)
                {
                    max_length=len_pline;
                    lowest_y=(l_y1+l_y2)/2;
                    c_x=(l_x1+l_x2)/2;
                    c_y=(l_y1+l_y2)/2;
                    c_angle=line_angle;
                    c_x_l=(l_x1+l_x2)/2;
                    c_y_l=(l_y1+l_y2)/2;
                    c_angle_l=line_angle;
                }
                else
                {
                    if (((l_y1+l_y2)/2)<lowest_y)
                    {
                        lowest_y=(l_y1+l_y2)/2;
                        c_x=(l_x1+l_x2)/2;
                        c_y=(l_y1+l_y2)/2;
                        c_angle=line_angle;
                        len_pline_l=len_pline;
                    }
                    if (len_pline>max_length)
                    {
                        max_length=len_pline;
                        c_x_l=(l_x1+l_x2)/2;
                        c_y_l=(l_y1+l_y2)/2;
                        c_angle_l=line_angle;
                    }
                }
            }
            adp+=(sizeof(NAGLOWEK)+sa->n);
        }
        else
        {
            nag = (NAGLOWEK*)adp;
            adp+=(sizeof(NAGLOWEK)+nag->n);
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
            if (draw) ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char*)&Tn[i], COPY_PUT, (int*)1));
        }

        last_r=0.0;
        for (i=0; i<new_arcs; i++)
        {
            this_r=(double)ln[i].r;
            if ((VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&this_r, (char*)&last_r, NULL)))==0)
                ret = (VOID_TO_INT(plugin_ptr(LUK_W_IN_BLOCK, (char *) &ln[i], NULL, NULL)));
            last_r=this_r;
        }

        //summ length
        memmove(&Tn1, &Tn0, sizeof(TEXT));
        //if lowest text angle is not equal zero and max_length is zero, max_lenght is taken
        double p2=0.0;
        if (((VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&c_angle, (char*)&p2, NULL)))==0) &&
        (((VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&c_angle_l, (char*)&p2, NULL)))==1)))
        {
            Tn1.x = (float)c_x_l;
            Tn1.y = (float)c_y_l;
            Tn1.kat = (float)c_angle_l;
        }
        else {
            Tn1.x = (float)c_x;
            Tn1.y = (float)c_y;
            Tn1.kat = (float)c_angle;
        }
        if (clockwise==-1) //clockwise order
        {
            Tn1.x -= (float)(dh2_text * sin(c_angle));
            Tn1.y += (float)(dh2_text * cos(c_angle));
        }
        else
        {
            Tn1.x += (float)(dh1_text * sin(c_angle));
            Tn1.y -= (float)(dh1_text * cos(c_angle));
        }
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

        ret=VOID_TO_INT(plugin_ptr(NORMALIZE_TXT, (char*)&Tn1, NULL, NULL));

        dane = (char *)plugin_ptr(GET_DATA, NULL, NULL, NULL);
        adr=plugin_ptr(ADD_OBJECT, dane, (void*)&Tn1, NULL);
        if (adr==NULL)
        {
            ret=VOID_TO_INT(plugin_ptr(SET_DIM_PRECISION, (char*)&precision, NULL, NULL));
            return;
        }
        if (draw) ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char*)&Tn1, COPY_PUT, (int *)1));
    }

    ret=VOID_TO_INT(plugin_ptr(SET_DIM_PRECISION, (char*)&precision, NULL, NULL));
    return;
}

static void update_dia_and_rad_in_menu(void)
{
    char st1[64];
    char r_str[16];
    char a_str[32];
    int ret;

    sprintf(st1, u8"%s %s r=%g*D", reinforcement, main_reinforcement, radius_factor);
    ret = VOID_TO_INT(plugin_ptr(NOTICE_STR_SHORT, st1, NULL, NULL));

    sprintf(r_str, u8"%g", radius_factor);
    sprintf(a_str, u8"%s %g/%g", anchor_std, anchor_factor, anchor_length);

    mRebar.xdl = VOID_TO_INT(plugin_ptr(UTF8LEN, (char *) ((*mRebar.pola)[1].txt), NULL, NULL)) + 8;
    ret = VOID_TO_INT(plugin_ptr(MENU_PAR_NEW, (char *) ((*mRebar.pola)[0].txt), main_reinforcement, NULL));
    ret = VOID_TO_INT(plugin_ptr(MENU_PAR_NEW, (char *) ((*mRebar.pola)[1].txt), r_str, NULL));
    ret = VOID_TO_INT(plugin_ptr(MENU_PAR_NEW, (char *) ((*mRebar.pola)[2].txt), a_str, NULL));
}

static void  set_diameter (int n)
/*------------------------*/
{  char st1[60];
    int ret;

    strcpy(main_reinforcement, (*mNets0.pola)[n-4].txt);
    diameter=diameters[n-4];

    sprintf(st1,u8"%s %s r=%g*D",reinforcement, main_reinforcement, radius_factor);
    ret = VOID_TO_INT(plugin_ptr(NOTICE_STR_SHORT, st1, NULL, NULL));
}

static void minimal_anchorage_length(void)
{
    int ret, n;
    int iconno_eurocode = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "eurocode.png", NULL, NULL));
    int iconno_aci = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "aci.png", NULL, NULL));
    (*mAnchor.pola)[0].iconno=iconno_eurocode;
    (*mAnchor.pola)[1].iconno=iconno_aci;
    n = VOID_TO_INT(plugin_ptr(GETWSP1, &mAnchor, NULL, NULL))-1;
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_eurocode, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_aci, NULL, NULL));
    if (n==-1) return;
    switch (n)
    {
        case 0:
            anchor_factor = 5;  //[units] //5 for Eurocode 2, 6 for ACI 318
            anchor_length = 50;  //[mm]  //50 for Eurocode 2, 75 for ACI
            anchor_std=anchor_eu;
            break;
        case 1:
            anchor_factor = 6;
            anchor_length = 75;
            anchor_std=anchor_aci;
            break;
        default:
            anchor_factor = 5;
            anchor_length = 50;
            anchor_std=anchor_eu;
            break;
    }
    update_dia_and_rad_in_menu();
}

static void minimal_bending_radius(void)
{
    char radius_str[16];
    char prompt[64];
    int ret;
    int retval_no = 1;
    double buf_ret[1];

    switch (language)
    {
        case ENGLISH:
            sprintf(prompt,u8"minimal bending radius (*D): ");
        break;
        case POLISH:
            sprintf(prompt,u8"minimalny promień gięcia (*D): ");
        break;
        case UKRAINIAN:
            sprintf(prompt,u8"мінімум радіус вигину (*D): ");
        break;
        case SPANISH:
            sprintf(prompt,u8"mínimo radio de curvatura (*D): ");
        break;
        default:
            sprintf(prompt,u8"minimal bending radius %s: ", Units);
        break;
    }

    ret = VOID_TO_INT(plugin_ptr(DF_TO_STRING, (char*)radius_str, "%-7.3f", (double *)&radius_factor));
    ret = VOID_TO_INT(plugin_ptr(GET_STRING_STR, (char*)&radius_str, "", (char*)prompt));

    ret = VOID_TO_INT(plugin_ptr(CALCULATOR, (char*)radius_str, (int*)&retval_no, (double*)&buf_ret));

    if ((ret==FALSE) || (retval_no < 1))
    {
        return ;
    }
    radius_factor = buf_ret [0] ;

    update_dia_and_rad_in_menu();
}

void COMND_MANAGER(int comnd) {
    if (comnd==1) minimal_bending_radius();
    else if (comnd==2) minimal_anchorage_length();
    else set_diameter(comnd);
}

static void Rotate_Point (double si, double co,
                   double x1, double y1,  			    /*center point*/
                   double x2, double y2, double *x, double *y) /*rotate point*/
/*-------------------------------------------------------------------------------------*/
{
    *x = x1 + (x2 - x1) * co - (y2 - y1) * si ;
    *y = y1 + (x2 - x1) * si + (y2 - y1) * co ;
}

static void obrd(double si,double co, double xp, double yp, double *xn, double *yn)
/*------------------------------------------------*/
{ *xn=+xp*co-yp*si;	/*nie zgodny z kierunkiem ruchu wskazowek zegara*/
    *yn= xp*si+yp*co;
}

int set_solid(LINIA *line, float width, float axis1, float axis2, int color, int current_layer, WIELOKAT *solid)
{
    double dx1, dy1, dx2, dy2, df_l1 ;
    double angle_r, si_r, co_r, angle_b, si_b, co_b ;
    double df_sx1, df_sy1, df_sx2, df_sy2 ;

    dx1 = line->x2 - line->x1 ;
    dy1 = line->y2 - line->y1 ;
    df_l1 = sqrt (dx1 * dx1 + dy1 * dy1) ;
    angle_r = Atan2 (dy1, dx1) ;
    si_r = sin (angle_r) ;
    co_r = cos (angle_r) ;

        df_sx1 = 0 ;
        df_sx2 = 0 ;

        df_sy1 = (width / 2) - axis1;
        df_sy2 = (-width / 2) - axis1;

        obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
        obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;
        solid->xy [0] = df_sx1 + line->x1 ;
        solid->xy [1] = df_sy1 + line->y1 ;
        solid->xy [2] = df_sx2 + line->x1 ;
        solid->xy [3] = df_sy2 + line->y1 ;

    df_sx1 = df_l1 ;
    df_sx2 = df_l1 ;


    df_sy1 = (- width / 2) - axis2;
    df_sy2 = (width / 2) - axis2;
    obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
    obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;
    solid->xy [4] = df_sx1 + line->x1 ;
    solid->xy [5] = df_sy1 + line->y1 ;
    solid->xy [6] = df_sx2 + line->x1 ;
    solid->xy [7] = df_sy2 + line->y1 ;
    solid->kolor = color ;
    solid->warstwa = current_layer ;

    return 1;
}

static double Angle_Normal (double angle)
/*-------------------------------*/
{
    if (fabs (angle) >= Pi2 + o_male)
    {
        angle -= floor (0.5 + angle / Pi2) * Pi2 ;
    }
    if(angle < 0)
    {
        angle += Pi2;
    }
    return angle;
}


static double polygonArea(OKRAG *o, int n)
{   int i, j;
    double area = 0;
    for (i = 0; i < n; i++) {
        j = (i + 1) % n;
        area += o[i].x * o[j].y;
        area -= o[j].x * o[i].y;
    }
    return area / 2;
}

int get_clockwise(OKRAG *circles, int circles_no)
{
    double area;

    area=polygonArea(circles, circles_no);
    if (area==0)
    {
        if (circles_no==2) return -1;
        else
        {
            area = polygonArea(circles, 3);
            if (area<0) return -1; // polygon points are in clockwise order
            else return 1; //polygon points are in counterclockwise order
        }
    }
    if (area<=0) return -1; // polygon points are in clockwise order
    else return 1; //polygon points are in counterclockwise order
}

int add_stirrup(OKRAG *circles, int circles_no, int *clockwise, BOOL closed, BOOL draw)
{
    int ret;
    PLINIA PL;
    char *dane;
    char *adr;
    LINIA L=Ldef, L1=Ldef;
    WIELOKAT w=S4def;
    SOLIDARC sa=sadef;
    BLOK *b;
    void *obj;
    double line1_angle, line2_angle;;
    double L_x1, L_y1, L_x2, L_y2;
    double extra_angle, delta_angle;
    double angle, kos, koc;
    float radius;
    double x11, y11;
    float width, axis1, axis2;
    int color, current_layer;
    double area;

    area=polygonArea(circles, circles_no);
    if (area==0)
    {
        if (circles_no==2) *clockwise=-1;
        else
        {
            area = polygonArea(circles, 3);
            if (area<0) *clockwise=-1; // polygon points are in clockwise order
            else *clockwise=1; //polygon points are in counterclockwise order
        }
    }
    else if (area<0) *clockwise=-1; // polygon points are in clockwise order
    else *clockwise=1; //polygon points are in counterclockwise order

    //building trace
    ret = VOID_TO_INT(plugin_ptr(ADD_TRACE, NULL, NULL, NULL));
    ////printf("%x\n", ret);
    if (ret == TRUE)
    {
        w.empty_typ = 2;
        w.blok=1;
        sa.empty_typ = 2;
        sa.blok=1;

        if (*clockwise==1) sa.reversed=0;
        else sa.reversed=1;

        current_layer=*(int*)(plugin_ptr(GET_CURRENT_LAYER, NULL, NULL, NULL));
        color = (((LINIA*)(plugin_ptr(GET_LINE_G, 0, NULL, NULL)))->kolor);

        sa.warstwa=current_layer;
        sa.kolor=color;
        sa.widoczny=1;

        sa.axis1=0;
        sa.axis2=0;

        //anchorage=max(anchor_factor*diameter, anchor_length)*Jednostki/SkalaF;
        anchorage=max(anchor_factor*diameter, anchor_length)/SkalaF;
        radius=(radius_factor*diameter)*Jednostki/SkalaF;
        //width=diameter*Jednostki/SkalaF;
        width=diameter/SkalaF;

        for (int i = 0; i < circles_no-1; i++)
        {
            L.x1 = circles[i].x;
            L.y1 = circles[i].y;
            L.x2 = circles[i+1].x;
            L.y2 = circles[i+1].y;

            line1_angle=atan2((L.y2-L.y1), (L.x2-L.x1));

            if (i == 0)
            {
                L_x1=(double)(L.x1);
                L_x2=(double)(L.x2);


                extra_angle=+Pi_/4;

                if (circles_no>2)
                {
                    L1.x1 = circles[1].x;
                    L1.y1 = circles[1].y;
                    L1.x2 = circles[2].x;
                    L1.y2 = circles[2].y;
                    L_x1=(double)(L1.x1);
                    L_x2=(double)(L1.x2);
                    line2_angle=atan2((L1.y2-L1.y1), (L1.x2-L1.x1));
                    if (line2_angle<line1_angle) line2_angle+=Pi2;

                    if (line2_angle-line1_angle>(Pi_*1.01)) extra_angle=-Pi_/4;
                }

                if (*clockwise==1) extra_angle=+Pi_/4;
                else extra_angle=-Pi_/4;

                ////printf("circles_no=%d\n", circles_no);

                ////printf("BEGINING: line1_angle=%f  line2_angle=%f  extra_angle=%f\n",line1_angle,line2_angle,extra_angle);

                ret=VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char*)&L, (char*)&PL, NULL));
                ////printf("angle=%f\n",PL.kat);

                angle=Pi_* PL.kat / 180 + extra_angle;

                kos=sin(angle);
                koc=cos(angle);

                ////printf("angle=%f  kos=%f  koc=%f\n",angle, kos, koc);

                Rotate_Point(kos, koc, L.x1, L.y1, L.x1+anchorage, L.y1, &x11, &y11);

                L1.x1=x11;
                L1.y1=y11;

                L1.x2=circles[i].x;
                L1.y2=circles[i].y;

                dane = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);

                //adding solid
                axis1=*clockwise*(width/2.0+circles[i].r);
                //axis2=width/2.0+circles[i+1].r;
                axis2=axis1;
                set_solid(&L1, width, axis1, axis2, color, current_layer, &w);
                ////printf("%f %f %f %f %f %f %f %f\n",w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[4], w.xy[5], w.xy[6], w.xy[7]);

                obj=(void*)&w;

                adr = (char *) plugin_ptr(ADD_OBJECT, dane, (void *)obj, NULL);
                if (adr == NULL) return 0;
                ////ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char *) obj, COPY_PUT, (int *) 1));

                dane = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);
                //solidarc
                sa.r=circles[i].r+width/2.0;
                if (sa.reversed)
                {
                    sa.kat2 = Angle_Normal(angle - Pi_ / 2.0);
                    sa.kat1 = Angle_Normal(line1_angle + Pi_ / 2.0);
                }
                else
                {
                    sa.kat1 = Angle_Normal(angle + Pi_ / 2.0);
                    sa.kat2 = Angle_Normal(line1_angle - Pi_ / 2.0);
                }
                sa.width1=width;
                sa.width2=width;
                sa.x=circles[i].x;
                sa.y=circles[i].y;

                obj=(void*)&sa;

                adr = (char *) plugin_ptr(ADD_OBJECT, dane, (void *)obj, NULL);
                if (adr == NULL) return 0;
                ////ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char *) obj, COPY_PUT, (int *) 1));

            }
            else
            {
                L1.x1 = circles[i+1].x;
                L1.y1 = circles[i+1].y;
                if (i<circles_no-2)
                {
                    L1.x2 = circles[i + 2].x;
                    L1.y2 = circles[i + 2].y;
                }
                else
                {
                    L1.x2 = circles[1].x;
                    L1.y2 = circles[1].y;
                }
                line2_angle=atan2((L1.y2-L1.y1), (L1.x2-L1.x1));
            }

            dane = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);

            //adding solid
            axis1=*clockwise*(width/2.0+circles[i].r);
            axis2=*clockwise*(width/2.0+circles[i+1].r);
            set_solid(&L, width, axis1, axis2, color, current_layer, &w);
            ////printf("%f %f %f %f %f %f %f %f\n",w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[4], w.xy[5], w.xy[6], w.xy[7]);

            obj=(void*)&w;

            adr = (char *) plugin_ptr(ADD_OBJECT, dane, (void *)obj, NULL);
            if (adr == NULL) return 0;

            ////ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char *) obj, COPY_PUT, (int *) 1));

            if (i == circles_no-2)
            {

                L_x1=(double)(L.x1);
                L_x2=(double)(L.x2);
                line1_angle=atan2((L.y2-L.y1), (L.x2-L.x1));


                extra_angle=+3*Pi_/4;

                if (circles_no > 2) {
                    L1.x1 = circles[0].x;
                    L1.y1 = circles[0].y;
                    L1.x2 = circles[1].x;
                    L1.y2 = circles[1].y;
                    L_x1 = (double) (L1.x1);
                    L_x2 = (double) (L1.x2);
                    line2_angle = atan2((L1.y2 - L1.y1), (L1.x2 - L1.x1));
                    if (line2_angle < line1_angle) line2_angle += Pi2;

                    if (line2_angle - line1_angle > (Pi_ * 1.01)) extra_angle = -3 * Pi_ / 4;
                }

                if (*clockwise==1) extra_angle=+3*Pi_/4;
                else extra_angle=-3 * Pi_/4;

                ////printf("END: line1_angle=%f  line2_angle=%f  extra_angle=%f\n",line1_angle,line2_angle,extra_angle);

                ret=VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char*)&L, (PLINIA*)&PL, NULL));
                ////printf("angle=%f\n",PL.kat);

                angle=Pi_* PL.kat / 180 + extra_angle;

                kos=sin(angle);
                koc=cos(angle);

                ////printf("angle=%f  kos=%f  koc=%f\n",angle, kos, koc);

                Rotate_Point(kos, koc, L.x2, L.y2, L.x2+anchorage, L.y2, &x11, &y11);

                L1.x2=x11;
                L1.y2=y11;

                L1.x1=L.x2;
                L1.y1=L.y2;

                dane = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);
                //solidarc
                sa.r=circles[i+1].r+width/2.0;
                if (sa.reversed)
                {
                    sa.kat2 = Angle_Normal(line1_angle + Pi_ / 2.0);
                    sa.kat1 = Angle_Normal(angle + Pi_ / 2.0);
                }
                else
                {
                    sa.kat1 = Angle_Normal(line1_angle - Pi_ / 2.0);
                    sa.kat2 = Angle_Normal(angle - Pi_ / 2.0);
                }
                sa.width1=width;
                sa.width2=width;
                sa.x=circles[i+1].x;
                sa.y=circles[i+1].y;

                obj=(void*)&sa;

                adr = (char *) plugin_ptr(ADD_OBJECT, dane, (void *)obj, NULL);
                if (adr == NULL) return 0;
                ////ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char *) obj, COPY_PUT, (int *) 1));


                dane = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);

                //adding solid
                axis1=*clockwise*(width/2.0+circles[i+1].r);
                axis2=axis1;
                set_solid(&L1, width, axis1, axis2, color, current_layer, &w);
                ////printf("%f %f %f %f %f %f %f %f\n",w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[4], w.xy[5], w.xy[6], w.xy[7]);

                obj=(void*)&w;

                adr = (char *) plugin_ptr(ADD_OBJECT, dane, (void *)obj, NULL);
                if (adr == NULL) return 0;
                ////ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char *) obj, COPY_PUT, (int *) 1));

            }
            else
            {
                dane = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);
                //solidarc
                sa.r=circles[i+1].r+width/2.0;
                //sa.kat1=Angle_Normal(line2_angle+Pi_);
                //sa.kat2=Angle_Normal(line1_angle);
                if (sa.reversed)
                {
                    sa.kat2 = Angle_Normal(line1_angle + Pi_ / 2.0);
                    sa.kat1 = Angle_Normal(line2_angle + Pi_ / 2.0);
                }
                else
                {
                    sa.kat1 = Angle_Normal(line1_angle - Pi_ / 2.0);
                    sa.kat2 = Angle_Normal(line2_angle - Pi_ / 2.0);
                }
                sa.width1=width;
                sa.width2=width;
                sa.x=circles[i+1].x;
                sa.y=circles[i+1].y;

                obj=(void*)&sa;

                adr = (char *) plugin_ptr(ADD_OBJECT, dane, (void *)obj, NULL);
                if (adr == NULL) return 0;
                ////ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char *) obj, COPY_PUT, (int *) 1));
            }
        }

        if (draw)
        {
            dane = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);
            b = (BLOK *) dane;
            ret = VOID_TO_INT(plugin_ptr(DRAW_BLOCK, (char *) dane, (char *) dane + b->n + sizeof(NAGLOWEK),(int *) ANieOkreslony));
            ret = VOID_TO_INT(plugin_ptr(FLIPSCREEN, NULL, NULL, NULL));
        }
    }
    return 1;
}

static double nearest_orto(double angle)
{   double anglemin, absanglemin, angle_;
    angle_=fabs(angle);
    absanglemin=fabs(angle);
    anglemin=angle;
    if (fabs(Pi_/2.0-angle_)<absanglemin)
    {
        absanglemin=fabs(Pi_/2.0-angle_);
        anglemin=Pi_/2-angle_;
    }
    if (fabs(Pi_-angle_)<absanglemin)
    {
        absanglemin=fabs(Pi_-angle_);
        anglemin=Pi_-angle_;
    }
    if (fabs(3*Pi_/2.0-angle_)<absanglemin)
    {
        absanglemin=fabs(3*Pi_/2.0-angle_);
        anglemin=Pi_-angle_;
    }
    if (fabs(2*Pi_-angle_)<absanglemin)
    {
        absanglemin=fabs(2*Pi_-angle_);
        anglemin=2*Pi_-angle_;
    }
    return anglemin;
}

void alfa_func(int a) {
    int ret, n;
    unsigned int Ltype;
    char blok_type[31];
    long dane_size000;
    char *dane000;
    static MENUPSTRUCT menup={NULL, "", 32, 0};
    static MENUPSTRUCT menup_plugin={NULL, "", 32, 0};
    MENUPSTRUCT *menup_p;
    MENUPSTRUCT *menup_plugin_p;
    unsigned int typ;
    char *obj_ptr;
    char kom[254];
    OKRAG *circles;
    int circles_no=0;
    int max_circles_no=10;
    PLINIA PL0, PL, PL1;
    double x11, y11;
    double angle, kos, koc, kos1, koc1;
    int clockwise;
    float dx, dy;
    int p1, p2, p3;
    double X,Y;
    char stirrup_str[64];
    int iconno_aux, iconno_radius, iconno_anchor, iconno_diameter;
    unsigned short reinforcement_c;
    double radius_min, d_radius;
    LINIA L1, L2;
    BOOL closed;
    double cx, cy, cx1, cy1;

    circles=(OKRAG*)malloc(max_circles_no*sizeof(OKRAG));

    SkalaF = *(double*)plugin_ptr(GET_SCALE, NULL, NULL, NULL);
    Jednostki = *(double*)plugin_ptr(GET_UNITS, NULL, NULL, NULL);
    strcpy((char*)&Units,(char*)plugin_ptr(GET_UNITS_STR, NULL, NULL, NULL));

    enum PLINE_MODE {
        PL_MODE_CONTINUE = 1, PL_MODE_LINE, PL_MODE_ARC,
        PL_MODE_END, PL_MODE_UNDO
    };

    switch (language) {
        case ENGLISH:
            strcpy(reinforcement, u8"reinforcement Stirrup");
            strcpy(kom, u8"Select the reinforcing bars (circles or discs) to be surrounded by the stirrup");
            reinforcement_c=L'S';
            strcpy(stirrup_str, u8"Stirrup shape & dimensions");
            break;
        case POLISH:
            strcpy(reinforcement, u8"Strzemię zbrojeniowe");
            strcpy(kom, u8"Wskaż pręty zbrojeniowe (okrągi lub koła), które mają być otoczone strzemieniem");
            reinforcement_c=L'S';
            strcpy(stirrup_str, u8"Kształt i wymiary strzemiona");
            break;
        case UKRAINIAN:
            strcpy(reinforcement, u8"Стремена арматури");
            strcpy(kom, u8"Виберіть арматурний стрижень (колеса або диски), які мають бути оточені стременем");
            reinforcement_c=L'С';
            strcpy(stirrup_str, u8"Форма і розмір стремена");
            break;
        case SPANISH:
            strcpy(reinforcement, u8"Estribo de refuerzo");
            strcpy(kom, u8"Seleccione las barras de refuerzo (círculos o discos) que estarán rodeadas por el estribo");
            reinforcement_c=L'E';
            strcpy(stirrup_str, u8"Forma y tamaño del estribo");
            break;
        default:
            strcpy(reinforcement, u8"reinforcement Stirrup");
            strcpy(kom, u8"Select the reinforcing bars (circles or discs) to be surrounded by the stirrup");
            reinforcement_c=L'S';
            strcpy(stirrup_str, u8"Stirrup shape & dimensions");
            break;
    }

    mRebar.pola=pmRebar_L[language];

    mNets0.off=0;
    n = VOID_TO_INT(plugin_ptr(GETWSP1, &mNets0, NULL, NULL))-1;

    if (n==-1) return;

    diameter=diameters[n];

    ret = VOID_TO_INT(plugin_ptr(NOTICE, 0, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(NOTICE0, 0, NULL, NULL));
    Ltype = (((LINIA*)(plugin_ptr(GET_LINE_G, 0, NULL, NULL)))->typ);

    strcpy(main_reinforcement, (*mNets0.pola)[n].txt);

    update_dia_and_rad_in_menu();

    p1=128;
    ret = VOID_TO_INT(plugin_ptr(SET_LINE_G_TYPE, (char*)&p1, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(SET_ARC_G_TYPE, (char*)&p1, NULL, NULL));

    sprintf(blok_type,"%s", main_reinforcement);

    p1=TRUE;
    ret = VOID_TO_INT(plugin_ptr(SET_PLINE_PARAM, (char*)&p1, NULL, NULL));

    iconno_aux = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "stirrup.png", NULL, NULL));
    iconno_diameter = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "rebar-diameter.png", NULL, NULL));
    iconno_radius = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "stirrup_r.png", NULL, NULL));
    iconno_anchor = VOID_TO_INT(plugin_ptr(LOAD_CLIENT_BITMAP, "stirrup_a.png", NULL, NULL));

    //changing offset of diameter above binding radius
    mNets0.off=4;
    menup_plugin.menu=(char*)&mRebar;
    strcpy(menup_plugin.st, reinforcement); //"Bent rebar");
    menup_plugin.cod=reinforcement_c; //L'R';
    menup_plugin.iconno=iconno_aux;

    (*mRebar.pola)[0].iconno=iconno_diameter;
    (*mRebar.pola)[1].iconno=iconno_radius;
    (*mRebar.pola)[2].iconno=iconno_anchor;

    menup_p=&menup;
    menup_plugin_p=&menup_plugin;

    //saving aux menu
    ret = VOID_TO_INT(plugin_ptr(GETMENUP, (char*)menup_p, NULL, NULL));

beginning:

    ret = VOID_TO_INT(plugin_ptr(SETMENUP, (char*)menup_plugin_p, NULL, NULL));

    ret = VOID_TO_INT(plugin_ptr(INVOKE_CUSTOM_COMND, (char*)COMND_MANAGER, NULL, NULL));

    dane_size000 = (long)plugin_ptr(GET_DATA_SIZE, NULL, NULL, NULL);

    typ=(Bokrag | Bkolo);


    while (1)
    {
        obj_ptr = plugin_ptr(SELECT_OBJECT, (char*)&typ, kom, NULL);
        if (obj_ptr == NULL) break;
        // collecting circle data
        if (circles_no==max_circles_no)
        {
            max_circles_no+=10;
            circles=(OKRAG*)realloc(circles, max_circles_no*sizeof(OKRAG));
        }
        memmove(&circles[circles_no], obj_ptr, sizeof(OKRAG));
        circles_no++;
        //marking object
        p2=Ablok;
        ret = VOID_TO_INT(plugin_ptr(SET_ATTRIBUTE, (char*)obj_ptr, (char*)&p2, NULL));
        p3=1;
        p2=XOR_PUT;
        ret = VOID_TO_INT(plugin_ptr(DRAW_OBJECT, (char*)obj_ptr, (char*)&p2, (char*)&p3));
    }

    ret=VOID_TO_INT(plugin_ptr(OUT_EDIT_STR, NULL, NULL, NULL));

    ret = VOID_TO_INT(plugin_ptr(REJECT_CUSTOM_COMND, NULL, NULL, NULL));

    ret = VOID_TO_INT(plugin_ptr(SETMENUP, (char*)menup_p, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_radius, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_anchor, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_diameter, NULL, NULL));
    ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_radius, NULL, NULL));

    /*
    for (int i=0; i<circles_no; i++)
    {
        printf("x=%f  y=%f  r=%f\n", circles[i].x, circles[i].y, circles[i].r);
    }
    */

    //correction of circles position due to minimum bending radius
    if (diameter<=16) radius_min=radius_factor*diameter;  //e.g. for 8 mm and factor 2 stirrup radius_min=16 mm
    else radius_min=radius_factor*1.75*diameter; //e.g. for 18 mm and factor 2 stirrup radius_min=63 mm

    //radius_min=radius_min*Jednostki/SkalaF;
    radius_min=radius_min/SkalaF;

    clockwise=get_clockwise(circles, circles_no);
    //if  -1  polygon points are in clockwise order
    //else if 1 polygon points are in counterclockwise order

    closed=FALSE;

    cx=(double)circles[0].x;
    cy=(double)circles[0].y;
    cx1=(double)circles[circles_no-1].x;
    cy1=(double)circles[circles_no-1].y;

    if ((VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&cx, (char*)&cx1, NULL))) &&
            (VOID_TO_INT(plugin_ptr(CHECK_IF_EQUAL, (char*)&cy, (char*)&cy1, NULL))))  //is closed
        closed=TRUE;

    for (int i=0; i<circles_no; i++)
    {
        d_radius=radius_min-circles[i].r;
        if (d_radius<0) continue;

        if (clockwise==1)
        {
            if (i == 0)
            {
                L1.x1 = circles[i].x;
                L1.y1 = circles[i].y;
                L1.x2 = circles[i + 1].x;
                L1.y2 = circles[i + 1].y;
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L1, (char *) &PL, NULL));
                if (closed) {
                    L2.x1 = circles[circles_no - 1].x;
                    L2.y1 = circles[circles_no - 1].y;
                    L2.x2 = circles[circles_no - 2].x;
                    L2.y2 = circles[circles_no - 2].y;
                }
                else {
                    L2.x1 = L1.x1;
                    L2.y1 = L1.y1;
                    koc1 = cos(Pi_/2.0);
                    kos1 = sin(Pi_/2.0);
                    Rotate_Point(kos1, koc1, (double)L1.x1, (double)L1.y1, (double)L1.x2, (double)L1.y2, &x11, &y11);
                    L2.x2 = (float)x11;
                    L2.y2 = (float)y11;
                }
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L2, (char *) &PL1, NULL));
            }
            else if (i == circles_no - 1)
            {
                L2.x1 = circles[circles_no - 1].x;
                L2.y1 = circles[circles_no - 1].y;
                L2.x2 = circles[circles_no - 2].x;
                L2.y2 = circles[circles_no - 2].y;
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L2, (char *) &PL1, NULL));
                if (closed) {
                    L1.x1 = circles[0].x;
                    L1.y1 = circles[0].y;
                    L1.x2 = circles[1].x;
                    L1.y2 = circles[1].y;
                }
                else
                {
                    L1.x1 = L2.x1;
                    L1.y1 = L2.y1;
                    koc1 = cos(-Pi_/2.0);
                    kos1 = sin(-Pi_/2.0);
                    Rotate_Point(kos1, koc1, (double)L2.x1, (double)L2.y1, (double)L2.x2, (double)L2.y2, &x11, &y11);
                    L1.x2 = (float)x11;
                    L1.y2 = (float)y11;
                }
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L1, (char *) &PL, NULL));
            } else
            {
                L1.x1 = circles[i].x;
                L1.y1 = circles[i].y;
                L1.x2 = circles[i + 1].x;
                L1.y2 = circles[i + 1].y;
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L1, (char *) &PL, NULL));
                L2.x1 = circles[i].x;
                L2.y1 = circles[i].y;
                L2.x2 = circles[i - 1].x;
                L2.y2 = circles[i - 1].y;
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L2, (char *) &PL1, NULL));
            }

            if (PL1.kat < PL.kat) PL1.kat += 360;
            angle = (PL1.kat + PL.kat) / 2.0;

            koc = cos(Pi_ * (angle) / 180);
            kos = sin(Pi_ * (angle) / 180);

            Rotate_Point(kos, koc, (double)circles[i].x, (double)circles[i].y, (double)circles[i].x+d_radius, (double)circles[i].y, &x11, &y11);

            circles[i].x = (float) x11;
            circles[i].y = (float) y11;
            circles[i].r = (float) radius_min;
        }
        else
        {
            if (i == 0)
            {
                L2.x1 = circles[i].x;
                L2.y1 = circles[i].y;
                L2.x2 = circles[i + 1].x;
                L2.y2 = circles[i + 1].y;
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L2, (char *) &PL1, NULL));

                if (closed) {
                    L1.x1 = circles[circles_no - 1].x;
                    L1.y1 = circles[circles_no - 1].y;
                    L1.x2 = circles[circles_no - 2].x;
                    L1.y2 = circles[circles_no - 2].y;
                }
                else {
                    L1.x1 = L2.x1;
                    L1.y1 = L2.y1;
                    koc1 = cos(-Pi_/2.0);
                    kos1 = sin(-Pi_/2.0);
                    Rotate_Point(kos1, koc1, (double)L2.x1, (double)L2.y1, (double)L2.x2, (double)L2.y2, &x11, &y11);
                    L1.x2 = (float)x11;
                    L1.y2 = (float)y11;
                }
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L1, (char *) &PL, NULL));
            }
            else if (i == circles_no - 1)
            {
                L1.x1 = circles[circles_no - 1].x;
                L1.y1 = circles[circles_no - 1].y;
                L1.x2 = circles[circles_no - 2].x;
                L1.y2 = circles[circles_no - 2].y;
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L1, (char *) &PL, NULL));

                if (closed) {
                    L2.x1 = circles[0].x;
                    L2.y1 = circles[0].y;
                    L2.x2 = circles[1].x;
                    L2.y2 = circles[1].y;
                }
                else
                {
                    L2.x1 = L1.x1;
                    L2.y1 = L1.y1;
                    ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L1, (char *) &PL0, NULL));
                    koc1 = cos(Pi_/2.0);
                    kos1 = sin(Pi_/2.0);
                    Rotate_Point(kos1, koc1, (double)L1.x1, (double)L1.y1, (double)L1.x2, (double)L1.y2, &x11, &y11);
                    L2.x2 = (float)x11;
                    L2.y2 = (float)y11;
                }
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L2, (char *) &PL1, NULL));
            } else
            {
                L1.x1 = circles[i].x;
                L1.y1 = circles[i].y;
                L1.x2 = circles[i - 1].x;
                L1.y2 = circles[i - 1].y;
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L1, (char *) &PL, NULL));
                L2.x1 = circles[i].x;
                L2.y1 = circles[i].y;
                L2.x2 = circles[i + 1].x;
                L2.y2 = circles[i + 1].y;
                ret = VOID_TO_INT(plugin_ptr(LINEPARAMETERS, (char *) &L2, (char *) &PL1, NULL));
            }

            if (PL1.kat < PL.kat) PL1.kat += 360;
            angle = (PL1.kat + PL.kat) / 2.0;

            koc = cos(Pi_ * (angle) / 180);
            kos = sin(Pi_ * (angle) / 180);

            Rotate_Point(kos, koc, (double)circles[i].x, (double)circles[i].y, (double)circles[i].x+d_radius, (double)circles[i].y, &x11, &y11);

            circles[i].x = (float) x11;
            circles[i].y = (float) y11;
            circles[i].r = (float) radius_min;
        }
    }

    /*
    for (int i=0; i<circles_no; i++)
    {
        printf("x=%f  y=%f  r=%f\n", circles[i].x, circles[i].y, circles[i].r);
    }
    */

    if (circles_no>1)
    {
        if (add_stirrup(circles, circles_no, &clockwise, closed, TRUE))
        {
            //rotating stirrup if necessary
            //if clockwise==-1  polygon points are in clockwise order
            //if clockwise==1 polygon points are in counterclockwise order

            if (clockwise==1) //counterclockwise order
            angle = atan2((double)circles[circles_no-1].y - (double)circles[circles_no-2].y, (double)circles[circles_no-1].x - (double)circles[circles_no-2].x);
            else angle = atan2((double)circles[1].y - (double)circles[0].y, (double)circles[1].x - (double)circles[0].x);

            angle=-nearest_orto(angle);

            kos = sin(angle);
            koc = cos(angle);

            for (int i = 0; i < circles_no; i++) {
                //rotating
                if (clockwise==-1) //clockwise order
                Rotate_Point(kos, koc, circles[0].x, circles[0].y, circles[i].x, circles[i].y, &x11, &y11);
                else Rotate_Point(kos, koc, (double) circles[circles_no-1].x, (double) circles[circles_no-1].y, (double) circles[i].x, (double) circles[i].y, &x11, &y11);
                circles[i].x = (float) x11;
                circles[i].y = (float) y11;
                ////printf("x=%f  y=%f  r=%f\n", circles[i].x, circles[i].y, circles[i].r);
            }

            ret=VOID_TO_INT(plugin_ptr(GET_POSXY, (char*)&X, (char*)&Y, NULL));

            dx = -circles[0].x;
            dy = -circles[0].y;

            for (int i = 0; i < circles_no; i++) {
                //moving
                circles[i].x += dx;
                circles[i].y += dy;
                ////printf("x=%f  y=%f  r=%f\n", circles[i].x, circles[i].y, circles[i].r);
            }


            if (closed)
            {
                //rotating of last circle
                if (clockwise == 1) //counterclockwise order
                {
                    //last arm
                    //angle = atan2((double) circles[circles_no - 1].y - (double) circles[circles_no - 2].y,(double) circles[circles_no - 1].x - (double) circles[circles_no - 2].x);
                    kos = sin(-open_arm_angle);
                    koc = cos(-open_arm_angle);
                    Rotate_Point(kos, koc, (double) circles[circles_no - 2].x, (double) circles[circles_no - 2].y,
                                 (double) circles[circles_no - 1].x, (double) circles[circles_no - 1].y, &x11, &y11);
                    circles[circles_no - 1].x = (float) x11;
                    circles[circles_no - 1].y = (float) y11;
                } else {
                    //first arm
                    //angle = atan2((double)circles[1].y - (double)circles[0].y, (double)circles[1].x - (double)circles[0].x);
                    kos = sin(-open_arm_angle);
                    koc = cos(-open_arm_angle);
                    Rotate_Point(kos, koc, (double) circles[1].x, (double) circles[1].y, (double) circles[0].x,
                                 (double) circles[0].y, &x11, &y11);
                    circles[0].x = (float) x11;
                    circles[0].y = (float) y11;
                }
            }

            ret = add_stirrup(circles, circles_no, &clockwise, closed, FALSE);
            dane000 = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);
            Dimensioning_stirrup(dane000, 0, clockwise, FALSE);
        }

        dane000 = (char *) plugin_ptr(GET_DATA, NULL, NULL, NULL);
        dane_size000 = (long)plugin_ptr(GET_DATA_SIZE, NULL, NULL, NULL);

        ret=VOID_TO_INT(plugin_ptr(BLOKZAP1, dane000, dane000+dane_size000, NULL));
        ret=VOID_TO_INT(plugin_ptr(RESET_ATTRIBUTES, dane000, dane000+dane_size000, NULL));

        p2=Ablok;
        ret=VOID_TO_INT(plugin_ptr(ADREM_BLOK1, dane000, (char*)&p2, NULL));

        BLOK *b=(BLOK*)dane000;

        ret=VOID_TO_INT(plugin_ptr(PLACE_BLOCK, stirrup_str, NULL, NULL));
    }

    finish_stirrup(Ltype);

    //ret = VOID_TO_INT(plugin_ptr(SETMENUP, (char*)menup_p, NULL, NULL));
    //ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_radius, NULL, NULL));
    //ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_anchor, NULL, NULL));
    //ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_diameter, NULL, NULL));
    //ret = VOID_TO_INT(plugin_ptr(DELETE_CLIENT_BITMAP, (char*)&iconno_radius, NULL, NULL));

    free(circles);

    return;
}
