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

	this->init(5,_width,_height,GL_RGBA,_name,1, _isWarpable);
	    
    fboGui.allocate(_width ,_height, GL_RGBA);
        
    sceneElements = _elements;
	numOfElements = _numOfElements;
	elementsOrder = _elementsOrder;
    
    ofAddListener(UI->newGUIEvent,this,&elementMixer::guiEvent); 
    
    actualVolume = 1;
	
    wideScreenPreview=false;
    showGrid=true;
    
    parallax=0;
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

void elementMixer::drawIntoFbo()
{
    
    //clear dall'fbo in cui disegno griglie e controlli warp
    fboGui.begin();
    ofClear(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboGui.end();

    fboLeft.begin();
    ofClear(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboLeft.end();


    fboRight.begin();
    ofClear(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboRight.end();

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
                
                ofPushMatrix();
                if (sceneElements[elementsOrder[a]]->getDrawInStereo()==true && getDrawInStereo() && getOutputMode()!=ELM_MONO) ofTranslate(parallax, 0);

                sceneElements[elementsOrder[a]]->drawLeft(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
                ofDisableBlendMode();
                                
                ofPopMatrix();

                //NOTA: gli "special" sono diseganti in un fbo diverso, per essere fuori dagli stili (ad esempio non vengono toccati dalle maschere per l'anaglifo)
                fboGui.begin();
                
                ofPushStyle();
                ofSetColor(255, 255, 255, 255);

                //se è selezionato disegna l'outline rosso, e i markers se il warp è attivo
                if(sceneElements[elementsOrder[a]]->isSelected) 
                {
                    //outline se selezionato 
                    sceneElements[elementsOrder[a]]->warper.drawElementOutline();
                    
                    //marker per il quad warping
                    if (sceneElements[elementsOrder[a]]->warper.bWarpActive) sceneElements[elementsOrder[a]]->warper.drawMarkers();
                    //griglia per il mesh warping
                    if (sceneElements[elementsOrder[a]]->warper.bViewGrid) sceneElements[elementsOrder[a]]->warper.drawGrid();
                }
                
                ofPopStyle();

                fboGui.end();
                
                ofPopStyle();
            }
		

		fboLeft.end();
		
        //////////////////////////
		// RIGHT FRAME:
		//////////////////////////
        
//    if (outputMode!=ELM_MONO)
//    {
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
                        
                        
                        //disegna l'uscita sinistra se ho scelto mono per l'element
                        if (sceneElements[elementsOrder[a]]->getElementInputType()==ELM_INPUT_MONO)
                            {
                                sceneElements[elementsOrder[a]]->drawLeft(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());                                
                            }
                            else //oppure disegna la destra se sono in stereo sia l'element che il mixer
                            {
                                ofPushMatrix();
                                if (sceneElements[elementsOrder[a]]->getDrawInStereo()==true && getDrawInStereo() && getOutputMode()!=ELM_MONO) ofTranslate(-parallax, 0);
                                sceneElements[elementsOrder[a]]->drawRight(0,0,sceneElements[elementsOrder[a]]->getWidth(),sceneElements[elementsOrder[a]]->getHeight());
                                ofPopMatrix();
                            }
                        ofDisableBlendMode();                        
                        ofPopStyle();
                    }
                    fboRight.end();
//    }
    
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
                ofPushStyle();
                fboGui.draw(_x,_y,_width,_height);
                ofPopStyle();
                break;
                
            case ELM_STEREO_ANAGLYPH:
                ofPushStyle();
                glColorMask(true, false, false, true);
                if (getSwapLeftRight()) fboRight.draw(_x,_y,_width,_height);
                else fboLeft.draw(_x,_y,_width,_height);

                glColorMask(false, true, true, true);
                if (getDrawInStereo()) 
                {
                if (getSwapLeftRight()) fboLeft.draw(_x,_y,_width,_height);
                else fboRight.draw(_x,_y,_width,_height);
                }
                else                 
                {
                    if (getSwapLeftRight()) fboRight.draw(_x,_y,_width,_height);
                    else fboLeft.draw(_x,_y,_width,_height);
                }
                glColorMask(true, true, true, true);
                ofPopStyle();
                
                ofPushStyle();
                fboGui.draw(_x,_y,_width,_height);
                ofPopStyle();
				break;

            case ELM_STEREO_OPENGL:
                glDrawBuffer(GL_BACK_LEFT);
                if (getSwapLeftRight()) fboRight.draw(_x,_y,_width,_height);
                    else fboLeft.draw(_x,_y,_width,_height);
                fboGui.draw(_x,_y,_width,_height);

                glDrawBuffer(GL_BACK_RIGHT);
                if (getSwapLeftRight()) fboLeft.draw(_x,_y,_width,_height);
                    else fboRight.draw(_x,_y,_width,_height);
                fboGui.draw(_x,_y,_width,_height);
                break;

            case ELM_STEREO_LEFTRIGHT:
                ofPushStyle();
                if (getSwapLeftRight()) {
                    fboRight.draw(_x,_y,_width*.5,_height);
                    fboLeft.draw(_x+_width*.5,_y,_width*.5,_height);
                } else {
                    fboLeft.draw(_x,_y,_width*.5,_height);
                    fboRight.draw(_x+_width*.5,_y,_width*.5,_height);
                }
                ofPopStyle();

                ofPushStyle();
                fboGui.draw(_x,_y,_width*.5,_height);
                ofPopStyle();

                break;

            case ELM_STEREO_TOPBOTTOM:
                ofPushStyle();
                if (getSwapLeftRight()) {
                    fboRight.draw(_x,_y,_width,_height*.5);
                    fboLeft.draw(_x,_y+_height*.5,_width,_height*.5);
                } else {
                    fboLeft.draw(_x,_y,_width,_height*.5);
                    fboRight.draw(_x,_y+_height*.5,_width,_height*.5);
                }
                ofPopStyle();

                ofPushStyle();
                fboGui.draw(_x,_y,_width,_height*.5);
                ofPopStyle();

                break;

			default:
				break;
		}   
		
    ofPopStyle();
    
}



//-----------------------------------------------------------------
void elementMixer::drawInfo()
{
    ofPushStyle();
  
    //output mode
	ofSetColor(255,0,206);
    switch (outputMode) 
	{
		case ELM_STEREO_OPENGL:
            fontMedium.drawString("STEREO OPENGL",1100,24);
			break;
			
		case ELM_MONO:
            if (getSwapLeftRight()) fontMedium.drawString("SINGLE RIGHT CHANNEL",1100,24);
			else fontMedium.drawString("SINGLE LEFT CHANNEL",1100,24);
			break;
            
		case ELM_STEREO_ANAGLYPH:
			fontMedium.drawString("STEREO ANAGLYPH",1100,24);
			break;
		
		case ELM_STEREO_LEFTRIGHT:
			fontMedium.drawString("STEREO LEFTRIGHT",1100,24);
			break;

        default:
			break;
	}
    
    //framerate
	ofSetColor(255,255,255);
    fontMedium.drawString("FPS: ",1100,54);
    fontMedium.drawString(ofToString(ofGetFrameRate()),1135,54);
    
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
    
        //STEREOSCOPIC
        if(e.widget->getName()=="MAIN STEREO")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            setDrawInStereo(toggle->getValue());
        }
        //SWAP LEFT RIGHT
        if(e.widget->getName()=="SWAP LEFT RIGHT")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            setSwapLeftRight(toggle->getValue());
        }
        //PARALLAX
        if(e.widget->getName()=="ZERO PARALLAX")
        {
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            parallax=(slider->getScaledValue());
            cout << parallax << endl;
        }
        //VIEW GRID
        if(e.widget->getName()=="GRID")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            showGrid=!showGrid;
        }
        //WIDESCREEN PREVIEW
