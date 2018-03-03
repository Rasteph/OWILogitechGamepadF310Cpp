#ifndef __OWIUSBConnectionOne__
#define __OWIUSBConnectionOne__

#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cassert>
#include <stdbool.h>

#define VENDOR       0x1267
#define PRODUCT      0x0000
#define TIMEOUT       1000
#define CMD_DATALEN      3

class OWIUSBConnection
{
    public:
        OWIUSBConnection();
        ~OWIUSBConnection();

        void controlTransfer(unsigned char cmd[3]);

        unsigned char setupCommand();

        void stopMotion();
        void baseMotion(char motionDirection);
        void shoulderMotion(char motionDirection);
        void wristMotion(char motionDirection);
        void gripMotion(char motionDirection);
        void elbowMotion(char motionDirection);
        void setLightSwitch();
        int getLightSwitch();

    private:
        int base;
        int shoulder;
        int elbow;
        int grip;
        int lightSwitch;
        int wrist;
        bool switchLight;
        struct libusb_device_handle *devh;
        libusb_device *dev;


};

#endif