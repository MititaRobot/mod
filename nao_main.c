#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "motor.h"
#include "libconnect_julius.h"
#include "libget_word.h"
#include "playSound.h"
#include "onpa.h"
#include <sys/ioctl.h>
#include "camera.h"
#include <opencv/highgui.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

#define TRUE 1
#define FALSE 0
#define MAX_WORD 128
#define MISSION_COMPLETED 0

char word[MAX_WORD];

void init(){
	motor_init();
	connectJulius(HOST_PORT);
	onpa_init();
}

void deinit(){
	closeJulius();
	motor_close();
	onpa_close();
}

int spokenStop(){
		getWord(word);
		printf("%s\n",word);
		if(0 == strcmp(word, "止まってください")){
			return TRUE;
		}
		return FALSE;
}

int identifyObjectIsRed(){
	int result;
	camera_init(160,120);
	camera_capture();
	result = identifyRed();
	camrea_close();
	return result;
}

/*
int isStop(){
	int prev_dist = 0;
	int cur_dist = 0;
	onpa_init();
	while(1){
		usleep(500);
		if((cur_dist = get_onpa_length(4)) == prev_dist){
			onpa_close();
			return TRUE;
		}
		prev_dist = cur_dist;
	}
}
*/

int transfer(){
	if(0 == strcmp(word, "右")){
	 	 	playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			turn_right(0x4000);
			return TRUE;
	}
	if(0 == strcmp(word, "左")){
			playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			turn_left(0x4000);
			return TRUE;
	}
	if(0 == strcmp(word, "前")){
			playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			move_forward(0x4000);
			return TRUE;
	}
	if(0 == strcmp(word, "後ろ")){
			playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			move_backward(0x4000);
			return TRUE;
	}	
	return FALSE;
}

int existObject(){
	if(get_onpa_length(4) <= 100){
		return TRUE; 	
	}
	return FALSE;
}

int main(){
//init
	initAll();

//konnichiwa
	while(1){
		printf("please speak konnichiwa\n");
		getWord(word);
		printf("%s\n",word);
		if(0 == strcmp(word, "こんにちは")){
			playSound("/home/pi/mod/tomo/voice_wav/konnichiwa.wav", 0);
			break;
		}
	}

//transfer
	while(1){
		playSound("/home/pi/mod/tomo/voice_wav/ikuzo.wav", 0);
		getWord(word);
		printf("%s\n", word);

		if(!transfer()){
			continue;	
		}
		
		while(1){
			if(existObject()){
				stop();
				if(identifyObjectIsRed()){
					playSound("/home/pi/mod/tomo/voice_wav/mokuhyobutu.wav", 0);
					MISSION_COMPLETED = TRUE;
					break;
				}else{
					playSound("/home/pi/mod/tomo/voice_wav/syougaibutu.wav", 0);
					break;
				}
			}
			if(spokenStop()){
				playSound("/home/pi/mod/tomo/voice_wav/haraheri.wav", 0);
				break;
			}
		}

		if(MISSION_COMPLETED == TRUE){
			break;	
		}
	}
	
//sayonara
	while(1){
		printf("please speak sayonara\n");
		getWord(word);
		printf("%s\n",word);
		if(0 == strcmp(word, "さよなら")){
			playSound("/home/pi/mod/tomo/voice_wav/sayonara.wav", 0);
			break;
		}
	}

//deinit
	deinit();
	
	return 0;
}
	
/*
}
		if(0 == strcmp(word, "右")){
		  	playSound("/home/pi/mod/tomo/voice_wav/idou.wav", 0);
			turn_right(0x1000);
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

		if(0 == strcmp(word, "さよなら")){
			playSound("/home/pi/mod/tomo/voice_wav/sayonara.wav", 0);
			break;
		}

	}
	return 0;
}
*/
