#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#include "sl_types.h"
#include "sl_api.h"
#include "reg_sci.h"
#include "register_readback.h"
#include "esm_application_callback.h"
#include "sci.h"

#include<sl_priv.h>
#include "sys_selftest.h"

#if FUNCTION_PROFILING_ENABLED
#include <stdio.h>
#include <math.h>
#endif

#if FUNCTION_PROFILING_ENABLED
#pragma DATA_SECTION ( fptests , "PROFILE_DATA" );
FILE *fptests;
#pragma DATA_SECTION ( fppbisttests , "PROFILE_DATA" );
FILE *fppbisttests;


static inline uint8 *stringFromTestType(uint32 profile_testyype)
{
    static const uint8 *TestTypeString[] = {

    	    "SRAM_ECC_ERROR_FORCING_1BIT",            /**< BTCM SRAM 1-Bit ECC Error forcing tests(to be used with API @ref SL_SelfTest_SRAM)        */
    	    "SRAM_ECC_1BIT_FAULT_INJECTION",                /**< BTCM SRAM 1-Bit ECC Fault injection(to be used with API @ref SL_SelfTest_SRAM)            */
    	    "SRAM_ECC_ERROR_FORCING_2BIT",                /**< BTCM SRAM 2-Bit ECC Error forcing test(to be used with API @ref SL_SelfTest_SRAM)            */
    	    "SRAM_ECC_2BIT_FAULT_INJECT",                    /**< BTCM SRAM 2-Bit ECC Error fault inject    (to be used with API @ref SL_SelfTest_SRAM)        */
    	    "SRAM_PAR_ADDR_CTRL_SELF_TEST",                /**< BTCM SRAM Address & Control Parity self test(to be used with API @ref SL_SelfTest_SRAM)    */
    	    "SRAM_ECC_ERROR_PROFILING",                    /**< SRAM Single bit Error Profiling test(to be used with API @ref SL_SelfTest_SRAM)            */
    	    "SRAM_ECC_ERROR_PROFILING_FAULT_INJECT",        /**< SRAM Single bit Error Profiling fault injection(to be used with API @ref SL_SelfTest_SRAM)*/
    	    "SRAM_RADECODE_DIAGNOSTICS",                    /**< SRAM Redundant Address Decode self test(to be used with API @ref SL_SelfTest_SRAM)        */
    	    "SRAM_LIVELOCK_DIAGNOSTICS",					/**< SRAM Livelock test(to be used with API @ref SL_SelfTest_SRAM)        */
    	    "ADC_SELFTEST_ALL",							/**< ADC self test(to be used with API @ref SL_SelfTest_ADC)        */
    		"ADC_SRAM_PARITY_TEST",						/**< ADC SRAM Parity test(to be used with API @ref SL_SelfTest_ADC)        */
    	    "FEE_ECC_DATA_CORR_MODE",                        /**< ECC Data correction diagnostic mode.(to be used with API @ref SL_SelfTest_FEE)*/
    	    "FEE_ECC_SYN_REPORT_MODE",                    /**< ECC Syndrome reporting diagnostic mode(to be used with API @ref SL_SelfTest_FEE) */
    	    "FEE_ECC_MALFUNCTION_MODE1",                    /**< ECC Malfunction test mode 1 (same data mode)(to be used with API @ref SL_SelfTest_FEE)*/
    	    "FEE_ECC_MALFUNCTION_MODE2",                    /**< ECC malfunction test mode 2 (inverted data mode)(to be used with API @ref SL_SelfTest_FEE) */
    	    "FEE_ECC_ADDR_TAG_REG_MODE",                    /**< Address tag register diagnostic mode(to be used with API @ref SL_SelfTest_FEE) */
    	    "FEE_ECC_TEST_MODE_1BIT",                        /**< 1Bit ECC Data correction diagnostic mode(to be used with API @ref SL_SelfTest_FEE) */
    	    "FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT",        /**< Address tag register diagnostic mode(to be used with API @ref SL_SelfTest_FEE) */
    	    "FEE_ECC_TEST_MODE_2BIT",                        /**< 2Bit ECC Data correction diagnostic mode.Verifies whether the 2 bit ecc error is getting created in FEE properly*/
    	    "FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT",        /**< 2Bit ECC Fault inject creates a 2 bit ecc error and leaves the application to handle the scenario */

    	    "FLASH_ECC_ADDR_TAG_REG_MODE",                /**< Address tag register diagnostic mode(to be used with API @ref SL_SelfTest_Flash) */
    	    /*FLASH_ECC_PIPELINE_BUF_PAR_MODE,            < Pipeline buffer parity diagnostic mode <NOT IMPLEMENTED IN THIS RELEASE> */
    	    "FLASH_ADDRESS_ECC_SELF_TEST",                /**< Flash wrapper address ecc self test verifies whether the address ecc is working fine (to be used with API @ref SL_SelfTest_Flash)*/
    	    "FLASH_ADDRESS_ECC_FAULT_INJECT",                /**< Flash wrapper address ecc self test fault inject creaters an address ecc error and leaves it for the application to handle it(to be used with API @ref SL_SelfTest_Flash)*/
    	    "FLASH_ADDRESS_PARITY_SELF_TEST",             /**< Flash wrapper address parity self test.Verifies whether teh address parity feature is working fine*/
    	    "FLASH_ADDRESS_PARITY_FAULT_INJECT",          /**< Flash wrapper address parity self test fault inject creates an address parity error and leaves it for the application to handle it(to be used with API @ref SL_SelfTest_Flash)*/
    	    "FLASH_ECC_TEST_MODE_1BIT",                   /**< 1Bit ECC Data correction diagnostic mode(to be used with API @ref SL_SelfTest_Flash) */
    	    "FLASH_ECC_TEST_MODE_2BIT",                    /**< 2Bit ECC Data correction diagnostic mode(to be used with API @ref SL_SelfTest_Flash) */
    	    "FLASH_HARD_ERROR_LIVELOCK",
    	    "FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT",         /**< 2Bit ECC fault injection mode. No response handled. NOTE: Application must read from 0x20000000 to generate error(to be used with API @ref SL_SelfTest_Flash) */

    	    "PBIST_EXECUTE",                                 /**< PBIST self test mode .The RAM info register RINFOL and RINFOU are used to select the memories for test(to be used with API @ref SL_SelfTest_PBIST)*/
    	    "PBIST_EXECUTE_OVERRIDE",                        /**< PBIST test mode with ROM override.The memory information available from ROM will override the RAM selection from RAM info Registers (to be used with API @ref SL_SelfTest_PBIST)*/

    	    "STC_RUN",                                       /**< CCMR4 test using STC(to be used with API @ref SL_SelfTest_STC) */
    	    "STC_COMPARE_SELFCHECK",                         /**< STC self check diagnostic(to be used with API @ref SL_SelfTest_STC) */

    	    "PSCON_SELF_TEST",                               /**< PSCON self test using Compare match & mismatch tests (to be used with API @ref SL_SelfTest_PSCON)*/
    	    "PSCON_ERROR_FORCING",                           /**< PSCON Error-Forcing self test mode (to be used with API @ref SL_SelfTest_PSCON)*/
    	    "PSCON_ERROR_FORCING_FAULT_INJECT",               /**< PSCON Error-Forcing fault inject(to be used with API @ref SL_SelfTest_PSCON) */
    	    "PSCON_SELF_TEST_ERROR_FORCING",                /**< PSCON Self-Test Error forcing mode (to be used with API @ref SL_SelfTest_PSCON)*/
    	    "PSCON_SELF_TEST_ERROR_FORCING_FAULT_INJECT",    /**< PSCON Self-Test Error forcing fault inject(to be used with API @ref SL_SelfTest_PSCON) */
    	    "PSCON_PMA_TEST",                                /**< PSCON Privilege Mode access test - MUST BE RUN IN USER MODE (to be used with API @ref SL_SelfTest_PSCON)*/

    	    "EFUSE_SELF_TEST_AUTOLOAD",                      /**< EFuse Auto load self test (to be used with API @ref SL_SelfTest_EFuse)*/
    	    "EFUSE_SELF_TEST_ECC",                           /**< EFuse ECC Self test (to be used with API @ref SL_SelfTest_EFuse)*/
    	    "EFUSE_SELF_TEST_STUCK_AT_ZERO",                 /**< EFuse stuck-at-zero self test (to be used with API @ref SL_SelfTest_EFuse)*/
    	    "CCMR4F_SELF_TEST",                              /**< CCMR4F Self-test (to be used with API @ref SL_SelfTest_CCMR4F)*/
    	    "CCMR4F_ERROR_FORCING_TEST",                    /**< CCMR4F Error forcing test(to be used with API @ref SL_SelfTest_CCMR4F) */
    	    "CCMR4F_ERROR_FORCING_TEST_FAULT_INJECT",        /**< CCMR4F Error focing test, but does not clear the nERROR & ESM Interrupt (Callee function must clear these) (to be used with API @ref SL_SelfTest_CCMR4F)*/
    	    "CCMR4F_SELF_TEST_ERROR_FORCING",

			"L2INTERCONNECT_RESERVED_ACCESS",                  /** L2 interconnect  reserved location access to create error trap (to be used with API @ref SL_SelfTestL2L3Interconnect)*/
			"L2INTERCONNECT_UNPRIVELEGED_ACCESS",				/** L2 interconnect  unprivileged access to create error trap (to be used with API @ref SL_SelfTestL2L3Interconnect)*/
			"L3INTERCONNECT_RESERVED_ACCESS",                  /** L3 interconnect reserved location access to create error trap(to be used with API @ref SL_SelfTestL2L3Interconnect)*/
			"L3INTERCONNECT_UNPRIVELEGED_ACCESS",				/** L3 interconnect  unprivileged access to create error trap (to be used with API @ref SL_SelfTestL2L3Interconnect)*/
			"VIM_SRAM_PARITY_TEST",							/**< VIM SRAM Parity test(to be used with API @ref SL_SelfTest_VIM)        */
			"VIM_SOFTWARE_TEST",								/**< VIM Functionality test(to be used with API @ref SL_SelfTest_VIM)        */
		#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
			"DMA_SRAM_PARITY_TEST",					 	/**< DMA RAM SECDED Parity Errorforcing test(to be used with API@ref SL_SelfTest_DMA)*/
			"DMA_SOFTWARE_TEST",							/**< DMA Functionality test(to be used with API @ref SL_SelfTest_DMA)        */
		#endif
			"HET_SRAM_PARITY_TEST",						/**< HET SRAM Parity test(to be used with API @ref SL_SelfTest_HET)        */
			"HET_ANALOG_LOOPBACK_TEST",					/**< HET Analog Loopback test(to be used with API @ref SL_SelfTest_HET)        */
			"HTU_SRAM_PARITY_TEST",						/**< HTU SRAM Parity test(to be used with API @ref SL_SelfTest_HTU)        */
			"MIBSPI_SRAM_PARITY_TEST",					/**< MIBSPI SRAM Parity test(to be used with API @ref SL_SelfTest_MibSPI)        */
			"MIBSPI_ANALOG_LOOPBACK_TEST",				/**< MIBSPI Analog Loopback test(to be used with API @ref SL_SelfTest_MibSPI)        */
			"SPI_ANALOG_LOOPBACK_TEST",					/**< SPI Analog Loopback test(to be used with API @ref SL_SelfTest_SPI)        */
			"CAN_SRAM_PARITY_TEST",						/**< CAN SRAM Parity test(to be used with API @ref SL_SelfTest_CAN)        */
			"CAN_ANALOG_LOOPBACK_TEST",					/**< CAN Analog Loopback test(to be used with API @ref SL_SelfTest_CAN)        */
			"GIO_ANALOG_LOOPBACK_TEST",					/**< GIO Analog Loopback test(to be used with API @ref SL_SelfTest_GIO)        */
			"SCI_ANALOG_LOOPBACK_TEST",					/**< SCI Analog Loopback test(to be used with API @ref SL_SelfTest_SCI)        */
			"LIN_ANALOG_LOOPBACK_TEST"

};

    return (uint8*)TestTypeString[profile_testyype];
}

