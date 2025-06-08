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

#include <stdio.h>

#include <allegro.h>
#include <forwin.h>
#include "bib_e.h"
#include "o_dialog.h"

extern void (*CUR_OFF)(double, double);
extern void (*CUR_ON)(double, double);
extern double X,Y;
extern const KOLORY kolory;
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);

int gui_border_dark, gui_border_light;

/* dotted_rect:
 *  Draws a dotted rectangle, for showing an object has the input focus.
 */
static void dotted_rect(int x1, int y1, int x2, int y2, int fg, int bg)
{
    BITMAP *gui_bmp = gui_get_screen();
    int x = ((x1+y1) & 1) ? 1 : 0;
    int c;

    /* two loops to avoid bank switches */
    for (c=x1; c<=x2; c++)
        putpixel(gui_bmp, c, y1, (((c+y1) & 1) == x) ? fg : bg);
    for (c=x1; c<=x2; c++)
        putpixel(gui_bmp, c, y2, (((c+y2) & 1) == x) ? fg : bg);

    for (c=y1+1; c<y2; c++) {
        putpixel(gui_bmp, x1, c, (((c+x1) & 1) == x) ? fg : bg);
        putpixel(gui_bmp, x2, c, (((c+x2) & 1) == x) ? fg : bg);
    }
}

static void solid_rect(int x1, int y1, int x2, int y2, int fg, int bg)
{
    BITMAP *gui_bmp = gui_get_screen();
    int x = ((x1+y1) & 1) ? 1 : 0;
    int c;

    /* two loops to avoid bank switches */
    hline(gui_bmp, x1-1, y1-1, x2+1, fg);
    vline(gui_bmp, x2+1, y1-1, y2+1, fg);
    hline(gui_bmp, x1-1, y2+1, x2+1, fg);
    vline(gui_bmp, x1-1, y1-1, y2+1, fg);

    hline(gui_bmp, x1-2, y1-2, x2+2, fg);
    vline(gui_bmp, x2+2, y1-2, y2+2, fg);
    hline(gui_bmp, x1-2, y2+2, x2+2, fg);
    vline(gui_bmp, x1-2, y1-2, y2+2, fg);


    hline(gui_bmp, x1, y1, x2 - 1, gui_border_dark);
    vline(gui_bmp, x1, y1, y2, gui_border_dark);

    hline(gui_bmp, x1 + 1, y2, x2, gui_border_light);
    vline(gui_bmp, x2, y1 + 1, y2, gui_border_light);
}



/* d_slider_proc:
 *  A slider control object. This object returns a value in d2, in the
 *  range from 0 to d1. It will display as a vertical slider if h is
 *  greater than or equal to w; otherwise, it will display as a horizontal
 *  slider. dp can contain an optional bitmap to use for the slider handle;
 *  dp2 can contain an optional callback function, which is called each
 *  time d2 changes. The callback function should have the following
 *  prototype:
 *
 *  int function(void *dp3, int d2);
 *
 *  The d_myslider_proc object will return the value of the callback function.
 */

int slider_function(int *var)
{   char slider_var[32];

    sprintf(slider_var, "%d", *var);
    komunikat_str_short(slider_var, FALSE, FALSE);
    //*var=10;

    return 1;
}

int grab_slider(void *dp3, int d2)
{
    int var3=0;
    int (*SlideFun)(int*);

    SlideFun = (int(*)(int *))dp3;

    int var1=d2;
    int var2= SlideFun(&var1);
    return 0;
}


int my_object_message(SLIDER *dialog, int msg, int c)
{
    SLIDER *slider;
#ifdef ALLEGRO_WINDOWS
    /* exported address of d_clear_proc */
   extern int (*_d_clear_proc)(int, DIALOG *, int);
#endif

    int ret;
    ASSERT(dialog);

    if (msg == MSG_DRAW) {
        if (dialog->flags & D_HIDDEN)
            return D_O_K;

/*
#ifdef ALLEGRO_WINDOWS
        if (dialog->proc == _d_clear_proc)
#else
        if (dialog->proc == d_clear_proc)
#endif
            scare_mouse();
        else
            scare_mouse_area(dialog->x, dialog->y, dialog->w, dialog->h);
*/
        //select_mouse_cursor(MOUSE_CURSOR_NONE);
        show_mouse(NULL);
        acquire_screen();
    }


    slider = dialog->slider;
    slider->d2 = dialog->d2;

    ret = dialog->proc(msg, dialog, c);

    if (msg == MSG_DRAW) {
        release_screen();
        show_mouse(screen);
        //select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
        /*
        unscare_mouse();
         */
    }

    if (ret & D_REDRAWME) {
        dialog->flags |= D_DIRTY;
        ret &= ~D_REDRAWME;
    }

    return ret;
}


