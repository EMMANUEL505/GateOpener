/* 
 * File:   SIM800L.h
 * Author: e882742
 *
 * Created on March 2, 2016, 11:19 AM
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

#ifndef HIGH
#define	HIGH        1
#endif

#ifndef LOW
#define	LOW         0
#endif

#define TRUE             1
#define FALSE            0
#define RESET_PIN(state)          LATCbits.LATC6=state

/*FUNCTION PROTOTYPES*/

uint8_t SIM800init();
uint8_t SIM800sendCommand();