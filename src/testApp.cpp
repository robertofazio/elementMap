#include "testApp.h"

float lastTime=0.0f;
float medianFrameStep;
int num=0;

int margin = 10;

#define PREVIEW_WIDTH 100
#define PREVIEW_HEIGHT 75

//--------------------------------------------------------------
void testApp::setup()
{	
	
	outputResolutionX	= ofGetScreenWidth();
	outputResolutionY	= ofGetScreenHeight();
	drawPreviews		= true;
	drawUIs				= true;
	
    
	ofEnableAlphaBlending();
	
	// test that GL_STEREO is working on this machine
	GLboolean isStereoCapable = false;
	glGetBooleanv(GL_STEREO,&isStereoCapable);
	GLint maxVertexTextureImageUnits;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxVertexTextureImageUnits);

	if (isStereoCapable) printf(">> GL_STEREO OK \n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
	else printf(">> GL_STEREO KO !!\n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);	
	
	numOfElements = 4;
	
	int previewWidth = (ofGetWidth()-(20*numOfElements))/4;
	int previewHeight = previewWidth / (float(ofGetWidth())/float(ofGetHeight()));

	// create & setup elements on this app 
	elemImg.setup("./images/testPattern1024.jpg", "", false, -50000 , (margin * 9) - 8 ,"Test Pattern");
	elemV1.setup("./movies/left1024.mov","./movies/right1024.mov",true, 215 , (margin * 9) - 8 + (190 * 1),"Movies");
	elemImg2.setup("./images/left1024.jpg", "./images/right1024.jpg", true, 215 , (margin * 9) - 8 + (190 * 2),"Images");
	elemSy.setup("","",outputResolutionX,outputResolutionY, 215 , (margin * 9) - 8 + (190 * 0),"Syphon");
	
    bPaused=false;
    
    elemImg.UI->toggleVisible();
    elemImg.setOpacity(1);
    elemImg.setIsShow(true);
    
	// add elements to the vector
	myElements = new element*[4];
	myElements[0] = &elemImg;
	myElements[1] = &elemV1;
	myElements[2] = &elemImg2;
	myElements[3] = &elemSy;
	
//    elemImg.addFX(ELEMENT_FX_MASK);
//    elemImg2.addFX(ELEMENT_FX_MASK);
//    elemSy.addFX(ELEMENT_FX_MASK);
    
//	myElements.push_back(&elemV1);
//	myElements.push_back(&elemSy);
//	myElements.push_back(&elemImg2);
//	myElements.push_back(&elemImg);

	// setup mix stuff
	drawingOrder = new int[numOfElements];
	drawingOrder[0]=2;
	drawingOrder[1]=1;
	drawingOrder[2]=3;
	drawingOrder[3]=0;
	
	elemMix.setup(outputResolutionX,outputResolutionY,ELM_STEREO_MONO,myElements,numOfElements,drawingOrder, 650, (margin * 9) - 7,"mixer");
	
//    elemMix.addFX(ELEMENT_FX_MASK);
	//ofBackground(0, 0,60);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	
    
    bFullscreen=false;
    
    
    
    //------------WARP STUFF BEGIN ----------------
    

    //set texture w & h
    width=outputResolutionX;
    height=outputResolutionY;
    //prepare a texture 
    text.allocate(width, height, GL_RGB);
    //default grid= 4x4 control points
    xRes=4;
    yRes=4;
    //create grid coordinates
    createGrid(xRes, yRes);
    //start with active warp and transalte non-active
    bWarpActive=true;
    bSposta=false;
    bHoldSelection=false;
    //quadWarper init    
    mainVertici[0]=ofPoint(0,0);            //top left
    mainVertici[1]=ofPoint(width,0);        //top right
    mainVertici[2]=ofPoint(width,height);   //bottom right
    mainVertici[3]=ofPoint(0,height);       //bottom left
    quadWarp.setSourceRect( ofRectangle( 0, 0, width, height ) );              
    quadWarp.setTopLeftCornerPosition(mainVertici[0]);            
    quadWarp.setTopRightCornerPosition(mainVertici[1]);        
    quadWarp.setBottomRightCornerPosition(mainVertici[2]); 
    quadWarp.setBottomLeftCornerPosition(mainVertici[3]);  
    
    //auto-load last warp settings on startup
    loadXML(xRes, yRes, &vertici[0], nPoints, &texVert[0], nPoints, &screenPos[0], nPoints, &mainVertici[0], 4, &mainIndex[0], 4);
    createGrid(xRes, yRes);
    loadXML(xRes, yRes, &vertici[0], nPoints, &texVert[0], nPoints, &screenPos[0], nPoints, &mainVertici[0], 4, &mainIndex[0], 4);
    quadWarp.setTopLeftCornerPosition(mainVertici[0]);            
    quadWarp.setTopRightCornerPosition(mainVertici[1]);        
    quadWarp.setBottomRightCornerPosition(mainVertici[2]); 
    quadWarp.setBottomLeftCornerPosition(mainVertici[3]);

    
    comandi ="element.map alpha 0.0.2\n\n'w'\t\tactivate/deactivate warp\n't'\t\tactivate/deactivate translate\n\n'z'/'x'\tincrease/decrease grid X resolution\n'q'/'a'\tincrease/decrease grid Y resolution\n'n'/'m'\tselect previous/next point\n'v'\t\tselect quad vertex\n'h'\t\thold to select multiple grid points\n'c'\t\tclear quad warp transformation\n'r'\t\treset point position\n's'\t\tsave warp to xml\n'l'\t\tload warp from xml\n\n\nall working with arrow keys;\n quad warping support mouse drag too\n\nSPACEBAR\tplay/pause video\nBACKSPACE\trewind video";

    //------------WARP STUFF END ----------------

    
    verdana8.loadFont("verdana.ttf", 8);
    verdana14.loadFont("verdana.ttf", 14);
    
	verdana8.setLineHeight(12.0f);
	verdana8.setLetterSpacing(1.037);
    
	verdana14.setLineHeight(12.0f);
	verdana14.setLetterSpacing(1.037);    
    
    
    logo.loadImage("./images/logo.jpg");
    
}

//--------------------------------------------------------------
void testApp::update()
{
	elemMix.update();

    //------------WARP STUFF BEGIN ----------------

    if (bFullscreen) 
    {
    //converte le coordinate "interne" all'fbo (griglia) in coordinate dello schermo
    for (int index=0; index<nPoints; index++) {
        
        ofVec3f screen = mat.preMult( ofVec3f(vertici[index].x,vertici[index].y,0 ) );
        screenPos[index].x = screen.x;
        screenPos[index].y = screen.y;
        }
    }
    //------------WARP STUFF END ----------------
}




//--------------------------------------------------------------
void testApp::draw()
{	
    ofBackground(35, 31, 32);
	
    ofSetColor(0,0);
    elemMix.drawIntoFbo(isStereoCapable);
    
    if (bFullscreen) {
  
        ofSetColor(255, 255, 255);
        //load texture from mixer fbo (repeat for right side when we will want stereo mode....)
        text=elemMix.fboLeft.getTextureReference();
        
        mat = quadWarp.getMatrix();
        //dai vertici del warp ricavo la matrice per 
        glPushMatrix();

        glMultMatrixf(mat.getPtr());
        ofSetColor(ofColor :: white);
        //Cinzio: non so perch, ma devo prima disegnare la texture, poi pulire il buffer,
        //altrimenti non me la disegna dopo! :(
        text.draw(0,0,width,height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //bind texture
        glEnable(text.getTextureData().textureTarget);
        
        //create correspondence between control points and texture points
        int quad=1;
        int index=0;
        
        while (quad<(nQuads+1)) {
            glBegin(GL_QUADS);
            for (int vertex=0; vertex<4; vertex++) 
            {
                glTexCoord2f(texVert[index].x, texVert[index].y);
                glVertex2f(vertici[index].x, vertici[index].y);
                
                index++;
            }
            glEnd();
            quad++;
        }
        
        glDisable(text.getTextureData().textureTarget);
        
        if (bWarpActive) drawGrid();        
        glPopMatrix();

        //draw special markers for wuad warp vertices
        if (bWarpActive)
        {
            for (int corner=0; corner<4; corner++)
            {
                ofPushStyle();
                ofSetColor(ofColor :: white);
                ofSetLineWidth(2);
                ofNoFill();
                
                if (vertici[mainIndex[corner]].z==0 || bSposta) {
                    ofLine(screenPos[mainIndex[corner]].x,screenPos[mainIndex[corner]].y-24,screenPos[mainIndex[corner]].x,screenPos[mainIndex[corner]].y+24);   
                    ofLine(screenPos[mainIndex[corner]].x-24,screenPos[mainIndex[corner]].y,screenPos[mainIndex[corner]].x+24,screenPos[mainIndex[corner]].y);
                }
                else 
                {
                    if (!bSposta)
                    {
                        ofLine(0, screenPos[mainIndex[corner]].y,ofGetScreenWidth(),screenPos[mainIndex[corner]].y);
                        ofLine(screenPos[mainIndex[corner]].x,0,screenPos[mainIndex[corner]].x,ofGetScreenHeight());
                    }
                    ofSetColor(255,0,0);
                }
                if (bSposta) ofSetColor(255,0,0);

                ofRect(screenPos[mainIndex[corner]].x-12, screenPos[mainIndex[corner]].y-12, 24, 24);  
                ofPopStyle();
            }
        }

        
        
        ofPoint position(ofGetWindowWidth() - 100, ofGetWindowHeight() - 10);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), position);
    }
                    
    else 
    
    {
    
        ofSetColor(255, 255, 255);
        
        verdana14.drawString("element.Map", margin , margin * 4);
        ofSetColor(0, 255, 206);
        ofLine(650, margin * 6, ofGetWindowWidth() - 10, margin * 6);
        elemMix.drawOutput(650, margin * 8, 600, 450   );
        
        
        ofSetColor(255, 255, 255);
        verdana8.drawString("Press 'f' to enter in fullscreen mode and edit warp; when in fullscreen press 'i' for info", 10 , ofGetWindowHeight() - 10);
        
        if(drawPreviews)    
        {
            // just draw the preview inputs of mixer
            ofSetColor(255,255);
            int previewWidth = (ofGetWidth()-(20*numOfElements))/numOfElements;
            int previewHeight = previewWidth / (4.0/3.0);
            glDrawBuffer(GL_BACK);
            for(int i = 0;i<numOfElements - 1;i++)
            {
                ofSetColor(0, 255, 206);
                ofLine(margin, (margin * 6) + ((i) * 190), 645, (margin * 6) + ((i) * 190));
                ofSetColor(255, 255, 255);
                myElements[drawingOrder[i]]->drawGraphic(margin ,(margin * 7) + ((numOfElements - i -2) * 190), PREVIEW_WIDTH,PREVIEW_HEIGHT);
            }
            
        }

        logo.draw(1159, 550, 100, 100);
        
//        ofPushStyle();
//        ofSetColor(ofColor :: white);
//        ofFill();
//        ofRect(margin, 620, 200, 200);
//        ofPopStyle();
    }
    
}


                           
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if(key=='f')
	{
        drawUIs=!drawUIs;
        
        if(drawUIs)
		{	
			myElements[0]->UI->setVisible(true);
			myElements[1]->UI->setVisible(true);
			myElements[2]->UI->setVisible(true);
			myElements[3]->UI->setVisible(true);
			elemMix.UI->setVisible(true);
		}
		else 
		{
			myElements[0]->UI->setVisible(false);
			myElements[1]->UI->setVisible(false);
			myElements[2]->UI->setVisible(false);
			myElements[3]->UI->setVisible(false);
			elemMix.UI->setVisible(false);
		}
        
        bFullscreen=!bFullscreen;
        ofToggleFullscreen();
	}

    if (bFullscreen) {
    
        switch (key) {

            case 's':
                saveXML(xRes, yRes, &vertici[0], nPoints, &texVert[0], nPoints, &screenPos[0], nPoints, &mainVertici[0], 4, &mainIndex[0], 4);
                cout << "Xml Saved" << endl;
                break;

            case 'l':
                loadXML(xRes, yRes, &vertici[0], nPoints, &texVert[0], nPoints, &screenPos[0], nPoints, &mainVertici[0], 4, &mainIndex[0], 4);
                createGrid(xRes, yRes);
                loadXML(xRes, yRes, &vertici[0], nPoints, &texVert[0], nPoints, &screenPos[0], nPoints, &mainVertici[0], 4, &mainIndex[0], 4);
                quadWarp.setTopLeftCornerPosition(mainVertici[0]);            
                quadWarp.setTopRightCornerPosition(mainVertici[1]);        
                quadWarp.setBottomRightCornerPosition(mainVertici[2]); 
                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
                break;

                
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
                break;
                
            case 'w':
                bWarpActive=!bWarpActive;
                quadWarp.toggleShow();
                break;
                
            case 't':
                bSposta=!bSposta;
                if (bSposta) 
                {
                    for (int i=0; i<nPoints; i++) vertici[i].z=0;
                    vertici[mainIndex[0]].z=1;
                    vertici[mainIndex[1]].z=1;
                    vertici[mainIndex[2]].z=1;
                    vertici[mainIndex[3]].z=1;
                    
                }
                else for (int i=0; i<nPoints; i++) vertici[i].z=0;
                
                break;
                
            case 'x':
                if (bWarpActive)
                {
                    xRes++;
                    if (xRes>MAX_RESOLUTION) xRes=MAX_RESOLUTION;
                    createGrid(xRes, yRes);
                }
                break;
                
                
            case 'z':
                if (bWarpActive)
                {
                    xRes--;
                    if (xRes<2) xRes=2;
                    createGrid(xRes, yRes);
                }
                break;
                
                
            case 'q':
                if (bWarpActive)
                {
                    yRes++;
                    if (yRes>MAX_RESOLUTION) yRes=MAX_RESOLUTION;
                    createGrid(xRes, yRes);
                }
                break;
                
                
            case 'a':
                if (bWarpActive)
                {
                    yRes--;
                    if (yRes<2) yRes=2;
                    createGrid(xRes, yRes);
                }
                break;
                
                
            case OF_KEY_UP:
                if (bWarpActive)
                {
                    
                    for (int i=0; i<nPoints; i++) {
                        
                        if (vertici[i].z==1)
                        {
                            
                            //se sono sul rettangolo esterno:
                            if (i==mainIndex[0]) 
                            { 
                                mainVertici[0].y-=1;
                                quadWarp.setTopLeftCornerPosition(mainVertici[0]);
                            }
                            else if (i==mainIndex[1]) 
                            { 
                                mainVertici[1].y-=1;
                                quadWarp.setTopRightCornerPosition(mainVertici[1]);
                            }
                            else if (i==mainIndex[2]) 
                            { 
                                mainVertici[2].y-=1;
                                quadWarp.setBottomRightCornerPosition(mainVertici[2]);
                            }
                            else if (i==mainIndex[3]) 
                            { 
                                mainVertici[3].y-=1;
                                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
                            }
                            
                            //se sono in griglia:
                            else vertici[i].y-=1;
                            
                        }
                    }
                }
                break;
                
            case OF_KEY_DOWN:
                if (bWarpActive)
                {
                    
                    for (int i=0; i<nPoints; i++) {
                        
                        if (vertici[i].z==1)
                        {
                            //se sono sul rettangolo esterno:
                            if (i==mainIndex[0]) 
                            { 
                                mainVertici[0].y+=1;
                                quadWarp.setTopLeftCornerPosition(mainVertici[0]);
                            }
                            else if (i==mainIndex[1]) 
                            { 
                                mainVertici[1].y+=1;
                                quadWarp.setTopRightCornerPosition(mainVertici[1]);
                            }
                            else if (i==mainIndex[2]) 
                            { 
                                mainVertici[2].y+=1;
                                quadWarp.setBottomRightCornerPosition(mainVertici[2]);
                            }
                            else if (i==mainIndex[3]) 
                            { 
                                mainVertici[3].y+=1;
                                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
                            }
                            
                            //se sono in griglia:
                            else vertici[i].y+=1;
                        }
                    }
                }
                break;
                
            case OF_KEY_LEFT:
                if (bWarpActive)
                {
                    
                    for (int i=0; i<nPoints; i++) {
                        
                        if (vertici[i].z==1)
                        {
                            //se sono sul rettangolo esterno:
                            if (i==mainIndex[0]) 
                            { 
                                mainVertici[0].x-=1;
                                quadWarp.setTopLeftCornerPosition(mainVertici[0]);
                            }
                            else if (i==mainIndex[1]) 
                            { 
                                mainVertici[1].x-=1;
                                quadWarp.setTopRightCornerPosition(mainVertici[1]);
                            }
                            else if (i==mainIndex[2]) 
                            { 
                                mainVertici[2].x-=1;
                                quadWarp.setBottomRightCornerPosition(mainVertici[2]);
                            }
                            else if (i==mainIndex[3]) 
                            { 
                                mainVertici[3].x-=1;
                                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
                            }
                            
                            //se sono in griglia:
                            else vertici[i].x-=1;
                        }
                        
                    }
                    
                }
                break;
                
            case OF_KEY_RIGHT:
                if (bWarpActive)
                {
                    
                    for (int i=0; i<nPoints; i++) {
                        
                        if (vertici[i].z==1)
                        {
                            //se sono sul rettangolo esterno:
                            if (i==mainIndex[0]) 
                            { 
                                mainVertici[0].x+=1;
                                quadWarp.setTopLeftCornerPosition(mainVertici[0]);
                            }
                            else if (i==mainIndex[1]) 
                            { 
                                mainVertici[1].x+=1;
                                quadWarp.setTopRightCornerPosition(mainVertici[1]);
                            }
                            else if (i==mainIndex[2]) 
                            { 
                                mainVertici[2].x+=1;
                                quadWarp.setBottomRightCornerPosition(mainVertici[2]);
                            }
                            else if (i==mainIndex[3]) 
                            { 
                                mainVertici[3].x+=1;
                                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
                            }
                            
                            //se sono in griglia:
                            else vertici[i].x+=1;
                        }
                    }
                }
                break;
                
            case 'm':
                if (bWarpActive)
                {
                    int nuovaX,nuovaY;
                    
                    for (int i=0; i<nPoints; i++) {
                        
                        if (vertici[i].z==1)
                        {
                            nuovaX=texVert[i].x+(width/(xRes-1));
                            nuovaY=texVert[i].y;
                            if (nuovaX>=width) { 
                                nuovaX=0;
                                nuovaY+=height/(yRes-1);    
                            }
                            vertici[i].z=0;
                        }
                    }
                    
                    for (int i=0; i<nPoints; i++) {
                        if (abs(nuovaX-texVert[i].x)<20 && abs(nuovaY-texVert[i].y)<20) vertici[i].z=1;
                    }
                    
                }
                break;
                
                
                
            case 'n':
                if (bWarpActive)
                {
                    int nuovaX,nuovaY;
                    
                    for (int i=0; i<nPoints; i++) {
                        
                        if (vertici[i].z==1)
                        {
                            nuovaX=texVert[i].x-(width/(xRes-1));
                            nuovaY=texVert[i].y;
                            if (nuovaX<0) { 
                                nuovaX=width;
                                nuovaY-=height/(yRes-1);    
                            }
                            vertici[i].z=0;
                        }
                    }
                    
                    for (int i=0; i<nPoints; i++) {
                        if (abs(nuovaX-texVert[i].x)<20 && abs(nuovaY-texVert[i].y)<20) vertici[i].z=1;
                    }
                    
                }
                break;
                
            
            case 'v':
                
                int attivo;
                attivo=4;
                
                for (int c=0; c<4; c++) {
                    if (vertici[mainIndex[c]].z==1) attivo=c;
                }
                
                if (attivo<4) 
                {
                    vertici[mainIndex[attivo]].z=0;
                    attivo++;
                    if (attivo==4) attivo=0;
                    vertici[mainIndex[attivo]].z=1;
                }
                else
                {
                vertici[mainIndex[0]].z=1;
                }
           
                break;
                
                
            case 'r':
                if (bWarpActive)
                {
                    
                    for (int i=0; i<nPoints; i++) {
                        
                        if (vertici[i].z==1)
                        {
                            vertici[i]=texVert[i];
                        }
                    }
                }
                break;
                
            case 'c':
                mainVertici[0]=ofPoint(0,0);            //top left
                mainVertici[1]=ofPoint(width,0);        //top right
                mainVertici[2]=ofPoint(width,height);   //bottom right
                mainVertici[3]=ofPoint(0,height);       //bottom left
                quadWarp.setTopLeftCornerPosition(mainVertici[0]);            
                quadWarp.setTopRightCornerPosition(mainVertici[1]);        
                quadWarp.setBottomRightCornerPosition(mainVertici[2]); 
                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);  
                break;
                
            case 'i':
                ofSystemAlertDialog(comandi);
                break;

            case 'h':
                bHoldSelection=true;
                break;
                
    default:
        break;
        }

    }
    
 }

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    switch (key) {
        case 'h':
            bHoldSelection=false;
            break;

    }
    
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
 
    
    if (bWarpActive && !bSposta && bFullscreen )
    {
        for (int i=0; i<nPoints; i++) {
            if (vertici[i].z==1)
            {
                //se sono sul rettangolo esterno:
                if (i==mainIndex[0]) 
                { 
                    mainVertici[0]=ofPoint(x,y);
                    quadWarp.setTopLeftCornerPosition(mainVertici[0]);
                }
                else if (i==mainIndex[1]) 
                { 
                    mainVertici[1]=ofPoint(x,y);
                    quadWarp.setTopRightCornerPosition(mainVertici[1]);
                }
                else if (i==mainIndex[2]) 
                { 
                    mainVertici[2]=ofPoint(x,y);
                    quadWarp.setBottomRightCornerPosition(mainVertici[2]);
                }
                else if (i==mainIndex[3]) 
                { 
                    mainVertici[3]=ofPoint(x,y);
                    quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
                }
                
            }
        }
    }

    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    if (bWarpActive && !bSposta && bFullscreen )
    {
        
        for (int i=0; i<nPoints; i++) {
            if (abs(x-screenPos[i].x)<10 && abs(y-screenPos[i].y)<10) 
            {
                if (vertici[i].z==0) vertici[i].z=1;
            }
            else 
            {
                if (!bHoldSelection) vertici[i].z=0;      
            }
            
        }
        
    }
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


