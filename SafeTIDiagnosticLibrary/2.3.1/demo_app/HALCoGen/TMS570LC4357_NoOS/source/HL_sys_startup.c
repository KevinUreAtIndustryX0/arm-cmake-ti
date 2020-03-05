/** @file HL_sys_startup.c 
*   @brief Startup Source File
*   @date 02-Mar-2016
*   @version 04.05.02
*
*   This file contains:
*   - Include Files
*   - Type Definitions
*   - External Functions
*   - VIM RAM Setup
*   - Startup Routine
*   .
*   which are relevant for the Startup.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */


/* Include Files */

#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_sys_vim.h"
#include "HL_sys_core.h"
#include "HL_esm.h"
#include "HL_sys_mpu.h"

/* USER CODE BEGIN (1) */
#include "HL_sys_selftest.h"
#include "esm_application_callback.h"
#include "sl_priv.h"
/* USER CODE END */

/* USER CODE BEGIN (2) */
volatile uint64 crcAtInit_VIMRAM = 0;
volatile uint64 crcAtInit_FLASH = 0;
/* USER CODE END */

/* External Functions */

/*SAFETYMCUSW 218 S MR:20.2 <APPROVED> "Functions from library" */
extern void __TI_auto_init(void);
/*SAFETYMCUSW 354 S MR:NA <APPROVED> " Startup code(main should be declared by the user)" */
extern void main(void);
/*SAFETYMCUSW 122 S MR:20.11 <APPROVED> "Startup code(exit and abort need to be present)" */
/*SAFETYMCUSW 354 S MR:NA <APPROVED> " Startup code(Extern declaration present in the library)" */
extern void exit(int _status);


/* USER CODE BEGIN (3) */
void afterSTC(void);
SL_ResetReason     resetReason;        /* Reset reason */
/* USER CODE END */

/* Startup Routine */
void _c_int00(void);
/* USER CODE BEGIN (4) */
extern uint32                ulHighHandlerLoadStart;
extern uint32                ulHighHandlerLoadEnd;
extern uint32                ulHighHandlerSize;
extern uint32                ulHighHandlerStartAddr;
extern uint32                ulHighHandlerEndAddr;
SL_STC_Config                 stcSelfTestConfig;  /* STC Configuration */
/* USER CODE END */

#pragma CODE_STATE(_c_int00, 32)
#pragma INTERRUPT(_c_int00, RESET)
#pragma WEAK(_c_int00)

