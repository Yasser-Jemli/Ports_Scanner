#include "common.h"

void print_help_menu(int argc , char *argv[]){
    printf("Usage: %s -[OPT 1] [ARG 1]  -[OPT 2] ...\n", argv[0]);
    printf("Options:\n");
    printf("  -h        Show this help menu\n");
    printf("  -l        list a summarry of the available ports in the host machine\n");
    printf("  -v        Enable verbose mode\n");
}

void get_timestamp(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

void log_message(const char *level, const char *message, int print_to_stdout) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    char timestamp[20];
    get_timestamp(timestamp, sizeof(timestamp));

    // Format: [TIME] [LEVEL] message
    fprintf(log_file, "[%s] [%s] %s\n", timestamp, level, message);

    if (print_to_stdout) {
        fprintf(stdout ,"[%s] [%s] %s\n", timestamp, level, message);
    }

    fclose(log_file);
}