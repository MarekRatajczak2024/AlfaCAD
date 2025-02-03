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

#define __O_ANIMATE_DYN__
#include <stdlib.h>
#include <stdio.h>
#ifdef LINUX
#include <sys/time.h>
#else
#include <time.h>
typedef struct timeval {
    long tv_sec;
    long tv_usec;
} timeval;
#endif
#include <string.h>
#include <math.h>
#include <forwin.h>
#ifdef LINUX
#include <unistd.h>
#endif
#include <allegext.h>
#include "bib_e.h"
#include "o_static.h"
#include "o_bloklb.h"
#include "bib_blok.h"
#include "o_object.h"
#include "rysuj_e.h"
#include "message.h"
#include "o_loadf.h"
#include "o_animate_dynamics.h"

#include "leak_detector_c.h"

extern void rysuj_spline_(SPLINE *ad, float *ad_xy, int mode, int kolor, BOOL count_mvcurb, BOOL partial, BOOL moving);
extern void delay(int time);
extern void  outtext_r_(BITMAP *ui_screen, char  *textstring);
extern int TTF_text_len(char *text);
extern void setcolor(int kolor);
extern void set_global_hidden_blocks_visibility(BOOL block_visibility);
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);
extern void remove_short_notice(void);
extern int  getcom(TMENU *menu);

extern void lock_mouse(void);
extern void free_mouse(void);
extern void dialog_cursor(int on);
extern void set_posXY(double pozx, double pozy);
extern void get_posXY(double *pozx, double *pozy);

#ifndef LINUX
int gettimeofday(struct timeval* tp, struct timezone* tzp);
#endif

POLE pmanimation[] = {
        {u8"", L' ', 0, NULL},
};

TMENU manimation={ 1,0,0,6,16,8,0,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmanimation,NULL,NULL };

int n=-1;

int animate_blocks(int frame_no, int blocks_no, ANIM_BLOCKS *anim_blocks, unsigned char mode, int *i_len)
{
    int poffset;
    SPLINE *ptrs_spline;
    char *adp, *adk;
    TEXT T;
    int x,y, ii_len;
    int ret=0;
    struct timeval stop, start;
    int delta_t;

    okno_r();

    if (frame_no == 0)
    {
        poffset = 0;
        n=n*(-1);
    }
    else poffset = 2;

    gettimeofday(&start,NULL);


    for (int i = 0; i < blocks_no; i++)
    {
        if (anim_blocks[i].mode==mode)
        {
            x = (int) pikseleX0(anim_blocks[i].x) - n*(frame_no * 2) - 10;
            y = (int) pikseleY0(anim_blocks[i].y) - (frame_no * 2);
            ii_len = i_len[i];
            if ((x + ii_len) >= pikseleX0(Xk))
            {
                x = pikseleX0(Xk) - ii_len - (frame_no * 2);
                y = (int) pikseleY0(anim_blocks[i].y) + n*(frame_no * 2);
            }

            moveto(x, y);
            setcolor(static_colors.dynamic_color);
            outtext_r(anim_blocks[i].type);

            adp = anim_blocks[i].adp;
            adk = anim_blocks[i].adk;
            obiekt_tok_all(adp, adk, (char **) &ptrs_spline, Ospline);
            while (ptrs_spline != NULL)
            {
                rysuj_spline_(ptrs_spline, &ptrs_spline->xy[frame_no * 22 + poffset], COPY_PUT, 1, TRUE, TRUE, TRUE);
                obiekt_tok_all(NULL, adk, (char **) &ptrs_spline, Ospline);
            }
        }
    }

    gettimeofday(&stop,NULL);
    delta_t=(int)(((((long long)stop.tv_sec)*1000)+(stop.tv_usec/1000))-((((long long)start.tv_sec)*1000)+(start.tv_usec/1000)));

    okno_all();

    if (delta_t>39) delta_t=39;

    delay(40-delta_t);
    ret=inkeys(&manimation, FALSE);
    flip_screen();
    CUR_ON(X,Y);
    return ret;
}

