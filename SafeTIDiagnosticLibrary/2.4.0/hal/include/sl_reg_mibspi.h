/** @file HL_reg_mibspi.h
*   @brief MIBSPI Register Layer Header File
*   @date 04.Oct.2013
*   @version 03.07.00
*   
*   This file contains:
*   - Definitions
*   - Types
*   - Interface Prototypes
*   .
*   which are relevant for the MIBSPI driver.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

#ifndef __SL_REG_MIBSPI_H__
#define __SL_REG_MIBSPI_H__

#include "sl_sys_common.h"
#include "sl_reg_gio.h"

/** @enum chipSelect
*   @brief Transfer Group Chip Select
*/
enum sl_mibspiChipSelect
{
    SL_CS_NONE = 0xFFU,
    SL_CS_0    = 0xFEU,
    SL_CS_1    = 0xFDU,
    SL_CS_2    = 0xFBU,
    SL_CS_3    = 0xF7U,
    SL_CS_4    = 0xEFU,
    SL_CS_5    = 0xDFU,
    SL_CS_6    = 0xBFU,
    SL_CS_7    = 0x7FU
};

/* Mibspi Register Frame Definition */
/** @struct mibspiBase
*   @brief MIBSPI Register Definition
*
*   This structure is used to access the MIBSPI module registers.
*/
/** @typedef mibspiBASE_t
*   @brief MIBSPI Register Frame Type Definition
*
*   This type is used to access the MIBSPI Registers.
*/
typedef volatile struct sl_mibspiBase
{
    uint32  GCR0;                 /**< 0x0000: Global Control 0 */
    uint32  GCR1;                 /**< 0x0004: Global Control 1 */	
    uint32  INT0;                 /**< 0x0008: Interrupt Register */	
    uint32  LVL;                  /**< 0x000C: Interrupt Level */
    uint32  FLG;                  /**< 0x0010: Interrupt flags */	
    uint32  PC0;                /**< 0x0014: Function Pin Enable */
    uint32  PC1;                /**< 0x0018: Pin Direction */
    uint32  PC2;                /**< 0x001C: Pin Input Latch */
    uint32  PC3;               /**< 0x0020: Pin Output Latch */
    uint32  PC4;                /**< 0x0024: Output Pin Set */
    uint32  PC5;                /**< 0x0028: Output Pin Clr */
    uint32  PC6;                /**< 0x002C: Open Drain Output Enable */
    uint32  PC7;                /**< 0x0030: Pullup/Pulldown Disable */
    uint32  PC8;                /**< 0x0034: Pullup/Pulldown Selection */
    uint32  DAT0;                 /**< 0x0038: Transmit Data */
    uint32  DAT1;                 /**< 0x003C: Transmit Data with Format and Chip Select */
    uint32  BUF;                  /**< 0x0040: Receive Buffer */
    uint32  EMU;                  /**< 0x0044: Emulation Receive Buffer */
    uint32  DELAY;                /**< 0x0048: Delays */
    uint32  DEF;                /**< 0x004C: Default Chip Select */
    uint32  FMT0;                 /**< 0x0050: Data Format 0 */
    uint32  FMT1;                 /**< 0x0054: Data Format 1 */
    uint32  FMT2;                 /**< 0x0058: Data Format 2 */
    uint32  FMT3;                 /**< 0x005C: Data Format 3 */
    uint32  INTVECT0;             /**< 0x0060: Interrupt Vector 0 */
    uint32  INTVECT1;             /**< 0x0064: Interrupt Vector 1 */
    uint32  SRSEL;                /**< 0x0068: Slew Rate Select */
    uint32  PMCTRL;               /**< 0x006C: Parallel Mode Control */
    uint32  MIBSPIE;              /**< 0x0070: Multi-buffer Mode Enable  */
    uint32  TGITENST;             /**< 0x0074: TG Interrupt Enable Set */
    uint32  TGITENCR;             /**< 0x0078: TG Interrupt Enable Clear */
    uint32  TGITLVST;             /**< 0x007C: Transfer Group Interrupt Level Set */
    uint32  TGITLVCR;             /**< 0x0080: Transfer Group Interrupt Level Clear */
    uint32  TGINTFLG;             /**< 0x0084: Transfer Group Interrupt Flag */	
    uint32    rsvd1[2U];           /**< 0x0088: Reserved */
    uint32  TICKCNT;              /**< 0x0090: Tick Counter */
    uint32  LTGPEND;              /**< 0x0094: Last TG End Pointer */
    uint32  TGCTRL[16U];          /**< 0x0098 - 0x00D4: Transfer Group Control */
    uint32  DMACTRL[8U];          /**< 0x00D8 - 0x00F4: DMA Control */
    uint32  DMACOUNT[8U];         /**< 0x00F8 - 0x0114: DMA Count */
    uint32  DMACNTLEN;            /**< 0x0118 - 0x0114: DMA Control length */
    uint32    rsvd2;              /**< 0x011C: Reserved */
    uint32  PAR_ECC_CTRL;         /**< 0x0120: Multi-buffer RAM Uncorrectable Parity Error Control */
    uint32  PAR_ECC_STAT;         /**< 0x0124: Multi-buffer RAM Uncorrectable Parity Error Status */
    uint32  UERRADDRRX;           /**< 0x0128: RXRAM Uncorrectable Parity Error Address */	
    uint32  UERRADDRTX;           /**< 0x012C: TXRAM Uncorrectable Parity Error Address */	
    uint32  RXOVRN_BUF_ADDR;      /**< 0x0130: RXRAM Overrun Buffer Address */
    uint32  IOLPKTSTCR;           /**< 0x0134: IO loopback */
    uint32  EXT_PRESCALE1;        /**< 0x0138: */
    uint32  EXT_PRESCALE2;        /**< 0x013C: */
    uint32  ECC_DIAG_CTRL;        /**< 0x0140: ECC Diagnostic Control register */
    uint32  ECC_DIAG_STAT;        /**< 0x0144: ECC Diagnostic Status register */
    uint32  SBERR_ADDRRX;          /**< 0x0148: Single Bit Error Address Register - RXRAM */
    uint32  SBERR_ADDRTX;          /**< 0x0152: Single Bit Error Address Register - TXRAM */
} sl_mibspiBASE_t;

