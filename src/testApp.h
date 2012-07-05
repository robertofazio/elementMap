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

#include "ofxQuadWarp.h"

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
    
    
    //TEMPORARY SOLUTION: WARPING IS ONLY ACTIVE IN FULLSCREEN:
    //warping vars and functions:
    
    void createGrid(int _xRes, int _yRes);
    void drawGrid();
    
    ofImage     img;
    ofTexture   text;
    
    int xRes;
    int yRes;
    
    int nPoints;
    int nQuads;
    
    int width;
    int height;
    
    float zoomRatioX;
    float zoomRatioY;
    
    ofPoint vertici[MAX_POINTS];
    ofPoint texVert[MAX_POINTS];
    ofPoint screenPos[MAX_POINTS];
    ofPoint mainVertici[4];
    int     mainIndex[4];
    
    bool bWarpActive;
    bool bSposta;
    bool bHoldSelection;
    
    //ofxQuadWarp
    ofxQuadWarp quadWarp;
    ofFbo       qwFbo;
    ofMatrix4x4 mat;
    string      comandi;
    

    bool        bPaused;

    
    ofTrueTypeFont          verdana14;
    ofTrueTypeFont          verdana8;

    // Save/load xml /thanks to Mauro!)
	ofxXmlSettings		XML;
    void                saveXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex);
    void                loadXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex);

	
};
