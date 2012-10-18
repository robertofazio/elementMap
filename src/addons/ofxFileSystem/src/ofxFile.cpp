/*
 * ofxFile.cpp
 *
 *  Created on: 29/11/2009
 *      Author: arturo castro
 */

#include "ofxFile.h"

ofxFile::ofxFile() {
	file = NULL;
}

ofxFile::~ofxFile() {
	close();
}

bool ofxFile::openReadOnly(const string & path){
	if(file && file->is_open()){
		if(!close()) return false;
	}

	file = new ifstream(ofToDataPath(path,true).c_str());

	if(!file || !file->is_open()){
		ofLog(OF_LOG_ERROR,"cannot open file " + ofToDataPath(path,true));
		close();
		return false;
	}else{
		this->path=ofToDataPath(path,true);
		name = this->path.substr(this->path.rfind('/')+1);
		return true;
	}
}
//bool openWrite(const string & path);
//bool openAppend(const string & path);

bool ofxFile::close(){
	if(!file) return true;
	file->close();
	if(file->is_open()){
		ofLog(OF_LOG_ERROR,"cannot close " + path);
		return false;
	}
	path = "";
	name = "";
	delete file;
	file = NULL;
	return true;
}

long ofxFile::size(){
	if(!file) return -1;
	filebuf *pbuf=file->rdbuf();

	// get file size using buffer's members
	long size=pbuf->pubseekoff (0,ios::end,ios::in);
	pbuf->pubseekpos (0,ios::in);

	return size;
}

vector<char> ofxFile::getContent(){
	if(!file) return vector<char>();
	unsigned filesize = size();
	char * buffer = new char[filesize];
	filebuf *pbuf=file->rdbuf();
	pbuf->pubseekpos (0,ios::in);
	// get file data
	pbuf->sgetn (buffer,filesize);
	return vector<char>(buffer,buffer+filesize);
}

long ofxFile::getContent(char*buffer){
	if(!file) return 0;
	int filesize = size();
	filebuf *pbuf=file->rdbuf();
	pbuf->pubseekpos (0,ios::in);
	// get file data
	pbuf->sgetn (buffer,filesize);
	//TODO: can this be different to size()
	return filesize;
}

string ofxFile::getTextContent(){
	if(!file) return string();
	stringbuf str;
	file->seekg(0,ios::beg);
	while(file->good()) *file >> &str;
	return str.str();
}

bool ofxFile::copyTo(const string & newPath){
	if(!file){
		ofLog(OF_LOG_ERROR,"cannot copy, no opened file yet");
		return false;
	}
	ofstream copy(ofToDataPath(newPath,true).c_str());
	if(!copy.is_open()){
		ofLog(OF_LOG_ERROR,"cannot copy " + path + " to " + newPath);
		return false;
	}
	file->seekg(0,ios::beg);
	copy << file->rdbuf();
	copy.close();
	return true;
}

string ofxFile::getPath(){
	return path;
}

string ofxFile::getName(){
	return name;
}

