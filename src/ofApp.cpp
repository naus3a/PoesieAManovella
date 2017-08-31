#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    ofVec2f curSz;
    curSz.x = ofGetScreenWidth();
    curSz.y = ofGetScreenHeight();
    ofSetWindowShape(curSz.x, curSz.y);
    ofSetFullscreen(true);
    
    sndPiano.load("snd/c.wav");
    sndPiano.setMultiPlay(true);
    setupScale();
    
    bDebugPoems = false;
    bDebugSound = false;
    bDebugKinect = false;
    bRender = true;
    bSoundOut = true;
    
    pe.setup(curSz.x,curSz.y);
    pe.loadDictionary("poetry/verses.xml", 20);
    
    km.linkPoetryEngine(&pe);
    
    ofAddListener(soundIn.bandListener.evLoudestBand, this, &ofApp::onBand);
    ofAddListener(pe.evTrigger, this, &ofApp::onTrigger);
    
    setupGUI();
    
}

void ofApp::setupGUI(){
    butSave = new ofxUILabelButton("save",false);
    butLoad = new ofxUILabelButton("load", false);
    togDebugPoems = new ofxUILabelToggle("debug",&bDebugPoems);
    togDebugSound = new ofxUILabelToggle("debug sound",&bDebugSound);
    togSound = new ofxUILabelToggle("active", soundIn.isActive());
    togDebugCamera = new ofxUILabelToggle("debug camera", &bDebugKinect);
    togFlow = new ofxUILabelToggle("motion", &km.bFlow);
    togGaussian = new ofxUILabelToggle("gaussian", km.flow.isGaussian());
    togMoveWord = new ofxUILabelToggle("move single word", &km.bWord);
    togRender = new ofxUILabelToggle("silhouettes", &bRender);
    togSoundOut = new ofxUILabelToggle("sound out", & bSoundOut);
    sliLow = new ofxUISlider("low", 0.0, 1.0, &soundIn.thrLow, 80,10);
    sliMid = new ofxUISlider("mid", 0.0, 1.0, &soundIn.thrMid, 80,10);
    sliHig = new ofxUISlider("high", 0.0, 1.0, &soundIn.thrHigh, 80,10);
    sliVol = new ofxUISlider("vol", 0.0, 1.0, &soundIn.thrVol, 80,10);
    sliBand = new ofxUISlider("bands",0.0,1.0,&soundIn.thrBand,180,10);
    sliTTL = new ofxUISlider("ttl",0.0,60.0, pe.getWordTTL(), 80,10);
    sliFade = new ofxUISlider("fade at",0.0,1.0, pe.getPctFade(), 80,10);
    sliGrown = new ofxUISlider("grown at",0.0,1.0,pe.getPctGrown(),80,10);
    sliZspeed = new ofxUISlider("z speed",0.0,2.0,pe.getZspeed(),80,10);
    sliNear = new ofxUISlider("near",0.0,1.0,&km.near,80,10);
    sliFar = new ofxUISlider("far",0.0,1.0,&km.far,80,10);
    sliPyrScale = new ofxUISlider("pyr scale",0,0.99,km.flow.getPyrScale(),80,10);
    sliLevels = new ofxUISlider("levels",1,8,km.flow.getLevels(),80,10);
    sliIterations = new ofxUISlider("iterations",1,8,km.flow.getIterations(),80,10);
    sliPolyN = new ofxUISlider("poly N",5,10,km.flow.getPolyN(),80,10);
    sliWinSize = new ofxUISlider("win size",4,64,km.flow.getWinSize(),80,10);
    sliPolySigma = new ofxUISlider("poly sigma", 1.1,2,km.flow.getPolySigma(),80,10);
    sliThrMovPoem = new ofxUISlider("poem thr",0.0,30.0,&km.thrPoem,80,10);
    sliThrMovWord = new ofxUISlider("word thr",0.0,10.0,&km.thrWord,80,10);
    sliSpdX = new ofxUISlider("speed X",0.00001,0.001,&km.wallSpd.x,80,10);
    sliSpdY = new ofxUISlider("speed Y",0.00001,0.001,&km.wallSpd.y,80,10);
    
    gui = new ofxUICanvas(0,0,200,ofGetHeight());
    gui->addWidgetDown(new ofxUILabel("InteractivePoetry", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(butSave);
    gui->addWidgetRight(butLoad);
    gui->addWidgetRight(new ofxUIFPS(OFX_UI_FONT_MEDIUM));
    gui->addSpacer();
    gui->addWidgetDown(new ofxUILabel("Poems",OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(togDebugPoems);
    gui->addWidgetDown(sliTTL);
    gui->addWidgetRight(sliZspeed);
    gui->addWidgetDown(sliFade);
    gui->addWidgetRight(sliGrown);
    gui->addWidgetDown(sliSpdX);
    gui->addWidgetRight(sliSpdY);
    gui->addWidgetDown(new ofxUILabel("Sound",OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(togSound);
    gui->addWidgetRight(togDebugSound);
    gui->addWidgetDown(togSoundOut);
    gui->addWidgetDown(sliLow);
    gui->addWidgetRight(sliMid);
    gui->addWidgetDown(sliHig);
    gui->addWidgetRight(sliVol);
    gui->addWidgetDown(sliBand);
    gui->addWidgetDown(new ofxUILabel("Camera",OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(togDebugCamera);
    gui->addWidgetDown(togRender);
    gui->addWidgetDown(togMoveWord);
    gui->addWidgetDown(togFlow);
    gui->addWidgetDown(sliNear);
    gui->addWidgetRight(sliFar);
    gui->addWidgetDown(sliThrMovPoem);
    gui->addWidgetRight(sliThrMovWord);
    gui->addWidgetDown(new ofxUILabel("Optical flow", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(togGaussian);
    gui->addWidgetDown(sliPyrScale);
    gui->addWidgetRight(sliLevels);
    gui->addWidgetDown(sliIterations);
    gui->addWidgetRight(sliPolyN);
    gui->addWidgetDown(sliPolySigma);
    gui->addWidgetRight(sliWinSize);
    
    loadGUI();
    
    ofAddListener(gui->newGUIEvent, this, &ofApp::onGUI);
    
    gui->setVisible(false);
}

void ofApp::saveGUI(){
    gui->saveSettings("xml/gui.xml");
}

void ofApp::loadGUI(){
    gui->loadSettings("xml/gui.xml");
    
    pe.setWordTTL(sliTTL->getValue());
    pe.setPctFade(sliFade->getValue());
    pe.setPctGrown(sliGrown->getValue());
    pe.setZspeed(sliZspeed->getValue());
    
    soundIn.setActive(togSound->getValue());
    
    km.flow.setGaussian(togGaussian->getValue());
    km.flow.setPyrScale(sliPyrScale->getValue());
    km.flow.setPolySigma(sliPolySigma->getValue());
    km.flow.setLevels(sliLevels->getValue());
    km.flow.setIterations(sliIterations->getValue());
    km.flow.setPolyN(sliPolyN->getValue());
    km.flow.setWinSize(sliWinSize->getValue());
}

void ofApp::onGUI(ofxUIEventArgs & e){
    if(e.widget==butSave){
        if(butSave->getValue()){
            saveGUI();
        }
    }else if(e.widget==butLoad){
        if(butLoad->getValue()){
            loadGUI();
        }
    }else if(e.widget==togSound){
        soundIn.setActive(togSound->getValue());
    }else if(e.widget==sliTTL){
        pe.setWordTTL(sliTTL->getValue());
    }else if(e.widget==sliFade){
        pe.setPctFade(sliFade->getValue());
    }else if(e.widget==sliGrown){
        pe.setPctGrown(sliGrown->getValue());
    }else if(e.widget==sliZspeed){
        pe.setZspeed(sliZspeed->getValue());
    }else if(e.widget==sliPyrScale){
        km.flow.setPyrScale(sliPyrScale->getValue());
    }else if(e.widget==sliPolySigma){
        km.flow.setPolySigma(sliPolySigma->getValue());
    }else if(e.widget==sliLevels){
        km.flow.setLevels(sliLevels->getValue());
    }else if(e.widget==sliIterations){
        km.flow.setIterations(sliIterations->getValue());
    }else if(e.widget==sliPolyN){
        km.flow.setPolyN(sliPolyN->getValue());
    }else if(e.widget==sliWinSize){
        km.flow.setWinSize(sliWinSize->getValue());
    }else if(e.widget==togGaussian){
        km.flow.setGaussian(togGaussian->getValue());
    }
}

void ofApp::onLow(float &val){}
void ofApp::onMid(float &val){}
void ofApp::onHigh(float &val){}
void ofApp::onVolume(float &val){}

void ofApp::onTrigger(int & wId){
    if(bSoundOut){
        sndPiano.play();
        int note = wId%7;
        int oct = int(wId/7);
        float spd = 1;
        spd = toOctave(spd, oct);
        spd *= scale[note];
        sndPiano.setSpeed(spd);
    }
}

void ofApp::onBand(SoundInput::BandArgs &ba){
    int pId = freqBand2Poem(ba.bandId);
    pe.getPoems()[pId].triggerIncremental();
}

int ofApp::freqBand2Poem(int & bId){
    return int(float(bId)/32.0*float(pe.getPoems().size()));
}

float ofApp::toOctave(float val, int _oct){
    if(_oct>0){
        for(int i=1;i<=_oct;i++){
            val *=2;
        }
    }
    return val;
}

void ofApp::setupScale(){
    scale.resize(7);
    scale[0] = 1;
    scale[1] = nextTone(scale[0]);
    scale[2] = nextSemitone(scale[1]);
    scale[3] = nextTone(scale[2]);
    scale[4] = nextTone(scale[3]);
    scale[5] = nextSemitone(scale[4]);
    scale[6] = nextTone(scale[5]);
}

//--------------------------------------------------------------
void ofApp::update(){
    float now = ofGetElapsedTimef();
    pe.update(now);
    
    soundIn.update();
    km.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(bRender)km.drawShapes(0,0,ofGetWidth(),ofGetHeight());
    pe.draw();
    
    if(bDebugPoems)pe.drawDebug();
    if(bDebugSound)soundIn.drawDebug();
    if(bDebugKinect)km.drawDebug(200, 0);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='g'){
        gui->toggleVisible();
    }else if(key==' '){
        //pe.getPoems()[1].getVerses()[0].getWords()[0].trigger();
        //pe.getPoems()[1].getVerses()[0].getWords()[1].trigger();
        //pe.getPoems()[1].getVerses()[0].getWords()[2].trigger();
        //pe.getPoems()[1].getVerses()[0].getWords()[3].trigger();
        //pe.getPoems()[1].trigger();
        pe.getPoems()[1].triggerIncremental();
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
