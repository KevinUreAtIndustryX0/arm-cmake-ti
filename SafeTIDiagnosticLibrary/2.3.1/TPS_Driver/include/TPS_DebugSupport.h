/**
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION
 *
 *                 Property of Texas Instruments, Unauthorized reproduction and/or distribution
 *                 is strictly prohibited.  This product  is  protected  under  copyright  law
 *                 and  trade  secret law as an  unpublished work.
 *                 (C) Copyright Texas Instruments.  All rights reserved.
 *
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *  ------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         @file       Tps_DebugSupport.h
 *         @version    2.3.1
 *         @component  TPS
 *         @module     TPS Driver
 *         @generator  No Configuration
 *
 *         @brief      This files contains the Debug Support API prototypes.
 *         @details    The Tps_DebugSupport.h file provides the debug support API
 *                     prototypes.These API's should help the application\n
 *                     debug the end application /driver.
 *
 *---------------------------------------------------------------------------------------------------------------------
 * @author  Manoj
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
 |Version       | Date         | Author           |    Change ID       | Description                 |
 |------------: |:------------:|:----------------:|:------------------:|:----------------------------|
 |2.3.1         | 20Dec2012    | Manoj R          |    00000000000     | Initial version             |
 */
#ifndef __TPS_DEBUGSUPPORT__
#define __TPS_DEBUGSUPPORT__
/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "TPS_Types.h"
#include "TPS_Config.h"
#include <stdio.h>
#include <string.h>
/*********************************************************
 Defines / data types / structs / unions /enums
 **********************************************************/
/*********************************************************
 Function Prototypes
 **********************************************************/
/**
 * @brief  API to initialize the debug support in the TPS driver.
 *
 *
 * @description The API is used to set up the TPS driver for the debug support.
 *
 * @image html  TPS_DriverInit.jpg
 * @image rtf   TPS_DriverInit.jpg
 *
 * @param [in]   SendTPSDebugText  pointer to the Application supported API\n
 *               which receives the debug text from the TPS driver.
 *
 * Example Usage:
 * @code
 *   TPS_DebugInit(SendDebugTextTPS);
 * @endcode
 *
 * @entrymode   any,The debu support interface such as SCI has to be\n
 * initialized before the debug init is called.
 * @exitmode    any.
 *
 */
void TPS_DebugInit(SendDebugText SendTPSDebugText);
/**
 * @brief  The API is used flag the errors/debug messages for debug purposes.
 *
 *
 * @description The API is used flag the errors/debug messages for debug purposes.
 *
 * @image html  TPS_DriverInit.jpg
 * @image rtf   TPS_DriverInit.jpg
 *
 * @param [in]   severity  Severity ERROR or DEBUG of the debug message.
 * @param [in]   u8buftext NULL terminated text buffer containing user\n
 *               information.
 * @param [in]   u32value  Value printed along with the debug text.This\n
 *               arguement is ignored if severity is INFO
 * Example Usage:
 * @code
 * boolean blRetVal = TRUE;
 * TPS_SendDebugText(INFO,
 *                        (uint8*) "Initialization of the TPS driver", 0);
 * TPS_SendDebugText(DEBUG,
 *                        (uint8*) "TPS device is in diagnostic state", blRetVal);
 * @endcode
 *
 * @entrymode   any,The debug support interface such as SCI has to be\n
 * initialized before the debug init is called.
 * @exitmode    any.
 *
 *
 *
 */
boolean TPS_SendDebugText(TPS_DebugSeverity severity, const uint8* const u8buftext,
        uint32 u32value);
#endif

