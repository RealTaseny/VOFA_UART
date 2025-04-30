#include "vofa_function.h"
#include <stdio.h>
#include "base_transfer.h"
#include "vofa_uart.h"

/*VOFA type structure*/
vofaJustFloatFrame JustFloat_Data;
vofaCommand        vofaCommandData;

/**
* @param vofaJFFframe: 包含数据帧的结构体
* @return void
*/
void vofaSendJustFloat(vofaJustFloatFrame *vofaJFFrame)
{
	uint8_t i;
	uint8_t u8Array[4];
	for (i = 0; i < CH_COUNT; i++)
	{
		float2uint8Array(u8Array, vofaJFFrame->fdata[i], 0);
		uartSendData(vofaJFFrame->u8Array, sizeof(u8Array));
	}
	uartSendData(vofaJFFrame->frametail, FRAME_TAIL_SIZE);
}

/**
* @param fdata: 指向要发送的浮点数据的指针
* @param ulSize： 要发送的数据个数
* @return void
*/
void vofaSendFirewater(const float *fdata, const uint32_t ulSize)
{
	uint32_t i;
	for (i = 0; i < ulSize - 1; i++)
	{
		printf("%.6f,", *(fdata + i));
	}
	printf("%.6f\n", *(fdata + i));
}

/**
* @param pData: 指向要发送的单字节数据的指针
* @param ulSize： 要发送的数据个数
* @return void
*/
void vofaSendRawdata(uint8_t *pData, const uint32_t ulSize)
{
	uint32_t i;
	for (i = 0; i < ulSize; i++)
	{
		uartSendByte(*(Data + i));
	}
}

/**
* @brief 初始化JustFloat帧结构体
*/
void vofaJustFloatInit(void)
{
	vofaCommandData.cmdID                   = INVALID;
	vofaCommandData.cmdType                 = INVALID;
	vofaCommandData.completionFlag          = 0;
	JustFloat_Data.frametail[0] = 0x00;
	JustFloat_Data.frametail[1] = 0x00;
	JustFloat_Data.frametail[2] = 0x80;
	JustFloat_Data.frametail[3] = 0x7f;
}

/**
* @brief 将串口收到的数据判断并存入数据包中，并比对帧控制接收完成标志位置位
* @param byte_data： 串口接收到的字节数据 
*/
void uartCMDRecv(uint8_t byte_data) //此函数放在串口中断中
{
	vofaCommandData.uartRxPacket[vofaRxBufferIndex] = byte_data;

	if (vofaCommandData.uartRxPacket[vofaRxBufferIndex - 1] == '!' && vofaCommandData.uartRxPacket[vofaRxBufferIndex] == '#')
	{
		vofaCommandData.completionFlag = 1;
		vofaRxBufferIndex  = 0;
	}

	else if (vofaRxBufferIndex > (CMD_FRAME_SIZE - 1))
	{
		vofaCommandData.completionFlag = 0;
		vofaRxBufferIndex  = 0;
		memset(vofaCommandData.uartRxPacket, 0, 10);
	}

	else
	{
		vofaRxBufferIndex++;
	}
}

/**
* @brief vofa命令帧解析
*/
void vofaCommandParse(void)
{
	uint8_t* pRxPacket;
	pRxPacket = vofaCommandData.uartRxPacket;

	if (vofaCommandData.uartRxPacket[0] != '@' || vofaCommandData.uartRxPacket[3] != '=' || vofaCommandData.uartRxPacket[CMD_FRAME_SIZE - 2] != '!' || vofaCommandData.
		uartRxPacket[CMD_FRAME_SIZE - 1] != '#')
	{
		memset(vofaCommandData.uartRxPacket, 0, CMD_FRAME_SIZE);
		return;
	}

	switch (vofaCommandData.uartRxPacket[1])
	{
		case 'S': vofaCommandData.cmdType = Speed;
			break;
		case 'P': vofaCommandData.cmdType = Position;
			break;
		default: vofaCommandData.cmdType = INVALID;
			break;
	}

	switch (vofaCommandData.uartRxPacket[2])
	{
		case '1': vofaCommandData.cmdID = Direct_Assignment;
			break;
		case '2': vofaCommandData.cmdID = Increase;
			break;
		case '3': vofaCommandData.cmdID = Decrease;
			break;
		default: vofaCommandData.cmdID = INVALID;
			break;
	}
	memcpy(vofaCommandData.validData, pRxPacket + 4, 4);

	vofaCommandData.floatData = uint8Array2Float(vofaCommandData.validData, 0);

	pRxPacket = NULL;
	memset(vofaCommandData.validData, 0, 4);
	memset(vofaCommandData.uartRxPacket, 0, CMD_FRAME_SIZE);
}