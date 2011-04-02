#ifndef _STDDEF_H_
#define _STDDEF_H_

typedef int             ptrdiff_t;
typedef unsigned        size_t;
typedef unsigned short  wchar_t;
#define offsetof(ty, mem)      ((int)&(((ty *)0)->mem))
extern int errno;

#ifndef NULL
#define NULL ((void *) 0)
#endif

#endif /* _STDDEF_H_ */
