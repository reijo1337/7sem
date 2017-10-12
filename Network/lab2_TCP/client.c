#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>

#define BUF_SIZE 10000

int main(int argc, char **argv) {
	int Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket < 0) {
		perror("Failure when making socket descriptor");
		return EXIT_FAILURE;
	}	

	struct sockaddr_in SockAddr;						// Структура для представления адреса
	SockAddr.sin_family = AF_INET;						// Семейство адресов
	SockAddr.sin_port = htons(12345);
	SockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);	// 127.0.0.1

	if (connect(Socket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr)) < 0) {	// Подключение к сокетуй
		perror("Failure when making connect to server");
		return EXIT_FAILURE;
	}

	char msg[BUF_SIZE];
	printf("print message for server:\n");
	
	char c;
	int i = 0;
	while ((c = getchar()) != '\n') 
	{
		msg[i] = c;
		i++;
	}
	msg[i] = '\0';

	send(Socket, msg, sizeof(msg), MSG_NOSIGNAL);	// Требует не посылать сигнал SIGPIPE, если при работе с ориентированным 
													// на поток сокетом другая сторона обрывает соединение. Код ошибки EPIPE 
													// возвращается в любом случае.

	shutdown(Socket, SHUT_RDWR);					// Остановка соединения

	return 0;
}