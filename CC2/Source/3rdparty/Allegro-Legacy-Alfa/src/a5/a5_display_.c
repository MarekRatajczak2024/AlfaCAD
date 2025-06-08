/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Stuff for BeOS.
 *
 *      By Jason Wilkins.
 *
 *      See readme.txt for copyright information.
 */

#include <stdio.h>

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/ainta5.h"
#include "allegro/platform/ala5.h"

//just or Linux
#include "xdnd.h"  //by MAREK

void all_render_screen(void);

//AL_LEGACY_VAR(BITMAP *, screen_back);

BITMAP *screen_back=NULL;
//static int screen_no=0;

#define ALLEGRO_LEGACY_PIXEL_FORMAT_8888  0
#define ALLEGRO_LEGACY_PIXEL_FORMAT_OTHER 1

#define VARIANT1

static ALLEGRO_THREAD * _a5_screen_thread = NULL;
static ALLEGRO_BITMAP * _a5_screen = NULL;
static ALLEGRO_COLOR _a5_screen_palette[256];
static uint32_t _a5_screen_palette_a5[256];
static int _a5_screen_format = ALLEGRO_LEGACY_PIXEL_FORMAT_OTHER;

/* display thread data */
static bool _a5_disable_threaded_display = false;
static int _a5_display_width = 0;
static int _a5_display_height = 0;
static int _a5_new_display_flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_GENERATE_EXPOSE_EVENTS | ALLEGRO_DRAG_AND_DROP;//0;
static int _a5_new_bitmap_flags = 0;
static ALLEGRO_TRANSFORM _a5_transform;
static volatile int _a5_display_creation_done = 0;
static ALLEGRO_EVENT_QUEUE * _a5_display_thread_event_queue = NULL;
static ALLEGRO_TIMER * _a5_display_thread_timer = NULL;
static ALLEGRO_EVENT_SOURCE _a5_display_thread_event_source;
static ALLEGRO_EVENT_QUEUE * _a5_display_vsync_event_queue = NULL;

static bool semaphore=1;  //by MAREK
int drop_x = -1;  //by MAREK
int drop_y = -1;  //by MAREK
_xdnd_struct xdnd_buf; //by MAREK
static ALLEGRO_LOCKED_REGION * lr; //MAREK
static int file_no=0;

//static bool state_change_flag=0;
//static bool state_change_hidden=0;

//void reset_state_change_flag(void)
//{
//    state_change_flag=0;
//}

//bool get_state_change_flag(void)
//{
//    return state_change_flag;
//}

//bool get_state_change_hidden(void)
//{
//    return state_change_hidden;
//}

void set_semaphore(bool sem)  //by MAREK
{
    semaphore=sem;
}

bool get_semaphore(void)   //by MAREK
{
    return semaphore;
}

void empty_dropped_fill_buf(void)  //by MAREK
{
    xdnd_buf.mflag=0;
    xdnd_buf.iflag=1;
    xdnd_buf.filebuf[0]='\0';
}

void dropped_fill_buf_activate()  //by MAREK
{
    xdnd_buf.mflag=1;
}

void file_dropped_fill_buf(char *fn)  //by MAREK
{  int i;
    i=0;
    while ((xdnd_buf.mflag!=0) && (i<10)) {i++;}
    xdnd_buf.mflag=0;
    xdnd_buf.iflag=1;
    //memcpy(xdnd_buf.filebuf, fn, strlen(fn));
    strcat(xdnd_buf.filebuf, fn);
    strcat(xdnd_buf.filebuf, "\r\n");
    xdnd_buf.iflag=0;
    //xdnd_buf.mflag=1;
}

