CC=avr-gcc
CFLAGS=-g -Os -Wall -I../common -mcall-prologues -mmcu=atmega328p -DF_CPU=8000000UL
OBJ2HEX=avr-objcopy 
TARGET=toothbrush
LDFLAGS = -Wl,-Map,$(TARGET).map

hex: $(TARGET).hex

program: $(TARGET).hex 
	sudo avrdude -e -D -p m328p -P usb -c avrispmkII -B 1 -Uflash:w:$(TARGET).hex

fuses:
	sudo avrdude -p m328p -P usb -c avrispmkII -B 8 -U lfuse:w:0xe2:m -U hfuse:w:0xd7:m -U efuse:w:0xff:m -U lock:w:0xff:m

clear:
	sudo avrdude -p m328p -P usb -c avrispmkII -B 8 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m -U lock:w:0xff:m
	program

$(TARGET).o: $(TARGET).cpp defs.h
	$(CC) $(CFLAGS) -c $(TARGET).cpp 

ws2812.o: ws2812.cpp ws2812.h defs.h
	$(CC) $(CFLAGS) -c ws2812.cpp

$(TARGET).hex: $(TARGET).obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

$(TARGET).obj: $(TARGET).o ws2812.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $(TARGET).o ws2812.o

clean:
	rm -f *.hex *.obj *.o *.map
