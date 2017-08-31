//
//  PoetryEngine.cpp
//  InteractivePoetry
//
//  Created by enrico<naus3a>viola on 8/7/17.
//
//

#include "PoetryEngine.h"

void PoetryEngine::Word::setText(string s){
    text = s;
    push.set(0,0);
}

void PoetryEngine::Verse::setFromStrings(vector<string> & ll, ofTrueTypeFont * _font){
    font = _font;
    words.clear();
    words.resize(ll.size());
    for(int i=0;i<ll.size();i++){
        words[i].setText(ll[i]);
    }
}

void PoetryEngine::Poem::loadLinesFromXml(ofxXmlSettings & xml, ofTrueTypeFont * _font){
    font = _font;
    
    lines.clear();
    lines.resize(xml.getNumTags("l"));
    for(int i=0;i<lines.size();i++){
        string l = xml.getValue("l", "", i);
        vector<string> ll = ofSplitString(l, " ");
        lines[i].setFromStrings(ll, font);
    }
}

void PoetryEngine::Word::setStartPosition(float _x, float _y, ofTrueTypeFont * _font){
    font=_font;
    pos.set(_x,_y);
    
    float fW = font->stringWidth(text);
    float fH = font->stringHeight(text);
    txtOff.set(-fW/2, fH/2);
    
    reset();
}

void PoetryEngine::Word::setBlock(ofRectangle r){
    rBlock = r;
    rBlockCv.set(0,0,640*0.25,480*0.25);
    rBlockCv.width = ofMap(rBlock.width, 0, ofGetWidth(), 0, 640*0.25);
    rBlockCv.height = ofMap(rBlock.height, 0, ofGetHeight(), 0, 480*0.25);
    rBlockCv.x = ofMap(rBlock.x, 0, ofGetWidth(), 0, 640*0.25);
    rBlockCv.y = ofMap(rBlock.y, 0, ofGetHeight(), 0, 480*0.25);
}

void PoetryEngine::Verse::setBlock(ofRectangle r){
    block = r;
    float pY = r.y + r.height/2;
    float xSpc = r.width/float(words.size());
    float xPad = xSpc/2;
    float wBw = r.width/words.size();
    for(int i=0;i<words.size();i++){
        words[i].setStartPosition(r.x + xPad + xSpc*i, pY, font);
        words[i].setBlock(ofRectangle(words[i].getStartPos().x-wBw/2,r.y,wBw,r.height));
        if(i>0){
            words[i].endOff.set(0, 0);
            float wPrev = font->stringWidth(words[i-1].text);
            float wCur = font->stringWidth(words[i].text);
            float pDist = words[i].getStartPos().x-words[i-1].getStartPos().x;
            float prevEnd=-pDist+wPrev/2;
            if(i>1){
                prevEnd += words[i-1].endOff.x;
            }
            words[i].endOff.set(wCur/2 + prevEnd + font->getSize()/3, 0);
        }
    }
}

void PoetryEngine::Poem::setBlock(ofRectangle r){
    block = r;
    float vH = r.height/float(lines.size());
    for(int i=0;i<lines.size();i++){
        lines[i].setBlock(ofRectangle(r.x, vH*i, r.width, vH));
    }
}

bool PoetryEngine::loadDictionary(string pth, int _fontSz){
    strongestPush = 0;
    
    font.load("fonts/verdana.ttf", _fontSz);
    
    poems.clear();
    
    ofxXmlSettings xml;
    if(xml.load(pth)){
        if(xml.tagExists("verses")){
            xml.pushTag("verses");
            int nPoems = xml.getNumTags("poem");
            cout<<nPoems<<" poems found"<<endl;
            poems.resize(nPoems);
            for(int i=0;i<nPoems;i++){
                xml.pushTag("poem", i);
                poems[i].loadLinesFromXml(xml, &font);
                xml.popTag();
            }
            xml.popTag();
            makeBlocks();
            
            for(int p=0;p<getPoems().size();p++){
                for(int v=0;v<getPoems()[p].getVerses().size();v++){
                    for(int w=0;w<getPoems()[p].getVerses()[v].getWords().size();w++){
                        getPoems()[p].getVerses()[v].getWords()[w].evTrigger = &evTrigger;
                        getPoems()[p].getVerses()[v].getWords()[w].wordId = w;
                    }
                }
            }
            
            return true;
        }else{
            ofLogError("PoetryEngine::loadDictionary")<<"malformed "<<pth;
            return false;
        }
    }else{
        ofLogError("PoetryEngine::loadDictionary")<<"cannot load "<<pth;
        return false;
    }
}

void PoetryEngine::makeBlocks(){
    float blockW = getWidth()/float(poems.size());
    for(int i=0;i<poems.size();i++){
        poems[i].setBlock(ofRectangle(blockW*i, 0, blockW, getHeight()));
    }
}

void PoetryEngine::setup(float _w, float _h){
    sz.set(_w, _h);
}

