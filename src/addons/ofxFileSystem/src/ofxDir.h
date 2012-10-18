/*
 * ofxDir.h
 *
 *  Created on: 29/11/2009
 *      Author: arturo castro
 */

#ifndef OFXDIR_H_
#define OFXDIR_H_

#include "ofMain.h"
#include "ofxFile.h"

class ofxDir {
public:
	ofxDir();
	virtual ~ofxDir();

	bool open(string path, bool create=false);
	void close();

	int getNumDirs();
	int getNumFiles();

	ofxFile getFile(int num);
	ofxFile getFile(const string & name);
	string getFileName(int num);
	string getFilePath(int num);

	ofxDir getDir(int num);
	ofxDir getDir(const string & name);
	string getDirName(int num);
	string getDirPath(int num);

	bool fileExists(const string & name);
	bool dirExists(const string & name);

	bool createFile(const string & name);
	bool createDir(const string & name);

	bool remove(const string & name = ".");
	bool copyTo(string path);

private:
	vector<string> files;
	vector<string> directories;
	vector <string> allowedFileExt;
	string path;
	string name;
};

#endif /* OFXDIR_H_ */
