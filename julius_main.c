#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "julius_func.h"

#define HOST_PORT 10500
#define HOST_ADDR "192.168.0.123"

int main(int argc, char **argv){

  
	char word[128];
	julius_get(word);
	printf("%s/n", word);
  return 0;
}
