//
// Created by marek on 3/18/25.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "forwin.h"
#include "bib_e.h"

#include "o_section_block.h"

#ifdef LINUX
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

LINIA LG=Ldef;
char *LG1, *LG2;
LUK AG=ldef;
LUK CG=Odef;
ELLIPSE EG=Edef;
ELLIPTICALARC EAG=eldef;
char *OG;

char *Section_Units_System;

extern char *units_system_si;
extern double SkalaF;

int create_profile_block(char *units_system, char *series0, char *type0, double h, double b, double tw, double tf, double r1, double r2, double sf, double bt, double bb, double bf, double t, double ha, double ba, double ab, double c, double c1, double r3, double ri, double sw)
{  int i;
   char *pd0, *pd, *pde, *pdf;
   int retval_no = 1 ;
   double buf_ret [1] = {0};
   char *nag;
   char formula[MaxTextLen*2];
   char xy[]="xy(0,0)";
   BLOK *blk;
   BLOK blkd=Bdef;
   BLOK *buf_block;
   T_Desc_Ex_Block 	*ptrs_desc_blkd;
   BOOL ret;
   BOOL found=FALSE;
   double xblk=0.0, yblk=0.0;
   double r2l;

    ////STEEL

    //IH
    char *IHp[]= {"HD","HE","HE A","HE AA","HE B","HE C","HE M","HEA","HEB","HEM","HL","HLZ","IPE","IPE 750","IPE A","IPE AA","IPE O","IPE V","IPER","PEA","UB","UBP","UC",
                 "AW (Table 7)","H","HHD","HP","HP (Imperial)","HP (metric)","I","I (Table 12)","I (Table 13)","I (Table 8)","M","W","W (imperial)","W (metric)","W (RSA)","W (Table 8)","WF (Table 9)","WF(A-N) (Table 10)","WTM",
                 "Advance UKBP","Advance UKC","UB (RSA)","UC (RSA)"};
    int IHp_n=sizeof(IHp)/sizeof(IHp[0]);
    char *IHt[]= {"I W","INP","IPN","J","S","S (Table 11)","S (Table 9)","WF (Table 9)","RSJ"};
    int IHt_n=sizeof(IHt)/sizeof(IHt[0]);
    char *IHs[]= {"ASB"};
    int IHs_n=sizeof(IHs)/sizeof(IHs[0]);

    //C
    char *Cp[]= {"PFC","UPE","CS (Table 4)","CS (Table 7)","Advance UKPFC","PFC","SCP"};
    int Cp_n=sizeof(Cp)/sizeof(Cp[0]);
    char *Ct[]= {"UPN","C","C (imperial)","C (metric)","C (Table 5)","C (Table 6)","CS (Table 6)","MC","MC (imperial)","MC (metric)","CH","SC","TFC"};
    int Ct_n=sizeof(Ct)/sizeof(Ct[0]);
    char *Ccf[]= {"CS","CS-S"};
    int Ccf_n=sizeof(Ccf)/sizeof(Ccf[0]);
    char *Ctcf[]= {"CU-T"};
    int Ctcf_n=sizeof(Ctcf)/sizeof(Ctcf[0]);
    char *Cucf[]= {"HU"};
    int Cucf_n=sizeof(Cucf)/sizeof(Cucf[0]);

    //L
    char *Lp[]= {"L","L-equal","LNP","LNP-equal",
                 "L (imperial)","L (metric)","L (Table 10)","L (Table 12)","L (Table 14)","L (Table 16)","L-equal","L-equal (imperial)","L-equal (metric)","LS (Table 11)","LS (Table 13)","LS (Table 15)","LS (Table 17)",
                 "Advance UKA","Advance UKA-equal","EA","UA"};
    int Lp_n=sizeof(Lp)/sizeof(Lp[0]);
    char *Lt[]= {""};
    int Lt_n=sizeof(Lt)/sizeof(Lt[0]);
    char *Lcf[]= {"LS"};
    int Lcf_n=sizeof(Lcf)/sizeof(Lcf[0]);
    char *Ltcf[]= {"LU"};
    int Ltcf_n=sizeof(Ltcf)/sizeof(Ltcf[0]);

    //L2
    char *L2p[]= {"2L LLBB","2L SLBB","2L-equal BB"};
    int L2p_n=sizeof(L2p)/sizeof(L2p[0]);
    char *L2t[]= {""};
    int L2t_n=sizeof(L2t)/sizeof(L2t[0]);
    char *L2cf[]= {""};
    int L2cf_n=sizeof(L2cf)/sizeof(L2cf[0]);

    //T
    char *Tp[]= {"1/2 HEA","1/2 HEB","1/2 HEM","1/2 IPE","MT","WT","1/2 UB","1/2 UC","Advance UKT (UKB)","Advance UKT (UKC)","TB"};
    int Tp_n=sizeof(Tp)/sizeof(Tp[0]);
    char *Tt[]= {"T","TB","TPH","ST","T (A-N) (Table 19)","T (Table 14)","T (Table 18)"};
    int Tt_n=sizeof(Tt)/sizeof(Tt[0]);
    char *Tcf[]= {""};
    int Tcf_n=sizeof(Tcf)/sizeof(Tcf[0]);

    //Z
    char *Zp[]= {"Z (Table 15)","Z (Table 20)"};
    int Zp_n=sizeof(Zp)/sizeof(Zp[0]);
    char *Zt[]= {""};
    int Zt_n=sizeof(Zt)/sizeof(Zt[0]);
    char *Zcf[]= {"ZS"};
    int Zcf_n=sizeof(Zcf)/sizeof(Zcf[0]);
    char *Ztcf[]= {"ZU"};
    int Ztcf_n=sizeof(Ztcf)/sizeof(Ztcf[0]);

    //RT
    char *RTp[]= {"Celsius 355 RHS","Celsius 355 SHS","Celsius RHS","Celsius SHS","Hybox 355 RHS","Hybox 355 SHS","Hybox RHS","Hybox SHS","MSH Q","MSH R","RHC","RHH","RHS","RRK (RHS)","RRK (SHS)","RRW (RHS)","RRW (SHS)","SHC","SHH","SHS",
                  "Rect HSS","Rect HSS (A1085)","RT (Table 18)","RT (Table 19)","RT (Table 23)","RT (Table 24)","Sqr HSS","Sqr HSS (A1085)"};
    int RTp_n=sizeof(RTp)/sizeof(RTp[0]);

    //CT
    char *CTp[]= {"Celsius 355 CHS","Celsius 355 CHS","Celsius CHS","CHC","CHS","DN","Hybox 355 CHS","Hybox CHS","MSHRund","ROR",
                  "NPS","NPS (Table 17)","NPS (Table 22)","OD (Table 16)","OD (Table 21)","Pipes Std.","Pipes x-Strong","Pipes xx-Strong","Round HSS","Round HSS (A1085)"};
    int CTp_n=sizeof(CTp)/sizeof(CTp[0]);

    //ET
    char *ETp[]= {"Celsius 355 EHS","EHS"};
    int ETp_n=sizeof(ETp)/sizeof(ETp[0]);

    //SET
    char *SETp[]= {"SEHS"};
    int SETp_n=sizeof(SETp)/sizeof(SETp[0]);

    //IH
    char *IHpd="lin(b,0);lin(0,-tf);lin(-b/2.+tw/2.+r1,0);arc(0,-r1,r1,pi/2.0,pi,0);lin(0,-h+2.*tf+2.*r1);arc(r1,0,r1,pi,3./2.*pi,0);lin(b/2.-tw/2.-r1,0);lin(0,-tf);lin(-b,0);lin(0,tf);lin(b/2.-tw/2.-r1,0);arc(0,r1,r1,3./2.*pi,0,0);lin(0,h-2.*tf-2.*r1);arc(-r1,0,r1,0, pi/2.,0);lin(-b/2.+tw/2.+r1,0);lin(0,tf)";
    char *IHtd="lin(b,0);lin(0,-tf);xy(3./4.*b,-tf);vec2(b/4.,atanr(sf),1,b/4.,pi+atanr(sf),0);fil(r2);xy(b/2.+tw/2.,-tf);lin(0,-h+2*tf);fil(r1);xy(3./4.*b,-h+tf);vec2(b/4.,pi-atanr(sf),1,b/4.,2*pi-atanr(sf),0);fil(r1);xy(b,-h+tf);lin(0,-tf);fil(r2);lin(-b,0);lin(0,tf);xy(b/4.,-h+tf);vec2(b/4.,pi+atanr(sf),1,b/4.,atanr(sf),0);fil(r2);xy(b/2.-tw/2.,-h+tf);lin(0,h-2*tf);fil(r1);xy(b/4.,-tf);vec2(b/4.,2*pi-atanr(sf),1,b/4.,pi-atanr(sf),0);fil(r1);xy(0,-tf);lin(0,tf);fil(r2)";
    char *IHsd="lin(bt,0);lin(0,-tf);lin(-bt/2.+tw/2.+r1,0);arc(0,-r1,r1,pi/2.0,pi,0);lin(0,-h+2.*tf+2.*r1);arc(r1,0,r1,pi,3./2.*pi,0);lin(bb/2.-tw/2.-r1,0);lin(0,-tf);lin(-bb,0);lin(0,tf);lin(bb/2.-tw/2.-r1,0);arc(0,r1,r1,3./2.*pi,0,0);lin(0,h-2.*tf-2.*r1);arc(-r1,0,r1,0, pi/2.,0);lin(-bt/2.+tw/2.+r1,0);lin(0,tf)";
    //C
    char *Cpd="lin(b,0);lin(0,-tf);lin(-b+tw+r1,0);arc(0,-r1,r1,pi/2.0,pi,0);lin(0,-h+2.*tf+2.*r1);arc(r1,0,r1,pi,3./2.*pi,0);lin(b-tw-r1,0);lin(0,-tf);lin(-b,0);lin(0,h)";
    char *Ctd="lin(b,0);lin(0,-tf);xy(b/2.,-tf);vec2(b/2.,atanr(sf),1,b/2.,pi+atanr(sf),0);fil(r2);xy(tw,-tf);lin(0,-h+2*tf);fil(r1);xy(b/2.,-h+tf);vec2(b/2.,pi-atanr(sf),1,b/2.,2*pi-atanr(sf),0);fil(r1);xy(b,-h+tf);lin(0,-tf);fil(r2);lin(-b,0);lin(0,h)";
    char *Ccfd="xy(t+ri,0);lin(b-2*ri-2*t,0);arc(0,-ri-t,ri+t,0,pi/2.,1);lin(0,-c+ri+t);lin(-t,0);lin(0,c-t-ri);arc(-ri,0,ri,0,pi/2.,0);lin(-b+2*ri+2*t,0);arc(0,-ri,ri,pi/2.,pi,0);lin(0,-h+2*t+2*ri);arc(ri,0,ri,pi,3./2.*pi,0);lin(b-2*t-2*ri,0);arc(0,ri,ri,3./2.*pi,0,0);lin(0,c-t-ri);lin(t,0);lin(0,-c+t+ri);arc(-ri-t,0,ri+t,3./2.*pi,0,1);lin(-b+2*t+2*ri,0);arc(0,ri+t,ri+t,pi,3./2.*pi,1);lin(0,h-2*t-2*ri);arc(ri+t,0,ri+t,pi/2.,pi,1)";
    char *Ctcfd="xy(t+ri,0);lin(b-ri-t,0);lin(0,-t);lin(-b+ri+t,0);arc(0,-ri,ri,pi/2.,pi,0);lin(0,-h+2*t+2*ri);arc(ri,0,ri,pi,3./2.*pi,0);lin(b-t-ri,0);lin(0,-t);lin(-b+t+ri,0);arc(0,ri+t,ri+t,pi,3./2.*pi,1);lin(0,h-2*t-2*ri);arc(ri+t,0,ri+t,pi/2.,pi,1)";
    char *Cucfd="xy(c1+ri,0);lin(c-2*t-2*ri,0);arc(0,-ri-t,ri+t,0,pi/2.,1);lin(0,-h+2*t+2*ri);arc(ri,0,ri,pi,3./2.*pi,0);lin(c1-t-ri,0);lin(0,-t);lin(-c1+t+ri,0);arc(0,ri+t,ri+t,pi,3./2.*pi,1);lin(0,h-2*t-2*ri);arc(-ri,0,ri,0,pi/2.,0);lin(-c+2*t+2*ri,0);arc(0,-ri,ri,pi/2.,pi,0);lin(0,-h+2*t+2*ri);arc(-ri-t,0,ri+t,3./2.*pi,0,1);lin(-c1+t+ri,0);lin(0,t);lin(c1-t-ri,0);arc(0,ri,ri,3./2.*pi,0,0);lin(0,h-2*t-2*ri);arc(ri+t,0,ri+t,pi/2.,pi,1)";
    //L
    char *Lpd="lin(t-r2,0);arc(0,-r2,r2,0,pi/2.,1);lin(0,-h+t+r1+r2);arc(r1,0,r1,pi,3./2.*pi,0);lin(b-t-r1-r2,0);arc(0,-r2,r2,0,pi/2.,1);lin(0,-t+r2);lin(-b,0);lin(0,h)";
    char *Lpcd="lin(max(t-r2,0),0);arc(t-r2,-r2l,r2,0,pi/2.+asinr(min(t-r2,0)/r2),1);lin(0,-h+t+r2l+r1);arc(r1,0,r1,pi,3./2.*pi,0);lin(b-t-r2l-r1,0);arc(0,-r2,r2,-asinr(min(t-r2,0)/r2),pi/2.,1);lin(0,max(t-r2,0));lin(-b,0);lin(0,h)"; //modified Lp for t<r2
    char *Ltd="";
    char *Lcfd="xy(t+ri,0);lin(c-t-ri,0);lin(0,-t);lin(-c+t+ri,0);arc(0,-ri,ri,pi/2.,pi,0);lin(0,-h+2*t+2*ri);arc(ri,0,ri,pi,3./2.*pi,0);lin(b-2*t-2*ri,0);arc(0,ri,ri,3./2.*pi,0,0);lin(0,c-t-ri);lin(t,0);lin(0,-c+t+ri);arc(-ri-t,0,ri+t,3./2.*pi,0,1);lin(-b+2*t+2*ri,0);arc(0,ri+t,ri+t,pi,3./2.*pi,1);lin(0,h-2*t-2*ri);arc(ri+t,0,ri+t,pi/2.,pi,1)";   //LS
    char *Ltcfd="lin(t,0);lin(0,-h+t+ri);arc(ri,0,ri,pi,3./2.*pi,0);lin(b-t-ri,0);arc(0,ri+t,ri+t,pi,3./2.*pi,1);lin(0,h-t-ri)";  //LU
    //L2
    char *L2pd="lin(b/2.-ab/2.,0);lin(0,-h);lin(-t+r2,0);arc(0,r2,r2,pi,3./2.*pi,1);lin(0,h-r2-r1-t);arc(-r1,0,r1,0,pi/2.,0);lin(-b/2.+ab/2.+t+r1+r2,0);arc(0,r2,r2,pi,3./2.*pi,1);lin(0,t-r2);xy(b/2.+ab/2.,0);lin(b/2.-ab/2.,0);lin(0,-t+r2);arc(-r2,0,r2,3./2.*pi,0,1);lin(-b/2.+ab/2.+t+r1+r2,0);arc(0,-r1,r1,pi/2.,pi,0);lin(0,-h+t+r1+r2);arc(-r2,0,r2,3./2.*pi,0,1);lin(-t+r2,0);lin(0,h)";
    char *L2td="";
    char *L2cfd="";
    //T
    char *Tpd="lin(b,0);lin(0,-tf);lin(-b/2.+tw/2.+r1,0);arc(0,-r1,r1,pi/2.0,pi,0);lin(0,-h+tf+r1);lin(-tw,0);lin(0,h-tf-r1);arc(-r1,0,r1,0,pi/2.,0);lin(-b/2.+tw/2.+r1,0);lin(0,tf)";
    char *Ttd="lin(b,0);lin(0,-tf+r2);arc(-r2,0,r2,3./2.0*pi,0,1);lin(-b/2.,0);xy(b/2.+tw/2.,-h/2.);vec2(h/2.,pi/2.-atanr(sw),1,h/2.,3./2.*pi-atanr(sw),0);fil(r1);xy(b/2.+tw/2.,-h);lin(-tw,0);fil(r3);xy(b/2.-tw/2.,-h/2.);vec2(h/2,3./2.*pi+atanr(sw),1,h/2.,pi/2.+atanr(sw),0);fil(r3);xy(b/2.-tw/2.,-tf);lin(-b/2.+tw/2.+r2,0);fil(r1);arc(0,r2,r2,pi,3./2.*pi,1);lin(0,tf-r2)";
    //char *Ttd="lin(b,0);lin(0,-tf+r3);arc(-r3,0,r3,3./2.0*pi,0,1);lin(-b/2.,0);xy(b/2.+tw/2.,-h/2.);vec2(h/2.,pi/2.-atanr(sw),1,h/2.,3./2.*pi-atanr(sw),0);fil(r1);xy(b/2.+tw/2.,-h);lin(-tw,0);fil(r3);xy(b/2.-tw/2.,-h/2.);vec2(h/2,3./2.*pi+atanr(sw),1,h/2.,pi/2.+atanr(sw),0);fil(r3);xy(b/2.-tw/2.,-tf);lin(-b/2.+tw/2.+r3,0);fil(r1);arc(0,r3,r3,pi,3./2.*pi,1);lin(0,tf-r3)";
    char *Tcfd="";
    //Z
    char *Zpd="lin(b/2.+tw/2.,0);lin(0,-h+tf+r1);arc(r1,0,r1,pi,3./2.*pi,0);lin(b/2.-tw/2.-r1-r2,0);arc(0,-r2,r2,0,pi/2.,1);lin(0,-tf+r2);lin(-b/2.-tw/2.,0);lin(0,h-tf-r1);arc(-r1,0,r1,0,pi/2.,0);lin(-b/2.+tw/2.+r1+r2,0);arc(0,r2,r2,pi,3./2.*pi,1);lin(0,tf-r2)";
    char *Ztd="";
    char *Zcfd="xy(0,-c/sqrt(2));lin(c/sqrt(2),c/sqrt(2));lin(bf-t-ri,0);fil(ri);arc(0,-ri-t,ri+t,0,pi/2.,1);lin(0,-h+2*t+2*ri);arc(ri,0,ri,pi,3./2.*pi,0);lin(bf-t-ri-t*tan(22.5),0);lin((c-t*tan(22.5))/sqrt(2),(c-t*tan(22.5))/sqrt(2));fil(ri);lin(t/sqrt(2),-t/sqrt(2));lin(-c/sqrt(2),-c/sqrt(2));lin(-bf+t+ri,0);fil(ri);arc(0,ri+t,ri+t,pi,3./2.*pi,1);lin(0,h-2*t-2*ri);arc(-ri,0,ri,0,pi/2.,0);lin(-bf+t+ri+t*tan(22.5),0);lin(-(c-t*tan(22.5))/sqrt(2),-(c-t*tan(22.5))/sqrt(2));fil(ri);lin(-t/sqrt(2),t/sqrt(2))"; //ZS
    char *Ztcfd="lin(bf-t-ri,0);arc(0,-ri-t,ri+t,0,pi/2.,1);lin(0,-h+2*t+2*ri);arc(ri,0,ri,pi,3./2.*pi,0);lin(bf-t-ri,0);lin(0,-t);lin(-bf+t+ri,0);arc(0,ri+t,ri+t,pi,3./2.*pi,1);lin(0,h-2*t-2*ri);arc(-ri,0,ri,0,pi/2.,0);lin(-bf+t+ri,0);lin(0,t)";   //ZU
    //RT
    char *RTpd="xy(0,-ri-t);lin(0,-h+2*t+2*ri);arc(ri+t,0,ri+t,pi,3./2.*pi,0);lin(b-2*t-2*ri,0);arc(0,ri+t,ri+t,3./2.*pi,0,0);lin(0,h-2*t-2*ri);arc(-ri-t,0,ri+t,0,pi/2.,0);lin(-b+2*t+2*ri,0);arc(0,-ri-t,ri+t,pi/2.,pi,0);xy(t,-ri-t);lin(0,-h+2*t+2*ri);arc(ri,0,ri,pi,3./2.*pi,0);lin(b-2*t-2*ri,0);arc(0,ri,ri,3./2.*pi,0,0);lin(0,h-2*t-2*ri);arc(-ri,0,ri,0,pi/2.,0);lin(-b+2*t+2*ri,0);arc(0,-ri,ri,pi/2.,pi,0)";
    //CT
    char *CTpd="cir(h/2.);cir(h/2.-t)";
    //ET
    char *ETpd="elp(b/2.,h/2.,0);elp(b/2.-t,h/2.-t,0)";
    //SET
    char *SETpd="elpa(b/2.,h,0,0,pi);xy(-b/2.,0);lin(b,0);xy(0,0);elpa(b/2.-t,h-t,0,atanr(t/(b/2.-t)),pi-atanr(t/(b/2.-t)));xy(-b/2.+t,t);lin(b-2*t,0)";

    ////WOOD
    //IH
    char *RBar[]= {"Beams and Stringers","Boards","Dimension Lumber","Dimension Lumber and Decking","Post and Timber","Southern Pine Glulam","Western Species Glulam"};
    int RBar_n=sizeof(RBar)/sizeof(RBar[0]);
    char *RBard="lin(b,0);lin(0,-h);lin(-b,0);lin(0,h)";

    Section_Units_System=units_system;

    if (FALSE == calculator (xy, &retval_no, buf_ret)  || retval_no < 1)
    {
        return 0;
    }

   //sprintf(formula,"h=%g;b=%g;tw=%g;tf=%g;r1=%g;r2=%g;sf=%g;bt=%g;bb=%g;bf=%g;t=%g;ha=%g;ba=%g;ab=%g;c=%g;c1=%g;r3=%g;ri=%g;sw=%g;", h,b,tw,tf,r1,r2,sf,bt,bb,bf,t,ha,ba,ab,c,c1,r3,ri,sw);
   //pdf=&formula[strlen(formula)];

   r2l=r2;

   //IHp
   for (i=0; i<IHp_n; i++)
   {
       if (strcmp(series0, IHp[i])==0)
       {
           pd0=malloc(strlen(IHpd)+1);
           memmove(pd0,IHpd,strlen(IHpd)+1);
           xblk=b/2.;
           yblk=0.;
           found=TRUE;
           break;
       }
   }
   //IHt
   if (found==FALSE)
   {
       for (i=0; i<IHt_n; i++)
       {
           if (strcmp(series0, IHt[i])==0)
           {
               pd0=malloc(strlen(IHtd)+1);
               memmove(pd0,IHtd,strlen(IHtd)+1);
               xblk=b/2.;
               yblk=0.;
               found=TRUE;
               break;
           }
       }
   }
    //IHs
    if (found==FALSE)
    {
        for (i=0; i<IHs_n; i++)
        {
            if (strcmp(series0, IHs[i])==0)
            {
                pd0=malloc(strlen(IHsd)+1);
                memmove(pd0,IHsd,strlen(IHsd)+1);
                xblk=b/2.;
                yblk=0.;
                found=TRUE;
                break;
            }
        }
    }
   //Cp
    if (found==FALSE)
    {
        for (i=0; i<Cp_n; i++)
        {
            if (strcmp(series0, Cp[i])==0)
            {
                pd0=malloc(strlen(Cpd)+1);
                memmove(pd0,Cpd,strlen(Cpd)+1);
                xblk=b/2.;
                yblk=0.;
                found=TRUE;
                break;
            }
        }
    }
    //Ct
    if (found==FALSE)
    {
        for (i=0; i<Ct_n; i++)
        {
            if (strcmp(series0, Ct[i])==0)
            {
                pd0=malloc(strlen(Ctd)+1);
                memmove(pd0,Ctd,strlen(Ctd)+1);
                xblk=b/2.;
                yblk=0.;
                found=TRUE;
                break;
            }
        }
    }
    //Ccf
    if (found==FALSE)
    {
        for (i=0; i<Ccf_n; i++)
        {
            if (strcmp(series0, Ccf[i])==0)
            {
                pd0=malloc(strlen(Ccfd)+1);
                memmove(pd0,Ccfd,strlen(Ccfd)+1);
                xblk=b/2.;
                yblk=0.;
                found=TRUE;
                break;
            }
        }
    }
    //Ctcf
    if (found==FALSE)
    {
        for (i=0; i<Ctcf_n; i++)
        {
            if (strcmp(series0, Ctcf[i])==0)
            {
                pd0=malloc(strlen(Ctcfd)+1);
                memmove(pd0,Ctcfd,strlen(Ctcfd)+1);
                xblk=b/2.;
                yblk=0.;
                found=TRUE;
                break;
            }
        }
    }
    //Cucf
    if (found==FALSE)
    {
        for (i=0; i<Cucf_n; i++)
        {
            if (strcmp(series0, Cucf[i])==0)
            {
                pd0=malloc(strlen(Cucfd)+1);
                memmove(pd0,Cucfd,strlen(Cucfd)+1);
                xblk=b/2.;
                yblk=0.;
                found=TRUE;
                break;
            }
        }
    }
    //Lp
    if (found==FALSE)
    {
        for (i = 0; i < Lp_n; i++)
        {
            if (strcmp(series0, Lp[i]) == 0)
            {
                if (t>=r2)
                {
                    pd0 = malloc(strlen(Lpd) + 1);
                    memmove(pd0, Lpd, strlen(Lpd) + 1);
                }
                else
                {
                    pd0 = malloc(strlen(Lpcd) + 1);
                    memmove(pd0, Lpcd, strlen(Lpcd) + 1);
                    r2l=sqrt(pow(r2,2)-pow(max(r2-t,0),2));
                }
                xblk=b/2.;
                yblk=-h;
                found = TRUE;
                break;
            }
        }
    }
    //Lt
    if (found==FALSE)
    {
        for (i=0; i<Lt_n; i++)
        {
            if (strcmp(series0, Lt[i])==0)
            {
                pd0=malloc(strlen(Ltd)+1);
                memmove(pd0,Ltd,strlen(Ltd)+1);
                xblk=b/2.;
                yblk=-h;
                found=TRUE;
                break;
            }
        }
    }
    //Lcf
    if (found==FALSE)
    {
        for (i=0; i<Lcf_n; i++)
        {
            if (strcmp(series0, Lcf[i])==0)
            {
                pd0=malloc(strlen(Lcfd)+1);
                memmove(pd0,Lcfd,strlen(Lcfd)+1);
                xblk=b/2.;
                yblk=-h;
                found=TRUE;
                break;
            }
        }
    }
    //Ltcf
    if (found==FALSE)
    {
        for (i=0; i<Ltcf_n; i++)
        {
            if (strcmp(series0, Ltcf[i])==0)
            {
                pd0=malloc(strlen(Ltcfd)+1);
                memmove(pd0,Ltcfd,strlen(Ltcfd)+1);
                xblk=b/2.;
                yblk=-h;
                found=TRUE;
                break;
            }
        }
    }
    //L2p
    if (found==FALSE)
    {
        for (i = 0; i < L2p_n; i++)
        {
            if (strcmp(series0, L2p[i]) == 0)
            {
                pd0 = malloc(strlen(L2pd) + 1);
                memmove(pd0, L2pd, strlen(L2pd) + 1);
                xblk=b/2.;
                yblk=0.;
                found = TRUE;
                break;
            }
        }
    }
    //L2t
    if (found==FALSE)
    {
        for (i=0; i<L2t_n; i++)
        {
            if (strcmp(series0, L2t[i])==0)
            {
                pd0=malloc(strlen(L2td)+1);
                memmove(pd0,L2td,strlen(L2td)+1);
                xblk=b/2.;
                yblk=0.;
                found=TRUE;
                break;
            }
        }
    }
    //L2cf
    if (found==FALSE)
    {
        for (i=0; i<L2cf_n; i++)
        {
            if (strcmp(series0, L2cf[i])==0)
            {
                pd0=malloc(strlen(L2cfd)+1);
                memmove(pd0,L2cfd,strlen(L2cfd)+1);
                xblk=b/2.;
                yblk=0.;
                found=TRUE;
                break;
            }
        }
    }
    //Tp
    if (found==FALSE)
    {
        for (i = 0; i < Tp_n; i++)
        {
            if (strcmp(series0, Tp[i]) == 0)
            {
                pd0 = malloc(strlen(Tpd) + 1);
                memmove(pd0, Tpd, strlen(Tpd) + 1);
                xblk=b/2.;
                yblk=0.;
                found = TRUE;
                break;
            }
        }
    }
    //Tt
    if (found==FALSE)
    {
        for (i=0; i<Tt_n; i++)
        {
            if (strcmp(series0, Tt[i])==0)
            {
                pd0=malloc(strlen(Ttd)+1);
                memmove(pd0,Ttd,strlen(Ttd)+1);
                xblk=b/2.;
                yblk=0.;
                if (r2==0) r2=r3; //in US there is no r2
                found=TRUE;
                break;
            }
        }
    }
    //Tcf
    if (found==FALSE)
    {
        for (i=0; i<Tcf_n; i++)
        {
            if (strcmp(series0, Tcf[i])==0)
            {
                pd0=malloc(strlen(Tcfd)+1);
                memmove(pd0,Tcfd,strlen(Tcfd)+1);
                xblk=b/2.;
                yblk=0.;
                if (r2==0) r2=r3; //in US there is no r2
                found=TRUE;
                break;
            }
        }
    }
    //Zp
    if (found==FALSE)
    {
        for (i = 0; i < Zp_n; i++)
        {
            if (strcmp(series0, Zp[i]) == 0)
            {
                pd0 = malloc(strlen(Zpd) + 1);
                memmove(pd0, Zpd, strlen(Zpd) + 1);
                xblk=b/2.;
                yblk=-h/2.;
                found = TRUE;
                break;
            }
        }
    }
    //Zt
    if (found==FALSE)
    {
        for (i=0; i<Zt_n; i++)
        {
            if (strcmp(series0, Zt[i])==0)
            {
                pd0=malloc(strlen(Ztd)+1);
                memmove(pd0,Ztd,strlen(Ztd)+1);
                xblk=b/2.;
                yblk=-h/2.;
                found=TRUE;
                break;
            }
        }
    }
    //Zcf
    if (found==FALSE)
    {
        for (i=0; i<Zcf_n; i++)
        {
            if (strcmp(series0, Zcf[i])==0)
            {
                pd0=malloc(strlen(Zcfd)+1);
                memmove(pd0,Zcfd,strlen(Zcfd)+1);
                xblk=b/2.;
                yblk=-h/2.;
                found=TRUE;
                break;
            }
        }
    }
    //Ztcf
    if (found==FALSE)
    {
        for (i=0; i<Ztcf_n; i++)
        {
            if (strcmp(series0, Ztcf[i])==0)
            {
                pd0=malloc(strlen(Ztcfd)+1);
                memmove(pd0,Ztcfd,strlen(Ztcfd)+1);
                xblk=b/2.;
                yblk=-h/2.;
                found=TRUE;
                break;
            }
        }
    }
    //RTp
    if (found==FALSE)
    {
        for (i = 0; i < RTp_n; i++)
        {
            if (strcmp(series0, RTp[i]) == 0)
            {
                pd0 = malloc(strlen(RTpd) + 1);
                memmove(pd0, RTpd, strlen(RTpd) + 1);
                xblk=b/2.;
                yblk=0.;
                found = TRUE;
                break;
            }
        }
    }
    //CTp
    if (found==FALSE)
    {
        for (i = 0; i < CTp_n; i++)
        {
            if (strcmp(series0, CTp[i]) == 0)
            {
                pd0 = malloc(strlen(CTpd) + 1);
                memmove(pd0, CTpd, strlen(CTpd) + 1);
                xblk=0.;
                yblk=0.;
                found = TRUE;
                break;
            }
        }
    }
    //ETp
    if (found==FALSE)
    {
        for (i = 0; i < ETp_n; i++)
        {
            if (strcmp(series0, ETp[i]) == 0)
            {
                pd0 = malloc(strlen(ETpd) + 1);
                memmove(pd0, ETpd, strlen(ETpd) + 1);
                xblk=0.;
                yblk=0.;
                found = TRUE;
                break;
            }
        }
    }
    //SETp
    if (found==FALSE)
    {
        for (i = 0; i < SETp_n; i++)
        {
            if (strcmp(series0, SETp[i]) == 0)
            {
                pd0 = malloc(strlen(SETpd) + 1);
                memmove(pd0, SETpd, strlen(SETpd) + 1);
                xblk=0.;
                yblk=0.;
                found = TRUE;
                break;
            }
        }
    }
    //RBar
    if (found==FALSE)
    {
        for (i = 0; i < RBar_n; i++)
        {
            if (strcmp(series0, RBar[i]) == 0)
            {
                pd0 = malloc(strlen(RBard) + 1);
                memmove(pd0, RBard, strlen(RBard) + 1);
                xblk=b/2.;
                yblk=-h/2.;
                found = TRUE;
                break;
            }
        }
    }

    if (found==FALSE)
        return 0;


   sprintf(formula,"h=%g;b=%g;tw=%g;tf=%g;r1=%g;r2=%g;sf=%g;bt=%g;bb=%g;bf=%g;t=%g;ha=%g;ba=%g;ab=%g;c=%g;c1=%g;r3=%g;ri=%g;sw=%g;r2l=%g;", h,b,tw,tf,r1,r2,sf,bt,bb,bf,t,ha,ba,ab,c,c1,r3,ri,sw,r2l);
   pdf=&formula[strlen(formula)];

   pd=pd0;

   //creating block
   ret=add_block_();
   LG.kolor=AG.kolor=CG.kolor=EG.kolor=EAG.kolor=LiniaG.kolor;
   LG.warstwa=AG.warstwa=CG.warstwa=EG.warstwa=EAG.warstwa=LiniaG.warstwa;
   LG.blok=AG.blok=CG.blok=EG.blok=EAG.blok=ElemBlok;

   pde=strchr(pd, ';');
   while (pde!=NULL)
   {
       *pde='\0';
       OG=NULL;
       memmove(pdf,pd,strlen(pd)+1);
       if (FALSE == calculator (formula, &retval_no, buf_ret)  || retval_no < 1)
       {
           free(pd);
           return 0;
       }
       if (OG!=NULL) {
           printf("obiekt=%d\n", ((NAGLOWEK *) OG)->obiekt);
           nag = dodaj_obiekt((BLOK *) dane, OG);

           if (((NAGLOWEK*)nag)->obiekt==Olinia)
           {
               LG1=Get_PTR__GTMPFIRST();
               LG2=nag;
               Set_PTR__GTMPFIRST(LG2);
           }
       }
       pd=pde+1;
       pde=strchr(pd, ';');
   }
   //the rest of string
    OG=NULL;
    memmove(pdf,pd,strlen(pd)+1);
    if (FALSE == calculator (formula, &retval_no, buf_ret)  || retval_no < 1)
    {
        free(pd);
        return 0;
    }
    if (OG!=NULL) {
        printf("obiekt=%d\n", ((NAGLOWEK *) OG)->obiekt);
        nag = dodaj_obiekt((BLOK *) dane, OG);
    }
    free(pd0);
    ////cover with outer block with block name and origin point
    blk=(BLOK*)dane;

    unsigned int size_block0=blk->n+sizeof(NAGLOWEK);
    unsigned int size_block = B3 ;

    int len_type = (int) strlen(type0) + 1;
    int len_desc = sizeof(unsigned) + 2 * sizeof(float) + sizeof(len_type) + len_type;
    size_block += len_desc ;

    buf_block = (BLOK*)malloc(sizeof(NAGLOWEK) + size_block);
    if (buf_block!=NULL)
    {
        memcpy (buf_block, &blkd, sizeof (blkd)) ;

        buf_block->n = size_block ;
        buf_block->kod_obiektu = 'E' ;
        buf_block->dlugosc_opisu_obiektu = len_desc ;
        ptrs_desc_blkd = (T_Desc_Ex_Block *)(&buf_block->opis_obiektu [0]) ;
        ptrs_desc_blkd->flags = EBF_IP ;

        double factor;
        if (strcmp(Section_Units_System, units_system_si)==0)
            factor=1./SkalaF;
        else factor=25.4/SkalaF;

        ptrs_desc_blkd->x = (float)(xblk*factor);
        ptrs_desc_blkd->y = (float)(yblk*factor);
        ptrs_desc_blkd->len = len_type ;
        strcpy (&ptrs_desc_blkd->sz_type [0], type0) ;
        BLOK *ptrs_block = (BLOK*)dodaj_obiekt (NULL, buf_block);

        if (ptrs_block!=NULL)
        {
            blk=(BLOK*)((char*)ptrs_block+ptrs_block->n+sizeof(NAGLOWEK));
            blk->blok=1;
            size_block += size_block0;
            ptrs_block->n = size_block;
        }
        free(buf_block);
    }
    ///////
    adrem_blok1 ((BLOK*)dane, Ablok);
    Place_Import_Block(5, type0);
    return 1;
}