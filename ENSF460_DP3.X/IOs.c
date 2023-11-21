/*
 * File:   IOs.c
 * Author: Halanna Le, Christy Guirguis, Gillian Habermehl
 *
 * Created on October 11, 2023, 2:22 PM
 */


#include "xc.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include "IOs.h"

void IOinit(void) {
    //Clock output on REFO pin - pin 18 RB15
    TRISBbits.TRISB15 = 0;  // Set RB15 as output for REFO
    REFOCONbits.ROEN = 1; // Ref oscillator is enabled
    REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep
    REFOCONbits.ROSEL = 0; // Output base clk showing clock switching
    REFOCONbits.RODIV = 0b0000;
     
    //IO Initializations
    AD1PCFG = 0xFFFF;  // Turn all analog pins to digital
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RB4/CN1
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA2/CN30
    
    // Set input and outputs
    TRISAbits.TRISA2 = 1; //Sets RA2 as PB1
    TRISAbits.TRISA4 = 1; //Sets RA4 as PB2
    TRISBbits.TRISB4 = 1; //Sets RB4 as PB3
    TRISBbits.TRISB8 = 0; //Sets RB8-GPIO12 as output
     
    return;
}

void IOcheck(void) {
    
    // RA2: PB1
    // RA4: PB2
    // RB4: PB3

    // PB1 is pressed
    if (PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) {
        // Delay 1s and switch LED output
        delay_ms(1000);           
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
    }
           
    // PB2 is pressed
    else if ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1)) {
        // Delay 2s and switch LED output
        delay_ms(2000);
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
    }
        
    // PB3 is pressed
    else if ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0)) {
        // Delay 3s and switch LED output
        delay_ms(3000);
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
    }
        
    // PB1 and PB3 are pressed
    else if ((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0)) {
        //Turn LED on
        LATBbits.LATB8 = 1;
    }
        
    // PB2 and PB3 are pressed
    else if ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0)) {
        // Turn LED on
        LATBbits.LATB8 = 1;
    }
        
    // PB1 and PB2 are pressed
    else if ((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1)) {
        // Delay 1ms and switch LED output
        delay_ms(1);
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
    }
        
    // PB1, PB2 and PB3 are pressed
    else if ((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0)) {
        // Turn LED on
        LATBbits.LATB8 = 1;
    }
        
    // No push buttons are being pressed
    else if ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1)) {
        // Turn LED off
        LATBbits.LATB8 = 0;
    }   
    
    return;
}
