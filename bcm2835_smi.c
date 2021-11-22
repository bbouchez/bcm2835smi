/* bcm2835_smi.c
// SMI support for BCM2835 library created by Mike McCauley
// Databus width is limited to 8 bits and address bus width
// is limited to 4 bits because of the way RPi GPIO
// connector is built (SA4 and SA5 are used for HAT I2C
// EEPROM connection)
// On Compute Module, it should be possible to use all
// bus configurations (like the 9 (?) or 16 bits) but
// has not yet been tested
//
// Author: Benoit BOUCHEZ (BEB)
// Copyright (C) 2020-2021 Benoit BOUCHEZ
*/

#include "bcm2835.h"
#include "bcm2835_smi.h"
#include <sys/mman.h>

int bcm2835_smi_begin(void)
{
    volatile uint32_t* paddr;
    uint32_t defConfig;

    if (bcm2835_smi == MAP_FAILED)
      return 0; /* bcm2835_smi_init() failed, or not root */

    /* Set the SMI pins to the Alt 1 function to enable SMI access on them */
    bcm2835_gpio_fsel(2, BCM2835_GPIO_FSEL_ALT1); /* SA3 */
    bcm2835_gpio_fsel(3, BCM2835_GPIO_FSEL_ALT1); /* SA2 */
    bcm2835_gpio_fsel(4, BCM2835_GPIO_FSEL_ALT1); /* SA1 */
    bcm2835_gpio_fsel(5, BCM2835_GPIO_FSEL_ALT1); /* SA0 */
    bcm2835_gpio_fsel(6, BCM2835_GPIO_FSEL_ALT1); /* SOE_N / SE */
    bcm2835_gpio_fsel(7, BCM2835_GPIO_FSEL_ALT1); /* SWE_N / SRW_N */
    bcm2835_gpio_fsel(8, BCM2835_GPIO_FSEL_ALT1); /* SD0 */
    bcm2835_gpio_fsel(9, BCM2835_GPIO_FSEL_ALT1); /* SD1 */
    bcm2835_gpio_fsel(10, BCM2835_GPIO_FSEL_ALT1); /* SD2 */
    bcm2835_gpio_fsel(11, BCM2835_GPIO_FSEL_ALT1); /* SD3 */
    bcm2835_gpio_fsel(12, BCM2835_GPIO_FSEL_ALT1); /* SD4 */
    bcm2835_gpio_fsel(13, BCM2835_GPIO_FSEL_ALT1); /* SD5 */
    bcm2835_gpio_fsel(14, BCM2835_GPIO_FSEL_ALT1); /* SD6 */
    bcm2835_gpio_fsel(15, BCM2835_GPIO_FSEL_ALT1); /* SD7 */

    /* Set the SMI clock to 125MHz -> one clock period lasts 8 ns */
    paddr = bcm2835_clk + SMICLK_CNTL;
    bcm2835_peri_write (paddr, 0x5A000000);     /* Disable SMI clock */
    paddr = bcm2835_clk + SMICLK_DIV;
    bcm2835_peri_write (paddr, 0x5A004000);     /* Set clock divider to 4 */
    paddr = bcm2835_clk + SMICLK_CNTL;
    bcm2835_peri_write (paddr, 0x5A000016);     /* Enable SMI clock with PLLD */

    /* Set a default useable configuration for the four SMI config slots */
    /* 8 bits, Intel mode, always pace, 10 clocks for R/W setup = 80 ns
     20 clocks for R/W strobe = 160 ns, 20 clocks for R/W hold = 160 ns,
     1 clock for pace control (not used but a value is needed) */
    defConfig = BCM2835_SMI_RW_WID8 | BCM2835_SMI_RW_MODE80  |
             BCM2835_SMI_RW_PACEALL | (10<<BCM2835_SMI_RW_SETUP_LS) |
             (20<<BCM2835_SMI_RW_STROBE_LS) | (20<<BCM2835_SMI_RW_HOLD_LS) |
             (1<<BCM2835_SMI_RW_PACE_LS);

    paddr = bcm2835_smi + BCM2835_SMI_READ0;
    bcm2835_peri_write (paddr, defConfig);
    paddr = bcm2835_smi + BCM2835_SMI_WRITE0;
    bcm2835_peri_write (paddr, defConfig);
    paddr = bcm2835_smi + BCM2835_SMI_READ1;
    bcm2835_peri_write (paddr, defConfig);
    paddr = bcm2835_smi + BCM2835_SMI_WRITE1;
    bcm2835_peri_write (paddr, defConfig);
    paddr = bcm2835_smi + BCM2835_SMI_READ2;
    bcm2835_peri_write (paddr, defConfig);
    paddr = bcm2835_smi + BCM2835_SMI_WRITE2;
    bcm2835_peri_write (paddr, defConfig);
    paddr = bcm2835_smi + BCM2835_SMI_READ3;
    bcm2835_peri_write (paddr, defConfig);
    paddr = bcm2835_smi + BCM2835_SMI_WRITE3;
    bcm2835_peri_write (paddr, defConfig);

    return 1; // OK
}  /* bcm2835_smi_begin */
/* --------------------------------------------------- */

