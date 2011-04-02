#ifndef __STDARG_H_
#define __STDARG_H_

typedef void *va_list[1];
#define _STACK_ALIGN 2

#define va_start(ap, parmn)                                       \
     (*ap = (char *)&parmn + ((sizeof(parmn) + _STACK_ALIGN - 1 ) \
                              &~(_STACK_ALIGN-1)))

#define va_arg(ap, type)   (*(*(type **)ap)++)
#define va_end(ap)

#endif /* __STDARG_H_ */