//        if(e.widget->getName()=="PREVIEW 16:9")
//        {
//            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
//            wideScreenPreview=!wideScreenPreview;
//            
//        }
        //MAIN OPACITY
        if(e.widget->getName()=="MAIN OPACITY")
        {
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            setOpacity(slider->getScaledValue());
        }
        //TEST PATTERN
        if(e.widget->getName()=="TEST PATTERN")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            sceneElements[0]->setIsActive(toggle->getValue());
        }
        //SAVE PROJECT
        if(e.widget->getName()=="SAVE PROJECT")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            if (button->getValue()) {

            for (int i=1; i<numOfElements; i++)
            {
                sceneElements[i]->warper.save();
                sceneElements[i]->saveSettings();
            }
                ofSystemAlertDialog("Project Saved");
            }
            
//            UI->saveSettings("./XML/MixerGUI.xml");
        }
        //OPEN PROJECT
        if(e.widget->getName()=="OPEN PROJECT")
        {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            if (button->getValue()) {

//            UI->loadSettings("./XML/MixerGUI.xml");
            
            for (int i=1; i<numOfElements; i++)
            {
                sceneElements[i]->warper.load();
                sceneElements[i]->loadSettings();
                sceneElements[i]->elementUIBase::aggiornaGUI();
            }
            ofSystemAlertDialog("Project Loaded");
            }
            
        }
        //OUTPUT MODE
        if( e.widget->getParent()->getName()=="Output Mode")
        {
            if(name=="ANAGLYPH") setOutputMode(ELM_STEREO_ANAGLYPH);
            else if(name=="MONO") setOutputMode(ELM_MONO);
            else if(name=="OPEN GL") {
                if (mainWindow->bGLStereoCapable) setOutputMode(ELM_STEREO_OPENGL);   
                else ofSystemAlertDialog("GL STEREO MODE NOT SUPPORTED ON THIS MACHINE!");
            }
            else if(name=="LEFT-RIGHT") setOutputMode(ELM_STEREO_LEFTRIGHT);
            else if(name=="TOP-BOTTOM") setOutputMode(ELM_STEREO_TOPBOTTOM);
        }

        // ++++ VIDEO PLAYER ++++ //
        
        //NAVIGATOR
        if(e.widget->getName()=="VIDEO NAVIGATOR")
        {
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            int _frame = int (slider->getValue()*mainWindow->elemV1.leftChannelPlayer.getTotalNumFrames());
            mainWindow->elemV1.element_goToFrame(_frame);
        }
        //REW
    if(e.widget->getName()=="REW 2x")   {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) mainWindow->elemV1.element_videoPlay(-2);
    }
        //PLAY
    if(e.widget->getName()=="PLAY")  {
            ofxUIButton *button = (ofxUIButton *) e.widget;
    if (button->getValue()) mainWindow->elemV1.element_videoPlay(1);
    }
    
        //FFW
    if(e.widget->getName()=="FFW 2x")  {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) mainWindow->elemV1.element_videoPlay(2);
    }
    
        //PREV FRAME
    if(e.widget->getName()=="PREV")  {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) mainWindow->elemV1.element_frameIndietro();
    }
    
        //NEXT FRAME
    if(e.widget->getName()=="NEXT")  {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) mainWindow->elemV1.element_frameAvanti();
    }
    
        //PAUSE
    if(e.widget->getName()=="PAUSE")  {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) mainWindow->elemV1.element_videoPause();
    }
    
        //STOP
    if(e.widget->getName()=="STOP")  {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) mainWindow->elemV1.element_videoStop();
    }
        
        //LOOP TOGGLE
        if(e.widget->getName()=="LOOP")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            if(toggle->getValue() == 0) mainWindow->elemV1.element_setLoop(OF_LOOP_NONE);
            else mainWindow->elemV1.element_setLoop(OF_LOOP_NORMAL);
        }
        //MUTE
        if( name == "MUTE")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            mainWindow->elemV1.element_toggleSound();
        }
        //VOLUME
        if( name == "SOUND VOLUME")
        {
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            actualVolume = slider->getValue();
            mainWindow->elemV1.element_soundVolume(actualVolume);
        }
    
        elementUIBase::guiEvent(e);
}


