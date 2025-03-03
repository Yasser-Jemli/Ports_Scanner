#include "common.h"
#include "ports_scanner.h"

bool verbose_mode = VERBOSE_INACTIVE;
bool localhostssearch = LOCALHOSTSEARCHNOTSELECTED;
bool ipadresssearch = IPADDRESSSEARCHNOTSELECTED;
bool rangeportssearch = RANGEPORTSSEARCHNOTSELECTED;

void handle_options(int argc, char *argv[], char **ip_address, char **rangeofports) {
    if (argc == 1){
        fprintf(stderr , "Error : No arguments provided \n");
        fprintf(stderr , "Kindly run the program with -h option to see the help menu \n");
        exit(STATUS_ERROR);
    }
    int opt;
    while ((opt = getopt(argc, argv, "hvlp:r:")) != -1) {
        switch (opt) {
            case 'a':
                printf("Option a is selected \n");
                break;
            case 'h':
                printf("***********Display Help Menu *********** \n");
                print_help_menu(argc, argv);
                exit(STATUS_SUCCESS);
            case 'l':
                printf("Option l is selected \n");
                localhostssearch = LOCALHOSTSEARCHSELECTED;
                *ip_address = LOCALHOST_IP;
                break;
            case 'p':
                printf("Option p is selected \n");
                *ip_address = optarg;
                ipadresssearch = IPADDRESSSEARCHSELECTED;
                break;
            case 'r':
                printf("Option r is selected \n");
                rangeportssearch = RANGEPORTSSEARCHSELECTED;
                *rangeofports = optarg;
                break;
            case 'v':
                printf("Option v is selected \n");
                verbose_mode = VERSBOSE_ACTIVE;
                break;
            case '?':
                printf("Invalid option \n");
                printf("Kindly run the program with -h option to see the help menu \n");
                exit(STATUS_ERROR);
        }
    }
}

void perform_scans(char *ip_address, char *rangeofports) {
    int start_port, end_port;
    if (localhostssearch && rangeportssearch) {
        parse_port_range(rangeofports, &start_port,&end_port);
        scan_ports_multithreaded(ip_address, start_port, end_port);
    } else if (localhostssearch) {
        scan_ports_multithreaded(ip_address, START_PORT, MAX_PORTS);
    } else if (ipadresssearch && rangeportssearch) {
        parse_port_range(rangeofports, &start_port, &end_port);
        scan_ports_multithreaded(ip_address, start_port, end_port);
    } else if (ipadresssearch) {
        scan_ports_multithreaded(ip_address, START_PORT, MAX_PORTS);
    } else {
        printf("No valid scan options selected. Use -h for help.\n");
        exit(STATUS_ERROR);
    }
}

int main(int argc, char *argv[]) {
    char *ip_address = NULL;
    char *rangeofports = NULL;


    handle_options(argc, argv, &ip_address, &rangeofports);
    
    if (ip_address == NULL) {
        log_message("ERROR", 1, "IP Address is NULL. Exiting...");
        return STATUS_ERROR;
    }
    
    perform_scans(ip_address, rangeofports);

    return STATUS_SUCCESS;
}