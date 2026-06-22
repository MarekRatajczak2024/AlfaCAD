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
#include <math.h>
#include "bib_e.h"
#include "message.h"
#include "o_static.h"
#include "rysuj_e.h"

#include "o_section_block.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

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

extern TMENU mIH_section_AU_si;
extern TMENU mU_section_AU_si;
extern TMENU mT_section_AU_si;
extern TMENU mL_section_AU_si;
extern TMENU m2L_section_AU_si;
extern TMENU mRT_section_AU_si;
extern TMENU mCT_section_AU_si;
extern TMENU mET_section_AU_si;
extern TMENU mZ_section_AU_si;
extern TMENU mVJ_section_AU_si;

extern TMENU mIH_section_CA_si;
extern TMENU mU_section_CA_si;
extern TMENU mT_section_CA_si;
extern TMENU mL_section_CA_si;
extern TMENU m2L_section_CA_si;
extern TMENU mRT_section_CA_si;
extern TMENU mCT_section_CA_si;
extern TMENU mET_section_CA_si;
extern TMENU mZ_section_CA_si;
extern TMENU mVJ_section_CA_si;

extern TMENU mIH_section_CN_si;
extern TMENU mU_section_CN_si;
extern TMENU mT_section_CN_si;
extern TMENU mL_section_CN_si;
extern TMENU mRT_section_CN_si;
//extern TMENU mCT_section_CA_si;
extern TMENU mZ_section_CN_si;

extern TMENU mtimber_beams_US_si;
extern POLE pmtimber_beams_US_si[];
extern TMENU mtimber_beams_CA_si;
extern POLE pmtimber_beams_CA_si[];

extern POLE pmWoodMoistureUS[];
extern TMENU mWoodMoistureUS;
extern POLE pmWoodSpeciesUS[];
extern TMENU mWoodSpeciesUS;


extern POLE pmSteelGradeUS[];
extern TMENU mSteelGradeUS;
extern POLE pmConcreteClassUS[];
extern TMENU mConcreteClassUS;
extern POLE pmConcreteUS[];
extern TMENU mConcreteUS;

extern POLE pmSteelGradeEU[];
extern TMENU mSteelGradeEU;
extern POLE pmConcreteClassEU[];
extern TMENU mConcreteClassEU;
extern POLE pmConcreteEU[];
extern TMENU mConcreteEU;

extern int  getwsp(TMENU *);
extern void set_menu_level(int menu_l);
extern void simulate_keypress(int key);
extern int Tekst_factory(char *prefix, BOOL repeat);
extern double Jednostki;
extern void set_decimal_format(char *text, double l, double precision);
extern int get_string (char *, char *, int , int , int );
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);
extern BOOL Check_if_Equal (double , double ) ;
extern double calculate_Jx(double h, double b);
extern double get_rc_alpha(double width, double height);

extern double dim_precision;
extern PROP_PRECISIONS SI_precisions;
extern PROP_PRECISIONS IMP_precisions;
extern PROP_PRECISIONS *prop_precisions;
extern double stress_precision;

extern TEXT TextG;

static int last_property_no=0;
static unsigned char hidden_last=0;

extern BOOL Semaphore;

#ifdef LINUX
char *directory="Elements/";
#else
char *directory="Elements\\";
#endif


const double g_in_s2 = 386.088556; // Acceleration of gravity in inches/sec²

POLE pmSteelAU[] = {
        {u8"IH", L'I', 775, &mIH_section_AU_si}, 
        {u8"U", L'U', 776, &mU_section_AU_si},
        {u8"RT", L'R', 780, &mRT_section_AU_si}, 
        {u8"CT", L'C', 781, &mCT_section_AU_si},
};

TMENU mSteelAU = { 4,0,0,6,16,8, TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelAU,NULL,NULL };

POLE pmSteelCA[] = {
        {u8"IH", L'I', 775, &mIH_section_CA_si},
        {u8"U", L'U', 776, &mU_section_CA_si},
        {u8"T", L'T', 777, &mT_section_CA_si},
        {u8"L", L'L', 778, &mL_section_CA_si},
        {u8"2L", L'2', 779, &m2L_section_CA_si},
        {u8"RT", L'R', 780, &mRT_section_CA_si},
        {u8"CT", L'C', 781, &mCT_section_CA_si},
        {u8"ET", L'E', 782, &mET_section_CA_si},
        {u8"Z", L'Z', 783, &mZ_section_CA_si},
};

TMENU mSteelCA = { 9,0,0,6,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelCA,NULL,NULL };

POLE pmSteelCN[] = {
        {u8"IH", L'I', 775, &mIH_section_CN_si}, //
        {u8"U", L'U', 776, &mU_section_CN_si},  //
        {u8"T", L'T', 777, &mT_section_CN_si},  //
        {u8"L", L'L', 778, &mL_section_CN_si},  //
        {u8"RT", L'R', 780, &mRT_section_CN_si},  //
        //{u8"CT", L'C', 781, &mCT_section_CN_si},
        {u8"Z", L'Z', 783, &mZ_section_CN_si},  //
};

TMENU mSteelCN = { 6,0,0,6,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelCN,NULL,NULL };


POLE pmSteelUK[] = {
        {u8"IH", L'I', 775, &mIH_section_UK_si},
        {u8"U", L'U', 776, &mU_section_UK_si},
        {u8"T", L'T', 777, &mT_section_UK_si},
        {u8"L", L'L', 778, &mL_section_UK_si},
        {u8"RT", L'R', 780, &mRT_section_UK_si},
        {u8"CT", L'C', 781, &mCT_section_UK_si},
        {u8"ET", L'E', 782, &mET_section_UK_si},
};


TMENU mSteelUK = { 7,0,0,6,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelUK,NULL,NULL };


POLE pmSteelEU[] = {
        {u8"IH", L'I', 775, &mIH_section_EU_si},
        {u8"U", L'U', 776, &mU_section_EU_si},
        {u8"T", L'T', 777, &mT_section_EU_si},
        {u8"L", L'L', 778, &mL_section_EU_si},
        {u8"RT", L'R', 780, &mRT_section_EU_si},
        {u8"CT", L'C', 781, &mCT_section_EU_si},
        {u8"ET", L'E', 782, &mET_section_EU_si},
};


TMENU mSteelEU = { 7,0,0,6,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelEU,NULL,NULL };


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


TMENU mSteelUS = { 9,0,0,6,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmSteelUS,NULL,NULL };


POLE pmTimberUS[] = {
        {u8"timber Rectangles", L'R', 785, &mtimber_beams_US_si},
};


TMENU mTimberUS = { 1,0,0,6,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmTimberUS,NULL,NULL };

POLE pmTimberCA[] = {
        {u8"timber Rectangles", L'R', 785, &mtimber_beams_CA_si},
};


TMENU mTimberCA = { 1,0,0,6,16,8,TADD | ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmTimberCA,NULL,NULL };

char *units_system_si="si";
char *units_system_imp="imp";

char *regions[]={"US", "EU", "UK"};


