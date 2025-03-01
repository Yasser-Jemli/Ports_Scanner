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
        return PORT_ACTIVE;  // Port is open
    }

    close(sock);
    return PORT_INACTIVE;  // Port is closed
}

void *scan_port_thread(void *args) {
    thread_args_t *thread_args = (thread_args_t *)args;

    while (1) {
        int port;

        // Lock mutex to get the next port
        pthread_mutex_lock(thread_args->queue_mutex);
        if (thread_args->current_port > thread_args->max_port) {
            pthread_mutex_unlock(thread_args->queue_mutex);
            break;
        }
        port = thread_args->current_port++;
        pthread_mutex_unlock(thread_args->queue_mutex);

        // Scan the port
        int status = is_port_open(thread_args->ip_address, port);
        if (status == PORT_ACTIVE) {
            log_message("INFO", thread_args->verbose_mode, "IP Address: %s -> Port %d is open", thread_args->ip_address, port);
        } else if (status == PORT_INACTIVE) {
            log_message("INFO", thread_args->verbose_mode, "IP Address: %s -> Port %d is closed", thread_args->ip_address, port);
        } else {
            log_message("ERROR", thread_args->verbose_mode, "IP Address: %s -> Error with Port %d", thread_args->ip_address, port);
        }
    }

    return NULL;
}

void scanning_all_ports(const char *ip_address, bool verbose_mode) {
    pthread_t threads[MAX_THREADS];
    pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

    thread_args_t args = {ip_address, 1, MAX_PORT, verbose_mode, &queue_mutex};

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