#ifndef defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
static inline uint8 *stringFromalgo(uint32 algo)
{
static const uint8 *algostring[]={"PBISTALGO_TRIPLE_READ_SLOW_READ",
		"PBISTALGO_TRIPLE_READ_FAST_READ",
		"PBISTALGO_MARCH13N_RED_2PORT   ",
		"PBISTALGO_MARCH13N_RED_1PORT   ",
		"PBISTALGO_DOWN1A_RED_2PORT     ",
		"PBISTALGO_DOWN1A_RED_1PORT     ",
		"PBISTALGO_MAPCOLUMN_2PORT      ",
		"PBISTALGO_MAPCOLUMN_1PORT      ",
		"PBISTALGO_PRECHARGE_2PORT      ",
		"PBISTALGO_PRECHARGE_1PORT      ",
		"PBISTALGO_DTXN2_2PORT          ",
		"PBISTALGO_DTXN2_1PORT          ",
		"PBISTALGO_PMOS_OPEN_2PORT      ",
		"PBISTALGO_PMOS_OPEN_1PORT      ",
		"PBISTALGO_PMOS_OPEN_SLICE1_2POR",
		"PBISTALGO_PMOS_OPEN_SLICE2_1POR",
		"PBISTALGO_FLIP10_2PORT         ",
		"PBISTALGO_FLIP10_1PORT         ",
		"PBISTALGO_IDDQ_2PORT           ",
		"PBISTALGO_IDDQ_1PORT           ",
		"PBISTALGO_RETENTION_2PORT      ",
		"PBISTALGO_RETENTION_1PORT      ",
		"PBISTALGO_IDDQ2_2PORT          ",
		"PBISTALGO_IDDQ2_1PORT          ",
		"PBISTALGO_RETENTION2_2PORT     ",
		"PBISTALGO_RETENTION2_1PORT     ",
		"PBISTALGO_IDDQROWSTRIPE_2PORT  ",
		"PBISTALGO_IDDQROWSTRIPE_1PORT  ",
		"PBISTALGO_IDDQROWSTRIPE2_2PORT ",
		"PBISTALGO_IDDQROWSTRIPE2_1PORT "};

return (uint8*)algostring[algo];
}

static inline uint8 *stringFromramgroup(uint32 ramgroup)
{
static const uint8 *ramgroupstring[]={

		"PBIST_RAMGROUP_01_PBIST_ROM",
		"PBIST_RAMGROUP_02_STC_ROM  ",
		"PBIST_RAMGROUP_03_DCAN1    ",
		"PBIST_RAMGROUP_04_DCAN2    ",
		"PBIST_RAMGROUP_05_DCAN3    ",
		"PBIST_RAMGROUP_06_ESRAM1   ",
		"PBIST_RAMGROUP_07_MIBSPI1  ",
		"PBIST_RAMGROUP_08_MIBSPI3  ",
		"PBIST_RAMGROUP_09_MIBSPI5  ",
		"PBIST_RAMGROUP_10_VIM      ",
		"PBIST_RAMGROUP_11_MIBADC1  ",
		"PBIST_RAMGROUP_12_DMA      ",
		"PBIST_RAMGROUP_13_N2HET1   ",
		"PBIST_RAMGROUP_14_HETTU1   ",
		"PBIST_RAMGROUP_15_RTP      ",
		"PBIST_RAMGROUP_16_FRAY     ",
		"PBIST_RAMGROUP_17_FRAY     ",
		"PBIST_RAMGROUP_18_MIBADC2  ",
		"PBIST_RAMGROUP_19_N2HET2   ",
		"PBIST_RAMGROUP_20_HETTU2   ",
		"PBIST_RAMGROUP_21_ESRAM5   ",
		"PBIST_RAMGROUP_22_ESRAM6   ",
		"PBIST_RAMGROUP_23_ETHERNET1",
		"PBIST_RAMGROUP_24_ETHERNET2",
		"PBIST_RAMGROUP_25_ETHERNET3",
		"PBIST_RAMGROUP_26_USB1     ",
		"PBIST_RAMGROUP_27_USB2     ",
		"PBIST_RAMGROUP_28_ESRAM8   ",
		"PBIST_RAMGROUP_29_RSVD     "
#endif
		};

return (uint8*)ramgroupstring[ramgroup];
}

#endif
/*common 2 port memories and algos*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
uint32 all2portmemories[] = {PBIST_RAMGROUP_03_DCAN1,
		PBIST_RAMGROUP_04_DCAN2,
		PBIST_RAMGROUP_05_DCAN3,
		PBIST_RAMGROUP_07_MIBSPI1,
		PBIST_RAMGROUP_08_MIBSPI3,
		PBIST_RAMGROUP_09_MIBSPI5,
		PBIST_RAMGROUP_11_MIBADC1,
		PBIST_RAMGROUP_12_DMA,
		PBIST_RAMGROUP_13_N2HET1,
		PBIST_RAMGROUP_14_HETTU1,
		PBIST_RAMGROUP_18_MIBADC2,
		PBIST_RAMGROUP_19_N2HET2,
		PBIST_RAMGROUP_20_HETTU2,
		PBIST_RAMGROUP_23_ETHERNET1,
		PBIST_RAMGROUP_24_ETHERNET2};
#endif
#if defined(_RM42x_) || defined(_TMS570LS04x_)
uint32 all2portmemories[] = {PBIST_RAMGROUP_03_DCAN1,
    	PBIST_RAMGROUP_04_DCAN2,
    	PBIST_RAMGROUP_07_MIBSPI1,
    	PBIST_RAMGROUP_11_MIBADC1,
    	PBIST_RAMGROUP_13_N2HET1,
    	PBIST_RAMGROUP_14_HETTU1};
#endif


uint32 all2portalgos[] = {PBISTALGO_MARCH13N_RED_2PORT,
						  PBISTALGO_DOWN1A_RED_2PORT,
						  PBISTALGO_MAPCOLUMN_2PORT,
						  PBISTALGO_PRECHARGE_2PORT,
						  PBISTALGO_DTXN2_2PORT,
						  PBISTALGO_FLIP10_2PORT,
						  PBISTALGO_IDDQ_2PORT,
						  PBISTALGO_RETENTION_2PORT,
						  PBISTALGO_IDDQ2_2PORT,
						  PBISTALGO_RETENTION2_2PORT,
						  PBISTALGO_IDDQROWSTRIPE_2PORT,
						  PBISTALGO_IDDQROWSTRIPE2_2PORT};

/*common 1 port memories and algos*/
uint32 all1portmemories[] = {PBIST_RAMGROUP_25_ETHERNET3};
uint32 all1portalgos[] = {PBISTALGO_MAPCOLUMN_1PORT,
		PBISTALGO_PRECHARGE_1PORT,
		PBISTALGO_DTXN2_1PORT,
		PBISTALGO_PMOS_OPEN_1PORT,
		PBISTALGO_FLIP10_1PORT,
		PBISTALGO_IDDQ_1PORT,
		PBISTALGO_RETENTION_1PORT,
		PBISTALGO_IDDQ2_1PORT,
		PBISTALGO_RETENTION2_1PORT,
		PBISTALGO_IDDQROWSTRIPE_1PORT,
		PBISTALGO_IDDQROWSTRIPE2_1PORT};

/*2 port special case 1*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
uint32 allopenslice2portmemories[] = {PBIST_RAMGROUP_12_DMA,PBIST_RAMGROUP_13_N2HET1,PBIST_RAMGROUP_19_N2HET2,PBIST_RAMGROUP_23_ETHERNET1};
#endif
#if defined(_RM42x_) || defined(_TMS570LS04x_)
uint32 allopenslice2portmemories[] = {PBIST_RAMGROUP_13_N2HET1};
#endif
uint32 allopenslicealgos[] = {PBISTALGO_PMOS_OP_SLICE1_2PORT,PBISTALGO_PMOS_OP_SLICE2_2PORT};

/*2 port special case 2*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
uint32 allpmosopen2portmemories[] = {PBIST_RAMGROUP_04_DCAN2,
		PBIST_RAMGROUP_05_DCAN3,
		PBIST_RAMGROUP_07_MIBSPI1,
		PBIST_RAMGROUP_08_MIBSPI3,
		PBIST_RAMGROUP_09_MIBSPI5,
		PBIST_RAMGROUP_11_MIBADC1,
		PBIST_RAMGROUP_14_HETTU1,
		PBIST_RAMGROUP_18_MIBADC2,
		PBIST_RAMGROUP_20_HETTU2,
		PBIST_RAMGROUP_24_ETHERNET2};
#endif
#if defined(_RM42x_) || defined(_TMS570LS04x_)
uint32 allpmosopen2portmemories[] = {PBIST_RAMGROUP_04_DCAN2,
		PBIST_RAMGROUP_07_MIBSPI1,
		PBIST_RAMGROUP_11_MIBADC1,
		PBIST_RAMGROUP_14_HETTU1};
#endif
uint32 allpmosopenalgos[] = {PBISTALGO_PMOS_OPEN_2PORT};


/*common ROM memories and algos*/
uint32 allROMmemories[] = {PBIST_RAMGROUP_01_PBIST_ROM,PBIST_RAMGROUP_02_STC_ROM};

uint32 allROMalgos[] = {PBISTALGO_TRIPLE_READ_SLOW_READ,PBISTALGO_TRIPLE_READ_FAST_READ};

uint32 initSelfTestPassCount = 0, initSelfTestFailCount = 0;
uint32 maintaskcount = 0;

#define INCREMENT_PASS_FAIL_COUNTER(x,y) \
		if ((ST_PASS == (x)) && (TRUE == (y))) {	\
			periodicSTPassCount++;      			\
		} else {                    				\
			periodicSTFailCount++;      			\
			while(1);								\
		}
                
uint32 periodicSTPassCount=0;
uint32 periodicSTFailCount=0;

boolean SendDebugText(uint8 *u8textbuf,uint32 u32textlength)
{
    boolean blRetVal = TRUE;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
    sciSend(sciREG, u32textlength, (uint8*)u8textbuf);
#endif
#if defined(_RM42x_) || defined(_TMS570LS04x_)
    sciSend(scilinREG, u32textlength, (uint8*)u8textbuf);
#endif
    return blRetVal;
}


