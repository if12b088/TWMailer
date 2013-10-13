/*
 * ServerBusiness.h
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#ifndef SERVERBUSINESS_H_
#define SERVERBUSINESS_H_

#include <string>
#include <list>
#include "ServerDao.h"

class ServerBusiness {
public:
	ServerBusiness(ServerDao dao);
	virtual ~ServerBusiness();
	virtual bool sendMsg(std::string from, std::string to, std::string subject, std::string msg);
	virtual std::list<std::string> listMsg(std::string username);
	virtual std::string readMsg(std::string username, int msgNr);
	virtual bool deleteMsg(std::string username, int msgNr);

};

#endif /* SERVERBUSINESS_H_ */
