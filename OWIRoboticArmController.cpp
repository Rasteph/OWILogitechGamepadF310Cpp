#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cassert>

#define VENDOR       0x1267
#define PRODUCT      0x0000
#define TIMEOUT       1000



 class OWIRoboticarmController{
     public:
        OWIRoboticarmController();
        std::string run();
 };

OWIRoboticarmController::OWIRoboticarmController(){
}

std::string OWIRoboticarmController::run(){
}

static void print_devs(libusb_device **list){
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

        if (desc.idVendor == 0x1267 && desc.idProduct == 0x0000)
            printf ("ha!");
        }
        printf("Vendor: Device = %04x:%04x\n", desc.idVendor, desc.idProduct);
    }
}

int main (int argc, char** argv){
    OWIRoboticarmController cORA;
    printf("hi");
    //cORA.run();

    libusb_device **list = NULL;
    print_devs(list);

    libusb_free_device_list(list, 1);
    libusb_exit(NULL);
}