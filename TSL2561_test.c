#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "light.h"
#include "TSL2561.h"

void light_init(void);
void light_close(void);
void get_light(uint32_t *,uint16_t *,uint16_t *);
uint32_t get_lux(void);
TSL2561 light;



void light_close()
{
	TSL2561_CLOSE(&light);
}

uint32_t get_lux()
{
	uint16_t broadband, ir;
	uint32_t lux=0;
	int	rc = TSL2561_SENSELIGHT(&light, &broadband, &ir, &lux, 1);
	return lux;
}

void get_light(uint32_t* lux,uint16_t* broadband,uint16_t* ir)
{
	int rc = TSL2561_SENSELIGHT(&light, broadband, ir, lux, 1);
}

int main() {
	int rc;
	uint16_t broadband, ir;
	uint32_t lux=0;
	
	TSL2561 light;
	get_light(&lux,&broadband,&ir);
	fprintf(stderr,"Test. RC: %i(%s), broadband: %i, ir: %i, lux: %i\n", rc, strerror(light.lasterr), broadband, ir, lux);
	light_close();
	
	return 0;
}

void light_init()
{
	int rc = 0;
	light.adapter_nr = 1;
	light.integration_time = TSL2561_INTEGRATIONTIME_402MS;
	light.gain = TSL2561_GAIN_16X;
	light.adapter_fd = -1;
	light.sensor_addr = TSL2561_ADDR_FLOAT;
	light.lasterr = 0;
//light.buf = {0};
	if ((rc = TSL2561_OPEN(&light)) != 0) {
		fprintf(stderr, "Error initializing TSL2561 sensor (%s). Check your i2c bus (es. i2cdetect)\n", strerror(light.lasterr));
		light_close();
	}
	rc = TSL2561_SETGAIN(&light, TSL2561_GAIN_1X);
	rc = TSL2561_SETINTEGRATIONTIME(&light, TSL2561_INTEGRATIONTIME_101MS);
}
