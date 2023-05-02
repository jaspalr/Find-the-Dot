#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sharedMem-Linux.h"
#include <math.h>
#include "accelerometer.h"

#include "buzzer.h"
#include "joystickcontrols.h"
#include "finddot.h"
#include "display.h"

int main(){
    PRUinit();
    buzzerstart();
    accelerometerInit();
    initI2cBus();
    joystickstart();
    findDot();
    joystickcleanup();


    

    

    
}
