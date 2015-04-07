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
        level = 7;
    }
    void update() {
        grabber.update();
        if(grabber.isFrameNew()) {
            tree.setTexture(grabber.getTextureReference(), level);
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
        
        ofSetColor(ofColor::red);
        ofDrawBitmapString(string("threashold : ") + ofToString(tree.getThreashold()), 40, 40);
        ofDrawBitmapString(string("split level : ") + ofToString(level), 40, 60);
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
        if(key == OF_KEY_LEFT) {
            level = max(1, level -1);
        }
        if(key == OF_KEY_RIGHT) {
            level = min(10, level + 1);
        }
        if(key == OF_KEY_UP) {
            tree.setThreashold(max(tree.getThreashold() - 0.1f, 4.0f));
        }
        if(key == OF_KEY_DOWN) {
            tree.setThreashold(min(tree.getThreashold() + 0.1f, 64.0f));
        }
    }
    
private:
    ofVideoGrabber grabber;
    ofxQuadTree tree;
    
    bool bWire;
    bool bPlane;
    bool bOriginal;
    
    int level;
};
