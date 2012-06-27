#include "testApp.h"

float lastTime=0.0f;
float medianFrameStep;
int num=0;
//--------------------------------------------------------------
void testApp::setup()
{	
	
	outputResolutionX	= 1024;
	outputResolutionY	= 768;
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
	elemImg.setup("./images/testPattern1024.jpg", "", false, (20) + (previewWidth*0),300,"Test Pattern");
	elemV1.setup("./movies/left1024.mov","./movies/right1024.mov",true,(20) + (previewWidth*1),300,"Movies");
	elemImg2.setup("./images/left1024.jpg", "./images/right1024.jpg", true, (20*2) + (previewWidth*2),300,"Images");
	elemSy.setup("","",outputResolutionX,outputResolutionY, (20*3) + (previewWidth*3),300,"Syphon");
	
	// add elements to the vector
	myElements = new element*[4];
	myElements[0] = &elemImg;
	myElements[1] = &elemV1;
	myElements[2] = &elemImg2;
	myElements[3] = &elemSy;
	
//    elemImg.addFX(ELEMENT_FX_MASK);
//    elemImg2.addFX(ELEMENT_FX_MASK);
//    elemSy.addFX(ELEMENT_FX_MASK);
    
//	myElements.push_back(&elemV1);
//	myElements.push_back(&elemSy);
//	myElements.push_back(&elemImg2);
//	myElements.push_back(&elemImg);

	// setup mix stuff
	drawingOrder = new int[numOfElements];
	drawingOrder[0]=0;
	drawingOrder[1]=1;
	drawingOrder[2]=2;
	drawingOrder[3]=3;

	
	elemMix.setup(outputResolutionX,outputResolutionY,ELM_STEREO_MONO,myElements,numOfElements,drawingOrder,650,650,"mixer");
	
//    elemMix.addFX(ELEMENT_FX_MASK);
	ofBackground(0, 0,60);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	elemImg.setOpacity(0.5f);
	elemImg2.setOpacity(0.5f);
	elemV1.setOpacity(0.5f);
	elemSy.setOpacity(0.5f);
	
    
    bFullscreen=false;
    
    //warp stuff:
    text.allocate(outputResolutionX,outputResolutionY, GL_RGBA);
    //default grid= 4x4 control points
    xRes=4;
    yRes=4;
    //create grid coordinates
    createGrid(xRes, yRes);    
    //start with active warp
    bWarpActive=true;

    
    
}

//--------------------------------------------------------------
void testApp::update()
{
	elemMix.update();
}