void main()
{

	volatile boolean 	    retVal;             /* For function return values */
	SL_SelfTest_Result          failInfoFlash;      /* Flash Self test failure information */
	SL_SelfTest_Result          failInfoTCMRAM;     /* TCM RAM Failure  information */
	SL_PBIST_FailInfo           failInfoPBISTOthers;/* PBIST Failure information for non-TCM memories */
	SL_EFuse_Config             stConfigEFuse;      /* EFuse self test configuration */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
	SL_PSCON_FailInfo           failInfoPSCON;      /* PSCON failure information */
#endif

#if 0
	/* Used when CCMR4F tests are enabled */
	SL_CCMR4F_FailInfo	    failInfoCCMR4F;		/* CCMR4 Self Test fail info */
#endif
	uint32			    tempVal;
	uint32 i,j;
	extern uint64 crcAtInit_VIMRAM;
	extern uint64 crcAtInit_FLASH;
	extern uint32 ulFlashStartAddr;
	extern uint32 ulFlashEndAddr;

#if 0
	Reg_Read_Compare(VIM,REBASELINE);
	Reg_Read_Compare(RTI,REBASELINE);
#endif
	periodicSTPassCount=0;
	periodicSTFailCount=0;
        
        SL_ESM_Init(ESM_ApplicationCallback);

	#if FUNCTION_PROFILING_ENABLED
    SL_Init_Profiling();
#endif

    while(1) {
    	maintaskcount++;
        if(maintaskcount>10)
        {

		#ifdef __TI_COMPILER_VERSION__
        	__asm(" b #-8 ");
		#endif
		#ifdef __IAR_SYSTEMS_ICC__
		    asm(" B .-8");
		#endif

#if FUNCTION_PROFILING_ENABLED
        	fptests =  fopen("Profile_tests.txt","w+");
        	fppbisttests =  fopen("Profile_pbist_tests.txt","w+");
        	if ((fptests == NULL)||(fppbisttests == NULL)) {
        	  printf("Can't open files for writing!\n");
        	}
        	else
        	{

        		fputs("testype\tlastentrytick\tlast_exittick\terror_creation_tick\texecution_count\tcumilative_execution_tick\tesm_entrytick\tesm_exittick\taborthandler_entrytick\taborthandler_exittick\n",fptests);
        		for(i =0;i<(TESTTYPE_MAX - TESTTYPE_MIN);i++)
        		{
        			fprintf(fptests,"%s\t",stringFromTestType(i));
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].last_entrytick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].last_exittick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].error_creation_tick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].execution_count);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].cumilative_execution_tick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].esm_entrytick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].esm_exittick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].aborthandler_entrytick);
          			fprintf(fptests,"%d\n",SL_Profile_Struct[i].aborthandler_exittick);
        		}
        		fclose(fptests);



        		fputs("ramgroup\talgoinfo\tlast_entrytick\tlast_exittick\ttexecution_count\tcumilative_execution_tick\n",fppbisttests);
        		for(i =0;i<PBIST_MEM_MAX;i++)
        		{
            		for(j =0;j<PBIST_ALGO_MAX;j++)
            		{
            		fprintf(fppbisttests,"%s\t",stringFromramgroup(i));
        			fprintf(fppbisttests,"%s\t",stringFromalgo(j));
          			fprintf(fppbisttests,"%d\t",SL_Pbist_Profile_Struct[i][j].last_entrytick);
          			fprintf(fppbisttests,"%d\t",SL_Pbist_Profile_Struct[i][j].last_exittick);
          			fprintf(fppbisttests,"%d\t",SL_Pbist_Profile_Struct[i][j].execution_count);
          			fprintf(fppbisttests,"%d\n",SL_Pbist_Profile_Struct[i][j].cumilative_execution_tick);
           		}
        		}
        		fclose(fptests);


        	}
        	for(i=0;i<10000;i++); //wait for fprintf to complete
        	__asm(" b #-8 ");

#endif

        }
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
        retVal  = Reg_Read_Compare(PMM,COMPARE);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

        retVal  = Reg_Read_Compare(CCMR4,COMPARE);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal  = Reg_Read_Compare(EFC,COMPARE);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal  = Reg_Read_Compare(PBIST,COMPARE);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal  = Reg_Read_Compare(STC,COMPARE);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal  = Reg_Read_Compare(DCC1,COMPARE);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
		retVal  = Reg_Read_Compare(DCC2,COMPARE);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
		retVal  = Reg_Read_Compare(SYSTEM,COMPARE);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
		retVal  = Reg_Read_Compare(SRAM,COMPARE);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
		retVal  = Reg_Read_Compare(VIM,COMPARE);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);


		SL_ADC_Config adc_config;
		SL_ADC_Pinstatus pinStatus;

		/* ADC Self Test - ADC_PIN_GOOD on adcREG1 */
		adc_config.adc_channel = 8u;
		adc_config.adcbase = sl_adcREG1;
	    pinStatus = ADC_PIN_UNDETERMINED;
	    retVal = SL_SelfTest_ADC(ADC_SELFTEST_ALL, FALSE, &adc_config, &pinStatus);
	    INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
		/* ADC Self Test - ADC_PIN_GOOD on adcREG2 */
		adc_config.adc_channel = 8u;
		adc_config.adcbase = sl_adcREG2;
	    pinStatus = ADC_PIN_UNDETERMINED;
	    retVal = SL_SelfTest_ADC(ADC_SELFTEST_ALL, FALSE, &adc_config, &pinStatus);
	    INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

		/* ADC Self Test - ADC_PIN_open on adcREG1 */
		adc_config.adc_channel = 11u;
		adc_config.adcbase = sl_adcREG1;
	    pinStatus = ADC_PIN_UNDETERMINED;
	    retVal = SL_SelfTest_ADC(ADC_SELFTEST_ALL, FALSE, &adc_config, &pinStatus);
	    INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
		/* ADC Self Test - ADC_PIN_open on adcREG2 */
		adc_config.adc_channel = 12u;
		adc_config.adcbase = sl_adcREG2;
	    pinStatus = ADC_PIN_UNDETERMINED;
	    retVal = SL_SelfTest_ADC(ADC_SELFTEST_ALL, FALSE, &adc_config, &pinStatus);
	    INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

		/* ADC SRAM Parity Test */
		adc_config.adc_channel = 0u;
		adc_config.adcbase = sl_adcREG1;
		retVal = SL_SelfTest_ADC(ADC_SRAM_PARITY_TEST, FALSE, &adc_config, &pinStatus);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
		adc_config.adcbase = sl_adcREG2;
		retVal = SL_SelfTest_ADC(ADC_SRAM_PARITY_TEST, FALSE, &adc_config, &pinStatus);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
        /* running Perpheral SRAM Parity checks */
        retVal = SL_SelfTest_DMA(DMA_SRAM_PARITY_TEST);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

        retVal = SL_SelfTest_VIM(VIM_SRAM_PARITY_TEST);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

        retVal = SL_SelfTest_HET(HET_SRAM_PARITY_TEST,SL_HET1,0U,0U);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
        retVal = SL_SelfTest_HET(HET_SRAM_PARITY_TEST,SL_HET2,0U,0U);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

        retVal = SL_SelfTest_HTU(HTU_SRAM_PARITY_TEST,SL_HTU1);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
        retVal = SL_SelfTest_HTU(HTU_SRAM_PARITY_TEST,SL_HTU2);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

        retVal = SL_SelfTest_MibSPI(MIBSPI_SRAM_PARITY_TEST,SL_MIBSPI1);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
        retVal = SL_SelfTest_MibSPI(MIBSPI_SRAM_PARITY_TEST,SL_MIBSPI3);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal = SL_SelfTest_MibSPI(MIBSPI_SRAM_PARITY_TEST,SL_MIBSPI5);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
        retVal = SL_SelfTest_CAN(CAN_SRAM_PARITY_TEST,SL_DCAN1);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal = SL_SelfTest_CAN(CAN_SRAM_PARITY_TEST,SL_DCAN2);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
        retVal = SL_SelfTest_CAN(CAN_SRAM_PARITY_TEST,SL_DCAN3);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
        /* running Perpheral analog I/O loopback tests */
        retVal = SL_SelfTest_HET(HET_ANALOG_LOOPBACK_TEST,SL_HET1,10U,11U);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);


        retVal = SL_SelfTest_HET(HET_ANALOG_LOOPBACK_TEST,SL_HET2,5U,4U);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
        
#if defined(_RM42x_) || defined(_TMS570LS04x_)
    	retVal = SL_SelfTest_SPI(SPI_ANALOG_LOOPBACK_TEST,SL_SPI1);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
    	retVal = SL_SelfTest_SPI(SPI_ANALOG_LOOPBACK_TEST,SL_SPI3);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
       	retVal = SL_SelfTest_SPI(SPI_ANALOG_LOOPBACK_TEST,SL_SPI2);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
    	retVal = SL_SelfTest_SPI(SPI_ANALOG_LOOPBACK_TEST,SL_SPI4);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

    	retVal = SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI1);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
    	retVal = SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI3);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
    	retVal = SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI5);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

    	retVal = SL_SelfTest_GIO(GIO_ANALOG_LOOPBACK_TEST, SL_GIOPORTA, 5U);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
    	retVal = SL_SelfTest_GIO(GIO_ANALOG_LOOPBACK_TEST, SL_GIOPORTB, 1U);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
    	retVal = SL_SelfTest_SCI(SCI_ANALOG_LOOPBACK_TEST, SL_SCI1);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

#if 0
    	retVal = SL_SelfTest_LIN(LIN_ANALOG_LOOPBACK_TEST, SL_LIN1);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif

    	/*Periodic software test of VIM functionality*/
        retVal = SL_SelfTest_VIM(VIM_SOFTWARE_TEST);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
    	/*Periodic software test of DMA functionality*/
        retVal = SL_SelfTest_DMA(DMA_SOFTWARE_TEST);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
#if 0
    	/* Run SRAM_ECC_ERROR_PROFILING test on TCMRAM */
		retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_PROFILING, TRUE, &failInfoTCMRAM);
		INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);

		/* Run SRAM_ECC_ERROR_PROFILING_FAULT_INJECT test on TCMRAM  */
		retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_PROFILING_FAULT_INJECT, TRUE, &failInfoTCMRAM);
		INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);
