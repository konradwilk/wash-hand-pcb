OBJDUMP		= avr-objdump
CHIP		= atmega328p
FQBN		= arduino:avr:uno
EXTRA_FLAGS	= -DDEBUG
BLOB		= Wash_Hand.ino.with_bootloader.hex

all:
	arduino-cli -v compile --warnings=all --build-properties compiler.cpp.extra_flags="$(EXTRA_FLAGS)" --fqbn $(FQBN) --output-dir `pwd`

clean:
	rm -f *.ihex *.elf *.eep *.bin *.hex

buspirate:
	avrdude -C/etc/avrdude/avrdude.conf -v -cbuspirate -p$(CHIP) -P/dev/ttyUSB0 -x ascii -b115200 -D -Uflash:w:$(BLOB):i

upload:
	avrdude -C/etc/avrdude/avrdude.conf -v -carduino -p$(CHIP) -P/dev/ttyACM0 -b115200 -D -Uflash:w:$(BLOB):i

upload-tqfp:
	 avrdude -C/etc/avrdude/avrdude.conf -v -p$(CHIP) -cstk500v1 -P/dev/ttyACM0 -b19200 -D -Uflash:w:$(BLOB):i

usb:
	avrdude -C/etc/avrdude/avrdude.conf -v -carduino -p$(CHIP) -P/dev/ttyUSB0 -b115200 -D -Uflash:w:$(BLOB):i

usbasp:
	avrdude -C/etc/avrdude/avrdude.conf -v -B 4 -cusbasp -p$(CHIP) -Uflash:w:$(BLOB):i

setup:
	arduino-cli core update-index
	arduino-cli core install arduino:avr

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

