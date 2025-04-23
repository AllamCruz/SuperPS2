# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Versão mínima do CMake requerida para o projeto
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

# Incluir submódulos
add_subdirectory(common)
add_subdirectory(cpu)
add_subdirectory(spu2)
add_subdirectory(gpu)
add_subdirectory(jni)

# Criar biblioteca compartilhada
add_library(superps2 SHARED
    ps2engine.cpp
)

# Localiza as bibliotecas necessárias
find_library(log-lib log)
find_library(android-lib android)
find_library(egl-lib EGL)
find_library(glesv2-lib GLESv2)

# Adiciona zlib para cálculo de CRC32
find_library(ZLIB_LIB z)

# Vincular bibliotecas
target_link_libraries(superps2
    ${log-lib}
    ${android-lib}
    ${egl-lib}
    ${glesv2-lib}
    ${ZLIB_LIB}
    android_native_app_glue
    common
    cpu
    spu2
    gpu
    jni
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

target_include_directories(superps2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ANDROID_NDK}/sources/android/native_app_glue
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++abi/include
    ${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include
    ${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/lib64/clang/14.0.7/include
    ${ANDROID_NDK}/sources/android/native_app_glue/include
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include
    ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++abi/include
    ${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include
    ${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/lib64/clang/14.0.7/include
)