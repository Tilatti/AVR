GCC = avr-gcc

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

AVRDUDE = avrdude

MAIN = sleep

all: sleep.hex

clean:
	rm sleep.hex

sleep.hex: sleep
	$(OBJCOPY) -j .text -j .data -O ihex sleep sleep.hex

sleep: sleep.c
	$(GCC) -g -Os -mmcu=attiny2313 sleep.c -o sleep

upload: sleep.hex
	$(AVRDUDE) -c buspirate -P /dev/ttyUSB0  -p t2313 -v -U flash:w:sleep.hex
