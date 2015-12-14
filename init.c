#include "init.h"



void all_init()
{

	motor_init();
	light_init();
	acce_init();
	onpa_init();	
}


void all_close()
{
	motor_close();
	light_init();
	acce_close();
	onpa_close();	
}

