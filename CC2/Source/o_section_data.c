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


#define  __O_SECTION_DATA__
#include <stdio.h>
#include<forwin.h>
#include <stdlib.h>
#include <string.h>
#include "bib_e.h"
#include "message.h"
#include "o_static.h"

extern TMENU mIH_section_US_si;
extern TMENU mU_section_US_si;
extern TMENU mT_section_US_si;
extern TMENU mL_section_US_si;
extern TMENU m2L_section_US_si;
extern TMENU mRT_section_US_si;
extern TMENU mCT_section_US_si;
extern TMENU mET_section_US_si;
extern TMENU mZ_section_US_si;
extern TMENU mVJ_section_US_si;

extern TMENU mIH_section_EU_si;
extern TMENU mU_section_EU_si;
extern TMENU mT_section_EU_si;
extern TMENU mL_section_EU_si;
extern TMENU m2L_section_EU_si;
extern TMENU mRT_section_EU_si;
extern TMENU mCT_section_EU_si;
extern TMENU mET_section_EU_si;
extern TMENU mZ_section_EU_si;
extern TMENU mVJ_section_EU_si;

extern TMENU mIH_section_UK_si;
extern TMENU mU_section_UK_si;
extern TMENU mT_section_UK_si;
extern TMENU mL_section_UK_si;
extern TMENU m2L_section_UK_si;
extern TMENU mRT_section_UK_si;
extern TMENU mCT_section_UK_si;
extern TMENU mET_section_UK_si;
extern TMENU mZ_section_UK_si;
extern TMENU mVJ_section_UK_si;

extern TMENU mtimber_beams_US_si;
extern POLE pmtimber_beams_US_si[];

extern POLE pmWoodMoistureUS[];
extern TMENU mWoodMoistureUS;
extern POLE pmWoodSpeciesUS[];
extern TMENU mWoodSpeciesUS;



extern int  getwsp(TMENU *);
extern void set_menu_level(int menu_l);
extern void simulate_keypress(int key);
extern int Tekst_factory(char *prefix, BOOL repeat);
extern double Jednostki;
extern void set_decimal_format(char *text, double l, double precision);
extern int get_string (char *, char *, int , int , int );
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);
extern BOOL Check_if_Equal (double , double ) ;

extern double dim_precision;
extern PROP_PRECISIONS SI_precisions;
extern PROP_PRECISIONS IMP_precisions;
extern PROP_PRECISIONS *prop_precisions;

extern TEXT TextG;

static int last_property_no=0;
static unsigned char hidden_last=0;

#ifdef LINUX
char *directory="Elements/";
#else
char *directory="Elements\\";
#endif

POLE pmSteelUK[] = {
        {u8"IH", L'I', 775, &mIH_section_UK_si},
        {u8"U", L'U', 776, &mU_section_UK_si},
        {u8"T", L'T', 777, &mT_section_UK_si},
        {u8"L", L'L', 778, &mL_section_UK_si},
        {u8"RT", L'R', 780, &mRT_section_UK_si},
        {u8"CT", L'C', 781, &mCT_section_UK_si},
        {u8"ET", L'E', 782, &mET_section_UK_si},
};


TMENU mSteelUK = { 7,0,0,6,16,8,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelUK,NULL,NULL };


POLE pmSteelEU[] = {
        {u8"IH", L'I', 775, &mIH_section_EU_si},
        {u8"U", L'U', 776, &mU_section_EU_si},
        {u8"T", L'T', 777, &mT_section_EU_si},
        {u8"L", L'L', 778, &mL_section_EU_si},
        {u8"RT", L'R', 780, &mRT_section_EU_si},
        {u8"CT", L'C', 781, &mCT_section_EU_si},
        {u8"ET", L'E', 782, &mET_section_EU_si},
};


TMENU mSteelEU = { 7,0,0,6,16,8,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelEU,NULL,NULL };


