#ifndef CONFIG_H
#define CONFIG_H


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <linux/if_ether.h>
 
#ifdef HAVE_SYS_BUFMOD_H
	/*
	 * Size of a bufmod chunk to pass upstream; that appears to be the
	 * biggest value to which you can set it, and setting it to that value
	 * (which is bigger than what appears to be the Solaris default of 8192)
	 * reduces the number of packet drops.
	 */
#define	CHUNKSIZE	65536

	/*
	 * Size of the buffer to allocate for packet data we read; it must be
	 * large enough to hold a chunk.
	 */
#define	PKTBUFSIZE	CHUNKSIZE

#else /* HAVE_SYS_BUFMOD_H */

	/*
	 * Size of the buffer to allocate for packet data we read; this is
	 * what the value used to be - there's no particular reason why it
	 * should be tied to MAXDLBUF, but we'll leave it as this for now.
	 */
#define	MAXDLBUF	8192
#define	PKTBUFSIZE	(MAXDLBUF * sizeof(int))

#endif

/*默认缓冲区大小*/
#define BUFFER_SIZE     4096

/*redis 默认监听端口*/
#define DEA_PORT        6379


#ifndef bool
typedef enum {
    false,
    true,
} bool;
#endif

struct ether_header {
    unsigned char  ether_dhost[ETH_ALEN];      // destination eth addr
    unsigned char  ether_shost[ETH_ALEN];      // source ether addr
    unsigned short ether_type;                 // packet type ID field
} __attribute__ ((__packed__));


#endif /* CONFIG_H */
