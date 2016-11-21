//#define _WIN32_WINNT 0x0500
#include "VLCMovie.h"
#include <media_internal.h>
//#include <plugins/vlc_input_item.h>


//libvlc_instance_t *VLCMovie::libvlc = NULL;

//VLCMovie::VLCMovie(string filename) : filename(filename), frontImage(&image[1]), backImage(&image[0]), isFliped(true), isLooping(true), movieFinished(false), soundBuffer(2048 * 320), isInitialized(false) {
VLCMovie::VLCMovie(string filename) : filename(filename), frontImage(&image[1]), backImage(&image[0]), isFliped(true), isLooping(true), movieFinished(false), isInitialized(false), isVLCInitialized(false), isThumbnailOK(false), frontTexture(NULL), tryUpdate(false) {
    cout << "VLCMovie constructor" << endl;
}

VLCMovie::~VLCMovie(void)
{
    cout << "VLCMovie destructor" << endl;
	cleanupVLC();
}

void VLCMovie::init() {
    if (isInitialized) return;

    initializeVLC();
    if (!isVLCInitialized) return;

    for (int i = 0; i < 2; i++) {
        image[i].allocate(videoWidth, videoHeight, OF_IMAGE_COLOR_ALPHA);
    }

	frontTexture = &frontImage->getTextureReference();
    isInitialized = true;
}

void VLCMovie::initializeVLC() {
    //if (!libvlc) {
        cout << "init libvlc" << endl;
        char const *vlc_argv[] = {
            "--plugin-path=../../../data/",
            "--no-osd"
        };

        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
        libvlc = libvlc_new(vlc_argc, vlc_argv);
cout << "libvlc: " << libvlc << endl;
        //libvlc = libvlc_new(0, NULL);
        if (!libvlc) {
            const char *error = libvlc_errmsg();
            cout << error << endl;
            return;
        }
    //}

    libvlc_audio_output_t *aout = libvlc_audio_output_list_get(libvlc);
    while(aout) {
        cout << aout->psz_name << endl;
        cout << aout->psz_description << endl;
        aout = aout->p_next;
    }

    m = libvlc_media_new_path(libvlc, filename.c_str());
    mp = libvlc_media_player_new_from_media(m);
	//libvlc_audio_output_set(mp, "adummy");
	libvlc_audio_output_set(mp, "waveout");

    // TODO: libvlc_video_set_formatの代わりにlibvlc_video_set_format_callbacksを使う
    videoWidth = 0;
    videoHeight = 0;
  //  libvlc_video_set_callbacks(mp, NULL, NULL, NULL, this);
  //  libvlc_audio_set_callbacks(mp, NULL, NULL, NULL, NULL, NULL, this);
  //  libvlc_media_player_play(mp);

  //  while (videoWidth == 0 && videoHeight == 0) {
  //      if (!libvlc_media_player_will_play(mp))
  //          return;
  //      Sleep(100);
  //      videoWidth = libvlc_video_get_width(mp);
  //      videoHeight = libvlc_video_get_height(mp);
		//video_length_ms = libvlc_media_player_get_length(mp);
  //  }
  //  libvlc_media_player_stop(mp);
  //  libvlc_media_player_set_position(mp, 0);

    libvlc_media_parse(m);
    videoWidth = libvlc_video_get_width(mp);
    videoHeight = libvlc_video_get_height(mp);
    video_length_ms = libvlc_media_get_duration(m);
    cout << video_length_ms << endl;

	cout << "Video: (" << videoWidth << ", " << videoHeight << ")" << endl;

    libvlc_video_set_callbacks(mp, lockForThumbnailStatic, unlockForThumbnailStatic, displayForThumbnailStatic, this);
    libvlc_video_set_format(mp, "RGBA", videoWidth, videoHeight, videoWidth * 4);

	thumbnailImage.allocate(videoWidth, videoHeight, OF_IMAGE_COLOR_ALPHA);

    libvlc_media_player_play(mp);
#ifdef WIN32
    Sleep(500);
#else
    usleep(500 * 1000);
#endif

    libvlc_media_player_set_position(mp, 0.5);
#ifdef WIN32
    Sleep(500);
#else
    usleep(500 * 1000);
#endif
    
	while (!isThumbnailOK) {
#ifdef WIN32
        Sleep(100);
#else
        usleep(100 * 1000);
#endif
    }
    libvlc_media_player_stop(mp);
    libvlc_media_player_set_position(mp, 0);
	//thumbnailImage.reloadTexture();

	cout << "Created Thumbnail" << endl;
	cout << "Video:length " << video_length_ms << "(ms)" << endl;

    libvlc_audio_output_set(mp, "aout_directx");
    libvlc_video_set_callbacks(mp, lockStatic, unlockStatic, displayStatic, this);
    libvlc_video_set_format(mp, "RGBA", videoWidth, videoHeight, videoWidth * 4);
    
    //libvlc_audio_set_callbacks(mp, playStatic, pauseStatic, resumeStatic, flushStatic, drainStatic, this);
    //libvlc_audio_set_format_callbacks(mp, setupStatic, cleanupStatic);

    eventManager = libvlc_media_player_event_manager(mp);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerEndReached, vlcEventStatic,  this);

    input_item_t *it = m->p_input_item;
    
    for (int i = 0; i < it->i_es; i++) {
        es_format_t *es = it->es[i];
        if (es) {
            if (es->video.i_frame_rate) {
                fps = (float)es->video.i_frame_rate / es->video.i_frame_rate_base;
                cout << "fps: " << fps << endl;
            }
            //cout << es->video.i_width << endl;
        }
    }

    isVLCInitialized = true;
}

