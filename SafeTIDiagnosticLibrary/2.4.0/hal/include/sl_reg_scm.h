/** @file sl_reg_scm.h
*   @brief SCM Register Layer Header File
*   @date
*   @version
*
*   This file contains:
*   - Definitions
*   - Types
*   .
*   which are relevant for the SCM driver.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

#ifndef __SL_REG_SCM_H__
#define __SL_REG_SCM_H__

#include "sl_sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN (0) */
/* USER CODE END */

/* SCM Register Frame Definition */
/** @struct scmBase
*   @brief SCM Base Register Definition
*
*   This structure is used to access the SCM module registers.
*/
/** @typedef scmBASE_t
*   @brief SCM Register Frame Type Definition
*
*   This type is used to access the SCM Registers.
*/
typedef volatile struct sl_scmBase
{
    uint32 SCMREVID;      /**< 0x0000: SCM REVID Register */
    uint32 SCMCNTRL;      /**< 0x0004: SCM Control Register */
    uint32 SCMTHRESHOLD;  /**< 0x0008: SCM Compare Threshold Counter Register   */
    uint32 rsvd1;         /**< 0x000C: Reserved */
    uint32 SCMIAERR0STAT; /**< 0x0010: SCM Initiator Error0 Status Register */
    uint32 SCMIAERR1STAT; /**< 0x0014: SCM Initiator Error1 Status Register */
    uint32 SCMIASTAT;     /**< 0x0018: SCM Initiator Active Status Register */
    uint32 rsvd2;         /**< 0x001C: Reserved */
    uint32 SCMTASTAT;     /**< 0x0020: SCM Target Active Status Register */
} sl_scmBASE_t;

#define sl_scmREG1   ((sl_scmBASE_t *)0xFFFF0A00U)

/* USER CODE BEGIN (1) */
/* USER CODE END */

/**@}*/
#ifdef __cplusplus
}
#endif

#endif

