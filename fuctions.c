#include <stdint.h>
#include <xc.h>
#include <stdio.h>

#include <stdlib.h>
#include "config.h"

#include "usart_pic16.h"


void EEPROMClear(uint8_t fromAddr, uint8_t toAddr)
{
    uint8_t count=0;
    for(count=fromAddr;count<=toAddr;count++)
    {
        eeprom_write(count,'\0');
    }
}

uint8_t EEPROMSaveNumber(const char *nmbr, uint8_t nmbrLenght, uint8_t position)
{
    uint8_t count=0,value=0,result=1;
    for(count=EEPROM_NUMBER_LENGHT;count>0;count--)
    {
        if(nmbrLenght)
        {
            nmbrLenght--;
            if(*(nmbr+nmbrLenght)>='0') 
                {value=*(nmbr+nmbrLenght)-'0';}     //Convert from ASCII to decimal
            else result=FALSE;      //Error if invalid character
            if(nmbrLenght)
            {
                nmbrLenght--;
                if(*(nmbr+nmbrLenght)>='0') 
                    {value=value+((*(nmbr+nmbrLenght)-'0')<<4);}     //Convert from ASCII to decimal
                else result=FALSE;      //Error if invalid character
            }
        }
        else value='\0';

        eeprom_write((position*EEPROM_NUMBER_LENGHT)+(count-1),value);
    }
    return result;
}

uint8_t EEPROMSearchNumber(const char *nmbr, uint8_t nmbrLenght)
{
    char convNmbr[EEPROM_NUMBER_LENGHT];
    uint8_t count=0,value=0,aux=0, result=0;
    for(count=EEPROM_NUMBER_LENGHT;count>0;count--)
    {
        if(nmbrLenght)
        {
            nmbrLenght--;
            if(*(nmbr+nmbrLenght)>='0') value=*(nmbr+nmbrLenght)-48;
            if(nmbrLenght)
            {
                nmbrLenght--;
                if(*(nmbr+nmbrLenght)>='0') value=value+((*(nmbr+nmbrLenght)-48)<<4);             
            }
        }
        else
        {
            value=10;//'\0';
        }
        convNmbr[count-1]=value;
    }
    aux=EEPROM_NUMBER_LENGHT-1;

    while(!result && aux<(EEPROM_SIZE-EEPROM_NUMBER_LENGHT))
    {
        count=0;
        do{
            if(eeprom_read(aux-count)==convNmbr[EEPROM_NUMBER_LENGHT-(count+1)]){result=1;}
            count++;
        }while(result && count<EEPROM_NUMBER_LENGHT); 
        aux=aux+EEPROM_NUMBER_LENGHT;
    }
    return result;
}