#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    ofVideoPlayer video;
    
    ofxXmlSettings XML;
    
    int video_duration;
    bool isHost;
    int resetTime;
    bool isPlaying;
    int clientNum;
    int port;
    
    vector<string> clientIpAddress;
    
};
