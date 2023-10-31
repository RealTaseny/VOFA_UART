#ifndef __UART_WYH_H__
#define __UART_WYH_H__

#include "HEADFILE_WYH.H"

#define CMD_FRAME_SIZE 10

typedef struct VOFA_command{
	uint8_t cmd_Type;
	uint8_t cmd_ID;
	uint8_t Valid_Data[4];
	uint8_t uart_Rx_Packet[CMD_FRAME_SIZE];			//串口数据包接收数组
	uint8_t Completion_Flag;
	float float_data;
}VOFA_CMD;

void uart_sendbytes_wyh(uint8_t *Array,uint8_t SIZE);
void uart_cmd_received(uint8_t byte_data);

#endif