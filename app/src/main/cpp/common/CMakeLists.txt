# Módulo Common
add_library(common STATIC
    memory.cpp
    state.cpp
    engine_options.cpp
)

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
) 