char *get_section_data(char *file_name,  int property_no, char *material, char *section, char *region, char *units_system, char *standard0, char *series0, char *manufacturer0, char *type0, char *species0, char *moisture0)
{
    FILE *f, *fs;
    static char section_data[MaxTextLen]="";
    char data_row[MaxMultitextLen];

    char series[64];
    char *ptr_tm;
    char description[64];
    char standard[64];
    char manufacturer[64];
    char type[64];
    char measure_system[12];
    char *ptr, *ptr1, *ptr2;
    char par[18][MaxTextLen]={"","","","","","","","","","","","", "", "", "", "", "", ""};
    char par_[MaxTextLen];
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
             Nu, Gw, Am, V, Am_V, Aw, d0,  w, w1, bt, bb, bf, t, ha, ba, ab, c, c1, r3, ri, sw;


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

        if ((strcmp(standard, standard0)==0) && (strcmp(series, series0)==0) && (strcmp(type, type0)==0) && ((manufacturer0==NULL) || (strcmp(manufacturer, manufacturer0)==0))) {
            fclose(f);

            //extensions
            t = 0;
            ha = 0;
            ba = 0;
            ab = 0;
            c = 0;
            c1 = 0;
            r3 = 0;
            ri = 0;
            bt = 0;
            bb = 0;
            bf = 0;

            int no = sscanf(ptr1,
                            "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                            &h, &b, &tw, &tf, &hi, &r1, &r2, &b1, &dw, &k,
                            &kdes, &kdet, &k1, &ss, &sf, &A, &Iy, &Iz, &Ip, &iy,
                            &iz, &ip, &Sy_max, &Sz_max, &Wy, &Wz, &Ay, &Az, &It, &It_s,
                            &Wt, &w_max, &Iw, &iw, &Ww, &Sw_max, &c_tf, &c_tw, &Wpl_y, &Wpl_z,
                            &Wpl_w, &Wpl_y_webs, &Wpl_z_flanges, &apl_y, &apl_z, &apl_w, &Apl_y, &Apl_z, &Npl, &Vpl_y,
                            &Vpl_z, &Mpl_y, &Mpl_z, &Nu, &Gw, &Am, &V, &Am_V, &Aw, &d0,
                            &w, &w1, &bt, &bb, &bf, &t, &ha, &ba, &ab, &c, &c1, &r3, &ri, &sw);

            if (no < 62) {
                sprintf(ERROR, "%s %s", _CANNOT_READ_, file_name);
                ret = ask_question(1, "", (char *) confirm, "", (char *) ERROR, 12, "", 11, 1, 62);
                return section_data;
            }

            //in case of L with equal flanges
            if (Check_if_Equal(b, 0)) b = h;
            if (Check_if_Equal(Az, 0)) Az = Ay;
            if (Check_if_Equal(Iz, 0)) Iz = Iy;
            if (Check_if_Equal(Wz, 0)) Wz = Wy;
            
            //generating of profile block
            ret = create_profile_block(units_system, series0, manufacturer0, type0, h, b, tw, tf, r1, r2, sf / 100.0, bt, bb, bf, t, ha, ba, ab, c, c1, r3, ri, sw / 100.0);

            //#1 h=140 A=16.43 As=8.13 Iy=541.20 Iz=44.92 E=210 G=81 r=0 d=7850 a=11.7e-6 IPE 140
            set_decimal_format(par[0], h, prop_precisions->dim_precision);
            set_decimal_format(par[1], b, prop_precisions->dim_precision);
            set_decimal_format(par[2], A, prop_precisions->A_precision);
            set_decimal_format(par[3], Ay, prop_precisions->A_precision);
            set_decimal_format(par[4], Az, prop_precisions->A_precision);

            if (Check_if_Equal(Apl_y, 0.)) sprintf(par[5], "");
            else {
                set_decimal_format(par_, Apl_y, prop_precisions->A_precision);
                sprintf(par[5], " Aply=%s", par_);
            }
            if (Check_if_Equal(Apl_z, 0.)) sprintf(par[6], "");
            else {
                set_decimal_format(par_, Apl_z, prop_precisions->A_precision);
                sprintf(par[6], " Aplz=%s", par_);
            }
            set_decimal_format(par[7], Iy, prop_precisions->I_precision);
            set_decimal_format(par[8], Iz, prop_precisions->I_precision);
            //set_decimal_format(par[9], Ip, prop_precisions->I_precision);
            set_decimal_format(par[9], It, prop_precisions->I_precision);  //used It instead of Ip
            set_decimal_format(par[10], Wy, prop_precisions->I_precision);
            set_decimal_format(par[11], Wz, prop_precisions->I_precision);

            if (Check_if_Equal(Wt, 0.)) sprintf(par[12], "");
            else {
                set_decimal_format(par_, Wt, prop_precisions->I_precision);
                sprintf(par[12], " Wt=%s", par_);
            }

            if (strcmp(material, "Timber") == 0)
            {
                // -----------------------------------------------------------
                // 1. CHOOSE ACTIVE SYSTEM MATRIX DEFAULT FALLBACKS
                // -----------------------------------------------------------
                if (strcmp(units_system, units_system_si) == 0) {
                    E = 11.0;
                    G = 0.69;
                    d = 350.0;    // GPa and kg/m³ (Natively pure mass space for SI)
                    a = 5.0e-6;
                } else {
                    E = 1595000.0;
                    G = 100000.0;
                    d = 0.00003274; // FIX: High-precision lbf/in³/g mass density (0.0126 / 386.09)
                    a = 5.0e-6;    // psi and lbf/in³/g
                }

                // -----------------------------------------------------------
                // 2. PARSE THE DYNAMIC REGIONAL DATA SYSTEM
                // -----------------------------------------------------------
                if (strcmp(region, "US") == 0)
                {
                    // Select matching database files based on system flags
#ifdef LINUX
                    const char *csv_path = (strcmp(units_system, units_system_si) == 0)
                                           ? "Elements/woods_US_si.csv"
                                           : "Elements/woods_US_imp.csv";

#else
                    // Windows Static Selection: Simple, direct, and completely stable
                    const char *csv_path = (strcmp(units_system, units_system_si) == 0)
                                            ? "Elements\\woods_US_si.csv"
                                            : "Elements\\woods_US_imp.csv";
#endif

                    fs = fopen(csv_path, "rt");

                    if (!fs) {
                        sprintf(ERROR, "%s %s", _CANNOT_OPEN_, csv_path);
                        ret = ask_question(1, "", (char *) confirm, "", (char *) ERROR, 12, (char *) _DEFAULT_TAKEN_, 11, 1, 62);
                    } else {
                        if (fgets(data_row, MaxMultitextLen, fs) == NULL) { // Skip Header Row
                            fclose(fs);
                            sprintf(ERROR, "%s %s", _CANNOT_READ_, csv_path);
                            ret = ask_question(1, "", (char *) confirm, "", (char *) ERROR, 12, (char *) _DEFAULT_TAKEN_, 11, 1, 62);
                        } else {
                            // FIXED: Reading explicitly from the open 'fs' stream pointer loop
                            while (fgets(data_row, MaxMultitextLen, fs) != NULL) {
                                data_row[strlen(data_row) - 1] = '\0';

                                // Parse text columns tracking string quotes
                                ptr = data_row;
                                ptr1 = strrchr(data_row, '\"');
                                ptr1++;

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

                                if ((strcmp(species, species0) == 0) && (strcmp(moisture, moisture0) == 0)) {
                                    no = sscanf(ptr1, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                                                &SG, &ER, &EL, &WML, &IB, &CT, &CP, &SP, &TP, &SH, &R);

                                    if (no < 11) {
                                        fclose(fs);
                                        sprintf(ERROR, "%s %s", _CANNOT_READ_, csv_path);
                                        ret = ask_question(1, "", (char *) confirm, "", (char *) ERROR, 12, (char *) _DEFAULT_TAKEN_, 11, 1, 62);
                                    } else {
                                        int m = (strcmp(moisture0, "Green") == 0) ? 100 : 12;

                                        if (strcmp(units_system, units_system_si) == 0) {
                                            // --- SI TARGET ASSIGNMENT ---
                                            E = EL / 1000.0;                       // Converst to clean GPa display unit
                                            G = (EL / 1000.0) * 1.1 * R;           // Orthotropic G ratio mapping
                                            a = 5.0e-6;
                                            d = (62.4 * (SG / (1.0 + (SG * 0.009 * m)))) * (1.0 + (m / 100.0)) * 16.018; // kg/m³
                                        } else {
                                            // --- IMPERIAL TARGET ASSIGNMENT ---
                                            E = EL * 1000000.0;                    // million psi (Mpsi) scaled cleanly to pure psi
                                            G = (EL * 1000000.0) * 1.1 * R;        // Pure torsional psi
                                            a = 5.0e-6;

                                            // Calculate Imperial Weight Density cleanly in lbf/in³:
                                            // 1. Find lbs/ft³ via USDA formula baseline
                                            double density_lbs_ft3 = (62.4 * (SG / (1.0 + (SG * 0.009 * m)))) * (1.0 + (m / 100.0));
                                            // 3. Divide by gravity to yield pure lbf/in^3/g mass density
                                            d = (density_lbs_ft3 / 1728.0) / g_in_s2;
                                        }
                                    }
                                    break; // Data matched and saved, break loop cleanly
                                }
                            }
                            fclose(fs);
                        }
                    }
                } else if (strcmp(region, "EU") == 0) {
                    if (strcmp(units_system, units_system_si) == 0) {
                        E = 11.0;   // GPa
                        G = 0.69;   // GPa
                        d = 420.0;  // 420 kg/m³ (Clean, realistic physical weight)
                        a = 5.0e-6;
                    } else {
                        // NEW REPLICABLE EU IMPERIAL FALLBACK: 1.6 Mpsi and coherent weight density
                        E = 1600000.0;  // psi
                        G = 100000.0;   // psi
                        d = 0.00003930; // True mass density in lbf/in^3/g (0.015173 / 386.0886)
                        a = 5.0e-6;
                    }
                } else {
                    if (strcmp(units_system, units_system_si) == 0) {
                        E = 11.0;
                        G = 0.69;
                        d = 420.0;
                        a = 5.0e-6;
                    } else {
                        E = 1600000.0;
                        G = 100000.0;
                        d = 0.00003930; // True mass density in lbf/in^3/g (0.015173 / 386.0886)
                        a = 5.0e-6;
                    }
                }
            } else if (strcmp(material, "Steel") == 0)
            {
                if (strcmp(units_system, units_system_si) == 0) {
                    if (strcmp(region, "US") == 0) {
                        E = 200.0;
                        G = 77.2;
                        d = 7850.0;
                        a = 1.4e-5;
                    } else if (strcmp(region, "EU") == 0) {
                        E = 210.0;
                        G = 81.0;
                        d = 7850.0;
                        a = 1.2e-5;
                    } else  //globally
                    {
                        E = 210.0;
                        G = 81.0;
                        d = 7850.0;
                        a = 1.2e-5;
                    }

                } else {
                    if (strcmp(region, "US") == 0) {
                        E = 29000000.0;  // 29,000,000 psi
                        G = 11200000.0;  // 11,200,000 psi
                        d = 0.000734588;       // 0.284 lbf/in³/g (Weight density)
                        a = 1.4e-5;
                    } else if (strcmp(region, "EU") == 0) {
                        E = 30450000.0;  // 30,450,000 psi
                        G = 11745000.0;  // 11,745,000 psi
                        d = 0.000734588;       // 0.284 lbf/in³/g (Weight density)
                        a = 1.2e-5;
                    } else //globally
                    {
                        E = 30450000.0;  // 30,450,000 psi
                        G = 11745000.0;  // 11,745,000 psi
                        d = 0.000734588;       // 0.284 lbf/in³/g (Weight density)
                        a = 1.2e-5;
                    }

                }
            }
            else if (strcmp(material, "Concrete") == 0)  //TO DO
            {
                //u8"Concrete class", u8"Rebar grade"
                //u8"Klasa betonu", u8"Klasa stali zbrojeniowej"
                //u8"Клас бетону", u8"Клас арматури"
                //u8"Clase de hormigón / concreto", "Tipo de acero"
                //TEMPORARY:
                char *concrete_class="C25/30";
                char *rebar_grade="B400B";
                if (strcmp(units_system, units_system_si) == 0)
                {
                    // ===========================================================
                    // 🔬 SI MODE DECK: DISPLAY VALUE TARGETS = GPa & kg/m³
                    // ===========================================================
                    // Default values to act as safe fallbacks (C25/30 Profile)
                    E = 31.0;   G = 12.9;  d = 2500.0;  a = 1.0e-5;

                    if (strcmp(concrete_class, "C20/25") == 0) { E = 30.0; G = 12.5; }
                    else if (strcmp(concrete_class, "C25/30") == 0) { E = 31.0; G = 12.917; }
                    else if (strcmp(concrete_class, "C30/37") == 0) { E = 33.0; G = 13.75; }
                    else if (strcmp(concrete_class, "C35/45") == 0) { E = 34.0; G = 14.167; }
                    else if (strcmp(concrete_class, "C40/50") == 0) { E = 35.0; G = 14.583; }
                    else if (strcmp(concrete_class, "C45/55") == 0) { E = 36.0; G = 15.0; }
                    else if (strcmp(concrete_class, "C50/60") == 0) { E = 37.0; G = 15.417; }

                        // Also map ACI selections cleanly if a user switches code contexts in SI
                    else if (strcmp(concrete_class, "3000 psi") == 0) { E = 21.526; G = 8.969; }
                    else if (strcmp(concrete_class, "4000 psi") == 0) { E = 24.856; G = 10.357; }
                    else if (strcmp(concrete_class, "5000 psi") == 0) { E = 27.789; G = 11.579; }
                    else if (strcmp(concrete_class, "6000 psi") == 0) { E = 30.441; G = 12.684; }
                    else if (strcmp(concrete_class, "8000 psi") == 0) { E = 35.151; G = 14.646; }

                    d = 2500.0; // Standard Reinforced Concrete Weight Density is universally 2500 kg/m³
                    a = 1.0e-5;
                }
                else
                {
                    // ===========================================================
                    // 🇺🇸 IMPERIAL MODE DECK: DISPLAY VALUE TARGETS = psi & lbf/in³
                    // ===========================================================
                    // Default values to act as safe fallbacks (4000 psi Profile)
                    E = 3605000.0;  G = 1502083.0;

                    if (strcmp(concrete_class, "3000 psi") == 0) { E = 3122019.0; G = 1300841.0; }
                    else if (strcmp(concrete_class, "4000 psi") == 0) { E = 3605000.0; G = 1502083.0; }
                    else if (strcmp(concrete_class, "5000 psi") == 0) { E = 4030509.0; G = 1679379.0; }
                    else if (strcmp(concrete_class, "6000 psi") == 0) { E = 4415065.0; G = 1839610.0; }
                    else if (strcmp(concrete_class, "8000 psi") == 0) { E = 5098235.0; G = 2124265.0; }

                        // Also map Eurocode classes cleanly if an overseas engineer prefers psi metrics
                    else if (strcmp(concrete_class, "C20/25") == 0) { E = 4351132.0; G = 1812972.0; }
                    else if (strcmp(concrete_class, "C25/30") == 0) { E = 4496170.0; G = 1873404.0; }
                    else if (strcmp(concrete_class, "C30/37") == 0) { E = 4786245.0; G = 1994269.0; }
                    else if (strcmp(concrete_class, "C35/45") == 0) { E = 4931283.0; G = 2054701.0; }
                    else if (strcmp(concrete_class, "C40/50") == 0) { E = 5076321.0; G = 2115134.0; }
                    else if (strcmp(concrete_class, "C45/55") == 0) { E = 5221359.0; G = 2175566.0; }
                    else if (strcmp(concrete_class, "C50/60") == 0) { E = 5366397.0; G = 2235999.0; }

                    double weight_density_concrete = 0.090394; // lbf/in³ (~156 lb/ft³) // Standard Reinforced Concrete Weight Density: (156.0 lbs/ft³) / 1728 = 0.090394 lbf/in³
                    // Safely scales your database weight density (lbs/in³) down to lbf/in³/g
                    // to match your steel and timber mass matrices perfectly!
                    d = weight_density_concrete / g_in_s2;
                    a = 1.0e-5;
                }

                // --- REBAR MATERIAL PROPERTIES GENERATOR ---
                double design_rebar_fy; // The fyd/fy variable passed downstream to your RC designer matrix

                if (strcmp(units_system, units_system_si) == 0)
                {
                    // ===========================================================
                    // 🔬 SI DESIGN INTERFACE LAYOUT (Output Target: MPa)
                    // ===========================================================
                    // Standard European structural default fallback (B500B Baseline)
                    double design_rebar_fy = 434.78; // 500 / 1.15

                    if (strcmp(rebar_grade, "B400B") == 0)       { design_rebar_fy = 347.83; }
                    else if (strcmp(rebar_grade, "B500B") == 0)  { design_rebar_fy = 434.78; }
                    else if (strcmp(rebar_grade, "B500C") == 0)  { design_rebar_fy = 434.78; }

                        // Cross-code conversion matching: If a metric engineer manually selects a US Grade
                    else if (strcmp(rebar_grade, "Grade 40") == 0) { design_rebar_fy = 275.79; }
                    else if (strcmp(rebar_grade, "Grade 60") == 0) { design_rebar_fy = 413.69; }
                    else if (strcmp(rebar_grade, "Grade 75") == 0) { design_rebar_fy = 517.11; }
                    else if (strcmp(rebar_grade, "Grade 80") == 0) { design_rebar_fy = 551.58; }
                }
                else
                {
                    // ===========================================================
                    // 🇺🇸 IMPERIAL DESIGN INTERFACE LAYOUT (Output Target: psi)
                    // ===========================================================
                    // Standard American structural default fallback (Grade 60 Baseline)
                    double design_rebar_fy = 60000.0; // Stays fully un-factored as required by ASCE/ACI code loops

                    if (strcmp(rebar_grade, "Grade 40") == 0)      { design_rebar_fy = 40000.0; }
                    else if (strcmp(rebar_grade, "Grade 60") == 0) { design_rebar_fy = 60000.0; }
                    else if (strcmp(rebar_grade, "Grade 75") == 0) { design_rebar_fy = 75000.0; }
                    else if (strcmp(rebar_grade, "Grade 80") == 0) { design_rebar_fy = 80000.0; }

                        // Cross-code conversion matching: If an Imperial engineer models an overseas B-grade
                    else if (strcmp(rebar_grade, "B400B") == 0)    { design_rebar_fy = 50444.0; }
                    else if (strcmp(rebar_grade, "B500B") == 0)    { design_rebar_fy = 63063.0; }
                    else if (strcmp(rebar_grade, "B500C") == 0)    { design_rebar_fy = 63063.0; }
                }

// Format 'design_rebar_fy' cleanly into your data card template variable slot
                sprintf(par[11], "%.1f", design_rebar_fy);
// Yields clean layout text tokens like fyd=434.8 or fyd=60000

            }

            sprintf(par[13], "%g", E);
            sprintf(par[14], "%g", G);
            sprintf(par[15], "%g", d);
            sprintf(par[16], "%g", a);

            sprintf(section_data, "#%d h=%s b=%s A=%s Asy=%s Asz=%s%s%s Iy=%s Iz=%s Jx=%s Wy=%s Wz=%s%s\\n E=%s G=%s r=0 d=%s a=%s  %s", property_no, par[0], par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11], par[12], par[13], par[14], par[15], par[16], type0);
            return section_data;
        }
    }

    fclose(f);
    return section_data;
}

