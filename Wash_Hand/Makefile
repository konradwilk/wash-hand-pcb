
all:
	arduino-cli -v compile --fqbn  arduino:avr:uno --output-dir `pwd`

clean:
	rm -f *.ihex *.elf *.eep *.bin *.hex

buspirate:
	avrdude -C/etc/avrdude/avrdude.conf -v -cbuspirate -patmega328p -P/dev/ttyUSB0 -x ascii -b115200 -D -Uflash:w:Wash_Hand.ino.with_bootloader.hex:i

upload:
	avrdude -C/etc/avrdude/avrdude.conf -v -carduino -patmega328p -P/dev/ttyACM0 -b115200 -D -Uflash:w:Wash_Hand.ino.hex:i
