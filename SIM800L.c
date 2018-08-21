#include "general.h"
#include "sim800l.h"
#include "usart.h"
#include "gpio.h"
#include "eeprom.h"

uint8_t SIM800init()
{
    SIM800_RST_PIN=HIGH;
    __delay_ms(10);
    SIM800_RST_PIN=LOW;
    __delay_ms(500);
    SIM800_RST_PIN=HIGH;
    __delay_ms(5000);
    __delay_ms(5000);
    USARTWriteLine("AT\r\n");           //Test connection
    __delay_ms(2000);
    USARTWriteLine("ATE0\r\n");         //disable Echo
    __delay_ms(2000);
    USARTWriteLine("AT+CLIP=1\r\n");    //Enable CLIP notification
    __delay_ms(2000);
    /*if (sendCommand("AT")) {
        sendCommand("AT+IPR=115200");
        sendCommand("ATE0");
        sendCommand("AT+CFUN=1", 10000);
        return TRUE;
    }*/
    return FALSE;
}

uint8_t SIM800SendSms(const char *nmbr, const char *msg,const char *msg2)
{
    __delay_ms(2000);
    USARTWriteLine("AT+CMGF = 1\r\n");       //Text format
    __delay_ms(2000);
    USARTWriteLine("AT+CMGS=\"");     //Send SMS
    USARTWriteString(nmbr);
    USARTWriteString("\"\r\n");
    //Wait for >
    __delay_ms(2000);
    USARTWriteString(msg);
    USARTWriteString(msg2);    
    USARTWriteChar(CTRLZ);
    __delay_ms(2000);

    return 1;
}

uint8_t SIM800ReadSms(const char *mem)
{
    __delay_ms(200);
    USARTWriteLine("AT+CMGF = 1\r\n");      //Text format
    __delay_ms(200);
    USARTWriteLine("AT+CMGR=");             //Read SMS
    USARTWriteString(mem);
    USARTWriteString("\r\n");
    __delay_ms(600);
    return 1;
}
uint8_t SIM800DeleteSms(const char *index, const char *flag)
{
    __delay_ms(1000);
    USARTWriteLine("AT+CMGD=");     //Delete SMS
    USARTWriteString(index);
    USARTWriteChar(',');
    USARTWriteString(flag);
    USARTWriteString("\r\n");
    __delay_ms(2000);
    return 1;
}

