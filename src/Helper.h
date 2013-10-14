/*
 * Helper.h
 *
 *  Created on: 13.10.2013
 *      Author: richie
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <unistd.h>
#include <errno.h>

class Helper {
public:
	static ssize_t readline(int fd, char *vptr, size_t maxlen);
private:
	static ssize_t my_read(int fd, char *ptr);

};

#endif /* HELPER_H_ */
