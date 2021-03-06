//
//  OutputWindow.cpp
//  elementStereoInteractive
//
//  Created by mauro on 12/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include "OutputWindow.h"
#include "testApp.h"
#include "ofEvents.h"

OutputWindow::OutputWindow(testApp* _mainScene)
{
    mainScene = _mainScene;
}

void OutputWindow::windowResized(int &w, int &h)
{
}

void OutputWindow::keyPressed(int key, ofxFenster* window) 
{              
    cout << "OUTPUT WINDOW KEY" << endl;
    
    switch (key) {
         
        case 102: //'f'
            mainScene->mainWindow->deselectAllElements();

            ofxFensterManager::get()->getWindowById(1)->toggleFullscreen();
            
            if (ofxFensterManager::get()->getWindowById(1)->getWindowMode() == OF_WINDOW)
            {
                ofxFensterManager::get()->getWindowById(1)->setWindowShape(400, 300);
                ofxFensterManager::get()->getWindowById(1)->setWindowPosition(400, 200);
            }
            
            break;
            
        case 48: // '0' = deselect all layers
            mainScene->mainWindow->deselectAllElements();
            break;
            
        case 49: // '1' = select Syphon element
            mainScene->mainWindow->deselectAllElements();
            mainScene->mainWindow->elemSy.isSelected=true;
            break;
            
        case 50: // '2' = select Video element
            mainScene->mainWindow->deselectAllElements();
            mainScene->mainWindow->elemV1.isSelected=true;  
            break;
            
        case 51: // '3' = select Image element
            mainScene->mainWindow->deselectAllElements();
            mainScene->mainWindow->elemImg2.isSelected=true;
            break;
            
        default:
            break;
    }
    
    //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO:
    //da rendere un po' più dinamico, una volta che il mixer sarà definitivo o quasi;
    //adesso è così tanto per provare velocemente se funziona...
    if (mainScene->mainWindow->elemSy.isSelected==true && mainScene->mainWindow->elemSy.isWarpable==true) mainScene->mainWindow->elemSy.warper.warpKeyPressedHandler(key);
    else if (mainScene->mainWindow->elemV1.isSelected==true && mainScene->mainWindow->elemV1.isWarpable==true) mainScene->mainWindow->elemV1.warper.warpKeyPressedHandler(key);
    else if (mainScene->mainWindow->elemImg2.isSelected==true && mainScene->mainWindow->elemImg2.isWarpable==true) mainScene->mainWindow->elemImg2.warper.warpKeyPressedHandler(key);
    
    
    
}

void OutputWindow::keyReleased(int key, ofxFenster *window)
{
    if (mainScene->mainWindow->elemSy.isSelected==true && mainScene->mainWindow->elemSy.isWarpable==true) mainScene->mainWindow->elemSy.warper.warpKeyReleasedHandler(key);
    else if (mainScene->mainWindow->elemV1.isSelected==true && mainScene->mainWindow->elemV1.isWarpable==true) mainScene->mainWindow->elemV1.warper.warpKeyReleasedHandler(key);
    else if (mainScene->mainWindow->elemImg2.isSelected==true && mainScene->mainWindow->elemImg2.isWarpable==true) mainScene->mainWindow->elemImg2.warper.warpKeyReleasedHandler(key);
    
}

void OutputWindow::update()
{
    mainScene->mainWindow->	elemMix.update();
    
}

void OutputWindow::setWindowShape(int w, int h)
{
}

void OutputWindow::draw()
{
    ofBackground(0, 0, 0); 
    
    ofPushStyle();
    ofSetColor(255, 255, 255, ofMap(mainScene->mainWindow->elemMix.getOpacity(), 0, 1, 0, 255));
        
    ofPoint dimensioni = ofxFensterManager::get()->getWindowById(1)->getWindowSize();
   
    if(mainScene->mainWindow->elemMix.getOutputMode() != ELM_STEREO_OPENGL) glDrawBuffer(GL_BACK);
    mainScene->mainWindow->elemMix.drawOutput(0, 0, dimensioni.x, dimensioni.y);
        
    ofPopStyle();
    
}
void OutputWindow::mouseDragged(int x, int y, int button,  ofxFenster* f)
{
//    //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO: come sopra...
//    if (mainScene->mainWindow->elemSy.isSelected==true && mainScene->mainWindow->elemSy.isWarpable==true) mainScene->mainWindow->elemSy.warper.mouseDragged(x, y, button);
//    else if (mainScene->mainWindow->elemV1.isSelected==true && mainScene->mainWindow->elemV1.isWarpable==true) mainScene->mainWindow->elemV1.warper.mouseDragged(x, y, button);
//    else if (mainScene->mainWindow->elemImg2.isSelected==true && mainScene->mainWindow->elemImg2.isWarpable==true) mainScene->mainWindow->elemImg2.warper.mouseDragged(x, y, button);    
}

void OutputWindow::setup()
{
}

void OutputWindow::mousePressed(int x, int y, int btn, ofxFenster* f)
{
//    cout << "mouse pressed sentito da OutputWindow" << endl;
//    ofPoint dimensioni = ofxFensterManager::get()->getWindowById(1)->getWindowSize();
//    cout << "finestra output: " << dimensioni.x << "x" << dimensioni.y << endl;
//    cout << x << ";" << y << endl;
//    x*=ofGetScreenWidth()/dimensioni.x;
//    y*=ofGetScreenHeight()/dimensioni.y;
//    cout << x << ";" << y << endl;
//    
//    //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO: come sopra...
//    if (mainScene->mainWindow->elemSy.isSelected==true && mainScene->mainWindow->elemSy.isWarpable==true) mainScene->mainWindow->elemSy.warper.mousePressed(x, y, btn);
//    else if (mainScene->mainWindow->elemV1.isSelected==true && mainScene->mainWindow->elemV1.isWarpable==true) mainScene->mainWindow->elemV1.warper.mousePressed(x, y, btn);
//    else if (mainScene->mainWindow->elemImg2.isSelected==true && mainScene->mainWindow->elemImg2.isWarpable==true) mainScene->mainWindow->elemImg2.warper.mousePressed(x, y, btn);
}

