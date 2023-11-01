#ifndef FRAME_H
#define FRAME_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef int INT32;

#define CRC16_POLY 0x1021
#define EXT_DATA_MAX_LENGTH 256
#define EXT_FRAME_MAX_LENGTH 1024
#define COM_ZT1 0x5A
#define COM_ZT2 0x54
#define COM_ZW1 0x5A
#define COM_ZW2 0xFE
#define COM_TC 0x00
#define COM_ZWTC 0xAA

#ifdef __cplusplus
extern "C" {
#endif

INT32 COM_ParseExtFrame(uint8_t * pbBuf, int wBufLen);
INT32 COM_AssembleExtFrame(UINT8 * pbBuf, INT32 iBufLen);

#ifdef __cplusplus
}
#endif

#endif // FRAME_H
