#ifndef BASE_TRANSFER_H__
#define BASE_TRANSFER_H__

#include <stdint.h>
#include <string.h>

float uint8_tArray2float(uint8_t* u8Array, uint8_t Flip);
void  float2uint8_tArray(uint8_t* u8Array, float* fdata, uint8_t Flip);

#endif
