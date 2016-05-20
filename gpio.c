#include "general.h"
#include "gpio.h"


void GPIOPortInit()
{
    /*EEPROM port definitions*/
    TRISB7=OUTPUT;       //Configure PINB7 as output (EEPROM CS)
    TRISB6=OUTPUT;       //Configure PINB6 as output (EEPROM CLK)
    TRISC7=OUTPUT;       //Configure PINC7 as output (EEPROM DI)
    TRISB4=INPUT;        //Configure PINB4 as input (EEPROM DO)
    ANSELB=0x00;
    //LED's port definitions
    TRISC0=OUTPUT;       //Configure PINC0 as output (Red LED)
    TRISC1=OUTPUT;       //Configure PINC1 as output (Green LED)
    TRISC2=OUTPUT;       //Configure PINC2 as output (Blue LED)
    //USART port definitions
    TRISC4=OUTPUT;       //Configure PINC4 as output (USART TX)
    TRISC5=INPUT;        //Configure PINC5 as input (USART RX)
    //SIM800L port definitions
    TRISC6=OUTPUT;      //Configure PINC6 as output (SIM800L RST)
    //RELAY port definitions
    TRISA4=OUTPUT;       //Configure PINB4 as output (Relay)
    TRISA5=OUTPUT;       //Configure PINB4 as output (Relay)
}

void GPIORedLedBlink(uint8_t times)
{
    uint8_t i=0;
    for(i=0;i<times;i++)
    {
        GPIORedLedSet();
        __delay_ms(100);
        GPIORedLedClear();
        __delay_ms(100);
    }
}

void GPIOBlueLedBlink(uint8_t times)
{
    uint8_t i=0;
    for(i=0;i<times;i++)
    {
        GPIOBlueLedSet();
        __delay_ms(100);
        GPIOBlueLedClear();
        __delay_ms(100);
    }
}
