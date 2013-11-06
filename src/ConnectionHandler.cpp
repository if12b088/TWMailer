/*
 * ConnectionHelper.cpp
 *
 *  Created on: 06.11.2013
 *      Author: matthias
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include "ConnectionHandler.h"
#include "MessageService.h"
#include "Helper.h"

#define BUF 1024

ConnectionHandler::ConnectionHandler(int socket, std::string username, MessageService* msgService) {
	this->socket = socket;
	this->username = username;
	this->msgService = msgService;
}
ConnectionHandler::~ConnectionHandler() {

}
void ConnectionHandler::sendCommand() {
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
		sizeTo = Helper::readline(socket, toChar, BUF - 1);
		to.append(Helper::removeNewline(std::string(toChar)));
	} while (toChar[sizeTo - 1] != '\n');

	Helper::readline(socket, subjectChar, BUF - 1);
	subject = Helper::removeNewline(std::string(subjectChar));

	char lastChar = 0;
	int sizeText = 1;
	do {
		lastChar = textTempStr[sizeText - 1];

		textTempStr = "";
		textTempChar[0] = '\0';
		sizeText = Helper::readline(socket, textTempChar, BUF - 1);
		textTempStr = std::string(textTempChar);

		if (lastChar != '\n' || textTempStr.compare(".\n") != 0) {
			text.append(textTempStr);
		}

	} while (lastChar != '\n' || textTempStr.compare(".\n") != 0);

	Helper::readline(socket, fileSizeChar, BUF - 1);
	fileSize = atoll(fileSizeChar);

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

		Helper::readline(socket, fileNameChar, BUF - 1);
		fileName = Helper::removeNewline(std::string(fileNameChar));
		fileObj->setFilename(fileName);

		char* file = new char[fileSize];
		bzero(file, fileSize);
		int toRead;
		char* pos = file;
		int readSize;
		std::string blockOK;

		while (fileSize > 0) {

			if (fileSize < BUF) {
				toRead = fileSize;
			} else {
				toRead = BUF;
			}
			char readBuffer[toRead];
			bzero(readBuffer, toRead);

			do {
				readSize = recv(socket, readBuffer, toRead, 0);

				if (readSize == toRead) {
					blockOK = "Y";
				} else {
					blockOK = "N";
					std::cout << blockOK << std::endl;
				}
				send(socket, blockOK.c_str(), 2, 0);
			} while (readSize != toRead);

			std::cout << readSize << std::endl;
			memcpy(pos, readBuffer, readSize);
			pos += readSize;
			fileSize -= readSize;
		}
		fileObj->setFile(file);
	}

	msg->setFile(fileObj);
	if (msgService->sendMsg(msg)) {
		Helper::answerClient(socket, "OK\n");
	} else {
		Helper::answerClient(socket, "ERR\n");
	}

	delete (msg);
}

void ConnectionHandler::list() {
	std::list<Message*> msgList = msgService->listMsg(username);

	std::stringstream ss;

	ss << msgList.size() << "\n";

	for (std::list<Message*>::iterator it = msgList.begin(); it != msgList.end(); it++) {
		Message* msg = *it;
		ss << msg->getMsgNr() << ": " << msg->getSubject() << "\n";
		delete (msg);
	}
	Helper::answerClient(socket, ss.str());

}

void ConnectionHandler::read() {
	char nrChar[BUF];

	Helper::readline(socket, nrChar, BUF - 1);
	nrChar[strlen(nrChar) - 1] = '\0';

	Message* msg = msgService->readMsg(username, atol(nrChar));

	std::stringstream ss;

	if (msg->getMsgNr() == 0) {
		Helper::answerClient(socket, "0\nEs konnte keine eindeutige Message gefunden werden\n");
	} else {
		ss << msg->getMsgNr() << "\n" << msg->toProtocol();
		Helper::answerClient(socket, ss.str());
	}

	int sendSize;
	char blockOK[2];
	//Send File
	if (msg->isFileAttached()) {
		long long fileSize = msg->getFile()->getFilesize();
		int toSend;
		char* file = msg->getFile()->getFile();

		while (fileSize > 0) {
			if (fileSize > BUF) {
				toSend = BUF;
			} else {
				toSend = fileSize;
			}

			do {
				sendSize = send(socket, file, toSend, 0);
				recv(socket, blockOK, 2, 0);
				if (strcmp(blockOK, "Y") != 0) {
					printf("blockOK: %s\n", blockOK);
				}
			} while (strcmp(blockOK, "Y") != 0);

			file += sendSize;
			fileSize -= sendSize;
		}
	}
	delete (msg);
}

void ConnectionHandler::del() {
	char nrChar[BUF];

	Helper::readline(socket, nrChar, BUF - 1);
	nrChar[strlen(nrChar) - 1] = '\0';

	//convert nr to long
	if (msgService->deleteMsg(username, atol(nrChar))) {
		Helper::answerClient(socket, "OK\n");
	} else {
		Helper::answerClient(socket, "ERR\n");
	}
}

