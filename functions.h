/* 
 * File:   functions.h
 * Author: e882742
 *
 * Created on March 4, 2016, 12:58 PM
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTIONS_H */

void EEPROMClear(uint8_t fromAddr, uint8_t toAddr);
uint8_t EEPROMSaveNumber(const char *nmbr, uint8_t nmbrLenght, uint8_t position);
uint8_t EEPROMSearchNumber(const char *nmbr, uint8_t nmbrLenght);