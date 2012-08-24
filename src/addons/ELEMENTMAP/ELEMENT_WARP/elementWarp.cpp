#include "elementWarp.h"


//-----------------------------------------------------------------
elementWarp::elementWarp()
{
}



//-----------------------------------------------------------------
void elementWarp::setup(int _outputWidth, int _outputHeight, string _name)
{
    //set texture w & h
    width=_outputWidth;
    height=_outputHeight;

    //allocate texture
    text.allocate(width, height, GL_RGBA);
    
    //file name for save/load settings
    xmlName=_name+".xml";
    cout << "XML FILE: " + xmlName << endl;

    // QUAD WARPER INIT
    bWarpActive=true;
    mainVertici[0]=ofPoint(0,0);                                    //top left
    mainVertici[1]=ofPoint(text.getWidth(),0);                      //top right
    mainVertici[2]=ofPoint(text.getWidth(),text.getHeight());       //bottom right
    mainVertici[3]=ofPoint(0,text.getHeight());                     //bottom left
    quadWarp.setSourceRect( ofRectangle( 0, 0, text.getWidth(), text.getHeight() ) );              
    quadWarp.setTopLeftCornerPosition(ofPoint(mainVertici[0].x, mainVertici[0].y));            
    quadWarp.setTopRightCornerPosition(ofPoint(mainVertici[1].x, mainVertici[1].y));        
    quadWarp.setBottomRightCornerPosition(ofPoint(mainVertici[2].x, mainVertici[2].y));        
    quadWarp.setBottomLeftCornerPosition(ofPoint(mainVertici[3].x, mainVertici[3].y));        
        
    // GRID WARPER INIT
    //default grid= 4x4 control points
    xRes=4;
    yRes=4;
    //create grid coordinates
    bViewGrid=true;
    createGrid(xRes, yRes);

    //start with everything deactivated
    bWarpActive=false;
    bViewGrid=false;
    bSposta=false;
    bHoldSelection=false;
    bMela=false;
    bSpeedUp=false;

    //auto-load last warp settings on startup
    load();
    
    
}



//-----------------------------------------------------------------
void elementWarp::draw(ofTexture _text)
{

    text=_text;
    
    mat = quadWarp.getMatrix();
    //dai vertici del warp ricavo la matrice per 
    glPushMatrix();
    glMultMatrixf(mat.getPtr());
    ofSetColor(ofColor :: white);
    //Cinzio: non so perchè, ma devo prima disegnare la texture, poi pulire il buffer,
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
//    if (bViewGrid) drawGrid();            
    glPopMatrix();
    
    //draw special markers for quad warp vertices
//    if (bWarpActive) drawMarkers();


    
}





//-----------------------------------------------------------------
void elementWarp::updateCoordinates()
{
//converte le coordinate "interne" all'fbo (griglia) in coordinate dello schermo
for (int index=0; index<nPoints; index++) {
    
    ofVec3f screen = mat.preMult( ofVec3f(vertici[index].x,vertici[index].y,0 ) );
    screenPos[index].x = screen.x;
    screenPos[index].y = screen.y;
}
}






