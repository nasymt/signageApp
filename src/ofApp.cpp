#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofHideCursor();
    
    //-----------------------------------
    // XML
    //-----------------------------------
    XML.loadFile("xml/config.xml");
    
    //    string videoPath = ofToString(XML.getValue( "config:videoPath" ,
    //    "main.mp4"));
 
    XML.pushTag("config");
    string videoPath = XML.getValue("videoPath", "main.mp4");;
    
    int iHost = XML.getValue("host", 0);
    if (iHost == 1) {
        isHost = true;
    } else {
        isHost = false;
    }
    
    cout << videoPath << endl;
    cout << "host: " << isHost << endl;
    
    video.load("fingers.mov");
    video.play();
    
    int isMute = XML.getValue("mute",0);
    if ( isMute == 1 ){
        video.setVolume(0);
        cout << "-- audio mute --" << endl;
    }
    
    video_duration = video.getDuration();
    cout << videoPath << endl;
    cout << "Duration: " << video_duration << endl;
    
    
    
//    string address = XML.getValue("config:ipAddress", "127.0.0.1");
//    int port = XML.getValue("config:port", 8000);
    string address = XML.getValue("ipAddress", "127.0.0.1");
    port = XML.getValue("port", 8000);
    
    clientNum = XML.getValue("num", 0);
    cout << "client num: " << clientNum << endl;
    for (int i = 0; i < clientNum ; i++){
        string str = XML.getValue("ipAddress", "127.0.0.1" , i);
        clientIpAddress.push_back(str);
        cout << str << " : " << port << endl;
    }
    
    XML.popTag();
    
    //-----------------------------------
    // OSC
    //-----------------------------------
    receiver.setup(port);
    sender.setup(address, port);
}

//--------------------------------------------------------------
void ofApp::update() {
    video.update();
    int nowPos = video.getCurrentFrame();
    //cout << nowPos << " : " << video.getTotalNumFrames() << endl;
    if (isHost) {
        if (nowPos <= 10) {
            for (int i=0; i < clientNum; i++){
                sender.setup(clientIpAddress[i], port);
                ofxOscMessage m;
                m.setAddress("/video/play");
                m.addBoolArg(true);
                sender.sendMessage(m, false);
                cout << "send Play" << endl;
            }
        }
        
    } else {
        bool bPlay;
        int now = ofGetElapsedTimeMillis();
        while (receiver.hasWaitingMessages()) {
            ofxOscMessage m;
            receiver.getNextMessage(m);
            if (m.getAddress() == "/video/play") {
                bPlay = m.getArgAsBool(0);
            }
        }
        if (bPlay && !isPlaying) {
            video.firstFrame();
            isPlaying = true;
            resetTime = now + 3000;
        }
        if (isPlaying && now > resetTime) {
            isPlaying = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() { video.draw(0, 0, ofGetWidth(), ofGetHeight()); }

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if (key == 'a') {
        video.firstFrame();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}