//WARP UTILITIES:
//--------------------------------------------------------------
//--------------------------------------------------------------
void testApp::createGrid(int _xRes, int _yRes){ 
    
    nQuads=(xRes-1)*(yRes-1);
    nPoints=4*nQuads;
    
    int index = 0;
    int row=0;
    int col=0;
    
    while (row<(_yRes-1)) {
        for (int ind=0; ind<4; ind++) {
            
            if (ind==0 || ind==3) vertici[index].x=col*(width/(_xRes-1));
            if (ind==1 || ind==2) vertici[index].x=(col+1)*(width/(_xRes-1));
            
            if (ind==0 || ind==1) vertici[index].y=row*(height/(_yRes-1)); 
            if (ind==2 || ind==3) vertici[index].y=(row+1)*(height/(_yRes-1)); 
            
            texVert[index]=vertici[index];
            screenPos[index]=vertici[index];
            
            index++;
        }
        col++;
        if (col>(_xRes-2)) 
        {
            row++;
            col=0;
        }
    }
    
    //assegna gli indici dei 4 vertici principali:
    for (int point=0; point<nPoints; point++) {
        if (point==0) mainIndex[0]=point;                               //top left
        else if (point==((4*(xRes-2))+1)) mainIndex[1]=point;           //top right
        else if (point==(nPoints-2)) mainIndex[2]=point;                //bottom right
        else if (point==(nPoints-1-(4*(xRes-2)))) mainIndex[3]=point;   //bottom left
        
    }
    
}



