#ifndef _SHARED_DATA_STRUCT_H_
#define _SHARED_DATA_STRUCT_H_

#include <stdbool.h>
#include <stdint.h>


// WARNING:
// Fields in the struct must be aligned to match ARM's alignment
//    bool/char, uint8_t:   byte aligned
//    int/long,  uint32_t:  word (4 byte) aligned
//    double,    uint64_t:  dword (8 byte) aligned
// Add padding fields (char _p1) to pad out to alignment.

// My Shared Memory Structure
// ----------------------------------------------------------------
typedef struct {
    // 1 byte
    
    // 1 byte
    bool isRightButtonPressed;
    
    bool isDownButtonPressed;
    // 2 byte short (2 byte aligned)
    

    // // Padding
    // char _p0;
     char _p1, _p2,p3,p5,p6,p7;

    uint32_t color[8];

    // Must be dword aligned (multiple of 8)
               // Works!
    // _Alignas(uint64_t) uint64_t numMsSinceBigBang;      // Fails!
    
} sharedMemStruct_t;

#endif
