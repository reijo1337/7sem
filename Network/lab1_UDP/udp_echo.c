#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <getopt.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <string.h>

#define BUFF_SIZE 128

void sigint_handler();

int main(int argc, char* argv[]) {
    int option, sock_fd;
    int f_ip = 0, f_port = 0; 
    
    char* buff = malloc(BUFF_SIZE);
    
    struct sockaddr_in bind_addr, remote_addr;
    socklen_t addr_len = (socklen_t) sizeof(remote_addr);
    
    signal(SIGINT, sigint_handler);
    
    while((option = getopt(argc, argv, "i:p:")) != -1) {
        switch(option) {
            case 'i':
                f_ip = inet_pton(AF_INET, optarg, &bind_addr.sin_addr);
                break;
                
            case 'p':
                bind_addr.sin_port = htons(atoi(optarg));
                f_port = 1;
                break;
        }
    }
    
    if(!f_port) {
        printf("No -p param. Using default port 31337\n");
        bind_addr.sin_port = htons(31337);
    }
    
    if(!f_ip) {
        printf("No or wrong value for -i param. Using localhost: INADDR_ANY\n");
        bind_addr.sin_addr.s_addr = INADDR_ANY;
    }
    
    bind_addr.sin_family = AF_INET;
    printf("BIND %s:%d\n", inet_ntoa(bind_addr.sin_addr), ntohs(bind_addr.sin_port));
    
    if((sock_fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket()");
        exit(errno);
    }
    
    if(bind(sock_fd, (struct sockaddr*) &bind_addr,sizeof(struct sockaddr_in)) < 0) {
        perror("bind()");
        exit(errno);
    }
            
    while(1) {
        memset(buff, 0, BUFF_SIZE);
        
        if(recvfrom(sock_fd, buff, BUFF_SIZE, 0, (struct sockaddr*) &remote_addr, &addr_len) < 0) {
            perror("recvfrom()");
            exit(errno);
        }
        
        if(sendto(sock_fd, buff, strlen(buff), 0, (struct sockaddr*) &remote_addr, addr_len) < 0) {
            perror("sendto()");
            exit(errno);
        }
        
        printf("recvd> %s", buff);
    }
    
    return (EXIT_SUCCESS);
}

void sigint_handler() {
    printf("ai bol'no v noge\n");
    exit(EXIT_SUCCESS);
}