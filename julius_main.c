#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "julius_func.h"

int main(int argc, char **argv){
  int s;
	char word[128];
	word[0] = '\0';

	connectJulius(i);
	julius_get(s, word);
	printf("%s/n", word);
	close(s);
  return 0;
}
