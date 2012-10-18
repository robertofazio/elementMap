/*
 * ofxDir.cpp
 *
 *  Created on: 29/11/2009
 *      Author: arturo castro
 */

#include "ofxDir.h"

#ifdef TARGET_WIN32
	#include <stdio.h>
	#include <iostream>
	#include <string.h>
#else
	#include <dirent.h>
#endif

static std::string::size_type idx;
static string getExt(string filename){
    idx = filename.rfind('.');

    if(idx != std::string::npos){
        return filename.substr(idx+1);
    }
    else{
        return "";
    }
}

static string strToLower(string myStr){
    transform( myStr.begin(), myStr.end(), myStr.begin(), ::tolower );
    return myStr;
}

ofxDir::ofxDir() {
	// TODO Auto-generated constructor stub

}

ofxDir::~ofxDir() {
	// TODO Auto-generated destructor stub
}

bool ofxDir::open(string directory, bool create){
	directory = ofToDataPath(directory,true);

	files.clear();
	directories.clear();

	if(directory.length() <= 0)return 0;

	//if the trailing slash was not added - then add it
	if( directory[directory.length()-1] != '/'){
		directory = directory + "/";
	}

	DIR *dir = NULL;
	struct dirent *entry;

	//open the directory
	ofLog(OF_LOG_VERBOSE, "ofxDir - attempting to open %s", directory.c_str());
	dir = opendir(directory.c_str());

	if(dir == NULL){
		//try to create it
		if(create){
			//mkdir(directory.c_str(),S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
			dir = opendir(directory.c_str());
		}
		if(dir==NULL){
			ofLog(OF_LOG_ERROR, "ofxDir - error opening directory");
			return false;
		}
	}

	ofLog(OF_LOG_VERBOSE, "ofxDir - success opening directory");


	string entry_name = "";
	string ext = "";

	//by default we don't skip files unless we are checking extensions
	bool skip = false;

	while ((entry = readdir(dir)) != NULL){

		//turn it into a C++ string
		entry_name = entry->d_name;

		//lets get the length of the string here as we query it again
		int fileLen = entry_name.length();

		if(fileLen <= 0)continue; //if the name is not existant
		if(entry_name[0] == '.')continue; //ignore invisible files, ./ and ../

		#ifdef _DIRENT_HAVE_D_TYPE
			if(entry->d_type==DT_DIR){
				directories.push_back(entry_name);
				continue;
			}
		#endif

		skip = false;

		if(allowedFileExt.size() > 0){
			//we will skip this files unless it has an allowed extension
			skip = true;
			for(int i = 0; i < (int)allowedFileExt.size(); i++){

				//if the wildecard * has been entered for an ext type then don't check any extensions
				if( allowedFileExt[i] == "*"){ skip = false; break; }


				int extLen = allowedFileExt[i].length();

				//the extension has to be shorter than the filename - simple check
				if(extLen >= fileLen) continue;

				//lets get the ext as lowercase
				ext = strToLower( getExt(entry_name) );

				//if no ext - then skip this ext check
				if( ext == "" )continue;

				//if we find a match then stop checking and approve this file
				if(ext == allowedFileExt[i]){
					skip = false;
					break;
				}
			}
		}

		if(skip) continue;

		files.push_back(entry_name);

		//ofLog(OF_LOG_VERBOSE, "ofxDirList - listing %s ", nameArray.back().c_str());
	}

	this->path=directory;
	name = this->path.substr(this->path.rfind('/')+1);
	//ofLog(OF_LOG_VERBOSE, "ofxDirList - listed %i files in %s", nameArray.size(), directory.c_str());
	return true;
}

void ofxDir::close(){
	files.clear();
	directories.clear();
	allowedFileExt.clear();
	name.clear();
	path.clear();
}

int ofxDir::getNumDirs(){
	return directories.size();
}

int ofxDir::getNumFiles(){
	return files.size();
}

ofxFile ofxDir::getFile(int num){
	ofxFile file;
	if(num < (int)files.size())
		file.openReadOnly(path + files[num]);
	return file;
}

ofxFile ofxDir::getFile(const string & name){
	ofxFile file;
	if(!file.openReadOnly(path + name)) ofLog(OF_LOG_ERROR, "cannot open " + path + name);;
	return file;
}

string ofxDir::getFileName(int num){
	return files[num];
}

string ofxDir::getFilePath(int num){
	return path + files[num];
}

ofxDir ofxDir::getDir(int num){
	ofxDir dir;
	if(num < (int)files.size())
		dir.open(path + directories[num]);
	return dir;
}

ofxDir ofxDir::getDir(const string & name){
	ofxDir dir;
	if(!dir.open(path + name)) ofLog(OF_LOG_ERROR, "cannot open " + path + name);
	return dir;
}

string ofxDir::getDirName(int num){
	return directories[num];
}

string ofxDir::getDirPath(int num){
	return path + directories[num];
}

bool ofxDir::fileExists(const string & name){
	for(unsigned i=0; i<files.size(); i++){
		if(files[i]==name) return true;
	}
	return false;
}

bool ofxDir::dirExists(const string & name){
	for(unsigned i=0; i<directories.size(); i++){
		if(directories[i]==name) return true;
	}
	return false;
}

bool ofxDir::createFile(const string & name){
	ofxFile file;
	return file.openReadOnly(path + name);
}

bool ofxDir::createDir(const string & name){
	if(name.find("/")!=string::npos){
		ofLog(OF_LOG_ERROR,"can only create directories directly in this path");
		return false;
	}
	return false; //mkdir((path + name).c_str(),S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=-1;
}

bool ofxDir::remove(const string & name){
	if(name=="."){
		for(unsigned i=0;i<files.size(); i++){
			unlink((path + files[i]).c_str());
		}
		for(unsigned i=0;i<directories.size();i++){
			ofxDir dir;
			dir.open(path + directories[i]);
			dir.remove();
		}
		rmdir(path.c_str());
		return true;
	}else{
		if(name.find("/")!=string::npos){
			ofLog(OF_LOG_ERROR,"only names in this directory allowed");
			return false;
		}
		if(fileExists(name)){
			unlink((path + name).c_str());
		}
		if(dirExists(name)){
			getDir(name).remove(".");
		}
		return true;
	}
}

bool ofxDir::copyTo(string path){
	path = ofToDataPath(path,true);
	if(path.length() <= 0)return false;

	//if the trailing slash was not added - then add it
	if( path[path.length()-1] != '/'){
		path = path + "/";
	}
	ofxDir dst;
	if(!dst.open(path,true)){
		ofLog(OF_LOG_ERROR, "cannot create directory " + path);
		return false;
	}
	bool allCopied=true;
	for(unsigned i=0; i<files.size(); i++){
		if(!getFile(getFileName(i)).copyTo(path + getFileName(i))){
			ofLog(OF_LOG_WARNING, "cannot copy " + getFilePath(i) + " to " + path + getFileName(i));
			allCopied = false;
		}
	}
	for(unsigned i=0;i<directories.size(); i++){
		if(!getDir(getDirName(i)).copyTo(path + getDirName(i))){
			ofLog(OF_LOG_WARNING, "cannot copy " + getDirPath(i) + " to " + path + getDirName(i));
			allCopied = false;
		}
	}
	return allCopied;
}