char *get_section(int property_no, char *material, char *region0, char *section, char *standard, char *series, char *manufacturer, char *type, char *species, char *moisture)   //"Steel","UK",section,standard,series,type
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

    strcpy(section_data, get_section_data(file_name,  property_no, material, section, region, units_system, standard, series, manufacturer, type, species, moisture));

    return section_data;
}

char *get_concrete_section(int property_no, char *material, char *region, SECTION_PAR *section_par, char *section, int section_no, char *concrete_class, char *steel_grade)
{
    static char section_data[MaxTextLen];
    char *units_system;
    double E, G, d, a, Ax, Asy, Asz, Aply, Aplz, Jx, Iy, Iz, Wy, Wz, Wt;
    char par[32][32];
    char par_[32];
    char sk[MaxTextLen]="";
    char prompt[MaxTextLen]="";
    int retval_no=0;
    double buf_ret [5] ;
    char *dim;
    int ret;
    double h0,b0,w0,t0,ro0,ri0;
    double h=0.,b=0.,w=0.,t=0.,ro=0.,ri=0., ccover=0.;
    char block_section[32];
    char profile[64];
    char *default_cover;

    double fck = 0.0;
    double fcd = 0.0;
    double fyk = 0.0;
    double fyd = 0.0;

    // =========================================================================
    // 1. UNIT SYSTEM IDENTIFICATION
    // =========================================================================
    if (Jednostki == 1)        // mm
    {
        units_system = units_system_si;
        prop_precisions = &SI_precisions;
    }
    else if (Jednostki == 10)   // cm
    {
        units_system = units_system_si;
        prop_precisions = &SI_precisions;
    }
    else if (Jednostki == 1000)  // m
    {
        units_system = units_system_si;
        prop_precisions = &SI_precisions;
    }
    else if (Jednostki == 1000000) // km
    {
        units_system = units_system_si;
        prop_precisions = &SI_precisions;
    }
    else if (Jednostki == 25.4) // " (Inch)
    {
        units_system = units_system_imp;
        prop_precisions = &IMP_precisions;
    }
    else if (Jednostki == 304.8) // ' (Foot)
    {
        units_system = units_system_imp;
        prop_precisions = &IMP_precisions;
    }
    else if (Jednostki == 914.4) // yd
    {
        units_system = units_system_imp;
        prop_precisions = &IMP_precisions;
    }
    else if (Jednostki == 1609344) // mi
    {
        units_system = units_system_imp;
        prop_precisions = &IMP_precisions;
    }
    else
    {
        units_system = units_system_si;
        prop_precisions = &SI_precisions;
    }

    if (units_system==units_system_si)
    {
        dim=(char*)_mm_;
        default_cover=_default_cover_mm_;
    }
    else
    {
        dim=(char*)_inch_;
        default_cover=_default_cover_in_;
    }

    double r1=0., r2=0., sf=0., bt=0., bb=0., bf=0., t_=0., ha=0., ba=0., ab=0., cc=0., c1=0., r3=0., ri_=0., sw=0.;

    //asking for parameters
    switch (section_no)
    {
    case 0:  //rectangular
        retval_no=3;
        sprintf(prompt,"%s%s%s%s:",(char*)_R_SECTION_,default_cover,(char*)_in_,dim);
        if (!get_string_str (sk, "", MaxTextLen, 0, prompt)) return NULL;
        if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 2)
        {
            return NULL;
        }
        d = buf_ret [0] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        h = d;
        d = buf_ret [1] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        b = d;
        if (retval_no>2)
        {
            d = buf_ret [2] ;
            if ( d <= 0 )
            {
                ErrList (30);
                return NULL;
            }
            else ccover = buf_ret [2] ;
        }
        strcpy(block_section,"R-beam");
        set_decimal_format(par[0], h, prop_precisions->dim_precision);
        set_decimal_format(par[1], b, prop_precisions->dim_precision);
        sprintf(profile,"R %sx%s", par[0], par[1]);
        break;
    case 1:  //I-section
        retval_no=5;
        sprintf(prompt,"%s%s%s%s:",(char*)_I_SECTION_,default_cover,(char*)_in_,dim);
        if (!get_string_str (sk, "", MaxTextLen, 0, prompt)) return NULL;
        if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 4)
        {
            return NULL;
        }
        d = buf_ret [0] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        h = d;
        d = buf_ret [1] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        b = d;
        d = buf_ret [2] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        w = d;
        d = buf_ret [3] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        t = d;
        if (retval_no>4)
        {
            d = buf_ret [4] ;
            if ( d <= 0 )
            {
                ErrList (30);
                return NULL;
            }
            else ccover = buf_ret [4] ;
        }
        strcpy(block_section,"I-beam");
        set_decimal_format(par[0], h, prop_precisions->dim_precision);
        set_decimal_format(par[1], b, prop_precisions->dim_precision);
        set_decimal_format(par[2], w, prop_precisions->dim_precision);
        set_decimal_format(par[3], t, prop_precisions->dim_precision);
        sprintf(profile,"I %sx%sx%sx%s", par[0], par[1],par[3], par[4]);
        break;
    case 2:  //T-section
        retval_no=5;
        sprintf(prompt,"%s%s%s%s:",(char*)_T_SECTION_,default_cover,(char*)_in_,dim);
        if (!get_string_str (sk, "", MaxTextLen, 0, prompt)) return NULL;
        if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 4)
        {
            return NULL;
        }
        d = buf_ret [0] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        h = d;
        d = buf_ret [1] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        b = d;
        d = buf_ret [2] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        w = d;
        d = buf_ret [3] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        t = d;
        if (retval_no>4)
        {
            d = buf_ret [4] ;
            if ( d <= 0 )
            {
                ErrList (30);
                return NULL;
            }
            else ccover = buf_ret [4] ;
        }
        strcpy(block_section,"T-beam");
        set_decimal_format(par[0], h, prop_precisions->dim_precision);
        set_decimal_format(par[1], b, prop_precisions->dim_precision);
        set_decimal_format(par[2], w, prop_precisions->dim_precision);
        set_decimal_format(par[3], t, prop_precisions->dim_precision);
        sprintf(profile,"T %sx%sx%sx%s", par[0], par[1],par[3], par[4]);
        break;
    case 3:  //CHS-section
        retval_no=3;
        sprintf(prompt,"%s%s%s%s:",(char*)_CT_SECTION_,default_cover,(char*)_in_,dim);
        if (!get_string_str (sk, "", MaxTextLen, 0, prompt)) return NULL;
        if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 2)
        {
            return NULL;
        }
        d = buf_ret [0] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        h=b=d;
        ro = d/2.;
        d = buf_ret [1] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        ri = ro-d;
        t_ = d;   //t_ for for block purpose
        if (retval_no>2)
        {
            d = buf_ret [2] ;
            if ( d <= 0 )
            {
                ErrList (30);
                return NULL;
            }
            else ccover = buf_ret [2] ;
        }
        if (ri<0.) return NULL;  //WARNING MESSAGE
        strcpy(block_section,"CHS-beam");
        set_decimal_format(par[0], ro, prop_precisions->dim_precision);
        set_decimal_format(par[1], ri, prop_precisions->dim_precision);
        sprintf(profile,"CHS %s/%s", par[0], par[1]);
        break;
    case 4:  //SHS-section
        retval_no=3;
        sprintf(prompt,"%s%s%s%s:",(char*)_ST_SECTION_,default_cover,(char*)_in_,dim);
        if (!get_string_str (sk, "", MaxTextLen, 0, prompt)) return NULL;
        if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 2)
        {
            return NULL;
        }
        d = buf_ret [0] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        h = b = d;
        d = buf_ret [1] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        t = t_ = d;
        if (retval_no>2)
        {
            d = buf_ret [2] ;
            if ( d <= 0 )
            {
                ErrList (30);
                return NULL;
            }
            else ccover = buf_ret [2] ;
        }
        strcpy(block_section,"SHS-beam");
        set_decimal_format(par[0], h, prop_precisions->dim_precision);
        set_decimal_format(par[1], t, prop_precisions->dim_precision);
        sprintf(profile,"SHS %sx%s", par[0], par[1]);
        break;
    case 5:  //RHS-section
        retval_no=4;
        sprintf(prompt,"%s%s%s%s:",(char*)_RT_SECTION_,default_cover,(char*)_in_,dim);
        if (!get_string_str (sk, "", MaxTextLen, 0, prompt)) return NULL;
        if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 3)
        {
            return NULL;
        }
        d = buf_ret [0] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        h = d;
        d = buf_ret [1] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        b = d;
        d = buf_ret [2] ;
        if ( d <= 0 )
        {
            ErrList (30);
            return NULL;
        }
        t = t_ = d;
        if (retval_no>3)
        {
            d = buf_ret [3] ;
            if ( d <= 0 )
            {
                ErrList (30);
                return NULL;
            }
            else ccover = buf_ret [3] ;
        }
        strcpy(block_section,"RHS-beam");
        set_decimal_format(par[0], h, prop_precisions->dim_precision);
        set_decimal_format(par[1], t, prop_precisions->dim_precision);
        sprintf(profile,"RHS %sx%s", par[0], par[1]);
        break;
    case 6:  //plate
        {
            retval_no=2;
            sprintf(prompt,"%s%s%s%s:",(char*)_PLATE_SECTION_,default_cover,(char*)_in_,dim);
            if (!get_string_str (sk, "", MaxTextLen, 0, prompt)) return NULL;
            if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
            {
                return NULL;
            }
            d = buf_ret [0] ;
            if ( d <= 0 )
            {
                ErrList (30);
                return NULL;
            }
            h = d;
            if (retval_no>1)
            {
                d = buf_ret [1] ;
                if ( d <= 0 )
                {
                    ErrList (30);
                    return NULL;
                }
                else ccover = buf_ret [1] ;
            }
            strcpy(block_section,"plate-section");
            set_decimal_format(par[0], h, prop_precisions->dim_precision);
            sprintf(profile,"plate %s", par[0]);

            double DRAWING_PLATE_B=25.;  //25 mm on drawing
            double LINE_UP=5.; //section line 5 mm up
            double LINE_Z_SIDE=5.; //section zig-zac side
            double LINE_Z_DOWN=1.; //section zig-zac down

            //fixed width for block, what is DRAWING_PLATE_B
            b=DRAWING_PLATE_B*SkalaF;  //expresed in mm
            //lineup
            ha=LINE_UP*SkalaF;
            ab=LINE_Z_SIDE*SkalaF;
            ba=LINE_Z_DOWN*SkalaF;
            if (units_system == units_system_imp)
            {
                b/=25.4;  //expressed in inch
                ha/=25.4;
                ab/=25.4;
                ba/=25.4;
            }

            // --- PLATE LAYOUT (Vertical Break Line) ---
            // Controlled by height 'h'
            if ((4.0 * ba) > (0.4 * h)) {
                ba = (0.4 * h) / 4.0;
                ab = ba * 5.0; // Keeps the 5:1 aspect ratio of your design
            }
        }
        break;
    case 7:  //shield
        {
            retval_no=2;
            sprintf(prompt,"%s%s%s%s:",(char*)_SHIELD_SECTION_,default_cover,(char*)_in_,dim);
            if (!get_string_str (sk, "", MaxTextLen, 0, prompt)) return NULL;
            if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
            {
                return NULL;
            }
            d = buf_ret [0] ;
            if ( d <= 0 )
            {
                ErrList (30);
                return NULL;
            }
            b = d;
            if (retval_no>1)
            {
                d = buf_ret [1] ;
                if ( d <= 0 )
                {
                    ErrList (30);
                    return NULL;
                }
                else ccover = buf_ret [1] ;
            }
            strcpy(block_section,"shield-section");
            set_decimal_format(par[0], b, prop_precisions->dim_precision);
            sprintf(profile,"shield %s", par[0]);

            double DRAWING_SHIELD_H=25.;  //25 mm on drawing
            double LINE_UP=5.; //section line 5 mm up
            double LINE_Z_SIDE=5.; //section zig-zac side
            double LINE_Z_DOWN=1.; //section zig-zac down

            //fixed width for block, what is DRAWING_SHIELD_H
            h=DRAWING_SHIELD_H*SkalaF;  //expresed in mm
            //lineup
            ha=LINE_UP*SkalaF;
            ab=LINE_Z_SIDE*SkalaF;
            ba=LINE_Z_DOWN*SkalaF;
            if (units_system == units_system_imp)
            {
                h/=25.4;  //expressed in inch
                ha/=25.4;
                ab/=25.4;
                ba/=25.4;
            }

            // --- SHIELD LAYOUT (Horizontal Break Line) ---
            // Controlled by width 'b'
            if ((4.0 * ba) > (0.4 * b)) {
                ba = (0.4 * b) / 4.0;
                ab = ba * 5.0; // Keeps the 5:1 aspect ratio of your design
            }

        }
        break;
    default:
        return NULL;
        break;
    }

    h0=h;  //we leave original h and b intact
    b0=b;
    w0=w;
    t0=t;
    ro0=ro;
    ri0=ri;

    if (units_system == units_system_si)
    {
        //converting mm to cm;
        h/=10.;
        b/=10.;
        w/=10.;
        t/=10.0;
        ro/=10.;
        ri/=10.;
    }
    
    //generating of profile block
    //preparing data

    ret = create_profile_block(units_system, block_section, region, profile, h0, b0, w0, t0, r1, r2, sf / 100.0, bt, bb, bf, t_, ha, ba, ab, cc, c1, r3, ri_, sw / 100.0);

    // =========================================================================
    // 2. GEOMETRICAL MOMENTS & PROPERTIES (PURE GEOMETRY STEP)
    // =========================================================================
    switch (section_no)
    {
    case 0:  // Rectangle
        {
            Ax = h * b;
            Asy = Asz = Ax;
            Aply = Aplz = Ax / 1.5;
            Jx = calculate_Jx(h, b);
            Iy = pow(h, 3) * b / 12.;
            Iz = h * pow(b, 3) / 12.;
            Wz = h * pow(b, 2) / 6.;
            Wy = pow(h, 2) * b / 6.;

            double min_dim = min(b, h);
            double max_dim = max(b, h);
            double alpha = get_rc_alpha(b, h);
            Wt = alpha * (min_dim * min_dim) * max_dim;
        }
        break;
    case 1: // I-Shape
        {
            Ax = b * h - (h - 2. * t) * (b - w);
            Asy = 1.64 * b * t;
            Asz = h * w;
            Aply = Aplz = 0.0;  //undefined
            Jx = (1 / 3.) * (2. * b * pow(t, 3) + h * pow(w, 3));
            Iy = (1 / 12.) * (b * pow(h, 3) - (b - w) * pow((h - 2. * t), 3));
            Iz = (1 / 12.) * (2. * t * pow(b, 3) + (h - 2. * t) * pow(w, 3));

            Wy = Iy / (h / 2.);
            Wz = Iz / (b / 2.);

            double max_t = (t > w) ? t : w;
            Wt = Jx / max_t;

        }
        break;
    case 2: // T-Shape
        {
            Ax = b * t + w * (h - t);
            double y_top = (b * t * (t / 2.0) + w * (h - t) * (t + (h - t) / 2.0)) / Ax;
            double y_bot = h - y_top;

            Asy = (5.0 / 6.0) * b * t;
            Asz = (5.0 / 6.0) * w * h;

            Aply = Aplz = 0.0;  //undefined

            Iy = (b * pow(t, 3) / 12.0) + b * t * pow(y_top - (t / 2.0), 2)
                 + (w * pow(h - t, 3) / 12.0) + w * (h - t) * pow(t + (h - t) / 2.0 - y_top, 2);

            Iz = (t * pow(b, 3) / 12.0) + ((h - t) * pow(w, 3) / 12.0);
            Jx = (1.0 / 3.0) * (b * pow(t, 3) + (h - t) * pow(w, 3));

            Wy = Iy / max(y_top, y_bot);
            Wz = Iz / (b / 2.0);

            double max_thick = (t > w) ? t : w;
            Wt = Jx / max_thick;
        }
        break;
    case 3: // Circular tube
        Ax = Pi_ * (pow(ro, 2) - pow(ri, 2));
        Asy = Asz = Ax / (0.54414 + 2.97294 * (ri / ro) - 1.51899 * pow((ri / ro), 2));
        Aply = Aplz = 0.0;   //undefined

        Jx = (1 / 2.) * Pi_ * (pow(ro, 4) - pow(ri, 4));
        Iy = Iz = (1 / 4.) * Pi_ * (pow(ro, 4) - pow(ri, 4));

        Wy = Iy / ro;
        Wz = Iz / ro;
        Wt = Jx / ro;

        break;
    case 4: // Square tube
        Ax = pow(b, 2) - pow((b - 2 * t), 2);
        Asy = Asz = Ax / (2.39573 - 0.25009 * (t / b) - 7.89675 * pow((t / b), 2));
        Aply = Aplz = 0.0;   //undefined

        Jx = pow((b - t), 3) * t;
        Iy = Iz = (1 / 12.) * (pow(b, 4) - pow((b - 2. * t), 4));

        Wy = Iy / (b / 2.);
        Wz = Iz / (b / 2.);
        Wt = Jx / t;
        break;
    case 5: // Box (Rectangular tube)
        {
            double a_box = b;
            double b_box = h;
            double t_box = t;

            Ax = a_box * b_box - (a_box - 2 * t_box) * (b_box - 2 * t_box);

            if (a_box >= b_box)
            {
                Asy = Ax / (0.93498 - 1.28084 * (t_box / b_box) + 1.36441 * (b_box / a_box) + 0.00295 * pow((a_box / b_box), 2) + 0.25797 * (t_box * a_box / pow(b_box, 2)));
                Asz = Ax / (1.63544 - 8.34935 * (t_box / b_box) + 0.60125 * (a_box / b_box) + 0.41403 * pow((a_box / b_box), 2) + 4.95373 * (t_box * a_box / pow(b_box, 2)));
            }
            else
            {
                Asy = Ax / (1.63544 - 8.34935 * (t_box / a_box) + 0.60125 * (b_box / a_box) + 0.41403 * pow((b_box / a_box), 2) + 4.95373 * (t_box * b_box / pow(a_box, 2)));
                Asz = Ax / (0.93498 - 1.28084 * (t_box / a_box) + 1.36441 * (a_box / b_box) + 0.00295 * pow((b_box / a_box), 2) + 0.25797 * (t_box * b_box / pow(a_box, 2)));
            }

            Aply = Aplz = 0.0;   //undefined

            Jx = 2 * t_box * pow((a_box - t_box), 2) * pow((b_box - t_box), 2) / (a_box + b_box - 2 * t_box);
            Iy = (1 / 12.) * (a_box * pow(b_box, 3) - (a_box - 2 * t_box) * pow((b_box - 2 * t_box), 3));
            Iz = (1 / 12.) * (pow(a_box, 3) * b_box - pow((a_box - 2 * t_box), 3) * (b_box - 2 * t_box));

            Wy = Iy / (b_box / 2.);
            Wz = Iz / (a_box / 2.);
            Wt = Jx / t_box;
        }
        break;
    case 6: // plate
        //nothing to calculate
        break;
    case 7: // shield
        //nothing to calculate
        break;
    default:
        return NULL;
        break;
    }

    // =========================================================================
    // Geometry processing complete. Moving on to Section 3: Material Handling...
    // =========================================================================
        // =========================================================================
    // 3. MATERIAL STRATEGIES & STRING PRE-PARSING FALLBACKS
    // =========================================================================
    char *concrete_class0 = "C25/30";
    char *steel_grade0 = "B400B";
    if (strlen(concrete_class) == 0) concrete_class = concrete_class0;
    if (strlen(steel_grade) == 0)   steel_grade = steel_grade0;

    if (strcmp(units_system, units_system_si) == 0)
    {
        // =====================================================================
        // 🔬 SI MODE DECK: DISPLAY VALUE TARGETS = GPa & kg/m³
        // =====================================================================
        // Fallback default assignments (EU C25/30 Profile)
        E = 31.0; G = 12.9;
        fcd = 25.0; fck = 0.0;

        if (strcmp(concrete_class, "C20/25") == 0)      { E = 30.0; G = 12.5;   fcd = 20.0; fck = 0.0; }
        else if (strcmp(concrete_class, "C25/30") == 0) { E = 31.0; G = 12.917; fcd = 25.0; fck = 0.0; }
        else if (strcmp(concrete_class, "C30/37") == 0) { E = 33.0; G = 13.75;  fcd = 30.0; fck = 0.0; }
        else if (strcmp(concrete_class, "C35/45") == 0) { E = 34.0; G = 14.167; fcd = 35.0; fck = 0.0; }
        else if (strcmp(concrete_class, "C40/50") == 0) { E = 35.0; G = 14.583; fcd = 40.0; fck = 0.0; }
        else if (strcmp(concrete_class, "C45/55") == 0) { E = 36.0; G = 15.0;   fcd = 45.0; fck = 0.0; }
        else if (strcmp(concrete_class, "C50/60") == 0) { E = 37.0; G = 15.417; fcd = 50.0; fck = 0.0; }

        // Cross-code tracking if a US class is explicitly evaluated in SI Metrics (fck used, fcd = 0)
        else if (strcmp(concrete_class, "3000 psi") == 0) { E = 21.526; G = 8.969;  fcd = 0.0; fck = 20.68; }
        else if (strcmp(concrete_class, "4000 psi") == 0) { E = 24.856; G = 10.357; fcd = 0.0; fck = 27.58; }
        else if (strcmp(concrete_class, "5000 psi") == 0) { E = 27.789; G = 11.579; fcd = 0.0; fck = 34.47; }
        else if (strcmp(concrete_class, "6000 psi") == 0) { E = 30.441; G = 12.684; fcd = 0.0; fck = 41.37; }
        else if (strcmp(concrete_class, "8000 psi") == 0) { E = 35.151; G = 14.646; fcd = 0.0; fck = 55.16; }

        d = 2500.0;
        a = 1.0e-5;

        // --- REBAR PARAMETER SUB-BLOCK (SI) ---
        // Default standard European fallback (B500B Baseline)
        fyd = 500.0; fyk = 0.0;

        if (strcmp(steel_grade, "B400B") == 0)       { fyd = 400.0; fyk = 0.0; }
        else if (strcmp(steel_grade, "B500B") == 0)  { fyd = 500.0; fyk = 0.0; }
        else if (strcmp(steel_grade, "B500C") == 0)  { fyd = 500.0; fyk = 0.0; }

        // Cross-code parsing for US Grades mapped into SI units (fyk used, fyd = 0)
        else if (strcmp(steel_grade, "Grade 40") == 0) { fyd = 0.0; fyk = 275.79; }
        else if (strcmp(steel_grade, "Grade 60") == 0) { fyd = 0.0; fyk = 413.69; }
        else if (strcmp(steel_grade, "Grade 75") == 0) { fyd = 0.0; fyk = 517.11; }
        else if (strcmp(steel_grade, "Grade 80") == 0) { fyd = 0.0; fyk = 551.58; }
    }
    else
    {
        // =====================================================================
        // 🇺🇸 IMPERIAL MODE DECK: DISPLAY VALUE TARGETS = psi & lbf/in³
        // =====================================================================
        // Fallback default assignments (US 4000 psi Profile)
        E = 3605000.0; G = 1502083.0;
        fck = 4000.0; fcd = 0.0;

        if (strcmp(concrete_class, "3000 psi") == 0)      { E = 3122019.0; G = 1300841.0; fck = 3000.0; fcd = 0.0; }
        else if (strcmp(concrete_class, "4000 psi") == 0) { E = 3605000.0; G = 1502083.0; fck = 4000.0; fcd = 0.0; }
        else if (strcmp(concrete_class, "5000 psi") == 0) { E = 4030509.0; G = 1679379.0; fck = 5000.0; fcd = 0.0; }
        else if (strcmp(concrete_class, "6000 psi") == 0) { E = 4415065.0; G = 1839610.0; fck = 6000.0; fcd = 0.0; }
        else if (strcmp(concrete_class, "8000 psi") == 0) { E = 5098235.0; G = 2124265.0; fck = 8000.0; fcd = 0.0; }

        // Cross-code tracking if a Eurocode class is evaluated in Imperial Metrics (fcd used, fck = 0)
        else if (strcmp(concrete_class, "C20/25") == 0) { E = 4351132.0; G = 1812972.0; fck = 0.0; fcd = 2900.75; }
        else if (strcmp(concrete_class, "C25/30") == 0) { E = 4496170.0; G = 1873404.0; fck = 0.0; fcd = 3625.94; }
        else if (strcmp(concrete_class, "C30/37") == 0) { E = 4786245.0; G = 1994269.0; fck = 0.0; fcd = 4351.13; }
        else if (strcmp(concrete_class, "C35/45") == 0) { E = 4931283.0; G = 2054701.0; fck = 0.0; fcd = 5076.32; }
        else if (strcmp(concrete_class, "C40/50") == 0) { E = 5076321.0; G = 2115134.0; fck = 0.0; fcd = 5801.51; }
        else if (strcmp(concrete_class, "C45/55") == 0) { E = 5221359.0; G = 2175566.0; fck = 0.0; fcd = 6526.70; }
        else if (strcmp(concrete_class, "C50/60") == 0) { E = 5366397.0; G = 2235999.0; fck = 0.0; fcd = 7251.89; }

        double weight_density_concrete = 0.090394; // lbf/in³ (~156 lb/ft³)
        d = weight_density_concrete / g_in_s2;     // Yields 0.000234128
        a = 1.0e-5;

        // --- REBAR PARAMETER SUB-BLOCK (IMPERIAL) ---
        // Default standard American fallback (Grade 60 Baseline)
        fyd = 0.0; fyk = 60000.0;

        if (strcmp(steel_grade, "Grade 40") == 0)      { fyd = 0.0; fyk = 40000.0; }
        else if (strcmp(steel_grade, "Grade 60") == 0) { fyd = 0.0; fyk = 60000.0; }
        else if (strcmp(steel_grade, "Grade 75") == 0) { fyd = 0.0; fyk = 75000.0; }
        else if (strcmp(steel_grade, "Grade 80") == 0) { fyd = 0.0; fyk = 80000.0; }

        // Cross-code parsing for European structural steel mapped into Imperial units (fyd used, fyk = 0)
        else if (strcmp(steel_grade, "B400B") == 0)    { fyd = 58015.1; fyk = 0.0; }
        else if (strcmp(steel_grade, "B500B") == 0)    { fyd = 72518.9; fyk = 0.0; }
        else if (strcmp(steel_grade, "B500C") == 0)    { fyd = 72518.9; fyk = 0.0; }
    }

    // =========================================================================
    // 4. BUFFER SERIALIZATION & DATA FORMATTING
    // =========================================================================
    set_decimal_format(par[0], h0, prop_precisions->dim_precision);  //original h and b
    set_decimal_format(par[1], b0, prop_precisions->dim_precision);
    set_decimal_format(par[2], Ax, prop_precisions->A_precision);
    set_decimal_format(par[3], Asy, prop_precisions->A_precision);
    set_decimal_format(par[4], Asz, prop_precisions->A_precision);

    if (Check_if_Equal(Aply, 0.))
    {
        sprintf(par[5], "");
    }
    else
    {
        set_decimal_format(par_, Aply, prop_precisions->A_precision);
        sprintf(par[5], " Aply=%s", par_);
    }

    if (Check_if_Equal(Aplz, 0.))
    {
        sprintf(par[6], "");
    }
    else
    {
        set_decimal_format(par_, Aplz, prop_precisions->A_precision);
        sprintf(par[6], " Aplz=%s", par_);
    }

    set_decimal_format(par[7], Iy, prop_precisions->I_precision);
    set_decimal_format(par[8], Iz, prop_precisions->I_precision);
    set_decimal_format(par[9], Jx, prop_precisions->I_precision);
    set_decimal_format(par[10], Wy, prop_precisions->I_precision);
    set_decimal_format(par[11], Wz, prop_precisions->I_precision);

    if (Check_if_Equal(Wt, 0.))
    {
        sprintf(par[12], "");
    }
    else
    {
        set_decimal_format(par_, Wt, prop_precisions->I_precision);
        sprintf(par[12], " Wt=%s", par_);
    }

    sprintf(par[13], "%g", E);
    sprintf(par[14], "%g", G);
    sprintf(par[15], "%g", d);
    sprintf(par[16], "%g", a);

    // Precise conditioning formatting for Concrete limits
    if (Check_if_Equal(fcd, 0.))
    {
        sprintf(par[17], "");
    }
    else
    {
        set_decimal_format(par_, fcd, prop_precisions->I_precision); // Safely maps precision sizing
        sprintf(par[17], " fcd=%s", par_);
    }

    if (Check_if_Equal(fck, 0.))
    {
        sprintf(par[18], "");
    }
    else
    {
        set_decimal_format(par_, fck, prop_precisions->I_precision);
        sprintf(par[18], " fck=%s", par_);
    }

    // Precise conditioning formatting for Steel limits
    if (Check_if_Equal(fyd, 0.))
    {
        sprintf(par[19], "");
    }
    else
    {
        set_decimal_format(par_, fyd, prop_precisions->I_precision);
        sprintf(par[19], " fyd=%s", par_);
    }

    if (Check_if_Equal(fyk, 0.))
    {
        sprintf(par[20], "");
    }
    else
    {
        set_decimal_format(par_, fyk, prop_precisions->I_precision);
        sprintf(par[20], " fyk=%s", par_);
    }

    if (Check_if_Equal(ccover, 0.))
    {
        sprintf(par[21], "");
    }
    else
    {
        set_decimal_format(par_, ccover, prop_precisions->dim_precision);
        sprintf(par[21], " c=%s", par_);
    }

    // =========================================================================
    // Ready for the final token aggregation / sprintf(section_data, ...) line!
    // =========================================================================
    switch (section_no)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        sprintf(section_data, "#%d RC h=%s b=%s A=%s Asy=%s Asz=%s%s%s Iy=%s Iz=%s Jx=%s Wy=%s Wz=%s%s\\n E=%s G=%s r=0 d=%s a=%s  %s%s%s%s%s", property_no, par[0], par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11], par[12], par[13], par[14], par[15], par[16], par[17], par[18],  par[19], par[20], par[21]);
        break;
    case 6:
        sprintf(section_data, "#%d RC h=%s E=%s G=%s r=0 d=%s a=%s  %s%s%s%s%s", property_no, par[0], par[13], par[14], par[15], par[16], par[17], par[18],  par[19], par[20], par[21]);
        break;
    case 7:
        sprintf(section_data, "#%d RC t=%s E=%s G=%s r=0 d=%s a=%s  %s%s%s%s%s", property_no, par[1], par[13], par[14], par[15], par[16], par[17], par[18],  par[19], par[20], par[21]);
        break;
    }
    section_par->h=h;
    section_par->b=b;
    section_par->w=w;
    section_par->t=t;
    section_par->ro=ro;
    section_par->ri=ri;

    return section_data;
}


