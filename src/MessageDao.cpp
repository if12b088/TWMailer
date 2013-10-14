/*
 * MessageDao.cpp
 *
 *  Created on: 07.10.2013
 *      Author: richie
 */

#include "MessageDao.h"
#include <fstream>
#include <string.h>
#include <string>
#include <list>
#include <dirent.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

MessageDao::MessageDao(std::string dirPath) {
	this->dirPath = dirPath;
}

MessageDao::~MessageDao() {
	// TODO Auto-generated destructor stub
}

bool MessageDao::saveMessage(Message msg) {
	std::stringstream userPath;
	userPath << this->dirPath << "/" << msg.getTo();

	const char* path = userPath.str().c_str();

	DIR *dirp;
	if ((dirp = opendir(path)) == NULL) {

		if (mkdir(path, 0755) == -1) {
			std::cerr << "Error beim Dir erzeugen" << std::endl;
			return false;
		}
		if ((dirp = opendir(path)) == NULL) {
			std::cerr << "Error beim Dir auslesen" << std::endl;
			return false;
		}
	}
	closedir(dirp);

	struct timeval te;
	gettimeofday(&te, NULL); // get current time

	userPath << "/" << te.tv_sec * 1000LL + te.tv_usec / 1000 << ".msg";

	const char* fullPath = userPath.str().c_str();

	std::fstream f(fullPath, std::ios::out);
	f << msg.getFrom() << std::endl;
	f << msg.getTo() << std::endl;
	f << msg.getSubject() << std::endl;
	f << msg.getText() << std::endl;
	f.close();
	return true;
}

std::list<Message> MessageDao::loadMessages(std::string username) {
	std::string path;
	path.append(this->dirPath);
	path.append("/");
	path.append(username);

	std::list<Message> list;

	DIR *dirp;
	struct dirent* dirent;
	if ((dirp = opendir(path.c_str())) != NULL) {
		while ((dirent = readdir(dirp)) != NULL) {
			char* filename = dirent->d_name;
			int filename_len = strlen(filename);
			if (filename_len >= 4
					&& strcmp(filename + filename_len - 4, ".msg") == 0) {
				char* p;
				p = strtok(filename, ".");
				Message msg = this->readMessage(username, atoll(p));
				list.push_back(msg);
			}
		}
	} else {
		std::cerr << "User nicht vorhanden: " << path << std::endl;
	}
	return list;
}

Message MessageDao::readMessage(std::string username, long long msgNr) {
	std::stringstream userPath;
	userPath << this->dirPath << "/" << username << "/" << msgNr << ".msg";

	Message msg;
	msg.setMsgNr(msgNr);

	const char* path = userPath.str().c_str();

	std::string line;
	std::ifstream f(path);
	if (f.is_open()) {
		getline(f, line);
		msg.setFrom(line);
		getline(f, line);
		msg.setTo(line);
		getline(f, line);
		msg.setSubject(line);
		std::string text;
		while (getline(f, line)) {
			text.append(line);
		}
		msg.setText(text);
		f.close();
	}
	return msg;
}

bool MessageDao::delMessage(std::string username, long long msgNr) {
	std::stringstream userPath;
	userPath << this->dirPath << "/" << username << "/" << msgNr << ".msg";

	const char* path = userPath.str().c_str();

	if (remove(path) != 0)
		return false;
	else
		return true;
}

