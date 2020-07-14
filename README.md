# License

The project is under Attribution-ShareAlike 4.0 International (CC BY-SA 4.0).

# Libraries used for this project

 - adafruit
 - SparkFun-Capacitors
 - SparkFun-Connectors
 - SparkFun-Hardware
 - darnok\_common (https://github.com/konradwilk/eagle-libraries.git)
 - darnok\_led (https://github.com/konradwilk/eagle-libraries.git)
 - darnok\_usb (https://github.com/konradwilk/eagle-libraries.git)

# Purpose

Quite simply it is to have a count-down timer for 20 seconds for washing hands.
It is activated by a line sensor (which has infrared light) that plugs in the board.

## PCB v1.2-routing

The v1.2-routing is the stable release.  See the <a href="BOM.txt">BOM</a> for the list of materials.
Note the missing line sensor which was <a href="https://www.aliexpress.com/item/32654587628.html">on AliExpress</a>

<img src="front.png" width=250></img>

Note that the routing was done automatically with slight adjustments whenever the Design Rule Check did not
like it.

# Future plans for hardware

Which in no particular order are:

 - Integrate the line-sensor logic in the board (a simple infrared light and a voltage comparison).
 - Use the SMD version of resistors and ATMEGA328P
 - Add a SD card
 - Fix the I2C lines missing resistors
 - Add 9V battery connection and line regulator
 - Add switch to choose between 9V or 5V from USB

# Future plans for software

In particular order:
 - Fetch data from a SD-card (if attached), otherwise read from EEPROM (1K).
 - Format-type of the data type for above mechanism.
 - Utilize sleep operations to keep the CPU most of the time asleep.
