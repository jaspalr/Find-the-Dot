#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "utilities/sleep.h"
#include "utilities/writeToFile.h"
#include "utilities/runCommand.h"
#define buzzerenable "/dev/bone/pwm/0/a/enable"
#define buzzerduty_cycle "/dev/bone/pwm/0/a/duty_cycle"
#define buzzerperiod "/dev/bone/pwm/0/a/period"

static int soundToplay = 0;
static bool run;
static pthread_t buzzerthread;

static void playsound1(){
    writetofile(buzzerduty_cycle, "0");
    writetofile(buzzerperiod, "1000000");
    writetofile(buzzerduty_cycle, "500000");
    writetofile(buzzerenable, "1");
    sleepForMs(500);
    writetofile(buzzerenable, "0");
}

static void playsound2(){
    writetofile(buzzerduty_cycle, "0");
    writetofile(buzzerperiod, "10000000");
    writetofile(buzzerduty_cycle, "50000");
    writetofile(buzzerenable, "1");
    sleepForMs(500);
    writetofile(buzzerenable, "0");
}

static void* playsound(){
    while(run){
        if(soundToplay == 1){
            playsound1();
            soundToplay = 0;
        }
        else if(soundToplay == 2){
            playsound2();
            soundToplay = 0;
        }
    }
    return NULL;
}

void buzzerstart(){
    runCommand("sudo config-pin p9_22 pwm");
    run = true;
  
    pthread_create(&buzzerthread, NULL, playsound, NULL);
}

void buzzercleanup(){
    writetofile(buzzerenable, "0");
    run = false;
    pthread_join(buzzerthread,NULL);
   
}

void changeSound(int i){
    soundToplay = i;

}