POLE pmSteelUS[] = {
        {u8"IH", L'I', 775, &mIH_section_US_si},
        {u8"U", L'U', 776, &mU_section_US_si},
        {u8"T", L'T', 777, &mT_section_US_si},
        {u8"L", L'L', 778, &mL_section_US_si},
        {u8"2L", L'2', 779, &m2L_section_US_si},
        {u8"RT", L'R', 780, &mRT_section_US_si},
        {u8"CT", L'C', 781, &mCT_section_US_si},
        {u8"Z", L'Z', 783, &mZ_section_US_si},
        {u8"VJ", L'V', 784, &mVJ_section_US_si},
};


TMENU mSteelUS = { 9,0,0,6,16,8,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelUS,NULL,NULL };


POLE pmTimberUS[] = {
        {u8"timber Rectangles", L'R', 785, &mtimber_beams_US_si},
};


TMENU mTimberUS = { 1,0,0,6,16,8,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmTimberUS,NULL,NULL };

char *units_system_si="si";
char *units_system_imp="imp";

char *regions[]={"US", "EU", "UK"};

char *get_section_data(char *file_name,  int property_no, char *material, char *section, char *region, char *units_system, char *standard0, char *series0, char *type0, char *species0, char *moisture0)
{
    FILE *f, *fs;
    static char section_data[MaxTextLen]="";
    char data_row[MaxMultitextLen];

    char series[64];
    char description[64];
    char standard[64];
    char manufacturer[64];
    char type[64];
    char measure_system[12];
    char *ptr, *ptr1, *ptr2;
    char par[13][MaxTextLen]={"","","","","","","","","","","","", ""};
    double E,G,d,a;
    char moisture[16];
    char species[36];
    double SG, ER, EL, WML, IB, CT, CP, SP, TP, SH, R;
    double m;
    int ret;
    char ERROR[128];

    double h, b, tw, tf, hi, r1, r2, b1, dw, k, kdes, kdet, k1, ss,
            sf, A, Iy, Iz,Ip, iy, iz,
            ip, Sy_max, Sz_max, Wy, Wz, Ay, Az, It, It_s, Wt, w_max, Iw,
            iw, Ww, Sw_max, c_tf, c_tw, Wpl_y, Wpl_z, Wpl_w, Wpl_y_webs, Wpl_z_flanges,
            apl_y, apl_z, apl_w, Apl_y, Apl_z, Npl, Vpl_y, Vpl_z, Mpl_y, Mpl_z,
             Nu, Gw, Am, V, Am_V, Aw, d0,  w, w1;

    strcpy(section_data,"");

    f=fopen(file_name, "rt");
    if (!f)
    {
        sprintf(ERROR,"%s %s",_CANNOT_OPEN_, file_name);
        ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, "", 11, 1, 62);
        return section_data;
    }

    if (fgets( data_row, MaxMultitextLen, f ) == NULL) //header
    {
        fclose(f);
        sprintf(ERROR,"%s %s",_CANNOT_READ_, file_name);
        ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, "", 11, 1, 62);
        return section_data;
    }

    while( fgets( data_row, MaxMultitextLen, f ) != NULL)
    {
        data_row[strlen(data_row)-1] = '\0';

        //check last appearance of "
        ptr=data_row;
        ptr1=strrchr(data_row, '\"');
        ptr1++;

        //"series" "standard" "type" "description" "manufacturer" "measure_system
        ptr2=strchr(ptr, '\"');
        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        *ptr2='\0';
        strcpy(series,ptr);

        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        *ptr2='\0';
        strcpy(standard,ptr);

        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        *ptr2='\0';
        strcpy(type,ptr);

        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        *ptr2='\0';
        strcpy(description,ptr);

        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        *ptr2='\0';
        strcpy(manufacturer,ptr);

        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        ptr=ptr2+1;
        ptr2=strchr(ptr, '\"');
        *ptr2='\0';
        strcpy(measure_system,ptr);

        if ((strcmp(standard, standard0)==0) && (strcmp(series, series0)==0) && (strcmp(type, type0)==0))
        {
            fclose(f);

            int no=sscanf(ptr1,
                          "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                          &h, &b, &tw, &tf, &hi, &r1, &r2, &b1, &dw, &k,
                          &kdes, &kdet, &k1, &ss, &sf, &A, &Iy, &Iz, &Ip, &iy,
                          &iz, &ip, &Sy_max, &Sz_max, &Wy, &Wz, &Ay, &Az, &It, &It_s,
                          &Wt, &w_max, &Iw, &iw, &Ww, &Sw_max, &c_tf, &c_tw, &Wpl_y, &Wpl_z,
                          &Wpl_w, &Wpl_y_webs, &Wpl_z_flanges, &apl_y, &apl_z, &apl_w, &Apl_y, &Apl_z, &Npl, &Vpl_y,
                          &Vpl_z, &Mpl_y, &Mpl_z, &Nu, &Gw, &Am, &V, &Am_V, &Aw, &d0,
                          &w, &w1);

            if (no<62)
            {
                sprintf(ERROR,"%s %s",_CANNOT_READ_, file_name);
                ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, "", 11, 1, 62);
                return section_data;
            }

            //in case of L with equal flanges
            if (Check_if_Equal(b, 0)) b=h;
            if (Check_if_Equal(Az, 0)) Az=Ay;
            if (Check_if_Equal(Iz, 0)) Iz=Iy;
            if (Check_if_Equal(Wz, 0)) Wz=Wy;

            //#1 h=140 A=16.43 As=8.13 Iy=541.20 Iz=44.92 E=210 G=81 r=0 d=7850 a=11.7e-6 IPE 140
            set_decimal_format(par[0], h, prop_precisions->dim_precision);
            set_decimal_format(par[1], b, prop_precisions->dim_precision);
            set_decimal_format(par[2], A, prop_precisions->A_precision);
            set_decimal_format(par[3], Ay, prop_precisions->A_precision);
            set_decimal_format(par[4], Az, prop_precisions->A_precision);
            set_decimal_format(par[5], Iy, prop_precisions->I_precision);
            set_decimal_format(par[6], Iz, prop_precisions->I_precision);
            set_decimal_format(par[7], Wy, prop_precisions->I_precision);
            set_decimal_format(par[8], Wz, prop_precisions->I_precision);

            if (strcmp(material,"Timber")==0)
            {
                if (strcmp(units_system, units_system_si)==0)
                {
                    if (strcmp(region, "US")==0)
                    {
                        E=11.0;
                        G=0.69;
                        d=350.0;
                        a=5.0e-6;
                        //searching in wood database
#ifdef LINUX
                        fs=fopen("Elements/woods_US_si.csv", "rt");
#else
                        fs=fopen("Elements\\woods_US_si.csv", "rt");
#endif
                        if (!fs)
                        {
                            sprintf(ERROR,"%s %s",_CANNOT_OPEN_, "Elements/woods_US_si.csv");
                            ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, (char *) _DEFAULT_TAKEN_, 11, 1, 62);
                        }
                        else
                        {
                            if (fgets( data_row, MaxMultitextLen, fs ) == NULL) //header
                            {
                                fclose(fs);
                                sprintf(ERROR,"%s %s",_CANNOT_READ_, "Elements/woods_US_si.csv");
                                ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, (char *) _DEFAULT_TAKEN_, 11, 1, 62);
                            }
                            else
                            {
                                while (fgets(data_row, MaxMultitextLen, f) != NULL)
                                {
                                    data_row[strlen(data_row) - 1] = '\0';

                                    //check last appearance of "
                                    ptr = data_row;
                                    ptr1 = strrchr(data_row, '\"');
                                    ptr1++;

                                    //"species" "moisture"
                                    ptr2 = strchr(ptr, '\"');
                                    ptr = ptr2 + 1;
                                    ptr2 = strchr(ptr, '\"');
                                    *ptr2 = '\0';
                                    strcpy(species, ptr);

                                    ptr = ptr2 + 1;
                                    ptr2 = strchr(ptr, '\"');
                                    ptr = ptr2 + 1;
                                    ptr2 = strchr(ptr, '\"');
                                    *ptr2 = '\0';
                                    strcpy(moisture, ptr);

                                    if ((strcmp(species, species0) == 0) && (strcmp(moisture, moisture0) == 0))
                                    {
                                        no = sscanf(ptr1,
                                                    "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                                                    &SG, &ER, &EL, &WML, &IB, &CT, &CP, &SP, &TP, &SH, &R);
                                        if (no < 11)
                                        {
                                            fclose(fs);
                                            sprintf(ERROR,"%s %s",_CANNOT_READ_, "Elements/woods_US_si.csv");
                                            ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, (char *) _DEFAULT_TAKEN_, 11, 1, 62);
                                        } else {
                                            //
                                            E = EL/1000.0;
                                            G = EL/1000.0 * 1.1 * R;
                                            a = 5.0e-6;
                                            //calculating density in [kg/m3]
                                            if (strcmp(moisture0, "Green")==0) m = 100;
                                            else m = 12;
                                            d = (62.4 * (SG / (1 + (SG * 0.009 * m)))) * (1 + (m / 100)) * 16.018;
                                        }
                                        break;
                                    }
                                }
                                fclose(fs);
                            }
                        }
                    }
                    else if (strcmp(region, "EU")==0)
                    {
                        E=11.0;
                        G=0.69;
                        d=350.0;
                        a=5.0e-6;

                    }
                    else
                    {
                        E=11.0;
                        G=0.69;
                        d=350.0;
                        a=5.0e-6;
                    }
                }
                else
                {
                    if (strcmp(region, "US")==0)
                    {
                        E=1595.0;
                        G=100.0;
                        d=3.6127;
                        a=5.0e-6;
#ifdef LINUX
                        fs=fopen("Elements/woods_US_imp.csv", "rt");
#else
                        fs=fopen("Elements\\woods_US_imp.csv", "rt");
#endif
                        if (!fs)
                        {
                            sprintf(ERROR,"%s %s",_CANNOT_OPEN_, "Elements/woods_US_imp.csv");
                            ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, (char *) _DEFAULT_TAKEN_, 11, 1, 62);
                        }
                        else
                        {
                            if (fgets( data_row, MaxMultitextLen, fs ) == NULL) //header
                            {
                                fclose(fs);
                                sprintf(ERROR,"%s %s",_CANNOT_READ_, "Elements/woods_US_imp.csv");
                                ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, (char *) _DEFAULT_TAKEN_, 11, 1, 62);
                            }
                            else
                            {
                                while (fgets(data_row, MaxMultitextLen, f) != NULL)
                                {
                                    data_row[strlen(data_row) - 1] = '\0';

                                    //check last appearance of "
                                    ptr = data_row;
                                    ptr1 = strrchr(data_row, '\"');
                                    ptr1++;

                                    //"species" "moisture"
                                    ptr2 = strchr(ptr, '\"');
                                    ptr = ptr2 + 1;
                                    ptr2 = strchr(ptr, '\"');
                                    *ptr2 = '\0';
                                    strcpy(species, ptr);

                                    ptr = ptr2 + 1;
                                    ptr2 = strchr(ptr, '\"');
                                    ptr = ptr2 + 1;
                                    ptr2 = strchr(ptr, '\"');
                                    *ptr2 = '\0';
                                    strcpy(moisture, ptr);

                                    if ((strcmp(species, species0) == 0) && (strcmp(moisture, moisture0) == 0))
                                    {
                                        no = sscanf(ptr1,
                                                    "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                                                    &SG, &ER, &EL, &WML, &IB, &CT, &CP, &SP, &TP, &SH, &R);
                                        if (no < 11)
                                        {
                                            fclose(fs);
                                            sprintf(ERROR,"%s %s",_CANNOT_READ_, "Elements/woods_US_si.csv");
                                            ret = ask_question(1, "", (char *) confirm, "",(char *) ERROR, 12, _DEFAULT_TAKEN_, 11, 1, 62);
                                        } else {
                                            //
                                            E = EL * 1000.0;  //in kpis
                                            G = EL * 1000.0 * 1.1 * R;
                                            a = 5.0e-6;
                                            //calculating density in [lb/in3]
                                            if (strcmp(moisture0, "Green")==0) m = 100;
                                            else m = 12;
                                            d = (62.4 * (SG / (1 + (SG * 0.009 * m)))) * (1 + (m / 100)) * 0.0005787;
                                        }
                                        break;
                                    }
                                }
                                fclose(fs);
                            }
                        }
                    }
                    else if (strcmp(region, "EU")==0)
                    {
                        E=1595.0;
                        G=100.0;
                        d=3.6127;
                        a=5.0e-6;
                    }
                    else
                    {
                        E=1595.0;
                        G=100.0;
                        d=3.6127;
                        a=5.0e-6;
                    }
                }
            }
            else if (strcmp(material,"Steel")==0)
            {
                if (strcmp(units_system, units_system_si)==0)
                {
                    if (strcmp(region, "US")==0)
                    {
                        E=200.0;
                        G=77.2;
                        d=7850.0;
                        a=1.4e-5;
                    }
                    else if (strcmp(region, "EU")==0)
                    {
                        E=210.0;
                        G=81.0;
                        d=7850.0;
                        a=1.2e-5;
                    }
                    else
                    {
                        E=210.0;
                        G=81.0;
                        d=7850.0;
                        a=1.2e-5;
                    }

                }
                else
                {
                    if (strcmp(region, "US")==0)
                    {
                        E=29000.0;
                        G=11200.0;
                        d=0.284;
                        a=1.4e-5;
                    }
                    else if (strcmp(region, "EU")==0)
                    {
                        E=30450.0;
                        G=11745.0;
                        d=0.284;
                        a=1.2e-5;
                    }
                    else
                    {
                        E=30450.0;
                        G=11745.0;
                        d=0.284;
                        a=1.2e-5;
                    }

                }
            }

            sprintf(par[9], "%g", E);
            sprintf(par[10], "%g", G);
            sprintf(par[11], "%g", d);
            sprintf(par[12], "%g", a);

            sprintf(section_data,"#%d h=%s b=%s A=%s Asy=%s Asz=%s Iy=%s Iz=%s Wy=%s Wz=%s E=%s G=%s r=0 d=%s a=%s  %s", property_no, par[0],par[1],par[2],par[3],par[4],par[5],par[6],par[7],par[8], par[9], par[10], par[11], par[12], type0);
            return section_data;
        }
    }

    fclose(f);
    return section_data;
}

