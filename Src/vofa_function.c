#include "vofa_function.h"
#include <stdio.h>
#include "base_transfer.h"
#include "vofa_uart.h"

/*VOFA type structure*/
vofaJustFloatFrame JustFloat_Data;
vofaCommand        cmd;

void vofaJustFloatInit(void)
{
	cmd.cmdID                   = INVALID;
	cmd.cmdType                 = INVALID;
	cmd.completionFlag          = 0;
	JustFloat_Data.frametail[0] = 0x00;
	JustFloat_Data.frametail[1] = 0x00;
	JustFloat_Data.frametail[2] = 0x80;
	JustFloat_Data.frametail[3] = 0x7f;
}

void vofaSendJustFloat(float* chData)
{
	uint8_t u8Array[4];
	float2uint8_tArray(u8Array, chData, 1);
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
	cmd.uartRxPacket[vofaRxBufferIndex] = byte_data;

	if (cmd.uartRxPacket[vofaRxBufferIndex - 1] == '!' && cmd.uartRxPacket[vofaRxBufferIndex] == '#')
	{
		cmd.completionFlag = 1;
		vofaRxBufferIndex  = 0;
	}

	else if (vofaRxBufferIndex > (CMD_FRAME_SIZE - 1))
	{
		cmd.completionFlag = 0;
		vofaRxBufferIndex  = 0;
		memset(cmd.uartRxPacket, 0, 10);
	}

	else
	{
		vofaRxBufferIndex++;
	}
}

void vofaCommandParse(void)
{
	uint8_t* pRxPacket;
	pRxPacket = cmd.uartRxPacket;

	if (cmd.uartRxPacket[0] != '@' || cmd.uartRxPacket[3] != '=' || cmd.uartRxPacket[CMD_FRAME_SIZE - 2] != '!' || cmd.
		uartRxPacket[CMD_FRAME_SIZE - 1] != '#')
	{
		memset(cmd.uartRxPacket, 0, CMD_FRAME_SIZE);
		return;
	}

	switch (cmd.uartRxPacket[1])
	{
		case 'S': cmd.cmdType = Speed;
			break;
		case 'P': cmd.cmdType = Position;
			break;
		default: cmd.cmdType = INVALID;
			break;
	}

	switch (cmd.uartRxPacket[2])
	{
		case '1': cmd.cmdID = Direct_Assignment;
			break;
		case '2': cmd.cmdID = Increase;
			break;
		case '3': cmd.cmdID = Decrease;
			break;
		default: cmd.cmdID = INVALID;
			break;
	}
	memcpy(cmd.validData, pRxPacket + 4, 4);

	cmd.floatData = uint8_tArray2float(cmd.validData, 1);

	pRxPacket = NULL;
	memset(cmd.validData, 0, 4);
	memset(cmd.uartRxPacket, 0, CMD_FRAME_SIZE);
}
