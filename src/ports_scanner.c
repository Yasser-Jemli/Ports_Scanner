#include "ports_scanner.h"
#include "common.h"

int is_port_open(const char *ip_address, int port) {
    int sock;
    struct sockaddr_in server;

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set up the server address structure
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Convert IP address from text to binary
    if (inet_pton(AF_INET, ip_address, &server.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sock);
        return -1;
    }

    // Try connecting to the port
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0) {
        close(sock);
        return 1;  // Port is open
    }

    close(sock);
    return 0;  // Port is closed
}


void scanning_hostmachine_ports(const char *ip_address , bool verbose_mode) {
    int port;
    int status;

    for (port = 1; port <= MAX_PORT; port++) {
        status = is_port_open(ip_address, port);
        if (status == PORT_ACTIVE) 
        {
            log_message("INFO", " IP Adress : %s -> Port %d is open", verbose_mode , ip_address ,port);
            
        } 
        
        else if (status == PORT_INACTIVE) 
        {
            log_message("INFO", "IP Adress : %s  Port %d is closed", verbose_mode ,ip_address ,port);
        }
        
        else
        {
            log_message("ERROR", "IP Adress : %s  - Facing Error with Port %d", verbose_mode , ip_address,port);
        }
        
    }
}