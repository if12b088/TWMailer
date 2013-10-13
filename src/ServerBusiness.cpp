/*
 * ServerBusiness.cpp
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#include "ServerBusiness.h"
#include "ServerDao.h"

ServerBusiness::ServerBusiness(ServerDao dao) {
	// TODO Auto-generated constructor stub

}

ServerBusiness::~ServerBusiness() {
	// TODO Auto-generated destructor stub
}

bool ServerBusiness::sendMsg(std::string from, std::string to, std::string subject, std::string msg){
	return true;
}
std::list<std::string> ServerBusiness::listMsg(std::string username){
	std::list<std::string> list;
	return list;
}
std::string ServerBusiness::readMsg(std::string username, int msgNr){
	return "bla";
}
bool ServerBusiness::deleteMsg(std::string username, int msgNr){
	return true;
}

