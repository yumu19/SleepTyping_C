#include <stdio.h>
#include <usb.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

/*
static unsigned char keycode[256] = {
  0,0,0,0,'a','b','c','d','e','f', //0-9
  'g','h','i','j','k','l','m','n','o','p', //10-19
  'q','r','s','t','u','v','w','x','y','z', //20-29
  '1','2','3','4','5','6','7','8','9','0', //30-39
  'Enter','Esc','Bksp','Tab','Space',0,0,'@','[',0, //40-49
  ']',';','*','hz','<','>','/','Caps','F1','F2', //50-59
  'F3','F4','F5','F6','F7','F8','F9','F10','F11','F12', //60-69
  'PrtSc','ScLk','PsBk','Ins','Home','PgUp','Del','End','PgDn','right', //7
  'left','down','up','NumLock','n/','n*','n-','n+','nEnt','n1', //8
  'n2','n3','n4','n5','n6','n7','n8','n9','n0','n.', //9
  0,'Mark',0,0,0,0,0,0,0,0, //10
  0,0,0,0,0,0,0,0,0,0, //11
  0,0,0,0,0,0,0,0,0,0, //12
  0,0,0,0,0,0,0,'¥','MHR','MHL', //13
  0,0,0,0,0,0,0,0,0,0, //14
  0,0,0,0,0,0,0,0,0,0, //15
  0,0,0,0,0,0,0,0,0,0, //16
  0,0,0,0,0,0,0,0,0,0, //17
  0,0,0,0,0,0,0,0,0,0, //18
  0,0,0,0,0,0,0,0,0,0, //19
  0,0,0,0,0,0,0,0,0,0, //20
  0,0,0,0,0,0,0,0,0,0, //21
  0,0,0,0,0,0,0,0,0,0, //22
  0,0,0,0,0,0,0,0,0,0, //23
  0,0,0,0,0,0,0,0,0,0, //24
  0,0,0,0,0,0, //25
};
*/

static int keyX[256] = {
  0,0,0,0,26,81,57,49,48,61, //0-9
  72,84,106,96,107,119,104,93,117,129, //10-19
  25,59,28,71,94,69,36,46,83,35, //20-29
  19,31,42,54,65,77,88,100,112,124, //30-39
  167,8,169,11,75,0,0,141,152,0, //40-49
  154,130,142,8,116,127,139,10,32,43, //50-59
  55,66,83,95,106,128,135,147,158,169, //60-69
  185,197,208,185,197,208,185,197,208,202, //70-79
  191,197,197,224,235,247,258,258,258,224, //80-89
  235,247,224,235,247,224,235,247,230,247, //90-99
  0,155,0,0,0,0,0,0,0,0, //100-109
  0,0,0,0,0,0,0,0,0,0, //110-119
  0,0,0,0,0,0,0,0,0,0, //120-129
  0,0,0,0,0,0,0,158,108,52, //130-139
  0,0,0,0,0,0,0,0,0,0, //140-149
  0,0,0,0,0,0,0,0,0,0, //150-159
  0,0,0,0,0,0,0,0,0,0, //160-169
  0,0,0,0,0,0,0,0,0,0, //170-179
  0,0,0,0,0,0,0,0,0,0, //180-189
  0,0,0,0,0,0,0,0,0,0, //190-199
  0,0,0,0,0,0,0,0,0,0, //200-209
  0,0,0,0,0,0,0,0,0,0, //210-219
  0,0,0,0,0,0,0,0,0,0, //220-229
  0,0,0,0,0,0,0,0,0,0, //230-239
  0,0,0,0,0,0,0,0,0,0, //240-249
  0,0,0,0,0,0 //250-255
};

static int keyY[256] = {
   0, 0, 0, 0,31,20,20,31,43,31, //0-9
  31,31,43,31,31,31,20,20,43,43, //10-19
  43,43,31,43,43,20,43,20,43,20, //20-29
  54,54,54,54,54,54,54,54,54,54, //30-39
  37,73,54,43, 9, 0, 0,43,43, 0, //40-49
  31,31,31,54,20,20,20,31,73,73, //50-59
  73,73,73,73,73,73,73,73,73,73, //60-69
  54,54,54,43,43,43,31,31,31,15, //70-79
  15, 9,20,54,54,54,54,37,15,20, //80-89
  20,20,31,31,31,43,43,43, 9, 9, //90-99
   0, 9, 0, 0, 0, 0, 0, 0, 0, 0, //100-109
  0,0,0,0,0,0,0,0,0,0, //110-119
  0,0,0,0,0,0,0,0,0,0, //120-129
  0,0,0,0,0,0,0,54,9,9, //130-139
  0,0,0,0,0,0,0,0,0,0, //140-149
  0,0,0,0,0,0,0,0,0,0, //150-159
  0,0,0,0,0,0,0,0,0,0, //160-169
  0,0,0,0,0,0,0,0,0,0, //170-179
  0,0,0,0,0,0,0,0,0,0, //180-189
  0,0,0,0,0,0,0,0,0,0, //190-199
  0,0,0,0,0,0,0,0,0,0, //200-209
  0,0,0,0,0,0,0,0,0,0, //210-219
  0,0,0,0,0,0,0,0,0,0, //220-229
  0,0,0,0,0,0,0,0,0,0, //230-239
  0,0,0,0,0,0,0,0,0,0, //240-249
  0,0,0,0,0,0 //250-255
};

