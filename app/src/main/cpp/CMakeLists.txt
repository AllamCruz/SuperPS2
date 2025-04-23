cmake_minimum_required(VERSION 3.22.1)
project("superps2")

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(superps2 SHARED
    ${SOURCE_FILES}
)

target_include_directories(superps2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_library(log-lib log)
find_library(android-lib android)
find_library(gles-lib GLESv3)

target_link_libraries(superps2
    ${android-lib}
    ${log-lib}
    ${gles-lib}
)
