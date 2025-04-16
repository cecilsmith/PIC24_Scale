// Date: 4/17/24
// Name: Nathanael Smith
// Student ID number: 5806738
// Course number: EE2361
// Term: Spring 2024
// Lab/assignment number: Lab 6
// Short Program Description: LCD.c File

#include "xc.h"
#include "lcd.h"

#define CONTRAST 0b01110000 // last 4 bits controlled

void delay_ms(unsigned int ms) // delay in milliseconds
{
    while (ms-- > 0)
    {
        asm("repeat #15998");
        asm("nop");
    }
}

void lcd_init(void)
{
    // Initialize I2C2
    I2C2BRG = 0x9D; // 100 kHz clock frequency
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1; // enable I2C2

    // Initialize LCD
    delay_ms(40);

    lcd_cmd(0b00111000); // function set
    lcd_cmd(0b00111001); // function set
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(CONTRAST);   // contrast low
    lcd_cmd(0b01010110);
    lcd_cmd(0b01101100); // follower control

    delay_ms(20);

    lcd_cmd(0b00111000); // function set
    lcd_cmd(0b00001100); // display on
    lcd_cmd(0b00000001); // clear display

    delay_ms(1);
}

void lcd_cmd(char command)
{
    // Initialize start condition
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.SEN = 1;

    // Wait for start condition to clear
    while (I2C2CONbits.SEN == 1)
        ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; // slave address and R/W bit

    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000; // 8 bits containing control byte

    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = command; // 8 bits of data

    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1; // send stop sequence

    while (I2C2CONbits.PEN == 1)
        ; // wait for PEN to clear
}

void lcd_setCursor(char x, char y)
{
    // If not the first row, write to second.
    if (y == 0)
    {
        lcd_cmd(0b10000000 + x);
    }
    else
    {
        lcd_cmd(0b10000000 + x + 0x40); // 0x40 added for second row
    }
}

void lcd_printChar(char myChar)
{
    // Initialize start condition
    I2C2CONbits.SEN = 1;

    // Wait for start condition to clear
    while (I2C2CONbits.SEN == 1)
        ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; // slave address and R/W bit

    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01000000; // 8 bits containing control byte

    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = myChar; // 8 bits of data

    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1; // send stop sequence

    while (I2C2CONbits.PEN == 1)
        ; // wait for PEN to clear
}

void lcd_printStr(const char s[])
{
    unsigned int i;
    // Due to how the LCD automatically updates the cursor,
    // we can just feed in characters to write
    for (i = 0; s[i] != '\0'; i++)
    {
        lcd_printChar(s[i]);
    }
}

void lcd_shiftDisplayRight(void)
{
    lcd_cmd(0b000011100); // Display shift right command
}

void lcd_shiftDisplayLeft(void)
{
    lcd_cmd(0b000011000); // Display shift left command
}