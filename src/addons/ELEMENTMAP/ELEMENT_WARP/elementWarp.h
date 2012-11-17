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
    
    void setup(int _outputWidth, int _outputHeight, string _name);
    void draw(ofTexture _text);
    void updateCoordinates();
    
    void warpKeyPressedHandler(int _key);
    void warpKeyReleasedHandler(int _key);
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
    void setLeftRightInput();
    
    void createGrid(int _xRes, int _yRes);
    void drawGrid();
    void drawMarkers();
    
    //grid control:
    void increaseXgrid();
    void decreaseXgrid();
    void increaseYgrid();
    void decreaseYgrid();
    
    void selectMainCorners();
    void selectNextMainCorner();
    
    void deselectAll();

    void resetCorners();
    void resetPoint();
    
    void pointUP(int _delta);
    void pointDOWN(int _delta);
    void pointLEFT(int _delta);
    void pointRIGHT(int _delta);
    
    void selectNextPoint();
    void selectPrevPoint();
    
    void mirror(bool _horizontal, bool _vertical);
    void rotateCW();
    void rotateCCW();
    
    // Save/load xml (thanks to Mauro!)
	ofxXmlSettings		XML;
    void                saveXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex);
    void                loadXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex);

    void save();
    void load();
    
    string      xmlName;
    
    ofImage     img;
    ofTexture   text;
    
    ofFbo   fboAnaglyph;
    
    int xRes;
    int yRes;
    
    int nPoints;
    int nQuads;
    
    int width;
    int height;
    
    float gridFactorW;
    float gridFactorH;
    
    ofPoint vertici[MAX_POINTS];
    ofPoint texVert[MAX_POINTS];
    ofPoint screenPos[MAX_POINTS];
    ofPoint gridVert[MAX_POINTS];
    
    ofPoint mainVertici[4];
    int     mainIndex[4];
    
    bool bWarpActive;
    bool bSposta;
    bool bHoldSelection;
    bool bViewGrid;
    bool bMela;
    bool bSpeedUp;
        
    //ofxQuadWarp
    ofxQuadWarp quadWarp;
    ofFbo       qwFbo;
    ofMatrix4x4 mat;
    string      comandi;

    void drawElementOutline();
};

#endif