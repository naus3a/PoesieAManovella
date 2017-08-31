//
//  KinectManager.h
//  InteractivePoetry
//
//  Created by enrico<naus3a>viola on 8/28/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxKinect.h"
#include "FlowManager.h"
#include "PoetryEngine.h"

class KinectManager{
public:
    KinectManager();
    ~KinectManager();
    void init();
    void close();
    
    void linkPoetryEngine(PoetryEngine * _pe);
    
    void update();
    void drawShapes(float _x, float _y, float _w, float _h);
    void drawDebug(float _x, float _y);
    
    void checkPoemMotion();
    void checkWordMotion();
    
    ofxKinect kinect;
    FlowManager flow;
    ofImage imgWall;
    ofVec2f wallSpd;
    float near;
    float far;
    float scCv;
    float thrPoem;
    float thrWord;
    bool bFlow;
    bool bWord;
    
protected:
    PoetryEngine * pe;
    
    ofShader shdDist;
    ofShader shdShapes;
    ofFbo fboThr;
    ofFbo fboCv;
    ofVec2f posWall;
    
};