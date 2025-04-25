#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <p24Fxxxx.h>
#include "lcd.h" //for delay function

#define BUFSIZE 32
#define NUMSAMPLES 32
/*
This is the main ADC conversion file which takes in the 
voltage difference for the load scale and converts
it into a usable digital value.
*/

volatile unsigned long adc_ScaleBuffer[BUFSIZE];
int buffindxScale = 0;


//init sensor buffer to buffsize
void initScaleBuffer() {
    for(int i=0; i<BUFSIZE; i++) {
        adc_ScaleBuffer[i]=0;
    }
    buffindxScale = 0;
}

//place ADC value into sensor buffer 
void putScaleVal(int ADCvalue) {
    adc_ScaleBuffer[buffindxScale++] = ADCvalue;
    if(buffindxScale>=BUFSIZE) {
        buffindxScale = 0;
    }
}

//gets average of values in buffer and returns the value 
unsigned long getScaleAvg() {
    unsigned long scale_sum = 0;
    for(int j=0; j<NUMSAMPLES; j++) {
        scale_sum += adc_ScaleBuffer[j];
    }
    return scale_sum / NUMSAMPLES;
}