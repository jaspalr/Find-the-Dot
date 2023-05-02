#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sharedMem-Linux.h"
#include <math.h>
#include "accelerometer.h"
#include "utilities/colors.h"
#include "utilities/sleep.h"
#include "buzzer.h"

static bool hit = false;
static bool run = true;
static double xpoint = 0.0;
static double ypoint = 0.0;
bool gethit(){
    return hit;
}

void stop(){
    run = false;
}

double getRandomFloat(){
    return ((float)rand()/RAND_MAX) - 0.5;
}

bool approximatelyEqualRel(double a, double b)
{
	return fabs(a - b) < 0.05? true:false;
}

void changex(double val){
    xpoint = val;
}

void changey(double val){
    ypoint = val;
}
void changehit(){
    hit = false;
}
void findDot(){
    xpoint = getRandomFloat();
    ypoint = getRandomFloat();
    double accval[3] = {0.0,0.0,0.0};
    int colors[8] = {0,0,0,0,0,0,0,0};
    int color = 0;
    while(run){
        accelermeterGetValue(accval);
        if(approximatelyEqualRel(xpoint, accval[0])){
            color = blue;
        }
        else if(xpoint > accval[0]){
            color = green;
        }
        else{
            color = red;
        }

        if(approximatelyEqualRel(ypoint, accval[1])){
            if(color == blue){
                hit = true;
            }
            for(int i = 0; i < 8; i++){
                colors[i] = color;
            }
        }
        else if(ypoint > accval[1]){
            if(fabs(ypoint -accval[1]) < 0.10 ){
                for(int i = 0; i < 8; i++){
                    if(i >4){
                        colors[i] = color;
                    }
                    else{
                        colors[i] = 0.0;
                    }
            }
            }
            else if(fabs(ypoint - accval[1]) < 0.2){
                for(int i = 0; i < 8; i++){
                    if(i >5){
                        colors[i] = color;
                    }
                    else{
                        colors[i] = 0.0;
                    }
                }

            }
            else{
                for(int i = 0; i < 8; i++){
                    if(i >6){
                        colors[i] = color;
                    }
                    else{
                        colors[i] = 0.0;
                    }
                }

            }
             hit = false;
        }
        
        else{
            if(fabs(ypoint - accval[1]) < 0.1){
                for(int i = 0; i < 8; i++){
                    if(i <3){
                        colors[i] = color;
                    }
                    else{
                        colors[i] = 0.0;
                    }
                }
            }
            else if(fabs(ypoint - accval[1]) < 0.2){
                for(int i = 0; i < 8; i++){
                    if(i <2){
                        colors[i] = color;
                    }
                    else{
                        colors[i] = 0.0;
                    }
                }
            }
            else{
                for(int i = 0; i < 8; i++){
                    if(i <1){
                        colors[i] = color;
                    }
                    else{
                        colors[i] = 0.0;
                    }
                }
            }
            hit = false;
        }
     
        setNeoPixelValue(colors);
        sleepForMs(200);
       

    }
}