void SteelUS(void)
{   int n;
    char section[MaxTextLen];
    char standard[MaxTextLen];
    char series[MaxTextLen];
    //char manufacturer[MaxTextLen];
    char type[MaxTextLen];
    TMENU *m_section;
    TMENU *m_standard;
    PTMENU *m_series;
    //TMENU *m_type;
    char section_data[MaxTextLen];
    char property_no_str[64];
    int property_no;
    unsigned int hidden;
    char *ptrsz_temp0;
    char *ptrsz_temp_tadd;
    char *manufacturer;

    Semaphore = FALSE;

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

        manufacturer=NULL;
        if (m_standard->flags&TADD)
        {
            ptrsz_temp0=(*m_standard->pola)[m_standard->foff + m_standard->poz].txt;

            ptrsz_temp_tadd = ptrsz_temp0 + strlen(ptrsz_temp0)+1;
            if (strlen(ptrsz_temp_tadd) > 0)
            {
                manufacturer = strstr(ptrsz_temp_tadd, u8"™");
                if (manufacturer != NULL) manufacturer += 3;
            }
        }

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "US", section, standard, series, manufacturer!=NULL ? manufacturer : "" , type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "US", section, standard, series, manufacturer, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
}

void SteelCA(void)
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
    char *ptrsz_temp0;
    char *ptrsz_temp_tadd;
    char *manufacturer;

    Semaphore = FALSE;

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
    n = getwsp(&mSteelCA) ;

    if (n>0)
    {
        strcpy(section, pmSteelCA[mSteelCA.foff + mSteelCA.poz].txt);
        m_section = pmSteelCA[mSteelCA.foff + mSteelCA.poz].menu;
        m_standard = (*m_section->pola)[m_section->foff + m_section->poz].menu;
        strcpy(standard, (*m_section->pola)[m_section->foff + m_section->poz].txt);
        m_series = (PTMENU *)(*m_standard->pola)[m_standard->foff + m_standard->poz].menu;
        strcpy(series, (*m_standard->pola)[m_standard->foff + m_standard->poz].txt);
        strcpy(type, (*m_series->pola)[m_series->foff + m_series->poz].txt);

        manufacturer=NULL;
        if (m_standard->flags&TADD)
        {
            ptrsz_temp0=(*m_standard->pola)[m_standard->foff + m_standard->poz].txt;

            ptrsz_temp_tadd = ptrsz_temp0 + strlen(ptrsz_temp0)+1;
            if (strlen(ptrsz_temp_tadd) > 0)
            {
                manufacturer = strstr(ptrsz_temp_tadd, u8"™");
                if (manufacturer != NULL) manufacturer += 3;
            }
        }

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "CA", section, standard, series, manufacturer!=NULL ? manufacturer : "", type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "CA", section, standard, series, manufacturer, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
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
    char *ptrsz_temp0;
    char *ptrsz_temp_tadd;
    char *manufacturer;

    Semaphore = FALSE;

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

        manufacturer=NULL;
        if (m_standard->flags&TADD)
        {
            ptrsz_temp0=(*m_standard->pola)[m_standard->foff + m_standard->poz].txt;

            ptrsz_temp_tadd = ptrsz_temp0 + strlen(ptrsz_temp0)+1;
            if (strlen(ptrsz_temp_tadd) > 0)
            {
                manufacturer = strstr(ptrsz_temp_tadd, u8"™");
                if (manufacturer != NULL) manufacturer += 3;
            }
        }

        printf("\"Timber\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "US", section, standard, series, manufacturer!=NULL ? manufacturer : "", type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Timber", "US", section, standard, series, manufacturer, type, species, moisture));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
}