void VLCMovie::cleanupVLC() {
    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);
    libvlc_media_release(m);
    libvlc_release(libvlc);
}

void VLCMovie::play() {
    if (isLooping) {
        libvlc_media_add_option(m, "input-repeat=-1");
    } else {
        libvlc_media_add_option(m, "input-repeat=0");
    }
    movieFinished = false;

    libvlc_media_player_play(mp);

}

void VLCMovie::rewind() {
    libvlc_media_player_set_position(mp, 0);
}

void VLCMovie::stop() {

    libvlc_media_player_stop(mp);
}

void VLCMovie::seek(float position) {
    libvlc_media_player_set_position(mp, position);
}

void *VLCMovie::lockStatic(void *data, void **p_pixels) {
    return ((VLCMovie *)data)->lock(p_pixels);
}

void VLCMovie::unlockStatic(void *data, void *id, void *const *p_pixels) {
    ((VLCMovie *)data)->unlock(id, p_pixels);
}

void VLCMovie::displayStatic(void *data, void *id) {
    ((VLCMovie *)data)->display(id);

}

void *VLCMovie::lockForThumbnailStatic(void *data, void **p_pixels) {
    return ((VLCMovie *)data)->lockForThumbnail(p_pixels);
}

void VLCMovie::unlockForThumbnailStatic(void *data, void *id, void *const *p_pixels) {
    ((VLCMovie *)data)->unlockForThumbnail(id, p_pixels);
}

void VLCMovie::displayForThumbnailStatic(void *data, void *id) {
    ((VLCMovie *)data)->displayForThumbnail(id);

}

//void VLCMovie::playStatic(void *data, const void *samples, unsigned int count, int64_t pts) {
//    ((VLCMovie *)data)->play(samples, count, pts);
//}
//
//void VLCMovie::pauseStatic(void *data, int64_t pts) {
//    cout << "*****************pause*****************" << endl;
//}
//
//void VLCMovie::resumeStatic(void *data, int64_t pts) {
//    cout << "*****************resume*****************" << endl;
//}
//
//void VLCMovie::flushStatic(void *data, int64_t pts) {
//    cout << "*****************flush*****************" << endl;
//}
//
//void VLCMovie::drainStatic(void *data) {
//    cout << "*****************drain*****************" << endl;
//}
//
//int VLCMovie::setupStatic(void **data, char *format, unsigned int *rate, unsigned int *channels) {
//    return ((VLCMovie *)*data)->setup(format, rate, channels);
//}
//
//void VLCMovie::cleanupStatic(void *data) {
//    cout << "*****************cleanup*****************" << endl;
//}

void VLCMovie::vlcEventStatic(const libvlc_event_t *event, void *data) {
    ((VLCMovie *)data)->vlcEvent(event);
}

void VLCMovie::vlcEvent(const libvlc_event_t *event) {
    if (event->type == libvlc_MediaPlayerEndReached) {
        movieFinished = true;
    }
}