int d_myslider_proc(int msg, void *d_, int c)
{
    BITMAP *gui_bmp = gui_get_screen();
    BITMAP *slhan = NULL;
    int oldpos, newpos;
    int sfg;                /* slider foreground color */
    int vert = TRUE;        /* flag: is slider vertical? */
    int hh = 7;             /* handle height (width for horizontal sliders) */
    int hmar;               /* handle margin */
    int slp;                /* slider position */
    int mp;                 /* mouse position */
    int irange;
    int slx, sly, slh, slw;
    int msx, msy;
    int retval = D_O_K;
    int upkey, downkey;
    int pgupkey, pgdnkey;
    int homekey, endkey;
    int delta;
    fixed slratio, slmax, slpos;
    int (*proc)(void *cbpointer, int d2value);
    int oldval;
    int mb;
    SLIDER *d=d_;
    ASSERT(d);

    /* check for slider direction */
    if (d->h < d->w)
        vert = FALSE;

    /* set up the metrics for the control */
    if (d->dp != NULL) {
        slhan = (BITMAP *)d->dp;
        if (vert)
            hh = slhan->h;
        else
            hh = slhan->w;
    }

    hmar = hh/2;
    irange = (vert) ? d->h : d->w;
    slmax = itofix(irange-hh);
    if (d->d1==0) slratio=1;
    else slratio = slmax / (d->d1);
    slpos = slratio * d->d2;
    slp = fixtoi(slpos);

    if (mb=gui_mouse_b()==2)
    {
        msg=22;
    }

    switch (msg)
    {
        case MSG_RPRESS:
            retval = -1;
            break;

        case MSG_DRAW:

            sfg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;

            if (vert) {
                rectfill(gui_bmp, d->x, d->y, d->x+d->w/2-2, d->y+d->h-1, d->bg);
                rectfill(gui_bmp, d->x+d->w/2-1, d->y, d->x+d->w/2+1, d->y+d->h-1, sfg);
                rectfill(gui_bmp, d->x+d->w/2+2, d->y, d->x+d->w-1, d->y+d->h-1, d->bg);
            }
            else {
                rectfill(gui_bmp, d->x, d->y, d->x+d->w-1, d->y+d->h/2-2, d->bg);
                rectfill(gui_bmp, d->x, d->y+d->h/2-1, d->x+d->w-1, d->y+d->h/2+1, sfg);
                rectfill(gui_bmp, d->x, d->y+d->h/2+2, d->x+d->w-1, d->y+d->h-1, d->bg);
            }

            /* okay, background and slot are drawn, now draw the handle */
            if (slhan) {
                if (vert) {
                    slx = d->x+(d->w/2)-(slhan->w/2);
                    sly = d->y+(d->h-1)-(hh+slp);
                }
                else {
                    slx = d->x+slp;
                    sly = d->y+(d->h/2)-(slhan->h/2);
                }
                draw_sprite(gui_bmp, slhan, slx, sly);
            }
            else {
                /* draw default handle */
                if (vert) {
                    slx = d->x;
                    sly = d->y+(d->h)-(hh+slp);
                    slw = d->w-1;
                    slh = hh-1;
                }
                else {
                    slx = d->x+slp;
                    sly = d->y;
                    slw = hh-1;
                    slh = d->h-1;
                }

                /* draw body */
                rectfill(gui_bmp, slx+2, sly, slx+(slw-2), sly+slh, sfg);
                vline(gui_bmp, slx+1, sly+1, sly+slh-1, sfg);
                vline(gui_bmp, slx+slw-1, sly+1, sly+slh-1, sfg);
                vline(gui_bmp, slx, sly+2, sly+slh-2, sfg);
                vline(gui_bmp, slx+slw, sly+2, sly+slh-2, sfg);
                vline(gui_bmp, slx+1, sly+2, sly+slh-2, d->bg);
                hline(gui_bmp, slx+2, sly+1, slx+slw-2, d->bg);
                putpixel(gui_bmp, slx+2, sly+2, d->bg);
            }

            if (d->flags & D_GOTFOCUS) solid_rect(d->x, d->y, d->x+d->w-1, d->y+d->h-1, gui_mg_color, d->bg);  //sfg
            else  solid_rect(d->x, d->y, d->x+d->w-1, d->y+d->h-1, gui_mg_color, d->bg);  //sfg

            break;

        case MSG_WANTFOCUS:
        case MSG_LOSTFOCUS:
            return D_WANTFOCUS;

        case MSG_KEY:
            if (!(d->flags & D_GOTFOCUS))
                return D_WANTFOCUS;
            else
                return D_O_K;

            //it's simply not necessary
/*
        case MSG_CHAR:
            // handle movement keys to move slider
            c >>= 8;

            if (vert) {
                upkey = KEY_UP;
                downkey = KEY_DOWN;
                pgupkey = KEY_PGUP;
                pgdnkey = KEY_PGDN;
                homekey = KEY_END;
                endkey = KEY_HOME;
            }
            else {
                upkey = KEY_RIGHT;
                downkey = KEY_LEFT;
                pgupkey = KEY_PGDN;
                pgdnkey = KEY_PGUP;
                homekey = KEY_HOME;
                endkey = KEY_END;
            }

            if (c == upkey)
                delta = 1;
            else if (c == downkey)
                delta = -1;
            else if (c == pgdnkey)
                delta = -d->d1 / 16;
            else if (c == pgupkey)
                delta = d->d1 / 16;
            else if (c == homekey)
                delta = -d->d2;
            else if (c == endkey)
                delta = d->d1 - d->d2;
            else
                delta = 0;

            if (delta) {
                oldpos = slp;
                oldval = d->d2;

                while (1) {
                    d->d2 = d->d2+delta;
                    slpos = slratio*d->d2;
                    slp = fixtoi(slpos);
                    if ((slp != oldpos) || (d->d2 <= 0) || (d->d2 >= d->d1))
                        break;
                }

                if (d->d2 < 0)
                    d->d2 = 0;

                if (d->d2 > d->d1)
                    d->d2 = d->d1;

                retval = D_USED_CHAR;

                if (d->d2 != oldval) {
                    // call callback function here
                    if (d->dp2) {
                        proc = d->dp2;
                        retval |= (*proc)(d->dp3, d->d2);
                    }
                    //gui_set_screen(screen);
                    object_message(d, MSG_DRAW, 0);
                }
            }
            break;

        case MSG_WHEEL:
            oldval = d->d2;
            d->d2 = CLAMP(0, d->d2+c, d->d1);
            if (d->d2 != oldval) {
                // call callback function here
                if (d->dp2) {
                    proc = d->dp2;
                    retval |= (*proc)(d->dp3, d->d2);
                }
                //gui_set_screen(screen);
                object_message(d, MSG_DRAW, 0);
            }
            break;
*/
        case MSG_CLICK:
#ifndef ALLEGRO5
#ifndef MACOS
            disable_hardware_cursor();
#endif
#endif
            /* track the mouse until it is released */
            mp = slp;

            while (mb=gui_mouse_b()) {

                if (mb==2) return retval;
                msx = gui_mouse_x();
                msy = gui_mouse_y();
                oldval = d->d2;
                if (vert)
                    mp = (d->y+d->h-hmar)-msy;
                else
                    mp = msx-(d->x+hmar);
                if (mp < 0)
                    mp = 0;
                if (mp > irange-hh)
                    mp = irange-hh;
                slpos = itofix(mp);
                slmax = fixdiv(slpos, slratio);
                newpos = fixtoi(slmax);
                if (newpos != oldval) {
                    d->d2 = newpos;

                    /* call callback function here */
                    if (d->dp2 != NULL) {
                        proc = d->dp2;
                        retval |= (*proc)(d->dp3, d->d2);
                    }

                    gui_set_screen(screen);
                    my_object_message(d, MSG_DRAW, 0);
                }

                /* let other objects continue to animate */
                broadcast_dialog_message(MSG_IDLE, 0);
            }
#ifndef ALLEGRO5
#ifndef MACOS
            enable_hardware_cursor();
#endif
#endif
            break;
    }

    return retval;
}

