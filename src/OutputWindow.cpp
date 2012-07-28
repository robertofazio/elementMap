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
    comandi ="element.map alpha 0.0.2\n\n'w'\t\tactivate/deactivate warp\n't'\t\tactivate/deactivate translate\n\n'z'/'x'\tincrease/decrease grid X resolution\n'q'/'a'\tincrease/decrease grid Y resolution\n'n'/'m'\tselect previous/next point\n'v'\t\tselect quad vertex\n'h'\t\thold to select multiple grid points\n'c'\t\tclear quad warp transformation\n'r'\t\treset point position\n\n'g'\t\tshow/hide mesh grid\n's'\t\tsave warp to xml\n'l'\t\tload warp from xml\n\n'l'\treturn\t main window hide/show GUI\n\n'f'\t\t in second window fullscreen model\n\n\nall working with arrow keys;\n quad warping support mouse drag too\n\nSPACEBAR\tplay/pause video\nBACKSPACE\trewind video";
}

void OutputWindow::windowResized(int &w, int &h)
{
    cout << "RESIZED OUTPUT" << endl;
}

void OutputWindow::keyPressed(int key, ofxFenster* window) 
{              
    cout << "KEY PRESSED OUTPUT" << endl;
    cout << key << endl;
    
    switch (key) {
         
        case 102: //'f'
            ofToggleFullscreen();
            
            if (ofGetWindowMode() == OF_WINDOW)
            {
                ofSetWindowPosition(ofGetWindowPositionX(), 10);
                ofToggleFullscreen();
                ofSetWindowPosition(ofGetWindowPositionX(), 0);
                ofToggleFullscreen();
            }
            
            break;
            
        case 48: // '0' = deselect all layers
            mainScene->mainWindow->elemImg2.isSelected=false;
            mainScene->mainWindow->elemSy.isSelected=false;
            mainScene->mainWindow->elemV1.isSelected=false;
            cout << "deselect" << endl;
            break;
            
        case 49: // '1' = select Syphon element
            mainScene->mainWindow->elemImg2.isSelected=false;
            mainScene->mainWindow->elemSy.isSelected=true;
            mainScene->mainWindow->elemV1.isSelected=false;
            cout << "syphon selected" << endl;
            break;
            
        case 50: // '2' = select Video element
            mainScene->mainWindow->elemImg2.isSelected=false;
            mainScene->mainWindow->elemSy.isSelected=false;
            mainScene->mainWindow->elemV1.isSelected=true;
            cout << "video selected" << endl;                
            break;
            
        case 51: // '3' = select Image element
            mainScene->mainWindow->elemImg2.isSelected=true;
            mainScene->mainWindow->elemSy.isSelected=false;
            mainScene->mainWindow->elemV1.isSelected=false;
            cout << "image selected" << endl;
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
    
    ofTexture   text;
    
    ofSetColor(255, 255, 255);
    
    //draw directly mixer fbo
    if(mainScene->mainWindow->elemMix.getOutputStereoMode() == ELM_STEREO_ANAGLYPH)
        mainScene->mainWindow->elemMix.fboAnagliph.draw(0, 0);    
    else
        mainScene->mainWindow->elemMix.fboLeft.draw(0,0);
    
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10,10);
    
}
void OutputWindow::mouseDragged(int x, int y, int button,  ofxFenster* f)
{
    //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO: come sopra...
    if (mainScene->mainWindow->elemSy.isSelected==true && mainScene->mainWindow->elemSy.isWarpable==true) mainScene->mainWindow->elemSy.warper.mouseDragged(x, y, button);
    else if (mainScene->mainWindow->elemV1.isSelected==true && mainScene->mainWindow->elemV1.isWarpable==true) mainScene->mainWindow->elemV1.warper.mouseDragged(x, y, button);
    else if (mainScene->mainWindow->elemImg2.isSelected==true && mainScene->mainWindow->elemImg2.isWarpable==true) mainScene->mainWindow->elemImg2.warper.mouseDragged(x, y, button);
    
}

void OutputWindow::setup()
{
}

void OutputWindow::mousePressed(int x, int y, int btn, ofxFenster* f)
{
    cout << "MOUSE PRESSED OUTPUT" << endl;
    //    cout << x << endl;
    //    cout << y << endl;
    //    cout << btn << endl;
    
    //MANDO I COMANDI AL WARPER DEL LIVELLO SELEZIONATO: come sopra...
    if (mainScene->mainWindow->elemSy.isSelected==true && mainScene->mainWindow->elemSy.isWarpable==true) mainScene->mainWindow->elemSy.warper.mousePressed(x, y, btn);
    else if (mainScene->mainWindow->elemV1.isSelected==true && mainScene->mainWindow->elemV1.isWarpable==true) mainScene->mainWindow->elemV1.warper.mousePressed(x, y, btn);
    else if (mainScene->mainWindow->elemImg2.isSelected==true && mainScene->mainWindow->elemImg2.isWarpable==true) mainScene->mainWindow->elemImg2.warper.mousePressed(x, y, btn);
    
}