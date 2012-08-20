#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // test that GL_STEREO is working on this machine
//	isGLCapable = GL_FALSE;
//	glGetBooleanv(GL_STEREO,&isGLCapable);
//    GLint maxVertexTextureImageUnits;
//	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxVertexTextureImageUnits);
//
//    if (isGLCapable==GL_TRUE) printf(">> GL_STEREO OK \n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
//	else printf(">> GL_STEREO KO !!\n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	

    outputSizeWidth=1920;
    outputSizeHeight=1080;

    mainWindow = new MainWindow();
    mainWindow->outputResolutionX = outputSizeWidth;
    mainWindow->outputResolutionY = outputSizeHeight;
    mainWindow->setup();
    
    // ofSetFrameRate(60);
    
	ofBackground(0,0,0);
        
    outputWindow = new OutputWindow(this);
    int firstScreenWidth = ofGetScreenWidth() - 1;      // Se posiziono esattamente alla fine del primo schermo, la finestra viene spostata indietro e posizionata totalmente all'interno del primo schermo. Cos“ invece si posiziona nel secondo schermo
    output = ofxFensterManager::get()->createFenster(400, 200, 400, 300, OF_WINDOW);
    output->addListener(outputWindow);
    output->setWindowTitle("Output");
    
    outputFullScreen = false;

}

void testApp::mouseMovedEvent(ofMouseEventArgs &args)
{
	cout << "MOUSE WAS MOVED" << endl;
}

//--------------------------------------------------------------
void testApp::update(){
    mainWindow->update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushStyle();
    ofBackground(0, 0, 0);  
    mainWindow->draw();
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    cout << "Key Pressed" << key << endl;
    mainWindow->keyPressed(key);    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
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
    outputWindow->setWindowShape(w, h);
}


//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
}