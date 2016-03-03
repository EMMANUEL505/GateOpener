#include <stdint.h>
#include <xc.h>
#include <stdio.h>
#include "config.h"
#include <stdlib.h>
#include "SIM800L.h"

uint8_t SIM800init()
{
    RESET_PIN(HIGH);
    __delay_ms(10);
    RESET_PIN(LOW);
    __delay_ms(500);
    RESET_PIN(HIGH);
    __delay_ms(10000);
    USARTWriteLine("AT\r\n");       //Test connection
    __delay_ms(2000);
    USARTWriteLine("ATE0\r\n");     //disable Echo
    __delay_ms(2000);
    /*if (sendCommand("AT")) {
        sendCommand("AT+IPR=115200");
        sendCommand("ATE0");
        sendCommand("AT+CFUN=1", 10000);
        return TRUE;
    }*/
    return FALSE;
}

uint8_t SIM800sendCommand()
{
    USARTWriteLine("AT\n\r");       //Test connection
    return 0;
}