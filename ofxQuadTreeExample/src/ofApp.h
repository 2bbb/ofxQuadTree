#pragma once

#include "ofMain.h"
#include "ofxQuadTree.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
private:
    ofVideoGrabber grabber;
    ofxQuadTree tree;
    
    bool bWire;
    bool bPlane;
    bool bOriginal;
    
    int level;
};
