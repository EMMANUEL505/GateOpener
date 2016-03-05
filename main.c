/* 
 * File:   main.c
 * Author: e882742
 *
 * Created on February 29, 2016, 11:43 AM
 */
#include "config.h"
#include "SIM800L.h"

char c=0;
uint8_t i=0,i2=0;

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

    eeprom_write(0,0x41);
    eeprom_write(1,0x65);
    eeprom_write(2,0x48);
    eeprom_write(3,0x18);

    eeprom_write(16,0x42);
    eeprom_write(17,0x42);
    eeprom_write(18,0x29);
    eeprom_write(19,0x23);

    EEPROMSaveNumber("6141858600",10,10);
    /*USART initialize*/
    APFCON0bits.RXDTSEL=TRUE;          //Configure RX function on PORTC pin
    APFCON0bits.TXCKSEL=TRUE;          //Configure TX function on PORTC pin
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
        RCSTAbits.CREN=ENABLED;   //Enable receive

        USARTWriteLine("AT+CSQ\r\n");       //Check signal Quality
        __delay_ms(2000);                   // 5 Second Delay

        while(SIM800L.busy==TRUE);
        di();                               //disable interrupts
        RCSTAbits.CREN=DISABLED;   //Disable receive

        switch(task)
        {
            case WAITING:
                USARTClearSIM800L();
            break;
            case CALL_IN:
                USARTWriteLine("ATH\r\n");   //Hang up
                //if(SIM800L.cell[3]==eeprom_read(0) && SIM800L.cell[4]==eeprom_read(1) && SIM800L.cell[5]==eeprom_read(2)){LED_GREEN=!LED_GREEN; RELAY=!RELAY;}
                if(EEPROMSearchNumber(SIM800L.cell,SIM800L.cell_lenght)){LED_GREEN=!LED_GREEN; RELAY=!RELAY;}
                else{ for(i=0;i<10;i++){LED_RED=!LED_RED; __delay_ms(200);}}
                for(i2=0;i2<128;i2++) SIM800L.buffer[i]=0;
                task=WAITING;
            break;
            case SMS_IN:
                for(i=0;i<10;i++){LED_BLUE=!LED_BLUE; __delay_ms(200);}
                task=WAITING;
            break;
            default:
            break;
        }
    }
}

