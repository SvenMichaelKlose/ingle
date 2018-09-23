#ifndef FILE_WINDOW_H
#define FILE_WINDOW_H

#include "window.h"

struct dirent {
    char            name[17];
    unsigned long   size;
    unsigned char   type;
    struct dirent * next;
};

struct drive_ops {
    char              (*opendir)  (void);
    char __fastcall__ (*readdir)  (struct cbm_dirent *);
    void              (*closedir) (void);
    char __fastcall__ (*enterdir) (char * name);
    char __fastcall__ (*open)     (char * name);
    int  __fastcall__ (*read)     (void *, unsigned);
    void              (*close)    (void);
};

struct file_window_content {
    struct obj          obj;
    struct drive_ops *  drive_ops;
    struct dirent *     files;
    int    len;
    int    pos;    /* User's position in list. */
};

struct obj * __fastcall__ make_file_window (char * title, gpos x, gpos y, gsize w, gsize h);

#endif /* #ifndef FILE_WINDOW_H */
