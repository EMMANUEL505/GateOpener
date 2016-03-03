/* 
 * File:   main.c
 * Author: e882742
 *
 * Created on February 29, 2016, 11:43 AM
 */
#include "config.h"
#include "SIM800L.h"

char c=0;


 /***** MAIN PROGRAM *****/ 
int main(void) {
    OSCCON=0b01110010;              //Configure Internal Oscillator to 8MHz
    /*Port configuration*/
    TRISC0=0;                       //Configure PINC0 as output (Red LED)
    TRISC1=0;                       //Configure PINC1 as output (Green LED)
    TRISC2=0;                       //Configure PINC1 as output (Blue LED)
    TRISC4=0;                       //Configure PINC4 as output (USART TX)
    TRISC5=1;                       //Configure PINC5 as input (USART RX)
    TRISB4=0;                       //Configure PINB4 as output (Relay)
    /*USART initialize*/
    APFCON0bits.RXDTSEL=1;          //Configure RX function on PORTC pin
    APFCON0bits.TXCKSEL=1;          //Configure TX function on PORTC pin
    USARTInit(115);                 //Configure BaudRate to 115200
    /*SIM800 initialize*/
    SIM800init();                   //Initialize SIM800L module
    /*Initialize port states*/
    LED_RED=ENABLED;
    LED_GREEN=DISABLED;
    LED_BLUE=DISABLED;
    RELAY=DISABLED;

    while(1)
    {
        //USARTWriteChar('a');
        ei();                               //Enable interrupts
        USARTWriteLine("AT+CSQ\r\n");       //Check signal Quality
        __delay_ms(2000);                   // 5 Second Delay
        while(PIR1bits.RCIF);
        di();                               //disable interrupts
        //USARTWriteLine("----DataAvailable:----");
        //USARTWriteInt(USARTDataAvailable(),2);
        //USARTWriteLine("----Buffer start:----");
        while(USARTDataAvailable()>30)
        {
           c= USARTReadData();
           //USARTWriteChar(c);
           if(c=='+' && USARTReadData()=='C' && USARTReadData()=='L' && USARTReadData()=='I')
            {
               USARTWriteLine("ATH\r\n");   //Hang up
               LED_GREEN=!LED_GREEN;
               RELAY=!RELAY;
               USARTFlushBuffer();
            }
        }
        //USARTWriteLine("----Buffer end:----");
        //USARTClearBuffer();
    }
}

