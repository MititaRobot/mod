#include "playSound.h"
#include <sys/wait.h>

int main()
{
	// wait child process
	playSound("/home/pi/mod/tomo/voice_wav/oc.wav", 0);

	// NOHANG child process
	playSound("/home/pi/mod/tomo/voice_wav/oc.wav", WNOHANG);

	return 0;
}
