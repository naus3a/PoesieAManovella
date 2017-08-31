//
//  SoundInput.cpp
//  InteractivePoetry
//
//  Created by enrico<naus3a>viola on 8/7/17.
//
//

#include "SoundInput.h"

SoundInput::SoundInput(){
    fft.setup();
    fft.setNormalize(false);
    bActive = false;
    
    thrLow = 0.25;
    thrMid = 0.25;
    thrHigh = 0.25;
    thrVol = 0.25;
    thrBand = 0.25;
    
    bandListener.setup(&fft,&thrBand);
}

void SoundInput::setActive(bool b){
    bActive = b;
}

void SoundInput::update(){
    if(!bActive)return;
    fft.update();
    bandListener.update();
}

void SoundInput::drawDebug(){
    fft.drawBars();
    ofPushStyle();
    ofNoFill();
    ofSetColor(0, 0, 0, 100);
    float xx = ofGetWidth()-200;
    ofDrawRectangle(xx,0,200,200);
    ofDrawRectangle(xx,200,200,200);
    ofDrawRectangle(xx,400,200,200);
    ofDrawRectangle(xx,600,200,200);
    fft.drawHistoryGraph(ofPoint(xx,0), LOW);
    fft.drawHistoryGraph(ofPoint(xx,200),MID );
    fft.drawHistoryGraph(ofPoint(xx,400),HIGH );
    fft.drawHistoryGraph(ofPoint(xx,600),MAXSOUND );
    ofSetColor(255,0,0,100);
    float yy = ofGetHeight()-20-ofMap(thrBand*fft.getVolumeRange(), 0, fft.getVolumeRange(), 0, ofGetHeight()-20);
    ofDrawLine(0, yy, ofGetWidth(), yy);
    drawChartThreshold(xx,200,thrLow);
    drawChartThreshold(xx,400,thrMid);
    drawChartThreshold(xx,600,thrHigh);
    drawChartThreshold(xx,800,thrVol);
    xx+=25;
    ofSetColor(ofColor::black);
    ofDrawBitmapString("LOW",xx,20);
    ofDrawBitmapString("HIGH",xx,420);
    ofDrawBitmapString("MID",xx,220);
    ofDrawBitmapString("MAX VOLUME",xx,620);
    ofPopStyle();
}

void SoundInput::drawChartThreshold(float _x, float _y, float & val){
    ofPushMatrix();
    ofTranslate(_x, _y);
    float yy = -val*fft.getVolumeRange();
    ofDrawLine(0, yy, 200, yy);
    ofPopMatrix();
}

template<typename T>
void SoundInput::addListener(void(T::*func)(), T * listener, fftRangeType fftTp){
    if(fftTp==LOW){
        vlLow.setup(&fft, &thrLow, fftTp);
        vlLow.setAutoUpdate(true);
        ofAddListener(vlLow.ev, listener, func);
    }else if(fftTp==MID){
        vlMid.setup(&fft, &thrMid, fftTp);
        vlMid.setAutoUpdate(true);
        ofAddListener(vlMid.ev, listener, func);
    }else if(fftTp==HIGH){
        vlHig.setup(&fft, &thrHigh, fftTp);
        vlHig.setAutoUpdate(true);
        ofAddListener(vlHig.ev, listener, func);
    }else if(fftTp==MAXSOUND){
        vlVol.setup(&fft, &thrVol, fftTp);
        vlVol.setAutoUpdate(true);
        ofAddListener(vlVol.ev, listener, func);
    }
}

template<typename T>
void SoundInput::removeListener(void(T::*func)(), T * listener, fftRangeType fftTp){
    if(fftTp==LOW){
        vlLow.setAutoUpdate(false);
        ofRemoveListener(vlLow.ev, listener, func);
    }else if(fftTp==MID){
        vlMid.setAutoUpdate(false);
        ofRemoveListener(vlMid.ev, listener, func);
    }else if(fftTp==HIGH){
        vlHig.setAutoUpdate(false);
        ofRemoveListener(vlHig.ev, listener, func);
    }else if(fftTp==MAXSOUND){
        vlVol.setAutoUpdate(false);
        ofRemoveListener(vlVol.ev, listener, func);
    }
}

SoundInput::ValueListener::ValueListener(){
    fft = NULL;
    val = NULL;
    bAutoUpdate = false;
}

SoundInput::ValueListener::~ValueListener(){
    setAutoUpdate(false);
}

void SoundInput::ValueListener::setup(ProcessFFT *_fft, float *_val, fftRangeType fftTp){
    fft=_fft;
    val=_val;
    
    if(fftTp==LOW){
        getterFunc = std::bind(&ProcessFFT::getLowVal, fft);
    }else if(fftTp==MID){
        getterFunc = std::bind(&ProcessFFT::getMidVal, fft);
    }else if(fftTp==HIGH){
        getterFunc = std::bind(&ProcessFFT::getHighVal, fft);
    }else if(fftTp==MAXSOUND){
        getterFunc = std::bind(&ProcessFFT::getUnScaledLoudestValue, fft);
    }
}

void SoundInput::ValueListener::setAutoUpdate(bool b){
    if(bAutoUpdate==b)return;
    if(b){
        ofAddListener(ofEvents().update, this, &SoundInput::ValueListener::autoUpdate);
    }else{
        ofRemoveListener(ofEvents().update, this, &SoundInput::ValueListener::autoUpdate);
    }
    bAutoUpdate=b;
}

void SoundInput::ValueListener::update(){
    float curVal = getterFunc();
    if(curVal>(*val)){
        ofNotifyEvent(ev, curVal, this);
    }
}

void SoundInput::BandListener::update(){
    if(!bActive)return;
    BandArgs ba;
    ba.bandId = fft->getLoudBand();
    ba.val = fft->getSpectrum()[ba.bandId];
    if(ba.val>((*thr)*fft->getVolumeRange())){
        ofNotifyEvent(evLoudestBand, ba, this);
    }
}