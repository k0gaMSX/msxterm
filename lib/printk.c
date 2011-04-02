/* -*- mode: c ; coding: iso-8859-1 -*- */

/**
 * This is a really non portable and limited implementation of
 * a printf alike function. We don't need anything more for
 * printing kernel messages, and this implementation is
 * very small and fast.
 */

#include "arch/stdarg.h"
#include "arch/stddef.h"
#include "arch/string.h"

#include "tnix/vt100.h"

#define isdigit(c) ((c) >= '0' &&  (c) <= '9')

static const unsigned base10[] = {10000, 1000, 100, 10, 0};
static const unsigned base7[] = {7*7*7*7*7, 7*7*7*7, 7*7*7, 7*7, 7, 0};
static const unsigned *base;



#define p_itoa10(x,y) (base = base10, p_itoaX(x, y))
#define p_itoa8(x,y) (base = base7, p_itoaX(x, y))


/*
 * Convert a number in a string of base octal or decimal.
 * You must set base pointer before calling this function.
 * Result string will have a lenght of 5 digits and '\0'
 * character.
 */


static void p_itoaX(unsigned val, char *dst)
{
     do {
          *dst = '0';
          while (val >= *base) {
               val -= *base;
               ++*dst;
          }

          ++dst, ++base;
     } while (*base);

     *dst++ = val + '0';
     *dst = '\0';
}




/*
 * Convert 16 bits integer into 5 hexadecimal digit string
 * and '\0' character
 */

static void p_itoa16(unsigned val, char *dst)
{
     static const unsigned char base16[] = "0123456789abcdef";

     *dst++ = base16[((val >> 8) & 0xf0) >> 4]; /* hitech-c */
     *dst++ = base16[(val >> 8) & 0x0f];        /* understand >> 8 as*/
     *dst++ = base16[(val &0xf0) >> 4];         /* high byte of 16 */
     *dst++ = base16[val & 0x0f];               /* bit registers */
     *dst = '\0';
}





/*
 * This function print a string based in a format string. Format
 * specifier match %0wt, where optional 0 means filling with 0 instead of
 * spaces, optional w is wide field (only a digit) and t can be one of
 * following:
 *     c: character
 *     s: string
 *     p: pointer (assumed 16 bits pointers)
 *     d: unsigned decimal integer (assumed 16 bits)
 *     x: unsigned hexadecimal integer (assumed 16 bits)
 *     o: unsigned octal integer (assumed 16 bits)
 * Other values of t are directly written to the output.
 */


size_t vsprintk(char * buf, const char *fmt, va_list args)
{
     static unsigned char pad, width, len;
     static char number[10], *str, *bp;

     for (bp = buf; *fmt; ++fmt) {
          if (*fmt != '%') {
               *bp++ = *fmt;
               continue;
          }

          if (*++fmt == '0')
               pad = *fmt++;
          else
               pad = ' ';

          width =  (isdigit(*fmt)) ? *fmt++ - '0' : 0;
          str = number;

          switch (*fmt) {
          case 's':
               str = va_arg(args, char *);
               len = strlen(str);
               goto string;

          case 'p':             /* limits pointer to 16 bits */
          case 'x':
               p_itoa16(va_arg(args, unsigned), number);
               len = 4;
               goto numeric;

          case 'o':
               p_itoa8(va_arg(args, unsigned), number);
               len = 6;
               goto numeric;

          case 'd':
               p_itoa10(va_arg(args, unsigned), number);
               len = 5;
               goto numeric;

          case 'c':
               len = 1;
               number[0] = va_arg(args, int);
               goto string;

          default:
               *bp++ = *fmt++;
               continue;
          }

     numeric:
          while (*str == '0')
               ++str, --len;    /* increment pointer, decrement size */

          if (!len)
               --str, ++len;    /* decrement pointer, increment size */

     string:
          if (width > len) {
               width -= len;
               memset(bp, pad, width);
               bp += width;
          }

          strcpy(bp, str);
          bp += len;
     }

     return bp - buf;
}



static char buffer[1024];

size_t printk(char * bp, const char *fmt, ...)
{
     va_list args;
     size_t cnt;
     va_start(args, fmt);
     cnt = vsprintk(buffer, fmt, args);
     term_write(buffer, cnt);
     va_end(args);

     return cnt;
}
