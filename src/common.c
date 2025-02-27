#include "common.h"

void print_help_menu(int argc , char *argv[]){
    printf("Usage: %s [-h] [-f filename] [-v]\n", argv[0]);
    printf("Options:\n");
    printf("  -h        Show this help message\n");
    printf("  -l        list a summarry of the available ports in the host machine\n");
    printf("  -v        Enable verbose mode\n");
}