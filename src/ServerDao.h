/*
 * ServerDao.h
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#ifndef SERVERDAO_H_
#define SERVERDAO_H_

#include <string>

class ServerDao {
public:
	ServerDao(std::string dirPath);
	virtual ~ServerDao();
};

#endif /* SERVERDAO_H_ */
