/*
 * BockedUser.cpp
 *
 *  Created on: Oct 29, 2013
 *      Author: richie
 */

#include "BlockedUser.h"

BlockedUser::~BlockedUser(){

}

std::string BlockedUser::getIPAdress(){
	return this->ipAdress;
}

void BlockedUser::setIPAdress(std::string ipAdress){
	this->ipAdress = ipAdress;
}

int BlockedUser::getTimeStamp(){
	return this->timeStamp;
}

void BlockedUser::setTimeStamp(int timeStamp){
	this->timeStamp = timeStamp;
}



