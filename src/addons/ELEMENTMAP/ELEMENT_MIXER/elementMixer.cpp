#include "elementMixer.h"
#include "elementVideo.h"

#include "testApp.h"
#include "MainWindow.h"

//-----------------------------------------------------------------
void elementMixer::setup(MainWindow* _mainWindow, int _width, int _height, int _outputMode,element** _elements,int _numOfElements,int* _elementsOrder,int _posX, int _posY,string _name, bool _isWarpable)
{
    mainWindow = _mainWindow;
	setOutputMode(_outputMode);

    this->setDrawInStereo(true);
    this->setIsStereo(true);
		
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
                        
                        //disegna l'uscita destra se sono in stereo sia l'element che il mixer
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
    //opacità generale
    ofPushStyle();
    ofSetColor(255, 255, 255, ofMap(getOpacity(), 0, 1, 0, 255));
    
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
		
    ofPopStyle();
    drawInfo();

}



//-----------------------------------------------------------------
void elementMixer::drawInfo()
{
    ofPushStyle();
  
    
    //output mode
	ofSetColor(0,255,206);
    switch (outputMode) 
	{
		case ELM_STEREO_OPENGL:
			ofDrawBitmapString("OPENGL",ofGetWidth()-200,ofGetHeight()-145);
			break;
			
		case ELM_MONO:
			ofDrawBitmapString("MONO",ofGetWidth()-200,ofGetHeight()-145);
			break;
            
		case ELM_STEREO_ANAGLYPH:
			ofDrawBitmapString("ANAGLYPH",ofGetWidth()-200,ofGetHeight()-145);
			break;
		
        default:
			break;
	}
    
    //framerate
	ofSetColor(255,255,255);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),ofGetWidth()-200,ofGetHeight()-130);
    
    
    
    //glSterreo capability
    ofSetColor(0,255,206);
    if ( ((testApp*)ofGetAppPtr())->isGLCapable==GL_TRUE) ofDrawBitmapString("GL_STEREO \nSUPPORTED",ofGetWidth()-200,ofGetHeight()-115);
    else ofDrawBitmapString("GL_STEREO \nNOT SUPPORTED",ofGetWidth()-200,ofGetHeight()-115);
    
    ofPopStyle();
    
    
    
}


//-----------------------------------------------------------------
void elementMixer::setOutputMode(int _mode)
{
	outputMode = _mode;
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
	
        if(e.widget->getName()=="Main Opacity")
        {
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            setOpacity(slider->getScaledValue());
        }
        
        if(e.widget->getName()=="Test Pattern")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            sceneElements[0]->setIsActive(toggle->getValue());
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
    
    if(e.widget->getName()=="Opacity")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setOpacity(slider->getScaledValue());
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
    
    elementUIBase::guiEvent(e);
}


