#include <stdio.h>
#include <stdlib.h>
#include "motor.h"

int main(int argc, char * argv[]) 
{
	motor_init();

	move_forward(5000);


	sleep(4);

	move_forward(0);

  motor_close();

	return 0;
}
