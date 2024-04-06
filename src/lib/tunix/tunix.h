#ifndef _TUNIX_H_
#define _TUNIX_H_

extern void tunix_schedule (void);
extern char tunix_getpid (void);
extern char tunix_fork (void);
extern char tunix_wait (char pid);
extern char tunix_suspend (char pid);
extern char tunix_resume (char pid);
extern char tunix_iopage_alloc (void);
extern char tunix_iopage_commit (char);
extern char tunix_iopage_free (char);

#endif // #ifndef _TUNIX_H_