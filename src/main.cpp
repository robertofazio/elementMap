/*
 #include "testApp.h"
 #include "ofAppGlutWindow.h"
 
 //--------------------------------------------------------------
 int main(){
 ofAppGlutWindow window; // create a window
 // set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
 ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
 ofRunApp(new testApp()); // start the app
 }
 
 */

#include "ofMain.h"
#include "testApp.h"
#include "ofxFensterManager.h"
#include "ofAppGlutWindow.h"




//========================================================================
int main( ){
//	ofSetupOpenGL(ofxFensterManager::get(), 1280, 800, OF_FULLSCREEN);			
	ofSetupOpenGL(ofxFensterManager::get(), 1280, 800, OF_WINDOW);			
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunFensterApp(new testApp());
}

