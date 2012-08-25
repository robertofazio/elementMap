//
//  OutputWindow.h
//  elementStereoInteractive
//
//  Created by mauro on 12/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef elementStereoInteractive_OutputWindow_h
#define elementStereoInteractive_OutputWindow_h

#include "ofMain.h"
#include "ofxFenster.h"
#include "elementWarp.h"

class testApp;

class OutputWindow: public ofxFensterListener{
public:
    OutputWindow(testApp* _mainScene);
    void        draw();
    void        setup();
    void        update();
    //  void        dragEvent(ofDragInfo &dragInfo);
    void        keyPressed(int key, ofxFenster* window);
    void        keyReleased(int key, ofxFenster* window);
    void        mouseDragged(int x, int y, int button,  ofxFenster* f);
    void        mousePressed(int x, int y, int btn, ofxFenster* f);
  //  void        dragEvent(ofDragInfo dragInfo,  ofxFenster* window);
    void        setWindowShape(int w, int h);  
    
    testApp*    mainScene;

    void        windowResized(int &w, int &h);
    
};


#endif
