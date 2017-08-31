//
//  PoetryEngine.h
//  InteractivePoetry
//
//  Created by enrico<naus3a>viola on 8/7/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxXmlSettings.h"

class PoetryEngine{
public:
    class Word{
    public:
        
        Word(){
            evTrigger = NULL;
            font=NULL;
            defaultTTL = 5.0;
            pctFade = 0.6;
            pctGrown = 0.3;
            defaultSpeedZ = 0.5;
            wordId=0;
        }
        void setStartPosition(float _x, float _y, ofTrueTypeFont * _font);
        void setBlock(ofRectangle r);
        void setText(string s);
        void update(float & now);
        void draw();
        void drawDebug();
        void drawPushVector(float &thrVal);
        
        void updateFlow(ofxCv::FlowFarneback * flow);
        
        void reset();
        void trigger();
        bool isActive(){return bActive;}
        
        ofVec2f getStartPos(){return pos;}
        ofRectangle & getBlock(){return rBlock;}
        
        float defaultTTL;
        float pctFade;
        float pctGrown;
        float defaultSpeedZ;
    
        ofVec2f push;
        ofVec2f endOff;
        float pushLength;
        string text;
        int wordId;
        
        ofEvent<int> * evTrigger;
    protected:
        ofTrueTypeFont * font;
        ofRectangle rBlock;
        ofRectangle rBlockCv;
        ofVec3f pos;
        ofVec3f curPos;
        ofVec3f curSpd;
        ofVec2f curOff;
        
        ofVec2f txtOff;
        float alpha;
        float ttl;
        float startTime;
        float stopTime;
        float curScale;
        bool bActive;
    };
    
    class Verse{
    public:
        Verse(){font=NULL;curInc=0;pushLength=0;}
        void setFromStrings(vector<string> & ll, ofTrueTypeFont * _font);
        void setBlock(ofRectangle r);
        void update(float & now);
        void draw(){for(int i=0;i<words.size();i++)words[i].draw();}
        void drawDebug();
        void drawPushVectors(float &thrVal){for(int i=0;i<words.size();i++)words[i].drawPushVector(thrVal);}
        
        void updateFlow(ofxCv::FlowFarneback * flow);
        
        void trigger(){for(int i=0;i<words.size();i++)words[i].trigger();}
        bool triggerIncremental(){words[curInc].trigger();curInc++;if(curInc>=words.size()){curInc=0;return true;}else{return false;}}
        
        void setWordTTL(float val){for(int i=0;i<words.size();i++)words[i].defaultTTL=val;}
        void setPctFade(float val){for(int i=0;i<words.size();i++)words[i].pctFade=val;}
        void setPctGrown(float val){for(int i=0;i<words.size();i++)words[i].pctGrown=val;}
        void setZspeed(float val){for(int i=0;i<words.size();i++)words[i].defaultSpeedZ=val;}
        
        vector<PoetryEngine::Word> & getWords(){return words;}
        
        float getWordTTL(){return words.size()>0?words[0].defaultTTL:0;}
        float getPctFade(){return words.size()>0?words[0].pctFade:0;}
        float getPctGrown(){return words.size()>0?words[0].pctGrown:0;}
        float getZspeed(){return words.size()>0?words[0].defaultSpeedZ:0;}
        
        float pushLength;
        
    protected:
        ofTrueTypeFont * font;
        vector<PoetryEngine::Word> words;
        ofRectangle block;
        int curInc;
    };
    
    class Poem{
    public:
        Poem(){font=NULL;curInc=0;pushLength=0;}
        void loadLinesFromXml(ofxXmlSettings & xml, ofTrueTypeFont * _font);
        void setBlock(ofRectangle r);
        void update(float & now);
        void draw(){for(int i=0;i<lines.size();i++)lines[i].draw();}
        void drawDebug();
        void drawPushVectors(float &thrVal){for(int i=0;i<lines.size();i++)lines[i].drawPushVectors(thrVal);}
        
        void updateFlow(ofxCv::FlowFarneback * flow);
        
        void trigger(){for(int i=0;i<lines.size();i++)lines[i].trigger();}
        void triggerIncremental(){if(lines[curInc].triggerIncremental()){curInc++;if(curInc>=lines.size())curInc=0;}}
        
        void setWordTTL(float val){for(int i=0;i<lines.size();i++)lines[i].setWordTTL(val);}
        void setPctFade(float val){for(int i=0;i<lines.size();i++)lines[i].setPctFade(val);}
        void setPctGrown(float val){for(int i=0;i<lines.size();i++)lines[i].setPctGrown(val);}
        void setZspeed(float val){for(int i=0;i<lines.size();i++)lines[i].setZspeed(val);}
        
        vector<PoetryEngine::Verse> & getVerses(){return lines;}
        
        float getWordTTL(){return lines.size()>0?lines[0].getWordTTL():0;}
        float getPctFade(){return lines.size()>0?lines[0].getPctFade():0;}
        float getPctGrown(){return lines.size()>0?lines[0].getPctGrown():0;}
        float getZspeed(){return lines.size()>0?lines[0].getZspeed():0;}
        
        float pushLength;
        ofRectangle block;
    protected:
        ofTrueTypeFont * font;
        vector<PoetryEngine::Verse> lines;
        
        int curInc;
    };
    
    void setup(float _w, float _h);
    bool loadDictionary(string pth, int _fontSz);
    void makeBlocks();
    
    void update(float & now);
    void draw();
    void drawDebug();
    void drawPushVectors(float &thrVal);
    
    void updateFlow(ofxCv::FlowFarneback * flow);
    
    void setWordTTL(float val){for(int i=0;i<poems.size();i++)poems[i].setWordTTL(val);}
    void setPctFade(float val){for(int i=0;i<poems.size();i++)poems[i].setPctFade(val);}
    void setPctGrown(float val){for(int i=0;i<poems.size();i++)poems[i].setPctGrown(val);}
    void setZspeed(float val){for(int i=0;i<poems.size();i++)poems[i].setZspeed(val);}
    
    vector<Poem> & getPoems(){return poems;}
    
    inline float getWidth(){return sz.x;}
    inline float getHeight(){return sz.y;}
    
    float getWordTTL(){return poems.size()>0?poems[0].getWordTTL():0;}
    float getPctFade(){return poems.size()>0?poems[0].getPctFade():0;}
    float getPctGrown(){return poems.size()>0?poems[0].getPctGrown():0;}
    float getZspeed(){return poems.size()>0?poems[0].getZspeed():0;}
    
    int strongestPush;
    ofEvent<int> evTrigger;
protected:
    ofTrueTypeFont font;
    vector<Poem> poems;
    ofVec2f sz;
};