#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "config.h"

//        s
//      (o5o)
//     /7(=)\_
// VermouthMoth
//
// $ gcc $(pkg-config --cflags --libs libevdev) marionette.c -o marionette
// $ sudo ./marionette

int fd;
struct libevdev *dev;
struct libevdev_uinput *uidev;
int keys_count;
bool *keydown_flags;

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

void uinput_write_event(unsigned int type, unsigned int code, int value)
{
   int ret;
   ret = libevdev_uinput_write_event(uidev, type, code, value);
   if (ret < 0)
      cleanup("[E] Failed to write event", ret);

   // tell the device's listeners that the event is coming
   ret = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
   if (ret < 0)
      cleanup("[E] Failed to write event", ret);
}

void check_keydown(struct input_event ev)
{
   if (ev.type == EV_KEY)
   {
      if (ev.value == 1 || ev.value == 2)
      {
         // if the KEY_i is pressed or held down,
         // set the value of the KEY_i-th element to true
         keydown_flags[ev.code] = true;
      }
      else
      {
         // if the KEY_i is released,
         // set the value of the KEY_i-th element to false
         keydown_flags[ev.code] = false;
      }
   }
}

void handle_event(struct input_event ev)
{
   check_keydown(ev);

   if (ev.type == EV_KEY
    && (ev.code == POINTER_MODE_KEY || ev.code == SCROLLING_MODE_KEY)
    && ev.value == 1)
   {
      // only at the moment the key is pressed
      unsigned int MODE_KEY = ev.code;
      for (int i = 0; i < keys_count; i++)
      {
         if (i != MODE_KEY
          && keydown_flags[i] == true)
         {
            // release all the other keys that are pressed down
            // otherwise, those keys will remain pressed
            uinput_write_event(EV_KEY, i, 0);
         }
      }
   }

   if (keydown_flags[POINTER_MODE_KEY]
    || keydown_flags[SCROLLING_MODE_KEY])
   {
      if (keydown_flags[POINTER_MODE_KEY])
      {
         if (ev.type == EV_KEY && ev.code == MOUSE_LEFT_BUTTON)
            uinput_write_event(EV_KEY, BTN_LEFT, ev.value);
         if (ev.type == EV_KEY && ev.code == MOUSE_RIGHT_BUTTON)
            uinput_write_event(EV_KEY, BTN_RIGHT, ev.value);
         if (ev.type == EV_KEY && ev.code == MOUSE_MIDDLE_BUTTON)
            uinput_write_event(EV_KEY, BTN_MIDDLE, ev.value);
      }
   }
   else
   {
      // let the event pass by as it comes
      uinput_write_event(ev.type, ev.code, ev.value);
   }
}

void event_loop(void)
{
   int ret;
   do
   {
      struct input_event ev;
      ret = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL
                                   | LIBEVDEV_READ_FLAG_BLOCKING, &ev);

      if (ret == LIBEVDEV_READ_STATUS_SUCCESS)
      {
         // all right...
         handle_event(ev);
      }
      else if (ret == LIBEVDEV_READ_STATUS_SYNC)
      {
         // several events dropped
         while (ret == LIBEVDEV_READ_STATUS_SYNC)
         {
            // repeat until re-synced
            handle_event(ev);
            ret = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_SYNC, &ev);
         }
      }
   } while (ret == LIBEVDEV_READ_STATUS_SYNC
         || ret == LIBEVDEV_READ_STATUS_SUCCESS
         || ret == -EAGAIN); // no events available at this time

   cleanup("[E] Failed to handle event", ret);
}

void move_pointer(void)
{
   // on the monitor,
   //
   //  U
   // L+R-------> REL_X > 0
   //  D
   //  |
   //  |
   //  |
   //  v REL_Y > 0
   //

   int value;
   if (keydown_flags[SPEEDUP_KEY])
      value = POINTER_MOVEMENT_SPEED * SPEEDUP_FACTOR;
   else
      value = POINTER_MOVEMENT_SPEED;

   if (keydown_flags[UP_KEY]
    && keydown_flags[RIGHT_KEY])      // up-right
   {
      // prevent diagonal movement from being faster
      // than vertical or horizontal movement
      value = (int) value / sqrt(2);
      uinput_write_event(EV_REL, REL_Y, -value);
      uinput_write_event(EV_REL, REL_X, +value);
   }
   else if (keydown_flags[RIGHT_KEY]
         && keydown_flags[DOWN_KEY])  // right-down
   {
      value = (int) value / sqrt(2);
      uinput_write_event(EV_REL, REL_X, +value);
      uinput_write_event(EV_REL, REL_Y, +value);
   }
   else if (keydown_flags[DOWN_KEY]
         && keydown_flags[LEFT_KEY])  // down-left
   {
      value = (int) value / sqrt(2);
      uinput_write_event(EV_REL, REL_Y, +value);
      uinput_write_event(EV_REL, REL_X, -value);
   }
   else if (keydown_flags[LEFT_KEY]
         && keydown_flags[UP_KEY])    // left-up
   {
      value = (int) value / sqrt(2);
      uinput_write_event(EV_REL, REL_X, -value);
      uinput_write_event(EV_REL, REL_Y, -value);
   }
   else
   {
      // stop when two keys in opposite directions are pressed
      // at the same time
      if (keydown_flags[UP_KEY])      // up
         uinput_write_event(EV_REL, REL_Y, -value);
      if (keydown_flags[RIGHT_KEY])   // right
         uinput_write_event(EV_REL, REL_X, +value);
      if (keydown_flags[DOWN_KEY])    // down
         uinput_write_event(EV_REL, REL_Y, +value);
      if (keydown_flags[LEFT_KEY])    // left
         uinput_write_event(EV_REL, REL_X, -value);
   }
}

