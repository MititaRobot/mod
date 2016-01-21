#include<stdio.h>

int main()
{
	int wait = 1;
	char *command[] = {"aplay", "-D", "plughw:0,0", "/home/pi/mod/tomo/voice_wav/oc.wav", NULL};
	soundplay(command, wait);

}