#endif

		/* L2 Interconnect Error trapping - reserved location access */
		retVal = SL_SelfTestL2L3Interconnect(L2INTERCONNECT_RESERVED_ACCESS, (volatile uint32*)0U, 0U, 0U);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		/* L2 Interconnect Error trapping - unprivileged access */
		retVal = SL_SelfTestL2L3Interconnect(L2INTERCONNECT_UNPRIVELEGED_ACCESS, (volatile uint32*)0xFFF7A400U, &sl_pcrREG->PPROTSET2, 24u);
		if (FALSE == retVal) { /* Must fail, since unprivileged access tests cannot be run in privilege modes */
        	periodicSTPassCount++;
        } else {
        	periodicSTFailCount++;
        }

		/* L3 Interconnect Error trapping - reserved location access */
		retVal = SL_SelfTestL2L3Interconnect(L3INTERCONNECT_RESERVED_ACCESS, (volatile uint32*)0U, 0U, 0U);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		/* L3 Interconnect Error trapping - unprivileged access */
		retVal = SL_SelfTestL2L3Interconnect(L3INTERCONNECT_UNPRIVELEGED_ACCESS, (volatile uint32*)0xFFF7BC08U, &sl_pcrREG->PPROTSET2, 0u);
        if (FALSE == retVal) { /* Must fail, since unprivileged access tests cannot be run in privilege modes */
        	periodicSTPassCount++;
        } else {
        	periodicSTFailCount++;
        }

        /* Run 1Bit ECC test on TCM RAM */
        retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_1BIT, TRUE, &failInfoTCMRAM);
        INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);

        /* Run 2Bit ECC test on TCM RAM */
        retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_2BIT, TRUE, &failInfoTCMRAM);
    	INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);

    	/* Run Diagmode 1 */
		retVal = SL_SelfTest_FEE(FEE_ECC_DATA_CORR_MODE, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

		/* Run 1 bit selftest */
		retVal = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_1BIT, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

		/* Run 2 bit selftest */
		retVal = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_2BIT, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

		/* Run Diagmode 2 */
		retVal = SL_SelfTest_FEE(FEE_ECC_SYN_REPORT_MODE, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

		/* Run Diagmode 3 */
		retVal = SL_SelfTest_FEE(FEE_ECC_MALFUNCTION_MODE1, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

		/* Run Diagmode 4 */
		retVal = SL_SelfTest_FEE(FEE_ECC_MALFUNCTION_MODE2, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);


	/* Run Diagmode 5 */
#if 0
		retVal = SL_SelfTest_Flash(FLASH_ECC_ADDR_TAG_REG_MODE, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);
#endif

        /* Run 1Bit ECC test on Flash */
        retVal = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_1BIT, TRUE, &failInfoFlash);
        INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

        /* Run 2Bit ECC test on Flash */
		retVal = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_2BIT, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

		/* Run Parity Selftest on Flash */
		retVal = SL_SelfTest_Flash(FLASH_ADDRESS_PARITY_SELF_TEST, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

        /* Run PSCON self tests in sync mode */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
        retVal = SL_SelfTest_PSCON(PSCON_SELF_TEST, TRUE, &failInfoPSCON);
        INCREMENT_PASS_FAIL_COUNTER(failInfoPSCON.stResult, retVal);

        retVal = SL_SelfTest_PSCON(PSCON_ERROR_FORCING, TRUE, &failInfoPSCON);
        INCREMENT_PASS_FAIL_COUNTER(failInfoPSCON.stResult, retVal);

        retVal = SL_SelfTest_PSCON(PSCON_SELF_TEST_ERROR_FORCING, TRUE, &failInfoPSCON);
        INCREMENT_PASS_FAIL_COUNTER(failInfoPSCON.stResult, retVal);

        retVal = SL_SelfTest_PSCON(PSCON_PMA_TEST, TRUE, &failInfoPSCON);
        if (FALSE == retVal) { /* Must fail, since PMA tests cannot be run in privilege modes */
        	periodicSTPassCount++;
        } else {
        	periodicSTFailCount++;
        }
#endif
        /* Run EFuse self tests */
        stConfigEFuse.numPatterns      = 600u;
        stConfigEFuse.seedSignature    = 0x5362F97Fu;
        stConfigEFuse.failInfo.stResult= ST_FAIL;
        stConfigEFuse.failInfo.failInfo= EFUSE_ERROR_NONE;
        retVal = SL_SelfTest_EFuse(EFUSE_SELF_TEST_STUCK_AT_ZERO, TRUE, &stConfigEFuse);
        INCREMENT_PASS_FAIL_COUNTER(stConfigEFuse.failInfo.stResult, retVal);

        retVal = SL_SelfTest_EFuse(EFUSE_SELF_TEST_ECC, TRUE, &stConfigEFuse);
        while (TRUE != SL_SelfTest_Status_EFuse(&stConfigEFuse.failInfo));
        INCREMENT_PASS_FAIL_COUNTER(stConfigEFuse.failInfo.stResult, retVal);
        /* Run RAD Self tests on TCMRAM */
        retVal = SL_SelfTest_SRAM(SRAM_RADECODE_DIAGNOSTICS, TRUE, &failInfoTCMRAM);
        INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);
        /* Run SRAM Address & Control Parity self test */
        retVal = SL_SelfTest_SRAM(SRAM_PAR_ADDR_CTRL_SELF_TEST, TRUE, &failInfoTCMRAM);
    	INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);

    	/*running all posible 2 port algorithms on 2 port momories*/
    	for(i = 0;i < (sizeof(all2portmemories)/sizeof(uint32));i++)
    	{
    		for(j =0;j<(sizeof(all2portalgos)/sizeof(uint32));j++)
    		{
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling_PBIST(all2portmemories[i],all2portalgos[j]);
#endif

    	        retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
    	        							all2portmemories[i],
    	        							all2portalgos[j]);
    	        while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers));
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling_PBIST(all2portmemories[i],all2portalgos[j]);
#endif
    	        INCREMENT_PASS_FAIL_COUNTER(failInfoPBISTOthers.stResult, retVal);
    		}

    	}
    	/*running all posible 1 port algorithms on 1 port momories*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
    	for(i = 0;i < (sizeof(all1portmemories)/sizeof(uint32));i++)
    	{
    		for(j =0;j<(sizeof(all1portalgos)/sizeof(uint32));j++)
    		{
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling_PBIST(all1portmemories[i],all1portalgos[j]);
#endif
    	        retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
    	        							all1portmemories[i],
    	        							all1portalgos[j]);
    	        while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers));
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling_PBIST(all1portmemories[i],all1portalgos[j]);
#endif
    	        INCREMENT_PASS_FAIL_COUNTER(failInfoPBISTOthers.stResult, retVal);
    		}
    	}
#endif
    	/*running pbist on all ROM memories*/
    	for(i = 0;i < (sizeof(allROMmemories)/sizeof(uint32));i++)
    	{
    		for(j =0;j<(sizeof(allROMalgos)/sizeof(uint32));j++)
    		{
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling_PBIST(allROMmemories[i],allROMalgos[j]);
#endif
    	        retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
    	        							allROMmemories[i],
    	        							allROMalgos[j]);
    	        while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers));
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling_PBIST(allROMmemories[i],allROMalgos[j]);
#endif
    	        INCREMENT_PASS_FAIL_COUNTER(failInfoPBISTOthers.stResult, retVal);
    		}

    	}
    	/*running open slice algos  on memories*/
    	for(i = 0;i < (sizeof(allopenslice2portmemories)/sizeof(uint32));i++)
    	{
    		for(j =0;j<(sizeof(allopenslicealgos)/sizeof(uint32));j++)
    		{
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling_PBIST(allopenslice2portmemories[i],allopenslicealgos[j]);
#endif
    	        retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
    	        		allopenslice2portmemories[i],
    	        		allopenslicealgos[j]);
    	        while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers));
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling_PBIST(allopenslice2portmemories[i],allopenslicealgos[j]);
#endif
    	        INCREMENT_PASS_FAIL_COUNTER(failInfoPBISTOthers.stResult, retVal);
    		}

    	}

    	/*running pmos open algo on all  memories*/
    	for(i = 0;i < (sizeof(allpmosopen2portmemories)/sizeof(uint32));i++)
    	{
    		for(j =0;j<(sizeof(allpmosopenalgos)/sizeof(uint32));j++)
    		{
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling_PBIST(allpmosopen2portmemories[i],allpmosopenalgos[j]);
#endif
    	        retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
    	        		allpmosopen2portmemories[i],
    	        		allpmosopenalgos[j]);
    	        while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers));
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling_PBIST(allpmosopen2portmemories[i],allpmosopenalgos[j]);
#endif
    	        INCREMENT_PASS_FAIL_COUNTER(failInfoPBISTOthers.stResult, retVal);
    		}

    	}

    	/* re-init of peripheral rams required after pbist test (for further tests) */

    	/* disable htu */
        sl_htuREG1->GC &= (uint32)(~0x10000u);
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
        sl_htuREG2->GC &= (uint32)(~0x10000u);
#endif

        /* Enable parity on selected RAMs */
        enableParity();

#if !(defined(_RM42x_) || defined(_TMS570LS04x_))

        memoryInit( (uint32)((uint32)1U << 1U)    /* DMA RAM */
                  | (uint32)((uint32)1U << 5U)    /* CAN1 RAM */
                  | (uint32)((uint32)1U << 6U)    /* CAN2 RAM */
                  | (uint32)((uint32)1U << 10U)   /* CAN3 RAM */
                  | (uint32)((uint32)1U << 8U)    /* ADC1 RAM */
                  | (uint32)((uint32)1U << 14U)   /* ADC2 RAM */
                  | (uint32)((uint32)1U << 3U)    /* HET1 RAM */
                  | (uint32)((uint32)1U << 4U)    /* HTU1 RAM */
                  | (uint32)((uint32)1U << 15U)   /* HET2 RAM */
                  | (uint32)((uint32)1U << 16U)   /* HTU2 RAM */
                  );

#else
        memoryInit((uint32)((uint32)1U << 5U)    /* CAN1 RAM */
                  | (uint32)((uint32)1U << 6U)    /* CAN2 RAM */
                  | (uint32)((uint32)1U << 8U)    /* ADC1 RAM */
                  | (uint32)((uint32)1U << 3U)    /* HET1 RAM */
                  | (uint32)((uint32)1U << 4U)    /* HTU1 RAM */
                  );
#endif

        /*Enable HTU*/
        sl_htuREG1->GC |= (uint32)0x10000u;
#if !(defined(_RM42x_) || defined(_TMS570LS04x_))
        sl_htuREG2->GC |= (uint32)0x10000u;
#endif
    	mibspiInit();
    	canInit();
		spiInit();

        /* Run CRC Checks on VIM RAM */
        if (SL_CRC_Calculate(((uint64 *)0xFFF82000), 256) == crcAtInit_VIMRAM) {
        	periodicSTPassCount++;
        } else {
        	periodicSTFailCount++;
        }

        /* Run CRC Checks on Flash */
#ifdef __TI_COMPILER_VERSION__
        if (SL_CRC_Calculate((uint64 *)((uint32)&ulFlashStartAddr), ((((uint32)&ulFlashEndAddr)-((uint32)&ulFlashStartAddr)) >> 6)) == crcAtInit_FLASH) {
           	periodicSTPassCount++;
        } else {
        	periodicSTFailCount++;
        }
#endif
#ifdef __IAR_SYSTEMS_ICC__
        if (SL_CRC_Calculate((uint64 *)((uint32)ulFlashStartAddr), ((((uint32)ulFlashEndAddr)-((uint32)ulFlashStartAddr)) >> 6)) == crcAtInit_FLASH) {
        	periodicSTPassCount++;
		} else {
			periodicSTFailCount++;
		}
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
    	if(SL_SelfTest_PSCON(PSCON_ERROR_FORCING_FAULT_INJECT, TRUE, &failInfoPSCON))
    	{
    		/*check for the esm callback */
    		if(FALSE == pscon_errorforcing_app_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
    			periodicSTPassCount++;
    			pscon_errorforcing_app_callback = FALSE;
    		}
    	}
#endif

