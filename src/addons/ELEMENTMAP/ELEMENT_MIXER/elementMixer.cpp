#include "elementMixer.h"
#include "elementVideo.h"

#include "testApp.h"
#include "MainWindow.h"

//-----------------------------------------------------------------
void elementMixer::setup(MainWindow* _mainWindow, int _width, int _height, int _outputMode,element** _elements,int _numOfElements,int* _elementsOrder,int _posX, int _posY,string _name, bool _isWarpable)
{
    mainWindow = _mainWindow;
	setOutputMode(_outputMode);
	if(outputMode==0)
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
    
    ofAddListener(UI->newGUIEvent,this,&elementMixer::guiEvent); 
    
    actualVolume = 50;
	
}


//-----------------------------------------------------------------
// mixer::update
// esegue il metodo "update" di tutti gli element inseriti, che per ora si occupa solo di aggiornare le coordinate del warp;
// se l'uscita principale (= mixer output) è warpable, aggiorna anche se stesso

void elementMixer::update()
{
	for(int i=0;i<numOfElements;i++)
	{
		sceneElements[i]->update();
	}
    
    if (isWarpable) warper.updateCoordinates();
}


//-----------------------------------------------------------------
// mixer::drawIntoFbo
// somma tutte le uscite dei singoli element in ordine (secondo i blending modes definiti all'interno di ogni element)
// e le prepara in due fbo nel caso di uscita stereoscopica, o soltanto nel sinistro per uscita mono

void elementMixer::drawIntoFbo(bool _drawMonoOrStereo)
{
		//setDrawInStereo(_drawMonoOrStereo);
		
		//////////////////////////
		// LEFT FRAME:
		//////////////////////////
        
		fboLeft.begin();
        ofClear(0,0,0,0);
        
        for(int a = 0; a < numOfElements; a++)
            if(sceneElements[elementsOrder[a]]->getIsActive())
            {
                ofPushStyle();
                
                //legge l'opacità del livello e la applica
                float opacity = ofMap(sceneElements[elementsOrder[a]]->getOpacity(), 0, 1, 0, 255);
                ofSetColor(255, 255, 255, opacity);
                
                //legge il blending mode associato al livello e lo applica
                ofEnableBlendMode(sceneElements[elementsOrder[a]]->getBlendingMode());
                
                //disegna l'uscita dell'element secondo le sue dimensioni proprie
                sceneElements[elementsOrder[a]]->drawLeft(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
                ofDisableBlendMode();
                
                //se è selezionato disegna l'outline rosso
                if(sceneElements[elementsOrder[a]]->isSelected) sceneElements[elementsOrder[a]]->warper.drawElementOutline();
                
                
                ofPopStyle();
            }
		

		fboLeft.end();
		
        //////////////////////////
		// RIGHT FRAME:
		//////////////////////////
        
    if (outputMode!=ELM_MONO)
    {
                fboRight.begin();
                ofClear(0,0,0,0);
                
                for(int a = 0; a < numOfElements; a++)
                    if(sceneElements[elementsOrder[a]]->getIsActive())
                    {
                        ofPushStyle();
                        
                        //legge l'opacità del livello e la applica
                        float opacity = ofMap(sceneElements[elementsOrder[a]]->getOpacity(), 0, 1, 0, 255);
                        ofSetColor(255, 255, 255, opacity);
                        
                        //legge il blending mode associato al livello e lo applica
                        ofEnableBlendMode(sceneElements[elementsOrder[a]]->getBlendingMode());
                        
                        //disegna l'uscita dell'element secondo le sue dimensioni proprie
                        if (sceneElements[elementsOrder[a]]->getDrawInStereo()==false || getDrawInStereo()==false)
                            {
                                sceneElements[elementsOrder[a]]->drawLeft(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());                                
                            }
                            else sceneElements[elementsOrder[a]]->drawRight(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());

                        ofDisableBlendMode();                        
                        
                        //se è selezionato disegna l'outline rosso
                        if(sceneElements[elementsOrder[a]]->isSelected) sceneElements[elementsOrder[a]]->warper.drawElementOutline();

                        
                        ofPopStyle();
                    }
                
                
                fboRight.end();
    }
	
    //fbo dedicato per l'uscita anaglifo
    //nuova versione 18/08/2012
    if(outputMode == ELM_STEREO_ANAGLYPH)
    {
        fboAnagliph.begin();
    ofPushStyle();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColorMask(true, false, false, true);
    fboRight.draw(0, 0);
    glColorMask(false, true, true, true);
    fboLeft.draw(0,0);
    glColorMask(true, true, true, true);
    ofPopStyle();
        fboAnagliph.end();
    }

}




//-----------------------------------------------------------------
void elementMixer::drawOutput(int _x, int _y,int _width, int _height)
{
		switch (outputMode) 
		{
            
			case ELM_MONO:
                fboLeft.draw(_x,_y,_width,_height);
            break;
                
            case ELM_STEREO_ANAGLYPH:
                fboAnagliph.draw(_x,_y,_width,_height);
				break;

            case ELM_STEREO_OPENGL:
                glDrawBuffer(GL_BACK_LEFT);	
                fboLeft.draw(_x,_y,_width,_height);
                glDrawBuffer(GL_BACK_RIGHT);
                fboRight.draw(_x,_y,_width,_height);
                break;

                
			default:
				break;
		}   
		
    
    drawInfo();

}



//-----------------------------------------------------------------
void elementMixer::drawInfo()
{
    ofPushStyle();
	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),ofGetWidth()-100,ofGetHeight()-80);
    
	ofSetColor(0,255,0);
    
	switch (outputMode) 
	{
		case ELM_STEREO_OPENGL:
			ofDrawBitmapString("OPENGL",ofGetWidth()-100,ofGetHeight()-100);
			break;
			
		case ELM_MONO:
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
void elementMixer::setOutputMode(int _mode)
{
	outputMode = _mode;
    
    if((outputMode == ELM_STEREO_ANAGLYPH) || (outputMode == ELM_MONO))
		setIsStereo(false);
	else
		setIsStereo(true);
}


//-----------------------------------------------------------------
int elementMixer::getOutputMode()
{
    return outputMode;
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
    
    if(e.widget->getName()=="Stereo")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        setDrawInStereo(toggle->getValue());
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
    
    
    else if( e.widget->getParent()->getName()=="Output Mode")
	{
        if(name=="ANAGLYPH") setOutputMode(ELM_STEREO_ANAGLYPH);
        else if(name=="MONO") setOutputMode(ELM_MONO);
        else if(name=="OPENGL") setOutputMode(ELM_STEREO_OPENGL);
        
        
        
	}

    
    
//    else if( e.widget->getParent()->getName()=="Resolution")
//	{
//        ofxUIDropDownList* resolutions = (ofxUIDropDownList* )e.widget;
//        
//        for(int i=0;i<resolutionName.size();i++)
//		{
//			if(name==resolutionName[i]) 
//			{
//                if(i == 0)   
//                    for(int a = 0; a < 4; a++)
//                        sceneElements[a]->resetOutput(1024, 768);
//                else if(i == 1)
//                    for(int a = 0; a < 4; a++)
//                        sceneElements[a]->resetOutput(1280, 1024);
//                else if(i == 2)   
//                    for(int a = 0; a < 4; a++)
//                        sceneElements[a]->resetOutput(1920, 1080);
//			}			
//		}
//        
//	}
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