_Noreturn void *pointer_loop(void *arg)
{
   for (;;)
   {
      if (keydown_flags[POINTER_MODE_KEY])
         move_pointer();

      // equivalent to a polling rate of 125 Hz
      usleep(8000); // [us]
   }
}

void scroll(void)
{
   // difficult to fine-tune speed
   // might be better to use *_HI_RES version for these event codes

   // on the monitor,
   //
   //  ^ REL_WHEEL > 0
   //  |
   //  |
   //  |
   //  U
   // L+R-------> REL_HWHEEL > 0
   //  D
   //

   if (keydown_flags[UP_KEY])
      uinput_write_event(EV_REL, REL_WHEEL, +SCROLLING_SPEED);
   if (keydown_flags[DOWN_KEY])
      uinput_write_event(EV_REL, REL_WHEEL, -SCROLLING_SPEED);
   if (keydown_flags[RIGHT_KEY])
      uinput_write_event(EV_REL, REL_HWHEEL, +SCROLLING_SPEED);
   if (keydown_flags[LEFT_KEY])
      uinput_write_event(EV_REL, REL_HWHEEL, -SCROLLING_SPEED);
}

_Noreturn void *scrolling_loop(void *arg)
{
   for (;;)
   {
      if (keydown_flags[SCROLLING_MODE_KEY])
         scroll();

      // equivalent to a polling rate of 20 Hz
      usleep(50000); // [us]
   }
}

int main(int argc, char *argv[])
{
   /*
   if (argc < 2)
   {
      fprintf(stderr, "[E] No device specified: $ sudo ./marionette device\n");
      exit(EXIT_FAILURE);
   }

   const char *file;
   file = argv[1];
   */
   const char *file;
   file = DEVICE;
   fd = open(file, O_RDONLY);
   if (fd < 0)
   {
      perror("[E] Failed to open device");
      exit(EXIT_FAILURE);
   }

   int ret;
   ret = libevdev_new_from_fd(fd, &dev);
   if (ret < 0)
      cleanup("[E] Failed to initiate libevdev", ret);

   // wait a bit to avoid unnecessary trouble
   // hope all the keys are released during this time...
   usleep(500000); // [us]

   // send all events from device through uinput device,
   // which will be newly created a little later
   ret = libevdev_grab(dev, LIBEVDEV_GRAB);
   if (ret < 0)
      cleanup("[E] Failed to grab device", ret);

   // force enable the following event codes
   // to make uinput device inherit them
   // REL_X, REL_Y, REL_WHEEL, REL_HWHEEL,
   // BTN_LEFT, BTN_RIGHT and BTN_MIDDLE
   ret = libevdev_enable_event_code(dev, EV_REL, REL_X, NULL);
   if (ret < 0)
      cleanup("[E] Failed to enable event code", ret);
   ret = libevdev_enable_event_code(dev, EV_REL, REL_Y, NULL);
   if (ret < 0)
      cleanup("[E] Failed to enable event code", ret);
   ret = libevdev_enable_event_code(dev, EV_REL, REL_WHEEL, NULL);
   if (ret < 0)
      cleanup("[E] Failed to enable event code", ret);
   ret = libevdev_enable_event_code(dev, EV_REL, REL_HWHEEL, NULL);
   if (ret < 0)
      cleanup("[E] Failed to enable event code", ret);
   ret = libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);
   if (ret < 0)
      cleanup("[E] Failed to enable event code", ret);
   ret = libevdev_enable_event_code(dev, EV_KEY, BTN_RIGHT, NULL);
   if (ret < 0)
      cleanup("[E] Failed to enable event code", ret);
   ret = libevdev_enable_event_code(dev, EV_KEY, BTN_MIDDLE, NULL);
   if (ret < 0)
      cleanup("[E] Failed to enable event code", ret);

   ret = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
   if (ret < 0)
      cleanup("[E] Failed to create uinput device", ret);

   const char *uidev_syspath;
   uidev_syspath = libevdev_uinput_get_syspath(uidev);
   printf("[I] syspath for uinput device: %s\n", uidev_syspath);
   const char *uidev_devnode;
   uidev_devnode = libevdev_uinput_get_devnode(uidev);
   printf("[I] device node for uinput device: %s\n", uidev_devnode);

   // create an array with as many elements as key codes
   // manage the current state of each key pressed or not
   // change the values of the elements only in the main thread
   keys_count = libevdev_event_type_get_max(EV_KEY);
   keydown_flags = (bool *)calloc(keys_count, sizeof(bool));

   // create two threads for controlling pointer movement and scrolling
   pthread_t pointer_loop_thread;
   pthread_t scrolling_loop_thread;
   pthread_create(&pointer_loop_thread, NULL, pointer_loop, NULL);
   pthread_create(&scrolling_loop_thread, NULL, scrolling_loop, NULL);
   // no plans to return
   pthread_detach(pointer_loop_thread);
   pthread_detach(scrolling_loop_thread);

   // also no plans to return without failure...
   // just use Ctrl+C or kill command to terminate
   event_loop();
}