static bool _a5_setup_screen(int w, int h)
{
  ALLEGRO_STATE old_state;
  int pixel_format;
  al_set_new_display_flags(_a5_new_display_flags);
  //al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, false, ALLEGRO_REQUIRE); //by MAREK
  //al_set_new_display_option(ALLEGRO_SWAP_METHOD, 1, ALLEGRO_REQUIRE); //by MAREK
  //al_set_new_display_option(ALLEGRO_RENDER_METHOD, 0, ALLEGRO_REQUIRE); //by MAREK
  al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);
  _a5_display = al_create_display(w, h);
  if(!_a5_display)
  {
    goto fail;
  }
  al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
  al_set_new_bitmap_flags(_a5_new_bitmap_flags | ALLEGRO_NO_PRESERVE_TEXTURE);

  _a5_screen = al_create_bitmap(w, h);
  al_restore_state(&old_state);
  if(!_a5_screen)
  {
    goto fail;
  }

  _a5_display_vsync_event_queue = al_create_event_queue();
  if(!_a5_display_vsync_event_queue)
  {
    goto fail;
  }
  al_init_user_event_source(&_a5_display_thread_event_source);
  al_register_event_source(_a5_display_vsync_event_queue, &_a5_display_thread_event_source);

  /* see if we need to hide the mouse cursor */
  if(al_is_mouse_installed() && !(gfx_capabilities & GFX_SYSTEM_CURSOR))
  {
      al_hide_mouse_cursor(_a5_display);
  }

  pixel_format = al_get_bitmap_format(_a5_screen);
  if(pixel_format == ALLEGRO_PIXEL_FORMAT_ARGB_8888 || pixel_format == ALLEGRO_PIXEL_FORMAT_ABGR_8888 || pixel_format == ALLEGRO_PIXEL_FORMAT_RGBA_8888 || pixel_format == ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE)
  {
    _a5_screen_format = ALLEGRO_LEGACY_PIXEL_FORMAT_8888;
  }
  return true;

  fail:
  {
    if(_a5_display_vsync_event_queue)
    {
      al_destroy_event_queue(_a5_display_vsync_event_queue);
      goto fail;
    }
    if(_a5_screen)
    {
      al_destroy_bitmap(_a5_screen);
      _a5_screen = NULL;
    }
    if(_a5_display)
    {
      al_destroy_display(_a5_display);
      _a5_display = NULL;
    }
    return false;
  }
}

static void _a5_destroy_screen(void)
{
  al_destroy_event_queue(_a5_display_vsync_event_queue);
  _a5_display_vsync_event_queue = NULL;
  al_destroy_bitmap(_a5_screen);
  _a5_screen = NULL;
  al_destroy_display(_a5_display);
  _a5_display = NULL;
#ifdef VARIANT1
    destroy_bitmap(screen_back);
    screen_back = NULL;
#endif
}

static void * _a5_display_thread(ALLEGRO_THREAD * thread, void * data)
{
  ALLEGRO_EVENT event;
  float refresh_rate = 60.0; //60.0; //60  120

  if(!_a5_setup_screen(_a5_display_width, _a5_display_height))
  {
    return NULL;
  }
  if(_refresh_rate_request > 0)
  {
    refresh_rate = _refresh_rate_request;
  }

  _a5_display_thread_timer = al_create_timer(1.0 / refresh_rate);
  if(!_a5_display_thread_timer)
  {
    goto fail;
  }
  _a5_display_thread_event_queue = al_create_event_queue();
  if(!_a5_display_thread_event_queue)
  {
    goto fail;
  }
  al_register_event_source(_a5_display_thread_event_queue, al_get_display_event_source(_a5_display));
  al_register_event_source(_a5_display_thread_event_queue, al_get_timer_event_source(_a5_display_thread_timer));

  ////blit(screen, screen_back, 0,0, 0,0,screen->w, screen->h);  //TUTAJ

  al_start_timer(_a5_display_thread_timer);
  _a5_display_creation_done = 1;

  while(!al_get_thread_should_stop(_a5_screen_thread))
  {
    al_wait_for_event(_a5_display_thread_event_queue, &event);
    //if (! semaphore) continue;//by MAREK

    switch(event.type)
    {
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
      {
        if(_a5_close_button_proc)
        {
          _a5_close_button_proc();
        }
        break;
      }
      case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
      {
        _switch_in();
        //state_change_flag=1;
        //state_change_hidden=0;
        break;
      }
      case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
      {
        _switch_out();
        //state_change_flag=1;
        //state_change_hidden=1;
        break;
      }
      case ALLEGRO_EVENT_DROP: //by MAREK
            drop_x = event.drop.x;
            drop_y = event.drop.y;
            if (event.drop.row == 0)
            {
                file_no=0;
                empty_dropped_fill_buf();
            }
            if (event.drop.text)
            {
                printf("%s\n", event.drop.text);
                if (event.drop.is_file)
                {
                    printf("file\n");
                    if (file_no<16)
                    {
                        file_dropped_fill_buf(event.drop.text);
                        file_no++;
                    }
                }
            }
            if (event.drop.is_complete) dropped_fill_buf_activate();
          break;
    }
    if(al_event_queue_is_empty(_a5_display_thread_event_queue))
    {
            all_render_screen();
            event.user.type = ALLEGRO_GET_EVENT_TYPE('V', 'S', 'N', 'C');
            al_emit_user_event(&_a5_display_thread_event_source, &event, NULL);
    }
  }
  if(_a5_display_thread_timer)
  {
    al_destroy_timer(_a5_display_thread_timer);
    _a5_display_thread_timer = NULL;
  }
  if(_a5_display_thread_event_queue)
  {
    al_destroy_event_queue(_a5_display_thread_event_queue);
    _a5_display_thread_event_queue = NULL;
  }
  _a5_destroy_screen();
  return NULL;

  fail:
  {
    if(_a5_display_thread_timer)
    {
      al_destroy_timer(_a5_display_thread_timer);
    }
    if(_a5_display_thread_event_queue)
    {
      al_destroy_event_queue(_a5_display_thread_event_queue);
      _a5_display_thread_event_queue = NULL;
    }
    _a5_destroy_screen();
    return NULL;
  }
}

