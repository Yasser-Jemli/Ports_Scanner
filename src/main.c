#include "common.h"
#include "ports_scanner.h"

int verbose_mode = VERBOSE_INACTIVE;


int main(int argc , char *argv[]){
    int opt ; 

    while((opt = getopt(argc , argv , "hvl")) != -1 ){
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
                scanning_hostmachine_ports(LOCALHOST_IP);
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
}



