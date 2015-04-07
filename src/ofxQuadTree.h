//
//  ofxQuadTree.h
//
//  Created by ISHII 2bit on 2015/04/05.
//
//

#pragma once

#include "ofMain.h"

struct SplitInfo {
    ofColor c;
    ofRectangle rect;
};

class ofxQuadTree {
    int width, height;
    int x, y;
    int size;
    ofColor c;
    ofxQuadTree **nodes;
    
    float threashold;
    int blockSize;
public:
    ofxQuadTree()
    : threashold(24.0f) {}
    
    inline void setup(float threashold = 24.0f) {
        this->threashold = threashold;
    }
    
    inline void setThreashold(float threashold) {
        this->threashold = threashold;
    }
    
    inline float getThreashold() const {
        return threashold;
    }
    
    inline void setTexture(ofTexture &tex, int depth) {
        ofPixels pix;
        tex.readToPixels(pix);
        ofImage img(pix);
        setImage(img, depth);
    }
    inline void setImage(ofImage &img, int depth) {
        int w = img.getWidth();
        int h = img.getHeight();
        img.resize(2 << (depth - 1), 2 << (depth - 1));
        generateBase(img, depth, w, h, 0, 0, 2 << (depth - 1));
        calc();
    }
    
    inline void draw(unsigned char alpha = 255) {
        draw(0, 0, width, height, alpha);
    }
    
    inline void draw(int x, int y, unsigned char alpha = 255) {
        draw(x, y, width, height, alpha);
    }
    
    void draw(int x, int y, int width, int height, unsigned char alpha = 255) {
        float scaleX = width / (float)blockSize;
        float scaleY = height / (float)blockSize;
        
        if(nodes != NULL) {
            for(int i = 0; i < 4; i++) {
                nodes[i]->draw(x, y, width, height, alpha);
            }
        } else {
            ofSetColor(c, alpha);
            ofRect(x + scaleX * this->x, y + scaleY * this->y, scaleX * size, scaleY * size);
        }
    }

    inline void drawWire(const ofColor &color = ofColor::black) {
        drawWire(0, 0, width, height, color);
    }
    
    inline void drawWire(int x, int y, const ofColor &color = ofColor::black) {
        drawWire(x, y, width, height, color);
    }
    
    void drawWire(int x, int y, int width, int height, const ofColor &color = ofColor::black) {
        float scaleX = width / (float)blockSize;
        float scaleY = height / (float)blockSize;
        
        if(nodes != NULL) {
            for(int i = 0; i < 4; i++) {
                nodes[i]->drawWire(x, y, width, height, color);
            }
        } else {
            ofSetColor(color);
            ofLine(x + scaleX * this->x, y + scaleY * this->y, x + scaleX * (this->x + size), y + scaleY * this->y);
            ofLine(x + scaleX * (this->x + size), y + scaleY * this->y, x + scaleX * (this->x + size), y + scaleY * (this->y + size));
            ofLine(x + scaleX * (this->x + size), y + scaleY * (this->y + size), x + scaleX * this->x, y + scaleY * (this->y + size));
            ofLine(x + scaleX * this->x, y + scaleY * (this->y + size), x + scaleX * this->x, y + scaleY * this->y);
        }
    }
    
    void addRectangleToVector(vector<SplitInfo> &rects) {
        if(nodes != NULL) {
            for(int i = 0; i < 4; i++) {
                nodes[i]->addRectangleToVector(rects);
            }
        } else {
            rects.push_back((SplitInfo){
                .rect =  ofRectangle(x, y, size, size),
                .c = c
            });
        }
    }
    
private:
    void generateBase(ofImage &img, int rank, int width, int height, int x, int y, int blockSize) {
        this->blockSize = blockSize;
        
        this->width = width;
        this->height = height;
        if(rank == 0) {
            this->x = x;
            this->y = y;
            this->c = img.getColor(x, y);
            size = 1;
            nodes = NULL;
        } else {
            int nr = rank - 1;
            nodes = new ofxQuadTree*[4];
            for(int i = 0; i < 4; i++) {
                nodes[i] = new ofxQuadTree();
                nodes[i]->generateBase(img,
                                       nr,
                                       width,
                                       height,
                                       x + (i % 2) * (1 << nr),
                                       y + (i / 2) * (1 << nr),
                                       blockSize);
            }
        }
    }
    
    inline float calcDistance(const ofColor &c0, const ofColor &c1) {
        float r = c0.r - c1.r, g = c0.g - c1.g, b = c0.b - c1.b;
        return sqrtf(r * r + g * g + b * b);
    }
    
    void calc() {
        if(nodes != NULL) {
            for(int i = 0; i < 4; i++) {
                nodes[i]->calc();
            }
            bool bAllLeaf = true;
            for(int i = 0; i < 4; i++) {
                bAllLeaf = bAllLeaf && (nodes[i]->nodes == NULL);
            }
            if(bAllLeaf) {
                float dist = 0.0f;
                for(int j = 0; j < 4; j++) {
                    for(int i = 0; i < 4; i++) {
                        if(i == j) continue;
                        dist += calcDistance(nodes[i]->c, nodes[j]->c);
                    }
                }
                if(dist / 12.0f < threashold) {
                    size = 2 * nodes[0]->size;
                    x = nodes[0]->x;
                    y = nodes[0]->y;
                    
                    int r = 0, g = 0, b = 0;
                    for(int i = 0; i < 4; i++) {
                        r += nodes[i]->c.r;
                        g += nodes[i]->c.g;
                        b += nodes[i]->c.b;
                        delete nodes[i];
                    }
                    c.set(r / 4, g / 4, b / 4);
                    
                    delete[] nodes;
                    nodes = NULL;
                }
            }
        }
    }
};
