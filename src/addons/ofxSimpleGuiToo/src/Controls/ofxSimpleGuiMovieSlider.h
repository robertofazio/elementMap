#pragma once

#include "ofxSimpleGuiControl.h"


//------------------------------------------------------------------------------ Movie slider
class ofxSimpleGuiMovieSlider : public ofxSimpleGuiControl {
public:
	float				barwidth;
	ofVideoPlayer		*input;
	float				videoLength;
	float				videoTotalNumFrames;
	float				videoFrameRate;
	float				videoFramePosition;
	
	ofxSimpleGuiMovieSlider(string name, ofVideoPlayer& input); 
	void setup();
	void update();
	void onDragOver(int x, int y, int buton);
	void onPress(int x, int y, int button);
	void onRelease();
	void draw(float x, float y);	
};
 