//--------------------------------------------------------------
void testApp::draw()
{	
	
    if (bFullscreen) {
        
        ofBackground(0,0,0,255);
        ofSetColor(0,0);
        elemMix.drawIntoFbo(isStereoCapable);
        ofSetColor(255, 255, 255);

        //load texture from mixer fbo (repeat for right side when we will want stereo mode....)
        text=elemMix.fboLeft.getTextureReference();

        text.draw(0,0,outputResolutionX, outputResolutionY);
       
        
        //bind texture
        glEnable(text.getTextureData().textureTarget);
        
        //create correspondence between control points and texture points
        int quad=1;
        int index=0;
        
        while (quad<(nQuads+1)) {
            glBegin(GL_QUADS);
            for (int vertex=0; vertex<4; vertex++) 
            {
                glTexCoord2f(texVert[index].x, texVert[index].y);
                glVertex2f(vertici[index].x, vertici[index].y);
                
                index++;
            }
            glEnd();
            quad++;
        }
        
        glDisable(text.getTextureData().textureTarget);
        
        if (bWarpActive) drawGrid();    
        
        //elemMix.drawOutput(ofGetWidth()/2,340,ofGetWidth()/2,ofGetHeight()/2);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
//        elemMix.drawPreview(0,0,ofGetWidth(),ofGetHeight());
 
        
    }
    else 
    {
    // prepare and draw mixer element
	ofSetColor(0,0);
	elemMix.drawIntoFbo(isStereoCapable);
//	ofSetColor(255,255);
    ofSetColor(255, 255, 255);
	//elemMix.drawOutput(ofGetWidth()/2,340,ofGetWidth()/2,ofGetHeight()/2);
    elemMix.drawInfo();
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    elemMix.drawPreview(20,340,ofGetWidth()/2,ofGetHeight()/2);

	if(drawPreviews)
	{	
		// just draw the preview inputs of mixer
		ofSetColor(255,255);
		int previewWidth = (ofGetWidth()-(20*numOfElements))/numOfElements;
		int previewHeight = previewWidth / (4.0/3.0);
		glDrawBuffer(GL_BACK);
		for(int i=0;i<numOfElements;i++)
		{
            if(myElements[drawingOrder[i]]->effects.size() == 0)
                myElements[drawingOrder[i]]->drawLeft(20+(drawingOrder[i]*ofGetWidth()/4),20,ofGetWidth()/4,ofGetHeight()/4);
			else
                myElements[drawingOrder[i]]->drawPreview(20+(drawingOrder[i]*ofGetWidth()/4),20,ofGetWidth()/4,ofGetHeight()/4);
			
		}
	}
	
//	ofSetColor(255);
//	float f = ofGetElapsedTimeMillis()-lastTime;
//	medianFrameStep = (medianFrameStep + f) / float(num+1);
//	ofDrawBitmapString(ofToString(f) + " /\n/ " + ofToString(medianFrameStep),100,100);
//	lastTime=ofGetElapsedTimeMillis();
//	num=num+1;

    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if(key=='f')
	{
        //		printf("fps : %f\n",ofGetFrameRate());
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
        ofToggleFullscreen();
	}

    if (bFullscreen) {
    
        switch (key) {

    case 'w':
        bWarpActive=!bWarpActive;
        break;
        
    case 'x':
        if (bWarpActive)
        {
            xRes++;
            if (xRes>MAX_RESOLUTION) xRes=MAX_RESOLUTION;
            createGrid(xRes, yRes);
        }
        break;
        
    case 'z':
        if (bWarpActive)
        {
            xRes--;
            if (xRes<2) xRes=2;
            createGrid(xRes, yRes);
        }
        break;
        
    case 'q':
        if (bWarpActive)
        {
            yRes++;
            if (yRes>MAX_RESOLUTION) yRes=MAX_RESOLUTION;
            createGrid(xRes, yRes);
        }
        break;
        
    case 'a':
        if (bWarpActive)
        {
            yRes--;
            if (yRes<2) yRes=2;
            createGrid(xRes, yRes);
        }
        break;
        
    case OF_KEY_UP:
        if (bWarpActive)
        {
            
            for (int i=0; i<nPoints; i++) {
                
                if (vertici[i].z==1)
                {
                    vertici[i].y-=1;
                }
            }
        }
        break;
        
    case OF_KEY_DOWN:
        if (bWarpActive)
        {
            
            for (int i=0; i<nPoints; i++) {
                
                if (vertici[i].z==1)
                {
                    vertici[i].y+=1;
                }
            }
        }
        break;
        
    case OF_KEY_LEFT:
        if (bWarpActive)
        {
            
            for (int i=0; i<nPoints; i++) {
                
                if (vertici[i].z==1)
                {
                    vertici[i].x-=1;
                }
            }
        }
        break;
    case OF_KEY_RIGHT:
        if (bWarpActive)
        {
            
            for (int i=0; i<nPoints; i++) {
                
                if (vertici[i].z==1)
                {
                    vertici[i].x+=1;
                }
            }
        }
        break;
    
    default:
        break;
        }

    }
    
    else {
    if(key=='a')
	{
		elemImg.setIsActive(false);
	}
//	if(key=='1')
//	{
//		ofToggleFullscreen();
//	}
	if(key=='u')
	{
		elemMix.useNoShader=true;
	}
	if(key=='U')
	{
		elemMix.useNoShader=false;
	}
	else if(key=='p')
	{
		drawPreviews=!drawPreviews;
	}
	else if(key=='g')
	{
		drawUIs=!drawUIs;
	}
        
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    if (bWarpActive&&bFullscreen)
    {
        
        for (int i=0; i<nPoints; i++) {
            
            if (vertici[i].z==1)
            {
                vertici[i].x=x;
                vertici[i].y=y;
            }
        }
    }
    

    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    if (bWarpActive&&bFullscreen)
    {
        
        for (int i=0; i<nPoints; i++) {
            
            if (abs(mouseX-vertici[i].x)<10 && abs(mouseY-vertici[i].y)<10) {
                vertici[i].z=1;
            }
            else vertici[i].z=0;   
        }
        
    }

    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


//WARP UTILITIES:
//--------------------------------------------------------------
void testApp::createGrid(int _xRes, int _yRes){ 
    
    nQuads=(xRes-1)*(yRes-1);
    nPoints=4*nQuads;
    
    int index = 0;
    int row=0;
    int col=0;
    
    while (row<(_yRes-1)) {
        for (int ind=0; ind<4; ind++) {
            
            if (ind==0 || ind==3) vertici[index].x=col*(outputResolutionX/(_xRes-1));
            if (ind==1 || ind==2) vertici[index].x=(col+1)*(outputResolutionX/(_xRes-1));
            
            if (ind==0 || ind==1) vertici[index].y=row*(outputResolutionY/(_yRes-1)); 
            if (ind==2 || ind==3) vertici[index].y=(row+1)*(outputResolutionY/(_yRes-1)); 
            
            texVert[index]=vertici[index];
            
            index++;
        }
        col++;
        if (col>(_xRes-2)) 
        {
            row++;
            col=0;
        }
    }
}



//--------------------------------------------------------------
void testApp::drawGrid() {
    
    //draw Grid Lines
    int quad=0;
    int index=0;
    
    ofPushStyle();
    ofSetColor(0,255,0,150);
    ofFill();
    ofSetLineWidth(1);
    while (quad<nQuads) {
        for (int vertex=0; vertex<4; vertex++) 
        {
            if (vertex!=0) ofLine(vertici[index-1].x, vertici[index-1].y, vertici[index].x, vertici[index].y);
            index++;
            
        }
        quad++;
    }
    ofPopStyle();
    
    
    //draw Points
    for (int point=0; point<nPoints; point++) {
        ofPushStyle();
        if (vertici[point].z==1) ofSetColor(255,0,0,255);
        else ofSetColor(0,255,0,255);
        ofFill();
        ofCircle(vertici[point].x, vertici[point].y, 5, 5);
        ofPopStyle();
        
    }
    
}



