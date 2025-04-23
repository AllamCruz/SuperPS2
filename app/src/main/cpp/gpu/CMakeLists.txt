# Módulo GPU
add_library(gpu STATIC
    gl_renderer.cpp
)

target_include_directories(gpu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(gpu
    common
    ${glesv2-lib}
    ${egl-lib}
) 