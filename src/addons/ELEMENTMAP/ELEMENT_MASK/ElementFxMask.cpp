//
//  ElementFxMask.cpp
//  elementPlayer_a0.1_Eloi
//
//  Created by mauro on 23/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ElementFXMask.h"
#include "Element.h"
#include <GLUT/GLUT.h>


static ofShader             shader;
static int                  shaderInit = 0;
static ofShader             internalShader;
static int                  shaderInitInternal = 0;

ElementFXMask::ElementFXMask()
{
    
}

void ElementFXMask::init(int _type, ofTexture& originalLeftTexture)
{
    whichPoint = -1;
    ElementFX::init(_type, originalLeftTexture);
    
    maskFbo.allocate(this->getWidth(), this->getHeight());
    maskFbo.begin();
    ofClear(0,0,0,0);
    maskFbo.end();
    this->modifyEffect();
    
    finalFbo.allocate(this->getWidth(), this->getHeight());
    finalFbo.begin();
    ofClear(0, 0, 0, 0); 
    finalFbo.end();
    isDragging = false;
    isInverted = false;
}

void ElementFXMask::setIsInverted(bool b)
{
    this->isInverted = b;
}

bool ElementFXMask::getIsInverted()
{
    return this->isInverted;
}
                                        

void ElementFXMask::update()
{
    
}

void ElementFXMask::startSpecialFX(ofFbo* originalFboInternal)
{
    if(shaderInitInternal == 0)
    {
        string shaderProgram = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        \
        void main (void){\
        vec2 pos = gl_TexCoord[0].st;\
        \
        vec3 src = texture2DRect(tex0, pos).rgb;\
        float mask = texture2DRect(maskTex, pos).r;\
        \
        gl_FragColor = vec4( src , mask);\
        }";
        internalShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
        internalShader.linkProgram();
        shaderInitInternal++;
    }
    internalShader.begin();  
}

void ElementFXMask::applySpecialFX(ofFbo* originalFboInternal)
{
    maskFbo.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofSetColor(255, 0, 0);
    glBegin(GL_POLYGON);
    for(int a = 0; a < points.size(); a++)  
        glVertex2d(points[a]->x * this->getLeftTexture().getWidth(), points[a]->y * this->getLeftTexture().getHeight());
    glEnd();  
    ofPopStyle();  
    maskFbo.end();
    internalShader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );  
    originalFboInternal->draw(0, 0);
}
void ElementFXMask::endSpecialFX(ofFbo* originalFboInternal)
{
    internalShader.end();
}


void ElementFXMask::applyFX(ofTexture &originalTexture)
{
    if(shaderInit == 0)
    {
        string shaderProgram = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex0;\
        uniform sampler2DRect maskTex;\
        \
        void main (void){\
        vec2 pos = gl_TexCoord[0].st;\
        \
        vec3 src = texture2DRect(tex0, pos).rgb;\
        float mask = texture2DRect(maskTex, pos).r;\
        \
        gl_FragColor = vec4( src , mask);\
        }";
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
        shaderInit++;;         
        shader.linkProgram();
    }
    
    finalFbo.begin();
    ofClear(0, 0, 0, 0); 
    //    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
    //
    //  this->getElement().draw(0, 0);
    originalTexture.draw(0, 0);
    //  
    //ofSetColor(255, 0, 0);
    //  ofCircle(100, 100, 100);
    //maskFbo.draw(0, 0);
    //
    shader.end();
    finalFbo.end(); 
}

