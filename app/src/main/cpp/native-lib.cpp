// native-lib.cpp
// Compilação com suporte a C++17 e otimizações recomendadas (ex.: -O2)
// Este arquivo contém a implementação inicial do núcleo do emulador "SuperPS2"
// e serve como interface entre o código Kotlin/Android e a camada nativa via JNI.

#include <jni.h>
#include <string>
#include <exception>
#include "ps2engine/ps2engine.h"
#include "osd/osd.h"
#include "emulation/options/engine_options.h"
#include "emulation/input/input_manager.h"
#include <android/log.h>
#include "core/ps2_core.h"

// Define macros para logging no Android
#define LOG_TAG "SuperPS2-CPP"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define TAG "NativeEntry"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

/*
 * Função auxiliar para inicializar os módulos de CPU, GPU, áudio e I/O.
 * Esta função é um stub inicial, que poderá ser expandido com
 * otimizações específicas para processadores Snapdragon (ex.: uso de NEON).
 */
bool initializeEmulatorModules() {
    try {
        // Inicialização do módulo de CPU
        // TODO: Implementar inicialização da CPU e otimizações de processamento paralelo.
        LOGI("Inicializando módulo de CPU...");

        // Inicialização do módulo de GPU
        // TODO: Configurar renderização gráfica e usar instruções específicas para aceleração via hardware.
        LOGI("Inicializando módulo de GPU...");

        // Inicialização do módulo de Áudio
        // TODO: Configurar sincronização de áudio e qualidade de som.
        LOGI("Inicializando módulo de Áudio...");

        // Inicialização dos módulos de I/O e outros periféricos
        // TODO: Preparar os subsistemas de entrada/saída e realizar configurações iniciais.
        LOGI("Inicializando módulos de I/O...");

        // Placeholder para inicialização de outros recursos necessários
        LOGI("Todos os módulos foram inicializados com sucesso.");

        // Se todas as inicializações ocorrerem sem problemas, retorna true.
        return true;
    }
    catch (const std::exception &e) {
        // Em caso de exceção, registre o erro e retorne false para indicar falha na inicialização.
        LOGE("Exceção na inicialização do emulador: %s", e.what());
        return false;
    }
    catch (...) {
        LOGE("Erro desconhecido na inicialização do emulador.");
        return false;
    }
}

/*
 * Função auxiliar para carregar e preparar recursos da emulação.
 * Pode incluir carregamento de ROMs, imagens, sons, entre outros.
 * Atualmente é um stub para futuras implementações.
 */
void loadEmulatorResources() {
    // TODO: Adicionar o carregamento e preparação dos recursos de emulação.
    LOGI("Carregando recursos do emulador...");
}

/*
 * Função auxiliar para atualizar os parâmetros de emulação em tempo real.
 * Essa função poderá ser expandida para receber novos parâmetros a partir da UI (via JNI).
 */
void updateEmulatorParameters() {
    // TODO: Implementar atualização de parâmetros, como configurações de desempenho e gráficos.
    LOGI("Atualizando parâmetros do emulador...");
}

/*
 * Função auxiliar para liberar os recursos e limpar alocações feitas pela camada nativa.
 * Deve ser chamada quando a Activity ou o emulador for finalizado.
 */
void releaseEmulatorResources() {
    // TODO: Liberar recursos, encerrar threads, e liberar memória alocada.
    LOGI("Liberando recursos do emulador...");
}

/*
 * A função nativa initEmulator, chamada a partir da MainActivity via JNI,
 * serve para inicializar o núcleo do emulador. A função retorna JNI_TRUE se 
 * a inicialização for bem-sucedida, caso contrário, retorna JNI_FALSE.
 *
 * Observação:
 * - A convenção de nomenclatura segue o padrão JNI: Java_<package>_<Class>_<method>.
 * - Caso o pacote utilizado na MainActivity seja diferente, ajuste o nome da função
 *   de acordo com o nome completo da Activity.
 */
extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_MainActivity_initEmulator(JNIEnv* env, jobject instance) {
    LOGI("Chamando initEmulator via JNI...");

    // Exemplo: Carregar recursos necessários para a emulação
    loadEmulatorResources();

    // Inicializa os módulos do emulador
    bool initialized = initializeEmulatorModules();

    // Se necessário, atualizar parâmetros do emulador de forma dinâmica
    if (initialized) {
        updateEmulatorParameters();
    }

    // Retorna JNI_TRUE caso a inicialização tenha sido bem-sucedida
    return initialized ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_NativeBridge_startEmulation(JNIEnv *env, jobject, jstring isoPath) {
    const char *path = env->GetStringUTFChars(isoPath, 0);

    __android_log_print(ANDROID_LOG_INFO, "SuperPS2", "Iniciando emulação: %s", path);

    // Em um prompt futuro: chamar a engine real aqui
    // bool success = ps2core_start(path);

    env->ReleaseStringUTFChars(isoPath, path);

    return JNI_TRUE; // Temporariamente sempre retorna sucesso
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_NativeBridge_startEmulation(JNIEnv *env, jobject, jstring isoPath) {
    const char *path = env->GetStringUTFChars(isoPath, 0);
    std::string isoPathStr(path);
    env->ReleaseStringUTFChars(isoPath, path);

    bool success = ps2engine::start(isoPathStr);
    return static_cast<jboolean>(success);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_NativeBridge_startEmulation(JNIEnv *env, jobject, jstring isoPath) {
    const char *path = env->GetStringUTFChars(isoPath, 0);
    std::string isoPathStr(path);
    env->ReleaseStringUTFChars(isoPath, path);
    return static_cast<jboolean>(ps2engine::start(isoPathStr));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_superps2_emu_core_NativeBridge_setSurface(JNIEnv *env, jobject, jobject surface) {
    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    if (ps2renderer::init(window)) {
        std::thread renderThread([]() {
            ps2renderer::renderLoop();
        });
        renderThread.detach();
    }
}

/*
 * Outras funções nativas podem ser declaradas e implementadas a seguir,
 * tais como funções para atualizar configurações de desempenho, alterar gráficos
 * em tempo real ou liberar recursos ao finalizar a Activity.
 *
 * Por exemplo:
 *
 * extern "C" JNIEXPORT void JNICALL
 * Java_com_superps2_emu_MainActivity_releaseEmulator(JNIEnv* env, jobject instance) {
 *     releaseEmulatorResources();
 * }
 *
 * Essas funções devem seguir a mesma convenção e poderão ser invocadas conforme necessário.
 */

#include "audio/spu2.h"

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_NativeAudio_initAudio(JNIEnv*, jobject, jint sr, jint ch, jint bs) {
    return spu2::init(sr, ch, bs);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_NativeAudio_writeAudio(JNIEnv* env, jobject, jshortArray data, jint num) {
    jshort* pcm = env->GetShortArrayElements(data, nullptr);
    spu2::writeSamples(reinterpret_cast<uint16_t*>(pcm), num);
    env->ReleaseShortArrayElements(data, pcm, 0);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_NativeAudio_shutdownAudio(JNIEnv*, jobject) {
    spu2::shutdown();
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_OSDBridge_initOSD(JNIEnv*, jobject, jdouble targetFps) {
    osd::init(targetFps);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_OSDBridge_notifyFrame(JNIEnv*, jobject) {
    osd::notifyFrameRendered();
}

extern "C" JNIEXPORT jdouble JNICALL
Java_com_superps2_emu_core_OSDBridge_getFps(JNIEnv*, jobject) {
    return osd::getFps();
}

extern "C" JNIEXPORT jdouble JNICALL
Java_com_superps2_emu_core_OSDBridge_getSpeed(JNIEnv*, jobject) {
    return osd::getEmulationSpeed();
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_OSDBridge_shutdownOSD(JNIEnv*, jobject) {
    osd::shutdown();
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_EngineSettingsBridge_setResolutionScale(JNIEnv*, jobject, jfloat scale) {
    options::setResolutionScale(scale);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_EngineSettingsBridge_setFrameLimit(JNIEnv*, jobject, jint fps) {
    options::setFrameLimit(fps);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_EngineSettingsBridge_setVSyncEnabled(JNIEnv*, jobject, jboolean en) {
    options::setVSyncEnabled(en);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_EngineSettingsBridge_setMultiThreadEnabled(JNIEnv*, jobject, jboolean en) {
    options::setMultiThreadEnabled(en);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_core_EngineSettingsBridge_setNeonOptimizations(JNIEnv*, jobject, jboolean en) {
    options::setNeonOptimizations(en);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_superps2_emu_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "SuperPS2 Native Initialized!";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_sendButtonEvent(JNIEnv*, jobject, jint code, jboolean pressed) {
    input::setButtonState(code, pressed == JNI_TRUE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_NativeInput_onAxisEvent(JNIEnv*, jobject, jint axis, jfloat value) {
    input::setAxisValue(axis, value);
}

#include "graphics/gl_renderer.h"
#include "emulation/gs/renderer/vertex_buffer.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_superps2_emu_graphics_GLRenderer_nativeInit(JNIEnv* env, jobject thiz) {
    LOGI("Initializing native GL renderer...");
    if (!graphics::glRenderer.init()) {
        LOGI("Failed to initialize OpenGL renderer");
    } else {
        LOGI("OpenGL renderer initialized successfully.");
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_superps2_emu_graphics_GLRenderer_nativeRender(JNIEnv* env, jobject thiz) {
    renderer::VertexBuffer buffer;
    
    // Teste temporário: triângulo de exemplo
    buffer.addVertex({{0.0f,  0.5f, 0.0f}, {255, 0, 0, 255}});
    buffer.addVertex({{-0.5f, -0.5f, 0.0f}, {0, 255, 0, 255}});
    buffer.addVertex({{0.5f, -0.5f, 0.0f}, {0, 0, 255, 255}});

    graphics::glRenderer.render(buffer);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_superps2_emu_graphics_GLRenderer_nativeShutdown(JNIEnv* env, jobject thiz) {
    LOGI("Shutting down GL renderer...");
    graphics::glRenderer.shutdown();
}

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_EmulatorBridge_startEmulation(JNIEnv *env, jobject /* this */, jstring isoPath) {
    const char *nativePath = env->GetStringUTFChars(isoPath, nullptr);
    std::string pathStr(nativePath);
    env->ReleaseStringUTFChars(isoPath, nativePath);

    bool success = ps2::initializeCore(pathStr);
    if (success) {
        ps2::launchEmulationThread();
    }
    return success ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_EmulatorBridge_stopEmulation(JNIEnv *env, jobject /* this */) {
    LOGI("Recebido comando para parar emulação");
    ps2::shutdown();
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_EmulatorBridge_pauseEmulation(JNIEnv *env, jobject /* this */) {
    ps2::pause();
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_EmulatorBridge_resumeEmulation(JNIEnv *env, jobject /* this */) {
    ps2::resume();
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_EmulatorBridge_resetEmulation(JNIEnv *env, jobject /* this */) {
    ps2::reset();
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_EmulatorBridge_saveState(JNIEnv* env, jobject /* this */, jstring savePath) {
    const char* path = env->GetStringUTFChars(savePath, nullptr);
    bool result = ps2::saveStateToFile(path);
    env->ReleaseStringUTFChars(savePath, path);
    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_EmulatorBridge_loadState(JNIEnv* env, jobject /* this */, jstring savePath) {
    const char* path = env->GetStringUTFChars(savePath, nullptr);
    bool result = ps2::loadStateFromFile(path);
    env->ReleaseStringUTFChars(savePath, path);
    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_EmulatorBridge_shutdownEmulation(JNIEnv *env, jobject /* this */) {
    ps2::shutdown();
    return JNI_TRUE;
}

} // extern "C"