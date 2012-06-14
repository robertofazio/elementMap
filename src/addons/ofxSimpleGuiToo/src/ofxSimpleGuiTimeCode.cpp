
#include "ofxSimpleGuiTimeCode.h"


void ofxSimpleGuiTimeCode::setup() {
	setSize(config->gridSize.x - config->padding.x, config->titleHeight);
}


//-------------------------------

void ofxSimpleGuiTimeCode::draw(float x, float y) {
	//update postion of gui object
	setPos(x, y);
	//VALUE CLAMP
	barwidth = ofMap(float(*value), 0.0, float(*numberOfFrames), 0.0, (float)width);
	if(barwidth > width) barwidth = width;
	else if(barwidth < 0) barwidth = 0;
	
	ofEnableAlphaBlending();
	glPushMatrix();
	glTranslatef(x, y, 0);
	ofFill();
	
	setEmptyColor();
	ofRect(0, 0, width, config->sliderHeight);
	
	
	setFullColor();
	ofRect(0, 0, barwidth, config->sliderHeight);
	
	setTextBGColor();
	ofRect(0, config->sliderHeight, width, config->sliderTextHeight);
	

	setTextColor();
	
	int frames = int(*value);
	div_t divresult;
	div_t divresult2;
	div_t divresult3;

	divresult = div (frames,*fps);
	divresult2 = div(divresult.quot,60);
	divresult3 = div(divresult2.quot,60);

	string s = ofToString(divresult3.quot)+ "h: " + ofToString(divresult2.quot%60) + "m :" + ofToString(divresult.quot%60) + "s :" + ofToString(divresult.rem) + "f";
	ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	ofDisableAlphaBlending();
	glPopMatrix();

}




