// Date: 4/17/24
// Name: Nathanael Smith
// Student ID number: 5806738
// Course number: EE2361
// Term: Spring 2024
// Lab/assignment number: Lab 6
// Short Program Description: Buffer C File

#include "buffer.h"

#define BUFSIZE 1024
#define NUMSAMPLES 128

unsigned int adc_buffer[BUFSIZE];
int buffer_index = 0;

/* Reset all buffer entries */
void initBuffer(void)
{
    int i;
    for (i = 0; i < BUFSIZE; i++)
    {
        adc_buffer[i] = 0;
    }
}

/* Add a new value to the buffer */
void putVal(int ADCvalue)
{
    adc_buffer[buffer_index++] = ADCvalue;
    if (buffer_index >= BUFSIZE)
    {
        buffer_index = 0;
    }
}

/* Calculate the last NUMSAMPLES average of the buffer */
unsigned int getBufferAverage(void)
{
    // This function is responsible for obtaining the average of the buffer.
    int i = 0;
    unsigned long sum = 0;
    // Add all values in the buffer
    for (i = 0; i < NUMSAMPLES; i++)
    {
        sum += adc_buffer[(buffer_index - i) % BUFSIZE];
    }
    return sum / NUMSAMPLES;
}