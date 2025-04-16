// Date: 4/17/24
// Name: Nathanael Smith
// Student ID number: 5806738
// Course number: EE2361
// Term: Spring 2024
// Lab/assignment number: Lab 6
// Short Program Description: ADC C File

#include "xc.h"
#include "adc.h"

void adc_init(void)
{
    // TODO: Set up the reference pins to analog?

    // Initialize input for ADC
    TRISBbits.TRISB12 = 1;  // set pin 23 to input
    AD1PCFGbits.PCFG12 = 0; // setup I/O for pin 23

    AD1CON2bits.VCFG = 0b011;   // Use external reference pins
    AD1CON3bits.ADCS = 2;       // TAD >= 75ns (Tcy=62.5ns)
    AD1CON1bits.SSRC = 0b010;   // Sample on TMR3 event
    AD1CON3bits.SAMC = 0b00001; // At least one autosample time bit
    AD1CON1bits.FORM = 0;       // Data output form (unsigned int)
    AD1CON1bits.ASAM = 1;       // Auto sample again after conversion
    AD1CON2bits.SMPI = 0b0000;  // Sample/Convert Sequences Per Interrupt Selection bits
    AD1CON1bits.ADON = 1;       // turn on ADC

    _AD1IF = 0; // clear flag
    _AD1IE = 1; // enable interrupt

    TMR3 = 0;
    T3CON = 0;
    T3CONbits.TCKPS = 0b10;          // 1:64 prescaler
    PR3 = 16000000 / (64 * KEY) - 1; // sample every 1/K second (1/16 = 0.0625s)
    T3CONbits.TON = 1;
}