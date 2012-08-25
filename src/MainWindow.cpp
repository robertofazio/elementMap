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
    drawPreviews		= true;
	drawUIs				= true;

	ofEnableAlphaBlending();
    ofEnableSmoothing();
    
	numOfElements = 4;
	
	int previewWidth = (ofGetWidth()-(20*numOfElements))/4;
	int previewHeight = previewWidth / (float(ofGetWidth())/float(ofGetHeight()));
    
	// create & setup elements on this app 
	
    //pattern image
    elemImg.setup("./images/testPattern1024.jpg", "", outputResolutionX,outputResolutionY, false, -50000 , -50000 ,"TestPattern", false);
    elemImg.UI->toggleVisible();
    
    
    //syphon
    elemSy.setup("","",outputResolutionX,outputResolutionY, LEFT_MARGIN_X , UPPER_MARGIN_Y,"Syphon", true);
	
    
    //video
	elemV1.setup("./movies/left1024Audio.mov","./movies/right1024.mov", outputResolutionX,outputResolutionY, true, LEFT_MARGIN_X , UPPER_MARGIN_Y+STRIP_HEIGHT,"Movie", true);
	
    //image
    elemImg2.setup("./images/left1024.jpg", "./images/right1024.jpg", outputResolutionX,outputResolutionY, true, LEFT_MARGIN_X , UPPER_MARGIN_Y+STRIP_HEIGHT*2,"Image", true);
	
    
    
    //video starts at frame number 0, paused and no-looping
    elemV1.leftChannelPlayer.play();
    elemV1.rightChannelPlayer.play();
    elemV1.leftChannelPlayer.firstFrame();
    elemV1.rightChannelPlayer.firstFrame();    
    elemV1.leftChannelPlayer.stop();
    elemV1.rightChannelPlayer.stop();
    elemV1.leftChannelPlayer.setLoopState(OF_LOOP_NONE);
    elemV1.rightChannelPlayer.setLoopState(OF_LOOP_NONE);
    
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
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    bFullscreen=false;
    
    comandi ="element.map v0.2.0 alpha\n\n'w'\t\tactivate/deactivate warp\n't'\t\tactivate/deactivate translate\n\n'z'/'x'\tincrease/decrease grid X resolution\n'q'/'a'\tincrease/decrease grid Y resolution\n'n'/'m'\tselect previous/next point\n'v'\t\tselect quad vertex\n'h'\t\thold to select multiple grid points\n'c'\t\tclear quad warp transformation\n'r'\t\treset point position\n'cmd'+'r'\treset all grid points\n\n'g'\t\tshow/hide mesh grid\n's'\t\tsave warp to xml\n'l'\t\tload warp from xml\n\n'return'\tin main window hide/show GUI\n'f'\t\tin second window change fullscreen mode\n\n\nall working with arrow keys;\nquad warping support mouse drag too\n\nSPACEBAR\tplay/pause video\nBACKSPACE\trewind video\n','/'.'\t\t\tframe by frame\n\n**********************************\n\nin output window press:\n\n'1'\tto select syphon layer\n'2'\tto select video layer\n'3'\tto select image layer\n'0'\tto deselect all";
    
    fontSmall.loadFont("CODEBold.otf", 7);
    fontMedium.loadFont("CODEBold.otf", 10);
    fontLarge.loadFont("CODEBold.otf", 18);
    
	fontSmall.setLineHeight(12.0f);
	fontSmall.setLetterSpacing(1.017);
    
	fontLarge.setLineHeight(12.0f);
	fontLarge.setLetterSpacing(1.017);

	fontMedium.setLineHeight(12.0f);
	fontMedium.setLetterSpacing(1.017);
    
    logo.loadImage("./images/logo.png");
    logoChristie.loadImage("./images/logoChristie.png");
    logoDigitalNetwork.loadImage("./images/logoDigitalNetwork.png");
    previewGrid.loadImage("./images/previewGrid.png");

    //di default il test pattern non è visibile:
    elemImg.setIsActive(false);    
    
    for (int i=1; i<numOfElements; i++)
    {
        myElements[i]->warper.load();
        myElements[i]->loadSettings();
    }

}

//--------------------------------------------------------------
void MainWindow::update()
{
	elemMix.update();  
    elemMix.drawIntoFbo(true);
}


//--------------------------------------------------------------
void MainWindow::draw()

