#include "general.h"
#include "gpio.h"
#include "usart.h"
#include "eeprom.h"
#include "I2C.h"

void EepromStart()
{
    //The Start bit is detected by the device if CS and DI are
    //both high with respect to the positive edge of CLK for
    //the first time:
    EEPROM_CLK_PIN=LOW;             //Clk falling edge
    EEPROM_CS_PIN=HIGH;
    EEPROM_DI_PIN=HIGH;
    __delay_us(CLK_DELAY);
    EEPROM_CLK_PIN=HIGH;            //Clk rising edge
    __delay_us(CLK_DELAY);
    EEPROM_CLK_PIN=LOW;             //Clk falling edge
    __delay_us(CLK_DELAY/2);
}
void EepromEnd()
{
    EEPROM_CLK_PIN=LOW;             //Clk falling edge
    EEPROM_CS_PIN=LOW;
    EEPROM_DI_PIN=LOW;
    __delay_us(CLK_DELAY);
}
void EepromSend(uint8_t b)
{
    int8_t i=0;
    for(i=7;i>=0;i--)
    {
    EEPROM_DI_PIN=b>>i;             //&OPCODE_MASK;      //second bit
    __delay_us(CLK_DELAY/2);
    EEPROM_CLK_PIN=HIGH;            //Clk rising edge
    __delay_us(CLK_DELAY);
    EEPROM_CLK_PIN=LOW;             //Clk falling edge
    __delay_us(CLK_DELAY/2);
    }
}
uint8_t EepromReceive()
{
    uint8_t b=0;
    int8_t i=0;
    for(i=7;i>=0;i--)
    {
        __delay_us(CLK_DELAY/2);
        EEPROM_CLK_PIN=HIGH;            //Clk rising edge
        __delay_us(CLK_DELAY/2);
        b=b+(EEPROM_DO_PIN<<i);
        __delay_us(CLK_DELAY/2);
        EEPROM_CLK_PIN=LOW;             //Clk falling edge
        __delay_us(CLK_DELAY/2);
    }
    return b;
}
void EepromOpCode(uint8_t opcode, uint16_t address)
{
    int8_t i=0;
    EEPROM_DI_PIN=opcode>>1&OPCODE_MASK;      //second bit
    __delay_us(CLK_DELAY/2);
    EEPROM_CLK_PIN=HIGH;            //Clk rising edge
    __delay_us(CLK_DELAY);
    EEPROM_CLK_PIN=LOW;             //Clk falling edge
    __delay_us(CLK_DELAY/2);
    //OP code 3rd bit
    EEPROM_DI_PIN=opcode&OPCODE_MASK;      //third bit
    __delay_us(CLK_DELAY/2);
    EEPROM_CLK_PIN=HIGH;            //Clk rising edge
    __delay_us(CLK_DELAY);
    EEPROM_CLK_PIN=LOW;             //Clk falling edge
    __delay_us(CLK_DELAY/2);

    for(i=10;i>=0;i--)
    {
        EEPROM_DI_PIN=address>>i&OPCODE_MASK;      //second bit
        __delay_us(CLK_DELAY/2);
        EEPROM_CLK_PIN=HIGH;            //Clk rising edge
        __delay_us(CLK_DELAY);
        EEPROM_CLK_PIN=LOW;             //Clk falling edge
        __delay_us(CLK_DELAY/2);
    }
}
void EepromWrite(uint16_t address, uint8_t data)
{

/*
    EepromStart();
    EepromOpCode(EWEN, 0x600);
    EepromEnd();

    EepromStart();
    EepromOpCode(WRITE, address);
    EepromSend(data);
    EepromEnd();
*/
    EEPROM_CS_PIN=0;
    I2CStart();
    I2CSend(0xA0);
    I2CSend(address>>8);
    I2CSend(address);
    I2CSend(data);
    I2CStop();


}

uint8_t EepromRead(uint16_t address)
{
    uint8_t data=0;
    EEPROM_CS_PIN=0;
    I2CStart();
    /* Slave address */
    I2CSend(0xA0);
    /* 2-Byte Sub Address */
    I2CSend(address>>8);
    I2CSend(address);
    I2CRestart();
    /* Slave address with Read bit */
    I2CSend(0xA1);
    /* Read Byte from EEPROM */
    data = I2CRead();
    I2CNak();
    I2CStop();
        /*
    EepromStart();
    EepromOpCode(READ, address);
    data=EepromReceive();
    EepromEnd();*/

    return data;
}

