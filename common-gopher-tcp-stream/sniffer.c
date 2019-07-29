#include <sys/types.h> 
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/filter.h>
#include "config.h"
#include "parse.h"
#include "common.h"


int
check_argv(int argc, char *argv[])
{
    int    opt;

    if(argc > 2) {
        printf("Usage: ./sniffer [-p port]\n");
        return 0;
    }
    while((opt = getopt(argc, argv, "p:")) != -1) {
        switch(opt) {

        case 'p':
            port = validate_port(optarg);
            break;
        default:
            printf("Usage: ./sniffer [-p port]\n");
            return 0;
        }
    }
    return 1;
}

void
filter_port(int sock_raw, int port)
{
    struct sock_filter bpf_code[] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 0, 6, 0x000086dd },
        { 0x30, 0, 0, 0x00000014 },
        { 0x15, 0, 15, 0x00000006 },
        { 0x28, 0, 0, 0x00000036 },
        { 0x15, 12, 0, port },
        { 0x28, 0, 0, 0x00000038 },
        { 0x15, 10, 11, port },
        { 0x15, 0, 10, 0x00000800 },
        { 0x30, 0, 0, 0x00000017 },
        { 0x15, 0, 8, 0x00000006 },
        { 0x28, 0, 0, 0x00000014 },
        { 0x45, 6, 0, 0x00001fff },
        { 0xb1, 0, 0, 0x0000000e },
        { 0x48, 0, 0, 0x0000000e },
        { 0x15, 2, 0, port },
        { 0x48, 0, 0, 0x00000010 },
        { 0x15, 0, 1, port },
        { 0x6, 0, 0, 0x00040000 },
        { 0x6, 0, 0, 0x00000000 },
    };
    struct sock_fprog filter;

    filter.len = sizeof(bpf_code)/sizeof(bpf_code[0]);
    filter.filter = bpf_code;
    if (setsockopt(sock_raw, SOL_SOCKET, SO_ATTACH_FILTER, &filter, sizeof(filter)) == -1) {
        perror("setsockopt");
        close(sock_raw);
        exit(1);
    }

}

int
main(int argc, char *argv[])
{
    int             sock_raw;
    int              data_size;
    unsigned int     saddr_size;
    struct sockaddr  saddr;
    unsigned char   *buffer;
    char            *data;

    if (!check_argv(argc, argv)) {
        return -1;
    }

    buffer = (unsigned char *)malloc(PKTBUFSIZE);

    /*接受所有的ip数据帧*/
    sock_raw = socket(PF_PACKET , SOCK_RAW , htons(ETH_P_IP));
    if(sock_raw < 0) {
        die("Socket Error");
    }
    filter_port(sock_raw, port);

    saddr_size = sizeof(saddr);
    for(;;) {
        memset(buffer, '\0', BUFFER_SIZE);

        data_size = recvfrom(sock_raw , buffer , BUFFER_SIZE , 0 , &saddr , &saddr_size);
        if(data_size < 0) {
            die("Recvfrom error , failed to get packets");
        }
        if(data_size > 0) {
            redis_process_packet(buffer, data_size);
        }
    }

    free(buffer);
    close(sock_raw);
    return 0;
}
