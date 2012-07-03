#ifndef ELEMENTMIXER
#define ELEMENTMIXER


#include "ofMain.h"
#include "element.h"



// 
#define ELM_STEREO_OPENGL		0
#define ELM_STEREO_ANAGLYPH		1
#define ELM_STEREO_MONO			2
#define ELM_STEREO_LEFTRIGHT	3
#define ELM_STEREO_TOPBOTTOM	4


class elementMixer : public element
{
	public :
	
	elementMixer(){};
	void update();
	void setup(int _width, int _height, int _stereoMode,element** _elements,int _numOfElements,int* _elementsOrder,int _posX, int _posY,string _name);
	void setOutputStereoMode(int _stereoMode);
	void drawIntoFbo(bool _drawMonoOrStereo);
	void drawOutput(int _x, int _y,int _width, int _height);
	void drawLeft(int x, int y, int w, int h){};
	void drawRight(int x, int y, int w, int h){};
	ofTexture& getLeftTexture(){};
	ofTexture& getRightTexture(){};
	
	void drawQuadGeometry();
	void drawInfo();
    
    void guiEvent(ofxUIEventArgs &e);



	bool				useNoShader;
    void                applyFX();
    void                addFX(int type);       // Mauro
    void drawPreview(int x, int y, int w, int h);
	private :

	element**			sceneElements;
	int					numOfElements;
	int*				elementsOrder;
	int*				elementsBlendModes;
	float*				elementsOpacity;

	int					blendMode;
	ofShader			shader;
	int					outputStereoMode;
	
	// used to help on drawing ...
	ofImage				blacktexture;
	
	void				updateOpacity();
	
};

#endif;