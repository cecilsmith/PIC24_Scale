/* 
 * File:   lcd.h
 * Author: hunte
 *
 * Created on February 26, 2025, 9:51 AM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

void delay_ms(unsigned int ms);
void lcd_cmd(char Package);
void lcd_init();
void lcd_setCursor(char x, char y);
void lcd_printStr(const char s[]);
void lcd_printChar(char myChar);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