static BITMAP * a5_display_init(int w, int h, int vw, int vh, int color_depth)
{
    BITMAP * bp;
    //ALLEGRO_STATE old_state;
    //int pixel_format;

    al_set_new_display_flags(_a5_new_display_flags);

    _a5_new_display_flags = al_get_new_display_flags();
    _a5_new_bitmap_flags = al_get_new_bitmap_flags();
    al_identity_transform(&_a5_transform);

#ifdef VARIANT1
    if (screen_back!=NULL) destroy_bitmap(screen_back);
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);
    screen_back = create_system_bitmap(w, h);
    //screen_back = create_bitmap_ex(24, w, h);
    //screen_back = create_bitmap_ex(32, w, h);
    clear_to_color(screen_back, 0x000000);
    //clear_to_color(screen_back, 0xFFFFFFFF);
#endif
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);
    bp = create_system_bitmap(w, h);
    if(bp)
    {
      if(!_a5_disable_threaded_display)
      {
        _a5_display_creation_done = 0;
        _a5_display_width = w;
        _a5_display_height = h;
        _a5_screen_thread = al_create_thread(_a5_display_thread, NULL);
        al_start_thread(_a5_screen_thread);
        while(!_a5_display_creation_done);
      }
      else
      {
        if(!_a5_setup_screen(w, h))
        {
          return NULL;
        }
      }
      gfx_driver->w = bp->w;
      gfx_driver->h = bp->h;
      return bp;
    }
    return NULL;
}

static void a5_display_exit(BITMAP * bp)
{
  if(_a5_screen_thread)
  {
    al_destroy_thread(_a5_screen_thread);
    _a5_screen_thread = NULL;
  }
  else
  {
    _a5_destroy_screen();
  }
}

static void a5_display_vsync(void)
{
  ALLEGRO_EVENT event;

  if(!_a5_disable_threaded_display)
  {
    /* chew up already queued up events so we get only the next vsync */
    while(!al_event_queue_is_empty(_a5_display_vsync_event_queue))
    {
      al_wait_for_event(_a5_display_vsync_event_queue, &event);
    }
    al_wait_for_event(_a5_display_vsync_event_queue, &event);
  }
  else
  {
    al_wait_for_vsync();
  }
}

static void a5_palette_from_a4_palette(const PALETTE a4_palette, ALLEGRO_COLOR * a5_palette, int from, int to)
{
    int i;
    unsigned char r, g, b;

    if(a4_palette)
    {
        for(i = from; i <= to; i++)
        {
            a5_palette[i] = al_map_rgba_f((float)a4_palette[i].r / 63.0, (float)a4_palette[i].g / 63.0, (float)a4_palette[i].b / 63.0, 1.0);

            /* create palette of pre-packed pixels for various pixel formats */
            al_unmap_rgb(a5_palette[i], &r, &g, &b);
            if(al_get_bitmap_format(_a5_screen) == ALLEGRO_PIXEL_FORMAT_ABGR_8888)
            {
                _a5_screen_palette_a5[i] = r | (g << 8) | (b << 16) | (255 << 24);
            }
            else if(al_get_bitmap_format(_a5_screen) == ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE)
            {
                _a5_screen_palette_a5[i] = r | (g << 8) | (b << 16) | (255 << 24);
            }
            else if(al_get_bitmap_format(_a5_screen) == ALLEGRO_PIXEL_FORMAT_ARGB_8888)
            {
                _a5_screen_palette_a5[i] = b | (g << 8) | (r << 16) | (255 << 24);
            }
            else if(al_get_bitmap_format(_a5_screen) == ALLEGRO_PIXEL_FORMAT_RGBA_8888)
            {
                _a5_screen_palette_a5[i] = 255 | (b << 8) | (g << 16) | (r << 24);
            }
        }
    }
}

