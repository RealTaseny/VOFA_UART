#ifndef BASE_TRANSFER_H__
#define BASE_TRANSFER_H__

#define USE_BIG_ENDIAN		0		//启用大端存储格式
#define USE_LITTLE_ENDIAN	1		//启用小端存储格式

#include <stdint.h>
#include <string.h>

float uint8Array2Float(const uint8_t* u8Array);
void float2uint8Array(uint8_t* u8Array, const float* fdata);

#endif
