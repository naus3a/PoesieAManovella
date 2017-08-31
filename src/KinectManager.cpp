//
//  KinectManager.cpp
//  InteractivePoetry
//
//  Created by enrico<naus3a>viola on 8/28/17.
//
//

#include "KinectManager.h"

KinectManager::KinectManager(){
    pe = NULL;
    
    bFlow = true;
    bWord = false;
    
    scCv = 0.25;
    
    near = 1.0;
    far = 0.0;
    
    thrPoem = 5;
    thrWord = 5;
    
    posWall.set(0,0);
    wallSpd.set(0.0005,0.001);
    
    imgWall.load("img/poetrywall.png");
    
    shdDist.setupShaderFromFile(GL_FRAGMENT_SHADER, "shaders/dist.frag");
    shdDist.linkProgram();
    
    shdShapes.setupShaderFromFile(GL_FRAGMENT_SHADER, "shaders/shapes.frag");
    shdShapes.linkProgram();
    
    fboThr.allocate(640, 480, GL_RGBA);
    fboThr.begin();
    ofClear(0, 0, 0, 255);
    fboThr.end();
    
    fboCv.allocate(fboThr.getWidth()*scCv, fboThr.getHeight()*scCv, GL_RGB);
    fboCv.begin();
    ofClear(0, 0, 0);
    fboCv.end();
    
    init();
}

KinectManager::~KinectManager(){
    close();
}

void KinectManager::linkPoetryEngine(PoetryEngine * _pe){
    pe=_pe;
}

void KinectManager::init(){
    kinect.setRegistration(false);
    kinect.init();
    kinect.open();
    if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
}

void KinectManager::close(){
    kinect.setCameraTiltAngle(0);
	kinect.close();
}

void KinectManager::update(){
    kinect.update();
    if(kinect.isFrameNew()){
        
        fboThr.begin();
        ofClear(0, 0, 0, 255);
        shdDist.begin();
        shdDist.setUniform1f("near", near);
        shdDist.setUniform1f("far", far);
        kinect.drawDepth(0, 0, fboThr.getWidth(), fboThr.getHeight());
        shdDist.end();
        fboThr.end();
        
        if(bFlow){
            fboCv.begin();
            fboThr.draw(0,0,fboCv.getWidth(),fboCv.getHeight());
            fboCv.end();
            flow.update(fboCv);
            
            pe->updateFlow(&flow.fb);
            
            if(bWord){
                checkWordMotion();
            }else{
                checkPoemMotion();
            }
        }
    }
}

void KinectManager::checkWordMotion(){
    for(int p=0;p<pe->getPoems().size();p++){
        for(int v=0;v<pe->getPoems()[p].getVerses().size();v++){
            for(int w=0;w<pe->getPoems()[p].getVerses()[v].getWords().size();w++){
                if(pe->getPoems()[p].getVerses()[v].getWords()[w].pushLength>thrWord){
                    pe->getPoems()[p].getVerses()[v].getWords()[w].trigger();
                }
            }
        }
    }
}

void KinectManager::checkPoemMotion(){
    if(pe->getPoems()[pe->strongestPush].pushLength>thrPoem){
        pe->getPoems()[pe->strongestPush].triggerIncremental();
    }
}

void KinectManager::drawDebug(float _x, float _y){
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofPushStyle();
    ofSetColor(255, 255, 255, 100);
    fboThr.draw(0,0,320,240);
    if(bFlow){
        ofSetColor(255,0,0,100);
        flow.fb.draw(0, 0, 320, 240);
    }
    ofPopStyle();
    ofPopMatrix();
    if(bFlow){
        ofPushStyle();
        ofSetColor(255, 0, 0, 100);
        //flow.fb.draw(0, 0, ofGetWidth(), ofGetHeight());
        pe->drawPushVectors(thrWord);
        ofSetColor(ofColor::orange);
        ofSetLineWidth(3);
        ofDrawLine(0, thrPoem*30, ofGetWidth(), thrPoem*30);
        ofPopStyle();
    }
}

void KinectManager::drawShapes(float _x, float _y, float _w, float _h){
    shdShapes.begin();
    shdShapes.setUniformTexture("texDpt", fboThr.getTexture(), 1);
    shdShapes.setUniform2f("szWall", imgWall.getWidth(), imgWall.getHeight());
    shdShapes.setUniform2f("szDpt", fboThr.getWidth(), fboThr.getHeight());
    shdShapes.setUniform2f("posWall", posWall.x, posWall.y);
    imgWall.draw(_x,_y,_w,_h);
    shdShapes.end();
    
    posWall = posWall+wallSpd;
    if(posWall.x>1.0)posWall.x = posWall.x-1.0;
    if(posWall.y>1.0)posWall.y = posWall.y-1.0;
}