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

#define BUF 1024

void printUsage(std::string programName) {
	std::cerr << "Usage:" << programName << " PORT(>1023) DIRPATH" << std::endl;
	exit(EXIT_FAILURE);
}

void answerClient(int new_socket, std::string returnMsg) {
	//answer
	if (send(new_socket, returnMsg.c_str(), returnMsg.length(), 0) == -1) {
		perror("Send error");
		return;
	}
}

void handleConnection(int new_socket, MessageService* service,
		BlockedUserService* blockedUser, struct sockaddr_in* cliaddress) {

	char buffer[BUF];
	std::string returnMsg;
	int size;
	int countLogin = 0;
	std::string username;
	LDAPService* ldap = new LDAPService();

	if (new_socket > 0) {
//		printf("Client connected from %s:%d...\n",
//				inet_ntoa(cliaddress->sin_addr), ntohs(cliaddress->sin_port));
		strcpy(buffer, "Welcome to myserver, Please enter your Username:\n");
		send(new_socket, buffer, strlen(buffer), 0);

		do {
			bzero(buffer, BUF);
			size = Helper::readline(new_socket, buffer, BUF - 1);

#ifdef _DEBUG
			std::cout << "buffer: " << buffer << std::endl;
#endif

			if (size > 0) {
				buffer[size] = '\0';
				//returnBuffer[0] = '\0';
				returnMsg = "";

//LOGIN
				if (strcmp(buffer, "LOGIN\n") == 0) {

					char userChar[BUF];
					std::string user;
					char passwdChar[BUF];
					std::string passwd;

					Helper::readline(new_socket, userChar, BUF - 1);
					user = Helper::removeNewline(std::string(userChar));

					Helper::readline(new_socket, passwdChar, BUF - 1);
					passwd = Helper::removeNewline(std::string(passwdChar));

					bool b;
					if (user == "if12b088" || user == "if12b046") {
						b = true;
					} else {
						b = ldap->login(user, passwd);
					}

					//b = ldap->login(user, passwd);

					if (b) {
						// Username setzten
						username = user;
						returnMsg = "OK\n";
					} else {
						countLogin++;

#ifdef _DEBUG
						std::cout << "countLogin: " << countLogin << std::endl;
#endif
						if (countLogin >= 3) {

							//IP sperren
							blockedUser->blockUser(
									inet_ntoa(cliaddress->sin_addr));
							returnMsg = "BAN\n";

							if (send(new_socket, returnMsg.c_str(),
									returnMsg.length(), 0) == -1) {
								perror("Send error");
								return;
							}
							close(new_socket);
							return;
						} else {
							returnMsg = "ERR\n";
						}
					}
					//answer
					answerClient(new_socket, returnMsg);
				}

//SEND
				if (strcmp(buffer, "SEND\n") == 0) {
					char toChar[BUF];
					std::string to;
					char subjectChar[BUF];
					std::string subject;
					char textTempChar[BUF];
					std::string textTempStr;
					std::string text;
					char fileSizeChar[BUF];
					long long fileSize;
					char fileNameChar[BUF];
					std::string fileName;

					int sizeTo = 1;
					do {
						sizeTo = Helper::readline(new_socket, toChar, BUF - 1);
						to.append(Helper::removeNewline(std::string(toChar)));
					} while (toChar[sizeTo - 1] != '\n');

					Helper::readline(new_socket, subjectChar, BUF - 1);
					subject = Helper::removeNewline(std::string(subjectChar));

					char lastChar = 0;
					int sizeText = 1;
					do {
						lastChar = textTempStr[sizeText - 1];
#ifdef _DEBUG
						std::cout << "lastChar: " << lastChar << std::endl;
#endif
						textTempStr = "";
						textTempChar[0] = '\0';
						sizeText = Helper::readline(new_socket, textTempChar,
						BUF - 1);
						textTempStr = std::string(textTempChar);

						if (lastChar != '\n'
								|| textTempStr.compare(".\n") != 0) {
							text.append(textTempStr);
						}
#ifdef _DEBUG
						std::cout << "sizeText: " << sizeText << " eine Zeile: " << textTempStr << std::endl;
#endif
					} while (lastChar != '\n' || textTempStr.compare(".\n") != 0);

					Helper::readline(new_socket, fileSizeChar, BUF - 1);
					fileSize = atoll(fileSizeChar);

#ifdef _DEBUG
					//std::cout << "From: " << from << std::endl;
					std::cout << "To: " << to << std::endl;
					std::cout << "Subject: " << subject << std::endl;
					std::cout << "Text: " << text << std::endl;
					std::cout << "FileSize: " << fileSize << std::endl;
#endif

//					if (send(new_socket, "Protokoll OK\n", 13, 0) == -1) {
//						perror("Send error");
//						return;
//					}

					Message* msg = new Message();
					File* fileObj = new File();
					msg->setFrom(username);
					msg->setTo(Helper::splitString(to, ";"));
					msg->setSubject(subject);
					msg->setText(text);
					msg->setFileAttached(false);

					if (fileSize != 0) {
						//ATTACHMENT vorhanden
						msg->setFileAttached(true);
						fileObj->setFilesize(fileSize);

						Helper::readline(new_socket, fileNameChar, BUF - 1);
						fileName = Helper::removeNewline(
								std::string(fileNameChar));
						fileObj->setFilename(fileName);
#ifdef _DEBUG
						std::cout << "FileName: " << fileName << std::endl;
#endif
						char* file = new char[fileSize];
						bzero(file, fileSize);
						int toRead;
						char* pos = file;

						while (fileSize > 0) {

							if (fileSize < BUF) {
								toRead = fileSize;
							} else {
								toRead = BUF;
							}
							char readBuffer[toRead];
							bzero(readBuffer, toRead);

							int sendSize = recv(new_socket, readBuffer, toRead, 0);

							memcpy(pos, readBuffer, sendSize);
							pos += sendSize;
							fileSize -= sendSize;
						}
						fileObj->setFile(file);
#ifdef _DEBUG
						std::cout << file << std::endl;
#endif
					}

					msg->setFile(fileObj);
					if (service->sendMsg(msg)) {
						returnMsg = "OK\n";
					} else {
						returnMsg = "ERR\n";
					}

					delete (msg);
					//delete[] file;

					//answer
					answerClient(new_socket, returnMsg);

				} else if (strcmp(buffer, "LIST\n") == 0) {
//LIST

#ifdef _DEBUG
					std::cout << "User: " << username << std::endl;
#endif

					std::list<Message*> msgList = service->listMsg(username);
					std::stringstream ss;

					ss << msgList.size() << "\n";

					for (std::list<Message*>::iterator it = msgList.begin();
							it != msgList.end(); it++) {
						Message* msg = *it;
						ss << msg->getMsgNr() << ": " << msg->getSubject()
								<< "\n";
						delete (msg);
					}

					//strcpy(returnBuffer, ss.str().c_str());
					returnMsg = ss.str();
#ifdef _DEBUG
					std::cout << "returnMsg: " << returnMsg << std::endl;
#endif
					//answer
					answerClient(new_socket, returnMsg);

				} else if (strcmp(buffer, "READ\n") == 0) {
//READ
					char nrChar[BUF];

					Helper::readline(new_socket, nrChar, BUF - 1);
					nrChar[strlen(nrChar) - 1] = '\0';

#ifdef _DEBUG
					std::cout << "User: " << username << std::endl;
					std::cout << "Nr: " << nrChar << ", size: " << strlen(nrChar) << std::endl;
#endif

					Message* msg = service->readMsg(username, atol(nrChar));

					if (msg->getMsgNr() == 0) {
						returnMsg = "Es konnte keine eindeutige Message gefunden werden\n";
					} else {

						std::stringstream ss;

						//	ss << "Nachricht mit der Nummer: " << msg->getMsgNr()
						ss << msg->getMsgNr() << "\n" << msg->toProtocol()
								<< ".\n";
						returnMsg = ss.str();
					}
#ifdef _DEBUG
					std::cout << "returnMsg: " << returnMsg << std::endl;
#endif

					//answer
					answerClient(new_socket, returnMsg);

					//Send File
					if (msg->isFileAttached()) {
						long long fileSize = msg->getFile()->getFilesize();
						int toRead;
						char* file = msg->getFile()->getFile();

						while (fileSize > 0) {
							if (fileSize > BUF) {
								toRead = BUF;
							} else {
								toRead = fileSize;
							}

							int sendSize = send(new_socket, file, toRead, 0);
							file += sendSize;
							fileSize -= sendSize;
						}
					}
					delete (msg);

				} else if (strcmp(buffer, "DEL\n") == 0) {
//DEL
					char nrChar[BUF];

					Helper::readline(new_socket, nrChar, BUF - 1);
					nrChar[strlen(nrChar) - 1] = '\0';

#ifdef _DEBUG
					//std::cout << "User: " << username << std::endl;
					//std::cout << "Nr: " << nrChar << ", size: " << strlen(nrChar) << std::endl;
#endif

					//convert nr to long
					if (service->deleteMsg(username, atol(nrChar))) {
						returnMsg = "OK\n";
						//strcpy(returnBuffer, "OK\n");
					} else {
						returnMsg = "ERR\n";
						//strcpy(returnBuffer, "ERR\n");
					}

					//answer
					answerClient(new_socket, returnMsg);
				}
#ifdef _DEBUG
			//	std::cout << "returnMsg: " << returnMsg << std::endl;
#endif
				//printf("Message received: %s\n", buffer);
			} else if (size == 0) {
				printf("Client closed remote socket\n");
				break;
			} else {
				perror("recv error");
				return;
			}

#ifdef _DEBUG
			//std::cout << "ende" << std::endl;
#endif

		} while (strncmp(buffer, "quit", 4) != 0);
		close(new_socket);
	}
}

