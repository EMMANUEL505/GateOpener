/*
 * File:   usart.h
 * Author: Emmanuel Sigala
 *
 * Created on March 15, 2016, 12:34 PM
 */
#ifndef USART_H
#define	USART_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* USART_PIC16_H */

void USARTInit(uint16_t baud_rate);
void USARTWriteChar(char ch);
void USARTWriteString(const char *str);
void USARTWriteLine(const char *str);
void USARTWriteInt(int16_t val, int8_t field_length);
void USARTHandleRxInt();
void USARTGotoNewLine();
