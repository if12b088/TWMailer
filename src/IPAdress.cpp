/*
 * IPAdress.cpp
 *
 *  Created on: Oct 29, 2013
 *      Author: richie
 */

#include "IPAdress.h"

IPAdress::~IPAdress(){

}

std::string IPAdress::getIPAdress(){
	return this->IP;
}

void IPAdress::setIPAdress(std::string IP){
	this->IP = IP;
}

int IPAdress::getTimeStamp(){
	return this->TimeStamp;
}

void IPAdress::setTimeStamp(int TimeStamp){
	this->TimeStamp = TimeStamp;
}



