#include "elementMask.h"



//-----------------------------------------------------------------
void elementMask::setup(int _maskWidth, int _maskHeight, string _loadPath)
{
	isStereo = false;
	drawInStereo = isStereo;
    		
    elementWidth = _maskWidth;
    elementHeight= _maskHeight;
    
	this->init(0,elementWidth,elementHeight,GL_RGBA);	
    
    bMouseDraw  =   false;
    bMouseErase =   false;
    bDrawCurved =   false;
    
    nPunti =0;
}

//-----------------------------------------------------------------
void elementMask::update()
{
    nPunti  =   myMask.size();
}

//-----------------------------------------------------------------
void elementMask::drawIntoFbo(bool _drawMonoOrStereo)
{

	drawInStereo = false;
	
	fboLeft.begin();
	
    ofBackground(0,0,0,0);
	ofSetColor(255,255,255,255);
    
    //sfondo nero
    ofPushMatrix();
    ofSetColor(0, 0, 0, 255);
    ofRect(0,0,1920,1080);
    ofPopMatrix();
    
        //maschera bianca trasparente
    if (nPunti>1) {
        
        if (bDrawCurved) {
            
            
            //disegna il contorno con una linea curva
            ofPushStyle();
            ofSetColor(255,255,255, 255);
            ofFill();
            ofBeginShape();
            
            for (int i = 0; i < nPunti; i++){
                
                if (i == 0){
                    ofCurveVertex(myMask[0].x, myMask[0].y); 
                    ofCurveVertex(myMask[0].x, myMask[0].y); 
                } else if (i == nPunti-1){
                    ofCurveVertex(myMask[i].x, myMask[i].y);
                    ofCurveVertex(myMask[0].x, myMask[0].y);
                    ofCurveVertex(myMask[0].x, myMask[0].y);
                } else {
                    ofCurveVertex(myMask[i].x, myMask[i].y);
                }
            }
            
            ofEndShape();
            ofPopStyle();
        } 
        
        else 
            
        {
            
            //disegna il contorno come linea spezzata
            ofPushStyle();
            ofSetColor(255,255,255,255);
            ofFill();
            ofBeginShape();
            for (int i = 0; i < nPunti; i++){
                ofVertex(myMask[i].x, myMask[i].y);
            }
            ofVertex(myMask[nPunti-1].x, myMask[nPunti-1].y);            
            ofEndShape(true);
            ofPopStyle();
        }
    }
    
    drawDraggableVertex();
    
    fboLeft.end();

}


//-----------------------------------------------------------------
void elementMask::drawDraggableVertex()
{
    //disegna i pallini sui vertici
    ofPushStyle();
    //    ofSetColor(255,255,255, 255);
    for (int i = 0; i < nPunti; i++){
        if (myMask[i].bOver == true) {
            ofSetColor(255,0, 0, 255);
        }
        else ofSetColor(255,255,255,255); 
        ofFill();   
        ofCircle(myMask[i].x, myMask[i].y,4);
    }
    ofPopStyle();

}


//------------- -------------------------------------------------
void elementMask::mouseMoved(int x, int y ){
    
    if (nPunti<0) {
        
        for (int i = 0; i < nPunti; i++){
            float diffx = x - myMask[i].x;
            float diffy = y - myMask[i].y;
            float dist = sqrt(diffx*diffx + diffy*diffy);
            if (dist < myMask[i].radius){
                myMask[i].bOver = true;
            } else {
                myMask[i].bOver = false;
            }	
        }
    }
}

//--------------------------------------------------------------
void elementMask::mouseDragged(int x, int y, int button){
	for (int i = 0; i < nPunti; i++){
		if (myMask[i].bBeingDragged == true){
			myMask[i].x = x;
			myMask[i].y = y;
		}
	}
}

//--------------------------------------------------------------
void elementMask::mousePressed(int x, int y, int button){
	for (int i = 0; i < nPunti; i++){
		float diffx = x - myMask[i].x;
		float diffy = y - myMask[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < myMask[i].radius){
            if (bMouseErase) myMask.erase(myMask.begin()+i);
            else myMask[i].bBeingDragged = true;
		} else {
			myMask[i].bBeingDragged = false;
		}	
	}
}

//--------------------------------------------------------------
void elementMask::mouseReleased(int x, int y, int button){
    
	for (int i = 0; i < nPunti; i++){
		myMask[i].bBeingDragged = false;	
	}
    
    if (bMouseDraw) {
        draggableVertex _newVertex;
        _newVertex.x                =x;
        _newVertex.y                =y;
        _newVertex.bOver 			= false;
        _newVertex.bBeingDragged 	= false;
        _newVertex.radius           = 4;
        myMask.push_back(_newVertex);
    }
    
}

