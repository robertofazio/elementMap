#ifndef ELEMENTMASK
#define ELEMENTMASK


#include "ofMain.h"
#include "element.h"


typedef struct {
    
	float 	x;
	float 	y;
	bool 	bBeingDragged;
	bool 	bOver;
	float 	radius;
	
}draggableVertex;


class elementMask : public element
{
	public :
	
	elementMask(){};
	
	void setup(int _maskWidth, int _maskHeight, string _loadPath);
	void drawIntoFbo(bool _drawMonoOrStereo);
	void update();
    
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void drawDraggableVertex();
    
    vector<draggableVertex>     myMask;
    int                         nPunti;
    
    bool                        bDrawCurved;
    bool                        bMouseDraw;
    bool                        bMouseErase;

};

#endif;