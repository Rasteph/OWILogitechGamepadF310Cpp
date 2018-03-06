## OWI Robotic Arm controlled by the Logitech F310 Gamepad

[OWI Robotic Arm controlled by the Logitech F310 Gamepad - Click to watch youtube video. <img src="https://github.com/pastelhh/OWILogitechF310Control/blob/master/img/OWILogitechGamepadF310.jpg?raw=true" width="480">](https://www.youtube.com/watch?v=YJlikTFGr-c&feature=youtu.be "OWI Robotic arm controlled by Logitech Gamepad F310 - Click to Watch!")

Used devices:
 - OWI Robotic Arm
 - USB control board (OWI toy robot arm accessory)
 - Logitech F310 Gamepad
 - Linux machine (laptop, raspberry pi, ...)

The Logitech F310 gamepad as well as the OWI Robotic Arm USB control board are connected to a linux machine which handles the communication between both devices.

To read and interpret the events (e.g. Button X pushed) of the Logitech F310 Gamepad, the following libraries are used:
- libevdev-1.0 - libevdev is a wrapper library for evdev devices. https://www.freedesktop.org/wiki/Software/libevdev/
- libusb-1.0 - libusb is a C library that provides generic access to USB devices. http://libusb.info/

C++ files:
- `OWIUSBConnectionOne.hpp` : Header for the application
- `OWIUSBConnectionOne.cpp` : Implements `OWIUSBConnectionOne.hpp`; provides functionality: connection to OWI Robotic Arm and transfer of control data
- `controlOWIUSBRoboticArm` : Contains button and joystick logic; does connect to the Logitech Gamepad F310

 ### What needs to be started to use the Logitech F310 Gampepad as controller for the OWI Robotic Arm:
  1. Clone GitHub project
  2. Compile with e.g `g++ controlOWIUSBRoboticArm.cpp OWIUSBConnectionOne.cpp  $(pkg-config --libs libusb-1.0 libevdev) -o controller.out`  
  3. Connect Logitech Gamepad F310 and OWI Robotic Arm to the Linux machine
  4. run `sudo ./controller.out`
  5. push buttons on the gamepad ;)

