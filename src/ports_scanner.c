#include "ports_scanner.h"
#include "common.h"


// Shared data
int open_ports[MAX_PORTS];
int open_ports_count = 0;
pthread_mutex_t lock;

int is_port_open(const char *ip_address, int port) {
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address, &server.sin_addr) <= 0) {
        close(sock);
        return -1;
    }

    int result = connect(sock, (struct sockaddr *)&server, sizeof(server));
    close(sock);

    return (result == 0) ? PORT_ACTIVE : PORT_INACTIVE;
}

void *scan_ports(void *arg) {
    PortRange *range = (PortRange *)arg;
    printf("Scanning ports from %d to %d...\n", range->start_port, range->end_port);

    for (int port = range->start_port; port <= range->end_port; port++) {
        if (is_port_open(range->ip_address, port) == PORT_ACTIVE) {
            pthread_mutex_lock(&lock);
            open_ports[open_ports_count++] = port;
            pthread_mutex_unlock(&lock);
        }
    }

    return NULL;
}

void scan_ports_multithreaded(const char *ip_address, int start_port, int end_port) {
    int num_threads = (end_port - start_port + 1) / 1000;
    if ((end_port - start_port + 1) % 1000 != 0) {
        num_threads++;  // Handle remaining ports
    }

    pthread_t threads[num_threads];
    PortRange ranges[num_threads];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < num_threads; i++) {
        ranges[i].start_port = start_port + (i * 1000);
        ranges[i].end_port = (ranges[i].start_port + 999 > end_port) ? end_port : (ranges[i].start_port + 999);
        strncpy(ranges[i].ip_address, ip_address, sizeof(ranges[i].ip_address));

        if (pthread_create(&threads[i], NULL, scan_ports, &ranges[i]) != 0) {
            perror("Thread creation failed");
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("\n===== Open Ports =====\n");
    if (open_ports_count == 0) {
        printf("No open ports found.\n");
    } else {
        printf("Port\tStatus\n");
        printf("-------------------\n");
        for (int i = 0; i < open_ports_count; i++) {
            printf("%d\tOpen\n", open_ports[i]);
        }
    }
}

void parse_port_range(const char *rangeport, int *start_port, int *end_port) {
    char *dash = strchr(rangeport, '-');  // Find the '-' character
    if (dash == NULL) {
        fprintf(stderr, "Invalid port range format. Expected format: start-end (e.g., 5-50)\n");
        exit(EXIT_FAILURE);
    }

    *dash = '\0';  // Split the string at '-'
    *start_port = atoi(rangeport);   // Convert the first part to int
    *end_port = atoi(dash + 1);      // Convert the second part to int

    // Validate port numbers
    if (*start_port < 1 || *end_port > 65535 || *start_port > *end_port) {
        fprintf(stderr, "Invalid port range: %d-%d. Ports must be in range 1-65535.\n", *start_port, *end_port);
        exit(EXIT_FAILURE);
    }
}