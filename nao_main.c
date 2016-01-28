#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "motor.h"
#include "libconnect_julius.h"
#include "libget_word.h"
#include "playSound.h"
#include "onpa.h"
#include <sys/ioctl.h>

#define TRUE 1
#define FALSE 0

#define MAX_WORD 128

char word[MAX_WORD];

int isStop(){
	int prev_dist = 0;
	int cur_dist = 0;
	onpa_init();
	while(1){
		if((cur_dist = get_onpa_length(4)) == prev_dist){
			return TRUE;
		}else{
			prev_dist = cur_dist;
	}
	}
	return FALSE;
	onpa_close();
}



int existObject(){
	onpa_init();
	if(get_onpa_length(4) <= 100){
		return TRUE; 	
	}
	return FALSE;
	onpa_close();
}

int main(){
	motor_init();
	connectJulius(HOST_PORT);

	move_forward(0x4000);
	sleep(2);
	stop();
	

	while(1){
	printf("please speak\n");
	getWord(word);
	printf("%s\n",word);
		if(0 == strcmp(word, "こんにちは")){
			playSound("/home/pi/mod/tomo/voice_wav/konnichiwa.wav", 0);
			break;
		}
	}


	while(1){
		playSound("/home/pi/mod/tomo/voice_wav/ikuzo.wav", 0);
		getWord(word);
		printf("%s\n", word);

		if(0 == strcmp(word, "右")){
	  	playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			turn_right(0x1000);
			sleep(1);
			move_forward(0);

			if(isStop()){
				if(existObject()){
						playSound("/home/pi/mod/tomo/voice_wav/syougaibutu.wav", 0);
				}
				continue;
			}
		}

		if(0 == strcmp(word, "左")){
			playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			turn_left(0x1000);
			sleep(1);
			move_forward(0);
			if(isStop()){
				if(existObject()){
						playSound("/home/pi/mod/tomo/voice_wav/syougaibutu.wav", 0);
				}
				continue;
			}
		}

		if(0 == strcmp(word, "前")){
			if(existObject()){
				playSound("/home/pi/mod/tomo/voice_wav/buuchibuuuuuuchi.wav", 0);
				continue;
			}
			playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			move_forward(0x4000);
			if(isStop()){
				if(existObject()){
						playSound("/home/pi/mod/tomo/voice_wav/syougaibutu.wav", 0);
				}
				continue;
			}
		}

		if(0 == strcmp(word, "後ろ")){
			playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			sleep(5);
			move_backward(0x4000);
			sleep(10);
			move_forward(0);
//			if(isStop()){
				if(existObject()){
						playSound("/home/pi/mod/tomo/voice_wav/syougaibutu.wav", 0);
				}
				continue;
//			}
		}
/*
		if(0 == strcmp(word, "さよなら")){
			playSound("/home/pi/mod/tomo/voice_wav/sayonara.wav", 0);
			break;
		}
*/
	}
	closeJulius();
	motor_close();
	return 0;
}
