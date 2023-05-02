#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "utilities/sleep.h"
#include "sharedMem-Linux.h"
#include "display.h"
#include "shutDown.h"
#include "buzzer.h"
#include "finddot.h"




static bool run;
static pthread_t joystickthread;
static int hits = 0;
static double getRandomFloat(){
    return ((float)rand()/RAND_MAX) - 0.5;
}

static void* joystick(){
    
    while(run){
        if(downJoyStickValue() == 1){
            if(gethit()){
                changehit();
                changeSound(1);
                hits++;
                changenum(hits);
                changex(getRandomFloat());
                changey(getRandomFloat());



            }
            else{
                changeSound(2);
            }
            sleepForMs(100);
        }
        else if(rightJoyStickValue() == 1){
            shutdown();
            run = false;
            sleepForMs(100);
        }             
    }
    return NULL;

}
//start the joystick thread
void joystickstart(){
    run = true;
    pthread_create(&joystickthread, NULL, joystick, NULL);
}
//ends the joystick thread
void joystickcleanup(){
    run = false;
    pthread_join(joystickthread,NULL);
   
}

