// this version HACKS ofAppGlutWindow in order to enable GL_STEREO mode !! 

//by Ciz: ridurre questi valori per migliorare le performance in caso di necessità!
#define MAX_RESOLUTION  17
#define MAX_POINTS 2048

// TO : DO 

// add swapLeftRight on element_img

#pragma once

#include "ofMain.h"
#include "elementMain.h"
#include "ElementFX.h"
#include "ElementFXMask.h"
#include "ofxFensterManager.h"
#include "elementWarp.h"
#include "ofxUI.h"


class MainWindow : public ofBaseApp{
    
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
    
    void deselectAllElements();
	
	element**				myElements;	
	elementVideo			elemV1;
	elementSyphon			elemSy;
	elementMixer			elemMix;
	elementImage			elemImg;
	elementImage			elemImg2;
    
	int						numOfElements;
		
	int						outputResolutionX;
	int						outputResolutionY;
    bool                    outputRatioWide;
	
	int*					drawingOrder;
    
    bool                    bFullscreen;
    bool                    bRisparmio;
    
    bool                    frameByframe;
    
    bool                    bGLStereoCapable;
    
    ofFbo                   fboAnaglyph;
    
    string                  comandi;
    
    ofTrueTypeFont          fontLarge;
    ofTrueTypeFont          fontSmall;
    ofTrueTypeFont          fontMedium;
    
    ofImage                 logo;
    ofImage                 previewGrid;
    ofImage                 sfondo;
    
    //interfaccia minimale
    void guiEvent(ofxUIEventArgs &e);   
    ofxUICanvas*            minimalGUI;
    ofxUILabelButton*       minPlay;
    ofxUILabelButton*       minPause;
    ofxUILabelButton*       minRev;
	
};
