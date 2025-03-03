#include "pscan.hpp"

void help(){
    printf("Please specify a flag : \n"
        "   '-s' - Scan system ports \n"
        "   '-c' - Scan user ports \n"
        "   '-p' - Scan private ports \n"
        "   '-a' - Scan all ports \n"
        "   '-h' - print this help message  \n");
}

inline void print_ports(){
    for (int i : open_ports){
        printf("%s%d%s\n","\x1b[1m",i,"\x1b[0m");
    }
}

void count_open_ports(int start , int end ){
    int sockfd ; 

    struct sockaddr_in tower ; 
    
    if (inet_pton(AF_INET, "127.0.0.1" , &tower.sin_addr) <= 0) {
        fprintf(stderr,"Problem loading IP address");
        exit(EXIT_FAILURE);
    }

    memset(&tower, 0, sizeof(tower));
    tower.sin_family = AF_INET;
    tower.sin_addr.s_addr = inet_addr("127.0.0.1");

    for (int i = start ;i < end ; i++)   {
        
        tower.sin_port = htons(i);

        if (( sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            fprintf(stderr,"Problem creating socket for port %d \n" , i);
            close( sockfd);
            i--;
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&tower, sizeof(tower)) < 0) {
            lock_guard<mutex> guard(vec_mtx);
            open_ports.push_back(i);
        }

        close(sockfd);
        
    }
}


void thread_handler(int start , int end){
    int max_threads = std::thread::hardware_concurrency();
    thread thread_list[max_threads];
    int interval_sz = (end -start + 1)/max_threads;
    int thread_num;


    for(thread_num = 0 ; thread_num < max_threads ; thread_num++){
        int right_bound = start + interval_sz;
        thread_list[thread_num] = thread(count_open_ports , start , right_bound);
        start  = right_bound + 1;
    }

    for (thread_num = 0 ; thread_num < max_threads ; thread_num++){
        thread_list[thread_num].join();
    }

    sort(open_ports.begin() , open_ports.end());
    print_ports();

}

int main(int argc , char* argv[]){
    if (argc == 1 or (strcmp(argv[1], "-") == 0)){
        help();
        return 1;
    }

    int c , start,end ; 
    while ((c = getopt(argc , argv , "supah")) != -1 ){
        switch(c){
            case 's':
                start = 0 ;
                end = 1023;
                thread_handler(start , end);
                break;
            case 'u':
                start = 1024;
                end = 49151;
                thread_handler(start , end);
                break;
            case 'p':
                start = 49152;
                end = 65535;
                thread_handler(start , end);
                break;
            case 'a':
                start = 0;
                end = 65535;
                thread_handler(start , end);
                break;
            case 'h':
                help();
                return 1;
        }
    }
}