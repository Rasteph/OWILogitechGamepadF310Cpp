#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cassert>

#define VENDOR       0x1267
#define PRODUCT      0x0000
#define TIMEOUT       1000
#define CMD_DATALEN      3



 class OWIRoboticarmController{
     public:
        OWIRoboticarmController();
        std::string run();
 };

OWIRoboticarmController::OWIRoboticarmController(){
}

std::string OWIRoboticarmController::run(){
}

libusb_device * find_dev(libusb_device **list){
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
		    return device;
		}
        printf("Vendor: Device = %04x:%04x\n", desc.idVendor, desc.idProduct);
    }
    return NULL;
}

int main (int argc, char** argv){
    OWIRoboticarmController cORA;
    printf("hi");
    //cORA.run();

    libusb_device **list = NULL;
    libusb_device *dev;
    struct libusb_device_handle *devh = NULL;
    
    dev = find_dev(list);

    if(!dev)
    {
	    fprintf(stderr, "Robot Arm not found\n");
	    return -1;
    }

    int r = libusb_open(dev,&devh);
    if(r!=0)
    {
	    fprintf(stderr, "Error opening device\n");
        libusb_free_device_list(list, 1);
        libusb_exit(NULL);
	    return -1;
    }

    int actual_length = -1;
    unsigned char cmd[3];

    cmd[0]=(unsigned char)strtol("00",NULL,16);
    cmd[1]=(unsigned char)strtol("00",NULL,16);
    cmd[2]=(unsigned char)strtol("01",NULL,16);

    // cmd[0] = 0;
    // cmd[1] = 0;
    // cmd[2] = 0;

    r = libusb_control_transfer(devh,
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

    libusb_close(devh);
    libusb_free_device_list(list, 1);
    libusb_exit(NULL);
}