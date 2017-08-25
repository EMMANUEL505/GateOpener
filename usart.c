#include "general.h"
#include "usart.h"
#include "gpio.h"
#include "sim800l.h"

void USARTInit(uint16_t baud_rate)
{       
    //SPBRG
    switch(baud_rate)
    {
     case 9600:
        //SPBRG=51;     //Use for 8MHZ
         SPBRG=207;     //Use for 32MHZ
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

    APFCON0bits.RXDTSEL=TRUE;          //Configure RX function on PORTC pin
    APFCON0bits.TXCKSEL=TRUE;          //Configure TX function on PORTC pin
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

    ei();   //Enable interrupts

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
    GPIOBlueLedSet();
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
                SIM800L.buffer[bufque]=0;   //End of string
                SIM800L.uncomplete=FALSE;   //Clear flag indicating full buffer received
                
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
                       SIM800L.cell[ci-8]='\0';
                       task=CALL_IN;                               //Configure task to CALL_IN
                   }
                  if(SIM800L.buffer[1]=='C' && SIM800L.buffer[2]=='M' && SIM800L.buffer[3]=='T' && SIM800L.buffer[4]=='I'  )    //+CMTI, save SMS
                   {
                       uint8_t ci=12;                               //SMS memory location initiates at twelve position [+CMTI: "SM",XX..
                       while(SIM800L.buffer[ci]>='0' && SIM800L.buffer[ci]<='9' )             //Save sms location
                       {
                           SIM800L.smsmem[ci-12]=SIM800L.buffer[ci];  //Save memory location
                           ci++;
                       }
                       SIM800L.smsmem[ci-12]=0;
                       SIM800L.unreadsms=1;
                       task=SMS_IN;                               //Configure task to SMS_IN
                   }
                  if(SIM800L.buffer[1]=='C' && SIM800L.buffer[2]=='S' && SIM800L.buffer[3]=='Q')    //+CSQ, save CSQ
                   {
                     uint8_t ci=6;
                     while(SIM800L.buffer[ci]!=',' && (ci)<8)             //Save characters until , arrive
                     {
                         SIM800L.csq[ci-6]=SIM800L.buffer[ci];  //Save cell number characters
                         ci++;
                     }
                     SIM800L.csq[ci-6]=0;
                   }
                   if(SIM800L.buffer[1]=='C' && SIM800L.buffer[2]=='M' && SIM800L.buffer[3]=='D')
                   {
                     uint8_t ci=4;
                     while(SIM800L.buffer[ci]!='#' && (ci-4)<25)             //Save characters until " arrive
                      {
                          SIM800L.command[ci-4]=SIM800L.buffer[ci];  //Save cell number characters
                          ci++;
                      }
                     task=COMMAND;
                   }
                   if(SIM800L.buffer[1]=='C' && SIM800L.buffer[2]=='M' && SIM800L.buffer[3]=='G' && SIM800L.buffer[4]=='R')
                   {
                       uint8_t ci=20,end=0;                                           //ci=21 for "614xxxxxxx"
                       //uint8_t ci=21;                                           //ci=21 for "614xxxxxxx"
                       //if(SIM800L.buffer[ci]=='+') ci=24;                       //ci=24 for "+52614xxxxxxx"
                       while(end==0 && ci<40)
                       {
                           ci++;
                           if(SIM800L.buffer[ci]=='\"') end=1;
                       }
                       ci=ci-10;
                       //if(SIM800L.buffer[ci]=='+') ci=24;                       //ci=24 for "+52614xxxxxxx"
                       while(SIM800L.buffer[ci]!='\"' && SIM800L.cell_lenght<SIM800L_CELL_LENGHT && end)             //Save characters until " arrive
                        {
                            SIM800L.sender_cell[SIM800L.cell_lenght]=SIM800L.buffer[ci];  //Save cell number characters
                            ci++;
                            SIM800L.cell_lenght++;                  //Save cell number lenght
                        }
                        SIM800L.sender_cell[SIM800L.cell_lenght]='\0';   
                        SIM800L.cell_lenght=0;
                   }
                }
               else if(SIM800L.buffer[0]=='O' && SIM800L.buffer[1]=='K') SIM800L.ok=TRUE;
                bufque=0;
             }
             SIM800L.busy=FALSE;    //Clear busy flag
         break;
         default:
             if(bufque<(SIM800L_BUFFER_SIZE-1))
             {
                SIM800L.buffer[bufque]=data;
                bufque++;
             }
         break;
     }
    GPIOBlueLedClear();
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
