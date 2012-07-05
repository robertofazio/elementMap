#include "elementMixer.h"
#include "elementVideo.h"

#include "testApp.h"

// TODO ::
// add support for alpha values on the PSmode mixer

// find a solution for the drawQuadGeometry . 
// now it's working because we're working an 
// image and we're discarting the tex0 uniform in shader.


//-----------------------------------------------------------------
void elementMixer::setup(int _width, int _height, int _stereoMode,element** _elements,int _numOfElements,int* _elementsOrder,int _posX, int _posY,string _name)
{
	setOutputStereoMode(_stereoMode);
	if(outputStereoMode==0)
	{	
		this->setDrawInStereo(true);
		this->setIsStereo(true);
		
	}
	// UI params
	xPos = _posX;
	yPos = _posY;

	this->init(5,_width,_height,GL_RGBA,_name,this->getIsStereo());
	
//    fboAnaglyph.allocate(_width, _height, GL_RGBA);
    
    sceneElements = _elements;
	numOfElements = _numOfElements;
	elementsOrder = _elementsOrder;
	elementsBlendModes = new int[numOfElements];
	elementsOpacity = new float[numOfElements];
    blendMode = 0;	
	shader.load("./shaders/pssh");
	blacktexture.loadImage("./images/blackTexture.jpg");
	
	useNoShader = false;
    
    ofAddListener(UI->newGUIEvent,this,&elementMixer::guiEvent); 
	
}


//-----------------------------------------------------------------
void elementMixer::update()
{
	for(int i=0;i<numOfElements;i++)
	{
		sceneElements[i]->update();
	}		
}

void elementMixer::applyFX()
{
 for(int a = 0; a < effects.size(); a++)
    {
        if(effects[a]->getIsActive())
            effects[a]->applyFX();
    }
}


void elementMixer::addFX(int type)       // Mauro
{
    switch(type)
    {
        case ELEMENT_FX_MASK:            
            newEffect.init(ELEMENT_FX_MASK, fboLeft.getTextureReference());
            effects.push_back(&newEffect);
            break;
    }    
}

