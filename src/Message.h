/*
 * Message.h
 *
 *  Created on: 13.10.2013
 *      Author: matthias
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <list>
#include "File.h"

class Message {
public:
	Message();
	virtual ~Message();
	virtual std::string getFrom();
	virtual void setFrom(std::string from);
	virtual std::list<std::string> getTo();
	virtual void setTo(std::list<std::string> to);
	virtual std::string getSubject();
	virtual void setSubject(std::string subject);
	virtual std::string getText();
	virtual void setText(std::string text);
	virtual long long getMsgNr();
	virtual void setMsgNr(long long msgNr);
	virtual bool isFileAttached();
	virtual void setFileAttached(bool fileAttached);
	virtual void setFile(File* file);
	virtual File* getFile();
	virtual std::string toString();
	virtual std::string toProtocol();
private:
	long long msgNr;
	std::string from;
	std::list<std::string> to;
	std::string subject;
	std::string text;
	bool fileAttached;
	File* file;
};

#endif /* MESSAGE_H_ */
