/******************************************************************************

 Serial communication library for PIC16F series MCUs.

 Compiler: Microchip XC8 v1.12 (http://www.microchip.com/xc)

 Version: 1.0 (21 July 2013)

 MCU: PIC16F877A
 Frequency: 20MHz

                                     NOTICE

NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN
COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.


WRITTEN BY:
AVINASH GUPTA
me@avinashgupta.com

*******************************************************************************/
#include <stdint.h>
#include <xc.h>
#include <stdio.h>

#include <stdlib.h>
#include "config.h"

#include "usart_pic16.h"

void USARTInit(uint16_t baud_rate)
{       
    //SPBRG
    switch(baud_rate)
    {
     case 9600:
        SPBRG=51;
        break;
     case 19200:
        SPBRG=64;
        break;
     case 28800:
        SPBRG=42;
        break;
     case 33600:
        SPBRG=36;
        break;
     case 115:
        SPBRG=16;
        break;
    }

    BAUDCONbits.BRG16=1;
    //TXSTA
    TXSTAbits.TX9=0;  //8 bit transmission
    TXSTAbits.TXEN=1; //Transmit enable
    TXSTAbits.SYNC=0; //Async mode
    TXSTAbits.BRGH=1; //High speed baud rate

    //RCSTA
    RCSTAbits.SPEN=1;   //Serial port enabled
    RCSTAbits.RX9=0;    //8 bit mode
    RCSTAbits.CREN=1;   //Enable receive
    RCSTAbits.ADDEN=0;  //Disable address detection

    //Receive interrupt
    RCIE=1;
    PEIE=1;

    ei();

}

void USARTWriteChar(char ch)
{
  while(!PIR1bits.TXIF);
  TXREG=ch;
}

void USARTWriteString(const char *str)
{
  while(*str!='\0')
  {
      USARTWriteChar(*str);
      str++;
  }
}

void USARTWriteLine(const char *str)
{
    USARTWriteChar('\r');//CR
    USARTWriteChar('\n');//LF
    USARTWriteString(str);
}

void USARTHandleRxInt()
{
    LED_BLUE=ENABLED;
    //Read the data
    char data=RCREG;
    SIM800L.busy=TRUE;
    switch(data)
    {
        case '\r':      //Do nothing for first CR incomming character
            if(SIM800L.uncomplete==TRUE && bufque==0){SIM800L.uncomplete=FALSE;}  //Used for lost of sync
        break;
        case '\n':
            if(SIM800L.uncomplete==FALSE) SIM800L.uncomplete=TRUE;
            else
            {
                SIM800L.uncomplete=FALSE;
                if(SIM800L.buffer[0]=='+')                         //Case for unsolicited messages
                {
                   if(SIM800L.buffer[1]=='C' && SIM800L.buffer[2]=='L' && SIM800L.buffer[3]=='I'  )    //+CLIP, save caller's cellnumber
                   {
                       uint8_t ci=8;                               //Cell number initiates at eight position [+CLIP: "89..."
                       while(SIM800L.buffer[ci]!='\"' && (ci-8)<SIM800L_CELL_LENGHT)             //Save characters until " arrive
                       {
                           SIM800L.cell[ci-8]=SIM800L.buffer[ci];  //Save cell number characters
                           ci++;
                           SIM800L.cell_lenght++;                  //Save cell number lenght
                       }
                       task=CALL_IN;                               //Configure task to CALL_IN
                   }
                  if(SIM800L.buffer[1]=='C' && SIM800L.buffer[2]=='M' && SIM800L.buffer[3]=='T' && SIM800L.buffer[4]=='I'  )    //+CMTI, save SMS
                   {

                       task=SMS_IN;                               //Configure task to SMS_IN
                   }
                }
                bufque=0;
             }
             SIM800L.busy=FALSE;
         break;
         default:
             SIM800L.buffer[bufque]=data;
             bufque++;
         break;
     }
    LED_BLUE=DISABLED;
}
void USARTWriteInt(int16_t val, int8_t field_length)
{
    char str[5]={0,0,0,0,0};
    int8_t i=4,j=0;
    //Handle negative integers
    if(val<0)
    {
        USARTWriteChar('-');   //Write Negative sign
        val=val*-1;     //convert to positive
    }
    else
    {
        USARTWriteChar(' ');
    }

    if(val==0 && field_length<1)
    {
        USARTWriteChar('0');
        return;
    }
    while(val)
    {
        str[i]=val%10;
        val=val/10;
        i--;
    }

    if(field_length==-1)
        while(str[j]==0) j++;
    else
        j=5-field_length;


    for(i=j;i<5;i++)
    {
        USARTWriteChar('0'+str[i]);
    }
}

void USARTGotoNewLine()
{
    USARTWriteChar('\r');//CR
    USARTWriteChar('\n');//LF
}

void USARTClearSIM800L(void)
{
    uint8_t i;
    SIM800L.busy=FALSE;
    SIM800L.uncomplete=FALSE;
    SIM800L.cell_lenght=FALSE;
    for(i=0;i<SIM800L_CELL_LENGHT;i++){SIM800L.cell[i]='\0';}
    for(i=0;i<SIM800L_BUFFER_SIZE;i++) SIM800L.buffer[i]=0;
}