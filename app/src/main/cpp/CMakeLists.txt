# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name.
project(superps2)

# Configurações do Android
set(ANDROID_NDK ${ANDROID_NDK})
set(ANDROID_ABI ${ANDROID_ABI})
set(ANDROID_PLATFORM android-24)

# Definições necessárias para tipos fixos e JNI
add_definitions(
    -D__STDC_LIMIT_MACROS
    -D__STDC_CONSTANT_MACROS
    -DJNIEXPORT=__attribute__((visibility("default")))
)

# Configurar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir diretórios
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${ANDROID_NDK}/sources/android/native_app_glue
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++abi/include
    ${ANDROID_NDK}/sysroot/usr/include
    ${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include
    ${CMAKE_SOURCE_DIR}/audio
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/emulation
    ${CMAKE_SOURCE_DIR}/graphics
    ${CMAKE_SOURCE_DIR}/ps2engine
    ${CMAKE_SOURCE_DIR}/utils
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

# Linkar com bibliotecas necessárias
target_link_libraries(superps2
    android
    log
    EGL
    GLESv2
)

# Opções de compilação
target_compile_options(superps2 PRIVATE
    -Wall
    -Wextra
    -fexceptions
    -frtti
    -fPIC
)