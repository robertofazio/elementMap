#include "elementMixer.h"
#include "elementVideo.h"

#include "testApp.h"
#include "MainWindow.h"

// TODO ::
// add support for alpha values on the PSmode mixer

// find a solution for the drawQuadGeometry . 
// now it's working because we're working an 
// image and we're discarting the tex0 uniform in shader.


//-----------------------------------------------------------------
void elementMixer::setup(MainWindow* _mainWindow, int _width, int _height, int _stereoMode,element** _elements,int _numOfElements,int* _elementsOrder,int _posX, int _posY,string _name, bool _isWarpable)
{
    mainWindow = _mainWindow;
	setOutputStereoMode(_stereoMode);
	if(outputStereoMode==0)
	{	
		this->setDrawInStereo(true);
		this->setIsStereo(true);
		
	}
	// UI params
	xPos = _posX;
	yPos = _posY;

	this->init(5,_width,_height,GL_RGBA,_name,this->getIsStereo(), _isWarpable);
	
    fboAnagliph.allocate(_width ,_height, GL_RGBA);
        
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
    
    actualVolume = 50;
	
}


//-----------------------------------------------------------------
void elementMixer::update()
{
	for(int i=0;i<numOfElements;i++)
	{
		sceneElements[i]->update();
	}
    
    if (isWarpable) warper.updateCoordinates();
}

//-----------------------------------------------------------------
void elementMixer::applyFX()
{
 for(int a = 0; a < effects.size(); a++)
    {
        if(effects[a]->getIsActive())
            effects[a]->applyFX();
    }
}


//-----------------------------------------------------------------
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
                ofPushStyle();
                ofSetColor(255, 255, 255, opacity);
                sceneElements[elementsOrder[a]]->drawLeft(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
                if(sceneElements[elementsOrder[a]]->isSelected) sceneElements[elementsOrder[a]]->warper.drawElementOutline();
                ofPopStyle();
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
                            ofPushStyle();

                            if(sceneElements[elementsOrder[a]]->getIsStereo())
                                sceneElements[elementsOrder[a]]->drawRight(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
                            else
                                sceneElements[elementsOrder[a]]->drawLeft(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
                            
                            ofPopStyle();

                                }

                fboRight.end();
            }
	}
    
    if(outputStereoMode == ELM_STEREO_ANAGLYPH)
    {
        fboAnagliph.begin();
        //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // left
        glColorMask(true, false, false, false);
        ofSetColor(255, 255, 255);
        fboLeft.draw(0,0);
        
        
        // right
        glColorMask(false, true, true, false);
        ofSetColor(255, 255, 255);
        fboRight.draw(0, 0);
        
        glColorMask(true, true, true, true);
        
        fboAnagliph.end();
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
                ofSetColor(255, 255, 255, ofMap(this->getOpacity(), 0, 1, 0, 255));
                fboAnagliph.draw(_x, _y, _width, _height);
				break;
			default:
				break;
		}   
		drawInfo();
	}
}



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
	
    if((outputStereoMode == ELM_STEREO_ANAGLYPH) || (outputStereoMode == ELM_STEREO_MONO))
		setIsStereo(false);
	else
		setIsStereo(true);
}


//-----------------------------------------------------------------
int elementMixer::getOutputStereoMode()
{
    return outputStereoMode;
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
        mainWindow->elemV1.leftChannelPlayer.play();
        mainWindow->elemV1.rightChannelPlayer.play();

	}


    if(e.widget->getName()=="Pause")
	{
        mainWindow->elemV1.leftChannelPlayer.stop();
        mainWindow->elemV1.rightChannelPlayer.stop();
	}
    
    if(e.widget->getName()=="Rew")
	{
        mainWindow->elemV1.leftChannelPlayer.play();
        mainWindow->elemV1.rightChannelPlayer.play();
        
        
        mainWindow->elemV1.leftChannelPlayer.setPosition(0.0);
        mainWindow->elemV1.rightChannelPlayer.setPosition(0.0);
        
        mainWindow->elemV1.leftChannelPlayer.stop();
        mainWindow->elemV1.rightChannelPlayer.stop();
	}
    
    if(e.widget->getName()=="Video loop")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if(toggle->getValue() == 0)
            mainWindow->elemV1.leftChannelPlayer.setLoopState(OF_LOOP_NONE);
        else
            mainWindow->elemV1.leftChannelPlayer.setLoopState(OF_LOOP_NORMAL);
	}
    else if( e.widget->getParent()->getName()=="Resolution")
	{
        ofxUIDropDownList* resolutions = (ofxUIDropDownList* )e.widget;
        
        for(int i=0;i<resolutionName.size();i++)
		{
			if(name==resolutionName[i]) 
			{
                if(i == 0)   
                    for(int a = 0; a < 4; a++)
                        sceneElements[a]->resetOutput(1024, 768);
                else if(i == 1)
                    for(int a = 0; a < 4; a++)
                        sceneElements[a]->resetOutput(1280, 1024);
                else if(i == 2)   
                    for(int a = 0; a < 4; a++)
                        sceneElements[a]->resetOutput(1920, 1080);
			}			
		}
        
	}
    else if( name == "Sound on/off")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if(toggle->getValue() == 0)
           mainWindow->elemV1.leftChannelPlayer.setVolume(0);
        else 
            mainWindow->elemV1.leftChannelPlayer.setVolume(actualVolume);
    }
    else if( name == "Sound Volume")
	{
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        actualVolume = slider->getValue() * 1000;
        cout << slider->getScaledValue() << endl;
        cout << actualVolume << endl;
        mainWindow->elemV1.leftChannelPlayer.setVolume(actualVolume);
        mainWindow->elemV1.rightChannelPlayer.setVolume(actualVolume);
    }
    
    
    
    /* 
     
     UI->addWidget(new ofxUIToggle(marginLeft + 150, posY+=30, 10, 10, parentElement->getIsActive(),"Sound on/off"));
     UI->addWidget(new ofxUISlider(marginLeft + 150, posY+=20, 100,10,0.0,1.0,parentElement->getOpacity() ,"Sound Volume"));
     */

    /*
     enum ofLoopType{
     OF_LOOP_NONE=0x01,
     OF_LOOP_PALINDROME=0x02,
     OF_LOOP_NORMAL=0x03
     };

     */
    elementUIBase::guiEvent(e);
}

