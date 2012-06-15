#ifndef ELEMENTMIXER
#define ELEMENTMIXER


#include "ofMain.h"
#include "element.h"



// 
#define ELM_STEREO_OPENGL		0
#define ELM_STEREO_ANAGLYPH		1
#define ELM_STEREO_LEFTRIGHT	2
#define ELM_STEREO_TOPBOTTOM	3
#define ELM_STEREO_MONO			4


class elementMixer : public element
{
	public :
	
	elementMixer(){};
	void update();
	void setup(int _width, int _height, int _stereoMode,vector<element*>* _elements,int* _elementsOrder, int* _elementsBlendModes);
	void setOutputStereoMode(int _stereoMode);
	void drawIntoFbo(bool _drawMonoOrStereo);
	void drawOutput(int _x, int _y,int _width, int _height);
	void drawQuadGeometry();
	
	void drawInfo();


	private :

	vector<element*>	sceneElements;
	int*				elementsOrder;
	vector<float*>		elementsOpacity;
	int*				elementsBlendModes;
	int					blendMode;
	ofShader			shader;
	int					outputStereoMode;
	
	// used to help on drawing ...
	ofImage				blacktexture;
	
	private : 
	
	void				updateOpacity();
	
};

#endif;