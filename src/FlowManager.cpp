//
//  FlowManager.cpp
//  InteractivePoetry
//
//  Created by enrico<naus3a>viola on 8/28/17.
//
//

#include "FlowManager.h"

FlowManager::FlowManager(){
    setPyrScale(0.5);
    setLevels(4);
    setIterations(2);
    setPolyN(7);
    setPolySigma(1.5);
    setWinSize(32);
    setGaussian(false);
    
    curFlow = &fb;
}

void FlowManager::update(ofFbo & _frame){
    ofPixels pix;
    _frame.readToPixels(pix);
    update(pix);
}

void FlowManager::update(ofPixels & _frame){
    curFlow->calcOpticalFlow(_frame);
}