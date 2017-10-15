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

void socket_write();
void socket_read();
void sigint_handler();
 
struct sockaddr_in bind_addr;
int sock_fd;

int main(int argc, char* argv[]) {
    int option;
    int f_ip = 0, f_port = 0;
    
    signal(SIGINT, sigint_handler);
    
    while((option = getopt(argc, argv, "p:i:")) != -1) {
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
        printf("No -p param. Using default value: 31337\n");
        bind_addr.sin_port = htons(31337);
    }
    
    if(!f_ip) {
        printf("No or wrong value for -i param. Using localhost: 127.0.0.1\n");
        inet_pton(AF_INET, "127.0.0.1", &bind_addr.sin_addr);
    }
    
    bind_addr.sin_family = AF_INET;
    printf("TARGET %s:%d\n", inet_ntoa(bind_addr.sin_addr), bind_addr.sin_port);
    
    if((sock_fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket()");
        exit(errno);
    }
    
    switch(fork()) {
        case -1:
            perror("fork()");
            exit(EXIT_FAILURE);
        case 0:
            socket_write();
            break;
        default:
            socket_read();
            break;
    }
    
    return (EXIT_SUCCESS);
}

void socket_write() {
    char* buff = malloc(BUFF_SIZE);
    
    while(fgets(buff, BUFF_SIZE, stdin)) {
        if(sendto(sock_fd, buff, strlen(buff), 0, (struct sockaddr*)&bind_addr, sizeof(struct sockaddr)) < 0) {
            perror("sendto()");
        }
        memset(buff, 0, BUFF_SIZE);
    } 
}

void socket_read() {
    char* buff = malloc(BUFF_SIZE);
    
    while(1) {
        memset(buff, 0, BUFF_SIZE);
        if(recv(sock_fd, buff, BUFF_SIZE, 0) < 0) {
            perror("recv()");
            exit(errno);
        }
        
        printf("echo> %s", buff);
    }
}

void sigint_handler() {
    printf("v solyanogo niyaaaaaaa\n");
    exit(EXIT_SUCCESS);
}