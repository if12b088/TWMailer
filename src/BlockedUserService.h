/*
 * BlockedUserService.h
 *
 *  Created on: Oct 30, 2013
 *      Author: richie
 */

#ifndef BLOCKEDUSERSERVICE_H_
#define BLOCKEDUSERSERVICE_H_

#include <string>
#include <map>

class BlockedUserService {
public:
	BlockedUserService(std::string path);
	virtual ~BlockedUserService();
	virtual void blockUser(std::string ipAddress);
	virtual bool isBlocked(std::string ipAddress);
private:
	virtual std::map<std::string, time_t> readBlockedUser();
	virtual void writeBlockedUser(std::map<std::string, time_t>);
	std::string path;
};

#endif /* BLOCKEDUSERSERVICE_H_ */