//void VLCMovie::play(const void *samples, unsigned int count, int64_t pts) {
//    //cout << "play: samples=" << samples << " count=" << count << " pts=" << pts << endl;
//    //soundBuffer.writeBuffer((short *)samples, pts, count * audioChannels);
//
//}
//
//int VLCMovie::setup(char *format, unsigned int *rate, unsigned int *channels) {
//    // 音声セットアップ
//    //printf("channels: %d rate: %d format: %As\n", *channels, *rate, format); 
//    audioChannels = *channels;
//    //soundStream.setup(*channels, 0, *rate, 2048, 8);
//    //soundStream.setOutput(this);
//    return 0;
//}


void *VLCMovie::lock(void **p_pixels) {
    while (tryUpdate) {
#ifdef WIN32
        Sleep(10);
#else
        usleep(10 * 1000);
#endif
    }
    //backImageMutex.lock(10000);
    //imageFlipMutex.lock(10000);
    *p_pixels = backImage->getPixels().getData();
    //cout << "lock " << *p_pixels << endl;
    //imageFlipMutex.unlock();
    return NULL;
}



void VLCMovie::unlock(void *id, void *const *p_pixels) {
    //backImageMutex.unlock();
}

void VLCMovie::display(void *id) {
    while (tryUpdate) {
#ifdef WIN32
        Sleep(10);
#else
        usleep(10 * 1000);
#endif
    }

    //imageFlipMutex.lock(10000);
    if (imageFlipMutex.tryLock()) {
//cout << "flip" << endl;
        ofImage *tmp = backImage;
        backImage = frontImage;
        frontImage = tmp;
        isFliped = true;
        imageFlipMutex.unlock();
    } 
    //imageFlipMutex.unlock();
}

void *VLCMovie::lockForThumbnail(void **p_pixels) {
    *p_pixels = thumbnailImage.getPixels().getData();
    return NULL;
}

void VLCMovie::unlockForThumbnail(void *id, void *const *p_pixels) {
}

void VLCMovie::displayForThumbnail(void *id) {
	isThumbnailOK = true;
}

unsigned int VLCMovie::getImageWidth() {
    return videoWidth;
}

unsigned int VLCMovie::getImageHeight() {
    return videoHeight;
}

void VLCMovie::updateTexture() {
    if (!isFliped) return;
    tryUpdate = true;
    //cout << "try" << endl;
    imageFlipMutex.lock(10000);
    //cout << "update" << endl;
    frontImage->update();
    frontTexture = &frontImage->getTextureReference();
    isFliped = false;
    imageFlipMutex.unlock();
    tryUpdate = false;
    //cout << libvlc_video_get_width(mp) << endl;
    //cout << libvlc_video_get_height(mp) << endl;

}

ofTexture &VLCMovie::getTexture() {
    return *frontTexture;
}

////--------------------------------------------------------------
//void VLCMovie::audioOut(float *input, int bufferSize, int nChannels) {
//    //cout << "bufSize=" << bufferSize << " channel=" << nChannels << endl;
//    //cout << "readBuffer=" << soundBuffer.readBuffer(input) << endl;
//    int64_t ts;
//    //soundBuffer.readBuffer(input, &ts, bufferSize * nChannels);
//
//}

void VLCMovie::setLoop(bool isLooping) {
    this->isLooping = isLooping;
}

bool VLCMovie::isMovieFinished() {
    return movieFinished;
}

bool VLCMovie::isPlaying() {
    return libvlc_media_player_is_playing(mp);
}

bool VLCMovie::getIsInitialized() {
    return isInitialized;
}

float VLCMovie::getPosition() {
	return libvlc_media_player_get_position(mp);
}

ofImage &VLCMovie::getThumbnailImage() {
	return thumbnailImage;
}

libvlc_time_t VLCMovie::getTimeMillis(){
	return libvlc_media_player_get_time(mp);
}

void VLCMovie::setTimeMillis(libvlc_time_t ms){
	libvlc_media_player_set_time(mp, ms);
}

float VLCMovie::getFPS() {
    return fps;
}

float VLCMovie::getDuration() {
    return video_length_ms / fps;
}

void VLCMovie::setFrame(int frame) {
    libvlc_time_t ms = 1000 * frame / fps;
    setTimeMillis(ms);
}

int VLCMovie::getCurrentFrame() {
    libvlc_time_t ms = getTimeMillis();
    int frame = fps * ms / 1000;
    return frame;
}

int VLCMovie::getTotalNumFrames() {
    return fps * video_length_ms / 1000;
}

void VLCMovie::setVolume(int volume) {
    libvlc_audio_set_volume(mp, volume);
}

void VLCMovie::toggleMute() {
    libvlc_audio_toggle_mute(mp);
}
