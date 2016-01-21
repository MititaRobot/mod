#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <unistd.h>         /* for read, write, close */
#include "/usr/include/linux/i2c-dev.h"
#include "onpa.h"

int main()
{
	int i = 0;
	onpa_init();
	for (i = 0; i < 5; i++) {
		printf("2: %d\n", get_onpa_length(2));
		printf("4: %d\n", get_onpa_length(4));
		printf("6: %d\n", get_onpa_length(6));
		printf("8: %d\n", get_onpa_length(8));
	}
	onpa_close();	
	return 0;
}