#if 0
    	/* WITH DEBUGGER CONNECTED following three tests WILL FAIL, since CCM is disabled in debug mode */
        retVal = SL_SelfTest_CCMR4F(CCMR4F_SELF_TEST, TRUE, &failInfoCCMR4F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR4F.stResult, retVal);

		retVal = SL_SelfTest_CCMR4F(CCMR4F_SELF_TEST_ERROR_FORCING, TRUE, &failInfoCCMR4F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR4F.stResult, retVal);

		retVal = SL_SelfTest_CCMR4F(CCMR4F_ERROR_FORCING_TEST, TRUE, &failInfoCCMR4F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR4F.stResult, retVal);

		if(SL_SelfTest_CCMR4F (CCMR4F_ERROR_FORCING_TEST_FAULT_INJECT, TRUE, &failInfoCCMR4F))
		{
			/*check for the esm callback */
			if(FALSE == cccmr4f_errorforcing_app_callback)
			{
			    periodicSTFailCount++;
			}
			else
			{
			    periodicSTPassCount++;
			    cccmr4f_errorforcing_app_callback = FALSE;
			}
		}

#endif


		uint32 flashRegBackup=flashWREG->FDIAGCTRL;
        /* Run CRC Checks on Flash, with Fault Injected */
        /* Note1: The fault is injected on the first DWORD Access to flash and this location must not contain 0. Else the CRC check will PASS */
        /* Note2: The flash start address must point to the slave access port of flash */
        retVal = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT, TRUE, &failInfoFlash);
#ifdef __TI_COMPILER_VERSION__
        if (SL_CRC_Calculate((uint64 *)(((uint32)&ulFlashStartAddr) | 0x20000000u), ((((uint32)&ulFlashEndAddr)-((uint32)&ulFlashStartAddr)) >> 6)) == crcAtInit_FLASH) {
        	periodicSTFailCount++;
		} else {
			periodicSTPassCount++;
		}
#endif
#ifdef __IAR_SYSTEMS_ICC__
        if (SL_CRC_Calculate((uint64 *)(((uint32)ulFlashStartAddr) | 0x20000000u), ((((uint32)ulFlashEndAddr)-((uint32)ulFlashStartAddr)) >> 6)) == crcAtInit_FLASH) {
        	periodicSTFailCount++;
		} else {
			periodicSTPassCount++;
		}
#endif
        /* Now clear the fault status */
        SL_Clear_nERROR(); /* Clear nError */
        flashWREG->FEDACSTATUS = 0x100u;
		esmREG->SR1[0] = 0x40;
		tempVal = flashWREG->FUNCERRADD;
		/* To remove compiler warning */
		tempVal = tempVal;
		flashWREG->FDIAGCTRL= flashRegBackup;

    }
}
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#include "sl_types.h"
#include "sl_api.h"
#include "register_readback.h"
#include "esm_application_callback.h"


#if FUNCTION_PROFILING_ENABLED
#include <stdio.h>
#include <math.h>
#endif

#if FUNCTION_PROFILING_ENABLED
#pragma DATA_SECTION ( fptests , "PROFILE_DATA" );
FILE *fptests;
#pragma DATA_SECTION ( fppbisttests , "PROFILE_DATA" );
FILE *fppbisttests;


static inline uint8 *stringFromTestType(uint32 profile_testyype)
{
    static const uint8 *TestTypeString[] = {

    	    "SRAM_ECC_ERROR_FORCING_1BIT",   /**< L2RAMW SECDED 1-Bit ECC Error
    	                                              forcing tests(to be used with API
    	                                              @ref SL_SelfTest_SRAM)
    	                                             */
    	    "SRAM_ECC_ERROR_FORCING_2BIT",            /**< L2RAMW SECDED 2-Bit ECC Error
    	                                              forcing test(to be used with API
    	                                              @ref SL_SelfTest_SRAM)
    	                                             */
    		"SRAM_RADECODE_DIAGNOSTICS",				/**< L2RAMW Redundant address decode
    													test(to be used with API
    	                                              @ref SL_SelfTest_SRAM)
    	                                             */

    	    "FLASH_ECC_TEST_MODE_1BIT",               /**< Test of the CPU SECDED for 1
    	                                              bit ECC Error on read from L2FMC
    	                                              (to be used with API @ref
    	                                              SL_SelfTest_Flash) */
    	    "FLASH_ECC_TEST_MODE_2BIT",               /**< Test of the CPU SECDED for 2
    	                                              bit ECC Error on read from L2FMC
    	                                              (to be used with API @ref
    	                                              SL_SelfTest_Flash) */

    	    "PBIST_EXECUTE",                          /**< PBIST self test mode .The RAM
    	                                              info register RINFOL and RINFOU
    	                                              are used to select the memories
    	                                              for test(to be used with API @ref
    	                                              SL_SelfTest_PBIST)*/
    	    "PBIST_EXECUTE_OVERRIDE",                 /**< PBIST test mode with ROM
    	                                              override.The memory information
    	                                              available from ROM will override
    	                                              the RAM selection from RAM info
    	                                              Registers (to be used with API
    	                                              @ref SL_SelfTest_PBIST)*/

    	    "EFUSE_SELF_TEST_AUTOLOAD",               /**< EFuse Auto load self test (to
    	                                              be used with API @ref
    	                                              SL_SelfTest_EFuse)*/
    	    "EFUSE_SELF_TEST_ECC",                    /**< EFuse ECC Self test (to be used
    	                                              with API @ref SL_SelfTest_EFuse)*/
    	    "EFUSE_SELF_TEST_STUCK_AT_ZERO",          /**< EFuse stuck-at-zero self test
    	                                              (to be used with API @ref
    	                                              SL_SelfTest_EFuse)*/

    	    "CCMR5F_CPUCOMP_SELF_TEST",                /**< CCMR5F CPU Comparator Self-test (to be used
    	                                              with API @ref
    	                                              SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_CPUCOMP_ERROR_FORCING_TEST",     /**< CCMR5F CPU Compare Error forcing test(to be
    	                                              used with API @ref
    	                                              SL_SelfTest_CCMR5F) */
    	    "CCMR5F_CPUCOMP_ERROR_FORCING_TEST_FAULT_INJECT", /**< CCMR5F CPU Compare Error Fault injection , but
    	                                              does not clear the nERROR & ESM
    	                                              Interrupt (Callee function must
    	                                              clear these) (to be used with API
    	                                              @ref SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_CPUCOMP_SELF_TEST_ERROR_FORCING", /**< CCMR5F CPU Comparator Self-test error forcing
    	                                              (to be used with API @ref
    	                                              SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_VIMCOMP_SELF_TEST",               /**< CCMR5F VIM Comparator Self-test (to be used
    	                                              with API @ref
    	                                              SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_VIMCOMP_ERROR_FORCING_TEST",      /**< CCMR5F VIM Compare Error forcing test(to be
    	                                              used with API @ref
    	                                              SL_SelfTest_CCMR5F) */
    	    "CCMR5F_VIMCOMP_ERROR_FORCING_TEST_FAULT_INJECT", /**< CCMR5F VIM Compare Error focing test, but
    	                                              does not clear the nERROR & ESM
    	                                              Interrupt (Callee function must
    	                                              clear these) (to be used with API
    	                                              @ref SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_VIMCOMP_SELF_TEST_ERROR_FORCING", /**< CCMR5F VIM Comparator Self-test error forcing
    	                                              (to be used with API @ref
    	                                              SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_PDCOMP_SELF_TEST",                /**< CCMR5F Power Domain Comparator Self-test (to be used
    	                                              with API @ref
    	                                              SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_PDCOMP_ERROR_FORCING_TEST",       /**< CCMR5F Power Domain Compare Error forcing test(to be
    	                                              used with API @ref
    	                                              SL_SelfTest_CCMR5F) */
    	    "CCMR5F_PDCOMP_ERROR_FORCING_TEST_FAULT_INJECT", /**< CCMR5F Power Domain
    	                                                     Compare Error focing test, but
    	                                              does not clear the nERROR & ESM
    	                                              Interrupt (Callee function must
    	                                              clear these) (to be used with API
    	                                              @ref SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_PDCOMP_SELF_TEST_ERROR_FORCING",  /**< CCMR5F Power Domain Comparator Self-test error forcing
    	                                              (to be used with API @ref
    	                                              SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_INMCOMP_SELF_TEST",               /**< CCMR5F Inactive CPU Monitor
    	                                              Comparator Self-test (to be used
    	                                              with API @ref
    	                                              SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_INMCOMP_ERROR_FORCING_TEST",      /**< CCMR5F Inactive CPU Monitor
    	                                              Compare Error forcing test(to be
    	                                              used with API @ref
    	                                              SL_SelfTest_CCMR5F) */
    	    "CCMR5F_INMCOMP_ERROR_FORCING_TEST_FAULT_INJECT", /**< CCMR5F Inactive CPU
    	                                                      Monitor Compare Error focing test, but
    	                                              does not clear the nERROR & ESM
    	                                              Interrupt (Callee function must
    	                                              clear these) (to be used with API
    	                                              @ref SL_SelfTest_CCMR5F)*/
    	    "CCMR5F_INMCOMP_SELF_TEST_ERROR_FORCING", /**< CCMR5F Inactive CPU Monitor
    	                                              Comparator Self-test error forcing
    	                                              (to be used with API @ref
    	                                              SL_SelfTest_CCMR5F)*/

    	    "STC1_RUN",                               /**< CPU test using STC1(to be used
    	                                              with API @ref SL_SelfTest_STC) */
    	    "STC1_COMPARE_SELFCHECK",                 /**< STC1 self check diagnostic(to be
    	                                              used with API @ref
    	                                              SL_SelfTest_STC) */
    	    "STC2_RUN",                               /**< */
    	    "STC2_COMPARE_SELFCHECK",                 /**< */

    	    "DMA_ECC_TEST_MODE_1BIT",                 /**< DMA RAM SECDED 1-Bit ECC Error
    	                                              forcing tests(to be used with API
    	                                              @ref SL_SelfTest_DMA)
    	                                             */

    	    "DMA_ECC_TEST_MODE_2BIT",                 /**< DMA RAM SECDED 2-Bit ECC Error
    	                                              forcing tests(to be used with API
    	                                              @ref SL_SelfTest_DMA)
    	                                             */
    	    "DMA_ECC_TEST_MODE_1BIT_FAULT_INJECT",    /**< DMA RAM SECDED 1-Bit ECC Error
    	                                              Fault injection(to be used with API
    	                                              @ref SL_SelfTest_DMA)
    	                                             */
    	    "DMA_ECC_TEST_MODE_2BIT_FAULT_INJECT",     /**< DMA RAM SECDED 2-Bit ECC Error
    	                                              Fault injection(to be used with API
    	                                              @ref SL_SelfTest_DMA)
    	                                             */
    		"DMA_SOFTWARE_TEST",						/**< DMA Periodic software test
    													(to be used with API
    	                                              @ref SL_SelfTest_DMA)
    	                                             */
    	    "CAN_ECC_TEST_MODE_1BIT",                  /**< CAN RAM SECDED 1-Bit ECC Error
    	                                              forcing tests(to be used with API
    	                                              @ref SL_SelfTest_CAN)
    	                                             */
    	    "CAN_ECC_TEST_MODE_2BIT",                  /**< CAN RAM SECDED 2-Bit ECC Error
    	                                              forcing tests(to be used with API
    	                                              @ref SL_SelfTest_CAN)
    	                                             */
    	    "CAN_ECC_TEST_MODE_1BIT_FAULT_INJECT",     /**< CAN RAM SECDED 1-Bit ECC Error
    	                                              Fault injection (to be used with API
    	                                              @ref SL_SelfTest_CAN)
    	                                             */
    	    "CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT",     /**< CAN RAM SECDED 2-Bit ECC Error
    	                                              Fault injection (to be used with API
    	                                              @ref SL_SelfTest_CAN)
    	                                             */

    	    "MIBSPI_ECC_TEST_MODE_1BIT",               /**< MibSPI RAM SECDED 1-Bit ECC Error
    	                                              forcing tests (to be used with API
    	                                              @ref SL_SelfTest_MibSPI)
    	                                             */
    	    "MIBSPI_ECC_TEST_MODE_2BIT",               /**< MibSPI RAM SECDED 2-Bit ECC Error
    	                                              forcing tests (to be used with API
    	                                              @ref SL_SelfTest_MibSPI)
    	                                             */
    	    "MIBSPI_ECC_TEST_MODE_1BIT_FAULT_INJECT",  /**< MibSPI RAM SECDED 1-Bit ECC Error
    	                                              Fault injection(to be used with API
    	                                              @ref SL_SelfTest_MibSPI)
    	                                             */
    	    "MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT",   /**< MibSPI RAM SECDED 2-Bit ECC Error
    	                                              Fault injection (to be used with API
    	                                              @ref SL_SelfTest_MibSPI)
    	                                             */
    		"MIBSPI_ANALOG_LOOPBACK_TEST",			/**< MibSPI Analog loopback test
    		 	 	 	 	 	 	 	 	 	 	 	 (to be used with API
    	                                              @ref SL_SelfTest_MibSPI)
    	                                             */

    	    "PSCON_SELF_TEST",                           /**< PSCON self test using Compare match
    	    											& mismatch tests (to be used with API @ref
    	    											SL_SelfTest_PSCON)
    	    											*/
    	    "PSCON_ERROR_FORCING",                           /**< PSCON Error-Forcing self test mode (to be used with API @ref SL_SelfTest_PSCON)*/
    	    "PSCON_ERROR_FORCING_FAULT_INJECT",               /**< PSCON Error-Forcing fault inject(to be used with API @ref SL_SelfTest_PSCON) */
    	    "PSCON_SELF_TEST_ERROR_FORCING",                /**< PSCON Self-Test Error forcing mode (to be used with API @ref SL_SelfTest_PSCON)*/
    	    "PSCON_SELF_TEST_ERROR_FORCING_FAULT_INJECT",    /**< PSCON Self-Test Error forcing fault inject(to be used with API @ref SL_SelfTest_PSCON) */
    	    "PSCON_PMA_TEST",

    		"MEMINTRCNT_RESERVED_ACCESS",					/**< Memory Interconnect test of accessing a
    													reserved location on memory interconnect
    													(to be used with API @ref
    													SL_SelfTest_MemoryInterconnect)
    													*/
    		"MEMINTRCNT_SELFTEST",						/**< Memory Interconnect selftest
    													(to be used with API @ref
    													SL_SelfTest_MemoryInterconnect)
    													*/
    		"MAINPERIPHINTRCNT_RESERVED_ACCESS",			/**< Main Perpheral Interconnect test of accessing a
    													reserved location on memory interconnect
    													(to be used with API @ref
    													SL_SelfTest_MemoryInterconnect)
    													*/

    		"PERIPHSEGINTRCNT_RESERVED_ACCESS",			/**< Peripheral Segment Interconnect test of accessing a
    													reserved location on memory interconnect
    													(to be used with API @ref
    													SL_SelfTest_MemoryInterconnect)
    													*/

    		"PERIPHSEGINTRCNT_UNPRIVELEGED_ACCESS",		/**< Peripheral Segment Interconnect test of accessing a
    													protected location on memory interconnect
    													(to be used with API @ref
    													SL_SelfTest_MemoryInterconnect)*/

    		"ADC_SELFTEST_ALL",

    		"ADC_SRAM_PARITY_TEST",						/**< Parity test on ADC SRAM
    													(to be used with API @ref
    													SL_SelfTest_ADC)*/

    	    "VIM_SOFTWARE_TEST"							/**< VIM Periodic software test
    													(to be used with API
    	                                                @ref SL_SelfTest_VIM)
    	                                                */
};

    return (uint8*)TestTypeString[profile_testyype];
}