void TimberCA(void)
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
    char *ptrsz_temp0;
    char *ptrsz_temp_tadd;
    char *manufacturer;

    Semaphore = FALSE;

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
    n = getwsp(&mtimber_beams_CA_si);
    if (n>0)
    {
        strcpy(moisture, (pmWoodMoistureUS[mWoodMoistureUS.foff + mWoodMoistureUS.poz].txt));
        strcpy(species, (pmWoodSpeciesUS[mWoodSpeciesUS.foff + mWoodSpeciesUS.poz].txt+2));

        strcpy(section, pmTimberCA[0].txt);
        m_section = pmTimberCA[0].menu;
        m_standard = (*m_section->pola)[m_section->foff + m_section->poz].menu;
        strcpy(standard, (*m_section->pola)[m_section->foff + m_section->poz].txt);
        m_series = (PTMENU *)(*m_standard->pola)[m_standard->foff + m_standard->poz].menu;
        strcpy(series, (*m_standard->pola)[m_standard->foff + m_standard->poz].txt);
        strcpy(type, (*m_series->pola)[m_series->foff + m_series->poz].txt);

        manufacturer=NULL;

        printf("\"Timber\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "CA", section, standard, series, manufacturer!=NULL ? manufacturer : "", type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Timber", "CA", section, standard, series, manufacturer, type, species, moisture));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
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
    char *ptrsz_temp0;
    char *ptrsz_temp_tadd;
    char *manufacturer;

    Semaphore = FALSE;

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

        manufacturer=NULL;
        if (m_standard->flags&TADD)
        {
            ptrsz_temp0 = (*m_standard->pola)[m_standard->foff + m_standard->poz].txt;

            ptrsz_temp_tadd = ptrsz_temp0 + strlen(ptrsz_temp0) + 1;
            if (strlen(ptrsz_temp_tadd) > 0)
            {
                manufacturer = strstr(ptrsz_temp_tadd, u8"™");
                if (manufacturer != NULL) manufacturer += 3;
            }
        }

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "EU", section, standard, series, manufacturer!=NULL ? manufacturer : "", type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "EU", section, standard, series, manufacturer, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
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
    char *ptrsz_temp0;
    char *ptrsz_temp_tadd;
    char *manufacturer;

    //get property no

    Semaphore = FALSE;

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

        manufacturer=NULL;
        if (m_standard->flags&TADD)
        {
            ptrsz_temp0 = (*m_standard->pola)[m_standard->foff + m_standard->poz].txt;

            ptrsz_temp_tadd = ptrsz_temp0 + strlen(ptrsz_temp0) + 1;
            if (strlen(ptrsz_temp_tadd) > 0)
            {
                manufacturer = strstr(ptrsz_temp_tadd, u8"™");
                if (manufacturer != NULL) manufacturer += 3;
            }
        }

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "UK", section, standard, series, manufacturer!=NULL ? manufacturer : "", type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "UK", section, standard, series, manufacturer, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
}

