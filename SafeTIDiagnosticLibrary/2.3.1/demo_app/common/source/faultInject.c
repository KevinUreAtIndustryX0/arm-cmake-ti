#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
//This functionality is already present for R4 devices in sys_selftest.c

#include<HL_sys_selftest.h>

/** @fn void checkPLL1Slip(void)
*   @brief Check PLL1 Slip detection logic.
*
*   This function checks PLL1 Slip detection logic.
*/
/* SourceId : SELFTEST_SourceId_037 */
/* DesignId : SELFTEST_DesignId_030 */
/* Requirements : HL_SR384 */
void checkPLL1Slip(void)
{
    uint32 ghvsrc_bk, pllctl1_bk;

    /* Back up the the registers GHVSRC and PLLCTRL1 */
    ghvsrc_bk = systemREG1->GHVSRC;
    pllctl1_bk = systemREG1->PLLCTL1;

    /* Switch all clock domains to oscillator */
    systemREG1->GHVSRC = 0x00000000U;

    /* Disable Reset on PLL Slip and enable Bypass on PLL slip */
    systemREG1->PLLCTL1 &= 0x1FFFFFFFU;

    /* Force a PLL Slip */
    systemREG1->PLLCTL1 ^= 0x8000U;

    /* Wait till PLL slip flag is set */
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
   // while((sl_systemREG1->GBLSTAT & 0x300U) == 0U)
    {
    } /* Wait */

    if((esmREG->SR1[0U] & 0x400U) != 0x400U)
    {
        /* ESM flag not set */
        //selftestFailNotification(CHECKPLL1SLIP_FAIL1);
    }
    else
    {
        /* Disable PLL1 */
        systemREG1->CSDISSET = 0x2U;

        /* Wait till PLL1 is disabled */
        /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
        while((systemREG1->CSDIS & 0x2U) == 0U)
        {
        } /* Wait */

        /* Restore the PLL multiplier value */
        systemREG1->PLLCTL1 ^= 0x8000U;

        /* Enable PLL1 */
        systemREG1->CSDISCLR = 0x2U;

        /* Wait till PLL1 is disabled */
        /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
        while((systemREG1->CSDIS & 0x2U) != 0U)
        {
        } /* Wait */

        /* Switch back to the initial clock source */
        systemREG1->GHVSRC = ghvsrc_bk;

        /* Clear PLL slip flag */
        systemREG1->GBLSTAT = 0x300U;

        /* Clear ESM flag */
        esmREG->SR1[0U] = 0x400U;

        /* Restore the PLLCTL1 register */
        systemREG1->PLLCTL1 = pllctl1_bk;
    }
}

/** @fn void checkPLL2Slip(void)
*   @brief Check PLL2 Slip detection logic.
*
*   This function checks PLL2 Slip detection logic.
*/
/* SourceId : SELFTEST_SourceId_038 */
/* DesignId : SELFTEST_DesignId_031 */
/* Requirements : HL_SR384 */
void checkPLL2Slip(void)
{
    uint32 ghvsrc_bk;

    /* Back up the the register GHVSRC */
    ghvsrc_bk = systemREG1->GHVSRC;

    /* Switch all clock domains to oscillator */
    systemREG1->GHVSRC = 0x00000000U;

    /* Force a PLL2 Slip */
    systemREG2->PLLCTL3 ^= 0x8000U;

    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    //while((esmREG->SR4[0U] & 0x400U) != 0x400U)
    {
        /* Wait till ESM flag is set */
    }

    /* Disable PLL2 */
    systemREG1->CSDISSET = 0x40U;

    /* Wait till PLL2 is disabled */
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while((systemREG1->CSDIS & 0x40U) == 0U)
    {
    } /* Wait */

    /* Restore the PLL 2 multiplier value */
    systemREG2->PLLCTL3 ^= 0x8000U;

    /* Enable PLL2 */
    systemREG1->CSDISCLR = 0x40U;

    /* Wait till PLL2 is disabled */
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while((systemREG1->CSDIS & 0x40U) != 0U)
    {
    } /* Wait */

    /* Switch back to the initial clock source */
    systemREG1->GHVSRC = ghvsrc_bk;

    /* Clear PLL slip flag */
    systemREG1->GBLSTAT = 0x300U;

    /* Clear ESM flag */
    esmREG->SR4[0U] = 0x400U;
}

#endif
