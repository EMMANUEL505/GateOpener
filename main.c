/*
 * File:   main.c
 * Author: Adolfo Sigala
 *
 * Created on March 15, 2016, 12:15 PM
 */

#include "general.h"
#include "system.h"
#include "eeprom.h"
#include "sim800l.h"
#include "gpio.h"
#include "usart.h"

void main()
{
    SYSTEMInit(_8MHZ);  //Initialize core at 8MHz
    USARTInit(115);     //Initialize USART at 115200 baudrate
    GPIOPortInit();     //Initialize port directions (output,input)
    //EEPROMPrint();
    SIM800init();       //Initialize GSM module
    EEPROMUpdatePassword("1234");
    GPIORedLedClear();
    GPIOBlueLedClear();
    GPIOGreenLedClear();

    ei();                       //Enable interrupts
    RCSTAbits.CREN=ENABLED;     //Enable USART receiver

    SIM800SendSms("6141654818", "System enabled");
    while(TRUE)
    {
        switch(task)
        {
            case WAITING:
                if(RCSTAbits.OERR==1)  {CREN=DISABLED;__delay_us(200);RCSTAbits.CREN=ENABLED; } //Check for overwrite
                USARTWriteLine("AT+CSQ\r\n");               //Check signal Quality
                __delay_ms(2000);
            break;
            case CALL_IN:
                USARTWriteLine("ATH\r\n");   //Hang up
                if(EEPROMSearchNumber(SIM800L.cell,SIM800L.cell_lenght))
                {
                    GPIORelaySet();
                    __delay_ms(1000);
                    GPIORelayClear();
                } 
                else
                {
                    GPIORedLedBlink(5);
                }
                SIM800LClear();
                task=WAITING;
            break;
            case SMS_IN:
                GPIOBlueLedBlink(5);
                SIM800ReadSms(SIM800L.smsmem);
                //SIM800DeleteSms("1", ALL_MESSAGES);
                task=COMMAND;
            break;
            case COMMAND:
                GPIORedLedSet();
                SIM800Command();
                SIM800LClear();
                SIM800DeleteSms("1", ALL_MESSAGES);
                GPIORedLedClear();
                task=WAITING;
            break;
            default:
            break;
        }
    }
}