static void a5_display_set_palette(const struct RGB * palette, int from, int to, int vsync)
{
    if(vsync)
    {
      a5_display_vsync();
    }
    a5_palette_from_a4_palette(palette, _a5_screen_palette, from, to);
}

static void a5_display_move_mouse(int x, int y)
{
  al_set_mouse_xy(_a5_display, x, y);
}

static int a5_display_show_mouse(BITMAP * bp, int x, int y)
{
  if(bp)
  {
    return -1;
  }
  al_show_mouse_cursor(_a5_display);
  a5_display_move_mouse(x, y);
  return 0;
}

static void a5_display_hide_mouse(void)
{
  al_hide_mouse_cursor(_a5_display);
}

static ALLEGRO_COLOR a5_get_color(int depth, int color)
{
    int r, g, b, a;

    switch(depth)
    {
        case 15:
        {
            r = getr15(color);
            g = getg15(color);
            b = getb15(color);
            a = 0xFF;
            break;
        }
        case 16:
        {
            r = getr16(color);
            g = getg16(color);
            b = getb16(color);
            a = 0xFF;
            break;
        }
        case 24:
        {
            r = getr24(color);
            g = getg24(color);
            b = getb24(color);
            a = 0xFF;
            break;
        }
        case 32:
        {
            r = getr32(color);
            g = getg32(color);
            b = getb32(color);
            a = geta32(color);
            ////a = 0xFF;
            break;
        }
    }

    return al_map_rgba(r, g, b, a);
}

