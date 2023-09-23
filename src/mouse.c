#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#include <libevdev/libevdev.h>

#include "../include/mouse.h"
#include "../include/setup.h"
#include "../include/event_loop.h"

//        s
//      (o5o)
//     /7(=)\_
// VermouthMoth

static void move_pointer(void)
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
   if (keydown_flags[POINTER_SPEEDUP_KEY])
      value = POINTER_MOVEMENT_SPEED * POINTER_SPEEDUP_FACTOR;
   else
      value = POINTER_MOVEMENT_SPEED;

   if (keydown_flags[POINTER_UP_KEY]
    && keydown_flags[POINTER_RIGHT_KEY])      // up-right
   {
      // prevent diagonal movement from being faster
      // than vertical or horizontal movement
      value = (int) value / sqrt(2);
      uinput_write_event(EV_REL, REL_Y, -value);
      uinput_write_event(EV_REL, REL_X, +value);
   }
   else if (keydown_flags[POINTER_RIGHT_KEY]
         && keydown_flags[POINTER_DOWN_KEY])  // right-down
   {
      value = (int) value / sqrt(2);
      uinput_write_event(EV_REL, REL_X, +value);
      uinput_write_event(EV_REL, REL_Y, +value);
   }
   else if (keydown_flags[POINTER_DOWN_KEY]
         && keydown_flags[POINTER_LEFT_KEY])  // down-left
   {
      value = (int) value / sqrt(2);
      uinput_write_event(EV_REL, REL_Y, +value);
      uinput_write_event(EV_REL, REL_X, -value);
   }
   else if (keydown_flags[POINTER_LEFT_KEY]
         && keydown_flags[POINTER_UP_KEY])    // left-up
   {
      value = (int) value / sqrt(2);
      uinput_write_event(EV_REL, REL_X, -value);
      uinput_write_event(EV_REL, REL_Y, -value);
   }
   else
   {
      // stop when two keys in opposite directions are pressed
      // at the same time
      if (keydown_flags[POINTER_UP_KEY])      // up
         uinput_write_event(EV_REL, REL_Y, -value);
      if (keydown_flags[POINTER_RIGHT_KEY])   // right
         uinput_write_event(EV_REL, REL_X, +value);
      if (keydown_flags[POINTER_DOWN_KEY])    // down
         uinput_write_event(EV_REL, REL_Y, +value);
      if (keydown_flags[POINTER_LEFT_KEY])    // left
         uinput_write_event(EV_REL, REL_X, -value);
   }
}

static void scroll(void)
{
   // on the monitor,
   //
   //  ^ REL_WHEEL_HI_RES > 0
   //  |
   //  |
   //  |
   //  U
   // L+R-------> REL_HWHEEL_HI_RES > 0
   //  D
   //

   int value;
   if (keydown_flags[SCROLLING_SPEEDUP_KEY])
      value = SCROLLING_SPEED * SCROLLING_SPEEDUP_FACTOR;
   else
      value = SCROLLING_SPEED;

   if (keydown_flags[SCROLLING_UP_KEY])
      uinput_write_event(EV_REL, REL_WHEEL_HI_RES, +value);
   if (keydown_flags[SCROLLING_DOWN_KEY])
      uinput_write_event(EV_REL, REL_WHEEL_HI_RES, -value);
   if (keydown_flags[SCROLLING_RIGHT_KEY])
      uinput_write_event(EV_REL, REL_HWHEEL_HI_RES, +value);
   if (keydown_flags[SCROLLING_LEFT_KEY])
      uinput_write_event(EV_REL, REL_HWHEEL_HI_RES, -value);
}

_Noreturn void *mouse_loop(void *arg)
{
   for (;;)
   {
      if (keydown_flags[POINTER_MODE_KEY])
         move_pointer();
      if (keydown_flags[SCROLLING_MODE_KEY])
         scroll();

      // equivalent to a polling rate of 125 Hz
      usleep(8000); // [us]
   }
}
