#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
/*
This file is used to take in raw ADC input values
that are averaged and convert them into a float which 
represents the current weight upon the digital scale.
This data can be further used in the TARE function if needed.
Finaly the computed weight will be transmitted live to the LCD
over the I2C protocol.
*/

long int avgScale = 0;

void readScale(void) {
    // Get AVG value from ADC Buffers
    avgScale = getScaleAvg();
}

int convWeight() {
    //Call read function
    readScale();

    int currWeight = 0;

    //Convert raw ADC value to grams using conversion math
    //currWeight = mavgScale

    return currWeight;
}