char *get_section(int property_no, char *material, char *region0, char *section, char *standard, char *series, char *type, char *species, char *moisture)   //"Steel","UK",section,standard,series,type
{
    static char section_data[MaxTextLen];
    char *units_system;
    char file_name[MAXPATH];
    char *region=region0;
    //searching data in data base
    /////////////////////////
    if (Jednostki==1)   //mm
    {
        units_system=units_system_si;
        prop_precisions=&SI_precisions;
    }
    else if (Jednostki==10) //cm
    {
        units_system=units_system_si;
        prop_precisions=&SI_precisions;
    }
    else if (Jednostki==1000)  //m
    {
        units_system=units_system_si;
        prop_precisions=&SI_precisions;
    }
    else if (Jednostki==1000000) //km
    {
        units_system=units_system_si;
        prop_precisions=&SI_precisions;
    }
    else if (Jednostki==25.4) //"
    {
        units_system=units_system_imp;
        prop_precisions=&IMP_precisions;
    }
    else if (Jednostki==304.8) //'
    {
        units_system=units_system_imp;
        prop_precisions=&IMP_precisions;
    }
    else if (Jednostki==914.4) //yd
    {
        units_system=units_system_imp;
        prop_precisions=&IMP_precisions;
    }
    else if (Jednostki==1609344) //mi
    {
        units_system=units_system_imp;
        prop_precisions=&IMP_precisions;
    }
    else
    {
        units_system=units_system_si;
        prop_precisions=&SI_precisions;
    }
    /////////////////////////
    //if (strcmp(region, u8"US")==0) region=regions[0];
    //else if (strcmp(region, u8"EU")==0) region=regions[1];
    //else if (strcmp(region, u8"UK")==0) region=regions[2];
    if (strcmp(region, u8"США")==0) region=regions[0];
    else if (strcmp(region, u8"ЄС")==0) region=regions[1];
    else if (strcmp(region, u8"Великобританія")==0) region=regions[2];
    else if (strcmp(region, u8"EE.UU")==0) region=regions[0];
    else if (strcmp(region, u8"UE")==0) region=regions[1];
    else if (strcmp(region, u8"Reino Unido")==0) region=regions[2];

    if (strcmp(material,"Timber")==0)
    {
        sprintf(file_name, "%s%s_%s_%s.csv", directory, "timber_beams", region, units_system);
    }
    else if (strcmp(material,"Steel")==0)
    {
        sprintf(file_name, "%s%s_section_%s_%s.csv", directory, section, region, units_system);
    }

    strcpy(section_data, get_section_data(file_name,  property_no, material, section, region, units_system, standard, series, type, species, moisture));

    return section_data;
}

