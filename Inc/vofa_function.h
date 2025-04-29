#ifndef VOFA_FUNCTIONS_H__
#define VOFA_FUNCTIONS_H__

#define INVALID 0xFF
#define FRAME_TAIL_SIZE (4U)
#define INCREASE_STEP (0.01f)
#define DECREASE_STEP (0.01f)
#define CH_COUNT (3U)				//开启的通道数量
#define CMD_FRAME_SIZE 10

#include <stdint.h>

enum CommandID
{
	Direct_Assignment,
	Increase,
	Decrease
};

enum CommandType
{
	Speed,
	Position
};

enum va_list
{
	UINT8_T,
	UINT16_T,
	UINT32_T,
	FLOAT
};

enum Channels
{
	CH1,
	CH2,
	CH3,
	CH4,
	CH5,
	CH7,
	CH8
};

typedef struct vofaJustFloatFrame
{
	float   fdata[CH_COUNT];
	uint8_t frametail[FRAME_TAIL_SIZE];
} vofaJustFloatFrame;

typedef struct vofaCommand
{
	uint8_t cmdType;
	uint8_t cmdID;
	uint8_t validData[4];
	uint8_t uartRxPacket[CMD_FRAME_SIZE]; //串口数据包接收数组
	uint8_t completionFlag;
	float   floatData;
} vofaCommand;

#ifdef __cplusplus
extern "C" {
#endif

void vofaSendJustFloat(float* CH_data);
void vofaSendFirewater(float fdata);
void vofaSendRawdata(uint8_t Data);
void vofaSendJustFloatFrameTail(uint8_t* Array);

void vofaJustFloatInit(void);
void uartCMDRecv(uint8_t byte_data);
void vofaCommandParse(void);

extern vofaJustFloatFrame JustFloat_Data;
extern vofaCommand vofaCommandData;

#ifdef __cplusplus
}
#endif

#endif