{	
    
    ofBackground(35, 31, 32);
        
    if (bFullscreen) {
        ofPoint position(ofGetWindowWidth() - 100, ofGetWindowHeight() - 10);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), position);
    }
    
    else 
        
    {
        //preview window (non-fulscreen)
        ofPushStyle();
        ofSetColor(255, 255, 255);
        
        fontLarge.drawString("element.Map v0.2.1 ", 70 , 44);
        ofSetColor(0, 255, 206);
        ofLine(650, margin * 6, ofGetWindowWidth() - 10, margin * 6);
        ofPopStyle();

        if (elemMix.showGrid) previewGrid.draw(650, margin*8, 600, 450);
        else 
        {
            ofPushStyle();
            ofSetColor(ofColor :: black);
            ofRect(650, margin*8, 600, 450);   
            ofPopStyle();
        }

        if (elemMix.wideScreenPreview) 
        {
            elemMix.drawOutput(650, (margin * 8)+56, 600, 338   );
        }
        else 
        {
            elemMix.drawOutput(650, margin * 8, 600, 450   );
        }

        if (elemSy.isSelected==true) fontMedium.drawString("SYHPON ELEMENT SELECTED", 660,100);
        else if (elemV1.isSelected==true) fontMedium.drawString("VIDEO ELEMENT SELECTED", 660,100);
        else if (elemImg2.isSelected==true) fontMedium.drawString("IMAGE ELEMENT SELECTED", 660,100);
        else fontMedium.drawString("NO ELEMENTs SELECTED", 660,100);

        
        elemMix.UI->draw();
        
        ofPushStyle();
        ofSetColor(255, 255, 255);
        fontMedium.drawString("Press 'i' for info", 10 , ofGetWindowHeight() - 10);
        
        // DYSPLAY STEREOSCOPIC VIDEO SYNCHRONIZE
        
        ofSetColor(255, 255, 255);
        fontSmall.drawString(ofToString(elemV1.leftChannelPlayer.getCurrentFrame()), 735, 737);
        fontSmall.drawString(ofToString(elemV1.rightChannelPlayer.getCurrentFrame()), 735, 749);
        
        if (elemV1.leftChannelPlayer.getCurrentFrame() == elemV1.rightChannelPlayer.getCurrentFrame()) {
            ofSetColor(255, 206, 0);
            fontMedium.drawString("STEROSCOPIC SYNC", 655,765);
            
        }
        
        else {
            ofSetHexColor(0xFF0000);
            fontMedium.drawString("STEROSCOPIC NOT IN SYNC", 655, 765);
            
        }
        
        ofPopStyle();
        
        if(drawPreviews)    
        {
            // just draw the preview inputs of mixer
            ofPushStyle();
            ofSetColor(255,255);
            glDrawBuffer(GL_BACK);
            for(int i = 0;i<numOfElements - 1;i++)
            {
                ofSetColor(0, 255, 206);
                ofLine(margin, (margin * 6) + ((i) * 190), 645, (margin * 6) + ((i) * 190));
                ofSetColor(255, 255, 255);
                myElements[drawingOrder[i]]->UI->draw();
                myElements[drawingOrder[i]]->drawGraphic(LEFT_MARGIN_X+5 ,(margin * 7) + ((numOfElements - i -2) * 190), PREVIEW_WIDTH,PREVIEW_HEIGHT);
            }
            ofPopStyle();
        }
        
        logo.draw(10, 9, 60, 60);
        logoDigitalNetwork.draw(1150, 12, 110, 40);
//        logoChristie.draw(950,20,235,35);
    }
    
}



//--------------------------------------------------------------
void MainWindow::keyPressed(int key)
{
	if(key == OF_KEY_RETURN)
	{
        drawUIs=!drawUIs;
        
        if(drawUIs)
		{	
//			myElements[0]->UI->setVisible(true);
			myElements[1]->UI->setVisible(true);
			myElements[2]->UI->setVisible(true);
			myElements[3]->UI->setVisible(true);
			elemMix.UI->setVisible(true);
		}
		else 
		{
//			myElements[0]->UI->setVisible(false);
			myElements[1]->UI->setVisible(false);
			myElements[2]->UI->setVisible(false);
			myElements[3]->UI->setVisible(false);
			elemMix.UI->setVisible(false);
		}
        
        bFullscreen=!bFullscreen;
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
    
    else if(key == 's') 
    {
        for (int i=1; i<numOfElements; i++)
        {
            myElements[i]->warper.save();
            myElements[i]->saveSettings();
        }
        
    }
    
    else if(key == 'l') 
    {
        for (int i=1; i<numOfElements; i++)
        {
            myElements[i]->warper.load();
            myElements[i]->loadSettings();
        }
        
    }
    
    else if (key == 'y')
    {
        elemMix.wideScreenPreview=!elemMix.wideScreenPreview;
    }

}

//--------------------------------------------------------------
void MainWindow::keyReleased(int key)
{
}


//--------------------------------------------------------------
void MainWindow::mouseMoved(int x, int y )
{
    
}

//--------------------------------------------------------------
void MainWindow::mouseDragged(int x, int y, int button)
{
    //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO: come sopra...
    if (elemSy.isSelected==true && elemSy.isWarpable==true) elemSy.warper.mouseDragged(x, y, button);
    else if (elemV1.isSelected==true && elemV1.isWarpable==true) elemV1.warper.mouseDragged(x, y, button);
    else if (elemImg2.isSelected==true && elemImg2.isWarpable==true) elemImg2.warper.mouseDragged(x, y, button);
    
}
//--------------------------------------------------------------
void MainWindow::mousePressed(int x, int y, int button)
{
    //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO: come sopra...
    if (elemSy.isSelected==true && elemSy.isWarpable==true) elemSy.warper.mousePressed(x, y, button);
    else if (elemV1.isSelected==true && elemV1.isWarpable==true) elemV1.warper.mousePressed(x, y, button);
    else if (elemImg2.isSelected==true && elemImg2.isWarpable==true) elemImg2.warper.mousePressed(x, y, button);
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



