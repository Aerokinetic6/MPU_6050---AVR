#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/twi.h>
#include "i2c.h"
#include "mpu.h"




void port_init();
void mpu_init();
void read_mpu();
void led_out(uint8_t led);
void segm_out();

uint8_t szam[4];

uint8_t acc_xl=0;

int main()
{
	uint8_t sec,min;
	
	port_init();
	i2c_init();
	mpu_init();
	
	
	while(1){
	
	  read_mpu();
	
	  led_out(acc_xl);
	
		
	}//while

}



///////////////////////////////////////
///////////
/////////////////////////////////////

void port_init()
{
	DDRD=0xF0;
	DDRB=0xF0;
	DDRG=0x00;
	DDRA=0xFF;

	//sei();

}

void mpu_init()
{
        _delay_ms(150);
        I2C_WR(MPU_ADDR, SMPLRT_DIV, 0x07);
        I2C_WR(MPU_ADDR, PWR_MGMT_1, 0x01);
        I2C_WR(MPU_ADDR, CONFIG, 0x00);
        I2C_WR(MPU_ADDR, GYRO_CONFIG, 0x18);
        I2C_WR(MPU_ADDR, INT_ENABLE, 0x01);
}


void read_mpu()
{
        acc_xl = I2C_RD(MPU_ADDR, GYRO_YOUT_H);
}

void led_out(uint8_t led)
{
	PORTD=led;
	PORTB=(led<<4);
}




void segm_out()
{
	uint8_t digit=0, j=0;

	while(digit<4)
		{
			
			PORTA=(0x80+(digit<<4)+szam[j]);
			digit++;
			j++;
			_delay_ms(5);
		}

}
