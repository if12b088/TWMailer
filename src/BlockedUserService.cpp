/*
 * BlockedUserService.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: richie
 */

#include "BlockedUserService.h"
#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <pthread.h>

#define BLOCKEDUSERFILE "blockedUser.log"

static const double BLOCKEDINTERVALL = 30 * 60; // 30 Minuten
static const std::string DELIMETER = "#";
static pthread_mutex_t blockedUserMutex = PTHREAD_MUTEX_INITIALIZER;

BlockedUserService::BlockedUserService(std::string path)
{
	std::string blockedFilePath;
	blockedFilePath.append(path);
	blockedFilePath.append("/");
	blockedFilePath.append(BLOCKEDUSERFILE);

	this->path = blockedFilePath;
}

BlockedUserService::~BlockedUserService()
{
	// TODO Auto-generated destructor stub
	}

	void BlockedUserService::blockUser(std::string ipAddress)
	{
		std::map<std::string, time_t> blockedUser = readBlockedUser();
		time_t blockedTime = time(NULL);
		std::map<std::string, time_t>::iterator it = blockedUser.find(ipAddress);
		if (it == blockedUser.end())
		{
			blockedUser.insert(
					std::pair<std::string, time_t>(ipAddress, blockedTime));
		}
		else
		{
			it->second = blockedTime;
		}
		writeBlockedUser(blockedUser);
	}

	bool BlockedUserService::isBlocked(std::string ipAddress)
	{
		std::map<std::string, time_t> blockedUser = readBlockedUser();
		std::map<std::string, time_t>::iterator it = blockedUser.find(ipAddress);
		if (it != blockedUser.end())
		{
			time_t now = time(NULL);
			time_t blockedTime = it->second;
			double diffSeconds = difftime(now, blockedTime);
			if (BLOCKEDINTERVALL > diffSeconds)
			{
				return true;
			}
		}
		return false;
	}

std::map<std::string,time_t > BlockedUserService ::readBlockedUser(){
	std::map<std::string, time_t> blockedUserMap;
	std::string line;

	pthread_mutex_lock(&blockedUserMutex);

	std::ifstream f(path);
	if (f.is_open())
	{
		while (getline(f, line))
		{
			size_t delimeterPos = line.find(DELIMETER);
			std::string ipAddress = line.substr(0, delimeterPos);
			time_t timestamp = atol(
			line.substr(delimeterPos + DELIMETER.length(),
					line.length()).c_str());

			blockedUserMap.insert(std::pair<std::string, time_t>(ipAddress, timestamp));

		}
		f.close();
	}else{
		exit(EXIT_FAILURE);
	}

	pthread_mutex_unlock(&blockedUserMutex);

	return blockedUserMap;
}

void BlockedUserService::writeBlockedUser(std::map<std::string, time_t> blockedUserMap) {
	pthread_mutex_lock(&blockedUserMutex);
	std::fstream f(path, std::ios::out);
	for (std::map<std::string, time_t>::iterator it = blockedUserMap.begin(); it != blockedUserMap.end(); it++) {
		f << it->first << DELIMETER << it->second << std::endl;
	}
	f.close();
	pthread_mutex_unlock(&blockedUserMutex);
}

