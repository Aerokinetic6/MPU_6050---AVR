#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/twi.h>
#include "i2c.h"
#include "mpu.h"
#include "uart.h"




void port_init();
void MPU_init();
void TIMER2_init();
void read_MPU();
void led_out(uint8_t led);
void segm_out();

uint8_t szam[4];
uint8_t t = 0;

uint16_t gyro_X, gyro_Y, gyro_Z, accel_X, accel_Y, accel_Z;

int main()
{
	uint8_t sec,min;
	
	port_init();
	I2C_init();
	UART_init();
	TIMER2_init();
	MPU_init();
	
	sei();	
	
	while(1){
	
	  read_MPU();
	
	  led_out(t);
	  
	  
	  if (t > 80) {
	    UART_snd_byte (12);
	    UART_snd_int(accel_X);
	    UART_snd_str("\t");
	    UART_snd_int(accel_Y);
	    UART_snd_str("\t");
	    UART_snd_int(accel_Z);
	    UART_snd_str("\n");
	    UART_snd_str("\n\r");
	    
	    UART_snd_int(gyro_X);
	    UART_snd_str("\t");
	    UART_snd_int(gyro_Y);
	    UART_snd_str("\t");
	    UART_snd_int(gyro_Z);
	    UART_snd_str("\n\r");
	    t=0;
	  }
	 //UART_snd_str("\r");
	  
          
	  
	  
	
		
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

void MPU_init()
{
        _delay_ms(150);
        I2C_WR(MPU_ADDR, SMPLRT_DIV, 0x07);
        I2C_WR(MPU_ADDR, PWR_MGMT_1, 0x01);
        I2C_WR(MPU_ADDR, CONFIG, 0x00);
        I2C_WR(MPU_ADDR, GYRO_CONFIG, 0x18);
        I2C_WR(MPU_ADDR, INT_ENABLE, 0x01);
}


void TIMER2_init()
{
        TIMSK|=(1<<TOIE2);
        TCCR2|=(1<<CS22);

}



void read_MPU()
{
        accel_X = I2C_RD(MPU_ADDR, ACCEL_XOUT_H);
        accel_X = accel_X << 8;
        accel_X = accel_X | I2C_RD(MPU_ADDR, ACCEL_XOUT_L);
        
        accel_Y = I2C_RD(MPU_ADDR, ACCEL_YOUT_H);
        accel_Y = accel_Y << 8;
        accel_Y = accel_Y | I2C_RD(MPU_ADDR, ACCEL_YOUT_L);
        
        accel_Z = I2C_RD(MPU_ADDR, ACCEL_ZOUT_H);
        accel_Z = accel_Z << 8;
        accel_Z = accel_Z | I2C_RD(MPU_ADDR, ACCEL_ZOUT_L);
        
        gyro_X = I2C_RD(MPU_ADDR, GYRO_XOUT_H);
        gyro_X = gyro_X << 8;
        gyro_X = gyro_X | I2C_RD(MPU_ADDR, GYRO_XOUT_L);
        
        gyro_Y = I2C_RD(MPU_ADDR, GYRO_YOUT_H);
        gyro_Y = gyro_Y << 8;
        gyro_Y = gyro_Y | I2C_RD(MPU_ADDR, GYRO_YOUT_L);
        
        gyro_Z = I2C_RD(MPU_ADDR, GYRO_ZOUT_H);
        gyro_Z = gyro_Z << 8;
        gyro_Z = gyro_Z | I2C_RD(MPU_ADDR, GYRO_ZOUT_L);
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



ISR(TIMER2_OVF_vect)
{
	TCNT2=0x00;
	t++;
	
}	

