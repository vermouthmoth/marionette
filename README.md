# marionette
`marionette` is a C program to emulate mouse with keyboard on Linux using `libevdev`.

The following features are available.
- Moving the pointer
- Mouse clicks
- Scrolling

This program creates a virtual keyboard exactly the same as your keyboard, but adds some mouse functions.  
The virtual keyboard then outputs the input events from the original keyboard in a form that basically passes through as is, and converts them to mouse events as needed.

> [!NOTE]  
> An quick overview , from downloading to running, will be as follows.
> ```
> $ git clone https://github.com/vermouthmoth/marionette.git
> $ cd marionette
> $ make
>
> $ ./gen_config > config.xml
> $ your_favorite_editor config.xml
>
> $ sudo ./marionette config.xml
> ```

This program relies on `libevdev`, which stands between the kernel and other subsequent programs that handle input events.  
Thus, it works **no matter what window system** you are using, e.g. `X Window System` or `Wayland`.  
It even works on a non-graphical console.  
For `libevdev`, see [https://www.freedesktop.org/wiki/Software/libevdev/](https://www.freedesktop.org/wiki/Software/libevdev/).

This program uses XML file for config file.  
To handle XML format, `libxml2` is used.  
For `libxml2`, see [https://gitlab.gnome.org/GNOME/libxml2/-/wikis/](https://gitlab.gnome.org/GNOME/libxml2/-/wikis/).

This program consists of the following directories and files.
- `src/`: direcotory containing source files
- `include/`: directory containing header files
- `config/`: directory containing `config.dtd`
- `MakeFile`: makefile

## Building
```
$ git clone https://github.com/vermouthmoth/marionette.git
$ cd marionette
$ make
```
Two executable binary files `marionette` and `gen_config` will be created.  
`MakeFile` also supports `clean` target.

## Configuration
Configuration file is written in XML format.  
Valid format rules are shown in `config/config.dtd`.  
`marionette` validates if the format of a given config file is valid using the DTD.

Config file template can be created using `gen_config`.
```
$ ./gen_config > config.xml
```
The name of the config file can be anything.

The content of the config file template is as follows.
```xml
$ cat config.xml
<?xml version="1.0" encoding="ASCII"?>
<!DOCTYPE config SYSTEM "config.dtd">
<config>
  <device>
    <DEVICE>/path/to/your/keyboard</DEVICE>
  </device>
  <pointer_mode>
    <POINTER_MODE_KEY>KEY_RIGHTCTRL</POINTER_MODE_KEY>
    <UP_KEY>KEY_W</UP_KEY>
    <DOWN_KEY>KEY_S</DOWN_KEY>
    <RIGHT_KEY>KEY_D</RIGHT_KEY>
    <LEFT_KEY>KEY_A</LEFT_KEY>
    <POINTER_MOVEMENT_SPEED>2</POINTER_MOVEMENT_SPEED>
    <POINTER_SPEEDUP_KEY>KEY_SLASH</POINTER_SPEEDUP_KEY>
    <POINTER_SPEEDUP_FACTOR>5</POINTER_SPEEDUP_FACTOR>
    <MOUSE_LEFT_BUTTON>KEY_COMMA</MOUSE_LEFT_BUTTON>
    <MOUSE_RIGHT_BUTTON>KEY_DOT</MOUSE_RIGHT_BUTTON>
    <MOUSE_MIDDLE_BUTTON>KEY_M</MOUSE_MIDDLE_BUTTON>
  </pointer_mode>
  <scrolling_mode>
    <SCROLLING_MODE_KEY>KEY_RIGHTSHIFT</SCROLLING_MODE_KEY>
    <SCROLLING_SPEED>25</SCROLLING_SPEED>
    <SCROLLING_SPEEDUP_KEY>KEY_SLASH</SCROLLING_SPEEDUP_KEY>
    <SCROLLING_SPEEDUP_FACTOR>2</SCROLLING_SPEEDUP_FACTOR>
  </scrolling_mode>
</config>
```

Below are the configuration items, their default values, and descriptions.
Item | Default value | Description
--- | --- | ---
Path to `config.dtd` | `config/config.dtd` | Relative or absolute path to the `config.dtd` from the config file
DEVICE | `/path/to/your/keyboard` | Path to your keyboard under `/dev/input`
POINTER_MODE_KEY | KEY_RIGHTCTRL | Key to activate pointer mode while being pushed down
UP_KEY | KEY_W | Key to move the pointer up (in pointer mode)<br>Key to scroll up (in scrolling mode)
DOWN_KEY | KEY_S | Key to move the pointer down (in pointer mode)<br>Key to scroll down (in scrolling mode)
RIGHT_KEY | KEY_D | Key to move the pointer right (in pointer mode)<br>Key to scroll right (in scrolling mode)
LEFT_KEY | KEY_A | Key to move the pointer left (in pointer mode)<br>Key to scroll left (in scrolling mode)
POINTER_MOVEMENT_SPEED | 2 | Pointer movement speed, must be a positive integer
POINTER_SPEEDUP_KEY | KEY_SLASH | Key to increases the speed of the pointer only while being pushed down
POINTER_SPEEDUP_FACTOR | 5 | Pointer movement speed will be multiplied by this value
MOUSE_LEFT_BUTTON | KEY_COMMA | Key to left click (in pointer mode)
MOUSE_RIGHT_BUTTON | KEY_DOT | Key to right click (in pointer mode)
MOUSE_MIDDLE_BUTTON | KEY_M | Key to middle click (in pointer mode)
SCROLLING_MODE_KEY | KEY_RIGHTSHIFT | Key to activate scrolling mode while being pushed down
SCROLLING_SPEED | 25 | Scrolling speed, must be a positive integer
SCROLLING_SPEEDUP_KEY | KEY_SLASH | Key to increases the speed of scrolling only while being pushed down
SCROLLING_SPEEDUP_FACTOR | 2 | Scrolling speed will be multiplied by this value

For example, if you want to change the key that enters pointer mode from `KEY_RIGHTCTRL` to `KEY_RIGHTALT`, edit as follows.
```diff
$ diff old.xml new.xml 
8c8
<     <POINTER_MODE_KEY>KEY_RIGHTCTRL</POINTER_MODE_KEY>
---
>     <POINTER_MODE_KEY>KEY_RIGHTALT</POINTER_MODE_KEY>
```
For a list of available key codes, see [here](https://gitlab.freedesktop.org/libevdev/libevdev/-/blob/master/include/linux/linux/input-event-codes.h?ref_type=heads#L75), for example.  
Note that they must be key codes that are actually supported by your keyboard.

## Usage
```
$ sudo ./marionette /path/to/your/config.xml
```
If it starts without error, the following output will appear on the terminal.
```
[I] start loading settings...
[I] DEVICE                   : /dev/input/eventX
[I] POINTER_MODE_KEY         : KEY_RIGHTCTRL
...
[I] syspath for uinput device: /sys/devices/virtual/input/inputX
[I] device node for uinput device: /dev/input/eventX

```

Press `POINTER_MODE_KEY` to activate pointer mode.  
Pointer mode is activated only as long as the key is kept pressed.  
In this mode, you can move the pointer with the `UP_KEY`, `DOWN_KEY`, `RIGHT_KEY` and `LEFT_KEY`.  
Pressing `SPEEDUP_KEY` while in this mode will increase the speed of the pointer for as long as you hold it down.  
The `MOUSE_LEFT_BUTTON`, `MOUSE_RIGHT_BUTTON`, and `MOUSE_MIDDLE_BUTTON` allow you to left-click, right-click, and middle-click.

Press `SCROLLING_MODE_KEY` to activate scrolling mode.  
Scrolling mode is activated only as long as the key is kept pressed.  
In this mode, you can scroll up and down with the `UP_KEY` and `DOWN_KEY`, and right and left with the `RIGHT_KEY` and `LEFT_KEY`.  
Pressing `SPEEDUP_KEY` while in this mode will increase the speed of scrolling for as long as you hold it down.

In both pointer and scrolling modes, the keyboard does not provide any output as a keyboard.

To terminate, just use `Ctrl+C` or `kill`.

#### License
MIT License Copyright (c) 2023 VermouthMoth

#### Contact
[`0o.jx_xjzzz`](https://discordapp.com/users/1150388238037565541) on Discord
___
_This program was originally written just for my own use._  
_**ANY** kind of your feedback is sincerely welcome._  
_It will make this program more useful for more people..._
