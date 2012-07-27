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
	
	//outputResolutionX	= ofGetScreenWidth();
	//outputResolutionY	= ofGetScreenHeight();
	drawPreviews		= true;
	drawUIs				= true;
	
	ofEnableAlphaBlending();
	
	// test that GL_STEREO is working on this machine
	GLboolean isStereoCapable = false;
	glGetBooleanv(GL_STEREO,&isStereoCapable);
	GLint maxVertexTextureImageUnits;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxVertexTextureImageUnits);
    
	if (isStereoCapable) printf(">> GL_STEREO OK \n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
	else printf(">> GL_STEREO KO !!\n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
	
	numOfElements = 4;
	
	int previewWidth = (ofGetWidth()-(20*numOfElements))/4;
	int previewHeight = previewWidth / (float(ofGetWidth())/float(ofGetHeight()));
    
	// create & setup elements on this app 
	elemImg.setup("./images/testPattern1024.jpg", "", outputResolutionX,outputResolutionY, false, -50000 , (margin * 9) - 8 ,"Test Pattern");
	elemV1.setup("./movies/left1024.mov","./movies/right1024.mov", outputResolutionX,outputResolutionY, true, 215 , (margin * 9) - 8 + (190 * 1),"Movies");
	elemImg2.setup("./images/left1024.jpg", "./images/right1024.jpg", outputResolutionX,outputResolutionY, true, 215 , (margin * 9) - 8 + (190 * 2),"Images");
	elemSy.setup("","",outputResolutionX,outputResolutionY, 215 , (margin * 9) - 8 + (190 * 0),"Syphon");
	
    bPaused=false;
    
    //video starts at frame number 0 and paused
    elemV1.leftChannelPlayer.play();
    elemV1.rightChannelPlayer.play();
    elemV1.leftChannelPlayer.setPosition(0.0);
    elemV1.rightChannelPlayer.setPosition(0.0);
    elemV1.leftChannelPlayer.stop();
    elemV1.rightChannelPlayer.stop();

    
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
	
	elemMix.setup(outputResolutionX,outputResolutionY,ELM_STEREO_MONO,myElements,numOfElements,drawingOrder, 650, (margin * 9) - 7,"mixer");
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    bFullscreen=false;
    bSpeedUp=false;
    
    mainOutputWarp.setup(outputResolutionX, outputResolutionY);
    
    
    comandi ="element.map alpha 0.0.2\n\n'w'\t\tactivate/deactivate warp\n't'\t\tactivate/deactivate translate\n\n'z'/'x'\tincrease/decrease grid X resolution\n'q'/'a'\tincrease/decrease grid Y resolution\n'n'/'m'\tselect previous/next point\n'v'\t\tselect quad vertex\n'h'\t\thold to select multiple grid points\n'c'\t\tclear quad warp transformation\n'r'\t\treset point position\n\n'g'\t\tshow/hide mesh grid\n's'\t\tsave warp to xml\n'l'\t\tload warp from xml\n\n'return'\t in main window hide/show GUI\n'f'\t\t in second window change fullscreen mode\n\n\nall working with arrow keys;\n quad warping support mouse drag too\n\nSPACEBAR\tplay/pause video\nBACKSPACE\trewind video";
    
    georgiaitalic8.loadFont("georgiaz.ttf", 7);
    georgiaitalic14.loadFont("georgiaz.ttf", 18);
    
	georgiaitalic8.setLineHeight(12.0f);
	georgiaitalic8.setLetterSpacing(1.017);
    
	georgiaitalic14.setLineHeight(12.0f);
	georgiaitalic14.setLetterSpacing(1.017);    
    
    
    logo.loadImage("./images/logo.png");
    
}

//--------------------------------------------------------------
void MainWindow::update()
{
	elemMix.update();
    if (bFullscreen) mainOutputWarp.updateCoordinates();
    
}


//--------------------------------------------------------------
void MainWindow::draw()
{	
    ofBackground(35, 31, 32);
    
    ofSetColor(0,0);
    elemMix.drawIntoFbo(isStereoCapable);
    
    if (bFullscreen) {
        /*
        ofSetColor(255, 255, 255);
        //load texture from mixer fbo 
        if(elemMix.getOutputStereoMode() == ELM_STEREO_ANAGLYPH)
            text=elemMix.fboAnagliph.getTextureReference();
        else
            text=elemMix.fboLeft.getTextureReference();
        
        mainOutputWarp.warp(text);
        */
        ofPoint position(ofGetWindowWidth() - 100, ofGetWindowHeight() - 10);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), position);
    }
    
    else 
        
    {
        
        
        //preview window (non-fulscreen)
        
        ofSetColor(255, 255, 255);
        
        georgiaitalic14.drawString("element.Map", margin , margin * 4);
        ofSetColor(0, 255, 206);
        ofLine(650, margin * 6, ofGetWindowWidth() - 10, margin * 6);
        elemMix.drawOutput(650, margin * 8, 600, 450   );
        elemMix.UI->draw();
        
        
        ofSetColor(255, 255, 255);
        georgiaitalic8.drawString("Press 'f' to enter in fullscreen mode and edit warp; when in fullscreen press 'i' for info", 10 , ofGetWindowHeight() - 10);
        
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
        
        if(drawPreviews)    
        {
            // just draw the preview inputs of mixer
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
        //ofToggleFullscreen();
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
    elemV1.leftChannelPlayer.setPosition(0.0);
    elemV1.rightChannelPlayer.setPosition(0.0);
    elemV1.leftChannelPlayer.stop();
    elemV1.rightChannelPlayer.stop();
    }

    
    if(key == 'p')
    {
        frameByframe=!frameByframe;
        elemV1.leftChannelPlayer.setPaused(frameByframe);
        elemV1.rightChannelPlayer.setPaused(frameByframe);        
    }
    if(key == 'i')
        ofSystemAlertDialog(comandi);
    
    /*
    if (bFullscreen) {
        
        switch (key) {
                
            case 's':
                mainOutputWarp.save();
                break;
                
            case 'l':
                mainOutputWarp.load();
                break;
                
                
            case ' ':
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
                
                break;
                
            case 'p':
                frameByframe=!frameByframe;
                elemV1.leftChannelPlayer.setPaused(frameByframe);
                elemV1.rightChannelPlayer.setPaused(frameByframe);
                break;
                
            case ':':
                elemV1.leftChannelPlayer.nextFrame();
                elemV1.rightChannelPlayer.nextFrame();
                break;
                
            case ';':
                elemV1.leftChannelPlayer.previousFrame();
                elemV1.rightChannelPlayer.previousFrame();
                break;
                
            case OF_KEY_BACKSPACE:
                elemV1.leftChannelPlayer.play();
                elemV1.rightChannelPlayer.play();
                elemV1.leftChannelPlayer.setPosition(0.0);
                elemV1.rightChannelPlayer.setPosition(0.0);
                elemV1.leftChannelPlayer.stop();
                elemV1.rightChannelPlayer.stop();
                break;
                
            case 'w':
                mainOutputWarp.quadWarp.toggleShow();
                mainOutputWarp.bWarpActive=!mainOutputWarp.bWarpActive;
                break;     
                
            case 'g':
                if (mainOutputWarp.bWarpActive) mainOutputWarp.bViewGrid=!mainOutputWarp.bViewGrid;
                break;     
                
            case 't':
                mainOutputWarp.bSposta=!mainOutputWarp.bSposta;
                mainOutputWarp.selectMainCorners();                
                break;
                
            case 'x':
                mainOutputWarp.increaseXgrid();
                break;
                
                
            case 'z':
                mainOutputWarp.decreaseXgrid();
                break;
                
                
            case 'q':
                mainOutputWarp.increaseYgrid();
                break;
                
                
            case 'a':
                mainOutputWarp.decreaseYgrid();
                break;
                
                
            case OF_KEY_UP:
                if (bSpeedUp) mainOutputWarp.pointUP(40);
                else mainOutputWarp.pointUP(1);
                break;
                
            case OF_KEY_DOWN:
                if (bSpeedUp) mainOutputWarp.pointDOWN(40);
                else mainOutputWarp.pointDOWN(1);
                break;
                
            case OF_KEY_LEFT:
                if (bSpeedUp) mainOutputWarp.pointLEFT(40);
                else mainOutputWarp.pointLEFT(1);
                break;
                
            case OF_KEY_RIGHT:
                if (bSpeedUp) mainOutputWarp.pointRIGHT(40);
                else mainOutputWarp.pointRIGHT(1);
                break;
                
            case 'm':
                mainOutputWarp.selectNextPoint();
                break;
                
            case 'n':
                mainOutputWarp.selectPrevPoint();
                break;
                
                
            case 'v':
                mainOutputWarp.selectNextMainCorner();
                break;
                
                
            case 'r':
                mainOutputWarp.resetPoint();
                break;
                
            case 'c':
                mainOutputWarp.resetCorners();
                break;
                
            case 'i':
                ofSystemAlertDialog(comandi);
                break;
                
            case 'h':
                mainOutputWarp.bHoldSelection=true;
                break;
                
            case '<':
                bSpeedUp=true;
                break;
                
            default:
                break;
        }
        
     
    }
     */
    
}

//--------------------------------------------------------------
void MainWindow::keyReleased(int key)
{
    switch (key) {
        case 'h':
            mainOutputWarp.bHoldSelection=false;
            break;
            
        case '<':
            bSpeedUp=false;
            break;
            
            
    }
    
}


//--------------------------------------------------------------
void MainWindow::mouseMoved(int x, int y )
{
    
}

//--------------------------------------------------------------
void MainWindow::mouseDragged(int x, int y, int button)
{
    
    
    if (bFullscreen )     mainOutputWarp.mouseDragged(x, y, button);
    
}

//--------------------------------------------------------------
void MainWindow::mousePressed(int x, int y, int button)
{
    
    if (bFullscreen )    mainOutputWarp.mousePressed(x, y, button);
    
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

