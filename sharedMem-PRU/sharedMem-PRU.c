#include <stdint.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "../sharedMem-Linux/sharedDataStruct.h"

// Reference for shared RAM:
// https://markayoder.github.io/PRUCookbook/05blocks/blocks.html#_controlling_the_pwm_frequency

/*
    1.  Set both GPIO pins to be controlled by PRU0
            config-pin p8_12 pruout
            config-pin p8_15 pruin

    Display something on 14-seg display:
    2a. Enable I2C:
            config-pin P9_18 i2c
            config-pin P9_17 i2c

    2b. Enable I2C chip & set pattern; Pick correct board
        ZEN CAPE GREEN:
            i2cset -y 1 0x20 0x00 0x00
            i2cset -y 1 0x20 0x01 0x00
            i2cset -y 1 0x20 0x14 0x1E
            i2cset -y 1 0x20 0x15 0x78
        ZEN CAPE RED
            i2cset -y 1 0x20 0x02 0x00
            i2cset -y 1 0x20 0x03 0x00
            i2cset -y 1 0x20 0x00 0x0f
            i2cset -y 1 0x20 0x01 0x5e
*/

#define STR_LEN         8       // # LEDs in our string
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u

// P8_11 for output (on R30), PRU0
#define DATA_PIN 15       // Bit number to output on


// GPIO Configuration
// ----------------------------------------------------------
volatile register uint32_t __R30;   // output GPIO register
volatile register uint32_t __R31;   // input GPIO register

// GPIO Output: P8_12 = pru0_pru_r30_14 
//   = LEDDP2 (Turn on/off right 14-seg digit) on Zen cape
#define DIGIT_ON_OFF_MASK (1 << 14)
// GPIO Input: P8_15 = pru0_pru_r31_15 
//   = JSRT (Joystick Right) on Zen Cape
//   (Happens to be bit 15 and p8_15; just a coincidence; see P8 header info sheet)
#define JOYSTICK_RIGHT_MASK (1 << 15)
#define JOYSTICK_DOWN_MASK (1 << 14)

// Shared Memory Configuration
// -----------------------------------------------------------
#define THIS_PRU_DRAM       0x00000         // Address of DRAM
#define OFFSET              0x200           // Skip 0x100 for Stack, 0x100 for Heap (from makefile)
#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)

// This works for both PRU0 and PRU1 as both map their own memory to 0x0000000
volatile sharedMemStruct_t *pSharedMemStruct = (volatile void *)THIS_PRU_DRAM_USABLE;



void main(void)
{
    // Initialize:

    pSharedMemStruct->isRightButtonPressed = false;
    pSharedMemStruct->isDownButtonPressed = false;
    uint32_t* color = pSharedMemStruct->color;
    for(int i = 0; i < 8; i++){
        color[i] = 0;
    }


     CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    while (true) { 
        __delay_cycles(resetCycles);

        for(int j = 0; j < STR_LEN; j++) {
            for(int i = 31; i >= 0; i--) {
                if(color[j] & ((uint32_t)0x1 << i)) {
                    __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
                    __delay_cycles(oneCyclesOn-1);
                    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
                    __delay_cycles(oneCyclesOff-2);
                } else {
                    __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
                    __delay_cycles(zeroCyclesOn-1);
                    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
                    __delay_cycles(zeroCyclesOff-2);
                }
            }
        }

    // Send Reset
    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
    __delay_cycles(resetCycles);


 

        // Sample button state to shared memory
        pSharedMemStruct->isRightButtonPressed = (__R31 & JOYSTICK_RIGHT_MASK) != 0;
        pSharedMemStruct->isDownButtonPressed = (__R31 & JOYSTICK_DOWN_MASK) != 0;
    }
}
