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

	void setElementInputType(int _inType);

    void openLeft(string _path){};
    void openRight(string _path){};

    //textures
	ofTexture& getLeftTexture();
	ofTexture& getRightTexture();
    
    //stereo input utilities:
    ofTexture lT;
    ofTexture rT;
    ofFbo tempLeft;
    ofFbo tempRight;    

    //Syhpon:
	ofxSyphonClient	syphonClient;
	string	serverName;
	string  applicationName;
	
	int syphonWidth;
	int syphonHeight;

};

#endif