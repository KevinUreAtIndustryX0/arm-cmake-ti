#ifndef MOCK_HL_GIO_H
#define MOCK_HL_GIO_H

#include "fff.h"
#include <stdint.h>
typedef uint32_t uint32;
typedef volatile struct gioPort
{
    uint32 DIR;    /**< 0x0000: Data Direction Register */
    uint32 DIN;    /**< 0x0004: Data Input Register */
    uint32 DOUT;   /**< 0x0008: Data Output Register */
    uint32 DSET;   /**< 0x000C: Data Output Set Register */
    uint32 DCLR;   /**< 0x0010: Data Output Clear Register */
    uint32 PDR;    /**< 0x0014: Open Drain Register */
    uint32 PULDIS; /**< 0x0018: Pullup Disable Register */
    uint32 PSL;    /**< 0x001C: Pull Up/Down Selection Register */
} gioPORT_t;
#define gioPORTB ((gioPORT_t *)0xFFF7BC54U)
DECLARE_FAKE_VOID_FUNC(gioSetBit, gioPORT_t *, uint32, uint32)
DECLARE_FAKE_VALUE_FUNC(uint32, gioGetBit, gioPORT_t *, uint32)
DECLARE_FAKE_VOID_FUNC(gioInit)
#endif //MOCK_HL_GIO_H
