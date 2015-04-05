#pragma once

#include "ofMain.h"
#include "ofxQuadTree.h"

class ofApp : public ofBaseApp{
public:
    void setup() {
        bWire = true;
        bPlane = false;
        bOriginal = false;
        grabber.initGrabber(1280, 720);
        tree.setup();
    }
    void update() {
        grabber.update();
        if(grabber.isFrameNew()) {
            tree.setTexture(grabber.getTextureReference(), 7);
        }
    }
    void draw() {
        ofBackground(ofColor::black);
        if(bOriginal) {
            ofSetColor(ofColor::white);
            grabber.draw(0, 0);
        }
        if(bPlane) {
            tree.draw();
        }
        if(bWire) {
            tree.drawWire(ofColor::white);
        }
    }
    
    void keyPressed(int key) {
        if(key == 'w') {
            bWire ^= true;
        }
        if(key == 'p') {
            bPlane ^= true;
        }
        if(key == 't') {
            bOriginal ^= true;
        }
    }
    
private:
    ofVideoGrabber grabber;
    ofxQuadTree tree;
    
    bool bWire;
    bool bPlane;
    bool bOriginal;
};