/* render 8-bit BITMAP to 32-bit ALLEGRO_BITMAP (8888 formats) */
static void render_8_8888(BITMAP * bp, ALLEGRO_BITMAP * a5bp)
{
    ALLEGRO_LOCKED_REGION * lr;
    uint8_t * line_8;
    uint32_t * line_32;
    int i, j;

    lr = al_lock_bitmap(a5bp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
    if(lr)
    {
        line_8 = lr->data;
        line_32 = lr->data;
        for(i = 0; i < bp->h; i++)
        {
            for(j = 0; j < bp->w; j++)
            {
                line_32[j] = _a5_screen_palette_a5[bp->line[i][j]];
            }
            line_8 += lr->pitch;
            line_32 = (uint32_t *)line_8;
        }
        al_unlock_bitmap(a5bp);
    }
}

static void render_other_8(BITMAP * bp)
{
    uint8_t * line_8;
    int i, j;

    for(i = 0; i < bp->h; i++)
    {
        for(j = 0; j < bp->w; j++)
        {
            line_8 = (uint8_t *)(bp->line[i]);
            al_put_pixel(j, i, _a5_screen_palette[line_8[j]]);
        }
    }
}

static void render_other_15(BITMAP * bp)
{
    uint16_t * line_16;
    int i, j;

    for(i = 0; i < bp->h; i++)
    {
        for(j = 0; j < bp->w; j++)
        {
            line_16 = (uint16_t *)(bp->line[i]);
            al_put_pixel(j, i, a5_get_color(15, line_16[j]));
        }
    }
}

static void render_other_16(BITMAP * bp)
{
    uint16_t * line_16;
    int i, j;

    for(i = 0; i < bp->h; i++)
    {
        for(j = 0; j < bp->w; j++)
        {
            line_16 = (uint16_t *)(bp->line[i]);
            al_put_pixel(j, i, a5_get_color(16, line_16[j]));
        }
    }
}
/*
static void render_other_24_(BITMAP * bp)
{
    int i, j;

    for(i = 0; i < bp->h; i++)
    {
        for(j = 0; j < bp->w; j++)
        {
            al_put_pixel(j, i, a5_get_color(24, _getpixel24(bp, j, i)));
        }
    }
}
*/
typedef struct RGB_
{
    unsigned char r, g, b;
} RGB_;

static void render_other_24(BITMAP * bp)
{
    RGB_ *line_24;
    int i, j;
    ALLEGRO_COLOR col;
    col.a = 0xFF;

    //method line by pixel with color conversion
    for(i = 0; i < bp->h; i++)
    {
        line_24 = (RGB_ *)(bp->line[i]);
        for(j = 0; j < bp->w; j++)
        {
            col.r = line_24[j].r;
            col.g = line_24[j].g;
            col.b = line_24[j].b;
            //col.a = 0xFF;
            al_put_pixel(j, i, al_map_rgba(col.r, col.g, col.b, col.a));
        }
    }
}


static void render_other_32(BITMAP * bp)
{
    uint32_t * line_32;
    int i, j;
    //ALLEGRO_COLOR col;
    RGB col;
    //uint32_t col_32;
    struct ALLEGRO_COLOR col32;
    unsigned int col32i;

    //method line by pixel with color conversion
    for(i = 0; i < bp->h; i++)
    {
        line_32 = (uint32_t *)(bp->line[i]);
        for(j = 0; j < bp->w; j++)
        {
            ////line_32 = (uint32_t *)(bp->line[i]); ////
            ////al_put_pixel(j, i, a5_get_color(32, line_32[j]));
/*
            col.r = (float)getr32(line_32[j]);
            col.g = (float)getg32(line_32[j]);
            col.b = (float)getb32(line_32[j]);
            ////col.a = (float)geta32(line_32[j]);
            col.a = (float)0xFF;
*/
/*
            col.r = getr32(line_32[j]);
            col.g = getg32(line_32[j]);
            col.b = getb32(line_32[j]);
            ////col.a = geta32(line_32[j]);
            col.filler = 0xFF;
*/
            ////col32i=line_32[j] |= 0xFF;
            ////memmove(&col, &col32i, 4);

            col32.r = (float)getr32(line_32[j]);
            col32.g = (float)getg32(line_32[j]);
            col32.b = (float)getb32(line_32[j]);
            ////col.a = geta32(line_32[j]);
            col32.a = (float)0xFF;

            ////al_put_pixel(j, i, al_map_rgba(col.r, col.g, col.b, col.filler));
            al_put_pixel(j, i, col32);

            ////col_32=line_32[j]>>8;
            ////col_32=(col_32<<8) + 0xFF;
            ////al_put_pixel(j, i, &col_32);

        }
    }
}

static void render_other_32_(BITMAP * bp)
{
    uint32_t * line_32;
    int i, j;
    //ALLEGRO_COLOR col;

    //method line by pixel with color conversion
    for(i = 0; i < bp->h; i++)
    {
        line_32 = (uint32_t *)(bp->line[i]);
        for(j = 0; j < bp->w; j++)
        {
            ////line_32 = (uint32_t *)(bp->line[i]); ////
            al_put_pixel(j, i, a5_get_color(32, line_32[j]));

            //col.r = getr32(line_32[j]);
            //col.g = getg32(line_32[j]);
            //col.b = getb32(line_32[j]);
            //col.a = geta32(line_32[j]);
            //col.a = 0xFF;
            //al_put_pixel(j, i, al_map_rgba(col.r, col.g, col.b, col.a));
        }
    }
}

static void render_other(BITMAP * bp, ALLEGRO_BITMAP * a5bp)
{
    ALLEGRO_STATE old_state;
    int depth;

    if (bp==NULL) return;

    depth = bitmap_color_depth(bp);
    al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP);
    al_set_target_bitmap(a5bp);
    lr=al_lock_bitmap(a5bp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
    switch(depth)
    {
        case 8:
        {
            render_other_8(bp);
            break;
        }
        case 15:
        {
            render_other_15(bp);
            break;
        }
        case 16:
        {
            render_other_16(bp);
            break;
        }
        case 24:
        {
            render_other_24(bp);
            break;
        }
        case 32:
        {
            render_other_32(bp);
            break;
        }
    }
    al_unlock_bitmap(a5bp);
    al_restore_state(&old_state);
}

static void render_other_(BITMAP * bp, ALLEGRO_BITMAP * a5bp)
{
    ALLEGRO_STATE old_state;
    int depth;

    if (bp==NULL) return;

    depth = bitmap_color_depth(bp);
    al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP);
    al_set_target_bitmap(a5bp);
    al_lock_bitmap(a5bp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
    switch(depth)
    {
        case 8:
        {
            render_other_8(bp);
            break;
        }
        case 15:
        {
            render_other_15(bp);
            break;
        }
        case 16:
        {
            render_other_16(bp);
            break;
        }
        case 24:
        {
            render_other_24(bp);
            break;
        }
        case 32:
        {
            render_other_32(bp);
            //render_other_32_m(bp, a5bp);
            break;
        }
    }
    al_unlock_bitmap(a5bp);
    al_restore_state(&old_state);
}

ALLEGRO_DISPLAY * all_get_display(void)
{
  return _a5_display;
}

void all_render_a5_bitmap(BITMAP * bp, ALLEGRO_BITMAP * a5bp)
{
    int depth;

    depth = bitmap_color_depth(bp);
    if(depth == 8 && _a5_screen_format == ALLEGRO_LEGACY_PIXEL_FORMAT_8888)
    {
        render_8_8888(bp, a5bp);
    }
    else
    {
        render_other(bp, a5bp);
    }
}

void all_render_a5_bitmap_(BITMAP * bp, ALLEGRO_BITMAP * a5bp)
{
    int depth;

    depth = bitmap_color_depth(bp);
    if(depth == 8 && _a5_screen_format == ALLEGRO_LEGACY_PIXEL_FORMAT_8888)
    {
        render_8_8888(bp, a5bp);
    }
    else
    {
        render_other_(bp, a5bp);
    }
}

ALLEGRO_BITMAP * all_get_a5_bitmap(BITMAP * bp)
{
    ALLEGRO_BITMAP * bitmap;

    bitmap = al_create_bitmap(bp->w, bp->h);
    if(bitmap)
    {
        all_render_a5_bitmap(bp, bitmap);
        return bitmap;
    }
    return NULL;
}

ALLEGRO_BITMAP * all_get_a5_bitmap_(BITMAP * bp)
{
    ALLEGRO_BITMAP * bitmap;

    bitmap = al_create_bitmap(bp->w, bp->h);
    if(bitmap)
    {
        all_render_a5_bitmap_(bp, bitmap);
        return bitmap;
    }
    return NULL;
}

void flip_display(void)
{
   al_flip_display();
}

void all_render_screen(void)
{
    if (get_semaphore()) //by MAREK
    {
#ifndef VARIANT1
        al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);
        screen_back = create_system_bitmap(screen->w, screen->h);
#endif
        //by blit
        blit(screen, screen_back, 0,0, 0,0,screen->w, screen->h);
        ////by memmove
        ////memmove(screen_back->dat, screen->dat, screen->w*screen->h*4);

        all_render_a5_bitmap(screen_back, _a5_screen); //looks like copy to screen_back is not necessary
        //all_render_a5_bitmap(screen, _a5_screen);

        al_use_transform(&_a5_transform);
        al_draw_bitmap(_a5_screen, 0, 0, 0);
        al_flip_display();
#ifndef VARIANT1
        destroy_bitmap(screen_back);
#endif
    }
}

