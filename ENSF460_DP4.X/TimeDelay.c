/*
 
File:   TimeDelay.c
Author: Halanna Le, Christy Guirguis, Gillian Habermehl
*
Created on October 11, 2023, 2:24 PM
*/


#include "xc.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include "Ios.h"
#include "UART2.h"

void delay_ms(uint16_t time_ms) {

    // Default is 8MHz
    // clkVal = clkVal / 2
    uint16_t clkVal = 4000;

    // Configure clock
    if(time_ms >= 16 && time_ms <= 262) {
        // clkVal = clkVal / 2
        clkVal = 250;
        // 500 kHz
        NewClk(500);
    }
    else if(time_ms > 262) {
        // clkVal = clkVal / 2
        clkVal =16;
        // 32 kHz
        NewClk(32);
    }

    // T2CON configuration
    T2CONbits.TSIDL = 0; // Continue module operation in Idle mode
    T2CONbits.T32 = 0; // Timer2 and Timer3 act as two 16-bit timers
    T2CONbits.TCKPS = 0b00; // Prescaler = 1
    T2CONbits.TCS = 0; // External clock (Fosc/2)

    TMR2 = 0;
    // Compute PR2
    PR2 = clkVal * time_ms;

    T2CONbits.TON = 1; // Start the 16-bit timer

    // Timer2 interrupt configuration
    IPC1bits.T2IP = 7; // Set priority level
    IEC0bits.T2IE = 1; // Enable timer interrupt
    IFS0bits.T2IF = 0; // In the even of an interrupt TxIF is set. Clear it at setup

    Idle(); // Stay here until interrupt

    if(clkVal != 4000) {
        // Switch back clock to default if needed
        NewClk(8);
    }


    return;
}

// Timer2 interrupt subroutine
// Once interrupt triggers when TMR2 = PR2
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void) {
    IFS0bits.T2IF = 0; // Clear the Timer2 interrupt flag
    TMR2 = 0;
    T2CONbits.TON = 0; // Stop timer

    return;
}