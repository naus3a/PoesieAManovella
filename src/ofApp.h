#pragma once

#include "ofMain.h"
#include "PoetryEngine.h"
#include "SoundInput.h"
#include "KinectManager.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void setupGUI();
    void saveGUI();
    void loadGUI();
    void onGUI(ofxUIEventArgs & e);
    
    void onLow(float & val);
    void onMid(float & val);
    void onHigh(float & val);
    void onVolume(float & val);
    void onBand(SoundInput::BandArgs & ba);
    void onTrigger(int & wId);
    
    int freqBand2Poem(int & bId);
    void setupScale();
    float nextSemitone(float val){return (val*1.05946);}
    float nextTone(float val){return nextSemitone(nextSemitone(val));}
    float toOctave(float val, int _oct);
    
    ofxUICanvas * gui;
    ofxUILabelButton * butSave;
    ofxUILabelButton * butLoad;
    ofxUILabelToggle * togDebugPoems;
    ofxUILabelToggle * togDebugSound;
    ofxUILabelToggle * togSound;
    ofxUILabelToggle * togDebugCamera;
    ofxUILabelToggle * togFlow;
    ofxUILabelToggle * togGaussian;
    ofxUILabelToggle * togMoveWord;
    ofxUILabelToggle * togRender;
    ofxUILabelToggle * togSoundOut;
    ofxUISlider * sliTTL;
    ofxUISlider * sliFade;
    ofxUISlider * sliGrown;
    ofxUISlider * sliZspeed;
    ofxUISlider * sliLow;
    ofxUISlider * sliMid;
    ofxUISlider * sliHig;
    ofxUISlider * sliVol;
    ofxUISlider * sliBand;
    ofxUISlider * sliNear;
    ofxUISlider * sliFar;
    ofxUISlider * sliPyrScale;
    ofxUISlider * sliPolySigma;
    ofxUISlider * sliLevels;
    ofxUISlider * sliIterations;
    ofxUISlider * sliPolyN;
    ofxUISlider * sliWinSize;
    ofxUISlider * sliThrMovPoem;
    ofxUISlider * sliThrMovWord;
    ofxUISlider * sliSpdX;
    ofxUISlider * sliSpdY;
    
    SoundInput soundIn;
    KinectManager km;
    PoetryEngine pe;
    ofSoundPlayer sndPiano;
    vector<float> scale;
    bool bRender;
	bool bDebugPoems;
    bool bDebugSound;
    bool bDebugKinect;
    bool bSoundOut;
};
