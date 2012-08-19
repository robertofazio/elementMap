#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofxXmlSettings tempXML;
    tempXML.loadFile("outputRes.xml");
    
    tempXML.pushTag("output");
    outputSizeWidth = (int)tempXML.getValue("width", 0);
    outputSizeHeight = (int)tempXML.getValue("height", 0);
    
    mainWindow = new MainWindow();
    mainWindow->outputResolutionX = outputSizeWidth;
    mainWindow->outputResolutionY = outputSizeHeight;
    mainWindow->setup();
    
    // ofSetFrameRate(60);
    
	ofBackground(0,0,0);
        
    outputWindow = new OutputWindow(this);
    int firstScreenWidth = ofGetScreenWidth() - 1;      // Se posiziono esattamente alla fine del primo schermo, la finestra viene spostata indietro e posizionata totalmente all'interno del primo schermo. Cos“ invece si posiziona nel secondo schermo
    output = ofxFensterManager::get()->createFenster(firstScreenWidth, 0, 1024, 768, OF_WINDOW);
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
    ofBackground(0, 0, 0);  
    mainWindow->draw();
    
    
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