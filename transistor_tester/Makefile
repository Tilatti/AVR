GCC = avr-gcc

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

AVRDUDE = avrdude

MAIN = main

all: main.hex

clean:
	rm main.hex main

main.hex: main
	$(OBJCOPY) -j .text -j .data -O ihex main main.hex

main: main.c
	$(GCC) -g -Os -mmcu=attiny2313 main.c -o main

upload: main.hex
	$(AVRDUDE) -c buspirate -P /dev/ttyUSB0  -p t2313 -v -U flash:w:main.hex
