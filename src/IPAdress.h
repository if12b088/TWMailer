/*
 * IPadress.h
 *
 *  Created on: Oct 29, 2013
 *      Author: richie
 */

#ifndef IPADRESS_H_
#define IPADRESS_H_

#include <string>

class IPAdress {
public:
	virtual ~IPAdress();
	virtual std::string getIPAdress();
	virtual void setIPAdress(std::string IPAdress);
	virtual int getTimeStamp();
	virtual void setTimeStamp(int timeStamp);
private:
	std::string IP;
	int TimeStamp;
};

#endif /* IPADRESS_H_ */