void PoetryEngine::update(float & now){
    for(int i=0;i<poems.size();i++){
        poems[i].update(now);
    }
}

void PoetryEngine::Poem::update(float & now){
    for(int i=0;i<lines.size();i++){
        lines[i].update(now);
    }
}

void PoetryEngine::Verse::update(float & now){
    for(int i=0;i<words.size();i++){
        words[i].update(now);
    }
}

void PoetryEngine::Word::update(float & now){
    if(!bActive)return;
    if(now<stopTime){
        curPos += curSpd;
        
        float curPct = ofMap(now, startTime, stopTime, 0, 1);
        if(curPct<0.5){
            curOff.x = ofLerp(0, endOff.x, curPct*2);
        }else{
            curOff.x=endOff.x;
        }
        if(curPct<pctGrown){
            curScale = ofMap(curPct, 0, pctGrown, 0, 1);
        }else if(curPct>pctFade){
            alpha = ofMap(curPct, pctFade, 1.0, 255, 0);
        }
    }else{
        bActive = false;
    }
}

void PoetryEngine::Word::updateFlow(ofxCv::FlowFarneback * flow){
    push = flow->getAverageFlowInRegion(rBlockCv);
    pushLength = push.length();
}

void PoetryEngine::Verse::updateFlow(ofxCv::FlowFarneback *flow){
    pushLength = 0;
    for(int i=0;i<words.size();i++){
        words[i].updateFlow(flow);
        pushLength+=words[i].pushLength;
    }
    pushLength/=words.size();
}

void PoetryEngine::Poem::updateFlow(ofxCv::FlowFarneback *flow){
    pushLength = 0;
    for(int i=0;i<lines.size();i++){
        lines[i].updateFlow(flow);
        pushLength+=lines[i].pushLength;
    }
    pushLength/=lines.size();
}

void PoetryEngine::updateFlow(ofxCv::FlowFarneback *flow){
    float highVal = -1;
    strongestPush = 0;
    for(int i=0;i<poems.size();i++){
        poems[i].updateFlow(flow);
        if(poems[i].pushLength>highVal){
            highVal = poems[i].pushLength;
            strongestPush = i;
        }
    }
}

void PoetryEngine::Word::draw(){
    if(!bActive)return;
    ofPushMatrix();
    ofTranslate(curPos);
    ofScale(curScale, curScale, 1);
    ofSetColor(0,0,0,alpha);
    //ofNoFill();
    //ofCircle(0, 0, 20);
    font->drawString(text, txtOff.x+curOff.x, txtOff.y+curOff.y);
    
    ofPopMatrix();
}

void PoetryEngine::draw(){
    ofPushStyle();
    for(int i=0;i<poems.size();i++){
        poems[i].draw();
    }
    ofPopStyle();
}

void PoetryEngine::drawDebug(){
    ofPushStyle();
    ofNoFill();
    for(int i=0;i<poems.size();i++){
        poems[i].drawDebug();
    }
    ofPopStyle();
}

void PoetryEngine::Poem::drawDebug(){
    ofSetColor(255,0,0,100);
    ofDrawRectangle(block);
    for(int i=0;i<lines.size();i++){
        lines[i].drawDebug();
    }
}

void PoetryEngine::Verse::drawDebug(){
    ofSetColor(0,255,0,100);
    ofDrawRectangle(block);
    for(int i=0;i<words.size();i++){
        words[i].drawDebug();
    }
}

void PoetryEngine::Word::drawDebug(){
    ofSetColor(0,0,255,100);
    ofDrawCircle(pos.x, pos.y, 5);
    //ofDrawRectangle(rBlock);
}

void PoetryEngine::Word::drawPushVector(float &thrVal){
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofDrawLine(0, 0, push.x*10, push.y*10);
    ofPushStyle();
    ofSetColor(ofColor::orange);
    ofNoFill();
    ofDrawCircle(0, 0, thrVal*10);
    ofPopStyle();
    ofPopMatrix();
}

void PoetryEngine::drawPushVectors(float &thrVal){
    for(int i=0;i<poems.size();i++){
        ofPushStyle();
        ofSetColor(255,0,0,100);
        poems[i].drawPushVectors(thrVal);
        ofSetColor(255, i==strongestPush?0:255, 0, 100);
        ofFill();
        ofDrawRectangle(poems[i].block.x, poems[i].block.y, poems[i].block.width, poems[i].pushLength*20);
        ofPopStyle();
    }
}

void PoetryEngine::Word::reset(){
    bActive = false;
    alpha = 0;
    curPos = pos;
    curScale = 0;
    curOff.set(0,0);
    ttl = defaultTTL * ofRandom(0.8,1.2);
    curSpd.set(0,0, defaultSpeedZ* ofRandom(0.8,1.2));
}

void PoetryEngine::Word::trigger(){
    if(bActive)return;
    reset();
    bActive = true;
    alpha = 255;
    startTime = ofGetElapsedTimef();
    stopTime = startTime+ttl;
    
    ofNotifyEvent(*evTrigger, wordId);
}
