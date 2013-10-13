/*
 * Message.h
 *
 *  Created on: 13.10.2013
 *      Author: matthias
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>

class Message {
public:
	Message();
	Message(std::string from, std::string to, std::string subject,
			std::string text);
	virtual ~Message();
	virtual std::string getFrom();
	virtual void setFrom(std::string from);
	virtual std::string getTo();
	virtual void setTo(std::string to);
	virtual std::string getSubject();
	virtual void setSubject(std::string subject);
	virtual std::string getText();
	virtual void setText(std::string text);
	virtual long long getMsgNr();
	virtual void setMsgNr(long long msgNr);
	virtual std::string toString();
private:
	std::string from;
	std::string to;
	std::string subject;
	std::string text;
	long long msgNr;
};

#endif /* MESSAGE_H_ */
