#ifndef PORTS_SCANNER_H
#define PORTS_SCANNER_H

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define MAX_PORT 65535  // Highest possible port number
#define LOCALHOST_IP  "127.0.0.1"
#define PORT_ACTIVE 1 
#define PORT_INACTIVE 0


int is_port_open (const char *ip_address , int port);
void scanning_all_ports(const char *ip_address , bool verbose_mode);

#endif