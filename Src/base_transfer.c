#include "base_transfer.h"

//使用小端(Little-endian)的架构：
//最常用x86架构(包括x86_64)，还有 6502 (including 65802, 65C816), Z80 (including Z180, eZ80 etc.), MCS-48, 8051, DEC Alpha, Altera Nios, Atmel AVR, SuperH, VAX, 和 PDP-11 等等；

//使用大端(Big-endian)的架构：
// Motorola 6800 and 68k, Xilinx Microblaze, IBM POWER, system/360, System/370 等等。

//支持配置endian的架构：
//如下架构有配置endian为大端、小端中任一种的功能， ARM, PowerPC, Alpha, SPARC V9, MIPS, PA-RISC 和 IA-64 等等。


float uint8Array2Float(const uint8_t* u8Array, const uint8_t Flip)
{
	float   fdata = 0;
	uint8_t Data[4];
	if (Flip)
	{
		Data[3] = u8Array[0];
		Data[2] = u8Array[1];
		Data[1] = u8Array[2];
		Data[0] = u8Array[3];
	}
	else
	{
		Data[0] = u8Array[0];
		Data[1] = u8Array[1];
		Data[2] = u8Array[2];
		Data[3] = u8Array[3];
	}
	memcpy(&fdata, Data, 4);
	return fdata;
}

void float2uint8Array(uint8_t* u8Array, const float* fdata, const uint8_t Flip)
{
	uint8_t floatArray[4];
	*(float*)floatArray = *fdata;
	if (Flip)
	{
		u8Array[3] = floatArray[0];
		u8Array[2] = floatArray[1];
		u8Array[1] = floatArray[2];
		u8Array[0] = floatArray[3];
	}
	else
	{
		u8Array[0] = floatArray[0];
		u8Array[1] = floatArray[1];
		u8Array[2] = floatArray[2];
		u8Array[3] = floatArray[3];
	}
}
