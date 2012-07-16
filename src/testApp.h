// this version HACKS ofAppGlutWindow in order to enable GL_STEREO mode !! 

//by Ciz: ridurre questi valori per migliorare le performance in caso di necessitˆ!
#define MAX_RESOLUTION  17
#define MAX_POINTS 2048

// TO : DO 

// add swapLeftRight on element_img

#pragma once

#include "ofMain.h"
#include "elementMain.h"
#include "ElementFX.h"
#include "ElementFXMask.h"

#include "elementWarp.h"


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
	
	element**				myElements;	
	elementVideo			elemV1;
	elementSyphon			elemSy;
	elementMixer			elemMix;
	elementImage			elemImg;
	elementImage			elemImg2;
	int						numOfElements;
	
	GLboolean				isStereoCapable;
	
	int						outputResolutionX;
	int						outputResolutionY;
	
	bool					drawPreviews;
	bool					drawUIs;
	int*					drawingOrder;
    
    bool                    bFullscreen;
    
    bool                    bSpeedUp;
    bool                    frameByframe;
    
    ofTexture   text;
    
    ofFbo   fboAnaglyph;

    string      comandi;
    
    elementWarp mainOutputWarp;

    bool        bPaused;


    ofTrueTypeFont          georgiaitalic14;
    ofTrueTypeFont          georgiaitalic8;
    
    ofImage                 logo;


	
};