void SteelUS(void)
{   int n;
    char section[MaxTextLen];
    char standard[MaxTextLen];
    char series[MaxTextLen];
    char type[MaxTextLen];
    TMENU *m_section;
    TMENU *m_standard;
    PTMENU *m_series;
    //TMENU *m_type;
    char section_data[MaxTextLen];
    char property_no_str[64];
    int property_no;
    unsigned int hidden;

    //get property no
    sprintf(property_no_str,"%d",last_property_no+1);
    if (!get_string (property_no_str, "1234567890", 16, 0, 211))
    {
        return;
    }
    if (strlen(property_no_str)==0)
    {
        return;
    }

    property_no=atoi(property_no_str);

    set_menu_level(0);
    simulate_keypress(ENTER);
    n = getwsp(&mSteelUS) ;

    if (n>0)
    {
        strcpy(section, pmSteelUS[mSteelUS.foff + mSteelUS.poz].txt);
        m_section = pmSteelUS[mSteelUS.foff + mSteelUS.poz].menu;
        m_standard = (*m_section->pola)[m_section->foff + m_section->poz].menu;
        strcpy(standard, (*m_section->pola)[m_section->foff + m_section->poz].txt);
        m_series = (PTMENU *)(*m_standard->pola)[m_standard->foff + m_standard->poz].menu;
        strcpy(series, (*m_standard->pola)[m_standard->foff + m_standard->poz].txt);
        strcpy(type, (*m_series->pola)[m_series->foff + m_series->poz].txt);

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "US", section, standard, series, type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "US", section, standard, series, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }

    return;
}

