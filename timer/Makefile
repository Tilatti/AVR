GCC = avr-gcc

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

AVRDUDE = avrdude

test.hex:
	$(GCC) -g -Os -mmcu=attiny2313 test.c -o test
	$(OBJCOPY) -j .text -j .data -O ihex test test.hex

upload:
	$(AVRDUDE) -c buspirate -P /dev/ttyUSB0  -p t2313 -v -U flash:w:test.hex
