/*
 * ofxFile.h
 *
 *  Created on: 29/11/2009
 *      Author: arturo castro
 */

#ifndef OFXFILE_H_
#define OFXFILE_H_

#include "ofMain.h"
#include <iostream>
#include <fstream>

class ofxFile {
public:
	ofxFile();
	virtual ~ofxFile();

	bool openReadOnly(const string & path);
	//bool openWrite(const string & path);
	//bool openAppend(const string & path);

	bool close();

	long size();

	//easier but slower to use, for performance or really big files use char*
	vector<char> getContent();
	long getContent(char*buffer);
	string getTextContent();

	bool copyTo(const string & newPath);

	string getPath();
	string getName();

private:
	ifstream * file;
	string path;
	string name;
};

#endif /* OFXFILE_H_ */
