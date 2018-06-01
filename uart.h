#ifndef __UART_H_
#define __UART_H_

#include <avr/io.h>

#define UART0_DDR	DDRE
#define UART0_PORT	PORTE
#define UART0_TX	PE1
#define UART0_RX	PE0

#define UART1_DDR	DDRD
#define UART1_PORT	PORTD
#define UART1_TX	PD3
#define UART1_RX	PD2

void UART_init();
unsigned char UartReadByte();
void UART_snd_int(unsigned int i);
void UART_snd_str(unsigned char *p);
void UART_snd_byte (unsigned char byte);
#endif
