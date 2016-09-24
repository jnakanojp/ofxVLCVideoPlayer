#pragma once

#include "ofMain.h"
#ifdef WIN32
#define LIBVLC_USE_PTHREAD_CANCEL
typedef SSIZE_T ssize_t;
#endif
#include <vlc/vlc.h>
#include <memory>

// TODO: スタートフレーム、エンドフレーム
//class VLCMovie : public AbstractMovie, public ofBaseSoundOutput
class VLCMovie
{
    string filename;
    ofImage image[2];
    ofImage *frontImage;
    ofImage *backImage;
    ofTexture *frontTexture;
	ofImage thumbnailImage;

    //static libvlc_instance_t *libvlc;
    libvlc_instance_t *libvlc;
    libvlc_media_t *m;
    libvlc_media_player_t *mp;
    libvlc_event_manager_t *eventManager;

    void initializeVLC();
    void cleanupVLC();

    bool isInitialized;
    bool isVLCInitialized;

    int videoWidth, videoHeight;
    float fps;
	libvlc_time_t video_length_ms;
	bool isThumbnailOK;
    bool isFliped;
    bool isLooping;
    bool tryUpdate;

    bool movieFinished;


    Poco::Mutex backImageMutex;
    Poco::Mutex imageFlipMutex;

    // VLC Video callbaks
    static void *lockStatic(void *data, void **p_pixels);
    static void unlockStatic(void *data, void *id, void *const *p_pixels);
    static void displayStatic(void *data, void *id);

	static void *lockForThumbnailStatic(void *data, void **p_pixels);
    static void unlockForThumbnailStatic(void *data, void *id, void *const *p_pixels);
    static void displayForThumbnailStatic(void *data, void *id);

	void *lock(void **p_pixels);
    void unlock(void *id, void *const *p_pixels);
    void display(void *id);

	void *lockForThumbnail(void **p_pixels);
    void unlockForThumbnail(void *id, void *const *p_pixels);
    void displayForThumbnail(void *id);

    //// VLC Audio callbacks
    //static void playStatic(void *data, const void *samples, unsigned int count, int64_t pts);
    //static void pauseStatic(void *data, int64_t pts);
    //static void resumeStatic(void *data, int64_t pts);
    //static void flushStatic(void *data, int64_t pts);
    //static void drainStatic(void *data);
    //static int setupStatic(void **data, char *format, unsigned int *rate, unsigned int *channels);
    //static void cleanupStatic(void *data);
    //void play(const void *samples, unsigned int count, int64_t pts);
    //void pause(int64_t pts);
    //void resume(int64_t pts);
    //void flush(int64_t pts);
    //void drain();
    //int setup(char *format, unsigned int *rate, unsigned int *channels);
    //void cleanup();

    // VLC Event callbacks
    static void vlcEventStatic(const libvlc_event_t *event, void *data);
    void vlcEvent(const libvlc_event_t *event);

public:
    VLCMovie(string filename);
    ~VLCMovie(void);
    void init();
    void play();
    void rewind();
    void stop();
    void seek(float position);
    void setLoop(bool isLooping);
    unsigned int getImageWidth();
    unsigned int getImageHeight();
    float getFPS();
    float getDuration();
    int getTotalNumFrames();
    void updateTexture();
    ofTexture &getTexture();
	ofImage &getThumbnailImage();
    bool isMovieFinished();
    bool isPlaying();
    bool getIsInitialized();
	float getPosition();
	libvlc_time_t getTimeMillis();
    void setTimeMillis(libvlc_time_t ms);
    void setFrame(int frame);
    int getCurrentFrame();
    void setVolume(int volume);
    void toggleMute();

    //ofSoundStream soundStream;
    //SoundBuffer soundBuffer;
    //virtual void audioOut(float *output, int bufferSize, int channels);
};
