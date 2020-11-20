#include "ofApp.h"

void ofApp::setup() {
    bWire = true;
    bPlane = false;
    bOriginal = false;
    grabber.initGrabber(1280, 720);
    tree.setup();
    level = 7;
}

void ofApp::update() {
    grabber.update();
    if(grabber.isFrameNew()) {
        tree.setTexture(grabber.getTexture(), level);
    }
}

void ofApp::draw() {
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

void ofApp::keyPressed(int key) {
    switch(key){
        case 'w':
            bWire ^= true;
            break;
        case 'p':
            bPlane ^= true;
            break;
        case 't':
            bOriginal ^= true;
            break;
        case OF_KEY_LEFT:
            level = max(1, level -1);
            break;
        case OF_KEY_RIGHT:
            level = min(10, level + 1);
            break;
        case OF_KEY_UP:
            tree.setThreashold(max(tree.getThreashold() - 0.1f, 4.0f));
            break;
        case OF_KEY_DOWN:
            tree.setThreashold(min(tree.getThreashold() + 0.1f, 64.0f));
            break;
    }
}
