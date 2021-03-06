#include "MainWindow.h"

float lastTime=0.0f;
float medianFrameStep;
int num=0;

int margin = 10;

#define LEFT_MARGIN_X       10
#define UPPER_MARGIN_Y      82
#define STRIP_HEIGHT        190

#define PREVIEW_WIDTH       100
#define PREVIEW_HEIGHT      75

//--------------------------------------------------------------
void MainWindow::setup()
{	
    
	ofEnableAlphaBlending();
    ofEnableSmoothing();
    
	numOfElements = 4;
	
	int previewWidth = (ofGetWidth()-(20*numOfElements))/4;
	int previewHeight = previewWidth / (float(ofGetWidth())/float(ofGetHeight()));
    
	// create & setup elements on this app 
	
    //pattern image
    elemImg.setup("./utils/testPattern1024.jpg", "", outputResolutionX,outputResolutionY, ELM_INPUT_MONO, -50000 , -50000 ,"TestPattern", false);
    elemImg.UI->toggleVisible();
    
    
    //syphon
    elemSy.setup("","",outputResolutionX,outputResolutionY, LEFT_MARGIN_X , UPPER_MARGIN_Y,"Syphon", true);
	
//    //video
//    elemV1.setup("./movies/alpha_test.mov","", outputResolutionX,outputResolutionY, ELM_INPUT_STEREO_TWO_CHANNEL, LEFT_MARGIN_X , UPPER_MARGIN_Y+STRIP_HEIGHT,"Movie", true);
//
//    //image
//    elemImg2.setup("./images/leftRightHD.jpg", "", outputResolutionX,outputResolutionY, ELM_INPUT_STEREO_LEFTRIGHT, LEFT_MARGIN_X , UPPER_MARGIN_Y+STRIP_HEIGHT*2,"Image", true);
//	

    //video
    elemV1.setup("./utils/empty.mov","./utils/empty.mov", outputResolutionX,outputResolutionY, ELM_INPUT_STEREO_TWO_CHANNEL, LEFT_MARGIN_X , UPPER_MARGIN_Y+STRIP_HEIGHT,"Movie", true);
    
    //image
    elemImg2.setup("./utils/empty.jpg", "./utils/empty.jpg", outputResolutionX,outputResolutionY, ELM_INPUT_STEREO_TWO_CHANNEL, LEFT_MARGIN_X , UPPER_MARGIN_Y+STRIP_HEIGHT*2,"Image", true);
	

    //video starts at frame number 0, paused and no-looping
    elemV1.leftChannelPlayer.play();
    elemV1.rightChannelPlayer.play();
    elemV1.leftChannelPlayer.firstFrame();
    elemV1.rightChannelPlayer.firstFrame();    
    elemV1.leftChannelPlayer.stop();
    elemV1.rightChannelPlayer.stop();
    elemV1.leftChannelPlayer.setLoopState(OF_LOOP_NONE);
    elemV1.rightChannelPlayer.setLoopState(OF_LOOP_NONE);
    if (elemV1.rightChannelPath=="") {
        elemV1.rightChannelPlayer.closeMovie();
    }
    else {
    elemV1.rightChannelPlayer.play();
    elemV1.rightChannelPlayer.firstFrame();    
    elemV1.rightChannelPlayer.stop();
    elemV1.rightChannelPlayer.setLoopState(OF_LOOP_NONE);
    }
    elemImg.UI->toggleVisible();
    
	// add elements to the vector
	myElements = new element*[4];
	myElements[0] = &elemImg;
	myElements[1] = &elemV1;
	myElements[2] = &elemImg2;
	myElements[3] = &elemSy;
    
	// setup mix stuff
	drawingOrder = new int[numOfElements];
	drawingOrder[0]=2;  
	drawingOrder[1]=1;  
	drawingOrder[2]=3;
	drawingOrder[3]=0;
	    
	elemMix.setup(this, outputResolutionX,outputResolutionY,ELM_MONO,myElements,numOfElements,drawingOrder, 650, (margin * 9) - 7,"mixer", false);
    elemMix.wideScreenPreview=outputRatioWide;
    
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    //comunico il puntatore al mixer a tutti gli element
    for (int i=1; i<numOfElements; i++)
    {
        myElements[i]->elementUIBase::allElementsPointer=myElements;
    }

    
    bFullscreen=false;
    bRisparmio=false;
    
    comandi ="element.map v0.3.2 alpha\n\n'w'\t\tactivate/deactivate warp\n't'\t\tactivate/deactivate translate\n\n'z'/'x'\tincrease/decrease grid X resolution\n'q'/'a'\tincrease/decrease grid Y resolution\n'n'/'m'\tselect previous/next point\n'v'\t\tselect quad vertex\n'h'\t\thold to select multiple grid points\n'c'\t\tclear quad warp transformation\n'r'\t\treset point position\n'cmd'+'r'\treset all grid points\n\n'g'\t\tshow/hide mesh grid\n's'\t\tsave warp to xml\n'l'\t\tload warp from xml\n\n'return'\tin main window hide/show GUI\n'f'\t\tin second window change fullscreen mode\n\nSPACEBAR\tplay/pause video\nBACKSPACE\trewind video\n','/'.'\t\t\tframe by frame\n\n**********************************\n\nin output window press:\n\n'1'\tto select syphon layer\n'2'\tto select video layer\n'3'\tto select image layer\n'0'\tto deselect all";
    
//    fontSmall.loadFont("CODEBold.otf", 7);
//    fontMedium.loadFont("CODEBold.otf", 10);
//    fontLarge.loadFont("CODEBold.otf", 18);
    fontSmall.loadFont("NEOSANS.otf", 7);
    fontMedium.loadFont("NEOSANS.otf", 10);
    fontLarge.loadFont("NEOSANS.otf", 18);
    
	fontSmall.setLineHeight(12.0f);
	fontSmall.setLetterSpacing(1.017);
    
	fontLarge.setLineHeight(12.0f);
	fontLarge.setLetterSpacing(1.017);

	fontMedium.setLineHeight(12.0f);
	fontMedium.setLetterSpacing(1.017);
    
    logo.loadImage("./utils/elementMapTrasp.png");
    previewGrid.loadImage("./utils/previewGrid.png");
    sfondo.loadImage("./utils/sfondoELMT02.jpg");

    //di default il test pattern non è visibile:
    elemImg.setIsActive(false);    

    //auto-load last saved data on startup
//    elemMix.UI->loadSettings("./XML/MixerGUI.xml");
    for (int i=1; i<numOfElements; i++)
    {
        myElements[i]->warper.load();
        myElements[i]->loadSettings();
        myElements[i]->elementUIBase::aggiornaGUI();
    }
    
    
    //setup minimal interface
    minimalGUI = new ofxUICanvas(10, 80, 380, 100);
    minimalGUI->setDrawOutline(false);
    minimalGUI->setDrawBack(false);
    
    minimalGUI->addWidget(new ofxUILabel(10, 10, "test", OFX_UI_FONT_MEDIUM));

    minimalGUI->setVisible(true);
//    minimalGUI->disable();
    ofAddListener(minimalGUI->newGUIEvent, this, &MainWindow::guiEvent);

}