void SteelAU(void)
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
    char *ptrsz_temp0;
    char *ptrsz_temp_tadd;
    char *manufacturer;

    Semaphore = FALSE;

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
    n = getwsp(&mSteelAU);
    if (n>0)
    {
        strcpy(section, pmSteelAU[mSteelAU.foff + mSteelAU.poz].txt);
        m_section = pmSteelAU[mSteelAU.foff + mSteelAU.poz].menu;
        m_standard = (*m_section->pola)[m_section->foff + m_section->poz].menu;
        strcpy(standard, (*m_section->pola)[m_section->foff + m_section->poz].txt);
        m_series = (PTMENU *)(*m_standard->pola)[m_standard->foff + m_standard->poz].menu;
        strcpy(series, (*m_standard->pola)[m_standard->foff + m_standard->poz].txt);
        strcpy(type, (*m_series->pola)[m_series->foff + m_series->poz].txt);

        manufacturer=NULL;
        if (m_standard->flags&TADD)
        {
            ptrsz_temp0 = (*m_standard->pola)[m_standard->foff + m_standard->poz].txt;

            ptrsz_temp_tadd = ptrsz_temp0 + strlen(ptrsz_temp0) + 1;
            if (strlen(ptrsz_temp_tadd) > 0)
            {
                manufacturer = strstr(ptrsz_temp_tadd, u8"™");
                if (manufacturer != NULL) manufacturer += 3;
            }
        }

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "AU", section, standard, series, manufacturer!=NULL ? manufacturer : "", type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "AU", section, standard, series, manufacturer, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
}

