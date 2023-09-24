# marionette
`marionette` is a C program to **emulate mouse with keyboard on Linux**.

The following features are available.
- Moving the pointer
- Mouse clicks
- Scrolling

This program creates a virtual keyboard exactly the same as your keyboard, but adds some mouse functions.  
The virtual keyboard then outputs the input events from the original keyboard in a form that basically passes through as is, and converts them to mouse events as needed.  
It works **no matter what window system** you are using, e.g. `X Window System` or `Wayland`.

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

<details>
<summary>Additional info</summary>

This program relies on `libevdev`, which stands between the kernel and other subsequent programs that handle input events.  
Thus, it works no matter what window system you are using.  
It even works on a non-graphical console.  
For `libevdev`, see [https://www.freedesktop.org/wiki/Software/libevdev/](https://www.freedesktop.org/wiki/Software/libevdev/).

This program uses XML file for config file.  
To handle XML format, `libxml2` is used.  
For `libxml2`, see [https://gitlab.gnome.org/GNOME/libxml2/-/wikis/](https://gitlab.gnome.org/GNOME/libxml2/-/wikis/).

This program consists of the following directories and files.
- `src/`: direcotory containing source files
- `include/`: directory containing header files
- `MakeFile`: makefile
</details>

## Building

This program uses `libevdev` and `libxml2`.  
You may need to install something like `libevdev`, `libevdev-devel` and `libxml2`.

```
$ git clone https://github.com/vermouthmoth/marionette.git
$ cd marionette
$ make
```
Two executable binary files `marionette` and `gen_config` will be created.  
`MakeFile` also supports `clean` target.

## Configuration
Configuration file is written in XML format.  

Config file template can be created using `gen_config`.
```
$ ./gen_config > config.xml
```
The name of the configuration file can be anything.
<details>
<summary>Additional info</summary>

`marionette` validates if the format of a given config file is valid using DTD info.  
Valid format rules are written in `config/config.dtd`.  
`gen_config` makes the template include `config/config.dtd`.  
If you have moved the location of `gen_config` or `config/config.dtd`, instead run the following.
```
$ ./gen_config /path/to/config.dtd > config.xml
```
</details>
  
The editable contents in the config file template is as follows.
```xml
<config DEVICE="/path/to/your/keyboard">
  <pointer_mode POINTER_MODE_KEY="KEY_RIGHTCTRL">
    <POINTER_UP_KEY>KEY_W</POINTER_UP_KEY>
    <POINTER_DOWN_KEY>KEY_S</POINTER_DOWN_KEY>
    <POINTER_RIGHT_KEY>KEY_D</POINTER_RIGHT_KEY>
    <POINTER_LEFT_KEY>KEY_A</POINTER_LEFT_KEY>
    <POINTER_MOVEMENT_SPEED>2</POINTER_MOVEMENT_SPEED>
    <POINTER_SPEEDUP_KEY>KEY_SLASH</POINTER_SPEEDUP_KEY>
    <POINTER_SPEEDUP_FACTOR>5</POINTER_SPEEDUP_FACTOR>
    <MOUSE_LEFT_BUTTON>KEY_COMMA</MOUSE_LEFT_BUTTON>
    <MOUSE_RIGHT_BUTTON>KEY_DOT</MOUSE_RIGHT_BUTTON>
    <MOUSE_MIDDLE_BUTTON>KEY_M</MOUSE_MIDDLE_BUTTON>
  </pointer_mode>
  <scrolling_mode SCROLLING_MODE_KEY="KEY_RIGHTSHIFT">
    <SCROLLING_UP_KEY>KEY_W</SCROLLING_UP_KEY>
    <SCROLLING_DOWN_KEY>KEY_S</SCROLLING_DOWN_KEY>
    <SCROLLING_RIGHT_KEY>KEY_D</SCROLLING_RIGHT_KEY>
    <SCROLLING_LEFT_KEY>KEY_A</SCROLLING_LEFT_KEY>
    <SCROLLING_SPEED>25</SCROLLING_SPEED>
    <SCROLLING_SPEEDUP_KEY>KEY_SLASH</SCROLLING_SPEEDUP_KEY>
    <SCROLLING_SPEEDUP_FACTOR>2</SCROLLING_SPEEDUP_FACTOR>
  </scrolling_mode>
</config>
```

Below are the configuration items, their default values, and descriptions.
Item | Default value | Description
--- | --- | ---
DEVICE | `/path/to/your/keyboard` | Path to your keyboard under `/dev/input`
POINTER_MODE_KEY | KEY_RIGHTCTRL | Key to activate pointer mode while being pushed down
POINTER_UP_KEY | KEY_W | Key to move the pointer up
POINTER_DOWN_KEY | KEY_S | Key to move the pointer down
POINTER_RIGHT_KEY | KEY_D | Key to move the pointer right
POINTER_LEFT_KEY | KEY_A | Key to move the pointer left
POINTER_MOVEMENT_SPEED | 2 | Pointer movement speed, must be a positive integer
POINTER_SPEEDUP_KEY | KEY_SLASH | Key to increases the speed of the pointer only while being pushed down
POINTER_SPEEDUP_FACTOR | 5 | Pointer movement speed will be multiplied by this value
MOUSE_LEFT_BUTTON | KEY_COMMA | Key to left click (in pointer mode)
MOUSE_RIGHT_BUTTON | KEY_DOT | Key to right click (in pointer mode)
MOUSE_MIDDLE_BUTTON | KEY_M | Key to middle click (in pointer mode)
SCROLLING_MODE_KEY | KEY_RIGHTSHIFT | Key to activate scrolling mode while being pushed down
SCROLLING_UP_KEY | KEY_W | Key to scroll up
SCROLLING_DOWN_KEY | KEY_S | Key to scroll down
SCROLLING_RIGHT_KEY | KEY_D | Key to scroll right
SCROLLING_LEFT_KEY | KEY_A | Key to scroll left
SCROLLING_SPEED | 25 | Scrolling speed, must be a positive integer
SCROLLING_SPEEDUP_KEY | KEY_SLASH | Key to increases the speed of scrolling only while being pushed down
SCROLLING_SPEEDUP_FACTOR | 2 | Scrolling speed will be multiplied by this value

For example, to change the key assignment of `POINTER_MODE_KEY` from `KEY_RIGHTCTRL` to `KEY_RIGHTALT`, edit as follows.
```diff
$ diff old.xml new.xml 
28c28
<   <pointer_mode POINTER_MODE_KEY="KEY_RIGHTCTRL">
---
>   <pointer_mode POINTER_MODE_KEY="KEY_RIGHTALT">
```
For a list of available key codes, see [here](https://gitlab.freedesktop.org/libevdev/libevdev/-/blob/master/include/linux/linux/input-event-codes.h?ref_type=heads#L75), for example.  
Note that they must be key codes that are actually supported by your keyboard.  
Also, for items to which you do not want to assign keys, you can leave those values blank.

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

### Pointer Mode
Press `POINTER_MODE_KEY` to activate pointer mode.  
Pointer mode is activated only as long as the key is kept pressed.  
In this mode, you can move the pointer with the `POINTER_UP_KEY`, `POINTER_DOWN_KEY`, `POINTER_RIGHT_KEY` and `POINTER_LEFT_KEY`.  
Pressing `SPEEDUP_KEY` while in this mode will increase the speed of the pointer for as long as you hold it down.  
The `MOUSE_LEFT_BUTTON`, `MOUSE_RIGHT_BUTTON`, and `MOUSE_MIDDLE_BUTTON` allow you to left-click, right-click, and middle-click.

### Scrolling Mode
Press `SCROLLING_MODE_KEY` to activate scrolling mode.  
Scrolling mode is activated only as long as the key is kept pressed.  
In this mode, you can scroll up and down with the `SCROLLING_UP_KEY` and `SCROLLING_DOWN_KEY`, and right and left with the `SCROLLING_RIGHT_KEY` and `SCROLLING_LEFT_KEY`.  
Pressing `SPEEDUP_KEY` while in this mode will increase the speed of scrolling for as long as you hold it down.

In both pointer and scrolling modes, the keyboard does not provide any output as a keyboard.

To terminate, just use `Ctrl+C` or `kill`.
___
#### Contact
[`0o.jx_xjzzz`](https://discordapp.com/users/1150388238037565541) on Discord  
_This program was originally written just for my own use._  
_**ANY** kind of your feedback is sincerely welcome._  
