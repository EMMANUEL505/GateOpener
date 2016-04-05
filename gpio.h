/* 
 * File:   gpio.h
 * Author: Emmanuel Sigala
 *
 * Created on March 15, 2016, 1:00 PM
 */

#ifndef GPIO_H
#define	GPIO_H

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* GPIO_H */

#define OUTPUT  0
#define INPUT   1

#define HIGH    1
#define LOW     0

#define RELAY_PIN               LATAbits.LATA4

#define RED_LED_PIN             LATCbits.LATC0
#define BLUE_LED_PIN            LATCbits.LATC1
#define GREEN_LED_PIN           LATCbits.LATC2

#define EEPROM_CS_PIN           LATBbits.LATB7
#define EEPROM_CLK_PIN          LATBbits.LATB6
#define EEPROM_DI_PIN           LATCbits.LATC7
#define EEPROM_DO_PIN           PORTBbits.RB4

#define SIM800_RST_PIN          LATCbits.LATC6

#define GPIORelayPut(state)     RELAY_PIN=state
#define GPIORelaySet()          RELAY_PIN=HIGH
#define GPIORelayClear()        RELAY_PIN=LOW
#define GPIORelayToggle()       RELAY_PIN=!RELAY_PIN

#define GPIORedLedPut(state)    RED_LED_PIN=state
#define GPIORedLedSet()         RED_LED_PIN=HIGH
#define GPIORedLedClear()       RED_LED_PIN=LOW
#define GPIORedLedToggle()      RED_LED_PIN=!RED_LED_PIN

#define GPIOBlueLedPut(state)   BLUE_LED_PIN=state
#define GPIOBlueLedSet()        BLUE_LED_PIN=HIGH
#define GPIOBlueLedClear()      BLUE_LED_PIN=LOW
#define GPIOBlueLedToggle()     BLUE_LED_PIN=!BLUE_LED_PIN

#define GPIOGreenLedPut(state)  GREEN_LED_PIN=state
#define GPIOGreenLedSet()       GREEN_LED_PIN=HIGH
#define GPIOGreenLedClear()     GREEN_LED_PIN=LOW
#define GPIOGreenLedToggle()    GREEN_LED_PIN=!GREEN_LED_PIN

void GPIOPortInit();
void GPIORedLedBlink(uint8_t times);