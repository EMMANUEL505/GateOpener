/* 
 * File:   sim800l.h
 * Author: Emmanuel Sigala
 *
 * Created on March 15, 2016, 1:58 PM
 */

#ifndef SIM800L_H
#define	SIM800L_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SIM800L_H */



//  SIM800L definitions
#define SIM800L_BUFFER_SIZE     64
#define SIM800L_CELL_LENGHT     15

#define READ_MESSAGES               "1"
#define READ_SENT_MESSAGES          "2"
#define READ_SENT_UNSENT_MESSAGES   "3"
#define ALL_MESSAGES                "4"

#ifndef HIGH
#define	HIGH        1
#endif

#ifndef LOW
#define	LOW         0
#endif

#define CTRLZ       0x1A

#define TRUE             1
#define FALSE            0
#define RESET_PIN(state)          LATCbits.LATC6=state

/*FUNCTION PROTOTYPES*/

typedef struct{
    uint8_t busy;       //Flag
    uint8_t uncomplete;
    uint8_t ok;
    uint8_t resp;
    uint8_t cell[SIM800L_CELL_LENGHT];
    uint8_t cell_lenght;
    uint8_t unreadsms;
    char smsmem[5];
    char buffer[SIM800L_BUFFER_SIZE];
    char command[25];
    char csq[5];
    char password[5];
}AT;




AT SIM800L;
uint8_t bufque;


uint8_t SIM800init();
uint8_t SIM800sendCommand();
uint8_t SIM800SendSms(const char *nmbr, const char *msg);
uint8_t SIM800ReadSms(const char *mem);
uint8_t SIM800DeleteSms(const char *index, const char *flag);
uint8_t SIM800Process();
uint8_t SIM800Command();
void SIM800LClear(void);