void ElementFXMask::applyFX()
{        
    if(shaderInit == 0)
        {
            string shaderProgram = "#version 120\n \
            #extension GL_ARB_texture_rectangle : enable\n \
            \
            uniform sampler2DRect tex0;\
            uniform sampler2DRect maskTex;\
            \
            void main (void){\
            vec2 pos = gl_TexCoord[0].st;\
            \
            vec3 src = texture2DRect(tex0, pos).rgb;\
            float mask = texture2DRect(maskTex, pos).r;\
            \
            gl_FragColor = vec4( src , mask);\
            }";
            shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
            shaderInit++;;         
            shader.linkProgram();
    }

    finalFbo.begin();
    ofClear(0, 0, 0, 0); 
//    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
//
  //  this->getElement().draw(0, 0);
    this->getLeftTexture().draw(0, 0);

  //  maskFbo.draw(0, 0);
    //  
    //ofSetColor(255, 0, 0);
  //  ofCircle(100, 100, 100);
    //maskFbo.draw(0, 0);
//
    shader.end();
    finalFbo.end();
    
//    this->getElement()->fboLeft.begin();
//    ofClear(0, 0, 0, 0);
//    finalFbo.draw(0, 0);
//    this->getElement()->fboLeft.end();
    
    
    
    /*
    if(this->getElement()->getIsStereo())
    {
        cout << "STEREO " << this->getElement()->getIsStereo() << endl;
        finalFbo.begin();
        ofClear(0, 0, 0, 0); 
        
        shader.begin();
        shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
        
        this->getElement()->fboRight.draw(0, 0);
            
        shader.end();
        finalFbo.end();
        
        this->getElement()->fboRight.begin();
        ofClear(0, 0, 0, 0);
        finalFbo.draw(0, 0);
        this->getElement()->fboRight.end();
    }
    */

}

void ElementFXMask::modifyEffect()
{
    maskFbo.begin();
    if(this->getIsInverted())
        ofClear(255, 255, 255, 255);
    else
        ofClear(0, 0, 0, 255);

    this->drawMask(255, this->getLeftTexture().getWidth(), this->getLeftTexture().getHeight());
    
//    ofPushStyle();
//        ofSetColor(255, 0, 0);
//        glBegin(GL_POLYGON);
//        for(int a = 0; a < points.size(); a++)  
//            glVertex2d(points[a]->x * this->getLeftTexture().getWidth(), points[a]->y * this->getLeftTexture().getHeight());
//        glEnd();  
//    ofPopStyle();  
    maskFbo.end();
}

void ElementFXMask::drawMask(int opacity, int w, int h)
{
    ofColor filledColor;
    if(isInverted)
    {
        filledColor.r = 0;
        filledColor.g = 0;
        filledColor.b = 0;        
    }
    else
    {
        filledColor.r = 255;
        filledColor.g = 255;
        filledColor.b = 255;        
    }
    filledColor.a = opacity;
    maskPath.clear();
    maskPath.setFillColor(filledColor);
    //maskPath.setPolyWindingMode();
    for(int a = 0; a < points.size(); a++)  
        maskPath.lineTo(points[a]->x * w, points[a]->y * h);
    maskPath.close();
    maskPath.draw(0, 0);
}

void ElementFXMask::resetFX()
{
    points.clear();
    for(int a = 0; a < 4; a++)
        points.push_back(new ofPoint());
    
    points[0]->x = 0;
    points[0]->y = 0;
    
    points[1]->x = 1;
    points[1]->y = 0;
    
    points[2]->x = 1;
    points[2]->y = 1;
    
    points[3]->x = 0;
    points[3]->y = 1;
}

void ElementFXMask::drawGUI(int x, int y, int w, int h)
{
    previewPos.x = x;
    previewPos.y = y;
    previewWidth = w;
    previewHeight = h;
    
    if(isDragging)
        ofSetColor(255, 200);
    else 
        ofSetColor(255, 1);
    
    ofPushMatrix();
    ofTranslate(x, y);
    
    /// http://forum.openframeworks.cc/index.php/topic,6089.0.html
    // Usare ofPath o ofPoly
    
    this->drawMask(100, w, h);
//    glBegin(GL_POLYGON);
//    for(int a = 0; a < points.size(); a++)
//        glVertex2d(points[a]->x * w, points[a]->y * h);
//    glEnd();
    
    ofPushStyle();
    ofSetColor(255, 0,0,255);
    
    for(int a = 0; a < points.size(); a++)
    {
        ofFill();
        ofCircle(points[a]->x * w, points[a]->y * h, 5);
        if(a == whichPoint)
        {
            ofNoFill();
            ofCircle(points[a]->x * w, points[a]->y * h, 20);
        }
        //ofDrawBitmapString(ofToString(a), points[a]->x * w, points[a]->y * h);
    }    
    ofPopStyle();
    ofPopMatrix();
    
    ofSetColor(255);
}

