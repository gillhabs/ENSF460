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
#include "UART2.h"


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
    
    // Idle until CN interrupt triggered (button pressed)
    Idle();

    return;
}

// Configure CN interrupt
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {

    IFS1bits.CNIF = 0;
    
    // To filter out debouncing
    delay_ms(60);
    
    // RA2: PB1
    // RA4: PB2
    // RB4: PB3
    
    char* buttons;
    uint8_t isChanged; // If we have changed the clock to print to the serial
    uint16_t clkValue = OSCCONH; // Get current clock value
    
    // Set the string value of what buttons are pressed, print only on an interrupt
    // Use nested if statements to speed up this function, uses less else if's
    if(PORTAbits.RA2 == 1) {
        if(PORTAbits.RA4 == 0) {
            if(PORTBbits.RB4 == 0) {
                // RA2 is 1
                // RA4 is 0
                // RB4 is 0
                buttons = "PB2 and PB3 are pressed";
            }
            else {
                // RA2 is 1
                // RA4 is 0
                // RB4 is 1
                buttons = "PB2 is pressed";  
            }
        }
        else {
            if(PORTBbits.RB4 == 0) {
                // RA2 is 1
                // RA4 is 1
                // RB4 is 0
                buttons = "PB3 is pressed";
            }
            else {
                // RA2 is 1
                // RA4 is 1
                // RB4 is 1
                buttons = "Nothing pressed";
            }
        }
    }
    
    // RA2 has to be 0
    else if(PORTAbits.RA4 == 1) {
        if(PORTBbits.RB4 == 0) {
            // RA2 is 0
            // RA4 is 1
            // RB4 is 0
            buttons = "PB1 and PB3 are pressed";
        }
        else {
            // RA2 is 0
            // RA4 is 1
            // RB4 is 1
            buttons = "PB1 is pressed";
        }
    }
    
    // RA2 and RA4 are both 0
    else if(PORTBbits.RB4 == 0){
        // RA2 is 0
        // RA4 is 0
        // RB4 is 0 
        buttons = "All buttons pressed";
        }
    else {
        // RA2 is 0
        // RA4 is 0
        // RB4 is 1
        buttons = "PB1 and PB2 are pressed";
    }
    
    // If clock isn't currently 32kHz
    if(OSCCONH != 0x55) {
        isChanged = 1; // We are changing the clock value
        NewClk(32);
    }
    // Print in 32kHz at 300 baud rate
    XmitUART2('\n', 1);
    XmitUART2('\r', 1);
    Disp2String(buttons);
    // If we have changed the clock value to print to serial
    if(isChanged == 1) {
        // Change back to old clock
        NewClk(8);
    }
    
    
    // PB1 is pressed
    while (PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) {
        // Delay 0.5s and switch LED output
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
        delay_ms(500);  
    }
           
    // PB2 is pressed
    while ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1)) {
        // Delay 2s and switch LED output
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
        delay_ms(2000);
    }
        
    // PB3 is pressed
    while ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0)) {
        // Delay 3s and switch LED output
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
        delay_ms(3000);
    }
        
    // PB1 and PB3 are pressed
    while ((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0)) {
        //Turn LED on
        LATBbits.LATB8 = 1;
    }
        
    // PB2 and PB3 are pressed
    while ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0)) {
        // Turn LED on
        LATBbits.LATB8 = 1;   
    }
        
    // PB1 and PB2 are pressed
    while ((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1)) {
        // Turn LED on
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; 
    }
        
    // PB1, PB2 and PB3 are pressed
    while ((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0)) {
        // Turn LED on
        LATBbits.LATB8 = 1;
    }
        
    // No push buttons are being pressed
    while ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1)) {
        // Turn LED off
        LATBbits.LATB8 = 0;
    }   
    
    return;
}