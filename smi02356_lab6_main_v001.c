// Date: 4/17/24
// Name: Nathanael Smith
// Student ID number: 5806738
// Course number: EE2361
// Term: Spring 2024
// Lab/assignment number: Lab 6
// Short Program Description: Main C File

#include "xc.h"
#include "lcd.h"
#include "buffer.h"
#include "adc.h"
#include <p24Fxxxx.h>
#include <string.h> // for strlen()
#include <stdio.h>  // for sprintf()

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1   // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF   // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF    // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)

// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI  // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF  // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON  // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME // Clock Switching and Monitor (Clock switching is enabled,
// Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

// Set up ADC write to buffer
void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void)
{
    putVal(ADC1BUFC);
    _AD1IF = 0;
}

// Set up TMR1 interrupt
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    _T1IF = 0;
    lcd_cmd(0b00000001); // clear LCD
    lcd_setCursor(0, 0); // set cursor to home

    unsigned int adValue;
    char adStr[20];
    adValue = getBufferAverage();
    sprintf(adStr, "%6.4f V", (3.3 / 1024) * adValue); // ?x.xxxx V?
    // 6.4 in the format string ?%6.4f? means 6 placeholders for the whole
    // floating-point number, 4 of which are for the fractional part.
    lcd_printStr(adStr);
}

void pic24_init(void)
{
    _RCDIV = 0;       // set frequency to 16 MHz
    AD1PCFG = 0xEFFF; // set all pins to digital except 23
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
}

void timer1_init(void)
{
    // 100 ms timer
    TMR1 = 0;
    T1CON = 0;
    T1CONbits.TCKPS = 0b10; // 1:64 prescaler
    PR1 = 24999;            // sample every 100ms
    T1CONbits.TON = 1;
    _T1IE = 1;
    _T1IF = 0;
}

int main(void)
{
    pic24_init();
    lcd_init();
    adc_init();
    timer1_init(); // initialize a timer for 100 ms
    initBuffer();

    while (1)
    {
    }
}