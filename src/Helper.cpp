/*
 * Helper.cpp
 *
 *  Created on: 13.10.2013
 *      Author: richie
 */

#include "Helper.h"
#define MAXLINE 1500

ssize_t Helper::my_read(int fd, char *ptr) {
	static int read_cnt = 0;
	static char *read_ptr;
	static char read_buf[MAXLINE];
	if (read_cnt <= 0) {
		again:
		if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return (-1);
		} else if (read_cnt == 0)
			return (0);
		read_ptr = read_buf;
	};
	read_cnt--;
	*ptr = *read_ptr++;
	return (1);
}

ssize_t Helper::readline(int fd, char *vptr, size_t maxlen) {
	ssize_t n, rc;
	char c, *ptr;
	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ((rc = Helper::my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n'){
				*(--ptr) = '\0'; // remove '\n'
				break;
			}
		} else if (rc == 0) {
			if (n == 1)
				return (0);
			else
				break;
		} else
			return (-1);
	};
	*ptr = 0;
	return (n);
}
