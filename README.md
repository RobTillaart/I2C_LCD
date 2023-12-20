
[![Arduino CI](https://github.com/RobTillaart/I2C_LCD/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/I2C_LCD/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/I2C_LCD/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/I2C_LCD/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/I2C_LCD/actions/workflows/jsoncheck.yml)
[![GitHub issues](https://img.shields.io/github/issues/RobTillaart/I2C_LCD.svg)](https://github.com/RobTillaart/I2C_LCD/issues)

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/I2C_LCD/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/I2C_LCD.svg?maxAge=3600)](https://github.com/RobTillaart/I2C_LCD/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/robtillaart/library/I2C_LCD.svg)](https://registry.platformio.org/libraries/robtillaart/I2C_LCD)


# I2C_LCD

Arduino library for I2C_LCD  LCD displays.


**Warning: experimental with side effects, use at own risk**

do not overfeed your display with too much data
too fast as it may not be able to handle.
During longer tests my 20x4 display garbled too often. 
Under investigation.


## Description

**Experimental** Arduino library for the I2C LCD display, typical 20x4 characters.

The goal of the library is to minimize footprint and improve performance.
This is partly done by dedicate the library to I2C only.

The library is inspired by the excellent LiquidCrystel_I2C library of F. Malpartida. Therefore the interface is kept quite identical.
- https://github.com/fmalpartida/New-LiquidCrystal

The library implements the print interface, so all data types are printable
assuming they fit on the display.


#### Compatibility

Near 100% compatible with F. Malpartida's I2C library, relative minor differences.
Furthermore the current version does not support all functionality, and
more important not tested that much.
So if you need full functionality, or reliability, you still should use 
the **New-liquidCrystal** library.


#### Tests

Tested on an UNO and a 20x4 character LCD, and limited on a 16x2 LCD.




## Performance

The most important optimization is to send a byte in a single I2C transaction.
This takes 5 bytes to transport, which is 3 less than the reference.

Furthermore there is an optimization if the pins are in order, as then
it is far easier to get the nibble (half bytes) to send.

First performance tests are good. See example **I2C_LCD_performance.ino**.

The performance measurement is done on an UNO, pins are in order.

|  I2C clock  |  time (us)  |  notes  |
|:-----------:|:-----------:|:-------:|
|  100000     |     4316    |
|  200000     |     2440    |
|  300000     |     1780    |
|  400000     |     1496    |  problems with spectrum examples - too much data
|  500000     |     1308    |
|  600000     |     1176    |
|  700000     |     1076    |
|  800000     |     1024    |


#### Related

- https://github.com/fmalpartida/New-LiquidCrystal


## Interface

```cpp
#include "I2C_LCD.h"
```

#### Constructor

- **I2C_LCD(uint8_t address, TwoWire \* wire = &Wire)** Constructor, 
mandatory address and optional alternative I2C bus.
- **void config(uint8_t address, uint8_t enable, uint8_t readWrite, uint8_t registerSelect, 
                   uint8_t data4, uint8_t data5, uint8_t data6, uint8_t data7, 
                   uint8_t backlight, uint8_t policy)** pin configuration.
Will change in the future.
- **void begin(uint8_t cols = 20, uint8_t rows = 4)** initializes library size.


#### Backlight

- **void setBacklightPin (uint8_t pin, uint8_t policy)**
 policy not implemented yet.
- **void setBacklight(bool on)**
- **void backlight()** wrapper for setBacklight()
- **void noBacklight()** wrapper for setBacklight()


#### Display

- **void display()** set display on
- **void noDisplay()** set display off
- **void on()** set display on
- **void off()** set display off


#### Positioning and cursor

- **void clear()** clear whole screen and set cursor to 0, 0.
- **void clearEOL()** clears line from current pos. **NEW**
- **void home()** set cursor to 0, 0.
- **bool setCursor(uint8_t col, uint8_t row)** set cursor to given position.
There is a check if this is out of range, if so it return false.
- **void noBlink()** idem.
- **void blink()** idem.
- **void noCursor()** idem.
- **void cursor()** idem.


#### Miscellaneous

Minimal tested.

- **void scrollDisplayLeft(void)**
- **void scrollDisplayRight(void)**
- **void moveCursorRight(void)**
- **void moveCursorLeft(void)**

The next 4 have only limited support  
(either autoscroll or leftright can be set, not both)

- **void autoscroll(void)**
- **void noAutoscroll(void)**
- **void leftToRight(void)**
- **void rightToLeft(void)**


#### Create your own characters

A charmap consists of an array of 8 bytes with values 0..31 (5 bits).
There are 8 slots to place a special character, indec 0..7.

- **void createChar(uint8_t index, uint8_t \* charmap)**
- **size_t special(uint8_t index)** to print the special char.


index = 0..7,  charmap = 8 bytes (0..31)

To be printed with 
- **lcd.write((uint8_t)index)** or simpler
- **lcd.special(index)** wrapper.


#### Print interface

- **size_t write(uint8_t c)**

Array writing not implemented as there are no gains seen.


## Experimental

#### Position tracking

The library tries to keep track of the current position on the line it is.
When going beyond the number of columns, it will not print any more.
This protects against printing beyond screen position (and writing
one some other line).
Additional it saves some time.

This position tracking only works for "standard" use, no display 
scrolling, right to left mode etc. 
So it can cause unwanted side effects, which can be fixed by removing
the "pos < cols" condition from **write()**.


#### Tab printing

Experimental, 
When '\t' (character 9) is printed to the LCD, it moves to the first position
that is a multiple of 4.
This allows a simple way to get some sort of tabular data representation.
See the example.

Idea for the future might be to set the tab-stops instead of current hardcoded ones.



## Future

#### Must

- update documentation

#### Should

- test, test, test
- test with other platforms
- test with other display sizes
- test more
- fix TODO's in code

#### Could

- add examples
  - Wire1, Wire2 etc
- make an issue for New-LiquidCrystal library.
- function to define the tab-stops, instead of hard coded ones.
- investigate other special characters to support, like
  - \r => goto begin of current line
  - \n => goto begin of next line
  - FF => form feed is clear screen.
  - BELL => blink of the display  (oeps 7 is already a special char )
- make a separate include file for charmaps by name.

#### Wont for now.

- implement unit tests (possible?)
- add timestamp last print


## Support

If you appreciate my libraries, you can support the development and maintenance.
Improve the quality of the libraries by providing issues and Pull Requests, or
donate through PayPal or GitHub sponsors.

Thank you,

