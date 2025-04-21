#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <p24Fxxxx.h>
#include "conversion.h"
#include "lcd.h"
#include "adc.h"
#define NUMSAMPLES 128

// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)
// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

/*
 This file acts as the main file for this project. It initializes all necessary peripherals like the 
 pic24 micro itself, the ADC portion of the pic, as well as the LCD screen. 
 After its init funcions, it calls each sensor buffer init function as well and runs a while loop
 which continuously updates the hand character which is displayed on the display. 
*/

void pic24_init() {
    _RCDIV = 0; //set internal clock to 16MHz
    AD1PCFG = 0xFFFE; //set all pins digital
}

void adc_init() {
    AD1CON2bits.VCFG = 0b011;
    AD1CON3bits.ADCS = 0b00000001;
    AD1CON1bits.SSRC = 0b010;
    AD1CON3bits.SAMC = 0b0001;
    AD1CON1bits.FORM = 0b00;
    
    TRISBbits.TRISB12 = 1; // make pin 23 analog
    TRISAbits.TRISA0 = 1; // make pin 2 analog
    TRISAbits.TRISA1 = 1; // make pin 3 analog
    
    AD1PCFGbits.PCFG12 = 0; // make pin 23 Analog
    AD1PCFGbits.PCFG0 = 0; // make pin 2 Analog
    AD1PCFGbits.PCFG1 = 0; // make pin 3 Analog
    
    
    AD1CON1bits.ASAM = 0b01;
    AD1CON2bits.SMPI = 0b0000;
    AD1CON1bits.ADON = 1;
    
    AD1CHS = 0;
    AD1CHSbits.CH0SA = 12; //SAVED US!!!
    
    _AD1IF = 0;
    _AD1IE = 1;
    
    TMR3 = 0;
    T3CON = 0;
    T3CONbits.TCKPS = 0b10;
    PR3 = 250000/NUMSAMPLES;
    T3CONbits.TON = 1;
    
    //Interrupt Stuff
//    IEC0bits.AD1IE = 1;
//    IFS0bits.AD1IF = 0;
//
//    IEC0bits.T1IE = 1;
//    IFS0bits.T1IF = 0;
}

// Interrupt Function
void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void) {
    _AD1IF = 0; //clear interrupt flags
    
    //store buffer register value
    int adScaleValue = ADC1BUF0;

    //place the stored value into sensor buffer 
    putScaleVal(adScaleValue);
}

int main(void) {
    pic24_init();
    lcd_init();
    adc_init();
    initScaleBuffer();
    
    char adStrScale[20];

    while (1) {
    	long int avgScale = getScaleAvg();
        lcd_setCursor(0,0);
        sprintf(adStrScale, "%6.4ld",avgScale);
        lcd_printStr(adStrScale);

        delay_ms(10);
    }
    return 0;
}