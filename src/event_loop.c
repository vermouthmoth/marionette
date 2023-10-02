#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "../include/event_loop.h"
#include "../include/marionette.h"
#include "../include/setup.h"

//        s
//      (o5o)
//     /7(=)\_
// VermouthMoth

#define RELEASE             0
#define PRESS               1
#define HOLD_DOWN           2

#define KEYCODES_HIST_MAX 256

bool *keydown_flags;

unsigned int keycodes_hist[KEYCODES_HIST_MAX];

void uinput_write_event(unsigned int type, unsigned int code, int value)
{
   int ret;
   ret = libevdev_uinput_write_event(uidev, type, code, value);
   if (ret < 0)
      cleanup("[E] failed to write event", ret, false);

   // tell the device's listeners that the event is coming
   ret = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
   if (ret < 0)
      cleanup("[E] failed to write event", ret, false);
}

static void check_keydown(struct input_event ev)
{
   if (ev.type == EV_KEY)
   {
      if (ev.value == RELEASE)
      {
         // if the KEY_i is released,
         // set the value of the KEY_i-th element to false
         keydown_flags[ev.code] = false;
      }
      else
      {
         // if the KEY_i is pressed or held down,
         // set the value of the KEY_i-th element to true
         keydown_flags[ev.code] = true;
      }
   }
}

static void update_history(struct input_event ev)
{
   if ((ev.type == EV_KEY)
    && (ev.value == PRESS))
   {
      // KEY_A, KEY_B, KEY_C, ..., KEY_Y, KEY_Z
      // -> KEY_NEW, KEY_A, KEY_B, ..., KEY_Y
      memmove(&keycodes_hist[1],
              &keycodes_hist[0],
              sizeof(keycodes_hist) - sizeof(keycodes_hist[0]));
      keycodes_hist[0] = ev.code;
   }
}

static void check_termination()
{
   unsigned int kill_magic[] = {KEY_L,
                                KEY_L,
                                KEY_I,
                                KEY_K,
                                POINTER_MODE_KEY};

   int ret = memcmp(&keycodes_hist[0],
                    &kill_magic[0],
                    sizeof(kill_magic));
   if (ret == 0)
      cleanup("[I] terminate the marionette...", 0xC0010FF, true);
}

static void handle_event(struct input_event ev)
{
   check_keydown(ev);
   update_history(ev);
   check_termination();

   if (ev.type == EV_KEY
    && (ev.code == POINTER_MODE_KEY || ev.code == SCROLLING_MODE_KEY))
   {
      if (ev.value == PRESS)
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
               uinput_write_event(EV_KEY, i, RELEASE);
            }
         }
      }

      if (ev.value == RELEASE)
      {
         // prevent the mouse from staying in dragging
         uinput_write_event(EV_KEY, BTN_LEFT, RELEASE);
         uinput_write_event(EV_KEY, BTN_RIGHT, RELEASE);
         uinput_write_event(EV_KEY, BTN_MIDDLE, RELEASE);
      }
   }

   if ((keydown_flags[POINTER_MODE_KEY] || keydown_flags[SCROLLING_MODE_KEY])
    && ev.type == EV_KEY)
   {
      if (keydown_flags[POINTER_MODE_KEY])
      {
         if (ev.code == MOUSE_LEFT_BUTTON)
            uinput_write_event(EV_KEY, BTN_LEFT, ev.value);
         if (ev.code == MOUSE_RIGHT_BUTTON)
            uinput_write_event(EV_KEY, BTN_RIGHT, ev.value);
         if (ev.code == MOUSE_MIDDLE_BUTTON)
            uinput_write_event(EV_KEY, BTN_MIDDLE, ev.value);
      }

      // pass through
      if (keydown_flags[POINTER_MODE_KEY])
      {
         for (int i = 0; i < pass_through_keys_count; i++)
         {
            if ((pass_through_keys[i].mode == POINTER_MODE)
             && (pass_through_keys[i].keycode == ev.code))
               uinput_write_event(ev.type, ev.code, ev.value);
         }
      }
      
      // pass through
      if (keydown_flags[SCROLLING_MODE_KEY])
      {
         for (int i = 0; i < pass_through_keys_count; i++)
         {
            if ((pass_through_keys[i].mode == SCROLLING_MODE)
             && (pass_through_keys[i].keycode == ev.code))
               uinput_write_event(ev.type, ev.code, ev.value);
         }
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

   cleanup("[E] failed to handle event", ret, false);
}
