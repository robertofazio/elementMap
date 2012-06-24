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

	
	elemMix.setup(outputResolutionX,outputResolutionY,ELM_STEREO_OPENGL,myElements,numOfElements,drawingOrder,650,650,"mixer");
	
	ofBackground(0, 0,60);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	elemImg.setOpacity(0.5f);
	elemImg2.setOpacity(0.5f);
	elemV1.setOpacity(0.5f);
	elemSy.setOpacity(0.5f);
	
}

//--------------------------------------------------------------
void testApp::update()
{
	elemMix.update();
}

//--------------------------------------------------------------
void testApp::draw()
{	
	// prepare and draw mixer element
	ofSetColor(255,255);
	elemMix.drawIntoFbo(isStereoCapable);
	ofSetColor(255,255);
	elemMix.drawOutput(20,340,ofGetWidth()/2,ofGetHeight()/2);

	if(drawPreviews)
	{	
		// just draw the preview inputs of mixer
		ofSetColor(255,255);
		int previewWidth = (ofGetWidth()-(20*numOfElements))/numOfElements;
		int previewHeight = previewWidth / (4.0/3.0);
		glDrawBuffer(GL_BACK);
		for(int i=0;i<numOfElements;i++)
		{
			myElements[drawingOrder[i]]->drawLeft(20+(drawingOrder[i]*ofGetWidth()/4),20,ofGetWidth()/4,ofGetHeight()/4);
			
		}
	}
	
//	ofSetColor(255);
//	float f = ofGetElapsedTimeMillis()-lastTime;
//	medianFrameStep = (medianFrameStep + f) / float(num+1);
//	ofDrawBitmapString(ofToString(f) + " /\n/ " + ofToString(medianFrameStep),100,100);
//	lastTime=ofGetElapsedTimeMillis();
//	num=num+1;
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if(key=='a')
	{
		elemImg.setIsActive(false);
	}
	if(key=='1')
	{
		ofToggleFullscreen();
	}
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
	else if(key=='f')
	{
		printf("fps : %f\n",ofGetFrameRate());
	}
	else if(key=='g')
	{
		drawUIs=!drawUIs;
		
		if(drawUIs)
		{	
//			myElements[0]->UI->setVisible(true);
//			myElements[1]->UI->setVisible(true);
//			myElements[2]->UI->setVisible(true);
//			myElements[3]->UI->setVisible(true);
//			elemMix.UI->setVisible(true);
		}
		else 
		{
//			myElements[0]->UI->setVisible(false);
//			myElements[1]->UI->setVisible(false);
//			myElements[2]->UI->setVisible(false);
//			myElements[3]->UI->setVisible(false);
//			elemMix.UI->setVisible(false);
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

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

