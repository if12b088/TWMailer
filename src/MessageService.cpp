/*
 * MessageService.cpp
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#include "MessageService.h"
#include "MessageDao.h"

MessageService::MessageService(MessageDao* dao) {
	this->dao = dao;
}

MessageService::~MessageService() {
	delete this->dao;
}

bool MessageService::sendMsg(std::string from, std::string to,
		std::string subject, std::string msg) {
	return true;
}
std::list<std::string> MessageService::listMsg(std::string username) {
	std::list < std::string > list;
	return list;
}
std::string MessageService::readMsg(std::string username, int msgNr) {
	return "bla";
}
bool MessageService::deleteMsg(std::string username, int msgNr) {
	return true;
}

