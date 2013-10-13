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
	virtual bool sendMsg(std::string from, std::string to, std::string subject, std::string msg);
	virtual std::list<std::string> listMsg(std::string username);
	virtual std::string readMsg(std::string username, int msgNr);
	virtual bool deleteMsg(std::string username, int msgNr);
private:
	MessageDao* dao;
};

#endif /* MessageService_H_ */
