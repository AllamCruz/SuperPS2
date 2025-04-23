# Módulo JNI
add_library(jni STATIC
    StateBridge.cpp
    ps2_jni_bridge.cpp
    input_bridge.cpp
    emulator_bridge.cpp
    native-audio-bridge.cpp
)

target_include_directories(jni PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${JNI_INCLUDE_DIRS}
)

target_link_libraries(jni
    common
    cpu
    spu2
    gpu
) 