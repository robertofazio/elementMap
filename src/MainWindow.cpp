#include "MainWindow.h"

float lastTime=0.0f;
float medianFrameStep;
int num=0;

int margin = 10;

#define PREVIEW_WIDTH 100
#define PREVIEW_HEIGHT 75

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
	elemImg.setup("./images/testPattern1024.jpg", "", outputResolutionX,outputResolutionY, false, -50000 , (margin * 9) - 8 ,"TestPattern", false);
	elemV1.setup("./movies/left1024Audio.mov","./movies/right1024.mov", outputResolutionX,outputResolutionY, true, 120 , (margin * 9) - 8 + (190 * 1),"Movie", true);
	elemImg2.setup("./images/left1024.jpg", "./images/right1024.jpg", outputResolutionX,outputResolutionY, true, 120 , (margin * 9) - 8 + (190 * 2),"Image", true);
	elemSy.setup("","",outputResolutionX,outputResolutionY, 120 , (margin * 9) - 8 + (190 * 0),"Syphon", true);
	    
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
    elemImg.setOpacity(1);
    elemImg.setIsShow(true);
    
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
    
    comandi ="element.map alpha\n\n'w'\t\tactivate/deactivate warp\n't'\t\tactivate/deactivate translate\n\n'z'/'x'\tincrease/decrease grid X resolution\n'q'/'a'\tincrease/decrease grid Y resolution\n'n'/'m'\tselect previous/next point\n'v'\t\tselect quad vertex\n'h'\t\thold to select multiple grid points\n'c'\t\tclear quad warp transformation\n'r'\t\treset point position\n'cmd'+'r'\treset all grid points\n\n'g'\t\tshow/hide mesh grid\n's'\t\tsave warp to xml\n'l'\t\tload warp from xml\n\n'return'\tin main window hide/show GUI\n'f'\t\tin second window change fullscreen mode\n\n\nall working with arrow keys;\nquad warping support mouse drag too\n\nSPACEBAR\tplay/pause video\nBACKSPACE\trewind video\n','/'.'\t\t\tframe by frame\n\n**********************************\n\nin output window press:\n\n'1'\tto select syphon layer\n'2'\tto select video layer\n'3'\tto select image layer\n'0'\tto deselect all";
    
//    fontSmall.loadFont("georgiaz.ttf", 7);
//    fontLarge.loadFont("georgiaz.ttf", 18);

    fontSmall.loadFont("CODEBold.otf", 7);
    fontLarge.loadFont("CODEBold.otf", 18);
    
	fontSmall.setLineHeight(12.0f);
	fontSmall.setLetterSpacing(1.017);
    
	fontLarge.setLineHeight(12.0f);
	fontLarge.setLetterSpacing(1.017);
        
    
    logo.loadImage("./images/logo.png");

    //di default il test pattern non è visibile:
    elemImg.setIsActive(false);    
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
        fontLarge.drawString("element.Map", margin , margin * 4);
        ofSetColor(0, 255, 206);
        ofLine(650, margin * 6, ofGetWindowWidth() - 10, margin * 6);
        ofPopStyle();

        elemMix.drawOutput(650, margin * 8, 600, 450   );
        elemMix.UI->draw();
        
        ofPushStyle();
        ofSetColor(255, 255, 255);
        fontSmall.drawString("Press 'i' for info", 10 , ofGetWindowHeight() - 10);
        
        // frame by frame
        ofDrawBitmapString("video frame Left " + ofToString(elemV1.leftChannelPlayer.getCurrentFrame()), 650, 700);
        ofDrawBitmapString("video frame Right " + ofToString(elemV1.rightChannelPlayer.getCurrentFrame()), 650, 710);
        if (elemV1.leftChannelPlayer.getCurrentFrame() == elemV1.rightChannelPlayer.getCurrentFrame()) {
            ofSetHexColor(0x00FFFF);            
            ofDrawBitmapString("FRAME STERO SYNC", 650,740);
            
        }
        
        else {
            ofSetHexColor(0xFF0000);
            ofDrawBitmapString("FRAME STERO NOT IN SYNC", 650,740);
        }
        
        ofPopStyle();
        
        if(drawPreviews)    
        {
            // just draw the preview inputs of mixer
            ofPushStyle();
            ofSetColor(255,255);
            int previewWidth = (ofGetWidth()-(20*numOfElements))/numOfElements;
            int previewHeight = previewWidth / (4.0/3.0);
            glDrawBuffer(GL_BACK);
            for(int i = 0;i<numOfElements - 1;i++)
            {
                ofSetColor(0, 255, 206);
                ofLine(margin, (margin * 6) + ((i) * 190), 645, (margin * 6) + ((i) * 190));
                ofSetColor(255, 255, 255);
                myElements[drawingOrder[i]]->drawGraphic(margin ,(margin * 7) + ((numOfElements - i -2) * 190), PREVIEW_WIDTH,PREVIEW_HEIGHT);
                myElements[drawingOrder[i]]->UI->draw();
            }
            ofPopStyle();
        }
        
        logo.draw(1195, 2, 60, 60);
        
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
			myElements[0]->UI->setVisible(true);
			myElements[1]->UI->setVisible(true);
			myElements[2]->UI->setVisible(true);
			myElements[3]->UI->setVisible(true);
			elemMix.UI->setVisible(true);
		}
		else 
		{
			myElements[0]->UI->setVisible(false);
			myElements[1]->UI->setVisible(false);
			myElements[2]->UI->setVisible(false);
			myElements[3]->UI->setVisible(false);
			elemMix.UI->setVisible(false);
		}
        
        bFullscreen=!bFullscreen;
	}
    if(key == ' ')
    {
        if (elemV1.leftChannelPlayer.isPlaying()) 
        {
            elemV1.leftChannelPlayer.stop();
            elemV1.rightChannelPlayer.stop();
            
        } 
        else
        {
            elemV1.leftChannelPlayer.play();
            elemV1.rightChannelPlayer.play();
        }
        
    }
    
    if (key== OF_KEY_BACKSPACE)
    {
        elemV1.leftChannelPlayer.play();
        elemV1.rightChannelPlayer.play();
        elemV1.leftChannelPlayer.setFrame(0);
        elemV1.rightChannelPlayer.setFrame(0);
        elemV1.leftChannelPlayer.stop();
        elemV1.rightChannelPlayer.stop();
    }

    
    if(key == '.')
    {
        elemV1.leftChannelPlayer.play();
        elemV1.rightChannelPlayer.play();
        elemV1.leftChannelPlayer.setFrame(elemV1.leftChannelPlayer.getCurrentFrame()+1);
        elemV1.rightChannelPlayer.setFrame(elemV1.leftChannelPlayer.getCurrentFrame());
        elemV1.leftChannelPlayer.stop();
        elemV1.rightChannelPlayer.stop();

    }

    if(key == ',')
    {
        elemV1.leftChannelPlayer.play();
        elemV1.rightChannelPlayer.play();
        elemV1.leftChannelPlayer.setFrame(elemV1.leftChannelPlayer.getCurrentFrame()-1);
        elemV1.rightChannelPlayer.setFrame(elemV1.leftChannelPlayer.getCurrentFrame());
        elemV1.leftChannelPlayer.stop();
        elemV1.rightChannelPlayer.stop();
        
    }

    if(key == 'i')
        ofSystemAlertDialog(comandi);
    
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

