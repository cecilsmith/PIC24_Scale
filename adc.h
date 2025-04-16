/* 
 * File:   adc.h
 * Author: hunte
 *
 * Created on February 26, 2025, 9:52 AM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

void initScaleBuffer();
long int getScaleAvg();
void putScaleVal(int ADCvalue);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

