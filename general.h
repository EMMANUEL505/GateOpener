/* 
 * File:   general.h
 * Author: Emmanuel Sigala
 *
 * Created on March 15, 2016, 12:44 PM
 */

#ifndef GENERAL_H
#define	GENERAL_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* GENERAL_H */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#define _XTAL_FREQ  32000000        // oscillator frequency for _delay()

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = ON       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = ON          // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = ON         // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON      // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

//  Ouput state definitions
#define DISABLED                0
#define ENABLED                 1
//
#define FALSE                0
#define TRUE                 1

//  Task state definitions
#define WAITING     0
#define CALL_IN     1
#define SMS_IN      2
#define COMMAND     3

uint8_t task,i,getsmsnumber;
char password[4];
char password_empty[4];