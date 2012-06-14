#ifndef ELEMENTSYPHON
#define ELEMENTSYPHON


#include "ofMain.h"
#include "element.h"
#include "ofxSyphon.h"

class elementSyphon : public element
{
	public :
	
	elementSyphon();
	
	void setup(string _applicationName,string _serverName,int _width, int _height);
	void drawIntoFbo(bool _drawMonoOrStereo);
	void update(){};
	
	ofxSyphonClient	syphonClient;
	
	string	serverName;
	string  applicationName;
	
	int syphonWidth;
	int syphonHeight;

};

#endif;