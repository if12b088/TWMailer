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
	virtual void saveMessage(Message* msg);
	virtual std::list<Message> loadMessages(std::string username);
	virtual Message* readMessage(std::string username, int msgNr);
	virtual bool delMessage(std::string username, int msgNr);
private:
	std::string path;
};

#endif /* MessageDao_H_ */