uint8_t EEPROMSaveNumber(const char *nmbr, uint8_t nmbrLenght, uint16_t position)
{
    uint16_t count=0,result=1;
    uint16_t address=0;
    address=position*EEPROM_NUMBER_LENGHT;
    address=address+(EEPROM_NUMBER_LENGHT-1);

    for(count=EEPROM_NUMBER_LENGHT;count>0;count--)
    {
        if(nmbrLenght)
        {
            nmbrLenght--;
            if(*(nmbr+nmbrLenght)>='0' && *(nmbr+nmbrLenght)<='9')
                {
                    EepromWrite(address,*(nmbr+nmbrLenght));
                }
        }
        else
        {
            EepromWrite(address,'\0');
        }
        address--;
        __delay_ms(TWC_DELAY);
    }
    return result;
}

uint8_t EEPROMDeleteNumber(uint16_t position)
{
    uint8_t count=0;
    uint16_t address=0;
    address=position*EEPROM_NUMBER_LENGHT;

    for(count=0;count<EEPROM_NUMBER_LENGHT;count++)
    {
       EepromWrite(address+count,0xFF);
        __delay_ms(TWC_DELAY);
    }
    return TRUE;
}
uint16_t EEPROMSearchNumber(const char *nmbr, uint8_t nmbrLenght)
{
    uint16_t aux=0;
    uint16_t count=0, result=0;

    aux=EEPROM_NUMBER_LENGHT-1;
    //if(nmbrLenght>EEPROM_NUMBER_LENGHT) nmbrLenght=EEPROM_NUMBER_LENGHT;        //comment this line
    
    while(!result && aux<((EEPROM_MAX*EEPROM_NUMBER_LENGHT)-EEPROM_NUMBER_LENGHT))
    {
        count=0;
        do{
            //if(EepromRead(aux-count)==*(nmbr+EEPROM_NUMBER_LENGHT-(count+1)))   //For different number lenght its necesary to use nmbrLenght instead of EEPROM_NUMBER_LENGHT
            if(EepromRead(aux-count)==*(nmbr+nmbrLenght-(count+1)))    
                {result=1;}
            else
               {result=0;}
            count++;
        }while(result && count<nmbrLenght && count<EEPROM_NUMBER_LENGHT);//&& count<EEPROM_NUMBER_LENGHT);
        aux=aux+EEPROM_NUMBER_LENGHT;
    }
    if(result==TRUE)
    {
        return (aux/EEPROM_NUMBER_LENGHT);
    }
    else return FALSE;
}

void EEPROMEraseAll(void)
{
    /*EEPROM_CS_PIN=LOW;
    __delay_ms(100);
    EepromStart();
    EepromOpCode(EWEN, 0x600);
    EepromEnd();

    EepromStart();
    EepromOpCode(ERAL, 0x400);
    EepromEnd();
    __delay_ms(500);*/
    
    uint16_t i;
    for(i=0;i<800;i++)
    {
        EepromWrite(i,0xFF);
        __delay_ms(TWC_DELAY);
    }

}



uint8_t EEPROMAdd(const char *nmbr, uint8_t nmbrLenght)
{
    uint16_t empty=0,found=0;
    char aux=0;
    do
    {
        aux=EepromRead((empty*EEPROM_NUMBER_LENGHT));
        if(aux<'0' || aux>'9')
        {
            found=TRUE;
        }
        empty++;
    }while(!found&& empty<EEPROM_MAX);
    EEPROMSaveNumber(nmbr,nmbrLenght,(empty-1));

    return TRUE;
}
void EEPROMUpdatePassword(const char *pass)
{
    EepromWrite(PASSWORD1,*(pass));
    __delay_ms(TWC_DELAY);
    EepromWrite(PASSWORD2,*(pass+1));
    __delay_ms(TWC_DELAY);
    EepromWrite(PASSWORD3,*(pass+2));
    __delay_ms(TWC_DELAY);
    EepromWrite(PASSWORD4,*(pass+3));
    __delay_ms(TWC_DELAY);
}

void EEPROMUpdateVersion(const char *ver)
{
    EepromWrite(VERSION1,*(ver));
    __delay_ms(TWC_DELAY);
    EepromWrite(VERSION2,*(ver+1));
    __delay_ms(TWC_DELAY);
    EepromWrite(VERSION3,*(ver+2));
    __delay_ms(TWC_DELAY);
    EepromWrite(VERSION4,*(ver+3));
    __delay_ms(TWC_DELAY);
}


uint8_t EEPROMCheckPassword(const char *pass)
{
    password[0]=EepromRead(PASSWORD1);
    password[1]=EepromRead(PASSWORD2);
    password[2]=EepromRead(PASSWORD3);
    password[3]=EepromRead(PASSWORD4);
    if(*(pass)==password[0] && *(pass+1)==password[1] && *(pass+2)==password[2] && *(pass+3)==password[3])
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void EEPROMPrint()
{
    uint16_t x,y;
    for(x=0;x<1000;x++)
    {
        USARTWriteInt(x,3);
        USARTWriteLine("->");
        for(y=0;y<10;y++)
        {
            USARTWriteChar(EepromRead((x*10)+y));
        }
    }

}
