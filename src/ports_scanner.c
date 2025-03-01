#include "ports_scanner.h"
#include "common.h"


int is_port_open(const char *ip_address, int port) {
    int sock;
    struct sockaddr_in server;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set up server address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Convert IP address
    if (inet_pton(AF_INET, ip_address, &server.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sock);
        return -1;
    }

    // Check connection
    int result = connect(sock, (struct sockaddr *)&server, sizeof(server));
    close(sock);

    return (result == 0) ? PORT_ACTIVE : PORT_INACTIVE;
}

void *scan_port_thread(void *args) {
    if (!args) {
        log_message("ERROR", 1, "Thread received NULL args");
        return NULL;
    }

    thread_args_t *thread_args = (thread_args_t *)args;
    
    while (1) {
        // Atomically get the next port number
        int port = atomic_fetch_add(&thread_args->current_port, 1);
        if (port > thread_args->max_port) {
            break; // Stop if all ports are scanned
        }

        if (!thread_args->ip_address) {
            log_message("ERROR", 1, "IP Address is NULL in scan_port_thread");
            return NULL;
        }

        // Scan port
        int status = is_port_open(thread_args->ip_address, port);
        if (status == PORT_ACTIVE) {
            // log_message("INFO", thread_args->verbose_mode, "IP: %s -> Port %d is OPEN", thread_args->ip_address, port);
            fprintf(stdout , "IP: %s -> Port %d is OPEN\n", thread_args->ip_address, port);
        } else if (status == PORT_INACTIVE) {
            // log_message("INFO", thread_args->verbose_mode, "IP: %s -> Port %d is CLOSED", thread_args->ip_address, port);
            fprintf(stdout , "IP: %s -> Port %d is CLOSED\n", thread_args->ip_address, port);
        } else {
            // log_message("ERROR", thread_args->verbose_mode, "IP: %s -> Error with Port %d", thread_args->ip_address, port);
            fprintf(stderr , "IP: %s -> Error with Port %d\n", thread_args->ip_address, port);
        }
    }

    return NULL;
}

void scanning_all_ports(const char *ip_address, bool verbose_mode) {
    pthread_t threads[MAX_THREADS];

    thread_args_t args;
    args.ip_address = ip_address;
    atomic_init(&args.current_port, 1);  // Start scanning from port 1
    args.max_port = MAX_PORT;
    args.verbose_mode = verbose_mode;

    // Create threads
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, scan_port_thread, &args) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    // Join threads
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}



void scanning_range_of_ports(const char *ip_address, char *port_range, bool verbose_mode) {
    int start_port, end_port;

    // Parse the port range
    char *token = strtok(port_range, "-");
    if (token == NULL) {
        log_message("ERROR", verbose_mode, "Invalid port range format: %s", port_range);
        return;
    }
    start_port = atoi(token);

    token = strtok(NULL, "-");
    if (token == NULL) {
        log_message("ERROR", verbose_mode, "Invalid port range format: %s", port_range);
        return;
    }
    end_port = atoi(token);

    // Validate range
    if (start_port <= 0 || end_port > MAX_PORT || start_port > end_port) {
        log_message("ERROR", verbose_mode, "Invalid port range: %d-%d", start_port, end_port);
        return;
    }

    log_message("INFO", verbose_mode, "Scanning Range of Ports from %d to %d", start_port, end_port);

    pthread_t threads[MAX_THREADS];
    pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    thread_args_t args = {ip_address, start_port, end_port, verbose_mode, &queue_mutex};

    // Create worker threads
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, scan_port_thread, &args) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queue_mutex);
}