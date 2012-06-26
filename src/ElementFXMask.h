//
//  ElementFXMask.h
//  elementPlayer_a0.1_Eloi
//
//  Created by mauro on 23/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef elementPlayer_a0_1_Eloi_ElementFXMask_h
#define elementPlayer_a0_1_Eloi_ElementFXMask_h

#include "ElementFX.h"



////////////////////// Code From Sons Of Poing /////////////////////////
////////////////////////////////////////////////////////////////////////
/// http://www.sonsofpong.com/blog/c-sorting-and-custom-comparator/ ////
////////////////////////////////////////////////////////////////////////
class pred {
    
private:
    static 
    float getAngle(ofPoint const & a) {
        ofVec2f origin = ofVec2f(0.5,0.5);
        ofVec2f p = ofVec2f(a.x, a.y);
        
        ofVec2f o_a = p-origin; // vector between origin and p
        ofVec2f xAxis = ofVec2f(0,1);
        
        float angle = o_a.angle(xAxis);
        return angle;
    }
    
public:
    bool operator()(ofPoint const * a, ofPoint const * b) const {
        return getAngle(*a) < getAngle(*b);
    }
};


/////////////////// End Code From Sons Of Poing ////////////////////////
////////////////////////////////////////////////////////////////////////
///////////////////////// Thanks Alberto ///////////////////////////////
////////////////////////////////////////////////////////////////////////

/*
Non capisco perchè questo non funziona:
 
 
//removePoint::pointToRemove = pointClicked; 
ofRemove(points, removePoint());

class removePoint{
public:
    static ofPoint pointToRemove;
    bool operator()(const ofPoint* p)
    {
        return  removePoint::pointToRemove.x == p->x;
    }
};
*/

class ElementFXMask : public ElementFX
{
public:
                        ElementFXMask();
    void                init(int _type, ofTexture& originalLeftTexture);
    void                resetFX();
    void                drawMask(int opacity, int w, int h);
    void                drawGUI(int x, int y, int w, int h);
    void                applyFX();
    void                applyFX(ofTexture &originalTexture);
    void                startSpecialFX(ofFbo* originalFboInternal);
    void                applySpecialFX(ofFbo* originalFboInternal);
    void                endSpecialFX(ofFbo* originalFboInternal);
    void                update();
    void                activeEvents();
    void                deactiveEvents();
    void                mousePressed(ofMouseEventArgs &args);
    void                mouseReleased(ofMouseEventArgs &args);
    void                mouseMoved(ofMouseEventArgs &args);
    void                mouseDragged(ofMouseEventArgs &args);
    void                keyPressed(ofKeyEventArgs &e);
    bool                ofKeyAlt();
    bool                ofKeyShift();
    bool                ofKeyControl();
    void                modifyEffect();
    bool                getIsInverted();
    void                setIsInverted(bool b);
    ofPath              maskPath;
    
protected:
    int whichPoint;
    vector<ofPoint*>    points;
    ofFbo               maskFbo;
    bool                isDragging;
    bool                isInverted;
};

#endif
