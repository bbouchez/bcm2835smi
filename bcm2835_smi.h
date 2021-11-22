#ifndef __BCM2835_SMI_H__
#define __BCM2835_SMI_H__

/* Registers offets from BCM2835_SMI_BASE */
#define BCM2835_SMI_CS           0          /*! < Control and status register > */
#define BCM2835_SMI_LENGTH       1          /*! < Transfer length register > */
#define BCM2835_SMI_ADRS         2          /*! < Transfer address register > */
#define BCM2835_SMI_DATA         3          /*! < Transfer data register > */
#define BCM2835_SMI_READ0        4          /*! < Read  settings 0 register > */
#define BCM2835_SMI_WRITE0       5          /*! < Write settings 0 register > */
#define BCM2835_SMI_READ1        6          /*! < Read  settings 1 register > */
#define BCM2835_SMI_WRITE1       7          /*! < Write settings 1 register > */
#define BCM2835_SMI_READ2        8          /*! < Read  settings 2 register > */
#define BCM2835_SMI_WRITE2       9          /*! < Write settings 2 register > */
#define BCM2835_SMI_READ3       10          /*! < Read  settings 3 register > */
#define BCM2835_SMI_WRITE3      11          /*! < Write settings 3 register > */
#define BCM2835_SMI_DMAC        12          /*! < DMA control register > */
#define BCM2835_SMI_DIRCS       13          /*! < Direct control register > */
#define BCM2835_SMI_DIRADDR     14          /*! < Direct access address register > */
#define BCM2835_SMI_DIRDATA     15          /*! < Direct access data register > */

/* Register masks for SMI_READ and SMI_WRITE */
#define BCM2835_SMI_RW_WIDTH_MSK        0xC0000000 /*! < Data width mask > */
#define BCM2835_SMI_RW_WID8             0x00000000 /*! < Data width 8 bits > */
#define BCM2835_SMI_RW_WID16            0x40000000 /*! < Data width 16 bits > */
#define BCM2835_SMI_RW_WID18            0x80000000 /*! < Data width 18 bits > */
#define BCM2835_SMI_RW_WID9             0xC0000000 /*! < Data width 9 bits > */
#define BCM2835_SMI_RW_SETUP_MSK        0x3F000000 /*! < Setup cycles (6 bits) > */
#define BCM2835_SMI_RW_SETUP_LS         24         /*! < Shift for setup cycles > */
#define BCM2835_SMI_RW_MODE68           0x00800000 /*! < Run cycle motorola mode > */
#define BCM2835_SMI_RW_MODE80           0x00000000 /*! < Run cycle intel mode > */
#define BCM2835_SMI_READ_FSETUP         0x00400000 /*! < Read : Setup only for first cycle > */
#define BCM2835_SMI_WRITE_SWAP          0x00400000 /*! < Write : swap pixel data > */
#define BCM2835_SMI_RW_HOLD_MSK         0x003F0000 /*! < Hold cycles (6 bits) > */
#define BCM2835_SMI_RW_HOLD_LS          16         /*! < Shift for hold cycles > */
#define BCM2835_SMI_RW_PACEALL          0x00008000 /*! < Apply pacing always > */
#define BCM2835_SMI_RW_PACE_MSK         0x00007F00 /*! < Pace cycles (7 bits) > */
#define BCM2835_SMI_RW_PACE_LS          8          /*! < Shift for pace cycles > */
#define BCM2835_SMI_RW_DREQ             0x00000080 /*! < Use DMA req on read/write > */
#define BCM2835_SMI_RW_STROBE_MSK       0x0000007F /*! < Strobe cycles (7 bits) > */
#define BCM2835_SMI_RW_STROBE_LS        0          /*! < Shift for strobe cycles > */

/* Registers masks for Direct Access control register */
#define BCM2835_SMI_DIRCS_ENABLE        0x00000001  /*! < Set to enable SMI. 0 = Read from ext. devices > */
#define BCM2835_SMI_DIRCS_START         0x00000002  /*! < Initiate SMI transfer > */
#define BCM2835_SMI_DIRCS_DONE          0x00000004  /*! < Set if transfer has finished / Write to clear flag > */
#define BCM2835_SMI_DIRCS_WRITE         0x00000008  /*! < 1 = Write to ext. devices > */

