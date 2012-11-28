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

    if (QuadBufferCapable) 
    {
        glDrawBuffer(GL_BACK);
        printf(">> testApp: GL_STEREO OK \n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
    }
	else {
        
        glDrawBuffer(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        printf(">> testApp: GL_STEREO KO !!\n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
    }

    splash.loadImage("./utils/splash.jpg");
    msgFont.loadFont("NEOSANS.otf", 10);
    
    //default resolution: HD
    outputSizeWidth=1920;
    outputSizeHeight=1080;
    outputWidescreen=true;
    bOptionsDone= false;
    selectOutputResolution();
    bSetIniziale=true;

}

//--------------------------------------------------------------
void testApp::mouseMovedEvent(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void testApp::update(){
    if (bOptionsDone) mainWindow->update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    if (bOptionsDone) {
        if (bSetIniziale) {
            selectResolutionGUI->disable();
            bSetIniziale=false;
            mainWindow->bGLStereoCapable=QuadBufferCapable;
        }
        if (QuadBufferCapable) glDrawBuffer(GL_BACK_LEFT);
        ofPushStyle();
        ofBackground(0, 0, 0);  
        mainWindow->draw();
        ofPopStyle();
    } 
    else 
    {
        splash.draw(0,0,1280,800);
//        logoNew.draw(ofGetWindowWidth()*.5+100,ofGetWindowHeight()*.5-120, 150, 96);
        selectResolutionGUI->draw();
        ofPushStyle();
        ofSetColor(ofColor :: white);
        msgFont.drawString("OPEN_GL Stereo Mode\n\non this machine", 185,250);
        string message = "\n";
        if (QuadBufferCapable) message += "SUPPORTED";
        else message += "NOT SUPPORTED";
        ofPushStyle();
        ofSetColor(36, 203, 228);
        msgFont.drawString(message, 185,250);
        ofPopStyle();
        ofPopStyle();
    }

}


//--------------------------------------------------------------
void testApp::selectOutputResolution(){

    selectResolutionGUI = new ofxUICanvas(0,0, 1280, 800);
    selectResolutionGUI->setDrawOutline(true);
    selectResolutionGUI->setDrawBack(false);

//    selectResolutionGUI->addWidget(new ofxUILabel(600, 500, "run", "LAUNCH ELEMENT", OFX_UI_FONT_MEDIUM));
    doneButton = new ofxUILabelButton(490, 600, 300, false, "LAUNCH ELEMENT.MAP", OFX_UI_FONT_LARGE);
    doneButton->setDrawOutline(true);
    selectResolutionGUI->addWidget(doneButton);

    selectResolutionGUI->addWidget(new ofxUILabel(170, 320, "res", "SET OUTPUT RESOLUTION", OFX_UI_FONT_MEDIUM));
//    selectResolutionGUI->addWidget(new ofxUILabel(30, 55, "subtitle", "2nd monitor or projector", OFX_UI_FONT_SMALL));
    
    resolutionsList.push_back("1024x768");
    resolutionsList.push_back("1280x800");
    resolutionsList.push_back("1280x1024");
    resolutionsList.push_back("1440x900");
    resolutionsList.push_back("1600x1200");
    resolutionsList.push_back("1920x1080");
    
    resGUI = new ofxUIDropDownList(185, 340, 150, "RESOLUTION", resolutionsList, OFX_UI_FONT_SMALL);
    resGUI->setDrawOutline(true);
    resGUI->setAutoClose(true);
    selectResolutionGUI->addWidget(resGUI);
    
    selectResolutionGUI->setVisible(true);
    ofAddListener(selectResolutionGUI->newGUIEvent,this,&testApp::guiEvent); 
    
}


//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
    //DONE BUTTON
    if( name=="LAUNCH ELEMENT.MAP")
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
        outputWidescreen=false;

    }
    else if (name == "1280x800") {
        outputSizeWidth=1280;
        outputSizeHeight=800;
        outputWidescreen=true;
    }
    else if (name == "1280x1024") {
        outputSizeWidth=1280;
        outputSizeHeight=1024;
        outputWidescreen=false;
    }
    else if (name == "1440x900") {
        outputSizeWidth=1440;
        outputSizeHeight=900;
        outputWidescreen=true;
    }
    else if (name == "1600x1200") {
        outputSizeWidth=1600;
        outputSizeHeight=1200;
        outputWidescreen=false;
    }
    else if (name == "1920x1080") {
        outputSizeWidth=1920;
        outputSizeHeight=1080;
        outputWidescreen=true;
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (bOptionsDone) mainWindow->keyPressed(key);    
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
    if (bOptionsDone) {
        
        //imposto dimensioni e posizione della preview:
        int altezza, larghezza, posizioneX, posizioneY;
        
        if (mainWindow->elemMix.getOutputMode()==ELM_STEREO_LEFTRIGHT) larghezza = 300;
        else larghezza = 600;
        posizioneX = 650;
        
        if (mainWindow->elemMix.wideScreenPreview) 
        {
            if (mainWindow->elemMix.getOutputMode()==ELM_STEREO_TOPBOTTOM) altezza = 169;
            else altezza = 338;
            posizioneY = 136;
        }
        else
        {
            if (mainWindow->elemMix.getOutputMode()==ELM_STEREO_TOPBOTTOM) altezza = 225;
            else altezza = 450;
            posizioneY = 80;        
        }
        
        //controllo se ho premuto dentro l'area della preview:
        if (x>posizioneX && x<posizioneX+larghezza && y>posizioneY && y<posizioneY+altezza) 
        {
            
            //faccio le proporzioni:
            x = (x-posizioneX)*outputSizeWidth/larghezza;
            y = (y-posizioneY)*outputSizeHeight/altezza;
            
            //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO:
            if (mainWindow->elemSy.isSelected==true && mainWindow->elemSy.isWarpable==true) mainWindow->elemSy.warper.mouseDragged(x, y, button);
            else if (mainWindow->elemV1.isSelected==true && mainWindow->elemV1.isWarpable==true) mainWindow->elemV1.warper.mouseDragged(x, y, button);
            else if (mainWindow->elemImg2.isSelected==true && mainWindow->elemImg2.isWarpable==true) mainWindow->elemImg2.warper.mouseDragged(x, y, button);     
            
        }
        
    }
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
   
     if (bOptionsDone) {
         
    //imposto dimensioni e posizione della preview:
    int altezza, larghezza, posizioneX, posizioneY;
    
         if (mainWindow->elemMix.getOutputMode()==ELM_STEREO_LEFTRIGHT) larghezza = 300;
         else larghezza = 600;

    posizioneX = 650;
   
         if (mainWindow->elemMix.wideScreenPreview) 
         {
             if (mainWindow->elemMix.getOutputMode()==ELM_STEREO_TOPBOTTOM) altezza = 169;
             else altezza = 338;
             posizioneY = 136;
         }
         else
         {
             if (mainWindow->elemMix.getOutputMode()==ELM_STEREO_TOPBOTTOM) altezza = 225;
             else altezza = 450;
             posizioneY = 80;        
         }
        
    //controllo se ho premuto dentro l'area della preview:
    if (x>posizioneX && x<posizioneX+larghezza && y>posizioneY && y<posizioneY+altezza) 
    {
    
        //faccio le proporzioni:
        x = (x-posizioneX)*outputSizeWidth/larghezza;
        y = (y-posizioneY)*outputSizeHeight/altezza;
        
        //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO:
        if (mainWindow->elemSy.isSelected==true && mainWindow->elemSy.isWarpable==true) mainWindow->elemSy.warper.mousePressed(x, y, button);
        else if (mainWindow->elemV1.isSelected==true && mainWindow->elemV1.isWarpable==true) mainWindow->elemV1.warper.mousePressed(x, y, button);
        else if (mainWindow->elemImg2.isSelected==true && mainWindow->elemImg2.isWarpable==true) mainWindow->elemImg2.warper.mousePressed(x, y, button);     
        
    }
    
     }
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
}


//--------------------------------------------------------------
void testApp::firstSetup() {
    
    mainWindow = new MainWindow();
    mainWindow->outputResolutionX = outputSizeWidth;
    mainWindow->outputResolutionY = outputSizeHeight;
    mainWindow->outputRatioWide = outputWidescreen;
    mainWindow->setup();
    
	ofBackground(0,0,0);
    
    createOutputWindow();
    
    
}

//--------------------------------------------------------------
void testApp::createOutputWindow(){
    
    //from ofxFanster example: per mandare la seconda finsetra sul secondo schermo, se c'
/*     ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
     ofxDisplay* disp = displays[0];
     if(displays.size() > 1) disp = displays[1];
     ofxFensterManager::get()->setActiveDisplay(disp); */
    
    outputWindow = new OutputWindow(this);
    output = ofxFensterManager::get()->createFenster(400, 200, 400, 300, OF_WINDOW);
    output->addListener(outputWindow);
    output->setWindowTitle("Output");
    
    outputFullScreen = false;
    
}
