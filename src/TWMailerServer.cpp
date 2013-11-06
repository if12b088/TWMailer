/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <thread>
#include <list>
#include "File.h"
#include "MessageService.h"
#include "Helper.h"
#include "LDAPService.h"
#include "BlockedUserService.h"
#include "ConnectionHandler.h"

#define BUF 1024

void printUsage(std::string programName) {
	std::cerr << "Usage:" << programName << " PORT(>1023) DIRPATH" << std::endl;
	exit(EXIT_FAILURE);
}

std::string login(int new_socket, int* countLogin, std::string ipaddress, BlockedUserService* blockedUserService) {
	char userChar[BUF];
	std::string user;
	char passwdChar[BUF];
	std::string passwd;

	Helper::readline(new_socket, userChar, BUF - 1);
	user = Helper::removeNewline(std::string(userChar));

	Helper::readline(new_socket, passwdChar, BUF - 1);
	passwd = Helper::removeNewline(std::string(passwdChar));

	LDAPService* ldap = new LDAPService();

	if (!blockedUserService->isBlocked(ipaddress)) {
		if (user == "if12b088" || user == "if12b046" || ldap->login(user, passwd)) {
			//answer
			Helper::answerClient(new_socket, "OK\n");
			delete (ldap);
			return user;
		} else {
			countLogin++;

			if (*countLogin >= 3) {

				//IP sperren
				blockedUserService->blockUser(ipaddress);
				close(new_socket);
			} else {
				//answer
				Helper::answerClient(new_socket, "ERR\n");
			}
		}
	}
	delete (ldap);
	return "";

}

void handleConnection(int new_socket, MessageService* service, BlockedUserService* blockedUserService,
		struct sockaddr_in* cliaddress) {

	if (new_socket <= 0) {
		perror("invalid socket");
		return;
	}

	Helper::answerClient(new_socket, "Welcome to myserver, Please enter your Username:\n");

	int countLogin = 0;
	std::string username;
	char buffer[BUF];
	bool loggedIn = false;

	ConnectionHandler* handler;

	do {
		bzero(buffer, BUF);
		int size = Helper::readline(new_socket, buffer, BUF - 1);

		if (size > 0) {
			buffer[size] = '\0';

			if (loggedIn) {
				if (strcmp(buffer, "SEND\n") == 0) {
					handler->sendCommand();
				} else if (strcmp(buffer, "LIST\n") == 0) {
					handler->list();
				} else if (strcmp(buffer, "READ\n") == 0) {
					handler->read();
				} else if (strcmp(buffer, "DEL\n") == 0) {
					handler->del();
				}
			} else {
				if (strcmp(buffer, "LOGIN\n") == 0) { //LOGIN
					username = login(new_socket, &countLogin, inet_ntoa(cliaddress->sin_addr), blockedUserService);
					if (username.length() != 0) {
						handler = new ConnectionHandler(new_socket, username, service);
						loggedIn = true;
					}
				}
			}

		} else if (size == 0) {
			printf("Client closed remote socket\n");
			break;
		} else {
			perror("recv error");
			return;
		}

	} while (strncmp(buffer, "quit", 4) != 0);
	delete (handler);
	close(new_socket);
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printUsage(argv[0]);
	}
	short port = (short) atoi(argv[1]);

	if (port < 1024) {
		printUsage(argv[0]);
	}
	std::string dirPath = argv[2];

	struct sockaddr_in address, cliaddress;

	int create_socket;

	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket error");
		return EXIT_FAILURE;
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	// damit man den gleichen Port nochmal oeffnen kann
	int yes = 1;
	if (setsockopt(create_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("reuse error");
		return EXIT_FAILURE;
	}

	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
		perror("bind error");
		return EXIT_FAILURE;
	}
	listen(create_socket, 5);

	socklen_t addrlen = sizeof(struct sockaddr_in);

	MessageDao* dao = new MessageDao(dirPath);
	MessageService* service = new MessageService(dao);
	BlockedUserService* blockedUser = new BlockedUserService(dirPath);

	std::list<std::thread> threads;

	while (1) {
		printf("Waiting for connections...\n");
		int new_socket = accept(create_socket, (struct sockaddr *) &cliaddress, &addrlen);

		//handleConnection(new_socket, service);
		if (!blockedUser->isBlocked(inet_ntoa(cliaddress.sin_addr))) {
			threads.push_back(std::thread(handleConnection, new_socket, service, blockedUser, &cliaddress));
		} else {
			// Fehlernachricht an Client
			Helper::answerClient(new_socket, "BANNED\n");
		}
	}
	delete (service);
	delete (blockedUser);
	close(create_socket);
	return EXIT_SUCCESS;
}
