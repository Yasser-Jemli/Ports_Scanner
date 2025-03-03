#include "pscan.hpp"

void scan(){
    for (int i =0 ;i < 65535 ; i++)   {
        int sockfd ; 
        struct sockaddr_in tower ; 
        if (inet_pton(AF_INET, "127.0.0.1" , &tower.sin_addr) <= 0) {
            fprintf(stderr,"Problem loading IP address");
            exit(EXIT_FAILURE);
        }

        memset(&tower, 0, sizeof(tower));
        tower.sin_family = AF_INET;
        tower.sin_addr.s_addr = inet_addr("127.0.0.1");
        tower.sin_port = htons(i);

        if (( sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            fprintf(stderr,"Problem creating socket");
            close( sockfd);
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&tower, sizeof(tower)) < 0) {
            open_ports.push_back(i);
        }

        close(sockfd);
        
    }
}


int main(){

    int num_threads = thread::hardware_concurrency();
    printf("Threads Available : %d\n", num_threads);
    scan();
}