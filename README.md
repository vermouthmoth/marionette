# marionette
`marionette` is a C program to emulate mouse with keyboard on Linux using `libevdev`.  
For `libevdev`, see [https://www.freedesktop.org/wiki/Software/libevdev/](https://www.freedesktop.org/wiki/Software/libevdev/).

This program consists of the following three files.
- `marionette.c`: main source file
- `config.h`: config file
- `MakeFile`: makefile

# Building
```
$ git clone https://github.com/vermouthmoth/marionette.git
$ cd marionette
$ make
```
`MakeFile` also supports `clean` target.

# Configuration
Configuration is done using the `config.h`.  
The file is actually just a header file.  
The configuration items and their values are written in the form of `#define item value`.  
The configuration values are changed by editing `value`.  
Below are the configuration items, their default values, and descriptions.
Item | Default value | Description
--- | --- | ---
DEVICE | "/path/to/your/keyboard" | Path to your keyboard under `/dev/input`
POINTER_MODE_KEY | KEY_RIGHTCTRL | Key to enter pointer mode
UP_KEY | KEY_W | Key to move the pointer up (in pointer mode)<br>Key to scroll up (in scrolling mode)
DOWN_KEY | KEY_S | Key to move the pointer down (in pointer mode)<br>Key to scroll down (in scrolling mode)
RIGHT_KEY | KEY_D | Key to move the pointer right (in pointer mode)<br>Key to scroll right (in scrolling mode)
LEFT_KEY | KEY_A | Key to move the pointer left (in pointer mode)<br>Key to scroll left (in scrolling mode)
POINTER_MOVEMENT_SPEED | 3 | Pointer movement speed, must be a positive integer
SPEEDUP_KEY | KEY_SLASH | Key that increases the speed of the pointer only while being pushed down
SPEEDUP_FACTOR | 3 | Pointer movement speed is multiplied by this value
MOUSE_LEFT_BUTTON | KEY_COMMA | Key to left click (in pointer mode)
MOUSE_RIGHT_BUTTON | KEY_DOT | Key to right click (in pointer mode)
MOUSE_MIDDLE_BUTTON | KEY_M | Key to middle click (in pointer mode)
SCROLLING_MODE_KEY | KEY_RIGHTSHIFT | Key to enter scrolling mode
SCROLLING_SPEED | 3 | Scrolling speed, must be a positive integer

For example, if you want to change the key that enters pointer mode from `KEY_RIGHTCTRL` to `KEY_RIGHTALT`, edit as follows.
```
//#define POINTER_MODE_KEY KEY_RIGHTCTRL
#define POINTER_MODE_KEY KEY_RIGHTALT
```
For a list of available key codes, see [here](https://gitlab.freedesktop.org/libevdev/libevdev/-/blob/master/include/linux/linux/input-event-codes.h?ref_type=heads#L75), for example.  
Note that they must be key codes that are actually supported by your keyboard.

Whenever `config.h` is updated, run `make` afterwards.

# Usage
```
$ sudo ./marionette
```
If it starts without error, the following output will appear on the terminal.
```
[I] syspath for uinput device: /sys/devices/virtual/input/inputX
[I] device node for uinput device: /dev/input/eventX

```

Press `POINTER_MODE_KEY` to enter pointer mode.  
In this mode, you can move the pointer with the `UP_KEY`, `DOWN_KEY`, `RIGHT_KEY` and `LEFT_KEY`.  
Pressing `SPEEDUP_KEY` while in this mode will increase the speed of the pointer for as long as you hold it down.  
The `MOUSE_LEFT_BUTTON`, `MOUSE_RIGHT_BUTTON`, and `MOUSE_MIDDLE_BUTTON` allow you to left-click, right-click, and middle-click.

Press `SCROLLING_MODE_KEY` to enter scrolling mode.  
In this mode, you can scroll up and down with the `UP_KEY` and `DOWN_KEY`, and right and left with the `RIGHT_KEY` and `LEFT_KEY`.

In both pointer and scrolling modes, the keyboard does not provide any output as a keyboard.

## License
MIT License Copyright (c) 2023 VermouthMoth

## Contact
`0o.jx_xjzzz` on Discord
___
*This tiny program was originally written just for my own use.*  
*__ANY__ kind of your feedback is sincerely welcome.*  
*It will make this program more useful for more people.*
