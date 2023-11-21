/*
 * Project: ENSF460_DP6
 * File:   IOs.c
 * Author: Halanna Le, Christy Guirguis, Gillian Habermehl
 * Created on November 2, 2023, 7:22 PM
 */

#include "xc.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include "IOs.h"
#include "UART2.h"
#include "ADC.h"

uint16_t old_ADCvalue = 0;
uint16_t ADCvalue;

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
    TRISAbits.TRISA2 = 1; //Sets RA2 as PB1 input
    TRISAbits.TRISA4 = 1; //Sets RA4 as PB2 input
    TRISBbits.TRISB4 = 1; //Sets RB4 as PB3 input
    TRISBbits.TRISB8 = 0; //Sets RB8-GPIO12 as output
    
    // Configure CN on inputs
    CNEN2bits.CN30IE = 1; // Enable RA2 CN
    CNEN1bits.CN1IE = 1; // Enable RB4 CN
    CNEN1bits.CN0IE = 1; // Enable RA4 CN
    
    // Configure IO CN interrupt 
    IPC4bits.CNIP = 6; // Set CN priority to 6
    IFS1bits.CNIF = 0; // Enable CN interrupt
    IEC1bits.CNIE = 1; // Clear CN flag
    
     
    return;
}


void IOcheck(void) {
    
    // Compute the ADC value
    ADCvalue = do_ADC();
    
    NewClk(32);
    XmitUART2('G', 1);
    Disp2Hex(ADCvalue);
    XmitUART2('\n', 1);
    NewClk(8);
    return;
}