void TimberUS(void)
{   int n;
    char section[MaxTextLen];
    char standard[MaxTextLen];
    char series[MaxTextLen];
    char type[MaxTextLen];
    char moisture[16];
    char species[36];
    TMENU *m_section;
    TMENU *m_standard;
    PTMENU *m_series;
    char section_data[MaxTextLen];
    char property_no_str[64];
    int property_no;
    unsigned int hidden;

    //get property no
    sprintf(property_no_str,"%d",last_property_no+1);
    if (!get_string (property_no_str, "1234567890", 16, 0, 211))
    {
        return;
    }
    if (strlen(property_no_str)==0)
    {
        return;
    }

    property_no=atoi(property_no_str);

    set_menu_level(0);
    simulate_keypress(ENTER);
    //n = getwsp(&mTimberUS) ;
    n = getwsp(&mtimber_beams_US_si);
    if (n>0)
    {
        strcpy(moisture, (pmWoodMoistureUS[mWoodMoistureUS.foff + mWoodMoistureUS.poz].txt));
        strcpy(species, (pmWoodSpeciesUS[mWoodSpeciesUS.foff + mWoodSpeciesUS.poz].txt+2));

        strcpy(section, pmTimberUS[0].txt);
        m_section = pmTimberUS[0].menu;
        m_standard = (*m_section->pola)[m_section->foff + m_section->poz].menu;
        strcpy(standard, (*m_section->pola)[m_section->foff + m_section->poz].txt);
        m_series = (PTMENU *)(*m_standard->pola)[m_standard->foff + m_standard->poz].menu;
        strcpy(series, (*m_standard->pola)[m_standard->foff + m_standard->poz].txt);
        strcpy(type, (*m_series->pola)[m_series->foff + m_series->poz].txt);

        printf("\"Timber\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "US", section, standard, series, type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Timber", "US", section, standard, series, type, species, moisture));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }

    return;
}