//-----------------------------------------------------------------
void elementMixer::drawIntoFbo(bool _drawMonoOrStereo)
{
	if(isActive)
	{
		setDrawInStereo(_drawMonoOrStereo);
		
		//////////////////////////
		// here the left :
		//////////////////////////
        
		fboLeft.begin();
        ofClear(0,0,0,0);
        
        for(int a = 0; a < numOfElements; a++)
            if(sceneElements[elementsOrder[a]]->getIsActive())
            {
                float opacity = ofMap(sceneElements[elementsOrder[a]]->getOpacity(), 0, 1, 0, 255);
                ofSetColor(255, 255, 255, opacity);
                sceneElements[elementsOrder[a]]->drawLeft(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
            }
		
		fboLeft.end();
		
        //////////////////////////
		// here the right :
		//////////////////////////
        
		if(this->getIsStereo())
            {
                fboRight.begin();
                ofClear(0,0,0,0);
                
                for(int a = 0; a < numOfElements; a++)
                    if(sceneElements[elementsOrder[a]]->getIsActive())
                        {
                            float opacity = ofMap(sceneElements[elementsOrder[a]]->getOpacity(), 0, 1, 0, 255);
                            ofSetColor(255, 255, 255, opacity);
                            if(sceneElements[elementsOrder[a]]->getIsStereo())
                                sceneElements[elementsOrder[a]]->drawRight(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
                            else
                                sceneElements[elementsOrder[a]]->drawLeft(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
                                }

               fboRight.end();
            
                
                }
        
	}
}



//-----------------------------------------------------------------
void elementMixer::drawOutput(int _x, int _y,int _width, int _height)
{
	if(isActive)
	{
		switch (outputStereoMode) 
		{
			case ELM_STEREO_OPENGL:
				
				if(!getSwapLeftRight())
                {
                    // left
					glDrawBuffer(GL_BACK_LEFT);	
					setOpacityColor();
					fboLeft.draw(_x,_y,_width,_height);
					
					// right
					glDrawBuffer(GL_BACK_RIGHT);
					setOpacityColor();
					if(getDrawInStereo()) fboRight.draw(_x,_y,_width,_height);
					else fboLeft.draw(_x,_y,_width,_height);
				}
				else 
				{
					// left
					glDrawBuffer(GL_BACK_LEFT);	
					setOpacityColor();
					fboRight.draw(_x,_y,_width,_height);
					
					// right
					glDrawBuffer(GL_BACK_RIGHT);
					setOpacityColor();
					if(getDrawInStereo()) fboLeft.draw(_x,_y,_width,_height);
					else fboLeft.draw(_x,_y,_width,_height);
				}
                
				break;
				
			case ELM_STEREO_MONO:
                setOpacityColor();
                fboLeft.draw(_x,_y,_width,_height);
				
                
				break;
                
			case ELM_STEREO_ANAGLYPH:
 
				// left
				glColorMask(true, false, false, false);
				//setOpacityColor();
                ofSetColor(255, 255, 255);
				fboLeft.draw(_x,_y,_width,_height);
				
				// right
				glColorMask(false, true, true, false);
//				setOpacityColor();
				ofSetColor(255, 255, 255);
                fboRight.draw(_x,_y,_width,_height);
				
				glColorMask(true, true, true, true);
				
				break;
			default:
				break;
		}   
		drawInfo();
	}
}

/*
 
 Cose in this function works for FX... now i remove and we add better code later
 void elementMixer::drawPreview(int x, int y, int w, int h)
 {
 for(int a = 0; a < effects.size(); a++)
 {
 if(effects[a]->getIsActive())
 {
 applyFX();
 setOpacityColor();
 effects[a]->finalFbo.draw(x, y, w, h);
 }
 else
 effects[a]->getLeftTexture().draw(x, y, w, h);
 if(effects[a]->getGUIVisible())
 {
 effects[a]->drawGUI(x, y, w, h);
 }
 }
 
 if(effects.size() == 0)
 fboLeft.draw(x, y, w, h);
 }
 */

//-----------------------------------------------------------------
void elementMixer::drawInfo()
{
    ofPushStyle();
	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),ofGetWidth()-100,ofGetHeight()-80);
    
	ofSetColor(0,255,0);
    
	switch (outputStereoMode) 
	{
		case ELM_STEREO_OPENGL:
			ofDrawBitmapString("GL_STEREO",ofGetWidth()-100,ofGetHeight()-100);
			break;
			
		case ELM_STEREO_MONO:
			ofDrawBitmapString("MONO",ofGetWidth()-100,ofGetHeight()-100);
			break;
		case ELM_STEREO_ANAGLYPH:
			ofDrawBitmapString("ANAGLYPH",ofGetWidth()-100,ofGetHeight()-100);
			break;
		default:
			break;
	}
    ofPopStyle();
    
}

//-----------------------------------------------------------------
void elementMixer::drawQuadGeometry()
{
	blacktexture.draw(0,0,elementWidth,elementHeight);
}

//-----------------------------------------------------------------
void elementMixer::setOutputStereoMode(int _stereoMode)
{
	outputStereoMode = _stereoMode;
//	if((outputStereoMode == ELM_STEREO_ANAGLYPH) || (outputStereoMode == ELM_STEREO_MONO))
//		setIsStereo(false);
//	else
//		setIsStereo(true);
}

//-----------------------------------------------------------------
void elementMixer::updateOpacity()
{	
}

//--------------------------------------------------------------
void elementMixer::guiEvent(ofxUIEventArgs &e)
{	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
    for(int a = 0; a < 4; a++)
    {
        if(e.widget->getName()=="Test Pattern")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            
            if(a == 0)
            {
                if (toggle->getValue())
                { 
                    sceneElements[a]->setOpacity(1);
                    sceneElements[a]->setIsShow(true);
                    
                }
                else 
                {
                    sceneElements[a]->setOpacity(0);
                    sceneElements[a]->setIsShow(false);
                }
                sceneElements[a]->UI->setVisible(false);
            }
            else
            {
                if (toggle->getValue())
                { 
                    sceneElements[a]->setOpacity(0);
                    sceneElements[a]->setIsShow(false);
                    
                }
                else 
                {
                    sceneElements[a]->setOpacity(1);
                    sceneElements[a]->setIsShow(true);
                }
                sceneElements[a]->UI->setVisible(true);   
            }
        } 
    }
    
    if(e.widget->getName()=="Play")
	{
        ((testApp*)ofGetAppPtr())->elemV1.leftChannelPlayer.play();
        ((testApp*)ofGetAppPtr())->elemV1.rightChannelPlayer.play();

	}


    if(e.widget->getName()=="Pause")
	{
        ((testApp*)ofGetAppPtr())->elemV1.leftChannelPlayer.stop();
        ((testApp*)ofGetAppPtr())->elemV1.rightChannelPlayer.stop(); 
	}
    
    if(e.widget->getName()=="Rew")
	{
        ((testApp*)ofGetAppPtr())->elemV1.leftChannelPlayer.play();
        ((testApp*)ofGetAppPtr())->elemV1.rightChannelPlayer.play();
        ((testApp*)ofGetAppPtr())->elemV1.leftChannelPlayer.setPosition(0.0);
        ((testApp*)ofGetAppPtr())->elemV1.rightChannelPlayer.setPosition(0.0);
        ((testApp*)ofGetAppPtr())->elemV1.leftChannelPlayer.stop();
        ((testApp*)ofGetAppPtr())->elemV1.rightChannelPlayer.stop(); 
        
	}

    
    elementUIBase::guiEvent(e);
}

