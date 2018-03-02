// Script containing logic on buttons and controls triggering OWI robotic arm motion.
// Gamepad events of the Logitech Gamepad F310 trigger motion of OWI robotic arm motors and the light bulb.
//
// config: R2:                           moveBase ->
//         L2:                           moveBase <-
//         right joystick rightwards:    moveShoulder ^
//         right joystick leftwards:     moveShoulder v
//         left joystick rightwards:     moveElbow ^
//         left joystick leftwards:      moveElbow v
//         arrow right:                  moveWrist ^
//         arrow left:                   moveWrist v
//         R1 moveGrip:                  open
//         L1 moveGrip:                  close
//
// Information on Logitech Gamepad F310: https://www.logitech.com/assets/35017/gamepad-f310-gsw.pdf

#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include "OWIUSBConnectionOne.hpp"
#include <fcntl.h>    /* For O_RDWR, O_RDNONLY.. */
#include <string.h>
#include <cstring>
#include <sstream>

int main(int argc, char **argv)
{
    OWIUSBConnection OUC;
    struct libevdev *dev = NULL;
    int fd;
    int rc = 1;

    const char *F310 = "Logitech Gamepad F310"; 
    for (int i = 10; i < 20; i++){

        std::string a = "/dev/input/event";
        std::string s = std::to_string(i);
        std::string str = a + s;
        const char * eventPath = str.c_str();
        std::cout << eventPath << std::endl;

        fd = open(eventPath, O_RDONLY|O_NONBLOCK);
        rc = libevdev_new_from_fd(fd, &dev);
        if (rc == 0){
        const char* name = libevdev_get_name(dev);
        std::cout << name << std::endl;
        if(*libevdev_get_name(dev) == *F310){
            break;
        }
        }
            
    }
    //fd = open("/dev/input/event16", O_RDONLY|O_NONBLOCK);
    //rc = libevdev_new_from_fd(fd, &dev);
    //const char *a = libevdev_get_name(dev);
    //printf("%s",a);

    if (rc < 0) {
            fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
            exit(1);
    }
    printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
//     printf("Input device ID: bus %#x vendor %#x product %#x\n",
//             libevdev_get_id_bustype(dev),
//             libevdev_get_id_vendor(dev),
//             libevdev_get_id_product(dev));
    
    while (true){
        struct input_event ev;
        const char* code;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0){
                 printf("Input event: %s %s %d\n",
                 libevdev_event_type_get_name(ev.type),
                 libevdev_event_code_get_name(ev.type, ev.code),
                 ev.value);
                code = libevdev_event_code_get_name(ev.type, ev.code);

                const char* eventCode = code;

                std::cout << "code: " << ev.code << std::endl;
                std::cout << "value: " << ev.value << std::endl;
                std::cout << "type: " << ev.type << std::endl;
                
                if(ev.code == 310){ //L1
                    if (ev.value == 1){
                        OUC.gripMotion(1);
                    }
                    else
                    {
                        OUC.gripMotion(0);
                    }
                }
                else if(ev.code == 311){ //R1
                    if (ev.value == 1){
                        OUC.gripMotion(2);
                    }
                    else
                    {
                        OUC.gripMotion(0);
                    }
                }
                else if(ev.code == 308 && ev.value == 1){ // Y
                    OUC.setLightSwitch();
                }
                else if(ev.code == 2){ // ABS_Z L2
                    if(ev.value > 1){
                        OUC.baseMotion(1);
                    }
                    else{
                        OUC.baseMotion(0);
                    }
                }
                else if(ev.code == 5){ // ABS_RZ R2
                    if(ev.value > 1){
                        OUC.baseMotion(2);
                    }
                    else{
                        OUC.baseMotion(0);
                    }
                }
                else if(ev.code == 0 && ev.type == 3){ // ABS_C left/right
                    if (-1000 < ev.value && ev.value < 1000){
                        OUC.elbowMotion(0);
                    }
                    else if (ev.value > 1000){
                        OUC.elbowMotion(1);
                    }
                    else if (ev.value < -1000){
                        OUC.elbowMotion(2);
                    }
                }
                else if(ev.code == 3 && ev.type == 3){ // ABS_RX left/right
                    if (-1000 < ev.value && ev.value < 1000){
                        OUC.shoulderMotion(0);
                    }
                    else if (ev.value > 1000){
                        OUC.shoulderMotion(1);
                    }
                    else if (ev.value < -1000){
                        OUC.shoulderMotion(2);
                    }
                }
                else if(ev.code == 17){ // ABS_HAT0Y left/right
                    if(ev.value > 0){
                        OUC.wristMotion(2);
                    }
                    else if(ev.value < 0){
                        OUC.wristMotion(1);
                    }
                    else{
                        OUC.wristMotion(0);
                    }
                }

                




                // compare contents
                // if(*char1 == *char2){
                // if(*ev.code == ){
                //     if (ev.value == 1){
                //         std::cout << "BTN TL 1" << std::endl;
                //         OUC.setLightSwitch();
                //     } 
                //     else {
                //         std::cout << "halloooo!!! BTN TL 0" << std::endl;
                //         OUC.setLightSwitch();
                //     }
                // }
    }
}



//     if (!libevdev_has_event_type(dev, EV_REL) ||
//         !libevdev_has_event_code(dev, EV_KEY, BTN_LEFT)) {
//             printf("This device does not look like a mouse\n");
//             exit(1);
//     }

//     do {
//             struct input_event ev;
//             rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
//             if (rc == 0)
//                     printf("Event: %s %s %d\n",
//                             libevdev_event_type_get_name(ev.type),
//                             libevdev_event_code_get_name(ev.type, ev.code),
//                             ev.value);
//     } while (rc == 1 || rc == 0 || rc == -EAGAIN);
}