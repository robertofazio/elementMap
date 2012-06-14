#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofxSimpleGuiSliderBase.h"


class ofxSimpleGuiTimeCode : public ofxSimpleGuiSliderBase<int> {
	
public:
	ofxSimpleGuiTimeCode(string name, int &value, int &_fps, int &_duration) : ofxSimpleGuiSliderBase<int>(name, value, min, max) {
		controlType = "SliderInt";
		setIncrement(1);
		fps=&_fps;
		numberOfFrames=&_duration;
	}
	
	int *fps;
	int *numberOfFrames;

	void setup();
	void draw(float x, float y);
	
	
	
	void onPress(int x, int y, int button) {
	}
	
	void onDragOver(int x, int y, int button) {
	}
	
	void onDragOutside(int x, int y, int button) {
	}
	
	
	
	void onKeyRight() {
	}
	
	void onKeyLeft() {
	}
	
	void onKeyUp() {
	}
	
	void onKeyDown() {
	}
	

};
