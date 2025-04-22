#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cwchar>
#include "spu2.h"
#include <aaudio/AAudio.h>
#include <android/log.h>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <vector>

#define LOG_TAG "SPU2"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace spu2 {

static AAudioStream* stream = nullptr;
static std::mutex bufferMutex;
static std::queue<std::vector<uint8_t>> audioQueue;
static std::atomic<bool> running(false);
static int32_t sampleRate = 0;
static int32_t channels = 0;
static int32_t frameSize = 0;

aaudio_data_callback_result_t dataCallback(
    AAudioStream* stream,
    void* userData,
    void* audioData,
    int32_t numFrames) {

    std::lock_guard<std::mutex> lock(bufferMutex);
    if (!audioQueue.empty()) {
        auto& buffer = audioQueue.front();
        memcpy(audioData, buffer.data(), std::min<int32_t>(buffer.size(), numFrames * frameSize));
        audioQueue.pop();
    } else {
        LOGE("Audio underflow!");
        memset(audioData, 0, numFrames * frameSize);
    }
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

bool init(int32_t rate, int32_t ch, int32_t bufferSizeInFrames) {
    if (stream) return false;

    sampleRate = rate;
    channels = ch;
    frameSize = sizeof(int16_t) * ch;

    AAudioStreamBuilder* builder;
    AAudio_createStreamBuilder(&builder);

    AAudioStreamBuilder_setSampleRate(builder, rate);
    AAudioStreamBuilder_setChannelCount(builder, ch);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_EXCLUSIVE);
    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
    AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    AAudioStreamBuilder_setDataCallback(builder, dataCallback, nullptr);

    aaudio_result_t result = AAudioStreamBuilder_openStream(builder, &stream);
    if (result != AAUDIO_OK) {
        LOGE("Failed to open stream: %s", AAudio_convertResultToText(result));
        return false;
    }

    result = AAudioStream_requestStart(stream);
    if (result != AAUDIO_OK) {
        LOGE("Failed to start stream: %s", AAudio_convertResultToText(result));
        return false;
    }

    running.store(true);
    LOGI("Audio stream started: %d Hz, %d channels", rate, ch);
    AAudioStreamBuilder_delete(builder);
    return true;
}

void writeSamples(const uint16_t* pcmData, int32_t numSamples) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    std::vector<uint8_t> buffer(reinterpret_cast<const uint8_t*>(pcmData),
                                reinterpret_cast<const uint8_t*>(pcmData) + numSamples * sizeof(uint16_t));
    audioQueue.push(std::move(buffer));
}

void shutdown() {
    if (stream) {
        running.store(false);
        AAudioStream_requestStop(stream);
        AAudioStream_close(stream);
        stream = nullptr;
        LOGI("Audio stream shut down");
    }
}
}