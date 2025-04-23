# Versão mínima do CMake requerida para o projeto. 
# Utilizamos a versão 3.22.1 para garantir compatibilidade com recursos modernos.
cmake_minimum_required(VERSION 3.22.1)

# Define o nome do projeto
project(superps2)

# Configurar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definições necessárias
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-DJNIEXPORT=__attribute__\(\(visibility\(\"default\"\)\)\))

# Configuração do Android
set(ANDROID_NDK ${ANDROID_NDK})
set(ANDROID_ABI ${ANDROID_ABI})
set(ANDROID_PLATFORM android-26)

# Adicionar biblioteca android_native_app_glue
add_library(android_native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Incluir diretórios
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ANDROID_NDK}/sources/android/native_app_glue
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++abi/include
    ${ANDROID_NDK}/sysroot/usr/include
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/emulation
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/ps2engine
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# Coletar arquivos fonte
file(GLOB SOURCES 
    "*.cpp"
    "*.c"
    "audio/*.cpp"
    "core/*.cpp"
    "emulation/*.cpp"
    "graphics/*.cpp"
    "ps2engine/*.cpp"
    "utils/*.cpp"
)

# Criar biblioteca compartilhada
add_library(superps2 SHARED ${SOURCES})

# Localiza as bibliotecas necessárias
find_library(log-lib log)
find_library(android-lib android)
find_library(egl-lib EGL)
find_library(glesv2-lib GLESv2)
find_library(oboe-lib oboe)

# Adiciona zlib para cálculo de CRC32
find_package(ZLIB REQUIRED)

# Vincular bibliotecas
target_link_libraries(superps2
    ${log-lib}
    ${android-lib}
    ${egl-lib}
    ${glesv2-lib}
    ${oboe-lib}
    ZLIB::ZLIB
    android_native_app_glue
)

# Opções de compilação
target_compile_options(superps2 PRIVATE
    -Wall
    -Wextra
    -fexceptions
    -frtti
    -fPIC
)

# Inclui os arquivos fonte do projeto
target_sources(superps2 PRIVATE
    src/main/cpp/ps2engine/ps2engine.cpp
    src/main/cpp/emulation/ps2core.cpp
    src/main/cpp/emulation/bios/bios.cpp
    src/main/cpp/emulation/memory/memory.cpp
    src/main/cpp/emulation/cpu/ee_core.cpp
    src/main/cpp/emulation/dma/dma.cpp
    src/main/cpp/emulation/vif/vif.cpp
    src/main/cpp/emulation/gif/gif.cpp
    src/main/cpp/emulation/state/state.cpp
    src/main/cpp/emulation/memorycard/memorycard.cpp
    src/main/cpp/emulation/cheats/cheat.cpp
    src/main/cpp/jni/StateBridge.cpp
    src/main/cpp/osd/osd.cpp
    src/main/cpp/emulation/options/engine_options.cpp
    src/main/cpp/emulation/vu0/vu0.cpp
    src/main/cpp/emulation/vu1/vu1.cpp
    src/main/cpp/emulation/scratchpad/scratchpad.cpp
    src/main/cpp/emulation/iop/iop.cpp
    src/main/cpp/emulation/ee/interrupt.cpp
    src/main/cpp/emulation/spu2/spu2.cpp
    src/main/cpp/emulation/ee/memory/ram.cpp
    src/main/cpp/emulation/ee/memory/mem_mainram.cpp
    src/main/cpp/emulation/ee/memory/memory.cpp
    src/main/cpp/emulation/ee/cop0/cop0.cpp
    src/main/cpp/emulation/ee/cpu/ee_core.cpp
    src/main/cpp/emulation/ee/vu/vu0.cpp
    src/main/cpp/emulation/ee/vu/vu1.cpp
    src/main/cpp/emulation/ee/cpu/gpr.cpp
    src/main/cpp/emulation/ee/cpu/pc.cpp
    src/main/cpp/emulation/ee/cpu/decoder.cpp
    src/main/cpp/emulation/ee/cpu/interpreter.cpp
    src/main/cpp/jni/ps2_jni_bridge.cpp
    src/main/cpp/renderer/renderer.cpp
    src/main/cpp/jni/input_bridge.cpp
    src/main/cpp/jni/emulator_bridge.cpp
    src/main/cpp/emulation/input/input_manager.cpp
    src/main/cpp/graphics/gl_renderer.cpp
    src/main/cpp/audio/spu2.cpp
    src/main/cpp/jni/native-audio-bridge.cpp
)

# Incluir diretórios adicionais
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/ps2engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/emulation/bios)