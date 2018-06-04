#MACROS
TARGET_C = mpu_main.c i2c.c uart0.c
TARGET_O = mpu_main.o i2c.o uart0.o
TARGET_E = mpu.elf
TARGET_H = mpu.hex
TARGET_F = flash:w:mpu.hex:i

compile:
	rm -f *.o *.elf *.hex
	avr-gcc -g -Os -mmcu=atmega128 -c $(TARGET_C)
	avr-gcc -g -mmcu=atmega128 -o $(TARGET_E) $(TARGET_O)
	avr-objcopy -j .text -j .data -O ihex $(TARGET_E) $(TARGET_H)
	#cat $(TARGET_H)
	echo "$$(tput setaf 2)Compiled OK"

flash:
	sudo avrdude -p m128 -c jtagmkI -P /dev/ttyUSB0 -U $(TARGET_F)
	echo "$$(tput setaf 2)Flashed OK"


	
