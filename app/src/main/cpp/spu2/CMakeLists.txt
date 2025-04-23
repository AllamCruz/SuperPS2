# Módulo SPU2
add_library(spu2 STATIC
    spu2.cpp
)

target_include_directories(spu2 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spu2
    common
) 