void bcm2835_smi_end(void)
{
    /* Set all the SMI pins back to input */
    bcm2835_gpio_fsel(2, BCM2835_GPIO_FSEL_INPT); /* SA3 */
    bcm2835_gpio_fsel(3, BCM2835_GPIO_FSEL_INPT); /* SA2 */
    bcm2835_gpio_fsel(4, BCM2835_GPIO_FSEL_INPT); /* SA1 */
    bcm2835_gpio_fsel(5, BCM2835_GPIO_FSEL_INPT); /* SA0 */
    bcm2835_gpio_fsel(6, BCM2835_GPIO_FSEL_INPT); /* SOE_N / SE */
    bcm2835_gpio_fsel(7, BCM2835_GPIO_FSEL_INPT); /* SWE_N / SRW_N */
    bcm2835_gpio_fsel(8, BCM2835_GPIO_FSEL_INPT); /* SD0 */
    bcm2835_gpio_fsel(9, BCM2835_GPIO_FSEL_INPT); /* SD1 */
    bcm2835_gpio_fsel(10, BCM2835_GPIO_FSEL_INPT); /* SD2 */
    bcm2835_gpio_fsel(11, BCM2835_GPIO_FSEL_INPT); /* SD3 */
    bcm2835_gpio_fsel(12, BCM2835_GPIO_FSEL_INPT); /* SD4 */
    bcm2835_gpio_fsel(13, BCM2835_GPIO_FSEL_INPT); /* SD5 */
    bcm2835_gpio_fsel(14, BCM2835_GPIO_FSEL_INPT); /* SD6 */
    bcm2835_gpio_fsel(15, BCM2835_GPIO_FSEL_INPT); /* SD7 */
}  /* bcm2835_smi_end */
/* --------------------------------------------------- */

void bcm2835_smi_set_timing (uint32_t smichannel, uint32_t readchannel,
                             uint32_t setupcycles, uint32_t strobecycles,
                             uint32_t holdcycles, uint32_t pacecycles)
{
    int RegOffset;
    volatile uint32_t* psmitiming;
    uint32_t RegValue;

    if (readchannel!=0)
    {
        switch (smichannel)
        {
            case 0 : RegOffset = BCM2835_SMI_READ0; break;
            case 1 : RegOffset = BCM2835_SMI_READ1; break;
            case 2 : RegOffset = BCM2835_SMI_READ2; break;
            case 3 : RegOffset = BCM2835_SMI_READ3; break;
            default : return;
        }
    }
    else
    {
        switch (smichannel)
        {
            case 0 : RegOffset = BCM2835_SMI_WRITE0; break;
            case 1 : RegOffset = BCM2835_SMI_WRITE1; break;
            case 2 : RegOffset = BCM2835_SMI_WRITE2; break;
            case 3 : RegOffset = BCM2835_SMI_WRITE3; break;
            default : return;
        }
    }

    /* Get current timing configuration of the slot */
    psmitiming = bcm2835_smi + RegOffset;
    RegValue = bcm2835_peri_read (psmitiming);
    /* Clear timing fields in register */
    RegValue &= ~(BCM2835_SMI_RW_SETUP_MSK|BCM2835_SMI_RW_HOLD_MSK|BCM2835_SMI_RW_PACE_MSK|BCM2835_SMI_RW_STROBE_MSK);
    /* Set timing values and write back to register */
    RegValue |= (setupcycles << BCM2835_SMI_RW_SETUP_LS)  |
                (strobecycles << BCM2835_SMI_RW_STROBE_LS) |
                (holdcycles << BCM2835_SMI_RW_HOLD_LS)   |
                (pacecycles << BCM2835_SMI_RW_PACE_LS);
    bcm2835_peri_write (psmitiming, RegValue);
}  /* bcm2835_set_smi_timing */
/* --------------------------------------------------- */