/* Registers masks for Direct Access address register */
#define BCM2835_SMI_DIRADRS_DEV_MSK     0x00000300  /*! < Timing configuration slot >  */
#define BCM2835_SMI_DIRADRS_DEV_LS      8           /*! < Shift for configuration slot > */
#define BCM2835_SMI_DIRADRS_DEV0        0x00000000  /*! < Use timing config slot 0 > */
#define BCM2835_SMI_DIRADRS_DEV1        0x00000100  /*! < Use timing config slot 1 > */
#define BCM2835_SMI_DIRADRS_DEV2        0x00000200  /*! < Use timing config slot 2 > */
#define BCM2835_SMI_DIRADRS_DEV3        0x00000300  /*! < Use timing config slot 3 > */
#define BCM2835_SMI_DIRADRS_MSK         0x0000003F  /*! < Adress bits SA5..SA0 > */
#define BCM2835_SMI_DIRADRS_LS          0           /*! < Shift for address bits > */

/* SMI clock control registers : defined as offset from bcm2835_clk */
#define SMICLK_CNTL  (44)  /* = 0xB0 */
#define SMICLK_DIV   (45)  /* = 0xB4 */

#ifdef __cplusplus
extern "C" {
#endif

    /*! Start SMI operations.
      Forces RPi SMI pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK), P1-24 (CE0) and P1-26 (CE1)
      to alternate function ALT1, which enables those pins for SMI interface.
      You should call bcm2835_smi_end() when all SMI functions are complete to return the pins to
      their default functions.
      Only address bits SA0 to SA3 are available as RPi uses GPIO0 (SA5) and GPIO1 (SA4) for I2C
      HAT identification EEPROM access
      \sa  bcm2835_smi_end()
      \return 1 if successful, 0 otherwise (perhaps because you are not running as root)
    */
    extern int bcm2835_smi_begin (void);

    /*! End SMI operations.
      SMI pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK), P1-24 (CE0) and P1-26 (CE1)
      are returned to their default INPUT behaviour.
    */
    extern void bcm2835_smi_end (void);

    /*! Setup SMI bus cycle timing parameters
    There are four SMI channels for read operation and four channels for write operation
    Cycles are expressed as multiple of 8ns
    Note that Pace cycles are not used (no effect on hardware) but they are required for
    configuration. It is recommended to set this value to 1 (and not 0) to have the
    smallest cycle in case the hardware would recognize it
    \param[in] smichannel SMI configuration slot to setup (0 to 3)
    \param[in] readchannel Set to 1 to configure the read channel (0 = configure write channel)
    \param[in] setupcycles Time between address assertion on bus and OE/WR signal assertion
    \param[in] strobecycles Duration of OE/WR signal assertion
    \param[in] holdcycles Time after OE/WR deassertion before address is deasserted
    \param[in] pacecycles Time before next SMI bus cycle
    */
    extern void bcm2835_smi_set_timing(uint32_t smichannel, uint32_t readchannel,
                                        uint32_t setupcycles, uint32_t strobecycles,
                                        uint32_t holdcycles, uint32_t pacecycles);

    /*! Transfers one byte to SMI bus.
      Uses polled transfer as described in BCM 2835 ARM Peripherals manual
      \param[in] timingslot SMI configuration slot to use (0 to 3)
      \param[in] data The data byte to write
      \param[in] address The address to write to
      \sa bcm2835_smi_writenb()
    */
    extern void bcm2835_smi_write (uint32_t smichannel, uint8_t data, uint32_t address);

    /*! Reads one byte from SMI bus.
      Uses polled transfer as described in BCM 2835 ARM Peripherals manual
      \param[in] smichannel SMI configuration slot to use (0 to 3)
      \param[in] address The address to read from
      \return value read from SMI bus
      \sa bcm2835_smi_readnb()
    */
    extern uint32_t bcm2835_smi_read (uint32_t smichannel, uint32_t address);
#ifdef __cplusplus
}
#endif

#endif // __BCM2835_SMI_H__
