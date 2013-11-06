/*
 * ConnectionHandler.h
 *
 *  Created on: 06.11.2013
 *      Author: matthias
 */

#ifndef CONNECTIONHANDLER_H_
#define CONNECTIONHANDLER_H_

#include "MessageService.h"

class ConnectionHandler {
public:
	ConnectionHandler(int socket, std::string username, MessageService* msgService);
	virtual ~ConnectionHandler();
	virtual void sendCommand();
	virtual void list();
	virtual void read();
	virtual void del();
private:
	int socket;
	std::string username;
	MessageService* msgService;
};

#endif /* CONNECTIONHANDLER_H_ */
