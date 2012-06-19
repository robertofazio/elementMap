#include "testApp.h"

int num = 1;

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
	
	
	int previewWidth = (ofGetWidth()-(20*myElements.size()))/4;
	int previewHeight = previewWidth / (float(ofGetWidth())/float(ofGetHeight()));

	// create & setup elements on this app 
	elemImg.setup("./images/testPattern1024.jpg", "", false, (20*3) + (previewWidth*3),300,"Test Pattern");
	elemImg2.setup("./images/left.png", "./images/right.png", true, (20*2) + (previewWidth*2),300,"Images");
	elemV1.setup("./movies/left.mov","./movies/right.mov",true,20,300,"Movies");
	elemSy.setup("","",outputResolutionX,outputResolutionY, (20*1) + (previewWidth*1),300,"Syphon");
	
	// add elements to the vector
	myElements.push_back(&elemV1);
	myElements.push_back(&elemSy);
	myElements.push_back(&elemImg2);
	myElements.push_back(&elemImg);

	// setup mix stuff
	int * drawingOrder = new int[myElements.size()];
	drawingOrder[0]=0;
	drawingOrder[1]=1;
	drawingOrder[2]=2;
	drawingOrder[3]=3;

	
	elemMix.setup(outputResolutionX,outputResolutionY,ELM_STEREO_OPENGL,&myElements,drawingOrder,650,650,"mixer");
	
	ofBackground(255, 0,0);
	
	drawAllStereo=true;
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
	elemMix.drawOutput(20,450,ofGetWidth()/2.5,(ofGetWidth()/2.5)/(4.0/3.0));


	if(drawPreviews)
	{	
		// just draw the preview inputs of mixer
		ofSetColor(255,255);
		int previewWidth = (ofGetWidth()-(20*myElements.size()))/myElements.size();
		int previewHeight = previewWidth / (4.0/3.0);
		glDrawBuffer(GL_BACK);
		for(int i=0;i<myElements.size();i++)
		{
			ofSetColor(255);
			myElements[i]->fboLeft.draw( (20*i) + (previewWidth*i),20,previewWidth,previewHeight);
			
		}
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if(key=='a')
	{
		elemImg.setIsActive(false);
	}
	else if(key=='p')
	{
		drawPreviews=!drawPreviews;
	}
	else if(key=='f')
	{
		ofToggleFullscreen();
	}
    else if(key=='g')
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

	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	if(!elemV1.getDrawInStereo()) elemV1.setDrawInStereo(true);
	else elemV1.setDrawInStereo(false);
	drawAllStereo=!drawAllStereo;
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

