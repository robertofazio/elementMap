#ifndef ELEMENTSYPHON
#define ELEMENTSYPHON


#include "ofMain.h"
#include "element.h"
#include "ofxSyphon.h"

class elementSyphon : public element
{
	public :
	
	elementSyphon();
	
	void setup(string _applicationName,string _serverName,int _width, int _height,int _posX, int _posY,string _name, bool _isWarpable);
	void drawIntoFbo(bool _drawMonoOrStereo){};
	void update();
	void drawLeft(int x, int y, int w, int h);
	void drawRight(int x, int y, int w, int h);

	ofTexture& getLeftTexture();
	ofTexture& getRightTexture();

	
	ofxSyphonClient	syphonClient;
	
	string	serverName;
	string  applicationName;
	
	int syphonWidth;
	int syphonHeight;

};

#endif