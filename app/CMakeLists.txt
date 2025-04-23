cmake_minimum_required(VERSION 3.22.1)

# Define o nome do projeto
project("superps2")

# Configura o C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona os diretórios de include
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/gpu
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/input
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/memory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/bios
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/utils
)

# Lista todos os arquivos fonte
file(GLOB_RECURSE SOURCES
    "src/main/cpp/*.cpp"
    "src/main/cpp/core/*.cpp"
    "src/main/cpp/gpu/*.cpp"
    "src/main/cpp/input/*.cpp"
    "src/main/cpp/audio/*.cpp"
    "src/main/cpp/memory/*.cpp"
    "src/main/cpp/bios/*.cpp"
    "src/main/cpp/utils/*.cpp"
)

# Cria a biblioteca nativa
add_library(${CMAKE_PROJECT_NAME} SHARED
    ${SOURCES}
)

# Encontra as bibliotecas necessárias
find_library(log-lib log)
find_library(android-lib android)
find_library(gles3-lib GLESv3)

# Linka as bibliotecas
target_link_libraries(${CMAKE_PROJECT_NAME}
    ${log-lib}
    ${android-lib}
    ${gles3-lib}
)
