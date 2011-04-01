
#ifndef ARCH_H_
#define ARCH_H_

#define di()     asm("di")
#define ei()     asm("ei")
#define halt()   asm("halt")
#define im0()    asm("im 0");


void arch_init(void);

#endif /* ARCH_H_ */




