#include <inttypes.h>

#ifndef _I2C_H
#define _I2C_H

	void i2c_init();
	unsigned char i2c_start(unsigned char adress);
	void i2c_start_wait(unsigned char address);
	unsigned char i2c_rep_start(unsigned char address);
	void i2c_stop(void);
	unsigned char i2c_write( unsigned char data);
	unsigned char i2c_readAck(void);
	unsigned char i2c_readNak(void);


#endif

#ifndef __I2C_COM_H_
#define __I2C_COM_H_

void I2C_WR(uint8_t ADDRESS,uint8_t DATA_REG,uint8_t DATA);
uint8_t I2C_RD(uint8_t ADDRESS,uint8_t DATA_REG);

#endif
