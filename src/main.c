#include "common.h"
#include "ports_scanner.h"

bool verbose_mode = VERBOSE_INACTIVE;
bool localhostssearch = LOCALHOSTSEARCHNOTSELECTED;
bool ipadresssearch = IPADDRESSSEARCHNOTSELECTED;
bool rangeportssearch = RANGEPORTSSEARCHNOTSELECTED;

void handle_options(int argc, char *argv[], char **ip_address, char **rangeofports) {
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
            default:
                printf("Invalid option \n");
                printf("Kindly run the program with -h option to see the help menu \n");
                exit(STATUS_ERROR);
        }
    }
}

void perform_scans(char *ip_address, char *rangeofports) {
    if (localhostssearch && rangeportssearch) {
        scanning_range_of_ports(LOCALHOST_IP, rangeofports, verbose_mode);
    } else if (localhostssearch) {
        scanning_all_ports(LOCALHOST_IP, verbose_mode);
    } else if (ipadresssearch && rangeportssearch) {
        scanning_range_of_ports(ip_address, rangeofports, verbose_mode);
    } else if (ipadresssearch) {
        scanning_all_ports(ip_address, verbose_mode);
    } else {
        printf("No valid scan options selected. Use -h for help.\n");
        exit(STATUS_ERROR);
    }
}

int main(int argc, char *argv[]) {
    char *ip_address = NULL;
    char *rangeofports = NULL;

    handle_options(argc, argv, &ip_address, &rangeofports);
    perform_scans(ip_address, rangeofports);

    return STATUS_SUCCESS;
}