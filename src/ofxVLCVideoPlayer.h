#pragma once

#include <memory>
#include "VLCMovie.h"

class ofxVLCVideoPlayer
{
    shared_ptr<VLCMovie> vlcMovieInstance;
    ofTexture dummyTexture;
	ofImage dummyImage;
public:
    ofxVLCVideoPlayer(void);
    ~ofxVLCVideoPlayer(void);
    bool loadMovie(string name);
    void closeMovie();
    void update();
    ofTexture &getTextureReference();
	ofImage &getThumbnailImage();
    void draw(float x, float y, float w, float h);
    void draw(float x, float y);
    void play();
    void stop();
    bool getIsMovieDone();
    void setPosition(float pct);
    void setLoop(bool loop);
    float getHeight();
    float getWidth();
    bool isPlaying();
	bool isLoaded();
	float getPosition();
	int getTimeMillis();
	void setTimeMillis(int ms);
    float getFPS();
    float getDuration();
    void setFrame(int frame);
    int getCurrentFrame();
    int getTotalNumFrames();
    void setVolume(int volume);
    void toggleMute();
};

