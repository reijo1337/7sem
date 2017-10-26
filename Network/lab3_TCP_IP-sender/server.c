#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1

#define POOL_SIZE 8
#define BUFFER_SIZE 1024
#define STATUS_PERIOD 3

void server_echo(int client_sfd);

int main(int argc, char* argv[]) {
    struct sockaddr_in bind_addr;
    uint32_t port_flag = FALSE, ip_flag = FALSE, reuse_flag = FALSE;
    uint32_t option, server_sfd, client_sfd;
    
    while((option=getopt(argc, argv, "i:p:r")) != ERROR) {
        switch(option) {
            case 'i':
                ip_flag = inet_pton(AF_INET, optarg, &bind_addr.sin_addr);
                break;
            case 'p':;
                bind_addr.sin_port = htons(atoi(optarg));
                port_flag = TRUE;
                break;
            case 'r':
                reuse_flag = TRUE;
                break;
        }
    }
    
    bind_addr.sin_family = AF_INET;
    
    if(!ip_flag) {
        printf("No -i param. Binding to default INADDR_ANY\n");
        bind_addr.sin_addr.s_addr = INADDR_ANY;
    }
    
    if(!port_flag) {
        printf("No -p param. Binding to default port 31337\n");
        bind_addr.sin_port = htons(31337);
    }
    
    if((server_sfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == ERROR) {
        perror("socket()");
        exit(errno);
    }
    
    if(setsockopt(server_sfd, SOL_SOCKET, SO_REUSEADDR, &reuse_flag, sizeof(reuse_flag)) == ERROR) {
        perror("setsockopt()");
        exit(errno);
    }
    
    if(bind(server_sfd, (struct sockaddr*) &bind_addr, sizeof(bind_addr)) == ERROR) {
        perror("bind()");
        exit(errno);
    }
    
    if(listen(server_sfd, POOL_SIZE) == ERROR) {
        perror("listen()");
        exit(errno);
    }
    
    while(TRUE) {
        struct sockaddr_in remote_addr;
        socklen_t remote_addr_len;
        pid_t echo_pid;
        
        if((client_sfd=accept(server_sfd, (struct sockaddr*) &remote_addr, &remote_addr_len)) == ERROR) {
            perror("accept()");
            exit(errno);
        }
        
        if((echo_pid=fork())) {
            server_echo(client_sfd);
        } else if(echo_pid == ERROR) {
            perror("fork(2)");
            exit(errno);   
        }
    }
    
    wait(NULL);
    close(server_sfd);
    close(client_sfd);
    return (EXIT_SUCCESS);
}

void server_echo(int client_sfd) {
    char* in_buffer = malloc(BUFFER_SIZE);
    char* out_buffer = malloc(BUFFER_SIZE);
    ssize_t bytes;
    
    while(TRUE) {
        bytes = recv(client_sfd, in_buffer, BUFFER_SIZE, 0);
        
        if(bytes == ERROR) {
            perror("recv(echo)");
            break;
        } else if(bytes == 0) {
            printf("Client disconnected!\n");
            break;
        }
        
        strcpy(out_buffer, in_buffer);
        printf("%d> %s", getpid(), in_buffer);
        
        bytes = send(client_sfd, out_buffer, strlen(out_buffer), 0);
        if(bytes == 0) {
            printf("Client disconnected!\n");
            break;
        } else if(bytes == ERROR) {
            perror("send(echo)");
            break;
        }
        
        memset(in_buffer, 0, BUFFER_SIZE);
        memset(out_buffer, 0, BUFFER_SIZE);
    }
    
    close(client_sfd);
}