void bcm2835_smi_write(uint32_t smichannel, uint8_t data, uint32_t address)
{
    uint32_t status;
    volatile uint32_t* psmics = bcm2835_smi + BCM2835_SMI_DIRCS;
    volatile uint32_t* psmiaddr = bcm2835_smi + BCM2835_SMI_DIRADDR;
    volatile uint32_t* psmidata = bcm2835_smi + BCM2835_SMI_DIRDATA;

    /* Make sure we use a configuration that exists */
    if (smichannel>3) return;

    /* clear done bit if set */
    if (bcm2835_peri_read(psmics) & BCM2835_SMI_DIRCS_DONE)
    {
        bcm2835_peri_write (psmics, BCM2835_SMI_DIRCS_DONE);
    }

    /* Start write transfer */
    bcm2835_peri_write (psmiaddr, (smichannel<<BCM2835_SMI_DIRADRS_DEV_LS) | (address&BCM2835_SMI_DIRADRS_MSK));
    bcm2835_peri_write (psmidata, data);
    bcm2835_peri_write (psmics, BCM2835_SMI_DIRCS_WRITE|BCM2835_SMI_DIRCS_START|BCM2835_SMI_DIRCS_ENABLE);

    /* Wait until write cycle is finished */
    do {
        status = bcm2835_peri_read(psmics);
    } while ((status & BCM2835_SMI_DIRCS_DONE)==0);

    /* clear done bit */
    bcm2835_peri_write (psmics, BCM2835_SMI_DIRCS_DONE);
}  /* bcm2835_smi_write */
/* --------------------------------------------------- */

uint32_t bcm2835_smi_read (uint32_t smichannel, uint32_t address)
{
    uint32_t status;
    uint32_t data;
    volatile uint32_t* psmics = bcm2835_smi + BCM2835_SMI_DIRCS;
    volatile uint32_t* psmiaddr = bcm2835_smi + BCM2835_SMI_DIRADDR;
    volatile uint32_t* psmidata = bcm2835_smi + BCM2835_SMI_DIRDATA;

    /* Make sure we use a configuration that exists */
    if (smichannel>3) return 0;

    /* clear done bit if set */
    if (bcm2835_peri_read(psmics) & BCM2835_SMI_DIRCS_DONE)
    {
        bcm2835_peri_write (psmics, BCM2835_SMI_DIRCS_DONE);
    }

    /* Start read transfer */
    bcm2835_peri_write (psmiaddr, (smichannel<<BCM2835_SMI_DIRADRS_DEV_LS) | (address&BCM2835_SMI_DIRADRS_MSK));
    bcm2835_peri_write (psmics, BCM2835_SMI_DIRCS_START|BCM2835_SMI_DIRCS_ENABLE);

    /* Wait until read cycle is finished */
    do {
        status = bcm2835_peri_read(psmics);
    } while ((status & BCM2835_SMI_DIRCS_DONE)==0);

    /* Read data */
    data = bcm2835_peri_read (psmidata);

    /* clear done bit */
    bcm2835_peri_write (psmics, BCM2835_SMI_DIRCS_DONE);

    return data;
}  /* bcm2835_smi_read */
/* --------------------------------------------------- */
