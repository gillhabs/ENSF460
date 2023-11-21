/*
 * File:   TimeDelay.h
 * Author: Halanna Le, Christy Guirguis, Gillian Habermehl
 *
 * Created on October 11, 2023, 2:24 PM
 */

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */


void delay_ms(uint16_t time_ms);
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void);

#endif	/* XC_HEADER_TEMPLATE_H */

