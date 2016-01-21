#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include "libget_word.h"
#include "libconnect_julius.h"

void getWord(char *myword)
{

	int  datalen;//データ長
	char buf[16384] = {'\0'};
	socklen_t sktlen;//ソケットのサイズ

	//引数の初期化
  myword[0] = '\0';
  //メイン  
	while(myword[0] == '\0'){
		//クライアントへの接続
		sktlen = sizeof(skt);
		if (recv(s, buf, sizeof(buf), 0) < 0) {
			perror("recv");
			exit(1);
		}
		char *ret;//first parse

		//音声認識結果のWORDタグの部分をパース
		if ((ret = strstr(buf, "WORD")) != NULL) {
			ret += 6;
			int i;
			for (i = 0; *(ret + i) != '"'; i++) {
				myword[i] = *(ret + i);
			}
			myword[i] = '\0';
		}
		memset(&buf, 0, sizeof(buf));//バッファの再初期化
	}
}
