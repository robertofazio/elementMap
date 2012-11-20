#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    
    ofSetLogLevel(OF_LOG_SILENT);
    
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
 //       ofSystemAlertDialog("OPEN_GL quad buffered Stereo mode NOT supported on this machine");

    }

    logoNew.loadImage("./utils/elementLogoNew.png");
    
    outputSizeWidth=0;
    outputSizeHeight=0;
    bOptionsDone= false;
    selectOutputResolution();
      
}

//--------------------------------------------------------------
void testApp::mouseMovedEvent(ofMouseEventArgs &args)
{
	cout << "MOUSE WAS MOVED" << endl;
}

//--------------------------------------------------------------
void testApp::update(){
    if (bOptionsDone) mainWindow->update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    if (bOptionsDone) {
        ofPushStyle();
        ofBackground(0, 0, 0);  
        mainWindow->draw();
        ofPopStyle();
    } 
    else {
        logoNew.draw(ofGetScreenWidth()*.5+100,ofGetScreenHeight()*.5-120, 150, 96);
     selectResolutionGUI->draw();
        ofPushStyle();
        ofSetColor(ofColor :: white);
        string message = "";
        if (QuadBufferCapable) message = "OPEN_GL quad buffered Stereo mode SUPPORTED on this machine";
        else message = "OPEN_GL quad buffered Stereo mode NOT SUPPORTED on this machine";
        ofDrawBitmapString(message, (ofGetScreenWidth()-600)*.5+10,(ofGetScreenHeight()-400)*.5+380);
        ofPopStyle();
    }

}


//--------------------------------------------------------------
void testApp::selectOutputResolution(){

    selectResolutionGUI = new ofxUICanvas((ofGetScreenWidth()-600)*.5,(ofGetScreenHeight()-400)*.5, 600, 400);
    selectResolutionGUI->setDrawOutline(true);
    selectResolutionGUI->setDrawBack(false);
    
    selectResolutionGUI->addWidget(new ofxUILabel(30, 30, "title", "OUTPUT RESOLUTION", OFX_UI_FONT_MEDIUM));
    selectResolutionGUI->addWidget(new ofxUILabel(30, 55, "subtitle", "2nd monitor or projector", OFX_UI_FONT_SMALL));
    
    resolutionsList.push_back("1024x768");
    resolutionsList.push_back("1280x800");
    resolutionsList.push_back("1280x1024");
    resolutionsList.push_back("1440x900");
    resolutionsList.push_back("1600x1200");
    resolutionsList.push_back("1920x1080");
    
    resGUI = new ofxUIDropDownList(30, 80, 150, "RESOLUTION", resolutionsList, OFX_UI_FONT_SMALL);
    resGUI->setDrawOutline(true);
    resGUI->setAutoClose(true);
    selectResolutionGUI->addWidget(resGUI);
    
    doneButton = new ofxUILabelButton(400, 30, 150, false, "DONE", OFX_UI_FONT_LARGE);
    doneButton->setDrawOutline(true);
    selectResolutionGUI->addWidget(doneButton);
    
    selectResolutionGUI->setVisible(true);
    ofAddListener(selectResolutionGUI->newGUIEvent,this,&testApp::guiEvent); 
    
}


//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
    //DONE BUTTON
    if( name=="DONE")
    {
        if (doneButton->getValue()) 
        {
            bOptionsDone=true;
            firstSetup();
        }
    }
    else if (name == "1024x768") {
        outputSizeWidth=1024;
        outputSizeHeight=768;
    }
    else if (name == "1280x800") {
        outputSizeWidth=1280;
        outputSizeHeight=800;
    }
    else if (name == "1280x1024") {
        outputSizeWidth=1280;
        outputSizeHeight=1024;
    }
    else if (name == "1440x900") {
        outputSizeWidth=1440;
        outputSizeHeight=900;
    }
    else if (name == "1600x1200") {
        outputSizeWidth=1600;
        outputSizeHeight=1200;
    }
    else if (name == "1920x1080") {
        outputSizeWidth=1920;
        outputSizeHeight=1080;
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
    if (bOptionsDone) mainWindow->keyPressed(key);    
    if (key=='1') 
    {
        bOptionsDone=true;
        firstSetup();
    }


}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if (bOptionsDone) mainWindow->keyReleased(key);       
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    cout << "mouse Pressed sentito da TestApp" << endl;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    if (bOptionsDone) outputWindow->setWindowShape(w, h);
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
    }
    

    
}


//--------------------------------------------------------------
void testApp::firstSetup() {
    
    mainWindow = new MainWindow();
    mainWindow->outputResolutionX = outputSizeWidth;
    mainWindow->outputResolutionY = outputSizeHeight;
    mainWindow->setup();
    
	ofBackground(0,0,0);
    
    outputWindow = new OutputWindow(this);
    output = ofxFensterManager::get()->createFenster(400, 200, 400, 300, OF_WINDOW);
    output->addListener(outputWindow);
    output->setWindowTitle("Output");
    
    outputFullScreen = false;
    
}