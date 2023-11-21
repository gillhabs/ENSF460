/*
 * Project: ENSF460_DP1
 * File:   main.c
 * Author: gillianhabermehl
 *
 * Created on September 22, 2023, 9:01 AM
 */


#include "xc.h"

void main(void) {
    
    TRISAbits.TRISA0 = 1; // Sets RA0 as input
    TRISAbits.TRISA1 = 1; // Sets RA1 as input 
    TRISAbits.TRISA4 = 1; // Sets RA4 as input
    TRISAbits.TRISA5 = 1; // Sets RA5 as input
    
    TRISBbits.TRISB0 = 0; // Sets RB0 as output
    TRISBbits.TRISB1 = 0; // Sets RB1 as output
    TRISBbits.TRISB2 = 0; // Sets RB2 as output
    TRISBbits.TRISB4 = 0; // Sets RB4 as output
    TRISBbits.TRISB7 = 0; // Sets RB7 as output
    TRISBbits.TRISB8 = 0; // Sets RB8 as output
    TRISBbits.TRISB9 = 0; // Sets RB9 as output
    TRISBbits.TRISB12 = 0; // Sets RB12 as output
    
    // infinite loop until microcontroller is turned off
    while(1) {
        
        // Entry is 0000
        if((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 0) 
                && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 0)) {

            // Turn 0 LED's on
            // Turn all pins to lo
            LATBbits.LATB0 = 0;
            LATBbits.LATB1 = 0;
            LATBbits.LATB2 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
        }
        
        // Entry is 0001
        else if((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 0) 
                && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 1)) {
            
            // Turn 3 LED's on (first UCID digit)
            // Turn RB0, RB1 and RB2 hi, everything else lo
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;   
        }
        
        // Entry is 0010
        else if((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 0) 
                && (PORTAbits.RA4 == 1) && (PORTAbits.RA5 == 0)) {

            // Turn 0 LED's on (second UCID digit)
            // Turn all pins to lo
            LATBbits.LATB0 = 0;
            LATBbits.LATB1 = 0;
            LATBbits.LATB2 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
        }
        
        // Entry is 0011
        else if((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 0) 
                && (PORTAbits.RA4 == 1) && (PORTAbits.RA5 == 1)) {

            // Turn 1 LED on (third UCID digit)
            // Turn RB1 hi, everything else lo
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 0;
            LATBbits.LATB2 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
        }
        
        // Entry is 0100
        else if((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 1) 
                && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 0)) {

            // Turn 4 LED's on (fourth UCID digit)
            // Turn RB0, RB1, RB2 and RB4 hi, everything else lo
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
        }
        
        // Entry is 0101
        else if((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 1) 
                && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 1)) {

            // Turn 5 LED's on (fifth UCID digit)
            // Turn RB0, RB1, RB2, RB4 and RB7 hi, everything else lo
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
        }
        
        // Entry is 0110
        else if((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 1) 
                && (PORTAbits.RA4 == 1) && (PORTAbits.RA5 == 0)) {

            // Turn 9 LED's on (sixth UCID digit)
            // Turn all output pins to hi
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            LATBbits.LATB9 = 1;
            LATBbits.LATB12 = 1;
        }
        
        // Entry is 0111
        else if((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 1) 
                && (PORTAbits.RA4 == 1) && (PORTAbits.RA5 == 1)) {

            // Turn 9 LED's on (seventh UCID digit)
            // Turn all output pins to hi
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            LATBbits.LATB9 = 1;
            LATBbits.LATB12 = 1;
        }
        
        // Entry is 1000
        else if((PORTAbits.RA0 == 1) && (PORTAbits.RA1 == 0) 
                && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 0)) {

            // Turn 4 LED's on (eighth UCID digit)
            // Turn RB0, RB1, RB2 and RB4 hi, everything else lo
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
        }
        
        // Entry is 1001 to 1111
        else {
            
            // Turn 0 LED's on (UCID is completed, no more digits to go through)
            // Turn all output pins to lo
            LATBbits.LATB0 = 0;
            LATBbits.LATB1 = 0;
            LATBbits.LATB2 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
        }         
    }
    return;
}
