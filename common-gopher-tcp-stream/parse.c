#include "config.h"
#include "common.h"
#include "parse.h"


unsigned short  port = DEA_PORT;


static void __parse_tcp_packet__(unsigned char* Buffer, int size, int offset);

void
redis_process_packet(unsigned char* buffer, int size)
{
    int      offset = 0;
    char *   data = NULL;

    offset += ETH_HLEN;

    struct iphdr* iph = (struct iphdr*)(buffer+offset);
    switch (iph->protocol) { 

    case IPPROTO_ICMP:          /*ICMP Protocol*/
    case IPPROTO_IGMP:          /*IGMP Protocol*/
        break;
    case IPPROTO_TCP:           /*TCP Protocol*/
        __parse_tcp_packet__(buffer, size, offset);
        break;
    case IPPROTO_UDP:           /*UDP Protocol*/
        break;
    default:                    /*Some Other Protocol like ARP etc.*/
        break;
    }

}

void
__parse_tcp_packet__(unsigned char* buffer, int size, int offset)
{
    struct tcphdr     *tcph;
    struct iphdr      *iph;
    unsigned short     iphdrlen;
    struct MsgHeader  *msgh;
    char              *data = NULL;

    iph = (struct iphdr*)(buffer+offset);
    iphdrlen = iph->ihl*4;
    offset += iphdrlen;
    tcph = (struct tcphdr*)(buffer + offset);
    unsigned short s_port = ntohs(tcph->source);
    unsigned short d_port = ntohs(tcph->dest);

    offset += tcph->doff*4;
    if (size == offset) {            /*it's a TCP packet*/
        return;
    }

    if (s_port == port) {
        return;
    }
    for (int i = offset; i < size; i++) {
        printf("%%%02x", buffer[i]);
    }
    fflush(stdout);

}

