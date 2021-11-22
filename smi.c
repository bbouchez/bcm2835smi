// smi.c
//
// Example program for bcm2835_smi library
// Shows how to send and receive a byte over SMI bus
//
// A CodeBlocks project is provided for easier tests
//
// This test program must be run with root rights !!
// From terminal, use sudo ./smi
//
// Author: Benoit BOUCHEZ (BEB)
// Copyright (C) 2021 Benoit BOUCHEZ

#include <stdio.h>
#include <stdlib.h>
#include "bcm2835.h"
#include "bcm2835_smi.h"

#define SMI_CHANNEL     0

// Number of SMI clock cycles for each SMI transaction phase
#define SMI_SETUP_CLOCKS    10      // setup time = 80ns
#define SMI_STROBE_CLOCKS   20      // strobe time = 160ns
#define SMI_HOLD_CLOCKS     20      // hold time = 160ns
#define SMI_PACE_CLOCKS     1       // pace time = 8ns

int main(int argc, char **argv)
{
    int Address=0;
    int WriteData;
    uint8_t ReadData[16];

    if (!bcm2835_init())
    {
        printf("bcm2835_init() failed. Are you running as root??\n");
        return 1;
    }

    if (!bcm2835_smi_begin())
    {
        printf("bcm2835_smi_begin() failed. Are you running as root??\n");
        return 1;
    }

    // The following test sequence writes 4096 bytes and reads 16 bytes
    // With default timing parameters, the complete sequence takes 4.6ms
    // on a RPi 3B (with O3 optimization), giving a transfer speed of 894kB/s
    // A SPI interface would require a 7MHz clock to achieve the same results...

    // Change SMI timing parameters for writing
    bcm2835_smi_set_timing (SMI_CHANNEL, 0, SMI_SETUP_CLOCKS, SMI_STROBE_CLOCKS, SMI_HOLD_CLOCKS, SMI_PACE_CLOCKS);
    // Change SMI timing parameters for reading
    bcm2835_smi_set_timing (SMI_CHANNEL, 1, SMI_SETUP_CLOCKS, SMI_STROBE_CLOCKS, SMI_HOLD_CLOCKS, SMI_PACE_CLOCKS);

    // Write a sequence of bytes to each possible address
    for (Address=0; Address<=15; Address++)
    {
        for (WriteData=0; WriteData<=255; WriteData++)
        {
            bcm2835_smi_write (SMI_CHANNEL, WriteData, Address);
        }
    }

    // Read a sequence of bytes at each possible address
    for (Address=0; Address<=15; Address++)
    {
        ReadData[Address] = bcm2835_smi_read (SMI_CHANNEL, Address);
     }

    // Release GPIO
    bcm2835_smi_end();
    bcm2835_close();

    return 0;
}