//--------------------------------------------------------------
void elementWarp::createGrid(int _xRes, int _yRes){ 
    
    nQuads=(_xRes-1)*(_yRes-1);
    nPoints=4*nQuads;
    
    int index = 0;
    int row=0;
    int col=0;
    
    while (row<(_yRes-1)) {
        for (int ind=0; ind<4; ind++) {
            
            if (ind==0 || ind==3) gridVert[index].x=col*(width/(_xRes-1));
            if (ind==1 || ind==2) gridVert[index].x=(col+1)*(width/(_xRes-1));
            
            if (ind==0 || ind==1) gridVert[index].y=row*(height/(_yRes-1)); 
            if (ind==2 || ind==3) gridVert[index].y=(row+1)*(height/(_yRes-1)); 
            
            vertici[index]=gridVert[index];
            screenPos[index]=gridVert[index];

            //            texVert[index]=vertici[index];
            
            if (ind==0 || ind==3) texVert[index].x=col*(text.getWidth()/(_xRes-1));
            if (ind==1 || ind==2) texVert[index].x=(col+1)*(text.getWidth()/(_xRes-1));
            
            if (ind==0 || ind==1) texVert[index].y=row*(text.getHeight()/(_yRes-1)); 
            if (ind==2 || ind==3) texVert[index].y=(row+1)*(text.getHeight()/(_yRes-1)); 
            
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
void elementWarp::drawMarkers() {
    for (int corner=0; corner<4; corner++)
    {
        ofPushStyle();
        ofSetColor(ofColor :: white);
        
        ofNoFill();
        
        if (vertici[mainIndex[corner]].z==0 || bSposta) {
            ofLine(screenPos[mainIndex[corner]].x,screenPos[mainIndex[corner]].y-120,screenPos[mainIndex[corner]].x,screenPos[mainIndex[corner]].y+120);   
            ofLine(screenPos[mainIndex[corner]].x-120,screenPos[mainIndex[corner]].y,screenPos[mainIndex[corner]].x+120,screenPos[mainIndex[corner]].y);
        }
        else 
        {
            if (!bSposta)
            {
                ofPushStyle();
                ofSetColor(0,255,206); 
                ofSetLineWidth(4);                        
                ofLine(0, screenPos[mainIndex[corner]].y,10000,screenPos[mainIndex[corner]].y);
                ofLine(screenPos[mainIndex[corner]].x,0,screenPos[mainIndex[corner]].x,10000);
                
                ofPopStyle();
                
            }
            ofSetColor(255,0,0);
        }
        if (bSposta) ofSetColor(255,0,0);
        
        ofSetLineWidth(10);
        ofRect(screenPos[mainIndex[corner]].x-40, screenPos[mainIndex[corner]].y-40, 80, 80);  
        ofPopStyle();
    }
}


//--------------------------------------------------------------
void elementWarp::drawGrid() {
    
    mat = quadWarp.getMatrix();
    //dai vertici del warp ricavo la matrice per 
    glPushMatrix();
    glMultMatrixf(mat.getPtr());
    
    
    //draw Grid Lines
    int quad=0;
    int index=0;
    
    ofPushStyle();
    ofSetColor(0,255,206);
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
        ofPushStyle();
        if (vertici[point].z==1) ofSetColor(255,0,0,255);
        else ofSetColor(255,255,255);
        ofFill();
        if (vertici[point].z==1) ofCircle(vertici[point].x, vertici[point].y, 14);
        else ofCircle(vertici[point].x, vertici[point].y, 8);
        ofPopStyle();

   }
    
    glPopMatrix();
}



//--------------------------------------------------------------
void elementWarp::mouseDragged(int x, int y, int button){
    
    if ( (bWarpActive||bViewGrid) && !bSposta)
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
void elementWarp::mousePressed(int x, int y, int button){
    
    if (!bSposta)
    {
    
    if (bWarpActive)    //controlla se sono in prossimità dei vertici principali
    {
        
        for (int c=0; c<4; c++) {
            if (abs(x-screenPos[mainIndex[c]].x)<45 && abs(y-screenPos[mainIndex[c]].y)<45) 
            {
                if (vertici[mainIndex[c]].z==0) vertici[mainIndex[c]].z=1;
            }
            else 
            {
                if (!bHoldSelection) vertici[mainIndex[c]].z=0;      
            }
        }
    }
        else if (bViewGrid)     //cerca con minore tolleranza se ho selezionato un punto della griglia
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

}




//--------------------------------------------------------------
void elementWarp::increaseXgrid()
{
    if (bViewGrid)
    {
        xRes++;
        if (xRes>MAX_RESOLUTION) xRes=MAX_RESOLUTION;
        createGrid(xRes, yRes);
    }

}

//--------------------------------------------------------------
void elementWarp::decreaseXgrid()
{
    if (bViewGrid)
    {
        xRes--;
        if (xRes<2) xRes=2;
        createGrid(xRes, yRes);
    }
   
}

//--------------------------------------------------------------
void elementWarp::increaseYgrid()
{
    if (bViewGrid)
    {
        yRes++;
    if (yRes>MAX_RESOLUTION) yRes=MAX_RESOLUTION;
    createGrid(xRes, yRes);
    }
}

//--------------------------------------------------------------
void elementWarp::decreaseYgrid()
{
//    if (bWarpActive)
    if (bViewGrid)
    {
        yRes--;
        if (yRes<2) yRes=2;
        createGrid(xRes, yRes);
    }   
}




//--------------------------------------------------------------
void elementWarp::selectMainCorners()
{

    if (bSposta && bWarpActive) 
    {
        for (int i=0; i<nPoints; i++) vertici[i].z=0;
        vertici[mainIndex[0]].z=1;
        vertici[mainIndex[1]].z=1;
        vertici[mainIndex[2]].z=1;
        vertici[mainIndex[3]].z=1;
        
    }
    else for (int i=0; i<nPoints; i++) vertici[i].z=0;
}





//--------------------------------------------------------------
void elementWarp::selectNextMainCorner()
{
    if (bWarpActive)
    {
        
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
}
}



//--------------------------------------------------------------
void elementWarp::resetCorners()
{
    if (bWarpActive)
    {
        mainVertici[0]=ofPoint(0,0);            //top left
        mainVertici[1]=ofPoint(width,0);        //top right
        mainVertici[2]=ofPoint(width,height);   //bottom right
        mainVertici[3]=ofPoint(0,height);       //bottom left
        quadWarp.setTopLeftCornerPosition(ofPoint(mainVertici[0].x, mainVertici[0].y));            
        quadWarp.setTopRightCornerPosition(ofPoint(mainVertici[1].x, mainVertici[1].y));        
        quadWarp.setBottomRightCornerPosition(ofPoint(mainVertici[2].x, mainVertici[2].y));        
        quadWarp.setBottomLeftCornerPosition(ofPoint(mainVertici[3].x, mainVertici[3].y));        
    }
}



//--------------------------------------------------------------
void elementWarp::resetPoint()
{
    if (bWarpActive||bViewGrid)
    {
        
        for (int i=0; i<nPoints; i++) {
            
            if (vertici[i].z==1)
            {
                //mi comporto diversamente se devo resettare il warp esterno
                if (i==mainIndex[0]) 
                { 
                    mainVertici[0]=ofPoint(0,0);            //top left
                    quadWarp.setTopLeftCornerPosition(mainVertici[0]);
                }
                else if (i==mainIndex[1]) 
                { 
                    mainVertici[1]=ofPoint(width,0);        //top right
                    quadWarp.setTopRightCornerPosition(mainVertici[1]);
                }
                else if (i==mainIndex[2]) 
                { 
                    mainVertici[2]=ofPoint(width,height);   //bottom right
                    quadWarp.setBottomRightCornerPosition(mainVertici[2]);
                }
                else if (i==mainIndex[3]) 
                { 
                    mainVertici[3]=ofPoint(0,height);       //bottom left
                    quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
                }
                
    
                else vertici[i]=gridVert[i];
            }
        }
    }
}

//--------------------------------------------------------------
void elementWarp::pointUP(int _delta)
{
    if (bWarpActive||bViewGrid)
    {
        for (int i=0; i<nPoints; i++) {
        
        if (vertici[i].z==1)
        {
            
            //se sono sul rettangolo esterno:
            if (i==mainIndex[0]) 
            { 
                mainVertici[0].y-=_delta;
                quadWarp.setTopLeftCornerPosition(mainVertici[0]);
            }
            else if (i==mainIndex[1]) 
            { 
                mainVertici[1].y-=_delta;
                quadWarp.setTopRightCornerPosition(mainVertici[1]);
            }
            else if (i==mainIndex[2]) 
            { 
                mainVertici[2].y-=_delta;
                quadWarp.setBottomRightCornerPosition(mainVertici[2]);
            }
            else if (i==mainIndex[3]) 
            { 
                mainVertici[3].y-=_delta;
                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
            }
            
            //se sono in griglia precisione massima:
            else vertici[i].y-=1;
            
        }
        }
    }
}


//--------------------------------------------------------------
void elementWarp::pointDOWN(int _delta)
{
    if (bWarpActive||bViewGrid)
    {
        for (int i=0; i<nPoints; i++) {
        
        if (vertici[i].z==1)
        {
            //se sono sul rettangolo esterno:
            if (i==mainIndex[0]) 
            { 
                mainVertici[0].y+=_delta;
                quadWarp.setTopLeftCornerPosition(mainVertici[0]);
            }
            else if (i==mainIndex[1]) 
            { 
                mainVertici[1].y+=_delta;
                quadWarp.setTopRightCornerPosition(mainVertici[1]);
            }
            else if (i==mainIndex[2]) 
            { 
                mainVertici[2].y+=_delta;
                quadWarp.setBottomRightCornerPosition(mainVertici[2]);
            }
            else if (i==mainIndex[3]) 
            { 
                mainVertici[3].y+=_delta;
                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
            }
            
            //se sono in griglia precisione massima:
            else vertici[i].y+=1;
        }
    }
    }
}


//--------------------------------------------------------------
void elementWarp::pointLEFT(int _delta)
{
    if (bWarpActive||bViewGrid)
    {
        
    for (int i=0; i<nPoints; i++) {
        
        if (vertici[i].z==1)
        {
            //se sono sul rettangolo esterno:
            if (i==mainIndex[0]) 
            { 
                mainVertici[0].x-=_delta;
                quadWarp.setTopLeftCornerPosition(mainVertici[0]);
            }
            else if (i==mainIndex[1]) 
            { 
                mainVertici[1].x-=_delta;
                quadWarp.setTopRightCornerPosition(mainVertici[1]);
            }
            else if (i==mainIndex[2]) 
            { 
                mainVertici[2].x-=_delta;
                quadWarp.setBottomRightCornerPosition(mainVertici[2]);
            }
            else if (i==mainIndex[3]) 
            { 
                mainVertici[3].x-=_delta;
                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
            }
            
            //se sono in griglia precisione massima:
            else vertici[i].x-=1;
        }
        
    }
    }

}



//--------------------------------------------------------------
void elementWarp::pointRIGHT(int _delta)
{
    if (bWarpActive||bViewGrid)
    {
    
    for (int i=0; i<nPoints; i++) {
        
        if (vertici[i].z==1)
        {
            //se sono sul rettangolo esterno:
            if (i==mainIndex[0]) 
            { 
                mainVertici[0].x+=_delta;
                quadWarp.setTopLeftCornerPosition(mainVertici[0]);
            }
            else if (i==mainIndex[1]) 
            { 
                mainVertici[1].x+=_delta;
                quadWarp.setTopRightCornerPosition(mainVertici[1]);
            }
            else if (i==mainIndex[2]) 
            { 
                mainVertici[2].x+=_delta;
                quadWarp.setBottomRightCornerPosition(mainVertici[2]);
            }
            else if (i==mainIndex[3]) 
            { 
                mainVertici[3].x+=_delta;
                quadWarp.setBottomLeftCornerPosition(mainVertici[3]);
            }
            
            //se sono in griglia precisione massima:
            else vertici[i].x+=1;
        }
    }
    }
}



//--------------------------------------------------------------
void elementWarp::selectNextPoint()
{
    if (bViewGrid)
    {
        int nuovaX,nuovaY;
        
        for (int i=0; i<nPoints; i++) 
        {
            
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
   
}


//--------------------------------------------------------------
void elementWarp::selectPrevPoint()
{
    if (bViewGrid)
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

}



//--------------------------------------------------------------
void elementWarp::deselectAll()
{
    for (int i=0; i<nPoints; i++) vertici[i].z=0;       
}



//--------------------------------------------------------------
void elementWarp::warpKeyPressedHandler(int _key)
{
    cout << "Warp key" << _key << endl;
    switch (_key) {
            
        case 115: //'s':
            save();
            break;
            
        case 108: //'l':
            cout << "Load" << endl;
            load();
            break;
            
        case 102:
            ofToggleFullscreen();
            
            
            if(ofGetWindowMode() == OF_WINDOW)
            {
                ofSetWindowPosition(ofGetWindowPositionX(), 10);
                ofToggleFullscreen();
                ofSetWindowPosition(ofGetScreenWidth(), 0);
                ofToggleFullscreen();   
            }
            break;
            
            
        case 119: //'w':
            bViewGrid=false;
            deselectAll();
            quadWarp.toggleShow();
            bWarpActive=!bWarpActive;
            break;     
            
        case 103: //'g':
            bWarpActive=false;
            quadWarp.hide();
            deselectAll();
            bViewGrid=!bViewGrid;
            break;     
            
        case 116: //'t':
            bSposta=!bSposta;
            selectMainCorners();                
            break;
            
        case 120: // 'x':
            increaseXgrid();
            break;
            
            
        case 122: //'z':
            decreaseXgrid();
            break;
            
            
        case 113: //'q':
            increaseYgrid();
            break;
            
            
        case 97: //'a':
            decreaseYgrid();
            break;
            
            
        case 269:
            cout << "UP" << endl;
            if (bSpeedUp) pointUP(40);
            else pointUP(1);
            break;
            
        case 270 ://OF_KEY_DOWN:
            if (bSpeedUp) pointDOWN(40);
            else pointDOWN(1);
            break;
            
        case 267: //OF_KEY_LEFT:
            if (bSpeedUp) pointLEFT(40);
            else pointLEFT(1);
            break;
            
        case 268: //OF_KEY_RIGHT:
            if (bSpeedUp) pointRIGHT(40);
            else pointRIGHT(1);
            break;
            
        case 109: //'m':
            selectNextPoint();
            break;
            
        case 110: //'n':
            selectPrevPoint();
            break;
            
            
        case 118: //'v':
            selectNextMainCorner();
            break;
            
            
        case 114: //'r':
            if (bMela)
            {
                increaseXgrid();
                decreaseXgrid();
            }
            else resetPoint();
            break;
            
        case 99: //'c':
            resetCorners();
            break;
                       
        case 262: //command
            bMela=true;
            break;
            
        case 104: //'h':
            bHoldSelection=true;
            break;
            
        case 60: //'<':
            bSpeedUp=true;
            break;
            
        default:
            break;
    }
    
}


//--------------------------------------------------------------
void elementWarp::warpKeyReleasedHandler(int _key)
{
    switch (_key) {
            
        case 262: //command
            bMela=false;
            break;

        case 104: // 'h':
            bHoldSelection=false;
            break;
            
        case 60: // '<':
            bSpeedUp=false;
            break;
    }
 
}





////WARP UTILITIES:
//--------------------------------------------------------------
void elementWarp::saveXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex)
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
    string XMLpath = "./XML/"+xmlName;
    XML.saveFile(XMLpath);
}



//--------------------------------------------------------------
void elementWarp::loadXML(int &resX, int &resY, ofPoint vertici[], int totVertici, ofPoint textVert[], int totTextVert, ofPoint screenPos[], int totScreenPos, ofPoint mainVertici[], int totMainVertici, int mainIndex[], int totMainIndex)
{
    ofxXmlSettings tempXML;
    string XMLpath = "./XML/"+xmlName;
    tempXML.loadFile(XMLpath);
    
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




//--------------------------------------------------------------
void elementWarp::save()
{
    saveXML(xRes, yRes, &vertici[0], nPoints, &texVert[0], nPoints, &screenPos[0], nPoints, &mainVertici[0], 4, &mainIndex[0], 4);
}



//--------------------------------------------------------------
void elementWarp::load()
{
    loadXML(xRes, yRes, &vertici[0], nPoints, &texVert[0], nPoints, &screenPos[0], nPoints, &mainVertici[0], 4, &mainIndex[0], 4);
    createGrid(xRes, yRes);
    loadXML(xRes, yRes, &vertici[0], nPoints, &texVert[0], nPoints, &screenPos[0], nPoints, &mainVertici[0], 4, &mainIndex[0], 4);
    quadWarp.setTopLeftCornerPosition(ofPoint(mainVertici[0].x, mainVertici[0].y));            
    quadWarp.setTopRightCornerPosition(ofPoint(mainVertici[1].x, mainVertici[1].y));        
    quadWarp.setBottomRightCornerPosition(ofPoint(mainVertici[2].x, mainVertici[2].y));        
    quadWarp.setBottomLeftCornerPosition(ofPoint(mainVertici[3].x, mainVertici[3].y));   
}


//--------------------------------------------------------------
void elementWarp::drawElementOutline()
{
    ofPushStyle();
    ofSetColor(ofColor :: red);
    ofSetLineWidth(8);
    ofLine(mainVertici[0], mainVertici[1]);
    ofLine(mainVertici[1], mainVertici[2]);
    ofLine(mainVertici[2], mainVertici[3]);
    ofLine(mainVertici[3], mainVertici[0]);
    ofPopStyle();
}
