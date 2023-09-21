#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "../include/marionette.h"
#include "../include/setup.h"
#include "../include/event_loop.h"
#include "../include/mouse.h"

//        s
//      (o5o)
//     /7(=)\_
// VermouthMoth

static int fd;
struct libevdev *dev;
struct libevdev_uinput *uidev;
int keys_count;
static unsigned int const evrels_to_enable[]
               = {REL_X, REL_Y, REL_WHEEL_HI_RES, REL_HWHEEL_HI_RES};
static unsigned int const evkeys_to_enable[]
               = {BTN_LEFT, BTN_RIGHT, BTN_MIDDLE};

void cleanup(char *msg, int ret)
{
   // what about normal termination? i don't care...
   fprintf(stderr, "%s: %s\n", msg, strerror(-ret));

   libevdev_uinput_destroy(uidev);
   libevdev_free(dev);
   close(fd);
   free(keydown_flags);

   exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
   if (argc < 2)
   {
      fprintf(stderr, "[E] no config file specified: $ sudo ./marionette config/your_config.xml\n");
      exit(EXIT_FAILURE);
   }
   char const *config_file = argv[1];

   setup(config_file);

   printf("[I] start opening device: %s\n", DEVICE);
   fd = open(DEVICE, O_RDONLY);
   if (fd < 0)
   {
      perror("[E] failed to open device");
      exit(EXIT_FAILURE);
   }

   int ret;
   ret = libevdev_new_from_fd(fd, &dev);
   if (ret < 0)
      cleanup("[E] failed to initiate libevdev", ret);

   // wait a bit to avoid unnecessary trouble
   // hope all the keys are released during this time...
   usleep(500000); // [us]

   // send all events from device through uinput device,
   // which will be newly created a little later
   ret = libevdev_grab(dev, LIBEVDEV_GRAB);
   if (ret < 0)
      cleanup("[E] failed to grab device", ret);

   // force enable the following event codes
   // to make uinput device inherit them
   for (int i = 0; i < sizeof(evrels_to_enable)/sizeof(unsigned int); i++)
   {
      // REL_X, REL_Y, REL_WHEEL_HI_RES and REL_HWHEEL_HI_RES
      ret = libevdev_enable_event_code(dev, EV_REL,
                                       evrels_to_enable[i], NULL);
      if (ret < 0)
         cleanup("[E] failed to enable event code", ret);
   }
   for (int i = 0; i < sizeof(evkeys_to_enable)/sizeof(unsigned int); i++)
   {
      // BTN_LEFT, BTN_RIGHT and BTN_MIDDLE
      ret = libevdev_enable_event_code(dev, EV_KEY,
                                       evkeys_to_enable[i], NULL);
      if (ret < 0)
         cleanup("[E] failed to enable event code", ret);
   }

   ret = libevdev_uinput_create_from_device(dev,
                                    LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
   if (ret < 0)
      cleanup("[E] failed to create uinput device", ret);

   char const *uidev_syspath;
   uidev_syspath = libevdev_uinput_get_syspath(uidev);
   printf("[I] syspath for uinput device: %s\n", uidev_syspath);
   char const *uidev_devnode;
   uidev_devnode = libevdev_uinput_get_devnode(uidev);
   printf("[I] device node for uinput device: %s\n", uidev_devnode);

   // create an array with as many elements as key codes
   // manage the current state of each key pressed or not
   // change the values of the elements only in the main thread
   keys_count = libevdev_event_type_get_max(EV_KEY);
   keydown_flags = (bool *)calloc(keys_count, sizeof(bool));

   // create a thread for controlling pointer movement and scrolling
   pthread_t mouse_loop_thread;
   pthread_create(&mouse_loop_thread, NULL, mouse_loop, NULL);
   // no plans to return
   pthread_detach(mouse_loop_thread);

   // also no plans to return without failure...
   // just use Ctrl+C or kill command to terminate
   event_loop();
}
