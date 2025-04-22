#include <jni.h>
#include <android/log.h>
#include <oboe/Oboe.h>
#include "spu2/spu2.h"

#define LOG_TAG "NativeAudio"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class AudioEngine : public oboe::AudioStreamCallback {
private:
    oboe::AudioStream *stream = nullptr;
    bool isInitialized = false;

public:
    bool init(int32_t sampleRate, int32_t channels, int32_t bufferSize) {
        if (isInitialized) {
            LOGI("Audio already initialized");
            return true;
        }

        oboe::AudioStreamBuilder builder;
        builder.setDirection(oboe::Direction::Output)
               ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
               ->setSharingMode(oboe::SharingMode::Exclusive)
               ->setFormat(oboe::AudioFormat::I16)
               ->setChannelCount(channels)
               ->setSampleRate(sampleRate)
               ->setBufferCapacityInFrames(bufferSize)
               ->setCallback(this);

        oboe::Result result = builder.openStream(&stream);
        if (result != oboe::Result::OK) {
            LOGE("Failed to create audio stream: %s", oboe::convertToText(result));
            return false;
        }

        result = stream->requestStart();
        if (result != oboe::Result::OK) {
            LOGE("Failed to start audio stream: %s", oboe::convertToText(result));
            return false;
        }

        isInitialized = true;
        LOGI("Audio initialized: %d Hz, %d channels, %d frames", 
             sampleRate, channels, bufferSize);
        return true;
    }

    void shutdown() {
        if (!isInitialized) return;

        if (stream) {
            stream->requestStop();
            stream->close();
            stream = nullptr;
        }

        isInitialized = false;
        LOGI("Audio shutdown");
    }

    oboe::DataCallbackResult onAudioReady(
            oboe::AudioStream *audioStream,
            void *audioData,
            int32_t numFrames) override {
        
        if (!isInitialized) return oboe::DataCallbackResult::Stop;

        // Get audio samples from SPU2
        int16_t* outputBuffer = static_cast<int16_t*>(audioData);
        spu2::generateSamples(outputBuffer, numFrames);

        return oboe::DataCallbackResult::Continue;
    }
};

static AudioEngine audioEngine;

extern "C" {

JNIEXPORT void JNICALL
Java_com_superps2_emu_NativeAudio_initAudio(
        JNIEnv* env,
        jclass clazz,
        jint sampleRate,
        jint channels,
        jint bufferSize) {
    audioEngine.init(sampleRate, channels, bufferSize);
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_NativeAudio_shutdownAudio(
        JNIEnv* env,
        jclass clazz) {
    audioEngine.shutdown();
}

} // extern "C" 