void SteelEU(void)
{
    int n;
    char section[MaxTextLen];
    char standard[MaxTextLen];
    char series[MaxTextLen];
    char type[MaxTextLen];
    TMENU *m_section;
    TMENU *m_standard;
    PTMENU *m_series;
    char section_data[MaxTextLen];
    char property_no_str[64];
    int property_no;
    unsigned int hidden;

    //get property no
    sprintf(property_no_str,"%d",last_property_no+1);
    if (!get_string (property_no_str, "1234567890", 16, 0, 211))
    {
        return;
    }
    if (strlen(property_no_str)==0)
    {
        return;
    }

    property_no=atoi(property_no_str);

    set_menu_level(0);
    simulate_keypress(ENTER);
    n = getwsp(&mSteelEU) ;

    if (n>0)
    {
        strcpy(section, pmSteelEU[mSteelEU.foff + mSteelEU.poz].txt);
        m_section = pmSteelEU[mSteelEU.foff + mSteelEU.poz].menu;
        m_standard = (*m_section->pola)[m_section->foff + m_section->poz].menu;
        strcpy(standard, (*m_section->pola)[m_section->foff + m_section->poz].txt);
        m_series = (PTMENU *)(*m_standard->pola)[m_standard->foff + m_standard->poz].menu;
        strcpy(series, (*m_standard->pola)[m_standard->foff + m_standard->poz].txt);
        strcpy(type, (*m_series->pola)[m_series->foff + m_series->poz].txt);

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "EU", section, standard, series, type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "EU", section, standard, series, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }

    return;
}

