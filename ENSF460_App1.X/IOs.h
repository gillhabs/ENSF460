/*
 * Project: ENSF460_App1
 * File:   IOs.h
 * Author: Halanna Le, Christy Guirguis, Gillian Habermehl
 * Created on November 1, 2023, 6:22 PM
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

extern uint8_t CNflag;
extern uint16_t i;
extern uint16_t runningpb3;
extern uint16_t timerOn;
extern uint16_t alarmOn;
extern uint16_t seconds;
extern uint16_t minutes;
extern uint16_t button3Pressed;
extern uint32_t button3Time;
extern uint16_t clkVal;
extern uint16_t isChanged;
extern uint16_t alarmPrinted; 
extern uint16_t resetTriggered; 
extern uint16_t timerStarted;

void IOinit(void);
void printInt(int num);
void handleRest(void);
void IOcheck(void);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);


#endif	/* XC_HEADER_TEMPLATE_H */

