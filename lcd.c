#include <xc.h>
#include <stdio.h>    // for sprintf
#include <stdlib.h>

#define BUFSIZE 128
#define NUMSAMPLES 256


void delay_ms(unsigned int ms) {
    while(ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}


void lcd_cmd(char Package) {
    IFS3bits.MI2C2IF=0;
    I2C2CONbits.SEN=1; //initialize start command
    while(I2C2CONbits.SEN==1); //wait for start bit to complete
    
    IFS3bits.MI2C2IF=0; //clear interrupt flag
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF==0); //wait for flag to be tripped
    
    IFS3bits.MI2C2IF=0; //clear interrupt flag
    I2C2TRN = 0b00000000; // 8-bits consisting of control byte
    while(IFS3bits.MI2C2IF==0); //wait for flag to be tripped

    IFS3bits.MI2C2IF=0; //clear interrupt flag
    I2C2TRN = Package; // 8-bits consisting of the data byte
    while(IFS3bits.MI2C2IF==0); //wait for flag to be tripped
    
    IFS3bits.MI2C2IF=0; //clear flag
    I2C2CONbits.PEN=1; //start sending stop bit
    while(I2C2CONbits.PEN==1); //wait for stop bit to be sent
}    


void lcd_init() {
    _I2CEN=0; //disable IC2 mode
    I2C2BRG=0x9D; //clock frequency of 100 kHz
    IFS3bits.MI2C2IF=0; //clear interrupt flag
    I2C2CONbits.I2CEN=1; //enable IC2 mode
    
    delay_ms(40);
    
    lcd_cmd(0b00111000); //function set, normal instrc mode 
    lcd_cmd(0b00111001); //function set, extended instruction mode
    lcd_cmd(0b00010100); //interval osc
    lcd_cmd(0b01110000); //contrast low
    lcd_cmd(0b01010110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); //follower control
    
    delay_ms(200);
    
    lcd_cmd(0b00111000); //function set, normal instrc mode
    lcd_cmd(0b00001100); //display on
    lcd_cmd(0b00000001); //clear display
    
    delay_ms(1);
}


void lcd_setCursor(char x, char y) {
    char rowColumn=(0x40*y)+x; //y==row number, x==column number
    rowColumn |= 0b10000000; //mask first bit with 1
    lcd_cmd(rowColumn); //pass cursor address to lcd
}

void lcd_printStr(const char s[]) {
    int i=0; //set index to 0
    I2C2CONbits.SEN=1; //initialize start command
    while(I2C2CONbits.SEN==1); //wait for start command to finish sending
    
    IFS3bits.MI2C2IF=0; //clear flag
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0); //wait for flag to be enabled
     
    //while the next char isn't end of string
    while(s[i+1] != '\0') {
        IFS3bits.MI2C2IF=0; //clear flag
        I2C2TRN = 0b11000000; // tell the lcd another character will be passed right after
        while(IFS3bits.MI2C2IF == 0); //wait for flag to be enabled
        
        IFS3bits.MI2C2IF=0; //clear flag
        I2C2TRN = s[i]; // 8-bits consisting of the data byte
        while(IFS3bits.MI2C2IF == 0); //wait for flag to be enabled
        
        i++; //increment index
    }
    
    IFS3bits.MI2C2IF=0; //clear flag
    I2C2TRN = 0b01000000; // tell the lcd this is the final character that will be passed
    while(IFS3bits.MI2C2IF == 0); //wait for flag to be enabled
        
    IFS3bits.MI2C2IF=0; //clear flag
    I2C2TRN = s[i]; // 8-bits consisting of the data byte
    while(IFS3bits.MI2C2IF == 0); //wait for flag to be enabled
    
    IFS3bits.MI2C2IF=0; //clear flag
    I2C2CONbits.PEN=1; //start sending stop bit
    while(I2C2CONbits.PEN==1); //wait for stop bit to finish sending
}

void lcd_printChar(char myChar) {
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.SEN = 1;
        while(I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN= 0b01111100;
        while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01000000;           //RS = 1
        while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = myChar;
        while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
        while(I2C2CONbits.PEN);
}