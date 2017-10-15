#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 1024

// Установка сокета в неблок.
int set_nonblock(int fd) {
	int flags;
#if defined(O_NONBLOCK)
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif 
}

int main(int argc, char **argv) {
	int MasterSocket = socket(
		AF_INET,		// IPv4
		SOCK_STREAM,	// TCP
		0				// IPPROTO_TCP
		);
	if (MasterSocket < 0) {
		perror("Failure when making socket descriptor");
		return EXIT_FAILURE;
	}

	std::set<int> SlaveSockets;

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(12345);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// 0.0.0.0
	if (bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr)) < 0) {
		perror("Failure when assigning address to a socket");
    	return EXIT_FAILURE;
	}

	set_nonblock(MasterSocket);

	listen(MasterSocket, SOMAXCONN);

	printf("Server started\n\n");

	while(true) {
		fd_set Set;		// 1024 битов {0, 1}. Если добавляется фд с номером 4, то 4ый бит устанавливается в 1.
		FD_ZERO(&Set);	// обнуление
		FD_SET(MasterSocket, &Set);		// добавляем мастер сокет для принятия соединения
		for (auto Iter = SlaveSockets.begin(); Iter != SlaveSockets.end(); Iter++) {
			FD_SET(*Iter, &Set);
		}

		// Максимальный дескриптор
		int Max = std::max(MasterSocket, *std::max_element(SlaveSockets.begin(), SlaveSockets.end()));
		select(Max + 1, &Set, NULL, NULL, NULL);

		for(auto Iter = SlaveSockets.begin(); Iter != SlaveSockets.end(); Iter++) {
			if(FD_ISSET(*Iter, &Set)) {			// Сокет в множестве => сработал
				static char Buffer[BUF_SIZE];

				int RecvSize = recv(*Iter, Buffer, BUF_SIZE, MSG_NOSIGNAL);
				if ((RecvSize == 0) && (errno != EAGAIN)) {
					shutdown(*Iter, SHUT_RDWR);
					close(*Iter);
					SlaveSockets.erase(Iter);
				} else {
					printf("User%d: %s\n", *Iter, Buffer);
				}
			}
		}

		if(FD_ISSET(MasterSocket, &Set)) {
			int i;
			struct sockaddr_in address;
			unsigned int addrlen = sizeof(address);

			int SlaveSocket = accept(MasterSocket, (struct sockaddr*)&address, &addrlen);

			printf("New connection. socket fd = %d, ip = %s:%d\n", SlaveSocket, 
				inet_ntoa(address.sin_addr), ntohs(address.sin_port));
			if (MasterSocket < 0) {
				perror("Failure when making socket descriptor");
				return EXIT_FAILURE;
			}

			set_nonblock(SlaveSocket);
			SlaveSockets.insert(SlaveSocket);
		}
	}

	return 0;
}