/*
 * BlockedUserService.h
 *
 *  Created on: Oct 30, 2013
 *      Author: richie
 */

#ifndef BLOCKEDUSERSERVICE_H_
#define BLOCKEDUSERSERVICE_H_

#include <string>

class BlockedUserService {
public:
	BlockedUserService();
	virtual ~BlockedUserService();
	virtual void blockUser(std::string ipAddress);
	virtual bool isBlocked(std::string ipAddress);
};

#endif /* BLOCKEDUSERSERVICE_H_ */
