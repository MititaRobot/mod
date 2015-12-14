#include <stdint.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "compass.h"



int acce_fd = 0;

void acce_init()
{
	
	char *filename = "/dev/i2c-1";
	if ((acce_fd= open(filename,O_RDWR)) == -1) {
        	printf("Unable to open I2C bus!");
          exit(1);
	}

  writeAccReg(CTRL_REG1_XM, 0b01100111); //  z,y,x axis enabled, continuos update,  100Hz data rate
  writeAccReg(CTRL_REG2_XM, 0b00100000); // +/- 16G full scale
  writeMagReg( CTRL_REG5_XM, 0b11110000);   // Temp enable, M data rate = 50Hz
  writeMagReg( CTRL_REG6_XM, 0b01100000);   // +/-12gauss
  writeMagReg( CTRL_REG7_XM, 0b00000000);   // Continuous-conversion mode
  writeGyrReg(CTRL_REG1_G, 0b00001111); // Normal power mode, all axes enabled
  writeGyrReg(CTRL_REG4_G, 0b00110000); // Continuos update, 2000 dps full scale
}

void acce_close()
{
	close(acce_fd);
}

float get_heading()
{
	static int magRaw[3];
	readMAG(magRaw);
	magRaw[1] = -magRaw[1];
	float heading = 180 * atan2(magRaw[1],magRaw[0])/M_PI;
  if(heading < 0)
	  heading += 360;
	return heading;
}


int main(int argc, char *argv[])
{

	acce_init();
	printf("%lf\n",get_heading());
	acce_close();

}

void  readBlock(uint8_t command, uint8_t size, uint8_t *data)
{
    if (i2c_smbus_read_i2c_block_data(acce_fd, command, size, data) != size) {
       printf("Failed to read block from I2C.");
        exit(1);
    }
}

void selectDevice(int file, int addr)
{
        if (ioctl(file, I2C_SLAVE, addr) < 0) {
		 printf("Failed to select I2C device.");
        }
}


void readACC(int  *a)
{
        uint8_t block[6];
        selectDevice(acce_fd,ACC_ADDRESS);
        readBlock(0x80 | OUT_X_L_A, sizeof(block), block);

        *a = (int16_t)(block[0] | block[1] << 8);
        *(a+1) = (int16_t)(block[2] | block[3] << 8);
        *(a+2) = (int16_t)(block[4] | block[5] << 8);

}


void readMAG(int  *m)
{
        uint8_t block[6];
        selectDevice(acce_fd,MAG_ADDRESS);
        readBlock(0x80 | OUT_X_L_M, sizeof(block), block);

        *m = (int16_t)(block[0] | block[1] << 8);
        *(m+1) = (int16_t)(block[2] | block[3] << 8);
        *(m+2) = (int16_t)(block[4] | block[5] << 8);

}


void readGYR(int *g)
{
	uint8_t block[6];
        selectDevice(acce_fd,GYR_ADDRESS);
	readBlock(0x80 | OUT_X_L_G, sizeof(block), block);

        *g = (int16_t)(block[0] | block[1] << 8);
        *(g+1) = (int16_t)(block[2] | block[3] << 8);
        *(g+2) = (int16_t)(block[4] | block[5] << 8);
}


void writeAccReg(uint8_t reg, uint8_t value)
{
    selectDevice(acce_fd,ACC_ADDRESS);
    if (i2c_smbus_write_byte_data(acce_fd, reg, value) == -1) {
        printf ("Failed to write byte to I2C Acc.");
        exit(1);
    }
}

void writeMagReg(uint8_t reg, uint8_t value)
{
    selectDevice(acce_fd,MAG_ADDRESS);
		if (i2c_smbus_write_byte_data(acce_fd, reg, value) == -1) {
        printf("Failed to write byte to I2C Mag.");
        exit(1);
    }
}


void writeGyrReg(uint8_t reg, uint8_t value)
{
		selectDevice(acce_fd,GYR_ADDRESS);
		if (i2c_smbus_write_byte_data(acce_fd, reg, value) == -1) {
			printf ("Failed to write byte to I2C Mag.");
			exit(1);
	}
}


