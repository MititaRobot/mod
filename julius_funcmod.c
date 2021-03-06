#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "julius_func.h"

void julius_init()
{
  int p_id;
  int status;
	char* str[] = {"/home/pi/dvd/julius/julius-4.2.3/julius/julius", "-C", "/home/pi/dvd/julius/julius-kits/dictation-kit-v4.2.3/word.jconf", "-charconv", "EUC-JP", "UTF-8", "-module", NULL};
  if((p_id = fork()) < 0){
    return;
  } else if (p_id == 0){
    execvp(str[0], str);
    fprintf(stderr, "fails\n");
  } else {
    waitpid(p_id, &status ,WNOHANG);
  }
  return;
}



void julius_get(char *myword)
{
  int s;//ソケット番号
  int  datalen;//データ長
  struct sockaddr_in skt;//相手ソケット
        char buf[16384] = {'\0'};
        in_port_t port;//相手ポート番号
        struct in_addr ipaddr;//相手IPアドレス
        socklen_t sktlen;//ソケットのサイズ

        //引数の初期化
        myword[0] = '\0';
        //IPアドレスを設定
        char ip[20] = HOST_ADDR;//192.168.0.124
        inet_aton(ip, &ipaddr);

        //ポート番号の設定
        port = 10500;
        skt.sin_family = AF_INET;
        skt.sin_port = htons(port);
        skt.sin_addr.s_addr = ipaddr.s_addr;
        //ソケットの作成
        if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket");
                exit(1);
        }
        printf("socket ok\n");
        //コネクション確立
        if (connect(s, (struct sockaddr *)&skt, sizeof(skt)) < 0) {
                perror("connect");
                exit(1);
        }
        printf("connected\n");
        //メイン
        while(myword[0] == '\0'){
          //クライアントへの接続
                sktlen = sizeof(skt);
                if (recv(s, buf, sizeof(buf), 0) < 0) {
                        perror("recv");
                        exit(1);
                }
                char *ret;//first parse

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
        close(s);
        return;
}
