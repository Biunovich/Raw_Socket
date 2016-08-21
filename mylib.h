#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <linux/if_ether.h>
#include <error.h>
#include <linux/if_packet.h>
#include <linux/if_arp.h>

#define D_PORT 1337
#define S_PORT 1488
#define S_IP "127.0.0.1"
#define D_IP "127.0.0.1"
#define MSGLEN 512

struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t udp_length;
};