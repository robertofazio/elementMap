// this version HACKS ofAppGlutWindow in order to enable GL_STEREO mode !! 


// TO : DO 

// add swapLeftRight on element_img

#pragma once

#include "ofMain.h"
#include "elementMain.h"
#include "ofxSimpleGuiToo.h"

class testApp : public ofBaseApp{

	public:

	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	vector<element*>			myElements;	
	elementVideo			elemV1;
	elementSyphon			elemSy;
	elementMixer			elemMix;
	elementImage			elemImg;
	elementImage			elemImg2;
	
	GLboolean				isStereoCapable;
	
	bool					drawAllStereo;
	
	
	int						outputResolutionX;
	int						outputResolutionY;
	
	bool					drawPreviews;
	bool					drawUIs;
	
};
