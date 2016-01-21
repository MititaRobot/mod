#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libconnect_julius.h"
#include "libget_word.h"

int s;//ソケット番号
struct sockaddr_in skt;//相手ソケット

void connectJulius(in_port_t port){//相手ポート番号
	struct in_addr ipaddr;//相手IPアドレス

	//IPアドレスを設定
  char ip[20] = HOST_ADDR;//192.168.0.124  
	inet_aton(ip, &ipaddr);

	//ポート番号の設定
	skt.sin_family = AF_INET;
	skt.sin_port = htons(port);
	skt.sin_addr.s_addr = ipaddr.s_addr;

	//ソケットの作成
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket\n");
		exit(1);
	}

	printf("socket ok\n");

	//コネクション確立
	if (connect(s, (struct sockaddr *)&skt, sizeof(skt)) < 0){
		perror("connect\n");
		exit(1);
	}
	printf("connected\n");
}			

void closeJulius(){
	close(s);
	
}

