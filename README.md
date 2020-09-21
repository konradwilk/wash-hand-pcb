# License

The project is under Attribution-ShareAlike 4.0 International (CC BY-SA 4.0).

# Libraries used for this project

 - adafruit
 - darnok\_common (https://github.com/konradwilk/eagle-libraries.git)
 - darnok\_led (https://github.com/konradwilk/eagle-libraries.git)
 - darnok\_usb (https://github.com/konradwilk/eagle-libraries.git)
 - darnok\_sensors (https://github.com/konradwilk/eagle-libraries.git)
 - darnok\_microsd (https://github.com/konradwilk/eagle-libraries.git)
 - darnok\_4050 (https://github.com/konradwilk/eagle-libraries.git)
 - SparkFun-Aesthetics
 - SparkFun-Capacitors
 - SparkFun-Connectors
 - SparkFun-Hardware
 - SparkFun-IC-Microcontroller
 - SparkFun-IC-Power
 - SparkFun-Jumpers

# Purpose

Quite simply it is to have a count-down timer for 20 seconds for washing hands.
It is activated by a line sensor (which has infrared light) that plugs in the board.

### PCB v1.3b.rc5


 - <a href="v1.3b.rc5/BOM.txt">BOM</a> for the list of materials.
 - <a href="v1.3b.rc5/b-v1.3b.rc5.png">Bottom placement</a>
 - <a href="v1.3b.rc5/t-v1.3b.rc5.png">Top placement</a>
 - <a href="Board-Wash_Hand_v1.3.pdf">Schematic</a>

# Future plans for software

In particular order:

 - Fetch data from a SD-card (if attached), otherwise read from EEPROM (1K).
 - Format-type of the data type for above mechanism.
 - Utilize sleep operations to keep the CPU most of the time asleep.
 - Utilize modulo for the numbers.
