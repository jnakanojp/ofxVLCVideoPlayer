#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255,255,255);
    ofSetWindowTitle("ofxVLCVideoPlayer example");
    
    ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    
    player.loadMovie("test.mp4");
    player.play();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    player.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    player.draw(0,0);
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
    
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
