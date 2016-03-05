/* 
 * File:   config.h
 * Author: e882742
 *
 * Created on February 29, 2016, 11:51 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "usart_pic16.h"
#include "functions.h"

#define _XTAL_FREQ  8000000        // oscillator frequency for _delay()

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

//  Ouput state definitions
#define DISABLED                0
#define ENABLED                 1
#define LED_RED              LATCbits.LATC0
#define LED_GREEN            LATCbits.LATC1
#define LED_BLUE             LATCbits.LATC2
#define RELAY                LATBbits.LATB4
#define FALSE                0
#define TRUE                 1

//  Task state definitions
#define WAITING    0
#define CALL_IN     1
#define SMS_IN      2

//  EEPROM definitions
#define EEPROM_SIZE             256
#define EEPROM_NUMBER_LENGHT    4

//  SIM800L definitions
#define SIM800L_BUFFER_SIZE     128
#define SIM800L_CELL_LENGHT     15

typedef struct{
    uint8_t busy;
    uint8_t uncomplete;
    uint8_t resp;
    uint8_t cell[SIM800L_CELL_LENGHT];
    uint8_t cell_lenght;
    uint8_t unreadsms;
    uint8_t smsmem[10];
    char buffer[SIM800L_BUFFER_SIZE];
}AT;

AT SIM800L;
uint8_t bufque;

uint8_t task;