void all_disable_threaded_display(void)
{
  _a5_disable_threaded_display = true;
}

void all_set_display_transform(ALLEGRO_TRANSFORM * transform)
{
  al_copy_transform(&_a5_transform, transform);
}

GFX_DRIVER display_allegro_5 = {
   GFX_ALLEGRO_5,                     // int id;
   empty_string,                      // char *name;
   empty_string,                      // char *desc;
   "Allegro 5 Display",               // char *ascii_name;
   a5_display_init,   // AL_LEGACY_METHOD(struct BITMAP *, init, (int w, int h, int v_w, int v_h, int color_depth));
   a5_display_exit, //be_gfx_bwindowscreen_exit,         // AL_LEGACY_METHOD(void, exit, (struct BITMAP *b));
   NULL, //be_gfx_bwindowscreen_scroll,       // AL_LEGACY_METHOD(int, scroll, (int x, int y));
   a5_display_vsync, //be_gfx_vsync,                      // AL_LEGACY_METHOD(void, vsync, (void));
   a5_display_set_palette,  // AL_LEGACY_METHOD(void, set_palette, (struct RGB *p, int from, int to, int vsync));
   NULL, //be_gfx_bwindowscreen_request_scroll,// AL_LEGACY_METHOD(int, request_scroll, (int x, int y));
   NULL, //be_gfx_bwindowscreen_poll_scroll,  // AL_LEGACY_METHOD(int, poll_scroll, (void));
   NULL,                              // AL_LEGACY_METHOD(void, enable_triple_buffer, (void));
   NULL,                              // AL_LEGACY_METHOD(struct BITMAP *, create_video_bitmap, (int width, int height));
   NULL,                              // AL_LEGACY_METHOD(void, destroy_video_bitmap, (struct BITMAP *bitmap));
   NULL,                              // AL_LEGACY_METHOD(int, show_video_bitmap, (struct BITMAP *bitmap));
   NULL, //be_gfx_bwindowscreen_request_video_bitmap,// AL_LEGACY_METHOD(int, request_video_bitmap, (struct BITMAP *bitmap));
   NULL,                              // AL_LEGACY_METHOD(struct BITMAP *, create_system_bitmap, (int width, int height));
   NULL,                              // AL_LEGACY_METHOD(void, destroy_system_bitmap, (struct BITMAP *bitmap));
   NULL,                              // AL_LEGACY_METHOD(int, set_mouse_sprite, (struct BITMAP *sprite, int xfocus, int yfocus));
   a5_display_show_mouse,                              // AL_LEGACY_METHOD(int, show_mouse, (struct BITMAP *bmp, int x, int y));
   a5_display_hide_mouse,                              // AL_LEGACY_METHOD(void, hide_mouse, (void));
   a5_display_move_mouse,                              // AL_LEGACY_METHOD(void, move_mouse, (int x, int y));
   NULL,                              // AL_LEGACY_METHOD(void, drawing_mode, (void));
   NULL,                              // AL_LEGACY_METHOD(void, save_state, (void));
   NULL,                              // AL_LEGACY_METHOD(void, restore_state, (void));
   NULL,                              // AL_LEGACY_METHOD(void, set_blender_mode, (int mode, int r, int g, int b, int a));
   NULL, //be_gfx_bwindowscreen_fetch_mode_list,// AL_LEGACY_METHOD(int, fetch_mode_list, (void));
   0, 0,                              // int w, h;  /* physical (not virtual!) screen size */
   TRUE,                              // int linear;  /* true if video memory is linear */
   0,                                 // long bank_size;  /* bank size, in bytes */
   0,                                 // long bank_gran;  /* bank granularity, in bytes */
   0,                                 // long vid_mem;  /* video memory size, in bytes */
   0,                                 // long vid_phys_base;  /* physical address of video memory */
   TRUE                              // int windowed;  /* true if driver runs windowed */
};