/** @def mibspiREG1
*   @brief MIBSPI1 Register Frame Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi module registers.
*/
#define sl_mibspiREG1 ((sl_mibspiBASE_t *)0xFFF7F400U)


/** @def mibspiPORT1
*   @brief MIBSPI1 GIO Port Register Pointer
*
*   Pointer used by the GIO driver to access I/O PORT of MIBSPI1
*   (use the GIO drivers to access the port pins).
*/
#define sl_mibspiPORT1 ((sl_gioPORT_t *)0xFFF7F418U)

/** @def mibspiREG2
*   @brief MIBSPI2 Register Frame Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi module registers.
*/
#define sl_mibspiREG2 ((sl_mibspiBASE_t *)0xFFF7F600U)


/** @def mibspiPORT2
*   @brief MIBSPI2 GIO Port Register Pointer
*
*   Pointer used by the GIO driver to access I/O PORT of MIBSPI2
*   (use the GIO drivers to access the port pins).
*/
#define sl_mibspiPORT2 ((sl_gioPORT_t *)0xFFF7F618U)


/** @def mibspiREG3
*   @brief MIBSPI3 Register Frame Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi module registers.
*/
#define sl_mibspiREG3 ((sl_mibspiBASE_t *)0xFFF7F800U)


/** @def mibspiPORT3
*   @brief MIBSPI3 GIO Port Register Pointer
*
*   Pointer used by the GIO driver to access I/O PORT of MIBSPI3
*   (use the GIO drivers to access the port pins).
*/
#define sl_mibspiPORT3 ((sl_gioPORT_t *)0xFFF7F818U)

/** @def mibspiREG4
*   @brief MIBSPI4 Register Frame Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi module registers.
*/
#define sl_mibspiREG4 ((sl_mibspiBASE_t *)0xFFF7FA00U)


/** @def mibspiPORT4
*   @brief MIBSPI4 GIO Port Register Pointer
*
*   Pointer used by the GIO driver to access I/O PORT of MIBSPI4
*   (use the GIO drivers to access the port pins).
*/
#define sl_mibspiPORT4 ((sl_gioPORT_t *)0xFFF7FA18U)


/** @def mibspiREG5
*   @brief MIBSPI5 Register Frame Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi module registers.
*/
#define sl_mibspiREG5 ((sl_mibspiBASE_t *)0xFFF7FC00U)


/** @def mibspiPORT5
*   @brief MIBSPI5 GIO Port Register Pointer
*
*   Pointer used by the GIO driver to access I/O PORT of MIBSPI5
*   (use the GIO drivers to access the port pins).
*/
#define sl_mibspiPORT5 ((sl_gioPORT_t *)0xFFF7FC18U)


/** @struct mibspiRamBase
*   @brief MIBSPI Buffer RAM Definition
*
*   This structure is used to access the MIBSPI buffer memory.
*/
/** @typedef mibspiRAM_t
*   @brief MIBSPI RAM Type Definition
*
*   This type is used to access the MIBSPI RAM.
*/
typedef volatile struct sl_mibspiRamBase
{
    struct 
    {
#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
     uint16  data;     /**< tx buffer data    */   
     uint16  control;  /**< tx buffer control */  
#else
     uint16  control;  /**< tx buffer control */
     uint16  data;     /**< tx buffer data    */
#endif
    } tx[128];
    struct
    {
#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
     uint16 data;     /**< rx buffer data  */
	 uint16 flags;    /**< rx buffer flags */
#else
	 uint16 flags;    /**< rx buffer flags */
     uint16 data;     /**< rx buffer data  */
#endif
    } rx[128];
} sl_mibspiRAM_t;


/** @def mibspiRAM1
*   @brief MIBSPI1 Buffer RAM Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi buffer memory.
*/
#define sl_mibspiRAM1 ((sl_mibspiRAM_t *)0xFF0E0000U)

/** @def mibspiRAM2
*   @brief MIBSPI2 Buffer RAM Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi buffer memory.
*/
#define sl_mibspiRAM2 ((sl_mibspiRAM_t *)0xFF080000U)

/** @def mibspiRAM3
*   @brief MIBSPI3 Buffer RAM Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi buffer memory.
*/
#define sl_mibspiRAM3 ((sl_mibspiRAM_t *)0xFF0C0000U)

/** @def mibspiRAM4
*   @brief MIBSPI4 Buffer RAM Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi buffer memory.
*/
#define sl_mibspiRAM4 ((sl_mibspiRAM_t *)0xFF060000U)

/** @def mibspiRAM5
*   @brief MIBSPI5 Buffer RAM Pointer
*
*   This pointer is used by the MIBSPI driver to access the mibspi buffer memory.
*/
#define sl_mibspiRAM5 (((sl_mibspiRAM_t *)0xFF0A0000U))


#endif
