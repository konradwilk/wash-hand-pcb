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

![](front.png =250x)

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
