#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

  //-----------------------------------
  // XML
  //-----------------------------------
  XML.loadFile("xml/config.xml");

  //    string videoPath = ofToString(XML.getValue( "config:videoPath" ,
  //    "main.mp4"));
  string tmp = XML.getValue("config:videoPath", "main.mp4");
  string videoPath = tmp;

  int iHost = XML.getValue("config:host", 0);
  if (iHost == 1) {
    isHost = true;
  } else {
    isHost = false;
  }

  int port = XML.getValue("config:port", 8000);

  cout << videoPath << endl;
  cout << "host ? " << isHost << endl;

  //    video.load(videoPath);
  //    video.load("352x240.mp4");
  video.load("fingers.mov");
  video.play();

  video_duration = video.getDuration();
  cout << videoPath << endl;
  cout << "Duration : " << video_duration << endl;

  string adress = XML.getValue("config:ipAdress", "127.0.0.1");

  //-----------------------------------
  // OSC
  //-----------------------------------
  receiver.setup(port);
  //    sender.setup(adress, SENDER_PORT);
  sender.setup(adress, port);
}

//--------------------------------------------------------------
void ofApp::update() {
  video.update();
  int nowPos = video.getCurrentFrame();
  //cout << nowPos << " : " << video.getTotalNumFrames() << endl;
  if (isHost) {
    if (nowPos <= 10) {
      ofxOscMessage m;
      m.setAddress("/video/play");
      m.addBoolArg(true);
      sender.sendMessage(m, false);
      cout << "send Play" << endl;
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