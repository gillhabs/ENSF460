/*
 * Project: ENSF460_DP6
 * File:   ADCs.h
 * Author: Halanna Le, Christy Guirguis, Gillian Habermehl
 * Created on November 2, 2023, 7:22 PM
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


uint16_t ADCvalue;
uint16_t do_ADC(void);
void print_ADC(uint16_t ADCvalue);
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

#endif	/* XC_HEADER_TEMPLATE_H */

