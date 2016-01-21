#ifndef LIBCONNECT_JULIUS_H
#define LIBCONNECT_JULIUS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define HOST_ADDR "192.168.0.123"
extern int s;
extern struct sockaddr_in skt;//相手ソケット
extern void connectJulius(in_port_t port);
extern void closeJulius();

#endif
