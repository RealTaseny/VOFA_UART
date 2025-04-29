#include "vofa_function.h"
#include <stdio.h>
#include "base_transfer.h"
#include "vofa_uart.h"

/*VOFA type structure*/
vofaJustFloatFrame JustFloat_Data;
vofaCommand        vofaCommandData;

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

void vofaSendJustFloat(float* chData)
{
	uint8_t u8Array[4];
	float2uint8Array(u8Array, chData, 0);
	uartSendData(u8Array, sizeof(u8Array));
}

void vofaSendJustFloatFrameTail(uint8_t* Array)
{
	uartSendData(Array, FRAME_TAIL_SIZE);
}

void vofaSendFirewater(float fdata)
{
	printf("%.6f\n", fdata);
}

void vofaSendRawdata(uint8_t Data)
{
	uartSendByte(Data);
}

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

//@S/P1/2/3=%%!#
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