void ElementFXMask::mousePressed(ofMouseEventArgs &args)
{
    if(!this->hitTest(args.x, args.y)) return;

    ofPoint pointClicked((float)(args.x - previewPos.x)/previewWidth, (float)(args.y - previewPos.y)/previewHeight);
    
    if(this->ofKeyControl())
    {
        points.push_back(new ofPoint(pointClicked.x, pointClicked.y));
        if(this->ofKeyShift())
            std::sort(points.begin(), points.end(), pred());
        this->modifyEffect();
    }
    else 
    {
        float smallestDist = 1.0;
        whichPoint = -1;
        
        for(int i = 0; i < points.size(); i++)
        {
            float dist = points[i]->distance(pointClicked);
            if(dist < smallestDist && dist < 0.05)
            {
                whichPoint = i;
                smallestDist = dist;
            }
        }
        
        if(this->ofKeyAlt())
        {            
            vector<ofPoint*>::iterator vItr = points.begin();
            int cont = 0;
            while ( vItr != points.end() )
            {
                if ( cont == whichPoint )
                {
                    vItr = points.erase( vItr );
                    break;
                }
                else
                {
                    cont++;
                    vItr++;
                }
            }
            whichPoint = -1;
            this->modifyEffect();
        }
    }
}

void ElementFXMask::mouseReleased(ofMouseEventArgs &args)
{
    whichPoint = -1;
    isDragging = false;
}

void ElementFXMask::mouseMoved(ofMouseEventArgs &args)
{
    
}

void ElementFXMask::keyPressed(ofKeyEventArgs &e)
{
    bool GUIvisible = this->getGUIVisible();
    bool FXactived = this->getIsActive();
    bool inv = this->getIsInverted();
    switch(e.key)
    {
        case 'm':
            GUIvisible != GUIvisible;
            this->setGUIVisible(!GUIvisible);
            break;
        case 'n':
            FXactived != FXactived;
            this->setIsActive(!FXactived);
            break;
        case 'b':
            inv != inv;
            this->setIsInverted(!inv);
            this->modifyEffect();
            break;              
    }
}

bool ElementFXMask::ofKeyAlt() {
    return (glutGetModifiers() & GLUT_ACTIVE_ALT);
}

bool ElementFXMask::ofKeyShift() {
    return (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
}

bool ElementFXMask::ofKeyControl() {
    return (glutGetModifiers() & GLUT_ACTIVE_CTRL);
}

void ElementFXMask::mouseDragged(ofMouseEventArgs &args)
{
    isDragging = true;
    if(whichPoint >= 0)
    {
        points[whichPoint]->x = ofClamp((args.x - previewPos.x) / previewWidth, 0, 1);
        points[whichPoint]->y = ofClamp((args.y - previewPos.y) / previewHeight, 0, 1);
        this->modifyEffect();
    }
}

void ElementFXMask::activeEvents()
{
    ofAddListener(ofEvents().mousePressed, this, &ElementFXMask::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ElementFXMask::mouseReleased);
    ofAddListener(ofEvents().mouseMoved, this, &ElementFXMask::mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ElementFXMask::mouseDragged);
    ofAddListener(ofEvents().keyPressed, this, &ElementFXMask::keyPressed);
}

void ElementFXMask::deactiveEvents()
{
    ofRemoveListener(ofEvents().mousePressed, this, &ElementFXMask::mousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &ElementFXMask::mouseReleased);
    ofRemoveListener(ofEvents().mouseMoved, this, &ElementFXMask::mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &ElementFXMask::mouseDragged);
    ofRemoveListener(ofEvents().keyPressed, this, &ElementFXMask::keyPressed);
}