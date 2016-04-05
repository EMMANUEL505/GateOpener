/*
 * File:   main.c
 * Author: e882742
 *
 * Created on March 15, 2016, 12:15 PM
 */

//#include "general.h"
#include "general.h"
#include "system.h"
#include "eeprom.h"
#include "sim800l.h"
#include "gpio.h"
#include "usart.h"

void main()
{
    SYSTEMInit(_8MHZ);
    USARTInit(115);
    GPIOPortInit();
    SIM800init();

    EEPROMEraseAll();

    EEPROMSaveNumber("6141654818",10,10);
    EEPROMSaveNumber("6145136845",10,11);

    while(TRUE)
    {
        ei();                       //Enable interrupts
        RCSTAbits.CREN=ENABLED;     //Enable USART receiver

        USARTWriteLine("AT+CSQ\r\n");       //Check signal Quality
        __delay_ms(2000);                   //2 Second Delay

        while(SIM800L.busy==TRUE);
        di();                           //disable interrupts
        RCSTAbits.CREN=DISABLED;        //Disable USART receiver

        switch(task)
        {
            case WAITING:
                USARTClearSIM800L();
            break;
            case CALL_IN:
                USARTWriteLine("ATH\r\n");   //Hang up
                if(EEPROMSearchNumber(SIM800L.cell,SIM800L.cell_lenght))
                {
                    GPIOGreenLedToggle();
                    GPIORelayToggle();
                    SIM800SendSms(SIM800L.cell,SIM800L.csq);//SIM800L.csq);//"Received-Authorized");
                } //"Received-Authorized"); }
                else
                {
                    GPIORedLedBlink(5);
                    SIM800SendSms(SIM800L.cell, "Received-Unauthorized");}
                task=WAITING;
            break;
            case SMS_IN:
                for(i=0;i<10;i++){GPIOBlueLedToggle(); __delay_ms(200);}
                SIM800ReadSms(SIM800L.smsmem);
                SIM800DeleteSms("1", ALL_MESSAGES);
                task=WAITING;
            break;
            case COMMAND:
                SIM800SendSms("+526141654818", "Command received");
                task=WAITING;
            break;
            default:
            break;
        }
    }
}