void SteelUK(void)
{
    int n;
    char section[MaxTextLen];
    char standard[MaxTextLen];
    char series[MaxTextLen];
    char type[MaxTextLen];
    TMENU *m_section;
    TMENU *m_standard;
    PTMENU *m_series;
    char section_data[MaxTextLen];
    char property_no_str[64];
    int property_no;
    unsigned int hidden;

    //get property no
    sprintf(property_no_str,"%d",last_property_no+1);
    if (!get_string (property_no_str, "1234567890", 16, 0, 211))
    {
        return;
    }
    if (strlen(property_no_str)==0)
    {
        return;
    }

    property_no=atoi(property_no_str);

    set_menu_level(0);
    simulate_keypress(ENTER);
    n = getwsp(&mSteelUK);
    if (n>0)
    {
        strcpy(section, pmSteelUK[mSteelUK.foff + mSteelUK.poz].txt);
        m_section = pmSteelUK[mSteelUK.foff + mSteelUK.poz].menu;
        m_standard = (*m_section->pola)[m_section->foff + m_section->poz].menu;
        strcpy(standard, (*m_section->pola)[m_section->foff + m_section->poz].txt);
        m_series = (PTMENU *)(*m_standard->pola)[m_standard->foff + m_standard->poz].menu;
        strcpy(series, (*m_standard->pola)[m_standard->foff + m_standard->poz].txt);
        strcpy(type, (*m_series->pola)[m_series->foff + m_series->poz].txt);

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "UK", section, standard, series, type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "UK", section, standard, series, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }

    return;
}

////strategy of choosing section from database
//searching foff + poz of following menus:
//mMaterial   poz=0 steel, poz=1 timber
//if steel:
//mSteel 0: US, 1: EU, 2: UK
//if US:
//mSteelUS 0: IH.....  9: VJ
//if IH
//mIH_section_US_si is showing file (..._si.cws or ..._imp.cvs) e.g. IH_section_US_si
//txt indicates standard e.g. "Euronorm 53-62; DIN 1025-4"
//poz indicates menu e.g. mIH_section_EU_si_0
//poz of mIH_section_EU_si_0 indicates section e.g. HEM and menu mIH_section_EU_si_0_0
//poz of mIH_section_EU_si_0_0 indicates type e.g. HEM 100
// so we search in file IH_section_US_si.cvs or IH_section_US_imp.cve the row with standard:
//     "Euronorm 53-62; DIN 1025-4", section "HEM" and exact type "HEM 100"

#undef __O_SECTION_DATA__