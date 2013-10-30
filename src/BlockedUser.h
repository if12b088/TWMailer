/*
 * BlockedUser.h
 *
 *  Created on: Oct 29, 2013
 *      Author: richie
 */

#ifndef BLOCKEDUSER_H_
#define BLOCKEDUSER_H_

#include <string>

class BlockedUser {
public:
	virtual ~BlockedUser();
	virtual std::string getIPAdress();
	virtual void setIPAdress(std::string ipAdress);
	virtual int getTimeStamp();
	virtual void setTimeStamp(int timeStamp);
private:
	std::string ipAdress;
	int timeStamp;
};

#endif /* BLOCKEDUSER */
