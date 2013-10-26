/*
 * File.cpp
 *
 *  Created on: 26.10.2013
 *      Author: matthias
 */

#include "File.h"

File::~File() {
	free(file);
}

long long File::getFilesize() {
	return this->filesize;
}
void File::setFilesize(long long filesize) {
	this->filesize = filesize;
}
std::string File::getFilename() {
	return this->filename;
}
void File::setFilename(std::string filename) {
	this->filename = filename;
}
char* File::getFile() {
	return this->file;
}
void File::setFile(char* file) {
	this->file = file;
}

