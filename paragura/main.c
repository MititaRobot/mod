//61322125 室 航

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fts.h>
#include <pwd.h>
#include <grp.h>
#include <sys/fcntl.h>
#include "motor.h"
#include "compass.h"
#include "playSound.h"

#define NHASH 4
#define NBUF 12
#define WAIT_SEG_SIZE 5
#define MAXLEN 128
#define NARGS 16
#define NSTAT 6  /** statの種類数 */
#define SERVER_PORT 50021

typedef enum {
	S_AIKO = 0,S_BUUCHI,S_HARAHERI,S_IDOU,
	S_IKUZO,S_JANKEN,S_HELLO,S_MOKUHYOU,
	S_OC,S_SAYONARA
}SOUND;


void print_message(char* str,...)
{
    printf("mymurod: ");
    va_list va;
    va_start(va,str);
    vprintf(str,va);
    va_end(va);
    printf("\n");
}

void print_error(char* str,...)
{
    fprintf(stderr,"mymurod: ");
    va_list va;
    va_start(va,str);
    vfprintf(stderr,str,va);
    va_end(va);
    fprintf(stderr,"\n");
}



void play_sound(SOUND type)
{
	//0~9
	if (type < S_AIKO || type > S_SAYONARA) {
		print_error("Invalid sound type: %d",type);
		return;
	}

	const char* names[] = {"aiko.wav","buuchi.wav","haraheri.wav","idou.wav","ikuzo.wav","janken.wav","hello.wav","mokuhyou.wav","oc.wav","sayonara.wav"};
	
	char path[30];
	path[0] = '\0';
	strcpy(path,"music/");
	strcat(path,names[type]);
	playSound(path,WNOHANG);	

}

int muro_send(int sock_fd,char *str)
{
    int len = 0;
    if ((len = send(sock_fd,str, strlen(str), 0)) < 0) {
        print_error("send: %s",strerror(errno));
        return -1;
    }
    return len;
}


int muro_recv(int sock_fd,char* buf,int *receive_len)
{

    if ((*receive_len = recv(sock_fd,buf,MAXLEN,0)) < 1) {
        if (*receive_len == 0) {
            print_error("client halt!");
            exit(1);
        }
        print_error("recv: %s",strerror(errno));
        return -1;
    }
    buf[*receive_len] = '\0';
    print_message("%d %s",*receive_len,buf);
    return 0;
}

void convert_path_str(char* dst,char*base)
{
    dst[0] = '\0';
    if (strlen(base) == 0) {
        //baseがない場合
        return;
    }
    char *home = getenv("HOME");
    if (home == NULL) {
        //envがおかしい場合。
        strcpy(dst,"");
        return;
    }

    if (base[0] == '~' && (strlen(base) == 1 || base[1] == '/')) {
        //base = "~/..."か、"~"の場合
        strcpy(dst,home);
        if (strlen(base) == 1) {
            return;
        }
        base++;
    }
    strcat(dst,base);
}

void getargs(int *ac,char* av[],char *p)
{
    *ac = 0;
    while (1) {
        while (*p == ' ' || *p == '\t') { p++; }
        if (*p == '\0') { return; }
        av[(*ac)++] = p;
        while (*p != ' ' && *p != '\t' && *p != '\0') {
            p++;
        }
        if (*p == '\0') {
            return;
        }
        *p++ = '\0';
    }
}

int sock_fd = 0;
struct sockaddr_in my_sock;

void sigint_handler()
{
    print_message("close.");
    close(sock_fd);
    exit(1);
}
void mokuhyou()
{

	turn_left(80000);
	usleep(500000);
	turn_right(80000);
	usleep(500000);
	turn_left(80000);
	usleep(500000);
	turn_right(80000);
	usleep(500000);
	turn_left(75000);
	usleep(100000);
	stop();
}

void haraheri()
{

}

void hello()
{
	move_forward(80000);
	usleep(500000);
	move_backward(80000);
	usleep(500000);
	stop();
}


void init( struct sockaddr_in* my_sock) {

    if ((sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket");
        exit(1);
    }

    bzero(my_sock, sizeof (struct sockaddr_in));
    my_sock->sin_family = AF_INET;
    my_sock->sin_port = htons(SERVER_PORT);
    my_sock->sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_fd,(struct sockaddr*)my_sock, sizeof (struct sockaddr_in)) < 0) {
        print_error("bind: %s",strerror(errno));
        exit(1);
    }

    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGINT);
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        print_error("signal: %s",strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void in_child(int sock_fd)
{

		motor_init();


    char buf[MAXLEN];
    int ac;
    char*av[NARGS];
    int len = 0;
    int fd = 0;
    int i = 0;
    char *commands[] = {"move","rotate","hara","hello","moku","quit"};
    int command_num = 6;


    while (1) {
        bzero(buf,MAXLEN);
        if (muro_recv(sock_fd,buf,&len) < 0) { continue; }
        getargs(&ac,av,buf);
        if (ac == 0) { continue; }

        for (i = 0; i < command_num && strncmp(commands[i],av[0],strlen(commands[i])); i++);
				print_message("%d",i);
        if (i == 6) { continue; }

        switch (i) {
            case 0:
                move_forward(5000);
                break;
            case 1:
                turn_left(1000);
                break;
            case 2:
								play_sound(S_HARAHERI);
								haraheri();
                break;
            case 3:
                play_sound(S_HELLO);
								hello();
                break;
            case 4:
                play_sound(S_MOKUHYOU);
								mokuhyou();
                break;
            case 5:
                stop();
                exit(1);
                break;
            default:
                break;
        }
    }
}


int main(int argc, char* argv[])
{
    struct sockaddr_in my_sock,recv_sock;
    socklen_t sock_len = sizeof(recv_sock);
    init(&my_sock);

		play_sound(S_IKUZO);
		


    while (1) {
        if (listen(sock_fd,WAIT_SEG_SIZE) != 0) {
            print_error("listen: %s",strerror(errno));
            exit(1);
        }
        int s;
        if ((s = accept(sock_fd, (struct sockaddr *) &recv_sock, &sock_len)) < 0) {
            print_error("accept: %s", strerror(errno));
            exit(1);
        }
        print_message("connection accepted. source: %s:%u",inet_ntoa(recv_sock.sin_addr),(unsigned int) ntohs(recv_sock.sin_port));

        if (fork() == 0) {
            in_child(s);
        }
        else {

        }
    }
}
