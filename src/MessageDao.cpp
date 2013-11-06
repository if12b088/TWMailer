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
#include <pthread.h>

static pthread_mutex_t msgNrMutex = PTHREAD_MUTEX_INITIALIZER;

MessageDao::MessageDao(std::string dirPath) {
	this->dirPath = dirPath;
}

MessageDao::~MessageDao() {
}

bool MessageDao::saveMessage(Message* msg) {
	std::list<std::string> to = msg->getTo();
	for (std::list<std::string>::iterator it = to.begin(); it != to.end(); it++) {

		std::stringstream userPath;
		userPath << this->dirPath << "/" << *it;

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

		long long msgNr;

		pthread_mutex_lock(&msgNrMutex);
		struct timeval te;
		gettimeofday(&te, NULL); // get current time
		msgNr = te.tv_sec * 1000LL + te.tv_usec / 1000;
		pthread_mutex_unlock(&msgNrMutex);

		userPath << "/" << msgNr << ".msg";

		const char* fullPath = userPath.str().c_str();

		std::fstream f(fullPath, std::ios::out);
		f << msg->toString() << std::endl;
		f.close();

		if (msg->isFileAttached() == true) {
			File* file = msg->getFile();
			std::stringstream attachmentPath;
			attachmentPath << this->dirPath << "/" << *it << "/" << msgNr << "_" << file->getFilename();

			std::ofstream outfile(attachmentPath.str(), std::ofstream::binary);
			outfile.write(file->getFile(), file->getFilesize());
			outfile.close();
		}

	}
	return true;
}

std::list<Message*> MessageDao::loadMessages(std::string username) {
	std::string path;
	path.append(this->dirPath);
	path.append("/");
	path.append(username);

	std::list<Message*> list;

	DIR *dirp;
	struct dirent* dirent;
	if ((dirp = opendir(path.c_str())) != NULL) {
		while ((dirent = readdir(dirp)) != NULL) {
			char* filename = dirent->d_name;
			int filename_len = strlen(filename);
			if (filename_len >= 4 && strcmp(filename + filename_len - 4, ".msg") == 0) {
				char* p;
				p = strtok(filename, ".");
				Message* msg = this->readMessageInternal(username, atoll(p));
				list.push_back(msg);
			}
		}
	} else {
		std::cerr << "User nicht vorhanden: " << path << std::endl;
	}
	return list;
}

Message* MessageDao::readMessage(std::string username, long long msgNumber) {

	long long msgNr = findMessageNr(username, msgNumber);

	if (msgNr == 0) {
		Message* msg = new Message();
		msg->setMsgNr(0);
		return msg;
	}

	return readMessageInternal(username, msgNr);

}

Message* MessageDao::readMessageInternal(std::string username, long long msgNr) {

	std::stringstream userPath;
	userPath << this->dirPath << "/" << username << "/" << msgNr << ".msg";

	Message* msg = new Message();
	msg->setMsgNr(0);

	const char* path = userPath.str().c_str();

	std::string line;
	std::ifstream f(path);
	if (f.is_open()) {
		msg->setMsgNr(msgNr);
		getline(f, line);
		msg->setFrom(line);
		getline(f, line);
		std::list<std::string> to;
		std::string delimiter = ";";
		size_t pos = 0;
		std::string token;
		while ((pos = line.find(delimiter)) != std::string::npos) {
			token = line.substr(0, pos);
			to.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
		if (line.length() > 0) {
			to.push_back(line);
		}
		msg->setTo(to);
		getline(f, line);
		msg->setSubject(line);
		getline(f, line);

		std::string text;
		std::string prefix = "ATT: ";
		if (line.substr(0, prefix.size()) == prefix) {
			std::stringstream attPath;
			std::string filename = line.substr(prefix.size());
			attPath << this->dirPath << "/" << username << "/" << msgNr << "_" << filename;
			std::ifstream infile(attPath.str(), std::ifstream::binary);
			// get size of file
			infile.seekg(0, infile.end);
			long size = infile.tellg();
			infile.seekg(0);
			// allocate memory for file content
			char* buffer = new char[size];
			// read content of infile
			infile.read(buffer, size);

			File* file = new File();
			file->setFilename(filename);
			file->setFilesize(size);
			file->setFile(buffer);

			msg->setFile(file);
			msg->setFileAttached(true);

			infile.close();

		} else {
			text.append(line);
			text.append("\n");
		}
		while (getline(f, line)) {
			text.append(line);
			text.append("\n");
		}
		msg->setText(text);
		f.close();
	}
	return msg;
}

bool MessageDao::delMessage(std::string username, long long msgNumber) {

	Message* delMsg = readMessage(username, msgNumber);

	if (delMsg->getMsgNr() == 0) {
		delete (delMsg);
		return false;
	}

	long long msgNr = delMsg->getMsgNr();

	std::stringstream userPath;
	userPath << this->dirPath << "/" << username << "/" << msgNr << ".msg";
	const char* path = userPath.str().c_str();

	if (remove(path) != 0) {
		delete (delMsg);
		return false;
	}

	if (delMsg->isFileAttached() == true) {
		std::stringstream attPath;
		attPath << this->dirPath << "/" << username << "/" << msgNr << "_" << delMsg->getFile()->getFilename();
		const char* path = attPath.str().c_str();

		if (remove(path) != 0) {
			delete (delMsg);
			return false;
		}
	}

	delete (delMsg);
	return true;
}

long long MessageDao::findMessageNr(std::string username, long long msgNumber) {

	std::stringstream dir;
	dir << this->dirPath << "/" << username << "/";

	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.str().c_str())) == NULL) {
		return 0;
	}

	long long result = 0;

	while ((dirp = readdir(dp)) != NULL) {
		std::string filename = dirp->d_name;
		if (filename.length() <= 4) {
			continue;
		}
		filename.erase(filename.length() - 4, filename.length());
		std::stringstream numberStream;
		numberStream << msgNumber;

		std::string number = numberStream.str();

		if (filename.compare(filename.length() - number.length(), number.length(), number) == 0) {
			if (result == 0) {
				result = atoll(filename.c_str());
			} else {
				result = 0;
				break;
			}
		}
	}
	closedir(dp);
	return result;
}

