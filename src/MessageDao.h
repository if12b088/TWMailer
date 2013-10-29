/*
 * MessageDao.h
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#ifndef MessageDao_H_
#define MessageDao_H_

#include <string>
#include <list>
#include "Message.h"

class MessageDao {
public:
	MessageDao(std::string dirPath);
	virtual ~MessageDao();
	virtual bool saveMessage(Message* msg);
	virtual std::list<Message*> loadMessages(std::string username);
	virtual Message* readMessage(std::string username, long long msgNr);
	virtual bool delMessage(std::string username, long long msgNr);
private:
	std::string dirPath;
};

#endif /* MessageDao_H_ */
