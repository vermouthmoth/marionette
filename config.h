// specify your keyboard
// for example, in my case, i have
//#define DEVICE "/dev/input/by-id/usb-PFU_Limited_HHKB-Hybrid-event-kbd"
#define DEVICE "/path/to/your/keyboard"

// to change the key settings, edit KEY_*
// for example,
// if you want the key that activates pointer mode to be "rightalt", then
//#define POINTER_MODE_KEY KEY_RIGHTALT
// for a list of available key codes, see, for example,
// https://gitlab.freedesktop.org/libevdev/libevdev/-/blob/master/include/linux/linux/input-event-codes.h?ref_type=heads#L75

#define POINTER_MODE_KEY KEY_RIGHTCTRL
#define UP_KEY KEY_W
#define DOWN_KEY KEY_S
#define RIGHT_KEY KEY_D
#define LEFT_KEY KEY_A
#define POINTER_MOVEMENT_SPEED 2 // must be a positive integer
#define SPEEDUP_KEY KEY_SLASH
#define SPEEDUP_FACTOR 5 // must be a positive integer

#define MOUSE_LEFT_BUTTON KEY_COMMA
#define MOUSE_RIGHT_BUTTON KEY_DOT
#define MOUSE_MIDDLE_BUTTON KEY_M

#define SCROLLING_MODE_KEY KEY_RIGHTSHIFT
#define SCROLLING_SPEED 1 // must be a positive integer
