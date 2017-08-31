//
//  FlowManager.h
//  InteractivePoetry
//
//  Created by enrico<naus3a>viola on 8/28/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxCv.h"

class FlowManager{
public:
    FlowManager();
    void update(ofFbo & _frame);
    void update(ofPixels & _frame);
    
    ofxCv::FlowFarneback fb;
    
    void setPyrScale(float val){fbPyrScale=val;fb.setPyramidScale(val);}
    void setPolySigma(float val){fbPolySigma=val;fb.setPolySigma(val);}
    void setLevels(int val){fbLevels=val;fb.setNumLevels(val);}
    void setIterations(int val){fbIterations=val;fb.setNumIterations(val);}
    void setPolyN(int val){fbPolyN=val;fb.setPolyN(val);}
    void setWinSize(int val){fbWinSize=val;fb.setWindowSize(val);}
    void setGaussian(bool b){bGaussian=b;fb.setUseGaussian(b);}
    
    float getPyrScale(){return fbPyrScale;}
    float getPolySigma(){return fbPolySigma;}
    int getLevels(){return fbLevels;}
    int getIterations(){return fbIterations;}
    int getPolyN(){return fbPolyN;}
    int getWinSize(){return fbWinSize;}
    bool isGaussian(){return bGaussian;}
protected:
    ofxCv::Flow * curFlow;
    
    float fbPyrScale;
    float fbPolySigma;
    int fbLevels;
    int fbIterations;
    int fbPolyN;
    int fbWinSize;
    bool bGaussian;
};