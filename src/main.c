#include "common.h"
#include "ports_scanner.h"

bool verbose_mode = VERBOSE_INACTIVE;
bool localhostssearch = LOCALHOSTSEARCHNOTSELECTED;
bool ipadresssearch = IPADDRESSSEARCHNOTSELECTED;

int main(int argc , char *argv[]){
    int opt ;
    char *ip_address = NULL; 

    while((opt = getopt(argc , argv , "hvlp:")) != -1 ){
        switch(opt){
            case 'a':
                printf("Option a is selected \n");
                break ;
            case 'h':
                printf("***********Display Help Menu *********** \n");
                print_help_menu(argc , argv);
                break;
            case 'l':
                printf("Option l is selected \n");
                localhostssearch = LOCALHOSTSEARCHSELECTED;
                break ;
            case 'p':
                printf("Option p is selected \n");
                ip_address = optarg;
                ipadresssearch = IPADDRESSSEARCHSELECTED;
                break ;
            case 'v':
                printf("Option v is selected \n");
                verbose_mode = VERSBOSE_ACTIVE;
                break ;
            default : 
                printf("Invalid option \n");
                printf("Kindly run the program with -h option to see the help menu \n");
                return STATUS_ERROR ;
        }     
    }

    if (localhostssearch){
        scanning_hostmachine_ports(LOCALHOST_IP, verbose_mode);
    }

    if(ipadresssearch){
        scanning_hostmachine_ports(ip_address, verbose_mode);
    }
}