///////////////EXTRAS
int get_display_width(void)
{
    return al_get_display_width(_a5_display);
}

int get_display_height(void)
{
    return al_get_display_height(_a5_display);
}

void get_window_position(int *x, int *y)
{
    al_get_window_position(_a5_display, x, y);
}

bool get_window_constraints(int *min_w, int *min_h, int *max_w, int *max_h)
{
    return al_get_window_constraints(_a5_display, min_w, min_h, max_w, max_h);
}


bool set_window_constraints(int min_w, int min_h, int max_w, int max_h)
{
    return al_set_window_constraints(_a5_display, min_w, min_h, max_w, max_h);
}

void set_window_position(int x, int y)
{
    al_set_window_position(_a5_display, x, y);
}

int get_x_window_id(void)
{
    return al_get_x_window_id(_a5_display);
}

void set_display_icon(BITMAP *icon)
{
    ALLEGRO_BITMAP *a5_icon=all_get_a5_bitmap_(icon);
    al_set_display_icon(_a5_display, a5_icon);
}

void load_set_display_icon(char *icon_file)
{
    ALLEGRO_BITMAP *a5_icon=al_load_bitmap(icon_file);
    al_set_display_icon(_a5_display, a5_icon);
}

int get_display_flags(void)
{
    return al_get_display_flags(_a5_display);
}

void set_timer_speed(int speed)
{
    al_set_timer_speed(_a5_display_thread_timer, 1. / speed);
}