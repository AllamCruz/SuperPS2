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

# Definições necessárias para tipos fixos
add_definitions(
    -D__STDC_LIMIT_MACROS
    -D__STDC_CONSTANT_MACROS
    -D__STDC_FORMAT_MACROS
)

# Configurar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de compilação
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -frtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden")

# Incluir diretórios
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/emulation
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/input
    ${CMAKE_CURRENT_SOURCE_DIR}/jni
    ${CMAKE_CURRENT_SOURCE_DIR}/osd
    ${CMAKE_CURRENT_SOURCE_DIR}/ps2engine
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${ANDROID_NDK}/sources/android/native_app_glue
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++abi/include
    ${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include
    ${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/android
)

# Coletar todos os arquivos .cpp recursivamente
file(GLOB_RECURSE SOURCES 
    "*.cpp"
    "audio/*.cpp"
    "core/*.cpp"
    "emulation/*.cpp"
    "graphics/*.cpp"
    "ps2engine/*.cpp"
    "utils/*.cpp"
    "jni/*.cpp"
)

# Criar biblioteca compartilhada
add_library(superps2 SHARED ${SOURCES})

# Localiza as bibliotecas necessárias
find_library(log-lib log)
find_library(android-lib android)
find_library(glesv3-lib GLESv3)
find_library(egl-lib EGL)

# Vincular bibliotecas
target_link_libraries(superps2
    ${log-lib}
    ${android-lib}
    ${glesv3-lib}
    ${egl-lib}
)

# Opções de compilação
target_compile_options(superps2 PRIVATE
    -Wall
    -Wextra
    -fexceptions
    -frtti
    -fPIC
)

set_target_properties(superps2 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
    VISIBILITY_INLINES_HIDDEN ON
    CXX_VISIBILITY_PRESET hidden
)