static inline uint8 *stringFromalgo(uint32 algo)
{
static const uint8 *algostring[]={

 "PBISTALGO_TRIPLE_READ_SLOW_READ	",
 "PBISTALGO_TRIPLE_READ_FAST_READ   ",
 "PBISTALGO_MARCH13N_2PORT          ",
 "PBISTALGO_MARCH13N_1PORT          ",
 "PBISTALGO_DOWN2_1PORT             ",
 "PBISTALGO_DOWN2_2PORT             ",
 "PBISTALGO_MARCH_DIS_INC_1PORT     ",
 "PBISTALGO_MARCH_DIS_INC_2PORT     ",
 "PBISTALGO_MARCH_DIS_DEC_1PORT     ",
 "PBISTALGO_MARCH_DIS_DEC_2PORT     ",
 "PBISTALGO_MARCH_DIS_INC_1PORT_1   ",
 "PBISTALGO_MARCH_DIS_INC_2PORT_1   ",
 "PBISTALGO_MARCH_DIS_DEC_1PORT_1   ",
 "PBISTALGO_MARCH_DIS_DEC_2PORT_1   ",
 "PBISTALGO_MAP_COLUMN_1PORT        ",
 "PBISTALGO_MAP_COLUMN_2PORT        ",
 "PBISTALGO_PRECHARGE_1PORT         ",
 "PBISTALGO_PRECHARGE_2PORT         ",
 "PBISTALGO_FLIP10_1PORT            ",
 "PBISTALGO_FLIP10_2PORT            ",
 "PBISTALGO_DTXN2_1PORT             ",
 "PBISTALGO_DTXN2_2PORT             ",
 "PBISTALGO_PMOS_OP_SLICE1_1PORT    ",
 "PBISTALGO_PMOS_OP_SLICE1_2PORT    ",
 "PBISTALGO_PMOS_OP_SLICE2_2PORT	"

};

return (uint8*)algostring[algo];
}

static inline uint8 *stringFromramgroup(uint32 ramgroup)
{
static const uint8 *ramgroupstring[]={

    "PBIST_RAMGROUP_02_STC1_1_ROM_R5		",
    "PBIST_RAMGROUP_03_STC1_2_ROM_R5        ",
    "PBIST_RAMGROUP_04_STC2_ROM_NHET        ",
    "PBIST_RAMGROUP_05_AWM1                 ",
    "PBIST_RAMGROUP_06_DCAN1                ",
    "PBIST_RAMGROUP_07_DCAN2                ",
    "PBIST_RAMGROUP_08_DMA                  ",
    "PBIST_RAMGROUP_09_HTU1                 ",
    "PBIST_RAMGROUP_10_MIBSPI1              ",
    "PBIST_RAMGROUP_11_MIBSPI2              ",
    "PBIST_RAMGROUP_12_MIBSPI3              ",
    "PBIST_RAMGROUP_13_NHET1                ",
    "PBIST_RAMGROUP_14_VIM                  ",
    "PBIST_RAMGROUP_15_RSVD                 ",
    "PBIST_RAMGROUP_16_RTP                  ",
    "PBIST_RAMGROUP_17_ATB                  ",
    "PBIST_RAMGROUP_18_AWM2                 ",
    "PBIST_RAMGROUP_19_DCAN3                ",
    "PBIST_RAMGROUP_20_DCAN4                ",
    "PBIST_RAMGROUP_21_HTU2                 ",
    "PBIST_RAMGROUP_22_MIBSPI4              ",
    "PBIST_RAMGROUP_23_MIBSPI5              ",
    "PBIST_RAMGROUP_24_NHET2                ",
    "PBIST_RAMGROUP_25_FTU                  ",
    "PBIST_RAMGROUP_26_FRAY_INBUF_OUTBUF    ",
    "PBIST_RAMGROUP_27_CPGMAC_STATE_RXADDR  ",
    "PBIST_RAMGROUP_28_CPGMAC_STAT_FIFO     ",
    "PBIST_RAMGROUP_29_L2RAMW               ",
    "PBIST_RAMGROUP_30_L2RAMW               ",
    "PBIST_RAMGROUP_31_R5_ICACHE            ",
    "PBIST_RAMGROUP_32_R5DCACHE             ",
    "PBIST_RAMGROUP_33_RSVD                 ",
    "PBIST_RAMGROUP_34_RSVD                 ",
    "PBIST_RAMGROUP_35_FRAY_TRBUF_MSGRAM    ",
    "PBIST_RAMGROUP_36_CPGMAC_CPPI          ",
    "PBIST_RAMGROUP_37_R5_DCACHE_DIRTY      ",
    "PBIST_RAMGROUP_38_RSVD 				"

};

return (uint8*)ramgroupstring[ramgroup];
}
#endif

/*common 2 port memories and algos*/
uint64 all2portmemories[] = {PBIST_RAMGROUP_05_AWM1,
		PBIST_RAMGROUP_06_DCAN1,
		PBIST_RAMGROUP_07_DCAN2,
		PBIST_RAMGROUP_08_DMA,
		PBIST_RAMGROUP_09_HTU1,
		PBIST_RAMGROUP_10_MIBSPI1,
		PBIST_RAMGROUP_11_MIBSPI2,
		PBIST_RAMGROUP_12_MIBSPI3,
		PBIST_RAMGROUP_13_NHET1,
		PBIST_RAMGROUP_14_VIM,
		PBIST_RAMGROUP_16_RTP,
		PBIST_RAMGROUP_17_ATB,
		PBIST_RAMGROUP_18_AWM2,
		PBIST_RAMGROUP_19_DCAN3,
		PBIST_RAMGROUP_20_DCAN4,
		PBIST_RAMGROUP_21_HTU2,
		PBIST_RAMGROUP_22_MIBSPI4,
		PBIST_RAMGROUP_23_MIBSPI5,
		PBIST_RAMGROUP_24_NHET2,
#if defined(_TMS570LC43x_)
		PBIST_RAMGROUP_26_FRAY_INBUF_OUTBUF,
#endif
		PBIST_RAMGROUP_27_CPGMAC_STATE_RXADDR,
		PBIST_RAMGROUP_28_CPGMAC_STAT_FIFO,
		};


/* nERROR with PBIST_RAMGROUP_29_L2RAMW (PBIST_RAMGROUP_30_L2RAMW) 
 * Running PBIST on Cache memories need special considerations to be taken care
 * of in application. */
uint64 all1portmemories[] = {
#if defined(_TMS570LC43x_)
		PBIST_RAMGROUP_35_FRAY_TRBUF_MSGRAM,
#endif
		PBIST_RAMGROUP_36_CPGMAC_CPPI};


/*common ROM memories and algos*/

uint64 allROMmemories[] = {PBIST_RAMGROUP_01_PBIST_ROM,PBIST_RAMGROUP_02_STC1_1_ROM_R5,PBIST_RAMGROUP_03_STC1_2_ROM_R5, PBIST_RAMGROUP_04_STC2_ROM_NHET};
uint32 allROMalgos[] = {PBISTALGO_TRIPLE_READ_SLOW_READ,PBISTALGO_TRIPLE_READ_FAST_READ};


