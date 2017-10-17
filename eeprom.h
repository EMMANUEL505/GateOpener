/* 
 * File:   eeprom.h
 * Author: Emmanuel Sigala
 *
 * Created on March 15, 2016, 1:58 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */


#define READ            0b00000110
#define EWEN            0b00000100
#define ERASE           0b00000111
#define ERAL            0b00000100
#define WRITE           0b00000101
#define WRAL            0b00000100
#define EWDS            0b00000100

#define OPCODE_MASK     0b00000001

#define CLK_DELAY       4       //Value in us, it must be divisible by two
#define TWC_DELAY       5       //Value in ms
//64Kbit memory capacity, can save up to 8,000 8-bit characters, this could be used for 800 10char numbers
#define PASSWORD1       7900
#define PASSWORD2       7901
#define PASSWORD3       7902
#define PASSWORD4       7903

#define VERSION1         7904
#define VERSION2         7905
#define VERSION3         7906
#define VERSION4         7907
//  EEPROM definitions

#define EEPROM_NUMBER_LENGHT    10
#define EEPROM_MAX             450

void EepromStart();
void EepromEnd();
void EepromSend(uint8_t b);
uint8_t EepromReceive();
uint8_t EepromRead(uint16_t address);
void EepromOpCode(uint8_t opcode, uint16_t address);
void EepromWrite(uint16_t address, uint8_t data);
uint8_t EEPROMSaveNumber(const char *nmbr, uint8_t nmbrLenght, uint16_t position);
uint8_t EEPROMDeleteNumber(uint16_t position);
uint16_t EEPROMSearchNumber(const char *nmbr, uint8_t nmbrLenght);
void EEPROMEraseAll(void);
uint8_t EEPROMAdd(const char *nmbr, uint8_t nmbrLenght);
void EEPROMUpdatePassword(const char *pass);
void EEPROMUpdateVersion(const char *ver);
uint8_t EEPROMCheckPassword(const char *pass);
void EEPROMPrint();