//--------------------------------------------------------------
void MainWindow::update()
{
    //disegno negli fbo
	elemMix.update();  
    elemMix.drawIntoFbo();
    
    //aggiorno i valori sull'interfaccia
    for (int i=1; i<numOfElements; i++)
    {
        myElements[i]->elementUIBase::aggiornaGUI();
    }
    //aggiorna la barra di scorrimento del video
    elemMix.elementUIBase::GUI_videoNavigator->setValue(elemV1.leftChannelPlayer.getPosition());
}


//--------------------------------------------------------------
void MainWindow::draw()

{	
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ofBackground(35, 31, 32);
    sfondo.draw(0, 0, 1280, 800);
        
    if (bRisparmio) {
        ofPoint position(ofGetWindowWidth() - 100, ofGetWindowHeight() - 10);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), position);
        ofPushStyle();
        ofSetColor(ofColor :: white);
        fontLarge.drawString("MINIMAL INTERFACE", 60, 40);
        ofPopStyle();
    }
    
    else 
        
    {
        //preview window (non-fulscreen)
        ofPushStyle();
        ofSetColor(255, 255, 255);
        
        fontLarge.drawString("element.Map v0.3.2 ", 70 , 44);
        ofSetColor(36, 203, 228);
        ofLine(650, margin * 6, ofGetWindowWidth() - 10, margin * 6);
        ofPopStyle();

        
        //preview area: nero di sfondo, griglia se selezionata
        if (elemMix.wideScreenPreview) 
        {
            ofPushStyle();
            ofSetColor(ofColor :: black);
            ofRect(650, margin*8+56, 600, 338);
            ofPopStyle();
            if (elemMix.showGrid) previewGrid.draw(650, margin*8+56, 600, 338);
            elemMix.drawOutput(650, (margin * 8)+56, 600, 338   );
        }
        else 
        {
            ofPushStyle();
            ofSetColor(ofColor :: black);
            ofRect(650, margin*8, 600, 450);
            ofPopStyle();
            if (elemMix.showGrid) previewGrid.draw(650, margin*8, 600, 450);
            elemMix.drawOutput(650, margin * 8, 600, 450   );
        }

        glDrawBuffer(GL_BACK);
        
        int yInfo = 75;
        
        //testo in alto nella preview:
        if (elemSy.isSelected==true)
        {
            if (elemSy.warper.bWarpActive) fontMedium.drawString("SYHPON ELEMENT SELECTED - QUAD WARP", 660,yInfo);
            else if (elemSy.warper.bViewGrid) fontMedium.drawString("SYHPON ELEMENT SELECTED - GRID WARP", 660,yInfo);
            else fontMedium.drawString("SYHPON ELEMENT SELECTED", 660,yInfo);
        }
        else if (elemV1.isSelected==true) 
        {
            if (elemV1.warper.bWarpActive) fontMedium.drawString("VIDEO ELEMENT SELECTED - QUAD WARP", 660,yInfo);
            else if (elemV1.warper.bViewGrid) fontMedium.drawString("VIDEO ELEMENT SELECTED - GRID WARP", 660,yInfo);
            else fontMedium.drawString("VIDEO ELEMENT SELECTED", 660,yInfo);
        }
        else if (elemImg2.isSelected==true) 
        {
            if (elemImg2.warper.bWarpActive) fontMedium.drawString("IMAGE ELEMENT SELECTED - QUAD WARP", 660,yInfo);
            else if (elemImg2.warper.bViewGrid) fontMedium.drawString("IMAGE ELEMENT SELECTED - GRID WARP", 660,yInfo);
            else fontMedium.drawString("IMAGE ELEMENT SELECTED", 660,yInfo);
        }

        else fontMedium.drawString("NO ELEMENTs SELECTED", 660,yInfo);

        if (elemMix.getIsStereo()) {
            string swapInfo = "first frame: ";
            if (elemMix.getSwapLeftRight()) swapInfo+="RIGHT";
            else swapInfo+="LEFT";
            ofPushStyle();
            ofSetColor(36, 203, 228);
            fontMedium.drawString(swapInfo, 1100, 39);
            ofPopStyle();
        }
        
        elemMix.UI->draw();
        
        ofPushStyle();
        ofSetColor(255, 255, 255);
        fontMedium.drawString("Press 'i' for info", 10 , ofGetWindowHeight() - 10);
        
        // DYSPLAY STEREOSCOPIC VIDEO SYNCHRONIZE
        ofSetColor(255, 255, 255);
        if (elemV1.inputType==ELM_INPUT_STEREO_TWO_CHANNEL) {
            fontSmall.drawString("FRAME LEFT: "+ofToString(elemV1.leftChannelPlayer.getCurrentFrame()), 655, 36);
            fontSmall.drawString("FRAME RIGHT: "+ofToString(elemV1.rightChannelPlayer.getCurrentFrame()), 655, 48);
        
        if (elemV1.leftChannelPlayer.getCurrentFrame() == elemV1.rightChannelPlayer.getCurrentFrame()) {
            ofPushStyle();
            ofSetColor(255, 206, 0);
            fontMedium.drawString("STEROSCOPIC SYNC", 655,24);
            ofPopStyle();
        }
        else {
            ofPushStyle();
            ofSetHexColor(0xFF0000);
            fontMedium.drawString("STEROSCOPIC NOT IN SYNC", 655, 24);
            ofPopStyle();
        }
        } else {
            ofPushStyle();
            ofSetColor(255, 206, 0);
            fontMedium.drawString("SINGLE VIDEO SOURCE", 655,24);
            ofSetColor(255, 255, 255);
            fontSmall.drawString("CURRENT FRAME: "+ofToString(elemV1.leftChannelPlayer.getCurrentFrame()), 655, 36);
            ofPopStyle();
        }
        
        elemMix.drawInfo();

        ofPopStyle();
        
            // just draw the preview inputs of mixer
            ofPushStyle();
            ofSetColor(255,255);
            glDrawBuffer(GL_BACK);
            
            for(int i = 0;i<numOfElements - 1;i++)
            {
                ofSetColor(36, 203, 228);
                ofLine(margin, (margin * 8)  + ((i) * 190), margin+630, (margin * 8) + ((i) * 190));
                ofSetColor(255, 255, 255);
                myElements[drawingOrder[i]]->drawGraphic(LEFT_MARGIN_X+5 ,(margin * 7) + ((numOfElements - i -2) * 190), PREVIEW_WIDTH,PREVIEW_HEIGHT);
                myElements[drawingOrder[i]]->UI->draw();
                
                int yGridLabel = 95;
                if(myElements[drawingOrder[i]]->getIsStereo()) yGridLabel+=20;
                
                fontMedium.drawString(ofToString(myElements[drawingOrder[i]]->warper.xRes-1), 235, (margin * 7) -9 + ((numOfElements - i -2) * 190) + yGridLabel);
                fontMedium.drawString(ofToString(myElements[drawingOrder[i]]->warper.yRes-1), 235, (margin * 7) -9+ ((numOfElements - i -2) * 190) + yGridLabel+20);
            }
            ofPopStyle();
        }
                
        logo.draw(10, 9, 45, 45);
    
}



