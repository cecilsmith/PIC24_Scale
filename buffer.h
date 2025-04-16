// Date: 4/17/24
// Name: Nathanael Smith
// Student ID number: 5806738
// Course number: EE2361
// Term: Spring 2024
// Lab/assignment number: Lab 6
// Short Program Description: Buffer Header File

#ifndef BUFFER_LIB
#define BUFFER_LIB

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void initBuffer(void);
    void putVal(int ADCvalue);
    unsigned int getBufferAverage(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BUFFER_LIB */
