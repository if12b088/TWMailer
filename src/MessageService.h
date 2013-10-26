/*
 * MessageService.h
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#ifndef MessageService_H_
#define MessageService_H_

#include <string>
#include <list>
#include "MessageDao.h"

class MessageService {
public:
	MessageService(MessageDao* dao);
	virtual ~MessageService();
	virtual bool sendMsg(std::string from, std::list<std::string> to, std::string subject,
			std::string msg);
	virtual std::list<Message> listMsg(std::string username);
	virtual Message readMsg(std::string username, long long msgNr);
	virtual bool deleteMsg(std::string username, long long msgNr);
private:
	MessageDao* dao;
};

#endif /* MessageService_H_ */
