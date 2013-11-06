/*
 * File.h
 *
 *  Created on: 26.10.2013
 *      Author: matthias
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>

class File {
public:
	virtual ~File();
	virtual long long getFilesize();
	virtual void setFilesize(long long filesize);
	virtual std::string getFilename();
	virtual void setFilename(std::string filename);
	virtual char* getFile();
	virtual void setFile(char* file);
private:
	long long filesize;
	std::string filename;
	char* file;
};

#endif /* FILE_H_ */
