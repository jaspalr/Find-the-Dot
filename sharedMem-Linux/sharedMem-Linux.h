#ifndef sharedMem_Linux
#define sharedMem_Linux

//start the joystick and the neopixel
void PRUinit();

//stops the joystick and the neopixel and free the pru shared memory
void PRUstop();

//gets the value of the right joystick 1 if pressed
int rightJoyStickValue();

//gets the value of the down joystick 1 if pressed
int downJoyStickValue();

//set the NeoPixel Value 0 is the most bottom led, must send array size of 8
void setNeoPixelValue(int* values);

#endif
