/*
 * MessageDao.cpp
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#include "MessageDao.h"
#include <string>
#include <list>

MessageDao::MessageDao(std::string dirPath) {
	path = dirPath;
}

MessageDao::~MessageDao() {
	// TODO Auto-generated destructor stub
}

void MessageDao::saveMessage(Message* msg) {

}

std::list<Message> MessageDao::loadMessages(std::string username) {
	std::list<Message> list;
	return list;
}

Message* MessageDao::readMessage(std::string username, int msgNr) {
	Message* msg = new Message();
	return msg;
}

bool MessageDao::delMessage(std::string username, int msgNr) {
	return false;
}