//--------------------------------------------------------------
void testApp::drawGrid() {
    
    //draw Grid Lines
    int quad=0;
    int index=0;
    
    ofPushStyle();
    ofSetColor(0,255,0,150);
    ofFill();
    ofSetLineWidth(1);
    while (quad<nQuads) {
        for (int vertex=0; vertex<4; vertex++) 
        {
            if (vertex!=0) ofLine(vertici[index-1].x, vertici[index-1].y, vertici[index].x, vertici[index].y);
            index++;
            
        }
        quad++;
    }
    ofPopStyle();
    
    
    //draw Points
    for (int point=0; point<nPoints; point++) {
        if (!(point== mainIndex[0] || point== mainIndex[1] || point== mainIndex[2] || point== mainIndex[3]))
        {
        ofPushStyle();
        if (vertici[point].z==1) ofSetColor(255,0,0,255);
        else ofSetColor(0,255,0,255);
        ofFill();
        ofCircle(vertici[point].x, vertici[point].y, 5, 5);
        ofPopStyle();
        }
    }
}



//--------------------------------------------------------------
void testApp::saveXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex)
{
    XML.addTag("warp");
    XML.pushTag("warp");
    XML.addValue("xRes", resX);
    XML.addValue("yRes", resY);
    
    XML.addTag("vertici");
    XML.pushTag("vertici");
    XML.addValue("totVertici", totVertici);
    for(int a = 0; a < totVertici; a++)
    {
        XML.addValue("values_" + ofToString(a) + "_x", vertici[a].x);
        XML.addValue("values_" + ofToString(a) + "_y", vertici[a].y);
        XML.addValue("values_" + ofToString(a) + "_z", vertici[a].z);
    }
    XML.popTag();
    
    XML.addTag("textVert");
    XML.pushTag("textVert");
    XML.addValue("totTextVert", totTextVert);
    for(int a = 0; a < totTextVert; a++)
    {
        XML.addValue("values_" + ofToString(a) + "_x", textVert[a].x);
        XML.addValue("values_" + ofToString(a) + "_y", textVert[a].y);
        XML.addValue("values_" + ofToString(a) + "_z", textVert[a].z);
    }
    XML.popTag();
    
    XML.addTag("screenPos");
    XML.pushTag("screenPos");
    XML.addValue("totScreenPos", totScreenPos);
    for(int a = 0; a < totScreenPos; a++)
    {
        XML.addValue("values_" + ofToString(a) + "_x", screenPos[a].x);
        XML.addValue("values_" + ofToString(a) + "_y", screenPos[a].y);
        XML.addValue("values_" + ofToString(a) + "_z", screenPos[a].z);
    }
    XML.popTag();
    
    XML.addTag("mainVertici");
    XML.pushTag("mainVertici");
    XML.addValue("totMainVertici", totMainVertici);
    for(int a = 0; a < totMainVertici; a++)
    {
        XML.addValue("values_" + ofToString(a) + "_x", mainVertici[a].x);
        XML.addValue("values_" + ofToString(a) + "_y", mainVertici[a].y);
        XML.addValue("values_" + ofToString(a) + "_z", mainVertici[a].z);
    }
    XML.popTag();
    
    XML.addTag("mainIndex");
    XML.pushTag("mainIndex");
    XML.addValue("totMainIndex", totMainIndex);
    for(int a = 0; a < totMainIndex; a++)
        XML.addValue("values_" + ofToString(a), mainIndex[a]);
    XML.popTag();
    
    XML.popTag();
    XML.saveFile("elementWarp.xml");
}



