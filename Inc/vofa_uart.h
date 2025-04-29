#ifndef __UART_WYH_H__
#define __UART_WYH_H__

#include <stdint.h>
#include <string.h>

void uartSendByte(uint8_t c);
void uartSendData(uint8_t* Array, uint8_t SIZE);

extern uint8_t vofaRxBufferIndex;

#endif
