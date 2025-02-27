#include "common.h"


int main(int argc , char *argv[]){
    int opt ; 

    while((opt = getopt(argc , argv , "l")) != -1 ){
        switch(opt){
            case 'a':
                printf("Option a is selected \n");
                break ;
            case 'h':
                printf("***********Display Help Menu *********** \n");
                void print_help_menu(argc , argv);
                break;
        }        
    }
}



