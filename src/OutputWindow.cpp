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
    bSpeedUp = false;
    bMela = false;
    comandi ="element.map alpha 0.0.2\n\n'w'\t\tactivate/deactivate warp\n't'\t\tactivate/deactivate translate\n\n'z'/'x'\tincrease/decrease grid X resolution\n'q'/'a'\tincrease/decrease grid Y resolution\n'n'/'m'\tselect previous/next point\n'v'\t\tselect quad vertex\n'h'\t\thold to select multiple grid points\n'c'\t\tclear quad warp transformation\n'r'\t\treset point position\ncmd+r\treset all grid points\n\n'g'\t\tshow/hide mesh grid\n's'\t\tsave warp to xml\n'l'\t\tload warp from xml\n\n'l'\treturn\t main window hide/show GUI\n\n'f'\t\tin second window fullscreen model\n\n\nall working with arrow keys;\n quad warping support mouse drag too\n\nSPACEBAR\tplay/pause video\nBACKSPACE\trewind video";
}

void OutputWindow::windowResized(int &w, int &h)
{
    cout << "RESIZED OUTPUT" << endl;
}

void OutputWindow::keyPressed(int key, ofxFenster* window) 
{              
    cout << "KEY RELESED OUTPUT" << endl;
    cout << key << endl;
    
        switch (key) {
                
            case 115: //'s':
                cout << "Saved" << endl;
                mainOutputWarp.save();
                break;
                
            case 108: //'l':
                mainOutputWarp.load();
                break;
            case 102:
                ofToggleFullscreen();
                
                
                if(ofGetWindowMode() == OF_WINDOW)
                {
                    ofSetWindowPosition(ofGetWindowPositionX(), 10);
                    ofToggleFullscreen();
                    ofSetWindowPosition(ofGetScreenWidth(), 0);
                    ofToggleFullscreen();   
                }
                break;
                
             /*   
            case ' ':
                if (elemV1.leftChannelPlayer.isPlaying()) 
                {
                    elemV1.leftChannelPlayer.stop();
                    elemV1.rightChannelPlayer.stop();
                    
                } 
                else
                {
                    elemV1.leftChannelPlayer.play();
                    elemV1.rightChannelPlayer.play();
                } 
                
                break;
                
            case OF_KEY_BACKSPACE:
                elemV1.leftChannelPlayer.play();
                elemV1.rightChannelPlayer.play();
                elemV1.leftChannelPlayer.setPosition(0.0);
                elemV1.rightChannelPlayer.setPosition(0.0);
                elemV1.leftChannelPlayer.stop();
                elemV1.rightChannelPlayer.stop();
                */
                break;
                
            case 119: //'w':
                mainOutputWarp.bViewGrid=false;
                mainOutputWarp.deselectAll();
                mainOutputWarp.quadWarp.toggleShow();
                mainOutputWarp.bWarpActive=!mainOutputWarp.bWarpActive;
                break;     
                
            case 103: //'g':
                mainOutputWarp.bWarpActive=false;
                mainOutputWarp.quadWarp.hide();
                mainOutputWarp.deselectAll();
                mainOutputWarp.bViewGrid=!mainOutputWarp.bViewGrid;
                break;     
                
            case 116: //'t':
                mainOutputWarp.bSposta=!mainOutputWarp.bSposta;
                mainOutputWarp.selectMainCorners();                
                break;
                
            case 120: // 'x':
                mainOutputWarp.increaseXgrid();
                break;
                
                
            case 122: //'z':
                mainOutputWarp.decreaseXgrid();
                break;
                
                
            case 113: //'q':
                mainOutputWarp.increaseYgrid();
                break;
                
                
            case 97: //'a':
                mainOutputWarp.decreaseYgrid();
                break;
                
                
            case 269: //OF_KEY_UP
                if (bSpeedUp) mainOutputWarp.pointUP(40);
                else mainOutputWarp.pointUP(1);
                break;
                
            case 270 ://OF_KEY_DOWN:
                if (bSpeedUp) mainOutputWarp.pointDOWN(40);
                else mainOutputWarp.pointDOWN(1);
                break;
                
            case 267: //OF_KEY_LEFT:
                if (bSpeedUp) mainOutputWarp.pointLEFT(40);
                else mainOutputWarp.pointLEFT(1);
                break;
                
            case 268: //OF_KEY_RIGHT:
                if (bSpeedUp) mainOutputWarp.pointRIGHT(40);
                else mainOutputWarp.pointRIGHT(1);
                break;
                
            case 109: //'m':
                mainOutputWarp.selectNextPoint();
                break;
                
            case 110: //'n':
                mainOutputWarp.selectPrevPoint();
                break;
                
                
            case 118: //'v':
                mainOutputWarp.selectNextMainCorner();
                break;
                
                
            case 114: //'r':
                if (bMela) 
                {
                    mainOutputWarp.increaseXgrid();
                    mainOutputWarp.decreaseYgrid();
                    
                }
                else mainOutputWarp.resetPoint();
                break;
                
            case 99: //'c':
                mainOutputWarp.resetCorners();
                break;

            case 104: //'h':
                mainOutputWarp.bHoldSelection=true;
                break;
                
            case 60: //'<':
                bSpeedUp=true;
                break;
                
            case 262: // command
                bMela=true;
                break;
                
            default:
                break;
        }
}

void OutputWindow::keyReleased(int key, ofxFenster *window)
{
    switch (key) {
        case 104: // 'h':
            mainOutputWarp.bHoldSelection=false;
            break;
            
        case 60: // '<':
            bSpeedUp=false;
            break;
            
        case 262: // command
            bMela=false;
            break;

    }
}

void OutputWindow::update()
{
	mainOutputWarp.updateCoordinates();
}

void OutputWindow::setWindowShape(int w, int h)
{
}

void OutputWindow::draw()
{
    ofBackground(0, 0, 0); 
    
    ofTexture   text;
    
    ofSetColor(255, 255, 255);
    //load texture from mixer fbo 
    if(mainScene->mainWindow->elemMix.getOutputStereoMode() == ELM_STEREO_ANAGLYPH)
        text = mainScene->mainWindow->elemMix.fboAnagliph.getTextureReference();
    else
        text = mainScene->mainWindow->elemMix.fboLeft.getTextureReference();
    
    mainOutputWarp.warp(text);
    
    
}
void OutputWindow::mouseDragged(int x, int y, int button,  ofxFenster* f)
{
    mainOutputWarp.mouseDragged(x, y, button);
}

void OutputWindow::setup()
{
}

void OutputWindow::mousePressed(int x, int y, int btn, ofxFenster* f)
{
    cout << "MOUSE PRESSED OUTPUT" << endl;
    cout << x << endl;
    cout << y << endl;
    cout << btn << endl;
    mainOutputWarp.mousePressed(x, y, btn);
    
}