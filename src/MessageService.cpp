/*
 * MessageService.cpp
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#include "MessageService.h"
#include "MessageDao.h"
#include <iostream>

MessageService::MessageService(MessageDao* dao) {
	this->dao = dao;
}

MessageService::~MessageService() {
	delete this->dao;
}

bool MessageService::sendMsg(std::string from, std::string to,
		std::string subject, std::string text) {
	std::list<std::string> toList;

	Message msg;
	msg.setFrom(from);
	std::string delimiter = ";";
	size_t pos = 0;
	std::string token;
	while ((pos = to.find(delimiter)) != std::string::npos) {
		token = to.substr(0, pos);
		toList.push_back(token);
		to.erase(0, pos + delimiter.length());
	}
	if (to.size() != 0) {
		toList.push_back(to);
	}
	msg.setTo(toList);
	msg.setSubject(subject);
	msg.setText(text);
	std::cout << msg.toString() << std::endl;
	return this->dao->saveMessage(msg);
}
std::list<Message> MessageService::listMsg(std::string username) {
	return this->dao->loadMessages(username);
}
Message MessageService::readMsg(std::string username, long long msgNr) {
	return this->dao->readMessage(username, msgNr);
}
bool MessageService::deleteMsg(std::string username, long long msgNr) {
	return this->dao->delMessage(username, msgNr);
}

