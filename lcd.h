// Date: 4/17/24
// Name: Nathanael Smith
// Student ID number: 5806738
// Course number: EE2361
// Term: Spring 2024
// Lab/assignment number: Lab 6
// Short Program Description: LCD.H Header File

#ifndef LCD_LIB
#define LCD_LIB

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void lcd_init(void);
    void lcd_cmd(char command);
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char myChar);
    void lcd_printStr(const char s[]);
    void lcd_shiftDisplayRight(void);
    void lcd_shiftDisplayLeft(void);
    void delay_ms(unsigned int ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LCD_LIB */
