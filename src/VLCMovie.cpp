#define _WIN32_WINNT 0x0500
#include "VLCMovie.h"
#include <lib/media_internal.h>
#include <vlc/plugins/vlc_input_item.h>

//VLCMovie::VLCMovie(string filename) : filename(filename), frontImage(&image[1]), backImage(&image[0]), isFliped(true), isLooping(true), movieFinished(false), soundBuffer(2048 * 320), isInitialized(false) {
VLCMovie::VLCMovie(string filename) : filename(filename), frontImage(&image[1]), backImage(&image[0]), isFliped(true), isLooping(true), movieFinished(false), isInitialized(false), isVLCInitialized(false), isThumbnailOK(false), frontTexture(NULL) {
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
    char const *vlc_argv[] = {
        "--no-osd"
    };

    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
    libvlc = libvlc_new(vlc_argc, vlc_argv);
    //libvlc = libvlc_new(0, NULL);
    if (!libvlc) {
        const char *error = libvlc_errmsg();
        cout << error << endl;
        return;
    }

    // *** list all audio output names ***
    //libvlc_audio_output_t *aout = libvlc_audio_output_list_get(libvlc);
    //while(aout) {
    //    cout << aout->psz_name << endl;
    //    cout << aout->psz_description << endl;
    //    aout = aout->p_next;
    //}

    m = libvlc_media_new_path(libvlc, filename.c_str());
    mp = libvlc_media_player_new_from_media(m);
    
    // *** if you want to change how to output audio, please change setting below
    // *** you can get appreciate audio output name by libvlc_audio_output_list_get (code wrote and commented-out 10 lines above)
    // libvlc_audio_output_set(mp, "adummy"); // audio output to buffer (need to setup callbacks with libvlc_audio_set_callbacks)
    libvc_audio_output_set(mp, "waveout"); // audio output to MME Wave Out
    // libvlc_audio_output_set(mp, "aout_directx"); // audio output with Direct X
    

    // TODO: use libvlc_video_set_format_callbacks instead of libvlc_video_set_format
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
	while (!isThumbnailOK) {
        Sleep(100);
    }
    libvlc_media_player_stop(mp);
    libvlc_media_player_set_position(mp, 0);
	thumbnailImage.reloadTexture();

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

void VLCMovie::pause() {
    libvlc_media_player_pause(mp);
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
    backImageMutex.lock(10000);
    imageFlipMutex.lock(10000);
    *p_pixels = backImage->getPixels();
    imageFlipMutex.unlock();
    return NULL;
}



void VLCMovie::unlock(void *id, void *const *p_pixels) {
    backImageMutex.unlock();
}

void VLCMovie::display(void *id) {
    imageFlipMutex.lock(10000);
    ofImage *tmp = backImage;
    backImage = frontImage;
    frontImage = tmp;
    isFliped = true;
    imageFlipMutex.unlock();
}

void *VLCMovie::lockForThumbnail(void **p_pixels) {
	*p_pixels = thumbnailImage.getPixels();
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
    imageFlipMutex.lock(10000);
    frontImage->update();
    frontTexture = &frontImage->getTextureReference();
    isFliped = false;
    imageFlipMutex.unlock();
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
    return video_length_ms / 1000;
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