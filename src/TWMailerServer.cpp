/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
//#include <thread>
//#include "ServerBusiness.h"
#include "ServerBusiness.cpp"
#define BUF 1024

void printUsage(std::string programName) {
	std::cerr << "Usage:" << programName << "PORT(>1023) DIRPATH" << std::endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printUsage(argv[0]);
	}
	short port = (short) std::atoi(argv[1]);

	if (port < 1024) {
		printUsage(argv[0]);
	}
	std::string dirPath = argv[2];


	struct sockaddr_in address, cliaddress;

	int create_socket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons (port);

	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address))
			!= 0) {
		perror("bind error");
		return EXIT_FAILURE;
	}
	listen(create_socket, 5);

	socklen_t addrlen = sizeof(struct sockaddr_in);

	char buffer[BUF];
	int size;
	//std::vector<std::thread> threads;

	while (1) {
		printf("Waiting for connections...\n");
		int new_socket = accept(create_socket, (struct sockaddr *) &cliaddress,
				&addrlen);
		if (new_socket > 0) {
			printf("Client connected from %s:%d...\n",
					inet_ntoa(cliaddress.sin_addr),
					ntohs(cliaddress.sin_port) );
			strcpy(buffer, "Welcome to myserver, Please enter your command:\n");
			send(new_socket, buffer, strlen(buffer), 0);
		}
		do {
			size = recv(new_socket, buffer, BUF - 1, 0);
			if (size > 0) {
				buffer[size] = '\0';
				printf("Message received: %s\n", buffer);
			} else if (size == 0) {
				printf("Client closed remote socket\n");
				break;
			} else {
				perror("recv error");
				return EXIT_FAILURE;
			}
		} while (strncmp(buffer, "quit", 4) != 0);
		close(new_socket);
	}
	close(create_socket);
	return EXIT_SUCCESS;
}
