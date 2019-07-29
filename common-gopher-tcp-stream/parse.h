#ifndef PARSE_H
#define PARSE_H


#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>

extern unsigned short port;

void redis_process_packet(unsigned char* buffer, int size);


#endif /* PARSE_H */
