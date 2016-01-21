#include <stdio.h>
#include "libget_word.h"
#include "libconnect_julius.h"

int main (){
	  printf("test start\n");
		connectJulius(HOST_PORT);
		printf("connect ok\n");
		char myword[128];
		int i;
		for (i = 0; i < 4; i++){
			printf("please speak\n");
			getWord(myword);
			printf("%s\n", myword);
		}
		closeJulius();
		printf("close ok\n");
		return 0;
}
