#ifndef ELEMENTWARP
#define ELEMENTWARP

//by Ciz: ridurre questi valori per migliorare le performance in caso di necessità!
#define MAX_RESOLUTION  17
#define MAX_POINTS 2048

#include "ofMain.h"
#include "ofxQuadWarp.h"
#include "ofxXmlSettings.h"

class elementWarp
{
	public :
	
	elementWarp();
    
    void setup(int _outputWidth, int _outputHeight);
    void warp(ofTexture _text);
    void updateCoordinates();
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
    void createGrid(int _xRes, int _yRes);
    void drawGrid();
    
    //grid control:
    void increaseXgrid();
    void decreaseXgrid();
    void increaseYgrid();
    void decreaseYgrid();
    
    void selectMainCorners();
    void selectNextMainCorner();

    void resetCorners();
    void resetPoint();
    
    void pointUP();
    void pointDOWN();
    void pointLEFT();
    void pointRIGHT();
    
    void selectNextPoint();
    void selectPrevPoint();
    
    // Save/load xml /thanks to Mauro!)
	ofxXmlSettings		XML;
    void                saveXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex);
    void                loadXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex);

    void save();
    void load();
    
    ofImage     img;
    ofTexture   text;
    
    ofFbo   fboAnaglyph;
    
    int xRes;
    int yRes;
    
    int nPoints;
    int nQuads;
    
    int width;
    int height;
    
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

    
};

#endif