//--------------------------------------------------------------
void MainWindow::keyPressed(int key)
{
    if(key == OF_KEY_RETURN)
	{
        bRisparmio=!bRisparmio;
        
        if (bRisparmio) minimalGUI->enable();
        else minimalGUI->disable();
        minimalGUI->setVisible(bRisparmio);
        
        if(!bRisparmio)
		{	
			myElements[1]->UI->setVisible(true);
			myElements[2]->UI->setVisible(true);
			myElements[3]->UI->setVisible(true);
			elemMix.UI->setVisible(true);
            
            ofxFensterManager::get()->getWindowById(0)->setWindowShape(1280, 800);
            ofxFensterManager::get()->getWindowById(0)->setWindowPosition(0, 0);
		}
		else 
		{
			myElements[1]->UI->setVisible(false);
			myElements[2]->UI->setVisible(false);
			myElements[3]->UI->setVisible(false);
			elemMix.UI->setVisible(false);
            
            ofxFensterManager::get()->getWindowById(0)->setWindowShape(400, 200);
            ofxFensterManager::get()->getWindowById(0)->setWindowPosition(100, 100);
		}        
	}
    else if(key == ' ')
    {
        if (elemV1.leftChannelPlayer.isPlaying()) elemV1.element_videoPause();
        else elemV1.element_videoPlay(1);
    }
    
    else if (key== OF_KEY_BACKSPACE) elemV1.element_videoStop();
    
    else if(key == '.') elemV1.element_frameAvanti();
    
    else if(key == ',') elemV1.element_frameIndietro();        
    
    else if(key == 'i') ofSystemAlertDialog(comandi);
        
    else if(key == '0') {
    deselectAllElements();
    }
    
    else if(key == '1') {
    deselectAllElements();
    elemSy.isSelected=true;
    }
    
    else if(key == '2') {
    deselectAllElements();
    elemV1.isSelected=true;

    }
    
    else if(key == '3') {
    deselectAllElements();
    elemImg2.isSelected=true;
    }
    
    if (elemSy.isSelected==true && elemSy.isWarpable==true) elemSy.warper.warpKeyPressedHandler(key);
    else if (elemV1.isSelected==true && elemV1.isWarpable==true) elemV1.warper.warpKeyPressedHandler(key);
    else if (elemImg2.isSelected==true && elemImg2.isWarpable==true) elemImg2.warper.warpKeyPressedHandler(key);

    
}

//--------------------------------------------------------------
void MainWindow::keyReleased(int key)
{
    
    if (elemSy.isSelected==true && elemSy.isWarpable==true) elemSy.warper.warpKeyReleasedHandler(key);
    else if (elemV1.isSelected==true && elemV1.isWarpable==true) elemV1.warper.warpKeyReleasedHandler(key);
    else if (elemImg2.isSelected==true && elemImg2.isWarpable==true) elemImg2.warper.warpKeyReleasedHandler(key);

}


//--------------------------------------------------------------
void MainWindow::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void MainWindow::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void MainWindow::mousePressed(int x, int y, int button) {    
}

//--------------------------------------------------------------
void MainWindow::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void MainWindow::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void MainWindow::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void MainWindow::dragEvent(ofDragInfo dragInfo){             
}


//--------------------------------------------------------------
void MainWindow::deselectAllElements()
{
    for (int i=1; i<numOfElements; i++)
    {
        myElements[i]->isSelected=true;
        myElements[i]->warper.bWarpActive=false;
        myElements[i]->warper.bViewGrid=false;
        myElements[i]->isSelected=false;
    }
}

//--------------------------------------------------------------
void MainWindow::guiEvent(ofxUIEventArgs &e)
{	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
}
