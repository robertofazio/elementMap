
#ifndef ELEMENT_H
#define ELEMENT_H

#include "ofMain.h"

class element : public ofNode
{
	public :
	
	element(){};
	
	void init(int _type,int _width, int _height,int internalformat);
	virtual void update() =0;
	virtual void drawIntoFbo(bool _drawMonoOrStereo) =0;
	
	void			setIsStereo(bool _b);
	void			setDrawInStereo(bool _b);
	void			setWidth(int _i);
	void			setHeight(int i_);
	void			setInternalFormat(int _i);
	void			setSwapLeftRight(bool b);
	
	bool			getIsStereo();
	bool			getDrawInStereo();
	int				getWidth();
	int				getHeight();
	int				getInternalFormat();
	int				getElementType();	
	bool			getSwapLeftRight();

	ofFbo			fboLeft;
	ofFbo			fboRight;
	
	
	
	private :
	
	int				type;
	int				elementWidth;
	int				elementHeight;
	bool			isStereo;
	bool			drawInStereo;
	bool			swapLeftRight;
	int				internalFormat; //GL_RGB,GL_RGBA ...

};

#endif;