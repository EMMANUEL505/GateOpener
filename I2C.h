/* 
 * File:   I2C.h
 * Author: E882742
 *
 * Created on May 20, 2016, 3:29 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

void I2CInit();
void I2CStart();
void I2CStop();
void I2CRestart();
void I2CAck();
void I2CNak();
void I2CWait();
void I2CSend(unsigned char dat);
unsigned char I2CRead(void);