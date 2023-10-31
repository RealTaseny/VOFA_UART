#ifndef __VOFA_FUNCTIONS_WYH_H__
#define __VOFA_FUNCTIONS_WYH_H__

#include "HEADFILE_WYH.H"

#define INVALID 0xFF
#define FRAME_TAIL_SIZE 4
#define INCREASE_STEP 0.01
#define DECREASE_STEP 0.01
#define CH_COUNT 3

enum CommandID {
	Direct_Assignment,
	Increase,
	Decrease
};

enum CommandType {
	Speed,
	Position
};

enum va_list {
	UINT8_T,
	UINT16_T,
	UINT32_T,
	FLOAT
};

enum Channels {
	CH1,
	CH2,
	CH3,
	CH4,
	CH5,
	CH7,
	CH8
};

typedef struct JustFloatFrame{
	float fdata[CH_COUNT];
	uint8_t frametail[FRAME_TAIL_SIZE];
}JF_Frame;

void VOFA_Justfloat_Init_wyh(void);
void VOFA_Send_Justfloat_CH_wyh(float *CH_data);
void printf_float_wyh(float *float_data);
void VOFA_Send_Firewater_wyh(float fdata);
void VOFA_Send_Rawdata_wyh(uint8_t Data);
void VOFA_Send_Justfloat_frametail(uint8_t *Array);
void VOFA_translate_cmd_wyh(void);

#endif