#define SLIDER_OBJECT   1

/* here it comes - the big bad ugly DIALOG array for our main dialog */
SLIDER the_dialog[] =
{
   /* (dialog proc)        (x)   (y)   (w)   (h) (fg)(bg) (key) (flags)     (d1) (d2)    (dp) (dp2) (dp3) */
   /* this element just clears the screen, therefore it should come before the others */
   //{ d_clear_proc,        0,   0,    0,    0,   0,  0,    0,      0,       0,   0,    NULL, NULL, NULL  },
   { d_myslider_proc,       0,   0,  160,   24,   0,  0,    0,      0,     100,   0,    NULL , grab_slider, slider_function},
   { NULL,                  0,   0,    0,    0,   0,  0,    0,      0,       0,   0,    NULL, NULL, NULL  }
};

static int slider(void)
{
   /* set up colors */

   //gui_fg_color = makecol(0, 0, 0);
    gui_fg_color = palette_color[kolory.inkm];
   //gui_mg_color = makecol(128, 128, 128);
    gui_mg_color = palette_color[kolory.borderm];
   //gui_bg_color = makecol(200, 240, 200);
   //gui_bg_color = makecol(200, 200, 200);
    gui_bg_color = palette_color[kolory.paperm];

    gui_border_dark = palette_color[8];
    gui_border_light = palette_color[15];

    set_dialog_color(the_dialog, gui_fg_color, gui_bg_color);

   /* white color for d_clear_proc and the d_?text_procs */
   //the_dialog[0].bg = makecol(0, 0, 0);

   /* shift the dialog 2 pixels away from the border */
   position_dialog(the_dialog, 200, 200);
   
   /* do the dialog */
   do_dialog(the_dialog, -1);

   CUR_OFF(X,Y);
   CUR_ON(X,Y);
   
   return the_dialog[0].d2;
}

