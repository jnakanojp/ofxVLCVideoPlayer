#include "ofxVLCVideoPlayer.h"

//using shared_ptr;

ofxVLCVideoPlayer::ofxVLCVideoPlayer(void)
{
}


ofxVLCVideoPlayer::~ofxVLCVideoPlayer(void)
{
    //closeMovie();
}

bool ofxVLCVideoPlayer::loadMovie(string name) {
    closeMovie();
    vlcMovieInstance = shared_ptr<VLCMovie>(new VLCMovie(ofToDataPath(name)));
    vlcMovieInstance->init();
    bool result = vlcMovieInstance->getIsInitialized();
    if (!result) vlcMovieInstance.reset();

    return result;
}

void ofxVLCVideoPlayer::closeMovie() {
    if (vlcMovieInstance) {
        vlcMovieInstance->stop();
        vlcMovieInstance.reset();
    }
}

void ofxVLCVideoPlayer::update() {
    if (vlcMovieInstance) {
        vlcMovieInstance->updateTexture();
    }
} 

ofTexture &ofxVLCVideoPlayer::getTextureReference() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getTexture();
    } else {
        return dummyTexture;
    }
}

void ofxVLCVideoPlayer::draw(float x, float y, float w, float h) {
    if (vlcMovieInstance) {
        vlcMovieInstance->getTexture().draw(x, y, 0, w, h);
    }
}

void ofxVLCVideoPlayer::draw(float x, float y) {
    if (vlcMovieInstance) {
        vlcMovieInstance->getTexture().draw(x, y);
    }
}

void ofxVLCVideoPlayer::play() {
    if (vlcMovieInstance) {
        vlcMovieInstance->play();
    }
}

void ofxVLCVideoPlayer::stop() {
    if (vlcMovieInstance) {
        vlcMovieInstance->stop();
    }
}

bool ofxVLCVideoPlayer::getIsMovieDone() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->isMovieFinished();
    } else {
        return true;
    }
}

void ofxVLCVideoPlayer::setPosition(float pct) {
    if (vlcMovieInstance) {
        vlcMovieInstance->seek(pct);
    }
}

void ofxVLCVideoPlayer::setLoop(bool loop) {
    if (vlcMovieInstance) {
        vlcMovieInstance->setLoop(loop);
    }
}

float ofxVLCVideoPlayer::getHeight() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getImageHeight();
    } else {
        return 0;
    }
}

float ofxVLCVideoPlayer::getWidth() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getImageWidth();
    } else {
        return 0;
    }
}

bool ofxVLCVideoPlayer::isPlaying() {
   if (vlcMovieInstance) {
        return vlcMovieInstance->isPlaying();
   } else {
       return false;
   }
}

bool ofxVLCVideoPlayer::isLoaded() {
   if (vlcMovieInstance) {
        return vlcMovieInstance->getIsInitialized();
   } else {
       return false;
   }
}

float ofxVLCVideoPlayer::getPosition() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getPosition();
    } else {
        return 0;
    }
}

ofImage &ofxVLCVideoPlayer::getThumbnailImage() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getThumbnailImage();
    } else {
        return dummyImage;
    }
}

int ofxVLCVideoPlayer::getTimeMillis(){
	if(vlcMovieInstance){
		return vlcMovieInstance->getTimeMillis();
	}
	else{
		return -1;
	}
}

void ofxVLCVideoPlayer::setTimeMillis(int ms){
	if(vlcMovieInstance){
		vlcMovieInstance->setTimeMillis(ms);
	}
}

float ofxVLCVideoPlayer::getFPS() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getFPS();
    } else {
        return -1;
    }
}

void ofxVLCVideoPlayer::setFrame(int frame) {
	if(vlcMovieInstance){
		return vlcMovieInstance->setFrame(frame);
	}
}

int ofxVLCVideoPlayer::getCurrentFrame() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getCurrentFrame();
    } else {
        return -1;
    }
}

int ofxVLCVideoPlayer::getTotalNumFrames() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getTotalNumFrames();
    } else {
        return -1;
    }
}

float ofxVLCVideoPlayer::getDuration() {
    if (vlcMovieInstance) {
        return vlcMovieInstance->getDuration();
    } else {
        return -1;
    }
}

void ofxVLCVideoPlayer::setVolume(int volume) {
    if (vlcMovieInstance) {
        vlcMovieInstance->setVolume(volume);
    }
}

void ofxVLCVideoPlayer::toggleMute() {
    if (vlcMovieInstance) {
        vlcMovieInstance->toggleMute();
    }
}