void _c_int00(void)
{

/* USER CODE BEGIN (5) */
#if 0
/* USER CODE END */

    /* Initialize Core Registers to avoid CCM Error */
    _coreInitRegisters_();

    /* Reset handler: the following instructions read from the system exception status register
     * to identify the cause of the CPU reset.
     */
    switch(getResetSource())
    {
        case POWERON_RESET:
        case DEBUG_RESET:
        case EXT_RESET:

/* USER CODE BEGIN (6) */
#endif

	/* Initialize Core Registers to avoid CCM Error */
	SL_Init_R5Registers();

	/* Initialize Stack Pointers
	 * The SL_Init_ResetReason API (and the correspondig HALCoGen getresetsource API) is a C function which uses stack.
	 * Stack must be setup correctly before calling this function.*/
    SL_Init_StackPointers();

    /* Enable CPU Event Export */
    /* This allows the CPU to signal any single-bit or double-bit errors detected
     * by its ECC logic for accesses to program flash or data RAM.
     */
    /* NOTE - needs to be called on every reset */
    _SL_Init_EnableEventExport();

	resetReason = SL_Init_ResetReason();
    switch(resetReason)
    {
        case RESET_TYPE_POWERON:
        case RESET_TYPE_DEBUG:
        case RESET_TYPE_EXTRST:
        case RESET_TYPE_EXTRST_NERROR:
#if 0
/* USER CODE END */

        /* Initialize L2RAM to avoid ECC errors right after power on */
        _memInit_();

/* USER CODE BEGIN (7) */
#endif
        /* Initialize L2RAM to avoid ECC errors right after power on
         * This function uses the system module's hardware for auto-initialization of memories and their
         * associated protection schemes. The CPU RAM is initialized by setting bit 0 of the MSIENA register.
         * Hence the value 0x1 passed to the function.
         * This function will initialize the entire CPU RAM and the corresponding ECC locations.
         */

        SL_Init_Memory(RAMTYPE_RAM);
/* USER CODE END */

/* USER CODE BEGIN (8) */
#if 0
/* USER CODE END */

        /* Initialize Stack Pointers */
        _coreInitStackPointer_();

/* USER CODE BEGIN (9) */
#endif 

#if 0
/* USER CODE END */

        /* Enable CPU Event Export */
        /* This allows the CPU to signal any single-bit or double-bit errors detected
         * by its ECC logic for accesses to program flash or data RAM.
         */
        _coreEnableEventBusExport_();

/* USER CODE BEGIN (10) */
#endif

        /* Enable CPU Event Export */
        /* This allows the CPU to signal any single-bit or double-bit errors detected
         * by its ECC logic for accesses to program flash or data RAM.
         */
        //_SL_Init_EnableEventExport();

/* USER CODE END */

        /* Check if there were ESM group3 errors during power-up.
         * These could occur during eFuse auto-load or during reads from flash OTP
         * during power-up. Device operation is not reliable and not recommended
         * in this case. */
        if ((esmREG->SR1[2]) != 0U)
        {
           while(1); //ESM group3 error
        }
	
        /* Initialize System - Clock, Flash settings with Efuse self check */
        systemInit();

/* USER CODE BEGIN (11) */
#if 0
/* USER CODE END */

        /* Enable IRQ offset via Vic controller */
        _coreEnableIrqVicOffset_();
            
        /* Initialize VIM table */
	    vimInit();

/* USER CODE BEGIN (12) */
/* USER CODE END */
        /* Configure system response to error conditions signaled to the ESM group1 */
        /* This function can be configured from the ESM tab of HALCoGen */
        esmInit();

/* USER CODE BEGIN (13) */
#endif

#if 0
/* USER CODE END */

        break;

        case OSC_FAILURE_RESET:
/* USER CODE BEGIN (14) */
#endif

        break;

        case RESET_TYPE_OSC_FAILURE:
#if 0
/* USER CODE END */
        break;
		
        case WATCHDOG_RESET:
/* USER CODE BEGIN (15) */
#endif

        break;

        case RESET_TYPE_WATCHDOG:
#if 0
/* USER CODE END */
        break;
    
        case CPU0_RESET:
		case CPU1_RESET:
/* USER CODE BEGIN (16) */
#endif

 break;

        case RESET_TYPE_CPU0:
		case RESET_TYPE_CPU1:

            /* Reset caused due to CPU reset.
            CPU reset can be caused by CPU self-test completion, or
            by toggling the "CPU RESET" bit of the CPU Reset Control Register. */

            /* reset could be caused by stcSelfCheck run or by an actual CPU self-test run */

            /* check if this was an stcSelfCheck run */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
            if ((stcREG->STCSCSCR & 0xFU) == 0xAU)
            {
                /* check if the self-test fail bit is set */
                if ((stcREG->STCGSTAT & 0x3U) != 0x3U)
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
            if ((stcREG1->STCSCSCR & 0xFU) == 0xAU)
            {
                /* check if the self-test fail bit is set */
                if ((stcREG1->STCGSTAT & 0x3U) != 0x3U)
#endif
                {
                    for(;;)
                    {

                    }
                }
                /* STC self-check has passed */
                else
                {
                    /* clear self-check mode */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
                    stcREG->STCSCSCR = 0x05U;

                    /* clear STC global status flags */
                    stcREG->STCGSTAT = 0x3U;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
                    stcREG1->STCSCSCR = 0x05U;

                    /* clear STC global status flags */
                    stcREG1->STCGSTAT = 0x3U;
#endif

                    /* clear ESM group1 channel 27 status flag */
                    esmREG->SR1[0U] = 0x08000000U;

                    /* Start CPU Self-Test */
                    /*there is no alternate usercode place holder to place the safety library api so this block of code can *
                    get overwritten on helcogen code regeneration*/
                    /*********************************************************************************************************/
                    /*cpuSelfTest(STC_INTERVAL, STC_MAX_TIMEOUT, TRUE);*/
                    stcSelfTestConfig.stcClockDiv        = 0;             /* STC Clock divider = 1 */
                    stcSelfTestConfig.intervalCount      = 1;             /* One interval only */
                    stcSelfTestConfig.restartInterval0   = TRUE;         /* Start from interval 0 */
                    stcSelfTestConfig.timeoutCounter     = 0xFFFFFFFF;     /* Timeout counter*/
                    _SL_HoldNClear_nError();

                    /* mask vim interrupts before running STC */
                    vimREG->REQMASKCLR0 = 0xFFFFFFFFu;
                    vimREG->REQMASKCLR1 = 0xFFFFFFFFu;
                    vimREG->REQMASKCLR2 = 0xFFFFFFFFu;
                    vimREG->REQMASKCLR3 = 0xFFFFFFFFu;

                    /* ensure no pending ESM GRP2 errors before running STC */
                    if(esmREG->SSR2 == 0u)
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
                    {SL_SelfTest_STC(STC_RUN, TRUE, &stcSelfTestConfig);}
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
                    {SL_SelfTest_STC(STC1_RUN, TRUE, &stcSelfTestConfig);}
#endif
                    /*********************************************************************************************************/
                }
            }
            /* CPU reset caused by CPU self-test completion */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
            else if ((stcREG->STCGSTAT & 0x1U) == 0x1U)
            {
                /* Self-Test Fail flag is set */
                if ((stcREG->STCGSTAT & 0x2U) == 0x2U)
                {
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
            else if ((stcREG1->STCGSTAT & 0x1U) == 0x1U)
            {
                /* Self-Test Fail flag is set */
                if ((stcREG1->STCGSTAT & 0x2U) == 0x2U)
                {
#endif
                    for(;;)
                    {

                    }
                }
                /* CPU self-test completed successfully */
                else
                {
                    /* clear STC global status flag */
                    stcREG1->STCGSTAT = 0x1U;

                    /* Continue start-up sequence after CPU STC completed */
                    afterSTC();
                }
            }
            /* CPU reset caused by software writing to CPU RESET bit */
            else
            {
                /* Add custom routine here to handle the case where software causes CPU reset */

/* USER CODE END */

/* USER CODE BEGIN (17) */
#if 0
/* USER CODE END */
        
		/* Initialize Stack Pointers */
        _coreInitStackPointer_();
		
/* USER CODE BEGIN (18) */
#endif
#if 0
/* USER CODE END */

        /* Enable CPU Event Export */
        /* This allows the CPU to signal any single-bit or double-bit errors detected
         * by its ECC logic for accesses to program flash or data RAM.
         */
        _coreEnableEventBusExport_();
		
/* USER CODE BEGIN (19) */
#endif

        /* Enable CPU Event Export */
        /* This allows the CPU to signal any single-bit or double-bit errors detected
         * by its ECC logic for accesses to program flash or data RAM.
         */
        _SL_Init_EnableEventExport();
#if 0
/* USER CODE END */
        break;
    
        case SW_RESET:
/* USER CODE BEGIN (20) */
#endif

        }

        break;

		case RESET_TYPE_SWRST:

        break;

        case RESET_TYPE_ICSTRST:

        	if(!SL_SelfTest_Status_MemIntrcntSelftest())
        	{	while(1);	} //Memory Interconnect Error
/* USER CODE END */
        break;
    
        default:
/* USER CODE BEGIN (21) */
/* USER CODE END */
        break;
    }

/* USER CODE BEGIN (22) */
	if(RESET_TYPE_DEBUG != resetReason)
	{
		if(RESET_TYPE_ICSTRST != resetReason)
		{
			/* Memory interconnect selftest */
			SL_SelfTest_MemoryInterconnect(MEMINTRCNT_SELFTEST);
		}

		/* Make sure that the CPU self-test controller can actually detect a fault inside CPU */
		stcSelfTestConfig.stcClockDiv        = 0;             /* STC Clock divider = 1 */
		stcSelfTestConfig.intervalCount     = 1;             /* One interval only */
		stcSelfTestConfig.restartInterval0     = TRUE;         /* Start from interval 0 */
		stcSelfTestConfig.timeoutCounter     = 0xFFFFFFFF;     /* Timeout counter*/
		_SL_HoldNClear_nError();

		/* mask vim interrupts before running STC */
		vimREG->REQMASKCLR0 = 0xFFFFFFFFu;
		vimREG->REQMASKCLR1 = 0xFFFFFFFFu;
		vimREG->REQMASKCLR2 = 0xFFFFFFFFu;
		vimREG->REQMASKCLR3 = 0xFFFFFFFFu;

		/* ensure no pending ESM GRP2 errors before running STC */
		if(esmREG->SSR2 == 0u)
		{
			SL_SelfTest_STC(STC1_COMPARE_SELFCHECK, TRUE, &stcSelfTestConfig);
		}
		else
		{
			while(1); //ESM Group2 error
		}

	}
	else
	{
		afterSTC();
	}

}

void afterSTC(void)
{

    /* Enable IRQ offset via Vic controller */
    _coreEnableIrqVicOffset_();

    /* Initialize VIM table */
    vimInit();

    /* Configure system response to error conditions signaled to the ESM group1 */
    /* This function can be configured from the ESM tab of HALCoGen */
    esmInit();
/* USER CODE END */

    _mpuInit_();
	
/* USER CODE BEGIN (23) */
/* USER CODE END */

    _cacheEnable_();

/* USER CODE BEGIN (24) */
/* USER CODE END */


/* USER CODE BEGIN (25) */
/* USER CODE END */

        /* initialize global variable and constructors */
    __TI_auto_init();
/* USER CODE BEGIN (26) */

    /* Initialize EPC */
    epcInit();

    /* enable DMA ECC */
    sl_dmaREG->DMAPCR |= (uint32)0xAU;

    /* initialise DMA RAM */
    SL_Init_Memory(RAMTYPE_DMA_RAM);

    /* Initialise ADC */
    adcInit();

    /* Initialise ADC SRAM */
    SL_Init_Memory(RAMTYPE_MIBADC1_RAM);
    SL_Init_Memory(RAMTYPE_MIBADC2_RAM);

    /* Initialise MIBSPI */
    mibspiInit();


    /* Initialise MIBSPI SRAM */
    SL_Init_Memory(RAMTYPE_MIBSPI1_RAM);
    SL_Init_Memory(RAMTYPE_MIBSPI2_RAM);
    SL_Init_Memory(RAMTYPE_MIBSPI3_RAM);
    SL_Init_Memory(RAMTYPE_MIBSPI4_RAM);
    SL_Init_Memory(RAMTYPE_MIBSPI5_RAM);

    /* Initialise SCI */
    sciInit();

    /* Initialise CAN */
    canInit();

    /* Initialise CAN SRAM */
    SL_Init_Memory(RAMTYPE_DCAN1_RAM);
    SL_Init_Memory(RAMTYPE_DCAN2_RAM);
    SL_Init_Memory(RAMTYPE_DCAN3_RAM);
    SL_Init_Memory(RAMTYPE_DCAN4_RAM);

    SL_ESM_Init(ESM_ApplicationCallback);
       _enable_interrupt_();

       uint32 i;
       uint32 size=(uint32)&ulHighHandlerSize;
       for(i=0;i<size;i++)
       {
           ((char *)&ulHighHandlerStartAddr)[i] =((char *)&ulHighHandlerLoadStart)[i];
       }

   	/* Boot time MIBSPI Analog Loopback Test */
   	SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI2);

   	SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI4);

   	SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI3);

   	SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI1);

   	SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI5);


   	//Boot time Software test of VIM functionality
       SL_SelfTest_VIM(VIM_SOFTWARE_TEST);

   	//Boot time Software test of DMA functionality
       SL_SelfTest_DMA(DMA_SOFTWARE_TEST);

       checkPLL1Slip();
       checkPLL2Slip();

/* USER CODE END */
    
        /* call the application */
/*SAFETYMCUSW 296 S MR:8.6 <APPROVED> "Startup code(library functions at block scope)" */
/*SAFETYMCUSW 326 S MR:8.2 <APPROVED> "Startup code(Declaration for main in library)" */
/*SAFETYMCUSW 60 D MR:8.8 <APPROVED> "Startup code(Declaration for main in library;Only doing an extern for the same)" */
    main();
/* USER CODE BEGIN (27) */
/* USER CODE END */
/*SAFETYMCUSW 122 S MR:20.11 <APPROVED> "Startup code(exit and abort need to be present)" */
    exit(0);


/* USER CODE BEGIN (28) */
/* USER CODE END */

}

/* USER CODE BEGIN (29) */
/* USER CODE END */



/* USER CODE BEGIN (30) */
/* USER CODE END */
