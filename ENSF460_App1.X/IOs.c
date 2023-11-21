/*
 * Project: ENSF460_App1
 * File:   IOs.c
 * Author: Halanna Le, Christy Guirguis, Gillian Habermehl
 * Created on November 1, 2023, 6:22 PM
 */

#include <stdio.h>
#include "xc.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include "IOs.h"
#include "UART2.h"

// Extern declarations for the global variables
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
    
    // T2CON configuration
    T2CONbits.TSIDL = 0; // Continue module operation in Idle mode
    T2CONbits.T32 = 1; // Timer2 and Timer3 act as one 32-bit timer
    T2CONbits.TCKPS = 0b00; // Prescaler = 1
    T2CONbits.TCS = 0; // External clock (Fosc/2)

    // Set TMR2 and TMR3 to 0
    TMR2 = 0;
    TMR3 = 0;
    
    // Compute PR2 and PR3 to max value
    PR2 = 0xFFFF;
    PR3 = 0xFFFF; 

    T2CONbits.TON = 0; // Stop the 32-bit timer

    // Timer2 interrupt configuration
    IPC1bits.T2IP = 7; // Set priority level
    IEC0bits.T2IE = 1; // Enable timer interrupt
    IFS0bits.T2IF = 0; // In the even of an interrupt TxIF is set. Clear it at setup
     
    return;
}

// Print current time to serial through UART
void printTime(uint16_t minutes, uint16_t seconds) {
    // If clock isn't currently 32kHz
    if(OSCCONH != 0x55) {
        isChanged = 1; // We are changing the clock value
        NewClk(32); // Make the clock value 32 kHz
    }
        
    // Print in 32kHz at 300 baud rate
    XmitUART2('\n', 1);  // Send newline character
    XmitUART2('\r', 1);  // Send carriage return character
    
    // Print minutes with leading zero if minutes under 10
    if (minutes < 10) {
        XmitUART2('0', 1);
    }
    printInt(minutes);
    
    Disp2String("m : ");
    
    // Print seconds with leading zero if seconds under 10
    if (seconds < 10) {
        XmitUART2('0', 1);
    }
    printInt(seconds);
    
    Disp2String("s");   

    // If the timer is on, and we hit 0, trigger the alarm
    if (timerOn == 1) {
        if(seconds == 0 && minutes == 0 && timerOn == 1) {
            Disp2String(" -- ALARM");  
            
            // Alarm is on and printed, turn LED on 
            alarmOn = 1;
            alarmPrinted = 1;
            LATBbits.LATB8 = 1; 
            
            // Sleep (low power mode) in alarm until another button is pressed
            Sleep();
        }
    }
    
    // If we have changed the clock value to print to serial
    if(isChanged == 1) {
        // Change back to old clock
        NewClk(8);
    }
}

// Function to convert an integer to a string and send it over UART2 to serial
void printInt(int num) {
    
    // If number is 0, print it
    if (num == 0) {
        XmitUART2('00', 1); // If the number is 0, send '0'
        return;
    }
   
    // Can only be 0-59 (2 digits and a newline)
    char str[3];
    int i = 0;

    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    i--; 

    // Print the digit over UART to the serial one char at a time
    while (i >= 0) {
        XmitUART2(str[i], 1); 
        i--;
    }
}

void IOcheck(void) {
    // RA2: PB1
    // RA4: PB2
    // RB4: PB3
   
    // Low power mode until a button is pressed
    Sleep();

    return;
}

// Configure CN interrupt
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;
    
    // Delay to filter out debouncing
    delay_ms(60);
    
    // PB1 is pressed
    while (PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) {
        // Turn the timer, alarm LED, and print off
        timerOn = 0;
        alarmOn = 0;
        LATBbits.LATB8 = 0; 
        alarmPrinted = 0;
        
        // Set only minutes (0-59)
        if (minutes == 59) {
            minutes = 0;
        }
        else {
            minutes ++;
        }
        
        // Print the time
        printTime(minutes, seconds);
        
        // Delay 1s (factoring in printing and debouncing delays) 
        delay_ms(500);  
    }
           
    // PB2 is pressed
    while ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1)) {
        // Turn the timer, alarm LED, and print off
        timerOn = 0;
        alarmOn = 0;
        LATBbits.LATB8 = 0; 
        alarmPrinted = 0;
        
        // Set only seconds (0-59)
        if (seconds == 59) {
            seconds = 0;
        }
        else {
            seconds ++;
        }
        
        // Print the time
        printTime(minutes, seconds);
        
        // Delay 1s (factoring in printing and debouncing delays)
        delay_ms(500);  
    }
        
    // PB3 is pressed
    while ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0)) {
        // Only start timer if it is not already running
        if (!button3Pressed) {    
            // Since time is 3s (3000ms), we need a new Clkval of 32kHz 
            clkVal = 16;
            NewClk(32);
            button3Pressed = 1; // PB3 has been pressed
            
            // Set timer to 0 and start it
            TMR2 = 0;
            T2CONbits.TON = 1;  
        }
    }
    
    // PB3 is released (PB3 has been previously pressed and now nothing is pressed)
    while (button3Pressed && PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) {
        T2CONbits.TON = 0;  // Stop Timer2
        button3Pressed = 0; // PB3 is not pressed anymore
        button3Time = ((uint32_t)TMR3 << 16) | TMR2; // Combine TMR2 and TMR3 for 32-bit value
        
        // If button was held less than 3 seconds
        if (button3Time <= 64000) { 
            timerOn = !timerOn; // Switch timer
            
            // If the timer is turned off
            if(timerOn == 0) {
                // Turn LED off, alarm has not been printed
                LATBbits.LATB8 = 0; 
                alarmPrinted = 0;
                Sleep(); // Low power mode
            }
            alarmOn = 0; // Turn alarm off
        } 
        // If button was held more than 3 seconds 
        else { 
            // Reset all values to 0 and print the reset time
            timerOn = 0;
            alarmOn = 0;
            seconds = 0;
            minutes = 0;
            alarmPrinted = 0;
            printTime(minutes, seconds);
        }
        TMR2 = 0; // Reset timer 2 value
        NewClk(8); // Set the clock back to 8MHz
    }
        
    // There are 2 or more buttons pressed
    while ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) <= 1) {
        LATBbits.LATB8 = 0; // Turn LED off
        
        // Timer, alarm, and alarm print is set to 0
        timerOn = 0; 
        alarmOn = 0;
        alarmPrinted = 0;
        
        // Low power mode
        Sleep();
    }
      
    // No push buttons are being pressed
    while ((PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1)) {
        // If our timer is counting down and the alarm hasn't printed yet
        if(timerOn == 1 && alarmPrinted == 0) {
            
            // Count down one seconds (switch minutes if neccessary)
            if(seconds == 0) {
                if(minutes != 0) {
                    minutes -= 1;
                    seconds = 59;
                }
            }
            else {
                seconds -= 1;
            }
            
            // if the alarm is on
            if(alarmOn == 1) {
                // Turn LED on
                LATBbits.LATB8 = 1;
            }
            else {
                // Switch LED state
                LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
            }
            
            // Print the time
            printTime(minutes, seconds);
            
            // Delay 1s (factoring in printing and debouncing delays)
            delay_ms(500);
        }
        
        else {
            // Low power mode
            Sleep();
        }
    }   
    
    return;
}
