#ifndef __ASSERT_H_
#define __ASSERT_H_


#ifndef NDEBUG
#define TOK(x) #x
   extern int __fail_assert(char * exp, char * file, int line);
   #define assert(x)                                                       \
             ((void) ((x) ? 0 : __fail_assert(TOK(x), __FILE__, __LINE__)))
#else
   #define assert(x)
#endif

#endif /* __ASSERT_H_ */
