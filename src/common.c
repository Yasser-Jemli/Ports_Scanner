#include "common.h"

void print_help_menu(int argc , char *argv[]){
    printf("Usage: %s -[OPT 1] [ARG 1]  -[OPT 2] ...\n", argv[0]);
    printf("Options:\n");
    printf("  -h        Show this help menu\n");
    printf("  -l        Scan the available ports in the host machine or if include -r argumenet scan range of pots in host machine\n");
    printf("  -p        Specify the IP address to scan\n");
    printf("  -r        Specify the range of ports to scan , ex 0-50 \n");
    printf("  -v        Enable verbose mode\n");
}


void get_timestamp(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}


void log_message(const char *level, bool verbose_mode, const char *format, ...) {
    va_list args;
    va_start(args, format);

    if (level == NULL || format == NULL) {
        fprintf(stderr, "ERROR: log_message received NULL argument!\n");
        return;
    }

    if (verbose_mode) {
        vfprintf(stdout, format, args);
        printf("\n");
    }

    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        vfprintf(log_file, format, args);
        fprintf(log_file, "\n");
        fclose(log_file);
    }

    va_end(args);
}

