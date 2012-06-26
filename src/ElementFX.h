//
//  ElementFX.h
//  elementPlayer_a0.1_Eloi
//
//  Created by mauro on 23/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef elementPlayer_a0_1_Eloi_ElementFX_h
#define elementPlayer_a0_1_Eloi_ElementFX_h

#include "ofMain.h"


//                          0
enum typeOfElementFX {ELEMENT_FX_MASK};

class element;

class ElementFX 
{
public:
                                ElementFX(){};
    void                        init(int _type, ofTexture& originalLeftTexture);
    
    virtual void                update() = 0;
    virtual void                applyFX() = 0;
    virtual void                applyFX(ofTexture &originalTexture) = 0;
    virtual void                startSpecialFX(ofFbo* originalFboInternal) = 0;
    virtual void                applySpecialFX(ofFbo* originalFboInternal) = 0;
    virtual void                endSpecialFX(ofFbo* originalFboInternal) = 0;
    virtual void                drawGUI(int x, int y, int w, int h) = 0;
    virtual void                resetFX() = 0;
    virtual void                modifyEffect() = 0;
    
    virtual void                activeEvents() = 0;
    virtual void                deactiveEvents() = 0;
    
    virtual void                mousePressed(ofMouseEventArgs &args) = 0;
    virtual void                mouseReleased(ofMouseEventArgs &args) = 0;
    virtual void                mouseMoved(ofMouseEventArgs &args) = 0;
    virtual void                mouseDragged(ofMouseEventArgs &args) = 0;
    
    bool                        hitTest(int x, int y);
    
    void                        setIsStereo(bool b);
    void                        setIsActive(bool a);
    void                        setGUIVisible(bool g);
    
    bool                        getIsStereo();
    bool                        getIsActive();
    bool                        getGUIVisible();
    int                         getWidth();
    int                         getHeight();
    int                         getElementFXType();
    
    ofFbo*                      fboLeft;
    ofFbo*                      fboRight;
    
    ofTexture&                   getLeftTexture();
    ofFbo                       finalFbo;
    
protected:
    
    ofTexture                  originalLeftTexture;
    bool                        isActive;
    bool                        guiVisibile;
    int                         width;
    int                         height;
    int                         previewWidth;
    int                         previewHeight;
    ofPoint                     previewPos;

private:
    int                         type;   
    bool                        isStereo; 
};


#endif