uint32 initSelfTestPassCount = 0, initSelfTestFailCount = 0;
uint32 maintaskcount = 0;

#define INCREMENT_PASS_FAIL_COUNTER(x,y) 			\
		if ((ST_PASS == (x)) && (TRUE == (y))) {	\
			periodicSTPassCount++;      			\
		} else {                    				\
			periodicSTFailCount++; 					\
		}
uint32 periodicSTPassCount=0, periodicSTFailCount=0;

boolean SendDebugText(uint8 *u8textbuf,uint32 u32textlength)
{
    boolean blRetVal = TRUE;
    sciSend(sciREG2, u32textlength, (uint8*)u8textbuf);
    return blRetVal;
}



void main()
{
	volatile boolean 	    retVal;             /* For function return values */
	SL_SelfTest_Result          failInfoFlash;      /* Flash Self test failure information */
	SL_SelfTest_Result          failInfoTCMRAM;     /* TCM RAM Failure  information */
	SL_PBIST_FailInfo           failInfoPBISTOthers;/* PBIST Failure information for non-TCM memories */
	SL_EFuse_Config             stConfigEFuse;      /* EFuse self test configuration */
	SL_CCMR5F_FailInfo	    failInfoCCMR5F;	/* CCMR5 Self Test fail info */
	SL_PSCON_FailInfo           failInfoPSCON;      /* PSCON failure information */

	uint32 i,j;
	extern uint64 crcAtInit_VIMRAM;
	extern uint64 crcAtInit_FLASH;
#ifdef __TI_COMPILER_VERSION__
	extern uint32 ulFlashStartAddr;
	extern uint32 ulFlashEndAddr;
#endif
#ifdef __IAR_SYSTEMS_ICC__
#pragma section=".intvecs"
#pragma section=".rodata"
	uint32 ulFlashStartAddr = (uint32) __section_begin(".intvecs"); /* Used for Flash CRC Calculation */
	uint32 ulFlashEndAddr = (uint32) __section_end(".rodata");       /* Used for Flash CRC Calculation */
#endif

	periodicSTPassCount=0;
	periodicSTFailCount=0;



#if FUNCTION_PROFILING_ENABLED
    SL_Init_Profiling();
#endif

    while(1) {
    	maintaskcount++;
        if(maintaskcount>10)
        {
#ifdef __TI_COMPILER_VERSION__
        	__asm(" b #-8 ");
#endif
#ifdef __IAR_SYSTEMS_ICC__
        	asm(" B .");
#endif

#if FUNCTION_PROFILING_ENABLED
        	fptests =  fopen("Profile_tests.txt","w+");
        	fppbisttests =  fopen("Profile_pbist_tests.txt","w+");
        	if ((fptests == NULL)||(fppbisttests == NULL)) {
        	  printf("Can't open files for writing!\n");
        	}
        	else
        	{
            	  printf("Hi!\n");

        		fputs("testype\tlastentrytick\tlast_exittick\terror_creation_tick\texecution_count\tcumilative_execution_tick\tesm_entrytick\tesm_exittick\taborthandler_entrytick\taborthandler_exittick\n",fptests);

        		for(i =0;i<(TESTTYPE_MAX - TESTTYPE_MIN);i++)
        		{

        			fprintf(fptests,"%s\t",stringFromTestType(i));
        			fprintf(fptests,"%d\t",SL_Profile_Struct[i].last_entrytick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].last_exittick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].error_creation_tick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].execution_count);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].cumilative_execution_tick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].esm_entrytick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].esm_exittick);
          			fprintf(fptests,"%d\t",SL_Profile_Struct[i].aborthandler_entrytick);
          			fprintf(fptests,"%d\n",SL_Profile_Struct[i].aborthandler_exittick);
        		}

        		fclose(fptests);



        		fputs("ramgroup\talgoinfo\tlast_entrytick\tlast_exittick\ttexecution_count\tcumilative_execution_tick\n",fppbisttests);
        		for(i =0;i<PBIST_MEM_MAX;i++)
        		{
            		for(j =0;j<PBIST_ALGO_MAX;j++)
            		{

            		fprintf(fppbisttests,"%s\t",stringFromramgroup(i));
        			fprintf(fppbisttests,"%s\t",stringFromalgo(j));
          			fprintf(fppbisttests,"%d\t",SL_Pbist_Profile_Struct[i][j].last_entrytick);
          			fprintf(fppbisttests,"%d\t",SL_Pbist_Profile_Struct[i][j].last_exittick);
          			fprintf(fppbisttests,"%d\t",SL_Pbist_Profile_Struct[i][j].execution_count);
          			fprintf(fppbisttests,"%d\n",SL_Pbist_Profile_Struct[i][j].cumilative_execution_tick);
           		}
        		}
        		fclose(fptests);


        	}
        	for(i=0;i<10000;i++); //wait for fprintf to complete
        	__asm(" b #-8 ");
