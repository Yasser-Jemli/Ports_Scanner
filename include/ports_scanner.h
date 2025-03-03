#ifndef PORTS_SCANNER_H
#define PORTS_SCANNER_H

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#define MAX_PORTS 65535  // Highest possible port number
#define LOCALHOST_IP  "127.0.0.1"
#define PORT_ACTIVE 1 
#define PORT_INACTIVE 0
#define MAX_THREADS 100
#define START_PORT 1

typedef struct {
    char ip_address[16];
    int start_port;
    int end_port;
} PortRange;

extern int open_ports[MAX_PORTS];
extern int open_ports_count;
extern pthread_mutex_t lock;


int is_port_open(const char *ip_address, int port);
void *scan_ports(void *arg);
void scan_ports_multithreaded(const char *ip_address, int start_port, int end_port);
void parse_port_range(const char *rangeport, int *start_port, int *end_port);

#endif