#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h" //for delay function

#define BUFSIZE 128

/*
This is the main ADC conversion file which takes in the 
voltage differecne for the load scale and converts
it into a usable digital value.
*/

int adc_ScaleBuffer[BUFSIZE];
int buffindxScale = 0;
long int getScale;

//init sensor buffer to buffsize
void initScaleBuffer() {
    for(int i=0; i<BUFSIZE; i++) {
        adc_ScaleBuffer[i]=0;
    }
}

//place ADC value into sensor buffer 
void putScaleVal(int ADCvalue) {
    adc_ScaleBuffer[buffindxScale++]=ADCvalue;
    if(buffindxScale>=BUFSIZE) {
        buffindxScale = 0;
    }
}

//gets average of values in buffer and returns the value 
long int getScaleAvg() {
    scale_avg=0;
    for(int j=0; j<BUFSIZE; j++) {
        scale_avg += adc_ScaleBuffer[j];
    }
    scale_avg /= BUFSIZE;
    return scale_avg;
}