void SteelCN(void)
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
    char *ptrsz_temp0;
    char *ptrsz_temp_tadd;
    char *manufacturer;

    Semaphore = FALSE;

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
    n = getwsp(&mSteelCN);
    if (n>0)
    {
        strcpy(section, pmSteelCN[mSteelCN.foff + mSteelCN.poz].txt);
        m_section = pmSteelCN[mSteelCN.foff + mSteelCN.poz].menu;
        m_standard = (*m_section->pola)[m_section->foff + m_section->poz].menu;
        strcpy(standard, (*m_section->pola)[m_section->foff + m_section->poz].txt);
        m_series = (PTMENU *)(*m_standard->pola)[m_standard->foff + m_standard->poz].menu;
        strcpy(series, (*m_standard->pola)[m_standard->foff + m_standard->poz].txt);
        strcpy(type, (*m_series->pola)[m_series->foff + m_series->poz].txt);

        manufacturer=NULL;
        if (m_standard->flags&TADD)
        {
            ptrsz_temp0 = (*m_standard->pola)[m_standard->foff + m_standard->poz].txt;

            ptrsz_temp_tadd = ptrsz_temp0 + strlen(ptrsz_temp0) + 1;
            if (strlen(ptrsz_temp_tadd) > 0)
            {
                manufacturer = strstr(ptrsz_temp_tadd, u8"™");
                if (manufacturer != NULL) manufacturer += 3;
            }
        }

        printf("\"Steel\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", "CN", section, standard, series, manufacturer!=NULL ? manufacturer : "", type);
        fflush(stdout);

        strcpy(section_data, get_section(property_no, "Steel", "CN", section, standard, series, manufacturer, type, "", ""));

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
}

