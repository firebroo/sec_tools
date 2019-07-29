#ifndef COMMON_H
#define COMMON_H


#include <time.h>

void die(const char* error);

unsigned short validate_port(char *p);

char* current_time(void);

void long2ip(long ip, char *buf);


#endif /* COMMON_H */
