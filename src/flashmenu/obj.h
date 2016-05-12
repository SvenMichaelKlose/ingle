#ifndef OBJ_H
#define OBJ_H

#include "libgfx.h"

#define OBJ(x)  ((struct obj *) x)

typedef unsigned char uchar;

struct _configuration {
    char type;
};

struct rect {
    gpos x;
    gpos y;
    gsize w;
    gsize h;
};

struct treenode {
    char            type;
    struct obj *    prev;
    struct obj *    next;
    struct obj *    parent;
    struct obj *    children;
};

typedef void __fastcall__ (*func_draw_t) (void *);
typedef void __fastcall__ (*func_layout_t) (struct obj *);

struct obj_ops {
    func_draw_t     draw;
    func_layout_t   layout;
};

struct obj {
    struct treenode   node;
    struct rect       rect;
    struct obj_ops *  ops;
};

struct scrollable {
    struct obj  obj;
    char        bank;
};

extern void * __fastcall__ alloc_obj (size_t size, gpos x, gpos y, gsize w, gsize h, struct obj_ops *);
extern void __fastcall__ free_obj (struct obj *);
extern void __fastcall__ draw_obj (struct obj *);
extern void __fastcall__ draw_obj_children (struct obj *);
extern void __fastcall__ layout_obj (struct obj *);
extern void __fastcall__ layout_obj_children (struct obj *);
extern void __fastcall__ append_obj (struct obj * parent, struct obj * x);
extern void __fastcall__ copy_obj_ops (struct obj_ops * dest, struct obj_ops * src);
extern void __fastcall__ set_obj_ops (struct obj *, struct obj_ops *);
extern void __fastcall__ set_obj_region (struct obj *);

#endif /* #ifndef OBJ_H */
