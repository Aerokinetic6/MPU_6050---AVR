#define F_CPU 16000000
#define PI 3.14159265359
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
signed int gyro_X, gyro_Y, gyro_Z, accel_X, accel_Y, accel_Z;
float angle_X_GYR, angle_Y_GYR, angle_Z_GYR;  // angle pos. by GYRO (drifting)
float angle_X_ACC, angle_Y_ACC, angle_Z_ACC; // angle pos by ACCEL (unstable?)
float angle_X, angle_Y, angle_Z;             //Filtered angle positions

int main()
{
	uint8_t sec,min;

	port_init();
	I2C_init();
	UART_init();
	TIMER2_init();
	MPU_init();

	//sei();

	while(1){

	  read_MPU();

	  angle_X_GYR += (float)gyro_X/2000;
	  angle_Y_GYR += (float)gyro_Y/2000;
	  angle_Z_GYR += (float)gyro_Z/2000;

	  angle_X_ACC = ((atan2f(accel_Y, accel_Z))*180)/PI;
	  angle_Y_ACC = ((atan2f(accel_X, accel_Z))*180)/PI;

	  angle_X += (float)gyro_X/2000;
	  angle_Y += (float)gyro_Y/2000;

	  angle_X = 0.98*angle_X + 0.02*angle_X_ACC;
	  angle_Y = 0.98*angle_Y + 0.02*angle_Y_ACC;
		angle_Z = angle_Z_GYR;

	  led_out(t);

	  if (t > 50) {
			if (angle_X < 0.01) { UART_snd_byte('X');
														UART_snd_byte('-');
														UART_snd_float((-1)*angle_X);
													  UART_snd_byte('$');
													}
			else {								UART_snd_byte('X');
														UART_snd_float(angle_X);
														UART_snd_byte('$');
													}



			if (angle_Y < 0.01) { UART_snd_byte('Y');
														UART_snd_byte('-');
														UART_snd_float((-1)*angle_Y);
														UART_snd_byte('$');
													}
			else {								UART_snd_byte('Y');
														UART_snd_float(angle_Y);
														UART_snd_byte('$');
													}



			if (angle_Z < 0.01) { UART_snd_byte('Z');
														UART_snd_byte('-');
														UART_snd_float((-1)*angle_Z);
														UART_snd_byte('$');
													}
			else {								UART_snd_byte('Z');
														UART_snd_float(angle_Z);
														UART_snd_byte('$');
													}

	    t=0;
	  }
	 t++;
	 _delay_ms(1);





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
        accel_X = accel_X-ACC_X_OFFSET;

        accel_Y = I2C_RD(MPU_ADDR, ACCEL_YOUT_H);
        accel_Y = accel_Y << 8;
        accel_Y = accel_Y | I2C_RD(MPU_ADDR, ACCEL_YOUT_L);
        accel_Y = accel_Y-ACC_Y_OFFSET;

        accel_Z = I2C_RD(MPU_ADDR, ACCEL_ZOUT_H);
        accel_Z = accel_Z << 8;
        accel_Z = accel_Z | I2C_RD(MPU_ADDR, ACCEL_ZOUT_L);
        accel_Z = accel_Z-ACC_Z_OFFSET;

        gyro_X = I2C_RD(MPU_ADDR, GYRO_XOUT_H);
        gyro_X = gyro_X << 8;
        gyro_X = gyro_X | I2C_RD(MPU_ADDR, GYRO_XOUT_L);
        gyro_X = gyro_X-GYRO_X_OFFSET;

        gyro_Y = I2C_RD(MPU_ADDR, GYRO_YOUT_H);
        gyro_Y = gyro_Y << 8;
        gyro_Y = gyro_Y | I2C_RD(MPU_ADDR, GYRO_YOUT_L);
        gyro_Y = gyro_Y-GYRO_Y_OFFSET;

        gyro_Z = I2C_RD(MPU_ADDR, GYRO_ZOUT_H);
        gyro_Z = gyro_Z << 8;
        gyro_Z = gyro_Z | I2C_RD(MPU_ADDR, GYRO_ZOUT_L);
        gyro_Z = gyro_Z-GYRO_Z_OFFSET;
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
