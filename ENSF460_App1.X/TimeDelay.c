/*
 * Project: ENSF460_App1
 * File:   TimeDelay.c
 * Author: Halanna Le, Christy Guirguis, Gillian Habermehl
 * Created on November 1, 2023, 6:24 PM
 */



#include "xc.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include "Ios.h"
#include "UART2.h"

void delay_ms(uint16_t time_ms) {

    // Default is 8MHz
    // clkVal = clkVal / 2
    clkVal = 4000;

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


    // T1CON configuration
    T1CONbits.TSIDL = 0; // Continue module operation in Idle mode
    //T1CONbits.T32 = 0; // Timer2 and Timer3 act as two 16-bit timers
    T1CONbits.TCKPS = 0b00; // Prescaler = 1
    T1CONbits.TCS = 0; // External clock (Fosc/2)

    TMR1 = 0;
    
    // Compute PR1
    PR1 = time_ms * clkVal;

    T1CONbits.TON = 1; // Turn on the 16-bit timer

    // Timer1 interrupt configuration
    IPC1bits.T2IP1 = 7; // Set priority level
    IEC0bits.T1IE = 1; // Enable timer interrupt
    IFS0bits.T1IF = 0; // In the even of an interrupt TxIF is set. Clear it at setup
    Idle(); // Stay here until interrupt

    if(clkVal != 4000) {
        // Switch back clock to default if needed
        NewClk(8);
    }


    return;
}

// Timer2 interrupt subroutine
// Once interrupt triggers when TMR2 = PR2
void __attribute__((interrupt, no_auto_psv))_T1Interrupt(void) {
    IFS0bits.T1IF = 0; // Clear the Timer2 interrupt flag
    TMR1 = 0;
    T1CONbits.TON = 0; // Stop timer

    return;
}