void Animate_dynamics(void)
{
    char  *adp, *adk ;
    BLOK 	*ptrs_block ;
    SPLINE 	*ptrs_spline ;
    T_Desc_Ex_Block *ptrs_desc_bl ;
    int blocks_no=0;
    ANIM_BLOCKS anim_blocks[MAX_ANIM_BLOCKS];
    int frame_no;
    int poffset;
    int x, y;
    int mode;
    char *ptr;
    int mode_no;
    int modes=0;
    int val,scancode;
    char st [MaxTextLen] ;
    int ret;
    int *i_len;
    int i_len0;

//searching for visible blocks flag!=2
    adp=(char*)dane;
    adk = dane + dane_size ;
    set_global_hidden_blocks_visibility(TRUE);
    obiekt_tok_all (adp, adk, (char **)&ptrs_spline, Ospline) ;
    while (ptrs_spline != NULL)
    {
       if ((ptrs_spline->multiple==1) && (Layers[ptrs_spline->warstwa].on)) //to animate and layer is visible
       {
           ptrs_block=FIRSTB(ptrs_spline);
           if (ptrs_block != NULL)
           {
               if ((ptrs_block->kod_obiektu == B_EXPORT) && (ptrs_block->dlugosc_opisu_obiektu > 1) && (ptrs_block->flag & 1))
               {
                   ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
                   if (ptrs_desc_bl->flags == EBF_IP && ptrs_desc_bl->sz_type [0] != '\0')
                   {
                       anim_blocks[blocks_no].adp=(char*)ptrs_block+sizeof(NAGLOWEK)+B3+ptrs_block->dlugosc_opisu_obiektu;
                       anim_blocks[blocks_no].adk=(char*)ptrs_block+sizeof(NAGLOWEK)+ptrs_block->n-1;
                       anim_blocks[blocks_no].flag=ptrs_block->flag;
                       anim_blocks[blocks_no].type=&ptrs_desc_bl->sz_type [0];
                       ptr=strstr(anim_blocks[blocks_no].type, "MODE");
                       if (ptr!=NULL)
                       {
                           sscanf(ptr+4, "%d", &mode_no);
                           anim_blocks[blocks_no].mode=mode_no;
                           if (mode_no>modes) modes=mode_no;
                       }
                       else anim_blocks[blocks_no].mode=1;
                       anim_blocks[blocks_no].x=ptrs_desc_bl->x;
                       anim_blocks[blocks_no].y=ptrs_desc_bl->y;
                       blocks_no++;
                       if (blocks_no==MAX_ANIM_BLOCKS) break;
                   }
               }
               adp=(char*)ptrs_block+sizeof(NAGLOWEK)+ptrs_block->n;
               obiekt_tok_all (adp, adk, (char **)&ptrs_spline, Ospline) ;
           }
           else obiekt_tok_all (NULL, adk, (char **)&ptrs_spline, Ospline) ;
       }
       else obiekt_tok_all (NULL, adk, (char **)&ptrs_spline, Ospline) ;
    }
    set_global_hidden_blocks_visibility(FALSE);

//instead of creating the list of spline addresses it will follow block addresses, expecting in each block will be only splines
//   set second screen
   Set_Second_Screen();
    okno_r_second();

    solid_mode();

    if (blocks_no>0) i_len = (int*) malloc(blocks_no*sizeof(int));
//removing splines from second screen
   for (int i=0; i<blocks_no; i++)
   {
       i_len[i] = TTF_text_len(anim_blocks[i].type);

       if (!(anim_blocks[i].flag & 2))
       {
           adp = anim_blocks[i].adp;
           adk = anim_blocks[i].adk;
           obiekt_tok_all(adp, adk, (char **) &ptrs_spline, Ospline);
           while (ptrs_spline != NULL) {
               rysuj_spline_(ptrs_spline, ptrs_spline->xy, COPY_PUT, 0, TRUE, TRUE, TRUE);

               obiekt_tok_all(NULL, adk, (char **) &ptrs_spline, Ospline);
           }

       }
   }
   okno_all_second();
   Set_Screen();
   flip_screen();

   komunikat_str_short (_ANIMATE_, TRUE, FALSE);

//how it is done:
//outtextxy_w(t, COPY_PUT);
//rysuj_spline(s, mode, 1, FALSE, FALSE, FALSE);
//set screen
//rysuj_spline(s, mode, 1, FALSE, FALSE, FALSE);
//increase counter frame_no
//flip second_screen to screen
//if frame_no>0 copy s to bufs with indexed xy for next frame
//set screen
//rysuj_spline(bufs, mode, 1, FALSE, FALSE, FALSE);
//until frame_no==25
//then do the same decreasing frame_no down to 0
//when any key pressed, stop loop and :
//set second screen
//rysuj_spline(s, mode, 1, FALSE, FALSE, FALSE);
//outtextxy_w_0(t);   //NOT_PUT
//flip second screen to screen

//while (!keypressed())
//dialog_cursor(1);
//free_mouse();

    mode=1;
    while (mode<=modes)
    {
        ret=0;
        while (!ret)
        {
            for (frame_no = 0; frame_no < 26; frame_no++) {
                if (ret) break;
                ret=animate_blocks(frame_no, blocks_no, anim_blocks, mode, i_len);
            };
            for (frame_no = 24; frame_no > 0; frame_no--) {
                if (ret) break;
                ret=animate_blocks(frame_no, blocks_no, anim_blocks, mode, i_len);
            }
        }

        val = ret;
        clear_keybuf();
        if (val == 0x001B)
             break;  //ESC
        else if (val == 0x0008)
            mode=max(1,mode-1);    //BackSpace
        else mode++;

    }

    Set_Second_Screen();
    okno_r_second();

    solid_mode();
//restoring splines from second screen
    for (int i=0; i<blocks_no; i++)
    {
        if (!(anim_blocks[i].flag & 2))
        {

            adp = anim_blocks[i].adp;
            adk = anim_blocks[i].adk;
            obiekt_tok_all(adp, adk, (char **) &ptrs_spline, Ospline);
            while (ptrs_spline != NULL)
            {
                rysuj_spline_(ptrs_spline, ptrs_spline->xy, COPY_PUT, 1, TRUE, TRUE, TRUE);

                obiekt_tok_all(NULL, adk, (char **) &ptrs_spline, Ospline);
            }
        }

    }
    okno_all_second();
    Set_Screen();
    flip_screen();

    if (blocks_no>0) free(i_len);

    komunikat_str_short("", TRUE, FALSE);
    remove_short_notice();

}

#undef __O_ANIMATE_DYN__