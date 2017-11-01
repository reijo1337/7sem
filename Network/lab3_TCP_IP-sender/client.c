#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define BUFF_SIZE 128

struct sockaddr_in dest_addr;
int sock_fd;

void socket_write();
void socket_read();
void sigint_handler();

int main(int argc, char* argv[]) {
    uint32_t option;
    uint8_t f_ip = FALSE, f_port = FALSE;
    
    signal(SIGINT, sigint_handler);
    
    while((option=getopt(argc, argv, "i:p:")) != ERROR) {
        switch(option) {
            case 'i': 
                f_ip = inet_pton(AF_INET, optarg, &dest_addr.sin_addr);
                break;
            case 'p':
                dest_addr.sin_port = htons(atoi(optarg));
                f_port = TRUE;
                break;
        }
    }
    
    dest_addr.sin_family = AF_INET;
    
    if(!f_ip) {
        printf("No -i param. Using ip 127.0.0.1\n");
        inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr);
    }
    
    if(!f_port) {
        printf("No -p param. Using port 8080\n");
        dest_addr.sin_port = htons(8080);
    }
    
    if((sock_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == ERROR) {
        perror("socket()");
        exit(errno);
    }

    if(connect(sock_fd, (struct sockaddr*) &dest_addr, sizeof(dest_addr)) == ERROR) {
        perror("connect()");
        exit(errno);
    }
    
    switch(fork()) {
        case ERROR:
            perror("fork()");
            exit(errno);
        case 0:
            socket_write();
            break;
        default:
            socket_read();
            break;
    }
    
    close(sock_fd);
    wait(NULL);
    return (EXIT_SUCCESS);
}

void socket_write() {
    char* out_buffer = malloc(BUFF_SIZE);
    ssize_t bytes;
    
    while(fgets(out_buffer, BUFF_SIZE, stdin) != NULL) {
        bytes = send(sock_fd, out_buffer, strlen(out_buffer), 0);
        
        if(bytes == ERROR) {
            perror("send()");
            break;
        } else if(bytes == 0) {
            printf("Server connection lost!\n");
            break;
        }
        
        memset(out_buffer, 0, BUFF_SIZE);
    }
}

void socket_read() {
    char* in_buffer = malloc(BUFF_SIZE);
    ssize_t bytes;
    
    while(TRUE) {
        bytes = recv(sock_fd, in_buffer, BUFF_SIZE, 0);
        
        if(bytes == ERROR) {
            perror("recv()");
            break;
        } else if(bytes == 0) {
            printf("Server connection lost!\n");
            break;
        }
        
        printf("%s", in_buffer);
        memset(in_buffer, 0, BUFF_SIZE);
    }
}

void sigint_handler() {
    close(sock_fd);
    exit(EXIT_SUCCESS);
}