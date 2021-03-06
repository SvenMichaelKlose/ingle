#ifndef LIST_H
#define LIST_H

#include "obj.h"

#define LIST(x)     ((struct list *) x)

extern struct obj_ops list_ops;

#define LIST_HORIZONTAL     0
#define LIST_VERTICAL       1

struct list {
    struct obj  obj;
    char        orientation;
};

extern struct list * __fastcall__ make_list (char orientation);
extern void          __fastcall__ draw_list (struct obj *);

#endif /* #ifndef LIST_H */
