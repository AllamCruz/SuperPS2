# Módulo CPU
add_library(cpu STATIC
    ee_core.cpp
    gpr.cpp
    pc.cpp
    decoder.cpp
    interpreter.cpp
)

target_include_directories(cpu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cpu
    common
) 