void ConcreteUS(void)
{
    int n;
    char section[MaxTextLen];
    char concrete_class[MaxTextLen];
    char steel_grade[MaxTextLen];
    int section_no;
    //TMENU *m_section;
    //TMENU *m_concrete_class;
    //TMENU *m_steel_grade;
    char section_data[MaxTextLen];
    char property_no_str[64];
    int property_no;
    unsigned int hidden;
    SECTION_PAR section_par={0.,0.,0.,0.};;

    Semaphore = FALSE;

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
    n = getwsp(&mConcreteUS);
    if (n>0)
    {
        strcpy(section, (pmConcreteUS[mConcreteUS.foff + mConcreteUS.poz].txt));
        section_no=mConcreteUS.foff + mConcreteUS.poz;
        strcpy(concrete_class, (pmConcreteClassUS[mConcreteClassUS.foff + mConcreteClassUS.poz].txt));
        strcpy(steel_grade, (pmSteelGradeUS[mSteelGradeUS.foff + mSteelGradeUS.poz].txt));

        printf("\"Concrete\" \"%s\" \"%s\" \"%s\" \"%s\"\n", "US", section, concrete_class, steel_grade);
        fflush(stdout);

        char *section_data_p=get_concrete_section(property_no, "Concrete", "US",  &section_par, section, section_no, concrete_class, steel_grade);
        if (section_data_p==NULL) return;

        strcpy(section_data, section_data_p);

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
}

void ConcreteEU(void)
{
    int n;
    char section[MaxTextLen];
    int section_no;
    char concrete_class[MaxTextLen];
    char steel_grade[MaxTextLen];
    //TMENU *m_section;
    //TMENU *m_concrete_class;
    //TMENU *m_steel_grade;
    char section_data[MaxTextLen];
    char property_no_str[64];
    int property_no;
    unsigned int hidden;
    SECTION_PAR section_par={0.,0.,0.,0.};

    Semaphore = FALSE;

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
    n = getwsp(&mConcreteEU);
    if (n>0)
    {
        strcpy(section, (pmConcreteEU[mConcreteEU.foff + mConcreteEU.poz].txt));
        section_no=mConcreteEU.foff + mConcreteEU.poz;
        strcpy(concrete_class, (pmConcreteClassEU[mConcreteClassEU.foff + mConcreteClassEU.poz].txt));
        strcpy(steel_grade, (pmSteelGradeEU[mSteelGradeEU.foff + mSteelGradeEU.poz].txt));

        printf("\"Concrete\" \"%s\" \"%s\" \"%s\" \"%s\"\n", "EU", section, concrete_class, steel_grade);
        fflush(stdout);

        char *section_data_p=get_concrete_section(property_no, "Concrete", "EU",  &section_par, section, section_no, concrete_class, steel_grade);
        if (section_data_p==NULL) return;

        strcpy(section_data, section_data_p);

        if (strlen(section_data) == 0) return;

        TextG.width=0;
        TextG.height=0;
        hidden=TextG.ukryty;
        TextG.ukryty=hidden_last;
        if (Tekst_factory(section_data, FALSE)) last_property_no = property_no;
        hidden_last=TextG.ukryty;
        TextG.ukryty=hidden;
    }
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