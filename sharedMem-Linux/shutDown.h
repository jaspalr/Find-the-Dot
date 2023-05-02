//shutdown the program
#ifndef shutdown_h
#define shutdown_h

#include "buzzer.h"
#include "accelerometer.h"
#include "sharedMem-Linux.h"
#include "finddot.h"


void shutdown(){
    stop();
    buzzercleanup();
    accelerometerStop();
    exitI2cBus();
    PRUstop();
   


}

#endif
