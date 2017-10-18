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
    SYSTEMInit(_32MHZ); //Initialize core at 32MHz
    GPIOPortInit();     //Initialize port directions (output,input)
    GPIORelayClear();   //Ensure all relay outputs are disabled 
    USARTInit(9600);    //Initialize USART at 9600 baudrate
    I2CInit();          //Initialize I2C EEPROM

    GPIORedLedSet();    //All LED's remain enabled during GSM initialization
    GPIOBlueLedSet();   //All LED's remain enabled during GSM initialization
    GPIOGreenLedSet();  //All LED's remain enabled during GSM initialization
    SIM800init();       //Initialize GSM module, 18 sec approx 
    
    EEPROMUpdateVersion("v1.2");   //version writting
    password_empty[0]=0xFF; password_empty[1]=0xFF; password_empty[2]=0xFF; password_empty[3]=0xFF;//Empty password
    if(EEPROMCheckPassword(password_empty))   //If empty password use default password
    {
        EEPROMUpdatePassword("1234");   //Initialize password
    }
    
    GPIORedLedClear();  //Only Green led remains enabled indicating proper functionality
    GPIOBlueLedClear(); //Only Green led remains enabled indicating proper functionality
    GPIOGreenLedSet();  //Only Green led remains enabled indicating proper functionality

    ei();               //Enable interrupts
    RCSTAbits.CREN=ENABLED; //Enable USART receiver
    SIM800SendSms("6141654818", "System enabled","");  //After initialize unit sends a message
    while(TRUE) //Main loop
    {
        switch(task)
        {
            case WAITING:   
                if(RCSTAbits.OERR==1)  {CREN=DISABLED;__delay_us(200);RCSTAbits.CREN=ENABLED; } //Check for overwrite
                __delay_ms(200);
                if(chkcount<200) chkcount++;
                else{
                    chkcount=0;
                    USARTWriteLine("AT+CSQ\r\n");   //Check signal Quality
                    __delay_ms(500);
                    if(SIM800L.ok!=TRUE){
                        USARTWriteLine("AT+CSQ\r\n");   //Double check to avoid cycling due to send message error
                        while(SIM800L.ok!=TRUE);
                    }
                    else SIM800L.ok=FALSE;
                }
                CLRWDT();                
            break;
            
            case CALL_IN:
                USARTWriteLine("ATH\r\n");      //Hang up
                //USARTWriteLine("ATA\r\n");    //Answer
                //__delay_ms(5000);
                if(EEPROMSearchNumber(SIM800L.cell,SIM800L.cell_lenght))    //Check if authorized number is calling
                {                                      
                    GPIORelaySet(); GPIOGreenLedBlink(5); ; GPIORelayClear();   //Relay enabled period of 1sec
                    GPIOGreenLedSet();          //Green LED remains enabled
                } 
                else {  GPIORedLedBlink(5);   } //Red LED blinking, indicating denied access
                
                SIM800LClear();
                task=WAITING;
            break;
            
            case SMS_IN:
                //GPIOBlueLedBlink(5);
                GPIOBlueLedSet();
                SIM800ReadSms(SIM800L.smsmem);
                GPIOBlueLedClear();
                task=COMMAND;
            break;
            
            case COMMAND:
                //GPIOBlueLedSet();
                SIM800Command();
                GPIOBlueLedSet();
                SIM800LClear();
                SIM800DeleteSms("1", ALL_MESSAGES);
                GPIOBlueLedClear();
                task=WAITING;
            break;
            
            default:
            break;
        }
    }
}