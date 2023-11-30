# Find the Dot
Uses an accelerometer connected to the BeagleBone to find the correct orientation the board should be in. \
The neo pixel tells the user what orientation the board should be.\
Then the user presses down on the joystick to record a hit or miss. Buzzer is used to tell the user if they were successful or not using the PWM on the BeagleBone. 
## To Run:
&nbsp; &nbsp; &nbsp; Type "Make" into the host's terminal \
&nbsp; &nbsp; &nbsp; Then on the beagleBone type “cd /mnt/remote/pru/sharedMem-PRU” \
&nbsp; &nbsp; &nbsp; Then “make” \
&nbsp; &nbsp; &nbsp; Then “make install_PRU0” \
&nbsp; &nbsp; &nbsp; Then “cd /mnt/remote/myApps/” \
&nbsp; &nbsp; &nbsp; Then “sudo ./find-the-dot”

## Video Demo


https://user-images.githubusercontent.com/105681721/235599004-486211f3-38f2-4817-a7c3-d9a9d89ee71c.MOV

