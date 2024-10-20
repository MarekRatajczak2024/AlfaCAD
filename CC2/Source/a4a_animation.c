/*
 * al4anim is a simple animation library for Allegro 4.
 * Copyright (C) 2010 Brandon McCaig
 *
 * This file is part of al4anim.
 *
 * al4anim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * al4anim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with al4anim.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "a4a_animation.h"

#include "leak_detector_c.h"

a4a_animation_t * a4a_animation_create(
        a4a_sprite_type_t type,
        int ticks_per_frame,
        int num_frames,
        ...)
{
    va_list ap;

    va_start(ap, num_frames);

    a4a_animation_t * a = a4a_animation_createv(
            type,
            ticks_per_frame,
            num_frames,
            ap);

    va_end(ap);

    return a;
}

a4a_animation_t * a4a_animation_createa(
        a4a_sprite_type_t type,
        int ticks_per_frame,
        int num_frames,
        char * filenames[])
{
    assert(filenames);

    int i = -1;
    BITMAP ** frames = 0;
    BITMAP * (*load)(const char *, RGB * pal);

    switch(type)
    {
        case A4A_SPRITE_TYPE_BMP:
            load = load_bitmap;
            break;
        case A4A_SPRITE_TYPE_PNG:
            load = load_png;
            break;
        default:
            return 0;
    }

    a4a_animation_t * a = malloc(sizeof(a4a_animation_t));

    if(!a)
        return 0;

    frames = malloc(sizeof(BITMAP **) * num_frames);

    if(!frames)
        goto error;

    for(i=0; i<num_frames; i++)
    {
        char * filename = filenames[i];

        assert(filename);

        frames[i] = load(filename, 0);

        if(!frames[i])
        {
             i--;
             goto error;
        }
    }

    a->frames_ = frames;
    a->num_frames_ = num_frames;
    a->own_frames_ = 1;
    a->start_ticks_ = 0;
    a->ticks_per_frame_ = ticks_per_frame;

    return a;

error:
    for(; i>=0; i--)
        destroy_bitmap(frames[i]);

    free(frames);

    free(a);

    return 0;
}

a4a_animation_t * a4a_animation_createb(
        int ticks_per_frame,
        int num_frames,
        int own_frames,
        BITMAP * frames[])
{
    assert(frames);

    int i;

    a4a_animation_t * a = malloc(sizeof(a4a_animation_t));

    if(!a)
        return 0;

    BITMAP ** tmp = malloc(sizeof(BITMAP *) * num_frames);

    if(!tmp)
    {
        free(a);
        return 0;
    }

    for(i=0; i<num_frames; i++)
    {
        tmp[i] = frames[i];
    }

    a->frames_ = tmp;
    a->num_frames_ = num_frames;
    a->own_frames_ = own_frames;
    a->start_ticks_ = 0;
    a->ticks_per_frame_ = ticks_per_frame;

    return a;
}

a4a_animation_t * a4a_animation_createf(
        a4a_sprite_type_t type,
        int ticks_per_frame,
        int num_frames,
        const char * filename_format)
{
    assert(filename_format);

    a4a_animation_t * a = 0;
    char ** filenames = 0;
    int i = -1;
    int maxlen = (int)strlen(filename_format) * 2;

    filenames = malloc(sizeof(char *) * num_frames);

    if(!filenames)
        return 0;

    for(i=0; i<num_frames; i++)
    {
        char * filename;
        int len;

        filename = malloc(sizeof(char) * (maxlen + 1));

        if(!filename)
        {
            i--;
            goto cleanup;
        }

        len = snprintf(filename, maxlen, filename_format, i + 1);

        filename[len] = 0;

        filenames[i] = filename;
    }

    i--;

    a = a4a_animation_createa(
            type,
            ticks_per_frame,
            num_frames,
            filenames);

cleanup:
    for(; i>=0; i--)
        free(filenames[i]);

    free(filenames);

    return a;
}

a4a_animation_t * a4a_animation_createv(
        a4a_sprite_type_t type,
        int ticks_per_frame,
        int num_frames,
        va_list ap)
{
    int i;

    char ** filenames = malloc(sizeof(char *) * num_frames);

    if(!filenames)
        return 0;

    for(i=0; i<num_frames; i++)
        filenames[i] = va_arg(ap, char *);

    a4a_animation_t * a = a4a_animation_createa(
            type,
            ticks_per_frame,
            num_frames,
            filenames);

    free(filenames);

    return a;
}

void a4a_animation_destroy(a4a_animation_t ** p_a)
{
    int i;

    assert(p_a);

    a4a_animation_t * a = *p_a;

    if(a)
    {
        if(a->own_frames_)
            for(i=0; i<a->num_frames_; i++)
                destroy_bitmap(a->frames_[i]);

        free(a->frames_);
        free(a);
        *p_a = 0;
    }
}

BITMAP * a4a_animation_begin(a4a_animation_t * a, int ticks)
{
    assert(a);

    a->start_ticks_ = ticks;

    return a4a_animation_frame(a, ticks);
}

BITMAP * a4a_animation_frame(a4a_animation_t * a, int ticks)
{
    assert(a);
    assert(ticks >= a->start_ticks_);

    int past = (ticks - a->start_ticks_) / a->ticks_per_frame_;
    int current_frame_ = past % (a->num_frames_ - 1);

    return a->frames_[current_frame_];
}