void
print_input(unsigned char *buf, uint16_t size, int dev_num)
{
        int i;
        time_t tm = time(NULL);
        double tmd = (double)tm;
        double usec;

        struct timeval tv;
        gettimeofday(&tv,NULL);
        usec = (double)tv.tv_usec/1000000.0;


        if (size < 3) return;
        if (buf[2] == 0x01) return;

        for (i=2; i<size; i++) {
                if (buf[i])
                        printf("%lf,%d,%d,%d\n",tmd+usec,keyX[buf[i]],keyY[buf[i]],dev_num);
        }
}

int
usb_keyboard_test(struct usb_device *dev, int dev_num)
{
        usb_dev_handle *handle;
        struct usb_config_descriptor *config = &dev->config[0];
        struct usb_interface *interface = &config->interface[0];
        struct usb_interface_descriptor *altsetting = &interface->altsetting[0];
        struct usb_endpoint_descriptor *endpoint = &altsetting->endpoint[0];

        uint8_t ep = endpoint->bEndpointAddress;

        unsigned char buf[256];
        unsigned char bufprd[256];
        ssize_t read_size;
        int ignore = 1;
        int usbclaim;

        handle = usb_open(dev);
        usb_get_string_simple(handle, dev->descriptor.iProduct,
                              (char *)bufprd, sizeof(bufprd));

        printf("USB-DEV: 0x%04x/0x%04x \"%s\"\n",
               dev->descriptor.idVendor, dev->descriptor.idProduct, bufprd);
        if (usb_set_configuration(handle, config->bConfigurationValue) < 0) {
                if (usb_detach_kernel_driver_np(handle,
                                                altsetting->bInterfaceNumber) < 0) {
                        printf("usb_set_configuration() error.\n");
                        usb_close(handle);
                        return -1;
                }
        }

        usbclaim = usb_claim_interface(handle, altsetting->bInterfaceNumber);

        while (1) {
                read_size = usb_interrupt_read(handle, ep, (char *)buf,
                                               endpoint->wMaxPacketSize, 1000);

                /* if (read_size < 0) {
                        if (read_size == -ETIMEDOUT)
                                printf("read timeout.\n");
                        else
                                printf("read error: %zd\n", read_size);
                } */

                if (!ignore || buf[2])

                        print_input(buf, endpoint->wMaxPacketSize, dev_num);

                if (buf[2] != 0) {
                        ignore = 0;
                      }
                else {
                        ignore = 1;
                      }
   //             if (buf[2] == 0x29)
   //                     break;
        }

        usb_resetep(handle, ep);
        usb_release_interface(handle, altsetting->bInterfaceNumber);

         usb_close(handle);
         return 0;
 }

 int
 check_keyboard(struct usb_device *dev)
 {
         struct usb_config_descriptor *config = &dev->config[0];
         struct usb_interface *interface = &config->interface[0];
         struct usb_interface_descriptor *altsetting = &interface->altsetting[0];

         if (altsetting->bInterfaceClass == 3 &&
             altsetting->bInterfaceSubClass == 1 &&
             altsetting->bInterfaceProtocol == 1)
                 return 1;
         return 0;
 }

 int main(int argc, char *argv[])
 {
         struct usb_bus *bus;
         struct usb_device *dev;

         int dev_num = atoi(argv[1]);
	 int count = 0;

         usb_init();

         if (!usb_get_busses()) {
                 usb_find_busses();
                 usb_find_devices();
         }

         for (bus = usb_get_busses(); bus; bus = bus->next) {
                 for (dev = bus->devices; dev; dev = dev->next) {
                         if (dev->descriptor.idVendor != 0 || dev->descriptor.idProduct != 0) {
                                 if (check_keyboard(dev)) {
					if (count == dev_num) {
                                            usb_keyboard_test(dev,dev_num);
                                            return 0;
					}
					count++;
                                 }
                         }
                 }
         }
     return 0;
 }
