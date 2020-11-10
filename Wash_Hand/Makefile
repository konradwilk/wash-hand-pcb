OBJDUMP		= $(GCCROOT)avr-objdump
OBJCOPY		= $(GCCROOT)avr-objcopy

# To be used with usbasp or usb targets..
BLOB		= Wash_Hand.ino.with_bootloader.hex
CHIP		= atmega328p
#BLOB		= Wash_Hand_atmega8.hex
#CHIP		= atmega8

PROGRAM		= Wash_Hand

EXTRA_FLAGS	= -DDEBUG
AVRDUDE_CONF	= /etc/avrdude/avrdude.conf
BOOTLOADERS	= /usr/share/arduino/hardware/arduino/avr/bootloaders/optiboot

atmega328: TARGET = atmega328
atmega328: FQBN = arduino:avr:uno
atmega328: $(PROGRAM)_atmega328.lst
atmega328: $(PROGRAM)_atmega328.hex
atmega328: $(PROGRAM)_atmega328.bin

atmega328_isp: atmega328
atmega328_isp: CHIP = atmega328p
atmega328_isp: TARGET = atmega328
atmega328_isp: BOOTLOADER = optiboot_atmega328.hex
atmega328_isp: HFUSE = 0xDE
atmega328_isp: LFUSE = 0xFF
atmega328_isp: EFUSE = 0xFD
atmega328_isp: isp flash

atmega8: TARGET = atmega8
atmega8: FQBN	= arduino:avr:atmegang:cpu=atmega8
atmega8: $(PROGRAM)_atmega8.lst
atmega8: $(PROGRAM)_atmega8.hex
atmega8: $(PROGRAM)_atmega8.bin

atmega8_isp: atmega8
atmega8_isp: CHIP = atmega8
atmega8_isp: TARGET = atmega8
atmega8_isp: BOOTLOADER = optiboot_atmega8.hex
atmega8_isp: HFUSE = 0xCA
atmega8_isp: LFUSE = 0xDF
atmega8_isp: isp_no_e flash

ISPFUSES	= $(GCCROOT)avrdude -C$(AVRDUDE_CONF) -v -p$(CHIP) -cusbasp -Pusb -e \
		-Ulock:w:0x3F:m -Uefuse:w:$(EFUSE):m -Uhfuse:w:$(HFUSE):m -Ulfuse:w:$(LFUSE):m

ISPFUSES_NO_E	=$(GCCROOT)avrdude -C$(AVRDUDE_CONF) -v -p$(CHIP) -cusbasp -Pusb -e \
		-Ulock:w:0x3F:m -Uhfuse:w:$(HFUSE):m -Ulfuse:w:$(LFUSE):m

isp: $(TARGET)
	$(ISPFUSES)

isp_no_e: $(TARGET)
	$(ISPFUSES_NO_E)

flash: $(TARGET)
	avrdude -C$(AVRDUDE_CONF) -v -p$(CHIP) -cusbasp -Pusb -Uflash:w:$(BOOTLOADERS)/$(BOOTLOADER):i -Ulock:w:0x0F:m
	avrdude -C$(AVRDUDE_CONF) -v -B 4 -cusbasp -p$(CHIP) -D -Uflash:w:$(PROGRAM)_$(TARGET).hex:i

%.elf:
	arduino-cli -v compile --warnings=all --build-properties compiler.cpp.extra_flags="$(EXTRA_FLAGS)" --fqbn $(FQBN) --output-dir `pwd`
	mv $(PROGRAM).ino.elf $(PROGRAM)_$(TARGET).elf

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O binary $< $@

clean:
	rm -f *.ihex *.elf *.eep *.bin *.hex *.lst

buspirate:
	avrdude -C$(AVRDUDE_CONF) -v -cbuspirate -p$(CHIP) -P/dev/ttyUSB0 -x ascii -b115200 -D -Uflash:w:$(BLOB):i

upload:
	avrdude -C$(AVRDUDE_CONF) -v -carduino -p$(CHIP) -P/dev/ttyACM0 -b115200 -D -Uflash:w:$(BLOB):i

upload-tqfp:
	 avrdude -C$(AVRDUDE_CONF) -v -p$(CHIP) -cstk500v1 -P/dev/ttyACM0 -b19200 -D -Uflash:w:$(BLOB):i

usb:
	avrdude -C$(AVRDUDE_CONF) -v -carduino -p$(CHIP) -P/dev/ttyUSB0 -b115200 -D -Uflash:w:$(BLOB):i

usbasp:
	avrdude -C$(AVRDUDE_CONF) -v -B 4 -cusbasp -p$(CHIP) -D -Uflash:w:$(BLOB):i

setup:
	arduino-cli core update-index
	arduino-cli core install arduino:avr

