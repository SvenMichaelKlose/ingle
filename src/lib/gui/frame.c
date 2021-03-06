#include <stdlib.h>

#include "libgfx.h"
#include "obj.h"
#include "frame.h"
#include "layout-ops.h"
#include "message.h"

void __fastcall__ draw_frame (struct obj *);

struct obj_ops frame_ops = {
    draw_frame,
    layout_none,
    obj_noop
};

struct obj *
make_frame ()
{
    struct obj * f = alloc_obj (sizeof (struct obj), &frame_ops);
    return f;
}

void __fastcall__
draw_frame (struct obj * f)
{
    gfx_push_context ();
    set_obj_region (f);
    draw_obj_children (f);
    gfx_pop_context ();
}
