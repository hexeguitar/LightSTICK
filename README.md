# LightStick

![alt text][pic1]

## Features:  
- High CRI >95 dual color white leds (cold/warm).
- Modular system with one Master module and up to 4 secondary modules.
- Linear constant current LED drivers.
- IR remote control (brightness, color, presets)
- Optional analog control using potentiometers (not implemented yet).
- Power saving OFF mode  
  
## Background/design:
Traditional and common studio lights used in photography and filming usually use a high current PWM to control the brightness. For my use in filming analog instruments and effects (often high gain ones) i really needed something quiet and as noise/EMI free as possible.  
That's how the **LightSTICK** modular light was born - it uses linear constant current drivers for the LEDs and a DC control voltage to set the brightness and the color temperature.  
## Modularity
The design is modular, one "Master" module, consisting the MCU and IR receiver can control up to 4 additional secondary modules connected via a 5 wire bus:  
* 12V supply for the LEDs
* 5V supply for the constant current drivers / control voltage reference
* **WH** - cold white control voltage
* **WWH** - warm white control voltage
* GND  
## LED type  
LEDs used in the LightSTICK are manufactured by the LEDGUON company, obtainable through their direct store at Aliexpress.  
The part number is: **JH-5054WWW12S35-T8A-97**
![alt text][pic2]  

## PCB design
PCB is designed as 4 layer one, using the most of the top layer for heat dissipation.  
![alt text][pic3]  
## Firmware
LightSTICK uses ATtiny806 as it's MCU. The firmware is available as Platformio project and compiled with Arduino framework. Only one library is required and included in the /lib folder: **Arduino-IRremote**, which has been modified to support the ATtiny806 chip.  

### Pot support
Initially i planned to use two potentiometers as control input for the brightness and color temp. However, during the firmware development the IR remote control turned out to be much more useful, hence i went that route. Nevertheless, the footprint for pots are provided and a basic interrupt driven ADC read functionality is implemented in case some would need it. Expanding the firmware should not be a hard task.  

### IR remote control
The easiest way to adapt an existing tv/camera remote as LightSTICK controller is to analyze it using a generic arduino board and one of the examples from the **Arduino-IRremote** library (ie. "Receive Demo). Simply write down the protocol of the remote and all the command codes for each button. In my case of a small remote from an old TC card the results were as follows:  
```
Protocol:           RC5

...
+:                  Command=0x3 Toggle=1 Raw-Data=0x19C3
                    Command=0x3 Raw-Data=0x11C3

-:                  Command=0x9 Toggle=1 Raw-Data=0x19C9
                    Command=0x9 Raw-Data=0x11C9

Up:                 Command=0x6 Toggle=1 Raw-Data=0x19C6
                    Command=0x6 Raw-Data=0x11C6
                    
Down:               Command=0xC Toggle=1 Raw-Data=0x19CC
                    Command=0xC Raw-Data=0x11CC

1:                  Command=0xF Toggle=1 Raw-Data=0x19CF
                    Command=0xF Raw-Data=0x11CF

2:                  Command=0x15 Raw-Data=0x11D5
                    Command=0x15 Toggle=1 Raw-Data=0x19D5

3:                  Command=0x10 Toggle=1 Raw-Data=0x19D0
                    Command=0x10 Raw-Data=0x11D0

4:                  Command=0x18 Raw-Data=0x11D8
                    Command=0x18 Toggle=1 Raw-Data=0x19D8

5:                  Command=0x1B Toggle=1 Raw-Data=0x19DB
                    Command=0x1B Raw-Data=0x11DB
...
```
The command codes are then used in the IR command table (*`remote.h`* file)
```c++
/**
 * @brief IR remote command table. 
 * 
 */
typedef enum
{
    CMD_IDLE            = 0x00,
    CMD_PWR_TGL         = 0x39,     // key POWER
    CMD_BRIGHT_INC      = 0x03,     // key +
    CMD_BRIGHT_DEC      = 0x09,     // key -
    CMD_BRIGHT_MAX      = 0x30,     // key PLAY
    CMD_COLOR_WARMER    = 0x06,     // key UP
    CMD_COLOR_COLDER    = 0x0C,     // key DOWN
    CMD_COLOR_WARM      = 0x33,     // key FWD
    CMD_COLOR_COLD      = 0x2D,     // key REW
    CMD_PRESET_0        = 0x27,     // key 0
    CMD_PRESET_1        = 0x0F,     // key 1
    CMD_PRESET_2        = 0x15,     // key 2
    CMD_PRESET_3        = 0x10,     // key 3
    CMD_PRESET_4        = 0x18,     // key 4
    CMD_PRESET_5        = 0x1B,     // key 5
    CMD_PRESET_6        = 0x1E,     // key 6
    CMD_PRESET_7        = 0x11,     // key 7
    CMD_PRESET_8        = 0x21,     // key 8
    CMD_PRESET_9        = 0x12,     // key 9
    CMD_RESET_MCU       = 0x3C,     // key STOP
    CMD_STORE_PRESET    = 0x36      // key REC
}ir_cmd_t;
```  
![alt text][pic5]  
### Low power design  
Both, the electronics and the firmware implements low power design techniques making the LightSTICK useful as battery operated device, too.  
In **OFF** state the global 12V and 5V power busses are disabled, the MCU is in power off state, practically leaving the IR receiver as a single powered on device.

### Programming
ATtiny806 uses UPDI protocol for programming the flash. The programmer should be connected to the pads marked !UPDI, 5V and GND, depending on the used device. Programmers like the Atmel-ICE require the 5V VDD to set the proper logic level.  
An UPDI programmer can be also made of a generic Arduino board. In that case make sure the board runs on 5V, only the UPDI and GND connections will be required.  
The `platformio.ini` file assumes the Atmel-ICE programmer. This file has to be modified if another type of programmer is used.      
First use the following command in Platformio to set the correct fuse values:  
```
pio run -t fuses -e set_fuses
```
and then normally upload the firmware.  

## Applications  
Cheap "book lamp" rebuilt into remote controlled high CRI light for macro photography. Improved thermals by mounting the pcbs on two pieces of 15mm wide alu flat profile with 3mm thick silicon pad isolation.
![alt text][pic6]  
A ring of 5 LightSTICKs used as source of indirect light in macro photography.
![alt text][pic7]  
![alt text][pic4]  


----  
Copyright 2021 by Piotr Zapart  
www.hexefx.com

[pic1]: pics/LighSTICK_a.png "LightSTICK main"
[pic2]: pics/Led_type.jpg "LEDs used in the project"
[pic3]: pics/LightStick9.jpg "LightSTICK"
[pic4]: pics/LightStick8.jpg "LightSTICK"
[pic5]: pics/LightStick6.jpg "LightSTICK"
[pic6]: pics/LightStick11.jpg "LightSTICK"
[pic7]: pics/LightStick12.jpg "LightSTICK"  

