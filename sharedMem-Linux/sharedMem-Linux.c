#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

#include "sharedDataStruct.h"
#include "sharedMem-Linux.h"
#include "utilities/runCommand.h"

#include "utilities/colors.h"
// General PRU Memomry Sharing Routine
// ----------------------------------------------------------------
#define PRU_ADDR      0x4A300000   // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN       0x80000      // Length of PRU memory
#define PRU0_DRAM     0x00000      // Offset to DRAM
#define PRU1_DRAM     0x02000
#define PRU_SHAREDMEM 0x10000      // Offset to shared memory
#define PRU_MEM_RESERVED 0x200     // Amount used by stack and heap

// Convert base address to each memory section
#define PRU0_MEM_FROM_BASE(base) ( (base) + PRU0_DRAM + PRU_MEM_RESERVED)
#define PRU1_MEM_FROM_BASE(base) ( (base) + PRU1_DRAM + PRU_MEM_RESERVED)
#define PRUSHARED_MEM_FROM_BASE(base) ( (base) + PRU_SHAREDMEM)

// Return the address of the PRU's base memory
volatile void* getPruMmapAddr(void)
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("ERROR: could not open /dev/mem");
        exit(EXIT_FAILURE);
    }

    // Points to start of PRU memory.
    volatile void* pPruBase = mmap(0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
    if (pPruBase == MAP_FAILED) {
        perror("ERROR: could not map memory");
        exit(EXIT_FAILURE);
    }
    close(fd);

    return pPruBase;
}

void freePruMmapAddr(volatile void* pPruBase)
{
    if (munmap((void*) pPruBase, PRU_LEN)) {
        perror("PRU munmap failed");
        exit(EXIT_FAILURE);
    }
}
///////////////////////////////////////////////////////////////////public Methods//////////////////////////////////////////////////////////////
static volatile sharedMemStruct_t *pSharedPru0;
static volatile void *pPruBase;

//start the joystick and the neopixel
void PRUinit(){
    runCommand("config-pin p8_15 pruin");
    runCommand("config-pin p8_16 pruin");
    runCommand("config-pin p8.11 pruout");
    runCommand("echo start | sudo tee /sys/class/remoteproc/remoteproc1/state || true");
    pPruBase = getPruMmapAddr();
    pSharedPru0 = PRU0_MEM_FROM_BASE(pPruBase);
    for(int i = 0; i <8; i++){
        pSharedPru0->color[i] = 0;
    }
}
//stops the joystick and the neopixel and free the pru shared memory
void PRUstop(){
    for(int i = 0; i <8; i++){
        pSharedPru0->color[i] = 0;
    }

    runCommand("echo stop | sudo tee /sys/class/remoteproc/remoteproc1/state");
    // Cleanup
    freePruMmapAddr(pPruBase);
}

//gets the value of the right joystick 1 if pressed
int rightJoyStickValue(){
    if(pSharedPru0->isRightButtonPressed == 0){
        return 1;
    }
    else{
        return 0;
    }
}

//gets the value of the down joystick 1 if pressed
int downJoyStickValue(){
    if(pSharedPru0->isDownButtonPressed == 0){
        return 1;
    }
    else{
        return 0;
    }
}
//set the NeoPixel Value 0 is the most bottom led, must send array size of 8
void setNeoPixelValue(int* values){
    for(int i = 0; i < 8; i++){
      
      
        pSharedPru0->color[i] = values[i];
      
        
    }
}