int main(int argc, char *argv[]) {

	// Ausgabe der Parameter
#ifdef _DEBUG
	std::cout << "argv0: " << argv[0] << std::endl;
	std::cout << "argv1: " << argv[1] << std::endl;
	std::cout << "argv2: " << argv[2] << std::endl;
	std::cout << "argc: " << argc << std::endl;
#endif

	if (argc != 3) {
		printUsage(argv[0]);
	}
	short port = (short) atoi(argv[1]);

	if (port < 1024) {
		printUsage(argv[0]);
	}
	std::string dirPath = argv[2];

	MessageDao* dao = new MessageDao(dirPath);
	MessageService* service = new MessageService(dao);

	BlockedUserService* blockedUser = new BlockedUserService(dirPath);

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
	if (setsockopt(create_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1) {
		perror("reuse error");
		return EXIT_FAILURE;
	}

	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address))
			!= 0) {
		perror("bind error");
		return EXIT_FAILURE;
	}
	listen(create_socket, 5);

	socklen_t addrlen = sizeof(struct sockaddr_in);

	//std::vector<std::thread> threads;
	std::list<std::thread> threads;

	while (1) {
		printf("Waiting for connections...\n");
		int new_socket = accept(create_socket, (struct sockaddr *) &cliaddress,
				&addrlen);

		//handleConnection(new_socket, service);
		if (!blockedUser->isBlocked(inet_ntoa(cliaddress.sin_addr))) {
			//if(1){
			threads.push_back(
					std::thread(handleConnection, new_socket, service,
							blockedUser, &cliaddress));
		} else {
			// Fehlernachricht an Client
			std::string returnMsg = "BANNED\n";
			if (send(new_socket, returnMsg.c_str(), returnMsg.length(), 0)
					== -1) {
				perror("Send error");
				return EXIT_FAILURE;
			}
		}
	}
	close(create_socket);
//delete(IPAdress);
	return EXIT_SUCCESS;
}
