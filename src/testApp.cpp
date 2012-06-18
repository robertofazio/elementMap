#include "testApp.h"

int num = 1;

//--------------------------------------------------------------
void testApp::setup()
{	
	ofEnableAlphaBlending();
    ofEnableSmoothing();
	
	// test that GL_STEREO is working on this machine
	GLboolean isStereoCapable = false;
	glGetBooleanv(GL_STEREO,&isStereoCapable);
	GLint maxVertexTextureImageUnits;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxVertexTextureImageUnits);

	if (isStereoCapable) printf(">> GL_STEREO OK \n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
	else printf(">> GL_STEREO KO !!\n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
	
	
	// create & setup elements on this app 
	elemImg.setup("./images/testHD.jpg", "", false,1030,300,"test Pattern");
	elemImg2.setup("./images/left.jpg", "./images/right.jpg", true,1460,300,"left/right images");
	elemV1.setup("./movies/left.mov","./movies/right.mov",true,20,300,"left/right movies");
	elemSy.setup("","",1920,1080,520,300,"Syphon:server");
	
	// add elements to the vector
	myElements.push_back(&elemV1);
	myElements.push_back(&elemSy);
	myElements.push_back(&elemImg);
	myElements.push_back(&elemImg2);

	// setup mix stuff
	int * drawingOrder = new int[myElements.size()];
	drawingOrder[0]=0;
	drawingOrder[1]=1;
	drawingOrder[2]=2;
	drawingOrder[3]=3;

	
	elemMix.setup(1920,1080,ELM_STEREO_ANAGLYPH,&myElements,drawingOrder,1000,600,"4chan mixer");
	
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
	elemMix.drawOutput(20,500,ofGetWidth()/2,ofGetHeight()/2);

	// just draw the preview inputs of mixer
	ofSetColor(255,255);
	int previewWidth = (ofGetWidth()-(20*myElements.size()))/myElements.size();
	int previewHeight = previewWidth / (float(ofGetWidth())/float(ofGetHeight()));
	for(int i=0;i<myElements.size();i++)
	{
		glDrawBuffer(GL_BACK);
		ofSetColor(255);
		myElements[i]->fboLeft.draw( (20*i) + (previewWidth*i),20,previewWidth,previewHeight);
		
	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

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

