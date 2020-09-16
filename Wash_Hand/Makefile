
all:
	arduino-cli -v compile --warnings=all --fqbn  arduino:avr:uno --output-dir `pwd`

usb:
	arduino-cli -v compile --warnings=all --build-properties compiler.cpp.extra_flags=-DUSB --fqbn  arduino:avr:uno --output-dir `pwd`

debug:
	arduino-cli -v compile --warnings=all --build-properties compiler.cpp.extra_flags=-DDEBUG --fqbn  arduino:avr:uno --output-dir `pwd`

clean:
	rm -f *.ihex *.elf *.eep *.bin *.hex

buspirate:
	avrdude -C/etc/avrdude/avrdude.conf -v -cbuspirate -patmega328p -P/dev/ttyUSB0 -x ascii -b115200 -D -Uflash:w:Wash_Hand.ino.with_bootloader.hex:i

upload:
	avrdude -C/etc/avrdude/avrdude.conf -v -carduino -patmega328p -P/dev/ttyACM0 -b115200 -D -Uflash:w:Wash_Hand.ino.hex:i

upload-dip28:
	avrdude -C/etc/avrdude/avrdude.conf -v -cstk500v1 -patmega328p -P/dev/ttyACM0 -b19200 -D -Uflash:w:Wash_Hand.ino.hex:i

upload-tqfp:
	 avrdude -C/etc/avrdude/avrdude.conf -v -patmega328 -cstk500v1 -P/dev/ttyACM0 -b19200 -D -Uflash:w:Wash_Hand.ino.hex:i

mini:
	avrdude -C/etc/avrdude/avrdude.conf -vvvv -carduino -patmega328p -P/dev/ttyUSB0 -b57600 -D -Uflash:w:Wash_Hand.ino.hex:i

usbasp:
	avrdude -C/etc/avrdude/avrdude.conf -v -B 4 -cusbasp -pm328p -D -Uflash:w:Wash_Hand.ino.hex:i

setup:
	arduino-cli core update-index
	arduino-cli core install arduino:avr

