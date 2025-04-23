cmake_minimum_required(VERSION 3.22.1)
project(superps2-app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(android_native_app_glue STATIC
    ${ANDROID_NDK_HOME}/sources/android/native_app_glue/android_native_app_glue.c)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp
)

add_subdirectory(src/main/cpp)