#endif

        }


        /* Run EFuse self tests */
        stConfigEFuse.numPatterns      = 600u;
        stConfigEFuse.seedSignature    = 0x5362F97Fu;
        stConfigEFuse.failInfo.stResult= ST_FAIL;
        stConfigEFuse.failInfo.failInfo= EFUSE_ERROR_NONE;
        retVal = SL_SelfTest_EFuse(EFUSE_SELF_TEST_STUCK_AT_ZERO, TRUE, &stConfigEFuse);
        INCREMENT_PASS_FAIL_COUNTER(stConfigEFuse.failInfo.stResult, retVal);

        retVal = SL_SelfTest_EFuse(EFUSE_SELF_TEST_ECC, TRUE, &stConfigEFuse);
        while (TRUE != SL_SelfTest_Status_EFuse(&stConfigEFuse.failInfo));
        INCREMENT_PASS_FAIL_COUNTER(stConfigEFuse.failInfo.stResult, retVal);

        /* Run RAD Self tests on sRAM */
        retVal = SL_SelfTest_SRAM(SRAM_RADECODE_DIAGNOSTICS, TRUE, &failInfoTCMRAM);
        INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);

    	/* PSCON Selftests */
        retVal = SL_SelfTest_PSCON(PSCON_SELF_TEST, TRUE, &failInfoPSCON);
        INCREMENT_PASS_FAIL_COUNTER(failInfoPSCON.stResult, retVal);

        retVal = SL_SelfTest_PSCON(PSCON_ERROR_FORCING, TRUE, &failInfoPSCON);
        INCREMENT_PASS_FAIL_COUNTER(failInfoPSCON.stResult, retVal);

    	if(SL_SelfTest_PSCON(PSCON_ERROR_FORCING_FAULT_INJECT, TRUE, &failInfoPSCON))
    	{
    		/*check for the esm callback */
    		if(FALSE == pscon_errorforcing_app_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
    			periodicSTPassCount++;
    			pscon_errorforcing_app_callback = FALSE;
    		}
    	}

        retVal = SL_SelfTest_PSCON(PSCON_SELF_TEST_ERROR_FORCING, TRUE, &failInfoPSCON);
        INCREMENT_PASS_FAIL_COUNTER(failInfoPSCON.stResult, retVal);

    	if(SL_SelfTest_PSCON(PSCON_SELF_TEST_ERROR_FORCING_FAULT_INJECT, TRUE, &failInfoPSCON))
    	{
    		/*check for the esm callback */
    		if(FALSE == pscon_selftest_errorforcing_app_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
    			periodicSTPassCount++;
    			pscon_selftest_errorforcing_app_callback = FALSE;
    		}
    	}

        retVal = SL_SelfTest_PSCON(PSCON_PMA_TEST, TRUE, &failInfoPSCON);
        if (FALSE == retVal) { /* Must fail, since PMA tests cannot be run in privilege modes */
               	periodicSTPassCount++;
               } else {
               	periodicSTFailCount++;
               }

		/* Memory interconnect - reserved location access */
		retVal = SL_SelfTest_MemoryInterconnect(MEMINTRCNT_RESERVED_ACCESS);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		/* Main Peripheral interconnect - reserved location access */
		retVal = SL_SelfTest_MainPeripheralInterconnect(MAINPERIPHINTRCNT_RESERVED_ACCESS);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		/* Peripheral Segment1 interconnect - reserved location access.
		 * Note: don't need to pass the following: volatile uint32* location, volatile uint32* protsetreg, uint32 protbit */
		retVal = SL_SelfTest_PeripheralSegmentInterconnect(PERIPHSEGINTRCNT_RESERVED_ACCESS, SL_PERIPH_SEGMENT1, (volatile uint32*)0U, 0U, 0U);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		/* Peripheral Segment1 interconnect - unprivileged access of protected location -
		 * Note: Segment Instance is not used. */
		retVal = SL_SelfTest_PeripheralSegmentInterconnect(PERIPHSEGINTRCNT_UNPRIVELEGED_ACCESS, SL_PERIPH_SEGMENT1, (volatile uint32*)0xFFFFF000U, (volatile uint32*)0xFFFF1020u, 16u);
		if (FALSE == retVal) { /* Must fail, since unprivileged access tests cannot be run in privilege modes */
        	periodicSTPassCount++;
        } else {
        	periodicSTFailCount++;
        }

		/* Peripheral Segment2 interconnect - reserved location access
		 * Note: don't need to pass the following: volatile uint32* location, volatile uint32* protsetreg, uint32 protbit  */
		retVal = SL_SelfTest_PeripheralSegmentInterconnect(PERIPHSEGINTRCNT_RESERVED_ACCESS, SL_PERIPH_SEGMENT2, (volatile uint32*)0U, 0U, 0U);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		/* Peripheral Segment2 interconnect - unprivileged access of protected location
		 * Note: Segment Instance is not used. */
		retVal = SL_SelfTest_PeripheralSegmentInterconnect(PERIPHSEGINTRCNT_UNPRIVELEGED_ACCESS, SL_PERIPH_SEGMENT2,  (volatile uint32*)0xFCF78C00U, (volatile uint32*)0xFCFF102Cu, 16u);
		if (FALSE == retVal) { /* Must fail, since unprivileged access tests cannot be run in privilege modes */
        	periodicSTPassCount++;
        } else {
        	periodicSTFailCount++;
        }

		/* Peripheral Segment3 interconnect - reserved location access
           Note: don't need to pass the following: volatile uint32* location, volatile uint32* protsetreg, uint32 protbit  */
		retVal = SL_SelfTest_PeripheralSegmentInterconnect(PERIPHSEGINTRCNT_RESERVED_ACCESS, SL_PERIPH_SEGMENT3, (volatile uint32*)0U, 0U, 0U);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		/* Peripheral Segment3 interconnect - unprivileged access of protected location
		 * Note: Segment Instance is not used. */
		retVal = SL_SelfTest_PeripheralSegmentInterconnect(PERIPHSEGINTRCNT_UNPRIVELEGED_ACCESS,  SL_PERIPH_SEGMENT3, (volatile uint32*)0xFFF7BC08U, (volatile uint32*)0xFFF78028u, 0u);
		if (FALSE == retVal) { /* Must fail, since unprivileged access tests cannot be run in privilege modes */
        	periodicSTPassCount++;
        } else {
        	periodicSTFailCount++;
        }


		/* ADC SRAM Parity Test */

		SL_ADC_Config adc_config;
		adc_config.adc_channel = 0u;
		adc_config.adcbase = sl_adcREG1;
		SL_ADC_Pinstatus pinStatus;

		retVal = SL_SelfTest_ADC(ADC_SRAM_PARITY_TEST, FALSE, &adc_config, &pinStatus);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		adc_config.adcbase = sl_adcREG2;
		retVal = SL_SelfTest_ADC(ADC_SRAM_PARITY_TEST, FALSE, &adc_config, &pinStatus);
		INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

		/* MIBSPI Analog Loopback Test */
    	retVal = SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI2);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
    	retVal = SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI4);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
    	retVal = SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI3);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
    	retVal = SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI1);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
    	retVal = SL_SelfTest_MibSPI(MIBSPI_ANALOG_LOOPBACK_TEST,SL_MIBSPI5);
    	INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

    	//Software test of VIM functionality
        retVal = SL_SelfTest_VIM(VIM_SOFTWARE_TEST);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

    	//Software test of DMA functionality
        retVal = SL_SelfTest_DMA(DMA_SOFTWARE_TEST);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);


        retVal = SL_SelfTest_DMA(DMA_ECC_TEST_MODE_1BIT);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

        retVal = SL_SelfTest_DMA(DMA_ECC_TEST_MODE_2BIT);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

    	if(SL_SelfTest_DMA(DMA_ECC_TEST_MODE_1BIT_FAULT_INJECT))
    	{
    		/*check for the esm callback */
    		if(FALSE == dma_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                dma_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_DMA(DMA_ECC_TEST_MODE_2BIT_FAULT_INJECT))
    	{
    		/*check for the esm callback */
    		if(FALSE == dma_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                dma_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}


#if 1

        retVal = SL_SelfTest_CAN(CAN_ECC_TEST_MODE_1BIT, SL_DCAN1);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

        retVal = SL_SelfTest_CAN(CAN_ECC_TEST_MODE_1BIT, SL_DCAN2);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal = SL_SelfTest_CAN(CAN_ECC_TEST_MODE_1BIT, SL_DCAN3);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal = SL_SelfTest_CAN(CAN_ECC_TEST_MODE_1BIT, SL_DCAN4);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

        retVal = SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT, SL_DCAN1);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal = SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT, SL_DCAN2);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal = SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT, SL_DCAN3);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
        retVal = SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT, SL_DCAN4);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
    	if(SL_SelfTest_CAN(CAN_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_DCAN1))
    	{
    		/*check for the esm callback */
    		if(FALSE == can_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                can_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}
    	if(SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_DCAN1))
    	{
    		/*check for the esm callback */
    		if(FALSE == can_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                can_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

    	if(SL_SelfTest_CAN(CAN_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_DCAN2))
    	{
    		/*check for the esm callback */
    		if(FALSE == can_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                can_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}
    	if(SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_DCAN2))
    	{
    		/*check for the esm callback */
    		if(FALSE == can_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                can_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

    	if(SL_SelfTest_CAN(CAN_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_DCAN3))
    	{
    		/*check for the esm callback */
    		if(FALSE == can_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                can_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}
    	if(SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_DCAN3))
    	{
    		/*check for the esm callback */
    		if(FALSE == can_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                can_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

    	if(SL_SelfTest_CAN(CAN_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_DCAN4))
    	{
    		/*check for the esm callback */
    		if(FALSE == can_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                can_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}
    	if(SL_SelfTest_CAN(CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_DCAN4))
    	{
    		/*check for the esm callback */
    		if(FALSE == can_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                can_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

#if 1
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT, SL_MIBSPI1);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT, SL_MIBSPI1);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

#if 1
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT, SL_MIBSPI2);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT, SL_MIBSPI2);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

#if 1
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT, SL_MIBSPI3);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT, SL_MIBSPI3);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

#if 1
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT, SL_MIBSPI4);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT, SL_MIBSPI4);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

#if 1
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT, SL_MIBSPI5);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);
#endif
        retVal = SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT, SL_MIBSPI5);
        INCREMENT_PASS_FAIL_COUNTER(ST_PASS, retVal);

    	if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_MIBSPI1))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_MIBSPI2))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_MIBSPI3))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_MIBSPI4))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_1BIT_FAULT_INJECT, SL_MIBSPI5))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_1bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_1bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI1))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI2))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI3))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}


        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI4))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        if(SL_SelfTest_MibSPI(MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT, SL_MIBSPI5))
    	{
    		/*check for the esm callback */
    		if(FALSE == mibspi_2bit_faultinject_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
                mibspi_2bit_faultinject_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}

        /* Run 1Bit ECC test on Flash */
        retVal = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_1BIT, TRUE, &failInfoFlash);
        INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

         /* Run 2Bit ECC test on Flash */
		retVal = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_2BIT, TRUE, &failInfoFlash);
		INCREMENT_PASS_FAIL_COUNTER(failInfoFlash, retVal);

    	/* Run 1Bit ECC test on sRAM */
    	retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_1BIT, TRUE, &failInfoTCMRAM);
    	INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);

    	/* Run 2Bit ECC test on sRAM */
    	retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_2BIT, TRUE, &failInfoTCMRAM);
    	INCREMENT_PASS_FAIL_COUNTER(failInfoTCMRAM, retVal);

    	/*running PBISTALGO_MARCH13N_1PORT algorithms on 2 port memories*/
    	for(i = 0;i < (sizeof(all2portmemories)/sizeof(uint64));i++)
    	{
            /* only run March13N algorithm. */
    		{
    	        retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
    	        							all2portmemories[i],
    	        							PBISTALGO_MARCH13N_2PORT);

    	        while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers));

    	        SL_SelfTest_PBIST_StopExec();
    	        INCREMENT_PASS_FAIL_COUNTER(failInfoPBISTOthers.stResult, retVal);
    		}
    	}

    	/*running PBISTALGO_MARCH13N_1PORT algorithms on 1 port memories*/
    	for(i = 0;i < (sizeof(all1portmemories)/sizeof(uint64));i++)
    	{
            /* Only run March13N algorithm. */
    		{
    	        retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
    	        							all1portmemories[i],
    	        							PBISTALGO_MARCH13N_1PORT);
    	        while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers));

    	        SL_SelfTest_PBIST_StopExec();INCREMENT_PASS_FAIL_COUNTER(failInfoPBISTOthers.stResult, retVal);
    		}
    	}

    	/*running pbist on all ROM memories*/
    	for(i = 0;i < (sizeof(allROMmemories)/sizeof(uint64));i++)
    	{
    		for(j =0;j<(sizeof(allROMalgos)/sizeof(uint32));j++)
    		{
    	        retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
    	        							allROMmemories[i],
    	        							allROMalgos[j]);
    	        while (TRUE != SL_SelfTest_Status_PBIST(&failInfoPBISTOthers));
    	        SL_SelfTest_PBIST_StopExec();INCREMENT_PASS_FAIL_COUNTER(failInfoPBISTOthers.stResult, retVal);
    		}
    	}

        vimInit();
        SL_ESM_Init(ESM_ApplicationCallback);

        /* Re-Initialise CAN SRAM after pbist tests*/
        SL_Init_Memory(RAMTYPE_DCAN1_RAM);
        SL_Init_Memory(RAMTYPE_DCAN2_RAM);
        SL_Init_Memory(RAMTYPE_DCAN3_RAM);
        SL_Init_Memory(RAMTYPE_DCAN4_RAM);

        retVal = SL_SelfTest_CCMR5F(CCMR5F_CPUCOMP_SELF_TEST,  TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);

		retVal = SL_SelfTest_CCMR5F(CCMR5F_CPUCOMP_SELF_TEST_ERROR_FORCING, TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);//working

		retVal = SL_SelfTest_CCMR5F(CCMR5F_CPUCOMP_ERROR_FORCING_TEST, TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);//working

        retVal = SL_SelfTest_CCMR5F(CCMR5F_VIMCOMP_SELF_TEST,  TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);

		retVal = SL_SelfTest_CCMR5F(CCMR5F_VIMCOMP_SELF_TEST_ERROR_FORCING, TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);//working

		retVal = SL_SelfTest_CCMR5F(CCMR5F_VIMCOMP_ERROR_FORCING_TEST, TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);//working

        retVal = SL_SelfTest_CCMR5F(CCMR5F_PDCOMP_SELF_TEST,  TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);

		retVal = SL_SelfTest_CCMR5F(CCMR5F_PDCOMP_SELF_TEST_ERROR_FORCING, TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);//working

		retVal = SL_SelfTest_CCMR5F(CCMR5F_PDCOMP_ERROR_FORCING_TEST, TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);//working

        retVal = SL_SelfTest_CCMR5F(CCMR5F_INMCOMP_SELF_TEST,  TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);

		retVal = SL_SelfTest_CCMR5F(CCMR5F_INMCOMP_SELF_TEST_ERROR_FORCING, TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);//working

		retVal = SL_SelfTest_CCMR5F(CCMR5F_INMCOMP_ERROR_FORCING_TEST, TRUE, &failInfoCCMR5F);
		INCREMENT_PASS_FAIL_COUNTER(failInfoCCMR5F.stResult, retVal);//working

		if(SL_SelfTest_CCMR5F(CCMR5F_CPUCOMP_ERROR_FORCING_TEST_FAULT_INJECT, TRUE, &failInfoCCMR5F))
    	{
    		/*check for the esm callback */
    		if(FALSE == cccmr5f_errorforcing_app_callback)
    		{
    			periodicSTFailCount++;
    		}
    		else
    		{
    			cccmr5f_errorforcing_app_callback = FALSE;
    			periodicSTPassCount++;
    		}
    	}
    	if(SL_SelfTest_CCMR5F(CCMR5F_VIMCOMP_ERROR_FORCING_TEST_FAULT_INJECT, TRUE, &failInfoCCMR5F))
    	{
    		/*check for the esm callback */
    	    if(FALSE == cccmr5f_errorforcing_app_callback)
    	    {
    	    	periodicSTFailCount++;
    	    }
    	    else
    	    {
    	    	cccmr5f_errorforcing_app_callback = FALSE;
    	    	periodicSTPassCount++;
    	    }
    	}
    	if(SL_SelfTest_CCMR5F(CCMR5F_PDCOMP_ERROR_FORCING_TEST_FAULT_INJECT, TRUE, &failInfoCCMR5F))
    	{
    		/*check for the esm callback */
    	    if(FALSE == cccmr5f_errorforcing_app_callback)
    	    {
    	    	periodicSTFailCount++;
    	    }
    	    else
    	    {
    	    	cccmr5f_errorforcing_app_callback = FALSE;
    	    	periodicSTPassCount++;
    	    }
    	}
    	if(SL_SelfTest_CCMR5F(CCMR5F_INMCOMP_ERROR_FORCING_TEST_FAULT_INJECT, TRUE, &failInfoCCMR5F))
    	{
    		/*check for the esm callback */
    	    if(FALSE == cccmr5f_errorforcing_app_callback)
    	    {
    	    	periodicSTFailCount++;
    	    }
    	    else
    	    {
    	    	cccmr5f_errorforcing_app_callback = FALSE;
    	    	periodicSTPassCount++;
    	    }
    	}

    }
}
#endif
