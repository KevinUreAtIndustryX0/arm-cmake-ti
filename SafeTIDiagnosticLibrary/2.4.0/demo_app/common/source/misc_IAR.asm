;/*******************************************************************************
;**+--------------------------------------------------------------------------+**
;**|                            ****                                          |**
;**|                            ****                                          |**
;**|                            ******o***                                    |**
;**|                      ********_///_****                                   |**
;**|                      ***** /_//_/ ****                                   |**
;**|                       ** ** (__/ ****                                    |**
;**|                           *********                                      |**
;**|                            ****                                          |**
;**|                            ***                                           |**
;**|                                                                          |**
;**|         Copyright (c) 2012 Texas Instruments Incorporated                |**
;**|                        ALL RIGHTS RESERVED                               |**
;**|                                                                          |**
;**| Permission is hereby granted to licensees of Texas Instruments           |**
;**| Incorporated (TI) products to use this computer program for the sole     |**
;**| purpose of implementing a licensee product based on TI products.         |**
;**| No other rights to reproduce, use, or disseminate this computer          |**
;**| program, whether in part or in whole, are granted.                       |**
;**|                                                                          |**
;**| TI makes no representation or warranties with respect to the             |**
;**| performance of this computer program, and specifically disclaims         |**
;**| any responsibility for any damages, special or consequential,            |**
;**| connected with the use of this program.                                  |**
;**|                                                                          |**
;**+--------------------------------------------------------------------------+**
;*******************************************************************************/
;/**
; * @file        misc
; * @version     0.0.1
; * @brief       misc functions needed for testing
; *
; * @notes		1. All functions are redirected to test_sl section
; *
; * <\todo Detailed description goes here>
; */


    section .text:CODE
    arm

		public    _svc

_svc
		stmfd   sp!, {r12,lr}
		mrs     r12, spsr
        	ands    r12, r12, #0x20
        	ldrbne  r12, [lr, #-2]
        	ldrbeq  r12, [lr, #-4]
		ldr		r14,  table
		ldr     r12, [r14, r12, lsl #2]
		blx     r12
		ldmfd   sp!, {r12,pc}^

table
		dcd	jumpTable

jumpTable
		dcd	swiGetCurrentMode		; 0
		dcd   swiSwitchToMode			; 1



;-------------------------------------------------------------------------------



swiGetCurrentMode
		mrs		r12, spsr
		ands	r0, r12, #0x1F		; return value
		bx		r14



;-------------------------------------------------------------------------------



swiSwitchToMode
		mrs		r12, spsr
		bic		r12, r12, #0x1F
		orr		r12, r12, r0
		msr		spsr_c, r12
		bx		r14



;-------------------------------------------------------------------------------

	end

;-------------------------------------------------------------------------------



