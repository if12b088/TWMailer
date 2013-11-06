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

bool MessageService::sendMsg(Message* msg) {
	return this->dao->saveMessage(msg);
}
std::list<Message*> MessageService::listMsg(std::string username) {
	return this->dao->loadMessages(username);
}
Message* MessageService::readMsg(std::string username, long long msgNr) {
	return this->dao->readMessage(username, msgNr);
}
bool MessageService::deleteMsg(std::string username, long long msgNr) {
	return this->dao->delMessage(username, msgNr);
}

