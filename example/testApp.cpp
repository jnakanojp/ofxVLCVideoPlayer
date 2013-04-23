#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	counter = 0;
	ofSetCircleResolution(50);
	ofBackground(255,255,255);
	bSmooth = false;
	ofSetWindowTitle("ofxVLCVideoPlayer example");

	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.

    player.loadMovie("test.mp4");
    player.play();

    ofxVLCVideoPlayer testPlayer;
    testPlayer.loadMovie("test.mp4");
    testImage = testPlayer.getThumbnailImage();
}

//--------------------------------------------------------------
void testApp::update(){
    player.update();
    player2.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    player.draw(640,0);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}