//--------------------------------------------------------------
void testApp::loadXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex)
{
    ofxXmlSettings tempXML;
    tempXML.loadFile("elementWarp.xml");
    
    tempXML.pushTag("warp");
    resX = tempXML.getValue("xRes", 0);
    resY = tempXML.getValue("yRes", 0);
    
    tempXML.pushTag("vertici");
    totVertici = tempXML.getValue("totVertici", 0);
    for(int a = 0; a < totVertici; a++)
    {
        vertici[a].x = tempXML.getValue("values_" + ofToString(a) + "_x", 0);
        vertici[a].y = tempXML.getValue("values_" + ofToString(a) + "_y", 0);
        vertici[a].z = tempXML.getValue("values_" + ofToString(a) + "_z", 0);
    }
    tempXML.popTag();
    
    
    tempXML.pushTag("textVert");
    totTextVert = tempXML.getValue("totTextVert", 0);
    for(int a = 0; a < totTextVert; a++)
    {
        textVert[a].x = tempXML.getValue("values_" + ofToString(a) + "_x", 0);
        textVert[a].y = tempXML.getValue("values_" + ofToString(a) + "_y", 0);
        textVert[a].z = tempXML.getValue("values_" + ofToString(a) + "_z", 0);
    }
    tempXML.popTag();
    
    tempXML.pushTag("screenPos");
    totScreenPos = tempXML.getValue("totScreenPos", 0);
    for(int a = 0; a < totTextVert; a++)
    {
        screenPos[a].x = tempXML.getValue("values_" + ofToString(a) + "_x", 0);
        screenPos[a].y = tempXML.getValue("values_" + ofToString(a) + "_y", 0);
        screenPos[a].z = tempXML.getValue("values_" + ofToString(a) + "_z", 0);
    }
    tempXML.popTag();
    
    tempXML.pushTag("mainVertici");
    totMainVertici = tempXML.getValue("totMainVertici", 0);
    for(int a = 0; a < totMainVertici; a++)
    {
        mainVertici[a].x = tempXML.getValue("values_" + ofToString(a) + "_x", 0);
        mainVertici[a].y = tempXML.getValue("values_" + ofToString(a) + "_y", 0);
        mainVertici[a].z = tempXML.getValue("values_" + ofToString(a) + "_z", 0);
    }
    tempXML.popTag();
    
    tempXML.pushTag("mainIndex");
    totMainIndex = tempXML.getValue("totMainIndex", 0);
    for(int a = 0; a < totMainIndex; a++)
        mainIndex[a] = tempXML.getValue("values_" + ofToString(a), 0);
    tempXML.popTag();
    
    tempXML.popTag();
}
