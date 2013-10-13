/*
 * Message.cpp
 *
 *  Created on: 13.10.2013
 *      Author: matthias
 */

#include "Message.h"

Message::Message() {
}

Message::Message(std::string from, std::string to, std::string subject,
		std::string text) {
	this->from = from;
	this->to = to;
	this->subject = subject;
	this->text = text;
}

Message::~Message() {
	// TODO Auto-generated destructor stub
}

std::string Message::getFrom() {
	return this->from;
}
void Message::setFrom(std::string from) {
	this->from = from;
}
std::string Message::getTo() {
	return this->to;
}
void Message::setTo(std::string to) {
	this->to = to;
}
std::string Message::getSubject() {
	return this->subject;
}
void Message::setSubject(std::string subject) {
	this->subject = subject;
}
std::string Message::getText() {
	return this->text;
}
void Message::setText(std::string text) {
	this->text = text;
}
long long Message::getMsgNr() {
	return this->msgNr;
}
void Message::setMsgNr(long long msgNr) {
	this->msgNr = msgNr;
}

std::string Message::toString() {
	std::string result;
	result.append(this->from);
	result.append("\n");
	result.append(this->to);
	result.append("\n");
	result.append(this->subject);
	result.append("\n");
	result.append(this->text);
	return result;
}

