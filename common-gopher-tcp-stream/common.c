#include "config.h"
#include "common.h"

void
die (const char* error) {
    fprintf(stderr, "%s\n", error);
    exit(-1);
}

unsigned short
validate_port(char *p)
{
    int     port = atoi(p);

    if(port <= 0 || port > 0xffff){
        die("Invalid Port");
    }

    /* return port number if its valid */
    return (unsigned short)port;
}

char*
current_time(void)
{
    struct tm   *p;
    time_t       timep;
    char         buf[BUFFER_SIZE];

    time(&timep);
    p = gmtime(&timep);
    sprintf(buf, "%d-%d-%d %02d:%02d:%02d",
                 1900 + p->tm_year,
                 1 + p->tm_mon,
                 p->tm_mday,
                 p->tm_hour + 8,
                 p->tm_min,
                 p->tm_sec);
    return strdup(buf);
}

void
long2ip (long ip, char *buf)
{
    sprintf (buf, "%ld.%ld.%ld.%ld",
                  ((0xFF  << 24) & ip) >> 24,
                  ((0xFF << 16) & ip) >> 16,
                  ((0xFF << 8) & ip) >> 8,
                  ip & 0xFF);
}
