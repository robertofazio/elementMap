
#ifndef ELEMENT_H
#define ELEMENT_H

#include "ofMain.h"
#include "elementUIBase.h"

class element : public ofNode, public elementUIBase 
{
	public :
	
	element(){};
	
	void init(int _type,int _width, int _height,int internalformat,string _name);
	virtual void update() =0;
	virtual void drawIntoFbo(bool _drawMonoOrStereo) =0;
	
	void			setIsStereo(bool _b);
	void			setDrawInStereo(bool _b);
	void			setWidth(int _i);
	void			setHeight(int i_);
	void			setInternalFormat(int _i);
	void			setSwapLeftRight(bool b);
	void			setOpacity(float f);
	void			setIsShow(bool b);
	void			setIsActive(bool b);
	void			setBlendingMode(int _i);
	
	bool			getIsStereo();
	bool			getDrawInStereo();
	int				getWidth();
	int				getHeight();
	int				getInternalFormat();
	int				getElementType();	
	bool			getSwapLeftRight();
	bool			getIsShow();
	bool			getIsActive();
	float			getOpacity();
	string			getElementName();
	int				getBlendingMode();
	
	ofFbo			fboLeft;
	ofFbo			fboRight;
	
	// set protected member to be just accesible from their derived classes (as private members are not accessible)
	
	protected :

	void			setOpacityColor();

	bool			isActive;	// indicates that we don't want this element to loose any time of cpu, so doesn't draw in preview or in draw
	bool			isShow;		// indicates we want to keep the element to preview but not draw it on the output
	bool			isClear;	// indicates if the fbo's have been cleared to black to avoid clearing on every draw
	int				elementWidth;
	int				elementHeight;
	int				blendingMode;

	
	private :
	
	int				type;
	bool			isStereo;
	int				internalFormat; //GL_RGB,GL_RGBA ...
	bool			drawInStereo;
	bool			swapLeftRight;
	float			opacity;
	string			elementName;
	
	
};

#endif;