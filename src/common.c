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


void log_message(const char *level, const char *format, bool verbose_mode, ...) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    char timestamp[20];
    get_timestamp(timestamp, sizeof(timestamp));

    va_list args;
    va_start(args, verbose_mode);

    if (verbose_mode) {
        printf("- %s - [%s] ", timestamp, level); 
        vprintf(format, args);                    
        printf("\n");                             
    }

    fprintf(log_file, "- %s - [%s] ", timestamp, level);  
    vfprintf(log_file, format, args);                  
    fprintf(log_file, "\n");                          

    va_end(args);
    fclose(log_file);
}
