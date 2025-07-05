# Bluetooth-game-pad
This project turns an ESP32 DevKit into a fully functional Bluetooth gamepad using the BleGamepad library

I recently finished a fun project where I built my very own low-cost Bluetooth joystick controller for 
gaming using an ESP32 board! I put a lot of effort into soldering the components onto a PCB prototype 
board and connected buttons and dual analog joysticks to create a custom controller that's really 
effective. 

To make things more interesting, I used the ESP32-BLE-Gamepad library by lemmingDev to turn my 
ESP32 into a full-featured game controller. I also integrated the ESP32-BLE-Mouse library by T-vK to 
add mouse functionality, allowing me to simulate a left-click with a physical button (which I assigned 
to the 'Select' button). 

This project was more than just a hobby for me - it was a fantastic learning experience! I got to dive 
deep into Bluetooth HID communication, improve my understanding of ESP32 programming, explore 
input device emulation, and sharpen my soldering skills. 

If you're curious about how everything works or want to give it a shot yourself, I've documented the 
circuit diagram, shared some images, and included the Arduino code in my repository. Feel free to 
check it out and try building your own version! 

Reference Libraries: 
ESP32-BLE-Gamepad by lemmingDev 
ESP32-BLE-Mouse by T-vK
