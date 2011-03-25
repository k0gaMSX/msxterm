
#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef HAVE_INTTYPES_H
  #include <inttypes.h>
#else
  typedef unsigned char uint8_t;
  typedef signed char int8_t;

  typedef unsigned short uint16_t;
  typedef signed short int16_t;

  typedef unsigned long uint32_t;
  typedef unsigned long int32_t;
#endif

#endif /* _TYPES_H_ */
