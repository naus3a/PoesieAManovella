//
//  SoundInput.h
//  InteractivePoetry
//
//  Created by enrico<naus3a>viola on 8/7/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxProcessFFT.h"

class SoundInput{
public:
    class BandArgs{
    public:
        int bandId;
        float val;
    };
    
    class ValueListener{
    public:
        ValueListener();
        ~ValueListener();
        void setup(ProcessFFT * _fft, float * _val, fftRangeType fftTp);
        void setAutoUpdate(bool b);
        void update();
        
        ofEvent<float> ev;
    protected:
        void autoUpdate(ofEventArgs & e){update();}
        
        ProcessFFT * fft;
        std::function<float(void)>getterFunc;
        float * val;
        bool bAutoUpdate;
    };
    
    class BandListener{
    public:
        BandListener(){fft=NULL;thr=NULL;bActive=true;}
        void setup(ProcessFFT * _fft, float * _thr){fft=_fft;thr=_thr;}
        void update();
        void setActive(bool b){bActive=b;}
        bool iActive(){return bActive;}
        ofEvent<BandArgs> evLoudestBand;
    protected:
        ProcessFFT * fft;
        float * thr;
        bool bActive;
    };
    
    SoundInput();
    void update();
    void drawDebug();
    
    int getNumBands(){fft.getSpectrum().size();}
    
    void setActive(bool b);
    bool isActive(){return bActive;}
    
    template<typename T>
    void addListener(void(T::*func)(), T * listener, fftRangeType fftTp);
    
    template<typename T>
    void removeListener(void(T::*func)(), T * listener, fftRangeType fftTp);
    
    float thrLow, thrMid, thrHigh, thrVol;
    float thrBand;
    
    BandListener bandListener;
    
protected:
    void drawChartThreshold(float _x, float _y, float & val);
    
    ValueListener vlLow;
    ValueListener vlMid;
    ValueListener vlHig;
    ValueListener vlVol;
    
    ProcessFFT fft;
    bool bActive;
};