uint8_t SIM800Command()
{
    if(SIM800L.command[0]==':')
    {
        if(EEPROMCheckPassword(&SIM800L.command[1]))   //Password
        {
            if(SIM800L.command[6]=='A' && SIM800L.command[7]=='D' && SIM800L.command[8]=='D' )      //"ADD" command for add new numbers
            {
                i=10;       //Number starts at 10 position  :pass:ADD:614xxxxxxx
                SIM800L.cell_lenght=0;  //Required after sender number acquirement
                while(SIM800L.command[i]>='0' && SIM800L.command[i]<='9' )
                {
                    SIM800L.cell[i-10]=SIM800L.command[i];  //Save cell number characters
                    i++;
                    SIM800L.cell_lenght++;
                }
                SIM800L.cell[i-10]='\0';    //'\0' character at last position to indicate where ends the string
                if(!EEPROMSearchNumber(SIM800L.cell,SIM800L.cell_lenght))       //ADD new number if doesn't exist
                {
                    EEPROMAdd(SIM800L.cell, SIM800L.cell_lenght);           //ADD new number to memory
                    SIM800SendSms(SIM800L.cell, "Su número ha sido agregado","");
                    SIM800SendSms(SIM800L.sender_cell, "Usted agregó el número: ",SIM800L.cell);
                }
                else
                {
                    SIM800SendSms(SIM800L.cell, "Su número ya existe","");
                    SIM800SendSms(SIM800L.sender_cell, "El número ya existe: ",SIM800L.cell);
                }
            }
            if(SIM800L.command[6]=='D' && SIM800L.command[7]=='E' && SIM800L.command[8]=='L' )      //"ADD" command for new numbers
            {
                i=10;                   //Number starts at 10 position  :pass:DEL:614xxxxxxx
                SIM800L.cell_lenght=0;  //Required after sender number acquire
                while(SIM800L.command[i]>='0' && SIM800L.command[i]<='9' )
                {
                    SIM800L.cell[i-10]=SIM800L.command[i];  //Save cell number characters
                    i++;
                    SIM800L.cell_lenght++;
                }
                SIM800L.cell[i-10]='\0';
                uint8_t add=EEPROMSearchNumber(SIM800L.cell,SIM800L.cell_lenght);
                if(add)       //Delete number if exists
                {
                    EEPROMDeleteNumber(add-1);
                    SIM800SendSms(SIM800L.cell, "Su número ha sido borrado","");
                    SIM800SendSms(SIM800L.sender_cell, "Usted borró el número: ",SIM800L.cell);
                    
                }
            }
            if(SIM800L.command[6]=='F' && SIM800L.command[7]=='A' && SIM800L.command[8]=='C' )      //"ADD" command for new numbers
            {
                EEPROMEraseAll();
                EEPROMUpdatePassword("1234");
            }
            if(SIM800L.command[6]=='P' && SIM800L.command[7]=='A' && SIM800L.command[8]=='S' )      //"ADD" command for new numbers
            {
                i=10;       //Password starts at 10 position  :pass:PAS:XYZW
                while(SIM800L.command[i]>='0' && SIM800L.command[i]<='9' )
                {
                    SIM800L.password[i-10]=SIM800L.command[i];  //Save password characters
                    i++;
                }
                SIM800L.password[i-10]='\0';
                if(i==14) EEPROMUpdatePassword(SIM800L.password); //create new password only if there are 4 chars
            } 
            if(SIM800L.command[6]=='C' && SIM800L.command[7]=='S' && SIM800L.command[8]=='Q' )      //"CSQ" command
            {
                SIM800SendSms(SIM800L.sender_cell,"The signal quality is: ",SIM800L.csq);
            }   
            if(SIM800L.command[6]=='V' && SIM800L.command[7]=='E' && SIM800L.command[8]=='R' )      //"VER" command
            {
                ver[0]=EepromRead(VERSION1);ver[1]=EepromRead(VERSION2);ver[2]=EepromRead(VERSION3);ver[3]=EepromRead(VERSION4);ver[4]='\0';
                SIM800SendSms(SIM800L.sender_cell,"The firmware version is: ",ver);
            } 
        }
        else if(SIM800L.command[1]=='O' && SIM800L.command[2]=='P' && SIM800L.command[3]=='E' )
        {
                if(EEPROMSearchNumber(SIM800L.sender_cell,SIM800L.cell_lenght))    //Check if authorized number is calling
                {                                      
                    GPIOSecRelaySet(); GPIOGreenLedBlink(6); ; GPIOSecRelayClear();   //Relay enabled period of 1sec
                    GPIOGreenLedSet();          //Green LED remains enabled
                } 
                else {  GPIORedLedBlink(5);   } //Red LED blinking, indicating denied access
        }
        else GPIORedLedBlink(5);        //If password is wrong red led will blink 5 times
    }
    return TRUE;  
}

void SIM800LClear(void)
{
    uint8_t i;
    SIM800L.busy=FALSE;
    SIM800L.uncomplete=FALSE;
    SIM800L.cell_lenght=FALSE;
    SIM800L.ok=FALSE;
    for(i=0;i<SIM800L_CELL_LENGHT;i++){SIM800L.cell[i]='\0'; SIM800L.sender_cell[i]='\0';}
    for(i=0;i<SIM800L_BUFFER_SIZE;i++) SIM800L.buffer[i]=0;
    for(i=0;i<25;i++) SIM800L.command[i]=0;
}
