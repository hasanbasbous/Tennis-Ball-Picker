/*
 * File:   mainFP.c
 * Author: HasanBasbous
 *
 * Created on December 11, 2021, 11:04 AM
 */


//
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 45         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

//#pragma config FOSC = HS, FCMEN = ON, WDT = OFF, IESO =ON, XINST = OFF, LVP = OFF

#include <xc.h>
//#include <pic18f4550.h>

#define rs PORTAbits.RA0
#define rw PORTAbits.RA1
#define en PORTAbits.RA2

#define lcdport LATD

void lcd_ini();
void lcdcmd(unsigned char);
void lcddata(unsigned char);
void Delay_ms(unsigned int);

void main(void) {
    ADCON0=0; //Disable A/D converter module
    ADCON1bits.PCFG=0xF; /* This makes all pins as digital I/O */ 
    
    TRISEbits.RE0 = 1; //input GPIO pin 8
    TRISEbits.RE1 = 1; //input GPIO pin 10
    TRISEbits.RE2 = 1;
        
    
    TRISAbits.RA0=0;		// Configure Port A as output port
    TRISAbits.RA1=0;
    TRISAbits.RA2=0;

    
	LATAbits.LA0=0;
    LATAbits.LA1=0;
    LATAbits.LA2=0;
    
    TRISD = 0;  
    LATD = 0;   // PORTD as output, 8 data bits to the LCD
	    
    lcd_ini();		// LCD initialization
    
     
    while(1){
        if(PORTEbits.RE2 == 0 && PORTEbits.RE1 == 0 && PORTEbits.RE0 == 0){
            lcdcmd(0x01);
            lcddata('L');
            lcddata('o');
            lcddata('c');
            lcddata('a');
            lcddata('t');
            lcddata('e');
            lcddata(' ');
            lcddata('B');
            lcddata('a');
            lcddata('l');
            lcddata('l');      
        } else if (PORTEbits.RE2 == 0 && PORTEbits.RE1 == 0 && PORTEbits.RE0 == 1){
            lcdcmd(0x01);
            lcddata('C');
            lcddata('o');
            lcddata('l');
            lcddata('l');
            lcddata('e');
            lcddata('c');
            lcddata('t');
            lcddata(' ');
            lcddata('B');
            lcddata('a');
            lcddata('l');
            lcddata('l');
        } else if(PORTEbits.RE2 == 0 && PORTEbits.RE1 == 1 && PORTEbits.RE0 == 0){
            lcdcmd(0x01);
            lcddata('L');
            lcddata('o');
            lcddata('c');
            lcddata('a');
            lcddata('t');
            lcddata('e');
            lcddata(' ');
            lcddata('B');
            lcddata('o'); 
            lcddata('x'); 
        } else if(PORTEbits.RE2 == 1 && PORTEbits.RE1 == 1 && PORTEbits.RE0 == 1){
            lcdcmd(0x01);
            lcddata('L');
            lcddata('o');
            lcddata('c');
            lcddata('a');
            lcddata('t');
            lcddata('e');
            lcddata(' ');
            lcddata('W'); 
            lcddata('h'); 
            lcddata('i'); 
            lcddata('t'); 
            lcddata('e'); 
            lcdcmd(0xC0);
            lcddata('A');
            lcddata('r');
            lcddata('e');
            lcddata('a');
        } else if(PORTEbits.RE2 == 0 && PORTEbits.RE1 == 1 && PORTEbits.RE0 == 1){
            lcdcmd(0x01);
            lcddata('D');
            lcddata('e');
            lcddata('p');
            lcddata('o');
            lcddata('s');
            lcddata('i');
            lcddata('t');
            lcddata(' ');
            lcddata('B');
            lcddata('a'); 
            lcddata('l'); 
            lcddata('l'); 
        } else if(PORTEbits.RE2 == 1 && PORTEbits.RE1 == 0 && PORTEbits.RE0 == 1){
            lcdcmd(0x01);
            lcddata('6');
        } else if(PORTEbits.RE2 == 1 && PORTEbits.RE1 == 1 && PORTEbits.RE0 == 0){
            lcdcmd(0x01);
            lcddata('7');
        } else if(PORTEbits.RE2 == 1 && PORTEbits.RE1 == 1 && PORTEbits.RE0 == 1){
            lcdcmd(0x01);
            lcddata('8');
        }
        
        Delay_ms(1000);
    }
    
}


void lcd_ini()
{
	lcdcmd(0x38);		// Configure the LCD in 8-bit mode, 2 line and 5x7 font
	lcdcmd(0x0C);		// Display On and Cursor Off
	lcdcmd(0x01);		// Clear display screen
	lcdcmd(0x06);		// Increment cursor
	lcdcmd(0x80);		// Set cursor position to 1st line, 1st column
}

void lcdcmd(unsigned char cmdout)
{
	lcdport=cmdout;		//Send command to lcdport=PORTB
	rs=0;						
	rw=0;
	en=1;
	Delay_ms(10);
	en=0;
}

void lcddata(unsigned char dataout)
{
	lcdport=dataout;	//Send data to lcdport=PORTB
	rs=1;
	rw=0;
	en=1;
	Delay_ms(10);
	en=0;
}

void Delay_ms(unsigned int tdelay){
    unsigned int i; 
    unsigned char j;
    for(i=0; i<2*tdelay;i++)
         for(j=0;j<176;j++);
    
       
}


