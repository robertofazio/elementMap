#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // test that GL_STEREO is working on this machine
    GLboolean	isGLCapable = GL_FALSE;
	glGetBooleanv(GL_STEREO,&isGLCapable);
    QuadBufferCapable=isGLCapable;
    
    GLint maxVertexTextureImageUnits;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxVertexTextureImageUnits);

    if (QuadBufferCapable) printf(">> testApp: GL_STEREO OK \n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
	else {
        
        glDrawBuffer(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        printf(">> testApp: GL_STEREO KO !!\n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
        ofSystemAlertDialog("OPEN_GL quad buffered Stereo mode NOT supported on this machine");

    }
    
    
    outputSizeWidth=1920;
    outputSizeHeight=1080;
    
    mainWindow = new MainWindow();
    mainWindow->outputResolutionX = outputSizeWidth;
    mainWindow->outputResolutionY = outputSizeHeight;
    mainWindow->setup();

    
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
    mainWindow->keyPressed(key);    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    mainWindow->keyReleased(key);       
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
    
    ofPoint dragPt;
    
    if( dragInfo.files.size() > 0 ){
        dragPt = dragInfo.position;
        cout << dragPt.x << " ; " << dragPt.y << endl;
        
        //            draggedImages.assign( dragInfo.files.size(), ofImage() );
        //            for(int k = 0; k < dragInfo.files.size(); k++){
        //            draggedImages[k].loadImage(dragInfo.files[k]);
        //            }
    }
    

    
}