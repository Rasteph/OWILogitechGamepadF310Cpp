// #include <libusb-1.0/libusb.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <string>
// #include <iostream>
// #include <cassert>
// #include <stdbool.h>

// #define VENDOR       0x1267
// #define PRODUCT      0x0000
// #define TIMEOUT       1000
// #define CMD_DATALEN      3

#include "OWIUSBConnectionOne.hpp"

// class OWIUSBConnection{
//     public:
//         OWIUSBConnection();
//         ~OWIUSBConnection();
//         void controlTransfer(unsigned char cmd[3]);

//         unsigned char setupCommand();

//         void stopMotion();
//         void baseMotion(char motionDirection);
//         void shoulderMotion(char motionDirection);
//         void wristMotion(char motionDirection);
//         void gripMotion(char motionDirection);
//         void elbowMotion(char motionDirection);
//         void setLightSwitch(bool switchVal);
//         int getLightSwitch();

//     private:
//         bool switchLight;
//         int base;
//         int shoulder;
//         int elbow;
//         int grip;
//         int lightSwitch;
//         int wrist;
//         struct libusb_device_handle *devh;
        
// };

libusb_device *find_dev(libusb_device **list){
    libusb_context *context = NULL;
    int rc = 0;
    ssize_t count = 0;

    rc = libusb_init(&context);
    assert(rc == 0);

    count = libusb_get_device_list(context, &list);
    assert(count>0);

    for (size_t idx = 0; idx < count; idx++){
        libusb_device *device = list[idx];
        libusb_device_descriptor desc = {0};

        rc = libusb_get_device_descriptor(device, &desc);
        assert(rc == 0);

        if(desc.idVendor == VENDOR && desc.idProduct == PRODUCT){
            std::cout << "here i am: " << std::endl;
            printf("Vendor: Device = %04x:%04x\n", desc.idVendor, desc.idProduct);

		    return device;
		}
        printf("Vendor: Device = %04x:%04x\n", desc.idVendor, desc.idProduct);
    }
    return NULL;
}


OWIUSBConnection::OWIUSBConnection(){
    libusb_device **list;
    libusb_device *dev = find_dev(list);

    devh = NULL;
    int r = libusb_open(dev,&devh);
    if(r!=0)
    {
	    fprintf(stderr, "Error opening device\n");
        libusb_free_device_list(list, 1);
        libusb_exit(NULL);
    }

    base = 0;
    shoulder = 0;
    elbow = 0;
    grip = 0;
    lightSwitch = 0;
    wrist = 0;
    switchLight = false;
    stopMotion();

}

OWIUSBConnection::~OWIUSBConnection(){
    //libusb_close(this->devh);
    //libusb_free_device_list(list, 1);
    //libusb_exit(NULL);
}

void OWIUSBConnection::controlTransfer(unsigned char cmd[3]){
    int actual_length = -1;
    int r = libusb_control_transfer(devh,
                                0x40,   //uint8_t 	    bmRequestType,
                                6,      //uint8_t 	    bRequest,
                                0x100,  //uint16_t 	    wValue,
                                0,      //uint16_t 	    wIndex,
                                cmd,
                                CMD_DATALEN,
                                0	 
    );
    if(!(r == 0 && actual_length >= CMD_DATALEN))
    {
        fprintf(stderr, "Write err %d. len=%d\n",r,actual_length);
    }
}

unsigned char OWIUSBConnection::setupCommand(){
    unsigned char cmd[3];
    cmd[0] = (shoulder << 6) + (elbow << 4) + (wrist << 2) + grip;
    cmd[1] = base;
    cmd[2] = lightSwitch;
    controlTransfer(cmd);
    //return *cmd;

    //cmd[0]=(unsigned char)strtol("00",NULL,16);
    //cmd[1]=(unsigned char)strtol("00",NULL,16);
    //cmd[2]=(unsigned char)strtol("00",NULL,16);
    //controlTransfer(cmd);


    //cmd[0]=(unsigned char)strtol("00",NULL,16);
    //cmd[1]=(unsigned char)strtol("00",NULL,16);
    //cmd[2]=(unsigned char)strtol("00",NULL,16);
    //return cmd;

    //bytes[0] = (self.shoulder << 6) + (self.elbow << 4) + (self.wrist << 2) + self.grip
    //bytes[1] = self.base
    //bytes[2] = self.lightSwitch
    //return bytes

}

void OWIUSBConnection::stopMotion(){
    base = 0;
    shoulder = 0;
    elbow = 0;
    grip = 0;
    if (switchLight == true){
        lightSwitch = 0;
    }
    lightSwitch = 0;
    wrist = 0;
    setupCommand();
}

void OWIUSBConnection::baseMotion(char motionDirection){
    base = motionDirection;
    setupCommand();
}

void OWIUSBConnection::elbowMotion(char motionDirection){
    elbow = motionDirection;
    setupCommand();
}

void OWIUSBConnection::gripMotion(char motionDirection){
    grip = motionDirection;
    setupCommand();
}

void OWIUSBConnection::shoulderMotion(char motionDirection){
    shoulder = motionDirection;
    setupCommand();
}

void OWIUSBConnection::wristMotion(char motionDirection){
    wrist = motionDirection;
    setupCommand();
}

void OWIUSBConnection::setLightSwitch(){
    if (lightSwitch){
        lightSwitch = 0;
    }
    else{
        lightSwitch = 1;
    }
    setupCommand();
}

int OWIUSBConnection::getLightSwitch(){
    return lightSwitch; 
}
