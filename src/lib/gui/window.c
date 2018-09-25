#include <stdlib.h>
#include <string.h>

#include "libgfx.h"
#include "obj.h"
#include "layout-ops.h"
#include "event.h"
#include "window.h"
#include "message.h"
#include "frame.h"

extern struct obj * focussed_window;

#define WINDOW_TITLE_HEIGHT     11

void __fastcall__ layout_window_content_frame (struct obj *);

struct obj_ops window_ops = {
    draw_window,
    layout_window_content_frame,
    obj_noop,
    event_handler_passthrough
};

void __fastcall__
draw_window_content (struct obj * o)
{
    gfx_push_context ();
    gfx_reset_region ();
    set_obj_region (o);
    gfx_set_pattern (pattern_empty);
    gfx_draw_box (0, 0, o->rect.w, o->rect.h);
    gfx_pop_context ();
    gfx_push_context ();
    draw_obj_children (OBJ(o));
    gfx_pop_context ();
}

void __fastcall__
free_window (struct obj * o)
{
    free (o->ops);
}

struct obj_ops window_content_ops = {
    draw_window_content,
    layout_window_content_frame,
    free_window,
    event_handler_passthrough
};

struct window * __fastcall__
make_window (char * title, struct obj * content, event_handler_t event_handler)
{
    struct obj_ops * ops = malloc (sizeof (struct obj_ops));
    struct window * win;

    memcpy (ops, &window_ops, sizeof (struct obj_ops));
    win = alloc_obj (sizeof (struct window), ops);
    if (!content)
        content = alloc_obj (sizeof (struct obj), &window_content_ops);
    append_obj (OBJ(win), content);
    if (event_handler)
        ops->event_handler = event_handler;
    win->title = title;
    return win;
}

void __fastcall__
draw_title_grip (gpos y, gpos yb, gsize iw)
{
    gpos   cx;
    gsize  gw;

    cx = gfx_x ();
    if (cx > iw)
        return;

    gw = iw - cx;
    for (; y < yb; y += 2)
        gfx_draw_hline (cx, y, gw);
}

void __fastcall__
draw_window_fullscreen (struct window * win)
{
    struct rect * r = &win->obj.rect;
    gpos   x = 0;
    gpos   y = WINDOW_TITLE_HEIGHT - 1;
    gsize  w = 160;
    gsize  h = 183;
    gsize  iw = r->w - 2;
    gpos   yb = WINDOW_TITLE_HEIGHT - 3;
    gpos   ch = h - WINDOW_TITLE_HEIGHT;

    /* Draw window title. */
    gfx_push_context ();
    gfx_reset_region ();
    gfx_set_region (0, 0, w, WINDOW_TITLE_HEIGHT);
    gfx_set_pencil_mode (PENCIL_MODE_OR);
    gfx_set_pattern (pattern_empty);
    gfx_draw_box (0, 0, w, WINDOW_TITLE_HEIGHT - 2);
    gfx_set_pattern (pattern_solid);
    gfx_draw_hline (0, WINDOW_TITLE_HEIGHT - 2, w);
    gfx_set_font (charset_4x8, 2);
    gfx_draw_text (1, 1, win->title);
    if (focussed_window == (struct obj *) win)
        draw_title_grip (1, yb, iw);
    gfx_pop_context ();

    /* Draw contents. */
    gfx_push_context ();
    draw_obj_children (OBJ(win));
    gfx_pop_context ();
}


void __fastcall__
draw_window_standard (struct window * win)
{
    struct rect * r = &win->obj.rect;
    gpos   x = r->x;
    gpos   y = r->y;
    gsize  w = r->w;
    gsize  h = r->h;
    gsize  iw = r->w - 2;
    gpos   yb = y + WINDOW_TITLE_HEIGHT - 1;
    gpos   ch = h - WINDOW_TITLE_HEIGHT;

    /* Draw window title. */
    gfx_push_context ();
    gfx_set_region (x, y, w, WINDOW_TITLE_HEIGHT);
    gfx_set_pencil_mode (PENCIL_MODE_OR);
    gfx_set_font (charset_4x8, 2);
    gfx_set_pattern (pattern_empty);
    gfx_draw_box (1, 1, w - 2, WINDOW_TITLE_HEIGHT - 2);
    gfx_set_pattern (pattern_solid);
    gfx_draw_frame (0, 0, w, WINDOW_TITLE_HEIGHT);
    gfx_draw_text (2, 2, win->title);
    if (focussed_window == (struct obj *) win)
        draw_title_grip (2, yb, iw);
    gfx_pop_context ();

    /* Draw content frame. */
    gfx_push_context ();
    gfx_set_pattern (pattern_solid);
    gfx_set_region (x, y + WINDOW_TITLE_HEIGHT - 1, w, ch);
    gfx_draw_frame (0, 0, w, h - WINDOW_TITLE_HEIGHT);
    gfx_pop_context ();

    /* Draw contents. */
    gfx_push_context ();
    draw_obj_children (OBJ(win));
    gfx_pop_context ();
}

void __fastcall__
draw_window (struct obj * _w)
{
    struct window * win = (struct window *) _w;

    if (win->flags & W_FULLSCREEN)
        draw_window_fullscreen (win);
    else
        draw_window_standard (win);
}

void
layout_window_content_frame (struct obj * o)
{
    struct window * win = (struct window *) o;
    gpos w = o->rect.w;
    gpos h = o->rect.h;

    if (win->flags & W_FULLSCREEN)
        set_obj_position_and_size (o->node.children, 0, WINDOW_TITLE_HEIGHT - 1, w, h - WINDOW_TITLE_HEIGHT + 1);
    else
        set_obj_position_and_size (o->node.children, 1, WINDOW_TITLE_HEIGHT, w - 2, h - WINDOW_TITLE_HEIGHT - 1);

    layout_obj_children (o);
}
