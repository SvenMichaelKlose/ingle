#ifndef TABLE_H
#define TABLE_H

#define MAX_TABLE_COLUMNS   32

extern struct obj_ops table_ops;
extern struct obj_ops table_ops_center;

extern struct obj * make_table (void);
extern void __fastcall__ draw_table (void *);

#endif /* #ifndef TABLE_H */
