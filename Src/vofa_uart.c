#include "vofa_uart.h"
#include "vofa_function.h"
#include "usart.h"

uint8_t            vofaRxBufferIndex = 0;
extern vofaCommand cmd;

void uartSendByte(const uint8_t c)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&c, 1, HAL_MAX_DELAY); //修改为串口发送接口
}

void uartSendData(uint8_t* Array, uint8_t SIZE)
{
	while (SIZE)
	{
		uartSendByte(*Array);
		Array++;
		SIZE--;
	}
}