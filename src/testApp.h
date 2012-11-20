#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"
#include "OutputWindow.h"
#include "MainWindow.h"

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
    //   void windowResizedFenster(int &w, int &h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void drawScene();
	void PlaceSceneElements();
    void mouseMovedEvent(ofMouseEventArgs &args);
    
    void selectOutputResolution();
    void firstSetup();
    
    ofxFenster* output;
    OutputWindow* outputWindow;

    ofxFenster* test;
    
    MainWindow*  mainWindow;
    
    ofxUICanvas*    selectResolutionGUI;
    ofxUILabelButton*   doneButton;
    void guiEvent(ofxUIEventArgs &e);   
    bool            bOptionsDone;
    vector <string> resolutionsList;
    ofxUIDropDownList* resGUI;
    
    ofImage     logoNew;
    
    int         outputSizeWidth;
    int         outputSizeHeight;
    
    bool        outputFullScreen;

    bool        QuadBufferCapable;

};
