/*
 * LDAPService.h
 *
 *  Created on: 15.10.2013
 *      Author: matthias
 */

#ifndef LDAPSERVICE_H_
#define LDAPSERVICE_H_

#include <string>

class LDAPService {
public:
	LDAPService();
	virtual ~LDAPService();
	virtual bool login(std::string username, std::string password);
};

#endif /* LDAPSERVICE_H_ */
